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

if { $argc < 3} {
    set name [file tail [info script]]
    puts stdout "\n Usage:"
    puts stdout "\n $name <input_image\[.hdr\]> <\[-frame\] | \[-slice\]> <number of output frames> \[out_name\]"
    puts stdout "       -frame: input interleaved by frame"
    puts stdout "       -slice: input interleaved by slice"
    exit
}

set filename1 [lindex $argv 0]
set interleave [lindex $argv 1]
set number [lindex $argv 2]
set fname "[ file rootname $filename1 ]"
set outname "${fname}_inter"

if { $argc >= 4 } {
    set outname [lindex $argv 3]
}

puts stdout "Reading in image [lindex $argv 0]"
vtkpxAnalyzeImageSource ana1
ana1 Load $filename1

vtkmpjImageInterleaving inter
inter SetInput [ ana1 GetOutput ]

if { $interleave == "-frame" } {
    inter SetFrameInterleave 
} else {
    inter SetSliceInterleave
}

inter SetNumberOfFrames $number
inter Update

puts stdout "Saving resulting image..."
vtkpxAnalyzeImageWriter out
out SetInput [inter GetOutput]
out SetOrientation [ana1 GetOrientation]
out Save $outname

puts stdout "Done!\n"

ana1 Delete
inter Delete

exit


