

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



#ifndef __vtkmpjImageTwoNorm_h
#define __vtkmpjImageTwoNorm_h

#include "vtkpxStructuredPointsToStructuredPointsFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImageTwoNorm : public vtkpxStructuredPointsToStructuredPointsFilter
{
public:
  static vtkmpjImageTwoNorm *New();
  vtkTypeMacro(vtkmpjImageTwoNorm,vtkpxStructuredPointsToStructuredPointsFilter);

  // Multiple Input Stuff
  // --------------------
  // Description:
  // Set an Input of this filter. 
  virtual void SetInput1(vtkStructuredPoints *input);
  virtual void SetInput2(vtkStructuredPoints *input);
  
  // Description:
  // Get one input to this filter.
  vtkStructuredPoints *GetInput1();
  vtkStructuredPoints *GetInput2();

protected:

  vtkmpjImageTwoNorm();
  virtual ~vtkmpjImageTwoNorm();
  vtkmpjImageTwoNorm(const vtkmpjImageTwoNorm&) {};
  void operator=(const vtkmpjImageTwoNorm&) {};

  void Execute();
  void ExecuteInformation();

};

#endif
