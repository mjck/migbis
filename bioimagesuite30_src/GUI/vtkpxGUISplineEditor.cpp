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
  Module:    $RCSfile: vtkpxGUISplineEditor.cpp,v $
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

#include "vtkpxGUISplineEditor.h"
#include "vtkObjectFactory.h"

vtkpxGUISplineEditor* vtkpxGUISplineEditor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUISplineEditor");
  if(ret)
      {
	return (vtkpxGUISplineEditor*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUISplineEditor;
}

// Construct object with no children.
vtkpxGUISplineEditor::vtkpxGUISplineEditor()
{
  single_slice_mode=PXTrue;
  xy_slice_only=PXTrue;
  this->NumberOfViewers=1;
  this->NumberOfColumns=1;
  this->NumberOfRows=1;
  this->MaximumNumberOfViewers=1;

  this->CreateHighlights=0;
  this->SplineControl=NULL;
  this->NumberOfSplines=4;
  this->ColorMode=0;
  this->CurrentSlice=-1;
  this->ForceNoAutoUpdate=0;
  this->sliceChangeCallbackName=NULL;
  this->EnableSliceChangeCallback=1;
  this->SlaveMode=0;
  this->DefaultScaleMode=4;
  this->EnableAutoUpdateModeCallback=1;
}
/* -------------------------------------------------------------------------*/
vtkpxGUISplineEditor::~vtkpxGUISplineEditor()
{
  if (this->SplineControl)
    this->SplineControl->Delete();

  if (this->sliceChangeCallbackName!=NULL)
    delete [] this->sliceChangeCallbackName;

}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineEditor::SetNumberOfSplines(int a)
{
  if (this->Initialized==1)
    return;
  
  this->NumberOfSplines=Irange(a,1,SP_MAX_COLLECTIONS);
}

/* -------------------------------------------------------------------------*/
char* vtkpxGUISplineEditor::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();
  
  vtkpxGUIComponent::Initialize(name,inside);

  PXTkFrame* manager=(PXTkFrame*)(this->EventManager->getMainWindow());

  PXTkFrame* tframe=new PXTkFrame(this->EventManager,manager);
  PXTkFrame* bframe=new PXTkFrame(this->EventManager,manager);

  manager->setPackMode(PXTrue);
  manager->addChildren("-side bottom -expand false -fill x",bframe);
  manager->addChildren("-side top    -expand true  -fill both",tframe);

  PXTkFrame* lframe=new PXTkFrame(this->EventManager,tframe);
  PXTkFrame* rframe=new PXTkFrame(this->EventManager,tframe);
  PXTkFrame* lframe2=new PXTkFrame(this->EventManager,bframe);
  PXTkFrame* rframe2=new PXTkFrame(this->EventManager,bframe);


  tframe->addChildren("-side right -expand false -fill y",rframe);
  tframe->addChildren("-side left -expand true -fill both",lframe);


  bframe->addChildren("-side right -expand false -fill y",rframe2);
  bframe->addChildren("-side left -expand true -fill both",lframe2);

  InitControls(lframe2,this->CreateHighlights);
  this->ShowFrameControls(0);
  InitViewer(lframe);


  this->SplineControl=vtkpxGUISplineControl::New();
  this->SplineControl->SetNumberOfSplines(this->NumberOfSplines);
  this->SplineControl->SetSlaveMode(this->SlaveMode);
  this->SplineControl->SetDefaultScaleMode(this->DefaultScaleMode);
  this->SetEditor(this->SplineControl);
  this->SetMouseToEditor(1);
  if (this->EventParent!=NULL && this->CallbackNo>0)
    this->SplineControl->SetCallback(this,800);
  this->SplineControl->Initialize(rframe->getWidgetName(),1);
     
  if (!inside)
    {
      PXTkFrame* bottomButtonFrame=rframe2;
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
int vtkpxGUISplineEditor::HandleEvent(int event)
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
      
    default:
      vtkpxGUIMosaicViewer::HandleEvent(event);
    }
  
  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineEditor::SetSlice(int sl)
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
  ChangeSliceIncrement(sl,-1,-1);
  this->CurrentSlice=vtkpxGUIMosaicViewer::GetBeginSlice();
  this->ForceNoAutoUpdate=a;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUISplineEditor::IsAutoUpdateModeEnabled()
{

  if (this->SplineControl->GetAutoUpdateMode()==1 && this->ForceNoAutoUpdate==0)
    return 1;
  
  return 0;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineEditor::ChangeSliceIncrement(int sl,int incr,int fr)
{

  if (this->SplineControl!=NULL)
    {
      if (this->IsAutoUpdateModeEnabled()==1  && this->EnableAutoUpdateModeCallback==1)
	{
	  this->HandleEvent(800);
	}
    }

  this->CurrentSlice=vtkpxGUIMosaicViewer::GetBeginSlice();
  vtkpxGUIMosaicViewer::ChangeSliceIncrement(sl,incr,fr);
  PXTkApp::executeTclCommand("update idletasks");

  char line[300];
  sprintf(line,"SplineEditor Slice=%d Frame=%d",
	  this->GetBeginSlice(),this->GetCurrentFrame()+1);
  this->EventManager->setTitle(line);

  this->ExecuteSliceChangeCallback();
}
/* -------------------------------------------------------------------------*/
// Description:
// Enable/Disable Spline
void vtkpxGUISplineEditor::EnableSpline(int i)
{
  if (this->SplineControl==NULL)
    return;
  this->SplineControl->EnableSpline(i);
}

void vtkpxGUISplineEditor::DisableSpline(int i)
{
 if (this->SplineControl==NULL)
   return;
 this->SplineControl->DisableSpline(i);
}
/* -------------------------------------------------------------------------*/
int vtkpxGUISplineEditor::GetBeginSlice()
{
  if (this->CurrentSlice==-1)
    this->CurrentSlice=vtkpxGUIMosaicViewer::GetBeginSlice();
  return this->CurrentSlice;
}

int vtkpxGUISplineEditor::ResetViewer()
{
  if (this->Orientation==3)
    this->Orientation=1;
  return vtkpxGUIMosaicViewer::ResetViewer();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUISplineEditor::CreateSnake()
{
  if (this->SplineControl!=NULL)
    this->SplineControl->DoSnake(1);
}

void vtkpxGUISplineEditor::UpdateSnake()
{
  if (this->SplineControl!=NULL)
    this->SplineControl->DoSnake(2);

}

void vtkpxGUISplineEditor::CopyCallback()
{
  if (this->SplineControl!=NULL)
    this->SplineControl->CopySpline();
}

void vtkpxGUISplineEditor::PasteCallback()
{
  if (this->SplineControl!=NULL)
    this->SplineControl->PasteSpline();
}

void vtkpxGUISplineEditor::SetSliceChangeCallback(const char* name)
{
  if (name==NULL)
    return;

  if (this->sliceChangeCallbackName!=NULL)
    delete [] this->sliceChangeCallbackName;

  this->sliceChangeCallbackName=new char[strlen(name)+1];
  strcpy(this->sliceChangeCallbackName,name);
}

void  vtkpxGUISplineEditor::ExecuteSliceChangeCallback()
{
  if (this->EnableSliceChangeCallback==0 || this->sliceChangeCallbackName==NULL)
    return;

  PXTkApp::executeTclCommand(this->sliceChangeCallbackName);

}

