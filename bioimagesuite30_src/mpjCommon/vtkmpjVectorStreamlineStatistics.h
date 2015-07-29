

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


#ifndef __vtkmpjVectorStreamlineStatistics_h
#define __vtkmpjVectorStreamlineStatistics_h

#include "vtkPolyDataToPolyDataFilter.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"

class vtkmpjVectorStreamlineStatistics : public vtkPolyDataToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjVectorStreamlineStatistics,vtkPolyDataToPolyDataFilter);

  // Description:
  // Construct object 
  static vtkmpjVectorStreamlineStatistics *New();

  // Get statistics across fibers
  vtkGetMacro(NumberOfFibers, int);

  vtkGetMacro(Minimum, float);
  vtkGetMacro(Maximum, float);
  vtkGetMacro(Mean, float);
  vtkGetMacro(Variance, float);
  
 protected:
  vtkmpjVectorStreamlineStatistics();
  ~vtkmpjVectorStreamlineStatistics() {}
  
  // Usual data generation method
  void Execute();
  
  int NumberOfFibers;
  float Minimum;
  float Maximum;
  float Variance;
  float Mean;
 
private:
  vtkmpjVectorStreamlineStatistics(const vtkmpjVectorStreamlineStatistics&);// Not implemented.
  void operator=(const vtkmpjVectorStreamlineStatistics&);  // Not implemented.
};

#endif
