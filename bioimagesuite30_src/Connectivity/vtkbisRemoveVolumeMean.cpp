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
 * vtkbisRemoveVolumeMean.cpp
 *
 *  Created on: Aug 28, 2008
 *      Author: Isabella Murphy, Xilin Shen
 *
 * Subtract slice mean of each frame for each voxel
 */

#include "vtkbisRemoveVolumeMean.h"
#include "vtkObjectFactory.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include <math.h>// for the sqrt function
#include "pxisinf.h"

vtkbisRemoveVolumeMean* vtkbisRemoveVolumeMean::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisRemoveVolumeMean");
  if(ret)
    {
      return (vtkbisRemoveVolumeMean*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisRemoveVolumeMean;
}

vtkbisRemoveVolumeMean::vtkbisRemoveVolumeMean()
{
	this->ImageMask = NULL;
}

vtkbisRemoveVolumeMean::~vtkbisRemoveVolumeMean()
{
  this->SetImageMask(NULL);
}
/*-------------------------------------------------------------------------------------
 * force the output data type to double, need to change extend information to let
 * the pipeline know the difference
 * @Set: output data type = double
 */
int vtkbisRemoveVolumeMean::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector,
					  vtkInformationVector *outputVector)
{
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
	return 1;
}
/*!-----------------------------------------------------------------------------------------------------------
* Calculate mean of brain for each frame
* @param[in]inpdata: input image
* @param[in]mask: mask image
* @param[in]numvoxels: number of voxels in brain
* @param[in]numtimepoints: number for time points (number of frames)
* @param[out]outarr: output array. It will store the mornalized mean of the brain for each time points
*/
void vtkbisRemoveVolumeMean::CalculateVolumeMean(vtkDataArray* inpdata,vtkDataArray* mask,
						 double* outarr, int numvoxels, int numtimepoints)
{
  int voxel = 0;
  double tempsum=0;
  double meanInnerproduct = 0;
  int f,i;
  double normV = 0;
  int usedvoxels = 0;


  for (f=0;f<numtimepoints; f++)
  {
      for (i=0; i<numvoxels; i++)
      {
    	  //voxel = slice+i;
    	  int doGlm = 1;
    	  if (mask != NULL)
    	  {
    		  //if (mask->GetComponent(voxel,0)<1)
    		  if (mask->GetComponent(i,0)<1)
    		  {
    			  doGlm=0;
    		  }
	      //std::cout<<"voxel:"<<voxel<<"do gui:"<<this->DoGlm<<endl;
    	  }
    	  if(doGlm)
    	  {
    		  tempsum += inpdata->GetComponent(i,f);
    		  ++usedvoxels;
	      //std::cout<<"voxel:"<<voxel<<"get here 1!"<<endl;
    	  }
      }
      outarr[f] = tempsum/usedvoxels;//mean of time f
      tempsum = 0;
      usedvoxels = 0;
      meanInnerproduct += pow(outarr[f],2);
  }
  normV = sqrt(meanInnerproduct);
  if (normV<0.00001)
  	  normV=1.0;
  if (isnan(normV) || isinf(normV))
  	  normV=1.0;

  //normalize the mean for each time point
  for (f=0;f<numtimepoints;f++)
  {
    if (isnan(outarr[f]) || isinf(outarr[f]))
      outarr[f]=0.0;
    outarr[f]=outarr[f]/normV;
    std::cout<<"f:"<<f<<"="<<outarr[f]<<endl;
  }

}
/*!--------------------------------------------------------------------
 * This method subtract slice mean of each frame for a voxel
 * x-(innerproduct of x and mean)*mean
 * @param[in]inp: input vtkDataArray pointer
 * @param[in]outarr: array pointer, outarr hold the normalized slice mean of each frame calculated by this->CalculateVolumeMean method
 * @param[in]int voxel:the voxel we will work on
 * @param[in]numc: number of frames a voxel has
 * @param[out]out: ouput vtkDataArray pointer
 */
void vtkbisRemoveVolumeMean::RemoveMean(vtkDataArray* inp, vtkDataArray* out,double* outarr, int voxel, int numc)
{
  double innerproduct = 0;
  int m,f;

  for (m=0; m<numc; m++)
    innerproduct += inp->GetComponent(voxel, m)*outarr[m];

  for (f=0; f<numc; f++){
    out->SetComponent(voxel,f,inp->GetComponent(voxel, f)-innerproduct*outarr[f]);
    //std::cout<<"voxel:"<<voxel<<"f:"<<f<<"="<<out->GetComponent(voxel,f)<<endl;
  }
}
// ---------------------------------------------------------------------------
void vtkbisRemoveVolumeMean::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
  //fprintf(stderr,"Beginning vtkbisRemoveVolumeMean \n");
  int dim[3];
  int dimMask[3];
  int numc = 0;//number of components
  int voxel= 0;
  int slice = 0;
  int numvoxel= 0; //number of voxels in one slice
  int sum=0;
  int i,j,k,f,ia;

  input->GetDimensions(dim);

  if (this->ImageMask!=NULL)
    {
      this->ImageMask->GetDimensions(dimMask);
      for (ia=0;ia<=2;ia++)
	sum += abs(dim[ia]-dimMask[ia]);
      if (sum>0)
    	{
	  fprintf(stderr,"Bad Mask Input to vtkbisRemoveVolumnMean SimpleExecute\n");
	  return;
        }
    }

    vtkDataArray*   inp=input->GetPointData()->GetScalars();
    vtkDataArray*   out=output->GetPointData()->GetScalars();
    vtkDataArray*   mask = NULL;
    if (this->ImageMask!=NULL)
    	mask=this->ImageMask->GetPointData()->GetScalars();

    numc=input->GetNumberOfScalarComponents();
    double* outarr=new double[numc];//create an array to hold the mean on all time points t
    numvoxel=dim[0]*dim[1]*dim[2];//number of voxel per volume

    // Cleanup first -- fill output with zeros
    for (f=0;f<numc;f++)
      out->FillComponent(f,0.0);

    this->UpdateProgress(0.01);// for the use interface to show progress

    this->CalculateVolumeMean(inp,mask,outarr,numvoxel, numc);

    for (k=0;k<dim[2];k++)
    {
    	for (j=0;j<dim[1];j++)
    	{
    		for (i=0;i<dim[0];i++)
    		{
    			int dooperation=1;
    			if (mask!=NULL)
    			{
    				if (mask->GetComponent(voxel,0)<1)
    					dooperation=0;
    			}
    			if(dooperation)
    			{
    				this->RemoveMean(inp,out,outarr,voxel,numc);
    			}
    			++voxel;
    		}
    	}
    	this->UpdateProgress(float(k+1)/float(dim[2]));
    }
    delete [] outarr;
    this->UpdateProgress(1.0);
}


