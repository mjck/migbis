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
 * vtkbisIpsilateralConnMap.cpp
 *
 *  Created on: Nov 13, 2008
 *      Author: Isabella Murphy, Xilin Shen
 * @Input: 4D vtkImageData object of the whole brain
 * @output: 3D vtkImageData object with sum of absolute or power value of correlation among
 * each voxel of the same side of the brain.
 * User can chose to look at only the positive correlation , only the negative correlation or both.
 * For example, correlation of voxel 0 to each voxel is stored in output vtkImagedata voxel 0 component 0.
 * In the output image, xyz=input dimension, component=1.
 * @mask: if want to use mask to distinguish left and right brain, 0 is background, 1 is right brain, 2 is left brain
 */

#include <vtkbisIpsilateralConnMap.h>
#include "vtkbisImageTimeSeriesNormalize.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkObject.h"
#include "vtkPointData.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include <math.h>
#include <vector>
#include <time.h>
#include "vtkbisTimeSeriesCorrelation.h"
#include "pxisinf.h"
#include <fstream>
#include <cstdlib>

using namespace std;

vtkbisIpsilateralConnMap::vtkbisIpsilateralConnMap() {
	this->Threshold= 0.0;
	this->ImageMask = NULL;
	this->Range = 3;
	this->Abs = 1;
	this->Optimize = 1;
	this->Type = 1;
	this->OutData2=NULL;
//	this->OutputRaw=0;
	this->DoContralateral=0;
	this->LMean=0.0;
	this->LStd=0.0;
	this->RMean=0.0;
	this->RStd=0.0;
	this->LDegreeMean=0.0;
	this->LDegreeStd=0.0;
	this->RDegreeMean=0.0;
	this->RDegreeStd=0.0;
	this->FileName="";
}

vtkbisIpsilateralConnMap::~vtkbisIpsilateralConnMap() {
	this->SetImageMask(NULL);
	if (this->OutData2!=NULL)
	  this->OutData2->Delete();

}


vtkbisIpsilateralConnMap* vtkbisIpsilateralConnMap::New()
{
	// First try to create the object from the vtkObjectFactory
	vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisIpsilateralConnMap");
	if(ret)
    {
		return (vtkbisIpsilateralConnMap*)ret;
    }
	// If the factory was unable to create the object, then create it here.
	return new vtkbisIpsilateralConnMap;
}
void vtkbisIpsilateralConnMap::SetFileName(const char* filename)
{
  this->FileName=filename;
}

const char* vtkbisIpsilateralConnMap::GetFileName()
{
  return this->FileName.c_str();
}

//----------------------------------------------------------------------------------------------------------------------
//RequestInformation
//I. Our vtkImageData input has number of components = number of frames but output vtkImageData object has number of
//components = 1.
// ---------------------------------------------------------------------------------------------------------------------------
int vtkbisIpsilateralConnMap::RequestInformation (vtkInformation * request,
			vtkInformationVector ** inputVector,
			vtkInformationVector * outputVector)
{
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, 1);//set number of components to 1

	return 1;
}

float vtkbisIpsilateralConnMap::CalculateGoodSum (float sum,int& count)
{
	float fsum = 0;
	switch(this->Range)
	{
	case 1://process for >0 sum only
		if(sum >= this->Threshold)
		{
			fsum = pow(sum,2);
			if(this->Abs == 1)
			{
				fsum = sum;
			}
			++count;
		}
	    break;
	case 2://process for <0 sum only
		if(sum <= this->Threshold * -1)
		{
			fsum= pow(sum,2);
			if(this->Abs == 1)
			{
				sum = sum * -1;
				fsum = sum;
			}
			++count;
		}
		break;
	case 3://process all sum
		if(sum >= this->Threshold || sum <= this->Threshold * -1)
		{
			fsum = pow(sum,2);
			if(this->Abs == 1)
			{
				if(sum <0)
				{
					sum = sum * -1;
					fsum = sum;
				}
			}
			++count;
		}
		break;
	default:
		std::cout<<"Something wrong with the range selection"<<endl;
	}//end of switch
	return fsum;
}
//templated function, optimized the original code
template <class IT,class OT>
static void vtkbisIpsilateralConnMapDoConnectivity(vtkbisIpsilateralConnMap *self,
						 vtkImageData *inData,
						 vtkImageData *outData,
						 vtkImageData *OutData2,
						 vtkImageData *maskData,
						 IT*, OT*)
{
  IT *img1 = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
  OT *msk  = NULL;
  if (maskData!=NULL)
    msk=(OT*)maskData->GetPointData()->GetScalars()->GetVoidPointer(0);
  int nt=inData->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nc=inData->GetPointData()->GetScalars()->GetNumberOfComponents();
  // ------------------------------------------------------------------------------
  // Normalization Code
  // ------------------------------------------------------------------------------

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
    	  float psum=0.0;
	      IT* temp=img1;
	      for (int m = 0; m < nc; m++)
	      {
	        //GetComponent(voxel,m) Return the data component at the ith tuple and mth component
	        psum += (*temp)*(*temp);
	        ++temp;
	      }
	      float norm = sqrt(psum);

	      if (norm != 0)
	      {
	    	  for (int m = 0; m < nc; m++)
	    	  {
	    		  float v= float(*img1)/norm;
	    		  *img1= (IT)v;
	    		  //cout<<"psum:"<<psum<<"norm"<<norm<<"img:"<<*img1;
	    		  ++img1;
	    	  }
		  }else{
			  for (int m=0;m<nc;m++)
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

	double position[3],spacing[3];
	int dim[3];
	vector<int> lbrain,rbrain;
	inData->GetSpacing(spacing);
	inData->GetDimensions(dim);
	msk  = NULL;

	//int usemaskforleftright=0;

	if (maskData!=NULL)
	  {
	    msk=(OT*) maskData->GetPointData()->GetScalars()->GetVoidPointer(0);
	    double r[2];
	    maskData->GetPointData()->GetScalars()->GetRange(r);
	    if (r[0]==0.0 && r[1]!=200.0)
	    {
	    	cout<<"Invalid mask. User needs to provide a mask with 0 = background, 100 = right brain, 200 = left brain";
	    	return;
	    }
	      //usemaskforleftright=1;
	  }

	//-------------------------------------------------------
	//sort voxel to left or right brain vector
	//------------------------------------------
	for(int t= 0; t < nt; t++)
	  {
	    int DoGlm=1;
	    if (msk!=NULL)
	    {
	    	if (*msk < 1 )
	    	{
	    		DoGlm=0;
	    		++msk;
	    	}

	    }
	    if(DoGlm)
	    {
	    	/*if (usemaskforleftright==0)
	    	{
	    		inData->GetPoint(t,position);
	    		if( position[0]/spacing[0] > (dim[0]-1)/2 )
	    		{
	    			lbrain.push_back(t);
	    			//std::cout<<"lb:"<<t;
	    		}else{
	    			rbrain.push_back(t);
		      //std::cout<<"rb"<<t;
	    		}
	    		++msk;

	    	}*/
	    	//else
	    	//{
	    		if (*msk == 100){
	    			rbrain.push_back(t);
	    			//std::cout<<"rb:"<<t;
	    		}

	    		else{
	    			lbrain.push_back(t);
	    			//std::cout<<"lb"<<t;
	    		}
	    		++msk;


	    	//}
	    }
	 }
	self->SetLSize(lbrain.size());
	self->SetRSize(rbrain.size());
	//cout<<"lb:"<<lbrain.size()<<"rb:"<<rbrain.size()<<endl;



	//clean up the output images
	outData->GetPointData()->GetScalars()->FillComponent(0,0.0);
	OutData2->GetPointData()->GetScalars()->FillComponent(0,0.0);

	int Type = self->GetType();
	float Threshold = self->GetThreshold();
	if(self->GetDoContralateral() == 0)
	{
		switch(Type)
		{
			case 1:
				positiveAbs(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 2:
				positivePow(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 3:
				negativeAbs(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 4:
				negativePow(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 5:
				bothAbs(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 6:
				bothPow(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
		}
	}else{
		switch(Type)
		{
			case 1:
				ConpositiveAbs(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 2:
				ConpositivePow(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 3:
				ConnegativeAbs(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 4:
				ConnegativePow(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 5:
				ConbothAbs(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 6:
				ConbothPow(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
		}
	}
	//normalize left brain output
	vtkDataArray* out=outData->GetPointData()->GetScalars();
	double sum=0.0,sum2=0.0;
	for(int i=0;i<lbrain.size();i++)
	{
		double v=out->GetComponent(lbrain[i],0);
		//std::cout<<"frame:"<<j<<"v:"<<v;
		sum+=v;
		sum2+=v*v;
	}
	double scalefactor=1.0/(double)lbrain.size();
	double mean=sum*scalefactor;
	double sigma=sqrt(sum2*scalefactor-mean*mean);
	//std::cout<<"m:"<<mean<<"s"<<sigma;
	if (sigma<0.00001)
		sigma=1.0;
	if (isnan(sigma) || isinf(sigma))
		sigma=1.0;
	if (isnan(mean) || isinf(mean))
		mean=0.0;
	//double	sumx = 0;
	for(int i=0;i<lbrain.size();i++)
	{
		out->SetComponent(lbrain[i],0,(out->GetComponent(lbrain[i],0)-mean)/sigma);
	}
	self->SetLMean(mean);
	self->SetLStd(sigma);


	//normalize right brain output
	sum=0.0;sum2=0.0;
	for(int i=0;i<rbrain.size();i++)
	{
		double v=out->GetComponent(rbrain[i],0);
		//std::cout<<"frame:"<<j<<"v:"<<v;
		sum+=v;
		sum2+=v*v;
	}
	scalefactor=1.0/(double)rbrain.size();
	mean=sum*scalefactor;
	sigma=sqrt(sum2*scalefactor-mean*mean);
	//std::cout<<"m:"<<mean<<"s"<<sigma;
	if (sigma<0.00001)
		sigma=1.0;
	if (isnan(sigma) || isinf(sigma))
		sigma=1.0;
	if (isnan(mean) || isinf(mean))
		mean=0.0;
	//double sumx = 0;

	for(int i=0;i<rbrain.size();i++)
	{
		//double x = (out->GetComponent(rbrain[i],0)-mean)/sigma;
		//std::cout<<"frame2:"<<j<<"x"<<x<<endl;
		out->SetComponent(rbrain[i],0,(out->GetComponent(rbrain[i],0)-mean)/sigma);
		//sumx+=x*x;
	}
	//std::cout<<"rbrain size:"<<rbrain.size()<<endl;
	//std::cout<<"sumx"<<sumx<<endl;
	self->SetRMean(mean);
	self->SetRStd(sigma);


	if (Threshold != 0)
	{
		//normalize left brain degree output
		vtkDataArray* out2=OutData2->GetPointData()->GetScalars();

		double sum=0.0,sum2=0.0;
		for(int i=0;i<lbrain.size();i++)
		{
			double v2=out2->GetComponent(lbrain[i],0);
			//std::cout<<"frame:"<<j<<"v:"<<v;
			sum+=v2;
			sum2+=v2*v2;
		}
		double scalefactor=1.0/(double)lbrain.size();
		double mean=sum*scalefactor;
		double sigma=sqrt(sum2*scalefactor-mean*mean);
		//std::cout<<"m:"<<mean<<"s"<<sigma;
		if (sigma<0.00001)
			sigma=1.0;
		if (isnan(sigma) || isinf(sigma))
			sigma=1.0;
		if (isnan(mean) || isinf(mean))
			mean=0.0;
		//double	sumx = 0;
		for(int i=0;i<lbrain.size();i++)
		{
			//double x = (out2->GetComponent(lbrain[i],0)-mean)/sigma;
			out2->SetComponent(lbrain[i],0,(out2->GetComponent(lbrain[i],0)-mean)/sigma);
			//sumx+=x*x;
		}
		//std::cout<<"lbrain size:"<<lbrain.size()<<endl;
		//std::cout<<"sumx"<<sumx<<endl;
		self->SetLDegreeMean(mean);
		self->SetLDegreeStd(sigma);

		//normalize right brain degree output
		sum=0.0;sum2=0.0;
		for(int i=0;i<rbrain.size();i++)
		{
			double v2=out2->GetComponent(rbrain[i],0);
			//std::cout<<"frame:"<<j<<"v:"<<v;
			sum+=v2;
			sum2+=v2*v2;
		}
		scalefactor=1.0/(double)rbrain.size();
		mean=sum*scalefactor;
		sigma=sqrt(sum2*scalefactor-mean*mean);
		//std::cout<<"m:"<<mean<<"s"<<sigma;
		if (sigma<0.00001)
			sigma=1.0;
		if (isnan(sigma) || isinf(sigma))
			sigma=1.0;
		if (isnan(mean) || isinf(mean))
			mean=0.0;
		//double sumx = 0;

		for(int i=0;i<rbrain.size();i++)
		{
			//double x = (out2->GetComponent(rbrain[i],0)-mean)/sigma;
			//std::cout<<"frame2:"<<j<<"x"<<x<<endl;
			out2->SetComponent(rbrain[i],0,(out2->GetComponent(rbrain[i],0)-mean)/sigma);
			//sumx+=x*x;
		}
		//std::cout<<"rbrain size:"<<rbrain.size()<<endl;
		//std::cout<<"sumx"<<sumx<<endl;
		self->SetRDegreeMean(mean);
		self->SetRDegreeStd(sigma);

	}



	//-----------------------------------------------------
	//left brain intrinsic connectivity calculation
	//-----------------------------------------------------

/*	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *lptr = &lbrain[0];int *oldptr;

	//move pointer point to the first voxel of left brain on the output array
	out = out + (*lptr);
	count = count + (*lptr);

	for(int lb=0;lb<lbrain.size();lb++)//lptr counting
	{
		float goodSum = 0.0;
		int counter = 0;

		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;

		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &lbrain[0];

		for(int lbf=0;lbf<lbrain.size();lbf++)//lptrf counting
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;

			//inner loop pointer move to the beginning of the next voxel's time series
			IT *friendvoxel = innerloop+(*lptrf)*nc;

			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			//self->OverThreshold(sum,counter);
			goodSum +=self->CalculateGoodSum(sum,counter);
			++lptrf;
		}
		//write results to output arrays
		*out=(IT)goodSum/lbrain.size();
		*count=(float)counter/lbrain.size();

		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;

		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));

		self->UpdateProgress(float(lb+1)/float(nt));
		//sstd::cout<<"lv: "<<lbrain[lb];
	}//end of left brain

	//-----------------------------------------------------
	//right brain intrinsic connectivity calculation
	//-----------------------------------------------------
	out =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *rptr = &rbrain[0];
	out = out + (*rptr);
	count = count + (*rptr);
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;
		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &rbrain[0];
		for(int rbf=0;rbf<rbrain.size();rbf++)//lptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;
			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			//self->OverThreshold(sum,counter);
			goodSum +=self->CalculateGoodSum(sum,counter);
			++rptrf;
			}
			*out=(IT)goodSum/rbrain.size();
			*count=(float)counter/rbrain.size();
			oldptr = rptr;
			++rptr;
			out = out + ((*rptr)-(*oldptr));
			count= count + ((*rptr)-(*oldptr));
			//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
			self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
			//std::cout<<"rv: "<<rbrain[rb];
		}//end of right brain
		*/
}//end of template function
//--------------------------------------------------------------------------------
//templated function invoked when mask is not used
//-----------------------------------------------------------------------------
template <class IT>
static void vtkbisIpsilateralConnMapDoConnectivity2(vtkbisIpsilateralConnMap *self,
						  vtkImageData *inData,
						  vtkImageData *outData,
						  vtkImageData *OutData2,
						  IT* )
{
	IT *img1 = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
	int nt=inData->GetPointData()->GetScalars()->GetNumberOfTuples();
	int nc=inData->GetPointData()->GetScalars()->GetNumberOfComponents();
	// Normalize the time course
/*	for (int i=0;i< nt;i++)
	{
		float psum=0.0;
		IT* temp=img1;
		for (int m = 0; m < nc; m++)
		{
			//GetComponent(voxel,m) Return the data component at the ith tuple and mth component
		    psum += (*temp)*(*temp);
		    ++temp;
		}
		float norm = sqrt(psum);
		if (norm != 0 )
		{
			for (int m = 0; m < nc; m++)
			{
				float v= float(*img1)/norm;
				*img1= (IT)v;
				++img1;
			}
		}else{
			for (int m = 0; m < nc; m++)
			{
				*img1= 0;
				++img1;
			}
		}
	}*/
	//sort voxel to left or right brain vector
	double position[3],spacing[3];
	int dim[3];
	vector<int> lbrain,rbrain;
	inData->GetSpacing(spacing);
	inData->GetDimensions(dim);
	for(int t= 0; t < nt; t++)
	{
		inData->GetPoint(t,position);
		if( position[0]/spacing[0] > (dim[0]-1)/2 )
		{
			lbrain.push_back(t);
			//std::cout<<"lb:"<<t;
		}else{
			rbrain.push_back(t);
			//std::cout<<"rb"<<t;
		}
	}
	self->SetLSize(lbrain.size());
	self->SetRSize(rbrain.size());
	//clean up the output images
	outData->GetPointData()->GetScalars()->FillComponent(0,0.0);
	OutData2->GetPointData()->GetScalars()->FillComponent(0,0.0);

	int Type = self->GetType();
	float Threshold = self->GetThreshold();
	if(self->GetDoContralateral() == 0)
	{
		switch(Type)
		{
			case 1:
				positiveAbs(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 2:
				positivePow(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 3:
				negativeAbs(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 4:
				negativePow(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 5:
				bothAbs(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 6:
				bothPow(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
		}
	}else{
		switch(Type)
		{
			case 1:
				ConpositiveAbs(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 2:
				ConpositivePow(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 3:
				ConnegativeAbs(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 4:
				ConnegativePow(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 5:
				ConbothAbs(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
			case 6:
				ConbothPow(self,inData,outData,OutData2,static_cast<IT *>(0),lbrain,rbrain,nc,nt,Threshold);
				break;
		}
	}
	//normalize left brain output
		vtkDataArray* out=outData->GetPointData()->GetScalars();
		double sum=0.0,sum2=0.0;
		for(int i=0;i<lbrain.size();i++)
		{
			double v=out->GetComponent(lbrain[i],0);
			//std::cout<<"frame:"<<j<<"v:"<<v;
			sum+=v;
			sum2+=v*v;
		}
		double scalefactor=1.0/(double)lbrain.size();
		double mean=sum*scalefactor;
		double sigma=sqrt(sum2*scalefactor-mean*mean);
		//std::cout<<"m:"<<mean<<"s"<<sigma;
		if (sigma<0.00001)
			sigma=1.0;
		if (isnan(sigma) || isinf(sigma))
			sigma=1.0;
		if (isnan(mean) || isinf(mean))
			mean=0.0;
		//double	sumx = 0;
		for(int i=0;i<lbrain.size();i++)
		{
			out->SetComponent(lbrain[i],0,(out->GetComponent(lbrain[i],0)-mean)/sigma);
		}
		self->SetLMean(mean);
		self->SetLStd(sigma);

		//normalize right brain output
		sum=0.0;sum2=0.0;
		for(int i=0;i<rbrain.size();i++)
		{
			double v=out->GetComponent(rbrain[i],0);
			//std::cout<<"frame:"<<j<<"v:"<<v;
			sum+=v;
			sum2+=v*v;
		}
		scalefactor=1.0/(double)rbrain.size();
		mean=sum*scalefactor;
		sigma=sqrt(sum2*scalefactor-mean*mean);
		//std::cout<<"m:"<<mean<<"s"<<sigma;
		if (sigma<0.00001)
			sigma=1.0;
		if (isnan(sigma) || isinf(sigma))
			sigma=1.0;
		if (isnan(mean) || isinf(mean))
			mean=0.0;
		//double sumx = 0;

		for(int i=0;i<rbrain.size();i++)
		{
			//double x = (out->GetComponent(rbrain[i],0)-mean)/sigma;
			//std::cout<<"frame2:"<<j<<"x"<<x<<endl;
			out->SetComponent(rbrain[i],0,(out->GetComponent(rbrain[i],0)-mean)/sigma);
			//sumx+=x*x;
		}
		self->SetRMean(mean);
		self->SetRStd(sigma);
		//std::cout<<"rbrain size:"<<rbrain.size()<<endl;
		//std::cout<<"sumx"<<sumx<<endl;


		if (Threshold != 0)
		{
			//normalize left brain degree output
			vtkDataArray* out2=OutData2->GetPointData()->GetScalars();

			double sum=0.0,sum2=0.0;
			for(int i=0;i<lbrain.size();i++)
			{
				double v2=out2->GetComponent(lbrain[i],0);
				//std::cout<<"frame:"<<j<<"v:"<<v;
				sum+=v2;
				sum2+=v2*v2;
			}
			double scalefactor=1.0/(double)lbrain.size();
			double mean=sum*scalefactor;
			double sigma=sqrt(sum2*scalefactor-mean*mean);
			//std::cout<<"m:"<<mean<<"s"<<sigma;
			if (sigma<0.00001)
				sigma=1.0;
			if (isnan(sigma) || isinf(sigma))
				sigma=1.0;
			if (isnan(mean) || isinf(mean))
				mean=0.0;
			//double	sumx = 0;
			for(int i=0;i<lbrain.size();i++)
			{
				//double x = (out2->GetComponent(lbrain[i],0)-mean)/sigma;
				out2->SetComponent(lbrain[i],0,(out2->GetComponent(lbrain[i],0)-mean)/sigma);
				//sumx+=x*x;
			}
			//std::cout<<"lbrain size:"<<lbrain.size()<<endl;
			//std::cout<<"sumx"<<sumx<<endl;
			self->SetLDegreeMean(mean);
			self->SetLDegreeStd(sigma);

			//normalize right brain degree output
			sum=0.0;sum2=0.0;
			for(int i=0;i<rbrain.size();i++)
			{
				double v2=out2->GetComponent(rbrain[i],0);
				//std::cout<<"frame:"<<j<<"v:"<<v;
				sum+=v2;
				sum2+=v2*v2;
			}
			scalefactor=1.0/(double)rbrain.size();
			mean=sum*scalefactor;
			sigma=sqrt(sum2*scalefactor-mean*mean);
			//std::cout<<"m:"<<mean<<"s"<<sigma;
			if (sigma<0.00001)
				sigma=1.0;
			if (isnan(sigma) || isinf(sigma))
				sigma=1.0;
			if (isnan(mean) || isinf(mean))
				mean=0.0;
			//double sumx = 0;

			for(int i=0;i<rbrain.size();i++)
			{
				//double x = (out2->GetComponent(rbrain[i],0)-mean)/sigma;
				//std::cout<<"frame2:"<<j<<"x"<<x<<endl;
				out2->SetComponent(rbrain[i],0,(out2->GetComponent(rbrain[i],0)-mean)/sigma);
				//sumx+=x*x;
			}
			//std::cout<<"rbrain size:"<<rbrain.size()<<endl;
			//std::cout<<"sumx"<<sumx<<endl;
			self->SetRDegreeMean(mean);
			self->SetRDegreeStd(sigma);

		}


}
template <class IT>
static void positiveAbs(vtkbisIpsilateralConnMap *self,vtkImageData *inData,vtkImageData *outData,vtkImageData *OutData2,IT*,vector<int>& lbrain,vector<int>& rbrain,int nc,int nt,const float Threshold)
{
	cout<<"Ipsilateral, Positive, Abs"<<endl;
	int maxcount=int(lbrain.size()/10);
	int outcount=0;
	float pog=0.0;
	//left brain intrinsic connectivity calculation
	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);

	int *lptr = &lbrain[0];
	int *oldptr;
	//move pointer point to the first voxel of left brain on the output array

	out = out + (*lptr);
	count = count + (*lptr);
	//cout<<"size:"<<lbrain.size()<<endl;

	for(int lb=0;lb<lbrain.size();lb++)
	{
		float goodSum = 0.0;
		int counter = 0;
		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;

		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &lbrain[0];

		for(int lbf=0;lbf<lbrain.size();lbf++)
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;
			//inner loop pointer move to the beginning of the next voxel's time series

			IT *friendvoxel = innerloop+(*lptrf)*nc;
			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if (sum >= Threshold)
			{
				goodSum += sum;
				++counter;
			}
			++lptrf;
		}
		//write results to output arrays
		*out=(IT)goodSum;
		*count=(float)counter;
		//cout<<*lptr<<"GS:"<<goodSum<<"S:"<<*out<<"N:"<<*count<<endl;
		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;

		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));
		++outcount;
		/*if (outcount==maxcount)
		{
			pog=float(lb+1)/float(nt);
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(lb+1)/float(nt));
		}*/
		//self->UpdateProgress(float(lb+1)/float(nt));
		//std::cout<<"Newlv: "<<lbrain[lb];
	}//end of left brain
	//right brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count2=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);

	int *rptr = &rbrain[0];
	out2 = out2 + (*rptr);
	count2 = count2 + (*rptr);
	maxcount=int(rbrain.size()/10);
	outcount=0;
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;

		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &rbrain[0];

		for(int rbf=0;rbf<rbrain.size();rbf++)//lptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;

			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if (sum >= Threshold)
			{
				goodSum += sum;
				++counter;
			}
			++rptrf;
		}
		*out2=(IT)goodSum;
		*count2=(float)counter;
		//cout<<*rptr<<"RGS:"<<goodSum<<"S:"<<*out<<"N:"<<*count<<endl;
		oldptr = rptr;
		++rptr;
		out2 = out2 + ((*rptr)-(*oldptr));
		count2= count2 + ((*rptr)-(*oldptr));
		++outcount;
		if (outcount==maxcount)
		{
			pog=(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size()));
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size())));
		}
		//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
		//self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
		//std::cout<<"Newrv: "<<rbrain[rb];
	}//end of right brain
}
template <class IT>
static void ConpositiveAbs(vtkbisIpsilateralConnMap *self,vtkImageData *inData,vtkImageData *outData,vtkImageData *OutData2,IT*,vector<int>& lbrain,vector<int>& rbrain,int nc,int nt,const float Threshold)
{
	cout<<"Contralateral, Positive, Abs"<<endl;
	int maxcount=int(lbrain.size()/10);
	int outcount=0;
	float pog=0.0;
	//left to right brain intrinsic connectivity calculation
	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);

	int *lptr = &lbrain[0];
	int *oldptr;
	//move pointer point to the first voxel of left brain on the output array

	out = out + (*lptr);
	count = count + (*lptr);
	//cout<<"left brain size:"<<lbrain.size()<<endl;

	for(int lb=0;lb<lbrain.size();lb++)
	{
		float goodSum = 0.0;
		int counter = 0;
		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;

		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &rbrain[0];

		for(int lbf=0;lbf<rbrain.size();lbf++)
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;
			//inner loop pointer move to the beginning of the next voxel's time series

			IT *friendvoxel = innerloop+(*lptrf)*nc;
			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if (sum >= Threshold)
			{
				//cout<<"sum:"<<sum<<endl;
				goodSum += sum;
				++counter;
			}
			++lptrf;
		}
		//write results to output arrays
		*out=(IT)goodSum;
		*count=(float)counter;
		//cout<<*lptr<<"GS:"<<goodSum<<"S:"<<*out<<"N:"<<*count<<endl;
		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;

		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));
		/*++outcount;
		if (outcount==maxcount)
		{
			pog=float(lb+1)/float(nt);
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(lb+1)/float(nt));
		}*/
		//self->UpdateProgress(float(lb+1)/float(nt));
		//std::cout<<"Newlv: "<<lbrain[lb];
	}//end of left brain
	//right to left brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count2=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);

	int *rptr = &rbrain[0];
	out2 = out2 + (*rptr);
	count2 = count2 + (*rptr);
	maxcount=int(rbrain.size()/10);
    outcount=0;
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;

		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &lbrain[0];

		for(int rbf=0;rbf<lbrain.size();rbf++)//rptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;

			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if (sum >= Threshold)
			{
				goodSum += sum;
				++counter;
			}
			++rptrf;
		}
		*out2=(IT)goodSum;
		*count2=(float)counter;
		//cout<<*rptr<<"RGS:"<<goodSum<<"S:"<<*out<<"N:"<<*count<<endl;
		oldptr = rptr;
		++rptr;
		out2 = out2 + ((*rptr)-(*oldptr));
		count2= count2 + ((*rptr)-(*oldptr));
		++outcount;
		if (outcount==maxcount)
		{
			pog=(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size()));
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size())));
		}
		//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
		//self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
		//std::cout<<"Newrv: "<<rbrain[rb];
	}//end of right brain
}
template <class IT>
static void positivePow(vtkbisIpsilateralConnMap *self,vtkImageData *inData,vtkImageData *outData,vtkImageData *OutData2,IT*,vector<int>& lbrain,vector<int>& rbrain,int nc,int nt,const float Threshold)
{
	cout<<"Ipsilateral, Positive, Sqr"<<endl;
	int maxcount=int(lbrain.size()/10);
	int outcount=0;
	float pog=0.0;
	//left brain intrinsic connectivity calculation
	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *lptr = &lbrain[0];
	int *oldptr;
	//move pointer point to the first voxel of left brain on the output array
	out = out + (*lptr);
	count = count + (*lptr);
	for(int lb=0;lb<lbrain.size();lb++)//lptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;
		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &lbrain[0];
		for(int lbf=0;lbf<lbrain.size();lbf++)//lptrf counting
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;
			//inner loop pointer move to the beginning of the next voxel's time series
			IT *friendvoxel = innerloop+(*lptrf)*nc;
			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if(sum >= Threshold)
			{
				goodSum += sum*sum;
				++counter;
			}
			++lptrf;
		}
		//write results to output arrays
		*out=(IT)goodSum;
		*count=(float)counter;
		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;
		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));
		++outcount;
		/*if (outcount==maxcount)
		{
			pog=float(lb+1)/float(nt);
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(lb+1)/float(nt));
		}*/
		//self->UpdateProgress(float(lb+1)/float(nt));
		//std::cout<<"Newlv: "<<lbrain[lb];
	}//end of left brain
	//right brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count2=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *rptr = &rbrain[0];
	out2 = out2 + (*rptr);
	count2 = count2 + (*rptr);
	maxcount=int(rbrain.size()/10);
	outcount=0;
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;
		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &rbrain[0];
		for(int rbf=0;rbf<rbrain.size();rbf++)//lptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;
			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if(sum >= Threshold)
			{
				goodSum += sum*sum;
				++counter;
			}
			++rptrf;
		}
		*out2=(IT)goodSum;
		*count2=(float)counter;
		oldptr = rptr;
		++rptr;
		out2 = out2+ ((*rptr)-(*oldptr));
		count2= count2 + ((*rptr)-(*oldptr));
		//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
		++outcount;
		if (outcount==maxcount)
		{
			pog=(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size()));
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size())));
		}
		//self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
		//std::cout<<"Newrv: "<<rbrain[rb];
	}//end of right brain
}
template <class IT>
static void ConpositivePow(vtkbisIpsilateralConnMap *self,vtkImageData *inData,vtkImageData *outData,vtkImageData *OutData2,IT*,vector<int>& lbrain,vector<int>& rbrain,int nc,int nt,const float Threshold)
{
	cout<<"Contralateral, Positive, Sqr"<<endl;
	int maxcount=int(lbrain.size()/10);
	int outcount=0;
	float pog=0.0;
	//left to right brain intrinsic connectivity calculation
	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *lptr = &lbrain[0];
	int *oldptr;
	//move pointer point to the first voxel of left brain on the output array
	out = out + (*lptr);
	count = count + (*lptr);
	for(int lb=0;lb<lbrain.size();lb++)//lptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;
		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &rbrain[0];
		for(int lbf=0;lbf<rbrain.size();lbf++)//lptrf counting
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;
			//inner loop pointer move to the beginning of the next voxel's time series
			IT *friendvoxel = innerloop+(*lptrf)*nc;
			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if(sum >= Threshold)
			{
				goodSum += sum*sum;
				++counter;
			}
			++lptrf;
		}
		//write results to output arrays
		*out=(IT)goodSum;
		*count=(float)counter;
		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;
		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));
		/*++outcount;
		if (outcount==maxcount)
		{
			pog=float(lb+1)/float(nt);
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(lb+1)/float(nt));
		}*/
		//self->UpdateProgress(float(lb+1)/float(nt));
		//std::cout<<"Newlv: "<<lbrain[lb];
	}//end of left brain
	//right to left brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count2=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *rptr = &rbrain[0];
	out2 = out2 + (*rptr);
	count2 = count2 + (*rptr);
	maxcount=int(rbrain.size()/10);
	outcount=0;
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;
		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &lbrain[0];
		for(int rbf=0;rbf<lbrain.size();rbf++)//lptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;
			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if(sum >= Threshold)
			{
				goodSum += sum*sum;
				++counter;
			}
			++rptrf;
		}
		*out2=(IT)goodSum;
		*count2=(float)counter;
		oldptr = rptr;
		++rptr;
		out2 = out2+ ((*rptr)-(*oldptr));
		count2= count2 + ((*rptr)-(*oldptr));
		//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
		++outcount;
		if (outcount==maxcount)
		{
			pog=(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size()));
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size())));
		}
		//self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
		//std::cout<<"Newrv: "<<rbrain[rb];
	}//end of right brain
}
template <class IT>
static void negativeAbs(vtkbisIpsilateralConnMap *self,vtkImageData *inData,vtkImageData *outData,vtkImageData *OutData2,IT*,vector<int>& lbrain,vector<int>& rbrain,int nc,int nt,const float Threshold)
{
	cout<<"Ipsilateral, Negative, Abs"<<endl;
	int maxcount=int(lbrain.size()/10);
	int outcount=0;
	float pog=0.0;
	//left brain intrinsic connectivity calculation
	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *lptr = &lbrain[0];
	int *oldptr;
	//move pointer point to the first voxel of left brain on the output array
	out = out + (*lptr);
	count = count + (*lptr);
	for(int lb=0;lb<lbrain.size();lb++)//lptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;
		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &lbrain[0];
		for(int lbf=0;lbf<lbrain.size();lbf++)//lptrf counting
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;
			//inner loop pointer move to the beginning of the next voxel's time series
			IT *friendvoxel = innerloop+(*lptrf)*nc;
			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if(sum <= -Threshold)
			{
				sum = fabs(sum);
				goodSum += sum;
				++counter;
			}
			++lptrf;
		}
		//write results to output arrays
		*out=(IT)goodSum;
		*count=(float)counter;
		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;
		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));
		++outcount;
		/*if (outcount==maxcount)
		{
			pog=float(lb+1)/float(nt);
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(lb+1)/float(nt));
		}*/
		//self->UpdateProgress(float(lb+1)/float(nt));
		//std::cout<<"Newlv: "<<lbrain[lb];
	}//end of left brain
	//right brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count2=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *rptr = &rbrain[0];
	out2 = out2 + (*rptr);
	count2 = count2 + (*rptr);
	maxcount=int(rbrain.size()/10);
	outcount=0;
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;
		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &rbrain[0];
		for(int rbf=0;rbf<rbrain.size();rbf++)//lptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;
			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if(sum <= -Threshold)
			{
				sum = sum * -1;
				goodSum += sum;
				++counter;
			}
			++rptrf;
		}
		*out2=(IT)goodSum;
		*count2=(float)counter;
		oldptr = rptr;
		++rptr;
		out2 = out2 + ((*rptr)-(*oldptr));
		count2= count2 + ((*rptr)-(*oldptr));
		//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
		++outcount;
		if (outcount==maxcount)
		{
			pog=(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size()));
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size())));
		}
		//self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
		//std::cout<<"Newrv: "<<rbrain[rb];
	}//end of right brain
}
template <class IT>
static void ConnegativeAbs(vtkbisIpsilateralConnMap *self,vtkImageData *inData,vtkImageData *outData,vtkImageData *OutData2,IT*,vector<int>& lbrain,vector<int>& rbrain,int nc,int nt,const float Threshold)
{
	cout<<"Contralateral, Negative, Abs"<<endl;
	int maxcount=int(lbrain.size()/10);
	int outcount=0;
	float pog=0.0;
	//left to right brain intrinsic connectivity calculation
	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *lptr = &lbrain[0];
	int *oldptr;
	//move pointer point to the first voxel of left brain on the output array
	out = out + (*lptr);
	count = count + (*lptr);
	for(int lb=0;lb<lbrain.size();lb++)//lptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;
		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &rbrain[0];
		for(int lbf=0;lbf<rbrain.size();lbf++)//lptrf counting
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;
			//inner loop pointer move to the beginning of the next voxel's time series
			IT *friendvoxel = innerloop+(*lptrf)*nc;
			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if(sum <= -Threshold)
			{
				sum = sum * -1;
				goodSum += sum;
				++counter;
			}
			++lptrf;
		}
		//write results to output arrays
		*out=(IT)goodSum;
		*count=(float)counter;
		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;
		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));
		/*++outcount;
		if (outcount==maxcount)
		{
			pog=float(lb+1)/float(nt);
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(lb+1)/float(nt));
		}*/
		//self->UpdateProgress(float(lb+1)/float(nt));
		//std::cout<<"Newlv: "<<lbrain[lb];
	}//end of left brain
	//right to left brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count2=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *rptr = &rbrain[0];
	out2 = out2 + (*rptr);
	count2 = count2 + (*rptr);
	maxcount=int(rbrain.size()/10);
	outcount=0;
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;
		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &lbrain[0];
		for(int rbf=0;rbf<lbrain.size();rbf++)//lptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;
			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if(sum <= -Threshold)
			{
				sum = sum * -1;
				goodSum += sum;
				++counter;
			}
			++rptrf;
		}
		*out2=(IT)goodSum;
		*count2=(float)counter;
		oldptr = rptr;
		++rptr;
		out2 = out2 + ((*rptr)-(*oldptr));
		count2= count2 + ((*rptr)-(*oldptr));
		//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
		++outcount;
		if (outcount==maxcount)
		{
			pog=(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size()));
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size())));
		}
		//self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
		//std::cout<<"Newrv: "<<rbrain[rb];
	}//end of right brain
}
template <class IT>
static void negativePow(vtkbisIpsilateralConnMap *self,vtkImageData *inData,vtkImageData *outData,vtkImageData *OutData2,IT*,vector<int>& lbrain,vector<int>& rbrain,int nc,int nt,const float Threshold)
{
	cout<<"Ipsilateral, Negative, Pow"<<endl;
	int maxcount=int(lbrain.size()/10);
	int outcount=0;
	float pog=0.0;
	//left brain intrinsic connectivity calculation
	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *lptr = &lbrain[0];
	int *oldptr;
	//move pointer point to the first voxel of left brain on the output array
	out = out + (*lptr);
	count = count + (*lptr);
	for(int lb=0;lb<lbrain.size();lb++)//lptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;
		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &lbrain[0];
		for(int lbf=0;lbf<lbrain.size();lbf++)//lptrf counting
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;
			//inner loop pointer move to the beginning of the next voxel's time series
			IT *friendvoxel = innerloop+(*lptrf)*nc;
			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if (sum <= -Threshold)
			{
				goodSum += sum*sum;
				++counter;
			}
			++lptrf;
		}
		//write results to output arrays
		*out=(IT)goodSum;
		*count=(float)counter;
		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;
		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));
		++outcount;
		/*if (outcount==maxcount)
		{
			pog=float(lb+1)/float(nt);
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(lb+1)/float(nt));
		}*/
		//self->UpdateProgress(float(lb+1)/float(nt));
		//std::cout<<"Newlv: "<<lbrain[lb];
	}//end of left brain
	//right brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count2=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *rptr = &rbrain[0];
	out2 = out2 + (*rptr);
	count2 = count2 + (*rptr);
	maxcount=int(rbrain.size()/10);
	outcount=0;
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;
		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &rbrain[0];
		for(int rbf=0;rbf<rbrain.size();rbf++)//lptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;
			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if (sum <= -Threshold)
			{
				goodSum += sum*sum;
				++counter;
			}
			++rptrf;
		}
		*out2=(IT)goodSum;
		*count2=(float)counter;
		oldptr = rptr;
		++rptr;
		out2 = out2 + ((*rptr)-(*oldptr));
		count2= count2 + ((*rptr)-(*oldptr));
		//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
		++outcount;
		if (outcount==maxcount)
		{
			pog=(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size()));
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size())));
		}
		//self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
		//std::cout<<"Newrv: "<<rbrain[rb];
	}//end of right brain
}
template <class IT>
static void ConnegativePow(vtkbisIpsilateralConnMap *self,vtkImageData *inData,vtkImageData *outData,vtkImageData *OutData2,IT*,vector<int>& lbrain,vector<int>& rbrain,int nc,int nt,const float Threshold)
{
	cout<<"Contralateral, Negative, Sqr"<<endl;
	int maxcount=int(lbrain.size()/10);
	int outcount=0;
	float pog=0.0;
	//left to right brain intrinsic connectivity calculation
	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *lptr = &lbrain[0];
	int *oldptr;
	//move pointer point to the first voxel of left brain on the output array
	out = out + (*lptr);
	count = count + (*lptr);
	for(int lb=0;lb<lbrain.size();lb++)//lptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;
		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &rbrain[0];
		for(int lbf=0;lbf<rbrain.size();lbf++)//lptrf counting
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;
			//inner loop pointer move to the beginning of the next voxel's time series
			IT *friendvoxel = innerloop+(*lptrf)*nc;
			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if (sum <= -Threshold)
			{
				goodSum += sum*sum;
				++counter;
			}
			++lptrf;
		}
		//write results to output arrays
		*out=(IT)goodSum;
		*count=(float)counter;
		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;
		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));
		/*++outcount;
		if (outcount==maxcount)
		{
			pog=float(lb+1)/float(nt);
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(lb+1)/float(nt));
		}*/
		//self->UpdateProgress(float(lb+1)/float(nt));
		//std::cout<<"Newlv: "<<lbrain[lb];
	}//end of left brain
	//right to left brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count2=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *rptr = &rbrain[0];
	out2 = out2 + (*rptr);
	count2 = count2 + (*rptr);
	maxcount=int(rbrain.size()/10);
	outcount=0;
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;
		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &lbrain[0];
		for(int rbf=0;rbf<lbrain.size();rbf++)//lptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;
			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if (sum <= -Threshold)
			{
				goodSum += sum*sum;
				++counter;
			}
			++rptrf;
		}
		*out2=(IT)goodSum;
		*count2=(float)counter;
		oldptr = rptr;
		++rptr;
		out2 = out2 + ((*rptr)-(*oldptr));
		count2= count2 + ((*rptr)-(*oldptr));
		//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
		++outcount;
		if (outcount==maxcount)
		{
			pog=(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size()));
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size())));
		}
		//self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
		//std::cout<<"Newrv: "<<rbrain[rb];
	}//end of right brain
}
template <class IT>
static void bothAbs(vtkbisIpsilateralConnMap *self,vtkImageData *inData,vtkImageData *outData,vtkImageData *OutData2,IT*,vector<int>& lbrain,vector<int>& rbrain,int nc,int nt,const float Threshold)
{
	cout<<"Ipsilateral, Both, Abs"<<endl;
	int maxcount=int(lbrain.size()/10);
	int outcount=0;
	float pog=0.0;
	//left brain intrinsic connectivity calculation
	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *lptr = &lbrain[0];
	int *oldptr;
	//move pointer point to the first voxel of left brain on the output array
	out = out + (*lptr);
	count = count + (*lptr);
	for(int lb=0;lb<lbrain.size();lb++)//lptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;
		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &lbrain[0];
		for(int lbf=0;lbf<lbrain.size();lbf++)//lptrf counting
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;
			//inner loop pointer move to the beginning of the next voxel's time series
			IT *friendvoxel = innerloop+(*lptrf)*nc;
			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if (sum <= -Threshold || sum >= Threshold)
			{
				if(sum <0)
					sum = sum * -1;
				goodSum += sum;
				++counter;
			}
			++lptrf;
		}
		//write results to output arrays
		*out=(IT)goodSum;
		*count=(float)counter;
		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;
		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));
		++outcount;
		/*if (outcount==maxcount)
		{
			pog=float(lb+1)/float(nt);
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(lb+1)/float(nt));
		}*/
		//self->UpdateProgress(float(lb+1)/float(nt));
		//std::cout<<"Newlv: "<<lbrain[lb];
	}//end of left brain
	//right brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count2=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *rptr = &rbrain[0];
	out2 = out2+ (*rptr);
	count2 = count2 + (*rptr);
	maxcount=int(rbrain.size()/10);
	outcount=0;
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;
		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &rbrain[0];
		for(int rbf=0;rbf<rbrain.size();rbf++)//lptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;
			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if (sum <= -Threshold || sum >= Threshold)
			{
				if(sum <0)
					sum = sum * -1;
				goodSum += sum;
				++counter;
			}
			++rptrf;
		}
		*out2=(IT)goodSum;
		*count2=(float)counter;
		oldptr = rptr;
		++rptr;
		out2 = out2 + ((*rptr)-(*oldptr));
		count2= count2 + ((*rptr)-(*oldptr));
		//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
		++outcount;
		if (outcount==maxcount)
		{
			pog=(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size()));
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size())));
		}
		//self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
		//std::cout<<"Newrv: "<<rbrain[rb];
	}//end of right brain
}
template <class IT>
static void ConbothAbs(vtkbisIpsilateralConnMap *self,vtkImageData *inData,vtkImageData *outData,vtkImageData *OutData2,IT*,vector<int>& lbrain,vector<int>& rbrain,int nc,int nt,const float Threshold)
{
	cout<<"Contralateral, Both, Abs"<<endl;
	int maxcount=int(lbrain.size()/10);
	int outcount=0;
	float pog=0.0;
	//left to right brain intrinsic connectivity calculation
	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *lptr = &lbrain[0];
	int *oldptr;
	//move pointer point to the first voxel of left brain on the output array
	out = out + (*lptr);
	count = count + (*lptr);
	for(int lb=0;lb<lbrain.size();lb++)//lptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;
		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &rbrain[0];
		for(int lbf=0;lbf<rbrain.size();lbf++)//lptrf counting
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;
			//inner loop pointer move to the beginning of the next voxel's time series
			IT *friendvoxel = innerloop+(*lptrf)*nc;
			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if (sum <= -Threshold || sum >= Threshold)
			{
				if(sum <0)
					sum = sum * -1;
				goodSum += sum;
				++counter;
			}
			++lptrf;
		}
		//write results to output arrays
		*out=(IT)goodSum;
		*count=(float)counter;
		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;
		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));
		/*++outcount;
		if (outcount==maxcount)
		{
			pog=float(lb+1)/float(nt);
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(lb+1)/float(nt));
		}*/
		//self->UpdateProgress(float(lb+1)/float(nt));
		//std::cout<<"Newlv: "<<lbrain[lb];
	}//end of left brain
	//right to left brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count2=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *rptr = &rbrain[0];
	out2 = out2+ (*rptr);
	count2 = count2 + (*rptr);
	maxcount=int(rbrain.size()/10);
	outcount=0;
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;
		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &lbrain[0];
		for(int rbf=0;rbf<lbrain.size();rbf++)//lptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;
			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if (sum <= -Threshold || sum >= Threshold)
			{
				if(sum <0)
					sum = sum * -1;
				goodSum += sum;
				++counter;
			}
			++rptrf;
		}
		*out2=(IT)goodSum;
		*count2=(float)counter;
		oldptr = rptr;
		++rptr;
		out2 = out2 + ((*rptr)-(*oldptr));
		count2= count2 + ((*rptr)-(*oldptr));
		//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
		++outcount;
		if (outcount==maxcount)
		{
			pog=(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size()));
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size())));
		}
		//self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
		//std::cout<<"Newrv: "<<rbrain[rb];
	}//end of right brain
}
template <class IT>
static void bothPow(vtkbisIpsilateralConnMap *self,vtkImageData *inData,vtkImageData *outData,vtkImageData *OutData2,IT*,vector<int>& lbrain,vector<int>& rbrain,int nc,int nt,const float Threshold)
{
	cout<<"Ipsilateral, Both, Sqr"<<endl;
	int maxcount=int(lbrain.size()/10);
	int outcount=0;
	float pog=0.0;
	//left brain intrinsic connectivity calculation
	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *lptr = &lbrain[0];
	int *oldptr;
	//move pointer point to the first voxel of left brain on the output array
	out = out + (*lptr);
	count = count + (*lptr);
	for(int lb=0;lb<lbrain.size();lb++)//lptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;
		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &lbrain[0];
		for(int lbf=0;lbf<lbrain.size();lbf++)//lptrf counting
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;
			//inner loop pointer move to the beginning of the next voxel's time series
			IT *friendvoxel = innerloop+(*lptrf)*nc;
			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if(sum <= -Threshold || sum >= Threshold)
			{
				goodSum += sum*sum;
				//cout<<sum<<"*"<<sum;
				++counter;
			}
			++lptrf;
			//cout<<"+";
		}
		//cout<<"lS:"<<goodSum<<"/"<<lbrain.size()<<"=";
		//write results to output arrays
		*out=(IT)goodSum;
		//cout<<*out<<endl;
		*count=(float)counter;
		//cout<<"counter:"<<counter<<"N:"<<*count<<endl;
		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;
		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));
		//++outcount;
		//if (outcount==maxcount)
		//{
			//pog=(float(lb+1)/float(lbrain.size()))/2;
			//self->UpdateProgress(pog);
			//outcount=0;
			//fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(lb+1)/float(lbrain.size()))/2);
		//}
		//self->UpdateProgress(float(lb+1)/float(nt));
		//std::cout<<"Newlv: "<<lbrain[lb];
	}//end of left brain
	//right brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count2=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *rptr = &rbrain[0];
	out2 = out2 + (*rptr);
	count2 = count2 + (*rptr);
	maxcount=int(rbrain.size()/10);
	outcount=0;
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;
		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &rbrain[0];
		for(int rbf=0;rbf<rbrain.size();rbf++)//lptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;
			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if(sum <= -Threshold || sum >= Threshold)
			{
				goodSum += sum*sum;
				//cout<<sum<<"*"<<sum;
				++counter;
			}
			++rptrf;
			//cout<<"+";
		}
		//cout<<"=rS"<<goodSum<<"/"<<rbrain.size()<<"=";
		*out2=(IT)goodSum;
		//cout<<*out<<endl;
		*count2=(float)counter;
		//cout<<"counter:"<<counter<<"N:"<<*count<<endl;
		oldptr = rptr;
		++rptr;
		out2= out2 + ((*rptr)-(*oldptr));
		count2= count2 + ((*rptr)-(*oldptr));
		//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
		++outcount;
		if (outcount==maxcount)
		{
			pog=(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size()));
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size())));
		}
		//self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
		//std::cout<<"Newrv: "<<rbrain[rb];
	}//end of right brain
}
template <class IT>
static void ConbothPow(vtkbisIpsilateralConnMap *self,vtkImageData *inData,vtkImageData *outData,vtkImageData *OutData2,IT*,vector<int>& lbrain,vector<int>& rbrain,int nc,int nt,const float Threshold)
{
	cout<<"Contralateral, Both, Sqr"<<endl;
	int maxcount=int(lbrain.size()/10);
	int outcount=0;
	float pog=0.0;
	//left to right brain intrinsic connectivity calculation
	//make the pointers point to the beginning of the output array
	IT *out = (IT*)  outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *lptr = &lbrain[0];
	int *oldptr;
	//move pointer point to the first voxel of left brain on the output array
	out = out + (*lptr);
	count = count + (*lptr);
	for(int lb=0;lb<lbrain.size();lb++)//lptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		//move current voxel pointer point to the beginning of a voxel on left brain
		IT *outerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*lptr)*nc;
		//inner loop pointer point to the beginning of the input array
		IT *innerloop = (IT*)inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *lptrf = &rbrain[0];
		for(int lbf=0;lbf<rbrain.size();lbf++)//lptrf counting
		{
			//thisvoxel pointer move back to the beginning of current voxel
			IT *thisvoxel = outerloop;
			//inner loop pointer move to the beginning of the next voxel's time series
			IT *friendvoxel = innerloop+(*lptrf)*nc;
			float sum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*lptr<<"*"<<*lptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if(sum <= -Threshold || sum >= Threshold)
			{
				goodSum += sum*sum;
				//cout<<sum<<"*"<<sum;
				++counter;
			}
			++lptrf;
			//cout<<"+";
		}
		//cout<<"lS:"<<goodSum<<"/"<<lbrain.size()<<"=";
		//write results to output arrays
		*out=(IT)goodSum;
		//cout<<*out<<endl;
		*count=(float)counter;
		//cout<<"counter:"<<counter<<"N:"<<*count<<endl;
		//increment outerloop(current voxel) pointer
		oldptr = lptr;
		++lptr;
		//output pointer move to the next voxel on the left brain
		out = out + ((*lptr)-(*oldptr));
		count= count + ((*lptr)-(*oldptr));
		++outcount;
		/*if (outcount==maxcount)
		{
		    pog=float(lb+1)/float(nt);
		    self->UpdateProgress(pog);
		    outcount=0;
		    fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,float(lb+1)/float(nt));
		}*/
		//self->UpdateProgress(float(lb+1)/float(nt));
		//std::cout<<"Newlv: "<<lbrain[lb];
	}//end of left brain
	//right to left brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	float* count2=(float*) OutData2->GetPointData()->GetScalars()->GetVoidPointer(0);
	int *rptr = &rbrain[0];
	out2 = out2 + (*rptr);
	count2 = count2 + (*rptr);
	maxcount=int(rbrain.size()/10);
	outcount=0;

	//xilin added this
//	float sum_volume = 0;
//	float sum2_volume = 0;
	//end of xilin's addition
	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
		float goodSum = 0.0;
		int counter = 0;
		IT *outerloop = (IT*) inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		outerloop=outerloop+(*rptr)*nc;
		IT *innerloop = (IT*)  inData->GetPointData()->GetScalars()->GetVoidPointer(0);
		int *rptrf = &lbrain[0];
		for(int rbf=0;rbf<lbrain.size();rbf++)//lptrf counting
		{
			IT *thisvoxel = outerloop;
			IT *friendvoxel = innerloop+(*rptrf)*nc;
			float sum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				sum += (*thisvoxel)*(*friendvoxel);
				//std::cout<<*rptr<<"*"<<*rptrf<<"="<<*thisvoxel<<"*"<<*friendvoxel<<"="<<sum<<endl;
				++thisvoxel;
				++friendvoxel;
			}
			sum/=nc;
			if(sum <= -Threshold || sum >= Threshold)
			{
				goodSum += sum*sum;
				//cout<<sum<<"*"<<sum;
				++counter;
			}
			++rptrf;
			//cout<<"+";
		}
		//cout<<"=rS"<<goodSum<<"/"<<rbrain.size()<<"=";

		//xilin added these lines
//		sum_volume += goodSum;
//		sum2_volume +=  goodSum*goodSum;
		//end of xilin's addtion

		*out2=(IT)goodSum;
		//cout<<*out<<endl;
		*count2=(float)counter;
		//cout<<"counter:"<<counter<<"N:"<<*count<<endl;
		oldptr = rptr;
		++rptr;
		out2= out2 + ((*rptr)-(*oldptr));
		count2= count2 + ((*rptr)-(*oldptr));
		//std::cout<<(*rptr)<<"-"<<(*oldptr)<<endl;
		++outcount;
		if (outcount==maxcount)
		{
			pog=(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size()));
			self->UpdateProgress(pog);
			outcount=0;
			fprintf(stderr," %.2f/100 (%.2f good) ",100.0*pog,(float(rb+1)/float(rbrain.size()))+(lbrain.size()/(lbrain.size()+rbrain.size())));
		}
		//self->UpdateProgress((float(rb+1)+lbrain.size())/float(nt));
		//std::cout<<"Newrv: "<<rbrain[rb];
	}//end of right brain


	// xilin added these lines
	//right to left brain intrinsic connectivity calculation
	//make the pointers go back to the beginning of the output array
/*	int *rptr = &rbrain[0];
	IT* out2 =(IT*) outData->GetPointData()->GetScalars()->GetVoidPointer(0);
	out2 = out2 + (*rptr);

	float sum_avg = sum_volume/lbrain.size();
	float sum_sig = sum2_volume/lbrain.size()- sum_avg*sum_avg;

	for(int rb=0;rb<rbrain.size();rb++)//rptr counting
	{
	  *out2 = (IT)(*out2-sum_avg)/sum_sig;
	}

*/
}


template <class IT>
static void vtkbisIpsilateralConnMapDoConnectivity1(vtkbisIpsilateralConnMap *self,
						  vtkImageData *inData,
						  vtkImageData *outData,
						  vtkImageData *OutData2,
						  vtkImageData *maskData,
						  IT* )
{
	switch (maskData->GetScalarType())
	{
		vtkTemplateMacro7(vtkbisIpsilateralConnMapDoConnectivity,self
				,inData,outData,OutData2
				,maskData,static_cast<IT *>(0),static_cast<VTK_TT *>(0));
		default:
			vtkGenericWarningMacro("Execute: Unknown input ScalarType");
        return;
    }
}
//-------------------------------------------------------------------------------
//Old not optimized simpleExecute code
//-------------------------------------------------------------------------------
void vtkbisIpsilateralConnMap::OldExecute(vtkImageData* original_input ,vtkImageData* output)
{
	fprintf(stderr,"beginning vtkbisIpsilateralConnMap Old method");

/*	vtkImageCast* cast=vtkImageCast::New();
	cast->SetInput(original_input);
	cast->SetOutputScalarTypeToFloat();
	cast->Update();

	vtkImageData* inp=vtkImageData::New();
	inp->ShallowCopy(cast->GetOutput());
	cast->Delete();
*/
	vtkbisImageTimeSeriesNormalize* norm1=vtkbisImageTimeSeriesNormalize::New();
	norm1->SetInput(original_input);
	norm1->Update();

	vtkImageData* inp=vtkImageData::New();
	inp->ShallowCopy(norm1->GetOutput());
	norm1->Delete();

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
			fprintf(stderr,"Bad Mask Input to vtkbisIpsilateralConnMap SimpleExecute\n");
			return;
		}
	}

	vtkDataArray* temp=inp->GetPointData()->GetScalars();
	int nt=temp->GetNumberOfTuples();//number of voxels
	int nc=temp->GetNumberOfComponents();//number of frames (time series)

	vtkDataArray* mask = NULL;
	if (this->ImageMask!=NULL)
		mask=this->ImageMask->GetPointData()->GetScalars();

	vtkDataArray* out=output->GetPointData()->GetScalars();
	out->FillComponent(0,0.0);

	this->OutData2=vtkImageData::New();
	this->OutData2->SetOrigin(output->GetOrigin());
	this->OutData2->SetSpacing(output->GetSpacing());
	this->OutData2->SetDimensions(output->GetDimensions());
	this->OutData2->SetScalarTypeToFloat();
	this->OutData2->AllocateScalars();//allocation space on memery
	vtkDataArray* out2 = this->OutData2->GetPointData()->GetScalars();
	out2->FillComponent(0,0.0);

	//normalize time series voxel by voxel
/*	for (int i=0;i< nt;i++)
	{
		float psum = 0;
		int DoGlm=1;
		if (mask!=NULL)
		{
			if (mask->GetComponent(i,0)<1)
				DoGlm=0;
		}
		if(DoGlm)
		{
			for (int m = 0; m < nc; m++)
			{
				psum += temp->GetComponent(i,m)*temp->GetComponent(i,m);//GetComponent(voxel,m) Return the data component at the ith tuple and mth component
			}
			float norm = sqrt(psum);
			if (norm != 0)
			{
				for (int m2 = 0; m2 < nc; m2++)
				{
					float ns = temp->GetComponent(i,m2) / norm;
					//cout<<temp->GetComponent(i,m2)<<"/"<<norm<<"="<<ns<<";";
					temp->SetComponent(i,m2,ns);
					//cout<<temp->GetComponent(i,m2)<<";";
				}
			}
		 }
	  }
*/
	double position[3],spacing[3];
	vector<int> lbrain,rbrain;
	original_input->GetSpacing(spacing);
	//sort voxel to left or right brain vector
	for(int t= 0; t < nt; t++)
	{
		int DoGlm=1;
		if (mask!=NULL)
		{
			if(mask->GetComponent(t,0) < 1)
				DoGlm=0;
		}
		if(DoGlm)
		{
			original_input->GetPoint(t,position);
			if( position[0]/spacing[0] > (dim[0]-1)/2 )
			{
				lbrain.push_back(t);
				//std::cout<<"lb:"<<t;
		    }
		    else
		    {
			    rbrain.push_back(t);
			    //std::cout<<"rb"<<t;
		    }
	    }
    }
	//cout<<"oldwaywithmask lbrain:"<<lbrain.size()<<"rbrain:"<<rbrain.size()<<endl;
	this->UpdateProgress(0.01);
	//-----------------------------------------------------
	//left brain intrinsic connectivity calculation
	//-----------------------------------------------------
	//cout<<"size"<<lbrain.size()<<endl;
	for(int lb=0;lb<lbrain.size();lb++)
	{
		float lgoodSum = 0.0;
		int counter = 0;
		for(int lbf=0;lbf<lbrain.size();lbf++)
		{
			float lsum = 0;
			for(int lm=0;lm<nc;lm++)
			{
				lsum += temp->GetComponent(lbrain[lb],lm)*temp->GetComponent(lbrain[lbf],lm);
				//cout<<lbrain[lb]<<"*"<<lbrain[lbf]<<"="<<temp->GetComponent(lbrain[lb],lm)<<"*"<<temp->GetComponent(lbrain[lbf],lm)<<"="<<lsum<<endl;
			}
			//lsum/nc if use Xenios normalize code
			lsum/=nc;

			lgoodSum +=this->CalculateGoodSum(lsum,counter);
			//cout<<lbrain[lb]<<"to"<<lbrain[lbf]<<"="<<lsum<<","<<counter<<";";
		}
		out2->SetComponent(lbrain[lb],0,(float)counter/lbrain.size());
		//cout<<lbrain[lb]<<"N:"<<out2->GetComponent(lbrain[lb],0)<<";";
		out->SetComponent(lbrain[lb],0,lgoodSum/lbrain.size());
		//cout<<"S:"<<out->GetComponent(lbrain[lb],0)<<endl;
		//std::cout<<"lv: "<<lbrain[lb];
	}
	//---------------------------------------------------------
	//right brain intrinsic connectivity calculation
	//---------------------------------------------------------
	for(int rb=0;rb<rbrain.size();rb++)
	{
		int counter = 0;
		float rgoodSum = 0;
		for(int rbf=0;rbf<rbrain.size();rbf++)
		{
			float rsum = 0;
			for(int rm=0;rm<nc;rm++)
			{
				rsum += temp->GetComponent(rbrain[rb],rm)*temp->GetComponent(rbrain[rbf],rm);
			}
			//rsum/nc if use Xenios normalize code
			rsum/=nc;

			rgoodSum +=this->CalculateGoodSum(rsum,counter);
		}
		out2->SetComponent(rbrain[rb],0,(float)counter/rbrain.size());
		out->SetComponent(rbrain[rb],0,rgoodSum/rbrain.size());
		//std::cout<<" rv:"<<rbrain[rb];
	}
	this->UpdateProgress(1.0);
}//end of old execute

// ---------------------------------------------------------------------------------------------------------------------------
//   Main Function
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisIpsilateralConnMap::SimpleExecute(vtkImageData* original_input ,vtkImageData* output)
{
	//fprintf(stderr," beginning vtkbisIpsilateralConnMap");
	if (this->Optimize == 0)
	  {
	    fprintf(stderr,"Running Non Optimized Code\n");
	    this->OldExecute(original_input,output);
	    return;
	  }


	int dim[3];
	int dimMask[3];
	int ia;
	int tsum = 0;


	vtkbisImageTimeSeriesNormalize* norm1=vtkbisImageTimeSeriesNormalize::New();
	norm1->SetInput(original_input);
	norm1->Update();

	vtkImageData* inp=vtkImageData::New();
	inp->ShallowCopy(norm1->GetOutput());
	norm1->Delete();


	inp->GetDimensions(dim);
	if (this->ImageMask!=NULL)
	{
		this->ImageMask->GetDimensions(dimMask);
		for (ia=0;ia<=2;ia++)
			tsum += abs(dim[ia]-dimMask[ia]);
		if (tsum>0)
		{
			fprintf(stderr,"Bad Mask Input to vtkbisIpsilateralConnMap SimpleExecute\n");
			return;
		}
	}

	if (this->OutData2!=NULL)
	  this->OutData2->Delete();

	this->OutData2=vtkImageData::New();
	this->OutData2->SetOrigin(output->GetOrigin());
	this->OutData2->SetSpacing(output->GetSpacing());
	this->OutData2->SetDimensions(output->GetDimensions());
	this->OutData2->SetScalarTypeToFloat();
	this->OutData2->AllocateScalars();//allocation space on memery


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

	//fprintf(stderr," doContralater=%d use type=%d threshold=%.2f range=%d abs=%d optimize=%d\n",
	 // 		  this->DoContralateral,this->Type,this->Threshold,this->Range,this->Abs,this->Optimize);

	this->UpdateProgress(0.01);
	time_t start,end;
	time(&start);

	if(this->ImageMask!=NULL)
	{
		switch (inp->GetScalarType())
		{
		vtkTemplateMacro6(vtkbisIpsilateralConnMapDoConnectivity1,this,
				inp,output, this->OutData2,
				this->ImageMask,static_cast<VTK_TT *>(0));
		default:
			vtkErrorMacro(<< "Execute: Unknown ScalarType");
		return;
		}
	}else{
		switch (inp->GetScalarType())
		{
		vtkTemplateMacro5(vtkbisIpsilateralConnMapDoConnectivity2,this,
						inp, output, this->OutData2,
						static_cast<VTK_TT *>(0));
		default:
			vtkErrorMacro(<< "Execute: Unknown ScalarType");
		return;
		}
	}

/*	if (this->FileName=="")
	  return;

	std::ofstream myfile(this->FileName.c_str());
	if(!myfile.is_open())
	  {
	    cout<< "Unable to open file" << this->FileName.c_str();
	    return;
	  }
	myfile<<"Left Brain Mean: \t "<<this->LMean<<"\t"<<"Left Brain Standard Deviation: \t "<<this->LStd<<"Left Brain Number of Voxels: \t "<<this->LSize<<"\n";
	myfile<<"Right Brain Mean: \t "<<this->RMean<<"\t"<<"Right Brain Standard Deviation: \t "<<this->RStd<<"Right Brain Number of Voxels: \t "<<this->RSize<<"\n";
	if (Threshold != 0)
	  {
	    myfile<<"Left Brain Degree Mean: \t "<<this->LDegreeMean<<"\t"<<"Left Brain Degree Standard Deviation: \t "<<this->LDegreeStd<<"\n";
	    myfile<<"Right Brain Degree Mean: \t "<<this->RDegreeMean<<"\t"<<"Right Brain Degree Standard Deviation: \t "<<this->RDegreeStd<<"\n";
	  }
	myfile.close();
	*/
	//normalize brain frame by frame

	/*	vtkDataArray* dat=output->GetPointData()->GetScalars();
		int nt = dat->GetNumberOfTuples();
		if (!this->OutputRaw)
		{
		std::cout<<"I am here to output Z"<<endl;
		for (int i=0;i<nt;i++)
		{
		dat->SetComponent(i,0,vtkbisTimeSeriesCorrelation::RhoToZConversion(dat->GetComponent(i,0)));
		}
		}
	*/
	time(&end);

	std::cout<<endl<<"elapsed time "<<difftime(end, start)<<" seconds"<<endl;
	this->UpdateProgress(1.0);
}








