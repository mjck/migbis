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
#include "pxtkframe.h"
#include "pxtkeventobj.h"
#include "pxtkdialog.h"
#include <stdlib.h>
#include <math.h>
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu
   
   see pxtkframe.h for details

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

 /* ********************************************************************

    Definition of PXTkFrame Class 

 ********************************************************************* */

PXTkFrame::PXTkFrame(PXTkEventObj* evpar,PXTkGadget* gadg_par,int width,
		     int height,PXBool dodisplay):PXTkGadget(evpar,gadg_par)
{
  //children=new PXTkBaseList;
  pack_mode=PXTrue;
  if (dodisplay)
      {
	initDisplay();
	setDimensions(width,height);
      }
}

PXTkFrame::PXTkFrame(PXTkEventObj* evpar,const char* name):PXTkGadget(evpar,name)
{
  pack_mode=PXTrue;
  is_display_ok=PXTrue;
}

PXTkFrame::~PXTkFrame()
{
  //  delete children;
}

PXTkClassId PXTkFrame::getClassId()
{
  return PXTkFrameId;
}


int PXTkFrame::initDisplay()
{
  sprintf(pxtk_buffer,"frame %s",widget_name);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);

  if (a==TCL_OK)
      is_display_ok=PXTrue;
  return a;
}

void PXTkFrame::setPackMode(PXBool pack)
{
  pack_mode=pack;
}
 
PXBool PXTkFrame::getPackMode()
{
  return pack_mode;
}

// Unmap Child

int PXTkFrame::unmapChild(PXTkGadget* child)
{
  if (!is_display_ok)
      return TCL_ERROR;

  if (pack_mode)
      sprintf(pxtk_buffer,"pack forget %s",child->getWidgetBaseName());
  else
      sprintf(pxtk_buffer,"grid forget %s",child->getWidgetBaseName());

  return PXTkApp::executeTclCommand(pxtk_buffer);
  
}

int PXTkFrame::mapChild(PXTkGadget* child,const char* options)
{
  if (!is_display_ok)
      return TCL_ERROR;

  if (pack_mode)
      sprintf(pxtk_buffer,"pack %s %s",child->getWidgetBaseName(),options);
  else
      sprintf(pxtk_buffer,"grid %s %s",child->getWidgetBaseName(),options);

  
  return PXTkApp::executeTclCommand(pxtk_buffer);

}


  // UsefulOptions
int PXTkFrame::setDimensions(int width,int height)
{
  int a=TCL_OK,b=TCL_OK;
  if (width!=-1) 
      a=configure("-width",width);
  if (height!=-1) 
      b=configure("-height",width);

  if (a==TCL_OK && b==TCL_OK)
      return TCL_OK;
  else return TCL_ERROR;
}

int PXTkFrame::childConfigure(PXTkGadget* child,const char* option,const char* value)
{
  if (!is_display_ok)
      return TCL_ERROR;

  if (pack_mode)
      sprintf(pxtk_buffer,"pack configure %s %s %s",child->getWidgetBaseName(),option,value);
  else
      sprintf(pxtk_buffer,"grid configure %s %s %s",child->getWidgetBaseName(),option,value);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}


int PXTkFrame::gridRowColumnChild(PXTkGadget* child,int row,int column,int rowspan,int columnspan)
{
  if (!is_display_ok)
      return TCL_ERROR;

  sprintf(pxtk_buffer,"grid %s -column %d -row %d -columnspan %d -rowspan %d",child->getWidgetBaseName(),
	  column,row,columnspan,rowspan);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int PXTkFrame::gridRowColumnConfigure(PXTkGadget* child,int row,int column,int rowspan,int columnspan)
{
  if (!is_display_ok)
      return TCL_ERROR;
  
  sprintf(pxtk_buffer,"grid configure %s -column %d -row %d -columnspan %d -rowspan %d",child->getWidgetBaseName(),
	  column,row,columnspan,rowspan);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}


int PXTkFrame::gridExpandRowConfigure(int row,int weight,int minsize,int pad)
{
  if (!is_display_ok)
      return TCL_ERROR;
  
  sprintf(pxtk_buffer,"grid rowconfigure %s %d -weight %d",widget_name,row,weight);
  if (minsize!=-1)
    sprintf(pxtk_buffer,"%s -minsize %d",pxtk_buffer,minsize);
  if (pad!=-1)
        sprintf(pxtk_buffer,"%s -pad %d",pxtk_buffer,pad);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int PXTkFrame::gridExpandColumnConfigure(int row,int weight,int minsize,int pad)
{
  if (!is_display_ok)
      return TCL_ERROR;
  
  sprintf(pxtk_buffer,"grid columnconfigure %s %d -weight %d",widget_name,row,weight);
  if (minsize!=-1)
    sprintf(pxtk_buffer,"%s -minsize %d",pxtk_buffer,minsize);
  if (pad!=-1)
        sprintf(pxtk_buffer,"%s -pad %d",pxtk_buffer,pad);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int PXTkFrame::addChildren(const char* options,PXTkGadget* child1,
			    PXTkGadget* child2,PXTkGadget* child3,PXTkGadget* child4,PXTkGadget* child5)
{
  if (!is_display_ok)
      return TCL_ERROR;

  if (pack_mode)
      sprintf(pxtk_buffer,"pack");
  else
      sprintf(pxtk_buffer,"grid");


  sprintf(pxtk_buffer,"%s %s",pxtk_buffer,child1->getWidgetBaseName());
  // children->add(child1);

  if (child2!=NULL)
      {
	sprintf(pxtk_buffer,"%s %s",pxtk_buffer,child2->getWidgetBaseName());
	// children->add(child2);
      }
  if (child3!=NULL)
      {
	sprintf(pxtk_buffer,"%s %s",pxtk_buffer,child3->getWidgetBaseName());
	// children->add(child3);
      }
  if (child4!=NULL)
      {
	sprintf(pxtk_buffer,"%s %s",pxtk_buffer,child4->getWidgetBaseName());  
	// children->add(child4);
      }

  if (child5!=NULL)
      {
	sprintf(pxtk_buffer,"%s %s",pxtk_buffer,child5->getWidgetBaseName());
	// children->add(child5);
      }
  
  sprintf(pxtk_buffer,"%s %s",pxtk_buffer,options);

  return PXTkApp::executeTclCommand(pxtk_buffer);

}


