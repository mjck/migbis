

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


/* -- vtkmpjTensorBMatrixLeastSquares.h 
   
      Marcel Jackowski, 2007

      Given N total measurements, M for b > 0 and M-N for b == 0
      fits a tensor using least squares.

      x = B * d

      x = [ ln(S1) ln(S2) ... ln (SN) ]' 
      B = [ [ -bxx1 -byy1 -bzz1 -2bxy1 -2bxz1 -2byz1 1 ] ...
            [ -bxx2 -byy2 -bzz2 -2bxy2 -2bxz2 -2byz2 1 ] ...
                               ....
            [ -bxxN -byyN -bzzN -2bxyN -2bxzN -2byzN 1 ] ]

      d = [ Dxx Dyy Dzz Dxy Dxz Dyz ln(S0) ]' 
 
      It expects an vtkImageData structure containing
      N DWIs. There should also be a b-value associated with
      every DWI.
      
      The symmetric tensor component information is stored in 6 
      different frames. The frames contain the elements
      Dxx, Dxy, Dxz, Dyy, Dyz and  Dzz, respectively.
      
*/

#ifndef __vtkmpjTensorBMatrixLeastSquares_h
#define __vtkmpjTensorBMatrixLeastSquares_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkDoubleArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjTensorBMatrixLeastSquares : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjTensorBMatrixLeastSquares *New();
  vtkTypeMacro(vtkmpjTensorBMatrixLeastSquares,vtkpxSimpleImageToImageFilter);
  
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
  // Set/Get the number of directions
  vtkSetMacro(NumberOfDirections,int);
  vtkGetMacro(NumberOfDirections,int);

  // Description:
  // Set/Get Shift Factor so that S(b)/S(0) can be computed when S(0)=0
  // ADC = (S(b)+shift)/(S(0)+shift), shift ~= 0
  vtkSetMacro(Shift,float);
  vtkGetMacro(Shift,float);
  
  // Description:
  // Set/Get the b-Value list
  vtkSetObjectMacro(bValues,vtkDoubleArray);
  vtkGetObjectMacro(bValues,vtkDoubleArray);
  
  // Description
  // Set/Get the number of b-values
  vtkSetMacro(NumberOfbValues,int);
  vtkGetMacro(NumberOfbValues,int);

  // Description: turn on/off symmetric tensor flag
  vtkGetMacro(SymmetricTensor, int);
  vtkSetClampMacro(SymmetricTensor, int, 0, 1);
  vtkBooleanMacro(SymmetricTensor, int);

 protected:
  float Shift;
  int NumberOfDirections;
  int NumberOfbValues;
  int SymmetricTensor;
  vtkImageData *Mask;
  vtkDoubleArray *Gradients;
  vtkDoubleArray *bValues;

  vtkmpjTensorBMatrixLeastSquares();
  virtual ~vtkmpjTensorBMatrixLeastSquares();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  
  vtkmpjTensorBMatrixLeastSquares(const vtkmpjTensorBMatrixLeastSquares&); // Not implemented
  void operator=(const vtkmpjTensorBMatrixLeastSquares&); // Not implemented
};

#endif
