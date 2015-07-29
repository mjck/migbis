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

if { $argc < 4} {
    set name [file tail [info script]]
    puts stdout "\n Usage:"
    puts stdout "            $name <image1\[.hdr\]> <starting> <ending> <outname>"
    exit
}

set filename [lindex $argv 0]
set start [lindex $argv 1]
set end [lindex $argv 2]
set outname [lindex $argv 3]

set ana [ vtkpxAnalyzeImageSource [ pxvtable::vnewobj ] ]
$ana Load $filename

vtkmpjFrameExtract ext
ext SetInput [ $ana GetOutput ]
ext SetStartFrame $start
ext SetEndFrame $end

ext Update

puts stdout "Saving resulting image $outname..."
vtkpxAnalyzeImageWriter out
out SetInput [ext GetOutput]
out SetImageHeader [ $ana GetImageHeader ]
out Save $outname

puts stdout "Done!\n"

exit


