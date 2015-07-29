

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
  -- vtkmpjFastMarchingVesselConnectivity.h --

  Marcel Jackowski, Mar 2005.

*/

#define VTK_MPJ_FROZEN       -1     
#define VTK_MPJ_NARROW_BAND   0
#define VTK_MPJ_UNVISITED    +1

#ifndef __vtkmpjFastMarchingVesselConnectivity_h
#define __vtkmpjFastMarchingVesselConnectivity_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkmpjPriorityQueue.h"
#include "vtkImageAccumulate.h"
#include "vtkDoubleArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjFastMarchingVesselConnectivity : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjFastMarchingVesselConnectivity *New();
  vtkTypeMacro(vtkmpjFastMarchingVesselConnectivity,vtkpxSimpleImageToImageFilter);
  
  // Description: Sets speed image
  void SetSpeedImage(vtkImageData *speed);

  // Description:
  // Set/Get boundary conditions for the propagation
  vtkSetObjectMacro(KnownPoints, vtkDoubleArray);
  vtkGetObjectMacro(KnownPoints, vtkDoubleArray);

  // Description:
  // Turn on/off values for known points
  vtkGetMacro(KnownValues,int);
  vtkSetClampMacro(KnownValues,int,0,1);
  vtkBooleanMacro(KnownValues,int);

  // Description:
  // Set/Get maximum propagation time
  vtkGetMacro(PropagationTime,double);
  vtkSetMacro(PropagationTime,double);
  
  // Description:
  // Set/Get infinity value
  vtkGetMacro(Infinity,double);
  vtkSetMacro(Infinity,double);

  // Description:
  // Get/Set grid spacing
  vtkGetVector3Macro(Spacing,double);
  vtkSetVector3Macro(Spacing,double);

  vtkGetMacro(MaxIntensity,double);
  vtkSetMacro(MaxIntensity,double);
  
  // Front label image. Points interior to the front have value -1, points 
  // on the front boundary have value 0, and points outside have value +1.
  vtkGetObjectMacro(LabelImage, vtkImageData);
  
  // Return an array containing front point vertices and their values
  vtkGetObjectMacro(FrontPoints, vtkDoubleArray);
  
  // Description: 
  // Sets/Gets mask for determining propagation extents
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);

  // Description:
  // Sets/Gets propagation scaling
  vtkSetObjectMacro(ScaleImage, vtkImageData);
  vtkGetObjectMacro(ScaleImage, vtkImageData);

 protected:
  double MaxIntensity;
  double Spacing[3];
  double PropagationTime;
  int KnownValues;
  double Infinity;
  vtkImageData *Speed;
  vtkImageData *LabelImage;
  vtkImageData *Mask;
  vtkImageData *ScaleImage;
  vtkDoubleArray *KnownPoints;
  vtkDoubleArray *FrontPoints;

  vtkDataArray *label;
  vtkmpjPriorityQueue *queue;     

  vtkmpjFastMarchingVesselConnectivity();
  virtual ~vtkmpjFastMarchingVesselConnectivity();
  
  void ExecuteInformation();  
  virtual int InMask(int neighbor);
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  virtual void InitializeFront(vtkImageData *input, vtkImageData *output);
  virtual void PropagateFront(vtkImageData *input, vtkImageData *output);
  virtual void SaveFrontPoints(vtkImageData *input);  
  virtual double ComputeSpeed(vtkImageData *input, vtkImageData *output,
			      int parent, int coord[3], int index, double normal[3]);  
  virtual double ComputeFirstOrder(vtkImageData *input, vtkImageData *output,
				   int parent, int coord[3], int index);
  virtual void ComputeNormal(vtkImageData *input, vtkImageData *output,
			     int coord[3], int index, double result[3]);
  
  void SortTerms(double term[3]);
  double Solve2(double a, double b, double rhs);
  double SolveN(int number, double term[3], double rhs);
  double SolveQuadratic(int number, double term[3], double rhs);
  
  double ComputeTime(vtkImageData *input, vtkImageData *output,
		     int parent, int coord[3], int index);  

  double ComputeWeight(double value, double max, 
		       double steepness, double center);
    
  void Jacobi3x3(double a[3][3], double w[3], double v[3][3]);
  void NormalizeTensor3x3(double T[3][3], double W[3], double V[3][3], double N[3][3]);
  void SortEigenvectors(double W[3], double T[3][3]);
  void PrintMatrix3x3(char *title, double M[3][3]);

 private:
  //BTX
  double pog;
  int count, tenth; 
  vtkDataArray *mask;
  int symmetric;
  //ETX
  
  vtkmpjFastMarchingVesselConnectivity(const vtkmpjFastMarchingVesselConnectivity&);// Not implemented
  void operator=(const vtkmpjFastMarchingVesselConnectivity&); // Not Implemented
};

#endif
