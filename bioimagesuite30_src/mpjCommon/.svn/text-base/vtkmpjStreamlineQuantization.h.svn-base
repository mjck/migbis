

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


#ifndef __vtkmpjStreamlineQuantization_h
#define __vtkmpjStreamlineQuantization_h

#include "vtkDataSetToDataSetFilter.h"
#include "vtkDataSet.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjStreamlineQuantization : public vtkDataSetToDataSetFilter
{
 public:
  static vtkmpjStreamlineQuantization *New();
  vtkTypeMacro(vtkmpjStreamlineQuantization,vtkDataSetToDataSetFilter);
  
  // Description: 
  // Sets/Gets image for streamline quantization
  vtkSetObjectMacro(Image, vtkImageData);
  vtkGetObjectMacro(Image, vtkImageData);

  // Description: Set/Gets background value
  vtkSetMacro(BackgroundValue,float);
  vtkGetMacro(BackgroundValue,float);

  // Description: Set/Gets foreground value
  vtkSetMacro(MaskValue,float);
  vtkGetMacro(MaskValue,float);

  // Description: Get number of quantized points
  vtkGetMacro(NumberOfVoxels, long int);

 protected:  
 
  float BackgroundValue;
  float MaskValue;  
  long NumberOfVoxels;
  vtkImageData *Image;
 
  vtkmpjStreamlineQuantization();
  virtual ~vtkmpjStreamlineQuantization();
    
  void Execute();
  
 private:

  vtkmpjStreamlineQuantization(const vtkmpjStreamlineQuantization&); // Not implemented
  void operator=(const vtkmpjStreamlineQuantization&); // Not implemented
};

#endif
