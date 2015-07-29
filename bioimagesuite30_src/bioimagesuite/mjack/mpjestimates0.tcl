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

if { $argc < 5} {
    set name [file tail [info script]]
    puts stdout "\n Usage:"
    puts stdout "\n $name <input_file\[.hdr\]> <low-b> <index-low-b> <high-b> <index-high-b>"
    exit
}

set filename [ lindex $argv 0 ]
set lowb [lindex $argv 1]
set indexlowb [ lindex $argv 2]
set highb [lindex $argv 3]
set indexhighb [ lindex $argv 4]

vtkpxAnalyzeImageSource ana1
ana1 Load $filename

set origin [ [ ana1 GetOutput ] GetOrigin ]
set spacing [ [ ana1 GetOutput ] GetSpacing ]

vtkmpjEstimateDiffusionS0 s0
s0 SetInput [ ana1 GetOutput ]
s0 SetLowBValue $lowb
s0 SetLowBValueIndex $indexlowb
s0 SetHighBValue $highb
s0 SetHighBValueIndex $indexhighb
s0 Update

set outp [ vtkImageData [ pxvtable::vnewobj ]]
$outp ShallowCopy [ s0 GetOutput ]

vtkpxAnalyzeImageWriter ana2

#ana2 SetInput [ s0 GetOutput ]
ana2 SetInput $outp
ana2 SetImageHeader [ana1 GetImageHeader] 
#ana2 SetOrientation [ ana1 GetOrientation ]
#ana2 SetCompression 0
##ana2 SetNumFrames 1

ana2 Save "${filename}_S0"

ana1 Delete
s0 Delete
ana2 Delete
$outp Delete

exit


