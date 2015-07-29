

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



#ifndef __vtkmpjFramePermutation_h
#define __vtkmpjFramePermutation_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkIntArray.h"

class vtkmpjFramePermutation : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkmpjFramePermutation *New();
  vtkTypeMacro(vtkmpjFramePermutation,vtkpxSimpleImageToImageFilter);

  // Description:
  // target frame permutation
  vtkGetObjectMacro(Permutation, vtkIntArray);
  vtkSetObjectMacro(Permutation, vtkIntArray);

protected:
  vtkIntArray *Permutation;

  vtkmpjFramePermutation();
  virtual ~vtkmpjFramePermutation();
  
  virtual void SimpleExecute(vtkImageData *in, vtkImageData *out);

 private:
  vtkmpjFramePermutation(const vtkmpjFramePermutation&) {};
  void operator=(const vtkmpjFramePermutation&) {};
  
};

#endif
