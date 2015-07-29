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





# 	$Id: pxitclbaresimpleviewer.tcl,v 1.1 2003/09/03 18:17:05 xenios Exp xenios $	

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]

package require vtkpxcontrib  1.1
package require pxitclbaseimageviewer 1.0


wm withdraw .
set breg [ pxitclbaseimageviewer \#auto 0 ]
$breg configure -appname pxitclbaresimpleviewer
$breg configure -show_standard_images 0
$breg configure -enable_multisubjectcontrol 0
$breg configure -enable_rendering_on_startup  0
$breg configure -enable_headercontrol  0
$breg configure -enable_importcontrol  0
$breg configure -enable_imageutility  0
$breg configure -enable_landmarkcontrol  0
$breg configure -enable_overlaytool  0
$breg configure -enable_histcontrol  0
$breg configure -enable_displaymenu  0
$breg configure -enable_helpmenu  0
$breg InitializeMosaicViewer .[pxvtable::vnewobj ] 1

set argc [llength $argv]

if { $argc > 0 } {   $breg LoadImage [lindex $argv 0]   }
if { $argc > 1 } {  [ $breg cget -overlaytool ] LoadFunctionalImage [lindex $argv 1] }
if { $argc > 2 } {   [ $breg cget -overlaytool ] LoadTransformation [lindex $argv 2] }


set vr [ $breg GetViewer ]
$breg ShowWindow
$vr SetEnableRendering 1
#after 100 "$vr UpdateDisplay"




