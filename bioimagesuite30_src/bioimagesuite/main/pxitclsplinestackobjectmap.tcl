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





# 	$Id: pxitclsplinestackobjectmap.tcl,v 1.2 2004/02/03 15:57:52 xenios Exp xenios $	

package provide pxitclsplinestackobjectmap 1.0


package require  labelframe 1.0
package require  pxtclutil  1.0
package require  pxitclbaseimagecontrol 1.0
package require  Iwidgets 4.0

itcl::class pxitclsplinestackobjectmap {

    inherit pxitclbaseimagecontrol

    private common thisparam
    protected variable surfaceeditor
    protected variable numberofsurfaces

    constructor { par } {
	pxitclbaseimagecontrol::constructor $par
    } {
    }

    public method Initialize { widget surfaceeditor }
    public method CreateObjectMap { }
    public method CreateDistanceMap { }
    public method MaskImage       { mode }
}


itcl::body pxitclsplinestackobjectmap::Initialize { widget surfaceed } {
	

    set surfaceeditor $surfaceed
    
    if { $initialized == 1 } { return $basewidget }

    set basewidget $widget
    toplevel $basewidget -width 400 -height 250; wm withdraw $basewidget
    wm geometry $basewidget 400x250


    set numberofsurfaces [ $surfaceeditor cget -numberofsurfaces ]

    for { set i 0 } { $i < $numberofsurfaces } { incr i } {
	set thisparam($this,use_stack_$i) 0 
	set thisparam($this,value_$i) [ expr $i + 1 ]
    }
    set thisparam($this,baselevel) 0
    set thisparam($this,use_stack_0) 1

    set w [ frame $basewidget.top -bg darkgray -width 10 ]
    pack $w -side top -expand t -fill both -padx 5 

    set wf [ frame $w.topbase ] ; pack $w.topbase -side top -fill both -expand t
    label $wf.l -text "Background Value:" -width 20
    eval "entry $wf.t -textvariable [ itcl::scope thisparam($this,baselevel) ] -width 4 -relief sunken"
    pack $wf.l $wf.t -padx 10 -side left -expand f -fill x


    for { set k 0 } { $k < $numberofsurfaces } { incr k } {
	    
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
    eval "button $w.com.b1b -text DistanceMap! -command { $this CreateDistanceMap  } "
    eval "button $w.com.b2 -text \"Mask\" -command { $this MaskImage 0  } "
    eval "button $w.com.b2b -text \"Inverse\" -command { $this MaskImage 1  } "
    eval "button $w.com.b3 -text Close -command {  wm withdraw $basewidget }"
    pack $w.com.b1 $w.com.b1b $w.com.b2 $w.com.b2b $w.com.b3 -side left -fill x -expand t   

    set initialized 1
    SetTitle "Surface Object-map Control"
    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget 
    
    
}


itcl::body pxitclsplinestackobjectmap::MaskImage { mode } {

    set ok [ $this CreateObjectMap ]
    if { $ok == 0 } {
	return 0
    }


    WatchOn

    set sc  [  vtkImageThreshold [ pxvtable::vnewobj ] ]
    $sc SetInput  [ $currentresults GetImage ]
    $sc ThresholdBetween -0.5 0.5
    $sc ReplaceOutOn
    $sc ReplaceInOn
    if { $mode > 0 } {
	$sc SetInValue  1
	$sc SetOutValue 0
    } else {
	$sc SetInValue  0
	$sc SetOutValue 1
    }
    $sc Update
    $currentresults ShallowCopyImage [ $sc GetOutput ]
    $sc Delete

    set current_mask  [ $currentresults GetImage ]
    set current_image [ [ $surfaceeditor cget -currentimage ] GetImage ]    

    set ct1 [ vtkImageCast [ pxvtable::vnewobj ] ]
    set ct2 [ vtkImageCast [ pxvtable::vnewobj ] ]
    $ct1 SetOutputScalarTypeToShort
    $ct2 SetOutputScalarTypeToShort
    $ct1 SetInput $current_image
    $ct2 SetInput $current_mask

    set mask  [  vtkImageMathematics [ pxvtable::vnewobj ] ]
    $mask SetInput1 [ $ct1 GetOutput ]
    $mask SetInput2 [ $ct2 GetOutput ]
    $mask SetOperationToMultiply
    SetFilterCallbacks $mask  "Mask x Image"
    $mask Update
    
    $currentresults ShallowCopyImage [ $mask GetOutput ]

    $mask Delete
    $ct1 Delete
    $ct2 Delete

    $surfaceeditor SetResultsFromObject $currentresults  $this 
    
    WatchOff 
    
    return 1

}    

itcl::body pxitclsplinestackobjectmap::CreateObjectMap { } {

    set current_image [ $surfaceeditor cget -currentimage ]
    
    if { [ $current_image GetImageSize ] <1 } {
	::pxtclutil::Warning "No Image in Memory\n Cannot Generate Surface Object Map!"
	return  0
    }
	
    WatchOn	
    puts stdout "Baselevel = $thisparam($this,baselevel)"

    set sc  [  vtkImageThreshold [ pxvtable::vnewobj ] ]
    $sc SetInput [ $current_image GetImage ] 
    $sc ThresholdBetween 0 1
    $sc ReplaceOutOn
    $sc ReplaceInOn
    $sc SetInValue  $thisparam($this,baselevel)
    $sc SetOutValue  $thisparam($this,baselevel)
    $sc Update
    $currentresults ShallowCopyImage [ $sc GetOutput ]
    $sc Delete 

    $currentresults CopyImageHeader [ $current_image GetImageHeader ]
    $currentresults configure -filename    "surface_objectmap.hdr"
    set tmp_image [ $currentresults GetImage ]

    for { set i 0 } { $i < $numberofsurfaces } { incr i } {
	if { $thisparam($this,use_stack_$i) > 0 } {
	    set val $thisparam($this,value_$i) 
	    puts stdout "Using Surface $i val=$val"
	    set stack [ $surfaceeditor GetSurface $i ]
	    $stack AddToObjectMap $tmp_image $val 1 $thisparam($this,baselevel)
	    $surfaceeditor ShowProgressVal "Added [expr $i+1 ]" [ expr (1+ $i )/($numberofsurfaces +0.01)]
	}
    }
    
    $surfaceeditor ShowProgressVal "Done" "1.0"
    $surfaceeditor SetResultsFromObject $currentresults  $this 
    WatchOff
    return 1
}


itcl::body pxitclsplinestackobjectmap::CreateDistanceMap { } {

    set current_image [ $surfaceeditor cget -currentimage ]
    
    if { [ $current_image GetImageSize ] <1 } {
	::pxtclutil::Warning "No Image in Memory\n Cannot Generate Surface Distance Map!"
	return  0
    }
	

    set sur -1
    for { set i 0 } { $i < $numberofsurfaces } { incr i } {
	if { $thisparam($this,use_stack_$i) > 0 } {
	    set sur $i
	    set i [ expr $numberofsurfaces + 1 ]
	}
    }

    if { $sur == - 1 } {
	::pxtclutil::Warning "No Surface Selected from which to create distance map\n"
	return  0
    }
	
    
    WatchOn	

    set img [ $currentresults GetImage ]
    $img CopyStructure [ $current_image GetImage ]
    $img SetScalarTypeToShort
    $img SetNumberOfScalarComponents 1
    $img AllocateScalars
    
    $currentresults CopyImageHeader [ $current_image GetImageHeader ]
    $currentresults configure -filename    "surface_distancemap.hdr"
    
    set stack [ $surfaceeditor GetSurface $sur ]
    $stack FillDistanceMap $img
    $surfaceeditor ShowProgressVal "Done" "1.0"
    $surfaceeditor SetResultsFromObject $currentresults  $this 
    WatchOff
    return 1
}



