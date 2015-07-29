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





#include "pxutil.h"
#include "vtkActor.h"
#include "vtkImageQuantizeRGBToIndex.h"
#include "vtkObjectFactory.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkImageThreshold.h"
#include "vtkpxColorMapUtil.h"
#include "vtkpxGUIObjectmapMosaicEditor.h"
#include "vtkpxGUIObjectmapMosaicViewer.h"

// -------------------------------------------------------------------------
vtkpxGUIObjectmapMosaicViewer* vtkpxGUIObjectmapMosaicViewer::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIObjectmapMosaicViewer");
  if(ret)
      {
	return (vtkpxGUIObjectmapMosaicViewer*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIObjectmapMosaicViewer;
}
// -------------------------------------------------------------------------
// Construct object with no children.
vtkpxGUIObjectmapMosaicViewer::vtkpxGUIObjectmapMosaicViewer()
{
  maskTransparency=NULL;
  maskImage=NULL;
  maskLookupTable=vtkLookupTable::New();

  for (int i=0;i<MVP_MAX_VIEWERS;i++)
    this->maskImageSlice[i]=NULL;

  this->create_internal_editor=0;
  this->InternalEditor=NULL;
  this->ColorMode=0;
}

// -------------------------------------------------------------------------
vtkpxGUIObjectmapMosaicViewer::~vtkpxGUIObjectmapMosaicViewer()
{
  if (maskImage!=NULL)
    maskImage->Delete();
  
  if (maskLookupTable!=NULL)
    maskLookupTable->Delete();

}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapMosaicViewer::SetCreateInternalEditorOn()
{
  if (this->Initialized==0)
    create_internal_editor=1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIObjectmapMosaicViewer::ResetViewer()
{
  if (!(hasImage && this->Initialized))
      return 0;


  this->AutoCreateMaskAndLookupTable();
  vtkpxGUIMosaicViewer::ResetViewer();

  for (int kk=0;kk<this->NumberOfViewers;kk++)
    {
      if (maskImageSlice[kk]!=NULL)
	{
	  maskImageSlice[kk]->SetInput(maskImage);
	  maskImageSlice[kk]->SetFrame(0);
	  maskImageSlice[kk]->SetLookupTable(this->maskLookupTable);
	  maskImageSlice[kk]->Update();
	}
    }

  if (this->InternalEditor!=NULL)
    this->InternalEditor->UpdateFromViewer();

  this->UpdateDisplay();

  return TCL_OK;
}
// -------------------------------------------------------------------------
int vtkpxGUIObjectmapMosaicViewer::HandleEvent(int event)
{
  if (event<799)
      return vtkpxGUIMosaicViewer::HandleEvent(event);
  
  if (event==800)
    {
      this->SetObjectmapTransparency(this->maskTransparency->getValue());
      this->LastEvent=event;
    }

  if (event==801)
    {
      if (this->InternalEditor!=NULL)
	this->InternalEditor->Show();
    }
  
  return TCL_OK;
  
}


// -------------------------------------------------------------------------
int vtkpxGUIObjectmapMosaicViewer::UpdateViewers(int firstv,int lastv)
{
  int ok=vtkpxGUIMosaicViewer::UpdateViewers(firstv,lastv);

  this->EventManager->setWaitCursor(PXTrue);

  firstv=Irange(firstv,0,this->NumberOfViewers-1);
  lastv=Irange(lastv,0,this->NumberOfViewers-1);

  for (int k=firstv;k<=lastv;k++)
    {
      int lv=imageSlice[k]->GetLevel();
      int pl=imageSlice[k]->GetCurrentPlane();
      if (maskImageSlice[k]!=NULL)
	{
	  maskImageSlice[k]->SetFrame(0);
	  maskImageSlice[k]->SetLevel(lv);
	  maskImageSlice[k]->SetCurrentPlane(pl);
	  maskImageSlice[k]->SetDisplayMode(2);
	  maskImageSlice[k]->Update();
	}
    }
  
  this->EventManager->setWaitCursor(PXFalse);
  this->UpdateDisplay();
  return TCL_OK;
}
// -------------------------------------------------------------------------
void vtkpxGUIObjectmapMosaicViewer::CreateViewerAndSlice(int index)
{
  if (hasImage== PXFalse || this->Initialized==0)
      return;

  index=Irange(index,0,MVP_MAX_VIEWERS-1);
  if (initialized[index])
      return;

  vtkpxGUIMosaicViewer::CreateViewerAndSlice(index);


  maskImageSlice[index]=vtkpxImageSlice::New();
  maskImageSlice[index]->SetAutoUpdate(0);
  maskImageSlice[index]->SetInput(maskImage);
  maskImageSlice[index]->SetFrame(0);
  maskImageSlice[index]->SetLookupTable(maskLookupTable);
  maskImageSlice[index]->SetOpacity(0.5);
  maskImageSlice[index]->SetInterpolation(0);
  renderer[index]->GetRenderer()->AddActor(maskImageSlice[index]);
  
  return;

}
// -------------------------------------------------------------------------
void vtkpxGUIObjectmapMosaicViewer::InitControls(PXTkFrame* parwidget,int dohighlight)
{
  vtkpxGUIMosaicViewer::InitControls(parwidget,dohighlight);
  if (this->extraFrame==NULL)
    return;

  maskTransparency=new PXTkArrowScale(this->EventManager,extraFrame,"Mask:",800,PXTrue);
  maskTransparency->setRange(0.0,100.0);
  maskTransparency->setResolution(1.0);
  maskTransparency->setValue(50.0);
  maskTransparency->setLengthWidth(80,15);

  extraFrame->addChildren("-side left -expand true -fill x",maskTransparency->getMainWindow());

  if (create_internal_editor==0)
    return;

  PXTkButton* ut5=new PXTkButton(this->EventManager,controlFrame,"Show Paint Controls",801); 
  ut5->configure("-fg","red");
  controlFrame->addChildren("-side bottom -expand true -fill x -padx 5",ut5);
  

  this->InternalEditor=vtkpxGUIObjectmapMosaicEditor::New();
  this->InternalEditor->Initialize(this->GetWidgetName(),0);
  this->InternalEditor->SetObjectmapViewer(this);
  //  this->InternalEditor->CreateDefaultLookupTable();
}

// -------------------------------------------------------------------------
float vtkpxGUIObjectmapMosaicViewer::GetObjectmapTransparency()
{
  if (  this->maskImageSlice[0]==NULL)
    return 0.0;
  
  return 100.0*this->maskImageSlice[0]->GetOpacity();
}

void vtkpxGUIObjectmapMosaicViewer::SetObjectmapTransparency(float v)
{
  if (this->maskImageSlice[0]!=NULL)
    {
      v*=0.01;
      for (int ia=0;ia<this->NumberOfViewers;ia++)
	{
	  if (this->maskImageSlice[ia]!=NULL)
	    maskImageSlice[ia]->SetOpacity(v);
	}
      this->UpdateDisplay();
      this->maskTransparency->setValue(v*100.0,PXFalse);
    }
}

void vtkpxGUIObjectmapMosaicViewer::UpdateObjectmap()
{
  if (this->maskImageSlice[0]==NULL)
    return;
  
  for (int kk=0;kk<this->NumberOfViewers;kk++)
    maskImageSlice[kk]->SetInput(maskImage);

  if (this->InternalEditor!=NULL)
    this->InternalEditor->UpdateFromViewer();


  this->UpdateDisplay();
}


int vtkpxGUIObjectmapMosaicViewer::SetObjectMapImage(vtkImageData* img)
{
  if ((!hasImage && this->Initialized))
    return 0;
  
  int ndim[3]; img->GetDimensions(ndim);
  int odim[3]; this->currentImage->GetDimensions(odim);
  int nc=img->GetNumberOfScalarComponents();
  int onc=this->currentImage->GetNumberOfScalarComponents();
  int sum=0;
  for (int i=0;i<=2;i++)
    sum+=abs(ndim[i]-odim[i]);

  if (sum!=0 || nc>onc)
    {
      vtkErrorMacro(<<"Bad Mask Image Dimensions\n");
      return 0;
    }
  
  this->maskImage->ShallowCopy(img);
  double r[2]; img->GetPointData()->GetScalars()->GetRange(r);

  vtkpxSurfaceUtil::DefaultObjectMapLookupTable(maskLookupTable,int(r[1]+2),1);
  UpdateObjectmap();
  return 1;
}

// -------------------------------------------------------------------------
int vtkpxGUIObjectmapMosaicViewer::AutoCreateMaskAndLookupTable()
{
  this->ClearMaskImage();
  if (this->maskLookupTable==NULL)
    maskLookupTable=vtkLookupTable::New();

  vtkpxSurfaceUtil::DefaultObjectMapLookupTable(maskLookupTable,28,1);

  return 1;
}

int vtkpxGUIObjectmapMosaicViewer::SetObjectLookupTable(vtkLookupTable* lkp)
{
  return this->SetObjectLookupTable(lkp,1);
}

int vtkpxGUIObjectmapMosaicViewer::SetObjectLookupTable(vtkLookupTable* lkp,int updateeditor)
{
  if (lkp==NULL)
    return 0;

  if (this->maskLookupTable==NULL)
    maskLookupTable=vtkLookupTable::New();

  vtkpxColorMapUtil::CopyLookupTable(lkp,this->maskLookupTable);

  if (this->InternalEditor!=NULL && updateeditor==1)
    this->InternalEditor->UpdateFromViewer(1);
  
  this->UpdateDisplay();
  return 1;
}

void vtkpxGUIObjectmapMosaicViewer::ClearMaskImage()
{
  if (this->maskImage==NULL)
    this->maskImage=vtkImageData::New();

  vtkImageThreshold* thr=vtkImageThreshold::New();
  thr->SetInput(currentImage);
  thr->ThresholdByLower(1);
  thr->ReplaceInOn();
  thr->ReplaceOutOn();
  thr->SetInValue(0.0);
  thr->SetOutValue(0.0);
  thr->SetOutputScalarTypeToUnsignedChar();
  thr->Update();

  this->maskImage->ShallowCopy(thr->GetOutput());
  thr->Delete();


}

void vtkpxGUIObjectmapMosaicViewer::QuickUpdateObjectmap()
{
  for (int i=0;i<this->MaximumNumberOfViewers;i++)
    {
      if (this->maskImageSlice[i]!=NULL)
	{
	  this->maskImageSlice[i]->Update();
	  this->maskImageSlice[i]->UpdateTexture();
	  this->maskImageSlice[i]->Modified();
	}
    }
}

// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIObjectmapMosaicViewer::HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev)
{
  int status=vtkpxGUIMosaicViewer::HandleMouseButtonEvent(nbutton,state,x,y,ev);
  if (status==0)
    return status;

  if (this->InternalEditor==NULL)
    return status;
  
  double lv[3];
  this->GetLastClickedPoint(lv);
  
  int x1=int(lv[0]+0.5);
  int y1=int(lv[1]+0.5);
  int z1=int(lv[2]+0.5);

  int plane=currentPlane->getIndex();
  this->InternalEditor->SetPaint2DAxis(plane);

  //  fprintf(stderr,"Sending to Int Editor %d,%d,%d from %f,%f,%f\n",x1,y1,z1,lv[0],lv[1],lv[2]);
  this->InternalEditor->HandleClickedPoint(x1,y1,z1,nbutton,state);
  return status;
}

std::string vtkpxGUIObjectmapMosaicViewer::GetObjectmapValueAsText(int a,int b,int c)
{
  std::string s="n";
  if (this->maskImage!=NULL)
    {
      int dim[3]; this->maskImage->GetDimensions(dim);
      if (a>=0 && a<dim[0] &&
	  b>=0 && b<dim[1] &&
	  c>=0 && c<dim[2])
	{
	  int vl=(int)this->maskImage->GetScalarComponentAsDouble(a,b,c,0);
	  s=this->GetFormatedNumber(vl,3);
	}
    }
  return s;
}

int vtkpxGUIObjectmapMosaicViewer::PaintingEnabled()
{
  if (this->InternalEditor==NULL)
    return 0;
  return (this->InternalEditor->GetPaintMode()>0);
}

