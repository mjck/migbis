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

// ------------------------------------------------------------------------
//
// This file derives from original work
// in the vtkCISG Toolkit
// that was released under the terms of 
// the GNU General Public License (either version 2,
// of the License, or any later version.)
// 
// The code has been modified for inclusion in BioImage Suite
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
// ------------------------------------------------------------------------



#ifndef __vtkpxLinearTransform_h
#define __vtkpxLinearTransform_h

#include "vtkTransform.h"
#include "vtkMatrix4x4.h"
#include "vtkDoubleArray.h"

class vtkImageData;

class vtkpxLinearTransform : public vtkTransform
{
 public:
  static vtkpxLinearTransform *New();
  vtkTypeMacro(vtkpxLinearTransform,vtkTransform);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the parameters
  void SetParameters(vtkDoubleArray *parameters);
  void SetParameters(vtkDoubleArray *parameters,int frame);
  void GetParameters(vtkDoubleArray *parameters);
  void GetParameters(vtkDoubleArray *parameters,int frame);

  // Description:
  // Puts a transformation parameter (transformation matrix is updated)
  void   Put(int, float);
  void   Set(int a, float v) {  Put(a,v);}
  /// Gets a transformation parameter
  float Get(int) const;

  void SetFlipX(int f);
  void SetFlipY(int f);
  void SetFlipZ(int f);
  vtkGetMacro(FlipX, int);
  vtkGetMacro(FlipY, int);
  vtkGetMacro(FlipZ, int);
  
  // Description:
  // Stop Auto Updates
  vtkSetClampMacro(FreezeUpdates,int,0,1);
  vtkGetMacro(FreezeUpdates,int);

  void SetDistortionMode(int m);
  vtkGetMacro(DistortionMode, int);

  // Description:
  // Load / Save Transformation
  virtual int Save(char* fname);
  virtual int Load(char* fname);

  // Description:
  // Invert the transformation.  This will also set a flag so that
  // the transformation will use the inverse of its Input, if an Input
  // has been set.
  void Inverse();
  void UpdateParameters();

  // Description:
  // Number Of Parameters
  virtual int GetNumberOfParameters() { return 19;}

  // Description:
  // Set Identity
  virtual void Identity();

  // Description:
  // Copy
  virtual void Copy(vtkpxLinearTransform* other);


  // Description:
  // This is key code for setting/exporting transformation given an input
  // and images of different orientations

  int ManualSetParameters(float tx,float ty,float tz,
			  float rx,float ry,float rz,
			  float sx,float sy,float sz,
			  vtkImageData* reference,int ref_orientation,
			  vtkImageData* transform,int trn_orientation,
			  vtkTransform* combined);
  
  int ExtractParameters(vtkAbstractTransform* tr,int nparam,
			vtkImageData* reference,int ref_orientation,
			vtkImageData* transform,int trn_orientation);


  int GenerateUpdateTransform(vtkTransform* output,
			      vtkImageData* reference,int ref_orientation,
			      vtkImageData* transform,int trn_orientation);
			      


  int GenerateUpdateTransform(vtkTransform* output,
			      float minusshift[3],int ref_orientation,
			      float shift[3],int trn_orientation);
			      


  
protected:
  vtkpxLinearTransform ();
  ~vtkpxLinearTransform ();

//BTX
  /// Scaling along the x-axis, y-axis, z-axis
  float _sx,_sy,_sz;
  /// Skew angle in the x-y ...  plane (in degrees)
  float _sxy, _syx,_syz,_szy,_szx,_sxz;
  /// Translation along the x-axis (in mm)
  float _tx,_ty, _tz;
  /// Rotation around the x-axis (in degrees)
  float _rx,_ry,_rz ;

  int FlipX;
  int FlipY;
  int FlipZ;

  int DistortionMode;
  int FreezeUpdates;
//ETX

};

#endif



