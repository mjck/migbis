

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
   
      Marcel Jackowski, 2002

      This class computes the dot product of two vector images.

*/

#ifndef __vtkmpjImageDotProduct_h
#define __vtkmpjImageDotProduct_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkMath.h"

class vtkmpjImageDotProduct : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageDotProduct *New();
  vtkTypeMacro(vtkmpjImageDotProduct,vtkpxSimpleImageToImageFilter);

  void SetInput1(vtkImageData *input);
  void SetInput2(vtkImageData *input);
  vtkImageData *GetInput(int index);
  
  // Description:
  // Sets/Gets mask where eigenvalues are to be computed
  vtkGetObjectMacro(Mask, vtkImageData);
  vtkSetObjectMacro(Mask, vtkImageData);

 protected:
  vtkImageData *Mask;

  vtkmpjImageDotProduct();
  virtual ~vtkmpjImageDotProduct();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();
  
 private:
  vtkmpjImageDotProduct(const vtkmpjImageDotProduct&); // Not implemented
  void operator=(const vtkmpjImageDotProduct&); // Not Implemented
};

#endif
