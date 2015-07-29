

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
  -- vtkmpjLevelSetExtensionVelocity.h --
   
  Calculates the extension velocity for level set computation

  Marcel Jackowski, June 2003.
*/

#ifndef __vtkmpjLevelSetExtensionVelocity_h
#define __vtkmpjLevelSetExtensionVelocity_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkmpjLevelSetDistance.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjLevelSetExtensionVelocity : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjLevelSetExtensionVelocity *New();
  vtkTypeMacro(vtkmpjLevelSetExtensionVelocity,vtkpxSimpleImageToImageFilter);
  
 protected:
  vtkmpjLevelSetExtensionVelocity();
  virtual ~vtkmpjLevelSetExtensionVelocity();
    
  vtkSetObjectMacro(Velocity, vtkImageData);
  vtkGetObjectMacro(Velocity, vtkImageData);

  void ExecuteInformation();
  
  
  virtual double ComputeVelocity(vtkImageData *input, int coord[3], int index);
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);

 private:
  //BTX
  float pog;
  int count, tenth;  
  int neigh_index[3]; 
  //ETX
  
  vtkmpjLevelSetExtensionVelocity(const vtkmpjLevelSetExtensionVelocity&);// Not implemented
  void operator=(const vtkmpjLevelSetExtensionVelocity&); // Not Implemented
};

#endif
