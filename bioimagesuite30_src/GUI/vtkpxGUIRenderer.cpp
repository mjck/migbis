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



/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxGUIRenderer.cpp,v $
  Language:  C++
  Date:      $Date: 2003/05/15 15:33:58 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkpxGUIRenderer.h"
#include "vtkObjectFactory.h"
#include "pxtkcomplexwidgets.h"
#include <vtkVRMLExporter.h>
#include <vtkTextMapper.h>
#include <vtkTIFFWriter.h> 
#include <vtkJPEGWriter.h> 
#include <vtkIVExporter.h> 
#include <vtkActor2D.h>
#include <vtkRendererSource.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkMath.h>
#include "pxutil.h"
#include "pxtkapp.h"
#include "vtkImageConstantPad.h"
#include "pxitkcombobox.h"
#include "vtkCommand.h"
#include "vtkTransform.h"
#include "vtkActorCollection.h"
#include "vtkVolumeCollection.h"
#include "vtkTextProperty.h"
#include "vtkpxGUIBaseImageViewer.h"
#include "vtkGenericRenderWindowInteractor.h"
#include "vtkInteractorObserver.h"

// ------------------------------------- Helper Class ------------------------------

class vtkpxGUIRendererCallback : public vtkCommand
{
public:

  static vtkpxGUIRendererCallback *New() {
    return new vtkpxGUIRendererCallback;
  }
  vtkTypeMacro(vtkpxGUIRendererCallback,vtkCommand);
  virtual void AttachFilter(vtkpxGUIRenderer* guiren0) { this->guiren=guiren0; }
  virtual void Execute(vtkObject *caller,unsigned long event, void* vtkNotUsed(v)) {
    
    if (this->guiren==NULL)
      return;
    if (caller)
      this->guiren->RendererEvent();
  }

protected:

  vtkpxGUIRendererCallback() {
    this->guiren=NULL;
  }

  vtkpxGUIRendererCallback(const vtkpxGUIRendererCallback&) {};
  void operator=(const vtkpxGUIRendererCallback&) {};

  vtkpxGUIRenderer*   guiren;
};



// ---------------------------------------------------------------------------------


vtkpxGUIRenderer* vtkpxGUIRenderer::New()
{
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIRenderer");
  if(ret)
      {
	return (vtkpxGUIRenderer*)ret;
      }
  return new vtkpxGUIRenderer;
}

// -----------------------------------------------------------------------------------
// Construct object with no children.
// -----------------------------------------------------------------------------------
vtkpxGUIRenderer::vtkpxGUIRenderer()
{
  this->markerWidget=NULL;

  this->renderer=NULL;
  this->CurrentImage=NULL;
  this->textActor=NULL;
  this->main_light=NULL;
  this->mode_menu=NULL;
  this->mode_button=NULL;
  this->plane_button=NULL;
  this->plane_menu=NULL;
  this->angle_menu=NULL;
  this->camera_menu=NULL;
  this->last_axis=-1;

  this->flip_mode=0;
  this->clip_mode=PXFalse;
  this->vertical_decoration=PXFalse;
  this->no_gui=PXFalse;
  this->plane_mode=-1;
  this->orientation=0;

  this->NoPolyClipControls=0;
  
  for (int k=0;k<3;k++)
      {
	this->bounds[2*k]=0;
	this->bounds[2*k+1]=1.0;
	this->clipBounds[k]=0.5;
      }

  this->update_rate[0]=0.1;
  this->update_rate[1]=15.0;
  this->enableClip=NULL;

  this->clipping_thickness=2.0;
  this->clipping_front=-1.0;
  this->camera_scale=1.0;
  this->camera_mode=1;
  this->auto_zoom=1.0;

  this->clip_min=NULL;
  this->clip_thickness=NULL;
  this->hasclipcontrols=0;

  this->ViewerCoordinates[0]=0;
  this->ViewerCoordinates[1]=0;
  this->ViewerCoordinates[2]=0;

  this->CameraEventParent=NULL;
  this->CameraCallbackNo=-1;
  this->CameraCallbackName=NULL;
  this->CameraPresets=vtkpxCameraInfo::New();
  this->RendererCallback=NULL;
  this->ActorTransform=NULL;
  this->EnableActorTransform=0;

  for (int ia=0;ia<=2;ia++)
    {
      for (int ib=0;ib<=1;ib++)
	{
	  int index=2*ia+ib;
	  this->ClippingPlanes[index]=vtkPlane::New();
	  double p[3]= { 0.0,0.0,0.0};
	  double n[3]= { 0.0,0.0,0.0};
	  if (ib==1)
	    {
	      p[ia]=1.0;
	      n[ia]=-1.0;
	    }
	  else
	    {
	      n[ia]=1.0;
	    }

	  this->ClippingPlanes[index]->SetOrigin(p);
	  this->ClippingPlanes[index]->SetNormal(n);
	}
    }
}
// -----------------------------------------------------------------------------------
vtkpxGUIRenderer::~vtkpxGUIRenderer()
{
  if (this->renderer)
    this->renderer->Delete();
  if (this->textActor)
    this->textActor->Delete();
  if (this->main_light)
    this->main_light->Delete();

  if (this->CameraCallbackName!=NULL)
    delete [] this->CameraCallbackName;


  if (this->RendererCallback!=NULL)
    this->RendererCallback->Delete();

  this->SetCurrentImage(NULL);
  this->CameraPresets->Delete();

  if (this->ActorTransform!=NULL)
    this->ActorTransform->Delete();

  for (int ia=0;ia<=5;ia++)
    {
      if (this->ClippingPlanes[ia]!=NULL)
	this->ClippingPlanes[ia]->Delete();
    }
}
// -----------------------------------------------------------------------
void vtkpxGUIRenderer::SetCameraCallback(vtkpxGUIComponent* comp,int call_no)
{
  this->CameraEventParent=comp;
  this->CameraCallbackNo=call_no;
}

void vtkpxGUIRenderer::SetCameraCallback(const char* name)
{
  if (name==NULL)
    return;

  if (this->CameraCallbackName!=NULL)
    delete [] this->CameraCallbackName;

  this->CameraCallbackName=new char[strlen(name)+1];
  strcpy(this->CameraCallbackName,name);
}
// -----------------------------------------------------------------------  

// -------------------------------------------------------------------
//  
//    Enable/Disable orientation marker to renderer
//
// -------------------------------------------------------------------
void vtkpxGUIRenderer::EnableOrientationMarker(int enable)
{
	if (markerWidget)
		this->markerWidget->SetEnabled(this->enablemarker->getState() && enable);
}

// -------------------------------------------------------------------
//  
//    Add orientation marker to renderer
//
// -------------------------------------------------------------------
void vtkpxGUIRenderer::CreateOrientationMarker()
{
  // marker widget
  	vtkGenericRenderWindowInteractor *markerInteractor = vtkGenericRenderWindowInteractor::New();
  	markerInteractor->SetRenderWindow(this->GetRenderer()->GetRenderWindow());
  	markerInteractor->SetInteractorStyle(NULL);
  	//markerInteractor->Initialize();

  	markerWidget = vtkmpj3DOrientationMarkerWidget::New();
  	markerWidget->SetInteractor(markerInteractor);
  	markerWidget->SetEnabled(0);
}
// -------------------------------------------------------------------
//  
//    I n i t i a l i z e    ( and GUI if present)
//
// -------------------------------------------------------------------
char* vtkpxGUIRenderer::Initialize(const char* widgetname,vtkRenderer* ren,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();

  this->renderer=ren;
  vtkpxGUIComponent::Initialize(widgetname,inside);
  
  this->RendererCallback=vtkpxGUIRendererCallback::New();
  this->RendererCallback->AttachFilter(this);
  this->renderer->AddObserver(vtkCommand::StartEvent,this->RendererCallback);
  
  if (inside==0)
    this->EventManager->setTitle("3D Renderer Controls");
  
  PXTkFrame* manager=(PXTkFrame*)this->EventManager->getMainWindow();

  // Just add Light and make sure camera is parallel

  main_light=vtkLight::New();
  renderer->AddLight(main_light);
  renderer->GetActiveCamera()->ParallelProjectionOn();

  // Add Controls here 
  // -----------------

  if (no_gui==PXFalse)
      {
	PXTkDualArrowLabel* zoom,*rotate,*rotate2;
	PXTkFrame* topframe,*bottomframe,*middleframe;

	if (vertical_decoration==PXFalse)
	  {
	    topframe=new PXTkFrame(this->EventManager,manager);
	    middleframe=new PXTkFrame(this->EventManager,manager);
	    bottomframe=new PXTkFrame(this->EventManager,manager);
	    manager->addChildren("-side top -expand true -fill x ",topframe,middleframe,bottomframe);      
	  } 
	else
	  {
	    topframe=manager;
	    middleframe=manager;
	    bottomframe=manager;
	  }

	if (vertical_decoration)
	  {
	    zoom=new PXTkDualArrowLabel(this->EventManager,topframe,"Zm",102,101,1);
	    rotate=new PXTkDualArrowLabel(this->EventManager,topframe,"Az",106,107,1);
	    rotate2=new PXTkDualArrowLabel(this->EventManager,topframe,"El",116,117,1);
	  }
	else
	  {
	    zoom=new PXTkDualArrowLabel(this->EventManager,topframe,"Zm",102,101,0);
	    rotate=new PXTkDualArrowLabel(this->EventManager,topframe,"Az",106,107,0);
	    rotate2=new PXTkDualArrowLabel(this->EventManager,topframe,"El",116,117,0);
	    this->angle_menu=new PXTkOptionMenu(this->EventManager,topframe,"5.0",-1);
	    this->angle_menu->addOption("10.0");
	    this->angle_menu->addOption("15.0");
	    this->angle_menu->addOption("30.0");
	    this->angle_menu->addOption("45.0");
	    this->angle_menu->addOption("60.0");
	    this->angle_menu->addOption("90.0");
	    this->angle_menu->addOption("1200.0");
	  }
	
	plane_button=new PXTkButton(this->EventManager,topframe,"Rt",103); 
	
	SetPlaneMode(plane_mode);
	plane_button->setPadding(1,3,3);

	PXTkButton* cp=new PXTkButton(this->EventManager,topframe,"Cl",123);

	if (vertical_decoration)
	  {
	    topframe->addChildren("-side top -expand false ",zoom,rotate,this->angle_menu,rotate2);     
 	    topframe->addChildren("-side top -expand false ",plane_button,cp);      
	  }
	else
	  {
	    topframe->addChildren("-side left -expand false ",zoom,rotate,this->angle_menu,rotate2);     
 	    topframe->addChildren("-side left -expand false ",plane_button,cp);      
	  }

	if (plane_mode==-1)
	    {
	      plane_menu=new PXITkComboBox(this->EventManager,middleframe,"View:",109);
	      plane_menu->addOption("XY",109);
	      plane_menu->addOption("XZ",109);
	      plane_menu->addOption("XY",109);
	      plane_menu->addOption("-YZ",109);
	      plane_menu->addOption("-XZ",109);
	      plane_menu->addOption("-XY",109);
	      int tmp=this->orientation;
	      this->orientation=-1;
	      this->SetOrientation(tmp);

	      PXTkButton* ad=new PXTkButton(this->EventManager,middleframe,"Add",110); 
	      update_plane=new PXTkButton(this->EventManager,middleframe,"Upd",111); 
	      PXTkButton* ld=new PXTkButton(this->EventManager,middleframe,"Load",112); 
	      PXTkButton* sv=new PXTkButton(this->EventManager,middleframe,"Save",113); 
	      
	      if (vertical_decoration)
		{
		  middleframe->addChildren("-side top -expand false",plane_menu,ad);      
		  middleframe->addChildren("-side top -expand false",update_plane,ld,sv);
		}
	      else
		{
		  middleframe->addChildren("-side left -expand false",plane_menu,ad);      
		  middleframe->addChildren("-side left -expand false",update_plane,ld,sv);
		}
	      plane_menu->selectItem(2);
	      plane_menu->enableEntry(PXFalse);
	      update_plane->enable(PXFalse);
	    }

	PXTkButton* save=new PXTkButton(this->EventManager,topframe,"Sv",114);
	save->setPadding(1,3,3);

	if (vertical_decoration)
	    {
	      mode_button=new PXTkButton(this->EventManager,topframe,"Nr",105);
	      mode_button->setPadding(1,3,3);
	      topframe->addChildren("-side top ",mode_button,save);      
	    }
	else
	    {
	      mode_menu=new PXTkOptionMenu(this->EventManager,topframe,"Flipped",104);
	      mode_menu->addOption("Normal");
	      topframe->addChildren("-side left",mode_menu,save);      
	    }

	// mpj - 07/23/2014
  	camera_menu = new PXTkOptionMenu(this->EventManager,topframe,"Parallel",133);
	camera_menu->addOption("Perspective");
	topframe->addChildren("-side left",camera_menu, save); 
	// mpj
  

	if (!flip_mode)
	    SetFlipMode(flip_mode);
	
	if (!this->EventManager->isInside())
	    {
	      PXTkButton* cl=new PXTkButton(this->EventManager,topframe,"Ok",3);
	      if (vertical_decoration)
		{
		  topframe->addChildren("-side top",cl);
		}
	      else
		{
		  cl->setText("Close");
		  topframe->addChildren("-side left ",cl);
		}
	    } 
	
	if (bottomframe!=topframe)
	  {
	    PXTkFrame* b2=new PXTkFrame(this->EventManager,manager);
	    PXTkFrame* b3=new PXTkFrame(this->EventManager,manager);
	    bottomframe->addChildren("-side right -expand f" ,b3);
	    bottomframe->addChildren("-side left -expand t -fill x",b2);
	    
	    PXTkButton* rt=new PXTkButton(this->EventManager,b3,"Rt",122,PXTrue);
	    b3->addChildren("-side right -expand f",rt);
	    
	    clip_min=new PXTkArrowScale(this->EventManager,b2,"Near Plane:",120,PXTrue);
	    clip_thickness=new PXTkArrowScale(this->EventManager,b2,"Thickness:",121,PXTrue);
	    
	    clip_min->setLengthWidth(120,10);
	    clip_thickness->setLengthWidth(150,10);
	    clip_thickness->setRange(0.01,200.0);
	    clip_min->setIncrement(1);
	    clip_thickness->setIncrement(1);
	    clip_min->setRange(0.01,200.0);
	    clip_thickness->setValue(this->clipping_thickness);
	    this->hasclipcontrols=1;

	    b2->addChildren("-side top -expand t -fill x",clip_min->getMainWindow(),clip_thickness->getMainWindow());

	    if (this->NoPolyClipControls==0)
	      {
		
		PXTkLabelFrame* labfr=new PXTkLabelFrame(this->EventManager,b2,"Polygonal Clipping");
		b2->addChildren("-side top -expand false -fill x",labfr);
		
		for (int ia=0;ia<=2;ia++)
		  {
		    PXTkFrame* fr=new PXTkFrame(this->EventManager,labfr);
		    labfr->addChildren("-side top -expand false -fill x",fr);
		    for (int ib=0;ib<=1;ib++)
		      {
			int index=ia*2+ib;
			if (ia==0)
			  strcpy(pxtk_buffer4,"x-");
			else if (ia==1)
			  strcpy(pxtk_buffer4,"y-");
			else
			  strcpy(pxtk_buffer4,"z-");
			
			if (ib==0)
			  sprintf(pxtk_buffer4,"%s-min",pxtk_buffer4);
			else
			  sprintf(pxtk_buffer4,"%s-max",pxtk_buffer4);
			
			clip_planes[index]=new PXTkArrowScale(this->EventManager,fr,pxtk_buffer4,125);
			clip_planes[index]->setRange(0,400);
			clip_planes[index]->setLengthWidth(120,10);
			//		    clip_planes[index]->setDigits(4);
			clip_planes[index]->setResolution(1);
		      }
		    fr->addChildren("-side left -expand true -fill x ",clip_planes[2*ia]->getMainWindow(),
				    clip_planes[2*ia+1]->getMainWindow());
		  }
		this->enableClip=new PXTkCheckButton(this->EventManager,labfr,"Enable Polygonal Clipping",124);
		this->enableClip->setState(PXFalse);
		labfr->addChildren("-side bottom -expand false -fill x",this->enableClip);
	      }

	    // added light change support - mpj 04/17/2014
		PXTkLabelFrame* ambfr=new PXTkLabelFrame(this->EventManager,b2,"Ambient light");
		b2->addChildren("-side top -expand false -fill x",ambfr);

		double rgb[3];
		main_light->GetAmbientColor(rgb);

		ambient[0] = new PXTkArrowScale(this->EventManager,ambfr,"R:",130,PXFalse);
		ambient[0]->setRange(0.0, 1.0);
		ambient[0]->setDecimalPoints(3);
		//ambient[0]->setLengthWidth(120, 10);
		ambient[0]->setIncrement(1);
		ambient[0]->setValue(rgb[0]);

		ambient[1] = new PXTkArrowScale(this->EventManager,ambfr,"G:",130,PXFalse);
		ambient[1]->setRange(0.0, 1.0);
		ambient[1]->setDecimalPoints(3);
		//ambient[1]->setLengthWidth(120, 10);
		ambient[1]->setIncrement(0.1);
		ambient[1]->setValue(rgb[1]);

		ambient[2] = new PXTkArrowScale(this->EventManager,ambfr,"B:",130,PXFalse);
		ambient[2]->setRange(0.0, 1.0);
		ambient[2]->setDecimalPoints(3);
		//ambient[2]->setLengthWidth(120, 10);
		ambient[2]->setIncrement(0.1);
		ambient[2]->setValue(rgb[2]);

	    b2->addChildren("-side left -expand true -fill x",
	    	ambient[0]->getMainWindow(),ambient[1]->getMainWindow(),ambient[2]->getMainWindow());


		PXTkLabelFrame* difffr=new PXTkLabelFrame(this->EventManager,b2,"Diffuse light");
		b2->addChildren("-side top -expand false -fill x",difffr);

		main_light->GetDiffuseColor(rgb);

		diffuse[0] = new PXTkArrowScale(this->EventManager,difffr,"R:",130,PXFalse);
		diffuse[0]->setRange(0.0, 1.0);
		diffuse[0]->setDecimalPoints(3);
		//diffuse[0]->setLengthWidth(120, 10);
		diffuse[0]->setIncrement(1);
		diffuse[0]->setValue(rgb[0]);

		diffuse[1] = new PXTkArrowScale(this->EventManager,difffr,"G:",130,PXFalse);
		diffuse[1]->setRange(0.0, 1.0);
		diffuse[1]->setDecimalPoints(3);
		//diffuse[1]->setLengthWidth(120, 10);
		diffuse[1]->setIncrement(0.1);
		diffuse[1]->setValue(rgb[1]);

		diffuse[2] = new PXTkArrowScale(this->EventManager,difffr,"B:",130,PXFalse);
		diffuse[2]->setRange(0.0, 1.0);
		diffuse[2]->setDecimalPoints(3);
		//diffuse[2]->setLengthWidth(120, 10);
		diffuse[2]->setIncrement(0.1);
		diffuse[2]->setValue(rgb[2]);

	    b2->addChildren("-side left -expand true -fill x",
	    	diffuse[0]->getMainWindow(),diffuse[1]->getMainWindow(),diffuse[2]->getMainWindow());


		PXTkLabelFrame* specfr=new PXTkLabelFrame(this->EventManager,b2,"Specular light");
		b2->addChildren("-side top -expand false -fill x",specfr);

		main_light->GetSpecularColor(rgb);

		specular[0] = new PXTkArrowScale(this->EventManager,specfr,"R:",130,PXFalse);
		specular[0]->setRange(0.0, 1.0);
		specular[0]->setDecimalPoints(3);
		//specular[0]->setLengthWidth(120, 10);
		specular[0]->setIncrement(1);
		specular[0]->setValue(rgb[0]);

		specular[1] = new PXTkArrowScale(this->EventManager,specfr,"G:",130,PXFalse);
		specular[1]->setRange(0.0, 1.0);
		specular[1]->setDecimalPoints(3);
		//specular[1]->setLengthWidth(120, 10);
		specular[1]->setIncrement(0.1);
		specular[1]->setValue(rgb[1]);

		specular[2] = new PXTkArrowScale(this->EventManager,specfr,"B:",130,PXFalse);
		specular[2]->setRange(0.0, 1.0);
		specular[2]->setDecimalPoints(3);
		//specular[2]->setLengthWidth(120, 10);
		specular[2]->setIncrement(0.1);
		specular[2]->setValue(rgb[2]);

	    b2->addChildren("-side left -expand true -fill x",
	    	specular[0]->getMainWindow(),specular[1]->getMainWindow(),specular[2]->getMainWindow());


	    PXTkLabelFrame* intfr=new PXTkLabelFrame(this->EventManager,b2,"Light intensity");
		b2->addChildren("-side top -expand false -fill x",intfr);

		double intensity = main_light->GetIntensity();

		light_intensity = new PXTkArrowScale(this->EventManager,intfr,"Value:",132,PXFalse);
		light_intensity->setRange(0.0, 1.0);
		light_intensity->setDecimalPoints(3);
		//specular[0]->setLengthWidth(120, 10);
		light_intensity->setIncrement(1);
		light_intensity->setValue(intensity);

		b2->addChildren("-side left -expand true -fill x", light_intensity->getMainWindow());

		PXTkLabelFrame* orientfr=new PXTkLabelFrame(this->EventManager,b2,"Orientation marker");
		b2->addChildren("-side top -expand false -fill x",orientfr);

		PXTkFrame* enablefr=new PXTkFrame(this->EventManager,orientfr);
		orientfr->addChildren("-side top -expand false -fill x",enablefr);
		enablemarker=new PXTkCheckButton(this->EventManager,enablefr,"Enable marker",140);
		enablemarker->setState(PXTrue);
		enablelabels=new PXTkCheckButton(this->EventManager,enablefr,"Enable axes labels",141);
		enablelabels->setState(PXTrue);
		PXTkButton* colorsph=new PXTkButton(this->EventManager,enablefr,"Sphere color",148); 
		colorsph->setPadding(1,3,3);
		b2->addChildren("-side left -expand false -fill x -padx 7 -padx 2",enablemarker, enablelabels, colorsph);

		PXTkFrame* xfr=new PXTkFrame(this->EventManager,orientfr);
		orientfr->addChildren("-side top -expand false -fill x",xfr);
		PXTkLabel *lblx = new PXTkLabel(this->EventManager, xfr, "Label x:");
		b2->addChildren("-side left -expand false -fill x",lblx);
		labelx=new PXTkEntry(this->EventManager,xfr);
		labelx->setText("X");
		b2->addChildren("-side left -expand true -fill x",labelx);
		PXTkButton* updatex=new PXTkButton(this->EventManager,xfr,"Update",142); 
		updatex->setPadding(1,3,3);
		PXTkButton* colorx=new PXTkButton(this->EventManager,xfr,"Color",143); 
		colorx->setPadding(1,3,3);
		b2->addChildren("-side left -expand false -fill x -padx 7 -padx 2",updatex, colorx);

		PXTkFrame* yfr=new PXTkFrame(this->EventManager,orientfr);
		orientfr->addChildren("-side top -expand false -fill x",yfr);
		PXTkLabel *lbly = new PXTkLabel(this->EventManager, yfr, "Label y:");
		b2->addChildren("-side left -expand false -fill x",lbly);
		labely=new PXTkEntry(this->EventManager,yfr);
		labely->setText("Y");
		b2->addChildren("-side left -expand true -fill x",labely);
		PXTkButton* updatey=new PXTkButton(this->EventManager,yfr,"Update",144); 
		updatey->setPadding(1,3,3);
		PXTkButton* colory=new PXTkButton(this->EventManager,yfr,"Color",145); 
		colory->setPadding(1,3,3);
		b2->addChildren("-side left -expand false -fill x -padx 7 -padx 2",updatey, colory);

		PXTkFrame* zfr=new PXTkFrame(this->EventManager,orientfr);
		orientfr->addChildren("-side top -expand false -fill x",zfr);
		PXTkLabel *lblz = new PXTkLabel(this->EventManager, zfr, "Label z:");
		b2->addChildren("-side left -expand false -fill x",lblz);
		labelz=new PXTkEntry(this->EventManager,zfr);
		labelz->setText("Z");
		b2->addChildren("-side left -expand true -fill x",labelz);
		PXTkButton* updatez=new PXTkButton(this->EventManager,zfr,"Update",146); 
		updatez->setPadding(1,3,3);
		PXTkButton* colorz=new PXTkButton(this->EventManager,zfr,"Color",147); 
		colorz->setPadding(1,3,3);
		b2->addChildren("-side left -expand false -fill x -padx 7 -padx 2",updatez, colorz);

	  }
      }
  View();
  return GetWidgetName();
}
/* -------------------------------------------------------------------------

    R e n d e r    S t u f f

 -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::Render()
{
  if (renderer==NULL)
    return;

  vtkCamera* camera=renderer->GetActiveCamera();
  main_light->SetPosition(camera->GetPosition());
  main_light->SetFocalPoint(camera->GetFocalPoint());
  renderer->GetRenderWindow()->Render();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::RenderDelay(int t)
{
  Render();
  /*if (renderer!=NULL)
    Tcl_CreateTimerHandler(t,vtkpxGUIRenderer::HandleRenderEvent,this);*/
}

void vtkpxGUIRenderer::HandleRenderEvent(void *arg1)
{
  ((vtkpxGUIRenderer*)arg1)->Render();
}

/* -------------------------------------------------------------------------*/
//
//   A c c e s s  (Get/Set) Stuff from Outside 
//
/* -------------------------------------------------------------------------*/
vtkRenderer* vtkpxGUIRenderer::GetRenderer()
{
  return renderer;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::GetBounds(double bd[6])
{
 for (int k=0;k<6;k++)
      bd[k]=bounds[k];
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIRenderer::GetFlipMode()
{
  return flip_mode;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIRenderer::GetPlaneMode()
{
  return plane_mode;
}
/* -------------------------------------------------------------------------*/
double vtkpxGUIRenderer::GetCameraScale()
{
  return camera_scale;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetLabel(const char* text,double x,double y,double c1,double c2,double c3)
{
  if (renderer==NULL)
      return;
  
  vtkTextMapper* tm=vtkTextMapper::New();

  if (tm==NULL)
      return;

  int havetextactor=1;

  tm->SetInput(text);
  if (textActor==NULL)
      {
	havetextactor=0;
	textActor=vtkActor2D::New();
      }
  
  textActor->SetMapper(tm);
  tm->GetTextProperty()->BoldOn();
  tm->GetTextProperty()->SetFontFamilyToArial();
  tm->GetTextProperty()->ShadowOff();
  tm->GetTextProperty()->SetFontSize(14);

  textActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
  textActor->SetPosition(x,y);
  textActor->GetProperty()->SetColor(c1,c2,c3);
  textActor->GetProperty()->SetOpacity(1);
  textActor->GetProperty()->SetDisplayLocationToForeground();

  textActor->SetVisibility(1);
  //  textActor->Update();
  
  
  if (havetextactor==0)
    renderer->AddActor2D(textActor);

  tm->Delete();
}
 
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::HideLabel()
{
  if (textActor!=NULL)
      textActor->SetVisibility(0);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::ShowLabel()
{
  if (textActor!=NULL)
      textActor->SetVisibility(1);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetFlipMode(int mode)
{
  flip_mode=mode;
  if (mode_menu!=NULL)
      {
	if (flip_mode)
	    mode_menu->setIndex(0);
	else
	    mode_menu->setIndex(1);
      }
  else if (mode_button!=NULL)
      {
	if (flip_mode)
	    mode_button->setText("Nr");
	else
	    mode_button->setText("Fl");
      }

  if (renderer!=NULL)
      MoveAbove(plane_mode);
}
/* -------------------------------------------------------------------------*/
int  vtkpxGUIRenderer::GetOrientation()
{
  return this->orientation;
}
// -----------------------------------------------------------------------------------
void vtkpxGUIRenderer::SetOrientation(int orient,int doupdate)
{
  int orn=this->orientation;
  this->orientation=orient;

  if (orn!=this->orientation && this->plane_menu!=NULL)
    {
      int old=plane_menu->getIndex();
      int oldstyleorient=vtkpxGUIBaseImageViewer::MapOrientationToOldStyle(this->orientation);

      if (this->orientation>=40)
	oldstyleorient+=4;

      switch(oldstyleorient)
	{
	case 0:
	  plane_menu->changeLabel(0,"Sagittal-YZ");
	  plane_menu->changeLabel(1,"Coronal-XZ");
	  plane_menu->changeLabel(2,"Axial-XY");
	  plane_menu->changeLabel(3,"Rev Sagittal-YZ");
	  plane_menu->changeLabel(4,"Rev Coronal-XZ");
	  plane_menu->changeLabel(5,"Rev Axial-XY");
	  break;
	case 1:
	  plane_menu->changeLabel(0,"Sagittal-YZ");
	  plane_menu->changeLabel(1,"Axial-XZ");
	  plane_menu->changeLabel(2,"Coronal-XY");
	  plane_menu->changeLabel(3,"Rev Sagittal-YZ");
	  plane_menu->changeLabel(4,"Rev Axial-XZ");
	  plane_menu->changeLabel(5,"Rev Coronal-XY");
	  break;
	case 2:
	  plane_menu->changeLabel(0,"Coronal-YZ");
	  plane_menu->changeLabel(1,"Axial-XZ");
	  plane_menu->changeLabel(2,"Sagittal-XY");
	  plane_menu->changeLabel(3,"Rev Coronal-YZ");
	  plane_menu->changeLabel(4,"Rev Axial-XZ");
	  plane_menu->changeLabel(5,"Rev Sagittal-XY");
	  break;
	case 3:
	  plane_menu->changeLabel(0,"Short Axis");
	  plane_menu->changeLabel(1,"Radial-1");
	  plane_menu->changeLabel(2,"Radial-2");
	  plane_menu->changeLabel(3,"Rev Short-Axis");
	  plane_menu->changeLabel(4,"Rev Radial-1");
	  plane_menu->changeLabel(5,"Rev Radial-2");

	  // Tranposed Axuak
	case 4:
	  plane_menu->changeLabel(0,"Coronal-YZ");
	  plane_menu->changeLabel(1,"Sagittal-XZ");
	  plane_menu->changeLabel(2,"Axial-YX");
	  plane_menu->changeLabel(3,"Rev Coronal-YZ");
	  plane_menu->changeLabel(4,"Rev Sagittal-XZ");
	  plane_menu->changeLabel(5,"Rev Axial-YX");
	  break;
	case 5:
	  plane_menu->changeLabel(0,"Axial-YZ");
	  plane_menu->changeLabel(1,"Sagittal-ZX");
	  plane_menu->changeLabel(2,"Coronal-YX");
	  plane_menu->changeLabel(4,"Rev Sagittal-ZX");
	  plane_menu->changeLabel(3,"Rev Axial-YZ");
	  plane_menu->changeLabel(5,"Rev Coronal-YX");
	  break;
	case 6:
	  plane_menu->changeLabel(1,"Coronal-ZX");
	  plane_menu->changeLabel(0,"Axial-ZY");
	  plane_menu->changeLabel(2,"Sagittal-YX");
	  plane_menu->changeLabel(4,"Rev Coronal-ZX");
	  plane_menu->changeLabel(3,"Rev Axial-ZY");
	  plane_menu->changeLabel(5,"Rev Sagittal-YX");
	  break;
	  break;
	}
      plane_menu->setIndex(old);
    }


  if (renderer!=NULL && doupdate>0)
      MoveAbove(plane_mode);
}

/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetClipMode(PXBool m)
{
  clip_mode=m;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetPlaneMode(int m)
{
  if (m>5)
    {
      int preset=m-6;
      this->CameraPresets->UpdateCamera(this->renderer->GetActiveCamera(),preset);
      this->Render();
      return;
    }

  plane_mode=Irange(m,-1,5);
  
  if (plane_button!=NULL)
      {
	switch(plane_mode)
	    {
	    case 2:
	    case 5:
	      plane_button->setText("XY");
	      break;
	    case 1:
	    case 4:
	      plane_button->setText("XZ");
	      break;
	    case 0:
	    case 3:
	      plane_button->setText("YZ");
	      break;
	    case -1:
	      plane_button->setText("Va");
	      break;
	    }
      }
  
  camera_scale=GetCameraWidth();
  if (renderer!=NULL)
    MoveAbove(plane_mode);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetCameraScale(double autoz)
{
  camera_scale=autoz;
}

void vtkpxGUIRenderer::SetAutoZoom(double autoz)
{
  auto_zoom=Frange(autoz,0.5,2.0);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetCameraScale(vtkpxGUIRenderer* ren)
{
  if (ren==NULL || renderer==NULL)
      return;

  double scale2= ren->GetCameraScale();
  double wd1   = this->GetCameraWidth();
  double wd2   = ren->GetCameraWidth();

  if (wd1>0 && wd2>0)
      {
	double scale=wd1*scale2/wd2;
	Zoom(scale,PXTrue);
      }

}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetViewport(double x0,double y0,double x1,double y1)
{
  if (renderer!=NULL)
      {
	renderer->SetViewport(x0,y0,x1,y1);
	renderer->Render();
      }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetUpdateRate(double interactiverate,double staticrate)
{
  if (renderer!=NULL)
      renderer->GetRenderWindow()->SetDesiredUpdateRate(staticrate);

  update_rate[0]=staticrate;  update_rate[1]=interactiverate;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetVerticalDecoration(PXBool m)
{
  if (this->Initialized==0)
      vertical_decoration=m;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetNoGUI(PXBool gui)
{
  no_gui=PXTrue;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetBounds(double bd[6])
{
  for (int k=0;k<6;k++)
    this->bounds[k]=bd[k];

  this->ResetClipScales();

}
/* -------------------------------------------------------------------------*/ 
void vtkpxGUIRenderer::SetBounds(vtkDataSet* dataset)
{
  dataset->GetBounds(this->bounds);
  this->ResetClipScales();
}
/* -------------------------------------------------------------------------*/ 
double vtkpxGUIRenderer::GetClipFront()
{
  return clipping_front;
}

double vtkpxGUIRenderer::GetClipThickness()
{
  return clipping_thickness;
}

void vtkpxGUIRenderer::SetClipFront(double fr)
{
  clipping_front=fr;
}

void vtkpxGUIRenderer::SetClipThickness(double thick)
{
  if (thick<0.0)
    {
      if (this->orientation!=3 || this->CurrentImage==NULL && plane_mode!=-1)
	switch (plane_mode)
	  {
	  case 2:
	  case 5:
	    thick=this->CurrentImage->GetSpacing()[2];
	    break;
	  case 1:
	  case 4:
	    thick=this->CurrentImage->GetSpacing()[1];
	    break;
	  case 0:
	  case 3:
	    thick=this->CurrentImage->GetSpacing()[0];
	    break;
	  }
    }
  clipping_thickness=thick;
  View();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetClipBounds(double x,double y,double z)
{
  clipBounds[0]=x;
  clipBounds[1]=y;
  clipBounds[2]=z;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetClipBounds(int x,int y,int z,double sp[3],double ori[3])
{
  clipBounds[0]=x*sp[0]+ori[0];
  clipBounds[1]=y*sp[1]+ori[1];
  clipBounds[2]=z*sp[2]+ori[2];
}  
/* -------------------------------------------------------------------------*/
int vtkpxGUIRenderer::InViewport(int x1,int x2)
{
  if (renderer==NULL)
      return 0;
  
  int *sz=renderer->GetRenderWindow()->GetSize();
  double* vp=renderer->GetViewport();

  // Check for inside the viewpoer stuff
  if (x1<vp[0]*sz[0] || x1>vp[2]*sz[0])
      return 0;
  if (x2<vp[1]*sz[1] || x2>vp[3]*sz[1])
      return 0;

  return 1;
}
/* -------------------------------------------------------------------------
   
   M o u s e    C a l l b a c k   H a n d l i ng 

 -------------------------------------------------------------------------*/

int vtkpxGUIRenderer::MouseEventRotate(int state,int x1,int x2)
{

  static int oldx1=-1,oldx2=-1;
  static double deltaElevation=0.1,deltaAzimuth=0.1;

  switch (state)
    {
    case 0:
      {
	oldx1=x1;
	oldx2=x2;
	renderer->GetRenderWindow()->SetDesiredUpdateRate(update_rate[1]);
	
	int *sz=renderer->GetRenderWindow()->GetSize();
	double* vp=renderer->GetViewport();
	
	double vsize[2];
	vsize[0]=sz[0]*(vp[2]-vp[0]);
	vsize[1]=sz[1]*(vp[3]-vp[1]);
	
	deltaElevation = -20.0/ vsize[1]*  10.0;
	deltaAzimuth   = -20.0/ vsize[0] * 10.0;
      }
      break;
      
    case 1:
      {
	vtkCamera* camera=renderer->GetActiveCamera();

	double rxf = (double)(x1 - oldx1) * deltaAzimuth;
	double ryf = (double)(x2 - oldx2) * deltaElevation;
	
	camera->Azimuth(rxf);
	camera->Elevation(ryf);
	camera->OrthogonalizeViewUp();
	
	oldx1=x1;
	oldx2=x2;
	this->CameraModifiedCallback();
	Render();
      }
      break;
    case 2:
      renderer->GetRenderWindow()->SetDesiredUpdateRate(update_rate[0]);
      this->CameraModifiedCallback();
      Render();
      break;
    }

   vtkRenderWindowInteractor *markerInteractor = markerWidget->GetInteractor();
   markerInteractor->SetEventPosition(x1,x2);

   switch(state)
	{
    	case 0: 
    		markerInteractor->LeftButtonPressEvent();
    		break;
    	case 1: 
    		markerInteractor->MouseMoveEvent();
    		break;
    	case 2:
    		markerInteractor->LeftButtonReleaseEvent();
    		break;
    	default:
    		break;
	};

  return 1;

}

int vtkpxGUIRenderer::MouseEventPan(int state,int x1,int x2)
{
  static int oldx1=-1,oldx2=-1;
  switch (state)
    {
    case 0:
      oldx1=x1;
      oldx2=x2;
      renderer->GetRenderWindow()->SetDesiredUpdateRate(update_rate[1]);
      break;
      
    case 1:
      {
	vtkCamera* camera=renderer->GetActiveCamera();
	double FPoint[3],PPoint[3];
	camera->GetFocalPoint(FPoint);
	camera->GetPosition(PPoint);

	renderer->SetWorldPoint(FPoint[0],FPoint[1],FPoint[2],1.0);
	renderer->WorldToDisplay();
	
	double DPoint[3],focalDepth;
	
	this->renderer->GetDisplayPoint(DPoint);
	focalDepth=DPoint[2];

	/*sprintf(line,"FPoint=(%.1f,%.1f,%.1f) PPoint=(%.1f,%.1f,%.1f) focDepth=%.1f\n",
	  FPoint[0],FPoint[1],FPoint[2],
	  PPoint[0],PPoint[1],PPoint[2],focalDepth);
	  PXTkApp::printToConsole(line);*/

	int *sz=renderer->GetRenderWindow()->GetSize();
	double* vp=renderer->GetViewport();

	double WindowCenterX=0.5*(vp[0]+vp[2])*sz[0];
	double WindowCenterY=0.5*(vp[1]+vp[3])*sz[1];

	double APoint0=WindowCenterX + (x1 - oldx1 );
	double APoint1=WindowCenterY + (x2 - oldx2 );

	this->renderer->SetDisplayPoint(APoint0,APoint1,focalDepth);
	this->renderer->DisplayToWorld();

	double RPoint[4];
	this->renderer->GetWorldPoint(RPoint);

	if ( RPoint[3] != 0.0 )
	  {
	    for (int ia=0;ia<=2;ia++)
	      RPoint[ia]/=RPoint[3];
	  }
	camera->SetFocalPoint(
			      (FPoint[0]-RPoint[0])/2.0+FPoint[0],
			      (FPoint[1]-RPoint[1])/2.0+FPoint[1],
			      (FPoint[2]-RPoint[2])/2.0+FPoint[2]);
	camera->SetPosition(
			    (FPoint[0]-RPoint[0])/2.0+PPoint[0],
			    (FPoint[1]-RPoint[1])/2.0+PPoint[1],
			    (FPoint[2]-RPoint[2])/2.0+PPoint[2]); 

	oldx1=x1;
	oldx2=x2;
	this->CameraModifiedCallback();
	Render();
      }
      break;
    case 2:
      renderer->GetRenderWindow()->SetDesiredUpdateRate(update_rate[0]);
      this->CameraModifiedCallback();
      Render();
      break;
    }
  return 1;
}

int vtkpxGUIRenderer::MouseEventZoom(int state,int x1,int x2)
{
  static int oldx1=-1,oldx2=-1;
  switch (state)
    {
    case 0:
      oldx1=x1;
      oldx2=x2;
      renderer->GetRenderWindow()->SetDesiredUpdateRate(update_rate[1]);
      break;
      
    case 1:
      {
	double zoomFactor=pow(1.02,0.5*(x2-oldx2));
	vtkCamera* camera=renderer->GetActiveCamera();
	//printf("Parallel=%lf, Perspective=%lf\n",camera->GetParallelScale()*zoomFactor,zoomFactor);
	if (camera->GetParallelProjection() ) 
	  {
	    double parallelScale=camera->GetParallelScale()*zoomFactor;
	    camera->SetParallelScale(parallelScale);
	  } 
	else 
	  {
	    camera->Dolly(1/zoomFactor);
	    this->ResetCameraClippingRange();
	  }
	oldx1=x1;
	oldx2=x2;
	Render();
      }
      break;

    case 2:
      renderer->GetRenderWindow()->SetDesiredUpdateRate(update_rate[0]);
      Render();
      break;
    }
  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIRenderer::ShowCameraControls()
{
  if (!this->EventManager->isInside() && !no_gui)
    {
      this->EventManager->popupDialog();
      return 1;
    }
  return 0;
}

/* -------------------------------------------------------------------------*/
int vtkpxGUIRenderer::HandleMouseButtonEvent(int nbutton,int state,int x1,int x2,PXTkEventObj* ev)
{
  if (!InViewport(x1,x2))
      return 0;
  
  if (renderer==NULL)
      return 0;

  if( nbutton==6 && state==0)
    {
      return this->ShowCameraControls();
    }

  if (nbutton==5 && state==0)
    {
      this->Reset();
      return 1;
    }

  if (nbutton==2 || nbutton==9)
    return this->MouseEventZoom(state,x1,x2);
  
  if (nbutton==1 || nbutton==4)
    return this->MouseEventRotate(state,x1,x2);
  
  if (nbutton==3)
    return this->MouseEventPan(state,x1,x2);
  
  return 0;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIRenderer::Get2DClickedPoint(double& px,double& py,int mousex,int mousey,int orient)
{
  px=0.0;    py=0.0;	

  if (!InViewport(mousex,mousey))
      return 0;


  if (orient==-1)
    {
      orient=last_axis;
      if (orient==-1)
	orient=2;
    }
  else
    {
      orient=vtkpxGUIBaseImageViewer::MapOrientationToOldStyle(orient);
    }

  vtkCoordinate* coord=vtkCoordinate::New();
  coord->SetCoordinateSystemToDisplay();
  coord->SetValue(mousex,mousey,0.0);
  double* out=coord->GetComputedWorldValue(renderer);
  //  fprintf(stderr,"out=%.2f,%.2f,%.2f inp=%d,%d\n",out[0],out[1],out[2],mousex,mousey);

  switch (orient)
      {
      case 0: // yz plane
	 px=out[1];
	 py=out[2];
	 break;
      case 1: // xz plane
	 px=out[0];
	 py=out[2];
	 break;
      case 2: // xy plane
	 px=out[0];
	 py=out[1];
	 break;
       }
  coord->Delete();
  return 1;
  
}
/* -------------------------------------------------------------------------*/
//
//   Generic Operations View/Reset/Zoom
//
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::View(int axis)
{
  if (renderer==NULL)
    return;
  
  if (axis==-1)
    last_axis=plane_mode;
  else
    last_axis=axis;
  
  if (axis!=-1)
    {
      MoveAbove(last_axis);
      this->CameraModifiedCallback();
    }
  else 
    {
      this->ResetCamera();
      this->CameraModifiedCallback();
      RenderDelay();
    }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::Reset()
{
  SetCameraScale(GetCameraWidth());
  View(plane_mode);
  RenderDelay();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::Zoom(double scale,PXBool dorender)
{
  if (renderer==NULL)
      return;

  if (scale<1.0)
      scale=GetCameraWidth();
  else
      scale=Frange(scale,0.001,10000.0);

  // mpj - 07/23/2014
  if (camera_mode) {
  	renderer->GetActiveCamera()->SetParallelScale(scale);
  	camera_scale=renderer->GetActiveCamera()->GetParallelScale();
  } 
  // mpjg

  if (dorender)
      RenderDelay(); 
}

void vtkpxGUIRenderer::Rotate(double angle,int doazimuth)
{
  this->Rotate(angle,doazimuth,PXTrue);
}

void vtkpxGUIRenderer::Rotate(double angle,int doazimuth,PXBool dorender)
{
  if (renderer==NULL)
      return;

  if ( (angle==1000.0 || angle==-1000.0) && this->angle_menu!=NULL )
    {
      int opt=angle_menu->getIndex();
      float sgn=2.0*(angle>0)-1.0;
      switch (opt)
	{
	case 0:	  angle=5.0; break;
	case 1:	  angle=10.0; break;
	case 2:	  angle=15.0; break;
	case 3:	  angle=30.0; break;
	case 4:	  angle=45.0; break;
	case 5:	  angle=60.0; break;
	case 6:	  angle=90.0; break;
	case 7:	  angle=120.0; break;
	}
      angle=sgn*angle;
    }
  else
    angle=Frange(angle,-30.0,30.0);
  
  if (doazimuth)
    renderer->GetActiveCamera()->Azimuth(angle);
  else
    renderer->GetActiveCamera()->Elevation(angle);
  renderer->GetActiveCamera()->OrthogonalizeViewUp();
  if (dorender)
    RenderDelay(); 
}
/* -------------------------------------------------------------------------*/
// Save as TiFF
/* -------------------------------------------------------------------------*/
int vtkpxGUIRenderer::SaveAsTIFF(const char* fname,PXBool wholewindow)
{
  if (renderer==NULL)
      return 0;

  strcpy(pxtk_buffer,fname);
  char* ln=eatsuffix(fname);
  sprintf(pxtk_buffer,"%s.tif",ln);
  delete [] ln;
  
  renderer->GetRenderWindow()->Render();
  char line[400];
  strncpy(line,fname,400);
  PXTkApp::executeTclCommand("update idletasks");

  vtkRendererSource* renSrc=vtkRendererSource::New();
  renSrc->SetInput(renderer);
  if (wholewindow)
    renSrc->WholeWindowOn();
  else
    renSrc->WholeWindowOff();
  renSrc->Update();	

  int dim[3]; renSrc->GetOutput()->GetDimensions(dim);
  
  vtkImageConstantPad* pad=vtkImageConstantPad::New();
  pad->SetInput(renSrc->GetOutput());
  
  vtkImageWriter* writer=NULL;

  if (strcmp(PXTkApp::getFilenameExtension(line),".jpg")==0 ||
      (strcmp(PXTkApp::getFilenameExtension(line),".jpeg")==0) )
    {
      writer=vtkJPEGWriter::New();
      int d[3];
      for (int ia=0;ia<=2;ia++)
	{
	  double a=log(float(dim[ia]))/log(float(2.0));
	  int   t=int(a+0.999);
	  d[ia]=(int)pow(float(2.0),float(t));
	}
      pad->SetOutputWholeExtent(0,d[0]-1,0,d[1]-1,0,0);
    }
  else
    {
      writer=vtkTIFFWriter::New();
      pad->SetOutputWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
    }

  pad->SetOutputNumberOfScalarComponents(renSrc->GetOutput()->GetNumberOfScalarComponents());
  pad->SetConstant(0.0);
  writer->SetInput(pad->GetOutput());

  writer->SetFileName(line);
  writer->Write();
  writer->Delete();
  renSrc->Delete();
  pad->Delete();


  return 1;
}
/* -------------------------------------------------------------------------*/
//
//  GUI Callback Router
//
/* -------------------------------------------------------------------------*/
int vtkpxGUIRenderer::HandleEvent(int event)
{
  if (event<10)
      return this->EventManager->handleDirectEvent(event);
  

  switch(event)
      {
      case 101:
	SetCameraScale(GetCameraScale()*1.25);
	Zoom(camera_scale,PXTrue);
	if (this->EventParent!=NULL && this->CallbackNo>0)
	    this->EventParent->HandleEvent(CallbackNo);
	break;
	  
      case 102:
	SetCameraScale(GetCameraScale()*0.8);
	Zoom(camera_scale,PXTrue);
	if (this->EventParent!=NULL && this->CallbackNo>0)
	    this->EventParent->HandleEvent(CallbackNo);
	break;

      case 103:
	Reset();
	if (this->EventParent!=NULL && this->CallbackNo>0)
	    this->EventParent->HandleEvent(CallbackNo);
	break;

      case 104:
	if (mode_menu->getIndex()==1)
	  flip_mode=0;
	else
	  flip_mode=1;

	Reset();
	if (this->EventParent!=NULL && this->CallbackNo>0)
	    this->EventParent->HandleEvent(CallbackNo);
	break;

      case 105:
	if (flip_mode)
	    SetFlipMode(0);
	else
	    SetFlipMode(1);
	Reset();
	if (this->EventParent!=NULL && this->CallbackNo>0)
	    this->EventParent->HandleEvent(CallbackNo);
	break;
	
      case 106:
	Rotate(-1000.0,1,PXTrue);  
	break;

      case 107:
	Rotate(1000.0,1,PXTrue); 
	break;

      case 116:
	Rotate(-1000.0,0,PXTrue);  
	break;

      case 117:
	Rotate(1000.0,0,PXTrue);
	break;

      case 114:
	{
	  char *line=this->EventManager->getSaveFilenameComplex(pxtk_buffer,"screen.jpg",
								"{\"JPEG Files\" {\"*.jpeg\"}} {\"TIFF File\" {\".tif\"}}");

	  if (line==NULL)
	      return 0;
	  if (strlen(line)<1)
	      return 0;

	  SaveAsTIFF(line,PXFalse);
	}
	break;

      case 109:
	SetPlaneMode(plane_menu->getIndex());
	if (plane_menu->getIndex()<6)
	  {
	    Reset();
	  }
	else
	  return TCL_OK;

	break;

      case 110:
	if (this->CameraPresets->InsertCamera(this->renderer->GetActiveCamera())>0)
	  {
	    char line[30];
	    sprintf(line,"Custom %d",this->CameraPresets->GetNumberOfCameras());
	    plane_menu->addOption(line,109);
	    plane_menu->selectItem(this->CameraPresets->GetNumberOfCameras()+5);
	    update_plane->enable(PXTrue);
	  }
	break;

      case 111:
	if (plane_menu->getIndex()>5)
	  this->CameraPresets->CopyCamera(this->renderer->GetActiveCamera(),plane_menu->getIndex()-6);
	break;

      case 112: // Load
	{
	  char* line=this->EventManager->getOpenFilename("Load Camera Presets","Cameras","*.cm");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		{
		  int old=this->CameraPresets->GetNumberOfCameras();
		  int ok=this->CameraPresets->Load(line);
		  if (ok)
		    {
		      for (int i=old-1;i>=0;i=i-1)
			this->plane_menu->clearItem(i);
		      int np=this->CameraPresets->GetNumberOfCameras();
		      if (np>0)
			{
			  char line2[30];
			  for (int i=0;i<np;i++)
			    {
			      sprintf(line2,"Custom %d",i+1);
			      plane_menu->addOption(line2,109);
			      plane_menu->setIndex(6,PXTrue);
			    }
			  update_plane->enable(PXTrue);
			}
		      else
			{
			  plane_menu->setIndex(0,PXTrue);
			  update_plane->enable(PXFalse);
			}
		    }
		}
	    }
	}
      	break;

      case 113: // Save
	{
	  char* line=this->EventManager->getSaveFilename("Save Camera Presets","cameras.cm","Cameras","*.cm");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		this->CameraPresets->Save(line);
	    }
	}
      	break;





      case 120:
      case 121:
	// Clip Stuff 
	{
	  SetClipFront(clip_min->getValue());
	  clipping_thickness=clip_thickness->getValue();
	  vtkCamera* camera=renderer->GetActiveCamera();
	  camera->SetClippingRange(clipping_front,clipping_front+clipping_thickness);
	  this->CameraModifiedCallback();
	  RenderDelay();
	}
	break;

      case 122:
	{
	  this->ResetCamera();
	  double r[2]; renderer->GetActiveCamera()->GetClippingRange(r);
	  clip_min->setRange(r[0],r[1]);
	  clip_min->setValue(r[0]);
	  clip_thickness->setRange(0.01,r[1]-r[0]);
	  clip_thickness->setValue(r[1]-r[0]);
	  this->CameraModifiedCallback();
	  RenderDelay();
	}
	break;
	
	
	// Forced Clip
      case 123:
	{
	  //	  PXBool a=this->clip_mode;
	  if (plane_menu!=NULL)
	    SetPlaneMode(plane_menu->getIndex());
	  this->MoveAbove(-1);
	  this->Clip(-1);
	}
	break;

      case 124:
	this->SetPolygonalClipping(-1);
	this->Render();
	break;

      case 125:
	this->UpdateClippingPlanes();
	this->Render();
	break;

	case 130:
		main_light->SetAmbientColor(ambient[0]->getValue(),ambient[1]->getValue(),ambient[2]->getValue());
		main_light->SetDiffuseColor(diffuse[0]->getValue(),diffuse[1]->getValue(),diffuse[2]->getValue());
		main_light->SetSpecularColor(specular[0]->getValue(),specular[1]->getValue(),specular[2]->getValue());
		this->Render();
		break;

	case 132:
		main_light->SetIntensity(light_intensity->getValue());
		this->Render();
		break;


    case 133:
		if (camera_menu->getIndex()==1)
	  		camera_mode=0;
		else
	  		camera_mode=1;
	  	renderer->GetActiveCamera()->SetParallelProjection(this->camera_mode);
	  	this->CameraModifiedCallback();
      	this->RenderDelay();
	  	//this->Render();
	  	break;


	  	// orientation marker
	 case 140:
	 	markerWidget->SetEnabled(this->enablemarker->getState());
	 	this->RenderDelay();
	 	break;

	 case 141:
	 	markerWidget->SetAxisLabels(this->enablelabels->getState());
	 	this->RenderDelay();
	 	break;

	 case 142: 
	 	markerWidget->SetXAxisLabelText(this->labelx->getText());
	 	this->RenderDelay();
	 	break;

	 case 143:
	 	{
	 	  double c[3];
	 	  markerWidget->GetXAxisColor(c);
          int a=this->EventManager->d_colorBox("X Axis Color",c[0],c[1],c[2]);
          if (a==1)
            {
               markerWidget->SetXAxisColor(c);
               this->RenderDelay();
            }
		}
	 	break;

	 case 144:
	 	markerWidget->SetYAxisLabelText(this->labely->getText()); 
	 	this->RenderDelay();
	 	break;

	 case 145:
	  	{
	 	  double c[3];
	 	  markerWidget->GetYAxisColor(c);
          int a=this->EventManager->d_colorBox("Y Axis Color",c[0],c[1],c[2]);
          if (a==1)
            {
               markerWidget->SetYAxisColor(c);
               this->RenderDelay();
            }
		}
	 	break;

	 case 146: 
	 	markerWidget->SetZAxisLabelText(this->labelz->getText());
	 	this->RenderDelay();
	 	break;

	 case 147:
	  	{
	 	  double c[3];
	 	  markerWidget->GetZAxisColor(c);
          int a=this->EventManager->d_colorBox("Z Axis Color",c[0],c[1],c[2]);
          if (a==1)
            {
               markerWidget->SetZAxisColor(c);
               this->RenderDelay();
            }
		}
	 	break;

	 case 148:
	  	{
	 	  double c[3];
	 	  markerWidget->GetSphereColor(c);
          int a=this->EventManager->d_colorBox("Sphere Color",c[0],c[1],c[2]);
          if (a==1)
            {
               markerWidget->SetSphereColor(c);
               this->RenderDelay();
            }
		}
	 	break;

      }
  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
//
//  More Complex Stuff 
//
/* -------------------------------------------------------------------------*/
double vtkpxGUIRenderer::GetCameraWidth()
{
  double mmwidth=1.0;

  if (renderer==NULL)
      return mmwidth;
  
  double bd[6];
  renderer->ComputeVisiblePropBounds(bd);

  int w_plane_mode=plane_mode;
  if (w_plane_mode==-1)
      {
	this->ResetCamera();
	w_plane_mode=last_axis;
	if (w_plane_mode==-1)
	    w_plane_mode=2;
      }

  if (this->orientation!=3 || this->CurrentImage==NULL)
    {
      switch (w_plane_mode)
	{
	case 2:
	case 5:
	  mmwidth=Fmax(bd[3]-bd[2],bd[1]-bd[0]);
	  break;
	case 1:
	case 4:
	  mmwidth=Fmax(bd[1]-bd[0],bd[5]-bd[4]);
	  break;
	case 0:
	case 3:
	  mmwidth=Fmax(bd[3]-bd[2],bd[5]-bd[4]);
	  break;
	}
    }
  else
    {
      int   dim[3]; this->CurrentImage->GetDimensions(dim);
      double  sp[3]; this->CurrentImage->GetSpacing(sp);
      double ori[3]; this->CurrentImage->GetOrigin(ori);
      double w=double(dim[0])*sp[0];
      double h=double(dim[2])*sp[2];

      if (w_plane_mode == 0 || w_plane_mode==3)
	mmwidth=w;
      else
	mmwidth=Fmax(h,w);
    }

  if (mmwidth<1.0/auto_zoom)
    return 1.0;

  return mmwidth*auto_zoom;
}
/* -------------------------------------------------------------------------*/
//                     Sagittal/Axial/Coronal -- original image orientations
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::moveAboveSagittal(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis)
{
  // If image-orientation is sagittal then
  // axis = 2 XY --> Sagittal
  // axis = 1 XZ --> Coronal
  // axis = 0 YZ --> Axial
  // suborientation is now a number between 0 and 7


  if (this->orientation>=40)
    {
      this->moveAboveTransposedSagittal(midx,midy,midz,distance,camera,flip,axis);
      return;
    }




  int suborientation=vtkpxGUIBaseImageViewer::MapOrientationToNewStyle(this->orientation)-30;

  int wrongx=(suborientation & 1);
  int wrongy=(suborientation & 2);
  int wrongz=(suborientation & 4);


  //  fprintf(stderr,"sag orientation=%d suborientation=%d wrongx=%d, wrongy=%d, wrongz=%d flip=%d axis=%d\n",this->orientation,suborientation,
  //	  wrongx,wrongy,wrongz,flip,axis);

  
  // First pass to handle flips ....
  if (wrongz>0 && (axis==0 || axis==1 || axis== 3 || axis == 4))
    {
      axis=axis+3;
      if (axis>5)
	axis=axis-6;
    }

  if (wrongx>0) 
    {
      if (axis==2 || axis==5)
	{
	  axis=axis+3;
	  if (axis>5)
	    axis=axis-6;
	}
      else if (axis==1 || axis==4)
	{
	  flip=1-(flip>0);
	}
    }

  if (wrongy>0 && (axis==0 || axis==2 || axis==3 || axis==5))
    {
      flip=1-(flip>0);
    }

  //  fprintf(stderr,"\t\t flip=%d axis=%d\n\n\n\n",flip,axis);
  //  fflush(stderr);


  camera->SetFocalPoint(midx,midy,midz);
  switch(axis)
    {
    case 5: // Axial
      if (!flip)
	{
	  camera->SetPosition(midx,midy,midz-distance);
	  camera->SetViewUp(0,1,0);
	}
      else
	{
	  camera->SetPosition(midx,midy,midz+distance);
	  camera->SetViewUp(0,-1,0);
	}
      break;

    case 4: // Coronal
      if (!flip)
	{
	  camera->SetPosition(midx,midy-distance,midz);
	  camera->SetViewUp(-1,0,0);
	}
      else
	{
	  camera->SetPosition(midx,midy+distance,midz);
	  camera->SetViewUp(1,0,0);
	}
      break;
      
    case 3: // Sagital

      if (!flip)
	{
	  camera->SetPosition(midx-distance,midy,midz);
	  camera->SetViewUp(0,1,0);
	}
      else
	{
	  camera->SetPosition(midx+distance,midy,midz);
	  camera->SetViewUp(0,-1,0);
	}
      break;

    case 2: // Axial
      if (!flip)
	{
	  camera->SetPosition(midx,midy,midz+distance);
	  camera->SetViewUp(0,1,0);
	}
      else
	{
	  camera->SetPosition(midx,midy,midz-distance);
	  camera->SetViewUp(0,-1,0);
	}
      break;

    case 1:
      if (!flip)
	{
	  camera->SetPosition(midx,midy+distance,midz);
	  camera->SetViewUp(-1,0,0);
	}
      else
	{
	  camera->SetPosition(midx,midy-distance,midz);
	  camera->SetViewUp(1,0,0);
	}
      break;
      
    case 0:
      if (!flip)
	{
	  camera->SetPosition(midx+distance,midy,midz);
	  camera->SetViewUp(0,1,0);
	}
      else
	{
	  camera->SetPosition(midx-distance,midy,midz);
	  camera->SetViewUp(0,-1,0);
	}

      break;
    }
  camera->ComputeViewPlaneNormal();
}

void vtkpxGUIRenderer::moveAboveCoronal(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis)
{
  // If image-orientation is axial then
  // axis = 0 YZ --> Sagittal
  // axis = 1 XZ --> Axial
  // axis = 2 XY --> Coronal

  if (this->orientation>=40)
    {
      this->moveAboveTransposedCoronal(midx,midy,midz,distance,camera,flip,axis);
      return;
    }


  // suborientation is now a number between 0 and 7
  int suborientation=vtkpxGUIBaseImageViewer::MapOrientationToNewStyle(this->orientation)-20;

  int wrongx=(suborientation & 1);
  int wrongy=(suborientation & 2);
  int wrongz=(suborientation & 4);

  //  fprintf(stderr,"coronal orientation=%d suborientation=%d wrongx=%d, wrongy=%d, wrongz=%d flip=%d axis=%d\n",this->orientation,suborientation,
  //	  wrongx,wrongy,wrongz,flip,axis);

  
  // First pass to handle flips ....
  // Affects Axial + Coronal
  if (wrongx>0 && (axis==5 || axis==2 || axis== 4 || axis == 1))
    {
      //      fprintf(stderr,"doing x ...");
      axis=axis+3;
      if (axis>5)
	axis=axis-6;
    }

  // Affects Axial + Sagital
  if (wrongz>0) 
    {
      if (axis==1 || axis==4 )
	{
	  flip=1-(flip>0);
	}
      else if (axis==0 || axis==3)
	{
	  axis+=3;
	  if (axis>5)
	    axis=axis-6;
	}
    }

  if (wrongy>0 && (axis==0 || axis==2 || axis==3 || axis==5))
    {
      //      fprintf(stderr,"doing y ...");
      flip=1-(flip>0);
    }

  //  fprintf(stderr,"\t\t flip=%d axis=%d\n\n\n\n",flip,axis);
  //  fflush(stderr);


  camera->SetFocalPoint(midx,midy,midz);

  switch (axis)
    {
    case 5: // Coronal Reverse
      if (!flip)
	{
	  camera->SetPosition(midx,midy,midz+distance);
	  camera->SetViewUp(0,-1,0);
	}
      else
	{
	  camera->SetPosition(midx,midy,midz-distance);
	  camera->SetViewUp(0,1,0);
	}
      break;

    case 4: // Axial Reverse
      
      if (flip)
	{
	  camera->SetPosition(midx,midy+distance,midz);
	  camera->SetViewUp(0,0,+1);
	}
      else
	{
	  camera->SetPosition(midx,midy-distance,midz);
	  camera->SetViewUp(0,0,-1);
	}

      break;
      
    case 3: // Sagittal Reverse
      
      if (!flip)
	{
	  camera->SetPosition(midx-distance,midy,midz);
	  camera->SetViewUp(0,-1,0);
	}
      else
	{
	  camera->SetPosition(midx+distance,midy,midz);
	  camera->SetViewUp(0,1,0);
	}

      break;

    case 2: // Coronal
      if (!flip)
	{
	  camera->SetPosition(midx,midy,midz-distance);
	  camera->SetViewUp(0,-1,0);
	}
      else
	{
	  camera->SetPosition(midx,midy,midz+distance);
	  camera->SetViewUp(0,1,0);
	}
      break;

    case 1: // Axial
      if (!flip)
	{
	  camera->SetPosition(midx,midy+distance,midz);
	  camera->SetViewUp(0,0,-1);
	}
      else
	{
	  camera->SetPosition(midx,midy-distance,midz);
	  camera->SetViewUp(0,0,+1);
	}
      break;
      
    case 0: // Sagittal
      if (!flip)
	{
	  camera->SetPosition(midx+distance,midy,midz);
	  camera->SetViewUp(0,-1,0);
	}
      else
	{
	  camera->SetPosition(midx-distance,midy,midz);
	  camera->SetViewUp(0,+1,0);
	}
      break;
    }
  camera->ComputeViewPlaneNormal();
}

void vtkpxGUIRenderer::moveAboveAxial(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis)
{

  if (this->orientation>=40)
    {
      this->moveAboveTransposedAxial(midx,midy,midz,distance,camera,flip,axis);
      return;
    }

  // If image-orientation is axial then
  // axis = 0 YZ --> Sagittal
  // axis = 1 XZ --> Coronal
  // axis = 2 XY --> Axial

  // suborientation is now a number between 0 and 7
  int suborientation=vtkpxGUIBaseImageViewer::MapOrientationToNewStyle(this->orientation)-10;

  int wrongx=(suborientation & 1);
  int wrongy=(suborientation & 2);
  int wrongz=(suborientation & 4);

  //  fprintf(stderr,"axial orientation=%d suborientation=%d wrongx=%d, wrongy=%d, wrongz=%d\n",this->orientation,suborientation,
  //	  wrongx,wrongy,wrongz);
  
  // First pass to handle flips ....
  // 
  if (wrongx>0 && (axis==5 || axis==2 || axis== 4 || axis == 1))
    {
      axis=axis+3;
      if (axis>5)
	axis=axis-6;
    }

  if (wrongy>0)
    {
      if (axis==2 || axis==5)
	{
	  flip=1-(flip>0);
	}
      else if (axis==0 || axis==3)
	{
	  axis=axis+3;
	  if (axis>5) 
	    axis=axis-6;
	}
    }
  if (wrongz>0 && (axis==0 || axis==1 || axis==3 || axis==4))
    {
      flip=1-(flip>0);
    }

  camera->SetFocalPoint(midx,midy,midz);

  switch (axis)
    {

    case 5: // Axial -- flip LR
      if (!flip)
	{
	  camera->SetPosition(midx,midy,midz+distance);
	  camera->SetViewUp(0,-1,0);
	}
      else
	{
	  camera->SetPosition(midx,midy,midz-distance);
	  camera->SetViewUp(0,1,0);
	}
      break;

    case 4: // Coronal -- flip LR
      if (!flip)
	{
	  camera->SetPosition(midx,midy+distance,midz);
	  camera->SetViewUp(0,0,1);
	}
      else
	{
	  camera->SetPosition(midx,midy-distance,midz);
	  camera->SetViewUp(0,0,-1);
	}
      break;

    case 3: // Sagital -- flip AP
      if (!flip)
	{
	  camera->SetPosition(midx-distance,midy,midz);
	  camera->SetViewUp(0,0,1);
	}
      else
	{
	  camera->SetPosition(midx+distance,midy,midz);
	  camera->SetViewUp(0,0,-1);
	}

      break;

    case 2: // Axial 
      if (!flip)
	{
	  camera->SetPosition(midx,midy,midz-distance);
	  camera->SetViewUp(0,-1,0);
	}
      else
	{
	  camera->SetPosition(midx,midy,midz+distance);
	  camera->SetViewUp(0,1,0);
	}

      break;

    case 1: // Coronal
      if (!flip)
	{
	  camera->SetPosition(midx,midy-distance,midz);
	  camera->SetViewUp(0,0,1);
	}
      else
	{
	  camera->SetPosition(midx,midy+distance,midz);
	  camera->SetViewUp(0,0,-1);
	}
      break;
      
    case 0: // Sagital
      if (!flip)
	{
	  camera->SetPosition(midx+distance,midy,midz);
	  camera->SetViewUp(0,0,1);
	}
      else
	{
	  camera->SetPosition(midx-distance,midy,midz);
	  camera->SetViewUp(0,0,-1);
	}
      break;

    }

  camera->ComputeViewPlaneNormal();

}

/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::moveAbovePolar(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis)
{

  if (this->CurrentImage==NULL)
    {
      //      fprintf(stderr,"No Image Set\n");
      return;
    }

  int   dim[3]; this->CurrentImage->GetDimensions(dim);
  double  sp[3]; this->CurrentImage->GetSpacing(sp);
  double ori[3]; this->CurrentImage->GetOrigin(ori);

  double ox=ori[0]+sp[0]*double(dim[0]-1)*0.5;
  int slice_no=this->ViewerCoordinates[1];
  if (axis == 2)
    slice_no=this->ViewerCoordinates[2];
  double theta=double(slice_no)/double(dim[2])*M_PI;  
  double z=sp[0]*double(this->ViewerCoordinates[0])+ori[1];

  
  switch(axis)
    {
    case 0: // Short-Axis
      camera->SetFocalPoint(ox,ox,z);
      camera->SetPosition(ox,ox,z+distance);
      camera->SetViewUp(0,1,0);
      camera->ComputeViewPlaneNormal();
      break;

    case 3: // Reverse Short-Axis
      camera->SetFocalPoint(ox,ox,z);
      camera->SetPosition(ox,ox,z-distance);
      camera->SetViewUp(0,1,0);
      camera->ComputeViewPlaneNormal();
      break;

    case 2:
    case 1: // Radial 1 & 2
      camera->SetFocalPoint(ox,ox,midz);
      camera->SetPosition(ox-distance*sin(theta),ox+distance*cos(theta),midz);
      camera->SetViewUp(0,0,1);
      camera->ComputeViewPlaneNormal();
      break;

    case 4: // Reverse Radial 1 & 2 
    case 5:
      camera->SetFocalPoint(ox,ox,midz);
      camera->SetPosition(ox+distance*sin(theta),ox-distance*cos(theta),midz);
      camera->SetViewUp(0,0,1);
      camera->ComputeViewPlaneNormal();
    }


}


/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::MoveAbove(int axis)
{
  if (renderer==NULL)
      return;

  if (axis==-1)
      {
	if (plane_mode!=-1)
	  axis=plane_mode;
	else
	  return;
      }
 

  
  double midx,midy,midz;

  if (this->CurrentImage!=NULL)
    {
      int   dim[3]; this->CurrentImage->GetDimensions(dim);
      double  sp[3]; this->CurrentImage->GetSpacing(sp);
      double ori[3]; this->CurrentImage->GetOrigin(ori);

      midx=double((dim[0]-1)*sp[0])*0.5+ori[0];
      midy=double((dim[1]-1)*sp[1])*0.5+ori[1];
      midz=double((dim[2]-1)*sp[2])*0.5+ori[2];
    }
  else
    {
      renderer->ComputeVisiblePropBounds(bounds);
      midx=0.5*(bounds[0]+bounds[1]);
      midy=0.5*(bounds[2]+bounds[3]);
      midz=0.5*(bounds[4]+bounds[5]);
    }
      

  vtkCamera*  camera=renderer->GetActiveCamera();
  double width=this->GetCameraWidth();
  double distance=width/tan(camera->GetViewAngle()*vtkMath::Pi()/360.0);

  //  fprintf(stderr,"Move Above this=%d axis=%d orientation=%d, flip_mode=%d\n",this,axis,orientation,flip_mode);

  int oldstyleorient=vtkpxGUIBaseImageViewer::MapOrientationToOldStyle(this->orientation);
  //  fprintf(stderr,"orientation=%d oldstyle=%d\n",this->orientation,oldstyleorient);
  switch (oldstyleorient)
    {
    case 3:
      moveAbovePolar(midx,midy,midz,distance,camera,flip_mode,axis);
      break;
      
    case 2:
      moveAboveSagittal(midx,midy,midz,distance,camera,flip_mode,axis);
      break;
    case 1:
      moveAboveCoronal(midx,midy,midz,distance,camera,flip_mode,axis);
      break;
    default:
      moveAboveAxial(midx,midy,midz,distance,camera,flip_mode,axis);
      break;
    }

  this->ResetCameraClippingRange();
  //  fprintf(stderr,"Camera _scale =%f\n",camera_scale);

  Zoom(camera_scale,PXFalse);
  
  if (clip_mode==PXFalse)
    {
      this->CameraModifiedCallback();
      RenderDelay();
      return;
    }
  else
    {
      main_light->SetPosition(camera->GetPosition());
      main_light->SetFocalPoint(camera->GetFocalPoint());
      Clip(axis);
    }

}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::Clip(int axis)
{
  if (renderer==NULL)
      return;

  if (axis==-1)
    {
      if (plane_mode!=-1)
	axis=plane_mode;
      else
	return;
    }

  vtkCamera*  camera=renderer->GetActiveCamera();
  double p[3]; camera->GetPosition(p);
  double front=this->clipping_front;
  double width=this->GetCameraWidth();
  double distance=width/tan(camera->GetViewAngle()*vtkMath::Pi()/360.0);
	  
  if (this->orientation!=3)
    {
      if (this->clipping_front==-1.0)
	front=fabs(p[axis]-clipBounds[axis])-0.5*clipping_thickness;
      camera->SetClippingRange(front,front+clipping_thickness);
    }
  else
    {
      if (this->clipping_front==-1.0)
	front=distance-0.5*clipping_thickness;
      camera->SetClippingRange(front,front+clipping_thickness);
    }

  if (hasclipcontrols)
    {
      clip_min->setValue(front);
      clip_thickness->setValue(clipping_thickness);
    }

  this->CameraModifiedCallback();

}
/* -------------------------------------------------------------------------*/
// Reset Camera
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::ResetCamera()
{
  if (this->renderer==NULL)
    return;
  double bd[6];
  renderer->ComputeVisiblePropBounds(bd);
  this->ResetCamera(bd);
}

void vtkpxGUIRenderer::ResetCamera(double nbounds[6])
{
  if (this->renderer==NULL)
    return;
  
  double center[3];
  double distance;
  double width;
  double vn[3], *vup;
  
  vtkCamera* camera=renderer->GetActiveCamera();
  
  if ( camera == NULL )
    return;

  camera->GetViewPlaneNormal(vn);
  center[0] = (nbounds[0] + nbounds[1])/2.0;
  center[1] = (nbounds[2] + nbounds[3])/2.0;
  center[2] = (nbounds[4] + nbounds[5])/2.0;

  width = nbounds[3] - nbounds[2];
  if (width < (nbounds[1] - nbounds[0]))
    width = nbounds[1] - nbounds[0];
  
  // If we have just a single point, pick a width of 1.0
  width = (width==0)?(1.0):(width);
  
  distance = 0.8*width/tan(camera->GetViewAngle()*vtkMath::Pi()/360.0);
  distance = distance + (nbounds[5] - nbounds[4])/2.0;

  // check view-up vector against view plane normal
  vup = camera->GetViewUp();
  if ( fabs(vtkMath::Dot(vup,vn)) > 0.999 )
    {
      vtkWarningMacro(<<"Resetting view-up since view plane normal is parallel");
      camera->SetViewUp(-vup[2], vup[0], vup[1]);
    }

  // update the camera
  camera->SetFocalPoint(center[0],center[1],center[2]);
  camera->SetPosition(center[0]+distance*vn[0],
                                  center[1]+distance*vn[1],
                                  center[2]+distance*vn[2]);

  this->ResetCameraClippingRange( nbounds );

   // setup default parallel scale
  camera->SetParallelScale(width);
  camera->SetParallelProjection(camera_mode);

}

/* -------------------------------------------------------------------------*/
// Reset the camera clipping range to include this entire bounding box
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::ResetCameraClippingRange()
{
  if (this->renderer==NULL)
    return;
  double bd[6];
  renderer->ComputeVisiblePropBounds(bd);
  this->ResetCameraClippingRange(bd);
}

void vtkpxGUIRenderer::ResetCameraClippingRange( double nbounds[6] )
{
  if (this->renderer==NULL)
    return;
  
  vtkCamera* camera=renderer->GetActiveCamera();
  double  vn[3], position[3], a, b, c, d;
  double  range[2], dist;
  int     i, j, k;

  // Don't reset the clipping range when we don't have any 3D visible props
  if ( nbounds[0] == VTK_LARGE_FLOAT )
    return;
  
  if ( camera == NULL )
    return;
  
  // Find the plane equation for the camera view plane
  camera->GetViewPlaneNormal(vn);
  camera->GetPosition(position);
  a = -vn[0];
  b = -vn[1];
  c = -vn[2];
  d = -(a*position[0] + b*position[1] + c*position[2]);

  // Set the max near clipping plane and the min far clipping plane
  range[0] = a*nbounds[0] + b*nbounds[2] + c*nbounds[4] + d;
  range[1] = 1e-18;

  // Find the closest / farthest bounding box vertex
  for ( k = 0; k < 2; k++ )
    {
      for ( j = 0; j < 2; j++ )
	{
	  for ( i = 0; i < 2; i++ )
	    {
	      dist = a*nbounds[i] + b*nbounds[2+j] + c*nbounds[4+k] + d;
	      range[0] = (dist<range[0])?(dist):(range[0]);
	      range[1] = (dist>range[1])?(dist):(range[1]);
	    }
	}
    }
  
  // Do not let the range behind the camera throw off the calculation.
  if (range[0] < 0.0)
    range[0] = 0.0;
  
  // Give ourselves a little breathing room
  range[0] = 0.99*range[0] - (range[1] - range[0])*0.5;
  range[1] = 1.01*range[1] + (range[1] - range[0])*0.5;
  
  // Make sure near is not bigger than far
  range[0] = (range[0] >= range[1])?(0.01*range[1]):(range[0]);

  // Make sure near is at least some fraction of far - this prevents near
  // from being behind the camera or too close in front. How close is too
  // close depends on the resolution of the depth buffer
  int ZBufferDepth = 16;
  if (this->renderer->GetRenderWindow())
    ZBufferDepth = this->renderer->GetRenderWindow()->GetDepthBufferSize();

  if ( ZBufferDepth <= 16 )
    {
      range[0] = (range[0] < 0.01*range[1])?(0.01*range[1]):(range[0]);
    }
  else if ( ZBufferDepth <= 24 )
    {
      range[0] = (range[0] < 0.01*range[1])?(0.01*range[1]):(range[0]);
    }
  else
    {
      range[0] = (range[0] < 0.01*range[1])?(0.01*range[1]):(range[0]);
    }
  
  camera->SetClippingRange( range );


  if (hasclipcontrols)
    {
      double r[2];
      camera->GetClippingRange(r);
      double t=fabs(r[1]-r[0]);
      r[0]-=t;
      if (r[0]<10.0)
	r[0]=10.0;
      r[1]=r[0]+2.0*t;
      clip_min->setRange(r[0]-10.0,r[1]+100.0);
      clip_min->setResolution(1.0);
      clip_min->setValue(r[0]);
      clip_thickness->setRange(1.0,1.5*(r[1]-r[0]));
      if (clip_mode)
	clip_thickness->setValue(this->clipping_thickness);
      else
	clip_thickness->setValue(r[1]-r[0]);	
      clip_thickness->setResolution(1.0);
      this->clipping_front=-1.0;
    }


  this->ResetClipScales();

}


void vtkpxGUIRenderer::CameraModifiedCallback()
{
  if (this->renderer!=NULL)
    {
      double r[2]; renderer->GetActiveCamera()->GetClippingRange(r);

      if (this->clip_min!=NULL && this->clip_thickness!=NULL)
	{
	  
	  clip_min->setValue(r[0]);
	  //clip_min->setRange(r[0],r[1]);

	  //	  clip_thickness->setRange(0.01,r[1]-r[0]);
	  clip_thickness->setValue(r[1]-r[0]);
	}
      
      if (this->CameraEventParent!=NULL && this->CameraCallbackNo>0)
	{
	  this->CameraEventParent->HandleEvent(CameraCallbackNo);
	}
    }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRendererInterpolateValues(double* a,double* b,double w1,double w2,double* c,int n)
{
  for (int ia=0;ia<n;ia++)
    c[ia]=w1*a[ia]+w2*b[ia];
}

void vtkpxGUIRenderer::InterpolateCamera(vtkCamera* newCamera,vtkCamera* oldCamera,vtkCamera* oldCamera2,double weight)
{
  if (weight<0.0)
    weight=0.0;
  else if (weight>1.0)
    weight=1.0;
  
  double w1=weight;
  double w2=1.0-w1;

  double x[3],y[3],z[3];
  oldCamera->GetFocalPoint(x);   oldCamera2->GetFocalPoint(y);
  vtkpxGUIRendererInterpolateValues(&x[0],&y[0],w1,w2,&z[0],3);
  newCamera->SetFocalPoint(z);

  oldCamera->GetPosition(x);   oldCamera2->GetPosition(y);
  vtkpxGUIRendererInterpolateValues(&x[0],&y[0],w1,w2,&z[0],3);
  newCamera->SetPosition(z);

  oldCamera->GetViewUp(x);   oldCamera2->GetViewUp(y);
  vtkpxGUIRendererInterpolateValues(&x[0],&y[0],w1,w2,&z[0],3);

  // ***** 
  newCamera->SetViewUp(z);
  newCamera->ComputeViewPlaneNormal();

  double x2[2],y2[2],z2[2];
  oldCamera->GetClippingRange(x2);   oldCamera2->GetClippingRange(y2);
  vtkpxGUIRendererInterpolateValues(&x2[0],&y2[0],w1,w2,&z2[0],3);
  newCamera->SetClippingRange(z2);

  newCamera->SetParallelProjection(oldCamera->GetParallelProjection());

  
  newCamera->SetParallelScale(w1*oldCamera->GetParallelScale()+w2*oldCamera2->GetParallelScale());

}

void vtkpxGUIRenderer::CopyCamera(vtkCamera* newCamera,vtkCamera* oldCamera)
{
  newCamera->SetFocalPoint(oldCamera->GetFocalPoint());
  newCamera->SetPosition(oldCamera->GetPosition());
  newCamera->SetViewUp(oldCamera->GetViewUp());
  newCamera->ComputeViewPlaneNormal();
  newCamera->SetClippingRange(oldCamera->GetClippingRange());
  newCamera->SetParallelProjection(oldCamera->GetParallelProjection());
  newCamera->SetParallelScale(oldCamera->GetParallelScale());
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::RendererEvent()
{
  if (this->EnableActorTransform==0)
    return;


  vtkPropCollection* collection[3];
  collection[0]=this->renderer->GetActors();
  collection[1]=this->renderer->GetVolumes();
  collection[2]=this->renderer->GetViewProps();

  vtkLinearTransform* newt=this->ActorTransform;

  for (int c=0;c<=2;c++)
    {
      collection[c]->InitTraversal();
      for (int j=0;j<collection[c]->GetNumberOfItems();j++)
	{
	  vtkProp* prop0=collection[c]->GetNextProp();
	  if (prop0->IsA("vtkProp3D")==1)
	    {
	      vtkProp3D* prop=(vtkProp3D*)prop0;
	      vtkLinearTransform* current_t=prop->GetUserTransform();
	      if (newt!=current_t)
		{
		  prop->SetUserTransform(newt);
		  prop->Modified();
		  //		  fprintf(stderr,"Modifying c=%d j=%d %s\n",c,j,prop0->GetClassName());
		}
	    }
	}
    }
  this->renderer->SetTwoSidedLighting(1);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIRenderer::SetCurrentImageOrientation(vtkImageData* img,int new_orientation,int doupdate)
{
  if (new_orientation!=-1)
    this->SetOrientation(new_orientation,doupdate);
  
  this->SetCurrentImage(img);
  
  if (this->EnableActorTransform==0)
    return;
 

  // Temporary bypass option
  this->EnableActorTransform=0;
  return;

  // Fix this at some point
  /*  if (this->orientation!=2)
    {
      if (this->ActorTransform!=NULL)
	this->ActorTransform->Delete();
      this->ActorTransform=NULL;
      return;
    }
     
  if (this->ActorTransform==NULL)
    this->ActorTransform=vtkTransform::New();

  this->ActorTransform->Identity();
  this->ActorTransform->PostMultiply();
  this->ActorTransform->Scale(1.0,1.0,-1.0);

  if (this->GetCurrentImage()!=NULL)
    {
      vtkImageData* n_image=this->GetCurrentImage();
      double sp[3];  n_image->GetSpacing(sp);
      double ori[3]; n_image->GetOrigin(ori);
      int    dim[3]; n_image->GetDimensions(dim);
      double shift_z=sp[2]*double(dim[2]-1)+ori[2];

      this->ActorTransform->Translate(0.0,0.0,shift_z);
    }

  fprintf(stderr,"Marix = \n");
  vtkMatrix4x4* matrix=this->ActorTransform->GetMatrix();
	  
  for(int i=0;i<=3;i++)
    {
      for (int j=0;j<=3;j++)
	fprintf(stderr,"%7.3f ",matrix->GetElement(i,j));
      fprintf(stderr,"\n");
    }
  */


}

int  vtkpxGUIRenderer::GetClipScaleValue(int i)
{
  if (i<0 || i >5  || this->enableClip==NULL)
    return -1;

  return (int) clip_planes[i]->getValue();

}

void vtkpxGUIRenderer::SetClipScaleValue(int i,int v)
{
  if (i<0 || i >5)
    return;

  if (this->enableClip==NULL)
    return;

  if (v==-1)
    {
      if ( i==0 || i==2 || i ==4 )
	{
	  v=int(clip_planes[i]->getmin());
	} 
      else 
	{
	  v=int(clip_planes[i]->getmax());
	}
    }
  clip_planes[i]->setValue(float(v));
}

void vtkpxGUIRenderer::ResetClipScales()
{
  if (this->enableClip!=NULL)
    {
      for (int ia=0;ia<=2;ia++)
	{
	  int x1=ia*2;
	  int x2=ia*2+1;
	  
	  for (int ib=0;ib<=1;ib++)
	    {
	      int index=ia*2+ib;
	      clip_planes[index]->setRange(bounds[x1]-10.0,bounds[x2]+10.0);
	      if (ib==0)
		clip_planes[index]->setValue(clip_planes[index]->getmin());
	      else
		clip_planes[index]->setValue(clip_planes[index]->getmax());
	    }
	}
      this->UpdateClippingPlanes();
    }
}

void vtkpxGUIRenderer::SetPolygonalClipping(int mode)
{
  if (this->enableClip==NULL)
    return;

  if (mode==-1)
    {
      if (this->enableClip->getState()==PXTrue)
	mode=1;
      else
	mode=0;
    }
  mode=(mode>0);
  if (mode>0)
    this->enableClip->setState(PXTrue);
  else
    this->enableClip->setState(PXFalse);

  //  fprintf(stderr,"Set Polygonal Clipping mode=%d\n",mode);

  vtkPropCollection* propC=renderer->GetViewProps();
  if (propC==NULL)
    return;
  propC->InitTraversal();

  for (int j=0;j<propC->GetNumberOfItems();j++)
    {
      vtkProp* prop=propC->GetNextProp();
      if (prop!=NULL)
	{
	  if (prop->IsA("vtkActor"))
	    {
	      //	      fprintf(stderr,"Item %d type=%s\n",j,prop->GetClassName());
	      vtkMapper *mapper=((vtkActor*)prop)->GetMapper();
	      if (mapper!=NULL)
		{
		  mapper->RemoveAllClippingPlanes();
		  if (mode==1)
		    {
		      for (int ia=0;ia<=5;ia++)
			mapper->AddClippingPlane(this->ClippingPlanes[ia]);
		    }
		  prop->Modified();
		  mapper->Modified();
		}
	    }
	}
    }
}

void vtkpxGUIRenderer::UpdateClippingPlanes()
{
  if (this->enableClip==NULL)
    return;

  for (int ia=0;ia<=2;ia++)
    {
      for (int ib=0;ib<=1;ib++)
	{
	  int index=2*ia+ib;
	  this->ClippingPlanes[index]=vtkPlane::New();
	  double p[3]= { 0.0,0.0,0.0};
	  double n[3]= { 0.0,0.0,0.0};
	  if (ib==1)
	    n[ia]=-1.0;
	  else
	    n[ia]=1.0;

	  if (this->enableClip!=NULL)
	    p[ia]=clip_planes[index]->getValue();

	  //	  this->ClippingPlanes[index]->DebugOn();
	  this->ClippingPlanes[index]->SetOrigin(p);
	  this->ClippingPlanes[index]->SetNormal(n);
	  //	  this->ClippingPlanes[index]->DebugOff();
	}
    }

  if (this->enableClip->getState()!=PXTrue)
    return;

  this->SetPolygonalClipping(1);
}

/* ------------------------------------------------------------------------------------*/
//                     Sagittal/Axial/Coronal -- original image orientations tranposed
//
//
//      This implies that the x and y axis are tranposed
//
/* ------------------------------------------------------------------------------------*/

void vtkpxGUIRenderer::moveAboveTransposedAxial(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis)
{

  // If image-orientation is axial then
  // axis = 0 YZ --> Sagittal
  // axis = 1 XZ --> Coronal
  // axis = 2 XY --> Axial

  // suborientation is now a number between 0 and 7
  int suborientation=vtkpxGUIBaseImageViewer::MapOrientationToNewStyle(this->orientation)-40;

  int wrongx=(suborientation & 1);
  int wrongy=(suborientation & 2);
  int wrongz=(suborientation & 4);

  /*  wrongx=0;
  wrongy=0;
  wrongz=0;*/

  //  fprintf(stderr,"transp axial orientation=%d suborientation=%d axis=%d, flip=%d\n\t\twrongx=%d, wrongy=%d, wrongz=%d\n",this->orientation,suborientation,axis,flip,
  //  wrongx,wrongy,wrongz);
  
  // First pass to handle flips ....
  // 
  if (wrongx>0)
    {
      if (axis== 4 || axis == 1)
	{
	  axis=axis+3;
	  if (axis>5)
	    axis=axis-6;
	}
      if (axis==2 || axis==5)
	flip=1-(flip>0);
    }

  if (wrongy>0)
    {
      if (axis==2 || axis==5 || axis==0 || axis==3)
	{
	  axis=axis+3;
	  if (axis>5) 
	    axis=axis-6;
	}
    }
  if (wrongz>0 && (axis==0 || axis==1 || axis==3 || axis==4))
    {
      flip=1-(flip>0);
    }
  //  fprintf(stderr,"\t\t flip=%d axis=%d\n\n\n\n",flip,axis);
  //  fflush(stderr);

  camera->SetFocalPoint(midx,midy,midz);

  switch (axis)
    {

    case 5: // Axial -- flip LR --- DONE -----------
      if (!flip)
	{
	  camera->SetPosition(midx,midy,midz-distance);
	  camera->SetViewUp(-1,0,0);
	}
      else
	{
	  camera->SetPosition(midx,midy,midz+distance);
	  camera->SetViewUp(1,0,0);
	}
      break;

    case 4: // Sagittal -- flip LR -- DONE --------
      if (!flip)
	{
	  camera->SetPosition(midx,midy+distance,midz);
	  camera->SetViewUp(0,0,1);
	}
      else
	{
	  camera->SetPosition(midx,midy-distance,midz);
	  camera->SetViewUp(0,0,-1);
	}
      break;

    case 3: // Coronal -- flip AP  -- DONE --------
      if (!flip)
	{
	  camera->SetPosition(midx-distance,midy,midz);
	  camera->SetViewUp(0,0,1);
	}
      else
	{
	  camera->SetPosition(midx+distance,midy,midz);
	  camera->SetViewUp(0,0,-1);
	}

      break;

    case 2: // Axial  -- DONE -------------
      if (!flip)
	{
	  camera->SetPosition(midx,midy,midz+distance);
	  camera->SetViewUp(-1,0,0);
	}
      else
	{
	  camera->SetPosition(midx,midy,midz-distance);
	  camera->SetViewUp(1,0,0);
	}

      break;

    case 1: // Sagittal  -- DONE --------
      if (!flip)
	{
	  camera->SetPosition(midx,midy-distance,midz);
	  camera->SetViewUp(0,0,1);
	}
      else
	{
	  camera->SetPosition(midx,midy+distance,midz);
	  camera->SetViewUp(0,0,-1);
	}
      break;
      
    case 0: // Coronal  -- DONE --------
      if (!flip)
	{
	  camera->SetPosition(midx+distance,midy,midz);
	  camera->SetViewUp(0,0,1);
	}
      else
	{
	  camera->SetPosition(midx-distance,midy,midz);
	  camera->SetViewUp(0,0,-1);
	}
      break;

    }

  camera->ComputeViewPlaneNormal();

}

/* -------------------------------------------------------------------------*/

void vtkpxGUIRenderer::moveAboveTransposedCoronal(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis)
{
  // If image-orientation is axial then
  // axis = 0 YZ --> Sagittal
  // axis = 1 XZ --> Axial
  // axis = 2 XY --> Coronal

  // suborientation is now a number between 0 and 7
  int suborientation=vtkpxGUIBaseImageViewer::MapOrientationToNewStyle(this->orientation)-50;

  int wrongx=(suborientation & 1);
  int wrongy=(suborientation & 2);
  int wrongz=(suborientation & 4);

  //  fprintf(stderr,"transp coronal orientation=%d suborientation=%d wrongx=%d, wrongy=%d, wrongz=%d flip=%d axis=%d\n",this->orientation,suborientation,
  //	  wrongx,wrongy,wrongz,flip,axis);

  
  // First pass to handle flips ....
  // Affects Axial + Coronal
  if (wrongx>0 && (axis==5 || axis==2 || axis== 4 || axis == 1))
    {
      flip=1-(flip>0);
    }

  // Affects Axial + Sagital
  if (wrongz>0) 
    {
      if (axis==0 || axis==3 )
	{
	  flip=1-(flip>0);
	}
      else if (axis==1 || axis==4)
	{
	  axis+=3;
	  if (axis>5)
	    axis=axis-6;
	}
    }

  if (wrongy>0 && (axis==0 || axis==2 || axis==3 || axis==5))
    {
      axis+=3;
      if (axis>5)
	axis=axis-6;
    }

  //  fprintf(stderr,"\t\t flip=%d axis=%d\n\n\n\n",flip,axis);
  //  fflush(stderr);


  camera->SetFocalPoint(midx,midy,midz);

  switch (axis)
    {
    case 5: // Coronal Reverse -------------- DONE --------
      if (!flip)
	{
	  camera->SetPosition(midx,midy,midz-distance);
	  camera->SetViewUp(-1,0,0);
	}
      else
	{
	  camera->SetPosition(midx,midy,midz+distance);
	  camera->SetViewUp(1,0,0);
	}
      break;

    case 4: // Sagittal Reverse ------------ DONE ---------
      
      if (!flip)
	{
	  camera->SetPosition(midx,midy+distance,midz);
	  camera->SetViewUp(-1,0,0);
	}
      else
	{
	  camera->SetPosition(midx,midy-distance,midz);
	  camera->SetViewUp(1,0,0);
	}
      break;

      
    case 3: // Axial Reverse ------------- DONE ----------
       
      if (!flip)
	{
	  camera->SetPosition(midx+distance,midy,midz);
	  camera->SetViewUp(0,0,-1);
	}
      else
	{
	  camera->SetPosition(midx-distance,midy,midz);
	  camera->SetViewUp(0,0,1);
	}
      break;

    case 2: // Coronal ---------------- DONE -----------
      if (!flip)
	{
	  camera->SetPosition(midx,midy,midz+distance);
	  camera->SetViewUp(-1,0,0);
	}
      else
	{
	  camera->SetPosition(midx,midy,midz-distance);
	  camera->SetViewUp(1,0,0);
	}
      break;

    case 1: // Sagittal -------------- DONE -----------
      if (!flip)
	{
	  camera->SetPosition(midx,midy-distance,midz);
	  camera->SetViewUp(-1,0,0);
	}
      else
	{
	  camera->SetPosition(midx,midy+distance,midz);
	  camera->SetViewUp(1,0,0);
	}
      break;
      
    case 0: // Axial ------------- DONE ----------
      if (!flip)
	{
	  camera->SetPosition(midx-distance,midy,midz);
	  camera->SetViewUp(0,0,-1);
	}
      else
	{
	  camera->SetPosition(midx+distance,midy,midz);
	  camera->SetViewUp(0,0,1);
	}
      break;
    }
  camera->ComputeViewPlaneNormal();
}

// ----------------------------------------------------------------------------------------------------------------------------------------
void vtkpxGUIRenderer::moveAboveTransposedSagittal(double midx,double midy,double midz,double distance,vtkCamera* camera,int flip,int axis)
{
  // If image-orientation is sagittal then
  // axis = 2 XY --> Sagittal
  // axis = 1 XZ --> Coronal
  // axis = 0 YZ --> Axial
  // suborientation is now a number between 0 and 7
  int suborientation=vtkpxGUIBaseImageViewer::MapOrientationToNewStyle(this->orientation)-60;

  int wrongx=(suborientation & 1);
  int wrongy=(suborientation & 2);
  int wrongz=(suborientation & 4);


  //  fprintf(stderr,"trans sag orientation=%d suborientation=%d wrongx=%d, wrongy=%d, wrongz=%d flip=%d axis=%d\n",this->orientation,suborientation,
  //  	  wrongx,wrongy,wrongz,flip,axis);

  
  // First pass to handle flips ....
  if (wrongz>0 && (axis==0 || axis==1 || axis== 3 || axis == 4))
    {
      axis=axis+3;
      if (axis>5)
	axis=axis-6;
    }

  if (wrongx>0) 
    {
      if (axis==2 || axis==5)
	{
	  flip=1-(flip>0);
	}
      else if (axis==1 || axis==4)
	{
	  flip=1-(flip>0);
	}
    }

  if (wrongy>0 )
    {
      if ((axis==0 || axis==3))
	{
	  flip=1-(flip>0);
	}
      else if (axis==2 || axis==5)
	{
	  axis=axis+3;
	  if (axis>5)
	    axis=axis-6;
	}
    }
  
  //  fprintf(stderr,"\t\t flip=%d axis=%d\n\n\n\n",flip,axis);
  //  fflush(stderr);


  camera->SetFocalPoint(midx,midy,midz);
  switch(axis)
    {
    case 5: // Reverse Sagittal ---------- DONE -------
      if (!flip)
	{
	  camera->SetPosition(midx,midy,midz+distance);
	  camera->SetViewUp(1,0,0);
	}
      else
	{
	  camera->SetPosition(midx,midy,midz-distance);
	  camera->SetViewUp(-1,0,0);
	}
      break;


    case 4: // Reverse Coronal - ----- DONE -------
      if (!flip)
	{
	  camera->SetPosition(midx,midy+distance,midz);
	  camera->SetViewUp(1,0,0);
	}
      else
	{
	  camera->SetPosition(midx,midy-distance,midz);
	  camera->SetViewUp(-1,0,0);
	}
      break;
      
    case 3: // Reverse Axial -------- DONE ---------
      if (!flip)
	{
	  camera->SetPosition(midx+distance,midy,midz);
	  camera->SetViewUp(0,-1,0);
	}
      else
	{
	  camera->SetPosition(midx-distance,midy,midz);
	  camera->SetViewUp(0,1,0);
	}
      break;

    case 2: // Sagittal ----------- DONE ----------
      if (!flip)
	{
	  camera->SetPosition(midx,midy,midz-distance);
	  camera->SetViewUp(1,0,0);
	}
      else
	{
	  camera->SetPosition(midx,midy,midz+distance);
	  camera->SetViewUp(-1,0,0);
	}
      break;

    case 1: // Coronal -------------- DONE ----------
      if (!flip)
	{
	  camera->SetPosition(midx,midy-distance,midz);
	  camera->SetViewUp(1,0,0);
	}
      else
	{
	  camera->SetPosition(midx,midy+distance,midz);
	  camera->SetViewUp(-1,0,0);
	}
      break;
      
    case 0: // Axial -------------- DONE -----------
      if (!flip)
	{
	  camera->SetPosition(midx-distance,midy,midz);
	  camera->SetViewUp(0,-1,0);
	}
      else
	{
	  camera->SetPosition(midx+distance,midy,midz);
	  camera->SetViewUp(0,1,0);
	}

      break;
    }
  camera->ComputeViewPlaneNormal();
}
// ----------------------------------------------------------------------------------------------------------------------------------------

