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





// pxtkgadgets.cc
#include "pxtkgadgets.h"
#include "pxtkeventobj.h"
#include "pxtkdialog.h"
#include <stdlib.h>
#include <math.h>
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu
   
   see pxtk.h for details

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

 /* ********************************************************************

    Definition of PXTkGadget Class 

 ********************************************************************* */

char PXTkGadget::pxtk_buffer[4000];
char PXTkGadget::pxtk_buffer2[4000];


// Do not use these variables; they are used for internal implementation
// ---------------------------------------------------------------------
char   PXTkGadget::pxtk_intbuffer[4000];

PXTkGadget::PXTkGadget(PXTkEventObj* evpar,PXTkGadget* gadg_par):PXTkBase(evpar,gadg_par)
{
  if (!has_gadget_parent)
      {
	widget_name=new char[10];
	sprintf(widget_name,".w%d",object_id);
      }
  else
      {
	widget_name=new char[strlen(gadget_parent->getWidgetName())+10];
	if (strcmp(gadget_parent->getWidgetName(),".")==0)
	    sprintf(widget_name,".w%d",object_id);
	else
	    sprintf(widget_name,"%s.w%d",gadget_parent->getWidgetName(),object_id);
      }

  widget_base_name=new char[strlen(widget_name)+1];
  strcpy(widget_base_name,widget_name);
}

PXTkGadget::PXTkGadget(PXTkEventObj* evpar, const char* name):PXTkBase(evpar)
{
  widget_name=new char[strlen(name)+1];
  strcpy(widget_name,name);
  widget_base_name=new char[strlen(name)+1];
  strcpy(widget_base_name,name);
  is_display_ok=PXTrue;
}

PXTkGadget::~PXTkGadget()
{
  delete [] widget_name;
  delete [] widget_base_name;
}

int PXTkGadget::wmconfigure(const char* option,const char* value,PXBool noquote)
{
  if (!is_display_ok)
      return TCL_ERROR;
  
  if (!noquote)
      sprintf(pxtk_intbuffer,"wm %s %s \"%s\"",option,widget_name,value);
  else
      sprintf(pxtk_intbuffer,"wm %s %s %s",option,widget_name,value);

  if (hasEventParent())
      return PXTkApp::executeTclCommand(pxtk_intbuffer);
  else
      return TCL_ERROR;

}

int PXTkGadget::configure(const char* option,const char* value)
{
  if (!is_display_ok)
      return TCL_ERROR;

  sprintf(pxtk_intbuffer,"%s configure %s %s",widget_name,option,value);
  if (hasEventParent())
      return PXTkApp::executeTclCommand(pxtk_intbuffer);
  else
      return TCL_ERROR;
}


int PXTkGadget::configurequote(const char* option,const char* value)
{
  if (!is_display_ok)
      return TCL_ERROR;

  sprintf(pxtk_intbuffer,"%s configure %s \"%s\"",widget_name,option,value);

  if (hasEventParent())
      return PXTkApp::executeTclCommand(pxtk_intbuffer);
  else
      return TCL_ERROR;
}

int PXTkGadget::configure(const char* option,int value)
{
  if (!is_display_ok)
      return TCL_ERROR;

  sprintf(pxtk_intbuffer,"%s configure %s %d",widget_name,option,value);
  if (hasEventParent())
      return PXTkApp::executeTclCommand(pxtk_intbuffer);
  else
      return TCL_ERROR;
}

int PXTkGadget::configure(const char* option,float value)
{
  if (!is_display_ok)
      return TCL_ERROR;

  sprintf(pxtk_intbuffer,"%s configure %s %f",widget_name,option,value);
  if (hasEventParent())
      return PXTkApp::executeTclCommand(pxtk_intbuffer);
  else
      return TCL_ERROR;
}



char* PXTkGadget::cget(const char* option)
{
  if (!is_display_ok)
      return NULL;

  sprintf(pxtk_intbuffer,"%s cget %s ",widget_name,option);
  if (PXTkApp::executeTclCommand(pxtk_intbuffer)==TCL_OK)
      return PXTkApp::getTclStringResult();
  else
      return NULL;

}

char* PXTkGadget::setWidgetVariable(const char* value)
{
  sprintf(pxtk_intbuffer,"widgetvar%d",object_id);
  char *line=PXTkApp::setTclVariable(pxtk_intbuffer,value);
  if (PXTkApp::debug_tcl)
      {
	sprintf(pxtk_intbuffer,"%s=%s (orig=%s)\n",pxtk_intbuffer,line,value);
	PXTkApp::printToConsole(pxtk_intbuffer);
      }
  return line;
}


char* PXTkGadget::getWidgetVariable()
{
  sprintf(pxtk_intbuffer,"widgetvar%d",object_id);
  char* line=PXTkApp::getTclVariable(pxtk_intbuffer);
  if (PXTkApp::debug_tcl)
    {
      sprintf(pxtk_intbuffer,"%s=%s\n",pxtk_intbuffer,line);
      PXTkApp::printToConsole(pxtk_intbuffer);
    }
  return line;
}


int PXTkGadget::deleteWidgetVariable()
{
  sprintf(pxtk_intbuffer,"widgetvar%d",object_id);
  return PXTkApp::deleteTclVariable(pxtk_intbuffer);
}

char* PXTkGadget::getWidgetBaseName()
{
  return widget_base_name;
}


char* PXTkGadget::getWidgetName()
{
  return widget_name;
}

PXTkClassId PXTkGadget::getClassId()
{
  return PXTkGadgetId;
}

int PXTkGadget::bindCallback(const char* event,int callno)
{
  sprintf(pxtk_buffer,"bind %s %s {pxtkcallback %ld %d}",widget_name,event,
	  event_parent->getHandlerNo(),callno);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int  PXTkGadget::enable(PXBool enab)
{
  if (enab)
      return configure("-state","normal");
  else
      return configure("-state","disabled");
}



