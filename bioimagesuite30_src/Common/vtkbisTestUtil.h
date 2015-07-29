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






#ifndef __vtkbisTestUtil_h
#define __vtkbisTestUtil_h

#include "vtkObject.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"
#include "vtkPolyData.h"
#include <string>

class vtkbisTestUtil : public vtkObject
{
public:
  static vtkbisTestUtil *New();
  vtkTypeMacro(vtkbisTestUtil,vtkObject);
  
  // -------------------------------------------------------
  // Compare Image and Surfaces
  // -------------------------------------------------------
   int CompareDataArrays(vtkDataArray* data1,vtkDataArray* data2);
   int CompareDataArrays(vtkDataArray* data1,vtkDataArray* data2,double threshold);

   int CorrelateDataArrays(vtkDataArray* data1,vtkDataArray* data2);
   int CorrelateDataArrays(vtkDataArray* data1,vtkDataArray* data2,double threshold);


   int CompareImages(vtkImageData* img1,vtkImageData* img2,double threshold);
   int CompareImages(vtkImageData* img1,vtkImageData* img2);

   int CorrelateImages(vtkImageData* img1,vtkImageData* img2,double threshold);
   int CorrelateImages(vtkImageData* img1,vtkImageData* img2);

   int CompareSurfaces(vtkPolyData* img1,vtkPolyData* img2,double threshold);
   int CompareSurfaces(vtkPolyData* img1,vtkPolyData* img2);

   int ComparePoints(vtkPoints* pt1,vtkPoints* pt2);
   int ComparePoints(vtkPoints* pt1,vtkPoints* pt2,double threshold);

   int CompareSurfacesByCorrespondence(vtkPolyData* img1,vtkPolyData* img2,double threshold);

   const char* ReturnStatus();
   void ClearStatus();


protected:

  //BTX
  vtkbisTestUtil();

   std::string Status;
   char Buffer[200];
  //ETX
  
};
#endif

