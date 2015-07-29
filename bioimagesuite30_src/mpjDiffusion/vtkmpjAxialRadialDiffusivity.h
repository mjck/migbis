

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


#ifndef __vtkmpjAxialRadialDiffusivity_h
#define __vtkmpjAxialRadialDiffusivity_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjAxialRadialDiffusivity : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjAxialRadialDiffusivity *New();
  vtkTypeMacro(vtkmpjAxialRadialDiffusivity,vtkpxSimpleImageToImageFilter);
  
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
  
protected:
  float MaskOutValue; 
  vtkImageData *Mask;
  
  vtkmpjAxialRadialDiffusivity();
  virtual ~vtkmpjAxialRadialDiffusivity();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  vtkmpjAxialRadialDiffusivity(const vtkmpjAxialRadialDiffusivity&); // Not implemented
  void operator=(const vtkmpjAxialRadialDiffusivity&); // Not Implemented
};

#endif
