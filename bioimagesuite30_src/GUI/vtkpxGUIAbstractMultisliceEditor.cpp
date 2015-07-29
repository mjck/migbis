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
  Module:    $RCSfile: vtkpxGUIAbstractMultisliceEditor.cpp,v $
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

#include "vtkpxGUIAbstractMultisliceEditor.h"
#include "vtkObjectFactory.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkpxImageExtractVOI.h"
#include "vtkImageThreshold.h"
#include "vtkImageSeedConnectivity.h"
#include "vtkImageExtractComponents.h"
#include "vtkMath.h"
#include "vtkbisObjectmapRegularization.h"
#include "vtkpxDataArrayUndoStack.h"

using namespace bioimagesuite_legacy;

// Construct object with no children.
vtkpxGUIAbstractMultisliceEditor::vtkpxGUIAbstractMultisliceEditor()
{
  for (int ia=0;ia< pxAME_NUM_COLOR_BUTTONS;ia++)
    this->colorButton[ia]=NULL;
  for (int ia=0;ia<pxAME_NUM_DIALOG_COLOR_BUTTONS;ia++)
    this->dialogColorButton[ia]=NULL;


  this->DefaultScaleMode=4;

  objectmapcolormapname=new char[20];
  strcpy(objectmapcolormapname,"object.cmap");
  this->current_fill_color=1;
  this->draw_mode=0;
  this->old_draw_mode=0;
  this->draw_mode3D=0;
  this->MainImage=NULL;
  this->Objectmap=NULL;
  this->ObjectmapLookupTable=NULL;
  this->SimpleMode=0;
  this->Paint2DAxis=2;
  this->SingleSliceMode=0;
  this->paintMode=NULL;
  this->paintMode3D=NULL;
  this->smoothMode=NULL;
  this->smoothConvergenceMode=NULL;
  this->fillMode=NULL;
  this->fillThresholds[0]=NULL;
  this->fillThresholds[1]=NULL;
  this->colorSelection=NULL;
  this->connectFillMode=NULL;
  this->voiNameEntry=NULL;
  
  this->UndoStack=vtkpxDataArrayUndoStack::New();
  this->UndoStack->Initialize(40,5,VTK_INT);

  this->ResetVOINames();
}
/* -------------------------------------------------------------------------*/
vtkpxGUIAbstractMultisliceEditor::~vtkpxGUIAbstractMultisliceEditor()
{
  delete []  objectmapcolormapname;
  if (this->MainImage)
    this->MainImage->Delete();
  if (this->Objectmap)
    this->Objectmap->Delete();
  if (this->ObjectmapLookupTable)
    this->ObjectmapLookupTable->Delete();

  this->UndoStack->Delete();


}

int vtkpxGUIAbstractMultisliceEditor::SetImagesAndLookupTable(vtkImageData* mainImage,vtkImageData* objectmap,vtkLookupTable* lkup,
							      int iscolor)
{
  //  fprintf(stderr,"Setting Image and Objectmap %d %d\n",mainImage,objectmap);

  if (mainImage==NULL || objectmap == NULL)
    return 0;

  if (mainImage->GetNumberOfPoints()!=objectmap->GetNumberOfPoints())
    return 0;

  if (this->MainImage==NULL)
    this->MainImage=vtkImageData::New();
  this->MainImage->ShallowCopy(mainImage);

  this->CurrentImageIsColor=0;
  if (iscolor>0)
    {
      if (this->MainImage->GetNumberOfScalarComponents()==3 && this->MainImage->GetScalarType()==VTK_UNSIGNED_CHAR)
	this->CurrentImageIsColor=1;
    }


  if (this->fillThresholds[0]!=NULL)
    {
      //      fprintf(stderr,"Reseting thresholds\n");
      if (!this->CurrentImageIsColor)
	{
	  double range[2]; this->MainImage->GetPointData()->GetScalars()->GetRange(range);
	  sprintf(pxtk_buffer,"%.2f",0.8*range[1]+0.2*range[0]);
	  this->fillThresholds[0]->setText(pxtk_buffer);
	  sprintf(pxtk_buffer,"%.2f",range[1]);
	  this->fillThresholds[1]->setText(pxtk_buffer);
	} 
      else 
	{
	  this->fillThresholds[0]->setText("204");
	  this->fillThresholds[1]->setText("255");
	}
    }

  if (this->colorSelection!=NULL)
    {
      if (this->CurrentImageIsColor)
	this->colorSelection->enable(PXTrue);
      else
	this->colorSelection->enable(PXFalse);
    }

  if (this->Objectmap==NULL)
    this->Objectmap=vtkImageData::New();
  this->Objectmap->ShallowCopy(objectmap);



  this->UndoStack->Reset();


  if (this->ObjectmapLookupTable==NULL)
    this->ObjectmapLookupTable=vtkLookupTable::New();
  
  if (lkup!=NULL)
    {
      this->ObjectmapLookupTable->DeepCopy(lkup);
    }
  else
    {
      this->CreateDefaultLookupTable();
    }


  this->UpdateColorButtons();

  return 1;

}
/* -------------------------------------------------------------------------*/
double vtkpxGUIAbstractMultisliceEditor::SetObjectmapWithUndo(vtkImageData* objectmap)
{
  if (objectmap == NULL || this->Objectmap==NULL)
    return -1.0;

  if (this->Objectmap->GetNumberOfPoints()!=objectmap->GetNumberOfPoints())
    return -1.0;

  vtkDataArray* olddata=this->Objectmap->GetPointData()->GetScalars();
  vtkDataArray* newdata=objectmap->GetPointData()->GetScalars();
  int nt=newdata->GetNumberOfTuples();

  vtkDataArray* elements=this->UndoStack->GetNextArray();
  
  int dim[3]; this->Objectmap->GetDimensions(dim);

  int index=0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  int old_color=(int)olddata->GetComponent(index,0);
	  int new_color=(int)newdata->GetComponent(index,0);
	  if (new_color!=old_color)
	    this->ChangeObjectmapValue(elements,i,j,k,new_color);
	  ++index;
	}

  double p=100.0*double(elements->GetNumberOfTuples())/double(nt);

  /*  fprintf(stderr,"Changed %d/%d elements (%.2f %%)\n",
      elements->GetNumberOfTuples(),nt,p);*/

	  

  this->UpdateViewerObjectmap();
  this->UpdateViewerDisplay();
  return p;

}
/* -------------------------------------------------------------------------*/

char* vtkpxGUIAbstractMultisliceEditor::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();
  
  vtkpxGUIComponent::Initialize(name,inside);
  this->EventManager->setTitle("Objectmap Editor");

  PXTkFrame* manager=(PXTkFrame*)(this->EventManager->getMainWindow());

  PXTkLabelFrame* fillFrame=new PXTkLabelFrame(this->EventManager,manager,"Paint Control");
  PXTkFrame* bottomButtonFrame=new PXTkFrame(this->EventManager,manager);

  manager->setPackMode(PXTrue);
  manager->addChildren("-side bottom -expand false -fill x",bottomButtonFrame);
  manager->addChildren("-side top    -expand true  -fill both",fillFrame);

  this->CreateFillControl(fillFrame);

  if (!inside)
    {
      PXTkButton* closeB=new PXTkButton(this->EventManager,bottomButtonFrame,"Close",3);
      bottomButtonFrame->addChildren("-side right -expand false -fill x -padx 10",closeB);
    }

  return GetWidgetName();

}
int vtkpxGUIAbstractMultisliceEditor::CreateDefaultLookupTable(int voimode)
{
  if (this->ObjectmapLookupTable==NULL)
    this->ObjectmapLookupTable=vtkLookupTable::New();
  if (voimode==0)
    vtkpxSurfaceUtil::DefaultObjectMapLookupTable(ObjectmapLookupTable,-1,1);
  else
    vtkpxSurfaceUtil::VOIObjectMapLookupTable(ObjectmapLookupTable,-1);
  this->UpdateObjectmapColormap();

  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIAbstractMultisliceEditor::HandleEvent(int event)
{

  if (event<10)
    return this->EventManager->handleDirectEvent(event);


  if (event>=821 && event<821+vtkpxSurfaceUtil::DefaultObjectMapLookupTableNumberOfColors())
    {
      current_fill_color=event-821;


      if (current_fill_color==0)
	{
	  voiNameEntry->configure("-bg","black");
	  voiNameEntry->configure("-fg","white");
	}
      else
	{
	  char* line=dialogColorButton[current_fill_color]->cget("-bg");	  
	  strcpy(pxtk_buffer,line);
	  editcolorframe->configure("-bg",pxtk_buffer);
	  voiNameEntry->configure("-fg","black");
	  voiNameEntry->configure("-bg","white");
	}
      voiNameEntry->setText(voiNames[current_fill_color].c_str());
    }
  else
    {
      switch(event)
	{
	case 800:
	  this->draw_mode=paintMode->getIndex();
	  if (this->draw_mode==10)
	    {
	      this->draw_mode3D=1;
	      if (fillMode!=NULL)
		fillMode->setState(PXTrue);
	      if (connectFillMode!=NULL)
		connectFillMode->setState(PXTrue);
	      if (this->paintMode3D!=NULL)
		paintMode3D->setState(PXTrue);
	    }

	  if (this->draw_mode==10)
	    this->EventManager->messageBox("You have enabled a large brush ... go slowly!","Painting  Warning");
	  break;

	case 801:
	  if (this->paintMode3D!=NULL)
	    {
	      if (this->draw_mode==10)
		{
		  paintMode3D->setState(PXTrue);
		}
	      else
		{
		  if (paintMode3D->getState())
		    this->draw_mode3D=1;
		  else
		    this->draw_mode3D=0;
		}
	    }
	  break;
	  

	case 802:
	  this->colorButtonDialog->popupDialog();
	  break;

	  
	case 803:
	  this->LoadObjectmapColormap();
	  break;
	  
	case 804:
	  this->SaveObjectmapColormap();
	  break;
	  
	case 805:
	  this->CreateDefaultLookupTable();
	  break;

	case 806:
	  this->EditColorButton(current_fill_color);
	  break;

	case 807:
	  this->UndoLastObjectmapChange();
	  break;

	case 808:
	  this->RedoLastObjectmapChange();
	  break;

	case 809:
	  this->SmoothObjectmap();
	  break;

	case 810:
	  // Update name
	  //	  fprintf(stderr,"Updating VOI Name\n");
		{
	  char *line=voiNameEntry->getText();
	  if (strlen(line)>0 && current_fill_color>0)
	    voiNames[current_fill_color]=line;
	  else
	    voiNameEntry->setText(voiNames[current_fill_color].c_str());
		}
	  break;
	  
	default:
	  vtkpxGUIComponent::HandleEvent(event);
	}
    }
  
  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIAbstractMultisliceEditor::GetPaintConnectMode()
{
  if (this->connectFillMode!=NULL)
    {
      if (connectFillMode->getState())
	return 1;
    }
  return 0;

}

int vtkpxGUIAbstractMultisliceEditor::GetPaintUseThresholdMode()
{
  if (this->fillMode!=NULL)
    {
      if (fillMode->getState())
	return 1;
    }
  return 0;

}

int vtkpxGUIAbstractMultisliceEditor::GetPaintUseMaskMode()
{
  if (baseImageMode!=NULL)
    {
      if (baseImageMode->getState()==PXTrue)
	return 1;
    }
  return 0;
}
 
void vtkpxGUIAbstractMultisliceEditor::GetThresholds(float val[2])
{
  val[0]=0.0;
  val[1]=0.0;
  if (fillThresholds[0]==NULL || fillThresholds[1]==NULL)
    return;

  val[0]=atof(fillThresholds[0]->getText());
  val[1]=atof(fillThresholds[1]->getText());

  return;
}

int vtkpxGUIAbstractMultisliceEditor::GetCurrentColor()
{
  return current_fill_color;
}

int vtkpxGUIAbstractMultisliceEditor::GetPaintMode()
{
  return this->draw_mode;
}

int vtkpxGUIAbstractMultisliceEditor::GetPaint3DMode()
{
  return this->draw_mode3D;
}


void vtkpxGUIAbstractMultisliceEditor::ChangeObjectmapValue(vtkDataArray* elements,int i,int j,int k,int new_color)
{
  double old_color=this->Objectmap->GetScalarComponentAsDouble(i,j,k,0);
  this->Objectmap->SetScalarComponentFromDouble(i,j,k,0,double(new_color));
  double v[5];
  v[0]=double(i);
  v[1]=double(j);
  v[2]=double(k);
  v[3]=old_color;
  v[4]=double(new_color);

  if (elements!=NULL)
    elements->InsertNextTuple(v);
}

void vtkpxGUIAbstractMultisliceEditor::UndoLastObjectmapChange()
{
  vtkDataArray* elements=this->UndoStack->GetNextUndoArray();
  if (elements==NULL)
    return;

  //  vtkpxDataArrayUndoStack::ConsolidateArray(elements,3);

  int nt=elements->GetNumberOfTuples();
  for (int ia=nt-1;ia>=0;ia=ia-1)
    {
      int i=int(elements->GetComponent(ia,0));
      int j=int(elements->GetComponent(ia,1));
      int k=int(elements->GetComponent(ia,2));
      double old_color=elements->GetComponent(ia,3);
      double new_color=elements->GetComponent(ia,4);
      this->Objectmap->SetScalarComponentFromDouble(i,j,k,0,old_color);
    }

  this->UpdateViewerObjectmap();
  this->UpdateViewerDisplay();
  this->UpdateUndoIndicesDisplay();
}
// --------------------------------------------------------------------------------------
void vtkpxGUIAbstractMultisliceEditor::RedoLastObjectmapChange()
{
  vtkDataArray* elements=this->UndoStack->GetNextRedoArray();
  if (elements==NULL)
    return;

  int nt=elements->GetNumberOfTuples();
  for (int ia=0;ia<nt;ia++)
    {
      double* v=elements->GetTuple(ia);
      int i=int(v[0]);
      int j=int(v[1]);
      int k=int(v[2]);
      double old_color=v[3];
      double new_color=v[4];
      this->Objectmap->SetScalarComponentFromDouble(i,j,k,0,new_color);
    }

  this->UpdateViewerObjectmap();
  this->UpdateViewerDisplay();
  this->UpdateUndoIndicesDisplay();
}
// --------------------------------------------------------------------------------------
void vtkpxGUIAbstractMultisliceEditor::UpdateUndoIndicesDisplay()
{
  if (this->Initialized==0)
    return;

  if (!this->Debug)
    return;
  
  sprintf(pxtk_buffer,"C=%d U=%d (%d) R=%d (%d,m=%d)",
	  this->UndoStack->GetCurrentIndex(),
	  this->UndoStack->GetCurrentUndoIndex(),	  this->UndoStack->GetCurrentlyUndoing(),
	  this->UndoStack->GetCurrentRedoIndex(),this->UndoStack->GetCurrentlyRedoing(),
	  this->UndoStack->GetMaxIndex());
  fprintf(stderr,"Status=%s\n",pxtk_buffer);
}
// -------------------------------------------------------------------------------------
vtkDataArray* vtkpxGUIAbstractMultisliceEditor::GetNextArray()
{
  if (this->UndoStack==NULL)
    return NULL;
  return this->UndoStack->GetNextArray();
}

int vtkpxGUIAbstractMultisliceEditor::HandleClickedPoint(int x,int y,int z,int nbutton,int state)
{
  if (!this->EventManager->isOpen() && !this->EventManager->isInside())
    return 0;

  if ( nbutton==4  && state!=2)
    return 0;

  if (nbutton==4) 
    {
      if (draw_mode>0)
	{
	  this->old_draw_mode=this->draw_mode;
	  this->draw_mode=0;
	  this->paintMode->setIndex(draw_mode);
	  return 0;
	}
      else
	{
	  this->draw_mode=this->old_draw_mode;
	  this->old_draw_mode=0;
	  this->paintMode->setIndex(draw_mode);
	}
      return 0;
    }

  if (nbutton!=1)
    return 0;
  
  if (state==2 || this->draw_mode==0)
    return 0;
  

  // Paint Mode
  int   dim[3];   
  int   lv[3];
  this->MainImage->GetDimensions(dim);
  lv[0]=Irange(x,0,dim[0]-1);
  lv[1]=Irange(y,0,dim[1]-1);
  lv[2]=Irange(z,0,dim[2]-1);
  
  //  fprintf(stderr,"Dimensions = %d %d %d, point=%d,%d,%d\n",dim[0],dim[1],dim[2],x,y,z);

  int offset[3][3];
  //fprintf(stderr,"\n");
  int dm=draw_mode;
  switch (dm)
    {
    case 7: dm=8; break;
    case 8: dm=10; break;
    case 9: dm=12; break;
    case 10: dm=50; draw_mode3D=1; break;
    }
      

  for (int ia=0;ia<=2;ia++)
    {
      offset[ia][0]=Irange(lv[ia]-dm+1,0,dim[ia]-1);
      offset[ia][1]=Irange(lv[ia]+dm-1,0,dim[ia]-1);
    }
  
  if (draw_mode3D==0)
    {
      offset[this->Paint2DAxis][0]=lv[this->Paint2DAxis];
      offset[this->Paint2DAxis][1]=lv[this->Paint2DAxis];
    }

  int seed[3];
  for (int ia=0;ia<=2;ia++)
    seed[ia]=lv[ia]-offset[ia][0];


  //  fprintf(stderr,"draw_mode=%d, dm=%d, d3d=draw_mode3D\n",draw_mode,dm,draw_mode3D);
      
  PXBool thrmode=fillMode->getState();
  PXBool cmode=connectFillMode->getState();
  float a[2];
  a[0]=atof(fillThresholds[0]->getText());
  a[1]=atof(fillThresholds[1]->getText());

  int selectcolor=0;

  if (this->CurrentImageIsColor)
    selectcolor=Irange(this->colorSelection->getIndex(),0,2);
  

  vtkImageData* sourceImage=this->MainImage;
  int domask=0;

  if (baseImageMode->getState()==PXTrue)
    domask=1;

  vtkDataArray* elements;
  if (state==0)
    elements=this->UndoStack->GetNextArray();
  else
    elements=this->UndoStack->GetCurrentArray();

  if (elements==NULL)
    {
      fprintf(stderr,"Bad Undo Array ....\n");
    }
  
  this->UpdateUndoIndicesDisplay();

  if ( (cmode==PXTrue && thrmode==PXTrue) || draw_mode==10)
    {
      double vl=sourceImage->GetScalarComponentAsDouble(lv[0],lv[1],lv[2],selectcolor);
      if (vl>=a[0] && vl<=a[1])
	{
	  vtkImageThreshold* thr=vtkImageThreshold::New();
	  int dvoi[3];
	  
	  vtkpxImageExtractVOI* voi=vtkpxImageExtractVOI::New();
	  voi->SetInput(sourceImage);
	  voi->SetVOI(offset[0][0],offset[0][1],offset[1][0],offset[1][1],offset[2][0],offset[2][1]);
	  voi->Update();
	  voi->GetOutput()->GetDimensions(dvoi);
	  
	  if (this->CurrentImageIsColor)
	    {
	      vtkImageExtractComponents* extr=vtkImageExtractComponents::New();
	      extr->SetInput(voi->GetOutput());
	      //	      fprintf(stderr,"Using Color=%d\n",selectcolor);
	      extr->SetComponents(selectcolor);
	      extr->Update();
	      thr->SetInput(extr->GetOutput());
	      extr->Delete();
	    }
	  else
	    {
	      thr->SetInput(voi->GetOutput());
	    }
	  voi->Delete();
	  
	  thr->ThresholdBetween(a[0],a[1]);
	  thr->SetInValue(1);
	  thr->SetOutValue(0);
	  thr->ReplaceInOn();
	  thr->ReplaceOutOn();
	  thr->SetOutputScalarTypeToUnsignedChar();
	  
	  vtkImageSeedConnectivity* connectivity=vtkImageSeedConnectivity::New();
	  connectivity->SetInput(thr->GetOutput());
	  connectivity->SetInputConnectValue(1);
	  connectivity->AddSeed(seed[0],seed[1],seed[2]);
	  connectivity->SetOutputConnectedValue(1);
	  connectivity->SetOutputUnconnectedValue(0);
	  connectivity->SetDimensionality(3);
	  connectivity->Update();
	  
	  vtkImageData* tmp=connectivity->GetOutput();
	  
	  for (int i=0;i<dvoi[0];i++)
	    for (int j=0;j<dvoi[1];j++)
	      for (int k=0;k<dvoi[2];k++)
		{
		  double vl=tmp->GetScalarComponentAsDouble(i,j,k,0);
		  if (vl>0)
		    {
		      int m=0;
		      if (domask)
			m=(this->Objectmap->GetScalarComponentAsDouble(i+offset[0][0],j+offset[1][0],k+offset[2][0],0)>0);
		      if (m==0)
			this->ChangeObjectmapValue(elements,i+offset[0][0],j+offset[1][0],k+offset[2][0],current_fill_color);
		    }
		}
	  connectivity->Delete();
	  thr->Delete();
	}
      /*      else 
	{
	  fprintf(stdout,"No Dice\n");
	  }*/
    }
  else
    {
      for (int i=offset[0][0];i<=offset[0][1];i++)
	for (int j=offset[1][0];j<=offset[1][1];j++)
	  for (int k=offset[2][0];k<=offset[2][1];k++)
	    {
	      if (thrmode==PXFalse)
		{
		  int m=0;
		  if (domask)
		    m=(this->Objectmap->GetScalarComponentAsDouble(i,j,k,0)>0);
		  if (m==0)
		    this->ChangeObjectmapValue(elements,i,j,k,current_fill_color);
		}
	      else
		{
		  double vl=sourceImage->GetScalarComponentAsDouble(i,j,k,selectcolor);
		  if (vl>=a[0] && vl<=a[1])
		    {
		      int m=0;
		      if (domask)
			m=(this->Objectmap->GetScalarComponentAsDouble(i,j,k,0)>0);
		      if (m==0)
			this->ChangeObjectmapValue(elements,i,j,k,current_fill_color);
		    }
		}
	    }
    }
  
  this->UpdateViewerObjectmap();
  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIAbstractMultisliceEditor::DisablePainting()
{
  if (this->draw_mode==0)
    return 0;
  this->draw_mode=0;
  paintMode->setIndex(0,PXFalse);
  return 1;
}

int vtkpxGUIAbstractMultisliceEditor::EnablePainting()
{
  if (this->draw_mode!=0)
    return 0;
  this->draw_mode=1;
  paintMode->setIndex(1,PXFalse);
  return 1;
}

PXTkFrame* vtkpxGUIAbstractMultisliceEditor::GetExtraFrame() 
{ 
  return this->extraFrame; 
}

char* vtkpxGUIAbstractMultisliceEditor::GetExtraFrameName()
{
  if (this->extraFrame==NULL)
    return NULL;

  return this->extraFrame->getWidgetName();
}

int vtkpxGUIAbstractMultisliceEditor::CreateFillControl(PXTkFrame* fillframe)
{

  PXTkFrame* voiNameFrame=new PXTkFrame(this->EventManager,fillframe);
  PXTkFrame* colorframe1=new PXTkFrame(this->EventManager,fillframe);
  PXTkFrame* colorframe2=new PXTkFrame(this->EventManager,fillframe);
  editcolorframe=new PXTkFrame(this->EventManager,fillframe);
  PXTkFrame* fillmodeframe=new PXTkFrame(this->EventManager,fillframe);
  PXTkFrame* fillmodeframe2=new PXTkFrame(this->EventManager,fillframe);
  PXTkFrame* buttonframe=new PXTkFrame(this->EventManager,fillframe);
  PXTkFrame* paintframe=new PXTkFrame(this->EventManager,fillframe);
  this->extraFrame=new PXTkFrame(this->EventManager,fillframe);
  
  PXTkLabelFrame* buttonframe2=new PXTkLabelFrame(this->EventManager,fillframe,"Colormap");
  PXTkLabelFrame* buttonframe3=new PXTkLabelFrame(this->EventManager,fillframe,"Objectmap Smoothing");


  PXTkButton* b11=new PXTkButton(this->EventManager,buttonframe2,"Load",803);
  PXTkButton* b21=new PXTkButton(this->EventManager,buttonframe2,"Save",804);
  PXTkButton* b31=new PXTkButton(this->EventManager,buttonframe2,"Restore",805);
  buttonframe2->addChildren("-side left -padx 1 -expand true -fill x",b11,b21,b31);


  fillframe->addChildren("-side bottom -expand false -fill x -pady 1",buttonframe2,buttonframe3);
  fillframe->addChildren("-side top -expand false -fill x -pady 1",voiNameFrame,colorframe1,colorframe2,editcolorframe);
  fillframe->addChildren("-side top -expand false -fill x -pady 1",paintframe,fillmodeframe,fillmodeframe2);
  fillframe->addChildren("-side top -expand false -fill x -pady 1",buttonframe,extraFrame);

  if (this->SimpleMode==0)
    {
      PXTkButton* popupbut=new PXTkButton(this->EventManager,editcolorframe,"More Colors",802);
      editcolorframe->addChildren("-side right -padx 0 -expand false -fill x",popupbut);
    }
  PXTkButton* b5=new PXTkButton(this->EventManager,editcolorframe,"Edit Color",806);
  editcolorframe->addChildren("-side left -padx 10",b5);

  voiNameEntry=new PXTkEntry(this->EventManager,voiNameFrame);
  voiNameEntry->configure("-fg","black");
  voiNameEntry->configure("-bg","white");
  voiNameEntry->setText(voiNames[current_fill_color].c_str());
  PXTkButton* voiNameUpdate=new PXTkButton(this->EventManager,voiNameFrame,"Upd",810);
  voiNameFrame->addChildren("-side right -padx 2 -expand false",voiNameUpdate);
  voiNameFrame->addChildren("-side left -padx 0 -expand true -fill x",voiNameEntry);


  for (int k=0;k<pxAME_NUM_COLOR_BUTTONS;k++)
    {
      sprintf(pxtk_buffer,"%d",k);
      if (k==0)
	colorButton[k]=new PXTkRadioButton(this->EventManager,colorframe1,pxtk_buffer,821+k);
      else if (k<=4)
	colorButton[k]=new PXTkRadioButton(this->EventManager,colorframe1,pxtk_buffer,821+k,colorButton[0]);
      else
	colorButton[k]=new PXTkRadioButton(this->EventManager,colorframe2,pxtk_buffer,821+k,colorButton[0]);
    
      colorButton[k]->configure("-width",4);

      if (k<=4)
	colorframe1->addChildren("-side left -padx 0 -expand true -fill x",colorButton[k]);
      else
	colorframe2->addChildren("-side left -padx 0 -expand true -fill x",colorButton[k]);
    }

  this->colorButtonDialog=new PXTkDialog(this->EventManager,colorframe2,"Select Colors",PXFalse);
  this->colorButtonDialog->initDisplay();

  PXTkFrame* cframe=(PXTkFrame*)this->colorButtonDialog->getMainWindow();

  PXTkButton* closebut=new PXTkButton(colorButtonDialog,cframe,"Close",3);
  cframe->addChildren("-side bottom -padx 0 -expand true -fill x",closebut);

  int maxc=vtkpxSurfaceUtil::DefaultObjectMapLookupTableNumberOfColors();
  if (maxc>pxAME_NUM_DIALOG_COLOR_BUTTONS)
    maxc=pxAME_NUM_DIALOG_COLOR_BUTTONS;

  for (int i=0;i<maxc;i+=2)
    {
      PXTkFrame* afr=new PXTkFrame(this->EventManager,cframe);
      cframe->addChildren("-side top -padx 0 -expand true -fill x",afr);
      sprintf(pxtk_buffer,"%d",i);
      this->dialogColorButton[i]=new PXTkRadioButton(this->EventManager,afr,pxtk_buffer,821+i,colorButton[0]);
      afr->addChildren("-side left -padx 0 -expand true -fill x",this->dialogColorButton[i]);
      if ((i+1)<maxc)
	{
	  sprintf(pxtk_buffer,"%d",i+1);
	  this->dialogColorButton[i+1]=new PXTkRadioButton(this->EventManager,afr,pxtk_buffer,821+i+1,colorButton[0]);
	  afr->addChildren("-side left -padx 0 -expand true -fill x",this->dialogColorButton[i+1]);
	}
    }

  colorButtonDialog->popdownDialog();



  PXTkLabel*  md=new PXTkLabel(this->EventManager,paintframe,"Mode (Shift-Mouse 1 to Toggle On/Off):");
  md->configure("-bg","black");
  md->configure("-fg","white");
  md->configure("-padx","0");

  paintMode=new PXTkOptionMenu(this->EventManager,paintframe,"Off",800);
  paintMode->addOption("Brush-1");
  paintMode->addOption("Brush-2");
  paintMode->addOption("Brush-3");
  paintMode->addOption("Brush-4");
  paintMode->addOption("Brush-5");
  paintMode->addOption("Brush-6");
  paintMode->addOption("Brush-8");
  paintMode->addOption("Brush-10");
  paintMode->addOption("Brush-12");
  paintMode->addOption("Brush-50");
  paintMode->setIndex(this->draw_mode);

  paintframe->addChildren("-side left -padx 2",md,paintMode);


  PXTkLabel*  md2=new PXTkLabel(this->EventManager,buttonframe3,"Factor:");
  md->configure("-padx","0");

  this->smoothMode=new PXTkOptionMenu(this->EventManager,buttonframe3,"1.0",-1);
  this->smoothMode->addOption("2.0");
  this->smoothMode->addOption("4.0");
  this->smoothMode->addOption("8.0");
  this->smoothMode->addOption("16.0");
  this->smoothMode->addOption("32");
  this->smoothMode->addOption("64.0");
  this->smoothMode->setIndex(3);

  PXTkLabel*  md3=new PXTkLabel(this->EventManager,buttonframe3,"Conv:");
  md3->configure("-padx","0");

  this->smoothConvergenceMode=new PXTkOptionMenu(this->EventManager,buttonframe3,"0.025",-1);
  this->smoothConvergenceMode->addOption("0.05");
  this->smoothConvergenceMode->addOption("0.1");
  this->smoothConvergenceMode->addOption("0.2");
  this->smoothConvergenceMode->addOption("0.4");
  this->smoothConvergenceMode->addOption("0.8");
  this->smoothConvergenceMode->setIndex(3);

  PXTkButton* smoothbut=new PXTkButton(this->EventManager,buttonframe3,"Smooth",809);

  paintframe->addChildren("-side right -padx 2",smoothbut);
  paintframe->addChildren("-side left -padx 1 -expand true -fill x",md2,this->smoothMode,md3,this->smoothConvergenceMode);

  if (this->SingleSliceMode!=1)
    {
      paintMode3D=new PXTkCheckButton(this->EventManager,fillmodeframe2,"3D",801);
      editcolorframe->addChildren("-side left -padx 2",paintMode3D);
    }


  fillMode=new PXTkCheckButton(this->EventManager,fillmodeframe2,"Use Thrshld",-1);
  connectFillMode=new PXTkCheckButton(this->EventManager,fillmodeframe2,"Connect",-1);
  baseImageMode=new PXTkCheckButton(this->EventManager,fillmodeframe2,"No Overwrite",-1);
  fillmodeframe2->addChildren("-side left -padx 0 -expand false -fill x",baseImageMode,fillMode,connectFillMode);

  for (int ia=0;ia<=1;ia++)
    {
      fillThresholds[ia]=new PXTkEntry(this->EventManager,fillmodeframe);
      fillThresholds[ia]->configure("-width",6);
      sprintf(pxtk_buffer,"%f",300.0*float(ia));
      fillThresholds[ia]->setText(pxtk_buffer);
    }

  this->colorSelection=new PXTkOptionMenu(this->EventManager,fillmodeframe,"R",-1);
  this->colorSelection->addOption("G");
  this->colorSelection->addOption("B");
  fillmodeframe->addChildren("-side right -padx 1 -expand f ",this->colorSelection);
  this->colorSelection->enable(PXFalse);

  fillmodeframe->addChildren("-side left -padx 1 -expand t -fill x",fillThresholds[0],fillThresholds[1]);

  PXTkButton* b01=new PXTkButton(this->EventManager,buttonframe,"Undo",807);
  PXTkButton* b02=new PXTkButton(this->EventManager,buttonframe,"Redo",808);
  buttonframe->addChildren("-side left -padx 1 -expand true -fill x",b01,b02);


  UpdateColorButtons();

  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIAbstractMultisliceEditor::UpdateObjectmapColormap()
{
  int num=ObjectmapLookupTable->GetNumberOfColors();
  UpdateColorButtons();
  this->ObjectmapLookupTable->Modified();
  this->UpdateViewerObjectmapColormap();
  this->Objectmap->Modified();
  this->UpdateViewerObjectmap();
  this->UpdateViewerDisplay();
  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIAbstractMultisliceEditor::LoadObjectmapColormap()
{
  char* line=this->EventManager->getOpenFilename("Objectmap Colormap File name","Colormap Files","*.cmap*");
  
  if (line==NULL)
    return 0;
  if (strlen(line)==0)
    return 0;
  strcpy(pxtk_buffer2,line);

  gzFile fin=gzsuffixopen(pxtk_buffer2,"rb");
  if (!fin)
    return 0;
  
  gzgets(fin,pxtk_buffer,100);
  if (gzfindstringinstring(pxtk_buffer,"#IPAGRGBAColormap")==0)
    {
      gzclose(fin);
      return 0;
    }
  
  gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
  int numc;  sscanf(pxtk_buffer,"%d",&numc);

  int maxc=vtkpxSurfaceUtil::DefaultObjectMapLookupTableNumberOfColors();
  if (numc>maxc)
    numc=maxc;


  double dat[2];
  gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
  sscanf(pxtk_buffer,"%lf %lf",&dat[0],&dat[1]);

  if (numc>ObjectmapLookupTable->GetNumberOfColors())
    {
      ObjectmapLookupTable->SetNumberOfTableValues(numc);
      ObjectmapLookupTable->SetTableRange(dat);
    }

  //  double d[2];     ObjectmapLookupTable->GetTableRange(d);
  //  fprintf(stderr,"New Range = %f:%f\n",d[0],d[1]);
  

  gzgets(fin,pxtk_buffer,100);  gzgets(fin,pxtk_buffer,100);
  for (int i=0;i<numc;i++)
    {
      double val[4];
      int tmp;
      gzgets(fin,pxtk_buffer,100);
      sscanf(pxtk_buffer,"%d %lf %lf %lf %lf",&tmp,&val[0],&val[1],&val[2],&val[3]);
      ObjectmapLookupTable->SetTableValue(tmp,val);
    }
  gzclose(fin);
    
  delete [] objectmapcolormapname;
  objectmapcolormapname=new char[strlen(pxtk_buffer2)+1];
  strcpy(objectmapcolormapname,pxtk_buffer2);


  this->UpdateObjectmapColormap();
  return 1;

}
// -------------------------------------------------------------------------
int vtkpxGUIAbstractMultisliceEditor::SaveObjectmapColormap()
{
  char* line=this->EventManager->getSaveFilename("Save Objectmap Colormap",objectmapcolormapname,"Colormaps","*.cmap");
  if (line==NULL)
    return 0;
  if (strlen(line)==0)
    return 0;
  
  strcpy(pxtk_buffer2,line);

  FILE* fout=fopen(pxtk_buffer2,"w");
  if (!fout)
    return 0;

  fprintf(fout,"#IPAGRGBAColormap\n");
  fprintf(fout,"#Number of Colors\n%d\n",ObjectmapLookupTable->GetNumberOfColors());

  double dat[2];
  ObjectmapLookupTable->GetTableRange(dat);
  fprintf(fout,"#Range\n %6.3f %6.3f \n",dat[0],dat[1]);

  fprintf(fout,"#Colors\n#Index  R     G     B  Alpha\n");
  for (int i=0;i<ObjectmapLookupTable->GetNumberOfColors();i++)
      {
	double val[4];
	ObjectmapLookupTable->GetTableValue(i,val);
	fprintf(fout,"%4d %5.3f %5.3f %5.3f %5.3f\n",i,val[0],val[1],val[2],val[3]);
      }
  fclose(fout);

  if (strcmp(objectmapcolormapname,pxtk_buffer2)!=0)
      {
	delete [] objectmapcolormapname;
	objectmapcolormapname=new char[strlen(pxtk_buffer2)+1];
	strcpy(objectmapcolormapname,pxtk_buffer2);
      }
  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIAbstractMultisliceEditor::UpdateColorButtons()
{
  if (ObjectmapLookupTable==NULL)
    return 0;
  int num=ObjectmapLookupTable->GetNumberOfColors();

  
  int maxc=vtkpxSurfaceUtil::DefaultObjectMapLookupTableNumberOfColors();
  if (maxc>pxAME_NUM_DIALOG_COLOR_BUTTONS)
    maxc=pxAME_NUM_DIALOG_COLOR_BUTTONS;


  if (num>maxc)
    num=maxc;



  for (int k=0;k<num;k++)
    {
      double cd[4]; ObjectmapLookupTable->GetTableValue(k,cd);
      int c[3];
      strcpy(pxtk_buffer,"#");
      
      for (int i=0;i<=2;i++)
	{
	  c[i]=int(cd[i]*255.0);
	  for (int byte=0;byte<=1;byte++)
	    {
	      int val=0;
	      if (byte==0)
		val=c[i]/16;
	      else
		val=c[i]-16*int(c[i]/16);
	      
	      switch (val)
		{
		case 10:    strcat(pxtk_buffer,"A");  break;
		case 11:    strcat(pxtk_buffer,"B");  break;
		case 12:    strcat(pxtk_buffer,"C");  break;
		case 13:    strcat(pxtk_buffer,"D");  break;
		case 14:    strcat(pxtk_buffer,"E");  break;
		case 15:    strcat(pxtk_buffer,"F");  break;
		default:    sprintf(pxtk_buffer,"%s%1d",pxtk_buffer,val); break;
		  
		}
	    }
	}
      //      fprintf(stderr,"k=%d, value=%.2f,%.2f,%.2f,%.2f --> %s\n",k,cd[0],cd[1],cd[2],cd[3],pxtk_buffer);

      if (k<pxAME_NUM_COLOR_BUTTONS)
	{
	  if (colorButton[k]!=NULL)
	    {
	      if (k<num)
		{
		  
		  colorButton[k]->configure("-bg",pxtk_buffer);
		  colorButton[k]->enable(PXTrue);
		}
	      else
		{
		  colorButton[k]->configure("-bg","black");
		  colorButton[k]->enable(PXFalse);
		}
	    }
	}

      //      fprintf(stderr,"On to Dialog Color Button k=%d\n");

      if (k<maxc)
	{
	  if (dialogColorButton[k]!=NULL)
	    {
	      //	      fprintf(stderr,"Doing Dialog Button %d\n",k);
	      if (k<num)
		{
		  //		  fprintf(stderr,"Configuring Dialog Button %d\n",k);
		  dialogColorButton[k]->configure("-bg",pxtk_buffer);
		  dialogColorButton[k]->enable(PXTrue);
		}
	      else
		{
		  dialogColorButton[k]->configure("-bg","black");
		  dialogColorButton[k]->enable(PXFalse);
		}
	    }
	}
      //fprintf(stderr,"\t\tDone k=%d\n",k);
    }
  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIAbstractMultisliceEditor::EditColorButton(int n)
{
  if (n<0 || n>=vtkpxSurfaceUtil::DefaultObjectMapLookupTableNumberOfColors())
    return 0;
  if (dialogColorButton[n]==NULL)
    return 0;

  double c[3],cd[4];
  ObjectmapLookupTable->GetTableValue(n,cd);
  sprintf(pxtk_buffer2,"Color %d",n);
  int a=this->EventManager->d_colorBox(pxtk_buffer2,cd[0],cd[1],cd[2]);
  if (a==1)
    {
      ObjectmapLookupTable->SetTableValue(n,cd);
      this->UpdateObjectmapColormap();
      char* line=dialogColorButton[n]->cget("-bg");
      strcpy(pxtk_buffer,line);
      editcolorframe->configure("-bg",pxtk_buffer);
    }

  return a;
}
// -------------------------------------------------------------------------
void vtkpxGUIAbstractMultisliceEditor::RenamePaintOffLabel(const char* newname)
{
  if (this->paintMode==NULL)
    return;

  this->paintMode->changeLabel(0,newname);

}

// -------------------------------------------------------------------------
int vtkpxGUIAbstractMultisliceEditor::SmoothObjectmap()
{

  if (this->Objectmap==NULL)
    return 0;

  double lambda=2.0;
  if (this->smoothMode!=NULL)
    {
      double v=(double)this->smoothMode->getIndex();
      lambda=pow(2.0,v);
    }

  double epsilon=0.2;
  if (this->smoothConvergenceMode!=NULL)
    {
      double v=(double)this->smoothConvergenceMode->getIndex();
      epsilon=0.025*pow(2.0,v);
    }

  vtkbisObjectmapRegularization* segm=vtkbisObjectmapRegularization::New();
  segm->SetInput(this->Objectmap);
  segm->SetSmoothness(lambda);
  segm->SetConvergencePercentage(epsilon);
  segm->Update();


  double p=this->SetObjectmapWithUndo(segm->GetOutput());

  char pxtk_buffer4[200];
  if (p>0.0)
    sprintf(pxtk_buffer4,"Objectmap smoothed %.2f %% of all voxels changed",p);
  else
    sprintf(pxtk_buffer4,"Objectmap smoothing failed.",p);
  
  this->EventManager->messageBox(pxtk_buffer4,"Objectmap Smoothing");
  
  segm->Delete();
  return 1;
}
// ------------------------------------------------------------------------------
int vtkpxGUIAbstractMultisliceEditor::SetVOISelectMode(double minthr,double maxthr,int force3dbrush)
{
  if (fillThresholds[0]==NULL || fillThresholds[1]==NULL)
    return 0;

  sprintf(pxtk_buffer,"%.2f",minthr);  fillThresholds[0]->setText(pxtk_buffer);
  sprintf(pxtk_buffer,"%.2f",maxthr);  fillThresholds[1]->setText(pxtk_buffer);

  if (force3dbrush==0)
    return 1;

  if (fillMode!=NULL)
    fillMode->setState(PXTrue);
  if (connectFillMode!=NULL)
    connectFillMode->setState(PXTrue);
  if (this->paintMode3D!=NULL)
    paintMode3D->setState(PXTrue);

  this->draw_mode=10;
  this->draw_mode3D=1;
  this->paintMode->setIndex(10);
  return 1;
}
// ------------------------------------------------------------------------------
int vtkpxGUIAbstractMultisliceEditor::SetVOIName(int index,char* name)
{
if (index<0)
    index=0;
 if (index>=pxAME_NUM_DIALOG_COLOR_BUTTONS)
   index=pxAME_NUM_DIALOG_COLOR_BUTTONS-1;
 
  if (strlen(name)>0)
    {
      voiNames[index]=name;

      if (voiNameEntry!=NULL && current_fill_color==index)
	voiNameEntry->setText(voiNames[index].c_str());
      return 1;
    }
  
  return 0;
}

const char* vtkpxGUIAbstractMultisliceEditor::GetVOIName(int index)
{
  if (index<0)
    index=0;
  if (index>=pxAME_NUM_DIALOG_COLOR_BUTTONS)
    index=pxAME_NUM_DIALOG_COLOR_BUTTONS-1;
  
  return voiNames[index].c_str();
}

int vtkpxGUIAbstractMultisliceEditor::GetNumberOfVOIS()
{
  return pxAME_NUM_DIALOG_COLOR_BUTTONS;
}

void vtkpxGUIAbstractMultisliceEditor::ResetVOINames()
{
  voiNames[0]="Background";
  for (int ia=1;ia<pxAME_NUM_DIALOG_COLOR_BUTTONS;ia++)
    {
      sprintf(pxtk_buffer,"VOI %d",ia);
      voiNames[ia]=pxtk_buffer;
    }

  if (voiNameEntry!=NULL)
    voiNameEntry->setText(voiNames[current_fill_color].c_str());


}

