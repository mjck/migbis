

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


#ifndef __vtkmpjImagePointSource_h
#define __vtkmpjImagePointSource_h

#include "vtkPolyDataSource.h"
#include "vtkImageData.h"

class vtkmpjImagePointSource : public vtkPolyDataSource 
{
public:
  vtkTypeMacro(vtkmpjImagePointSource,vtkPolyDataSource);

  // Description:
  // Construct volume with resolution 1x1x1, width
  // height and depth 1.0, and centered at the origin.
  static vtkmpjImagePointSource *New();

  vtkSetVector3Macro(Size,float);
  vtkGetVectorMacro(Size,float,3);

  // Description: Set/Get the density of points
  vtkSetMacro(Density,float);
  vtkGetMacro(Density,float);

  // Description:
  // Specify a point defining the origin of the volume
  vtkSetVector3Macro(Center,float);
  vtkGetVectorMacro(Center,float,3);

  // Description:
  // Specify the origin of the coord system
  vtkSetVector3Macro(Origin,float);
  vtkGetVectorMacro(Origin,float,3);

  // Description:
  // Specify the spacing between points
  vtkSetVector3Macro(Spacing,float);
  vtkGetVectorMacro(Spacing,float,3);

  // Description:
  // Set/Get Mask of points
  vtkSetObjectMacro(Mask,vtkImageData);
  vtkGetObjectMacro(Mask,vtkImageData);

protected:
  vtkmpjImagePointSource();
  ~vtkmpjImagePointSource();

  void Execute();

  vtkImageData *Mask;
  float Density;
  float Center[3];
  float Origin[3];
  float Spacing[3];
  float Size[3];

private:
  vtkmpjImagePointSource(const vtkmpjImagePointSource&);  // Not implemented.
  void operator=(const vtkmpjImagePointSource&);  // Not implemented.
};

#endif


