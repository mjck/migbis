

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


#ifndef __vtkmpjDistanceMapNormals_h
#define __vtkmpjDistanceMapNormals_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjDistanceMapNormals : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjDistanceMapNormals *New();
  vtkTypeMacro(vtkmpjDistanceMapNormals,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining propagation extents
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
 protected:
  vtkImageData *Mask;

  vtkmpjDistanceMapNormals();
  virtual ~vtkmpjDistanceMapNormals();
  
  void ComputeNormals(vtkImageData *input, vtkImageData *output, int coord[3], int index);
  void ComputeMap(vtkImageData *input, vtkImageData *output, int coord[3], int index);
  void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  int InsideMask(int index);
  
 private: 
  //BTX
  vtkDataArray *mask;
  //ETX
  vtkmpjDistanceMapNormals(const vtkmpjDistanceMapNormals&);// Not implemented
  void operator=(const vtkmpjDistanceMapNormals&); // Not Implemented
};

#endif
