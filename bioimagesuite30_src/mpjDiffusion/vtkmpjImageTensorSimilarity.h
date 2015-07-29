

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


/* -- vtkmpjImageTensorSimilarity.h 
   
  Tensor Similarity Indices

  Marcel Jackowski, June 2005

*/

#ifndef __vtkmpjImageTensorSimilarity_h
#define __vtkmpjImageTensorSimilarity_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

// Ordering
#define VTK_MPJ_TENSOR_XXXYXZYYYZZZ 0
#define VTK_MPJ_TENSOR_XXYYZZXYYZXZ 1

class vtkmpjImageTensorSimilarity : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageTensorSimilarity *New();
  vtkTypeMacro(vtkmpjImageTensorSimilarity,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining the extents for invariants
  // computation. If a mask is not provided, invariants are
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);

  // Description:
  // Set/Get the component ordering information
  vtkSetMacro(Ordering,int);
  vtkGetMacro(Ordering,int);

  // Description:
  // Set/Get the kernel size for computations;
  vtkSetVector3Macro(KernelSize,int);
  vtkGetVector3Macro(KernelSize,int);
  
protected:
  int Ordering;
  int KernelSize[3];
  float MaskOutValue; 
  vtkImageData *Mask;
  
  vtkmpjImageTensorSimilarity();
  virtual ~vtkmpjImageTensorSimilarity();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  vtkmpjImageTensorSimilarity(const vtkmpjImageTensorSimilarity&); // Not implemented
  void operator=(const vtkmpjImageTensorSimilarity&); // Not Implemented

  void ExecuteWithReducedTensor(vtkImageData *input, vtkImageData *output);
  void ExecuteWithFullTensor(vtkImageData *input, vtkImageData *output);
  void GetReducedTensor(vtkDataArray *in, int index, float T[3][3]);
  void GetFullTensor(vtkDataArray *in, int index, float T[3][3]);
  void TensorDifference(float T[3][3], float U[3][3], float R[3][3]);
};

#endif
