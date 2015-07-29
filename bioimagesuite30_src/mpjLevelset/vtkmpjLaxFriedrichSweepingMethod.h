

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
   -- vtkmpjLaxFriedrichSweepingMethod.h --
   
   Implements Stanley Osher's Fast Sweeping Scheme for solving
   Static Hamilton-Jacobi Equations
   
   Marcel Jackowski, October 2003.
   
*/

#define VTK_MPJ_KNOWN   100   
#define VTK_MPJ_UNKNOWN   0   

#ifndef __vtkmpjLaxFriedrichSweepingMethod_h
#define __vtkmpjLaxFriedrichSweepingMethod_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageAccumulate.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjLaxFriedrichSweepingMethod : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjLaxFriedrichSweepingMethod *New();
  vtkTypeMacro(vtkmpjLaxFriedrichSweepingMethod,vtkpxSimpleImageToImageFilter);
  
  // Description: Sets speed image
  void SetSpeedImage(vtkImageData *speed);

  // Description:
  // Set/Get boundary conditions for the propagation
  vtkSetObjectMacro(KnownPoints, vtkFloatArray);
  vtkGetObjectMacro(KnownPoints, vtkFloatArray);

  // Description: Turns on/off second order approximation to the gradient
  vtkGetMacro(SecondOrder,int);
  vtkSetClampMacro(SecondOrder,int,0,1);
  vtkBooleanMacro(SecondOrder,int);

  vtkGetMacro(NumberOfIterations,int);
  vtkSetMacro(NumberOfIterations,int);

  // Front label image. Points interior to the front have value -1, points 
  // on the front boundary have value 0, and points outside have value +1.
  vtkGetObjectMacro(LabelImage, vtkImageData);
  vtkGetObjectMacro(ConvergenceImage, vtkImageData);
  
  vtkSetObjectMacro(Solution, vtkImageData);
  vtkGetObjectMacro(Solution, vtkImageData);
    
 protected:
  int SecondOrder;
  int NumberOfIterations;
  int KnownValues;
  vtkImageData *Speed;
  vtkImageData *Solution;
  vtkFloatArray *KnownPoints;
  vtkImageData *LabelImage;
  vtkImageData *ConvergenceImage;
  vtkDataArray *label;
  vtkDataArray *convergence;

  vtkmpjLaxFriedrichSweepingMethod();
  virtual ~vtkmpjLaxFriedrichSweepingMethod();
  
  void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);

  void Initialize(vtkImageData *input, vtkImageData *output);
  void Iterate(vtkImageData *input, vtkImageData *output);
  void Sweep(vtkImageData *input, vtkImageData *output, int dir[3]);
  void SweepBoundaries(vtkImageData *input, vtkImageData *output);
  double Update(vtkImageData *input, vtkImageData *output, int coord[3], 
		int index);
  double SolveH(int coord[3], int index, double p, double q, double r, 
		float N[3][3], float V[3][3], float FA);
  double SolveHp(int coord[3], int index, double p, double q, double r, 
		float N[3][3], float V[3][3], float FA); 
  double SolveHq(int coord[3], int index, double p, double q, double r, 
		float N[3][3], float V[3][3], float FA);
  double SolveHr(int coord[3], int index, double p, double q, double r, 
		float N[3][3], float V[3][3], float FA);
  
  void NormalizeTensor3x3(float T[3][3], float W[3], float V[3][3], 
			  float N[3][3]);
  void Jacobi3x3(float a[3][3], float w[3], float v[3][3]);
  
 private: 
  //BTX
  float pog;
  int count, tenth; 
  //ETX
  
  vtkmpjLaxFriedrichSweepingMethod(const vtkmpjLaxFriedrichSweepingMethod&);// Not implemented
  void operator=(const vtkmpjLaxFriedrichSweepingMethod&); // Not Implemented
};

#endif
