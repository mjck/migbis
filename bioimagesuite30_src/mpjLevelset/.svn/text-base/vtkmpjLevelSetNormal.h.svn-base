

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


#ifndef __vtkmpjLevelSetNormal_h
#define __vtkmpjLevelSetNormal_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjLevelSetNormal : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjLevelSetNormal *New();
  vtkTypeMacro(vtkmpjLevelSetNormal,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining propagation extents
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
 protected:
  vtkImageData *Mask;

  vtkmpjLevelSetNormal();
  virtual ~vtkmpjLevelSetNormal();
  
  int InsideMask(int index);
  void ComputeNormal(vtkImageData *input, vtkImageData *output, 
		     int coord[3], int index, double result[3]);
  
  void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
    
 private: 
  //BTX
  vtkDataArray *mask;
  //ETX
  vtkmpjLevelSetNormal(const vtkmpjLevelSetNormal&);// Not implemented
  void operator=(const vtkmpjLevelSetNormal&); // Not Implemented
};

#endif
