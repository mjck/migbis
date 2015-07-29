/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkmpjTensorStreamLine.h

  Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkmpjTensorStreamLine - generate streamlines in tensor datasets
// .SECTION Description

// .SECTION See Also
// vtkStreamer vtkDashedStreamLine vtkStreamPoints

#ifndef __vtkmpjTensorStreamLine_h
#define __vtkmpjTensorStreamLine_h

#include "vtkmpjTensorStreamer.h"

class vtkmpjTensorStreamLine : public vtkmpjTensorStreamer
{
public:
  vtkTypeMacro(vtkmpjTensorStreamLine,vtkmpjTensorStreamer);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Construct object with step size set to 1.0.
  static vtkmpjTensorStreamLine *New();

  // Description:
  // Specify the length of a line segment. The length is expressed in terms of
  // elapsed time. Smaller values result in smoother appearing streamlines, but
  // greater numbers of line primitives.
  vtkSetClampMacro(StepLength,double,0.000001,VTK_DOUBLE_MAX);
  vtkGetMacro(StepLength,double);

  // Description: enable/disable directional color coding
  vtkSetMacro(ComputeStatistics, bool);
  vtkGetMacro(ComputeStatistics, bool);

  // Description: enable/disable directional color coding
  vtkSetMacro(DirectionalColormap, int);
  vtkGetMacro(DirectionalColormap, int);
  vtkBooleanMacro(DirectionalColormap, int);
  
  // Description: determine type of color scheme to use
  vtkSetMacro(DirectionalColorScheme, int);
  vtkGetMacro(DirectionalColorScheme, int);

  // Description: color saturation
  vtkSetMacro(DirectionalColorSaturation, double);
  vtkGetMacro(DirectionalColorSaturation, double);
  
  // Description: intensity scaling
  vtkSetMacro(DirectionalColorScaling, double);
  vtkGetMacro(DirectionalColorScaling, double);

  // Description: magnitude component
  vtkSetMacro(DirectionalColorModulate, int);
  vtkGetMacro(DirectionalColorModulate, int);
          
  // Description: enable/disable fiber transparency
  vtkSetMacro(DirectionalColorAlphaBlending, int);
  vtkGetMacro(DirectionalColorAlphaBlending, int);
  
  // Description: enable/disable normal calculation
  vtkSetMacro(ComputeNormals, bool);
  vtkGetMacro(ComputeNormals, bool);

  // Description: enable/disable parameter calculation
  vtkSetMacro(ComputeParameters, bool);
  vtkGetMacro(ComputeParameters, bool);
  
protected:
  vtkmpjTensorStreamLine();
  ~vtkmpjTensorStreamLine() {};

  // Convert streamer array into vtkPolyData
  virtual int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *);

  // Directional colormap support functions
  void AbsoluteValueScheme(double FA, double *v, double *nrgb);
  void RotationalSymmetryScheme(double FA, double *v, double *nrgb);
  void NoSymmetryScheme(double FA, double *v, double *nrgb);
  void MirrorSymmetryScheme(double FA, double *v, double *nrgb);

  // the length of line primitives
  double StepLength;

  bool ComputeStatistics;
  bool ComputeNormals;
  bool ComputeParameters;

  // color-coding
  int DirectionalColormap;
  int DirectionalColorScheme;
  double DirectionalColorSaturation;
  double DirectionalColorScaling;
  int DirectionalColorModulate;
  int DirectionalColorAlphaBlending;

private:
  vtkmpjTensorStreamLine(const vtkmpjTensorStreamLine&);  // Not implemented.
  void operator=(const vtkmpjTensorStreamLine&);  // Not implemented.
};

#endif
