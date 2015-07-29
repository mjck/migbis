

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


/* -- vtkmpjImageTensorVariance.h 
   
  Computes the intervoxel variance

  Essentially the variance of the dot product of the principal eigenvector
  in a neighborhood.

  Marcel Jackowski, December 2005.

*/

#ifndef __vtkmpjImageTensorVariance_h
#define __vtkmpjImageTensorVariance_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImageTensorVariance : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageTensorVariance *New();
  vtkTypeMacro(vtkmpjImageTensorVariance,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining the extents for invariants
  // computation. If a mask is not provided, invariants are
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);

  // Description:
  // Set/Get the kernel size for computations;
  vtkSetVector3Macro(KernelSize,int);
  vtkGetVector3Macro(KernelSize,int);
  
protected:
  int KernelSize[3];
  float MaskOutValue; 
  vtkImageData *Mask;
  
  vtkmpjImageTensorVariance();
  virtual ~vtkmpjImageTensorVariance();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  vtkmpjImageTensorVariance(const vtkmpjImageTensorVariance&); // Not implemented
  void operator=(const vtkmpjImageTensorVariance&); // Not Implemented
};

#endif
