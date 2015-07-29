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





/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

 This file is part of the PXTk library 

 Xenios Papademetris May 2000 papad@noodle.med.yale.edu

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

#include "vtkpxGUIBaseCurveControl.h"
#include "pxutil.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataWriter.h"
#include "vtkAssembly.h"
#include "vtkRenderer.h"
#include "vtkObjectFactory.h"
#include "vtkpxImageExtract.h"
#include "vtkSphereSource.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkIdentityTransform.h"
#include "vtkMath.h"
#include "vtkPoints.h"
#include "vtkTransform.h"
#include "vtkImageShiftScale.h"
#include "vtkpxUtil.h"
#include "vtkpxColorMapUtil.h"
#include "vtkpxAnalyzeImageWriter.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkLookupTable.h"
#include "vtkImageData.h"
#include "vtkProperty.h"
#include "vtkPolyData.h"

// 	$Id: vtkpxGUIBaseCurveControl.cpp,v 1.1 2002/02/20 18:06:52 Xenios Exp Xenios $	


/* -------------------------------------------------------------------------*/
char vtkpxGUIBaseCurveControl::lamstring[400];
vtkpxBaseCurve* vtkpxGUIBaseCurveControl::clipboardCollection=NULL;
/* -------------------------------------------------------------------------*/

vtkpxGUIBaseCurveControl* vtkpxGUIBaseCurveControl::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIBaseCurveControl");
  if(ret)
      {
	return (vtkpxGUIBaseCurveControl*)ret;
      }
  return new vtkpxGUIBaseCurveControl;
}

// Construct object with no children.
vtkpxGUIBaseCurveControl::vtkpxGUIBaseCurveControl()
{
  num_collections=9;
  if (clipboardCollection==NULL)
    clipboardCollection=vtkpxBaseCurve::New();

  this->transform=vtkIdentityTransform::New();
  owns_transform=PXTrue;

  for (int i=0;i<num_collections;i++)
      {
	allCollections[i]=vtkpxBaseCurve::New();
	showCollection[i]=PXTrue;
	allCollections[i]->SetPointScale(1.0);
	allCollections[i]->SetShowControls(1);
	allCollections[i]->SetShowCurve(0);
	allCollections[i]->SetClosedCurve(0);
	
	for (int j=0;j<BCC_MAX_RENDERERS;j++)
	  {
	    if (i==0)
	      {
		renderer[j]=NULL;
		highlightActor[j]=NULL;
	      }
	    landmarkActor[j][i]=NULL;
	  }
	
	landmarkFilter[i]=vtkTransformPolyDataFilter::New();
	landmarkFilter[i]->SetInput(allCollections[i]->GetOutput());
	landmarkFilter[i]->SetTransform(this->transform);
      }

  highlightData=NULL;
  highlightFilter=NULL;
  highlightIndex=-1;
  current_collection=0;
  landmarkCollection=allCollections[current_collection];
  strcpy(clipboard," ");

  backupColormap=NULL;
  currentColormap=NULL;
  edit_window=NULL;
  label_window=NULL;
  extractGUI=NULL;
  thresholdGUI=NULL;
  snakeGUI=NULL;
  edit_item=-1;

  enable_complex=PXTrue;
  enable_labels=PXTrue;
  enable_curve=PXTrue;
  enable_list=PXTrue;
  image=NULL;
  image_plane=2;
  image_frame=0;
  image_level=-1;


}

vtkpxGUIBaseCurveControl::~vtkpxGUIBaseCurveControl()
{
  for (int i=0;i<num_collections;i++)
    {
      for (int j=0;j<BCC_MAX_RENDERERS;j++)
	{
	  if (landmarkActor[j][i]!=NULL)
	    landmarkActor[i][j]->Delete();
	  if (i==0)
	    {
	      if (highlightActor[j]!=NULL)
		{
		  highlightActor[j]->Delete();
		  landmarkFilter[j]->Delete();
		}
	    }
	}
      allCollections[i]->Delete();
    }

  if (highlightData!=NULL)
    {
      highlightData->Delete();
      highlightFilter->Delete();
    }
     
  if (this->transform!=NULL)
    {
      if (owns_transform)
	this->transform->Delete();
    }

  /*if (clipboardCollection!=NULL)
    clipboardCollection->Delete();*/

}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::GetNumberOfCollections()
{
  return this->num_collections;
}

char* vtkpxGUIBaseCurveControl::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();


  vtkpxGUIComponent::Initialize(name,inside);
  //  PXTkDialog::initDisplay();

  PXTkFrame* landmarkFrame=(PXTkFrame*)this->EventManager->getMainWindow();

  landmarkFrame->bindCallback("<Delete>",204);

  // Menu Stuff
  // ----------
  PXTkFrame* menubar=new PXTkFrame(this->EventManager,landmarkFrame);
  landmarkFrame->addChildren("-side top -expand false -fill x",menubar);

  PXTkMenuButton* but1=new PXTkMenuButton(this->EventManager,menubar,"File",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but1);
  
  PXTkMenu *menu1=new PXTkMenu(this->EventManager,but1,PXFalse);
  menu1->addButton("New",100);
  menu1->addSeparator();
  menu1->addButton("Load",101);
  menu1->addButton("Save",102);
  menu1->addSeparator();
  menu1->addButton("Export to Surface File",104);
  if (enable_list)
    {
      menu1->addSeparator();
      if (enable_complex)
	{
	  //	  menu1->addButton("Save as Transformation",307);
	  menu1->addButton("Save as Object Map",310);
	  menu1->addSeparator();
	}
      menu1->addButton("Clear All Sets",105);
    }
  else
    menu1->addSeparator();
  if (this->EventParent)
      menu1->addButton("Close",3);
  else
      menu1->addButton("Exit",1);
  
  PXTkMenuButton* but2=new PXTkMenuButton(this->EventManager,menubar,"Edit",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but2);

  
  PXTkMenu *menu2=new PXTkMenu(this->EventManager,but2,PXFalse);
  if (enable_list)
      {
	menu2->addButton("Edit Point",210);
	menu2->addSeparator();
	menu2->addButton("Cut Point",201);
	menu2->addButton("Copy Point",202);
	menu2->addButton("Paste Point at End",203);
     }
  menu2->addButton("Delete Last Point",204);
  menu2->addButton("Delete All Points",100);

  PXTkMenuButton* but24=new PXTkMenuButton(this->EventManager,menubar,"Color",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but24);
  PXTkMenu *menu24=new PXTkMenu(this->EventManager,but24,PXFalse);
  menu24->addButton("Landmark Color",400);
  menu24->addButton("Highlight Color",401);


 
  if (enable_labels)
    {
      PXTkMenuButton* but3=new PXTkMenuButton(this->EventManager,menubar,"Setup",-1,PXTrue);
      menubar->addChildren("-side left -expand false ",but3);

      PXTkMenu *menu3=new PXTkMenu(this->EventManager,but3,PXFalse);
      menu3->addButton("Edit Setup File",303);
      menu3->addSeparator();
      menu3->addButton("Load Setup File",300);
      menu3->addButton("Save Setup File",301);
      menu3->addSeparator();
      menu3->addButton("Talairach Mode",302);
      //      menu3->addButton("Brainstrip Mode",304);
      //      menu3->addButton("Transform Mode",305);
      //      menu3->addButton("Articulated Mouse Mode",306);
    }
  
  PXTkMenuButton* but6=new PXTkMenuButton(this->EventManager,menubar,"Point Set",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but6);
  
  PXTkMenu *menu6=new PXTkMenu(this->EventManager,but6,PXFalse);
  menu6->addButton("Copy  Point Set",590);
  menu6->addButton("Paste Point Set",591);
  if (enable_curve)
    {
      menu6->addButton("Paste Curve (2D)",592);
      menu6->addSeparator();
      menu6->addButton("Create Circle",596);
    }

  PXTkMenuButton* but62=new PXTkMenuButton(this->EventManager,menubar,"Curve",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but62);

  PXTkMenu *menu62=new PXTkMenu(this->EventManager,but62,PXFalse);
  menu62->addButton("Area",605);
  menu62->addButton("Length",604);
  menu62->addButton("Image Stats (ROI)",606);
  menu62->addSeparator();
  menu62->addButton("Remove Loops",597);
  menu62->addButton("Smooth",602);
  menu62->addSeparator();
  menu62->addButton("Equispace",603);
  menu62->addButton("Equispace 2.0",598);
  menu62->addButton("Sample x2",600);
  menu62->addButton("Sample x8",599);
  menu62->addButton("Subsample x2",601);
  
 
  if (enable_curve)
    {
      PXTkMenuButton* but7=new PXTkMenuButton(this->EventManager,menubar,"Operation",-1,PXTrue);
      menubar->addChildren("-side left -expand false ",but7);
      
      PXTkMenu *menu7=new PXTkMenu(this->EventManager,but7,PXFalse);
      //      menu7->addButton("Threshold",612);
      menu7->addButton("Angle",615);
      //      menu7->addSeparator();
      menu7->addButton("Extract",609);
      //      menu7->addButton("Snake",610);
    }

  // Other Stuff 
  // --------------
  // Collection Properties 

  PXTkLabelFrame* checkFrame=new PXTkLabelFrame(this->EventManager,landmarkFrame,"PointSet Properties");
  landmarkFrame->addChildren("-side top -expand true -fill both -padx 2 -pady 2",checkFrame);
  checkFrame->setPackMode(PXFalse);

  curveMode=new PXTkOptionMenu(this->EventManager,checkFrame,"Landmarks",110);
  curveMode->addOption("Open   Curve");
  curveMode->addOption("Closed Curve");
  curveMode->addOption("Bounding Box");
  checkFrame->gridRowColumnChild(curveMode,0,0);
  if (allCollections[0]->GetShowCurve()==1)
    curveMode->setIndex(1);
  else
    curveMode->setIndex(0);//landmarkCollection->GetCurveMode());

  scaleMode=new PXTkOptionMenu(this->EventManager,checkFrame,"Size=0.25",111);
  for (float s=0.50;s<=4.50;s+=0.25)
      {
	sprintf(pxtk_buffer,"Size=%3.2f",s);
	scaleMode->addOption(pxtk_buffer);
      }
  int sc=int(landmarkCollection->GetPointScale()/0.25)-1;
  scaleMode->setIndex(sc);
  checkFrame->gridRowColumnChild(scaleMode,0,1);

  displayCollection=new PXTkCheckButton(this->EventManager,checkFrame,"Show",501);
  displayCollection->setState(showCollection[0]);
  checkFrame->gridRowColumnChild(displayCollection,0,2);

  message=new PXTkLabel(this->EventManager,checkFrame,"Num");
  checkFrame->gridRowColumnChild(message,1,0,1,3);
  message->configure("-relief","ridge");

  
  if (enable_labels)
    {
      currentLabel=new PXTkLabel(this->EventManager,checkFrame,"No Label");
      checkFrame->gridRowColumnChild(currentLabel,2,0,1,2);
      currentLabel->configure("-relief","ridge");
      currentLabel->configure("-pady",2);
      currentLabel->configure("-padx",2);
      currentLabel->configure("-width",40);
    }

  // List Box and Scalebar
  if (enable_list)
      {
	PXTkFrame* listFrame=new PXTkFrame(this->EventManager,checkFrame);
	checkFrame->gridRowColumnChild(listFrame,3,0,3,2);
	
	PXTkScrollbar* ybar=new PXTkScrollbar(this->EventManager,listFrame,PXTrue);
	listBox=new PXTkListBox(this->EventManager,listFrame,208);
	ybar->attachGadget(listBox);
	
	listBox->configure("-width",30);
	listBox->configure("-height",10);
	listBox->configure("-wrap","none");
	
	listFrame->addChildren("-side left -expand true -fill both",listBox);
	listFrame->addChildren("-side left -expand false -fill y",ybar);
      }

  // Control Properties
  PXTkLabelFrame* vFrame=new PXTkLabelFrame(this->EventManager,landmarkFrame,"General Properties");
  landmarkFrame->addChildren("-side bottom -expand false -fill x -padx 5 -pady 5",vFrame);
  vFrame->setPackMode(PXFalse);
  
  
  mouseMode=new PXTkOptionMenu(this->EventManager,vFrame,"Disabled  ",502);
  mouseMode->addOption("Pick Mode");
  //  mouseMode->addOption("Smooth Mode");
  mouseMode->addOption("Auto Mode");
  mouseMode->addOption("Add  Mode");
  mouseMode->addOption("Continuous Add  Mode");
  mouseMode->setIndex(3);
  vFrame->gridRowColumnChild(mouseMode,1,0);

  currentCollection=new PXTkOptionMenu(this->EventManager,vFrame,"Point Set 1",500);
  for (int i=1;i<num_collections;i++)
      {
	sprintf(lamstring,"Point Set %d",i+1);
	currentCollection->addOption(lamstring);
      }
  vFrame->gridRowColumnChild(currentCollection,1,1);


  PXTkLabel* lab=new PXTkLabel(this->EventManager,vFrame,"");
  lab->configure("-image","$::pxtclvtkpxcontrib::smalllogo ");
  vFrame->gridRowColumnChild(lab,1,2);

  /*displayMode=new PXTkOptionMenu(this->EventManager,vFrame,"Show None",501);
  displayMode->addOption("Show Current");
  displayMode->addOption("Show All");
  displayMode->setIndex(1);
  vFrame->gridRowColumnChild(displayMode,1,2);*/


  
  if (enable_labels)
    CreateLabelWindow();

  UpdateStatus();
  

  return GetWidgetName();
}

/* -------------------------------------------------------------------------*/ 
void vtkpxGUIBaseCurveControl::SetRenderer(vtkRenderer* ren,int num)
{
  num=Irange(num,0,BCC_MAX_RENDERERS-1);
  if (renderer[num]!=NULL || ren==NULL)
    return;

  renderer[num]=ren;

  
  if (highlightData==NULL)
    {
      highlightData=vtkSphereSource::New();
      highlightIndex=-1;
      highlightData->SetRadius(1.5*landmarkCollection->GetPointScale());
      highlightData->SetThetaResolution(5);
      highlightData->SetPhiResolution(5);

      highlightData->SetCenter(0.0,0.0,0.0);

      highlightFilter=vtkTransformPolyDataFilter::New();
      highlightFilter->SetInput(highlightData->GetOutput());
      highlightFilter->SetTransform(this->transform);
      //highlightData->DebugOn();
    }


  highlightData->Update();
  vtkPolyDataMapper* map2=vtkPolyDataMapper::New();
  map2->SetInput(highlightFilter->GetOutput());

  highlightActor[num]=vtkActor::New();
  highlightActor[num]->SetMapper(map2);
  highlightActor[num]->GetProperty()->SetRepresentationToWireframe();
  highlightActor[num]->GetProperty()->SetLineWidth(2.0);
  highlightActor[num]->GetProperty()->SetAmbient(1.0);
  highlightActor[num]->GetProperty()->SetDiffuse(0.0);
  highlightActor[num]->GetProperty()->SetSpecular(0.0);
  highlightActor[num]->SetVisibility(0);
  renderer[num]->AddActor(highlightActor[num]);

  map2->Delete();

  for (int i=0;i<num_collections;i++)
      {
	vtkPolyDataMapper* map=vtkPolyDataMapper::New();
	map->SetInput(landmarkFilter[i]->GetOutput());
	landmarkActor[num][i]=vtkActor::New();
	landmarkActor[num][i]->SetMapper(map);
	landmarkActor[num][i]->GetProperty()->SetLineWidth(2.0);
	/*landmarkActor[num][i]->GetProperty()->SetAmbient(1.0);
	landmarkActor[num][i]->GetProperty()->SetDiffuse(0.0);
	landmarkActor[num][i]->GetProperty()->SetSpecular(0.0);*/
	allCollections[i]->Update();
	renderer[num]->AddActor(landmarkActor[num][i]);
	map->Delete();

	if (num==0)
	  {
	    double c[3]; vtkpxSurfaceUtil::GetColor(i,c);
	    landmarkActor[num][i]->GetProperty()->SetColor(c);
	  }
	else
	  {
	    if (landmarkActor[0][i]!=NULL)
	      {
		landmarkActor[num][i]->GetProperty()->SetColor(landmarkActor[0][i]->GetProperty()->GetColor());
	      }
	  }
      }

}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::Update()
{
  highlightData->Update();
  SetHighlightDisplay(1);

  for (int j=0;j<BCC_MAX_RENDERERS;j++)
    {
      if (renderer[j]!=NULL)
	{
	  for (int i=0;i<num_collections;i++)
	    {
	      allCollections[i]->Update();
	      if (showCollection[i]==PXTrue)
		landmarkActor[j][i]->SetVisibility(1);
	      else
		landmarkActor[j][i]->SetVisibility(0);
	    }
	}
    }
  // Curve Stuff 
  UpdateDisplay();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::UpdateDisplay()
{
  if (!this->Initialized)
      return;

  if (showCollection[current_collection]==PXTrue)
    {
      highlightData->Update();
      landmarkCollection->Update();
    }
  
  if (this->EventParent!=NULL && this->CallbackNo>0)
    this->EventParent->HandleEvent(CallbackNo);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::UpdateStatus()
{
  if (!this->Initialized)
    return;
  
  if (enable_list)
    {
      int a=this->highlightIndex;
      //int a=listBox->getCurrentItem();//this->highlightIndex;
      listBox->selectItem(a);

      if (mouseMode->getIndex()>=3)
	a+=1;

      if (enable_labels)
	{
	  if (a>=0)
	    {
	      // Display Next Item
	      char line2[20];
	      if (mouseMode->getIndex()>=3)
		strcpy(line2,"Click Now for :");
	      else
		strcpy(line2,"Name:");

	      int num=labelListBox->getNumItems();
	      if (a<num)
		{
		  char* line=labelListBox->getItem(a);
		  if (strlen(line)>=2)
		    {
		      strcpy(pxtk_buffer,&line[1]);
		      pxtk_buffer[strlen(pxtk_buffer)-1]=(char)0;
		      sprintf(pxtk_buffer2,"%s %s ",line2,pxtk_buffer);
		      currentLabel->setText(pxtk_buffer2);
		    }
		  else
		    currentLabel->setText(line);
		}
	      else
		{
		  sprintf(pxtk_buffer2,"%s Point %d",line2,a+1);
		  currentLabel->setText(pxtk_buffer2);
		}
	    }
	  else
	    currentLabel->setText("No Point Selected");
	}
    }

  sprintf(pxtk_buffer,"Name=%s Num Landmarks=%d",
	  currentCollection->getLabel(current_collection),landmarkCollection->GetNumPoints());
  message->setText(pxtk_buffer);
}
/* -------------------------------------------------------------------------*/
int  vtkpxGUIBaseCurveControl::HandleEvent(int event)
{
  if (!this->Initialized)
      return TCL_OK;

  if (event<10)
      return this->EventManager->handleDirectEvent(event);

  switch(event)
      {
      case 100:
	if (landmarkCollection->GetNumPoints()>0)
	    {
	      int ok=this->EventManager->questionBox("This will delete your old landmarks. Are you sure?");
	      if (ok)
		  {
		    landmarkCollection->DeleteAllPoints(); SetHighlight(-1);

		    if (enable_list)
			listBox->clearList();
		    UpdateDisplay();
		    sprintf(pxtk_buffer2,"Point Set %d",current_collection+1);
		    currentCollection->changeLabel(current_collection,pxtk_buffer2);
		    UpdateStatus();
		    if (mouseMode->getIndex()!=0)
		      mouseMode->setIndex(3);
		  }
	    }
	break;

      case 101:
	{
	  int ok=1;
	  if (landmarkCollection->GetNumPoints()>0)
	      ok=this->EventManager->questionBox("Loading will delete your old landmarks. Are you sure?");
	  if (ok)
	      {
		char* line=this->EventManager->getOpenFilename("Load landmarks","Landmarks","*.land");
		if (line!=NULL)
		    {
		      if (strlen(line)>0)
			  LoadLandmarks(line);
		    }
	      }
	}
	break;
	
      case 102:
	{
	  char* line=this->EventManager->getSaveFilename("Save Landmarks","points.land","Landmarks","*.land");
	  if (line!=NULL)
	      {
		if (strlen(line)>0)
		    {
		      strcpy(pxtk_buffer2,line);
		      if(landmarkCollection->Save(line))
			  {
			    sprintf(pxtk_buffer2,"%s.%d",PXTkApp::getFilenameTail(pxtk_buffer2),current_collection+1);
			    currentCollection->changeLabel(current_collection,pxtk_buffer2);
			    UpdateStatus();
			  }
		      else
			  {
			    sprintf(pxtk_buffer,"Failed to save landmarks %d in %s",current_collection+1,line);
			    this->EventManager->messageBox(pxtk_buffer,"Save Failure");
			  }
		    }
	      }
	}
	break;

      case 103:
	{
	  int ok=1;
	  if (landmarkCollection->GetNumPoints()>0)
	    ok=this->EventManager->questionBox("Importing seeds will delete your old landmarks. Are you sure?");
	  if (ok)
	    {
	      char* line=this->EventManager->getOpenFilename("Import Seed File","Seed Files","*.dat");
	      if (line!=NULL)
		{
		  if (strlen(line)>0)
		    LoadSeedFile(line);
		}
	    }
	}
	break;
	
  
      case 104:
	{
	  char* line=this->EventManager->getSaveFilename("Save as Surface File","surface.vtk","Surface Files","*.vtk");
	  if (line!=NULL)
	      {
		if (strlen(line)>0)
		  {
		    strcpy(pxtk_buffer2,line);
		    vtkPolyDataWriter* writer=vtkPolyDataWriter::New();
		    writer->SetFileName(pxtk_buffer2);
		    vtkPolyData* newp=vtkPolyData::New();
		    landmarkCollection->ExportToPolyData(newp,-1.0);
		    writer->SetInput(newp);
		    writer->SetFileTypeToASCII();
		    writer->Update();
		    writer->Delete();
		    newp->Delete();
		  }
	      }
	}
	break;
  

      case 105:
	{
	  int ok=1;
	  ok=this->EventManager->questionBox("Loading will delete ALL your old landmarks in all sets. Are you sure?");
	  if (ok)
	      {
		for (int i=0;i<num_collections;i++)
		    {
		      allCollections[i]->DeleteAllPoints(); SetHighlight(-1);
		      sprintf(pxtk_buffer2,"Point Set %d",i+1);
		      currentCollection->changeLabel(i,pxtk_buffer2);
		    }
		
		if (enable_list)
		    listBox->clearList();
		UpdateDisplay();
		UpdateStatus();
		if (mouseMode->getIndex()!=0)
		  mouseMode->setIndex(3);
	      }
	}
	break;

      case 110:
	{
	  int c=curveMode->getIndex();
	  if (c!=2)
	    {
	      landmarkCollection->SetClosedCurve(0);
	    }
	  else
	    {
	      landmarkCollection->SetClosedCurve(1);
	      if (mouseMode->getIndex()>=3)
		mouseMode->setIndex(1);
	    }

	  if (c==2 || c==1)
	    landmarkCollection->SetShowCurve(1);
	  else
	    landmarkCollection->SetShowCurve(0);

	  if (c==3)
	    landmarkCollection->SetShowBoundingBox(1);
	  else
	    landmarkCollection->SetShowBoundingBox(0);

	  landmarkCollection->SetShowControls(1);
	  landmarkCollection->Update();
	}
	break;
	
      case 111:
	landmarkCollection->SetPointScale(0.25+0.25*float(scaleMode->getIndex()));
	highlightData->SetRadius(1.5*landmarkCollection->GetPointScale());
	break;
	
      case 201:
      case 202:
      case 203:
      case 204:
	ClipboardCommand(event-200);
	UpdateDisplay();
	break;


      case 208: // ListBox Callback
	if (mouseMode->getIndex()>=3)
	  mouseMode->setIndex(1);
	SetHighlight(listBox->getCurrentItem());
	UpdateStatus();
	break;

      case 209:
	HandleEditWindowCallback();
	break;

      case 210: 
	if (edit_window==NULL)
	    CreateEditWindow(209);
	UpdateEditWindow();
	break;

      case 300:
	{
	  char* line=this->EventManager->getOpenFilename("Load Setup File","Setup File","*.set");
	  if (line!=NULL)
	      {
		if (strlen(line)>0)
		    LoadSetupFile(line);
	      }
	}
	break;

      case 301:
	{
	  char* line=this->EventManager->getSaveFilename("Save Setup File","setup.set","Setup Files","*.set");
	  if (line!=NULL)
	      {
		if (strlen(line)>0)
		    SaveSetupFile(line);
	      }
	}
	break;


      case 302:
	SetTalarachSetup();
	break;
	
      case 303: // popupLabel Callback
	label_window->popupDialog();
	break;

      case 320: // labelListBox Callback
      case 321: // update Callback
      case 322: // add Callback
      case 323: // insert Callback
      case 324: // deleteLabel Callback
      case 325: // deleteAll Callback
	HandleLabelWindowCallback(event-320);
	break;

      case 304:
	SetBrainstripSetup();
	break;

      case 305:
	SetTransformSetup();
	break;

      case 306:
	SetArticulatedMouseSetup();
	break;

      case 307:
	{
	  char* line=this->EventManager->getSaveFilename("Save as Transformation","reslice.matr","Matrix Files","*.matr");
	  if (line!=NULL)
	      {
		if (strlen(line)>0)
		    {
		      strcpy(pxtk_buffer2,line);
		      SaveTransform(line);
		    }
	      }
	}
	break;

      case 310:
	if (image!=NULL)
	  {
	    char* line=this->EventManager->getSaveFilename("Save as Object Map","objectmap.hdr","Analyze Files","*.hdr");
	    if (line!=NULL)
	      {
		if (strlen(line)>0)
		  {
		    strcpy(pxtk_buffer,line);

		    vtkImageShiftScale* shiftscale=vtkImageShiftScale::New();
		    shiftscale->SetOutputScalarTypeToUnsignedChar();
		    shiftscale->SetInput(image);
		    shiftscale->SetScale(0.0);
		    shiftscale->Update();
		    
		    vtkImageData* outim=vtkImageData::New();
		    outim->DeepCopy(shiftscale->GetOutput());
		    shiftscale->Delete();
		    
		    for (int i=0;i<num_collections;i++)
		      allCollections[i]->AddToImage(outim,float(i+1));
		    
		    vtkpxAnalyzeImageWriter* wr=vtkpxAnalyzeImageWriter::New();
		    wr->SetInput(outim);
		    wr->Save(pxtk_buffer);
		    wr->Delete();
		    outim->Delete();
		  }
	      }
	  }
	break;

      case 400:
	{
	  double c[3];
	  if (renderer[0]!=NULL)
	    {
	      landmarkActor[0][current_collection]->GetProperty()->GetColor(c);
	      //landmarkCollection->GetColor(c);
	      int a=this->EventManager->d_colorBox("Landmark Color",c[0],c[1],c[2]);

	      if (a==1)
		{
		  for (int j=0;j<BCC_MAX_RENDERERS;j++)
		    {
		      if (landmarkActor[j][current_collection]!=NULL)
			landmarkActor[j][current_collection]->GetProperty()->SetColor(c);
		    }
		}
	    }
	  else
	    fprintf(stderr,"Renderer[0]=NULL\n");
	  break;
	}

      case 401:
	{
	  double c[3]; 	  
	  if (renderer[0]!=NULL)
	    {
	      highlightActor[0]->GetProperty()->GetColor(c);
	      int a=this->EventManager->d_colorBox("Highlight Color",c[0],c[1],c[2]); 	    
	      if (a==1)
		{
		  for (int j=0;j<BCC_MAX_RENDERERS;j++)
		    {
		      if (highlightActor[j]!=NULL)
			highlightActor[j]->GetProperty()->SetColor(c);
		    }
		}
	    }
	  else
	    fprintf(stderr,"Renderer[0]=NULL\n");
	  break;
	}

      case 500:
	SetCurrentCollection(currentCollection->getIndex());
	if (mouseMode->getIndex()!=0)
	    {
	      if (landmarkCollection->GetNumPoints()==0)
		  mouseMode->setIndex(3);
	      else  if (mouseMode->getIndex()>=3)
		  mouseMode->setIndex(1);
	    }
	break;
	      

      case 501:
	showCollection[current_collection]=displayCollection->getState();
	Update();
	break;

      case 502:
	if (mouseMode->getIndex()>=3)
	    {
	      int index=this->highlightIndex;// this->highlightIndex;
	      int numitems=landmarkCollection->GetNumPoints();
	      if (numitems>0 && index<0)
		  {
		    SetHighlight(0);
		    UpdateStatus();
		  }
	    }
	break;
	
      case 590:
	clipboardCollection->Copy(landmarkCollection);
	break;
	
      case 591:
	if (clipboardCollection->GetNumPoints()>0)
	    SetCollection(clipboardCollection,-1);
	break;

      case 592:
	{
	  PXContour* cntr=new PXContour();
	  clipboardCollection->StoreInContour(cntr,image_plane);
	  SetCollection(cntr,-1,image_plane,image_level);
	  delete cntr;
	}
	break;

      case 595:
	landmarkCollection->Equispace(1.0);
	//landmarkCollection->SetPointScale(0.5);
	UpdateListFromCurrentCollection();
	break;

      case 596:
	landmarkCollection->CreateCircle(image_plane,image_level);
	UpdateListFromCurrentCollection();
	break;

      case 597:
	landmarkCollection->RemoveIntersections(PXTrue,image_plane,image_level);
	UpdateListFromCurrentCollection();
	break;

      case 598:
	landmarkCollection->Equispace(2.0);
	//landmarkCollection->SetPointScale(0.5);
	UpdateListFromCurrentCollection();
	break;

      case 599:
	landmarkCollection->Sample(8);
	UpdateListFromCurrentCollection();
	break;

      case 600:
	landmarkCollection->Sample(2);
	UpdateListFromCurrentCollection();
	break;

      case 601:
	landmarkCollection->SubSample(2);
	UpdateListFromCurrentCollection();
	break;

      case 602:
	landmarkCollection->Smooth(0.33);
	UpdateListFromCurrentCollection();
	break;

      case 603:
	landmarkCollection->Equispace(-1.0);
	UpdateListFromCurrentCollection();
	break;

      case 604:
	{
	  float len=landmarkCollection->GetLength();
	  sprintf(pxtk_buffer,"Curve %d Length=%8.1f mm\n",current_collection+1,len);
	  PXTkApp::printToConsole(pxtk_buffer);
	  this->EventManager->messageBox(pxtk_buffer,"Curve Length ...");
	  break;
	}

      case 605:
	{
	  int plane=landmarkCollection->GetCurrentInternalPlane();//Irange(image_plane,0,2);
	  if (image_plane==-1)
	      plane=2;

	  float area=landmarkCollection->GetPlaneArea(plane);
	  sprintf(pxtk_buffer,"Curve %d Area=%8.1f (plane=%d)\n",current_collection+1,area,plane);
	  PXTkApp::printToConsole(pxtk_buffer);
	  this->EventManager->messageBox(pxtk_buffer,"Curve Area ...");
	  break;
	}

      case 606:
	{
	  float mean,sigma;
	  int plane,level;
	  landmarkCollection->GetCurrentInternalPlaneAndLevel(plane,level);//Irange(image_plane,0,2);

	  int num=landmarkCollection->ExtractVOIProperties(image,plane,level,image_frame,mean,sigma);
	  float area=landmarkCollection->GetAreaFromPixels(num,plane);
	  if(thresholdGUI!=NULL)
	      {
		if (thresholdGUI->isOpen())
		    {
		      float val=mean+2.0*sigma;
		      sprintf(pxtk_buffer2,"%6.1f",val);
		      entryThreshold->setText(pxtk_buffer2);
		    }
	      }

	  if (num>=0)
	      {
		sprintf(pxtk_buffer,"Curve %d Mean Intensity=%8.1f Stdev=%8.1f (pixels>0=%d voxels, area=%5.2f mm2)\n(plane=%d,level=%d)",current_collection+1,mean,sigma,num,area,plane,level);
		PXTkApp::printToConsole(pxtk_buffer);
		this->EventManager->messageBox(pxtk_buffer,"Image Stats ...");
	      }
	  else
	      {
		sprintf(pxtk_buffer,"Curve %d Mean Intensity=%8.1f Stdev=%8.1f (Using Circle Mode)\n",
			current_collection+1,mean,sigma);
		PXTkApp::printToConsole(pxtk_buffer);
		this->EventManager->messageBox(pxtk_buffer,"Image Stats ...");
		UpdateListFromCurrentCollection();
	      }
	  
	}
	break;

      
      case 607:
	DoExtract(1);
	break;

      case 608:
	DoExtract(2);
	break;

      case 609:
	DoExtract(0);
	break;

      case 610:
	DoSnake(0);
	break;

      case 611:
	DoSnake(1);
	break;

      case 612:
	DoThreshold(0);
	break;

      case 613:
	DoThreshold(1);
	break;

      case 614:
	DoThreshold(2);
	break;

      case 615: // Angle
	{
	  double x1[3],x2[3];
	  int np=landmarkCollection->GetNumPoints();
	  if (np<2)
	    {
	      this->EventManager->messageBox("Need 2 points to calculate angle","Error!");
	    }
	  else
	    {
	      landmarkCollection->GetPoints()->GetPoint(np-2,x1);
	      landmarkCollection->GetPoints()->GetPoint(np-1,x2);
	      double sp[3]; landmarkCollection->GetSpacing(sp);

	      for (int ia=0;ia<=2;ia++)
		{
		  x2[ia]=(x2[ia]-x1[ia])*sp[ia];
		  x1[ia]=fabs(x2[ia]);
		}

	      int minindex=0;
	      if (x1[1]<x1[0] && x1[1]<x1[2])
		minindex=1;
	      else if (x1[2]<x1[0] && x1[2]<x1[1])
		minindex=2;
	      
	      int a1=Icyclic(minindex+1,3);
	      int a2=Icyclic(minindex+2,3);
	      float angle=atan2(x2[a2],x2[a1])*180/M_PI;
	      
	      switch (minindex)
		{
		case 0:
		  sprintf(pxtk_buffer," X-Rotation %5.1f",angle);
		  break;
		case 1:
		  sprintf(pxtk_buffer," Y-Rotation %5.1f",angle);
		  break;
		case 2:
		  sprintf(pxtk_buffer," Z-Rotation %5.1f",angle);
		  break;
		}
	      this->EventManager->messageBox(pxtk_buffer,"Rotation Result (use -1*answer)");
	    }
	}
	break;
      }
  

  if (event>=595 && event<=614)
    {
      if (mouseMode->getIndex()>=3)
	  mouseMode->setIndex(1);
    }

  this->UpdateDisplay();
  /*  if (this->EventParent!=NULL && this->CallbackNo>0)
      this->EventParent->HandleEvent(CallbackNo);*/

  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
void  vtkpxGUIBaseCurveControl::SetSpacingOrigin(double sp[3],double ori[3])
{
  for (int i=0;i<num_collections;i++)
    {
      allCollections[i]->RescaleOriginSpacing(ori,sp);
      //allCollections[i]->SetOrigin(ori);
      allCollections[i]->Update();
    }

  clipboardCollection->RescaleOriginSpacing(ori,sp);
  //clipboardCollection->SetSpacing(sp);
  //clipboardCollection->SetOrigin(ori);

  UpdateListFromCurrentCollection();

}

/* -------------------------------------------------------------------------*/
vtkpxBaseCurve* vtkpxGUIBaseCurveControl::GetCollection(int index)
{
  if (index==-1)
      index=current_collection;

  return allCollections[Irange(index,0,num_collections-1)];
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::GetCurrentIndex()
{
  return current_collection;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::SetCollection(vtkpxBaseCurve* land,int index)
{
  if (index==-1)
    index=current_collection;
  
  index=Irange(index,0,num_collections-1);
  //  fprintf(stderr,"Setting Collection %d\n",index);

  allCollections[index]->Copy(land);
  allCollections[index]->Modified();

  if (index==current_collection)
      {
	UpdateListFromCurrentCollection();
	if (landmarkCollection->GetNumPoints()==0)
	  mouseMode->setIndex(3);
	else  if (mouseMode->getIndex()>=3)
	  mouseMode->setIndex(1);
      }
  
  /*if (allCollections[index]->GetCurveActor()!=NULL)
    {
    allCollections[index]->DrawLandmarks(0);*/
  allCollections[index]->Update();
  //}
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::SetCollection(PXContour* cntr,int index,int plane,int level)
{
  if (cntr==NULL)
      return;
  
  if (index==-1)
      index=current_collection;

  if (plane==-1)
    plane=2;
  plane=Irange(plane,0,2);

  index=Irange(index,0,num_collections-1);
  //fprintf(stderr,"Changing point set %d (np=%d) --> ",index,allCollections[index]->GetNumPoints());
  allCollections[index]->SetFromContour(cntr,plane,level);

  //  fprintf(stderr,"to %d (np=%d)\n",cntr->GetNumPoints(),allCollections[index]->GetNumPoints());
  
  if (index==current_collection)
      {
	UpdateListFromCurrentCollection();
	if (landmarkCollection->GetNumPoints()==0)
	    mouseMode->setIndex(3);
	else  if (mouseMode->getIndex()>=3)
	    mouseMode->setIndex(1);
      }

  /*if (allCollections[index]->GetCurveActor()!=NULL)
      {
	if (allCollections[index]->GetVisibility()==1)
	    {
	      allCollections[index]->DrawLandmarks(0);
	      allCollections[index]->DrawCurve();
	    }
	    }*/
  allCollections[index]->Update();
 
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::DisableCurve()
{
  if (!this->Initialized)
    enable_curve=PXFalse;
}

void vtkpxGUIBaseCurveControl::DisableComplex()
{
  if (!this->Initialized)
    enable_complex=PXFalse;
}

void vtkpxGUIBaseCurveControl::DisableLabels()
{
  if (!this->Initialized)
    {
      enable_labels=PXFalse;
      for (int i=0;i<num_collections;i++)
	{
	  allCollections[i]->SetShowControls(1);
	  allCollections[i]->SetShowCurve(1);
	}
    }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::DisableList()
{
  if (!this->Initialized)
      {
	enable_list=PXFalse;
	enable_labels=PXFalse;
	for (int i=0;i<num_collections;i++)
	  {
	    allCollections[i]->SetShowControls(1);
	    allCollections[i]->SetShowCurve(1);
	  }
      }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::SetImage(vtkImageData* img,int plane,int level,int fr)
{
  if (img==NULL)
    return;

  if (image!=img)
      {
	image=img;
	image->GetPointData()->GetScalars()->GetRange(image_range);
	if (extractGUI!=NULL)
	    {
	      if (extractGUI->isOpen())
		extractGUI->popdownDialog();
	    }
	SetSpacingOrigin(image->GetSpacing(),image->GetOrigin());
      }
  SetPlaneLevel(plane,level);
  SetFrame(fr);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::SetPlaneLevel(int plane,int level)
{
  //  fprintf(stderr,"Setting Plane and Level=%d,%d\n",plane,level);
  image_plane=plane;
  image_level=level;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::SetFrame(int fr)
{
  image_frame=fr;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::SetLookupTable(vtkLookupTable* lookuptable)
{
  currentColormap=lookuptable;
  if (backupColormap==NULL)
    backupColormap=vtkLookupTable::New();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::ClipboardCommand(int mode)
{
  // modes = { 1=Cut , 2=Copy , 3= Paste , 4=Delete }3

  //int  current=this->highlightIndex;
  int current=this->highlightIndex;
  
  if (current==-1 && (mode==1 || mode==2 || mode==4))
      return;

  if ((mode==1 || mode==2) && enable_list)
      strcpy(clipboard,listBox->getItem(current));

  if ((mode==1 || mode==4))
      {
	int oldc=current;
	landmarkCollection->DeletePoint(current); 
	if (enable_list)
	    listBox->clearItem(current);
	      
	if (oldc>0)
	  SetHighlight(oldc-1);
	else
	  SetHighlight(0);
      }

  if ((mode==3 && strlen(clipboard)>10) && enable_list)
      {
	float x,y,z;
	ExtractCoordinates(clipboard,x,y,z);
	AddListLandmark(x,y,z,-1,0);
	SetHighlight(landmarkCollection->GetNumPoints()-1);
      }
  UpdateStatus();
}
/* -------------------------------------------------------------------------*/      
void vtkpxGUIBaseCurveControl::AddListLandmark(float x,float y,float z,
					       int index,int scaled,int listonly)
{
  //fprintf(stderr,"\n +++ Adding List Landmark (%f,%f,%f) index=%d scaled=%d listonly=%d\n",x,y,z,index,scaled,listonly);
  double sp[3]; landmarkCollection->GetSpacing(sp);
  double ori[3]; landmarkCollection->GetOrigin(ori);

  if (index==-1)
    {
      if (!listonly)
	{
	  landmarkCollection->AddPoint(x,y,z,scaled);
	  landmarkCollection->Modified();
	  landmarkCollection->Update();
	}
      if (enable_list)
	{
	  
	  if (scaled==0)
	    sprintf(pxtk_buffer,"%6.2f,%6.2f,%6.2f",x,y,z); 
	  else
	    sprintf(pxtk_buffer,"%6.2f,%6.2f,%6.2f",(x-ori[0])/sp[0],(y-ori[1])/sp[1],(z-ori[2])/sp[2]); 
	  listBox->addItem(pxtk_buffer);
	  PXTkApp::executeTclCommand("update idletasks");
	}
    }
  else
    {
      if (!listonly)
	{
	  landmarkCollection->SetPoint(index,x,y,z,scaled);    
	  landmarkCollection->Modified();
	  landmarkCollection->Update();
	}
      
      if (enable_list)
	{
	  if (scaled==0)
	    sprintf(pxtk_buffer,"%6.2f,%6.2f,%6.2f",x,y,z); 
	  else
	    sprintf(pxtk_buffer,"%6.2f,%6.2f,%6.2f",(x-ori[0])/sp[0],(y-ori[1])/sp[1],(z-ori[2])/sp[2]); 
	  listBox->clearItem(index);
	  listBox->addItem(pxtk_buffer,index);
	  PXTkApp::executeTclCommand("update idletasks");
	}
    }
  
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::HandleClickedPoint(float x,float y,float z,int scaled,int state)
{
  if (!this->EventManager->isOpen() && !this->EventManager->isInside())
    return 0;

  //  fprintf(stderr,"x=%.1f,y=%.1f z=%.1f scaled=%d state=%d\n",x,y,z,scaled,state);

  int mode=mouseMode->getIndex();
  
  if (mode==0 || state>=3)
      return 0;
  
  int index=this->highlightIndex;//listBox->getCurrentItem();
  int numitems=landmarkCollection->GetNumPoints();//listBox->getNumItems();
  
  /*fprintf(stderr,"Mousemode=%d , index=%d, numitems=%d coord=(%f,%f,%f) state=%d\n",
    mode,index,numitems,x,y,z,state);*/

  switch (mode)
    {
    case 4:  // Continuous Add Mode 
      switch(state)
	{
	case 0:
	  break;
	case 1:
	  AddListLandmark(x,y,z,-1,scaled,0);
	  /*index=landmarkCollection->GetNumPoints()-1;
	  if (enable_list)
	  SetHighlight(index);*/
	  break;
	case 2:
	  SetHighlight(index);
	  if (enable_list)
	    {
	      sprintf(pxtk_buffer,"%s see end",listBox->getWidgetName());
	      PXTkApp::executeTclCommand(pxtk_buffer);
	    }
	  UpdateStatus();
	  break;
	}
      return 1;
      break;
      

    case 3:  // Add Mode 
      switch(state)
	{
	case 0:
	  AddListLandmark(x,y,z,-1,scaled,0);
	  index=landmarkCollection->GetNumPoints()-1;
	  //if (enable_list)
	  SetHighlight(index);
	  //fprintf(stderr,"Current Highlight Index=%d\n",this->highlightIndex);
	  break;
	case 1:
	  AddListLandmark(x,y,z,index,scaled,0);
	  //	  if (enable_list)
	  SetHighlight(index);
	  break;
	case 2:
	  AddListLandmark(x,y,z,index,scaled,0);
	  SetHighlight(index);
	  if (enable_list)
	    {

	      sprintf(pxtk_buffer,"%s see end",listBox->getWidgetName());
	      PXTkApp::executeTclCommand(pxtk_buffer);
	    }
	  UpdateStatus();
	  break;
	}
      return 1;
      break;
      
    case 1: // Pick Mode 
      if (numitems<1)
	return 0;
      switch (state)
	{
	case 0:
	  {
	    double lx[3];
	    lx[0]=x;lx[1]=y;lx[2]=z;
	    int found=landmarkCollection->FindClosestPoint(lx,landmarkCollection->GetPointScale()*1.2,scaled);
	    if (found>-1)
	      {
		if (enable_list)
		  listBox->selectItem(found);
		SetHighlight(found);
	      }
	    else 
	      {
		if (enable_list)		    
		  listBox->selectItem(-1);
		SetHighlight(-1);
	      }
	    break;
	  }
	case 1:
	  if (index>=0)
	    {
	      AddListLandmark(x,y,z,index,scaled,0);
	      SetHighlight(index);
	    }
	    break;
	case 2:
	  if (index>=0)
	    {
	      AddListLandmark(x,y,z,index,scaled,0);
	      if (enable_list)
		SetHighlight(index);
	      UpdateStatus();
	    }
	  break;
	}
      return 1;
      break;
      
    case 2: // Edit Mode 
      if (index<0)
	return 0;
      switch(state)
	{
	case 0:
	  AddListLandmark(x,y,z,index,scaled,0);
	  listBox->selectItem(index);
	  break;
	case 1:
	  AddListLandmark(x,y,z,index,scaled,0);
	  SetHighlight(index);
	  break;
	case 2:
	  AddListLandmark(x,y,z,index,scaled,0);
	  if (index<numitems-1)
	    {
	      index++;
	    }
	  else if (enable_labels)
	    {
	      if (index<labelListBox->getNumItems())
		mouseMode->setIndex(3);
	    }
	  SetHighlight(index);
	  UpdateStatus();
	  break;
	}
      return 1;
    }
  return 0;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::LoadLandmarks(const char* fname)
{
  strcpy(pxtk_buffer2,fname);
  int ok=landmarkCollection->Load(fname);
  if (!ok)
      {
	sprintf(pxtk_buffer,"Failed to load landmarks %d from %s",current_collection+1,fname);
	this->EventManager->messageBox(pxtk_buffer,"Load Failure");
      }
  else
    {
      sprintf(pxtk_buffer2,"%s.%d",PXTkApp::getFilenameTail(pxtk_buffer2),current_collection+1);
      currentCollection->changeLabel(current_collection,pxtk_buffer2);
      
      if (image!=NULL)
	landmarkCollection->RescaleOriginSpacing(image->GetOrigin(),image->GetSpacing());

      UpdateListFromCurrentCollection();
      landmarkCollection->Update();
    }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::SaveSeedFile(const char* fname)
{
  strcpy(pxtk_buffer2,fname);
  FILE* fout=fopen(fname,"w");
  if (fout)
    {
      for (int i=0;i<landmarkCollection->GetNumPoints();i++)
	    {
	      double x[3];
	      landmarkCollection->GetPoints()->GetPoint(i,x);
	      fprintf(fout,"%6.1f %6.1f %6.1f 1.5 1.5 1.5\n",x[0],x[1],x[2]);
	    }
      fclose(fout);
    }
  else
    {
      sprintf(pxtk_buffer,"Failed to export landmarks %d to seed file %s",current_collection+1,fname);
      this->EventManager->messageBox(pxtk_buffer,"Export Failure");
    }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::LoadSeedFile(const char* fname)
{
  strcpy(pxtk_buffer2,fname);
  gzFile fin=gzsuffixopen(fname,"rb");
  if (fin)
    {
      landmarkCollection->DeleteAllPoints(); SetHighlight(-1);

      float x,y,z,tx,ty,tz;
      while (gzgets(fin,pxtk_buffer2,100))
	{
	  sscanf(pxtk_buffer2,"%f %f %f %f %f %f",&x,&y,&z,&tx,&ty,&tz);
	  landmarkCollection->AddPoint(x,y,z,0);
	}
      gzclose(fin);
      UpdateListFromCurrentCollection();
      landmarkCollection->Update();
    }
  else
    {
      sprintf(pxtk_buffer,"Failed to import landmarks %d from seed file %s",current_collection+1,fname);
      this->EventManager->messageBox(pxtk_buffer,"Import Failure");
    }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::ExtractCoordinates(const char* line,float& x,float& y,float& z)
{
  sscanf(line,"{%f,%f,%f}",&x,&y,&z);
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::SetCurrentCollection(int num)
{
  num=Irange(num,0,num_collections-1);
  if (num==current_collection)
      return;
  current_collection=num;

  landmarkCollection=allCollections[current_collection];  
  displayCollection->setState(showCollection[current_collection]);
  Update();

  int sc=int(landmarkCollection->GetPointScale()/0.25)-1;
  scaleMode->setIndex(sc);

  int c=landmarkCollection->GetShowCurve();
  int m=landmarkCollection->GetClosedCurve();
  int b=landmarkCollection->GetShowBoundingBox();

  if (b==1)
    {
      curveMode->setIndex(3);
    }
  else
    {
      if (c==0)
	{
	  curveMode->setIndex(0);
	}
      else
	{
	  if (m==1)
	    curveMode->setIndex(2);
	  else
	    curveMode->setIndex(1);
	}
    }

  UpdateListFromCurrentCollection();
  if (landmarkCollection->GetNumPoints()>0)
    SetHighlight(0);
  else
    SetHighlight(-1);
  UpdateStatus();

}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::UpdateListFromCurrentCollection()
{
  //  curveMode->setIndex(landmarkCollection->GetCurveMode());
  int c=landmarkCollection->GetShowCurve();
  int m=landmarkCollection->GetClosedCurve();
  int b=landmarkCollection->GetShowBoundingBox();

  if (b==1)
    {
      curveMode->setIndex(3);
    }
  else
    {
      if (c==0)
	{
	  curveMode->setIndex(0);
	}
      else
	{
	  if (m==1)
	curveMode->setIndex(2);
	  else
	    curveMode->setIndex(1);
	}
    }

  int sc=int(landmarkCollection->GetPointScale()/0.25)-1;
  scaleMode->setIndex(sc);

  if (enable_list)
      {
	listBox->clearList();

	if (landmarkCollection->GetNumPoints()>0)
	    {
	      for (int i=0;i<landmarkCollection->GetNumPoints();i++)
		  {
		    double x[3];
		    landmarkCollection->GetPoint(i,x[0],x[1],x[2]);
		    AddListLandmark(x[0],x[1],x[2],i,0,1);
		  }
	      SetHighlight(0);
	    }
      }
  UpdateStatus();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::CreateEditWindow(int editcallback)
{
  if (edit_window!=NULL || enable_list==PXFalse)
      return;

  PXTkFrame* landmarkFrame=(PXTkFrame*)this->EventManager->getMainWindow();
  
  edit_window=new PXTkDialog(this->EventManager,landmarkFrame,"Edit Point",PXFalse);
  edit_window->initDisplay();
  PXTkFrame* editFrame=(PXTkFrame*)edit_window->getMainWindow();
  editFrame->setPackMode(PXFalse);
  
  PXTkLabel* lab2=new PXTkLabel(this->EventManager,editFrame,"Coord (x,y,z)");
  editFrame->gridRowColumnChild(lab2,1,0);
  
  for (int kk=0;kk<=2;kk++)
      {
	coord[kk]=new PXTkEntry(this->EventManager,editFrame);
	coord[kk]->configure("-width",8);
	editFrame->gridRowColumnChild(coord[kk],0,kk+1);
      }

  PXTkButton* upd=new PXTkButton(this->EventManager,editFrame,"Update",editcallback);
  editFrame->gridRowColumnChild(upd,1,0,1,2);

  PXTkButton* b1=new PXTkButton(edit_window,editFrame,"Close",3);
  editFrame->gridRowColumnChild(b1,1,2);
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::UpdateEditWindow()
{
  if (enable_list==PXFalse)
      return TCL_OK;

  edit_item=listBox->getCurrentItem();
  if (edit_item<0)
      return TCL_OK;

  char* line=listBox->getItem(edit_item);
  float x[3];
  ExtractCoordinates(line,x[0],x[1],x[2]);
  
  for (int kk=0;kk<=2;kk++)
      {
	sprintf(pxtk_buffer,"%6.2f",x[kk]);
	coord[kk]->setText(pxtk_buffer);
      }

  edit_window->popupDialog();
  edit_window->setModality(PXTrue);

  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::HandleEditWindowCallback()
{
  if (edit_item<0 || enable_list==PXFalse)
      return TCL_OK;

  float x[3];
  for (int i=0;i<=2;i++)
      {
	char*a =coord[i]->getText();
	x[i]=atof(a);
      }

  edit_window->setModality(PXFalse);
  AddListLandmark(x[0],x[1],x[2],edit_item,0,0);
  SetHighlight(edit_item);
  edit_window->setModality(PXTrue);

  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::LoadSetupFile(const char* fname)
{
  strcpy(lamstring,fname);
  gzFile fin=gzsuffixopen(fname,"rb");
  if (fin)
      {
	gzgets(fin,pxtk_buffer,100);
	if (gzfindstringinstring(pxtk_buffer,"#Landmark Setup File")==0)
	    {
	      gzclose(fin);
	      return 0;
	    }
	gzgets(fin,pxtk_buffer,100); 	
	gzgets(fin,pxtk_buffer,100);
	int num;
	sscanf(pxtk_buffer,"%d",&num);
	gzgets(fin,pxtk_buffer,100); 	

	labelListBox->clearList();
	
	for (int i=0;i<num;i++)
	    {
	      gzgets(fin,pxtk_buffer,100);
	      pxtk_buffer[strlen(pxtk_buffer)-1]=(char)0; // eat end of-line
	      labelListBox->addItem(pxtk_buffer,-1);
	    }
	gzclose(fin);

	int num2=(landmarkCollection->GetNumPoints());
	if (num2<num)
	    {
	      for (int i=num2;i<num;i++)
		  AddListLandmark(0.0,0.0,0.0,-1,0,0);
	    }
	SetHighlight(0);

	if (mouseMode->getIndex()>=3)
	    mouseMode->setIndex(2);
	
	sprintf(lamstring,"Loaded  setupfile (%d)  from %s \n",num,lamstring);
	PXTkApp::printToConsole(lamstring);

	UpdateStatus();
	return 1;
      }
  sprintf(pxtk_buffer,"Failed to load setup file from %s",fname);
  this->EventManager->messageBox(pxtk_buffer,"Load Failure");

  return 0;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::SaveSetupFile(const char* fname)
{
  strcpy(lamstring,fname);
  FILE* fout=fopen(fname,"w");
  if (fout)
      {
	fprintf(fout,"#Landmark Setup File\n");
	fprintf(fout,"#Number of Landmarks\n%d\n",labelListBox->getNumItems());
	fprintf(fout,"#Landmark Names\n");
	
	for (int i=0;i<labelListBox->getNumItems();i++)
	    {
	      char* line=labelListBox->getItem(i);
	      if (strlen(line)>=2)
		  {
		    strcpy(pxtk_buffer,&line[1]);
		    pxtk_buffer[strlen(pxtk_buffer)-1]=(char)32;
		    fprintf(fout,"%s\n",pxtk_buffer);
		  }
	      else
		  fprintf(fout,"%s\n",labelListBox->getItem(i));
	    }
	fclose(fout);
	sprintf(lamstring,"Saved Setup File (%d)  in %s \n",labelListBox->getNumItems(),lamstring);
	PXTkApp::printToConsole(lamstring);
	return 1;
      }

  sprintf(pxtk_buffer,"Failed to save setup file in %s",fname);
  this->EventManager->messageBox(pxtk_buffer,"Save Failure");
  return 0;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::SetTalarachSetup()
{
  int ok=1;
  if (landmarkCollection->GetNumPoints()>0)
    ok=this->EventManager->questionBox("Setup will delete ALL your old landmarks in all sets. Are you sure?");
  if (!ok)
    return 0;

  landmarkCollection->DeleteAllPoints(); 
  labelListBox->clearList();
  labelListBox->addItem("AC     ",-1);
  labelListBox->addItem("PC     ",-1);
  labelListBox->addItem("Bottom Z",-1);
  labelListBox->addItem("Top    Z",-1);
  labelListBox->addItem("Bound 1",-1);
  labelListBox->addItem("Bound 2",-1);
  labelListBox->addItem("Bound 3",-1);
  labelListBox->addItem("Bound 4",-1);
  labelListBox->addItem("Bound 5",-1);
  labelListBox->addItem("Bound 6",-1);
  labelListBox->addItem("Bound 7",-1);
  labelListBox->addItem("Bound 8",-1);
  labelListBox->addItem("Bound 9",-1);

  SetHighlight(-1);
  mouseMode->setIndex(3);
  curveMode->setIndex(3,PXTrue);
  UpdateListFromCurrentCollection();
  landmarkCollection->Update(); 
  UpdateStatus();
  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::SetBrainstripSetup()
{
  labelListBox->clearList();
  labelListBox->addItem("Left 1",-1);
  labelListBox->addItem("Left 2",-1);
  labelListBox->addItem("Left 3",-1);
  labelListBox->addItem("Right 1",-1);
  labelListBox->addItem("Right 2",-1);
  labelListBox->addItem("Right 3",-1);
  labelListBox->addItem("Stem   ",-1);

  int num=(landmarkCollection->GetNumPoints());
  if (num<7)
    {
      for (int i=num;i<7;i++)
	AddListLandmark(0.0,0.0,0.0,-1,0,0);
    }
  
  SetHighlight(0);
  mouseMode->setIndex(2);
  UpdateStatus();
  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::SetArticulatedMouseSetup(int hasfeet,int hashindlimbs,int hasforelimbs,
						       int hashead,int taildivisions,int spinedivisions)
{
  int ok=0;
  if (landmarkCollection->GetNumPoints()>0)
    ok=this->EventManager->questionBox("Would you like to delete your current points?");

  //  fprintf(stderr,"In Setting Articulated Mouse Setup\n");
  
  if (ok==1)
    landmarkCollection->DeleteAllPoints(); 
  SetHighlight(-1);
  labelListBox->clearList();
  labelListBox->addItem("Body Center",-1);
  labelListBox->addItem("Body X-Axis",-1);
  labelListBox->addItem("Body Y-Axis",-1);

  if (hashindlimbs>0)
    {
      labelListBox->addItem("Left Hip Center",-1);
      labelListBox->addItem("Left Hip X-Axis",-1);
      labelListBox->addItem("Left Hip Y-Axis",-1);
      labelListBox->addItem("Left Knee Center",-1);
      labelListBox->addItem("Left Knee X-Axis",-1);
      labelListBox->addItem("Left Knee Y-Axis",-1);
      if (hasfeet)
	{
	  labelListBox->addItem("Left Ankle Center",-1);
	  labelListBox->addItem("Left Ankle X-Axis",-1);
	  labelListBox->addItem("Left Ankle Y-Axis",-1);
	}
      labelListBox->addItem("Right Hip Center",-1);
      labelListBox->addItem("Right Hip X-Axis",-1);
      labelListBox->addItem("Right Hip Y-Axis",-1);
      labelListBox->addItem("Right Knee Center",-1);
      labelListBox->addItem("Right Knee X-Axis",-1);
      labelListBox->addItem("Right Knee Y-Axis",-1);
      if (hasfeet)
	{
	  labelListBox->addItem("Right Ankle Center",-1);
	  labelListBox->addItem("Right Ankle X-Axis",-1);
	  labelListBox->addItem("Right Ankle Y-Axis",-1);
	}
    }

  if (hasforelimbs>0)
    {
      labelListBox->addItem("Left Shoulder Center",-1);
      labelListBox->addItem("Left Shoulder X-Axis",-1);
      labelListBox->addItem("Left Shoulder Y-Axis",-1);
      labelListBox->addItem("Left Elbow Center",-1);
      labelListBox->addItem("Left Elbow X-Axis",-1);
      labelListBox->addItem("Left Elbow Y-Axis",-1);
      if (hasfeet)
	{
	  labelListBox->addItem("Left Wrist Center",-1);
	  labelListBox->addItem("Left Wrist X-Axis",-1);
	  labelListBox->addItem("Left Wrist Y-Axis",-1);
	}
      labelListBox->addItem("Right Shoulder Center",-1);
      labelListBox->addItem("Right Shoulder X-Axis",-1);
      labelListBox->addItem("Right Shoulder Y-Axis",-1);
      labelListBox->addItem("Right Elbow Center",-1);
      labelListBox->addItem("Right Elbow X-Axis",-1);
      labelListBox->addItem("Right Elbow Y-Axis",-1);
      if (hasfeet)
	{
	  labelListBox->addItem("Right Wrist Center",-1);
	  labelListBox->addItem("Right Wrist X-Axis",-1);
	  labelListBox->addItem("Right Wrist Y-Axis",-1);
	}
    }

  char line[200],line2[100];
  if (taildivisions>0)
    {
      for (int i=0;i<taildivisions;i++)
	{
	  sprintf(line2,"Tail-%d ",i+1);
	  sprintf(line,"%s Center",line2); 	  labelListBox->addItem(line,-1);
	  sprintf(line,"%s X-Axis",line2); 	  labelListBox->addItem(line,-1);
	  sprintf(line,"%s Y-Axis",line2); 	  labelListBox->addItem(line,-1);
	}
    }

  if (spinedivisions>0)
    {
      for (int i=0;i<spinedivisions;i++)
	{
	  sprintf(line2,"Spine-%d ",i+1);
	  sprintf(line,"%s Center",line2); 	  labelListBox->addItem(line,-1);
	  sprintf(line,"%s X-Axis",line2); 	  labelListBox->addItem(line,-1);
	  sprintf(line,"%s Y-Axis",line2); 	  labelListBox->addItem(line,-1);
	}
    }

  if (hashead>0)
    {
      labelListBox->addItem("Neck Center",-1);
      labelListBox->addItem("Neck X-Axis",-1);
      labelListBox->addItem("Neck Y-Axis",-1);
    }

  mouseMode->setIndex(3);  
  curveMode->setIndex(1,PXTrue);
  UpdateListFromCurrentCollection();
  landmarkCollection->Update(); 
  UpdateStatus();

  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::SetTransformSetup()
{
  labelListBox->clearList();
  labelListBox->addItem("Origin",-1);
  labelListBox->addItem("X-Axis",-1);
  labelListBox->addItem("Y-Axis",-1);
  labelListBox->addItem("Z-Axis",-1);
  int num=(landmarkCollection->GetNumPoints());
  if (num<4)
    {
      for (int i=num;i<4;i++)
	AddListLandmark(0.0,0.0,0.0,-1,0,0);
    }
  
  SetHighlight(0);
  mouseMode->setIndex(2);
  UpdateStatus();
  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::SaveTransform(const char* fname)
{
  strcpy(pxtk_buffer,fname);


  if (landmarkCollection->GetNumPoints()<2)
    {
      this->EventManager->messageBox("Not Enough Points (<2) to Generate Tranformation","No Transformation Generated!");
      return;
    }

  if (this->image==NULL)
    {
      this->EventManager->messageBox("Not Image to Generate Tranformation","No Transformation Generated!");
      return;
    }
  double origin[3],sp[3],ori[3];
  int dim[3];

  image->GetSpacing(sp);
  image->GetOrigin(ori);
  image->GetDimensions(dim);

  for (int ic=0;ic<=2;ic++)
    origin[ic]=0.5*(float(dim[ic])*sp[ic]+ori[ic]);
  

  vtkTransform* tr=vtkTransform::New();
  tr->Identity();
  tr->PostMultiply();



  vtkpxBaseCurve* tmp=vtkpxBaseCurve::New();
  tmp->Copy(landmarkCollection);
  tmp->Compact();


  double x0[3],x1[3],x2[3],x3[3],midpoint[3];
  int kk;
  for (kk=0;kk<=2;kk++)
    midpoint[kk]=0.0;
 
  vtkPoints* tmppoints=tmp->GetPoints();

  //  fprintf(stderr,"Origin = %.1f %.1f %.1f \n",origin[0],origin[1],origin[2]);
  
  if (landmarkCollection->GetNumPoints()>2)
    {
      tmppoints->GetPoint(0,x0);
      tmppoints->GetPoint(1,x1);
      tmppoints->GetPoint(2,x2);


      for (int ia=0;ia<=2;ia++)
	{
	  midpoint[ia]=(x0[ia]+x1[ia]+x2[ia])/3.0;
	  x1[ia]=x1[ia]-x0[ia];
	  x2[ia]=x2[ia]-x0[ia];
	}
      
      vtkMath::Normalize(x2);
      vtkMath::Normalize(x1);
      vtkMath::Cross(x1,x2,x3);
    }
  else
    {
      tmppoints->GetPoint(0,x0);
      tmppoints->GetPoint(1,x2);



      int bestdim=0;
      float mindist=fabs(x0[0]-x2[0]);
      for (int ia=1;ia<=2;ia++)
	{
	  float dist=fabs(x0[ia]-x2[ia]);
	  if (dist<mindist)
	    bestdim=ia;
	}
      
      for (int dim=0;dim<=2;dim++)
	{
	  if (dim!=bestdim)
	    x1[dim]=0.0;
	  else
	    x1[dim]=1.0;
	}
      
      {
	for (int ia=0;ia<=2;ia++)
	  {
	    midpoint[ia]=(x0[ia]+x2[ia])/2.0;
	    x2[ia]=x2[ia]-x0[ia];
	  }
      }
      vtkMath::Normalize(x2);

      vtkMath::Cross(x1,x2,x3);

      /*fprintf(stderr,"Bestdim=%d\n",bestdim);
	fprintf(stderr,"x1 = %.1f %.1f %.1f \n",x1[0],x1[1],x1[2]);
	fprintf(stderr,"x2 = %.1f %.1f %.1f \n",x2[0],x2[1],x2[2]);
	fprintf(stderr,"x3 = %.1f %.1f %.1f \n",x3[0],x3[1],x3[2]);*/

    }



  tr->Translate(-origin[0],-origin[1],-origin[2]);

  double matr[16];
  for (kk=0;kk<15;kk++)
    matr[kk]=0.0;

  // Set the 3x3 Section with the vectors x2,x3,x1 ::: Done 
  for (int i=0;i<=2;i++)
    {
      matr[0*4+i]=x1[i];
      matr[1*4+i]=x2[i];
      matr[2*4+i]=x3[i];
    }
  matr[15]=1.0;
  tr->Concatenate(matr);


  //tr->Translate(origin[0],origin[1],origin[2]);
  tr->Translate(midpoint[0],midpoint[1],midpoint[2]);

  FILE* fout=fopen(pxtk_buffer,"w");
  if (fout)
    {
      for(int i=0;i<=3;i++)
	{
	  for (int j=0;j<=3;j++)
	    fprintf(fout,"%7.3f ",tr->GetMatrix()->GetElement(i,j));
	  fprintf(fout,"\n");
	}
      fclose(fout);
    }
  tmp->Delete();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::CreateLabelWindow()
{
  if (!enable_labels)
    return;

  PXTkFrame* landmarkFrame=(PXTkFrame*)(this->EventManager->getMainWindow());
  
  label_window=new PXTkDialog(this->EventManager,landmarkFrame,"Label Editor",PXFalse);
  label_window->initDisplay();
  PXTkFrame* labelFrame=(PXTkFrame*)label_window->getMainWindow();
  
  PXTkFrame* listFrame=new PXTkFrame(this->EventManager,labelFrame);
  labelFrame->addChildren("-side top -expand true -fill both",listFrame);  
  PXTkFrame* buttonFrame=new PXTkFrame(this->EventManager,labelFrame);
  labelFrame->addChildren("-side top -expand false -fill x",buttonFrame);  
  buttonFrame->setPackMode(PXFalse);

  PXTkScrollbar* ybar=new PXTkScrollbar(this->EventManager,listFrame,PXTrue);
  labelListBox=new PXTkListBox(this->EventManager,listFrame,320);
  ybar->attachGadget(labelListBox);
  
  labelListBox->configure("-width",25);
  labelListBox->configure("-height",10);
  labelListBox->configure("-wrap","none");
  labelListBox->configure("-relief","ridge");
  
  listFrame->addChildren("-side left -expand true -fill both",labelListBox);
  listFrame->addChildren("-side left -expand false -fill y",ybar);

  PXTkButton *b1=new PXTkButton(this->EventManager,buttonFrame,"Add",322);
  PXTkButton *b2=new PXTkButton(this->EventManager,buttonFrame,"Insert",323);
  PXTkButton *b3=new PXTkButton(this->EventManager,buttonFrame,"Delete",324);
  PXTkButton *b4=new PXTkButton(this->EventManager,buttonFrame,"Delete All",325);
  buttonFrame->gridRowColumnChild(b1,1,0);
  buttonFrame->gridRowColumnChild(b2,1,1);
  buttonFrame->gridRowColumnChild(b3,1,2);
  buttonFrame->gridRowColumnChild(b4,1,3);

  label_entry=new PXTkEntry(this->EventManager,buttonFrame);
  buttonFrame->gridRowColumnChild(label_entry,0,0,1,3);
  PXTkButton *b0=new PXTkButton(this->EventManager,buttonFrame,"Update",321);
  buttonFrame->gridRowColumnChild(b0,0,3);

  PXTkButton* b5=new PXTkButton(label_window,buttonFrame,"Close",3);
  buttonFrame->gridRowColumnChild(b5,2,1,1,2);
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::HandleLabelWindowCallback(int option)
{
  if (!enable_labels)
      return TCL_OK;

  static int oldindex=-1;
  char* entrytext=label_entry->getText();
  
  switch (option)
      {
      case 0: // List Callback
	{
	  int index=labelListBox->getCurrentItem();
	  oldindex=index;
	  if (index>=0)
	    SetHighlight(index);
	  char* line=labelListBox->getItem(index);
	  if (strlen(line)>=2)
	      {
		strcpy(pxtk_buffer,&line[1]);
		pxtk_buffer[strlen(pxtk_buffer)-1]=(char)0;
		label_entry->setText(pxtk_buffer);
	      }
	  else
	      label_entry->setText(line);
	}
	break;
	
      case 1: // Update
	if (oldindex>=0)
	    {
	      labelListBox->clearItem(oldindex);
	      labelListBox->addItem(entrytext,oldindex);
	      labelListBox->selectItem(oldindex);
	    }
	break;
	
      case 2: // Add
	labelListBox->addItem(entrytext,-1);
	break;
	
      case 3: // Insert
	if (oldindex>=0)
	    {
	      labelListBox->addItem(entrytext,oldindex);
	      labelListBox->selectItem(oldindex);
	    }
	break;
	
      case 4: // Delete
	if (oldindex>=0)
	    {
	      labelListBox->clearItem(oldindex);
	      label_entry->setText("");
	      oldindex=-1;
	    }
	break;
	
      case 5:
	if (labelListBox->getNumItems()>0)
	    {
	      int ok=this->EventManager->questionBox("This will delete all your labels. Are you sure?");
	      if (ok)
		  {
		    labelListBox->clearList();
		    label_entry->setText("");
		    oldindex=-1;
		  }
	    }
	break;
      }
  UpdateStatus();
  return TCL_OK;
}


int vtkpxGUIBaseCurveControl::DoExtract(int apply)
{
  if (image==NULL)
      return 0;

  if (apply==0)
      {
	if (extractGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)(this->EventManager->getMainWindow());
	      extractGUI=new PXTkDialog(this->EventManager,polyFrame,"Extract Controls",PXFalse);
	      extractGUI->initDisplay();
	      
	      PXTkFrame* frame=(PXTkFrame*)extractGUI->getMainWindow();
	      extractType=new PXTkOptionMenu(extractGUI,frame,"Auto Threshold",-1);
	      extractType->addOption("Manual Threshold");
	      extractType->setIndex(0);

	      thresholdScale=new PXTkArrowScale(extractGUI,frame,"Threshold",-1,PXTrue);
	      thresholdScale->setRange(0.0,255.0);
	      thresholdScale->setValue(128.0);
	      thresholdScale->setIncrement(1);
	      thresholdScale->setResolution(1);
	      thresholdScale->setLengthWidth(120,8);

	      medianScale=new PXTkArrowScale(extractGUI,frame,"Median Radius",-1,PXTrue);
	      medianScale->setRange(0.0,4.0);
	      medianScale->setValue(0.0);
	      medianScale->setIncrement(1.0);
	      medianScale->setResolution(1.0);
	      medianScale->setLengthWidth(120,8);

	      sigmaScale=new PXTkArrowScale(extractGUI,frame,"Sigma",-1,PXTrue);
	      sigmaScale->setRange(0.0,5.0);
	      sigmaScale->setValue(0.0);
	      sigmaScale->setIncrement(0.5);
	      sigmaScale->setResolution(0.1);
	      sigmaScale->setDecimalPoints(1);
	      sigmaScale->setLengthWidth(120,8);

	      PXTkFrame* fr2=new PXTkFrame(extractGUI,frame);
	      PXTkButton* b1=new PXTkButton(this->EventManager,fr2,"Extract",607);
	      //PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Method2",608);
	      PXTkButton* b3=new PXTkButton(extractGUI,fr2,"Close",3);
	      frame->addChildren("-side top -expand true -fill x",extractType,thresholdScale->getMainWindow(),
				 medianScale->getMainWindow(),sigmaScale->getMainWindow(),fr2);
	      fr2->addChildren("-side left -expand true -fill x",b1/*,b2*/,b3);
	    }
	
	thresholdScale->setRange(image_range[0],image_range[1]);
	thresholdScale->setValue(Frange(thresholdScale->getValue(),image_range[0],image_range[1]));
	extractGUI->popupDialog();
	return 0;
      }

  if (extractGUI==NULL)
      {
	DoExtract(0);
	extractGUI->popdownDialog();
      }


  int ok=0;

  if (extractType->getIndex()==0)
      {
	float threshold=-200000.0;

	ok=landmarkCollection->ExtractImageContour(image,image_plane,image_level,image_frame,
						   threshold,
						   (int)medianScale->getValue(),sigmaScale->getValue());
	if (ok)
	    thresholdScale->setValue(threshold);
      }
  else
      {
	float threshold=thresholdScale->getValue();
	ok=landmarkCollection->ExtractImageContour(image,image_plane,image_level,image_frame,threshold,
						   (int)medianScale->getValue(),sigmaScale->getValue());
      }

  if (ok)
      {
	if (mouseMode->getIndex()>=3)
	    mouseMode->setIndex(1);
	sprintf(pxtk_buffer2,"Extr%1d_%d.%d",image_plane,image_level,current_collection+1);
	currentCollection->changeLabel(current_collection,pxtk_buffer2);
	UpdateListFromCurrentCollection();
      }
  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::DoSnake(int apply)
{
  if (image==NULL)
      return 0;

  if (apply==0)
      {
	if (snakeGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)(this->EventManager->getMainWindow());
	      snakeGUI=new PXTkDialog(this->EventManager,polyFrame,"Snake Controls",PXFalse);
	      snakeGUI->initDisplay();
	      PXTkFrame* frame=(PXTkFrame*)snakeGUI->getMainWindow();

	      snakeEdgeWeight=new PXTkArrowScale(snakeGUI,frame,"Edge Weight",-1,PXTrue);
	      snakeEdgeWeight->setRange(0.0,1.0);
	      snakeEdgeWeight->setIncrement(0.05);
	      snakeEdgeWeight->setResolution(0.01);
	      snakeEdgeWeight->setDecimalPoints(2);
	      snakeEdgeWeight->setLengthWidth(120,8);
	      snakeEdgeWeight->setValue(0.5);

	      snakeSmoothWeight=new PXTkArrowScale(snakeGUI,frame,"Smoothness Weight",-1,PXTrue);
	      snakeSmoothWeight->setRange(0.0,1.0);
	      snakeSmoothWeight->setIncrement(0.05);
	      snakeSmoothWeight->setResolution(0.01);
	      snakeSmoothWeight->setDecimalPoints(2);
	      snakeSmoothWeight->setLengthWidth(120,8);
	      snakeSmoothWeight->setValue(0.3);

	      snakeBaloon=new PXTkArrowScale(snakeGUI,frame,"Baloon Weight",-1,PXTrue);
	      snakeBaloon->setValue(0.0);
	      snakeBaloon->setResolution(0.01);
	      snakeBaloon->setDecimalPoints(3);
	      snakeBaloon->setRange(-0.5,0.5);
	      snakeBaloon->setIncrement(0.05);
	      snakeBaloon->setLengthWidth(120,8);

	      snakeSigma=new PXTkArrowScale(snakeGUI,frame,"Sigma",-1,PXTrue);
	      snakeSigma->setRange(0.0,4.0);
	      snakeSigma->setIncrement(0.5);
	      snakeSigma->setResolution(0.1);
	      snakeSigma->setDecimalPoints(1);
	      snakeSigma->setLengthWidth(120,8);
	      snakeSigma->setValue(1.0);

	      snakeIterations=new PXTkArrowScale(snakeGUI,frame,"Iterations",-1,PXTrue);
	      snakeIterations->setRange(1,300);
	      snakeIterations->setIncrement(10);
	      snakeIterations->setResolution(1);
	      snakeIterations->setDecimalPoints(0);
	      snakeIterations->setLengthWidth(120,8);
	      snakeIterations->setValue(50);

	      snakeTolerance=new PXTkArrowScale(snakeGUI,frame,"Tolerance",-1,PXTrue);
	      snakeTolerance->setResolution(0.001);
	      snakeTolerance->setDecimalPoints(3);
	      snakeTolerance->setRange(0.0,0.5);
	      snakeTolerance->setIncrement(0.025);
	      snakeTolerance->setLengthWidth(120,8);
	      snakeTolerance->setValue(0.05);

	      snakeStep=new PXTkArrowScale(snakeGUI,frame,"Step",-1,PXTrue);
	      snakeStep->setRange(0.0,3.0);
	      snakeStep->setIncrement(0.5);
	      snakeStep->setResolution(0.1);
	      snakeStep->setDecimalPoints(2);
	      snakeStep->setLengthWidth(120,8);
	      snakeStep->setValue(1.0);


	      PXTkFrame* fr2=new PXTkFrame(snakeGUI,frame);
	      PXTkButton* b1=new PXTkButton(this->EventManager,fr2,"Snake",611);
	      PXTkButton* b3=new PXTkButton(snakeGUI,fr2,"Close",3);
	      frame->addChildren("-side top -expand true -fill x",
				 snakeEdgeWeight->getMainWindow(),
				 snakeBaloon->getMainWindow(),
				 snakeSmoothWeight->getMainWindow(),
				 snakeSigma->getMainWindow(),
				 snakeIterations->getMainWindow());
	      frame->addChildren("-side bottom -expand true -fill x",
				 fr2,
				 snakeTolerance->getMainWindow(),
				 snakeStep->getMainWindow());
	      fr2->addChildren("-side left -expand true -fill x",b1,b3);
	    }
	
	/*snakeEdgeWeight->setRange(image_range[0],image_range[1]);
	  snakeEdgeWeight->setValue(Frange(snakeEdgeWeight->getValue(),image_range[0],image_range[1]));*/
	snakeGUI->popupDialog();
	return 0;
      }


  if (snakeGUI==NULL)
      {
	DoSnake(0);
	snakeGUI->popdownDialog();
      }

  if (apply==1)
      {
	float res=snakeTolerance->getValue();
	float w_img=snakeEdgeWeight->getValue();
	float w_bal=snakeBaloon->getValue();
	float w_smo=snakeSmoothWeight->getValue();
	float sigma=snakeSigma->getValue();
	float stp=snakeStep->getValue();
	int   iter=(int)snakeIterations->getValue();

	int plane,level;
	landmarkCollection->GetCurrentInternalPlaneAndLevel(plane,level);//Irange(image_plane,0,2);


	vtkImageData* en_out=landmarkCollection->UpdateSnake(image,NULL,
								    plane,level,image_frame,
								    res,w_img,w_bal,w_smo,
								    sigma,stp,iter);
	en_out->Delete();
	
	if (mouseMode->getIndex()>=3)
	    mouseMode->setIndex(1);
	UpdateListFromCurrentCollection();
      }

  return 1;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl_thresholdStats(vtkImageData* img,int plane,int level,int frame,float threshold,int& numin,int& total)
{
  vtkpxImageExtract* extr=vtkpxImageExtract::New();
  extr->SetInput(img);
  extr->SetCurrentPlane(plane);
  extr->SetSliceNo(level);
  extr->SetFrame(frame);
  extr->Update();

  vtkImageData* image=extr->GetOutput();
  
  numin=0,total=0;

  vtkDataArray* scal=image->GetPointData()->GetScalars();

  /*fprintf(stderr,"Image Dimensions %d x %d x%d (%d x %d) plane=%d level=%d\n",dim[0],dim[1],dim[2],
	  image->getWidth(),image->getHeight(),plane,level);*/


  for (int ia=0;ia<scal->GetNumberOfTuples();ia++)
      {
	total++;
	float px=scal->GetComponent(ia,0);
	if (px>threshold)
	    numin++;
      }
  
  /*fprintf(stderr,"Pixels with Intensity >%f = %d (total=%d) percentage=%5.2f \n",
	  threshold,numin,total,100.0*float(numin)/float(total));*/

  extr->Delete();
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIBaseCurveControl::DoThreshold(int apply)
{
  if (image==NULL)
    return 0;

  if (apply==0)
      {
	if (thresholdGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)(this->EventManager->getMainWindow());
	      thresholdGUI=new PXTkDialog(this->EventManager,polyFrame,"Threshold Controls",PXFalse);
	      thresholdGUI->initDisplay();
	      PXTkFrame* frame=(PXTkFrame*)thresholdGUI->getMainWindow();
	      frame->setPackMode(PXFalse);
	      
	      PXTkLabel* label=new PXTkLabel(thresholdGUI,frame,"Threshold:");
	      entryThreshold=new PXTkEntry(thresholdGUI,frame);
	      entryThreshold->setText("128.0");
	      
	      frame->gridRowColumnChild(label,0,0);
	      frame->gridRowColumnChild(entryThreshold,0,1,1,2);

	      PXTkButton* b1=new PXTkButton(this->EventManager,frame,"Threshold",613);
	      PXTkButton* b2=new PXTkButton(this->EventManager,frame,"Undo",614);
	      PXTkButton* b3=new PXTkButton(thresholdGUI,frame,"Close",3);
	      frame->gridRowColumnChild(b1,1,0);
	      frame->gridRowColumnChild(b2,1,1);
	      frame->gridRowColumnChild(b3,1,2);
	    }
	
	float mean,sigma;
	int plane=Irange(image_plane,0,2);
	if (image_plane==-1)
	    plane=2;
	
	landmarkCollection->ExtractVOIProperties(image,image_plane,image_level,image_frame,mean,sigma);
	float val=mean+2.0*sigma;
	sprintf(pxtk_buffer2,"%6.1f",val);
	entryThreshold->setText(pxtk_buffer2);
	thresholdGUI->popupDialog();
	return 0;
      }


  if (thresholdGUI==NULL)
      {
	DoThreshold(0);
	thresholdGUI->popdownDialog();
      }

  if (apply==1)
      {
	float threshold=atof(entryThreshold->getText());
	int numin,total;
	
	vtkpxGUIBaseCurveControl_thresholdStats(image,image_plane,image_level,image_frame,
						threshold,numin,total);
	if (total==0)
	    total=-1;
	
	if (currentColormap!=NULL && backupColormap!=NULL)
	    {
	      vtkpxColorMapUtil::CopyLookupTable(currentColormap,backupColormap);
	      
	      double range[2];
	      currentColormap->GetTableRange(range);
	      int numc=currentColormap->GetNumberOfColors();
	      
	      for (int i=0;i<numc;i++)
		  {
		    double dat[4];
		    currentColormap->GetTableValue(i,dat);
		    float val=(float(i)/float(numc))*(range[1]-range[0])+range[0];
		    if (val<threshold)
			dat[3]=0.0;
		    currentColormap->SetTableValue(i,dat);
		  }

	      sprintf(pxtk_buffer,"Slice =%d Threshold =%6.2f Pixels = %d/%d %5.2f",
		      image_level,threshold,numin,total,float(numin)/float(total)*100.0);
	      PXTkApp::printToConsole(pxtk_buffer);
	      this->EventManager->messageBox(pxtk_buffer,"Threhold Stats ...");
	      UpdateListFromCurrentCollection();
  	    }
	
      }
  
  if (apply==2)
      if (currentColormap!=NULL && backupColormap!=NULL)
	vtkpxColorMapUtil::CopyLookupTable(backupColormap,currentColormap);
  
  return 1;
}


/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::SetHighlightDisplay(int disp)
{
  int flag=1;
  if (disp==0 || highlightIndex==-1 || displayCollection->getState()==PXFalse)
    flag=0;


  for (int j=0;j<BCC_MAX_RENDERERS;j++)
    {
      if (renderer[j]!=NULL)
	{
	  if (highlightActor[j]->GetVisibility() != flag )
	    highlightActor[j]->SetVisibility(flag);
	}
    }
}	  
/* -------------------------------------------------------------------------*/
void vtkpxGUIBaseCurveControl::SetHighlight(int numh)
{

  if (highlightData==NULL || landmarkCollection==NULL)
    {
      this->highlightIndex=-1;
      SetHighlightDisplay(0);
      return;
    }

  int np=landmarkCollection->GetNumPoints();
  
  if (numh<0 || np<1)
    {
      highlightData->SetRadius(0.01);
      highlightData->Update();
      SetHighlightDisplay(0);
      this->highlightIndex=-1;
      return;
    }

  numh=Irange(numh,0,np-1);

  double sp[3]; landmarkCollection->GetSpacing(sp);
  double ori[3]; landmarkCollection->GetOrigin(ori);

  numh=Irange(numh,0,np-1);
  this->highlightIndex=numh;

  double x[3];
  landmarkCollection->GetPoints()->GetPoint(numh,x);
  for (int ia=0;ia<=2;ia++)
    x[ia]=x[ia]*sp[ia]+ori[ia];

  highlightData->SetCenter(x);
  highlightData->SetRadius(1.5*landmarkCollection->GetPointScale());
  highlightData->Update();
  SetHighlightDisplay(1);
}

void vtkpxGUIBaseCurveControl::SetTransform(vtkAbstractTransform* newtransform)
{
  if (newtransform==NULL)
    return;

  if (this->transform!=NULL)
    {
      if (owns_transform)
	this->transform->Delete();
    }

  this->transform=newtransform;
  owns_transform=PXFalse;

    for (int i=0;i<num_collections;i++)
    {
      if (landmarkFilter[i]!=NULL)
	{
	  landmarkFilter[i]->SetTransform(this->transform);
	  landmarkFilter[i]->Update();
	}
    }

  if (highlightFilter!=NULL)
    {
      highlightFilter->SetTransform(this->transform);
      highlightFilter->Update();
    }
}

void vtkpxGUIBaseCurveControl::SetClipboardCollection(vtkpxBaseCurve* data,int scaled)
{
  if (data==NULL)
    return;

  if (scaled==0)
    {
      clipboardCollection->Copy(data);
      return;
    }

  clipboardCollection->DeleteAllPoints();
  for (int i=0;i<data->GetNumPoints();i++)
    {
      double x[3];
      data->GetPoints()->GetPoint(i,x);
      clipboardCollection->AddPoint(x[0],x[1],x[2],1);
    }
}

void vtkpxGUIBaseCurveControl::SetClipboardCollection(vtkPoints* data)
{
  if (data==NULL)
    return;

  clipboardCollection->SetFromPoints(data);
}

