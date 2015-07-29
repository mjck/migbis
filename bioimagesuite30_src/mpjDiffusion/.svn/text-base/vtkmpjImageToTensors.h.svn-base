

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


#ifndef __vtkmpjImageToTensors_h
#define __vtkmpjImageToTensors_h

#include "vtkDataSetToDataSetFilter.h"
#include "vtkDataSet.h"
#include "vtkPointData.h"

class vtkmpjImageToTensors : public vtkDataSetToDataSetFilter
{
 public:
  static vtkmpjImageToTensors *New();
  vtkTypeMacro(vtkmpjImageToTensors,vtkDataSetToDataSetFilter);

  // Description: turn on/off symmetric tensor flag
  vtkGetMacro(SymmetricTensor, int);  
  vtkSetClampMacro(SymmetricTensor, int, 0, 1);
  vtkBooleanMacro(SymmetricTensor, int);

  // Description: whether to flip the sign of the x-coordinate 
  vtkSetMacro(FlipX,int);
  vtkGetMacro(FlipX,int);
  vtkBooleanMacro(FlipX,int);
  
  // Description: whether to flip the sign of the y-coordinate
  vtkSetMacro(FlipY,int);
  vtkGetMacro(FlipY,int);
  vtkBooleanMacro(FlipY,int);
  
  // Description: whether to flip the sign of the z-coordinate
  vtkSetMacro(FlipZ,int);
  vtkGetMacro(FlipZ,int);
  vtkBooleanMacro(FlipZ,int);

 protected:  
  vtkmpjImageToTensors();
  virtual ~vtkmpjImageToTensors();
  int SymmetricTensor;  

  int FlipX;
  int FlipY;
  int FlipZ;

  void Execute();
  
 private:
  vtkmpjImageToTensors(const vtkmpjImageToTensors&); // Not implemented
  void operator=(const vtkmpjImageToTensors&); // Not implemented
};

#endif
