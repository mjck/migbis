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
    puts stdout "Usage: $name <image.hdr> <lower-value> <upper-value> \[output_surface.vtk\]"
    exit
}

for each 
vtkpxAnalyzeImageSource ana1
ana1 Load [ lindex $argv 0 ]
set or1 [ ana1 GetOrientation ]
#set spc [ ana1 GetSpacing ]
#set org [ ana1 GetOrigin ]
puts stdout "Reading in image \#1 [ lindex $argv 0 ] [ [ ana1 GetOutput ] GetDimensions ] ori=[ ana1 GetOrientation]"



vtkThresholdPoints thresh
thresh SetInput [ ana1 GetOutput ]
thresh ThresholdBetween [ lindex $argv 1 ] [ lindex $argv 2 ]
thresh Update

puts "done thresholding input files..."

#vtkpxAnalyzeImageSource ana2
#ana2 Load [ lindex $argv 1 ]
#set or2 [ ana2 GetOrientation ]
#puts stdout "Reading in image \#2 [ lindex $argv 1 ] [ [ ana2 GetOutput ] GetDimensions ] ori=[ ana2 GetOrientation] "

vtkDelaunay3D delau
delau SetInput [ thresh GetOutput ]
delau SetTolerance 0.001
delau Update

puts "done triangulating input points..."

vtkGeometryFilter geom
geom SetInput [ delau GetOutput ]
geom Update

#vtkmpjPolyDataToImageFilter filter
#filter SetInput [ ana1 GetOutput ]
#filter SetPolyData [ poly GetPolyDataOutput ]
#filter SetPolyData [ poly GetOutput ]
#filter SetEpsilon [ lindex $argv 2]
#filter Update

vtkPolyDataWriter out
out SetInput [ geom GetOutput ]

set fname [lindex $argv 1]
if { $num <= 3 } {
    set foutname "[file rootname $fname]_map" 
} else {
    set foutname "[lindex $argv 3]"
}

out SetFileName $foutname

puts stdout "Writing surface $foutname..."
out Write


puts stdout "Done!\n"

out Delete
delau Delete
geom Delete
thresh Delete
ana1 Delete

exit
