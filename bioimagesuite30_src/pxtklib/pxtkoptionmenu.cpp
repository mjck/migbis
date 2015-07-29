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





#include "pxtkoptionmenu.h"
#include "pxtkeventobj.h"
#include <stdlib.h>


/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu

   see pxtkoptionmenu.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

/* ********************************************************************
   
   Definition of PXTkOptionMenu Class 
   
   ********************************************************************* */

PXTkOptionMenu::PXTkOptionMenu(PXTkEventObj* evpar,PXTkGadget* gadg_par,
	       const char* label,int callno):PXTkGadget(evpar,gadg_par)
{
  initial_value=new char[strlen(label)+1];
  callbackno=callno;
  strcpy(initial_value,label);
  initDisplay();
}

PXTkOptionMenu::~PXTkOptionMenu()
{
  // This should never be needed!
  if (initial_value!=NULL)
      delete [] initial_value;
}

char* PXTkOptionMenu::getLabel(int index)
{
  sprintf(pxtk_buffer,"%s.menu entrycget %d -label",widget_name,index);
  if (PXTkApp::executeTclCommand(pxtk_buffer)==TCL_OK)
      return PXTkApp::getTclStringResult();
  else
      return NULL;
}

int  PXTkOptionMenu::changeLabel(int index, const char* label)
{
  index=Irange(index,0,getNumOptions()-1);
  /*sprintf(pxtk_buffer,"%s.menu entryconfigure %d -label \"%s\" -variable widgetvar%d",
	  widget_name,index,label,object_id);
  fprintf(stderr,"Command = %s\n",pxtk_buffer);
  return PXTkApp::executeTclCommand(pxtk_buffer);*/

  int oldIndex=getIndex();
  deleteOption(index);
  insertOption(label,index);
  setIndex(oldIndex);
  return TCL_OK;
}

int   PXTkOptionMenu::addOption(const char* label,int callno)
{
  if (callno==-1)
      callno=callbackno;

  sprintf(pxtk_buffer,"%s.menu add radio -label \"%s\" -variable widgetvar%d -command {pxtkcallback %ld %d %ld}",
	  widget_name,label,object_id,
	  event_parent->getHandlerNo(),callno,(long)this);
  return PXTkApp::executeTclCommand(pxtk_buffer);
	  
}

int   PXTkOptionMenu::insertOption(const char* label,int index,int callno)
{
  if (callno==-1)
      callno=callbackno;

  if (index<0)
      index=getNumOptions();

  sprintf(pxtk_buffer,"%s.menu insert %d radio -label \"%s\" -variable widgetvar%d -command {pxtkcallback %ld %d %ld}",
	  widget_name,index,label,object_id,
	  event_parent->getHandlerNo(),callno,(long)this);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int   PXTkOptionMenu::deleteOption(int index)
{
  PXBool cleanup=PXFalse;
  if (index==getIndex())
      cleanup=PXTrue;

  sprintf(pxtk_buffer,"%s.menu delete %d ",widget_name,index);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);

  if (cleanup)
      setIndex(0);
  return a;
}
  
int   PXTkOptionMenu::getNumOptions()
{
  sprintf(pxtk_buffer,"%s.menu index last",widget_name);
  if (PXTkApp::executeTclCommand(pxtk_buffer)==TCL_OK)
      {
	int a=PXTkApp::getTclIntResult();
	return (a+1);
      }
  else
      return 0;
  
}
void  PXTkOptionMenu::setIndex(int index,PXBool callback)
{
  if (callback)
      {
	sprintf(pxtk_buffer,"%s.menu invoke %d",widget_name,index);
	PXTkApp::executeTclCommand(pxtk_buffer);
      }
  else
      {
	sprintf(pxtk_buffer,"%s.menu entrycget %d -label",widget_name,index);
	int a=PXTkApp::executeTclCommand(pxtk_buffer);
	if (a==TCL_OK)
	    {
	      strcpy(pxtk_buffer2,PXTkApp::getTclStringResult());
	      setWidgetVariable(pxtk_buffer2);
	    }
      }
}

int   PXTkOptionMenu::getIndex()
{
  sprintf(pxtk_buffer2,getValue());
  sprintf(pxtk_buffer,"%s.menu index \"%s\"",widget_name,pxtk_buffer2);

  if (PXTkApp::executeTclCommand(pxtk_buffer)==TCL_OK)
      return PXTkApp::getTclIntResult();
  else
      return -1;
}

char* PXTkOptionMenu::getValue()
{
  return getWidgetVariable();
}
  
int PXTkOptionMenu::initDisplay()
{
  sprintf(pxtk_buffer,"tk_optionMenu %s widgetvar%d \"%s\" ",widget_name,object_id,initial_value);
  delete [] initial_value;
  initial_value=NULL;
  
  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  if (a==TCL_OK)
      {
	is_display_ok=PXTrue;
	sprintf(pxtk_buffer,"%s.menu entryconfigure 0 -command {pxtkcallback %ld %d %ld}",
		widget_name,
		event_parent->getHandlerNo(),callbackno,(long)this);
	return PXTkApp::executeTclCommand(pxtk_buffer);
      }
  return a;
}


