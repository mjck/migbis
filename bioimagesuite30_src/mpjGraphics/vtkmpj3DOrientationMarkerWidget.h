/*=========================================================================

 Program:   MedSquare
 Original Module:    vtkmsq3DOrientationMarkerWidget.h

 Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
 All rights reserved.
 See Copyright.txt or http://www.medsquare.org/copyright for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notice for more information.

 =========================================================================*/

#ifndef __vtkmpj3DOrientationMarkerWidget_h
#define __vtkmpj3DOrientationMarkerWidget_h

#include "vtkObject.h"
#include "vtkRenderWindowInteractor.h"

class vtkOrientationMarkerWidget;
class vtkActor;
class vtkAxesActor;
class vtkRenderWindowInteractor;
class vtkRenderer;

class vtkmpj3DOrientationMarkerWidget : public vtkObject
{
public:
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkmpj3DOrientationMarkerWidget *New();

  void SetInteractor(vtkRenderWindowInteractor *interactor);
  vtkRenderWindowInteractor *GetInteractor();
  void SetCurrentRenderer(vtkRenderer *renderer);
  void SetEnabled(int enable);
  void SetAxisLabels(int enable);

  void SetZAxisLabelText(const char *string);
  void SetYAxisLabelText(const char *string);
  void SetXAxisLabelText(const char *string);

  void GetXAxisColor(double color[3]);
  void SetXAxisColor(double color[3]);

  void GetYAxisColor(double color[3]);
  void SetYAxisColor(double color[3]);

  void GetZAxisColor(double color[3]);
  void SetZAxisColor(double color[3]);

  void GetSphereColor(double color[3]);
  void SetSphereColor(double color[3]);

protected:
  vtkmpj3DOrientationMarkerWidget();
  virtual ~vtkmpj3DOrientationMarkerWidget();

private:
  vtkOrientationMarkerWidget *markerWidget;
  vtkAxesActor *axesActor;
  vtkActor *sphereActor;

  double axialColor[3];
  double sagittalColor[3];
  double coronalColor[3];

  vtkmpj3DOrientationMarkerWidget(const vtkmpj3DOrientationMarkerWidget&); // Not implemented.
  void operator=(const vtkmpj3DOrientationMarkerWidget&); // Not implemented.
};

#endif
