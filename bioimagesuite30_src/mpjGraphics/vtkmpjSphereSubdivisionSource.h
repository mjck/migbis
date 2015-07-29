

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
// Generates a triangle mesh approximating a sphere by 
// recursive subdivision. First approximation is a platonic solid; 
// each level of refinement increases the number of 
// triangles by a factor of 4.
//
// Level 3 (128 triangles for an octahedron) is a good tradeoff if 
// gouraud shading is used to render the database.
// 
// Jon Leech (leech @ cs.unc.edu) 3/24/89 * http://www.cs.unc.edu/~jon
// 

#ifndef __vtkmpjSphereSubdivisionSource_h
#define __vtkmpjSphereSubdivisionSource_h

#include "vtkPolyDataSource.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"

#define VTK_MPJ_MAX_DEPTH 10

class vtkmpjSphereSubdivisionSource : public vtkPolyDataSource 
{
public:
  vtkTypeMacro(vtkmpjSphereSubdivisionSource,vtkPolyDataSource);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct sphere with radius=0.5 and default resolution 8 in both Phi
  // and Theta directions. Theta ranges from (0,360) and phi (0,180) degrees.
  static vtkmpjSphereSubdivisionSource *New();

  // Description:
  // Set the depth of the sphere subdivision
  vtkSetClampMacro(Depth,int,1,VTK_MPJ_MAX_DEPTH);
  vtkGetMacro(Depth,int);

  // Description:
  // Set whether the list of points should contain duplicates
  vtkSetClampMacro(DuplicatePoints,int,0,1);
  vtkBooleanMacro(DuplicatePoints,int);
  vtkGetMacro(DuplicatePoints,int);

  // Description:
  // Set initial polyhedron for subdivision
  //BTX
  enum
    {
      OCTAHEDRON,    // 8F, 6V, 12E
      TETRAHEDRON,   // 4F, 4V
      ICOSAHEDRON    // 20F, 12V, 30E
    };
  //ETX
  vtkSetClampMacro(SubdivisionType, int, OCTAHEDRON, ICOSAHEDRON);
  vtkGetMacro(SubdivisionType, int);
  void SetSubdivisionTypeToOctahedron()
    {this->SetSubdivisionType(OCTAHEDRON);};
  void SetSubdivisionTypeToTetrahedron()
    {this->SetSubdivisionType(TETRAHEDRON);};
  void SetSubdivisionTypeToIcosahedron()
    {this->SetSubdivisionType(ICOSAHEDRON);};

protected:
  vtkmpjSphereSubdivisionSource();
  ~vtkmpjSphereSubdivisionSource() {}

  void Execute();
  
  int Depth;
  int DuplicatePoints;
  int SubdivisionType;
  
  void InitializePolys(vtkPoints *pts, vtkCellArray *polys);
  void SubdividePolys(vtkPoints *newPoints, vtkCellArray *oldCells, vtkCellArray *newCells);
  void FindPoints(vtkPoints *pts, double a[3], double b[3], double c[3], vtkIdType &pa, vtkIdType &pb, vtkIdType &pc);
  
private:
  vtkmpjSphereSubdivisionSource(const vtkmpjSphereSubdivisionSource&);  // Not implemented.
  void operator=(const vtkmpjSphereSubdivisionSource&);  // Not implemented.
};

#endif


