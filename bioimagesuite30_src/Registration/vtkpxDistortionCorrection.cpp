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
  Module:    $RCSfile: vtkpxNonLinearRegistration.cpp,v $
  Language:  C++
  Date:      $Date: 2002/04/16 17:51:27 $
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
#include "vtkpxDistortionCorrection.h"
#include "vtkObjectFactory.h"
#include "pxutil.h"
#include "vtkbisImageReslice.h"
#include "vtkpxUtil.h"
#include "vtkImageMathematics.h"
#include "vtkMath.h"
#include "vtkpxRegistration.h"

vtkpxDistortionCorrection* vtkpxDistortionCorrection::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxDistortionCorrection");
  if(ret)
    {
    return (vtkpxDistortionCorrection*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxDistortionCorrection;
  
}
//-------------------------------------------------------------------------
vtkpxDistortionCorrection::vtkpxDistortionCorrection()
{
  this->LastImageJacobian=NULL;
  this->LastSignalLossImage=NULL;
  this->UseJacobian=0;
  this->UseSignalLossCorrection=0;
  this->KeepSameNumberOfSlices=1;
  this->UpdateTransform->SetNonLinearFirst(0);
  this->PhaseEncodeAxis=1;
}
//-------------------------------------------------------------------------
vtkpxDistortionCorrection::~vtkpxDistortionCorrection() 
{
  if (this->LastImageJacobian!=NULL)
    this->LastImageJacobian->Delete();
  if (this->LastSignalLossImage!=NULL)
    this->LastSignalLossImage->Delete();
}
//-------------------------------------------------------------------------
int vtkpxDistortionCorrection::Initialize(int level) 
{
  this->WarpX=0;
  this->WarpY=0;
  this->WarpZ=0;

  switch (this->PhaseEncodeAxis)
    {
    case 0:
      this->WarpX=1;
      break;
    case 1:
      this->WarpY=1;
      break;
    case 2:
      this->WarpZ=1;
      break;
    }

  vtkpxNonLinearRegistration::Initialize(level);
  if (this->InitialTransform!=NULL)
    {
      if (this->InitialTransform->IsA("vtkLinearTransform")==1)
	this->ComboTransform->SetNonLinearFirst(0);
    }

  
  if (this->LastImageJacobian!=NULL)
    this->LastImageJacobian->Delete();
  this->LastImageJacobian=vtkImageData::New();

  if (this->LastSignalLossImage!=NULL)
    this->LastSignalLossImage->Delete();
  this->LastSignalLossImage=vtkImageData::New();
  
  return 1;
}
//-------------------------------------------------------------------------
double vtkpxDistortionCorrection::Evaluate()
{
  if (this->LastTargetImage==NULL)
    this->LastTargetImage=vtkImageData::New();
  vtkpxRegistration::ResliceImage(this->LastTargetImage,this->SampledTransformImage,
				  this->SampledReferenceImage,this->ComboTransform,this->InterpolationMode);

  if (this->UseJacobian==1)
    this->ScaleIntensitiesUsingJacobian(this->LastTargetImage);
  this->Histogram->WeightedFillHistogram(this->SampledReferenceImage,this->LastTargetImage,this->SampledReferenceWeightImage,1);  
  
  this->LastSimilarity=this->Similarity(this->Histogram);
  if (this->Lambda>0.0001)
    this->LastSmoothness=this->ComputeInternalEnergy(this->GridTransform);
  else
    this->LastSmoothness=0.0;
  return this->LastSimilarity-this->LastSmoothness*this->Lambda;
}
//-------------------------------------------------------------------------
double vtkpxDistortionCorrection::EvaluatePiece(vtkImageData* reference,
						vtkImageData* target,
						vtkImageData* previoustarget,
						vtkImageData* weightimage,
						int cpoint)
{
  this->Histogram->BackupHistogram();

  // Reslice And Scale 
  vtkImageData* tmp=vtkImageData::New();
  vtkpxRegistration::ResliceImage(tmp,target,reference,this->ComboTransform,this->InterpolationMode);
  if (this->UseJacobian==1)
    this->ScaleIntensitiesUsingJacobian(tmp);
  
  // Remove Last Target Value
  this->Histogram->WeightedRemoveFromHistogram(reference,previoustarget,weightimage);
  this->Histogram->WeightedFillHistogram(reference,tmp,weightimage,0);
  tmp->Delete();

  double e1=this->Similarity(this->Histogram);
  this->Histogram->ResetHistogram();

  double l1=0.0;
  if (this->Lambda>0.000001)
    l1 = this->ComputeInternalEnergy(this->GridTransform,cpoint)*double(NumberOfControlPoints);
  double s1=e1-this->Lambda*l1;
  
  return s1;
}
//-------------------------------------------------------------------------
int vtkpxDistortionCorrection::ComputeSimpleJacobianAndSignalLoss(vtkImageData* ref,vtkImageData* jacobian,vtkImageData* signalloss,
								  vtkAbstractTransform* tr)
{
  if (ref==NULL || jacobian==NULL || tr==NULL)
    return 0;

  int dim[3];  ref->GetDimensions(dim);
  int dim2[3]; jacobian->GetDimensions(dim2);

  if (dim[0]!=dim2[0] || dim[1]!=dim2[1] || dim[2]!=dim2[2] || 
      jacobian->GetScalarType()!=VTK_DOUBLE || 
      jacobian->GetNumberOfScalarComponents()!=1)
    {
      jacobian->CopyStructure(ref);
      jacobian->SetNumberOfScalarComponents(1);
      jacobian->SetScalarTypeToDouble();
      jacobian->AllocateScalars();
    }

  int sigmode=0;
  if (signalloss!=NULL)
    {
      sigmode=1;
      signalloss->GetDimensions(dim2);
      
      if (dim[0]!=dim2[0] || dim[1]!=dim2[1] || dim[2]!=dim2[2] || 
	  signalloss->GetScalarType()!=VTK_DOUBLE || 
	  signalloss->GetNumberOfScalarComponents()!=1)
	{
	  signalloss->CopyStructure(ref);
	  signalloss->SetNumberOfScalarComponents(1);
	  signalloss->SetScalarTypeToDouble();
	  signalloss->AllocateScalars();
	}
    }
  
  double sp[3];  ref->GetSpacing(sp);
  double ori[3]; ref->GetOrigin(ori);
  double der[3][3],x[3],x2[3];
  
  vtkDataArray* dat=jacobian->GetPointData()->GetScalars();
  vtkDataArray* sig=NULL;
  if (sigmode)
    sig=signalloss->GetPointData()->GetScalars();
  int index=0;

  for (int k=0;k<dim[2];k++)
    {
      x[2]=k*sp[2]+ori[2];
      for (int j=0;j<dim[1];j++)
	{
	  x[1]=j*sp[1]+ori[1];
	  for (int i=0;i<dim[0];i++)
	    {
	      x[0]=i*sp[0]+ori[0];
	      tr->InternalTransformDerivative(x,x2,der);
	      double det=der[1][1];//vtkMath::Determinant3x3(der);
	      dat->SetComponent(index,0,det);
	      if (sigmode)
		{
		  double v=1.0+sqrt(der[1][0]*der[1][0]*sp[0]*sp[0]+
				   der[1][1]*der[1][1]*sp[1]*sp[1]+
				   der[1][2]*der[1][2]*sp[2]*sp[2]);
		  sig->SetComponent(index,0,v);
		}
	      ++index;
	    }
	}
    }
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxDistortionCorrection::ScaleIntensitiesUsingJacobian(vtkImageData* inout)
{
  if (this->UseSignalLossCorrection)
    return vtkpxDistortionCorrection::ScaleIntensitiesUsingJacobianAndSignalLoss(inout,this->GridTransform,
										 this->LastImageJacobian,
										 this->LastSignalLossImage,
										 this->LogarithmMode);
								  

  return vtkpxDistortionCorrection::ScaleIntensitiesUsingJacobian(inout,this->GridTransform,
								  this->LastImageJacobian,
								  this->LogarithmMode);
								  
}
//-------------------------------------------------------------------------
int vtkpxDistortionCorrection::ScaleIntensitiesUsingJacobian(vtkImageData* inout,vtkAbstractTransform* tr,vtkImageData* jac,
							     int logmode)
{
  int ok=vtkpxDistortionCorrection::ComputeSimpleJacobianAndSignalLoss(inout,jac,NULL,tr);
  // int ok=vtkpxRegistration::ComputeSimpleJacobianImage(inout,jac,tr,1,0.0);
  if (ok==0)
    {
      fprintf(stderr,"Bad Jacobian Calculation\n");
      return 0;
    }

  int np1= inout->GetPointData()->GetScalars()->GetNumberOfTuples();
  int np2= jac->GetPointData()->GetScalars()->GetNumberOfTuples();

  if (np1!=np2)
    {
      fprintf(stderr,"Bad Jacobian Image\n");
      return 0;
    }

  vtkDataArray* scal=inout->GetPointData()->GetScalars();
  double *img2 = (double*) jac->GetPointData()->GetScalars()->GetVoidPointer(0);  
  
  for (int i=0;i<np1;i++)
    {
      double a=scal->GetComponent(i,0);
      double b=( *img2);
      if (b<0.33)
	b=0.33;
      else if (b>3.0)
	b=3.0;
      if (logmode)
	scal->SetComponent(i,0,a+log(b));
      else
	scal->SetComponent(i,0,a*b);
      ++img2;
    }
  return 1;
}
//------------------------------------------------------------------------- 
int vtkpxDistortionCorrection::ScaleIntensitiesUsingJacobianAndSignalLoss(vtkImageData* inout,vtkAbstractTransform* tr,
									  vtkImageData* jac,vtkImageData* sig,
									  int logmode)
{
  int ok=vtkpxDistortionCorrection::ComputeSimpleJacobianAndSignalLoss(inout,jac,sig,tr);
  if (ok==0)
    {
      fprintf(stderr,"Bad Jacobian/Signal Loss Calculation\n");
      return 0;
    }

  int np1= inout->GetPointData()->GetScalars()->GetNumberOfTuples();
  int np2= jac->GetPointData()->GetScalars()->GetNumberOfTuples();

  if (np1!=np2)
    {
      fprintf(stderr,"Bad Jacobian Image\n");
      return 0;
    }

  vtkDataArray* scal=inout->GetPointData()->GetScalars();
  double *img2 = (double*) jac->GetPointData()->GetScalars()->GetVoidPointer(0);  
  double *loss = (double*) sig->GetPointData()->GetScalars()->GetVoidPointer(0);  
  
  for (int i=0;i<np1;i++)
    {
      double a=scal->GetComponent(i,0);
      double b=( *img2);
      double l=( *loss);

      if (b<0.33)
	b=0.33;
      else if (b>3.0)
	b=3.0;
      if (logmode)
	scal->SetComponent(i,0,a+log(b)+log(l));
      else
	scal->SetComponent(i,0,a*b*l);
      ++img2;
      ++loss;
    }
  return 1;
}
//-------------------------------------------------------------------------

int vtkpxDistortionCorrection::Finalize(int level)
{
  if (this->LastImageJacobian!=NULL && this->UseJacobian==1)
    {
      double range[2];
      this->LastImageJacobian->GetPointData()->GetScalars()->GetRange(range);
      fprintf(stderr,"Jacobian Range =%.4f:%.4f\n",range[0],range[1]);
    }
  return vtkpxNonLinearRegistration::Finalize(level);
}
//-------------------------------------------------------------------------

double vtkpxDistortionCorrection::ComputeSimilarityBetweenImages(vtkImageData* img1,vtkImageData* img2,
								vtkAbstractTransform* tr,
								double resolution[3],double sigma,
								int metric,int logarithmmode,
								int interpolation,int numbins)
{
  vtkImageData* ref=vtkImageData::New();
  vtkImageData* trn=vtkImageData::New();
  
  vtkpxRegistration::ResampleAndSmoothImage(ref,img1,
					    resolution,sigma,1.0);
  
  vtkpxRegistration::ResampleAndSmoothImage(trn,img2,
					    resolution,sigma,1.0);
  
  short minr=0,maxr=1,mint=0,maxt=1;
  int num_ref=vtkpxRegistration::CalculateNumberOfBinsAndAdjustImage(ref,numbins,minr,maxr,logarithmmode);
  int num_tar=vtkpxRegistration::CalculateNumberOfBinsAndAdjustImage(trn,numbins,mint,maxt,logarithmmode);

  vtkpxJointHistogram* Histogram=vtkpxJointHistogram::New();
  Histogram->Allocate(num_ref,num_tar);

  vtkbisImageReslice* resl=vtkbisImageReslice::New();    resl->OptimizationOff();

  resl->SetInput(trn);
  resl->SetInformationInput(ref);
  resl->SetResliceTransform(tr);
  resl->SetInterpolationMode(interpolation);
  resl->Update();

  vtkImageData* target=vtkImageData::New();
  target->ShallowCopy(resl->GetOutput());
  resl->Delete();


  metric=Irange(metric,0,2);

  if (metric>0)
    {
      vtkImageData* jac=vtkImageData::New();
      vtkImageData* loss=vtkImageData::New();

      vtkpxDistortionCorrection::ComputeSimpleJacobianAndSignalLoss(target,jac,loss,tr);
      if (metric==2)
	vtkpxDistortionCorrection::ScaleIntensitiesUsingJacobianAndSignalLoss(target,tr,jac,loss,logarithmmode);
      else
	vtkpxDistortionCorrection::ScaleIntensitiesUsingJacobian(target,tr,jac,logarithmmode);
      jac->Delete();
      loss->Delete();
    }
								  
  Histogram->FillHistogram(ref,target,1);
  double val= +Histogram->NormalizedMutualInformation();

  target->Delete();
  Histogram->Delete();
  ref->Delete();
  trn->Delete();
  return val;
}

