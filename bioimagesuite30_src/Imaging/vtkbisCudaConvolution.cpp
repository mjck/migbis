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
#include "vtkbisCudaConvolution.h"
#include "vtkpxUtil.h"
#include "vtkMath.h"
#include "vtkImageCast.h"
#include <time.h>

#ifdef BIOIMAGESUITE3_USE_CUDA

#include <cufft.h>
#include <cuda.h>
#include <cuda_runtime.h>

extern "C" void FFTFirstImage(float *cuInp1,cufftComplex *fft1,int dim1[3],int convdim[3]);
extern "C" void ConvolveWithFilter(cufftComplex *fft1,float *cuInp2,float *cuOut,int dim1[3],int dim2[3],int convdim[3]);

#endif

const int CUDA_FFT_LIMIT = 256*256*128;

vtkbisCudaConvolution* vtkbisCudaConvolution::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisCudaConvolution");
  if(ret)
    {
      return (vtkbisCudaConvolution*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisCudaConvolution;
}

vtkbisCudaConvolution::vtkbisCudaConvolution() 
{
  this->Filter=NULL;
}

vtkbisCudaConvolution::~vtkbisCudaConvolution() 
{  
  this->SetFilter(NULL);
}
int vtkbisCudaConvolution::RequestInformation(vtkInformation *request,
				      vtkInformationVector **inputVector,
				      vtkInformationVector *outputVector)
{
  int nc=this->Filter->GetNumberOfScalarComponents();
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, nc);
  return 1;
}


// ---------------------------------------------------------------------------------------------------------------------------
//   Main Function
// ---------------------------------------------------------------------------------------------------------------------------
void vtkbisCudaConvolution::SimpleExecute(vtkImageData* input ,vtkImageData* output)
{

#ifndef BIOIMAGESUITE3_USE_CUDA
  fprintf(stderr,"\nCuda disabled exiting now\n");
  return;
#endif
#ifdef BIOIMAGESUITE3_USE_CUDA

 
  if (this->Filter==NULL)
    {
      fprintf(stderr,"Image not set\n Exiting Now!!!!");
      return;
    }
  
  fprintf(stderr,"\nBeginning vtkbisCudaConvolution\n");
  
  int dim1[3]; input->GetDimensions(dim1);
  int dim2[3]; this->Filter->GetDimensions(dim2);
  int nc=this->Filter->GetNumberOfScalarComponents();
  
  int convdim[3];
  for (int i=0;i<3;i++)
    convdim[i]=dim1[i]+dim2[i]-1;

  int ntconv=convdim[0]*convdim[1]*convdim[2];
  int nt1=dim1[0]*dim1[1]*dim1[2];
  int nt2=dim2[0]*dim2[1]*dim2[2];

  if(ntconv>CUDA_FFT_LIMIT)
    {
      fprintf(stderr,"\nError resultant convolution is too big for CUDA FFT\n");
      return;
    }

  vtkImageCast* cast=vtkImageCast::New();
  cast->SetInput(input);
  cast->SetOutputScalarTypeToFloat();
  cast->Update();

  const int size=sizeof(float);
  const int mem_size_inp1=size*nt1;
  const int mem_size_inp2=size*nt2;
  const int mem_size_conv=sizeof(cufftComplex)*ntconv;

  float* inp1=(float*)cast->GetOutput()->GetPointData()->GetScalars()->GetVoidPointer(0);
  float* inp2=(float*)this->Filter->GetPointData()->GetScalars()->GetVoidPointer(0);
  float* out =(float*)output->GetPointData()->GetScalars()->GetVoidPointer(0);
  
  float* cuInp1;
  cudaMalloc((void**)&cuInp1,mem_size_inp1); 
  cudaMemcpy(cuInp1,inp1,mem_size_inp1,cudaMemcpyHostToDevice);

  float* cuInp2;
  cudaMalloc((void**)&cuInp2,mem_size_inp2); 

  float* cuOut;
  cudaMalloc((void**)&cuOut,mem_size_inp1); 
  
  cufftComplex *fft1;
  cudaMalloc((void**)&fft1,mem_size_conv); 

  cudaMemcpyToSymbol( "CONVSIZE", &ntconv, sizeof(int) );

  FFTFirstImage(cuInp1,fft1,dim1,convdim);

  float* framein =new float[nt2];
  float* frameout=new float[nt1];
  
  for(int i=0;i<nc;i++)
    {
      printf("\ndoing frame %d\n",i);
      
      for(int j=0;j<nt2;j++)
	framein[j]=*(inp2+i+j*nc);
      
      cudaMemcpy(cuInp2,framein,mem_size_inp2,cudaMemcpyHostToDevice);      
      
      ConvolveWithFilter(fft1,cuInp2,cuOut,dim1,dim2,convdim);
      
      cudaMemcpy(frameout,cuOut,mem_size_inp1,cudaMemcpyDeviceToHost);
      
      for(int j=0;j<nt1;j++)
	*(out+i+j*nc)=frameout[j];
    }
  
  cudaFree(fft1);
  cudaFree(cuInp1);
  cudaFree(cuInp2);
  cudaFree(cuOut);

  delete [] framein;
  delete [] frameout;

  cast->Delete();
   
#endif
}

void vtkbisCudaConvolution::CreateHessian(double sigma1,double sigma2,double sigma3,double radius)
{
 
  double sigma[3] = { sigma1, sigma2, sigma3 };
  int dim[3];
  for (int ia=0;ia<=2;ia++)
    dim[ia]=int(radius*sigma[ia]+0.5)*2+1;

  vtkImageData* newimg=vtkImageData::New();
  newimg->SetScalarTypeToFloat();
  newimg->SetNumberOfScalarComponents(6);
  newimg->SetDimensions(dim);
  newimg->AllocateScalars();

  
  double midx=0.5*double(dim[0]-1);
  double midy=0.5*double(dim[1]-1);
  double midz=0.5*double(dim[2]-1);
  
  double invs2[3];
  double s2[3];
  double invs4[3];
  for (int ia=0;ia<=2;ia++)
    {
      s2[ia]=sigma[ia]*sigma[ia];
      invs2[ia]=1.0/(s2[ia]);
      invs4[ia]=invs2[ia]*invs2[ia];
    }

  double scalefactor=1.0/(sqrt(2.0*vtkMath::Pi())*sqrt(2.0*vtkMath::Pi())*sqrt(2.0*vtkMath::Pi())*sigma[0]*sigma[1]*sigma[2]); // should be sqrt(2pi)^3 but just a constant...

  for (int k=0;k<dim[2];k++)
    {
      double z=double(k)-midz;
      double z2=z*z*invs2[2];
      
      for (int j=0;j<dim[1];j++)
	{
	  double y=double(j)-midy;
	  double y2=y*y*invs2[1];
	  for (int i=0;i<dim[0];i++)
	    {
	      double x=double(i)-midx;
	      double x2=x*x*invs2[0];

	      double G=scalefactor*exp(- 0.5*( x2+y2+z2));

	      newimg->SetScalarComponentFromDouble(i,j,k,0, (x*x - s2[0])*invs4[0]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,3, (y*y - s2[1])*invs4[1]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,5, (z*z - s2[2])*invs4[2]*G);

	      newimg->SetScalarComponentFromDouble(i,j,k,1, x*y * invs2[0]*invs2[1]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,2, x*z * invs2[0]*invs2[2]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,4, y*z * invs2[1]*invs2[2]*G);

	    }
	}
    }
  
  this->SetFilter(newimg);
  newimg->Delete();

}

