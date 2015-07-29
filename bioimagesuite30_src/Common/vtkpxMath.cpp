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
//
//---------------------------------------------------------------------------------------------------------


#include <nrutil.h>
#include "vtkpxMath.h"
#include "vtkMath.h"
#include "nrutil.h"
#include "vtkObjectFactory.h"
#include "vtkFloatArray.h"
#include "fitpack.h"
#include "vtkPolyData.h"
#include "pxgeometrystruct.h"
#include "vtkPolyDataNormals.h"
#include "vtkPointData.h"
#include "pxutil.h"
#include "pxisinf.h"
#include <vtkstd/algorithm>
#include "vnl/vnl_gamma.h"
#include "vnl/vnl_math.h"
#include "vnl/vnl_erf.h"
#include "bio_f2c.h"
#include "bioimagesuite_settings.h"


extern "C" double dbetai_(double *x, double *pin, double *qin);
extern "C" double dgamma_(double *x);



//extern "C" { double dbetai_(double *x, double *pin, double *qin); }

// ------------------------------------------------------------------------
// Define M_PI for Windows !!
// ------------------------------------------------------------------------
#ifndef M_PI
#define M_PI 3.1415926854
#endif

// ------------------------------------------------------------------------

vtkpxMath* vtkpxMath::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxMath");
  if(ret)
    {
      return (vtkpxMath*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxMath;
}

//----------------------------------------------------------------------------
// Euclidean To Polar Conversions
//----------------------------------------------------------------------------
void vtkpxMath::EuclideanToPolar(float X,float Y,float& R,float& Theta)
{
  R=0.0;
  Theta=0.0;

  if (X!=0.0 && Y !=0.0)
    {
      R=sqrt(X*X+Y*Y);
      Theta=atan2(Y,X);
    }
}

void vtkpxMath::PolarToEuclidean(float R,float Theta,float& X,float& Y)
{
  X=R*cos(Theta);
  Y=R*sin(Theta);
}


// -------------------------------------------------------------------------------
// Code To Compute Distances around a circle  (units =radians)
// -------------------------------------------------------------------------------

float vtkpxMath::FindClosestAngleP1(float p1,float p0)
{
  if (p0==p1)
    return p1;

  while (p1-p0 > M_PI)
    p1-=M_PI*2.0;
  while (p1-p0 < -M_PI)
    p1+=M_PI*2.0;

  return p1;
}

float vtkpxMath::FindMinAngleDistance(float p1,float p0)
{
  return vtkpxMath::FindClosestAngleP1(p1,p0)-p0;
}


int vtkpxMath::ComputeAngleDistancePairs(float* ph,float* temp,int step,int np)
{
  int numdiff=np-step;
  if (numdiff<1)
    return 0;

  for (int i=0;i<numdiff;i++)
    temp[i]=FindMinAngleDistance(ph[i+step],ph[i]);

  return numdiff;
}

// ------------------------------------------------------------------------------------
// Order Statistics Code From Numerical Recipes
// -------------------------------------------------------------------------------------



float vtkpxMath::SelectKthLargest(unsigned long k0offset, unsigned long n, float* arr0offset)
{

  std::nth_element(arr0offset,arr0offset+k0offset,arr0offset+n);
  return arr0offset[k0offset];
  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

}


float vtkpxMath::GetMedian(unsigned long n, float* arr)
{
  if (n<1)
    return 0.0;

  if (n==1)
    return arr[0];

  if (n==2)
    return 0.5*(arr[0]+arr[1]);

  unsigned long half=n/2;

  // Even Number
  if (half*2 == n)
    {
      float v1=SelectKthLargest(half-1,n,arr);
      float v2=SelectKthLargest(half,n,arr);
      return 0.5*(v1+v2);
    }

  // Odd Number
  return SelectKthLargest(half,n,arr);

}


void vtkpxMath::Meanfit(float* x, float* y, int ndata, float& a, float& b, float& abdev)
{
  // Original Code Follows
  // This fits a straight line using ordinary least squares
  float sx=0.0,sy=0.0,sxy=0.0,sxx=0.0;

  for (int j=0;j<ndata;j++)
    {
      sx += x[j];
      sy += y[j];
      sxy += x[j]*y[j];
      sxx += x[j]*x[j];
    }
  //  fprintf(stderr,"sx=%.2f sy=%.2f sxy=%.2f sxx=%.2f\n",sx,sy,sxy,sxx);

  float del=ndata*sxx-sx*sx;
  a=(sxx*sy-sx*sxy)/del;
  b=(ndata*sxy-sx*sy)/del;

  //  fprintf(stderr,"del = %.2f a=%.2f b=%.2f\n",del,a,b);
  abdev=0.0;
  float temp=0.0;
  for (int ja=0;ja<ndata;ja++)
    abdev += (temp=y[ja]-(a+b*x[ja]),temp*temp);
}

void vtkpxMath::WeightedMeanfit(float *x, float *y, int ndata, float *w, int mwt,
				float &a,float &b,
				float &siga, float &sigb, float &chi2, float &q)
{
  float wt,t,sxoss,sx=0.0,sy=0.0,st2=0.0,ss,sigdat;

  b=0.0;
  if (mwt)
    {
      ss=0.0;
      for (int i=0;i<ndata;i++)
	{
	  wt=pow(w[i],(float)2.0);
	  ss += wt;
	  sx += x[i]*wt;
	  sy += y[i]*wt;
	}
    }
  else
    {
      for (int i=0;i<ndata;i++)
	{
	  sx += x[i];
	  sy += y[i];
	}
      ss=ndata;
    }
  sxoss=sx/ss;
  if (mwt)
    {
      for (int i=0;i<ndata;i++)
	{
	  t=(x[i]-sxoss)*w[i];
	  st2 += t*t;
	  b += t*y[i]*w[i];
	}
    }
  else
    {
      for (int i=0;i<ndata;i++)
	{
	  t=x[i]-sxoss;
	  st2 += t*t;
	  b += t*y[i];
	}
    }
  b /= st2;
  a=(sy-sx*b)/ss;
  siga=sqrt((1.0+sx*sx/(ss*st2))/ss);
  sigb=sqrt(1.0/st2);
  chi2=0.0;
  if (mwt == 0)
    {
      for (int i=0;i<ndata;i++)
	chi2 += sqrt(y[i]-a-b*x[i]);
      q=1.0;
      sigdat=sqrt(chi2/(ndata-2));
      siga *= sigdat;
      sigb *= sigdat;
    }
  else
    {
      for (int i=0;i<ndata;i++)
	chi2 += sqrt((y[i]-a-b*x[i]))*w[i];
      q=vnl_gamma_q( double(0.5*(ndata-2)) , chi2*0.5 );
    }
}


void vtkpxMath::Meanfit_Short(short* x, short* y, int ndata, short& a, short& b, short& abdev)
{
  // Original Code Follows
  // This fits a straight line using ordinary least squares
  short sx=0,sy=0,sxy=0,sxx=0;

  for (int j=0;j<ndata;j++)
    {
      sx += x[j];
      sy += y[j];
      sxy += x[j]*y[j];
      sxx += x[j]*x[j];
    }

  short del=ndata*sxx-sx*sx;
  a=(sxx*sy-sx*sxy)/del;
  b=(ndata*sxy-sx*sy)/del;
  abdev=0;
  short temp=0;
  for (int ja=0;ja<ndata;ja++)
    abdev += (temp=y[ja]-(a+b*x[ja]),temp*temp);
}


// --------------------------------------------------------------------------------
// Fiting Code from Numerical Recipes to fit y=ax+b using
// absolute differences (Medfit)
// NRrofunc is a utility routine
// --------------------------------------------------------------------------------



/*extern int ndatat;
  extern float *xt,*yt,aa,abdevt;*/
// originally rofunc in numerical recipes, eliminated 1-offset code and global variables

float vtkpxMath::NRrofunc(float b,float* xt,float* yt,float& aa,float& abdevt,int ndatat,float* arr)
{
#ifdef BIS_USE_NR
  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

#else
  return 0.0;
#endif
}




void vtkpxMath::Medfit(float* x, float* y, int ndata, float& a, float& b, float& abdev)
{

  float* temp_arr=new float[ndata];
  Medfit(x,y,ndata,a,b,abdev,temp_arr);
  delete [] temp_arr;
}


void vtkpxMath::Medfit(float* x, float* y, int ndata, float& a, float& b, float& abdev,float *temp_arr)
{
#ifdef BIS_USE_NR
  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

#else
  vtkpxMath::Meanfit(x,y,ndata,a,b,abdev);
#endif

}



// --------------------------------------------------------------------------------
void vtkpxMath::Heapsort(unsigned long n, float* ra0offset)
{
  std::sort(ra0offset,ra0offset+n);
  //The following code is commented out,
  //It is left there for Legacy purposes only

  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

}


void vtkpxMath::Heapsort_Short(unsigned long n, short* ra0offset)
{

  std::sort(ra0offset,ra0offset+n);


  //The following code is commented out,
  //It is left there for Legacy purposes only

  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

}

// ----------------------------------------------------------------------------------
float vtkpxMath::Gaussian(float x,float m,float sigma2)
{
  float t= (x-m)*(x-m)/(-2.0*sigma2);
  float A= 1.0/sqrt(2.0*M_PI*sigma2);
  return A*exp(t);
}

float vtkpxMath::Rayleigh(float x,float beta2)
{
  if (x<0.0)
    return 0.0;

  return (x/beta2)*exp(-(0.5*x*x)/beta2);
}
// ----------------------------------------------------------------------------------
float vtkpxMath::GaussianRandom()
{
  static int mz_previous_normal_flag=0;
  static double mz_previous_normal=0.0;
  if (mz_previous_normal_flag)
    {
      mz_previous_normal_flag = 0;
      return mz_previous_normal;
    }
  else
    {
      double x,y,r2;
      do
	{
	  x=2.0*vtkMath::Random()-1.0;
	  y=2.0*vtkMath::Random()-1.0;
	  r2 = x*x+y*y;
	}
      while (r2 >=1.0 || r2 == 0.0);
      double fac = vcl_sqrt(-2.0*vcl_log(r2)/r2);
      mz_previous_normal = x*fac;
      mz_previous_normal_flag = 1;
      return y*fac;
    }
}


float vtkpxMath::ZscoreToPvalue(float Z)
{
  // From itkGaussianDistribution (2 tail)
  return 2.0*(1.0-0.5 * (vnl_erf(vnl_math::sqrt1_2 * fabs(Z)) + 1.0));

}

float vtkpxMath::PvalueToZscore(float p)
{
  /*  if (p<1e-8 || p>0.99 )
      return -1.0;*/

  //  fprintf(stderr,"Looking for t for p=%.6f\n",p);

  float maxt=0.5,minp=p+1.0;
  while (minp>p)
    {
      maxt*=2.0;
      minp=ZscoreToPvalue(maxt);
      //      fprintf(stderr,"maxt=%.4f,minp=%.4f, p=%.4f\n");
    }
  //  fprintf(stderr,"Bound 1 = minp=%6.4f maxt=%6.4f\n",minp,maxt);

  float mint=10.0,maxp=p-1.0;

  while (maxp<p)
    {
      mint*=0.5;
      maxp=ZscoreToPvalue(mint);
    }

  //  fprintf(stderr,"Bound 2 = maxp=%6.4f mint=%6.4f\n",maxp,minp);
  //  fprintf(stderr,"Looking in t=%6.4f:%6.4f\n",mint,maxt);

  float dt=fabs(maxt-mint)/(maxt+mint);
  float newt=0.5*(maxt+mint);

  int count=1;

  while (dt>0.001 && count<1000)
    {
      newt=0.5*(maxt+mint);

      //      fprintf(stderr,"Looking in %.4f (%.6f):%.4f(%.6f) dt=%.6f\t",	      mint,maxp,maxt,minp,dt);
      float newp=ZscoreToPvalue(newt);
      //      fprintf(stderr,"newt=%.4f newp=%.4f\n",newt,newp);

      if (newp>p)
	{
	  mint=newt;
	  maxp=newp;
	}
      else
	{
	  maxt=newt;
	  minp=newp;
	}
      dt=fabs(maxt-mint)/(maxt+mint);
      ++count;
    }

  if (count==1000)
    newt=-newt;

  return newt;
}

#ifdef BIS_USE_NR

float vtkpxMath_gammln(float xx)
{
  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

}

#define MAXIT 100
#define EPS 3.0e-7
#define FPMIN 1.0e-30


float vtkpxMath_betacf(float a, float b, float x)
{
  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

}
#undef MAXIT
#undef EPS
#undef FPMIN


float vtkpxMath_betai(float a, float b, float x)
{
  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

}
// This ends a section of BIS_USE_NR stuff
#endif

float vtkpxMath::TtestToPvalue(float x,int df)
{
#ifdef BIS_USE_NR
  float fre=float(df);
  float   xi=fre/(fre+x*x);

  //  fprintf(stderr,"fre=%f pin=%.2f xi=%f\n",fre*0.5,0.5,xi);
  double value2=vtkpxMath_betai(fre*0.5,0.5,xi);


  return value2;
#else
  return vtkpxMath::TtestToPvalue_itk(x,df);
#endif
}


float vtkpxMath::TtestToPvalue_itk(float x,int df)
{
  double bx;
  double pin, qin;
  double dof;


  // Based on Abramowitz and Stegun 26.7.1, which gives the probability
  // that the absolute value of a random variable with a Student-t
  // distribution is less than or equal to a specified t.
  //
  // P[|x| <= t] = 1 - Ix(v/2, 1/2)
  //
  // where Ix is the incomplete beta function and v is the number of
  // degrees of freedom in the Student-t distribution and x is
  // v / (v + t^2).
  //
  // To calculate the cdf of the Student-t we need to convert
  // this formula.  For an x >= 0,
  //
  // P[|x| <= t] =   \int_{-t}^{t} p(x) dx
  //             = 2 \int_0^t p(x) dx
  //
  // The cdf of the Student-t is
  //
  // P[x <= t] = \int_{-\inf}^t p(x) dx
  //           = 0.5 + \int_0^t p(x) dx           (for x >= 0)
  //           = 0.5 + 0.5 * P[|x| < t]           (from above)
  //           = 0.5 + 0.5 * (1 - Ix(v/2. 1/2))
  //           = 1 - 0.5 * Ix(v/2, 1/2)
  //
  dof = static_cast<double>(df);
  bx = dof / (dof + (x*x));
  pin = dof / 2.0;
  qin = 0.5;

  /*  if (x<0.0 || x > 1.0 )
      return 1.0;*/

  /*  for (int ia=0;ia<=10;ia++)
    {
      double numf=10.0;
      double inpx=double(ia)*0.2;
      double bbx=numf/(numf+inpx*inpx);
      double ppin=numf*0.5;
      double v=dbetai_(&bbx,&ppin,&qin);
      fprintf(stdout,"x=%.2f \t v=%.2f \t(numf=%.1f)\n",bbx,v,numf);
      }*/

  //fprintf(stderr,"bx=%f pin=%.2f qin=%f\n",bx,pin,qin);

  double value2=dbetai_(&bx,&pin,&qin);
  return value2;

  /*  if (x >= 0.0)
    return 1.0 - 0.5 * dbetai_(&bx, &pin, &qin);

    return 0.5 * dbetai_(&bx, &pin, &qin);*/
}

float vtkpxMath::PvalueToTvalue(float p,int df)
{
  if (p<1e-8 || p>0.99 || df < 1)
    return -1.0;

  //fprintf(stderr,"Looking for t for p=%.6f\n",p);

  float maxt=0.5,minp=p+1.0;
  while (minp>p)
    {
      maxt*=2.0;
      minp=TtestToPvalue(maxt,df);
    }
  //fprintf(stderr,"Bound 1 = minp=%6.4f maxt=%6.4f\n",minp,maxt);

  float mint=10.0,maxp=p-1.0;

  while (maxp<p)
    {
      mint*=0.5;
      maxp=TtestToPvalue(mint,df);
    }

  //fprintf(stderr,"Bound 2 = maxp=%6.4f mint=%6.4f\n",maxp,minp);
  //fprintf(stderr,"Looking in t=%6.4f:%6.4f\n",mint,maxt);

  float dt=fabs(maxt-mint)/(maxt+mint);
  float newt=0.5*(maxt+mint);

  int count=1;

  while (dt>0.001 && count<1000)
    {
      newt=0.5*(maxt+mint);

      //fprintf(stderr,"Looking in %.4f (%.6f):%.4f(%.6f) dt=%.6f\t",	      mint,maxp,maxt,minp,dt);
      float newp=TtestToPvalue(newt,df);
      //fprintf(stderr,"newt=%.4f newp=%.4f\n",newt,newp);

      if (newp>p)
	{
	  mint=newt;
	  maxp=newp;
	}
      else
	{
	  maxt=newt;
	  minp=newp;
	}
      dt=fabs(maxt-mint)/(maxt+mint);
      ++count;
    }

  if (count==1000)
    newt=-newt;

  return newt;
}

// -----------------------------------------------------------------------------------------
//               Geometric Stuff ...
// ------------------------------------------------------------------------------------------
int vtkpxMath::CalculateCurvatures(vtkPolyData* polydata,int scale,PXIntegerList* neighbours)
{
  static int debugmode=0;
  int mode=0;
  int numpoints=polydata->GetNumberOfPoints();


  // Do Normals Scalars Stuff
  // ------------------------
  vtkFloatArray* curvature=vtkFloatArray::New();
  curvature->SetNumberOfComponents(2);
  curvature->SetNumberOfTuples(numpoints);

  vtkFloatArray* normals=vtkFloatArray::New();
  if (polydata->GetPointData()->GetNormals()!=NULL)
    {
      normals->DeepCopy(polydata->GetPointData()->GetNormals());
    }
  else
    {
      vtkPolyDataNormals* normalsFilter=vtkPolyDataNormals::New();
      normalsFilter->SetInput(polydata);
      normalsFilter->SetSplitting(0);
      normalsFilter->Update();
      normals->DeepCopy(normalsFilter->GetOutput()->GetPointData()->GetNormals());
      normalsFilter->Delete();
    }

  vtkPoints* points=polydata->GetPoints();

  if (debugmode)
    fprintf(stderr,"Num Points= %d , curvature(%d,%d) normals(%d,%d)\n",
	    numpoints,
	    curvature->GetNumberOfTuples(),curvature->GetNumberOfComponents(),
	    normals->GetNumberOfTuples(),normals->GetNumberOfComponents());

  scale=Irange(scale,1,10);
  if (mode==1 && debugmode)
    fprintf(stderr,"Curvature mode=%d, scale=%d\n",mode,scale);

  Vertex_3d neighbour_point[500],xformed_neighbour_point[500];
  int neighbour_index[500];

  Node_3d N, T1, T2, T;
  double c[4], length;
  double x, y, z;

  double fx, fy, fxx, fxy, fyy;
  //  Node_3d k1, k2;
  double K1, K2;
  int nrot;

  double** A     = dmatrix(0, 500-1, 0, 5);
  double** At    = dmatrix(0, 5, 0, 500-1);
  double** B     = dmatrix(0, 5, 0, 5);
  double** B_1   = dmatrix(0, 5, 0, 5);
  int*     indx  = ivector(0, 5);
  double** C     = dmatrix(0, 5, 0, 0);
  double** C2    = dmatrix(0, 5, 0, 0);
  double*  coeff = dvector(0, 5);
  double** data  = dmatrix(0, 500-1, 0, 0);

  double** g     = dmatrix(0, 1, 0, 1);
  double** g_1   = dmatrix(0, 1, 0, 1);
  double** b     = dmatrix(0, 1, 0, 1);
  double** beta  = dmatrix(0, 1, 0, 1);


  double ** a1_repl = new double*[2];
  a1_repl[0] = new double[2];
  a1_repl[1] = new double[2];

  double ** v1_repl = new double*[2];
  v1_repl[0] = new double[2];
  v1_repl[1] = new double[2];

  double* d1_repl=new double[2];


  for(int i=0; i<numpoints; i++)
    {
      if (debugmode)
	{
	  if (100*int(i/100)==i)
	    fprintf(stderr,"%d/%d ",i,numpoints);
	}
      for(int j=0; j<500; j++)
	{
	  neighbour_point[j].x   = 0.0;  neighbour_point[j].y   = 0.0;
	  neighbour_point[j].z   = 0.0;
	  neighbour_point[j].K   = 0.0;  neighbour_point[j].H   = 0.0;
	  neighbour_point[j].k1  = 0.0;  neighbour_point[j].k2  = 0.0;
	  neighbour_point[j].k1x = 0.0;  neighbour_point[j].k1y = 0.0;
	  neighbour_point[j].k1z = 0.0;
	  neighbour_point[j].k2x = 0.0;  neighbour_point[j].k2y = 0.0;
	  neighbour_point[j].k2z = 0.0;  neighbour_point[j].nx  = 0.0;
	  neighbour_point[j].ny  = 0.0;  neighbour_point[j].nz  = 0.0;
	}

      /* find the neighbouring points within the defined radius */
      neighbour_index[0] = i;
      int k = 1;

      /* run through the layers of neighbours */
      int num1 = 0, num2 = k;
      for(int s=1; s<=scale; s++)
	{
	  for(int j=num1; j<num2; j++)
	    {
	      neighbours[neighbour_index[j]].Rewind();
	      for(int m=0; m<neighbours[neighbour_index[j]].getnumnodes(); m++)
		{
		  int index=neighbours[neighbour_index[j]].getcurrent();
		  int flag = 0;
		  for (int n=0;n<k;n++)
		    {
		      if (index==neighbour_index[n])
			flag=1;
		    }
		  if (flag==0)
		    {
		      neighbour_index[k]=index;
		      k++;
		    }
		  neighbours[neighbour_index[j]].Next();
		}
	    }
	  num1 = num2;
	  num2 = k;
	}
      int num_neighbours = k;
      if (debugmode)
	{
	  if (100*int(i/100)==i)
	    fprintf(stderr,"num_neighbours[%d] = %d\t", i, num_neighbours);
	}

      /*for(j=0; j<num_neighbours; j++)
	neighbour_point[j] = point[neighbour_index[j]];*/

      double point_x[3],normal_x[3];

      for (int j=0;j<num_neighbours;j++)
	{

	  points->GetPoint(neighbour_index[j],point_x);
	  normals->GetTuple(neighbour_index[j],normal_x);

	  neighbour_point[j].x =point_x[0];
	  neighbour_point[j].y =point_x[1];
	  neighbour_point[j].z =point_x[2];
	  neighbour_point[j].nx=normal_x[0];
	  neighbour_point[j].ny=normal_x[1];
	  neighbour_point[j].nz=normal_x[2];
	}

      if(num_neighbours<5)
	{
	  if (debugmode)
	    {
	      fprintf(stderr,"point no.%d: very few neighbours: %d\n", i, num_neighbours);
	      fprintf(stderr,"add three interpolated points\n");
	    }
	  /* there are at least four neighbours (including myself) */
	  /* add three neighbours from the interpolation between   */
	  /* me and the three existing neighbours. 		*/
	  /* (need at least five neighbours 			*/
	  for(int j=0; j<3; j++){
	    neighbour_point[j+num_neighbours].x
	      = (neighbour_point[0].x + neighbour_point[j].x) / 2.0;
	    neighbour_point[j+num_neighbours].y
	      = (neighbour_point[0].y + neighbour_point[j].y) / 2.0;
	    neighbour_point[j+num_neighbours].z
	      = (neighbour_point[0].z + neighbour_point[j].z) / 2.0;
	  }
	}
      if(num_neighbours>499 && debugmode)
	{
	  fprintf(stderr,"point no.%d: too many neighbours: %d\n", i, num_neighbours);
	  fprintf(stderr,"do you really need that many neighbours??\n");
	}


      /* the normal vector at point[i] */
      points->GetPoint(i,point_x);
      normals->GetTuple(i,normal_x);
      N.x = normal_x[0];
      N.y = normal_x[1];
      N.z = normal_x[2];

      /* the coeffs of the tangent plane at point[i] */
      c[0] = N.x;
      c[1] = N.y;
      c[2] = N.z;
      c[3] = c[0] * point_x[0] + c[1] * point_x[1] + c[2] * point_x[2];

      /* pick a random point T on the tangent plane */
      if(c[0]==1.0 || c[0]==-1.0)
	{
	  T.x = 0.0; T.y = 1.0;  T.z = 0.0;
	}
      else if(c[1]==1.0 || c[1]==-1.0)
	{
	  T.x = 0.0;  T.y = 0.0;  T.z = 1.0;
	}
      else if(c[2]==1.0 || c[2]==-1.0)
	{
	  T.x = 1.0;  T.y = 0.0;  T.z = 0.0;
	}
      else if(c[0]!=0.0)
	{
	  T.x = c[3] / c[0];  T.y = 0.0;  T.z = 0.0;
	}
      else if(c[1]!=0.0)
	{
	  T.x = 0.0;  T.y = c[3] / c[2];T.z = 0.0;
	}
      else if(c[2]!=0.0)
	{
	  T.x = 0.0; T.y = 0.0; T.z = c[3] / c[2];
	}

      /* the vector between point[i] and T is the first(x) tangent vector */
      T1.x = point_x[0] - T.x;
      T1.y = point_x[1] - T.y;
      T1.z = point_x[2] - T.z;
      length = T1.x * T1.x + T1.y * T1.y + T1.z * T1.z;
      length = sqrt(length);
      T1.x /= length;
      T1.y /= length;
      T1.z /= length;

      /* the cross product N x T1 is the second(y) tangent vector */
      T2.x = N.y * T1.z - T1.y * N.z;
      T2.y = N.z * T1.x - T1.z * N.x;
      T2.z = N.x * T1.y - T1.x * N.y;
      /*
	fprintf(stderr,"N:  (%4.2f, %4.2f, %4.2f)\n", N.x, N.y, N.z);
	fprintf(stderr,"Ti: (%4.2f, %4.2f, %4.2f)\n", T1.x, T1.y, T1.z);
	fprintf(stderr,"T2: (%4.2f, %4.2f, %4.2f)\n", T2.x, T2.y, T2.z);
      */
      /* transform the neighbour points, such that   */
      /* the current point (I) becomes the origin,  */
      /* the normal direction N becomes the z-axis, */
      /* and the tangent directions T1/T2 becomes   */
      /* the x/y-axes.	 			  */
      for(int m=0; m<num_neighbours; m++)
	{
	  x = neighbour_point[m].x - point_x[0];
	  y = neighbour_point[m].y - point_x[1];
	  z = neighbour_point[m].z - point_x[2];
	  xformed_neighbour_point[m].x = x * T1.x + y * T1.y + z * T1.z;
	  xformed_neighbour_point[m].y = x * T2.x + y * T2.y + z * T2.z;
	  xformed_neighbour_point[m].z = x * N.x  + y * N.y  + z * N.z;
	}


      /* fit the local quadric surface */
      /* using least square method     */
      for(int m=0; m<num_neighbours; m++)
	data[m][0] = xformed_neighbour_point[m].z;

      for(int m=0; m<num_neighbours; m++){
	A[m][0] = xformed_neighbour_point[m].x
	  * xformed_neighbour_point[m].x;
	A[m][1] = xformed_neighbour_point[m].y
	  * xformed_neighbour_point[m].y;
	A[m][2] = xformed_neighbour_point[m].x
	  * xformed_neighbour_point[m].y;
	A[m][3] = xformed_neighbour_point[m].x;
	A[m][4] = xformed_neighbour_point[m].y;
	A[m][5] = 1.0;
      }

      double det=0;
      int sing=0;

      matrix_transpose(A, At, num_neighbours, 6);
      matrix_product(B, At, 6, num_neighbours, A, num_neighbours, 6);
      if (mode==0)
	{
	  matrix_inverse(B, B_1, 6);
	}
      else
	{
	  ludcmp0(B,6,indx,&det,sing);
	  //fprintf(stderr,"Det =%f, sing=%d ",det,sing);
	}

      matrix_product(C, At, 6, num_neighbours, data, num_neighbours, 1);

      if (mode==0)
	{
	  matrix_product(C2, B_1, 6, 6, C, 6, 1);
	  for (int ii=0;ii<6;ii++)
	    coeff[ii]=C2[ii][0];
	}
      else
	{
	  for (int ii=0;ii<6;ii++)
	    coeff[ii]=C[ii][0];
	  lubksb0(B,6,indx,coeff);
	}
      /*
	// compute the residual of the fit
	double residual = 0.0;
	for(m=0; m<num_neighbours; m++)
	residual += fabs(data[m]
	- coeff[0] * A[m][0]
	- coeff[1] * A[m][1]
	- coeff[2] * A[m][2]
	- coeff[3] * A[m][3]
	- coeff[4] * A[m][4]
	- coeff[5] * A[m][5]);
      */
      /* compute the curvatures */
      fx  = coeff[3];
      fy  = coeff[4];
      fxx = coeff[0] * 2.0;
      fxy = coeff[2];
      fyy = coeff[1] * 2.0;

      g[0][0] = 1.0 + fx*fx;
      g[0][1] = fx * fy;
      g[1][0] = fx * fy;
      g[1][1] = 1.0 + fy*fy;



      matrix_inverse(g, g_1, 2);

      b[0][0] = fxx / ((1.0+fx*fx+fy*fy) * (1.0+fx*fx+fy*fy));
      b[0][1] = fxy / ((1.0+fx*fx+fy*fy) * (1.0+fx*fx+fy*fy));
      b[1][0] = fxy / ((1.0+fx*fx+fy*fy) * (1.0+fx*fx+fy*fy));
      b[1][1] = fyy / ((1.0+fx*fx+fy*fy) * (1.0+fx*fx+fy*fy));

      matrix_product(beta, g_1, 2, 2, b, 2, 2);

      //point[i].K = beta[0][0] * beta[1][1] - beta[0][1] * beta[1][0];
      //point[i].H = (beta[0][0] + beta[1][1]) / 2.0;


      for(int m=1; m<3; m++)
	for(int n=1; n<3; n++)
	  a1_repl[m-1][n-1] = beta[m-1][n-1];


      vtkMath::JacobiN(a1_repl,2,d1_repl,v1_repl);

      //      get_eigenvalues_vectors(a1, 2, d1, v1, &nrot);
      //      sort_eigenvalues_vectors(d1, v1, 2);

      K1 = d1_repl[0];
      K2 = d1_repl[1];

      if (debugmode)
	{
	  if (100*int(i/100)==i)
	    fprintf(stderr," k1=%6.3f, k2=%6.3f\n",K1,K2);
	}

      /* the prinipal curvatures and directions in local space */

      curvature->SetComponent(i,0,K1);
      curvature->SetComponent(i,1,K2);

      /* the normal vector in global space */
      normals->SetComponent(i,0, N.x);
      normals->SetComponent(i,1, N.y);
      normals->SetComponent(i,2, N.z);
    }

  free_dmatrix(A,     0,500-1,0,5);
  free_dmatrix(At,    0,5,0,500-1);
  free_dmatrix(B,     0,5,0,5);
  free_dmatrix(B_1,   0,5,0,5);
  free_ivector(indx  ,0,5);
  free_dmatrix(C,     0,5,0,5);
  free_dmatrix(C2,    0,5,0,5);
  free_dvector(coeff, 0,5);
  free_dmatrix(data,  0,500-1,0,0);
  free_dmatrix(g,     0, 1, 0, 1);
  free_dmatrix(g_1,   0, 1, 0, 1);
  free_dmatrix(b,     0, 1, 0, 1);
  free_dmatrix(beta,  0, 1, 0, 1);
  //  free_dmatrix(a1,    1, 2, 1, 2);
  //  free_dvector(d1,    1, 2);
  //  free_dmatrix(v1,    1, 2, 1, 2);



  delete [] a1_repl[1];
  delete [] a1_repl[0];
  delete [] a1_repl;


  delete [] v1_repl[1];
  delete [] v1_repl[0];
  delete [] v1_repl;

  delete [] d1_repl;




  polydata->GetPointData()->SetNormals(normals);
  polydata->GetPointData()->SetScalars(curvature);

  curvature->Delete();
  normals->Delete();
  return 1;


}

