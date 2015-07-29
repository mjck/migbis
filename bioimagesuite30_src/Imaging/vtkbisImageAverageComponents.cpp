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
 * vtkbisImageAverageComponents.cpp
 *
 *  Created on: Sep 26, 2008
 *      Author: Isabella Murphy; Xilin Lin
 * @Input: 4D image and an ROI(Region of Interest) 3D image,
 * @output: average time courses in each ROI region.Dimension: number of regions*1*1*N
 * Assume user name each region using integer
 * In output vtkImageData object, voxel 0 = region 0, voxel 10 = region 10 and so on.
 * dimension is number of regions *1*1*1
 */

#include <vtkbisImageAverageComponents.h>
#include "vtkObjectFactory.h"
#include "vtkObject.h"
#include "vtkPointData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkPoints.h"

using namespace std;

vtkbisImageAverageComponents* vtkbisImageAverageComponents::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisImageAverageComponents");
  if(ret)
    {
      return (vtkbisImageAverageComponents*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisImageAverageComponents;
}

vtkbisImageAverageComponents::vtkbisImageAverageComponents() {
  this->StandardDeviationImage=NULL;
}

vtkbisImageAverageComponents::~vtkbisImageAverageComponents() {

  if (this->StandardDeviationImage!=NULL)
    this->StandardDeviationImage->Delete();
}

/*-------------------------------------------------------------------------------------
 * Input/Output number of voxels are different, need to change extend information to let
 * the pipeline know the difference
 * @Set: output dimension = number of regions*1*1*N
 * !!!Output will loose the ROI region information
 */
int vtkbisImageAverageComponents::RequestInformation(vtkInformation *request,
				      vtkInformationVector **inputVector,
				      vtkInformationVector *outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, 1);
  return 1;
}
// ---------------------------------------------------------------------------------------------------------------------------
//   Main Function
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisImageAverageComponents::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{
  if (this->GetNumberOfInputPorts()==0 || this->GetImageDataInput(0)==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }

  if (this->StandardDeviationImage!=NULL)
    this->StandardDeviationImage->Delete();

  this->StandardDeviationImage=vtkImageData::New();
  this->StandardDeviationImage->CopyStructure(output);
  this->StandardDeviationImage->SetNumberOfScalarComponents(output->GetNumberOfScalarComponents());
  this->StandardDeviationImage->AllocateScalars();
  
  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* out2=this->StandardDeviationImage->GetPointData()->GetScalars();
  int numvoxels=inp->GetNumberOfTuples();
  int numframes=inp->GetNumberOfComponents();

  this->UpdateProgress(0.01);
  float pog=0.00;

  float tenth=(numvoxels)/10.0;
  int count=0;

  double correction=sqrt((double(numframes)))/sqrt(double(numframes-1));


  double factor=double(1.0/numframes);
  for (int n=0;n<numvoxels;n++)
    {
      double sum=0.0;
      double sum2=0.0;
      for (int frame=0;frame<numframes;frame++)
	{
	  double v=inp->GetComponent(n,frame);
	  sum+=v;
	  sum2+=v*v;
	}
      double mean =sum*factor;
      out->SetComponent(n,0,mean);
      double sigma=sqrt(sum2*factor-mean*mean)*correction;
      out2->SetComponent(n,0,sigma);
  
      count++;
      if (count==tenth)
	{
	  pog+=0.1;
	  this->UpdateProgress(pog);
	  count=0;
	}
    }
  this->UpdateProgress(1.0);
}




