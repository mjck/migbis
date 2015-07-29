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

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu

   see pxvtkgadgets.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#include "pxvtkgadgets.h"
#include "pxtkeventobj.h"
#include "pxtkdialog.h"
#include "vtkTclUtil.h"
#include "vtkRenderWindow.h"

/* -------------------------------------------------------------------------*/
/*                   PXVTkTkGadget                                          */
/* -------------------------------------------------------------------------*/

PXVTkTkGadget::PXVTkTkGadget(PXTkEventObj* evpar,PXTkGadget* gadg_par,int moption):PXTkGadget(evpar,gadg_par)
{
  call_no=moption;
}

int PXVTkTkGadget::setWidthHeight(int w,int h)
{
  int a=TCL_OK;
  if (w>0)
      a=configure("-width",w);
  if (h>0 && a==TCL_OK)
      a=configure("-height",w);
  return a;
}
 
int PXVTkTkGadget::setPadding(int bd,int padx,int pady)
{
  int a=TCL_OK;
  if (bd>=0)
      a=configure("-borderwidth",bd);
  if (padx>=0 && a==TCL_OK)
      a=configure("-padx",padx);
  if (pady>=0 && a==TCL_OK)
      a=configure("-pady",pady);
  return a;
}

int PXVTkTkGadget::bindMouseEvents()
{
  if (!has_event_parent)
      return 0;

  strcpy(pxtk_buffer2," %x %y");
  for (int mouse=1;mouse<=3;mouse++)
      {
	sprintf(pxtk_buffer,"bind %s <ButtonPress-%d> { pxtkcallback %ld %d %d 0 %s %d }",
		getWidgetName(),mouse,
		event_parent->getHandlerNo(),call_no,mouse,pxtk_buffer2,object_id);
	PXTkApp::executeTclCommand(pxtk_buffer);
	
	sprintf(pxtk_buffer,"bind %s <B%d-Motion> { pxtkcallback %ld %d %d 1 %s %d }",
		getWidgetName(),mouse,
		event_parent->getHandlerNo(),call_no,mouse,pxtk_buffer2,object_id);
	PXTkApp::executeTclCommand(pxtk_buffer);
	
	sprintf(pxtk_buffer,"bind %s <ButtonRelease-%d> { pxtkcallback %ld %d %d 2 %s %d }",
		getWidgetName(),mouse,
		event_parent->getHandlerNo(),call_no,mouse,pxtk_buffer2,object_id);
	PXTkApp::executeTclCommand(pxtk_buffer);

	sprintf(pxtk_buffer,"bind %s <Shift-ButtonPress-%d> { pxtkcallback %ld %d %d 0 %s %d }",
		getWidgetName(),mouse,
		event_parent->getHandlerNo(),call_no,mouse+3,pxtk_buffer2,object_id);
	PXTkApp::executeTclCommand(pxtk_buffer);
	
	sprintf(pxtk_buffer,"bind %s <Shift-B%d-Motion> { pxtkcallback %ld %d %d 1 %s %d }",
		getWidgetName(),mouse,
		event_parent->getHandlerNo(),call_no,mouse+3,pxtk_buffer2,object_id);
	PXTkApp::executeTclCommand(pxtk_buffer);
	
	sprintf(pxtk_buffer,"bind %s <Shift-ButtonRelease-%d> { pxtkcallback %ld %d %d 2 %s %d }",
		getWidgetName(),mouse,
		event_parent->getHandlerNo(),call_no,mouse+3,pxtk_buffer2,object_id);
	PXTkApp::executeTclCommand(pxtk_buffer);

	sprintf(pxtk_buffer,"bind %s <Control-ButtonPress-%d> { pxtkcallback %ld %d %d 0 %s %d }",
		getWidgetName(),mouse,
		event_parent->getHandlerNo(),call_no,mouse+6,pxtk_buffer2,object_id);
	PXTkApp::executeTclCommand(pxtk_buffer);
	
	sprintf(pxtk_buffer,"bind %s <Control-B%d-Motion> { pxtkcallback %ld %d %d 1 %s %d }",
		getWidgetName(),mouse,
		event_parent->getHandlerNo(),call_no,mouse+6,pxtk_buffer2,object_id);
	PXTkApp::executeTclCommand(pxtk_buffer);
	
	sprintf(pxtk_buffer,"bind %s <Control-ButtonRelease-%d> { pxtkcallback %ld %d %d 2 %s %d }",
		getWidgetName(),mouse,
		event_parent->getHandlerNo(),call_no,mouse+6,pxtk_buffer2,object_id);
	PXTkApp::executeTclCommand(pxtk_buffer);

      }

  /*sprintf(pxtk_buffer,"bind %s <Map> { pxtkcallback %ld %d %d }",
	  getWidgetName(),
	  event_parent->getHandlerNo(),call_no+1,object_id);
  PXTkApp::executeTclCommand(pxtk_buffer);*/
  
  /*sprintf(pxtk_buffer,"bind %s <Configure> { pxtkcallback %ld %d %d }",
	  getWidgetName(),
	  event_parent->getHandlerNo(),call_no+1,object_id);
  PXTkApp::executeTclCommand(pxtk_buffer);*/
  
  sprintf(pxtk_buffer,"bind %s <Expose> { pxtkcallback %ld %d %d }",
	  getWidgetName(),
	  event_parent->getHandlerNo(),call_no+1,object_id);
  PXTkApp::executeTclCommand(pxtk_buffer);

  return 1;
}

/* -------------------------------------------------------------------------*/
/*       Definition of PXVTkTkRenderWidget Class                            */
/* -------------------------------------------------------------------------*/
PXVTkTkRenderWidget::PXVTkTkRenderWidget(PXTkEventObj* evpar,PXTkGadget* gadg_par,
					 int moption):PXVTkTkGadget(evpar,gadg_par,moption)
{
  initDisplay();
}
  
int PXVTkTkRenderWidget::initDisplay()
{
  sprintf(pxtk_buffer,"vtkTkRenderWidget %s -width 300 -height 300",widget_name);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);

  if (a!=TCL_OK)
      return TCL_ERROR;
  
  // GetRenderWindow
  sprintf(pxtk_buffer," %s GetRenderWindow",widget_name);
  PXTkApp::executeTclCommand(pxtk_buffer);
  
  strcpy(pxtk_buffer,PXTkApp::getTclStringResult());

  // Get Pointers to C++ classes
  int error=0;
  vtkRenderWindow *renWin = (vtkRenderWindow *)(vtkTclGetPointerFromObject(pxtk_buffer,"vtkRenderWindow",
									   PXTkApp::tcl_interpreter,error));  
  if (error==1)
    {
      is_display_ok=PXFalse;
      renderWindow=NULL;
      renderer=NULL;
      return TCL_ERROR;
    }
  is_display_ok=PXTrue;
  renderWindow=renWin;
  renderer=vtkRenderer::New();
  renderWindow->AddRenderer(renderer);
  return TCL_OK;
}

vtkRenderWindow* PXVTkTkRenderWidget::getRenderWindow()
{
  return renderWindow;
}

vtkRenderer* PXVTkTkRenderWidget::getRenderer()
{
  return renderer;
}


void PXVTkTkRenderWidget::Render()
{
  if (is_display_ok)
      renderWindow->Render();
}

void PXVTkTkRenderWidget::RenderDelay(int t)
{
  Tcl_CreateTimerHandler(t,PXVTkTkRenderWidget::handleRenderEvent,this);
}

void PXVTkTkRenderWidget::handleRenderEvent(void *arg1)
{
  ((PXVTkTkRenderWidget*)arg1)->Render();
}


/* -------------------------------------------------------------------------*/
/*           Definition of PXVTkTkImageViewerWidget                         */
/* -------------------------------------------------------------------------*/
/*PXVTkTkImageWindowWidget::PXVTkTkImageWindowWidget(PXTkEventObj* evpar,PXTkGadget* gadg_par,
						   int moption):PXVTkTkGadget(evpar,gadg_par,moption)
{
  initDisplay();
}
  
int PXVTkTkImageWindowWidget::initDisplay()
{
  sprintf(pxtk_buffer,"vtkImageWindow pxvtkren%d",object_id);
  PXTkApp::executeTclCommand(pxtk_buffer);

  sprintf(pxtk_buffer,"vtkTkImageWindowWidget %s -width 150 -height 150 -iw pxvtkren%d",widget_name,object_id);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  
  if (a!=TCL_OK)
      return TCL_ERROR;
  
  int error=0;
  sprintf(pxtk_buffer,"pxvtkren%d",object_id);
  // Get Pointers to C++ classes
  vtkImageWindow *ren = (vtkImageWindow *)(vtkTclGetPointerFromObject(pxtk_buffer,"vtkImageWindow",
								      PXTkApp::tcl_interpreter,error));  
  if (error==1)
      {
	is_display_ok=PXFalse;
	imageWindow=NULL;
#ifndef _WIN32
	fprintf(stderr,"Bad Image Viewer %s\n",widget_name);
#endif
	return TCL_ERROR;
      }
  is_display_ok=PXTrue;
  imageWindow=ren;
  return TCL_OK;
}

vtkImageWindow* PXVTkTkImageWindowWidget::getImageWindow()
{
  return imageWindow;
}
*/

/* -------------------------------------------------------------------------*/
/*           Definition of PXVTkTkImageViewerWidget                         */
/* -------------------------------------------------------------------------*/

/*PXVTkTkImageViewerWidget::PXVTkTkImageViewerWidget(PXTkEventObj* evpar,PXTkGadget* gadg_par,
						   int moption):PXVTkTkGadget(evpar,gadg_par,moption)
{
  initDisplay();
}
  
int PXVTkTkImageViewerWidget::initDisplay()
{
  sprintf(pxtk_buffer,"vtkImageViewer pxvtkren%d",object_id);
  PXTkApp::executeTclCommand(pxtk_buffer);

  sprintf(pxtk_buffer,"vtkTkImageViewerWidget %s -width 150 -height 150 -iv pxvtkren%d",widget_name,object_id);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  
  if (a!=TCL_OK)
      return TCL_ERROR;
  
  int error=0;
  sprintf(pxtk_buffer,"pxvtkren%d",object_id);
  // Get Pointers to C++ classes
  vtkImageViewer *ren = (vtkImageViewer *)(vtkTclGetPointerFromObject(pxtk_buffer,"vtkImageViewer",
								      PXTkApp::tcl_interpreter,error));  
  if (error==1)
      {
	is_display_ok=PXFalse;
	imageViewer=NULL;
#ifndef _WIN32
	fprintf(stderr,"Bad Image Viewer %s\n",widget_name);
#endif
	return TCL_ERROR;
      }
  is_display_ok=PXTrue;
  imageViewer=ren;
  return TCL_OK;
}

vtkImageViewer* PXVTkTkImageViewerWidget::getImageViewer()
{
  return imageViewer;
}

*/

