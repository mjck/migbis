//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
//BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
//BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
//BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
//BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
//BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
//BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//BIOIMAGESUITE_LICENSE  See also  http://www.gnu.org/licenses/gpl.html
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
//BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
//BIOIMAGESUITE_LICENSE 
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkjoSphereSource.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkjoSphereSource - create a polygonal sphere centered at the origin
// .SECTION Description
// vtkjoSphereSource creates a sphere (represented by polygons) of specified
// radius centered at the origin. This class extends VTK's vtkSphereSource class
// by allowing a second style of tessellation of the sphere.  For the first style
// that uses latitudinal and longitudinal tessellation, the resolution (polygonal
// discretization) in both the latitude (phi) and longitude (theta) directions can be
// specified. It also is possible to create partial spheres by specifying
// maximum phi and theta angles. By default, the surface tessellation of
// the sphere uses triangles; however you can set LatLongTessellation to
// produce a tessellation using quadrilaterals.  Alternatively, users may choose
// to tessellate the sphere starting from an initial icosahedron polygon and
// subsequently subdividing each of the faces.  The SubdivisionsLevel variable
// sets the number of recursive subdivisions.
// .SECTION Caveats
// Resolution means the number of latitude or longitude lines for a complete
// sphere. If you create partial spheres the number of latitude/longitude
// lines may be off by one.

#ifndef __vtkjoSphereSource_h
#define __vtkjoSphereSource_h

#include "vtkPolyDataAlgorithm.h"

#define VTK_MAX_SPHERE_RESOLUTION 1024
#define VTK_MAX_SPHERE_SUBDIVISIONS 8

class vtkjoSphereSource : public vtkPolyDataAlgorithm
{
public:
  vtkTypeRevisionMacro(vtkjoSphereSource,vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct sphere with radius=0.5 and default resolution 8 in both Phi
  // and Theta directions. Theta ranges from (0,360) and phi (0,180) degrees.
  static vtkjoSphereSource *New();

  // Description:
  // Set radius of sphere. Default is .5.
  vtkSetClampMacro(Radius,double,0.0,VTK_DOUBLE_MAX);
  vtkGetMacro(Radius,double);

  // Description:
  // Set the center of the sphere. Default is 0,0,0.
  vtkSetVector3Macro(Center,double);
  vtkGetVectorMacro(Center,double,3);

  // Description:
  // Set the number of points in the longitude direction (ranging from
  // StartTheta to EndTheta).
  vtkSetClampMacro(ThetaResolution,int,3,VTK_MAX_SPHERE_RESOLUTION);
  vtkGetMacro(ThetaResolution,int);

  // Description:
  // Set the number of points in the latitude direction (ranging
  // from StartPhi to EndPhi).
  vtkSetClampMacro(PhiResolution,int,3,VTK_MAX_SPHERE_RESOLUTION);
  vtkGetMacro(PhiResolution,int);

  // Description:
  // Set the starting longitude angle. By default StartTheta=0 degrees.
  vtkSetClampMacro(StartTheta,double,0.0,360.0);
  vtkGetMacro(StartTheta,double);

  // Description:
  // Set the ending longitude angle. By default EndTheta=360 degrees.
  vtkSetClampMacro(EndTheta,double,0.0,360.0);
  vtkGetMacro(EndTheta,double);

  // Description:
  // Set the starting latitude angle (0 is at north pole). By default
  // StartPhi=0 degrees.
  vtkSetClampMacro(StartPhi,double,0.0,360.0);
  vtkGetMacro(StartPhi,double);

  // Description:
  // Set the ending latitude angle. By default EndPhi=180 degrees.
  vtkSetClampMacro(EndPhi,double,0.0,360.0);
  vtkGetMacro(EndPhi,double);

  // Description:
  // Cause the sphere to be tessellated with edges along the latitude
  // and longitude lines. If off, triangles are generated at non-polar
  // regions, which results in edges that are not parallel to latitude and
  // longitude lines. If on, quadrilaterals are generated everywhere
  // except at the poles. This can be useful for generating a wireframe
  // sphere with natural latitude and longitude lines.
  vtkSetMacro(LatLongTessellation,int);
  vtkGetMacro(LatLongTessellation,int);
  vtkBooleanMacro(LatLongTessellation,int);

  // Description:
  // Start with an icosahedron rather than use longitude and latitude
  // style for constructing the sphere.  The vertices are more uniformly
  // distributed throughout the sphere surface.
  vtkSetMacro(IcosahedralTessellation, int);
  vtkGetMacro(IcosahedralTessellation, int);
  vtkBooleanMacro(IcosahedralTessellation, int);

  // Description:
  // Set the number of subdivisions to perform on the icosahedron when creating
  // the sphere.  0 subdivions will return the initial icosahedron.
  vtkSetClampMacro(SubdivisionLevels, int, 0, VTK_MAX_SPHERE_SUBDIVISIONS);
  vtkGetMacro(SubdivisionLevels, int);

protected:
  vtkjoSphereSource(int res=8);
  ~vtkjoSphereSource() {}

  int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  double Radius;
  double Center[3];
  int ThetaResolution;
  int PhiResolution;
  double StartTheta;
  double EndTheta;
  double StartPhi;
  double EndPhi;
  int LatLongTessellation;
  int IcosahedralTessellation;
  int SubdivisionLevels;

  int PerformLatLongTessellation(vtkInformationVector *);
  int PerformIcosahedralTessellation(vtkInformationVector *);

private:
  vtkjoSphereSource(const vtkjoSphereSource&);  // Not implemented.
  void operator=(const vtkjoSphereSource&);  // Not implemented.
};

#endif

