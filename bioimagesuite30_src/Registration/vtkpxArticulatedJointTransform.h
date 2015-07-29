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





// .NAME vtkpxArticulatedJointTransform - describes linear transformations via a 4x4 matrix
// .SECTION Description
// This generates a similarity/rigid transform based on
// (a) center of rotation (3 params)
// (b) scaling parameter (1 param)
// (c) roation angles (3 params)
// (d) translation (3 params)
// (e) additional transformation (vtkMatrixToLinearTransform)
//
// 

#ifndef __vtkpxArticulatedJointTransform_h
#define __vtkpxArticulatedJointTransform_h

#include "vtkpxSimilarityTransform.h"
#include "vtkImageData.h"
#include "vtkTransform.h"
#include "vtkAbstractTransform.h"

class vtkpxArticulatedJointTransform : public vtkAbstractTransform
{
 public:
  static vtkpxArticulatedJointTransform *New();
  vtkTypeMacro(vtkpxArticulatedJointTransform,vtkAbstractTransform);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the parameters from array
  // This is key for Optimization Classes 
  void SetParameters(vtkDoubleArray *parameters,int InputOffset=0);
  // { this->SimilarityTransform->SetParameters(parameters,InputOffset);}
  void GetParameters(vtkDoubleArray *parameters,int InputOffset=0);
  // { this->SimilarityTransform->GetParameters(parameters,InputOffset);}

  // Description:
  // Detail Mode , lots of fprintf statements
  vtkGetMacro(DetailMode,int);
  vtkSetClampMacro(DetailMode,int,0,1);

  // Description:
  // Smooth Rotation Angle across r,z
  vtkGetMacro(SmoothingMode,int);
  vtkSetClampMacro(SmoothingMode,int,0,1);

  // Description:
  // Smooth Rotation Angle across r,z
  vtkGetMacro(SmoothingRadius,float);
  vtkSetClampMacro(SmoothingRadius,float,0.1,15.0);

  // Description:
  // Smooth Rotation Angle across r,z
  vtkGetMacro(ZBlendingSize,float);
  vtkSetClampMacro(ZBlendingSize,float,0.1,40.0);

  // Description:  
  // 0 = No Blending
  // 1 = Avoid Folding
  // 2 = Smooth Wake + Avoid Folding
  vtkGetMacro(BlendedMode,int);
  vtkSetClampMacro(BlendedMode,int,0,2);

  // Description:  
  // SeparateZRotation
  vtkGetMacro(SeparateZRotation,int);
  vtkSetClampMacro(SeparateZRotation,int,0,1);



  // Description:  
  // 0 = No Blending
  // 1 = Avoid Folding
  // 2 = Smooth Wake + Avoid Folding
  vtkGetMacro(BlendingWindow,float);
  vtkSetClampMacro(BlendingWindow,float,0.05,3.0);


  // Description:
  // Cubic Blending fits cubic polynomial for $C^1$ continuity, otherwise linear
  vtkGetMacro(CubicBlending,int);
  vtkSetClampMacro(CubicBlending,int,0,1);


  // Description:
  // Active = 0 is fixed parameters, 1 = enabled
  vtkGetMacro(Active,int);
  virtual void SetActive(int active);


  // Description:
  // Fast Interface Search
  vtkGetMacro(MinimumSectorAngle,float);
  vtkSetClampMacro(MinimumSectorAngle,float,0.0,180.0);

  // Description:
  // RegionLabelImage
  vtkGetObjectMacro(RegionLabelImage,vtkImageData);
  virtual void ExternalSetRegionLabelImage(vtkImageData* img);



  // Description:
  // Index in Region
  vtkGetMacro(RegionLabelIndex,int);
  vtkSetMacro(RegionLabelIndex,int);

  // Description:
  // Child Index in Region
  // If ChildRegionLabelIndex == 1000 then all regions > RegionLabelIndex are children by default, in this case ChildRegionLabelIndex2 is ignored!
  vtkGetMacro(ChildRegionLabelIndex,int);
  vtkSetMacro(ChildRegionLabelIndex,int);

  // Description:
  // Index in Region
  vtkGetMacro(ChildRegionLabelIndex2,int);
  vtkSetMacro(ChildRegionLabelIndex2,int);
 
  // Descrpition:
  // Get Similarity Transform
  vtkGetObjectMacro(SimilarityTransform,vtkpxSimilarityTransform);
  vtkGetObjectMacro(ZSimilarityTransform,vtkpxSimilarityTransform);

  // Description:
  // Copy
  virtual void Copy(vtkpxArticulatedJointTransform* other);


  // Description:
  // Limit Modes to only applicable ones (Nothing, 
  
  virtual  void SetModeToRotationXYOnly()  { this->SetMode(0); }
  virtual  void SetModeToRotationOnly()  { this->SetMode(1); }
  virtual  void SetModeToRotationScale() { this->SetMode(2); }
  virtual  void SetModeToRotationTranslation() { this->SetMode(3); }
  virtual  void SetModeToRotationScaleTranslation() { this->SetMode(4); }
  virtual  void SetModeToRotationOrigin() { this->SetMode(5); }
  virtual  void SetModeToFixedParameters() { this->SetMode(-1); }
  virtual  void SetMode(int m);
  virtual  int  GetMode();
  //{ return this->SimilarityTransform->GetMode(); }



  // Description:
  // Puts a transformation parameter (transformation matrix is updated)
  void   Put(int a, float v);
  void   Set(int a, float v);
  // Gets a transformation parameter
  float Get(int a);


  // Description:
  // Stop Auto Updates
  virtual void SetFreezeUpdates(int a);
  virtual int  GetFreezeUpdates();


  // Description:
  // Load / Save Transformation
  virtual int Load(const char* fname);
  virtual int Load(gzFile fin);

  virtual int Save(const char* fname);
  virtual int Save(FILE* fout);

  // Description:
  // Number Of Parameters
  virtual int GetNumberOfParameters();

  // Description:
  // Set/Get Alignment Transform
  vtkGetObjectMacro(AlignmentTransform,vtkAbstractTransform);
  vtkSetObjectMacro(AlignmentTransform,vtkAbstractTransform);

  // Description:
  // Index in Region
  vtkGetMacro(AlignmentTransformIndex,int);
  vtkSetMacro(AlignmentTransformIndex,int);

  // Description:
  // Internal Transform Point -- OK
  void InternalTransformPoint(const float in[3], float out[3]);
  void InternalTransformPoint(const double in[3], double out[3]);

  // Description:
  // Internal Transform Derivative (Not done yet) -- OK
  void InternalTransformDerivative(const float in[3], float out[3],
				  float derivative[3][3]);
  void InternalTransformDerivative(const double in[3], double out[3],
				  double derivative[3][3]);


  // Description:
  // Inverse
  virtual void Inverse() { vtkErrorMacro(<<"Not Implemented\n");}
  
  // Description:
  // Make another transform of the same type.
  vtkAbstractTransform *MakeTransform();

  // Description:
  // Compute All Angles 
  vtkImageData* ComputeAllAngles();
  
  // Description:
  // Adjust Origin to be outside 
  virtual void AdjustOriginToBeOutside(double gap);

  // Description:
  // Set Origin and LocalZAxis via cross-product of three co-planar points
  virtual void SetOriginLocalAxis(double ori[3],double p1[3],double p2[3]);


  // Description:
  // Local Z Axis

  virtual void SetLocalXZAxis(float x[3],float z[3]);
  virtual float* GetOrigin() { return this->SimilarityTransform->GetOrigin(); }
  virtual void GetOrigin(float x[3]) {  this->SimilarityTransform->GetOrigin(x); }
  virtual float* GetLocalXAxis() { return this->SimilarityTransform->GetLocalXAxis(); }
  virtual float* GetLocalZAxis() { return this->SimilarityTransform->GetLocalZAxis(); }
  virtual void GetLocalZAxis(float z[3]) {  this->SimilarityTransform->GetLocalZAxis(z); }
  virtual void GetLocalXAxis(float x[3]) {  this->SimilarityTransform->GetLocalXAxis(x); }


  // Description:
  // Initialize Angles from Points
  virtual int InitializeAngles(double source[3],double target[3]);


  // Description:
  // Identity
  virtual void Identity();

  // Description:
  // Specify name of the joint
  vtkSetStringMacro(JointName);
  vtkGetStringMacro(JointName);



protected:
  vtkpxArticulatedJointTransform ();
  virtual ~vtkpxArticulatedJointTransform ();

 

//BTX
  vtkImageData* RegionLabelImage;
  int           RegionLabelIndex;
  int           ChildRegionLabelIndex;
  int           ChildRegionLabelIndex2;
  int           BlendedMode;
  int           CubicBlending;
  float         MinimumSectorAngle;
  int           DetailMode;
  float         BlendingWindow;
  float         ZBlendingSize;
  int           SmoothingMode;
  int           SeparateZRotation;
  int           Active;
  int           OldSimilarityMode;
  int           OldZSimilarityMode;
  int           OldBlendedMode;
  int           OldSeparateZRotation;
  float         SmoothingRadius;
  char          *JointName;
  vtkTransform* AngleAxisTransform;
  vtkpxSimilarityTransform* SimilarityTransform;
  vtkpxSimilarityTransform* ZSimilarityTransform;
  vtkAbstractTransform* AlignmentTransform;
  int           AlignmentTransformIndex;
//ETX
  
  // Description:
  // Return -1 if no region label image (re-route to vtkpxSimilarityTransform) or no Blending
  // Return  0 if in static region      (do nothing)
  // Return  1 if in moving region      (do something)
  virtual int    IsInsideRegion(const double x[3]);
  virtual void   CreateAngleAxisTransformation(double theta,vtkpxSimilarityTransform* trans);

  virtual double SearchForInterface(const double x[3],double maxtheta,double stepsize,vtkpxSimilarityTransform* trans);
  virtual double GetActualRotationAngle(const double x[3],vtkpxSimilarityTransform* trans);
  virtual double GetSmoothRotationAngle(const double x[3],vtkpxSimilarityTransform* trans);

  virtual double AdjustForZBlending(const double point[3],vtkpxSimilarityTransform* trans,float max);

  // Make This Protected
  // -------------------
  vtkSetObjectMacro(RegionLabelImage,vtkImageData);

};

#endif



