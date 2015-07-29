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
  Module:    $RCSfile: vtkpxMRFSegmentation.cpp,v $
  Language:  C++
  Date:      $Date: 2005/08/19 18:34:53 $
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
#include "vtkpxMRFSegmentation.h"
#include "vtkpxUtil.h"
#include "vtkFloatArray.h"
#include "vtkpxMath.h"
#include "vtkImageShiftScale.h"
#include "vtkMath.h"
#include "vtkImageExtractComponents.h"
#include "vtkbisImageReslice.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageCast.h"
#include "vtkImageShiftScale.h"
#include "vtkpxAbstractBiasFieldCorrection.h"
#include "pxisinf.h"


vtkpxMRFSegmentation* vtkpxMRFSegmentation::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxMRFSegmentation");
  if(ret)
    {
    return (vtkpxMRFSegmentation*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxMRFSegmentation;
}

// Construct object with no children.
vtkpxMRFSegmentation::vtkpxMRFSegmentation()
{
  this->Smoothness=0.3;
  this->MRFIterations=8;
  this->DebugVoxel=0;

  this->MRFConvergencePercentage=0.2;
  this->PartialStep=0.125;
  this->FuzzyClassification=0;

  this->FitQualityMap=NULL;
  this->MultiFrameOutput=NULL;
  this->BiasField=NULL;
  this->BiasFieldWeights=NULL;
  this->HistogramNormalization=0;
  this->NumberOfChannels=1;
  this->SingleChannel=1;
  this->ImageNoiseSigma=1.0;
  this->MRFBasedFitQuality=1;
  this->MRFThreshold=0.1;
  this->ComputeBiasField=0;
  this->TensorSpline=vtkpxImageTensorSpline::New();
  this->PerformingFuzzyClassification=0;
  this->BiasFieldFactor=6.0;
  this->BiasFieldSigma=0.25;
}
// ----------------------------------------------------------------------------
vtkpxMRFSegmentation::~vtkpxMRFSegmentation()
{
  if (this->MultiFrameOutput!=NULL)
    this->MultiFrameOutput->Delete();

  if (this->BiasField!=NULL)
    this->BiasField->Delete();
  
  if (this->BiasFieldWeights!=NULL)
    this->BiasFieldWeights->Delete();

  this->SetFitQualityMap(NULL);

  if (this->TensorSpline!=NULL)
    this->TensorSpline->Delete();
}
// ----------------------------------------------------------------------------
//
//       M - Step 
//
// ----------------------------------------------------------------------------

void vtkpxMRFSegmentation::CombineMeansAndStandardDeviations(vtkFloatArray* params,int class1,int class2,float fraction1,float* theta,int debug)
{
  fraction1=Frange(fraction1,0.0,1.0);

  for (int channel=0;channel<this->NumberOfChannels;channel++)
    {

      int b0=this->ChannelMean(channel);
      int b1=this->ChannelSigma(channel);

      if (class1==class2)
	{
	  for (int k=b0;k<=b1;k++)
	    theta[k]=params->GetComponent(class1,k);
	  
	  if ( debug)
	    {
	      fprintf(stderr,"Combining channel=%d\n",channel);
	      fprintf(stderr,"\t class=%d =%.2f+%.2f \n",class1+1,theta[b0],theta[b1]);
	    }
	}
      else
	{
	  theta[b0]=fraction1*params->GetComponent(class1,b0)+(1.0-fraction1)*params->GetComponent(class2,b0);
	  theta[b1]=sqrt(pow(fraction1*params->GetComponent(class1,b1),2.0)+pow((1.0-fraction1)*params->GetComponent(class2,b1),2.0));
	  
	  
	  if ( debug)
	    {
	      fprintf(stderr,"\t channel =%d\n",channel+1);
	      fprintf(stderr,"\t class1=%d (%.2f)\t  %.2f+%.2f \n",class1+1,fraction1,
		      params->GetComponent(class1,b0),
		      params->GetComponent(class1,b1));
	      
	      fprintf(stderr,"\t class2=%d (%.2f)\t  %.2f+%.2f \n",class2+1,1-fraction1,
		      params->GetComponent(class2,b0),
		      params->GetComponent(class2,b1));
	      
	      fprintf(stderr,"\t combined  \t \t %.2f+%.2f \n\n",
		      theta[b0],theta[b1]);
	    }
	}
    }
  return;
  
}

// -------------------------------------------------------------------------------------------------------------------------------
//
//      Expectation Step Stuff 
//
// -------------------------------------------------------------------------------------------------------------------------------
void vtkpxMRFSegmentation::UpdateFitQualityMapFromMRF(vtkImageData* label_image,vtkImageData* fit_quality,float threshold)
{
  
  vtkDataArray* fitq=fit_quality->GetPointData()->GetScalars();
  vtkDataArray* labels=label_image->GetPointData()->GetScalars();

  int dim[3]; label_image->GetDimensions(dim);
  int incr[6]; float wgth[6]; this->ComputeMRFIncrementsAndWeights(label_image,incr,wgth);

  int mink=1,maxk=dim[2]-1;
  if (dim[2]==1)
    {
      mink=0;
      maxk=1;
    }

  int slicesize=dim[0]*dim[1];
  fitq->FillComponent(0,0.0);

  int numclasses=labels->GetNumberOfComponents();


  int numgood=0;
  int numtotal=fitq->GetNumberOfTuples();

  float ratio=0.0;

  while (ratio<25.0)
    {
      numgood=0;
      for (int k=mink;k<maxk;k++)
	for (int j=1;j<dim[1]-1;j++)
	  for (int i=1;i<dim[0]-1;i++)
	    {
	      int vox=i+j*dim[0]+k*slicesize;
	      double sum=0.0;
	      
	      for (int c=0;c<numclasses;c++)
		{
		  float g=0.0;
		  for (int ia=0;ia<=5;ia++)
		    {
		      if (wgth[ia]>0.0001)
			g+=wgth[ia]*labels->GetComponent(vox+incr[ia],c);
		    }
		  sum+=(pow(labels->GetComponent(vox,c)-g,2.0));
		}
	      
	      if (sum<threshold)
		{
		  fitq->SetComponent(vox,0,1.0);
		  numgood+=1;
		}
	    }
      
      ratio=100.0*float(numgood)/float(numtotal);
      fprintf(stderr,"\t\t ********* Fit Quality  %.2f  voxels good\n",ratio);
      threshold*=2.0;
    }
}
// -------------------------------------------------------------------------------------------------------------------------------
void vtkpxMRFSegmentation::ComputeClassSums(vtkImageData* label_image,
					    vtkImageData* fitted_image,
					    vtkImageData* fit_quality,
					    vtkImageData* bias_field,
					    float partial_threshold,
					    vtkFloatArray* params,int numclasses)
{
  vtkDataArray* fitted=fitted_image->GetPointData()->GetScalars();
  vtkDataArray* labels=label_image->GetPointData()->GetScalars();

  vtkDataArray* quality=NULL;
  if (fit_quality!=NULL)
    quality=fit_quality->GetPointData()->GetScalars();

  vtkDataArray* bfield=NULL;
  if (bias_field!=NULL)
    bfield=bias_field->GetPointData()->GetScalars();
  
  int nt=fitted->GetNumberOfTuples();

  int nparam=params->GetNumberOfComponents();
  for (int ia=0;ia<nparam;ia++)
    params->FillComponent(ia,0.0);

  /*  fprintf(stderr,"Number Of Components param=%d label=%d fitted=%d number of channels=%d ImageNoise=%.2f\n",
      nparam,labels->GetNumberOfComponents(),fitted->GetNumberOfComponents(),this->NumberOfChannels,this->ImageNoiseSigma);*/
  
  // Compute Sums
  // ------------
  //  partial_threshold=0.0;

  // Step 1 ------- Compute Sums and Sum Of Squares 

  for (int vox=0;vox<nt;vox++)
    {
      float w=1.0;
      if (quality!=NULL)
	w=quality->GetComponent(vox,0);

      float biasfieldvalue=1.0;
      if (bfield!=NULL)
	biasfieldvalue=bfield->GetComponent(vox,0);
      
      for (int c=0;c<numclasses;c++)
	{
	  float partial=labels->GetComponent(vox,c);
	  if (partial> partial_threshold)
	    {
	      partial=partial*w;
	      params->SetComponent(c,0,params->GetComponent(c,0)+partial);
	      
	      for (int channel=0;channel<this->NumberOfChannels;channel++)
		{
		  int b0=this->ChannelMean(channel);
		  int b1=this->ChannelSigma(channel);
		  float d_value=fitted->GetComponent(vox,channel)/biasfieldvalue;
		  
		  params->SetComponent(c,b0,params->GetComponent(c,b0)+partial*d_value);
		  params->SetComponent(c,b1,params->GetComponent(c,b1)+partial*d_value*d_value);
		}
	    }
	}
    }

  for (int c=0;c<numclasses;c++)
    {
      if (params->GetComponent(c,0)<1e-6)
	params->SetComponent(c,0,1e-6);
    }

}
// -------------------------------------------------------------------------------------------------------------------------------
int vtkpxMRFSegmentation::FitModelToSums(vtkFloatArray* ,int )
{
  return 0;
}
// -------------------------------------------------------------------------------------------------------------------------------
  
void vtkpxMRFSegmentation::ComputeClassMeansAndStandardDeviations(vtkFloatArray* params,int numclasses,int domeans,int doprint)
{
  int nc=this->NumberOfChannels;
  float* maxsigma =new float[nc];
  float totalnp=0.0;
  
  for (int cl2=0;cl2<numclasses;cl2++)
    totalnp+=params->GetComponent(cl2,0);

  for (int channel=0;channel<this->NumberOfChannels;channel++)
    {
      int b0=this->ChannelMean(channel);
      int b1=this->ChannelSigma(channel);
      
      maxsigma[channel]=0.0;
      for (int cl=0;cl<numclasses;cl++)
	{
	  float np=params->GetComponent(cl,0);

	  float meanden =params->GetComponent(cl,b0);
	  if (isnan(meanden))
	    meanden=1.0;

	  if (domeans)
	    meanden=meanden/np;

	  float mean2den =params->GetComponent(cl,b1)/np;
	  if (isnan(mean2den))
	    mean2den=2.0;
	  
	  float sigmaden= sqrt( mean2den-meanden*meanden);
	  if (isnan(sigmaden))
	    sigmaden=0.1*meanden;

	  params->SetComponent(cl,b0,meanden);
	  params->SetComponent(cl,b1,sigmaden);
      
	  if (sigmaden>maxsigma[channel])
	    maxsigma[channel]=sigmaden;
	}
      maxsigma[channel]=maxsigma[channel]*this->MaxSigmaRatio;
    }

  if (doprint)
    {
      fprintf(stderr,"\t\t Composition\n");
      for (int cl=0;cl<numclasses;cl++)
	{
	  if (cl==numclasses-1 && this->OutlierDetection==1)
	    fprintf(stderr,"+++++++++++++++++++++++++++++ Outlier\t %8.0f \t %5.2f  \n",params->GetComponent(cl,0),100.0*params->GetComponent(cl,0)/float(totalnp));
	  else
	    fprintf(stderr,"+++++++++++++++++++++++++++++ %d\t %8.0f \t %5.2f  \n",cl+1,params->GetComponent(cl,0),100.0*params->GetComponent(cl,0)/float(totalnp));
	}
    }

  if (doprint)
    fprintf(stderr,"\n\n# Data Printing\n Channel \t  Mean, Sigma      \t Mean, Sigma     \t Mean, Sigma     \n");

  for (int channel=0;channel<this->NumberOfChannels;channel++)
    {
      fprintf(stderr,"%6.2f \t ",this->GetChannelEchoTime(channel));
      for (int cl=0;cl<numclasses;cl++)
	{
      	  int b0=this->ChannelMean(channel);
	  int b1=this->ChannelSigma(channel);
	  
	  if (params->GetComponent(cl,b1)<maxsigma[channel])
	    {
	      float old=params->GetComponent(cl,b1);
	      params->SetComponent(cl,b1,maxsigma[channel]);
	    }

	  if (doprint)
	    fprintf(stderr,"%6.2f, %6.2f \t ",params->GetComponent(cl,b0),params->GetComponent(cl,b1));
	}
      
      if (doprint)
	fprintf(stderr,"%.2f \n",maxsigma[channel]);
    }

  return;
}
// -------------------------------------------------------------------------------------------------------------------------------
void vtkpxMRFSegmentation::DoExpectationStep(vtkImageData* label_image,
					     vtkImageData* fitted_image,
					     vtkImageData* fit_quality,
					     vtkImageData* bias_field,
					     float partial_threshold,
					     vtkFloatArray* params,
					     int numclasses)
{

  if (this->MRFBasedFitQuality)
    this->UpdateFitQualityMapFromMRF(label_image,fit_quality,this->MRFThreshold);

  this->ComputeClassSums(label_image,fitted_image,fit_quality,bias_field,partial_threshold,params,numclasses);

  int domeans=1;
  if (this->FitModelToSums(params,numclasses)==1)
    domeans=0;
  this->ComputeClassMeansAndStandardDeviations(params,numclasses,domeans,1);
}

// ----------------------------------------------------------------------------
//
// M - Step
//
// ---------------------------------------------------------------------------
void vtkpxMRFSegmentation::ComputeInitialBiasField(vtkImageData* clean_image,vtkImageData* cleanLabels,vtkImageData* fit_quality,
						   vtkImageData* bias_field_image,vtkImageData* weight_image,
						   vtkFloatArray* params)
{
  fprintf(stderr,"\t\t ******* Computing Initial Bias Field .... \n");
  vtkDataArray* image_data=clean_image->GetPointData()->GetScalars();
  vtkDataArray* label_data=cleanLabels->GetPointData()->GetScalars();
  vtkDataArray* fitq_data =fit_quality->GetPointData()->GetScalars();
  vtkDataArray* bfield=bias_field_image->GetPointData()->GetScalars();
  vtkDataArray* weights=weight_image->GetPointData()->GetScalars();

  bfield->FillComponent(0,0.0);
  weights->FillComponent(0,0.0);
  

  int nt=image_data->GetNumberOfTuples();
  int numc=this->NumberOfClasses;
  for (int voxel_index=0;voxel_index<nt;voxel_index++)
    {
      if (fitq_data->GetComponent(voxel_index,0)>0)
	{
	  int   bestcl=0;
	  float bestprob=label_data->GetComponent(voxel_index,0);
	  for (int i=1;i<numc;i++)
	    {
	      float newl=label_data->GetComponent(voxel_index,i);
	      if (newl>bestprob)
		{
		  bestprob=newl;
		  bestcl=i;
		}
	    }
	  
	  if (bestcl>0)
	    {
	      float sxx=0.0,sxy=0.0;
	      for (int channel=0;channel<this->NumberOfChannels;channel++)
		{
		  int b0=this->ChannelMean(channel);
		  float v1=params->GetComponent(bestcl,b0);
		  float v2=image_data->GetComponent(voxel_index,channel);
		  
		  sxx+=v1*v1;
		  sxy+=v2*v1;
		}
	      float r=sxy/sxx;
	      if (r<0.5)
		r=0.5;
	      else if (r>2.0)
		r=2.0;
	      bfield->SetComponent(voxel_index,0,r*1.0);
	      weights->SetComponent(voxel_index,0,1.0);
	    }
	}
    }
}

void vtkpxMRFSegmentation::ComputeSmoothBiasField(vtkImageData* bias_field,vtkImageData* fit_quality,float Sigma)
{
  Sigma=fabs(Sigma);
  if (Sigma<1.0)
    Sigma=1.0;
  else if (Sigma>5.0)
    Sigma=5.0;

  fprintf(stderr,"\t\t ******* Computing Smooth Bias Field .... %.2f",Sigma);

  int offset=1;//int(Sigma/2);

  double sp[3],sigma[3]; bias_field->GetSpacing(sp);
  Sigma=Sigma*sp[0];



  for (int i=0;i<=2;i++)
    sigma[i]=Sigma/sp[i];

  vtkImageGaussianSmooth* sm[2];
  int nvox[2];

  for (int i=0;i<=1;i++)
    {
      sm[i]=vtkImageGaussianSmooth::New();
      if (i==0)
	sm[i]->SetInput(bias_field);
      else
	sm[i]->SetInput(fit_quality);
      sm[i]->SetStandardDeviations(sigma);
      sm[i]->Update();
      nvox[i]=sm[i]->GetOutput()->GetPointData()->GetScalars()->GetNumberOfTuples();
    }



  int nt=bias_field->GetPointData()->GetScalars()->GetNumberOfTuples();


  if (nvox[0]==nt && nvox[1]==nt)
    {
      vtkDataArray* numerator=sm[0]->GetOutput()->GetPointData()->GetScalars();
      vtkDataArray* denominator=sm[1]->GetOutput()->GetPointData()->GetScalars();
      vtkDataArray* bfield=bias_field->GetPointData()->GetScalars();

      bfield->FillComponent(0,1.0);

      int dim[3]; bias_field->GetDimensions(dim);
      int slicesize=dim[0]*dim[1];
      float sum=0.0,nsum=0.0;


      for (int k=offset;k<dim[2]-offset;k++)
	for (int j=offset;j<dim[1]-offset;j++)
	  for (int i=offset;i<dim[0]-offset;i++)
	    {
	      int indx=i+j*dim[0]+k*slicesize;
	      double y=numerator->GetComponent(indx,0);
	      double x=denominator->GetComponent(indx,0);
	      
	      if (x>0.1)
		{
		  float r=y/x;
		  if (r<0.667)
		    r=0.667;
		  else if (r>1.5)
		    r=1.5;
		  bfield->SetComponent(indx,0,r);
		}
	      
	      sum+=bfield->GetComponent(indx,0);
	      nsum+=1.0;
	    }
      
      float factor=nsum/sum;
      fprintf(stderr,"Factor=%.2f\n",factor);
      /*	      for (int i=0;i<nt;i++)
	      {
	      double b=bfield->GetComponent(i,0)*factor;
	      bfield->SetComponent(i,0,b);
	      }*/
      
      fprintf(stderr,"\t\t  Done Computing Smooth Bias Field\n");
    }
  else
    {
      fprintf(stderr,"Error Not Computing Smooth Bias Field %d,%d,%d\n",nt,nvox[0],nvox[1]);
    }

  sm[1]->Delete();
  sm[0]->Delete();




}
// ----------------------------------------------------------------------------

void vtkpxMRFSegmentation::TensorFitBiasFieldImage(vtkImageData* bias_field,vtkImageData* weight_image)
{
  fprintf(stderr,"\t\t ****** Fitting Tensor Spline .... \n");
  if (this->TensorSpline!=NULL)
    this->TensorSpline->Delete();
  this->TensorSpline=vtkpxImageTensorSpline::New();
  this->TensorSpline->SetInput(bias_field);
  this->TensorSpline->SetWeightImage(weight_image);

  double sp[3]; bias_field->GetSpacing(sp);
  int   dim[3]; bias_field->GetDimensions(dim);

  float spacing=dim[0]*sp[0]/this->BiasFieldFactor;
  
  this->TensorSpline->SetControlPointSpacing(spacing);
  this->TensorSpline->SetResolution(2.0);
  this->TensorSpline->SetAutoInitialize(1);
  this->TensorSpline->Run();
  bias_field->ShallowCopy(this->TensorSpline->GetOutput());

  vtkDataArray* bfield=bias_field->GetPointData()->GetScalars();
  int nt=bfield->GetNumberOfTuples();
  for (int i=0;i<nt;i++)
    {
      float v=bfield->GetComponent(i,0);
      if (v<0.667)
	bfield->SetComponent(i,0,0.667);
      else if (v>1.5)
	bfield->SetComponent(i,0,1.5);
    }

  fprintf(stderr,"\t\t ****** Fitting Tensor Spline .... Done\n");

}
// ----------------------------------------------------------------------------
// Compute Individual Probability Components 
// ----------------------------------------------------------------------------

int vtkpxMRFSegmentation::DebugPrint(int vox)
{
  if (vox==0)
    return 1;


  if (this->DebugVoxel==0 || vox==0)
    return 0;

  if (vox - this->DebugVoxel == 0)
    return 1;

  return 0;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

double vtkpxMRFSegmentation::ComputeLogLikelihoodProbability(vtkDataArray* image_data,int vox,float* theta,float noise_sigma,float bfield,float& newbfield,
							     int debug)
{
  double totalprob=0.0;
  float r=1.0;
  /*  if (this->ComputeBiasField)
    {
      float sxx=0.0,sxy=0.0;
      for (int channel=0;channel<this->NumberOfChannels;channel++)
	{
	  int b0=this->ChannelMean(channel);
	  float v1=theta[b0];
	  float v2=image_data->GetComponent(vox,channel);
	  
	  sxx+=v1*v1;
	  sxy+=v2*v1;
	}
      r=sxy/sxx;
      
      float r1=(r-bfield);

      if (r1<-this->BiasFieldSigma)
	r1=bfield+this->BiasFieldSigma;
      else if (r1>this->BiasFieldSigma)
	r1=bfield-this->BiasFieldSigma;
      newbfield=r1;
    }
    else*/
  newbfield=bfield;
	
  for (int channel=0;channel<this->NumberOfChannels;channel++)
    {
      int b0=this->ChannelMean(channel);
      int b1=this->ChannelSigma(channel);
            
      float mean=theta[b0];
      float variance=theta[b1]*theta[b1]+pow(noise_sigma/newbfield,float(2.0));
      totalprob+=log(vtkpxHistogramSegmentation::Metric(image_data->GetComponent(vox,channel)/newbfield,mean,variance));
    }

  if (debug)
    fprintf(stderr,"\n\t PROB(%d)::   -logp=%f\n",    vox,-(totalprob));
  
  return totalprob;
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
double vtkpxMRFSegmentation::ComputeMRFPrior(vtkDataArray* labels,int vox,float* current_labels,int incr[6],float wgth[6],int debug)
{
  if (debug)
    fprintf(stderr,"\t MRF [ g=");

  double sum=0.0;
  int numclasses=labels->GetNumberOfComponents();
  for (int c=0;c<numclasses;c++)
    {
      float g=0.0;
      for (int i=0;i<=5;i++)
	{
	  if (wgth[i]>0.0001)
	    g+=wgth[i]*labels->GetComponent(vox+incr[i],c);
	}

      if (debug)
	fprintf(stderr,"%d(%5.3f - %5.3f),",c+1,current_labels[c],g);
      
      sum+=(pow(current_labels[c]-g,float(2.0)));
    }

  if (debug)
    fprintf(stderr,"] sum=%.2f\n ",sum);

  return sum;
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
double vtkpxMRFSegmentation::ComputeTotalMinusLogProbability(vtkDataArray* image_data,
							    vtkDataArray* labels,
							     int voxel_index,
							     float *currentlab,
							     float* theta,
							     float noise_sigma,
							     float bfield,
							     int incr[6],float wgth[6],
							     float smoothness,
							     int debug)
{
  float newbfield=0.0;
  double mlterm=this->ComputeLogLikelihoodProbability(image_data,voxel_index,theta,noise_sigma,bfield,newbfield,debug);
  double pmrf  =0.0;
  if (smoothness>0.001)
    pmrf=smoothness*this->ComputeMRFPrior(labels,voxel_index,currentlab,incr,wgth,debug);
  /*  double bterm =0.0;
  if (this->ComputeBiasField)
  bterm=log(vtkpxHistogramSegmentation::Metric(newbfield,bfield,this->BiasFieldSigma));*/
  double t=-mlterm+pmrf;

  if (debug)
    fprintf(stderr,"\t Total Minus Log Probability=%.2f\n ",t);

  return t;
}


double vtkpxMRFSegmentation::ComputeTotalMinusLogProbabilityOutlier(vtkDataArray* labels,
								   int voxel_index,
								   float *currentlab,
								   float* theta,
								   int incr[6],float wgth[6],float smoothness,int debug)
{
  double mlterm=this->OutlierProbability;
  double pmrf  =this->ComputeMRFPrior(labels,voxel_index,currentlab,incr,wgth,debug);
  double t=-log(mlterm)+smoothness*pmrf;

  if (debug)
    fprintf(stderr,"\t Total Minus Log Probability=%.2f\n ",t);

  return t;

}
// --------------------------------------------------------------------------------------------
int vtkpxMRFSegmentation::DoHardClassification(int voxel_index,int actual_num_classes,float prob[20],float& bestprob,
						 float new_labels[20])
						 
{
  int bestclass=0;
  bestprob=0.0;

  float biasfieldvalue=1.0;
  if (OptimizationStructure.biasfield!=NULL)
    biasfieldvalue=OptimizationStructure.biasfield->GetComponent(voxel_index,0);

  for (int cl=0;cl< actual_num_classes;cl++)
    {
      for (int cl2=0;cl2< actual_num_classes;cl2++)
	new_labels[cl2]=0.0;


      float theta[200];
      if ( this->DebugPrint(voxel_index))
	fprintf(stderr,"\n\n ------------ Beginning Class %d\n++++ Prior(%d):",
		cl+1,OptimizationStructure.voxel_index);
      
      this->CombineMeansAndStandardDeviations(OptimizationStructure.params,cl,cl,1.0,theta,this->DebugPrint(voxel_index));
      OptimizationStructure.SetIndex(voxel_index,this->DebugPrint(voxel_index));

      OptimizationStructure.currentclass[0]=cl;
      OptimizationStructure.currentclass[1]=cl;
      OptimizationStructure.currentfraction=1.0;
      OptimizationStructure.numberofcomponentclasses=1;
      new_labels[cl]=1.0;
      
      if (this->OutlierDetection==1 && cl==actual_num_classes-1)
	prob[cl]=this->ComputeTotalMinusLogProbabilityOutlier(OptimizationStructure.labels,
							      OptimizationStructure.voxel_index,
							      &new_labels[0],
							      &theta[0],
							      OptimizationStructure.incr,
							      OptimizationStructure.wgth,
							      OptimizationStructure.smoothness,
							      OptimizationStructure.debug);
      else
	prob[cl]=this->ComputeTotalMinusLogProbability(OptimizationStructure.image_data,
						       OptimizationStructure.labels,
						       OptimizationStructure.voxel_index,
						       &new_labels[0],
						       &theta[0],
						       OptimizationStructure.noisesigma,
						       biasfieldvalue,
						       OptimizationStructure.incr,
						       OptimizationStructure.wgth,
						       OptimizationStructure.smoothness,
						       OptimizationStructure.debug);
      if (cl==0 || prob[cl]<bestprob)
	{
	  bestprob=prob[cl];
	  bestclass=cl;
  	}
      
    }




  return bestclass;
}
// --------------------------------------------------------------------------------------------
int vtkpxMRFSegmentation::DoFuzzyClassification(int voxel_index,int actual_num_classes,float prob[20],float& bestprob,
						float new_labels[20])
{
  
  if (this->DebugPrint(voxel_index))
    fprintf(stderr,"\n\n ffff Fuzzy Classification \n");
  
  float biasfieldvalue=1.0;
  if (OptimizationStructure.biasfield!=NULL)
    biasfieldvalue=OptimizationStructure.biasfield->GetComponent(voxel_index,0);

  // 1. Sort and find best 2 classes for this voxel
  // ----------------------------------------------
  int ignore=-1,rank[2],bestindex=0;
  float best=prob[0];
  
  for (int outer=0;outer<=1;outer++)
    {
      for (int cl=0;cl< this->NumberOfClasses;cl++)
	{
	  if (prob[cl]<best && cl!=ignore)
	    {
	      best=prob[cl];
	      bestindex=cl;
	      
	    }
	}
      rank[outer]=bestindex;
      
      if (outer==0)
	{
	  ignore=bestindex;
	  if (bestindex==0)
	    bestindex=1;
	  else
	    bestindex=0;
	  best=prob[bestindex];
	}
    }
  
  
  // 2. Optimize Partial
  // -------------------
  bestprob=prob[rank[0]];
  float secondbestprob=prob[rank[1]];
  float bestpartial=0.0;
  float secondbestpartial=1.0;
  
  if (this->DebugPrint(voxel_index))
    fprintf(stderr,"\t\t BestClasses %d (%.2f) and  %d (%.2f) optimal=(%.2f,%.2f)\n",rank[0]+1,prob[rank[0]],rank[1]+1,prob[rank[1]],bestpartial,bestprob);
  
  OptimizationStructure.SetIndex(voxel_index,this->DebugPrint(voxel_index));
  
  int maxattempts=4;
  
  for (int attempt=0;attempt<maxattempts;attempt++)
    {
      float partial=0.5*(bestpartial+secondbestpartial);
      
      float fraction[2];
      float theta[200];
      fraction[0]=1.0-partial;
      fraction[1]=partial;
      
      for (int cl2=0;cl2< this->NumberOfClasses;cl2++)
	new_labels[cl2]=0.0;
      new_labels[rank[0]]=fraction[0];
      new_labels[rank[1]]=fraction[1];
      
      if (this->DebugPrint(voxel_index))
	{
	  fprintf(stderr,"\n\t\t Partial Prob %.3f of %d and %.3f of %d\n",fraction[0],rank[0]+1,fraction[1],rank[1]+1);
	  fprintf(stderr,"\t\t\t bestpartial %.3f (%.3f) secondbest %.3f (%.3f) \n",bestpartial,bestprob,secondbestpartial,secondbestprob);
	}
      
      this->CombineMeansAndStandardDeviations(OptimizationStructure.params,rank[0],rank[1],fraction[0],theta,
					      this->DebugPrint(voxel_index));
      float partial_prob=this->ComputeTotalMinusLogProbability(OptimizationStructure.image_data,
							       OptimizationStructure.labels,
							       OptimizationStructure.voxel_index,
							       &new_labels[0],
							       &theta[0],
							       OptimizationStructure.noisesigma,
							       biasfieldvalue,
							       OptimizationStructure.incr,
							       OptimizationStructure.wgth,
							       OptimizationStructure.smoothness,
							       OptimizationStructure.debug);
      
      if (partial_prob < secondbestprob && partial_prob > bestprob)
	{
	  secondbestprob=partial_prob;
	  secondbestpartial=partial;
	}
      else if (partial_prob < bestprob )
	{
	  secondbestprob=bestprob;
	  secondbestpartial=bestpartial;
	  bestprob=partial_prob;
	  bestpartial=partial;
	}
      else
	{
	  attempt=maxattempts;
	}
    }
  
  new_labels[rank[0]]=1.0-bestpartial;
  new_labels[rank[1]]=bestpartial;
  
  if ( this->DebugPrint(voxel_index))
    fprintf(stderr,"\n **** Best Fraction =%.3f , bestprob=%.2f (%.3f,%.3f)\n",bestpartial,bestprob,new_labels[rank[0]],new_labels[rank[1]]);

  return rank[1];
  
}
// --------------------------------------------------------------------------------------------
float  vtkpxMRFSegmentation::DoVoxelFit(int voxel_index)
{
  int actual_num_classes=this->NumberOfClasses+this->OutlierDetection;

  // 1. Examine probability for all pure voxels   (use meanr2,mean density for each class i.e. do not reoptimize here)
  // ----------------------------------------------

  float current_labels[20],new_labels[20];
  for (int c=0;c<actual_num_classes;c++)
    current_labels[c]=OptimizationStructure.labels->GetComponent(voxel_index,c);

  float prob[20],bestprob=0.0;
  int bestclass=this->DoHardClassification(voxel_index,actual_num_classes,prob,bestprob,new_labels);


  
  if ( this->DebugPrint(voxel_index))
    fprintf(stderr,"\n **** New_Labels:");

  for (int cl=0;cl< actual_num_classes;cl++)
    {
      if (cl!=bestclass)
	new_labels[cl]=0.0;
      else
	new_labels[cl]=1.0;
      
      if ( this->DebugPrint(voxel_index))
	fprintf(stderr,"\t [%d]=%.2f (%.2f)",cl+1,new_labels[cl],prob[cl]);
    }
  
  if ( this->DebugPrint(voxel_index))
    fprintf(stderr,"\n");

  if (this->PerformingFuzzyClassification==1 && this->OutlierDetection==0)
    {
      this->DoFuzzyClassification(voxel_index,actual_num_classes,prob,bestprob,new_labels);
    }
  
  float changeinlab=0.0;
  if ( this->DebugPrint(voxel_index))
    fprintf(stderr," **** New_Fuzzy_Labels:");

  for (int cl=0;cl< this->NumberOfClasses;cl++)
    {
      changeinlab+=fabs(new_labels[cl]-current_labels[cl]);
      OptimizationStructure.labels->SetComponent(voxel_index,cl,new_labels[cl]);
      if ( this->DebugPrint(voxel_index))
	fprintf(stderr,"\t [%d]=%.2f, ",cl+1,new_labels[cl]);
    }

  if ( this->DebugPrint(voxel_index))
    fprintf(stderr,"change =%.2f\n",changeinlab);
  return changeinlab;

}
// ----------------------------------------------------------------------------------------------------
//  High Level Routines
// ----------------------------------------------------------------------------------------------------
int  vtkpxMRFSegmentation::UpdateVoxel(int eveni,int evenj,int evenk,int pass)
{
  int update_voxel=0;
  if (eveni == evenj) update_voxel=1;
  if (evenk>=0)
    {
      if (evenk)   	  
	update_voxel=1-update_voxel;
    }
  if (pass==1) 	  update_voxel=1-update_voxel;
  
  return update_voxel;
}
// ----------------------------------------------------------------------------
int  vtkpxMRFSegmentation::ComputeMRFIncrementsAndWeights(vtkImageData* img,int incr[6],float wgt[6])
{
  int dim[3]; img->GetDimensions(dim);
  double sp[3];img->GetSpacing(sp);

  if (dim[2]>1)
    {
      incr[5]=dim[0]*dim[1];   wgt[5]=1.0/sp[2];
      incr[4]=-incr[5];        wgt[4]=1.0/sp[2];
    }
  else
    {
      incr[5]=0;  incr[4]=0;
      wgt[5]=0.0; wgt[4]=0.0;
    }

  incr[3]=dim[0];          wgt[3]=1.0/sp[1];
  incr[2]=-incr[3];        wgt[2]=1.0/sp[1];
  incr[1]=1;               wgt[1]=1.0/sp[0];
  incr[0]=-incr[1];        wgt[0]=1.0/sp[0];



  float sum=0.0;
  for (int l=0;l<=5;l++)
    sum+=wgt[l];

  for (int k=0;k<=5;k++)
    wgt[k]/=sum;
  
    
  return 1;
}
// ----------------------------------------------------------------------------
float vtkpxMRFSegmentation::DoMaximizationStep(vtkImageData* label_image,vtkImageData* intensity_image,vtkImageData* bias_field,
					       float noise_sigma,
					       vtkFloatArray* params,int numclasses,
					       float smoothness,int maxiter)
{
  vtkDataArray* intensities=intensity_image->GetPointData()->GetScalars();
  vtkDataArray* labels=label_image->GetPointData()->GetScalars();
  vtkDataArray* bfield=bias_field->GetPointData()->GetScalars();

  fprintf(stderr,"M-Step Number of Components intensity=%d labels=%d channels=%d\n",intensities->GetNumberOfComponents(),labels->GetNumberOfComponents(),
	  this->NumberOfChannels);

  int nt=intensities->GetNumberOfTuples();

  int dim[3]; intensity_image->GetDimensions(dim);

  int incr[6]; float weights[6]; this->ComputeMRFIncrementsAndWeights(intensity_image,incr,weights);

  int done=0,iter=0;

  int tenth=nt/11;


  OptimizationStructure.SetBaseValues(intensities,
				      labels,
				      bfield,
				      params,
				      &incr[0],
				      &weights[0],
				      smoothness,
				      noise_sigma);
  
  int mink=1,maxk=dim[2]-1;
  if (dim[2]==1)
    {
      mink=0;
      maxk=1;
    }

  float sumchanged=0.0;
  if (smoothness<0.001)
    maxiter=1;

  while (done==0 && iter<maxiter)
    {
      float total=0.0;
      float changed=0.0;
      
      int order=(vtkMath::Random()>0.5);
  
      fprintf(stderr,"\t **** MRF Fuzzy=%d Regularization (%.2f) iter=%d :",this->PerformingFuzzyClassification,smoothness,iter+1);
      int count=0;

      for (int pass=0;pass<=1;pass++)
	{
	  int realpass=pass;
	  if (order==1)
	    realpass=1-pass;

	  for (int k=mink;k<maxk;k++)
	    {
	      int evenk= ( ( k % 2 )  == 0);
	      if (maxk==1)
		evenk=-1;

	      for (int j=1;j<dim[1]-1;j++)
		{
		  int evenj= ( ( j % 2 ) == 0 );
		  int vox=k*incr[5]+j*incr[3]+1;
		  for (int i=1;i<dim[0]-1;i++)
		    {
		      int eveni= ( ( i % 2 ) == 0 );
		      ++count;
		      if (count==tenth)
			{
			  fprintf(stderr,".");
			  count=0;
			}

		      // ----------------------------
		      // Updating Voxel
		      // ----------------------------
		      
		      if ( this->UpdateVoxel(eveni,evenj,evenk,pass)==1)
			{
			  changed+=this->DoVoxelFit(vox);

			  for (int cl=0;cl<numclasses;cl++)
			    {
			      labels->SetComponent(vox,cl,OptimizationStructure.labels->GetComponent(vox,cl));
			      total+=1.0;
			    }
			  

			}
		      ++vox;
		    }
		}
	    }
	}
         
      changed=100.0*changed/total;

      sumchanged+=changed;

      fprintf(stderr,"changed=%.2f\n\n",changed);
      if (changed<this->MRFConvergencePercentage)
	done=1;
      ++iter;


    }
  return sumchanged;
}
// ----------------------------------------------------------------------------
//
//     Execute Segmentation 
//
// ----------------------------------------------------------------------------
void vtkpxMRFSegmentation::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *input=this->GetImageDataInput(0);
  vtkImageData *output=this->GetOutput();

  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxMRFSegmentation");
      return;
    }

  output->SetScalarTypeToShort();
  output->SetNumberOfScalarComponents(1);
}
// ----------------------------------------------------------------------------
//
//         Helper Routines
//
// ----------------------------------------------------------------------------
void vtkpxMRFSegmentation::GenerateLabelOutputs(vtkImageData* cleanLabels,vtkImageData* output)
{
  vtkImageShiftScale* sc=vtkImageShiftScale::New();
  sc->SetScale(100.0);
  sc->SetOutputScalarTypeToShort();
  sc->SetShift(0.0);
  sc->SetInput(cleanLabels);
  sc->Update();
  
  if (this->MultiFrameOutput!=NULL)
    this->MultiFrameOutput->Delete();
  this->MultiFrameOutput=vtkImageData::New();
  this->MultiFrameOutput->ShallowCopy(sc->GetOutput());
  sc->Delete();
  
  vtkDataArray* complex=this->MultiFrameOutput->GetPointData()->GetScalars();
  vtkDataArray*     out=output->GetPointData()->GetScalars();

  int np=out->GetNumberOfTuples();
  int np2=complex->GetNumberOfComponents();
  for (int i=0;i<np;i++)
    {
      int best=0;
      float maxv=complex->GetComponent(i,0);
      for (int j=1;j<np2;j++)
	{
	  float v=complex->GetComponent(i,j);
	  if (v>maxv)
	    {
	      maxv=v;
	      best=j;
	    }
	}
      out->SetComponent(i,0,best);
    }
}
// ----------------------------------------------------------------------------
void vtkpxMRFSegmentation::Optimization(vtkImageData* cleanLabels,vtkImageData* cleanInput,
					vtkImageData* fitted_image,vtkImageData* fit_quality,
					vtkImageData* bias_field,
					float noise_sigma)
{
  int iter=1,done=0,countsmooth=1;
  this->PerformingFuzzyClassification=0;
  while (iter<=this->MRFIterations && done==0)
    {
      float smooth=this->Smoothness;//float(iter)*this->Smoothness/(0.5*float(this->MRFIterations));
      fprintf(stderr,"\n\n---------------------------------------------------------------------------------------------------\n Beginning Global Iteration %d/%d  \n\n",iter,this->MRFIterations);
      if (smooth>=this->Smoothness)
	smooth=this->Smoothness;
      
      if (this->ComputeBiasField)
	this->BiasField->GetPointData()->GetScalars()->FillComponent(0,1.0);
	  
      float changed=this->DoMaximizationStep(cleanLabels,cleanInput,bias_field,
					     noise_sigma,
						 this->Parameters,this->NumberOfClasses+this->OutlierDetection,smooth,5);
      
      
      if (this->ComputeBiasField && countsmooth==4 )
	{
	  this->ComputeInitialBiasField(cleanInput,cleanLabels,fit_quality,bias_field,this->BiasFieldWeights,this->Parameters);
	  
	  vtkpxUtil::SaveAnalyze("initial_field.hdr",this->BiasField,0);
	  
	  if (this->BiasFieldFactor>0.0)
	    this->TensorFitBiasFieldImage(bias_field,this->BiasFieldWeights);
	  else
	    this->ComputeSmoothBiasField(bias_field,this->BiasFieldWeights,this->BiasFieldFactor);
	  
	  vtkpxUtil::SaveAnalyze("smooth_field.hdr",this->BiasField,0);
	  changed=100.0;
	  countsmooth=0;
	}
      
      this->DoExpectationStep(cleanLabels,
			      fitted_image,fit_quality,bias_field,
			      0.75,this->Parameters,this->NumberOfClasses+this->OutlierDetection);
      
      ++iter;
      ++countsmooth;
      this->UpdateProgress(float(iter)/float(this->MRFIterations));
      
      if (changed<this->MRFConvergencePercentage)
	done=1;
    }

  // Fuzzy Classification if needed .. no additional bias field or expectation steps from here on
  // --------------------------------------------------------------------------------------------
  if (this->FuzzyClassification)
    {
      fprintf(stderr,"On to Fuzzy Classification Optimization %.2f\n",this->ImageNoiseSigma);
      this->FuzzyClassificationOptimization(cleanLabels,cleanInput,fitted_image,fit_quality,bias_field,noise_sigma);
    }

}
// ----------------------------------------------------------------------------
void vtkpxMRFSegmentation::FuzzyClassificationOptimization(vtkImageData* cleanLabels,vtkImageData* cleanInput,
							   vtkImageData* fitted_image,vtkImageData* fit_quality,
							   vtkImageData* bias_field,
							   float noise_sigma)
{
  int iter=1,done=0;
  this->PerformingFuzzyClassification=1;
  while (iter<=this->MRFIterations && done==0)
    {
      float smooth=float(iter)*this->Smoothness/(float(this->MRFIterations));
      fprintf(stderr,"\n\n---------------------------------------------------------------------------------------------------\n Beginning Fuzzy Iteration %d/%d  \n\n",iter,this->MRFIterations);
      if (smooth>=this->Smoothness)
	smooth=this->Smoothness;

      float changed=this->DoMaximizationStep(cleanLabels,cleanInput,bias_field,noise_sigma,
					     this->Parameters,this->NumberOfClasses+this->OutlierDetection,smooth,5);

      ++iter;
      this->UpdateProgress(float(iter)/float(this->MRFIterations));
      if (changed<0.50*this->MRFConvergencePercentage)
	done=1;
    }


}
// ----------------------------------------------------------------------------
void vtkpxMRFSegmentation::GenerateInitialCleanLabelImageFromFirstFrame(vtkImageData* tmpLabels,vtkImageData* cleanLabels)
{
  vtkDataArray* lab=cleanLabels->GetPointData()->GetScalars();
  vtkDataArray* tmp=tmpLabels->GetPointData()->GetScalars();
  int  nt=lab->GetNumberOfTuples();
  int  nc=lab->GetNumberOfComponents();

  
  for (int i=0;i<nt;i++)
    {
      int c=(int)tmp->GetComponent(i,0);
      for (int j=0;j<nc;j++)
	{
	  if (j==c)
	    lab->SetComponent(i,j,1.0);
	  else
	    lab->SetComponent(i,j,0.0);
	}
    }

}

// ----------------------------------------------------------------------------
void vtkpxMRFSegmentation::GenerateFittedParametersAndCleanInput(vtkImageData* input,vtkImageData* cleanInput,
								 vtkImageData* fitted_image,vtkImageData* fit_quality)
{

  cleanInput->ShallowCopy(input);
  fitted_image->ShallowCopy(cleanInput);

  if (this->FitQualityMap!=NULL &&   this->MRFBasedFitQuality==0)
    fit_quality->ShallowCopy(this->FitQualityMap);
  else
    {
      fit_quality->CopyStructure(cleanInput);
      fit_quality->SetNumberOfScalarComponents(1);
      fit_quality->SetScalarTypeToUnsignedChar();
      fit_quality->AllocateScalars();
      fit_quality->GetPointData()->GetScalars()->FillComponent(0,1.0);
    }


}
// ----------------------------------------------------------------------------
vtkImageData* vtkpxMRFSegmentation::CreateInitialLabels(vtkImageData* output)
{
  vtkImageData* cleanLabels=vtkImageData::New();
  cleanLabels->CopyStructure(output);
  cleanLabels->SetScalarTypeToFloat();
  cleanLabels->SetNumberOfScalarComponents(this->NumberOfClasses+this->OutlierDetection);
  cleanLabels->AllocateScalars();
  
  vtkDataArray* dat=cleanLabels->GetPointData()->GetScalars();
  int nc=dat->GetNumberOfComponents();
  float v=1.0/nc;
  for (int i=0;i<nc;i++)
    dat->FillComponent(i,v);
  return cleanLabels;

}
// ----------------------------------------------------------------------------
int vtkpxMRFSegmentation::CheckInitialParameters(vtkImageData* input)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxMRFSegmentation");
      return -1;
    }

  fprintf(stderr,"Number Of Classes = %d\n",this->NumberOfClasses);
  if (this->Parameters==NULL)
    this->Parameters=vtkFloatArray::New();

  if (this->OutlierDetection)
    this->FuzzyClassification=0;

  return 1;
}
// ----------------------------------------------------------------------------
vtkImageData* vtkpxMRFSegmentation::CreateSingleChannel(vtkImageData* input)
{
  vtkImageData* single=vtkImageData::New();
  int nt=input->GetNumberOfScalarComponents();

  if (nt==1)
    {
      single->DeepCopy(input);
      this->SingleChannel=1;
    }
  else
    {
      if (this->KeyChannel<0)
	this->KeyChannel=0;
      else if (this->KeyChannel>=nt)
	this->KeyChannel=nt-1;
 
      fprintf(stderr,"Creating Single Channel: Using Key Channel=%d\n",this->KeyChannel);
     
      vtkImageExtractComponents* extr=vtkImageExtractComponents::New();
      extr->SetInput(input);
      extr->SetComponents(this->KeyChannel);
      extr->Update();
      
      single->ShallowCopy(extr->GetOutput());
      extr->Delete();
    }

  if (this->HistogramNormalization)
    HistogramNormalizeImage(single,0.01,0.99);
  
  return single;
}
// ----------------------------------------------------------------------------
void vtkpxMRFSegmentation::InitializeSegmentation(vtkImageData* singleChannel,vtkImageData* fitImage,
						  vtkImageData* fitQuality,vtkImageData* cleanLabels)
{
  this->GenerateLabels=1;
  
  vtkImageData* tmp_labels=vtkImageData::New();
  this->HistogramSegmentation(singleChannel,tmp_labels,this->Parameters,0,0);
  this->GenerateInitialCleanLabelImageFromFirstFrame(tmp_labels,cleanLabels);


  if (this->SingleChannel!=1)
    {
      // Force an Expectation Step at this Point
      int nt=this->Parameters->GetNumberOfTuples();
      int nc=2*fitImage->GetNumberOfScalarComponents()+1;
      this->Parameters->SetNumberOfComponents(nc);
      this->Parameters->SetNumberOfTuples(nt);
      fprintf(stderr,"Doing Expectation Step\n\n");
      this->DoExpectationStep(cleanLabels,
			      fitImage,fitQuality,NULL,
			      0.75,this->Parameters,this->NumberOfClasses+this->OutlierDetection);
    }

}
// ----------------------------------------------------------------------------
void vtkpxMRFSegmentation::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  int ok=this->CheckInitialParameters(input);
  if (ok==-1)
    return;

  vtkImageData* cleanLabels=this->CreateInitialLabels(output);
  vtkImageData* singleChannel=this->CreateSingleChannel(input);

  vtkImageData* cleanInput=vtkImageData::New();
  vtkImageData* fittedImage=vtkImageData::New();
  vtkImageData* fitQuality =vtkImageData::New();

  if (this->SingleChannel)
    this->GenerateFittedParametersAndCleanInput(singleChannel,cleanInput,fittedImage,fitQuality);
  else
    this->GenerateFittedParametersAndCleanInput(input,cleanInput,fittedImage,fitQuality);

  this->NumberOfChannels=fittedImage->GetNumberOfScalarComponents();

  this->BiasField=vtkImageData::New();
  this->BiasField->CopyStructure(input);
  this->BiasField->SetNumberOfScalarComponents(1);
  this->BiasField->SetScalarTypeToFloat();
  this->BiasField->AllocateScalars();
  this->BiasField->GetPointData()->GetScalars()->FillComponent(0,1.0);

  if (this->ComputeBiasField)
    {
      this->BiasFieldWeights=vtkImageData::New();
      this->BiasFieldWeights->CopyStructure(this->BiasField);
      this->BiasFieldWeights->SetScalarTypeToFloat();
      this->BiasFieldWeights->AllocateScalars();
    }
  
  // Initialization Stuff 
  // --------------------
  fprintf(stderr,"Initializing Segmentation\n");
  this->InitializeSegmentation(singleChannel,fittedImage,fitQuality,cleanLabels);

  
  fprintf(stderr,"On to Update Progress %d\n",this->MRFIterations);
  this->UpdateProgress(float(0.2)/float(this->MRFIterations+1));

  // EM Iterations (or ME) 
  // ---------------------
  fprintf(stderr,"On to Optimization %.2f\n",this->ImageNoiseSigma);
  this->Optimization(cleanLabels,cleanInput,fittedImage,fitQuality,this->BiasField,this->ImageNoiseSigma);


  // Generate Proper Label Output
  // ----------------------------
  this->GenerateLabelOutputs(cleanLabels,output);

  if (this->FitQualityMap==NULL)
    this->SetFitQualityMap(fitQuality);

  cleanInput->Delete();
  cleanLabels->Delete();
  singleChannel->Delete();
  fittedImage->Delete();
  fitQuality->Delete();

  this->UpdateProgress(1.0);
}




