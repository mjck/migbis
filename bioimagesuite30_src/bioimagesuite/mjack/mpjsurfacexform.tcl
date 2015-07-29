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
    puts stdout "Usage: $name <surface.vtk> \[tranformations\] <output_surface.vtk>\n"
    puts stdout "transformations\n:"
    puts stdout "translations (normalized coords):"
    puts stdout "   Tx for translation in x"
    puts stdout "   Ty for translation in y"
    puts stdout "   Tz for translation in z\n"
    puts stdout "rotations (degrees):"
    puts stdout "   Rx for rotation in x"
    puts stdout "   Ry for rotation in y"
    puts stdout "   Rz for rotation in z\n"
    puts stdout "scaling:"
    puts stdout "   Sx for scale in x"
    puts stdout "   Sy for scale in y"
    puts stdout "   Sz for scale in z\n"
    exit
}

vtkGenericDataObjectReader poly
poly SetFileName [ lindex $argv 0 ]
poly Update

puts stdout "Reading in surface file [ lindex $argv 0 ] ..."

set tx 0.0
set ty 0.0
set tz 0.0

set sx 1.0
set sy 1.0
set sz 1.0

set rx 0.0
set ry 0.0
set rz 0.0

vtkTransform xform
xform PostMultiply
xform Identity

for { set i 1 } { $i <= [expr $num - 2 ]} { incr i } {		

	set item [ lindex $argv $i ]

	if { $item == "tx" || $item == "Tx" || $item == "tX" || $item == "TX" } {
		set tx [ lindex $argv [ expr $i + 1 ] ]
		puts "Applying Tx($tx)"
		xform Translate $tx 0.0 0.0

	} elseif { $item == "ty" || $item == "Ty" || $item == "tY" || $item == "TY" } {
		set ty [ lindex $argv [ expr $i + 1]  ]
		puts "Applying Ty($ty)"
		xform Translate 0.0 $ty 0.0

	} elseif { $item == "tz" || $item == "Tz" || $item == "tZ" || $item == "TZ" } {
		set tz [ lindex $argv [ expr $i + 1]  ]
		puts "Applying Tz($tz)"
		xform Translate 0.0 0.0 $tz

	} elseif { $item == "rx" || $item == "Rx" || $item == "rX" || $item == "RX" } {
		set rx [ lindex $argv [ expr $i + 1 ] ]
		puts "Applying Rx($rx)"
		xform RotateX $rx

	} elseif { $item == "ry" || $item == "Ry" || $item == "rY" || $item == "RY" } {
		set ry [ lindex $argv [ expr $i + 1 ] ]
		puts "Applying Ry($ry)"
		xform RotateY $ry

	} elseif { $item == "rz" || $item == "Rz" || $item == "rZ" || $item == "RZ" } {
		set rz [ lindex $argv [ expr $i + 1 ] ]
		puts "Applying Rz($rz)"
		xform RotateZ $rz

	} elseif { $item == "sx" || $item == "Sx" || $item == "sX" || $item == "SX" } {
		set sx [ lindex $argv [ expr $i + 1 ] ]
		puts "Applying Sx($sx)"
		xform Scale $sx 1.0 1.0

	} elseif { $item == "sy" || $item == "Sy" || $item == "sY" || $item == "SY" } {
		set sy [ lindex $argv [ expr $i + 1 ] ]
		puts "Applying Sy($sy)"
		xform Scale 1.0 $sy 1.0

	} elseif { $item == "sz" || $item == "Sz" || $item == "sZ" || $item == "SZ" } {
		set sz [ lindex $argv [ expr $i + 1 ] ]
		puts "Applying Sz($sz)"
		xform Scale 1.0 1.0 $sz
	}
	
}

vtkTransformPolyDataFilter transformer
transformer SetInput [ poly GetPolyDataOutput ]
transformer SetTransform xform
transformer Update

vtkPolyDataWriter out
out SetInput [ transformer GetOutput ]

set foutname "[lindex $argv [expr $num - 1]]"
out SetFileName $foutname

puts stdout "Writing surface $foutname..."
out Write

puts stdout "Done!\n"

out Delete
transformer Delete
xform Delete
poly Delete

exit
