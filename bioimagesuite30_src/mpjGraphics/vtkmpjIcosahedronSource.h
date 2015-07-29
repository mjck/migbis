

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


// 
// Creates an icosahedron of an arbitrary depth (frequency).
// With depth=1, it will have 20 faces and 12 vertices.
// 

#ifndef __vtkmpjIcosahedronSource_h
#define __vtkmpjIcosahedronSource_h

#include "vtkPolyDataSource.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkFloatArray.h"

#define VTK_MPJ_MAX_DEPTH 100

class vtkmpjIcosahedronSource : public vtkPolyDataSource 
{
public:
  vtkTypeMacro(vtkmpjIcosahedronSource,vtkPolyDataSource);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct an icosahedron of depth=1
  static vtkmpjIcosahedronSource *New();

  // Description:
  // Set the depth of the sphere subdivision
  vtkSetClampMacro(Depth,int,1,VTK_MPJ_MAX_DEPTH);
  vtkGetMacro(Depth,int);

  // Description:
  // Set whether the list of points should contain duplicates
  vtkSetClampMacro(AllowDuplicatePoints,int,0,1);
  vtkBooleanMacro(AllowDuplicatePoints,int);
  vtkGetMacro(AllowDuplicatePoints,int);

protected:
  vtkmpjIcosahedronSource();
  ~vtkmpjIcosahedronSource() {}

  void Execute();
  
  int Depth;
  int AllowDuplicatePoints;
  
  void InitializePolys(vtkPoints *pts, vtkCellArray *polys, vtkUnsignedCharArray *scalars);
  void SubdividePolys(vtkPoints *newPoints, vtkCellArray *oldCells, vtkCellArray *newCells, vtkUnsignedCharArray *newScalars);
  void FindOrInsert1Point(vtkPoints *newPoints, vtkUnsignedCharArray *newScalars, float a[3], vtkIdType &ra);
  void FindOrInsert3Points(vtkPoints *newPoints, vtkUnsignedCharArray *newScalars, float a[3], float b[3], float c[3], vtkIdType &ra, vtkIdType &rb, vtkIdType &rc);
  
 private:    
  
  vtkIdType **conn;
  
  vtkmpjIcosahedronSource(const vtkmpjIcosahedronSource&);  // Not implemented.
  void operator=(const vtkmpjIcosahedronSource&);  // Not implemented.
};

#endif


