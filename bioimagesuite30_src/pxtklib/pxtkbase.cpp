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









#include "pxtkbase.h"

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu
   
   see pxtk.h for details

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

 /* ********************************************************************

    Definition of PXTkBase Class 

 ********************************************************************* */

int    PXTkBase::num_objects=0;

PXTkBase::PXTkBase(PXTkEventObj* par,PXTkGadget* gadg) 
{
  object_id=num_objects;
  ++num_objects;
  has_gadget_parent=PXFalse;
  gadget_parent=NULL;
  has_event_parent=PXFalse;
  event_parent=NULL;

  if (par!=NULL)
      {
	has_event_parent=PXTrue;
	event_parent=par;
      }

  if (gadg!=NULL)
      {
	gadget_parent=gadg;
	has_gadget_parent=PXTrue;
      }

  is_display_ok=PXFalse;
}

PXBool PXTkBase::isGadget() 
{ 
  return PXTrue;
}

PXTkClassId PXTkBase::getClassId() 
{ 
  return PXTkBaseId;
}

int PXTkBase::getObjectId() 
{
  return object_id;
}

PXTkEventObj* PXTkBase::getEventParent()
{
  return event_parent;
}

PXBool PXTkBase::hasEventParent()
{
  return has_event_parent;
}

PXTkGadget* PXTkBase::getGadgetParent()
{
  return gadget_parent;
}

PXBool PXTkBase::hasGadgetParent()
{
  return has_gadget_parent;
}

int PXTkBase::initDisplay() 
{
  return TCL_OK;
}

PXBool PXTkBase::isDisplayOk()
{
  return is_display_ok;
}

int PXTkBase::enable(PXBool )
{
  return TCL_OK;
}

int PXTkBase::disable()
{
  return enable(PXFalse);
}

int  PXTkBase::Irange(int   a,int   minv,int   maxv)
{
  if (a<minv) a=minv;
  if (a>maxv) a=maxv;
  return a;
}


float PXTkBase::Frange(float a,float minv,float maxv)
{
  if (a<minv) a=minv;
  if (a>maxv) a=maxv;
  return a;
}


