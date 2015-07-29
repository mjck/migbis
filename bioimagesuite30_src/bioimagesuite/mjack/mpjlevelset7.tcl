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

if { $argc < 2 } {
    set name [file tail [info script]]
    puts stdout "\n Usage:"
    puts stdout "\n $name <input_phi\[.hdr\]> <speed\[.hdr\]>"
    exit
}

set filename1 [lindex $argv 0]
set filename2 [lindex $argv 1]

puts stdout "Reading in phi image [lindex $argv 0]"

vtkpxAnalyzeImageSource ana1
ana1 Load $filename1

puts stdout "Reading in speed image [lindex $argv 1]"

vtkpxAnalyzeImageSource ana2
ana2 Load $filename2

## extract signed distance function
vtkmpjLevelSetSignedDistance extr
extr SetInput [ ana1 GetOutput ]
extr SetExtensionImage [ ana2 GetOutput ]
extr Update

vtkpxAnalyzeImageWriter out
out SetInput [ extr GetOutput ]
out SetOrientation [ana1 GetOrientation]
set fname "[ file rootname $filename1 ]"
set fname "${fname}_dist1"
out Save $fname

extr Delete

exit


