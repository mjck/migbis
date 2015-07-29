

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


#ifndef __vtkmpjEstimateDiffusionS0_h
#define __vtkmpjEstimateDiffusionS0_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjEstimateDiffusionS0 : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjEstimateDiffusionS0 *New();
  vtkTypeMacro(vtkmpjEstimateDiffusionS0,vtkpxSimpleImageToImageFilter);
  
  // Set/Get the value for results falling outside the mask
  vtkSetMacro(LowBValue,float);
  vtkGetMacro(LowBValue,float);

  // Set/Get the value for results falling outside the mask
  vtkSetMacro(HighBValue,float);
  vtkGetMacro(HighBValue,float);

   // Set/Get the value for results falling outside the mask
  vtkSetMacro(LowBValueIndex,int);
  vtkGetMacro(LowBValueIndex,int);

  // Set/Get the value for results falling outside the mask
  vtkSetMacro(HighBValueIndex,int);
  vtkGetMacro(HighBValueIndex,int);

protected:
  float LowBValue;
  float HighBValue;
  int LowBValueIndex;
  int HighBValueIndex;

  vtkmpjEstimateDiffusionS0();
  virtual ~vtkmpjEstimateDiffusionS0();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  vtkmpjEstimateDiffusionS0(const vtkmpjEstimateDiffusionS0&); // Not implemented
  void operator=(const vtkmpjEstimateDiffusionS0&); // Not Implemented
};

#endif
