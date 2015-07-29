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





// 	$Id: vtkpxGUI4DTriangulatedStackControl.cpp,v 1.1 2002/06/07 19:40:45 papad Exp papad $	

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkpxGUI4DTriangulatedStackControl.h"
#include "vtkpxUtil.h"
#include "vtkActor.h"
#include "vtkAssembly.h"
#include "vtkRenderer.h"
#include "vtkObjectFactory.h"
#include "vtkpxImageExtract.h"
#include "vtkPolyDataMapper.h"
#include "vtkImageData.h"
#include "vtkpxGUI4DRenderer.h"
#include "vtkCollection.h"
#include "vtkActorCollection.h"
#include "vtkpxShapeTracking.h"
#include "vtkProperty.h"
#include "pxcontourstack.h"
/* -------------------------------------------------------------------------*/
vtkpxGUI4DTriangulatedStackControl* vtkpxGUI4DTriangulatedStackControl::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUI4DTriangulatedStackControl");
  if(ret)
      {
	return (vtkpxGUI4DTriangulatedStackControl*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUI4DTriangulatedStackControl;
}

// ----------------------------------------------------------------------------
vtkpxGUI4DTriangulatedStackControl::vtkpxGUI4DTriangulatedStackControl()
{
  for (int j=0;j<TSP_MAX_RENDERERS;j++)
    this->MultiRenderer[j]=NULL;
  
  this->CurrentFrame=0;
  this->NumberOfFrames=0;
  this->ActualNumberOfFrames=0;
  this->triangulatedStackCollection=NULL;
  this->triangulatedStackActorCollection=NULL;
  
  for (int ia=0;ia<TSP_MAX_COLLECTIONS;ia++)
    {
      surface_filename[ia]=new char[20];
      sprintf(surface_filename[ia],"unnamed_%d",ia+1);
    }
}
// ----------------------------------------------------------------------------
vtkpxGUI4DTriangulatedStackControl::~vtkpxGUI4DTriangulatedStackControl()
{
  
  if (this->triangulatedStackCollection!=NULL)
    this->triangulatedStackCollection->Delete();

  if (this->triangulatedStackActorCollection==NULL)
    this->triangulatedStackActorCollection->Delete();

  // Set to NULL to avoid deleting by parent
  for (int i=0;i<this->NumberOfTriangulatedStacks;i++)
    {
      this->triangulatedStackSet[i]=NULL;
      for (int j=0;j<TSP_MAX_RENDERERS;j++)
	this->triangulatedStackActor[j][i]=NULL;
    }
    
  for (int ia=0;ia<TSP_MAX_COLLECTIONS;ia++)
    delete [] surface_filename[ia];

}
// ----------------------------------------------------------------------------
void vtkpxGUI4DTriangulatedStackControl::InitializeTriangulatedStacks()
{
  this->triangulatedStackCollection=vtkCollection::New();
  this->triangulatedStackActorCollection=vtkActorCollection::New();

  current_triangulatedstack=0;
  clipboardTriangulatedStack=vtkpxTriangulatedStackSource::New();

}
// ----------------------------------------------------------------------------
void vtkpxGUI4DTriangulatedStackControl::ResetTriangulatedStacks()
{
  //  fprintf(stderr,"In Reset Triangulated Stacks");

  int d[3]; float sp[3],ori[3];
  for (int ib=0;ib<=2;ib++)
    {
      d[ib]=this->dimensions[ib];
      sp[ib]=this->spacing[ib];
      ori[ib]=this->origin[ib];
    }
  
  int oldnumframes=this->NumberOfFrames;

  this->NumberOfFrames=this->image->GetNumberOfScalarComponents();
  this->image->GetSpacing(this->spacing);
  this->image->GetOrigin(this->origin);
  this->image->GetDimensions(this->dimensions);

  this->CreateFrames(this->NumberOfFrames);
  for (int i=1;i<this->NumberOfFrames;i++)
    this->UpdateFrame(i);
  this->UpdateFrame(0);


  float sum=0.0;
  for (int ia=0;ia<=2;ia++)
    sum+=
      fabs(float(d[ia]-this->dimensions[ia]))+
      fabs(sp[ia]-this->spacing[ia])+
      fabs(ori[ia]-this->origin[ia]);
  
  if (sum>0.01)
    {
      if (oldnumframes>0)
	{
	  int index=0;
	  
	  for (int f=0;f<oldnumframes;f++)
	    for (int i=0;i<this->NumberOfTriangulatedStacks;i++)
	      {
		vtkpxTriangulatedStackSource *st=(vtkpxTriangulatedStackSource*)this->triangulatedStackCollection->GetItemAsObject(index);
		++index;
		PXContourStack* cstack=new PXContourStack(this->dimensions[2]);
		cstack->setellipticalcylinder(8,
					      spacing[0]*float(dimensions[0])/8.0*(1.0+i),
					      spacing[1]*float(dimensions[1])/8.0*(1.0+i),
					      origin[0]+spacing[0]*dimensions[0]/2.0,
					      origin[1]+spacing[1]*dimensions[1]/2.0);
		st->SetFromContourStack(cstack);
		delete cstack;//fprintf(stderr,"Setting Elliptical Cylinder f=%d,i=%d\n",f,i);
	      }
	}
    }
}

// ----------------------------------------------------------------------------
void vtkpxGUI4DTriangulatedStackControl::CreateFrames(int numframes)
{

  if (numframes<this->ActualNumberOfFrames)
    return;

  if (numframes>this->MultiRenderer[0]->GetNumberOfFrames())
    numframes=this->MultiRenderer[0]->GetNumberOfFrames();
  if (numframes<1)
    numframes=1;


  for (int fr=this->ActualNumberOfFrames;fr<numframes;fr++)
    {
      for (int i=0;i<this->NumberOfTriangulatedStacks;i++)
	{
	  triangulatedStackSet[i]=vtkpxTriangulatedStackSource::New();
	  triangulatedStackSet[i]->SetColorMode(color_mode[i]);
	  
	  //	  fprintf(stderr,"Create Frames:: Setting Elliptical Cylinder f=%d,i=%d\n",fr,i);
	  PXContourStack* cstack=new PXContourStack(this->dimensions[2]);
	  cstack->setellipticalcylinder(8,
					spacing[0]*float(dimensions[0])/8.0*(1.0+i),
					spacing[1]*float(dimensions[1])/8.0*(1.0+i),
					origin[0]+spacing[0]*dimensions[0]/2.0,
					origin[1]+spacing[1]*dimensions[1]/2.0);
	  triangulatedStackSet[i]->SetFromContourStack(cstack);
	  delete cstack;//fprintf(stderr,"Setting Elliptical Cylinder f=%d,i=%d\n",f,i);
	  this->triangulatedStackCollection->AddItem(triangulatedStackSet[i]);
	  
	  for (int r=0;r<TSP_MAX_RENDERERS;r++)
	    {
	      triangulatedStackActor[r][i]=vtkActor::New();
	      this->triangulatedStackActorCollection->AddItem(triangulatedStackActor[r][i]);
	      
	      if (this->MultiRenderer[r]!=NULL)
		{
		  vtkPolyDataMapper* map=vtkPolyDataMapper::New();
		  map->SetInput(triangulatedStackSet[i]->GetOutput());
		  
		  triangulatedStackActor[r][i]->SetMapper(map);
		  if (display_mode[i]!=0)
		    triangulatedStackActor[r][i]->SetVisibility(1);
		  else
		    triangulatedStackActor[r][i]->SetVisibility(0);
		  
		  if (wireframemode[r]==1)
		    {
		      triangulatedStackActor[r][i]->GetProperty()->SetRepresentationToWireframe();
		      triangulatedStackActor[r][i]->GetProperty()->SetLineWidth(2.0);
		      triangulatedStackActor[r][i]->GetProperty()->SetAmbient(1.0);
		      triangulatedStackActor[r][i]->GetProperty()->SetDiffuse(0.0);
		      triangulatedStackActor[r][i]->GetProperty()->SetSpecular(0.0);
		    }
		  map->Delete();
		  this->MultiRenderer[r]->GetRendererForFrame(fr)->AddActor(triangulatedStackActor[r][i]);
		}
	    }
	}
      if (fr==0)
	{
	  current_triangulatedstack=0;
	  currentTriangulatedStack=triangulatedStackSet[current_triangulatedstack];
	}
    }

  this->ActualNumberOfFrames=numframes;

  
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DTriangulatedStackControl::UpdateFrame(int frame)
{
  //  fprintf(stderr,"Updating Frame %d\n",frame);

  if (frame>=this->ActualNumberOfFrames)
    return;
  
  // Image Slice Ortho Slice First 
  int index=frame*this->NumberOfTriangulatedStacks;
  int index2=index*TSP_MAX_RENDERERS;

  //  fprintf(stderr,"Index=%d index2=%d\n",index,index2);
  for (int i=0;i<this->NumberOfTriangulatedStacks;i++)
    {
      triangulatedStackSet[i]=(vtkpxTriangulatedStackSource*)this->triangulatedStackCollection->GetItemAsObject(index);
      ++index;
      for (int r=0;r<TSP_MAX_RENDERERS;r++)
	{
	  triangulatedStackActor[r][i]=(vtkActor*)this->triangulatedStackActorCollection->GetItemAsObject(index2);
	  ++index2;
	}
    }

  //fprintf(stderr,"Final Index=%d index2=%d\n",index,index2);

  for (int r=0;r<TSP_MAX_RENDERERS;r++)
    {
      if (this->MultiRenderer[r]!=NULL)
	this->Renderer[r]=this->MultiRenderer[r]->GetRendererForFrame(frame);
    }

  //  fprintf(stderr,"Done Updating %d\n",frame);

}
// -------------------------------------------------------------------------
void vtkpxGUI4DTriangulatedStackControl::ChangeFrame(int newframe)
{
  newframe=Irange(newframe,0,this->NumberOfFrames-1);
  if (newframe==this->CurrentFrame)
    return;

  this->UpdateFrame(newframe); 
  this->currentTriangulatedStack=triangulatedStackSet[current_triangulatedstack];
  this->CurrentFrame=newframe;
  this->Update();
}
// -------------------------------------------------------------------------

char* vtkpxGUI4DTriangulatedStackControl::Initialize(const char* name,int inside)
{
  return this->Initialize(name,name,inside);
}

char* vtkpxGUI4DTriangulatedStackControl::Initialize(const char* name,const char* name2,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();

  vtkpxGUITriangulatedStackControl::Initialize(name,inside);

  PXTkLabelFrame* mFrame=NULL;
  if (strcmp(name,name2)==0)
    {
      mFrame=new PXTkLabelFrame(this->EventManager,this->controlFrame,"Multi Frame");
      this->controlFrame->addChildren("-side top -expand false -fill x -padx 1 -pady 5",mFrame);
    }
  else
    {
      PXTkFrame* m=new PXTkFrame(NULL,name2);
      mFrame=new PXTkLabelFrame(this->EventManager,m,"Multi Frame");
      m->addChildren("-side top -expand false -fill x -padx 1 -pady 5",mFrame);
    }
  mFrame->setPackMode(PXTrue);

   PXTkFrame* f2=new PXTkFrame(this->EventManager,mFrame);
  PXTkFrame* f4=new PXTkFrame(this->EventManager,mFrame);
  mFrame->addChildren("-side top -expand false -fill x -pady 0 ",f2,f4);

     
  PXTkButton* b0a=new PXTkButton(this->EventManager,f2,"Load All",801);
  PXTkButton* b0b=new PXTkButton(this->EventManager,f2,"Save All",802);
  f2->addChildren("-side left -expand f -fill x -pady 1 -padx 2",b0a,b0b);

  f4->setPackMode(PXTrue);

  PXTkButton* but1=new PXTkButton(this->EventManager,f4,"Curvatures",810);
  PXTkButton* but2=new PXTkButton(this->EventManager,f4,"Shape Tracking",812);
  f4->addChildren("-side left -expand f -fill x -pady 0",but1,but2);
  return GetWidgetName();
}

/* -------------------------------------------------------------------------*/ 
void vtkpxGUI4DTriangulatedStackControl::SetRenderer(vtkRenderer* ren,int index)
{
  vtkErrorMacro(<<"Cannot Use this Function in vtkpxGUI4DTriangulatedStackControl");
}

void vtkpxGUI4DTriangulatedStackControl::SetRenderer(vtkpxGUI4DRenderer* ren,int index)
{
  this->SetRenderer(ren,index,0);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DTriangulatedStackControl::SetRenderer(vtkRenderer *ren,int index,int wireframe)
{
  vtkErrorMacro(<<"Cannot Use this Function in vtkpxGUI4DTriangulatedStackControl");
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DTriangulatedStackControl::SetRenderer(vtkpxGUI4DRenderer *ren,int index,int wireframe)
{
  index=Irange(index,0,TSP_MAX_RENDERERS-1);

  if (this->MultiRenderer[index]!=NULL)
    return;

  this->MultiRenderer[index]=ren;
  this->wireframemode[index]=wireframe;
  this->Renderer[index]=this->MultiRenderer[index]->GetRendererForFrame(this->CurrentFrame);

}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DTriangulatedStackControl::Update()
{
  if (this->triangulatedStackActor[0]==NULL)
      return;

  
  for (int frame=0;frame<this->NumberOfFrames;frame++)
    {
      int index=frame*this->NumberOfTriangulatedStacks;
      int index2=index*TSP_MAX_RENDERERS;
      //  fprintf(stderr,"Index=%d index2=%d\n",index,index2);
      for (int i=0;i<this->NumberOfTriangulatedStacks;i++)
	{
	  vtkpxTriangulatedStackSource *stack2=this->GetTriangulatedStack(frame,i);
	  if (stack2->GetColorMode()!=color_mode[i])
	    stack2->SetColorMode(color_mode[i]);

	  for (int r=0;r<TSP_MAX_RENDERERS;r++)
	    {
	      vtkActor* act=(vtkActor*)this->triangulatedStackActorCollection->GetItemAsObject(index2);
	      ++index2;
	      
	      if (display_mode[i]==0)
		{
		  act->SetVisibility(0);
		}
	      else
		{
		  vtkProperty* prop=act->GetProperty();
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
		  act->SetVisibility(1);
		}
	    }
	}
    }
  // Curve Stuff 
  UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DTriangulatedStackControl::UpdateDisplay()
{
  vtkpxGUITriangulatedStackControl::UpdateDisplay();
}
/* -------------------------------------------------------------------------*/

int  vtkpxGUI4DTriangulatedStackControl::HandleEvent(int event)
{
  if (!this->Initialized)
      return TCL_OK;

  if (event<10)
      return this->EventManager->handleDirectEvent(event);

  if (event<800)
    return vtkpxGUITriangulatedStackControl::HandleEvent(event);



  switch (event)
    {
    case 801:
      if (this->NumberOfFrames>0)
	{
	  char* line=this->EventManager->getOpenFilename("Load series","Series","*.01.tstack");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		this->MultiLoad(line);
	    }
	}
      break;
    case 802:
      if (this->NumberOfFrames>0)
	{
	  sprintf(pxtk_buffer,"%s.01.tstack",surface_filename[current_triangulatedstack]);
	  char* line=this->EventManager->getSaveFilename("Save series",pxtk_buffer,"Series","*.01.tstack");
	  if (line!=NULL)
	    {
	      this->MultiSave(line);
	    }
	}
      break;

    case 810:
      this->DoCurvatures(0,850);
      break;

    case 850:
      this->DoCurvatures(1);
      break;

    case 812:
      this->DoShapeTracking(0,860);
      break;

    case 860:
      this->DoShapeTracking(1);
      break;

    }
  
  
  this->UpdateDisplay();

  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DTriangulatedStackControl::SetImage(vtkImageData* img,int orient,int level,int frame)
{
  if (img!=NULL)  this->SetImage(img);
  this->orientation=Irange(orient,0,3);
  this->SetCurrentFrame(frame);
  
}

void vtkpxGUI4DTriangulatedStackControl::SetImage(vtkImageData* img)
{
  if (img==NULL)
    return;

  if (this->image!=NULL)
    this->image->Delete();
  this->image=vtkImageData::New();
  this->image->ShallowCopy(img);      
  
  this->ResetTriangulatedStacks();

}

/* -------------------------------------------------------------------------*/
int vtkpxGUI4DTriangulatedStackControl::MultiLoad(const char* fname)
{
  strcpy(pxtk_buffer2,fname);

  char* noext1=nothisextension(fname,".tstack");
  char* noext2=nothisextension(noext1,".01");
  int sum=0;

  for (int frame=0;frame<this->NumberOfFrames;frame++)
    {
      sprintf(pxtk_buffer3,"%s.%02d.tstack",noext2,frame+1);
      vtkpxTriangulatedStackSource *stack=this->GetTriangulatedStack(frame,current_triangulatedstack);
      int ok=stack->Load(pxtk_buffer3);
      stack->Modified();
      sum+=(1-ok);
      sprintf(pxtk_buffer,"Loading from %s (status=%d,current=%d)\n",pxtk_buffer3,ok,current_triangulatedstack);
      PXTkApp::printToConsole(pxtk_buffer);
    }

  if (sum<this->NumberOfFrames)
    {
      delete [] surface_filename[current_triangulatedstack];
      surface_filename[current_triangulatedstack]=new char[strlen(noext2)+1];
      strcpy(surface_filename[current_triangulatedstack],noext2);
    }

  if (sum>0)
    PXTkApp::popupConsole();
      
  delete [] noext2;
  delete [] noext1;
  this->UpdateAfterExternalEditing();
  return 1;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DTriangulatedStackControl::UpdateAfterExternalEditing()
{
  this->UpdateFrame(this->CurrentFrame);

}
/* -------------------------------------------------------------------------*/
int vtkpxGUI4DTriangulatedStackControl::MultiSave(const char* fname)
{
  char* noext1=nothisextension(fname,".tstack");
  char* noext2=nothisextension(noext1,".01");
  int sum=0;



  for (int frame=0;frame<this->NumberOfFrames;frame++)
    {
      sprintf(pxtk_buffer3,"%s.%02d.tstack",noext2,frame+1);
      vtkpxTriangulatedStackSource *stack=this->GetTriangulatedStack(frame,current_triangulatedstack);
      int ok=stack->Save(pxtk_buffer3);
      sum+=(1-ok);
      sprintf(pxtk_buffer,"Saving to %s (status=%d)\n",pxtk_buffer3,ok);
      PXTkApp::printToConsole(pxtk_buffer);
    }

  if (sum>0)
    PXTkApp::popupConsole();

  if (sum<this->NumberOfFrames)
    {
      delete [] surface_filename[current_triangulatedstack];
      surface_filename[current_triangulatedstack]=new char[strlen(noext2)+1];
      strcpy(surface_filename[current_triangulatedstack],noext2);
    }

      
  delete [] noext2;
  delete [] noext1;

  this->UpdateAfterExternalEditing();
  return 1;
}
/* -------------------------------------------------------------------------*/
vtkpxTriangulatedStackSource* vtkpxGUI4DTriangulatedStackControl::GetTriangulatedStack(int frame,int index)
{
  if (this->triangulatedStackCollection==NULL)
    return NULL;

  frame=Irange(frame,0,this->NumberOfFrames-1);
  index=Irange(index,0,this->NumberOfTriangulatedStacks);

  int ind=frame*this->NumberOfTriangulatedStacks+index;
  return (vtkpxTriangulatedStackSource*)this->triangulatedStackCollection->GetItemAsObject(ind);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DTriangulatedStackControl::SetCurrentFrame(int t)
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
int vtkpxGUI4DTriangulatedStackControl::GetCurrentFrame()
{
  return this->CurrentFrame;
}

int vtkpxGUI4DTriangulatedStackControl::GetNumberOfFrames()
{
  return this->NumberOfFrames;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DTriangulatedStackControl::DoCurvatures(int mode,int moption)
{
  if (mode==0)
    {
      if (this->optionDialog->isOpen())
	return;

      delete this->optionDialog;
      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,2);
      this->optionDialog->initDisplay();

      this->optionDialog->setTitleDescription("Computing Curvatures",
					      "Enter Parameters for Curvature Computation");
      this->optionDialog->setioption(0,"Scale","%d",4);
      this->optionDialog->setioption(1,"Smooth Iterations","%d",50);
      this->optionDialog->setMenuOption(moption);
      this->optionDialog->popupDialog();
    }

  if (mode==1)
    {
      int scale=Irange(this->optionDialog->getioption(0)  ,2,6);
      int iterations=Irange(this->optionDialog->getioption(1),0,100);

      PXTkApp::popupConsole();
      for (int frame=0;frame<this->NumberOfFrames;frame++)
	{
	  vtkpxTriangulatedStackSource *stack=this->GetTriangulatedStack(frame,current_triangulatedstack);
	  if (iterations>0)
	    stack->NonShrinkSmooth(0.33,0.34,iterations);
	  stack->ComputeCurvatures(scale);
	  stack->Update();
	  sprintf(pxtk_buffer3,"Compute Curvatures for frame %d (%d,%d)\n",frame+1,scale,iterations);
	  PXTkApp::printToConsole(pxtk_buffer3);
	}
      
      this->UpdateAfterExternalEditing();
    }
      
      
}

/* -------------------------------------------------------------------------*/
void vtkpxGUI4DTriangulatedStackControl::DoShapeTracking(int mode,int moption)
{
  if (mode==0)
    {
      if (this->optionDialog->isOpen())
	return;

      delete this->optionDialog;
      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,4);
      this->optionDialog->initDisplay();

      this->optionDialog->setTitleDescription("Shape-based Tracking",
					      "Enter  Parameters for Shape Tracking");
      this->optionDialog->setioption(0,"Start Frame","%d",1);
      this->optionDialog->setioption(1,"Stop  Frame","%d",9);
      this->optionDialog->setioption(2,"Search Window","%d",3);
      this->optionDialog->setioption(3,"Surface Subdivision","%d",2);
      
      this->optionDialog->setMenuOption(moption);
      this->optionDialog->popupDialog();
    }

  if (mode==1)
    {

      sprintf(pxtk_buffer,"%s_01_02_init_sm1.match",surface_filename[current_triangulatedstack]);
      char* fname=this->EventManager->getSaveFilename("Save tracking results",pxtk_buffer,"Shape tracking results","*_01_02_init_sm1.match");
      if (fname==NULL)
	return;

      char* first=strstr(fname,"_01_02_init_sm1.match");
      if (first!=NULL)
	strncpy(pxtk_buffer4,fname,(first-fname));
      else
	strcpy(pxtk_buffer4,fname);

      int bframe=Irange(this->optionDialog->getioption(0)  ,0,this->NumberOfFrames-1);
      int eframe=Irange(this->optionDialog->getioption(1)  ,0,this->NumberOfFrames-1);
      int searchwindow=Irange(this->optionDialog->getioption(2),1,10);
      int subdivision=Irange(this->optionDialog->getioption(3),0,3);

      PXTkApp::popupConsole();

      if (eframe<bframe)
	eframe+=this->NumberOfFrames;

      for (int frame=bframe;frame<eframe;frame++)
	{
	  vtkpxTriangulatedStackSource *stack=this->GetTriangulatedStack(frame,current_triangulatedstack);
	  int nextf=frame+1;
	  if (nextf>this->NumberOfFrames)
	    nextf-=this->NumberOfFrames;
	  vtkpxTriangulatedStackSource *stack2=this->GetTriangulatedStack(nextf,current_triangulatedstack);

	  sprintf(pxtk_buffer2,"%s_%02d_%02d_init_sm1.match",pxtk_buffer4,frame,frame+1);
	  int ok=vtkpxShapeTracking::OldStyleShapeTracking(stack,stack2,pxtk_buffer2,
							   searchwindow,subdivision);
	  sprintf(pxtk_buffer3,"Saved results for frame %d in %s (%d)\n",frame+1,pxtk_buffer2,ok);
	  PXTkApp::printToConsole(pxtk_buffer3);
	}
    }
}

