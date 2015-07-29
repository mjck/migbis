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
#include "vtkbisCudaHessian.h"
#include "vtkMath.h"
#include "vtkImageExtractComponents.h"

#include <time.h>

vtkbisCudaHessian* vtkbisCudaHessian::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkbisCudaHessian");
  if(ret)
    {
      return (vtkbisCudaHessian*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkbisCudaHessian;
}

vtkbisCudaHessian::vtkbisCudaHessian() 
{
  this->Filter=NULL;
}

vtkbisCudaHessian::~vtkbisCudaHessian() 
{  
  this->SetFilter(NULL);
}

int vtkbisCudaHessian::RequestInformation(vtkInformation *request,
				      vtkInformationVector **inputVector,
				      vtkInformationVector *outputVector)
{
  if (this->Filter==NULL)
    {
      vtkErrorMacro(<<"No Filter Bank Image Set\n");
      return 1;
    } 

  int nc=this->Filter->GetNumberOfScalarComponents();
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, nc);
  
  return 1;
}

void vtkbisCudaHessian::SimpleExecute(vtkImageData* input, vtkImageData* output)
{

    if (input==NULL || this->Filter ==NULL )
    {
      vtkErrorMacro(<<"Bad Inputs to vtkxqImageFourierConvolution");
      return;
    }
    
    vtkDataArray* out=output->GetPointData()->GetScalars();

    int nc=this->Filter->GetNumberOfScalarComponents();
    
    for(int i=0;i<nc;i++)
      {
	vtkImageExtractComponents* comp=vtkImageExtractComponents::New();
	vtkbisCudaConvolution* conv=vtkbisCudaConvolution::New();

	conv->SetInput(input);
	comp->SetInput(this->Filter);

	comp->SetComponents(i);
	comp->Update();
	
	conv->SetFilter(comp->GetOutput());
	
	//clock_t before=clock();
	conv->Update();
	//printf("\ncuda convolution took %f seconds\n",((clock()-before)/CLOCKS_PER_SEC));

	out->CopyComponent(i,conv->GetOutput()->GetPointData()->GetScalars(),0);

	comp->Delete();
	conv->Delete();
      }
}

void vtkbisCudaHessian::CreateHessian(double sigma1,double sigma2,double sigma3,double radius)
{
 
  double sigma[3] = { sigma1, sigma2, sigma3 };
  int dim[3];
  for (int ia=0;ia<=2;ia++)
    dim[ia]=int(radius*sigma[ia]+0.5)*2+1;

  vtkImageData* newimg=vtkImageData::New();
  newimg->SetScalarTypeToDouble();
  //  newimg->SetNumberOfScalarComponents(6);
  newimg->SetNumberOfScalarComponents(12);
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

	      newimg->SetScalarComponentFromDouble(i,j,k,1, (y*y - s2[1])*invs4[1]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,2, (z*z - s2[2])*invs4[2]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,3, (x*x - s2[0])*invs4[0]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,4, (y*y - s2[1])*invs4[1]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,5, (z*z - s2[2])*invs4[2]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,6, (x*x - s2[0])*invs4[0]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,7, (y*y - s2[1])*invs4[1]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,8, (z*z - s2[2])*invs4[2]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,9, (x*x - s2[0])*invs4[0]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,10, (y*y - s2[1])*invs4[1]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,11, (z*z - s2[2])*invs4[2]*G);

	      /*
	      newimg->SetScalarComponentFromDouble(i,j,k,0, (x*x - s2[0])*invs4[0]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,3, (y*y - s2[1])*invs4[1]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,5, (z*z - s2[2])*invs4[2]*G);

	      newimg->SetScalarComponentFromDouble(i,j,k,1, x*y * invs2[0]*invs2[1]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,2, x*z * invs2[0]*invs2[2]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,4, y*z * invs2[1]*invs2[2]*G);
	      */
	    }
	}
    }
  
  this->SetFilter(newimg);
  newimg->Delete();

}

