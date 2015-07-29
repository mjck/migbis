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
 * vtkbisROIMean.cpp
 *
 *  Created on: Sep 26, 2008
 *      Author: Isabella Murphy; Xilin Shen
 * @Input: 4D image and an ROI(Region of Interest) 3D image,
 * @output: average time courses in each ROI region.Dimension: number of regions*1*1*N
 * Assume user name each region using integer
 * In output vtkImageData object, voxel 0 = region 0, voxel 10 = region 10 and so on.
 * dimension is number of regions*1*1*N
 */

#include <vtkbisROIMean.h>
#include "vtkObjectFactory.h"
#include "vtkObject.h"
#include "vtkPointData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkPoints.h"
#include <fstream>
#include <cstdlib>

using namespace std;

vtkbisROIMean* vtkbisROIMean::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisROIMean");
  if(ret)
    {
      return (vtkbisROIMean*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisROIMean;
}

vtkbisROIMean::vtkbisROIMean() {
  this->ImageROI=NULL;
  this->NumberOfRegions=0;
  this->Centroids=NULL;
  this->DoText = 0;
  this->ImageMask=NULL;
  this->AddRegion = 0;
}

vtkbisROIMean::~vtkbisROIMean() {
  this->SetImageROI(NULL);
  if (this->Centroids!=NULL)
    this->Centroids->Delete();
  this->SetImageMask(NULL);
}
void vtkbisROIMean::SetFileName(const char* filename)
{
  this->FileName=filename;
}

const char* vtkbisROIMean::GetFileName()
{
  return this->FileName.c_str();
}
void vtkbisROIMean::SetFileName2(const char* filename2)
{
  this->FileName2=filename2;
}

const char* vtkbisROIMean::GetFileName2()
{
  return this->FileName2.c_str();
}

/*-------------------------------------------------------------------------------------
 * Input/Output number of voxels are different, need to change extend information to let
 * the pipeline know the difference
 * @Set: output dimension = number of regions*1*1*N
 * !!!Output will loose the ROI region information
 */
int vtkbisROIMean::RequestInformation(vtkInformation *request,
				      vtkInformationVector **inputVector,
				      vtkInformationVector *outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  int wholeExtent[6];
  for(int i = 0;i<6;i++)
    wholeExtent[i]=0;

  vtkDataArray* roi=NULL;
  if (this->ImageROI!=NULL)
    roi=this->ImageROI->GetPointData()->GetScalars();

  double range[2];
  roi->GetRange(range);
  std::cout<<"ROI region min="<<range[0]<<" max="<<range[1]<<endl;

  this->NumberOfRegions = int(range[1]);
  if ( this->AddRegion >0 ) {
	  this->NumberOfRegions +=(int)this->AddRegion ;
  }

  wholeExtent[1]= this->NumberOfRegions-1;
  /*fprintf(stderr,"\n\nOutput Whole Extent=%d:%d,%d:%d,%d:%d",
	  wholeExtent[0],wholeExtent[1],
	  wholeExtent[2],wholeExtent[3],
	  wholeExtent[4],wholeExtent[5]);
*/
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),wholeExtent,6);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
  return 1;
}
// ---------------------------------------------------------------------------------------------------------------------------
//   Main Function
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisROIMean::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{

  fprintf(stderr,"Beginning vtkbisROIMean\n");
  int dim[3];
  int dim2[3];
  int dim3[3];


 input->GetDimensions(dim);
 fprintf(stderr," Input dim=%d x %d x %d , nframes=%d\n",
	 dim[0],dim[1],dim[2],input->GetNumberOfScalarComponents());

 output->GetDimensions(dim);
 fprintf(stderr," Output dim=%d x %d x %d , nframes=%d\n",
	 dim[0],dim[1],dim[2],output->GetNumberOfScalarComponents());


  if (this->ImageMask!=NULL)
    {
      int dim3[3]; this->ImageMask->GetDimensions(dim3);
      int dim2[3]; input->GetDimensions(dim2);
      int sum=0;
      for (int ia=0;ia<=2;ia++)
	sum+=abs(dim3[ia]-dim2[ia]);
      if (sum>0)
	{
	  fprintf(stderr,"Bad Mask Input to vtkbisROIMean SimpleExecute\n");
	  return;
	}
    }

  if (this->ImageROI!=NULL)
    {
      this->ImageROI->GetDimensions(dim);
      input->GetDimensions(dim2);
      int sum=0;
      for (int ia=0;ia<=2;ia++)
	sum+=abs(dim[ia]-dim2[ia]);
      if (sum>0)
	{
    	  fprintf(stderr,"Bad ROI Input to vtkbisROIMean SimpleExecute\n");
    	  return;
       }
     }
   else
   {
     cout<<"bad input to vtkbisROIMean, no ROI image."<<endl;
     return;
   }

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  int nt=inp->GetNumberOfTuples();//number of voxels
  int nc=inp->GetNumberOfComponents();//number of frames

  vtkDataArray* out=output->GetPointData()->GetScalars();
  for (int ia=0;ia<nc;ia++)
    out->FillComponent(ia,0.0);

  vtkDataArray* roi=NULL;
  if (this->ImageROI!=NULL)
    roi=this->ImageROI->GetPointData()->GetScalars();

  vtkDataArray* msk=NULL;
  if (this->ImageMask!=NULL)
    msk=this->ImageMask->GetPointData()->GetScalars();

  this->UpdateProgress(0.01);

  if (this->Centroids==NULL)
    {
      this->Centroids=vtkPoints::New();
    }
  //double range[2];
  //roi->GetRange(range);
  //std::cout<<"region min:"<<range[0]<<"region max:"<<range[1]<<endl;
  //this->NumberOfRegions = int(range[1]);
  //if ( this->AddRegion >0 )
  //{
	  //this->NumberOfRegions +=(int)this->AddRegion ;
  //}
  this->Centroids->SetNumberOfPoints(this->NumberOfRegions+1);// As many points as ROIs
  //fprintf(stderr,"In Simple Execute, Number Of Regions = %d\n",this->NumberOfRegions);

  double* x=new double[this->NumberOfRegions+1];
  double* y=new double[this->NumberOfRegions+1];
  double* z=new double[this->NumberOfRegions+1];
  double* num=new double[this->NumberOfRegions+1];
  double position[3];
  //initialize to 0

  double range[2];  roi->GetRange(range);
  int minroi=int(range[0])+1;

  for (int i=0;i<=this->NumberOfRegions;i++)
  {
      x[i]=0.0; y[i]=0.0; z[i]=0.0; num[i]=0.0;
  }

  //do something voxel by voxel
  for (int i=0;i< nt;i++)
  {
      int doinclude=1;
      if (msk!=NULL)
      {
    	  if (msk->GetComponent(i,0)<1)
    		  doinclude=0;
      }
      if (doinclude)
      {
    	  //guarantee that the first frame of the output will not be all 0
    	  int regionLabel = (int)roi->GetComponent(i,0)-minroi;
    	  //int regionLabel = (int)roi->GetComponent(i,0);
    	  input->GetPoint(i,position);//get x,y,z location of the voxel

    	  if (regionLabel >=0 )
    	  {
    		  for(int j=0;j<nc;j++)
    			  out->SetComponent(regionLabel,j,out->GetComponent(regionLabel,j)+inp->GetComponent(i,j));
    		  x[regionLabel]+=position[0];
    		  y[regionLabel]+=position[1];
    		  z[regionLabel]+=position[2];
    		  num[regionLabel]+=1.0;
    	  }

      }
   }




  int maxcount=int(nt/10);
  int count=0;
  double pog=0.0;
  //calculate mean time series of each ROI region
  for (int region=0;region<=this->NumberOfRegions;region++)
  {
	  //std::cout<<"region:"<<region;

    if (num[region]>0.00001)
	{
    	 //std::cout<<" Num:"<<num[region]<<endl;

    	x[region]=x[region]/num[region];
    	y[region]=y[region]/num[region];
    	z[region]=z[region]/num[region];

    	for (int j=0;j<nc;j++)
    	{
    		//std::cout<<"data:"<<out->GetComponent(region,j)<<"num:"<<num[region]<<endl;
    		out->SetComponent(region,j,out->GetComponent(region,j)/num[region]);
	 	}

    	this->Centroids->SetPoint(region,x[region],y[region],z[region]);
	}
    else
    {

    	this->Centroids->SetPoint(region,-10000,-10000,-10000);
    }

  }


  if(this->DoText == 1 )
  {

    /*  if(this->NumberOfRegions > 400 || nc > 500 )
      {
    	  fprintf(stderr, "number of voxels or number of frames > 400. Too much data to export.");
    	  return;
      }
*/
      std::ofstream myfile(this->FileName.c_str());

      if(!myfile.is_open())
      {
    	  cout<< "Unable to open file"<<this->FileName.c_str()<<"\n";
    	  return;
      }
      myfile<<"ROI \t ";
      for (int ia=0;ia<this->NumberOfRegions;ia++)
    	  myfile<<"ROI "<< ia <<"\t";
      myfile<<"\n";
      for(int i = 0; i < nc;i++)
      {
    	  myfile<<i<<"\t";
    	  for (int j = 0; j<this->NumberOfRegions;j++)
    	  {
    		  myfile<<out->GetComponent(j,i)<< "\t";
    	  }
    	  myfile<<"\n";
      }
      myfile.close();

      //*******************************************
      //file output for Alark Viewer
      //*******************************************
      std::ofstream alarkfile(this->FileName2.c_str());

      if(!alarkfile.is_open())
      {
    	  cout<< "Unable to open file "<<this->FileName2.c_str()<<"\n";
          return;
      }
      alarkfile<<"#Connectivity file\n#Points\n"<<this->NumberOfRegions<<"\n# index x y z label\n";
      for(int i=0;i<this->NumberOfRegions;i++)
      {
    	  alarkfile<<i<<"\t"<<x[i]<<"\t"<<y[i]<<"\t"<<z[i]<<"\t"<<i+1<<"\n";
      }
      alarkfile.close();

  }//end of DoText==1

  delete [] x; delete [] y; delete []z  ; delete [] num ;

  this->UpdateProgress(1.0);

}




