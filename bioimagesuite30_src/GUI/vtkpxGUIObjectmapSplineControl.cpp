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





// 	$Id: vtkpxGUIObjectmapSplineControl.cpp,v 1.2 2003/09/11 13:34:39 xenios Exp xenios $	

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkpxGUIObjectmapSplineControl.h"
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
vtkpxGUIObjectmapSplineControl* vtkpxGUIObjectmapSplineControl::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIObjectmapSplineControl");
  if(ret)
      {
	return (vtkpxGUIObjectmapSplineControl*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIObjectmapSplineControl;
}

// ----------------------------------------------------------------------------
vtkpxGUIObjectmapSplineControl::vtkpxGUIObjectmapSplineControl()
{
  this->NumberOfSplines=50;
  this->ObjectmapEditor=NULL;

  for (int ia=0;ia<=2;ia++)
    {
      this->origin[ia]=0.0;
      this->spacing[ia]=1.0;
      this->dimensions[ia]=100;
    }
  
  for (int i=0;i<this->NumberOfSplines;i++)
    {
      splineSet[i]=NULL;
      splineActor[i]=NULL;
      if (i==0)
	display_mode[i]=PXTrue;
      else
	display_mode[i]=PXFalse;
      scale_mode[i]=4;
      spline_enabled[i]=1;
      this->changed[i]=0;
    }

  editCurve=NULL;
  manualEditCurve=NULL;

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

}
// ----------------------------------------------------------------------------

vtkpxGUIObjectmapSplineControl::~vtkpxGUIObjectmapSplineControl()
{
  
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
    }
    
  if (this->image!=NULL)
    this->image->Delete();


}
// ----------------------------------------------------------------------------
void vtkpxGUIObjectmapSplineControl::SetObjectmapEditor(vtkpxGUIObjectmapEditor* ed)
{
  if (this->Initialized==0 && ed!=NULL)
    this->ObjectmapEditor=ed;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineControl::SetNumberOfSplines(int a)
{
  if (this->Initialized==1)
    {
      return;
    }
  
  this->NumberOfSplines=Irange(a,1,SP_MAX_COLLECTIONS);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineControl::InitializeSplines()
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



}
/* -------------------------------------------------------------------------*/
char* vtkpxGUIObjectmapSplineControl::Initialize(const char* name,int inside)
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
  mouseMode=new PXTkCheckButton(this->EventManager,landmarkFrame,"Edit Mode",-1);
  mouseMode->setState(PXTrue);
  editMode=new PXTkCheckButton(this->EventManager,landmarkFrame,"Manual Mode",510);
  editMode->setState(PXFalse);
  showControls=new PXTkCheckButton(this->EventManager,landmarkFrame,"Controls",502);
  showControls->setState(PXTrue);
  PXTkButton* b0=new PXTkButton(this->EventManager,landmarkFrame,"(Sag -)",504);
  b0->configure("-padx",2);
  landmarkFrame->addChildren("-side top -expand false -fill x",mouseMode,editMode,showControls,b0);

  message=new PXTkLabel(this->EventManager,landmarkFrame,"Num");
  message->configure("-relief","ridge");
  displayMode=new PXTkCheckButton(this->EventManager,landmarkFrame,"Display",501);
  displayMode->setState(display_mode[current_spline]);

  PXTkFrame* fr1=new PXTkFrame(this->EventManager,landmarkFrame);
  landmarkFrame->addChildren("-side top -expand false -fill x",message,displayMode,fr1);

  PXTkLabel* lab0=new PXTkLabel(this->EventManager,fr1,"Size:");
  scaleMode=new PXTkOptionMenu(this->EventManager,fr1,"0.25",503);
  for (float s=0.50;s<=4.5;s+=0.25)
      {
	sprintf(pxtk_buffer,"%3.2f",s);
	scaleMode->addOption(pxtk_buffer);
      }
  scaleMode->setIndex(scale_mode[current_spline]);
  fr1->addChildren("-side left -expand true -fill x",lab0,scaleMode);

  // Collection Operations 
  // ---------------------
  PXTkFrame* oFrame=new PXTkFrame(this->EventManager,landmarkFrame);
  landmarkFrame->addChildren("-side top -expand false -fill x -padx 1 -pady 1",oFrame);

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
  topfr->addChildren("-side left ",but3,but4,but4a);
  
  SmoothingEntry=new PXTkEntry(this->EventManager,midfr);
  SmoothingEntry->configure("-width",6);
  SmoothingEntry->configure("-bg","red");
  char line[10]; sprintf(line,"%f",this->DefaultSmoothing); SmoothingEntry->setText(line);
  PXTkButton* but4c=new PXTkButton(this->EventManager,midfr,"sm",604);
  midfr->addChildren("-side left -expand true -fill x ",SmoothingEntry,but4c);
  
  
  PXTkButton* but6=new PXTkButton(this->EventManager,midfr2,"Color",450);
  PXTkButton* but7=new PXTkButton(this->EventManager,midfr2,"C-Color",451);
  but6->configure("-padx",0);   but7->configure("-padx",0);
  midfr2->addChildren("-side left -padx 0 -expand t -fill x",but6,but7);

  PXTkButton* but5=new PXTkButton(this->EventManager,midfr3,"Ellipse",452);
  PXTkButton* bbut5=new PXTkButton(this->EventManager,midfr3,"Area",453);
  but5->configure("-padx",0);  bbut5->configure("-padx",0);
  midfr3->addChildren("-side left -padx 0 -expand t -fill x",but5,bbut5);

  PXTkButton* but9=new PXTkButton(this->EventManager,botfr,"Snake",610);
  PXTkButton* but10=new PXTkButton(this->EventManager,botfr,"Upd Snake",612);
  but9->configure("-padx",0);
  but10->configure("-padx",0);
  botfr->addChildren("-side left -padx 0 -expand t -fill x",but9,but10);

  bottomButtonFrame=new PXTkFrame(this->EventManager,landmarkFrame);  
  landmarkFrame->addChildren("-side bottom -expand false -fill x -padx 00",bottomButtonFrame);


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
void vtkpxGUIObjectmapSplineControl::SetRenderer(vtkRenderer *ren,int num)
{
  this->SetRenderer(ren);
}

void vtkpxGUIObjectmapSplineControl::SetRenderer(vtkRenderer* ren)
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
void vtkpxGUIObjectmapSplineControl::Update()
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
      }

  editActor->SetVisibility(splineActor[current_spline]->GetVisibility());

  // Curve Stuff 
  UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineControl::UpdateDisplay()
{
  if (this->Renderer!=NULL)
    this->Renderer->GetRenderWindow()->Render();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineControl::UpdateStatus()
{
  if (!this->Initialized)
      return;

  if (spline_enabled[current_spline]==1)
    sprintf(pxtk_buffer4,"Points=%d",
	    currentSpline->GetNumPoints());
  else
    sprintf(pxtk_buffer4,"Disabled!");
  message->setText(pxtk_buffer4);
}
/* -------------------------------------------------------------------------*/
int  vtkpxGUIObjectmapSplineControl::HandleEvent(int event)
{
  if (!this->Initialized)
    return TCL_OK;
  
  if (event<10)
    return this->EventManager->handleDirectEvent(event);

  if (event==500)
    {
      //      SetCurrentCollection(0);//currentCollection->getIndex());
      if (mouseMode->getState())
	{
	  if (currentSpline->GetNumPoints()==0)
	    mouseMode->setState(PXFalse);
	}
    }
  else if (event==614)
    {
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
	case 450:
	  if (splineActor[current_spline]!=NULL)
	    {
	      double c[3];
	      splineActor[current_spline]->GetProperty()->GetColor(c);
	      int a=this->EventManager->d_colorBox("Spline Color",c[0],c[1],c[2]);
	      if (a==1)
		splineActor[current_spline]->GetProperty()->SetColor(c);
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
	
	  
	case 510:
	  if (editMode->getState()==PXFalse)
	    {
	      if (this->manual_add_mode==1)
		this->FitSplineToCurrentControls();
	    }
	  else
	    {
	      this->manual_add_mode=1;
	      splineSet[current_spline]->ExportToBaseCurve(this->manualEditCurve);
	      this->manualEditCurve->SetPointScale(editCurve->GetPointScale()*0.5);
	      this->manualEditCurve->DeleteAllPoints();
	      this->manualEditActor->SetVisibility(1);
	    }
	  break;

	case 600: // x0.8
	  {
	    float s=this->currentSpline->GetSpline()->getlength(0.0,1.0,0.05)/100.0;
	    currentSpline->ChangeSpacing(this->DefaultSmoothing*s,(80*currentSpline->GetNumPoints())/100);
	    UpdateControlsFromCurrentSpline();
	  }
	  break;
	  
	case 601: // x1.0
	  {
	    float s=this->currentSpline->GetSpline()->getlength(0.0,1.0,0.05)/100.0;
	    currentSpline->ChangeSpacing(this->DefaultSmoothing*s,currentSpline->GetNumPoints());
	    UpdateControlsFromCurrentSpline();
	  }
	  break;
	  
	case 602: // x1.25
	  {
	    float s=this->currentSpline->GetSpline()->getlength(0.0,1.0,0.05)/100.0;
	    currentSpline->ChangeSpacing(this->DefaultSmoothing*s,(125*currentSpline->GetNumPoints())/100);
	    UpdateControlsFromCurrentSpline();
	  }
	  break;
	  
	  
	case 604: // sD
	  {
	    float s=this->currentSpline->GetSpline()->getlength(0.0,1.0,0.05)/100.0;
	    float t=Frange(atof(SmoothingEntry->getText()),0.0001,10.0);
	    char line[10]; sprintf(line,"%f",t); SmoothingEntry->setText(line);
	    currentSpline->ChangeSpacing(t*s,-1);
	    UpdateControlsFromCurrentSpline();
	  }
	break;
	
	  
	case 610:
	  DoSnake(0);
	  break;
	  
	case 611:
	  DoSnake(1);
	  break;
	  
	case 612:
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
void vtkpxGUIObjectmapSplineControl::SetItemColor(int index,double color[3])
{
  if (splineActor[index]!=NULL)
    splineActor[index]->GetProperty()->SetColor(color);

}

void vtkpxGUIObjectmapSplineControl::SetItemFilename(int index,const char* s)
{
  if (this->Initialized==0)
    return;

  //  fprintf(stderr,"index=%d s=%s\n",index,s);

  if (index==-1)
    {
      sprintf(pxtk_buffer2,"%s.%d",PXTkApp::getFilenameTail(s),current_spline+1);
      currentCollection->changeLabel(current_spline,pxtk_buffer2);
      return;
    }

  sprintf(pxtk_buffer2,"%s.%d",PXTkApp::getFilenameTail(s),index+1);
  currentCollection->changeLabel(index,pxtk_buffer2);

}

/* -------------------------------------------------------------------------*/
vtkpxSplineSource* vtkpxGUIObjectmapSplineControl::GetSpline(int index)
{
  if (index==-1)
      index=current_spline;

  return splineSet[Irange(index,0,this->NumberOfSplines-1)];
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIObjectmapSplineControl::GetCurrentIndex()
{
  return current_spline;
}

int  vtkpxGUIObjectmapSplineControl::GetCurrentSliceNumber()
{
  return image_level;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineControl::SetSpline(vtkpxSplineSource* land,int index)
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
void vtkpxGUIObjectmapSplineControl::SetChangedStatus(int index,int val)
{
  index=Irange(index,0,this->NumberOfSplines-1);
  this->changed[index]=(val>0);
  //  fprintf(stderr,"Changed[%d]=%d\n",index+1,this->changed[index]);
}

int  vtkpxGUIObjectmapSplineControl::GetChangedStatus(int index)
{
  index=Irange(index,0,this->NumberOfSplines-1);
  return this->changed[index];
}

/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineControl::SetImage(vtkImageData* img,int plane,int level,int frame)
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

void vtkpxGUIObjectmapSplineControl::SetPlaneLevel(int plane,int level)
{
  this->SetLevel(level);
}


void vtkpxGUIObjectmapSplineControl::SetSpacingOrigin(double sp[3],double ori[3])
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

void vtkpxGUIObjectmapSplineControl::SetLevel(int level)
{
  image_level=level;
  double zlev=level*this->spacing[2]+this->origin[2];
  this->manual_zcoord=zlev+slice_offset;
  for (int i=0;i<this->NumberOfSplines;i++)
    {
      splineSet[i]->SetZlevel(zlev+slice_offset);
    }
  UpdateControlsFromCurrentSpline();
}

/* -------------------------------------------------------------------------*/
int vtkpxGUIObjectmapSplineControl::HandleClickedPoint(float x,float y,float z,int scaled,int state)
{
  static int found_point=-1;
  if (!mouseMode->getState())
    return 0;


  if (spline_enabled[current_spline]==0)
    return 0;

  fprintf(stderr,"add_mode=%d \n",this->manual_add_mode);


  if (this->manual_add_mode==1)
    {
      switch (state)
	{
	case 0:
	  this->manualEditCurve->AddPoint(x,y,manual_zcoord,scaled);
	  fprintf(stderr,"Adding Point %.2f %.2f\n",x,y);
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
void vtkpxGUIObjectmapSplineControl::UpdateControlsFromCurrentSpline()
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
int vtkpxGUIObjectmapSplineControl::DoSnake(int apply)
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
      
      currentSpline->CreateSnake(image,image_level,image_frame,
				 w_img,w_smo,stp,sigma,iter);
      this->changed[current_spline]=1;
    }

  if (apply==1 || apply==2)
    {
      if (mouseMode->getState()==PXFalse)
	mouseMode->setState(PXTrue);
      float w_img=snakeEdgeWeight->getValue()*10.0;
      float w_smo=snakeSmoothWeight->getValue()*0.1;

      currentSpline->UpdateSnake(w_img,w_smo);      
      this->changed[current_spline]=1;
      UpdateControlsFromCurrentSpline();
    }

  return 1;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineControl::EnableSpline(int i)
{
  i=Irange(i,0,this->NumberOfSplines-1);
  spline_enabled[i]=1;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineControl::DisableSpline(int i)
{
  i=Irange(i,0,this->NumberOfSplines-1);
  spline_enabled[i]=1; // Fix this some time
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineControl::FitSplineToCurrentControls()
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
      return;
    }

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

