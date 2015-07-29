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
 * vtkbisROICorrelation.cpp
 *
 *  Created on: Sep 30, 2008
 *      Author: Isabella Murphy; Xilin Lin
 * @Input: 4D vtkImageData object of the whole image and a 4D vtkImageData object of the ROI average time course
 * @output: 4D vtkImageData object with correlation among each voxel of the image to each ROI region.
 * For example, correlation of voxel 0 to each ROI (say 0-10) is stored in output vtkImagedata voxel 0 component 0-10;
 * correlation of voxel 1 to each ROI (say 0-10) is stored in output vtkImageData voxel 1 component 0-10 and so on.
 * In the output image, xyz=input dimension, component=number of ROI regions
 */

#include "vtkbisROICorrelation.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkObject.h"
#include "vtkPointData.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkbisTimeSeriesCorrelation.h"
#include "vtkbisImageTimeSeriesNormalize.h"
#include <math.h>

using namespace std;

vtkbisROICorrelation::vtkbisROICorrelation() {
  this->AverageROI= NULL;
  this->ImageMask=NULL;
  this->OutputRaw=0;
}

vtkbisROICorrelation::~vtkbisROICorrelation() {
  this->SetAverageROI(NULL);
  this->SetImageMask(NULL);

}

vtkbisROICorrelation* vtkbisROICorrelation::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisROICorrelation");
  if(ret)
    {
      return (vtkbisROICorrelation*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisROICorrelation;
}
//----------------------------------------------------------------------------------------------------------------------
//RequestInformation
//I. Our vtkImageData input has number of components = number of frames but output vtkImageData object has number of
//components = roint.
//II. Input and Output has different number of voxels, has to let the pipeline know it (change extend)
//need to call RequstInformation to set those differences.
// ---------------------------------------------------------------------------------------------------------------------------
int vtkbisROICorrelation::RequestInformation (vtkInformation * request,
			vtkInformationVector ** inputVector,
			vtkInformationVector * outputVector)
{
  //vtkInformation *inInfo =  inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  //int wholeExtent[6];
  //inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExtent);
  //int dim[3];dim[0]= wholeExtent[1]+1;dim[1]= wholeExtent[3]+1;dim[2]= wholeExtent[5]+1;
  //wholeExtent[3] = dim[0]*dim[1]*dim[2]-1;
  vtkDataArray* roi=NULL;
  if (this->AverageROI!=NULL)
    roi=this->AverageROI->GetPointData()->GetScalars();
  else
    cout<<"bad input for vtkbisROICorrelation: no average ROI image"<<endl;
  int roint = roi->GetNumberOfTuples();//number of regions
  //wholeExtent[1]=roint-1;

  /*fprintf(stderr,"\n\nInput Whole Extent=%d:%d,%d:%d,%d:%d",
    wholeExtent[0],wholeExtent[1],
    wholeExtent[2],wholeExtent[3],
    wholeExtent[4],wholeExtent[5]);*/

  //outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),wholeExtent,6);//change extend
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, roint);//set number of components to roint

  return 1;
}
// ---------------------------------------------------------------------------------------------------------------------------
//   Main Function
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisROICorrelation::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{
  fprintf(stderr,"Beginning vtkbisROICorrelation\n");

  if (this->ImageMask!=NULL)
    {
      int dim[3]; this->ImageMask->GetDimensions(dim);
      int dim2[3]; input->GetDimensions(dim2);
      int sum=0;
      for (int ia=0;ia<=2;ia++)
	sum+=abs(dim[ia]-dim2[ia]);
      if (sum>0)
	{
	  fprintf(stderr,"Bad Mask Input to vtkbisROICorrelation SimpleExecute\n");
	  return;
	}
    }

  if (this->AverageROI->GetNumberOfScalarComponents()!=input->GetNumberOfScalarComponents())
    {
      fprintf(stderr,"Bad ROI Image in vtkbisROICorrelation SimpleExecute\n");
      return;
    }



  vtkbisImageTimeSeriesNormalize* norm1=vtkbisImageTimeSeriesNormalize::New();
  norm1->SetInput(input);
  norm1->Update();

  vtkImageData* temp=vtkImageData::New();
  temp->ShallowCopy(norm1->GetOutput());
  norm1->Delete();

  vtkDataArray*   inp=temp->GetPointData()->GetScalars();

  vtkbisImageTimeSeriesNormalize* norm2=vtkbisImageTimeSeriesNormalize::New();
  norm2->SetInput(this->AverageROI);
  norm2->Update();

  vtkImageData* temp2=vtkImageData::New();
  temp2->ShallowCopy(norm2->GetOutput());
  norm2->Delete();
  vtkDataArray*   roi=(vtkFloatArray*)temp2->GetPointData()->GetScalars();

  int nt=inp->GetNumberOfTuples();//number of voxels
  int nc=inp->GetNumberOfComponents();//number of frames (time series)
  float factor=1.0/float(nc);

  vtkDataArray* out=output->GetPointData()->GetScalars();
  int ncout=out->GetNumberOfComponents();
  for (int ia=0;ia<ncout;ia++)
    out->FillComponent(ia,0.0);


  int roint = roi->GetNumberOfTuples();//number of regions
  int roinc = roi->GetNumberOfComponents();//number of frames (time series)


  vtkDataArray* msk=NULL;
  if (this->ImageMask!=NULL)
    msk=this->ImageMask->GetPointData()->GetScalars();

  this->UpdateProgress(0.01);
  //Calculate correlation

  // For each voxel
  for (int voxel = 0; voxel<nt;voxel++)
  {
    int docompute=1;
    if (msk!=NULL)
      {
	if (msk->GetComponent(voxel,0)<1)
	  docompute=0;
      }

    if (docompute)
      {
	for (int roiindex = 0; roiindex<roint; roiindex++)
    	  {
	    //	      double rho=vtkbisTimeSeriesCorrelation::ComputeCorrelationCoefficient(inp,voxel,roi,roiindex);
	    float rho=vtkbisTimeSeriesCorrelation::ComputePreNormalizedCorrelationCoefficient(inp,voxel,roi,roiindex,factor,nc);
	    if (this->OutputRaw)
	      out->SetComponent(voxel,roiindex,rho);
	    else
	      out->SetComponent(voxel,roiindex,vtkbisTimeSeriesCorrelation::RhoToZConversion(rho));
	    //std::cout<<out->GetComponent(voxel,roiindex)<<";";
    	  }
	//    	  std::cout<<endl;
      }
  }

  temp->Delete();
  temp2->Delete();

  this->UpdateProgress(1.0);
}


