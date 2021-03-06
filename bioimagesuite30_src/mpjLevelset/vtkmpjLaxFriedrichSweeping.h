

//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
//BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


/* 
   -- vtkmpjLaxFriedrichSweeping.h --
   
   Implements Stanley Osher's Fast Sweeping Scheme for solving
   Static Hamilton-Jacobi Equations
   
   Marcel Jackowski, Feb. 2004.
   
*/

#define VTK_MPJ_KNOWN   100   
#define VTK_MPJ_UNKNOWN   0   

#ifndef __vtkmpjLaxFriedrichSweeping_h
#define __vtkmpjLaxFriedrichSweeping_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageAccumulate.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjLaxFriedrichSweeping : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjLaxFriedrichSweeping *New();
  vtkTypeMacro(vtkmpjLaxFriedrichSweeping,vtkpxSimpleImageToImageFilter);
  
  // Description:
  // Set/Get boundary conditions for the propagation. You can also
  // set the boundary points in the label image. See below.
  vtkSetObjectMacro(KnownPoints, vtkFloatArray);
  vtkGetObjectMacro(KnownPoints, vtkFloatArray);
  
  // Description:
  // Get/Set total number of Iterations
  vtkGetMacro(NumberOfIterations,int);
  vtkSetMacro(NumberOfIterations,int);
  
  // Description:
  // Set/Get the label image. If KnownPoints is not set, then
  // LabelImage must have values >0 for initialized (boundary) points,
  // and 0 for points on the grid to be calculated. 
  vtkGetObjectMacro(LabelImage, vtkImageData);
  vtkSetObjectMacro(LabelImage, vtkImageData);
  
  // Description:
  // Set initial solution for boundary points.
  vtkSetObjectMacro(Solution, vtkImageData);
  
  // Description:
  // Get convergence image
  vtkGetObjectMacro(ConvergenceImage, vtkImageData);
  
  // Description:
  // Get convergence image
  vtkGetObjectMacro(Gradient, vtkImageData);
  
  // Description:
  // Set/Get tensor symmetry flag
  vtkSetMacro(SymmetricTensor, int);
  vtkGetMacro(SymmetricTensor, int);
  vtkBooleanMacro(SymmetricTensor, int);

  // Description:
  // Enables/disables stopping at maximum error
  vtkSetMacro(StopOnMaximumError, int);
  vtkGetMacro(StopOnMaximumError, int);
  vtkBooleanMacro(StopOnMaximumError, int);

  // Description:
  // Get/Set maximum error value
  vtkGetMacro(MaximumError,float);
  vtkSetMacro(MaximumError,float);

  // Description:
  // Get/Set error value
  vtkGetMacro(ErrorValue,float);
  vtkSetMacro(ErrorValue,float);

  // Description:
  // Get/Set error value
  vtkGetMacro(IterationNumber,int);
  vtkSetMacro(IterationNumber,int);
  
  // Description:
  // Get/Set grid spacing
  vtkGetVector3Macro(Spacing,float);
  vtkSetVector3Macro(Spacing,float);

 protected:
  int SymmetricTensor;
  int NumberOfIterations;
  int KnownValues;
  float ErrorValue;
  int StopOnMaximumError;
  float MaximumError;
  int IterationNumber;
  float Spacing[3];

  vtkFloatArray *KnownPoints;
  vtkImageData *Mask;
  vtkImageData *Solution;
  vtkImageData *LabelImage;
  vtkDataArray *label;
  vtkImageData *ConvergenceImage;
  vtkDataArray *convergence;
  vtkImageData *Gradient;
  vtkDataArray *gradient;
  
  vtkmpjLaxFriedrichSweeping();
  ~vtkmpjLaxFriedrichSweeping();
  
  virtual void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  
  void Initialize(vtkImageData *input, vtkImageData *output);
  void Iterate(vtkImageData *input, vtkImageData *output);

  void Sweep(vtkImageData *input, vtkImageData *output, int dir[3]);
  void SweepBoundaries(vtkImageData *input, vtkImageData *output);
  
  double Update(vtkImageData *input, vtkImageData *output, 
		int coord[3], int index, double n[3]);
  
  double SolveH(int coord[3], int index, double p, double q, double r, double v);
  double SolveHp(int coord[3], int index, double p, double q, double r, double v);
  double SolveHq(int coord[3], int index, double p, double q, double r, double v); 
  double SolveHr(int coord[3], int index, double p, double q, double r, double v); 

 private: 
  //BTX
  float pog;
  int count, tenth; 
  //ETX
  
  vtkmpjLaxFriedrichSweeping(const vtkmpjLaxFriedrichSweeping&);// Not implemented
  void operator=(const vtkmpjLaxFriedrichSweeping&); // Not Implemented
};

#endif
