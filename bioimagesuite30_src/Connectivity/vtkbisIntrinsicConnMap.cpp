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

/*
 * vtkbisIntrinsicConnMap.cpp
 *
 *  Created on: Nov 3, 2008
 *      Author: Isabella Murphy; Xilin Shen
 * @Input: 4D vtkImageData object of the whole image and a threshold
 * @output: 3D vtkImageData object with sum of absolute or power value of correlation among
 * each voxel of the image which are above the threshold.
 * User can chose to look at only the positive correlation , only the negative correlation or both.
 * For example, correlation of voxel 0 to each voxel is stored in output vtkImagedata voxel 0 component 0.
 * In the output image, xyz=input dimension, component=1.
 */

#include "vtkbisIntrinsicConnMap.h"
#include "vtkbisImageTimeSeriesNormalize.h"
#include "vtkbisTimeSeriesCorrelation.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkObject.h"
#include "vtkPointData.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include <math.h>
#include <time.h>
#include "pxisinf.h"
#include <fstream>
#include <cstdlib>

using namespace std;

vtkbisIntrinsicConnMap::vtkbisIntrinsicConnMap()
{
  this->Threshold= 0.0;
  this->ImageMask = NULL;
  this->Range = 3;
  this->Abs = 1;
  this->Optimized = 1;
  this->Type = 1;
  this->OutData2=NULL;
 // this->OutputRaw=0;
  this->Mean=0.0;
  this->Std=0.0;
  this->DegreeMean=0.0;
  this->DegreeStd=0.0;
  this->FileName="";

}

vtkbisIntrinsicConnMap::~vtkbisIntrinsicConnMap()
{
  this->SetImageMask(NULL);
  if (this->OutData2!=NULL)
    this->OutData2->Delete();

}
void vtkbisIntrinsicConnMap::SetFileName(const char* filename)
{
  this->FileName=filename;
}

const char* vtkbisIntrinsicConnMap::GetFileName()
{
  return this->FileName.c_str();
}

vtkbisIntrinsicConnMap* vtkbisIntrinsicConnMap::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisIntrinsicConnMap");
  if(ret)
    {
      return (vtkbisIntrinsicConnMap*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisIntrinsicConnMap;
}
//----------------------------------------------------------------------------------------------------------------------
//RequestInformation
//I. Our vtkImageData input has number of components = number of frames but output vtkImageData object has number of
//components = 1.
// ---------------------------------------------------------------------------------------------------------------------------
int vtkbisIntrinsicConnMap::RequestInformation (vtkInformation * request,
						vtkInformationVector ** inputVector,
						vtkInformationVector * outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, 1);//set number of components to 1

  return 1;
}
//--------------------------------------------------------------------------
//templated function, optimized the original code
//--------------------------------------------------------------------------
template <class IT,class OT>
static void vtkbisIntrinsicConnMapDoConnectivity(vtkbisIntrinsicConnMap *self,
						 vtkImageData *inData,
						 vtkImageData *outData,
						 vtkImageData *OutData2,
						 vtkImageData *maskData,
						 IT*, OT*)
{
  //IT *img1 = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
  OT *msk  = NULL;
  //if (maskData!=NULL)
  //  msk=(OT*) maskData->GetPointData()->GetScalars()->GetVoidPointer(0);

  float *count=(float*)OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
  int nt=inData->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc=inData->GetPointData()->GetScalars()->GetNumberOfComponents();
  // Normalize input

  /*  for (int i=0;i< nt;i++)
	  {
		int DoGlm=1;
		if (msk!=NULL)
		{
			if (*msk < 1 )
			  DoGlm=0;
			++msk;
		}
		if(DoGlm)
		{
			double psum=0.0;
			IT* temp=img1;
			for (int m = 0; m < nc; m++)
			  {
				//GetComponent(voxel,m) Return the data component at the ith tuple and mth component
				psum += (*temp)*(*temp);
				++temp;
			  }
			double norm = sqrt(psum);
			if(norm != 0)
			{
				for (int m = 0; m < nc; m++)
				{
					double v= double(*img1)/norm;
					*img1= (IT)v;
					++img1;
				}
			}else{
			for (int m = 0; m < nc; m++)
			{
				 *img1=0;
				 ++img1;
			}
			}
      }
      else
      {
		  for (int m = 0; m < nc; m++)
		  {
			  *img1=0;
			  ++img1;
		  }
      }
    }*/
  //Calculation intrinsic connectivity
  //1.move all the pointer to the front of the array
  msk  = NULL;
  if (maskData!=NULL)
  {
	  msk=(OT*) maskData->GetPointData()->GetScalars()->GetVoidPointer(0);
  }



  IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
  IT *outerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
  int Type = self->GetType();
  float Threshold = self->GetThreshold();
  switch(Type)
    {
    case 1:
      positiveAbs(static_cast<OT *>(0),maskData,msk,self,nt,nc,static_cast<IT *>(0),inData,outerloop,Threshold,out,count);
      break;
    case 2:
      positivePow(static_cast<OT *>(0),maskData,msk,self,nt,nc,static_cast<IT *>(0),inData,outerloop,Threshold,out,count);
      break;
    case 3:
      negativeAbs(static_cast<OT *>(0),maskData,msk,self,nt,nc,static_cast<IT *>(0),inData,outerloop,Threshold,out,count);
      break;
    case 4:
      negativePow(static_cast<OT *>(0),maskData,msk,self,nt,nc,static_cast<IT *>(0),inData,outerloop,Threshold,out,count);
      break;
    case 5:
      bothAbs(static_cast<OT *>(0),maskData,msk,self,nt,nc,static_cast<IT *>(0),inData,outerloop,Threshold,out,count);
      break;
    case 6:
      bothPow(static_cast<OT *>(0),maskData,msk,self,nt,nc,static_cast<IT *>(0),inData,outerloop,Threshold,out,count);
      break;
    }//end of Type switch
}//end of template function
//---------------------------------------------------------------------------
//templated function, called when not using mask
//---------------------------------------------------------------------------
template <class IT>
static void vtkbisIntrinsicConnMapDoConnectivity2(vtkbisIntrinsicConnMap *self,
						 vtkImageData *inData,
						 vtkImageData *outData,
						 vtkImageData *OutData2,
						 IT*)
{
  //IT *img1 = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
  float *count=(float*)OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
  int nt=inData->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc=inData->GetPointData()->GetScalars()->GetNumberOfComponents();
  // Normalize input
/*  for (int i=0;i< nt;i++)
    {
      double psum=0.0;
      IT* temp=img1;
      for (int m = 0; m < nc; m++)
	{
	  //GetComponent(voxel,m) Return the data component at the ith tuple and mth component
	  psum += (*temp)*(*temp);
	  ++temp;
	}
      double norm = sqrt(psum);
      for (int m = 0; m < nc; m++)
	{
	  double v= double(*img1)/norm;
	  *img1= (IT)v;
	  ++img1;
	}
    }
    */
  IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
  IT *outerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
  int Type = self->GetType();
  float Threshold = self->GetThreshold();
  switch(Type)
    {
    case 1:
      positiveAbsNoMask(self, nt, nc,static_cast<IT *>(0),inData,outerloop,Threshold,out,count);
      break;
    case 2:
      positivePowNoMask(self, nt, nc,static_cast<IT *>(0),inData,outerloop,Threshold,out,count);
      break;
    case 3:
      negativeAbsNoMask(self, nt, nc,static_cast<IT *>(0),inData,outerloop,Threshold,out,count);
      break;
    case 4:
      negativePowNoMask(self, nt, nc,static_cast<IT *>(0),inData,outerloop,Threshold,out,count);
      break;
    case 5:
      bothAbsNoMask(self, nt, nc,static_cast<IT *>(0),inData,outerloop,Threshold,out,count);
      break;
    case 6:
      bothPowNoMask(self, nt, nc,static_cast<IT *>(0),inData,outerloop,Threshold,out,count);
      break;
    }
}
template <class IT>
static void positiveAbsNoMask(vtkbisIntrinsicConnMap *self,int& nt,int& nc,IT*,vtkImageData *inData,IT *outerloop,float Threshold,IT *out,float *count)
{
	cout<<"no Mask, Whole, Positive, Abs"<<endl;
	int maxcount=int(nt/10);
	int outcount=0;
	float pog=0.0;
  for (int l = 0; l<nt;l++)
    {
      float goodSum = 0.0;
      int counter = 0;
      //for each voxel n
      IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
      for (int n = 0; n<nt; n++)
	{
	  IT *thisvoxel = outerloop;
	  float sum = 0;
	  // For each component m
	  for (int m = 0; m < nc; m++)
	    {
	      sum += (*thisvoxel)*(*innerloop);
	      ++thisvoxel;
	      ++innerloop;
	    }
	  sum/=nc;
	  if (sum >= Threshold)
	    {
	      goodSum += sum;
	      ++counter;
	    }
	}//end of the other voxel
      *out=(IT)goodSum;
      *count=counter;
      ++out;
      ++count;
      outerloop=outerloop+nc;
      ++outcount;
      if (outcount==maxcount)
      {
    	  pog=float(l)/float(nt);
          self->UpdateProgress(pog);
          outcount=0;
          fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(l)/float(nt));
      }
      //self->UpdateProgress(float(l+1)/float(nt)); //fprintf(stderr,"Progress = %f\n",self->GetProgress());
    }//end of outer loop
}

template <class IT,class OT>
static void positiveAbs(OT*,vtkImageData *maskData,OT *msk,vtkbisIntrinsicConnMap *self,int& nt,int& nc,IT*,vtkImageData *inData,IT *outerloop,float Threshold,IT *out,float *count)
{
	cout<<"Whole, Positive, Abs"<<endl;
	int maxcount=int(nt/10);
	int outcount=0;
	float pog=0.0;
	for (int l = 0; l<nt;l++)
    {
		float goodSum = 0.0;
		int counter = 0;
		int DoGlm=1;
		if (msk != NULL)
		{
			if (*msk < 1 )
				DoGlm=0;
			++msk;
		}
		if(DoGlm)
		{
			//for each voxel n
			IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
			OT *msk2  = NULL;
			if (maskData!=NULL)
			{
			   msk2=(OT*) maskData->GetPointData()->GetScalars()->GetVoidPointer(0);
			}
			for (int n = 0; n<nt; n++)
			{
				IT *thisvoxel = outerloop;
				float sum = 0;
				int DoGlm2=1;
				if(msk2 != NULL)
				{
					if (*msk2 < 1 )
						DoGlm2=0;
				    ++msk2;
				}
				if(DoGlm2)
				{
					// For each component m
					for (int m = 0; m < nc; m++)
					{
						sum += (*thisvoxel)*(*innerloop);
						++thisvoxel;
						++innerloop;
					}
					sum/=nc;
					if (sum >= Threshold)
					{
						goodSum += sum;
						++counter;
					}
				}else{
					innerloop=innerloop+nc;
				}//end of DoGlm2
	    }//end of the other voxel
	}//end of DoGlm
      *out=(IT)goodSum;
      *count=counter;
      ++out;
      ++count;
      outerloop=outerloop+nc;
      ++outcount;
      if (outcount==maxcount)
      {
          pog=float(l)/float(nt);
          self->UpdateProgress(pog);
          outcount=0;
          fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(l)/float(nt));
      }
      //self->UpdateProgress(float(l+1)/float(nt)); //fprintf(stderr,"Progress = %f\n",self->GetProgress());
    }//end of outer loop
}

template <class IT>
static void positivePowNoMask(vtkbisIntrinsicConnMap *self,int& nt,int& nc,IT*,vtkImageData *inData,IT *outerloop,float Threshold,IT *out,float *count)
{
	cout<<"no Mask, Whole, Positive, Sqr"<<endl;
	int maxcount=int(nt/10);
	int outcount=0;
	float pog=0.0;
  for (int l = 0; l<nt;l++)
    {
      float goodSum = 0.0;
      int counter = 0;
      // For each voxel n
      IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
      for (int n = 0; n<nt; n++)
	{
	  IT *thisvoxel = outerloop;
	  float sum = 0;
	  // For each component m
	  for (int m = 0; m < nc; m++)
	    {
	      sum += (*thisvoxel)*(*innerloop);
	      ++thisvoxel;
	      ++innerloop;
	    }
	  sum/=nc;
	  if(sum >= Threshold)
	    {
	      goodSum += sum*sum;
	      ++counter;
	    }
	}//end of the other voxel
      *out=(IT)goodSum;
      *count=counter;
      ++out;
      ++count;
      outerloop=outerloop+nc;
      ++outcount;
      if (outcount==maxcount)
      {
          pog=float(l)/float(nt);
          self->UpdateProgress(pog);
          outcount=0;
          fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(l)/float(nt));
      }
      //self->UpdateProgress(float(l+1)/float(nt)); //fprintf(stderr,"Progress = %f\n",self->GetProgress());
    }//end of outer loop

}

template <class IT,class OT>
static void positivePow(OT*,vtkImageData *maskData,OT *msk,vtkbisIntrinsicConnMap *self,int& nt,int& nc,IT*,vtkImageData *inData,IT *outerloop,float Threshold,IT *out,float *count)
{
	cout<<"Whole, Positive, Sqr"<<endl;
	int maxcount=int(nt/10);
	int outcount=0;
	float pog=0.0;
  for (int l = 0; l<nt;l++)
    {
      float goodSum = 0.0;
      int counter = 0;
      int DoGlm=1;
      if (msk != NULL)
	{
	  if (*msk < 1 )
	    DoGlm=0;
	  ++msk;
	}
      if(DoGlm)
	{
	  // For each voxel n
	  IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
	  OT *msk2  = NULL;
	  if (maskData!=NULL)
	  {
	     msk2=(OT*) maskData->GetPointData()->GetScalars()->GetVoidPointer(0);
	  }
	  for (int n = 0; n<nt; n++)
	  {
	      IT *thisvoxel = outerloop;
	      float sum = 0;
	      int DoGlm2=1;
	      if(msk2 != NULL)
	      {
	    	  if (*msk2 < 1 )
	    		  DoGlm2=0;
	    	  ++msk2;
	      }
	      if(DoGlm2)
	      {
	    	  // For each component m
	    	  for (int m = 0; m < nc; m++)
	    	  {
	    		  sum += (*thisvoxel)*(*innerloop);
	    		  ++thisvoxel;
	    		  ++innerloop;
	    	  }
	    	  sum/=nc;
	    	  if(sum >= Threshold)
	    	  {
	    		  goodSum += sum*sum;
	    		  ++counter;
	    	  }
	      }else{
	    	  innerloop=innerloop+nc;
	      }//end of DoGlm2
	    }//end of the other voxel
	}//end of DoGlm
      *out=(IT)goodSum;
      *count=counter;
      ++out;
      ++count;
      outerloop=outerloop+nc;
      ++outcount;
      if (outcount==maxcount)
      {
          pog=float(l)/float(nt);
          self->UpdateProgress(pog);
          outcount=0;
          fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(l)/float(nt));
      }
      //self->UpdateProgress(float(l+1)/float(nt)); //fprintf(stderr,"Progress = %f\n",self->GetProgress());
    }//end of outer loop
}

template <class IT>
static void negativeAbsNoMask(vtkbisIntrinsicConnMap *self,int& nt,int& nc,IT*,vtkImageData *inData,IT *outerloop,float Threshold,IT *out,float *count)
{
	cout<<"no Mask, Whole,Negative, Abs"<<endl;
	int maxcount=int(nt/10);
	int outcount=0;
	float pog=0.0;
  for (int l = 0; l<nt;l++)
    {
      float goodSum = 0.0;
      int counter = 0;
      // For each voxel n
      IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
      for (int n = 0; n<nt; n++)
	{
	  IT *thisvoxel = outerloop;
	  float sum = 0;
	  // For each component m
	  for (int m = 0; m < nc; m++)
	    {
	      sum += (*thisvoxel)*(*innerloop);
	      ++thisvoxel;
	      ++innerloop;
	    }
	  sum/=nc;
	  if(sum <= -Threshold)
	    {
	      sum = sum * -1;
	      goodSum += sum;
	      ++counter;
	    }
	}//end of the other voxel
      *out=(IT)goodSum;
      *count=counter;
      ++out;
      ++count;
      outerloop=outerloop+nc;
      ++outcount;
      if (outcount==maxcount)
      {
          pog=float(l)/float(nt);
          self->UpdateProgress(pog);
          outcount=0;
          fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(l)/float(nt));
      }
      //self->UpdateProgress(float(l+1)/float(nt)); //fprintf(stderr,"Progress = %f\n",self->GetProgress());
    }//end of outer loop
}

template <class IT,class OT>
static void negativeAbs(OT*,vtkImageData *maskData,OT *msk,vtkbisIntrinsicConnMap *self,int& nt,int& nc,IT*,vtkImageData *inData,IT *outerloop,float Threshold,IT *out,float *count)
{
	cout<<"Whole,Negative, Abs"<<endl;
	int maxcount=int(nt/10);
	int outcount=0;
	float pog=0.0;
  for (int l = 0; l<nt;l++)
    {
      float goodSum = 0.0;
      int counter = 0;
      int DoGlm=1;
      if (msk != NULL)
	{
	  if (*msk < 1 )
	    DoGlm=0;
	  ++msk;
	}
      if(DoGlm)
	{
	  // For each voxel n
	  IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
	  OT *msk2  = NULL;
	  if (maskData!=NULL)
	  {
		  msk2=(OT*) maskData->GetPointData()->GetScalars()->GetVoidPointer(0);
	  }
	  for (int n = 0; n<nt; n++)
	  {
	      IT *thisvoxel = outerloop;
	      float sum = 0;
	      int DoGlm2=1;
	      if(msk2 != NULL)
	      {
	    	  if (*msk2 < 1 )
	    		  DoGlm2=0;
	          ++msk2;
	      }
	      if(DoGlm2)
	      {
	    	  // For each component m
	    	  for (int m = 0; m < nc; m++)
	    	  {
	    		  sum += (*thisvoxel)*(*innerloop);
	    		  ++thisvoxel;
	    		  ++innerloop;
	    	  }
	    	  sum/=nc;
	    	  if(sum <= -Threshold)
	    	  {
	    		  sum = sum * -1;
	    		  goodSum += sum;
	    		  ++counter;
	    	  }
	      }else{
	    	  innerloop=innerloop+nc;
	      }//end of DoGlm2
	    }//end of the other voxel
	}//end of DoGlm
      *out=(IT)goodSum;
      *count=counter;
      ++out;
      ++count;
      outerloop=outerloop+nc;
      ++outcount;
      if (outcount==maxcount)
      {
          pog=float(l)/float(nt);
          self->UpdateProgress(pog);
          outcount=0;
          fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(l)/float(nt));
      }
      //self->UpdateProgress(float(l+1)/float(nt)); //fprintf(stderr,"Progress = %f\n",self->GetProgress());
    }//end of outer loop
}

template <class IT>
static void negativePowNoMask(vtkbisIntrinsicConnMap *self,int& nt,int& nc,IT*,vtkImageData *inData,IT *outerloop,float Threshold,IT *out,float *count)
{
	cout<<"no Mask, Whole, Negative, Sqr"<<endl;
	int maxcount=int(nt/10);
	int outcount=0;
	float pog=0.0;
  for (int l = 0; l<nt;l++)
    {
      float goodSum = 0.0;
      int counter = 0;
      // For each voxel n
      IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
      for (int n = 0; n<nt; n++)
	{
	  IT *thisvoxel = outerloop;
	  float sum = 0;
	  // For each component m
	  for (int m = 0; m < nc; m++)
	    {
	      sum += (*thisvoxel)*(*innerloop);
	      ++thisvoxel;
	      ++innerloop;
	    }
	  sum/=nc;
	  if (sum <= -Threshold)
	    {
	      goodSum += sum*sum;
	      ++counter;
	    }
	}//end of the other voxel
      *out=(IT)goodSum;
      *count=counter;
      ++out;
      ++count;
      outerloop=outerloop+nc;
      ++outcount;
      if (outcount==maxcount)
      {
          pog=float(l)/float(nt);
          self->UpdateProgress(pog);
          outcount=0;
          fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(l)/float(nt));
      }
      //self->UpdateProgress(float(l+1)/float(nt)); //fprintf(stderr,"Progress = %f\n",self->GetProgress());
    }//end of outer loop
}

template <class IT,class OT>
static void negativePow(OT*,vtkImageData *maskData,OT *msk,vtkbisIntrinsicConnMap *self,int& nt,int& nc,IT*,vtkImageData *inData,IT *outerloop,float Threshold,IT *out,float *count)
{
	cout<<"Whole, Negative, Sqr"<<endl;
	int maxcount=int(nt/10);
	int outcount=0;
	float pog=0.0;
  for (int l = 0; l<nt;l++)
    {
      float goodSum = 0.0;
      int counter = 0;
      int DoGlm=1;
      if (msk != NULL)
	{
	  if (*msk < 1 )
	    DoGlm=0;
	  ++msk;
	}
      if(DoGlm)
	{
	  // For each voxel n
	  IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
	  OT *msk2  = NULL;
	  if (maskData!=NULL)
	  {
		  msk2=(OT*) maskData->GetPointData()->GetScalars()->GetVoidPointer(0);
	  }
	  for (int n = 0; n<nt; n++)
	  {
	      IT *thisvoxel = outerloop;
	      float sum = 0;
	      int DoGlm2=1;
	      if(msk2 != NULL)
	      {
	    	  if (*msk2 < 1 )
	    		  DoGlm2=0;
	          ++msk2;
	      }
	      if(DoGlm2)
	      {
	    	  // For each component m
	    	  for (int m = 0; m < nc; m++)
	    	  {
	    		  sum += (*thisvoxel)*(*innerloop);
	    		  ++thisvoxel;
	    		  ++innerloop;
	    	  }
	    	  sum/=nc;
	    	  if (sum <= -Threshold)
	    	  {
	    		  goodSum += sum*sum;
	    		  ++counter;
	    	  }
	      }else{
	    	  innerloop=innerloop+nc;
	      }//end of DoGlm2
	    }//end of the other voxel
	}//end of DoGlm
      *out=(IT)goodSum;
      *count=counter;
      ++out;
      ++count;
      outerloop=outerloop+nc;
      ++outcount;
      if (outcount==maxcount)
      {
          pog=float(l)/float(nt);
          self->UpdateProgress(pog);
          outcount=0;
          fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(l)/float(nt));
      }
      //self->UpdateProgress(float(l+1)/float(nt)); //fprintf(stderr,"Progress = %f\n",self->GetProgress());
    }//end of outer loop
}

template <class IT>
static void bothAbsNoMask(vtkbisIntrinsicConnMap *self,int& nt,int& nc,IT*,vtkImageData *inData,IT *outerloop,float Threshold,IT *out,float *count)
{
	cout<<"no Mask, Whole,Both, Abs"<<endl;
	int maxcount=int(nt/10);
	int outcount=0;
	float pog=0.0;
  for (int l = 0; l<nt;l++)
    {
      float goodSum = 0.0;
      int counter = 0;
      // For each voxel n
      IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
      for (int n = 0; n<nt; n++)
	{
	  IT *thisvoxel = outerloop;
	  float sum = 0;
	  // For each component m
	  for (int m = 0; m < nc; m++)
	    {
	      sum += (*thisvoxel)*(*innerloop);
	      ++thisvoxel;
	      ++innerloop;
	    }
	  sum/=nc;
	  if (sum <= -Threshold || sum >= Threshold)
	    {
	      if(sum <0)
		sum = sum * -1;
	      goodSum += sum;
	      ++counter;
	    }
	}//end of the other voxel
      *out=(IT)goodSum;
      *count=counter;
      ++out;
      ++count;
      outerloop=outerloop+nc;
      ++outcount;
      if (outcount==maxcount)
      {
          pog=float(l)/float(nt);
          self->UpdateProgress(pog);
          outcount=0;
          fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(l)/float(nt));
      }
      //self->UpdateProgress(float(l+1)/float(nt)); //fprintf(stderr,"Progress = %f\n",self->GetProgress());
    }//end of outer loop
}

template <class IT,class OT>
static void bothAbs(OT*,vtkImageData *maskData,OT *msk,vtkbisIntrinsicConnMap *self,int& nt,int& nc,IT*,vtkImageData *inData,IT *outerloop,float Threshold,IT *out,float *count)
{
	cout<<"Whole,Both, Abs"<<endl;
	int maxcount=int(nt/10);
	int outcount=0;
	float pog=0.0;
  for (int l = 0; l<nt;l++)
    {
      float goodSum = 0.0;
      int counter = 0;
      int DoGlm=1;
      if (msk != NULL)
	{
	  if (*msk < 1 )
	    DoGlm=0;
	  ++msk;
	}
      if(DoGlm)
	{
	  // For each voxel n
	  IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
	  OT *msk2  = NULL;
	  if (maskData!=NULL)
	  {
	     msk2=(OT*) maskData->GetPointData()->GetScalars()->GetVoidPointer(0);
	  }
	  for (int n = 0; n<nt; n++)
	    {
	      IT *thisvoxel = outerloop;
	      float sum = 0;
	      int DoGlm2=1;
	      if(msk2 != NULL)
	      {
	    	  if (*msk2 < 1 )
	    		  DoGlm2=0;
	          ++msk2;
	      }
	      if(DoGlm2)
	      {
	    	  // For each component m
	    	  for (int m = 0; m < nc; m++)
	    	  {
	    		  sum += (*thisvoxel)*(*innerloop);
	    		  ++thisvoxel;
	    		  ++innerloop;
	    	  }
	    	  sum/=nc;
	    	  if (sum <= -Threshold || sum >= Threshold)
	    	  {
	    		  if(sum <0)
	    			  sum = sum * -1;
	    		  goodSum += sum;
	    		  ++counter;
	    	  }
	      }else{
	    	  innerloop=innerloop+nc;
	      }//end of DoGlm2
	    }//end of the other voxel
	}//end of DoGlm
      *out=(IT)goodSum;
      *count=counter;
      ++out;
      ++count;
      outerloop=outerloop+nc;
      ++outcount;
      if (outcount==maxcount)
      {
          pog=float(l)/float(nt);
          self->UpdateProgress(pog);
          outcount=0;
          fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(l)/float(nt));
      }
      //self->UpdateProgress(float(l+1)/float(nt)); //fprintf(stderr,"Progress = %f\n",self->GetProgress());
    }//end of outer loop
}

template <class IT>
static void bothPowNoMask(vtkbisIntrinsicConnMap *self,int& nt,int& nc,IT*,vtkImageData *inData,IT *outerloop,float Threshold,IT *out,float *count)
{
	cout<<"no Mask, Whole, Both, Sqr"<<endl;
	int maxcount=int(nt/10);
	int outcount=0;
	float pog=0.0;
	for (int l = 0; l<nt;l++)
    {
      float goodSum = 0.0;
      int counter=0;
      // For each voxel n
      IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
      for (int n = 0; n<nt; n++)
      {
	  IT *thisvoxel = outerloop;
	  float sum = 0;
	  // For each component m
	  for (int m = 0; m < nc; m++)
	    {
	      sum += (*thisvoxel)*(*innerloop);
	      ++thisvoxel;
	      ++innerloop;
	    }
	  sum/=nc;
	  if(sum <= -Threshold || sum >= Threshold)
	    {
	      goodSum += sum*sum;
	      ++counter;
	    }
	}//end of the other voxel
      *out=(IT)goodSum;
      *count=counter;
      ++out;
      ++count;
      outerloop=outerloop+nc;
      ++outcount;
      if (outcount==maxcount)
      {
    	  pog=float(l)/float(nt);
          self->UpdateProgress(pog);
          outcount=0;
          fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(l)/float(nt));
      }
      //self->UpdateProgress(float(l+1)/float(nt)); //fprintf(stderr,"Progress = %f\n",self->GetProgress());
    }//end of outer loop
}

template <class IT,class OT>
static void bothPow(OT*,vtkImageData *maskData,OT *msk,vtkbisIntrinsicConnMap *self,int& nt,int& nc,IT*,vtkImageData *inData,IT *outerloop,float Threshold,IT *out,float *count)
{
	cout<<"Whole, Both, Sqr"<<endl;
	int maxcount=int(nt/10);
	int outcount=0;
	float pog=0.0;
	for (int l = 0; l<nt;l++)
    {
      float goodSum = 0.0;
      int counter=0;
      int DoGlm=1;
      if (msk != NULL)
      {
    	  if (*msk < 1 )
    		  DoGlm=0;
    	  ++msk;
      }
      if(DoGlm)
      {
    	  // For each voxel n
    	  IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
    	  OT *msk2  = NULL;
    	  if (maskData!=NULL)
    	  {
    	  	  msk2=(OT*) maskData->GetPointData()->GetScalars()->GetVoidPointer(0);
    	  }
    	  for (int n = 0; n<nt; n++)
    	  {
    		  IT *thisvoxel = outerloop;
    	      float sum = 0;
    	      int DoGlm2=1;
    	      if(msk2 != NULL)
    	      {
    	    	  if (*msk2 < 1 )
    	    		  DoGlm2=0;
    	    	  ++msk2;
    	      }
    	      if(DoGlm2)
    	      {
    	    	  // For each component m
    	    	  for (int m = 0; m < nc; m++)
    	    	  {
    	    		  sum += (*thisvoxel)*(*innerloop);
			  // cout<<(*thisvoxel)<<"*"<<(*innerloop)<<"\n";
    	    		  ++thisvoxel;
    	    		  ++innerloop;
    	    	  }
    	    	  sum/=nc;
		  // cout<<"sum/nc="<<sum<<"\n";
    	    	  if(sum <= -Threshold || sum >= Threshold)
    	    	  {
    	    		  goodSum += sum*sum;
			  //cout<<sum<<"*"<<sum<<"="<<sum*sum<<"\n";
    	    		  ++counter;
			  //cout<<"counter:"<<counter<<"\n";
    	    	  }
    	      }else{
    	    	  innerloop=innerloop+nc;
    	      }//end of DoGlm2
    	  }//end of the other voxel
	}//end of DoGlm
      *out=(IT)goodSum;
      *count=counter;
      //cout<<"out:"<<*out<<"count:"<<*count<<"\n";
      ++out;
      ++count;
      outerloop=outerloop+nc;
      ++outcount;
      if (outcount==maxcount)
      {
    	  pog=float(l)/float(nt);
    	  self->UpdateProgress(pog);
      	  outcount=0;
      	  fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(l)/float(nt));
      }
      //self->UpdateProgress(float(l+1)/float(nt)); //fprintf(stderr,"Progress = %f\n",self->GetProgress());
    }//end of outer loop
}

template <class IT>
static void vtkbisIntrinsicConnMapDoConnectivity1(vtkbisIntrinsicConnMap *self,
						  vtkImageData *inData,
						  vtkImageData *outData,
						  vtkImageData *OutData2,
						  vtkImageData *maskData,
						  IT* )
{
  switch (maskData->GetScalarType())
    {
      vtkTemplateMacro7(vtkbisIntrinsicConnMapDoConnectivity, self, inData,outData,OutData2,maskData,
			static_cast<IT *>(0),
			static_cast<VTK_TT *>(0));
    default:
      vtkGenericWarningMacro("Execute: Unknown input ScalarType");
      return;
    }
}
// --------------------------------------------------------------------
// Old Way
// ---------------------------------------------------------------------
void vtkbisIntrinsicConnMap::OldExecute(vtkImageData* input ,vtkImageData* output)
{
  fprintf(stderr,"vtkbisIntrinsicConnMap");

  int dim[3];
  int dimMask[3];
  int ia;
  int tsum = 0;
/*
  vtkImageCast* cast=vtkImageCast::New();
  cast->SetInput(input);
  cast->SetOutputScalarTypeTofloat();
  cast->Update();

  vtkImageData* inp=vtkImageData::New();
  inp->ShallowCopy(cast->GetOutput());
  cast->Delete();
 */
  vtkbisImageTimeSeriesNormalize* norm1=vtkbisImageTimeSeriesNormalize::New();
  norm1->SetInput(input);
  norm1->Update();

  vtkImageData* inp=vtkImageData::New();
  inp->ShallowCopy(norm1->GetOutput());
  norm1->Delete();

  vtkDataArray* temp=inp->GetPointData()->GetScalars();
  int nt=temp->GetNumberOfTuples();//number of voxels
  int nc=temp->GetNumberOfComponents();//number of frames (time series)

  inp->GetDimensions(dim);
  if (this->ImageMask!=NULL)
    {
      this->ImageMask->GetDimensions(dimMask);
      for (ia=0;ia<=2;ia++)
	tsum += abs(dim[ia]-dimMask[ia]);
      if (tsum>0)
	{
	  fprintf(stderr,"Bad Mask Input to vtkbisIntrinsicConnMap SimpleExecute\n");
	  return;
	}
    }

  vtkDataArray*   mask = NULL;
  if (this->ImageMask!=NULL)
    mask=this->ImageMask->GetPointData()->GetScalars();

  vtkDataArray* out=output->GetPointData()->GetScalars();
  out->FillComponent(0,0.0);

  OutData2=vtkImageData::New();
  OutData2->SetOrigin(output->GetOrigin());
  OutData2->SetSpacing(output->GetSpacing());
  OutData2->SetDimensions(output->GetDimensions());
  OutData2->SetScalarTypeToFloat();
  OutData2->AllocateScalars();//allocation space on memery

  vtkDataArray* out2=OutData2->GetPointData()->GetScalars();
  out2->FillComponent(0,0);

  float sum = 0;
  float psum = 0;
  float norm = 0;
  float goodSum = 0;
  int i,l,n,m,u,v,index;
  this->UpdateProgress(0.01);
  //normalize time series voxel by voxel
/*  for (i=0;i< nt;i++)
    {
      psum = 0;
      int DoGlm=1;
      if (mask!=NULL)
	{
	  if (mask->GetComponent(i,0)<1)
	    {
	      DoGlm=0;
	    }
	}
      if(DoGlm)
	{
	  for (m = 0; m < nc; m++)
	    {
	      psum += pow(temp->GetComponent(i,m),2);//GetComponent(voxel,m) Return the data component at the ith tuple and mth component
	    }
	  norm = sqrt(psum);

	  for (m = 0; m < nc; m++)
	    {
	      temp->SetComponent(i,m,temp->GetComponent(i,m)/norm);
	      //std::cout<<"t:"<<i<<"m:"<<m<<"n:"<<temp->GetComponent(i,m)<<endl;
	    }
	}else{
	for (m = 0; m < nc; m++)
	  temp->SetComponent(i,m,0);
      }
    }
*/
  //Calculation intrinsic connectivity
  for (l = 0; l<nt;l++)
    {
      goodSum = 0;
      int DoGlm=1;
      int counter = 0;
      if (mask != NULL)
	{
	  if (mask->GetComponent(l,0)<1)
	    {
	      DoGlm=0;
	    }
	}
      if(DoGlm)
	{
	  for (n = 0; n<nt; n++)
	    {
	      sum = 0;
	      for (m = 0; m < nc; m++)
		{
		  //fprintf(stderr,"Getting %d,%d and %d,%d\n",l,m,n,m);
		  sum += temp->GetComponent(l,m)*temp->GetComponent(n,m);
		}
	      sum/=nc;
	      switch(this->Range)
		{
		case 1:
		  //process for >0 sum only
		  if(sum > this->Threshold)
		    {
		      if(this->Abs == 1)
			{
			  goodSum += sum;
			  ++counter;
			}else{
			goodSum += pow(sum,2);
			++counter;
		      }
		    }
		  break;
		case 2:
		  //process for <0 sum only
		  if(sum < this->Threshold * -1)
		    {
		      if(this->Abs == 1)
			{
			  sum = sum * -1;
			  goodSum += sum;
			  ++counter;
			}else{
			goodSum += pow(sum,2);
			++counter;
		      }
		    }
		  break;
		case 3:
		  //process all sum
		  if(sum > this->Threshold || sum < this->Threshold * -1)
		    {
		      if(this->Abs == 1)
			{
			  if(sum <0)
			    {
			      sum = sum * -1;
			    }
			  goodSum += sum;
			  ++counter;
			}else{
			goodSum += pow(sum,2);
			++counter;
		      }
		    }
		}//end of switch
	    }//end of the other voxel
	}//end of DoGlm
      out->SetComponent(l,0,goodSum/nt);
      out2->SetComponent(l,0,counter/nt);
      //std::cout<<l<<"voxel done"<<endl;
      this->UpdateProgress(float(l+1)/float(nt));
    }
  this->UpdateProgress(1.0);
}
// ---------------------------------------------------------------------------------------------------------------------------
//   Main Function
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisIntrinsicConnMap::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{
  //fprintf(stderr," beginning vtkbisIntrinsicConnMap\n");

  if (this->Optimized==0)
    {
      fprintf(stderr,"Running Non Optimized Code\n");
      this->OldExecute(input,output);
      return;
    }

  /*  vtkImageCast* cast=vtkImageCast::New();
    cast->SetInput(input);
    cast->SetOutputScalarTypeToFloat();
    cast->Update();*/

  vtkbisImageTimeSeriesNormalize* norm1=vtkbisImageTimeSeriesNormalize::New();
  norm1->SetInput(input);
  norm1->Update();

  vtkImageData* inp=vtkImageData::New();
  inp->ShallowCopy(norm1->GetOutput());
  norm1->Delete();

  /*vtkDataArray* inpinp=inp->GetPointData()->GetScalars();
  int inpnt=inpinp->GetNumberOfTuples();//number of voxels
  int inpnc=inpinp->GetNumberOfComponents();//number of frames
  for(int i=0;i<inpnt;i++)
  {
	  for(int j=0;j<inpnc;j++)
	  {
		  cout<<inpinp->GetComponent(i,j)<<"\t";
	  }
	  cout<<"\n";
  }*/

  if (this->OutData2!=NULL)
    this->OutData2->Delete();

  OutData2=vtkImageData::New();
  OutData2->SetDimensions(output->GetDimensions());
  OutData2->SetSpacing(output->GetSpacing());
  OutData2->SetOrigin(output->GetOrigin());
  OutData2->SetNumberOfScalarComponents(output->GetNumberOfScalarComponents());
  OutData2->SetScalarTypeToFloat();
  OutData2->AllocateScalars();//allocation space on memery
  OutData2->GetPointData()->GetScalars()->FillComponent(0,0.0);

  int dim[3];
  int dimMask[3];
  int ia;
  int tsum = 0;
  inp->GetDimensions(dim);
  if (this->ImageMask!=NULL)
    {
      this->ImageMask->GetDimensions(dimMask);
      for (ia=0;ia<=2;ia++)
	tsum += abs(dim[ia]-dimMask[ia]);
      if (tsum>0)
	{
    	  fprintf(stderr,"Bad Mask Input to vtkbisIntrinsicConnMap SimpleExecute\n");
    	  return;
	}
    }

  if (this->Range == 1 ) {
    if (this->Abs == 1 ) {
      //PositiveAbs
      this->Type = 1;
    }else{
      //PositivePow
      this->Type= 2 ;
    }
  }else if ( this->Range == 2 ) {
    if ( this->Abs == 1 ) {
      //NegativeAbs
      this->Type = 3;
    }else{
      //NegativePow
      this->Type = 4;
    }
  }else{
    if ( this->Abs == 1 ){
      //BothAbs
      this->Type = 5;
    } else {
      //BothPow
      this->Type = 6;
    }
  }
  //fprintf(stderr,"running vtkbisIntrinsicConnMap use type=%d threshold=%.2f range=%d abs=%d optimize=%d\n",
	//  this->Type,this->Threshold,this->Range,this->Abs,this->Optimized);

  this->UpdateProgress(0.01);
  time_t start,end;
  time(&start);

  if(this->ImageMask!=NULL)
    {
      switch (inp->GetScalarType())
	{
	  vtkTemplateMacro6(vtkbisIntrinsicConnMapDoConnectivity1,this,
			    inp, output, OutData2,
			    this->ImageMask,static_cast<VTK_TT *>(0));
	default:
	  vtkErrorMacro(<< "Execute: Unknown ScalarType");
	  return;
	}
    }else{
    switch (inp->GetScalarType())
      {
	vtkTemplateMacro5(vtkbisIntrinsicConnMapDoConnectivity2,this,
			  inp,output,OutData2,static_cast<VTK_TT *>(0));
      default:
	vtkErrorMacro(<< "Execute: Unknown ScalarType");
	return;
      }
  }
//normalize the output spatial wise
  vtkDataArray* msk=NULL;
  if (this->ImageMask!=NULL)
    msk=this->ImageMask->GetPointData()->GetScalars();

  vtkDataArray* out=output->GetPointData()->GetScalars();
  	int nt = out->GetNumberOfTuples();

  int counter=0;
  double sum=0.0,sum2=0.0;
  for(int i=0;i<nt;i++)
  {

  	int doAlg=1;
  	if (msk!=NULL)
  	{
  		if (msk->GetComponent(i,0)<1)
  			doAlg=0;
  	}
  	if (doAlg)
  	{
  		++counter;
  		double v=out->GetComponent(i,0);
  		sum+=v;
  		sum2+=v*v;
  	}
  }
  double scalefactor=1.0/(double)counter;
  double mean=sum*scalefactor;
  double sigma=sqrt(sum2*scalefactor-mean*mean);
  if (sigma<0.00001)
	  sigma=1.0;
  if (isnan(sigma) || isinf(sigma))
	  sigma=1.0;
  if (isnan(mean) || isinf(mean))
	  mean=0.0;
  //double sumx=0.0;
  for(int i=0;i<nt;i++)
  {
	  int doAlg=1;
	  if (msk!=NULL)
	  {
		  if (msk->GetComponent(i,0)<1)
			  doAlg=0;
	  }
	  if (doAlg)
	  {
		  //double x = (out->GetComponent(i,0)-mean)/sigma;

		  //std::cout<<"frame2:"<<j<<"x"<<x<<endl;
		  out->SetComponent(i,0,(out->GetComponent(i,0)-mean)/sigma);
		  //std::cout<<out->GetComponent(i,0)<<"@"<<x<<"%";
		  //sumx+=x*x;
	  }

  }
  this->Mean=mean;
  this->Std=sigma;
  		//std::cout<<"sumx"<<sumx<<endl;
  		//std::cout<<"number of tuples in the mask:"<<counter<<endl;
  if (this->Threshold != 0)
  	{
  		//normalize the count output spatial wise


  		vtkDataArray* out2=this->OutData2->GetPointData()->GetScalars();
  		int nt = out2->GetNumberOfTuples();


  		int counter=0;
  		sum=0.0,sum2=0.0;
  		for(int i=0;i<nt;i++)
  		{
  		  	int doAlg=1;
  		  	if (msk!=NULL)
  		  	{
  		  		if (msk->GetComponent(i,0)<1)
  		  			doAlg=0;
  		  	}
  		  	if (doAlg)
  		  	{
  		  		++counter;
  		  		double v=(double)out2->GetComponent(i,0);
  		  		//std::cout<<v<<"|";
  		  		sum+=v;
  		  		sum2+=v*v;
  		  	}
  		  }
  		  double scalefactor=1.0/(double)counter;
  		  double mean=sum*scalefactor;
  		  double sigma=sqrt(sum2*scalefactor-mean*mean);
  		  if (sigma<0.00001)
  			  sigma=1.0;
  		  if (isnan(sigma) || isinf(sigma))
  			  sigma=1.0;
  		  if (isnan(mean) || isinf(mean))
  			  mean=0.0;
  		  //double sumx=0.0;
  		  for(int i=0;i<nt;i++)
  		  {
  			  int doAlg=1;
  			  if (msk!=NULL)
  			  {
  				  if (msk->GetComponent(i,0)<1)
  					  doAlg=0;
  			  }
  			  if (doAlg)
  			  {
  				  //double x = (out2->GetComponent(i,0)-mean)/sigma;
  				  //std::cout<<"frame2:"<<j<<"x"<<x<<endl;
  				  out2->SetComponent(i,0,(out2->GetComponent(i,0)-mean)/sigma);
  				  //sumx+=x*x;
  			  }

  		  }
  		  this->DegreeMean=mean;
  		  this->DegreeStd=sigma;
  		  		//std::cout<<"sumx"<<sumx<<endl;
  		  		//std::cout<<"number of tuples :"<<counter<<endl;
  	}



/*
  if (this->FileName=="")
    return;

  std::ofstream myfile(this->FileName.c_str());
  if(!myfile.is_open())
    {
      cout<< "Unable to open text file" << this->FileName << "\n";
      return;
    }
  myfile<<"Mean: \t "<<this->Mean<<"\t"<<"Standard Deviation: \t "<<this->Std<<"Number of Voxels: \t "<<counter<<"\n";
  if (this->Threshold != 0)
    {
      myfile<<"Degree Mean: \t "<<this->DegreeMean<<"\t"<<"Degree Standard Deviation: \t "<<this->DegreeStd<<"\n";
    }

  myfile.close();

*/
/*  vtkDataArray* dat=output->GetPointData()->GetScalars();
  vtkDataArray* cnt=OutData2->GetPointData()->GetScalars();
  int nt=dat->GetNumberOfTuples();

  for (int i=0;i<nt;i++)
  {
      float c=cnt->GetComponent(i,0);
      if (isnan(c) || isinf(c) || c<1.0)
      {
    	  cnt->SetComponent(i,0,0.0);
    	  dat->SetComponent(i,0,0.0);
      }
      else
      {
    	  if (this->OutputRaw)
    		  dat->SetComponent(i,0,dat->GetComponent(i,0)/c);
    	  else
    		  dat->SetComponent(i,0,vtkbisTimeSeriesCorrelation::RhoToZConversion(dat->GetComponent(i,0)/c));
      }
  }

*/
  time(&end);
  std::cout<<endl<<"elapsed time "<<difftime(end, start)<<" seconds"<<endl;
  this->UpdateProgress(1.0);

}





