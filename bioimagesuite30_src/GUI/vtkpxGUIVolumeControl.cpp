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
  Module:    $RCSfile: vtkpxGUIVolumeControl.cpp,v $
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
#include "vtkpxGUIColormapEditor.h"
#include "vtkpxGUIVolumeControl.h"
#include "vtkPolyData.h"
#include "vtkObjectFactory.h"
#include "pxtkapp.h"
#include "pxutil.h"
#include "vtkpxBaseCurve.h"

vtkpxGUIVolumeControl* vtkpxGUIVolumeControl::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIVolumeControl");
  if(ret)
      {
	return (vtkpxGUIVolumeControl*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIVolumeControl;
}

// Construct object with no children.
vtkpxGUIVolumeControl::vtkpxGUIVolumeControl()
{
  has_volume=PXFalse;
  this->Volume=NULL;

  this->ColormapEditor=NULL;
  this->Viewer=NULL;
  this->internal_viewer=NULL;
  this->show_volume=NULL;
  this->Visibility=PXFalse;
  this->DualInput=PXFalse;
  this->OrthoViewer=NULL;
  this->LandmarkControl=NULL;
}
// ------------------------------------------------------------------------------------
void vtkpxGUIVolumeControl::SetVolumeRenderer(vtkpxVolume* vol,vtkpxGUIBaseRenderer* view)
{
  if (vol!=NULL)
    {
      this->Volume=vol;
      //outline=outl;
      this->Viewer=view;
      has_volume=PXTrue;
      resliceMode->setIndex(this->Volume->GetResliceMode());
      interpolationMode->setIndex(Irange(this->Volume->GetInterpolationMode(),0,2));
      //interpolationMode->selectItem(Irange(this->Volume->GetInterpolationMode(),0,2));
      this->Update(1);
    }

}
// ------------------------------------------------------------------------------------
void  vtkpxGUIVolumeControl::UpdateVolumeRenderer(vtkpxVolume* vol,vtkpxGUIBaseRenderer* ren)
{
  if (vol==NULL || ren==NULL)
    return;

  if (has_volume==PXFalse)
    {
      this->SetVolumeRenderer(vol,ren);
      return;
    }

  int d1[6]; this->Volume->GetImageRange(d1);
  int d2[6]; vol->GetImageRange(d2);


  int d=0;
  for (int ia=1;ia<=5;ia+=2)
    d+=abs(d1[ia]-d2[ia]);



  this->Volume=vol;
  this->Viewer=ren;
  this->Volume->SetResliceInterpolationModes(resliceMode->getIndex(),interpolationMode->getIndex());

  if (this->DualInput)
    this->Volume->SetDualInputMode(1);
  else
    this->Volume->SetDualInputMode(0);
  
  if (this->Volume->GetDualInputMode()==1)
    this->DualInput=PXTrue;
  else
    this->DualInput=PXFalse;


  if (d>0)
    this->Update(0);
  else
    this->UpdateCrop();
}
// ------------------------------------------------------------------------------------
void vtkpxGUIVolumeControl::SetRenderer(vtkpxGUIBaseRenderer* viewer1)
{
  this->Viewer=viewer1;
}
// ------------------------------------------------------------------------------------
char*  vtkpxGUIVolumeControl::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();
  
  vtkpxGUIComponent::Initialize(name,inside);

  PXTkFrame* parentFrame=(PXTkFrame*)(this->EventManager->getMainWindow());
  viewerFrame=new PXTkFrame(this->EventManager,parentFrame);

  volFrame=new PXTkFrame(this->EventManager,parentFrame);
  
  parentFrame->setPackMode(PXTrue);
  parentFrame->addChildren("-side left -expand false -fill both",viewerFrame,volFrame);

  volFrame->setPackMode(PXFalse);

  show_volume=new PXTkCheckButton(this->EventManager,volFrame,"Show Volume",104);
  show_volume->setState(this->Visibility);


  shadeOn=new PXTkCheckButton(this->EventManager,volFrame,"Shading",105);
  shadeOn->setState(PXFalse);

  fourColorMode=new PXTkCheckButton(this->EventManager,volFrame,"RGBA",108);
  fourColorMode->setState(PXFalse);

  volumeRenderingMode=new PXTkOptionMenu(this->EventManager,volFrame,"RayCasting",106);
  volumeRenderingMode->addOption("MIP");
  volumeRenderingMode->addOption("Texture2D");
  volumeRenderingMode->addOption("Texture3D");
  volumeRenderingMode->addOption("CropRay");
  volumeRenderingMode->setIndex(2);


  irregularClipping =new PXTkCheckButton(this->EventManager,volFrame,"Functional crop", 109);
  gradientEnhancement =new PXTkCheckButton(this->EventManager,volFrame,"Gradient Enhancement", 114);
 

  char* line=PXTkApp::getTclVariable("pxtcl_pref_array(VolumeTextureMode)");

  if (line==NULL)
    {
      volumeRenderingMode->setIndex(2);
    }
  else
    {
      int a=atoi(line);
      if (a==1 || a==2)
	volumeRenderingMode->setIndex(2);
      else if (a==3)
	volumeRenderingMode->setIndex(3);
      else
	volumeRenderingMode->setIndex(0);
    }

  volFrame->gridRowColumnChild(show_volume,0,0);
  volFrame->gridRowColumnChild(shadeOn,0,1);
  volFrame->gridRowColumnChild(volumeRenderingMode,1,0);  
  volFrame->gridRowColumnChild(fourColorMode,1,1);
  volFrame->gridRowColumnChild(irregularClipping,2,0);  
  volFrame->gridRowColumnChild(gradientEnhancement,2,1);  

  rateScale=new PXTkScale(this->EventManager,volFrame,"Interactive Rate",102,PXTrue);
  rateScale->setRange(0.0,60.0);
  rateScale->setDigits(4);
  rateScale->setValue(float(40));
  rateScale->setLengthWidth(120,10);
  rateScale->setResolution(1.0);

  rateScale2=new PXTkScale(this->EventManager,volFrame,"Still Rate",102,PXTrue);
  rateScale2->setRange(0.00,5.0);
  rateScale2->setDigits(4);
  rateScale2->setValue(float(1.0));
  rateScale2->setLengthWidth(120,10);
  rateScale2->setResolution(0.25);

  cropSphereRadius=new PXTkScale(this->EventManager,volFrame,"Sphere Radius",112,PXTrue);
  cropSphereRadius->setRange(1.0,100.0);
  cropSphereRadius->setDigits(4);
  cropSphereRadius->setValue(float(25));
  cropSphereRadius->setLengthWidth(120,10);
  cropSphereRadius->setResolution(1.0);
  
  cropCubeLength=new PXTkScale(this->EventManager,volFrame,"Cube Length",112,PXTrue);
  cropCubeLength->setRange(10.0,150.0);
  cropCubeLength->setDigits(4);
  cropCubeLength->setValue(float(0.1));
  cropCubeLength->setLengthWidth(120,10);
  cropCubeLength->setResolution(0.25);

  cropEllipseRadiusX=new PXTkScale(this->EventManager,volFrame,"Ellipsoidal Radius-X",112,PXTrue);
  cropEllipseRadiusX->setRange(10.0,100.0);
  cropEllipseRadiusX->setDigits(4);
  cropEllipseRadiusX->setValue(float(25));
  cropEllipseRadiusX->setLengthWidth(120,10);
  cropEllipseRadiusX->setResolution(5.0);
  

  cropEllipseRadiusY=new PXTkScale(this->EventManager,volFrame,"Ellipsoidal Radius-Y",112,PXTrue);
  cropEllipseRadiusY->setRange(10.0,100.0);
  cropEllipseRadiusY->setDigits(4);
  cropEllipseRadiusY->setValue(float(15));
  cropEllipseRadiusY->setLengthWidth(120,10);
  cropEllipseRadiusY->setResolution(5.0);  


  cropEllipseRadiusZ=new PXTkScale(this->EventManager,volFrame,"Ellipsoidal Radius-Z",112,PXTrue);
  cropEllipseRadiusZ->setRange(10.0,150.0);
  cropEllipseRadiusZ->setDigits(4);
  cropEllipseRadiusZ->setValue(float(15));
  cropEllipseRadiusZ->setLengthWidth(120,10);
  cropEllipseRadiusZ->setResolution(5.0);
  

  
  cropCylinderRadius=new PXTkScale(this->EventManager,volFrame,"Cylinder Radius",112,PXTrue);
  cropCylinderRadius->setRange(10.0,150.0);
  cropCylinderRadius->setDigits(4);
  cropCylinderRadius->setValue(float(0.1));
  cropCylinderRadius->setLengthWidth(120,10);
  cropCylinderRadius->setResolution(0.25);
  
  
  volFrame->gridRowColumnChild(cropSphereRadius,5,0);      
  volFrame->gridRowColumnChild(cropCubeLength,5,1);      
  volFrame->gridRowColumnChild(cropEllipseRadiusX,5,2);      
  volFrame->gridRowColumnChild(cropEllipseRadiusY,5,3);      
  volFrame->gridRowColumnChild(cropEllipseRadiusZ,5,4);      
  volFrame->gridRowColumnChild(cropCylinderRadius,5,5);      
  
  resliceMode=new PXTkOptionMenu(this->EventManager,volFrame,"Original Dimensions",103);
  resliceMode->addOption("Half Original");
  resliceMode->addOption("Quarter Original");
  resliceMode->addOption("64x64x64");
  resliceMode->addOption("128x128x64");
  resliceMode->addOption("128x128x128");
  resliceMode->addOption("256x256x128");
  resliceMode->addOption("256x256x256");
  resliceMode->addOption("512x512x256");
  resliceMode->addOption("512x512x512");
  resliceMode->setIndex(3);
  volFrame->gridRowColumnChild(resliceMode,6,0);

  interpolationMode=new PXTkOptionMenu(this->EventManager,volFrame,"No Interpolation",101);
  interpolationMode->addOption("Linear");
  interpolationMode->addOption("Cubic");
  interpolationMode->setIndex(1);

  croppingShape =new PXTkOptionMenu(this->EventManager,volFrame,"Sphere",113);
  croppingShape->addOption("Cube");
  croppingShape->addOption("Ellipsoid");
  croppingShape->addOption("Cylinder");
  croppingShape->setIndex(0);

  /*  interpolationMode=new PXITkComboBox(this->EventManager,volFrame,"Interp",101);
  interpolationMode->addItem("None");
  interpolationMode->addItem("Linear");
  interpolationMode->addItem("Cubic");
  interpolationMode->selectItem(1);*/

  volFrame->gridRowColumnChild(croppingShape,6,1);

  volFrame->gridRowColumnChild(interpolationMode,7,1);

  PXTkLabelFrame* cropFrame=new PXTkLabelFrame(this->EventManager,volFrame,"Cropping");
  volFrame->gridRowColumnChild(cropFrame,8,0,1,2);
  cropFrame->setPackMode(PXFalse);



  for (int ia=0;ia<=2;ia++)
      {
	for (int ib=0;ib<=1;ib++)
	    {
	      if (ia==0)
		  strcpy(pxtk_buffer4,"x-");
	      else if (ia==1)
		  strcpy(pxtk_buffer4,"y-");
	      else
		  strcpy(pxtk_buffer4,"z-");
	      
	      if (ib==0)
		  sprintf(pxtk_buffer4,"%s-min",pxtk_buffer4);
	      else
		  sprintf(pxtk_buffer4,"%s-max",pxtk_buffer4);
	      
	      int index=ia*2+ib;
	      clipScale[index]=new PXTkScale(this->EventManager,cropFrame,pxtk_buffer4,107);
	      clipScale[index]->setRange(0,100);
	      clipScale[index]->setLengthWidth(120,10);
	      clipScale[index]->setDigits(4);
	      clipScale[index]->setResolution(1);
	      volFrame->gridRowColumnChild(clipScale[index],1+ia,ib);
	    }
      }
  
  PXTkFrame*  mFrame=new PXTkFrame(this->EventManager,volFrame);
  volFrame->gridRowColumnChild(mFrame,9,0,1,2);


  PXTkButton* b1c0=new PXTkButton(this->EventManager,mFrame,"St",120);
  PXTkButton* b1c=new PXTkButton(this->EventManager,mFrame,"Nr",121);
  PXTkButton* b1d=new PXTkButton(this->EventManager,mFrame,"F1",122);
  PXTkButton* b1e=new PXTkButton(this->EventManager,mFrame,"F2",123);
  PXTkButton* b1f=new PXTkButton(this->EventManager,mFrame,"F4",124);
  PXTkButton* b1g=new PXTkButton(this->EventManager,mFrame,"Colormap",110);


  b1c0->configure("-padx",1);
  b1c->configure("-padx",1);
  b1d->configure("-padx",1);
  b1e->configure("-padx",1);
  b1f->configure("-padx",1);
  b1g->configure("-padx",1);

  mFrame->addChildren("-side left ",b1c0,b1c,b1d);
  mFrame->addChildren("-side left ",b1e,b1f);
  mFrame->addChildren("-side right -expand true -fill both",b1g);

  if (!this->EventManager->isInside())
      {
	PXTkButton* b5=new PXTkButton(this->EventManager,volFrame,"Close",3); 
	volFrame->gridRowColumnChild(b5,10,1,1,2);

	PXTkLabel* lab=new PXTkLabel(this->EventManager,volFrame,"");
	lab->configure("-image","$::pxtclvtkpxcontrib::smalllogo ");
	volFrame->gridRowColumnChild(lab,10,0);
      }

  
  ColormapEditor=vtkpxGUIColormapEditor::New();
  ColormapEditor->SetSimpleMode(0);
  ColormapEditor->SetVolumeMode(1);
  ColormapEditor->SetCallback(this,111);
  ColormapEditor->Initialize(this->GetWidgetName(),0);
  ColormapEditor->SetRange(0,255);


  int vm=this->volumeRenderingMode->getIndex();
  if (vm==2 || vm==3)
    this->ShowRateControls(0);
  else
    this->ShowRateControls(1);

  if(vm ==4)
	this->ShowCropControls(1);
  else
	this->ShowCropControls(0);

  // By MPJ
  sprintf(pxtk_buffer,"wm protocol %s WM_DELETE_WINDOW { pxtkcallback %ld 3 }",this->GetWidgetName(),this->EventManager->getHandlerNo());
  PXTkApp::executeTclCommand(pxtk_buffer);
  // By MPJ

  this->Initialized=1;

  return GetWidgetName();
}
 // ------------------------------------------------------------------------------------
void vtkpxGUIVolumeControl::Update(int full)
{
  if (!this->Initialized || !has_volume)
      return;

  
  int range[6]; 
  this->Volume->GetImageRange(range);
  if (full>0)
    {
      for (int ia=0;ia<3;ia++)
	for (int ib=0;ib<2;ib++)
	  {
	    int index=ia*2+ib;
	    int vindex=ia*2+1;
	    clipScale[index]->setRange(0,(float)range[vindex]);
	    clipScale[index]->setResolution(1);
	    
	    if (ib==0)
	      clipScale[index]->setValue(0);
	    else
	      clipScale[index]->setValue((float)range[index]);
	  }


      if (full==1)
	{
	  //	  fprintf(stderr,"Setting Colormap Range\n");
	  ColormapEditor->SetRange(0,this->Volume->GetMaxIntensity());
	  //	  fprintf(stderr,"Done\n");
	  this->UpdateCrop();
	}
    }

  this->Volume->SetFromLookupTable(ColormapEditor->GetColormap());
  this->Volume->SetResliceInterpolationModes(resliceMode->getIndex(),interpolationMode->getIndex());

  
  if (this->Visibility)	
    {
      this->Volume->SetShadingMode(shadeOn->getState());
      
      int vm=this->volumeRenderingMode->getIndex();
      this->Volume->SetVolumeRenderingMode(vm);
      this->Volume->SetVisibility(1);
    }
  else
    {
      this->Volume->SetVisibility(0);
      int range[6];
      for (int ia=0;ia<=5;ia++)
	range[ia]=(int)(clipScale[ia]->getValue());
      this->Volume->SetCroppingRange(range);
    }

  if (full==2)
    this->UpdateCrop();

}
// ------------------------------------------------------------------------------------
void vtkpxGUIVolumeControl::UpdateDisplay()
{
  if (!this->Initialized || !has_volume)
      return;

  this->Viewer->SetUpdateRate(rateScale->getValue(),rateScale2->getValue());
  if (!this->Visibility)
      return;
  
  this->UpdateCrop();
}
// ------------------------------------------------------------------------------------
int  vtkpxGUIVolumeControl::GetClipScaleValue(int i)
{
  if (i<0) i=0;
  if (i>5) i=5;
  int v=(int)(clipScale[i]->getValue());
  return v;
}

void vtkpxGUIVolumeControl::SetClipScaleValue(int i,int v)
{
  if (i<0) i=0;
  if (i>5) i=5;
  if (v!=-1)
    {
      clipScale[i]->setValue((float)v);
    }
  else
    {
      int d1[6]; this->Volume->GetImageRange(d1);
      clipScale[i]->setValue(float(d1[i]));
    }
    
}

int vtkpxGUIVolumeControl::UpdateCrop()
{
  if (!this->Initialized || !has_volume)
      return 0;

  int range[6],oldrange[6];

  this->Volume->GetCroppingRange(oldrange);
  
  int sum=0;

  for (int ia=0;ia<=5;ia++)
    {
      range[ia]=(int)(clipScale[ia]->getValue());
      sum+=abs(range[ia]-oldrange[ia]);
    }

  if (sum>0)
    this->Volume->SetCroppingRange(range);

  if (this->Visibility)	
    this->Volume->Update();

  return 1;
}
// ------------------------------------------------------------------------------------
void vtkpxGUIVolumeControl::SetVisibility(int vis)
{
  vis=(vis>0);
  if (vis)
    this->Visibility=PXTrue;
  else
    this->Visibility=PXFalse;

  if (this->show_volume!=NULL)
    show_volume->setState(this->Visibility);
  
  if (this->Volume!=NULL)
    {
      if (this->Visibility)
	this->Volume->SetVisibility(1);
      else
	this->Volume->SetVisibility(0);
    }

}
// ------------------------------------------------------------------------------------
void vtkpxGUIVolumeControl::ShowRateControls(int show)
{
  if (!this->Initialized)
    return;
  
  if (show==0)
    {
      volFrame->unmapChild(rateScale);
      volFrame->unmapChild(rateScale2);
    }
  else
    {
      volFrame->gridRowColumnChild(rateScale,4,0);
      volFrame->gridRowColumnChild(rateScale2,4,1);
    }

}
// ------------------------------------------------------------------------------------
void vtkpxGUIVolumeControl::ShowCropControls(int show)
{
  if (!this->Initialized)
    return;
  
  

  if (show==0)
  {
	  volFrame->unmapChild(croppingShape); // needs to be fixed. 
	  volFrame->unmapChild(cropSphereRadius);
	  volFrame->unmapChild(cropCubeLength);		
	  volFrame->unmapChild(cropEllipseRadiusX);
	  volFrame->unmapChild(cropEllipseRadiusY);
	  volFrame->unmapChild(cropEllipseRadiusZ);
	  volFrame->unmapChild(cropCylinderRadius);		
  }
  else if(show==1)
  {
	  volFrame->gridRowColumnChild(croppingShape,6,1);
	  volFrame->gridRowColumnChild(cropSphereRadius,5,0);      	 
	  volFrame->unmapChild(cropCubeLength);		
	  volFrame->unmapChild(cropEllipseRadiusX);
	  volFrame->unmapChild(cropEllipseRadiusY);
	  volFrame->unmapChild(cropEllipseRadiusZ);
	  volFrame->unmapChild(cropCylinderRadius);		
	  volFrame->gridRowColumnChild(resliceMode,6,0);
  }
  else if(show==2)
  {
	  volFrame->gridRowColumnChild(croppingShape,6,1);
	  volFrame->gridRowColumnChild(cropCubeLength,5,0);      	
	  volFrame->unmapChild(cropSphereRadius);		
	  volFrame->unmapChild(cropEllipseRadiusX);
	  volFrame->unmapChild(cropEllipseRadiusY);
	  volFrame->unmapChild(cropEllipseRadiusZ);
	  volFrame->unmapChild(cropCylinderRadius);
	  volFrame->gridRowColumnChild(resliceMode,6,0);
  }
  else if(show==3)
  {
	  volFrame->gridRowColumnChild(croppingShape,7,1);
	  volFrame->gridRowColumnChild(cropEllipseRadiusX,5,0);      
	  volFrame->gridRowColumnChild(cropEllipseRadiusY,5,1);      
	  volFrame->gridRowColumnChild(cropEllipseRadiusZ,6,0);      
	  volFrame->unmapChild(cropSphereRadius);
	  volFrame->unmapChild(cropCubeLength);				 
	  volFrame->unmapChild(cropCylinderRadius);
	  volFrame->gridRowColumnChild(resliceMode,7,0);
  }
  else if(show==4)
  {
	  volFrame->gridRowColumnChild(croppingShape,6,1);
	  volFrame->gridRowColumnChild(cropCylinderRadius,5,0);      
	  volFrame->unmapChild(cropSphereRadius);
	  volFrame->unmapChild(cropCubeLength);		
	  volFrame->unmapChild(cropEllipseRadiusX);
	  volFrame->unmapChild(cropEllipseRadiusY);
	  volFrame->unmapChild(cropEllipseRadiusZ);
	  volFrame->gridRowColumnChild(resliceMode,6,0);

  }

}

// -------------------------------------------------------------------------

int  vtkpxGUIVolumeControl::HandleEvent(int event)
{
	double dirvec[3] = {1, 0, 0};

  // By MPJ
  if (event == 3) {
    sprintf(pxtk_buffer,"wm iconify %s",this->GetWidgetName());
    return PXTkApp::executeTclCommand(pxtk_buffer);
  } else {
    if (event<=10)
      return this->EventManager->handleDirectEvent(event);
  }
  // By MPJ
  
  if (!has_volume)    // Nothing to do 
      return TCL_OK;

  /*  if (this->LandmarkControl!=NULL)
    {
      vtkpxBaseCurve* land=this->LandmarkControl->GetCollection(-1);
      double sp[3]; land->GetSpacing(sp);
      double ori[3]; land->GetOrigin(ori);
      fprintf(stdout,"num land points=%d\n",land->GetNumPoints());
      vtkPoints* pts=land->GetPoints();
      if (land->GetNumPoints()>0)
	{
	  double x[3];
	  pts->GetPoint(0,x);
	  fprintf(stderr, "First Point Raw = %f %f %f\n",x[0],x[1],x[2]);
	  for (int ia=0;ia<=2;ia++)
	    x[ia]=x[ia]*sp[ia]+ori[ia];
	  fprintf(stderr, "First Point = %f %f %f\n",x[0],x[1],x[2]);
	}
    }
  else
  fprintf(stderr,"Landmark Control is NULL\n");*/


  switch (event)
      {
      case 101:
	this->Volume->SetResliceInterpolationModes(resliceMode->getIndex(),interpolationMode->getIndex());
	break;

      case 102:
	UpdateDisplay();
	break;
	
      case 103:
	this->Update(2);
	break;

      case 104:
	this->Visibility=show_volume->getState();
	if (this->Visibility)
	  this->Volume->SetVisibility(1);
	else
	  this->Volume->SetVisibility(0);
	break;
	
      case 105:
	this->Volume->SetShadingMode((int)shadeOn->getState());
	UpdateCrop();
	break;
	
      case 106:
	{
	  int vm=this->volumeRenderingMode->getIndex();
	  if (vm==2 || vm==3)
	    {
	      this->ShowRateControls(0);
	      this->ShowCropControls(0);
	    }
	  else
	    {
	      this->ShowRateControls(1);		
	      this->ShowCropControls(1);		
	    }
	  this->Volume->SetVolumeRenderingMode(vm);
	  UpdateCrop();
	}
	break;

      case 107:
	UpdateCrop();
	break;

      case 108:
	this->Volume->SetRGBAMode((int)fourColorMode->getState());
	UpdateCrop();
	break;

      case 109:
	if(this->irregularClipping->getState())
	  this->Volume->SetIrregularCroppingMode(1);
	else
	  this->Volume->SetIrregularCroppingMode(0);
	break;
	
      case 110:
	ColormapEditor->Show();
	break;

      case 112:
	
	this->Volume->SetCropValues((int)croppingShape->getIndex(), 
				    (double)cropSphereRadius->getValue(),
				    (double)cropCubeLength->getValue(),
				    (double)cropCubeLength->getValue(),
				    (double)cropCubeLength->getValue(),
				    (double)cropEllipseRadiusX->getValue(),
					(double)cropEllipseRadiusY->getValue(),
					(double)cropEllipseRadiusZ->getValue(),
				    (double)cropCylinderRadius->getValue(),
				    20, dirvec);	
	this->UpdateOrthogonalViewerNavigationPointer();		  
	
	break; 


      case 113:		
	this->ShowCropControls((int)croppingShape->getIndex()+1);			
	this->UpdateOrthogonalViewerNavigationPointer();
	
	this->Volume->SetCropValues((int)croppingShape->getIndex(), 
				    (double)cropSphereRadius->getValue(),
				    (double)cropCubeLength->getValue(),
				    (double)cropCubeLength->getValue(),
				    (double)cropCubeLength->getValue(),
				    (double)cropEllipseRadiusX->getValue(),
					(double)cropEllipseRadiusY->getValue(),
					(double)cropEllipseRadiusZ->getValue(),
				    (double)cropCylinderRadius->getValue(), 20, dirvec);		
	
	break;
	
	// gradient enhancement
      case 114:
	if(this->gradientEnhancement->getState())
	  this->Volume->SetGradientEnhancementMode(1);
	else
	  this->Volume->SetGradientEnhancementMode(0);
	break;
	
      case 120:
      case 121:
      case 122:
      case 123:
      case 124:
	this->SetPresetLookupTable(event-121);
	break;

      case 111:
	if (ColormapEditor!=NULL)
	  {
	    this->Volume->SetFromLookupTable(ColormapEditor->GetColormap());
	  }		    
	break;


      }
  
  Viewer->Render();
  return TCL_OK;
}
// ------------------------------------------------------------------------------------
vtkpxGUIBaseRenderer* vtkpxGUIVolumeControl::GetViewer()
{
  return this->Viewer;
}

void vtkpxGUIVolumeControl::SetPresetLookupTable(int m)
{
  m=Irange(m,-1,5);
  if (ColormapEditor!=NULL)
    {

      int oldint=(interpolationMode->getIndex()>0);
      int newint=0;
      if (m>0)
	newint=0;
      else
	newint=1;

      if (oldint!=newint)
	{
	  interpolationMode->setIndex(newint);
	  //interpolationMode->selectItem(newint);
	  this->Volume->SetResliceInterpolationModes(resliceMode->getIndex(),interpolationMode->getIndex());
	}
      ColormapEditor->SetFmriMode(m);
    }
}
// ------------------------------------------------------------------------------------
void vtkpxGUIVolumeControl::SetLookupTable(vtkLookupTable* lkup,int auto_convert)
{



}
// ------------------------------------------------------------------------------------
int vtkpxGUIVolumeControl::UpdateOrthogonalViewerNavigationPointer()
{
  if (this->OrthoViewer==NULL)
    return 0;
  
  int md=(int)croppingShape->getIndex();
  
  double r=(double)cropSphereRadius->getValue();
  
  double cubelength = (double)cropCubeLength->getValue();
  
  if (md==0)
    this->OrthoViewer->SetNavigationPointerGeometryToSphere(r);
  else if (md==1)
    this->OrthoViewer->SetNavigationPointerGeometryToCube(cubelength);
  else if (md==2)
    {
      // This needs to be changed to do ellipse in ortho viewer but KEEP cone
      this->OrthoViewer->SetNavigationPointerGeometryToEllipse(cropEllipseRadiusX->getValue(), cropEllipseRadiusY->getValue(), cropEllipseRadiusZ->getValue());
    }
  else if (md==3)
    {
      this->OrthoViewer->SetNavigationPointerGeometryToCylinder(cropCylinderRadius->getValue()); 
    }

  int dm=this->OrthoViewer->GetNavigationPointerDrawMode();

  if (dm<2)
    this->OrthoViewer->RenderNavigationPointer(3);

  return 1;
}

