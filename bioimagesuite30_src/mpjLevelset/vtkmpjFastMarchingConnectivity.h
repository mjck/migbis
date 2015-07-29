

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
  -- vtkmpjFastMarchingConnectivity.h --

  Marcel Jackowski, Dec 2004.
*/

#define VTK_MPJ_FROZEN       -1     
#define VTK_MPJ_NARROW_BAND   0
#define VTK_MPJ_UNVISITED    +1

#ifndef __vtkmpjFastMarchingConnectivity_h
#define __vtkmpjFastMarchingConnectivity_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkmpjPriorityQueue.h"
#include "vtkImageAccumulate.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjFastMarchingConnectivity : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjFastMarchingConnectivity *New();
  vtkTypeMacro(vtkmpjFastMarchingConnectivity,vtkpxSimpleImageToImageFilter);
  
  // Description: Sets speed image
  void SetSpeedImage(vtkImageData *speed);

  // Description:
  // Set/Get boundary conditions for the propagation
  vtkSetObjectMacro(KnownPoints, vtkFloatArray);
  vtkGetObjectMacro(KnownPoints, vtkFloatArray);

  // Description:
  // Turn on/off values for known points
  vtkGetMacro(KnownValues,int);
  vtkSetClampMacro(KnownValues,int,0,1);
  vtkBooleanMacro(KnownValues,int);

  // Description:
  // Set/Get maximum propagation time
  vtkGetMacro(PropagationTime,float);
  vtkSetMacro(PropagationTime,float);
  
  // Description:
  // Set/Get infinity value
  vtkGetMacro(Infinity,float);
  vtkSetMacro(Infinity,float);

  // Description:
  // Get/Set grid spacing
  vtkGetVector3Macro(Spacing,float);
  vtkSetVector3Macro(Spacing,float);

  // Front label image. Points interior to the front have value -1, points 
  // on the front boundary have value 0, and points outside have value +1.
  vtkGetObjectMacro(LabelImage, vtkImageData);
  
  // Return an array containing front point vertices and their values
  vtkGetObjectMacro(FrontPoints, vtkFloatArray);
  
  // Description: 
  // Sets/Gets mask for determining propagation extents
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
    
  // Description:
  // Set/Get tensor symmetry flag
  vtkSetMacro(SymmetricTensor, int);
  vtkGetMacro(SymmetricTensor, int);
  vtkBooleanMacro(SymmetricTensor, int);

 protected:
  int SymmetricTensor;
  float Spacing[3];
  float PropagationTime;
  int KnownValues;
  float Infinity;
  vtkImageData *Speed;
  vtkImageData *LabelImage;
  vtkImageData *Mask;
  vtkFloatArray *KnownPoints;
  vtkFloatArray *FrontPoints;

  vtkDataArray *label;
  vtkmpjPriorityQueue *queue;     

  vtkmpjFastMarchingConnectivity();
  virtual ~vtkmpjFastMarchingConnectivity();
  
  void ExecuteInformation();  
  virtual int InMask(int neighbor);
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  virtual void InitializeFront(vtkImageData *input, vtkImageData *output);
  virtual void PropagateFront(vtkImageData *input, vtkImageData *output);
  virtual void SaveFrontPoints(vtkImageData *input);  
  virtual double ComputeSpeed(vtkImageData *input, vtkImageData *output,
			      int parent, int coord[3], int index, float normal[3]);  
  virtual double ComputeFirstOrder(vtkImageData *input, vtkImageData *output,
				   int parent, int coord[3], int index);
  virtual void ComputeNormal(vtkImageData *input, vtkImageData *output,
			     int coord[3], int index, float result[3]);
  
  void SortTerms(double term[3]);
  double Solve2(double a, double b, double rhs);
  double SolveN(int number, double term[3], double rhs);
  double SolveQuadratic(int number, double term[3], double rhs);
  
  double ComputeTime(vtkImageData *input, vtkImageData *output,
		     int parent, int coord[3], int index);  
  
  void Jacobi3x3(float a[3][3], float w[3], float v[3][3]);
  void NormalizeTensor3x3(float T[3][3], float W[3], float V[3][3], float N[3][3]);
		 
		 

 private:
  //BTX
  float pog;
  int count, tenth; 
  vtkDataArray *mask;
  int symmetric;
  //ETX
  
  vtkmpjFastMarchingConnectivity(const vtkmpjFastMarchingConnectivity&);// Not implemented
  void operator=(const vtkmpjFastMarchingConnectivity&); // Not Implemented
};

#endif
