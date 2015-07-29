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





// 	$Id: vtkpxGUIObjectmapSplineStackControl.cpp,v 1.2 2003/09/12 12:59:32 xenios Exp xenios $	

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkpxGUIObjectmapSplineStackControl.h"
#include "vtkpxUtil.h"
#include "vtkActor.h"
#include "vtkAssembly.h"
#include "vtkRenderer.h"
#include "vtkObjectFactory.h"
#include "vtkpxImageExtract.h"
#include "vtkPolyDataMapper.h"
#include "vtkImageData.h"
#include "vtkPolyDataWriter.h"
#include "vtkpxTriangulatedStackSource.h"
#include "pxchamferdistancemap.h"
#include "pxfpbspline.h"
#include "pxcontour.h"
#include "vtkpxGUIBaseImageViewer.h"
#include "vtkpxSurfaceUtil.h"
/* -------------------------------------------------------------------------*/
vtkpxGUIObjectmapSplineStackControl* vtkpxGUIObjectmapSplineStackControl::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIObjectmapSplineStackControl");
  if(ret)
      {
	return (vtkpxGUIObjectmapSplineStackControl*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIObjectmapSplineStackControl;
}

// ----------------------------------------------------------------------------
vtkpxGUIObjectmapSplineStackControl::vtkpxGUIObjectmapSplineStackControl()
{
  this->do_autosave=PXTrue;
  this->do_backup=PXTrue;
  this->dimensions[0]=-1;
  this->SplineEditor=NULL;
  this->sliceScale=NULL;
  //  this->frameScale=NULL;
  this->BaseViewer=NULL;

  surface_setfilename=new char[30];
  strcpy(surface_setfilename,"unnamed_set.multisur");

}
// ----------------------------------------------------------------------------
vtkpxGUIObjectmapSplineStackControl::~vtkpxGUIObjectmapSplineStackControl()
{
  this->SetBaseViewer(NULL);
  delete [] surface_setfilename;
}
// ----------------------------------------------------------------------------
void vtkpxGUIObjectmapSplineStackControl::InitializeSplineStacks()
{
  vtkpxGUISplineStackControl::InitializeSplineStacks();
}
// ----------------------------------------------------------------------------
void vtkpxGUIObjectmapSplineStackControl::ResetSplineStacks()
{
  int d[3]; double sp[3],ori[3];
  for (int ib=0;ib<=2;ib++)
    {
      d[ib]=this->dimensions[ib];
      sp[ib]=this->spacing[ib];
      ori[ib]=this->origin[ib];
    }
  
  this->image->GetSpacing(this->spacing);
  this->image->GetOrigin(this->origin);
  this->image->GetDimensions(this->dimensions);

  
  float sum=0.0;
  for (int ia=0;ia<=2;ia++)
    sum+=
      fabs(float(d[ia]-this->dimensions[ia]))+
      fabs(sp[ia]-this->spacing[ia])+
      fabs(ori[ia]-this->origin[ia]);

  vtkpxSplineStackSource *st=splineStackSet[0];

  if ( (st->GetPolarMode()==0 && this->orientation==3) || (st->GetPolarMode()==1 && this->orientation!=3))
       sum=1.0;

  if (sum>0.01)
    {
      float dz=spacing[2];
      float factor=1.0;
      if (this->orientation==3)
	dz=180.0/float(this->dimensions[2]);

      for (int i=0;i<this->NumberOfSplineStacks;i++)
	{
	  vtkpxSplineStackSource *st=splineStackSet[i];
	  surface_modified[i]=0;
	  //	  fprintf(stderr,"Creating Surface dz=%.2f\n",dz);


	  st->SetEllipticalCylinder(8,
				    this->dimensions[2],
				    factor*spacing[0]*dimensions[0]/(2.0*this->NumberOfSplineStacks)*(1.0+i),
				    factor*spacing[1]*dimensions[1]/(2.0*this->NumberOfSplineStacks)*(1.0+i),
				    origin[0]+spacing[0]*dimensions[0]/2.0,
				    origin[1]+spacing[1]*dimensions[1]/2.0,
				    dz,
				    this->origin[2]);
	  st->SetDrawMode(0);
	  
	  if (this->orientation==3)
	    {
	      st->SetPolarMode(1);
	      st->SetPolarMiddle(spacing[0]*dimensions[0]*0.5+origin[0]);
	    }
	  else
	    {
	      st->SetPolarMode(0);
	      if (dimensions[2]>12)
		{
		  st->SetTrimBottom(dimensions[2]/2-5);
		  st->SetTrimTop(dimensions[2]/2+5);
		}
	      else
		{
		  st->SetTrimTop(dimensions[2]-1);
		  st->SetTrimBottom(0);
		}
	    }
	  
	  st->SetResolutionU(2.0*spacing[1]);
	  st->SetResolutionV(spacing[2]);
	}
    }
  
  // Get Current Spline Stack then set trims
  trimScale[0]->setRange(0,this->dimensions[2]-1);
  trimScale[1]->setRange(0,this->dimensions[2]-1);
  trimScale[0]->setValue(this->currentSplineStack->GetTrimBottom());
  trimScale[1]->setValue(this->currentSplineStack->GetTrimTop());
  this->UpdateSplineEditor();
}

// -------------------------------------------------------------------------

char* vtkpxGUIObjectmapSplineStackControl::Initialize(const char* name,int inside)
{
  return this->Initialize(name,name,name,inside);
}

char* vtkpxGUIObjectmapSplineStackControl::Initialize(const char* name,const char* name2,const char* name3,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();

  vtkpxGUISplineStackControl::Initialize(name,inside);

  PXTkLabelFrame* mFrame=NULL;
  if (strcmp(name,name2)==0)
    {
      mFrame=new PXTkLabelFrame(this->EventManager,this->controlFrame,"Surface");
      this->controlFrame->addChildren("-side top -expand false -fill x -padx 1 -pady 2",mFrame);
    }
  else
    {
      PXTkFrame* m=new PXTkFrame(NULL,name2);
      mFrame=new PXTkLabelFrame(this->EventManager,m,"Additional Operations");
      m->addChildren("-side top -expand false -fill x -padx 1 -pady 2",mFrame);
    }

  masterFrame=NULL;
  if (strcmp(name,name3)==0)
    {
      masterFrame=new PXTkFrame(this->EventManager,this->controlFrame);
      this->controlFrame->addChildren("-side top -expand false -fill x -padx 1 -pady 2",masterFrame);
    }
  else
    {
      PXTkFrame* m=new PXTkFrame(NULL,name3);
      masterFrame=new PXTkFrame(this->EventManager,m);
      m->addChildren("-side top -expand true -fill both -padx 1 -pady 2",masterFrame);
    }

  mFrame->setPackMode(PXTrue);
  masterFrame->setPackMode(PXTrue);


  PXTkFrame* f1=new PXTkFrame(this->EventManager,mFrame);
  PXTkFrame* f2=new PXTkFrame(this->EventManager,mFrame);
  PXTkFrame* f3=new PXTkFrame(this->EventManager,mFrame);
  PXTkFrame* f4=new PXTkFrame(this->EventManager,mFrame);
  mFrame->addChildren("-side top -expand false -fill x -pady 2 ",f1,f2,f3,f4);

  this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,10);
 
  PXTkLabel* labs=new PXTkLabel(this->EventManager,f1,"Current Stack:");
  currentCollection2=new PXTkOptionMenu(this->EventManager,f1,"Stack 1",800);
  for (int ip=1;ip<this->NumberOfSplineStacks;ip++)
    {
      sprintf(pxtk_buffer4,"Stack %d",ip+1);
      currentCollection2->addOption(pxtk_buffer4);
    }
  currentCollection2->setIndex(current_splinestack);
  f1->addChildren("-side left -expand t -fill x -pady 1",labs,currentCollection2);
    

  PXTkButton* but1a=new PXTkButton(this->EventManager,f3,"Load Set",702);
  PXTkButton* but2a=new PXTkButton(this->EventManager,f3,"Save Set",703);
  f3->addChildren("-side top -expand t -fill x -pady 1",but1a,but2a);


  autosavebutton=new PXTkCheckButton(this->EventManager,f3,"Autosave",803);
  autosavebutton->setState(do_autosave);
  backupbutton=new PXTkCheckButton(this->EventManager,f3,"Backup",803);
  backupbutton->setState(do_backup);
  f3->addChildren("-side top -expand t -fill x -pady 1",autosavebutton,backupbutton);

  f4->setPackMode(PXTrue);

  PXTkButton* but1=new PXTkButton(this->EventManager,f4,"Shift And Scale",810);
  PXTkButton* but4=new PXTkButton(this->EventManager,f4,"Cylinders",813);
  PXTkButton* but5=new PXTkButton(this->EventManager,f4,"Export",815);
  PXTkButton* but7=new PXTkButton(this->EventManager,f4,"Extract From (Binary) Image",816);
  f4->addChildren("-side top -expand t -fill x -pady 1",but1,but4,but5,but7);


  // Master Frame Stuff (i.e. frame and scale)

  sliceScale=new PXTkArrowScale(this->EventManager,masterFrame,"Slice",804,PXFalse);
  //  frameScale=new PXTkArrowScale(this->EventManager,masterFrame,"Fr",805,PXFalse);
  PXTkButton* but9=new PXTkButton(this->EventManager,masterFrame,"Objectmap/Spline Editor",806);
  //  frameScale->setRange(1,1);

  sliceScale->setValue(float(0.0));
  sliceScale->setLengthWidth(200,10);
  sliceScale->setIncrement(1.0);
  sliceScale->setResolution(1.0);

  /*  frameScale->setValue(float(0.0));
  frameScale->setLengthWidth(200,10);
  frameScale->setIncrement(1.0);
  frameScale->setResolution(1.0);*/

  masterFrame->addChildren("-side right -expand false -padx 5",but9);
  masterFrame->addChildren("-side left -expand false -fill x",sliceScale->getMainWindow());
  //,frameScale->getMainWindow());


  return GetWidgetName();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineStackControl::SetItemColor(int index,double color[3])
{
  for (int j=0;j<SSP_MAX_RENDERERS;j++)
    {
      if (splineStackActor[j][index]!=NULL)
	splineStackActor[j][index]->GetProperty()->SetColor(color);

      this->SplineEditor->GetSplineControl()->SetItemColor(index,color);
    }

}

// -----------------------------------------------------------------------------------------------
void vtkpxGUIObjectmapSplineStackControl::SetItemFilename(int i,const char* s)
{
  //  fprintf(stderr,"Here SetFileName GUIObjectmapSPlineControl\n");

  delete surface_filename[i];
  surface_filename[i]=new char[strlen(s)+1];
  strcpy(surface_filename[i],s);

  if (this->Initialized==1)
    {
      strcpy(pxtk_buffer4,s);
      sprintf(pxtk_buffer4,"%s.%d",PXTkApp::getFilenameTail(pxtk_buffer4),i+1);
      currentCollection->changeLabel(i,pxtk_buffer4);
    }
  //  fprintf(stderr,"On to SPline Control\n");
  //  fprintf(stderr,"Setting Filename of %d = %s\n",i,s);
  this->SplineEditor->GetSplineControl()->SetItemFilename(i,s);
}

/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineStackControl::Update()
{
  vtkpxGUISplineStackControl::Update();
  UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineStackControl::UpdateDisplay()
{
  vtkpxGUISplineStackControl::UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
int  vtkpxGUIObjectmapSplineStackControl::HandleEvent(int event)
{
  if (!this->Initialized)
      return TCL_OK;

  if (event<10)
      return this->EventManager->handleDirectEvent(event);

  int updateeditor=0;  

  if (event<800 && event!=700 && event!=701)
    {
      updateeditor=this->HandleEventSecond(event);
    }




  switch (event)
    {
    case 700:
      if (this->SplineEditor->GetSplineControl()->GetAutoUpdateMode()==1)
	this->UpdateFromSplineEditor();
      this->UpdateSplineEditor();
      this->UpdateMasterFrame();
      break;

    case 701:
      //      fprintf(stderr,"\n Explicit Update from Editor\n");
      this->UpdateFromSplineEditor();
      this->UpdateMasterFrame();
      break;

    case 702:
      {
	int ok=this->EventManager->questionBox("Loading will delete ALL your surfaces. Are you sure?");
	if (ok)
	  {
	    char* line=this->EventManager->getOpenFilename("Load splinestack set","SplineStack Sets","*.multisur");
	    if (line!=NULL)
	      {
		if (strlen(line)>0)
		  {
		    char newline[400]; strcpy(newline,line);  LoadSet(newline);
		  }
		updateeditor=1;
	      }
	  }
      }
      break;
      
    case 703:
      {
	sprintf(pxtk_buffer,"%s",surface_setfilename);
	//	fprintf(stderr,"Old filename = %s\n",surface_setfilename);
	char* line=this->EventManager->getSaveFilename("Save Spline Stack Set",pxtk_buffer,"SplineStack Sets","*.multisur");
	if (line!=NULL)
	  {
	    if (strlen(line)>0)
	      {
		strcpy(pxtk_buffer,line);
		char* noext1=nothisextension(pxtk_buffer,".multisur");
		char newname[400];
		sprintf(newname,"%s.multisur",noext1);
		//		delete [] noext1;
		this->SaveSet(newname);
	      }
	  }
      }
      break;
  
    case 800:
      SetCurrentCollection(currentCollection2->getIndex());
      break;


    case 804: // Update from slice/frame scales
      this->UpdateFromMasterFrame(0);
      break;

    case 805: // Update from slice/frame scales
      this->UpdateFromMasterFrame(1);
      break;
      
    case 806: // Popup spline editor;
      if (this->SplineEditor!=NULL)
	this->SplineEditor->Show();
      break;
      

    case 803:
      do_autosave=autosavebutton->getState();
      do_backup=backupbutton->getState();
      break;

    case 810:
      this->DoShiftScale(0,850);
      break;
    case 811:
      this->DoResample(0,852);
      break;

    case 813:
      this->DoCylinders(0,856);
      break;

    case 815:
      this->DoExport(0,860);
      break;

    case 816:
      this->DoImport(0,864);
      break;


    case 860:
      this->DoExport(1);
      updateeditor=1;
      break;

    case 850:
      this->DoShiftScale(1);
      updateeditor=1;
      break;
    case 852:
      this->DoResample(1);
      updateeditor=1;
      break;

    case 856:
      this->DoCylinders(1);
      updateeditor=1;
      break;

    case 864:
      this->DoImport(1,864);
      updateeditor=1;
      break;

    }
  
  if (updateeditor==1 && this->BlockEditorCallback==0)
    this->UpdateSplineEditor();


  this->UpdateDisplay();

  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineStackControl::SetImage(vtkImageData* img,int orient,int level,int frame)
{
  this->BlockEditorCallback=1;

  this->orientation=vtkpxGUIBaseImageViewer::MapOrientationToOldStyle(orient);
  if (img!=NULL)  
    this->SetImage(img);
  this->BlockEditorCallback=0;
}


void vtkpxGUIObjectmapSplineStackControl::SetCurrentCollection(int num)
{
  if (!this->Initialized)
    return;
  vtkpxGUISplineStackControl::SetCurrentCollection(num);
  currentCollection2->setIndex(current_splinestack);
  currentCollection->setIndex(current_splinestack);
}

void vtkpxGUIObjectmapSplineStackControl::SetImage(vtkImageData* img)
{
  if (img==NULL)
    return;

  if (this->image!=NULL)
    this->image->Delete();
  this->image=vtkImageData::New();
  this->image->ShallowCopy(img);      
  
  if (this->SplineEditor!=NULL)
    {
      this->SplineEditor->SetImage(this->image,this->orientation);
      this->SplineEditor->ShowFrameControls(0);
      this->SplineEditor->SetSlice(-1);
    }

  /*  int nc=this->image->GetNumberOfScalarComponents();
  if (nc==1)
    {
      frameScale->getMainWindow()->enable(PXFalse);
    }
  else
    {
      frameScale->getMainWindow()->enable(PXTrue);
      frameScale->setRange(1,nc);
      frameScale->setValue(1);
      masterFrame->addChildren("-side left -expand false -fill x",frameScale->getMainWindow());
      }*/

  int numsl=this->image->GetDimensions()[2];
  sliceScale->setRange(0,numsl-1);
  sliceScale->setValue(this->SplineEditor->GetSliceNumber());

  this->ResetSplineStacks();


  /*  for (int ia=0;ia<=1;ia++)
    {
      sliceScale[ia]->setRange(0,this->image->GetDimensions()[2]-1);
      sliceScale[ia]->setValue(ia*(this->image->GetDimensions()[2]-1));
      }*/
}

/* -------------------------------------------------------------------------*/
int vtkpxGUIObjectmapSplineStackControl::DoBackup(int stackindex,const char* rootname)
{
  stackindex=Irange(stackindex,0,this->NumberOfSplineStacks-1);
  sprintf(pxtk_buffer,"%s.sur",rootname);
  PXTkApp::fileOperation("exists",pxtk_buffer,NULL);
  int ok=PXTkApp::getTclIntResult();
  if (ok==1)
    {
      sprintf(pxtk_buffer,"%s.sur",rootname);
      sprintf(pxtk_buffer2,"%s_back.sur",rootname);
      sprintf(pxtk_buffer3,"File %s exists copying to %s\n",pxtk_buffer,pxtk_buffer2);
      PXTkApp::printToConsole(pxtk_buffer3);
      PXTkApp::copyFile(pxtk_buffer,pxtk_buffer2);
    }

  return 1;
}

/* -------------------------------------------------------------------------*/
int vtkpxGUIObjectmapSplineStackControl::DoAutosave()
{
  for (int i=0;i<this->NumberOfSplineStacks;i++)
    {
      if ( surface_modified[i] == 1 ) 
	{
	  vtkpxSplineStackSource *stack=this->GetSplineStack(i);
	  sprintf(pxtk_buffer,"%s_auto.sur",surface_filename[i]);
	  stack->Save(pxtk_buffer);
	  surface_modified[i]=0;
	}
    }
  return 1;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineStackControl::UpdateFromMasterFrame(int mode)
{
  if (this->SplineEditor->GetSplineControl()->GetAutoUpdateMode()==1)
    {
      int  sl=this->SplineEditor->GetBeginSlice();
      this->UpdateFromSplineEditor(sl);
    }

  this->SplineEditor->SetEnableSliceChangeCallback(0);
  this->SplineEditor->SetSlice((int)this->sliceScale->getValue());
  this->SplineEditor->SetEnableSliceChangeCallback(1);
  this->UpdateSplineEditor();


  if (this->BaseViewer!=NULL)
    {
      double lp[3]; this->BaseViewer->GetLastClickedPoint(lp);
      lp[2]=sliceScale->getValue();
      this->BaseViewer->SetCoordinates(int(lp[0]),int(lp[1]),int(lp[2]));
    }

}

void vtkpxGUIObjectmapSplineStackControl::UpdateMasterFrame()
{
  sliceScale->setValue(this->SplineEditor->GetSliceNumber());

  if (this->BaseViewer!=NULL)
    {
      double lp[3]; this->BaseViewer->GetLastClickedPoint(lp);
      lp[2]=sliceScale->getValue();
      this->BaseViewer->SetCoordinates(int(lp[0]),int(lp[1]),int(lp[2]));

    }
}

void vtkpxGUIObjectmapSplineStackControl::UpdateSplineEditor(int sl)
{
  if (this->SplineEditor==NULL)
    return;

  if (sl==-1)
    sl=this->SplineEditor->GetBeginSlice();
  int num=this->GetNumberOfSplineStacks();


  //  fprintf(stderr,"\tSTACK:: Sending Update To Spline Editor for current slice=%d BLOCK=%d\n",sl,this->BlockEditorCallback);

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
void vtkpxGUIObjectmapSplineStackControl::UpdateFromSplineEditor(int sl)
{
  if (sl==-1)
    sl=(int)sliceScale->getValue();
  int num=this->GetNumberOfSplineStacks();

  //  fprintf(stderr,"\tSTACK:: Receiving Update From Spline Editor for previous slice=%d BLOCK=%d\n",sl,this->BlockEditorCallback);
  for (int i=0;i<num;i++)
    {
      if (this->SplineEditor->GetSplineControl()->GetChangedStatus(i)==1)
	{
	  vtkpxSplineStackSource* stack=this->GetSplineStack(i);
	  if (sl>=stack->GetTrimBottom() && sl<=stack->GetTrimTop())
	    {
	      vtkpxSplineSource*spl=this->SplineEditor->GetSplineControl()->GetSpline(i);
	      stack->SetSpline(spl,sl);
	      surface_modified[i]=1;

	      if (i==this->SplineEditor->GetSplineControl()->GetCurrentIndex())
		this->SplineEditor->GetSplineControl()->CopySpline();
	    }
	}
    }
  this->UpdateDisplay();
  if (this->BaseViewer!=NULL)
    this->BaseViewer->UpdateObjectmap();

  if (this->do_autosave==PXTrue)
    this->DoAutosave();

  //  this->UpdateMasterFrame();

}

/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineStackControl::DoShiftScale(int mode,int moption)
{
  if (mode==0)
    {
      if (this->optionDialog->isOpen())
	return;

      delete this->optionDialog;
      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,6);
      this->optionDialog->initDisplay();

      this->optionDialog->setTitleDescription("Shift+Scale Surface",
					      "Enter Multipliers to Shift and Scale the Surfaces");
      this->optionDialog->setfoption(0,"Shift-X","%.1f",0.0);
      this->optionDialog->setfoption(2,"Shift-Y","%.1f",0.0);
      this->optionDialog->setfoption(4,"Shift-Z","%.1f",0.0);
      this->optionDialog->setfoption(1,"Scale-X","%.2f",1.0);
      this->optionDialog->setfoption(3,"Scale-Y","%.2f",1.0);
      this->optionDialog->setfoption(5,"Scale-Z","%.2f",this->spacing[2]/this->currentSplineStack->GetZSpacing());
      this->optionDialog->setMenuOption(moption);
      this->optionDialog->popupDialog();
    }

  if (mode==1)
    {
      double sh[3],sc[3];
      for (int ia=0;ia<=2;ia++)
	{
	  sh[ia]=Frange(this->optionDialog->getfoption(ia*2)  ,-100.0,100.0);
	  sc[ia]=Frange(this->optionDialog->getfoption(ia*2+1),   0.1, 10.0);
	}
     
      vtkpxSplineStackSource *stack=this->GetSplineStack(current_splinestack);
      stack->ScaleShiftXY(sh[0],sh[1],sc[0],sc[1]);
      stack->ScaleShiftZ(sh[2],sc[2]);
      
      this->UpdateAfterExternalEditing();
    }
      
      
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineStackControl::DoResample(int mode,int moption)
{
  if (mode==0)
    {
      if (this->optionDialog->isOpen())
	return;

      delete this->optionDialog;
      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,2);
      this->optionDialog->initDisplay();
      
      this->optionDialog->setTitleDescription("Resample Surfaces",
					      "Enter npoints=-1 for auto-spacing");
      this->optionDialog->setioption(0,"Points","%d",-1);
      this->optionDialog->setfoption(1,"Smoothness","%.1f",2.0);
      this->optionDialog->setMenuOption(moption);
      this->optionDialog->popupDialog();
      return;
    }

  if (mode==1)
    {
      int np=Irange(this->optionDialog->getioption(0),3,50);
      if (np==3)
	np=-1;
      float ds=Frange(this->optionDialog->getfoption(1),1.0,20.0);
      
      vtkpxSplineStackSource *stack=this->GetSplineStack(current_splinestack);
      stack->ChangeSpacing(ds,np,0.01);
     
      this->UpdateAfterExternalEditing();
    }

      

}


void vtkpxGUIObjectmapSplineStackControl::DoCylinders(int mode,int moption)
{
  if (mode==0)
    {
      if (this->optionDialog->isOpen())
	return;

      delete this->optionDialog;
      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,7);
      this->optionDialog->initDisplay();

      this->optionDialog->setTitleDescription("Set Cylinders",
					      "Enter Cylinder Parameters");
      this->optionDialog->setfoption(0,"Center X","%.2f",this->origin[0]+this->dimensions[0]*this->spacing[0]*0.5);
      this->optionDialog->setfoption(2,"Center Y","%.2f",this->origin[1]+this->dimensions[1]*this->spacing[1]*0.5);
      this->optionDialog->setfoption(1,"Radius X","%.2f",this->dimensions[0]*this->spacing[0]*0.2);
      this->optionDialog->setfoption(3,"Radius Y","%.2f",this->dimensions[1]*this->spacing[1]*0.2);
      this->optionDialog->setioption(4,"Bot Slice","%d",0);
      this->optionDialog->setioption(5,"Top Slice","%d",this->dimensions[2]-1);
      this->optionDialog->setioption(6,"Num points","%d",8);
      this->optionDialog->setMenuOption(moption);
      this->optionDialog->popupDialog();
      return;
    }


  if (mode==1)
    {
      float c[2],r[2];
      for (int ia=0;ia<=1;ia++)
	{
	  c[ia]=Frange(this->optionDialog->getfoption(ia*2),
		       this->origin[ia],this->origin[ia]+this->dimensions[ia]*this->spacing[ia]);
	  r[ia]=Frange(this->optionDialog->getfoption(ia*2+1),
		       this->spacing[ia],0.5*+this->dimensions[ia]*this->spacing[ia]);
	}
      
      
      int numsl=this->dimensions[2];
      float bottomz=this->origin[2];
      float zsp=this->spacing[2];
      int nump=Irange(this->optionDialog->getioption(6),4,24);
      int minz=Irange(this->optionDialog->getioption(4),0,this->dimensions[2]-2);
      int maxz=Irange(this->optionDialog->getioption(5),minz,this->dimensions[2]-2);
      
      vtkpxSplineStackSource *stack=this->GetSplineStack(current_splinestack);
      stack->SetEllipticalCylinder(nump,numsl,r[0],r[1],c[0],c[1],zsp,bottomz);
      stack->SetTrimBottom(minz);
      stack->SetTrimTop(maxz);
      this->UpdateAfterExternalEditing();
    }

}
// ---------------------------------------------------------------------------
void vtkpxGUIObjectmapSplineStackControl::ComputeVOIProperties(int frame)
{
  vtkpxGUISplineStackControl::ComputeVOIProperties(frame);
}

void vtkpxGUIObjectmapSplineStackControl::SetSplineEditor(vtkpxGUI2DSplineObjectmapViewer* splineControl)
{
  this->SplineEditor=splineControl;
  if (this->SplineEditor!=NULL)
    {
      this->SplineEditor->SetNumberOfSplines(this->NumberOfSplineStacks);
      this->SplineEditor->SetCallback(this,700);
    }
}
// ---------------------------------------------------------------------------

int vtkpxGUIObjectmapSplineStackControl::LoadSet(const char* fname)
{
  int status=0;
  gzFile fin;

  fin=gzsuffixopen(fname,"r");
  char line[300];
  if (fin!=NULL)
    {
      gzgets(fin,line,100);
      if (gzfindstringinstring(line, "#BFPSplineStack Set File")==1)
	status=1;
    }
  
  if (status==0)
    {
      if (fin!=NULL)
	gzclose(fin);
      sprintf(pxtk_buffer2,"Failed to load SplineStack Set from %s",fname);
      this->EventManager->messageBox(pxtk_buffer2,"Load Set Failure");
      return 0;
    }
     
  gzgets(fin,line,100); int nt; sscanf(line,"%d",&nt);

  int ntorig=nt;
  if (nt>this->NumberOfSplineStacks)
    nt=this->NumberOfSplineStacks;

  int old_col=current_splinestack;
  
  for (int i=0;i<nt;i++)
    {
      gzgets(fin,line,100); 
      //      fprintf(stderr,"line=%s\n",line);
      gzgets(fin,pxtk_buffer2,200); 
      sscanf(pxtk_buffer2,"%s\n",&line[0]); // filename
      this->SetCurrentCollection(i);
      this->LoadSplineStack(line);

      gzgets(fin,line,100); 
      double c[3];  sscanf(line,"%lf %lf %lf",&c[0],&c[1],&c[2]);
      this->SetItemColor(i,c);
    }
  gzclose(fin);

  delete [] surface_setfilename;
  surface_setfilename=new char[strlen(fname+1)];
  strcpy(surface_setfilename,fname);
  
  sprintf(pxtk_buffer2,"Loaded %d surfaces from\n %s\n (total surfaces in file=%d)\n",nt,fname,ntorig);
  this->EventManager->messageBox(pxtk_buffer2,"Load Set Information");
  this->SetCurrentCollection(old_col);
  UpdateControlsFromCurrentSplineStack();
  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIObjectmapSplineStackControl::SaveSet(const char* fname)
{
  int status=0;
  FILE* fout=fopen(fname,"w");
  if (fout==NULL)
    {
      sprintf(pxtk_buffer2,"Failed to Save SplineStack Set in %s",fname);
      this->EventManager->messageBox(pxtk_buffer2,"Save Set Failure");
      return 0;
    }

  char line[300];
  int nt=this->NumberOfSplineStacks;
  int old_col=current_splinestack;

  for (int i=0;i<nt;i++)
    {
      sprintf(line,"unnamed_%d",i+1);
      int n=9+(i>=9);
      if (strncmp(surface_filename[i],line,n)==0)
	sprintf(line,"%s_%d.sur",fname,i+1);
      else
	strcpy(line,surface_filename[i]);
      
      this->SetCurrentCollection(i);
      this->SaveSplineStack(line);
    }
  
  fprintf(fout,"#BFPSplineStack Set File\n%d\n",this->NumberOfSplineStacks);
  for (int i=0;i<nt;i++)
    {
      fprintf(fout,"# ----------------- Spline Stack %d ---------------------\n",i+1);
      fprintf(fout,"%s\n",surface_filename[i]);
      double c[3]; vtkpxSurfaceUtil::GetColor(i,c);
	if (splineStackActor[0][current_splinestack]!=NULL)
	  {
	    if ( splineStackActor[0][i]!=NULL)
	      splineStackActor[0][i]->GetProperty()->GetColor(c);
	  }
	fprintf(fout,"%.3f %.3f %.3f\n",c[0],c[1],c[2]);
    }
  fclose(fout);


  delete [] surface_setfilename;
  surface_setfilename=new char[strlen(fname+1)];
  strcpy(surface_setfilename,fname);

  sprintf(pxtk_buffer2,"Saved %d surfaces to\n %s \n",nt,fname);
  this->EventManager->messageBox(pxtk_buffer2,"Save Set Information");
  this->SetCurrentCollection(old_col);
  UpdateControlsFromCurrentSplineStack();
  return 1;
}

/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapSplineStackControl::DoExport(int mode,int moption)
{
  if (mode==0)
    {
      if (this->optionDialog->isOpen())
	return;

      delete this->optionDialog;
      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,4);
      this->optionDialog->initDisplay();
      this->optionDialog->setTitleDescription("Export Surfaces",
					      "Format 1=.tstack 2=.vtk");
      
      if (this->orientation!=3)
	{
	  this->optionDialog->setfoption(0,"Spacing XY","%.1f",this->spacing[0]);
	  this->optionDialog->setfoption(1,"Spacing  Z","%.1f",this->spacing[0]);
	}
      else
	{
	  this->optionDialog->setfoption(0,"Spacing XY","%.1f",this->spacing[0]*3.0);
	  this->optionDialog->setfoption(1,"Spacing  Z","%.1f",this->spacing[0]*3.0);
	}

      this->optionDialog->setioption(2,"Format","%d",2);
      this->optionDialog->setioption(3,"Smoothing Iterations","%d",20);
      this->optionDialog->setMenuOption(moption);
    } 

  if (mode==0)
    this->optionDialog->popupDialog();

  if (mode!=1)
    return;

  float uspacing=Frange(this->optionDialog->getfoption(0),0.2*this->spacing[0],this->spacing[0]*10);
  float vspacing=Frange(this->optionDialog->getfoption(1),0.2*this->spacing[2],this->spacing[2]*10);
  int format=Irange(this->optionDialog->getioption(2),1,2);
  int iterations=Irange(this->optionDialog->getioption(2),0,100);
    
  char extension[10];
  char* line=NULL;
  if (format==1)
    {
      sprintf(pxtk_buffer,"%s.tstack",surface_filename[current_splinestack]);
      line=this->EventManager->getSaveFilename("Save as tstack",pxtk_buffer,"Tstack Surface",".tstack");
      strcpy(extension,".tstack");
    }
  else
    {
      sprintf(pxtk_buffer,"%s.vtk",surface_filename[current_splinestack]);
      line=this->EventManager->getSaveFilename("Save  as vtk surfaces",pxtk_buffer,"VTK Surface","*.vtk");
      strcpy(extension,".vtk");
    }
  
  if (line==NULL)
    return;
  
  char* noext2=nothisextension(line,extension);

  vtkpxTriangulatedStackSource* tstack=vtkpxTriangulatedStackSource::New();
  vtkPolyDataWriter* poly=vtkPolyDataWriter::New();
  
  PXTkApp::popupConsole();
  sprintf(pxtk_buffer3,"%s%s",noext2,extension);
  vtkpxSplineStackSource *stack=this->currentSplineStack;
  tstack->SetFromSplineStackSource2(stack,uspacing,vspacing,-10000.0,10000.0);
  tstack->NonShrinkSmooth(0.33,-0.34,iterations);
  
  if (format==1)
    {
      int ok=tstack->Save(pxtk_buffer3);
      sprintf(pxtk_buffer,"Saving to %s (status=%d)\n",pxtk_buffer3,ok);
    }
  else
    {
      tstack->Update();
      poly->SetInput(tstack->GetOutput());
      poly->SetFileName(pxtk_buffer3);
      poly->Update();
      sprintf(pxtk_buffer,"Saving to %s\n",pxtk_buffer3);
    }
  PXTkApp::printToConsole(pxtk_buffer);
  poly->Delete();
  tstack->Delete();
}


