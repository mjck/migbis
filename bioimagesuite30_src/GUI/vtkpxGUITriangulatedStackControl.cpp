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





// 	$Id: vtkpxGUITriangulatedStackControl.cpp,v 1.3 2002/12/18 16:11:09 papad Exp papad $	

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkpxGUITriangulatedStackControl.h"
#include "vtkpxUtil.h"
#include "vtkActor.h"
#include "vtkAssembly.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkObjectFactory.h"
#include "vtkpxImageExtract.h"
#include "vtkPolyDataMapper.h"
#include "vtkImageData.h"
#include "vtkProperty.h"
#include "pxfpbsplinestack.h"
#include "pxtriangulatedstack.h"
/* -------------------------------------------------------------------------*/
vtkpxGUITriangulatedStackControl* vtkpxGUITriangulatedStackControl::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUITriangulatedStackControl");
  if(ret)
      {
	return (vtkpxGUITriangulatedStackControl*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUITriangulatedStackControl;
}

// ----------------------------------------------------------------------------
vtkpxGUITriangulatedStackControl::vtkpxGUITriangulatedStackControl()
{
  this->NumberOfTriangulatedStacks=4;

  for (int ia=0;ia<=2;ia++)
    {
      this->origin[ia]=0.0;
      this->spacing[ia]=1.0;
      this->dimensions[ia]=50;
    }
  this->orientation=0;
  
  for (int i=0;i<this->NumberOfTriangulatedStacks;i++)
    {
      triangulatedStackSet[i]=NULL;
      for (int j=0;j<TSP_MAX_RENDERERS;j++)
	triangulatedStackActor[j][i]=NULL;
      display_mode[i]=0;
      color_mode[i]=-1;
    }

  for (int j=0;j<TSP_MAX_RENDERERS;j++)
    this->Renderer[j]=NULL;
  
  current_triangulatedstack=0;
  this->currentTriangulatedStack=NULL;
  clipboardTriangulatedStack=NULL;
  image=NULL;
}
// ----------------------------------------------------------------------------
vtkpxGUITriangulatedStackControl::~vtkpxGUITriangulatedStackControl()
{
  
  if (clipboardTriangulatedStack!=NULL)
    clipboardTriangulatedStack->Delete();

 
  for (int i=0;i<this->NumberOfTriangulatedStacks;i++)
    {
      if (triangulatedStackSet[i]!=NULL)
	triangulatedStackSet[i]->Delete();
      for (int j=0;j<TSP_MAX_RENDERERS;j++)
	if (triangulatedStackActor[j][i]!=NULL)
	  triangulatedStackActor[j][i]->Delete();
    }

  if (this->image!=NULL)
    this->image->Delete();
    
}
/* -------------------------------------------------------------------------*/
void vtkpxGUITriangulatedStackControl::SetNumberOfTriangulatedStacks(int a)
{
  if (this->Initialized==1)
    return;

  this->NumberOfTriangulatedStacks=Irange(a,1,TSP_MAX_COLLECTIONS);
}
// ----------------------------------------------------------------------------
void vtkpxGUITriangulatedStackControl::InitializeTriangulatedStacks()
{


  for (int i=0;i<this->NumberOfTriangulatedStacks;i++)
    {
      triangulatedStackSet[i]=vtkpxTriangulatedStackSource::New();
      PXContourStack* cstack=new PXContourStack(this->dimensions[2]);
      cstack->setellipticalcylinder(8,
				    spacing[0]*dimensions[0]/8.0*(1.0+i),
				    spacing[1]*dimensions[1]/8.0*(1.0+i),
				    origin[0]+spacing[0]*dimensions[0]/2.0,
				    origin[1]+spacing[1]*dimensions[1]/2.0);

      triangulatedStackSet[i]->SetFromContourStack(cstack);
      delete cstack;
    }

  current_triangulatedstack=0;
  currentTriangulatedStack=triangulatedStackSet[current_triangulatedstack];

  clipboardTriangulatedStack=vtkpxTriangulatedStackSource::New();
  clipboardTriangulatedStack->Copy(currentTriangulatedStack);

}
/* -------------------------------------------------------------------------*/
char* vtkpxGUITriangulatedStackControl::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();

  InitializeTriangulatedStacks();

  vtkpxGUIComponent::Initialize(name,inside);
  PXTkFrame* mFrame=(PXTkFrame*)this->EventManager->getMainWindow();
  this->controlFrame=new PXTkLabelFrame(this->EventManager,mFrame,"Triangulated Stack Control");
  mFrame->addChildren("-side top -expand t -fill x -padx 0 -pady 4 ",this->controlFrame);

  // Control Properties
  // ---------------------
  PXTkLabelFrame* vFrame=new PXTkLabelFrame(this->EventManager,this->controlFrame,"Global Properties");
  this->controlFrame->addChildren("-side top -expand false -fill x -padx 0 -pady 5",vFrame);
  vFrame->setPackMode(PXFalse);
 
  PXTkButton* b0a=new PXTkButton(this->EventManager,vFrame,"Show All",506);
  PXTkButton* b0b=new PXTkButton(this->EventManager,vFrame,"Hide All",505);
  vFrame->gridRowColumnChild(b0a,1,0);
  vFrame->gridRowColumnChild(b0b,1,1);
  
  // Collection Properties 
  // ---------------------
  PXTkLabelFrame* pFrame=new PXTkLabelFrame(this->EventManager,this->controlFrame,"Triangulated Stack Properties");
  this->controlFrame->addChildren("-side top -expand t -fill both -padx 0 -pady 2",pFrame);

  PXTkFrame* pfr0=new PXTkFrame(this->EventManager,pFrame);
  PXTkFrame* pfr1=new PXTkFrame(this->EventManager,pFrame);
  PXTkFrame* pfr2=new PXTkFrame(this->EventManager,pFrame);

  pFrame->addChildren("-side top -pady 0 -expand t -fill x",pfr0,pfr1,pfr2);


  PXTkLabel* labs=new PXTkLabel(this->EventManager,pfr0,"Current Stack:");
  currentCollection=new PXTkOptionMenu(this->EventManager,pfr0,"Stack 1",500);
  for (int ip=1;ip<this->NumberOfTriangulatedStacks;ip++)
    {
      sprintf(pxtk_buffer4,"Stack %d",ip+1);
      currentCollection->addOption(pxtk_buffer4);
    }
  currentCollection->setIndex(current_triangulatedstack);
  pfr0->addChildren("-side left -expand t -fill x",labs,currentCollection);

  PXTkLabel* labd=new PXTkLabel(this->EventManager,pfr1,"Display Mode:");
  displayMode=new PXTkOptionMenu(this->EventManager,pfr1,"Hide",501);
  displayMode->addOption("Points");
  displayMode->addOption("Wireframe");
  displayMode->addOption("Surface");
  displayMode->setIndex(display_mode[current_triangulatedstack]);
  pfr1->addChildren("-side left -expand t -fill x",labd,displayMode);

  PXTkLabel* labe=new PXTkLabel(this->EventManager,pfr2,"Color Mode:");
  colorMode=new PXTkOptionMenu(this->EventManager,pfr2,"None",501);
  colorMode->addOption("Gaussian");
  colorMode->addOption("Mean");
  colorMode->addOption("1st Princ");
  colorMode->addOption("2nd Princ");
  colorMode->addOption("Bending");
  colorMode->addOption("Shape Index");
  colorMode->setIndex(color_mode[current_triangulatedstack]);
  pfr2->addChildren("-side left -expand t -fill x",labe,colorMode);


  // Collection Operations 
  // ---------------------
  PXTkLabelFrame* oFrame=new PXTkLabelFrame(this->EventManager,this->controlFrame,"Operations on Current Stack");
  this->controlFrame->addChildren("-side top -expand t -fill both -padx 1 -pady 0",oFrame);

  PXTkFrame* midfr=new PXTkFrame(this->EventManager,oFrame);

  PXTkFrame* botfr=new PXTkFrame(this->EventManager,oFrame);  
  PXTkFrame* botfr2=new PXTkFrame(this->EventManager,oFrame);  
  oFrame->addChildren("-side top -pady 0 -expand t -fill x",midfr,botfr,botfr2);
  
  PXTkButton* but6=new PXTkButton(this->EventManager,midfr,"Color",450);
  PXTkButton* but5=new PXTkButton(this->EventManager,midfr,"Cylinder",452);
  
  but5->configure("-padx",0);
  but6->configure("-padx",0);
  midfr->addChildren("-side left -padx 1 -expand t -fill x",but6,but5);

  PXTkButton* but1=new PXTkButton(this->EventManager,botfr,"Load",401);
  PXTkButton* but2=new PXTkButton(this->EventManager,botfr,"Save",402);
  but1->configure("-padx",0);
  but2->configure("-padx",0);
  botfr->addChildren("-side left -padx 1 -expand t -fill x",but1,but2);

  PXTkButton* cp=new PXTkButton(this->EventManager,botfr2,"Copy",590);
  PXTkButton* pt=new PXTkButton(this->EventManager,botfr2,"Paste",591);
  cp->configure("-padx",0);
  pt->configure("-padx",0);
  botfr2->addChildren("-side left -padx 1 -expand t -fill x",cp,pt);

  
  if (!inside)
    {
      PXTkButton* closeB;
      if (this->EventParent)
	closeB=new PXTkButton(this->EventManager,mFrame,"Close",3);
      else
	closeB=new PXTkButton(this->EventManager,mFrame,"Exit",1);
      mFrame->addChildren("-side bottom -expand false -fill x -padx 2",closeB);
    }


  this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,10);

  // Other Stuff 
  return GetWidgetName();
}

/* -------------------------------------------------------------------------*/ 
void vtkpxGUITriangulatedStackControl::SetRenderer(vtkRenderer* ren,int index)
{
  this->SetRenderer(ren,index,0);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUITriangulatedStackControl::SetRenderer(vtkRenderer *ren,int index,int wireframe)
{
  index=Irange(index,0,TSP_MAX_RENDERERS-1);

  if (this->Renderer[index]!=NULL)
    return;

  for (int i=0;i<this->NumberOfTriangulatedStacks;i++)
      {
	if (triangulatedStackActor[index][i]==NULL)
	    {
	      vtkPolyDataMapper* map=vtkPolyDataMapper::New();
	      map->SetInput(triangulatedStackSet[i]->GetOutput());

	      triangulatedStackActor[index][i]=vtkActor::New();
	      triangulatedStackActor[index][i]->SetMapper(map);
	      if (display_mode[i]!=0)
		triangulatedStackActor[index][i]->SetVisibility(1);
	      else
		triangulatedStackActor[index][i]->SetVisibility(0);

	      if (display_mode[i]==1 || display_mode[i]==2)
		{
		  triangulatedStackActor[index][i]->GetProperty()->SetLineWidth(2.0);
		  triangulatedStackActor[index][i]->GetProperty()->SetAmbient(1.0);
		  triangulatedStackActor[index][i]->GetProperty()->SetDiffuse(0.0);
		  triangulatedStackActor[index][i]->GetProperty()->SetSpecular(0.0);
		  if (display_mode[i]==2)
		    triangulatedStackActor[index][i]->GetProperty()->SetRepresentationToWireframe();
		  else
		    triangulatedStackActor[index][i]->GetProperty()->SetRepresentationToPoints();
		}
	      
	      map->Delete();
	    }
	ren->AddActor(triangulatedStackActor[index][i]);
      }

  this->Renderer[index]=ren;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUITriangulatedStackControl::Update()
{
  if (this->triangulatedStackActor[0]==NULL)
      return;

  for (int j=0;j<TSP_MAX_RENDERERS;j++)
    {
      if (this->Renderer[j]!=NULL)
	{
	  for (int i=0;i<this->NumberOfTriangulatedStacks;i++)
	    {
	      if (j==0)
		{
		  if (triangulatedStackSet[i]->GetColorMode()!=color_mode[i])
		    triangulatedStackSet[i]->SetColorMode(color_mode[i]);
		}

	      if (display_mode[i]==0)
		{
		  triangulatedStackActor[j][i]->SetVisibility(0);
		}
	      else
		{
		  vtkProperty* prop=triangulatedStackActor[j][i]->GetProperty();
		  switch (display_mode[i])
		    {
		    case 3:
		      prop->SetRepresentationToSurface();
		      prop->SetDiffuse(1.0);
		      prop->SetAmbient(0.0);
		      prop->SetSpecular(0.0);
		      break;
		    case 2:
		      prop->SetRepresentationToWireframe();
		      prop->SetDiffuse(0.0);
		      prop->SetAmbient(1.0);
		      prop->SetSpecular(0.0);
		      break;
		    case 1:
		      prop->SetRepresentationToPoints();
		      prop->SetDiffuse(0.0);
		      prop->SetAmbient(1.0);
		      prop->SetSpecular(0.0);
		      prop->SetPointSize(3.0);		
		      break;
		    }
		  triangulatedStackActor[j][i]->SetVisibility(1);
		}
	    }
	}
    }
  
  // Curve Stuff 
  UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUITriangulatedStackControl::UpdateDisplay()
{
  if (this->Renderer[0]!=NULL)
    this->Renderer[0]->GetRenderWindow()->Render();

}
/* -------------------------------------------------------------------------*/

int  vtkpxGUITriangulatedStackControl::HandleEvent(int event)
{
  if (!this->Initialized)
      return TCL_OK;

  if (event<10)
      return this->EventManager->handleDirectEvent(event);

  if (this->currentTriangulatedStack==NULL)
    return TCL_OK;
  
  switch(event)
      {
      case 401:
	{
	  int ok=this->EventManager->questionBox("Loading will delete your old triangulatedstack. Are you sure?");
	  if (ok)
	      {
		char* line=this->EventManager->getOpenFilename("Load triangulatedstacks","TriangulatedStacks","*.tstack");
		if (line!=NULL)
		    {
		      if (strlen(line)>0)
			LoadTriangulatedStack(line);
		    }
	      }
	}
	break;
	
      case 402:
	{
	  char* line=this->EventManager->getSaveFilename("Save TriangulatedStacks","surface.tstack","TriangulatedStacks","*.tstack");
	  if (line!=NULL)
	      {
		if (strlen(line)>0)
		    {
		      strcpy(pxtk_buffer2,line);
		      if(currentTriangulatedStack->Save(line))
			{
			  sprintf(pxtk_buffer2,"%s.%d",PXTkApp::getFilenameTail(pxtk_buffer2),current_triangulatedstack+1);
			  currentCollection->changeLabel(current_triangulatedstack,pxtk_buffer2);
			  
			}
		      else
			{
			  sprintf(pxtk_buffer2,"Failed to save triangulatedstack %d in %s",current_triangulatedstack+1,line);
			  this->EventManager->messageBox(pxtk_buffer2,"Save Failure");
			}
		    }
	      }
	}
	break;
  

	
      case 450:
	if (triangulatedStackActor[0][current_triangulatedstack]!=NULL)
	  {
	    double c[3];
	    triangulatedStackActor[0][current_triangulatedstack]->GetProperty()->GetColor(c);
	    int a=this->EventManager->d_colorBox("TriangulatedStack Color",c[0],c[1],c[2]);
	    if (a==1)
	      {
		for (int j=0;j<TSP_MAX_RENDERERS;j++)
		  {
		    if (triangulatedStackActor[j][current_triangulatedstack]!=NULL)
		      triangulatedStackActor[j][current_triangulatedstack]->GetProperty()->SetColor(c);
		  }
	      }
	  }
	break;
	

      case 452: // Ellipse
	{
	  PXContourStack* cstack=new PXContourStack(this->dimensions[2]);
	  cstack->setellipticalcylinder(8,
					spacing[0]*float(dimensions[0])/8.0*(1.0+current_triangulatedstack),
					spacing[1]*float(dimensions[1])/8.0*(1.0+current_triangulatedstack),
					origin[0]+spacing[0]*dimensions[0]/2.0,
					origin[1]+spacing[1]*dimensions[1]/2.0);
	  currentTriangulatedStack->SetFromContourStack(cstack);
	  delete cstack;
	}
	break;
	  
      case 500:
	SetCurrentCollection(currentCollection->getIndex());
	break;
	
      case 501:
	display_mode[current_triangulatedstack]=displayMode->getIndex();
	color_mode[current_triangulatedstack]=colorMode->getIndex()-1;
	Update();
	break;

	
      case 505:
	{
	  for  (int i=0;i<this->NumberOfTriangulatedStacks;i++)
	    display_mode[i]=0;
	  displayMode->setIndex(0);
	}
	Update();
	break;

	
      case 506:
	{
	  for  (int i=0;i<this->NumberOfTriangulatedStacks;i++)
	    display_mode[i]=2;
	  displayMode->setIndex(3);
	}
	Update();
	break;
	
      case 590:
	this->CopyTriangulatedStack();
	break;
	
      case 591:
	this->PasteTriangulatedStack();
	break;


      case 614:
	if (this->EventParent!=NULL && this->CallbackNo>0)
	  this->EventParent->HandleEvent(CallbackNo);
	else
	  if (this->CallbackName!=NULL)
	    PXTkApp::executeTclCommand(this->CallbackName);
	break;

      }

  UpdateDisplay();

  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUITriangulatedStackControl::CopyTriangulatedStack()
{
  clipboardTriangulatedStack->Copy(currentTriangulatedStack);
}

void vtkpxGUITriangulatedStackControl::PasteTriangulatedStack()	
{
    SetTriangulatedStack(clipboardTriangulatedStack,-1);
}
/* -------------------------------------------------------------------------*/
vtkpxTriangulatedStackSource* vtkpxGUITriangulatedStackControl::GetTriangulatedStack(int index)
{
  if (index==-1)
      index=current_triangulatedstack;

  return triangulatedStackSet[Irange(index,0,this->NumberOfTriangulatedStacks-1)];
}
/* -------------------------------------------------------------------------*/
int vtkpxGUITriangulatedStackControl::GetCurrentIndex()
{
  return current_triangulatedstack;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUITriangulatedStackControl::SetTriangulatedStack(vtkpxTriangulatedStackSource* land,int index)
{
  if (index==-1)
      index=current_triangulatedstack;

  index=Irange(index,0,this->NumberOfTriangulatedStacks-1);

  triangulatedStackSet[index]->Copy(land);
 
}
/* -------------------------------------------------------------------------*/
void vtkpxGUITriangulatedStackControl::SetImage(vtkImageData* img,int orient,int lv,int fr)
{
  if (img!=NULL)
    {
      if (this->image!=NULL)
	this->image->Delete();
      this->image=vtkImageData::New();
      this->image->ShallowCopy(img);
      this->orientation=Irange(orient,0,3);
      
      int d[3]; double sp[3],ori[3];
      this->image->GetSpacing(sp);
      this->image->GetOrigin(ori);
      this->image->GetDimensions(d);
      
      float sum=0.0;
      for (int ia=0;ia<=2;ia++)
	sum+=
	  fabs(float(d[ia]-this->dimensions[ia]))+
	  fabs(sp[ia]-this->spacing[ia])+
	  fabs(ori[ia]-this->origin[ia]);
      
      
      if (sum>0.01)
	{
	  this->image->GetOrigin(this->origin);
	  this->image->GetSpacing(this->spacing);
	  this->image->GetDimensions(this->dimensions);
	}
    }
}

/* -------------------------------------------------------------------------*/
void vtkpxGUITriangulatedStackControl::LoadTriangulatedStack(const char* fname)
{
  strcpy(pxtk_buffer2,fname);
  int ok=currentTriangulatedStack->Load(fname);
  if (!ok)
      {
	sprintf(pxtk_buffer2,"Failed to load landmarks %d from %s",current_triangulatedstack+1,fname);
	this->EventManager->messageBox(pxtk_buffer2,"Load Failure");
      }
  else
    {
      sprintf(pxtk_buffer2,"%s.%d",PXTkApp::getFilenameTail(pxtk_buffer2),current_triangulatedstack+1);
      currentCollection->changeLabel(current_triangulatedstack,pxtk_buffer2);
    }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUITriangulatedStackControl::SetCurrentCollection(int num)
{
  
  if (num==current_triangulatedstack)
      return;
  current_triangulatedstack=num;

  currentTriangulatedStack=triangulatedStackSet[current_triangulatedstack];  
  displayMode->setIndex(display_mode[current_triangulatedstack]);
  colorMode->setIndex(color_mode[current_triangulatedstack]+1);
  Update();
}
/* -------------------------------------------------------------------------*/

