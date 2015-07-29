

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



#ifndef __vtkmpjImageT2Relax_h
#define __vtkmpjImageT2Relax_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"

class vtkmpjImageT2Relax : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkmpjImageT2Relax *New();
  vtkTypeMacro(vtkmpjImageT2Relax,vtkpxSimpleImageToImageFilter);

  // Description:
  // Set subset of frames to extract
  vtkSetMacro(EchoTime1, float);
  vtkGetMacro(EchoTime1, float);
  vtkSetMacro(EchoTime2, float);
  vtkGetMacro(EchoTime2, float);

// Threshold value for retaining high-intensity pixels.
  vtkSetMacro(ThresholdValue, int);
  vtkGetMacro(ThresholdValue, int);

  // Multiple Input Stuff
  // --------------------
  // Description:
  // Set an Input of this filter. 
  virtual void SetInput1(vtkImageData *input);
  virtual void SetInput2(vtkImageData *input);
  
  // Description:
  // Get one input to this filter.
  vtkImageData *GetInput1();
  vtkImageData *GetInput2();

protected:

  vtkmpjImageT2Relax();
  virtual ~vtkmpjImageT2Relax();
  vtkmpjImageT2Relax(const vtkmpjImageT2Relax&) {};
  void operator=(const vtkmpjImageT2Relax&) {};

  float EchoTime1;
  float EchoTime2;

  int ThresholdValue;

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

};

#endif
