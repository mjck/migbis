

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


/* -- vtkmpjImageVectorGradientMagnitude.h 
   
  This class computes vector gradient magnitude of a 
  multi-channel 3D image.

  Marcel Jackowski, June 2005

*/

#ifndef __vtkmpjImageVectorGradientMagnitude_h
#define __vtkmpjImageVectorGradientMagnitude_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImageVectorGradientMagnitude : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageVectorGradientMagnitude *New();
  vtkTypeMacro(vtkmpjImageVectorGradientMagnitude,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining the extents 
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
  // Description: 
  // Sets/Gets weight image
  vtkSetObjectMacro(Weight, vtkImageData);
  vtkGetObjectMacro(Weight, vtkImageData);

  // Description: 
  // Sets/Gets gradient calculation method
  vtkSetMacro(UsePrincipalComponents, int);
  vtkGetMacro(UsePrincipalComponents, int);
  vtkBooleanMacro(UsePrincipalComponents, int);

protected:
  int UsePrincipalComponents;
  float MaskOutValue; 
  vtkImageData *Mask;
  vtkImageData *Weight;
  
  vtkmpjImageVectorGradientMagnitude();
  virtual ~vtkmpjImageVectorGradientMagnitude();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();
  
 private:
  vtkmpjImageVectorGradientMagnitude(const vtkmpjImageVectorGradientMagnitude&); // Not implemented
  void operator=(const vtkmpjImageVectorGradientMagnitude&); // Not Implemented


  void ExecuteWithPrincipalComponents(vtkImageData *input, vtkImageData *output);
  void ExecuteWithSumOfSquaredMagnitudes(vtkImageData *input, vtkImageData *output);

  double CalculateDotProduct(float *a, float *b, int nc);
};

#endif
