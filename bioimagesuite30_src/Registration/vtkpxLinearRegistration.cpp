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

// ------------------------------------------------------------------------
//
// This file derives from original work
// in the vtkCISG Toolkit
// that was released under the terms of 
// the GNU General Public License (either version 2,
// of the License, or any later version.)
// 
// The code has been modified for inclusion in BioImage Suite
//
// It may consist of work derived from more than one source
// file in the vtkCISG Toolkit. The original copyright notice
// was:
//
//  Authors   : Thomas Hartkens <thomas@hartkens.de>
//              Daniel Rueckert <d.rueckert@doc.ic.ac.uk>
//	        Julia Schnabel  <julia.schnabel@kcl.ac.uk>
//	        under the supervision of Dave Hawkes and Derek Hill
//  Web       : http://www.image-registration.com
//  Copyright : King's College London
//              Div. of Radiological Sciences, 
//              Computational Imaging Science Group, 1997 - 2000
//	        http://www-ipg.umds.ac.uk/cisg
//
// ------------------------------------------------------------------------


/*=========================================================================*/
#include "vtkpxLinearRegistration.h"
#include "vtkpxRegistration.h"
#include "vtkObjectFactory.h"
#include "vtkTransform.h"
#include "vtkpxUtil.h"
#include "vtkpxOptimizer.h"
#include "vtkShortArray.h"

vtkpxLinearRegistration* vtkpxLinearRegistration::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxLinearRegistration");
  if(ret)
    {
      return (vtkpxLinearRegistration*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxLinearRegistration;
  
}

vtkpxLinearRegistration::vtkpxLinearRegistration()
{

  DofStatus=vtkShortArray::New();
  DofStatus->SetNumberOfTuples(15);
  for (int i=0; i<15; i++) 
    {
      DofStatus->SetComponent(i,0,1);
      DofWeight[i]=0.0;
    }
  
  this->SetTransformModeToAffine();
  this->LinearTransform=NULL;
  this->UpdateTransform=vtkTransform::New();
  this->InitialTransform=NULL;
  this->AutoCenterOrigin=1;
  this->TransformOutputMode=0;
  this->DistortionMode=0;
  this->OptimizationAutoScale=1;
  this->OptimizationScaleFactor=1.0;

}
//-------------------------------------------------------------------------
vtkpxLinearRegistration::~vtkpxLinearRegistration()
{
  if (this->LinearTransform!=NULL)
    {
      this->LinearTransform->Delete();
      this->LinearTransform=NULL;
    }
  if (this->UpdateTransform!=NULL)
    {
      this->UpdateTransform->Delete();
      this->UpdateTransform=NULL;
    }

  this->SetInitialTransform(NULL);
  this->DofStatus->Delete();
}

//-------------------------------------------------------------------------
void vtkpxLinearRegistration::SetTransformModeToRigid()
{
  TranslationX=1;	TranslationY=1; 	TranslationZ=1;
  RotationX=1;	RotationZ=1; 	RotationY=1;
  ScalingX=0; 	ScalingY=0;	ScalingZ=0;
  ShearingXY=0;	ShearingYX=0;	ShearingYZ=0;
  ShearingZY=0;	ShearingZX=0;	ShearingXZ=0;
}
//-------------------------------------------------------------------------
void vtkpxLinearRegistration::SetTransformModeToDistortion()
{
  TranslationX=1;	TranslationY=1; 	TranslationZ=1;
  RotationX=1;	RotationZ=1; 	RotationY=1;
  ScalingX=0; 	ScalingY=1;	ScalingZ=0;
  ShearingXY=0;	ShearingYX=0;	ShearingYZ=0;
  ShearingZY=0;	ShearingZX=0;	ShearingXZ=0;
  this->DistortionMode=1;
}
//-------------------------------------------------------------------------
void vtkpxLinearRegistration::SetTransformModeToSimilarity()
{
  TranslationX=1;	TranslationY=1; 	TranslationZ=1;
  RotationX=1;	RotationZ=1; 	RotationY=1;
  ScalingX=1; 	ScalingY=1;	ScalingZ=1;
  ShearingXY=0;	ShearingYX=0;	ShearingYZ=0;
  ShearingZY=0;	ShearingZX=0;	ShearingXZ=0;
}
//-------------------------------------------------------------------------
void vtkpxLinearRegistration::SetTransformModeToAffine()
{
  TranslationX=1;	TranslationY=1; 	TranslationZ=1;
  RotationX=1;	RotationZ=1; 	RotationY=1;
  ScalingX=1; 	ScalingY=1;	ScalingZ=1;
  ShearingXY=1;	ShearingYX=1;	ShearingYZ=1;
  ShearingZY=1;	ShearingZX=1;	ShearingXZ=1;
}
//-------------------------------------------------------------------------
void vtkpxLinearRegistration::SetTransformModeToRigid2D()
{
  TranslationX=1;	TranslationY=1; 	TranslationZ=0;
  RotationX=0;	RotationZ=1; 	RotationY=0;
  ScalingX=0; 	ScalingY=0;	ScalingZ=0;
  ShearingXY=0;	ShearingYX=0;	ShearingYZ=0;
  ShearingZY=0;	ShearingZX=0;	ShearingXZ=0;
}
//-------------------------------------------------------------------------
void vtkpxLinearRegistration::SetTransformModeToSimilarity2D()
{
  TranslationX=1;	TranslationY=1; 	TranslationZ=0;
  RotationX=0;	RotationZ=1; 	RotationY=0;
  ScalingX=1; 	ScalingY=1;	ScalingZ=0;
  ShearingXY=0;	ShearingYX=0;	ShearingYZ=0;
  ShearingZY=0;	ShearingZX=0;	ShearingXZ=0;
}
//-------------------------------------------------------------------------
void vtkpxLinearRegistration::SetTransformModeToAffine2D()
{
  TranslationX=1;	TranslationY=1; 	TranslationZ=0;
  RotationX=0;	RotationZ=1; 	RotationY=0;
  ScalingX=1; 	ScalingY=1;	ScalingZ=0;
  ShearingXY=1;	ShearingYX=1;	ShearingYZ=0;
  ShearingZY=0;	ShearingZX=0;	ShearingXZ=0;
}
//-------------------------------------------------------------------------
void vtkpxLinearRegistration::UpdateDofLabels()
{
  this->DofStatus->SetComponent(0,0,TranslationX);
  this->DofStatus->SetComponent(1,0,TranslationY); 
  this->DofStatus->SetComponent(2,0,TranslationZ); 
  this->DofStatus->SetComponent(3,0,RotationX); 
  this->DofStatus->SetComponent(4,0,RotationY); 
  this->DofStatus->SetComponent(5,0,RotationZ); 
  this->DofStatus->SetComponent(6,0,ScalingX); 
  this->DofStatus->SetComponent(7,0,ScalingY); 
  this->DofStatus->SetComponent(8,0,ScalingZ); 
  this->DofStatus->SetComponent(9,0,ShearingXY); 
  this->DofStatus->SetComponent(10,0,ShearingYX); 
  this->DofStatus->SetComponent(11,0,ShearingYZ); 
  this->DofStatus->SetComponent(12,0,ShearingZY); 
  this->DofStatus->SetComponent(13,0,ShearingZX); 
  this->DofStatus->SetComponent(14,0,ShearingXZ); 
}

int vtkpxLinearRegistration::GetDofStatus(int dof)
{
  return (int)DofStatus->GetComponent(dof,0);
}

int vtkpxLinearRegistration::InitializeAll() 
{
  vtkpxAbstractIntensityRegistration::InitializeAll();
  UpdateDofLabels();

  if (this->OptimizationAutoScale)
    {
      this->OptimizationScaleFactor=this->MeanResolution;
    }
  else 
    {
      if (this->OptimizationScaleFactor<0.001)
	this->OptimizationScaleFactor=0.001;
    }

  fprintf(stdout,"\t\t\t OptScaleFactor=%.5f (auto=%d)\n",this->OptimizationScaleFactor,this->OptimizationAutoScale);


  if (this->LinearTransform==NULL)
    this->LinearTransform=vtkpxLinearTransform::New();
  this->LinearTransform->Identity();

  if (this->InitialTransform!=NULL)
    {
      this->LinearTransform->Copy(this->InitialTransform);
      this->GenerateOutput(-1,-1,-1,-1,-1,-1,-1);
    }
  else
    fprintf(stdout,"Not Copying any initial transform\n");

  this->LinearTransform->SetDistortionMode(this->DistortionMode);  

  this->GenerateOutput(-1,-1,-1,-1,-1,-1,-1);

  this->TransformOutputMode=0;
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxLinearRegistration::Initialize(int level)
{
  vtkpxAbstractIntensityRegistration::Initialize(level); 

  if (this->AutoCenterOrigin)
    {
      double sp1[3],sp2[3];
      int   dim1[3],dim2[3];
      this->SampledReferenceImage->GetSpacing(sp1);
      this->SampledReferenceImage->GetDimensions(dim1);
      this->SampledTransformImage->GetSpacing(sp2);
      this->SampledTransformImage->GetDimensions(dim2);
      
      double or1[3],or2[3];
      for (int ia=0;ia<=2;ia++)
	{
	  or1[ia]=-0.5*sp1[ia]*dim1[ia];
	  or2[ia]=-0.5*sp2[ia]*dim2[ia];
	}
      
      this->SampledReferenceImage->SetOrigin(or1);
      this->SampledTransformImage->SetOrigin(or2);
      

      vtkpxRegistration::FixPipelineSpacingAndOrigin(this->SampledReferenceImage);
      vtkpxRegistration::FixPipelineSpacingAndOrigin(this->SampledTransformImage);
    }

  return 1;
}
//-------------------------------------------------------------------------
int vtkpxLinearRegistration::ComputeRegistration(int level)
{  
  double step=this->CurrentStepSize;
  int maxdofs=15;
  if (level>1)
    maxdofs=6;

  int v=0;
  for (int i=0;i<maxdofs;i++)
    {
      if (this->DofStatus->GetComponent(i,0)==1)
	v=i+1;
    }

  maxdofs=v;
  //fprintf(stderr,"vtkpxLinearRegistration:: Level = %d maxdofs=%d\n",level,maxdofs);
      
  for (int st=1;st<=this->NumberOfSteps;st++)
    {
      double old_similarity = this->Evaluate();
      double new_similarity=this->Optimize(maxdofs,step,old_similarity,level,st);
      old_similarity=this->Evaluate();
      this->GenerateOutput(level,st,this->NumberOfIterations,step,new_similarity,
			   this->LastSimilarity,this->LastSmoothness);
      step*=0.5;
    }
  
  return 1;
}

//-------------------------------------------------------------------------
double vtkpxLinearRegistration::Optimize(int maxdofs,double step,double old_similarity,int level,int st)
{

  if (this->OptimizationMethod==-1)
    {
      return this->OldOptimize(maxdofs,step,old_similarity,level,st);
    }


  OptimizationStructure.SetValues(maxdofs,step,old_similarity,level,st);

  vtkpxOptimizer* optimizer=vtkpxOptimizer::New();
  vtkDoubleArray* position=vtkDoubleArray::New();
  vtkDoubleArray* temp=vtkDoubleArray::New();
  this->GetCurrentParameters(position);
  optimizer->SetMasterObject(this);
  optimizer->SetEvaluateFunctionMethod(&vtkpxAbstractIntensityRegistration::CalculateFunction);
  optimizer->SetEvaluateGradientMethod(&vtkpxAbstractIntensityRegistration::CalculateGradient);
  optimizer->SetBeginIterationMethod(&vtkpxAbstractIntensityRegistration::OnBeginIteration);
  optimizer->SetFeedbackMethod(&vtkpxAbstractIntensityRegistration::OptimizerFeedback);


  double v=0.0;
  fprintf(stderr,"Optimization Method=%d\n\n",this->OptimizationMethod);

  switch (this->OptimizationMethod)
    {
    case 1:
      v=optimizer->ComputeSlowClimb(position,step,this->NumberOfIterations,this->DofStatus);
      break;

    case 2:
      v=optimizer->ComputePowell(position,temp,this->NumberOfIterations,0.0001);
      break;

    case 3:
      v=optimizer->ComputeGradientDescent(position,this->NumberOfIterations,0.0001);
      break;

    case 4:
      v=optimizer->ComputeConjugateGradient(position,this->NumberOfIterations,0.0001);
      break;

    case 5:
      v=optimizer->ComputeSlowClimb(position,step,this->NumberOfIterations,this->DofStatus);
      if (level==1 && st==this->NumberOfSteps)
	v=optimizer->ComputeConjugateGradient(position,this->NumberOfIterations,0.0001);
      break;

    default:
      if (this->MeanResolution>1.0)
	v=optimizer->ComputeConjugateGradient(position,this->NumberOfIterations,0.0001);
      else
	v=optimizer->ComputeGradientDescent(position,this->NumberOfIterations,0.0001);
    }

  optimizer->Delete();
  position->Delete();
  temp->Delete();
  return v;
}
//-------------------------------------------------------------------------
int vtkpxLinearRegistration::FinalizeAll()
{
  // Shift Origin back to 0,0,0 as opposed to center of image
  this->GenerateUpdateTransform();
  this->TransformOutputMode=1;
  //  this->LastSimilarity=Evaluate();
  return vtkpxAbstractIntensityRegistration::FinalizeAll();
}

vtkAbstractTransform* vtkpxLinearRegistration::GetTransformation()
{
  if (this->TransformOutputMode==0 || this->UpdateTransform==NULL)
    return this->LinearTransform;

  return this->UpdateTransform;
}

//-------------------------------------------------------------------------
void vtkpxLinearRegistration::GenerateUpdateTransform()
{
  this->UpdateTransform->Identity();

  if (!this->AutoCenterOrigin)
    {
      this->UpdateTransform->Concatenate(this->LinearTransform->GetMatrix());
      return;
    }

  /*  fprintf(stderr,"\n\n ------------------------------------------------\nIn Generate Update Transformation %d %d\n",this->ReferenceOrientation,this->TransformOrientation);
   */

  if (this->ReferenceOrientation==this->TransformOrientation)
    {
      this->LinearTransform->GenerateUpdateTransform(this->UpdateTransform,
						     this->ReferenceImage,this->ReferenceOrientation,
						     this->TransformImage,this->TransformOrientation);
    }
  else
    {

      int   rdim[3],tdim[3],dim2[3];
      double rsp[3],tsp[3],sp2[3];

      this->ReferenceImage->GetSpacing(rsp); this->ReferenceImage->GetDimensions(rdim);
      this->TransformImage->GetSpacing(sp2); this->TransformImage->GetDimensions(dim2);

      vtkpxUtil::ReOrientDimensions(sp2,dim2,
				    this->ReferenceOrientation,
				    this->TransformOrientation,
				    tsp,tdim);
      
      float shift[3],minusshift[3];
      for (int i=0;i<=2;i++)
	{
	  shift[i]=0.5*tsp[i]*double(tdim[i]-1);
	  minusshift[i]=-0.5*rsp[i]*double(rdim[i]-1);
	}

      /*fprintf(stderr,"tdim=%d %d %d dim2=(%d,%d,%d)\n",tdim[0],tdim[1],tdim[2],
	dim2[0],dim2[1],dim2[2]);*/
      this->LinearTransform->GenerateUpdateTransform(this->UpdateTransform,
						     minusshift,this->ReferenceOrientation,
						     shift,this->TransformOrientation);
    }

  //this->PrintInformation();
 
  return;
}
//-------------------------------------------------------------------------
void vtkpxLinearRegistration::PrintInformation()
{
  int i;
  vtkpxLinearTransform *t=this->LinearTransform;
  if (t!=NULL)
    {
      for (i=0; i<12; i++) 
	fprintf(stdout,"%7.5f ",t->Get(i));
      fprintf(stdout,"%5.2f\n",this->Similarity());
    }
}

//-------------------------------------------------------------------------
void vtkpxLinearRegistration::GenerateOutput(int level, int step,int iter,double stepsize,double actualSimilarity,
					     double last_image,double last_smooth)
{
  if (iter>=100)
    iter=99;

  sprintf(this->OutputStatus,"%2d %2d %2d %6.3f:",level,step,iter,stepsize);
  for (int ii=0; ii<15; ii++) 
    {
      if (this->DofStatus->GetComponent(ii,0)>0)
	sprintf(this->OutputStatus,"%s %-.3f ",this->OutputStatus,this->LinearTransform->Get(ii));
    }

  

  sprintf(this->OutputStatus,"%s\t%.3f (%.6f,%.6f) %d\n",this->OutputStatus,actualSimilarity,last_image,last_smooth,this->InterpolationMode);


  if (this->ProvideFeedback) 
    this->GenerateUpdateTransform();
  
  vtkpxAbstractIntensityRegistration::GenerateOutput(level,step,iter,stepsize,actualSimilarity);
  return;
}
//-------------------------------------------------------------------------
double vtkpxLinearRegistration::Evaluate()
{
  this->LastSmoothness=0.0;
  //  fprintf(stderr,"Not Here .........................\n");

  if (this->SampledReferenceWeightImage!=NULL && this->SampledTransformWeightImage!=NULL)
    {
      this->LastSimilarity=vtkpxRegistration::WeightedHistogramEvaluate(this->SampledReferenceImage,this->SampledReferenceWeightImage,
									    this->SampledTransformImage,this->SampledTransformWeightImage,
									    this->LinearTransform,
									    this->Histogram,this->SimilarityMeasure,this->InterpolationMode);
    }
  else if (this->SampledReferenceWeightImage!=NULL)
    {
      this->LastSimilarity=vtkpxRegistration::WeightedHistogramEvaluate(this->SampledReferenceImage,this->SampledReferenceWeightImage,
									this->SampledTransformImage,NULL,
									this->LinearTransform,
									this->Histogram,this->SimilarityMeasure,this->InterpolationMode);
    }
  else
    {
      this->LastSimilarity=vtkpxRegistration::HistogramEvaluate(this->SampledReferenceImage,this->SampledTransformImage,
								this->LinearTransform,
								this->Histogram,this->SimilarityMeasure,this->InterpolationMode);
    }
  return this->LastSimilarity;
}
//-------------------------------------------------------------------------
void vtkpxLinearRegistration::SetCurrentParameters(vtkDoubleArray* param)
{
  int numt=OptimizationStructure.maxdofs;
  int num=param->GetNumberOfTuples();

  if (num!=numt)
    {
      vtkErrorMacro(<<"Bad Data for Optimization");
      return;
    }
  
  this->LinearTransform->SetFreezeUpdates(1);
  int i=0;
  for (i=0;i<3;i++)
    this->LinearTransform->Put(i,param->GetComponent(i,0));
  for (i=3;i<numt;i++)
    this->LinearTransform->Put(i,param->GetComponent(i,0)/this->OptimizationScaleFactor);


  this->LinearTransform->SetFreezeUpdates(0);
  
  this->LinearTransform->UpdateParameters();
}
//-------------------------------------------------------------------------
void vtkpxLinearRegistration::GetCurrentParameters(vtkDoubleArray* param) 
{
  int numt=OptimizationStructure.maxdofs;
  if (param->GetNumberOfComponents()!=2 || param->GetNumberOfTuples()!=numt)
    {
      param->SetNumberOfComponents(2);
      param->SetNumberOfTuples(numt);
    }

  for (int j=0;j<=2;j++)
    {
      param->SetComponent(j,0,this->LinearTransform->Get(j));
      param->SetComponent(j,1,1.0);
    }
  
  for (int i=3;i<numt;i++)
    {
      param->SetComponent(i,0,this->LinearTransform->Get(i)*this->OptimizationScaleFactor);
      param->SetComponent(i,1,this->OptimizationScaleFactor);
    }
  
}
//-------------------------------------------------------------------------
double vtkpxLinearRegistration::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad) 
{
  if (grad->GetNumberOfComponents()!=1 || grad->GetNumberOfTuples()!=OptimizationStructure.maxdofs)
    {
      grad->SetNumberOfComponents(1);
      grad->SetNumberOfTuples(OptimizationStructure.maxdofs);
    }

  grad->FillComponent(0,0.0);
  this->OptimizationStructure.old_similarity=this->OptimizerValue(params);
  double GradientNorm = 0;
  fprintf(stderr,",Grad Step=%.4f (optsc=%.4f),",OptimizationStructure.step,this->OptimizationScaleFactor);

  for (int i=0;i<OptimizationStructure.maxdofs;i++)
    {
      if (DofStatus->GetComponent(i,0)>0) 
	{
	  double orig=params->GetComponent(i,0);
	  params->SetComponent(i, 0, orig + this->OptimizationStructure.step);
	  this->SetCurrentParameters(params);
	  double a=this->Evaluate();

	  params->SetComponent(i, 0, orig - this->OptimizationStructure.step);
	  this->SetCurrentParameters(params);
	  double b=this->Evaluate();

	  params->SetComponent(i, 0, orig);
	  this->SetCurrentParameters(params);

	  double g=-0.5*(b-a)/this->OptimizationStructure.step;
	  grad->SetComponent(i,0,g);
	  GradientNorm+=pow(g,2.0);
	}
    }
  GradientNorm = (sqrt(GradientNorm));


  // -------------------------
  // Hirohito Found this One!!
  // -------------------------
  for (int j=0;j<OptimizationStructure.maxdofs;j++)
    grad->SetComponent(j,0,grad->GetComponent(j,0)/GradientNorm);
      
  return GradientNorm;
}
//-------------------------------------------------------------------------
double vtkpxLinearRegistration::OptimizerValue(vtkDoubleArray* params) 
{
  this->SetCurrentParameters(params);
  return -this->Evaluate();
}
//-------------------------------------------------------------------------
//
//
//          OLD STUFF --- IGNORE
//
//
//-------------------------------------------------------------------------

double vtkpxLinearRegistration::OldOptimize(int maxdofs,double step,double old_similarity,int level,int st)
{

  double similarity=0.0,new_similarity=0.0;
  double last_smooth=0.0,last_image=0.0;

  double actstep=step;
  double actstep2=step*(1.0/this->MeanResolution);

  //fprintf(stderr,"Step=%f  actstep=%f actstep2=%f\n",step,actstep,actstep2);
  
  for (int iter=1;iter<this->NumberOfIterations;iter++)
    {
      int j = 0,k = 0;
      old_similarity = new_similarity = this->Evaluate();
      
      last_image=this->LastSimilarity;
      last_smooth=this->LastSmoothness;


      
      for (int i = 0; i < maxdofs; i++)
	{
	  if (i>=6 && i<=8)
	    step=actstep2;
	  else
	    step=actstep;

	  /*	  if (iter==1)
		  fprintf(stderr,"Param = %d step=%f\n",i,step);*/

	  if (DofStatus->GetComponent(i,0)>0)
	    {
	      double orig=this->LinearTransform->Get(i);
	      this->LinearTransform->Put(i, orig + step);
	      similarity = this->Evaluate();
	      
	      if (similarity > new_similarity)
		{
		  new_similarity = similarity;
		  j =  i;
		  k =  1;
		}
	      
	      this->LinearTransform->Put(i, orig - step);
	      similarity = this->Evaluate();
	      if (similarity > new_similarity)
		{
		  new_similarity = similarity;
		  j =  i;
		  k = -1; 
		  last_image=this->LastSimilarity;
		  last_smooth=this->LastSmoothness;
		}
	      this->LinearTransform->Put(i, orig);
	    }
	}
      
      if (new_similarity > old_similarity)
	{
	  if (j>=6 && j<=8)
	    step=actstep2;
	  else
	    step=actstep;


	  this->LinearTransform->Put(j, this->LinearTransform->Get(j) + k * step);
	  this->GenerateOutput(level,st,iter,step,new_similarity,last_image,last_smooth);
	  old_similarity=new_similarity;
	}
      else
	iter=this->NumberOfIterations+1; // Break Out
    }
  return old_similarity;
}
//-------------------------------------------------------------------------

