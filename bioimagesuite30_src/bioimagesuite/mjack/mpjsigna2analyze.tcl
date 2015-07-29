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
package require pxvtable 1.0

set utilobj [ vtkpxUtil util ]

set orient_name(0) "axial"
set orient_name(1) "coronal"
set orient_name(2) "sagittal"

set import_topslice 1
set import_bottomslice 1
set num_slices 1
set num_frames 1
set num_slicesperframe 1

proc GetNumberofSlices { prefix } {

    set pa(name) f
    set pa(0) 1
    set pa(1) 1
    
    set pa(name) $prefix  
    set flag 0
    
    while { $flag == 0 } {
	set tmpname "$pa(name)$pa(1)"
	set tmpname2 "$pa(name)$pa(1).gz"
	if { [ file exists $tmpname ] ==1 || [ file exists $tmpname2 ]==1 } {
	    incr pa(1)
	} else {
	    set flag 1 
	    set pa(1) [ expr $pa(1) -1 ]
	}
    }
    
    set pa_list [ array get pa ]
    return $pa_list

}

if { $argc == 0} {
    set name [file tail [info script]]
    puts stdout "\nUsage:\n      $name <prefix> <start-slice> <end-slice> <slices/frame> \[outname\]" 
    puts stdout "\nMarcel Jackowski\nmjack@noodle.med.yale.edu\nSeptember 2002"
    puts stdout [lindex $argv 0] 
    exit
}

set prefix [ lindex $argv 0 ]

set lx_reader [ vtkpxSignaLXImageSource [ pxvtable::vnewobj ]  ]
$lx_reader SetFilePrefix [lindex $argv 0]

if { [ $lx_reader ReadHeaderFile ] != 0 } {

	set import_xsp    [ lindex [$lx_reader GetDataSpacing ] 0 ]
	set import_ysp    [ lindex [$lx_reader GetDataSpacing ] 1 ]
	set import_zsp    [ lindex [$lx_reader GetDataSpacing ] 2 ]
	set import_width  [ lindex [$lx_reader GetDataDimensions ] 0 ]
	set import_height [ lindex [$lx_reader GetDataDimensions ] 1 ]
		
	set pa_list [ GetNumberofSlices [lindex $argv 0] ]
	array set pa $pa_list
	set import_topslice    $pa(1)
	set import_bottomslice $pa(0)
	
	set import_topslice    [ lindex $argv 2 ]
	set import_bottomslice [ lindex $argv 1 ]
	set num_slicesperframe [ lindex $argv 3 ] 
	set num_slices [ expr $import_topslice - $import_bottomslice + 1 ]
	set num_frames [ expr $num_slices / $num_slicesperframe ]

	set orient [ $lx_reader GetOrientation ] 
	
	puts stdout "Reading $prefix slices"
	puts stdout "dimensions: $import_width\x$import_height"
	puts stdout "orientation: $orient_name($orient)"
	puts stdout "x resolution: $import_xsp"
	puts stdout "y resolution: $import_ysp"
	puts stdout "z resolution: $import_zsp"
	puts stdout "start slice: $import_bottomslice"
	puts stdout "end slice: $import_topslice"
	puts stdout "num slices: $num_slices"
	puts stdout "num frames: $num_frames"
	
	$lx_reader Load [lindex $argv 0] $import_bottomslice $import_topslice

} 

set outp [ $lx_reader GetOutput ]

vtkpxAnalyzeImageWriter ana

ana SetInput $outp
ana SetOrientation $orient
ana SetCompression 0
ana SetNumFrames $num_frames

set first [ string last "i" $prefix ] 
if { $argc < 5 } {
    set outfname [ string range $prefix 0 [ expr $first - 1 ] ]
} else {
    set outfname [ lindex $argv 4 ]
} 

ana Save $outfname

$lx_reader Delete
ana Delete

exit


