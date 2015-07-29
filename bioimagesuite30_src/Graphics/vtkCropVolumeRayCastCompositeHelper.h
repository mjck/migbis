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
  Module:    $RCSfile: vtkCropVolumeRayCastCompositeHelper.h,v $
  Language:  C++
  Date:      $Date: 2005/05/04 14:13:58 $
  Version:   $Revision: 1.1 $

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME vtkCropVolumeRayCastCompositeHelper - A helper that generates composite images for the volume ray cast mapper
// .SECTION Description
// This is one of the helper classes for the vtkCropVolumeRayCastMapper.
// It will generate composite images using an alpha blending operation.
// This class should not be used directly, it is a helper class for
// the mapper and has no user-level API.
//
// .SECTION see also
// vtkCropVolumeRayCastMapper

#ifndef __vtkCropVolumeRayCastCompositeHelper_h
#define __vtkCropVolumeRayCastCompositeHelper_h

#include "vtkFixedPointVolumeRayCastHelper.h"

class vtkCropVolumeRayCastMapper;
class vtkVolume;

class  vtkCropVolumeRayCastCompositeHelper : public vtkFixedPointVolumeRayCastHelper
{
public:
  static vtkCropVolumeRayCastCompositeHelper *New();
  vtkTypeRevisionMacro(vtkCropVolumeRayCastCompositeHelper,vtkFixedPointVolumeRayCastHelper);
  void PrintSelf( ostream& os, vtkIndent indent );

  virtual void  GenerateImage( int threadID,
                               int threadCount,
                               vtkVolume *vol,
                               vtkCropVolumeRayCastMapper *mapper);


protected:
  vtkCropVolumeRayCastCompositeHelper();
  ~vtkCropVolumeRayCastCompositeHelper();

private:
  vtkCropVolumeRayCastCompositeHelper(const vtkCropVolumeRayCastCompositeHelper&);  // Not implemented.
  void operator=(const vtkCropVolumeRayCastCompositeHelper&);  // Not implemented.

};

#endif



