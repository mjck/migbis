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









// pxgrseldialog.h
// -----------------------------------------------------------------

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000 papad@noodle.med.yale.edu


_Module_Name : PXTkSeldialog


_Description : This is a dialog class to present options


_Call : 

_References : 

	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 11th May 2000

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PXDRAW_SEL_DIALOG
#define _PXDRAW_SEL_DIALOG

#include "pxtkdialog.h"
#include "pxtkgadgets.h"

// ****************************************
// ******* PXTkSelDialog Class ************
// ****************************************

class PXTkSelDialog : public PXTkDialog {
  
protected:
  
  // GUI Related ;
  
  int  menu_option,max_num_options,label_width,entry_width,debug_option,show_debug;
  int  num_options;
  int  pack_step;
  
  PXTkFrame* optionFrame;



public:
  
  PXTkSelDialog(PXTkEventObj* par,PXTkGadget* w,int n);
  virtual ~PXTkSelDialog();

  PXTkLabel* label[20];
  PXTkEntry* entry[20];
  PXTkMessage* message;
  PXTkButton*  debugbutton;

  // ----------------------------------------------------------
  // Parameter Setup
  // ----------------------------------------------------------

  virtual  void  setfoption(int i,const char* lab,const char* fmt,float val);
  virtual  void  setoption(int i,const char* lab,const char* fmt,const char* val);
  virtual  void  setioption(int i,const char* lab,const char* fmt,int val);

  virtual  char* getoption(int i);
  virtual  float getfoption(int i);
  virtual  int   getioption(int i);

  virtual  int   getNumOptions();
  virtual  int   setNumOptions(int newnumoptions);
  virtual void   setDescription(const char* description);
  virtual void   setTitleDescription(const char* title,const char* description);
  virtual void   setMenuOption(int m);

  // Third Button Options
  virtual void   setDebugButton(const char* label,int option);
  virtual void   hideDebugButton();
  virtual void   showDebugButton();

  // ----------------------------------------------------------
  // GUI Related
  // ----------------------------------------------------------
  
  virtual int  initDisplay();
  virtual int  handleEvent(int);

protected:
  
  virtual int getGridRow(int optionid);
  virtual int getGridColumn(int optionid);
};

#endif   


