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
package require mpjtensorutility 1.2
package require mpjmultibtensorutility 1.3
package require mpjtensoranalysis 1.2
package require mpjfibertracking 1.2
package require mpjsheettracking 1.0
package require mpjfibertrackingold 1.2
package require mpjcardiacanalysis 1.2
package require mpjconnectivity 1.0 
#option add *textBackground seashell

wm withdraw .

set breg [ pxitclbaseimageviewer \#auto 0 ]
$breg configure -appname "BioImage Suite::Diffusion Tool"
$breg configure -show_standard_images 1
$breg configure -enable_helpmenu 0
$breg configure -enable_multisubjectcontrol 0
$breg configure -enable_overlaytool 0
$breg configure -enable_vvlinkgadget  1
$breg configure -enable_talaraichoption 1
$breg configure -enable_rendering_on_startup  0
$breg InitializeObjectmapViewer .[pxvtable::vnewobj ] 1

set menubase [ $breg cget -menubase ]
set mb [ menu $menubase.diffusionm -tearoff 0  ]
$menubase add cascade  -label "Diffusion" -menu $menubase.diffusionm -underline 4
$mb add command -label "Tensor utility" -command { mpjtensorutility_new $breg }
$mb add command -label "Multi-b tensor utility" -command { mpjmultibtensorutility_new $breg }
$mb add command -label "Tensor analysis" -command { mpjtensoranalysis_new $breg }
$mb add command -label "Fiber tracking (New)" -command { mpjfibertracking_new $breg }
$mb add command -label "Fiber tracking (Old)" -command { mpjfibertrackingold_new $breg }
$mb add command -label "Sheet tracking" -command { mpjsheettracking_new $breg }
$mb add command -label "Cardiac analysis" -command { mpjcardiacanalysis_new $breg }
$mb add command -label "Connectivity analysis" -command { mpjconnectivity_new $breg }

global pxtcl_pref_array

$breg CreateHelpMenu
$breg ShowWindow   

[ $breg GetViewer ] SetEnableRendering 1
[ $breg GetViewer ] SetDisplayMode 2

update idletasks

proc mpjtensorutility_new { breg } {

    set tensorutil [ mpjtensorutility \#auto $breg ]
    $tensorutil Initialize [ $breg GetBaseWidget ].[ pxvtable::vnewobj ]
    $breg AddControl $tensorutil
    $tensorutil ShowWindow

}

proc mpjmultibtensorutility_new { breg } {

    set multibtensorutil [ mpjmultibtensorutility \#auto $breg ]
    $multibtensorutil Initialize [ $breg GetBaseWidget ].[ pxvtable::vnewobj ]
    $breg AddControl $multibtensorutil
    $multibtensorutil ShowWindow

}

proc mpjtensoranalysis_new { breg } {

    set tensoranal [ mpjtensoranalysis \#auto $breg ]
    $tensoranal Initialize [ $breg GetBaseWidget ].[ pxvtable::vnewobj ]
    $breg AddControl $tensoranal
    $tensoranal ShowWindow

}

proc mpjfibertracking_new { breg } {

    set fibertrack [ mpjfibertracking \#auto $breg ]
    $fibertrack Initialize [ $breg GetBaseWidget ].[ pxvtable::vnewobj ]
    $breg AddControl $fibertrack
    $fibertrack ShowWindow

}

proc mpjfibertrackingold_new { breg } {

    set fibertrackold [ mpjfibertrackingold \#auto $breg ]
    $fibertrackold Initialize [ $breg GetBaseWidget ].[ pxvtable::vnewobj ]
    $breg AddControl $fibertrackold
    $fibertrackold ShowWindow

}

proc mpjsheettracking_new { breg } {
    
    set sheettrack [ mpjsheettracking \#auto $breg ]
    $sheettrack Initialize [ $breg GetBaseWidget ].[ pxvtable::vnewobj ]
    $breg AddControl $sheettrack
    $sheettrack ShowWindow
    
}

proc mpjcardiacanalysis_new { breg } {

    set cardiacanal [ mpjcardiacanalysis \#auto $breg ]
    $cardiacanal Initialize [ $breg GetBaseWidget ].[ pxvtable::vnewobj ]
    $breg AddControl $cardiacanal
    $cardiacanal ShowWindow

}

proc mpjconnectivity_new { breg } {

    set connectanal [ mpjconnectivity \#auto $breg ]
    $connectanal Initialize [ $breg GetBaseWidget ].[ pxvtable::vnewobj ]
    $breg AddControl $connectanal
    $connectanal ShowWindow

}
