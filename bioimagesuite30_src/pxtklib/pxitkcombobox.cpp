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









// pxitkcombobox.cpp
#include "pxitkcombobox.h"
#include "pxtkeventobj.h"

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu
   
   see pxitkcombobox.h for details

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

/* ********************************************************************
   
   Definition of PXITkComboBox Class 
   
   ********************************************************************* */
PXITkComboBox::PXITkComboBox(PXTkEventObj* evpar,PXTkGadget* gadg_par,const char* label,int moption,
		   PXBool dodisplay):PXTkListBox(evpar,gadg_par,moption,PXFalse)
{
  int n=strlen(label);
  labelname=new char[n+1]; 
  sprintf(labelname,"%s",label);
  
  if (dodisplay)
    {
      call_no=moption;
      initDisplay();
    }
}
  
int PXITkComboBox::addItem(const char* item,int index)
{
  if (index==-1)
      sprintf(pxtk_buffer,"%s insert list end \"%s\" ",widget_name,item);
  else
      sprintf(pxtk_buffer,"%s insert list %d \"%s\" ",widget_name,index,item);

  int ok=PXTkApp::executeTclCommand(pxtk_buffer);

  if (this->getNumItems()==1)
    return this->selectItem(0);
  
  return ok;
}

int PXITkComboBox::clearList()
{
  sprintf(pxtk_buffer,"%s delete list 0 end ",widget_name);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int PXITkComboBox::clearItem(int index)
{
  sprintf(pxtk_buffer,"%s delete list %d",widget_name,index);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int PXITkComboBox::changeItem(const char* item,int index)
{
  addItem(item,index);
  return clearItem(index+1);
}


int PXITkComboBox::getIndex()
{
  sprintf(pxtk_buffer,"%s curselection",widget_name);
  if (PXTkApp::executeTclCommand(pxtk_buffer)==TCL_OK)
      return PXTkApp::getTclIntResult();

  return 0;
}

int PXITkComboBox::setIndex(int index,PXBool callback)
{
  this->selectItem(index);
  if (callback)
    {
      sprintf(pxtk_buffer,"pxtkcallback %ld %d %ld",
	      event_parent->getHandlerNo(),call_no,(long)this);
      return PXTkApp::executeTclCommand(pxtk_buffer);
    }
  return TCL_OK;
}

int PXITkComboBox::selectItem(int index)
{
  sprintf(pxtk_buffer,"%s selection set %d",widget_name,index);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

int PXITkComboBox::getNumItems()
{
  sprintf(pxtk_buffer,"%s index end",widget_name);
  if (PXTkApp::executeTclCommand(pxtk_buffer)==TCL_OK)
      return PXTkApp::getTclIntResult();
  else
      return 0;
}

int PXITkComboBox::getCurrentItem()
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

char* PXITkComboBox::getItem(int index)
{
  sprintf(pxtk_buffer,"%s get %d",widget_name,index);
  if (PXTkApp::executeTclCommand(pxtk_buffer)==TCL_OK)
      return PXTkApp::getTclStringResult();
  else
      return 0;
}

int PXITkComboBox::initDisplay()
{
  sprintf(pxtk_buffer,"iwidgets::combobox %s -labeltext %s -labelpos w ",widget_name,labelname);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  configure("-width",15);

  if (call_no>0)
      {
	sprintf(pxtk_buffer,"%s configure -selectioncommand { pxtkcallback %ld %d %ld }",
		widget_name,event_parent->getHandlerNo(),call_no,(long)this);
	a=PXTkApp::executeTclCommand(pxtk_buffer);
      }
  
  if (a==TCL_OK)
      is_display_ok=PXTrue;

  return a;

}

int PXITkComboBox::enableEntry(PXBool enab)
{
  if (enab)
    sprintf(pxtk_buffer,"%s entry configure -state normal",widget_name);
  else
    sprintf(pxtk_buffer,"%s entry configure -state disabled",widget_name);
  return PXTkApp::executeTclCommand(pxtk_buffer);
}

