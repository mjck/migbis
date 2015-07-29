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
  Module:    $RCSfile: vtkbisCylinderSource.h,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkbisCylinderSource - generate polygonal cone 
// .SECTION Description
// vtkbisCylinderSource creates a cylinder centered at a specified point and pointing in
// a specified direction. (By default, the center is the origin and the
// direction is the x-axis.) Depending upon the resolution of this object,
// different representations are created. If resolution=0 a line is created;
// if resolution=1, a single triangle is created; if resolution=2, two
// crossed triangles are created. For resolution > 2, a 3D cylinder (with
// resolution number of sides) is created. It also is possible to control
// whether the bottom of the cylinder is capped with a (resolution-sided)
// polygon, and to specify the height and eradius of the cylinder.

#ifndef __vtkbisCylinderSource_h
#define __vtkbisCylinderSource_h

#include "vtkConeSource.h"

class  vtkbisCylinderSource : public vtkConeSource 
{
public:
  vtkTypeRevisionMacro(vtkbisCylinderSource,vtkConeSource);


  // Description:
  // Construct with default resolution 6, height 1.0, radius 0.5, and
  // capping on. The cone is centered at the origin and points down
  // the x-axis.
  static vtkbisCylinderSource *New();


protected:
  vtkbisCylinderSource(int res=6);

  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  
private:
  vtkbisCylinderSource(const vtkbisCylinderSource&);  // Not implemented.
  void operator=(const vtkbisCylinderSource&);  // Not implemented.
};

#endif



