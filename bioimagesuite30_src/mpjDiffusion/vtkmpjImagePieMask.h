

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


#ifndef __vtkmpjImagePieMask_h
#define __vtkmpjImagePieMask_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImagePieMask : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImagePieMask *New();
  vtkTypeMacro(vtkmpjImagePieMask,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining the extents for invariants
  // computation. If a mask is not provided, invariants are
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);

  // Description:
  // Set/Get the value for number of slices in the pie
  vtkSetMacro(NumberOfSlices,int);
  vtkGetMacro(NumberOfSlices,int);
  
// Description:
  // Set/Get the value for number of pies
  vtkSetMacro(NumberOfPies,int);
  vtkGetMacro(NumberOfPies,int);

  // Description:
  // Set/Get the apex center of the pie   
  vtkGetVectorMacro(Apex, float, 3);
  vtkSetVectorMacro(Apex, float, 3);

  // Description:
  // Set/Get the base center of the pie   
  vtkGetVectorMacro(Base, float, 3);
  vtkSetVectorMacro(Base, float, 3);

  // Description:
  // Set/Get the apex point
  vtkGetVectorMacro(Anchor, float, 3);
  vtkSetVectorMacro(Anchor, float, 3);

  // Description:
  // Set/Get mask out value
  vtkGetMacro(MaskOutValue, float);
  vtkSetMacro(MaskOutValue, float);

  // Description:
  // Set/Get mask out angle
  vtkGetMacro(MaskOutAngle, float);
  vtkSetMacro(MaskOutAngle, float);

protected:
  int NumberOfSlices;
  int NumberOfPies;
  float Anchor[3];  
  float Apex[3];
  float Base[3];
  float MaskOutValue; 
  float MaskOutAngle;
  vtkImageData *Mask;
  
  vtkmpjImagePieMask();
  virtual ~vtkmpjImagePieMask();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  vtkmpjImagePieMask(const vtkmpjImagePieMask&); // Not implemented
  void operator=(const vtkmpjImagePieMask&); // Not Implemented
};

#endif
