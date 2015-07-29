

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


#ifndef __vtkmpjImageCharacteristics_h
#define __vtkmpjImageCharacteristics_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjImageCharacteristics : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjImageCharacteristics *New();
  vtkTypeMacro(vtkmpjImageCharacteristics,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining propagation extents
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
  // Sets/Gets tensor information
  vtkSetObjectMacro(Tensor, vtkImageData);
  vtkGetObjectMacro(Tensor, vtkImageData);

  // Sets/Gets gradient of u-map
  vtkSetObjectMacro(Gradient, vtkImageData);
  vtkGetObjectMacro(Gradient, vtkImageData);

  vtkSetMacro(CoefficientA, float);
  vtkGetMacro(CoefficientA, float);
  
  vtkSetMacro(CoefficientB, float);
  vtkGetMacro(CoefficientB, float);
  
  vtkSetMacro(CoefficientC, float);
  vtkGetMacro(CoefficientC, float);

 protected:
  vtkImageData *Mask;
  vtkImageData *Tensor;
  vtkImageData *Gradient;
  float CoefficientA;
  float CoefficientB;
  float CoefficientC;
  vtkmpjImageCharacteristics();
  virtual ~vtkmpjImageCharacteristics();
  
  void Jacobi3x3(float a[3][3], float w[3], float v[3][3]);
  void NormalizeTensor3x3(float T[3][3], float w[3], float V[3][3], float N[3][3]);
  void FindEllipsoidPoint(float n[3], float w[3], float T[3][3], float N[3][3], float V[3][3], float FA);
  void ComputeCharacteristics(vtkImageData *input, vtkImageData *output, int coord[3], int index);
  void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  int InsideMask(int index);
  
 private: 
  //BTX
  vtkDataArray *mask;
  //ETX
  vtkmpjImageCharacteristics(const vtkmpjImageCharacteristics&);// Not implemented
  void operator=(const vtkmpjImageCharacteristics&); // Not Implemented
};

#endif
