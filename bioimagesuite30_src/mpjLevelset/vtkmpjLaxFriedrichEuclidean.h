

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
   -- vtkmpjLaxFriedrichEuclidean.h --
   
   Implements Stanley Osher's Fast Sweeping Scheme for solving
   Static Hamilton-Jacobi Equations
   
   Formulation for 3-D Euclidean distance.

   Marcel Jackowski, Feb. 2004.
   
*/

#define VTK_MPJ_KNOWN   100   
#define VTK_MPJ_UNKNOWN   0   

#ifndef __vtkmpjLaxFriedrichEuclidean_h
#define __vtkmpjLaxFriedrichEuclidean_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageAccumulate.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjLaxFriedrichEuclidean : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjLaxFriedrichEuclidean *New();
  vtkTypeMacro(vtkmpjLaxFriedrichEuclidean,vtkpxSimpleImageToImageFilter);
  
  // Description: Sets speed image
  void SetSpeedImage(vtkImageData *speed);
  
  // Description:
  // Set/Get boundary conditions for the propagation
  vtkSetObjectMacro(KnownPoints, vtkFloatArray);
  vtkGetObjectMacro(KnownPoints, vtkFloatArray);
  
  // Description:
  // Get/Set Number of Iterations
  vtkGetMacro(NumberOfIterations,int);
  vtkSetMacro(NumberOfIterations,int);

  // Front label image. Points interior to the front have value -1, points 
  // on the front boundary have value 0, and points outside have value +1.
  vtkGetObjectMacro(LabelImage, vtkImageData);
  vtkGetObjectMacro(ConvergenceImage, vtkImageData);
  
  // Description:
  // Set/Get Solution
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

  vtkmpjLaxFriedrichEuclidean();
  virtual ~vtkmpjLaxFriedrichEuclidean();
  
  void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);

  void Initialize(vtkImageData *input, vtkImageData *output);
  void Iterate(vtkImageData *input, vtkImageData *output);

  void Sweep(vtkImageData *input, vtkImageData *output, int dir[3]);
  void SweepBoundaries(vtkImageData *input, vtkImageData *output);

  double Update(vtkImageData *input, vtkImageData *output, 
		int coord[3], int index);
  
  double SolveH(int coord[3], int index, double p, double q, double r);
  double SolveHp(int coord[3], int index, double p, double q, double r);
  double SolveHq(int coord[3], int index, double p, double q, double r);
  double SolveHr(int coord[3], int index, double p, double q, double r);

 private: 
  //BTX
  float pog;
  int count, tenth; 
  //ETX
  
  vtkmpjLaxFriedrichEuclidean(const vtkmpjLaxFriedrichEuclidean&);// Not implemented
  void operator=(const vtkmpjLaxFriedrichEuclidean&); // Not Implemented
};

#endif
