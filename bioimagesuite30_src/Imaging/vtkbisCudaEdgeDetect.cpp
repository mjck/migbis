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


#include "vtkObjectFactory.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "bioimagesuite_settings.h"
#include "vtkbisCudaEdgeDetect.h"
#include "vtkpxUtil.h"
#include "vtkMath.h"
#include "vtkImageCast.h"
#include <time.h>

#ifdef BIOIMAGESUITE3_USE_CUDA

#include <cufft.h>
#include <cuda.h>
#include <cuda_runtime.h>

extern "C" void EdgeDetection(float *input,float *output,int dim[3],float spa[3]);
extern "C" void Test(float *input,float *output,int dim[3]);

#endif

vtkbisCudaEdgeDetect* vtkbisCudaEdgeDetect::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisCudaEdgeDetect");
  if(ret)
    {
      return (vtkbisCudaEdgeDetect*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisCudaEdgeDetect;
}

vtkbisCudaEdgeDetect::vtkbisCudaEdgeDetect() { }

vtkbisCudaEdgeDetect::~vtkbisCudaEdgeDetect() { }

int vtkbisCudaEdgeDetect::RequestInformation(vtkInformation *request,
				      vtkInformationVector **inputVector,
				      vtkInformationVector *outputVector)
{
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, 1);
  return 1;
}


// ---------------------------------------------------------------------------------------------------------------------------
//   Main Function
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisCudaEdgeDetect::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{

#ifndef BIOIMAGESUITE3_USE_CUDA
  fprintf(stderr,"\nCuda disabled exiting now\n");
  return;
#endif
#ifdef BIOIMAGESUITE3_USE_CUDA

  fprintf(stderr,"\nBeginning vtkbisCudaEdgeDetect\n");
  
  int dim1[3]; input->GetDimensions(dim1);
  int nt1=dim1[0]*dim1[1]*dim1[2];

  double tmp[3]; float spa[3];
  input->GetSpacing(tmp);
  spa[0]=(float)tmp[0];
  spa[1]=(float)tmp[1];
  spa[2]=(float)tmp[2];

  const int mem_size_inp1=sizeof(float)*nt1;

  vtkImageCast* cast=vtkImageCast::New();
  cast->SetInput(input);
  cast->SetOutputScalarTypeToFloat();
  cast->Update();

  float* inp=(float*)cast->GetOutput()->GetPointData()->GetScalars()->GetVoidPointer(0);
  float* out=(float*)output->GetPointData()->GetScalars()->GetVoidPointer(0);
  
  float* cuInp1;
  cudaMalloc((void**)&cuInp1,mem_size_inp1); 
  cudaMemcpy(cuInp1,inp,mem_size_inp1,cudaMemcpyHostToDevice);

  float* cuOut;
  cudaMalloc((void**)&cuOut,mem_size_inp1); 
  
  //Test(cuInp1,cuOut,dim1);
  EdgeDetection(cuInp1,cuOut,dim1,spa);
      
  cudaMemcpy(out,cuOut,mem_size_inp1,cudaMemcpyDeviceToHost);
    
  cudaFree(cuInp1);
  cudaFree(cuOut);

  cast->Delete();
   
#endif
}


