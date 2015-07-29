

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
   -- vtkmpjExtractLevelSet.h --
   
   Extracts a levelset from a phi image.

   Marcel Jackowski, June 2003.
*/

#ifndef __vtkmpjExtractLevelSet_h
#define __vtkmpjExtractLevelSet_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjExtractLevelSet : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjExtractLevelSet *New();
  vtkTypeMacro(vtkmpjExtractLevelSet,vtkpxSimpleImageToImageFilter);
  
  // Set/Get the levelset to be extracted
  vtkGetMacro(LevelSetValue,float);
  vtkSetMacro(LevelSetValue,float);  

  // Get/Set output value for points inside levelset
  vtkGetMacro(InValue,float);
  vtkSetMacro(InValue,float);  
  
  // Get/Set output value for points outside levelset
  vtkGetMacro(OutValue,float);
  vtkSetMacro(OutValue,float);  

  // Get/Set inside points
  vtkGetObjectMacro(InsidePoints,vtkFloatArray);
  vtkSetObjectMacro(InsidePoints,vtkFloatArray);

  // Get/Set outside points
  vtkGetObjectMacro(OutsidePoints,vtkFloatArray);
  vtkSetObjectMacro(OutsidePoints,vtkFloatArray);

 protected:
  float InValue;                // output value for point on/inside levelset
  float OutValue;               // output value for point outside levelset
  float LevelSetValue;          // levelset to extract
  vtkFloatArray *InsidePoints;  // array of points on/immediately inside the levelset
  vtkFloatArray *OutsidePoints; // array of points immediately outside the levelset 
  
  vtkmpjExtractLevelSet();
  virtual ~vtkmpjExtractLevelSet();
    
  // void ExecuteInformation();
  
  void Sort(double values[3]);
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  virtual double ComputeDistance(vtkImageData *input, int coord[3], int index);
  
 private:
  //BTX
  float pog;
  int count, tenth;  
  //ETX
  
  vtkmpjExtractLevelSet(const vtkmpjExtractLevelSet&);// Not implemented
  void operator=(const vtkmpjExtractLevelSet&); // Not Implemented
};

#endif
