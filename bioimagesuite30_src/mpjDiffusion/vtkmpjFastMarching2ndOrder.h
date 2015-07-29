

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
  -- vtkmpjFastMarching2ndOrder.h --
   
  Implements Sethian's Fast Marching Method on 3D lattice domains.

  Solves the Eikonal equation |Gradient T| = 1 / F, where T is the
  arrival time of the front and F is the speed.

  This version employs a 2nd-order approximation to the gradient. It
  is also known as the high-accuracy FMM, or FMMHA in the literature.

  Marcel Jackowski, January 2003
*/

#define VTK_MPJ_FROZEN       200     
#define VTK_MPJ_NARROW_BAND  100
#define VTK_MPJ_UNVISITED    0

#ifndef __vtkmpjFastMarching2ndOrder_h
#define __vtkmpjFastMarching2ndOrder_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkmpjPriorityQueue.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkUnsignedCharArray.h"

class vtkmpjFastMarching2ndOrder : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjFastMarching2ndOrder *New();
  vtkTypeMacro(vtkmpjFastMarching2ndOrder,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining propagation extents
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
  // Description:
  // Set/Get the seed points for the front
  vtkSetObjectMacro(Seeds, vtkIntArray);
  vtkGetObjectMacro(Seeds, vtkIntArray);
  
 protected:   
  vtkImageData *Mask;
  vtkIntArray *Seeds;

  vtkmpjFastMarching2ndOrder();
  virtual ~vtkmpjFastMarching2ndOrder();
  
  void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
 
  // The speed function 
  virtual double F(vtkImageData *input, vtkImageData *output, 
		   int coord[3], int index); 

  virtual void InitializeFront(vtkImageData *input, vtkImageData *output);
  virtual void PropagateFront(vtkImageData *input, vtkImageData *output);

 private: 
 
  //BTX
  float pog;
  int count, tenth;
  vtkDataArray *mask;
  vtkUnsignedCharArray *state;    
  vtkmpjPriorityQueue *queue;     
  
  int    InMask(int index);
  double ComputeArrivalTime(vtkImageData *input, vtkImageData *output, 
			    int coord[3], int index);
  void   ComputeNeighbor(vtkImageData *input, vtkImageData *output, 
			 int coord[3], int index);  
  //ETX

  vtkmpjFastMarching2ndOrder(const vtkmpjFastMarching2ndOrder&); // Not implemented
  void operator=(const vtkmpjFastMarching2ndOrder&); // Not Implemented
};

#endif
