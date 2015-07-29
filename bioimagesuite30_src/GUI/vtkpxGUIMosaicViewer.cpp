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
  Module:    $RCSfile: vtkpxGUIMosaicViewer.cpp,v $
  Language:  C++
  Date:      $Date: 2004/02/03 15:28:38 $
  Version:   $Revision: 1.3 $


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
#include "vtkpxGUIMosaicViewer.h"
#include "vtkpxTalairachTransform.h"
// -------------------------------------------------------------------------
vtkpxGUIMosaicViewer* vtkpxGUIMosaicViewer::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIMosaicViewer");
  if(ret)
      {
	return (vtkpxGUIMosaicViewer*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIMosaicViewer;
}
// -------------------------------------------------------------------------
// Construct object with no children.
vtkpxGUIMosaicViewer::vtkpxGUIMosaicViewer()
{

  this->single_slice_mode=PXFalse;
  this->xy_slice_only=PXFalse;
  this->offset=0;
  this->NumberOfViewers=1;
  this->NumberOfRows=1;
  this->NumberOfColumns=1;
  this->MaximumNumberOfViewers=48;
  this->show_controlframe=1;

  this->highlightGUI=NULL;

  this->timeframeControlFrame=NULL;
  this->frameScale=NULL;
  this->CreateHighlights=0;
  this->extraFrame=NULL;
  this->reverseOrder=NULL;
  this->transposeOrder=NULL;
  this->currentPlane=NULL;
  this->MouseToEditor=0;
  this->ShiftMouseToEditor=0;
  this->Editor=NULL;

  for (int i=0;i<MVP_MAX_VIEWERS;i++)
      initialized[i]=PXFalse;
  this->statusLabel=NULL;
  this->donotRender=0;

  this->inexpose=0;
  this->inexposecount=0;
  this->LastEvent=-1;
  this->showLabels=NULL;

  this->firsttime=1;
  this->olddimensions[0]=-1;
  this->olddimensions[1]=-1;
  this->olddimensions[2]=-1;


}
// -------------------------------------------------------------------------
vtkpxGUIMosaicViewer::~vtkpxGUIMosaicViewer()
{


}
/* -------------------------------------------------------------------------*/
char* vtkpxGUIMosaicViewer::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();
  
  vtkpxGUIComponent::Initialize(name,inside);

  PXTkFrame* manager=(PXTkFrame*)(this->EventManager->getMainWindow());
  PXTkFrame* viewerFrame=new PXTkFrame(this->EventManager,manager);
  manager->setPackMode(PXTrue);

  if (single_slice_mode)
    manager->addChildren("-side top  -expand true -fill both",viewerFrame);
  else
    manager->addChildren("-side left -expand true -fill both",viewerFrame);

  InitControls(viewerFrame,this->CreateHighlights);
  InitViewer(viewerFrame);

  return GetWidgetName();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIMosaicViewer::Update()
{
  UpdateAllViewers();
  UpdateDisplay();
}
// -------------------------------------------------------------------------
void vtkpxGUIMosaicViewer::SetOffset(int of)
{
  if (this->Initialized==0)
      this->offset=of;
}
// -------------------------------------------------------------------------
void vtkpxGUIMosaicViewer::SetXYSliceOnly()
{
  if (this->Initialized==0)
    {
      xy_slice_only=PXTrue;
      this->DisableTalairachButtons();
    }
}

// -------------------------------------------------------------------------
void vtkpxGUIMosaicViewer::SetSingleSliceMode()
{
  if (this->Initialized==0)
    {
      single_slice_mode=PXTrue;
      CreateHighlights=0;
      this->DisableTalairachButtons();
      this->MaximumNumberOfViewers=1;
    }
}


/* -------------------------------------------------------------------------*/
int vtkpxGUIMosaicViewer::InitializeViewer()
{
  if ((!hasImage && this->Initialized))
      return 0;
  
  for (int i=0;i<NumberOfViewers;i++)
    CreateViewerAndSlice(i);
  
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
  //  fprintf(stderr,"Viewer Initialized\n");
}

// -------------------------------------------------------------------------
int vtkpxGUIMosaicViewer::ResetEditor()
{
  if (this->Editor!=NULL)
    this->Editor->SetLookupTable(mainLookupTable);

  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIMosaicViewer::ResetViewer()
{
  //  fprintf(stderr,"Reseting Viewer\n");

  if (!(hasImage && this->Initialized))
      return 0;

  // ------------------------------------------------------
  //           C h e c k    f o r    L o o k u p T a b l e
  // ------------------------------------------------------
  int numc=currentImage->GetNumberOfScalarComponents();
     

  
  // ------------------------------------------------------
  //   Bounds/ Highlights 
  // ------------------------------------------------------
  double bounds[6];  GetBounds(bounds);
  int   range[3];   currentImage->GetDimensions(range); 
  double sp[3];      currentImage->GetSpacing(sp);
  double ori[3];     currentImage->GetOrigin(ori);

  //  fprintf(stderr,"Image Dimensions = %d %d %d\n",range[0],range[1],range[2]);

  if (this->talairachTransform==NULL)
    this->talairachTransform=vtkpxTalairachTransform::New();
      
  this->talairachTransform->SetColinMode(1,this->currentImage);
  this->talairachTransform->Modified();
  this->UpdateAllViewers();
  this->UpdateStatusLabels();


  if (highlightGUI!=NULL)
      {
	for (int ka=0;ka<=2;ka++)
	  {
	    highlightscale[ka*2]->setRange(offset,range[ka]-2.0+float(offset));
	    highlightscale[ka*2+1]->setRange(offset+1.0,range[ka]-1.0+float(offset));
	    highlightscale[ka*2]->setValue(0.0+float(offset));
	    highlightscale[ka*2+1]->setValue(offset+range[ka]-1.0);
	  }
	UpdateHighlights();
      }

  if (frameScale!=NULL)
    {
      if (CurrentImageIsColor==0)
	this->ShowFrameControls((this->NumberOfFrames>1));
      else
	this->ShowFrameControls(0);
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

  //  fprintf(stderr,"This Far (1)\n");

  // THIS IS A HACK for now .........
  if (this->IsA("vtkpxGUIObjectmapMosaicViewer"))
    samesizeasbefore=0;
  
  if (samesizeasbefore==0)
    {
      //      fprintf(stderr,"Into samesizeas before = 0 ");

      if (!single_slice_mode && !xy_slice_only)
	currentPlane->setIndex(2);
      firsttime=0;
      beginSlice->setResolution(1);
      beginSlice->setRange(float(offset),float(numslices-1+offset));
      beginSlice->setValue(offset+dim[2]/2);

  
      if (!single_slice_mode)
	{
	  if (this->NumberOfViewers>0)
	    incrementSlice->setRange(1,numslices/this->NumberOfViewers);
	  else
	    incrementSlice->setRange(1,numslices);
	  incrementSlice->setResolution(1);
	  incrementSlice->setValue(1);
	}

      frameScale->setRange(1,this->NumberOfFrames);
      frameScale->setResolution(1);
      frameScale->setValue(1);

      if (this->NumberOfViewers>0)
	{
	  SetViewports();
	  //	  fprintf(stderr,"This Far samesizeasbefore=%d SetViewp numvie=%d\n",samesizeasbefore,this->NumberOfViewers);
	  
	  for (int kk=0;kk<this->NumberOfViewers;kk++)
	    {
	      imageSlice[kk]->SetCurrentPlane(2);
	      imageSlice[kk]->SetFrame(0);
	      imageSlice[kk]->SetLevel(-1);
	      if (this->Orientation==3)
		{
		  imageSlice[kk]->SetPolarMode(1);
		  renderer[kk]->SetClipMode(PXFalse);
		}
	      else
		{
		  imageSlice[kk]->SetPolarMode(0);
		  renderer[kk]->SetClipMode(PXFalse);
		}
	      imageSlice[kk]->SetColorMode(this->CurrentImageIsColor);
	      imageSlice[kk]->SetInput(currentImage);
	      
	      renderer[kk]->SetBounds(bounds);
	      renderer[kk]->SetClipThickness(currentImage->GetSpacing()[2]);
	      renderer[kk]->SetPlaneMode(2);
	      renderer[kk]->SetCurrentImageOrientation(currentImage,this->Orientation,1);
	      renderer[kk]->Reset();

	    }
	}
    }
  else
    {
      //      fprintf(stderr,"Same stuff\n");
      for (int kk=0;kk<this->NumberOfViewers;kk++)
	{
	  imageSlice[kk]->SetColorMode(this->CurrentImageIsColor);
	  imageSlice[kk]->SetInput(currentImage);
	  renderer[kk]->SetCurrentImageOrientation(currentImage,this->Orientation,0);
	  renderer[kk]->SetClipThickness(currentImage->GetSpacing()[2]);
	  renderer[kk]->SetBounds(bounds);
	}
    }


  if (currentPlane!=NULL)
    {
      int oldstyleorient=vtkpxGUIBaseImageViewer::MapOrientationToOldStyle(this->Orientation);
      //      fprintf(stderr,"This far %d,%d\n",this->Orientation,currentPlane);
      switch (oldstyleorient)
	{
	case 0: // Axial
	  currentPlane->changeLabel(0,"Sagittal-YZ");
	  currentPlane->changeLabel(1,"Coronal-XZ");
	  currentPlane->changeLabel(2,"Axial-XY");
	  break;
	case 1:
	  currentPlane->changeLabel(0,"Sagittal-YZ");
	  currentPlane->changeLabel(1,"Axial-XZ");
	  currentPlane->changeLabel(2,"Coronal-XY");
	  break;
	case 2:
	  currentPlane->changeLabel(0,"Coronal-YZ");
	  currentPlane->changeLabel(1,"Axial-XZ");
	  currentPlane->changeLabel(2,"Sagittal-XY");
	  break;
	case 3: // Polar
	  currentPlane->changeLabel(0,"Short-Axis");
	  currentPlane->changeLabel(1,"Radial");
	  currentPlane->changeLabel(2,"Radial");
	  break;
	}
    }

  if (this->Editor!=NULL)
    {
      int plane=2;
      if (currentPlane!=NULL)
	plane=currentPlane->getIndex();
      this->Editor->SetImage(currentImage,plane,0);
    }



  if (samesizeasbefore==0)
    ChangeSliceIncrement(-1,-1,-1);

  this->UpdateStatusLabels();

  return TCL_OK;
}
// -------------------------------------------------------------------------
int vtkpxGUIMosaicViewer::HandleEvent(int event)
{
  if (event<10)
      return this->EventManager->handleDirectEvent(event);
  
  if (!hasImage && event!=301)
    return TCL_OK;

  if (event==200)
      UpdateDisplay();
  
  if (event==201 || event==202)
    {
      if (hasImage)
	{
	  if (event==202)
	    renderer[0]->Zoom(renderer[0]->GetCameraScale()*1.2,PXFalse);
	  else
	    renderer[0]->Zoom(renderer[0]->GetCameraScale()*0.833,PXFalse);
	  
	  for (int kk=1;kk<MVP_MAX_VIEWERS;kk++)
	    {
	      if (initialized[kk]==PXTrue)
		{
		  renderer[kk]->Zoom(renderer[0]->GetCameraScale(),PXFalse);
		}
	    }
	}
      UpdateDisplay();
    }
  
   if (event==203)
     {
       if (hasImage)
	 {
	   int dim[3];     
	   currentImage->GetDimensions(dim);
	   int plane=2;
	   if (!single_slice_mode && !xy_slice_only )
	     plane=currentPlane->getIndex();
	   int numslices=dim[plane];
	   if (this->Orientation==3)
	     {
	       if (plane==0)
		 numslices=dim[1];
	       else
		 numslices=dim[2];
	     }
	   
	   beginSlice->setRange(offset,numslices-1+offset);
	   if (beginSlice->getValue()>=numslices+offset)
	     beginSlice->setValue(0+offset);
	   
	   if (!single_slice_mode)
	     {
	       incrementSlice->setRange(1,numslices/this->NumberOfViewers);
	       if (incrementSlice->getValue()>=numslices/this->NumberOfViewers)
		 {
		   if (numslices/this->NumberOfViewers>0)
		     incrementSlice->setValue(numslices/this->NumberOfViewers);
		   else
		       incrementSlice->setValue(1);			
		 }
	     }
	   UpdateAllViewers();
	   UpdateDisplay();
	 }
       }


   if (event==205)
     {
       if (hasImage)
	 {
	   PXBool interp=interpolateMode->getState();
	   for (int k=0;k<MVP_MAX_VIEWERS;k++)
	     {
	       if (initialized[k])
		 imageSlice[k]->SetInterpolation((int)interp);
	     }
	 }
       UpdateDisplay();
     }
   
   if (event==206)
     {
       ChangeSliceIncrement(-1,-1,-1);
     }

   if (event==207)
     {
       if (hasImage)
	 {
	   UpdateAllViewers();
	   UpdateDisplay();
	 }
     }
   
   if (event==208)
     ResetAll();
  
   if (event==221)
     this->SaveAsTiff(render_widget->getRenderWindow());
   
   if (event==222)
     this->SetBackgroundColor(render_widget->getRenderWindow());

   /*if (event==221)
     this->ExportAsVRML(render_widget->getRenderWindow());*/
   
   
   if (event==209)
     {
       if (this->ColormapEditor!=NULL && this->hasImage)
	 this->ColormapEditor->Show();
     }
   
   if (event==214 || event==215 || event==216 || event==217 || event==218 || event==219)
     {
       if (event<219)
	 this->SetPresetLookupTable(event-215);
       else
	 this->SetPresetLookupTable(event-213);
     }
   
   if (event==210)
     {
       if (highlightGUI!=NULL)
	 highlightGUI->popupDialog();
     }
   
   if (event==211)
     {
       if (highlightGUI!=NULL)
	 {
	   this->UpdateHighlights();
	   UpdateDisplay();
	 }
     }
   
   if (event==212)
     {
       int dm=0;
       
       if (highlightGUI!=NULL)
	 {
	   if (showhighlight->getState())
	     {
	       dm=1;
	       this->UpdateHighlights();
	     }
	   
	   for (int kk=0;kk<MVP_MAX_VIEWERS;kk++)
	     if (initialized[kk]==PXTrue)
	       imageSlice[kk]->SetDisplayHighlight(dm);
	   UpdateDisplay();
	 }
     }
   
   if (event==213)
     {
	if (hasImage && highlightGUI!=NULL)
	  {
	    int dim[3];     
	    currentImage->GetDimensions(dim);
	    for (int k=0;k<=2;k++)
	      {
		highlightscale[k*2]->setValue(0.0+float(offset));
		highlightscale[k*2+1]->setValue(dim[k]);
	      }
	    this->UpdateHighlights();
	    UpdateDisplay();
	  }
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
	  //  fprintf(stderr,"Setting delayed expose\n");
	  this->RenderDelay(500);
	}
      //      fprintf(stderr,"*%d*",this->LastEvent);
      //UpdateDisplay();
    }
  
  
  /*  if (event==305)
    {
      UpdateDisplay();
      }*/
  
  if (event==401)
    {
      int numr=(int)viewerPresetsRow->getValue();
      int numc=(int)viewerPresetsCol->getValue();
      //      fprintf(stderr,"event=%d, values=%d,%d\n",event,numr,numc);
      this->SetConfiguration(numr,numc);
    }

  if ( (event==402 || event==403) && hasImage)
    {
      if (this->talairachTransform==NULL)
	this->talairachTransform=vtkpxTalairachTransform::New();
      
      int   range[3];   currentImage->GetDimensions(range); 
      //      fprintf(stderr,"Tal %d Image Dimensions = %d %d %d\n",event,range[0],range[1],range[2]);

      if (event==402)
	this->talairachTransform->SetColinMode(1,this->currentImage);
      else
	this->talairachTransform->SetColinMode(-1,this->currentImage);
      this->talairachTransform->Modified();
      this->UpdateAllViewers();
      this->UpdateStatusLabels();
      
    }


  this->LastEvent=event;
  
  return TCL_OK;
  
}

int  vtkpxGUIMosaicViewer::UpdateAllViewers()
{
  if (!hasImage)
    return TCL_ERROR;
  
  return UpdateViewers(0,this->NumberOfViewers-1);
}
// -------------------------------------------------------------------------
int vtkpxGUIMosaicViewer::UpdateViewers(int firstv,int lastv)
{
  if (!hasImage)
      return TCL_ERROR;

  firstv=Irange(firstv,0,this->NumberOfViewers-1);
  lastv=Irange(lastv,0,this->NumberOfViewers-1);

  // Read UI variables 
  int plane=2;
  if (!single_slice_mode && !xy_slice_only )
      plane=currentPlane->getIndex();
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

  for (int k=firstv;k<=lastv;k++)
    {
      int doreset=0;
      int doclip=1;
      if (this->Orientation!=3)
	{
	  renderer[k]->SetClipMode(PXTrue);
	  doclip=1;
	}
      else
	renderer[k]->SetClipMode(PXFalse);

      renderer[k]->SetCameraScale(renderer[0]->GetCameraScale());

      if (renderer[k]->GetPlaneMode()!=plane)
	{
	  doreset=1;
	  renderer[k]->SetPlaneMode(plane);
	  imageSlice[k]->SetCurrentPlane(plane);
	  renderer[k]->SetFlipMode(0);
	}
      
      int slice=GetViewerSlice(k);
      renderer[k]->SetClipBounds(slice,slice,slice,sp,ori);
      renderer[k]->SetViewerCoordinates(slice,slice,slice);
      
      if (slice<dim[plane] && slice>=0)
	{
	  imageSlice[k]->SetColorMode(this->CurrentImageIsColor);
	  imageSlice[k]->SetFrame(frame);
	  imageSlice[k]->SetLevel(slice);
	  imageSlice[k]->SetCurrentPlane(plane);
	  imageSlice[k]->SetDisplayMode(2);
	  imageSlice[k]->Update();
	  renderer[k]->SetViewerCoordinates(slice,slice,slice);
	  char line[250];

  
	  if (!single_slice_mode && !xy_slice_only ) 
	    {
	      int axis;
	      double mni=this->GetMNISliceNo(slice,plane,axis);
	      if (axis==-1)
		{
		  sprintf(line,"Slice=%d/%d (%s)",
			  slice,dim[plane]-1,
			  currentPlane->getLabel(imageSlice[k]->GetCurrentPlane()));
		}
	      else
		{
		  switch (axis) {
		  case 0: sprintf(line,"MNI X=%.0f Slice=%d/%d",mni,slice,dim[plane]-1); break;
		  case 1: sprintf(line,"MNI Y=%.0f Slice=%d/%d",mni,slice,dim[plane]-1); break;
		  case 2: sprintf(line,"MNI Z=%.0f Slice=%d/%d",mni,slice,dim[plane]-1); break;
		  }
		}
	      renderer[k]->SetLabel(line,0.05,0.05,0.73,0.73,0.0);
	    }
	}
      else
	{
	  renderer[k]->SetLabel("None",0.05,0.05,
				0.8,0.8,0.0);
	  imageSlice[k]->SetDisplayMode(1);
	  imageSlice[k]->Update();
	}
      
      if (this->showLabels!=NULL)
	{
	  if (this->showLabels->getState()==PXTrue)
	    renderer[k]->ShowLabel();
	  else
	    renderer[k]->HideLabel();
	}

      if (this->Orientation==3)
	renderer[k]->MoveAbove();

      if (doreset)
	{
	  renderer[k]->SetClipThickness(currentImage->GetSpacing()[plane]);
	  renderer[k]->Reset();
      	}
      
      if (doclip)
	renderer[k]->Clip();

    }
  
  this->EventManager->setWaitCursor(PXFalse);
  return TCL_OK;
}
// -------------------------------------------------------------------------
void vtkpxGUIMosaicViewer::SetNumberOfViewers(int numv)
{
  if (numv<1 || numv>this->MaximumNumberOfViewers)
    return;

  int* sz=render_widget->getRenderWindow()->GetSize();
  double ratio=(float(sz[0])/float(sz[1]));
  double sq=double(numv);
  
  //float nc=sqrt(float(numv)*ratio);
  int num_cols=int(sqrt(ratio*sq));
  if (num_cols==0)
    num_cols++;
  int num_rows=int(numv/num_cols);
  if (num_rows==0)
    num_rows++;
  
  while (num_rows*num_cols<numv)
    {
      if ( (num_rows*(num_cols+1) < (num_rows+1)*num_cols))
	num_cols++;
      else
	num_rows++;
    }
  this->SetConfiguration(num_rows,num_cols);

}

void vtkpxGUIMosaicViewer::SetConfiguration(int numr,int numc)
{

  if (!hasImage)
      return;
  
  //  fprintf(stderr,"In Set Configuration\n");


  while (numc*numr > this->MaximumNumberOfViewers && numc>0 )
    numc=numc-1;
      

  while (numc*numr > this->MaximumNumberOfViewers && numr>0 )
    numr=numr-1;



  int old_num=this->NumberOfViewers;
  this->NumberOfViewers=numr*numc;

  //  fprintf(stderr,"old_num=%d, new=%d\n",old_num,this->NumberOfViewers);

  int createdviewers=0;

  if (this->NumberOfViewers>old_num)
    {
      for (int kk=old_num;kk<this->NumberOfViewers;kk++)
	{
	  if (initialized[kk])
	    {
	      render_widget->getRenderWindow()->AddRenderer(renderer[kk]->GetRenderer());
	    }
	  else
	    {
	      CreateViewerAndSlice(kk);
	      ++createdviewers;
	    }
	}
    }
  else 
    {
      for (int kk=this->NumberOfViewers;kk<old_num;kk++)
	render_widget->getRenderWindow()->RemoveRenderer(renderer[kk]->GetRenderer());
    }
  
  this->NumberOfRows=numr;
  this->NumberOfColumns=numc;

  viewerPresetsRow->setValue((float)this->NumberOfRows,PXFalse);
  viewerPresetsCol->setValue((float)this->NumberOfColumns,PXFalse);
  
  SetViewports();
  if (highlightGUI!=NULL)
    {
      if (showhighlight->getState())
	UpdateHighlights();
    }
  

  UpdateViewers(old_num,this->NumberOfViewers-1);
  for (int i=0;i<this->NumberOfViewers;i++)
    {
      renderer[i]->Reset();
      renderer[i]->Zoom(renderer[0]->GetCameraScale(),PXTrue);
    }

  UpdateDisplay();
  if ( createdviewers)
    {
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
}
// -------------------------------------------------------------------------
void vtkpxGUIMosaicViewer::ChangeSliceIncrement(int sl,int incr,int fr)
{
  if (!hasImage)
    return;

  if (sl!=-1)
    {
      beginSlice->setValue(sl+offset);
    }
  
  if (incr!=-1 && !single_slice_mode)
    {
      incrementSlice->setValue(incr);
    }
  
  if (fr!=-1 && !this->CurrentImageIsColor)
    {
      frameScale->setValue(fr+1);
    }

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
      int bl=(int)beginSlice->getValue()-offset;
      int plane=2;
      if (!single_slice_mode && !xy_slice_only)
	plane=currentPlane->getIndex();

      if (this->NumberOfViewers==1)
	this->Editor->SetPlaneLevel(plane,bl);
      else
	this->Editor->SetPlaneLevel(plane,bl+10000);

      this->Editor->SetFrame(CurrentFrame);
    }
  
  //fprintf(stderr,"Updating Display\n");
  UpdateAllViewers();
  //  fprintf(stderr,"Updating Display Done\n");
  //  UpdateDisplay();

  if (callframecallback==1)
    this->ExecuteFrameChangeCallback();

  if (this->donotRender==0)
    this->UpdateDisplay();

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
int vtkpxGUIMosaicViewer::GetBeginSlice()
{
  if (!hasImage)
    return -1;

  return (int) beginSlice->getValue();
}
// -------------------------------------------------------------------------
int vtkpxGUIMosaicViewer::GetIncrement()
{
  if (!hasImage || single_slice_mode)
    return 1;

  int inc=(int) incrementSlice->getValue();

  if (this->reverseOrder!=NULL)
    {
      if (reverseOrder->getState())
	{
	  return -inc;
	}
    }
  
  return inc;
}
// -------------------------------------------------------------------------

void vtkpxGUIMosaicViewer::SetCurrentFrame(int t)
{

  if (!this->CurrentImageIsColor)
    {
      if (t<0)
	t=this->NumberOfFrames-1;
      if (t>this->NumberOfFrames-1)
	t=0;
      this->ChangeSliceIncrement(-1,-1,t);
    }

}

// -------------------------------------------------------------------------
void vtkpxGUIMosaicViewer::SetViewports()
{
  double dx=1.0/float(this->NumberOfColumns);
  double dy=1.0/float(this->NumberOfRows);


  int dotranspose=0;
  if (this->transposeOrder!=NULL)
    {
      if (this->transposeOrder->getState())
	dotranspose=1;
    }
  
  // fprintf(stderr,"\n Viewer %d x %d dx=%.2f dy=%.2f\n",NumberOfRows,NumberOfColumns,dx,dy);
  
  for (int j=0;j<this->NumberOfRows;j++)
    for (int i=0;i<this->NumberOfColumns;i++)
      {
	float v[4];
	int index=i+j*this->NumberOfColumns;

	if (initialized[index])
	  {
	    if (dotranspose==0)
	      {
		v[0]=i*dx;
		v[1]=((this->NumberOfRows-1)-j)*dy;
		v[2]=v[0]+dx;
		v[3]=v[1]+dy;
	      }
	    else
	      {
		v[0]=j*dy;
		v[2]=v[0]+dy;
		v[1]=((this->NumberOfColumns-1)-i)*dx;
		v[3]=v[1]+dx;
	      }
	    renderer[index]->SetViewport(v[0],v[1],v[2],v[3]);
	    //	    fprintf(stderr,"indx=%d i=%d j=%d v=%.2f:%.2f %.2f:%.2f\n",index,i,j,v[0],v[1],v[2],v[3]);
	  }
      }

  for (int k=this->NumberOfViewers;k<MVP_MAX_VIEWERS;k++)
    {
      if (initialized[k]==PXTrue)
	renderer[k]->SetViewport(0.9999,0.9999,1.0,1.0);
    }
}
// -------------------------------------------------------------------------
void vtkpxGUIMosaicViewer::CreateViewerAndSlice(int index)
{
  if (hasImage== PXFalse || this->Initialized==0)
      return;

  index=Irange(index,0,this->MaximumNumberOfViewers-1);
  if (initialized[index])
      return;
  
  //  fprintf(stderr,"MOS Creating Viewer And Slice %d\n",index);

  int plane=2;
  if (!single_slice_mode && !xy_slice_only)
      plane=currentPlane->getIndex();

  int frame=0;

  vtkRenderer* ren=vtkRenderer::New();
  //ren->TwoSidedLightingOff();
  render_widget->getRenderWindow()->AddRenderer(ren);
    
  renderer[index]=vtkpxGUIRenderer::New();
  renderer[index]->SetPlaneMode(plane);
  renderer[index]->SetFlipMode(0);

  if (index>0 || this->single_slice_mode==PXFalse)
    renderer[index]->SetNoGUI(PXTrue);

  if (index>0)
    ren->SetBackground(renderer[0]->GetRenderer()->GetBackground());
  
  renderer[index]->Initialize(this->EventManager->getMainWindow()->getWidgetName(),ren,0);
  renderer[index]->SetAutoZoom(1.0);
  renderer[index]->SetCallback(this,201);



  imageSlice[index]=vtkpxImageSlice::New();
  imageSlice[index]->SetAutoUpdate(0);
  imageSlice[index]->SetColorMode(this->CurrentImageIsColor);
  imageSlice[index]->SetInput(currentImage);
  imageSlice[index]->SetLookupTable(mainLookupTable);

  if (this->Orientation==3)
    {
      imageSlice[index]->SetPolarMode(1);
    }
  else
    {
      imageSlice[index]->SetPolarMode(0);
    }

#ifndef _WIN32
  renderer[index]->SetClipMode(PXTrue);
#endif
  imageSlice[index]->SetCurrentPlane(2);
  imageSlice[index]->SetDisplayMode(1);

  if (highlightGUI!=NULL)
    {
      if (showhighlight->getState())
	imageSlice[index]->SetDisplayHighlight(1);
    }
#ifndef _WIN32
  renderer[index]->SetClipMode(PXTrue);
  renderer[index]->SetClipThickness(currentImage->GetSpacing()[2]);
#endif
  renderer[index]->GetRenderer()->AddActor(imageSlice[index]);


  
  if (this->Editor)
    this->Editor->SetRenderer(ren,index);


  double bounds[6];
  GetBounds(bounds);
  renderer[index]->SetBounds(bounds);
  renderer[index]->SetCurrentImageOrientation(currentImage,this->Orientation,1);
  renderer[index]->Reset();

  if (index>0)
    renderer[index]->SetCameraScale(renderer[0]->GetCameraScale());

  initialized[index]=PXTrue;
}
// -------------------------------------------------------------------------
int  vtkpxGUIMosaicViewer::GetViewerSlice(int v)
{
  v=Irange(v,0,this->NumberOfViewers-1);
  
  if (!this->Initialized)
    {
      return 0;
    }
  
  int firstslice=(int)beginSlice->getValue()-offset;
  int incr=1;

  if (firstslice<0)
      firstslice=0;
  

  int increment=this->GetIncrement();
  int final=firstslice+v*increment;
  //  fprintf(stderr,"v=%d firstslice=%d incr=%d final=%d\n",v,firstslice,incr,final);
  return final;
}

// -------------------------------------------------------------------------
int vtkpxGUIMosaicViewer::ResetAll()
{
  if (!hasImage)
      return 0;

  this->EventManager->setWaitCursor(PXTrue);

  for (int k=0;k<MVP_MAX_VIEWERS;k++)
    {
      if (initialized[k])
	{
	  renderer[k]->Reset();
	}
    }
  UpdateDisplay();
  this->EventManager->setWaitCursor(PXFalse);
  return TCL_OK;
}
// -------------------------------------------------------------------------
int vtkpxGUIMosaicViewer::HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev)
{
  int found=-1;
  if (ev==NULL || hasImage==PXFalse)
    return 0;
  
  for (int i=0;i<this->NumberOfViewers;i++)
    {
      if (renderer[i]->InViewport(x,y))
	{
	  found=i;
	  i=this->NumberOfViewers;
	}
    }
  
  if (found==-1)
    return 0;
  
  if (this->Editor!=NULL)
    {
      if (nbutton==3  && this->Editor->IsOpen()==0 && this->IsA("vtkpxGUISplineEditor")==0)
	{
	  if (state==2)
	    this->Editor->Show();
	  return 1;
	}
    }

  int sendtoeditor=0;
  if ( this->MouseToEditor && (  nbutton == 4 || nbutton==7 || ( this->ShiftMouseToEditor==0 && nbutton==1)))
    {
      if (this->Editor!=NULL)
	sendtoeditor=1;
    }

  if (nbutton  == 1 || nbutton == 4 || nbutton == 7 )
    {
      double px1,py1;
      
      renderer[found]->Get2DClickedPoint(px1,py1,x,y);
	
      int plane=2;
      if (!single_slice_mode && !xy_slice_only)
	plane=currentPlane->getIndex();
      
      int slice=GetViewerSlice(found);
      double sp[3],ori[3];
      currentImage->GetSpacing(sp);
      currentImage->GetOrigin(ori);
      double slice_sc=double(slice)*sp[plane]+ori[plane];
      //      fprintf(stderr,"slice_sc=%f (o=%f,s=%f)slice=%d\n\n",slice_sc,ori[plane],sp[plane],slice);
      
      if (nbutton==7)
	state=3+state;

      if (nbutton==4)
	{
	  if (this->ShiftMouseToEditor==0 && this->IsA("vtkpxGUISplineEditor")==1)
	    state=6+state;
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
      
      this->UpdateStatusLabels();

      UpdateDisplay();
      this->ExecuteCoordinatesChangeCallback();
      return 1;
    }

  if ( nbutton!=1 && nbutton!=4 && nbutton!=7)
    {
      return renderer[found]->HandleMouseButtonEvent(nbutton,state,x,y,ev);
    }

  return 0;
  
}
// -------------------------------------------------------------------------
void  vtkpxGUIMosaicViewer::SetShowControls(int show)
{
  show=(show>0);
  if (show==this->show_controlframe)
    return;

  this->show_controlframe=show;

  PXTkFrame* fr=(PXTkFrame*)controlFrame->getGadgetParent();
  if (this->controlFrame!=NULL && fr!=NULL)
    {
      if (this->show_controlframe==1)
	{
	  if (!single_slice_mode)
	    fr->addChildren("-side right -expand false",controlFrame);
	  else
	    fr->addChildren("-side bottom -expand false -fill x",controlFrame);
	}
      else
	fr->unmapChild(controlFrame);
    }
}
// -------------------------------------------------------------------------
void vtkpxGUIMosaicViewer::InitControls(PXTkFrame* parwidget,int dohighlight)
{
  controlFrame=new PXTkFrame(this->EventManager,parwidget);
  parwidget->setPackMode(PXTrue);

  
  if (!single_slice_mode)
    {
      parwidget->addChildren("-side right -expand false",controlFrame);

      PXTkFrame* iframe=new PXTkFrame(this->EventManager,controlFrame);
      interpolateMode=new PXTkCheckButton(this->EventManager,iframe,"Interpolation",205);
      interpolateMode->setState(PXTrue);

      showLabels=new PXTkCheckButton(this->EventManager,iframe,"Labels",207);
      showLabels->setState(PXTrue);

      if (!this->createTalairachButtons)
	{
	  if (this->showLabels!=NULL)
	    controlFrame->addChildren("-side top -expand false -fill x",showLabels);
	  controlFrame->addChildren("-side top -expand false -fill x",interpolateMode);
	}
      else
	{
	  if (this->showLabels!=NULL)
	    controlFrame->addChildren("-side left -expand false -fill x",showLabels);
	  controlFrame->addChildren("-side left -expand false -fill x",interpolateMode);
	}

      controlFrame->addChildren("-side top -expand false",iframe);

      PXTkDualArrowLabel* zoom=new PXTkDualArrowLabel(this->EventManager,controlFrame,"Zoom",201,202,0);


      if (!xy_slice_only)
	{
	  currentPlane=new PXTkOptionMenu(this->EventManager,controlFrame,"YZ",203);
	  currentPlane->addOption("XZ");
	  currentPlane->addOption("XY");
	  currentPlane->setIndex(2);
	  controlFrame->addChildren("-side top -expand false -fill x",currentPlane);
	}

      statusLabel=new PXTkLabel(this->EventManager,controlFrame,"PI:");
      statusLabel->configure("-bg","black");
      statusLabel->configure("-fg","white");
      statusLabel->configure("-width","25");
      controlFrame->addChildren("-side top -expand false -fill x -padx 2",statusLabel);

      if (this->createTalairachButtons)
	{
	  PXTkFrame* tframe2=new PXTkFrame(this->EventManager,controlFrame);
	  controlFrame->addChildren("-side top -expand true -fill x",tframe2);
	  
	  PXTkButton*   talairachButton[2];
	  talairachButton[0]=new PXTkButton(this->EventManager,tframe2,"Tal",402);
	  talairachButton[0]->configure("-pady","0");
	  talairachButton[0]->configure("-padx","1");
	  talairachButton[0]->configure("-fg","red");
	  talairachButton[1]=new PXTkButton(this->EventManager,tframe2,"Tal NEU",403);
	  
	  talairachButton[1]->configure("-pady","0");
	  talairachButton[1]->configure("-padx","1");
	  
	  
	  mniLabel=new PXTkLabel(this->EventManager,tframe2,"");
	  mniLabel->configure("-bg","black");
	  mniLabel->configure("-fg","white");
	  mniLabel->configure("-width","25");
	  mniLabel->configure("-padx","0");
	  //	  tframe2->addChildren("-side right -expand false",talairachButton[1],talairachButton[0]);
	  tframe2->addChildren("-side left -expand true -fill x -padx 2",mniLabel);
	}

      viewerPresetsRow=new PXTkArrowScale(this->EventManager,controlFrame,"Rows:",401);
      viewerPresetsRow->setRange(1,8);
      viewerPresetsRow->setIncrement(1);
      viewerPresetsRow->setResolution(1);
      viewerPresetsRow->setValue(1);
      viewerPresetsRow->setLengthWidth(90,10);

      viewerPresetsCol=new PXTkArrowScale(this->EventManager,controlFrame,"Cols:",401);
      viewerPresetsCol->setRange(1,12);
      viewerPresetsCol->setIncrement(1);
      viewerPresetsCol->setResolution(1);
      viewerPresetsCol->setValue(1);
      viewerPresetsCol->setLengthWidth(90,10);


      controlFrame->addChildren("-side top -expand true -fill x",viewerPresetsRow->getMainWindow(),viewerPresetsCol->getMainWindow());

      this->extraFrame=new PXTkFrame(this->EventManager,controlFrame);


      beginSlice=new PXTkArrowScale(this->EventManager,controlFrame,"First Slice",206,PXTrue);
      beginSlice->setLengthWidth(90,10);
      controlFrame->addChildren("-side top -expand t -fill x",this->extraFrame,beginSlice->getMainWindow());

      incrementSlice=new PXTkArrowScale(this->EventManager,controlFrame,"Increment",206,PXTrue);
      incrementSlice->setRange(1,20);
      incrementSlice->setIncrement(1);
      incrementSlice->setResolution(1);
      incrementSlice->setValue(1);
      incrementSlice->setLengthWidth(90,10);
      PXTkFrame* ordFrame=new PXTkFrame(this->EventManager,controlFrame);

      controlFrame->addChildren("-side top -expand t -fill x",incrementSlice->getMainWindow(),ordFrame);

      reverseOrder=new PXTkCheckButton(this->EventManager,ordFrame,"Reverse Inc",206);
      transposeOrder=new PXTkCheckButton(this->EventManager,ordFrame,"Transpose",401);
      ordFrame->addChildren("-side left -expand t -fill x",reverseOrder,transposeOrder);

      this->timeframeControlFrame=new PXTkFrame(this->EventManager,controlFrame);
      controlFrame->addChildren("-side top -expand t -fill x",timeframeControlFrame);
      
      frameScale=new PXTkArrowScale(this->EventManager,this->timeframeControlFrame,"Frame No:",206,PXTrue);
      frameScale->setRange(1,1);
      frameScale->setIncrement(1);
      frameScale->setResolution(1);
      frameScale->setValue(1);
      frameScale->setLengthWidth(10,10);
      timeframeControlFrame->addChildren("-side top -expand t -fill x",frameScale->getMainWindow());
      
      if (this->ownsLookupTable)
	{
	  PXTkFrame* colFrame=new PXTkFrame(this->EventManager,controlFrame);
	  PXTkButton* b1c0=new PXTkButton(this->EventManager,colFrame,"St",214);
	  b1c0->configure("-padx",0);
	  PXTkButton* b1c=new PXTkButton(this->EventManager,colFrame,"Nr",215);
	  b1c->configure("-padx",0);
	  PXTkButton* b1d=new PXTkButton(this->EventManager,colFrame,"F1",216);
	  b1d->configure("-padx",0);
	  PXTkButton* b1e=new PXTkButton(this->EventManager,colFrame,"F2",217);
	  b1e->configure("-padx",0);
	  PXTkButton* b1f1=new PXTkButton(this->EventManager,colFrame,"F4",219);
	  b1f1->configure("-padx",0);
	  PXTkButton* b1g=new PXTkButton(this->EventManager,colFrame,"Cmap",209);
	  b1g->configure("-padx",0);
	  colFrame->addChildren("-side left -expand true -fill x",b1c0,b1c);
	  colFrame->addChildren("-side left -expand true -fill x",b1d,b1e);
	  colFrame->addChildren("-side right -expand true -fill both",b1g,b1f1);
	  controlFrame->addChildren("-side top -expand true -fill x",colFrame);
	}
      
      PXTkFrame* botFrame=new PXTkFrame(this->EventManager,controlFrame);
      PXTkButton* b1=new PXTkButton(this->EventManager,botFrame,"Reset",208);
      PXTkButton* b2=new PXTkButton(this->EventManager,botFrame,"Snap",221);



      botFrame->addChildren("-side left -expand true -fill x ",b1,b2);
      controlFrame->addChildren("-side top -expand true -fill x",zoom,botFrame);
    }
  else
    {
      parwidget->addChildren("-side bottom -expand false -fill x",controlFrame);
      interpolateMode=new PXTkCheckButton(this->EventManager,controlFrame,"Interp",205);
      interpolateMode->setState(PXTrue);

      PXTkButton* b1=new PXTkButton(this->EventManager,controlFrame,"Reset",208);
      controlFrame->addChildren("-side right -expand t -fill x",interpolateMode,b1);
      
      if (this->ownsLookupTable)
	{
	  PXTkButton* b1c=new PXTkButton(this->EventManager,controlFrame,"ColorMap",209);
	  controlFrame->addChildren("-side right -expand t -fill x",interpolateMode,b1c);
	}

      //#ifdef _WIN32
      PXTkDualArrowLabel* zoom=new PXTkDualArrowLabel(this->EventManager,controlFrame,"Zoom",201,202,0);
      controlFrame->addChildren("-side right -expand t -fill x",zoom);
      //#endif

      this->timeframeControlFrame=new PXTkFrame(this->EventManager,controlFrame);
      controlFrame->addChildren("-side left -expand f -fill x -padx 2 ",timeframeControlFrame);

      this->extraFrame=new PXTkFrame(this->EventManager,controlFrame);

      beginSlice=new PXTkArrowScale(this->EventManager,timeframeControlFrame,"Slice:",206,PXTrue);
      beginSlice->setLengthWidth(120,15);
     
      frameScale=new PXTkArrowScale(this->EventManager,this->timeframeControlFrame,"Frame:",206,PXTrue);
      frameScale->setRange(1,1);
      frameScale->setIncrement(1);
      frameScale->setResolution(1);
      frameScale->setValue(1);
      frameScale->setLengthWidth(10,10);
      timeframeControlFrame->addChildren("-side left -expand f -fill x",
					 this->extraFrame,
					 beginSlice->getMainWindow(),
					 frameScale->getMainWindow());

      statusLabel=new PXTkLabel(this->EventManager,controlFrame,"PI:");
      statusLabel->configure("-bg","black");
      statusLabel->configure("-fg","white");
      statusLabel->configure("-width","24");

      PXTkButton* b2=new PXTkButton(this->EventManager,controlFrame,"Snapshot",221);

      controlFrame->addChildren("-side left -expand false -fill x -padx 0",statusLabel,b2);
    }

  if (this->CurrentImageIsColor)
    this->ShowFrameControls(0);

  beginSlice->setRange(float(offset),float(10.0+offset));
  beginSlice->setValue(float(offset));
  beginSlice->setIncrement(1.0);
  beginSlice->setResolution(1.0);

  if (dohighlight && !single_slice_mode)
      {
	PXTkButton* b1d=new PXTkButton(this->EventManager,controlFrame,"Highlight",210);
	controlFrame->addChildren("-side bottom -expand f -fill x",b1d);
	
	highlightGUI=new PXTkDialog(this->EventManager,controlFrame,"Highlight Control",PXFalse);
	highlightGUI->initDisplay();
	
	PXTkFrame* highlightFrame=(PXTkFrame*)highlightGUI->getMainWindow();
	highlightFrame->setPackMode(PXFalse);
	
	for (int kk=0;kk<=2;kk++)
	    for (int jj=0;jj<=1;jj++)
		{
		  int index=kk*2+jj;
		  
		  switch (index)
		      {
		      case 0:
			strcpy(pxtk_buffer4,"X-Min");	  break;
		      case 1:
			strcpy(pxtk_buffer4,"X-Max");	  break;
		      case 2:
			strcpy(pxtk_buffer4,"Y-Min");	  break;
		      case 3:
			strcpy(pxtk_buffer4,"Y-Max");	  break;
		      case 4:
			strcpy(pxtk_buffer4,"Z-Min");	  break;
		      case 5:
			strcpy(pxtk_buffer4,"Z-Max");	  break;
		      }
		  
		  highlightscale[index]=new PXTkArrowScale(this->EventManager,highlightFrame,pxtk_buffer4,211,PXTrue);
		  highlightscale[index]->setRange(offset,offset+10);
		  highlightscale[index]->setValue(offset);
		  highlightscale[index]->setIncrement(1.0);
		  highlightscale[index]->setResolution(1.0);
		  highlightscale[index]->setLengthWidth(90,10);
		  highlightFrame->gridRowColumnChild(highlightscale[index]->getMainWindow(),kk+1,jj);
		}
	
	showhighlight=new PXTkCheckButton(this->EventManager,highlightFrame,"Show",212);
	highlightFrame->gridRowColumnChild(showhighlight,0,0,1,1);
	
	PXTkButton* b51=new PXTkButton(highlightGUI,highlightFrame,"Reset",213); 
	highlightFrame->gridRowColumnChild(b51,0,1,1,1);
	
	PXTkButton* b5=new PXTkButton(highlightGUI,highlightFrame,"Close",3); 
	highlightFrame->gridRowColumnChild(b5,4,0,1,2);
      }

}

// -------------------------------------------------------------------------
void vtkpxGUIMosaicViewer::InitViewer(PXTkFrame* parFrame)
{
  render_widget=new PXVTkTkRenderWidget(this->EventManager,parFrame,300);
  render_widget->setWidthHeight(400,400);
  parFrame->setPackMode(PXTrue);
  parFrame->addChildren("-side left -expand true -fill both",render_widget);
  render_widget->bindMouseEvents();
}
// -------------------------------------------------------------------------
void vtkpxGUIMosaicViewer::GetBounds(double bounds[6])
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
void vtkpxGUIMosaicViewer::UpdateHighlights()
{
  if (highlightGUI==NULL)
      return;

  int hlevels[6];

  for (int k=0;k<=2;k++)
      {
	HighlightLevels[k*2]=int(highlightscale[k*2]->getValue());
	HighlightLevels[k*2+1]=int(highlightscale[k*2+1]->getValue());
	if ( HighlightLevels[k*2+1] <=	HighlightLevels[k*2])
	  {
	    highlightscale[k*2+1]->setValue(HighlightLevels[k*2]+1);
	    HighlightLevels[k*2+1]=int(highlightscale[k*2+1]->getValue());
	  }
	hlevels[k*2]=int(highlightscale[k*2]->getValue()-float(offset));
	hlevels[k*2+1]=int(highlightscale[k*2+1]->getValue()-float(offset));
      }
 
  if (showhighlight->getState() && hasImage)
      {
	for (int kk=0;kk<MVP_MAX_VIEWERS;kk++)
	    if (initialized[kk]==PXTrue)
		imageSlice[kk]->SetHighlightBounds(hlevels[0],
						   hlevels[1],
						   hlevels[2],
						   hlevels[3],
						   hlevels[4],
						   hlevels[5]);
      }
}
// -------------------------------------------------------------------------
void vtkpxGUIMosaicViewer::SetHighlightLevels(int p1,int p2,int p3,int p4,int p5,int p6)
{
  if (p1>=0)   HighlightLevels[0]=p1;
  if (p2>=0)   HighlightLevels[1]=p2;
  if (p3>=0)   HighlightLevels[2]=p3;
  if (p4>=0)   HighlightLevels[3]=p4;
  if (p5>=0)   HighlightLevels[4]=p5;
  if (p6>=0)   HighlightLevels[5]=p6;

  // Force Callback 
  if (highlightGUI!=NULL)
    {
      for (int i=0;i<=5;i++)
	highlightscale[i]->setValue(HighlightLevels[i]);
      showhighlight->setState(PXTrue);
      UpdateHighlights();
      UpdateDisplay();
    }
  
}

char* vtkpxGUIMosaicViewer::GetRenderWidgetName()
{
  if (this->render_widget==NULL)
    return NULL;
  else
    return this->render_widget->getWidgetName();
}
// -------------------------------------------------------------------------


vtkRenderer* vtkpxGUIMosaicViewer::GetRenderer(int i)
{
  i=Irange(i,0,MVP_MAX_VIEWERS-1);
  if (initialized[i])
      return renderer[i]->GetRenderer();
  else
      return NULL;
}

vtkpxGUIRenderer* vtkpxGUIMosaicViewer::GetGUIRenderer(int i)
{
  i=Irange(i,0,MVP_MAX_VIEWERS-1);
  if (initialized[i])
    return renderer[i];
  else
    return NULL;
}

char* vtkpxGUIMosaicViewer::GetControlFrameName()
{
  if (this->Initialized==0)
    return NULL;
  
  return this->controlFrame->getWidgetName();
}



PXTkFrame* vtkpxGUIMosaicViewer::GetControlFrame()
{
  if (this->Initialized==0)
    return NULL;
  
  return this->controlFrame;
}

// -------------------------------------------------------------------------
void vtkpxGUIMosaicViewer::ShowFrameControls(int show)
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
      beginSlice->setLengthWidth(70,10);
      timeframeControlFrame->addChildren("-side left -expand t -fill x",frameScale->getMainWindow());
    }
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIMosaicViewer::UpdateDisplay()
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
int vtkpxGUIMosaicViewer::UpdateStatusLabels()
{
  double pos[3]; this->GetLastClickedPointScaled(pos);
  double sp[3];    currentImage->GetSpacing(sp);
  double ori[3];   currentImage->GetOrigin(ori);
  int    dim[3];   currentImage->GetDimensions(dim);

  int level[3];

  for (int ia=0;ia<=2;ia++)
    {
      level[ia]=int((pos[ia]-ori[ia])/sp[ia]+0.5);
      if (level[ia]<0)
	level[ia]=0;
      else if (level[ia]>=dim[ia])
	level[ia]=dim[ia]-1;

      pos[ia]=double(level[ia])*sp[ia]+ori[ia];
      
    }

  if (this->createTalairachButtons && this->talairachTransform!=NULL)
    {
      this->DisplayNeuroCoordinates(pos,level,statusLabel,mniLabel);
    }
  else
    {
      double vl=0.0;
      if (this->valueImage!=NULL)
	vl=valueImage->GetScalarComponentAsDouble(level[0],level[1],level[2],this->CurrentFrame);
      else
	vl=currentImage->GetScalarComponentAsDouble(level[0],level[1],level[2],this->CurrentFrame);

      std::string objlabel=this->GetObjectmapValueAsText(level[0],level[1],level[2]);
      sprintf(pxtk_buffer,"(%d %d %d) %s %s",level[0],level[1],level[2],
	      this->GetFormatedNumber(vl,3),
	      objlabel.c_str());
      statusLabel->setText(pxtk_buffer);
      if (this->createTalairachButtons)
	mniLabel->setText("");

    }
  return 1;
}

