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




#include "pxutil.h"
#include "vtkImageData.h"
#include "vtkImageCast.h"
#include "vtkImageExtractComponents.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkbisImageResample.h"
#include "vtkbisImageReslice.h"
#include "vtkObjectFactory.h"
#include "vtkpxRegistration.h"
#include "vtkMath.h"
#include "vtkpxMath.h"
#include "vtkLandmarkTransform.h"
#include "vtkpxUtil.h"
#include "vtkTransform.h"
#include "vtkImageShiftScale.h"
#include "vtkPoints.h"
#include "vtkpxComboTransform.h"
#include "vtkpxAbstractIntensityRegistration.h"
#include "vtkbisImageHeader.h"
#include "vtkTimerLog.h"

vtkpxAbstractIntensityRegistration* vtkpxAbstractIntensityRegistration::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxAbstractIntensityRegistration");
  if(ret)
    {
      return (vtkpxAbstractIntensityRegistration*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxAbstractIntensityRegistration;
}


vtkpxAbstractIntensityRegistration::vtkpxAbstractIntensityRegistration()
{
  // Set parameters
  this->ReferenceImage=NULL;
  this->TransformImage=NULL;
  this->SampledReferenceImage=NULL;
  this->SampledTransformImage=NULL;

  this->ReferenceWeightImage=NULL;
  this->TransformWeightImage=NULL;
  this->SampledReferenceWeightImage=NULL;
  this->SampledTransformWeightImage=NULL;


  this->Histogram=NULL;
  this->NumberOfLevels=3;
  this->NumberOfExtraLevels=0;
  this->NumberOfBins=64;
  this->NumberOfSteps=4;
  this->Resolution=2.0;
  this->InternalResolution=2.0;
  this->InterpolationMode=1;
  this->StepSize=2.0;
  this->InternalStepSize=2.0;
  this->CurrentStepSize=2.0;
  this->CurrentResolution=2.0;
  this->ProvideFeedback=1;
  this->NumberOfIterations=25;
  this->SimilarityMeasure=5;
  strcpy(this->OutputStatus,"Nothing going on");
  this->LastProgress=0.0;
  this->ReferenceOrientation=0;
  this->TransformOrientation=0;
  this->LogarithmMode=0;
  this->PositiveIntensitiesOnly=0;
  this->BlurFactor=0.4247;
  this->OptimizationMethod=0;
  this->KeepSameNumberOfSlices=0;
  this->AutoNormalizeIntensities=1;
  this->JointlyNormalizeIntensities=0;
  for (int i=0;i<=2;i++)
    this->BaseResolution[i]=1.0;
  this->ResolutionRate=2.0;
  this->MidlineSymmetricMode=0;
}

vtkpxAbstractIntensityRegistration::~vtkpxAbstractIntensityRegistration()
{
  if (this->ReferenceImage!=NULL)
    this->ReferenceImage->Delete();
  if (this->TransformImage!=NULL)
    this->TransformImage->Delete();
  if (this->SampledReferenceImage!=NULL)
    this->SampledReferenceImage->Delete();
  if (this->SampledTransformImage!=NULL)
    this->SampledTransformImage->Delete();
  if (this->Histogram!=NULL)
    this->Histogram->Delete();


 if (this->ReferenceWeightImage!=NULL)
    this->ReferenceWeightImage->Delete();
  if (this->TransformWeightImage!=NULL)
    this->TransformWeightImage->Delete();
  if (this->SampledReferenceWeightImage!=NULL)
    this->SampledReferenceWeightImage->Delete();
  if (this->SampledTransformWeightImage!=NULL)
    this->SampledTransformWeightImage->Delete();
 
}


//----------------------------------------------------------------------------
void vtkpxAbstractIntensityRegistration::SetReferenceImage(vtkImageData* img,int frame)
{
  if (img==NULL)
    return;

  if (this->ReferenceImage==NULL)
    this->ReferenceImage=vtkImageData::New();

  vtkpxRegistration::ExtractComponentAndCastImageToShort(this->ReferenceImage,img,frame);
}
//----------------------------------------------------------------------------
void vtkpxAbstractIntensityRegistration::SetTransformImage(vtkImageData* img,int frame)
{
  if (img==NULL)
    return;

  if (this->TransformImage==NULL)
    this->TransformImage=vtkImageData::New();

  vtkpxRegistration::ExtractComponentAndCastImageToShort(this->TransformImage,img,frame);
}

//----------------------------------------------------------------------------
void vtkpxAbstractIntensityRegistration::PrintSelf(ostream& os, vtkIndent indent)
{
  os << indent << "### " << this->GetClassName() << " ### \n";
  if (this->GetTransformation()!=NULL)
    this->GetTransformation()->PrintSelf(os,indent.GetNextIndent());
  this->ReferenceImage->PrintSelf(os,indent.GetNextIndent());
  TransformImage->PrintSelf(os,indent.GetNextIndent());
}

//-------------------------------------------------------------------------
//                      Similarity Measure
//-------------------------------------------------------------------------
const char *vtkpxAbstractIntensityRegistration::GetOutputStatus()
{
  return this->OutputStatus;
}
//-------------------------------------------------------------------------
const char *vtkpxAbstractIntensityRegistration::NameOfMeasure()
{
  return vtkpxJointHistogram::NameOfMeasure(this->SimilarityMeasure);
}
//-------------------------------------------------------------------------
double vtkpxAbstractIntensityRegistration::Similarity(vtkpxJointHistogram* histo)
{
  //  fprintf(stdout,"\n\n\n In ABS Reg Similarity \n\n\n");

  if (histo==NULL)
    histo=this->Histogram;

  // Evaluate similarity from histogram
  if (histo==NULL)
    return -1.0;
  
  return histo->Similarity(this->SimilarityMeasure);
}

// ----------------------------------------------------------------------
// Run --- start procedure for all iterations
// ----------------------------------------------------------------------
void vtkpxAbstractIntensityRegistration::Run()
{
  if (this->TransformImage == NULL)
    {
      vtkErrorMacro(<< "::Run: Filter has no source input");
      return;
    }

  if (this->ReferenceImage == NULL)
    {
      vtkErrorMacro(<< "::Run: Filter has no target input");
      return;
    }
  
  vtkTimerLog* globtimer=vtkTimerLog::New();
  vtkTimerLog* timer=vtkTimerLog::New();
  globtimer->StartTimer();

  float optimizetime=0.0;

  vtkDebugMacro(<<"Initializing All");
  int ok=this->InitializeAll();
  if (ok==0)
    {
      vtkErrorMacro(<<"Initialization Failed\n");
      return;
    }

  for (int level=this->NumberOfLevels;level>=1;level=level-1)
    {
      vtkDebugMacro(<<"Initializing Level="<<level);
      fprintf(stderr,"\n\n\n--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--_--\n ++++++++++++++++ Beginning Level %d\n\n",level);
      this->Initialize(level);

      vtkDebugMacro(<<"Compute Registration Level="<<level);
      timer->StartTimer();
      this->ComputeRegistration(level);
      timer->StopTimer();
      optimizetime+=timer->GetElapsedTime();
      vtkDebugMacro(<<"Finalizing Level="<<level);
      this->Finalize(level);
    }

  //  fprintf(stderr,"On to Extra Levels=%d\n",this->NumberOfExtraLevels);
  for (int extra=1;extra<=this->NumberOfExtraLevels;extra++)
    {
      vtkDebugMacro(<<"Initializing Extra Level="<<extra);
      vtkDebugMacro(<<"\n\n Beginning Extra Level  " << extra << "/" << this->NumberOfExtraLevels << "\n");
      this->Initialize(1);
      vtkDebugMacro(<<"Compute Registration Extra Level="<<extra);
      this->ComputeRegistration(1);
      vtkDebugMacro(<<"Finalizing Extra Level="<<extra);
      this->Finalize(1);
    }

  vtkDebugMacro(<<"Finalizing All");
  this->FinalizeAll();
  globtimer->StopTimer();
  float globtime=globtimer->GetElapsedTime();
  timer->Delete();
  globtimer->Delete();
  fprintf(stderr,"\n\t\t\t Total Time = %.5fs compute time =%.5fs percentage = %.2f \%\n\n\n",
	  globtime,optimizetime,100.0*optimizetime/globtime);
  vtkDebugMacro(<<"vtkpxAbstractIntensityRegistration::Run() ends\n");

}
//-------------------------------------------------------------------------
int vtkpxAbstractIntensityRegistration::InitializeAll()
{
  if (this->TransformImage==NULL || this->ReferenceImage==NULL)
    {
      vtkErrorMacro(<<"Both Images Need to be non-NULL for registration to work");
      return 0;
    }

  if (this->SampledReferenceImage!=NULL)
    this->SampledReferenceImage->Delete();
  
  if (this->SampledTransformImage!=NULL)
    this->SampledTransformImage->Delete();

 if (this->SampledReferenceWeightImage!=NULL)
    this->SampledReferenceWeightImage->Delete();
  
  if (this->SampledTransformWeightImage!=NULL)
    this->SampledTransformWeightImage->Delete();


  this->SampledReferenceImage=vtkImageData::New();
  this->SampledTransformImage=vtkImageData::New();
  if (this->ReferenceWeightImage!=NULL)
    this->SampledReferenceWeightImage=vtkImageData::New();

  if (this->TransformWeightImage!=NULL)
    this->SampledTransformWeightImage=vtkImageData::New();

  this->NumberOfLevels=Irange(this->NumberOfLevels,1,8);
  this->NumberOfBins=Irange(this->NumberOfBins,2,1024);
  this->NumberOfIterations=Irange(this->NumberOfIterations,1,400);
  this->NumberOfSteps=Irange(this->NumberOfSteps,1,8);

  this->ReferenceImage->GetSpacing(this->BaseResolution);
  this->MinimumResolution=Fmin(this->BaseResolution[0],Fmin(this->BaseResolution[1],this->BaseResolution[2]));
  
  this->MeanResolution=0.0;
  for (int kk=0;kk<=2;kk++)
    {
      this->MeanResolution+=this->BaseResolution[kk]/3.0;

      // Make mostly isotropic data 
      if (this->BaseResolution[kk]>1.2*this->MinimumResolution)
	this->BaseResolution[kk]=1.2*this->MinimumResolution;
    }

  if (this->Resolution<0.0)
    this->InternalResolution=1.0;
  else
    this->InternalResolution=Frange(this->Resolution,0.25,50.0);


  double targ[3]; this->TransformImage->GetSpacing(targ);
  double tmin=Fmin(targ[0],Fmin(targ[1],targ[2]));
  
  double ratio=tmin/this->MinimumResolution;

  if (ratio>this->InternalResolution*2.0)
    {
      double a=this->InternalResolution;
      this->InternalResolution=ratio/2.0;
      this->CurrentResolution=this->InternalResolution;
      sprintf(this->OutputStatus,"\n\n Given Resolution Ratio of %.2f adjusting resolution from %.2f to %.2f\n",
	      ratio,a,this->InternalResolution);
      this->UpdateProgress(this->LastProgress);
    }



  this->CurrentResolution=this->InternalResolution;

  this->InterpolationMode=Irange(this->InterpolationMode,-5,3);
  if (this->InterpolationMode==2 || this->InterpolationMode==-1)
    this->InterpolationMode=1;

  this->InternalStepSize=Frange(this->StepSize,0.01,4.0);
  this->InternalStepSize*=this->MeanResolution;
  this->CurrentStepSize=this->InternalStepSize;
  //  fprintf(stdout,"MeanRes=%f, Step=%f\n",this->MeanResolution,this->CurrentStepSize);

  this->SimilarityMeasure=Irange(this->SimilarityMeasure,1,10);

  this->ReferenceOrientation=vtkbisImageHeader::MapOrientationToOldStyle(this->ReferenceOrientation);
  this->TransformOrientation=vtkbisImageHeader::MapOrientationToOldStyle(this->TransformOrientation);


  if (this->ReferenceOrientation!=this->TransformOrientation)
    {
      vtkpxRegistration::ReOrientImage(this->TransformImage,this->TransformImage,this->TransformOrientation,
				       this->ReferenceOrientation);

      if (this->TransformWeightImage!=NULL)
	vtkpxRegistration::ReOrientImage(this->TransformWeightImage,this->TransformWeightImage,this->TransformOrientation,
					 this->ReferenceOrientation);

      sprintf(this->OutputStatus,"Reorienting transform image image from %d to %d\n",
	      this->TransformOrientation,this->ReferenceOrientation);
      vtkDebugMacro(<<this->OutputStatus);
      this->UpdateProgress(this->LastProgress);
    }  

  sprintf(this->OutputStatus,"Beginning Registration SimilarityMetric = %s (%d)\n",
	  this->NameOfMeasure(),this->SimilarityMeasure);
  vtkDebugMacro(<<this->OutputStatus);
  //  fprintf(stderr,"%s\n",this->OutputStatus);
  this->UpdateProgress(this->LastProgress);
  return 1;
   
}
//-------------------------------------------------------------------------

int vtkpxAbstractIntensityRegistration::Initialize(int level)
{
  double reduction=pow(this->ResolutionRate,double(level-1.0));
  this->CurrentStepSize=this->InternalStepSize*reduction;
  this->CurrentResolution=this->InternalResolution*reduction;
  this->InternalBlurFactor=this->BlurFactor;
  if (this->CurrentResolution==1.0)
    this->InternalBlurFactor=0.0;

  if (this->SampledReferenceImage==NULL)
    this->SampledReferenceImage=vtkImageData::New();
  if (this->SampledTransformImage==NULL)
    this->SampledTransformImage=vtkImageData::New();

  // This forces the same slices regardless of level 

  //  vtkpxRegistration::PrintImageInfo(this->SampledReferenceImage,"Before");


  vtkpxRegistration::ResampleAndSmoothImage(this->SampledReferenceImage,this->ReferenceImage,
					    this->BaseResolution,
					    this->InternalBlurFactor*this->CurrentResolution,
					    this->CurrentResolution,this->KeepSameNumberOfSlices);

   
  //  vtkpxRegistration::PrintImageInfo(this->SampledReferenceImage,"ResandSmooth");

  vtkpxRegistration::ResampleAndSmoothImage(this->SampledTransformImage,this->TransformImage,
					    this->BaseResolution,
					    this->InternalBlurFactor*this->CurrentResolution,
					    this->CurrentResolution,this->KeepSameNumberOfSlices);

  short minr=0,maxr=1,mint=0,maxt=1;
  int num_ref,num_tar;

  sprintf(this->OutputStatus,"Going to Calc Range and Adjust \n\n");
  this->UpdateProgress(this->LastProgress);

  if (this->JointlyNormalizeIntensities)
    {
      num_ref=vtkpxRegistration::CalculateNumberOfBinsAndAdjustImages(this->SampledReferenceImage,
								      this->SampledTransformImage,
								      this->NumberOfBins,minr,maxr,
								      this->LogarithmMode,
								      this->PositiveIntensitiesOnly,
								      this->AutoNormalizeIntensities);
      num_tar=num_ref;
      mint=minr;
      maxt=maxr;
    }
  else
    {
      num_ref=vtkpxRegistration::CalculateNumberOfBinsAndAdjustImage(this->SampledReferenceImage,
								     this->NumberOfBins,minr,maxr,
								     this->LogarithmMode,
								     this->PositiveIntensitiesOnly,
								     this->AutoNormalizeIntensities);
      num_tar=vtkpxRegistration::CalculateNumberOfBinsAndAdjustImage(this->SampledTransformImage,
								     this->NumberOfBins,mint,maxt,
								     this->LogarithmMode,
								     this->PositiveIntensitiesOnly,
								     this->AutoNormalizeIntensities);
    }


  sprintf(this->OutputStatus,"Resample and Smooth Images \n\n");
  this->UpdateProgress(this->LastProgress);


  if (this->ReferenceWeightImage!=NULL)
    {
      vtkpxRegistration::ResampleAndSmoothImage(this->SampledReferenceWeightImage,this->ReferenceWeightImage,
						this->BaseResolution,
						this->InternalBlurFactor*this->CurrentResolution,
						this->CurrentResolution,this->KeepSameNumberOfSlices);


      

      vtkpxRegistration::ScaleWeightImage(this->SampledReferenceWeightImage,this->SampledReferenceImage,100.0);

      //      vtkpxUtil::SaveAnalyze("weight_image_raw.nii.gz",this->ReferenceWeightImage,9);
      //      vtkpxUtil::SaveAnalyze("ref_image.nii.gz",this->SampledReferenceImage,9);
      //      vtkpxUtil::SaveAnalyze("weight_image_scaled.nii.gz",this->SampledReferenceWeightImage,9);
      //      exit(0);
    }

  if (this->TransformWeightImage!=NULL)
    {
      vtkpxRegistration::ResampleAndSmoothImage(this->SampledTransformWeightImage,this->TransformWeightImage,
						this->BaseResolution,
						this->InternalBlurFactor*this->CurrentResolution,
						this->CurrentResolution,this->KeepSameNumberOfSlices);
      vtkpxRegistration::ScaleWeightImage(this->SampledTransformWeightImage,this->SampledTransformImage,100.0);
    }


  //  vtkpxRegistration::PrintImageInfo(this->SampledReferenceImage,"postNorm");

  if (this->Histogram!=NULL)
    this->Histogram->Delete();
  this->Histogram=vtkpxJointHistogram::New();
  if (!this->MidlineSymmetricMode)
    this->Histogram->Allocate(num_ref,num_tar);
  else
    this->Histogram->Allocate(num_tar,num_tar);

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
	  "%s\n Reference = %d x %d x %d (%.2f %.2f %.2f) (or=%.1f,%.1f,%.1f), Range=%d:%d bins=%d kern=%5.2f",
	  this->OutputStatus,dim1[0],dim1[1],dim1[2],sp1[0],sp1[1],sp1[2],or1[0],or1[1],or1[2],
	  minr,maxr,num_ref,this->InternalBlurFactor*this->CurrentResolution);
  sprintf(this->OutputStatus,
	  "%s\nTransform  = %d x %d x %d (%.2f %.2f %.2f) (or=%.1f,%.1f,%.1f), Range=%d:%d bins=%d kern=%5.2f midline=%d\n",
	  this->OutputStatus,dim2[0],dim2[1],dim2[2],sp2[0],sp2[1],sp2[2],or2[0],or2[1],or2[2],
	  mint,maxt,num_tar,this->InternalBlurFactor*this->CurrentResolution,this->MidlineSymmetricMode);


  if (this->SampledReferenceWeightImage!=NULL)
    {
      this->SampledReferenceWeightImage->GetSpacing(sp2);
      this->SampledReferenceWeightImage->GetDimensions(dim2);
      this->SampledReferenceWeightImage->GetOrigin(or2);
      double range[2]; this->SampledReferenceWeightImage->GetPointData()->GetScalars()->GetRange(range);

      sprintf(this->OutputStatus,
	      "%s Ref Weight  = %d x %d x %d (%.2f %.2f %.2f) (or=%.1f,%.1f,%.1f) range=%.2f:%.2f\n",
	      this->OutputStatus,dim2[0],dim2[1],dim2[2],sp2[0],sp2[1],sp2[2],or2[0],or2[1],or2[2],range[0],range[1]);
    }
  else
    { 
      sprintf(this->OutputStatus,"%s Ref Weight Image not set\n",this->OutputStatus);
    }

  if (this->SampledTransformWeightImage!=NULL)
    {
      this->SampledTransformWeightImage->GetSpacing(sp2);
      this->SampledTransformWeightImage->GetDimensions(dim2);
      this->SampledTransformWeightImage->GetOrigin(or2);
      double range[2]; this->SampledTransformWeightImage->GetPointData()->GetScalars()->GetRange(range);
      sprintf(this->OutputStatus,
	      "%s Transform Weight  = %d x %d x %d (%.2f %.2f %.2f) (or=%.1f,%.1f,%.1f) range=%.2f:%.2f\n\n",
	      this->OutputStatus,dim2[0],dim2[1],dim2[2],sp2[0],sp2[1],sp2[2],or2[0],or2[1],or2[2],range[0],range[1]);
    }
  else
    {
      sprintf(this->OutputStatus,"%s Transform Weight Image not set\n\n",this->OutputStatus);
    }

  vtkDebugMacro(<<this->OutputStatus);
  //fprintf(stderr,"%s\n",this->OutputStatus);
  this->UpdateProgress(this->LastProgress);
  return 1;

}
//-------------------------------------------------------------------------
int vtkpxAbstractIntensityRegistration::Finalize(int level)
{
 
//  this->SampledReferenceImage->Initialize();
//  this->SampledTransformImage->Initialize();

  sprintf(this->OutputStatus,"\n\n **** Done with level %d\n ------------------------------------------------------------ \n\n",level);
  vtkDebugMacro(<<this->OutputStatus);
  this->UpdateProgress(this->LastProgress);
  //  sprintf(this->OutputStatus,"****Finalize level %d done",level);
  //  vtkDebugMacro(<<this->OutputStatus);
  return 1;

}
//-------------------------------------------------------------------------
int vtkpxAbstractIntensityRegistration::FinalizeAll()
{
  //  printf("int vtkpxAbstractIntensityRegistration::FinalizeAll() starts..\n");
  /*this->SampledReferenceImage->Delete();
    this->SampledTransformImage->Delete();
    this->SampledReferenceImage=NULL;
    this->SampledTransformImage=NULL;*/
  return 1;

}
//-------------------------------------------------------------------------
double vtkpxAbstractIntensityRegistration::Evaluate()
{
  return vtkpxRegistration::HistogramEvaluate(this->SampledReferenceImage,this->SampledTransformImage,
					      this->GetTransformation(),
					      this->Histogram,this->SimilarityMeasure,
					      this->InterpolationMode);
}
//-------------------------------------------------------------------------
double vtkpxAbstractIntensityRegistration::WeightedEvaluate()
{
  if (this->SampledReferenceWeightImage==NULL)
    return this->Evaluate();

  return vtkpxRegistration::WeightedHistogramEvaluate(this->SampledReferenceImage,
						      this->SampledReferenceWeightImage,
						      this->SampledTransformImage,
						      this->SampledTransformWeightImage,
						      this->GetTransformation(),
						      this->Histogram,this->SimilarityMeasure,
						      this->InterpolationMode);
}

//-------------------------------------------------------------------------
void vtkpxAbstractIntensityRegistration::GenerateOutput(int level, int step,int iter,double stepsize,double actualSimilarity)
{
  double sc=1.0/double(this->NumberOfLevels);
  double pog=(double(step)/double(this->NumberOfSteps+1) + double(this->NumberOfLevels-level))*sc;
  this->LastProgress=pog;
  this->UpdateProgress(pog);
  //  vtkDebugMacro(<<this->OutputStatus);
  return;
}
//-------------------------------------------------------------------------
// Optimizer SsFinalizetuff
//-------------------------------------------------------------------------
void vtkpxAbstractIntensityRegistration::SetOutputStatus(const char* line,double v)
{
  sprintf(this->OutputStatus,"**Opt=%s (%.5f)\n",line,v);
  this->UpdateProgress(this->LastProgress);
}

//-------------------------------------------------------------------------
double vtkpxAbstractIntensityRegistration::CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output)
{
  return ((vtkpxAbstractIntensityRegistration*)self)->OptimizerGradient(position,output);
}

double vtkpxAbstractIntensityRegistration::CalculateFunction(vtkObject* self,vtkDoubleArray* position)
{
  return ((vtkpxAbstractIntensityRegistration*)self)->OptimizerValue(position);
}

double vtkpxAbstractIntensityRegistration::OnBeginIteration(vtkObject* self,vtkDoubleArray* position)
{
  //  fprintf(stderr,"On Begin Iteration Abstract\n");
  return ((vtkpxAbstractIntensityRegistration*)self)->OnBeginIteration(position);
}


void vtkpxAbstractIntensityRegistration::OptimizerFeedback(vtkObject* self,const char* line,double val)
{
  ((vtkpxAbstractIntensityRegistration*)self)->SetOutputStatus(line,val);
}
//-------------------------------------------------------------------------


