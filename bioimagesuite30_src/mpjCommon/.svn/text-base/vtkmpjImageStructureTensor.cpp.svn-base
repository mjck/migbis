

//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
//BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------



#include "vtkObjectFactory.h"
#include "vtkSource.h"
#include "vtkmpjImageStructureTensor.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageGradient.h"
#include "vtkImageExtractComponents.h"
//#include "vtkpxAppendComponents.h"
#include "vtkImageShiftScale.h"
//------------------------------------------------------------------------------
vtkmpjImageStructureTensor* vtkmpjImageStructureTensor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageStructureTensor");
  if(ret)
    {
    return (vtkmpjImageStructureTensor*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageStructureTensor;
}

// Construct object with no children.
vtkmpjImageStructureTensor::vtkmpjImageStructureTensor()
{
  this->Sigma=1.0;
}

// ----------------------------------------------------------------------------
vtkmpjImageStructureTensor::~vtkmpjImageStructureTensor()
{
}

// ----------------------------------------------------------------------------
void vtkmpjImageStructureTensor::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();
  output->SetScalarTypeToFloat();
  output->SetNumberOfScalarComponents(6);
}

void vtkmpjImageStructureTensor::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }

  if (input->GetNumberOfScalarComponents()!=1)
    {
      vtkErrorMacro(<<"Only Single Component Images are valid inputs\n");
      return;
    }

  for (int c=0;c<=5;c++)
    output->GetPointData()->GetScalars()->FillComponent(c,0.0);

  float sp[3]; input->GetSpacing(sp);
  float sig[3];
  for (int ia=0;ia<=2;ia++)
    sig[ia]=this->Sigma*sp[0]/sp[ia];
  
  vtkImageGaussianSmooth* sm=NULL;
  
  if (this->EnableSmoothing) {
    sm=vtkImageGaussianSmooth::New();
    sm->SetInput(input);
    sm->DebugOn();
    sm->SetStandardDeviations(sig);
    sm->DebugOff();
    sm->Update();
  }
  
  this->UpdateProgress(0.25);

  vtkImageGradient* gradient=vtkImageGradient::New();
  if (this->EnableSmoothing)
    gradient->SetInput(sm->GetOutput());
  else
    gradient->SetInput(input);
  gradient->SetDimensionality(3);
  gradient->Update();
  
  if (this->EnableSmoothing)
    sm->Delete();
  
  this->UpdateProgress(0.5);

  vtkImageData* grad=gradient->GetOutput();
  int dim[3]; grad->GetDimensions(dim);

  fprintf(stderr,"d=%d x %d x %d  c=%d\n",dim[0],dim[1],dim[2],grad->GetNumberOfScalarComponents());

  float gx, gy, gz;

  for (int i=1;i<dim[0]-1;i++)
    for (int j=1;j<dim[1]-1;j++)
      for (int k=1;k<dim[2]-1;k++)
	{
	  gx = grad->GetScalarComponentAsFloat(i,j,k,0);
	  gy = grad->GetScalarComponentAsFloat(i,j,k,1);
	  gz = grad->GetScalarComponentAsFloat(i,j,k,2);
	  
	  output->SetScalarComponentFromFloat(i,j,k,0,gx*gx*this->Sigma);  /* Txx = gx^2 */
	  output->SetScalarComponentFromFloat(i,j,k,1,gx*gy*this->Sigma);  /* Txy = gx*gy */
	  output->SetScalarComponentFromFloat(i,j,k,2,gx*gz*this->Sigma);  /* Txz = gx*gz */
	  output->SetScalarComponentFromFloat(i,j,k,3,gy*gy*this->Sigma);  /* Tyy = gy^2 */
	  output->SetScalarComponentFromFloat(i,j,k,4,gy*gz*this->Sigma);  /* Tyz = gy*gz */
	  output->SetScalarComponentFromFloat(i,j,k,5,gz*gz*this->Sigma);  /* Tzz = gz^2 */	
	}
  
  gradient->Delete();
  this->UpdateProgress(1.0);
}

