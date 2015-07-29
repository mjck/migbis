

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


#include "vtkmpjLevelSetNarrowBandFilter.h"

//------------------------------------------------------------------------------
vtkmpjLevelSetNarrowBandFilter* vtkmpjLevelSetNarrowBandFilter::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjLevelSetNarrowBandFilter");
  if (ret) {
    return (vtkmpjLevelSetNarrowBandFilter*)ret;
  }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjLevelSetNarrowBandFilter;
}

// Construct object with no children.
vtkmpjLevelSetNarrowBandFilter::vtkmpjLevelSetNarrowBandFilter()
{
  this->NarrowBandwidth = 12;
  this->LevelSetValue = 0;
  this->NarrowBandPoints = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjLevelSetNarrowBandFilter::~vtkmpjLevelSetNarrowBandFilter()
{
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetNarrowBandFilter::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetNarrowBandFilter::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  //  int factor[3] = {1, dim[0], dim0Tdim1};
  float maxwidth = this->NarrowBandwidth / 2.0;
  
  vtkDataArray *in = input->GetPointData()->GetScalars();
  vtkDataArray *out = output->GetPointData()->GetScalars();

  int total = in->GetNumberOfTuples();
  int coord[3];

  if (this->NarrowBandPoints != NULL) {
    this->NarrowBandPoints->Delete();
  }
  this->NarrowBandPoints = vtkFloatArray::New();
  this->NarrowBandPoints->SetNumberOfComponents(4);

  this->pog = 0.0;  
  this->tenth=(int)(total / 10.0);
  this->count = 0;
  
  this->UpdateProgress(0.01);
  
  for (int index=0; index<total; index++) {
    
    if (this->count==this->tenth)
      {
	this->pog+=0.1;
	this->UpdateProgress(this->pog);
	this->count=0;
      }
    
    // decompose linear index into point coordinates
    coord[2] = index / dim0Tdim1; 
    coord[1] = (index % dim0Tdim1) / dim[0];
    coord[0] = (index % dim0Tdim1) % dim[0];
    
    // get level set value
    float value = in->GetComponent(index, 0);
    
    // keep narrow band values
    if ((value >= -maxwidth) && (value <= maxwidth)) {     
      
      out->SetComponent(index, 0, 1.0);     
      
      this->NarrowBandPoints->InsertNextTuple4((float)coord[0], (float)coord[1], 
					       (float)coord[2], (float)value);
    }
    
  }

  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
