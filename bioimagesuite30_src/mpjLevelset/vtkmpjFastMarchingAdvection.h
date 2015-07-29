

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
  -- vtkmpjFastMarchingAdvection.h --
   
  Implements Sethian's Fast Marching Method on 3D lattice domains.

  Solves the Eikonal equation |grad(T)|*[U.n] = 1, where T is the
  arrival time of the front, U is an advection vector (x,y,z), and
  n is the normal to the front. 

  If
  |grad(T)| * F = 1, and
  F = dot(U, n), where n = grad(T)/|grad(T)|
  then solve
  dot(grad(T),U)=1.

  Marcel Jackowski, April 2003
*/

#define VTK_MPJ_FROZEN       200     
#define VTK_MPJ_NARROW_BAND  100
#define VTK_MPJ_UNVISITED    0

#ifndef __vtkmpjFastMarchingAdvection_h
#define __vtkmpjFastMarchingAdvection_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkmpjPriorityQueue.h"
#include "vtkImageAccumulate.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjFastMarchingAdvection : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjFastMarchingAdvection *New();
  vtkTypeMacro(vtkmpjFastMarchingAdvection,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining propagation extents
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
  // Description:
  // Set/Get the seed points for the propagation
  vtkSetObjectMacro(Seeds, vtkIntArray);
  vtkGetObjectMacro(Seeds, vtkIntArray);
  
  // Description: Turn on/off debugging
  vtkGetMacro(Debug,int);
  vtkSetClampMacro(Debug,int,0,1);
  vtkBooleanMacro(Debug,int);

  // In case a mask is not defined, it always returns 1.
  int InsideMask(int index);

 protected:
  int Debug;
  float MaskOutValue;    
  vtkImageData *Mask;
  vtkIntArray *Seeds;
  vtkUnsignedCharArray *State;    

  vtkmpjFastMarchingAdvection();
  virtual ~vtkmpjFastMarchingAdvection();
  
  void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
 
  // The speed function 
  virtual double F(vtkImageData *input, vtkImageData *output, 
		   int coord[3], int index); 
  
  //  int InsideMask(int index);
  virtual void InitializeFront(vtkImageData *input, vtkImageData *output);
  virtual void PropagateFront(vtkImageData *input, vtkImageData *output);
  
 private: 
  //BTX
  float pog;
  int count, tenth;
  vtkDataArray *mask;
  vtkmpjPriorityQueue *queue;     

  double ComputeTFirstOrder(vtkImageData *input, vtkImageData *output, 
			    int coord[3], int index);
  double ComputeNewTFirstOrder(vtkImageData *input, vtkImageData *output, 
			    int coord[3], int index);
  double ComputeTime(vtkImageData *input, vtkImageData *output, 
		     int coord[3], int index);
  //ETX
  
  vtkmpjFastMarchingAdvection(const vtkmpjFastMarchingAdvection&);// Not implemented
  void operator=(const vtkmpjFastMarchingAdvection&); // Not Implemented
};

#endif
