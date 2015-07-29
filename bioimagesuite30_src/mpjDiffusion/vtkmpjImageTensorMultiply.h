

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


/* -- vtkmpjImageTensorMultiply.h 

      Marcel Jackowski, 2003.
*/

#ifndef __vtkmpjImageTensorMultiply_h 
#define __vtkmpjImageTensorMultiply_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"

class vtkmpjImageTensorMultiply : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageTensorMultiply *New();
  vtkTypeMacro(vtkmpjImageTensorMultiply,vtkpxSimpleImageToImageFilter);
  
  void SetInput1(vtkImageData *input);
  void SetInput2(vtkImageData *input);
  vtkImageData *GetInput(int index);

  vtkGetMacro(NoiseLevel, float);
  vtkSetMacro(NoiseLevel, float);

 protected: 
  float NoiseLevel;

  vtkmpjImageTensorMultiply();
  ~vtkmpjImageTensorMultiply();

  float GetGaussianDeviate();
  virtual void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  
 private:
  void PrintMatrix3x3(char *title, float M[3][3]);
  vtkmpjImageTensorMultiply(const vtkmpjImageTensorMultiply&); // Not implemented
  void operator=(const vtkmpjImageTensorMultiply&); // Not implemented
}; 

#endif
