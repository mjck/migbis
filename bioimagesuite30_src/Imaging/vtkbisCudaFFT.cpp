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
#include "bioimagesuite_settings.h"
#include "vtkbisCudaFFT.h"

#ifdef BIOIMAGESUITE3_USE_CUDA

#include <cufft.h>
#include <cuda.h>
#include <cuda_runtime.h>

extern "C" void bisCUDA_FFT(float *input, int dim[3],int forward);

#endif

vtkbisCudaFFT* vtkbisCudaFFT::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisCudaFFT");
  if(ret)
    {
      return (vtkbisCudaFFT*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisCudaFFT;
}


vtkbisCudaFFT::vtkbisCudaFFT() 
{

}

vtkbisCudaFFT::~vtkbisCudaFFT() 
{

}


int vtkbisCudaFFT::RequestInformation(vtkInformation *vtkNotUsed(request),  vtkInformationVector **inputVector, 
				      vtkInformationVector *outputVector)

{ 
  vtkInformation *outInfo = outputVector->GetInformationObject(0); 
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_DOUBLE, -1);
#ifdef BIOIMAGESUITE3_USE_CUDA
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, 2);
#else
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, 1);
#endif
  return 1;
}


 /*void vtkbisCudaFFT::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();

  output->SetScalarTypeToDouble();
#ifdef BIOIMAGESUITE3_USE_CUDA
  output->SetNumberOfScalarComponents(2);
#else
  output->SetNumberOfScalarComponents(1);
#endif
}*/


// ---------------------------------------------------------------------------------------------------------------------------
//   Main Function
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisCudaFFT::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{
#ifndef BIOIMAGESUITE3_USE_CUDA

  fprintf(stderr,"\nCuda disabled exiting now\n");
  return;

#else
 
 fprintf(stderr,"\nBeginning vtkbisCudaFFT\n");

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();
  int nc=out->GetNumberOfComponents();
  for (int ia=0;ia<nc;ia++)
    out->FillComponent(ia,0.0);

  int dim[3]; input->GetDimensions(dim);
  int nt0=dim[0]*dim[1]*dim[2];
  float* inputdata=new float[nt0*2];

  fprintf(stderr,"nt0=%d nc1=%d dims=(%d,%d,%d)\n",nt0,1,dim[0],dim[1],dim[2]);

  // setting the real part first
  for (int i=0;i<nt0;i++)
    {
      int index=i*2;
      inputdata[index]=(float)inp->GetComponent(i,0); 	  
      inputdata[index+1]=0.0f;
    }
 
  bisCUDA_FFT(inputdata, dim, 1);  

  for (int i=0;i<nt0;i++)
    {
      int index=2*i;
      out->SetComponent(i,0,inputdata[index]);
      out->SetComponent(i,1,inputdata[index+1]);
    }

  delete [] inputdata;
#endif
}


