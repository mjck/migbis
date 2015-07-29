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
    exit
}

set filename1 [lindex $argv 0]

puts stdout "Reading in phi image [lindex $argv 0]"

vtkpxAnalyzeImageSource ana1
ana1 Load $filename1

## extract 0th level set ##
vtkmpjExtractLevelSet extr
extr SetInput [ ana1 GetOutput ]
extr SetLevelSetValue 0.0
extr Update

## get mask for inside points ##
vtkImageThreshold thresh1
thresh1 SetInput [ extr GetOutput ]
thresh1 SetOutputScalarTypeToFloat
thresh1 ThresholdByLower -1.0
thresh1 SetInValue 1.0
thresh1 SetOutValue 0.0
thresh1 Update

## get distance from inside points ##
vtkmpjFastMarchingMethod fmm1
fmm1 SetInput [ thresh1 GetOutput ]
fmm1 SetKnownPoints [ extr GetInsidePoints ]
fmm1 Update

## get mask for outside points ##
vtkImageThreshold thresh2
thresh2 SetInput [ extr GetOutput ]
thresh2 SetOutputScalarTypeToFloat
thresh2 ThresholdByUpper 1.0
thresh2 SetInValue 1.0
thresh2 SetOutValue 0.0
thresh2 Update

## get distance from outside points ##
vtkmpjFastMarchingMethod fmm2
fmm2 SetInput [ thresh2 GetOutput ]
fmm2 SetKnownPoints [ extr GetOutsidePoints ]
fmm2 Update

vtkImageThreshold thresh3
thresh3 SetInput [ fmm1 GetOutput ]
thresh3 SetOutputScalarTypeToFloat
thresh3 ThresholdByLower 1000
thresh3 SetOutValue 0
thresh3 Update

vtkImageThreshold thresh4
thresh4 SetInput [ fmm2 GetOutput ]
thresh4 SetOutputScalarTypeToFloat
thresh4 ThresholdByLower 1000
thresh4 SetOutValue 0
thresh4 Update

## combine inner and outer distance maps ##
vtkImageMathematics math1
math1 SetInput1 [ thresh3 GetOutput ]
math1 SetOperationToMultiplyByK
math1 SetConstantK -1.0
math1 Update

vtkImageMathematics math2
math2 SetInput1 [ math1 GetOutput ]
math2 SetInput2 [ thresh4 GetOutput ]
math2 SetOperationToAdd
math2 Update

puts stdout "Saving resulting image..."
vtkpxAnalyzeImageWriter out
out SetInput [ math2 GetOutput ]
out SetOrientation [ana1 GetOrientation]
set fname "[ file rootname $filename1 ]"
set fname "${fname}_dist"
out Save $fname

ana1 Delete
out Delete
extr Delete
fmm1 Delete
#fmm2 Delete
thresh1 Delete
#thresh3 Delete

exit


