//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
//BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
//BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
//BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
//BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
//BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
//BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//BIOIMAGESUITE_LICENSE  See also  http://www.gnu.org/licenses/gpl.html
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
//BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
//BIOIMAGESUITE_LICENSE 
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------





// .NAME vtkpxGUIRenderer - create hierarchies of actors
// .SECTION Description
#ifndef __vtkpxGUIRenderer_h
#define __vtkpxGUIRenderer_h

#include "vtkpxGUIBaseRenderer.h"
#include "pxtkbutton.h"
#include "pxtkoptionmenu.h"
#include "pxtkscale.h"
#include "pxtkcheckbutton.h"
#include "vtkCamera.h"
#include "vtkImageData.h"
#include "vtkRenderWindow.h"
#include "vtkLight.h"
#include "vtkProperty2D.h"
#include "pxtkcomplexwidgets.h"
#include "vtkpxCameraInfo.h"
#include "vtkPlane.h"
#include "vtkmpj3DOrientationMarkerWidget.h"
#include "vtkGenericRenderWindowInteractor.h"

//BTX
class PXITkComboBox;
class vtkpxGUIRendererCallback;
class vtkTransform;
//ETX

class  vtkpxGUIRenderer : public vtkpxGUIBaseRenderer
{
public:
  static vtkpxGUIRenderer *New();
  vtkTypeMacro(vtkpxGUIRenderer,vtkpxGUIBaseRenderer);
  
  virtual char* Initialize(const char* widgetname,vtkRenderer*, int inside);
  virtual vtkRenderer* GetRenderer();

  // Description
  // Orientation marker related
  virtual void   EnableOrientationMarker(int enable);
  virtual void   CreateOrientationMarker();

  // Description:
  // Bounds related
  virtual void   SetBounds(double bd[6]);
  virtual void   SetBounds(vtkDataSet* dataset);
  virtual void   GetBounds(double bd[6]);
  virtual void   SetClipBounds(double x,double y,double z);
  virtual void   SetClipBounds(int x,int y,int z,double sp[3],double ori[3]);

  // Description:
  // Rendering
  virtual void   SetViewport(double x0,double y0,double x1,double y1);
  virtual int    InViewport(int mousex,int mousey);
  virtual void   Render();
  virtual void   RenderDelay(int a=-1);
  virtual void   SetUpdateRate(double interactiverate=15.0,double staticrate=0.1);
  virtual void   Zoom(double scale=1.0,PXBool dorender=PXTrue);
  virtual void   Rotate(double angle=5.0,int doazimuth=1);
  virtual void   Rotate(double angle,int doazimuth,PXBool dorender);
  virtual void   MoveAbove(int axis=-1);
  virtual void   Clip(int axis=-1);
  virtual void   View(int axis=-1);
  virtual void   Reset();
  virtual int    SaveAsTIFF(const char* fname,PXBool wholewindow=PXFalse);  



  // Description:
  // Event Handling
  virtual int    HandleEvent(int event);
  virtual int    HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev);
  virtual int    Get2DClickedPoint(double& px,double& py,int mousex,int mousey,int orient=-1);

  // Description:
  // Access Internal Stuff
  virtual int    GetFlipMode();
  virtual int    GetPlaneMode();
  virtual void   SetFlipMode(int mode=0);
  virtual void   SetOrientation(int orient=0,int doupdate=1);
  virtual int    GetOrientation();

  virtual void   SetClipMode(PXBool mode=PXTrue);
  virtual void   SetPlaneMode(int plane=2);
  virtual void   SetAutoZoom(double autoz=1.0);

  // Description:
  // Enable Actor Transform --> flip left/right in Sagital Images for Volume Rendering etc
  vtkSetMacro(EnableActorTransform,int);
  vtkGetMacro(EnableActorTransform,int);


  // Description:
  // Reset Camera Stuff 
  virtual void ResetCamera(double bounds[6]);
  virtual void ResetCamera();

  // Description:
  // Reset Camera Clipping Range
  virtual void ResetCameraClippingRange(double bounds[6]);
  virtual void ResetCameraClippingRange();

  // Description:
  // Callback On Camera Modified
  virtual void CameraModifiedCallback();

  // Description:
  // Camera Modified Callback
  virtual void   SetCameraCallback(vtkpxGUIComponent* comp,int callno);
  virtual void   SetCameraCallback(const char* CallbackName);

  // Description:
  // Copy Camera Attributes
  static void CopyCamera(vtkCamera* newCamera,vtkCamera* oldCamera);
  static void InterpolateCamera(vtkCamera* newCamera,vtkCamera* oldCamera,vtkCamera* oldCamera2,double weight);

  // Description:
  // More Camera Stuff
  virtual double  GetCameraScale();
  virtual double  GetCameraWidth();
  virtual void   SetCameraScale(double autoz=1.0);
  virtual void   SetCameraScale(vtkpxGUIRenderer* ren);
  virtual double  GetClipFront();
  virtual double  GetClipThickness();
  virtual void   SetClipFront(double fr=-1.0);
  virtual void   SetClipThickness(double thick);

  // Description:
  // Label Stuff
  virtual void   SetLabel(const char* text,double x,double y,double c1=1.0,double c2=0.0,double c3=0.0);
  virtual void   HideLabel();
  virtual void   ShowLabel();


  // Description:
  // GUI 
  virtual void   SetVerticalDecoration(PXBool dec);
  virtual void   SetNoGUI(PXBool gui=PXTrue);

  // Description:
  // Set Polar Angle coordinates in case of polar images 
  virtual void   SetViewerCoordinates(int x,int y,int z)
  { this->ViewerCoordinates[0]=x; this->ViewerCoordinates[1]=y; this->ViewerCoordinates[2]=z; }

  // Description:
  // Renderer Event --- the renderer may invoke this prior to rendering
  virtual void RendererEvent();
  
  // Description:
  // Set/Get Current Image
  virtual void SetCurrentImageOrientation(vtkImageData* img,int orientation=-1,int doupdate=1);
  vtkGetObjectMacro(CurrentImage, vtkImageData);

  // Description:
  // Clip Stuff
  virtual int  GetClipScaleValue(int i);
  virtual void SetClipScaleValue(int i,int v);
  virtual void ResetClipScales();
  virtual void SetPolygonalClipping(int mode);
  virtual void UpdateClippingPlanes();


  // Description:
  // Hide PolyClip Controls
  vtkSetClampMacro(NoPolyClipControls,int,0,1);
  vtkGetMacro(NoPolyClipControls,int);

  // Description:
  // Show Camera Controls
  virtual int ShowCameraControls();


protected:

  vtkpxGUIRenderer();
  virtual        ~vtkpxGUIRenderer();
  
  // Hide this one so that no-one can access it
  virtual char* Initialize(const char* widgetname,int inside) {return NULL;}
  static void   HandleRenderEvent(void *arg1);

  vtkRenderer*       renderer;
  vtkActor2D*        textActor;
  vtkLight*          main_light;

  virtual void moveAbovePolar(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis);

  virtual void moveAboveSagittal(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis);
  virtual void moveAboveAxial(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis);
  virtual void moveAboveCoronal(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis);

  virtual void moveAboveTransposedSagittal(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis);
  virtual void moveAboveTransposedAxial(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis);
  virtual void moveAboveTransposedCoronal(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis);

  // Description:
  // Handle Mouse Events --from TkInteractor.tcl
  // -------------------------------------------
  virtual int MouseEventRotate(int state,int x1,int x2);
  virtual int MouseEventPan(int state,int x1,int x2);
  virtual int MouseEventZoom(int state,int x1,int x2);

  int                EnableActorTransform;
  int                flip_mode;
  PXBool	     clip_mode;
  PXBool	     vertical_decoration;
  PXBool	     no_gui;
  int                plane_mode;
  int		     last_axis;
  PXTkButton*        plane_button;
  PXTkButton*	     mode_button;
  PXTkOptionMenu*    mode_menu;
  PXTkOptionMenu*    angle_menu;
  // mpj
  PXTkOptionMenu*    camera_menu;
  int camera_mode;
  // mpj

  PXITkComboBox*     plane_menu;
  PXTkButton*        update_plane;
  //  PXTkOptionMenu*    plane_menu;
  PXTkArrowScale     *clip_min;
  PXTkArrowScale     *clip_thickness;

  int                NoPolyClipControls;
  PXTkArrowScale     *clip_planes[6];
  // mpj
  PXTkArrowScale     *ambient[3];
  PXTkArrowScale     *diffuse[3];
  PXTkArrowScale     *specular[3];
  PXTkArrowScale     *light_intensity;
  // mpj
  PXTkCheckButton    *enableClip;

  PXTkCheckButton    *enablemarker;
  PXTkCheckButton    *enablelabels;
  PXTkEntry          *labelx;
  PXTkEntry          *labely;
  PXTkEntry          *labelz;

  int                hasclipcontrols;
		  
  int                orientation;
  double              clipping_thickness;
  double              clipping_front;
  double              bounds[6];
  double              clipBounds[3];
  double              update_rate[2];
  double              camera_scale;
  double              auto_zoom;


  vtkImageData*      CurrentImage;
  int                ViewerCoordinates[3];

  // Callback Stuff
  int                CameraCallbackNo;
  char*              CameraCallbackName;
  vtkpxGUIComponent* CameraEventParent;	  

  // Camera Info
  vtkpxCameraInfo*          CameraPresets;
  vtkpxGUIRendererCallback* RendererCallback;
  vtkTransform*             ActorTransform;
  vtkPlane*                 ClippingPlanes[6];

  vtkmpj3DOrientationMarkerWidget *markerWidget;

  // Description:
  // Set Current Image Stuff 
  vtkSetObjectMacro(CurrentImage, vtkImageData);


  
};
#endif

