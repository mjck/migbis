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
#include "vtkbisCudaRFFT.h"

#ifdef BIOIMAGESUITE3_USE_CUDA

#include <cufft.h>
#include <cuda.h>
#include <cuda_runtime.h>

extern "C" void bisCUDA_FFT(float *input, int dim[3],int forward);

#endif

vtkbisCudaRFFT* vtkbisCudaRFFT::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisCudaRFFT");
  if(ret)
    {
      return (vtkbisCudaRFFT*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisCudaRFFT;
}


vtkbisCudaRFFT::vtkbisCudaRFFT() 
{

}

vtkbisCudaRFFT::~vtkbisCudaRFFT() 
{

}

/*void vtkbisCudaRFFT::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();

  output->SetScalarTypeToDouble();
#ifdef BIOIMAGESUITE3_USE_CUDA
  output->SetNumberOfScalarComponents(1);
#else
  output->SetNumberOfScalarComponents(2);
#endif
}*/


int vtkbisCudaRFFT::RequestInformation(vtkInformation *vtkNotUsed(request),  vtkInformationVector **inputVector, 
				      vtkInformationVector *outputVector)

{ 
  vtkInformation *outInfo = outputVector->GetInformationObject(0); 
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_DOUBLE, -1);
#ifdef BIOIMAGESUITE3_USE_CUDA
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, 1);
#else
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, 2);
#endif
  return 1;
}



// ---------------------------------------------------------------------------------------------------------------------------
//   Main Function
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisCudaRFFT::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{
#ifndef BIOIMAGESUITE3_USE_CUDA

  fprintf(stderr,"\nCuda disabled\n");
  return;

#else
 
  fprintf(stderr,"\nBeginning vtkbisCudaRFFT\n");

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
      inputdata[index+1]=(float)inp->GetComponent(i,1); 	  
    }
 
  bisCUDA_FFT(inputdata, dim, 0);  

  for (int i=0;i<nt0;i++)
    out->SetComponent(i,0,inputdata[i*2]);
  
  delete [] inputdata;
#endif
}


