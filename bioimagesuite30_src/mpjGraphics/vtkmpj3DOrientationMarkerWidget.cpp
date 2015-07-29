/*=========================================================================

 Program:   MedSquare
 Module:    vtkmpj3DOrientationMarkerWidget.cxx

 Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
 All rights reserved.
 See Copyright.txt or http://www.medsquare.org/copyright for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notice for more information.

 =========================================================================*/

#include "vtkmpj3DOrientationMarkerWidget.h"

#include "vtkActor.h"
#include "vtkAxesActor.h"
#include "vtkCaptionActor2D.h"
#include "vtkOrientationMarkerWidget.h"
#include "vtkObjectFactory.h"
#include "vtkPolyDataMapper.h"
#include "vtkPropAssembly.h"
#include "vtkProperty.h"
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkTextProperty.h"

/** \cond 0 */
vtkStandardNewMacro(vtkmpj3DOrientationMarkerWidget);
/** \endcond */

/***********************************************************************************//**
 * 
 */
vtkmpj3DOrientationMarkerWidget::vtkmpj3DOrientationMarkerWidget()
{
  axialColor[0] = 0.0;
  axialColor[1] = 0.47;
  axialColor[2] = 0.78;

  sagittalColor[0] = 1.0;
  sagittalColor[1] = 0.0;
  sagittalColor[2] = 0.0;

  coronalColor[0] = 0.0;
  coronalColor[1] = 1.0;
  coronalColor[2] = 0.0;

  axesActor = vtkAxesActor::New();
  axesActor->SetShaftTypeToCylinder();
  axesActor->SetTotalLength(1.0, 1.0, 1.0);
  axesActor->AxisLabelsOn();
  axesActor->SetCylinderRadius(0.09);
  axesActor->SetConeRadius(0.7);
  axesActor->GetZAxisTipProperty()->SetColor(axialColor);
  axesActor->GetZAxisShaftProperty()->SetColor(axialColor);

  axesActor->SetXAxisLabelText("X");
  axesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->ItalicOn();
  axesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
  axesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(sagittalColor);

  axesActor->SetYAxisLabelText("Y");
  axesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ItalicOn();
  axesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
  axesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(coronalColor);

  axesActor->SetZAxisLabelText("Z");
  axesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ItalicOn();
  axesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->ShadowOff();
  axesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(axialColor);

  vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
  sphere->SetCenter(0, 0, 0);
  sphere->SetPhiResolution(24);
  sphere->SetThetaResolution(24);
  sphere->SetRadius(0.5);

  vtkSmartPointer<vtkPolyDataMapper> sphereMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
  sphereMapper->SetInput(sphere->GetOutput());

  sphereActor = vtkActor::New();
  sphereActor->SetMapper(sphereMapper);

  vtkSmartPointer<vtkPropAssembly> props = vtkSmartPointer<vtkPropAssembly>::New();
  props->AddPart(axesActor);
  props->AddPart(sphereActor);

  this->markerWidget = vtkOrientationMarkerWidget::New();
  this->markerWidget->SetOrientationMarker(props);
  this->markerWidget->SetViewport(0.7, 0.0, 1.2, 0.2);
}

/***********************************************************************************//**
 * 
 */
vtkmpj3DOrientationMarkerWidget::~vtkmpj3DOrientationMarkerWidget()
{
  this->markerWidget->Delete();
}

/***********************************************************************************//**
 * 
 */
void vtkmpj3DOrientationMarkerWidget::SetAxisLabels(int enable)
{
  this->axesActor->SetAxisLabels(enable);
}

/***********************************************************************************//**
 * 
 */
 void vtkmpj3DOrientationMarkerWidget::GetSphereColor(double color[3])
 {
  sphereActor->GetProperty()->GetColor(color);
 }

/***********************************************************************************//**
 * 
 */
 void vtkmpj3DOrientationMarkerWidget::SetSphereColor(double color[3])
 {
  sphereActor->GetProperty()->SetColor(color);
 }

/***********************************************************************************//**
 * 
 */
 void vtkmpj3DOrientationMarkerWidget::GetXAxisColor(double color[3])
 {
    axesActor->GetXAxisShaftProperty()->GetColor(color);
 }

/***********************************************************************************//**
 * 
 */
 void vtkmpj3DOrientationMarkerWidget::SetXAxisColor(double color[3])
 {
    axesActor->GetXAxisTipProperty()->SetColor(color);
    axesActor->GetXAxisShaftProperty()->SetColor(color);
    axesActor->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(color);
 }

/***********************************************************************************//**
 * 
 */
 void vtkmpj3DOrientationMarkerWidget::GetYAxisColor(double color[3])
 {
    axesActor->GetYAxisShaftProperty()->GetColor(color);
 }

/***********************************************************************************//**
 * 
 */
 void vtkmpj3DOrientationMarkerWidget::SetYAxisColor(double color[3])
 {
    axesActor->GetYAxisTipProperty()->SetColor(color);
    axesActor->GetYAxisShaftProperty()->SetColor(color);
    axesActor->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(color);
 }

 /***********************************************************************************//**
 * 
 */
 void vtkmpj3DOrientationMarkerWidget::GetZAxisColor(double color[3])
 {
    axesActor->GetZAxisShaftProperty()->GetColor(color);
 }

/***********************************************************************************//**
 * 
 */
 void vtkmpj3DOrientationMarkerWidget::SetZAxisColor(double color[3])
 {
    axesActor->GetZAxisTipProperty()->SetColor(color);
    axesActor->GetZAxisShaftProperty()->SetColor(color);
    axesActor->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(color);
 }

/***********************************************************************************//**
 * 
 */
void vtkmpj3DOrientationMarkerWidget::SetXAxisLabelText(const char *string)
{
  this->axesActor->SetXAxisLabelText(string);
}

/***********************************************************************************//**
 * 
 */
void vtkmpj3DOrientationMarkerWidget::SetYAxisLabelText(const char *string)
{
  this->axesActor->SetYAxisLabelText(string);
}

/***********************************************************************************//**
 * 
 */
void vtkmpj3DOrientationMarkerWidget::SetZAxisLabelText(const char *string)
{
  this->axesActor->SetZAxisLabelText(string);
}

/***********************************************************************************//**
 * 
 */
void vtkmpj3DOrientationMarkerWidget::SetInteractor(vtkRenderWindowInteractor *interactor)
{
  this->markerWidget->SetInteractor(interactor);
  this->markerWidget->EnabledOn();
  this->markerWidget->InteractiveOff();
}

/***********************************************************************************//**
 * 
 */
vtkRenderWindowInteractor *vtkmpj3DOrientationMarkerWidget::GetInteractor()
{
  return this->markerWidget->GetInteractor();
}

/***********************************************************************************//**
 * 
 */
void vtkmpj3DOrientationMarkerWidget::SetCurrentRenderer(vtkRenderer *renderer)
{
  this->markerWidget->SetCurrentRenderer(renderer);
}

/***********************************************************************************//**
 * 
 */
void vtkmpj3DOrientationMarkerWidget::SetEnabled(int enable)
{
	this->markerWidget->SetEnabled(enable);
}

/***********************************************************************************//**
 * 
 */
void vtkmpj3DOrientationMarkerWidget::PrintSelf(ostream& os, vtkIndent indent)
{
  this->markerWidget->PrintSelf(os, indent);
}
