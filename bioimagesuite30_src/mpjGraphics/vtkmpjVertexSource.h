

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


#ifndef __vtkmpjVertexSource_h
#define __vtkmpjVertexSource_h

#include "vtkPolyDataSource.h"
#include "vtkCell.h"
#include "vtkCellArray.h"
#include "vtkDataSet.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkTransform.h"
#include "vtkPoints.h"

class vtkmpjVertexSource : public vtkPolyDataSource
{
public:
  vtkTypeMacro(vtkmpjVertexSource,vtkPolyDataSource);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkmpjVertexSource *New();

  // Description:
  // Specify scale factor to scale object by. (Scale factor always affects
  // output even if scaling is off.)
  vtkSetMacro(ScaleFactor,float);
  vtkGetMacro(ScaleFactor,float);

  // Description:
  // Specify number of points
  vtkSetMacro(NumberOfPoints,int);
  vtkGetMacro(NumberOfPoints,int);

  // Description:
  // Sets/Gets the array of points
  vtkSetObjectMacro(Points, vtkPoints);
  vtkGetObjectMacro(Points, vtkPoints);

protected:
  int NumberOfPoints;
  vtkPoints *Points;
  
  float ScaleFactor; // Scale factor to use to scale geometry
   
  vtkmpjVertexSource();
  ~vtkmpjVertexSource();

  void Execute();

private:
  vtkmpjVertexSource(const vtkmpjVertexSource&);  // Not implemented.
  void operator=(const vtkmpjVertexSource&);  // Not implemented.
};

#endif
