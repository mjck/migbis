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


#include "vtkpxUtil.h"
#include "vtkpxGUIColormapEditor.h"
#include "vtkLineSource.h"
#include "vtkProbeFilter.h"
#include "vtkDataSetCollection.h"
#include "vtkPolyDataMapper.h"
#include "vtkCubeSource.h"
#include "vtkActor.h"
#include "vtkDataArray.h"
#include "vtkpxColorMapUtil.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkpxUtil.h"
#include "vtkPolyData.h"
#include "vtkCommand.h"
#include "vtkCamera.h"
#include "vtkPlane.h"
#include "vtkCellArray.h"
#include "vtkMath.h"
#include "vtkCoordinate.h"

//#include "vtkContourWidget.h"
//#include "vtkGenericRenderWindowInteractor.h"
#include "vtkPolyData.h"


#include "vtkCommand.h"
#include "vtkRegressionTestImage.h"
#include "vtkDebugLeaks.h"
//#include "vtkTestUtilities.h"
#include "vtkCamera.h"
#include "vtkPlane.h"
#include "vtkCellArray.h"
#include "vtkMath.h"
#include "vtkCoordinate.h"

using namespace bioimagesuite_legacy;
// ---------------------------------------------------------------------------------------------------------


vtkpxGUIColormapEditor* vtkpxGUIColormapEditor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIColormapEditor");
  if(ret)
    {
      return (vtkpxGUIColormapEditor*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIColormapEditor;
}

// Construct object with no children.
vtkpxGUIColormapEditor::vtkpxGUIColormapEditor()
{
  colormap=vtkLookupTable::New();
  internal_colormap=vtkLookupTable::New();
  owns_colormap=1;

  colormap_filename=new char[20];
  sprintf(colormap_filename,"new_colormap.cmap");
  colormap_type=NULL;
    
  //  scalar_bar=NULL;
  plot_actor=NULL;
  render_widget=NULL;

  editor_dialog=NULL;
  simple_parent=NULL;
  min_value=0;
  num_decimal_points=0;
  max_value=255;
  num_colors=256;

 /* this->redSpline = NULL;
  this->greenSpline=NULL;
  this->blueSpline =NULL;
  this->alphaSpline = NULL; */
  //  this->WidgetInteractor=NULL;

  this->contourRep_red = NULL;
  //  this->contourWidget_red = NULL;
  this->contourRep_green = NULL;
  //  this->contourWidget_green = NULL;

  this->contourRep_blue = NULL;
  //  this->contourWidget_blue = NULL;

  this->contourRep_alpha = NULL;
  //  this->contourWidget_alpha = NULL;

  this->contourRep_rgb = NULL;
  //  this->contourWidget_rgb = NULL;

  /*  this->pd_r = NULL;
  this->lines_r = NULL; 
  this->points_r = NULL;
  this->lineIndices_r = NULL; 

  this->pd_g = NULL;
  this->lines_g = NULL; 
  this->points_g = NULL;
  this->lineIndices_g = NULL; 

  this->pd_b = NULL;
  this->lines_b = NULL; 
  this->points_b = NULL;
  this->lineIndices_b = NULL; 

  this->pd_a = NULL;
  this->lines_a = NULL; 
  this->points_a = NULL;
  this->lineIndices_a = NULL; 

  this->pd_rgb = NULL;
  this->lines_rgb = NULL; 
  this->points_rgb = NULL;
  this->lineIndices_rgb = NULL; */

  this->lockunlock=NULL;
  volume_mode=0;
  fmri_mode=0;
  simple_mode=1;
  locked=0;
  strcpy(data_type,"Not Defined");
  this->contourOffset[0]=0;
  this->contourOffset[1]=0;
  this->contourOffset[2]=0;

  this->contourScale[0]=1;
  this->contourScale[1]=255;
  this->contourScale[2]=0;
  //this->activeShiftPoint-1;
  this->activeShiftPoint=1;
  this->activeColorContour=4;
}

vtkpxGUIColormapEditor::~vtkpxGUIColormapEditor()
{
  if (owns_colormap)
      colormap->Delete();

  internal_colormap->Delete();
  delete [] colormap_filename;
}
// --------------------------------------------------------------------------------------------------------- 
char*  vtkpxGUIColormapEditor::Initialize(const char* name,int inside)
{

  if (this->Initialized==1)
    return GetWidgetName();
  
  vtkpxGUIComponent::Initialize(name,inside);
  PXTkFrame* mainFrame=(PXTkFrame*)(this->EventManager->getMainWindow());


  if (!inside)
      {
	if (!simple_mode)
	  {
	    PXTkFrame* menubar=new PXTkFrame(this->EventManager,mainFrame);
	    mainFrame->addChildren("-side top -expand false -fill x -pady 5",menubar);
	    CreateMenu(menubar);
	  }
	
	PXTkFrame* otherFrame=new PXTkFrame(this->EventManager,mainFrame);
	otherFrame->configure("-bg","black");
	mainFrame->addChildren("-side bottom -expand true -fill both -pady 5 -padx 5",otherFrame);
	CreateRenderer(otherFrame);
	if (simple_mode)
	  CreateSimpleControls(otherFrame);
	else
	  CreateComplexControls(otherFrame);
      }
  else
      {
	simple_mode=1;
	CreateSimpleControls(mainFrame);
      }


  
  ResetSimpleColormap();
  DisplayColormap();
  sprintf(pxtk_buffer,"wm resizable %s 0 0",this->GetWidgetName());
  PXTkApp::executeTclCommand(pxtk_buffer);

  // By MPJ
  sprintf(pxtk_buffer,"wm protocol %s WM_DELETE_WINDOW { pxtkcallback %ld 3 }",this->GetWidgetName(),this->EventManager->getHandlerNo());
  PXTkApp::executeTclCommand(pxtk_buffer);
  // By MPJ

  return GetWidgetName();

}
// ---------------------------------------------------------------------------------------------------------
vtkLookupTable* vtkpxGUIColormapEditor::GetColormap()
{
  return colormap;
}
// ---------------------------------------------------------------------------------------------------------
void  vtkpxGUIColormapEditor::AutoSetNumDecimalPoints()
{
  float maxdiff=50.0;
  float newdiff=max_value-min_value;

  if (newdiff>50.0)
    {
      num_decimal_points=0;
      return;
    }

  if (newdiff<0.00001)
    {
      num_decimal_points=6;
      return;
    }

  num_decimal_points=1;
  while (newdiff < maxdiff && num_decimal_points<7)
    {
      ++num_decimal_points;
      newdiff*=10.0;
    }
  

}
// ---------------------------------------------------------------------------------------------------------
void  vtkpxGUIColormapEditor::SetColormap(vtkLookupTable* cmap)
{
  if (cmap==NULL)
    {
      fprintf(stderr,"\n\n\n Bad Setting of Colormap in vtkpxGUIColormapEditor\n\n\n\n\n");
      return;
    }

  if (owns_colormap)
    {
      colormap->Delete();
      owns_colormap=0;
    }
  
  colormap=cmap;
  internal_colormap->DeepCopy(colormap);

  double range[2];
  colormap->GetTableRange(range);
  min_value=range[0];
  max_value=range[1];
  this->AutoSetNumDecimalPoints();
  
  ResetColormap(1);

  /*  if (scalar_bar!=NULL)
      scalar_bar->SetLookupTable(colormap);*/

  if (render_widget!=NULL)
      {
	UpdateGraphFromColormap();
	render_widget->RenderDelay();
      }

  if (this->EventParent!=NULL && this->CallbackNo>0)
    this->EventParent->HandleEvent(CallbackNo);
  
  if (this->CallbackName!=NULL)
    PXTkApp::executeTclCommand(this->CallbackName);



}
// ---------------------------------------------------------------------------------------------------------
void  vtkpxGUIColormapEditor::SetVolumeMode(int vol)
{
  volume_mode=(vol>0);
}

// ---------------------------------------------------------------------------------------------------------
void  vtkpxGUIColormapEditor::SetAutoOpacity(int mode)
{
  this->auto_opacity_chkbtn->setState(mode ? PXTrue : PXFalse);
}

void  vtkpxGUIColormapEditor::SetFmriMode(int fmri)
{
  // range set changed from 7 to 9 to incorporate extra preset colormaps 
  fmri_mode=Irange(fmri,-1,8);

  int auto_opacity = this->auto_opacity_chkbtn->getState() == PXTrue; 

  int vol=0;
  if (volume_mode)
    vol=1;
  
  int outmode=0;


  
  if (fmri_mode>0 && fmri_mode<8)
    {		
      vtkpxColorMapUtil::SetFmriMergeColormap(colormap,fmri_mode-1,vol,1-auto_opacity);
      internal_colormap->DeepCopy(colormap);
      outmode=4;
    }
  else if (vol==0)
    {
      if (fmri_mode==0)
	{
	  if (min_value<-1.0 && max_value>1.0 && !volume_mode )
	    {
	      vtkpxColorMapUtil::SetConstantHueColorMap(colormap,0.5,0.02,0.58,
							Fmax( fabs(auto_min),fabs(auto_max)),
							min_value,max_value,256,vol,1-auto_opacity);
	      outmode=2;
	    }
	  else
	    {
	      vtkpxColorMapUtil::SetStepColorMap(colormap,auto_min,auto_max,min_value,max_value,256,vol,1-auto_opacity);
	      outmode=0;
	    }
	  internal_colormap->DeepCopy(colormap);
	}
      else if(fmri_mode==8)
	{				
	  vtkpxSurfaceUtil::DefaultObjectMapLookupTable(colormap,num_colors,1);
	  internal_colormap->DeepCopy(colormap);
	  outmode = 0; 
	}
      else
	{
	  vtkpxColorMapUtil::SetStepColorMap(colormap,min_value,max_value,min_value,max_value,256,vol,1-auto_opacity);
	  outmode=0;
	  internal_colormap->DeepCopy(colormap);
	}
    }
  else
    {
      if (fmri_mode==0)
	{
	  if (auto_min==min_value && auto_max==max_value)
	    {
	      double r=0.4*(max_value-min_value);
	      double r2=r/8.0;
	      vtkpxColorMapUtil::SetStepColorMap(colormap,min_value+r2,max_value-r,min_value+r2,max_value-r,256,vol,1-auto_opacity);
	      vtkpxColorMapUtil::SetStepColorMap(internal_colormap,min_value+r2,max_value-r,min_value,max_value,256,vol,1-auto_opacity);
	      outmode=0;
	    }
	  else
	    {
	      vtkpxColorMapUtil::SetStepColorMap(colormap,auto_min,auto_max,auto_min,auto_max,256,vol,1-auto_opacity);
	      vtkpxColorMapUtil::SetStepColorMap(internal_colormap,auto_min,auto_max,min_value,max_value,256,vol,1-auto_opacity);
	      outmode=0;
	    }
	}
      else if(fmri_mode==8)
	{					
	  vtkpxSurfaceUtil::DefaultObjectMapLookupTable(colormap,num_colors,1);
	  internal_colormap->DeepCopy(colormap);
	  outmode = 0; 
	  
	}
      else
	{
	  vtkpxColorMapUtil::SetStepColorMap(colormap,min_value,max_value,min_value,max_value,256,vol,1-auto_opacity);
	  outmode=0;
	  internal_colormap->DeepCopy(colormap);
	}
    }
  
  
  if (this->Initialized)
    {
      colormap_type->setIndex(outmode);
      this->ResetSimpleColormap(1);
      DisplayColormap();
    }
  
  
  if (this->EventParent!=NULL && this->CallbackNo>0)
    this->EventParent->HandleEvent(CallbackNo);
  
  if (this->CallbackName!=NULL)
    PXTkApp::executeTclCommand(this->CallbackName);
}

void  vtkpxGUIColormapEditor::SetSimpleMode(int simple)
{
  if (this->Initialized)
      return;
  simple_mode=(simple>0);
}
// ---------------------------------------------------------------------------------------------------------

void  vtkpxGUIColormapEditor::SetRange(int min,int max)
{
  min_value=Frange(float(min),-32767.0,32766.0);
  max_value=Frange(float(max),min_value,32767.0);
  auto_min=min_value;
  auto_max=max_value;
  this->AutoSetNumDecimalPoints();
  

  if (this->Initialized)
    {
      ResetSimpleColormap();
      SetSimpleColormapFromDialog();
    }
}
// ---------------------------------------------------------------------------------------------------------
void  vtkpxGUIColormapEditor::SetRange(vtkImageData* image)
{
  if (image==NULL)
    {
      vtkErrorMacro(<<"Bad Image\n");
      this->SetRange(0,255);
      return;
    }
  
  
  double range[2];  vtkDataArray* arr=image->GetPointData()->GetScalars();
  arr->GetRange(range);
  
  min_value=range[0];
  max_value=range[1];

  int m=arr->GetNumberOfComponents();
  //  if (m>4)    m=4;

  for (int k=1;k<m;k++)
    {
      arr->GetRange(range,k);
      min_value=Fmin(min_value,range[0]);
      max_value=Fmax(max_value,range[1]);
    }

  if (fabs(max_value-min_value)<1.0e-6)
    max_value=min_value+1.0e-6;


  this->AutoSetNumDecimalPoints();
  //  fprintf(stderr,"Done Scanning Components %f:%f, %d\n",range[0],range[1],num_decimal_points);


  switch (image->GetScalarType()) 
    {
    case 0:
      strcpy(data_type,"VTK_VOID");
      break;
    case 1:
      strcpy(data_type,"VTK_BIT");
      break;
    case 2:
      strcpy(data_type,"VTK_CHAR");
      break;
    case 3:
      strcpy(data_type,"VTK_UNSIGNED_CHAR");
      break;
    case 4:
      strcpy(data_type,"VTK_SHORT");
      break;
    case 5:
      strcpy(data_type,"VTK_UNSIGNED_SHORT");
      break;
    case 6:
      strcpy(data_type,"VTK_INT");
      break;
    case 7:
      strcpy(data_type,"VTK_UNSIGNED_INT");
      break;
    case 8:
      strcpy(data_type,"VTK_LONG");
      break;
    case 9:
      strcpy(data_type,"VTK_UNSIGNED_LONG");
      break;
    case 10:
      strcpy(data_type,"VTK_FLOAT");
      break;
    case 11:
      strcpy(data_type,"VTK_DOUBLE");
      break;
    default:
      strcpy(data_type,"Not Defined");
      break;
    }

  if (this->Initialized)
      {
	if (min_value<0)
	  colormap_type->setIndex(2,PXTrue);
	else
	  colormap_type->setIndex(0,PXTrue);
      }
  
  auto_min=min_value;
  auto_max=max_value;

  //  fprintf(stderr,"Going to Robust Range\n");
  vtkpxUtil::ImageRobustRange(image,0.01,0.99,auto_min,auto_max);


  


}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::SetPickedColorMapControl(int value)
{
  if(value < 0)
    value = 0;
  if(value > 4)
    value = 4; 

  this->activeColorContour=value;
  
  for (int ib=0;ib<=4;ib++)
    {
      if (ib==value)
	{
	  this->contourRep[ib]->SetPointScale(6.0);
	  this->contourRep[ib]->SetShowControls(1);
	  
	}
      else
	{
	  this->contourRep[ib]->SetPointScale(2.0);
	  this->contourRep[ib]->SetShowControls(0);
	}
      this->contourRep[ib]->Update();
    }

  return 1; 
}


// ---------------------------------------------------------------------------------------------------------
void  vtkpxGUIColormapEditor::SetNumberOfColors(int numc)
{
  num_colors=Irange(numc,2,2048);
  if (this->Initialized)
    {
      ResetSimpleColormap();
      SetSimpleColormapFromDialog();
    }
}

void  vtkpxGUIColormapEditor::SetLock(int lock)
{
  locked=(lock>0);
  /*if (locked)
    {
      if (editor_dialog!=NULL)
	editor_dialog->popdownDialog();
    }*/

  strcpy(pxtk_buffer,colormap_filename);
  if (locked)
    sprintf(pxtk_buffer2,"Colormap Editor (Locked) %s",PXTkApp::getFilenameTail(pxtk_buffer));
  else
    sprintf(pxtk_buffer2,"Colormap Editor (Un Locked) %s",PXTkApp::getFilenameTail(pxtk_buffer));


  if (this->lockunlock!=NULL)
    {
      if (locked)
	{
	  this->lockunlock->configure("-text","UnLock");
	  this->lockunlock->configure("-bg","red");
	  for (int i=0;i<=4;i++)
	    scale[i]->disable();
	  colormap_type->disable();
	}
      else
	{
	  this->lockunlock->configure("-text","Lock");
	  this->lockunlock->configure("-bg","gray");
	  for (int i=0;i<=4;i++)
	    scale[i]->enable();
	  colormap_type->enable();
	}
    }

  this->EventManager->setTitle(pxtk_buffer2);
	
}

void vtkpxGUIColormapEditor::Update()
{
  double range[2];
  colormap->GetTableRange(range);
  DisplayColormap();
}

// ---------------------------------------------------------------------------------------------------------
int  vtkpxGUIColormapEditor::HandleEvent(int event)
{
   // By MPJ
  if (event == 3) {
    sprintf(pxtk_buffer,"wm iconify %s",this->GetWidgetName());
    return PXTkApp::executeTclCommand(pxtk_buffer);
  } else {
    if (event<10)
      return this->EventManager->handleDirectEvent(event);
  }
  // By MPJ
  
  
  switch(event)
      {
      case 100:
	ResetSimpleColormap();
	DisplayColormap();
	break;

      case 101:
	{
	  char* line=this->EventManager->getOpenFilename("Colormap File name","Colormap Files","*.cmap*");
	  
	  if (line!=NULL)
	      {
		if (strlen(line)>0)
		  ReadColormap(line);
	      }
	}
	break;
	
      case 102:
	SaveColormap(colormap_filename);
	break;
	
      case 103:
	{
	  char* line=this->EventManager->getSaveFilename("Colormap File Name",colormap_filename,"Colormap Files","*.cmap");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		SaveColormap(line);
	    }
	}
	break;

      case 104:
	if (locked==0)
	  SetColormapFromDialog();
	break;
	
      case 105: // 
      case 106: //
      case 107: //
      case 108: //
      case 109:
      case 110:
      case 111:
      case 112:
      case 113:
      case 114:
	this->locked=0;
	SetFmriMode(event-106);
	break;
	

      case 118:
	if (editor_dialog!=NULL && locked==0)
	    editor_dialog->popupDialog();
	break;

      case 120:
      case 121:
      case 122:
      case 123:
      case 124:
      case 125:
      case 126:
      case 127:
	if (locked==0)
	  {
	    num_colors=(int)pow(float(2.0),float(event-116.0));
	    //	    fprintf(stderr,"New Number Of Colors=%d\n",num_colors);
	    ResetSimpleColormap();
	    ResetComplexColormap();
	  }
	break;


      case 130:
	{
	  int* x=render_widget->getRenderWindow()->GetSize();
	  int mouse=PXTkApp::getIntArgument(0);
	  int state=PXTkApp::getIntArgument(1);
	  int x1=PXTkApp::getIntArgument(2);
	  int x2=x[1]-PXTkApp::getIntArgument(3);
	  int ok=this->HandleMouseButtonEvent(mouse,state,x1,x2,this->EventManager);
	}
	break;

	

      case 150:
	{
	  char line[175];
	  sprintf(line,"Intensity Range %5.0f:%5.0f\n Data type=%s, num_colors=%d (or %d)",min_value,max_value,data_type,
		  colormap->GetNumberOfTableValues(),num_colors);
	  this->EventManager->messageBox(line);
	}
	break;
	
	  case 151: case 152: case 153: case 154: case 155:
	    SetPickedColorMapControl(event-151);
		break;

	  case 160:
		  this->SetLock((locked==0));
		  break;

      case 200:
	if (locked==0)
	  {
	    ResetSimpleColormap();
	    SetSimpleColormapFromDialog();
	    ResetComplexColormap();
	  }
	break;

      case 201: // change options
	if (locked==0)
	  {
	    if (colormap_type->getIndex()==0)
	      {
		if (scale[0]->getValue()>scale[1]->getValue())
		  scale[0]->setValue(scale[1]->getValue());
	      }
	    SetSimpleColormapFromDialog();
	    ResetComplexColormap();
	  }
	break;
	
      case 202:
	if (locked==0)
	  {
	    if (colormap_type->getIndex()==0 || colormap_type->getIndex()==3)
	      {
		if (scale[0]->getValue()>scale[1]->getValue())
		  scale[1]->setValue(scale[0]->getValue());
	      }
	    SetSimpleColormapFromDialog();
	    ResetComplexColormap();
	  }
	break;
	
      case 203:
	  case 204:
      case 205:
	if (locked==0)
	  {
	    SetSimpleColormapFromDialog();
	    ResetComplexColormap();
	  }
	break;
	
      case 131:

	if (render_widget!=NULL)
	  {
	    this->SetPickedColorMapControl(this->activeColorContour);
	    render_widget->Render();
	  }
	break;
	
      case 300:
      case 301:
      case 302:
      case 303:
      case 304:
      case 305:
      case 306:
      case 307:
	if (locked==0)
	  SetComplexColormapFromDialog(event-300);
	break;
	
      case 310:
	if (locked==0)
	  ResetComplexColormap();
	break;
	
	
      case 401:
	{
	  char* line=this->EventManager->getOpenFilename("Import Analyze Colormap","Analyze Colormap","*.lkup");
	  
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		ImportColormap(line);
	    }
	}
	break;
		
      case 402:
	{
	  char* line=this->EventManager->getSaveFilename("Export Analyze Colormap","colormap.lkup","Analyze Colormap Files","*.lkup");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		ExportColormap(line);
	    }
	}
	break;
	
   case 502:
    ResetSimpleColormap();
    SetSimpleColormapFromDialog();
    ResetComplexColormap();
    break;

      }

  if (event==101 || event==401 || event==105 || event==106  || event == 107 || event == 108 ||
      event==200 || event==201 || event==202 || event==203 || event==204 || event==205 ||
      event>=300 || (event>120 && event<128))
      {
	if (this->EventParent!=NULL && this->CallbackNo>0)
	  this->EventParent->HandleEvent(CallbackNo);
	
	if (this->CallbackName!=NULL)
	  PXTkApp::executeTclCommand(this->CallbackName);
      }


  return TCL_OK;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::CreateMenu(PXTkFrame* menubar)
{
  PXTkMenuButton* but1=new PXTkMenuButton(this->EventManager,menubar,"Colormap",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but1);
  
  PXTkMenu *menu1=new PXTkMenu(this->EventManager,but1,PXFalse);
  menu1->addButton("New",100);
  menu1->addSeparator();
  menu1->addButton("Load",101);
  menu1->addButton("Save",102);
  menu1->addButton("Save As",103);
  menu1->addSeparator();
  menu1->addButton("Import Analyze",401);
  menu1->addButton("Export Analyze",402);
  menu1->addSeparator();
  menu1->addButton("Close",3);

  PXTkMenuButton* but4=new PXTkMenuButton(this->EventManager,menubar,"Presets",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but4);
  
  PXTkMenu *menu4=new PXTkMenu(this->EventManager,but4,PXFalse);
  menu4->addButton("Reset",105);
  menu4->addButton("Normalized",106);
  menu4->addButton("Objectmap",114);
  menu4->addSeparator();
  menu4->addButton("Fmri 1",107);
  menu4->addButton("Fmri 2",108);
  menu4->addButton("Fmri 3",109);
  menu4->addButton("Fmri 4",112);
  menu4->addButton("Fmri 5",113);
  menu4->addButton("Spect 1",110);
  menu4->addButton("Spect 2",111);



  PXTkMenuButton* but3=new PXTkMenuButton(this->EventManager,menubar,"Levels",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but3);
  
  PXTkMenu *menu3=new PXTkMenu(this->EventManager,but3,PXFalse);
  menu3->addButton("Update",104);
  menu3->addButton("Info",150);
  menu3->addSeparator();
  menu3->addRadioButton("16",1,16,PXFalse,120);
  menu3->addRadioButton("32",1,32,PXFalse,121);
  menu3->addRadioButton("64",1,64,PXFalse,122);
  menu3->addRadioButton("128",1,128,PXFalse,123);
  menu3->addRadioButton("256",1,256,PXTrue,124);
  menu3->addRadioButton("512",1,512,PXFalse,125);
  menu3->addRadioButton("1024",1,1024,PXFalse,126);
  menu3->addRadioButton("2048",1,2048,PXFalse,127);

  if (!simple_mode)
      {
  PXTkMenuButton* but5=new PXTkMenuButton(this->EventManager,menubar,"RGBA",-1,PXTrue);
  menubar->addChildren("-side left -expand false ",but5);
  
  PXTkMenu *menu5=new PXTkMenu(this->EventManager,but5,PXFalse);
  menu5->addButton("Red",151);
  menu5->addButton("Green",152);
  menu5->addButton("Blue",153);
  menu5->addButton("Alpha",154);
  menu5->addButton("RGB",155);
  menu5->addSeparator();
  menu5->addButton("Slider Controls",118);
  }
  
  return 1;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::CreateSimpleControls(PXTkFrame* parframe)
{
  this->simple_parent=new PXTkFrame(this->EventManager,parframe);
  parframe->addChildren("-side top -fill x -expand false",simple_parent);
  
  PXTkFrame* bframe=new PXTkFrame(this->EventManager,this->simple_parent);
  this->simple_parent->addChildren("-side top -fill x -expand true",bframe);

  colormap_type=new PXTkOptionMenu(this->EventManager,bframe,"Step",200);
  colormap_type->addOption("Gamma");
  colormap_type->addOption("Constant-Hue");
  colormap_type->addOption("Overlay");
  colormap_type->addOption("Complex Overlay");
  colormap_type->addOption("Temperature");
  colormap_type->addOption("Rainbow");
  //  colormap_type->addOption("Reverse Rainbow");
  colormap_type->setIndex(0);

  //PXTkLabel* lab=new PXTkLabel(this->EventManager,bframe,"");
  //lab->configure("-image","$::pxtclvtkpxcontrib::smalllogo ");
  this->auto_opacity_chkbtn=new PXTkCheckButton(this->EventManager,bframe,"Auto opacity",502);  // mpj
  this->auto_opacity_chkbtn->setState(PXFalse);

  this->lockunlock=new PXTkButton(this->EventManager,bframe,"Lock",160);
  bframe->addChildren("-side right  -expand false -padx 1",auto_opacity_chkbtn,lockunlock);
  bframe->addChildren("-side left  -expand true -fill x",colormap_type);

  PXBool inside_parent=PXTrue;

  for (int i=0;i<=4;i++)
      {
	scale[i]=new PXTkArrowScale(this->EventManager,this->simple_parent,"Min:",201+i,inside_parent);
	scale[i]->setRange(min_value,max_value);
	scale[i]->setValue(0.5*(max_value+min_value));
	scale[i]->setDecimalPoints(1);
	scale[i]->setIncrement(1.0);
	scale[i]->setLengthWidth(180,10);
	this->simple_parent->addChildren("-side top -fill x -expand false",scale[i]->getMainWindow());
	packed[i]=1;

      }
  return 1;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::CreateEditorControls(PXTkFrame* edit)
{
  PXTkFrame* fr=new PXTkFrame(this->EventManager,edit);
  edit->addChildren("-side top -expand true -fill both",fr);

  fr->setPackMode(PXFalse);
  
  editor_type=new PXTkOptionMenu(this->EventManager,fr,"RGB",310);
  editor_type->addOption("Red");
  editor_type->addOption("Green");
  editor_type->addOption("Blue");
  editor_type->addOption("Opacity");
 
  fr->gridRowColumnChild(editor_type,0,0,1,2);
  
  int maxno=5;

  for (int i=0;i<=maxno;i++)
      {
	float vl=float(i)/float(maxno);
	if (i!=0 && i!=maxno)
    {
      int index=int(vl*float(num_colors)+0.5);
      levelscale[i]=new PXTkArrowScale(this->EventManager,fr,"Level",300+i,PXFalse);
      levelscale[i]->setRange(0.0,num_colors-1);
      levelscale[i]->setDecimalPoints(1);
      levelscale[i]->setResolution(1);
      levelscale[i]->setValue(index);
      
      levelscale[i]->setIncrement(1);
      levelscale[i]->setLengthWidth(150,10);
      fr->gridRowColumnChild(levelscale[i]->getMainWindow(),i+1,0);
    }

	valuescale[i]=new PXTkArrowScale(this->EventManager,fr,"Value",300+i,PXFalse);
	valuescale[i]->setRange(0.0,1.0);
	valuescale[i]->setDecimalPoints(3);
	valuescale[i]->setResolution(0.001);
	valuescale[i]->setValue(vl);
	valuescale[i]->setIncrement(0.01);
	valuescale[i]->setLengthWidth(150,10);
	fr->gridRowColumnChild(valuescale[i]->getMainWindow(),i+1,1);
      }
  return 1;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::CreateRenderer(PXTkFrame* frame)
{
#if defined(VTK_USE_CARBON)
  return 0;
#endif

  render_widget=new PXVTkTkRenderWidget(this->EventManager,frame,130);
  render_widget->setWidthHeight(400,400);
  render_widget->bindMouseEvents();
  frame->addChildren("-side top -expand false",render_widget);

  //vtkScalarBarActor *scalar_bar=vtkScalarBarActor::New();
  scalar_bar = vtkScalarBarActor::New(); 
  scalar_bar->SetLookupTable(colormap);
  scalar_bar->SetOrientationToHorizontal();
  scalar_bar->GetPositionCoordinate()->SetValue(0.0,0.0);
  scalar_bar->SetWidth(1.0);

  if (simple_mode)
    scalar_bar->SetHeight(0.35);
  else
    scalar_bar->SetHeight(0.15);

  scalar_bar->SetLabelFormat("%5.0f");
  scalar_bar->SetNumberOfLabels(10);


  render_widget->getRenderer()->AddActor2D(scalar_bar);

  render_widget->getRenderer()->SetBackground(0.4,0.4,0.4);
  //render_widget->getRenderer()->SetBackground(0.1,0.1,0.1);

  // Create and Bind Interactor
  // --------------------------

  this->contourRep_red = vtkpxBaseCurve::New();
  this->contourRep_green = vtkpxBaseCurve::New();
  this->contourRep_blue = vtkpxBaseCurve::New();
  this->contourRep_alpha = vtkpxBaseCurve::New();
  this->contourRep_rgb = vtkpxBaseCurve::New();
  this->contourRep_box = vtkpxBaseCurve::New();

  this->contourRep[0]=this->contourRep_red;
  this->contourRep[1]=this->contourRep_green;
  this->contourRep[2]=this->contourRep_blue;
  this->contourRep[3]=this->contourRep_alpha;
  this->contourRep[4]=this->contourRep_rgb;
  this->contourRep[5]=this->contourRep_box;


  vtkPoints* pts=vtkPoints::New();
  pts->SetNumberOfPoints(6);
  for (int ia=0;ia<pts->GetNumberOfPoints();ia++)
    pts->SetPoint(ia,0,0,0);
  

  for (int ib=0;ib<=5;ib++)
    {
      //      fprintf(stderr,"Creating Contour %d",ib);
      this->contourRep[ib]->SetPointScale(6.0);
      this->contourRep[ib]->SetClosedCurve(0);
      this->contourRep[ib]->SetShowControls((ib==4));
      this->contourRep[ib]->SetShowCurve(1);

      if (ib!=5)
	{
	  this->contourRep[ib]->SetFromPoints(pts);
	}
      else
	{
	  this->contourRep[5]->AddPoint(0,0,1,0);
	  this->contourRep[5]->AddPoint(255,0,1,0);
	  this->contourRep[5]->AddPoint(255,255,1,0);
	  this->contourRep[5]->AddPoint(255,0,1,0);
	  this->contourRep[ib]->SetClosedCurve(1);
	  this->contourRep[ib]->SetShowBoundingBox(1);
	}
      //      fprintf(stderr,"Done creating curve %d nump=%d\n",ib,this->contourRep[ib]->GetNumPoints());
  
      vtkPolyDataMapper* map=vtkPolyDataMapper::New();
      map->SetInput(this->contourRep[ib]->GetOutput());

      vtkActor* act=vtkActor::New();
      act->SetMapper(map);
      
      switch (ib)
	{
	case 0:  act->GetProperty()->SetColor(1,0,0); break;
	case 1:  act->GetProperty()->SetColor(0,1,0); break;
	case 2:  act->GetProperty()->SetColor(0,0,1); break;
	case 3:  act->GetProperty()->SetColor(1,1,0); break;
	case 4:  act->GetProperty()->SetColor(1,1,1); break;	
	case 5:  act->GetProperty()->SetColor(0.2,0.2,0.2); break;
	}
      /*      act->GetProperty()->SetRepresentationToWireframe();
      act->GetProperty()->SetLineWidth(2.0);
      act->GetProperty()->SetAmbient(1.0);
      act->GetProperty()->SetDiffuse(0.0);
      act->GetProperty()->SetSpecular(0.0);*/


      render_widget->getRenderer()->AddActor(act);
      act->SetVisibility(1);
      act->Delete();
      map->Delete();
    }
  pts->Delete();
  
  //  render_widget->getRenderer()->GetCamera()->ParallelProjectionOn();

  

  return 1;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::CreateComplexControls(PXTkFrame* frame)
{
  CreateSimpleControls(frame);

  editor_dialog=new PXTkDialog(this->EventManager,frame,"Slider Controls",PXFalse);
  editor_dialog->initDisplay();
  
  PXTkFrame* editorframe=(PXTkFrame*)editor_dialog->getMainWindow();
  CreateEditorControls(editorframe);

  PXTkButton* editorclose=new PXTkButton(editor_dialog,editorframe,"Close",3);
  editorframe->addChildren("-side bottom -expand false -fill x",editorclose);

  return 0;
}

// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::ReadColormap(const char* name)
{
  int ok=vtkpxColorMapUtil::LoadColormap(colormap,name);
  if (ok==0)
    return 0;

  /*double dat[2];  colormap->GetTableRange(dat);
  min_value=dat[0];
  max_value=dat[1];*/

  fmri_mode=0;
  delete [] colormap_filename;
  colormap_filename=new char[strlen(name)+1];
  strcpy(colormap_filename,name);

  internal_colormap->DeepCopy(colormap);

  DisplayColormap();
  locked=0;
  return TCL_OK;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::SaveColormap(const char* name)
{
  int ok=vtkpxColorMapUtil::SaveColormap(colormap,name);
  if (ok==0)
    return 0;

  if (strcmp(colormap_filename,name)!=0)
    {
      delete [] colormap_filename;
      colormap_filename=new char[strlen(name)+1];
      strcpy(colormap_filename,name);
    }
  return DisplayColormap();
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::ImportColormap(const char* name)
{
  int ok=vtkpxColorMapUtil::LoadAnalyzeColormap(colormap,name,volume_mode);
  if (ok==0)
    return 0;
  
  fmri_mode=0;
  locked=0;
  internal_colormap->DeepCopy(colormap);
  //  vtkpxColorMapUtil::CopyLookupTable(colormap,internal_colormap);
  DisplayColormap();
  return TCL_OK;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::ExportColormap(const char* name)
{
  if (colormap==NULL)
    return 0;
  
  if (colormap->GetNumberOfColors()!=256)
    {
	this->EventManager->messageBox(pxtk_buffer,"Cannot export colormap as it does not have 256 colors\n");
	return 0;
      }

  int ok=vtkpxColorMapUtil::SaveAnalyzeColormap(colormap,name);
  return DisplayColormap();
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::DisplayColormap()
{
  if (!this->EventManager->isInside())
    {
      strcpy(pxtk_buffer,colormap_filename);
      if (locked)
	sprintf(pxtk_buffer2,"Colormap Editor (Locked) %s",PXTkApp::getFilenameTail(pxtk_buffer));
      else
	sprintf(pxtk_buffer2,"Colormap Editor (Un Locked) %s",PXTkApp::getFilenameTail(pxtk_buffer));
      this->EventManager->setTitle(pxtk_buffer2);
      if (render_widget!=NULL)
	{
	  ResetColormap(1);
	  if (render_widget!=NULL)
	    render_widget->RenderDelay();
	}
    }
  return TCL_OK;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::UpdateGraphFromColormap()
{
  if (render_widget==NULL || colormap==NULL)
      return 0;

  int dimensions[3];

  int numc=colormap->GetNumberOfTableValues();
  if (numc<2)
    return 0;

  double rgba[4]; 
  
  int ncp=this->contourRep_red->GetNumPoints();
  int sc=(numc/(ncp-1));

  double z=this->contourOffset[2];

  //  fprintf(stderr,"Updating Graph From Colormap ncp=%d numc=%d sc=%d\n",ncp,numc,sc);

  double xmin,xmax,ymin,ymax;

  // Update Location Of Control Points
  for (int i = 0; i< ncp; i++)
    {

      // Index is index into colormap of current control point
      int index=i*sc;
      if (index>=numc)
	index=numc-1;

      // X-Coordinate on display
      double x=double(this->contourScale[0]*index+this->contourOffset[0]);

      colormap->GetTableValue(index, rgba); 

      // Y-Coordinate(s) on display are values for five bars R,G,B,A, RGB
      double y[5];
      for (int ib=0;ib<=3;ib++)
	y[ib]=double(this->contourScale[1])*rgba[ib]+double(this->contourOffset[1]);
      y[4]=0.0;
      for (int ib=0;ib<=2;ib++)
	y[4]+=rgba[ib];
      y[4]=double(this->contourScale[1])*(y[4]/3.0)+double(this->contourOffset[1]);

      this->contourRep_red->SetPoint(i,x,y[0],z,0);
      this->contourRep_green->SetPoint(i,x,y[1],z,0);
      this->contourRep_blue->SetPoint(i,x,y[2],z,0);
      this->contourRep_alpha->SetPoint(i,x,y[3],z,0);
      this->contourRep_rgb->SetPoint(i,x,y[4],z,0);

      if (i==0)
	{
	  xmin=x;
	  xmax=x;
	  ymin=y[0]; ymax=y[0];
	}

      if (x<xmin) xmin=x;
      if (x>xmax) xmax=x;
      for (int ia=0;ia<=4;ia++)
	{
	  if (ymin>y[ia]) ymin=y[ia];
	  if (ymax<y[ia]) ymax=y[ia];
	}

    }
  
  for (int ib=0;ib<=4;ib++)
    this->contourRep[ib]->Update();
  
  this->render_widget->getRenderer()->ResetCamera(xmin,xmax,ymin-50,ymax,0,5);
  return 1;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::ResetColormap(int controlsonly)
{
  if (controlsonly)
    num_colors=colormap->GetNumberOfColors();
      

  ResetSimpleColormap(controlsonly);
  ResetComplexColormap(controlsonly);
  return 1;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::ResetComplexColormap(int controlsonly)
{
  if (!this->Initialized)
      return 0;

  // Graph is directly paired to complex colormap sliders 

  if (num_colors>0)
    this->contourScale[0]=256/double(num_colors);

  this->UpdateGraphFromColormap();


  if (editor_dialog==NULL)
      return 0;

  //  fprintf(stderr,"Doing COmplex ...");
  
  int maxno=5;

  int mode=Irange(editor_type->getIndex(),1,4);
  
  for (int i=0;i<=maxno;i++)
    {
      double c[4];
      int index=int(double(i)/double(maxno)*double(num_colors)+0.5);
      colormap->GetTableValue(index,c);
      if (i!=0 && i!=maxno)
	{
	  levelscale[i]->setRange(0,num_colors-1);
	  levelscale[i]->setValue(index);
	}
      valuescale[i]->setValue(c[mode-1]);
    }

  //  fprintf(stderr,"Doing COmplex ... Done");
  return 1;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::ResetSimpleColormap(int controlsonly)
{
  if (!this->Initialized)
    return 0;
  
  int option=colormap_type->getIndex();
  int maxshow=4;
  
  int auto_opacity = this->auto_opacity_chkbtn->getState() == PXTrue;

  //  fprintf(stderr,"Values = %f:%f numdp=%d\n",min_value,max_value,num_decimal_points);
  
  
  switch(option)
    {
    case 0:
      {
	for (int i=0;i<=1;i++)
	  {
	    scale[i]->setRange(min_value,max_value);
	    scale[i]->setDecimalPoints(num_decimal_points);
	    if (max_value-min_value>32)
	      scale[i]->setIncrement(1.0);
	    else		 
	      scale[i]->setIncrement(double(max_value-min_value)/32.0);
	    scale[i]->enable();
	  }
      }
      scale[0]->setLabel("Min Intensity");
      scale[0]->setValue(min_value);
      scale[1]->setLabel("Max Intensity");
      scale[1]->setValue(max_value);
      maxshow=2;
      
      if (!controlsonly)
	{
	  vtkpxColorMapUtil::SetStepColorMap(colormap,min_value,max_value,min_value,max_value,num_colors,volume_mode,1-auto_opacity);
	  internal_colormap->DeepCopy(colormap);
	  //	    vtkpxColorMapUtil::CopyLookupTable(colormap,internal_colormap);
	}
      break;
    case 1:
      scale[0]->setLabel("Gamma");
      scale[0]->setRange(0.0,5.0);
      scale[0]->setValue(1.0);
      scale[0]->setDecimalPoints(1);
      scale[0]->setIncrement(0.2);
      scale[0]->enable();
      maxshow=1;
      if (!controlsonly)
	{
	  vtkpxColorMapUtil::SetGammaColorMap(colormap,1.0,min_value,max_value,num_colors,volume_mode,1-auto_opacity);
	  internal_colormap->DeepCopy(colormap);
	  //	    vtkpxColorMapUtil::CopyLookupTable(colormap,internal_colormap);
	}
      break;
      
    case 2:
      {
	for (int i=0;i<=2;i++)
	  {
	    scale[i]->setRange(0.0,1.0);
	    scale[i]->setIncrement(0.01);
	    scale[i]->setDecimalPoints(3);
	    scale[i]->enable();
	  }
      }
      scale[0]->setLabel("Intensity");
      scale[0]->setValue(0.5);
      scale[1]->setLabel("Positive Hue");
      scale[1]->setValue(0.02);
      scale[2]->setLabel("Negative Hue");
      scale[2]->setValue(0.58);
      scale[3]->setLabel("Cutoff");
      scale[3]->setRange(0.0,Fmax(fabs(min_value),fabs(max_value)));
      scale[3]->enable();
      scale[3]->setDecimalPoints(num_decimal_points);
      scale[3]->setIncrement(1.0);
      scale[3]->setValue(Fmax(fabs(min_value),fabs(max_value)));
      maxshow=4;
      if (!controlsonly)
	vtkpxColorMapUtil::SetConstantHueColorMap(colormap,0.5,0.02,0.58,scale[3]->getValue(),
						  min_value,max_value,num_colors,volume_mode,1-auto_opacity);
      internal_colormap->DeepCopy(colormap);
      break;
      
    case 3:
    case 4:
      {
	int nc=colormap->GetNumberOfColors();
	float maxv=55.0;
	if (nc==256)
	  maxv=191.0;
	
	for (int i=0;i<=1;i++)
	  {
	    scale[i]->setRange(0,maxv);
	    scale[i]->setDecimalPoints(num_decimal_points);
	    scale[i]->setIncrement(1.0);
	    scale[i]->enable();
	  }
	
	scale[0]->setLabel("Min Intensity");
	scale[0]->setValue(min_value);
	scale[1]->setLabel("Max Intensity");
	scale[1]->setValue(max_value);
	
	for (int i=2;i<=4;i++)
	  {
	    scale[i]->setRange(0.0,1.0);
	    scale[i]->setIncrement(0.01);
	    scale[i]->setDecimalPoints(3);
	    scale[i]->enable();
	  }
	
	scale[2]->setLabel("Intensity");
	scale[2]->setValue(1.0);
	scale[3]->setLabel("Positive Hue");
	scale[3]->setValue(0.0);
	scale[4]->setLabel("Negative Hue");
	scale[4]->setValue(0.58);
	
	if (option==3)
	  maxshow=5;
	else
	  maxshow=2;
	
	vtkpxColorMapUtil::CreateOverlayColorMap(colormap,
						 colormap->GetNumberOfColors(),
						 0,maxv,0,maxv,
						 1.0,0.0,0.58,volume_mode,(option==3),1-auto_opacity);
	internal_colormap->DeepCopy(colormap);
      }
      break;
      
    case 5:
    case 6:
    case 7:
      {
	//	fprintf(stderr,"Option=%d\n",option);
	for (int i=0;i<=1;i++)
	  {
	    scale[i]->setRange(min_value,max_value);
	    scale[i]->setDecimalPoints(num_decimal_points);
	    if (max_value-min_value>32)
	      scale[i]->setIncrement(1.0);
	    else		 
	      scale[i]->setIncrement(double(max_value-min_value)/32.0);
	    scale[i]->enable();
	  }
      
	scale[0]->setLabel("Min Intensity");
	scale[0]->setValue(min_value);
	scale[1]->setLabel("Max Intensity");
	scale[1]->setValue(max_value);
	maxshow=2;
	
	if (!controlsonly)
	  {	
	    if (option==6)
	      vtkpxColorMapUtil::SetRainbowColorMap(colormap,min_value,max_value,min_value,max_value,num_colors,volume_mode,1-auto_opacity);
	    else if (option==7)
	      vtkpxColorMapUtil::SetReverseRainbowColorMap(colormap,min_value,max_value,min_value,max_value,num_colors,volume_mode,1-auto_opacity);
	    else
		{
	      vtkpxColorMapUtil::SetBlackBodyColorMap(colormap,min_value,max_value,min_value,max_value,num_colors,volume_mode,1-auto_opacity);
		}

	  internal_colormap->DeepCopy(colormap);		


	  }
      }
      break;
      
    }
  
  for (int j=0;j<=4;j++)
    {
      if (j<maxshow)
	{
	  if (packed[j]==0)
	    {
	      scale[j]->enable();
	      this->simple_parent->addChildren("-side top -fill x -expand false",scale[j]->getMainWindow());
	      packed[j]=1;
	    }
	}
      else
	{
	  scale[j]->setLabel("Not Unused");
	  scale[j]->disable();
	  if (packed[j]==1)
	    this->simple_parent->unmapChild(scale[j]->getMainWindow());
	  packed[j]=0;
	}
    }
  
  //  fprintf(stderr,"Done Values = %f:%f numdp=%d\n",min_value,max_value,num_decimal_points);
  return TCL_OK;
}
// ---------------------------------------------------------------------------------------------------------
int  vtkpxGUIColormapEditor::SetColormapFromDialog()
{
  if (simple_mode)
    return SetSimpleColormapFromDialog();
  else
    return SetComplexColormapFromDialog(0);

}
// ---------------------------------------------------------------------------------------------------------
int  vtkpxGUIColormapEditor::SetComplexColormapFromDialog(int active)
{
  if (editor_dialog==NULL)
      return 0;

  if (active<0)
    {
      return ResetComplexColormap();
    }
  
  int maxno=5;

  if (active<=maxno-2 && active >=1) 
    {
      if (levelscale[active]->getValue()>levelscale[active+1]->getValue())
	levelscale[active]->setValue(levelscale[active+1]->getValue());
    }
  if (active>=2 && active <= maxno-1)
    {
      if (levelscale[active]->getValue()<levelscale[active-1]->getValue())
	levelscale[active]->setValue(levelscale[active-1]->getValue());
    }
  
  int mode=editor_type->getIndex();
  
  if (num_colors!=colormap->GetNumberOfColors())
    {
      colormap->SetNumberOfTableValues(num_colors);
      mode=0;
      }
  
  for (int act=0;act<maxno;act++)
    {
      int index1=0;
      if (act>0)
	index1=int(levelscale[act]->getValue());
      
      int index2=num_colors-1;
      if (act<maxno-1)
	index2=int(levelscale[act+1]->getValue());

      if (fmri_mode>0)
	{
	  int maxc=55;
	  if (fmri_mode==2)
	    maxc=239;
	  if (index2>maxc)
	    {
	      index2=maxc;
	      if (index1>maxc-1)
		index1=maxc-1;
	    }
	}

      int range=Irange(index2-index1,1,num_colors);
      double dval=valuescale[act+1]->getValue()-valuescale[act]->getValue();
      double bval=valuescale[act]->getValue();
      
      for (int i=index1;i<=index2;i++)
	{
	  double frac=(double(i)-double(index1))/double(range);
	  double val=Frange(frac*dval+bval,0.0,1.0);
	  
	  double c[4];
	  colormap->GetTableValue(i,c);

	  /*if (i==10 || i==20 || i == 30)
	    fprintf(stderr,"Mode=%d In Color %d %4.2f %4.2f %4.2f %4.2f\n",mode,i,c[0],c[1],c[2],c[3]);*/
	  
	  switch(mode)
	    {
	    case 0:
	      colormap->SetTableValue(i,val,val,val,c[3]);
	      break;
	    case 1:
	      colormap->SetTableValue(i,val,c[1],c[2],c[3]);
	      break;
	    case 2:
	      colormap->SetTableValue(i,c[0],val,c[2],c[3]);
	      break;		  
	    case 3:
	      colormap->SetTableValue(i,c[0],c[1],val,c[3]);
	      break;
	    case 4:
	      colormap->SetTableValue(i,c[0],c[1],c[2],val);
	      break;
	    }
	  colormap->GetTableValue(i,c);

	  /*if (i==10 || i==20 || i == 30)
	    fpintf(stderr,"Out Color  %d %4.2f %4.2f %4.2f %4.2f\n",i,c[0],c[1],c[2],c[3]);*/
	}
    }
  internal_colormap->DeepCopy(colormap);
  //  vtkpxColorMapUtil::CopyLookupTable(colormap,internal_colormap);
  UpdateGraphFromColormap();
  render_widget->RenderDelay();
  return 1;
}
// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::SetSimpleColormapFromDialog()
{
  int option=colormap_type->getIndex();
  if (colormap==NULL)
    {
      return 0;
    }

  int auto_opacity = this->auto_opacity_chkbtn->getState() == PXTrue;

  switch(option)
  {
  case 0 : // Step Color Map
	  vtkpxColorMapUtil::SetStepColorMap(colormap,scale[0]->getValue(),scale[1]->getValue(),scale[0]->getValue(),scale[1]->getValue(),
		  num_colors,volume_mode,1-auto_opacity);
	  vtkpxColorMapUtil::SetStepColorMap(internal_colormap,scale[0]->getValue(),scale[1]->getValue(),min_value,max_value,num_colors,volume_mode,1-auto_opacity);
	  break;
  case 1: // Gamma Color Map
	  vtkpxColorMapUtil::SetGammaColorMap(colormap,scale[0]->getValue(),min_value,max_value,num_colors,volume_mode,1-auto_opacity);
	  if (internal_colormap!=NULL && colormap!=NULL )
		  internal_colormap->DeepCopy(colormap);

	  break;
  case 2: // Hue ColorMap
	  vtkpxColorMapUtil::SetConstantHueColorMap(colormap,
		  scale[0]->getValue(),scale[1]->getValue(),scale[2]->getValue(),
		  scale[3]->getValue(),
		  min_value,max_value,num_colors,volume_mode,1-auto_opacity);
	  internal_colormap->DeepCopy(colormap);

	  break;
  case 3:
  case 4:
	  {
		  int nc=colormap->GetNumberOfColors();
		  float maxv=55.0;
		  if (nc==256)
			  maxv=191.0;

		  vtkpxColorMapUtil::CreateOverlayColorMap(colormap,colormap->GetNumberOfColors(),
			  scale[0]->getValue(),scale[1]->getValue(),0,maxv,
			  scale[2]->getValue(),scale[3]->getValue(),scale[4]->getValue(),
			  volume_mode,(option==3),1-auto_opacity);
		  internal_colormap->DeepCopy(colormap);
	  }
	  break;

  case 6:
	  vtkpxColorMapUtil::SetRainbowColorMap(colormap,scale[0]->getValue(),scale[1]->getValue(),scale[0]->getValue(),scale[1]->getValue(),
		  num_colors,volume_mode,1-auto_opacity);
	  vtkpxColorMapUtil::SetRainbowColorMap(internal_colormap,scale[0]->getValue(),scale[1]->getValue(),min_value,max_value,num_colors,volume_mode,1-auto_opacity);
	  break;

  case 7:
	  vtkpxColorMapUtil::SetReverseRainbowColorMap(colormap,scale[0]->getValue(),scale[1]->getValue(),scale[0]->getValue(),scale[1]->getValue(),
		  num_colors,volume_mode,1-auto_opacity);
	  vtkpxColorMapUtil::SetReverseRainbowColorMap(internal_colormap,scale[0]->getValue(),scale[1]->getValue(),min_value,max_value,num_colors,volume_mode,1-auto_opacity);
	  break;

  case 5:
	  vtkpxColorMapUtil::SetBlackBodyColorMap(colormap,scale[0]->getValue(),scale[1]->getValue(),scale[0]->getValue(),scale[1]->getValue(),
		  num_colors,volume_mode,1-auto_opacity);
	  vtkpxColorMapUtil::SetBlackBodyColorMap(internal_colormap,scale[0]->getValue(),scale[1]->getValue(),min_value,max_value,num_colors,volume_mode,1-auto_opacity);
	  break;

  }
  

  if (!this->EventManager->isInside())
      {
	if (render_widget!=NULL)
	  render_widget->RenderDelay();
      }


  return TCL_OK;
}

// ---------------------------------------------------------------------------------------------------------
int vtkpxGUIColormapEditor::HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev)
{
  //  fprintf(stderr,"Handling mouse Button event (nbutton=%d, state=%d x=%d, y=%d\n",nbutton,state,x,y);

  if (nbutton==3 && state==2)
    {
      int a=this->activeColorContour+1;
      if (a>4)
	a=0;
      this->SetPickedColorMapControl(a);
      this->render_widget->RenderDelay();
    }


  if (nbutton!=1 || state>2)
    return 0;

  // Find which one is active
  int active=0,done=0;
  while (active<=4 && done==0)
    {
      done=this->contourRep[active]->GetShowControls();
      if (done==0)
	++active;
    }

  if (done==0)
    return 0;

  // Convert pixels to 3D because these are now 3D Objects
  vtkCoordinate* coord=vtkCoordinate::New();
  coord->SetCoordinateSystemToDisplay();
  coord->SetValue(x,y,0.0);
  double* out=coord->GetComputedWorldValue(render_widget->getRenderer());
  
  double px[3];
  px[0]=out[0];
  px[1]=out[1];
  px[2]=0.0;
  
  // Find Closest Point (or -1 if none found 2.0=tolerance)
  if (state==0)
    {
      this->activeShiftPoint=this->contourRep[active]->FindClosestPoint(px,4.0,1);
      return 1;
    }

  // Otherwise either drag or reset
  if (this->activeShiftPoint==-1)
    return 0;

  this->SetLock(1);
  int np=this->contourRep[active]->GetNumPoints();
  int numc=colormap->GetNumberOfTableValues();
  int sc=(numc/(np-1));
  
  // Moving --> Editing Curve
  if (state==1)
    {
      // Change Colormap at this point
      // ------------------------------------------------------------
      int pt=this->activeShiftPoint;
      
      // Force X-Coordinate Bounds
      if (pt==0)
	{
	  px[0]=this->contourOffset[0];
	}
      else if (pt==np-1)
	{
	  px[0]=double(this->contourScale[0]*(np-1)*sc+this->contourOffset[0]);
	}
      else
	{
	  double x0,x1,x2;
	  this->contourRep[active]->GetPoint(pt-1,x0,x1,x2);
	  if (px[0]<x0)
	    px[0]=x0+this->contourScale[0];
	  this->contourRep[active]->GetPoint(pt+1,x0,x1,x2);
	  if (px[0]>x0)
	    px[0]=x0-this->contourScale[0];
	}

      // Force Y
      if (px[1]<this->contourOffset[1])
	px[1]=this->contourOffset[1];
      if (px[1]>this->contourScale[1])
	px[1]=this->contourScale[1];
      
      this->contourRep[active]->SetPoint(pt,px[0],px[1],px[2],1);
      if (active==4)
	{
	  for (int ia=0;ia<=2;ia++)
	    this->contourRep[ia]->SetPoint(pt,px[0],px[1],px[2],1);
	}


      this->contourRep[active]->Update();
      this->render_widget->RenderDelay();
      // End of Moving
      return 1;
    }

  int pt=this->activeShiftPoint;
  this->activeShiftPoint=-1;
  
  int min_p=pt-1; if (min_p<0) min_p=0;
  int max_p=pt+1; if (max_p==np) max_p=np-1;
  int gap=max_p-min_p+1;

  double vx[3],vy[3]; // Control Points from 0:gap-1 (0:1, 0:2)

  /*  fprintf(stderr,"\n\n\n Setting Colormap around point %d (%d:%d) gap=%d\n",
      pt,min_p,max_p,gap);*/
  
  for (int ia=0;ia<gap;ia++)
    {
      double x0,x1,x2;
      int ind=min_p+ia;
      this->contourRep[active]->GetPoint(ind,x0,x1,x2);
      vx[ia]=(x0-this->contourOffset[0])/this->contourScale[0];
      vy[ia]=(x1-this->contourOffset[1])/this->contourScale[1];
      //      fprintf(stderr,"Indexing ia=%d pt=%d (vx=%.2f vy=%.2f)\n",ia,ind,vx[ia],vy[ia]);
    }

  
  // Now Change Colormap
  for (int ia=0;ia<gap-1;ia++)
    {
      int xc[2];      double yc[2];
      xc[0]=int(vx[ia]);    yc[0]=vy[ia];
      xc[1]=int(vx[ia+1]);  yc[1]=vy[ia+1];
      
      if (xc[0]<0)	xc[0]=0;
      if (xc[0]>=numc)	xc[0]=numc-1;
      if (xc[1]<0)	xc[1]=0;
      if (xc[1]>=numc)	xc[1]=numc-1;
      
      //      fprintf(stderr,"Interpolating ia=%d (x=%d:%d y=%.2f:%.2f)\n",ia,xc[0],xc[1],yc[0],yc[1]);

      if (xc[1]!=xc[0])
	{
	  for (int cl=xc[0];cl<xc[1];cl++)
	    {
	      double rgba[4];
	      colormap->GetTableValue(cl, rgba); 
	      double w1=double(xc[1]-cl)/double(xc[1]-xc[0]);
	      double w2=1.0-w1;
	      double val=w1*yc[0]+w2*yc[1];	      

	      switch(active)
		{
		case 0: rgba[0]=val; break;
		case 1: rgba[1]=val; break;
		case 2: rgba[2]=val; break;
		case 3: rgba[3]=val; break;
		case 4:
		  rgba[0]=val;
		  rgba[1]=val;
		  rgba[2]=val;
		  break;
		}
	      colormap->SetTableValue(cl,rgba);
	    }
	}
    }

  if (this->EventParent!=NULL && this->CallbackNo>0)
    this->EventParent->HandleEvent(CallbackNo);
  
  if (this->CallbackName!=NULL)
    PXTkApp::executeTclCommand(this->CallbackName);
  
  scalar_bar->SetLookupTable(colormap);
  return 1;
}
// ---------------------------------------------------------------------------------------------------------

