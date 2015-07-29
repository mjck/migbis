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




#ifndef _VTKPXNONLINEARREGISTRATION_H
#define _VTKPXNONLINEARREGISTRATION_H

#include "vtkpxRegistration.h"
#include "vtkpxAbstractIntensityRegistration.h"
#include "vtkImageData.h"
#include "vtkpxGridTransform.h"
#include "vtkpxComboTransform.h"
#include "vtkDoubleArray.h"
#include "vtkGeneralTransform.h"

//BTX
class vtkpxNonLinearRegistrationOptimizationStructure {
public:
  double old_similarity;
  double step;
  int   level;
  double origin[3];
  double spacing[3];
  int   dimensions[3];
  double gridspacing[3];

  void SetValues(double old_similarity0,
		 double step0,
		 int level0,
		 double origin0[3],double spacing0[3],
		 int dimensions0[3],double gridspacing0[3])
  {
    this->old_similarity=old_similarity0;
    this->step=step0;
    this->level=level0;
    for (int ia=0;ia<=2;ia++)
      {
	this->origin[ia]=origin0[ia];
	this->spacing[ia]=spacing0[ia];
	this->dimensions[ia]=dimensions0[ia];
	this->gridspacing[ia]=gridspacing0[ia];
      }
  }
};
//ETX


class  vtkpxNonLinearRegistration : public vtkpxAbstractIntensityRegistration {

public:
  static vtkpxNonLinearRegistration* New();
  vtkTypeMacro(vtkpxNonLinearRegistration,vtkpxAbstractIntensityRegistration);

  // Description:
  // Print information about the progress of the registration
  void PrintInformation();


  // Description:
  // Set Grid Spacing at highest resolution
  vtkSetMacro(ControlPointSpacing,double);
  vtkGetMacro(ControlPointSpacing,double);

  // Description:
  // Set to 1 to produce diffeomorphic transformation using jacobian constraint
  vtkSetMacro(DoDiffeomorphic,int);
  vtkGetMacro(DoDiffeomorphic,int);

  // Desription:
  // Set Rate of increase of spacing with resolution level (std=2.0)
  vtkSetClampMacro(ControlPointSpacingRate,double,1.0,2.0);
  vtkGetMacro(ControlPointSpacingRate,double);

  // Run Optimization for Level level
  virtual int ComputeRegistration(int level);

  
  // Description:
  // Get Internal Transformation Stuff
  vtkGetObjectMacro(ComboTransform,vtkpxComboTransform);

  // Description:
  // Get/Set Initial Transform -- typically an affine estimate
  vtkSetObjectMacro(InitialTransform,vtkAbstractTransform);
  vtkGetObjectMacro(InitialTransform,vtkAbstractTransform);


  // Description:
  // Image Mask -- only allow warp in regions where image mask > 0.0
  vtkSetObjectMacro(ImageMask,vtkImageData);
  vtkGetObjectMacro(ImageMask,vtkImageData);

  // Description:
  // Enable control point displacements in X,Y and Z directions (defaults=1)
  vtkSetClampMacro(WarpX,int,0,1);
  vtkSetClampMacro(WarpY,int,0,1);
  vtkSetClampMacro(WarpZ,int,0,1);

  vtkGetMacro(WarpX,int);
  vtkGetMacro(WarpY,int);
  vtkGetMacro(WarpZ,int);

  vtkBooleanMacro(WarpX,int);
  vtkBooleanMacro(WarpY,int);
  vtkBooleanMacro(WarpZ,int);


  // Description:
  // Epsilon is the convergence limit (default=0.0001);
  vtkGetMacro(Epsilon,double);
  vtkSetClampMacro(Epsilon,double,0.0001,0.1);

  // Description:
  // WindowSize is the window size for updating the transformation (full=2.0)
  vtkGetMacro(WindowSize,double);
  vtkSetClampMacro(WindowSize,double,1.0,2.0);

  // Description:
  // WindowSize is the window size for updating the transformation (full=2.0)
  vtkGetMacro(GradientScale,double);
  vtkSetClampMacro(GradientScale,double,0.1,100.0);

  // Description:
  // Get Update Transformation
  virtual vtkAbstractTransform* GetUpdateTransform();


  // Description:
  // GetTransformation
  virtual vtkAbstractTransform* GetTransformation() { return this->ComboTransform;}


  // -----------------------------------------------------------------------------------------------
  // Smoothness Constraint 
  // -----------------------------------------------------------------------------------------------
  

  // Description:
  // Lambda is smoothness constant (default=0.0)
  vtkGetMacro(Lambda,double);
  vtkSetMacro(Lambda,double);

  // Description:
  // Prior Mode
  vtkSetClampMacro(InternalEnergyMode,int,0,1);
  vtkGetMacro(InternalEnergyMode,int);


  // Description:
  // Utility Functions for Setting the Smoothness Mode
  virtual void SetInternalEnergyModeToBendingEnergy()     { this->SetInternalEnergyMode(0);} // Default
  virtual void SetInternalEnergyModeToLinearElastic()     { this->SetInternalEnergyMode(1);}

  // Description:
  // Append Transformations or Fit to Single
  vtkSetClampMacro(AppendTransformations,int,0,1);
  vtkGetMacro(AppendTransformations,int);


protected:

  // Make all these protected 
  // Constructor
  vtkpxNonLinearRegistration();
  virtual ~vtkpxNonLinearRegistration();
  vtkpxNonLinearRegistration(const vtkpxNonLinearRegistration&) {};
  void operator=(const vtkpxNonLinearRegistration&) {};

  // Description:
  // Image Data to store last match (for removing bins from histogram)
  vtkImageData* LastTargetImage;
  vtkImageData* LastTargetWeightImage;

  // Description:
  // Image Mask is image which defines region where warping is allowed
  vtkImageData* ImageMask;

  // Description:
  // Control Point Status
  vtkDataArray* Status;

  // Description:
  // Spacing of control points and rate of increase of spacing with resolution level (std=2.0)
  double ControlPointSpacing;
  double ControlPointSpacingRate;
  int DoDiffeomorphic;
  
  // Description:
  // This controls the optimization method 0=default,1=Cong Gradient
  vtkpxNonLinearRegistrationOptimizationStructure OptimizationStructure;

  // Description:
  // Actual Transformation
  vtkpxGridTransform*   GridTransform;
  vtkpxComboTransform*  ComboTransform;
  vtkTransform*         LinearTransform;

  // Description:
  // Initial Transformation (to initialize Grid values)
  vtkAbstractTransform* InitialTransform;

  // Description:
  // Update Transform
  vtkpxComboTransform* UpdateTransform;

  // Description:
  // Flgas to Enable/Disable Translations along x,y,z axis (for distortion correction)
  int WarpX;
  int WarpY;
  int WarpZ;

  // Description:
  // Set Convergence Limit
  double Epsilon;

  // Description:
  // If one use "fluid like" registration
  int AppendTransformations;

  // Description:
  // Set Window Size (def=2.0);
  double WindowSize;

  // Description:
  // Scaling of Gradient before Calling Gradient Descent etc
  double GradientScale;
  

  // Description:
  // Lambda is smoothness constant (default=0.0)
  double Lambda;

  // Description:
  // InternalEnergyMode determines the type of smoothness constraint used
  // UsePrior determines whether the prior transform is used 
  int InternalEnergyMode;


  // Description:
  // Cached Grid Dimensions, NumberOfControlPoints etc.
  int NumberOfControlPoints;
  int ExperimentalMode;

  // Description:
  // Initial set up for the registration at all levels
  virtual int InitializeAll();

  // Description:
  // Optimize for step size = step
  virtual double Optimize(vtkDoubleArray* grad,double old_similarity,double step,int level,
			 double origin[3],double spacing[3],int dimensions[3],
			 double gridspacing[3]);

  // Description:
  // Evaluates the registration. This function evaluates the registration by
  // looping over the target image and interpolating the transformed source 
  // image while filling the joint histogram. This function returns the value
  // of the similarity measure using Similarity().
  virtual double Evaluate();

  // Description:
  // Evaluates the registration. This function evaluates the registration by
  // looping over the target image and interpolating the transformed source 
  // image while filling the joint histogram. This function returns the value
  // of the similarity measure using Similarity().
  virtual double EvaluatePiece(vtkImageData* reference,
			       vtkImageData* target,
			       vtkImageData* previoustarget,
			       vtkImageData* weight=NULL,
			       vtkImageData* weights2=NULL,
			       int cpointno=-1);
  
  //BTX
  // Description:
  // Fast access to the parameters of the grid
  //ETX

  // Description:
  // Initial set up for the registration at a multiresolution level
  virtual int Initialize(int);

  // Description:
  // Called from Initialize to Initialize the Transformation
  virtual int InitializeTransformation(int);

  // Description:
  // Final set up for the registration at a multiresolution level
  virtual int FinalizeAll();


  // Description:
  // Generate Output
  virtual void GenerateOutput(int level, int step,int iter,double stepsize,double simil);


  // Description:
  // Needed for Optimizer
  virtual void SetCurrentParameters(vtkDoubleArray* par);
  virtual void GetCurrentParameters(vtkDoubleArray* par);
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerGradientGlobal(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);

  // Description:
  // SmoothnessMeasure
  virtual double ComputeInternalEnergy(vtkpxGridTransform* tr,int cp=-1);


  // Description:
  virtual void    ApproximateTransformationToGrid(vtkpxGridTransform* grid,vtkAbstractTransform* tr,double step,
						  int numiter,int numlevels,
						  double threshold,double lambda);

  virtual void ApproximateTransformationToGrid(vtkpxGridTransform* grid,vtkAbstractTransform* tr);
};


#endif

