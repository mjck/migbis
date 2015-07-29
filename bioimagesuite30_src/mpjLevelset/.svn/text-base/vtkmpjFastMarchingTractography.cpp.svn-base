

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



#include "vtkmpjFastMarchingTractography.h"

//------------------------------------------------------------------------------
vtkmpjFastMarchingTractography* vtkmpjFastMarchingTractography::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjFastMarchingTractography");
  if(ret)
    {
      return (vtkmpjFastMarchingTractography*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjFastMarchingTractography;
}

// Construct object with no children.
vtkmpjFastMarchingTractography::vtkmpjFastMarchingTractography()
{
  this->SpeedFunctionIndex = 0;
}

// ----------------------------------------------------------------------------
vtkmpjFastMarchingTractography::~vtkmpjFastMarchingTractography()
{
}

double vtkmpjFastMarchingTractography::F0(vtkImageData *input, vtkImageData *output, int coord[3], int index, double normal[3])
{
  float fa = input->GetPointData()->GetScalars()->GetComponent(index, 0);
  return (fa+1);
}

double vtkmpjFastMarchingTractography::F1(vtkImageData *input, vtkImageData *output, int coord[3], int index, double normal[3])
{
  double vector[3];
  double fa = input->GetPointData()->GetScalars()->GetComponent(index, 0);

  vector[0] = input->GetPointData()->GetScalars()->GetComponent(index, 1);
  vector[1] = input->GetPointData()->GetScalars()->GetComponent(index, 2);
  vector[2] = input->GetPointData()->GetScalars()->GetComponent(index, 3);

  (void)vtkMath::Normalize(vector);
  (void)vtkMath::Normalize(normal);

  float dp = vector[0]*normal[0] + vector[1]*normal[1] + vector[2]*normal[2];
  float dp2 = dp*dp;
  
  if (this->Debug) {
    printf("f:vector=%f %f %f\n",vector[0],vector[1],vector[2]);
    printf("f:normal=%f %f %f\n",normal[0],normal[1],normal[2]);
    printf("dp2=%f\n",fabs(dp2));
  }
  /*printf("dp2=%f\n",dp2);*/
  return fabs(dp2);
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingTractography::F(vtkImageData *input, vtkImageData *output, int coord[3], int index, double normal[3])
{
  double result = 1.0;

  switch(this->SpeedFunctionIndex) {
  case 1:
    result = this->F1(input, output, coord, index, normal);
    break;
  default:
    result = this->F0(input, output, coord, index, normal);
  }
  
  return result;
}

// ----------------------------------------------------------------------------
