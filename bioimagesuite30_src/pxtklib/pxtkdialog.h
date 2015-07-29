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





// pxgrinfodialog.h
// -----------------------------------------------------------------

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000 papad@noodle.med.yale.edu


_Module_Name : PXTkDialog


_Description : This is a abstract class to create popup-dialogs
               And Base Windows 

_Call : 
  
_References : 

	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 11th May 2000

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PXTk_DIALOG
#define _PXTk_DIALOG

#include "pxtkbase.h"
#include "pxtkeventobj.h"
#include "pxtkgadget.h"
#include "pxtkframe.h"
#include "pxtktoplevel.h"
#include  <stdlib.h>

// ****************************************
// ******* PXTkDialog Class ************
// ****************************************

class PXTkDialog : public PXTkEventObj {
  
protected:
  
  PXBool          create_inside_parent,modality;
  char*           dlg_title;

  int             height,width;
  
public:

  PXTkDialog(PXTkEventObj* par, const char* title,PXBool insideparent=PXFalse);
  PXTkDialog(PXTkEventObj* par,PXTkGadget* w, const char* title,PXBool insideparent=PXFalse);
  PXTkDialog(PXTkGadget*   par, const char* title,PXBool insideparent=PXFalse);
  virtual ~PXTkDialog();

  void init(const char* ttl,PXBool inside);
  
  virtual int  initDisplay();
  virtual void setTitle(const char* title);

  virtual void popupDialog();
  virtual void popdownDialog();

  virtual void exitCallback(const char* line=NULL);
  virtual void aboutCallback(const char* line=NULL);

  PXBool isOpen();
  PXBool isInside();

  virtual void setSize(int w,int h);
  virtual void setModality(PXBool modal=PXTrue);

  virtual int handleEvent(int eventno);

  virtual   PXTkClassId    getClassId();
};

#endif   


