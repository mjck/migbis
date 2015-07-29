

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


/*
 * Computes the structure tensor \grad I * \grad I'
 */

#ifndef __vtkmpjImageStructureTensor_h
#define __vtkmpjImageStructureTensor_h

#include "vtkpxSimpleImageToImageFilter.h"

class vtkmpjImageStructureTensor : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkmpjImageStructureTensor *New();
  vtkTypeMacro(vtkmpjImageStructureTensor,vtkpxSimpleImageToImageFilter);

  // Description:
  // Set Scale Sigma
  vtkGetMacro(Sigma,float);
  vtkSetMacro(Sigma,float);
  
  // Description:
  // Enable/Disable smoothing
  vtkGetMacro(EnableSmoothing, int);
  vtkSetMacro(EnableSmoothing, int);
  vtkBooleanMacro(EnableSmoothing, int);
  
protected:

  //BTX
  vtkmpjImageStructureTensor();
  virtual ~vtkmpjImageStructureTensor();
  vtkmpjImageStructureTensor(const vtkmpjImageStructureTensor&) {};
  void operator=(const vtkmpjImageStructureTensor&) {};
  //ETX
  virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);
  virtual void ExecuteInformation();

  float Sigma;
  int EnableSmoothing;
};

#endif


