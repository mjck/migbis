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





// 	$Id: vtkpxGUIFemPostControl.cpp,v 1.1 2003/05/28 16:42:54 papad Exp papad $	

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkpxGUIFemPostControl.h"
#include "vtkpxUtil.h"
#include "vtkActor.h"
#include "vtkAssembly.h"
#include "vtkRenderer.h"
#include "vtkObjectFactory.h"
#include "vtkpxFemSolid.h"
#include "vtkPolyDataMapper.h"
#include "vtkImageData.h"
#include "vtkpxGUI4DRenderer.h"
#include "vtkCollection.h"
#include "vtkActorCollection.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTransform.h"
#include "vtkRenderWindow.h"
#include "vtkpxBaseCurve.h"
#include "vtkProperty.h"
#include "vtkPoints.h"
#include "pxabaqussolid.h"

/* -------------------------------------------------------------------------*/
vtkpxGUIFemPostControl* vtkpxGUIFemPostControl::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIFemPostControl");
  if(ret)
    {
      return (vtkpxGUIFemPostControl*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIFemPostControl;
}

// ----------------------------------------------------------------------------
vtkpxGUIFemPostControl::vtkpxGUIFemPostControl()
{
  for (int j=0;j<TSP_MAX_RENDERERS;j++)
    {
      this->MultiRenderer[j]=NULL;
      this->Renderer[j]=NULL;
      this->wireframemode[j]=0;
      this->solidActor[j]=NULL;
    }
  
  this->CurrentFrame=0;
  this->NumberOfFrames=0;
  this->ActualNumberOfFrames=0;
  this->scalingTransformation=vtkTransform::New();
  this->scalingTransformation->Identity();
  this->transformFilterCollection=NULL;
  this->solidCollection=NULL;
  this->solidActorCollection=NULL;
  this->currentSolid=NULL;

  this->currentImageNumberOfFrames=0;
  this->currentOrientation=0;
  this->displayMode=1;
  
  study_filename=new char[20];
  sprintf(study_filename,"unnamed.sld");


  for (int ia=0;ia<=2;ia++)
    {
      this->ReferencePoint[ia]=0.0;
      this->ScaleFactor[ia]=1.0;
    }
}
// ----------------------------------------------------------------------------
vtkpxGUIFemPostControl::~vtkpxGUIFemPostControl()
{
  
  if (this->solidCollection!=NULL)
    this->solidCollection->Delete();

  if (this->transformFilterCollection!=NULL)
    this->transformFilterCollection->Delete();

  if (this->solidActorCollection==NULL)
    this->solidActorCollection->Delete();

  // Set to NULL to avoid deleting by parent
  this->currentSolid=NULL;
  for (int j=0;j<TSP_MAX_RENDERERS;j++)
    this->solidActor[j]=NULL;
    
  delete [] study_filename;
  this->scalingTransformation->Delete();

}
// ----------------------------------------------------------------------------
void vtkpxGUIFemPostControl::InitializeSolids()
{
  this->solidCollection=vtkCollection::New();
  this->solidActorCollection=vtkActorCollection::New();
  this->transformFilterCollection=vtkCollection::New();
}
// ----------------------------------------------------------------------------
void vtkpxGUIFemPostControl::ResetSolids()
{
  this->NumberOfFrames=this->currentImageNumberOfFrames;
  this->CreateFrames(this->NumberOfFrames);
  for (int i=1;i<this->NumberOfFrames;i++)
    this->UpdateFrame(i);
  this->UpdateFrame(0);
}

// ----------------------------------------------------------------------------
void vtkpxGUIFemPostControl::CreateFrames(int numframes)
{
  if (numframes<this->ActualNumberOfFrames)
    return;

  if (numframes>this->MultiRenderer[0]->GetNumberOfFrames())
    numframes=this->MultiRenderer[0]->GetNumberOfFrames();
  if (numframes<1)
    numframes=1;
  
  for (int fr=this->ActualNumberOfFrames;fr<numframes;fr++)
    {
      //      fprintf(stderr,"FEM Control :: Creating Frame =%d\n",fr);
      vtkpxFemSolid* solid=vtkpxFemSolid::New();
      solid->SetWarpMode(1);
      this->solidCollection->AddItem(solid);
      if (fr==0)
	this->currentSolid=solid;

      vtkTransformPolyDataFilter* trf=vtkTransformPolyDataFilter::New();
      this->transformFilterCollection->AddItem(trf);
      trf->SetInput(solid->GetOutput());
      trf->SetTransform(this->scalingTransformation);
      trf->Update();

      for (int r=0;r<TSP_MAX_RENDERERS;r++)
	{
	  vtkActor* tempActor=vtkActor::New();
	  this->solidActorCollection->AddItem(tempActor);
	  
	  if (this->MultiRenderer[r]!=NULL)
	    {
	      vtkPolyDataMapper* map=vtkPolyDataMapper::New();
	      map->SetInput(trf->GetOutput());
		  
	      tempActor->SetMapper(map);
	      if (displayMode)
		tempActor->SetVisibility(1);
	      else
		tempActor->SetVisibility(0);
	      
	      if (wireframemode[r]==1)
		{
		  tempActor->GetProperty()->SetRepresentationToWireframe();
		  tempActor->GetProperty()->SetLineWidth(2.0);
		  tempActor->GetProperty()->SetAmbient(1.0);
		  tempActor->GetProperty()->SetDiffuse(0.0);
		  tempActor->GetProperty()->SetSpecular(0.0);
		}
	      map->Delete();
	      this->MultiRenderer[r]->GetRendererForFrame(fr)->AddActor(tempActor);
	      if (fr==0)
		this->solidActor[r]=tempActor;
	    }
	}
    }

  this->ActualNumberOfFrames=numframes;
  
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIFemPostControl::UpdateFrame(int frame)
{
  if (frame>=this->ActualNumberOfFrames)
    return;
  
  // Image Slice Ortho Slice First 
  int index=frame;
  int index2=index*TSP_MAX_RENDERERS;

  this->currentSolid=(vtkpxFemSolid*)this->solidCollection->GetItemAsObject(index);
  for (int r=0;r<TSP_MAX_RENDERERS;r++)
    {
      solidActor[r]=(vtkActor*)this->solidActorCollection->GetItemAsObject(index2);
      ++index2;
    }
    
  for (int rr=0;rr<TSP_MAX_RENDERERS;rr++)
    {
      if (this->MultiRenderer[rr]!=NULL)
	this->Renderer[rr]=this->MultiRenderer[rr]->GetRendererForFrame(frame);
    }

}
// -------------------------------------------------------------------------
void vtkpxGUIFemPostControl::ChangeFrame(int newframe)
{
  newframe=Irange(newframe,0,this->NumberOfFrames-1);
  if (newframe==this->CurrentFrame)
    return;
  this->CurrentFrame=newframe;
}
// -------------------------------------------------------------------------

char* vtkpxGUIFemPostControl::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
    return GetWidgetName();

  this->InitializeSolids();

  vtkpxGUIComponent::Initialize(name,inside);
  PXTkFrame* mFrame=(PXTkFrame*)this->EventManager->getMainWindow();

  showHide=new PXTkCheckButton(this->EventManager,mFrame,"Show",401);
  showHide->setState(PXTrue);
  statusLine1=new PXTkLabel(this->EventManager,mFrame,"No Surface");
  statusLine2=new PXTkLabel(this->EventManager,mFrame,"0 pts, 0 elements");
  mFrame->addChildren("-side top -expand false -fill x -pady 0 ",showHide,statusLine1,statusLine2);

  for (int i=0;i<=2;i++)
    {
      PXTkFrame* f=new PXTkFrame(this->EventManager,mFrame);
      mFrame->addChildren("-side top -expand false -fill x -pady 0 ",f);
      PXTkLabel* l=NULL;
      PXTkOptionMenu* menu=NULL;
      switch (i)
	{
	case 0:
	  l=new PXTkLabel(this->EventManager,f,"Mode:");
	  modeMenu=new PXTkOptionMenu(this->EventManager,f,"None",400);
	  modeMenu->addOption("PiesBW");
	  modeMenu->addOption("Pies");
	  modeMenu->addOption("Strains");
	  modeMenu->addOption("HSV-Strain");
	  menu=modeMenu;
	  break;
	case 1:
	  l=new PXTkLabel(this->EventManager,f,"Plot:");
	  plotMenu=new PXTkOptionMenu(this->EventManager,f,"S1",400);
	  plotMenu->addOption("S2");
	  plotMenu->addOption("S3");
	  plotMenu->addOption("S4");
	  plotMenu->addOption("S5");
	  plotMenu->addOption("S6");
	  menu=plotMenu;
	  break;
	case 2:
	  l=new PXTkLabel(this->EventManager,f,"Resolution:");
	  resolutionMenu=new PXTkOptionMenu(this->EventManager,f,"Elements",400);
	  resolutionMenu->addOption("Pies");
	  menu=resolutionMenu;
	  break;
	}
      f->addChildren("-side left -expand f -fill x",l,menu);
    }


  // ---------- scale stuff -------------
  PXTkLabelFrame* scFrame=new PXTkLabelFrame(this->EventManager,mFrame,"Scale");
  mFrame->addChildren("-side top -expand false -fill x",scFrame);
  for (int ia=0;ia<=2;ia++)
    {
      PXTkFrame* f=new PXTkFrame(this->EventManager,scFrame);
      scFrame->addChildren("-side top -expand false -fill x -pady 0 ",f);
      PXTkLabel* l=NULL;
      switch (ia)
	{
	case 0:
	  l=new PXTkLabel(this->EventManager,f,"scale x:");
	  break;
	case 1:
	  l=new PXTkLabel(this->EventManager,f,"scale y:");
	  break;
	case 2:
	  l=new PXTkLabel(this->EventManager,f,"scale z:");
	  break;
	}
      scaleFactorEntry[ia]=new PXTkEntry(this->EventManager,f);
      scaleFactorEntry[ia]->setText("1.0");
      f->addChildren("-side left -expand f -fill x",l,scaleFactorEntry[ia]);
    }
  PXTkButton* scalebut=new PXTkButton(this->EventManager,scFrame,"Scale Solids",807);
  scFrame->addChildren("-side top -expand f -fill x -pady 1 ",scalebut);

  // ---------- reslice stuff -------------
  PXTkLabelFrame* f3=new PXTkLabelFrame(this->EventManager,mFrame,"Reslicing Parameters");
  mFrame->addChildren("-side top -expand false -fill x",f3);  

  for (int k=0;k<=2;k++)
    {
      PXTkFrame* f=new PXTkFrame(this->EventManager,f3);
      f3->addChildren("-side top -expand false -fill x -pady 0 ",f);
      PXTkLabel* l=NULL;
      PXTkOptionMenu* menu=NULL;
      switch (k)
	{
	case 0:
	  l=new PXTkLabel(this->EventManager,f,"Slices:");
	  numslicesMenu=new PXTkOptionMenu(this->EventManager,f,"2",-1);
	  numslicesMenu->addOption("3");
	  numslicesMenu->addOption("4");
	  numslicesMenu->addOption("5");
	  numslicesMenu->addOption("6");
	  numslicesMenu->addOption("7");
	  numslicesMenu->addOption("8");
	  numslicesMenu->addOption("9");
	  numslicesMenu->addOption("10");
	  numslicesMenu->setIndex(1);
	  menu=numslicesMenu;
	  break;
	case 1:
	  l=new PXTkLabel(this->EventManager,f,"Sectors:");
	  numsectorsMenu=new PXTkOptionMenu(this->EventManager,f,"8",-1);
	  numsectorsMenu->addOption("12");
	  numsectorsMenu->addOption("16");
	  numsectorsMenu->addOption("4");
	  menu=numsectorsMenu;
	  break;
	case 2:
	  l=new PXTkLabel(this->EventManager,f,"Transmural:");
	  numtransmuralMenu=new PXTkOptionMenu(this->EventManager,f,"1",-1);
	  numtransmuralMenu->addOption("2");
	  numtransmuralMenu->addOption("3");
	  numtransmuralMenu->addOption("4");
	  numtransmuralMenu->setIndex(0);
	  menu=numtransmuralMenu;
	  break;
	}
      f->addChildren("-side left -expand f -fill x",l,menu);
    }

  anticlockwise=new PXTkCheckButton(this->EventManager,f3,"Anticlockwise",-1);
  anticlockwise->setState(PXTrue);

  PXTkFrame* f3c=new PXTkFrame(this->EventManager,f3);
  PXTkFrame* f4c=new PXTkFrame(this->EventManager,f3);
  f3->addChildren("-side top -expand f -fill x -pady 1 ",anticlockwise,f3c,f4c);

  PXTkButton* resl=new PXTkButton(this->EventManager,f3c,"Reslice",803);
  PXTkButton* b3a=new PXTkButton(this->EventManager,f3c,"Save Pies",804);
  f3c->addChildren("-side left -expand f -fill x -pady 1 -padx 2",resl,b3a);

  PXTkButton* b4b=new PXTkButton(this->EventManager,f4c,"Load Indices",805);
  PXTkButton* b4a=new PXTkButton(this->EventManager,f4c,"Save Indices",806);
  f4c->addChildren("-side left -expand f -fill x -pady 1 -padx 2",b4b,b4a);

  // --------- bottom layer ---------------------
  PXTkLabelFrame* gFrame=new PXTkLabelFrame(this->EventManager,mFrame,"Load/Save");
  mFrame->addChildren("-side top -expand false -fill x",gFrame);

  PXTkFrame* f2=new PXTkFrame(this->EventManager,gFrame);
  gFrame->addChildren("-side top -expand false -fill x",f2);
  PXTkButton* b0a=new PXTkButton(this->EventManager,f2,"Load Study",801);
  PXTkButton* b0b=new PXTkButton(this->EventManager,f2,"Load Ref",802);
  f2->addChildren("-side left -expand f -fill x -pady 1 -padx 2",b0a,b0b);


  if (!inside)
    {
      PXTkButton* closeB;
      if (this->EventParent)
	closeB=new PXTkButton(this->EventManager,mFrame,"Close",3);
      else
	closeB=new PXTkButton(this->EventManager,mFrame,"Exit",1);
      mFrame->addChildren("-side bottom -expand false -fill x -padx 2",closeB);
    }

  return GetWidgetName();
}

/* -------------------------------------------------------------------------*/ 
void vtkpxGUIFemPostControl::SetRenderer(vtkpxGUI4DRenderer* ren,int index)
{
  this->SetRenderer(ren,index,0);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIFemPostControl::SetRenderer(vtkpxGUI4DRenderer *ren,int index,int wireframe)
{
  index=Irange(index,0,TSP_MAX_RENDERERS-1);

  if (this->MultiRenderer[index]!=NULL)
    return;

  this->MultiRenderer[index]=ren;
  this->wireframemode[index]=wireframe;
  this->Renderer[index]=this->MultiRenderer[index]->GetRendererForFrame(this->CurrentFrame);

}
/* -------------------------------------------------------------------------*/
void vtkpxGUIFemPostControl::SetImage(vtkImageData* img,int orient)
{
  if (img!=NULL)  
    {
      this->currentImageNumberOfFrames=img->GetNumberOfScalarComponents();
      this->currentOrientation=orient;
      this->ResetSolids();
      this->SetCurrentFrame(0);
    }
  
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIFemPostControl::SetCurrentFrame(int t)
{
  if (t<0) 
    t=this->NumberOfFrames-1;
  if (t>this->NumberOfFrames-1)
    t=0;

  if (!this->Initialized)
    return;

  this->ChangeFrame(t);
  this->UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIFemPostControl::GetCurrentFrame()
{
  return this->CurrentFrame;
}

int vtkpxGUIFemPostControl::GetNumberOfFrames()
{
  return this->NumberOfFrames;
}
/* -------------------------------------------------------------------------*/
//
//    Handle Callbacks 
//
/* -------------------------------------------------------------------------*/
int  vtkpxGUIFemPostControl::HandleEvent(int event)
{
  if (!this->Initialized)
      return TCL_OK;

  if (event<10)
      return this->EventManager->handleDirectEvent(event);

  
  switch (event)
    {
    case 400: //Render Mode
      this->Update();
      break;
      
    case 401: // Show/Hide
      if (this->showHide->getState())
	this->displayMode=1;
      else
	this->displayMode=0;
      this->UpdateDisplay();
      break;
      
    case 801: // Load Study
      if (this->NumberOfFrames>0)
	{
	  char* line=this->EventManager->getOpenFilename("Load Study","Finite Element Study","*.stat");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		this->LoadStudy(line);
	    }
	}
      break;

    case 802: // Load Reference Point
      if (this->NumberOfFrames>0)
	{
	  char* line=this->EventManager->getOpenFilenameComplex("Load Reference Point",
								"{\"Reference/Landmark Files\" {\".ref\" \".land\" } } { \"Reference Point\" {\"*.ref\"}} {\"Landmarks\" {\"*.land\"}}");
	  //	  char* line=this->EventManager->getOpenFilename("Load Reference Point","Reference Point","*.ref");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		this->LoadReferencePoint(line);
	    }
	}
      break;
      
      
    case 803:
      this->ReslicePies();
      break;

    case 804:
      if (this->NumberOfFrames>0)
	{
	  sprintf(pxtk_buffer,"%s.pie",study_filename);
	  char* line=this->EventManager->getSaveFilename("Save pies",pxtk_buffer,"Pies","*.pie");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		{
		  strcpy(pxtk_buffer,line);
		  this->SavePies(pxtk_buffer);
		}
	    }
	}
      break;

    case 805: // Load Reference Point
      if (this->NumberOfFrames>0)
	{
	  char* line=this->EventManager->getOpenFilename("Load Pie Indices","Indices","*.pieindex");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		this->LoadIndices(line);
	    }
	}
      break;

    case 806: // Load Reference Point
      if (this->NumberOfFrames>0)
	{
	  sprintf(pxtk_buffer,"%s.pieindex",study_filename);
	  char* line=this->EventManager->getSaveFilename("Save Pie Indices",pxtk_buffer,"Indices","*.pieindex");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		this->SaveIndices(line);
	    }
	}
      break;


    case 807:
      for (int ia=0;ia<=2;ia++)
	this->ScaleFactor[ia]=Frange(atof(scaleFactorEntry[ia]->getText()),0.2,10.0);
      this->UpdateScaling(this->ScaleFactor[0],
			  this->ScaleFactor[1],
			  this->ScaleFactor[2]);
      break;
    }
   
  
  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIFemPostControl::UpdateScaling(float x,float y,float z)
{
  if (this->NumberOfFrames<1)
    return;

  this->scalingTransformation->Identity();
  this->scalingTransformation->Scale(x,y,z);

  for (int i=0;i<this->NumberOfFrames;i++)
    {
      vtkTransformPolyDataFilter* trf=(vtkTransformPolyDataFilter*)this->transformFilterCollection->GetItemAsObject(i);
      trf->Modified();
    }
  this->UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIFemPostControl::Update()
{
  if (this->NumberOfFrames<1)
    return;

  int mode=this->modeMenu->getIndex();
  int plot=this->plotMenu->getIndex();
  int res= this->resolutionMenu->getIndex();

  int strain=plot+6*res;

  
  int cmode=0;

  switch (mode)
    {
    case 0: // None
    case 1: // Pies BW
    case 2: // Pies
    case 3: // Strain 
      cmode=mode;
      break;

    case 4: // Strain HSV
      cmode=5;
      break;
    }



  for (int i=0;i<this->NumberOfFrames;i++)
    {
      vtkpxFemSolid* solid=(vtkpxFemSolid*)this->solidCollection->GetItemAsObject(i);
      solid->SetColorMode(cmode);
      solid->SetStrainMode(strain);
      solid->SetWarpMode(1);
      solid->Update();
      //fprintf(stderr,"Updated solid %d\n",i);
    }
    
  this->UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIFemPostControl::UpdateDisplay()
{
  for (int i=0;i<this->NumberOfFrames;i++)
    {
      int index2=i*TSP_MAX_RENDERERS;
      for (int r=0;r<TSP_MAX_RENDERERS;r++)
	{
	  vtkActor* tmp=(vtkActor*)this->solidActorCollection->GetItemAsObject(index2);
	  if (displayMode)
	    tmp->SetVisibility(1);
	  else
	    tmp->SetVisibility(0);
	}
    }

  if (this->Renderer[0]!=NULL)
    this->Renderer[0]->GetRenderWindow()->Render();

}
/* -------------------------------------------------------------------------*/
//
//       This is work all the work is done ....
// 
/* -------------------------------------------------------------------------*/
int vtkpxGUIFemPostControl::LoadStudy(const char* filename)
{
  static int debugmode=1;
  gzFile fin=gzsuffixopen(filename,"r");
  if (fin==NULL)
    {
      return 0;
    }
  
  char line[400];
  gzgets(fin,line,200);
  int mode=0;
  
  if (gzfindstringinstring(line, "#XPResults File Set"))
      mode=1;
  else if (gzfindstringinstring(line, "#AbaqusResults File Set"))
    mode=2;
  else if (gzfindstringinstring(line, "#Abaqus Results File Set"))
    mode=3;


  if (mode==0)
    {
      gzclose(fin);
      fprintf(stderr,"Not a .stat file\n");
      return 0;
    }
  
  int numframes,numel,numnodes;
  gzgets(fin,line,200); sscanf(line,"%d",&numframes);
  gzgets(fin,line,200); sscanf(line,"%d",&numnodes);
  gzgets(fin,line,200); sscanf(line,"%d",&numel);
  
  char solidname[100];  
  gzgets(fin,line,200);     gzgets(fin,line,200);  
  sscanf(line,"%s\n",&solidname[0]);
  char jobname[100]; 
  gzgets(fin,line,200);     gzgets(fin,line,200);  
  sscanf(line,"%s\n",&jobname[0]);

  float scale[3];
  
  if (mode==3)
    {
      gzgets(fin,line,200);     gzgets(fin,line,200);  
      sscanf(line,"%f %f %f",&scale[0],&scale[1],&scale[2]);
    } 
  else
    {
      scale[0]=1.0;
      scale[1]=1,0;
      scale[2]=1.0;
    }

  fprintf(stderr,"Reading set file %s \n",filename);
  fprintf(stderr,"Job stem =`%s' solid name=`%s' frames=%d nodes=%d elements=%d\n",
	  jobname,solidname,numframes,numnodes,numel);
  
  gzclose(fin);

  char* path=anaextractpath(filename);
  sprintf(line,"%s%s",path,solidname);


  /*if (numframes>this->NumberOfFrames)
    numframes=this->NumberOfFrames;*/

  this->CreateFrames(numframes);

  vtkpxFemSolid* basesolid=(vtkpxFemSolid*)this->solidCollection->GetItemAsObject(0);

  int ok=basesolid->Load(line);

  if (ok==0)
    {
      fprintf(stderr,"Failed to Load solid from %s \n",line);
      delete [] path;
      return 0;
    }

  if (basesolid->GetNumberOfElements()!= numel ||
      basesolid->GetNumberOfNodes() != numnodes)
    {
      fprintf(stderr,"Not correct numnodes or numelements\n");
      delete [] path;
      return 0;
    }


  fprintf(stderr,".. Strain data from %s%s : (or=%d) mode=%d,",path,jobname,
	  basesolid->GetInternalSolid()->getorientationmode(),mode);

  
  for (int ii=0;ii<numframes;ii++)
    {
      vtkpxFemSolid* newsolid=(vtkpxFemSolid*)this->solidCollection->GetItemAsObject(ii);
      if (ii>0)
	newsolid->Copy(basesolid);
      
      newsolid->CreateFemData();
      if (mode==1)
	{
	  sprintf(line,"%s%s_%02d",path,jobname,ii+1);
	  if (debugmode)
	    fprintf(stderr,"Reading old %s\n",line);
	  else
	    fprintf(stderr,"(%d) ",ii+1);
	  newsolid->GetInternalFemData()->LoadOld(line,1);
	}
      else if (mode>1)
	{
	  sprintf(line,"%s%s.%02d.dat",path,jobname,ii+1);
	  if (debugmode)
	    fprintf(stderr,"Reading new data: %s\n",line);
	  else
	    fprintf(stderr,"%d ",ii+1);
	  newsolid->GetInternalFemData()->Load(line,0,1);
	} 
    }
  
  if (debugmode)
    fprintf(stderr,"Done Reading\n");
  
  sprintf(line,"%s%s.ref",path,jobname);
  this->LoadReferencePoint(line);
  delete [] path;

  sprintf(line,"Nodes=%d Elements=%d",
	  basesolid->GetNumberOfNodes(),
	  basesolid->GetNumberOfElements());
  statusLine1->setText(solidname);
  statusLine2->setText(line);

  for (int ia=0;ia<=2;ia++)
    {
      this->ScaleFactor[ia]=scale[ia];
      sprintf(line,"%.2f",this->ScaleFactor[ia]);
      scaleFactorEntry[ia]->setText(line);
    }
      
  this->UpdateScaling(this->ScaleFactor[0],
		      this->ScaleFactor[1],
		      this->ScaleFactor[2]);


  if (basesolid->GetInternalSolid()->hasfemdata())
    {
      int orient=basesolid->GetInternalSolid()->getorientationmode();
		    
      if (orient==2 || orient==4)
	{
	  plotMenu->changeLabel(0,"XX");
	  plotMenu->changeLabel(1,"RR");
	  plotMenu->changeLabel(2,"FF");
	  plotMenu->changeLabel(3,"XR");
	  plotMenu->changeLabel(4,"XF");
	  plotMenu->changeLabel(5,"RF");
	}
      else if (orient==5)
	{
	  plotMenu->changeLabel(0,"P1");
	  plotMenu->changeLabel(1,"P2");
	  plotMenu->changeLabel(2,"P3");
	  plotMenu->changeLabel(3,"N/A");
	  plotMenu->changeLabel(4,"N/A");
	  plotMenu->changeLabel(5,"N/A");
	}
      else
	{
	  plotMenu->changeLabel(0,"RR");
	  plotMenu->changeLabel(1,"CC");
	  plotMenu->changeLabel(2,"LL");
	  plotMenu->changeLabel(3,"RC");
	  plotMenu->changeLabel(4,"RL");
	  plotMenu->changeLabel(5,"CL");
	}
    }

  if (numframes<this->NumberOfFrames)
    {
      for (int fr=numframes;fr<this->NumberOfFrames;fr++)
	{
	  vtkpxFemSolid* newsolid=(vtkpxFemSolid*)this->solidCollection->GetItemAsObject(fr);
	  newsolid->Copy(basesolid);
	  newsolid->CreateFemData();
	}
    }

  this->Update();
  return numframes;


}
/* -------------------------------------------------------------------------*/
int vtkpxGUIFemPostControl::LoadReferencePoint(const char* fname)
{
  static int debugmode=1;
  
  if (debugmode)
    fprintf(stderr,"Opening %s ... ",fname);
  else
    fprintf(stderr,"\n");
  
  int ok=0;

  gzFile fin=gzsuffixopen(fname,"r");
  if (fin)
    {
      char line[100];
      gzgets(fin,line,100);
      float rx,ry,rz;
      if (gzfindstringinstring(line,"#Landmark Data")!=0)
	{
	  // Treat as Landmark File
	  gzclose(fin);
	  vtkpxBaseCurve* curve=vtkpxBaseCurve::New();
	  ok=curve->Load(fname);
	  if (ok)
	    {
	      if (curve->GetNumPoints() > 0 ) 
		{
		  rx=curve->GetPoints()->GetPoint(0)[0];
		  ry=curve->GetPoints()->GetPoint(0)[1];
		  rz=curve->GetPoints()->GetPoint(0)[2];
		}
	    }
	  curve->Delete();
	}
      else
	{
	  sscanf(line,"%f %f %f",&rx,&ry,&rz);
	  ok=1;
	  gzclose(fin);
	}

      if (ok)
	{
	  fprintf(stderr,"Junction Point (%5.2f %5.2f %5.2f)\n",rx,ry,rz);
	  
	  this->ReferencePoint[0]=rx;
	  this->ReferencePoint[1]=ry;
	  this->ReferencePoint[2]=rz;
	}

      return ok;
    }

  return 0;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIFemPostControl::LoadIndices(const char* fname)
{
  vtkpxFemSolid* basesolid=(vtkpxFemSolid*)this->solidCollection->GetItemAsObject(0);
  if (basesolid->GetNumberOfElements() < 1)
      return 0;

  int ok=basesolid->GetInternalFemData()->loadPieDivision(fname);
  if (ok)
    {
      for (int i=1;i<this->NumberOfFrames;i++)
	{
	  vtkpxFemSolid* newsld=(vtkpxFemSolid*)this->solidCollection->GetItemAsObject(i);
	  newsld->GetInternalFemData()->copypieindex(basesolid->GetInternalFemData());
	}
    }
  return ok;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIFemPostControl::SaveIndices(const char* fname)
{
  vtkpxFemSolid* basesolid=(vtkpxFemSolid*)this->solidCollection->GetItemAsObject(0);
  return  basesolid->GetInternalFemData()->savePieDivision(fname);
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIFemPostControl::ReslicePies()
{
  int numslices=atoi(numslicesMenu->getValue());
  int numsectors=atoi(numsectorsMenu->getValue());
  int numtransmural=atoi(numtransmuralMenu->getValue());
  int anti=0;
  if (anticlockwise->getState())
    anti=1;

  fprintf(stderr,"Reslicing Pies Ref= %.2f,%.2f,%.2f\n",
	  this->ReferencePoint[0],
	  this->ReferencePoint[1],
	  this->ReferencePoint[2]);

  int ok=vtkpxFemSolid::ReslicePies(this->solidCollection,this->NumberOfFrames,
				    numslices,numsectors,numtransmural,anti,
				    this->ReferencePoint);

  if (ok)
    this->Update();
  return ok;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIFemPostControl::SavePies(const char* fname)
{
  int numslices=atoi(numslicesMenu->getValue());
  int numsectors=atoi(numsectorsMenu->getValue());
  int numtransmural=atoi(numtransmuralMenu->getValue());
  int anti=0;
  if (anticlockwise->getState())
    anti=1;

  fprintf(stderr,"Saving Pies Ref= %.2f,%.2f,%.2f\n",
	  this->ReferencePoint[0],
	  this->ReferencePoint[1],
	  this->ReferencePoint[2]);

  return vtkpxFemSolid::SavePies(fname,this->solidCollection,this->NumberOfFrames,
				 numslices,numsectors,numtransmural,anti,
				 this->ReferencePoint);
 
}



