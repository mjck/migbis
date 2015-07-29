

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
   -- vtkmpjLevelSetDistance.h --
   
   Marcel Jackowski, June 2003.
*/

#ifndef __vtkmpjLevelSetDistance_h
#define __vtkmpjLevelSetDistance_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkmpjPriorityQueue.h"
#include "vtkImageAccumulate.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjLevelSetDistance : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjLevelSetDistance *New();
  vtkTypeMacro(vtkmpjLevelSetDistance,vtkpxSimpleImageToImageFilter);
  
  // Set/Get maximum propagation time
  vtkGetMacro(LevelSetValue,float);
  vtkSetMacro(LevelSetValue,float);  
  
 protected:
  float LevelSetValue;
  vtkImageData *LevelSetMask;

  vtkmpjLevelSetDistance();
  virtual ~vtkmpjLevelSetDistance();
    
  void ExecuteInformation();

  void Sort(double values[3]);
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  virtual double ComputeDistance(vtkImageData *input, int coord[3], int index);

 private:
  //BTX
  float pog;
  int count, tenth;  
  //ETX
  
  vtkmpjLevelSetDistance(const vtkmpjLevelSetDistance&);// Not implemented
  void operator=(const vtkmpjLevelSetDistance&); // Not Implemented
};

#endif
