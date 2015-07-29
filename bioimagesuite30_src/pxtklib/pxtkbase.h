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









// pxtkbase.h

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PxTk library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : PXTk

_Description : Definition of PXTkBase --- Abstract base class for all objects in this library

_Call :   
        
	
_References : 
              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 11th May 2000

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PXTkBase
#define  _PXTkBase

#include "pxtkapp.h"

enum PXTkClassId { PXTkBaseId = 0 , PXTkGadgetId, PXTkObjId ,PXTkDialogId,PXTkFrameId,PXTkToplevelId,PXVTkComponentId};


class PXTkEventObj;
class PXTkGadget;

class PXTkBase {

protected:

  static  int              num_objects;
  int                      object_id;
  PXTkEventObj*            event_parent;
  PXBool                   has_event_parent;
  PXTkGadget*              gadget_parent;
  PXBool                   has_gadget_parent;
  PXBool                   is_display_ok;

public:
  PXTkBase(PXTkEventObj* par=NULL,PXTkGadget* gadg=NULL);

  virtual   PXBool         isGadget();
  virtual   PXTkClassId    getClassId();
  virtual   int            getObjectId();
 
  virtual   PXTkEventObj*  getEventParent();
  virtual   PXBool      hasEventParent();

  virtual PXTkGadget*   getGadgetParent();
  virtual PXBool        hasGadgetParent();

  virtual int          initDisplay();
  virtual PXBool       isDisplayOk();

  virtual int          enable(PXBool enab=PXTrue);
  virtual int          disable();

  static int   Irange(int   a,int   minv,int   maxv);
  static float Frange(float a,float minv,float maxv);

};


#endif /*  _PXTkBase */

