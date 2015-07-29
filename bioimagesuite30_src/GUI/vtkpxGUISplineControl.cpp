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





// 	$Id: vtkpxGUISplineControl.cpp,v 1.2 2003/09/11 13:34:39 xenios Exp xenios $	

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkpxGUISplineControl.h"
#include "vtkpxUtil.h"
#include "vtkActor.h"
#include "vtkAssembly.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkObjectFactory.h"
#include "vtkpxImageExtract.h"
#include "vtkPolyDataMapper.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkImageData.h"
#include "vtkProperty.h"
#include "vtkPoints.h"
#include "pxfpbsplinestack.h"
/* -------------------------------------------------------------------------*/
vtkpxGUISplineControl* vtkpxGUISplineControl::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUISplineControl");
  if(ret)
      {
	return (vtkpxGUISplineControl*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUISplineControl;
}

// ----------------------------------------------------------------------------
vtkpxGUISplineControl::vtkpxGUISplineControl()
{
  this->NumberOfSplines=50;
  this->clipboardSpline=NULL;
  this->undoSpline=NULL;

  for (int ia=0;ia<=2;ia++)
    {
      this->origin[ia]=0.0;
      this->spacing[ia]=1.0;
      this->dimensions[ia]=100;
    }


  this->DefaultScaleMode=4;
  
  for (int i=0;i<this->NumberOfSplines;i++)
    {
      splineSet[i]=NULL;
      splineActor[i]=NULL;
      if (i<2)
	display_mode[i]=PXTrue;
      else
	display_mode[i]=PXFalse;
      spline_enabled[i]=1;
      for (int j=0;j<=2;j++)
	{
	  ghostSet[j][i]=NULL;
	  ghostActor[j][i]=NULL;
	}
      this->changed[i]=0;
      scale_mode[i]=this->DefaultScaleMode;
    }
  ghostMode=NULL;
  editCurve=NULL;
  manualEditCurve=NULL;
  extractGUI=NULL;

  editActor=NULL;
  manualEditActor=NULL;
  this->Renderer=NULL;
  current_spline=0;
  currentSpline=NULL;
  snakeGUI=NULL;
  image=NULL;
  image_level=0;
  image_frame=0;
  slice_offset=-0.02;
  ghost_displaymode=0;
  autoUpdate=NULL;
  AutoUpdateMode=1;
  
  this->SlaveMode=0;
  this->DistanceThreshold=1.0;
  this->ControlPointScaleFactor=1.0;
  this->manual_add_mode=0;
  this->manual_zcoord=0.0;
  this->DefaultSmoothing=1.0;

  char* line=PXTkApp::getTclVariable("pxtcl_pref_array(SplineSmoothing)");
  if (line!=NULL)
      this->DefaultSmoothing=Frange(atof(line),0.0001,10.0);

  this->ShowGhostControls=1;
}
// ----------------------------------------------------------------------------
vtkpxGUISplineControl::~vtkpxGUISplineControl()
{
  
  if (clipboardSpline!=NULL)
    clipboardSpline->Delete();
  
  if (undoSpline!=NULL)
    undoSpline->Delete();

  if (editCurve!=NULL)
    editCurve->Delete();
  if (editActor!=NULL)
    editActor->Delete();

  if (manualEditCurve!=NULL)
    manualEditCurve->Delete();
  if (manualEditActor!=NULL)
    manualEditActor->Delete();


  for (int i=0;i<this->NumberOfSplines;i++)
    {
      if (splineSet[i]!=NULL)
	splineSet[i]->Delete();
      if (splineActor[i]!=NULL)
	splineActor[i]->Delete();
      for (int j=0;j<2;j++)
	{
	  if (ghostActor[j][i]!=NULL)
	    ghostActor[j][i]->Delete();
	  if (ghostSet[j][i]!=NULL)
	    ghostSet[j][i]->Delete();
	}
    }
    
  if (this->image!=NULL)
    this->image->Delete();

}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::SetNumberOfSplines(int a)
{
  if (this->Initialized==1)
    return;
  
  this->NumberOfSplines=Irange(a,1,SP_MAX_COLLECTIONS);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::InitializeSplines()
{
  for (int i=0;i<this->NumberOfSplines;i++)
    {
      splineSet[i]=vtkpxSplineSource::New();
      splineSet[i]->SetResolution(0.01);
      splineSet[i]->SetEllipse(8,
			       spacing[0]*dimensions[0]/8.0*(1.0+i),
			       spacing[1]*dimensions[1]/8.0*(1.0+i),
			       origin[0]+spacing[0]*dimensions[0]/2.0,
			       origin[1]+spacing[1]*dimensions[1]/2.0,
			       0.0,2);
      for (int j=0;j<2;j++)
	{
	  splineSet[i]->SetResolution(0.03);
	  ghostSet[j][i]=vtkpxSplineSource::New();;
	  ghostSet[j][i]->Copy(splineSet[i]);
	}
      scale_mode[i]=this->DefaultScaleMode;
    }



  editCurve=vtkpxBaseCurve::New();
  editCurve->SetShowCurve(0);
  editCurve->SetShowControls(1);
  editCurve->SetControlsDisplayModeToDisc();
  editCurve->SetPointScale(1.0);
  editCurve->SetOrigin(this->origin);
  editCurve->SetSpacing(this->spacing);
  

  manualEditCurve=vtkpxBaseCurve::New();
  manualEditCurve->SetShowControls(1);
  manualEditCurve->SetShowCurve(1);
  manualEditCurve->SetControlsDisplayModeToDisc();
  manualEditCurve->SetPointScale(1.0);
  manualEditCurve->SetOrigin(this->origin);
  manualEditCurve->SetSpacing(this->spacing);
  

  splineSet[0]->ExportToBaseCurve(editCurve);
  splineSet[0]->ExportToBaseCurve(manualEditCurve);
  current_spline=0;
  currentSpline=splineSet[current_spline];

  clipboardSpline=vtkpxSplineSource::New();
  clipboardSpline->Copy(currentSpline);

  undoSpline=vtkpxSplineSource::New();
  undoSpline->Copy(currentSpline);

}
/* -------------------------------------------------------------------------*/
char* vtkpxGUISplineControl::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();

  InitializeSplines();

  vtkpxGUIComponent::Initialize(name,inside);
  PXTkFrame* mFrame=(PXTkFrame*)this->EventManager->getMainWindow();
  PXTkLabelFrame* landmarkFrame=new PXTkLabelFrame(this->EventManager,mFrame,"Spline Control");
  mFrame->addChildren("-side top -expand t -fill both -padx 4 -pady 4 ",landmarkFrame);

  // Control Properties
  // ---------------------
  PXTkLabelFrame* vFrame=new PXTkLabelFrame(this->EventManager,landmarkFrame,"Global Properties");
  landmarkFrame->addChildren("-side top -expand false -fill x -padx 5 -pady 5",vFrame);
  vFrame->setPackMode(PXFalse);
 
  mouseMode=new PXTkCheckButton(this->EventManager,vFrame,"Edit Mode",-1);
  mouseMode->setState(PXTrue);
  vFrame->gridRowColumnChild(mouseMode,0,0);


  editMode=new PXTkCheckButton(this->EventManager,vFrame,"Manual Mode",510);
  editMode->setState(PXFalse);
  vFrame->gridRowColumnChild(editMode,0,1);

  ExtractButton=new PXTkButton(this->EventManager,vFrame,"Extr",606);
  ExtractButton->enable(PXFalse);
  vFrame->gridRowColumnChild(ExtractButton,0,2);
  

  showControls=new PXTkCheckButton(this->EventManager,vFrame,"Controls",502);
  showControls->setState(PXTrue);
  vFrame->gridRowColumnChild(showControls,1,0,1,2);

  PXTkButton* b0=new PXTkButton(this->EventManager,vFrame,"(-)",504);
  b0->configure("-padx",1);
  vFrame->gridRowColumnChild(b0,1,2);

  if (this->ShowGhostControls)
    {
      PXTkLabel* lab1=new PXTkLabel(this->EventManager,vFrame,"Ghost Display:");
      ghostMode=new PXTkOptionMenu(this->EventManager,vFrame,"Hide All",507);
      ghostMode->addOption("Prev Only");
      ghostMode->addOption("Next Only");
      ghostMode->addOption("Show Both");
      ghostMode->setIndex(ghost_displaymode);
      
      vFrame->gridRowColumnChild(lab1,2,0);
      vFrame->gridRowColumnChild(ghostMode,2,1);
    }
  
  PXTkButton* b0a=new PXTkButton(this->EventManager,vFrame,"Show All",506);
  PXTkButton* b0b=new PXTkButton(this->EventManager,vFrame,"Hide All",505);
  PXTkLabel* lab=new PXTkLabel(this->EventManager,vFrame,"");
  lab->configure("-image","$::pxtclvtkpxcontrib::smalllogo ");
  vFrame->gridRowColumnChild(lab,4,0,1,2);
  vFrame->gridRowColumnChild(b0a,3,0);
  vFrame->gridRowColumnChild(b0b,3,1);
  
  // Collection Properties 
  // ---------------------
  PXTkLabelFrame* pFrame=new PXTkLabelFrame(this->EventManager,landmarkFrame,"Spline Properties");
  landmarkFrame->addChildren("-side top -expand true -fill both -padx 2 -pady 2",pFrame);
  pFrame->setPackMode(PXFalse);

  PXTkLabel* labs=new PXTkLabel(this->EventManager,pFrame,"Current Spline:");
  currentCollection=new PXTkOptionMenu(this->EventManager,pFrame,"Spline 1",500);
  for (int i=1;i<this->NumberOfSplines;i++)
    {
      sprintf(pxtk_buffer4,"Spline %d",i+1);
      currentCollection->addOption(pxtk_buffer4);
    }
  currentCollection->setIndex(current_spline);
  pFrame->gridRowColumnChild(labs,0,0,1,2);
  pFrame->gridRowColumnChild(currentCollection,0,2);


  
  message=new PXTkLabel(this->EventManager,pFrame,"Num");
  pFrame->gridRowColumnChild(message,1,0,1,3);
  message->configure("-relief","ridge");

  displayMode=new PXTkCheckButton(this->EventManager,pFrame,"Display",501);
  displayMode->setState(display_mode[current_spline]);
  pFrame->gridRowColumnChild(displayMode,2,0);

  PXTkLabel* lab0=new PXTkLabel(this->EventManager,pFrame,"Size:");
  pFrame->gridRowColumnChild(lab0,2,1);
  
  scaleMode=new PXTkOptionMenu(this->EventManager,pFrame,"0.25",503);
  for (float s=0.50;s<=4.5;s+=0.25)
      {
	sprintf(pxtk_buffer,"%3.2f",s);
	scaleMode->addOption(pxtk_buffer);
      }
  scaleMode->setIndex(scale_mode[current_spline]);
  pFrame->gridRowColumnChild(scaleMode,2,2);

  // Collection Operations 
  // ---------------------
  PXTkLabelFrame* oFrame=new PXTkLabelFrame(this->EventManager,landmarkFrame,"Operations on Current Spline");
  bottomButtonFrame=new PXTkFrame(this->EventManager,landmarkFrame);  
  landmarkFrame->addChildren("-side bottom -expand false -fill x -padx 00",bottomButtonFrame);

  landmarkFrame->addChildren("-side top -expand true -fill both -padx 2 -pady 1",oFrame);


  PXTkFrame* topfr=new PXTkFrame(this->EventManager,oFrame);
  PXTkFrame* midfr=new PXTkFrame(this->EventManager,oFrame);
  PXTkFrame* midfr2=new PXTkFrame(this->EventManager,oFrame);
  PXTkFrame* midfr3=new PXTkFrame(this->EventManager,oFrame);
  PXTkFrame* botfr=new PXTkFrame(this->EventManager,oFrame);  
  PXTkFrame* botfr2=new PXTkFrame(this->EventManager,oFrame);  
  oFrame->addChildren("-side top -pady 1 -expand f -fill x",topfr,midfr,midfr3,midfr2);
  oFrame->addChildren("-side top -pady 1 -expand f -fill x",botfr,botfr2);

  

  PXTkButton* but3=new PXTkButton(this->EventManager,topfr,"x0.8",600);
  PXTkButton* but4=new PXTkButton(this->EventManager,topfr,"x1.0",601);
  PXTkButton* but4a=new PXTkButton(this->EventManager,topfr,"x1.25",602);
  PXTkButton* but4b=new PXTkButton(this->EventManager,topfr,"x2",603);

  SmoothingEntry=new PXTkEntry(this->EventManager,topfr);
  SmoothingEntry->configure("-width",6);
  SmoothingEntry->configure("-bg","red");
  char line[10]; sprintf(line,"%f",this->DefaultSmoothing); SmoothingEntry->setText(line);
  PXTkButton* but4c=new PXTkButton(this->EventManager,topfr,"sm",604);
  
  but3->configure("-padx",1);
  but4->configure("-padx",1);
  but4a->configure("-padx",1);
  but4b->configure("-padx",1);
  but4c->configure("-padx",1);

  topfr->addChildren("-side left ",but3,but4);
  topfr->addChildren("-side left ",but4a,but4b,SmoothingEntry,but4c);
  

  PXTkButton* but6=new PXTkButton(this->EventManager,midfr,"Color",450);
  PXTkButton* but7=new PXTkButton(this->EventManager,midfr,"C-Color",451);
  PXTkButton* but5=new PXTkButton(this->EventManager,midfr,"Ellipse",452);
  PXTkButton* bbut5=new PXTkButton(this->EventManager,midfr,"Area",453);
  but5->configure("-padx",0);
  but6->configure("-padx",0);
  but7->configure("-padx",0);
  bbut5->configure("-padx",0);
  midfr->addChildren("-side left -padx 0 -expand t -fill x",but6,but7,but5,bbut5);


  if (this->SlaveMode==0)
    {
      for (int j=0;j<=1;j++)
	{
	  thresholdEntry[j]=new PXTkEntry(this->EventManager,midfr3);
	  thresholdEntry[j]->configure("-width",6);
	  thresholdEntry[j]->setText("0.1");
	}
      thresholdEntry[1]->setText("50.0");
      PXTkButton* bbut6=new PXTkButton(this->EventManager,midfr3,"ROI Props >",454);
      bbut6->configure("-padx",0);
      midfr3->addChildren("-side left -padx 0 -expand t -fill x",bbut6,thresholdEntry[0],thresholdEntry[1]);
    }

  PXTkButton* but9=new PXTkButton(this->EventManager,midfr2,"Snake",610);
  PXTkButton* but10=new PXTkButton(this->EventManager,midfr2,"Upd Snake",612);
  but9->configure("-padx",0);
  but10->configure("-padx",0);
  midfr2->addChildren("-side left -padx 0 -expand t -fill x",but9,but10);

  if (this->SlaveMode==0)
    {
      PXTkButton* but1=new PXTkButton(this->EventManager,botfr,"Load",401);
      PXTkButton* but2=new PXTkButton(this->EventManager,botfr,"Save",402);
      PXTkButton* but2a=new PXTkButton(this->EventManager,botfr,"Export",403);
      but1->configure("-padx",0);
      but2->configure("-padx",0);
      but2a->configure("-padx",0);
      botfr->addChildren("-side left -padx 0 -expand t -fill x",but1,but2,but2a);
    }

  PXTkButton* cp=new PXTkButton(this->EventManager,botfr2,"Copy",590);
  PXTkButton* pt=new PXTkButton(this->EventManager,botfr2,"Paste",591);
  PXTkButton* ptf=new PXTkButton(this->EventManager,botfr2,"Paste Flip",592);
  PXTkButton* un=new PXTkButton(this->EventManager,botfr2,"Undo",593);
  
  cp->configure("-padx",0);
  pt->configure("-padx",0);
  ptf->configure("-padx",0);
  un->configure("-padx",0);
  botfr2->addChildren("-side left -padx 0 -expand f -fill x",cp,pt,ptf,un);




  if ( (this->EventParent!=NULL && this->CallbackNo>0) || ( this->CallbackName!=NULL ))
    {
      autoUpdate=new PXTkCheckButton(this->EventManager,bottomButtonFrame,"Auto Upd",615);
      if (this->AutoUpdateMode)
	autoUpdate->setState(PXTrue);
      PXTkButton* updB=new PXTkButton(this->EventManager,bottomButtonFrame,"Update",614);
      bottomButtonFrame->addChildren("-side left -padx 10 -expand t -fill x",updB,autoUpdate);
    }
  
  if (inside==0)
    {
      PXTkButton* closeB;
      if (this->EventParent)
	closeB=new PXTkButton(this->EventManager,bottomButtonFrame,"Close",3);
      else
	closeB=new PXTkButton(this->EventManager,bottomButtonFrame,"Exit",1);
      bottomButtonFrame->addChildren("-side right -expand false -fill x -padx 10",closeB);
    }

  // Other Stuff 
  UpdateStatus();
  return GetWidgetName();
}

/* -------------------------------------------------------------------------*/ 
void vtkpxGUISplineControl::SetRenderer(vtkRenderer *ren,int num)
{
  this->SetRenderer(ren);
}

void vtkpxGUISplineControl::SetRenderer(vtkRenderer* ren)
{
  for (int i=0;i<this->NumberOfSplines;i++)
    {
      double c[3]; vtkpxSurfaceUtil::GetColor(i,c);
      if (splineActor[i]==NULL)
	{
	  vtkPolyDataMapper* map=vtkPolyDataMapper::New();
	  map->SetInput(splineSet[i]->GetOutput());
	  
	  splineActor[i]=vtkActor::New();
	  splineActor[i]->SetMapper(map);
	  
	  if (display_mode[i])
	    splineActor[i]->SetVisibility(1);
	  else
	    splineActor[i]->SetVisibility(0);
	  splineActor[i]->GetProperty()->SetColor(c);
	  map->Delete();
	}
      ren->AddActor(splineActor[i]);      
      for (int j=0;j<2;j++)
	{
	  if (ghostActor[j][i]==NULL)
	    {
	      vtkPolyDataMapper* map=vtkPolyDataMapper::New();
	      map->SetInput(ghostSet[j][i]->GetOutput());
	  
	      ghostActor[j][i]=vtkActor::New();
	      ghostActor[j][i]->SetMapper(map);
	      ghostActor[j][i]->GetProperty()->SetOpacity(0.3);

	      if (j==0 && (ghost_displaymode==1 || ghost_displaymode==3))
		ghostActor[j][i]->SetVisibility(1);
	      else if (j==1 && ghost_displaymode>=2)
		ghostActor[j][i]->SetVisibility(1);
	      else
		ghostActor[j][i]->SetVisibility(0);
	      map->Delete();
	    }
	  ren->AddActor(ghostActor[j][i]);
	}
    }
  
  if (editActor==NULL)
    {
      vtkPolyDataMapper* map=vtkPolyDataMapper::New();
      map->SetInput(editCurve->GetOutput());

      editActor=vtkActor::New();
      editActor->SetMapper(map);
      map->Delete();
    }

  if (manualEditActor==NULL)
    {
      vtkPolyDataMapper* map=vtkPolyDataMapper::New();
      map->SetInput(manualEditCurve->GetOutput());

      manualEditActor=vtkActor::New();
      manualEditActor->SetMapper(map);
      manualEditActor->SetVisibility(0);
      map->Delete();
    }

  ren->AddActor(editActor);
  ren->AddActor(manualEditActor);
  this->Renderer=ren;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::Update()
{
  if (this->splineActor[0]==NULL)
      return;

  for (int i=0;i<this->NumberOfSplines;i++)
      {
	if (display_mode[i] && spline_enabled[i]==1)
	  splineActor[i]->SetVisibility(1);
	else
	  splineActor[i]->SetVisibility(0);

	double c[3];
	for (int ib=0;ib<=2;ib++)
	  c[ib]=0.1;
	double w=splineActor[i]->GetProperty()->GetLineWidth();
	w*=0.5;

	for (int j=0;j<2;j++)
	  {
	    if (j==0)
	      {
		if ((ghost_displaymode==1 || ghost_displaymode==3) && spline_enabled[i]==1)
		  ghostActor[j][i]->SetVisibility(1);
		else
		  ghostActor[j][i]->SetVisibility(0);
	      }
	    else if (j==1)
	      {
		if (ghost_displaymode>=2 && spline_enabled[i]==1)
		  ghostActor[j][i]->SetVisibility(1);
		else
		  ghostActor[j][i]->SetVisibility(0);
	      }
	    ghostActor[j][i]->GetProperty()->SetColor(c);
	    ghostActor[j][i]->GetProperty()->SetLineWidth(w);
	  }
      }

  editActor->SetVisibility(splineActor[current_spline]->GetVisibility());

  // Curve Stuff 
  UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::UpdateDisplay()
{
  if (this->Renderer!=NULL)
    this->Renderer->GetRenderWindow()->Render();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::UpdateStatus()
{
  if (!this->Initialized)
      return;

  if (spline_enabled[current_spline]==1)
    sprintf(pxtk_buffer4,"Num Points=%d Edit=%d",
	    currentSpline->GetNumPoints(),
	    editCurve->GetNumPoints());
  else
    sprintf(pxtk_buffer4,"Spline Disabled!");
  message->setText(pxtk_buffer4);
}
/* -------------------------------------------------------------------------*/
int  vtkpxGUISplineControl::HandleEvent(int event)
{
  if (!this->Initialized)
    return TCL_OK;
  
  if (event<10)
    return this->EventManager->handleDirectEvent(event);

  if (event==500)
    {
      SetCurrentCollection(currentCollection->getIndex());
      if (mouseMode->getState())
	{
	  if (currentSpline->GetNumPoints()==0)
	    mouseMode->setState(PXFalse);
	}
    }
  else if (event==607 || event==606)
    {
      DoExtract(event-606);
    }
  else if (event==614)
    {
      if (spline_enabled[current_spline]==1)
	this->CopySpline();


      if (this->EventParent!=NULL && this->CallbackNo>0)
	this->EventParent->HandleEvent(CallbackNo);
      else
	if (this->CallbackName!=NULL)
	  PXTkApp::executeTclCommand(this->CallbackName);
    }
  else if (event==615)
    {
      if (autoUpdate->getState())
	this->AutoUpdateMode=1;
      else
	this->AutoUpdateMode=0;
    }
  else if (spline_enabled[current_spline]==1)
    {
      switch(event)
	{
	case 401:
	  {
	    int ok=1;
	    if (currentSpline->GetNumPoints()>0)
	      ok=this->EventManager->questionBox("Loading will delete your old spline. Are you sure?");
	    if (ok)
	      {
		char* line=this->EventManager->getOpenFilename("Load splines","Splines","*.spl");
		if (line!=NULL)
		  {
		    if (strlen(line)>0)
		      LoadSpline(line);
		  }
	      }
	  }
	  break;
	  
	case 402:
	  {
	    char* line=this->EventManager->getSaveFilename("Save Splines","spline.spl","Splines","*.spl");
	    if (line!=NULL)
	      {
		if (strlen(line)>0)
		  {
		    strcpy(pxtk_buffer2,line);
		    if(currentSpline->Save(line))
		      {
			this->SetItemFilename(current_spline,pxtk_buffer2);
			UpdateStatus();
		      }
		    else
		      {
			sprintf(pxtk_buffer2,"Failed to save splines %d in %s",current_spline+1,line);
			this->EventManager->messageBox(pxtk_buffer2,"Save Failure");
		      }
		  }
	      }
	  }
	  break;
	  
	case 403:
	  {
	    char* line=this->EventManager->getSaveFilename("Save Contour","spline.cntr","Contours","*.cntr");
	    if (line!=NULL)
	      {
		if (strlen(line)>0)
		  {
		    strcpy(pxtk_buffer2,line);
		    vtkPoints* temp=vtkPoints::New();
		    this->currentSpline->ExportKnotPointsToPoints(temp);
		    FILE* fout=fopen(line,"w");
		    if (fout)
		      {
			fprintf(fout,"#Number of Landmarks \n%d\n",temp->GetNumberOfPoints());
			fprintf(fout,"#Landmarks\n");
			for (int i=0;i<temp->GetNumberOfPoints();i++)
			  {
			    double x[3];
			    temp->GetPoint(i,x);
			    fprintf(fout,"%6.2f %6.2f %6.2f\n",x[0],x[1],x[2]);
			  }
			fclose(fout);
		      }
		    temp->Delete();
		  }
	      }
	  }
	  break;
	  
	case 450:
	  if (splineActor[current_spline]!=NULL)
	    {
	      double c[3];
	      splineActor[current_spline]->GetProperty()->GetColor(c);
	      int a=this->EventManager->d_colorBox("Spline Color",c[0],c[1],c[2]);
	      if (a==1)
		this->SetItemColor(current_spline,c);
	    }
	  break;
	  
	case 451:
	  if (editActor!=NULL)
	    {
	      double c[3];
	      editActor->GetProperty()->GetColor(c);
	      int a=this->EventManager->d_colorBox("Controls Color",c[0],c[1],c[2]);
	      if (a==1)
		editActor->GetProperty()->SetColor(c);
	    }
	  break;
	  
	case 452: // Ellipse
	  this->CopyToUndoSpline();
	  currentSpline->SetEllipse(8,
				    spacing[0]*float(dimensions[0])/8.0*(1.0+current_spline),
				    spacing[1]*float(dimensions[0])/8.0*(1.0+current_spline),
				    origin[0]+spacing[0]*dimensions[0]/2.0,
				    origin[1]+spacing[1]*dimensions[1]/2.0,
				    slice_offset+float(image_level)*this->spacing[2]+this->origin[2]);
	  UpdateControlsFromCurrentSpline();
	  break;
	  
	case 453:
	  sprintf(pxtk_buffer,"Curve %d Area=%8.1f mm^2\n",current_spline+1,currentSpline->GetArea());
	  PXTkApp::printToConsole(pxtk_buffer);
	  this->EventManager->messageBox(pxtk_buffer,"Curve Area ...");
	  break;
	  
	case 454:
	  if (image!=NULL)
	    {
	      float area[2],mean[2],std[2],threshold[2];
	      int np[2];
	      for (int ii=0;ii<2;ii++)
		{
		  threshold[ii]=atof(thresholdEntry[ii]->getText());
		  sprintf(pxtk_buffer,"%5.2f",threshold[ii]);
		  thresholdEntry[ii]->setText(pxtk_buffer);
		}
	      
	      int nt=currentSpline->GetDualVOIProperties(image,image_level,image_frame,
							 area,mean,std,np,threshold);
	      
	      if (area[0]<0.0001)
		area[0]=0.0001;
	      
	      sprintf(pxtk_buffer,"Curve %d Total Enclosed Area=%8.1f mm^2 npix=%d\n",
		      current_spline+1,currentSpline->GetArea(),nt);
	      PXTkApp::printToConsole(pxtk_buffer);
	      for (int i=0;i<=1;i++)
		{
		  sprintf(pxtk_buffer,"\t ROI %d area >%.1f \t= %6.1f (mm^2) mean=%5.1f std=%5.1f npix=%6d\n",
			  i+1,threshold[i],area[i],mean[i],std[i],np[i]);
		  PXTkApp::printToConsole(pxtk_buffer);
		}
	      sprintf(pxtk_buffer, "\t Ratio 2/1  \t= %5.2f %% \n\n",
		      area[1]/area[0]*100.0);
	      PXTkApp::printToConsole(pxtk_buffer);
	      PXTkApp::popupConsole();
	    }
	  break;
	  
	case 501:
	  display_mode[current_spline]=displayMode->getState();
	  Update();
	  break;

	case 502:
	  if (editActor!=NULL)
	    {
	      if (showControls->getState())
		editActor->SetVisibility(1);
	      else
	      editActor->SetVisibility(0);
	  }
	  break;
	  
	case 503:
	  scale_mode[current_spline]=scaleMode->getIndex();
	  editCurve->SetPointScale(0.25+0.25*float(scale_mode[current_spline])*this->ControlPointScaleFactor);
	  /*fprintf(stderr,"Dist Threshold=%f controlpointscalefactor=%f pointscale=%f\n",
		  this->DistanceThreshold,this->ControlPointScaleFactor,
		  editCurve->GetPointScale());*/
	  break;
	  
	case 504:
	  slice_offset=-slice_offset;
	  this->SetLevel(image_level);
	  break;
	  
	case 505:
	  {
	    for  (int i=0;i<this->NumberOfSplines;i++)
	      display_mode[i]=PXFalse;
	    displayMode->setState(PXFalse);
	  }
	  Update();
	  break;
	  
	case 506:
	{
	  for  (int i=0;i<this->NumberOfSplines;i++)
	    display_mode[i]=PXTrue;
	  displayMode->setState(PXTrue);
	}
	Update();
	break;
	
	case 507:
	  ghost_displaymode=ghostMode->getIndex();
	  Update();
	  break;
	  
	case 510:
	  if (editMode->getState()==PXFalse)
	    {
	      ExtractButton->enable(PXFalse);
	      if (this->manual_add_mode==1)
		this->FitSplineToCurrentControls();
	    }
	  else
	    {
	      this->manual_add_mode=1;
	      ExtractButton->enable(PXTrue);
	      splineSet[current_spline]->ExportToBaseCurve(this->manualEditCurve);
	      this->manualEditCurve->SetPointScale(editCurve->GetPointScale()*0.5);
	      this->manualEditCurve->DeleteAllPoints();
	      this->manualEditActor->SetVisibility(1);
	    }
	  break;

	case 600: // x0.8
	  {
	    this->CopyToUndoSpline();
	    float s=this->currentSpline->GetSpline()->getlength(0.0,1.0,0.01)/100.0;
	    currentSpline->ChangeSpacing(this->DefaultSmoothing*s,(80*currentSpline->GetNumPoints())/100);
	    this->changed[current_spline]=1;
	    UpdateControlsFromCurrentSpline();
	  }
	  break;
	  
	case 601: // x1.0
	  {
	    this->CopyToUndoSpline();
	    float s=this->currentSpline->GetSpline()->getlength(0.0,1.0,0.01)/100.0;
	    fprintf(stderr,"S=%.2f, this->Default=%f\n",s,this->DefaultSmoothing);
	    currentSpline->ChangeSpacing(this->DefaultSmoothing*s,currentSpline->GetNumPoints());
	    this->changed[current_spline]=1;
	    UpdateControlsFromCurrentSpline();
	  }
	  break;
	  
	case 602: // x1.25
	  {
	    this->CopyToUndoSpline();
	    float s=this->currentSpline->GetSpline()->getlength(0.0,1.0,0.01)/100.0;
	    currentSpline->ChangeSpacing(this->DefaultSmoothing*s,(125*currentSpline->GetNumPoints())/100);
	    this->changed[current_spline]=1;
	    UpdateControlsFromCurrentSpline();
	  }
	  break;
	  
	case 603: // x2.0
	  {
	    this->CopyToUndoSpline();
	    float s=this->currentSpline->GetSpline()->getlength(0.0,1.0,0.001)/100.0;
	    currentSpline->ChangeSpacing(this->DefaultSmoothing*s,(200*currentSpline->GetNumPoints())/100);
	    this->changed[current_spline]=1;
	    UpdateControlsFromCurrentSpline();
	    PXFPBSpline::debugmode=0;
	  }
	  break;
	  
	case 604: // sD
	  {
	    this->CopyToUndoSpline();
	    float s=this->currentSpline->GetSpline()->getlength(0.0,1.0,0.01)/100.0;
	    float t=Frange(atof(SmoothingEntry->getText()),0.0001,10.0);
	    char line[10]; sprintf(line,"%f",t); SmoothingEntry->setText(line);
	    currentSpline->ChangeSpacing(t*s,-1);
	    this->changed[current_spline]=1;
	    UpdateControlsFromCurrentSpline();
	  }
	break;
	
	  
	case 590:
	  this->CopySpline();
	  break;
	  
	case 591:
	  this->PasteSpline();
	  break;

	case 592:
	  this->PasteFlipSpline();
	  break;
	  
	case 593:
	  this->UndoSpline();
	  break;
	  
	case 610:
	  DoSnake(0);
	  break;
	  
	case 611:
	  this->CopyToUndoSpline();
	  DoSnake(1);
	  break;
	  
	case 612:
	  this->CopyToUndoSpline();
	  DoSnake(2);
	  break;
	  
	case 613:
	  DoSnake(3);
	  break;
	  
	}
    }


  UpdateDisplay();

  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::SetItemColor(int index,double color[3])
{
  if (splineActor[index]!=NULL)
    splineActor[index]->GetProperty()->SetColor(color);

}

void vtkpxGUISplineControl::SetItemFilename(int index,const char* s)
{
  if (this->Initialized==0)
    return;

  //  fprintf(stderr,"GUISpline:index=%d s=%s\n",index,s);

  if (index==-1)
    {
      sprintf(pxtk_buffer3,"%s.%d",PXTkApp::getFilenameTail(s),current_spline+1);
      currentCollection->changeLabel(current_spline,pxtk_buffer3);
      return;
    }

  sprintf(pxtk_buffer4,"%s.%d",PXTkApp::getFilenameTail(s),index+1);
  currentCollection->changeLabel(index,pxtk_buffer4);
}

/* -------------------------------------------------------------------------*/

void vtkpxGUISplineControl::CopySpline()
{
  clipboardSpline->Copy(currentSpline);
}

void vtkpxGUISplineControl::PasteSpline()	
{
  if (clipboardSpline->GetNumPoints()>0)
    SetSpline(clipboardSpline,-1);
}


void vtkpxGUISplineControl::PasteFlipSpline()	
{
  if (clipboardSpline->GetNumPoints()<1)
    return;

  PXFPBSpline* spl=clipboardSpline->GetSpline();

  float cx=spacing[0]*dimensions[0]/2+origin[0];

  for (int i=0;i<spl->getnumcontrolpoints();i++)
    {
      float x1=spl->getcontrolx(i);
      spl->shiftcontrol(i,-2.0*(x1-cx),0,0);
    }

  SetSpline(clipboardSpline,-1);
}


void vtkpxGUISplineControl::CopyToUndoSpline()
{
  undoSpline->Copy(currentSpline);
}

void vtkpxGUISplineControl::UndoSpline()	
{
  if (undoSpline->GetNumPoints()>0)
    SetSpline(undoSpline,-1);
}

/* -------------------------------------------------------------------------*/
vtkpxSplineSource* vtkpxGUISplineControl::GetSpline(int index)
{
  if (index==-1)
      index=current_spline;

  return splineSet[Irange(index,0,this->NumberOfSplines-1)];
}
/* -------------------------------------------------------------------------*/
int vtkpxGUISplineControl::GetCurrentIndex()
{
  return current_spline;
}

int  vtkpxGUISplineControl::GetCurrentSliceNumber()
{
  return image_level;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::SetSpline(vtkpxSplineSource* land,int index)
{
  if (index==-1)
    index=current_spline;

  index=Irange(index,0,this->NumberOfSplines-1);

  splineSet[index]->Copy(land);
  float zlev=image_level*this->spacing[2]+this->origin[2];
  splineSet[index]->SetZlevel(zlev+slice_offset);
  this->manual_zcoord=zlev+slice_offset;
  this->changed[index]=1;  


  if (index==current_spline)
    UpdateControlsFromCurrentSpline();
  
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::SetChangedStatus(int index,int val)
{
  index=Irange(index,0,this->NumberOfSplines-1);
  this->changed[index]=(val>0);

}

int  vtkpxGUISplineControl::GetChangedStatus(int index)
{
  index=Irange(index,0,this->NumberOfSplines-1);
  return this->changed[index];
}

/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::SetGhostSpline(vtkpxSplineSource* land,int ghost,int index)
{
  if (index==-1)
      index=current_spline;
  ghost=Irange(ghost,0,1);
  index=Irange(index,0,this->NumberOfSplines-1);

  ghostSet[ghost][index]->Copy(land);
  float zlev=image_level*this->spacing[2]+this->origin[2];
  ghostSet[ghost][index]->SetZlevel(zlev+slice_offset);
}
/* -------------------------------------------------------------------------*/
vtkpxSplineSource* vtkpxGUISplineControl::GetGhostSpline(int ghost,int index)
{
 if (index==-1)
      index=current_spline;
  ghost=Irange(ghost,0,1);
  index=Irange(index,0,this->NumberOfSplines-1);
  return ghostSet[ghost][index];
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::SetImage(vtkImageData* img,int plane,int level,int frame)
{
  if (img!=NULL)
    {
      if (this->image!=NULL)
	this->image->Delete();
      this->image=vtkImageData::New();
      this->image->ShallowCopy(img);
      
      int d[3]; double sp[3],ori[3];
      image->GetSpacing(sp);
      image->GetOrigin(ori);
      image->GetDimensions(d);

      double sum=0.0;
      for (int ia=0;ia<=2;ia++)
	sum+=fabs(float(d[ia]-this->dimensions[ia]))+fabs(sp[ia]-this->spacing[ia])+fabs(ori[ia]-this->origin[ia]);

      if (sum>0.01)
	{
	  this->SetSpacingOrigin(sp,ori);
	  this->SetLevel(level);
	  image->GetDimensions(d);
	  image->GetDimensions(this->dimensions);
	  	  
	  for (int i=0;i<this->NumberOfSplines;i++)
	    {
	      splineSet[i]->SetEllipse(6,
				       spacing[0]*dimensions[0]/8.0*(1.0+i),
				       spacing[1]*dimensions[1]/8.0*(1.0+i),
				       origin[0]+spacing[0]*dimensions[0]/2.0,
				       origin[1]+spacing[1]*dimensions[1]/2.0,
				       0.0,2);
	      splineSet[i]->SetResolution(sp[0]);
	      for (int j=0;j<2;j++)
		ghostSet[j][i]->Copy(splineSet[i]);
	    }

	  this->DistanceThreshold=(spacing[0]*dimensions[0])/50.0;
	  this->ControlPointScaleFactor=this->DistanceThreshold*0.5;
	  editCurve->SetPointScale(0.25+0.25*float(scale_mode[current_spline])*this->ControlPointScaleFactor);
	  /*fprintf(stderr,"Dist Threshold=%f controlpointscalefactor=%f pointscale=%f\n",
		  this->DistanceThreshold,this->ControlPointScaleFactor,
		  editCurve->GetPointScale());*/
	}
      else
	{
	  if (level!=image_level)
	    this->SetLevel(level);
	  image_frame=frame;
	}
    }
}

void vtkpxGUISplineControl::SetPlaneLevel(int plane,int level)
{
  this->SetLevel(level);
}


void vtkpxGUISplineControl::SetSpacingOrigin(double sp[3],double ori[3])
{
  for (int ia=0;ia<=2;ia++)
    {
      this->origin[ia]=ori[ia];
      this->spacing[ia]=sp[ia];
    }


  for (int i=0;i<this->NumberOfSplines;i++)
    {
      splineSet[i]->SetImageOrigin(this->origin);
      splineSet[i]->SetImageSpacing(this->spacing);
    }

}

void vtkpxGUISplineControl::SetLevel(int level)
{
  image_level=level;
  double zlev=level*this->spacing[2]+this->origin[2];
  this->manual_zcoord=zlev+slice_offset;
  for (int i=0;i<this->NumberOfSplines;i++)
    {
      splineSet[i]->SetZlevel(zlev+slice_offset);
      for (int j=0;j<=1;j++)
	ghostSet[j][i]->SetZlevel(zlev+slice_offset);
    }
  //fprintf(stderr,"Set Zlevel =%.1f (level=%d ori=%.1f sp=%.1f)\n",zlev,image_level,origin[2],spacing[2]);
  UpdateControlsFromCurrentSpline();
}

/* -------------------------------------------------------------------------*/
int vtkpxGUISplineControl::HandleClickedPoint(float x,float y,float z,int scaled,int state)
{
  static int found_point=-1;
  if (!mouseMode->getState())
    return 0;

  if (spline_enabled[current_spline]==0)
    return 0;


  if (this->manual_add_mode==1)
    {
      switch (state)
	{
	case 0:
	  this->manualEditCurve->AddPoint(x,y,manual_zcoord,scaled);
	  break;
	case 1:
	  this->manualEditCurve->SetPoint(this->manualEditCurve->GetNumPoints()-1,x,y,manual_zcoord,scaled);
	  break;
	case 2:
	  break;
	}
      return 1;
    }


  if (editActor==NULL)
    return 0;

  if (editActor->GetVisibility()==0)
    return 0;


  int numitems=currentSpline->GetNumPoints();//listBox->getNumItems();
  if (numitems<1)
    return 0;

  double lx[3];
  switch (state)
    {
    case 0:
    case 6:
      {
	lx[0]=x;lx[1]=y;lx[2]=z;
	found_point=editCurve->FindClosestPoint(lx,this->DistanceThreshold,1);
	if (found_point>=0)
	  return 1;
      }
      break;

    case 1:
      if (found_point>=0)
	{
	  double x1,y1,z1;
	  editCurve->GetPoint(found_point,x1,y1,z1);
	  editCurve->SetPoint(found_point,x,y,z1,scaled);
	  currentSpline->SetFromBaseCurve(editCurve);
	  this->SetChangedStatus(current_spline,1);
	  return 1;
	  return 1;
	}
      break;

    case 7:
      if (found_point>=0)
	{
	  double x1,y1,z1;
	  editCurve->GetPoint(found_point,x1,y1,z1);
	  float dx=x-x1;
	  float dy=y-y1;
	  int np=editCurve->GetNumPoints();
	  for (int ia=0;ia<np;ia++)
	    {
	      editCurve->GetPoint(ia,x1,y1,z1);
	      editCurve->SetPoint(ia,x1+dx,y1+dy,z1,scaled);
	    }
	  currentSpline->SetFromBaseCurve(editCurve);
	  this->SetChangedStatus(current_spline,1);
	  return 1;
	}
      break;

    case 2:
    case 8:
      if (found_point>=0)
	{
	  found_point=-1;
	  UpdateStatus();
	  return 1;
	}
      break;

    case 5:
      {
	int ok=currentSpline->InsertKnot(x,y);
	if (ok==1)
	  {
	    UpdateControlsFromCurrentSpline();
	  }
      }
      break;
    }
  return 0;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::LoadSpline(const char* fname)
{
  strcpy(pxtk_buffer2,fname);
  int ok=currentSpline->Load(fname);
  if (!ok)
      {
	sprintf(pxtk_buffer2,"Failed to load landmarks %d from %s",current_spline+1,fname);
	this->EventManager->messageBox(pxtk_buffer2,"Load Failure");
      }
  else
    {
      this->SetItemFilename(current_spline,pxtk_buffer2);
      UpdateControlsFromCurrentSpline();
    }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::SetCurrentCollection(int num)
{
  num=Irange(num,0,this->NumberOfSplines-1);
  if (num==current_spline)
      return;
  current_spline=num;

  currentSpline=splineSet[current_spline];  
  UpdateControlsFromCurrentSpline();
  UpdateStatus();
  Update();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::UpdateControlsFromCurrentSpline()
{
  if (spline_enabled[current_spline]==0)
    {
      UpdateStatus();
      return;
    }
  this->editCurve->SetShowCurve(0);
  splineSet[current_spline]->ExportToBaseCurve(editCurve);
  displayMode->setState(display_mode[current_spline]);
  scaleMode->setIndex(scale_mode[current_spline]);
  UpdateStatus();
}
/* -------------------------------------------------------------------------*/
int vtkpxGUISplineControl::DoSnake(int apply)
{
  if (image==NULL)
      return 0;
  
  if (apply==0)
    {
      if (snakeGUI==NULL)
	{
	  PXTkFrame* polyFrame=(PXTkFrame*)(this->EventManager->getMainWindow());
	  snakeGUI=new PXTkDialog(this->EventManager,polyFrame,"Snake Controls",PXFalse);
	  snakeGUI->initDisplay();
	  PXTkFrame* frame=(PXTkFrame*)snakeGUI->getMainWindow();
	  
	  snakeEdgeWeight=new PXTkArrowScale(snakeGUI,frame,"Edge Weight",-1,PXTrue);
	  snakeEdgeWeight->setRange(0.0,10.0);
	  snakeEdgeWeight->setIncrement(0.5);
	  snakeEdgeWeight->setResolution(0.1);
	  snakeEdgeWeight->setDecimalPoints(1);
	  snakeEdgeWeight->setLengthWidth(120,8);
	  
	  snakeSmoothWeight=new PXTkArrowScale(snakeGUI,frame,"Smoothness Weight",-1,PXTrue);
	  snakeSmoothWeight->setRange(0.0,10.0);
	  snakeSmoothWeight->setIncrement(0.5);
	  snakeSmoothWeight->setResolution(0.1);
	  snakeSmoothWeight->setDecimalPoints(1);
	  snakeSmoothWeight->setLengthWidth(120,8);
	  
	  snakeSigma=new PXTkArrowScale(snakeGUI,frame,"Sigma",-1,PXTrue);
	  snakeSigma->setRange(0.0,4.0);
	  snakeSigma->setIncrement(0.5);
	  snakeSigma->setResolution(0.1);
	  snakeSigma->setDecimalPoints(1);
	  snakeSigma->setLengthWidth(120,8);
	  
	  snakeIterations=new PXTkArrowScale(snakeGUI,frame,"Iterations",-1,PXTrue);
	  snakeIterations->setRange(1,20);
	  snakeIterations->setIncrement(3);
	  snakeIterations->setResolution(1);
	  snakeIterations->setDecimalPoints(0);
	  snakeIterations->setLengthWidth(120,8);
	  

	  snakeStep=new PXTkArrowScale(snakeGUI,frame,"Step",-1,PXTrue);
	  snakeStep->setRange(0.0,1.0);
	  snakeStep->setIncrement(0.05);
	  snakeStep->setResolution(0.01);
	  snakeStep->setDecimalPoints(2);
	  snakeStep->setLengthWidth(120,8);

	  PXTkFrame* fr2=new PXTkFrame(snakeGUI,frame);
	  PXTkButton* b1=new PXTkButton(this->EventManager,fr2,"Create",611);
	  PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Update",612);
	  PXTkButton* b3=new PXTkButton(this->EventManager,fr2,"Reset",613);
	  PXTkButton* b4=new PXTkButton(snakeGUI,fr2,"Close",3);
	  
	  b1->configure("-padx",1);
	  b2->configure("-padx",1);
	  b3->configure("-padx",1);
	  b4->configure("-padx",1);
	  
	  frame->addChildren("-side top -expand true -fill x",
			     snakeEdgeWeight->getMainWindow(),
			     snakeSmoothWeight->getMainWindow(),
			     snakeIterations->getMainWindow());
	  frame->addChildren("-side bottom -expand true -fill x",
			     fr2,
			     snakeSigma->getMainWindow(),
			     snakeStep->getMainWindow());
	  fr2->addChildren("-side left -expand true -fill x",b1,b2,b3,b4);
	}
      
      /*snakeEdgeWeight->setRange(image_range[0],image_range[1]);
	snakeEdgeWeight->setValue(Frange(snakeEdgeWeight->getValue(),image_range[0],image_range[1]));*/
      snakeGUI->popupDialog();
      DoSnake(3);
    }
  

  if (snakeGUI==NULL)
    {
      DoSnake(0);
      snakeGUI->popdownDialog();
    }
  
  
  if (apply==3)
    {
      snakeEdgeWeight->setValue(3.0);
      snakeSmoothWeight->setValue(3.0);
      snakeSigma->setValue(1.0);
      snakeIterations->setValue(5);
      snakeStep->setValue(0.05);
    }

  if (apply==1)
    {
      float w_img=snakeEdgeWeight->getValue()*10.0;
      float w_smo=snakeSmoothWeight->getValue()*0.1;
      float sigma=snakeSigma->getValue();
      float stp=snakeStep->getValue();
      int   iter=(int)snakeIterations->getValue();
      
      currentSpline->CreateSnake(image,image_level,image_frame,w_img,w_smo,stp,sigma,iter);
      this->changed[current_spline]=1;
    }

  if (apply==1 || apply==2)
    {
      float w_img=snakeEdgeWeight->getValue()*10.0;
      float w_smo=snakeSmoothWeight->getValue()*0.1;
      if (mouseMode->getState()==PXFalse)
	mouseMode->setState(PXTrue);
      currentSpline->UpdateSnake(w_img,w_smo);
      this->changed[current_spline]=1;
      UpdateControlsFromCurrentSpline();
    }

  return 1;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::EnableSpline(int i)
{
  i=Irange(i,0,this->NumberOfSplines-1);
  spline_enabled[i]=1;

}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::DisableSpline(int i)
{
  i=Irange(i,0,this->NumberOfSplines-1);
  spline_enabled[i]=0;

}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineControl::FitSplineToCurrentControls()
{
  int nump=manualEditCurve->GetNumPoints();
  int ok=0;

  if (nump<4)
    ok=this->EventManager->questionBox("Not enough points to fit spline! Do you want to add more points? (No will revert to current spline)");
  else
    ok=this->EventManager->questionBox("Do you want to fit spline to current set of controls? This will delete your old spline. Are you sure?");

  if (ok==0)
    {
      this->manual_add_mode=0;
      this->manualEditActor->SetVisibility(0);
      return;
    }

  if (nump<4)
    {
      editMode->setState(PXTrue);
      ExtractButton->enable(PXTrue);
      return;
    }

  //  this->CopyToUndoSpline();
  manualEditCurve->Compact();
  float z=this->currentSpline->GetZlevel();
  this->currentSpline->FitPoints(manualEditCurve->GetPoints(),0.1,-1,1,100);
  this->currentSpline->ChangeSpacing(1.0,currentSpline->GetNumPoints());
  this->currentSpline->SetZlevel(z);
  this->currentSpline->Modified();

  this->manualEditActor->SetVisibility(0);
  this->manual_add_mode=0;
  UpdateControlsFromCurrentSpline();
}
/* -------------------------------------------------------------------------*/
int vtkpxGUISplineControl::DoExtract(int apply)
{
  if (image==NULL)
      return 0;

  if (this->manual_add_mode==0)
    {
      this->EventManager->messageBox("Must be in Manual point, and curve must have at least one point to do extraction","Error!");
      return 0;
    }

  if (manualEditCurve->GetNumPoints()==0)
    {
      this->EventManager->messageBox("Need at least a seed point to do extraction","Error!");
      return 0;
    }

  if (apply==0)
    {
      if (extractGUI==NULL)
	{
	  PXTkFrame* polyFrame=(PXTkFrame*)(this->EventManager->getMainWindow());
	  extractGUI=new PXTkDialog(this->EventManager,polyFrame,"Extract Controls",PXFalse);
	  extractGUI->initDisplay();
	      
	  PXTkFrame* frame=(PXTkFrame*)extractGUI->getMainWindow();
	  extractType=new PXTkOptionMenu(extractGUI,frame,"Auto Threshold",-1);
	  extractType->addOption("Manual Threshold");
	  extractType->setIndex(0);
	  
	  thresholdScale=new PXTkArrowScale(extractGUI,frame,"Threshold",-1,PXTrue);
	  thresholdScale->setRange(0.0,255.0);
	  thresholdScale->setValue(128.0);
	  thresholdScale->setIncrement(1);
	  thresholdScale->setResolution(1);
	  thresholdScale->setLengthWidth(120,8);
	  
	  medianScale=new PXTkArrowScale(extractGUI,frame,"Median Radius",-1,PXTrue);
	  medianScale->setRange(0.0,4.0);
	  medianScale->setValue(0.0);
	  medianScale->setIncrement(1.0);
	  medianScale->setResolution(1.0);
	  medianScale->setLengthWidth(120,8);
	  
	  sigmaScale=new PXTkArrowScale(extractGUI,frame,"Sigma",-1,PXTrue);
	  sigmaScale->setRange(0.0,5.0);
	  sigmaScale->setValue(0.0);
	  sigmaScale->setIncrement(0.5);
	  sigmaScale->setResolution(0.1);
	  sigmaScale->setDecimalPoints(1);
	  sigmaScale->setLengthWidth(120,8);
	  
	  PXTkFrame* fr2=new PXTkFrame(extractGUI,frame);
	  PXTkButton* b1=new PXTkButton(this->EventManager,fr2,"Extract",607);
		  PXTkButton* b3=new PXTkButton(extractGUI,fr2,"Close",3);
	  frame->addChildren("-side top -expand true -fill x",extractType,thresholdScale->getMainWindow(),
			     medianScale->getMainWindow(),sigmaScale->getMainWindow(),fr2);
	  fr2->addChildren("-side left -expand true -fill x",b1/*,b2*/,b3);
	}
      
      image->GetPointData()->GetScalars()->GetRange(image_range);
      thresholdScale->setRange(image_range[0],image_range[1]);
      thresholdScale->setValue(Frange(thresholdScale->getValue(),image_range[0],image_range[1]));
      extractGUI->popupDialog();
      return 0;
    }
  
  if (extractGUI==NULL)
    {
      DoExtract(0);
      extractGUI->popdownDialog();
    }


  int ok=0;



  if (extractType->getIndex()==0)
      {
	float threshold=-200000.0;
	ok=manualEditCurve->ExtractImageContour(image,2,image_level,image_frame,
					  threshold,
					  (int)medianScale->getValue(),sigmaScale->getValue());
	if (ok)
	  thresholdScale->setValue(threshold);
      }
  else
      {
	float threshold=thresholdScale->getValue();
	ok=manualEditCurve->ExtractImageContour(image,2,image_level,image_frame,threshold,
						   (int)medianScale->getValue(),sigmaScale->getValue());
      }

  if (ok)
    {
      int num=manualEditCurve->GetNumPoints();
      for (int i=0;i<num;i++)
	{
	  double x,y,z;
	  manualEditCurve->GetPoint(i,x,y,z);
	  manualEditCurve->SetPoint(i,x,y,this->manual_zcoord,0);
	}
      UpdateDisplay();
    }
  return 1;
}

