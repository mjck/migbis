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
  Module:    $RCSfile: vtkpxGUIObjectmapOrthogonalViewer.cpp,v $
  Language:  C++
  Date:      $Date: 2004/04/27 18:52:50 $
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
#include "pxtkframe.h"
#include "vtkActor.h"
#include "vtkCellPicker.h"
#include "vtkCubeSource.h"
#include "vtkDataSetMapper.h"
#include "vtkLineSource.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkOutlineFilter.h"
#include "vtkPoints.h"
#include "vtkPolyDataMapper.h"
#include "vtkProbeFilter.h"
#include "vtkRenderer.h"
#include "vtkRendererCollection.h"

#include "vtkSphereSource.h"
#include "vtkPolyData.h"
#include "vtkpxColorMapUtil.h"
#include "vtkpxGUIObjectmapOrthogonalViewer.h"
#include "vtkpxGUIRenderer.h"
#include "vtkpxGUIVolumeControl.h"
#include "vtkpxGUIObliqueImageSliceControl.h"
#include "vtkpxImageExtract.h"
#include "vtkpxImageSlice.h"
#include "vtkpxObliqueImageSlice.h"
#include "vtkpxOrthoImageSlice.h"
#include "vtkpxTalairachTransform.h"
#include "vtkpxVolume.h"

#include "vtkImageSeedConnectivity.h"
#include "vtkImageDataGeometryFilter.h"
#include "vtkThresholdPoints.h"
#include "vtkImageThreshold.h"
#include "vtkpxImageExtractVOI.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkImageThreshold.h"

#include "vtkpxGUIObjectmapOrthogonalEditor.h"
#include "vtkpxGUI2DSplineObjectmapViewer.h"
#include "vtkpxGUI2DImageEditor.h"
#include "vtkstd/string"

/*#include "vtkImageMapToRGBA.h"
  #include "vtkCollection.h"
  #include "vtkImageQuantizeRGBToIndex.h"
  #include "vtkImageBlend.h"*/

// -------------------------------------------------------------------------
vtkpxGUIObjectmapOrthogonalViewer* vtkpxGUIObjectmapOrthogonalViewer::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIObjectmapOrthogonalViewer");
  if(ret)
      {
	return (vtkpxGUIObjectmapOrthogonalViewer*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIObjectmapOrthogonalViewer;
}

// Construct object with no children.
vtkpxGUIObjectmapOrthogonalViewer::vtkpxGUIObjectmapOrthogonalViewer()
{
  for (int i=0;i<=2;i++)
    maskImageSlice[i]=NULL;
  maskOrthoSlice=NULL;
  maskTransparency=NULL;
  SplineObjectmapEditor=NULL;
  maskImage=NULL;
  maskLookupTable=NULL;
  maskVolumeLookupTable=NULL;
  //  this->ColorMode=0;

  this->create_internal_editor=0;
  this->InternalEditor=NULL;
}

vtkpxGUIObjectmapOrthogonalViewer::~vtkpxGUIObjectmapOrthogonalViewer()
{
  for (int i=0;i<=2;i++)
    if (maskImageSlice[i]!=NULL)
      maskImageSlice[i]->Delete();

  if (maskOrthoSlice!=NULL)
    maskOrthoSlice->Delete();

  if (maskImage!=NULL)
    this->maskImage->Delete();

  if (maskLookupTable!=NULL)
    this->maskLookupTable->Delete();

  if (maskVolumeLookupTable!=NULL)
    this->maskVolumeLookupTable->Delete();

}
/* -------------------------------------------------------------------------*/
void vtkpxGUIObjectmapOrthogonalViewer::SetCreateInternalEditorOn()
{
  if (this->SplineObjectmapEditor!=NULL)
    return;

  if (this->Initialized==0)
    create_internal_editor=1;
}
/* -------------------------------------------------------------------------*/

int vtkpxGUIObjectmapOrthogonalViewer::HandleEvent(int event)
{
  if (event>0 && event<10)
      return this->EventManager->handleDirectEvent(event);

  if (event<400)
    return vtkpxGUIOrthogonalViewer::HandleEvent(event);

  if (!hasImage)
    return TCL_OK;


  if (event==400)
    {
      if (  this->maskImageSlice[0]!=NULL)
	{
	  this->SetObjectmapTransparency(this->maskTransparency->getValue());
	}
    }

  if (event==401)
    {
      if (this->InternalEditor!=NULL)
	this->InternalEditor->Show();
    }

  return TCL_OK;
     
}
// -------------------------------------------------------------------------
void vtkpxGUIObjectmapOrthogonalViewer::InitControls(int a)
{
  vtkpxGUIOrthogonalViewer::InitControls(a);
  // Put extra item in extraFrame

  maskTransparency=new PXTkArrowScale(this->EventManager,extraFrame,"Mask:",400,PXTrue);
  maskTransparency->setRange(0.0,100.0);
  maskTransparency->setResolution(1.0);
  maskTransparency->setValue(50.0);
  maskTransparency->setLengthWidth(80,15);


  extraFrame->addChildren("-side left -expand true -fill x",maskTransparency->getMainWindow());

  if (create_internal_editor==0)
    return;

  PXTkButton* ut5=new PXTkButton(this->EventManager,controlFrame,"Show Paint Controls",401); 
  ut5->configure("-fg","red");
  controlFrame->addChildren("-side bottom -expand true -fill x -padx 5",ut5);
  
  this->InternalEditor=vtkpxGUIObjectmapOrthogonalEditor::New();
  this->InternalEditor->Initialize(this->GetWidgetName(),0);
  //  this->InternalEditor->Initialize(this->GetWidgetName(),0);
  this->InternalEditor->SetObjectmapViewer(this);

}

void vtkpxGUIObjectmapOrthogonalViewer::UpdateMaskVolumeLookupTableAndImage()
{
  if (this->Volume==NULL)
    return;

  double opacity=0.5;

  //  fprintf(stderr,"opacity\n");
  if (  this->maskImageSlice[0]!=NULL)
    opacity=this->maskTransparency->getValue()*0.01;

  //fprintf(stderr,"This far\n");

  if (maskVolumeLookupTable==NULL)
    fprintf(stderr,"It is null\n");


  if (maskVolumeLookupTable->GetNumberOfColors()!=maskLookupTable->GetNumberOfColors())
    {
    //fprintf(stderr,"Setting num values\n");
      maskVolumeLookupTable->SetNumberOfTableValues(maskLookupTable->GetNumberOfColors());
    }

  //fprintf(stderr,"Setting range\n");

  maskVolumeLookupTable->SetTableRange(maskLookupTable->GetTableRange());

  //fprintf(stderr,"vol\n");

  for (int i=0;i<maskVolumeLookupTable->GetNumberOfColors();i++)
    {
      double v[4]; maskLookupTable->GetTableValue(i,v);
      if (i>0)
	v[3]=opacity;
      else
	v[3]=0.0;
      maskVolumeLookupTable->SetTableValue(i,v);
    }

  //fprintf(stderr,"Fixed MaskVolume Table\n");

  this->Volume->SetSecondLookupTable(this->maskVolumeLookupTable);

  //fprintf(stderr,"Done Setting Lookup Table\n");
  this->Volume->SetSecondInput(this->maskImage);

  //fprintf(stderr,"Done Setting Inputs\n");
}


// -------------------------------------------------------------------------
int vtkpxGUIObjectmapOrthogonalViewer::AutoCreateMaskAndLookupTable()
{
  if (this->maskImage==NULL)
    {
      this->maskImage=vtkImageData::New();
    }
  else
    {
      int ok=this->DoImagesHaveSameDimensions(this->maskImage,this->currentImage,0);
      if (ok==1)
	return 1;
    }


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

  /*  this->maskImage->CopyStructure(currentImage);
  this->maskImage->SetScalarTypeToUnsignedChar();
  this->maskImage->AllocateScalars();
  this->maskImage->GetPointData()->GetScalars()->FillComponent(0,0.0);*/

  if (this->maskLookupTable==NULL)
    maskLookupTable=vtkLookupTable::New();

  vtkpxSurfaceUtil::DefaultObjectMapLookupTable(maskLookupTable,28,1);

  if (this->maskVolumeLookupTable==NULL)
    maskVolumeLookupTable=vtkLookupTable::New();

  this->UpdateMaskVolumeLookupTableAndImage();

  if (this->InternalEditor!=NULL)
    this->InternalEditor->ResetVOINames();


  return 1;
}

int vtkpxGUIObjectmapOrthogonalViewer::SetObjectLookupTable(vtkLookupTable* lkp)
{
  return this->SetObjectLookupTable(lkp,1);
}

int vtkpxGUIObjectmapOrthogonalViewer::SetObjectLookupTable(vtkLookupTable* lkp,int updateeditor)
{

  if (lkp==NULL)
    return 0;
  if (this->maskLookupTable==NULL)
    maskLookupTable=vtkLookupTable::New();

  vtkpxColorMapUtil::CopyLookupTable(lkp,this->maskLookupTable);

  this->UpdateMaskVolumeLookupTableAndImage();
  this->UpdateDisplay();

  if (this->InternalEditor!=NULL && updateeditor==1)
    {
      //fprintf(stderr,"Updating Editor Objectmap Colormap");
      this->InternalEditor->UpdateFromViewer(1);
    }

  for (int ia=0;ia<=2;ia++)
    {
      if (maskImageSlice[ia]!=NULL) 
	{
	  maskImageSlice[ia]->UpdateTexture();
	  maskImageSlice[ia]->Modified();
	}
    }

  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIObjectmapOrthogonalViewer::InitializeViewer()
{
  if ((!hasImage && this->Initialized))
      return 0;

  if (this->SplineObjectmapEditor!=NULL)
    this->SplineObjectmapEditor->SetImage(this->currentImage);
    

  int ok=vtkpxGUIOrthogonalViewer::InitializeViewer();
  if (ok==0)
    return 0;

  this->maskImage=vtkImageData::New();
  if (this->SplineObjectmapEditor!=NULL)
    {
      maskImage->ShallowCopy(SplineObjectmapEditor->GetMaskImage());
      if (this->maskLookupTable==NULL)
	maskLookupTable=vtkLookupTable::New();
      
      vtkpxSurfaceUtil::DefaultObjectMapLookupTable(maskLookupTable,28,1);
      
      if (this->maskVolumeLookupTable==NULL)
	maskVolumeLookupTable=vtkLookupTable::New();
      
      this->UpdateMaskVolumeLookupTableAndImage();
    } 
  else
    this->AutoCreateMaskAndLookupTable();
  
  for (int k=0;k<=2;k++)
    {
      maskImageSlice[k]=vtkpxImageSlice::New();
      maskImageSlice[k]->SetAutoUpdate(0);
      maskImageSlice[k]->SetInput(maskImage);
      maskImageSlice[k]->SetFrame(0);
      maskImageSlice[k]->SetCurrentPlane(k);
      maskImageSlice[k]->PickableOff();
      maskImageSlice[k]->SetOpacity(0.5);
      maskImageSlice[k]->SetInterpolation(0);

      renderer[k]->GetRenderer()->AddActor(maskImageSlice[k]);
    }

  maskOrthoSlice=vtkpxOrthoImageSlice::New();
  maskOrthoSlice->SetAutoUpdate(0);
  maskOrthoSlice->SetInput(maskImage);
  maskOrthoSlice->SetFrame(0);
  maskOrthoSlice->PickableOff();
  maskOrthoSlice->SetOpacity(0.5);
  maskOrthoSlice->SetInterpolation(0);

  renderer[3]->GetRenderer()->AddActor(maskOrthoSlice);
  maskOrthoSlice->SetVisibility(this->OrthoSliceVisibility);


  if (this->Volume!=NULL)
    {
      this->UpdateMaskVolumeLookupTableAndImage();
    }

  return 1;
}

// -------------------------------------------------------------------------
void vtkpxGUIObjectmapOrthogonalViewer::ChangeSliceLevels(int lv[3],int infoonly)
{
  if (this->Initialized==0 || !hasImage || imageSlice[0]==NULL)
    return;

  double sp[3];    currentImage->GetSpacing(sp);
  double ori[3];   currentImage->GetOrigin(ori);

  if (infoonly==0)
    {
      for (int ia=0;ia<=2;ia++)
	{
	  imageSlice[ia]->SetLevel(lv[ia]);
	  maskImageSlice[ia]->SetLevel(lv[ia]);
	}
      orthoSlice->SetLevels(lv[0],lv[1],lv[2]);
      maskOrthoSlice->SetLevels(lv[0],lv[1],lv[2]);
    }

  double lvv[3],lvs[3];
  for (int ic=0;ic<=2;ic++)
    {
      lvv[ic]=float(lv[ic]);
      lvs[ic]=lvv[ic]*sp[ic]+ori[ic];
    }
  this->SetLastClickedPoint(lvv);
  this->SetLastClickedPointScaled(lvs);

  if (infoonly==0)
    {
      MoveAxis(1);
    }
}
// -------------------------------------------------------------------------
void vtkpxGUIObjectmapOrthogonalViewer::ChangeFrame(int newframe)
{
  if (newframe==this->CurrentFrame)
    return;
   
  this->CurrentFrame=Irange(newframe,0,this->NumberOfFrames-1);
  for (int ia=0;ia<=2;ia++)
    {
      imageSlice[ia]->SetFrame(this->CurrentFrame);
      maskImageSlice[ia]->SetFrame(this->CurrentFrame);
    }
  orthoSlice->SetFrame(this->CurrentFrame);
  maskOrthoSlice->SetFrame(this->CurrentFrame);

  obliqueSlice->SetFrame(this->CurrentFrame);

  if (this->Volume!=NULL)
    this->Volume->SetFrame(this->CurrentFrame);

  this->UpdateIntensityLabelInfo();
  this->ExecuteFrameChangeCallback();
}
// -------------------------------------------------------------------------
void vtkpxGUIObjectmapOrthogonalViewer::SetOrthoSliceVisibility(int orthovis)
{
  vtkpxGUIOrthogonalViewer::SetOrthoSliceVisibility(orthovis);
  if (this->maskOrthoSlice!=NULL)
    this->maskOrthoSlice->SetVisibility(orthoSlice->GetVisibility());
}

void vtkpxGUIObjectmapOrthogonalViewer::SetVolumeVisibility(int volvis)
{
  vtkpxGUIOrthogonalViewer::SetVolumeVisibility(volvis);
}

void vtkpxGUIObjectmapOrthogonalViewer::SetObliqueSliceVisibility(int orthovis)
{
  vtkpxGUIOrthogonalViewer::SetObliqueSliceVisibility(orthovis);
}

// -------------------------------------------------------------------------
int vtkpxGUIObjectmapOrthogonalViewer::ResetViewerUpdatePolar()
{
  vtkpxGUIOrthogonalViewer::ResetViewerUpdatePolar();
  for (int sc=0;sc<=2;sc++)
    {
      maskImageSlice[sc]->SetLevel(imageSlice[sc]->GetLevel());
      maskImageSlice[sc]->Update();
      maskImageSlice[sc]->SetAutoUpdate(1);
    }

  int levels[3];
  orthoSlice->GetLevels(levels);
  maskOrthoSlice->SetAutoUpdate(0);
  maskOrthoSlice->SetLevels(levels[0],levels[1],levels[2]);
  maskOrthoSlice->Update();
  maskOrthoSlice->SetAutoUpdate(1);

  return 1;
}
// ------------------------------------------------------------------------- 
int vtkpxGUIObjectmapOrthogonalViewer::ResetViewerUpdateCartesian() 
{
  vtkpxGUIOrthogonalViewer::ResetViewerUpdateCartesian();
  int   range[3]; 
  currentImage->GetDimensions(range);

  for (int ia=0;ia<=2;ia++)
    {
      maskImageSlice[ia]->SetAutoUpdate(0);
      maskImageSlice[ia]->SetCurrentPlane(ia);
    }
  
  maskOrthoSlice->SetAutoUpdate(0);
  maskOrthoSlice->SetLevels(range[0]/2,range[1]/2,range[2]/2);
  maskOrthoSlice->Update();
  maskOrthoSlice->SetAutoUpdate(1);

  for (int kk=0;kk<=2;kk++)
    {
      maskImageSlice[kk]->SetLevel(range[kk]/2);
      maskImageSlice[kk]->Update();
      maskImageSlice[kk]->SetAutoUpdate(1);
    }
  
  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIObjectmapOrthogonalViewer::ResetViewer()
{
  //  fprintf(stderr,"In Reset Viewer %d oldorient=%d firsttimereset=%d\n",this,oldorientation,firsttimereset);

  if (!(hasImage && this->Initialized))
      return 0;

  if (this->SplineObjectmapEditor==NULL)
    this->AutoCreateMaskAndLookupTable();
  else
    this->SplineObjectmapEditor->SetImage(this->currentImage);

  if (this->InternalEditor!=NULL)
    this->InternalEditor->UpdateFromViewer();


  vtkpxGUIOrthogonalViewer::ResetViewer();

  if (!hasImage)
    {
      oldorientation=-1;
      firsttimereset=-1;
    }

  if (this->SplineObjectmapEditor!=NULL)
    maskImage->ShallowCopy(SplineObjectmapEditor->GetMaskImage());
      

  for (int i=0;i<=2;i++)
    {
      maskImageSlice[i]->SetInput(maskImage);
      maskImageSlice[i]->SetFrame(this->CurrentFrame);
      if (this->SplineObjectmapEditor!=NULL)
	maskImageSlice[i]->SetLookupTable(this->SplineObjectmapEditor->GetMaskLookupTable());
      else
	maskImageSlice[i]->SetLookupTable(this->maskLookupTable);

      if (this->Orientation==3)
	maskImageSlice[i]->SetPolarMode(1);
      else
	maskImageSlice[i]->SetPolarMode(0);

      maskImageSlice[i]->Update();
    }

  maskOrthoSlice->SetInput(maskImage);
  maskOrthoSlice->SetFrame(this->CurrentFrame);


  if (this->SplineObjectmapEditor!=NULL)
    maskOrthoSlice->SetLookupTable(this->SplineObjectmapEditor->GetMaskLookupTable());
  else
    maskOrthoSlice->SetLookupTable(this->maskLookupTable);


  if (this->Volume!=NULL)
    {
      this->UpdateMaskVolumeLookupTableAndImage();
    }


  if (this->Orientation==3)
    {
      maskOrthoSlice->SetPolarMode(1);
      }
  else
    {
      maskOrthoSlice->SetPolarMode(0);
    }


  this->UpdateDisplay();
  return 1;
}

void vtkpxGUIObjectmapOrthogonalViewer::SetSplineObjectmapEditor(vtkpxGUI2DSplineObjectmapViewer* editor)
{
  if ((!hasImage && this->Initialized) || editor==NULL)
    return;

  if (this->create_internal_editor==1)
    return;
  this->SplineObjectmapEditor=editor;

}
vtkpxGUI2DSplineObjectmapViewer* vtkpxGUIObjectmapOrthogonalViewer::GetSplineObjectmapEditor()
{
  return SplineObjectmapEditor;
}


 

float vtkpxGUIObjectmapOrthogonalViewer::GetObjectmapTransparency()
{
  if (  this->maskImageSlice[0]==NULL)
    return 0.0;

  return 100.0*this->maskImageSlice[0]->GetOpacity();
}

void vtkpxGUIObjectmapOrthogonalViewer::SetObjectmapTransparency(float v)
{
  if (  this->maskImageSlice[0]!=NULL)
    {
       v*=0.01;
       for (int ia=0;ia<=2;ia++)
	 maskImageSlice[ia]->SetOpacity(v);
       maskOrthoSlice->SetOpacity(v);
       this->UpdateMaskVolumeLookupTableAndImage();
       this->UpdateDisplay();
       this->maskTransparency->setValue(v*100.0,PXFalse);
    }

}

void vtkpxGUIObjectmapOrthogonalViewer::UpdateObjectmap()
{
  if (this->maskImageSlice[0]==NULL)
    return;
  
  if (this->SplineObjectmapEditor!=NULL)
    maskImage->ShallowCopy(SplineObjectmapEditor->GetMaskImage());

  //  fprintf(stderr,"In Updating Objectmap\n");

  for (int ia=0;ia<=2;ia++)
      maskImageSlice[ia]->SetInput(maskImage);
      
  maskOrthoSlice->SetInput(maskImage);

  if (this->InternalEditor!=NULL)
    this->InternalEditor->UpdateFromViewer();

  this->QuickUpdateObjectmap();
  this->UpdateIntensityLabelInfo();
  this->UpdateDisplay();
}


void vtkpxGUIObjectmapOrthogonalViewer::QuickUpdateObjectmap()
{
 if (this->maskImageSlice[0]==NULL)
    return;
  
   for (int ia=0;ia<=2;ia++)
    {
      maskImageSlice[ia]->Update();
      maskImageSlice[ia]->UpdateTexture();
    }
      
  maskOrthoSlice->Update();
  maskOrthoSlice->UpdateTexture();
 
}


int vtkpxGUIObjectmapOrthogonalViewer::SetObjectMapImage(vtkImageData* img)
{
  //  fprintf(stderr,"In Set Objectmap Image\n");

  if ((!hasImage && this->Initialized))
    return 0;
  


  if (this->SplineObjectmapEditor!=NULL)
    {
      this->SplineObjectmapEditor->SetMaskImage(img,this->Orientation);
      return 0;
    }

  //  fprintf(stderr,"Setting Objectmap\n");

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
  
  //  fprintf(stderr,"Updating Objectmap part II");

  this->maskImage->ShallowCopy(img);
  double r[2]; img->GetPointData()->GetScalars()->GetRange(r);

  vtkpxSurfaceUtil::DefaultObjectMapLookupTable(maskLookupTable,int(r[1]+2),1);
  UpdateObjectmap();
  return 1;
}



int vtkpxGUIObjectmapOrthogonalViewer::HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev)
{
  int status=vtkpxGUIOrthogonalViewer::HandleMouseButtonEvent(nbutton,state,x,y,ev);
  if (status==0)
    return status;

  if (nbutton!=1 && nbutton!=4 && nbutton!=7)
    return status;


  if (this->InternalEditor==NULL)
    return status;

  double lv[3];
  this->GetLastClickedPoint(lv);
  int x1=int(lv[0]);
  int y1=int(lv[1]);
  int z1=int(lv[2]);
  this->InternalEditor->SetPaint2DAxis(this->LastClickedSlice);
  this->InternalEditor->HandleClickedPoint(x1,y1,z1,nbutton,state);
  return status;
}

std::string vtkpxGUIObjectmapOrthogonalViewer::GetObjectmapValueAsText(int a,int b,int c)
{
  std::string s=" ";
  if (this->maskImage!=NULL)
    {
      int dim[3]; this->maskImage->GetDimensions(dim);
      if (a>=0 && a<dim[0] &&
	  b>=0 && b<dim[1] &&
	  c>=0 && c<dim[2])
	{
	  double vl=this->maskImage->GetScalarComponentAsDouble(a,b,c,0);
	  s=this->GetFormatedNumber(vl,3);
	}
    }
  return s;
}

int vtkpxGUIObjectmapOrthogonalViewer::PaintingEnabled()
{
  if (this->InternalEditor==NULL)
    return 0;
  return (this->InternalEditor->GetPaintMode()>0);
}


void vtkpxGUIObjectmapOrthogonalViewer::SetPresetLookupTable(int mode)
{
  //  fprintf(stderr,"In Setting preset lookup table\n");

  if (!hasImage)
    return;

  vtkpxGUIOrthogonalViewer::SetPresetLookupTable(mode);

  if (mode<1 || mode>6)
    return;

  if (this->InternalEditor==NULL)
    return;

  int minv=56,maxv=63;

  if (mode==2 || mode==3)
    {
      minv=192;
      maxv=255;
    }
  else if ( mode==5 || mode == 6)
    {
      minv=238;
      maxv=255;
    }
  
  //  fprintf(stderr,"Setting Thresholds %f:%f\n",double(minv),double(maxv));

  double r[2]; this->maskImage->GetPointData()->GetScalars()->GetRange(r);
  vtkpxSurfaceUtil::VOIObjectMapLookupTable(maskLookupTable,-1);
  UpdateObjectmap();
  this->InternalEditor->CreateDefaultLookupTable(1);
  maskTransparency->setValue(100.0,PXTrue);
  this->InternalEditor->SetVOISelectMode(double(minv),double(maxv),0);
}

