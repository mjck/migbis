

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


#ifndef __vtkmpjStreamlineDistance_h
#define __vtkmpjStreamlineDistance_h

#include "vtkPolyDataToPolyDataFilter.h"
#include "vtkDataSet.h"
#include "vtkPolyData.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"

class vtkmpjStreamlineDistance : public vtkPolyDataToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjStreamlineDistance,vtkPolyDataToPolyDataFilter);

  // Description:
  // Construct object 
  static vtkmpjStreamlineDistance *New();
  
  void SetReference(vtkDataSet *input); 
  void SetTarget(vtkDataSet *input);
  vtkPolyData *GetInput(int index);

  // Get/Set reference polyline
  vtkGetMacro(ReferenceIndex, int);
  vtkSetMacro(ReferenceIndex, int);

  // Get/Set target polyline
  vtkGetMacro(TargetIndex, int);
  vtkSetMacro(TargetIndex, int);
  
  // Get/Set stats
  vtkGetMacro(Mean, float);
  vtkGetMacro(Minimum, float);
  vtkGetMacro(Maximum, float);

 protected:
  vtkmpjStreamlineDistance();
  ~vtkmpjStreamlineDistance() {}
  
  // Usual data generation method
  void Execute();
  
  int ReferenceIndex;
  int TargetIndex;
  float Mean;
  float Minimum;
  float Maximum;
  
 private:
  vtkmpjStreamlineDistance(const vtkmpjStreamlineDistance&);// Not implemented.
  void operator=(const vtkmpjStreamlineDistance&);  // Not implemented.
};

#endif
