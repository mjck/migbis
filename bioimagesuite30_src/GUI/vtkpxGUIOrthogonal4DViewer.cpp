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
  Module:    $RCSfile: vtkpxGUIOrthogonal4DViewer.cpp,v $
  Language:  C++
  Date:      $Date: 2003/05/28 16:43:52 $
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
#include "vtkpxGUIColormapEditor.h"
#include "pxtkframe.h"
#include "vtkActor.h"
#include "vtkActorCollection.h"
#include "vtkObjectFactory.h"
#include "vtkOutlineFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkPropCollection.h"
#include "vtkpxVolume.h"
#include "vtkpxGUIVolumeControl.h"
#include "vtkpxGUIObliqueImageSliceControl.h"
#include "vtkVolumeCollection.h"
#include "vtkRenderer.h"
#include "vtkpxGUI4DRenderer.h"
#include "vtkpxGUIOrthogonal4DViewer.h"
#include "vtkpxImageExtract.h"
#include "vtkpxImageSlice.h"
#include "vtkpxOrthoImageSlice.h"
#include "vtkpxObliqueImageSlice.h"
#include "pxtktoplevel.h"

vtkpxGUIOrthogonal4DViewer* vtkpxGUIOrthogonal4DViewer::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIOrthogonal4DViewer");
  if(ret)
      {
	return (vtkpxGUIOrthogonal4DViewer*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIOrthogonal4DViewer;
}

// Construct object with no children.
vtkpxGUIOrthogonal4DViewer::vtkpxGUIOrthogonal4DViewer()
{
  for (int i=0;i<=3;i++)
    this->MultiRenderer[i]=NULL;
  this->ImageSliceCollection=NULL;
  this->AxisActorCollection=NULL;
  this->OutlineActorCollection=NULL;
  this->ActualNumberOfFrames=0;
  this->EnableFrameChangeOnCallback=0;
  this->createTalairachButtons=PXFalse;
  this->ImageVolumeCollection=NULL;

  this->MovieControl=NULL;
  this->MovieFrameDelay=10.0;
  this->MovieFrameStep=1;
  this->MovieLoopMode=1;
  this->CurrentlyPlayingMovie=0;
  this->MoviePlayMode=0;
  this->MovieControlInside=1;
  //  this->SingleFrameMode=0;
  this->ColorMode=0;
  this->showMB=NULL;
}
// -------------------------------------------------------------------------
// This needs work 
vtkpxGUIOrthogonal4DViewer::~vtkpxGUIOrthogonal4DViewer()
{
  if (this->ImageSliceCollection)
    this->ImageSliceCollection->Delete();

  if (this->AxisActorCollection)
    this->AxisActorCollection->Delete();

  if (this->OutlineActorCollection)
    this->OutlineActorCollection->Delete();

  if (this->ImageVolumeCollection)
    this->ImageVolumeCollection->Delete();
  
  for (int i=0;i<=3;i++)
    if (this->MultiRenderer[i]!=NULL)
      this->MultiRenderer[i]->Delete();

}

/* -------------------------------------------------------------------------*/
void vtkpxGUIOrthogonal4DViewer::InitViewer()
{
  render_widget=new PXVTkTkRenderWidget(this->EventManager,viewerFrame,200);
  render_widget->setWidthHeight(400,400);
  render_widget->bindMouseEvents();
  viewerFrame->addChildren("-side left -expand true -fill both",render_widget);
  
  // Background renderer 

  vtkRenderer* ren=vtkRenderer::New();
  render_widget->getRenderWindow()->AddRenderer(ren);

  for (int ia=1;ia>=0;ia=ia-1)
    {
      for (int ib=1;ib>=0;ib-=1)
	{
	  int index=ia*2+ib;
	  vtkRenderer* ren=vtkRenderer::New();
	  render_widget->getRenderWindow()->AddRenderer(ren);
	  this->MultiRenderer[index]=vtkpxGUI4DRenderer::New();
	  this->renderer[index]=this->MultiRenderer[index];
	  this->MultiRenderer[index]->SetAutoZoom(1.0);
	  this->MultiRenderer[index]->SetFlipMode(0);
	  if (index!=3)
	    {
	      this->MultiRenderer[index]->SetNoGUI(PXTrue);
	      this->MultiRenderer[index]->SetPlaneMode(Irange(index,0,2));
	      this->MultiRenderer[index]->SetClipMode(PXTrue);
	      this->MultiRenderer[index]->SetClipThickness(2.0);
	      this->MultiRenderer[index]->SetEnableActorTransform(0);
	    }
	  else
	    {
	      this->MultiRenderer[index]->SetClipMode(PXFalse);
	      this->MultiRenderer[index]->SetClipThickness(0.25);
	      this->MultiRenderer[index]->SetVerticalDecoration(PXFalse);
	      this->MultiRenderer[index]->SetEnableActorTransform(1);
	    }
	  this->MultiRenderer[index]->Initialize(viewerFrame->getWidgetName(),ren,0);
	  this->MultiRenderer[index]->SetViewport((ia)*0.5,(1-ib)*0.5,(1+ia)*0.5,(2-ib)*0.5);
	  this->MultiRenderer[index]->Reset();
	}
    }
  this->UpdateDisplay();
  //render_widget->getRenderWindow()->Render();
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonal4DViewer::InitializeViewer()
{
  if ((!hasImage && this->Initialized))
      return 0;

  this->ColormapEditor=vtkpxGUIColormapEditor::New();
  this->ColormapEditor->SetSimpleMode(0);
  this->ColormapEditor->SetCallback(this,201);
  this->ColormapEditor->Initialize(this->GetWidgetName(),0);
  
  this->ImageSliceCollection=vtkPropCollection::New();
  this->AxisActorCollection=vtkActorCollection::New();
  this->OutlineActorCollection=vtkActorCollection::New();
  this->ImageVolumeCollection=vtkVolumeCollection::New();

  for (int i=0;i<=3;i++)
    {
      outlineFilter[i]=vtkOutlineFilter::New();
      outlineFilter[i]->SetInput(currentImage);
    } 

  this->SetDisplay3DMode(display3d_mode);
  this->SetDisplay3DAxisOutlineMode(display3d_axisoutlinemode);

  return 1;
}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonal4DViewer::CreateFrames(int numframes)
{
  if (numframes<this->ActualNumberOfFrames)
    return;

  if (numframes>999)
    numframes=999;
  if (numframes<1)
    numframes=1;

  int i=0;

  for (i=0;i<=3;i++)
    this->MultiRenderer[i]->SetNumberOfFrames(numframes);
  
  
  for (int fr=this->ActualNumberOfFrames;fr<numframes;fr++)
    {
      //fprintf(stderr,"--------------------------------------\n");
      for (i=0;i<=3;i++)
	this->MultiRenderer[i]->SetCurrentFrame(fr);

      /*fprintf(stderr,"Creating Frame=%d renderer[fr][0]=%d\n",fr,
	(int)this->MultiRenderer[0]->GetRenderer());*/
      
      for (int k=0;k<=2;k++)
	{
	  imageSlice[k]=vtkpxImageSlice::New();
	  imageSlice[k]->SetAutoUpdate(0);
	  imageSlice[k]->SetInput(currentImage);
	  imageSlice[k]->SetFrame(fr);
	  imageSlice[k]->SetCurrentPlane(k);
	  imageSlice[k]->PickableOff();
	  this->MultiRenderer[k]->GetRenderer()->AddActor(imageSlice[k]);
	  this->ImageSliceCollection->AddItem(imageSlice[k]);
	}
      
      orthoSlice=vtkpxOrthoImageSlice::New();
      orthoSlice->SetInput(currentImage);
      orthoSlice->SetFrame(fr);
      orthoSlice->PickableOff();
      orthoSlice->SetPolarMode(0);
      this->MultiRenderer[3]->GetRenderer()->AddActor(orthoSlice);
      orthoSlice->SetVisibility(this->OrthoSliceVisibility);
      this->ImageSliceCollection->AddItem(orthoSlice);

      obliqueSlice=vtkpxObliqueImageSlice::New();
      obliqueSlice->SetAutoUpdate(0);
      obliqueSlice->SetInput(currentImage);
      obliqueSlice->SetFrame(fr);
      obliqueSlice->PickableOff();
      obliqueSlice->SetVisibility(0);
      this->MultiRenderer[3]->GetRenderer()->AddActor(obliqueSlice);
      this->ImageSliceCollection->AddItem(obliqueSlice);
      
      // Volume Stuff
      this->Volume=vtkpxVolume::New();
      this->Volume->SetInput(currentImage,this->Orientation);
      this->Volume->SetFrame(fr);
      this->Volume->SetVisibility(0);
      this->Volume->Update();
      this->MultiRenderer[3]->GetRenderer()->AddVolume(this->Volume);
      this->ImageVolumeCollection->AddItem(this->Volume);
      
      // Outline Stuff
      for (i=0;i<=3;i++)
	{
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
	  this->MultiRenderer[i]->GetRenderer()->AddActor(outline[i]);
	  mapOutline->Delete();
	  this->OutlineActorCollection->AddItem(outline[i]);
	}
	  
      // Axis Stuff
      // Create Axis 
      for (int rend=0;rend<=2;rend++)
	for (int axis=0;axis<=1;axis++)
	  {
	    int index=rend*2+axis;
	    axisActor[index]=vtkActor::New();
	    axisActor[index]->PickableOff();
	    this->MultiRenderer[rend]->GetRenderer()->AddActor(axisActor[index]);
	    this->AxisActorCollection->AddItem(axisActor[index]);
	  }
      for (i=6;i<=9;i++)
	{
	  axisActor[i]=vtkActor::New();
	  axisActor[i]->PickableOff();
	  this->MultiRenderer[3]->GetRenderer()->AddActor(axisActor[i]);
	  axisActor[i]->SetVisibility(0);
	  this->AxisActorCollection->AddItem(axisActor[i]);
	}
    }
  this->ActualNumberOfFrames=numframes;
  
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonal4DViewer::HandleEvent(int event)
{

  // Block Volume Control pop-up
  if (event==330 && this->CurrentlyPlayingMovie==1)
    return TCL_OK;


  if (event<400)
    return vtkpxGUIOrthogonalViewer::HandleEvent(event);

  if (!hasImage)
    return TCL_OK;


  switch (event)
    {
    case 400:
      this->ShowMovieControl(1);
      break;
    case 401:
      this->ShowMovieControl(0);
      break;
    case 402:
      // Do Something about frame change
      {
	int cf=int(movieFrameScale->getValue()-1);
	this->FastChangeFrame(cf);
	this->UpdateDisplay();
      }
      break;

    case 403:
      this->MovieFrameDelay=1000.0*float(1.0/movieRateScale->getValue());
      break;

    case 404:
      this->MovieLoopMode=(int)loopMenu->getIndex();
      break;
      
    case 405:
      this->MoviePlayMode=(int)playMenu->getIndex();
      if (this->CurrentlyPlayingMovie==0)
	{
	  if (this->MoviePlayMode==0)
	    this->prepareB->enable(PXFalse);
	  else
	    this->prepareB->enable(PXTrue);
	}
      break;

    case 410:
      this->MoviePrepare();
      break;
    case 411:
      this->MoviePlay();
      break;
    case 412:
      this->MovieStop();
      break;
    case 413:
      this->FastChangeFrame(0);
      break;

    }

  this->LastEvent=event;
  return TCL_OK;
}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonal4DViewer::ChangeFrame(int newframe)
{
  newframe=Irange(newframe,0,this->NumberOfFrames-1);
  if (newframe==this->CurrentFrame)
    return;

  int levels[3],levels2[3];
  orthoSlice->GetLevels(levels);
  UpdateFrame(newframe,0); 
  orthoSlice->GetLevels(levels2);
  int d=0;
  for (int i=0;i<=2;i++)
    d+=abs(levels[i]-levels2[i]);
  
  this->CurrentFrame=newframe;
  //  fprintf(stderr,"in 4D Viewer:: Current Frame = %d\n",this->CurrentFrame);

  SetDisplay3DMode(display3d_mode);
  SetDisplay3DAxisOutlineMode(display3d_axisoutlinemode);

  if (this->MovieControl!=NULL)
    movieFrameScale->setValue(this->CurrentFrame+1);
  this->ExecuteFrameChangeCallback();


  if (d>0)
    this->SetCoordinates(levels[0],levels[1],levels[2]);
  else
    MoveAxis(1);

}
// -------------------------------------------------------------------------
// Movie Stuff 
// -------------------------------------------------------------------------
char* vtkpxGUIOrthogonal4DViewer::InitializeMovieControl(const char* name,int inside)
{
  if (this->Initialized==0)
    return NULL;

  if (this->MovieControl!=NULL)
    return this->MovieControl->getWidgetName();

  if (inside==1)
    {
      this->MovieControl=new PXTkFrame(NULL,name);
      this->MovieControlInside=1;
    }
  else
    {
      this->MovieControl=new PXTkToplevel(this->EventManager,this->EventManager->getMainWindow(),"Movie Controls");
      this->MovieControlInside=0;
    }
    
  PXTkFrame* mainFrame=new PXTkFrame(this->EventManager,this->MovieControl);
  this->MovieControl->addChildren("-side top -expand t -fill both",mainFrame);


  
  movieFrameScale=new PXTkArrowScale(this->EventManager,mainFrame,"Frame",402,PXTrue);
  movieFrameScale->setRange(1,10);
  movieFrameScale->setValue(float(1));
  movieFrameScale->setLengthWidth(120,10);
  movieFrameScale->setIncrement(1.0);
  movieFrameScale->setResolution(1.0);
	
  movieRateScale=new PXTkArrowScale(this->EventManager,mainFrame,"Rate",403,PXTrue);
  movieRateScale->setRange(1,64);
  movieRateScale->setValue(1.0/(this->MovieFrameDelay*0.01));
  movieRateScale->setLengthWidth(120,10);
  movieRateScale->setIncrement(1.0);
  movieRateScale->setResolution(1.0);

  PXTkFrame* m1=new PXTkFrame(this->EventManager,this->MovieControl);
  PXTkFrame* m2=new PXTkFrame(this->EventManager,this->MovieControl);

  mainFrame->addChildren("-side top -expand f -fill x",
			 movieFrameScale->getMainWindow(),movieRateScale->getMainWindow(),m1,m2);

  PXTkLabel*  md0=new PXTkLabel(this->EventManager,m1,"Play Mode:");
  playMenu=new PXTkOptionMenu(this->EventManager,m1,"Complete",405);
  playMenu->addOption("Fast");
  playMenu->setIndex(this->MoviePlayMode);

  PXTkLabel*  md=new PXTkLabel(this->EventManager,m1,"Loop Mode:");
  loopMenu=new PXTkOptionMenu(this->EventManager,m1,"Once",404);
  loopMenu->addOption("Loop");
  loopMenu->addOption("Swing");
  loopMenu->setIndex(this->MovieLoopMode);

  m1->addChildren("-side left -expand f -fill x",md0,playMenu,md,loopMenu);

  this->prepareB=new PXTkButton(this->EventManager,m2,"Prepare",410);
  this->playB=new PXTkButton(this->EventManager,m2,"Play",411);
  this->pauseB=new PXTkButton(this->EventManager,m2,"Pause",412);
  this->rewindB=new PXTkButton(this->EventManager,m2,"Rewind",413);
  m2->addChildren("-side left -expand f -fill x",this->prepareB,this->playB,this->pauseB,this->rewindB);
  this->pauseB->enable(PXFalse);

  this->closeB=new PXTkButton(this->EventManager,m2,"Close",401);

  if (inside==0)
    {
      m2->addChildren("-side right -expand f -fill x -padx 4",this->closeB);
      ((PXTkToplevel*)this->MovieControl)->hide();
      showMB=new PXTkButton(this->EventManager,coordScaleFrame,"Movie Control",400);
      coordScaleFrame->addChildren("-side bottom -expand f -fill x -padx 0 ",showMB);
    }
 
  return this->MovieControl->getWidgetName();

}
// -------------------------------------------------------------------------  
int   vtkpxGUIOrthogonal4DViewer::MoviePrepare()
{
  if (this->NumberOfFrames<2 || this->MovieControl==NULL)
    return 0;
  int fr=this->CurrentFrame;
  
  for (int i=0;i<this->NumberOfFrames;i++)
    {
      if (i!=fr)
	this->SetCurrentFrame(i);
    }
  this->SetCurrentFrame(fr);
  return 1;
}

int   vtkpxGUIOrthogonal4DViewer::MoviePlay()
{
  if (this->NumberOfFrames<2 || this->MovieControl==NULL)
    return 0;
  if (this->VolumeControl!=NULL)
    this->VolumeControl->Show(0);
  if (this->ObliqueControl!=NULL)
    this->ObliqueControl->Show(0);

  this->MovieFrameDelay=1000.0*float(1.0/movieRateScale->getValue());

  for (int i=0;i<=2;i++)
    this->MultiRenderer[i]->MoveAbove(-1);

  this->MovieFrameStep=1;
  Tcl_CreateTimerHandler((int)this->MovieFrameDelay,vtkpxGUIOrthogonal4DViewer::HandleNextFrameEvent,this);
  this->CurrentlyPlayingMovie=1;

  this->pauseB->enable(PXTrue);
  this->rewindB->enable(PXFalse);
  this->playB->enable(PXFalse);
  this->prepareB->enable(PXFalse);
  this->closeB->enable(PXFalse);
  this->playMenu->enable(PXFalse);
  this->movieFrameScale->enable(PXFalse);
  return 1;
}

int   vtkpxGUIOrthogonal4DViewer::MovieStop()
{
  if (this->NumberOfFrames<2)
    return 0;
  this->CurrentlyPlayingMovie=0;
  return 1;
}


void vtkpxGUIOrthogonal4DViewer::FastChangeFrame(int newfr)
{
  if (this->NumberOfFrames<2 || this->MovieControl==NULL)
    return;

  int newframe=newfr;

  if (newframe==-1 && this->NumberOfFrames==1)
    return;
  
  if (newframe==-1)
    {
      int fr=int(movieFrameScale->getValue()-1);
      newframe=fr+this->MovieFrameStep;
      //fprintf(stderr,"Newframe=%d fr=%d\n",newframe,fr);

      switch (this->MovieLoopMode)
	{
	case 0: // Once
	  if (newframe==this->NumberOfFrames)
	    {
	      this->CurrentlyPlayingMovie=0;
	      newframe=this->NumberOfFrames-1;
	    }
	  break;
	case 1: // Loop
	  if (newframe==this->NumberOfFrames)
	    newframe=0;
	  break;
	case 2:
	  if (newframe==this->NumberOfFrames)
	    {
	      newframe=this->NumberOfFrames-2;
	      this->MovieFrameStep=-1;
	    }
	  else if ( newframe==-1)
	    {
	      newframe=1;
	      this->MovieFrameStep=1;
	    }
	  break;
	}
    }
  

  if (this->CurrentlyPlayingMovie==1)
    {
      if (this->MoviePlayMode==1)
	{
	  this->UpdateFrame(newframe,-1); 
	  this->coordScale[3]->setValue(newframe+1,PXFalse);
	  if (newfr==-1)
	    movieFrameScale->setValue(newframe+1,PXFalse);
	  this->CurrentFrame=newframe;
	  this->UpdateDisplay();
	}
      else
	{
	  this->SetCurrentFrame(newframe);
	}
      Tcl_CreateTimerHandler((int)this->MovieFrameDelay,vtkpxGUIOrthogonal4DViewer::HandleNextFrameEvent,this);
    }
  else
    {
      this->SetCurrentFrame(newframe);
      this->pauseB->enable(PXFalse);
      this->rewindB->enable(PXTrue);
      this->playB->enable(PXTrue);
      if (this->MoviePlayMode==1)
	this->prepareB->enable(PXTrue);
      this->closeB->enable(PXTrue);
      this->playMenu->enable(PXTrue);
      this->movieFrameScale->enable(PXTrue);
    }
}

void vtkpxGUIOrthogonal4DViewer::HandleNextFrameEvent(void *arg1)
{
  ((vtkpxGUIOrthogonal4DViewer*)arg1)->FastChangeFrame(-1);
}

int vtkpxGUIOrthogonal4DViewer::ShowMovieControl(int show)
{
  if ( (this->NumberOfFrames<2 && show==1) ||  this->MovieControlInside==1)
    return 0;

  if (!show)
    ((PXTkToplevel*)this->MovieControl)->hide();
  else
    ((PXTkToplevel*)this->MovieControl)->show();

  return 1;
}

void vtkpxGUIOrthogonal4DViewer::ShowFrameControls(int show)
{
  return;
}
  

vtkpxGUI4DRenderer* vtkpxGUIOrthogonal4DViewer::GetMultiRenderer(int i)
{
  return this->MultiRenderer[Irange(i,0,3)];
}
// -------------------------------------------------------------------------
int vtkpxGUIOrthogonal4DViewer::ResetViewerUpdatePolar()
{
  int   range[3]; 
  currentImage->GetDimensions(range);

  coordScale[0]->setLabel("Z-Coord");
  coordScale[1]->setLabel("Angle 1");
  coordScale[2]->setLabel("Angle 2");
  
  int levels[3];

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
      /*imageSlice[sc]->SetAutoUpdate(0);
	imageSlice[sc]->SetCurrentPlane(sc);
	imageSlice[sc]->SetLevel(levels[sc]);
	imageSlice[sc]->Update();
	imageSlice[sc]->SetAutoUpdate(1);
	renderer[sc]->SetClipMode(PXTrue);
	renderer[sc]->SetOrientation(3);
	renderer[sc]->SetPlaneMode(sc);*/
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
  this->MoveAxisPolar(1);
  return 1;
}
// ------------------------------------------------------------------------- 
int vtkpxGUIOrthogonal4DViewer::ResetViewerUpdateCartesian() 
{
  int   range[3]; 
  currentImage->GetDimensions(range);

  coordScale[0]->setLabel("X-Coord:");
  coordScale[1]->setLabel("Y-Coord:");
  coordScale[2]->setLabel("Z-Coord:");
  
  /*  for (int kk=0;kk<=2;kk++)
      {
      imageSlice[kk]->SetAutoUpdate(0);
      imageSlice[kk]->SetCurrentPlane(kk);
      renderer[kk]->SetPlaneMode(kk);
      renderer[kk]->SetOrientation(this->Orientation);
      renderer[kk]->SetClipMode(PXFalse);
      }*/
  
  int levels[3];
  orthoSlice->GetLevels(levels);
  
  double   sp[3],ori[3];
  currentImage->GetSpacing(sp);
  currentImage->GetOrigin(ori);
  
  double lvv[3],lvs[3];
  for (int ic=0;ic<=2;ic++)
    {
      lvv[ic]=double(levels[ic]);
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

int vtkpxGUIOrthogonal4DViewer::ResetViewer()
{
  if (!(hasImage && this->Initialized))
      return 0;

  if (this->Orientation==3)
    this->ModifyPolarImageSpacing();

  this->CreateFrames(this->NumberOfFrames);

  for (int i=1;i<this->NumberOfFrames;i++)
    this->UpdateFrame(i,1);
  UpdateFrame(0,1);

  for (int j=0;j<=3;j++)
    outlineFilter[j]->SetInput(currentImage);

  int   range[3]; 
  currentImage->GetDimensions(range);

  this->ColormapEditor->SetLock(0);
  this->ColormapEditor->SetRange(currentImage);
  this->SetPresetLookupTable(this->PresetLookupTableMode);
  this->ColormapEditor->Update();


  int doupdate=1;
  if (firsttimereset>0)
    doupdate=0;

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
    {
      doupdate=1;
    }

  
  if (doupdate)
    {
      if (this->Orientation!=3)
	this->ResetViewerUpdateCartesian();
      else
	this->ResetViewerUpdatePolar();
    }

  
  if (this->MovieControl!=NULL)
    {
      movieFrameScale->setRange(1,this->NumberOfFrames);
      movieFrameScale->setValue(this->CurrentFrame+1);
      movieFrameScale->setResolution(1.0);
    }
  
  vtkpxGUIOrthogonalViewer::ShowFrameControls(1);

  if (this->NumberOfFrames==1)
    {
      coordScale[3]->enable(PXFalse);
      if (showMB!=NULL)
	showMB->enable(PXFalse);
      this->ShowMovieControl(0); 
    }
  else
    {
      coordScale[3]->enable(PXTrue);
      if (showMB!=NULL)
	showMB->enable(PXTrue);
    }

  if (this->obliqueSlice!=NULL && this->Orientation!=3)
    {
      //      fprintf(stderr,"Setting Camera Callback\n");
      this->MultiRenderer[3]->SetCameraCallback(this->ObliqueControl,140);
    }


  firsttimereset=0;
  return 1;
}
// -------------------------------------------------------------------------
// Update Frame Stuff
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonal4DViewer::UpdateFrameCartesian(int frame,int full)
{
  if (full==1)
    {
      int   range[3]; 
      currentImage->GetDimensions(range);
      
      orthoSlice->SetAutoUpdate(0);
      orthoSlice->SetLevels(range[0]/2,range[1]/2,range[2]/2);
      orthoSlice->Update();
      orthoSlice->SetAutoUpdate(1);

      for (int kk=0;kk<=2;kk++)
	{
	  imageSlice[kk]->SetAutoUpdate(0);
	  imageSlice[kk]->SetLevel(range[kk]/2);
	  imageSlice[kk]->Update();
	  imageSlice[kk]->SetAutoUpdate(1);
	}
    }

  this->Volume=(vtkpxVolume*)this->ImageVolumeCollection->GetItemAsObject(frame);


  if (full==1)
    {
      this->Volume->SetInput(currentImage,this->Orientation);
      this->Volume->SetFrame(frame);
      this->VolumeControl->Update(1);
      this->obliqueSlice->SetInput(currentImage);
      this->obliqueSlice->SetFrame(frame);
      this->ObliqueControl->Update(1);
    }



  if (full>=0)
    {
      this->Volume->SetFromLookupTable(this->VolumeControl->GetColorMapEditor()->GetColormap());
      this->VolumeControl->UpdateVolumeRenderer(this->Volume,renderer[3]);
      //      fprintf(stderr,"Updating Image Slice Renderer\n");
      this->ObliqueControl->UpdateObliqueImageSliceRenderer(this->obliqueSlice,renderer[3]);
      if (full==0)
	this->ObliqueControl->Update(0);
    }


  if (full==1)
    this->Volume->Update();


}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonal4DViewer::UpdateFramePolar(int frame,int full)
{
  if (full)
    {
      int   range[3]; 
      currentImage->GetDimensions(range);
      
      int v[3];
      v[0]=range[1]/2;
      v[1]=0;
      v[2]=range[2]/2;

      orthoSlice->SetAutoUpdate(0);      
      orthoSlice->SetLevels(v[0],v[1],v[2]);
      orthoSlice->Update();
      orthoSlice->SetAutoUpdate(1);

      for (int kk=0;kk<=2;kk++)
	{
	  imageSlice[kk]->SetAutoUpdate(0);
	  imageSlice[kk]->SetLevel(v[kk]);
	  imageSlice[kk]->Update();
	  imageSlice[kk]->SetAutoUpdate(1);
	}
      
    }
  this->Volume=(vtkpxVolume*)this->ImageVolumeCollection->GetItemAsObject(frame);


}
// -------------------------------------------------------------------------
void vtkpxGUIOrthogonal4DViewer::UpdateFrame(int frame,int full)
{
  if (frame>=this->ActualNumberOfFrames)
    return;
  
  // First Collect Stuff from Collections 
  if (full>=0)
    {
      for (int r=0;r<=3;r++)
	this->MultiRenderer[r]->SetCurrentFrame(frame);
    }
  else
    {
      for (int r=0;r<=3;r++)
	this->MultiRenderer[r]->FastSetCurrentFrame(frame);
    }
  
  // Image Slice Ortho Slice First 
  int index=frame*5;
  
  for (int i=0;i<=2;i++)
    {
      imageSlice[i]=(vtkpxImageSlice*)this->ImageSliceCollection->GetItemAsObject(index+i);
      if (full==1)
	{
	  imageSlice[i]->SetAutoUpdate(0);
	  imageSlice[i]->SetInput(currentImage);
	  imageSlice[i]->SetFrame(frame);
	  imageSlice[i]->SetLookupTable(mainLookupTable);
	  if (this->Orientation==3)
	    imageSlice[i]->SetPolarMode(1);
	  else
	    imageSlice[i]->SetPolarMode(0);
	  imageSlice[i]->SetAutoUpdate(1);
	  imageSlice[i]->Update();
	  //	  this->MultiRenderer[i]->SetOrientation(this->Orientation);
	  this->MultiRenderer[i]->SetCurrentImageOrientation(currentImage,this->Orientation);
	  this->MultiRenderer[i]->SetClipThickness(currentImage->GetSpacing()[2]);
	}
    }

  orthoSlice=(vtkpxOrthoImageSlice*)this->ImageSliceCollection->GetItemAsObject(index+3);
  obliqueSlice=(vtkpxObliqueImageSlice*)this->ImageSliceCollection->GetItemAsObject(index+4);
  if (full==1)
    {
      orthoSlice->SetAutoUpdate(0);
      orthoSlice->SetInput(currentImage);
      orthoSlice->SetFrame(frame);
      orthoSlice->SetLookupTable(mainLookupTable);
      if (this->Orientation==3)
	orthoSlice->SetPolarMode(1);
      else
	orthoSlice->SetPolarMode(0);
      orthoSlice->SetAutoUpdate(1);

      obliqueSlice->SetAutoUpdate(0);
      obliqueSlice->SetInput(currentImage);
      obliqueSlice->SetFrame(frame);
      obliqueSlice->SetLookupTable(mainLookupTable);
      if (this->Orientation==3)
	obliqueSlice->SetDisplayMode(0);
      else
	obliqueSlice->SetDisplayMode(3);
      obliqueSlice->SetAutoUpdate(1);

      //      this->MultiRenderer[3]->SetOrientation(this->Orientation);
      this->MultiRenderer[3]->SetPlaneMode(2);
      this->MultiRenderer[3]->GetRenderer()->SetBackground(0.2,0.2,0.3);
      this->MultiRenderer[3]->SetCurrentImageOrientation(currentImage,this->Orientation);
    }
  
  index=frame*4;
  for (int out=0;out<=3;out++)
    {
      int vis=0;
      if (outline[out]!=NULL)
	vis=outline[out]->GetVisibility();

      outline[out]=(vtkActor*)this->OutlineActorCollection->GetItemAsObject(index+out);
      outline[out]->SetVisibility(vis);
    }
  
  index=frame*10;
  for (int ax=0;ax<=9;ax++)
    {
      int vis=0;
      if (axisActor[ax]!=NULL)
	vis=axisActor[ax]->GetVisibility();
      axisActor[ax]=(vtkActor*)this->AxisActorCollection->GetItemAsObject(index+ax);
      axisActor[ax]->SetVisibility(vis);
    }



  if (this->Orientation==3)
    this->UpdateFramePolar(frame,full);
  else
    this->UpdateFrameCartesian(frame,full);
  
  return;
}

