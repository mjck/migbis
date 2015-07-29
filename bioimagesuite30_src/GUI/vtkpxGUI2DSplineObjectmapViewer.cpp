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
  Module:    $RCSfile: vtkpxGUI2DSplineObjectmapViewer.cpp,v $
  Language:  C++
  Date:      $Date: 2002/12/18 16:11:14 $
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

#include "vtkpxGUI2DSplineObjectmapViewer.h"
#include "vtkObjectFactory.h"
#include "vtkpxImageReplaceSlice.h"
#include "vtkpxGUI2DImageEditor.h"
#include "nrtk_iwidget_tabnotebook.h"

vtkpxGUI2DSplineObjectmapViewer* vtkpxGUI2DSplineObjectmapViewer::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUI2DSplineObjectmapViewer");
  if(ret)
      {
	return (vtkpxGUI2DSplineObjectmapViewer*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUI2DSplineObjectmapViewer;
}

// Construct object with no children.
vtkpxGUI2DSplineObjectmapViewer::vtkpxGUI2DSplineObjectmapViewer()
{
  this->CreateHighlights=0;
  this->SplineControl=NULL;
  this->NumberOfSplines=50;
  this->ColorMode=0;
  this->ForceNoAutoUpdate=0;
  this->sliceChangeCallbackName=NULL;
  this->EnableSliceChangeCallback=1;
  this->SplineControl=NULL;
  this->ObjectmapControl=NULL;
  this->DefaultScaleMode=4;

}
/* -------------------------------------------------------------------------*/
vtkpxGUI2DSplineObjectmapViewer::~vtkpxGUI2DSplineObjectmapViewer()
{
  if (this->SplineControl)
    this->SplineControl->Delete();

  if (this->ObjectmapControl)
    this->ObjectmapControl->Delete();

  if (this->sliceChangeCallbackName!=NULL)
    delete [] this->sliceChangeCallbackName;



}
/* -------------------------------------------------------------------------*/
void vtkpxGUI2DSplineObjectmapViewer::SetNumberOfSplines(int a)
{
  if (this->Initialized==1)
      return;
  
  this->NumberOfSplines=Irange(a,1,SP_MAX_COLLECTIONS);
  return;
}

/* -------------------------------------------------------------------------*/
char* vtkpxGUI2DSplineObjectmapViewer::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();
  
  vtkpxGUIComponent::Initialize(name,inside);

  PXTkFrame* manager=(PXTkFrame*)(this->EventManager->getMainWindow());

  PXTkFrame* tframe=new PXTkFrame(this->EventManager,manager);
  PXTkFrame* bframe=new PXTkFrame(this->EventManager,manager);


  manager->setPackMode(PXTrue);
  manager->addChildren("-side bottom -expand false -fill x",bframe);
  manager->addChildren("-side top    -expand true  -fill both -pady 2",tframe);

  PXTkFrame* v_frame=new PXTkFrame(this->EventManager,tframe);
  NRTkIWTabNotebook* tab=new NRTkIWTabNotebook(this->EventManager,tframe,"n",300,500);
  tframe->addChildren("-side right -expand false -fill y",tab->getMainWindow());
  tframe->addChildren("-side left -expand true -fill both",v_frame);


  PXTkFrame* fillFrame=tab->addPage("Objectmap");
  fillFrame->configure("-height",200);
  fillFrame->configure("-bg","blue");
  this->CreateObjectmapControl(fillFrame);

  InitControls(bframe,this->CreateHighlights);
  this->ShowFrameControls(0);
  InitViewer(v_frame);

  this->SplineControl=vtkpxGUISplineControl::New();
  this->SplineControl->SetShowGhostControls(0);
  this->SplineControl->SetNumberOfSplines(this->NumberOfSplines);
  this->SplineControl->SetSlaveMode(1);
  this->SplineControl->SetDefaultScaleMode(this->DefaultScaleMode);
  this->SetEditor(this->SplineControl);
  this->SetMouseToEditor(1);
  if (this->EventParent!=NULL && this->CallbackNo>0)
    this->SplineControl->SetCallback(this,800);

  
  PXTkFrame* spFrame=tab->addPage("Spline");
  this->SplineControl->Initialize(spFrame->getWidgetName(),1);

  tab->viewPage("Objectmap");
  
  if (!inside)
    {
      PXTkFrame* bottomButtonFrame=this->GetTimeSliceFrame();
      PXTkButton* closeB;
      if (this->EventParent!=NULL || this->CallbackName!=NULL)
	closeB=new PXTkButton(this->EventManager,bottomButtonFrame,"Close",3);
      else
	closeB=new PXTkButton(this->EventManager,bottomButtonFrame,"Exit",1);
      bottomButtonFrame->addChildren("-side right -expand false -fill x -padx 10",closeB);
    }

  this->ShowFrameControls(0);

  return GetWidgetName();
}
/* -------------------------------------------------------------------------*/
int vtkpxGUI2DSplineObjectmapViewer::HandleEvent(int event)
{
  switch(event)
    {
    case 800:
      if (this->EventParent!=NULL && this->CallbackNo>0)
	this->EventParent->HandleEvent(CallbackNo+1);
      else
	if (this->CallbackName!=NULL)
	  PXTkApp::executeTclCommand(this->CallbackName);
      break;
      
    case 806:
      this->FillObjectmap(this->ObjectmapControl->GetCurrentColor(),1);
      break;
      
    case 810:
      this->FillObjectmap(this->ObjectmapControl->GetCurrentColor(),0);
      break;
      
      
    case 802:
      {
	int sl=vtkpxGUI2DImageViewer::GetSliceNumber();
	this->LastExtractedSlice=-1;
	this->SetSlice(sl);
      }
      break;
      
    default:
      vtkpxGUI2DImageViewer::HandleEvent(event);
    }
    
  
  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI2DSplineObjectmapViewer::SetSlice(int sl)
{
  if (!hasImage)
    return;

  int dim[3];
  currentImage->GetDimensions(dim);
  
  if (sl<0)
    sl=dim[2]/2;
  else
    sl=Irange(sl,0,dim[2]-1);
 
  int a=this->ForceNoAutoUpdate;
  this->ForceNoAutoUpdate=1;
  this->ChangeSliceAndFrame(sl,-1);
  this->ForceNoAutoUpdate=a;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI2DSplineObjectmapViewer::ChangeSliceAndFrame(int sl,int fr)
{

  vtkpxGUI2DImageViewer::ChangeSliceAndFrame(sl,fr);
  PXTkApp::executeTclCommand("update idletasks");

  int nc=currentImage->GetNumberOfScalarComponents();
  if (nc==1)
    sprintf(pxtk_buffer,"Spline/ObjectmapEditor Slice=%d",
	    this->GetBeginSlice());
  else
    sprintf(pxtk_buffer,"Spline/ObjectmapEditor Slice=%d Frame=%d",
	    this->GetBeginSlice(),this->GetCurrentFrame()+1);
  this->EventManager->setTitle(pxtk_buffer);
  
  // Something here about objectmapcontrol ADD
  this->ObjectmapControl->UpdateFromViewer(0);
  // -----------------------------------------


  this->ExecuteSliceChangeCallback();
}
/* -------------------------------------------------------------------------*/
// Description:
// Enable/Disable Spline
void vtkpxGUI2DSplineObjectmapViewer::EnableSpline(int i)
{
  if (this->SplineControl==NULL)
    return;
  this->SplineControl->EnableSpline(i);
}

void vtkpxGUI2DSplineObjectmapViewer::DisableSpline(int i)
{
 if (this->SplineControl==NULL)
   return;
 this->SplineControl->DisableSpline(i);
}
/* -------------------------------------------------------------------------*/
int vtkpxGUI2DSplineObjectmapViewer::GetBeginSlice()
{
  return vtkpxGUI2DImageViewer::GetSliceNumber();
}

int vtkpxGUI2DSplineObjectmapViewer::ResetViewer()
{
  if (this->Orientation==3)
    this->Orientation=1;
  int status=vtkpxGUI2DImageViewer::ResetViewer();
  this->ObjectmapControl->UpdateFromViewer(1);
  return status;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI2DSplineObjectmapViewer::CreateSnake()
{
  if (this->SplineControl!=NULL)
    this->SplineControl->DoSnake(1);
}

void vtkpxGUI2DSplineObjectmapViewer::UpdateSnake()
{
  if (this->SplineControl!=NULL)
    this->SplineControl->DoSnake(2);

}

void vtkpxGUI2DSplineObjectmapViewer::SetSliceChangeCallback(const char* name)
{
  if (name==NULL)
    return;

  if (this->sliceChangeCallbackName!=NULL)
    delete [] this->sliceChangeCallbackName;

  this->sliceChangeCallbackName=new char[strlen(name)+1];
  strcpy(this->sliceChangeCallbackName,name);
}

void  vtkpxGUI2DSplineObjectmapViewer::ExecuteSliceChangeCallback()
{
  if (this->EnableSliceChangeCallback==0)
    return;
  
  if (this->sliceChangeCallbackName!=NULL)
    {
      PXTkApp::executeTclCommand(this->sliceChangeCallbackName);
    }
  else if (this->EventParent!=NULL && this->CallbackNo>0)
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
int vtkpxGUI2DSplineObjectmapViewer::HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev)
{
  int found=-1;
  if (ev==NULL || hasImage==PXFalse)
    return 0;
  
  if (!renderer->InViewport(x,y))
    return 0;
  
  /*  if (this->Editor!=NULL)
    {
      if (nbutton==3  && this->Editor->IsOpen()==0 && this->IsA("vtkpxGUI2DSplineObjectmapViewer")==0)
	{
	  if (state==2)
	    this->Editor->Show();
	  return 1;
	}
	}*/

  int sendtosplinecontrol=0;
  int draw_mode=this->ObjectmapControl->GetPaintMode();

  if (draw_mode==0)
    {
      if ( ( this->MouseToEditor || ( nbutton == 4 || nbutton==7 )) || ( this->ShiftMouseToEditor==0 && nbutton==1) )
	sendtosplinecontrol=1;
    }


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
	  if (this->IsA("vtkpxGUI2DSplineObjectmapViewer")==1)
	    {
	      state=6+state;
	    }
	}


      switch(plane)
	{
	case 0:
	  if (sendtosplinecontrol)
	    this->Editor->HandleClickedPoint(slice_sc,px1,py1,1,state);
	  this->LastClickedPointScaled[0]=slice_sc;
	  this->LastClickedPointScaled[1]=px1;
	  this->LastClickedPointScaled[2]=py1;
	  break;
	case 1:
	  if (sendtosplinecontrol)
	    this->Editor->HandleClickedPoint(px1,slice_sc,py1,1,state);
	  this->LastClickedPointScaled[0]=px1;
	  this->LastClickedPointScaled[1]=slice_sc;
	  this->LastClickedPointScaled[2]=py1;
	  break;
	case 2:
	  if (sendtosplinecontrol)
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

      if (!sendtosplinecontrol && draw_mode>0)
	{
	  int point[3];
	  for (int ia=0;ia<=2;ia++)
	    point[ia]=int(lv[ia]);

	  this->ObjectmapControl->SetPaint2DAxis(2);
	  this->ObjectmapControl->HandleClickedPoint(point[0],point[1],point[2],nbutton,state);
	  this->maskImageSlice->SetInput(this->maskImageRegion);
	}

      double vl=currentImage->GetScalarComponentAsDouble((int)lv[0],
							 (int)lv[1],
							 (int)lv[2],
							 this->CurrentFrame);
      int obj=(int)this->maskImageRegion->GetScalarComponentAsDouble((int)lv[0],
								    (int)lv[1],0,0);

      
      sprintf(pxtk_buffer,"%.2f/%d (%.0f,%.0f,%.0f)",vl,obj,
	      lv[0],lv[1],lv[2]);
      
      
      statusLabel->setText(pxtk_buffer);
          
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
int vtkpxGUI2DSplineObjectmapViewer::CreateObjectmapControl(PXTkFrame* fillframe)
{
  
  this->ObjectmapControl=vtkpxGUI2DImageEditor::New();
  this->ObjectmapControl->SetSimpleMode(0);
  this->ObjectmapControl->SetObjectmapViewer(this);
  this->ObjectmapControl->Initialize(fillframe->getWidgetName(),1);
  this->ObjectmapControl->RenamePaintOffLabel("Spline");
  this->ObjectmapControl->UpdateColorButtons();

  PXTkFrame* extra=this->ObjectmapControl->GetExtraFrame();

  PXTkButton* b1=new PXTkButton(this->EventManager,extra,"Fill In",806);  
  PXTkButton* b2=new PXTkButton(this->EventManager,extra,"Fill Out",810);  
  //  PXTkButton* b3=new PXTkButton(this->EventManager,extra,"Undo All",808);
  //PXTkButton* b4=new PXTkButton(this->EventManager,extra,"Revert",802);
  extra->addChildren("-side left -padx 1 -expand true -fill x",b1,b2);
  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUI2DSplineObjectmapViewer::FillObjectmap(int new_color,int inside)
{
  vtkpxSplineSource* spl=this->SplineControl->GetSpline(-1);


  int usethr=this->ObjectmapControl->GetPaintUseThresholdMode();


  if (inside<1)
    inside=0;
  else
    inside=1;


  vtkDataArray* elements=this->ObjectmapControl->GetNextArray();
 
  /*  if (inside==1 && usethr==0)
    {
      if (usethr==0)
	spl->AddToObjectMap(this->maskImageRegion,0,float(n),0);
    }
  else
  {*/
     
  vtkImageData* tmp=vtkImageData::New(); 
  tmp->CopyStructure(maskImageRegion); 
  tmp->SetNumberOfScalarComponents(maskImageRegion->GetNumberOfScalarComponents());
  tmp->AllocateScalars(); 
  tmp->GetPointData()->GetScalars()->FillComponent(0,0.0);
  spl->AddToObjectMap(tmp,0,1.0,0);
  
  vtkImageData* sourceImage=currentImageRegion;
  if (this->ObjectmapControl->GetPaintUseMaskMode())
    sourceImage=maskImageRegion;
  
  double range[2];   sourceImage->GetPointData()->GetScalars()->GetRange(range);
  float a[2];this->ObjectmapControl->GetThresholds(a);

      
  /*  vtkDataArray* src=sourceImage->GetPointData()->GetScalars();
  vtkDataArray* mask=this->maskImageRegion->GetPointData()->GetScalars();
  vtkDataArray* obj=tmp->GetPointData()->GetScalars();*/

  int dim[3]; this->maskImageRegion->GetDimensions(dim);

  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  int flag=int(tmp->GetScalarComponentAsDouble(i,j,k,0));
	  double v=sourceImage->GetScalarComponentAsDouble(i,j,k,0);
	  
	  if (usethr==0)
	    {
	      if ( (inside==1 && flag==1) || (inside==0 && flag==0) )
		{
		  //  mask->SetComponent(i,0,float(n));
		  this->ObjectmapControl->ChangeObjectmapValue(elements,i,j,k,new_color);
		}
	    }
	  else 
	    {
	      if (v>=a[0] && v<=a[1])
		{
		  if (inside==0 && flag==0)
		    this->ObjectmapControl->ChangeObjectmapValue(elements,i,j,k,new_color);
		  else if (inside==1 && flag==1)
		    this->ObjectmapControl->ChangeObjectmapValue(elements,i,j,k,new_color);
		}
	    }
	}

  tmp->Delete();
  maskImageSlice->SetInput(this->maskImageRegion);
  maskImageSlice->UpdateTexture();
  this->UpdateDisplay();

  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUI2DSplineObjectmapViewer::UpdateObjectmapSlice()
{
  if (this->ObjectmapControl!=NULL)
    this->ObjectmapControl->UpdateFromViewer(0);
  return 1;
}

int vtkpxGUI2DSplineObjectmapViewer::UpdateObjectmapChanges()
{
  // Never Extracted Nothing to do
  if (this->LastExtractedSlice == -1 )
    return 0;

  /*  int dim[3];
  this->maskImageRegion->GetDimensions(dim);
  int nc=  this->maskImageRegion->GetNumberOfScalarComponents();
  int nc2=  this->maskImage->GetNumberOfScalarComponents();*/
  //  fprintf(stderr,"Dimensions =%d,%d,%d f=%d vs %d\n",dim[0],dim[1],dim[2],nc,nc2);

  vtkpxImageReplaceSlice* repl=vtkpxImageReplaceSlice::New();
  repl->SetInput(maskImage);
  repl->SetInsertedSlice(maskImageRegion);
  repl->SetCurrentPlane(this->LastExtractedOrientation);
  repl->SetSliceNo(this->LastExtractedSlice);
  repl->SetFrame(this->LastExtractedFrame);
  repl->SetDeepCopyMode(0);
  repl->Update();
  maskImage->ShallowCopy(repl->GetOutput());
  repl->Delete();
  return 1;
}

void vtkpxGUI2DSplineObjectmapViewer::SetMaskImage(vtkImageData* maskimage,int orientation)
{
  vtkpxGUI2DImageViewer::SetMaskImage(maskimage,orientation);
  this->ObjectmapControl->UpdateFromViewer(0);
  this->ExecuteSliceChangeCallback();
}


