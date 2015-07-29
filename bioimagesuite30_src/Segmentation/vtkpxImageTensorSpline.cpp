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




/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxImageTensorSpline.cpp,v $
  Language:  C++
  Date:      $Date: 2004/12/15 14:20:06 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkImageData.h"
#include "vtkpxImageTensorSpline.h"
#include "vtkbisImageResample.h"
#include "vtkImageThreshold.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkObjectFactory.h"
#include "vtkpxOptimizer.h"
#include "vtkpxUtil.h"
#include "vtkpxImageExtractVOI.h"
#include "vtkImageContinuousDilate3D.h"
#include "vtkImageMedian3D.h"
//-----------------------------------------------------------------------------------
vtkpxImageTensorSpline* vtkpxImageTensorSpline::New()
{
  // First try to create the object from the vtkObjectFactory

  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageTensorSpline");
  if(ret)
    {
      return (vtkpxImageTensorSpline*)ret;
    }
  
  // If the factory was unable to create the object, then create it here.
  
  return new vtkpxImageTensorSpline;
}

//----------------------------------------------------------------------------

vtkpxImageTensorSpline::vtkpxImageTensorSpline()
{
  this->Output=NULL;
  this->Input=NULL;
  this->WeightImage=NULL;
  this->SampledInputImage=NULL;
  this->SampledWeightImage=NULL;
  this->MaskImage=NULL;

  this->Sigma=0.0;
  this->Lambda=0.001;
  this->StepSize=5.0;
  this->Resolution=2.0;
  this->PreSmoothIterations=8;
  this->AutoInitialize=1;
  this->Mode=0;
  this->ControlPointSpacing=20.0;
  this->Iterations=10;
  this->Epsilon=0.1;
}

//----------------------------------------------------------------------------

vtkpxImageTensorSpline::~vtkpxImageTensorSpline()
{
  if (this->Output!=NULL)
    this->Output->Delete();

  if (this->SampledInputImage!=NULL)
    this->SampledInputImage->Delete();

  if (this->SampledWeightImage!=NULL)
    this->SampledWeightImage->Delete();

  this->SetInput(NULL);
  this->SetWeightImage(NULL);
  this->SetMaskImage(NULL);
}
//----------------------------------------------------------------------------
vtkImageData* vtkpxImageTensorSpline::ResampleAndSmoothImage(vtkImageData* source,double res,double sigma)
{

  if (sigma<0.0)    
    sigma=0.0;
  else if (sigma>20.0)
    sigma=20.0;

  if (res<1.0)    
    res=1.0;
  else if (res>10.0)
    res=10.0;

  if (source==NULL)
      return 0;
  
  double sp[3];  source->GetSpacing(sp);
  double ori[3]; source->GetOrigin(ori);
  int dim[3];   source->GetDimensions(dim);

  double gp[3],sum=0.0;
  for (int ia=0;ia<=2;ia++)
    {
      gp[ia]=(sigma/sp[ia]);
      if (gp[ia]>25.0) 
	gp[ia]=25.0;
    }
  
  vtkbisImageResample* resamp=vtkbisImageResample::New();


  if (sigma>0.001)
    {
      vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
      smooth->SetInput(source);
      smooth->SetStandardDeviations(gp);
      resamp->SetInput(smooth->GetOutput());
      smooth->Delete();
      resamp->InterpolateOn();
    }
  else 
    {
      resamp->SetInput(source);
      resamp->InterpolateOff();
    }

  resamp->SetDimensionality(3);
  for (int ib=0;ib<=2;ib++)
    resamp->SetAxisOutputSpacing(ib,res*sp[0]);
  if (sp[2]>res*sp[0])
    resamp->SetAxisOutputSpacing(2,sp[2]);

  resamp->Update();


  vtkImageData* destination=vtkImageData::New();
  destination->ShallowCopy(resamp->GetOutput());
  resamp->Delete();

  return destination;
}

// --------------------------------------------------------------------------------------------------
int vtkpxImageTensorSpline::GenerateLaplacianExtentedFieldWeight(vtkImageData* field,vtkImageData* weight,double res,int maxiter)
{
  if (field==NULL || weight==NULL)
    return 0;
  
  vtkImageData* tmp=vtkImageData::New();
  vtkImageData* tmpw=vtkImageData::New();

  int dim[3];   field->GetDimensions(dim);
  double sp[3]; field->GetSpacing(sp);

  int incr[6];
  double wincr[6];
  int slicesize=dim[0]*dim[1];

  double gp[3];
  for (int ia=0;ia<=2;ia++)
    gp[ia]=1.0/sp[ia];

  incr[5]=-slicesize;  wincr[5]=gp[2];
  incr[4]=slicesize;   wincr[4]=gp[2];
  incr[3]=-dim[0];     wincr[3]=gp[1];
  incr[2]=dim[0];      wincr[2]=gp[1];
  incr[1]=-1;          wincr[1]=gp[0];
  incr[0]=1;           wincr[0]=gp[0];

  int nt=dim[0]*dim[1]*dim[2];

  int iter=1;

  for (int pass=1;pass<=1;pass++)
    {
      double maxchange=100.0;
      if (pass==2)
	{
	  iter=20;
	  maxiter=400+iter;
	}

      while (iter<=maxiter)
	{
	  maxchange=0.0;
	  tmp->DeepCopy(field);
	  tmpw->DeepCopy(weight);
	  
	  vtkDataArray* tmp_arr=tmp->GetPointData()->GetScalars();
	  vtkDataArray* tmpw_arr=tmpw->GetPointData()->GetScalars();

	  vtkDataArray* fld=field->GetPointData()->GetScalars();
	  vtkDataArray* wgt=weight->GetPointData()->GetScalars();
	  
	  int iincr=1;
	  int i=0;
	  /*if (2*int(iter/2)==iter)
	    {
	      iincr=-1;
	      i=nt-1;
	      }*/
	  
	  fprintf(stderr,"iter = %d , (iincr=%d i=%d) ",iter,iincr,i);
	  
	  while (i>=0 && i<=nt-1)
	    {
	      int z=i/slicesize;
	      int y=(i-z*slicesize)/dim[1];
	      int x=(i-z*slicesize-y*dim[1]);

	      int proceed=1;
	      if (x==0 || y==0 || x==(dim[0]-1) || y==(dim[1]-1))
		proceed=0;

	      double thisweight=tmpw_arr->GetComponent(i,0);
	      
	      if (  ( (thisweight==100 && pass==1 ) || (thisweight<100 && pass==2)) && proceed==1)
		{
		  double sum=0.0,nsum=0.0,sumw=0.0,nsumw=0.0;
		  
		  for (int sh=0;sh<4;sh++)
		    {
		      int newindex=i+incr[sh];
		      if (newindex<0 || newindex>=nt)
			newindex=i;
		      double w=tmpw_arr->GetComponent(newindex,0);
		      if (w>=thisweight)
			{
			  sum+=tmp_arr->GetComponent(newindex,0)*wincr[sh];
			  nsum+=wincr[sh];
			  sumw+=w;
			  nsumw+=1.0;
			}
		    }
		  
		  if (nsum>0.0)
		    {
		      double tmpx=sum/nsum;
		      double oldx=tmp_arr->GetComponent(i,0);

		      double neww=1.0;
		      if (nsumw>0.0)
			neww=Frange(sumw/nsumw,1.0,50.0);

		      double w1=0.95;
		      if (thisweight>=100.0)
			{
			  w1=0.1;
			}

		      double newx=w1*tmpx+(1.0-w1)*oldx;
		      if ( fabs(newx-oldx)>maxchange)
			maxchange=fabs(newx-oldx);
		      fld->SetComponent(i,0,newx);
		      tmp_arr->SetComponent(i,0,newx);
		      
		      if (wgt->GetComponent(i,0)<100)
			{
			  wgt->SetComponent(i,0,neww);
			  tmpw_arr->SetComponent(i,0,neww);
			}
		    }
		}
	      i=i+iincr;
	    }
	  
	  if (maxchange<0.2)
	    iter=maxiter;

	  ++iter;
	}
    }

  tmp->Delete();
  tmpw->Delete();

  this->SetWeightImage(weight);
  this->SampledInputImage=this->ResampleAndSmoothImage(field,res,0.00);
  this->SampledWeightImage=this->ResampleAndSmoothImage(this->WeightImage,res,0.00);
  

  return 1;
}

// --------------------------------------------------------------------------------------------------
int vtkpxImageTensorSpline::SetInputForBiasFieldApproximation(vtkImageData* ref,double res)
{
  if (ref==NULL)
      return 0;

  if (res<1.0)    
    res=1.0;
  else if (res>10.0)
    res=10.0;
  
  double sp[3];  ref->GetSpacing(sp);
  double ori[3]; ref->GetOrigin(ori);
  int dim[3];   ref->GetDimensions(dim);


  double mean=0.0,sum=0.0,max=0.0;
  int domask=0;
  vtkDataArray* orig=ref->GetPointData()->GetScalars();
  int nt=orig->GetNumberOfTuples();
 
  vtkImageData* cleanref=vtkImageData::New();
  cleanref->CopyStructure(ref);
  cleanref->AllocateScalars();

  if (this->MaskImage!=NULL)
    {
      vtkDataArray* mask=this->MaskImage->GetPointData()->GetScalars();
      int nt2=mask->GetNumberOfTuples();

      if (nt2==nt)
	{
	  domask=1;
	  
	  vtkDataArray* tmp=cleanref->GetPointData()->GetScalars();
	  vtkDataArray* mask=this->MaskImage->GetPointData()->GetScalars();
      
	  int nt=tmp->GetNumberOfTuples();
	  int nt2=mask->GetNumberOfTuples();
	  int nc=tmp->GetNumberOfComponents();
	  double r[2]; mask->GetRange(r);
	  for (int ia=0;ia<nt;ia++)
	    {
	      if (mask->GetComponent(ia,0)>r[0])
		{
		  for (int ib=0;ib<nc;ib++)
		    tmp->SetComponent(ia,ib,orig->GetComponent(ia,ib));
		  if (orig->GetComponent(ia,0) > 0)
		    {
		      mean+=orig->GetComponent(ia,0);
		      if (orig->GetComponent(ia,0)>max)
			max=orig->GetComponent(ia,0);
		      sum+=1.0;
		    }
		} 
	      else
		{
		  for (int ib=0;ib<nc;ib++)
		    tmp->SetComponent(ia,ib,0.0);
		}
	    }
	}
      else
	{
	  cleanref->ShallowCopy(ref);
	  domask=0;
	  for (int voxel=0;voxel<nt;voxel++)
	    {
	      double v=orig->GetComponent(voxel,0);
	      if (v>0.0)
		{
		  mean+=v;
		  if (v>max)
		    max=v;
		  sum+=1.0;
		}
	    }
	}
    }

  mean/=sum;

  fprintf(stderr,"In ImageTensorSpline domask=%d mean=%f max=%f\n",domask,mean,max);

  vtkImageThreshold* thr=vtkImageThreshold::New();
  thr->SetInput(cleanref);
  thr->ThresholdByLower(0.5);
  thr->ReplaceInOn();
  thr->ReplaceOutOn();
  thr->SetInValue(0.0);
  thr->SetOutValue(100.0);
  thr->Update();

  vtkImageData* field=vtkImageData::New();
  field->CopyStructure(cleanref);
  field->SetNumberOfScalarComponents(1);
  field->AllocateScalars();

  vtkDataArray* top=cleanref->GetPointData()->GetScalars();
  vtkDataArray* bot=thr->GetOutput()->GetPointData()->GetScalars();
  vtkDataArray* fld=field->GetPointData()->GetScalars();

  double range[2]; bot->GetRange(range);
  double threshold=0.99*range[0]+0.01*range[1];
  fprintf(stderr,"Auto Range range=%f:%f thr=%f mean=%f\n",range[0],range[1],threshold,mean);

  for (int voxel=0;voxel<nt;voxel++)
    {
      float b=bot->GetComponent(voxel,0);
      float r=100.0;
      if (b>threshold)
	{
	  float t=top->GetComponent(voxel,0);
	  if (t>0.0)
	    r=10000.0*t/(b*mean);
	  if (r<25.0)
	    r=25.0;
	  else if (r>400.0)
	    r=400.0;
	}
      fld->SetComponent(voxel,0,r);
    }

  this->GenerateLaplacianExtentedFieldWeight(field,thr->GetOutput(),res,this->PreSmoothIterations);
  field->Delete();
  thr->Delete();
  cleanref->Delete();


  return 1;
}
//-------------------------------------------------------------------------
vtkImageData* vtkpxImageTensorSpline::GenerateOutput(vtkImageData* ref)
{
  if (ref==NULL)
    return NULL;

  vtkImageData* output=vtkImageData::New();
  output->CopyStructure(ref);
  output->SetNumberOfScalarComponents(1);
  output->AllocateScalars();

  double ori[3];   output->GetOrigin(ori); 
  double sp[3];    output->GetSpacing(sp); 
  int   dim[3];   output->GetDimensions(dim); 

  vtkDataArray* fits=output->GetPointData()->GetScalars();
  
  int index=0;
  double x[3],tx[3];
  for (int ka=0;ka<dim[2];ka++)
    {
      x[2]=ka*sp[2]+ori[2];
      for (int ja=0;ja<dim[1];ja++)
	{
	  x[1]=ja*sp[1]+ori[1];
	  for (int ia=0;ia<dim[0];ia++)
	    {
	      x[0]=ia*sp[0]+ori[0];

	      this->TransformPoint(x,tx);
	      double d=tx[0]-x[0];
	      fits->SetComponent(index,0,d);
	      ++index;
	    }
	}
    }
  return output;
}

// --------------------------------------------------------------------------------------------------------------
double vtkpxImageTensorSpline::ComputeAverageSquaredDifference(vtkImageData* x,vtkImageData* y,vtkImageData* weight)
{
  vtkDataArray* ref=x->GetPointData()->GetScalars();
  vtkDataArray* fit=y->GetPointData()->GetScalars();
  vtkDataArray* wgt=NULL;
  if (weight!=NULL)
    wgt=weight->GetPointData()->GetScalars();

  int nt=ref->GetNumberOfTuples();
  double sum=0.0001;
  double sqerr=0.0;
  for (int ia=0;ia<nt;ia++)
    {
      double v1=ref->GetComponent(ia,0);
      double v2=fit->GetComponent(ia,0);
      double w=1.0;
      if (wgt!=NULL)
	w=wgt->GetComponent(ia,0);
      sqerr+=w*pow(v1-v2,2.0);
      sum+=w;
    }
  sqerr/=sum;
  return sqerr;
}
// -----------------------------------------------------------------------------------------
double vtkpxImageTensorSpline::ComputeAverageSquaredFittingError(vtkImageData* refimg,vtkImageData* weight)
{
  vtkDataArray* ref=refimg->GetPointData()->GetScalars();
  vtkDataArray* wgt=NULL;
  if (weight!=NULL)    wgt=weight->GetPointData()->GetScalars();

  double ori[3];   refimg->GetOrigin(ori); 
  double sp[3];    refimg->GetSpacing(sp); 
  int   dim[3];   refimg->GetDimensions(dim); 

  double sqerr=0.0;
  double sum=0.0001;
  int index=0;
  double x[3],tx[3];
  for (int ka=0;ka<dim[2];ka++)
    {
      x[2]=ka*sp[2]+ori[2];
      for (int ja=0;ja<dim[1];ja++)
	{
	  x[1]=ja*sp[1]+ori[1];
	  for (int ia=0;ia<dim[0];ia++)
	    {
	      x[0]=ia*sp[0]+ori[0];
	      
	      this->TransformPoint(x,tx);
	      /*	      if (ka==dim[2]/2 && ja==dim[1]/2 && ia==dim[0]/2)
			      fprintf(stderr,"x=%.2f,%.2f,%.2f \t tx=%.2f,%.2f,%.2f\n",x[0],x[1],x[2],tx[0],tx[1],tx[2]);*/

	      double v2=tx[0]-x[0];
	      double v1=ref->GetComponent(index,0);
	      double w=1.0;
	      if (wgt!=NULL)
		w=wgt->GetComponent(index,0);
	      sqerr+=w*(pow(v1-v2,2.0));
	      sum+=w;
	      ++index;
	    }
	}
    }
  sqerr/=sum;
  return sqerr;
}
// --------------------------------------------------------------------------------------------------------------
// Optimization Stuff
// --------------------------------------------------------------------------------------------------------------
double vtkpxImageTensorSpline::OptimizerValue(vtkDoubleArray* params)
{
  this->SetCurrentParameters(params);
  double sqerr=this->ComputeAverageSquaredFittingError(this->SampledInputImage,this->SampledWeightImage);

  this->LastError=sqrt(sqerr);

  double sum=0.0;
  
  if (this->Lambda>0.0)
    sum=this->ComputeTotalBendingEnergy();
    this->LastSmoothness =sum;

  return sqerr+sum*Lambda;
}
// -----------------------------------------------------------------------------------------
double vtkpxImageTensorSpline::EvaluatePiece(vtkImageData* reference,vtkImageData* weights,int cpoint)
{
  double e1=this->ComputeAverageSquaredFittingError(reference,weights);
  double l1 =this->ComputeBendingEnergyAroundControlPoint(cpoint);
  double s1=-(e1+this->Lambda*l1);
  return s1;
}

//-------------------------------------------------------------------------
double vtkpxImageTensorSpline::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)
{
  if (grad->GetNumberOfComponents()!=1 || grad->GetNumberOfTuples()!=this->NumberOfControlPoints)
    {
      grad->SetNumberOfComponents(1);
      grad->SetNumberOfTuples(this->NumberOfControlPoints);
    }

  this->OptimizerValue(params);
  int count=0;
  int per=this->NumberOfControlPoints/10;
  int count2=0;
  
  grad->FillComponent(0,0.0);

  fprintf(stderr,"Computing Gradient: ");

  double GradientNorm=0.0;
  for (int i = 0; i < this->NumberOfControlPoints; i++)
    {
      ++count;

      if (count==per)
	{
	  fprintf(stderr,"%.1f ",double(count2)*10.0);
	  count=0;
	  ++count2;
	  if (count2==10)
	    fprintf(stderr,"\n");
	}

      double p[3]; this->GetControlPointLocation(i,p);
      
      vtkpxImageExtractVOI* voi=vtkpxImageExtractVOI::New();
      voi->SetInput(this->SampledInputImage);
      voi->SetRegion(p,this->Spacing,1.1);
      voi->Update();

      vtkImageData* weight=NULL;

      if (this->SampledWeightImage!=NULL)
	{
	  vtkpxImageExtractVOI* m_voi=vtkpxImageExtractVOI::New();
	  m_voi->SetInput(this->SampledWeightImage);
	  m_voi->SetRegion(p,this->Spacing,1.1);
	  m_voi->Update();
	  weight=vtkImageData::New();
	  weight->ShallowCopy(m_voi->GetOutput());
	  m_voi->Delete();
	}
      
      this->ShiftControlPoint(i,0,-this->StepSize);
      double s1 = this->EvaluatePiece(voi->GetOutput(),weight,i);

      this->ShiftControlPoint(i,0,+2.0*this->StepSize);
      double s2 = this->EvaluatePiece(voi->GetOutput(),weight,i);

      this->ShiftControlPoint(i,0,-this->StepSize);

      grad->SetComponent(i,0,-(s2-s1));
      GradientNorm+=pow(s2-s1,2.0);
      voi->Delete();
      if (weight!=NULL)
	weight->Delete();

    }
  if (GradientNorm<0.001)
    GradientNorm=0.001;

  GradientNorm = (sqrt(GradientNorm));
  double sc=this->StepSize/GradientNorm;

  // Normalize vector
  if (GradientNorm > 0)
    {
      int nt=grad->GetNumberOfTuples();
      int nc=grad->GetNumberOfComponents();
      for (int t=0;t<nt;t++)
	for (int c=0;c<nc;c++)
	  grad->SetComponent(t,c,grad->GetComponent(t,c) * sc);
    }
  else 
    grad->FillComponent(0,0.0);

  fprintf(stderr,"Gradient Norm=%.5f scale=%f\n",GradientNorm,sc);
  return GradientNorm;
}

// -----------------------------------------------------------------------------------------
void  vtkpxImageTensorSpline::SetCurrentParameters(vtkDoubleArray* param)
{
  int numt=this->NumberOfControlPoints;
  int num=param->GetNumberOfTuples();

  if (num!=numt)
    {
      vtkErrorMacro(<<"Bad Data for Optimization");
      return;
    }

  for (int i=0;i<numt;i++)
    {
      double p[3];
      p[0]=param->GetComponent(i,0);
      p[1]=0.0;
      p[2]=0.0;
      this->Displacements->SetTuple(i,p);
    }
}
// -----------------------------------------------------------------------------------------
void  vtkpxImageTensorSpline::GetCurrentParameters(vtkDoubleArray* param)
{
  int num=this->NumberOfControlPoints;
  
  if (param->GetNumberOfComponents()!=1 || param->GetNumberOfTuples()!=num)
    {
      param->SetNumberOfComponents(1);
      param->SetNumberOfTuples(num);
    }

  for (int i=0;i<num;i++)
    {
      double* p=this->Displacements->GetTuple(i);
      param->SetComponent(i,0,p[0]);
    }
}

// -----------------------------------------------------------------------------------------
int vtkpxImageTensorSpline::ApproximateImage(vtkImageData* img,float resolution,float spacing,float offset,float smoothness,float initial)
{
  this->SetInput(img);
  this->SetResolution(resolution);
  if (spacing>0.0)
    {
      this->SetControlPointSpacing(spacing);
      this->SetAutoInitialize(1);
    }
  else
    this->SetAutoInitialize(0);
  //this->SetSigma(smoothness);
  this->Run();
  return 1;
}
// -----------------------------------------------------------------------------------------
int  vtkpxImageTensorSpline::ApproximateFieldMap(double scalefactor,int phasedirection)
{
  int ok=this->Run();
  if (ok==0)
    return 0;

  phasedirection=Irange(phasedirection,0,2);

  vtkDataArray* out=this->Output->GetPointData()->GetScalars();
  int nt=out->GetNumberOfTuples();

  for (int i=0;i<nt;i++)
    {
      double v=out->GetComponent(i,0)*scalefactor;
      out->SetComponent(i,phasedirection,v);
    }

  if (phasedirection!=0)
    out->FillComponent(0,0.0);
  
  return 1;
}

int vtkpxImageTensorSpline::Run()
{
  if (this->Input==NULL)
    {
      vtkErrorMacro(<<"Bad Input Image\n");
      return 0;
    }

  if (this->AutoInitialize==1 || this->Displacements==NULL)
    {
      fprintf(stderr,"Auto Initialize ...\n");
      this->Initialize(this->Input,this->ControlPointSpacing,0.05);
    }
  
  if (this->SampledInputImage!=NULL)
    this->SampledInputImage->Delete();
  
  if (this->SampledWeightImage!=NULL)
    this->SampledWeightImage->Delete();
  this->SampledWeightImage=NULL;

  if (this->Mode==0)
    {
      this->SampledInputImage=this->ResampleAndSmoothImage(this->Input,this->Resolution,this->Sigma);
      if (this->WeightImage!=NULL)
	{
	  this->SampledWeightImage=this->ResampleAndSmoothImage(this->WeightImage,this->Resolution,this->Sigma);
	}
    }
  else
    {
      this->SetInputForBiasFieldApproximation(this->Input,this->Resolution);
    }

  // Initialize B-Spline with value at closest point
  // Crude but OK
  int    dim[3]; this->SampledInputImage->GetDimensions(dim);
  double  sp[3]; this->SampledInputImage->GetSpacing(sp);
  double ori[3]; this->SampledInputImage->GetOrigin(ori);

  fprintf(stderr,"Approximating dim=%d x %d x %d (%.1f,%.1f,%.1f) Mode=%d\n",
	  dim[0],dim[1],dim[2],sp[0],sp[1],sp[2],this->Mode);
  
  for (int ia=0;ia<this->NumberOfControlPoints;ia++)
    {
      double x[3];
      double p[3]={0.0,0.0,0.0};
      this->GetControlPointLocation(ia,x);
      int voxel[3];
      for (int ib=0;ib<=2;ib++)
	voxel[ib]=Irange(int((x[ib]-ori[ib])/sp[ib]),0,dim[ib]-1);

      //      if (this->Mode>0)
      p[0]=this->SampledInputImage->GetScalarComponentAsDouble(voxel[0],voxel[1],voxel[2],0);

      this->ShiftControlPoint(ia,p);
    }


  {
    vtkImageData* out=this->GenerateOutput(this->Input);
    int ok=vtkpxUtil::SaveAnalyze("initial_fit",out,0);
    fprintf(stderr,"Initial_fit saved... %d",ok);
    out->Delete();
  }
  


  vtkpxOptimizer* optimizer=vtkpxOptimizer::New();
  vtkDoubleArray* position=vtkDoubleArray::New();
  vtkDoubleArray* temp=vtkDoubleArray::New();
  
  this->LastError=0.0;
  this->LastSmoothness=0.0;

  this->GetCurrentParameters(position);
  optimizer->SetMasterObject(this);
  optimizer->SetEvaluateFunctionMethod(&vtkpxImageTensorSpline::CalculateFunction);
  optimizer->SetEvaluateGradientMethod(&vtkpxImageTensorSpline::CalculateGradient);
  optimizer->SetFeedbackMethod(&vtkpxImageTensorSpline::OptimizerFeedback);
  
  double v1=this->OptimizerValue(position);
  fprintf(stderr,"Value=%f\n",v1);
  double tol=this->Epsilon*0.01*fabs(v1);
  fprintf(stderr,"Tolerance=%.5f\n",tol);

  double v=optimizer->ComputeConjugateGradient(position,this->Iterations,tol);
  optimizer->Delete();
  position->Delete();
  temp->Delete();

  if (this->Output!=NULL)
    this->Output->Delete();
  this->Output=this->GenerateOutput(this->Input);
  
  return 1;
}

// -----------------------------------------------------------------------------------------
// New Stuff for Optimization
// -----------------------------------------------------------------------------------------
double vtkpxImageTensorSpline::CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output)
{
  return ((vtkpxImageTensorSpline*)self)->OptimizerGradient(position,output);
}

double vtkpxImageTensorSpline::CalculateFunction(vtkObject* self,vtkDoubleArray* position)
{
  return ((vtkpxImageTensorSpline*)self)->OptimizerValue(position);
}

void vtkpxImageTensorSpline::OptimizerFeedback(vtkObject* slf,const char* line,double val)
{
  vtkpxImageTensorSpline* self=(vtkpxImageTensorSpline*)slf;
  fprintf(stderr,"%s (%.5f) [ dist=%.5f tbend=%.5f]\n",line,val,
	  self->LastError,
	  self->LastSmoothness);
}
// -----------------------------------------------------------------------------------------

