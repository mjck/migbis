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





# 	$Id: pxitclvolumeviewer.tcl,v 1.1 2003/05/28 16:43:26 papad Exp xenios $	

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]

package require vtkpxcontrib  1.1
package require pxtcluserprefs 1.0
package require pxitclbase4dimageviewer 1.0

wm withdraw .
set breg [ pxitclbase4dimageviewer \#auto 0 ]
$breg configure -appname "BioImage Suite::VolumeViewer"
set w [ $breg InitializeDisplay .[pxvtable::vnewobj ] 1 ]
wm geometry [ $breg GetBaseWidget ] 650x600

$breg CreateHelpMenu 

set argc [llength $argv]

if { $argc > 0 } {   $breg LoadImage [lindex $argv 0]   }
if { $argc >1  } {  [ $breg cget -abaquscontrol ] LoadStudy [ lindex $argv 1 ]}


$breg ShowWindow

if { $argc > 2 } {
    
    set ortho [ $breg GetViewer ]
    set numframes [ $ortho GetNumberOfFrames ]
    set name [ lindex $argv 2 ]

    for { set i 0 } { $i < $numframes } { incr i } {

	set j [ expr $i +1 ]
	set sphere [ vtkPolyDataReader read_$i ]
	$sphere SetFileName "${name}_${j}.vtk"
	$sphere Update

	puts stdout "\tRead [ $sphere GetFileName ] points = [ [ $sphere GetOutput ] GetNumberOfPoints ]"
	
	set mapper [ vtkPolyDataMapper map_$i ]
	$mapper SetInput [ $sphere GetOutput ]
	
	set act [ vtkActor act_$i ]
	$act SetMapper $mapper
	[ $act GetProperty ] SetColor 0.8 0.2 0.2 
	
	$ortho SetCurrentFrame $i
	set ren [ [ $ortho GetRenderer 3 ] GetRenderer ]
	$ren AddActor $act
	
	$mapper Delete
	$sphere Delete
	$act Delete
    }
    
    $ortho SetCurrentFrame $i
    $ortho SetDisplayMode3D
}

update idletasks
[ $breg GetViewer ] SetEnableRendering 1
[ $breg GetViewer ] UpdateDisplay


