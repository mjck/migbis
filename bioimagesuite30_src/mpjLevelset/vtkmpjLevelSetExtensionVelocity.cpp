

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


#include "vtkmpjLevelSetExtensionVelocity.h"

//------------------------------------------------------------------------------
vtkmpjLevelSetExtensionVelocity* vtkmpjLevelSetExtensionVelocity::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjLevelSetExtensionVelocity");
  if (ret) {
    return (vtkmpjLevelSetExtensionVelocity*)ret;
  }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjLevelSetExtensionVelocity;
}

// Construct object with no children.
vtkmpjLevelSetExtensionVelocity::vtkmpjLevelSetExtensionVelocity()
{
  this->Velocity = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjLevelSetExtensionVelocity::~vtkmpjLevelSetExtensionVelocity()
{
  if (this->Velocity != NULL)
    this->Velocity->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetExtensionVelocity::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  //output->SetNumberOfScalarComponents(1);  
  //output->SetScalarType(VTK_FLOAT);
  //output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetExtensionVelocity::Sort(double values[3]) 
{
  double tmp[3];
  
  if (values[0] < values[1]) {
    if (values[0] < values[2]) {
      if (values[1] < values[2]) {
	tmp[0] = values[0]; tmp[1] = values[1]; tmp[2] = values[2]; 
      } else {
	tmp[0] = values[0]; tmp[1] = values[2]; tmp[2] = values[1]; 
      }
    } else 
      {
	tmp[0] = values[2]; tmp[1] = values[0]; tmp[2] = values[1];
      }
  } else {
    if (values[1] < values[2]) {
      if (values[0] < values[2]) {
	tmp[0] = values[1]; tmp[1] = values[0]; tmp[2] = values[2];
      } else {
	tmp[0] = values[1]; tmp[1] = values[2]; tmp[2] = values[0];
      }
    } else
      {
	tmp[0] = values[2]; tmp[1] = values[1]; tmp[2] = values[0];
      }    
  }

  values[0] = tmp[0];
  values[1] = tmp[1];
  values[2] = tmp[2];
}

// ----------------------------------------------------------------------------
double vtkmpjLevelSetExtensionVelocity::ComputeDistance(vtkImageData *input, int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  vtkDataArray *in = input->GetPointData()->GetScalars();
  
  double center = in->GetComponent(index, 0) - this->LevelSetValue;
  
  if (center == 0.0) {
    return 0.0;
  }

  double distance = VTK_LARGE_FLOAT;
  
  for(int j=0; j<3; j++) {
    
    neigh_dist[j] = VTK_LARGE_FLOAT;
    
    for(int i=-1; i<2; i+=2) {
      
      int neighbor = index + i * factor[j];
      
      if (((coord[j]+i) >= 0) && ((coord[j]+i) < dim[j])) {
	
	double neigh_val = in->GetComponent(neighbor, 0) - this->LevelSetValue;
	
	if (((neigh_val > 0) && (center <= 0.0)) || ((neigh_val < 0) && (center > 0.0))) {
	  distance = center / (center - neigh_val);
	}
	
	if (neigh_dist[j] > distance) {
	  neigh_dist[j] = distance;
	  neigh_index[j] = neighbor;
	}
      }
    }
  }

  this->Sort(neigh_dist);

  distance = 0.0;

  for (int j=0; j<3; j++) {
    if (neigh_dist[j] >= VTK_LARGE_FLOAT) 
      break;
    distance += 1.0 / neigh_dist[j]*neigh_dist[j];    
  }

  if (distance == 0.0) 
    return VTK_LARGE_FLOAT;
  
  distance = sqrt(1.0 / distance);

  return distance;
}

// ----------------------------------------------------------------------------
double vtkmpjLevelSetExtensionVelocity::ComputeVelocity(vtkImageData *input, int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  vtkDataArray *in = input->GetPointData()->GetScalars();
  vtkDataArray *vel = this->Velocity->GetPointData()->GetScalars();

  double distance = this->ComputeDistance(input, coord, index);

  if (distance >= VTK_LARGE_FLOAT) {
    return distance;
  }

  
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetExtensionVelocity::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  vtkDataArray *in = input->GetPointData()->GetScalars();
  vtkDataArray *out = output->GetPointData()->GetScalars();

  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int total = in->GetNumberOfTuples();

  this->pog = 0.0;
  this->tenth=(int)(total / 10.0);
  this->count=0;

  this->UpdateProgress(0.01);

  int coord[3];
  for(int index=0; index<total; index++) {
    
    // decompose index into point coordinates
    coord[2] = index / dim0Tdim1; 
    coord[1] = (index % dim0Tdim1) / dim[0];
    coord[0] = (index % dim0Tdim1) % dim[0];

    // compute extension velocity
    double dist = ComputeVelocity(input, coord, index);
    
    // write out distance
    out->SetComponent(index, 0, (float)dist);
    
  }
  
  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
