

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



#ifndef __vtkmpjImageStatistics_h
#define __vtkmpjImageStatistics_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"

class vtkmpjImageStatistics : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageStatistics *New();
  vtkTypeMacro(vtkmpjImageStatistics,vtkpxSimpleImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Get single-region statistics information for the data
  vtkGetMacro(Min, double);
  vtkGetMacro(Max, double);
  vtkGetMacro(Mean, double);
  vtkGetMacro(Variance, double);
  vtkGetMacro(VoxelCount, long int);
  
  // Description::
  // Get/Set Number of regions in the mask
  vtkGetMacro(NumberOfRegions,int);
  vtkSetMacro(NumberOfRegions,int);
  
  // Description: 
  // Sets/Gets mask for computing statistics.
  // If a mask is not provided, tensor is
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
  // Gets region statistics
  double GetRegionMax(int index) { return this->RegionMax[index]; }
  double GetRegionMin(int index) { return this->RegionMin[index]; }
  double GetRegionMean(int index) { return this->RegionMean[index]; }
  double GetRegionVariance(int index) { return this->RegionVariance[index]; }
  long GetRegionVoxelCount(int index) { return this->RegionVoxelCount[index]; }

 protected:
  int NumberOfRegions;  
  vtkFloatArray *RegionValues;
  vtkImageData *Mask;

  double *RegionMax;
  double *RegionMin;
  double *RegionMean;
  double *RegionVariance;
  double *RegionSum;
  double *RegionSumSqr;
  long *RegionVoxelCount;

  double Min;
  double Max;
  double Mean;
  double Variance;
  long int VoxelCount;
  
  vtkmpjImageStatistics();
  virtual ~vtkmpjImageStatistics();
  
  virtual void SimpleExecute(vtkImageData* in, vtkImageData* out);
  //  virtual void ExecuteInformation();

 private:
  vtkmpjImageStatistics(const vtkmpjImageStatistics&); // Not implemented
  void operator=(const vtkmpjImageStatistics&); // Not implemented
};

#endif


