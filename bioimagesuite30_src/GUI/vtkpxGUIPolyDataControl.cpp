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




#include "pxutil.h"
#include "vtkActor.h"
#include "vtkAppendPolyData.h"
#include "vtkCellData.h"
#include "vtkCleanPolyData.h"
#include "vtkClipPolyData.h"
#include "vtkContourFilter.h"
#include "vtkDecimatePro.h"
#include "vtkDelaunay3D.h"
#include "vtkGeometryFilter.h"
#include "vtkIdList.h"
#include "vtkIdentityTransform.h"
#include "vtkImageData.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkbisImageResample.h"
#include "vtkImageThreshold.h"
#include "vtkImplicitVolume.h"
#include "vtkLinearSubdivisionFilter.h"
#include "vtkLookupTable.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkPolyDataReader.h"
#include "vtkPolyDataWriter.h"
#include "vtkTexture.h"
#include "vtkTextureMapToSphere.h"
#include "vtkTextureMapToCylinder.h"
#include "vtkTransformTextureCoords.h"
#include "vtkPNGReader.h"
#include "vtkProperty.h"
#include "vtkQuadricClustering.h"
#include "vtkSmoothPolyDataFilter.h"
#include "vtkSphereSource.h"
#include "vtkThresholdPoints.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTriangleFilter.h"
#include "vtkUnstructuredGridToPolyDataFilter.h"
#include "vtkWindowedSincPolyDataFilter.h"
#include "vtkpxGUICallback.h"
#include "vtkpxGUIPolyDataControl.h"
#include "vtkpxMapActivationToPolyData.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkpxTstackReader.h"
#include "vtkpxUtil.h"

// *********************************************
// ******* vtkpxGUIPolyDataControl Class *******
// *********************************************


vtkPolyData* vtkpxGUIPolyDataControl::clipboardPolyData=NULL;
char vtkpxGUIPolyDataControl::clipboardName[200];


vtkpxGUIPolyDataControl* vtkpxGUIPolyDataControl::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIPolyDataControl");
  if(ret)
      {
	return (vtkpxGUIPolyDataControl*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIPolyDataControl;
}


vtkpxGUIPolyDataControl::vtkpxGUIPolyDataControl()
{
  num_surfaces=10;
  current_surface=0;

  has_surface=PXFalse;
  simple_mode=PXFalse;
  save_binary=PXTrue;
  this->CurrentImage=NULL;

 
  previousPolyData=NULL;
  tempPolyData=NULL;
  //clipboardPolyData=NULL;

  internalTransform=vtkIdentityTransform::New();
  owns_transform=PXTrue;  

  num_renderers=4;

  for (int i=0;i<num_surfaces;i++)
      {
	polyData[i]=NULL;
	displayPolyData[i]=(i==0);
	polyDataFilter[i]=NULL;
	for (int j=0;j<num_renderers;j++)
	  surfaceActor[j][i]=NULL;
      }

  for (int j=0;j<num_renderers;j++)
    renderer[j]=NULL;

  wireFrame=PXFalse;


  has_surface=PXFalse;

  decimateGUI=NULL;
  smoothGUI=NULL;
  normalsGUI=NULL;

  cleanGUI=NULL;
  curvatureGUI=NULL;
  connectGUI=NULL;
  extractGUI=NULL;
  thresholdPointsGUI=NULL;
  appendGUI=NULL;
  extractObjectMapGUI=NULL;
  mapFunctionalGUI=NULL;
  extractObjectMapGUI=NULL;
  displaySizeGUI=NULL;

  materialGUI=NULL;
  textureGUI=NULL;

  //  transformControl=NULL;
  if (clipboardPolyData==NULL)
    strcpy(clipboardName,"None");
  for (int ia=0;ia<=2;ia++)
    lastPoint[ia]=0.0;
  
}

vtkpxGUIPolyDataControl::~vtkpxGUIPolyDataControl()
{
  if (this->internalTransform!=NULL && owns_transform==PXTrue)
    internalTransform->Delete();

  if (!has_surface)
      return;

  previousPolyData->Delete();
  tempPolyData->Delete();
  //  clipboardPolyData->Delete();

  
  for (int i=0;i<num_surfaces;i++)
    {
      if (polyData[i]!=NULL)
	polyData[i]->Delete();

      if (polyDataFilter[i]!=NULL)
	polyDataFilter[i]->Delete();
      
      for (int j=0;j<num_renderers;j++)
	if (surfaceActor[j][i]!=NULL)
	  surfaceActor[j][i]->Delete();
    }
}

void vtkpxGUIPolyDataControl::SetSimpleMode()
{
  if (!this->Initialized)
    simple_mode=PXTrue;
}

void vtkpxGUIPolyDataControl::InitGraphics()
{
  if (has_surface)
    return;
  
  has_surface=PXTrue;
  
  previousPolyData=vtkPolyData::New();
  tempPolyData=vtkPolyData::New();
  if (clipboardPolyData==NULL)
    clipboardPolyData=vtkPolyData::New();

  vtkSphereSource* sphere=vtkSphereSource::New();
  sphere->SetRadius(0.01);
  sphere->SetThetaResolution(3);
  sphere->SetPhiResolution(3);
  sphere->Update();
 
  for (int i=0;i<num_surfaces;i++)
      {

	polyData[i]=vtkPolyData::New();
	polyData[i]->CopyStructure(sphere->GetOutput());

	polyDataFilter[i]=vtkTransformPolyDataFilter::New();
	polyDataFilter[i]->SetInput(polyData[i]);
	polyDataFilter[i]->SetTransform(this->internalTransform);
	double c[3];
	vtkpxSurfaceUtil::GetColor(i,c);
	for (int j=0;j<num_renderers;j++)
	  {
	    vtkPolyDataMapper* surfaceMapper=vtkPolyDataMapper::New();
	    surfaceMapper->SetInput(polyDataFilter[i]->GetOutput());
	    surfaceActor[j][i]=vtkActor::New();
	    surfaceActor[j][i]->SetVisibility(0);
	    surfaceActor[j][i]->GetProperty()->SetColor(c[0],c[1],c[2]);
	    surfaceActor[j][i]->GetProperty()->SetLineWidth(2.0);
      surfaceActor[j][i]->SetMapper(surfaceMapper);

	    surfaceMapper->Delete();
	  }
      }

  sphere->Delete();
}

void vtkpxGUIPolyDataControl::SetRenderer(vtkRenderer *new_viewer,int num,int clip)
{
  num=Irange(num,0,3);
  
  if (renderer[num]!=NULL)
    return;

  InitGraphics();

  renderer[num]=new_viewer;

  //  fprintf(stderr,"Num=%d, clip=%d\n",num,clip);
  
  for (int i=0;i<num_surfaces;i++)
    {
      surfaceActor[num][i]->PickableOn();
      renderer[num]->AddActor(surfaceActor[num][i]);
      if (clip>0)
	{
	  /*if (i==0)
	    fprintf(stderr,"Clipping viewer %d\n",num);*/
	  surfaceActor[num][i]->GetProperty()->SetRepresentationToWireframe();
	  surfaceActor[num][i]->GetProperty()->SetAmbient(1.0);
	  surfaceActor[num][i]->GetProperty()->SetDiffuse(0.0);
	  surfaceActor[num][i]->GetProperty()->SetSpecular(0.0);
	}
    }
      
  UpdateDisplay();
  UpdateStatus();
}
/* -------------------------------------------------------------------------*/
char* vtkpxGUIPolyDataControl::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
    return GetWidgetName();
  
  
  vtkpxGUIComponent::Initialize(name,inside);
  //  PXTkDialog::initDisplay();

  PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
  
  PXTkFrame* menubar=new PXTkFrame(this->EventManager,polyFrame);
  polyFrame->addChildren("-side top -expand false -fill x",menubar);

  PXTkLabel* lab=new PXTkLabel(this->EventManager,menubar,"");
  lab->configure("-image","$::pxtclvtkpxcontrib::smalllogo ");
  menubar->addChildren("-side right -expand false  -padx 5 -pady 5",lab);



  PXTkMenuButton* but1=new PXTkMenuButton(this->EventManager,menubar,"File",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but1);
  
  PXTkMenu *menu1=new PXTkMenu(this->EventManager,but1,PXFalse);
  menu1->addButton("Load",101);
  //  menu1->addButton("Save",102);
  menu1->addButton("Save As",103);
  menu1->addSeparator();
  menu1->addCheckBox("Binary Save",0,save_binary,104);
  menu1->addSeparator();
  if (this->EventParent)
      menu1->addButton("Close",3);
  else
      menu1->addButton("Exit",1);
  
  PXTkMenuButton* but2=new PXTkMenuButton(this->EventManager,menubar,"Edit",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but2);
  
  PXTkMenu *menu2=new PXTkMenu(this->EventManager,but2,PXFalse);
  menu2->addButton("Undo",201);
  menu2->addSeparator();
  menu2->addButton("Copy",202);
  menu2->addButton("Paste",203);
  menu2->addSeparator();
  menu2->addButton("Append",310);
  
  PXTkMenuButton* but3=new PXTkMenuButton(this->EventManager,menubar,"Tools",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but3);
  
  PXTkMenu *menu3=new PXTkMenu(this->EventManager,but3,PXFalse);
  menu3->addButton("Smooth",301);
  menu3->addButton("Decimate",300);
  menu3->addButton("Subdivide",316);
  menu3->addButton("Cluster",304);
  menu3->addButton("Normals",302);

  if (!simple_mode)
    {
      //      menu3->addButton("Transform",306);
      //      menu3->addSeparator();
      menu3->addButton("Triangulate",303);
      menu3->addButton("Connectivity",305);
      menu3->addButton("Clip in Image",311);
      menu3->addButton("Convex Hull",315);
      menu3->addButton("Compute Curvatures",320);

    }
  menu3->addSeparator();
  menu3->addButton("Extract Iso-Contour",307);
  menu3->addButton("Threshold Points",312);
  menu3->addButton("Extract Object-Map",309);
  //  menu3->addSeparator();
  //  menu3->addButton("Map Activation",308);

  PXTkMenuButton* but4=new PXTkMenuButton(this->EventManager,menubar,"Display",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but4);
  
  PXTkMenu *menu4=new PXTkMenu(this->EventManager,but4,PXFalse);
  menu4->addButton("Surface Color",400);
  menu4->addButton("Surface Material",406);
  menu4->addButton("Surface Texture...",410);
  menu4->addButton("Texture Mapping",411);
  menu4->addButton("Display Size",314);
  menu4->addSeparator();
  menu4->addButton("Objectmap Colormap",402);
  menu4->addButton("ShapeIndex Colormap",404);
  menu4->addButton("Curvature Colormap",405);
  /* added my mjack */
  menu4->addButton("Custom Colormap...", 408);
  /* added by mjack */
  menu4->addSeparator();
  menu4->addButton("Clear Attributes",204);
  menu4->addSeparator();
  menu4->addButton("Info",403);
  menu4->addSeparator();
  menu4->addButton("Hide All",401);


  PXTkLabelFrame* vFrame=new PXTkLabelFrame(this->EventManager,polyFrame,"General Properties");
  polyFrame->addChildren("-side bottom -expand false  -padx 5 -pady 5",vFrame);
  vFrame->setPackMode(PXFalse);
  
  message=new PXTkLabel(this->EventManager,vFrame,"Num");
  vFrame->gridRowColumnChild(message,0,0,1,3);
  message->configure("-relief","ridge");
  
  currentSurface=new PXTkOptionMenu(this->EventManager,vFrame,"Surface 1",500);
  for (int i=1;i<num_surfaces;i++)
      {
	sprintf(pxtk_buffer,"Surface %d",i+1);
	currentSurface->addOption(pxtk_buffer);
      }
  vFrame->gridRowColumnChild(currentSurface,1,1);

  /*displaySurface=new PXTkCheckButton(this->EventManager,vFrame,"Show Surface",501);
    displaySurface->setState(displayPolyData[0]);*/
  displayMode=new PXTkOptionMenu(this->EventManager,vFrame,"Do Not Show",501);
  displayMode->addOption("Show as Surface");
  displayMode->addOption("Show as Wireframe");
  displayMode->addOption("Show as Points");
  displayMode->setIndex(displayPolyData[0]);
  vFrame->gridRowColumnChild(displayMode,1,2);
  //  vFrame->gridRowColumnChild(displaySurface,1,2);

  UpdateStatus();
  return GetWidgetName();

}
/* -------------------------------------------------------------------------*/
void  vtkpxGUIPolyDataControl::UpdateDisplay()
{
  if (!has_surface)
    return;

  int flag=0;

  for (int i=0;i<num_surfaces;i++)
    {
      int np=polyData[i]->GetNumberOfPoints();
      if (np<1)
	{
	  for (int j=0;j<num_renderers;j++)
	    surfaceActor[j][i]->SetVisibility(0);
	}
      else
	{
	  flag++;
	  double range[2];
	  range[0]=0.0;
	  range[1]=1.0;
	  if (polyData[i]->GetPointData()->GetScalars()!=NULL)
	    polyData[i]->GetPointData()->GetScalars()->GetRange(range);
	  
	  for (int j=0;j<num_renderers;j++)
	    {
	      if (displayPolyData[i]==0)
		{
		  surfaceActor[j][i]->SetVisibility(0);
		}
	      else
		{
		  vtkProperty* prop=surfaceActor[j][i]->GetProperty();
		  switch (displayPolyData[i])
		    {
		    case 1:
		      prop->SetRepresentationToSurface();
		      //prop->SetDiffuse(1.0);
		      //prop->SetAmbient(0.0);
		      //prop->SetSpecular(0.0);
		      // changes by mpj
		      //		      prop->SetOpacity(0.5);
		      // changes by mpj
		      break;
		    case 2:
		      prop->SetRepresentationToWireframe();
		      //prop->SetDiffuse(0.0);
		      //prop->SetAmbient(1.0);
		      //prop->SetSpecular(0.0);
		      break;
		    case 3:
		      prop->SetRepresentationToPoints();
		      //prop->SetDiffuse(0.0);
		      //prop->SetAmbient(1.0);
		      //prop->SetSpecular(0.0);
		      prop->SetPointSize(3.0);
		      break;
		    }
		  surfaceActor[j][i]->SetVisibility(1);
		  if (polyData[i]->GetPointData()->GetScalars()!=NULL)
		    {
		      if (polyData[i]->GetPointData()->GetScalars()->GetNumberOfComponents()==1)
			{
			  surfaceActor[j][i]->GetMapper()->SetScalarRange(range);
			  surfaceActor[j][i]->GetMapper()->ScalarVisibilityOn();
			} 
		      else 
			{
			  //fprintf(stderr,"Scalars (3)\n");
			  surfaceActor[j][i]->GetMapper()->ScalarVisibilityOn();
			  surfaceActor[j][i]->GetMapper()->SetColorModeToDefault();
			}
		    }
		      
		}
	    }
	}
    }

  if (flag)
    {
      for (int i=0;i<num_renderers;i++)
	if (renderer[i]!=NULL)
	  renderer[i]->Render();
    }

  return;
}

int  vtkpxGUIPolyDataControl::SetCurrentSurface(int s)
{
  s=Irange(s,0,num_surfaces-1);
  if (s==current_surface)
      return s;

  current_surface=s;
  displayMode->setIndex(displayPolyData[current_surface]);

  UpdateDisplay();
  UpdateStatus();
  return current_surface;
}

int  vtkpxGUIPolyDataControl::UpdateStatus()
{
  if (has_surface)
    {
      sprintf(pxtk_buffer,"Name=%s Points=%d Polys=%d",
	      currentSurface->getLabel(current_surface),
	      polyData[current_surface]->GetNumberOfPoints(),
	      polyData[current_surface]->GetNumberOfCells());
      return message->setText(pxtk_buffer);
    }
  else
    return 0;
}


int  vtkpxGUIPolyDataControl::HandleEvent(int event)
{
  if (!this->Initialized)
      return TCL_OK;

  if (event<10)
      return this->EventManager->handleDirectEvent(event);

  int dorender=0;

  if (!has_surface)
      return TCL_OK;

  switch(event)
      {
      case 101: // Load Surface
	dorender=this->LoadCurrentSurface("");
	break;
	
      case 102: // Save Surface
      case 103:
	{
	  if (polyData[current_surface]->GetNumberOfPoints()<1)
	    {
	      this->EventManager->messageBox("Cannot Save Surface, no surface in memory!","Error Message ...");
	    }
	  else
	    {
	      if (event==103)
		{
		  char *line=this->EventManager->getSaveFilenameComplex("Save PolyData","sur.vtk",
									"{\"VTK File\" {\".vtk\"}}  {\"All Files\" {\"*\"}}");
		  if (line==NULL)
		    break;
		  if (strlen(line)<1)
		    break;
		
		  strcpy(pxtk_buffer2,line);
		}
	      else
		{
		  int offset=0;
		  if (current_surface==9)
		    offset=1;
		  int n=strlen(currentSurface->getLabel(current_surface))-2-offset;
		  strncpy(pxtk_buffer2,currentSurface->getLabel(current_surface),n);
		  if (strcmp(pxtk_buffer2,"Surface")==0)
		    {
		      n+=2+offset;
		      strncpy(pxtk_buffer2,currentSurface->getLabel(current_surface),n);
		    }
		  pxtk_buffer2[n]=(char)0;
		  for (int i=0;i<n;i++)
		    {
		      if (pxtk_buffer2[i]==' ')
			pxtk_buffer2[i]='_';
		    }
		  
		  char* newname=anacreatefilename(pxtk_buffer2,".vtk",".vtk");
		  strcpy(pxtk_buffer2,newname);
		  delete [] newname;
		}
	      vtkPolyDataWriter* writer=vtkPolyDataWriter::New();
	      vtkpxGUICallback* clb=this->AddMethodsToFilter(writer,"Saving Surface");
	      
	      writer->SetFileName(pxtk_buffer2);
	      writer->SetInput(polyData[current_surface]);
	      if (this->save_binary==PXTrue)
		writer->SetFileTypeToBinary();
	      else
		{
		  writer->SetFileTypeToASCII();
		  //fprintf(stderr,"Saving Ascii\n");
		}
	      this->WatchOn();
	      writer->Update();
	      writer->Delete();
	      clb->Delete();
	      sprintf(pxtk_buffer2,"%s.%d",PXTkApp::getFilenameTail(pxtk_buffer2),current_surface+1);
	      currentSurface->changeLabel(current_surface,pxtk_buffer2);
	      UpdateStatus();
	      this->WatchOff();
	    }
	}
	break;

      case 104:
	if (save_binary==PXTrue)
	  save_binary=PXFalse;
	else
	  save_binary=PXTrue;
	break;

      case 201: // Undo
	polyData[current_surface]->ShallowCopy(previousPolyData);
	UpdateStatus();
	dorender=1;
	break;
	
      case 202: // Copy
	clipboardPolyData->ShallowCopy(polyData[current_surface]);
	strncpy(clipboardName,currentSurface->getLabel(current_surface),200);
	break;
	
      case 203: // Paste
	previousPolyData->ShallowCopy(polyData[current_surface]);
	polyData[current_surface]->ShallowCopy(clipboardPolyData);
	if (clipboardName[0]!='?')
	  sprintf(pxtk_buffer2,"Copy of %s.%d",clipboardName,current_surface+1);
	else
	  sprintf(pxtk_buffer2,"%s.%d",&clipboardName[1],current_surface+1);
	currentSurface->changeLabel(current_surface,pxtk_buffer2);
	UpdateStatus();
	UpdateDisplay();
	dorender=1;
	break;
	
      case 204: // Clear Pre-loaded colors etc 
	polyData[current_surface]->GetPointData()->SetScalars(NULL);
	dorender=1;
	break;

      case 300: // Decimate
	previousPolyData->ShallowCopy(polyData[current_surface]);
	doDecimate(0);
	break;
	
      case 350:
	dorender=doDecimate(1);
	UpdateStatus();
	break;
	
      case 301: // Smooth;
	previousPolyData->ShallowCopy(polyData[current_surface]);
	doSmooth(0);
	break;

      case 351:
	dorender=doSmooth(1);
	break;

      case 302: // Normals;
	previousPolyData->ShallowCopy(polyData[current_surface]);
	doNormals(0);
	break;
       
      case 352:
	dorender=doNormals(1);
	break;
	
      case 303: // Triangulate
	previousPolyData->ShallowCopy(polyData[current_surface]);
	dorender=doTriangulate();
	UpdateStatus();
	break;
	
      case 304: // Clean
	previousPolyData->ShallowCopy(polyData[current_surface]);
	doClean(0);
	break;
	
      case 354:
	dorender=doClean(1);
	UpdateStatus();
	break;
	
      case 305: // Connectivity
	previousPolyData->ShallowCopy(polyData[current_surface]);
	doConnect(0);
	break;

      case 355:
	dorender=doConnect(1);
	UpdateStatus();
	break;

	/*      case 306:
	doTransform(0);
	break;

      case 356:
	dorender=doTransform(1);
	UpdateStatus();
	break;*/

      case 307:
	previousPolyData->ShallowCopy(polyData[current_surface]);
	doExtract(0);
	break;

      case 308:
	previousPolyData->ShallowCopy(polyData[current_surface]);
	doMapFunctional(0);
	break;

      case 309:
	previousPolyData->ShallowCopy(polyData[current_surface]);
	doExtractObjectMap(0);
	break;

      case 310:
	previousPolyData->ShallowCopy(polyData[current_surface]);
	doAppend(0);
	break;
      
      case 311: // Clip
	previousPolyData->ShallowCopy(polyData[current_surface]);
	dorender=doClip();
	UpdateStatus();
	break;

      case 312:
	previousPolyData->ShallowCopy(polyData[current_surface]);
	this->doThresholdPoints(0);
	break;


      case 314:
	doChangeDisplaySize(0);
	break;

      case 315:
	previousPolyData->ShallowCopy(polyData[current_surface]);
	dorender=doDelaunay3D();
	break;

      case 316:
	previousPolyData->ShallowCopy(polyData[current_surface]);
	dorender=doSubdivision();
	break;


      case 320: // Compute Curvature
	previousPolyData->ShallowCopy(polyData[current_surface]);
	doCurvature(0);
	break;
	
      case 321:
	dorender=doCurvature(1);
	UpdateStatus();
	break;
	


      case 357:
	dorender=doExtract(1);
	UpdateStatus();
	break;

      case 358:
	dorender=doMapFunctional(1);
	UpdateStatus();
	break;

      case 359:
	dorender=doExtractObjectMap(1);
	UpdateStatus();
	break;


      case 360:
	dorender=doAppend(1);
	UpdateStatus();
	break;

      case 361:
	dorender=doAppend(2);
	UpdateStatus();
	break;


      case 362:
	dorender=this->doThresholdPoints(1);
	UpdateStatus();
	break;

      case 363:
	dorender=this->doThresholdPoints(2);
	UpdateStatus();
	break;


      case 374:
	doChangeDisplaySize(1);
	break;

      case 375:
	doChangeDisplaySize(2);
	break;


      case 400: // Surface Color
	{
	  double c[3];
	  vtkProperty* prop=surfaceActor[0][current_surface]->GetProperty();
	  prop->GetColor(c);
	  int a=this->EventManager->d_colorBox("Surface Color",c[0],c[1],c[2]);
	  if (a==1)
	    {
	      for (int k=0;k<num_renderers;k++)
		surfaceActor[k][current_surface]->GetProperty()->SetColor(c[0],c[1],c[2]);
	      dorender=1;
	    }
	}
	break;
	
      case 401:
	{
	  for (int i=0;i<num_surfaces;i++)
	    displayPolyData[i]=0;
	  UpdateDisplay();
	  dorender=1;
	}
	break;

      case 402:
	dorender=vtkpxSurfaceUtil::AddObjectMapLookupTableToSurface(polyData[current_surface]);
  for (int k=0;k<num_renderers;k++)
    surfaceActor[k][current_surface]->GetMapper()->UseLookupTableScalarRangeOff();
	break;

      case 404:
	dorender=vtkpxSurfaceUtil::AddShapeIndexLookupTableToSurface(polyData[current_surface]);
  for (int k=0;k<num_renderers;k++)
    surfaceActor[k][current_surface]->GetMapper()->UseLookupTableScalarRangeOff();
	break;

      case 405:
	dorender=vtkpxSurfaceUtil::AddCurvatureLookupTableToSurface(polyData[current_surface]);
  for (int k=0;k<num_renderers;k++)
    surfaceActor[k][current_surface]->GetMapper()->UseLookupTableScalarRangeOff();
	break;


	// mpj

      case 408: 
      dorender=this->LoadCustomLookupTable("");
      break;

      case 410:
      dorender=this->LoadCurrentTexture("");
      break;

      case 411:
      dorender=doTexture(0);
      UpdateStatus();
      break;

      case 412:
      dorender=doTexture(1);
      break;

      case 406:
	dorender=doMaterial(0);
	UpdateStatus();
	break;
      case 407:
	dorender=doMaterial(1);
	break;
	// mpj

      case 403:
	if (has_surface)
	  {
	    double range[2];
	    if (polyData[current_surface]->GetPointData()->GetScalars()!=NULL)
	      {
		int ns=polyData[current_surface]->GetPointData()->GetScalars()->GetNumberOfTuples();
		polyData[current_surface]->GetPointData()->GetScalars()->GetRange(range);
		if (polyData[current_surface]->GetPointData()->GetScalars()->GetLookupTable()!=NULL)
		  sprintf(pxtk_buffer2,"\n scalars=%d LKPT (range=%.1f:%.1f)",ns,range[0],range[1]);
		else
		  sprintf(pxtk_buffer2,"\n scalars=%d NONE (range=%.1f:%.1f)",ns,range[0],range[1]);
	      }
	    else 
	      strcpy(pxtk_buffer2," \n no scalars ");
	    
	    if (polyData[current_surface]->GetPointData()->GetNormals()!=NULL)
	      {
		int ns=polyData[current_surface]->GetPointData()->GetNormals()->GetNumberOfTuples();
		sprintf(pxtk_buffer3,"%s\n Normals=%d ",pxtk_buffer2,ns);
	      }
	    else
	      sprintf(pxtk_buffer3," %s\n no normals",pxtk_buffer2);
	    
	    sprintf(pxtk_buffer,"Points=%d Polys=%d  %s",
		    polyData[current_surface]->GetNumberOfPoints(),
		    polyData[current_surface]->GetNumberOfCells(),
		    pxtk_buffer3);
	    this->EventManager->messageBox(pxtk_buffer,"Current Surface Info ...");
	  }
	break;
	    
      case 500: // Current Surface;
	SetCurrentSurface(currentSurface->getIndex());
	dorender=1;
	break;
	
      case 501: // Show Surface
	displayPolyData[currentSurface->getIndex()]=displayMode->getIndex();
	UpdateDisplay();
	dorender=1;
	break;
      }

  if (dorender)
    {
      if (this->EventParent!=NULL && this->CallbackNo>0)
	{
	  this->EventParent->HandleEvent(CallbackNo);
	}
      else 
	{
	  for (int i=0;i<num_renderers;i++)
	    if (renderer[i]!=NULL)
	      renderer[i]->Render();
	}
    }
  return TCL_OK;
}


int vtkpxGUIPolyDataControl::GetCurrentSurface()
{
  return current_surface;
}

vtkPolyData* vtkpxGUIPolyDataControl::GetSurface(int index)
{
  int i=index;
  if (index==-1)
    i=current_surface;
  else
    i=Irange(index,0,num_surfaces-1);

  return polyData[i];
}
     
void vtkpxGUIPolyDataControl::SetSurface(int index,vtkPolyData* data)
{
  int i=Irange(index,0,num_surfaces-1);
  previousPolyData->ShallowCopy(polyData[i]);
  polyData[i]->ShallowCopy(data);
  polyData[i]->Modified();
  
  for (int v=0;v<num_renderers;v++)
    {
      if (renderer[v]!=NULL)
	{
	  if (surfaceActor[v][i]->GetVisibility()==1)
	    {
	      surfaceActor[v][i]->GetMapper()->Modified();
	      surfaceActor[v][i]->Modified();
	      renderer[v]->Render();
	    }
	}
    }
  UpdateStatus();
}

void vtkpxGUIPolyDataControl::SetSurface(int index,vtkPolyData* data,const char* name)
{
  int i=Irange(index,0,num_surfaces-1);
  SetSurface(i,data);

  sprintf(pxtk_buffer2,"%s.%d",name,i+1);
  currentSurface->changeLabel(i,pxtk_buffer2);

}

void vtkpxGUIPolyDataControl::SetClipboardSurface(vtkPolyData* data,const char* name)
{
  if (clipboardPolyData!=NULL)
    {
      clipboardPolyData->ShallowCopy(data);
      if (name!=NULL)
	  strcpy(clipboardName,name);
    }
}

void vtkpxGUIPolyDataControl::SetImage(vtkImageData* new_image)
{
  this->CurrentImage=new_image;

  if (extractGUI!=NULL)
    {
      if (extractGUI->isOpen())
	extractGUI->popdownDialog();
    }

  if (thresholdPointsGUI!=NULL)
    {
      if (thresholdPointsGUI->isOpen())
	thresholdPointsGUI->popdownDialog();
    }


  if (extractObjectMapGUI!=NULL)
    {
      if (extractObjectMapGUI->isOpen())
	extractObjectMapGUI->popdownDialog();
    }

  if (mapFunctionalGUI!=NULL)
    {
      if (mapFunctionalGUI->isOpen())
	mapFunctionalGUI->popdownDialog();
    }

  /*  if (transformControl!=NULL)
    {
      if (this->CurrentImage!=NULL)
	transformControl->SetDimensions(this->CurrentImage);
	}*/
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIPolyDataControl::SetLastPoint(double pt[3])
{
  for (int ia=0;ia<=2;ia++)
    lastPoint[ia]=pt[ia];
  
  if (connectGUI!=NULL)
    {
      if (connectGUI->isOpen())
	{
	  sprintf(pxtk_buffer,"Last Point (%6.2f,%6.2f,%6.2f)",lastPoint[0],lastPoint[1],lastPoint[2]);
	  connectPoint->setText(pxtk_buffer);
	}
    }
}

int vtkpxGUIPolyDataControl::doMaterial(int apply)
{

  if (apply==0)
    {
      if (materialGUI==NULL) {
	
        	PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
        	materialGUI=new PXTkDialog(this->EventManager,polyFrame,"Material Controls",PXFalse);
        	materialGUI->initDisplay();
        	
        	PXTkFrame* frame=(PXTkFrame*)materialGUI->getMainWindow();
        	opacityFactor=new PXTkScale(materialGUI,frame,"Opacity Factor",-1,PXTrue);
        	opacityFactor->setRange(0.0,1.0);
        	opacityFactor->setResolution(0.01);
        	opacityFactor->setValue(1.0);

          ambientFactor=new PXTkScale(materialGUI,frame,"Ambient Factor",-1,PXTrue);
          ambientFactor->setRange(0.0,1.0);
          ambientFactor->setResolution(0.01);
          ambientFactor->setValue(0.0);

          diffuseFactor=new PXTkScale(materialGUI,frame,"Diffuse Factor",-1,PXTrue);
          diffuseFactor->setRange(0.0,1.0);
          diffuseFactor->setResolution(0.01);
          diffuseFactor->setValue(1.0);

          specularFactor=new PXTkScale(materialGUI,frame,"Specular Factor",-1,PXTrue);
          specularFactor->setRange(0.0,1.0);
          specularFactor->setResolution(0.01);
          specularFactor->setValue(0.0);

          specularPower=new PXTkScale(materialGUI,frame,"Specular Power",-1,PXTrue);
          specularPower->setRange(0.0,200.0);
          specularPower->setResolution(0.1);
          specularPower->setValue(1.0);

        	PXTkFrame* fr2=new PXTkFrame(materialGUI,frame);
        	PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Apply",407);
        	PXTkButton* b4=new PXTkButton(materialGUI,fr2,"Close",3);

        	frame->addChildren("-side top -expand true -fill x",opacityFactor);
          frame->addChildren("-side top -expand true -fill x",ambientFactor,diffuseFactor,specularFactor,specularPower,fr2);
        	fr2->addChildren("-side left -expand true -fill x",b2,b4);

      }
      
      vtkProperty* prop=surfaceActor[0][current_surface]->GetProperty();
      
      opacityFactor->setValue(prop->GetOpacity());
      ambientFactor->setValue(prop->GetAmbient()); 
      diffuseFactor->setValue(prop->GetDiffuse());
      specularFactor->setValue(prop->GetSpecular());
      specularPower->setValue(prop->GetSpecularPower());

      materialGUI->popupDialog();
      
      return 0;

    }
  
  for (int k=0;k<num_renderers;k++) {
    surfaceActor[k][current_surface]->GetProperty()->SetOpacity(opacityFactor->getValue());
    surfaceActor[k][current_surface]->GetProperty()->SetAmbient(ambientFactor->getValue());
    surfaceActor[k][current_surface]->GetProperty()->SetDiffuse(diffuseFactor->getValue());
    surfaceActor[k][current_surface]->GetProperty()->SetSpecular(specularFactor->getValue());
    surfaceActor[k][current_surface]->GetProperty()->SetSpecularPower(specularPower->getValue());
  }
  
  return 1;
}

/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::doDecimate(int apply)
{

  if (polyData[current_surface]->GetNumberOfPoints()<2)
    {
      this->EventManager->messageBox("Cannot Decimate Surface, no surface in memory!","Error Message ...");
      return 0;
    }

  if (apply==0)
      {
	if (decimateGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
	      decimateGUI=new PXTkDialog(this->EventManager,polyFrame,"Decimate Controls",PXFalse);
	      decimateGUI->initDisplay();
	      
	      PXTkFrame* frame=(PXTkFrame*)decimateGUI->getMainWindow();
	      decimateFactor=new PXTkScale(decimateGUI,frame,"Factor",-1,PXTrue);
	      decimateFactor->setRange(0.0,1.0);
	      decimateFactor->setResolution(0.01);
	      decimateFactor->setValue(0.0);
	      
	      preserveTopology=new PXTkCheckButton(decimateGUI,frame,"Preserve Topology",-1);
	      preserveTopology->setState(PXTrue);

	      PXTkFrame* fr2=new PXTkFrame(decimateGUI,frame);
	      PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Decimate",350);
	      PXTkButton* b3=new PXTkButton(this->EventManager,fr2,"Undo",201);
	      PXTkButton* b4=new PXTkButton(decimateGUI,fr2,"Close",3);
	      frame->addChildren("-side top -expand true -fill x",decimateFactor,preserveTopology,fr2);
	      fr2->addChildren("-side left -expand true -fill x",b2,b3,b4);
	    }
  	decimateGUI->popupDialog();
	//decimateGUI->setModality(PXTrue);
	return 0;
      }

  
  //decimateGUI->popdownDialog();

  vtkTriangleFilter* triFilter=vtkTriangleFilter::New();
  triFilter->SetInput(polyData[current_surface]);
  
  vtkDecimatePro* deciFilter=vtkDecimatePro::New();
  vtkpxGUICallback* clb=this->AddMethodsToFilter(deciFilter,"Decimating Surface");
  deciFilter->SetInput(triFilter->GetOutput());
  deciFilter->SetTargetReduction(decimateFactor->getValue());
  
  if (preserveTopology->getState()==PXTrue)
      deciFilter->PreserveTopologyOn();
  else
      deciFilter->PreserveTopologyOff();

  this->WatchOn();
  triFilter->Update();
  deciFilter->Update();
  polyData[current_surface]->ShallowCopy(deciFilter->GetOutput());

  deciFilter->Delete();
  triFilter->Delete();
  this->WatchOff();  
  clb->Delete();

  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::doSmooth(int apply)
{
  if (polyData[current_surface]->GetNumberOfPoints()<2)
    {
      this->EventManager->messageBox("Cannot Smooth Surface, no surface in memory!","Error Message ...");
      return 0;
    }

  if (apply==0)
      {
	if (smoothGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
	      smoothGUI=new PXTkDialog(this->EventManager,polyFrame,"Smooth Controls",PXFalse);
	      smoothGUI->initDisplay();
	      
	      PXTkFrame* frame=(PXTkFrame*)smoothGUI->getMainWindow();
	      smoothIterations=new PXTkScale(smoothGUI,frame,"Iterations",-1,PXTrue);
	      smoothIterations->setRange(0.0,200);
	      smoothIterations->setResolution(1);
	      smoothIterations->setValue(0.0);
	      
	      smoothRelaxation=new PXTkScale(smoothGUI,frame,"Relaxation Factor (Lapl)",-1,PXTrue);
	      smoothRelaxation->setRange(0.0,1.0);
	      smoothRelaxation->setResolution(0.02);
	      smoothRelaxation->setValue(0.2);

	      smoothEdgeButton=new PXTkCheckButton(smoothGUI,frame,"Smooth Boundaries (Sinc)",-1,PXTrue);

	      smoothMethod=new PXTkOptionMenu(smoothGUI,frame,"Laplacian",-1);
	      smoothMethod->addOption("Windowed Sinc");
	      smoothMethod->setIndex(1);
	      	      
	      PXTkFrame* fr2=new PXTkFrame(smoothGUI,frame);
	      PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Smooth",351);
	      PXTkButton* b4=new PXTkButton(this->EventManager,fr2,"Undo",201);
	      PXTkButton* b3=new PXTkButton(smoothGUI,fr2,"Close",3);
	      frame->addChildren("-side top -expand true -fill x",smoothMethod,smoothIterations,smoothRelaxation,smoothEdgeButton);
	      frame->addChildren("-side top -expand true -fill x",fr2);
	      fr2->addChildren("-side left -expand true -fill x",b2,b4,b3);
	    }
  
	smoothGUI->popupDialog();
	//smoothGUI->setModality(PXTrue);
	return 0;
      }

  if (smoothMethod->getIndex()==0)
    {
      vtkSmoothPolyDataFilter* smoothFilter=vtkSmoothPolyDataFilter::New();  

      vtkpxGUICallback* clb=this->AddMethodsToFilter(smoothFilter,"Smoothing Surface");
      //this->WatchOn();      
      smoothFilter->SetInput(polyData[current_surface]);
      smoothFilter->SetNumberOfIterations(int(smoothIterations->getValue()));
      smoothFilter->SetRelaxationFactor(smoothRelaxation->getValue());
      smoothFilter->Update();
      polyData[current_surface]->ShallowCopy(smoothFilter->GetOutput());
      smoothFilter->Delete();
      clb->Delete();

      //      this->WatchOff();
    }
  else
    {
      vtkWindowedSincPolyDataFilter* smoothFilter=vtkWindowedSincPolyDataFilter::New();  
      vtkpxGUICallback* clb=this->AddMethodsToFilter(smoothFilter,"Smoothing Surface");
      this->WatchOn();      
      smoothFilter->SetInput(polyData[current_surface]);
      smoothFilter->SetNumberOfIterations(int(smoothIterations->getValue()));
      if (smoothEdgeButton->getState())
	smoothFilter->SetBoundarySmoothing(1);
      else
	smoothFilter->SetBoundarySmoothing(0);
      smoothFilter->Update();
      polyData[current_surface]->ShallowCopy(smoothFilter->GetOutput());
      smoothFilter->Delete();
      this->WatchOff();
      clb->Delete();

    }
  return 1;
}
/* -------------------------------------------------------------------------*/ 
int vtkpxGUIPolyDataControl::doNormals(int apply)
{

  if (polyData[current_surface]->GetNumberOfPoints()<2)
    {
      this->EventManager->messageBox("Cannot Calculate Surface Normals, no surface in memory!","Error Message ...");
      return 0;
    }

  if (apply==0)
      {
	if (normalsGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
	      normalsGUI=new PXTkDialog(this->EventManager,polyFrame,"Normals Controls",PXFalse);
	      normalsGUI->initDisplay();
	      
	      PXTkFrame* frame=(PXTkFrame*)normalsGUI->getMainWindow();
	      featureAngle=new PXTkScale(normalsGUI,frame,"Feature Angle",-1,PXTrue);
	      featureAngle->setRange(0.0,180.0);
	      featureAngle->setResolution(1.0);
	      featureAngle->setValue(30.0);
	      
	      edgeSplitButton=new PXTkCheckButton(normalsGUI,frame,"Edge Splitting",-1);
	      edgeSplitButton->setState(PXTrue);
	      
	      flipNormalsButton=new PXTkCheckButton(normalsGUI,frame,"Flip Normals",-1);
	      flipNormalsButton->setState(PXFalse);

	      PXTkFrame* fr2=new PXTkFrame(normalsGUI,frame);
	      PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Apply",352);
	      PXTkButton* b4=new PXTkButton(this->EventManager,fr2,"Undo",201);
	      PXTkButton* b3=new PXTkButton(normalsGUI,fr2,"Close",3);
	      frame->addChildren("-side top -expand true -fill x",featureAngle,edgeSplitButton,flipNormalsButton,fr2);
	      fr2->addChildren("-side left -expand true -fill x",b2,b4,b3);
	    }
	
	normalsGUI->popupDialog();
	//normalsGUI->setModality(PXTrue);
	return 0;
      }

  
  //normalsGUI->popdownDialog();
  this->WatchOn();

  vtkPolyDataNormals* normalsFilter=vtkPolyDataNormals::New();
  vtkpxGUICallback* clb=this->AddMethodsToFilter(normalsFilter,"Calculating Normals");
  normalsFilter->SetInput(polyData[current_surface]);
  normalsFilter->SetFeatureAngle(featureAngle->getValue());
  if (edgeSplitButton->getState()==PXTrue)
      normalsFilter->SetSplitting(1);
  else
      normalsFilter->SetSplitting(0);

  if (flipNormalsButton->getState()==PXTrue)
      normalsFilter->SetFlipNormals(1);
  else
      normalsFilter->SetFlipNormals(0);
  normalsFilter->Update();
  polyData[current_surface]->ShallowCopy(normalsFilter->GetOutput());
  normalsFilter->Delete();
  this->WatchOff();
  clb->Delete();
  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::doTriangulate(int apply)
{
  if (polyData[current_surface]->GetNumberOfPoints()<2)
    {
      this->EventManager->messageBox("Cannot Triangulate Surface, no surface in memory!","Error Message ...");
      return 0;
    }

  this->WatchOn();
  vtkTriangleFilter* triFilter=vtkTriangleFilter::New();
  vtkpxGUICallback* clb=this->AddMethodsToFilter(triFilter,"Triangulating Surface");
  triFilter->SetInput(polyData[current_surface]);
  triFilter->Update();
  polyData[current_surface]->ShallowCopy(triFilter->GetOutput());
  triFilter->Delete();
  clb->Delete();
  this->WatchOff();
  return 1;
}
/* -------------------------------------------------------------------------*/ 
int vtkpxGUIPolyDataControl::doDelaunay3D(int apply)
{
  if (polyData[current_surface]->GetNumberOfPoints()<2)
    {
      this->EventManager->messageBox("Cannot do Delaunay3D, no surface in memory!","Error Message ...");
      return 0;
    }

  this->WatchOn();
  vtkDelaunay3D* dFilter=vtkDelaunay3D::New();
  dFilter->SetInput(polyData[current_surface]);
  dFilter->Update();

  vtkDataSet* dt=(vtkDataSet*)(dFilter->GetOutput());
  /*  vtkUnstructuredGridToPolyDataFilter* filt=vtkUnstructuredGridToPolyDataFilter::New();
  filt->SetInput(dFilter->GetOutput());*/
  

  vtkGeometryFilter* geom=vtkGeometryFilter::New();
  geom->SetInput(dt);


  vtkpxGUICallback* clb2=this->AddMethodsToFilter(geom,"Extracting Outer Surface");
  geom->MergingOn();
  geom->Update();

  polyData[current_surface]->ShallowCopy(geom->GetOutput());
  dFilter->Delete();
  geom->Delete();
//  filt->Delete();
  clb2->Delete();
  this->WatchOff();
  return 1;
}
/* -------------------------------------------------------------------------*/ 
int vtkpxGUIPolyDataControl::doSubdivision(int apply)
{
  if (polyData[current_surface]->GetNumberOfPoints()<2)
    {
      this->EventManager->messageBox("Cannot do Delaunay3D, no surface in memory!","Error Message ...");
      return 0;
    }

  this->WatchOn();
  vtkLinearSubdivisionFilter* dFilter=vtkLinearSubdivisionFilter::New();
  dFilter->SetInput(polyData[current_surface]);
  dFilter->SetNumberOfSubdivisions(1);
  vtkpxGUICallback* clb2=this->AddMethodsToFilter(dFilter,"Extracting Outer Surface");
  dFilter->Update();

  polyData[current_surface]->ShallowCopy(dFilter->GetOutput());
  dFilter->Delete();
  clb2->Delete();
  this->WatchOff();
  return 1;
}
/* -------------------------------------------------------------------------*/ 
int vtkpxGUIPolyDataControl::doConnect(int apply)
{

  if (polyData[current_surface]->GetNumberOfPoints()<2)
    {
      this->EventManager->messageBox("Cannot Connect Surface, no surface in memory!","Error Message ...");
      return 0;
    }

  if (apply==0)
      {
	if (connectGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
	      connectGUI=new PXTkDialog(this->EventManager,polyFrame,"Connect Controls",PXFalse);
	      connectGUI->initDisplay();
	      
	      PXTkFrame* frame=(PXTkFrame*)connectGUI->getMainWindow();
	      connectType=new PXTkOptionMenu(connectGUI,frame,"Largest Region",-1);
	      connectType->addOption("Nearest Point");
	      connectType->setIndex(0);

	      connectPoint=new PXTkLabel(this->EventManager,frame,"");

	      PXTkFrame* fr2=new PXTkFrame(connectGUI,frame);
	      PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Apply",355);
	      PXTkButton* b4=new PXTkButton(this->EventManager,fr2,"Undo",201);
	      PXTkButton* b3=new PXTkButton(connectGUI,fr2,"Close",3);
	      frame->addChildren("-side top -expand true -fill x",connectType,connectPoint,fr2);
	      fr2->addChildren("-side left -expand true -fill x",b2,b4,b3);
	    }
	
	sprintf(pxtk_buffer,"Last Point (%6.2f,%6.2f,%6.2f)",lastPoint[0],lastPoint[1],lastPoint[2]);
	connectPoint->setText(pxtk_buffer);
	connectGUI->popupDialog();
	return 0;
      }

  this->WatchOn();
  vtkPolyDataConnectivityFilter* connectFilter=vtkPolyDataConnectivityFilter::New();
  vtkpxGUICallback* clb=this->AddMethodsToFilter(connectFilter,"Connectivity Filter");
  connectFilter->SetInput(polyData[current_surface]);
  if (connectType->getIndex()==0)
      {
	connectFilter->SetExtractionModeToLargestRegion();
      }
  else if (connectType->getIndex()==1)
      {
	connectFilter->SetExtractionModeToClosestPointRegion();
	connectFilter->SetClosestPoint(lastPoint);
      }
	
  connectFilter->Update();
  //fprintf(stderr,"Number of Regions=%d\n",connectFilter->GetNumberOfExtractedRegions());
 

  polyData[current_surface]->ShallowCopy(connectFilter->GetOutput());
  connectFilter->Delete();
  clb->Delete();
  this->WatchOff();
  return 1;
}
/* -------------------------------------------------------------------------*/  
int vtkpxGUIPolyDataControl::doClean(int apply)
{

  if (polyData[current_surface]->GetNumberOfPoints()<2)
    {
      this->EventManager->messageBox("Cannot Clean Surface, no surface in memory!","Error Message ...");
      return 0;
    }

  if (apply==0)
      {
	if (cleanGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
	      cleanGUI=new PXTkDialog(this->EventManager,polyFrame,"Clean Controls",PXFalse);
	      cleanGUI->initDisplay();
	      PXTkFrame* frame=(PXTkFrame*)cleanGUI->getMainWindow();

	      cleanMethod=new PXTkOptionMenu(cleanGUI,frame,"Distance Sampling",-1);
	      cleanMethod->addOption("Quadric Clustering");
	      cleanMethod->setIndex(0);
	      
	      cleanTolerance=new PXTkScale(cleanGUI,frame,"Tolerance",-1,PXTrue);
	      cleanTolerance->setRange(0.0,15.0);
	      cleanTolerance->setResolution(0.1);
	      cleanTolerance->setValue(1.0);
	      
	      PXTkFrame* fr2=new PXTkFrame(cleanGUI,frame);
	      PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Apply",354);
	      PXTkButton* b4=new PXTkButton(this->EventManager,fr2,"Undo",201);
	      PXTkButton* b3=new PXTkButton(cleanGUI,fr2,"Close",3);
	      frame->addChildren("-side top -expand true -fill x",cleanMethod,cleanTolerance,fr2);
	      fr2->addChildren("-side left -expand true -fill x",b2,b4,b3);
	    }
	
	cleanGUI->popupDialog();
	//cleanGUI->setModality(PXTrue);
	return 0;
      }

  //  cleanGUI->popdownDialog();
  this->WatchOn();

  if (cleanMethod->getIndex()==0)
    {
      vtkCleanPolyData* cleanerFilter=vtkCleanPolyData::New();
      vtkpxGUICallback* clb=this->AddMethodsToFilter(cleanerFilter,"Clean Surface Filter");
      cleanerFilter->SetInput(polyData[current_surface]);
      cleanerFilter->DebugOff();
      cleanerFilter->SetToleranceIsAbsolute(1);
      cleanerFilter->SetAbsoluteTolerance(cleanTolerance->getValue());
      cleanerFilter->Update();
      polyData[current_surface]->ShallowCopy(cleanerFilter->GetOutput());
      cleanerFilter->Delete();
      clb->Delete();
    }
  else
    {
      vtkTriangleFilter* triFilter=vtkTriangleFilter::New();
      triFilter->SetInput(polyData[current_surface]);
      triFilter->Update();

      double bounds[6];
      triFilter->GetOutput()->GetBounds(bounds);
      double sp=cleanTolerance->getValue();

      vtkQuadricClustering* clusterFilter=vtkQuadricClustering::New();
      vtkpxGUICallback* clb=this->AddMethodsToFilter(clusterFilter,"Cluster Surface Filter");
      clusterFilter->SetInput(triFilter->GetOutput());

      clusterFilter->SetDivisionOrigin(bounds[0],bounds[2],bounds[4]);
      clusterFilter->SetDivisionSpacing(sp,sp,sp);
      clusterFilter->SetNumberOfXDivisions(int(1.5+(bounds[1]-bounds[0])/sp));
      clusterFilter->SetNumberOfYDivisions(int(1.5+(bounds[3]-bounds[2])/sp));
      clusterFilter->SetNumberOfZDivisions(int(1.5+(bounds[5]-bounds[4])/sp));
      clusterFilter->Update();


      int copydirect=1;
      if (polyData[current_surface]->GetPointData()!=NULL)
	{
	  if (polyData[current_surface]->GetPointData()->GetScalars()!=NULL)
	    {
	      vtkPolyData* temp=vtkPolyData::New();
	      temp->ShallowCopy(clusterFilter->GetOutput());
	      int docopy=vtkpxSurfaceUtil::MapScalarsToSurface(polyData[current_surface],temp);
	      if (docopy)
		{
		  polyData[current_surface]->ShallowCopy(temp);
		  copydirect=0;
		}
	      temp->Delete();
	    }
	}
	
      if (copydirect)
	polyData[current_surface]->ShallowCopy(clusterFilter->GetOutput());
      clusterFilter->Delete();
      triFilter->Delete();
      clb->Delete();

    }
  this->WatchOff();
  return 1;
}
/* -------------------------------------------------------------------------*/
/*int vtkpxGUIPolyDataControl::doTransform(int apply)
{
  if (polyData[current_surface]->GetNumberOfPoints()<2)
    {
      this->EventManager->messageBox("Cannot Transform Surface, no surface in memory!","Error Message ...");
      return 0;
    }

    if (apply==0)
      {
	if (transformControl==NULL)
	  {
	    transformControl=vtkpxGUITransformControl::New();
	    transformControl->Initialize(this->GetWidgetName(),0);
	    transformControl->SetCallback(this,356);
	  }
	transformControl->SetDimensions(this->CurrentImage);
	//sprintf(pxtk_buffer,"wm deiconify %s",transformControl->GetWidgetName());
	//PXTkApp::executeTclCommand(pxtk_buffer);
	transformControl->Show();
	return 0;
      }
    
    this->WatchOn();
    vtkTransformPolyDataFilter* transformFilter=vtkTransformPolyDataFilter::New();
    vtkpxGUICallback* clb=this->AddMethodsToFilter(transformFilter,"Transforming Surface");
    transformFilter->SetInput(polyData[current_surface]);
    transformFilter->SetTransform(transformControl->GetCurrentTransform());
    transformFilter->Update();
    polyData[current_surface]->ShallowCopy(transformFilter->GetOutput());
    transformFilter->Delete();
    this->WatchOff();
    clb->Delete();
    return 1;
}*/
/* -------------------------------------------------------------------------*/
void vtkpxguipolydatacontrolextractstats(vtkImageData* img,const char* name)
{
  double range[2];
  int dim[3];
  img->GetDimensions(dim);
  img->GetPointData()->GetScalars()->GetRange(range);
  /*fprintf(stderr,"Image %s dimensions %dx%dx%d range %.1f:%.1f\n",
    name,dim[0],dim[1],dim[2],range[0],range[1]);*/
}

/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::doExtract(int apply)
{
  if (this->CurrentImage==NULL)
    {
      this->EventManager->messageBox("Cannot Extract Iso-Contour, no image in memory!","Error Message ...");
      return 0;
    }

  if (apply==0)
      {
	if (extractGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
	      extractGUI=new PXTkDialog(this->EventManager,polyFrame,"Extract Controls",PXFalse);
	      extractGUI->initDisplay();
	      PXTkFrame* frame=(PXTkFrame*)extractGUI->getMainWindow();	      
	      
	      PXTkLabelFrame* valFrame=new PXTkLabelFrame(extractGUI,frame,"Iso-Contour Level");
	      PXTkLabelFrame* smtFrame=new PXTkLabelFrame(extractGUI,frame,"Smooth Image");
	      PXTkLabelFrame* resFrame=new PXTkLabelFrame(extractGUI,frame,"Resample Image");
	      PXTkLabelFrame* thrFrame=new PXTkLabelFrame(extractGUI,frame,"Threshold Image");
	      frame->addChildren("-side top -fill x -expand f",valFrame,thrFrame,smtFrame,resFrame);

	      // Smooth GUI
	      extractDoSmooth=new PXTkOptionMenu(extractGUI,smtFrame,"No Smoothing",-1);
	      extractDoSmooth->addOption("Smooth");
	      extractDoSmooth->setIndex(0);

	      extractSmoothScale=new PXTkArrowScale(extractGUI,smtFrame,"Sigma",-1,PXTrue,PXTrue);
	      extractSmoothScale->setRange(0.25,6.0);
	      extractSmoothScale->setValue(1.0);
	      extractSmoothScale->setIncrement(0.25);
	      extractSmoothScale->setResolution(0.01);
	      extractSmoothScale->setLengthWidth(60,8);
	      extractSmoothScale->setDecimalPoints(2);
	      smtFrame->addChildren("-side left -fill x -expand f",extractDoSmooth,extractSmoothScale->getMainWindow());

	      extractDoResample=new PXTkCheckButton(extractGUI,resFrame,"Resample",-1);
	      extractDoResample->setState(PXFalse);
	      extractResampleScale=new PXTkArrowScale(extractGUI,resFrame,"Factor",-1,PXTrue,PXTrue);
	      extractResampleScale->setRange(1.0,6.0);
	      extractResampleScale->setValue(2.0);
	      extractResampleScale->setIncrement(0.5);
	      extractResampleScale->setResolution(0.5);
	      extractResampleScale->setLengthWidth(60,8);
	      extractResampleScale->setDecimalPoints(1);
	      smtFrame->addChildren("-side left -fill x -expand f",extractDoResample,extractResampleScale->getMainWindow());
	      
	      extractDoThreshold=new PXTkCheckButton(extractGUI,thrFrame,"Threshold",-1);
	      extractDoThreshold->setState(PXFalse);
	      thrFrame->addChildren("-side top -fill x -expand f",extractDoThreshold);

	      for (int ia=0;ia<=2;ia++)
		{
		  switch (ia)
		    {
		    case 0:
		      thresholdScale[ia]=new PXTkArrowScale(extractGUI,valFrame,"Extract Level",-1,PXTrue,PXTrue);
		      valFrame->addChildren("-side top -fill x -expand f",thresholdScale[ia]->getMainWindow());
		      thresholdScale[ia]->setRange(0.0,255.0);
		      thresholdScale[ia]->setValue(128.0);
		      thresholdScale[ia]->setDecimalPoints(2);
		      break;
		    case 1:
		      thresholdScale[ia]=new PXTkArrowScale(extractGUI,thrFrame,"Low Threshold",-1,PXTrue,PXTrue);
		      thrFrame->addChildren("-side top -fill x -expand f",thresholdScale[ia]->getMainWindow());
		      thresholdScale[ia]->setRange(0.0,255.0);
		      thresholdScale[ia]->setValue(0.0);
		      break;
		    case 2:
		      thresholdScale[ia]=new PXTkArrowScale(extractGUI,thrFrame,"High Threshold",-1,PXTrue,PXTrue);
		      thrFrame->addChildren("-side top -fill x -expand f",thresholdScale[ia]->getMainWindow());
		      thresholdScale[ia]->setRange(0.0,255.0);
		      thresholdScale[ia]->setValue(255.0);
		      break;
		    }
		  thresholdScale[ia]->setIncrement(1);
		  thresholdScale[ia]->setResolution(0.5);
		  thresholdScale[ia]->setLengthWidth(120,8);
		}
		  
	      PXTkFrame* fr2=new PXTkFrame(extractGUI,frame);
	      PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Apply",357);
	      PXTkButton* b4=new PXTkButton(this->EventManager,fr2,"Undo",201);
	      PXTkButton* b3=new PXTkButton(extractGUI,fr2,"Close",3);
	      frame->addChildren("-side top -expand true -fill x",fr2);
	      fr2->addChildren("-side left -expand true -fill x",b2,b4,b3);
	    }

	double range[2];
	this->CurrentImage->GetPointData()->GetScalars()->GetRange(range);
	//fprintf(stderr,"Image Range %f:%f\n",range[0],range[1]);
	for (int ia=0;ia<=2;ia++)
	  thresholdScale[ia]->setRange(range[0],range[1]);

	thresholdScale[0]->setValue(0.5*(range[0]+range[1]));
	thresholdScale[1]->setValue(range[0]);
	thresholdScale[2]->setValue(range[1]);

	double drange=range[1]-range[0];
	int    decimalpoints=1;
	double goodrange=50.0;
	double inc=1.0;

	if (range[1]-range[0]>50.0)
	  {
	    decimalpoints=0;
	  }
	else
	  {
	    while (drange<goodrange)
	      {
		decimalpoints+=1;
		drange*=10.0;
		inc*=0.2;
	      }
	  }

	for (int ia=0;ia<=2;ia++)
	  {
	    thresholdScale[ia]->setDecimalPoints(decimalpoints);
	    thresholdScale[ia]->setIncrement(inc);
	    thresholdScale[ia]->setResolution( (range[1]-range[0])/100.0);
	  }

	extractGUI->popupDialog();
	return 0;
      }

  this->WatchOn();

  vtkImageData* lastOutput=vtkImageData::New();
  lastOutput->ShallowCopy(this->CurrentImage);

  if (extractDoSmooth->getIndex()>0)
    {
      float sm=extractSmoothScale->getValue();
      int   smrad=int(sm*2.0+1.5);
      vtkImageGaussianSmooth* smoothFilter=vtkImageGaussianSmooth::New();
      smoothFilter->SetInput(lastOutput);
      smoothFilter->SetStandardDeviations(sm,sm,sm);
      smoothFilter->SetRadiusFactors(smrad,smrad,smrad);
      vtkpxGUICallback* clb1=this->AddMethodsToFilter(smoothFilter,"Smoothing Image");
      smoothFilter->Update();
      clb1->Delete();
      //      smoothFilter->DebugOn();
      lastOutput->ShallowCopy(smoothFilter->GetOutput());
      smoothFilter->Delete();
      vtkpxguipolydatacontrolextractstats(lastOutput,"Smoothing");
    }


  if (extractDoResample->getState()==PXTrue)
    {
      float scalefact=1.0/Frange(extractResampleScale->getValue(),0.001,10.0);
      vtkbisImageResample* resl=vtkbisImageResample::New();
      resl->SetInput(lastOutput);
      resl->SetInterpolate(1);
      resl->SetAxisMagnificationFactor(0,scalefact);
      resl->SetAxisMagnificationFactor(1,scalefact);
      resl->SetAxisMagnificationFactor(2,scalefact);
      vtkpxGUICallback* clb2=this->AddMethodsToFilter(resl,"Resampling Image");
      resl->Update();
      clb2->Delete();
      lastOutput->ShallowCopy(resl->GetOutput());
      //      resl->DebugOn();
      resl->Delete();
      vtkpxguipolydatacontrolextractstats(lastOutput,"Resampling");
    }

  if (extractDoThreshold->getState()==PXTrue)
    {
      float lowt=thresholdScale[1]->getValue();
      float hight=thresholdScale[2]->getValue();
      
      if (lowt>hight)
	{
	  float t=hight;
	  hight=lowt;
	  lowt=t;
	  thresholdScale[1]->setValue(lowt);
	  thresholdScale[2]->setValue(hight);
	}
      vtkImageThreshold* thrFilter=vtkImageThreshold::New();
      thrFilter->SetInput(lastOutput);
      vtkpxGUICallback* clb0=NULL;
      if (extractDoSmooth->getIndex()==2)
	{
	  thrFilter->ThresholdByUpper(lowt);
	  clb0=this->AddMethodsToFilter(thrFilter,"Thresholding Grad Image");
	}
      else
	{
	  thrFilter->ThresholdBetween(lowt,hight);
	  clb0=this->AddMethodsToFilter(thrFilter,"Thresholding Inten Image");
	}
      thrFilter->SetOutValue(0.0);
      thrFilter->ReplaceInOff();
      thrFilter->Update();
      clb0->Delete();
      
      lastOutput->ShallowCopy(thrFilter->GetOutput());
      vtkpxguipolydatacontrolextractstats(lastOutput,"Thresholding");
      thrFilter->Delete();
    }

  vtkpxguipolydatacontrolextractstats(lastOutput,"Final Output");
  vtkContourFilter* skinExtractor=vtkContourFilter::New();
  skinExtractor->SetInput(lastOutput);
  skinExtractor->SetUseScalarTree(0);

  int dim[3];   lastOutput->GetDimensions(dim);
  double r[2]; lastOutput->GetPointData()->GetScalars()->GetRange(r);
  sprintf(pxtk_buffer,"Extracting %dx%dx%d %.0f:%.0f",dim[0],dim[1],dim[2],r[0],r[1]);
  vtkpxGUICallback* clb=this->AddMethodsToFilter(skinExtractor,pxtk_buffer);
  
  skinExtractor->SetNumberOfContours(1);
  skinExtractor->SetValue(0,thresholdScale[0]->getValue());
  skinExtractor->ComputeScalarsOff();
  skinExtractor->ComputeGradientsOff();
  skinExtractor->ComputeNormalsOff();
  skinExtractor->Update();

  polyData[current_surface]->ShallowCopy(skinExtractor->GetOutput());
  skinExtractor->Delete();
  lastOutput->Delete();
  clb->Delete();
  this->WatchOff();
  UpdateDisplay();
  return 1;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIPolyDataControl::SetTransform(vtkAbstractTransform* trans)
{
  if (trans==NULL)
    return;

  if (this->internalTransform!=NULL && owns_transform==PXTrue)
    internalTransform->Delete();

  this->internalTransform=trans;
  owns_transform=PXFalse;

  if (has_surface)
    {
      for (int i=0;i<num_surfaces;i++)
	{
	  polyDataFilter[i]->SetTransform(this->internalTransform);
	  polyDataFilter[i]->Update();
	}
    }
}

/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::doMapFunctional(int apply)
{
  if (this->CurrentImage==NULL)
    {
      this->EventManager->messageBox("Cannot Map Functional, no image in memory!","Error Message ...");
      return 0;
    }

  if (apply==0)
      {
	if (mapFunctionalGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
	      mapFunctionalGUI=new PXTkDialog(this->EventManager,polyFrame,"Extract Controls",PXFalse);
	      mapFunctionalGUI->initDisplay();
	      PXTkFrame* frame=(PXTkFrame*)mapFunctionalGUI->getMainWindow();	      
	      
	      mapFunctionalType=new PXTkOptionMenu(this->EventManager,frame,"Map Positive Only",500);
	      mapFunctionalType->addOption("Map Negative Only");
	      mapFunctionalType->addOption("Map Both");
	      frame->addChildren("-side top -fill x -expand f",mapFunctionalType);

	      mapFunctionalMode=new PXTkOptionMenu(this->EventManager,frame,"Maximum Value",-1);
	      mapFunctionalMode->addOption("Average Value");
	      frame->addChildren("-side top -fill x -expand f",mapFunctionalMode);
	      
	      for (int ia=0;ia<=1;ia++)
		{
		  if (ia==0)
		    sprintf(pxtk_buffer2,"Low Threshold");
		  else
		    sprintf(pxtk_buffer2,"High Threshold");
		  
		  mapThreshold[ia]=new PXTkArrowScale(mapFunctionalGUI,frame,pxtk_buffer2,-1,PXTrue,PXTrue);
		  mapThreshold[ia]->setRange(0.0,255.0);
		  mapThreshold[ia]->setValue(float(64*(ia+1)));
		  mapThreshold[ia]->setIncrement(1);
		  mapThreshold[ia]->setResolution(1);
		  mapThreshold[ia]->setLengthWidth(120,8);
		  frame->addChildren("-side top -fill x -expand f",mapThreshold[ia]->getMainWindow());
		}

	      for (int ib=0;ib<=1;ib++)
		{
		  if (ib==1)
		    sprintf(pxtk_buffer2,"Max Distance");
		  else
		    sprintf(pxtk_buffer2,"Min Distance");
		  
		  mapDistance[ib]=new PXTkArrowScale(mapFunctionalGUI,frame,pxtk_buffer2,-1,PXTrue,PXTrue);
		  mapDistance[ib]->setRange(-50.0,50.0);
		  if (ib==1)
		    mapDistance[ib]->setValue(10.0);
		  else
		    mapDistance[ib]->setValue(0.0);
		  mapDistance[ib]->setIncrement(1.0);
		  mapDistance[ib]->setResolution(0.5);
		  mapDistance[ib]->setLengthWidth(120,8);
		  frame->addChildren("-side top -fill x -expand f",mapDistance[ib]->getMainWindow());
		}

	      PXTkFrame* fr2=new PXTkFrame(mapFunctionalGUI,frame);
	      PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Apply",358);
	      PXTkButton* b4=new PXTkButton(this->EventManager,fr2,"Undo",201);
	      PXTkButton* b3=new PXTkButton(mapFunctionalGUI,fr2,"Close",3);
	      frame->addChildren("-side top -expand true -fill x",fr2);
	      fr2->addChildren("-side left -expand true -fill x",b2,b4,b3);
	    }

	double range[2];
	this->CurrentImage->GetPointData()->GetScalars()->GetRange(range);
	if (range[0]<0.0)
	    {
	      if (range[0]>range[1])
		range[1]=range[0];
	      range[0]=0.0;
	    }
	  
	  if (range[1]<0.0)
	    range[1]=fabs(range[1]);

	  for (int ia=0;ia<=1;ia++)
	    mapThreshold[ia]->setRange(range[0],range[1]);
	  
	  // For Binary Images allow 0.5 increments
	  if ((range[1]-range[0])<10.0)
	    {
	      for (int ia=0;ia<=1;ia++)
		{
		  mapThreshold[ia]->setResolution(0.25);
		  mapThreshold[ia]->setIncrement(0.5);
		  mapThreshold[ia]->getScale()->setDigits(3);
		}
	  }
	else
	  {
	    for (int ia=0;ia<=1;ia++)
	      {
		mapThreshold[ia]->setDecimalPoints(0);
		mapThreshold[ia]->setIncrement(1);
		mapThreshold[ia]->setResolution(1);
	      }
	  }

	mapFunctionalGUI->popupDialog();
	return 0;
      }

  //  fprintf(stderr,"Extracting 1\n");

  this->WatchOn();
  vtkpxMapActivationToPolyData* functionMapper=vtkpxMapActivationToPolyData::New();
  vtkpxGUICallback* clb=this->AddMethodsToFilter(functionMapper,"Mapping Function");
  functionMapper->DebugOn();
  functionMapper->SetInput(polyData[current_surface]);
  functionMapper->SetFunctionalData(this->CurrentImage);
  functionMapper->SetLowThreshold(mapThreshold[0]->getValue());
  functionMapper->SetHighThreshold(mapThreshold[1]->getValue());

  if (mapFunctionalType->getIndex()==0)
    {
      functionMapper->SetThresholdPositive(1);
      functionMapper->SetThresholdNegative(0);
    }
  else if (mapFunctionalType->getIndex()==1)
    {
      functionMapper->SetThresholdPositive(0);
      functionMapper->SetThresholdNegative(1);
    }
  else
    {
      functionMapper->SetThresholdPositive(1);
      functionMapper->SetThresholdNegative(1);
    }

  functionMapper->SetMinDistance(mapDistance[0]->getValue());
  functionMapper->SetMaxDistance(mapDistance[1]->getValue());
  functionMapper->SetAverageMode(mapFunctionalMode->getIndex());
  functionMapper->Update();

  polyData[current_surface]->ShallowCopy(functionMapper->GetOutput());
  functionMapper->Delete();
  clb->Delete();
  this->WatchOff();
  UpdateDisplay();
  return 1;
}
/* -------------------------------------------------------------------------*/

int vtkpxGUIPolyDataControl::doExtractObjectMap(int apply)
{
  if (this->CurrentImage==NULL)
    {
      this->EventManager->messageBox("Cannot Extract Object-Map, no image in memory!","Error Message ...");
      return 0;
    }

  if (apply==0)
      {
	if (extractObjectMapGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
	      extractObjectMapGUI=new PXTkDialog(this->EventManager,polyFrame,"ObjectMap",PXFalse);
	      extractObjectMapGUI->initDisplay();
	      PXTkFrame* frame=(PXTkFrame*)extractObjectMapGUI->getMainWindow();	      
	      
	      PXTkLabelFrame* thrFrame=new PXTkLabelFrame(extractObjectMapGUI,frame,"Object Map Range");
	      frame->addChildren("-side top -fill x -expand f",thrFrame);

	      for (int ia=0;ia<=1;ia++)
		{
		  switch (ia)
		    {
		    case 0:
		      objectMapRange[ia]=new PXTkArrowScale(extractObjectMapGUI,thrFrame,"Low Threshold",-1,PXTrue,PXTrue);
		      thrFrame->addChildren("-side top -fill x -expand f",objectMapRange[ia]->getMainWindow());
		      objectMapRange[ia]->setRange(0.0,255.0);
		      objectMapRange[ia]->setValue(0.0);
		      break;
		    case 1:
		      objectMapRange[ia]=new PXTkArrowScale(extractObjectMapGUI,thrFrame,"High Threshold",-1,PXTrue,PXTrue);
		      thrFrame->addChildren("-side top -fill x -expand f",objectMapRange[ia]->getMainWindow());
		      objectMapRange[ia]->setRange(0.0,255.0);
		      objectMapRange[ia]->setValue(255.0);
		      break;
		    }
		  objectMapRange[ia]->setIncrement(1);
		  objectMapRange[ia]->setResolution(1);
		  objectMapRange[ia]->setLengthWidth(120,8);
		}
		
	      PXTkFrame* f1=new PXTkFrame(extractObjectMapGUI,frame);
	      PXTkFrame* f2=new PXTkFrame(extractObjectMapGUI,frame);
	      PXTkFrame* f3=new PXTkFrame(extractObjectMapGUI,frame);

	      frame->addChildren("-side top -fill x -expand f",f1,f3,f2);

	      PXTkLabel* l1=new PXTkLabel(extractObjectMapGUI,f1,"Smooth XY:");
	      objectMapValues[0]=new PXTkOptionMenu(extractObjectMapGUI,f1,"0.0",-1);
	      objectMapValues[0]->addOption("1.0");
	      objectMapValues[0]->addOption("2.0");
	      objectMapValues[0]->addOption("3.0");
	      objectMapValues[0]->addOption("4.0");

	      PXTkLabel* l3=new PXTkLabel(extractObjectMapGUI,f3,"Smooth Z:");
	      objectMapValues[2]=new PXTkOptionMenu(extractObjectMapGUI,f3,"0.0",-1);
	      objectMapValues[2]->addOption("1.0");
	      objectMapValues[2]->addOption("2.0");
	      objectMapValues[2]->addOption("3.0");
	      objectMapValues[2]->addOption("4.0");


	      PXTkLabel* l2=new PXTkLabel(extractObjectMapGUI,f2,"Resample:");
	      objectMapValues[1]=new PXTkOptionMenu(extractObjectMapGUI,f2,"1.0",-1);
	      objectMapValues[1]->addOption("2.0");
	      objectMapValues[1]->addOption("3.0");
	      objectMapValues[1]->addOption("4.0");
	      f1->addChildren("-side left -expand f -fill x",l1,objectMapValues[0]);
	      f2->addChildren("-side left -expand f -fill x",l2,objectMapValues[1]);
	      f3->addChildren("-side left -expand f -fill x",l3,objectMapValues[2]);
	        
	      PXTkFrame* fr2=new PXTkFrame(extractObjectMapGUI,frame);
	      PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Apply",359);
	      PXTkButton* b4=new PXTkButton(this->EventManager,fr2,"Undo",201);
	      PXTkButton* b3=new PXTkButton(extractObjectMapGUI,fr2,"Close",3);
	      frame->addChildren("-side top -expand true -fill x",fr2);
	      fr2->addChildren("-side left -expand true -fill x",b2,b4,b3);
	    }
	
	double range[2];
	this->CurrentImage->GetPointData()->GetScalars()->GetRange(range);
	for (int ia=0;ia<=1;ia++)
	  objectMapRange[ia]->setRange(1,range[1]);

	objectMapRange[0]->setValue(range[0]+1);
	objectMapRange[1]->setValue(range[1]);

	extractObjectMapGUI->popupDialog();
	return 0;
      }

  this->WatchOn();

  int low=(int)objectMapRange[0]->getValue();
  int high=(int)objectMapRange[1]->getValue();

  vtkPolyData* temp=vtkPolyData::New();
  if (vtkpxSurfaceUtil::ObjectMapToPolyData(this->CurrentImage,temp,low,high,
					    objectMapValues[0]->getIndex(),
					    objectMapValues[2]->getIndex(),
					    1.0+objectMapValues[1]->getIndex())>0)
    polyData[current_surface]->ShallowCopy(temp);
  temp->Delete();


  this->WatchOff();
  UpdateDisplay();
  return 1;
}

/* -------------------------------------------------------------------------*/

int vtkpxGUIPolyDataControl::doAppend(int apply)
{
  if (apply==0)
      {
	if (appendGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
	      appendGUI=new PXTkDialog(this->EventManager,polyFrame,"Append Surfaces",PXFalse);
	      appendGUI->initDisplay();
	      PXTkFrame* frame=(PXTkFrame*)appendGUI->getMainWindow();	      
	      
	      PXTkLabelFrame* thrFrame=new PXTkLabelFrame(appendGUI,frame,"Input Surfaces");
	      frame->addChildren("-side top -fill x -expand f",thrFrame);


	      PXTkLabelFrame* reFrame=new PXTkLabelFrame(appendGUI,frame,"Output Surface");

	      appendResult=new PXTkOptionMenu(appendGUI,reFrame,"Surface 1",-1);
	      reFrame->addChildren("-side top -fill x -expand f",appendResult);

	      for (int ia=0;ia<num_surfaces;ia++)
		{
		  sprintf(pxtk_buffer,"Surface %d",ia+1);
		  appendButton[ia]=new PXTkCheckButton(appendGUI,thrFrame,pxtk_buffer,-1);
		  appendButton[ia]->setState(PXFalse);
		  thrFrame->addChildren("-side top -fill x -expand f",appendButton[ia]);
		  if (ia>0)
		    appendResult->addOption(pxtk_buffer);
		}
	      
	      appendObjectMap=new PXTkCheckButton(appendGUI,frame,"Preserve Labels",-1);
	      appendObjectMap->setState(PXTrue);

	      PXTkFrame* frame2=new PXTkFrame(appendGUI,frame);
	      frame->addChildren("-side top -fill x -expand f",appendObjectMap,reFrame,frame2);


	      PXTkButton* b1=new PXTkButton(this->EventManager,frame2,"Append",360);
	      PXTkButton* b2=new PXTkButton(this->EventManager,frame2,"Undo",201);
	      PXTkButton* b3=new PXTkButton(this->EventManager,frame2,"Reset",361);
	      PXTkButton* b4=new PXTkButton(appendGUI,frame2,"Close",3);
	      frame2->addChildren("-side left -expand true -fill x",b1,b2,b3,b4);
	    }

	appendResult->setIndex(current_surface);
	appendGUI->popupDialog();
	return 0;
      }

  if (apply==2 && appendGUI!=NULL)
    {
      for (int ia=0;ia<num_surfaces;ia++)
	appendButton[ia]->setState(PXFalse);
      appendResult->setIndex(current_surface);
      appendObjectMap->setState(PXTrue);
      return 0;
    }


  // Check First that at least one surface has been selected 
  int numsur=0;
  for (int i=0;i<num_surfaces;i++)
    if (appendButton[i]->getState()==PXTrue)
      numsur++;
  if (numsur==0)
    return 0;

  vtkAppendPolyData* append=vtkAppendPolyData::New();

  int offset=0;
  int result=appendResult->getIndex();
  for (int j=0;j<num_surfaces;j++)
    {
      if (appendButton[j]->getState()==PXTrue)
	{
	  vtkPolyData* temp=vtkPolyData::New();
	  temp->ShallowCopy(polyData[j]);
	  offset+=vtkpxSurfaceUtil::AddObjectLabelsToSurface(temp,offset);
	  //  fprintf(stderr,"Surface = %d , offset=%d\n",j,offset);
	  append->AddInput(temp);
	  temp->Delete();
	}
    }
  append->Update();
  polyData[result]->ShallowCopy(append->GetOutput());
  append->Delete();
  vtkpxSurfaceUtil::AddObjectMapLookupTableToSurface(polyData[result]);

  this->WatchOff();
  UpdateDisplay();
  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::doClip(int apply)
{
  if (this->CurrentImage==NULL)
    {
      this->EventManager->messageBox("Cannot Clip Surface, no image in memory!","Error Message ...");
      return 0;
    }

  this->WatchOn();

  vtkImageThreshold* thrFilter=vtkImageThreshold::New();
  thrFilter->SetInput(this->CurrentImage);
  thrFilter->ThresholdByUpper(1.0);
  thrFilter->SetOutValue(0.0);
  thrFilter->SetInValue(1.0);
  thrFilter->ReplaceInOn();
  thrFilter->ReplaceOutOn();
  thrFilter->Update();

  vtkImplicitVolume* vol=vtkImplicitVolume::New();
  vol->SetVolume(thrFilter->GetOutput());
  vol->SetOutValue(0.0);

  thrFilter->Delete();

  vtkClipPolyData* clipFilter=vtkClipPolyData::New();
  vtkpxGUICallback* clb=this->AddMethodsToFilter(clipFilter,"Clipping Surface");
  clipFilter->SetInput(polyData[current_surface]);
  clipFilter->SetClipFunction(vol);
  clipFilter->GenerateClipScalarsOff();
  clipFilter->SetValue(0.5);
  clipFilter->Update();

  polyData[current_surface]->ShallowCopy(clipFilter->GetOutput());
  clipFilter->Delete();
  vol->Delete();
  clb->Delete();
  this->WatchOff();
  return 1;
}

/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::LoadCustomLookupTable(const char* fname)
{
  int ok;
  char* line;
  if (strlen(fname)<2)
    {
      char* line=this->EventManager->getOpenFilename("Colormap File name","Colormap Files","*.cmap*");
    
      if (line==NULL)
        return 0;
      if (strlen(line)<1)
        return 0;
      strcpy(pxtk_buffer2,line);
    }
  else
    {
      strcpy(pxtk_buffer2,fname);
    }

  vtkLookupTable *cmap=vtkLookupTable::New();
  ok=vtkpxColorMapUtil::LoadColormap(cmap,pxtk_buffer2);

  for (int k=0;k<num_renderers;k++)
    surfaceActor[k][current_surface]->GetMapper()->UseLookupTableScalarRangeOn();

  polyData[current_surface]->GetPointData()->GetScalars()->SetLookupTable(cmap);
  cmap->Delete();

  return 1;
  
}


/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::LoadCurrentTexture(const char* fname)
{
  if (polyData[current_surface]->GetNumberOfPoints()<2 || this->CurrentImage==NULL)
  {
    this->EventManager->messageBox("Cannot texture map, no surface or image in memory!", "Error Message ...");
    return 0;
  }

  int dim[3]; 
  double ori[3], spc[3], center[3];
  this->CurrentImage->GetDimensions(dim);
  this->CurrentImage->GetOrigin(ori);
  this->CurrentImage->GetSpacing(spc);
  center[0] = ori[0] + spc[0]*(dim[0]/2);
  center[1] = ori[1] + spc[1]*(dim[1]/2);
  center[2] = ori[2] + spc[2]*(dim[2]/2);

  vtkDataSetAlgorithm *tmapper=NULL;

  if (strlen(fname)<2)
  {
      char* line=this->EventManager->getOpenFilenameComplex("Load Texture",
                  "{\"PNG Files\" {\".png\" } }");
      if (line==NULL)
        return 0;
      if (strlen(line)<1)
        return 0;
      strcpy(pxtk_buffer2,line);
    }
  else
    {
      strcpy(pxtk_buffer2,fname);
    }

    char* ext=PXTkApp::getFilenameExtension(pxtk_buffer2);
    if (strcmp(ext,".png")==0 || strcmp(ext,".PNG")==0)
    {
      vtkPNGReader* Reader = vtkPNGReader::New();
      vtkpxGUICallback* clb=this->AddMethodsToFilter(Reader,"Loading Texture");
      Reader->SetFileName(pxtk_buffer2);
      this->WatchOn();
      Reader->Update();
    
      // create texture
      vtkTexture *texture = vtkTexture::New();
      texture->SetInputConnection(Reader->GetOutputPort());

      vtkTransformTextureCoords *xform = vtkTransformTextureCoords::New();

      if (textureGUI) {

          // create texture mapper 
          if (textureMode->getIndex() == 0) 
          {
            tmapper = vtkTextureMapToCylinder::New();

            // set cylindrical mapping center
            if (textureCyl->getState()) 
            {
              double p1[3];
              p1[0] = ori[0] + Frange(atoi(P1x->getText()),(float)0.0,(float)dim[0]) * spc[0];
              p1[1] = ori[1] + Frange(atoi(P1y->getText()),(float)0.0,(float)dim[1]) * spc[1];
              p1[2] = ori[2] + Frange(atoi(P1z->getText()),(float)0.0,(float)dim[2]) * spc[2];
              double p2[3];
              p2[0] = ori[0] + Frange(atoi(P2x->getText()),(float)0.0,(float)dim[0]) * spc[0];
              p2[1] = ori[1] + Frange(atoi(P2y->getText()),(float)0.0,(float)dim[1]) * spc[1];
              p2[2] = ori[2] + Frange(atoi(P2z->getText()),(float)0.0,(float)dim[2]) * spc[2];
              dynamic_cast<vtkTextureMapToCylinder*>(tmapper)->SetPoint1(p1);
              dynamic_cast<vtkTextureMapToCylinder*>(tmapper)->SetPoint2(p2);
              dynamic_cast<vtkTextureMapToCylinder*>(tmapper)->SetAutomaticCylinderGeneration(0);
            } else {
              dynamic_cast<vtkTextureMapToCylinder*>(tmapper)->SetAutomaticCylinderGeneration(1);
            }

            // set texture seam
            if (textureSeam->getState())
              dynamic_cast<vtkTextureMapToCylinder*>(tmapper)->PreventSeamOn();
            else
              dynamic_cast<vtkTextureMapToCylinder*>(tmapper)->PreventSeamOff();

          } 

         if (textureMode->getIndex() == 1) 
         {
            tmapper = vtkTextureMapToSphere::New();

            // set spherical mapping center
            if (sphereMode->getIndex() == 1) 
            {
              dynamic_cast<vtkTextureMapToSphere*>(tmapper)->SetCenter(center);
              dynamic_cast<vtkTextureMapToSphere*>(tmapper)->SetAutomaticSphereGeneration(0);
            } else {
              dynamic_cast<vtkTextureMapToSphere*>(tmapper)->SetAutomaticSphereGeneration(1);
            }

            // set texture seam
            if (textureSeam->getState())
              dynamic_cast<vtkTextureMapToSphere*>(tmapper)->PreventSeamOn();
            else
              dynamic_cast<vtkTextureMapToSphere*>(tmapper)->PreventSeamOff();
          }

        // transform texture coordinates
        if (textureMode->getIndex() == 2) {
          xform->SetInputConnection(polyDataFilter[current_surface]->GetOutputPort());
        } else {
          tmapper->SetInputConnection(polyDataFilter[current_surface]->GetOutputPort());
          xform->SetInputConnection(tmapper->GetOutputPort());
        }

        xform->SetOrigin(textureOrigin[0]->getValue(),textureOrigin[1]->getValue(), 1);
        xform->SetPosition(textureTranslate[0]->getValue(), textureTranslate[1]->getValue(), 1);
        xform->SetScale(textureScale[0]->getValue(), textureScale[1]->getValue(), 1);
        xform->SetFlipR(textureFlipR->getState());
        xform->SetFlipS(textureFlipS->getState());
      
        if (tmapper)
          tmapper->Delete();

      } else {

        // spherical mapping by default
        tmapper = vtkTextureMapToSphere::New();
        tmapper->SetInputConnection(polyDataFilter[current_surface]->GetOutputPort());

        // transform texture coordinates
        xform->SetInputConnection(tmapper->GetOutputPort());
        xform->SetScale(1, -1, 1);
      
        tmapper->Delete();
      }

      /* polydata mapper */
      vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
      mapper->SetInputConnection(xform->GetOutputPort());

      xform->Delete();

      for (int k=0;k<num_renderers;k++) {
        surfaceActor[k][current_surface]->SetMapper(mapper);
        surfaceActor[k][current_surface]->SetTexture(texture);
      }

      texture->Delete();
      mapper->Delete();

      Reader->Delete();
      clb->Delete();

    } else
    {
      printf("Not a valid filename!\n");
    }

     this->WatchOff();
    UpdateStatus();
    UpdateDisplay();
    return 1;
}

/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::LoadCurrentSurface(const char* fname)
{
  if (strlen(fname)<2)
    {
      char* line=this->EventManager->getOpenFilenameComplex("Load Surface",
							    "{\"Surface/Tstack Files\" {\".vtk\" \".tstack\" } } { \"Polydata File\" {\"*.vtk\"}} {\"Tstack File\" {\"*.tstack\"}}");
      if (line==NULL)
	return 0;
      if (strlen(line)<1)
	return 0;
      strcpy(pxtk_buffer2,line);
    }
  else
    {
      strcpy(pxtk_buffer2,fname);
    }

  char* ext=PXTkApp::getFilenameExtension(pxtk_buffer2);

  if (strcmp(ext,".vtk")==0 || strcmp(ext,".VTK")==0)
    {
      vtkPolyDataReader* Reader=vtkPolyDataReader::New();
      vtkpxGUICallback* clb=this->AddMethodsToFilter(Reader,"Loading Surface");
      Reader->SetFileName(pxtk_buffer2);
      this->WatchOn();
      Reader->Update();
      previousPolyData->ShallowCopy(polyData[current_surface]);
      polyData[current_surface]->ShallowCopy(Reader->GetOutput());
      Reader->Delete();
      clb->Delete();
    }
  else
    {
      vtkpxTstackReader* Reader=vtkpxTstackReader::New();
      vtkpxGUICallback* clb=this->AddMethodsToFilter(Reader,"Loading Surface");
      Reader->SetFileName(pxtk_buffer2);
      this->WatchOn();
      Reader->Update();
      previousPolyData->ShallowCopy(polyData[current_surface]);
      polyData[current_surface]->ShallowCopy(Reader->GetOutput());
      Reader->Delete();
      clb->Delete();
    }


  //  sprintf(pxtk_buffer2,"%s.%d",PXTkApp::getFilenameTail(pxtk_buffer2),current_surface+1);
  sprintf(pxtk_buffer2,"%s.%d",anaeatpath(pxtk_buffer2),current_surface+1);
  currentSurface->changeLabel(current_surface,pxtk_buffer2);
  
  this->WatchOff();
  UpdateStatus();
  UpdateDisplay();
  return 1;
}


/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::doThresholdPoints(int apply)
{
  if (this->CurrentImage==NULL)
    {
      this->EventManager->messageBox("Cannot Threshold Points, no image in memory!","Error Message ...");
      return 0;
    }

  if (apply==0)
      {
	if (thresholdPointsGUI==NULL)
	    {
	      PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
	      thresholdPointsGUI=new PXTkDialog(this->EventManager,polyFrame,"Threshold Points Controls",PXFalse);
	      thresholdPointsGUI->initDisplay();
	      PXTkFrame* frame=(PXTkFrame*)thresholdPointsGUI->getMainWindow();	      
	      
	      PXTkLabelFrame* valFrame=new PXTkLabelFrame(thresholdPointsGUI,frame,"Set Levels");
	      PXTkLabelFrame* reFrame=new PXTkLabelFrame(thresholdPointsGUI,frame,"Input Data-set");
	      frame->addChildren("-side top -fill x -expand f",reFrame,valFrame);

	      for (int ia=0;ia<=1;ia++)
		{
		  if (ia==0)
		    {
		      pointsThresholdScale[ia]=new PXTkArrowScale(thresholdPointsGUI,valFrame,"Low Threshold",-1,PXTrue,PXTrue);
		      valFrame->addChildren("-side top -fill x -expand f",pointsThresholdScale[ia]->getMainWindow());
		      pointsThresholdScale[ia]->setRange(0.0,255.0);
		      pointsThresholdScale[ia]->setValue(0.0);
		    } 
		  else
		    {
		      pointsThresholdScale[ia]=new PXTkArrowScale(thresholdPointsGUI,valFrame,"High Threshold",363,PXTrue,PXTrue);
		      valFrame->addChildren("-side top -fill x -expand f",pointsThresholdScale[ia]->getMainWindow());
		      pointsThresholdScale[ia]->setRange(0.0,255.0);
		      pointsThresholdScale[ia]->setValue(255.0);
		    }
		  pointsThresholdScale[ia]->setIncrement(1);
		  pointsThresholdScale[ia]->setResolution(0.5);
		  pointsThresholdScale[ia]->setLengthWidth(120,8);
		}
	      
	      
	      pointsThresholdInput=new PXTkOptionMenu(this->EventManager,reFrame,"Surface 1",362);
	      reFrame->addChildren("-side top -fill x -expand f",pointsThresholdInput);
	      for (int i=1;i<num_surfaces;i++)
		{
		  sprintf(pxtk_buffer,"Surface %d",i+1);
		  pointsThresholdInput->addOption(pxtk_buffer);
		}
	      pointsThresholdInput->addOption("Current Image");
	      
	      PXTkFrame* fr2=new PXTkFrame(thresholdPointsGUI,frame);
	      PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Apply",363);
	      PXTkButton* b4=new PXTkButton(this->EventManager,fr2,"Undo",201);
	      PXTkButton* b3=new PXTkButton(thresholdPointsGUI,fr2,"Close",3);
	      frame->addChildren("-side top -expand true -fill x",fr2);
	      fr2->addChildren("-side left -expand true -fill x",b2,b4,b3);
	    }
      }

  //fprintf(stderr,"apply=%d\n",apply);

  if (apply==0 || apply ==1)
    {
      vtkDataSet* data=this->CurrentImage;
      int index=pointsThresholdInput->getIndex();
      if (index<num_surfaces)
	data=(polyData[index]);

      double range[2];
      if (data->GetPointData()->GetScalars()!=NULL)
	{
	  data->GetPointData()->GetScalars()->GetRange(range);
	  //fprintf(stderr,"pdata");
	}
      else if (data->GetCellData()->GetScalars()!=NULL)
	{
	  data->GetCellData()->GetScalars()->GetRange(range);
	  //	  fprintf(stderr,"cdata");
	}
      else
	{
	  data=this->CurrentImage;
	  range[0]=0.0;
	  range[1]=1.0;
	}
      //fprintf(stderr,"Image Range %f:%f\n",range[0],range[1]);
      for (int ia=0;ia<=1;ia++)
	pointsThresholdScale[ia]->setRange(range[0],range[1]);
      
	pointsThresholdScale[0]->setValue(range[0]);
	pointsThresholdScale[1]->setValue(range[1]);
	
	// For Binary Images allow 0.5 increments
	if ((range[1]-range[0])<20.0)
	  {
	    for (int ia=0;ia<=1;ia++)
	      {
		pointsThresholdScale[ia]->setResolution(0.1);
		pointsThresholdScale[ia]->setIncrement(0.1);
		pointsThresholdScale[ia]->getScale()->setDigits(3);
	      }
	  }
	else
	  {
	    for (int ia=0;ia<=1;ia++)
	      {
		pointsThresholdScale[ia]->setDecimalPoints(1);
		pointsThresholdScale[ia]->setIncrement(1);
		pointsThresholdScale[ia]->setResolution(0.5);
	      }
	  }

	if (apply==0)
	  thresholdPointsGUI->popupDialog();
	return 0;
    }

  this->WatchOn();


  float lowt=pointsThresholdScale[0]->getValue();
  float hight=pointsThresholdScale[1]->getValue();
  
  if (lowt>hight)
    {
      float t=hight;
      hight=lowt;
      lowt=t;
      pointsThresholdScale[0]->setValue(lowt);
      pointsThresholdScale[1]->setValue(hight);
    }


  vtkThresholdPoints* thrFilter=vtkThresholdPoints::New();  

  int index=pointsThresholdInput->getIndex();
  if (index>=num_surfaces)
    thrFilter->SetInput(this->CurrentImage);
  else
    thrFilter->SetInput(polyData[index]);

  //  vtkpxGUICallback* clb0=NULL;
  thrFilter->ThresholdBetween(lowt,hight);
  thrFilter->Update();
  //  clb0->Delete();
  
  polyData[current_surface]->ShallowCopy(thrFilter->GetOutput());
  thrFilter->Delete();
  this->WatchOff();
  UpdateDisplay();
  return 1;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::doChangeDisplaySize(int apply)
{
  if (apply==0)
    {
      if (displaySizeGUI==NULL)
	{
	  PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
	  displaySizeGUI=new PXTkDialog(this->EventManager,polyFrame,"Display Sizes",PXFalse);
	  displaySizeGUI->initDisplay();
	  PXTkFrame* frame=(PXTkFrame*)displaySizeGUI->getMainWindow();	      
	  
	  PXTkLabelFrame* thrFrame=new PXTkLabelFrame(displaySizeGUI,frame,"Widths");
	  frame->addChildren("-side top -fill x -expand f",thrFrame);
	  
	  for (int ia=0;ia<=1;ia++)
	    {
	      switch (ia)
		{
		case 0:
		  displaySize[ia]=new PXTkArrowScale(displaySizeGUI,thrFrame,"Line Width",-1,PXTrue,PXTrue);
		  thrFrame->addChildren("-side top -fill x -expand f",displaySize[ia]->getMainWindow());
		  displaySize[ia]->setRange(0.01,10.0);
		  displaySize[ia]->setValue(1.0);
		  break;
		case 1:
		  displaySize[ia]=new PXTkArrowScale(displaySizeGUI,thrFrame,"Point Size",-1,PXTrue,PXTrue);
		  thrFrame->addChildren("-side top -fill x -expand f",displaySize[ia]->getMainWindow());
		  displaySize[ia]->setRange(0.01,10.0);
		  displaySize[ia]->setValue(1.0);
		  break;
		}
	      displaySize[ia]->setIncrement(1);
	      displaySize[ia]->setResolution(0.2);
	      displaySize[ia]->setLengthWidth(120,8);
	    }
	  
	  PXTkFrame* fr2=new PXTkFrame(displaySizeGUI,frame);
	  PXTkButton* b1=new PXTkButton(this->EventManager,fr2,"Apply",374);
	  PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Apply All",375);
	  PXTkButton* b3=new PXTkButton(displaySizeGUI,fr2,"Close",3);
	  frame->addChildren("-side top -expand true -fill x",fr2);
	  fr2->addChildren("-side left -expand true -fill x",b1,b2,b3);
	}
      
      vtkProperty* prop=surfaceActor[0][current_surface]->GetProperty();
      float lw=prop->GetLineWidth();
      float pz=prop->GetPointSize();
      
      displaySize[0]->setValue(lw);
      displaySize[1]->setValue(pz);
      
      displaySizeGUI->popupDialog();
      return 0;
    }

  this->WatchOn();

  float lw=displaySize[0]->getValue();
  float  pz=displaySize[1]->getValue();
  
  for (int i=0;i<num_surfaces;i++)
    {
      if (i==current_surface || apply==2)
	{
	  for (int j=0;j<num_renderers;j++)
	    {
	      vtkProperty* prop=surfaceActor[j][i]->GetProperty();
	      prop->SetLineWidth(lw);
	      prop->SetPointSize(pz);
	      surfaceActor[j][i]->Modified();
	    }
	}
    }
  this->WatchOff();
  UpdateDisplay();
  return 1;
}
 
/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::doTexture(int apply)
{
   if (polyData[current_surface]->GetNumberOfPoints()<2 || this->CurrentImage==NULL)
    {
      this->EventManager->messageBox("Cannot texture map, no surface or image in memory!","Error Message ...");
      return 0;
    }

  int dim[3]; 
  double ori[3], spc[3], center[3];
  this->CurrentImage->GetDimensions(dim);
  this->CurrentImage->GetOrigin(ori);
  this->CurrentImage->GetSpacing(spc);
  center[0] = ori[0] + spc[0]*(dim[0]/2);
  center[1] = ori[1] + spc[1]*(dim[1]/2);
  center[2] = ori[2] + spc[2]*(dim[2]/2);

  if (apply==0) {

      if (textureGUI==NULL) {

        PXTkFrame* texFrame=(PXTkFrame*)this->EventManager->getMainWindow();
        textureGUI=new PXTkDialog(this->EventManager,texFrame,"Texture Mapping",PXFalse);
        textureGUI->initDisplay();
        PXTkFrame* frame=(PXTkFrame*)textureGUI->getMainWindow();

        textureMode=new PXTkOptionMenu(textureGUI,frame,"Cylindrical Mapping",-1);
        textureMode->addOption("Spherical Mapping");
        textureMode->addOption("Customized");
        textureMode->setIndex(1);

        frame->addChildren("-side top -expand true -fill x",textureMode);

        textureSeam=new PXTkCheckButton(textureGUI,frame,"Prevent seam  ",-1);
        textureSeam->setState(PXFalse);

        PXTkFrame* frameOrig=new PXTkFrame(textureGUI,frame);
        frame->addChildren("-side top -expand true -fill x",frameOrig);

        textureOrigin[0]=new PXTkScale(textureGUI,frameOrig,"Origin-R:",-1,PXTrue);
        frameOrig->addChildren("-side left -fill x -expand t",textureOrigin[0]);
        textureOrigin[0]->setRange(-2.0,2.0);
        textureOrigin[0]->setResolution(0.01);
        textureOrigin[0]->setValue(0.5);
        textureOrigin[1]=new PXTkScale(textureGUI,frameOrig,"Origin-S:",-1,PXTrue);
        frameOrig->addChildren("-side left -fill x -expand t",textureOrigin[1]);
        textureOrigin[1]->setRange(-2.0,2.0);
        textureOrigin[1]->setResolution(0.01);
        textureOrigin[1]->setValue(0.5);

        PXTkFrame* frameTrans=new PXTkFrame(textureGUI,frame);
        frame->addChildren("-side top -expand true -fill x",frameTrans);

        textureTranslate[0]=new PXTkScale(textureGUI,frameTrans,"Translate-R:",-1,PXTrue);
        frameTrans->addChildren("-side left -fill x -expand t",textureTranslate[0]);
        textureTranslate[0]->setRange(-2.0,2.0);
        textureTranslate[0]->setResolution(0.01);
        textureTranslate[0]->setValue(0.0);
        textureTranslate[1]=new PXTkScale(textureGUI,frameTrans,"Translate-S:",-1,PXTrue);
        frameTrans->addChildren("-side left -fill x -expand t",textureTranslate[1]);
        textureTranslate[1]->setRange(-2.0,2.0);
        textureTranslate[1]->setResolution(0.01);
        textureTranslate[1]->setValue(0.0);

        PXTkFrame* frameScale=new PXTkFrame(textureGUI,frame);
        frame->addChildren("-side top -expand true -fill x",frameScale);

        textureScale[0]=new PXTkScale(textureGUI,frameScale,"Scale-R:",-1,PXTrue);
        frameScale->addChildren("-side left -fill x -expand t",textureScale[0]);
        textureScale[0]->setRange(-2.0,2.0);
        textureScale[0]->setResolution(0.01);
        textureScale[0]->setValue(1.0);
        textureScale[1]=new PXTkScale(textureGUI,frameScale,"Scale-S:",-1,PXTrue);
        frameScale->addChildren("-side left -fill x -expand t",textureScale[1]);
        textureScale[1]->setRange(-2.0,2.0);
        textureScale[1]->setResolution(0.01);
        textureScale[1]->setValue(1.0);

        PXTkLabelFrame* centerFrame=new PXTkLabelFrame(textureGUI,frame,"Spherical Mapping");
        frame->addChildren("-side top -fill x -expand t",centerFrame);

        PXTkLabel *sphereCenter = new PXTkLabel(textureGUI,centerFrame,"Center: ");
        sphereMode=new PXTkOptionMenu(textureGUI,centerFrame,"Object",-1);
        sphereMode->addOption("Image");
        sphereMode->setIndex(0);
        centerFrame->addChildren("-side left -padx 2 -pady 4 -fill x -expand f",sphereCenter,sphereMode);

        PXTkLabelFrame* cylFrame=new PXTkLabelFrame(textureGUI,frame,"Cylindrical Mapping");
        frame->addChildren("-side top -fill x -expand t",cylFrame);

        textureCyl=new PXTkCheckButton(textureGUI,cylFrame,"Manual",-1);
        textureCyl->setState(PXFalse);


        PXTkLabel *lp1 = new PXTkLabel(textureGUI,cylFrame,"P1: ");
        lp1->configure("-width",5);
        cylFrame->addChildren("-side left -padx 2 -pady 4 -fill x -expand f",textureCyl,lp1);

        P1x = new PXTkEntry(textureGUI,cylFrame);
        P1x->configure("-width",5);
        P1x->configure("-relief","sunken");
        sprintf(pxtk_buffer,"%d",dim[0]/2);
        P1x->setText(pxtk_buffer);
        cylFrame->addChildren("-side left -padx 2 -pady 4 -fill x -expand f",P1x);

        P1y = new PXTkEntry(textureGUI,cylFrame);
        P1y->configure("-width",5);
        P1y->configure("-relief","sunken");
        sprintf(pxtk_buffer,"%d",dim[1]/2);
        P1y->setText(pxtk_buffer);
        cylFrame->addChildren("-side left -padx 2 -pady 4 -fill x -expand f",P1y);

        P1z = new PXTkEntry(textureGUI,cylFrame);
        P1z->configure("-width",5);
        P1z->configure("-relief","sunken");
        sprintf(pxtk_buffer,"%d",0);
        P1z->setText(pxtk_buffer);
        cylFrame->addChildren("-side left -padx 2 -pady 4 -fill x -expand f",P1z);


        PXTkLabel *lp2 = new PXTkLabel(textureGUI,cylFrame,"P2: ");
        lp2->configure("-width",5);
        cylFrame->addChildren("-side left -padx 2 -pady 4 -fill x -expand f",textureCyl,lp2);

        P2x = new PXTkEntry(textureGUI,cylFrame);
        P2x->configure("-width",5);
        P2x->configure("-relief","sunken");
        sprintf(pxtk_buffer,"%d",dim[0]/2);
        P2x->setText(pxtk_buffer);
        cylFrame->addChildren("-side left -padx 2 -pady 4 -fill x -expand f",P2x);

        P2y = new PXTkEntry(textureGUI,cylFrame);
        P2y->configure("-width",5);
        P2y->configure("-relief","sunken");
        sprintf(pxtk_buffer,"%d",dim[1]/2);
        P2y->setText(pxtk_buffer);
        cylFrame->addChildren("-side left -padx 2 -pady 4 -fill x -expand f",P2y);

        P2z = new PXTkEntry(textureGUI,cylFrame);
        P2z->configure("-width",5);
        P2z->configure("-relief","sunken");
        sprintf(pxtk_buffer,"%d",dim[2]-1);
        P2z->setText(pxtk_buffer);
        cylFrame->addChildren("-side left -padx 2 -pady 4 -fill x -expand f",P2z);

        textureFlipR=new PXTkCheckButton(textureGUI,frame,"Flip R",-1);
        textureFlipR->setState(PXFalse);
        textureFlipS=new PXTkCheckButton(textureGUI,frame,"Flip S",-1);
        textureFlipS->setState(PXTrue);

        PXTkFrame* fr2=new PXTkFrame(textureGUI,frame);
        PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Apply",412);
        PXTkButton* b3=new PXTkButton(textureGUI,fr2,"Close",3);

        frame->addChildren("-side left -expand true -fill x",textureFlipR, textureFlipS, textureSeam);
        frame->addChildren("-side top -expand true -fill x",fr2);
        fr2->addChildren("-side left -expand true -fill x",b2,b3);

      }

      textureGUI->popupDialog();
      return 0;

    }

    // create texture mapper 
    vtkDataSetAlgorithm *tmapper=NULL;

    if (textureMode->getIndex() == 0) {

      tmapper = vtkTextureMapToCylinder::New();

      // set cylindrical mapping center
      if (textureCyl->getState()) 
      {
        double p1[3];
        p1[0] = ori[0] + Frange(atoi(P1x->getText()),(float)0.0,(float)dim[0]) * spc[0];
        p1[1] = ori[1] + Frange(atoi(P1y->getText()),(float)0.0,(float)dim[1]) * spc[1];
        p1[2] = ori[2] + Frange(atoi(P1z->getText()),(float)0.0,(float)dim[2]) * spc[2];
        double p2[3];
        p2[0] = ori[0] + Frange(atoi(P2x->getText()),(float)0.0,(float)dim[0]) * spc[0];
        p2[1] = ori[1] + Frange(atoi(P2y->getText()),(float)0.0,(float)dim[1]) * spc[1];
        p2[2] = ori[2] + Frange(atoi(P2z->getText()),(float)0.0,(float)dim[2]) * spc[2];
        dynamic_cast<vtkTextureMapToCylinder*>(tmapper)->SetPoint1(p1);
        dynamic_cast<vtkTextureMapToCylinder*>(tmapper)->SetPoint2(p2);
        dynamic_cast<vtkTextureMapToCylinder*>(tmapper)->SetAutomaticCylinderGeneration(0);
      } else {
        dynamic_cast<vtkTextureMapToCylinder*>(tmapper)->SetAutomaticCylinderGeneration(1);
      }

      // set texture seam
      if (textureSeam->getState())
        dynamic_cast<vtkTextureMapToCylinder*>(tmapper)->PreventSeamOn();
      else
        dynamic_cast<vtkTextureMapToCylinder*>(tmapper)->PreventSeamOff();

    } 

    if (textureMode->getIndex() == 1) {

      tmapper = vtkTextureMapToSphere::New();

      // set spherical mapping center
      if (sphereMode->getIndex() == 1) 
      {
        dynamic_cast<vtkTextureMapToSphere*>(tmapper)->SetCenter(center);
        dynamic_cast<vtkTextureMapToSphere*>(tmapper)->SetAutomaticSphereGeneration(0);
      } else {
        dynamic_cast<vtkTextureMapToSphere*>(tmapper)->SetAutomaticSphereGeneration(1);
      }

      // set texture seam
      if (textureSeam->getState())
        dynamic_cast<vtkTextureMapToSphere*>(tmapper)->PreventSeamOn();
      else
        dynamic_cast<vtkTextureMapToSphere*>(tmapper)->PreventSeamOff();
    }

     // transform texture coordinates
    vtkTransformTextureCoords *xform = vtkTransformTextureCoords::New();

    // transform texture coordinates
    if (textureMode->getIndex() == 2) {
       xform->SetInputConnection(polyDataFilter[current_surface]->GetOutputPort());
    } else {
       tmapper->SetInputConnection(polyDataFilter[current_surface]->GetOutputPort());
       xform->SetInputConnection(tmapper->GetOutputPort());
    }

    xform->SetOrigin(textureOrigin[0]->getValue(), textureOrigin[1]->getValue(), 1);
    xform->SetPosition(textureTranslate[0]->getValue(), textureTranslate[1]->getValue(), 1);
    xform->SetScale(textureScale[0]->getValue(), textureScale[1]->getValue(), 1);
    xform->SetFlipR(textureFlipR->getState());
    xform->SetFlipS(textureFlipS->getState());
    
    if (tmapper)
      tmapper->Delete();

    /* polydata mapper */
    vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
    mapper->SetInputConnection(xform->GetOutputPort());

    xform->Delete();

    for (int k=0;k<num_renderers;k++) {
      surfaceActor[k][current_surface]->SetMapper(mapper);
      //surfaceActor[k][current_surface]->SetTexture(texture);
    }
      
    mapper->Delete();

    return 1;
}

/* -------------------------------------------------------------------------*/
int vtkpxGUIPolyDataControl::doCurvature(int apply)
{

  if (polyData[current_surface]->GetNumberOfPoints()<2)
    {
      this->EventManager->messageBox("Cannot Compute Surface Curvature, no surface in memory!","Error Message ...");
      return 0;
    }

  if (apply==0)
      {
	if (curvatureGUI==NULL)
	  {
	    PXTkFrame* polyFrame=(PXTkFrame*)this->EventManager->getMainWindow();
	    curvatureGUI=new PXTkDialog(this->EventManager,polyFrame,"Curvature Controls",PXFalse);
	    curvatureGUI->initDisplay();
	    PXTkFrame* frame=(PXTkFrame*)curvatureGUI->getMainWindow();

	    curvatureType=new PXTkOptionMenu(curvatureGUI,frame,"Normal",-1);
	    curvatureType->addOption("Open Surface");
	    curvatureType->setIndex(0);

	    curvatureMode=new PXTkOptionMenu(curvatureGUI,frame,"ShapeIndex",-1);
	    curvatureMode->addOption("Mean");
	    curvatureMode->addOption("Gaussian");
	    curvatureMode->addOption("K1");
	    curvatureMode->addOption("K2");
	    curvatureMode->setIndex(0);

	    curvatureScale=new PXTkScale(curvatureGUI,frame,"Scale",-1,PXTrue);
	    curvatureScale->setRange(2.0,8.0);
	    curvatureScale->setResolution(1.0);
	    curvatureScale->setValue(4.0);

	    curvatureRange=new PXTkScale(curvatureGUI,frame,"Range",-1,PXTrue);
	    curvatureRange->setRange(0.1,5.0);
	    curvatureRange->setResolution(0.1);
	    curvatureRange->setValue(1.0);
	      
	    PXTkFrame* fr2=new PXTkFrame(curvatureGUI,frame);
	    PXTkButton* b2=new PXTkButton(this->EventManager,fr2,"Apply",321);
	    PXTkButton* b4=new PXTkButton(this->EventManager,fr2,"Undo",201);
	    PXTkButton* b3=new PXTkButton(curvatureGUI,fr2,"Close",3);
	    frame->addChildren("-side top -expand true -fill x",curvatureType,curvatureMode);
	    frame->addChildren("-side top -expand true -fill x",curvatureScale,curvatureRange,fr2);
	    fr2->addChildren("-side left -expand true -fill x",b2,b4,b3);
	  }
	
	curvatureGUI->popupDialog();
	return 0;
      }

  this->WatchOn();

  int opensurface=0;
  if (curvatureType->getIndex()==1)
    opensurface=1;

  int cmode=curvatureMode->getIndex();

  float range=curvatureRange->getValue();
  int scale=(int)curvatureScale->getValue();

  vtkpxSurfaceUtil::CalculateCurvatures(polyData[current_surface],scale,opensurface,cmode,range);

  this->WatchOff();
  return 1;
}
/* -------------------------------------------------------------------------*/

