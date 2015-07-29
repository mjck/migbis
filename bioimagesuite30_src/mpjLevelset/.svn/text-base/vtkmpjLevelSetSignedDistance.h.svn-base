

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
   -- vtkmpjLevelSetSignedDistance.h --
   
   Marcel Jackowski, June 2003.
*/

#ifndef __vtkmpjLevelSetSignedDistance_h
#define __vtkmpjLevelSetSignedDistance_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageAppendComponents.h"
#include "vtkImageThreshold.h"
#include "vtkImageMathematics.h"
#include "vtkmpjFastMarchingMethod.h"
#include "vtkmpjFastMarchingExtension.h"
#include "vtkmpjExtractLevelSet.h"
#include "vtkmpjFrameExtract.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjLevelSetSignedDistance : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjLevelSetSignedDistance *New();
  vtkTypeMacro(vtkmpjLevelSetSignedDistance,vtkpxSimpleImageToImageFilter);
  
  // Set/Get maximum propagation time
  vtkGetMacro(LevelSetValue,float);
  vtkSetMacro(LevelSetValue,float);  
  
  // Description:
  // Set/Get maximum propagation time
  vtkGetMacro(PropagationTime,float);
  vtkSetMacro(PropagationTime,float);

  // Description:
  // Set/Get Extension velocities
  vtkGetObjectMacro(ExtensionImage, vtkImageData);
  vtkSetObjectMacro(ExtensionImage, vtkImageData);

 protected:
  float LevelSetValue;
  float PropagationTime;
  vtkImageData *ExtensionImage;

  vtkmpjLevelSetSignedDistance();
  virtual ~vtkmpjLevelSetSignedDistance();
    
  void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);

 private:
  //BTX
  float pog;
  int count, tenth;  
  //ETX
  
  vtkmpjLevelSetSignedDistance(const vtkmpjLevelSetSignedDistance&);// Not implemented
  void operator=(const vtkmpjLevelSetSignedDistance&); // Not Implemented
};

#endif
