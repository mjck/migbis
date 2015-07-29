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


#ifndef _VTKPXRIGIDREGISTRATION_H
#define _VTKPXRIGIDREGISTRATION_H

#include "vtkpxAbstractIntensityRegistration.h"
#include "vtkpxLinearTransform.h"
#include "vtkTransform.h"
#include "vtkShortArray.h"

//BTX
class vtkpxLinearRegistrationOptimizationStructure {
public:
  double old_similarity;
  double step;
  int   maxdofs;
  int   level;
  int   st;

  void SetValues(int maxdofs0,double step0,
		 double old_similarity0,int level0,int st0)
  {
    this->old_similarity=old_similarity0;
    this->step=step0;
    this->maxdofs=maxdofs0;
    this->level=level0;
    this->st=st0;
  }
};
//ETX

class vtkpxLinearRegistration : public vtkpxAbstractIntensityRegistration {

public:
  static vtkpxLinearRegistration* New();
  vtkTypeMacro(vtkpxLinearRegistration,vtkpxAbstractIntensityRegistration);

  // Description:
  // Print information about the progress of the registration
  void PrintInformation();

  // Description:
  // Run Optimization for Level level
  virtual int ComputeRegistration(int level);

  // Description:
  // Internal Optimization
  virtual double Optimize(int maxdofs,double step,double old_similarity,int level,int st);
  virtual double OldOptimize(int maxdofs,double step,double old_similarity,int level,int st);


  // Description:
  // Auto Center Origin
  vtkSetClampMacro(AutoCenterOrigin,int,0,1);
  vtkGetMacro(AutoCenterOrigin,int);
  vtkBooleanMacro(AutoCenterOrigin,int);


  // Description:
  // Auto Center Origin
  vtkSetClampMacro(OptimizationAutoScale,int,0,1);
  vtkGetMacro(OptimizationAutoScale,int);
  vtkBooleanMacro(OptimizationAutoScale,int);

  // Description:
  // Auto Center Origin
  vtkSetMacro(OptimizationScaleFactor,double);
  vtkGetMacro(OptimizationScaleFactor,double);



  // Description:
  // Set Different Modes
  virtual void SetTransformModeToRigid();
  virtual void SetTransformModeToSimilarity();
  virtual void SetTransformModeToAffine();
  virtual void SetTransformModeToRigid2D();
  virtual void SetTransformModeToSimilarity2D();
  virtual void SetTransformModeToAffine2D();
  virtual void SetTransformModeToDistortion();

  // Description:
  // Distortion Mode;
  vtkSetClampMacro(DistortionMode,int,0,1);
  vtkGetMacro(DistortionMode,int);
  vtkBooleanMacro(DistortionMode,int);
  


  // Description:
  // Set Active/Passive Degrees of Freedom
  vtkSetClampMacro(TranslationX,int,0,1);
  vtkSetClampMacro(TranslationY,int,0,1);
  vtkSetClampMacro(TranslationZ,int,0,1);
  vtkSetClampMacro(RotationX,int,0,1);
  vtkSetClampMacro(RotationY,int,0,1);
  vtkSetClampMacro(RotationZ,int,0,1);
  vtkSetClampMacro(ScalingX,int,0,1);
  vtkSetClampMacro(ScalingY,int,0,1);
  vtkSetClampMacro(ScalingZ,int,0,1);
  vtkSetClampMacro(ShearingXY,int,0,1);
  vtkSetClampMacro(ShearingYX,int,0,1);
  vtkSetClampMacro(ShearingYZ,int,0,1);
  vtkSetClampMacro(ShearingZY,int,0,1);
  vtkSetClampMacro(ShearingZX,int,0,1);
  vtkSetClampMacro(ShearingXZ,int,0,1);

  vtkGetMacro(TranslationX,int);
  vtkGetMacro(TranslationY,int);
  vtkGetMacro(TranslationZ,int);
  vtkGetMacro(RotationX,int);
  vtkGetMacro(RotationY,int);
  vtkGetMacro(RotationZ,int);
  vtkGetMacro(ScalingX,int);
  vtkGetMacro(ScalingY,int);
  vtkGetMacro(ScalingZ,int);
  vtkGetMacro(ShearingXY,int);
  vtkGetMacro(ShearingYX,int);
  vtkGetMacro(ShearingYZ,int);
  vtkGetMacro(ShearingZY,int);
  vtkGetMacro(ShearingZX,int);
  vtkGetMacro(ShearingXZ,int);

  vtkBooleanMacro(TranslationX,int);
  vtkBooleanMacro(TranslationY,int);
  vtkBooleanMacro(TranslationZ,int);
  vtkBooleanMacro(RotationX,int);
  vtkBooleanMacro(RotationY,int);
  vtkBooleanMacro(RotationZ,int);
  vtkBooleanMacro(ScalingX,int);
  vtkBooleanMacro(ScalingY,int);
  vtkBooleanMacro(ScalingZ,int);
  vtkBooleanMacro(ShearingXY,int);
  vtkBooleanMacro(ShearingYX,int);
  vtkBooleanMacro(ShearingYZ,int);
  vtkBooleanMacro(ShearingZY,int);
  vtkBooleanMacro(ShearingZX,int);
  vtkBooleanMacro(ShearingXZ,int);


  // Description:
  // Return Internal Transform Structure
  vtkGetObjectMacro(LinearTransform,vtkpxLinearTransform);

  // Description:
  // Return Update Transformation
  virtual vtkAbstractTransform* GetUpdateTransform() { return this->UpdateTransform; } 

  // Description:
  // Get Transformation
  virtual vtkAbstractTransform* GetTransformation();

  // Description:
  // Initial Transform
  vtkGetObjectMacro(InitialTransform,vtkpxLinearTransform);
  vtkSetObjectMacro(InitialTransform,vtkpxLinearTransform);


  // Description:
  // Dof Status
  virtual int   GetDofStatus(int dof);
  virtual void  UpdateDofLabels();


protected:
  // Make all these protected 
  // Constructor
  vtkpxLinearRegistration();
  virtual ~vtkpxLinearRegistration();
  vtkpxLinearRegistration(const vtkpxLinearRegistration&) {};
  void operator=(const vtkpxLinearRegistration&) {};

  // Description:
  // Initial set up for the registration at all levels
  virtual int InitializeAll();

  // Description:
  // Initial set up for the registration at a multiresolution level
  virtual int Initialize(int Level);

  // Description:
  // Final set up for the registration at all levels
  virtual int FinalizeAll();

  // Description:
  // Evaluates the registration. This function evaluates the registration by
  // looping over the target image and interpolating the transformed source 
  // image while filling the joint histogram. This function returns the value
  // of the similarity measure using Similarity().
  virtual double Evaluate();


  // Description:
  // Pointer To  Transform Used for Evaluation
  vtkpxLinearTransform* LinearTransform;
  vtkTransform*     UpdateTransform;

  // Description:
  // Auto Center Origin
  int AutoCenterOrigin;

  // Description:
  // Optimization Auto Scale -- what to scale the non-translation parameters
  // relative to translation if OptimizationAutoScale=0 default (meanresolution) is used
  // if 1 value in OptimizationScaleFactor is used
  int OptimizationAutoScale;
  double OptimizationScaleFactor;
  

  // Description:
  // Initial Transform
  vtkpxLinearTransform*     InitialTransform;

  // Description:
  // Genereate Output
  virtual void GenerateUpdateTransform();
  virtual void GenerateOutput(int level, int step,int iter,double stepsize,double simil,double last_sim,double last_sm);

  // Description:
  // Transform Output Mode
  int TransformOutputMode;

  // Description:
  // Set Distortion Mode on Linear Xform
  int DistortionMode;


  // Description:
  // Optimizer Interface
  virtual void SetCurrentParameters(vtkDoubleArray* par);
  virtual void GetCurrentParameters(vtkDoubleArray* par);
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);

  // Description:
  // This controls the optimization method 0=default,1=Cong Gradient
  vtkpxLinearRegistrationOptimizationStructure OptimizationStructure;


  // Description:
  // Enable/Disable DOF
  vtkShortArray* DofStatus;


protected:

//BTX
  int TranslationX, TranslationY, TranslationZ;
  int RotationX, RotationZ, RotationY;
  int ScalingX, ScalingY, ScalingZ;
  int ShearingXY, ShearingYX, ShearingYZ, ShearingZY, ShearingZX, ShearingXZ;
  double DofWeight[15];

//ETX



};


#endif

