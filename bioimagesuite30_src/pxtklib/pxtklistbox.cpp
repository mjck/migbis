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





// pxtklistbox.cpp
#include "pxtklistbox.h"
#include "pxtkeventobj.h"

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu
   
   see pxtklistbox.h for details

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

/* ********************************************************************
   
   Definition of PXTkListBox Class 
   
   ********************************************************************* */
PXTkListBox::PXTkListBox(PXTkEventObj* evpar,PXTkGadget* gadg_par,int moption,
		   PXBool dodisplay):PXTkGadget(evpar,gadg_par)
{
  if (dodisplay)
      {
	call_no=moption;
	initDisplay();
      }
}
  
int PXTkListBox::addItem(const char* item,int index)
{
  if (index==-1)
      sprintf(pxtk_buffer,"%s insert end \"%s\" ",widget_name,item);
  else
      sprintf(pxtk_buffer,"%s insert %d \"%s\" ",widget_name,index,item);

  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int PXTkListBox::clearList()
{
  sprintf(pxtk_buffer,"%s delete 0 end ",widget_name);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int PXTkListBox::clearItem(int index)
{
  sprintf(pxtk_buffer,"%s delete %d",widget_name,index);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int PXTkListBox::selectItem(int index)
{
  int a=getCurrentItem();

  if (a!=-1)
      {
	sprintf(pxtk_buffer,"%s selection clear %d %d",widget_name,a,a);
	PXTkApp::executeTclCommand(pxtk_buffer);
      }
  if (index!=-1)
      {
	//sprintf(pxtk_buffer,"%s selection set %d %d",widget_name,a,index);
	sprintf(pxtk_buffer,"%s selection set %d %d",widget_name,index,index);
	PXTkApp::executeTclCommand(pxtk_buffer);
	//sprintf(pxtk_buffer,"%s selection set %d %d",widget_name,index,index);
	//sprintf(pxtk_buffer,"%s see %d",widget_name,index);
	//PXTkApp::executeTclCommand(pxtk_buffer);
      }

  return TCL_OK;
}

int PXTkListBox::getNumItems()
{
  sprintf(pxtk_buffer,"%s index end",widget_name);
  if (PXTkApp::executeTclCommand(pxtk_buffer)==TCL_OK)
      return PXTkApp::getTclIntResult();
  else
      return 0;
}

int PXTkListBox::getCurrentItem()
{
  sprintf(pxtk_buffer,"%s curselection",widget_name);
  if (PXTkApp::executeTclCommand(pxtk_buffer)==TCL_OK)
      {
	char* line=PXTkApp::getTclStringResult();
	if (strlen(line)==0)
	    return -1;
	else
	    return PXTkApp::getTclIntResult();
      }
  else
      return -1;
}

char* PXTkListBox::getItem(int index)
{
  sprintf(pxtk_buffer,"%s get %d %d",widget_name,index,index);
  if (PXTkApp::executeTclCommand(pxtk_buffer)==TCL_OK)
      return PXTkApp::getTclStringResult();
  else
      return 0;
}

int PXTkListBox::initDisplay()
{
  sprintf(pxtk_buffer,"listbox %s -exportselection 0",widget_name);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  configure("-selectMode","single");
  //configure("-exportselection","0");
  configure("-width",15);

  if (call_no>0)
      {
        sprintf(pxtk_buffer,"bind %s <ButtonRelease-1> { pxtkcallback %ld %d %ld }",
                widget_name,event_parent->getHandlerNo(),call_no,(long)this);
        a=PXTkApp::executeTclCommand(pxtk_buffer);
      }

  if (a==TCL_OK)
      is_display_ok=PXTrue;

  return a;

}


