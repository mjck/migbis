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





# 	$Id: pxitclsplineobjectmap.tcl,v 1.1 2003/03/03 20:17:28 papad Exp $	

package provide pxitclsplineobjectmap 1.0


package require  labelframe 1.0
package require  pxtclutil  1.0
package require  pxitclbaseimagecontrol 1.0
package require  Iwidgets 4.0

itcl::class pxitclsplineobjectmap {

    inherit pxitclbaseimagecontrol

    private common thisparam
    protected variable splineeditor
    protected variable numberofsplines

    constructor { par } {
	pxitclbaseimagecontrol::constructor $par
    } {
    }

    public method Initialize { widget splineeditor }
    public method CreateObjectMap { }
}


itcl::body pxitclsplineobjectmap::Initialize { widget splineed } {
	

    set splineeditor $splineed
    
    if { $initialized == 1 } { return $basewidget }

    set basewidget $widget
    toplevel $basewidget -width 400 -height 250; wm withdraw $basewidget
    wm geometry $basewidget 400x250


    set numberofsplines [ $splineeditor cget -numberofsplines ]

    for { set i 0 } { $i < $numberofsplines } { incr i } {
	set thisparam($this,use_stack_$i) 0 
	set thisparam($this,value_$i) [ expr $i + 1 ]
    }
	
    set thisparam($this,use_stack_0) 1

    set w [ frame $basewidget.top -bg darkgray -width 10 ]
    pack $w -side top -expand t -fill both -padx 5 

    for { set k 0 } { $k < $numberofsplines } { incr k } {
	    
	set wf [ frame $w.top$k ] ; pack $w.top$k -side top -fill both -expand t
	set v "Use Stack [expr $k +1 ]"
	set a "use_stack_$k"
	set b "value_$k"
	
	eval "checkbutton $wf.c -variable [ itcl::scope thisparam($this,$a) ] -text \"$v\" "
	label $wf.l -text "Object Map Value:" -width 15
	eval  "entry $wf.t -textvariable [ itcl::scope thisparam($this,$b) ] -width 4 -relief sunken"
	pack $wf.c $wf.l $wf.t -padx 10 -side left -expand f -fill x
    }
	
    frame $w.com; 
    pack $w.com -side top -fill x
    eval "button $w.com.b1 -text Create! -command { $this CreateObjectMap  } "
    eval "button $w.com.b3 -text Close -command {  wm withdraw $basewidget }"
    pack $w.com.b1  $w.com.b3 -side left -fill x -expand t   

    set initialized 1
    SetTitle "Spline Object-map Control"
    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget 
    
    
}


itcl::body pxitclsplineobjectmap::CreateObjectMap { } {

    set current_image [ $splineeditor cget -currentimage ]
    set level [ $splineeditor GetCurrentSlice ]

    if { [ $current_image GetImageSize ] <1 } {
	::pxtclutil::Warning "No Image in Memory\n Cannot Generate Spline Object Map!"
	return  0
    }
	
    WatchOn	
    set sc [ vtkImageShiftScale [ pxvtable::vnewobj ]]
    $sc SetInput [ $current_image GetImage ] 
    $sc SetShift 0
    $sc SetScale 0
    $sc Update
    $currentresults ShallowCopyImage [ $sc GetOutput ]
    $sc Delete 

    $currentresults CopyImageHeader [ $current_image GetImageHeader ]
    $currentresults configure -filename    "spline_objectmap.hdr"
    set tmp_image [ $currentresults GetImage ]

    for { set i 0 } { $i < $numberofsplines } { incr i } {
	if { $thisparam($this,use_stack_$i) > 0 } {
	    set val $thisparam($this,value_$i) 
	    set stack [ $splineeditor GetSpline $i ]
	    $stack AddToObjectMap  $tmp_image $level $val 1
	    $splineeditor ShowProgressVal "Added [expr $i+1 ]" [ expr (1+ $i )/($numberofsplines +0.01)]
	}
    }

    $splineeditor ShowProgressVal "Done" "1.0"
    $splineeditor SetResultsFromObject $currentresults  $this 
    WatchOff
    return 1
}



