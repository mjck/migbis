

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


#ifndef __vtkmpjTensorStreamlineAngleFilter_h
#define __vtkmpjTensorStreamlineAngleFilter_h

#include "vtkPolyDataToPolyDataFilter.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"

class vtkmpjTensorStreamlineAngleFilter : public vtkPolyDataToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjTensorStreamlineAngleFilter,vtkPolyDataToPolyDataFilter);

  // Description:
  // Construct object 
  static vtkmpjTensorStreamlineAngleFilter *New();

  // Set/Get Minimum Angle
  vtkSetMacro(MinimumAngle,double);
  vtkGetMacro(MinimumAngle,double);

  // Set/Get Maximum angle
  vtkSetMacro(MaximumAngle,double);
  vtkGetMacro(MaximumAngle,double);

 // Set/Get Minimum Torsion Angle
  vtkSetMacro(MinimumTorsion,double);
  vtkGetMacro(MinimumTorsion,double);

  // Set/Get Maximum Torsion angle
  vtkSetMacro(MaximumTorsion,double);
  vtkGetMacro(MaximumTorsion,double);

  // Set/Get Outside
  vtkSetClampMacro(AngleOutside,int,0,1);
  vtkGetMacro(AngleOutside,int);
  vtkBooleanMacro(AngleOutside,int);

  // Set/Get Outside
  vtkSetClampMacro(TorsionOutside,int,0,1);
  vtkGetMacro(TorsionOutside,int);
  vtkBooleanMacro(TorsionOutside,int);
  
 protected:
  vtkmpjTensorStreamlineAngleFilter();
  ~vtkmpjTensorStreamlineAngleFilter() {}
  
  // Usual data generation method
  void Execute();

  double MinimumAngle;
  double MaximumAngle;
  double MinimumTorsion;
  double MaximumTorsion;
  int AngleOutside;
  int TorsionOutside;

private:
  int TestAngle(double angleval,double minangle,double maxangle, int outside);

  vtkmpjTensorStreamlineAngleFilter(const vtkmpjTensorStreamlineAngleFilter&);// Not implemented.
  void operator=(const vtkmpjTensorStreamlineAngleFilter&);  // Not implemented.
};

#endif
