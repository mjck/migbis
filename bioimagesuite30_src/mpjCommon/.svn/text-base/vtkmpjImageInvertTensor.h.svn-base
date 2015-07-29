

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


/* -- vtkmpjImageInvertTensor.h 
   
   Inverts a symmetric tensor image.

*/

#ifndef __vtkmpjImageInvertTensor_h
#define __vtkmpjImageInvertTensor_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImageInvertTensor : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageInvertTensor *New();
  vtkTypeMacro(vtkmpjImageInvertTensor,vtkpxSimpleImageToImageFilter);

  // Description:
  // Sets/Gets mask where tensor are to be inverted
  vtkGetObjectMacro(Mask, vtkImageData);
  vtkSetObjectMacro(Mask, vtkImageData);

  // Description:
  // Sets/Gets infinity value
  vtkGetMacro(LargeValue, float);
  vtkSetMacro(LargeValue, float);
   
 protected:
  vtkImageData *Mask;
  float LargeValue;
  
  vtkmpjImageInvertTensor();
  virtual ~vtkmpjImageInvertTensor();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();
  
 private:
  vtkmpjImageInvertTensor(const vtkmpjImageInvertTensor&); // Not implemented
  void operator=(const vtkmpjImageInvertTensor&); // Not Implemented

  void SortEigenvectors(float w[3], float V[3][3]); // By decreasing magnitude
  void InvertFullTensor(vtkImageData *input, vtkImageData *output);
  void InvertReducedTensor(vtkImageData *input, vtkImageData *output);
  
};

#endif
