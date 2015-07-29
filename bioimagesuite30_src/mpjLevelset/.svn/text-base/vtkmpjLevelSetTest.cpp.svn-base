

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


#include "vtkmpjLevelSetTest.h"

//------------------------------------------------------------------------------
vtkmpjLevelSetTest* vtkmpjLevelSetTest::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjLevelSetTest");
  if (ret) {
    return (vtkmpjLevelSetTest*)ret;
  }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjLevelSetTest;
}

// Construct object with no children.
vtkmpjLevelSetTest::vtkmpjLevelSetTest()
{
  this->SpeedImage = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjLevelSetTest::~vtkmpjLevelSetTest()
{
  if (this->SpeedImage != NULL)
    this->SpeedImage->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetTest::ExecuteInformation()
{  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  if (this->SpeedImage == NULL) {
    vtkErrorMacro(<<"no speed image!");
    return;
  }
}

// ----------------------------------------------------------------------------
double vtkmpjLevelSetTest::ComputeAdvection(vtkImageData *image, double v[3], 
					    int index, int coord[3])
{
  vtkDataArray *phi = image->GetPointData()->GetScalars();  
  int dim[3]; image->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  
  double advection = 0.0;    
  double value = image->GetPointData()->GetScalars()->GetComponent(index, 0);
  double diff_val = 0.0;
  
  for(int j=0; j<3; j++) {
    
    // compute backward differences
    if (coord[j] > 0) {      

      diff_val = value - phi->GetComponent(index-factor[j], 0);
      
      if (v[j] >= 0) {	
	advection += v[j] * diff_val;
      }
    }

    // compute forward differences
    if (coord[j] < dim[j]-1) {
      
      diff_val = phi->GetComponent(index+factor[j], 0) - value;
      
      if (v[j] < 0) {
	advection += v[j] * diff_val;
      }
    }
  }

  return advection;
}


// ----------------------------------------------------------------------------
double vtkmpjLevelSetTest::ComputeSpeed(vtkImageData *image, int index, int coord[3])
{
  vtkDataArray *speed = this->SpeedImage->GetPointData()->GetScalars();
  
  double v[3] ={1.0, 0.0, 0.0};
  
  //v[0] = speed->GetComponent(index, 0);
  //v[1] = speed->GetComponent(index, 1);
  //v[2] = speed->GetComponent(index, 2);  
  //(void)vtkMath::Normalize(v);  
  //double fa = speed->GetComponent(index, 0);  
  //printf("fa=%f\n",fa);  
  //  double entropy = ComputeEntropyGradientMagnitude(image, advection, index, coord);
  
  float normal[3];
  
  this->ComputeNormal(image, index, coord, normal);
  double dot = v[0]*normal[0] + v[1]*normal[1] + v[2]*normal[2];
  
  //double advection = ComputeAdvection(image, dot, index, coord);   
  
  //double entropy = ComputeEntropyGradientMagnitude(image, fabs(dot), index, coord);

  //  printf("entropy=%f\n",entropy); 

  return dot;

}


