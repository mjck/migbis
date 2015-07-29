

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
  -- vtkmpjLevelSetNarrowBandFilter.h --
  
  Given an input level set, creates a narrow band of a given size.
  Returns an image mask of the narrow band as well as a list of
  locations and level set values.
  
  Marcel Jackowski, June 2003.
*/

#ifndef __vtkmpjLevelSetNarrowBandFilter_h
#define __vtkmpjLevelSetNarrowBandFilter_h

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

class vtkmpjLevelSetNarrowBandFilter : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjLevelSetNarrowBandFilter *New();
  vtkTypeMacro(vtkmpjLevelSetNarrowBandFilter,vtkpxSimpleImageToImageFilter);
  
  // Set/Get narrow bandwidth 
  vtkGetMacro(NarrowBandwidth,float);
  vtkSetMacro(NarrowBandwidth,float);

  // Set/Get level set to extract
  vtkGetMacro(LevelSetValue, float);
  vtkSetMacro(LevelSetValue, float);

  // Get extracted narrow band points
  vtkGetObjectMacro(NarrowBandPoints, vtkFloatArray);

 protected:
  vtkFloatArray *NarrowBandPoints;
  float NarrowBandwidth; // default 12
  float LevelSetValue;   // default 0

  vtkmpjLevelSetNarrowBandFilter();
  virtual ~vtkmpjLevelSetNarrowBandFilter();
  
  void ExecuteInformation();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  
 private:
  //BTX
  float pog;
  int count, tenth;
  //ETX

  vtkmpjLevelSetNarrowBandFilter(const vtkmpjLevelSetNarrowBandFilter&);// Not implemented
  void operator=(const vtkmpjLevelSetNarrowBandFilter&); // Not Implemented
};

#endif
