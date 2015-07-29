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





#include "pxtkdialog.h"

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris Mat 1995   papad@noodle.med.yale.edu

   see pxtkdialog.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

/* -------------------------------------------------------------------------*/
/*                   PXTkDialog                                          */
/* -------------------------------------------------------------------------*/

PXTkDialog::PXTkDialog(PXTkGadget* par, const char* title,PXBool insideparent):PXTkEventObj(NULL,par)
{
  init(title,insideparent);
}

/* -------------------------------------------------------------------------*/
PXTkDialog::PXTkDialog(PXTkEventObj* objpar, const char* title,PXBool insideparent):PXTkEventObj(objpar,objpar->getMainWindow())
{
  init(title,insideparent);
}
/* -------------------------------------------------------------------------*/
PXTkDialog::PXTkDialog(PXTkEventObj* objpar,PXTkGadget* w, const char* title,PXBool insideparent):PXTkEventObj(objpar,w)
{
  init(title,insideparent);
}
/* -------------------------------------------------------------------------*/
PXTkDialog::~PXTkDialog()
{
  if (is_display_ok)
      {
	if (!create_inside_parent)
	    popdownDialog();
	sprintf(pxtk_buffer,"tkwait destroy %s",main_window->getWidgetName());
	PXTkApp::executeTclCommand(pxtk_buffer);
      }
  delete [] dlg_title;
}
/* -------------------------------------------------------------------------*/
void PXTkDialog::init(const char* title,PXBool inside)
{
  is_display_ok=PXFalse;
   dlg_title=NULL;
   setTitle(title);
   height=300;
   width=400;
   create_inside_parent=inside;

 }
 /* -------------------------------------------------------------------------*/
int PXTkDialog::initDisplay()
{
  width=Irange(width,64,1024);
  height=Irange(height,64,1024);

  if (create_inside_parent)
      {
	PXTkFrame *frame=new PXTkFrame(this,gadget_parent,width,height);
	main_window=frame;
	packMainWindow();
       }
  else
      {
	PXTkToplevel *dlg=new PXTkToplevel(this,gadget_parent,dlg_title,width,height);
	main_window=dlg;
	dlg->hide();
	modality=PXFalse;
      }
  is_display_ok=PXTrue;
  
  return TCL_OK;
}

/* -------------------------------------------------------------------------*/
void PXTkDialog::setModality(PXBool modal)
{
  if (create_inside_parent)
      return;
  
  modality=modal;
  if (isOpen() && is_display_ok)
      ((PXTkToplevel*)main_window)->setGrab(modal);
}
/* -------------------------------------------------------------------------*/
void PXTkDialog::setSize(int w,int h)
{
  width=Irange(w,64,1024); height=Irange(h,64,1024);
  if (isOpen())
      ((PXTkFrame*)main_window)->setDimensions(w,h);
}
/* -------------------------------------------------------------------------*/
void PXTkDialog::popupDialog()
{
  if (create_inside_parent)
      return;
  
  if (is_display_ok==PXFalse)
      initDisplay();
  
  ((PXTkToplevel*)main_window)->show(1);
  
  if (modality)
      ((PXTkToplevel*)main_window)->setGrab(PXTrue);
}
/* -------------------------------------------------------------------------*/
void PXTkDialog::popdownDialog()
{
  //	printf("--->In PXTkDialog::popdownDialog... %d %d\n", create_inside_parent, modality);
  // Makes no sense so ignore 
  if (create_inside_parent)
      return;

  if (isOpen())
      {
	if (modality)
	    ((PXTkToplevel*)main_window)->setGrab(PXFalse);
	((PXTkToplevel*)main_window)->hide();
      }
}
/* -------------------------------------------------------------------------*/
void PXTkDialog::setTitle(const char* title)
{
  if (dlg_title!=NULL)
      delete [] dlg_title;

  dlg_title=new char[strlen(title)+2];
  strcpy(dlg_title,title);
  
  if (is_display_ok)
      {
	if (create_inside_parent && !has_event_parent)
	    {
	      sprintf(pxtk_buffer,"wm title . \"%s\"",dlg_title);
	      PXTkApp::executeTclCommand(pxtk_buffer);
	    }
	else
	    {
	      ((PXTkToplevel*)main_window)->setTitle(dlg_title);
	    }
      }
}
/* -------------------------------------------------------------------------*/
void PXTkDialog::exitCallback(const char* line)
{
#ifndef _WIN32
  exit(0);
#else
//  PXTkApp::executeTclCommand("destroy .");
  PXTkApp::executeTclCommand("exit");
#endif
}
/* -------------------------------------------------------------------------*/
void PXTkDialog::aboutCallback(const char* line)
{
  if (line==NULL)
      messageBox("This Application \n(c) X.Papademetris 1995-2000",
		 "About this application");
  else
      messageBox(line,
		 "About this application");
}
/* -------------------------------------------------------------------------*/
PXTkClassId PXTkDialog::getClassId()
{
  return PXTkDialogId;
}

PXBool PXTkDialog::isInside()
{
  return create_inside_parent;
}

PXBool PXTkDialog::isOpen()
{
  if (!is_display_ok)
      return PXFalse;

  if (create_inside_parent)
    return PXTrue;

  sprintf(pxtk_buffer,"wm state %s",main_window->getWidgetName());
  PXTkApp::executeTclCommand(pxtk_buffer);
  char* a=PXTkApp::getTclStringResult();
  if (a!=NULL)
    {
      if (strcmp(a,"normal")==0)
	return PXTrue;
    }
  
  return PXFalse;
}
/* -------------------------------------------------------------------------*/
int PXTkDialog::handleEvent(int eventno)
{
  switch(eventno)
      {
      case 0:
	aboutCallback();
	break;
      case 1:
	{
	  int ok=questionBox("Do you really want to exit?");
	  if (ok)
	      exitCallback();
	}
	break;
      case 2:
	PXTkApp::popupConsole();
	break;
      case 3:
	popdownDialog();
	break;
      case 4:
	PXTkApp::autoArrangeWindows();
	break;
      }
  return TCL_OK;
}

/* -------------------------------------------------------------------------*/

