

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


/* -- vtkmpjImageBoundary.h 
   
      Marcel Jackowski, Feb. 2004.

*/

#ifndef __vtkmpjImageBoundary_h
#define __vtkmpjImageBoundary_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImageBoundary : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageBoundary *New();
  vtkTypeMacro(vtkmpjImageBoundary,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining the extents for invariants
  // computation. If a mask is not provided, invariants are
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
  // Description:
  // Set/Get the value for boundary results
  vtkSetMacro(BoundaryOutValue,float);
  vtkGetMacro(BoundaryOutValue,float);

  // Description:
  // Set/Get the value for boundary results
  vtkSetMacro(MaskOutValue,float);
  vtkGetMacro(MaskOutValue,float);

  // Set/Get the value for boundary results
  vtkSetMacro(Boundary2D,int);
  vtkGetMacro(Boundary2D,int);
  
  // Description:
  // Set/Get Array of boundary positions
  vtkSetObjectMacro(BoundaryPoints,vtkFloatArray);
  vtkGetObjectMacro(BoundaryPoints,vtkFloatArray);
  
 protected:
  float BoundaryOutValue; 
  float MaskOutValue;
  vtkFloatArray *BoundaryPoints;
  vtkImageData *Mask;
  int Boundary2D;
  
  vtkmpjImageBoundary();
  virtual ~vtkmpjImageBoundary();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();
  
 private:
  vtkmpjImageBoundary(const vtkmpjImageBoundary&); // Not implemented
  void operator=(const vtkmpjImageBoundary&); // Not Implemented
};

#endif
