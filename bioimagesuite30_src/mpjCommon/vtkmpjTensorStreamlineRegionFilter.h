

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


#ifndef __vtkmpjTensorStreamlineRegionFilter_h
#define __vtkmpjTensorStreamlineRegionFilter_h

#include "vtkPolyDataToPolyDataFilter.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"

class vtkmpjTensorStreamlineRegionFilter : public vtkPolyDataToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjTensorStreamlineRegionFilter,vtkPolyDataToPolyDataFilter);

  // Description:
  // Construct object 
  static vtkmpjTensorStreamlineRegionFilter *New();

  // Set/Get ROI 1 constraint
  vtkSetClampMacro(RegionNumber1,int,0,100);
  vtkGetMacro(RegionNumber1,int);

  // Set/Get ROI 2 constraint
  vtkSetClampMacro(RegionNumber2,int,0,100);
  vtkGetMacro(RegionNumber2,int);

  // Set/Get Clipping
  vtkSetClampMacro(Clipping,int,0,1);
  vtkGetMacro(Clipping,int);
  vtkBooleanMacro(Clipping,int);
  
 protected:
  vtkmpjTensorStreamlineRegionFilter();
  ~vtkmpjTensorStreamlineRegionFilter() {}
  
  // Usual data generation method
  void Execute();

  int RegionNumber1;
  int RegionNumber2;
  int Clipping;

private:
  vtkmpjTensorStreamlineRegionFilter(const vtkmpjTensorStreamlineRegionFilter&);// Not implemented.
  void operator=(const vtkmpjTensorStreamlineRegionFilter&);  // Not implemented.
};

#endif
