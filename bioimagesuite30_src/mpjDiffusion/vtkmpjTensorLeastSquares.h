

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


/* -- vtkmpjTensorLeastSquares.h 
   
      Marcel Jackowski, 2002

      Fits a tensor from N given directions (DWIs), using the least
      squares formulation. 
      
      A * X = ADC

      where ADC_i = Si(0)/S(b) and
      A_i = [gx_i^2 gy_i^2 gz_i^2 2*gxgy_i 2*gxgz_i 2*gygz_i]

      size(A) = 6 x N
      size(X) = 1 x 6
      size(ADC) = N x 1

      It expects an vtkImageData structure containing
      DWIs acquired in the N different directions. 
      
      The symmetric tensor component information is stored in 6 
      different frames. The frames contain the elements
      Dxx, Dxy, Dxz, Dyy, Dyz and  Dzz, respectively.
      
*/

#ifndef __vtkmpjTensorLeastSquares_h
#define __vtkmpjTensorLeastSquares_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkDoubleArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjTensorLeastSquares : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjTensorLeastSquares *New();
  vtkTypeMacro(vtkmpjTensorLeastSquares,vtkpxSimpleImageToImageFilter);
  
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
  // Set/Get the b-Value
  vtkSetMacro(NumberOfDirections,int);
  vtkGetMacro(NumberOfDirections,int);

  // Description: turn on/off symmetric tensor flag
  vtkGetMacro(SymmetricTensor, int);
  vtkSetClampMacro(SymmetricTensor, int, 0, 1);
  vtkBooleanMacro(SymmetricTensor, int);

 protected:
  float Shift;
  float bValue;
  int NumberOfDirections;
  int SymmetricTensor;
  vtkImageData *Baseline;
  vtkImageData *Mask;
  vtkDoubleArray *Gradients;

  vtkmpjTensorLeastSquares();
  virtual ~vtkmpjTensorLeastSquares();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  
  vtkmpjTensorLeastSquares(const vtkmpjTensorLeastSquares&); // Not implemented
  void operator=(const vtkmpjTensorLeastSquares&); // Not implemented
};

#endif
