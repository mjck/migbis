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





#include "vtkpxGUIBaseImageViewer.h"
#include "vtkObjectFactory.h"
#include "vtkpxUtil.h"
#include "vtkImageData.h"
#include "vtkmpjX3DExporter.h"
#include <vtkVRMLExporter.h>
#include <vtkTIFFWriter.h> 
#include <vtkPNGWriter.h> 
#include <vtkJPEGWriter.h> 
#include "vtkWindowToImageFilter.h"
#include "vtkpxTalairachTransform.h"
#include "vtkpxBaseCurve.h"
#include "vtkLookupTable.h"
#include "vtkpxColorMapUtil.h"
#include "vtkpxImageComponentsToFrames.h"
#include "vtkImageConstantPad.h"
#include "vtkPolyData.h"
#include "vtkRenderer.h"
#include "vtkScalarBarActor.h"
#include "pxtkapp.h"
#include <vtkstd/string>
#include "vtkRendererCollection.h"
#include "vtkRenderer.h"
#include "vtkbisImageHeader.h"
#include "vtkstd/string"
#include "sstream"

// Construct object with no children.
vtkpxGUIBaseImageViewer::vtkpxGUIBaseImageViewer()
{
  this->hasImage=PXFalse;
  this->currentImage=NULL;
  this->valueImage=NULL;

  this->LastClickedPoint[0]=0.0;
  this->LastClickedPoint[1]=0.0;
  this->LastClickedPoint[2]=0.0;

  this->LastClickedPointScaled[0]=0.0;
  this->LastClickedPointScaled[1]=0.0;
  this->LastClickedPointScaled[2]=0.0;

  this->Orientation=0;
  this->NumberOfFrames=1;
  this->ColorMode=1;
  this->CurrentImageIsColor=0;

  this->CurrentFrame=0;

  this->talairachTransform=vtkpxTalairachTransform::New();
  this->mainLookupTable=vtkLookupTable::New();
  vtkpxColorMapUtil::SetStepColorMap(mainLookupTable,0,255,0,255,256,PXFalse);

  this->PresetLookupTableMode=-1;

  this->frameChangeCallbackName=NULL;
  this->EnableFrameChangeCallback=1;
  this->KeyRenderer=vtkRenderer::New();
  this->ScalarBar=vtkScalarBarActor::New();
  this->ScalarBar->VisibilityOff();
  this->KeyRenderer->AddActor2D(this->ScalarBar);


  this->ShowKeyRenderer=0;
  this->MagnificationFactor=1;

  this->coordinatesChangeCallbackName=NULL;
  this->EnableCoordinatesChangeCallback=1;

  this->EnableRendering=1;
  this->ColormapEditor=NULL;
  this->ownsLookupTable=1;
  this->createTalairachButtons=PXFalse;

  this->IntensityScale=1.0;
  this->IntensityOffset=0.0;

}

vtkpxGUIBaseImageViewer::~vtkpxGUIBaseImageViewer()
{
  if (this->currentImage)
      currentImage->Delete();

  if (this->talairachTransform)
    this->talairachTransform->Delete();

  if (mainLookupTable!=NULL &&  this->ownsLookupTable>0)
    mainLookupTable->Delete();
  
  if (this->frameChangeCallbackName!=NULL)
    delete [] this->frameChangeCallbackName;

  if (this->coordinatesChangeCallbackName!=NULL)
    delete [] this->coordinatesChangeCallbackName;


  if (this->ColormapEditor!=NULL)
    this->ColormapEditor->Delete();

  if (this->KeyRenderer!=NULL)
    this->KeyRenderer->Delete();

  if (this->valueImage!=NULL)
    this->valueImage->Delete();
}
/* -------------------------------------------------------------------------*/
char* vtkpxGUIBaseImageViewer::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();

  vtkpxGUIComponent::Initialize(name,inside);

  return GetWidgetName();

}

// -------------------------------------------------------------------------
void vtkpxGUIBaseImageViewer::SetColorMode(int cm)
{
  if (this->Initialized==0)
    {
      if (cm>0)
	this->ColorMode=1;
      else
	this->ColorMode=0;
    }
}

int vtkpxGUIBaseImageViewer::GetCurrentFrame()
{
  return this->CurrentFrame;
}

void vtkpxGUIBaseImageViewer::SetCurrentFrame(int t)
{
  if (!this->CurrentImageIsColor)
    this->CurrentFrame=t;
}

void vtkpxGUIBaseImageViewer::SetFrameChangeCallback(const char* name)
{
  if (name==NULL)
    return;

  if (this->frameChangeCallbackName!=NULL)
    delete [] this->frameChangeCallbackName;

  this->frameChangeCallbackName=new char[strlen(name)+1];
  strcpy(this->frameChangeCallbackName,name);
}

void  vtkpxGUIBaseImageViewer::ExecuteFrameChangeCallback()
{
  if (this->EnableFrameChangeCallback==0 || this->frameChangeCallbackName==NULL)
    return;

  PXTkApp::executeTclCommand(this->frameChangeCallbackName);

}
// -------------------------------------------------------------------------
void vtkpxGUIBaseImageViewer::SetCoordinatesChangeCallback(const char* name)
{
  if (name==NULL)
    return;

  if (this->coordinatesChangeCallbackName!=NULL)
    delete [] this->coordinatesChangeCallbackName;

  this->coordinatesChangeCallbackName=new char[strlen(name)+1];
  strcpy(this->coordinatesChangeCallbackName,name);
  //  fprintf(stderr,"New Coordinate change = %s\n",this->coordinatesChangeCallbackName);
}

void  vtkpxGUIBaseImageViewer::ExecuteCoordinatesChangeCallback()
{
  if (this->EnableCoordinatesChangeCallback==0 || this->coordinatesChangeCallbackName==NULL)
    return;

  PXTkApp::executeTclCommand(this->coordinatesChangeCallbackName);

}

void vtkpxGUIBaseImageViewer::ToggleKeyRenderer()
{
  this->SetShowKeyRenderer(this->ShowKeyRenderer<1);
  this->UpdateKeyRenderer();
}

// -------------------------------------------------------------------------
int vtkpxGUIBaseImageViewer::SetImage(vtkImageData* image,int orient,vtkLookupTable* cmap,double offset,double scale)
{
  if (image==NULL)
    return 0;
  
  this->Orientation=orient;

  if (this->currentImage==NULL)
    currentImage=vtkImageData::New();  
  
  int dim[3]; image->GetDimensions(dim);

  this->CurrentImageIsColor=0;
  if (this->ColorMode==0 || orient==3)
    {
      this->NumberOfFrames=image->GetNumberOfScalarComponents();
    }
  else  
    {
      int nc=image->GetNumberOfScalarComponents();
      if (nc==3 && image->GetScalarType()==VTK_UNSIGNED_CHAR)
	{
	  this->NumberOfFrames=1;
	  this->CurrentImageIsColor=1;
	}
      else
	this->NumberOfFrames=image->GetNumberOfScalarComponents();		  
    }
    
  //  fprintf(stderr,"Orientation=%d, colormode=%d iscolor=%d\n",this->Orientation,this->ColorMode,this->CurrentImageIsColor);
  
  currentImage->ShallowCopy(image);
  this->IntensityScale=scale;
  this->IntensityOffset=offset;

  if (this->valueImage!=NULL)
    this->valueImage->Delete();
  this->valueImage=NULL;
  
  if (this->CurrentFrame>=this->NumberOfFrames)
    this->CurrentFrame=0;
  
  if (hasImage==PXFalse)
    {
      hasImage=PXTrue;
      InitializeViewer();
    }

  ResetViewer();

  if (cmap==NULL)
    this->ResetColormap();
  else
    this->SetLookupTable(cmap);


  this->ResetEditor();
  this->UpdateIntensityLabelInfo();
  return TCL_OK;
}
// -------------------------------------------------------------------------
vtkImageData* vtkpxGUIBaseImageViewer::GetImage()
{
  return currentImage;
}

vtkImageData* vtkpxGUIBaseImageViewer::GetValueImage()
{
  return this->valueImage;
}
// -------------------------------------------------------------------------
int vtkpxGUIBaseImageViewer::DoImagesHaveSameDimensions(vtkImageData* img1,vtkImageData* img2,int checknumframes)
{
  if (img1==NULL || img2==NULL)
    return 0;

  int ndim[3]; img1->GetDimensions(ndim);
  int odim[3]; img2->GetDimensions(odim);
  int nnc=img1->GetNumberOfScalarComponents();
  int onc=img2->GetNumberOfScalarComponents();

  int sum=0;
  for (int i=0;i<=2;i++)
    sum+=abs(ndim[i]-odim[i]);
  if (checknumframes)
    sum+=abs(nnc-onc);

  if (sum>0)
    return 0;

  double nsp[3]; img1->GetSpacing(nsp);
  double osp[3]; img2->GetSpacing(osp);


  double fsum=0;
  for (int i=0;i<=2;i++)
    fsum+=fabs(nsp[i]-osp[i]);

  if (fsum>0.001)
    return 0;
  
  return 1;
}
// -------------------------------------------------------------------------
int vtkpxGUIBaseImageViewer::SetValueImage(vtkImageData* val)
{
  if (this->valueImage!=NULL)
    this->valueImage->Delete();
  this->valueImage=NULL;

  if (val==NULL || this->currentImage==NULL)
    return 0;

  if (this->DoImagesHaveSameDimensions(val,this->currentImage,1)==0)
    return 0;

  this->valueImage=vtkImageData::New();
  this->valueImage->ShallowCopy(val);
  this->UpdateIntensityLabelInfo();
  return 1;
}
// -------------------------------------------------------------------------
void vtkpxGUIBaseImageViewer::SaveAsTiff(vtkRenderWindow* renwin)
{
  this->SaveAsTiff(renwin,NULL);
}

void vtkpxGUIBaseImageViewer::SaveAsTiff(vtkRenderWindow* renwin,const char *fname)
{
  if (this->Initialized==0)
    return;

  if (fname==NULL)
    {
      int* size=renwin->GetSize();
      sprintf(pxtk_buffer,"Save Window %dx%d",size[0],size[1]);
      fname=this->EventManager->getSaveFilenameComplex(pxtk_buffer,"screen.png",
						       "{\"PNG Files\" {\"*.png\"}} {\"JPEG Files\" {\"*.jpg\"}} {\"TIFF File\" {\".tif\"}}");
    }
  if (strlen(fname)<1)
    return;

  char line[400];
  strncpy(line,fname,400);
  renwin->Render();
  PXTkApp::executeTclCommand("update idletasks");

  vtkWindowToImageFilter* renSrc=vtkWindowToImageFilter::New();
  renSrc->SetInput(renwin);
  renSrc->SetMagnification(this->MagnificationFactor);
  renSrc->Update();

  vtkImageWriter* writer=NULL;

  if (strcmp(PXTkApp::getFilenameExtension(line),".jpg")==0 ||
      (strcmp(PXTkApp::getFilenameExtension(line),".jpeg")==0) )
    {
      writer=vtkJPEGWriter::New();
    }
  else if (strcmp(PXTkApp::getFilenameExtension(line),".png")==0 )
    {
      writer=vtkPNGWriter::New();
    }
  else
    {
      writer=vtkTIFFWriter::New();
    }

  
  writer->SetInput(renSrc->GetOutput());

  writer->SetFileName(line);
  writer->Write();
  writer->Delete();
  renSrc->Delete();

  return;

}
/* -------------------------------------------------------------------------*/
//  Save All as Tiff
/* -------------------------------------------------------------------------*/

void vtkpxGUIBaseImageViewer::SaveAllAsTiff(vtkRenderWindow* renwin)
{
  this->SaveAllAsTiff(renwin,NULL);
}
void vtkpxGUIBaseImageViewer::SaveAllAsTiff(vtkRenderWindow* renwin,const char *fname)
{
  if (this->Initialized==0)
    return;

  int* size=renwin->GetSize();
  sprintf(pxtk_buffer,"Save Window (All Frames) %dx%d",size[0],size[1]);
  
  if (fname==NULL)
    fname=this->EventManager->getSaveFilenameComplex(pxtk_buffer,"screen.jpg",
						     "{\"JPEG Files\" {\"*.jp*g\"}} {\"TIFF File\" {\"*.tif*\"}}");
  if (strlen(fname)<1)
    return;

  strncpy(pxtk_buffer,fname,400);
  PXTkApp::executeTclCommand("update idletasks");
  
  strncpy(pxtk_buffer2,PXTkApp::getFilenameRoot(pxtk_buffer),400);

  int jpeg_mode=0;

  char extension[5];
  strncpy(extension,PXTkApp::getFilenameExtension(pxtk_buffer),5);
  if (strlen(extension)<2)
    strcpy(extension,".jpg");
  
  int oldframe=this->GetCurrentFrame();

  vtkWindowToImageFilter* renSrc=vtkWindowToImageFilter::New();
  renSrc->SetInput(renwin);

  
  vtkImageWriter* writer=NULL;
  if (strcmp(extension,".jpg")==0 ||
      (strcmp(extension,".jpeg")==0) )
    {
      writer=vtkJPEGWriter::New();
      jpeg_mode=1;
    }
  else
    writer=vtkTIFFWriter::New();

  
  for (int frame=0;frame<this->NumberOfFrames;frame++)
    {
      sprintf(pxtk_buffer3,"%s.%03d%s",pxtk_buffer2,frame+1,extension);
      sprintf(pxtk_buffer4,"Saving Output in %s\n",pxtk_buffer3);
      PXTkApp::printToConsole(pxtk_buffer4);
      this->SetCurrentFrame(frame);

      renwin->Render();
      PXTkApp::executeTclCommand("update idletasks");


      renSrc->Modified();
      renSrc->SetMagnification(this->MagnificationFactor);
      renSrc->Update();
      int dim[3];
      renSrc->GetOutput()->GetDimensions(dim);

      writer->SetInput(renSrc->GetOutput());
      writer->SetFileName(pxtk_buffer3);
      writer->Write();
    }

  writer->Delete();
  renSrc->Delete();
  this->SetCurrentFrame(oldframe);

}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseImageViewer::ExportAsX3D(vtkRenderWindow* renwin, vtkRenderer *renderer)
{
  if (this->Initialized==0)
    return;

  char *line=this->EventManager->getSaveFilenameComplex("Export 3D Window to X3D","screen.x3d","{\"X3D Files\" {\".x3d\"}} ");
  if (line==NULL)
    return;
  if (strlen(line)<1)
    return;
    
  renwin->Render();
  vtkmpjX3DExporter* exp=vtkmpjX3DExporter::New();
  exp->SetRenderWindow(renwin);
  exp->SetRenderer(renderer);
  exp->SetFileName(line);
  exp->Update();
  exp->Delete();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseImageViewer::ExportAsVRML(vtkRenderWindow* renwin)
{
  if (this->Initialized==0)
    return;

  char *line=this->EventManager->getSaveFilenameComplex("Export Window to VRML","screen.wrl","{\"VRML Files\" {\".wrl\"}} ");
  if (line==NULL)
    return;
  if (strlen(line)<1)
    return;
    
  renwin->Render();
  vtkVRMLExporter* exp=vtkVRMLExporter::New();
  exp->SetRenderWindow(renwin);
  exp->SetFileName(line);
  exp->Update();
  exp->Delete();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseImageViewer::SetTalairachPoints(vtkPoints* pt)
{
  if (pt==NULL)
    return;

  if (this->talairachTransform==NULL)
    this->talairachTransform=vtkpxTalairachTransform::New();

  this->talairachTransform->SetLandmarks(pt);
  this->talairachTransform->Update();
  this->UpdateTalairachGrid(1);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseImageViewer::LinkLookupTable(vtkLookupTable* lkp)
{
  if (lkp==NULL)
    return;

  if (hasImage)
    return;
  
  if (mainLookupTable!=NULL &&  this->ownsLookupTable>0)
    mainLookupTable->Delete();

  mainLookupTable=lkp;
  this->ownsLookupTable=0;
  //  fprintf(stderr,"Lookup Tables Linked\n");
}

void vtkpxGUIBaseImageViewer::SetLookupTable(vtkLookupTable* lkp)
{
  if (!this->Initialized || !hasImage || lkp==NULL)
      return;

  if (this->ownsLookupTable==0 || this->mainLookupTable==NULL)
    this->mainLookupTable=vtkLookupTable::New();
  this->ownsLookupTable=1;

  this->mainLookupTable->DeepCopy(lkp);

  if (this->ColormapEditor!=NULL)
    {
      this->ColormapEditor->SetColormap(this->mainLookupTable);
      this->ColormapEditor->Update();
    }



}

/* -------------------------------------------------------------------------*/ 

int vtkpxGUIBaseImageViewer::ResetColormap()
{
  if (this->ownsLookupTable==0 || this->ColormapEditor==NULL)
    {
      return 0;
    }
 
  this->ColormapEditor->SetLock(0);
  this->ColormapEditor->SetColormap(this->mainLookupTable);
  this->ColormapEditor->SetRange(currentImage);
  this->SetPresetLookupTable(this->PresetLookupTableMode);
  this->ColormapEditor->Update();

      
  return 1;
}
// -------------------------------------------------------------------------
void vtkpxGUIBaseImageViewer::SetFmriMergeLookupTable()
{
  if (!this->Initialized || !hasImage)
      return;

  vtkpxColorMapUtil::SetFmriMergeColormap(this->mainLookupTable,0);
}

void vtkpxGUIBaseImageViewer::SetFmriMergeLookupTable(int mode)
{
  if (!this->Initialized || !hasImage)
      return;

  vtkpxColorMapUtil::SetFmriMergeColormap(this->mainLookupTable,mode);
}

void vtkpxGUIBaseImageViewer::SetPresetLookupTable(int mode)
{
  if (!hasImage)
    return;

  //  fprintf(stderr,"Going to Preset Lookup Table\n");

  this->SetPresetLookupTableMode(mode);
  if (this->ColormapEditor!=NULL)
    this->ColormapEditor->SetFmriMode(this->PresetLookupTableMode);

  //  fprintf(stderr,"Done with Preset Lookup Table\n");

  return;

}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseImageViewer::LoadTalairachLandmarks(const char* fname)
{
  int dodialog=1;
  if (fname!=NULL)
    {
      if (strlen(fname)>0)
	{
	  strcpy(pxtk_buffer,fname);
	  dodialog=0;
	}
    }

  if (dodialog>0)
    {
      char* line=this->EventManager->getOpenFilename("Load Talairach Landmarks","Landmarks","*.land");
      if (line==NULL)
	return;
      if (strlen(line)<1)
	return;
      strcpy(pxtk_buffer,line);
    }

  vtkpxBaseCurve* land=vtkpxBaseCurve::New();
  int ok=land->Load(pxtk_buffer);

  if (ok==0)
    {
      land->Delete();
      vtkErrorMacro(<<"Failed to Load Landmarks (Talairach) from "<<pxtk_buffer);
      return;
    }

  land->Compact();
  this->SetTalairachPoints(land->GetPoints());
  land->Delete();
  return;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseImageViewer::LoadTalairachTransform(const char* fname)
{
  int dodialog=1;

  if (fname!=NULL)
    {
      if (strlen(fname)>0)
	{
	  strcpy(pxtk_buffer,fname);
	  dodialog=0;
	}
    }

  if (dodialog==1)
    {
      char* line=this->EventManager->getOpenFilename("Load Talairach Transform","T-Transform","*.tal");
      if (line==NULL)
	return 0;
      if (strlen(line)<1)
	return 0;
      strcpy(pxtk_buffer,line);
    }

  if (this->talairachTransform==NULL)
    this->talairachTransform=vtkpxTalairachTransform::New();
  int ok=this->talairachTransform->Load(pxtk_buffer);
  if (ok==0)
    {
      vtkErrorMacro(<<"Failed to Load Talairach Transform from "<<pxtk_buffer);
      return 0;
    }
  
  this->UpdateTalairachGrid(1);
  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseImageViewer::SaveTalairachTransform(const char* fname)
{

  if (this->talairachTransform==NULL)
    {
      vtkErrorMacro(<<"No Talairach Transform to Save!");
      return 0;
    }

  int dodialog=1;
  if (fname!=NULL)
    {
      if (strlen(fname)>1)
	{
	  strcpy(pxtk_buffer,fname);
	  dodialog=0;
	}
    }

  if (dodialog>0)
    {
      char* line=this->EventManager->getSaveFilename("Save Talairach Transform","transform.tal","T-Transform","*.tal");
      if (line==NULL)
	return 0;
      if (strlen(line)<1)
	return 0;
      strcpy(pxtk_buffer,line);
    }


  int ok=this->talairachTransform->Save(pxtk_buffer);
  if (ok==0)
    {
      vtkErrorMacro(<<"Failed to Save Talairach Transform from "<<pxtk_buffer);
      return 0;
    }

  return 1;
}

void vtkpxGUIBaseImageViewer::ShowFrameControls(int show)
{
  vtkDebugMacro(<<"Function has not been defined\n");
}

void vtkpxGUIBaseImageViewer::UpdateTalairachGrid(int )
{
  return;
}

void vtkpxGUIBaseImageViewer::RenderDelay(int t)
{
  Tcl_CreateTimerHandler(t,vtkpxGUIBaseImageViewer::handleRenderEvent,this);
}

void vtkpxGUIBaseImageViewer::handleRenderEvent(void *arg1)
{
  //fprintf(stderr,"On to delayed expose\n");
  ((vtkpxGUIBaseImageViewer*)arg1)->UpdateDisplay();
}

// ----------------------------------------------------------------------------------------------------------------------

const  char* vtkpxGUIBaseImageViewer::GetFormatedNumber(double vl,int dp)
{
  std::stringstream s;
  
  if (vl<10.0)
    s.precision(dp);
  s << vl;
  this->Buffer=s.str();
  return this->Buffer.c_str();
}

// ----------------------------------------------------------------------------------------------------------------------

int vtkpxGUIBaseImageViewer::DisplayNeuroCoordinates(double pos[3],int coord[3],PXTkLabel* talairachlabel,PXTkLabel* mnilabel,vtkImageData* img)
{
  vtkImageData* usethisimage=img;
  int usingvalueimage=1;
  if (img==NULL)
    {
      if (this->valueImage!=NULL)
	{
	  usethisimage=this->valueImage;
	}
      else
	{
	  usethisimage=this->currentImage;
	  usingvalueimage=0;
	}
    }

  if (this->talairachTransform==NULL || usethisimage==NULL)
    return 0;

  int ok=0;

  int oldstyleorient=vtkpxGUIBaseImageViewer::MapOrientationToOldStyle(this->Orientation);

  if (oldstyleorient==1 || oldstyleorient==2)
    {
      double p1[3];
      for (int ia=0;ia<=2;ia++) p1[ia]=pos[ia];
      ok=this->talairachTransform->ReorientCoordinates(usethisimage,oldstyleorient,p1,pos);
    }

  std::string objlabel=this->GetObjectmapValueAsText(coord[0],coord[1],coord[2]);
  
  if (oldstyleorient==0 || ok == 1)
    {
      double p[3];
      this->talairachTransform->TransformPoint(pos,p);

      
      double vl[3];
      if (!this->CurrentImageIsColor)
	{
	  vl[0]=usethisimage->GetScalarComponentAsDouble(coord[0],coord[1],coord[2],this->CurrentFrame);
	  if (usingvalueimage==0)
	    vl[0]=vl[0]*this->IntensityScale+IntensityOffset;
	}
      else 
	{
	  for (int ia=0;ia<=2;ia++)
	    vl[ia]=this->currentImage->GetScalarComponentAsDouble(coord[0],coord[1],coord[2],ia);
	}
     
      double mni[3];
      int domni=this->talairachTransform->GetMNICoordinates(pos,mni);
      
      char name[100];
      char name2[100];
      strcpy(name,"Custom TC");
      strcpy(name2,"");
      
      int md=this->talairachTransform->GetColinMode();
      int dostandard=this->talairachTransform->GetStandardMode();
      int sz=this->talairachTransform->GetColinYSize();
      
      if (md==1 || md==-1)
	{
	  if (sz==216)
	    strcpy(name,"C");
	  else
	    strcpy(name,"YC");
	  
	  if (md==1)
	    strcpy(name2,"_AP TC=");
	  else
	    strcpy(name2,"_PA TC=");
	  if (talairachlabel!=NULL)
	    talairachlabel->configure("-fg","red");
	}
      
      if (dostandard==1)
	{
	  mnilabel->setText("");
	  talairachlabel->configure("-fg","white");
	  if (!this->CurrentImageIsColor)
	    sprintf(pxtk_buffer,"(%d %d %d) %s %s",coord[0],coord[1],coord[2],this->GetFormatedNumber(vl[0],3),objlabel.c_str());
	  else
	    sprintf(pxtk_buffer,"(%d %d %d) (%.0f,%.0f,%.0f) %s",coord[0],coord[1],coord[2],
		    vl[0],vl[1],vl[2],objlabel.c_str());
	  talairachlabel->setText(pxtk_buffer);
	}
      else if (domni==1)
	{
	  sprintf(pxtk_buffer,"MNI=(%.0f,%.0f,%.0f)",mni[0],mni[1],mni[2]);
	  mnilabel->setText(pxtk_buffer);
	  if (!this->CurrentImageIsColor)
	    sprintf(pxtk_buffer,"%s%s(%.0f %.0f %.0f) %s %s",name,name2,p[0],p[1],p[2],this->GetFormatedNumber(vl[0],3),objlabel.c_str());
	  else
	    sprintf(pxtk_buffer,"%s%s(%.0f %.0f %.0f) (%.0f,%.0f,%.0f) %s",name,name2,p[0],p[1],p[2],
		    vl[0],vl[1],vl[2],objlabel.c_str());
	  talairachlabel->setText(pxtk_buffer);
	  talairachlabel->configure("-fg","green");
	}
      else
	{	
	  sprintf(pxtk_buffer,"%s%s(%.0f %.0f %.0f)",name,name2,p[0],p[1],p[2]);
	  talairachlabel->setText(pxtk_buffer);
	  talairachlabel->configure("-fg","yellow");
	  if (!this->CurrentImageIsColor)
	    sprintf(pxtk_buffer,"Int=%s %s",this->GetFormatedNumber(vl[0],3),objlabel.c_str());
	  else
	    sprintf(pxtk_buffer,"Int=(%.0f,%.0f,%.0f) %s",vl[0],vl[1],vl[2],objlabel.c_str());
	  mnilabel->setText(pxtk_buffer);
	}
      return 2;
    }

  mnilabel->setText("Not Axial");

  if (!this->CurrentImageIsColor)
    {
      double vl=usethisimage->GetScalarComponentAsDouble(coord[0],coord[1],coord[2],this->CurrentFrame);
      if (usingvalueimage==0)
	vl=vl*this->IntensityScale+IntensityOffset;
      sprintf(pxtk_buffer,"(%d %d %d) %s %s",coord[0],coord[1],coord[2],this->GetFormatedNumber(vl,3),objlabel.c_str());
    } 
  else 
    {
      double vl[3];
      for (int ia=0;ia<=2;ia++)
	vl[ia]=this->currentImage->GetScalarComponentAsDouble(coord[0],coord[1],coord[2],ia);
      sprintf(pxtk_buffer,"(%d %.d %d) (%.0f,%.0f,%.0f) %s",coord[0],coord[1],coord[2],vl[0],vl[1],vl[2],objlabel.c_str());
    }

  talairachlabel->setText(pxtk_buffer);
  talairachlabel->configure("-fg","white");
  return 1;
}

void vtkpxGUIBaseImageViewer::DisableTalairachButtons()
{
  if (this->Initialized==1)
    return;

  this->createTalairachButtons=PXFalse;
}

void vtkpxGUIBaseImageViewer::EnableTalairachButtons()
{
  if (this->Initialized==1)
    return;

  this->createTalairachButtons=PXTrue;
}

double vtkpxGUIBaseImageViewer::GetMNISliceNo(int index,int plane,int& axis)
{
  axis=-1;
  if (plane<0)
    plane=0;
  else if (plane>2)
    plane=2;

  if (this->talairachTransform==NULL || this->currentImage==NULL)
      return -1000;

  int oldstyleorient=vtkpxGUIBaseImageViewer::MapOrientationToOldStyle(this->Orientation);

  double sp[3];    currentImage->GetSpacing(sp);
  double ori[3];   currentImage->GetOrigin(ori);
  int    dim[3];   currentImage->GetDimensions(dim);
  double pos[3];
  int level[3];
  for (int ia=0;ia<=2;ia++)
    {
      if (ia==2)
	level[ia]=index;
      else
	level[ia]=int(0.5*double(dim[ia])*sp[ia]+ori[ia]);
      level[ia]=Irange(level[ia],0,dim[ia]-1);
      pos[ia]=double(level[ia])*sp[ia]+ori[ia];
    }
  
  if (oldstyleorient==1 || oldstyleorient==2)
    {
      double p1[3];
      for (int ia=0;ia<=2;ia++) p1[ia]=pos[ia];
      int ok=this->talairachTransform->ReorientCoordinates(this->currentImage,oldstyleorient,p1,pos);
      if (ok==0)
	return -1000;
    }
  

  double mni[3];
  int domni=this->talairachTransform->GetMNICoordinates(pos,mni);
  if (domni==0)
    return -1000;

  if (oldstyleorient==0)
    {
      axis=plane;
    }
  else if (oldstyleorient==1)
    {
      switch (plane) {
	case 0:	  axis=0;	  break;
	case 1:	  axis=2;	  break;
	case 2:	  axis=1;	  break;
      }
    }
  else if (oldstyleorient==2)
    {
      switch (plane) {
	case 0:	  axis=1;	  break;
	case 1:	  axis=2;	  break;
	case 2:	  axis=0;	  break;
      }
    }
  
  return mni[axis];
}

std::string vtkpxGUIBaseImageViewer::GetObjectmapValueAsText(int ,int,int ) 
{
  std::string s=" ";
  return s;
}


void vtkpxGUIBaseImageViewer::SetBackgroundColor(vtkRenderWindow* renwin)
{
  vtkRendererCollection* col=renwin->GetRenderers();
  int numren=col->GetNumberOfItems();

  vtkRenderer* ren=col->GetFirstRenderer();

  double c[3];   ren->GetBackground(c);
  int a=this->EventManager->d_colorBox("Background Color",c[0],c[1],c[2]);

  if (a==0)
    return;

  for (int i=0;i<numren;i++)
    {
      vtkRenderer* r=(vtkRenderer*)col->GetItemAsObject(i);
      r->SetBackground(c);
    }
  renwin->Render();
}

int vtkpxGUIBaseImageViewer::MapOrientationToNewStyle(int oldorient)
{
  return vtkbisImageHeader::MapOrientationToNewStyle(oldorient);
}

int vtkpxGUIBaseImageViewer::MapOrientationToOldStyle(int neworient)
{
  return vtkbisImageHeader::MapOrientationToOldStyle(neworient);
}

const char* vtkpxGUIBaseImageViewer::OrientationToAxisName(int neworient,int axis)
{
  this->Buffer=vtkbisImageHeader::OrientationToAxisName(neworient,axis);
  return this->Buffer.c_str();
}

