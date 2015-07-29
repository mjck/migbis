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





# 	$Id: pxitclsplinestackobjectmap.tcl,v 1.1 2003/03/03 20:17:28 papad Exp $	

package provide pxitclcreatesolidcontrol 1.0


package require  labelframe 1.0
package require  pxtclutil  1.0
package require  pxitclbaseimagecontrol 1.0
package require  Iwidgets 4.0

itcl::class pxitclcreatesolidcontrol {

    inherit pxitclbaseimagecontrol

    private common thisparam
    protected variable base4dviewer
    protected variable numberofsurfaces
    protected variable solid 0
    protected variable solidactorlist ""
    protected variable rendererlist   ""

    constructor { par } {
	pxitclbaseimagecontrol::constructor $par
	set solid [ vtkpxFemSolid [ pxvtable::vnewobj ]]
    } {
    }

    public method Initialize   { widget base4dviewer }
    public method CreateSolid  { }
    public method LoadSolid    { }
    public method SaveSolid    { }
    public method ToggleDisplay { }
    public method DisplaySolid { }
}


itcl::body pxitclcreatesolidcontrol::Initialize { widget surfaceed } {
	
    set base4dviewer $surfaceed
    
    if { $initialized == 1 } { return $basewidget }

    set basewidget $widget
    toplevel $basewidget; wm withdraw $basewidget
    wm geometry $basewidget 450x100


    set numberofsurfaces [ $base4dviewer cget -numberofsurfaces ]

    set thisparam($this,inner_surface) 1
    set thisparam($this,outer_surface) 2
    set thisparam($this,numpoints) 35
    set thisparam($this,numstacks) 4
    set thisparam($this,skipslices) 2
    set thisparam($this,bias) 1

    set surlist ""
    for { set k 0 } { $k < $numberofsurfaces } { incr k } {
	lappend surlist [ expr $k + 1 ]
    }

    set w [ frame $basewidget.top -bg darkgray -width 10 ]
    pack $w -side top -expand t -fill x -padx 5 
  
    frame $w.s1 ; frame $w.s2 ; pack $w.s1 $w.s2 -side top -expand f -fill x

    set cw [ [ iwidgets::labeledwidget  $w.s1.i -labeltext "Inner Surface:"] childsite ]
    pack $w.s1.i -side left -expand f -fill x -padx 10
    eval "tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,inner_surface) ] $surlist"
    pack $cw.optmenu  -expand f -fill x

    set cw [ [ iwidgets::labeledwidget  $w.s1.o -labeltext "Outer Surface:"] childsite ]
    pack $w.s1.o -side left -expand f -fill x -padx 10
    eval "tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,outer_surface) ] $surlist"
    pack $cw.optmenu  -expand f -fill x



    set base $w.s2

    set k 0 
    iwidgets::entryfield $base.$k -labeltext "Points:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,numpoints) ] -relief sunken 
    pack $base.$k -side left -expand f -fill x  ;incr k


    iwidgets::entryfield $base.$k -labeltext "Layers:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,numstacks) ] -relief sunken 
    pack $base.$k -side left -expand f -fill x  ;incr k


    iwidgets::entryfield $base.$k -labeltext "Skip Slices:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,skipslices) ] -relief sunken 
    pack $base.$k -side left -expand f -fill x  ;incr k


    iwidgets::entryfield $base.$k -labeltext "Endo Bias(0,1)"  -width 1 -validate real -textvariable [ itcl::scope thisparam($this,bias) ] -relief sunken 
    pack $base.$k -side left -expand f -fill x  ;incr k

	
    frame $w.com; 
    pack $w.com -side top -fill x
    eval "button $w.com.b1 -text Create! -command { $this CreateSolid  } "
    eval "button $w.com.b2a -text Load -command { $this LoadSolid } "
    eval "button $w.com.b2b -text Save -command { $this SaveSolid } "
    eval "button $w.com.b2c -text Show/Hide -command { $this ToggleDisplay } "
    eval "button $w.com.b3 -text Close -command {  wm withdraw $basewidget }"
    pack $w.com.b1 $w.com.b2a $w.com.b2b $w.com.b2c $w.com.b3 -side left -fill x -expand t   

    entry $w.base -textvariable [ itcl::scope thisparam($this,results) ] -relief raised -state disabled  -fg white -bg black
    pack $w.base -side bottom -fill x -expand f

    set initialized 1
    SetTitle "Surface Solid Control"
    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget 
    
}


itcl::body pxitclcreatesolidcontrol::CreateSolid { } {

    set endo [ $base4dviewer GetTstack [ expr $thisparam($this,inner_surface)-1 ] ]
    set epi  [ $base4dviewer GetTstack [ expr $thisparam($this,outer_surface)-1 ] ]
       
    

    set thisparam($this,results) "Geneating Solid $thisparam($this,numpoints) $thisparam($this,numstacks) $thisparam($this,bias) 1.0 $thisparam($this,skipslices)"
    WatchOn

    set ok [ $solid GenerateHexahedralSolid $endo $epi $thisparam($this,numpoints) $thisparam($this,numstacks) $thisparam($this,bias) 1.0 $thisparam($this,skipslices) ]
    WatchOff
    if { $ok == 0 } {
	pxtclutil::Warning "Failed to Generate Solid!"
	return 0
    }
    
    SetTitle "Surface Solid Control \[ new solid \] "
    DisplaySolid 
    return 1
}

itcl::body pxitclcreatesolidcontrol::ToggleDisplay { } {

    if { [ llength $rendererlist ] > 0 } {
	for { set i 0 } {$i < [ llength $rendererlist ] } { incr i } {
	    set a [ lindex $solidactorlist $i ]
	    $a SetVisibility [ expr 1 - [ $a GetVisibility ] ]
	}
	[ [ lindex $rendererlist 0 ] GetRenderWindow ] Render
    }
}

itcl::body pxitclcreatesolidcontrol::DisplaySolid { } {


    if { [ llength $rendererlist ] > 0 } {
	for { set i 0 } {$i < [ llength $rendererlist ] } { incr i } {
	    set r [ lindex $rendererlist $i ]
	    set a [ lindex $solidactorlist $i ]
	    catch { $r RemoveActor $a ; $a Delete
	    }
	}
    }

    set thisparam($this,results) "Nodes = [ $solid GetNumberOfNodes ], Elements = [ $solid GetNumberOfElements]"
	    
    set rendererlist ""
    set solidactorlist ""
    $solid Update

    for { set i 0 } { $i <= 3 } { incr i } {
	set renderer [ [ [ $base4dviewer GetViewer ] GetMultiRenderer $i ] GetRenderer ]
	lappend rendererlist $renderer
	
	
	set map [ vtkPolyDataMapper [ pxvtable::vnewobj ]]
	$map SetInput [ $solid GetOutput ]
	set solidactor [ vtkActor  [ pxvtable::vnewobj ]]
	$solidactor SetMapper $map
	$map Delete

	[ $solidactor GetProperty ] SetColor 1.0 1.0 0.0

	if { $i < 3 } {
	    [ $solidactor GetProperty ] SetLineWidth 2.0
	    [ $solidactor GetProperty ] SetAmbient 1.0
	    [ $solidactor GetProperty ] SetDiffuse 0.0
	    [ $solidactor GetProperty ] SetSpecular 0.0
	    [ $solidactor GetProperty ] SetRepresentationToWireframe
	} else {
	    
	}
	$renderer AddActor $solidactor
	lappend solidactorlist $solidactor
    }
    [ [ lindex $rendererlist 0 ] GetRenderWindow ] Render
}

itcl::body pxitclcreatesolidcontrol::LoadSolid { } {

    set typelist { 
	{"Solid  file" {.sld}} 
	{"All Files" {*}}
    }
	    
    set fname [tk_getOpenFile -title "Load Solid" -filetypes $typelist]
    
    if { [string length $fname ] < 2 } { return 0 }
    if { [ file exists $fname ] == 0 } { return 0 }

    if { [ $solid Load $fname ] > 0 } {
	SetTitle "Surface Solid Control [ file tail $fname]"
	DisplaySolid
    }
    
}

itcl::body pxitclcreatesolidcontrol::SaveSolid { } {

    set typelist { 
	{"Solid  file" {.sld}} 
	{"All Files" {*}}
    }
	    
    set fname [tk_getSaveFile -title "Save Solid" -filetypes $typelist]
    
    if { [string length $fname ] < 2 } { return 0;   }

    if { [ $solid Save $fname ] > 0 } {
	SetTitle "Surface Solid Control [ file tail $fname]"
    }

}

