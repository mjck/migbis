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

#include "vtkpxNonLinearRegistration.h"
#include "vtkObjectFactory.h"
#include "pxutil.h"
#include "vtkpxUtil.h"
#include "vtkpxImageExtractVOI.h"
#include "vtkpxOptimizer.h"
#include "vtkTimerLog.h"
#include "vtkMath.h"

vtkpxNonLinearRegistration* vtkpxNonLinearRegistration::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxNonLinearRegistration");
  if(ret)
    {
    return (vtkpxNonLinearRegistration*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxNonLinearRegistration;
  
}


vtkpxNonLinearRegistration::vtkpxNonLinearRegistration()
{
  this->LastTargetImage=NULL;
  this->LastTargetWeightImage=NULL;
  this->ImageMask=NULL;
  this->Status=NULL;
  this->ControlPointSpacing=20.0;
  this->ControlPointSpacingRate=1.0;
  this->GridTransform=NULL;
  this->AppendTransformations=0;
  this->InitialTransform=NULL;
  this->DoDiffeomorphic=0;

  this->ComboTransform=NULL;
  this->LinearTransform=NULL;
  this->WarpX=1;
  this->WarpY=1;
  this->WarpZ=1;
  this->Lambda=0.0;
  this->Epsilon=0.003;
  this->WindowSize=1.0;
  this->UpdateTransform=vtkpxComboTransform::New();
  this->UpdateTransform->SetNonLinearFirst(1);
  this->InternalEnergyMode=0;
  this->GradientScale=1.0;
}

vtkpxNonLinearRegistration::~vtkpxNonLinearRegistration() 
{
  if (this->GridTransform!=NULL)
    this->GridTransform->Delete();


  if (this->LinearTransform!=NULL)
    this->LinearTransform->Delete();

  if (this->ComboTransform!=NULL)
    this->ComboTransform->Delete();


  if (this->Status!=NULL)
    this->Status->Delete();

  if (this->LastTargetImage!=NULL)
    this->LastTargetImage->Delete();

  if (this->LastTargetWeightImage!=NULL)
    this->LastTargetWeightImage->Delete();

  this->UpdateTransform->Delete();
  this->SetInitialTransform(NULL);
  this->SetImageMask(NULL);
}

//-------------------------------------------------------------------------
int vtkpxNonLinearRegistration::InitializeAll() 
{
  vtkpxAbstractIntensityRegistration::InitializeAll();
  this->ControlPointSpacing=Frange(this->ControlPointSpacing,
				   this->MinimumResolution,
				   50.0*this->MinimumResolution);
  this->ControlPointSpacingRate=Frange(this->ControlPointSpacingRate,1.0,2.0);

  if (this->NumberOfExtraLevels>0)
    this->AppendTransformations=1;
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxNonLinearRegistration::InitializeTransformation(int level) 
{
  fprintf(stderr,"In vtkpxNonLinearRegistration::InitializeTransform\n");
  double reduction=pow(this->ControlPointSpacingRate,double(level-1.0));

  vtkTimerLog* timer=vtkTimerLog::New();
  timer->StartTimer();

  if (level==this->NumberOfLevels)
    {
      this->LinearTransform=vtkTransform::New();
      this->GridTransform=vtkpxGridTransform::New();
      this->ComboTransform=vtkpxComboTransform::New();
      this->ComboTransform->SetNonLinearFirst(1);
      this->LinearTransform->Identity();

      if (this->InitialTransform==NULL)
	{
	 	  fprintf(stderr,"No initial transformation -- nothing special create grid");
	  this->GridTransform->InitializeDisplacementGrid(this->SampledReferenceImage,
							  this->ControlPointSpacing*reduction,0.0,
							  NULL);
	}
      else if (this->InitialTransform->IsA("vtkLinearTransform")==1)
	{
	  // Just Linear Absorb
	  fprintf(stderr,"Linear initial transformation");
	  vtkLinearTransform* x=(vtkLinearTransform*)(this->InitialTransform);
	  this->LinearTransform->Identity();
	  this->LinearTransform->Concatenate(x->GetMatrix());
	  this->GridTransform->InitializeDisplacementGrid(this->SampledReferenceImage,
							  this->ControlPointSpacing*reduction,0.0);
	}
      else if (this->InitialTransform->IsA("vtkpxComboTransform")==1 || (this->InitialTransform->IsA("vtkpxGridTransform")==1))
	{
	  // Either a Grid or a combo
	  // If Combo get linear component else set linear to identity
	  fprintf(stderr,"Combo/Grid initial transformation");
	  this->LinearTransform->Identity();
	  vtkpxGridTransform* ingrid=NULL;
	  if  (this->InitialTransform->IsA("vtkpxComboTransform")==1)
	    {
	      vtkpxComboTransform* x=(vtkpxComboTransform*)(this->InitialTransform);
	      this->LinearTransform->Concatenate(x->GetLinearTransform()->GetMatrix());
	      this->ComboTransform->SetNonLinearFirst(x->GetNonLinearFirst());
	      ingrid=x->GetGridTransform();
	    } 
	  else
	    {
	      ingrid=(vtkpxGridTransform*)(this->InitialTransform);
	    }
	  
	  if (this->ComboTransform->GetNonLinearFirst()==1)
	    this->GridTransform->InitializeDisplacementGrid(this->SampledReferenceImage,
							    this->ControlPointSpacing*reduction,0.0);
	  else
	    this->GridTransform->InitializeDisplacementGrid(this->SampledReferenceImage,
							    this->ControlPointSpacing*reduction,0.0,
							    this->LinearTransform);
	  this->ComboTransform->SetTransformations(this->LinearTransform,ingrid);
	  
	  if (this->AppendTransformations)
	    this->ComboTransform->AddGridTransform(this->GridTransform);
	  else
	    this->ApproximateTransformationToGrid(this->GridTransform,ingrid,1.0,12,1,0.05*this->MinimumResolution,0.01);
	}
      else
	{
	  fprintf(stderr,"Other initial transformation .. brute force approximate this and hope for the best");
	  this->GridTransform=vtkpxGridTransform::New();
	  this->GridTransform->InitializeDisplacementGrid(this->SampledReferenceImage,
							  this->ControlPointSpacing*reduction,0.0,NULL);
	  fprintf(stderr,"\n -------------------- Approximating Grid Transformation ---------------\n");
	  this->ApproximateTransformationToGrid(this->GridTransform,this->InitialTransform,1.0,12,1,0.05*this->MinimumResolution,0.01);
	  fprintf(stderr,"\n -------------------- Approximating Grid Transformation Done-----------\n");
	}

      this->ComboTransform->SetTransformations(this->LinearTransform,this->GridTransform);
    }
 else if (this->ControlPointSpacingRate>1)
   {
     vtkpxGridTransform* prevTransform=this->GridTransform;
      this->GridTransform=vtkpxGridTransform::New();
      this->GridTransform->InitializeDisplacementGrid(this->SampledReferenceImage,
						      this->ControlPointSpacing*reduction,0.0,
						      NULL);
      
      if (this->AppendTransformations)
	{
	  this->ComboTransform->AddGridTransform(this->GridTransform);
	  
	}
      else
	{
	  if (prevTransform!=NULL)
	    {
	      fprintf(stderr,"\n -------------------- Approximating Grid Transformation ---------------\n");
	      this->ApproximateTransformationToGrid(this->GridTransform,prevTransform);
	      fprintf(stderr,"\n -------------------- Approximating Grid Transformation Done-----------\n");
	    }
	  
	  this->ComboTransform->SetTransformations(this->LinearTransform,this->GridTransform);
	}
      prevTransform->Delete();
    }
  else
    {
      if (this->AppendTransformations)
	{
	  this->GridTransform=vtkpxGridTransform::New();
	  this->GridTransform->InitializeDisplacementGrid(this->SampledReferenceImage,
							  this->ControlPointSpacing*reduction,0.0,
							  NULL);
	  this->ComboTransform->AddGridTransform(this->GridTransform);
	}
      else
	{
	  fprintf(stderr,"Keeping the same transformation and refining");
	}
    }

  this->NumberOfControlPoints=this->GridTransform->GetNumberOfControlPoints();
  timer->StopTimer();
  fprintf(stderr,"In vtkpxNonLinearRegistration::InitializeTransform Done numnonlinear=%d time=%f\n",
	  this->ComboTransform->GetNumberOfGridTransformations(),timer->GetElapsedTime());
  timer->Delete();

  return 1;
}
//-------------------------------------------------------------------------
void vtkpxNonLinearRegistration::ApproximateTransformationToGrid(vtkpxGridTransform* grid,vtkAbstractTransform* tr)
{
  this->ApproximateTransformationToGrid(grid,tr,0.05,20,1,0.0005,0.0);
}

void vtkpxNonLinearRegistration::ApproximateTransformationToGrid(vtkpxGridTransform* grid,vtkAbstractTransform* tr,double step,
								 int numiter,int numlevels,
								 double threshold,double lambda)
{
  grid->ApproximateTransformation(tr,step,numiter,numlevels,threshold,lambda);
}

//-------------------------------------------------------------------------
int vtkpxNonLinearRegistration::Initialize(int level) 
{
  // Different Procedure for toplevel and remaining levels
  // In top level initialize using InitialTransform
  // in other levels initialize using previous grid transform

  vtkpxAbstractIntensityRegistration::Initialize(level);
  this->InitializeTransformation(level);

  if (this->Status!=NULL)
    this->Status->Delete();

  this->Status=vtkDataArray::CreateDataArray(VTK_UNSIGNED_CHAR);
  this->Status->SetNumberOfComponents(3);
  this->Status->SetNumberOfTuples(this->NumberOfControlPoints);

  if (this->WarpX)
    this->Status->FillComponent(0,1);
  else
    this->Status->FillComponent(0,0);
  
  if (this->WarpY)
    this->Status->FillComponent(1,1);
  else
    this->Status->FillComponent(1,0);

  if (this->WarpZ)
    this->Status->FillComponent(2,1);
  else
    this->Status->FillComponent(2,0);
  

  // If Mask == 1 Do not warp
  if (this->ImageMask!=NULL)
    {
      vtkTransform* tr3=vtkTransform::New();
      tr3->Concatenate(this->LinearTransform);
      tr3->Inverse();

      int numactive=0,numtotal=0;

      int dim[3];
      this->ImageMask->GetDimensions(dim);

      double ori[3],sp[3];
      this->ImageMask->GetOrigin(ori);
      this->ImageMask->GetSpacing(sp);
      for (int i=0;i<this->NumberOfControlPoints;i++)
	{
	  double p[3];
	  int   q[3];
	  this->GridTransform->GetControlPointLocation(i,p);
	  tr3->TransformPoint(p,p);
	  for (int ia=0;ia<=2;ia++)
	    q[ia]=int((p[ia]-ori[ia])/sp[ia]);

	  ++numtotal;
	  if (q[0]>=0 && q[0]<dim[0] && q[1]>=0 && q[1]<dim[1] && q[2]>=0 && q[2]<dim[2])
	    {
	      if (this->ImageMask->GetScalarComponentAsDouble(q[0],q[1],q[2],0)==0)
		{
		  for (int ib=0;ib<=2;ib++)
		    this->Status->SetComponent(i,ib,0);
		  ++numactive;
		}
	    }
	  else
	    {
	      for (int ib=0;ib<=2;ib++)
		this->Status->SetComponent(i,ib,0);
	      ++numactive;
	    }
	}
      tr3->Delete();
      numactive=numtotal-numactive;
      fprintf(stderr,"Total Control Points = %d Active = %d (%5.2f)\n",
	      numtotal,numactive,double(numactive*100.0)/double(numtotal));
    }


  if (this->LastTargetImage!=NULL)
    this->LastTargetImage->Delete();
  this->LastTargetImage=vtkImageData::New();

  if (this->LastTargetWeightImage!=NULL)
    this->LastTargetWeightImage->Delete();
  this->LastTargetWeightImage=vtkImageData::New();

  double sp[3]; this->GridTransform->GetGridSpacing(sp);
  int    d[3]; this->GridTransform->GetGridDimensions(d);
  sprintf(this->OutputStatus,"\n Transformation Spacing %.2f %.2f %.2f dimensions=%d x %d x %d\n",sp[0],sp[1],sp[2],d[0],d[1],d[2]);
  this->UpdateProgress(this->LastProgress);

  return 1;
}
//-------------------------------------------------------------------------
int vtkpxNonLinearRegistration::FinalizeAll()
{
  this->LastTargetImage->Delete();
  this->LastTargetWeightImage->Delete();
  this->LastTargetImage=NULL;
  this->LastTargetWeightImage=NULL;
  this->Status->Delete();
  this->Status=NULL;
  return vtkpxAbstractIntensityRegistration::FinalizeAll();
}
//-------------------------------------------------------------------------
// Compute Registration
int vtkpxNonLinearRegistration::ComputeRegistration(int level)
{  

  double step=this->CurrentStepSize;

  double origin[3],spacing[3],gridspacing[3];
  int   dimensions[3];
  this->GridTransform->GetDisplacementGrid()->GetSpacing(gridspacing);
  this->SampledReferenceImage->GetOrigin(origin);
  this->SampledReferenceImage->GetSpacing(spacing);
  this->SampledReferenceImage->GetDimensions(dimensions);
  
  vtkDoubleArray* grad=vtkDoubleArray::New();
  grad->SetNumberOfComponents(3);
  grad->SetNumberOfTuples(this->NumberOfControlPoints);


  for (int st=1;st<=this->NumberOfSteps;st++)
    {
      double old_similarity = this->Evaluate();
      double new_similarity=this->Optimize(grad,old_similarity,step,level,
					  origin,spacing,dimensions,gridspacing);
      this->GenerateOutput(level,st,this->NumberOfIterations,step,new_similarity);
      step*=0.5;
    }

  grad->Delete();
  return 1;
}
//-------------------------------------------------------------------------

double vtkpxNonLinearRegistration::Optimize(vtkDoubleArray* grad,double old_similarity,
					   double step,int level,
					   double origin[3],double spacing[3],
					   int dimensions[3],double gridspacing[3])
{
  OptimizationStructure.SetValues(old_similarity,step,level,origin,spacing,dimensions,gridspacing);
     
  vtkpxOptimizer* optimizer=vtkpxOptimizer::New();
  vtkDoubleArray* position=vtkDoubleArray::New();
  vtkDoubleArray* temp=vtkDoubleArray::New();

  this->GetCurrentParameters(position);
  optimizer->SetMasterObject(this);
  optimizer->SetTempArray(grad);
  optimizer->SetEvaluateFunctionMethod(&vtkpxNonLinearRegistration::CalculateFunction);
  optimizer->SetEvaluateGradientMethod(&vtkpxNonLinearRegistration::CalculateGradient);
  optimizer->SetFeedbackMethod(&vtkpxNonLinearRegistration::OptimizerFeedback);
  optimizer->SetBeginIterationMethod(&vtkpxAbstractIntensityRegistration::OnBeginIteration);

  double v=0.0;


  fprintf(stderr,"In Non Linear Reg :: Optimize :: %d\n",this->OptimizationMethod);

  switch (this->OptimizationMethod)
    {
    case 2:
      v=optimizer->ComputePowell(position,temp,this->NumberOfIterations,0.001);
      break;

    case 3:
      v=optimizer->ComputeGradientDescent(position,this->NumberOfIterations,0.001);
      break;

    case 4:
      v=optimizer->ComputeConjugateGradient(position,this->NumberOfIterations,0.001);
      break;

    case 5:
      v=optimizer->ComputeGradientDescent(position,this->NumberOfIterations,0.001);
      if (level==1)
	v=optimizer->ComputeConjugateGradient(position,this->NumberOfIterations,0.001);
      break;

    default:
	v=optimizer->ComputeConjugateGradient(position,this->NumberOfIterations,0.001);
    }

  optimizer->Delete();
  position->Delete();
  temp->Delete();
  return v;
}
//-------------------------------------------------------------------------
double vtkpxNonLinearRegistration::Evaluate()
{
  if (this->LastTargetImage==NULL)
    this->LastTargetImage=vtkImageData::New();
  
  vtkpxRegistration::ResliceImage(this->LastTargetImage,this->SampledTransformImage,
				  this->SampledReferenceImage,this->ComboTransform,this->InterpolationMode);
  
  if (this->SampledReferenceWeightImage!=NULL && this->SampledTransformWeightImage!=NULL)
    {
      if (this->LastTargetWeightImage==NULL)
	this->LastTargetWeightImage=vtkImageData::New();

      vtkpxRegistration::ResliceImage(this->LastTargetWeightImage,this->SampledTransformWeightImage,
				      this->SampledReferenceImage,this->ComboTransform,this->InterpolationMode);

      //      fprintf(stderr,"Calling Dual Weighted Fill Histogram\n");
      this->Histogram->DualWeightedFillHistogram(this->SampledReferenceImage,this->LastTargetImage,
					     this->SampledReferenceWeightImage,this->LastTargetWeightImage,1);  
    }
  else   if (this->SampledReferenceWeightImage!=NULL)
    {
      //      fprintf(stderr,"Calling Weighted Fill Histogram\n");
      this->Histogram->WeightedFillHistogram(this->SampledReferenceImage,this->LastTargetImage,
					     this->SampledReferenceWeightImage,1);  
      
    }
  else
    {
      this->Histogram->FillHistogram(this->SampledReferenceImage,this->LastTargetImage,1);  
    }
  this->LastSimilarity=this->Similarity(this->Histogram);

  if (this->DoDiffeomorphic>0)
    this->LastSmoothness=this->GridTransform->ComputeJacobianConstraint(0.3);
  else if (this->Lambda>0.000001)
    this->LastSmoothness=this->ComputeInternalEnergy(this->GridTransform);
  else
    this->LastSmoothness=0.0;
  return this->LastSimilarity-this->LastSmoothness*this->Lambda;
}
//-------------------------------------------------------------------------
double vtkpxNonLinearRegistration::EvaluatePiece(vtkImageData* reference,
						 vtkImageData* target,
						 vtkImageData* previoustarget,
						 vtkImageData* weights,
						 vtkImageData* weights2,
						 int cpoint)

{
  this->Histogram->BackupHistogram();

  vtkImageData* tmp=vtkImageData::New();
  vtkpxRegistration::ResliceImage(tmp,target,reference,this->ComboTransform,this->InterpolationMode);

  // Remove Last Target Value

  // Add New Value

  if (weights!=NULL && weights2!=NULL)
    {
      //      fprintf(stderr,"Calling Dual Weighted Remove and Weighted Fill Histogram weights=%d, weights2=%d\n",weights,weights2);
      this->Histogram->DualWeightedRemoveFromHistogram(reference,previoustarget,weights,weights2);
      this->Histogram->DualWeightedFillHistogram(reference,tmp,weights,weights2,0);
    }
  else if (weights!=NULL)
    {
      //      fprintf(stderr,"Calling Weighted Remove and Weighted Fill Histogram\n");
      this->Histogram->WeightedRemoveFromHistogram(reference,previoustarget,weights);
      this->Histogram->WeightedFillHistogram(reference,tmp,weights,0);
    }
  else
    {
      this->Histogram->RemoveFromHistogram(reference,previoustarget);
      this->Histogram->FillHistogram(reference,tmp,0);
    }
  // Clean Up
  tmp->Delete();

  // Return Value
  double e1=this->Similarity(this->Histogram);

  this->Histogram->ResetHistogram();

  double l1=0.0;
  if (this->Lambda>0.000001)
    l1 = this->ComputeInternalEnergy(this->GridTransform,cpoint)*double(NumberOfControlPoints);
  double s1=e1-this->Lambda*l1;
  
  return s1;
}

//-------------------------------------------------------------------------
void vtkpxNonLinearRegistration::PrintInformation() 
{
  
}
//-------------------------------------------------------------------------

void vtkpxNonLinearRegistration::GenerateOutput(int level, int step,int iter,double stepsize,double actualSimilarity)
{
  if (iter>=100)
    iter=99;

  sprintf(this->OutputStatus,"Level=%2d Step=%2d iter=%2d stepsize=%.3f: tot=%8.6f (sim=%8.6f sm=%8.6f (dist=%.2f))\n",
	  level,step,iter,stepsize,actualSimilarity,
	  this->LastSimilarity,this->LastSmoothness*this->Lambda,
	  this->LastSmoothness);
  this->UpdateProgress(this->LastProgress);

  return;
}

//-------------------------------------------------------------------------
vtkAbstractTransform* vtkpxNonLinearRegistration::GetUpdateTransform()
{
  if (this->SampledReferenceImage==NULL || this->GridTransform==NULL)
    return this->UpdateTransform;

  return this->ComboTransform;
}

//-------------------------------------------------------------------------
// Optimizer Stuff 
//-------------------------------------------------------------------------
void vtkpxNonLinearRegistration::SetCurrentParameters(vtkDoubleArray* param)
{
  this->GridTransform->SetCurrentParameters(param);
}
//-------------------------------------------------------------------------
void vtkpxNonLinearRegistration::GetCurrentParameters(vtkDoubleArray* param)
{
  this->GridTransform->GetCurrentParameters(param);
}
//-------------------------------------------------------------------------
double vtkpxNonLinearRegistration::OptimizerValue(vtkDoubleArray* params)
{
  /*  double sum=0.0;
  int nt=params->GetNumberOfTuples();
  for (int i=0;i<nt;i++)
    sum+=params->GetComponent(i,0);
    fprintf(stdout,"Params sum=%f\n",sum);*/
  this->SetCurrentParameters(params);
  
  return -this->Evaluate();
}
//-------------------------------------------------------------------------
double vtkpxNonLinearRegistration::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)

{
  if (grad->GetNumberOfComponents()!=1 || grad->GetNumberOfTuples()!=this->NumberOfControlPoints*3)
    {
      grad->SetNumberOfComponents(1);
      grad->SetNumberOfTuples(this->NumberOfControlPoints*3);
    }

  OptimizationStructure.old_similarity=this->OptimizerValue(params);

  // Calculate gradient 
  double GradientNorm = 0;

  int per=this->NumberOfControlPoints/10;
  int count=0;
  int count2=0;
  
  grad->FillComponent(0,0.0);

  sprintf(this->OutputStatus,"");
  fprintf(stderr,"\n");
  this->GenerateOutput(OptimizationStructure.level,
  		       -1,-1,OptimizationStructure.step,
  		       OptimizationStructure.old_similarity);
  fprintf(stderr,"Optimizer Computing Gradients : (%5.3f)",OptimizationStructure.step);

  //int mid=this->NumberOfControlPoints;

  double tvoi=0.0,tpiece=0.0;
  vtkTimerLog* timer=vtkTimerLog::New();

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

      if (this->Status->GetComponent(i,0)+this->Status->GetComponent(i,1)+this->Status->GetComponent(i,2) > 0 )
	{
	  double p[3]; this->GridTransform->GetControlPointLocation(i,p);
	  
	  // Extract Relevant Portion of the Image
	  timer->StartTimer();

	  vtkpxImageExtractVOI* voi=vtkpxImageExtractVOI::New();
	  voi->SetInput(this->SampledReferenceImage);
	  voi->SetRegion(p,OptimizationStructure.gridspacing,this->WindowSize);
	  voi->Update();

	  /*	  if (i ==33)
	    {
	      int bounds[6]; voi->GetVOI(bounds);
	      fprintf(stderr,"Control Point %d bounds=%d:%d %d;%d, %d:%d p=%f,%f,%f sp=%f,%f,%f w=%d\n",i,bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5],
		      p[0],p[1],p[2],
		      OptimizationStructure.gridspacing[0],		      
		      OptimizationStructure.gridspacing[1],		      
		      OptimizationStructure.gridspacing[2],
		      this->WindowSize);
		      }*/


	  vtkpxImageExtractVOI* voi2=vtkpxImageExtractVOI::New();
	  voi2->SetInput(this->LastTargetImage);
	  voi2->SetRegion(p,OptimizationStructure.gridspacing,this->WindowSize);
	  voi2->Update();

	  vtkpxImageExtractVOI* voi3=NULL;
	  vtkImageData* roi3image=NULL;

	  vtkpxImageExtractVOI* voi4=NULL;
	  vtkImageData* roi4image=NULL;
	  if (this->SampledReferenceWeightImage!=NULL)
	    {
	      voi3=vtkpxImageExtractVOI::New();
	      voi3->SetInput(this->SampledReferenceWeightImage);
	      voi3->SetRegion(p,OptimizationStructure.gridspacing,this->WindowSize);
	      voi3->Update();
	      roi3image=voi3->GetOutput();
	    }

	  // Approximation here, do not mess with estimate of weight image ....
	  if (this->SampledTransformWeightImage!=NULL)
	    {
	      //	      fprintf(stderr,"*********** Creating voi4\n");
	      voi4=vtkpxImageExtractVOI::New();
	      voi4->SetInput(this->LastTargetWeightImage);
	      voi4->SetRegion(p,OptimizationStructure.gridspacing,this->WindowSize);
	      voi4->Update();
	      roi4image=voi4->GetOutput();
	    }

	  timer->StopTimer();
	  tvoi+=timer->GetElapsedTime();
	  timer->StartTimer();

	  for (int j=0;j<=2;j++)
	    {
	      if (this->Status->GetComponent(i,j)>0)
		{
		  this->GridTransform->ShiftControlPoint(i,j,-OptimizationStructure.step);
		  //		  if (i==0)   fprintf(stdout,"Shifted Control Point %i comp %d-> %f\n",i,j,this->GridTransform->GetControlPointDisplacement(i,j));
		  double s1 = this->EvaluatePiece(voi->GetOutput(),
						  this->SampledTransformImage,
						  voi2->GetOutput(),
						  roi3image,roi4image,
						 i);
		  this->GridTransform->ShiftControlPoint(i,j,+2.0*OptimizationStructure.step);
		  //		  if (i==0)   fprintf(stdout,"Shifted Control Point %i comp %d-> %f\n",i,j,this->GridTransform->GetControlPointDisplacement(i,j));
		  double s2 = this->EvaluatePiece(voi->GetOutput(),
						  this->SampledTransformImage,
						  voi2->GetOutput(),
						  roi3image,roi4image,
						 i);
		  this->GridTransform->ShiftControlPoint(i,j,-OptimizationStructure.step);
		  //		  if (i==0)   fprintf(stdout,"Shifted Control Point %i comp %d-> %f\n",i,j,this->GridTransform->GetControlPointDisplacement(i,j));
		  
		  // Minus sign for ascent 
		  grad->SetComponent(i*3+j,0,-(s2-s1));
		  /*		  if (i==33)
				  fprintf(stdout,"Grad Component i=%d j=%d -->%f (s1=%f s2=%f)\n",i,j,grad->GetComponent(i*3+j,0),s1,s2);*/
		  GradientNorm+=pow(s2-s1,2.0);
		  //}
		}
	    }
	  timer->StopTimer();
	  tpiece+=timer->GetElapsedTime();

	  voi->Delete();
	  voi2->Delete();
	  if (voi3!=NULL)
	    voi3->Delete();
	  if (voi4!=NULL)
	    voi4->Delete();
	}
    }

  timer->Delete();
  GradientNorm = (sqrt(GradientNorm));
  // Changing this one 
  //  double sc=OptimizationStructure.step/GradientNorm;
  double sc=this->GradientScale*OptimizationStructure.step/GradientNorm;

  // Normalize vector
  if (GradientNorm > 0)
    this->GridTransform->ScaleGradients(grad,sc);
  else 
    grad->FillComponent(0,0.0);
  fprintf(stderr,"\tGradient Norm=%.5f scale=%.5f (tvoi=%f tpiece=%f)\n",GradientNorm,sc,tvoi,tpiece);
  return GradientNorm;

}

//-------------------------------------------------------------------------

double vtkpxNonLinearRegistration::ComputeInternalEnergy(vtkpxGridTransform* tr,int cpindex)
{
  double v1=0.0;

  if (this->InternalEnergyMode==0)
    {
      // Bending Energy
      if (cpindex==-1)
	{
	  v1=tr->ComputeTotalBendingEnergy();
	}
      else
	{
	  v1=tr->ComputeBendingEnergyAtControlPoint(cpindex);
	}
    }
  else
    { 
      if (cpindex==-1)
	{
	  v1=tr->ComputeTotalStrainEnergy(0.45);
	}
      else
	{
	  v1=tr->ComputeStrainEnergyAtControlPoint(cpindex,0.45);
	}
    }

  return v1;
}


//-------------------------------------------------------------------------
double vtkpxNonLinearRegistration::OptimizerGradientGlobal(vtkDoubleArray* params,vtkDoubleArray* grad)
{
  if (grad->GetNumberOfComponents()!=1 || grad->GetNumberOfTuples()!=this->NumberOfControlPoints*3)
    {
      grad->SetNumberOfComponents(1);
      grad->SetNumberOfTuples(this->NumberOfControlPoints*3);
    }

  OptimizationStructure.old_similarity=this->OptimizerValue(params);

  // Calculate gradient 
  double GradientNorm = 0;

  grad->FillComponent(0,0.0);

  //  this->GenerateOutput(OptimizationStructure.level,
  //		       -1,-1,OptimizationStructure.step,
  //		       OptimizationStructure.old_similarity);
  fprintf(stderr,"\n\n%s\nOptimizer Computing Gradients : (%5.3f)",OutputStatus,OptimizationStructure.step);

  //int mid=this->NumberOfControlPoints;

  int nparam=params->GetNumberOfTuples();
  double stepsize=OptimizationStructure.step;

  for (int i = 0; i < nparam; i++)
    {
      params->SetComponent(i,0,params->GetComponent(i,0)-stepsize);
      double s1 = this->OptimizerValue(params);
      
      params->SetComponent(i,0,params->GetComponent(i,0)+2*stepsize);
      double s2 = this->OptimizerValue(params);

      params->SetComponent(i,0,params->GetComponent(i,0)-stepsize);

      grad->SetComponent(i,0,-(s2-s1));
      GradientNorm+=pow(s2-s1,2.0);
    }

  GradientNorm = (sqrt(GradientNorm));
  double sc=OptimizationStructure.step/GradientNorm;

  if (GradientNorm > 0)
    this->GridTransform->ScaleGradients(grad,sc);
  else 
    grad->FillComponent(0,0.0);
  fprintf(stderr,"Gradient Norm=%.5f scale=%.5f\n",GradientNorm,sc);
  return GradientNorm;

}

