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









// pxtkscrollbar.cpp
#include "pxtkscrollbar.h"
#include "pxtkeventobj.h"

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu
   
   see pxtkscrollbar.h for details

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */


/* ********************************************************************
   
   Definition of PXTkScrollbar Class 
   
   ********************************************************************* */

PXTkScrollbar::PXTkScrollbar(PXTkEventObj* evpar,PXTkGadget* gadg_par,PXBool vertical,
			     PXBool dodisplay):PXTkGadget(evpar,gadg_par)
{
  orientation=vertical;
  if (dodisplay)
      initDisplay();
}

int PXTkScrollbar::initDisplay()
{
  if (orientation)
      sprintf(pxtk_buffer,"scrollbar %s -orient vertical",widget_name);
  else
      sprintf(pxtk_buffer,"scrollbar %s -orient horizontal",widget_name);

  int a=PXTkApp::executeTclCommand(pxtk_buffer);

  if (a==TCL_OK)
      is_display_ok=PXTrue;
  return a;
}

int PXTkScrollbar::attachGadget(PXTkGadget* gadget)
{
  if (!is_display_ok)
      return TCL_ERROR;

  char* othername=gadget->getWidgetBaseName();
  char line[5];
  if (orientation)
      sprintf(line,"y");
  else
      sprintf(line,"x");
	
  sprintf(pxtk_buffer,"%s configure -command { %s %sview }",
	  widget_name,othername,line);
  PXTkApp::executeTclCommand(pxtk_buffer);

  sprintf(pxtk_buffer,"%s configure -%sscrollcommand {%s set }",
	  othername,line,widget_name);

  return PXTkApp::executeTclCommand(pxtk_buffer);
}


