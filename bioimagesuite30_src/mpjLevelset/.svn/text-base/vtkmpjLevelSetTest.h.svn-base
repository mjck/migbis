

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
  -- vtkmpjLevelSetTest.h --
   
  Marcel Jackowski, June 2003.
*/

#ifndef __vtkmpjLevelSetTest_h
#define __vtkmpjLevelSetTest_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkmpjLevelSetMethod.h"
#include "vtkImageAccumulate.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjLevelSetTest : public vtkmpjLevelSetMethod
{
 public: 
  static vtkmpjLevelSetTest *New();
  vtkTypeMacro(vtkmpjLevelSetTest,vtkpxSimpleImageToImageFilter);

  // Description: sets/gets front advection speed
  vtkSetObjectMacro(SpeedImage, vtkImageData);
  vtkGetObjectMacro(SpeedImage, vtkImageData);

 protected:
  vtkImageData *SpeedImage;
  
  vtkmpjLevelSetTest();
  virtual ~vtkmpjLevelSetTest();
    
  void ExecuteInformation();
  
  virtual double ComputeSpeed(vtkImageData *image, int index, int coord[3]);
  virtual double ComputeAdvection(vtkImageData *image, double v[3], int index, int coord[3]);
  //virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);

 private:
  //BTX
  float pog;
  int count, tenth;
  //ETX
  
  vtkmpjLevelSetTest(const vtkmpjLevelSetTest&);// Not implemented
  void operator=(const vtkmpjLevelSetTest&); // Not Implemented
};

#endif
