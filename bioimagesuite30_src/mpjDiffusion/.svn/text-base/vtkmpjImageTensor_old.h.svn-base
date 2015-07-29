/* -- vtkmpjImageTensor.h 
   
      Marcel Jackowski, 2002

      This class computes the diffusion tensor matrix from gradient
      images taken in 6 different directions.

      It is an implementation of Basser and Pierpaoli's paper 
      "A simplified Method to Measure the Diffusion Tensor from
      Seven MR Images", MRM 39:928-934 (1998).

      It expects a vtkStructuredPoints with a total of 6 DWIs
      scalar components (frames) as the input. 

      Returns a vtkStructuredPoints with the symmetric tensor component information
      stored in 6 different frames. The frames contain the elements
      Dxx, Dxy, Dxz, Dyy, Dyz and  Dzz, respectively.
      
*/

#ifndef __vtkmpjImageTensor_h
#define __vtkmpjImageTensor_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class VTK_IMAGING_EXPORT vtkmpjImageTensor : public vtkpxSimpleImageToImageFilter
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
  
 protected:
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
