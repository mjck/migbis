

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
   -- vtkmpjFastSweepingMethod.h --
   
   Implements Stanley Osher's Fast Sweeping method for solving
   the Eikonal equation |grad(T)|*F= 1.
   
   Marcel Jackowski, October 2003.
   
*/

#define VTK_MPJ_KNOWN   100   
#define VTK_MPJ_UNKNOWN   0   

#ifndef __vtkmpjFastSweepingMethod_h
#define __vtkmpjFastSweepingMethod_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageAccumulate.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjFastSweepingMethod : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjFastSweepingMethod *New();
  vtkTypeMacro(vtkmpjFastSweepingMethod,vtkpxSimpleImageToImageFilter);
  
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

 protected:
  int SecondOrder;
  int NumberOfIterations;
  int KnownValues;
  vtkImageData *Speed;
  vtkFloatArray *KnownPoints;
  vtkImageData *LabelImage;
  vtkDataArray *label;

  vtkmpjFastSweepingMethod();
  virtual ~vtkmpjFastSweepingMethod();
  
  void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);

  void Initialize(vtkImageData *input, vtkImageData *output);
  void Iterate(vtkImageData *input, vtkImageData *output);
  void Sweep(vtkImageData *input, vtkImageData *output, int dir[3]);
  double Update(vtkImageData *input, vtkImageData *output, int coord[3], int index);

  void Sort(double values[3]);
  double Solve(double a, double b);

  virtual double ComputeFirstOrder(vtkImageData *input, vtkImageData *output, int coord[3], int index);
  virtual void ComputeNormal(vtkImageData *input, vtkImageData *output, int coord[3], int index, double result[3]);
  virtual void SortTerms(double term[3]);

  double Solve2(double a, double b, double rhs);
  double SolveN(int number, double term[3], double rhs);
  double SolveQuadratic(int number, double term[3], double rhs);

 private: 
  //BTX
  float pog;
  int count, tenth; 
  //ETX
  
  vtkmpjFastSweepingMethod(const vtkmpjFastSweepingMethod&);// Not implemented
  void operator=(const vtkmpjFastSweepingMethod&); // Not Implemented
};

#endif
