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
#include "pxutil.h"
#include "vtkActor.h"
#include "vtkImageQuantizeRGBToIndex.h"
#include "vtkObjectFactory.h"
#include "vtkpxGUI2DImageViewer.h"
#include "vtkpxColorMapUtil.h"
#include "vtkImageThreshold.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkpxUtil.h"
#include "vtkbisImageReslice.h"
// -------------------------------------------------------------------------
vtkpxGUI2DImageViewer* vtkpxGUI2DImageViewer::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUI2DImageViewer");
  if(ret)
    {
      return (vtkpxGUI2DImageViewer*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUI2DImageViewer;
}
// -------------------------------------------------------------------------
// Construct object with no children.
vtkpxGUI2DImageViewer::vtkpxGUI2DImageViewer()
{
  this->offset=0;
  this->ColormapEditor=NULL;
  this->timeframeControlFrame=NULL;
  this->frameScale=NULL;

  this->MouseToEditor=0;
  this->ShiftMouseToEditor=1;
  this->Editor=NULL;

  initialized=PXFalse;
  this->statusLabel=NULL;
  this->donotRender=0;
  this->inexpose=0;
  this->inexposecount=0;
  this->LastEvent=-1;
  this->ownsLookupTable=1;

  this->maskImage=NULL;
  this->maskImageRegion=NULL;
  this->maskImageSlice=NULL;
  this->maskTransparency=NULL;
  this->currentImageRegion=NULL;

  this->LastExtractedSlice=-1;
  this->LastExtractedOrientation=0;
  this->LastExtractedFrame=0;

  maskLookupTable=vtkLookupTable::New();
  vtkpxSurfaceUtil::DefaultObjectMapLookupTable(maskLookupTable,-1,1);

  firsttime=1;
  olddimensions[0] =  -1;
  olddimensions[1] =  -1;
  olddimensions[1] =  -1;

}
// -------------------------------------------------------------------------
vtkpxGUI2DImageViewer::~vtkpxGUI2DImageViewer()
{
  if (this->maskImage)
    this->maskImage->Delete();
  if (this->maskImageRegion)
    this->maskImageRegion->Delete();
}
/* -------------------------------------------------------------------------*/
char* vtkpxGUI2DImageViewer::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
    return GetWidgetName();
  
  vtkpxGUIComponent::Initialize(name,inside);

  PXTkFrame* manager=(PXTkFrame*)(this->EventManager->getMainWindow());
  PXTkFrame* viewerFrame=new PXTkFrame(this->EventManager,manager);
  manager->setPackMode(PXTrue);

  manager->addChildren("-side top  -expand true -fill both",viewerFrame);
  InitControls(viewerFrame,this->CreateHighlights);
  InitViewer(viewerFrame);
  return GetWidgetName();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI2DImageViewer::Update()
{
  UpdateViewer();
  UpdateDisplay();
}
// -------------------------------------------------------------------------
void vtkpxGUI2DImageViewer::SetOffset(int of)
{
  if (this->Initialized==0)
      this->offset=of;
}
// -------------------------------------------------------------------------
int vtkpxGUI2DImageViewer::InitializeViewer()
{
  if ((!hasImage && this->Initialized))
      return 0;
  
  if (mainLookupTable==NULL)
    {
      mainLookupTable=vtkLookupTable::New();
      this->ownsLookupTable=1;
      vtkpxColorMapUtil::SetStepColorMap(mainLookupTable,0,255,0,255,256,PXFalse);

    }
  
  CreateViewerAndSlice();
  
  if (this->ColormapEditor==NULL && this->ownsLookupTable==1)
    {
      ColormapEditor=vtkpxGUIColormapEditor::New();
      ColormapEditor->SetSimpleMode(0);
      ColormapEditor->SetCallback(this,200);
      ColormapEditor->Initialize(this->GetWidgetName(),0);
      ColormapEditor->SetRange(0,255);
      this->ColormapEditor->SetColormap(mainLookupTable);
    }
  
  return 1;
}

// -------------------------------------------------------------------------
int vtkpxGUI2DImageViewer::ResetViewer()
{

  if (!(hasImage && this->Initialized))
      return 0;



  // ------------------------------------------------------
  //           C h e c k    f o r    L o o k u p T a b l e
  // ------------------------------------------------------
  int numc=currentImage->GetNumberOfScalarComponents();
  if (this->ownsLookupTable)
    {
      if (numc==3 && this->NumberOfFrames==1)
	{
	  vtkpxColorMapUtil::QuantizeColorImage(currentImage,currentImage,mainLookupTable);
	  this->ColormapEditor->Update();
	  this->ColormapEditor->SetLock(1);
	}
      else
	{
	  this->ColormapEditor->SetLock(0);
	  this->ColormapEditor->SetRange(currentImage);
	  this->SetPresetLookupTable(this->PresetLookupTableMode);
	  this->ColormapEditor->Update();
	}
    }
     
  if (this->Editor!=NULL)
    this->Editor->SetLookupTable(mainLookupTable);
  
  // ------------------------------------------------------
  //   Bounds/ Highlights 
  // ------------------------------------------------------
  double bounds[6];  GetBounds(bounds);
  int   range[3];   currentImage->GetDimensions(range); 
  double sp[3];      currentImage->GetSpacing(sp);
  double ori[3];     currentImage->GetOrigin(ori);

  
  if (frameScale!=NULL && this->CurrentImageIsColor==0)
    {
      this->ShowFrameControls((this->NumberOfFrames>1));
    }
  
  // ------------------------------------------------------
  //  C h e c k   for   S a m e   D i m e n s i o ns
  // ------------------------------------------------------
  int dim[3];     
  currentImage->GetDimensions(dim);
  int numslices=dim[2];
  int samesizeasbefore=0;

  if (firsttime==0)
    {
      int flag=0;
      for (int i=0;i<=2;i++)
	{
	  if (dim[i]!=olddimensions[i])
	    flag++;
	}
      if (frameScale!=NULL)
	{
	  if (this->NumberOfFrames!=(int(frameScale->getmax())))
	    flag++;
	}
      samesizeasbefore=(flag==0);
    }
  
  for (int i=0;i<=2;i++)
    olddimensions[i]=dim[i];

  if (samesizeasbefore==0)
    {
      firsttime=0;
      sliceNumber->setResolution(1);
      sliceNumber->setRange(float(offset),float(numslices-1+offset));
      sliceNumber->setValue(offset);
  
      frameScale->setRange(1,this->NumberOfFrames);
      frameScale->setResolution(1);
      frameScale->setValue(1);
  
      if (this->Orientation==3)
	{
	  renderer->SetClipMode(PXFalse);
	}
      else
	{
	  renderer->SetClipMode(PXTrue);
	}
      //      imageSlice->SetInput(currentImage);
      renderer->SetBounds(bounds);
      renderer->SetClipThickness(currentImage->GetSpacing()[2]);
      //      renderer->SetOrientation(this->Orientation);
      renderer->SetPlaneMode(2);
      renderer->SetCurrentImageOrientation(currentImage,this->Orientation);
      this->CreateMaskImage(currentImage);
    }
  else
    {
      //      imageSlice->SetInput(currentImage);
      renderer->SetCurrentImageOrientation(currentImage,this->Orientation);
    }

  


  if (this->Editor!=NULL)
    this->Editor->SetImage(currentImage,2,0);

  ChangeSliceAndFrame(-1,-1);
  this->ResetAll();
  return TCL_OK;
}
// -------------------------------------------------------------------------
int vtkpxGUI2DImageViewer::HandleEvent(int event)
{
  if (event<10)
      return this->EventManager->handleDirectEvent(event);
  
  if (event==200)
      UpdateDisplay();
  
  if (event==201 || event==202)
      {
       if (hasImage)
	 {
	   if (event==202)
	     renderer->Zoom(renderer->GetCameraScale()*1.2,PXFalse);
	   else
	     renderer->Zoom(renderer->GetCameraScale()*0.833,PXFalse);
	   
	   UpdateDisplay();
	 }
      }
  
   if (event==203)
     {
       if (hasImage)
	 {
	   int dim[3];     
	   currentImage->GetDimensions(dim);
	   int plane=2;
	   int numslices=dim[plane];
	   if (this->Orientation==3)
	     {
	       if (plane==0)
		 numslices=dim[1];
	       else
		 numslices=dim[2];
	     }
	   
	   sliceNumber->setRange(offset,numslices-1+offset);
	   if (sliceNumber->getValue()>=numslices+offset)
	     sliceNumber->setValue(0+offset);
	   
	   UpdateViewer();
	   UpdateDisplay();
	 }
     }


   if (event==205)
     {
       if (hasImage)
	 {
	   PXBool interp=interpolateMode->getState();
	   if (initialized)
	     imageSlice->SetInterpolation((int)interp);
	 }
       UpdateDisplay();
     }
   
   if (event==206)
     {
       ChangeSliceAndFrame(-1,-1);
     }

   if (event==207)
     {
       if (hasImage)
	 {
	   UpdateViewer();
	   UpdateDisplay();
	 }
     }
   
   if (event==208)
     ResetAll();
  
   if (event==210)
     {
       this->SetObjectmapTransparency(this->maskTransparency->getValue());
     }

   if (event==221)
     this->SaveAsTiff(render_widget->getRenderWindow());
   
   /*if (event==221)
     this->ExportAsVRML(render_widget->getRenderWindow());*/
   
   
   if (event==209)
     {
       if (this->ColormapEditor!=NULL)
	 this->ColormapEditor->Show();
     }
   
   if (event==214 || event==215 || event==216 || event==217 || event==218 || event==219)
     {
       if (event<219)
	 this->SetPresetLookupTable(event-215);
       else
	 this->SetPresetLookupTable(event-213);
     }
   
   
  if (event==300)
    {
      int* x=render_widget->getRenderWindow()->GetSize();
      int mouse=PXTkApp::getIntArgument(0);
      int state=PXTkApp::getIntArgument(1);
      int x1=PXTkApp::getIntArgument(2);
      int x2=x[1]-PXTkApp::getIntArgument(3);
      HandleMouseButtonEvent(mouse,state,x1,x2,this->EventManager);
    }
  
  if (event==301)
    {
      if (this->LastEvent!=301)
	{
	  this->RenderDelay(500);
	}
    }
  
  this->LastEvent=event;
  
  return TCL_OK;
  
}

void vtkpxGUI2DImageViewer::SetPresetLookupTable(int mode)
{
  if (!hasImage || this->ownsLookupTable==0)
    return;
  
  this->SetPresetLookupTableMode(mode);
  if (this->ColormapEditor!=NULL)
    this->ColormapEditor->SetFmriMode(this->PresetLookupTableMode);
}
     
// -------------------------------------------------------------------------
int vtkpxGUI2DImageViewer::UpdateViewer()
{
  if (!hasImage)
    return TCL_ERROR;

  int plane=2;
  int dim[3];     currentImage->GetDimensions(dim);
  int frame=this->CurrentFrame;

  double sp[3];  currentImage->GetSpacing(sp);
  double ori[3]; currentImage->GetOrigin(ori);
  
  this->EventManager->setWaitCursor(PXTrue);

  if (this->Orientation==3)
    {
      dim[0]=dim[1];
      dim[1]=dim[2];
    }

  int slice=GetSliceNumber();

  int doreset=0;
  int doclip=0;
  if (this->Orientation!=3)
    {
      renderer->SetClipMode(PXTrue);
      doclip=1;
    }
  else
    renderer->SetClipMode(PXFalse);

  if (renderer->GetPlaneMode()!=plane)
    {
      doreset=1;
      renderer->SetPlaneMode(plane);
      renderer->SetFlipMode(0);
    }
  
  renderer->SetClipBounds(slice,slice,slice,sp,ori);
  renderer->SetViewerCoordinates(slice,slice,slice);
      
  if (slice<dim[plane])
    {
      this->UpdateObjectmapChanges();

      vtkpxImageExtract* extract=vtkpxImageExtract::New();
      extract->SetInput(currentImage);
      extract->SetCurrentPlane(plane);
      extract->SetSliceNo(slice);
      extract->SetFrame(frame);
      if (this->Orientation==3)
	extract->SetPolarMode(1);
      else
	extract->SetPolarMode(0);
      extract->Update();
      if (this->currentImageRegion==NULL)
	this->currentImageRegion=vtkImageData::New();
      this->currentImageRegion->ShallowCopy(extract->GetOutput());

      imageSlice->SetInput(currentImageRegion);
      imageSlice->SetVisibility(1);
      imageSlice->Update();

      extract->SetInput(maskImage);
      extract->SetCurrentPlane(plane);
      extract->SetSliceNo(slice);
      extract->SetFrame(frame);
      if (this->Orientation==3)
	extract->SetPolarMode(1);
      else
	extract->SetPolarMode(0);
      extract->Update();

      if (this->maskImageRegion==NULL)
	this->maskImageRegion=vtkImageData::New();
      this->maskImageRegion->ShallowCopy(extract->GetOutput());
      maskImageSlice->SetInput(this->maskImageRegion);
      maskImageSlice->SetVisibility(1);
      maskImageSlice->Update();


      this->UpdateUndoImages();
      this->LastExtractedSlice=extract->GetSliceNo();
      this->LastExtractedOrientation=extract->GetCurrentPlane();
      this->LastExtractedFrame=extract->GetFrame();
      extract->Delete();

    }
  else
    {
      imageSlice->SetVisibility(0);
      imageSlice->Update();

      maskImageSlice->SetVisibility(0);
      maskImageSlice->Update();
    }

  this->UpdateObjectmapSlice();

      
  if (this->Orientation==3)
    renderer->MoveAbove();

  if (doreset)
    {
      renderer->SetClipThickness(currentImage->GetSpacing()[plane]);
      renderer->Reset();
    }
  
  if (doclip)
    renderer->Clip();
  
  this->EventManager->setWaitCursor(PXFalse);
  return TCL_OK;
}
// -------------------------------------------------------------------------
void vtkpxGUI2DImageViewer::ChangeSliceAndFrame(int sl,int fr)
{
  if (!hasImage)
    return;
  
  if (sl!=-1)
      sliceNumber->setValue(sl+offset);
  
  if (fr!=-1 && !this->CurrentImageIsColor)
      frameScale->setValue(fr+1);

  int callframecallback=0;
  if (!this->CurrentImageIsColor)
    {
      int a=(int) frameScale->getValue()-1;
      if (a!=this->CurrentFrame)
	{
	  this->CurrentFrame=a;
	  callframecallback=1;
	}
    }
    
  if (this->Editor!=NULL)
    {
      int bl=(int)sliceNumber->getValue()-offset;
      int plane=2;
      this->Editor->SetPlaneLevel(plane,bl);
      this->Editor->SetFrame(CurrentFrame);
    }
  
  UpdateViewer();

  if (callframecallback==1)
    this->ExecuteFrameChangeCallback();

  if (this->donotRender==0)
    this->UpdateDisplay();
   
}

// -------------------------------------------------------------------------
int vtkpxGUI2DImageViewer::GetSliceNumber()
{
  if (!hasImage)
    return -1;

  return (int) sliceNumber->getValue();
}
// -------------------------------------------------------------------------
void vtkpxGUI2DImageViewer::SetCurrentFrame(int t)
{
  if (!this->CurrentImageIsColor)
    {
      if (t<0)
	t=this->NumberOfFrames-1;
      if (t>this->NumberOfFrames-1)
	t=0;
      this->ChangeSliceAndFrame(-1,t);
    }
}

// -------------------------------------------------------------------------
void vtkpxGUI2DImageViewer::CreateViewerAndSlice()
{
  if (hasImage== PXFalse || this->Initialized==0)
    return;
  
  if (initialized)
    return;
  
  int plane=2;
  //  int frame=0;

  vtkRenderer* ren=vtkRenderer::New();
  ren->SetBackground(0.0,0.1,0.25);
  render_widget->getRenderWindow()->AddRenderer(ren);
    
  renderer=vtkpxGUIRenderer::New();
  renderer->SetPlaneMode(plane);
  renderer->SetOrientation(this->Orientation);
  renderer->SetFlipMode(0);
  renderer->SetNoGUI(PXTrue);
  
  renderer->Initialize(this->EventManager->getMainWindow()->getWidgetName(),ren,0);
  renderer->SetAutoZoom(1.0);
  renderer->SetCallback(this,201);

  imageSlice=vtkpx2DImageSlice::New();
  imageSlice->SetAutoUpdate(0);
  imageSlice->SetInput(currentImage);
  imageSlice->SetLookupTable(mainLookupTable);
 
  maskImageSlice=vtkpx2DImageSlice::New();
  maskImageSlice->SetAutoUpdate(0);
  maskImageSlice->SetInput(currentImage);
  maskImageSlice->SetLookupTable(maskLookupTable);
  maskImageSlice->SetOpacity(this->maskTransparency->getValue()*0.01);
 
#ifndef _WIN32
  renderer->SetClipMode(PXTrue);
#endif

#ifndef _WIN32
  renderer->SetClipMode(PXTrue);
  renderer->SetClipThickness(currentImage->GetSpacing()[2]);
#endif

  renderer->GetRenderer()->AddActor(imageSlice);
  renderer->GetRenderer()->AddActor(maskImageSlice);
  
  
  if (this->Editor)
    this->Editor->SetRenderer(ren);

  double bounds[6];  GetBounds(bounds);
  renderer->SetBounds(bounds);

  

  //  if (index>0)
  //  renderer->SetCameraScale(renderer->GetCameraScale());
  //else
  renderer->Reset();

  initialized=PXTrue;
}
// -------------------------------------------------------------------------
int vtkpxGUI2DImageViewer::ResetAll()
{
  if (!hasImage)
      return 0;

  if (initialized)
    renderer->Reset();

  UpdateDisplay();

  return TCL_OK;
}
// -------------------------------------------------------------------------
int vtkpxGUI2DImageViewer::HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev)
{
  //  int found=-1;
  if (ev==NULL || hasImage==PXFalse)
    return 0;
  
  if (!renderer->InViewport(x,y))
    return 0;
  
  if (this->Editor!=NULL)
    {
      if (nbutton==3  && this->Editor->IsOpen()==0 && this->IsA("vtkpxGUI2DImageViewer")==0)
	{
	  if (state==2)
	    this->Editor->Show();
	  return 1;
	}
    }

  int sendtoeditor=0;
  //  if ( this->MouseToEditor && (  nbutton == 4 || nbutton==7 || ( this->ShiftMouseToEditor==0 && nbutton==1)))
  if ( this->MouseToEditor==1 || ( nbutton == 4 || nbutton==7 || ( this->ShiftMouseToEditor==0 && nbutton==1)))
    {
      if (this->Editor!=NULL)
	sendtoeditor=1;
    }
  
  //  fprintf(stderr,"nbutton=%d sendtoeditor=%d mousetoeditor=%d shiftmouse=%d\n",nbutton,sendtoeditor,
  //  this->MouseToEditor,this->ShiftMouseToEditor);

  if (nbutton  == 1 || nbutton == 4 || nbutton == 7 )
    {
      double px1,py1;
      
      renderer->Get2DClickedPoint(px1,py1,x,y);

      int plane=2;
      int slice=GetSliceNumber();
      double sp[3],ori[3];
      currentImage->GetSpacing(sp);
      currentImage->GetOrigin(ori);
      float slice_sc=slice*sp[plane]+ori[plane];
      
      if (nbutton==7)
	state=3+state;

      if (nbutton==4)
	{
	  if (this->IsA("vtkpxGUI2DImageViewer")==1)
	    {
	      state=6+state;
	    }
	}


      switch(plane)
	{
	case 0:
	  if (sendtoeditor)
	    this->Editor->HandleClickedPoint(slice_sc,px1,py1,1,state);
	  this->LastClickedPointScaled[0]=slice_sc;
	  this->LastClickedPointScaled[1]=px1;
	  this->LastClickedPointScaled[2]=py1;
	  break;
	case 1:
	  if (sendtoeditor)
	    this->Editor->HandleClickedPoint(px1,slice_sc,py1,1,state);
	  this->LastClickedPointScaled[0]=px1;
	  this->LastClickedPointScaled[1]=slice_sc;
	  this->LastClickedPointScaled[2]=py1;
	  break;
	case 2:
	  if (sendtoeditor)
	    this->Editor->HandleClickedPoint(px1,py1,slice_sc,1,state);
	  this->LastClickedPointScaled[0]=px1;
	  this->LastClickedPointScaled[1]=py1;
	  this->LastClickedPointScaled[2]=slice_sc;
	  break;
	}
      
      double lv[3];
      for (int ia=0;ia<=2;ia++)
	lv[ia]=(this->LastClickedPointScaled[ia]-ori[ia])/sp[ia];
      this->SetLastClickedPoint(lv);

      int   dim[3];   
      currentImage->GetDimensions(dim);
      lv[0]=Frange(lv[0],0,dim[0]-1);
      lv[1]=Frange(lv[1],0,dim[1]-1);
      lv[2]=Frange(lv[2],0,dim[2]-1);

      /*      //      if (!sendtoeditor)
      //{
	  fprintf(stderr,"Setting %d %d to 400.0\n",int(lv[0]),int(lv[1]));
	  this->maskImageSlice->GetCurrentImage()->SetScalarComponentFromDouble((int)lv[0],(int)lv[1],0,0,1.0);
	  this->maskImageSlice->UpdateTexture();
	  //}
      
      //      if (this->statusLabel!=NULL)
      //{
      
	  double vl=currentImage->GetScalarComponentAsDouble((int)lv[0],
							    (int)lv[1],
							    (int)lv[2],
							    this->CurrentFrame);
	  sprintf(pxtk_buffer,"* %.2f (%.0f,%.0f,%.0f)",vl,
		  lv[0],lv[1],lv[2]);
	  
	  
	  statusLabel->setText(pxtk_buffer);
	  //}*/
      
      UpdateDisplay();
      return 1;
    }

  if ( nbutton!=1 && nbutton!=4 && nbutton!=7)
    {
      return renderer->HandleMouseButtonEvent(nbutton,state,x,y,ev);
    }

  return 0;
  
}
// -------------------------------------------------------------------------
void vtkpxGUI2DImageViewer::InitControls(PXTkFrame* parwidget,int dohighlight)
{
  controlFrame=new PXTkFrame(this->EventManager,parwidget);
  parwidget->setPackMode(PXTrue);
  parwidget->addChildren("-side bottom -expand false -fill x",controlFrame);

  PXTkFrame* tframe=new PXTkFrame(this->EventManager,controlFrame);
  PXTkFrame* bframe=new PXTkFrame(this->EventManager,controlFrame);
  controlFrame->addChildren("-side top -expand false -fill x",bframe);
  controlFrame->addChildren("-side bottom    -expand true  -fill both -pady 2",tframe);
  
  interpolateMode=new PXTkCheckButton(this->EventManager,bframe,"Interp",205);
  interpolateMode->setState(PXTrue);

  PXTkButton* b1=new PXTkButton(this->EventManager,bframe,"Reset",208);
  bframe->addChildren("-side right -expand t -fill x",interpolateMode,b1);
      
  if (this->ownsLookupTable)
    {
      PXTkButton* b1c=new PXTkButton(this->EventManager,bframe,"ColorMap",209);
      bframe->addChildren("-side right -expand t -fill x",interpolateMode,b1c);
    }
  
  //#ifdef _WIN32
  PXTkDualArrowLabel* zoom=new PXTkDualArrowLabel(this->EventManager,bframe,"Zoom",201,202,0);
  bframe->addChildren("-side right -expand t -fill x",zoom);
  //#endif
  
  this->timeframeControlFrame=new PXTkFrame(this->EventManager,tframe);
  tframe->addChildren("-side left -expand true -fill x -padx 2 ",timeframeControlFrame);
  
  sliceNumber=new PXTkArrowScale(this->EventManager,timeframeControlFrame,"Slice:",206,PXTrue);
  sliceNumber->setLengthWidth(120,15);
  
  maskTransparency=new PXTkArrowScale(this->EventManager,timeframeControlFrame,"Mask:",210,PXTrue);
  maskTransparency->setRange(0.0,100.0);
  maskTransparency->setResolution(1.0);
  maskTransparency->setValue(50.0);
  maskTransparency->setLengthWidth(80,15);


  frameScale=new PXTkArrowScale(this->EventManager,this->timeframeControlFrame,"Frame:",206,PXTrue);
  frameScale->setRange(1,1);
  frameScale->setIncrement(1);
  frameScale->setResolution(1);
  frameScale->setValue(1);
  frameScale->setLengthWidth(10,10);
  timeframeControlFrame->addChildren("-side left -expand f -fill x",sliceNumber->getMainWindow(),
				     frameScale->getMainWindow(),
				     maskTransparency->getMainWindow());
  
  statusLabel=new PXTkLabel(this->EventManager,bframe,"PI:");
  statusLabel->configure("-bg","black");
  statusLabel->configure("-fg","white");
  statusLabel->configure("-width","24");
  
  PXTkButton* b2=new PXTkButton(this->EventManager,bframe,"Snapshot",221);
  bframe->addChildren("-side left -expand false -fill x",statusLabel,b2);
  if (this->CurrentImageIsColor)
    this->ShowFrameControls(0);
  
  sliceNumber->setRange(float(offset),float(10.0+offset));
  sliceNumber->setValue(float(offset));
  sliceNumber->setIncrement(1.0);
  sliceNumber->setResolution(1.0);
}

// -------------------------------------------------------------------------
void vtkpxGUI2DImageViewer::InitViewer(PXTkFrame* parFrame)
{
  render_widget=new PXVTkTkRenderWidget(this->EventManager,parFrame,300);
  render_widget->setWidthHeight(400,400);
  parFrame->setPackMode(PXTrue);
  parFrame->addChildren("-side left -expand true -fill both",render_widget);
  render_widget->bindMouseEvents();
}
// -------------------------------------------------------------------------
void vtkpxGUI2DImageViewer::GetBounds(double bounds[6])
{
  if (!hasImage)
      return;

  int   range[3]; currentImage->GetDimensions(range);
  double sp[3];    currentImage->GetSpacing(sp);
  double ori[3];   currentImage->GetOrigin(ori);

  for (int k=0;k<=2;k++)
      {
	bounds[2*k]  =      ori[k]-0.5*sp[k];
	bounds[2*k+1]=float(range[k]-0.5)*sp[k]+ori[k];
      }
}
// -------------------------------------------------------------------------
char* vtkpxGUI2DImageViewer::GetRenderWidgetName()
{
  if (this->render_widget==NULL)
    return NULL;
  else
    return this->render_widget->getWidgetName();
}
// -------------------------------------------------------------------------
vtkRenderer* vtkpxGUI2DImageViewer::GetRenderer()
{
  if (initialized)
    return renderer->GetRenderer();
  else
    return NULL;
}

vtkpxGUIRenderer* vtkpxGUI2DImageViewer::GetGUIRenderer()
{
  if (initialized)
    return renderer;
  else
    return NULL;
}

char* vtkpxGUI2DImageViewer::GetControlFrameName()
{
  if (this->Initialized==0)
    return NULL;
  
  return this->controlFrame->getWidgetName();
}



PXTkFrame* vtkpxGUI2DImageViewer::GetControlFrame()
{
  if (this->Initialized==0)
    return NULL;
  
  return this->controlFrame;
}

PXTkFrame* vtkpxGUI2DImageViewer::GetTimeSliceFrame()
{
  if (this->Initialized==0)
    return NULL;
  
  return this->timeframeControlFrame;
}


// -------------------------------------------------------------------------
void vtkpxGUI2DImageViewer::ShowFrameControls(int show)
{
  if (!this->Initialized || frameScale==NULL)
    {
      return;
    }
  
  if (show==0)
    {
      frameScale->setLengthWidth(10,10);
      timeframeControlFrame->unmapChild(frameScale->getMainWindow());
      timeframeControlFrame->configure("-width",0);
    }
  else
    {
      frameScale->setLengthWidth(50,10);
      sliceNumber->setLengthWidth(70,10);
      timeframeControlFrame->addChildren("-side left -expand t -fill x",frameScale->getMainWindow());
    }
}
/* -------------------------------------------------------------------------*/
int vtkpxGUI2DImageViewer::UpdateDisplay()
{
  if (this->Initialized && this->donotRender==0)
    {
      if (inexpose==1 || this->EnableRendering==0)
	{
	  ++inexposecount;
	  return TCL_OK;
	}

      inexpose=1;
      PXTkApp::executeTclCommand("update display");
      render_widget->getRenderWindow()->Render();  
      this->LastEvent=-1;
      inexposecount=0;
      inexpose=0;
      return TCL_OK;
    }
  return TCL_OK;
}
// -------------------------------------------------------------------------	    

 
void vtkpxGUI2DImageViewer::CreateMaskImage(vtkImageData* cur_img)
{
  if (this->maskImage==NULL)
    {
      this->maskImage=vtkImageData::New();
    }

  double range[2]; currentImage->GetPointData()->GetScalars()->GetRange(range);

  vtkImageThreshold* thr=vtkImageThreshold::New();
  thr->SetInput(cur_img);
  thr->ThresholdByLower(0.5*(range[0]+range[1]));
  thr->ReplaceInOn();
  thr->ReplaceOutOn();
  thr->SetInValue(0.0);
  thr->SetOutValue(0.0);
  thr->SetOutputScalarTypeToUnsignedChar();
  thr->Update();

  maskImage->ShallowCopy(thr->GetOutput());
  thr->Delete();

  this->LastExtractedSlice=-1;
  this->UpdateViewer();
}

// ------------------------------------------------------------------------
int vtkpxGUI2DImageViewer::ClearMaskImage() 
{
  if ((!hasImage && this->Initialized))
    return 0;
  this->CreateMaskImage(currentImage);
  return 1;
}

void vtkpxGUI2DImageViewer::SetMaskImage(vtkImageData* newmaskimage,int orientation) 
{
  if (newmaskimage==NULL)
    return;
  
  if (this->maskImage==NULL)
    this->maskImage=vtkImageData::New();

  vtkbisImageReslice* resl=vtkbisImageReslice::New();
  resl->OptimizationOff();
  resl->SetInput(newmaskimage);
  resl->SetInformationInput(this->maskImage);
  resl->SetInterpolationMode(0);
  resl->SetBackgroundLevel(0);
  resl->Update();

  this->maskImage->ShallowCopy(resl->GetOutput());
  resl->Delete();
  this->LastExtractedSlice=-1;
  this->UpdateViewer();
}

vtkImageData*  vtkpxGUI2DImageViewer::GetMaskImage()
{
  this->UpdateObjectmapChanges();
  return this->maskImage;
}

float vtkpxGUI2DImageViewer::GetObjectmapTransparency()
{
  if (  this->maskImageSlice==NULL)
    return 0.0;

  return 100.0*this->maskImageSlice->GetOpacity();
}

void vtkpxGUI2DImageViewer::SetObjectmapTransparency(float v)
{
  if (  this->maskImageSlice!=NULL)
    {
      this->maskImageSlice->SetOpacity(float(v)*0.01);
      this->UpdateDisplay();
    }
}

vtkLookupTable*  vtkpxGUI2DImageViewer::GetMaskLookupTable()
{
  return maskLookupTable;
}

