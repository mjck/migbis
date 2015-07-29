

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


#ifndef __vtkmpjTensorLeastSquares_h
#define __vtkmpjTensorLeastSquares_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkDoubleArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjApparentDiffusion : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjApparentDiffusion *New();
  vtkTypeMacro(vtkmpjApparentDiffusion,vtkpxSimpleImageToImageFilter);
  
  // Sets/Gets the baseline image A(b=0); the echo amplitude without any
  // diffusion attenuation
  vtkSetObjectMacro(Baseline, vtkImageData);
  vtkGetObjectMacro(Baseline, vtkImageData);
  
  // Description: 
  // Sets/Gets mask for determining the extents of tensor
  // computation. If a mask is not provided, tensor is
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);

  // Description:
  // Set/Get Shift Factor so that S(b)/S(0) can be computed when S(0)=0
  // ADC = (S(b)+shift)/(S(0)+shift), shift ~= 0
  vtkSetMacro(Shift,float);
  vtkGetMacro(Shift,float);
  
  // Description:
  // Set/Get the b-Value
  vtkSetMacro(bValue,float);
  vtkGetMacro(bValue,float);
  
  // Description: Replace DWs with the computed ADCs or keep DWs as 
  // scalars and add a new "diffusion" ADC field to the dataset
  vtkGetMacro(ReplaceScalars,int);
  vtkSetClampMacro(ReplaceScalars,int,0,1);
  vtkBooleanMacro(ReplaceScalars,int);

 protected:
  float Shift;
  float bValue;
  int ReplaceScalars;
  vtkImageData *Baseline;
  vtkImageData *Mask;

  vtkmpjApparentDiffusion();
  virtual ~vtkmpjApparentDiffusion();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  
  vtkmpjApparentDiffusion(const vtkmpjApparentDiffusion&); // Not implemented
  void operator=(const vtkmpjApparentDiffusion&); // Not implemented
};

#endif
