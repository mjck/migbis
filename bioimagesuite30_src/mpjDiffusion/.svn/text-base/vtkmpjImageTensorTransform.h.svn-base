

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


/* -- vtkmpjImageTensorTransform.h 

      Marcel Jackowski, 2006.
*/

#ifndef __vtkmpjImageTensorTransform_h 
#define __vtkmpjImageTensorTransform_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkpxMath.h"

class vtkmpjImageTensorTransform : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageTensorTransform *New();
  vtkTypeMacro(vtkmpjImageTensorTransform,vtkpxSimpleImageToImageFilter);
  
  vtkGetVectorMacro(Rotation, float, 3);
  vtkSetVectorMacro(Rotation, float, 3);
  
 protected: 
  float Rotation[3];
  
  vtkmpjImageTensorTransform();
  ~vtkmpjImageTensorTransform();
  
  virtual void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  
 private:
  void PrintMatrix3x3(char *title, float M[3][3]);
  vtkmpjImageTensorTransform(const vtkmpjImageTensorTransform&); // Not implemented
  void operator=(const vtkmpjImageTensorTransform&); // Not implemented
}; 

#endif
