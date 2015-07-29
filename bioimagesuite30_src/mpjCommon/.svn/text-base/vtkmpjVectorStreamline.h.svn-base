

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



#ifndef __vtkmpjVectorStreamline_h
#define __vtkmpjVectorStreamline_h

#include "vtkmpjStreamer.h"

class vtkmpjVectorStreamline : public vtkmpjStreamer
{
public:
  vtkTypeMacro(vtkmpjVectorStreamline,vtkmpjStreamer);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct object with step size set to 1.0.
  static vtkmpjVectorStreamline *New();

  // Description:
  // Specify the length of a line segment. The length is expressed in terms of
  // elapsed time. Smaller values result in smoother appearing streamlines, but
  // greater numbers of line primitives.
  vtkSetClampMacro(StepLength,double,0.000001,VTK_LARGE_FLOAT);
  vtkGetMacro(StepLength,double);

protected:
  vtkmpjVectorStreamline();
  virtual ~vtkmpjVectorStreamline() {}

  // Convert streamer array into vtkPolyData
  void Execute();

  // the length of line primitives
  double StepLength;

private:
  vtkmpjVectorStreamline(const vtkmpjVectorStreamline&);  // Not implemented.
  void operator=(const vtkmpjVectorStreamline&);  // Not implemented.
};

#endif


