/*=========================================================================

  Program:   Visualization Toolkit

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkmpjSuperquadricsSource - create a polygonal superquadric centered 
// at the origin
// .SECTION Description
// vtkmpjSuperquadricsSource creates a superquadric (represented by polygons) 
// of specified
// size centered at the origin. The resolution (polygonal discretization)
// in both the latitude (phi) and longitude (theta) directions can be
// specified. Roundness parameters (PhiRoundness and ThetaRoundness) control
// the shape of the superquadric.  The Toroidal boolean controls whether
// a toroidal superquadric is produced.  If so, the Thickness parameter
// controls the thickness of the toroid:  0 is the thinnest allowable
// toroid, and 1 has a minimum sized hole.  The Scale parameters allow 
// the superquadric to be scaled in x, y, and z (normal vectors are correctly
// generated in any case).  The Size parameter controls size of the 
// superquadric.
//
// This code is based on "Rigid physically based superquadrics", A. H. Barr,
// in "Graphics Gems III", David Kirk, ed., Academic Press, 1992.
//
// .SECTION Caveats
// Resolution means the number of latitude or longitude lines for a complete 
// superquadric. The resolution parameters are rounded to the nearest 4
// in phi and 8 in theta.  
//
// Texture coordinates are not equally distributed around all superquadrics.
// 
// The Size and Thickness parameters control coefficients of superquadric
// generation, and may do not exactly describe the size of the superquadric.
//

#ifndef __vtkmpjSuperquadricsSource_h
#define __vtkmpjSuperquadricsSource_h

#include "vtkPolyDataSource.h"

#define VTK_MAX_SUPERQUADRIC_RESOLUTION 1024
#define VTK_MIN_SUPERQUADRIC_THICKNESS  1e-4
#define VTK_MIN_SUPERQUADRIC_ROUNDNESS  1e-24

class vtkmpjSuperquadricsSource : public vtkPolyDataSource 
{
public:
  // Description:
  // Create a default superquadric with a radius of 0.5, non-toroidal,
  // spherical, and centered at the origin.
  static vtkmpjSuperquadricsSource *New();

  vtkTypeMacro(vtkmpjSuperquadricsSource,vtkPolyDataSource);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the center of the superquadric. Default is 0,0,0.
  vtkSetVector3Macro(Center,double);
  vtkGetVectorMacro(Center,double,3);

  // Description:
  // Set the scale factors of the superquadric. Default is 1,1,1.
  vtkSetVector3Macro(Scale,double);
  vtkGetVectorMacro(Scale,double,3);

  // Description:
  // Set the number of points in the longitude direction.
  vtkGetMacro(ThetaResolution,int);
  void SetThetaResolution(int i);

  // Description:
  // Set the number of points in the latitude direction.
  vtkGetMacro(PhiResolution,int);
  void SetPhiResolution(int i);

  // Description:
  // Set/Get Superquadric ring thickness (toroids only).
  // Changing thickness maintains the outside diameter of the toroid.
  vtkGetMacro(Thickness,double);
  vtkSetClampMacro(Thickness,double,VTK_MIN_SUPERQUADRIC_THICKNESS,1.0);

  // Description:
  // Set/Get Superquadric north/south roundness. 
  // Values range from 0 (rectangular) to 1 (circular) to higher orders.
  vtkGetMacro(PhiRoundness,double);
  void SetPhiRoundness(double e); 

  // Description:
  // Set/Get Superquadric east/west roundness.
  // Values range from 0 (rectangular) to 1 (circular) to higher orders.
  vtkGetMacro(ThetaRoundness,double);
  void SetThetaRoundness(double e);

  // Description:
  // Set/Get Superquadric isotropic size.
  vtkSetMacro(Size,double);
  vtkGetMacro(Size,double);

  // Description:
  // Set/Get whether or not the superquadric is toroidal (1) or ellipsoidal (0).
  vtkBooleanMacro(Toroidal,int);
  vtkGetMacro(Toroidal,int);
  vtkSetMacro(Toroidal,int);

  // Description:
  // Set/Get axis swapping
  vtkBooleanMacro(SwapAxes,int);
  vtkGetMacro(SwapAxes, int);
  vtkSetMacro(SwapAxes, int);

protected:
  vtkmpjSuperquadricsSource(int res=16);
  ~vtkmpjSuperquadricsSource() {};

  void Execute();

  int Toroidal;
  int AxisOfSymmetry;
  double Thickness;
  double Size;
  double PhiRoundness;
  double ThetaRoundness;
   
  double Center[3];
  double Scale[3];
  int ThetaResolution;
  int PhiResolution;

  int SwapAxes;

private:

	double cf(double w, double m, double);
	double sf(double w, double m);

	void EvalSuperquadric(double theta, double phi,  
                          double dtheta, double dphi, 
                          double rtheta, double rphi,  
                          double dims[3],     
                          double alpha,      
                          double xyz[3],     
                          double nrm[3]);      

  vtkmpjSuperquadricsSource(const vtkmpjSuperquadricsSource&);  // Not implemented.
  void operator=(const vtkmpjSuperquadricsSource&);  // Not implemented.
};

#endif

