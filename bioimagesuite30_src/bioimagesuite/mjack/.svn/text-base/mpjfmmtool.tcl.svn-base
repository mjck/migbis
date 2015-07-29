#!/bin/sh
# the next line restarts using wish \
    exec vtk "$0" "$@"


#BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
#BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
#BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
#BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
#BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
#BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------



# 	$Id: pxitclbrainsegment.tcl,v 1.2 2004/12/16 16:10:43 xenios Exp xenios $	

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] main ]

package require vtkpxcontrib  1.1
package require vtkmpjcontrib  1.1
package require pxitclbaseimageviewer 1.0
package require mpjfastmarching 1.2

option add *textBackground seashell

wm withdraw .

set breg [ pxitclbaseimageviewer \#auto 0 ]
$breg configure -appname "BioImage Suite::FMM Tool"
$breg configure -show_standard_images 1
$breg configure -enable_helpmenu 0
$breg configure -enable_multisubjectcontrol 0
$breg configure -enable_overlaytool 0
$breg configure -enable_talaraichoption 1
$breg configure -enable_rendering_on_startup  0
$breg InitializeDisplay .[pxvtable::vnewobj ] 1

set fmmutil [ mpjfastmarching \#auto $breg ]
$fmmutil Initialize [ $breg GetBaseWidget ].[ pxvtable::vnewobj ]

set menubase [ $breg cget -menubase ]
set mb [ menu $menubase.fmmm -tearoff 0  ]
$menubase add cascade  -label "Fast Marching" -menu $menubase.fmmm -underline 0

$fmmutil AddToMenuButton $mb

$breg AddControl $fmmutil

$breg CreateHelpMenu
$breg ShowWindow   

set argc [llength $argv]

if { $argc > 0 } {   $breg LoadImage [lindex $argv 0]   }
if { [ $breg cget -enable_overlaytool ] == 1 } {
    if { $argc > 1 } {  [ $breg cget -overlaytool ] LoadFunctionalImage [lindex $argv 1] }
    if { $argc > 2 } {   [ $breg cget -overlaytool ] LoadTransformation [lindex $argv 2] }
}

update idletasks
[ $breg GetViewer ] SetEnableRendering 1

#[ $breg GetViewer ] UpdateDisplay





