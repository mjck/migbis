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





package provide pxitclbase4dimageviewer 1.0

# 	$Id: pxitclbase4dimageviewer.tcl,v 1.2 2005/11/09 22:04:54 xenios Exp xenios $	

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]


package require vtkpxcontrib  1.1
package require pxitclbaseimageviewer 1.0
package require Itcl 
package require Iwidgets 
package require pxitclcreatesolidcontrol 1.0


# ----------------------------------------------------------------------------

itcl::class pxitclbase4dimageviewer {

    inherit pxitclbaseimageviewer

    protected variable multisplinestackcontrol 0
    public    variable enable_multisplinestackcontrol 0

    protected variable tstackcontrol 0 
    public    variable enable_tstackcontrol 1

    public    variable abaquscontrol 0 
    public    variable enable_abaquscontrol 1 

    public    variable createsolidcontrol 0
    public    variable enable_createsolidcontrol 1


    public    variable numberofsurfaces   5

    constructor { par } {
	::pxitclbaseimageviewer::constructor $par
    } {
	InitializeBase4dImageViewer
    }

    protected method InitializeBase4dImageViewer { } 

    protected method Create4DControls { cardiacmenu } 
    protected method SetControlsViewer { viewer control } 

    public method InitializeDisplay { widget standalone}
    public method SetImageFromObject { img obj } 

    public    method GetTstack        { index } 
    public    method GetViewer { }
}

# ----------------------------------------------------------------------------

itcl::body pxitclbase4dimageviewer::InitializeBase4dImageViewer { } {

    set appname "volume viewer"
    set enable_landmarkcontrol 1
    set enable_surfaceobjectmap 0
    set enable_multisubjectcontrol 0
    set enable_talairachoption 0
    set enable_landmarkclicks  0
    set enable_overlaytool 0
    set show_standard_images 0

}

# ---------------------------------------------------------------------------------
itcl::body pxitclbase4dimageviewer::SetImageFromObject { img obj } {

    set a [ pxitclbaseimageviewer::SetImageFromObject $img $obj ]
    if { $a==0 } { return 0 }
    
    if { $multisplinestackcontrol != 0 } {
	$multisplinestackcontrol SetImage [ $currentimage GetImage ] [ $currentimage GetOrientation ] 0 0
    }

    if { $tstackcontrol != 0 } {
	$tstackcontrol SetImage [ $currentimage GetImage ] [ $currentimage GetOrientation ] 0 0
    }

    if { $abaquscontrol != 0 } {
	$abaquscontrol SetImage [ $currentimage GetImage ] [ $currentimage GetOrientation ] 
    }

	
}
# ---------------------------------------------------------------------------------

itcl::body pxitclbase4dimageviewer::GetTstack { index } {
    if { $tstackcontrol != 0 } {
	return [ $tstackcontrol GetTriangulatedStack $index ] 
    }

    puts stderr "Error in GetTstack\n"
    return 0
}

itcl::body pxitclbase4dimageviewer::GetViewer  { } {
    return $vtk_viewer 
}

# ---------------------------------------------------------------------------------
itcl::body pxitclbase4dimageviewer::Create4DControls { cardiacmenu } {

    if { $enable_multisplinestackcontrol == 1 } {

	set tnote [ toplevel $basewidget.[ pxvtable::vnewobj ] ]
	eval "wm protocol $tnote WM_DELETE_WINDOW { wm withdraw $tnote  }"
	wm title $tnote "Spline Surface Control"
	wm withdraw $tnote
	
	set note $tnote.notebook

	iwidgets::tabnotebook $note -tabpos n
	pack $note -side top -expand t -fill both

	set dframe [ $note add -label "Surface" ] 
	set eframe [ $note add -label "Multi" ] 
	set fframe [ $note add -label "Segment" ] 
	$note view "Surface"
    
	set multisplinestackcontrol [ vtkpxGUI4DSplineStackControl [ pxvtable::vnewobj ]]
	$multisplinestackcontrol Initialize $dframe $eframe $fframe 1
	SetControlsViewer $vtk_viewer $multisplinestackcontrol 
	eval "$cardiacmenu add command -label \"Spline Surface Control\" -command { wm deiconify $tnote }"
    }

    if { $enable_tstackcontrol == 1 } {
	
	set tstackcontrol [ vtkpxGUI4DTriangulatedStackControl [ pxvtable::vnewobj ]]
	
	#set t [ toplevel $basewidget.[ pxvtable::vnewobj ] ] ; wm withdraw $t
	$tstackcontrol SetCallback $tstackcontrol -1
	set t [ $tstackcontrol Initialize $basewidget 0  ]
	eval "wm protocol $t WM_DELETE_WINDOW { wm withdraw $t  }"
	wm title $t "T-stack Control"
	SetControlsViewer $vtk_viewer $tstackcontrol 
	eval "$cardiacmenu add command -label \"Tstack Control\" -command { wm deiconify $t }"

	if { $enable_createsolidcontrol == 1 } {
	    set createsolidcontrol  [ [ pxitclcreatesolidcontrol \#auto $this ] GetThisPointer ]
	    set t2 [ $createsolidcontrol Initialize $basewidget.[pxvtable::vnewobj] $this ]
	    eval "wm protocol $t2 WM_DELETE_WINDOW { wm withdraw $t2  }"
	    wm title $t2 "Create-Solid Control"
	    eval "button $t.bbot -text \"(Create) Solid Control\" -command {  wm deiconify $t2 }"
	    pack $t.bbot -side bottom -expand f -fill x -pady 2
	}
    }

    if { $enable_abaquscontrol == 1 } {

	set abaquscontrol [ vtkpxGUIFemPostControl [ pxvtable::vnewobj ]]
	$abaquscontrol SetCallback $abaquscontrol -1
	set t [ $abaquscontrol Initialize $basewidget 0 ]	
	eval "wm protocol $t WM_DELETE_WINDOW { wm withdraw $t  }"
	wm title $t "Abaqus post Control"
	SetControlsViewer $vtk_viewer $abaquscontrol 
	eval "$cardiacmenu add command -label \"Abaqus Post Control\" -command { wm deiconify $t }"
    }
}
# ---------------------------------------------------------------------------------
itcl::body pxitclbase4dimageviewer::SetControlsViewer { viewer control }  {


    $control SetRenderer [ $viewer GetMultiRenderer 3 ] 0 0
    $control SetRenderer [ $viewer GetMultiRenderer 0 ] 1 1
    $control SetRenderer [ $viewer GetMultiRenderer 1 ] 2 1
    $control SetRenderer [ $viewer GetMultiRenderer 2 ] 3 1


}
# ---------------------------------------------------------------------------------
itcl::body pxitclbase4dimageviewer::InitializeDisplay { widget st } {
    
    set	oldhelpmenu $enable_helpmenu
    set ok [ InitializeOrthogonal4DViewer $widget $st ]

    if { $ok == 0 } {
	return 0
    }

    set mb [ menu $menubase.cardiacm -tearoff 0 ]
    $menubase add cascade -label "Cardiac" -menu $mb -underline 0

    Create4DControls $mb 
    set enable_helpmenu $oldhelpmenu 
    return $ok

}
# ---------------------------------------------------------------------------------


