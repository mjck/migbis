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





# 	$Id: pxitclbrainsegment.tcl,v 1.2 2004/12/16 16:10:43 xenios Exp xenios $	

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]

package require vtkpxcontrib  1.1
package require pxtcluserprefs 
#package require vtkmpjcontrib  1.1
package require pxitclbaseimageviewer 1.0
#package require mpjdiffusionutility 1.0

wm withdraw .
set bsegm [ pxitclbaseimageviewer \#auto 0 ]
$bsegm configure -appname "BioImage Suite::Brain Segment"
$bsegm configure -show_standard_images 1
$bsegm configure -enable_multisubjectcontrol 0
$bsegm configure -enable_overlaytool 0
$bsegm configure -enable_vvlinkgadget  1
$bsegm configure -enable_talaraichoption 1
$bsegm configure -enable_rendering_on_startup  0
$bsegm configure -enable_internaleditor 1
$bsegm configure -enable_objectmapmenu   1
$bsegm InitializeObjectmapViewer .[pxvtable::vnewobj ] 1

$bsegm ShowWindow   

set argc [llength $argv]

if { $argc > 0 } {   $bsegm LoadImage [lindex $argv 0]   }


update idletasks
[ $bsegm GetViewer ] SetEnableRendering 1


#set ed [ [ $bsegm GetViewer ] GetInternalEditor ]
#if { $ed != "" } {
#    $ed SetVOISelectMode 254 259 1#
#
#    set lst [ list Black Red Green Blue ]##
#
#    for { set i 0 } { $i < [ llength $lst ] } { incr i } {
#	$ed SetVOIName $i [ lindex $lst $i ]
#    }
#}






