

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


#include "vtkmpjIcosahedronSource.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkUnsignedCharArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"

#include <math.h>

#define VTK_MPJ_EQTOL 1E-6

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
float vtkmpjIcosahedronSource_icosahedron[12][3] = {
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

/* Twelve colors of icosahedron on unit sphere */
float vtkmpjIcosahedronSource_icosahedron_color[12][4] = {
  {  tau*255,  one*255, 0.0, 255 },  // ZA
  {  tau*255,  one*255, 0.0, 255 },  // ZB
  {  tau*255, one*255, 0.0, 255 },  // ZC
  {  tau*255, one*255, 0.0, 255 },  // ZD
  {  one*255, 0.0,  tau*255, 255 },  // YA
  {  one*255, 0.0,  tau*255, 255 },  // YB
  {  one*255, 0.0,  tau*255, 255 },  // YC
  {  one*255, 0.0 , tau*255, 255 },  // YD
  { 0.0,  tau*255,  one*255, 255 },  // XA
  { 0.0,  tau*255,  one*255, 255 },  // XB
  { 0.0,  tau*255, one*255, 255 },  // XC
  { 0.0,  tau*255, one*255, 255 }}; // XD

/* Structure for unit icosahedron */
vtkIdType vtkmpjIcosahedronSource_ico[20][3] = {
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

vtkStandardNewMacro(vtkmpjIcosahedronSource);

//----------------------------------------------------------------------------
vtkmpjIcosahedronSource::vtkmpjIcosahedronSource()
{
  this->Depth = 1;
  this->AllowDuplicatePoints = 1;
}
 
//----------------------------------------------------------------------------
void vtkmpjIcosahedronSource::InitializePolys(vtkPoints *pts, vtkCellArray *polys, vtkUnsignedCharArray *scalars)
{
  // Create icosahedron
  for (int i=0;i<12;i++) {
    pts->InsertNextPoint((float *)&vtkmpjIcosahedronSource_icosahedron[i]);
    
    // insert scalars
    scalars->InsertNextTuple((float *)&vtkmpjIcosahedronSource_icosahedron_color[i]);
  }
  
  // Generate mesh connectivity
  for (int i=0;i<20;i++)
    polys->InsertNextCell(3, (vtkIdType *)&vtkmpjIcosahedronSource_ico[i]);
  
}

//----------------------------------------------------------------------------
void vtkmpjIcosahedronSource::FindOrInsert1Point(vtkPoints *newPoints, vtkUnsignedCharArray *newScalars, float a[3], vtkIdType &ra)
{
  int numpts = newPoints->GetNumberOfPoints();
  int found = 0;
  double *x;
  
  // initialize
  ra = -1;
  
  // compare
  for (int i=numpts-1; i>=0; i--) {    

    x = newPoints->GetPoint(i);    

    // if point is found, return
    if ((fabs(a[0]-x[0])<VTK_MPJ_EQTOL) && (fabs(a[1]-x[1])<VTK_MPJ_EQTOL) && (fabs(a[2]-x[2])<VTK_MPJ_EQTOL)) {
      
      ra = i;
      
      found = 1;
      
      break;
    }
    
  }
  
  // append new point
  if (found == 0) {
    float ca[4];
    ra = newPoints->InsertNextPoint(a[0], a[1], a[2]);
    ca[0] = 255*fabs(a[0]); ca[1]=255*fabs(a[1]); ca[2]=255*fabs(a[2]); ca[3]=255;
    newScalars->InsertNextTuple(ca);
  }

}

//----------------------------------------------------------------------------
void vtkmpjIcosahedronSource::FindOrInsert3Points(vtkPoints *newPoints, vtkUnsignedCharArray *newScalars, float a[3], float b[3], float c[3], vtkIdType &ra, vtkIdType &rb, vtkIdType &rc)
{
  int numpts = newPoints->GetNumberOfPoints();
  int found[3] = {0, 0, 0};
  double *x;
  
  // initialize
  ra = rb = rc = -1;
  
  // compare points
  for (int i=numpts-1; i>=0; i--) {

    x = newPoints->GetPoint(i);
    
    if ((fabs(a[0]-x[0])<VTK_MPJ_EQTOL) && (fabs(a[1]-x[1])<VTK_MPJ_EQTOL) && (fabs(a[2]-x[2])<VTK_MPJ_EQTOL)) 
      { ra = i; found[0] = 1; }
    if ((fabs(b[0]-x[0])<VTK_MPJ_EQTOL) && (fabs(b[1]-x[1])<VTK_MPJ_EQTOL) && (fabs(b[2]-x[2])<VTK_MPJ_EQTOL)) 
      { rb = i; found[1] = 1; }
    if ((fabs(c[0]-x[0])<VTK_MPJ_EQTOL) && (fabs(c[1]-x[1])<VTK_MPJ_EQTOL) && (fabs(c[2]-x[2])<VTK_MPJ_EQTOL)) 
      { rc = i; found[2] = 1; }

    // if all points were found, break out
    if (found[0] && found[1] && found[2])
      break;
    
  }

  // append new point(s)
  if (found[0] == 0) {
    float ca[4];
    ra = newPoints->InsertNextPoint(a[0], a[1], a[2]);
    ca[0] = 255*fabs(a[0]); ca[1]=255*fabs(a[1]); ca[2]=255*fabs(a[2]); ca[3]=255;
    newScalars->InsertNextTuple(ca);
  }
  if (found[1] == 0) {
    float cb[3];
    rb = newPoints->InsertNextPoint(b[0], b[1], b[2]);
    cb[0] = 255*fabs(b[0]); cb[1]=255*fabs(b[1]); cb[2]=255*fabs(b[2]); cb[3]=255;
    newScalars->InsertNextTuple(cb);
  }
  if (found[2] == 0) {
    float cc[3];
    rc = newPoints->InsertNextPoint(c[0], c[1], c[2]);
    cc[0] = 255*fabs(c[0]); cc[1]=255*fabs(c[1]); cc[2]=255*fabs(c[2]); cc[3]=255;
    newScalars->InsertNextTuple(cc);
  }

}

//----------------------------------------------------------------------------
void vtkmpjIcosahedronSource::SubdividePolys(vtkPoints *newPoints, vtkCellArray *oldCells, vtkCellArray *newCells, vtkUnsignedCharArray *newScalars)
{
  vtkIdType numPts, numCells, *pts;
  float *v;
  float x0[3], x1[3], x2[3], x3[3], x4[3];
  float a[3], b[3], c[3], pa[3], pb[3], pc[3];
   
  numPts = newPoints->GetNumberOfPoints();
  numCells = oldCells->GetNumberOfCells();
  
  // initiate triangle traversal
  oldCells->InitTraversal();  
  
  int count=-1;

  // traverse each triangle 
  while(oldCells->GetNextCell(numPts, pts) != 0) {
    
    count++;
    
    // fetch cells
    this->conn[0][this->Depth]=pts[0];
    this->conn[0][0]=pts[1];
    this->conn[this->Depth][0]=pts[2];

    // fetch cell points
    for(int i=0;i<3;i++) {
      x0[i] = (newPoints->GetPoint(pts[0]))[i]; 
      x1[i] = (newPoints->GetPoint(pts[1]))[i];
      x2[i] = (newPoints->GetPoint(pts[2]))[i];
    }
    
    // compute edge increment
    for(int i=0;i<3;i++) {
      a[i] = -(x0[i] - x2[i]) * (1.0 / (float)this->Depth);
      b[i] = -(x0[i] - x1[i]) * (1.0 / (float)this->Depth);
      c[i] = -(x1[i] - x2[i]) * (1.0 / (float)this->Depth);
    }
    
    // subdivide each edge of the current triangle
    for(int k=1;k<this->Depth;k++) {
      for(int i=0;i<3;i++) {
	pa[i] = x0[i] + a[i]*k;
	pb[i] = x0[i] + b[i]*k;
	pc[i] = x1[i] + c[i]*k; 
      }
      
      // normalize coordinates
      (void)vtkMath::Normalize(pa);
      (void)vtkMath::Normalize(pb);
      (void)vtkMath::Normalize(pc);
      
      // insert points
      if (this->AllowDuplicatePoints) {

	// allow point duplicates
	this->conn[k][this->Depth-k] = newPoints->InsertNextPoint(pa[0],pa[1],pa[2]);
	this->conn[0][this->Depth-k] = newPoints->InsertNextPoint(pb[0],pb[1],pb[2]);
	this->conn[k][0] = newPoints->InsertNextPoint(pc[0],pc[1],pc[2]);

	// insert new scalars
	float sc[4]={0.0,0.0,0.0,255};

	sc[0] = fabs(pa[0])*255; sc[1] = fabs(pa[1])*255; sc[2] = fabs(pa[2])*255;
	newScalars->InsertNextTuple(sc);

	sc[0] = fabs(pb[0])*255; sc[1] = fabs(pb[1])*255; sc[2] = fabs(pb[2])*255;
	newScalars->InsertNextTuple(sc);

	sc[0] = fabs(pc[0])*255; sc[1] = fabs(pc[1])*255; sc[2] = fabs(pc[2])*255;
	newScalars->InsertNextTuple(sc);

      } else {

	// non-duplication employs linear search - slow
	vtkIdType ra, rb, rc;
	
	FindOrInsert3Points(newPoints, newScalars, pa, pb, pc, ra, rb, rc);
	
	this->conn[k][this->Depth-k] = ra;
	this->conn[0][this->Depth-k] = rb;
	this->conn[k][0] = rc;
	
      }     
    }
    
    // compute points inside the triangle
    for(int l=2;l<this->Depth;l++) {

      for(int i=0;i<3;i++) {
	x3[i] = (newPoints->GetPoint(this->conn[0][l]))[i];
	x4[i] = (newPoints->GetPoint(this->conn[l][0]))[i];
      }
      
      // compute increment
      for(int i=0;i<3;i++) {
	a[i] = -(x3[i] - x4[i]) * (1.0 / (float)l);
      }
      
      // compute point coordinates
      for(int k=1;k<l;k++) {
	for(int i=0;i<3;i++) {
	  pa[i] = x3[i] + a[i]*k;	  
	} 
	
	// normalize coordinates
	(void)vtkMath::Normalize(pa);
	
	// insert points
	if (this->AllowDuplicatePoints) {
	  
	  // allow point duplicates
	  this->conn[k][l-k] = newPoints->InsertNextPoint(pa[0],pa[1],pa[2]);

	  // insert new scalars
	  float sc[4] = {0,0,0,255};

	  sc[0] = fabs(pa[0])*255; sc[1] = fabs(pa[1])*255; sc[2] = fabs(pa[2])*255;
	  newScalars->InsertNextTuple(sc);
	  
	} else {

	  // non-duplication employs linear search - slow
	  vtkIdType ra;
	  
	  FindOrInsert1Point(newPoints, newScalars, pa, ra);
	  
	  this->conn[k][l-k] = ra;
	}
      }      
    }
    
    // create new triangle cells

    // insert first batch of new triangles
    for(int i=0;i<this->Depth;i++)
      for(int j=this->Depth-i;j>0;j--) {
	// triangle [ij,ij-1,i+1j-1]
	newCells->InsertNextCell(3);
	newCells->InsertCellPoint(this->conn[i][j]);
	newCells->InsertCellPoint(this->conn[i][j-1]);
	newCells->InsertCellPoint(this->conn[i+1][j-1]);
      }

    // insert second batch of new triangles
    for(int i=1;i<this->Depth;i++)
      for(int j=this->Depth-i;j>0;j--) {
	// triangle [ij,i-1j,ij-1]
	newCells->InsertNextCell(3);
	newCells->InsertCellPoint(this->conn[i][j]);
	newCells->InsertCellPoint(this->conn[i-1][j]);
	newCells->InsertCellPoint(this->conn[i][j-1]);
      }  
    
  }
}

//----------------------------------------------------------------------------
void vtkmpjIcosahedronSource::Execute()
{
  int i, j;
  int numPts, numPolys;
  vtkPoints *newPoints; 
  vtkUnsignedCharArray *newScalars;
  vtkCellArray *newPolys, *oldPolys;
  vtkPolyData *output = this->GetOutput();
  vtkPointData *outPD = output->GetPointData();

  // Set things up; allocate memory
  //
  vtkDebugMacro("vtkmpjIcosahedronSource Executing");
  
  // allocate space for connectivity information
  this->conn = new vtkIdType*[this->Depth+1];
  for (int i=0;i<this->Depth+1;i++)
    this->conn[i] = new vtkIdType[this->Depth+1-i];

  newPolys = vtkCellArray::New();
  newPoints = vtkPoints::New();

  // create scalars
  newScalars = vtkUnsignedCharArray::New();
  newScalars->SetNumberOfComponents(4);
  
  // create intial polyhedron
  this->InitializePolys(newPoints, newPolys, newScalars);
  
  oldPolys = newPolys;
  numPolys = oldPolys->GetNumberOfCells();
  numPts = newPoints->GetNumberOfPoints();
  
  newPolys = vtkCellArray::New();
  
  // subdivide polygons
  this->SubdividePolys(newPoints, oldPolys, newPolys, newScalars);
  
  oldPolys->Delete();
  
  numPolys = newPolys->GetNumberOfCells();
  numPts = newPoints->GetNumberOfPoints();
  
  //  printf("level=%d, points=%d, cells=%d\n",this->Depth,numPts,numPolys);  
  
  // Update ourselves and release memeory
  newPoints->Squeeze();
  output->SetPoints(newPoints);
  newPoints->Delete();

  newPolys->Squeeze();

  output->SetPolys(newPolys);
  newPolys->Delete();

  outPD->SetScalars(newScalars);
  newScalars->Delete();

  // deleting connectivity information
  for (int i=0;i<this->Depth+1;i++)
    delete[] this->conn[i];
  
  delete[] this->conn;
}

//----------------------------------------------------------------------------
void vtkmpjIcosahedronSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  
  os << indent << "Depth: " << this->Depth << "\n";
  os << indent << "AllowDuplicatePoints: " << this->AllowDuplicatePoints << "\n";

}

