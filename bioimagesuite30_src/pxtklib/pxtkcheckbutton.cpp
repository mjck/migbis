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









// pxtkcheckbutton.cpp
#include "pxtkcheckbutton.h"

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu
   
   see pxtkcheckbutton.h for details

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

 /* ********************************************************************

    Definition of PXTkCheckButton Class 

 ********************************************************************* */

PXTkCheckButton::PXTkCheckButton(PXTkEventObj* evpar,PXTkGadget* gadg_par,const char* name,int callno,
				 PXBool dodisplay):PXTkButton(evpar,gadg_par,NULL,-1,PXFalse)
{
    if (dodisplay)
      {
	initDisplay();
	setText(name);
	setCallback(callno);
	call_no=callno;
      }
}

int PXTkCheckButton::initDisplay()
{
  setWidgetVariable("0");
  sprintf(pxtk_buffer,"checkbutton %s -variable widgetvar%d",widget_name,object_id);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  if (a==TCL_OK)
      is_display_ok=PXTrue;
  return a;
}
 
PXBool PXTkCheckButton::getState()
{
  char* line=getWidgetVariable();

  if (line==NULL)
    {
      fprintf(stderr,"NULL checkButton getState %s widgetvar%d\n",widget_name,object_id);
      return PXFalse;
    }

  int a=atoi(line);
  if (a==1)
      return PXTrue;
  return PXFalse;
}

int  PXTkCheckButton::setState(PXBool state,PXBool callback)
{
  if (state)
     setWidgetVariable("1");
  else
      setWidgetVariable("0");
  
  if (callback && call_no>0)
      return event_parent->handleEvent(call_no);
  else
      return TCL_OK;
}   



