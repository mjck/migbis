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



# -----------------------
# Dependencies and path
# -----------------------
lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]

#package require pxtclutil 1.0
package require vtkpxcontrib  1.1
package require vtkmpjcontrib 1.1
package require pxvtable 1.0

if { $argc < 1 } {
    set name [file tail [info script]]
    puts stdout "\n Usage:"
    puts stdout "\n $name <input_image\[.hdr\]>"
    exit
}

set filename [lindex $argv 0]

vtkpxAnalyzeImageSource ana1
ana1 Load $filename

set spacing [ lindex [ [ ana1 GetOutput ] GetSpacing ] 0 ]
#set spacing 1.0

set dim [ [ ana1 GetOutput ] GetDimensions ]
set dimx [ lindex $dim 0 ]
set dimy [ lindex $dim 1 ]
set dimz [ lindex $dim 2 ]
##puts "Image dimensions: [ lindex $dim 0 ] [ lindex $dim 1 ] [ lindex $dim 2 ]"

set image [ ana1 GetOutput ]
for { set i 0} { $i < [expr $dimx - 1] } { incr i } {
    for { set j 0} { $j < [expr $dimy - 1]} { incr j } {
	for { set k 0} { $k < [expr $dimz - 1]} { incr k } {
	    set value [ $image GetScalarComponentAsDouble $i $j $k 0 ]
	    if { $value > 0 } {
		    puts "$i $j $k 0.0"
	    }
	}
    }
}

ana1 Delete

exit


