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


if { $argc < 4} {
    set name [file tail [info script]]
    puts stdout "\n Usage:"
    puts stdout "\n $name <input_image> <echo1> <echo2> <thresh> \[-w\]"
    puts stdout "\n where <input_image> is the Analyze/NIFTI file containing the images acquired at the two echo times"
    puts stdout "       <echo1> is the echo time for first image"
    puts stdout "       <echo2> is the echo time for second image"
    puts stdout "       <thresh> is the value for thresholding anatomy"
    puts stdout "       -w: outputs the two echo images\n"
    puts stdout "e.g. $name /images/example1.hdr 30 80 40\n"
    exit
}

set filename1 [lindex $argv 0]
set TE1 [lindex $argv 1]
set TE2 [lindex $argv 2]
set thresh [lindex $argv 3]
set write_echos 0
set thresh 40

if { $argc > 4 } {
    set write_echos 1
}

puts stdout "Reading in image [lindex $argv 0]"
puts stdout "Threshold value is $thresh"
vtkpxAnalyzeImageSource ana1
ana1 Load $filename1

vtkmpjImageInterleaving inter
inter SetInput [ ana1 GetOutput ]
inter SetSliceInterleave 
inter SetNumberOfFrames 2
inter Update

vtkImageShiftScale shsc
shsc SetShift 0.1
shsc SetOutputScalarTypeToFloat
shsc SetInput [inter GetOutput]
shsc Update

puts stdout "Calculating T2 times image"
vtkmpjImageT2Relax t2relax
t2relax SetInput [shsc GetOutput]
t2relax SetThresholdValue $thresh
t2relax SetEchoTime1 $TE1
t2relax SetEchoTime2 $TE2
t2relax Update

vtkpxAnalyzeImageWriter out
out SetInput [t2relax GetOutput]
out SetImageHeader [ana1 GetImageHeader]
set fname "[ file rootname $filename1 ]"
set fname0 "${fname}_t2t"
puts "Saving $fname0..."
out Save $fname0

if { $write_echos } {

    vtkImageExtractComponents comp1
    comp1 SetInput [ inter GetOutput ]
    comp1 SetComponents 0
    comp1 Update
    
    vtkpxAnalyzeImageWriter out1
    out1 SetInput [comp1 GetOutput]
    out1 SetImageHeader [ana1 GetImageHeader]
    set fname1 "${fname}_$TE1"
    puts stdout "Saving $fname1..."
    out1 Save $fname1
    
    comp1 Delete
    out1 Delete
    
    vtkImageExtractComponents comp2
    comp2 SetInput [ inter GetOutput ]
    comp2 SetComponents 1
    comp2 Update
    
    vtkpxAnalyzeImageWriter out2
    out2 SetInput [comp2 GetOutput]
    out2 SetImageHeader [ana1 GetImageHeader]
    set fname2 "${fname}_$TE2"
    puts "Saving $fname2..."
    out2 Save $fname2
    
    comp2 Delete
    out2 Delete
}

puts stdout "Done!\n"

ana1 Delete
shsc Delete
inter Delete
t2relax Delete

exit


