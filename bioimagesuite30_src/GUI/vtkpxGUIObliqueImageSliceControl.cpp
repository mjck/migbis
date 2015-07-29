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
  Module:    $RCSfile: vtkpxGUIObliqueImageSliceControl.cpp,v $
  Language:  C++
  Date:      $Date: 2002/05/08 16:04:58 $
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
#include "vtkpxGUIObliqueImageSliceControl.h"
#include "vtkObjectFactory.h"
#include "vtkCamera.h"
#include "vtkPlaneSource.h"
#include "vtkpxAnalyzeImageWriter.h"
#include "vtkpxTransformationUtil.h"


vtkpxGUIObliqueImageSliceControl* vtkpxGUIObliqueImageSliceControl::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIObliqueImageSliceControl");
  if(ret)
      {
	return (vtkpxGUIObliqueImageSliceControl*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIObliqueImageSliceControl;
}

// Construct object with no children.
vtkpxGUIObliqueImageSliceControl::vtkpxGUIObliqueImageSliceControl()
{
  has_obliqueslice=PXFalse;
  this->ObliqueImageSlice=NULL;
  this->Viewer=NULL;
  this->internal_viewer=NULL;
  this->show_obliqueslice=NULL;
  this->Visibility=PXFalse;
  this->NormalVisibility=PXFalse;
  this->Initialized=0;
  this->FollowCamera=PXFalse;
}
// ------------------------------------------------------------------------------------
void vtkpxGUIObliqueImageSliceControl::SetObliqueImageSliceRenderer(vtkpxObliqueImageSlice* vol,vtkpxGUIBaseRenderer* view)
{
  if (vol!=NULL)
    {
      this->ObliqueImageSlice=vol;
      this->Viewer=view;
      has_obliqueslice=PXTrue;
      this->SetResolution(resliceMode->getIndex());
      //      fprintf(stderr,"Setting Oblique Image Slice Renderer\n");
      this->Update(1);
    }
}
// ------------------------------------------------------------------------------------
void  vtkpxGUIObliqueImageSliceControl::UpdateObliqueImageSliceRenderer(vtkpxObliqueImageSlice* vol,vtkpxGUIBaseRenderer* ren)
{
  if (vol==NULL || ren==NULL)
    return;

  if (has_obliqueslice==PXFalse)
    {
      //      fprintf(stderr,"Calling Set Oblique Image Slice Renderer\n");
      this->SetObliqueImageSliceRenderer(vol,ren);
      return;
    }

  float d1[6]; this->ObliqueImageSlice->GetImageRange(d1);
  float d2[6]; vol->GetImageRange(d2);

  float d=0;
  for (int ia=1;ia<=5;ia+=2)
    d+=fabs(d1[ia]-d2[ia]);

  if (d>0)
    distanceScale->setRange(-d,d);

  this->ObliqueImageSlice=vol;
  this->Viewer=ren;
  this->SetResolution(resliceMode->getIndex());
  //this->Update(0);
}
// ------------------------------------------------------------------------------------
void vtkpxGUIObliqueImageSliceControl::SetRenderer(vtkpxGUIBaseRenderer* viewer1)
{
  this->Viewer=viewer1;
}
// ------------------------------------------------------------------------------------
char*  vtkpxGUIObliqueImageSliceControl::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
    return GetWidgetName();
  
  vtkpxGUIComponent::Initialize(name,inside);

  PXTkFrame* parentFrame=(PXTkFrame*)(this->EventManager->getMainWindow());
  viewerFrame=new PXTkFrame(this->EventManager,parentFrame);
  volFrame=new PXTkFrame(this->EventManager,parentFrame);

  parentFrame->setPackMode(PXTrue);
  parentFrame->addChildren("-side top -expand false -fill both",volFrame,viewerFrame);
  
  show_obliqueslice=new PXTkCheckButton(this->EventManager,volFrame,"Show",104);
  show_obliqueslice->setState(this->Visibility);

  show_normal=new PXTkCheckButton(this->EventManager,volFrame,"Normal",104);
  show_normal->setState(this->NormalVisibility);

  follow_camera=new PXTkCheckButton(this->EventManager,volFrame,"Follow Cam",105);
  follow_camera->setState(PXFalse);
  volFrame->addChildren("-side left -expand f -fill x",show_obliqueslice,show_normal,follow_camera);

  PXTkLabel* l1=new PXTkLabel(this->EventManager,volFrame,"Res:"); 
  resliceMode=new PXTkOptionMenu(this->EventManager,volFrame,"64x64",103);
  resliceMode->addOption("128x128");
  resliceMode->addOption("256x256");
  resliceMode->addOption("512x512");
  resliceMode->setIndex(2);
  
  volFrame->addChildren("-side left -expand f -fill x",l1,resliceMode);



  PXTkFrame* vFrame=new PXTkFrame(this->EventManager,parentFrame);
  PXTkFrame* vFrame2=new PXTkFrame(this->EventManager,parentFrame);
  parentFrame->addChildren("-side top -expand false -fill x",vFrame,vFrame2);

  distanceScale=new PXTkArrowScale(this->EventManager,vFrame2,"Distance",107,PXTrue);
  distanceScale->setRange(-400,400);
  distanceScale->setResolution(0.1);
  distanceScale->setIncrement(0.5);
  distanceScale->setDecimalPoints(1);
  distanceScale->setLengthWidth(160,10);
  vFrame2->addChildren("-side left -expand t -fill x",distanceScale->getMainWindow());
	
  opacityScale=new PXTkScale(this->EventManager,vFrame2,"Opacity",106);
  opacityScale->setResolution(0.05);
  opacityScale->setRange(0.5,1);
  opacityScale->setDigits(3);
  opacityScale->setLengthWidth(80,10);
  vFrame2->addChildren("-side right -expand f -fill x",opacityScale);
	

  for (int ia=0;ia<=2;ia++)
    {
      if (ia==0)
	strcpy(pxtk_buffer4,"x-");
      else if (ia==1)
	strcpy(pxtk_buffer4,"y-");
      else
	strcpy(pxtk_buffer4,"z-");
      
      sprintf(pxtk_buffer4,"%s-norm",pxtk_buffer4);
      clipScale[ia]=new PXTkScale(this->EventManager,vFrame,pxtk_buffer4,107);
      clipScale[ia]->setRange(-1.0,1.0);
      clipScale[ia]->setDigits(3);
      clipScale[ia]->setResolution(0.01);
      clipScale[ia]->setLengthWidth(80,10);
      vFrame->addChildren("-side left -expand t -fill x",clipScale[ia]);
    }

  
  PXTkFrame* bFrame=new PXTkFrame(this->EventManager,parentFrame);
  PXTkFrame* bFrame2=new PXTkFrame(this->EventManager,parentFrame);
  parentFrame->addChildren("-side bottom -expand false -fill x",bFrame2,bFrame);

  if (!this->EventManager->isInside())
    {
      PXTkButton* b5=new PXTkButton(this->EventManager,bFrame2,"Close",3); 
      bFrame2->addChildren("-side right -expand f -fill x -padx 5",b5);
    }

  PXTkLabel* lab=new PXTkLabel(this->EventManager,bFrame,"");
  lab->configure("-image","$::pxtclvtkpxcontrib::smalllogo ");
  bFrame->addChildren("-side right -expand f",lab);

  PXTkButton* b4=new PXTkButton(this->EventManager,bFrame,"Reset",139); 
  PXTkButton* b5=new PXTkButton(this->EventManager,bFrame,"Look to Camera",138); 
  PXTkButton* b6=new PXTkButton(this->EventManager,bFrame,"Edge Camera",137); 
  bFrame->addChildren("-side left -expand t -fill x -padx 2",b4,b5,b6);


  PXTkButton* b7=new PXTkButton(this->EventManager,bFrame2,"Load Plane",141); 
  PXTkButton* b8=new PXTkButton(this->EventManager,bFrame2,"Save Plane",142); 
  PXTkButton* b9=new PXTkButton(this->EventManager,bFrame2,"Save Image",143); 
  PXTkButton* b10=new PXTkButton(this->EventManager,bFrame2,"Save Xform",144); 
  bFrame2->addChildren("-side left -expand t -fill x -padx 5",b7,b8,b9,b10);
  
  this->Initialized=1;

  return GetWidgetName();
}
 // ------------------------------------------------------------------------------------
void vtkpxGUIObliqueImageSliceControl::Update(int full)
{
  if (!this->Initialized || !has_obliqueslice)
      return;

  //  fprintf(stderr,"\n\n ********** Updating Oblique full=%d\n",full);

  if (this->Visibility)	
    this->ObliqueImageSlice->SetVisibility(1);
  else
    this->ObliqueImageSlice->SetVisibility(0);

  if (this->NormalVisibility)
    this->ObliqueImageSlice->SetShowNormalArrow(1);
  else
    this->ObliqueImageSlice->SetShowNormalArrow(0);


  if (full==0)
    {
      this->ObliqueImageSlice->SetOpacity(opacityScale->getValue());
      this->UpdateObliqueSlicePositionAndOrientation();
      Viewer->Render();
    }
  else if (full ==1)
    {
      float range[6]; 
      this->ObliqueImageSlice->GetImageRange(range);
      float d=sqrt(pow(range[0]-range[1],float(2.0))+
		   pow(range[2]-range[3],float(2.0))+
		   pow(range[4]-range[5],float(2.0)));
      
      //      fprintf(stderr,"Full =%d calling Update Scales from setting distance scale\n",full);
      distanceScale->setRange(-d,d);
      UpdateScales();
    }

}
// ------------------------------------------------------------------------------------
void vtkpxGUIObliqueImageSliceControl::UpdateDisplay()
{
  if (!this->Initialized || !has_obliqueslice)
      return;

  if (!this->Visibility)
      return;
  
  this->UpdateObliqueSlicePositionAndOrientation();
}
// ------------------------------------------------------------------------------------
int vtkpxGUIObliqueImageSliceControl::UpdateObliqueSlicePositionAndOrientation()
{
  if (!this->Initialized || !has_obliqueslice)
      return 0;

  //  fprintf(stderr,"\nUpdating Slice Position and Orientation");
  float norm[3],sum=0.0;
  for (int ia=0;ia<=2;ia++)
    {
      norm[ia]=(clipScale[ia]->getValue());
      sum+=norm[ia]*norm[ia];
    }
  sum=sqrt(sum);
  for (int ia=0;ia<=2;ia++)
    norm[ia]/=sum;

  float dist=distanceScale->getValue();


  //  fprintf(stderr,"Setting Norm %.2f %.2f %.2f\n",norm[0],norm[1],norm[2]);
  this->ObliqueImageSlice->UpdateImagePlane(dist,norm[0],norm[1],norm[2]);

  //  fprintf(stderr,"Calling Update Scales from UpdateObliqueSlice \t");
  this->UpdateScales();


  return 1;
}
// ------------------------------------------------------------------------------------
void vtkpxGUIObliqueImageSliceControl::SetVisibility(int vis)
{
  vis=(vis>0);
  if (vis)
    this->Visibility=PXTrue;
  else
    this->Visibility=PXFalse;

  if (this->show_obliqueslice!=NULL)
    show_obliqueslice->setState(this->Visibility);
  
  if (this->ObliqueImageSlice!=NULL)
    {
      if (this->Visibility)
	this->ObliqueImageSlice->SetVisibility(1);
      else
	this->ObliqueImageSlice->SetVisibility(0);
    }
}
// ------------------------------------------------------------------------------------
int  vtkpxGUIObliqueImageSliceControl::HandleEvent(int event)
{
  if (event<=10)
      return this->EventManager->handleDirectEvent(event);

  if (!has_obliqueslice)    // Nothing to do 
      return TCL_OK;

  switch (event)
      {
      case 102:
	UpdateDisplay();
	break;
	
      case 103:
	this->SetResolution(resliceMode->getIndex());
	Viewer->Render();
	break;


      case 104:
	this->Visibility=show_obliqueslice->getState();
	this->NormalVisibility=show_normal->getState();

	if (this->Visibility)
	  this->ObliqueImageSlice->SetVisibility(1);
	else
	  this->ObliqueImageSlice->SetVisibility(0);

	if (this->NormalVisibility)
	  this->ObliqueImageSlice->SetShowNormalArrow(1);
	else
	  this->ObliqueImageSlice->SetShowNormalArrow(0);
	break;
	

      case 105:
	this->FollowCamera=follow_camera->getState();
	if (this->FollowCamera)
	  {
	    this->UpdateFromCamera();
	    Viewer->Render();
	  }
	break;
	

      case 106:
	if (has_obliqueslice)
	  {
	    float v=(opacityScale->getValue());
	    this->ObliqueImageSlice->SetOpacity(v);
	    Viewer->Render();
	  }
	break;

      case 107:
	UpdateObliqueSlicePositionAndOrientation();
	break;

      case 139:
	if (has_obliqueslice)
	  {
	    this->ObliqueImageSlice->Reset();
	    //	    fprintf(stderr,"Event =139 calling Update Scales\n");
	    this->UpdateScales();
	    Viewer->Render();
	  }
	break;

      case 138:
	this->UpdateFromCamera(1,0);
	break;

      case 137:
	this->UpdateFromCamera(1,1);
	break;

      case 140:
	this->UpdateFromCamera(0,0);
	break;

      case 141:
	this->Load();
	break;

      case 142:
	this->Save();
	break;

      case 143:
	this->SaveCurrentImage();
	break;

      case 144:
	this->SaveCurrentTransform();
	break;
	
      }

  Viewer->Render();
  return TCL_OK;
}
// ------------------------------------------------------------------------------------
vtkpxGUIBaseRenderer* vtkpxGUIObliqueImageSliceControl::GetViewer()
{
  return this->Viewer;
}

// ------------------------------------------------------------------------------------
void vtkpxGUIObliqueImageSliceControl::UpdateFromCamera(int force,int mode)
{
  if (this->Viewer==NULL || ( this->FollowCamera==PXFalse && force==0) || has_obliqueslice==PXFalse)
    return;

  vtkCamera* cam=this->Viewer->GetRenderer()->GetActiveCamera();
  this->ObliqueImageSlice->UpdateImagePlane(cam,mode);
  //  fprintf(stderr,"Calling UpdateScales from Update from Camera\n");
  this->UpdateScales();
}


void vtkpxGUIObliqueImageSliceControl::UpdateScales()
{
  if (this->Initialized==0 ||  has_obliqueslice==PXFalse)
    return;

  //  fprintf(stderr,"Updating Scales\n");
  vtkPlaneSource* plane=this->ObliqueImageSlice->GetImagePlane();
  if (plane==NULL)
    return;

  float  d=this->ObliqueImageSlice->GetDistance();
  double n[3]; this->ObliqueImageSlice->GetNormal(n);
  //  fprintf(stderr,"\t\t Getting Norm from plane %.2f %.2f %.2f\n",n[0],n[1],n[2]);

  for (int i=0;i<=2;i++)
    clipScale[i]->setValue(n[i]);
  distanceScale->setValue(d);

}

void vtkpxGUIObliqueImageSliceControl::SetResolution(int mode)
{
  int r=64;
  switch (mode)
    {
    case 1:
      r=128;
      break;
    case 2:
      r=256;
      break;
    case 3:
      r=512;
      break;
    }
  

  this->ObliqueImageSlice->SetResolution(r);
}

int vtkpxGUIObliqueImageSliceControl::Load()
{
  if (this->Initialized==0 ||  has_obliqueslice==PXFalse)
    return 0;

  char* line=this->EventManager->getOpenFilename("Load Oblique Slice Position","Oblique Slice","*.obl");
  if (line!=NULL)
    {
      if (strlen(line)>0)
	{
	  gzFile fin=gzsuffixopen(line,"rb");
	  if (fin)
	    {
	      gzgets(fin,pxtk_buffer,100);
	      if (gzfindstringinstring(pxtk_buffer,"#Oblique Slice Position")==0)
		{
		  gzclose(fin);
		  return 0;
		}


	      float  d,n[3]; 
	      gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100);
	      sscanf(pxtk_buffer,"%f %f %f",&n[0],&n[1],&n[2]);
	      gzgets(fin,pxtk_buffer,100); gzgets(fin,pxtk_buffer,100);
	      sscanf(pxtk_buffer,"%f",&d);
	      gzclose(fin);	      
	      
	      this->ObliqueImageSlice->UpdateImagePlane(d,n[0],n[1],n[2]);
	      //	      fprintf(stderr,"Loaded calling Update Scales\n");
	      this->UpdateScales();

	      return 1;
	    }
	}

    }
  return 0;
}
// ----------------------------------------------------------------------------------------
int vtkpxGUIObliqueImageSliceControl::Save()
{
  if (this->Initialized==0 ||  has_obliqueslice==PXFalse)
    return 0;

  vtkPlaneSource* plane=this->ObliqueImageSlice->GetImagePlane();
  if (plane==NULL)
    return 0;

  char* line=this->EventManager->getSaveFilename("Save Oblique Slice Position","position.obl","Oblique Slice","*.obl");
  if (line!=NULL)
    {
      if (strlen(line)>0)
	{
	  FILE* fout=fopen(line,"w");
	  if (fout)
	    {
	      fprintf(fout,"#Oblique Slice Position\n");
	      
	      float  d=this->ObliqueImageSlice->GetDistance();
	      double n[3]; this->ObliqueImageSlice->GetNormal(n);
	      double c[3]; this->ObliqueImageSlice->GetCenter(c);
	      fprintf(fout,"#Normal\n %.4f %.4f %.4f\n#Distance\n%.3f\n",
		      n[0],n[1],n[2],d);
	      fprintf(fout,"#Center\n %.4f %.4f %.4f\n",c[0],c[1],c[2]);
	      fclose(fout);
	      return 1;
	    }
	}
    }
  return 0;
}
// ----------------------------------------------------------------------------------------
int vtkpxGUIObliqueImageSliceControl::SaveCurrentImage()
{
 if (this->Initialized==0 ||  has_obliqueslice==PXFalse)
    return 0;

  vtkPlaneSource* plane=this->ObliqueImageSlice->GetImagePlane();
  if (plane==NULL)
    return 0;
  
  vtkImageData* image=this->ObliqueImageSlice->GetObliqueImage();
  if (image==NULL)
    return 0;

  char* line=this->EventManager->getSaveFilename("Save Oblique Image","oblique.hdr","Analyze Image Files","*.hdr");
  if (line!=NULL)
    {
      if (strlen(line)>0)
	{
	  vtkpxAnalyzeImageWriter* ana=vtkpxAnalyzeImageWriter::New();
	  ana->SetInput(image);
	  ana->Save(line);
	  ana->Delete();
	  return 1;
	}
    }

  return 0;
}

int vtkpxGUIObliqueImageSliceControl::SaveCurrentTransform()
{
 if (this->Initialized==0 ||  has_obliqueslice==PXFalse)
    return 0;

  vtkTransform* xform=this->ObliqueImageSlice->GetObliqueTransform();
  if (xform==NULL)
    return 0;

  char* line=this->EventManager->getSaveFilename("Save Oblique Transformation","oblique.matr","Matrix Files","*.matr");
  if (line!=NULL)
    {
      if (strlen(line)>0)
	{
	  vtkpxTransformationUtil::SaveMatrixTransform(xform,line);
	  return 1;
	}
    }

  return 0;
}

