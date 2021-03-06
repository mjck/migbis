

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


/* -- vtkmpjImageTensorProduct.h 
   
  Computes the tensor product as in Basser and Pierpaoli's 
  paper "Microstructural and Physiological Features of Tissues
  Elucidated by Quantitative Diffusion Tensor MRI, 1996.

  Essentially the mean tensor product within in a prescribed 
  neighborhood.

  Marcel Jackowski, February 2003

*/

#ifndef __vtkmpjImageTensorProduct_h
#define __vtkmpjImageTensorProduct_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImageTensorProduct : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageTensorProduct *New();
  vtkTypeMacro(vtkmpjImageTensorProduct,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining the extents for invariants
  // computation. If a mask is not provided, invariants are
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);

  // Sets/Gets eigenvalues
  vtkSetObjectMacro(Eigenvalues, vtkImageData);
  vtkGetObjectMacro(Eigenvalues, vtkImageData);

  // Description:
  // Set/Get the kernel size for computations;
  vtkSetVector3Macro(KernelSize,int);
  vtkGetVector3Macro(KernelSize,int);
  
  void SetEigenvectors(vtkImageData *input) { this->SetInput(input); }
  vtkImageData* GetEigenvectors() { return this->GetInput(); }

protected:
  int KernelSize[3];
  float MaskOutValue; 
  vtkImageData *Mask;
  vtkImageData *Eigenvalues;
  
  vtkmpjImageTensorProduct();
  virtual ~vtkmpjImageTensorProduct();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  vtkmpjImageTensorProduct(const vtkmpjImageTensorProduct&); // Not implemented
  void operator=(const vtkmpjImageTensorProduct&); // Not Implemented
};

#endif
