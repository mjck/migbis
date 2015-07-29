

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


/* -- vtkmpjImageSymmetricTensor.h 

      Marcel Jackowski, 2003.
*/

#ifndef __vtkmpjImageSymmetricTensor_h 
#define __vtkmpjImageSymmetricTensor_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"

#define VTK_MPJ_OUTPUT_FULL_TENSOR      0
#define VTK_MPJ_OUTPUT_SYMMETRIC_TENSOR 1

class vtkmpjImageSymmetricTensor : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageSymmetricTensor *New();
  vtkTypeMacro(vtkmpjImageSymmetricTensor,vtkpxSimpleImageToImageFilter);
  
  // Description: Set/Get eigenvalues
  vtkSetVector3Macro(Eigenvalues,float);
  vtkGetVector3Macro(Eigenvalues,float);
  
  // Description: Set/Get direction
  vtkSetVector3Macro(Direction,float);
  vtkGetVector3Macro(Direction,float);
  
  // Description: Turn noise on/off
  vtkGetMacro(AddNoise,int);
  vtkSetClampMacro(AddNoise,int,0,1);
  vtkBooleanMacro(AddNoise,int);

  // Description: Enable inverse mask
  vtkGetMacro(InverseMask,int);
  vtkSetClampMacro(InverseMask,int,0,1);
  vtkBooleanMacro(InverseMask,int);

  // Description: Use input vector
  vtkGetMacro(UseInputForDirection, int);
  vtkSetClampMacro(UseInputForDirection, int,0,1);
  vtkBooleanMacro(UseInputForDirection, int);

  // Description: Output type
  vtkGetMacro(OutputType,int);
  vtkSetClampMacro(OutputType,int,VTK_MPJ_OUTPUT_FULL_TENSOR,VTK_MPJ_OUTPUT_SYMMETRIC_TENSOR);

  void SetOutputTypeToFullTensor() { this->OutputType=VTK_MPJ_OUTPUT_FULL_TENSOR; };
  void SetOutputTypeToSymmetricTensor() { this->OutputType=VTK_MPJ_OUTPUT_SYMMETRIC_TENSOR; };

  // Description:
  // Set/Get noise % level 
  vtkGetMacro(NoiseLevel,float);
  vtkSetClampMacro(NoiseLevel,float,0.0001,100.0);

  // Description:
  // Set/Get Angle to construct perpendicular vectors for tensor
  vtkGetMacro(Angle,float);
  vtkSetMacro(Angle,float);

  // Description: 
  // Sets/Gets mask for determining the extents of tensor
  // computation. If a mask is not provided, tensor is
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
 protected: 
  float Angle;
  int AddNoise;
  int OutputType;
  int InverseMask;
  int UseInputForDirection;
  float Eigenvalues[3];
  float Direction[3];
  float NoiseLevel;
  vtkImageData *Mask;
  
  vtkmpjImageSymmetricTensor();
  ~vtkmpjImageSymmetricTensor();
  
  float GetGaussianDeviate();
  void PrintMatrix3x3(char *title, float M[3][3]);

  virtual void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  
 private:
  vtkmpjImageSymmetricTensor(const vtkmpjImageSymmetricTensor&); // Not implemented
  void operator=(const vtkmpjImageSymmetricTensor&); // Not implemented
}; 

#endif
