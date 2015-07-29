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





// pxtkeventobj.h

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PxTk library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : PXTkEventObj

_Description : Definition of PXTkEventObj -- base class for all classes that handle callbacks

_Call :   
        
	
_References : 
              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 11th May 2000

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PXTkEventObj
#define  _PXTkEventObj

#include "pxtkbase.h"


class PXTkEventObj : public PXTkBase
{
protected:	
  // ***** Class Fields *******
  //int                   handler_no;
  PXTkGadget*           main_window;
  static char           pxtk_buffer[4000], pxtk_buffer2[4000];

private:
  static char           pxtk_intbuffer[4000], pxtk_intbuffer2[4000],pxtk_intbuffer3[4000],pxtk_intbuffer4[4000];

 public:
  
  // ****** Constructors & Destructor *****
  PXTkEventObj(PXTkEventObj* parent,PXTkGadget* widget);

  virtual              ~PXTkEventObj();	

  // ****** Member Access *****    
  virtual PXBool       isGadget();
  virtual PXTkClassId   getClassId();

  virtual long          getHandlerNo();
  //virtual void         setHandlerNo(int h=0);
  virtual PXTkGadget*  getMainWindow();


  // ****** Callback related ******
  virtual int           handleEvent(int eventno);
  virtual int handleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* obj=NULL);

  // ****** TCL Utility Code *****
  virtual int   packMainWindow();
  virtual int   setWaitCursor(PXBool setwait=PXTrue);

  virtual int   messageBox(const char* message, const char* title=NULL, const char* mboxtype=NULL);
  virtual int   questionBox(const char* message);
  virtual int   colorBox(const char* title, float& r, float& g, float& b);
  virtual int   d_colorBox(const char* title,double& dr,double& dg,double& db);  

  virtual char* getOpenFilename(const char* title, const char* extname=NULL, const char *ext=NULL);
  virtual char* getSaveFilename(const char* title, const char* orig=NULL, 
		                const char* extname=NULL, const char* ext=NULL);

  virtual char* getOpenFilenameComplex(const char* title, const char* filetypes);
  virtual char* getSaveFilenameComplex(const char* title, const char* orig, const char* filetypes);

  virtual char* chooseDirectory(const char* title);

};


#endif /*  _PXTkEventObj */

