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




#include "vtkLineWidget.h"
#include "vtkpxGUIColormapEditor.h"

#include "pxtkframe.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkCellPicker.h"
#include "vtkCubeSource.h"
#include "vtkDataSetMapper.h"
#include "vtkImageDataGeometryFilter.h"
#include "vtkImageSeedConnectivity.h"
#include "vtkImageThreshold.h"
#include "vtkLineSource.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkOutlineFilter.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProbeFilter.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"
#include "vtkSphereSource.h"
#include "vtkConeSource.h"
#include "vtkCubeSource.h"
#include "vtkCylinderSource.h"
#include "vtkThresholdPoints.h"
#include "vtkpxGUIObliqueImageSliceControl.h"
#include "vtkpxGUIOrthogonalViewer.h"
#include "vtkpxGUIRenderer.h"
#include "vtkpxGUIVolumeControl.h"
#include "vtkpxImageExtract.h"
#include "vtkpxImageExtractVOI.h"
#include "vtkpxImageSlice.h"
#include "vtkpxObliqueImageSlice.h"
#include "vtkpxOrthoImageSlice.h"
#include "vtkpxTalairachTransform.h"
#include "vtkpxVolume.h"
#include "vtkGenericRenderWindowInteractor.h"
#include "vtkCommand.h"
#include "vtkConeSource.h"
#include "vtkInteractorStyle.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTransform.h"

//#include "vtkInteractorStyleTrackballCamera.h"
//#include "vtkmpj3DOrientationMarkerWidget.h"
// -------------------------------------------------------------------------
// A Quick Helper Class
// -------------------------------------------------------------------------


// This does the actual work: updates the probe.
// Callback for the interaction
class vtkmyPWCallback : public vtkCommand
{
public:

  static vtkmyPWCallback *New()   { return new vtkmyPWCallback; }
  virtual void Execute(vtkObject *caller, unsigned long, void*)
  {
    this->Viewer->SetEventResponseFromWidget(1);
  }

  vtkpxGUIOrthogonalViewer *Viewer;
};


// -------------------------------------------------------------------------
vtkpxGUIOrthogonalViewer* vtkpxGUIOrthogonalViewer::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIOrthogonalViewer");
  if(ret)
      {
	return (vtkpxGUIOrthogonalViewer*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIOrthogonalViewer;
}

// Construct object with no children.
vtkpxGUIOrthogonalViewer::vtkpxGUIOrthogonalViewer()
{
  this->showPointer=NULL;
  this->viewerFrame=NULL;
  this->viewerFrame2=NULL;
  this->controlFrame=NULL;
  this->show_controlframe=1;


  this->display_mode=6;
  this->display3d_mode=1;
  this->display3d_axisoutlinemode=1;

  this->offset=0;

  this->OrthoSliceVisibility=1;

  this->orthoSlice=NULL;
  this->render_widget=NULL;
  this->MouseSliceChangeMode=1;
  this->Volume=NULL;

  this->VolumeControl=NULL;
  this->ObliqueControl=NULL;

  this->AutoOpacity=1;

  this->createTalairachButtons=PXTrue;
  for (int i=0;i<=2;i++)
    imageSlice[i]=NULL;
  this->obliqueSlice=NULL;

  for (int j=0;j<=3;j++)
    {
      renderer[j]=NULL;
      outline[j]=NULL;
      outlineFilter[j]=NULL;
    }
  this->BackgroundRenderer=NULL;  

  
  for (int k=0;k<10;k++)
    axisActor[k]=NULL;

  render_widget=NULL;

  // If not initialized in BaseImageViewer
  if (this->talairachTransform==NULL)
    this->talairachTransform=vtkpxTalairachTransform::New();

  this->TraceVolume=1;
  this->TraceVolumeVOI=0;
  this->TraceVolumeWeightedMode=0;
  this->TraceGeometry=0;
  this->TraceVolumeToleranceMode=1;


  this->oldorientation=-1;
  this->firsttimereset=1;
  this->inexpose=0;
  this->inexposecount=0;
  this->LastClickedSlice=2;

  this->NavigationPointer=NULL;
  this->WidgetInteractor=NULL;
  this->EventResponseFromWidget=0;
  this->SynchNavigationalPointerWithCrossHairs=0;
  this->NavigationRadius=4;
  this->NavigationRadius2=4;
  this->NavigationRadius_X= this->NavigationRadius_Y= this->NavigationRadius_Z= 10; 
  this->NavigationPointerDrawMode=0;

  this->NavigationPointerActor=NULL;
  this->NavigationPointerActor2=NULL;

  this->NavigationPointerGeometry=NULL;
  this->NavigationPointerGeometry2=NULL;

  this->NavigationPointerActorMode=0;
  this->NavigationConeSource=NULL;

  this->NavigationSphereSource=NULL;
  this->NavigationSphereSource2=NULL;

  this->NavigationCubeSource=NULL;
  this->NavigationEllipseSource=NULL;
  this->NavigationCylinderSource=NULL;
  this->NavigationCylinderFilter=NULL;
  this->NavigationCylinderTransform=NULL;

  this->NavigationPointerTip[0]=0;   this->NavigationPointerTip[1]=0;   this->NavigationPointerTip[2]=0; 
  this->NavigationPointerBase[0]=0;   this->NavigationPointerBase[1]=-30.0;   this->NavigationPointerBase[2]=0; 
}

vtkpxGUIOrthogonalViewer::~vtkpxGUIOrthogonalViewer()
{

  for (int i=0;i<=2;i++)
    if (imageSlice[i]!=NULL)
      imageSlice[i]->Delete();

  if (orthoSlice!=NULL)
    orthoSlice->Delete();

  if (obliqueSlice!=NULL)
    {
      obliqueSlice->Delete();
      this->ObliqueControl->Delete();
    }

  if (this->Volume!=NULL)
    {
      this->Volume->Delete();
      this->VolumeControl->Delete();
    }


  if (render_widget!=NULL)
    delete render_widget;

  for (int j=0;j<=3;j++)
    {
      if (renderer[j]!=NULL) renderer[j]->Delete();
      if (outline[j]!=NULL) outline[j]->Delete();
      if (outlineFilter[j]!=NULL) outlineFilter[j]->Delete();

    }
  
  for (int k=0;k<10;k++)
    if (axisActor[k]!=NULL)
      axisActor[k]->Delete();


  if (this->NavigationPointer!=NULL)
    this->NavigationPointer->Delete();

  if (this->WidgetInteractor!=NULL)
    this->WidgetInteractor->Delete();

  if (this->NavigationPointerActor!=NULL)
    this->NavigationPointerActor->Delete();

  if (this->NavigationPointerGeometry!=NULL)
    this->NavigationPointerGeometry->Delete();

 if (this->NavigationPointerActor2!=NULL)
    this->NavigationPointerActor2->Delete();

  if (this->NavigationPointerGeometry2!=NULL)
    this->NavigationPointerGeometry2->Delete();

  if (this->NavigationConeSource!=NULL)
    this->NavigationConeSource->Delete();

  if (this->NavigationSphereSource!=NULL)
  this->NavigationSphereSource->Delete();

  if (this->NavigationSphereSource2!=NULL)
  this->NavigationSphereSource2->Delete();
  
  if (this->NavigationCubeSource!=NULL)
    this->NavigationCubeSource->Delete();

  if (this->NavigationCylinderSource!=NULL)
    this->NavigationCylinderSource->Delete();

  if (this->NavigationCylinderFilter!=NULL)
    this->NavigationCylinderFilter->Delete();

  if (this->NavigationCylinderTransform!=NULL)
    this->NavigationCylinderTransform->Delete();



}
/* -------------------------------------------------------------------------*/

char* vtkpxGUIOrthogonalViewer::Initialize(const char* name,int inside)
{
  return Initialize(name,name,inside);
}

char* vtkpxGUIOrthogonalViewer::Initialize(const char* name,const char* name2,int inside)
{
  if (this->Initialized==1)
    return GetWidgetName();
  
  

  vtkpxGUIComponent::Initialize(name,inside);


  PXTkFrame* manager=(PXTkFrame*)(this->EventManager->getMainWindow());
  viewerFrame=new PXTkFrame(this->EventManager,manager);
  manager->setPackMode(PXTrue);
  manager->addChildren("-side left -expand true -fill both",viewerFrame);
      
  if (strcmp(name,name2)==0)
    {
      viewerFrame2=viewerFrame;
    }
  else
    {
      PXTkFrame* manager2=new PXTkFrame(NULL,name2);
      manager2->setPackMode(PXTrue);
      viewerFrame2=new PXTkFrame(this->EventManager,manager2);
      manager2->addChildren("-side left -expand true -fill both",viewerFrame2);
    }

  InitControls(0);
  InitViewer();

  /* vol Commented*/
  this->VolumeControl=vtkpxGUIVolumeControl::New();
  this->VolumeControl->Initialize(viewerFrame2->getWidgetName(),0);
  this->VolumeControl->GetEventManager()->setTitle("Volume Control"); 
  this->VolumeControl->SetOrthoViewer(this);

  this->ObliqueControl=vtkpxGUIObliqueImageSliceControl::New();
  this->ObliqueControl->Initialize(viewerFrame2->getWidgetName(),0);
  this->ObliqueControl->GetEventManager()->setTitle("Oblique Slice Control"); 
  
  AdjustViewports(-1);
  
  return GetWidgetName();
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::UpdateDisplay()
{
  if (inexpose==1 || this->EnableRendering==0)
    {
      ++inexposecount;
      return TCL_OK;
      }
 
  inexpose=1;
  PXTkApp::executeTclCommand("update idletasks");
  render_widget->getRenderWindow()->Render();  
  this->LastEvent=-1;
  inexposecount=0;
  inexpose=0;
  return TCL_OK;
}


// -------------------------------------------------------------------------

int vtkpxGUIOrthogonalViewer::HandleEvent(int event)
{
  if (event>0 && event<10)
      return this->EventManager->handleDirectEvent(event);


  if (!hasImage && event!=201)
    return TCL_OK;

  switch (event)
      {
      case 200:
	{
	  int* x=render_widget->getRenderWindow()->GetSize();
	  int mouse=PXTkApp::getIntArgument(0);
	  int state=PXTkApp::getIntArgument(1);
	  int x1=PXTkApp::getIntArgument(2);
	  int x2=x[1]-PXTkApp::getIntArgument(3);
	  HandleMouseButtonEvent(mouse,state,x1,x2,this->EventManager);
	}
	break;
	
      case 201:
	if (this->LastEvent!=201)
	  {
	    this->RenderDelay(200);
	  }
	break;
	
      case 300: // Interpolation;
	{
	  PXBool interp=interpolateMode->getState();
	  this->SetInterpolation((int)interp);
	  UpdateDisplay();
	}
	 break;


      case 301:
	  {
	    int cf=int(coordScale[3]->getValue()-1);
	    this->ChangeFrame(cf);
	    this->UpdateDisplay();
	  }
	break;

	
      case 302: // Coordinate Change
	{
	    int lv[3];
	    double lvv[3],lvs[3];
	    double   sp[3],ori[3];
	    currentImage->GetSpacing(sp);
	    currentImage->GetOrigin(ori);
	    
	    for (int ia=0;ia<=2;ia++)
	      {
		lv[ia]=int(coordScale[ia]->getValue()-offset);
		lvv[ia]=double(lv[ia]);
		lvs[ia]=lvv[ia]*sp[ia]+ori[ia];
	      }
	    this->ChangeSliceLevels(lv);
	    this->SetLastClickedPoint(lvv);
	    this->SetLastClickedPointScaled(lvs);
	    
	    this->ExecuteCoordinatesChangeCallback();
	    //this->UpdateDisplay();
	  }
	break;

	
      case 303: // Colormap
	if (this->ColormapEditor!=NULL)
	  this->ColormapEditor->Show();
	break;
	
	
      case 304:
      case 305: // Zoom
	if (event==305)
	  {
	    renderer[0]->Zoom(renderer[0]->GetCameraScale()*1.2,PXFalse);
	    renderer[3]->Zoom(renderer[0]->GetCameraScale()*1.2,PXFalse);
	  }
	else
	  {
	    renderer[0]->Zoom(renderer[0]->GetCameraScale()*0.833,PXFalse);
	    renderer[3]->Zoom(renderer[0]->GetCameraScale()*1.2,PXFalse);
	  }
	
	for (int kk=1;kk<3;kk++)
	  renderer[kk]->Zoom(renderer[0]->GetCameraScale(),PXFalse);
	
	 
	UpdateDisplay();
	break;
	
      case 306: // Reset
	for (int i=0;i<=3;i++)
	  renderer[i]->Reset();
	
	this->MoveAxis(1);
	UpdateDisplay();
	break;

      case 307:
	display_mode=(int)displayMode->getIndex();
	AdjustViewports(display_mode);
	break;

      case 308:
	if (freezeMode->getState())
	  this->MouseSliceChangeMode=1;
	else
	  this->MouseSliceChangeMode=0;
	break;


      case 309:
      case 310:
      case 311:
      case 312:
      case 313:
	this->SetPresetLookupTable(event-310);
	break;

      case 314:
	this->SetPresetLookupTable(6);
	break;
	
      case 315:
	this->SaveAsTiff(render_widget->getRenderWindow());
	break;

      case 316:
	this->SaveAllAsTiff(render_widget->getRenderWindow());
	break;

      case 317:
	{
	  if (showaxisc->getState()==PXTrue)
	    this->ShowCrossHairs(1);
	  else
	    this->ShowCrossHairs(0);
	}
	break;

      case 318:
	if (this->NavigationPointer!=NULL)
	  {
	    // Pointer
	    if (showPointer->getState()==PXTrue)
	      {
		this->SynchNavigationalPointerWithCrossHairs=1;
		if (this->NavigationPointerDrawMode==0)
		  this->RenderNavigationPointer(3);
		else
		  this->RenderNavigationPointer(-1);
	      }
	    else
	      {
		this->SynchNavigationalPointerWithCrossHairs=0;
		this->RenderDelay(200);
	      }
	  }
	break;

      case 320:
	if (this->talairachTransform==NULL)
	  this->talairachTransform=vtkpxTalairachTransform::New();
	this->talairachTransform->SetColinMode(1,this->currentImage);
	this->talairachTransform->Modified();
	this->MoveAxis(1);
	break;

      case 322:
	if (this->talairachTransform==NULL)
	  this->talairachTransform=vtkpxTalairachTransform::New();

	this->talairachTransform->SetColinMode(-1,this->currentImage);
	this->talairachTransform->Modified();
	this->MoveAxis(1);
	break;



      case 330: // Show Volume Controls
	if (this->VolumeControl!=NULL)
	{
	  this->VolumeControl->Show(1);
	}
	break;


      case 333: // Show Volume Controls
	if (this->ObliqueControl!=NULL)
	  this->ObliqueControl->Show(1);
	break;


      case 331:
	if (display_mode == 2 || display_mode ==7 || display_mode==6)
	  {
	    display3d_mode=(int)display3DMode->getIndex();
	    if (this->Orientation==3 && display3d_mode>=2)
	      {
		display3DMode->setIndex(1);
		display3d_mode=1;
	      }
	    SetDisplay3DMode(display3d_mode);
	    this->UpdateDisplay();
	  }
	else {
	  display3DMode->setIndex(display3d_mode);
	}
	break;

      case 332:
	display3d_axisoutlinemode=(int)display3DAxisOutlineMode->getIndex();
	SetDisplay3DAxisOutlineMode(display3d_axisoutlinemode);
	this->UpdateDisplay();
	break;

  case 345:
  this->ExportAsX3D(render_widget->getRenderWindow(), renderer[3]->GetRenderer());
  break;
	
      case 350: // Hide Widget
	SetShowControls(1-this->show_controlframe);
	break;
      }

  this->LastEvent=event;

  return TCL_OK;
}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::SetCurrentFrame(int t)
  {
  if (this->CurrentImageIsColor)
    return;

  if (t<0)
    t=this->NumberOfFrames-1;
  if (t>this->NumberOfFrames-1)
    t=0;

  if (!this->Initialized)
    return;

  coordScale[3]->setValue(t+1);
  this->ChangeFrame(t);

  this->UpdateDisplay();
}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::SetPresetLookupTable(int mode)
{
  if (!hasImage)
    return;

  vtkpxGUIBaseImageViewer::SetPresetLookupTable(mode);
  if (this->VolumeControl!=NULL && this->Orientation!=3)
    this->VolumeControl->SetPresetLookupTable(this->PresetLookupTableMode);
  
}
// -------------------------------------------------------------------------
void  vtkpxGUIOrthogonalViewer::SetCoordinates(int lx,int ly,int lz,int infoonly)
{
  if (this->Initialized==0 || !hasImage)
    return;
  
  int lv[3];
  lv[0]=lx; lv[1]=ly; lv[2]=lz;
			
  for (int ia=0;ia<=2;ia++)
    coordScale[ia]->setValue(lv[ia]+offset);

  this->ChangeSliceLevels(lv,infoonly);
}
// -------------------------------------------------------------------------
void  vtkpxGUIOrthogonalViewer::SetScaledCoordinates(float lx,float ly,float lz,int infoonly)
{
  if (this->Initialized==0 || !hasImage)
    return;
  
  float lvv[3];
  lvv[0]=lx; lvv[1]=ly; lvv[2]=lz;
		
  int lv[3];

  double   sp[3],ori[3];
  currentImage->GetSpacing(sp);
  currentImage->GetOrigin(ori);
  
  for (int ic=0;ic<=2;ic++)
    lv[ic]=int(0.5+ (lvv[ic]-ori[ic])/sp[ic]);

  this->SetCoordinates(lv[0],lv[1],lv[2],infoonly);
  this->UpdateDisplay();
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::SendEventToWidgetInteractor(int x,int y,int button,int state)
{

  if (this->WidgetInteractor==NULL)
    return 0;



  int cntrl=0;
  int shift=0;

  if (button>3 && button<=6)
    {
      shift=1;
      button=button-3;
    }
  else if (button>6)
    {
      cntrl=1;
      button=button-6;
    }


  this->WidgetInteractor->SetEventPosition(x,y);
  this->WidgetInteractor->SetControlKey(cntrl);
  this->WidgetInteractor->SetShiftKey(shift);
  this->WidgetInteractor->SetRepeatCount(0);

  if (state==1)
    {
      this->WidgetInteractor->MouseMoveEvent();
    }
  else if (button == 1 )
    {
      if (state==0)
	this->WidgetInteractor->LeftButtonPressEvent();
      else if (state==2)
	this->WidgetInteractor->LeftButtonReleaseEvent();
    }
  else if (button == 2 )
    {
      if (state==0)
	this->WidgetInteractor->MiddleButtonPressEvent();
      else if (state==2)
	this->WidgetInteractor->MiddleButtonReleaseEvent();
    }

  else if (button == 3 )
    {
      if (state==0)
	this->WidgetInteractor->RightButtonPressEvent();
      else if (state==2)
	this->WidgetInteractor->RightButtonReleaseEvent();
    }

  return 1;

}


int vtkpxGUIOrthogonalViewer::HandleMouseButtonEvent(int nbutton,int state,
						     int x,int y,PXTkEventObj* ev)
{

  // Not There for Polar Images 
  if (nbutton==9 && state==2)
    {
      if (this->NavigationPointer->GetEnabled())
    	{
    	  this->RenderNavigationPointer(0);
    	}
          else
    	{
    	  this->RenderNavigationPointer(3);
    	}
      return 1;
    }

  // for navigation actor
  if (nbutton==7 && state==2)
    {
      this->RenderNavigationPointer((this->NavigationPointerDrawMode + 1) % 5);
      return 1;
    }

  int status=1;
  int found=-1;
  if (ev==NULL)
    return 0;

  for (int i=3;i>=0;i-=1)
      {
	if (renderer[i]->InViewport(x,y))
	    found=i;
      }

  double lvv[3];
  for (int ic=0;ic<=2;ic++)
    lvv[ic]=float(-1);

  if (found==-1)
    {
      this->SetLastClickedPoint(lvv);
      this->SetLastClickedPointScaled(lvv);
      return 0;
    }

  if (found==3)
    {
      this->EventResponseFromWidget=0;
      this->SendEventToWidgetInteractor(x,y,nbutton,state);
      if (this->EventResponseFromWidget==1)
	{
	  // Now we can do stuff like .....
	  // Update Volume Control for example from here
	  // this->VolumeControl->UpdatePosition(this->NavigationPointer);

	  double p1[3];   this->NavigationPointer->GetPoint1(p1);
	  double p2[3];   this->NavigationPointer->GetPoint2(p2);
	  double d=sqrt(vtkMath::Distance2BetweenPoints(p2,p1));
	  char line[20]; sprintf(line,"Distance = %.3f",d);
	  this->renderer[3]->ShowLabel();
	  this->renderer[3]->SetLabel(line,0.05,0.05,1,0,0);
	  this->UpdateNavigationActorGeometry((state==2));
	  return 1;
	}



      if (this->TraceVolume && nbutton==4 && this->Orientation!=3)
	{
	  this->SetLastClickedPoint(lvv);
	  this->SetLastClickedPointScaled(lvv);
	  return this->VolumePick(x,y,renderer[3]->GetRenderer());
	}
      else if (this->TraceGeometry && nbutton == 4 )
	{
	  this->SetLastClickedPoint(lvv);
	  this->SetLastClickedPointScaled(lvv);
	  return this->GeometryPick(x,y,renderer[3]->GetRenderer());
	}
      else
	{
	  this->SetLastClickedPoint(lvv);
	  this->SetLastClickedPointScaled(lvv);
	  return renderer[found]->HandleMouseButtonEvent(nbutton,state,x,y,ev);
	}
    }



  if (!hasImage || this->Orientation==3)
    {
      this->SetLastClickedPoint(lvv);
      this->SetLastClickedPointScaled(lvv);
      return 0;
    }

  double px1,py1;
  renderer[found]->Get2DClickedPoint(px1,py1,x,y);
  
  if (nbutton==4 || nbutton==7 || nbutton==1 )
      {
	double sp[3];    currentImage->GetSpacing(sp);
	double ori[3];   currentImage->GetOrigin(ori);

	int lv[3];
	double lvs[3];
	for (int ib=0;ib<=2;ib++)
	  {
	    lv[ib]=int(coordScale[ib]->getValue())-offset;
	    lvs[ib]=lv[ib]*sp[ib]+ori[ib];
	  }

	switch(found)
	  {
	  case 0:
	    lv[1]=(int)((px1-ori[1])/sp[1]+0.5);
	    lv[2]=(int)((py1-ori[2])/sp[2]+0.5);
	    lvs[1]=px1;
	    lvs[2]=py1;
	    break;
	  case 1:
	    lv[0]=(int)((px1-ori[0])/sp[0]+0.5);
	    lv[2]=(int)((py1-ori[2])/sp[2]+0.5);
	    lvs[0]=px1;
	    lvs[2]=py1;
	    //sliceControl->SetScaleValues(px1,-1,py1,1);
	    break;
	  case 2:
	    //  sliceControl->SetScaleValues(px1,py1,-1,2);
	    lv[0]=(int)((px1-ori[0])/sp[0]+0.5);
	    lv[1]=(int)((py1-ori[1])/sp[1]+0.5);
	    lvs[0]=px1;
	    lvs[1]=py1;
	    break;
	  }

	this->LastClickedSlice=found;


	if (state !=0)
	  {
	    if (this->MouseSliceChangeMode && this->Orientation!=3)
	      {
		this->SetCoordinates(lv[0],lv[1],lv[2]);

	      }
	    else
	      {
		this->ChangeSliceLevels(lv,1);
	      }
	    this->SetLastClickedPointScaled(lvs);
	    this->ExecuteCoordinatesChangeCallback();
	  }
	else
	  {
	    //	    this->SetLastClickedPointScaled(lvs);
	    double lvv[3];
	    for (int ia=0;ia<=2;ia++)
	      lvv[ia]=double(lv[ia]);
	    this->SetLastClickedPoint(lvv);
	    this->SetLastClickedPointScaled(lvs);
	  }
	status=1;
      }
  else
    {
      int lv[3];
      lv[0]=-1;lv[1]=-1;lv[2]=-1;
      this->ChangeSliceLevels(lv,1);
      status=0;
    }
  
  if (nbutton!=1 && nbutton!=4 && nbutton!=7)
    {
      return renderer[found]->HandleMouseButtonEvent(nbutton,state,x,y,ev);
    }
  
  return status;
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::GetDisplayMode()
{
  return display_mode;
}

int vtkpxGUIOrthogonalViewer::GetDisplayMode3D()
{
  if (display_mode==2 || display_mode==6 || display_mode==7)
    return 1;
  else
    return 0;
}

void vtkpxGUIOrthogonalViewer::SetDisplayMode3D()
{
  AdjustViewports(6);
}

void vtkpxGUIOrthogonalViewer::SetDisplayMode3DIfNeeded()
{
  if (display_mode !=2 && display_mode!=6 && display_mode!=7)
    this->AdjustViewports(6);
}


void vtkpxGUIOrthogonalViewer::SetDisplayMode2D()
{
  AdjustViewports(1);
}
// ---------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::UpdateKeyRenderer()
{
  this->AdjustViewports(this->display_mode);
}
// ---------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::AdjustViewports(int mode)
{
  //printf( "mode=%d\n",mode);
  if (!this->Initialized)
    {
      return;
    }


  // Force 3d Mode if no image to anchor other views 
  if (!hasImage)
    {
      mode=1; 
    }

  display_mode=Irange(mode,0,7);
  if (displayMode->getIndex()!=display_mode)
    displayMode->setIndex(display_mode);

  double sp[3];    
  int dim[3];   
  double len[3];
  double ori[3];

  if (hasImage && (display_mode<=2 || display_mode==7))
    {
      currentImage->GetSpacing(sp);
      currentImage->GetOrigin(ori);
      currentImage->GetDimensions(dim);
    }

  for (int ia=0;ia<=2;ia++)
      len[ia]=sp[ia]*float(dim[ia]);

  // Eliminate all renderers first 

  for (int i=0;i<=3;i++)
    {
      renderer[i]->SetViewport(0.99999,0.9999,1.0,1.0);
      render_widget->getRenderWindow()->GetRenderers()->RemoveItem(renderer[i]->GetRenderer());
    }
  this->KeyRenderer->SetViewport(0.99,0.99,1.0,1.0);
  render_widget->getRenderWindow()->GetRenderers()->RemoveItem(this->KeyRenderer);

  double yvalues[2] = { 0.05,0.525 };
  if (this->ShowKeyRenderer==0)
    {
      yvalues[0]=0.0;
      yvalues[1]=0.5;

    }
  else
    {
      render_widget->getRenderWindow()->GetRenderers()->AddItem(this->KeyRenderer);      
      this->KeyRenderer->SetViewport(0.0,0.0,1.0,0.05);
    }
  render_widget->getRenderWindow()->GetRenderers()->InitTraversal();
      
  // Three types of modes 3-slice 4-renderers 1-renderer


  if (display_mode>2 && display_mode!=7)
    {
      // Single Renderer Mode Only, things are very easy here
      int r=display_mode-3;
      render_widget->getRenderWindow()->AddRenderer(renderer[r]->GetRenderer());
      renderer[r]->SetViewport(0.0, yvalues[0], 1.0, 1.0);
    }


  int oldstyleorient=vtkpxGUIBaseImageViewer::MapOrientationToOldStyle(this->Orientation);
  if (this->Orientation>=40)
    oldstyleorient+=4;
  
  if (display_mode==1 || display_mode==2 || display_mode==7)
    {
      int vr[3];
      switch (oldstyleorient)
	{
	case 1:
	  vr[0]=0; vr[1]=2; vr[2]=1;
	  break;
	case 0:
	  vr[0]=0; vr[1]=1; vr[2]=2;
	  break;
	case 2:
	  vr[0]=2; vr[1]=0; vr[2]=1;
	  break;
	case 3:
	  vr[0]=2; vr[1]=1; vr[2]=0;
	  break;

	case 4: // Transposed Axial OK
	  vr[0]=1; vr[1]=0; vr[2]=2;
	  break;
	case 5: // Tranposed Coronal
	  vr[0]=1; vr[1]=2; vr[2]=0;
	  break;
	case 6: // tranposed to come
	  vr[0]=2; vr[1]=1; vr[2]=0;
	  break;

	}

      for (int r=0;r<=2;r++)
	{
	  render_widget->getRenderWindow()->AddRenderer(renderer[r]->GetRenderer());
	}
      
      if (display_mode==2)
	{
	  render_widget->getRenderWindow()->AddRenderer(renderer[3]->GetRenderer());
	  renderer[vr[0]]->SetViewport(0.5, 0.7, 0.8, 1.0);
	  renderer[vr[1]]->SetViewport(0.0, 0.7, 0.3, 1.0);
	  renderer[vr[2]]->SetViewport(0.0, 0.2, 0.3, 0.5);
	  renderer[3]->SetViewport(0.3, yvalues[0], 1.0, 0.7);
	}  
      else if (display_mode == 7 )
	{
	  render_widget->getRenderWindow()->AddRenderer(renderer[3]->GetRenderer());
	  renderer[vr[0]]->SetViewport(0.5, yvalues[1], 1.0, 1.0);
	  renderer[vr[1]]->SetViewport(0.0, yvalues[1], 0.5, 1.0);
	  renderer[vr[2]]->SetViewport(0.0, yvalues[0], 0.5, yvalues[1]);
	  renderer[3]->SetViewport(0.5,yvalues[0],1.0,yvalues[1]);
	}
      else
	{
	  renderer[vr[0]]->SetViewport(0.5, yvalues[1], 1.0, 1.0);
	  renderer[vr[1]]->SetViewport(0.0, yvalues[1], 0.5, 1.0);
	  renderer[vr[2]]->SetViewport(0.0, yvalues[0],  0.5, yvalues[1]);
	}
    } 

  if (display_mode==0)
    {
      for (int r=0;r<=2;r++)
	render_widget->getRenderWindow()->AddRenderer(renderer[r]->GetRenderer());

      float r1,r2;
      r1=len[1]/(len[1]+2*len[0]);
      r2=r1+len[0]/(len[1]+2*len[0]);


      renderer[0]->SetViewport( 0.0, yvalues[0] , r1 , 1.0);
      renderer[1]->SetViewport(  r1, yvalues[0] , r2 , 1.0);
      renderer[2]->SetViewport(  r2, yvalues[0] , 1.0, 1.0);
    }


  if (display_mode == 2 || display_mode == 6 || display_mode == 7)
      renderer[3]->EnableOrientationMarker(1);
  else 
      renderer[3]->EnableOrientationMarker(0);


  for (int kk=3;kk>=0;kk-=1)
    {
      if (display_mode==2 || display_mode==6 ) {
	     renderer[kk]->HideLabel();
  }
      else
	     renderer[kk]->ShowLabel();
      renderer[kk]->SetAutoZoom(1.0);
      renderer[kk]->Reset();
    }

  if (hasImage==PXTrue)
    {
      MoveAxis(1);
    }
  else 
    {
      this->UpdateDisplay();
    }


     
}  
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::SetOffset(int of)
{
  if (this->Initialized==0)
      this->offset=of;
}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::InitControls(int )
{
  viewerFrame2->setPackMode(PXTrue);

  PXTkFrame* sideFrame=new PXTkFrame(this->EventManager,viewerFrame2);
  sideFrame->setPackMode(PXFalse);
  
  if (viewerFrame==viewerFrame2)
    {
      viewerFrame2->addChildren("-side right -expand false -fill y",sideFrame);
      PXTkButton* hideButton=new PXTkButton(this->EventManager,sideFrame,"x",350);
      hideButton->configure("-bg","white");
      hideButton->configure("-fg","black");
      hideButton->configure("-width","1");
      sideFrame->gridRowColumnChild(hideButton,0,1);
      sideFrame->childConfigure(hideButton,"-padx","0");
      sideFrame->childConfigure(hideButton,"-pady","0");
      sideFrame->childConfigure(hideButton,"-ipadx","0");
      sideFrame->childConfigure(hideButton,"-ipady","0");
      sideFrame->childConfigure(hideButton,"-sticky","nw");
    }
  else 
    viewerFrame2->addChildren("-side right -expand true -fill both",sideFrame);


  sideFrame->gridExpandColumnConfigure(0,1,0,0);
  sideFrame->gridExpandColumnConfigure(1,0,0,0);

  controlFrame=new PXTkFrame(this->EventManager,sideFrame);

  if (this->show_controlframe==1)
    sideFrame->gridRowColumnChild(controlFrame,1,0,1,2);

  displayMode=new PXTkOptionMenu(this->EventManager,controlFrame,"Simple Mode",307);
  displayMode->addOption("3-Slice Mode");
  displayMode->addOption("3-Slice+3D Mode");
  displayMode->addOption("Coronal Only");
  displayMode->addOption("Axial Only");
  displayMode->addOption("Sagittal Only");
  displayMode->addOption("3D Only");
  displayMode->addOption("3-Slice+3D Mode(2)");

  displayMode->setIndex(display_mode);
  PXTkFrame* threedframe=new PXTkFrame(this->EventManager,controlFrame);
  PXTkFrame* threedframe2=new PXTkFrame(this->EventManager,controlFrame);


  display3DMode=new PXTkOptionMenu(this->EventManager,threedframe,"None",331);
  display3DMode->addOption("3-slice");
  display3DMode->addOption("Volume");
  display3DMode->addOption("Oblique");
  display3DMode->addOption("Obl+Vol");
  display3DMode->addOption("Obl+3-s");
  display3DMode->addOption("3-s+vol");
  display3DMode->addOption("3s+vl+o");

  display3DMode->setIndex(display3d_mode);

  display3DAxisOutlineMode=new PXTkOptionMenu(this->EventManager,threedframe,"None",332);
  display3DAxisOutlineMode->addOption("Box");
  display3DAxisOutlineMode->addOption("Axis");
  display3DAxisOutlineMode->addOption("Both");
  display3DAxisOutlineMode->setIndex(display3d_axisoutlinemode);

  PXTkButton* b1h=new PXTkButton(this->EventManager,threedframe2,"Volume",330);
  PXTkButton* b1h2=new PXTkButton(this->EventManager,threedframe2,"Oblique",333);
  threedframe->addChildren("-side left -expand t -fill x -padx 0",display3DMode,display3DAxisOutlineMode);
  threedframe2->addChildren("-side left -expand t -fill x -padx 0",b1h,b1h2);
  controlFrame->addChildren("-side top -expand false -fill x",displayMode,threedframe,threedframe2);


  // CheckButton Frame 
  PXTkFrame* checkFrame=new PXTkFrame(this->EventManager,controlFrame);
  controlFrame->addChildren("-side top -expand false -fill x",checkFrame);
  checkFrame->setPackMode(PXFalse);


  interpolateMode=new PXTkCheckButton(this->EventManager,checkFrame,"Interp",300);
  interpolateMode->setState(PXTrue);
  interpolateMode->configure("-padx","0");
  checkFrame->gridRowColumnChild(interpolateMode,0,2);


  this->showPointer=new PXTkCheckButton(this->EventManager,checkFrame,"-P-",318);
  showPointer->setState(PXFalse);
  showPointer->configure("-padx","0");
  checkFrame->gridRowColumnChild(showPointer,0,3);


  freezeMode=new PXTkCheckButton(this->EventManager,checkFrame,"x-hairs",308);
  if (this->MouseSliceChangeMode)
    freezeMode->setState(PXTrue);
  else
    freezeMode->setState(PXFalse);
  freezeMode->configure("-padx","0");
  checkFrame->gridRowColumnChild(freezeMode,0,0);


  showaxisc=new PXTkCheckButton(this->EventManager,checkFrame,"Lb",317);
  showaxisc->setState(PXTrue);
  showaxisc->configure("-padx","0");
  checkFrame->gridRowColumnChild(showaxisc,0,1);


  // Talairach Frame 
  PXTkFrame* tframe=new PXTkFrame(this->EventManager,controlFrame);
  controlFrame->addChildren("-side top -expand true -fill x",tframe);

  talairachlabel=new PXTkLabel(this->EventManager,tframe,"");
  talairachlabel->configure("-bg","black");
  talairachlabel->configure("-fg","white");
  talairachlabel->configure("-width","20");
  talairachlabel->configure("-padx","0");
  tframe->addChildren("-side left -expand true -fill x -padx 2",talairachlabel);

  if (this->createTalairachButtons)
    {
      PXTkFrame* tframe2=new PXTkFrame(this->EventManager,controlFrame);
      controlFrame->addChildren("-side top -expand true -fill x",tframe2);

      PXTkButton*   talairachButton[2];
      talairachButton[0]=new PXTkButton(this->EventManager,tframe2,"Tal",320);
      talairachButton[0]->configure("-pady","0");
      talairachButton[0]->configure("-padx","1");
      talairachButton[0]->configure("-fg","red");
      talairachButton[1]=new PXTkButton(this->EventManager,tframe2,"Tal NEU",322);


      //      talairachButton[1]->configure("-pady","0");
      //      talairachButton[1]->configure("-padx","1");


      mnilabel=new PXTkLabel(this->EventManager,tframe2,"");
      mnilabel->configure("-bg","black");
      mnilabel->configure("-fg","white");
      mnilabel->configure("-width","15");
      mnilabel->configure("-padx","0");
      //      tframe2->addChildren("-side right -expand false",talairachButton[1],talairachButton[0]);
      //      tframe2->addChildren("-side right -expand false",talairachButton[0]);
      tframe2->addChildren("-side left -expand true -fill x -padx 2",mnilabel);
    }


  extraFrame=new PXTkFrame(this->EventManager,controlFrame);
  coordScaleFrame=new PXTkFrame(this->EventManager,controlFrame);
  controlFrame->addChildren("-side top -expand true -fill x",extraFrame,coordScaleFrame);

  for (int k=0;k<=3;k++)
      {
	switch(k)
	    {
	    case 0:
	      strcpy(pxtk_buffer4,"i-Coord:"); break;
	    case 1:
	      strcpy(pxtk_buffer4,"j-Coord:"); break;	 
	    case 2:
	      strcpy(pxtk_buffer4,"k-Coord:"); break;
	    case 3:
	      strcpy(pxtk_buffer4,"Frame:"); break;
	    }
	if (k!=3)
	  {
	    coordScale[k]=new PXTkArrowScale(this->EventManager,coordScaleFrame,pxtk_buffer4,302,PXTrue);
	    coordScale[k]->setRange(0.0+offset,1.0+offset);
	  }
	else
	  {
	    coordScale[k]=new PXTkArrowScale(this->EventManager,coordScaleFrame,pxtk_buffer4,301,PXTrue);
	    coordScale[k]->setRange(1,10);
	  }
	coordScale[k]->setValue(float(0.0));
	coordScale[k]->setLengthWidth(120,10);
	coordScale[k]->setIncrement(1.0);
	coordScale[k]->setResolution(1.0);
	coordScaleFrame->addChildren("-side top -expand false -fill x",coordScale[k]->getMainWindow());
	if (k==3 && this->CurrentImageIsColor==1)
	  this->ShowFrameControls(0);
      }


	
  PXTkFrame* colFrame=new PXTkFrame(this->EventManager,controlFrame);
  PXTkButton* b1c0=new PXTkButton(this->EventManager,colFrame,"St",309);
  PXTkButton* b1c=new PXTkButton(this->EventManager,colFrame,"Nr",310);
  PXTkButton* b1d=new PXTkButton(this->EventManager,colFrame,"F1",311);
  PXTkButton* b1e=new PXTkButton(this->EventManager,colFrame,"F2",312);
  PXTkButton* b1f2=new PXTkButton(this->EventManager,colFrame,"F4",314);
  PXTkButton* b1g=new PXTkButton(this->EventManager,colFrame,"Cmap",303);

  b1c0->configure("-padx",1);
  b1c->configure("-padx",1);
  b1d->configure("-padx",1);
  b1e->configure("-padx",1);
  b1f2->configure("-padx",1);


  colFrame->addChildren("-side left -expand f ",b1c0,b1c);
  colFrame->addChildren("-side left -expand f ",b1d,b1e,b1f2);
  colFrame->addChildren("-side right -expand true -fill x",b1g);
  PXTkDualArrowLabel* zoom=new PXTkDualArrowLabel(this->EventManager,controlFrame,"Zoom",304,305,0);
  PXTkFrame* botFrame=new PXTkFrame(this->EventManager,controlFrame);
  controlFrame->addChildren("-side top -expand true -fill both",colFrame,zoom,botFrame);
  PXTkButton* b1=new PXTkButton(this->EventManager,botFrame,"Reset",306);
  PXTkButton* b2=new PXTkButton(this->EventManager,botFrame,"Snapshot",315);
  PXTkButton* b3=new PXTkButton(this->EventManager,botFrame,"Export 3D",345);
  saveAllButton=new PXTkButton(this->EventManager,botFrame,"Save All",316);
  controlFrame->addChildren("-side left -expand true -fill both",b1,b2,b3);
}

// -------------------------------------------------------------------------

vtkpxGUIRenderer* vtkpxGUIOrthogonalViewer::GetRenderer(int i)
{
  return renderer[Irange(i,0,3)];
}

vtkpxImageSlice* vtkpxGUIOrthogonalViewer::GetImageSlice(int i)
{
  return imageSlice[Irange(i,0,2)];
}

vtkpxOrthoImageSlice* vtkpxGUIOrthogonalViewer::GetOrthoImageSlice()
{
  return orthoSlice;
}

vtkpxObliqueImageSlice* vtkpxGUIOrthogonalViewer::GetObliqueImageSlice()
{
  return this->obliqueSlice;
}

vtkpxGUIObliqueImageSliceControl*  vtkpxGUIOrthogonalViewer::GetObliqueControl()
{
  return this->ObliqueControl;
}

vtkpxGUIVolumeControl*  vtkpxGUIOrthogonalViewer::GetVolumeControl()
{
  return this->VolumeControl;
}

vtkActor* vtkpxGUIOrthogonalViewer::GetOutlineActor(int i)
{
  return outline[Irange(i,0,3)];
}

// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::SetOrthoSliceVisibility(int orthovis)
{
  this->OrthoSliceVisibility=orthovis>0;
  if (hasImage)
    {
      if (orthoSlice!=NULL)
	{
	  orthoSlice->SetDisplayMode(3);
	  orthoSlice->SetVisibility(this->OrthoSliceVisibility);
	}
    }
}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::SetVolumeVisibility(int volvis)
{
  volvis=volvis>0;
  if (hasImage)
    {
      if (this->VolumeControl!=NULL)
	this->VolumeControl->SetVisibility(volvis);
      if (volvis>0)
	this->Volume->Update();
      
    }
}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::SetObliqueSliceVisibility(int oblvis)
{
  oblvis=oblvis>0;
  if (hasImage)
    {
      if (this->ObliqueControl!=NULL) 
	this->ObliqueControl->SetVisibility(oblvis);
    }
}
// -------------------------------------------------------------------------
char* vtkpxGUIOrthogonalViewer::GetRenderWidgetName()
{
  if (this->render_widget==NULL)
    return NULL;
  else
    return this->render_widget->getWidgetName();
}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::SetMouseSliceChangeMode(int m)
{
  m=(m>0);
  if (m==this->MouseSliceChangeMode)
    return;

  this->MouseSliceChangeMode=m;
  if (!this->Initialized)
      return;

  if (this->MouseSliceChangeMode)
    freezeMode->setState(PXTrue);
  else
    freezeMode->setState(PXFalse);
}
// -------------------------------------------------------------------------
char* vtkpxGUIOrthogonalViewer::GetControlFrameName()
{
  if (this->controlFrame!=NULL)
    return this->controlFrame->getWidgetBaseName();
  else
    return NULL;

}

char* vtkpxGUIOrthogonalViewer::GetViewerFrameName()
{
  if (this->viewerFrame!=NULL)
    return this->viewerFrame->getWidgetBaseName();
  else
    return NULL;
}


void  vtkpxGUIOrthogonalViewer::SetShowControls(int show)
{
  show=(show>0);
  if (show==this->show_controlframe)
    return;

  this->show_controlframe=show;

  PXTkFrame* fr=(PXTkFrame*)controlFrame->getGadgetParent();
  if (this->controlFrame!=NULL && fr!=NULL)
    {
      if (this->show_controlframe==1)
	fr->gridRowColumnChild(controlFrame,1,0,4,4);
      else
	fr->unmapChild(controlFrame);
    }
}
// -------------------------------------------------------------------------

int vtkpxGUIOrthogonalViewerinBounds(float p[3],float bounds[6])
{
  if (p[0]<bounds[0] || p[0]>bounds[1] ||
      p[1]<bounds[2] || p[1]>bounds[3] ||
      p[2]<bounds[4] || p[2]>bounds[5])
    return 0;
  else
    return 1;
}

// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::VolumePick(int clickx, int clicky, vtkRenderer* pick_renderer)
{
  if (!hasImage)
    return 0;

  if (this->Volume==NULL)
    return 0;


  int   dim[3];     currentImage->GetDimensions(dim);
  double sp[3];      currentImage->GetSpacing(sp);
  double ori[3];     currentImage->GetOrigin(ori);
  int   boundsv[6]; this->Volume->GetCroppingRange(boundsv);

  float searchdim=Fmin(sp[0],Fmin(sp[1],sp[2]));

  switch(this->TraceVolumeToleranceMode)
    {
    case 0:
      searchdim*=2.0;
      break;
    case 2:
      searchdim/=2.0;
      break;
    default:
      searchdim*=1.0;
      break;
    }
  
  int i;
  
  float SelectionPoint[3];
  SelectionPoint[0] = float(clickx);
  SelectionPoint[1] = float(clicky);
  SelectionPoint[2] = 0.0;
  
  float selectionX=SelectionPoint[0];
  float selectionY=SelectionPoint[1];
  float selectionZ=SelectionPoint[2];
  
  // Get camera focal point and position. Convert to display (screen) 
  // coordinates. We need a depth value for z-buffer.
  vtkCamera* camera = pick_renderer->GetActiveCamera();
  double cameraPos[4], cameraFP[4];
  camera->GetPosition((double *)cameraPos);  cameraPos[3] = 1.0;
  camera->GetFocalPoint((double *)cameraFP); cameraFP[3] = 1.0;
  
  pick_renderer->SetWorldPoint(cameraFP);
  pick_renderer->WorldToDisplay();
  double* displayCoords = pick_renderer->GetDisplayPoint();
  selectionZ = displayCoords[2];
  
  // Convert the selection point into world coordinates.
  pick_renderer->SetDisplayPoint(selectionX, selectionY, selectionZ);
  pick_renderer->DisplayToWorld();
  double* worldCoords = pick_renderer->GetWorldPoint();
  float PickPosition[4];
  for (i=0; i < 3; i++) 
    PickPosition[i] = worldCoords[i] / worldCoords[3];
  
  //  Compute the ray endpoints.  The ray is along the line running from
  //  the camera position to the selection point, starting where this line
  //  intersects the front clipping plane, and terminating where this
  //  line intersects the back clipping plane.
  double ray[3], rayLength;
  for (i=0; i<3; i++)
    ray[i] = PickPosition[i] - cameraPos[i];
  
  double cameraDOP[3];
  for (i=0; i<3; i++)
    cameraDOP[i] = cameraFP[i] - cameraPos[i];
  vtkMath::Normalize(cameraDOP);
  
  if (( rayLength = vtkMath::Dot(cameraDOP,ray)) == 0.0 ) 
    return 0;
  
  double* clipRange = camera->GetClippingRange();
  double tF, tB;
  double p1World[3], p2World[3];
  
  if ( camera->GetParallelProjection() )
    {
      tF = clipRange[0] - rayLength;
      tB = clipRange[1] - rayLength;
      for (i=0; i<3; i++) 
	{
	  p1World[i] = PickPosition[i] + tF*cameraDOP[i];
	  p2World[i] = PickPosition[i] + tB*cameraDOP[i];
	}
    }
  else
    {
      tF = clipRange[0] / rayLength;
      tB = clipRange[1] / rayLength;
      for (i=0; i<3; i++) 
	{
	  p1World[i] = cameraPos[i] + tF*ray[i];
	  p2World[i] = cameraPos[i] + tB*ray[i];
	}
    }
  
  
  // Probe to Find Point
  // -------------------
  double tlength=sqrt(vtkMath::Distance2BetweenPoints(p1World,p2World));
  int   numsegments=int(tlength/searchdim);
  if (numsegments<10)
    numsegments=10;

  vtkLineSource* ln=vtkLineSource::New();
  ln->SetResolution(numsegments);
  ln->SetPoint1(p1World);
  ln->SetPoint2(p2World);
  ln->Update();

  int found=0,index=0,lvs[3];
  double vl=0.0;
  vtkPoints* pt=ln->GetOutput()->GetPoints();
  int np=pt->GetNumberOfPoints();

  while (index<np && found == 0 )
    {
      double p[3];
      pt->GetPoint(index,p);
      for (int ik=0;ik<=2;ik++)
	lvs[ik]=int(p[ik]/sp[ik]-ori[ik]);
	      
      if (lvs[0]>=boundsv[0] && lvs[0]<=boundsv[1]  &&
	  lvs[1]>=boundsv[2] && lvs[1]<=boundsv[3]  &&
	  lvs[2]>=boundsv[4] && lvs[2]<=boundsv[5])
	{
	  vl=this->currentImage->GetScalarComponentAsDouble(lvs[0],lvs[1],lvs[2],0);
	  if (vl>0.0)
	    found=1;
	}
      ++index;
    }
  ln->Delete();

  
  if (found)
    {
      double lvv[3];
      // -------------- Electrode ---------------
      if (this->TraceVolumeVOI>0 )
	this->FindCentroid(this->currentImage,lvs,lvv,this->TraceVolumeVOI);
      // -------------- Electrode ---------------
      
      if (this->MouseSliceChangeMode)
	this->SetCoordinates(lvs[0],lvs[1],lvs[2]);
      else 
	this->UpdateDisplay();

      if (this->TraceVolumeVOI>0 )
	this->SetLastClickedPoint(lvv);
    }
  
  
  return (found);
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::GeometryPick(int clickx, int clicky, vtkRenderer* pick_renderer)
{
  double ori[3],sp[3];

  if (!hasImage)
    {
      for (int ia=0;ia<=2;ia++)
	{
	  ori[ia]=0.0; sp[ia]=1.0;
	}
    }
  else
    {
      currentImage->GetOrigin(ori);
      currentImage->GetSpacing(sp);
    }

  vtkCellPicker* picker=vtkCellPicker::New();
  picker->SetTolerance(0.005);

  int ok=picker->Pick(float(clickx),float(clicky),float(0.0),pick_renderer);
  if (ok)
    {
      double lvv[3], lvs[3];
      picker->GetPickPosition(lvv);
      picker->Delete();
     
      for (int ic=0;ic<=2;ic++)
	lvs[ic]=(lvv[ic]-ori[ic])/sp[ic];
            
      if (this->MouseSliceChangeMode && hasImage)
	this->SetCoordinates((int)lvs[0],(int)lvs[1],(int)lvs[2],0);

      this->SetLastClickedPoint(lvv);
      this->SetLastClickedPointScaled(lvs);

    }
  /*  else
      fprintf(stderr,"Failed to find point\n");*/
  return (ok);
}
// -------------------------------------------------------------------------
//   The Following is overriden in vtkpxGUIOrthogonal4DViewer
// -------------------------------------------------------------------------


int vtkpxGUIOrthogonalViewer::InitializeViewer()
{
  if ((!hasImage && this->Initialized))
      return 0;


  this->ColormapEditor=vtkpxGUIColormapEditor::New();
  this->ColormapEditor->SetSimpleMode(0);
  this->ColormapEditor->SetCallback(this,201);
  this->ColormapEditor->Initialize(this->GetWidgetName(),0);

 
  for (int k=0;k<=2;k++)
      {
	imageSlice[k]=vtkpxImageSlice::New();
	imageSlice[k]->SetAutoUpdate(0);
	imageSlice[k]->SetInput(currentImage);
	imageSlice[k]->SetFrame(0);
	imageSlice[k]->SetCurrentPlane(k);
	imageSlice[k]->PickableOff();
	imageSlice[k]->SetColorMode(this->CurrentImageIsColor);
	renderer[k]->GetRenderer()->AddActor(imageSlice[k]);
      }

  orthoSlice=vtkpxOrthoImageSlice::New();
  orthoSlice->SetAutoUpdate(0);
  //orthoSlice->SetAutoOpacity(1);
  orthoSlice->SetInput(currentImage);
  orthoSlice->SetColorMode(this->CurrentImageIsColor);
  orthoSlice->SetFrame(0);
  orthoSlice->PickableOff();
  renderer[3]->GetRenderer()->AddActor(orthoSlice);
  orthoSlice->SetVisibility(this->OrthoSliceVisibility);


  obliqueSlice=vtkpxObliqueImageSlice::New();
  obliqueSlice->SetAutoUpdate(0);
  obliqueSlice->SetInput(currentImage);
  obliqueSlice->SetColorMode(this->CurrentImageIsColor);
  obliqueSlice->SetFrame(0);
  obliqueSlice->SetVisibility(0);
  obliqueSlice->PickableOff();
  this->ObliqueControl->SetObliqueImageSliceRenderer(this->obliqueSlice,renderer[3]);
  renderer[3]->GetRenderer()->AddActor(obliqueSlice);
  obliqueSlice->SetAutoUpdate(1);

 
  /*  Vol Commented*/
  this->Volume=vtkpxVolume::New();
  this->Volume->SetInput(currentImage,this->Orientation);
  this->Volume->SetFrame(0);
  this->Volume->SetVisibility(0);
  this->VolumeControl->SetVolumeRenderer(this->Volume,renderer[3]);
  this->Volume->Update();
  renderer[3]->GetRenderer()->AddVolume(this->Volume); 

   /*  Volume2 Commented*/
  /*  this->Volume2=vtkpxVolume::New();
  this->Volume2->SetInput(currentImage,this->Orientation);
  this->Volume2->SetFrame(0);
  this->Volume2->SetVisibility(0);
  this->VolumeControl->SetVolumeRenderer(this->Volume2,renderer[3]);
  this->Volume2->Update();
  renderer[3]->GetRenderer()->AddVolume(this->Volume2); 
  printf("added 2nd volume\n"); */
  
  
  for (int i=0;i<=3;i++)
      {
	outlineFilter[i]=vtkOutlineFilter::New();
	outlineFilter[i]->SetInput(currentImage);
	
	vtkPolyDataMapper *mapOutline = vtkPolyDataMapper::New();
	mapOutline->SetInput(outlineFilter[i]->GetOutput());
	
	outline[i] = vtkActor::New();
	outline[i]->SetMapper(mapOutline);
	outline[i]->GetProperty()->SetColor(0.0,1.0,0.0);
	outline[i]->GetProperty()->SetAmbient(1.0);
	outline[i]->GetProperty()->SetDiffuse(0.0);
	outline[i]->GetProperty()->SetSpecular(0.0);
	outline[i]->GetProperty()->SetRepresentationToWireframe();
	outline[i]->PickableOff();
	//	outline[i]->GetProperty()->SetOpacity(0.8);

	if (i!=3)
	  {
	    renderer[i]->GetRenderer()->AddActor(outline[i]);
	  }
	else
	  {
	    vtkAssembly* ass=vtkAssembly::New();
	    ass->AddPart(outline[i]);
	    renderer[i]->GetRenderer()->AddActor(ass);
	    ass->Delete();
	  }
	//	outline->Delete();
	mapOutline->Delete();

	switch(i)
	    {
	    case 0:
	      renderer[i]->SetLabel("Coronal-jk",0.05,0.05,0.73,0.73,0.0);
	      break;
	    case 1:
	      renderer[i]->SetLabel("Axial-ik",0.05,0.05,0.73,0.73,0.0);
	      break;
	    case 2:
	      renderer[i]->SetLabel("Sagittal-ij",0.05,0.05,0.73,0.73,0.0);
	      break;
	    case 3:
	      //renderer[i]->SetTitle("3D View");
	      break;
	    }
      }

  this->SetDisplay3DMode(display3d_mode);
  this->SetDisplay3DAxisOutlineMode(display3d_axisoutlinemode);
  AdjustViewports(display_mode);
  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIOrthogonalViewer::CreateNavigationPointer()
{
  vtkPolyData *point = vtkPolyData::New();
  

 
  vtkmyPWCallback* pointcallback= vtkmyPWCallback::New();
  pointcallback->Viewer=this;

  this->NavigationPointer = vtkLineWidget::New();
  this->NavigationPointer->SetInteractor(this->WidgetInteractor);
  this->NavigationPointer->SetInput(this->currentImage);
  this->NavigationPointer->SetPoint1(0,0,0);
  this->NavigationPointer->SetPoint2(20,20,20);
  this->NavigationPointer->SetResolution(1);
  //  this->NavigationPointer->PlaceWidget();
  this->NavigationPointer->AddObserver(vtkCommand::InteractionEvent,pointcallback);
  this->UpdateNavigationActorGeometry();

  double plocation[3] = {20.0, 20.0, 20.0};
  this->Volume->SetProbeLocation(plocation);
  this->Volume->Update();

  return 1;
}

int vtkpxGUIOrthogonalViewer::MoveNavigationPointer()
{
  if (this->NavigationPointer==NULL)
    return 0;

  // In mm
  double lvs[3]; this->GetLastClickedPointScaled(lvs);

  // in voxels
  double lvv[3]; this->GetLastClickedPoint(lvv);
  int dim[3]; this->currentImage->GetDimensions(dim);
  double spa[3]; this->currentImage->GetSpacing(spa);

  double pos[3] = { 0.0,0.0,0.0};
  for (int ia=0;ia<=2;ia++)
    {
      if (lvv[ia]>dim[ia]/2)
	pos[ia]=double(dim[ia]-1.0)*spa[ia];
    }

  this->NavigationPointer->SetPoint2(lvs);
  this->NavigationPointer->SetPoint1(pos);
  this->UpdateNavigationActorGeometry(1);

return 1;

}
/* -------------------------------------------------------------------------*/
void vtkpxGUIOrthogonalViewer::InitViewer()
{
  //vtkmpj3DOrientationMarkerWidget *markerWidget = vtkmpj3DOrientationMarkerWidget::New();

  render_widget=new PXVTkTkRenderWidget(this->EventManager,viewerFrame,200);
  render_widget->setWidthHeight(400,400);
  render_widget->bindMouseEvents();
  viewerFrame->addChildren("-side left -expand true -fill both",render_widget);
  


  // Background renderer 
  //  render_widget->getRenderWindow()->DebugOn();

  /*  vtkRenderWindow* rw=render_widget->getRenderWindow();
      fprintf(stderr,"swap buffers=%d\n",rw->GetSwapBuffers());*/

  //  render_widget->getRenderWindow()->SetNumberOfLayers(2);

  this->KeyRenderer->SetViewport(0.0,0.0,1.0,0.001);
  this->KeyRenderer->SetBackground(0.2,0.2,0.2);
  //  this->KeyRenderer->SetLayer(0.0);
  render_widget->getRenderWindow()->AddRenderer(this->KeyRenderer);


  for (int ia=1;ia>=0;ia=ia-1)
  {
    for (int ib=1;ib>=0;ib-=1)
	  {
	    int index=ia*2+ib;
	    vtkRenderer* ren=vtkRenderer::New();
	    ren->SetLayer(0);
	    // ren->TwoSidedLightingOff();
	    render_widget->getRenderWindow()->AddRenderer(ren);
	    renderer[index]=vtkpxGUIRenderer::New();
	    renderer[index]->SetAutoZoom(1.0);
	    renderer[index]->SetFlipMode(0);
	    if (index!=3)
	      {
		      renderer[index]->SetNoGUI(PXTrue);
		      renderer[index]->SetPlaneMode(Irange(index,0,2));
		      renderer[index]->SetClipMode(PXTrue);
		      renderer[index]->SetClipThickness(2.0);
		      renderer[index]->SetEnableActorTransform(0);
	      }
	    else
	      {
		      renderer[index]->SetClipMode(PXFalse);
		      renderer[index]->SetClipThickness(0.25);
		      renderer[index]->SetVerticalDecoration(PXFalse);
		      renderer[index]->SetEnableActorTransform(1);
	      }
	    renderer[index]->Initialize(viewerFrame->getWidgetName(),ren,0);
	    renderer[index]->SetViewport((ia)*0.5,(1-ib)*0.475+0.05,(1+ia)*0.5,(2-ib)*0.475+0.05);
	    renderer[index]->Reset();
	  }
  }

    renderer[3]->CreateOrientationMarker();

  this->BackgroundRenderer=vtkRenderer::New();
  render_widget->getRenderWindow()->AddRenderer(this->BackgroundRenderer);
  //  this->BackgroundRenderer->SetLayer(1);
  //  this->BackgroundRenderer->SetBackground(00.,0.2,0.0);
  this->BackgroundRenderer->SetViewport(0.0,0.0,1.0,1.0);

  this->WidgetInteractor=vtkGenericRenderWindowInteractor::New();
  this->WidgetInteractor->SetRenderWindow(render_widget->getRenderWindow());
  this->WidgetInteractor->SetInteractorStyle(NULL);

  this->WidgetInteractor->Initialize();
  vtkInteractorObserver* obs=this->WidgetInteractor->GetInteractorStyle();

  this->UpdateDisplay();
}

// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::ChangeSliceLevels(int lv[3],int infoonly)
{
  if (this->Initialized==0 || !hasImage)
    return;

  double sp[3];    currentImage->GetSpacing(sp);
  double ori[3];   currentImage->GetOrigin(ori);

  if (infoonly==0)
    {
      for (int ia=0;ia<=2;ia++)
	imageSlice[ia]->SetLevel(lv[ia]);
      orthoSlice->SetLevels(lv[0],lv[1],lv[2]);
    }

  double lvv[3],lvs[3];
  for (int ic=0;ic<=2;ic++)
    {
      lvv[ic]=float(lv[ic]);
      lvs[ic]=lvv[ic]*sp[ic]+ori[ic];
    }
  this->SetLastClickedPoint(lvv);
  this->SetLastClickedPointScaled(lvs);

  if (infoonly==0)
    {
      MoveAxis(1);
    }
}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::ChangeFrame(int newframe)
{
  if (this->Initialized==0 || !hasImage)
    return;

  if (newframe==this->CurrentFrame)
    return;
   
  this->CurrentFrame=Irange(newframe,0,this->NumberOfFrames-1);
  for (int ia=0;ia<=2;ia++)
    imageSlice[ia]->SetFrame(this->CurrentFrame);
  orthoSlice->SetFrame(this->CurrentFrame);
  obliqueSlice->SetFrame(this->CurrentFrame);
  

  if (this->Volume!=NULL)
    this->Volume->SetFrame(this->CurrentFrame);

  /*  if (this->Volume2!=NULL)
      this->Volume2->SetFrame(this->CurrentFrame);*/

  this->UpdateIntensityLabelInfo();
  this->ExecuteFrameChangeCallback();
}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::SetInterpolation(int interp)
{


  for (int k=0;k<3;k++)
    imageSlice[k]->SetInterpolation(interp);
  orthoSlice->SetInterpolation(interp);
  obliqueSlice->SetInterpolation(interp);
}

// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::SetDisplay3DMode(int m) 
{
  display3d_mode=Irange(m,0,7);

  switch (display3d_mode)
    {
    case 1: // 3-slice
      this->SetOrthoSliceVisibility(1);
      this->SetObliqueSliceVisibility(0);
      this->SetVolumeVisibility(0);
      break;

    case 2: // Volume
      this->SetOrthoSliceVisibility(0);
      this->SetObliqueSliceVisibility(0);
      this->SetVolumeVisibility(1);
      break;

    case 3: // Oblique
      this->SetOrthoSliceVisibility(0);
      this->SetObliqueSliceVisibility(1);
      this->SetVolumeVisibility(0);
      break;

    case 4: // Oblique + Volume
      this->SetOrthoSliceVisibility(0);
      this->SetObliqueSliceVisibility(1);
      this->SetVolumeVisibility(1);
      break;
      
    case 5: // Oblique + 3-slice
      this->SetOrthoSliceVisibility(1);
      this->SetObliqueSliceVisibility(1);
      this->SetVolumeVisibility(0);
      break;

    case 6: // 3-slice + volume
      this->SetOrthoSliceVisibility(1);
      this->SetObliqueSliceVisibility(0);
      this->SetVolumeVisibility(1);
      break;

    case 7: // everything
      this->SetOrthoSliceVisibility(1);
      this->SetObliqueSliceVisibility(1);
      this->SetVolumeVisibility(1);
      break;

    default: // None 
      this->SetOrthoSliceVisibility(0);
      this->SetObliqueSliceVisibility(0);
      this->SetVolumeVisibility(0);
      break;
    }

  display3DMode->setIndex(display3d_mode);  
}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::SetDisplay3DAxisOutlineMode(int m)
{
  display3d_axisoutlinemode=Irange(m,0,3);

  if (axisActor[0]!=NULL)
    {
      if (display3d_axisoutlinemode==0 || display3d_axisoutlinemode==1)
	{
	  for (int k=6;k<10;k++)
	    axisActor[k]->SetVisibility(0);
	}
      else 
	{
	  for (int k=6;k<10;k++)
	    axisActor[k]->SetVisibility(1);
	}
    }

  if (outline[3]!=NULL)
    {
      if (display3d_axisoutlinemode==0 || display3d_axisoutlinemode==2)
	outline[3]->SetVisibility(0);
      else
	outline[3]->SetVisibility(1);
    }
}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::ShowCrossHairs(int mode)
{
  if (axisActor[0]==NULL)
    return;

  for (int k=0;k<=5;k++)
    axisActor[k]->SetVisibility((mode>0));
  
  for (int j=0;j<=3;j++)
    {
      if (mode>0)
        renderer[j]->ShowLabel();
      else
	renderer[j]->HideLabel();
    }
  this->MoveAxis(1);
}

// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::ShowFrameControls(int show)
{
  if (!this->Initialized)
    return;

  if (show==0)
    {
      coordScaleFrame->unmapChild(coordScale[3]->getMainWindow());
      controlFrame->unmapChild(saveAllButton);
    }
  else
    {
      coordScaleFrame->addChildren("-side top -expand false -fill x",coordScale[3]->getMainWindow());
      controlFrame->addChildren("-side right -expand true -fill both",saveAllButton);
    }

  
}
void vtkpxGUIOrthogonalViewer::SetRendererName(vtkpxGUIRenderer* ren,const char* basename,
					       int axis,
					       int dmindex)
{
  char line[100];
  //  fprintf(stderr,"Basename=%s orientation=%d axis=%d\n",basename,this->Orientation,axis);
  sprintf(line,"%s",basename);
  /*  for (int i=0;i<=2;i++)
    {
      if (i!=axis)
	{
	  switch(i)
	    {
	    case 0: 
	      sprintf(line,"%s i -> %s",line,this->OrientationToAxisName(this->Orientation,i));
	      break;
	    case 1: 
	      sprintf(line,"%s j -> %s",line,this->OrientationToAxisName(this->Orientation,i));
	      break;
	    case 2: 
	      sprintf(line,"%s k -> %s",line,this->OrientationToAxisName(this->Orientation,i));
	      break;
	    }
	}
	}*/
  ren->SetLabel(line,0.05,0.05,0.73,0.73,0.0);
  if (dmindex>=0)
    displayMode->changeLabel(dmindex,basename);
}

// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::ResetCoordinateScalesAndLabels()
{
  //  fprintf(stderr,"In Reset Coordinate Scales and Labels %d\n",this->Orientation);
  int oldstyleorient=vtkpxGUIBaseImageViewer::MapOrientationToOldStyle(this->Orientation);

  if (this->Orientation>=40)
    oldstyleorient+=4;

  switch (oldstyleorient)
    {
    case 0: // Axial
      this->SetRendererName(renderer[0],"Sagital-jk",0,3);
      this->SetRendererName(renderer[1],"Coronal-ik",1,4);
      this->SetRendererName(renderer[2],"Axial-ij",2,5);
      break;
    case 1:
      this->SetRendererName(renderer[0],"Sagittal-jk",0,3);
      this->SetRendererName(renderer[1],"Axial-ik",1,4);
      this->SetRendererName(renderer[2],"Coronal-ij",2,5);
      break;
    case 2:
      this->SetRendererName(renderer[0],"Coronal-jk",0,3);
      this->SetRendererName(renderer[1],"Axial-ik",1,4);
      this->SetRendererName(renderer[2],"Sagittal-ij",2,5);
      break;
    case 3: // Polar
      displayMode->changeLabel(3,"Short-Axis");
      displayMode->changeLabel(4,"Radial-1");
      displayMode->changeLabel(5,"Radial-2");
      renderer[2]->SetLabel("Short-Axis",0.05,0.05,0.73,0.73,0.0);
      renderer[1]->SetLabel("Radial-2",0.05,0.05,0.73,0.73,0.0);
      renderer[0]->SetLabel("Radial-1",0.05,0.05,0.73,0.73,0.0);
      break;


    case 4: // Axial Transpose
      this->SetRendererName(renderer[1],"Sagital-ik",1,4);
      this->SetRendererName(renderer[0],"Coronal-jk",0,3);
      this->SetRendererName(renderer[2],"Axial-ji",2,5);
      break;
    case 5: // Coronal Transpose
      this->SetRendererName(renderer[1],"Sagittal-ki",1,4);
      this->SetRendererName(renderer[0],"Axial-jk",0,3);
      this->SetRendererName(renderer[2],"Coronal-ji",2,5);
      break;
    case 6: // Sagittal Tranpose
      this->SetRendererName(renderer[1],"Coronal-ki",1,4);
      this->SetRendererName(renderer[0],"Axial-kj",0,3);
      this->SetRendererName(renderer[2],"Sagittal-ji",2,5);
      break;

    }
  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::ModifyPolarImageSpacing()
{
  double sp[3]; currentImage->GetSpacing(sp);
  int  dim[3]; currentImage->GetDimensions(dim);
  sp[2]=float(dim[1]*sp[1])/float(dim[2]);
  sp[1]=float(dim[0]*sp[0])/float(dim[1]);
  currentImage->SetSpacing(sp);
  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::ResetViewerUpdatePolar()
{
  int   range[3]; 
  currentImage->GetDimensions(range);

  coordScale[0]->setLabel("Z-Coord");
  coordScale[1]->setLabel("Angle 1");
  coordScale[2]->setLabel("Angle 2");
  
  for (int ia=0;ia<=2;ia++)
    {
      renderer[ia]->SetClipMode(PXTrue);
      imageSlice[ia]->SetAutoUpdate(0);
      imageSlice[ia]->SetCurrentPlane(ia);
      renderer[ia]->SetPlaneMode(ia);
    }
  
  int levels[3];
  orthoSlice->GetLevels(levels);
  
  for (int sc=0;sc<=2;sc++)
    {
      int sp=1;
      if (sc>0)
	sp=2;
      levels[sc]=range[sp]/2;
      if (sc==1)
	levels[1]=0;
      coordScale[sc]->setRange(offset,range[sp]-1+offset);  
      coordScale[sc]->setValue(levels[sc]+offset);
      coordScale[sc]->setResolution(1.0);
      imageSlice[sc]->SetLevel(levels[sc]);
      imageSlice[sc]->Update();
      imageSlice[sc]->SetAutoUpdate(1);
    }

  orthoSlice->SetAutoUpdate(0);
  orthoSlice->SetLevels(levels[0],levels[1],levels[2]);
  orthoSlice->Update();
  orthoSlice->SetAutoUpdate(1);

  coordScale[3]->setRange(1,this->NumberOfFrames);
  coordScale[3]->setValue(this->CurrentFrame+offset);
  coordScale[3]->setResolution(1.0);


  if (range[2]==1)
    {
      if (display_mode!=5)
	{
	  display_mode=5;
	  AdjustViewports(display_mode);
	}
    }
  else  
    {
      display_mode=(int)displayMode->getIndex();
      AdjustViewports(display_mode);
    }
  for (int i=0;i<=3;i++)
    renderer[i]->Reset();

  this->MoveAxisPolar(1);
  return 1;
}
// ------------------------------------------------------------------------- 
int vtkpxGUIOrthogonalViewer::ResetViewerUpdateCartesian() 
{
  int   range[3]; 
  currentImage->GetDimensions(range);

  coordScale[0]->setLabel("i-Coord:");
  coordScale[1]->setLabel("j-Coord:");
  coordScale[2]->setLabel("k-Coord:");
  double sp[3]; currentImage->GetSpacing(sp);
  double ori[3];  currentImage->GetOrigin(ori);

  for (int ia=0;ia<=2;ia++)
    {
      imageSlice[ia]->SetAutoUpdate(0);
      imageSlice[ia]->SetCurrentPlane(ia);
      renderer[ia]->SetClipMode(PXTrue);
      renderer[ia]->SetPlaneMode(ia);
      renderer[ia]->SetClipThickness(sp[ia]);
    }
  
  orthoSlice->SetAutoUpdate(0);


  orthoSlice->SetLevels(range[0]/2,range[1]/2,range[2]/2);
  orthoSlice->Update();
  orthoSlice->SetAutoUpdate(1);

  for (int kk=0;kk<=2;kk++)
    {
      imageSlice[kk]->SetLevel(range[kk]/2);
      imageSlice[kk]->Update();
      imageSlice[kk]->SetAutoUpdate(1);
    }
  
  int levels[3];
  orthoSlice->GetLevels(levels);
  

  
  double lvv[3],lvs[3];
  for (int ic=0;ic<=2;ic++)
    {
      lvv[ic]=float(levels[ic]);
      lvs[ic]=lvv[ic]*sp[ic]+ori[ic];
    }
  this->SetLastClickedPoint(lvv);
  this->SetLastClickedPointScaled(lvs);
  
  coordScale[3]->setRange(1,this->NumberOfFrames);
  coordScale[3]->setValue(this->CurrentFrame+offset);
  coordScale[3]->setResolution(1.0);
  
  if (range[2]==1)
    {
      if (display_mode!=5)
	{
	  display_mode=5;
	  AdjustViewports(display_mode);
	}
    }
  else  
    {
      display_mode=(int)displayMode->getIndex();
      AdjustViewports(display_mode);
    }


  /*  if (this->Orientation==2)
    {
      vtkTransform* tr2=vtkTransform::New();
      fprintf(stderr,"Orientation =2 using flip transform\n");
      float ori[3]; currentImage->GetOrigin(ori);
      float sp[3];  currentImage->GetSpacing(sp);
      int  dim[3];  currentImage->GetDimensions(dim);

      float maxz=ori[2]+float(dim[2]-1)*sp[2];
      float minz=ori[2];
      float newz=-maxz;
      float offset=minz-newz;
      fprintf(stderr,"Bounds Orientation =2 using flip transform\n");
      tr2->Identity();
      tr2->PostMultiply();
      tr2->Scale(1,1,-1);
      tr2->Translate(0,0,offset);
      vtkpxCamera* cam=(vtkpxCamera*)(renderer[3]->GetRenderer()->GetActiveCamera());
      cam->SetUserTransform(tr2);
      tr2->Delete();    
    }
  else
    {
      vtkpxCamera* cam=(vtkpxCamera*)(renderer[3]->GetRenderer()->GetActiveCamera());
      cam->SetUserTransform(NULL);
      }*/

  for (int i=0;i<=3;i++)
    renderer[i]->Reset();
  this->MoveAxisCartesian(1);
  
  for (int sc=0;sc<=2;sc++)
    {
      coordScale[sc]->setRange(offset,range[sc]-1+offset);  
      coordScale[sc]->setValue(levels[sc]+offset);
      coordScale[sc]->setResolution(1.0);
    }

  


  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::ResetViewer()
{

  if (!(hasImage && this->Initialized))
      return 0;

  
  if (!hasImage)
    {
      oldorientation=-1;
      firsttimereset=-1;
    }


  if (this->Orientation==3)
    this->ModifyPolarImageSpacing();

  for (int i=0;i<=2;i++)
    {
      imageSlice[i]->SetInput(currentImage);
      imageSlice[i]->SetColorMode(this->CurrentImageIsColor);
      imageSlice[i]->SetFrame(this->CurrentFrame);
      imageSlice[i]->SetLookupTable(mainLookupTable);

      if (this->Orientation==3)
	imageSlice[i]->SetPolarMode(1);
      else
	imageSlice[i]->SetPolarMode(0);

      imageSlice[i]->Update();
      outlineFilter[i]->SetInput(currentImage);

      if (this->Orientation==3)
	outline[i]->SetVisibility(0);
      else
	outline[i]->SetVisibility(1);
    }

  orthoSlice->SetInput(currentImage);
  orthoSlice->SetColorMode(this->CurrentImageIsColor);
  orthoSlice->SetFrame(this->CurrentFrame);
  orthoSlice->SetLookupTable(mainLookupTable);

 
  if (this->Orientation==3)
    {
      orthoSlice->SetPolarMode(1);
    }
  else
    {
      orthoSlice->SetPolarMode(0);
    }
  outlineFilter[3]->SetInput(currentImage);
  

  if (this->Volume!=NULL && this->Orientation!=3)
    {
      this->Volume->SetInput(currentImage,this->Orientation);
      this->Volume->SetFrame(this->CurrentFrame);
      this->VolumeControl->Update(1);
      if (this->Volume->GetVisibility())
	this->Volume->Update();
    }

  if (this->obliqueSlice!=NULL && this->Orientation!=3)
    {
      this->obliqueSlice->SetAutoUpdate(0);
      this->obliqueSlice->SetColorMode(this->CurrentImageIsColor);
      this->obliqueSlice->SetInput(currentImage);
      this->obliqueSlice->SetFrame(this->CurrentFrame);
      this->obliqueSlice->SetLookupTable(mainLookupTable);
      this->ObliqueControl->Update(1);
      renderer[3]->SetCameraCallback(this->ObliqueControl,140);
      this->obliqueSlice->SetAutoUpdate(1);
    }



  int   range[3]; 
  currentImage->GetDimensions(range);

  int doupdate=1;
  if (firsttimereset==0)
    {
      int flag=0;
      for (int i=0;i<=2;i++)
	if (range[i]!=(int(coordScale[i]->getmax())+1-offset))
	  flag++;
      if (this->NumberOfFrames!=(int(coordScale[3]->getmax())))
	flag++;

      doupdate=(flag>0);
    }


  if (oldorientation!=this->Orientation)
    {
      doupdate=1;
      oldorientation=this->Orientation;
      this->ResetCoordinateScalesAndLabels();
    }

  if (range[2]==1 && display_mode!=5)
    doupdate=1;

  for (int ia=0;ia<=2;ia++)
    renderer[ia]->SetCurrentImageOrientation(currentImage,this->Orientation,doupdate);
  if (doupdate)
    renderer[3]->SetPlaneMode(2);
  renderer[3]->GetRenderer()->SetBackground(0.0,0.0,0.0);//0.2,0.2,0.3);
  renderer[3]->SetCurrentImageOrientation(currentImage,this->Orientation,doupdate);


  // Do something about Talairach ....
  if (this->talairachTransform!=NULL)
    {
      int md=this->talairachTransform->GetColinMode();
      if (md==1 || md==-1)
	{
	  this->talairachTransform->SetColinMode(md,this->currentImage);
	  this->talairachTransform->Modified();
	  this->UpdateTalairachGrid(1);
	}
      else
	{
	  this->talairachTransform->SetStandardModeOn();
	}
    }
    
  // ................................

  if (doupdate)
    {
      if (this->Orientation!=3)
	this->ResetViewerUpdateCartesian();
      else
	this->ResetViewerUpdatePolar();
    }

  if (!this->CurrentImageIsColor)
    this->ShowFrameControls(this->NumberOfFrames>1);
  else
    this->ShowFrameControls(0);
  
  firsttimereset=0;

  this->UpdateIntensityLabelInfo();


  if (this->talairachTransform==NULL)
    this->talairachTransform=vtkpxTalairachTransform::New();
  this->talairachTransform->SetColinMode(1,this->currentImage);
  this->talairachTransform->Modified();
  this->MoveAxis(1);


  if (this->NavigationPointer==NULL)
    this->CreateNavigationPointer();

  double spa[3]; this->currentImage->GetSpacing(spa);
  this->NavigationRadius=3.0*spa[0];
  this->NavigationRadius2=3.0*spa[0];
  this->NavigationRadius_X = this->NavigationRadius_Y = this->NavigationRadius_Z = 3.0*spa[0];


  


  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::MoveAxisPolar(int force)
{
  // Ultrasound Stuff
  for (int rend=0;rend<=2;rend++)
    for (int axis=0;axis<=1;axis++)
      {
	int index=rend*2+axis;
	if (axisActor[index]!=NULL)
	  axisActor[index]->SetVisibility(0);
      }
  
  int lv[3];
  for (int iaa=0;iaa<=2;iaa++)
    lv[iaa]=int(imageSlice[iaa]->GetLevel());
  
  for (int i=0;i<=3;i++)
    {
      this->renderer[i]->SetViewerCoordinates(lv[0],lv[1],lv[2]);
      if (i<3)
	renderer[i]->MoveAbove(-1);
    }
  
  if (this->createTalairachButtons)
    talairachlabel->setText("3D Echo");

  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::MoveAxisCartesian(int force)
{
  if (axisActor[0]==NULL)
    {
      // Create Axis 
      for (int rend=0;rend<=2;rend++)
	for (int axis=0;axis<=1;axis++)
	  {
	    int index=rend*2+axis;
	    axisActor[index]=vtkActor::New();
	    axisActor[index]->PickableOff();
	    renderer[rend]->GetRenderer()->AddActor(axisActor[index]);
	  }

      vtkAssembly* ass=vtkAssembly::New();
      for (int i=6;i<=9;i++)
	{
	  axisActor[i]=vtkActor::New();
	  axisActor[i]->PickableOff();
	  ass->AddPart(axisActor[i]);
	  axisActor[i]->SetVisibility(0);
	}
      renderer[3]->GetRenderer()->AddActor(ass);
      ass->Delete();
    }
  
  // Move Axis
  
  double sp[3];    currentImage->GetSpacing(sp);
  double ori[3];   currentImage->GetOrigin(ori);
  
  if (force==1)
    {
      float bounds[6];
      int   dim[3]; currentImage->GetDimensions(dim);
      float meansp=0.0;
      
      for (int k=0;k<=2;k++)
	{
	  bounds[2*k]  =      ori[k]-0.5*sp[k];
	  bounds[2*k+1]=float(dim[k]-0.5)*sp[k]+ori[k];
	  meansp+=0.333*sp[k];
	  //	  fprintf(stderr,"Bounds 2*k=%f Bounds 2*k+1 = %f\n",bounds[2*k],bounds[2*k+1]);
	}
      
      float thick=meansp*0.01;

      for (int axis=0;axis<=2;axis++)
	{
	  vtkCubeSource* cube=vtkCubeSource::New();
	  double c[3];
	  for (int ia=0;ia<=2;ia++)
	    c[ia]=0.0;
	  int pt[3];
	  
	  switch (axis)
	    {
	    case 0: // X-axis
	      cube->SetBounds(bounds[0],bounds[1], -thick,thick, -thick,thick);
	      pt[0]=2; pt[1]=4; pt[2]=6; c[0]=1.0;
	      break;
	    case 1:
	      cube->SetBounds(-thick,thick,bounds[2],bounds[3],	-thick,thick);
	      pt[0]=0; pt[1]=5; pt[2]=7; c[1]=1.0;
	      break;
	    case 2:
	      cube->SetBounds(-thick,thick,-thick,thick,bounds[4],bounds[5]);
	      pt[0]=1; pt[1]=3; pt[2]=8; c[2]=1.0;
	      break;
	    }
	  
	  for (int k=0;k<=2;k++)
	    {
	      int index=pt[k];
	      axisActor[index]->GetProperty()->SetColor(c);
	      axisActor[index]->GetProperty()->SetAmbient(1.0);
	      axisActor[index]->GetProperty()->SetDiffuse(0.0);
	      axisActor[index]->GetProperty()->SetSpecular(0.0);
	      
	      vtkPolyDataMapper *curveMapper = vtkPolyDataMapper::New();
	      curveMapper->SetInput(cube->GetOutput());
	      
	      axisActor[index]->SetMapper(curveMapper);
	      axisActor[index]->GetProperty()->SetOpacity(0.5);
	      axisActor[index]->GetProperty()->SetRepresentationToWireframe();
	      curveMapper->Delete();
	    }
	  cube->Delete();
	}
      
      // Now do the origin bit!!!
      vtkSphereSource* sphere=vtkSphereSource::New();
      sphere->SetRadius(3.0);	  
      vtkPolyDataMapper *map0=vtkPolyDataMapper::New();
      map0->SetInput(sphere->GetOutput());
      axisActor[9]->SetMapper(map0);
      map0->Delete();
      sphere->Delete();
      axisActor[9]->GetProperty()->SetColor(1.0,0.0,0.0);
    }
  
  
  double pos[3];
  int level[3];
  for (int ia=0;ia<=2;ia++)
    {
      int dim[3]; this->currentImage->GetDimensions(dim);
      level[ia]=Irange(imageSlice[ia]->GetLevel(),0,dim[ia]-1);
      pos[ia]=imageSlice[ia]->GetLevel()*sp[ia]+ori[ia];
    }
  
  axisActor[0]->SetPosition(pos[0],0.0,pos[2]);     // 
  axisActor[1]->SetPosition(pos[0],pos[1],0.0);
  axisActor[2]->SetPosition(0.0,pos[1],pos[2]);
  axisActor[3]->SetPosition(pos[0],pos[1],0.0);
  axisActor[4]->SetPosition(0.0,pos[1],pos[2]);
  axisActor[5]->SetPosition(pos[0],0.0,pos[2]);
  


  int lv[3];
  for (int iaa=0;iaa<=2;iaa++)
    {
      lv[iaa]=int(imageSlice[iaa]->GetLevel());
      //      fprintf(stderr,"Getting Level %d,%d\n",iaa,lv[iaa]);
    }
  


  for (int i=0;i<=3;i++)
    {
      this->renderer[i]->SetViewerCoordinates(lv[0],lv[1],lv[2]);
      renderer[i]->SetClipBounds(lv[0],lv[1],lv[2],sp,ori);
      if (i<3)
	renderer[i]->Clip();
    }
  
  if (this->SynchNavigationalPointerWithCrossHairs)
    this->MoveNavigationPointer();
  

  this->UpdateIntensityLabelInfo();
  this->UpdateDisplay(); 

  return 1;
}

// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::UpdateIntensityLabelInfo()
{
  double sp[3];    currentImage->GetSpacing(sp);
  double ori[3];   currentImage->GetOrigin(ori);
  int    dim[3];   currentImage->GetDimensions(dim);
  double pos[3];
  int level[3];
  for (int ia=0;ia<=2;ia++)
    {
      level[ia]=imageSlice[ia]->GetLevel();
      level[ia]=Irange(level[ia],0,dim[ia]-1);
      pos[ia]=double(level[ia])*sp[ia]+ori[ia];
    }

  if (this->createTalairachButtons)
    {
      if (this->talairachTransform==NULL)
	{
	  talairachlabel->setText("");
	}
      else
	{
	  this->DisplayNeuroCoordinates(pos,level,talairachlabel,mnilabel);
	}
    }
  else
    {
      double vl=0.0;
      if (this->valueImage!=NULL)
	vl=valueImage->GetScalarComponentAsDouble(level[0],level[1],level[2],this->CurrentFrame);
      else
	vl=currentImage->GetScalarComponentAsDouble(level[0],level[1],level[2],this->CurrentFrame);
      
      std::string objlabel=this->GetObjectmapValueAsText(level[0],level[1],level[2]);
      sprintf(pxtk_buffer,"(%d %d %d) %s %s",level[0],level[1],level[2],this->GetFormatedNumber(vl,3),objlabel.c_str());
      talairachlabel->setText(pxtk_buffer);
    }

  return 1;
}

// -------------------------------------------------------------------------
void vtkpxGUIOrthogonalViewer::MoveAxis(int force)
{
  if (!hasImage)
      return;

  //fprintf(stderr,"In Move Axis %d\n",force);

  if (this->Orientation!=3)
    this->MoveAxisCartesian(force);
  else
    this->MoveAxisPolar(force);


  if (this->EventParent!=NULL && this->CallbackNo>0)
    {
      this->EventParent->HandleEvent(CallbackNo);
    }
  else
    {
      if (this->CallbackName!=NULL)
	PXTkApp::executeTclCommand(this->CallbackName);
    }
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonalViewer::FindCentroid(vtkImageData* img,int location[3],double lvv[3],int radius)
{
  int min_loc[3],max_loc[3],idim[3];
  img->GetDimensions(idim);

  for (int k=0;k<=2;k++)
    {
      min_loc[k]=Irange(location[k]-radius,0,idim[k]-2);
      max_loc[k]=Irange(location[k]+radius,min_loc[k]+1,idim[k]-1);
    }


  vtkpxImageExtractVOI* voi=vtkpxImageExtractVOI::New();
  voi->SetInput(img);
  voi->SetVOI(min_loc[0],max_loc[0],
	      min_loc[1],max_loc[1],
	      min_loc[2],max_loc[2]);
  voi->Update();

	      
  vtkImageThreshold* thr=vtkImageThreshold::New();
  thr->SetInput(voi->GetOutput());
  thr->ThresholdByUpper(1);
  thr->SetInValue(1);
  thr->SetOutValue(0);
  thr->ReplaceInOn();
  thr->ReplaceOutOn();
  thr->SetOutputScalarTypeToUnsignedChar();
  
  vtkImageSeedConnectivity* connectivity=vtkImageSeedConnectivity::New();
  connectivity->SetInput(thr->GetOutput());
  connectivity->SetInputConnectValue(1);
  connectivity->AddSeed(location[0]-min_loc[0],location[1]-min_loc[1],location[2]-min_loc[2]);
  connectivity->SetOutputConnectedValue(100);
  connectivity->SetOutputUnconnectedValue(0);
  connectivity->SetDimensionality(3);
  connectivity->Update();


  vtkImageData* mask=connectivity->GetOutput();
  int dim[3]; mask->GetDimensions(dim);

  //  fprintf(stderr,"dim= %d x %d x %d\n",dim[0],dim[1],dim[2]);

  double sum[3], wsum=0.0;
  for (int j=0;j<=2;j++)
    sum[j]=0.0;


  for (int ia=0;ia<dim[0];ia++)
    for (int ib=0;ib<dim[1];ib++)
      for (int ic=0;ic<dim[2];ic++)
	{
	  double v=mask->GetScalarComponentAsDouble(ia,ib,ic,0);

	  if (v>0)
	    {
	      double w=1.0;
	      if (this->TraceVolumeWeightedMode)
		w=fabs(v);

	      sum[0]+=double(ia)*w;
	      sum[1]+=double(ib)*w;
	      sum[2]+=double(ic)*w;
	      wsum+=w;
	    }
	}

  double ori[3];  mask->GetOrigin(ori); 
  double spa[3];  mask->GetSpacing(spa);
  for (int ia=0;ia<=2;ia++)
    lvv[ia]=0.0;

  
  /*  fprintf(stderr,"Ori=%.2f,%.2f,%.2f spa=%.2f,%.2f,%.2f np=%d sum=%d %d %d\n",
	  ori[0],ori[1],ori[2],
	  spa[0],spa[1],spa[2],
	  np,
	  sum[0],sum[1],sum[2]);*/

  if (wsum>0.0)
    {
      for (int j=0;j<=2;j++)
	{
	  location[j]=int(sum[j]/wsum+0.5)+min_loc[j];
	  lvv[j]=ori[j]+spa[j]*sum[j]/wsum;
	}
      fprintf(stderr,"Loc = %d %d %d (Raw = %.3f %.3f %.3f)\n",
	      location[0],location[1],location[2],
	      lvv[0],lvv[1],lvv[2]);

    }

  //  this->SetLastClickedPoint(lvv);


  thr->Delete();
  voi->Delete();
  connectivity->Delete();
  return (wsum>0.0);

}
// ------------------------------------------------------------------------- 
vtkCamera* vtkpxGUIOrthogonalViewer::GetCurrentCamera()
{

  if (renderer[3]==NULL)
    return NULL;
  
  if (renderer[3]->GetRenderer()==NULL)
    return NULL;

  return renderer[3]->GetRenderer()->GetActiveCamera();
}
// ------------------------------------------------------------------------- 
int vtkpxGUIOrthogonalViewer::CopyCamera(vtkpxGUIOrthogonalViewer* other)
{
  return this->CopyCamera(other->GetCurrentCamera());
}

int vtkpxGUIOrthogonalViewer::CopyCamera(vtkCamera* othercamera)
{
  vtkCamera* thiscamera=this->GetCurrentCamera();

  if (thiscamera==NULL || othercamera==NULL)
    return 0;

  vtkpxGUIRenderer::CopyCamera(thiscamera,othercamera);
  return 1;
}
// ------------------------------------------------------------------------- 
void vtkpxGUIOrthogonalViewer::SetLookupTable(vtkLookupTable* lkp)
{
  vtkpxGUIBaseImageViewer::SetLookupTable(lkp);

  for (int ia=0;ia<=2;ia++)
    {
      if (imageSlice[ia]!=NULL) 
	{
	  imageSlice[ia]->UpdateTexture();
	  imageSlice[ia]->Modified();
	}
    }

  if (orthoSlice)
    orthoSlice->Modified();

  if (this->obliqueSlice)
    this->obliqueSlice->Modified();

  this->UpdateDisplay();
}

// ------------------------------------------------------------------------- 
void vtkpxGUIOrthogonalViewer::SetNavigationPointerGeometryToEllipse(double radius_x, double radius_y, double radius_z)
{
  //  printf(" in ellipse\n");
  this->NavigationPointerActorMode=2;
  this->NavigationRadius_X =radius_x;
  this->NavigationRadius_Y =radius_y;
  this->NavigationRadius_Z =radius_z;

  this->UpdateNavigationActorGeometry(0);
}


// ------------------------------------------------------------------------- 
void vtkpxGUIOrthogonalViewer::SetNavigationPointerGeometryToCone(double radius)
{
  //  printf(" in ellipse\n");
  this->NavigationPointerActorMode=2;
  this->NavigationRadius=radius;
  this->UpdateNavigationActorGeometry(0);
}

void vtkpxGUIOrthogonalViewer::SetNavigationPointerGeometryToCylinder(double radius)
{
  //printf(" in cylinder\n");
  this->NavigationPointerActorMode=3;
  this->NavigationRadius=radius;
  this->UpdateNavigationActorGeometry(0);
}


void vtkpxGUIOrthogonalViewer::SetNavigationPointerGeometryToSphere(double radius)
{
  this->NavigationPointerActorMode=0;
  this->NavigationRadius=radius;
  this->UpdateNavigationActorGeometry(0);
}

void vtkpxGUIOrthogonalViewer::SetNavigationPointerGeometryToDoubleSphere(double radius1, double radius2)
{
  this->NavigationPointerActorMode=4;
  this->NavigationRadius=radius1;
  this->NavigationRadius2=radius2;
  this->UpdateNavigationActorGeometry(0);
}

void vtkpxGUIOrthogonalViewer::SetNavigationPointerGeometryToCube(double cubelength)
{
  this->NavigationPointerActorMode=1;
  this->NavigationLength = cubelength;
  this->UpdateNavigationActorGeometry(0);

}
// ------------------------------------------------------------------------- 
int vtkpxGUIOrthogonalViewer::UpdateNavigationActorGeometry(int updatevolumecrop)
{

  if (this->NavigationPointer==NULL)
    return 0;

  if (this->NavigationPointerGeometry==NULL)
    this->NavigationPointerGeometry=vtkPolyData::New();

  if (this->NavigationPointerGeometry2==NULL)
    this->NavigationPointerGeometry2=vtkPolyData::New();
  
  
  double p1[3];   this->NavigationPointer->GetPoint1(p1);
  double p2[3];   this->NavigationPointer->GetPoint2(p2);

  for (int ia=0;ia<=2;ia++)
    {
      this->NavigationPointerTip[ia]=p2[ia];
      this->NavigationPointerBase[0]=p1[ia];   
    }


  double cnt[3],n[3],dist[3];
  double len=sqrt(vtkMath::Distance2BetweenPoints(p2,p1));
  for (int ia=0;ia<=2;ia++)
    {
      cnt[ia]=0.5*(p2[ia]+p1[ia]);
      dist[ia]=p2[ia]-p1[ia];
      n[ia]=dist[ia];
    }
  vtkMath::Normalize(n);

  
  if (this->NavigationPointerActorMode<0 || this->NavigationPointerActorMode>4)
    this->NavigationPointerActorMode=0;

  switch (this->NavigationPointerActorMode)
    {
    case 0: // Sphere
      {
      	if (this->NavigationSphereSource==NULL)
      	  this->NavigationSphereSource=vtkSphereSource::New();
      	
      	this->NavigationSphereSource->SetRadius(this->NavigationRadius-1);
      	this->NavigationSphereSource->SetCenter(p1);
      	this->NavigationSphereSource->SetPhiResolution(18);
      	this->NavigationSphereSource->SetThetaResolution(18);
      	this->NavigationSphereSource->Update();
      	this->NavigationPointerGeometry->ShallowCopy(this->NavigationSphereSource->GetOutput());
      	
      	if(updatevolumecrop)
      	  this->Volume->SetCropValues(0, this->NavigationRadius, 10, 10, 10, 10, 10, 10, 10, 10, n);		
      }
      break;
   
  case 4: // Double Sphere
      {
        if (this->NavigationSphereSource==NULL)
          this->NavigationSphereSource=vtkSphereSource::New();
        
        this->NavigationSphereSource->SetRadius(this->NavigationRadius);
        this->NavigationSphereSource->SetCenter(p1);
        this->NavigationSphereSource->SetPhiResolution(18);
        this->NavigationSphereSource->SetThetaResolution(18);
        this->NavigationSphereSource->Update();
        this->NavigationPointerGeometry->ShallowCopy(this->NavigationSphereSource->GetOutput());
        
        if (this->NavigationSphereSource2==NULL)
          this->NavigationSphereSource2=vtkSphereSource::New();
        
        this->NavigationSphereSource2->SetRadius(this->NavigationRadius2);
        this->NavigationSphereSource2->SetCenter(p2);
        this->NavigationSphereSource2->SetPhiResolution(18);
        this->NavigationSphereSource2->SetThetaResolution(18);
        this->NavigationSphereSource2->Update();
        this->NavigationPointerGeometry2->ShallowCopy(this->NavigationSphereSource2->GetOutput());

        if(updatevolumecrop)
          this->Volume->SetCropValues(0, this->NavigationRadius, 10, 10, 10, 10, 10, 10, 10, 10, n);    
      }
      break;

    case 1: // Cube
      {
	if (this->NavigationCubeSource==NULL)
	  this->NavigationCubeSource=vtkCubeSource::New();
	
	this->NavigationCubeSource->SetXLength(2.0*fabs(dist[0]));
	this->NavigationCubeSource->SetYLength(2.0*fabs(dist[1]));
	this->NavigationCubeSource->SetZLength(2.0*fabs(dist[2]));
	this->NavigationCubeSource->SetCenter(p2);
	this->NavigationCubeSource->Update();
	this->NavigationPointerGeometry->ShallowCopy(this->NavigationCubeSource->GetOutput());

	//printf("updatevolume crop %d\n", updatevolumecrop); 

	//if(updatevolumecrop)
	  this->Volume->SetCropValues(1, 10, 2.0*fabs(dist[0]), 2.0*fabs(dist[1]), 2.0*fabs(dist[2]), 10, 10, 10, 10, 10, n);

      }
      break;

	  case 2: // Ellipse
      {

	  	if (this->NavigationEllipseSource==NULL)
			this->NavigationEllipseSource=vtkEllipseSource::New();	

		this->NavigationEllipseSource->SetRadiusX(this->NavigationRadius_X); 
		this->NavigationEllipseSource->SetRadiusY(this->NavigationRadius_Y); 
		this->NavigationEllipseSource->SetRadiusZ(this->NavigationRadius_Z); 
		this->NavigationEllipseSource->SetCenter(p1); 

		this->NavigationEllipseSource->SetPhiResolution(18);
		this->NavigationEllipseSource->SetThetaResolution(18);
		this->NavigationEllipseSource->Update();
		this->NavigationPointerGeometry->ShallowCopy(this->NavigationEllipseSource->GetOutput());

		if(updatevolumecrop)
			this->Volume->SetCropValues(2, 10, 10, 10, 10, this->NavigationRadius_X, this->NavigationRadius_Y, this->NavigationRadius_Z, 10, 10, n);		
      }
      break;


    case 3: // Cylinder
      {
	if (this->NavigationCylinderSource==NULL)
	  this->NavigationCylinderSource=vtkCylinderSource::New();

	this->NavigationCylinderSource->SetCenter(0,0,0);
	this->NavigationCylinderSource->SetRadius(this->NavigationRadius);		
	this->NavigationCylinderSource->SetResolution(12); 
	this->NavigationCylinderSource->SetHeight(len);
	this->NavigationCylinderSource->Update();


	if (this->NavigationCylinderTransform==NULL)
	  {
	    this->NavigationCylinderTransform=vtkTransform::New();
	    this->NavigationCylinderTransform->PostMultiply();
	  }
	this->NavigationCylinderTransform->Identity();

	double cr[3];
	double ax[3]={0,1,0};
	vtkMath::Cross(n,ax,cr);
	double t=acos(vtkMath::Dot(n,ax))*180.0/vtkMath::Pi();

	this->NavigationCylinderTransform->RotateWXYZ(-t,cr[0],cr[1],cr[2]);
      	this->NavigationCylinderTransform->Translate(cnt);
	
	  
	if (this->NavigationCylinderFilter==NULL)
	  {
	    this->NavigationCylinderFilter=vtkTransformPolyDataFilter::New();
	  }
	this->NavigationCylinderFilter->SetInput(this->NavigationCylinderSource->GetOutput());
	this->NavigationCylinderFilter->SetTransform(NavigationCylinderTransform);
	this->NavigationCylinderFilter->Update();
	this->NavigationPointerGeometry->ShallowCopy(this->NavigationCylinderFilter->GetOutput());

	if(updatevolumecrop)
	  this->Volume->SetCropValues(3, 10, 10, 10, 10, 10, 10, 10, this->NavigationRadius, len, n);
      }
      break;
    }
      
  if (this->NavigationPointerActor==NULL)
    {
      vtkPolyDataMapper *map=vtkPolyDataMapper::New();
      map->SetInput(this->NavigationPointerGeometry);
      this->NavigationPointerActor=vtkActor::New();
      this->NavigationPointerActor->SetMapper(map);
      this->NavigationPointerActor->GetProperty()->SetOpacity(0.35);
      this->NavigationPointerActor->GetProperty()->SetColor(1.0, 0.8, 0.65);
      renderer[3]->GetRenderer()->AddActor(this->NavigationPointerActor);
      this->NavigationPointerActor->SetVisibility(0);
    }
  else
    {
      vtkPolyDataMapper *map=(vtkPolyDataMapper*)this->NavigationPointerActor->GetMapper();
      map->SetInput(this->NavigationPointerGeometry);
      this->NavigationPointerActor->Modified();
    }

  this->NavigationPointerActor->Modified();

  if (this->NavigationPointerActor2==NULL)
    {
      vtkPolyDataMapper *map2=vtkPolyDataMapper::New();
      map2->SetInput(this->NavigationPointerGeometry2);
      this->NavigationPointerActor2=vtkActor::New();
      this->NavigationPointerActor2->SetMapper(map2);
      this->NavigationPointerActor2->GetProperty()->SetOpacity(0.35);
      this->NavigationPointerActor2->GetProperty()->SetColor(0.85, 0.9, 0.23);
      renderer[3]->GetRenderer()->AddActor(this->NavigationPointerActor2);
      this->NavigationPointerActor2->SetVisibility(0);
    }
  else
    {
      vtkPolyDataMapper *map2=(vtkPolyDataMapper*)this->NavigationPointerActor2->GetMapper();
      map2->SetInput(this->NavigationPointerGeometry2);
      this->NavigationPointerActor2->Modified();
    }

  this->NavigationPointerActor2->Modified();


  //if(updatevolumecrop)
  //{  
    this->Volume->SetProbeLocation(p2);
  //}
return 1;
}
// -----------------------------------------------------------------------------------------------

int vtkpxGUIOrthogonalViewer::RenderNavigationPointer(int dm)
{
  if (dm>=0 && dm<=4 && dm!=this->NavigationPointerDrawMode)
    this->SetNavigationPointerDrawMode(dm);

  // Line Widget
  if (this->NavigationPointerDrawMode==0 || this->NavigationPointerDrawMode==2 || this->NavigationPointerDrawMode==4)
    this->NavigationPointer->Off();
  else
    this->NavigationPointer->On();

  // Pointer Actor = Decoration
  if (this->NavigationPointerDrawMode==0 || this->NavigationPointerDrawMode==1)
    {
      this->NavigationPointerActor->SetVisibility(0);
      this->NavigationPointerActor2->SetVisibility(0);
      this->renderer[3]->HideLabel();

    }
  else if (this->NavigationPointerDrawMode==2)
    {
      this->NavigationPointerActor->GetProperty()->SetRepresentationToSurface();
      this->NavigationPointerActor->GetProperty()->SetDiffuse(1.0);
      this->NavigationPointerActor->GetProperty()->SetAmbient(0.0);
      this->NavigationPointerActor->GetProperty()->SetSpecular(0.0);
      this->NavigationPointerActor->SetVisibility(1);

      this->NavigationPointerActor2->GetProperty()->SetRepresentationToSurface();
      this->NavigationPointerActor2->GetProperty()->SetDiffuse(1.0);
      this->NavigationPointerActor2->GetProperty()->SetAmbient(0.0);
      this->NavigationPointerActor2->GetProperty()->SetSpecular(0.0);
      this->NavigationPointerActor2->SetVisibility(1);
    }
  else
    {
      this->NavigationPointerActor->GetProperty()->SetRepresentationToWireframe();
      this->NavigationPointerActor->GetProperty()->SetLineWidth(0.5);
      this->NavigationPointerActor->GetProperty()->SetAmbient(1.0);
      this->NavigationPointerActor->GetProperty()->SetDiffuse(0.0);
      this->NavigationPointerActor->GetProperty()->SetSpecular(0.0);
      this->NavigationPointerActor->SetVisibility(1);

      this->NavigationPointerActor2->GetProperty()->SetRepresentationToWireframe();
      this->NavigationPointerActor2->GetProperty()->SetLineWidth(0.5);
      this->NavigationPointerActor2->GetProperty()->SetAmbient(1.0);
      this->NavigationPointerActor2->GetProperty()->SetDiffuse(0.0);
      this->NavigationPointerActor2->GetProperty()->SetSpecular(0.0);
      this->NavigationPointerActor2->SetVisibility(1);
    }

  this->RenderDelay(100);

  return 1; 
}


int vtkpxGUIOrthogonalViewer::SetNavigationalPointerFromTool(vtkPoints* pts  )
{
  if (pts==NULL)
    return 0;

  if (pts->GetNumberOfPoints()<2)
    return 0;

  double tip[3];   pts->GetPoint(0,tip);
  double base[3];  pts->GetPoint(1,base);
  return this->SetNavigationalPointerFromTool(tip,base);

}

int vtkpxGUIOrthogonalViewer::SetNavigationalPointerFromTool(vtkTransform* tr)
{
  if (tr==NULL)
    return 0;

  double base[3] = { 0.0 , -40 , 0.0};
  double tip[3] = {  0.0 , 0.0 , 0.0 };

  float sc[3];  tr->GetScale(sc);
  double sum=0.0;
  for (int ia=0;ia<=2;ia++)
    sum+=fabs(sc[ia]-100.0);

  //fprintf(stderr,"Scales = %.2f,%.2f,%.2f sum=%f\n",sc[0],sc[1],sc[2],sum);


  if (sum>0.001)
    base[1]=-1.0;

  tr->TransformPoint(base,base);
  tr->TransformPoint(tip,tip);
  return this->SetNavigationalPointerFromTool(tip,base);
}

int vtkpxGUIOrthogonalViewer::SetNavigationalPointerFromTool(double p2[3],double p1[3])
{
  
  double cnt[3],n[3],dist[3];
  double len=sqrt(vtkMath::Distance2BetweenPoints(p2,p1));
  for (int ia=0;ia<=2;ia++)
    {
      cnt[ia]=0.5*(p2[ia]+p1[ia]);
      dist[ia]=p2[ia]-p1[ia];
      n[ia]=dist[ia];
    }
  vtkMath::Normalize(n);

  if (this->NavigationConeSource==NULL)
    this->NavigationConeSource=vtkConeSource::New();
  

  this->NavigationConeSource->SetHeight(len);
  this->NavigationConeSource->SetRadius(this->NavigationRadius);
  this->NavigationConeSource->SetCenter(cnt);
  this->NavigationConeSource->SetDirection(n);
  this->NavigationConeSource->CappingOn();
  this->NavigationConeSource->SetResolution(36);
  this->NavigationConeSource->Update();
  this->NavigationPointerGeometry->ShallowCopy(this->NavigationConeSource->GetOutput());

  this->RenderNavigationPointer(2);

  return 1; 
}

void vtkpxGUIOrthogonalViewer::SetBackgroundColor(vtkRenderWindow* renwin)
{
  vtkRenderer* ren=renderer[0]->GetRenderer();

  double c[3];   ren->GetBackground(c);
  int a=this->EventManager->d_colorBox("Background Color",c[0],c[1],c[2]);

  if (a==0)
    return;

  for (int i=0;i<=3;i++)
    renderer[i]->GetRenderer()->SetBackground(c);
  this->KeyRenderer->SetBackground(c);
  this->BackgroundRenderer->SetBackground(c);
  render_widget->getRenderWindow()->Render();  
}

