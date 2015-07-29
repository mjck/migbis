

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


/* -- vtkmpjImageDotProduct.h 
   
      Marcel Jackowski, 2005

      This class computes the Frobenius norm of a matrix T.

*/

#ifndef __vtkmpjImageFrobeniusNorm_h
#define __vtkmpjImageFrobeniusNorm_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkMath.h"

// Ordering
#define VTK_MPJ_TENSOR_XXXYXZYYYZZZ 0
#define VTK_MPJ_TENSOR_XXYYZZXYYZXZ 1

class vtkmpjImageFrobeniusNorm : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageFrobeniusNorm *New();
  vtkTypeMacro(vtkmpjImageFrobeniusNorm,vtkpxSimpleImageToImageFilter);
  
  // Description:
  // Set/Get the component ordering information
  vtkSetMacro(Ordering,int);
  vtkGetMacro(Ordering,int);

  // Description:
  // Sets/Gets mask where eigenvalues are to be computed
  vtkGetObjectMacro(Mask, vtkImageData);
  vtkSetObjectMacro(Mask, vtkImageData);
  
 protected:
  vtkImageData *Mask;
  int Ordering;

  vtkmpjImageFrobeniusNorm();
  virtual ~vtkmpjImageFrobeniusNorm();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();
  
 private:
  vtkmpjImageFrobeniusNorm(const vtkmpjImageFrobeniusNorm&); // Not implemented
  void operator=(const vtkmpjImageFrobeniusNorm&); // Not Implemented

  void ExecuteWithReducedTensor(vtkImageData *input, vtkImageData *output);
  void ExecuteWithFullTensor(vtkImageData *input, vtkImageData *output);
  
};

#endif
