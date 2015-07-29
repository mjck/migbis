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

if { $argc < 6 } {
    set name [file tail [info script]]
    puts stdout "\n Usage:"
    puts stdout "\n $name <cardiac_wave\[.hdr\] <cardiac_mask\[.hdr]> <eigenvector_image\[.hdr]> <angle_type> <eigenvector> <projection>"
    puts stdout "\n <angle_type>  \(1: \[0, 90\]; 2: \[-90, 90\]; 3: \[90, -90\]; 4: \[0, 180\])"
    puts stdout "\n <eigenvector> \(1: e1, 2: e2, 3: e3\)" 
    puts stdout "\n <projection>  \(1: Circumferential, 2: Radial, 3: Longitudinal\)"
    exit
}

## cardiac vector image
set filename1 [lindex $argv 0]

## eigenvector image
set filename2 [lindex $argv 1]

## eigenvector image
set filename3 [lindex $argv 2]

## angle type
set angle_type [lindex $argv 3]

## eigen to project
set eigen_type [lindex $argv 4]

## plane to project
set plane_type [lindex $argv 5]

puts "Loading cardiac propagation wave..."
vtkpxAnalyzeImageSource ana1
ana1 Load $filename1

puts "Loading cardiac mask..."
vtkpxAnalyzeImageSource ana2
ana2 Load $filename2

puts "Loading eigenvectors..."
vtkpxAnalyzeImageSource ana3
ana3 Load $filename3

puts "Calculating local cardiac coordinates..."
vtkmpjCardiacCoordinates coords
coords SetInput [ ana1 GetOutput ]
coords SetMask [ ana2 GetOutput ]
coords Update

puts "Calculating fiber angle..."
vtkmpjCardiacAngle fiberangle
fiberangle SetInput [ coords GetOutput ]
fiberangle SetMask [ ana2 GetOutput ]
fiberangle SetEigenvectors [ ana3 GetOutput ]
fiberangle SetEigenvectorType [expr $eigen_type - 1]
fiberangle SetAngleType [expr $angle_type - 1]
fiberangle SetReferencePlane [expr $plane_type - 1]
fiberangle Update

vtkpxAnalyzeImageWriter anaout
vtkpxAnalyzeImageWriter anaout2

anaout SetInput [ fiberangle GetOutput ]
anaout SetImageHeader [ana1 GetImageHeader]
anaout SetCompression 0
##ana2 SetNumFrames 1

if { $plane_type == 1 } {
   anaout Save "${filename1}_e${eigen_type}_circ_anglemap"
} elseif { $plane_type == 2 } {
   anaout Save "${filename1}_e${eigen_type}_rad_anglemap"
} else {
   anaout Save "${filename1}_e${eigen_type}_long_anglemap"
}

anaout Delete
ana3 Delete
ana2 Delete
ana1 Delete

exit