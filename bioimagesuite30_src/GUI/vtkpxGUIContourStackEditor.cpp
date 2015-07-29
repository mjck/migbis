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
  Module:    $RCSfile: vtkpxGUIContourStackEditor.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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

#include "pxutil.h"
#include "vtkPolyData.h"
#include "vtkPolyDataWriter.h"
#include "pxcontourstack.h"
#include "pxfpbsplinestack.h"
#include "pxchamferdistancemap.h"
#include "vtkpxGUIContourStackEditor.h"
#include "vtkObjectFactory.h"
#include "vtkpxSurfaceUtil.h"
#include "vtkpxSplineStackSource.h"

vtkpxGUIContourStackEditor* vtkpxGUIContourStackEditor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIContourStackEditor");
  if(ret)
      {
	return (vtkpxGUIContourStackEditor*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIContourStackEditor;
}

// Construct object with no children.
vtkpxGUIContourStackEditor::vtkpxGUIContourStackEditor()
{
  num_contour_stacks=4;
  for (int i=0;i<CSE_MAX_STACKS;i++)
      surfaceStack[i]=NULL;

  multisliceGUI=NULL;
  exportAsSurfaceGUI=NULL;
  allow_multi_save=PXFalse;
  owns_surfaces=PXTrue;
  auto_save_mode=PXTrue;
  this->CreateHighlights=0;
}

vtkpxGUIContourStackEditor::~vtkpxGUIContourStackEditor()
{
  for (int i=0;i<CSE_MAX_STACKS;i++)
    if (surfaceStack[i]!=NULL)
      delete surfaceStack[i];
}
/* -------------------------------------------------------------------------*/
char* vtkpxGUIContourStackEditor::Initialize(const char* name,int inside)
{
  if (this->Initialized==1)
      return GetWidgetName();
  
  fprintf(stderr,"Initializing 2D Viewer");
  vtkpxGUI2DImageViewer::Initialize(name,inside);

  return GetWidgetName();
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIContourStackEditor::SetCurveControl(vtkpxGUIBaseCurveControl* cont)
{
  CurveEditor=cont;
  // Fot compatibility with vtkpxGUI2DImageViewer
  Editor=cont;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIContourStackEditor::ResetViewer()
{
  if (!(hasImage && this->Initialized))
      return 0;

  int dim[3];    
   currentImage->GetDimensions(dim);
  int numslices=dim[2];
  double sp[3],ori[3];
  currentImage->GetSpacing(sp);
  currentImage->GetOrigin(ori);
  
  for (int i=0;i<num_contour_stacks;i++)
    {
      if (owns_surfaces)
	{
	  if (surfaceStack[i]!=NULL)
	    delete surfaceStack[i];
	  surfaceStack[i]=new PXContourStack(numslices);
	}
      for (int k=0;k<surfaceStack[i]->getnumcontours();k++)
	surfaceStack[i]->setzlevel(k,float(k)*sp[2]+ori[2]);
      if (i==0)
	surfaceStack[i]->SaveNuages("init.cnt");
    }



  last_slice=0;
  if (multisliceGUI!=NULL)
      {
	int newdim[3];     
	currentImage->GetDimensions(newdim);
	beginMultislice->setRange(offset,newdim[2]-1+offset);
	beginMultislice->setValue(offset);
	endMultislice->setRange(offset,newdim[2]-1+offset);
	endMultislice->setValue(newdim[2]-1+offset);
      }

  return vtkpxGUI2DImageViewer::ResetViewer();
}
// -------------------------------------------------------------------------
int vtkpxGUIContourStackEditor::HandleEvent(int event)
{
  switch(event)
      {
      case 500:
	{
	  int ok=1;
	  int currentstack=CurveEditor->GetCurrentIndex();
	  if (surfaceStack[currentstack]->getnumpoints()>0)
	      ok=this->EventManager->questionBox("Loading will delete the current landmark set. Are you sure?");
	  if (ok)
	      {
		char* line=this->EventManager->getOpenFilename("Load Current Stack","Stack Files","*.cnt*");
		if (line!=NULL)
		    {
		      if (strlen(line)>0)
			  {
			    strcpy(pxtk_buffer,line);
			    PXContourStack* temp=new PXContourStack(2);

			    if (temp->LoadNuages(pxtk_buffer))
				{
				  for (int k=0;k<surfaceStack[currentstack]->getnumcontours();k++)
				      {
					if (k<temp->getnumcontours())
					    surfaceStack[currentstack]->getcontour(k)->copypoints(temp->getcontour(k));
					else
					    surfaceStack[currentstack]->getcontour(k)->DeleteAll();
				      }
				  
				  int currentslice=(int)sliceNumber->getValue();
				  CurveEditor->SetCollection(surfaceStack[currentstack]->getcontour(currentslice),
							     currentstack,2,currentslice);
				  last_slice=currentslice;
				  CurveEditor->UpdateStatus();
				}
			    else
				{
				  sprintf(pxtk_buffer,"Failed to load stack %d from %s",currentstack+1,pxtk_buffer);
				  this->EventManager->messageBox(pxtk_buffer,"Load Failure");
				}
			    delete temp;
			  }
		    }
	      }
	}
	break;
	
      case 502:
	{
	  int ok=1;
	      
	  int np=0;
	  for (int i=0;i<num_contour_stacks;i++)
	    np+=surfaceStack[i]->getnumpoints();
	  
	  if (np>0)
	    ok=this->EventManager->questionBox("Loading will delete your old stacks. Are you sure?");
	  
	  if (ok)
		{
		  char* line=this->EventManager->getOpenFilename("Load All Stacks","Stack Files","*.01.cnt*");
		  if (line!=NULL)
		      {
			if (strlen(line)>0)
			    {
			      strcpy(pxtk_buffer2,line);
			      char* line2=anaeatsuffix(pxtk_buffer2);
			      int currentslice=(int)sliceNumber->getValue();
			      
			      PXContourStack* temp=new PXContourStack(2);

			      for (int i=0;i<num_contour_stacks;i++)
				  {
				    
				    sprintf(pxtk_buffer,"%s.%02d.cnt",line2,i+1);
				    if (temp->LoadNuages(pxtk_buffer))
					{
					  for (int k=0;k<surfaceStack[i]->getnumcontours();k++)
					    {
					      if (k<temp->getnumcontours())
						surfaceStack[i]->getcontour(k)->copypoints(temp->getcontour(k));
					      else
						surfaceStack[i]->getcontour(k)->DeleteAll();
					    }
					  
					  CurveEditor->SetCollection(surfaceStack[i]->getcontour(currentslice),
								     i,2,currentslice);
					}
				    else
				      {
					sprintf(pxtk_buffer,"Failed to load stack %d from %s",i+1,pxtk_buffer);
					this->EventManager->messageBox(pxtk_buffer,"Load Failure");
				      }
				  }
			      delete temp;
			      last_slice=currentslice;
			      CurveEditor->UpdateStatus();
			    }
		      }
		}
	}
	break;
	
      case 501:
	{
	  char* line=this->EventManager->getSaveFilename("Save Current Stack","points.cnt","Stack Files","*.cnt*");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		{
		  strcpy(pxtk_buffer2,line);
		  int currentstack=CurveEditor->GetCurrentIndex();
		  UpdateFromEditor(PXTrue);
		  if (!surfaceStack[currentstack]->SaveNuages(pxtk_buffer2))
		    {
		      sprintf(pxtk_buffer,"Failed to save stack %d from %s",currentstack+1,line);
		      this->EventManager->messageBox(pxtk_buffer,"Save Failure");
		    }
		}
	    }
	}
	break;
	
      case 503:
	{
	  char* line=this->EventManager->getSaveFilename("Save All Stacks","points.01.cnt","Stack Files","*.01.cnt*");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		{
		  strcpy(pxtk_buffer2,line);
		  char* line2=anaeatsuffix(pxtk_buffer2);
		  UpdateFromEditor();
		  for (int i=0;i<num_contour_stacks;i++)
		    {
		      sprintf(pxtk_buffer,"%s.%02d.cnt",line2,i+1);
		      if (!surfaceStack[i]->SaveNuages(pxtk_buffer))
			{
			  sprintf(pxtk_buffer,"Failed to save stack %d from %s",i+1,pxtk_buffer);
			  this->EventManager->messageBox(pxtk_buffer,"Save Failure");
			}
		    }
		}
	    }
	}
	break;
	
      case 504:
	{
	  int currentstack=CurveEditor->GetCurrentIndex();
	  UpdateFromEditor(PXTrue);
	  int np=surfaceStack[currentstack]->getnumpoints();
	  sprintf(pxtk_buffer,"Stack %d has %d points",currentstack+1,np);
	  this->EventManager->messageBox(pxtk_buffer,"Stack Info");
	}
	break;
	
      case 505:
	{
	  PXTkApp::printToConsole("\n\n Area Info\n");
	  double sp[3];
	  currentImage->GetSpacing(sp);
	  int j=CurveEditor->GetCurrentIndex();
	  float total=0.0;
	  for (int i=0;i<surfaceStack[j]->getnumcontours();i++)
	      {
		float area=0.0;
		int np=surfaceStack[j]->getcontour(i)->getnumpoints();
		if (np>2)
		  {
		    PXContour* cntr=surfaceStack[j]->getcontour(i);
		    int wasclosed=cntr->isclosed();
		    cntr->setclosed(1);
		    area=cntr->getarea()*sp[0]*sp[1];
		    cntr->setclosed(wasclosed);
		    if (area>0.0)
		      {
			total+=area*sp[2];
			sprintf(pxtk_buffer,"slice =%d Area=%6.1f\n",i+offset,area);
			PXTkApp::printToConsole(pxtk_buffer);
		      }
		  }
	      }
	  sprintf(pxtk_buffer,"-------------------------------\nTotal Volume  = %6.1f\n",total);
	  PXTkApp::printToConsole(pxtk_buffer);
	  PXTkApp::popupConsole();
	}
	break;

      case 506:
      case 507:
	{
	  char* line;
	  if (event==506)
	    line=this->EventManager->getSaveFilename("Export Current Stack as surface","sur.vtk","vtk files","*.vtk");
	  else 
	    line=this->EventManager->getSaveFilename("Export Current Stack as tstack","sur.tstack","tstack files","*.tstack");

	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		{
		  strcpy(pxtk_buffer2,line);
		  int currentstack=CurveEditor->GetCurrentIndex();
		  UpdateFromEditor(PXTrue);
		  PXTriangulatedStack* tstack=new PXTriangulatedStack();
		  PXContourStack* tmp=new PXContourStack();
		  int numc=tmp->cleancopypoints(surfaceStack[currentstack]);
		  if (numc>1)
		    {
		      tmp->ensureAnticlockwise();
		      tstack->setfromcontourstack(tmp,0);
		      double sp[3];currentImage->GetSpacing(sp);
		      tstack->scalepoints(sp[0],sp[1]);
		      
		      if (event==506)
			{
			  vtkPolyData* temppoly=vtkPolyData::New();
			  vtkpxSurfaceUtil::drawtstack(temppoly,tstack);
			  vtkPolyDataWriter* writer=vtkPolyDataWriter::New();
			  writer->SetFileName(pxtk_buffer2);
			  writer->SetInput(temppoly);
			  this->WatchOn();
			  writer->Update();
			  this->WatchOff();
			  
			  writer->Delete();
			  temppoly->Delete();
			}
		      else
			{
			  int ok=tstack->Save(pxtk_buffer2);
			  if (!ok)
			    {
			      sprintf(pxtk_buffer2,"Failed to save surface in %s",pxtk_buffer2);
			      this->EventManager->messageBox(pxtk_buffer2,"Export Failure");
			    }
			}
		      delete tstack;
		    }
		  else
		    {
		      sprintf(pxtk_buffer3,"Failed to save surface in %s",pxtk_buffer2);
		      this->EventManager->messageBox(pxtk_buffer3,"Insufficient Points");
		    }
		  delete tmp;
		}
	    }
	}
	break;


      case 509:
	this->GetSurfaceFittingParameters();
	break;

      case 508:
	{
	  char* line=this->EventManager->getSaveFilename("Export Current Stack as bspline surface","surface.sur","surface files","*.sur");
	  if (line!=NULL)
	    {
	      if (strlen(line)>0)
		{
		  strcpy(pxtk_buffer2,line);
		  int ok=FitAndSaveBSplineSurface(pxtk_buffer2);
		  if (!ok)
		    {
		      sprintf(pxtk_buffer3,"Failed to save surface in %s",pxtk_buffer2);
		      this->EventManager->messageBox(pxtk_buffer3,"Fitting Failed");
		    }
		}
	    }
	}
	this->exportAsSurfaceGUI->popdownDialog();

	break;

      case 519:
	CreateThresholdStats();
	break;

      case 520:
	Operate(0);
	if (multisliceGUI!=NULL)
	  multisliceSelect->setIndex(0);
	break;

      case 521:
	Operate(0);
	if (multisliceGUI!=NULL)
	  multisliceSelect->setIndex(1);
	break;

      case 522:
	Operate(0);
	if (multisliceGUI!=NULL)
	  multisliceSelect->setIndex(2);
	break;

      case 523:
	Operate(0);
	if (multisliceGUI!=NULL)
	  multisliceSelect->setIndex(3);
	break;

      case 530:
	if (multisliceGUI!=NULL)
	  Operate(1);
	break;

      default:
	vtkpxGUI2DImageViewer::HandleEvent(event);
      }

  return TCL_OK;
}
// -------------------------------------------------------------------------
void vtkpxGUIContourStackEditor::ChangeSliceAndFrame(int sl,int fr)
{
  if (!hasImage)
      return;

  if (sl!=-1)
    sliceNumber->setValue(sl);
  
  int currentslice=(int)sliceNumber->getValue()-offset;

  if (this->Editor!=NULL)
    {
      if (last_slice!=currentslice)
	{
	  for (int i=0;i<num_contour_stacks;i++)
	    {
	      if (last_slice!=-1)
		(CurveEditor->GetCollection(i))->StoreInContour(surfaceStack[i]->getcontour(last_slice),2);
	      CurveEditor->SetCollection(surfaceStack[i]->getcontour(currentslice),
					 i,2,currentslice);
	    }
	  last_slice=currentslice;
	  CurveEditor->UpdateStatus();
	}
      
      int bl=(int)sliceNumber->getValue()-offset;
      int plane=2;
      this->Editor->SetImage(currentImage,plane,bl);
    }
  UpdateViewer();
  this->UpdateDisplay();
}
// -------------------------------------------------------------------------
void vtkpxGUIContourStackEditor::InitMenus(const char* parentname)
{
  PXTkFrame* menuBar=new PXTkFrame(NULL,parentname);
  PXTkMenuButton* mbut1=new PXTkMenuButton(this->EventManager,menuBar,"Stack",-1,PXTrue);
  menuBar->addChildren("-side left -expand false ",mbut1);
  PXTkMenu *stmenu=new PXTkMenu(this->EventManager,mbut1,PXFalse);
  

  stmenu->addButton("Load Stack",500);
  stmenu->addButton("Save Stack",501);
  stmenu->addSeparator();
  stmenu->addButton("Export .vtk",506);
  stmenu->addButton("Export .tstack",507);
  stmenu->addButton("Export .sur",509);
  stmenu->addSeparator();
  stmenu->addButton("Area Info",505);

  if (allow_multi_save)
      {
	stmenu->addSeparator();
	stmenu->addButton("Load All Stacks",502);
	stmenu->addButton("Save All Stacks",503);
	stmenu->addSeparator();
      }
  //stmenu->addCascade("Info",infoMenu);

  PXTkMenuButton* mbut2=new PXTkMenuButton(this->EventManager,menuBar,"Multi-Slice",-1,PXTrue);
  menuBar->addChildren("-side left -expand false ",mbut2);
  PXTkMenu *stmenu2=new PXTkMenu(this->EventManager,mbut2,PXFalse);

  stmenu2->addButton("Interpolate",520);
  stmenu2->addButton("Extract Iso",521);
  stmenu2->addButton("Auto-Snake",522);
  stmenu2->addButton("Batch-Snake",523);
  
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIContourStackEditor::Operate(int apply)
{
  if (!hasImage)
      return;
  
  if (apply==0)
    {
      if (multisliceGUI==NULL)
	{
	  PXTkFrame* polyFrame=(PXTkFrame*)(this->EventManager->getMainWindow());
	  multisliceGUI=new PXTkDialog(this->EventManager,polyFrame,"Multislice Controls",PXFalse);
	  multisliceGUI->initDisplay();
	  PXTkFrame* frame=(PXTkFrame*)multisliceGUI->getMainWindow();
	  
	  multisliceSelect=new PXTkOptionMenu(multisliceGUI,frame,"Interpolate",-1);
	  multisliceSelect->addOption("Extract Iso");
	  multisliceSelect->addOption("Auto-Snake");
	  multisliceSelect->addOption("Batch-Snake");
	  multisliceSelect->setIndex(0);
	  
	  beginMultislice=new PXTkArrowScale(multisliceGUI,frame,"Begin Slice",-1,PXTrue);
	  beginMultislice->setRange(offset,99+offset);
	  beginMultislice->setIncrement(1);
	  beginMultislice->setResolution(1);
	  beginMultislice->setLengthWidth(120,8);
	  beginMultislice->setValue(0);
	  
	  endMultislice=new PXTkArrowScale(multisliceGUI,frame,"End Slice",-1,PXTrue);
	  endMultislice->setRange(0+offset,99+offset);
	  endMultislice->setIncrement(1);
	  endMultislice->setResolution(1);
	  endMultislice->setLengthWidth(120,8);
	  endMultislice->setValue(99);
	  
	  PXTkFrame* fr2=new PXTkFrame(multisliceGUI,frame);
	  PXTkButton* b1=new PXTkButton(this->EventManager,fr2,"Apply",530);
	  PXTkButton* b3=new PXTkButton(multisliceGUI,fr2,"Close",3);
	  frame->addChildren("-side top -expand true -fill x",
			     multisliceSelect,
			     beginMultislice->getMainWindow(),
			     endMultislice->getMainWindow(),
			     fr2);
	  fr2->addChildren("-side left -expand true -fill x",b1,b3);
	}
      
      int dim[3];     
      currentImage->GetDimensions(dim);
      beginMultislice->setRange(0+offset,dim[2]-1+offset);
      endMultislice->setRange(0+offset,dim[2]-1+offset);
      multisliceGUI->popupDialog();
      return;
    }
  
  
  if (multisliceGUI==NULL)
    {
      this->Operate(0);
      multisliceGUI->popdownDialog();
    }
  
  if (apply==1)
    {
      int begin=(int)beginMultislice->getValue()-offset;
      int end=  (int)endMultislice->getValue()-offset;
      int mode=multisliceSelect->getIndex();
      switch (mode)
	{
	case 0:
	  Interpolate(begin,end);
	  break;
	case 1:
	  Extract(begin,end);
	  break;
	case 2:
	  Snake(begin,end,0);
	  break;
	case 3:
	  Snake(begin,end,1);
	  break;
	}
    }
  return;
  
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIContourStackEditor::Interpolate(int begin,int end)
{
  if (begin==end)
    return 0;
  
  if (begin>end)
    {
      int temp=end;
      end=begin;
      begin=temp;
    }
  
  if (end-begin<2)
    return 0;
  
  
  int currentslice=(int)sliceNumber->getValue();  
  UpdateFromEditor(PXTrue);

  int currentstack=CurveEditor->GetCurrentIndex();
  PXContour* cntr1=new PXContour();
  PXContour* cntr2=new PXContour();
  
  cntr1->copypoints(surfaceStack[currentstack]->getcontour(begin));
  cntr2->copypoints(surfaceStack[currentstack]->getcontour(end));

  if (cntr1->getnumpoints()<3 || cntr2->getnumpoints()<3)
      {
	this->EventManager->messageBox("Seed contours do not have enough points");
	delete cntr1;
	delete cntr2;
	return 0;
      }

  cntr1->Equispace(1.0);
  cntr2->Equispace(1.0);
  
  PXChamferDistanceMap* map1=new PXChamferDistanceMap(cntr1,1);
  PXChamferDistanceMap* map2=new PXChamferDistanceMap(cntr2,1);
  
  for (int slice=begin+1;slice<=end-1;slice++)
    {
      int fraction1=end-slice;
      int fraction2=slice-begin;
      PXChamferDistanceMap* map=new PXChamferDistanceMap(map1,map2,fraction1,fraction2);
      PXContour* cntr=map->extractContour();
      (surfaceStack[currentstack]->getcontour(slice))->copypoints(cntr);
      this->ChangeSliceAndFrame(slice);
      this->UpdateDisplay();
      delete map;
      delete cntr;
    }
  delete map1;
  delete map2;
  delete cntr1;
  delete cntr2;

  UpdateFromEditor(PXTrue);
  this->ChangeSliceAndFrame(currentslice);
  return 0;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIContourStackEditor::Snake(int begin,int end,int mode)
{
  int currentslice=(int)sliceNumber->getValue();  
  int temp;
  if (begin>end)
      {
	temp=end;
	end=begin;
	begin=temp;
      }

  if (mode==0)
      {
	if (currentslice<begin || currentslice>end)
	    {
	      this->EventManager->messageBox("Current Slice not in Range Specified!");
	      return 0;
	    }
	
	if (begin<currentslice)
	    {
	      for (int slice=currentslice-1;slice>=begin;slice-=1)
		  {
		    UpdateFromEditor(PXTrue);
		    this->CopySlice(slice+1,slice);
		    this->ChangeSliceAndFrame(slice);
		    CurveEditor->DoSnake(1);
		    this->UpdateDisplay();
		  }
	    }
	
	if (end>currentslice)
	    {
	      for (int slice=currentslice+1;slice<=end;slice++)
		  {
		    UpdateFromEditor(PXTrue);
		    this->CopySlice(slice-1,slice);
		    this->ChangeSliceAndFrame(slice);
		    CurveEditor->DoSnake(1);
		    this->UpdateDisplay();
		  }
	    }
      }
  else
      {
	for (int slice=begin;slice<=end;slice++)
	    {
	      this->ChangeSliceAndFrame(slice);
	      CurveEditor->DoSnake(1);
	      this->UpdateDisplay();
	    }
      }
  UpdateFromEditor(PXTrue);
  this->ChangeSliceAndFrame(currentslice);
  return 1;
}
/* -------------------------------------------------------------------------*/ 
int vtkpxGUIContourStackEditor::Extract(int begin,int end)
{
  int currentslice=(int)sliceNumber->getValue();  
  int temp;
  if (begin>end)
      {
	temp=end;
	end=begin;
	begin=temp;
      }


  if (currentslice<begin || currentslice>end)
      {
	this->EventManager->messageBox("Current Slice not in Range Specified!");
	return 0;
      }

  int currentstack=CurveEditor->GetCurrentIndex();
  int np=(CurveEditor->GetCollection(currentstack))->GetNumPoints();
  if (np<1)
      {
	this->EventManager->messageBox("No seed point specified on current slice");
	return 0;
      }
	
  if (begin<currentslice)
      {
	for (int slice=currentslice-1;slice>=begin;slice-=1)
	    {
	      UpdateFromEditor(PXTrue);
	      this->CopySlice(slice+1,slice);
	      this->ChangeSliceAndFrame(slice);
	      CurveEditor->DoExtract(1);
	      this->UpdateDisplay();
	    }
      }
  
  if (end>currentslice)
      {
	for (int slice=currentslice+1;slice<=end;slice++)
	    {
	      UpdateFromEditor(PXTrue);
	      this->CopySlice(slice-1,slice);
	      this->ChangeSliceAndFrame(slice);
	      CurveEditor->DoExtract(1);
	      this->UpdateDisplay();
	    }
      }

  UpdateFromEditor(PXTrue);
  this->ChangeSliceAndFrame(currentslice);
  return 1;
}
/* -------------------------------------------------------------------------*/ 
int vtkpxGUIContourStackEditor::CopySlice(int source,int target)
{
  int currentstack=CurveEditor->GetCurrentIndex();
  PXContour* dest=surfaceStack[currentstack]->getcontour(target);
  PXContour* src=surfaceStack[currentstack]->getcontour(source);
  if (dest!=src)
      dest->copypoints(src);
  return 1;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIContourStackEditor::UpdateFromEditor(PXBool currentonly)
{
  int currentslice=(int)sliceNumber->getValue();

  if (currentonly)
      {
	int currentstack=CurveEditor->GetCurrentIndex();
	(CurveEditor->GetCollection(currentstack))->StoreInContour(surfaceStack[currentstack]->getcontour(currentslice),2);
      }
  else
      {
	for (int i=0;i<num_contour_stacks;i++)
	  (CurveEditor->GetCollection(i))->StoreInContour(surfaceStack[i]->getcontour(currentslice),2);
      }
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIContourStackEditor::CreateThresholdStats()
{
  int currentstack=CurveEditor->GetCurrentIndex();
  UpdateFromEditor(PXTrue);

  PXTkApp::printToConsole("\n\n-------------------------------\n");
  sprintf(pxtk_buffer,"Area Info Based on Point Set %d\n",currentstack);
  PXTkApp::printToConsole(pxtk_buffer);
  PXTkApp::printToConsole("-------------------------------\n");

  
  vtkpxBaseCurve* templand=vtkpxBaseCurve::New();

  for (int j=0;j<surfaceStack[currentstack]->getnumcontours();j++)
      {
	int np=surfaceStack[currentstack]->getcontour(j)->getnumpoints();
	if (np>2)
	    {
	      PXContour* cntr=surfaceStack[currentstack]->getcontour(j);
	      int wasclosed=cntr->isclosed();
	      cntr->setclosed(1);
	      templand->SetFromContour(cntr,2,j);
	      cntr->setclosed(wasclosed);

	      float mean,sigma;
	      templand->ExtractVOIProperties(currentImage,2,j,this->CurrentFrame,mean,sigma);
	      //	      float threshold=mean+2.0*sigma;

	      /*PXVTkImage::thresholdStats(currentImage,2,j,
		threshold,numin,total);*/
	      
	      /*sprintf(pxtk_buffer,"Slice =%d Threshold =%5.1f (mean=%5.1f sigma=%5.1f) Pixels = %d/%d %5.2f\n",
		      j,threshold,mean,sigma,numin,total,float(numin)/float(total)*100.0);
	      PXTkApp::printToConsole(pxtk_buffer);*/
	    }
      }
  PXTkApp::popupConsole();
  templand->Delete();
  return;
}

void vtkpxGUIContourStackEditor::ExportToPolyData(vtkPolyData* poly,int stack)
{
  if (poly==NULL || this->Initialized==0)
    return;

  if (stack==-1)
    stack=CurveEditor->GetCurrentIndex();
  else
    stack=Irange(stack,0,num_contour_stacks-1);
  PXTriangulatedStack* tstack=new PXTriangulatedStack();
  tstack->setfromcontourstack(surfaceStack[stack],0);
  vtkpxSurfaceUtil::drawtstack(poly,tstack);
  delete tstack;
}

int vtkpxGUIContourStackEditor::CreateObjectMap(vtkImageData* img,int begin,int end)
{
  if (img==NULL || currentImage==NULL)
    return 0;

  if (begin==-1)
    begin=0;
  
  if (end==-1)
    end=num_contour_stacks-1;

  img->DeepCopy(currentImage);

  int dim[3];
  img->GetDimensions(dim);

  vtkDataArray* scal=img->GetPointData()->GetScalars();
  scal->FillComponent(0,0.0);

  int totalp=0;

  for (int st=begin;st<=end;st++)
    {
      for (int k=0;k<surfaceStack[st]->getnumcontours();k++)
      {
	int np=surfaceStack[st]->getcontour(k)->getnumpoints();
	if (np>2)
	    {
	      int base_offset=k*dim[0]*dim[1];
	      PXContour* cntr=surfaceStack[st]->getcontour(k);
	      int wasclosed=cntr->isclosed();
	      cntr->setclosed(1);
	      for (int ia=0;ia<dim[0];ia++)
		for (int ib=0;ib<dim[1];ib++)
		  {
		    if (cntr->IsInside(float(ia),float(ib)))
		      {
			int index=base_offset+ib*dim[0]+ia;
			scal->SetComponent(index,0,(1+st)*10);
			totalp++;
		      }
		  }
	      cntr->setclosed(wasclosed);
	    }
      }

      //fprintf(stderr,"st=%d totalp=%d\n",st,totalp);

    }
  


  return 1;
}
/* -------------------------------------------------------------------------*/
void vtkpxGUIContourStackEditor::GetSurfaceFittingParameters()
{
  if (!hasImage)
    return;

  if (exportAsSurfaceGUI==NULL)
    {
      PXTkFrame* polyFrame=(PXTkFrame*)(this->EventManager->getMainWindow());
      exportAsSurfaceGUI=new PXTkDialog(this->EventManager,polyFrame,"Export As B-spline Surface Controls",PXFalse);
      exportAsSurfaceGUI->initDisplay();
      PXTkFrame* frame=(PXTkFrame*)exportAsSurfaceGUI->getMainWindow();
      
      surfaceSmoothness=new PXTkOptionMenu(exportAsSurfaceGUI,frame,"Very Low Smoothness",-1);
      surfaceSmoothness->addOption("Low Smoothness");
      surfaceSmoothness->addOption("Medium Smoothness");
      surfaceSmoothness->addOption("Medium High Smoothness");
      surfaceSmoothness->addOption("High Smoothness");
      surfaceSmoothness->addOption("Extremely High Smoothness");
      surfaceSmoothness->setIndex(0);
      
      PXTkFrame* fr2=new PXTkFrame(exportAsSurfaceGUI,frame);
      PXTkButton* b1=new PXTkButton(this->EventManager,fr2,"Apply",508);
      PXTkButton* b3=new PXTkButton(exportAsSurfaceGUI,fr2,"Close",3);
      frame->addChildren("-side top -expand true -fill x",
			 surfaceSmoothness,
			 fr2);
      fr2->addChildren("-side left -expand true -fill x",b1,b3);
      fprintf(stderr,"Here\n");
    }
  
  exportAsSurfaceGUI->popupDialog();
  return;
}
/* -------------------------------------------------------------------------*/
int vtkpxGUIContourStackEditor::FitAndSaveBSplineSurface(const char* filename)
{
  float smoothness=2.0;
  switch (surfaceSmoothness->getIndex())
    {
    case 1:
      smoothness=5.0;
      break;
    case 2:
      smoothness=10.0;
      break;
    case 3:
      smoothness=15.0;
      break;
    case 4:
      smoothness=20.0;
      break;
    case 5:
      smoothness=50.0;
      break;
    }

  

  int currentstack=CurveEditor->GetCurrentIndex();
  UpdateFromEditor(PXTrue);

  double sp[3];currentImage->GetSpacing(sp);
  float sx=sp[0];
  float sy=sp[1];

  vtkpxSplineStackSource* sur=vtkpxSplineStackSource::New();
  int ok=sur->FitToContourStack(surfaceStack[currentstack],smoothness,smoothness/20.0,-1,1);

  if (ok)
    {
      for (int i=sur->GetTrimBottom(); i<=sur->GetTrimTop();i++)
	sur->GetSplineStack()->getspline(i)->scaleshift2D(0,0,sx,sy);
      
      int ok2=sur->Save(filename);
      
      sur->Delete();
      return ok2;
    }

  return ok;
}

