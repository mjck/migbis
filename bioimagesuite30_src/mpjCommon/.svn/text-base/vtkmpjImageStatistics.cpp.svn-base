

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
#include "vtkmpjImageStatistics.h"
#include "pxisinf.h"


//------------------------------------------------------------------------------
vtkmpjImageStatistics* vtkmpjImageStatistics::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageStatistics");
  if(ret)
    {
    return (vtkmpjImageStatistics*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageStatistics;
}

// Construct object with no children.
vtkmpjImageStatistics::vtkmpjImageStatistics()
{
  this->NumberOfRegions = 1;
  this->RegionMax = NULL;
  this->RegionMin = NULL;
  this->RegionMean = NULL;
  this->RegionVariance = NULL;
  this->RegionVoxelCount = NULL;
  this->Min = this->Max = this->Mean = this->Variance = 0.0;
  this->VoxelCount = 0;
  this->Mask = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjImageStatistics::~vtkmpjImageStatistics()
{ 
  if (this->Mask != NULL)
    this->Mask->Delete();  
  if (this->RegionMax != NULL)
    delete[] this->RegionMax;
  if (this->RegionMin != NULL)
    delete[] this->RegionMin;
  if (this->RegionMean != NULL)
    delete[] this->RegionMean;
  if (this->RegionVariance != NULL)
    delete[] this->RegionVariance;
  if (this->RegionVoxelCount != NULL)
    delete[] this->RegionVoxelCount;
}

// ----------------------------------------------------------------------------
void vtkmpjImageStatistics::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
  vtkDataArray *in=input->GetPointData()->GetScalars();
  int numscalars=in->GetNumberOfTuples();
   
  vtkDataArray *mask=NULL;
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  // make sure number of regions is at least 1
  if (this->NumberOfRegions <= 0)
    this->NumberOfRegions = 1;
  
  // reset statistics
  this->RegionSum = new double[this->NumberOfRegions];
  this->RegionSumSqr = new double[this->NumberOfRegions];
  this->RegionMax = new double[this->NumberOfRegions];
  this->RegionMin = new double[this->NumberOfRegions];
  this->RegionMean = new double[this->NumberOfRegions];
  this->RegionVariance = new double[this->NumberOfRegions];
  this->RegionVoxelCount = new long[this->NumberOfRegions];

  for(int j=0; j<this->NumberOfRegions; j++) {
    this->RegionSum[j] = 0.0;
    this->RegionSumSqr[j] = 0.0;
    this->RegionMax[j] = VTK_DOUBLE_MIN;
    this->RegionMin[j] = VTK_DOUBLE_MAX;
    this->RegionMean[j] = 0.0;
    this->RegionVariance[j] = 0.0;
    this->RegionVoxelCount[j] = 0;
  }
  
  // Progress computation
  int count = 0;
  float pog = 0.0;
  int tenth= (int)(numscalars / 10.0);
    
  this->UpdateProgress(0.01);
  
  for (int n=0;n<numscalars;n++)      
    {      
      int index = (int)((mask != NULL)?(mask->GetComponent(n, 0)):0);
      index--;

      if (index >= 0 && index < this->NumberOfRegions) {
	
	double value = (double)in->GetComponent(n, 0);
	
	if (!isnan(value)) {
	  
	  this->RegionSum[index] += value;
	  this->RegionSumSqr[index] += value*value;
	  
	  if (value > this->RegionMax[index]) {
	    this->RegionMax[index] = value;
	  } else if (value < this->RegionMin[index]) {
	    this->RegionMin[index] = value;
	  }
	  
	  this->RegionVoxelCount[index]++;
	}
      }
      
      count++;
      if (count==tenth)
	{
	  pog+=0.1;
	  this->UpdateProgress(pog);
	  count=0;
	}
    }
  
  
  // compute statistics
  for(int j=0; j<this->NumberOfRegions; j++) {
    
    if (this->RegionVoxelCount[j]) {
      this->RegionMean[j]=this->RegionSum[j] / (double)this->RegionVoxelCount[j];
      this->RegionVariance[j]=(this->RegionSumSqr[j] - ((double)this->RegionVoxelCount[j] * this->RegionMean[j] * this->RegionMean[j]))  / (double)(this->RegionVoxelCount[j]-1);
    } else {
      this->RegionMean[j] = this->RegionVariance[j] = 0.0;
    }
  }
  
  this->Min = this->RegionMin[0];
  this->Max = this->RegionMax[0];
  this->Mean = this->RegionMean[0];
  this->Variance = this->RegionVariance[0];
  this->VoxelCount = this->RegionVoxelCount[0];

  this->UpdateProgress(1.0);
}

void vtkmpjImageStatistics::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkpxSimpleImageToImageFilter::PrintSelf(os,indent);

  os << indent << "Mean: " << this->Mean << "\n";
  os << indent << "Minimum: " << this->Min << "\n";
  os << indent << "Maximum: " << this->Max << "\n";
  os << indent << "Variance: " << this->Variance << "\n";
  os << indent << "VoxelCount: " << this->VoxelCount << "\n";
  
}
