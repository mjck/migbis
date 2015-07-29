

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


/* -- vtkmpjTensorToApparentDiffusion.h 
   
      Marcel Jackowski, 2002

      Given tensor, and N given gradient directions, determine ADCS:

      A * X = ADC

      where ADC_i = Si(0)/S(b) and
      A_i = [gx_i^2 gy_i^2 gz_i^2 2*gxgy_i 2*gxgz_i 2*gygz_i]

      size(A) = 6 x N
      size(X) = 1 x 6
      size(ADC) = N x 1
      
*/

#ifndef __vtkmpjTensorToApparentDiffusion_h
#define __vtkmpjTensorToApparentDiffusion_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkDoubleArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjTensorToApparentDiffusion : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjTensorToApparentDiffusion *New();
  vtkTypeMacro(vtkmpjTensorToApparentDiffusion,vtkpxSimpleImageToImageFilter);
  
  // Sets/Gets the baseline image A(b=0); the echo amplitude without any
  // diffusion attenuation
  vtkSetObjectMacro(Baseline, vtkImageData);
  vtkGetObjectMacro(Baseline, vtkImageData);
  
  // Description: 
  // Sets/Gets mask for determining the extents of tensor
  // computation. If a mask is not provided, ADCs 
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);

  // Description:
  // Sets/Gets the array of gradient directions
  vtkSetObjectMacro(Gradients, vtkDoubleArray);
  vtkGetObjectMacro(Gradients, vtkDoubleArray);

  // Description:
  // Set/Get Shift Factor so that S(b)/S(0) can be computed when S(0)=0
  // ADC = (S(b)+shift)/(S(0)+shift), shift ~= 0
  vtkSetMacro(Shift,float);
  vtkGetMacro(Shift,float);
  
  // Description:
  // Set/Get the b-Value
  vtkSetMacro(bValue,float);
  vtkGetMacro(bValue,float);
  
  // Description:
  // Set/Get constant baseline value
  vtkSetMacro(BaselineValue,float);
  vtkGetMacro(BaselineValue,float);

  // Description:
  // Set/Get the b-Value
  vtkSetMacro(NumberOfDirections,int);
  vtkGetMacro(NumberOfDirections,int);

  // Description: turn on/off symmetric tensor flag
  vtkGetMacro(SymmetricTensor, int);
  vtkSetClampMacro(SymmetricTensor, int, 0, 1);
  vtkBooleanMacro(SymmetricTensor, int);

  // Description: turn on/off addition of noise
  vtkGetMacro(AddNoise, int);
  vtkSetClampMacro(AddNoise, int, 0, 1);
  vtkBooleanMacro(AddNoise, int);

  // Description:
  // Set/Get noise level
  vtkSetMacro(NoiseLevel,float);
  vtkGetMacro(NoiseLevel,float);

 protected:
  float Shift;
  float bValue;
  float BaselineValue;
  int NumberOfDirections;
  int SymmetricTensor;
  int AddNoise;
  float NoiseLevel;
  vtkImageData *Baseline;
  vtkImageData *Mask;
  vtkDoubleArray *Gradients;

  vtkmpjTensorToApparentDiffusion();
  virtual ~vtkmpjTensorToApparentDiffusion();

  float GaussianDeviate();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  
  vtkmpjTensorToApparentDiffusion(const vtkmpjTensorToApparentDiffusion&); // Not implemented
  void operator=(const vtkmpjTensorToApparentDiffusion&); // Not implemented
};

#endif
