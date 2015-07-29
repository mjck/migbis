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
 * vtkbisTaskBasedSeedCorrelation.cpp
 *
 *  Created on: Jul 23, 2009
 *      Author: Isabella Murphy,Xilin Shen
 */

#include <vtkbisTaskBasedSeedCorrelation.h>
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkbisConnectivityUtility.h"
#include "vtkbisTimeSeriesCorrelation.h"
#include "vtkbisImageTimeSeriesNormalize.h"
#include "vtkImageCast.h"
#include <math.h>
#include "pxisinf.h"

using namespace std;

vtkbisTaskBasedSeedCorrelation* vtkbisTaskBasedSeedCorrelation::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisTaskBasedSeedCorrelation");
  if(ret)
    {
      return (vtkbisTaskBasedSeedCorrelation*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisTaskBasedSeedCorrelation;
}


vtkbisTaskBasedSeedCorrelation::vtkbisTaskBasedSeedCorrelation() {
	this->ImageROI=NULL;
	this->NumberOfRegions=0;
	this->ImageMask=NULL;
	this->Matrix=NULL;
	this->Task=0;
	this->AddRegion=0;
	this->DoText = 0;

}

vtkbisTaskBasedSeedCorrelation::~vtkbisTaskBasedSeedCorrelation() {
	this->SetImageROI(NULL);
	this->SetImageMask(NULL);
	this->SetMatrix(NULL);
}
void vtkbisTaskBasedSeedCorrelation::SetFileName(const char* filename)
{
  this->FileName=filename;
}

const char* vtkbisTaskBasedSeedCorrelation::GetFileName()
{
  return this->FileName.c_str();
}

//----------------------------------------------------------------------------------------------------------------------
//RequestInformation
//I. Our vtkImageData input has number of components = number of frames but output vtkImageData object has number of
//components = roint.
//II. Input and Output has different number of voxels, has to let the pipeline know it (change extend)
//need to call RequstInformation to set those differences.
// ---------------------------------------------------------------------------------------------------------------------------
int vtkbisTaskBasedSeedCorrelation::RequestInformation (vtkInformation * request,
			vtkInformationVector ** inputVector,
			vtkInformationVector * outputVector)
{
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	vtkDataArray* roi=NULL;
	if (this->ImageROI!=NULL)
	{
		roi=this->ImageROI->GetPointData()->GetScalars();
	}else{
		std::cout<<"bad input for vtkbisROICorrelation: no average ROI image"<<endl;
		return 0;
	}

	double range[2];  roi->GetRange(range);
	int roint = int(range[1]);//number of regions

	if(this->AddRegion >0)
	{
		roint+=(int)this->AddRegion;
		this->NumberOfRegions=roint;
	}

	vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, roint);//set number of components to roint

	return 1;
}
/*
 * @Input[inp]: 4D functional image
 * @Input[roimean]: 4D ROI average time course, regions*1*1*N
 * @Input[msk]: mask
 * @Input[nt]: number of tuples
 * @Input[startframe]: first frame of this block
 * @Input[endframe]: last frame of this block
 * @output[out]: sum of correlation(z value) of all the blocks
 * For example, correlation of voxel 0 to each ROI (say 0-10) is stored in output vtkImagedata voxel 0 component 0-10;
 * correlation of voxel 1 to each ROI (say 0-10) is stored in output vtkImageData voxel 1 component 0-10 and so on.
 * In the output image, xyz=input dimension, component=number of ROI regions
 */
void vtkbisTaskBasedSeedCorrelation::CalculateSeedCorrelation(vtkFloatArray* inp,vtkFloatArray* roimean,vtkDataArray* msk,int nt,int startframe,int endframe,vtkDataArray* out)
{
	double scalefactor=1.0/(double)(endframe-startframe+1);

	//normalize input
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
			double sum=0.0,sum2=0.0;
			for(int j=startframe;j<=endframe;j++)
			{
				double v=inp->GetComponent(i,j);
				//std::cout<<"frame:"<<j<<"v:"<<v;
				sum+=v;
				sum2+=v*v;
			}
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
			for(int j=startframe;j<=endframe;j++)
			{
				//double x = (inp->GetComponent(i,j)-mean)/sigma;
				//std::cout<<"frame2:"<<j<<"x"<<x<<endl;
				inp->SetComponent(i,j,(inp->GetComponent(i,j)-mean)/sigma);
				//sumx+=x*x;
			}
			//std::cout<<"v:"<<i<<"sumx"<<sumx<<endl;
		}
	}

	//std::cout<<"FISNISH ONE BLOCK!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;

	//for(int f=startframe;f<=endframe;f++)
		//std::cout<<"f:"<<f<<"x:"<<inp->GetComponent(15093,f)<<"......";


	for (int voxel = 0; voxel<nt;voxel++)
	{
		int doAlg=1;
	    if (msk!=NULL)
	    {
	    	if (msk->GetComponent(voxel,0)<1)
	    		doAlg=0;
	    }
	    if (doAlg)
	    {
	    	for (int roiindex = 0; roiindex<this->NumberOfRegions; roiindex++)
	    	{
	    		float sumx =0;
	    		float sumy =0;
	    		double sum=0.0;double gValue = 0;
	    		for (int i=startframe;i<=endframe;i++)
	    		{

	    			float x=inp->GetComponent(voxel,i);
	    			/*if(voxel==15093)
	    			{
	    				   //std::cout<<"fagain:"<<i<<"xagain:"<<x<<"........";
	    			}*/
	    		    float y=roimean->GetComponent(roiindex,i);
	    		    //std::cout<<"("<<roiindex<<","<<i<<")"<<y<<endl;
	    		    float xx = x*x;
	    		    float yy = y*y;
	    		    //std::cout<<"x:"<<x<<"y:"<<y<<"|";
	    		    sum+=x*y;
	    		    sumx+= xx;
	    		    sumy+= yy;

	    		}
	    		//one block's rho/# of frames in that block
	    		double rho=sum*scalefactor;
	    		//std::cout<<"start:"<<startframe<<"endframe"<<endframe<<endl;
	    		//std::cout<<"("<<voxel<<","<<roiindex<<")"<<"sumx"<<sumx<<"sumy"<<sumy<<"sum:"<<sum<<"r:"<<rho<<endl;
	    		//store sum of z's of each block to out
	    		if (fabs(rho-1.0)>0.00001)
	    		{
	    			/*if((1+rho)/(1-rho)<=0)
	    			{
	    				std::cout<<rho<<log((1+rho)/(1-rho));
	    			}*/

	    			gValue = 0.5*log((1+rho)/(1-rho));
	    				    		//std::cout<<gValue;
	    		}

	    		//std::cout<<out->GetComponent(voxel,roiindex);

	    		out->SetComponent(voxel,roiindex,out->GetComponent(voxel,roiindex)+gValue);
	    		//std::cout<<out->GetComponent(voxel,roiindex);
	    	  }
	      }
	   }
}
/*
 * @Input[inp]: 4D functional image
 * @Input[roi]: ROI(Region of Interest) 3D image
 * @Input[msk]: mask
 * @Input[nt]: number of tuples
 * @Input[startframe]:first frame of this block
 * @Input[endframe]:last frame of this block
 * @output[roimean]: average time courses in each ROI region.Dimension: number of regions*1*1*N
 * Assume user name each region using integer
 * In output vtkImageData object, voxel 0 = region 0, voxel 10 = region 10 and so on.
 * dimension is number of regions*1*1*N
 */
void vtkbisTaskBasedSeedCorrelation::CalculateROIMean(vtkFloatArray* inp,vtkFloatArray* roi,vtkDataArray* msk,vtkFloatArray* roimean,int nt,int nc,int startframe,int endframe)
{
	//std::cout<<"number of regions in roimean:"<<this->NumberOfRegions<<endl;
	int* num=new int[this->NumberOfRegions ];
	for(int ia=0;ia<this->NumberOfRegions;ia++)
		num[ia]=0;
	double scalefactor=1.0/(double)(endframe-startframe+1);

	for (int i=0;i<nt;i++)
	{
		int doAlg=1;
	    if (msk!=NULL)
	    {
	    	if (msk->GetComponent(i,0)<1)
	    		doAlg=0;
	    }
	    if (doAlg)
	    {
	    	int regionLabel = (int)roi->GetComponent(i,0)-1;

	    	if (regionLabel >=0 )
	    	{

	    		for(int j=startframe;j<=endframe;j++)
	    		{
	    			//std::cout<<"region:"<<regionLabel<<"frame:"<<j<<"data1:"<<roimean->GetComponent(regionLabel,j)<<"data2:"<<inp->GetComponent(i,j)<<"sum:"<<roimean->GetComponent(regionLabel,j)+inp->GetComponent(i,j);
	    			roimean->SetComponent(regionLabel,j,roimean->GetComponent(regionLabel,j)+inp->GetComponent(i,j));
	    			//std::cout<<"output:"<<roimean->GetComponent(regionLabel,j)<<endl;
	    		}
	    		num[regionLabel]+=1;
	    	}
	     }
	}

	for (int region=0;region<this->NumberOfRegions;region++)
	{
		if (num[region] >= 1)
		{
			double sum=0.0,sum2=0.0;
			for (int j=startframe;j<=endframe;j++)
			{
				//std::cout<<"region:"<<region<<"frame:"<<j<<"data:"<<roimean->GetComponent(region,j)<<"num:"<<num[region];
				double v=roimean->GetComponent(region,j)/num[region];
				roimean->SetComponent(region,j,v);
				sum+=v;
				sum2+=v*v;
				//std::cout<<"("<<region<<","<<j<<")= "<<roimean->GetComponent(region,j)<<endl;
			}
			double mean=sum*scalefactor;
			double sigma=sqrt(sum2*scalefactor-mean*mean);
			if (sigma<0.00001)
				sigma=1.0;
			if (isnan(sigma) || isinf(sigma))
				sigma=1.0;
			if (isnan(mean) || isinf(mean))
				mean=0.0;
			for (int j=startframe;j<=endframe;j++)
			{
				//std::cout<<"region:"<<region<<"frame:"<<j<<"data:"<<roimean->GetComponent(region,j)<<"num:"<<num[region];
				roimean->SetComponent(region,j,(roimean->GetComponent(region,j)-mean)/sigma);
				//std::cout<<"("<<region<<","<<j<<")= "<<roimean->GetComponent(region,j)<<endl;
			}

		}
	}
	if(this->DoText == 1 )
	  {
	      if(this->NumberOfRegions > 400 || nc > 500 )
	      {
	    	  fprintf(stderr, "Number of frames > 500. Too much data to export.");
	    	  return;
	      }

	      std::ofstream myfile(this->FileName.c_str());
	      if(!myfile.is_open())
	      {
	    	  cout<< "Unable to open file"<<this->FileName.c_str()<<"\n";
	    	  return;
	      }
	      myfile<<"Frame \t ";
	      for (int ia=0;ia<this->NumberOfRegions;ia++)
	    	  myfile<<"ROI "<< ia+1 <<"\t";
	      myfile<<"\n";
	      for(int i = 0; i < nc;i++)
	      {
	    	  myfile<<i<<"\t";
	    	  for (int j = 0; j<this->NumberOfRegions;j++)
	    	  {
	    		  myfile<<roimean->GetComponent(j,i)<< "\t";
	    	  }
	    	  myfile<<"\n";
	      }
	      myfile.close();
	  }

	delete [] num;
}
void vtkbisTaskBasedSeedCorrelation::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{
	fprintf(stderr,"Beginning vtkbisTaskBasedSeedCorrelation\n");
	int dim2[3]; input->GetDimensions(dim2);
	if (this->ImageROI==NULL)
		{
		   fprintf(stderr,"bad input to vtkbisTaskBasedSeedCorrelation, no ROI image.\n");
		   exit(0);
		} else {
			int dim[3]; this->ImageROI->GetDimensions(dim);
			int sum=0;
			for (int ia=0;ia<=2;ia++)
				sum+=abs(dim[ia]-dim2[ia]);
			if (sum>0)
			{
				fprintf(stderr,"Bad ROI Input to vtkbisTaskBasedSeedCorrelation SimpleExecute\n");
				return;
			 }
		}

		if (this->ImageMask!=NULL)
		{
			int dim3[3]; this->ImageMask->GetDimensions(dim3);
		    int sum=0;
		    for (int ia=0;ia<=2;ia++)
		    	sum+=abs(dim3[ia]-dim2[ia]);
		    if (sum>0)
		    {
		    	fprintf(stderr,"Bad Mask Input to vtkbisTaskBasedSeedCorrelation SimpleExecute\n");
			    return;
			}
		}

		vtkImageCast* cast=vtkImageCast::New();
		cast->SetInput(input);
		cast->SetOutputScalarTypeToFloat();
		cast->Update();

		vtkImageData* temp=vtkImageData::New();
		temp->ShallowCopy(cast->GetOutput());
		cast->Delete();

		vtkFloatArray* inp=(vtkFloatArray*)temp->GetPointData()->GetScalars();
		int nt=inp->GetNumberOfTuples();//number of voxels
		int nc=inp->GetNumberOfComponents();//number of frames

		vtkFloatArray* roi=NULL;
		if (this->ImageROI!=NULL)
			roi=(vtkFloatArray*)this->ImageROI->GetPointData()->GetScalars();
		double range[2];  roi->GetRange(range);
		//int minroi=int(range[0])+1;
		this->NumberOfRegions = int(range[1]);//number of regions

		if(this->AddRegion > 0)
		{
			this->NumberOfRegions+=(int)this->AddRegion;
		}
		std::cout<<"final number of regions:"<<this->NumberOfRegions<<" number of regions added:"<<this->AddRegion<<endl;
		vtkDataArray* out=output->GetPointData()->GetScalars();
		for (int ia=0;ia<this->NumberOfRegions;ia++)
			out->FillComponent(ia,0.0);


		vtkDataArray* msk=NULL;
		if (this->ImageMask!=NULL)
			msk=this->ImageMask->GetPointData()->GetScalars();

		vtkImageData* AverageROI =vtkImageData::New();
		AverageROI->SetExtent(0,this->NumberOfRegions-1,0,0,0,0);
		AverageROI->SetNumberOfScalarComponents(nc);
		AverageROI->SetScalarTypeToFloat();
		AverageROI->AllocateScalars();
		vtkFloatArray* roimean=(vtkFloatArray*)AverageROI->GetPointData()->GetScalars();
		for(int ia=0;ia<nc;ia++)
			roimean->FillComponent(ia,0.0);

		int numrows[2];  this->Matrix->GetSize(numrows);
		vtkIntArray* blockarray = vtkIntArray::New();
		int numblocks=vtkbisConnectivityUtility::SplitBlocks(this->Matrix,this->Task,numrows[0],blockarray);
		//std::cout<<"(This information is for Isabella only)number of blocks of task "<<this->Task<<" is "<<numblocks<<endl;

		//1.calculate roi mean for each block, normalize on site(within a block), save in the big roimean array
		int block=0;int sframe=0;
		for (int i=0; i<numblocks;i++)
		{
			int startframe=(int)blockarray->GetComponent(block,0);
			int endframe=(int)blockarray->GetComponent(block+1,0);
			block+=2;
			this->CalculateROIMean(inp,roi,msk,roimean,nt,nc,startframe,endframe);

		}


		//2.calculate correlation
		block=0;
		for (int i=0; i<numblocks;i++)
		{
			int startframe=(int)blockarray->GetComponent(block,0);
			int endframe=(int)blockarray->GetComponent(block+1,0);
			block+=2;
			this->CalculateSeedCorrelation(inp,roimean,msk,nt,startframe,endframe,out);
		}

		//3.average all the z values from all the blocks
		for (int voxel = 0; voxel<nt;voxel++)
		{
			for (int region=0;region<this->NumberOfRegions;region++)
			{
				if(out->GetComponent(voxel,region) != 0)
				{
					out->SetComponent(voxel,region,out->GetComponent(voxel,region)/numblocks);
					//std::cout<<"voxel"<<voxel<<":"<<out->GetComponent(voxel,region);
				}

			}
		}
		AverageROI->Delete();
		blockarray->Delete();
		temp->Delete();

}


