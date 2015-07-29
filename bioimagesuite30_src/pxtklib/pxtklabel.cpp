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
#include "pxtklabel.h"
#include <stdlib.h>
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu
   
   see pxtklabel.h for details

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */


 /* ********************************************************************

    Definition of PXTkLabel Class 

 ********************************************************************* */

PXTkLabel::PXTkLabel(PXTkEventObj* evpar,PXTkGadget* gadg_par, 
           const char* label,PXBool dodisplay):PXTkGadget(evpar,gadg_par)
{
  if (dodisplay)
      {
	initDisplay();
	setText(label);
      }

}

int PXTkLabel::setText(const char* label)
{
  return configurequote("-text",label);
}

int PXTkLabel::initDisplay()
{
  sprintf(pxtk_buffer,"label %s",widget_name);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  if (a==TCL_OK)
      is_display_ok=PXTrue;
  return a;
}

int PXTkLabel::setWidthHeight(int w,int h)
{
  int a=TCL_OK;
  if (w>0)
      a=configure("-width",w);
  if (h>0 && a==TCL_OK)
      a=configure("-height",w);
  return a;
}

int PXTkLabel::setPadding(int bd,int padx,int pady)
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

char *PXTkLabel::getText()
{
	  return cget("-text");
}

