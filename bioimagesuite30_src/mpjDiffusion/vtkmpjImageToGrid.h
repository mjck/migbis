

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


#ifndef __vtkmpjImageToGrid_h
#define __vtkmpjImageToGrid_h

#include "vtkImageData.h"
#include "vtkStructuredGrid.h"
#include "vtkProcessObject.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImageToGrid : public vtkProcessObject
{
 public:

  static vtkmpjImageToGrid *New();
  vtkTypeMacro(vtkmpjImageToGrid,vtkProcessObject);

  virtual void SetInput(vtkImageData *input);
  vtkImageData *GetInput();

  vtkSetObjectMacro(Output, vtkStructuredGrid);
  vtkGetObjectMacro(Output, vtkStructuredGrid);
  
  virtual void Update();

 protected:
  vtkmpjImageToGrid();
  virtual ~vtkmpjImageToGrid();

  vtkStructuredGrid *Output;

  void Execute();
  
 private:
  vtkmpjImageToGrid(const vtkmpjImageToGrid&); // Not implemented
  void operator=(const vtkmpjImageToGrid&); // Not implemented
};

#endif
