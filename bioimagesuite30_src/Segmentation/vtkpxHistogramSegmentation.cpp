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
  Module:    $RCSfile: vtkpxHistogramSegmentation.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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
#include "vtkObjectFactory.h"
#include "vtkpxHistogramSegmentation.h"
#include "vtkpxUtil.h"
#include "vtkFloatArray.h"
#include "vtkDoubleArray.h"
#include "vtkpxUtil.h"
#include "vtkImageAccumulate.h"
#include "vtkImageExtractComponents.h"
#include "vtkpxMath.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkpxImageAlgebra.h"
#include "vtkImageThreshold.h"
#include "vtkpxImageRobustHistogram.h"
#include "vtkImageSeedConnectivity.h"
#include "vtkImageMedian3D.h"
#include "pxisinf.h"
#include "vtkpxSplineStackSource.h"
//------------------------------------------------------------------------------


vtkpxHistogramSegmentation* vtkpxHistogramSegmentation::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxHistogramSegmentation");
  if(ret)
    {
      return (vtkpxHistogramSegmentation*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxHistogramSegmentation;
}

// Construct object with no children.
vtkpxHistogramSegmentation::vtkpxHistogramSegmentation()
{
  this->NumberOfClasses=3;
  this->Iterations=30;
  this->Parameters=NULL;
  this->InitialParameters=NULL;
  this->Histogram=NULL;
  this->Convergence=0.5;
  this->NumberOfBins=128;
  this->HistogramOrigin=0.0;
  this->HistogramSpacing=1.0;
  this->Sigma=1.0;
  this->GenerateLabels=0;
  this->OutlierDetection=0;
  this->OutlierProbability=0.0;
  this->PreprocessingMode=2;
  this->MaskImage=NULL;
  this->DistributionModel=0;
  this->MaxSigmaRatio=0.2;
  this->RobustScaleFactor=4.0;
  this->KeyChannel=0;
}
// ----------------------------------------------------------------------------
vtkpxHistogramSegmentation::~vtkpxHistogramSegmentation()
{
  
  if (this->Parameters!=NULL)
    this->Parameters->Delete();
  this->Parameters=NULL;

  if (this->Histogram!=NULL)
    this->Histogram->Delete();
  this->Histogram=NULL;

  this->SetMaskImage(NULL);

  this->SetInitialParameters(NULL);

}
// ----------------------------------------------------------------------------
float vtkpxHistogramSegmentation::Metric(float x,float m,float sigma2)
{
  if (this->DistributionModel==0)
    return vtkpxMath::Gaussian(x,m,sigma2);
  else if (this->DistributionModel==1)
    return exp(-fabs(x-m)/sqrt(sigma2))/(2.0*sqrt(sigma2));

  return 0.5*(vtkpxMath::Gaussian(x,m,sigma2)+vtkpxMath::Gaussian(x,m,this->RobustScaleFactor*this->RobustScaleFactor*sigma2));
}

float vtkpxHistogramSegmentation::GetGaussianDifference(float x,float mean1,float mean2,float var1,float var2)
{
  return vtkpxMath::Gaussian(x,mean1,var1)-vtkpxMath::Gaussian(x,mean2,var2);
}

float vtkpxHistogramSegmentation::GetThreshold(float mean1,float mean2,float sigma1,float sigma2)
{
  float thr=0.01;

  if (fabs(mean1-mean2)<thr)
    return 0.5*(mean1+mean2);

  float var1=sigma1*sigma1;
  float var2=sigma2*sigma2;

  float upperbound=mean1;
  float lowerbound=mean2;
	  
  float test=0.5*(upperbound+lowerbound);
  float v=0.0;

  while (fabs(upperbound-lowerbound)>thr)
    {
      v=vtkpxHistogramSegmentation::GetGaussianDifference(test,mean1,mean2,var1,var2);
      //      fprintf(stderr,"Testing %.2f (%.2f : %.2f)\n",test,lowerbound,upperbound);

      if (v>0.0)
	upperbound=test;
      else
	lowerbound=test;
      test=0.5*(upperbound+lowerbound);
    }
  //  fprintf(stderr,"Found Threshold %.2f v=%.6f\n",v,test);

  return test;
}
// ----------------------------------------------------------------------------
int vtkpxHistogramSegmentation::CreateHistogram(vtkImageData* input,int NumBins)
{
  if (input==NULL)
    return 0;
  
  if (NumBins<4)
    NumBins=4;

  int UseMask=0;
  if (this->MaskImage!=NULL)
    {
      int nt=this->MaskImage->GetPointData()->GetScalars()->GetNumberOfTuples();
      int nt2=input->GetPointData()->GetScalars()->GetNumberOfTuples();
      if (nt==nt2)
	UseMask=1;
    }


  double range[2]; input->GetPointData()->GetScalars()->GetRange(range);
  float minv=range[0];
  float maxv=range[1];

  if (this->PreprocessingMode==2)
    vtkpxUtil::ImageRobustRange(input,0.002,0.998,minv,maxv);
  else if (this->PreprocessingMode==1 && minv<1.0)
    minv=1.0;

  this->HistogramOrigin=minv;
  this->HistogramSpacing=1.0;

  this->NumberOfBins=NumBins;
  int drange=int(maxv-minv+1);
  
  if (drange< this->NumberOfBins)
    this->NumberOfBins=drange;
  
  while(drange> this->NumberOfBins*this->HistogramSpacing)
    this->HistogramSpacing+=1.0;
  

  fprintf(stderr,"\t Input range = %.0f:%.0f   robust=%.0f:%.0f \n",range[0],range[1],minv,maxv);
  fprintf(stderr,"\t Input bins  = %d          output=%d (drange=%d)\n",NumBins,this->NumberOfBins,drange);
  fprintf(stderr,"\t Histogram origin=%.0f spacing=%.0f\n",this->HistogramOrigin,this->HistogramSpacing);

  this->NumberOfBins=int(drange/this->HistogramSpacing+0.5);

  vtkpxImageRobustHistogram* accumulate=vtkpxImageRobustHistogram::New();
  accumulate->SetInput(input);
  accumulate->InterpolationOff();
  accumulate->RobustModeOff();
  accumulate->SaturationModeOff();
  accumulate->SetLowerThreshold(range[0]);
  accumulate->SetUpperThreshold(range[1]);
  accumulate->SetNumberOfBins(this->NumberOfBins);
  if (UseMask)
    {
      accumulate->SetWeightImage(this->MaskImage);
      accumulate->SetIgnoreMaskedVoxels(0);
    }
  accumulate->Update();

  vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
  smooth->SetInput(accumulate->GetOutput());
  smooth->SetStandardDeviations(this->Sigma,0.0,0.0);
  smooth->Update();

  if (this->Histogram!=NULL)
    this->Histogram->Delete();

  this->Histogram=vtkImageData::New();
  this->Histogram->ShallowCopy(smooth->GetOutput());
  accumulate->Delete();
  smooth->Delete();
  return 1;
}


int vtkpxHistogramSegmentation::InitializeParameters(vtkImageData* histogram,int NumClasses,vtkFloatArray* params)
{
  if (histogram==NULL)
    return 0;

  this->NumberOfClasses=NumClasses;
  if (this->NumberOfClasses<1)
    this->NumberOfClasses=1;
  
  params->SetNumberOfComponents(3);
  params->SetNumberOfTuples(this->NumberOfClasses+this->OutlierDetection);
  params->FillComponent(0,1.0);

  int dim[3]; this->Histogram->GetDimensions(dim);

  double range[2];
  range[0]=this->HistogramOrigin;
  range[1]=this->HistogramSpacing*float(dim[0]-1);

  fprintf(stderr,"Beginning range = %.0f:%.0f\n Initial Means:\n",range[0],range[1]);

  int numtuples=this->Parameters->GetNumberOfTuples();
  float s=(range[1]-range[0])/float(this->NumberOfClasses);

  if (this->InitialParameters!=NULL)
    {

      int nc=this->InitialParameters->GetNumberOfComponents();
      int docopy=1;
      if (nc==0)
	docopy=0;

      if (numtuples==this->InitialParameters->GetNumberOfTuples())
	docopy=1;

      if (numtuples==this->InitialParameters->GetNumberOfTuples()+1 && this->OutlierDetection==1)
	docopy=1;

      int offset=0;
      if (nc>2)
	offset=1;

      if (docopy)
	{
	  fprintf(stderr,"\n\n ****** Using Initial Parameters ************ \n");
	  for (int j=0;j<numtuples-this->OutlierDetection;j++)
	    {
	      params->SetComponent(j,1,this->InitialParameters->GetComponent(j,0+offset));
	      if (nc>1)
		params->SetComponent(j,2,this->InitialParameters->GetComponent(j,1+offset));
	      else
		params->SetComponent(j,2,s);

	    }
	  return 1;
	}
    }
  for (int j=0;j<numtuples;j++)
    {
      float s=(range[1]-range[0])/float(this->NumberOfClasses);
      float m =range[0]+ float(j+0.5)/float(this->NumberOfClasses)*(range[1]-range[0]);
      // fprintf(stderr,"\t c=%d   mean=%.1f   sigma=%.1f\n",j+1,m,s);
      params->SetComponent(j,1,m);
      params->SetComponent(j,2,s);
      params->SetComponent(j,0,1.0);
    }
 

  return 1;
}


// ---------------------------------------------------------------------------------------------
//
//     Execute Segmentation 
//
// ----------------------------------------------------------------------------
void vtkpxHistogramSegmentation::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();
  output->SetScalarTypeToFloat();
  output->SetNumberOfScalarComponents(1);
}

void vtkpxHistogramSegmentation::SimpleExecute(vtkImageData* input,vtkImageData* output )
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxHistogramSegmentation");
      return;
    }

  int nc=input->GetNumberOfScalarComponents();

  if (this->KeyChannel<0)
    this->KeyChannel=0;
  if (this->KeyChannel>=nc)
    this->KeyChannel=nc-1;

  // Restrict input to single frame and float
  vtkImageExtractComponents* extr=vtkImageExtractComponents::New();
  extr->SetInput(input);
  extr->SetComponents(this->KeyChannel);
  extr->Update();
  this->UpdateProgress(0.1);

  if (this->Parameters==NULL)
    this->Parameters=vtkFloatArray::New();

  this->HistogramSegmentation(extr->GetOutput(),output,this->Parameters,0);
  this->UpdateProgress(1.0);
  extr->Delete();

}

void vtkpxHistogramSegmentation::HistogramSegmentation(vtkImageData* single_channel,vtkImageData* labels,vtkFloatArray* parameters,
						       int useparams,int usevar)
{
  this->CreateHistogram(single_channel,this->NumberOfBins);
  if (useparams==0)
    this->InitializeParameters(this->Histogram,this->NumberOfClasses,parameters);

  this->UpdateProgress(0.05);

  int dim[3]; this->Histogram->GetDimensions(dim);
  vtkDataArray* data=this->Histogram->GetPointData()->GetScalars();

  // ----------------- EM Estimation -------------------------------------
  int numclasses=this->Parameters->GetNumberOfTuples();
  float* mean =new float[numclasses];
  float* sigma2=new float[numclasses];
  float* sum  =new float[numclasses];
  float* sum2 =new float[numclasses];
  float*  num =new float[numclasses];

  fprintf(stderr,"\n**** Initial Parameters (maxsigmaratio=%.3f, Dist Model=%d, RobustScaleFactor=%.2f) \n --------------------------\n",
	  this->MaxSigmaRatio,this->DistributionModel,this->RobustScaleFactor);
  for (int j=0;j<numclasses;j++)
    {
      sigma2[j]=parameters->GetComponent(j,2);      sigma2[j]*=sigma2[j];
      mean[j] =parameters->GetComponent(j,1);
      fprintf(stderr,"\t c=%d   mean=%.1f   sigma=%.1f\n",j+1,mean[j],sqrt(sigma2[j]));
    }

  // ----------------------------------------------------------------------------------
  //            Iterations
  // ----------------------------------------------------------------------------------
  int iter=1;
  float error=this->Convergence+1.0;
  
  while (iter <= this->Iterations && error > this->Convergence)
    {
      error=0.0;
      float totalnum=0.0;
      
      for (int i=0;i<numclasses;i++)	{ sum[i]=0.0; sum2[i]=0.0; num[i]=0.0; }
      
      for (int bin=0;bin<dim[0];bin++)
	{
	  float v=this->HistogramOrigin+float(bin)*this->HistogramSpacing;
	  float numv=data->GetComponent(bin,0);
	  if (isnan(numv))
	    numv=0.0;
	  float bestp=0.0;
	  int   bestc=0;

	  for (int c=0;c<numclasses;c++)
	    {
	      float t=this->OutlierProbability;
	      if (c<this->NumberOfClasses)
		{
		  if (usevar)
		    t=this->Metric(v,mean[c],sigma2[c]);
		  else
		    t=this->Metric(v,mean[c],sigma2[0]);
		}
	      if (t>bestp)  {  bestp=t;  bestc=c;  }
	    }
	  num[bestc]+=numv;  sum[bestc]+=v*numv;  sum2[bestc]+=v*v*numv;  totalnum+=numv;
	}
      
     fprintf(stderr,"It=%03d : \t",iter);
     
     // Compute Parameters
     // ------------------
     float maxsigma=0.0;
     for (int c=0;c<numclasses;c++)
       {
	 //	 fprintf(stderr,"c=%d num[c]=%.1f  sum[c]=%.1f sum2[c]=%.1f\t",c,num[c],sum[c],sum2[c]);
	 if (num[c]<1.0)  num[c]=1.0;

	 //	 fprintf(stderr,"num[c]=%.1f  sum[c]=%.1f sum2[c]=%.1f\n",num[c],sum[c],sum2[c]);

	 float m=sum[c]/num[c];
	 if (c<this->NumberOfClasses)
	   error=Fmax(fabs(m-mean[c]),error);
	 
	 mean[c]=m;
	 sigma2[c]=(sum2[c]/num[c]-mean[c]*mean[c]);
	 if (this->DistributionModel==1)
	   sigma2[c]=0.5*sigma2[c];
	 
	 if (isnan(sigma2[c]) || sigma2[c]<2.0)
	   sigma2[c]=2.0;

	 float s=sqrt(sigma2[c]);
	 if (isnan(s))
	   s=2.0;
	 if (s>maxsigma)
	   maxsigma=s;
       }

     // Enforce MaxSigmaRatio (0.1)
     // ---------------------------
     for (int c=0;c<numclasses;c++)
       {
	 float s=sqrt(sigma2[c]);
	 if (s<this->MaxSigmaRatio*maxsigma)
	   sigma2[c]=pow(this->MaxSigmaRatio*maxsigma,float(2.0));
	 if (isnan(sigma2[c]))
	   sigma2[c]=2.0;
	 parameters->SetComponent(c,1,mean[c]);
	 parameters->SetComponent(c,2,sqrt(sigma2[c]));
	 parameters->SetComponent(c,0,num[c]);
	 fprintf(stderr,"%.1f + %.1f (%5.2f)\t",mean[c],sqrt(sigma2[c]),100.0*num[c]/totalnum);
       }
      fprintf(stderr,"usevar=%d error=%.2f\n",usevar,error);
      ++iter;
    }

  // At this point sort via mean[c]
  // ------------------------------
  fprintf(stderr,"\n\nSorting \n-----------------\n");
  for (int c=0;c<this->NumberOfClasses;c++)
    {
      int best=c;
      float bestm=mean[c];

      fprintf(stderr,"Orig: class %d :  mean=%.2f sigma=%.2f number=%.2f\n\t Means:",best,
	      parameters->GetComponent(best,1),
	      parameters->GetComponent(best,2),parameters->GetComponent(best,0));
      
      for (int c1=0;c1<this->NumberOfClasses;c1++)
	fprintf(stderr,"%.2f ",mean[c1]);
      fprintf(stderr,"\n");

      for (int c1=0;c1<this->NumberOfClasses;c1++)
	{
	  if (mean[c1]<bestm)
	    {
	      bestm=mean[c1];
	      best=c1;
	    }
	}
      
      parameters->SetComponent(c,1,mean[best]);
      parameters->SetComponent(c,2,sqrt(sigma2[best]));
      parameters->SetComponent(c,0,num[best]);
      mean[best]=1e+10;
      //      sigma2[best]=sigma2[c];
      //      num[best]=num[c];

      fprintf(stderr,"\t\t Final class %d : orig=%d mean=%.2f sigma=%.2f number=%.2f\n",c,best,
	      parameters->GetComponent(c,1),
	      parameters->GetComponent(c,2),parameters->GetComponent(c,0));



    }

  //  vtkDataArray* outdata=labels->GetPointData()->GetScalars();
  //  outdata->FillComponent(0,0.0);
  if (this->GenerateLabels)
    {
      float *thresholds=new float[numclasses];
      for (int j=0;j<(numclasses-1);j++)
	thresholds[j]=this->GetThreshold(parameters->GetComponent(j,1),parameters->GetComponent(j+1,1),
					 parameters->GetComponent(j,2),parameters->GetComponent(j+1,2));
      thresholds[numclasses-1]=1e+10;
      
      vtkDoubleArray* map=vtkDoubleArray::New();
      map->SetNumberOfComponents(3);
      map->SetNumberOfTuples(numclasses);
      
      for (int j=0;j<numclasses;j++)
	{
	  map->SetComponent(j,0,thresholds[j]);
	  map->SetComponent(j,1,float(j));
	  map->SetComponent(j,2,0.0);
	  //	  fprintf(stderr,"If x < %.3f --> x=%.2f\n",thresholds[j],float(j));
	}

      vtkpxImageAlgebra* algebra=vtkpxImageAlgebra::New();
      algebra->SetInput(single_channel);
      algebra->SetMap(map);
      algebra->SetOutputScalarTypeToShort();
      algebra->Update();
      labels->ShallowCopy(algebra->GetOutput());
      algebra->Delete();

      delete [] thresholds;
    }
  delete [] mean;  delete [] sigma2;  delete [] sum;  delete [] sum2;  delete [] num;
}

// -------------------------------------------------------------------------------------------------------------------------------------------
int vtkpxHistogramSegmentation::HistogramNormalizeImage(vtkImageData* image,float pmin,float pmax)
{
  if (image==NULL)
    {
      fprintf(stderr,"Bad Input to Histogram Normalize Image\n");
      return 0;
    }

  pmin=Frange(pmin,0.0,0.95);
  pmax=Frange(pmax,pmin+0.05,1.0);

  float tlow,thigh;
  vtkpxUtil::ImageRobustRange(image,pmin,pmax,tlow,thigh);
  double range[2]; image->GetPointData()->GetScalars()->GetRange(range);
  fprintf(stderr,"Histogram Image Normalization (%.2f:%.2f) r=(%.1f:%.1f) --> ",pmin,pmax,range[0],range[1]);

  vtkDoubleArray* map=vtkDoubleArray::New();
  map->SetNumberOfComponents(3);
  map->SetNumberOfTuples(3);
  map->SetComponent(0,0,tlow);   map->SetComponent(0,1,tlow);   map->SetComponent(0,2,0.0);
  map->SetComponent(1,0,thigh);  map->SetComponent(1,1,0.0);    map->SetComponent(1,2,1.0);
  map->SetComponent(2,0,range[1]+10.0);   map->SetComponent(2,1,thigh);   map->SetComponent(2,2,0.0);
  vtkpxImageAlgebra* algebra=vtkpxImageAlgebra::New();

  algebra->SetInput(image);
  algebra->SetMap(map);
  algebra->Update();
  image->ShallowCopy(algebra->GetOutput());
  algebra->Delete();
  map->Delete();
  image->GetPointData()->GetScalars()->GetRange(range);
  fprintf(stderr,"(%.1f : %.1f)\n",range[0],range[1]);

  return 1;
}

// -------------------------------------------------------------------------------------------------------------------------------------------
vtkImageData* vtkpxHistogramSegmentation::ComputeFatValues(vtkImageData* orig_image,vtkImageData* orig_map,vtkpxSplineStackSource* surface,
							   int abd_fat_value,int artifact_value,int subq_fat_value,int numclasses)
{
  vtkImageData* output=vtkImageData::New();
  output->CopyStructure(orig_map);
  output->AllocateScalars();

  fprintf(stderr,"\n\n+++++ Creating Surface Object Map\n");
  surface->CreateObjectMap(output);

  vtkImageData* artifactfree=vtkImageData::New();
  artifactfree->DeepCopy(orig_image);

  fprintf(stderr,"+++++ Eliminating Artifacts = %d\n",artifact_value);
  vtkDataArray* inp=artifactfree->GetPointData()->GetScalars();
  vtkDataArray* map=orig_map->GetPointData()->GetScalars();
  int np=map->GetNumberOfTuples();
  for (int i=0;i<np;i++)
    {
      if (map->GetComponent(i,0)==artifact_value)
	inp->SetComponent(i,0,0.0);
    }


  fprintf(stderr,"+++++ Histogram Segmentation\n");

  vtkpxHistogramSegmentation* hist=vtkpxHistogramSegmentation::New();
  hist->DebugOn();
  hist->SetInput(artifactfree);
  hist->SetNumberOfClasses(numclasses);
  hist->SetGenerateLabels(1);
  hist->DebugOff();
  hist->Update();

  //  vtkpxUtil::SaveAnalyze("hist.hdr",hist->GetOutput(),0);


  fprintf(stderr,"+++++ Final Objectmap Output\n");
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* back=hist->GetOutput()->GetPointData()->GetScalars();

  for (int j=0;j<np;j++)
    {
      float bk  =  back->GetComponent(j,0);
      float abd =   out->GetComponent(j,0);
      float visc=   map->GetComponent(j,0);
      
      if (visc!=artifact_value)
	{
	  if (abd>0)
	    {
	      if (visc==abd_fat_value && abd ==2 )
		out->SetComponent(j,0,3);
	      else if (abd==1 && bk>2 )
		out->SetComponent(j,0,2);
	      else
		out->SetComponent(j,0,0);
	    }
	 
	} 
      else
	{
	  out->SetComponent(j,0,artifact_value);
	}
    }

  hist->Delete();
  artifactfree->Delete();
  return output;
}

vtkImageData* vtkpxHistogramSegmentation::CreateRayleighSynthetic(float beta,float m1,float sigma1,float m2,float sigma2)
{
  vtkImageData* tmp=vtkImageData::New();
  int dim=128;
  tmp->SetDimensions(dim,dim,dim);
  tmp->SetScalarTypeToShort();
  tmp->AllocateScalars();

  int mid=dim/2;

  float sum=0.0,sumn=0.0,sum2=0.0;

  for (int k=0;k<dim;k++)
    for (int j=0;j<dim;j++)
      for (int i=0;i<dim;i++)
	{
	  float signal=vtkpxMath::GaussianRandom();
	  float noise=vtkpxMath::GaussianRandom();
	  float noise2=vtkpxMath::GaussianRandom();
	  noise=beta*sqrt(noise*noise+noise2*noise2);	

	  if ( i<dim/3)
	    {
	      signal=sigma1*signal+m1;

	    }
	  else if (i<2*dim/3)
	    {
	      signal=sigma2*signal+m2;

	    }
	  else
	    {
	      signal=noise;
	      
	    }
	  tmp->SetScalarComponentFromDouble(i,j,k,0,signal);
	  sum+=noise;
	  sum2+=noise*noise;
	  sumn+=1.0;
	}

  double mean=sum/sumn;
  double sigma=sqrt(sum2/sumn-mean*mean);
  double sig2 =sqrt(sum2/sumn-beta*beta);
  fprintf(stderr,"Gaussian Fit=%.2f,%.2f or %.2f\n",mean,sigma,sig2);

  return tmp;
}
	    

// -------------------------------------------------------------------------------------------------------------------------------------------
vtkImageData* vtkpxHistogramSegmentation::ComputeCaprioFatValues(vtkImageData* orig_map,
								 vtkpxSplineStackSource* visceral_surface,
								 vtkpxSplineStackSource* abdomen_surface,
								 vtkpxSplineStackSource* fascia_surface,
								 vtkpxSplineStackSource* skin_surface,
								 float minv,float maxv)
{
  vtkImageData* output=vtkImageData::New();
  output->CopyStructure(orig_map);
  output->AllocateScalars();

  // inside =2 , outside =1 offslice =0
  vtkImageData* internal=vtkImageData::New(); internal->CopyStructure(orig_map); internal->AllocateScalars(); 
  visceral_surface->CreateObjectMap(internal);
  
  vtkImageData* abdomen=vtkImageData::New();  abdomen->CopyStructure(orig_map); abdomen->AllocateScalars(); 
  abdomen_surface->CreateObjectMap(abdomen);
  
  vtkImageData* deepsub=vtkImageData::New();    deepsub->CopyStructure(orig_map); deepsub->AllocateScalars(); 
  fascia_surface->CreateObjectMap(deepsub);
  
  vtkImageData* superficial=vtkImageData::New(); superficial->CopyStructure(orig_map); superficial->AllocateScalars(); 
  skin_surface->CreateBoundedObjectMap(fascia_surface,superficial);
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* inner=internal->GetPointData()->GetScalars();
  vtkDataArray*   abd=abdomen->GetPointData()->GetScalars();
  vtkDataArray*  deep=deepsub->GetPointData()->GetScalars();
  vtkDataArray* super=superficial->GetPointData()->GetScalars();
  
  vtkDataArray* inp=orig_map->GetPointData()->GetScalars();
  out->FillComponent(0,0.0);

  if (maxv==-1.0 || minv==-1.0)
    {
      double range[2]; inp->GetRange(range);
      minv=range[1]-0.2;
      maxv=range[1]+0.2;
    }

  int np=out->GetNumberOfTuples();

  for (int index=0;index<np;index++)
    {
      double fat=inp->GetComponent(index,0);
      
      if (inner->GetComponent(index,0)==2)
	{
	  if (fat>=minv && fat <=maxv)
	    out->SetComponent(index,0,3.0);
	}
      else if (abd->GetComponent(index,0)==1)
	{
	  // outside the abdomen
	  if (fat>=minv && fat <=maxv)
	    {
	      if (deep->GetComponent(index,0)==2)
		out->SetComponent(index,0,1.0);
	      else if (super->GetComponent(index,0)==2)
		out->SetComponent(index,0,2.0);
	      else if (super->GetComponent(index,0)==3)
		out->SetComponent(index,0,4.0);
	    }
	}
    }

  internal->Delete();
  abdomen->Delete();
  deepsub->Delete();
  superficial->Delete();
  return output;
}
  
// -------------------------------------------------------------------------------------------------------------------------------------------
int vtkpxHistogramSegmentation::ComputeSegmentedMapStatistics(vtkImageData* image,vtkImageData* map,vtkFloatArray* output_values)
{
  if (image==NULL || map==NULL || output_values==NULL)
    return 0;


  double range[2]; 
  vtkDataArray* labels=map->GetPointData()->GetScalars();
  vtkDataArray* values=image->GetPointData()->GetScalars();

  if (labels->GetNumberOfTuples()!=values->GetNumberOfTuples())
    {
      fprintf(stderr,"Bad Match between labels and image\n");
      return 0;
    }

  labels->GetRange(range,0);

  int pbmap=0,numclasses=1;
  if (map->GetNumberOfScalarComponents()==1)
    {
      
      numclasses=int(range[1]-range[0]+1);
      if (numclasses>20)
	{
	  fprintf(stderr,"Too many classes for meaningful statistics\n");
	  return 0;
	}
    }
  else
    {
      pbmap=1;
      numclasses=map->GetNumberOfScalarComponents()-1;
    }

  fprintf(stderr,"Numclasses=%d numcomponents=%d pbmap=%d\n",numclasses,
	  map->GetNumberOfScalarComponents(),pbmap);

  int numcomponents=image->GetNumberOfScalarComponents();
  
  output_values->SetNumberOfComponents(4*numcomponents); // volume , numpix, mean, std   for each component
  output_values->SetNumberOfTuples(numclasses);
  
  for (int j=0;j<4*numcomponents;j++)
    output_values->FillComponent(j,0);
  
  int nt=labels->GetNumberOfTuples();
  
  for (int i=0;i<nt;i++)
    {
      if (pbmap==0)
	{
	  int l=(int)(labels->GetComponent(i,0)-range[0]);
	  for (int c=0;c<numcomponents;c++)
	    {
	      float v=values->GetComponent(i,c);
	      int index=c*4;
	      
	      // Count Pixels
	      output_values->SetComponent(l,index+1,output_values->GetComponent(l,index+1)+1.0);
	      output_values->SetComponent(l,index+2,output_values->GetComponent(l,index+2)+v);
	      output_values->SetComponent(l,index+3,output_values->GetComponent(l,index+3)+v*v);
	    }
	}
      else
	{
	  for (int c=0;c<numcomponents;c++)
	    {
	      float v=values->GetComponent(i,c);
	      int index=c*4;
	      for (int cl=0;cl<numclasses;cl++)
		{
		  float prob=labels->GetComponent(i,cl+1);
		  output_values->SetComponent(cl,index+1,output_values->GetComponent(cl,index+1)+prob);
		  output_values->SetComponent(cl,index+2,output_values->GetComponent(cl,index+2)+v*prob);
		  output_values->SetComponent(cl,index+3,output_values->GetComponent(cl,index+3)+v*v*prob);
		}
	    }
	}
    }
  
  double sp[3]; image->GetSpacing(sp);
  float volpix=sp[0]*sp[1]*sp[2];
  
  for (int l=0;l<numclasses;l++)
    for (int c=0;c<numcomponents;c++)
      {
	int index=c*4;
	
	// First Volume
	float numvox=output_values->GetComponent(l,index+1);
	output_values->SetComponent(l,index,numvox*volpix);
	
	// Then Mean
	float mean=output_values->GetComponent(l,index+2);
	float mean2=output_values->GetComponent(l,index+3);
	
	mean=mean/numvox;
	mean2=mean2/numvox-mean*mean;
	
	output_values->SetComponent(l,index+2,mean);
	output_values->SetComponent(l,index+3,sqrt(mean2));
	
	/*	fprintf(stderr,"Label =%d  Component=%d (index=%d (nt=%d,nc=%d))  Volume=%.2f (%.0f)\t Mean=%.2f std=%.2f\n",
		l+1,c+1,index,
		output_values->GetNumberOfTuples(),
		output_values->GetNumberOfComponents(),
		output_values->GetComponent(l,index),
		output_values->GetComponent(l,index+1),
		output_values->GetComponent(l,index+2),
		output_values->GetComponent(l,index+3));*/
      }


  return 1;
}

