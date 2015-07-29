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
  Date:      $Date: 2004/01/21 22:35:22 $
  Version:   $Revision: 1.2 $


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
#include "vtkpxAbstractBiasFieldCorrection.h"
#include "vtkObjectFactory.h"
#include "pxutil.h"
#include "vtkpxUtil.h"
#include "vtkExtractVOI.h"
#include "vtkpxOptimizer.h"
#include "vtkbisImageResample.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageGradientMagnitude.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkpxMath.h"
#include "vtkImageCast.h"
#include "vtkpxHistogramSegmentation.h"
#include "vtkpxImageRobustHistogram.h"
#include "vtkImageExtractComponents.h"

vtkpxAbstractBiasFieldCorrection* vtkpxAbstractBiasFieldCorrection::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxAbstractBiasFieldCorrection");
  if(ret)
    {
    return (vtkpxAbstractBiasFieldCorrection*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxAbstractBiasFieldCorrection;
  
}


vtkpxAbstractBiasFieldCorrection::vtkpxAbstractBiasFieldCorrection()
{

  this->StepSize=2.0;
  this->InternalStepSize=2.0;
  this->CurrentStepSize=2.0;
  this->CurrentResolution=2.0;
  this->LastProgress=0;

  this->ProvideFeedback=1;
  this->NumberOfIterations=25;

  this->Epsilon=0.003;
  this->NumberOfLevels=3;
  this->NumberOfSteps=1;
  this->Resolution=1.0;

  this->ReferenceImage=NULL;
  this->SampledReferenceImage=NULL;
  this->WeightImage=NULL;

  this->BiasField=NULL;
  this->SampledWeightImage=NULL;
  this->ClassParameters=NULL;
  this->Lambda=0.0;
  this->FixedNumberOfSlices=0;

  this->Histo_NumberOfClasses=3;
  this->Histo_Iterations=30;
  this->Histo_Convergence=0.5;
  this->Histo_NumberOfBins=128;
  this->Histo_Sigma=1.0;
  this->Histo_OutlierDetection=0;
  this->Histo_OutlierProbability=0.0005;
  this->Histo_PreprocessingMode=2;
  this->Histo_MaxSigmaRatio=0.05;

  this->MaskImage=NULL;
  this->Metric=0;

  this->Frame=0;

  strcpy(this->OutputStatus,"");
}

vtkpxAbstractBiasFieldCorrection::~vtkpxAbstractBiasFieldCorrection() 
{


  if (this->SampledReferenceImage!=NULL)
    this->SampledReferenceImage->Delete();

  if (this->ReferenceImage!=NULL)
    this->ReferenceImage->Delete();
  
  if (this->SampledWeightImage!=NULL)
    this->SampledWeightImage->Delete();

  if (this->WeightImage!=NULL)
    this->WeightImage->Delete();

  if (this->BiasField!=NULL)
    this->BiasField->Delete();




  this->SetMaskImage(NULL);
}

const char *vtkpxAbstractBiasFieldCorrection::GetOutputStatus()
{
  return this->OutputStatus;
}

//-------------------------------------------------------------------------
int vtkpxAbstractBiasFieldCorrection::InitializeAll() 
{
  this->NumberOfLevels=Irange(this->NumberOfLevels,1,8);
  this->NumberOfIterations=Irange(this->NumberOfIterations,1,100);
  this->NumberOfSteps=Irange(this->NumberOfSteps,1,8);

  double sp[3]; this->ReferenceImage->GetSpacing(sp);
  this->MinimumResolution=sp[0];
  
  if (this->Resolution<0.0)
    this->InternalResolution=1.0;
  else
    this->InternalResolution=Frange(this->Resolution,0.25,50.0);

  this->CurrentResolution=this->InternalResolution;

  this->InternalStepSize=Frange(this->StepSize,0.01,40.0);
  this->CurrentStepSize=this->InternalStepSize;
  this->CreateWeightImage(this->ReferenceImage);


  return 1;
}
//-------------------------------------------------------------------------
int vtkpxAbstractBiasFieldCorrection::CreateWeightImage(vtkImageData* ref) 
{
  if (this->WeightImage!=NULL)
    this->WeightImage->Delete();

  this->WeightImage=vtkImageData::New();
  this->WeightImage->CopyStructure(ref);
  this->WeightImage->SetScalarTypeToFloat();
  this->WeightImage->SetNumberOfScalarComponents(1);
  this->WeightImage->AllocateScalars();

  vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
  smooth->SetInput(ref);
  smooth->SetStandardDeviations(1.0,1.0,1.0);

  vtkImageGradientMagnitude* grad=vtkImageGradientMagnitude::New();
  grad->HandleBoundariesOn();
  grad->SetInput(smooth->GetOutput());
  grad->Update();

  vtkImageData* tmp=grad->GetOutput();

  vtkDataArray* magn=tmp->GetPointData()->GetScalars();
  double r[2]; magn->GetRange(r);
  double sigma2=pow(r[1]*0.1,2.0);


  vtkDataArray* in=ref->GetPointData()->GetScalars();
  vtkDataArray* wgt=this->WeightImage->GetPointData()->GetScalars();
  int nt=wgt->GetNumberOfTuples();

  if (this->MaskImage==NULL)
    {
      double range[2]; in->GetRange(range);
      for (int i=0;i<nt;i++)
	{
	  float m=in->GetComponent(i,0);
	  if (m>range[0])
	    wgt->SetComponent(i,0,1000*(1.0-vtkpxAbstractBiasFieldCorrection::Valley2(magn->GetComponent(i,0),sigma2)));
	  else
	    wgt->SetComponent(i,0,0.0);
	}
    }
  else
    {
      vtkDataArray* msk=this->MaskImage->GetPointData()->GetScalars();
      double mrange[2]; msk->GetRange(mrange);
      double range[2];  in->GetRange(range);

      for (int i=0;i<nt;i++)
	{
	  float m=msk->GetComponent(i,0);
	  float w=in->GetComponent(i,0);
	  if (m>mrange[0] && w>range[0])
	    wgt->SetComponent(i,0,1000*(1.0-vtkpxAbstractBiasFieldCorrection::Valley2(magn->GetComponent(i,0),sigma2)));
	  else
	    wgt->SetComponent(i,0,0.0);
	}
    }
  grad->Delete();
  smooth->Delete();
    //  vtkpxUtil::SaveAnalyze("ap_weight",this->WeightImage,0);
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxAbstractBiasFieldCorrection::InitializeTransformation(int level) 
{
  fprintf(stderr,"In vtkpxAbstractBiasFieldCorrection::InitializeTransform\n");
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxAbstractBiasFieldCorrection::Initialize(int level) 
{
  double reduction=pow(2.0,double(level-1.0));
  this->CurrentStepSize=this->InternalStepSize*reduction;
  this->CurrentResolution=this->InternalResolution*reduction;


  double sp[3];  this->ReferenceImage->GetSpacing(sp);
  int    dim[3]; this->ReferenceImage->GetDimensions(dim);

  /*  if (sp[2]>(sp[1]+sp[0]))
      this->FixedNumberOfSlices=1;*/

  fprintf(stderr,"Input sp = %.1f,%.1f,%.1f dim=%d,%d,%d fixed_slices=%d\n",sp[0],sp[1],sp[2],dim[0],dim[1],dim[2],
	  this->FixedNumberOfSlices);



  vtkbisImageResample* resamp=vtkbisImageResample::New();
  resamp->SetInput(this->ReferenceImage);

  double outspa[3];

  for (int ia=0;ia<=2;ia++)
    outspa[ia]=sp[ia]*this->CurrentResolution;

  double minspa=outspa[0];
  for (int ia=1;ia<=2;ia++)
    {
      if (outspa[ia]<minspa)
	minspa=outspa[ia];
    }

  for (int ia=0;ia<=2;ia++)
    {
      if (outspa[ia]>1.2*minspa)
	outspa[ia]=1.2*minspa;
    }

  if (this->FixedNumberOfSlices>0)
    outspa[2]=sp[2];

  for (int ia=0;ia<=2;ia++)
    resamp->SetAxisOutputSpacing(ia,outspa[ia]);
  // Check this!
  resamp->InterpolateOn();
  resamp->DebugOff();
  resamp->Update();

  
  if (this->SampledReferenceImage!=NULL)
    this->SampledReferenceImage->Delete();
  this->SampledReferenceImage=vtkImageData::New();
  this->SampledReferenceImage->DeepCopy(resamp->GetOutput());

  resamp->SetInput(this->WeightImage);
  resamp->Modified();
  resamp->Update();


  if (this->SampledWeightImage!=NULL)
    this->SampledWeightImage->Delete();
  this->SampledWeightImage=vtkImageData::New();
  
  this->SampledWeightImage->ShallowCopy(resamp->GetOutput());
  resamp->Delete();

  double sp1[3],or1[3],range[2];
  int   dim1[3];

  this->SampledReferenceImage->GetSpacing(sp1);
  this->SampledReferenceImage->GetDimensions(dim1);
  this->SampledReferenceImage->GetOrigin(or1);
  this->SampledReferenceImage->GetPointData()->GetScalars()->GetRange(range);

  sprintf(this->OutputStatus,"\nEnd Of Initialization Of Level =%d,%d",level,this->NumberOfLevels);
  sprintf(this->OutputStatus,
	  "%s\n Reference = %d x %d x %d (%.2f %.2f %.2f) (or=%.1f,%.1f,%.1f) range=%.1f:%.1f\n",
	  this->OutputStatus,dim1[0],dim1[1],dim1[2],sp1[0],sp1[1],sp1[2],or1[0],or1[1],or1[2],range[0],range[1]);

  vtkDebugMacro(<<this->OutputStatus);
  fprintf(stderr,"%s\n",this->OutputStatus);
  this->UpdateProgress(this->LastProgress);
  this->InitializeTransformation(level);
  return 1;

}
//-------------------------------------------------------------------------
int vtkpxAbstractBiasFieldCorrection::FinalizeAll()
{
  return 1;
}
//-------------------------------------------------------------------------
// Compute Registration
int vtkpxAbstractBiasFieldCorrection::ComputeBiasFit(int level)
{  
  return 1;
}
//-------------------------------------------------------------------------

double vtkpxAbstractBiasFieldCorrection::Optimize(vtkDoubleArray* grad,double old_similarity,
						   double step,int level,
						   double origin[3],double spacing[3],
						   int dimensions[3],double gridspacing[3])
{
  return 0.0;
}
//-------------------------------------------------------------------------
vtkImageData* vtkpxAbstractBiasFieldCorrection::ComputeBiasFieldImage(vtkImageData* ref)
{
  return NULL;
}
//-------------------------------------------------------------------------
vtkImageData* vtkpxAbstractBiasFieldCorrection::ComputeMeanPreservingCorrectedImage(vtkImageData* ref,vtkImageData* biasfield)
{
  return vtkpxAbstractBiasFieldCorrection::ComputeCorrectedImage(ref,biasfield,1);
}

vtkImageData* vtkpxAbstractBiasFieldCorrection::ComputeCorrectedImage(vtkImageData* ref,vtkImageData* biasfield)
{
  return vtkpxAbstractBiasFieldCorrection::ComputeCorrectedImage(ref,biasfield,0);
}
//-------------------------------------------------------------------------
double  vtkpxAbstractBiasFieldCorrection::ComputeScaleFactor(vtkImageData* ref,vtkImageData* biasfield)
{
  if (ref==NULL || biasfield==NULL)
    {
      fprintf(stderr,"NULL Inputs to ComputeScaleFactor\n");
      return 0.0;
    }

  double typeMax =  ref->GetScalarTypeMax();
  double range[2];  ref->GetPointData()->GetScalars()->GetRange(range);
  double brange[2]; biasfield->GetPointData()->GetScalars()->GetRange(brange);


  // Find Max Intensity
  double maximg=range[1];
  if (fabs(range[0])>range[1])
    maximg=fabs(range[0]);

  double minb=brange[0];
  if (minb<0.0001)
    minb=0.0001;

  double scale_in=1.0;
  double maxinten=maximg/minb;
  if (maxinten > typeMax)
    {
      scale_in= (typeMax)/(maxinten);
      fprintf(stderr,"Intensity Scaling = %.2f   (maximg=%.1f,minb=%.2f,maxinten=%.1f,typeMax=%.2f\n",
	      scale_in,maximg,minb,maxinten,typeMax);
    }
  return scale_in;


}
//-------------------------------------------------------------------------
vtkImageData* vtkpxAbstractBiasFieldCorrection::ComputeCorrectedImage(vtkImageData* ref,vtkImageData* biasfield,int meanpreserving)
{
  vtkImageData* output=vtkImageData::New();
  output->CopyStructure(ref);
  output->SetNumberOfScalarComponents(ref->GetNumberOfScalarComponents());
  output->AllocateScalars();

  int ok=vtkpxAbstractBiasFieldCorrection::ComputeCorrectedImage(ref,biasfield,output,meanpreserving);
  if (ok==0)
    {
      fprintf(stderr,"Correction Failed\n");
      output->Delete();
      output=NULL;
    }
  else
    {
      fprintf(stderr,"Correction Succeeded\n");
    }

  return output;
}
// -------------------------------------------------------------------------
int vtkpxAbstractBiasFieldCorrection::ComputeCorrectedImage(vtkImageData* ref,vtkImageData* biasfield,vtkImageData* output,int meanpreserving)
{
  if (ref==NULL || biasfield==NULL || output==NULL)
    {
      fprintf(stderr,"NULL Inputs to ComputeCorrectedImage\n");
      return 0;
    }

  
  vtkDataArray* img=ref->GetPointData()->GetScalars();
  vtkDataArray* bf =biasfield->GetPointData()->GetScalars();
  vtkDataArray* out =output->GetPointData()->GetScalars();

  int nt=img->GetNumberOfTuples();
  int nc=img->GetNumberOfComponents();

  if (nt!=bf->GetNumberOfTuples())
    {
      fprintf(stderr,"Mismatched ref image and bias field inputs to ComputeCorrectedImage\n");
      return 0;
    }

  if (nt!=out->GetNumberOfTuples() || nc!=out->GetNumberOfComponents())
    {
      output->CopyStructure(ref);
      output->SetNumberOfScalarComponents(ref->GetNumberOfScalarComponents());
      output->AllocateScalars();
      fprintf(stderr,"Compute Corrected, fixing output ******************************\n");
      out =output->GetPointData()->GetScalars();
    }
   


  double sum_in=0.00001;
  double sum_out=0.00001;
  double scale_in=vtkpxAbstractBiasFieldCorrection::ComputeScaleFactor(ref,biasfield);

  for (int i=0;i<nt;i++)
    {
      double b=bf->GetComponent(i,0);
      for (int c=0;c<nc;c++)
	{
	  double v=img->GetComponent(i,c)*scale_in;
	  sum_in+=v;
	  
	  if (b>0.000)
	    v=v/b;
	  sum_out+=v;
	  out->SetComponent(i,c,v);
	}
    }

  if (meanpreserving>0)
    {
      double scale=sum_in/sum_out;
      //      fprintf(stderr,"Mean Preserving Scale =%f (sum_in=%f sum_out=%f)\n",scale,sum_in,sum_out);
      for (int i=0;i<nt;i++)
	for (int c=0;c<nc;c++)
	  out->SetComponent(i,c,out->GetComponent(i,c)*scale);
    }

  return 1;
  
  
}
// --------------------------------------------------------------------------------------------------------------------------------------------
vtkImageData* vtkpxAbstractBiasFieldCorrection::ComputeMaskCorrectedImage(vtkImageData* ref,vtkImageData* biasfield,vtkImageData* mask,
									  int meanpreserving)
{
  if (ref==NULL || biasfield==NULL || mask==NULL)
    {
      fprintf(stderr,"NULL Inputs to ComputeCorrectedImage\n");
      return NULL;
    }

  vtkDataArray* img=ref->GetPointData()->GetScalars();
  vtkDataArray* bf =biasfield->GetPointData()->GetScalars();
  vtkDataArray* msk =mask->GetPointData()->GetScalars();

  int nt=img->GetNumberOfTuples();
  int nc=img->GetNumberOfComponents();

  if (nt!=bf->GetNumberOfTuples() || nt!=msk->GetNumberOfTuples())
    {
      fprintf(stderr,"Mismatched ref image and bias field/mask inputs to ComputeMaskCorrectedImage\n");
      return NULL;
    }

  vtkImageData* output=vtkImageData::New();
  output->CopyStructure(ref);
  //  output->SetScalarTypeToFloat();
  output->AllocateScalars();
  
  vtkDataArray* out =output->GetPointData()->GetScalars();

  double sum_in=0.00001;
  double sum_out=0.00001;
  double scale_in=vtkpxAbstractBiasFieldCorrection::ComputeScaleFactor(ref,biasfield);

  for (int i=0;i<nt;i++)
    {
      double m=msk->GetComponent(i,0);
      if (m>0)
	{
	  double b=bf->GetComponent(i,0);
	  for (int c=0;c<nc;c++)
	    {
	      double v=img->GetComponent(i,c)*scale_in;
	      sum_in+=v;
	      
	      if (b>0.000)
		v=v/b;
	      sum_out+=v;
	      
	      out->SetComponent(i,c,v);
	    }
	}
      else
	{
	  for (int c=0;c<nc;c++)
	    {
	      double v=img->GetComponent(i,c);
	      sum_in+=v;
	      sum_out+=v;
	      out->SetComponent(i,c,v);
	    }
	}
    }

  if (meanpreserving>0)
    {
      double scale=sum_in/sum_out;
      //      fprintf(stderr,"Mean Preserving Scale =%f (sum_in=%f sum_out=%f)\n",scale,sum_in,sum_out);
      for (int i=0;i<nt;i++)
	{
	  double m=msk->GetComponent(i,0);
	  if (m>0)
	    {
	      for (int c=0;c<nc;c++)
		out->SetComponent(i,c,out->GetComponent(i,c)*scale);
	    }
	}
    }

  return output;
  
  
}
//-------------------------------------------------------------------------
// Log Image
//-------------------------------------------------------------------------
vtkImageData* vtkpxAbstractBiasFieldCorrection::ComputeMeanPreservingCorrectedLogarithmImage(vtkImageData* logref,vtkImageData* logbiasfield)
{
  return vtkpxAbstractBiasFieldCorrection::ComputeCorrectedLogarithmImage(logref,logbiasfield,1);
}

vtkImageData* vtkpxAbstractBiasFieldCorrection::ComputeCorrectedLogarithmImage(vtkImageData* logref,vtkImageData* logbiasfield)
{
  return vtkpxAbstractBiasFieldCorrection::ComputeCorrectedLogarithmImage(logref,logbiasfield,0);
}

vtkImageData* vtkpxAbstractBiasFieldCorrection::ComputeCorrectedLogarithmImage(vtkImageData* logref,vtkImageData* logbiasfield,int meanpreserving)
{
  if (logref==NULL || logbiasfield==NULL)
    {
      fprintf(stderr,"NULL Inputs to ComputeCorrectedLogImage\n");
      return NULL;
    }

  vtkDataArray* img=logref->GetPointData()->GetScalars();
  vtkDataArray* bf =logbiasfield->GetPointData()->GetScalars();

  int nt=img->GetNumberOfTuples();
  int nc=img->GetNumberOfComponents();

  if (nt!=bf->GetNumberOfTuples())
    {
      fprintf(stderr,"Mismatched logref image and bias field inputs to ComputeCorrectedImage\n");
      return NULL;
    }

  vtkImageData* output=vtkImageData::New();
  output->CopyStructure(logref);
  //  output->SetScalarTypeToFloat();
  output->AllocateScalars();
  
  vtkDataArray* out =output->GetPointData()->GetScalars();

  double sum_in=0.00001;
  double sum_out=0.00001;
  for (int i=0;i<nt;i++)
    {
      double b=bf->GetComponent(i,0);
      for (int c=0;c<nc;c++)
	{
	  double v=img->GetComponent(i,c);
	  sum_in+=v;
	  
	  v=v-b;
	  sum_out+=v;
	  out->SetComponent(i,c,v);
	}
    }

  if (meanpreserving>0)
    {
      double scale=sum_in/sum_out;
      //      fprintf(stderr,"Mean Preserving Scale =%f (sum_in=%f sum_out=%f)\n",scale,sum_in,sum_out);
      for (int i=0;i<nt;i++)
	for (int c=0;c<nc;c++)
	  out->SetComponent(i,c,out->GetComponent(i,c)*scale);
    }

  return output;
  
  
}
//-------------------------------------------------------------------------
int vtkpxAbstractBiasFieldCorrection::ExponentiateBiasField(vtkImageData* logbiasfield)
{
  if (logbiasfield==NULL)
    {
      fprintf(stderr,"NULL Inputs to ExponentiateBiasField\n");
      return 0;
    }

  vtkDataArray* bf =logbiasfield->GetPointData()->GetScalars();
  int nt=bf->GetNumberOfTuples();
  for (int i=0;i<nt;i++)
    bf->SetComponent(i,0,exp(bf->GetComponent(i,0)));
  
  return 1;
}
//-------------------------------------------------------------------------
vtkFloatArray* vtkpxAbstractBiasFieldCorrection::ComputeLogClassParameters(vtkFloatArray* initial)
{
  if (initial==NULL)
    return NULL;


  vtkFloatArray* initiallog=vtkFloatArray::New();
  initiallog->DeepCopy(initial);
  int nt=initiallog->GetNumberOfTuples();
  for (int i=0;i<nt;i++)
    {
      double m=initiallog->GetComponent(i,0);
      double s=initiallog->GetComponent(i,1);
      
      fprintf(stderr," Class %d %.2f + %.2f ---> ",i+1,m,s);

      if (m>1.0)
	m=log(m);
      else
	m=0.0;
      
      /*      if (s>1.0)
	s=log(s);
	else*/
	s=1.0;
      
      initiallog->SetComponent(i,0,m);
      initiallog->SetComponent(i,1,s);

      fprintf(stderr,"  %.2f + %.2f \n",m,s);
    }
  return initiallog;
}
//-------------------------------------------------------------------------
vtkFloatArray* vtkpxAbstractBiasFieldCorrection::ComputeClassParameters(vtkImageData* input,vtkFloatArray *initial)
{
  if (input==NULL)
    return NULL;

  vtkpxHistogramSegmentation* histo=vtkpxHistogramSegmentation::New();
  histo->SetInput(input);
  histo->SetInitialParameters(initial);
  histo->SetNumberOfClasses(this->Histo_NumberOfClasses);
  histo->SetIterations(this->Histo_Iterations);
  histo->SetSigma(this->Histo_Sigma);
  histo->SetConvergence(this->Histo_Convergence);
  histo->SetNumberOfBins(this->Histo_NumberOfBins);
  histo->SetOutlierProbability(this->Histo_OutlierProbability);
  histo->SetOutlierDetection(this->Histo_OutlierDetection);
  histo->SetPreprocessingMode(this->Histo_PreprocessingMode);
  //  histo->SetMaskImage(this->WeightImage);
  histo->GenerateLabelsOff();
  histo->SetMaxSigmaRatio(this->Histo_MaxSigmaRatio);
  histo->Update();

  vtkFloatArray* p=vtkFloatArray::New();
  p->DeepCopy(histo->GetParameters());
  
  histo->Delete();
  return p;
}

int vtkpxAbstractBiasFieldCorrection::ComputeAndSetClassParameters(vtkImageData* input,vtkFloatArray* initial)
{
  vtkFloatArray* p=this->ComputeClassParameters(input,initial);
  if (p==NULL)
    return 0;

  if (this->Histo_OutlierDetection==0)
    {
      this->SetClassParameters(p);
    }
  else
    {
      vtkFloatArray* phat=vtkFloatArray::New();
      int nt=p->GetNumberOfTuples();
      phat->SetNumberOfComponents(p->GetNumberOfComponents());
      phat->SetNumberOfTuples(nt-1);

      for (int i=0;i<nt-1;i++)
	for (int j=0;j<p->GetNumberOfComponents();j++)
	  phat->SetComponent(i,j,p->GetComponent(i,j));

      this->SetClassParameters(phat);
      phat->Delete();
    }
  p->Delete();
  return 1;
}
//-------------------------------------------------------------------------
double vtkpxAbstractBiasFieldCorrection::Evaluate()
{
  this->LastSimilarity=0.0;
  this->LastSmoothness=0.0;
  return 0.0;
}

//-------------------------------------------------------------------------
void vtkpxAbstractBiasFieldCorrection::PrintInformation() 
{
  
}
//-------------------------------------------------------------------------

void vtkpxAbstractBiasFieldCorrection::GenerateOutput(int level, int step,int iter,double stepsize,double actualSimilarity)
{
  if (iter>=100)
    iter=99;

  sprintf(this->OutputStatus,"Level=%2d Step=%2d iter=%2d stepsize=%.3f: tot=%8.6f (sim=%8.6f sm=%8.6f (dist=%.2f))",
	  level,step,iter,stepsize,actualSimilarity,
	  this->LastSimilarity,this->LastSmoothness*this->Lambda,
	  this->LastSmoothness);
  //  this->UpdateProgress(this->LastProgress);

  double sc=1.0/double(this->NumberOfLevels);
  double pog=(double(step)/double(this->NumberOfSteps+1) + double(this->NumberOfLevels-level))*sc;
  this->LastProgress=pog;
  this->UpdateProgress(pog);
  vtkDebugMacro(<<this->OutputStatus);
  return;
}

//-------------------------------------------------------------------------
// Optimizer Stuff 
//-------------------------------------------------------------------------
void vtkpxAbstractBiasFieldCorrection::SetCurrentParameters(vtkDoubleArray* param)
{
  return;
}
//-------------------------------------------------------------------------
void vtkpxAbstractBiasFieldCorrection::GetCurrentParameters(vtkDoubleArray* param)
{
  return;
}
//-------------------------------------------------------------------------
double vtkpxAbstractBiasFieldCorrection::OptimizerValue(vtkDoubleArray* params)
{
  this->SetCurrentParameters(params);
  return -this->Evaluate();
}
//-------------------------------------------------------------------------
double vtkpxAbstractBiasFieldCorrection::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)

{
  return 0.0;
}


double vtkpxAbstractBiasFieldCorrection::OnBeginIteration(vtkDoubleArray* params)
{
  return 0.0;
}

//-------------------------------------------------------------------------
// Static Function Stuff
//-------------------------------------------------------------------------
double vtkpxAbstractBiasFieldCorrection::CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output)
{
  return ((vtkpxAbstractBiasFieldCorrection*)self)->OptimizerGradient(position,output);
}

double vtkpxAbstractBiasFieldCorrection::CalculateFunction(vtkObject* self,vtkDoubleArray* position)
{
  return ((vtkpxAbstractBiasFieldCorrection*)self)->OptimizerValue(position);
}

double vtkpxAbstractBiasFieldCorrection::OnBeginIteration(vtkObject* self,vtkDoubleArray* position)
{
  return ((vtkpxAbstractBiasFieldCorrection*)self)->OnBeginIteration(position);
}

void vtkpxAbstractBiasFieldCorrection::OptimizerFeedback(vtkObject* self,const char* line,double val)
{
  ((vtkpxAbstractBiasFieldCorrection*)self)->SetOutputStatus(line,val);
}
//-------------------------------------------------------------------------

void vtkpxAbstractBiasFieldCorrection::SetOutputStatus(const char* line,double v)
{
  sprintf(this->OutputStatus,"**Opt=%s (%.5f)\n",line,v);
  vtkDebugMacro(<<this->OutputStatus);    
  this->UpdateProgress(this->LastProgress);
}
//-------------------------------------------------------------------------
int vtkpxAbstractBiasFieldCorrection::CheckInputsAndCreateReferenceImage(vtkImageData* input)
{
  if (input==NULL)
    return -1;

  int nc=input->GetNumberOfScalarComponents();
  fprintf(stderr,"Input had %d frames",nc);

  if (this->Frame<0)
    this->Frame=0;
  else if (this->Frame>=nc)
    this->Frame=nc-1;

  if (this->ReferenceImage!=NULL)
    this->ReferenceImage->Delete();
  this->ReferenceImage=vtkImageData::New();

  if (nc==1)
    {
      this->ReferenceImage->ShallowCopy(input);
    }
  else
    {
      // Check this code -- it was giving a freeze on Windows XP Home if there was
      // only one component!
      vtkImageExtractComponents* extr=vtkImageExtractComponents::New();
      extr->SetInput(input);
      extr->SetComponents(this->Frame);
      extr->Update();
      this->ReferenceImage->ShallowCopy(extr->GetOutput());
      fprintf(stderr,"Extracting Frame %d for Bias Field Correction\n",this->Frame);
    }

  if (this->MaskImage==NULL)
    return 0;


  int dim[3]; this->MaskImage->GetDimensions(dim);
  int dam[3]; this->ReferenceImage->GetDimensions(dam);

  int sum=0;
  for (int i=0;i<=2;i++)
    sum+=abs(dim[i]-dam[i]);

  if (sum==0)
    return 1;

  this->SetMaskImage(NULL);
  return 0;
}
//-------------------------------------------------------------------------
void vtkpxAbstractBiasFieldCorrection::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  this->CheckInputsAndCreateReferenceImage(input);
  return;
}
//-------------------------------------------------------------------------
vtkImageData* vtkpxAbstractBiasFieldCorrection::GenerateSynthetic(int width,double factor)
{
  if (width<16)
    width=16;
  else if (width>256)
    width=256;

  if (factor < 0.000001)
    factor=0.000001;
  else if (factor>0.1)
    factor=0.1;

  int offset=width/7;

  vtkImageData* tmp=vtkImageData::New();
  tmp->SetDimensions(width,width,width);
  tmp->SetScalarTypeToShort();
  tmp->AllocateScalars();

  int mid=width/2;

  for (int slice=0;slice<width;slice++)
    for (int j=0;j<width;j++)
      {
	int count=0;
	int mode=0;
	for (int i=0;i<width;i++)
	  {
	    float v=0.0;
	    if (mode==0)
	      v=vtkpxMath::GaussianRandom()*5.0+100.0;
	    else
	      v=vtkpxMath::GaussianRandom()*5.0+50.0;
	    

	    float dist2=pow(float(i-mid),float(2.0))+pow(float(j-mid),float(2.0))+pow(float(slice-mid),float(2.0));
	    float dist=sqrt(dist2);
	    
	    double newv=v*(1.0-(factor*dist)*(factor*dist));

	    if (newv<0.0)
	      newv=0.0;
	    
	    tmp->SetScalarComponentFromDouble(i,j,slice,0,newv);
	    ++count;
	    if (count==offset)
	      {
		mode=1-mode;
		count=0;
	      }
	  }
      }
  return tmp;
}
// -----------------------------------------------------------------------------------
vtkImageData* vtkpxAbstractBiasFieldCorrection::CreateLogReferenceImage(vtkImageData* input)
{
  if (input==NULL)
    {
      fprintf(stderr,"Error NULL input in Log Reference Input\n");
      return NULL;
    }

  vtkImageData* refimage=vtkImageData::New();
  refimage->CopyStructure(input);
  refimage->AllocateScalars();

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* ref=refimage->GetPointData()->GetScalars();

  int nt=inp->GetNumberOfTuples();
  int nc=inp->GetNumberOfComponents();

  for (int i=0;i<nt;i++)
    for (int j=0;j<nc;j++)
      {
	double v=inp->GetComponent(i,j);
	if (v>1.0)
	  ref->SetComponent(i,j,log(v));
	else
	  ref->SetComponent(i,j,0.0);
      }
  return refimage;
}

// -----------------------------------------------------------------------------------
vtkImageData* vtkpxAbstractBiasFieldCorrection::ComputeRatioBiasFieldImage(vtkImageData* original,vtkImageData* corrected)
{
  if (original==NULL || corrected==NULL)
    {
      fprintf(stderr,"Error NULL input ComputeRatioBiasFieldImage\n");
      return NULL;
    }

  int np1=original->GetNumberOfPoints();
  int np2=corrected->GetNumberOfPoints();
  
  if (np1!=np2)
    {
      fprintf(stderr,"Unqueal Size Images in ComputeRatioBiasFieldImage\n");
      return NULL;

    }

  vtkImageData* bfield=vtkImageData::New();
  bfield->CopyStructure(corrected);
  bfield->SetNumberOfScalarComponents(1);
  bfield->SetScalarTypeToFloat();
  bfield->AllocateScalars();

  vtkDataArray* orig =original->GetPointData()->GetScalars();
  vtkDataArray* corr =corrected->GetPointData()->GetScalars();
  vtkDataArray* bfld =bfield->GetPointData()->GetScalars();

  bfld->FillComponent(0,1.0);

  for (int i=0;i<np1;i++)
    {
      double x=orig->GetComponent(i,0);
      double y=corr->GetComponent(i,0);
      // y=x/b --> b=x/y

      if (fabs(y)>1.0)
	bfld->SetComponent(i,0,x/y);
    }

  return bfield;
}

