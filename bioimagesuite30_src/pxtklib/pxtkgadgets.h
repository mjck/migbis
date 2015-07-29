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





// pxtkgadgets.h

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PxTk library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : PXTkGadgets

_Description : Definition of abstract class PXTkGadget
               and derived classes to wrap 15 original TK Widgets 

PXTkGadget -+-> PXTkFrame (1)  ---> PXTkToplevel (2)   (done)
	    |                
	    |
	    +-> PXTkLabel (3)  -+-> PXTkButton (4)  -+-> PXTkMenuButton  (5)        (done)
            |     	        |                    |                                                
            |                   |                    +-> PXTkCheckButton (6) ---> PXTkRadioButton  (7)  (done)
            |     	        |                   	                                                  
            |	                |                   	                           
            |        	        |                   
	    |                   |                   
	    |                   |
	    |                   +-> PXTkMessage (8)   (done 
	    |                   |
	    |                   +-> PXTkEntry (9)     (done)
	    |                   |                                
	    |                   +-> PXTkText (10)     (done)
	    |                                  
	    |                                  
            |
	    +-> PXTkScale (11)  (done)
	    |
	    +-> PXTkListBox (12) (done)
	    |
	    +-> PXTkScrollBar (13) (done)
	    |
	    +-> PXTkMenu (14) (done)  (Also PXTkOptionMenu (14a))
	    |
	    +-> PXTkCanvas (15)

	    Not all implemented yet 


_Call :   
        
	
_References : Practical Programming in Tcl and Tk, Brent B. Welch (Prentice Hall 2nd Edition)
             
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 11th May 2000

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#include "pxtkbase.h"
#include "pxtkapp.h"

class PXTkEventObj;

#ifndef _PXTkGadgets
#define  _PXTkGadgets


#include "pxtkgadget.h"
#include "pxtkframe.h"
#include "pxtktoplevel.h"
#include "pxtklabel.h"
#include "pxtkmessage.h"
#include "pxtkbutton.h"
#include "pxtkmenubutton.h"
#include "pxtkcheckbutton.h"
#include "pxtkradiobutton.h"
#include "pxtkentry.h"
#include "pxtktext.h"
#include "pxtkscale.h"
#include "pxtkscrollbar.h"
#include "pxtklistbox.h"
#include "pxtkmenu.h"
#include "pxtkoptionmenu.h"

#endif

