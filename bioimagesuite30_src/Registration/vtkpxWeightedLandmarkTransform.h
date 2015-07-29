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





// .NAME vtkpxWeightedLandmarkTransform - a linear transform specified by two corresponding point sets
// .SECTION Description
// A vtkpxWeightedLandmarkTransform is defined by two sets of landmarks, the 
// transform computed gives the best fit mapping one onto the other, in a 
// least squares sense. The indices are taken to correspond, so point 1 
// in the first set will get mapped close to point 1 in the second set, 
// etc. Call SetSourceLandmarks and SetTargetLandmarks to specify the two
// sets of landmarks, ensure they have the same number of points.
// .SECTION Caveats
// Whenever you add, subtract, or set points you must call Modified()
// on the vtkPoints object, or the transformation might not update.
// .SECTION Modification
// Added Weight Parameter to Establish a weighted Least Squares instead
// Xenios Papademetris November 2002
// .SECTION see also
// vtkLinearTransform

#ifndef __vtkpxWeightedLandmarkTransform_h
#define __vtkpxWeightedLandmarkTransform_h

#include "vtkLandmarkTransform.h"
#include "vtkMatrix4x4.h"
#include "vtkPoints.h"
#include "vtkpxMatrix.h"

class vtkpxWeightedLandmarkTransform : public vtkLandmarkTransform
{
public:
  static vtkpxWeightedLandmarkTransform *New();
  vtkTypeMacro(vtkpxWeightedLandmarkTransform,vtkLandmarkTransform);

  vtkSetObjectMacro(Weights,vtkpxMatrix);
  vtkGetObjectMacro(Weights, vtkpxMatrix);


protected:

  vtkpxWeightedLandmarkTransform();
  virtual ~vtkpxWeightedLandmarkTransform();

  // Update the matrix from the quaternion.
  void InternalUpdate();

  vtkpxMatrix* Weights;
};
#endif

