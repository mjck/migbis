

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


#include "vtkmpjSphereSubdivisionSource.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"

#include <math.h>

#define PPP 0
#define MMP 1
#define MPM 2
#define PMM 3

/* Structure describing a tetrahedron */
vtkIdType tetra[4][3] = {
  { PPP, MMP, MPM },
  { PPP, PMM, MMP },
  { MPM, MMP, PMM },
  { PMM, PPP, MPM }};

/* Vertices of a tetrahedron */
#define sqrt_3 0.5773502692

float tetrahedron[4][3] = {
  {  sqrt_3,  sqrt_3,  sqrt_3 },  /* +X, +Y, +Z */
  { -sqrt_3, -sqrt_3,  sqrt_3 },  /* -X, -Y, +Z */
  { -sqrt_3,  sqrt_3, -sqrt_3 },  /* -X, +Y, -Z */
  {  sqrt_3, -sqrt_3, -sqrt_3 }}; /* +X, -Y, -Z */

//----------------------------------------------------------------------------

#define XPLUS 0
#define XMIN  1
#define YPLUS 2
#define YMIN  3
#define ZPLUS 4
#define ZMIN  5

/* Vertices of a unit octahedron */
vtkIdType octagon[8][3] = {
  {XPLUS, ZPLUS, YPLUS},
  {YPLUS, ZPLUS, XMIN},
  {XMIN,  ZPLUS, YMIN},
  {YMIN,  ZPLUS, XPLUS},
  {XPLUS, YPLUS, ZMIN},
  {YPLUS, XMIN,  ZMIN},
  {XMIN,  YMIN,  ZMIN},
  {YMIN,  XPLUS, ZMIN}};

/* Six equidistant points lying on the unit sphere */
float octahedron[8][3] = {
  {+1.0,  0.0,  0.0},  // +x 
  {-1.0,  0.0,  0.0},  // -x
  { 0.0, +1.0,  0.0},  // +y
  { 0.0, -1.0,  0.0},  // -y
  { 0.0,  0.0, +1.0},  // +z
  { 0.0,  0.0, -1.0}}; // -z

//----------------------------------------------------------------------------

#define tau 0.8506508084 /* t=(1+sqrt(5))/2, tau=t/sqrt(1+t^2) */ 
#define one 0.5257311121 /* one=1/sqrt(1+t^2) , unit sphere */

#define ZA  0
#define ZB  1
#define ZC  2
#define ZD  3
#define YA  4
#define YB  5
#define YC  6
#define YD  7
#define XA  8
#define XB  9
#define XC 10
#define XD 11

/* Twelve vertices of icosahedron on unit sphere */
float icosahedron[12][3] = {
  {  tau,  one, 0.0 },  // ZA
  { -tau,  one, 0.0 },  // ZB
  { -tau, -one, 0.0 },  // ZC
  {  tau, -one, 0.0 },  // ZD
  {  one, 0.0,  tau },  // YA
  {  one, 0.0, -tau },  // YB
  { -one, 0.0, -tau },  // YC
  { -one, 0.0 , tau },  // YD
  { 0.0,  tau,  one },  // XA
  { 0.0, -tau,  one },  // XB
  { 0.0, -tau, -one },  // XC
  { 0.0,  tau, -one }}; // XD

/* Structure for unit icosahedron */
vtkIdType ico[20][3] = {
  { YA, XA, YD },
  { YA, YD, XB },
  { YB, YC, XD }, 
  { YB, XC, YC }, 
  { ZA, YA, ZD },
  { ZA, ZD, YB }, 
  { ZC, YD, ZB }, 
  { ZC, ZB, YC }, 
  { XA, ZA, XD },
  { XA, XD, ZB },
  { XB, XC, ZD }, 
  { XB, ZC, XC },
  { XA, YA, ZA },
  { XD, ZA, YB }, 
  { YA, XB, ZD },
  { YB, ZD, XC },
  { YD, XA, ZB }, 
  { YC, ZB, XD },
  { YD, ZC, XB },
  { YC, XC, ZC }};

vtkStandardNewMacro(vtkmpjSphereSubdivisionSource);

//----------------------------------------------------------------------------
vtkmpjSphereSubdivisionSource::vtkmpjSphereSubdivisionSource()
{
  this->Depth = 1;
  this->DuplicatePoints = 1;
  this->SubdivisionType = 0;
}

//----------------------------------------------------------------------------
void vtkmpjSphereSubdivisionSource::InitializePolys(vtkPoints *pts, vtkCellArray *polys)
{
  // Create initial polyhedron
  switch(this->SubdivisionType) {
    
  case OCTAHEDRON:
    
    // Create octahedron
    for (int i=0;i<6;i++) {
      pts->InsertNextPoint((float *)&octahedron[i]);
    }
    
    // Generate mesh connectivity
    for (int i=0;i<8;i++)
      polys->InsertNextCell(3, (vtkIdType *)&octagon[i]);
    
    break;
    
  case TETRAHEDRON:

    // Create tetrahedron
    for (int i=0;i<4;i++) {
      pts->InsertNextPoint((float *)&tetrahedron[i]);
    }
    
    // Generate mesh connectivity
    for (int i=0;i<4;i++)
      polys->InsertNextCell(3, (vtkIdType *)&tetra[i]);
    
    break;
    
  case ICOSAHEDRON:
    
    // Create icosahedron
    for (int i=0;i<12;i++) {
      pts->InsertNextPoint((float *)&icosahedron[i]);
    }
    
    // Generate mesh connectivity
    for (int i=0;i<20;i++)
      polys->InsertNextCell(3, (vtkIdType *)&ico[i]);
    
  };

}

//----------------------------------------------------------------------------
void vtkmpjSphereSubdivisionSource::FindPoints(vtkPoints *pts, double a[3], double b[3], double c[3], vtkIdType &pa, vtkIdType &pb, vtkIdType &pc)
{
  int numpts = pts->GetNumberOfPoints();
  double *x;
  
  pa = pb = pc = -1;
  
  for (int i=numpts-1; i>=0; i--) {
    x = pts->GetPoint(i);
    if ((a[0]==x[0]) && (a[1]==x[1]) && (a[2]==x[2])) pa = i;
    if ((b[0]==x[0]) && (b[1]==x[1]) && (b[2]==x[2])) pb = i;
    if ((c[0]==x[0]) && (c[1]==x[1]) && (c[2]==x[2])) pc = i;
  }
}

//----------------------------------------------------------------------------
void vtkmpjSphereSubdivisionSource::SubdividePolys(vtkPoints *newPoints, vtkCellArray *oldCells, vtkCellArray *newCells)
{
  vtkIdType numPts, numCells, *pts, pa, pb, pc;
  double *x0, *x1, *x2;
  double a[3], b[3], c[3];
  
  numPts = newPoints->GetNumberOfPoints();
  numCells = oldCells->GetNumberOfCells();

  // initiate triangle traversal
  oldCells->InitTraversal();  
  
  // traverse each triangle 
  while(oldCells->GetNextCell(numPts, pts) != 0) {
    
    // fetch cell points
    x0 = newPoints->GetPoint(pts[0]);
    x1 = newPoints->GetPoint(pts[1]);
    x2 = newPoints->GetPoint(pts[2]);

    // compute midpoint coordinates
    for(int i=0;i<3;i++) {
      a[i] = (x0[i] + x2[i]) * 0.5;
      b[i] = (x0[i] + x1[i]) * 0.5;
      c[i] = (x1[i] + x2[i]) * 0.5;
    }
    
    // normalize coordinates
    (void)vtkMath::Normalize(a);
    (void)vtkMath::Normalize(b);
    (void)vtkMath::Normalize(c);

    // try finding midpoints
    if (this->DuplicatePoints == 0) {

      this->FindPoints(newPoints, a, b, c, pa, pb, pc);      

      // insert midpoints      
      if (pa < 0) pa = newPoints->InsertNextPoint(a);
      if (pb < 0) pb = newPoints->InsertNextPoint(b);
      if (pc < 0) pc = newPoints->InsertNextPoint(c);
    } else {
      pa = newPoints->InsertNextPoint(a);
      pb = newPoints->InsertNextPoint(b);
      pc = newPoints->InsertNextPoint(c);
    }

    // triangle [0,b,a]
    newCells->InsertNextCell(3);
    newCells->InsertCellPoint(pts[0]);
    newCells->InsertCellPoint(pb);
    newCells->InsertCellPoint(pa);

    // triangle [b,1,c]
    newCells->InsertNextCell(3);
    newCells->InsertCellPoint(pb);
    newCells->InsertCellPoint(pts[1]);
    newCells->InsertCellPoint(pc);
    
    // triangle [a,b,c]
    newCells->InsertNextCell(3);
    newCells->InsertCellPoint(pa);
    newCells->InsertCellPoint(pb);
    newCells->InsertCellPoint(pc);

    // triangle [a,c,2]
    newCells->InsertNextCell(3);
    newCells->InsertCellPoint(pa);
    newCells->InsertCellPoint(pc);
    newCells->InsertCellPoint(pts[2]);
    
  }

}

//----------------------------------------------------------------------------
void vtkmpjSphereSubdivisionSource::Execute()
{
  int i, j;
  //int jStart, jEnd, numOffset;
  int numPts, numPolys;
  vtkPoints *newPoints; 
  vtkCellArray *newPolys, *oldPolys;
  vtkPolyData *output = this->GetOutput();
  
  // Set things up; allocate memory
  //
  vtkDebugMacro("vtkmpjSphereSubdivisionSource Executing");
  
  // number of creating triangles
  // n'th approximation is 8 * 4^(n-1)
  //  numPolys = 8;
  newPolys = vtkCellArray::New();
  //newPolys->Allocate(newPolys->EstimateSize(numPolys, 3));
  
  newPoints = vtkPoints::New();
    
  // create intial polyhedron
  this->InitializePolys(newPoints, newPolys);

  // subdivide polyhedron
  for (int level = 1; level<this->Depth; level++) {
    
    oldPolys = newPolys;
    numPolys = oldPolys->GetNumberOfCells();
    numPts = newPoints->GetNumberOfPoints();
    
    //printf("level=%d, points=%d, cells=%d\n",level,numPts,numPolys);
    
    newPolys = vtkCellArray::New();
    newPolys->Allocate(newPolys->EstimateSize(numPolys*4, 3));
    
    this->SubdividePolys(newPoints, oldPolys, newPolys);
    
    oldPolys->Delete();
  }
  
  numPolys = newPolys->GetNumberOfCells();
  numPts = newPoints->GetNumberOfPoints();
  
  printf("level=%d, points=%d, cells=%d\n",this->Depth,numPts,numPolys);
  
  //for(int i=0;i<numPts;i++) {
  //float *x = newPoints->GetPoint(i);
  // printf("x=%f,%f,%f\n",x[0],x[1],x[2]);
  //}
    
  // Update ourselves and release memeory
  newPoints->Squeeze();
  output->SetPoints(newPoints);
  newPoints->Delete();

  newPolys->Squeeze();
  output->SetPolys(newPolys);
  newPolys->Delete();
}

//----------------------------------------------------------------------------
void vtkmpjSphereSubdivisionSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  
  os << indent << "Depth: " << this->Depth << "\n";
  os << indent << "DuplicatePoints: " << this->DuplicatePoints << "\n";

}

