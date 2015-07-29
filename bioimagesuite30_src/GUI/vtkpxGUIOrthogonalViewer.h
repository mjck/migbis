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





// .NAME vtkpxGUIOrthogonalViewer 
// .SECTION Description

#ifndef __vtkpxGUIOrthogonalViewer_h
#define __vtkpxGUIOrthogonalViewer_h

#include "vtkpxGUIBaseImageViewer.h"
#include "vtkPolyData.h"
#include "vtkEllipseSource.h"
#include "pxvtkgadgets.h"

#include "pxtkscale.h"
#include "pxtkframe.h"
#include "pxtkoptionmenu.h"
#include "pxtklabel.h"
#include "pxutil.h"

class vtkActor;
class vtkpxGUIRenderer;
class vtkpxImageSlice;
class vtkpxOrthoImageSlice;
class vtkpxVolume;
class vtkpxGUIVolumeControl;
class vtkpxGUIObliqueImageSliceControl;
class vtkOutlineFilter;
class vtkpxVolume;
class vtkpxObliqueImageSlice;
class vtkCamera;
class vtkGenericRenderWindowInteractor;
class vtkLineWidget;
class vtkConeSource;
class vtkCubeSource;
class vtkSphereSource;
class vtkCylinderSource;
class vtkTransformPolyDataFilter;
class vtkTransform;

class  vtkpxGUIOrthogonalViewer : public vtkpxGUIBaseImageViewer 
{
public:
  
  static vtkpxGUIOrthogonalViewer *New();
  vtkTypeMacro(vtkpxGUIOrthogonalViewer,vtkpxGUIBaseImageViewer);


  // Desription:
  // tk interface
  virtual char* Initialize(const char* name,int inside);
  virtual char* Initialize(const char* name,const char* name2,int inside);
  virtual char* GetRenderWidgetName();
  virtual char* GetControlFrameName();
  virtual char* GetViewerFrameName();
  virtual void  SetShowControls(int show=1);
  virtual void SetBackgroundColor(vtkRenderWindow* renwin);

  // Description:
  // Colormap Stuff --> Volume
  virtual void  SetPresetLookupTable(int mode);
  virtual void SetLookupTable(vtkLookupTable* lkp);

  // Description:
  // Offset for 0 offset or 1 offset
  virtual void  SetOffset(int off);

  // Description:
  // Create and Reset Viewer and Update
  virtual int   InitializeViewer();
  virtual int   ResetViewer();
  virtual int   UpdateDisplay();
  virtual void  Update() { UpdateDisplay();}

  // Description:
  // Event Loop Handling
  virtual int   HandleEvent(int eventno);
  virtual int   HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev);

  // Description:
  // Coordinate Changes
  virtual void  SetCoordinates(int lx,int ly,int lz,int infoonly=0);
  virtual void  SetScaledCoordinates(float lx,float ly,float lz,int infoonly);
  virtual void  SetScaledCoordinates(float lx,float ly,float lz) { SetScaledCoordinates(lx,ly,lz,0); }
  virtual void  SetCurrentFrame(int t);
  virtual void  ChangeFrame(int newframe);

  // Description:
  // Set Viewer Modes
  virtual void  AdjustViewports(int mode);
  virtual void  SetDisplayMode(int dm) { AdjustViewports(dm);}
  virtual int  GetDisplayMode();
  virtual int  GetDisplayMode3D();
  virtual void SetDisplayMode3D();
  virtual void SetDisplayMode3DIfNeeded();
  virtual void SetDisplayMode2D();
  virtual void ShowFrameControls(int show=1);
  virtual void UpdateKeyRenderer();


  // Description:
  // Display 3D Volume or axis outline
  virtual void SetDisplay3DMode(int dm);
  virtual void SetDisplay3DAxisOutlineMode(int mode);

  // Description:
  // Set Visibility for Volume Ortho and Oblique Slices
  virtual void SetOrthoSliceVisibility(int orthovis);
  virtual void SetVolumeVisibility(int volvis);
  virtual void SetObliqueSliceVisibility(int volvis);


  // Decription:
  // Enable/Disable Mouse updates
  virtual void SetMouseSliceChangeMode(int m);
  vtkGetMacro(MouseSliceChangeMode,int);

  // Description:
  // Get access to internal stuff
  virtual vtkpxGUIRenderer*     GetRenderer(int i);
  virtual vtkpxImageSlice*      GetImageSlice(int i);
  virtual vtkpxObliqueImageSlice*  GetObliqueImageSlice();
  virtual vtkpxGUIObliqueImageSliceControl*  GetObliqueControl();
  virtual vtkpxGUIVolumeControl*     GetVolumeControl();
  virtual vtkpxOrthoImageSlice* GetOrthoImageSlice();
  virtual vtkActor*             GetOutlineActor(int i);
  virtual vtkActor*             GetAxisActor(int i) { return axisActor[Irange(i,0,9)]; }

  // Description:
  // View Synchronization
  virtual vtkCamera*            GetCurrentCamera();
  virtual int                   CopyCamera(vtkpxGUIOrthogonalViewer* other);
  virtual int                   CopyCamera(vtkCamera* camera);
  
  
  // Description:
  // Tracing/Clicking Interface
  vtkGetMacro(TraceVolume,int);
  vtkSetMacro(TraceVolume,int);


  vtkGetMacro(TraceVolumeVOI,int);
  vtkSetClampMacro(TraceVolumeVOI,int,0,40);
  vtkSetClampMacro(TraceVolumeWeightedMode,int,0,1);
  vtkGetMacro(TraceVolumeWeightedMode,int);

  vtkGetMacro(TraceGeometry,int);
  vtkSetMacro(TraceGeometry,int);
  vtkGetMacro(TraceVolumeToleranceMode,int);
  vtkSetMacro(TraceVolumeToleranceMode,int);

  vtkGetMacro(LastClickedSlice,int);

  
  // Description:
  // Show Cross Hairs and Update stuff
  virtual void ShowCrossHairs(int mode);
  virtual void MoveAxis(int force=0);

  // Description:
  // Event Response from Widget
  vtkSetClampMacro(EventResponseFromWidget,int,0,1);
  vtkGetMacro(EventResponseFromWidget,int);


  // Description:
  // Sync Widget With Cross Hairs
  vtkSetClampMacro(SynchNavigationalPointerWithCrossHairs,int,0,1);
  vtkGetMacro(SynchNavigationalPointerWithCrossHairs,int);

  // Description:
  // Modes
  // 0 = show nothing
  // 1 = show line widget only
  // 2 = show decoration only
  // 3 = show line widget + decoration wireframe
  // 4 = show decoration wireframe only
  vtkSetClampMacro(NavigationPointerDrawMode,int,0,4);
  vtkGetMacro(NavigationPointerDrawMode,int);
  // Setting dm will set NavigationPointerDraw Mode (-1 stays as is)
  virtual int RenderNavigationPointer(int dm=-1);

  // Description:
  // Force Surgical Mode Pointer
  virtual int SetNavigationalPointerFromTool(vtkPoints* pts  );
  virtual int SetNavigationalPointerFromTool(vtkTransform* tr);
  virtual int SetNavigationalPointerFromTool(double tip[3],double base[3]);
  

  // Tool Position
  vtkGetVectorMacro(NavigationPointerTip,double,3);
  vtkGetVectorMacro(NavigationPointerBase,double,3);


  // Description:
  // Access the Line Widget
  vtkGetObjectMacro(NavigationPointer,vtkLineWidget);

  // Description:
  // Navigational Pointer Actor
  // Mode 0 = Cone, 1 = Sphere, 2= Cube, 3= Cylinder, 4=Double Sphere
  vtkGetMacro(NavigationPointerActorMode,int);
  virtual void SetNavigationPointerGeometryToCone(double radius);
  virtual void SetNavigationPointerGeometryToSphere(double radius);
  virtual void SetNavigationPointerGeometryToDoubleSphere(double radius1, double radius2);
  virtual void SetNavigationPointerGeometryToCube(double cubelength);
  virtual void SetNavigationPointerGeometryToEllipse(double radius_x, double radius_y, double radius_z);
  virtual void SetNavigationPointerGeometryToCylinder(double radius);
  vtkGetObjectMacro(NavigationPointerActor,vtkActor);
  vtkGetObjectMacro(NavigationPointerActor2,vtkActor);
  
protected:

  vtkpxGUIOrthogonalViewer();
  virtual ~vtkpxGUIOrthogonalViewer();

  vtkpxGUIRenderer*         renderer[4];
  vtkRenderer*              BackgroundRenderer;
  PXVTkTkRenderWidget*      render_widget;
  vtkpxImageSlice*          imageSlice[3];
  vtkpxObliqueImageSlice*   obliqueSlice;

  vtkpxOrthoImageSlice       *orthoSlice;
  vtkActor*                  axisActor[10];
  vtkOutlineFilter*          outlineFilter[4];
  vtkActor*                  outline[4];
  vtkpxVolume*               Volume;

  vtkpxGUIVolumeControl*     VolumeControl;
  vtkpxGUIObliqueImageSliceControl*    ObliqueControl;

  int                        display_mode;
  int                        display3d_mode;
  int                        display3d_axisoutlinemode;
  int                        offset;
  int                        OrthoSliceVisibility;
  int                        MouseSliceChangeMode;
  int                        show_controlframe;
  int                        TraceVolume;
  int                        TraceVolumeVOI;
  int                        TraceVolumeWeightedMode;
  int                        TraceGeometry;
  int                        TraceVolumeToleranceMode;
  int                        EnableFrameChangeOnCallback;
  int                        AutoOpacity;


  PXTkOptionMenu*           displayMode;
  PXTkOptionMenu*           display3DMode;
  PXTkOptionMenu*           display3DAxisOutlineMode;
  PXTkFrame*                viewerFrame;
  PXTkFrame*                viewerFrame2;
  PXTkFrame*                controlFrame;
  PXTkFrame*                extraFrame;
  PXTkFrame*                coordScaleFrame;

  PXTkCheckButton*          showaxisc;
  PXTkCheckButton*          interpolateMode;
  PXTkCheckButton*          showPointer;
  PXTkCheckButton*          freezeMode;
  PXTkArrowScale            *coordScale[4];
  PXTkButton*               saveAllButton;


  PXTkLabel*                talairachlabel;
  PXTkLabel*                mnilabel;


  // Description:
  // Initialization Stuff 
  virtual void InitControls(int hm=1);
  virtual void InitViewer();


  // Description:
  // Picking Stuff 
  int VolumePick(int clickx, int clicky, vtkRenderer* pick_renderer);
  int GeometryPick(int clickx, int clicky, vtkRenderer* pick_renderer);

  // Description:
  // Change Coordinates Frame Stuff 
  virtual void ChangeSliceLevels(int lv[3],int infoonly=0);
  virtual void SetInterpolation(int interp);


  // Description:
  // Polar and Cartesian Stuff
  virtual int ResetCoordinateScalesAndLabels();
  virtual int ModifyPolarImageSpacing();


  virtual int ResetViewerUpdatePolar();
  virtual int ResetViewerUpdateCartesian();
  virtual int MoveAxisPolar(int force);
  virtual int MoveAxisCartesian(int force);
  virtual int UpdateIntensityLabelInfo();

  // Description:
  // Find Centroid of clicked volume
  virtual int FindCentroid(vtkImageData* img,int location[3],double lvv[3],int radius);

  // Description:
  // Some Flags which used to be static 
  //BTX
  int inexpose,inexposecount,oldorientation,firsttimereset;
  int LastClickedSlice;
  //ETX

  // Description:
  // Interactor and LineWidget Stuff
  vtkLineWidget*  NavigationPointer;
  vtkGenericRenderWindowInteractor* WidgetInteractor;
  int EventResponseFromWidget;
  int SynchNavigationalPointerWithCrossHairs;


  // Descrition:
  // Mode = 0 - Sphere, 1 - Cube, 2 - Ellipse, 3 - Cylinder, 4 - Double sphere
  int NavigationPointerActorMode;

  // Description:
  // Cone/Ellipse Radius
  double NavigationRadius;
  double NavigationRadius2;
  double NavigationRadius_X;
  double NavigationRadius_Y;
  double NavigationRadius_Z;


  // Description:
  // Cube Length
  double NavigationLength;

  // Description:
  // The underlying objects for the pointero
  vtkPolyData* NavigationPointerGeometry;
  vtkPolyData* NavigationPointerGeometry2;
  vtkActor* NavigationPointerActor;
  vtkActor* NavigationPointerActor2;
  vtkConeSource*     NavigationConeSource;
  vtkSphereSource*   NavigationSphereSource;
  vtkSphereSource*   NavigationSphereSource2;
  vtkCubeSource*     NavigationCubeSource;
  vtkEllipseSource*  NavigationEllipseSource;
  vtkCylinderSource* NavigationCylinderSource;
  vtkTransformPolyDataFilter* NavigationCylinderFilter;
  vtkTransform* NavigationCylinderTransform;
  double NavigationPointerTip[3];
  double NavigationPointerBase[3];
  // 
  int           NavigationPointerDrawMode;
  
  // Description:
  // Pointer Stuff
  virtual int CreateNavigationPointer();
  virtual int MoveNavigationPointer();
  virtual int SendEventToWidgetInteractor(int x,int y,int button,int state);
  virtual int UpdateNavigationActorGeometry(int updatevolumecrop=0);

  // Description:
  // Next Gen Renderer Name Stuff
  virtual void SetRendererName(vtkpxGUIRenderer* ren,const char* basename,
			       int axis,int dmindex=-1);
};


#endif   


