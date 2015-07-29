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





# 	$Id: pxitcl4dsurfaceeditor.tcl,v 1.3 2003/09/03 13:59:52 xenios Exp xenios $	
lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]

package provide pxitcl4dsurfaceeditor 1.0

# -----------------------
# Dependencies and path
# -----------------------
package require pxtclutil 1.0
package require vtkpxcontrib  1.1
package require pxitclbase4dimageviewer 1.0

# -----------------------------------------------------
#  Intialize Variables
# -----------------------------------------------------
itcl::class pxitcl4dsurfaceeditor {

    inherit pxitclbase4dimageviewer 

    protected variable splinestackcontrol 0
    protected variable splineeditor       0 
    public    variable gui_splineeditor   0
    public    variable gui_masterframe    0
    public    variable gui_masterslice    0
    public    variable gui_currentframe   0
    public    variable gui_currentslice   0


    constructor { par } {
	::pxitclbase4dimageviewer::constructor $par
    } {
	Initialize4dsurfaceeditor
    }
    
    private method Initialize4dsurfaceeditor { }
    public method SetImageFromObject { this obj } 
    
    # Callback Handling
    public method FrameChangedCallback { } 
    public method SliceChangedCallback { } 
    public method UpdateMasterFrame    { } 
    public method CreateMasterFrame { w }
    public method UpdateFromMasterFrame { mode }

    # Display Initialization 
    protected method InitializeViewer  {  } 
    public    method InitializeDisplay { widg standalone } 



    # Get Surface from Spline Stack Control
    public    method GetSurface       { index } 
    public    method GetCurrentFrame  { } 
    public    method GetNumberOfFrames { } 
    public    method SetCurrentFrame  { index } 


    public    method CacheAllFrames { }
}
# -----------------------------------------------------
itcl::body pxitcl4dsurfaceeditor::Initialize4dsurfaceeditor { } {

    set appname "BioImage Suite::4D Surface Editor"

    set splinestackcontrol [ vtkpxGUI4DSplineStackControl [ pxvtable::vnewobj ]]
    set splineeditor  [ vtkpxGUISplineEditor [ pxvtable::vnewobj ]]
    $splineeditor SetSlaveMode 1


    if { $normalize_anatomical  == 1 } {
	$splineeditor SetPresetLookupTableMode 0
    } else {
	$splineeditor SetPresetLookupTableMode -1
    }
    $splinestackcontrol SetSplineEditor $splineeditor


    set enable_headercontrol   1
    set enable_polydatacontrol 0
    set enable_overlaytool     0
}
    
# ---------------------------------------------------------------------------------
#  GUI Stuff 
# ---------------------------------------------------------------------------------

itcl::body pxitcl4dsurfaceeditor::SetImageFromObject { img obj } {

    set a [ pxitclbase4dimageviewer::SetImageFromObject $img $obj ]
    if { $a==0 } { return 0 }

    set numf [ [ $currentimage GetImage  ] GetNumberOfScalarComponents ]
    set numsl [ lindex [  [ $currentimage GetImage  ] GetDimensions ] 2 ]
    catch { $thisparam($this,gui_masterframe) configure -from 1 -to $numf }
    catch { $thisparam($this,gui_masterslice) configure -from 0 -to [expr $numsl -1 ] }
    
    if { $splinestackcontrol != 0 } {
	$splineeditor LinkLookupTable [ $vtk_viewer GetLookupTable ]
	$splinestackcontrol SetImage [ $currentimage GetImage ] [ $currentimage GetOrientation ] 0 0
    }
	
    UpdateMasterFrame




}
    
# ---------------------------------------------------------------------------------
#  Update Stuff -- for change frame communication between image and splinestack
# ---------------------------------------------------------------------------------
itcl::body pxitcl4dsurfaceeditor::GetCurrentFrame  { } {
    return [ $vtk_viewer GetCurrentFrame]
}


itcl::body pxitcl4dsurfaceeditor::GetNumberOfFrames  { } {
    return [ $vtk_viewer GetNumberOfFrames]
}


itcl::body pxitcl4dsurfaceeditor::SetCurrentFrame  { index } {
    $vtk_viewer SetCurrentFrame $index
}

itcl::body pxitcl4dsurfaceeditor::GetSurface { index } {
    return [ $splinestackcontrol GetSplineStack $index ]
}


itcl::body pxitcl4dsurfaceeditor::FrameChangedCallback { } {

    set fr [ $vtk_viewer GetCurrentFrame]
    $splinestackcontrol SetCurrentFrame $fr
    $tstackcontrol SetCurrentFrame $fr
    $abaquscontrol SetCurrentFrame $fr
    UpdateMasterFrame
}

itcl::body pxitcl4dsurfaceeditor::InitializeViewer {  } {

    if { $vtk_viewer ==0 } { return }

    set vframe [ frame $viewerbase.left ]
    set note $viewerbase.notebook
    iwidgets::tabnotebook $note -tabpos n
    pack $note -side right -expand f -fill y
    pack $vframe -side left -expand true -fill both

    set cframe [ $note add -label "Image" ] 
    set dframe [ $note add -label "Surface" ] 
    set eframe [ $note add -label "Multi" ] 
    set fframe [ $note add -label "Segment" ] 

    $note view "Surface"

    set vr [ $vtk_viewer Initialize $vframe $cframe 1 ]
    $splinestackcontrol SetNumberOfSplineStacks $numberofsurfaces
    set ct [ $splinestackcontrol Initialize $dframe $eframe $fframe 1 ]

    $vtk_viewer SetEnableFrameChangeCallback 1
    eval "$vtk_viewer SetFrameChangeCallback \"$this FrameChangedCallback\" "

    $splineeditor SetNumberOfSplines $numberofsurfaces

    set gui_splineeditor [ $splineeditor Initialize $viewerbase 0 ]
    wm title $gui_splineeditor "Spline Editor"
    wm geometry $gui_splineeditor 650x650

    eval "$splineeditor SetSliceChangeCallback \"$this UpdateMasterFrame\" "

    set cn [ $vtk_viewer InitializeMovieControl $basewidget 0 ]
    eval "bind all <Control-m> { wm deiconify $cn} "

    SetControlsViewer $vtk_viewer $splinestackcontrol
    $splinestackcontrol SetBaseViewer $vtk_viewer
}

# ---------------------------------------------------------------------------------
#  Initialize Display Stuff
# ---------------------------------------------------------------------------------

itcl::body pxitcl4dsurfaceeditor::InitializeDisplay { widg st } {
    
    if { $initialized == 1 } { return $basewidget }

    ::pxitclbase4dimageviewer::InitializeDisplay $widg $st
    $tstackcontrol SetNumberOfTriangulatedStacks $numberofsurfaces


    #    menubutton $menubase.edit -text "Edit Surface"   -menu $menubase.edit.m -underline 0
    set mb [ menu $menubase.editm -tearoff 0 ]
    #    pack $menubase.edit -side left
    $menubase add cascade -label "Edit Surface" -menu $mb -underline 0

    eval "$mb add command -label \"Spline Editor \" -command {  wm deiconify $gui_splineeditor }"
    $mb add separator
    eval "$mb add command -label \"Cache All Frames\" -command { $this CacheAllFrames }"

    eval "bind all <Control-e> { wm deiconify $gui_splineeditor }"

    set masterframe [ frame $basewidget.[pxvtable::vnewobj ] -relief raised]
    set masterframe2 [ frame $basewidget.[pxvtable::vnewobj ] ]
    set masterframe3 [ frame $basewidget.[pxvtable::vnewobj ] ]
    $masterframe2 configure -height 2 -bg black
    $masterframe3 configure -height 2 -bg black
    pack $masterframe3 $masterframe $masterframe2 -side bottom -expand f -fill x
    CreateMasterFrame $masterframe

    eval "button $masterframe.edit  -text Edit -command  {  wm deiconify $gui_splineeditor }"
    pack $masterframe.edit -side right -padx 2

    wm geometry $basewidget 800x605
    CreateHelpMenu
}


itcl::body pxitcl4dsurfaceeditor::CacheAllFrames { } {

    set nf [ $this GetNumberOfFrames ]
    WatchOn
    for { set i 0 } { $i < $nf } { incr i } {
	flush stdout
	$this SetCurrentFrame $i
	ShowProgressVal "Initializing Frame [ expr $i+1 ] / $nf" [  expr ($i +1.0 )/$nf ]
    }
    ShowProgressVal "Done" 1.0
    WatchOff
}

itcl::body pxitcl4dsurfaceeditor::UpdateMasterFrame {  } {

    set thisparam($this,gui_currentframe) [ expr [  $vtk_viewer GetCurrentFrame ] +1 ]
    set thisparam($this,gui_currentslice) [ $splineeditor GetBeginSlice ]
    set coord [ $vtk_viewer GetLastClickedPoint ]
    $vtk_viewer SetCoordinates [ expr int([ lindex $coord 0 ]) ] [ expr int([ lindex $coord 1 ])] $thisparam($this,gui_currentslice)  0

}

itcl::body pxitcl4dsurfaceeditor::UpdateFromMasterFrame { mode  } {

    if { $mode == 1 } {
	$vtk_viewer SetCurrentFrame [ expr $thisparam($this,gui_currentframe) -1 ]
	return
    } 
    
    if { $mode == 2 } {
	$splineeditor SetEnableSliceChangeCallback 0
	$splineeditor SetSlice  $thisparam($this,gui_currentslice) 
	$splineeditor SetEnableSliceChangeCallback 1
    }

}

itcl::body pxitcl4dsurfaceeditor::CreateMasterFrame { w } {

    label $w.0 -text "Frame:"
    set thisparam($this,gui_masterframe)  [ scale $w.s0 -orient horizontal -resolution 1 -showvalue 1 -variable [ itcl::scope thisparam($this,gui_currentframe) ]  ]

    label $w.1 -text "Slice:"
    set thisparam($this,gui_masterslice)  [ scale $w.s1 -orient horizontal -resolution 1 -showvalue 1 -variable [ itcl::scope thisparam($this,gui_currentslice) ]  ]

    pack $w.0 $w.s0 $w.1 $w.s1 -side left -expand f -padx 2

    eval "bind $thisparam($this,gui_masterframe) <ButtonRelease> \"$this UpdateFromMasterFrame 1\" "
    eval "bind $thisparam($this,gui_masterslice) <ButtonRelease> \"$this UpdateFromMasterFrame 2\" "
    

}


# ---------------------------------------------------------------------

if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    wm withdraw .    
    set imp [ pxitcl4dsurfaceeditor \#auto 0 ] 
    $imp InitializeDisplay .a 1
    $imp ShowWindow
    if { $argc > 0 } {  
	set ok [ $imp LoadImage [lindex $argv 0]  ] ; 
	if { $ok > 0 } {
	    # $imp CacheAllFrames
	}
    }

    [ $imp GetViewer ] SetEnableRendering 1
    [ $imp GetViewer ] UpdateDisplay
}

