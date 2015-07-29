

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


#ifndef __vtkmpjPointSource_h
#define __vtkmpjPointSource_h

#include "vtkPolyDataSource.h"

class vtkmpjPointSource : public vtkPolyDataSource 
{
public:
  vtkTypeMacro(vtkmpjPointSource,vtkPolyDataSource);

  // Description:
  // Construct volume with resolution 1x1x1, width
  // height and depth 1.0, and centered at the origin.
  static vtkmpjPointSource *New();

  vtkSetVector3Macro(Size,int);
  vtkGetVectorMacro(Size,int,3);

  vtkSetVector3Macro(Resolution,float);
  vtkGetVectorMacro(Resolution,float,3);

  // Description:
  // Specify a point defining the origin of the volume
  vtkSetVector3Macro(Origin,float);
  vtkGetVectorMacro(Origin,float,3);

  // Description:
  // Set/Get point spacing
  vtkGetVectorMacro(Spacing,float,3);
  vtkSetVectorMacro(Spacing,float,3);
  
protected:
  vtkmpjPointSource();
  ~vtkmpjPointSource() {};

  void Execute();

  float Resolution[3];
  float Origin[3];
  float Spacing[3];
  int Size[3];

private:
  vtkmpjPointSource(const vtkmpjPointSource&);  // Not implemented.
  void operator=(const vtkmpjPointSource&);  // Not implemented.
};

#endif


