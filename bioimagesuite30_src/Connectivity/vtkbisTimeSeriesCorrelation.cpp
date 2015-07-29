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
 * vtkbisTimeSeriesCorrelation.cpp
 *
 *  Created on: Sep 29, 2008
 *      Author: Isabella Murphy; Xilin Lin
 *      Also called matrix correlation, ROI Mean X ROI Mean
 * @Input: 4D vtkImageData object ,which is the output of vtkbisroimean. Dimension:number of regions*1*1*N
 * @output: 2D vtkImageData object with correlation among each ROI Mean.Dimension:number of regions *Number of regions*1*1
 * size = number of time series*number of time series*1*1
 * For example, correlation of average time course 0 to  (say 0-10) is stored in output
 * vtkImagedata voxel 0-10 component 0;correlation of average time course 1 to all other
 * average time course (say 0-10) is stored in output vtkImageData voxel 11-20 component
 * 0 and so on.
 * voxel 1 is stored at (1,0,0) ; voxel 2 is stored at (2,0,0)
 */

#include "vtkbisTimeSeriesCorrelation.h"
#include "vtkImageData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkObject.h"
#include "vtkPointData.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkbisImageTimeSeriesNormalize.h"
#include <math.h>
#include <fstream>
#include <cstdlib>
using namespace std;

vtkbisTimeSeriesCorrelation::vtkbisTimeSeriesCorrelation()
{
  this->DoText = 0;
  this->OutputRaw = 0;
}
// This creates the "New" Function
vtkbisTimeSeriesCorrelation* vtkbisTimeSeriesCorrelation::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisTimeSeriesCorrelation");
  if(ret)
    {
      return (vtkbisTimeSeriesCorrelation*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisTimeSeriesCorrelation;
}
void vtkbisTimeSeriesCorrelation::SetFileName(const char* filename)
{
  this->FileName=filename;

}
const char* vtkbisTimeSeriesCorrelation::GetFileName()
{
  return this->FileName.c_str();
}
void vtkbisTimeSeriesCorrelation::SetFileName2(const char* filename2)
{
  this->FileName2=filename2;
}

const char* vtkbisTimeSeriesCorrelation::GetFileName2()
{
  return this->FileName2.c_str();
}
/*----------------------------------------------------------------------------------------------------------------------
*RequestInformation
*I.vtkImageData input has many number of components(number of frames) but output
*vtkImageData object has number of components = 1,
*II.the input/output number of voxels is different, need to change extend
*extend[0],extend[1] for x;extend[2], extend[3] for y; extend[4], extend[5] for z
*if dimension is 16*4*1 then extend is (0,15,0,3,0,0)
*need to call RequstInformation to set those difference.
*/// ---------------------------------------------------------------------------------------------------------------------------
int vtkbisTimeSeriesCorrelation::RequestInformation (vtkInformation * request,
						     vtkInformationVector ** inputVector,
						     vtkInformationVector * outputVector)
{
  vtkInformation *inInfo =  inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  int wholeExtent[6];
  inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExtent);


  int nvoxels= (wholeExtent[1]+1)*(wholeExtent[3]+1)*(wholeExtent[5]+1);

  //wholeExtent = { 0, width-1, 0 , height -1, 0, depth-1 }
  //int half=ceil(double(nvoxels*nvoxels)/2.0)+1;

  wholeExtent[0]=0; 	wholeExtent[1]=nvoxels-1;
  wholeExtent[2]=0; 	wholeExtent[3]=nvoxels-1;
  wholeExtent[4]=0; 	wholeExtent[5]=0;

  /*wholeExtent[0]=0; 	wholeExtent[1]=half-1;
    wholeExtent[2]=0; 	wholeExtent[3]=0;
    wholeExtent[4]=0; 	wholeExtent[5]=0;*/

 /* fprintf(stderr,"\n\nOutput Whole Extent=%d:%d,%d:%d,%d:%d",
    wholeExtent[0],wholeExtent[1],
    wholeExtent[2],wholeExtent[3],
    wholeExtent[4],wholeExtent[5]);
*/
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),wholeExtent,6);//change extend
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, 1);//change number of components

  return 1;
}

//---------------------------------------------------------------------------------
// Compute Correlation Coefficient
//---------------------------------------------------------------------------------
double vtkbisTimeSeriesCorrelation::RhoToZConversion(double rho)
{

  double gValue = 0;
  if (fabs(rho-1.0)>0.00001)
    gValue = 0.5*log((1+rho)/(1-rho));
  return gValue;
}
//---------------------------------------------------------------------------------
double vtkbisTimeSeriesCorrelation::ComputeCorrelationCoefficient(vtkDataArray* a1,int index1,vtkDataArray* a2,int index2)
{
  if (a1==NULL || a2==NULL)
    return 0.0;
  if (a1->GetNumberOfComponents()!=a2->GetNumberOfComponents())
    return 0.0;

  int nc=a1->GetNumberOfComponents();
  if (nc<1)
    return 0.0;


  double sumxy=0.0;
  double sumx=0.0;
  double sumx2=0.0;
  double sumy=0.0;
  double sumy2=0.0;


  double factor=1.0/double(nc);
  for (int i=0;i<nc;i++)
    {
      double x=a1->GetComponent(index1,i);
      double y=a2->GetComponent(index2,i);

      sumx+=x;
      sumy+=y;
      sumx2+=x*x;
      sumy2+=y*y;
      sumxy+=x*y;
    }

  double exy=sumxy*factor;
  double ex2=sumx2*factor;
  double ex=sumx*factor;
  double ey2=sumy2*factor;
  double ey=sumy*factor;

  double numerator=exy-ex*ey;
  double denom    = sqrt(ex2-ex*ex)*sqrt(ey2-ey*ey);

  if (denom>0.0)
    return numerator/denom;

  return 0.0;
}

double vtkbisTimeSeriesCorrelation::ComputePreNormalizedCorrelationCoefficient(vtkDataArray* a1,int index1,vtkDataArray* a2,int index2,double factor,int nc)
{
  double sum=0.0;

  if ( (a1->IsA("vtkFloatArray") && a2->IsA("vtkFloatArray")) )
    {
      float* x1=((vtkFloatArray*)a1)->GetPointer(index1*nc);
      float* y1=((vtkFloatArray*)a2)->GetPointer(index2*nc);
      for (int i=0;i<nc;i++)
	{
    	  sum+=(*x1)*(*y1);
    	  ++x1;
    	  ++y1;
	}
    }
  else
    {
      for (int i=0;i<nc;i++)
	{
    	  double x=a1->GetComponent(index1,i);
    	  double y=a2->GetComponent(index2,i);
    	  sum+=x*y;
	}
    }

  return sum*factor;

}

//---------------------------------------------------------------------------------
//main function
//---------------------------------------------------------------------------------
void vtkbisTimeSeriesCorrelation::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  fprintf(stderr,"Beginning vtkbisTimeSeriesCorrelation.\n");
  int dim[3];

  input->GetDimensions(dim);
  /*fprintf(stderr,"dim=%d x %d x %d , nframes=%d\n",
	  dim[0],dim[1],dim[2],input->GetNumberOfScalarComponents());

  output->GetDimensions(dim);
  fprintf(stderr,"dim=%d x %d x %d , nframes=%d\n",
	  dim[0],dim[1],dim[2],output->GetNumberOfScalarComponents());*/

  vtkbisImageTimeSeriesNormalize* norm1=vtkbisImageTimeSeriesNormalize::New();
  norm1->SetInput(input);
  norm1->Update();

  vtkImageData* temp=vtkImageData::New();
  temp->ShallowCopy(norm1->GetOutput());
  norm1->Delete();


  vtkDataArray*   inp=temp->GetPointData()->GetScalars();
  vtkDataArray*   out=output->GetPointData()->GetScalars();

  int nt = inp->GetNumberOfTuples();//get number of regions
  int nc = inp->GetNumberOfComponents();//get number of frames(time series points)
  //cout<<"nt:"<<nt<<"nc"<<nc<<"\n";
  double factor=1.0/double(nc);

  int ncout=out->GetNumberOfComponents();
  int ntout=out->GetNumberOfTuples();
  //cout<<"ntout:"<<ntout<<"\n";
  for (int ia=0;ia<ncout;ia++)
    out->FillComponent(ia,0.0);


  this->UpdateProgress(0.01);

  int index=0;
  for (int ia=0;ia<nt;ia++)
  {
    for (int ib = 0;ib<nt;ib++)
	{
	  double rho=vtkbisTimeSeriesCorrelation::ComputePreNormalizedCorrelationCoefficient(inp,ia,inp,ib,factor,nc);
	  //	  double rho2=vtkbisTimeSeriesCorrelation::ComputeCorrelationCoefficient(inp,ia,inp,ib);

	  //	  fprintf(stderr,"%d \t %d \t %f \t %f\n",ia,ib,rho,rho2);
	  if (this->OutputRaw)
	  {
		  out->SetComponent(index,0,rho);
	  }else{
		  out->SetComponent(index,0,vtkbisTimeSeriesCorrelation::RhoToZConversion(rho));
	  }
	  ++index;
	}
      this->UpdateProgress(float(ia+1)/float(nt));
    }
  //cout<<"index:"<<index<<"\n";

  temp->Delete();


  if(this->DoText == 1 )
  {
      if(nc > 400 )
      {
    	  fprintf(stderr, "number of frames > 400. Too much data to export.");
    	  return;
      }
      std::ofstream myfile(this->FileName.c_str());//output file stream
      if(!myfile.is_open())
      {
    	  cout<< "Unable to open file"<<this->FileName.c_str()<<"\n";
    	  return;
      }
      //loop through the output image file
      int counter=0;
      for (int ia = 0; ia<nt; ia++)
      {
    	  for (int ib=0;ib<nt;ib++)
    	  {
    		  myfile<<out->GetComponent(counter,0)<< "\t";

    		  ++counter;
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

      alarkfile<<"#Connections and strengths\n"<<((1+nt-1)*(nt-1))/2<<"\n";
      vtkFloatArray* alarkoutput=vtkFloatArray::New();
      alarkoutput->SetNumberOfComponents(nt);
      alarkoutput->SetNumberOfTuples(nt);
      for(int ia=0;ia<nt;ia++)
    	  alarkoutput->FillComponent(ia,0.0);
      int index=0;
      for(int i=0;i<nt;i++)
      {
    	  for(int j=0;j<nt;j++)
    	  {
    		  alarkoutput->SetComponent(i,j,out->GetComponent(index,0));
    		  ++index;
    	  }
      }
      for(int i=0;i<nt;i++)
      {
          for(int j=i+1;j<nt;j++)
          {
        	  alarkfile<<i<<"\t"<<j<<"\t"<<alarkoutput->GetComponent(i,j)<<"\n";
          }
      }
      alarkfile.close();
      alarkoutput->Delete();
  }
  //  temp->Delete();
  this->UpdateProgress(1.0);
}

