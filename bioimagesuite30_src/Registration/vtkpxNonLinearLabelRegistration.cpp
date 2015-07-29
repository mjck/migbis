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

#include "vtkpxNonLinearLabelRegistration.h"
#include "vtkObjectFactory.h"
#include "pxutil.h"
#include "vtkpxUtil.h"
#include "vtkpxImageExtractVOI.h"
#include "vtkpxOptimizer.h"
#include "vtkTimerLog.h"
#include "vtkCollection.h"
#include "vtkImageCast.h"
#include <vtkstd/algorithm>
#include "vtkpxMath.h"

vtkpxNonLinearLabelRegistration* vtkpxNonLinearLabelRegistration::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxNonLinearLabelRegistration");
  if(ret)
    {
    return (vtkpxNonLinearLabelRegistration*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxNonLinearLabelRegistration;
  
}


vtkpxNonLinearLabelRegistration::vtkpxNonLinearLabelRegistration()
{
  this->LastTargetImage=NULL;
  this->ControlPointSpacing=20.0;
  this->ControlPointSpacingRate=1.0;
  this->GridTransform=NULL;
  this->AppendTransformations=0;
  this->InitialTransform=NULL;

  this->ComboTransform=NULL;
  this->LinearTransform=NULL;

  this->Lambda=0.0;
  this->Epsilon=0.003;
  this->WindowSize=1.0;
  this->UpdateTransform=vtkpxComboTransform::New();
  this->UpdateTransform->SetNonLinearFirst(1);
  this->InternalEnergyMode=0;
  this->GradientScale=1.0;
  this->CombinationMetric=1;
  this->similarityarray=NULL;
}

vtkpxNonLinearLabelRegistration::~vtkpxNonLinearLabelRegistration() 
{
  if (this->GridTransform!=NULL)
    this->GridTransform->Delete();


  if (this->LinearTransform!=NULL)
    this->LinearTransform->Delete();

  if (this->ComboTransform!=NULL)
    this->ComboTransform->Delete();



  if (this->LastTargetImage!=NULL)
    this->LastTargetImage->Delete();


  if (this->similarityarray!=NULL)
    delete [] this->similarityarray;

  this->UpdateTransform->Delete();
  this->SetInitialTransform(NULL);

}
//-------------------------------------------------------------------------
void vtkpxNonLinearLabelRegistration::SetReferenceImage(vtkImageData* ref,int frame)
{

  // Ignoring frame here ...
  if (ref==NULL)
    return;

  int nc=ref->GetNumberOfScalarComponents();
  if (nc<1)
    return;

  if (this->ReferenceImage==NULL)
    this->ReferenceImage=vtkImageData::New();

  vtkImageCast* cst=vtkImageCast::New();
  cst->SetInput(ref);
  cst->SetOutputScalarTypeToShort();
  cst->Update();

  this->ReferenceImage->ShallowCopy(cst->GetOutput());
  cst->Delete();

  if (this->similarityarray!=NULL)
    delete [] this->similarityarray;
  this->similarityarray=new float[nc];
}


void vtkpxNonLinearLabelRegistration::SetTransformImage(vtkImageData* img,int frame)
{
  if (img==NULL)
    return;

  if (this->TransformImage==NULL)
    this->TransformImage=vtkImageData::New();

  vtkpxRegistration::ExtractComponentAndCastImageToShort(this->TransformImage,img,frame,0);
}
//-------------------------------------------------------------------------
int vtkpxNonLinearLabelRegistration::InitializeAll() 
{
  vtkpxAbstractIntensityRegistration::InitializeAll();
  this->ControlPointSpacing=Frange(this->ControlPointSpacing,
				   this->MinimumResolution,
				   50.0*this->MinimumResolution);
  this->ControlPointSpacingRate=Frange(this->ControlPointSpacingRate,1.0,2.0);

  if (this->NumberOfExtraLevels>0)
    this->AppendTransformations=1;

  this->NumberOfLevels=1;
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxNonLinearLabelRegistration::InitializeTransformation(int level) 
{
  fprintf(stderr,"In vtkpxNonLinearLabelRegistration::InitializeTransform level=%d\n",level);
  double reduction=pow(this->ControlPointSpacingRate,double(level-1.0));

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
	    this->GridTransform->ApproximateTransformation(ingrid,1.0,12,1,0.05*this->MinimumResolution,0.01);
	}
      else
	{
	  fprintf(stderr,"Other initial transformation .. brute force approximate this and hope for the best");
	  this->GridTransform=vtkpxGridTransform::New();
	  this->GridTransform->InitializeDisplacementGrid(this->SampledReferenceImage,
							  this->ControlPointSpacing*reduction,0.0,NULL);
	  fprintf(stderr,"\n -------------------- Approximating Grid Transformation ---------------\n");
	  this->GridTransform->ApproximateTransformation(this->InitialTransform,1.0,12,1,0.05*this->MinimumResolution,0.01);
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
	      this->GridTransform->ApproximateTransformation(prevTransform);
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
  fprintf(stderr,"In vtkpxNonLinearLabelRegistration::InitializeTransform Done numnonlinear=%d\n",
	  this->ComboTransform->GetNumberOfGridTransformations());
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxNonLinearLabelRegistration::Initialize(int level) 
{
  // Different Procedure for toplevel and remaining levels
  // In top level initialize using InitialTransform
  // in other levels initialize using previous grid transform

  double reduction=pow(this->ResolutionRate,double(level-1.0));
  this->CurrentStepSize=this->InternalStepSize*reduction;
  this->CurrentResolution=this->InternalResolution*reduction;
  this->InternalBlurFactor=0.0;

  if (this->SampledReferenceImage==NULL)
    this->SampledReferenceImage=vtkImageData::New();
  if (this->SampledTransformImage==NULL)
    this->SampledTransformImage=vtkImageData::New();

  // -resolution = Nearest Neighbor!
  this->SampledReferenceImage->DeepCopy(this->ReferenceImage);
  this->SampledTransformImage->DeepCopy(this->TransformImage);
  /*  vtkpxRegistration::ResampleAndSmoothImage(this->SampledReferenceImage,this->ReferenceImage,
					    this->BaseResolution,
					    0.0,
					    -this->CurrentResolution,0);

   
  //  vtkpxRegistration::PrintImageInfo(this->SampledReferenceImage,"ResandSmooth");

  vtkpxRegistration::ResampleAndSmoothImage(this->SampledTransformImage,this->TransformImage,
					    this->BaseResolution,
					    0.0,
					    -this->CurrentResolution,0);*/

  short minr=0,maxr=1,mint=0,maxt=1;
  int num_ref,num_tar;

  sprintf(this->OutputStatus,"Label Registration --- Going to Calc Range and Adjust \n\n");
  this->UpdateProgress(this->LastProgress);

  
  if (this->Histogram!=NULL)
    this->Histogram->Delete();
  this->Histogram=NULL;

  this->HistogramCollection=vtkCollection::New();
  int nframes=this->SampledReferenceImage->GetNumberOfScalarComponents();

  double r1[2]; this->SampledTransformImage->GetPointData()->GetScalars()->GetRange(r1);

  double r2[2];
  for (int ia=0;ia<nframes;ia++)
    {
      this->SampledReferenceImage->GetPointData()->GetScalars()->GetRange(r2,ia);
      vtkpxJointHistogram* hist=vtkpxJointHistogram::New();
      int r_bins=int(r2[1]);
      int t_bins=int(r1[1]);

      hist->Allocate(r_bins,t_bins);
      fprintf(stderr,"------------------------- Label Reg -- Allocating Histogram ia=%d  (%d x %d)\n",ia,r_bins,t_bins);
      this->HistogramCollection->AddItem(hist);
      //      hist->Delete();
    }

  double sp1[3],sp2[3],or1[3],or2[3];
  int   dim1[3],dim2[3];

  this->SampledReferenceImage->GetSpacing(sp1);
  this->SampledReferenceImage->GetDimensions(dim1);
  this->SampledReferenceImage->GetOrigin(or1);
  this->SampledTransformImage->GetSpacing(sp2);
  this->SampledTransformImage->GetDimensions(dim2);
  this->SampledTransformImage->GetOrigin(or2);

  sprintf(this->OutputStatus,"\nEnd Of Initialization Of Level =%d,%d",level,this->NumberOfLevels);
  sprintf(this->OutputStatus,
	  "%s\n Reference = %d x %d x %d (%.2f %.2f %.2f) (or=%.1f,%.1f,%.1f), Range=%.0f:%.0f frames=%d\n",
	  this->OutputStatus,dim1[0],dim1[1],dim1[2],sp1[0],sp1[1],sp1[2],or1[0],or1[1],or1[2],
	  r2[0],r2[1],nframes);
  sprintf(this->OutputStatus, 
	  "%s\nTransform  = %d x %d x %d (%.2f %.2f %.2f) (or=%.1f,%.1f,%.1f), Range=%.0f:%.0f\n",
	  this->OutputStatus,dim2[0],dim2[1],dim2[2],sp2[0],sp2[1],sp2[2],or2[0],or2[1],or2[2],
	  r1[0],r1[1]);

  
  vtkDebugMacro(<<this->OutputStatus);
  //fprintf(stderr,"%s\n",this->OutputStatus);
  this->UpdateProgress(this->LastProgress);


  fprintf(stderr,"Going to initialize transformation ...........\n");
  this->InitializeTransformation(level);
  fprintf(stderr,"Done with initialize transformation ...........\n");

  if (this->LastTargetImage!=NULL)
    this->LastTargetImage->Delete();
  this->LastTargetImage=vtkImageData::New();


  double sp[3]; this->GridTransform->GetGridSpacing(sp);
  int    d[3]; this->GridTransform->GetGridDimensions(d);
  sprintf(this->OutputStatus,"\n Transformation Spacing %.2f %.2f %.2f dimensions=%d x %d x %d\n",sp[0],sp[1],sp[2],d[0],d[1],d[2]);

  sprintf(this->OutputStatus,"\n Combination Metric=%d",this->CombinationMetric);

  this->UpdateProgress(this->LastProgress);




  return 1;
}
//-------------------------------------------------------------------------
int vtkpxNonLinearLabelRegistration::FinalizeAll()
{
  this->LastTargetImage->Delete();
  this->LastTargetImage=NULL;

  return vtkpxAbstractIntensityRegistration::FinalizeAll();
}
//-------------------------------------------------------------------------
// Compute Registration
int vtkpxNonLinearLabelRegistration::ComputeRegistration(int level)
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
      fprintf(stderr,"Computing Optimization for Step %d/%d\n",st,this->NumberOfSteps);
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

double vtkpxNonLinearLabelRegistration::Optimize(vtkDoubleArray* grad,double old_similarity,
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
  optimizer->SetEvaluateFunctionMethod(&vtkpxNonLinearLabelRegistration::CalculateFunction);
  optimizer->SetEvaluateGradientMethod(&vtkpxNonLinearLabelRegistration::CalculateGradient);
  optimizer->SetFeedbackMethod(&vtkpxNonLinearLabelRegistration::OptimizerFeedback);
  optimizer->SetBeginIterationMethod(&vtkpxAbstractIntensityRegistration::OnBeginIteration);

  double v=0.0;


  fprintf(stderr,"In Non Label Linear Reg :: Optimize :: %d\n",this->OptimizationMethod);

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
double vtkpxNonLinearLabelRegistration::Evaluate()
{
  if (this->LastTargetImage==NULL)
    this->LastTargetImage=vtkImageData::New();
  
  fprintf(stderr,"Optimizing Value\n");

  vtkpxRegistration::ResliceImage(this->LastTargetImage,this->SampledTransformImage,
				  this->SampledReferenceImage,this->ComboTransform,0);
  

  int nframes=this->SampledReferenceImage->GetNumberOfScalarComponents();
  double sum=0.0;
  for (int ia=0;ia<nframes;ia++)
    {
      vtkpxJointHistogram* hist=(vtkpxJointHistogram*)(this->HistogramCollection->GetItemAsObject(ia));
      hist->FillHistogramComponent(this->SampledReferenceImage,this->LastTargetImage,ia,0,1);  
      double v=this->Similarity(hist);
      this->similarityarray[ia]=v;
    }

  this->LastSimilarity=this->ComputeCombinedValue(this->similarityarray,nframes);
  if (this->Lambda>0.000001)
    this->LastSmoothness=this->ComputeInternalEnergy(this->GridTransform);
  else
    this->LastSmoothness=0.0;
  return this->LastSimilarity-this->LastSmoothness*this->Lambda;
}
//-------------------------------------------------------------------------
double vtkpxNonLinearLabelRegistration::EvaluatePiece(vtkImageData* reference,
						 vtkImageData* target,
						 vtkImageData* previoustarget,
						 int cpoint)

{
  //  fprintf(stderr,"Going to evaluete piece\n");

  vtkImageData* tmp=vtkImageData::New();
  vtkpxRegistration::ResliceImage(tmp,target,reference,this->ComboTransform,0);

  // Remove Last Target Value

  // Add New Value

  int nframes=this->SampledReferenceImage->GetNumberOfScalarComponents();
  double sume1=0.0;
  double minvalue=0.0;
  for (int ia=0;ia<nframes;ia++)
    {
      vtkpxJointHistogram* hist=(vtkpxJointHistogram*)(this->HistogramCollection->GetItemAsObject(ia));
      hist->BackupHistogram();
      hist->RemoveFromHistogramComponent(reference,previoustarget,ia,0);
      hist->FillHistogramComponent(reference,tmp,ia,0,0);
      this->similarityarray[ia]=this->Similarity(hist);
      hist->ResetHistogram();
    }

  double e1=this->ComputeCombinedValue(this->similarityarray,nframes);

  // Clean Up
  tmp->Delete();
  double l1=0.0;
  if (this->Lambda>0.000001)
    l1 = this->ComputeInternalEnergy(this->GridTransform,cpoint)*double(NumberOfControlPoints);
  double s1=e1-this->Lambda*l1;
 
  //  fprintf(stderr,"done with evaluate piece\n");
  return s1;
}

//-------------------------------------------------------------------------
void vtkpxNonLinearLabelRegistration::PrintInformation() 
{
  
}
//-------------------------------------------------------------------------

void vtkpxNonLinearLabelRegistration::GenerateOutput(int level, int step,int iter,double stepsize,double actualSimilarity)
{
  if (iter>=100)
    iter=99;

  sprintf(this->OutputStatus,"Level=%2d Step=%2d iter=%2d stepsize=%.3f: tot=%8.6f (sim=%8.6f sm=%8.6f (dist=%.2f))",
	  level,step,iter,stepsize,actualSimilarity,
	  this->LastSimilarity,this->LastSmoothness*this->Lambda,
	  this->LastSmoothness);
  this->UpdateProgress(this->LastProgress);
  vtkpxAbstractIntensityRegistration::GenerateOutput(level,step,iter,stepsize,actualSimilarity);
  return;
}

//-------------------------------------------------------------------------
vtkAbstractTransform* vtkpxNonLinearLabelRegistration::GetUpdateTransform()
{
  if (this->SampledReferenceImage==NULL || this->GridTransform==NULL)
    return this->UpdateTransform;

  return this->ComboTransform;
}

//-------------------------------------------------------------------------
// Optimizer Stuff 
//-------------------------------------------------------------------------
void vtkpxNonLinearLabelRegistration::SetCurrentParameters(vtkDoubleArray* param)
{
  this->GridTransform->SetCurrentParameters(param);
}
//-------------------------------------------------------------------------
void vtkpxNonLinearLabelRegistration::GetCurrentParameters(vtkDoubleArray* param)
{
  this->GridTransform->GetCurrentParameters(param);
}
//-------------------------------------------------------------------------
double vtkpxNonLinearLabelRegistration::OptimizerValue(vtkDoubleArray* params)
{
  this->SetCurrentParameters(params);
  return -this->Evaluate();
}
//-------------------------------------------------------------------------
double vtkpxNonLinearLabelRegistration::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)

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

  
  this->GenerateOutput(OptimizationStructure.level,
		       -1,-1,OptimizationStructure.step,
		       OptimizationStructure.old_similarity);
  fprintf(stderr,"\n\n%s NonLinear Label nOptimizer Computing Gradients : (%5.3f)",OutputStatus,OptimizationStructure.step);

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

      double p[3]; this->GridTransform->GetControlPointLocation(i,p);
      
      // Extract Relevant Portion of the Image
      timer->StartTimer();
      
      vtkpxImageExtractVOI* voi=vtkpxImageExtractVOI::New();
      voi->SetInput(this->SampledReferenceImage);
      voi->SetRegion(p,OptimizationStructure.gridspacing,this->WindowSize);
      voi->Update();
      
      
      vtkpxImageExtractVOI* voi2=vtkpxImageExtractVOI::New();
      voi2->SetInput(this->LastTargetImage);
      voi2->SetRegion(p,OptimizationStructure.gridspacing,this->WindowSize);
      voi2->Update();
      
      
      timer->StopTimer();
      tvoi+=timer->GetElapsedTime();
      timer->StartTimer();
      
      for (int j=0;j<=2;j++)
	{
	  this->GridTransform->ShiftControlPoint(i,j,-OptimizationStructure.step);
	  double s1 = this->EvaluatePiece(voi->GetOutput(),
					  this->SampledTransformImage,
					  voi2->GetOutput(),
					  i);
	  this->GridTransform->ShiftControlPoint(i,j,+2.0*OptimizationStructure.step);
	  double s2 = this->EvaluatePiece(voi->GetOutput(),
					  this->SampledTransformImage,
					  voi2->GetOutput(),
					  i);
	  this->GridTransform->ShiftControlPoint(i,j,-OptimizationStructure.step);
	  
	  // Minus sign for ascent 
	  grad->SetComponent(i*3+j,0,-(s2-s1));
	  GradientNorm+=pow(s2-s1,2.0);
	  
	}
      timer->StopTimer();
      tpiece+=timer->GetElapsedTime();
      
      voi->Delete();
      voi2->Delete();
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
  fprintf(stderr,"Gradient Norm=%.5f scale=%.5f (tvoi=%f tpiece=%f)\n",GradientNorm,sc,tvoi,tpiece);
  return GradientNorm;

}
//-------------------------------------------------------------------------
double vtkpxNonLinearLabelRegistration::ComputeInternalEnergy(vtkpxGridTransform* tr,int cpindex)
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
double vtkpxNonLinearLabelRegistration::OptimizerGradientGlobal(vtkDoubleArray* params,vtkDoubleArray* grad)
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

  this->GenerateOutput(OptimizationStructure.level,
		       -1,-1,OptimizationStructure.step,
		       OptimizationStructure.old_similarity);
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

double vtkpxNonLinearLabelRegistration::ComputeCombinedValue(float* tv,int num)
{
  if (this->CombinationMetric==0)
    {
      double sum=0.0;
      for (int ia=0;ia<num;ia++)
	sum+=tv[ia];
      return sum/double(num);
    }
  else if (this->CombinationMetric==1)
    {
      return vtkpxMath::GetMedian(num,tv);
    }

  double minval=tv[0];
  for (int ia=1;ia<num;ia++)
    {
      if (minval>tv[ia])
	minval=tv[ia];
    }

  return minval;
}

