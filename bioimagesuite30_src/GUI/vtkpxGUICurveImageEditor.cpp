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





/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */
#include "vtkpxAnalyzeImageSource.h"
#include "vtkpxGUICurveImageEditor.h"
#include "vtkpxGUIMosaicViewer.h"
#include "pxutil.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkAssembly.h"
#include "vtkRenderer.h"
#include "vtkObjectFactory.h"
#include "vtkpxImageExtract.h"
#include "vtkSphereSource.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkIdentityTransform.h"
#include "vtkMath.h"
#include "vtkPoints.h"
#include "vtkTransform.h"

// 	$Id: vtkpxGUICurveImageEditor.cpp,v 1.2 2004/03/01 18:20:58 xenios Exp xenios $	


vtkpxGUICurveImageEditor* vtkpxGUICurveImageEditor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUICurveImageEditor");
  if(ret)
      {
	return (vtkpxGUICurveImageEditor*)ret;
      }
  return new vtkpxGUICurveImageEditor;
}

// Construct object with no children.
vtkpxGUICurveImageEditor::vtkpxGUICurveImageEditor()
{
  this->ImageViewer=NULL;
  enable_curve=PXTrue;
  enable_labels=PXFalse;
  enable_list=PXFalse;
}

vtkpxGUICurveImageEditor::~vtkpxGUICurveImageEditor()
{
  if (this->ImageViewer!=NULL)
    this->ImageViewer->Delete();
  if (this->image!=NULL)
    this->image->Delete();
}
/* -------------------------------------------------------------------------*/
char* vtkpxGUICurveImageEditor::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
    return GetWidgetName();


  vtkpxGUIComponent::Initialize(name,inside);

  PXTkFrame* baseFrame=(PXTkFrame*)this->EventManager->getMainWindow();

  baseFrame->bindCallback("<Delete>",204);

  // Menu Stuff
  // ----------
  PXTkFrame* menubar=new PXTkFrame(this->EventManager,baseFrame);
  baseFrame->addChildren("-side top -expand false -fill x",menubar);


  PXTkMenuButton* but0=new PXTkMenuButton(this->EventManager,menubar,"Image",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but0);

  PXTkMenu *menu0=new PXTkMenu(this->EventManager,but0,PXFalse);
  menu0->addButton("Load",700);
  menu0->addSeparator();
  if (this->EventParent)
    menu0->addButton("Close",3);
  else
    menu0->addButton("Exit",1);


  PXTkMenuButton* but1=new PXTkMenuButton(this->EventManager,menubar,"Curve",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but1);
  
  PXTkMenu *menu1=new PXTkMenu(this->EventManager,but1,PXFalse);
  menu1->addButton("New",100);
  menu1->addSeparator();
  menu1->addButton("Load",101);
  menu1->addButton("Save",102);

  
  PXTkMenuButton* but2=new PXTkMenuButton(this->EventManager,menubar,"Edit",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but2);
  PXTkMenu *menu2=new PXTkMenu(this->EventManager,but2,PXFalse);
  menu2->addButton("Delete Last Point",204);
  menu2->addButton("Delete All Points",100);

  PXTkMenuButton* but24=new PXTkMenuButton(this->EventManager,menubar,"Color",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but24);
  PXTkMenu *menu24=new PXTkMenu(this->EventManager,but24,PXFalse);
  menu24->addButton("Landmark Color",400);
  menu24->addButton("Highlight Color",401);

  PXTkMenuButton* but6=new PXTkMenuButton(this->EventManager,menubar,"Curve",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but6);
  
  PXTkMenu *menu6=new PXTkMenu(this->EventManager,but6,PXFalse);
  menu6->addButton("Copy  Point Set",590);
  menu6->addButton("Paste Point Set",591);
  menu6->addButton("Paste Curve (2D)",592);
  menu6->addSeparator();
  menu6->addButton("Create Circle",596);
  menu6->addButton("Remove Loops",597);
  menu6->addButton("Smooth",602);
  
  PXTkMenuButton* but62=new PXTkMenuButton(this->EventManager,menubar,"Resample",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but62);
  PXTkMenu *menu62=new PXTkMenu(this->EventManager,but62,PXFalse);
  menu62->addButton("Equispace",604);
  menu62->addButton("Equispace 1.0",595);
  menu62->addButton("Equispace 2.0",598);
  menu62->addButton("Sample x2",600);
  menu62->addButton("Sample x8",599);
  menu62->addButton("Subsample x2",601);
  
    
  PXTkMenuButton* but7=new PXTkMenuButton(this->EventManager,menubar,"Operation",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but7);
  
  PXTkMenu *menu7=new PXTkMenu(this->EventManager,but7,PXFalse);
  menu7->addButton("Area",605);
  menu7->addButton("Image Stats",606);
  menu7->addButton("Threshold",612);
  menu7->addSeparator();
  menu7->addButton("Extract",609);
  menu7->addButton("Snake",610);



  // Other Stuff 
  // --------------
  // Collection Properties 

  PXTkFrame* viewerFrame=new PXTkFrame(this->EventManager,baseFrame);
  baseFrame->addChildren("-side left -expand true -fill both -padx 2 -pady 2",viewerFrame);

  PXTkFrame* checkFrame=new PXTkFrame(this->EventManager,baseFrame);
  baseFrame->addChildren("-side left -expand f -fill x -padx 2 -pady 2",checkFrame);

  this->ImageViewer=vtkpxGUIMosaicViewer::New();
  this->ImageViewer->SetSingleSliceMode();
  this->ImageViewer->SetOffset(1);
  this->ImageViewer->SetEditor(this);
  this->ImageViewer->SetMouseToEditor(1);
  this->ImageViewer->Initialize(viewerFrame->getWidgetName(),1);
  
  currentCollection=new PXTkOptionMenu(this->EventManager,checkFrame,"Point Set 1",500);
  for (int i=1;i<num_collections;i++)
    {
      sprintf(lamstring,"Point Set %d",i+1);
      currentCollection->addOption(lamstring);
    }
  checkFrame->addChildren("-side top -expand f -fill x  -pady 1",currentCollection);

  curveMode=new PXTkOptionMenu(this->EventManager,checkFrame,"Points",110);
  curveMode->addOption("Open   Curve");
  curveMode->addOption("Closed Curve");
  curveMode->setIndex(1);

  scaleMode=new PXTkOptionMenu(this->EventManager,checkFrame,"Size=0.25",111);
  for (float s=0.50;s<=2.50;s+=0.25)
      {
	sprintf(pxtk_buffer,"Size=%3.2f",s);
	scaleMode->addOption(pxtk_buffer);
      }
  int sc=int(landmarkCollection->GetPointScale()/0.25)-1;
  scaleMode->setIndex(sc);

  displayCollection=new PXTkCheckButton(this->EventManager,checkFrame,"Show",501);
  displayCollection->setState(showCollection[0]);
  
  message=new PXTkLabel(this->EventManager,checkFrame,"Num");
  message->configure("-relief","ridge");

  checkFrame->addChildren("-side top -expand f -fill x  -pady 1",curveMode,scaleMode,displayCollection,message);

  mouseMode=new PXTkOptionMenu(this->EventManager,checkFrame,"Disabled  ",502);
  mouseMode->addOption("Pick Mode");
  mouseMode->addOption("Auto Mode");
  mouseMode->addOption("Add  Mode");
  mouseMode->addOption("Cont-Add Mode");
  mouseMode->setIndex(0);
  checkFrame->addChildren("-side top -expand f -fill x  -pady 1",mouseMode);

  PXTkButton* updateButton=new PXTkButton(this->EventManager,checkFrame,"Update",701);
  checkFrame->addChildren("-side top -expand f -fill x -padx 2 -pady 2",updateButton);

  UpdateStatus();

  fprintf(stderr,"This->Initalized=%d\n",this->Initialized);
  return GetWidgetName();
}

/* -------------------------------------------------------------------------*/ 
void vtkpxGUICurveImageEditor::SetRenderer(vtkRenderer* ren,int num)
{
  if (this->ImageViewer==NULL)
    return;

  num=0;
  if (renderer[num]!=NULL || ren==NULL)
    return;

  ren=this->ImageViewer->GetRenderer();
  vtkpxGUIBaseCurveControl::SetRenderer(ren,0);
}
/* -------------------------------------------------------------------------*/
int  vtkpxGUICurveImageEditor::HandleEvent(int event)
{
  if (!this->Initialized)
      return TCL_OK;

  if (event<10)
      return this->EventManager->handleDirectEvent(event);

  if (event<700)
    return vtkpxGUIBaseCurveControl::HandleEvent(event);

  if (event==700)
    {
      char* line=this->EventManager->getOpenFilename("Image File Name","Analyze Images","*.hdr*");
      if (line!=NULL)
	{
	  if (strlen(line)>0)
	    {
	      strcpy(pxtk_buffer,line);
	      vtkpxAnalyzeImageSource* ana=vtkpxAnalyzeImageSource::New();
	      int ok=ana->Load(pxtk_buffer);
	      if (ok!=0)
		this->SetImage(ana->GetOutput(),ana->GetOrientation());
	      ana->Delete();
	    }
	}
    }
  
  if (event==701)
    {
      if (this->EventParent!=NULL && this->CallbackNo>0)
	this->EventParent->HandleEvent(CallbackNo);
    }
  
  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
void  vtkpxGUICurveImageEditor::SetSpacingOrigin(double sp[3],double ori[3])
  {
  if (this->image==NULL)
    return;

  this->image->GetSpacing(sp);
  this->image->GetOrigin(ori);
  vtkpxGUIBaseCurveControl::SetSpacingOrigin(sp,ori);
}

/* -------------------------------------------------------------------------*/


/*      for (int i=0;i<num_collections;i++)
	{
	  allCollections[i]->SetShowControls(1);
	  allCollections[i]->SetShowCurve(1);
	}
*/


/* -------------------------------------------------------------------------*/
void vtkpxGUICurveImageEditor::SetImage(vtkImageData* img,int plane,int level)
{
  if (img != this->ImageViewer->GetImage())
    {
      this->ImageViewer->SetImage(img,plane);
      this->ImageViewer->UpdateAllViewers();
    }

  if (this->renderer[0]==NULL)
    this->SetRenderer(this->ImageViewer->GetRenderer(),0);

  vtkpxGUIBaseCurveControl::SetImage(this->ImageViewer->GetImage(),2,level);

  //fprintf(stderr,"Image Set image=%d, render=%d\n",(int)this->image,(int)this->renderer[0]);

}
/* -------------------------------------------------------------------------*/
void vtkpxGUICurveImageEditor::SetPlaneLevel(int plane,int level)
{
  image_plane=2; // Force XY Plane for editing 
  image_level=level;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUICurveImageEditor::SetLookupTable(vtkLookupTable* lookuptable)
{
  currentColormap=lookuptable;
  if (this->ImageViewer!=NULL)
    this->ImageViewer->SetLookupTable(lookuptable);
  if (backupColormap==NULL)
    backupColormap=vtkLookupTable::New();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUICurveImageEditor::UpdateStatus()
{
  if (!this->Initialized)
    return;
  
  sprintf(pxtk_buffer,"NP=%d",
	  landmarkCollection->GetNumPoints());
  message->setText(pxtk_buffer);
}
/* -------------------------------------------------------------------------*/
vtkpxGUIMosaicViewer* vtkpxGUICurveImageEditor::GetImageViewer()
{
  if (this->ImageViewer==NULL)
    {
      fprintf(stderr,"Likely Error Here\n");
      return NULL;
    }
  else
    return this->ImageViewer;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUICurveImageEditor::UpdateAllViewers()
{
  if (!this->Initialized)
    return 0;

  if (showCollection[current_collection]==PXTrue)
    {
      highlightData->Update();
      landmarkCollection->Update();
    }
  

    this->ImageViewer->UpdateDisplay();
    return 1;
}

