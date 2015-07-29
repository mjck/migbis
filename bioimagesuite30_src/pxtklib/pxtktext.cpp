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





// pxtktext.cpp
#include "pxtktext.h"
#include <vtkstd/string>
#include "sstream"
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu
   
   see pxtktext.h for details

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

/* ********************************************************************
   
   Definition of PXTkText Class 
   
   ********************************************************************* */

PXTkText::PXTkText(PXTkEventObj* evpar,PXTkGadget* gadg_par,
		   PXBool dodisplay):PXTkLabel(evpar,gadg_par,NULL,PXFalse)
{
  if (dodisplay)
      initDisplay();
}

int PXTkText::initDisplay()
{
  sprintf(pxtk_buffer,"text %s ",widget_name);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  if (a==TCL_OK)
      is_display_ok=PXTrue;
  return a;
}

int PXTkText::addText(const char* newtext)
{
  if (!is_display_ok)
      return TCL_ERROR;

  PXBool st=getEditableState();
  if (st==PXFalse)
    setEditableState(PXTrue);
  
  int a=this->addTextDirect(newtext);
  if (a==TCL_OK)
    {
      std::stringstream s2;
      s2 << widget_name << " see end";
      a=PXTkApp::executeTclCommand(s2.str().c_str());
    }
  
  if (st==PXFalse)
      setEditableState(PXFalse);
  return a;
}

int PXTkText::clearText()
{
  if (!is_display_ok)
      return TCL_ERROR;

  PXBool st=getEditableState();
  if (st==PXFalse)
      setEditableState(PXTrue);

  sprintf(pxtk_buffer,"%s delete 1.0  end  ",widget_name);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);

  if (st==PXFalse)
      setEditableState(PXFalse);
  return a;

}

int PXTkText::addTextDirect(const char* newtext)
{
  if (!is_display_ok)
    return TCL_ERROR;

  std::string s(newtext);
  PXTkApp::StringSanitize(s);
  //  fprintf(stderr,"Sanitized = %s\n",s.c_str());
  std::string s2(widget_name);
  s2+= " insert end \"" + s + " \"";
  //  fprintf(stderr,"Executing %s\n",s2.c_str());
  return PXTkApp::executeTclCommand(s2.c_str());
}


int PXTkText::setEditableState(PXBool state)
{
  if (!is_display_ok)
      return TCL_ERROR;
  
  if (state)
    return configure("-state","normal");
  else
    return configure("-state","disabled");
}

PXBool PXTkText::getEditableState()
{
  if (!is_display_ok)
      return PXFalse;

  char* line=cget("-state");
  if (line==NULL)
    return PXFalse;
  
  if (strcmp(line,"normal")==0)
    return PXTrue;
  else
    return PXFalse;
}


