

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


#ifndef __vtkmpjTensorStreamlineStatistics_h
#define __vtkmpjTensorStreamlineStatistics_h

#include "vtkPolyDataToPolyDataFilter.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"

class vtkmpjTensorStreamlineStatistics : public vtkPolyDataToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjTensorStreamlineStatistics,vtkPolyDataToPolyDataFilter);

  // Description:
  // Construct object 
  static vtkmpjTensorStreamlineStatistics *New();

  // Get statistics across fibers
  // min, max, mean, variance
  vtkGetVectorMacro(Anisotropy,float,4);
  vtkGetVectorMacro(Diffusivity,float,4);
  vtkGetVectorMacro(Distance,float,4);
  vtkGetVectorMacro(Angle,float,4);
  vtkGetVectorMacro(Other,float,4);

  vtkGetMacro(NumberOfFibers, int);
  
 protected:
  vtkmpjTensorStreamlineStatistics();
  ~vtkmpjTensorStreamlineStatistics() {}
  
  // Usual data generation method
  void Execute();
  
  int NumberOfFibers;
  float Anisotropy[4];
  float Diffusivity[4];
  float Distance[4];
  float Angle[4];
  float Other[4];

private:
  vtkmpjTensorStreamlineStatistics(const vtkmpjTensorStreamlineStatistics&);// Not implemented.
  void operator=(const vtkmpjTensorStreamlineStatistics&);  // Not implemented.
};

#endif
