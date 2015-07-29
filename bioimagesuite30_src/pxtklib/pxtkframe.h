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









// pxtkframe.h

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PxTk library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : PXTkFrame

_Description : Definition of class PXTkFrame to warp Frame TK Widget in C++


_Call :   
        
	
_References : Practical Programming in Tcl and Tk, Brent B. Welch (Prentice Hall 2nd Edition)
             
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 2nd January 2001

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#include "pxtkgadget.h"

#ifndef _PXTkFrame
#define  _PXTkFrame

/* ----  Definition of PXTkFrame Class  ---- */

class PXTkFrame : public PXTkGadget
{
protected:
  PXBool pack_mode;

public:

  PXTkFrame(PXTkEventObj* evpar,PXTkGadget* gadg_par,int width=-1,int height=-1,PXBool dodisplay=PXTrue);
  PXTkFrame(PXTkEventObj* evpar,const char* name);

  virtual ~PXTkFrame();

  virtual int initDisplay();
  virtual PXTkClassId   getClassId();

  // Addition of Children 
  virtual int addChildren(const char* options,PXTkGadget* child1,
			  PXTkGadget* child2=NULL,PXTkGadget* child3=NULL,PXTkGadget* child4=NULL,PXTkGadget* child5=NULL);
  virtual int childConfigure(PXTkGadget* child,const char* option,const char* value);


  // Special Grid Options
  virtual int gridRowColumnChild(PXTkGadget* child,int row,int column,int rowspan=1,int columnspan=1);
  virtual int gridRowColumnConfigure(PXTkGadget* child,int row,int column,int rowspan=1,int columnspan=1);
  virtual int gridExpandRowConfigure(int row,int weight=1,int minsize=-1,int pad=-1);
  virtual int gridExpandColumnConfigure(int row,int weight=1,int minsize=-1,int pad=-1);

  
  // Unmap Child
  virtual int   unmapChild(PXTkGadget* child);
  virtual int   mapChild(PXTkGadget* child,const char* options);

  // PackMode=PXTrue --> packer else grid
  virtual void   setPackMode(PXBool pack);
  virtual PXBool getPackMode();

  // UsefulOptions
  virtual int setDimensions(int width=-1,int height=-1);
      
};

#endif

