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





// PXTkanaheader.h
// -----------------------------------------------------------------

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000 papad@noodle.med.yale.edu


_Module_Name : PXTkAnaHeaderControl


_Description : Definition of Classes to draw iso-surfaces 
 
               PXTkAnaHeaderControl()

_Call : 

  
_References : The Visualization Toolkit: An Object-Oriented
              Approach to Graphics. 2nd Edition. Prentice-Hall
	      Schroeder, Martin and Lorensen
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 17th August 2000
                                
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PXTK_AnaHeaderControl
#define _PXTK_AnaHeaderControl
#include "pxutil.h"
#include "pxtkapp.h"
#include "pxtkdialog.h"
#include "pxtkgadgets.h"
#include "pxtkcomplexwidgets.h"
#include "pxtkseldialog.h"
#include "pxtkmenu.h"

// ****************************************
// ******* PXTkAnaHeaderControl Class ********
// ****************************************

class  PXTkAnaHeaderControl : public PXTkDialog
{
protected:
  anaimagedata *header;
  int           header_bits;
  char*         header_filename;
 
  PXTkLabel* label[9];
  PXTkEntry* entry[9];

  PXTkOptionMenu* orientation;
  PXTkOptionMenu* data_type;
  PXTkCheckButton* byte_swapped;

public:
  
  PXTkAnaHeaderControl(PXTkEventObj* evpar,PXTkGadget* par);

  virtual ~PXTkAnaHeaderControl();

  virtual int  initDisplay();
  virtual int  handleEvent(int);

  virtual int  readHeader(const char* name);
  virtual int  saveHeader(const char* name);
  virtual int  resetHeader();
  virtual int  setHeaderFromDialog();
  virtual int  displayHeader();
};



#endif   


