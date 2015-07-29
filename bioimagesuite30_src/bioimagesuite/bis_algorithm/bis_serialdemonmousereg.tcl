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

package require bis_resliceimage 1.0
package provide bis_serialdemonmousereg 1.0

#
# blend image
#

itcl::class bis_serialdemonmousereg {

    inherit bis_algorithm

     constructor { } {
	 $this Initialize
     }

    public method Initialize { }
    public method Execute { }
    public method GetGUIName { } { return "Serial Demons Mouse Registration"}


    # Debug/Utility Methods
    # ---------------------
    protected method SaveImage { img fname }
    protected method SaveTransformation { img fname }
    protected method ResliceImage { ref input trans interp }
    protected method  CombineTransformations { reference t1 t2 t3 } 
    protected method GenerateReducedImage { itclimg factor } 
    public method  UpdateOutputFilenames { } 
}




# -----------------------------------------------------------------------------------------
# Initialize
# ----------------------------------------------------------------------------------------

itcl::body bis_serialdemonmousereg::Initialize { } {

    PrintDebug "bis_serialdemonmousereg::Initialize" 

    set options {
	{ symmetric      "Symmetric or Non-symmetric" "Symmetric Mode"  boolean 1 { 0 1 } 1} 
	{ debug          "Debug Output On/Off" "Debug"  boolean 0 { 0 1 } 2} 
	{ reduction      "Perform computations on reduced image size " "Reduction"  integer 1 { 1 12 } 3 } 
    }

    set inputs { 
	{ image_ref           "Reference Image" pxitclimage  ""  20 }   
	{ binary_image_ref    "Reference Image Objectmap" pxitclimage  ""  20 }   
	{ image_trg   "Transform Image" pxitclimage  ""  25 }   
	{ binary_image_trg   "Transform Image Objectmap " pxitclimage  ""  30 }   
    }

    set outputs { 
	{ transformation "Transformation" pxitcltransform  ""  1 }   
	{ warped_target  "Resliced Target Image" pxitclimage  ""  101 }   
    }

    set defaultsuffix { "_output" }
    
    set scriptname bis_serialdemonmousereg
    set completionstatus "Done -- needs testing"
    #
    #document
    #

    set category "Registration"
    set description "Serial demon registration"
    set description2 "This is the nonrigid whole body mouse registration algorithm from Suh et al MICCAI 2009."
    set backwardcompatibility ""
    set authors "alark.joshi@yale.edu, jung.suh@yale.edu, dustin.scheinost@yale.edu, hirohito.okuda@yale.edu, xenophon.papademetris@yale.edu."

    $this AddDefaultOptions
}

# --------------------------------------------------------------------------------------
itcl::body bis_serialdemonmousereg::UpdateOutputFilenames { } {


    set f1 [ [ $this GetOutputObject transformation ] cget -filename ]

    if { [ string length $f1 ] > 1 } {
	return 1
    }

    set f(0) [ $InputsArray(image_ref) GetFileName ]
    set f(1) [ $InputsArray(image_trg) GetFileName ]
    
    for { set i 0 } { $i <= 1 } { incr i } {
	set l($i) [ ::bis_common::SplitFileNameGZ $f($i) ]
	set f($i) [ file tail [ lindex $l($i) 0 ] ]
    }
    
    set fname [ file join [ file dirname $f(0) ] "$f(0)_$f(1)" ]
    set outlist [ $this CreateDefaultOutputFileList $fname ]
    
    set force 0;    if { $guimode == "managed" } {	set force 1    }    
    $OutputsArray(transformation) SetFileName "[ lindex $outlist 0 ].vtk" $force
    puts stderr "[ lindex $outlist 0 ].vtk"

    return 1
}


# --------------------------------------------------------------------------------------
itcl::body bis_serialdemonmousereg::SaveImage { img fname } {

    set w [ vtkpxAnalyzeImageWriter New ]
    $w SetInput $img
    $w SetForceOutputFileFormat 2
    $w Save ${fname}.nii.gz
    puts stderr "Image Saved in $fname"
    
    $w Delete

    return
}

# --------------------------------------------------------------------------------------
itcl::body bis_serialdemonmousereg::SaveTransformation { tr fname } {

    # Save as .grd
    $tr Save ${fname}.grd.gz

    # Save as .vtk
    set w [ vtkStructuredPointsWriter New ]
    $w SetInput [ $tr GetDisplacementGrid ]
    $w SetFileTypeToBinary
    $w SetFileName ${fname}.vtk
    $w Write
    puts stderr "Transformation Saved in ${fname}.vtk"
    $w Delete

    return
}

#----------------------------------------------------------------------------------------
itcl::body bis_serialdemonmousereg::ResliceImage { reference input transformation interp } {

    # Reslice input to look like reference using transformation

    puts stderr "Reslicing Image interpolation = $interp"

    set w [ vtkImageReslice New ]
    $w SetInput $input
    $w SetInformationInput $reference
    $w SetResliceTransform $transformation
    $w SetInterpolationMode $interp
    $w Update

    set outimg [ vtkImageData New ]
    $outimg ShallowCopy [ $w GetOutput ]

    $w Delete

    return $outimg
}
# --------------------------------------------------------------------------------------
itcl::body bis_serialdemonmousereg::CombineTransformations { reference t1 t2 t3 } {


    set trans [ vtkGeneralTransform New ]
    $trans Identity
    $trans PostMultiply
    if { $t1 !=0 } { $trans Concatenate $t1 }
    if { $t2 !=0 } { $trans Concatenate $t2 }
    if { $t3 !=0 } { $trans Concatenate $t3 }
    

    set dim [ $reference GetDimensions ]
    set spa [ $reference GetSpacing    ]
    
    set tgrid [ vtkTransformToGrid New ]
    $tgrid SetGridExtent 0 [ expr [ lindex $dim  0 ] -1 ] 0 [ expr [ lindex $dim  1 ] -1 ] 0 [ expr [ lindex $dim  2 ] -1 ]
    $tgrid SetGridOrigin  0 0 0
    $tgrid SetGridSpacing [ lindex $spa 0 ] [ lindex $spa 1 ] [ lindex $spa 2 ] 
    $tgrid SetInput $trans
    $tgrid Update
    
    
    set trg [ vtkpxGridTransform New ]
    $trg SetDisplacementGrid [ $tgrid GetOutput ]
    $trg SetInterpolationModeToLinear

    $tgrid Delete
    $trans Delete
    return $trg
}
# --------------------------------------------------------------------------------------
itcl::body bis_serialdemonmousereg::GenerateReducedImage { itclimg factor } {
    
    set img [ $itclimg GetImage ]
    scan [  $img GetSpacing ] "%f %f %f" s(0) s(1) s(2)
    scan [  $img GetOrigin ] "%f %f %f" o(0) o(1) o(2)
    scan [ $img GetDimensions ] "%d %d %d" d(0) d(1) d(2)

    puts stderr "In Spacing $s(0) $s(1) $s(2), $d(0), $d(1), $d(2)"

    set fact $factor
    set div [ expr 1.0/$fact ]

    for { set i 0 } { $i <= 2 } { incr i } {
	set s($i) [ expr $fact*$s($i) ]
	set d($i) [ expr round($div*$d($i)) - 1 ]
    }

    puts stderr "Out Spacing $s(0) $s(1) $s(2), $d(0), $d(1), $d(2)"

    set resl [vtkImageReslice [pxvtable::vnewobj]] 
    $resl SetInput [ $itclimg GetImage ] 
    $resl SetOutputSpacing $s(0) $s(1) $s(2)
    $resl SetOutputOrigin  $o(0) $o(1) $o(2)
    $resl SetInterpolationMode 0 
    $resl SetOutputExtent 0 $d(0) 0 $d(1) 0 $d(2) 
    $resl Update
	
    $itclimg  ShallowCopyImage [ $resl GetOutput ]
    $resl Delete

}
# -----------------------------------------------------------------------------------------
# Execute
# ----------------------------------------------------------------------------------------

itcl::body bis_serialdemonmousereg::Execute {  } {

    PrintDebug "bis_serialdemonmousereg::Execute"
    pxtclvtkpxcontrib::LoadLibraries { vtkjwsDemonsRegistrationTCL }

    set symmetric [ $OptionsArray(symmetric) GetValue ] 
    set reduction [ $OptionsArray(reduction) GetValue ] 
    set debug [ $OptionsArray(debug) GetValue ] 
    
    set refgray [ [ pxitclimage \#auto ] GetThisPointer ]
    $refgray ShallowCopy  [ $this GetInputObject image_ref ] 

    set refbinary  [ [ pxitclimage \#auto ] GetThisPointer ]
    $refbinary ShallowCopy [ $this GetInputObject binary_image_ref ] 

    set targgray  [ [ pxitclimage \#auto ] GetThisPointer ]
    $targgray ShallowCopy  [ $this GetInputObject image_trg ]

    set targbinary [ [ pxitclimage \#auto ] GetThisPointer ]
    $targbinary ShallowCopy [ $this GetInputObject binary_image_trg ] 


    # Resizing in case halfsize is selected 
    set lst [ list $refbinary $refgray $targbinary $targgray ]
    foreach img $lst {
	puts stderr "Inp dims = [ [ $img GetImage ] GetDimensions ]"
	$this GenerateReducedImage $img $reduction
	puts stderr "\t\t\t out dims = [ [ $img GetImage ] GetDimensions ]"
    }
    set outname "output_grid"
  
    
    if {$symmetric == 0} {
	set demclass [ vtkjwsDemonsRegistration [pxvtable::vnewobj]] 
    } else {
	set demclass [vtkjwsSymmetricDemonsRegistration [pxvtable::vnewobj]] 
    }
    
    puts stdout "\n\n Deciding on Image Size\n"
    set arr [ $demclass DecideImageSize  [ $refgray GetImage ] [ $targgray GetImage ]  ]
    
    set width  [ $arr GetComponent 0 0 ]
    set height [ $arr GetComponent 0 1 ]
    set depth  [ $arr GetComponent 0 2 ]
    puts stdout "Suggested Dimensions = $width x $height x $depth"
    
        
    # --------------------------------------------------------------------------------
    # Step 5 is to pad images to suggested dimensions
    # --------------------------------------------------------------------------------
    set p_refbinary  [ $demclass PadImage [ $refbinary  GetImage] [ expr int($width) ] [expr int($height) ]  [ expr int($depth) ]]
    set p_targbinary [ $demclass PadImage [ $targbinary GetImage] [ expr int($width) ] [expr int($height) ]  [ expr int($depth) ]]
    
	
    puts stderr "Padded Dimensions: RefB = [  $p_refbinary GetDimensions ]"
    puts stderr "Padded Dimensions: TargB = [  $p_targbinary GetDimensions ]"
    
    # by wook
    if { $debug > 0 } {
	$this SaveImage $p_refbinary "${outname}_ref_paddedImage"
    }
    if { $debug > 0 } {
	$this SaveImage $p_targbinary "${outname}_targ_paddedImage"
    }
    
    # --------------------------------------------------------------------------------
    # Step 6 is to compute distance maps for bone .... 
    # --------------------------------------------------------------------------------
    
    puts stdout "\n\n Computing Bone Distance Maps \n\n"
    
    puts stdout "Reference :"
    set bone_ref  [ $demclass ComputeDistanceMap $p_refbinary 2 3 ]
    
    if { $debug > 0 } {
	$this SaveImage $bone_ref "${outname}_ref_bone_distancemap"
    }
    
    puts stdout "Target :"
    set bone_targ [ $demclass ComputeDistanceMap $p_targbinary 2 3 ]
    
    if { $debug > 0 } {
	$this SaveImage $bone_targ "${outname}_targ_bone_distancemap"
    }
    
    # --------------------------------------------------------------------------------
    # Step 7 is to run First Demons 
    # --------------------------------------------------------------------------------
    puts stdout "\n\n Computing First Demon \n\n"
    
    ## by wook
    if {$symmetric == 0} {
	set firstdemon [ vtkjwsDemonsRegistration New ]
    } else { 
	set firstdemon [vtkjwsSymmetricDemonsRegistration New ]
    }
    $firstdemon SetReferenceDistanceMap $bone_ref
    $firstdemon SetTargetDistanceMap $bone_targ
    $firstdemon RunExtendedDemons 1
    
    set transformation(1) [ $firstdemon GetGridTransform ]
    
    if { $debug > 0 } {
	set out1 ${outname}_first
	$this SaveTransformation $transformation(1) $out1
	puts stdout "Transformation Field -- First Edemons Saved in $out1"
    }
    $bone_targ Delete
       
    
    # --------------------------------------------------------------------------------
    # Step 8 is to deform objectmap for target 
    # --------------------------------------------------------------------------------
    
    puts stdout "\n\n Computing More Bone Distance Maps \n\n"
    puts stdout "Reference Skin:"
    set skin_ref  [ $demclass ComputeDistanceMap $p_refbinary 1 2 ]
    
    puts stdout "Target : First Reslice Binary"
    set warp1_targbinary [ $this ResliceImage $p_refbinary $p_targbinary $transformation(1) 0 ]
    
    set bone_targ  [ $demclass ComputeDistanceMap $warp1_targbinary 2 3 ]
    set skin_targ  [ $demclass ComputeDistanceMap $warp1_targbinary 1 2 ]
    
    if { $debug > 0 } {
	$this SaveImage $skin_ref  "${outname}_ref_skin_distancemap"
	$this SaveImage $bone_targ "${outname}_warp1_targ_bone_distancemap"
	$this SaveImage $skin_targ "${outname}_warp1_targ_skin_distancemap"
    }
    
    
    # -------------------------------------------------------------------------------
    # Step 9 is to combine distance maps
    # -------------------------------------------------------------------------------
    set combo_ref [ $demclass MergeDistanceMaps $skin_ref $bone_ref ]
    set combo_targ [ $demclass MergeDistanceMaps $skin_targ $bone_targ ]
    
    if { $debug > 0 } {
	$this SaveImage $combo_ref  "${outname}_combo_ref_distancemap"
	$this SaveImage $combo_targ "${outname}_combo_targ_distancemap"
    }
    
    $skin_targ Delete
    $skin_ref  Delete
    $bone_targ Delete
    
    
    # -------------------------------------------------------------------------------
    # Step 10 is to run second edemons
    # -------------------------------------------------------------------------------
    puts stdout "\n\n Computing Second Demon \n\n"
    
    #set seconddemon [ vtkjwsDemonsRegistration New ]
    
    ## by wook
    if {$symmetric== 0} {
	set seconddemon [ vtkjwsDemonsRegistration New ]
    } else { 
	set seconddemon [vtkjwsSymmetricDemonsRegistration New ]
    }
    
    
    $seconddemon SetReferenceDistanceMap $combo_ref
    $seconddemon SetTargetDistanceMap    $combo_targ
    $seconddemon RunExtendedDemons 2
    
    set transformation(2) [ $seconddemon GetGridTransform ]
    set transformation(21) [ $this CombineTransformations $bone_ref $transformation(2) $transformation(1) 0 ]
    
    if { $debug > 0 } {
	set out2 ${outname}_second
	$this SaveTransformation $transformation(2) $out2
	puts stdout "Transformation Field -- Second Edemons Saved in $out2"
	
	
	set out21 ${outname}_first_second
	$this SaveTransformation $transformation(21) $out21
	puts stdout "Transformation Field -- First and Second Edemons Saved in $out21"
    }
    
    
    
    # --------------------------------------------------------------------------------
    # Step 15 Gray Level Demons
    # --------------------------------------------------------------------------------
    set p_refgray    [ $demclass PadImage [ $refgray    GetImage] [expr int($width) ] [ expr int($height) ] [expr int($depth) ] ]
    set p_targgray   [ $this ResliceImage $p_refgray [ $targgray   GetImage ] $transformation(21) 1 ]
    
    puts stdout "Padded Dimensions: RefG  = [  $p_refgray GetDimensions  ]"
    puts stdout "Padded Dimensions: TargG = [  $p_targgray GetDimensions ]"
    
    # by wook
    if { $debug > 0 } {
	$this SaveImage $p_refgray "${outname}_ref_paddedGrayImage"
	$this SaveImage $p_targgray "${outname}_21Tr_targ_paddedGrayImage"
    }
    
    ## by wook
    if {$symmetric == 0} {
	set thirddemon [ vtkjwsDemonsRegistration New ]
    } else { 
	set thirddemon [vtkjwsSymmetricDemonsRegistration New ]
    }
    
    $thirddemon SetReferenceDistanceMap $p_refgray
    $thirddemon SetTargetDistanceMap    $p_targgray
    $thirddemon RunNormalDemons 
    
    set transformation(3) [ $thirddemon GetGridTransform ]
    
    
    if { $debug > 0 } {
	set out3 ${outname}_third
	$this SaveTransformation $transformation(3) $out3
	puts stdout "Transformation Field -- Third Edemons Saved in $out3"
    }
    
    set transformation(321) [ $this CombineTransformations $p_refgray $transformation(3) $transformation(2) $transformation(1) ]
    if { $debug > 0 } {
	set out321 ${outname}_complete
        $this SaveTransformation $transformation(321) $out321
	puts stdout "Transformation Field -- First, Second and Third Edemons Saved in $out321"
    }

    [ $this GetOutputObject transformation ] CopyTransformation $transformation(321)

    set r [ [ $this GetInputObject image_ref ]  GetImage ]
    set t [ [ $this GetInputObject image_trg ]  GetImage ]

    set w [ $this ResliceImage $r $t $transformation(321) 1 ]
    
    [ $this GetOutputObject warped_target ] ShallowCopyImage  $w
    [ $this GetOutputObject warped_target ] CopyImageHeader [ [ $this GetInputObject image_ref ]  GetImageHeader ]

    $w Delete
    
    $p_refgray Delete
    $p_targgray Delete
    
    $combo_ref  Delete
    $combo_targ Delete
    $bone_ref  Delete
    
    # ---------------------------------------------------------
    #  Cleaning up
    # ---------------------------------------------------------
    
    $firstdemon Delete
    
    ##############################
    
    
    return 1
}


# -----------------------------------------------------------------------------------------
#  This checks if executable is called (in this case bis_serialdemonmousereg.tcl) if it is execute
# ----------------------------------------------------------------------------------------
 
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    # this is essentially the main function


    set alg [bis_serialdemonmousereg [pxvtable::vnewobj]]
    $alg MainFunction 
}

