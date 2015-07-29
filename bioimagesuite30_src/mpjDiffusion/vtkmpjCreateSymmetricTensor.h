

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


/* -- vtkmpjCreateSymmetricTensor.h 
   
      Marcel Jackowski, 2003.

      This class creates a symmetric tensor given the eigenvalues
      and eigenvector images organized in decreasing order.
      
      It will output 6 frames corresponding to 6 image components.
*/

#ifndef __vtkmpjCreateSymmetricTensor_h
#define __vtkmpjCreateSymmetricTensor_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjCreateSymmetricTensor : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjCreateSymmetricTensor *New();
  vtkTypeMacro(vtkmpjCreateSymmetricTensor,vtkpxSimpleImageToImageFilter);

 protected:
  vtkmpjCreateSymmetricTensor();
  virtual ~vtkmpjCreateSymmetricTensor();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();
  
 private:
  vtkmpjCreateSymmetricTensor(const vtkmpjCreateSymmetricTensor&); // Not implemented
  void operator=(const vtkmpjCreateSymmetricTensor&); // Not Implemented
};

#endif
