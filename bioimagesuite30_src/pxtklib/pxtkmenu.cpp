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









#include "pxtkmenu.h"
#include "pxtkeventobj.h"
#include <stdlib.h>


/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu

   see pxtkmenu.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */


/* ********************************************************************
   
   Definition of PXTkMenu Class 
   
   ********************************************************************* */

PXTkMenu::PXTkMenu(PXTkEventObj* evpar,PXTkMenuButton* gadg_par,PXBool tearoff,
		   PXBool cascade):PXTkGadget(evpar,gadg_par)
{
  tear_off=tearoff;

  if (cascade)
      {
	tear_off=PXFalse;
      }
  else // Not a cascade menu i.e. parent is a menubutton directly 
      {
	delete widget_name;
	delete widget_base_name;
	widget_name=new char[strlen(gadg_par->getWidgetName())+10];
	widget_base_name=new char[strlen(gadg_par->getWidgetName())+10];
	sprintf(widget_name,"%s.menu",gadg_par->getWidgetName());
	sprintf(widget_base_name,"%s.menu",gadg_par->getWidgetName());
      }

  initDisplay();
}

int PXTkMenu::addSeparator()
{
  sprintf(pxtk_buffer,"%s add separator",widget_name);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int PXTkMenu::addCascade(const char* label,PXTkMenu* cascade)
{
  sprintf(pxtk_buffer,"%s add cascade -label \"%s\" -menu %s",widget_name,label,
	  cascade->getWidgetName());
  return PXTkApp::executeTclCommand(pxtk_buffer);
}
  
  
int  PXTkMenu::addButton(const char* label,int callno)
{
  sprintf(pxtk_buffer,"%s add command -label \"%s\" -command {pxtkcallback %ld %d %ld}",widget_name,label,
	  event_parent->getHandlerNo(),callno,(long)this);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int  PXTkMenu::addCheckBox(const char* label,int variable,PXBool state,int callno)
{
  sprintf(pxtk_buffer,"%s add check -label \"%s\" -variable widgetvar%d_%d -command {pxtkcallback %ld %d %ld}",
	  widget_name,label,
	  object_id,variable,
	  event_parent->getHandlerNo(),callno,(long)this);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  if (state)
      setMenuWidgetVariable(variable,1);
  return a;
} 

int PXTkMenu::addRadioButton(const char* label,int variable,int value,PXBool state,int callno)
{
  sprintf(pxtk_buffer,"%s add radio -label \"%s\" -variable widgetvar%d_%d -value %d -command {pxtkcallback %ld %d %ld}",
	  widget_name,label,
	  object_id,variable,value,
	  event_parent->getHandlerNo(),callno,(long)this);
  //  fprintf(stderr,"%s\n",pxtk_buffer);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  
  if (state)
      setMenuWidgetVariable(variable,value);
  return a;
}

int PXTkMenu::setMenuWidgetVariable(int var,int val)
{
  sprintf(pxtk_buffer,"widgetvar%d_%d",object_id,var);
  char value[50];
  sprintf(value,"%d",val);
  //  fprintf(stderr,"**** Setting %s -> %s\n",pxtk_buffer,value);
  char *line=PXTkApp::setTclVariable(pxtk_buffer,value);
  if (PXTkApp::debug_tcl)
    {
      sprintf(pxtk_buffer,"%s=%s (orig=%s)\n",pxtk_buffer,line,value);
      PXTkApp::printToConsole(pxtk_buffer);
    }
  return atoi(line);
}

int PXTkMenu::getMenuWidgetVariable(int var)
{
  sprintf(pxtk_buffer,"widgetvar%d_%d",object_id,var);
  //  fprintf(stderr,"getting %s\n",pxtk_buffer);
  char* line=PXTkApp::getTclVariable(pxtk_buffer);
  if (PXTkApp::debug_tcl)
    {
      sprintf(pxtk_buffer,"%s=%s\n",pxtk_buffer,line);
      PXTkApp::printToConsole(pxtk_buffer);
    }
  return atoi(line);
}

int PXTkMenu::initDisplay()
{
  Tk_MapWindow(PXTkApp::getWidgetPointer(gadget_parent->getWidgetName()));
  
  if (tear_off)
      sprintf(pxtk_buffer,"menu %s -tearoff 1",widget_name);
  else
      sprintf(pxtk_buffer,"menu %s -tearoff 0",widget_name);

  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  if (a==TCL_OK)
      is_display_ok=PXTrue;
  return a;
}


