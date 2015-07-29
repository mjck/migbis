

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


#include "vtkmpjExtractLevelSet.h"

vtkStandardNewMacro(vtkmpjExtractLevelSet);

// Construct object with no children.
vtkmpjExtractLevelSet::vtkmpjExtractLevelSet()
{
  this->LevelSetValue = 0.0;
  this->InValue = -1.0;
  this->OutValue = 1.0;
  this->InsidePoints = NULL;
  this->OutsidePoints = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjExtractLevelSet::~vtkmpjExtractLevelSet()
{
  if (this->InsidePoints != NULL) 
    this->InsidePoints->Delete();
  if (this->OutsidePoints != NULL) 
    this->OutsidePoints->Delete();
}

// ----------------------------------------------------------------------------
//void vtkmpjExtractLevelSet::ExecuteInformation()
//{
//this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
//}

// ----------------------------------------------------------------------------
void vtkmpjExtractLevelSet::Sort(double values[3]) 
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
double vtkmpjExtractLevelSet::ComputeDistance(vtkImageData *input, int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  vtkDataArray *in = input->GetPointData()->GetScalars();
  
  double center = in->GetComponent(index, 0) - this->LevelSetValue;
  
  if (center == 0.0) {
    return 0.0;
  }

  int neigh_index[3];
  double neigh_dist[3];
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
void vtkmpjExtractLevelSet::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  vtkDataArray *in = input->GetPointData()->GetScalars();
  vtkDataArray *out = output->GetPointData()->GetScalars();

  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int total = in->GetNumberOfTuples();
  
  // create array of inside points
  if (this->InsidePoints != NULL) 
    this->InsidePoints->Delete();  
  
  this->InsidePoints = vtkFloatArray::New(); 
  this->InsidePoints->SetNumberOfComponents(4);
  
  // create array of outside points
  if (this->OutsidePoints != NULL) 
    this->OutsidePoints->Delete();  
  
  this->OutsidePoints = vtkFloatArray::New(); 
  this->OutsidePoints->SetNumberOfComponents(4);
    
  this->pog = 0.0;
  this->tenth=(int)(total / 10.0);
  this->count=0;
  
  this->UpdateProgress(0.01);
  
  int coord[3];
  for(int index=0; index<total; index++) {
    
    this->count++;
    if (this->count==this->tenth)
      {
	this->pog+=0.1;
	this->UpdateProgress(this->pog);
	this->count=0;
      }
    
    // decompose index into point coordinates
    coord[2] = index / dim0Tdim1; 
    coord[1] = (index % dim0Tdim1) / dim[0];
    coord[0] = (index % dim0Tdim1) % dim[0];

    // determine if point is inside or outside the leveset
    int inside = (in->GetComponent(index, 0) - this->LevelSetValue) <= 0;

    // compute distance to the level set          
    double dist = ComputeDistance(input, coord, index);
    
    // write out distances
    if (inside) {
      
      // write out mask
      out->SetComponent(index, 0, (float)InValue);
      
      if (dist != VTK_LARGE_FLOAT) 
	this->InsidePoints->InsertNextTuple4((float)coord[0], (float)coord[1], 
					     (float)coord[2], dist);
    } else {
      
      // write out mask
      out->SetComponent(index, 0, (float)OutValue);
      
      if (dist != VTK_LARGE_FLOAT)
	this->OutsidePoints->InsertNextTuple4((float)coord[0], (float)coord[1], 
					      (float)coord[2], dist);
    }
  }
  
  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
