

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



#include "vtkmpjFastMarchingTensor.h"

//------------------------------------------------------------------------------
vtkmpjFastMarchingTensor* vtkmpjFastMarchingTensor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjFastMarchingTensor");
  if(ret)
    {
      return (vtkmpjFastMarchingTensor*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjFastMarchingTensor;
}

// Construct object with no children.
vtkmpjFastMarchingTensor::vtkmpjFastMarchingTensor()
{
}

// ----------------------------------------------------------------------------
vtkmpjFastMarchingTensor::~vtkmpjFastMarchingTensor()
{
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingTensor::F(vtkImageData *input, vtkImageData *output,
				   int coord[3], int index)
{
  float val = input->GetPointData()->GetScalars()->GetComponent(index, 0);
  /*  fprintf(stderr,"val=%f\n",val);*/
  return (double)val;
}
// ----------------------------------------------------------------------------
