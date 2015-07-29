
/* Computes the Hessian image */

#ifndef __vtkmpjImageHessian_h
#define __vtkmpjImageHessian_h

#include "vtkImageData.h"
#include "vtkpxSimpleImageToImageFilter.h"

class vtkmpjImageHessian : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkmpjImageHessian *New();
  vtkTypeMacro(vtkmpjImageHessian,vtkpxSimpleImageToImageFilter);

  // Description:
  // Set Scale Sigma
  vtkGetMacro(Sigma,float);
  vtkSetMacro(Sigma,float);

  // Description:
  // Sets/Gets mask where hessian is to be computed
  vtkGetObjectMacro(Mask, vtkImageData);
  vtkSetObjectMacro(Mask, vtkImageData);

  // Description:
  // Set/Get Enable Gaussian Smoothing
  vtkGetMacro(EnableSmoothing, int);
  vtkSetMacro(EnableSmoothing, int);
  vtkBooleanMacro(EnableSmoothing, int);

  // Description:
  // Set/Get Enable Thresholding
  vtkGetMacro(EnableThresholding, int);
  vtkSetMacro(EnableThresholding, int);
  vtkBooleanMacro(EnableThresholding, int);

  // Description:
  // Set Scale Sigma
  vtkGetMacro(ThresholdValue,float);
  vtkSetMacro(ThresholdValue,float);

  // Description:
  // Set/Get Append Sigma
  vtkGetMacro(AppendSigmaImage, int);
  vtkSetMacro(AppendSigmaImage, int);
  vtkBooleanMacro(AppendSigmaImage, int);

protected:

  //BTX
  vtkmpjImageHessian();
  virtual ~vtkmpjImageHessian();
  vtkmpjImageHessian(const vtkmpjImageHessian&) {};
  void operator=(const vtkmpjImageHessian&) {};
  //ETX
  virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);
  virtual void ExecuteInformation();

  vtkImageData *Mask;

  float Sigma;
  float ThresholdValue;
  int EnableSmoothing;
  int EnableThresholding;
  int AppendSigmaImage;

};

#endif


