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
    puts stdout "\n $name <input_phi\[.hdr\]>"
    puts stdout "\n Computes grad(phi)/|grad(phi)|, the normal to the levelset. \n"
    exit
}

set filename1 [lindex $argv 0]

puts stdout "Reading in phi image [lindex $argv 0]"

vtkpxAnalyzeImageSource ana1
ana1 Load $filename1

vtkmpjLevelSetNormal normal
normal SetInput [ ana1 GetOutput ]
normal Update

puts stdout "Saving resulting image..."
vtkpxAnalyzeImageWriter out
out SetInput [ normal GetOutput ]
out SetOrientation [ana1 GetOrientation]
set fname "[ file rootname $filename1 ]"
set fname "${fname}_normal"
out Save $fname

ana1 Delete
normal Delete
out Delete

exit


