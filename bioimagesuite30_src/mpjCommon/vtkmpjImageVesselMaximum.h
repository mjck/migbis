

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



#ifndef __vtkmpjImageVesselMaximum_h
#define __vtkmpjImageVesselMaximum_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImageVesselMaximum : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageVesselMaximum *New();
  vtkTypeMacro(vtkmpjImageVesselMaximum,vtkpxSimpleImageToImageFilter);
  
  // Multiple Input Stuff
  // --------------------
  // Description:
  // Set an Input of this filter. 
  virtual void SetInput(int num, vtkImageData *input);
  
  // Description:
  // Adds an input to the first null position in the input list.
  // Expands the list memory if necessary
  virtual void AddInput(vtkImageData *input);
  
  // Description:
  // Get one input to this filter
  vtkImageData *GetInput(int num);
  vtkImageData *GetInput();
  
protected:
  vtkmpjImageVesselMaximum();
  virtual ~vtkmpjImageVesselMaximum();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  vtkmpjImageVesselMaximum(const vtkmpjImageVesselMaximum&); // Not implemented
  void operator=(const vtkmpjImageVesselMaximum&); // Not implemented
};

#endif


