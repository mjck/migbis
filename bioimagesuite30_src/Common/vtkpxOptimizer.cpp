//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
//BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
//BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
//BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
//BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
//BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
//BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//BIOIMAGESUITE_LICENSE  See also  http://www.gnu.org/licenses/gpl.html
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
//BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
//BIOIMAGESUITE_LICENSE 
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


//
//
//
//  This File contains the following Numerical Recipes derived code that
//   These are replaced if  BIS_USE_NR is not defined by ITK Code
//   Good for testing for now, later eliminate all NR Code Completely
// ---------------------------------------------------------------------------------------------------------

// vtk includes

#include "pxutil.h"
#include "vtkpxOptimizer.h"
#include "vtkObjectFactory.h"
#include "vcl_algorithm.h"
#include <vnl/vnl_math.h>
#include "bioimagesuite_settings.h"
#include "vtkTimerLog.h"

#define TOL 2.0e-4


char vtkpxOptimizer::OutputStatus[400];

vtkpxOptimizer* vtkpxOptimizer::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxOptimizer");
  if(ret)
    {
      return (vtkpxOptimizer*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxOptimizer;
}


vtkpxOptimizer::vtkpxOptimizer()
{
  this->MasterObject=NULL;
  this->FeedbackMethod=NULL;
  this->InternalEvaluateFunction=NULL;
  this->InternalBeginIteration=NULL;
  this->InternalEvaluateGradient=NULL;
  this->TempArray=NULL;
  this->pcom=NULL;
  this->xicom=NULL;
  this->xtemp=NULL;
  this->ResetStatistics();
}

vtkpxOptimizer::~vtkpxOptimizer()
{
  this->SetTempArray(NULL);
  this->SetMasterObject(NULL);
  this->CleanUpLineTempCommons();
}

//----------------------------------------------------------------------------
//  Hook Functions
//----------------------------------------------------------------------------
void vtkpxOptimizer::SetFeedbackMethod(void (*f)(vtkObject*,const char*,double ))
{
  this->FeedbackMethod=f;
}

void vtkpxOptimizer::SetEvaluateFunctionMethod(double (*f)(vtkObject*,vtkDoubleArray* ))
{
  this->InternalEvaluateFunction=f;
}

void vtkpxOptimizer::SetBeginIterationMethod(double (*f)(vtkObject*,vtkDoubleArray* ))
{
  this->InternalBeginIteration=f;
}


void vtkpxOptimizer::SetEvaluateGradientMethod(double (*f)(vtkObject*,vtkDoubleArray*,vtkDoubleArray* ))
{
  this->InternalEvaluateGradient=f;
}


void vtkpxOptimizer::ResetStatistics()
{
  this->NumberOfFunctionEvaluations=0;
  this->NumberOfGradientEvaluations=0;
  this->MinFunctionValue=1e+30;
}
// ------------------------------------------------------------------------------
// Support Functions
// ------------------------------------------------------------------------------

void vtkpxOptimizer::AddScaledVectors(vtkDoubleArray* a1,double s1,vtkDoubleArray* a2,double s2,vtkDoubleArray* sum)
{
  int np=sum->GetNumberOfTuples();
  for (int i=0;i<np;i++)
    sum->SetComponent(i,0,
		      s1*a1->GetComponent(i,0)+
		      s2*a2->GetComponent(i,0));
}

void vtkpxOptimizer::AddToVector(vtkDoubleArray* position,vtkDoubleArray* grad,double factor)
{
  int np=position->GetNumberOfTuples();
  for (int i=0;i<np;i++)
    position->SetComponent(i,0,position->GetComponent(i,0)+factor*grad->GetComponent(i,0));

}

void vtkpxOptimizer::ScaleVector(vtkDoubleArray* position,double scale)
{
  int np=position->GetNumberOfTuples();
  for (int i=0;i<np;i++)
    position->SetComponent(i,0,position->GetComponent(i,0)*scale);
}

int vtkpxOptimizer::UseParameter(vtkShortArray* flag,int p)
{
  if (flag==NULL)
    return 1;

  return (flag->GetComponent(p,0)>0);
}

void vtkpxOptimizer::GenerateOutput(const char* prefix1,const char* prefix2,
				    vtkDoubleArray* position,double measure,int iter,vtkShortArray* flags)
{
  sprintf(this->OutputStatus,"%s:%s %2d : ",prefix1,prefix2,iter);
  if (position->GetNumberOfTuples()<=25)
    {
      sprintf(this->OutputStatus,"%s (",this->OutputStatus);
      for (int ii=0; ii<position->GetNumberOfTuples(); ii++) 
	{
	  if (this->UseParameter(flags,ii))
	    {
	      double a=position->GetComponent(ii,0);
	      if (position->GetNumberOfComponents()==2)
		a=a/position->GetComponent(ii,1);
	      sprintf(this->OutputStatus,"%s %-.5f ",this->OutputStatus,a);
	    }
	}
      sprintf(this->OutputStatus,"%s )",this->OutputStatus);
    }
  if (this->FeedbackMethod!=NULL)
    this->FeedbackMethod(this->MasterObject,this->OutputStatus,measure);

}


void vtkpxOptimizer::GenerateStatistics(const char* method,vtkDoubleArray* position)
{
  double v=this->EvaluateFunction(this->MasterObject,position);
  sprintf(this->OutputStatus,"%s Stats: Nfuncr=%d, Ngrad=%d Minv=%.4f ReturnedV=%.4f",
	  method,
	  this->NumberOfFunctionEvaluations-1,
	  this->NumberOfGradientEvaluations,
	  this->MinFunctionValue,
	  v);
  if (this->FeedbackMethod!=NULL)
    this->FeedbackMethod(this->MasterObject,this->OutputStatus,v);
}

//----------------------------------------------------------------------------
double vtkpxOptimizer::ComputeOldGradientDescent(vtkDoubleArray* position,double step,double tolerance,int maxsteps)
{
  if (this->MasterObject==NULL || this->InternalEvaluateFunction==NULL || this->InternalEvaluateGradient==NULL)
    {
      vtkErrorMacro(<<"Hook Functions Not Properly Defined\n");
    }

  vtkDoubleArray* grad=NULL;
  if (this->TempArray==NULL)
      grad=vtkDoubleArray::New();
  else
      grad=this->TempArray;


  double old_similarity=this->EvaluateFunction(this->MasterObject,position);
  this->EvaluateGradient(this->MasterObject,position,grad);
  double dstep=-1.0*step;
  this->GenerateOutput("oGD", "Beginning",position,old_similarity,0);

  int upd=1;
  double best=old_similarity;
  int flag=1;
  
  int count=0,maxcount=maxsteps;
  while (flag==1 && count<maxcount)
    {  
      ++count;
      AddToVector(position,grad,dstep);
      double similarity = this->EvaluateFunction(this->MasterObject,position);
      if (similarity<best)
	{
	  best=similarity;
	  this->GenerateOutput("oGD", "Iter",position,best,upd);
	  ++upd;
	}
      
      else
	flag=0;
    }
  
  // Last step was no improvement, so back track
  if (flag==0)
    AddToVector(position,grad,-dstep);
  
  this->GenerateOutput("oGD", "Done",position,best,upd);
  this->GenerateStatistics("oGD",position);

  if (this->TempArray==NULL)
      grad->Delete();
  return best;
}
//----------------------------------------------------------------------------
double vtkpxOptimizer::ComputeGradientDescent(vtkDoubleArray* position,int iterations,double tolerance)
{
  if (this->MasterObject==NULL || this->InternalEvaluateFunction==NULL || this->InternalEvaluateGradient==NULL)
    {
      vtkErrorMacro(<<"Hook Functions Not Properly Defined\n");
    }

  vtkDoubleArray* grad=NULL;
  if (this->TempArray==NULL)
      grad=vtkDoubleArray::New();
  else
      grad=this->TempArray;

  this->AllocateLineTempCommons(position);
  this->BeginIteration(this->MasterObject,position);  
  double old_similarity=this->EvaluateFunction(this->MasterObject,position);
  this->EvaluateGradient(this->MasterObject,position,grad);
  this->GenerateOutput("GD", "Beginning",position,old_similarity,0);

  double best=old_similarity;
  int flag=1;
  int iter=1;
  
  while (flag==1 && iter<iterations)
    {  
      this->BeginIteration(this->MasterObject,position);
      best=this->LineMinimization(position,grad,iter,tolerance,"GD");
      if (fabs(best-old_similarity)<tolerance)
	{
	  flag=0;
	}
      else
	{
	  old_similarity=this->EvaluateFunction(this->MasterObject,position);
	  this->EvaluateGradient(this->MasterObject,position,grad);
	  ++iter;
	}
    }

  this->GenerateOutput("GD", "Done",position,best,iter);
  this->GenerateStatistics("GD",position);
  this->CleanUpLineTempCommons();

  if (this->TempArray==NULL)
      grad->Delete();
  return best;
}
//----------------------------------------------------------------------------
double vtkpxOptimizer::ComputeSlowClimb(vtkDoubleArray* position,double step,int iterations,vtkShortArray* flags)
{
  if (this->MasterObject==NULL || this->InternalEvaluateFunction==NULL)
    {
      vtkErrorMacro(<<"Hook Functions Not Properly Defined\n");
    }

  this->BeginIteration(this->MasterObject,position);  
  double old_similarity=this->EvaluateFunction(this->MasterObject,position),similarity=old_similarity;
  int   nump=position->GetNumberOfTuples();

  char line[20];   sprintf(line,"SC %.5f",step);
  this->GenerateOutput(line, "Beginning",position,old_similarity,0);

  for (int iter=1;iter<iterations;iter++)
    {
      int j = 0,k = 0;
      double best_similarity=old_similarity;
      this->BeginIteration(this->MasterObject,position);

      for (int i = 0; i < nump; i++)
	{
	  if (this->UseParameter(flags,i))
	    {
	      double orig=position->GetComponent(i,0);
	      position->SetComponent(i,0, orig + step);
	      similarity=this->EvaluateFunction(this->MasterObject,position);
	      if (similarity < best_similarity)
		{
		  best_similarity = similarity;
		  j =  i;
		  k =  1;
		}
	      
	      position->SetComponent(i,0, orig - step);
	      similarity=this->EvaluateFunction(this->MasterObject,position);
	      
	      if (similarity < best_similarity)
		{
		  best_similarity = similarity;
		  j =  i;
		  k = -1; 
		}
	      position->SetComponent(i,0, orig);
	    }
	}

      if (best_similarity < old_similarity)
	{
	  double oldv=position->GetComponent(j,0);
	  double newv=oldv+k*step;
	  position->SetComponent(j,0,newv);
	  old_similarity=best_similarity;
	  this->GenerateOutput(line, "It",position,old_similarity,iter);
	}
      else
	iter=iterations+1; // Break Out
    }
  this->GenerateStatistics(line,position);
  return old_similarity;
}


double vtkpxOptimizer::ComputeSlowClimbPlus(vtkDoubleArray* position,int iterations,vtkShortArray* flags)
{
  if (this->MasterObject==NULL || this->InternalEvaluateFunction==NULL)
    {
      vtkErrorMacro(<<"Hook Functions Not Properly Defined\n");
    }

  this->BeginIteration(this->MasterObject,position);  
  double old_similarity=this->EvaluateFunction(this->MasterObject,position),similarity=old_similarity;
  int   nump=position->GetNumberOfTuples();

  char line[20];   sprintf(line,"SC ");
  this->GenerateOutput(line, "Beginning",position,old_similarity,0);

  for (int iter=1;iter<iterations;iter++)
    {
      int j = 0,k = 0;
      double best_similarity=old_similarity;
      this->BeginIteration(this->MasterObject,position);

      for (int i = 0; i < nump; i++)
	{
	  if (this->UseParameter(flags,i))
	    {
	      double orig=position->GetComponent(i,0);
	      double step=position->GetComponent(i,1);
	      double newp=orig+step;
	      if (newp>position->GetComponent(i,3))
		newp=position->GetComponent(i,3);

	      position->SetComponent(i,0, newp);
	      similarity=this->EvaluateFunction(this->MasterObject,position);
	      if (similarity < best_similarity)
		{
		  best_similarity = similarity;
		  j =  i;
		  k =  1;
		}

	      newp=orig-step;
	      if (newp<position->GetComponent(i,2))
		newp=position->GetComponent(i,2);

	      position->SetComponent(i,0, newp);
	      similarity=this->EvaluateFunction(this->MasterObject,position);
	      
	      if (similarity < best_similarity)
		{
		  best_similarity = similarity;
		  j =  i;
		  k = -1; 
		}
	      position->SetComponent(i,0, orig);
	    }
	}

      if (best_similarity < old_similarity)
	{
	  double oldv=position->GetComponent(j,0);
	  double step=position->GetComponent(j,1);
	  double newv=oldv+k*step;
	  if (newv<position->GetComponent(j,2))
	    newv=position->GetComponent(j,2);
	  else if (newv>position->GetComponent(j,3))
	    newv=position->GetComponent(j,3);
	  position->SetComponent(j,0,newv);
	  old_similarity=best_similarity;
	  this->GenerateOutput(line, "It",position,old_similarity,iter);
	}
      else
	iter=iterations+1; // Break Out
    }
  this->GenerateStatistics(line,position);
  return old_similarity;
}



// ---------------------------------------------------------------------------
double vtkpxOptimizer::ComputeConjugateGradient(vtkDoubleArray* position, int num_iterations,double tolerance)
{
  /*  int j,its=0;
  double gg,gam,fp,dgg;
  double fret;*/

  vtkTimerLog* timer=vtkTimerLog::New();
  timer->StartTimer();

  this->AllocateLineTempCommons(position);
  int n=position->GetNumberOfTuples();
  vtkDoubleArray* grad=vtkDoubleArray::New();  grad->SetNumberOfTuples(n);


  char line[60];
  sprintf(line,"Begin Optimization (it=%d), tolerance=%f",num_iterations,tolerance);
  this->GenerateOutput("GC",line,position,0,0);

  this->BeginIteration(this->MasterObject,position);  
  double funcval=this->EvaluateFunction(this->MasterObject,position);
  double norm=this->EvaluateGradient(this->MasterObject,position,grad);





  int done=0;

  if (norm<tolerance)
    {
      this->GenerateOutput("CG", "Done",position,funcval,0);  
      this->GenerateStatistics("CG",position);
      grad->Delete();this->CleanUpLineTempCommons();
      return funcval;
    }

  vtkDoubleArray* g=vtkDoubleArray::New();  g->SetNumberOfTuples(n);
  vtkDoubleArray* h=vtkDoubleArray::New();  h->SetNumberOfTuples(n);

  g->DeepCopy(grad); this->ScaleVector(g,-1.0); 
  h->DeepCopy(g);  grad->DeepCopy(h);

  int iter=1;
  while (iter<=num_iterations && done == 0 )
    {
      this->BeginIteration(this->MasterObject,position);
      double newval=this->LineMinimization(position,grad,iter,tolerance,"CG");
      done=1;
      if (fabs(funcval-newval)>tolerance)
	{
	  funcval=this->EvaluateFunction(this->MasterObject,position);
	  this->EvaluateGradient(this->MasterObject,position,grad);
	  double dgg=0.0,gg=0.0;
	  for (int j=0;j<n;j++) 
	    {
	      gg += g->GetComponent(j,0)*g->GetComponent(j,0);
	      dgg += (grad->GetComponent(j,0)+g->GetComponent(j,0))*grad->GetComponent(j,0);
	    }
	  
	  if (gg > 0.000001) 
	    {
	      double gam=dgg/gg;
	      g->DeepCopy(grad); this->ScaleVector(g,-1.0);
	      this->ScaleVector(h,gam); this->AddToVector(h,g,1.0);
	      grad->DeepCopy(h);
	      done=0;
	    }
	}
      ++iter;
    }
  
  timer->StopTimer();
  double elapsed=timer->GetElapsedTime();
  timer->Delete();

  sprintf(line,"Done (%10.4f s) \t\t",elapsed);

  this->GenerateOutput("CG", line,position,funcval,iter);  
  this->GenerateStatistics("CG",position);
  g->Delete();h->Delete();grad->Delete();this->CleanUpLineTempCommons();
  return funcval;
}

#define SQR(a)  ((sqrarg=(a)) = 0.0 ? 0.0 : sqrarg*sqrarg)
// ---------------------------------------------------------------------------
double vtkpxOptimizer::ComputePowell(vtkDoubleArray* position,vtkDoubleArray* temp, int iterations,double tolerance)
{
#ifdef BIS_USE_NR
  static double sqrarg;
  int n=position->GetNumberOfTuples();
  if (temp->GetNumberOfComponents()<n || temp->GetNumberOfTuples()<n)
    {
      temp->SetNumberOfComponents(n);
      temp->SetNumberOfTuples(n);
      for (int i=0;i<n;i++)
	temp->FillComponent(i,0.0);
      for (int j=0;j<n;j++)
	temp->SetComponent(j,j,1.0);
    }
  vtkDoubleArray* pt=vtkDoubleArray::New(); pt->SetNumberOfTuples(n);
  vtkDoubleArray* ptt=vtkDoubleArray::New(); ptt->SetNumberOfTuples(n);
  vtkDoubleArray* xit=vtkDoubleArray::New(); xit->SetNumberOfTuples(n);
  this->AllocateLineTempCommons(position);

  //This code derives from Numerical Recipes in C
  //It computes a powell optimization
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

  pt->Delete();
  ptt->Delete();
  xit->Delete();
  this->CleanUpLineTempCommons();
  fret=this->EvaluateFunction(this->MasterObject,position);
  this->GenerateOutput("PW", "Done",position,fret,iter);
  this->GenerateStatistics("PW",position);
  return fret;
#else
  // No Numerical Recipes So reroute to Gradient Descent for now
  fprintf(stderr,"Powell unavailable using Slow Climb Descent\n");
  return vtkpxOptimizer::ComputeSlowClimb(position,0.1,iterations);
#endif
}
#undef SQR  

//----------------------------------------------------------------------------
// Numerical Recipes code for optimization
// ---------------------------------------------------------------------------

void ShiftValues(double* a, double* b, double* c, double d)
{
  *a = *b;
  *b = *c;
  *c = d;
}


#define GOLD 1.618034
#define GLIMIT 100.0
#define TINY 1.0e-20
// Bracketing Routine 

void vtkpxOptimizer::BracketMinimum(double *ax, 
			    double *bx, 
			    double *cx, 
			    double *fa, 
			    double *fb, 
			    double *fc)
{
#ifdef BIS_USE_NR
  //This code derives from Numerical Recipes in C
  // It is practically unmodified
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

#else
  // Replacement code from ITK 3.4 (vnl_brent)
  double ulim,u,r,q,fu;

  *fa=this->LineFunction(*ax);
  *fb=this->LineFunction(*bx);
  if (*fb > *fa) {
    vcl_swap(*ax, *bx);
    vcl_swap(*fa, *fb);
  }
  *cx=(*bx)+GOLD*(*bx-*ax);
  *fc=this->LineFunction(*cx);
  while (*fb > *fc) {
    r=(*bx-*ax)*(*fb-*fc);
    q=(*bx-*cx)*(*fb-*fa);
    double dq = q-r;
    if (vcl_abs(dq) < TINY)
      dq = vnl_math_sgn(dq) * TINY;

    u=(*bx)-((*bx-*cx)*q-(*bx-*ax)*r)/(2.0*dq);
    ulim=(*bx)+GLIMIT*(*cx-*bx);
    if ((*bx-u)*(u-*cx) > 0.0) {
      fu=this->LineFunction(u);
      if (fu < *fc) {
        *ax=(*bx);
        *bx=u;
        *fa=(*fb);
        *fb=fu;
        return;
      } else if (fu > *fb) {
        *cx=u;
        *fc=fu;
        return;
      }
      u=(*cx)+GOLD*(*cx-*bx);
      fu=this->LineFunction(u);
    } else if ((*cx-u)*(u-ulim) > 0.0) {
      fu=this->LineFunction(u);
      if (fu < *fc) {
        //ShiftValues(bx,cx,&u,*cx+GOLD*(*cx-*bx)); awf dumped -- c is useless
        ShiftValues(bx,cx,&u,u+GOLD*(u-*cx));
        ShiftValues(fb,fc,&fu,this->LineFunction(u));
      }
    } else if ((u-ulim)*(ulim-*cx) >= 0.0) {
      u=ulim;
      fu=this->LineFunction(u);
    } else {
      u=(*cx)+GOLD*(*cx-*bx);
      fu=this->LineFunction(u);
    }
    ShiftValues(ax,bx,cx,u);
    ShiftValues(fa,fb,fc,fu);
  }
#endif
  
}
#undef GOLD
#undef GLIMIT
#undef TINY

// ---------------------------------------------------------------------------
#define ITMAX 100
#define CGOLD 0.3819660
#define ZEPS 1.0e-10


double vtkpxOptimizer::MinimizeGivenBounds(double ax, double bx, double cx,  double tol,  double *xmin)
{
#ifdef BIS_USE_NR
  //This code derives from Numerical Recipes in C
  // It is practically unmodified
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

#else
  // Replacement code from ITK 3.4 (vnl_brent)

  //
  // This code has been removed after it was pointed out
  // to ITK developers that it was a copy of code from
  // Numerical Recipies. January 23 2007
  //
  // The distribution license of numerical recipies is not
  // compatible with the BSD-License used by ITK.
  // 
  // -----------------------------------------------------------
  //
  // The following implementation was based on the description
  // of the Brent's method presented in the Wikipedia:
  //
  //    http://en.wikipedia.org/wiki/Brent%27s_method
  //
  double a = ax;
  double b = cx;

  bool mflag;

  if( ax > cx )
    {
    a = cx;
    b = ax;
    }

  double x = bx;

  double fa = this->LineFunction(a);
  double fb = this->LineFunction(b);
  double fx = this->LineFunction(x);

  if( fa * fb >= 0.0 )
    {
      //    fprintf(stderr,"vnl_brent f(%f) has same sign as f(%f) \n",a,b);
    *xmin = x;
    return fx;
    }

  if( vcl_fabs(fa) < vcl_fabs(fb) )
    {
    const double t= a;
    a = b;
    b = t;
    const double ft = fa;
    fa = fb;
    fb = ft;
    }

  double c = a;
  double d = a;   // it is not clear how to initialize d
  double fc = fa;

  double s;

  for( unsigned int iteration = 1; iteration <= ITMAX; iteration++)
    {

    if( vcl_fabs(fb) <= ZEPS || vcl_fabs( a - b ) <= ZEPS )
      {
      *xmin=b;
      return fb;
      }

    const double fac = fa - fc;
    const double fbc = fb - fc;
    const double fab = fa - fb;

    if( vcl_fabs( fac ) < ZEPS || vcl_fabs(fbc) < ZEPS )
      {
      // Apply secant rule
      s = b - fb * (b - a) / ( fb - fa );
      }
    else
      {
      // Inverse quadratic interpolation
      const double afbfc = ( a * fb * fc ) / ( fab * fac );
      const double bfafc = ( b * fa * fc ) / ( fab * fbc );
      const double cfafb = ( c * fa * fb ) / ( fac * fbc );
      s = afbfc - bfafc + cfafb;
      }

    if( !( s > ( 3 * a + b ) / 4.0 && s < b ) ||
         (  mflag && ( vcl_fabs( s - b ) >= vcl_fabs( b - c ) / 2.0 ) ) ||
         ( !mflag && ( vcl_fabs( s - b ) >= vcl_fabs( c - d ) / 2.0 ) )    )
      {
      s = ( a + b ) / 2;
      mflag = true;
      }
    else
      {
      mflag = false;
      }
       
    double fs = this->LineFunction(s);

    d = c;
    c = b;

    if( fa * fs < 0.0 )
      {
      b = s;
      fb = fs;
      }
    else
      {
      a = s;
      fa = fs;
      }

    if( vcl_fabs( fa ) < vcl_fabs( fb ) )
      {
      const double temp = a;
      a = b;
      b = temp;
      }
    
    }

  *xmin = b;
  return fb;
#endif
}
#undef ITMAX
#undef CGOLD
#undef ZEPS

// ---------------------------------------------------------------------------
// Line Minimization 
// ---------------------------------------------------------------------------
void vtkpxOptimizer::AllocateLineTempCommons(vtkDoubleArray* position)
{
  if (this->pcom==NULL)
    this->pcom=vtkDoubleArray::New();

  if (this->xicom==NULL)
    this->xicom=vtkDoubleArray::New();

  if (this->xtemp==NULL)
    this->xtemp=vtkDoubleArray::New();

  this->pcom->SetNumberOfTuples(position->GetNumberOfTuples());
  this->xicom->SetNumberOfTuples(position->GetNumberOfTuples());
  this->xtemp->SetNumberOfTuples(position->GetNumberOfTuples());
  
}

void vtkpxOptimizer::CleanUpLineTempCommons()
{
  if (this->pcom!=NULL)
    this->pcom->Delete();
  if (this->xicom!=NULL)
    this->xicom->Delete();
  if (this->xtemp!=NULL)
    this->xtemp->Delete();
  this->pcom=NULL;
  this->xicom=NULL;
  this->xtemp=NULL;
}


double vtkpxOptimizer::LineMinimization(vtkDoubleArray* p, vtkDoubleArray* xi,int iterno,double tolerance,
				       const char* method)
{
  static int firsttime=0;
  /*
    ncom=n;
    pcom=vector(1,n);
    xicom=vector(1,n);
    nrfunc=func;
    for (j=1;j<=n;j++) {
    pcom[j]=p[j];
    xicom[j]=xi[j];
    }*/

  pcom->DeepCopy(p);
  xicom->DeepCopy(xi);

  /*
	BracketMinimum(&ax,&xx,&bx,&fa,&fx,&fb,f1dim);
	*fret=MinimizeGivenBounds(ax,xx,bx,f1dim,TOL,&xmin);
	for (j=1;j<=n;j++) {
		xi[j] *= xmin;
		p[j] += xi[j];
	}
	free_vector(xicom,1,n);
	free_vector(pcom,1,n);
  */
  // This is a Critical Change here 
  double xx=1.0,xmin,fx,fb,fa,bx,ax=0.0;

  /*  if (firsttime)
      fprintf(stdout,"Searching ax=%.2f bx=%.2f\n",ax,bx);*/
    
  this->BracketMinimum(&ax,&xx,&bx,&fa,&fx,&fb);

  /*  if (firsttime)
      fprintf(stdout,"Bracketing ax=%.2f bx=%.2f cx=%.2f\n",ax,bx,xx);*/

    

  double fret=this->MinimizeGivenBounds(ax,xx,bx,tolerance,&xmin);

  /*  if (firsttime)
      fprintf(stdout,"MinimizeGivenBounds ax=%.2f bx=%.2f cx=%.2f xmin=%.2f \n",ax,xx,bx,xmin);*/


  this->ScaleVector(xi,xmin);
  this->AddToVector(p,xi,1.0);

  if (iterno>=0)
    this->GenerateOutput(method, "Lmin",p,fret,iterno);

  firsttime=0;
  return fret;
}


double vtkpxOptimizer::LineFunction(double x)
{
  xtemp->DeepCopy(pcom);
  this->AddToVector(xtemp,xicom,x);
  return this->EvaluateFunction(this->MasterObject,xtemp);
}

// ---------------------------------------------------------------------------
double vtkpxOptimizer::Compute1DMinimization(vtkDoubleArray* position,double min_v,double max_v,double tolerance)
{
  if (position->GetNumberOfTuples()>1)
    {
      //      fprintf(stdout,"1D Minimization Only!!!!\n");
      return -1.0;
    }

  if (this->MasterObject==NULL || this->InternalEvaluateFunction==NULL)
    {
      vtkErrorMacro(<<"Hook Functions Not Properly Defined\n");
      return -1.0;
    }
  
  double old_similarity=this->EvaluateFunction(this->MasterObject,position);

  this->GenerateOutput("1D", "Beginning",position,old_similarity,0);
  this->AllocateLineTempCommons(position);
  this->pcom->DeepCopy(position);
  if (tolerance<0.0001)
    tolerance=0.0001;
  this->xicom->SetComponent(0,0,fabs(max_v-min_v)*0.01);

  double bx,xmin,fx,fb,fa,xx=max_v,ax=min_v;
  this->BracketMinimum(&ax,&xx,&bx,&fa,&fx,&fb);
  //  fprintf(stdout,"Bracketing ax=%.2f (%f) xx=%.2f (%f) bx=%.2f (%f)\n",ax,fa,xx,fx,bx,fb);

  double fret=this->MinimizeGivenBounds(ax,xx,bx,tolerance,&xmin);
  //  fprintf(stdout,"MinimizeGivenBounds ax=%.2f bx=%.2f cx=%.2f xmin=%.2f \n",ax,xx,bx,xmin);

  this->ScaleVector(xicom,xmin);
  this->AddToVector(position,xicom,1.0);

  this->GenerateOutput("1D"," ",position,fret,1);
  this->GenerateStatistics("1D",position);
  this->CleanUpLineTempCommons();
  return fret;
}
 



int vtkpxOptimizer::UsesNumericalRecipes()
{
#ifdef BIS_USE_NR
  return 1;
#endif

  return 0;
}

