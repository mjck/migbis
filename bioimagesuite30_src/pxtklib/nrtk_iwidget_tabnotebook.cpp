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





#include "nrtk_iwidget_tabnotebook.h"
#include <stdlib.h>
#include <ctype.h>


/******************************************************************************/

NRTkIWTabNotebook::NRTkIWTabNotebook(PXTkEventObj* evpar,PXTkGadget* par,
                   const char* tab_pos, int width,int height):PXTkEventObj(evpar,par)
{
  num_items=0;
  
  initDisplay(tab_pos);

  if (width>0)
      rnotebook->configure("-width",width);
  if (height>0)
      rnotebook->configure("-height",height);
}
 
/******************************************************************************/

int NRTkIWTabNotebook::initDisplay(const char *tab_pos)
{

   PXTkFrame* main = new PXTkFrame(this, gadget_parent);
   main_window = main;

   main->configure("-padx",8);
   main->configure("-pady",8);
   main->configure("-bg","lightgray");
   main->configure("-relief","ridge");

   sprintf(pxtk_buffer2,"%s.rn%d",main_window->getWidgetName(),object_id);

   rnotebook = new PXTkGadget(this, pxtk_buffer2);

   char tabpos = tolower((unsigned char) tab_pos[0]);
   if(tabpos != 'n' && tabpos != 's' && tabpos != 'e' && tabpos != 'w') tabpos = 'n';
   
   sprintf(pxtk_buffer,"iwidgets::tabnotebook %s  -tabpos %c",rnotebook->getWidgetName(), tabpos);
   PXTkApp::executeTclCommand(pxtk_buffer);

/**
   sprintf(pxtk_buffer,"iwidgets::tabnotebook %s  -tabpos %c",rnotebook->getWidgetName(), tabpos);
   printf("iwidgets::tabnotebook %s  -tabpos %c",rnotebook->getWidgetName(), tabpos);
   PXTkApp::executeTclCommand(pxtk_buffer);
**/
   ((PXTkFrame*)main_window)->addChildren(" -expand true -fill both -padx 5 -pady 5",rnotebook);
/**
   num_items=1;
   sprintf(pxtk_buffer,"%s add -label %s", rnotebook->getWidgetName(), "Base");

   PXTkApp::executeTclCommand(pxtk_buffer);

   char* line = PXTkApp::getTclStringResult();
   sub_manager[num_items-1] = new PXTkFrame(this,line);
   **/
   return TCL_OK;
}
 
/******************************************************************************/

PXTkFrame* NRTkIWTabNotebook::getPage(int i)
{
  i=Irange(i,0,num_items-1);
  return sub_manager[i];
}
 
/******************************************************************************/

int NRTkIWTabNotebook::viewPage(const char *label)
{
   sprintf(pxtk_buffer,"%s view %s", rnotebook->getWidgetName(), label);

   PXTkApp::executeTclCommand(pxtk_buffer);

  return TCL_OK;
}
 
/******************************************************************************/

PXTkFrame* NRTkIWTabNotebook::addPage(const char* label)
{
  num_items++;
  // Create Frame and Tab
  sprintf(pxtk_buffer, "%s add -label %s", rnotebook->getWidgetName(), label);
  PXTkApp::executeTclCommand(pxtk_buffer);

  char* tabFrame = PXTkApp::getTclStringResult();

  sub_manager[num_items-1] = new PXTkFrame(this, tabFrame);

  return sub_manager[num_items-1];
}
 
/******************************************************************************/

int NRTkIWTabNotebook::getNumItems()
{
  return num_items;
}
 
/******************************************************************************/

