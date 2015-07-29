#!/bin/sh
# the next line restarts using wish \
    exec vtk "$0" -- "$@"

#BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
#BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
#BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
#BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
#BIOIMAGESUITE_LICENSE  Medicine, http:#www.bioimagesuite.org.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
#BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
#BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
#BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
#BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
#BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
#BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#BIOIMAGESUITE_LICENSE  See also  http:#www.gnu.org/licenses/gpl.html
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
#BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
#BIOIMAGESUITE_LICENSE 
#BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".." ] base]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".." ] apps]

package require bis_surfacetosurfacealgorithm 1.0

package provide bis_curvatures 1.0

#
# compute curvatures of polydata
#

itcl::class bis_curvatures {

    inherit bis_surfacetosurfacealgorithm

     constructor { } {
	 $this Initialize
     }

    public method Initialize { }
    public method Execute { }
    public method GetGUIName { } { return "Curvatures" }
}

# -----------------------------------------------------------------------------------------
# Initialize
# ----------------------------------------------------------------------------------------

itcl::body bis_curvatures::Initialize { } {

    PrintDebug "bis_curvatures::Initialize" 
    #commandswitch,description,shortdescription,optiontype,defaultvalue,valuerange,priority
    set options {
	{ ctype     "gaussian: Gaussian curvature , mean:Mean curvature   " "ctype" listofvalues   gaussian   { mean gaussian }  0 }
    }

    set defaultsuffix { "_curv" }
    
    set scriptname bis_curvatures

    #
    #document
    #
    set category "Surface Processing"
    set description  "computes curvatures of a surface"
    set description2 "curvature can be either gaussian curvature or mean curvature"
    set backwardcompaibitlity "Newly added."
    set authors "alark.joshi@yale.edu"

    $this InitializeSurfacetoSurfaceAlgorithm
}

# -----------------------------------------------------------------------------------------
# Execute
# ----------------------------------------------------------------------------------------

itcl::body bis_curvatures::Execute {  } {

    PrintDebug "bis_curvatures::Execute"

    # Get Inputs
    set surface    [ $this GetInput  ]
    # GetOptions
    set ctype      [ $OptionsArray(ctype) GetValue ]

    # Run Algorithm
    set filter [ vtkCurvatures [ pxvtable::vnewobj ] ]
    $filter SetInput [ $surface GetObject ]
    if { $ctype == "gaussian" } {
	$filter SetCurvatureTypeToGaussian
    } else {
	$filter SetCurvatureTypeToMean
    }
    $filter Update

    # Store Results in output
    set surface_out [$filter GetOutput ]
    [ $OutputsArray(output_surface) GetObject ] DeepCopySurface $surface_out 

    # Clean up
    $filter Delete

    return 1
}


# -----------------------------------------------------------------------------------------
#  This checks if executable is called (in this case bis_curvatures.tcl) if it is execute
# ----------------------------------------------------------------------------------------
 
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    # this is essentially the main function

    

    set alg [bis_curvatures [pxvtable::vnewobj]]
    $alg MainFunction 
}

