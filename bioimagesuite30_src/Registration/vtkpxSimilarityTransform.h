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





// .NAME vtkpxSimilarityTransform - describes linear transformations via a 4x4 matrix
// .SECTION Description
// This generates a similarity/rigid transform based on
// (a) center of rotation (3 params)
// (b) scaling parameter (1 param)
// (c) roation angles (3 params)
// (d) translation (3 params)
// (e) additional transformation (vtkMatrixToLinearTransform)
//
// 

#ifndef __vtkpxSimilarityTransform_h
#define __vtkpxSimilarityTransform_h

#include "vtkMatrixToLinearTransform.h"
#include "vtkMatrix4x4.h"
#include "vtkDoubleArray.h"
#include "vtkLinearTransform.h"
#include "vtkpxMatrix.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "pxutil.h"


class vtkpxSimilarityTransform : public vtkMatrixToLinearTransform
{
 public:
  static vtkpxSimilarityTransform *New();
  vtkTypeMacro(vtkpxSimilarityTransform,vtkMatrixToLinearTransform);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the parameters from array
  // This is key for Optimization Classes 
  void SetParameters(vtkDoubleArray *parameters,int InputOffset=0);
  void GetParameters(vtkDoubleArray *parameters,int InputOffset=0);
  
  // Description:  
  // Mode determines how Set/Get Parameters Operate, all other elements are active but fixed
  // i.e. fixed scale DOES not mean no scaling
  virtual  void SetModeToRotationXYOnly()  { this->SetMode(0); }
  virtual  void SetModeToRotationOnly()  { this->SetMode(1); }
  virtual  void SetModeToRotationScale() { this->SetMode(2); }
  virtual  void SetModeToRotationTranslation() { this->SetMode(3); }
  virtual  void SetModeToRotationScaleTranslation() { this->SetMode(4); }
  virtual  void SetModeToRotationOrigin() { this->SetMode(5); }
  virtual  void SetModeToRotationScaleOrigin() { this->SetMode(6); }
  virtual  void SetModeToRotationTranlationOrigin() { this->SetMode(7); }
  virtual  void SetModeToAll() { this->SetMode(8); }
  virtual  void SetModeToZRotationOnly() { this->SetMode(9); }
  virtual  void SetModeToFixedParameters() { this->SetMode(-1); }
  vtkGetMacro(Mode,int);
  virtual void SetMode(int m);

  // Description:
  // Puts a transformation parameter (transformation matrix is updated)
  void   Put(int, float);
  void   Set(int a, float v) {  Put(a,v);}
  /// Gets a transformation parameter
  float Get(int) const;

  // Description:
  // Stop Auto Updates
  vtkSetClampMacro(FreezeUpdates,int,0,1);
  vtkGetMacro(FreezeUpdates,int);

  // Description:
  // Load / Save Transformation
  virtual int Load(const char* fname);
  virtual int Load(gzFile fin);

  virtual int Save(const char* fname);
  virtual int Save(FILE* fout);

  // Description:
  // Set To Identity
  virtual void Identity();


  // Description:
  // Number Of Parameters
  virtual int GetNumberOfParameters();

  // Description:
  // Set Initialize
  virtual void Initialize();

  // Description:
  // Copy
  virtual void Copy(vtkpxSimilarityTransform* other);
  virtual void CopyParameters(vtkpxSimilarityTransform* other);

  // Description:
  // Point Fitting
  void ApproximatePoints(vtkPoints* SourceLandmarks,vtkPoints* TargetLandmarks,vtkpxMatrix* Weights);
		
  // Description:
  // Forces Parameter Updates
  virtual void UpdateMatrix();


  // Description:
  // BlendedCylinder
  // mode=0 no-blend, 1=symmetric blend, 2=causal blend
  static vtkPolyData* CreateBlendedCylinder(vtkPolyData* input,float angle,int mode);

  // Description:
  // Z-Axis : can 
  vtkGetVectorMacro(LocalZAxis,float,3);
  vtkGetVectorMacro(LocalXAxis,float,3);
  virtual void SetLocalXZAxis(float x[3],float z[3]);

  // Description:
  // Access Internal Numbers
  vtkGetVectorMacro(Origin,float,3);
  vtkGetVectorMacro(RotationAxis,float,3);
  vtkGetMacro(RotationAngle,float);


  // Description:
  // Translation Scale == size of voxel in mm
  vtkGetMacro(TranslationScale,float);
  vtkSetClampMacro(TranslationScale,float,0.001,10.0);
  
protected:
  vtkpxSimilarityTransform ();
  virtual ~vtkpxSimilarityTransform ();

//BTX
  /// Scaling along the x-axis, y-axis, z-axis
  float ScaleFactor;
  float Origin[3];
  float Angle[3];
  float Translation[3];
  float LocalZAxis[3];
  float LocalXAxis[3];
  int   FreezeUpdates;
  int   Mode;
  float TranslationScale;
  vtkMatrixToLinearTransform* LocalAxisTransform;

  // Keep this also
  // --------------
  float RotationAngle;
  float RotationAxis[3];

//ETX

};

#endif



