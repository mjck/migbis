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
    puts stdout "\n $name <input_image\[.hdr\]> x y z"
    puts stdout "       x y z: seed point"
    exit
}

set filename [lindex $argv 0]
set x [ lindex $argv 1 ]
set y [ lindex $argv 2 ]
set z [ lindex $argv 3 ]

puts stdout "Reading in image [lindex $argv 0]"
vtkpxAnalyzeImageWriter anaw

vtkpxAnalyzeImageSource ana1
ana1 Load $filename

set spacing [ lindex [ [ ana1 GetOutput ] GetSpacing ] 0 ]
set spacing 1.0

vtkFloatArray bndry
bndry SetNumberOfComponents 4
bndry InsertNextTuple4 $x $y $z 0.0

puts "seed point $x $y $z"

vtkmpjFastMarchingMethod fmm1
fmm1 SetKnownPoints bndry
fmm1 SetSpeedImage [ ana1 GetOutput ]
fmm1 SetPropagationTime 20
fmm1 SetSpacing $spacing
fmm1 Update


anaw SetInput [ fmm1 GetOutput ]
anaw SetOrientation [ana1 GetOrientation]
set fnamew "[ file rootname $filename ]"
set fnamew "${fnamew}_fmm1"
puts stdout "Saving fmm1 into $fnamew"
anaw Save $fnamew


puts "Range1: [ [ [ [fmm1 GetOutput] GetPointData] GetScalars ] GetRange]"

vtkImageCast imcast
imcast SetInput [ fmm1 GetLabelImage ]
imcast SetOutputScalarTypeToFloat
imcast Update

vtkImageData cspd
cspd CopyStructure [ ana1 GetOutput ]
cspd AllocateScalars
[ [ cspd GetPointData ] GetScalars ] FillComponent 0 1.0

puts stdout "Into Fast Marching Method"

vtkmpjFastMarchingMethod fmm2
fmm2 KnownValuesOff
fmm2 SetKnownPoints [ fmm1 GetFrontPoints ]
fmm2 SetSpeedImage cspd
fmm2 SetSpacing $spacing
fmm2 Update

puts "dims2: [ fmm2 GetOutput ] GetDimensions ]"
vtkImageMathematics math
math SetInput1 [ fmm2 GetOutput ]
math SetInput2 [ imcast GetOutput ]
math SetOperationToMultiply
math Update

anaw SetInput [ math GetOutput ]
anaw SetOrientation [ana1 GetOrientation]
set fnamew "[ file rootname $filename ]"
set fnamew "${fnamew}_fmm1_math"
puts stdout "Saving fmm1 into $fnamew"
anaw Save $fnamew

vtkmpjLevelSetNarrowBandFilter nbf
nbf SetInput [ math GetOutput ]
nbf Update

puts "Range: [ [ [ [fmm2 GetOutput] GetPointData] GetScalars ] GetRange]"

vtkImageThreshold thresh
thresh SetInput [ nbf GetOutput ]
thresh SetOutputScalarTypeToFloat
thresh ThresholdByLower 1000
thresh SetOutValue 1000
thresh Update

puts stdout "Saving resulting image..."
vtkpxAnalyzeImageWriter out
out SetInput [ thresh GetOutput ]
out SetOrientation [ana1 GetOrientation]
set fname "[ file rootname $filename ]"
set fname "${fname}_narrow"
out Save $fname

vtkpxAnalyzeImageWriter out1
out1 SetInput [ fmm1 GetLabelImage ]
out1 SetOrientation [ana1 GetOrientation]
set fname "[ file rootname $filename ]"
set fname "${fname}_fmm_mask"
out1 Save $fname
puts stdout "Done!\n"

ana1 Delete
out Delete
out1 Delete
fmm1 Delete
fmm2 Delete
#spd Delete
bndry Delete
thresh Delete

exit


