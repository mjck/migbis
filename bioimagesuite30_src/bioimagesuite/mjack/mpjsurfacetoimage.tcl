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

package require pxtclutil 1.0
package require vtkpxcontrib  1.1
package require vtkmpjcontrib 1.1
package require pxvtable 1.0

wm withdraw .
#puts stdout "Number of arguments = $argc"
set num $argc 

if { $num < 3 } {
    set name [info script]
    puts stdout "Usage: $name <background.hdr> <surface.vtk> <epsilon> \[output_map\] \[output_mask\]"
    exit
}

vtkpxAnalyzeImageSource ana1
ana1 Load [ lindex $argv 0 ]
set or1 [ ana1 GetOrientation ]
#set spc [ ana1 GetSpacing ]
#set org [ ana1 GetOrigin ]
puts stdout "Reading in image \#1 [ lindex $argv 0 ] [ [ ana1 GetOutput ] GetDimensions ] ori=[ ana1 GetOrientation]"

vtkGenericDataObjectReader poly
poly SetFileName [ lindex $argv 1 ]
poly Update

puts "done reading input files..."

vtkmpjPolyDataToImageFilter filter
filter SetInput [ ana1 GetOutput ]
filter SetPolyData [ poly GetPolyDataOutput ]
filter SetEpsilon [ lindex $argv 2]
filter Update

vtkpxAnalyzeImageWriter out
out SetInput [ filter GetOutput 0 ]
out SetOrientation $or1

set fname [lindex $argv 1]
if { $num <= 3 } {
    set foutname "[file rootname $fname]_map" 
} else {
    set foutname "[lindex $argv 3]"
}

puts stdout "Writing image map $foutname..."

out Save $foutname

vtkpxAnalyzeImageWriter out1
out1 SetInput [ filter GetOutput 1 ]
out1 SetOrientation $or1

set fname [lindex $argv 1]
if { $num <= 3 } {
    set foutname "[file rootname $fname]_mask" 
} else {
    set foutname "[lindex $argv 4]"
}

puts stdout "Writing image mask $foutname..."

out1 Save $foutname


puts stdout "Done!\n"

out Delete
out1 Delete
filter Delete
ana1 Delete
poly Delete

exit
