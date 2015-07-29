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





#ifndef __vtkpxGridTransform_h
#define __vtkpxGridTransform_h

class vtkProbeFilter;
class vtkPolyData;
class vtkDataArray;
class vtkpxMatrix;
class vtkLinearTransform;
class vtkDoubleArray;

#include "vtkPoints.h"
#include "vtkCollection.h"
#include "vtkpxBaseGridTransform.h"

//BTX
class vtkpxGridTransform;
class vtkpxGridTransformOptimizationStructure {
public:
  vtkCollection* ControlsCollection;
  vtkPoints* points;
  vtkPoints* points2;
  double step;
  double lambda;
  double bend;
  double error;
  vtkpxMatrix* weights;
  vtkpxGridTransform* priorTransform;
  vtkDoubleArray*    priorWeights;

  vtkpxGridTransformOptimizationStructure();
  void SetValues(vtkCollection* ControlsCollection0,
		 vtkPoints* points0,
		 vtkPoints* points2_0,
		 double step0,
		 double lambda0,
		 vtkpxMatrix* weights0,
		 vtkpxGridTransform* prior0=NULL,
		 vtkDoubleArray* priorweights0=NULL);
};

//ETX


class vtkpxGridTransform : public vtkpxBaseGridTransform
{
public:
  static vtkpxGridTransform *New();
  vtkTypeMacro(vtkpxGridTransform,vtkpxBaseGridTransform);

  // Description:
  // Initialize Displacement Grid Given Image
  virtual void InitializeDisplacementGrid(vtkImageData *image, double spacing,double offset,vtkAbstractTransform* tr=NULL);
  
  
  // Description:
  // Set Displacement Grid -- used to create cached structures
  virtual void SetDisplacementGrid(vtkImageData *grid);


  // -------------------------------------------------------------------
  // Symmetric RPM Stuff
  // -------------------------------------------------------------------

  // Description:
  // Initialize the Displacement Grid to be the same as the other
  virtual void InitializeDisplacementGrid(vtkpxGridTransform* other);
  virtual void InitializeDisplacementGrid(double bounds[6],double dx, double dy, double dz);
  virtual void InverseApproximateTransformation(vtkpxGridTransform* output,int initialize=0);


  // -----------------------------------------------------------------------------
  // Utility Functions to Set Invididual Control Points
  // -----------------------------------------------------------------------------

  // Description:
  // Access to Internal Structures for Information
  vtkGetVector3Macro(GridDimensions,int);
  vtkGetVector3Macro(GridSpacing,double);
  vtkGetVector3Macro(GridOrigin,double);


  // Description:
  // NumberOfControlPoints
  vtkGetMacro(NumberOfControlPoints,int);


  // Description:
  // NumberOfControlPoints
  vtkGetMacro(ApproximationMode,int);
  vtkSetMacro(ApproximationMode,int);

  // Description:
  // Get Index From Indices
  virtual int GetControlPointIndex(int i,int j,int k);

  // Description:
  // Shift Control Point and get control point location stuff
  virtual void  GetControlPointLocation(int index, double p[3]);
  virtual double GetControlPointLocation(int index, int comp);


  // Description:
  // Modify individual displacements
  virtual double GetControlPointDisplacement(int index,int comp);
  virtual void  GetControlPointDisplacement(int index,double val[3]);
  virtual void  SetControlPointDisplacement(int index,int comp,double val);
  virtual void  SetControlPointDisplacement(int index,double val[3]);

  // Description:
  // Shift Control Point
  virtual void  ShiftControlPoint(int index,int comp,double val);
  virtual void  ShiftControlPoint(int index,double val[3]);


  // Description:
  // Compute Derivatives at a point, returns determinat (Jacobian)
  virtual double ComputeDerivativeMatrix(double x1,double x2,double x3,vtkpxMatrix* matr);
  virtual double ComputeDerivativeMatrix(int index,vtkpxMatrix* matr);
  virtual double ComputeDerivativeMatrix(int index,double der[3][3]);
  virtual double ComputeDerivativeMatrix(double x[3],double der[3][3]);


  virtual double ComputeDerivative(int index,int comp1,int comp2);
  virtual double ComputeDerivative(double x1,double x2,double x3,int comp1,int comp2);
  virtual double ComputeDerivative(double x[3],int comp1,int comp2);

  // ----------------------------------------------------------------------
  // General Registration Stuff
  // ----------------------------------------------------------------------

  // Description:
  // Initialize Transformation using an arbitrary input transform
  virtual int   ApproximateTransformation(vtkAbstractTransform* tr);
  virtual int   ApproximateTransformation(vtkAbstractTransform* tr,double step,
					  int numiter,int numlevels,
					  double threshold,double lambda);
  virtual int   InitialApproximateTransformation(vtkAbstractTransform* tr);



  // Description:
  // Approximation Helper Routines
  virtual void   GenerateTestPoints(vtkPoints* points,double inc=1.0);
  virtual void   FillMatrices(vtkAbstractTransform* tr,vtkpxMatrix* A,vtkpxMatrix* B);
  virtual double  TestApproximation(vtkAbstractTransform* tr,vtkPoints* points);
  virtual double  TestInverseApproximation(vtkAbstractTransform* tr,vtkPoints* points);
  virtual double  TestApproximation(vtkPoints* points,vtkPoints* points2);
  virtual double  TestWeightedApproximation(vtkPoints* points,vtkPoints* points2,vtkpxMatrix* lsqweight,double& sum2error);

  // Description:
  // Test of B-Spline
  virtual double TestTransformPoint(double p1[3],double p2[3]);
  virtual double TestTransformPoint(double x,double y,double z);

  // Description:
  // Compute Bending Energy (this is the one used internally)
  virtual double ComputeTotalBendingEnergy();
  virtual double ComputeBendingEnergyAroundControlPoint(int cpoint);
  virtual double ComputeBendingEnergyAtControlPoint(int cpoint);
  virtual double ComputeBendingEnergyAtControlPoint(int i,int j,int k);

  // Description:
  // Compute Strain Energy (Linear Elasticity)
  virtual double ComputeTotalStrainEnergy(double v=0.45);
  virtual double ComputeStrainEnergyAtControlPoint(int cpoint,double v=0.45);
  virtual double ComputeStrainEnergyAtPoint(double p1[3],double v=0.45);

  // Description:
  // CombineWithLinear Transform
  virtual void CombineWithLinearTransform(vtkLinearTransform* tr,int premultiply=1);

  // Description:
  // Invert the transformation.  This is done by point fitting
  void Inverse();

  // -----------------------------------------------------------------------------
  // Description
  // The Following Routines are need to support point matching stuff
  // -----------------------------------------------------------------------------

  // Description:
  // Form Weight Matrix 
  virtual void FormWeightMatrix(vtkPoints* points,vtkpxMatrix* W);

  // Description:
  // Form Weight Structures
  virtual int  FormCollectionStructure(vtkPoints* points,vtkCollection* ControlsCollection,double thr);
  virtual int  FormCollectionStructure(vtkPoints* points,vtkCollection* ControlsCollection);
  virtual void ApproximatePoints(vtkPoints* points,vtkPoints* points2,
				 vtkCollection* ControlsCollection,
				 double step,
				 int numiter,int numlevels,double threshold,
				 double lambda,int initialize,vtkpxMatrix* controlweights=NULL,
				 vtkpxGridTransform* priorTransform=NULL,
				 vtkDoubleArray* priorWeights=NULL);

  virtual double ErrorAtControlPoint(int cp,vtkPoints* points,vtkPoints* points2,
				    vtkDoubleArray* indices,double lambda,
				    vtkpxMatrix* lsqweight=NULL,
				    vtkpxGridTransform* priorTransform=NULL,
				    vtkDoubleArray* priorWeights=NULL);




  // Description:
  // Compute Weighted Distance
  // -------------------------
  virtual double ComputeWeightedDistance(vtkpxGridTransform* other,int cp,vtkDoubleArray* weights=NULL);
  virtual int   FormOutputWeights(vtkCollection* collection,vtkpxMatrix* PointWeights,vtkDoubleArray* OutputWeights);
  // Description:
  // Approximate Points with Prior 
  /*  virtual void ApproximatePoints(vtkPoints* points,vtkPoints* points2,
				 vtkCollection* ControlsCollection,
				 double step,
				 int numiter,int numlevels,double threshold,
				 double lambda,int initialize,vtkpxMatrix* controlweights=NULL,
				 vtkpxGridTransform* priorTransform,vtkDoubleArray* priorweights);*/

  // Description:
  // Shift Control Points by step*values in grad
  virtual void  ShiftControlPoints(vtkDoubleArray* grad,double step);
  virtual void  ScaleGradients(vtkDoubleArray* grad,double scale);
  // -----------------------------------------------------------------------------

  // Description:
  // Stuff Needed for Using vtkpxOptimizer to improve approximation of new points
  // -----------------------------------------------------------------------------
  static void   OptimizerFeedback(vtkObject* self,const char* line,double val);
  static double  CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output);
  static double  CalculateFunction(vtkObject* self,vtkDoubleArray* position);

  virtual void  SetCurrentParameters(vtkDoubleArray* par);
  virtual void  GetCurrentParameters(vtkDoubleArray* par);
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);

  // Description:
  // Smoothing of Collections to yield new transformations
  // -----------------------------------------------------------------------------
  static int SmoothCollection(vtkCollection* TransformationCollection,int periodic=0,double sigma=1.0);

  virtual double ComputeJacobianConstraint(float gamma);
  //BTX
  // Description:
  // Pointer to Control Point
  virtual double* GetControlPointPointer(int index);
  virtual void   GetControlPointIndices(int index,int ind[3]);
  //ETX

protected:
  vtkpxGridTransform();
  virtual ~vtkpxGridTransform();
  vtkpxGridTransform(const vtkpxGridTransform&) {};
  void operator=(const vtkpxGridTransform&) {};



  // Description:
  // Get the inverse flag of the transformation.  This flag is
  // set to zero when the transformation is first created, and
  // is flipped each time Inverse() is called.
  // Protected so it is not accesible from outside 
  vtkGetMacro(InverseFlag,int);

  
  // Description:
  // Cached Grid Dimensions, NumberOfControlPoints etc.
  int    GridDimensions[3];
  int   NumberOfControlPoints;
  int   GridSliceDimensions;
  double GridSpacing[3];
  double GridOrigin[3];
  vtkDoubleArray* Displacements;
  int   ApproximationMode;
  
  //BTX
  // Description:
  // Get Lattice And Arclength Coordinates
  virtual void GetCoordinatesInGrid(double point[3],int lattice[3],double s[3]);

  vtkpxGridTransformOptimizationStructure OptimizationStructure;
  //ETX

public:
  // --------------------------------------------------------------------------------------------------
  // Description:
  // Approximation Stuff
  virtual double GradientAtControlPoint(int cp,double grad[3],
				       vtkPoints* points,vtkPoints* points2,vtkDoubleArray* indices,
				       double step,double lambda=0.0,vtkpxMatrix* lsqweights=NULL,
				       vtkpxGridTransform* priorTransform=NULL,
				       vtkDoubleArray* priorWeights=NULL);
  
protected:
  // Description:
  // Helper Routines for Approximate Displacements
  // Shift Control Points by step * grad
  virtual void  InitializeApproximatePoints(vtkPoints* points,vtkPoints* points2,vtkpxMatrix* lsqweights=NULL);

};



#endif

