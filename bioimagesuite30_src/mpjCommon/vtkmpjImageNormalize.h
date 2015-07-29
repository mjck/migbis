

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



#ifndef __vtkmpjImageNormalize_h
#define __vtkmpjImageNormalize_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"

class vtkmpjImageNormalize : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageNormalize *New();
  vtkTypeMacro(vtkmpjImageNormalize,vtkpxSimpleImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description::
  // Get/Set Number of regions in the mask
  vtkGetMacro(NumberOfRegions,int);
  vtkSetMacro(NumberOfRegions,int);
  
  // Description:
  // Get/Set region statistics - min, max, mean, etc...
  vtkSetObjectMacro(RegionStatistics, vtkFloatArray);
  vtkGetObjectMacro(RegionStatistics, vtkFloatArray);

  // Description: 
  // Sets/Gets mask for computing statistics.
  // If a mask is not provided, tensor is
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
 protected:
  int NumberOfRegions;  
  vtkImageData *Mask;
  vtkFloatArray *RegionStatistics;
    
  vtkmpjImageNormalize();
  virtual ~vtkmpjImageNormalize();
  
  virtual void SimpleExecute(vtkImageData* in, vtkImageData* out);
  void ExecuteInformation();

 private:
  vtkmpjImageNormalize(const vtkmpjImageNormalize&); // Not implemented
  void operator=(const vtkmpjImageNormalize&); // Not implemented
};

#endif


