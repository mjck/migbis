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





// 	$Id: vtkpxGUISplineStackControl.cpp,v 1.4 2003/11/13 17:56:57 xenios Exp xenios $	

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkpxGUISplineStackControl.h"
#include "vtkpxUtil.h"
#include "vtkActor.h"
#include "vtkAssembly.h"
#include "vtkRenderer.h"
#include "vtkObjectFactory.h"
#include "vtkpxImageExtract.h"
#include "vtkPolyDataMapper.h"
#include "vtkImageData.h"
#include "vtkProperty.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkpxGUIBaseImageViewer.h"
#include "pxfpbsplinestack.h"
/* -------------------------------------------------------------------------*/
vtkpxGUISplineStackControl* vtkpxGUISplineStackControl::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUISplineStackControl");
  if(ret)
      {
	return (vtkpxGUISplineStackControl*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUISplineStackControl;
}

// ----------------------------------------------------------------------------
vtkpxGUISplineStackControl::vtkpxGUISplineStackControl()
{
  this->optionDialog=NULL;
  this->CurrentFrame=0;

  this->NumberOfSplineStacks=50;
  {
    for (int ia=0;ia<=2;ia++)
      {
	this->origin[ia]=0.0;
	this->spacing[ia]=1.0;
	this->dimensions[ia]=50;
      }
  }
  this->orientation=0;
  
  for (int i=0;i<this->NumberOfSplineStacks;i++)
    {
      splineStackSet[i]=NULL;
      for (int j=0;j<SSP_MAX_RENDERERS;j++)
	splineStackActor[j][i]=NULL;
      
      if (i==0)
	display_mode[i]=1;
      else
	display_mode[i]=0;
    }

  for (int j=0;j<SSP_MAX_RENDERERS;j++)
    this->Renderer[j]=NULL;

  for (int ia=0;ia<SSP_MAX_COLLECTIONS;ia++)
    {
      surface_filename[ia]=new char[20];
      sprintf(surface_filename[ia],"unnamed_%d",ia+1);
      surface_modified[ia]=0;
    }


  
  current_splinestack=0;
  this->currentSplineStack=NULL;

  clipboardSplineStack=NULL;
  image=NULL;
  this->SplineEditor=NULL;
  this->BlockEditorCallback=0;

  this->defaultsmoothing=5.0;
  char* line=PXTkApp::getTclVariable("pxtcl_pref_array(SplineSmoothing)");
  if (line!=NULL)
    {
      this->defaultsmoothing=Frange(atof(line),0.1,10.0);
    }

}
// ----------------------------------------------------------------------------
vtkpxGUISplineStackControl::~vtkpxGUISplineStackControl()
{
  
  if (clipboardSplineStack!=NULL)
    clipboardSplineStack->Delete();

  
  for (int i=0;i<this->NumberOfSplineStacks;i++)
    {
      if (splineStackSet[i]!=NULL)
	splineStackSet[i]->Delete();
      for (int j=0;j<SSP_MAX_RENDERERS;j++)
	if (splineStackActor[j][i]!=NULL)
	  splineStackActor[j][i]->Delete();
    }

  if (this->image!=NULL)
    this->image->Delete();

  for (int ia=0;ia<SSP_MAX_COLLECTIONS;ia++)
    delete [] surface_filename[ia];

    
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::SetNumberOfSplineStacks(int a)
{
  if (this->Initialized==1)
    return;

  this->NumberOfSplineStacks=Irange(a,1,SSP_MAX_COLLECTIONS);
}
// ----------------------------------------------------------------------------
void vtkpxGUISplineStackControl::SetSplineEditor(vtkpxGUISplineEditor* splineControl)
{
  this->SplineEditor=splineControl;
  if (this->SplineEditor!=NULL)
    {
      this->SplineEditor->SetNumberOfSplines(this->NumberOfSplineStacks);
      this->SplineEditor->SetCallback(this,700);
    }
}
// ----------------------------------------------------------------------------
void vtkpxGUISplineStackControl::InitializeSplineStacks()
{

  for (int i=0;i<this->NumberOfSplineStacks;i++)
    {
      splineStackSet[i]=vtkpxSplineStackSource::New();
      //      fprintf(stderr,"Init Spline Stacks Setting Elliptical Cylinder i=%d, %.2f\n",i,this->spacing[2]);
      splineStackSet[i]->SetEllipticalCylinder(8,
					       this->dimensions[2],
					       spacing[0]*dimensions[0]/(2.00*this->NumberOfSplineStacks)*(1.0+i),
					       spacing[1]*dimensions[1]/(2.00*this->NumberOfSplineStacks)*(1.0+i),
					       origin[0]+spacing[0]*dimensions[0]/2.0,
					       origin[1]+spacing[1]*dimensions[1]/2.0,
					       this->spacing[2],
					       this->origin[2]);
      splineStackSet[i]->SetDrawMode(0);
      surface_modified[i]=0;
    }

  
  current_splinestack=0;
  currentSplineStack=splineStackSet[current_splinestack];

  clipboardSplineStack=vtkpxSplineStackSource::New();
  clipboardSplineStack->Copy(currentSplineStack);

}
/* -------------------------------------------------------------------------*/
char* vtkpxGUISplineStackControl::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();

  InitializeSplineStacks();

  vtkpxGUIComponent::Initialize(name,inside);
  PXTkFrame* mFrame=(PXTkFrame*)this->EventManager->getMainWindow();
  this->controlFrame=new PXTkFrame(this->EventManager,mFrame);
  mFrame->addChildren("-side top -expand t -fill x -padx 0 -pady 2",this->controlFrame);

  // Control Properties
  // ---------------------
  PXTkLabelFrame* vFrame=new PXTkLabelFrame(this->EventManager,this->controlFrame,"Global Properties");
  this->controlFrame->addChildren("-side top -expand false -fill x -padx 0 -pady 5",vFrame);
  vFrame->setPackMode(PXFalse);
 
  PXTkButton* b0a=new PXTkButton(this->EventManager,vFrame,"Show All",506);
  PXTkButton* b0b=new PXTkButton(this->EventManager,vFrame,"Hide All",505);
  vFrame->gridRowColumnChild(b0a,1,0);
  vFrame->gridRowColumnChild(b0b,1,1);
  
  // Collection Properties 
  // ---------------------
  PXTkLabelFrame* pFrame=new PXTkLabelFrame(this->EventManager,this->controlFrame,"Spline Stack Properties");
  this->controlFrame->addChildren("-side top -expand t -fill both -padx 0 -pady 2",pFrame);

  PXTkFrame* pfr0=new PXTkFrame(this->EventManager,pFrame);
  PXTkFrame* pfr05=new PXTkFrame(this->EventManager,pFrame);
  PXTkFrame* pfr1=new PXTkFrame(this->EventManager,pFrame);
  PXTkFrame* pfr2=new PXTkFrame(this->EventManager,pFrame);  
  pFrame->addChildren("-side top -pady 1 -expand t -fill x",pfr0,pfr05,pfr1,pfr2);


  PXTkLabel* labs=new PXTkLabel(this->EventManager,pfr0,"Current Stack:");
  currentCollection=new PXTkOptionMenu(this->EventManager,pfr0,"Stack 1",500);
  for (int ip=1;ip<this->NumberOfSplineStacks;ip++)
    {
      sprintf(pxtk_buffer4,"Stack %d",ip+1);
      currentCollection->addOption(pxtk_buffer4);
    }
  currentCollection->setIndex(current_splinestack);
  pfr0->addChildren("-side left -expand t -fill x",labs,currentCollection);

  PXTkLabel* labd=new PXTkLabel(this->EventManager,pfr1,"Display Mode:");
  displayMode=new PXTkOptionMenu(this->EventManager,pfr1,"Hide",501);
  displayMode->addOption("Fast Surface");
  displayMode->addOption("Full Surface");
  displayMode->setIndex(display_mode[current_splinestack]);
  pfr0->addChildren("-side left -expand t -fill x",labd,displayMode);

  PXTkLabel* labd5=new PXTkLabel(this->EventManager,pfr05,"Draw Mode:");
  drawMode=new PXTkOptionMenu(this->EventManager,pfr05,"Open Surface",501);
  drawMode->addOption("Bottom Closed");
  drawMode->addOption("Top Closed");
  drawMode->addOption("Both Closed");
  drawMode->setIndex(0);
  pfr05->addChildren("-side left -expand t -fill x",labd5,drawMode);

  for (int i=0;i<=1;i++)
    {
      if (i==0)
	trimScale[i]=new PXTkArrowScale(this->EventManager,pfr2,"Bot Trim",502,PXTrue);
      else
	trimScale[i]=new PXTkArrowScale(this->EventManager,pfr2,"Top Trim",503,PXTrue);
      trimScale[i]->setRange(0,this->dimensions[2]-1);
      trimScale[i]->setIncrement(1);
      trimScale[i]->setResolution(1.0);
      trimScale[i]->setDecimalPoints(0);
      trimScale[i]->setLengthWidth(55,8);
      pfr2->addChildren("-side left -expand t -fill x",trimScale[i]->getMainWindow());
    }
  trimScale[0]->setValue(0);
  trimScale[1]->setValue(dimensions[2]-1);


  // Collection Operations 
  // ---------------------
  PXTkLabelFrame* oFrame=new PXTkLabelFrame(this->EventManager,this->controlFrame,"Operations on Current Stack");
  this->controlFrame->addChildren("-side top -expand t -fill both -padx 1 -pady 1",oFrame);


  PXTkFrame* topfr=new PXTkFrame(this->EventManager,oFrame);
  PXTkFrame* midfr=new PXTkFrame(this->EventManager,oFrame);
  PXTkFrame* midfr2=new PXTkFrame(this->EventManager,oFrame);
  PXTkFrame* midfr3=new PXTkFrame(this->EventManager,oFrame);

  PXTkFrame* botfr=new PXTkFrame(this->EventManager,oFrame);  
  PXTkFrame* botfr2=new PXTkFrame(this->EventManager,oFrame);  
  oFrame->addChildren("-side top -pady 1 -expand t -fill x",topfr,midfr,midfr2,midfr3);
  oFrame->addChildren("-side top -pady 1 -expand t -fill x",botfr,botfr2);
  

  PXTkButton* but3=new PXTkButton(this->EventManager,topfr,"x0.5",600);
  PXTkButton* but4=new PXTkButton(this->EventManager,topfr,"x0.8",601);
  PXTkButton* but4a=new PXTkButton(this->EventManager,topfr,"x1.25",602);
  PXTkButton* but4b=new PXTkButton(this->EventManager,topfr,"x2",603);
  PXTkButton* but4c=new PXTkButton(this->EventManager,topfr,"s2",604);
  PXTkButton* but4d=new PXTkButton(this->EventManager,topfr,"s5",605);
  PXTkButton* but4e=new PXTkButton(this->EventManager,topfr,"FlipX",606);

  but3->configure("-padx",2);
  but4->configure("-padx",2);
  but4a->configure("-padx",2);
  but4b->configure("-padx",2);
  but4c->configure("-padx",2);
  but4d->configure("-padx",2);
  topfr->addChildren("-side right ",but4e);
  topfr->addChildren("-side left ",but3,but4,but4a);
  topfr->addChildren("-side left ",but4b,but4c,but4d);

  

  PXTkButton* but6=new PXTkButton(this->EventManager,midfr,"Color",450);
  PXTkButton* but5=new PXTkButton(this->EventManager,midfr,"Cylinder",452);

  but5->configure("-padx",0);
  but6->configure("-padx",0);
  if (strcmp(this->GetClassName(),"vtkpxGUI4DSplineStackControl")!=0)
    midfr->addChildren("-side left -padx 1 -expand t -fill x",but6,but5);
  else
    midfr->addChildren("-side left -padx 1 -expand t -fill x",but6);

  PXTkButton* bbut5=new PXTkButton(this->EventManager,midfr2,"Volume",453);
  bbut5->configure("-padx",0);
  midfr2->addChildren("-side left -padx 1 -expand t -fill x",bbut5);

  PXTkButton* bbut6=new PXTkButton(this->EventManager,midfr3,"VOI Prop",454);
  bbut6->configure("-padx",0);
  for (int j=0;j<=1;j++)
    {
      thresholdEntry[j]=new PXTkEntry(this->EventManager,midfr3);
      thresholdEntry[j]->configure("-width",6);
      thresholdEntry[j]->setText("0.1");
    }
  thresholdEntry[1]->setText("50.0");
  midfr3->addChildren("-side left -padx 1 -expand t -fill x",bbut6,thresholdEntry[0],thresholdEntry[1]);

  if (strcmp(this->GetClassName(),"vtkpxGUI4DSplineStackControl")!=0)
    {
      PXTkButton* but1=new PXTkButton(this->EventManager,botfr,"Load",401);
      PXTkButton* but2=new PXTkButton(this->EventManager,botfr,"Save",402);
      
      but1->configure("-padx",0);
      but2->configure("-padx",0);
      botfr->addChildren("-side left -padx 1 -expand t -fill x",but1,but2);
    }

  PXTkButton* cp=new PXTkButton(this->EventManager,botfr2,"Copy",590);
  PXTkButton* pt=new PXTkButton(this->EventManager,botfr2,"Paste",591);
  cp->configure("-padx",0);
  pt->configure("-padx",0);
  botfr2->addChildren("-side left -padx 1 -expand t -fill x",cp,pt);



  if (!inside)
    {
      PXTkButton* closeB;
      if (this->EventParent)
	closeB=new PXTkButton(this->EventManager,this->controlFrame,"Close",3);
      else
	closeB=new PXTkButton(this->EventManager,this->controlFrame,"Exit",1);
      this->controlFrame->addChildren("-side bottom -expand false -fill x -padx 2",closeB);
    }

  if ( (this->EventParent!=NULL && this->CallbackNo>0) || ( this->CallbackName!=NULL ))
    {
      PXTkButton* updB=new PXTkButton(this->EventManager,this->controlFrame,"Update",614);
      this->controlFrame->addChildren("-side top -padx 2 -expand t -fill x",updB);
    }

  if (this->optionDialog==NULL)
    this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,10);


  // Other Stuff 
  return GetWidgetName();
}

/* -------------------------------------------------------------------------*/ 
void vtkpxGUISplineStackControl::SetRenderer(vtkRenderer* ren,int index)
{
  this->SetRenderer(ren,index,0);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::SetRenderer(vtkRenderer *ren,int index,int wireframe)
{
  index=Irange(index,0,SSP_MAX_RENDERERS-1);

  if (this->Renderer[index]!=NULL)
    return;

  for (int i=0;i<this->NumberOfSplineStacks;i++)
    {
      double c[3]; vtkpxSurfaceUtil::GetColor(i,c);
      if (splineStackActor[index][i]==NULL)
	{
	  vtkPolyDataMapper* map=vtkPolyDataMapper::New();
	  map->SetInput(splineStackSet[i]->GetOutput());
	  
	  splineStackActor[index][i]=vtkActor::New();
	  splineStackActor[index][i]->SetMapper(map);
	  if (display_mode[i]!=0)
	    splineStackActor[index][i]->SetVisibility(1);
	  else
	    splineStackActor[index][i]->SetVisibility(0);
	  
	  if (wireframe>0)
	    {
	      splineStackActor[index][i]->GetProperty()->SetRepresentationToWireframe();
	      splineStackActor[index][i]->GetProperty()->SetLineWidth(2.0);
	      splineStackActor[index][i]->GetProperty()->SetAmbient(1.0);
	      splineStackActor[index][i]->GetProperty()->SetDiffuse(0.0);
	      splineStackActor[index][i]->GetProperty()->SetSpecular(0.0);
	    } 
	  
	  
	  splineStackActor[index][i]->GetProperty()->SetColor(c);
	  map->Delete();
	}
      ren->AddActor(splineStackActor[index][i]);
    }
  
  this->Renderer[index]=ren;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::Update()
{
  if (this->splineStackActor[0]==NULL)
      return;

  for (int j=0;j<SSP_MAX_RENDERERS;j++)
    {
      if (this->Renderer[j]!=NULL)
	{
	  for (int i=0;i<this->NumberOfSplineStacks;i++)
	    {
	      if (display_mode[i])
		splineStackActor[j][i]->SetVisibility(1);
	      else
		splineStackActor[j][i]->SetVisibility(0);
	    }
	}
    }

  // Curve Stuff 
  UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::UpdateDisplay()
{
  if (this->Renderer[0]!=NULL)
    this->Renderer[0]->GetRenderWindow()->Render();

}

/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::UpdateAfterExternalEditing()
{
  this->UpdateControlsFromCurrentSplineStack();
}
/* -------------------------------------------------------------------------*/


int  vtkpxGUISplineStackControl::HandleEvent(int event)
{
  if (!this->Initialized)
      return TCL_OK;

  if (event<10)
      return this->EventManager->handleDirectEvent(event);

  if (this->currentSplineStack==NULL)
    return TCL_OK;


  int updateeditor=this->HandleEventSecond(event);

  if (this->BlockEditorCallback==0 && (updateeditor==1 || event== 700))
    {
      //      fprintf(stderr,"Calling Update Spline Editor %s\n",this->GetClassName());
      this->UpdateSplineEditor();
    }

  UpdateDisplay();

  return TCL_OK;

}
  

int  vtkpxGUISplineStackControl::HandleEventSecond(int event)
{
  int updateeditor=0;

  switch(event)
      {
      case 401:
	{
	  int ok=this->EventManager->questionBox("Loading will delete your old splinestack. Are you sure?");
	  if (ok)
	      {
		char* line=this->EventManager->getOpenFilename("Load splinestacks","SplineStacks","*.sur");
		if (line!=NULL)
		    {
		      if (strlen(line)>0)
			{
			  char newline[400]; strncpy(newline,line,400); 
			  //			  fprintf(stderr,"Loading from %s\n",newline);
			  LoadSplineStack(newline);
			}
		      updateeditor=1;
		    }
	      }
	}
	break;
	
      case 402:
	{
	  sprintf(pxtk_buffer,"%s",surface_filename[current_splinestack]);
	  //	  fprintf(stderr,"Old filename = %s\n",surface_filename[current_splinestack]);
	  char* line=this->EventManager->getSaveFilename("Save SplineStacks",pxtk_buffer,"SplineStacks","*.sur");
	  if (line!=NULL)
	      {
		if (strlen(line)>0)
		  {
		    char newline[400];
		    strncpy(newline,line,400);
		    SaveSplineStack(newline);
		  }
	      }
	}
	break;
  	
      case 450:
	if (splineStackActor[0][current_splinestack]!=NULL)
	  {
	    double c[3];
	    splineStackActor[0][current_splinestack]->GetProperty()->GetColor(c);
	    int a=this->EventManager->d_colorBox("SplineStack Color",c[0],c[1],c[2]);
	    if (a==1)
		this->SetItemColor(current_splinestack,c);
	  }
	break;
	

      case 452: // Ellipse
	currentSplineStack->SetEllipticalCylinder(8,
						  dimensions[2],
						  spacing[0]*float(dimensions[0])/(2.00*this->NumberOfSplineStacks)*(1.0+current_splinestack),
						  spacing[1]*float(dimensions[1])/(2.00*this->NumberOfSplineStacks)*(1.0+current_splinestack),
						  origin[0]+spacing[0]*dimensions[0]/2.0,
						  origin[1]+spacing[1]*dimensions[1]/2.0,
						  spacing[2],
						  origin[2]);
	surface_modified[current_splinestack]=0;
	currentSplineStack->SetDrawMode(0);
	updateeditor=1;
	this->UpdateAfterExternalEditing();
	sprintf(pxtk_buffer2,"CStack");
	this->SetItemFilename(current_splinestack,pxtk_buffer2);
	break;
	  
      case 453:
	this->WatchOn();
	sprintf(pxtk_buffer,"Stack %d Volume=%8.1f mm^3\n",current_splinestack+1,currentSplineStack->GetVolume());
	PXTkApp::printToConsole(pxtk_buffer);
	this->EventManager->messageBox(pxtk_buffer,"Stack Volume ...");
	this->WatchOff();
	break;

      case 454:
	this->ComputeVOIProperties(0);
	break;
	
      case 500:
	SetCurrentCollection(currentCollection->getIndex());
	break;
	
      case 501:
	display_mode[current_splinestack]=displayMode->getIndex();
	currentSplineStack->SetDrawMode(drawMode->getIndex());
	if (display_mode[current_splinestack]==1 && currentSplineStack->GetWholeSurface()==1)
	  currentSplineStack->SetWholeSurface(0);
	if (display_mode[current_splinestack]==2 && currentSplineStack->GetWholeSurface()==0)
	  currentSplineStack->SetWholeSurface(1);
	Update();
	break;

      case 502: // Trim Bottom
	currentSplineStack->SetTrimBottom((int)trimScale[0]->getValue());
	trimScale[0]->setValue(currentSplineStack->GetTrimBottom());
	surface_modified[current_splinestack]=1;
	Update();
	break;

      case 503:
	currentSplineStack->SetTrimTop((int)trimScale[1]->getValue());
	trimScale[1]->setValue(currentSplineStack->GetTrimTop());
	surface_modified[current_splinestack]=1;
	Update();
	break;

      case 505:
	{
	  for  (int i=0;i<this->NumberOfSplineStacks;i++)
	    display_mode[i]=0;
	  displayMode->setIndex(0);
	}
	Update();
	break;

	
      case 506:
	{
	  for  (int i=0;i<this->NumberOfSplineStacks;i++)
	    display_mode[i]=2;
	  displayMode->setIndex(2);
	}
	Update();
	break;

	
      case 600: // x0.5
	currentSplineStack->ChangeSpacing(this->defaultsmoothing,0.5);
	updateeditor=1;
	this->UpdateAfterExternalEditing();
	break;

      case 601: // x0.8
	currentSplineStack->ChangeSpacing(this->defaultsmoothing,0.8);
	updateeditor=1;
	this->UpdateAfterExternalEditing();
	break;

      case 602: // x1.25
	currentSplineStack->ChangeSpacing(this->defaultsmoothing,1.25);
	updateeditor=1;
	this->UpdateAfterExternalEditing();
	break;

      case 603: // x2.0
	currentSplineStack->ChangeSpacing(this->defaultsmoothing,2.0);
	updateeditor=1;
	this->UpdateAfterExternalEditing();
	break;

      case 604: // s1.0
	currentSplineStack->ChangeSpacing(this->defaultsmoothing,-1.0);
	updateeditor=1;
	this->UpdateAfterExternalEditing();
	break;

      case 605: // s5.0
	currentSplineStack->ChangeSpacing(2.0,-1.0);
	updateeditor=1;
	this->UpdateAfterExternalEditing();
	break;

      case 606:
	{
	  int ok=currentSplineStack->FlipX(this->image);
	  if (ok>0)
	    {
	      updateeditor=1;
	      this->UpdateAfterExternalEditing();
	    }
	}
	break;

      case 590:
	this->CopySplineStack();
	break;
	
      case 591:
	this->PasteSplineStack();
	surface_modified[current_splinestack]=1;
	updateeditor=1;
	break;


      case 614:
	if (this->EventParent!=NULL && this->CallbackNo>0)
	  this->EventParent->HandleEvent(CallbackNo);
	else
	  if (this->CallbackName!=NULL)
	    PXTkApp::executeTclCommand(this->CallbackName);
	break;

      case 701:
	if (this->BlockEditorCallback==0)
	  this->UpdateFromSplineEditor();
	break;
      }


  return updateeditor;

}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::SetItemColor(int index,double color[3])
{
  for (int j=0;j<SSP_MAX_RENDERERS;j++)
    {
      if (splineStackActor[j][index]!=NULL)
	splineStackActor[j][index]->GetProperty()->SetColor(color);

      this->SplineEditor->GetSplineControl()->SetItemColor(index,color);
    }

}
// -------------------------------------------------------------------------
void vtkpxGUISplineStackControl::SetItemFilename(int index,const char* s)
{
  //  fprintf(stderr,"Here SetFileName GUISPlineControl\n");
  delete surface_filename[index];
  surface_filename[index]=new char[strlen(s)+1];
  strcpy(surface_filename[index],s);

  if (this->Initialized==1)
    {
      strcpy(pxtk_buffer4,s);
      sprintf(pxtk_buffer4,"%s.%d",PXTkApp::getFilenameTail(pxtk_buffer4),index+1);
      currentCollection->changeLabel(index,pxtk_buffer4);
    }
  //  fprintf(stderr,"Setting Filename of %d = %s\n",index,s);
  this->SplineEditor->GetSplineControl()->SetItemFilename(index,surface_filename[index]);
}
// -------------------------------------------------------------------------
void vtkpxGUISplineStackControl::CopySplineStack()
{
  clipboardSplineStack->Copy(currentSplineStack);
}

void vtkpxGUISplineStackControl::PasteSplineStack()	
{
    SetSplineStack(clipboardSplineStack,-1);
}
/* -------------------------------------------------------------------------*/
vtkpxSplineStackSource* vtkpxGUISplineStackControl::GetSplineStack(int index)
{
  if (index==-1)
      index=current_splinestack;

  return splineStackSet[Irange(index,0,this->NumberOfSplineStacks-1)];
}
/* -------------------------------------------------------------------------*/
int vtkpxGUISplineStackControl::GetCurrentIndex()
{
  return current_splinestack;
}


/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::SetSplineStack(vtkpxSplineStackSource* land,int index)
{
  if (index==-1)
      index=current_splinestack;

  index=Irange(index,0,this->NumberOfSplineStacks-1);

  splineStackSet[index]->Copy(land);
  
  if (index==current_splinestack)
    this->UpdateAfterExternalEditing();
    
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::SetImage(vtkImageData* img,int orient,int lv,int fr)
{
  if (img!=NULL)
    {
      if (this->image!=NULL)
	this->image->Delete();
      this->image=vtkImageData::New();
      this->image->ShallowCopy(img);
      this->orientation=vtkpxGUIBaseImageViewer::MapOrientationToOldStyle(orient);
      
      int d[3]; double sp[3],ori[3];
      this->image->GetSpacing(sp);
      this->image->GetOrigin(ori);
      this->image->GetDimensions(d);
      
      float sum=0.0;
      for (int ia=0;ia<=2;ia++)
	sum+=
	  fabs(float(d[ia]-this->dimensions[ia]))+
	  fabs(sp[ia]-this->spacing[ia])+
	  fabs(ori[ia]-this->origin[ia]);
      
        if ( (splineStackSet[0]->GetPolarMode()==0 && this->orientation==3) || 
	     (splineStackSet[0]->GetPolarMode()==1 && this->orientation!=3))
       sum=1.0;



      if (sum>0.01)
	{
	  this->image->GetOrigin(this->origin);
	  this->image->GetSpacing(this->spacing);
	  this->image->GetDimensions(this->dimensions);
	  
	  for (int i=0;i<this->NumberOfSplineStacks;i++)
	    {
	      //	      fprintf(stderr,"New Image::Set Elliptical Cylinder %d spa=%.2f\n",i,this->spacing[2]);
	      if (this->orientation==3)
		splineStackSet[i]->SetPolarMode(1);
	      else
		splineStackSet[i]->SetPolarMode(0);
			      
	      surface_modified[i]=0;
	      splineStackSet[i]->SetEllipticalCylinder(8,
						       this->dimensions[2],
						       spacing[0]*dimensions[0]/8.0*(1.0+i),
						       spacing[1]*dimensions[1]/8.0*(1.0+i),
						       origin[0]+spacing[0]*dimensions[0]/2.0,
						       origin[1]+spacing[1]*dimensions[1]/2.0,
						       this->spacing[2],
						       this->origin[2]);
	      splineStackSet[i]->SetResolutionU(2.0*spacing[1]);
	      splineStackSet[i]->SetResolutionV(spacing[2]);
	    }
	  
	  trimScale[0]->setRange(0,this->dimensions[2]-1);
	  trimScale[1]->setRange(0,this->dimensions[2]-1);
	  trimScale[0]->setValue(0);
	  trimScale[1]->setValue(dimensions[2]-1);
	}

      this->BlockEditorCallback=1;
      if (this->SplineEditor!=NULL)
	{
	  this->SplineEditor->SetImage(this->image,this->orientation);
	  this->SplineEditor->SetSlice(-1);
	  this->UpdateSplineEditor();
	}
      this->BlockEditorCallback=0;
    }
}

/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::SaveSplineStack(const char* fname)
{
  char* noext1=nothisextension(fname,".sur");
  sprintf(pxtk_buffer2,"%s.sur",noext1);
  //  delete [] noext1;
  
  if(currentSplineStack->Save(pxtk_buffer2))
    {
      this->SetItemFilename(current_splinestack,pxtk_buffer2);
      surface_modified[current_splinestack]=0;
    }
  else
    {
      sprintf(pxtk_buffer2,"Failed to save splinestack %d in %s (Check Permissions)",current_splinestack+1,pxtk_buffer2);
      this->EventManager->messageBox(pxtk_buffer2,"Save Failure");
    }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::LoadSplineStack(const char* fname)
{
  strcpy(pxtk_buffer2,fname);

  int ok=currentSplineStack->Load(fname);
  if (!ok)
      {
	sprintf(pxtk_buffer2,"Failed to load splinestack %d from %s",current_splinestack+1,fname);
	this->EventManager->messageBox(pxtk_buffer2,"Load Failure");
      }
  else
    {
      //      fprintf(stderr,"Here %s\n",pxtk_buffer2);
      this->SetItemFilename(current_splinestack,pxtk_buffer2);
      //      fprintf(stderr,"Here %s\n",surface_filename[current_splinestack]);
      surface_modified[current_splinestack]=0;
      this->UpdateAfterExternalEditing();
    }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::SetCurrentCollection(int num)
{
  num=Irange(num,0,this->NumberOfSplineStacks-1);
  if (num==current_splinestack)
      return;
  current_splinestack=num;

  currentSplineStack=splineStackSet[current_splinestack];  
  this->UpdateAfterExternalEditing();
  
  Update();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::UpdateControlsFromCurrentSplineStack()
{
  displayMode->setIndex(display_mode[current_splinestack]);
  drawMode->setIndex(currentSplineStack->GetDrawMode());
  trimScale[0]->setValue(currentSplineStack->GetTrimBottom());
  trimScale[1]->setValue(currentSplineStack->GetTrimTop());
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::UpdateFromSplineEditor()
{
  if (this->SplineEditor==NULL)
    {
      vtkErrorMacro(<<"No Spline Editor\n");
      return;
    }
  int sl=this->SplineEditor->GetBeginSlice();
  int num=this->GetNumberOfSplineStacks();

  //  fprintf(stderr,"\tSTACK:: Receiving Update From Spline Editor slice=%d BLOCK=%d\n",sl,this->BlockEditorCallback);
  for (int i=0;i<num;i++)
    {
      if (this->SplineEditor->GetSplineControl()->GetChangedStatus(i)==1)
	{
	  vtkpxSplineStackSource* stack=this->GetSplineStack(i);
	  vtkpxSplineSource*spl=this->SplineEditor->GetSplineControl()->GetSpline(i);
	  fprintf(stderr,"Changing Curve %d on Surface %d\n",i+1,sl);
	  stack->SetSpline(spl,sl);
	  surface_modified[i]=1;
	  fprintf(stderr,"Updating spline %d\n",i+1);
	}
      else 
	{
	  fprintf(stderr,"Not Updating spline %d\n",i+1);
	}
    }
  this->UpdateDisplay();

}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::UpdateSplineEditor()
{

  if (this->SplineEditor==NULL)
    {
      vtkErrorMacro(<<"No Spline Editor\n");
      return;
    }

  int sl=this->SplineEditor->GetBeginSlice();
  int num=this->GetNumberOfSplineStacks();

 

  //  fprintf(stderr,"\tSTACK:: Sending Update To Spline Editor slice=%d BLOCK=%d\n",sl,this->BlockEditorCallback);

  for (int i=0;i<num;i++)
    {
      vtkpxSplineStackSource* stack=this->GetSplineStack(i);
      if (sl>=stack->GetTrimBottom() && sl<=stack->GetTrimTop())
	{
	  vtkpxSplineSource* spl=this->SplineEditor->GetSplineControl()->GetSpline(i);
	  stack->SetSplineFromSplineStack(spl,sl);
	  this->SplineEditor->EnableSpline(i);
	  this->SplineEditor->GetSplineControl()->SetChangedStatus(i,0);
	}
      else
	{
	  this->SplineEditor->DisableSpline(i);
	}
    }
  this->SplineEditor->GetSplineControl()->UpdateControlsFromCurrentSpline();
  this->SplineEditor->GetSplineControl()->Update();
  
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::ComputeVOIProperties(int frame)
{
  this->WatchOn();
  if (this->image!=NULL)
    {
      float volume[2],mean[2],std[2],threshold[2];
      int numvox[2];

      for (int ii=0;ii<2;ii++)
	{
	  threshold[ii]=atof(thresholdEntry[ii]->getText());
	  sprintf(pxtk_buffer,"%5.2f",threshold[ii]);
	  thresholdEntry[ii]->setText(pxtk_buffer);
	}
      
      int nt=currentSplineStack->GetDualVOIProperties(this->image,frame,
						      volume,mean,std,numvox,threshold);

      if (volume[0]<0.0001)
	volume[0]=0.0001;
	    
      sprintf(pxtk_buffer,"Stack %d Volume=%8.1f mm^3 numvox=%d\n",
	      current_splinestack+1,currentSplineStack->GetVolume(),nt);
      PXTkApp::printToConsole(pxtk_buffer);
      for (int i=0;i<=1;i++)
	{
	  sprintf(pxtk_buffer,"\t ROI %d vol >%.1f \t= %6.1f (mm^3) mean=%5.1f std=%5.1f npix=%6d\n",
		  i+1,threshold[i],volume[i],mean[i],std[i],numvox[i]);
	  PXTkApp::printToConsole(pxtk_buffer);
	}
      
      sprintf(pxtk_buffer,"\t Inside Volume  %.1f > t  >%.1f \t= %6.1f (cc) %d voxels (%.2f x %.2f %.2f mm)\n\n",
	      threshold[0],threshold[1],0.001*(volume[0]-volume[1]),numvox[0]-numvox[1],spacing[0],spacing[1],spacing[2]);
      PXTkApp::printToConsole(pxtk_buffer);
      PXTkApp::popupConsole();
    }
  this->WatchOff();
}

/* -------------------------------------------------------------------------*/
void vtkpxGUISplineStackControl::DoImport(int mode,int moption)
{
  fprintf(stderr,"Do Import this->Dim=%d,%d,%d mode=%d moption=%d\n",this->dimensions[0],this->dimensions[1],this->dimensions[2],mode,moption);

  if (mode==0)
    {
      if (this->optionDialog!=NULL)
	{
	  if (this->optionDialog->isOpen())
	    {
	      fprintf(stderr,"Dialog is open\n");
	      return;
	    }
	  
	  delete this->optionDialog;
	}
      fprintf(stderr,"This far\n");
      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,4);
      this->optionDialog->initDisplay();

      this->optionDialog->setTitleDescription("Import Surface From Binary Image",
					      "Enter Smoothing and Slice Range Parameters");
      this->optionDialog->setfoption(0,"Surface Smoothness","%.1f",1.0);
      this->optionDialog->setfoption(1,"Image   Smoothness","%.1f",1.0);
      this->optionDialog->setioption(2,"Bottom  Slice","%d",0);
      this->optionDialog->setioption(3,"Top     Slice","%d",this->dimensions[2]-1) ;
      this->optionDialog->setMenuOption(moption);
      this->optionDialog->popupDialog();
      return;
    }

  fprintf(stderr,"On to extract ...");
  float smooth=Frange(this->optionDialog->getfoption(0),0.1,10.0);
  float sigma=Frange(this->optionDialog->getfoption(1),0.1,4.0);
  int   minslice=Irange(this->optionDialog->getioption(2),0,this->dimensions[2]-2);
  int   maxslice=Irange(this->optionDialog->getioption(3),minslice,this->dimensions[2]-1);
  
  currentSplineStack->CreateSurfaceFromImage(this->image,minslice,maxslice,
					     this->CurrentFrame,smooth,sigma);
  this->UpdateAfterExternalEditing();
}

