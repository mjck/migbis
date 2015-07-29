

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
  -- vtkmpjFastMarchingExtension.h --
   
  Marcel Jackowski, June 2003
*/

#define VTK_MPJ_FROZEN       -1     
#define VTK_MPJ_NARROW_BAND   0
#define VTK_MPJ_UNVISITED    +1

#ifndef __vtkmpjFastMarchingExtension_h
#define __vtkmpjFastMarchingExtension_h

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

class vtkmpjFastMarchingExtension : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjFastMarchingExtension *New();
  vtkTypeMacro(vtkmpjFastMarchingExtension,vtkpxSimpleImageToImageFilter);
  
  // Description: Sets speed image
  void SetSpeedImage(vtkImageData *speed);

  // Description: Set/Get extension image
  vtkSetObjectMacro(ExtensionImage, vtkImageData);
  vtkGetObjectMacro(ExtensionImage, vtkImageData);

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
  // Set/Get grid spacing
  vtkGetMacro(Spacing,float);
  vtkSetMacro(Spacing,float);
  
  // Front label image. Points interior to the front have value -1, points 
  // on the front boundary have value 0, and points outside have value +1.
  vtkGetObjectMacro(LabelImage, vtkImageData);
  
  // Return an array containing front point vertices and their values
  vtkGetObjectMacro(FrontPoints, vtkFloatArray);
  
 protected:
  float Spacing;
  float PropagationTime;
  int KnownValues;
  vtkImageData *Speed;
  vtkImageData *LabelImage;
  vtkImageData *ExtensionImage;
  vtkFloatArray *KnownPoints;
  vtkFloatArray *FrontPoints;

  vtkDataArray *label;
  vtkmpjPriorityQueue *queue;     

  vtkmpjFastMarchingExtension();
  virtual ~vtkmpjFastMarchingExtension();
  
  void ExecuteInformation();  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  virtual void InitializeFront(vtkImageData *input, vtkImageData *output);
  virtual void PropagateFront(vtkImageData *input, vtkImageData *output);
  virtual void SaveFrontPoints(vtkImageData *input);  
  virtual double ComputeSpeed(vtkImageData *input, vtkImageData *output, int coord[3], int index);
  virtual double ComputeFirstOrder(vtkImageData *input, vtkImageData *output, int coord[3], int index);

  int GetNeighborIndex(int j);
  void SortTerms(double term[3]);
  double Solve2(double a, double b, double rhs);
  double SolveN(int number, double term[3], double rhs);
  double SolveQuadratic(int number, double term[3], double rhs);
  
  double ComputeTime(vtkImageData *input, vtkImageData *output,
		     int coord[3], int index);  
 private:
  //BTX
  float pog;
  int count, tenth; 
  int neighbor_index[3];
  //ETX
  
  vtkmpjFastMarchingExtension(const vtkmpjFastMarchingExtension&);// Not implemented
  void operator=(const vtkmpjFastMarchingExtension&); // Not Implemented
};

#endif
