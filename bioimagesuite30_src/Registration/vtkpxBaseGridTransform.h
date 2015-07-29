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

//
// This file derives from original work
// in the vtkCISG Toolkit
// that was released under the terms of 
// the GNU General Public License (either version 2,
// of the License, or any later version.)
//
// It may consist of work derived from more than one source
// file in the vtkCISG Toolkit. The original copyright notice
// was:
//
//  Authors   : Thomas Hartkens <thomas@hartkens.de>
//              Daniel Rueckert <d.rueckert@doc.ic.ac.uk>
//	        Julia Schnabel  <julia.schnabel@kcl.ac.uk>
//	        under the supervision of Dave Hawkes and Derek Hill
//  Web       : http://www.image-registration.com
//  Copyright : King's College London
//              Div. of Radiological Sciences, 
//              Computational Imaging Science Group, 1997 - 2000
//	        http://www-ipg.umds.ac.uk/cisg
//


#ifndef __vtkpxBaseGridTransform_h
#define __vtkpxBaseGridTransform_h

#include "vtkGridTransform.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "pxutil.h"
#include "vtkDoubleArray.h"

#define VTK_GRID_BSPLINE 4
#define LookupTableSize 1000

class vtkpxBaseGridTransform : public vtkGridTransform
{
 public:
  static vtkpxBaseGridTransform *New();
  vtkTypeMacro(vtkpxBaseGridTransform,vtkGridTransform);


  // Description:
  // Set interpolation mode for sampling the grid.  Higher-order
  // interpolation allows you to use a sparser grid.
  // Default: Linear.
  void SetInterpolationMode(int mode);
  void SetInterpolationModeToBSpline()
    { this->SetInterpolationMode(VTK_GRID_BSPLINE); };
  const char *GetInterpolationModeAsString();

  // Description:
  // Initialize Displacement Grid
  void InitializeDisplacementGridTransform(double bounds[6],double spacing,double offset,vtkAbstractTransform* tr=NULL);
  void InitializeDisplacementGridBounds(double bounds[6],double dx, double dy, double dz);

//BTX

  // Description:
  // Make another transform of the same type.
  vtkAbstractTransform *MakeTransform();

  /// Size of lookup table for B-spline basis function values
  /// Memory for lookup table for B-spline basis function values
  static    double LookupTable[LookupTableSize+1][4];
  static    double dLookupTable[LookupTableSize+1][4];
  static    int    LookupTableInitialized;
  static    double  LutSize;
//ETX


  // Description:
  // Return Result of B-sline basis functions 
  static double B (int, double);
  static double dB (int, double);

  // Description:
  // Load/Save Transformation
  virtual int Load(const char* fname);
  virtual int Save(const char* fname);
  virtual int Save(const char* fname,int compression);

  virtual int Load(gzFile fin,int skipheader=0,const char* header=NULL);
  virtual int Save(FILE* fout);
  virtual int Save(gzFile fout);

  // Description:
  // Export/Import to Octave Format
  virtual int Export(const char* fname,const char* vname);
  virtual int Import(const char* fname);

 protected:
  vtkpxBaseGridTransform();

//BTX

  /// Returns the value of the i-th B-spline basis function

  static double B0(double);
  static double B1(double);
  static double B2(double);
  static double B3(double);

  /// Returns the derivative of the i-th B-spline basis functio

  static double dB0(double);
  static double dB1(double);
  static double dB2(double);
  static double dB3(double);

//ETX

};



#endif




