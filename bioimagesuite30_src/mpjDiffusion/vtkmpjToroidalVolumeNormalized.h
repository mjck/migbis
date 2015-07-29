

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


#ifndef __vtkmpjToroidalVolumeNormalized_h
#define __vtkmpjToroidalVolumeNormalized_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjToroidalVolumeNormalized : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjToroidalVolumeNormalized *New();
  vtkTypeMacro(vtkmpjToroidalVolumeNormalized,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining the extents for invariants
  // computation. If a mask is not provided, invariants are
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);

  // Description:
  // Set/Get the value for results falling outside the mask
  vtkSetMacro(MaskOutValue,float);
  vtkGetMacro(MaskOutValue,float);
  
   // Description:
  // Set/Get the value for bottom percentage
  vtkSetMacro(BottomPercentage,float);
  vtkGetMacro(BottomPercentage,float);

  // Description:
  // Set/Get the value for top percentage
  vtkSetMacro(TopPercentage,float);
  vtkGetMacro(TopPercentage,float);

  // Description:
  // Set/Get minimum mapping value
  vtkSetMacro(OutMinValue, float);
  vtkGetMacro(OutMinValue, float);

  // Description:
  // Set/Get maximum mapping value
  vtkSetMacro(OutMaxValue, float);
  vtkGetMacro(OutMaxValue, float);

protected:
  float MaskOutValue; 
  float TopPercentage;
  float BottomPercentage;
  float OutMaxValue;
  float OutMinValue;
  vtkImageData *Mask;
  
  vtkmpjToroidalVolumeNormalized();
  virtual ~vtkmpjToroidalVolumeNormalized();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  vtkmpjToroidalVolumeNormalized(const vtkmpjToroidalVolumeNormalized&); // Not implemented
  void operator=(const vtkmpjToroidalVolumeNormalized&); // Not Implemented
};

#endif
