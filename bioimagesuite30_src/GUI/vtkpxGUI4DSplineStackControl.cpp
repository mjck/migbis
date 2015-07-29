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



// 	$Id: vtkpxGUI4DSplineStackControl.cpp,v 1.2 2003/09/12 12:59:32 xenios Exp xenios $	

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkpxGUI4DSplineStackControl.h"
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
#include "vtkPolyDataWriter.h"
#include "vtkpxTriangulatedStackSource.h"
#include "pxchamferdistancemap.h"
#include "pxfpbspline.h"
#include "pxcontour.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkpxGUIBaseImageViewer.h"
/* -------------------------------------------------------------------------*/
vtkpxGUI4DSplineStackControl* vtkpxGUI4DSplineStackControl::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUI4DSplineStackControl");
  if(ret)
      {
	return (vtkpxGUI4DSplineStackControl*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUI4DSplineStackControl;
}

// ----------------------------------------------------------------------------
vtkpxGUI4DSplineStackControl::vtkpxGUI4DSplineStackControl()
{
  for (int j=0;j<SSP_MAX_RENDERERS;j++)
    this->MultiRenderer[j]=NULL;
  
  this->CurrentFrame=0;
  this->NumberOfFrames=0;
  this->ActualNumberOfFrames=0;
  this->splineStackCollection=NULL;
  this->splineStackActorCollection=NULL;
  this->do_autosave=PXTrue;
  this->do_backup=PXTrue;
  this->BaseViewer=NULL;
  
  this->dimensions[0]=-1;
  this->AbortProcessing=-1;
  this->optionDialog=NULL;
}
// ----------------------------------------------------------------------------
vtkpxGUI4DSplineStackControl::~vtkpxGUI4DSplineStackControl()
{
  this->SetBaseViewer(NULL);

  if (this->splineStackCollection!=NULL)
    this->splineStackCollection->Delete();

  if (this->splineStackActorCollection==NULL)
    this->splineStackActorCollection->Delete();

  // Set to NULL to avoid deleting by parent
  for (int i=0;i<this->NumberOfSplineStacks;i++)
    {
      this->splineStackSet[i]=NULL;
      for (int j=0;j<SSP_MAX_RENDERERS;j++)
	this->splineStackActor[j][i]=NULL;
    }
    
}
// ----------------------------------------------------------------------------
void vtkpxGUI4DSplineStackControl::InitializeSplineStacks()
{
  this->splineStackCollection=vtkCollection::New();
  this->splineStackActorCollection=vtkActorCollection::New();

  current_splinestack=0;
  clipboardSplineStack=vtkpxSplineStackSource::New();

}
// ----------------------------------------------------------------------------
void vtkpxGUI4DSplineStackControl::ResetSplineStacks()
{

  int d[3]; double sp[3],ori[3];
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

  //  fprintf(stderr,"Reset Spline Stacks this->Dim=%d,%d,%d\n",this->dimensions[0],this->dimensions[1],this->dimensions[2]);

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

  vtkpxSplineStackSource *stc=(vtkpxSplineStackSource*)this->splineStackCollection->GetItemAsObject(0);

  if ( (stc->GetPolarMode()==0 && this->orientation==3) || (stc->GetPolarMode()==1 && this->orientation!=3))
       sum=1.0;

  if (sum>0.01)
    {
      float dz=spacing[2];
      float factor=1.0;
      if (this->orientation==3)
	dz=180.0/float(this->dimensions[2]);

	    
      if (oldnumframes>0)
	{
	  int index=0;
	  
	  for (int f=0;f<oldnumframes;f++)
	    for (int i=0;i<this->NumberOfSplineStacks;i++)
	      {
		vtkpxSplineStackSource *st=(vtkpxSplineStackSource*)this->splineStackCollection->GetItemAsObject(index);
		++index;
		st->SetEllipticalCylinder(8,
					  this->dimensions[2],
					  factor*spacing[0]*dimensions[0]/8.0*(1.0+i),
					  factor*spacing[1]*dimensions[1]/8.0*(1.0+i),
					  origin[0]+spacing[0]*dimensions[0]/2.0,
					  origin[1]+spacing[1]*dimensions[1]/2.0,
					  dz,
					  this->origin[2]);
		st->SetDrawMode(0);

		if (this->orientation==3)
		  {
		    st->SetPolarMode(1);
		    st->SetPolarMiddle(spacing[0]*dimensions[0]*0.5+origin[0]);
		  }
		else
		  {
		    st->SetPolarMode(0);
		    if (dimensions[2]>12)
		      {
			st->SetTrimBottom(dimensions[2]/2-5);
			st->SetTrimTop(dimensions[2]/2+5);
		      }
		    else
		      {
			st->SetTrimTop(dimensions[2]-1);
			st->SetTrimBottom(0);
		      }
		  }
		
		st->SetResolutionU(2.0*spacing[1]);
		st->SetResolutionV(spacing[2]);
	      }
	}
  
      // Get Current Spline Stack then set trims
      trimScale[0]->setRange(0,this->dimensions[2]-1);
      trimScale[1]->setRange(0,this->dimensions[2]-1);
      trimScale[0]->setValue(this->currentSplineStack->GetTrimBottom());
      trimScale[1]->setValue(this->currentSplineStack->GetTrimTop());
      this->UpdateSplineEditor();
    }

}

// ----------------------------------------------------------------------------
void vtkpxGUI4DSplineStackControl::CreateFrames(int numframes)
{

  if (numframes<this->ActualNumberOfFrames)
    return;

  if (numframes>this->MultiRenderer[0]->GetNumberOfFrames())
    numframes=this->MultiRenderer[0]->GetNumberOfFrames();
  if (numframes<1)
    numframes=1;

  float dz=spacing[2];
  float factor=1.0;
  float cx = origin[0]+spacing[0]*dimensions[0]/2.0;
  float cy = origin[1]+spacing[1]*dimensions[1]/2.0;
  if (this->orientation==3)
    {
      dz=180.0/float(this->dimensions[2]);
      cy=cx;
    }

  for (int fr=this->ActualNumberOfFrames;fr<numframes;fr++)
    {
      for (int i=0;i<this->NumberOfSplineStacks;i++)
	{
	  double c[3]; vtkpxSurfaceUtil::GetColor(i,c);
	  splineStackSet[i]=vtkpxSplineStackSource::New();
	  splineStackSet[i]->SetEllipticalCylinder(8,
						   this->dimensions[2],
						   factor*spacing[0]*dimensions[0]/8.0*(1.0+i),
						   factor*spacing[1]*dimensions[1]/8.0*(1.0+i),
						   cx,cy,
						   dz,
						   this->origin[2]);
	  if (this->orientation==3)
	    {
	      splineStackSet[i]->SetPolarMode(1);
	      splineStackSet[i]->SetPolarMiddle(spacing[0]*dimensions[0]*0.5+origin[0]);
	    }
	  else
	    {
	      splineStackSet[i]->SetPolarMode(0);
	      if (dimensions[2]>12)
		{
		  splineStackSet[i]->SetTrimBottom(dimensions[2]/2-5);
		  splineStackSet[i]->SetTrimTop(dimensions[2]/2+5);
		}
	      else
		{
		  splineStackSet[i]->SetTrimTop(dimensions[2]-1);
		  splineStackSet[i]->SetTrimBottom(0);
		}
	    }
		
	  this->splineStackCollection->AddItem(splineStackSet[i]);
	  
	  for (int r=0;r<SSP_MAX_RENDERERS;r++)
	    {
	      splineStackActor[r][i]=vtkActor::New();
	      splineStackActor[r][i]->GetProperty()->SetColor(c);
	      this->splineStackActorCollection->AddItem(splineStackActor[r][i]);
	      
	      if (this->MultiRenderer[r]!=NULL)
		{
		  vtkPolyDataMapper* map=vtkPolyDataMapper::New();
		  map->SetInput(splineStackSet[i]->GetOutput());
		  
		  splineStackActor[r][i]->SetMapper(map);
		  if (display_mode[i]!=0)
		    splineStackActor[r][i]->SetVisibility(1);
		  else
		    splineStackActor[r][i]->SetVisibility(0);
		  
		  if (wireframemode[r]==1)
		    {
		      splineStackActor[r][i]->GetProperty()->SetRepresentationToWireframe();
		      splineStackActor[r][i]->GetProperty()->SetLineWidth(2.0);
		      splineStackActor[r][i]->GetProperty()->SetAmbient(1.0);
		      splineStackActor[r][i]->GetProperty()->SetDiffuse(0.0);
		      splineStackActor[r][i]->GetProperty()->SetSpecular(0.0);
		    }
		  map->Delete();
		  this->MultiRenderer[r]->GetRendererForFrame(fr)->AddActor(splineStackActor[r][i]);
		}
	    }
	}
      if (fr==0)
	{
	  current_splinestack=0;
	  currentSplineStack=splineStackSet[current_splinestack];
	}
    }

  this->ActualNumberOfFrames=numframes;

  
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DSplineStackControl::UpdateFrame(int frame)
{

  if (frame>=this->ActualNumberOfFrames)
    return;
  
  // Image Slice Ortho Slice First 
  int index=frame*this->NumberOfSplineStacks;
  int index2=index*SSP_MAX_RENDERERS;

  //  fprintf(stderr,"Index=%d index2=%d\n",index,index2);
  for (int i=0;i<this->NumberOfSplineStacks;i++)
    {
      splineStackSet[i]=(vtkpxSplineStackSource*)this->splineStackCollection->GetItemAsObject(index);
      ++index;
      for (int r=0;r<SSP_MAX_RENDERERS;r++)
	{
	  splineStackActor[r][i]=(vtkActor*)this->splineStackActorCollection->GetItemAsObject(index2);
	  ++index2;
	}
    }

  //fprintf(stderr,"Final Index=%d index2=%d\n",index,index2);

  for (int r=0;r<SSP_MAX_RENDERERS;r++)
    {
      if (this->MultiRenderer[r]!=NULL)
	this->Renderer[r]=this->MultiRenderer[r]->GetRendererForFrame(frame);
    }

  //  fprintf(stderr,"Done Updating %d\n",frame);

}
// -------------------------------------------------------------------------
void vtkpxGUI4DSplineStackControl::ChangeFrame(int newframe)
{

  newframe=Irange(newframe,0,this->NumberOfFrames-1);
  if (newframe==this->CurrentFrame)
    return;

  this->SplineEditor->SetEnableAutoUpdateModeCallback(0);
  // Check for Auto Update and execute it before frame changes 
  if (this->SplineEditor->IsAutoUpdateModeEnabled()==1)
    {
      this->UpdateFromSplineEditor();
    }


  int trim_bot[SSP_MAX_COLLECTIONS];
  int trim_top[SSP_MAX_COLLECTIONS];
  int draw_mode[SSP_MAX_COLLECTIONS];
  int ia=0;
  for (ia=0;ia<this->NumberOfSplineStacks;ia++)
    {
      trim_bot[ia]=splineStackSet[ia]->GetTrimBottom();
      trim_top[ia]=splineStackSet[ia]->GetTrimTop();
      draw_mode[ia]=splineStackSet[ia]->GetDrawMode();
    }

  this->UpdateFrame(newframe); 

  for (ia=0;ia<this->NumberOfSplineStacks;ia++)
    {
      splineStackSet[ia]->SetTrimBottom(trim_bot[ia]);
      splineStackSet[ia]->SetTrimTop(trim_top[ia]);
      splineStackSet[ia]->SetDrawMode(draw_mode[ia]);
      if (display_mode[ia]==1 && splineStackSet[ia]->GetWholeSurface()==1)
	splineStackSet[ia]->SetWholeSurface(0);
      if (display_mode[ia]==2 && splineStackSet[ia]->GetWholeSurface()==0)
	splineStackSet[ia]->SetWholeSurface(1);
    }

  this->currentSplineStack=splineStackSet[current_splinestack];
  this->CurrentFrame=newframe;
  this->UpdateSplineEditor();

  this->SplineEditor->SetEnableAutoUpdateModeCallback(1);
  this->Update();
}
// -------------------------------------------------------------------------

char* vtkpxGUI4DSplineStackControl::Initialize(const char* name,int inside)
{
  return this->Initialize(name,name,name,inside);
}

char* vtkpxGUI4DSplineStackControl::Initialize(const char* name,const char* name2,const char* name3,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();

  vtkpxGUISplineStackControl::Initialize(name,inside);

  //  fprintf(stderr,"name=%s,name2=%s,name3=%s\n",name,name2,name3);

  PXTkLabelFrame* mFrame=NULL;
  if (strcmp(name,name2)==0)
    {
      mFrame=new PXTkLabelFrame(this->EventManager,this->controlFrame,"Multi Frame");
      this->controlFrame->addChildren("-side top -expand false -fill x -padx 1 -pady 2",mFrame);
    }
  else
    {
      PXTkFrame* m=new PXTkFrame(NULL,name2);
      mFrame=new PXTkLabelFrame(this->EventManager,m,"Multi Frame");
      m->addChildren("-side top -expand false -fill x -padx 1 -pady 2",mFrame);
    }
  mFrame->setPackMode(PXTrue);


  PXTkFrame* segmentFrame=NULL;
  if (strcmp(name,name3)==0)
    {
      segmentFrame=new PXTkFrame(this->EventManager,this->controlFrame);
      this->controlFrame->addChildren("-side top -expand false -fill x -padx 1 -pady 2",segmentFrame);
    }
  else
    {
      PXTkFrame* m=new PXTkFrame(NULL,name3);
      segmentFrame=new PXTkFrame(this->EventManager,m);
      m->addChildren("-side top -expand false -fill x -padx 1 -pady 2",segmentFrame);
    }
  segmentFrame->setPackMode(PXTrue);


  PXTkFrame* f1=new PXTkFrame(this->EventManager,mFrame);
  PXTkFrame* f2=new PXTkFrame(this->EventManager,mFrame);
  PXTkFrame* f3=new PXTkFrame(this->EventManager,mFrame);
  PXTkFrame* f4=new PXTkFrame(this->EventManager,mFrame);
  mFrame->addChildren("-side top -expand false -fill x -pady 2 ",f1,f2,f3,f4);

 
  PXTkLabel* labs=new PXTkLabel(this->EventManager,f1,"Current Stack:");
  currentCollection2=new PXTkOptionMenu(this->EventManager,f1,"Stack 1",800);
  for (int ip=1;ip<this->NumberOfSplineStacks;ip++)
    {
      sprintf(pxtk_buffer4,"Stack %d",ip+1);
      currentCollection2->addOption(pxtk_buffer4);
    }
  currentCollection2->setIndex(current_splinestack);
  f1->addChildren("-side left -expand t -fill x -pady 1",labs,currentCollection2);
    
  PXTkButton* b0a=new PXTkButton(this->EventManager,f2,"Load All",801);
  PXTkButton* b0b=new PXTkButton(this->EventManager,f2,"Save All",802);
  f2->addChildren("-side top -expand t -fill x -pady 1 -padx 2",b0a,b0b);

  autosavebutton=new PXTkCheckButton(this->EventManager,f3,"Autosave",803);
  autosavebutton->setState(do_autosave);
  backupbutton=new PXTkCheckButton(this->EventManager,f3,"Backup",803);
  backupbutton->setState(do_backup);
  f3->addChildren("-side top -expand t -fill x -pady 1",autosavebutton,backupbutton);

  f4->setPackMode(PXTrue);

  PXTkButton* but1=new PXTkButton(this->EventManager,f4,"Shift And Scale",810);
  PXTkButton* but3=new PXTkButton(this->EventManager,f4,"Volume",812);
  PXTkButton* but4=new PXTkButton(this->EventManager,f4,"Cylinders",813);
  PXTkButton* but6=new PXTkButton(this->EventManager,f4,"Export",815);
  PXTkButton* but7=new PXTkButton(this->EventManager,f4,"Extract From (Binary) Image",816);
  f4->addChildren("-side top -expand t -fill x -pady 1",but1,but3,but4);
  f4->addChildren("-side top -expand t -fill x -pady 1",but6,but7);

  this->CreateSegmentationControl(segmentFrame);

  if (this->optionDialog==NULL)
    this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,10);


  return GetWidgetName();
}

/* -------------------------------------------------------------------------*/ 
void vtkpxGUI4DSplineStackControl::SetRenderer(vtkRenderer* ren,int index)
{
  vtkErrorMacro(<<"Cannot Use this Function in vtkpxGUI4DSplineStackControl");
}

void vtkpxGUI4DSplineStackControl::SetRenderer(vtkpxGUI4DRenderer* ren,int index)
{
  this->SetRenderer(ren,index,0);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DSplineStackControl::SetRenderer(vtkRenderer *ren,int index,int wireframe)
{
  vtkErrorMacro(<<"Cannot Use this Function in vtkpxGUI4DSplineStackControl");
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DSplineStackControl::SetRenderer(vtkpxGUI4DRenderer *ren,int index,int wireframe)
{
  index=Irange(index,0,SSP_MAX_RENDERERS-1);

  if (this->MultiRenderer[index]!=NULL)
    return;

  this->MultiRenderer[index]=ren;
  this->wireframemode[index]=wireframe;
  this->Renderer[index]=this->MultiRenderer[index]->GetRendererForFrame(this->CurrentFrame);

}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DSplineStackControl::Update()
{
  vtkpxGUISplineStackControl::Update();
  UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DSplineStackControl::UpdateDisplay()
{
  vtkpxGUISplineStackControl::UpdateDisplay();
}
/* -------------------------------------------------------------------------*/

int  vtkpxGUI4DSplineStackControl::HandleEvent(int event)
{
  if (!this->Initialized)
      return TCL_OK;

  //  fprintf(stderr,"Event=%d\n",event);

  if (event<10)
      return this->EventManager->handleDirectEvent(event);

  if (event<800)
    return vtkpxGUISplineStackControl::HandleEvent(event);

  int updateeditor=0;

  //  fprintf(stderr,"Switching to Event=%d Part I\n",event);

  if (this->NumberOfFrames==0)
    return TCL_OK;


  //  fprintf(stderr,"Switching to Event=%d\n",event);

  switch (event)
    {
    case 800:
      SetCurrentCollection(currentCollection2->getIndex());
      break;

    case 801:
      if (this->NumberOfFrames>0)
	{
	  char* line=this->EventManager->getOpenFilename("Load series","Series","*.01.sur");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		updateeditor=(this->MultiLoad(line)>0);
	    }
	}
      break;

    case 802:
      if (this->NumberOfFrames>0)
	{
	  sprintf(pxtk_buffer,"%s.01.sur",surface_filename[current_splinestack]);
	  char* line=this->EventManager->getSaveFilename("Save series",pxtk_buffer,"Series","*.01.sur");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		{
		  char *newline=new char[strlen(line)+1 ];
		  strcpy(newline,line);
		  //		  fprintf(stderr,"Multi Save %s\n",newline);
		  this->MultiSave(newline);
		  delete [] newline;
		}
	    }
	}
      break;

    case 803:
      do_autosave=autosavebutton->getState();
      do_backup=backupbutton->getState();
      break;

    case 810:
      this->DoShiftScale(0,850);
      break;
    
    case 811:
      this->DoResample(0,852);
      break;
    
    case 812:
      //      fprintf(stderr,"Doing nothing for 812\n");
      this->DoVolume(0,854);
      break;
    
    case 813:
      //      fprintf(stderr,"Option=813\n");
      this->DoCylinders(0,856);
      break;
    
    case 814:
      this->DoSmoothApex(0,858);
      break;
    
    case 815:
      this->DoExport(0,860);
      break;

    case 816:
      this->DoImport(0,864);
      break;

    case 850:
      this->DoShiftScale(1);
      updateeditor=1;
      break;
    
    case 852:
      this->DoResample(1);
      updateeditor=1;
      break;
    
    case 854:
      this->DoVolume(1);
      updateeditor=1;
      break;
    
    case 856:
      this->DoCylinders(1);
      updateeditor=1;
      break;
    
    case 858:
      this->DoSmoothApex(1);
      updateeditor=1;
      break;
    
    case 860:
      this->DoExport(1);
      updateeditor=1;
      break;
    
    case 862:
      this->DoExport(2);
      updateeditor=1;
      break;

    case 864:
      this->DoImport(1,864);
      updateeditor=1;
      break;

    case 900:
      this->AutoSegmentMode=1;
      this->AutoSegment();
      break;

    case 901:
      this->AutoSegmentMode=0;
      this->AutoSegment();
      break;

    case 902:
      this->InterpolateSpatially();
      break;
      
    case 903:
    case 904:
    case 905:
    case 906:
      this->AutoTemporalVolumeSegment(event-903);
      break;
      
    case 908:
      this->InterpolateTemporally(0);
      break;

    case 909:
      this->InterpolateTemporally(1);
      break;
      

    case 910:
      this->AbortProcessing=1;
      break;
    }
  
  //  fprintf(stderr,"Done %d\n",event);


  if (updateeditor==1 && this->BlockEditorCallback==0)
    this->UpdateSplineEditor();


  this->UpdateDisplay();

  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DSplineStackControl::SetImage(vtkImageData* img,int orient,int level,int frame)
{
  this->BlockEditorCallback=1;

  this->orientation=vtkpxGUIBaseImageViewer::MapOrientationToOldStyle(orient);

  if (img!=NULL)  
    this->SetImage(img);
  this->SetCurrentFrame(frame);
  this->BlockEditorCallback=0;

}


void vtkpxGUI4DSplineStackControl::SetCurrentCollection(int num)
{
  if (!this->Initialized)
    return;
  vtkpxGUISplineStackControl::SetCurrentCollection(num);
  currentCollection2->setIndex(current_splinestack);
  currentCollection->setIndex(current_splinestack);
}

void vtkpxGUI4DSplineStackControl::SetImage(vtkImageData* img)
{
  if (img==NULL)
    return;

  if (this->image!=NULL)
    this->image->Delete();
  this->image=vtkImageData::New();
  this->image->ShallowCopy(img);      
  
  if (this->SplineEditor!=NULL)
    {
      this->SplineEditor->SetImage(this->image,this->orientation);
      this->SplineEditor->ShowFrameControls(0);
      this->SplineEditor->SetSlice(-1);
    }

  this->ResetSplineStacks();


  if (this->NumberOfFrames<2)
    {
      toggle_past->configure("-state","disabled");
      toggle_future->configure("-state","disabled");
    } 
  else 
    {
      toggle_past->configure("-state","normal");
      toggle_future->configure("-state","normal");
    }

  if (this->image->GetDimensions()[2]<2)
    {
      toggle_up->configure("-state","disabled");
      toggle_down->configure("-state","disabled");
    } 
  else 
    {
      toggle_up->configure("-state","normal");
      toggle_down->configure("-state","normal");
    }

  for (int ia=0;ia<=1;ia++)
    {
      frameScale[ia]->setRange(1,this->NumberOfFrames);
      frameScale[ia]->setValue(1+ia*(this->NumberOfFrames-1));
      sliceScale[ia]->setRange(0,this->image->GetDimensions()[2]-1);
      sliceScale[ia]->setValue(ia*(this->image->GetDimensions()[2]-1));
    }


}

/* -------------------------------------------------------------------------*/
int vtkpxGUI4DSplineStackControl::MultiLoad(const char* fname)
{
  strcpy(pxtk_buffer2,fname);

  char* noext1=nothisextension(fname,".sur");
  char* noext2=nothisextension(noext1,".01");
  int sum=0;

  for (int frame=0;frame<this->NumberOfFrames;frame++)
    {
      sprintf(pxtk_buffer3,"%s.%02d.sur",noext2,frame+1);
      vtkpxSplineStackSource *stack=this->GetSplineStack(frame,current_splinestack);
      int ok=stack->Load(pxtk_buffer3);

      stack->AdjustNumberOfSplines(this->dimensions[2]);
      sum+=(1-ok);
      sprintf(pxtk_buffer,"Loading from %s (status=%d)\n",pxtk_buffer3,ok);
      PXTkApp::printToConsole(pxtk_buffer);

      if (ok>0 && frame==0)
	{
	  sprintf(pxtk_buffer3,"%s",noext2);
	  this->SetItemFilename(current_splinestack,pxtk_buffer3);
	  sprintf(pxtk_buffer4,"%s.%d",PXTkApp::getFilenameTail(noext2),current_splinestack+1);
	  currentCollection2->changeLabel(current_splinestack,pxtk_buffer4);
	}

    }

  if (sum<this->NumberOfFrames)
    {
      delete [] surface_filename[current_splinestack];
      surface_filename[current_splinestack]=new char[strlen(noext2)+1];
      strcpy(surface_filename[current_splinestack],noext2);
    }

  if (sum>0)
    PXTkApp::popupConsole();
      
  delete [] noext2;
  delete [] noext1;
  this->UpdateAfterExternalEditing();
  return 1;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DSplineStackControl::UpdateAfterExternalEditing()
{
  //  fprintf(stderr,"In 4D SSC:: UpdateAfterExternalEditing");
  this->UpdateFrame(this->CurrentFrame);
  this->UpdateControlsFromCurrentSplineStack();
}
/* -------------------------------------------------------------------------*/
int vtkpxGUI4DSplineStackControl::MultiSave(const char* fname)
{
  //  fprintf(stderr,"fname=%s\n",fname);
  char* noext1=nothisextension(fname,".sur");
  //  fprintf(stderr,"noext1=%s\n",noext1);
  char* noext2=nothisextension(noext1,".01");
  //  fprintf(stderr,"noext2=%s\n",noext2);
  int sum=0;

  if (this->do_backup==PXTrue)
    this->DoBackup(current_splinestack,noext2);

  for (int frame=0;frame<this->NumberOfFrames;frame++)
    {
      sprintf(pxtk_buffer3,"%s.%02d.sur",noext2,frame+1);
      vtkpxSplineStackSource *stack=this->GetSplineStack(frame,current_splinestack);
      int ok=stack->Save(pxtk_buffer3);
      sum+=(1-ok);
      sprintf(pxtk_buffer,"Saving to %s (status=%d)\n",pxtk_buffer3,ok);
      PXTkApp::printToConsole(pxtk_buffer);
      //fprintf(stderr,"%s\n",pxtk_buffer);

      if (ok>0 && frame==0)
	{
	  sprintf(pxtk_buffer3,"%s",noext2);
	  this->SetItemFilename(current_splinestack,pxtk_buffer3);
	  sprintf(pxtk_buffer4,"%s.%d",PXTkApp::getFilenameTail(noext2),current_splinestack+1);
	  currentCollection2->changeLabel(current_splinestack,pxtk_buffer4);
	}
    }
  


  if (sum>0)
    PXTkApp::popupConsole();

  if (sum<this->NumberOfFrames)
    {
      delete [] surface_filename[current_splinestack];
      surface_filename[current_splinestack]=new char[strlen(noext2)+1];
      strcpy(surface_filename[current_splinestack],noext2);
    }

      
  //  fprintf(stderr,"Here on to cleanup\n");

  delete [] noext2;
  delete [] noext1;

  //  fprintf(stderr,"Here on to screen update\n");

  this->UpdateAfterExternalEditing();
  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUI4DSplineStackControl::DoBackup(int stackindex,const char* rootname)
{
  stackindex=Irange(stackindex,0,this->NumberOfSplineStacks-1);
  //  fprintf(stderr,"Beginning Do Backup %d %s\n",stackindex,rootname);
  
  for (int ia=0;ia<this->NumberOfFrames;ia++)
    {
      sprintf(pxtk_buffer,"%s.%02d.sur",rootname,ia+1);
      PXTkApp::fileOperation("exists",pxtk_buffer,NULL);
      int ok=PXTkApp::getTclIntResult();
      //      fprintf(stderr,"pxtk_buffer=%s, %d\n",pxtk_buffer,ok);

      if (ok==1)
	{
	  sprintf(pxtk_buffer,"%s.%02d.sur",rootname,ia+1);
	  sprintf(pxtk_buffer2,"%s_back.%02d.sur",rootname,ia+1);
	  sprintf(pxtk_buffer3,"File %s exists copying to %s\n",pxtk_buffer,pxtk_buffer2);
	  PXTkApp::printToConsole(pxtk_buffer3);
	  PXTkApp::copyFile(pxtk_buffer,pxtk_buffer2);
	}
    }
  return 1;
}

/* -------------------------------------------------------------------------*/
int vtkpxGUI4DSplineStackControl::DoAutosave(int frame)
{
  frame=Irange(frame,0,this->NumberOfFrames-1);
  for (int i=0;i<this->NumberOfSplineStacks;i++)
    {
      vtkpxSplineStackSource *stack=this->GetSplineStack(frame,i);
      sprintf(pxtk_buffer,"%s_auto.%02d.sur",surface_filename[i],frame+1);
      stack->Save(pxtk_buffer);
    }
  return 1;
}


/* -------------------------------------------------------------------------*/
vtkpxSplineStackSource* vtkpxGUI4DSplineStackControl::GetSplineStack(int frame,int index)
{
  if (this->splineStackCollection==NULL)
    return NULL;

  frame=Irange(frame,0,this->NumberOfFrames-1);
  index=Irange(index,0,this->NumberOfSplineStacks);

  int ind=frame*this->NumberOfSplineStacks+index;
  return (vtkpxSplineStackSource*)this->splineStackCollection->GetItemAsObject(ind);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DSplineStackControl::UpdateSplineEditor()
{
  if (this->SplineEditor==NULL)
    return;
  if (this->SplineEditor->GetCurrentFrame()!=this->CurrentFrame)
    this->SplineEditor->SetCurrentFrame(this->CurrentFrame);

  int fr[2];
  fr[0]=this->CurrentFrame-1;
  if (fr[0]<0)
    fr[0]=this->NumberOfFrames-1;
  fr[1]=this->CurrentFrame+1;
  if (fr[1]>=this->NumberOfFrames)
    fr[1]=0;

  int sl=this->SplineEditor->GetBeginSlice();
  int num=this->GetNumberOfSplineStacks();

  for (int j=0;j<2;j++)  
    for (int i=0;i<num;i++)
      {
	vtkpxSplineStackSource *stack=this->GetSplineStack(fr[j],i);
	vtkpxSplineSource* spl=this->SplineEditor->GetSplineControl()->GetGhostSpline(j,i);
	if (spl!=NULL)
	  {
	    stack->SetSplineFromSplineStack(spl,sl);
	    spl->Modified();
	  }
      }
        
  vtkpxGUISplineStackControl::UpdateSplineEditor();
 
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DSplineStackControl::SetCurrentFrame(int t)
{
  if (t<0) 
    t=this->NumberOfFrames-1;
  if (t>this->NumberOfFrames-1)
    t=0;


  if (!this->Initialized)
    return;

  //  fprintf(stderr,"Changing Frame to %d\n",t);
  

  this->ChangeFrame(t);
  this->UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
int vtkpxGUI4DSplineStackControl::GetCurrentFrame()
{
  return this->CurrentFrame;
}

int vtkpxGUI4DSplineStackControl::GetNumberOfFrames()
{
  return this->NumberOfFrames;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DSplineStackControl::UpdateFromSplineEditor()
{
  vtkpxGUISplineStackControl::UpdateFromSplineEditor();
  if (this->do_autosave==PXTrue)
    this->DoAutosave(this->CurrentFrame);
}

/* -------------------------------------------------------------------------*/
void vtkpxGUI4DSplineStackControl::DoShiftScale(int mode,int moption)
{
  if (mode==0)
    {
      if (this->optionDialog!=NULL)
	{
	  if (this->optionDialog->isOpen())
	    return;
	  delete this->optionDialog;
	}
      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,6);
      this->optionDialog->initDisplay();

      this->optionDialog->setTitleDescription("Shift+Scale Surface",
					      "Enter Multipliers to Shift and Scale the Surfaces");
      this->optionDialog->setfoption(0,"Shift-X","%.1f",0.0);
      this->optionDialog->setfoption(2,"Shift-Y","%.1f",0.0);
      this->optionDialog->setfoption(4,"Shift-Z","%.1f",0.0);
      this->optionDialog->setfoption(1,"Scale-X","%.1f",this->spacing[0]);
      this->optionDialog->setfoption(3,"Scale-Y","%.1f",this->spacing[1]);
      this->optionDialog->setfoption(5,"Scale-Z","%.1f",this->spacing[2]/this->currentSplineStack->GetZSpacing());
      this->optionDialog->setMenuOption(moption);
      this->optionDialog->popupDialog();
    }

  if (mode==1)
    {
      double sh[3],sc[3];
      for (int ia=0;ia<=2;ia++)
	{
	  sh[ia]=Frange(this->optionDialog->getfoption(ia*2)  ,-100.0,100.0);
	  sc[ia]=Frange(this->optionDialog->getfoption(ia*2+1),   0.1, 10.0);
	}
     
      /*fprintf(stderr,"Shifts %.1f %.1f %.1f\t scales=%.1f %.1f %.1f\n",
	sh[0],sh[1],sh[2],sc[0],sc[1],sc[2]);*/
      for (int frame=0;frame<this->NumberOfFrames;frame++)
	{
	  vtkpxSplineStackSource *stack=this->GetSplineStack(frame,current_splinestack);
	  stack->ScaleShiftXY(sh[0],sh[1],sc[0],sc[1]);
	  stack->ScaleShiftZ(sh[2],sc[2]);
	  //fprintf(stderr,"Processed frame=%d/%d\n",frame+1,this->NumberOfFrames);
	}
      
      this->UpdateAfterExternalEditing();
    }
      
      
}
/* -------------------------------------------------------------------------*/
void vtkpxGUI4DSplineStackControl::DoResample(int mode,int moption)
{
  if (mode==0)
    {
      if (this->optionDialog!=NULL)
	{
	  if (this->optionDialog->isOpen())
	    return;
	  delete this->optionDialog;
	}
      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,2);
      this->optionDialog->initDisplay();
      
      this->optionDialog->setTitleDescription("Resample Surfaces",
					      "Enter npoints=-1 for auto-spacing");
      this->optionDialog->setioption(0,"Points","%d",-1);
      this->optionDialog->setfoption(1,"Smoothness","%.1f",2.0);
      this->optionDialog->setMenuOption(moption);
      this->optionDialog->popupDialog();
      return;
    }

  if (mode==1)
    {
      int np=Irange(this->optionDialog->getioption(0),3,50);
      if (np==3)
	np=-1;
      float ds=Frange(this->optionDialog->getfoption(1),1.0,20.0);
      
      for (int frame=0;frame<this->NumberOfFrames;frame++)
	{
	  //fprintf(stderr,"Processing frame=%d\n",frame);
	  vtkpxSplineStackSource *stack=this->GetSplineStack(frame,current_splinestack);
	  stack->ChangeSpacing(ds,np,0.01);
	}
      
      this->UpdateAfterExternalEditing();
    }

      

}

void vtkpxGUI4DSplineStackControl::DoVolume(int mode,int moption)
{

  //  fprintf(stderr,"in Volume mode=%d moption=%d\n",mode,moption);

  if (mode==0)
    {
      //      fprintf(stderr,"in Volume mode=%d moption=%d\n",mode,moption);

      if (this->optionDialog!=NULL)
	{
	  //	  fprintf(stderr,"Should I delete optionDialog\n");
	  if (this->optionDialog->isOpen())
	    return;
	  //	  fprintf(stderr,"Deleting optionDialog\n");
	  delete this->optionDialog;
	}
      //      fprintf(stderr,"Creating optionDialog\n");

      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,2);
      this->optionDialog->initDisplay();

      this->optionDialog->setTitleDescription("Volume Computation",
					      "compute volume in mm3");
      this->optionDialog->setioption(0,"Begin Frame","%d",1);
      this->optionDialog->setioption(1,"End   Frame","%d",this->NumberOfFrames);
      this->optionDialog->setMenuOption(moption);
      this->optionDialog->popupDialog();
    }

  if (mode==1)
    {
      int bframe=Irange(this->optionDialog->getioption(0),1,this->NumberOfFrames);
      int eframe=Irange(this->optionDialog->getioption(1),1,this->NumberOfFrames);
      bframe-=1;
      eframe-=1;
      if (eframe<bframe)
	eframe+=this->NumberOfFrames;
      PXTkApp::popupConsole();
      PXTkApp::printToConsole("\n Computing Volumes For Surfaces\n------------------------\n");
      int minf=bframe,maxf=bframe;
      float minv=0.0,maxv=0.0;
      for (int frame=bframe;frame<=eframe;frame++)
	{
	  int fr=frame;
	  if (frame>=this->NumberOfFrames)
	    fr-=this->NumberOfFrames;
	  vtkpxSplineStackSource *stack=this->GetSplineStack(fr,current_splinestack);
	  float a=stack->GetVolume();

	  if (frame==bframe)
	    {
	      minv=a;
	      maxv=a;
	    }
	  else if (a<minv)
	    {
	      minv=a;
	      minf=fr;
	    }
	  else if (a>maxv)
	    {
	      maxv=a;
	      maxf=fr;
	    }

	  sprintf(pxtk_buffer,"Surface %d frame %2d volume=%.2f\n",current_splinestack+1,fr+1,a);
	  PXTkApp::printToConsole(pxtk_buffer);
	}
      
      sprintf(pxtk_buffer,"\nLargest  Volume at frame %d = %.2f\n",maxf+1,maxv);
      PXTkApp::printToConsole(pxtk_buffer);
      sprintf(pxtk_buffer,"Smallest Volume at frame %d = %.2f\n\n",minf+1,minv);
      PXTkApp::printToConsole(pxtk_buffer);

    }


}

void vtkpxGUI4DSplineStackControl::DoCylinders(int mode,int moption)
{

  if (mode==0)
    {
      if (this->optionDialog!=NULL)
	{
	  if (this->optionDialog->isOpen())
	    return;
	  delete this->optionDialog;
	}

      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,7);
      this->optionDialog->initDisplay();

      this->optionDialog->setTitleDescription("Set Cylinders",
					      "Enter Cylinder Parameters");
      this->optionDialog->setfoption(0,"Center X","%.2f",this->origin[0]+this->dimensions[0]*this->spacing[0]*0.5);
      this->optionDialog->setfoption(2,"Center Y","%.2f",this->origin[1]+this->dimensions[1]*this->spacing[1]*0.5);
      this->optionDialog->setfoption(1,"Radius X","%.2f",this->dimensions[0]*this->spacing[0]*0.2);
      this->optionDialog->setfoption(3,"Radius Y","%.2f",this->dimensions[1]*this->spacing[1]*0.2);
      this->optionDialog->setioption(4,"Bot Slice","%d",0);
      this->optionDialog->setioption(5,"Top Slice","%d",this->dimensions[2]-1);
      this->optionDialog->setioption(6,"Num points","%d",8);
      this->optionDialog->setMenuOption(moption);
      this->optionDialog->popupDialog();
      return;
    }


  if (mode==1)
    {
      float c[2],r[2];
      for (int ia=0;ia<=1;ia++)
	{
	  c[ia]=Frange(this->optionDialog->getfoption(ia*2),
		       this->origin[ia],this->origin[ia]+this->dimensions[ia]*this->spacing[ia]);
	  r[ia]=Frange(this->optionDialog->getfoption(ia*2+1),
		       this->spacing[ia],0.5*+this->dimensions[ia]*this->spacing[ia]);
	}
      
      
      int numsl=this->dimensions[2];
      float bottomz=this->origin[2];
      float zsp=this->spacing[2];
      int nump=Irange(this->optionDialog->getioption(6),4,24);
      int minz=Irange(this->optionDialog->getioption(4),0,this->dimensions[2]-2);
      int maxz=Irange(this->optionDialog->getioption(5),minz,this->dimensions[2]-2);
      
      for (int frame=0;frame<this->NumberOfFrames;frame++)
	{
	  vtkpxSplineStackSource *stack=this->GetSplineStack(frame,current_splinestack);
	  stack->SetEllipticalCylinder(nump,numsl,r[0],r[1],c[0],c[1],zsp,bottomz);
	  //	  fprintf(stderr,"Do Cylinders :: Setting Elliptical Cylinder f=%d\n",frame);
	  stack->SetTrimBottom(minz);
	  stack->SetTrimTop(maxz);
	}
    
      this->UpdateAfterExternalEditing();
      
      char line[100];
      strcpy(line,"CStack.sur");
      this->SetItemFilename(current_splinestack,line);
      sprintf(line,"CStack.%d",current_splinestack+1);
      currentCollection2->changeLabel(current_splinestack,line);
      

    }

}
void vtkpxGUI4DSplineStackControl::DoSmoothApex(int mode,int moption)
{
  if (mode==0)
    {
      if (this->optionDialog!=NULL)
	{
	  if (this->optionDialog->isOpen())
	    return;
	  delete this->optionDialog;
	}
      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,3);
      this->optionDialog->initDisplay();
      this->optionDialog->setTitleDescription("Smooth Apex",
					      "Smooth Apex");
      this->optionDialog->setioption(0,"Begin Frame","%d",1);
      this->optionDialog->setioption(1,"End   Frame","%d",this->NumberOfFrames);
      this->optionDialog->setfoption(2,"Smooth Sigma","%.1f",1.0);
      this->optionDialog->setMenuOption(moption);
      this->optionDialog->popupDialog();
    }


}


// --------------------------------------------------------------------------


void vtkpxGUI4DSplineStackControl::DoExport(int mode,int moption)
{
  if (mode==0)
    {
      if (this->optionDialog->isOpen())
	return;

      delete this->optionDialog;
      this->optionDialog=new PXTkSelDialog(this->EventManager,this->controlFrame,7);
      this->optionDialog->initDisplay();
      this->optionDialog->showDebugButton();
      this->optionDialog->setDebugButton("Update Z-Range",moption+2);
      this->optionDialog->setTitleDescription("Export Surfaces",
					      "Format 1=.tstack 2=.vtk");

      
      if (this->orientation<3)
	{
	  this->optionDialog->setfoption(0,"Spacing XY","%.1f",this->spacing[0]);
	  this->optionDialog->setfoption(1,"Spacing  Z","%.1f",this->spacing[0]);
	}
      else
	{
	  this->optionDialog->setfoption(0,"Spacing XY","%.1f",this->spacing[0]*3.0);
	  this->optionDialog->setfoption(1,"Spacing  Z","%.1f",this->spacing[0]*3.0);
	}

      this->optionDialog->setioption(4,"Start Frame","%d",1);
      this->optionDialog->setioption(5,"End   Frame","%d",this->NumberOfFrames);
      this->optionDialog->setioption(6,"Format","%d",1);
      this->optionDialog->setMenuOption(moption);
    } 

  if (mode==0 || mode == 2)
    {
      float z[2];
      int bf=0,ef=this->NumberOfFrames-1;
      
      if (mode==2)
	{
	  bf=Irange(this->optionDialog->getioption(4)-1,0,this->NumberOfFrames-1);
	  ef=Irange(this->optionDialog->getioption(5)-1,0,this->NumberOfFrames-1);
	}
      
      vtkpxSplineStackSource::ComputeZRange(this->splineStackCollection,
					    this->NumberOfFrames,
					    this->NumberOfSplineStacks,
					    bf,ef,
					    current_splinestack,z);
      
      //      fprintf(stderr,"Z-range = %.1f %.1f\n",z[0],z[1]);

      this->optionDialog->setfoption(2,"Bot Slice","%.1f",z[0]);
      this->optionDialog->setfoption(3,"Top Slice","%.1f",z[1]);
    }

  if (mode==0)
    this->optionDialog->popupDialog();

  if (mode!=1)
    return;

  float uspacing=Frange(this->optionDialog->getfoption(0),0.2*this->spacing[0],this->spacing[0]*10);
  float vspacing=Frange(this->optionDialog->getfoption(1),0.2*this->spacing[2],this->spacing[2]*10);
  float z[2];
  
  if (this->orientation<3)
    {
      float botslice=Frange(this->optionDialog->getfoption(2),0,this->dimensions[2]-1.0);
      float topslice=Frange(this->optionDialog->getfoption(3),botslice+1,this->dimensions[2]-1.0);
      z[0]=botslice*spacing[2];
      z[1]=topslice*spacing[2];
    }
  else
    {
      z[0]=Frange(this->optionDialog->getfoption(2),0,this->dimensions[1]-1.0);
      z[1]=Frange(this->optionDialog->getfoption(3),z[0]+1.0,this->dimensions[1]-1.0);
    }
  
   
  int format=Irange(this->optionDialog->getioption(6),1,2);
  int bf=Irange(this->optionDialog->getioption(4)-1,0,this->NumberOfFrames-1);
  int ef=Irange(this->optionDialog->getioption(5)-1,0,this->NumberOfFrames-1);
  
  
  char extension[10];
  char* line=NULL;
  if (format==1)
    {
      sprintf(pxtk_buffer,"%s.01.tstack",surface_filename[current_splinestack]);
      line=this->EventManager->getSaveFilename("Save series as tstacks",pxtk_buffer,"Series","*.01.tstack");
      strcpy(extension,".tstack");
    }
  else
    {
      sprintf(pxtk_buffer,"%s.01.vtk",surface_filename[current_splinestack]);
      line=this->EventManager->getSaveFilename("Save exported series as vtk surfaces",pxtk_buffer,"Series","*.01.vtk");
      strcpy(extension,".vtk");
    }
  
  if (line==NULL)
    return;
  
  
  char* noext1=nothisextension(line,extension);
  char* noext2=nothisextension(noext1,".01");
  vtkpxTriangulatedStackSource* tstack=vtkpxTriangulatedStackSource::New();
  vtkPolyDataWriter* poly=vtkPolyDataWriter::New();
  
  PXTkApp::popupConsole();
  for (int frame=bf;frame<=ef;frame++)
    {
      sprintf(pxtk_buffer3,"%s.%02d%s",noext2,frame+1,extension);
      vtkpxSplineStackSource *stack=this->GetSplineStack(frame,current_splinestack);
      tstack->SetFromSplineStackSource2(stack,uspacing,vspacing,z[0],z[1]);
      
      if (format==1)
	{
	  int ok=tstack->Save(pxtk_buffer3);
	  sprintf(pxtk_buffer,"Saving to %s (status=%d)\n",pxtk_buffer3,ok);
	}
      else
	{
	  tstack->Update();
	  poly->SetInput(tstack->GetOutput());
	  poly->SetFileName(pxtk_buffer3);
	  poly->Update();
	  sprintf(pxtk_buffer,"Saving to %s\n",pxtk_buffer3);
	}
      PXTkApp::printToConsole(pxtk_buffer);
    }
  poly->Delete();
  tstack->Delete();
}


// ---------------------------------------------------------------------------
void vtkpxGUI4DSplineStackControl::ComputeVOIProperties(int frame)
{
  vtkpxGUISplineStackControl::ComputeVOIProperties(this->CurrentFrame);
}
// ---------------------------------------------------------------------------
//
//   Segmentation Stuff
//
// ---------------------------------------------------------------------------
// Create GUI
// ---------------------------------------------------------------------------
void vtkpxGUI4DSplineStackControl::CreateSegmentationControl(PXTkFrame* segmentFrame)
{
  PXTkLabelFrame* temporal=new PXTkLabelFrame(this->EventManager,segmentFrame,"Temporal");
  PXTkLabelFrame* spatial=new PXTkLabelFrame(this->EventManager,segmentFrame,"Spatial");
  segmentFrame->addChildren("-side top -expand f -fill x",spatial,temporal);

  PXTkFrame* spatial_t=new PXTkFrame(this->EventManager,spatial);
  toggle_up    =new PXTkCheckButton(this->EventManager,spatial_t,"Up",-1);
  toggle_down  =new PXTkCheckButton(this->EventManager,spatial_t,"Down",-1);
  PXTkButton* but1=new PXTkButton(this->EventManager,spatial_t,"Auto",900);
  PXTkButton* but2=new PXTkButton(this->EventManager,spatial_t,"Batch",901);

  toggle_up->setState(PXTrue);
  toggle_down->setState(PXTrue);
  spatial_t->addChildren("-side left -expand f -fill x ",toggle_up,toggle_down,but1,but2);


  sliceScale[0]=new PXTkArrowScale(this->EventManager,spatial,"Bottom",-1,PXTrue);
  sliceScale[1]=new PXTkArrowScale(this->EventManager,spatial,"Top",-1,PXTrue);
  for (int ia=0;ia<2;ia++)
    {
      sliceScale[ia]->setRange(0,15);
      sliceScale[ia]->setValue(float(ia*14+1));
      sliceScale[ia]->setLengthWidth(120,10);
      sliceScale[ia]->setIncrement(1.0);
      sliceScale[ia]->setResolution(1.0);
    }
  PXTkFrame* spatial_b=new PXTkFrame(this->EventManager,spatial);
  PXTkButton* but3=new PXTkButton(this->EventManager,spatial_b,"Interpolate",902);
  spatial_b->addChildren("-side right -expand f -padx 20 -pady 1",but3);
  spatial->addChildren("-side top -expand f -fill x",
		       spatial_t,sliceScale[0]->getMainWindow(),sliceScale[1]->getMainWindow(),spatial_b);
  
  

  PXTkFrame* temporal_t=new PXTkFrame(this->EventManager,temporal);
  PXTkFrame* temporal_t2=new PXTkFrame(this->EventManager,temporal);
  PXTkFrame* temporal_t3=new PXTkFrame(this->EventManager,temporal);

  toggle_past  =new PXTkCheckButton(this->EventManager,temporal_t,"Past",-1);
  toggle_future=new PXTkCheckButton(this->EventManager,temporal_t,"Future",-1);
  toggle_past->setState(PXTrue);
  toggle_future->setState(PXTrue);

  PXTkButton* tbut1=new PXTkButton(this->EventManager,temporal_t2,"Auto Current",903);
  PXTkButton* tbut2=new PXTkButton(this->EventManager,temporal_t2,"Auto All",904);
  
  PXTkButton* tbut3=new PXTkButton(this->EventManager,temporal_t3,"Batch Current",905);
  PXTkButton* tbut4=new PXTkButton(this->EventManager,temporal_t3,"Batch All",906);

  temporal_t->addChildren("-side left -expand f -fill x ",toggle_past,toggle_future);
  temporal_t2->addChildren("-side left -expand f -padx 2 ",tbut1,tbut2);
  temporal_t3->addChildren("-side left -expand f -padx 2 ",tbut3,tbut4);

  frameScale[0]=new PXTkArrowScale(this->EventManager,temporal,"ED",-1,PXTrue);
  frameScale[1]=new PXTkArrowScale(this->EventManager,temporal,"ES",-1,PXTrue);
  for (int iaa=0;iaa<2;iaa++)
    {
      frameScale[iaa]->setRange(1,10);
      frameScale[iaa]->setValue(float(iaa*9+1));
      frameScale[iaa]->setLengthWidth(120,10);
      frameScale[iaa]->setIncrement(1.0);
      frameScale[iaa]->setResolution(1.0);
    }

  PXTkFrame* temporal_b=new PXTkFrame(this->EventManager,temporal);
  PXTkButton* tbut13=new PXTkButton(this->EventManager,temporal_b,"Interpolate Current",908);
  PXTkButton* tbut14=new PXTkButton(this->EventManager,temporal_b,"Interpolate All",909);
  temporal_b->addChildren("-side right -expand f -padx 1 -pady 1",tbut13,tbut14);

  temporal->addChildren("-side top -expand f -fill x",
			temporal_t,temporal_t2,temporal_t3);

  temporal->addChildren("-side top -expand f -fill x",
			frameScale[0]->getMainWindow(),frameScale[1]->getMainWindow(),temporal_b);

  wait_dialog=new PXTkWaitDialog(this->EventManager,"Processing ...",910,PXTrue,200,200);
  wait_dialog->initDisplay();
  
}

/* **************************************************************************      */ 
int vtkpxGUI4DSplineStackControl::InterpolateSpatially()
{
  this->BlockEditorCallback=1;
  this->AbortProcessing=0;

  int bottom=Irange((int)sliceScale[0]->getValue(),
		    splineStackSet[current_splinestack]->GetTrimBottom(),
		    splineStackSet[current_splinestack]->GetTrimTop()-1);
  
  int top=Irange((int)sliceScale[1]->getValue(),
		 splineStackSet[current_splinestack]->GetTrimBottom()+1,
		 splineStackSet[current_splinestack]->GetTrimTop());
  

  wait_dialog->showStopPrompt(PXTrue);
  this->wait_dialog->setTitle("Interpolating Spatially");
  this->wait_dialog->setText("initializing ...");
  this->wait_dialog->popupDialog();

  PXContour* cnt1=splineStackSet[current_splinestack]->GetSpline(bottom)->exporttocontour(0.8);
  PXContour* cnt2=splineStackSet[current_splinestack]->GetSpline(top)->exporttocontour(0.8);

  PXChamferDistanceMap* map1=new PXChamferDistanceMap(cnt1);
  PXChamferDistanceMap* map2=new PXChamferDistanceMap(cnt2);

  
  int start=bottom+1;
  int end=top-1;


  for (int sl=start;sl<=end;sl++)
      {
	sprintf(pxtk_buffer,"Interpolating Frame %d Slice %d",this->CurrentFrame+1,sl);
	this->wait_dialog->setText(pxtk_buffer);
	PXTkApp::executeTclCommand("after 10");
	if (this->AbortProcessing==1)
	  {
	    sl=end+1;
	  }
	else
	  {
	    float z=splineStackSet[current_splinestack]->GetSpline(sl)->getzlevel();
    
	    int w1=int(100.0*float(top-sl)/float(top-bottom)+0.5);
	    int w2=int(100.0*float(sl-bottom)/float(top-bottom)+0.5);
	    
	    PXChamferDistanceMap* map=new PXChamferDistanceMap(map1,map2,w1,w2);
	    PXContour* cntr=map->extractContour();
	    
	    int np=int(0.5+float(w1*splineStackSet[current_splinestack]->GetSpline(bottom)->getnumpoints()+
				 w2*splineStackSet[current_splinestack]->GetSpline(top)->getnumpoints())
		       /float(w1+w2));
	    splineStackSet[current_splinestack]->GetSpline(sl)->fittocontour(cntr,2.0,0.0,np,1);
	    splineStackSet[current_splinestack]->GetSpline(sl)->setzlevel(z);
	    delete cntr;
	    delete map;      
	    splineStackSet[current_splinestack]->SetSpline((PXFPBSpline*)NULL,sl);
	    splineStackSet[current_splinestack]->Modified();
	    this->UpdateSplineEditor();
	    this->UpdateDisplay();
	  }
      }

  this->wait_dialog->popdownDialog();
    
  delete map1;  delete map2;
  delete cnt1;  delete cnt2;

  this->BlockEditorCallback=0;
  this->UpdateSplineEditor();
  return 1;
}

/* **************************************************************************      */ 
int vtkpxGUI4DSplineStackControl::InterpolateTemporally(int allslices)
{
  this->BlockEditorCallback=1;
  this->AbortProcessing=0;

  int bottom=Irange((int)sliceScale[0]->getValue(),
		    splineStackSet[current_splinestack]->GetTrimBottom(),
		    splineStackSet[current_splinestack]->GetTrimTop()-1);
  
  int top=Irange((int)sliceScale[1]->getValue(),
		 splineStackSet[current_splinestack]->GetTrimBottom()+1,
		 splineStackSet[current_splinestack]->GetTrimTop());
  
  if (allslices==0)
    {
      int sl=this->SplineEditor->GetBeginSlice();
      bottom=sl;
      top=sl;
    }


  wait_dialog->showStopPrompt(PXTrue);
  this->wait_dialog->setTitle("Interpolating Temporally");
  this->wait_dialog->setText("initializing ...");
  this->wait_dialog->popupDialog();


  int beginframe=Irange((int)frameScale[0]->getValue(),1,this->NumberOfFrames)-1;
  int endframe=Irange((int)frameScale[1]->getValue(),1,this->NumberOfFrames)-1;
  int oldcurrentframe=this->GetCurrentFrame();
  
  if (this->BaseViewer!=NULL)
    {
      this->BaseViewer->SetCurrentFrame(beginframe);
      this->BaseViewer->SetCurrentFrame(endframe);
    }

  int lastframe=endframe;
  if (endframe<beginframe)
    lastframe+=this->NumberOfFrames;

  
  for (int sl=bottom;sl<=top;sl++)
    {
      PXFPBSpline *spl1=this->GetSplineStack(beginframe,current_splinestack)->GetSpline(sl);
      PXContour* cnt1=spl1->exporttocontour(0.8);
      PXFPBSpline* spl2=this->GetSplineStack(endframe,current_splinestack)->GetSpline(sl);
      PXContour* cnt2=spl2->exporttocontour(0.8);
      float z=0.5*(spl1->getzlevel()+spl2->getzlevel());

      PXChamferDistanceMap* map1=new PXChamferDistanceMap(cnt1);
      PXChamferDistanceMap* map2=new PXChamferDistanceMap(cnt2);

      for (int frame=beginframe;frame<=lastframe;frame++)
	{
	  int actualframe=frame;
	  if (actualframe>=this->NumberOfFrames)
	    actualframe-=this->NumberOfFrames;

	  if (frame!=beginframe && frame!=lastframe)
	    {
	      if (this->BaseViewer)
		this->BaseViewer->SetCurrentFrame(actualframe);

	      sprintf(pxtk_buffer,"Interpolating Slice %d Frame %d",sl,actualframe+1);
	      this->wait_dialog->setText(pxtk_buffer);
	      PXTkApp::executeTclCommand("after 10");
	      if (this->AbortProcessing==1)
		{
		  frame=lastframe;
		  sl=top+1;
		}
	      else
		{
		  int w1=int(100.0*float(lastframe-frame)/float(lastframe-beginframe)+0.5);
		  int w2=int(100.0*float(frame-beginframe)/float(lastframe-beginframe)+0.5);
		  
		  PXChamferDistanceMap* map=new PXChamferDistanceMap(map1,map2,w1,w2);
		  PXContour* cntr=map->extractContour();
		  
		  int np=int(0.5+float(w1*spl1->getnumpoints()+
				       w2*spl2->getnumpoints())
			     /float(w1+w2));
		  
		  PXFPBSpline* spl=this->GetSplineStack(actualframe,current_splinestack)->GetSpline(sl);
		  
		  spl->fittocontour(cntr,2.0,0.0,np,1);
		  spl->setzlevel(z);
		  this->GetSplineStack(actualframe,current_splinestack)->SetSpline((PXFPBSpline*)NULL,sl);
		  this->GetSplineStack(actualframe,current_splinestack)->Modified();
		  delete map;      
		  delete cntr;
		  this->UpdateSplineEditor();
		  this->UpdateDisplay();
		}
	    }
	}
      delete map1;
      delete map2;
      delete cnt1;
      delete cnt2;
    }


  this->wait_dialog->popdownDialog();
	
  if (this->BaseViewer)
    this->BaseViewer->SetCurrentFrame(oldcurrentframe);
  

  this->BlockEditorCallback=0;

  return 1;

}

/*********************************************************************/
void vtkpxGUI4DSplineStackControl::AutoSegment()
{
  if (this->AbortProcessing!=-1)
    {
      //      fprintf(stderr,"Already Processing at Work\n");
      return;
    }

  this->SplineEditor->CopyCallback();  
  wait_dialog->showStopPrompt(PXTrue);
  this->wait_dialog->setTitle("Auto Segmentation");
  this->wait_dialog->setText("initializing ...");
  this->wait_dialog->popupDialog();
  this->AbortProcessing=0;

  Tcl_CreateTimerHandler(10,vtkpxGUI4DSplineStackControl::AutoSegmentStaticWork,this);
}
/*                *****************************************      */ 
void vtkpxGUI4DSplineStackControl::AutoSegmentStaticWork(void* cl)
{
  int done=((vtkpxGUI4DSplineStackControl*)cl)->AutoSegmentWork();
  if (done==0)
    Tcl_CreateTimerHandler(10,vtkpxGUI4DSplineStackControl::AutoSegmentStaticWork,cl);
}
/*                *****************************************      */ 
int vtkpxGUI4DSplineStackControl::AutoSegmentWork()
{
  static int startingFromScratch=1;
  static int lastslice=-1;
  static int firstslice;
  static int lastincrement=1;

  if (this->AbortProcessing==1)
    {
      this->wait_dialog->popdownDialog();
      startingFromScratch=1;
      this->AbortProcessing=-1;
      return 1;
    }
    
  if (startingFromScratch==1)
    {
      int curSlice=this->SplineEditor->GetBeginSlice();
      firstslice=curSlice;
      this->wait_dialog->setText("Segmenting slice : ",curSlice);
      this->SplineEditor->CreateSnake();
      this->SplineEditor->UpdateSnake();
      if (this->AutoSegmentMode)
	this->SplineEditor->CopyCallback();
      this->UpdateFromSplineEditor();
      if (toggle_up->getState())
	lastincrement=1;
      else
	lastincrement=-1;
      lastslice=firstslice+lastincrement;
      startingFromScratch=0;
      return 0;
    }

  if (lastincrement==1)
    {
      if (lastslice<=splineStackSet[current_splinestack]->GetTrimTop() )
	{
	  this->SplineEditor->SetSlice(lastslice);
	  this->wait_dialog->setText("Segmenting slice : ",lastslice);
	  if (this->AutoSegmentMode)
	    this->SplineEditor->PasteCallback(); 
	  this->SplineEditor->CreateSnake();
	  this->SplineEditor->UpdateSnake();
	  if (this->AutoSegmentMode)
	    this->SplineEditor->CopyCallback();  
	  this->UpdateFromSplineEditor();
	  lastslice++;
	  return 0;
	}
      else
	{
	  if (this->AutoSegmentMode)
	    {
	      this->SplineEditor->SetSlice(firstslice);
	      this->SplineEditor->CopyCallback();  
	    }
	  lastslice=firstslice-1;
	  lastincrement=-1;
	  return 0;
	}
    }
  
  if (lastincrement==-1 && 
      toggle_down->getState()==PXTrue &&
      lastslice>=splineStackSet[current_splinestack]->GetTrimBottom())
    {
      this->SplineEditor->SetSlice(lastslice);
      this->wait_dialog->setText("Segmenting slice : ",this->SplineEditor->GetBeginSlice());
      if (this->AutoSegmentMode)
	this->SplineEditor->PasteCallback();
      this->SplineEditor->CreateSnake();
      this->SplineEditor->UpdateSnake();
      if (this->AutoSegmentMode)
	this->SplineEditor->CopyCallback(); 
      this->UpdateFromSplineEditor();
      lastslice=lastslice-1;
      return 0;
    }

  this->wait_dialog->popdownDialog();
  this->splineStackSet[current_splinestack]->Modified();
  this->SplineEditor->SetSlice(firstslice);
  this->UpdateSplineEditor();
  startingFromScratch=1;
  lastincrement=1;
  this->UpdateDisplay();
  this->AbortProcessing=-1;
  PXTkApp::executeTclCommand("update idletasks");
  return 1;

}
/*********************************************************************/
void vtkpxGUI4DSplineStackControl::AutoTemporalVolumeSegment(int mode)
{

  // 0 == Auto Current Slice
  // 1 == Auto All Slices
  // 2 == Batch Current Slice
  // 3 == Batch All Slices


  int bottom=splineStackSet[current_splinestack]->GetTrimBottom();
  int top=splineStackSet[current_splinestack]->GetTrimTop();
  int middle=this->SplineEditor->GetBeginSlice();

  if (mode==0 || mode == 2)
    {
      bottom=middle;
      top=middle;
    }
  else
    {
      if (toggle_up->getState()==PXFalse)
	top=middle;
      if (toggle_down->getState()==PXFalse)
	bottom=middle;
    }
    int beginframe=this->CurrentFrame;
    int currentframe=this->CurrentFrame;
    int endframe=this->NumberOfFrames+beginframe-1;
    int frameincrement=1;

    if (toggle_past->getState()==PXFalse)
      {
	endframe=this->NumberOfFrames-1;
	if (toggle_future->getState()==PXFalse)
	  endframe=beginframe;
      }
    else if (toggle_future->getState()==PXFalse)
      {
	beginframe=0;
	endframe=currentframe;
	frameincrement=-1;
      }

    if (mode==0 || mode == 1)
      this->AutoSegmentMode=1;
    else
      this->AutoSegmentMode=0;


  this->AbortProcessing=0;

  wait_dialog->showStopPrompt(PXTrue);
  this->wait_dialog->setDebugMode(PXFalse);
  this->wait_dialog->setTitle("Temporal Segmentation");
  this->wait_dialog->setText("initializing ...");
  this->wait_dialog->popupDialog();


  /*  fprintf(stderr,"Currentframe =%d frameincrement=%d beginframe=%d endframe=%d\n",
      currentframe,frameincrement,beginframe,endframe);*/

  for (int slice=bottom;slice<=top;slice++)
    {
      sprintf(pxtk_buffer,"Segmenting Slice %d Frame %d",slice,currentframe+1);
      this->wait_dialog->setText(pxtk_buffer);
      if (this->BaseViewer)
	this->BaseViewer->SetCurrentFrame(currentframe);
      this->SetCurrentFrame(currentframe);
      this->SplineEditor->CreateSnake();
      this->SplineEditor->UpdateSnake();
      if (this->AutoSegmentMode)
	this->SplineEditor->CopyCallback();
      this->UpdateFromSplineEditor();
    	
      int done=0;
      int frame=currentframe;
      while (done ==0 )
	{
	  if (frame!=currentframe)
	    {
	      int actualframe=frame;
	      if (actualframe>=this->NumberOfFrames)
		actualframe-=this->NumberOfFrames;
	      
	      sprintf(pxtk_buffer,"Segmenting Slice %d Frame %d",slice,actualframe+1);
	      this->wait_dialog->setText(pxtk_buffer);
	      if (this->BaseViewer)
		this->BaseViewer->SetCurrentFrame(actualframe);
	      this->SetCurrentFrame(actualframe);
	      PXTkApp::executeTclCommand("after 10");
	      if (this->AbortProcessing==1)
		{
		  frame=endframe+1;
		  slice=top+1;
		}
	      else
		{
		  if (this->AutoSegmentMode)
		  this->SplineEditor->PasteCallback();
		  this->SplineEditor->CreateSnake();
		  this->SplineEditor->UpdateSnake();
		  if (this->AutoSegmentMode)
		    this->SplineEditor->CopyCallback();
		  this->UpdateFromSplineEditor();
		}
	    }

	  frame+=frameincrement;
	  if (frameincrement>0)
	    {
	      if (frame>endframe)
		done=1;
	    }
	  else
	    {
	      if (frame<beginframe)
		done=1;
	    }
	}
    }


  if (this->BaseViewer)
    this->BaseViewer->SetCurrentFrame(currentframe);
  this->SetCurrentFrame(currentframe);


  wait_dialog->popdownDialog();
  wait_dialog->setDebugMode(PXFalse);
}

/*********************************************************************/

