#!/bin/sh
# the next line restarts using wish \
    exec vtk "$0" "$@"

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






package provide pxitclmousechopper 1.0
package provide pxitclsurobj 1.0

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]

# 	$Id: pxitclmousechopper.tcl,v 1.1 2002/06/11 01:21:41 papad Exp papad $	

package require  labelframe 1.0
package require  pxtclutil  1.0
package require  pxvtable   1.0
package require  vtkpxcontrib 1.1
package require  pxitclobject 1.0
package require  pxitclbasecontrol 1.0

# ------------------------------------------------------------------------------------------
#
# pxitclmouseplanes 
#
# ------------------------------------------------------------------------------------------
itcl::class pxitclmouseplanes {

    inherit pxitclobject

    protected variable numcomp    8
    protected variable planearray 0
    protected variable planenames ""
    protected variable planeorder ""
    protected variable firstinit  1


    protected    variable hasfeet          0
    protected    variable hashindlimbs     0
    protected    variable hasforelimbs     0
    protected    variable hashead          0
    protected    variable taildivisions    0
    protected    variable spinedivisions 0
    public       variable dimensions [ list 300 250 400 ]
    
    constructor { maxplanes } { 	$this Initialize $maxplanes    }
    destructor  { catch { $planearray Delete }  }


    # --- begin method def -----
    public method Initialize { maxplanes } 


    # Things that must be over-ridden by derived classes
    # --------------------------------------------------
    public method GetObject { } { return $planearray }
    public method GetPlanes { } { return $planearray }
    public method GetNames { } { return $planenames  }
    protected method AddName { name } { lappend planenames $name }
    protected method AddPart { name nx ny nz dist above} 

    public method GetType { } { return "vtkFloatArray" }
    public method GetDescription { } 
    public method UpdateStatus { }  { }
    public method Copy    { obj }
    public method Clear   { } 
    public method InitializeModel { feet hindlimbs forelimbs head taildivisions spinedivisions }
    public method GetModelParameters { } { return [ list $hasfeet $hashindlimbs $hasforelimbs $hashead $taildivisions $spinedivisions ] }

    # Create Region Label Image
    public method CreateRegionLabelImage { img resolution kernel threshold surface } 
    public method GetPlaneNumber { jointno } 
    public method GetJointNumber { planeno } 
    public method GetAllJointNumbers { }

    # Create Wireframe Model
    # ----------------------
    protected method AddSkeletonPart { centerno joints numparts sekeletonpts linepairs inindex pointindex length } 
    public method CreateWireframeModel { joints spheresize tubesize }

    # Procedures to Load/Save 
    public method Load { args } 
    public method Save { args } 
}

# ---------------------------------------------------------------------------------------------

itcl::body pxitclmouseplanes::Initialize { maxplanes } {
    catch  { $planearray Delete }
    set planearray [ vtkFloatArray [ pxvtable::vnewobj ]]
    $planearray SetNumberOfComponents $numcomp
    
    if { $maxplanes < 2 } { set maxplanes 2 } elseif { $maxplanes > 30 } { set maxplanes 30 }

    $planearray SetNumberOfTuples $maxplanes
    $this Clear
    return $maxplanes
}

itcl::body pxitclmouseplanes::GetDescription {  } {
    
    return "Plane array $filename\nNumber of planes [ llength $planenames ]\n $planenames"
}

itcl::body pxitclmouseplanes::Copy { obj  } {
    $planearray DeepCopy [ $obj GetPlanes ]
    set planenames [ $obj GetPlaneNames ]
}

itcl::body pxitclmouseplanes::Clear {  } {


    $planearray FillComponent 0 1.0
    for { set i 1 } { $i < $numcomp } { incr i } {
	$planearray FillComponent $i 0 
    }
    set planenames ""
    
}

itcl::body pxitclmouseplanes::Load { args } {

    set len [ llength $args ]
    if { [ string length $filename ] == 0 && $len == 0 } { return  }

    if { $len < 1 } {
	set fname $filename
    } else  { 
	set fname [ lindex $args 0 ] 
    }

    if { $len > 1 } { 
	set desc [ lindex $args 1 ]
    } else {
	set desc "Planes File"
    }

    if { [ string length $fname ] == 0 } {
	set f2path [ file dirname $filename ]
	set f2tail [ file tail    $filename ]
	set fname  [tk_getOpenFile -title "Load $desc" -filetypes { {"Planes Files" {.planes}}} -initialfile $f2tail -initialdir $f2path ]   
    }
    
    if { [ string length $fname ] <1 } {
	return 0
    }

    # Load it here

    if { [ file exists $fname ] == 0 } {
	return 0
    }

    set fin [ open $fname r ]
    gets $fin line
    if { $line != "\#Planes Description File" } {
	close $fin
	return -1
    }

    set filename $fname

    gets $fin line; gets $fin line; 
    gets $fin maxplanes 
    
    set maxplanes [ $this Initialize $maxplanes ]

    gets $fin line; gets $fin hasfeet
    gets $fin line; gets $fin hashindlimbs
    gets $fin line; gets $fin hasforelimbs
    gets $fin line; gets $fin hashead
    gets $fin line; gets $fin taildivisions
    gets $fin line; gets $fin spinedivisions
	
    gets $fin line
    
    for { set i 0 } { $i < $maxplanes } { incr i } {
	gets $fin line
	lappend planenames $line
	gets $fin line
	scan $line "%f %f %f %f %f %f %f %d" nx ny nz dist cx cy cz abv 
	
	$planearray SetComponent $i 0 $nx
	$planearray SetComponent $i 1 $ny
	$planearray SetComponent $i 2 $nz
	$planearray SetComponent $i 3 $dist
	$planearray SetComponent $i 4 $cx
	$planearray SetComponent $i 5 $cy
	$planearray SetComponent $i 6 $cz
	$planearray SetComponent $i 7 $abv

    }
    close $fin
    return 1
}
# ----------------------------------------------------------
itcl::body pxitclmouseplanes::Save { args } {

    set len [ llength $args ]
    if { $len < 1 } {
	set fname $filename
    } else  { 
	set fname [ lindex $args 0 ] 
    }

    set desc "Planes File"
    if { $len > 1 } { 
	set desc [ lindex $args 1 ]
    }
    
    if { [ string length $fname ] == 0 } {
	set f2path [ file dirname $filename ]
	set f2tail [ file tail    $filename ]
	set fname  [tk_getSaveFile -title "Saving $desc" -filetypes { {"Planes Files" {.planes}}} -initialfile $f2tail -initialdir $f2path ]   
    }
    
    if { [ string length $fname ] < 1 } {
	return 0
    }

    set fout [ open $fname w ]
    puts $fout "\#Planes Description File\n\#------------------------------------"
    puts $fout "\#Number of Planes\n [llength $planenames ]"

    puts $fout "#Feet\n$hasfeet"
    puts $fout "#Hind Limbs \n$hashindlimbs"
    puts $fout "#Fore Limbs \n$hasforelimbs"
    puts $fout "#Head \n$hashead"
    puts $fout "#Tail Divisions \n$taildivisions"
    puts $fout "#Spine Divisions \n$spinedivisions"

    puts $fout "\#Planes "
    
    for { set i 0 } { $i < [ llength $planenames ] } { incr i } {
	puts $fout [ lindex $planenames $i ]
	for { set j 0 } { $j < 7 } { incr j } { 
	    puts -nonewline $fout [ format "%.3f " [ $planearray GetComponent $i $j ]]
	}
	puts $fout [ format "%d " [ expr round([ $planearray GetComponent $i 7 ]) ]  ]
    }
    close $fout
    set filename $fname
    return 1
}

itcl::body pxitclmouseplanes::AddPart { name nx ny nz offset above}  {

    set dist 1.0
#    catch { 
	set dist [ expr abs($nx) * [ lindex $dimensions 0 ] + abs($ny) * [ lindex $dimensions 1 ] + abs ($nz)*[ lindex $dimensions 2 ] ]

#    }
    set dist [ expr $dist * $offset ]

    set index [ llength $planenames ]
    
    lappend planenames $name
    $planearray SetComponent $index 0 $nx
    $planearray SetComponent $index 1 $ny
    $planearray SetComponent $index 2 $nz
    $planearray SetComponent $index 3 $dist
    $planearray SetComponent $index 4 [ expr 0.5* [ lindex $dimensions 0 ]]
    $planearray SetComponent $index 5 [ expr 0.5* [ lindex $dimensions 1 ]]
    $planearray SetComponent $index 6 [ expr 0.5* [ lindex $dimensions 2 ]]
    $planearray SetComponent $index 7 [ expr ($above > 0 ) ]

}

itcl::body pxitclmouseplanes::InitializeModel { feet hindlimbs forelimbs head taildiv spinediv } {

    if { $firstinit == 0 } {
	set ok [ pxtclutil::Question "Creating a new model will erase your current model. Are you sure?" ]
	if { $ok == "no" } {
	    return 0
	}
    } else {
	set firstinit 0
    }

    set hasfeet [ expr $feet > 0 ]
    set hashindlimbs [ expr $hindlimbs > 0 ]
    set hasforelimbs [ expr $forelimbs > 0 ]
    set hashead [ expr $head > 0 ]

    set taildivisions $taildiv
    if { $taildivisions < 0 } { set taildivisions 0 } elseif { $taildivisions > 5 } { set taildivisions 5 }

    set spinedivisions $spinediv
    if { $spinedivisions < 0 } { set spinedivisions 0 } elseif { $spinedivisions > 10 } { set spinedivisions 10 }

    set total [ expr 2*($hashindlimbs+$hasforelimbs)*(3+$hasfeet) + $hashead + $taildiv + $spinediv ]
    $this Initialize $total

    set dir [ list Left Right ]
    
    if { $hashindlimbs > 0 } {
	for { set i 0 } { $i <=1 } { incr i } {
	    set name [ lindex $dir $i ]
	    AddPart "${name}-Hipp-Horizontal" 0 0 1 -0.2 1
	    if { $i == 0 } {
		AddPart "${name}-Hipp-Vertical"   1 0 0 0.1 1 
	    } else {
		AddPart "${name}-Hipp-Vertical"   1 0 0 -0.1 0
	    }
	    AddPart "${name}-Knee" 0 0 1 0.1 1

	    if { $hasfeet > 0 } {
		AddPart "${name}-Ankle" 0 0 1 0.3 1
	    }
	}
    }

    if { $taildiv > 0 } {
	for { set i 0 } { $i < $taildiv } { incr i } { 
	    AddPart "Tail-[ expr $i+1 ]"  0 0 1 [ expr 0.1+0.25*($i +1 )/($taildiv+1) ] 1
	}
    }

    if { $spinediv > 0 } {
	for { set i 0 } { $i < $spinediv } { incr i } { 
	    AddPart "Spine-[ expr $i+1 ]"  0 0 1 [ expr -0.1-0.25*($i +1 )/($taildiv+1) ] -1
	}
    }

    if { $hasforelimbs > 0 } {
	for { set i 0 } { $i <=1 } { incr i } {
	    set name [ lindex $dir $i ]
	    AddName "${name}-Shoulder-Horizontal"
	    AddName "${name}-Shoulder-Vertical"
	    AddName "${name}-Elbow"
	    if { $hasfeet > 0 } {
		AddName "${name}-Wrist"
	    }
	}
    }

    if { $hashead > 0 } {
	AddName "Neck"
    }

    return 1

}

itcl::body pxitclmouseplanes::CreateRegionLabelImage { img resolution kernel threshold surface } {

    # First just threshold
    # --------------------
    set ut [ vtkpxUtil [ pxvtable::vnewobj ]]

    set resl [ vtkImageResample [ pxvtable::vnewobj ] ]
    $resl SetInput [ $img GetImage ]
    $resl SetDimensionality 3
    $resl InterpolateOn
    for { set i 0 } { $i <=2 } { incr i } {
	$resl SetAxisMagnificationFactor $i [ expr 1.0/$resolution ]
    }
    $resl Update

    puts stderr "[ [ $resl GetOutput ] GetDimensions ] "

    set thr [ vtkImageThreshold [ pxvtable::vnewobj ]]
    $thr SetInput [ $resl GetOutput ]
    $thr ThresholdByUpper $threshold
    $thr ReplaceInOn
    $thr ReplaceOutOn
    $thr SetInValue 100.0
    $thr SetOutValue 0.0
    $thr SetOutputScalarTypeToShort
    $thr Update

    $resl Delete

    # Next 
    # --------------------
    set sim [ vtkpxTransformationUtil [ pxvtable::vnewobj ]]
    set tmp [ $sim  CreateWholeBodyRegionLabelImage [ $thr GetOutput ] $planearray $hasfeet $hashindlimbs $hasforelimbs $hashead $taildivisions $spinedivisions ]
    
    set done 0
    catch {
	set done [ lindex [ $tmp GetDimensions ] 0 ]
    }

    if { $done == 0 } {
	$thr Delete
	return 0
    }

    #  $ut SaveAnalyze "tt_multi" $tmp 5

    if { $surface !=0 } {
	set imgout [ $sim CreateRegionLabelImage $tmp $kernel $surface ]
    } else {
	set imgout [ $sim CreateRegionLabelImage $tmp $kernel ]
    }

    #  $ut SaveAnalyze "tt_model" $imgout 5

    $tmp Delete
    $sim Delete
    $thr Delete
    $ut Delete
    return $imgout
}
# ------------------------------------------------------------------------------------------
itcl::body pxitclmouseplanes::GetPlaneNumber { jointno }  {
    
    if { $jointno < 1 } {
	return -1
    }
    
    set jointno [ expr $jointno -1 ]

    set legjoints  [ expr  2+$hasfeet ]
    set legplanes  [ expr 3+$hasfeet ]

    set offset 0
    
    set numlimbs [ expr 2*($hashindlimbs + $hasforelimbs) ]
    set totallimbjoints  [ expr $numlimbs * $legjoints ]
    set totallimbplanes  [ expr $numlimbs * $legplanes ]

#    puts stderr "\t\t\t jointno'=$jointno Number of limbs = $numlimbs,  totallimbjoints=$totallimbjoints, totallimbplanes=$totallimbplanes"

    # Step 1 is this a tail or a spine
    if { $jointno >= $totallimbjoints } {
	return [ expr int($jointno - $totallimbjoints + $totallimbplanes)  ]
    }
    
    # Step 2 is find which limb
    set limbno    [ expr int($jointno/$legjoints) ]
    set limbjoint [ expr $jointno-$limbno*$legjoints ]

#    puts stderr "\t\t\t limbno=$limbno limbjoint=$limbjoint"

    set offset [ expr int($limbno * $legplanes) ]

    return [ expr int($offset + $limbjoint +1) ]

}
# ------------------------------------------------------------------------------------------
itcl::body pxitclmouseplanes::GetAllJointNumbers {  }  {

    for { set i 0 } { $i < [ llength $planenames ] } { incr i } {
	set jnt [ $this GetJointNumber $i ]
	set pla [ $this GetPlaneNumber $jnt ]
	set pn  [ lindex $planenames $pla ]
	puts stderr "[ lindex $planenames $i ] --> Plane $i --> Joint $jnt --> Plane $pla, $pn"
    }
}

itcl::body pxitclmouseplanes::GetJointNumber { planeno }  {
    

    set legjoints [ expr  2+$hasfeet ]
    set legplanes  [ expr 3+$hasfeet ]

    set offset 0
    set totalhips 0

    set numlimbs [ expr 2*($hashindlimbs + $hasforelimbs) ]
    set totallimbjoints  [ expr $numlimbs * $legjoints ]
    set totallimbplanes  [ expr $numlimbs * $legplanes ]

    #    puts stderr "\t\t\t Number of limbs = $numlimbs,  totallimbjoints=$totallimbjoints, totallimbplanes=$totallimbplanes"

    # Step 1 is this a tail or a spine
    if { $planeno >= $totallimbplanes } {
	return [ expr $planeno - $totallimbplanes + $totallimbjoints +1 ]
    }

    # Step 2 is find which limb
    set limbno    [ expr floor($planeno/$legplanes) ]
    set limbplane [ expr $planeno-$limbno*$legplanes ]

    #puts stderr "\t\t\t limbno=$limbno limbplane=$limbplane"

    set offset [ expr int($limbno * $legjoints)+1 ]
    if { $limbplane < 2 } {
	return $offset 
    } elseif { $limbplane ==2 } {
	return [ expr $offset+1 ]
    } else {
	return [ expr $offset+2 ]
    }
}
# ------------------------------------------------------------------------------------------
itcl::body pxitclmouseplanes::AddSkeletonPart { centerno joints numparts skeletonpts linepairs inindex pointindex length } {

    puts stderr "\n\n---------------------------------------------------------------------"
    puts stderr "Addining skeleton part centerno=$centerno numparts=$numparts inindex=$inindex pointindex=$pointindex\n"

    for { set i 0 } { $i < $numparts } { incr i } {
	set x [ $joints GetPoint  [ expr $inindex*3 ] ]
	puts stderr "Inserting Point $inindex ($x)"
	$skeletonpts InsertNextPoint [ lindex $x 0 ] [ lindex $x 1 ] [ lindex $x 2 ]
	$linepairs InsertNextCell 2
	if { $i == 0 } {
	    set p1 $centerno
	} else {
	    set p1 [ expr $pointindex -1 ]
	}
	set p2 $pointindex
	$linepairs InsertCellPoint $p1
	$linepairs InsertCellPoint $p2
	puts stderr "Inserted Pair $p1, $p2 ... numberofpts = [ $skeletonpts GetNumberOfPoints ]"
	incr pointindex
	incr inindex
    }
    
    # This is the end of the leg
    # --- Add this later
    # --------------------------
    
     set x0 [ $skeletonpts GetPoint [ expr $pointindex - 2 ] ]
     set x1 [ $skeletonpts GetPoint [ expr $pointindex - 1 ] ]
     for { set i 0 } {$i <= 2 } { incr i } {
 	set x2($i) [ expr [ lindex $x1 $i ] + $length * ( [ lindex $x1 $i ] - [ lindex $x0 $i ]) ]
     }
     puts stderr "Inserting Point $x2(0) $x2(1) $x2(2) from x0 ( [ expr $pointindex - 2 ] )=$x0, x1 ( [ expr $pointindex - 1 ] )=$x1,   "
     $skeletonpts InsertNextPoint $x2(0) $x2(1) $x2(2)
     $linepairs InsertNextCell 2
     $linepairs InsertCellPoint [ expr $pointindex - 1 ]
     $linepairs InsertCellPoint $pointindex
     incr pointindex

    return $pointindex

}
# ------------------------------------------------------------------------------------------
itcl::body pxitclmouseplanes::CreateWireframeModel { joints spheresize tubesize } {

    set njoints   [ expr int([ $joints GetNumberOfPoints ] /3) ]
    set spinehead [ expr $spinedivisions + $hashead ]	

    set neededjoints [ expr 2*($hashindlimbs+$hasforelimbs)*(2+$hasfeet) + $taildivisions + $spinehead + 1]

    
    if { $tubesize <=0.0 && $spheresize <=0.0 } {
	::pxtclutil::Warning "sphereisze=0.0 and tubesize=0.0, nothing to do!"
	return 0
    }

    if { $njoints != $neededjoints } {
	::pxtclutil::Warning "Not enough points specified to create wireframemodel \nneed [ expr 3*$neededjoints ]"
	return 0
    }

    
    
    # Two prong strategy ... define all points .... define all spheres ... define poly (connect)

    # First two the joint locations --- this is easy every third point !!!
    # --------------------------------------------------------------------
    set jointloc [ vtkPoints [ pxvtable::vnewobj ]]
    $jointloc Allocate [ expr $njoints +8 ] 10

    for { set i 0 } { $i < $njoints } { incr i } {
	set x [ $joints GetPoint [ expr $i *3 ] ]
	$jointloc InsertNextPoint [ lindex $x 0 ] [ lindex $x 1 ] [ lindex $x 2 ]		
    }

    # Now The Skeleton
    set skeletonpts [ vtkPoints [ pxvtable::vnewobj ]]
    $skeletonpts Allocate [ expr $njoints +8 ] 10
    set linepairs [  vtkCellArray [ pxvtable::vnewobj ]]
    $linepairs Allocate [ expr $njoints +8 ] 10

    # Add Hip Center
    set x [ $joints GetPoint 0 ]
    $skeletonpts InsertNextPoint [ lindex $x 0 ] [ lindex $x 1 ] [ lindex $x 2 ]
    set center 0
    set pointindex  1
    set legparts [ expr 2 + $hasfeet ]

    if { $hashindlimbs ==1 } {
	for { set leg 0 } { $leg <=1 } { incr leg } {
	    set offset [ expr 1 + ( $legparts * $leg ) ]
	    set pointindex [ AddSkeletonPart $center $joints $legparts $skeletonpts $linepairs $offset $pointindex 0.25 ]
	}
    }

    if { $taildivisions > 0 } {
	set offset [ expr 1 + 2*$legparts*($hashindlimbs+$hasforelimbs) ]
	set pointindex [ AddSkeletonPart $center $joints $taildivisions $skeletonpts $linepairs $offset $pointindex 0.5 ]
    }

    if { $spinehead > 0 } {
	set offset [ expr 1 + 2*$legparts*($hashindlimbs+$hasforelimbs) + $taildivisions ]
	set length 1.0
	if { $hashead > 0 } {
	    set length 0.25
	}
	set pointindex [ AddSkeletonPart $center $joints $spinehead $skeletonpts $linepairs $offset $pointindex $length ]
	set center [ expr $pointindex -1 ]
    }

    if { $hasforelimbs ==1 } {
	for { set leg 0 } { $leg <=1 } { incr leg } {
	    set offset 1 
	    if { $hashindlimbs > 0 } {
		set offset [ expr 1 + $legparts *2 ]
	    }
	    set pointindex [ AddSkeletonPart $center $joints $legparts $skeletonpts $linepairs $offset $pointindex 0.25 ]
	}
    }


    # ... make output ...

    set append [ vtkAppendPolyData [ pxvtable::vnewobj ]]

    if { $spheresize > 0.0 } {
	# first spheres ... then tubes ... then combine
	set glyph [ vtkGlyph3D [ pxvtable::vnewobj ]]
	
	set sphere [ vtkSphereSource [ pxvtable::vnewobj ]] 
	$sphere SetRadius $spheresize
	$sphere Update
	$glyph SetSource [ $sphere GetOutput ]
	$sphere Delete
	
	set pdata [ vtkPolyData [ pxvtable::vnewobj ]] 
	$pdata SetPoints $jointloc; $jointloc Delete
	
	$glyph SetInput $pdata ; $pdata Delete
	$glyph Update
	$append AddInput [ $glyph GetOutput ] 
	$glyph Delete
    }

    # then tubes ----------------------------------
    if { $tubesize > 0.0 } {
	set tube [ vtkTubeFilter [ pxvtable::vnewobj ]]
	
	set pdata [ vtkPolyData [ pxvtable::vnewobj ]] 
	$pdata SetPoints $skeletonpts; $skeletonpts Delete
	$pdata SetLines $linepairs   ; $linepairs Delete
	
	$tube SetInput $pdata 
	$tube SetRadius $tubesize
	$tube SetNumberOfSides 8
	$tube SetVaryRadiusToVaryRadiusOff
	$tube Update
	$append AddInput [ $tube GetOutput ]
	#$append AddInput  $pdata
	$tube Delete
	
	$pdata Delete
	
	# then combine and we are done ---------------
    }
    $append Update
    set pdata [ vtkPolyData [ pxvtable::vnewobj ]] 
    $pdata ShallowCopy [ $append GetOutput ]; $append Delete
    return $pdata
}

# ------------------------------------------------------------------------------------------
#
# pxitclmousechopper control
#
# ------------------------------------------------------------------------------------------

itcl::class pxitclmousechopper {

    inherit pxitclbasecontrol

    private   common   thisparam
    protected variable initialized       0
    protected variable planeslistbox     0
    public    variable oblique_slice     0
    public    variable oblique_gui       0
    public    variable mouse_map_gui     0
    public    variable mouse_label_gui   0
    public    variable planes            0
    public    variable appname "Virtual Mouse Butcher"
    #    protected variable fullyinitialized  0
    # ------------------------------------------------------
    
    constructor { par } {
	::pxitclbasecontrol::constructor $par
    } { 
	set planes [ [ pxitclmouseplanes \#auto 30 ] GetThisPointer ]
    }
    
    # Initialization
    # --------------
    public    method AttachToBaseControl { breg { addseparator 1 } } 
    public    method Initialize  { basewidget args } 
    public    method DelayedInitialize  { }

    
    # Create GUI
    # ----------
    public    method CreateDefinitionsGUI { widget }
    public    method CreatePlanesGUI { widget }
    public    method CreateMapGUI { widget }
    public    method CreateSegmentedMapGUI { widget }
    public    method CreateOutputGUI { widget }

    # Segmentation Postprocessing
    # ---------------------------
    public    method UpdateLabelImageRanges { }
    public    method GrabSegmentationMapFromViewer { args }
    public    method CreateSoftTissueVOI {  }
    public    method CreateBoneVOI {  }
    public    method ExtractAndResliceLabelMap { tissuelabel  tissuelabel2 suffix  { dilateerode 0 } }

    #  Joint Extraction
    # ---------------------------
    public    method GetJoint { index map bonemap } 
    public    method AutoJoints {  } 
    public    method SingleJoint { } 
    public    method ClickJoints { { mode manual } } 


    # Model Generation
    # ----------------
    public    method LoadModel { fname } 
    protected method FixPlanes {  } 
    public    method LoadMouseMap { fname } 
    public    method LoadSegmentedMap { fname } 

    public    method SaveModel { fname } 
    public    method InitializeModel { mode args } 
    public    method GenerateTransformationModel { } 
    public    method GenerateMouseMap { } 
    public    method GenerateWireModel { } 

    # Oblique Slice Interaction
    # -------------------------
    public    method ShowOblique { } 
    public    method UpdateArrowColor { } 
    public    method UpdateGUIFromPlanes { } 
    public    method UpdatePlanesFromGUI { } 
    public    method SelectPlane { { value -1 } { direction 0 } } 
    public    method TestPlane { mode }
    public    method UpdatePlane { } 
    public    method GetObliqueSlice { } 
    
    # Segmentation Control
    # --------------------
    public method ShowSegmentationControl { }

}

itcl::body pxitclmousechopper::AttachToBaseControl { breg  { addseparator 1 } } { 
    
    $this Initialize [ $breg GetBaseWidget ].[pxvtable::vnewobj ]
    if { $addseparator > 0 } {
	[ $breg cget -featuremenu ] add separator
    }
    $this AddToMenuButton [ $breg cget -featuremenu ]  "Mouse Chopper"
}

# ------------------------------------------------------------------------------------------
itcl::body pxitclmousechopper::LoadMouseMap { fname } { 
    set image [ $mouse_map_gui GetObject ]
    $image Load $fname
    $mouse_map_gui Update
}

itcl::body pxitclmousechopper::LoadSegmentedMap { fname } { 
    set image [ $mouse_label_gui GetObject ]
    $image Load $fname
    $mouse_label_gui Update
}

# --------------------------------------------------------------------

itcl::body pxitclmousechopper::FixPlanes { } { 

    # Go to first plane
    if { [ $this GetObliqueSlice  ] == 0 } { return }

    set parray [ $planes GetPlanes ]
    set nt [ $parray GetNumberOfTuples ]
    set max [ expr $nt -1 ]

    set ok $thisparam($this,enable_updateplane)
    set thisparam($this,enable_updateplane) 1

    for { set index $max } { $index >= 0 } { set index [ expr $index -1 ] } {
	puts stderr "Fixing Plane $index/$nt"
	$this SelectPlane $index 0
	$oblique_slice UpdateImagePlane [ $parray GetComponent $index 3 ] [ $parray GetComponent $index 0 ]  [ $parray GetComponent $index 1 ]  [ $parray GetComponent $index 2 ] 
	$oblique_gui   UpdateScales
	$this UpdateArrowColor
	$this UpdatePlane
    }

    set thisparam($this,enable_updateplane) $ok
    
}

# --------------------------------------------------------------------

itcl::body pxitclmousechopper::LoadModel { fname } { 

    if { $fullyinitialized == 0 } { $this DelayedInitialize }

    set ok [ $planes Load $fname ]

    if { $ok > 0 } {

	SetTitle "$appname [ file tail [ $planes cget -filename ]]"

	$this FixPlanes

	$this UpdateGUIFromPlanes
    } elseif  { $ok == -1 } {
	::pxtclutil::Warning "Failed to read planes model from [ $planes cget -filename ]"
    }

    $planes  GetAllJointNumbers
}

itcl::body pxitclmousechopper::SaveModel { fname } { 

    UpdatePlanesFromGUI
    set ok [ $planes Save $fname ]

    if { $ok > 0 } {
	SetTitle "$appname [ file tail [ $planes cget -filename ]]"
    }

}
# -------------------------------------------------------------------------------
itcl::body pxitclmousechopper::ClickJoints { { mode manual } } {

    set param [ $planes GetModelParameters ] 
    
    set hasfeet [ lindex $param 0 ]
    set hashindlimbs [ lindex $param 1 ]
    set hasforelimbs [ lindex $param 2 ]
    set hashead [ lindex $param 3 ]
    set taildivisions [ lindex $param 4 ]
    set spinedivisions [ lindex $param 5 ]

    set ok 1
    
    set landmarkcontrol [ $parent GetLandmarkControl ] 
    $landmarkcontrol ShowWindow
    [ $landmarkcontrol GetLandmarkControl ] SetArticulatedMouseSetup $hasfeet $hashindlimbs $hasforelimbs $hashead $taildivisions $spinedivisions 
    
    if { $mode == "manual"} {
	::pxtclutil::Info "Follow the instructions in the landmark control to locate the joints"
    }

    return $ok

}
# -------------------------------------------------------------------------------
itcl::body pxitclmousechopper::GenerateWireModel { } {

    set landmarkcontrol 0
    set polydatacontrol 0

    catch { set landmarkcontrol [ [ $parent GetLandmarkControl ]  GetLandmarkControl ]  }
    catch { set  polydatacontrol [ [ $parent GetPolyDataControl ] GetPolyDataControl ] }

    if { $landmarkcontrol == 0 || $polydatacontrol == 0 } { return  }
    
    set curve [ $landmarkcontrol GetCollection -1 ]
    set newcurve [ vtkpxBaseCurve [ pxvtable::vnewobj ]]
    $newcurve Copy  $curve 
    $newcurve Compact
    set joints [ $newcurve GetPoints ]


    set surface [ $planes CreateWireframeModel $joints $thisparam($this,sphereradius) $thisparam($this,tuberadius) ]
    if { $surface !=0 } {
	$polydatacontrol SetClipboardSurface $surface "wiremodel.vtk"
	::pxtclutil::Info "Wiremodel surface wiremodel.vtk is in the clipboard of the viewer surface control"
	[ $parent GetPolyDataControl ] ShowWindow
	$surface Delete
    }
   
    $newcurve Delete
    return 0
}

# -------------------------------------------------------------------------------
itcl::body pxitclmousechopper::GenerateTransformationModel { } {

    # put it all together to generate the full transformation model and save in .psim file
    #
    #
    set image [ $mouse_map_gui GetObject ]
    if { [ $image GetImageSize ] < 2 } {
	::pxtclutil::Warning "Failed to generate transformation model, no Mouse VOI Map in memory"
	return 0
    }

    set r [ [ [ [ $image GetImage ] GetPointData ] GetScalars ] GetRange ]
    set rmax [ lindex $r 1 ]
    set rmin [ lindex $r 0 ]

    set param [ $planes GetModelParameters ] 
    set hasfeet [ lindex $param 0 ]
    set hashindlimbs [ lindex $param 1 ]
    set hasforelimbs [ lindex $param 2 ]
    set hashead [ lindex $param 3 ]
    set taildivisions [ lindex $param 4 ]
    set spinedivisions [ lindex $param 5 ]

    set totallabels [ expr 1+2*($hashindlimbs+$hasforelimbs)*(2+$hasfeet) + $hashead + $taildivisions + $spinedivisions ]
    set maxrange    [ expr $totallabels -1 ]


    if { $rmin !=-1 || $rmax != $maxrange } {
	::pxtclutil::Warning "Bad Mouse VOI Map. Range = ${rmin}:${rmax}, expecting something like -1:${totallabels}"
	return 0
    }
    
    set numjoints $rmax
    set neededpoints [ expr $numjoints * 3 ]

    set landmarkcontrol 0
    catch { 
	set landmarkcontrol [ [ $parent GetLandmarkControl ]  GetLandmarkControl ]
    }

    if { $landmarkcontrol == 0 } {
	return
    }

    set curve [ $landmarkcontrol GetCollection -1 ]
    set numpoints [ $curve GetNumPoints ]

    if { $numpoints < $neededpoints } {
	::pxtclutil::Warning "Bad Landmark File, not enough joint points clicked ($numpoints vs $neededpoints)"
	return 0
    }



    set outfname  [tk_getSaveFile -title "Specify Output Filename For Transformation"  -filetypes { {"PolySimilarity Files" {.psim}} } ]
    if { [ string length $outfname ] < 1 } {
	return 0
    }


    set newcurve [ vtkpxBaseCurve [ pxvtable::vnewobj ]]
    $newcurve Copy  $curve 
    $newcurve Compact
    set points [ $newcurve GetPoints ]

    set imgname "[ file root $outfname ]_model.hdr"
    $image Save $imgname
    $mouse_map_gui Update


    set newtmodel [ vtkpxPolySimilarityTransform [ pxvtable::vnewobj ] ]
    $newtmodel SetRegionLabelImageName $imgname
    $newtmodel SetRegionLabel [ $image GetImage ]
    $newtmodel SetWholeBodyPreset 0  $hasfeet $hashindlimbs $hasforelimbs $hashead $taildivisions $spinedivisions 
    $newtmodel SetOriginAxes $points

    $newtmodel CreateThinPlateSplineTransform $thisparam($this,tpspoints) 2.0 1.5 2.0


    set ok [ $newtmodel Save $outfname ]



    if { $ok == 1 } {
	::pxtclutil::Info "Transformation saved in $outfname (model in $imgname)"
    } else {
	::pxtclutil::Warning "Failed to save transformation in $outfname"
    }

    $newtmodel Delete
    $newcurve Delete
   

}
# -------------------------------------------------------------------------------
itcl::body pxitclmousechopper::GenerateMouseMap { } {

    set image [ $parent cget -currentimage ]

    set polydatacontrol 0
    catch { set  polydatacontrol [ [ $parent GetPolyDataControl ] GetPolyDataControl ] }
    set surface 0 

    if { $polydatacontrol != 0 } {
	set surface [ vtkPolyData [ pxvtable::vnewobj ]]
    }
    
    if { [ $image GetImageSize ] > 16 } {
	set img [ $planes CreateRegionLabelImage $image $thisparam($this,resolution) $thisparam($this,kernelsize) $thisparam($this,threshold) $surface ]
    }
    
    if { $img != 0 } {
	[ $mouse_map_gui GetObject ] ShallowCopyImage $img
	[ $mouse_map_gui GetObject ] configure -filename  "model.hdr"
	[ $mouse_map_gui GetObject ] CopyImageHeader [ $image GetImageHeader ]
	$mouse_map_gui Update
    }

    if { $surface != 0 } {
	$polydatacontrol SetClipboardSurface $surface "model.vtk"
	::pxtclutil::Info "Placed surface model.vtk in the clipboard of the viewer surface control"
	[ $parent GetPolyDataControl ] ShowWindow
	$surface Delete
    }
    
    return 1
}
# -------------------------------------------------------------------------------

itcl::body pxitclmousechopper::InitializeModel { mode args } { 

    # Add requisite plane types

    if { $fullyinitialized == 0 } { $this DelayedInitialize }

    set img [ $parent GetDisplayedImage ]
    if { [ $img GetImageSize ] > 16 } {
	set dim [ [  $img  GetImage ] GetDimensions ]
	set spa [ [  $img  GetImage ] GetSpacing ]
	set d ""
	for { set ia 0 } { $ia <=2 } { incr ia } {
	    lappend d [ expr [ lindex $spa $ia ] * [ lindex $dim $ia ]]
	}
	$planes configure -dimensions $d
    }

    if { $mode == "hindlimbfoot" } {
	set ok [ $planes InitializeModel 1 1 0 0 0 0 ]
    } elseif { $mode == "hindlimb" } {
	set ok [ $planes InitializeModel 0 1 0 0 0 0 ]
    } elseif { $mode == "gui" } {
	set ok [ $planes InitializeModel $thisparam($this,hasfeet) $thisparam($this,hashindlimbs) $thisparam($this,hasforelimbs) $thisparam($this,hashead) $thisparam($this,taildivisions) $thisparam($this,spinedivisions)  ]
    }

    if { $ok ==1 } { $this UpdateGUIFromPlanes }
}

itcl::body pxitclmousechopper::Initialize { basewidg args } {

    if { $initialized == 1 } { return $basewidget }

    set basewidget [ toplevel $basewidg ] 
    wm withdraw $basewidget
    wm geometry $basewidget 570x350

    SetTitle $appname

    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    set fullyinitialized 0
    return $basewidget
}

itcl::body pxitclmousechopper::GetObliqueSlice {  } {

    if { $oblique_slice == 0 } {
	set orthoviewer [ $parent GetViewer ]
	set oblique_slice [ $orthoviewer GetObliqueImageSlice ]
	if { [ string length $oblique_slice ] < 2 } {
	    set oblique_slice 0
	}
    }
    return $oblique_slice
}

itcl::body pxitclmousechopper::DelayedInitialize {  } {


    if { $fullyinitialized ==1 } {
	return
    }

    set fullyinitialized 1

    set orthoviewer [ $parent GetViewer ]
    set oblique_gui [ $orthoviewer GetObliqueControl ]
    $this GetObliqueSlice

    set w [ frame $basewidget.bottom ]


    set notebook [ iwidgets::tabnotebook $basewidget.top  -tabpos w -width 350 ]
    pack $w -side bottom -expand f -fill x -pady 10
    pack $notebook  -side top -expand t -fill both

    $this CreateDefinitionsGUI [ $notebook add -label "Definition" ]
    $this CreatePlanesGUI [ $notebook add -label "Planes" ]
    $this CreateMapGUI [ $notebook add -label "VOI Map" ]
    $this CreateSegmentedMapGUI [ $notebook add -label "Segmentation" ]
    $this CreateOutputGUI  [ $notebook add -label "Output Model" ]
    $notebook view "Definition"

    eval "button $w.1 -text Close -command { $this HideWindow } "
    eval "button $w.load -text \"Load Planes\" -command { $this LoadModel \"\" } "
    eval "button $w.save -text \"Save Planes\" -command { $this SaveModel \"\" } "
    eval "button $w.init -text Initialize -command { $this InitializeModel hindlimbfoot }"


    pack $w.load $w.save $w.init -side left -padx 2
    pack $w.1 -side right

    $this InitializeModel "hindlimbfoot"

    
    set canvas [ [ $notebook component tabset ] component canvas ];     set l [ AddLogoLabel $canvas ];     pack $l -side bottom -padx 1 
}

# ------------------------------------------------------------------------------------------     
itcl::body pxitclmousechopper::ShowOblique {  } {
    
    catch { $oblique_gui Show 1 }
}

itcl::body pxitclmousechopper::CreateDefinitionsGUI { widget } {

    set thisparam($this,hashead) 0 
    set thisparam($this,hasfeet) 1
    set thisparam($this,hashindlimbs) 1
    set thisparam($this,hasforelimbs) 0
    set thisparam($this,taildivisions) 1
    set thisparam($this,spinedivisions) 2

    set b [ frame  $widget.b ] ; pack $widget.b -side bottom -expand f -fill x
    set base  [ LabelFrame:create $widget.a -text "Model Options"]
    pack $widget.a  -side top -fill x -expand f

    checkbutton $base.1 -variable [ itcl::scope thisparam($this,hashindlimbs) ] -text "Hind Limbs"
    checkbutton $base.2 -variable [ itcl::scope thisparam($this,hasfeet)  ] -text "Feet"
    checkbutton $base.3 -variable [ itcl::scope thisparam($this,hasforelimbs) ] -text "Fore Limbs"
    checkbutton $base.4 -variable [ itcl::scope thisparam($this,hashead) ] -text "Head"
    
    set cw [ [ iwidgets::labeledwidget $base.5  -labeltext "Tail Divisions:"  ] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,taildivisions) ] 0 1 2 3 4 5 
    pack $cw.optmenu  -expand f

    set cw [ [ iwidgets::labeledwidget $base.6 -labeltext "Spine Divisions:"  ] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,spinedivisions) ] 0 1 2 3 4 5 6 7 8 9 
    pack $cw.optmenu  -expand f

    pack $base.1 $base.2 $base.5 $base.6 $base.3 $base.4 -side top -expand f -padx 10 -pady 2


    eval "button $b.1 -text \"Hind-Limb\" -command { $this InitializeModel hindlimb } "
    eval "button $b.2 -text \"Hind-Limb-Foot\" -command { $this InitializeModel hindlimbfoot } "
    eval "button $b.3 -text \"Custom\" -command { $this InitializeModel gui } "
    pack $b.1 $b.2 $b.3 -side left -expand f -fill x

}

itcl::body pxitclmousechopper::CreatePlanesGUI { widget } {

    set thisparam($this,enable_updateplane) 1
    set thisparam($this,currentplaneindex) -1
    set thisparam($this,currentplane) "None"
    set thisparam($this,currentplane_n)    "1 0 0 "
    set thisparam($this,currentplane_c)    "0 0 0 "
    set thisparam($this,currentplane_dist)  "0.0"
    set thisparam($this,currentplane_above) 0

    set thisparam($this,currentplane_autoupdate) 1

    frame $widget.left
    frame $widget.right

    eval "iwidgets::scrolledlistbox $widget.left.1 -labeltext \"List of Planes\" \
			 -vscrollmode dynamic -hscrollmode none \
			 -selectmode single -exportselection 0 \
			 -labelpos nw -selectioncommand { $this SelectPlane -1 } "
    set planeslistbox $widget.left.1
    $planeslistbox  clear
    pack $widget.left -side left -expand f -fill y
    pack $widget.right -side right -expand t -fill both

    set bbar [ frame $widget.left.2 ] ; pack $bbar -side bottom -expand f -fill x
    pack $widget.left.1 -side top -expand t -fill both

    eval "button $bbar.0 -text \"Previous\" -command { $this SelectPlane -1 -1 } "
    eval "button $bbar.1 -text \"Next\" -command { $this SelectPlane -1 1 } "
    pack $bbar.0 $bbar.1 -side left -expand t -padx 5 -pady 2


    set base   [ LabelFrame:create $widget.right.1 -text "Current Plane"]
    set base2  [ LabelFrame:create $widget.right.2 -text "Operations"]
    pack $widget.right.2 -side bottom -expand f -fill x
    pack $widget.right.1 -side top -expand t -fill both

    set k 0
    iwidgets::entryfield $base.$k -labeltext "Plane Name:"  -width 15 -validate real -textvariable [ itcl::scope thisparam($this,currentplane) ] -relief sunken  -state disabled
    pack $base.$k -side top -expand f -fill x; incr k

    frame $base.$k 
    iwidgets::entryfield $base.$k.1 -labeltext "Normal:"  -width 15 -validate real -textvariable [ itcl::scope thisparam($this,currentplane_n) ] -relief sunken  -state disabled
    eval "checkbutton $base.$k.2 -variable [ itcl::scope thisparam($this,currentplane_above) ] -text Above -command { $this UpdateArrowColor }"
    pack $base.$k.1 $base.$k.2 -side left -expand f -fill x
    pack $base.$k -side top -expand f -fill x; incr k

    frame $base.$k 
    iwidgets::entryfield $base.$k.1 -labeltext "Center:"  -width 15 -validate real -textvariable [ itcl::scope thisparam($this,currentplane_c) ] -relief sunken  -state disabled
    iwidgets::entryfield $base.$k.2 -labeltext "Offset:"  -width 5 -validate real -textvariable [ itcl::scope thisparam($this,currentplane_dist) ] -relief sunken  -state disabled
    pack $base.$k.1 $base.$k.2 -side left -expand f -fill x
    pack $base.$k -side top -expand f -fill x; incr k
    

    checkbutton $base.$k -variable [ itcl::scope thisparam($this,currentplane_autoupdate) ] -text "Update Oblique Slice"
    pack $base.$k -side top -expand f -fill x;  incr k

    set w  [ frame $base.$k ];     pack $base.$k -side top -expand f -fill x;  incr k
    eval "button $w.0 -text \"Update Plane\" -command { $this UpdatePlane }"
    eval "button $w.1 -text \"Oblique Slice Control\" -command { $this ShowOblique } "

    pack $w.0 $w.1 -side left -expand f -fill x -pady 2


    set w2 [ frame $base2.$k ];     pack $base2.$k -side top -expand f -fill x;  incr k
    set w3 [ frame $base2.$k ];     pack $base2.$k -side top -expand f -fill x;  incr k


    eval "button $w2.1 -text \"Apply Plane\"   -command { $this TestPlane 0 }"
#    eval "button $w2.2 -text \"Apply Plane Pair\"   -command { $this TestPlane 1 }"
    eval "button $w2.2 -text \"Single Joint\"   -command { $this SingleJoint } "
    pack  $w2.1 $w2.2 -side left -expand f -padx 2 -fill x

    eval "button $w3.3 -text \"Extract Leg\"   -command { $this TestPlane 2 }"
    eval "button $w3.4 -text \"Extr. Leg (no foot)\"   -command { $this TestPlane 3 }"
    pack  $w3.3 $w3.4 -side left -expand f -padx 0 -fill x 
    
}

# -----------------------------------------------------------------------------------------
itcl::body pxitclmousechopper::CreateMapGUI { widget } {


    set bot [ frame $widget.f1 ]
    set f1 [ frame $widget.f2 ]

    pack $f1 $bot -side bottom -expand false -fill x -pady 2

    set mouse_map_gui [[ pxitclimageGUI \#auto  ] GetThisPointer ]
    ${mouse_map_gui} configure -description "Mouse VOI Map"
    ${mouse_map_gui} Initialize $widget.top
    ${mouse_map_gui} AddFunction "$parent SetMaskFromObject" "Display Mask" "$this"
    ${mouse_map_gui} AddFunction "$parent SetResultsFromObject" "Display" "$this"
    $widget.top configure -background darkgray
    pack $widget.top -side top -expand f -fill x -pady 3

    pack forget [ ${mouse_map_gui} cget -browsebutton ]

    set thisparam($this,kernelsize) 2.0
    set thisparam($this,threshold)  200.0
    set thisparam($this,resolution)  2.0

    eval "button $f1.d -text \"Generate Mouse Map\"   -command { $this GenerateMouseMap } "
    pack $f1.d   -side left -padx 5 -expand f -fill x


    label $bot.a -text "Sigma"
    tk_optionMenu $bot.b [ itcl::scope thisparam($this,kernelsize) ]  "0.1" "0.5" "1.0" "1.5" "2.0" "3.0" "5.0" 
    label $bot.a1 -text "Resol"
    tk_optionMenu $bot.b1 [ itcl::scope thisparam($this,resolution) ]  "0.5" "1.0" "1.5" "2.0" "3.0" "4.0"
    iwidgets::entryfield $bot.c -labeltext "Threshold:"  -width 5 -validate real -textvariable [ itcl::scope thisparam($this,threshold) ] -relief sunken 
    pack $bot.a $bot.b $bot.a1 $bot.b1 $bot.c -side left -expand f -padx 2
}
# -----------------------------------------------------------------------------------------
itcl::body pxitclmousechopper::UpdateLabelImageRanges { } {

    set image  [ $mouse_label_gui GetObject ] 
    if { [ $image GetImageSize ] < 2 } {
	return 
    }

    set r [ [ [ [ $image GetImage ] GetPointData ] GetScalars ] GetRange ]
    set rmax [ lindex $r 1 ]
    set rmin [ lindex $r 0 ]

    if { $rmax > 20 } { 
	set rmax 20
    }

    set thisparam($this,boneclass) $rmax
    if { $rmax <= 2 } {
	set thisparam($this,softtissuemin) 1
	set thisparam($this,softtissuemax) 1
    } else {
	set thisparam($this,softtissuemin) 2
	set thisparam($this,softtissuemax) [ expr $rmax -1 ]
    }
}

itcl::body pxitclmousechopper::GrabSegmentationMapFromViewer { args } {

    set img [ $parent GetDisplayedImage ]
    if { [ $img GetImageSize ] < 16 } {
	::pxtclutil::Warning "No Segmentation Output Image in the Viewer"
	return
    }

    set r [ [ [ [ $img GetImage ] GetPointData ] GetScalars ] GetRange ]
    set rmax [ lindex $r 1 ]
    set rmin [ lindex $r 0 ]
    if { $rmin !=0 || $rmax>10 } {
	::pxtclutil::Warning "Bad Segmented Mask .. expecting image with three levels 0=air, 1=soft tissue, ... N=bone"
	return
    }
    
    [ $mouse_label_gui GetObject ] ShallowCopy $img
    $mouse_label_gui Update
    $this UpdateLabelImageRanges

}
# -----------------------------------------------------------------------------------------
itcl::body pxitclmousechopper::ExtractAndResliceLabelMap { tissuelabel   tissuelabel2 suffix { dilateerode 0 } } {


    set map [ $mouse_map_gui GetImage ]
    if { [ $map  GetImageSize ] < 2 } {
	::pxtclutil::Warning "No Mouse Map Image in Memory"
	return
    }

    set lab [ $mouse_label_gui GetImage ]
    if { [ $map  GetImageSize ] < 2 } {
	::pxtclutil::Warning "No Mouse Label Image in Memory"
	return
    }

    set thr [ vtkImageThreshold [ pxvtable::vnewobj ]]

    if { [ $map  GetImageSize ] != [ $lab GetImageSize ] } {
	puts stderr "Reslicing Label Map\n"
	set resl [ vtkbisImageReslice [ pxvtable::vnewobj ]]
	$resl SetInput [ $lab GetImage ]
	$resl SetInformationInput [ $map GetImage ]
	$resl SetInterpolationMode 0
	$resl Update
	$thr SetInput [ $resl GetOutput ]
	$resl Delete
    } else {
	$thr SetInput [ $lab GetImage ]
    }

    $thr DebugOn
    puts stderr "Thresholding $tissuelabel $tissuelabel2"
    $thr ThresholdBetween $tissuelabel $tissuelabel2
    $thr SetOutputScalarType [ [ $map GetImage ] GetScalarType ]
    $thr DebugOff
    $thr ReplaceInOn
    $thr ReplaceOutOn
    $thr SetInValue  1.0
    $thr SetOutValue 0.0
    $thr Update

    set image [ [ pxitclimage \#auto ] GetThisPointer ]

    if { $dilateerode == 0 } {
	$image ShallowCopyImage [ $thr GetOutput ]
    } elseif { $dilateerode > 0 } {
	puts stderr "Dilating $dilateerode"
	set dilate  [  vtkImageContinuousDilate3D [ pxvtable::vnewobj ] ]
	$dilate SetInput [ $thr GetOutput ]
	$dilate SetKernelSize $dilateerode $dilateerode $dilateerode
	$dilate Update
	$image ShallowCopyImage [ $dilate GetOutput ]
	$dilate Delete
    } else {
	set dilateerode [ expr -$dilateerode ]
	puts stderr "Eroding $dilateerode"
	set erode  [  vtkImageContinuousErode3D [ pxvtable::vnewobj ] ]
	$erode SetInput [ $thr GetOutput ]
	$erode SetKernelSize $dilateerode $dilateerode $dilateerode
	$erode Update
	$image ShallowCopyImage [ $erode GetOutput ]
	$erode Delete
    }

    $image CopyImageHeader [  $lab GetImageHeader ]
    set fname [  $lab cget -filename ]

    set fname "[ file root $fname]_${suffix}.hdr"

    puts stderr "Filename $fname"
    $image configure -filename $fname
    $thr Delete
    return $image
}
# -----------------------------------------------------------------------------------------
itcl::body pxitclmousechopper::CreateSoftTissueVOI { } {

    set map [ $mouse_map_gui GetImage ]
    if { [ $map  GetImageSize ] < 2 } {
	::pxtclutil::Warning "No Mouse Map Image in Memory"
	return
    }

    set lab [ $mouse_label_gui GetImage ]
    if { [ $map  GetImageSize ] < 2 } {
	::pxtclutil::Warning "No Mouse Label Image in Memory"
	return
    }

    set rmin $thisparam($this,softtissuemin) 
    set rmax $thisparam($this,softtissuemax) 
    
    set resllab [ $this ExtractAndResliceLabelMap $rmin $rmax "softtisue_${rmin}_${rmax}" $thisparam($this,dilateerode) ]

    set m [ vtkImageMathematics [ pxvtable::vnewobj ]]
    $m SetInput1 [ $resllab GetImage ]
    $m SetInput2 [ $map GetImage  ]
    $m SetOperationToMultiply
    $m Update

    set image [ [ pxitclimage \#auto ] GetThisPointer ]
    $image ShallowCopyImage [ $m GetOutput ]
    $image CopyImageHeader [  $map GetImageHeader ]

    set fname [  $map cget -filename ]
    set fname "[ file root $fname]_nobone.hdr"

    $image configure -filename $fname
    $parent SetResultsFromObject $image $this
    itcl::delete object $image
    catch { itcl::delete object $reslab }

    $m Delete

    return
}

# -----------------------------------------------------------------------------------------
itcl::body pxitclmousechopper::CreateBoneVOI { } {

    set map [ $mouse_map_gui GetImage ]
    if { [ $map  GetImageSize ] < 2 } {
	::pxtclutil::Warning "No Mouse Map Image in Memory"
	return
    }

    set lab [ $mouse_label_gui GetImage ]
    if { [ $map  GetImageSize ] < 2 } {
	::pxtclutil::Warning "No Mouse Label Image in Memory"
	return
    }

    set rmax $thisparam($this,boneclass) 
    set resllab [ $this ExtractAndResliceLabelMap $rmax $rmax "bone" $thisparam($this,dilateerode) ]

    $parent SetResultsFromObject $resllab $this

    catch { itcl::delete object $reslab }

    return
}

# -----------------------------------------------------------------------------------------
itcl::body pxitclmousechopper::GetJoint { jointno map bonemap } {

    # First Find The plane number of the joint = $jointno
    set planeno [ $planes GetPlaneNumber $jointno ]
    SelectPlane  $planeno

    # First Get The Oblique slice 
    set thisparam($this,enable_updateplane) 1
    set doupd $thisparam($this,currentplane_autoupdate) 
    set thisparam($this,currentplane_autoupdate)  1


    # ----------------------------------------------------------------
    # Next --- threshold and dilate the map for the region of interest
    # ----------------------------------------------------------------

    set thr [ vtkImageThreshold [ pxvtable::vnewobj ]]
    $thr SetInput [  $map GetImage ]
    puts stderr "\n Extracting From map $jointno:$jointno  for planeno $planeno : $thisparam($this,currentplane) Joint=$jointno"
    $thr ThresholdBetween $jointno $jointno
    $thr ReplaceInOn
    $thr ReplaceOutOn
    $thr SetInValue  100.0
    $thr SetOutValue 0.0

    set dilate  [  vtkImageContinuousDilate3D [ pxvtable::vnewobj ] ]
    $dilate SetInput [ $thr GetOutput ];     
    $dilate SetKernelSize 5 5 5

    set m [ vtkImageMathematics [ pxvtable::vnewobj ]]
    $m SetInput1 [ $dilate GetOutput ]
    $m SetInput2 [ $bonemap GetImage ]
    $m SetOperationToMultiply
    $m Update

    # ---------------------------------------------------------------
    # Next Display the Image
    # ---------------------------------------------------------------
    set tmp [ [ pxitclimage \#auto ] GetThisPointer ]
    $tmp ShallowCopyImage [ $m GetOutput ]
    $tmp CopyImageHeader [ $map GetImageHeader ]
    $thr Delete
    $dilate Delete
    $m Delete

    $parent SetResultsFromObject $tmp $this

    puts stderr "Selecting plane $planeno"
    SelectPlane  $planeno

    set oblmap [ vtkImageData [ pxvtable::vnewobj ]] 
    $oblmap ShallowCopy [ $oblique_slice GetObliqueImage ]


    set sim [ vtkpxTransformationUtil [ pxvtable::vnewobj ]]
    set newpts [ $sim GetCentroids $oblmap $thisparam($this,axislength) ]
    $sim Delete

    
    set xform  [ $oblique_slice GetObliqueTransform ] 


    for { set i 0 } { $i < 3 } { incr i } {
	set pt [ $newpts GetPoint $i ]
	set tpt [ $xform TransformPoint [ lindex $pt 0 ] [ lindex $pt 1 ] [ lindex $pt 2 ] ]
	$newpts SetPoint $i [ lindex $tpt 0 ] [ lindex $tpt 1 ] [ lindex $tpt 2 ]
    }

    # ----- Restore Automatic Update settings

   
    $oblmap Delete
    itcl::delete object $tmp

    set thisparam($this,currentplane_autoupdate)  $doupd
    set thisparam($this,enable_updateplane) 1
    
    return $newpts
}

# -----------------------------------------------------------------------------------------
itcl::body pxitclmousechopper::SingleJoint { } {

    set oblique_image 0

    catch {
	set dim [  [ $oblique_slice GetObliqueImage ] GetDimensions ]
	if { [ lindex $dim 0 ] > 1 } {
	    set oblique_image 1
	}
    }
    
    if { $oblique_image == 0 } {
	::pxtclutil::Warning "Oblique Slice is Uninitialized"
	return 0
    }
    
    
    set map [ $mouse_map_gui GetImage ]
    if { [ $map  GetImageSize ] < 2 } {
	::pxtclutil::Warning "No Mouse Map Image in Memory"
	return 0
    }
    
    set lab [ $mouse_label_gui GetImage ]
    if { [ $map  GetImageSize ] < 2 } {
	::pxtclutil::Warning "No Mouse Label Image in Memory"
	return 0
    }

    set rmax $thisparam($this,boneclass) 
    set bonemap [ $this ExtractAndResliceLabelMap $rmax $rmax "bone" 0 ]
    
    set r [ [ [ [ $map GetImage ] GetPointData ] GetScalars ] GetRange ]
    set rmax [ lindex $r 1 ]
    set rmin [ lindex $r 0 ]
    
    set numjoints [ expr $rmax - $rmin ]
    if { $numjoints < 1 } { 
	::pxtclutil::Warning "Bad Label Image in Memory $rmin:$rmax"
	return 0
    }

    set planeno $thisparam($this,currentplaneindex)
    set jointno [ $planes GetJointNumber $planeno ]

    puts stderr "\nCurrent  Joint = $thisparam($this,currentplane) (index=$planeno) --> $jointno "

    set interp [ $oblique_slice GetInterpolation ]
    $oblique_slice SetInterpolation 0


    set newpts [ $this GetJoint $jointno $map $bonemap ]
	
    set curve [ vtkpxBaseCurve [ pxvtable::vnewobj ]]
    $curve SetFromPoints $newpts
    $newpts Delete

    set landmarkcontrol [ $parent GetLandmarkControl ] 
    [ $landmarkcontrol GetLandmarkControl ]  SetCollection $curve -1
    $landmarkcontrol ShowWindow

    $curve Delete

    $oblique_slice SetInterpolation $interp

    #    $parent SetResultsFromObject $bonemap $this
    #   itcl::delete object $bonemap

    return 1
}

# -----------------------------------------------------------------------------------------
itcl::body pxitclmousechopper::AutoJoints { } {

    set oblique_image 0


    catch {
	set dim [  [ $oblique_slice GetObliqueImage ] GetDimensions ]
	if { [ lindex $dim 0 ] > 1 } {
	    set oblique_image 1
	}
    }
    
    if { $oblique_image == 0 } {
	::pxtclutil::Warning "Oblique Slice is Uninitialized"
	return 0
    }
    
    
    set map [ $mouse_map_gui GetImage ]
    if { [ $map  GetImageSize ] < 2 } {
	::pxtclutil::Warning "No Mouse Map Image in Memory"
	return 0
    }
    
    set lab [ $mouse_label_gui GetImage ]
    if { [ $map  GetImageSize ] < 2 } {
	::pxtclutil::Warning "No Mouse Label Image in Memory"
	return 0
    }
    
    set rmax $thisparam($this,boneclass) 
    set bonemap [ $this ExtractAndResliceLabelMap $rmax $rmax "bone" 0 ]
    
    set r [ [ [ [ $map GetImage ] GetPointData ] GetScalars ] GetRange ]
    set rmax [ lindex $r 1 ]
    set rmin [ lindex $r 0 ]
    
    set numjoints [ expr $rmax - $rmin ]
    if { $numjoints < 1 } { 
	::pxtclutil::Warning "Bad Label Image in Memory $rmin:$rmax"
	return 0
    }

     $this ClickJoints "auto" 

    

    set points [ vtkPoints [ pxvtable::vnewobj ]]

    set old  $thisparam($this,currentplane_autoupdate)
    set thisparam($this,currentplane_autoupdate) 1

    set interp [ $oblique_slice GetInterpolation ]
    $oblique_slice SetInterpolation 0

    
    $points SetNumberOfPoints [ expr $numjoints * 3 ] 
    $points SetPoint 0 100 100 100
    $points SetPoint 1 101 100 100
    $points SetPoint 2 100 101 100
    set pointindex 3
    
    puts stderr "Creating Joints $numjoints"
    
    for { set i 1 } { $i < $numjoints } { incr i } {
	set pts [ GetJoint $i $map $bonemap ]
	
	for { set j 0 } { $j <= 2 } { incr j } {
	    $points SetPoint $pointindex [ lindex [ $pts GetPoint $j ] 0 ] [ lindex [ $pts GetPoint $j ] 1 ] [ lindex [ $pts GetPoint $j ] 2 ]
	    incr pointindex
	}
	$pts Delete
    }
    
    set p     [ $planes GetModelParameters ]
    set hashindlimbs [ lindex $p 1 ]
    set hasfeet      [ lindex $p 0 ]
    
    set pt(0) 0.0;
    set pt(1) 0.0;
    set pt(2) 0.0;
    set nump 0.001
    
    
    if { $hashindlimbs == 1 } {
	set index1 3 
	set index2 12
	if { $hasfeet == 0 } {
	    set index2 9
	}
	
	set pt1 [ $points GetPoint 3 ]
	set pt2 [ $points GetPoint 12 ]
	
	for { set j 0 } { $j <=2 } { incr j } {
	    set pt($j) [ expr 0.5* ( [ lindex $pt1 $j ] + [ lindex $pt2 $j ] ) ]
	}
	
    } else {
	for { set j 1 } { $j < $numjoints } { incr j } {
	    set pt [ $points GetPoint [ expr $i * 3 ] ]
	    for { set j 0 } { $j <=2 } { incr j } {
		set pt($j) [ expr $pt($j) + [ lindex $pt $j ] ]
		incr nump
	    }
	    for { set j 0 } { $j <=2 } { incr j } {
		set pt($j) [ expr $pt($j) / $nump ]
	    }
	}
    }
    
    $points SetPoint 0 $pt(0) $pt(1) $pt(2)
    $points SetPoint 1 [ expr $thisparam($this,axislength) + $pt(0)  ] $pt(1) $pt(2)
    $points SetPoint 2 $pt(0) [ expr $pt(1) + $thisparam($this,axislength)  ] $pt(2)
    	
    set curve [ vtkpxBaseCurve [ pxvtable::vnewobj ]]
    $curve SetFromPoints $points

    set landmarkcontrol [ $parent GetLandmarkControl ] 
    [ $landmarkcontrol GetLandmarkControl ]  SetCollection $curve -1
    $landmarkcontrol ShowWindow

    $curve Delete

    $oblique_slice SetInterpolation $interp
    $points Delete

    set thisparam($this,currentplane_autoupdate) $old

    $parent SetResultsFromObject $bonemap $this
    SelectPlane 0

    itcl::delete object $bonemap

    return 1

}
# -----------------------------------------------------------------------------------------

itcl::body pxitclmousechopper::CreateSegmentedMapGUI { widget } {

    set thisparam($this,dilateerode) 0
    set thisparam($this,softtissuemin) 1
    set thisparam($this,softtissuemax) 4
    set thisparam($this,boneclass) 5

    set bot [ frame $widget.f1 ]
    set fr1 [ frame $widget.f2 ]
    set fr2 [ frame $widget.f3 ]
    pack  $bot $fr2 $fr1 -side bottom -expand false -fill x -pady 2

    set mouse_label_gui [[ pxitclimageGUI \#auto  ] GetThisPointer ]
    ${mouse_label_gui} configure -description "Mouse VOI Map"
    ${mouse_label_gui} Initialize $widget.top
    pack forget [ ${mouse_label_gui} cget -browsebutton ]

    ${mouse_label_gui} AddFunction "$this GrabSegmentationMapFromViewer" "Grab" "$this"
    ${mouse_label_gui} AddFunction "$parent SetMaskFromObject" "Display Mask" "$this"
    ${mouse_label_gui} AddFunction "$parent SetResultsFromObject" "Display" "$this"
    $widget.top configure -background darkgray
    pack $widget.top -side top -expand f -fill x -pady 3
    
    set cw [ [ iwidgets::labeledwidget $bot.1  -labeltext "Erosion/Dilation:"  ] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,dilateerode) ] -5 -3 0  3  5
    pack $cw.optmenu  -expand f
    
    set k 0
    eval "button $fr2.$k -text Update -command { $this UpdateLabelImageRanges }"
    pack $fr2.$k -side right -expand f -fill x; incr k

    iwidgets::entryfield $fr2.$k -labeltext "Muscle Min:"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,softtissuemin) ] -relief sunken 
    pack $fr2.$k -side left -expand f -fill x; incr k
    iwidgets::entryfield $fr2.$k -labeltext "Muscle Max:"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,softtissuemax) ] -relief sunken  
    pack $fr2.$k -side left -expand f -fill x; incr k
    iwidgets::entryfield $fr2.$k -labeltext "Bone Class:"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,boneclass) ] -relief sunken  
    pack $fr2.$k -side left -expand f -fill x; incr k



    eval "button $fr1.1 -text \"Show Segmentation Control\" -command { $this ShowSegmentationControl }"
    pack $fr1.1 -side left -padx 10 -expand t -fill x

    eval "button $bot.2 -text \"Bone VOI\"   -command { $this CreateBoneVOI } "
    eval "button $bot.3 -text \"Soft Tissue VOI\"   -command { $this CreateSoftTissueVOI } "
    pack $bot.3 $bot.2 -side right -padx 1 -expand f
    pack $bot.1  -side left -padx 1 -expand t -fill x
}
# -----------------------------------------------------------------------------------------
itcl::body pxitclmousechopper::ShowSegmentationControl { } {

    set segm [ $parent cget -brainstriptool ]
    $segm ShowWindow "Histogram Segmentation"
    
}
# -----------------------------------------------------------------------------------------
itcl::body pxitclmousechopper::CreateOutputGUI { widget } {

    set thisparam($this,distanceoffset) 1.5
    set thisparam($this,axislength) 3.0
    set thisparam($this,tpspoints)  150
    set thisparam($this,tuberadius)     0.5
    set thisparam($this,sphereradius)   1.0

    set base  [ LabelFrame:create $widget.a -text "Joint Locations"]
    pack  $widget.a -side top -expand false -fill x -pady 2
    
    set fr1 [ frame $base.1 ]
    set fr2 [ frame $base.2 ]
    pack $fr1 $fr2 -side top -expand f -fill x
    
    label $fr1.a -text "Joint Offset (voxels)"
    tk_optionMenu $fr1.b [ itcl::scope thisparam($this,distanceoffset) ] "0.0"  "1.0" "1.5" "2.0" "3.0" "5.0" 
    label $fr1.c -text "Axis Length"
    tk_optionMenu $fr1.d [ itcl::scope thisparam($this,axislength) ]  "1.0" "1.5" "2.0" "3.0" "5.0" "10" "20.0"

    pack $fr1.a $fr1.b $fr1.c $fr1.d  -side left -expand f -padx 0

    eval "button $fr2.1 -text \"Click Joints\"   -command { $this ClickJoints manual } "
    eval "button $fr2.2 -text \"Set All Joints\"   -command { $this AutoJoints  } "
    pack $fr2.1 $fr2.2  -side left -expand f -fill x -padx 5


    set base  [ LabelFrame:create $widget.b -text "Wireframe Model"]
    pack  $widget.b -side top -expand false -fill x -pady 2
    
    
    label $base.a -text "Tube Radius"
    tk_optionMenu $base.b [ itcl::scope thisparam($this,tuberadius) ] "0.0" "0.25" "0.5" "0.75" "1.0" "2.0" 
    label $base.c -text "Sphere Radius"
    tk_optionMenu $base.d [ itcl::scope thisparam($this,sphereradius) ] "0.25" "0.5" "1.0" "1.5" "2.0" "3.0" "4.0"
    eval "button $base.e -text \"Wire Model\"   -command { $this GenerateWireModel } "
    pack $base.a $base.b $base.c $base.d $base.e -side left -expand f -fill x -padx 1

    
    set test 0
    catch  {  set test [ vtkpxPolySimilarityTransform [ pxvtable::vnewobj ] ] }
    if { [ string length $test ] > 2 } {
	set f3  [ LabelFrame:create $widget.c -text "Transformation Model"]
	pack  $widget.c -side top -expand false -fill x -pady 2
	
	label $f3.1 -text "TPS Points:"
	tk_optionMenu $f3.2 [ itcl::scope thisparam($this,tpspoints) ] "75" "100" "125" "150" "200" "250"
	eval "button $f3.3 -text \"Create Transformation\"   -command { $this GenerateTransformationModel } "
	pack $f3.1 $f3.2 $f3.3 -side left -padx 5 -expand f -fill x
    }

}
# -----------------------------------------------------------------------------------------

itcl::body pxitclmousechopper::UpdateGUIFromPlanes { } { 

    $planeslistbox  clear
    set names [ $planes GetNames ]
    set p     [ $planes GetModelParameters ]

    for { set i 0 } { $i < [ llength $names ] } { incr i } {
	$planeslistbox insert end [ lindex $names $i ]
    }

    $planeslistbox selection set   0 0



    set thisparam($this,hasfeet) [ lindex $p 0 ]
    set thisparam($this,hashindlimbs) [ lindex $p 1 ]
    set thisparam($this,hasforelimbs) [ lindex $p 2 ]
    set thisparam($this,hashead) [ lindex $p 3 ]
    set thisparam($this,taildivisions) [ lindex $p 4 ]
    set thisparam($this,spinedivisions) [ lindex $p 5 ]
   

    $this SelectPlane 
} 

itcl::body pxitclmousechopper::UpdatePlanesFromGUI { } { 

    if { $thisparam($this,enable_updateplane) == 1 } {
	set oldindex $thisparam($this,currentplaneindex)
	
	if { $oldindex != -1 } {
	    set parray [ $planes GetPlanes ]
	    $parray SetComponent $oldindex 7 $thisparam($this,currentplane_above) 
	}
    }

}
    

itcl::body pxitclmousechopper::SelectPlane { { value -1 } { direction 0 } } { 

    set maxindex [ llength [ $planes GetNames ] ]

    if { $value == - 1 } {
	set index [ $planeslistbox curselection ]
    } else {
	set index $value
	$planeslistbox selection clear 0 end 
	$planeslistbox selection set $index
    }

    set newindex $index

    if { $direction == -1 && $index > 0 } {
	set newindex [ expr $index -1 ]
    } elseif { $direction == 1 && $index < [ expr $maxindex - 1 ] } {
	set newindex [ expr $index +1 ]
    } else { 
	set newindex $index
    }
    
    
    UpdatePlanesFromGUI
    
    if { $newindex != $index } {
	$planeslistbox selection clear 0 end
	$planeslistbox selection set $newindex
	set index $newindex
    }
    
    
    set parray [ $planes GetPlanes ]
    set thisparam($this,currentplane_n)  [ list [ $parray GetComponent $index 0 ]  [ $parray GetComponent $index 1 ]  [ $parray GetComponent $index 2 ]  ]
    set thisparam($this,currentplane_dist)  [ $parray GetComponent $index 3 ] 
    set thisparam($this,currentplane_c)  [ list [ $parray GetComponent $index 4 ]  [ $parray GetComponent $index 5 ]  [ $parray GetComponent $index 6 ]  ]

    set thisparam($this,currentplane_above) [ expr [ $parray GetComponent $index 7 ] > 0 ]
    set thisparam($this,currentplane) 	[ $planeslistbox getcurselection ]
    set thisparam($this,currentplaneindex) $index

    if { $thisparam($this,currentplane_autoupdate) > 0 } {

	if { [ $this GetObliqueSlice  ] == 0 } { return }
	$oblique_slice UpdateImagePlane [ $parray GetComponent $index 3 ] [ $parray GetComponent $index 0 ]  [ $parray GetComponent $index 1 ]  [ $parray GetComponent $index 2 ] 
	$oblique_gui    UpdateScales
	$this UpdateArrowColor
	   
    }
}

itcl::body pxitclmousechopper::UpdateArrowColor { } {  

    if { $thisparam($this,currentplane_autoupdate) > 0 } {
	if { [ $this GetObliqueSlice  ] == 0 } {  return }
	$oblique_slice SetNormalArrowColor $thisparam($this,currentplane_above) 
	$oblique_slice SetShowNormalArrow [ $oblique_slice GetShowNormalArrow ]
	[ $parent GetViewer ]     UpdateDisplay
    }
}

itcl::body pxitclmousechopper::TestPlane { mode } { 
    
    set d [ [ $parent cget -currentimage ] GetImageSize ]
    if { $d < 2 } { 
	return
    }

    if { [ $this GetObliqueSlice  ] == 0 } { return }
    

    if { $mode == 0 } {

	set img [ $oblique_slice GetMaskImage [ [ $parent cget -currentimage ] GetImage ] $thisparam($this,currentplane_above) 0 ]
	set image [ [ pxitclimage \#auto ] GetThisPointer ]
	$image ShallowCopyImage $img; $img Delete
	$image CopyImageHeader [ [ $parent cget -currentimage ] GetImageHeader ]
	$image configure -filename "test_oblique"
	$parent SetResultsFromObject $image $this
	itcl::delete object $image
	return
    }

    if { $mode == 1 } {

	set test [ string first "Foot" $thisparam($this,currentplane) ]
	if { $test != -1 } {
	    ::pxtclutil::Warning "This operation does not apply to a foot plane"
	    return 
	}

	set img2 [ $oblique_slice GetMaskImage [ [ $parent cget -currentimage ] GetImage ] $thisparam($this,currentplane_above) 0 ]
	SelectPlane -1 1
	set img [ $oblique_slice GetMaskImage  $img2 $thisparam($this,currentplane_above) 0 ]
	SelectPlane -1 -1
	$img2 Delete

	set image [ [ pxitclimage \#auto ] GetThisPointer ]
	$image ShallowCopyImage $img; $img Delete
	$image CopyImageHeader [ [ $parent cget -currentimage ] GetImageHeader ]
	$image configure -filename "test_oblique"
	$parent SetResultsFromObject $image $this
	itcl::delete object $image
	return
    }
   
    set test [ string first "Horizontal" $thisparam($this,currentplane) ]
    if { $test == -1 } {
	::pxtclutil::Warning "Please select a 'horizontal' plane before using TestPlane pair. This option is meant to be used to create a VOI based on one of the limbs. Select a 'first' hipp or shoulder plane first and try again."
	return 
    }

    set ind $thisparam($this,currentplaneindex)
    set indp [ expr $ind +1 ]

    set sim [ vtkpxTransformationUtil [ pxvtable::vnewobj ]]
    set img1 [ $sim GetMaskImage [ [ $parent cget -currentimage ]  GetImage ] [ $planes GetPlanes ] $ind ]
    set img2 [ $sim GetMaskImage $img1 [ $planes GetPlanes ] $indp ]
    $img1 Delete

    set param [ $planes GetModelParameters ] 
    set hasfeet [ lindex $param 0 ]
    if { $mode == 3 && $hasfeet == 1 } {
	set img3 $img2
	set indf [ expr $ind +3 ]
	set img2 [ $sim GetMaskImage $img3 [ $planes GetPlanes ] $indf 1 ]
	$img3 Delete
    }

    $sim Delete
   
    set image [ [ pxitclimage \#auto ] GetThisPointer ]
    $image ShallowCopyImage $img2; $img2 Delete
    $image CopyImageHeader [ [ $parent cget -currentimage ] GetImageHeader ]
    $image configure -filename "test_leg"
    $parent SetResultsFromObject $image $this
    itcl::delete object $image
    return
}
	       

itcl::body pxitclmousechopper::UpdatePlane { } { 

    if { $thisparam($this,enable_updateplane) == 1 } {

	if { [ $this GetObliqueSlice  ] == 0 } { return }

	set thisparam($this,currentplane_n)    [  $oblique_slice GetNormal ]
	set thisparam($this,currentplane_c)    [ $oblique_slice GetCenter ]
	set thisparam($this,currentplane_dist) [ $oblique_slice GetDistance ]
	
	set index $thisparam($this,currentplaneindex) 
	set parray [ $planes GetPlanes ]
	for { set i 0 } { $i <= 2 } { incr i } {
	    $parray SetComponent $index $i [ lindex $thisparam($this,currentplane_n) $i ]
	    $parray SetComponent $index [ expr 4+ $i ] [ lindex $thisparam($this,currentplane_c) $i ]
	}
	$parray SetComponent $index 3 $thisparam($this,currentplane_dist)
    } 

}
    

if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    
    source [ file join [ file dirname [ info script ] ] pxitclmousesegment.tcl ]
}


