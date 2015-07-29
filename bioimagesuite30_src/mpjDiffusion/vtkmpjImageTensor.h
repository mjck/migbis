

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


/* -- vtkmpjImageTensor.h 
   
      Marcel Jackowski, 2002

      This class computes the diffusion tensor matrix from gradient
      images taken in 6 different directions.

      It is an implementation of Basser and Pierpaoli's paper 
      "A simplified Method to Measure the Diffusion Tensor from
      Seven MR Images", MRM 39:928-934 (1998).

      It expects at least one vtkImageData structure containing
      DWIs acquired in 6 different directions. 
      
      The symmetric tensor component information is stored in 6 
      different frames. The frames contain the elements
      Dxx, Dxy, Dxz, Dyy, Dyz and  Dzz, respectively.
      
*/

#ifndef __vtkmpjImageTensor_h
#define __vtkmpjImageTensor_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImageTensor : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageTensor *New();
  vtkTypeMacro(vtkmpjImageTensor,vtkpxSimpleImageToImageFilter);
  
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
  // Set/Get Shift Factor
  vtkSetMacro(Shift,float);
  vtkGetMacro(Shift,float);

  // Description:
  // Set/Get bValue
  vtkSetMacro(bValue,float);
  vtkGetMacro(bValue,float);
  
 protected:
  float Shift;
  float bValue;
  vtkImageData *Baseline;
  vtkImageData *Mask;
  
  vtkmpjImageTensor();
  virtual ~vtkmpjImageTensor();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();
  
 private:
  vtkmpjImageTensor(const vtkmpjImageTensor&); // Not implemented
  void operator=(const vtkmpjImageTensor&); // Not implemented
};

#endif
