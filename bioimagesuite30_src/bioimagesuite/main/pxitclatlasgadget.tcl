#!/bin/sh
# the next line restarts using wish \
    exec vtk "$0" "$@"

#BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
#BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
#BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
#BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
#BIOIMAGESUITE_LICENSE  Medicine, http:#www.bioimagesuite.org.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
#BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
#BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
#BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
#BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
#BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
#BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#BIOIMAGESUITE_LICENSE  See also  http:#www.gnu.org/licenses/gpl.html
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
#BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
#BIOIMAGESUITE_LICENSE 
#BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------




# This tool loads and interogates the FUWFU Pickatlas version 2.4 
# It also   interogates the Yale Atlas if present
#
# No part of the atlas software/images is integrated by BioImage Suite, it simply
# looks for the atlas software and loads the ROIs
#
#     The wfu_pickatlas toolbox and user manual can be obtained from:
#        www.fmri.wfubmc.edu 


package provide pxitclatlasgadget 1.0

# 	$Id: pxitclatlasgadget.tcl,v 1.2 2004/02/03 17:36:31 xenios Exp xenios $	

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]


package require  vtkpxcontrib 1.1
package require  pxtclutil    1.0
package require  pxvtable     1.0
package require  Itcl         3.2
package require pxitclwfuatlasgadget 1.0
package require pxitclyaleatlasgadget 1.0
# ----------------------------------------------------------------------------------------------

itcl::class pxitclatlasgadget {

    inherit pxitclbasecontrol
    
    # Generic Stuff 

    protected variable wfugadget 0
    protected variable yalegadget 0

    public variable enable_yalegadget 1
    public variable enable_wfugadget  1

    # public 

    
    #
    constructor { par args } {
	pxitclbasecontrol::constructor $par
    } {
	InitializeAtlasGadget
    }

    protected method InitializeAtlasGadget { }

    public method Initialize { widget }
    public method DelayedInitialize { }
    public method AddToMenuButton { mb args }
    public method IdentifyPoint { x y z } 
    public method SetError { txt }
    public method SetImageFromObject   { img obj }
    public method ShowGadget { md }

}


::itcl::body pxitclatlasgadget::AddToMenuButton { mb args} {

    foreach gadg [ list $yalegadget $wfugadget ] { 
	if { $gadg !=0 } {
	    $gadg AddToMenuButton $mb $args
	}
    } 

}

::itcl::body pxitclatlasgadget::ShowGadget { md } {

    if { $md == "wfu" } {
	$wfugadget ShowWindow
    } else {
	$yalegadget ShowWindow
    }
}


itcl::body pxitclatlasgadget::InitializeAtlasGadget { } {

    if { $enable_wfugadget } {
	set wfugadget  [ [ pxitclwfuatlasgadget \#auto $this ] GetThisPointer ]
    }
    
    if { $enable_yalegadget == 1 } {
	set yalegadget [ [ pxitclyaleatlasgadget \#auto $this ] GetThisPointer ]
    }

#    puts stderr "Created gadgets wfu=$wfugadget, yale=$yalegadget"
}

itcl::body pxitclatlasgadget::Initialize { widget } { 

    set i 0


    foreach gadg [ list $wfugadget $yalegadget ] { 
	if { $gadg !=0 } {
	    $gadg Initialize ${widget}_$i
	    incr i
	}
    } 

}

itcl::body pxitclatlasgadget::DelayedInitialize { } { 

    foreach gadg [ list $wfugadget $yalegadget ] { 
	if { $gadg !=0 } {
	    $gadg DelayedInitialize
	    $gadg LoadAtlas
	}
    }

    set fullyinitialized 1
}

itcl::body pxitclatlasgadget::IdentifyPoint { x y z }  {

    foreach gadg [ list $wfugadget $yalegadget ] { 
	if { $gadg !=0 } {
	    $gadg IdentifyPoint $x $y $z
	}
    }
}

itcl::body pxitclatlasgadget::SetImageFromObject { img obj }  {

    foreach gadg [ list $wfugadget $yalegadget ] { 
	if { $gadg !=0 } {
	    $gadg SetImageFromObject $img $obj
	}
    }
}


itcl::body pxitclatlasgadget::SetError { txt } { 

    foreach gadg [ list $wfugadget $yalegadget ] { 
	if { $gadg !=0 } {
	    $gadg SetError $txt
	}
    }
}


