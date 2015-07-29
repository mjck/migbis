

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


#ifndef __vtkmpjTensorStreamlineROIFilter_h
#define __vtkmpjTensorStreamlineROIFilter_h

#include "vtkPolyDataToPolyDataFilter.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"

class vtkmpjTensorStreamlineROIFilter : public vtkPolyDataToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjTensorStreamlineROIFilter,vtkPolyDataToPolyDataFilter);

  // Description:
  // Construct object 
  static vtkmpjTensorStreamlineROIFilter *New();

  // Description:
  // Specify the origin of the first spherical ROI
  vtkSetVector3Macro(Center1,double);
  vtkGetVectorMacro(Center1,double,3);

  // Description: Set/Get sphere radius
  vtkSetMacro(Radius1,double);
  vtkGetMacro(Radius1,double);

  // Description:
  // Specify the origin of the second spherical ROI
  vtkSetVector3Macro(Center2,double);
  vtkGetVectorMacro(Center2,double,3);

  // Description: Set/Get sphere radius
  vtkSetMacro(Radius2,double);
  vtkGetMacro(Radius2,double);

  // Set/Get Clipping
  vtkSetClampMacro(Clipping,int,0,1);
  vtkGetMacro(Clipping,int);
  vtkBooleanMacro(Clipping,int);

    // Set/Get Compute Stats
  vtkSetClampMacro(ComputeStatistics,int,0,1);
  vtkGetMacro(ComputeStatistics,int);
  vtkBooleanMacro(ComputeStatistics,int);
  
 protected:
  vtkmpjTensorStreamlineROIFilter();
  ~vtkmpjTensorStreamlineROIFilter() {}
  
  // Usual data generation method
  void Execute();

  double Radius1;
  double Radius2;
  double Center1[3];
  double Center2[3];
  int Clipping;
  int ComputeStatistics;

private:
  vtkmpjTensorStreamlineROIFilter(const vtkmpjTensorStreamlineROIFilter&);// Not implemented.
  void operator=(const vtkmpjTensorStreamlineROIFilter&);  // Not implemented.
};

#endif
