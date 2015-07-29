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





# 	$Id: pxitclsplineeditor.tcl,v 1.2 2004/12/16 16:17:39 xenios Exp xenios $	
lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]

package provide pxitclsplineeditor 1.0

# -----------------------
# Dependencies and path
# -----------------------
package require pxtclutil 1.0
package require vtkpxcontrib  1.1
package require pxitclbaseimageviewer 1.0
package require pxitclsplineobjectmap 1.0

# -----------------------------------------------------
#  Intialize Variables
# -----------------------------------------------------
itcl::class pxitclsplineeditor {

    inherit pxitclbaseimageviewer 

    public    variable numberofsplines    5
    protected variable objectmapcontrol   0 
    public    variable gui_objectmapcontrol   0

    constructor { par } {
	::pxitclbaseimageviewer::constructor $par
    } {
	InitializeSplineEditor
    }
    
    private method InitializeSplineEditor { }
    public method SetImageFromObject { this obj } 
    public method GetSpline { index } 
    public method GetCurrentSlice { } 
    
    # Display Initialization 
    public    method InitializeDisplay { widg standalone } 
}
# -----------------------------------------------------
itcl::body pxitclsplineeditor::InitializeSplineEditor { } {

    set appname "Spline Editor"

    set vtk_viewer  [ vtkpxGUISplineEditor [ pxvtable::vnewobj ]]
    set objectmapcontrol  [ [ pxitclsplineobjectmap \#auto $this ] GetThisPointer ]

    set enable_headercontrol   1
    set enable_landmarkcontrol 0
    set enable_polydatacontrol 0
    set enable_overlaytool     0
}
    
# ---------------------------------------------------------------------------------
#  GUI Stuff 
# ---------------------------------------------------------------------------------
itcl::body pxitclsplineeditor::GetSpline { index } {
    return [ [ $vtk_viewer GetSplineControl ] GetSpline $index ]
}

itcl::body pxitclsplineeditor::GetCurrentSlice { } {
    return [  $vtk_viewer GetBeginSlice ]
}


itcl::body pxitclsplineeditor::SetImageFromObject { img obj } {

    set a [ pxitclbaseimageviewer::SetImageFromObject $img $obj ]
    if { $a==0 } { return 0 }
    
    if { $vtk_viewer != 0 } {
	$vtk_viewer SetImage [ $currentimage GetImage ] [ $currentimage GetOrientation ] 
	$vtk_viewer SetSlice -1
    }
	
}
    
# ---------------------------------------------------------------------------------
#  Initialize Display Stuff
# ---------------------------------------------------------------------------------

itcl::body pxitclsplineeditor::InitializeDisplay { widg st } {
    
    if { $initialized == 1 } { return $basewidget }

    $vtk_viewer SetNumberOfSplines $numberofsplines
    set w [ InitializeInterface $widg $st ]
    wm geometry $basewidget 800x850

    set gui_objectmapcontrol [ $objectmapcontrol Initialize $widg.[pxvtable::vnewobj ] $this ]

    #    menubutton $menubase.edit -text "Object Map"   -menu $menubase.edit.m -underline 0
    set mb [ menu $menubase.editm -tearoff 0 ]
    $menubase add cascade -label "Object Map" -menu $mb

    #   pack $menubase.edit -side left
    eval "$mb add command -label \"Object Map Control \" -command {  wm deiconify $gui_objectmapcontrol }"
    #    $mb add separator
    return $w
}

# ---------------------------------------------------------------------

if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    wm withdraw .    
    set imp [ pxitclsplineeditor \#auto 0 ] 
    $imp InitializeDisplay .a 1
    $imp ShowWindow
    if { $argc > 0 } {  set ok [ $imp LoadImage [lindex $argv 0]  ] }
}

