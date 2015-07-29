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
lappend auto_path [ file dirname [ info script ] ]
lappend auto_path [ file join [file join [ file dirname [ info script ]] ".."] base ]
lappend auto_path [ file join [file join [ file dirname [ info script ]] ".."] main ]

package provide mpjvesselutility 1.0

package require  Itcl 3.2
package require  Iwidgets 4.0

package require  pxitcllistselector 1.0
package require  pxitclbaseimagecontrol 1.0
package require  pxitclbaseimageviewer 1.0

package require  vtkmpjcontrib 1.1
package require  mpjguilookuptable 1.0

itcl::class mpjvesselutility {

    inherit pxitclbaseimagecontrol
    
    #-----------------------------------
    # construction and descruction
    #-----------------------------------
    constructor { par } {
	pxitclbaseimagecontrol::constructor $par
    } { InitializeControl }
    
    destructor { }

    #-----------------------------------
    # global variables
    #-----------------------------------
    private variable widget_list
    private variable global_list
    
    #-----------------------------------
    # initialization methods
    #-----------------------------------
    public method Initialize { inpwidg }
    public method InitializeControl { }
    public method DismissWindow { }
      
    #-----------------------------------
    # diffusion series input/output
    #-----------------------------------
    public method LoadMask { fname }
    public method LoadInput { fname }
    public method ComputeResults { }
    public method SaveAllResults { }
    public method SaveResult { index }
    public method ClearAllResults { }
    public method DisplayResult { index }
    public method GrabImage { index }
    public method ClearDisplay { }
    public method SetResultPreffix { preffix }
    public method GetResultPreffix { } 
    public method SetCurrentDirectory { fname } 
    
    #-----------------------------------
    # interface creation methods
    #-----------------------------------
    private method CreateDialogs { base }
    private method CreateResultsControl { base }
    private method CreateVesselControl { base }
    private method CreateTensorControl { base }
    #    private method CreateMenu { mb }
    public method AddToMenuButton { mb args }    
 
    private method linspace { x1 x2 n }
    private method logspace { x1 x2 n }
    private method loadmask { }
    private method loadinput { }
    private method displayresult { }
    private method saveresult { }
    private method toggleresult { }
    private method saveallresults { }
 
    private method mask_autochanged_cb { }
    private method mask_autothreshchanged_cb { }

    private method vessel_type_cb { }
    private method vessel_intensity_cb { }

    private method initialize_glyphs { }
    private method input_updateinfo { index }    

    private method result_clear { start end }
    private method result_createhessian { }
    protected method ResampleImage { img spacing } 
}  

#-------------------------------------------------------------------------------------------
#  Close window 
# -------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::DismissWindow { } {
    
    set INPUT 0
    set MASK 1

    set ok "yes"
    
    set tensorsz [ [ lindex $global_list(result_list) $INPUT ] GetImageSize ]
    set masksz [ [ lindex $global_list(result_list) $MASK ] GetImageSize ]

    if { [expr ($tensorsz > 1) || ($masksz > 1) ] } {
	
	set ok [ tk_messageBox -type yesnocancel -default no \
		     -parent $basewidget -title "Think again ..." \
		     -message "Unload all images from memory?" -icon question  ]
    }
    
    if { $ok == "yes" } {	
	
	$this ClearAllResults
	
	$this SetTitle $global_list(appname)
	
	set global_list(mask_fname) ""
	set global_list(tensor_fname) ""
    }
    
    if { $ok != "cancel" } {
	$this HideWindow
    }
    
}

#-------------------------------------------------------------------------------------------
#  Initialize tool
# -------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::InitializeControl { } {

    set appname "Vessel Utility v1.0"
    set aboutstring "(c) Marcel Jackowski and Xenios Papademetris 2006"

    set global_list(appname) "Vessel Utility"

    # create image titles
    set global_list(result_title) \
	[ list \
	      { "Input image"             "input" 1 } \
	      { "Region of interest mask" "mask" 1 } \
	      { "Hessian matrix"          "hessian" 1 }\
	      { "Vesselness measure"      "vessel" 1 } \
	      { "Maximum scale"           "scale" 1 } ] 

    ## create empty images
    foreach item $global_list(result_title) {
 	lappend result_list [ pxitclimage \#auto ]
	set global_list(enable_list,[lindex $item 1]) 0
    }  
    
    set global_list(result_fpreffix) "preffix_"
    set global_list(result_fdir) [ pwd ] 
    set global_list(result_list) $result_list
    
    set global_list(mask_fname) ""
    set global_list(mask_threshval) 0
    set global_list(mask_autothresh) 0
    set global_list(mask_auto) 1
    
    set global_list(input_fname) ""
    set global_list(input_numscales) 5
    set global_list(input_fromscale) 1.0
    set global_list(input_toscale) 5.0
    set global_list(input_logscale) 1
    set global_list(input_smoothsigma) 1.0
    set global_list(input_isotropic) 1
    set global_list(input_scale_vesselness) 1
    
    set global_list(vessel_alpha) 0.5
    set global_list(vessel_beta) 0.5
    set global_list(vessel_gamma) 0.25
    set global_list(vessel_maxintensity) 100.0
    set global_list(vessel_type) 0
    set global_list(vessel_intensity) 0
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in mask auto status
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::mask_autochanged_cb { } {
    set state(0) disabled; set state(1) normal        
    $widget_list(mask_load) buttonconfigure load -state \
	$state([expr 1 - $global_list(mask_auto)])
    $widget_list(mask_load) buttonconfigure disp -state \
	$state([expr 1 - $global_list(mask_auto)])
    $widget_list(mask_load) buttonconfigure grab -state \
	$state([expr 1 - $global_list(mask_auto)])
    $widget_list(mask_fname) configure -state \
	$state([expr 1 - $global_list(mask_auto)])

    if { $global_list(mask_auto) == 0 } {
	set global_list(mask_autothresh) 0
    }
	
    $widget_list(mask_autothresh) configure -state \
	$state($global_list(mask_auto))	

    if { $global_list(mask_autothresh) == 0 } {
	$widget_list(mask_threshval) configure -state disabled
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in mask thresh status
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::mask_autothreshchanged_cb { } {
    set state(0) disabled; set state(1) normal

    $widget_list(mask_threshval) configure -state \
	$state($global_list(mask_autothresh))
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in structure type for detection
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::vessel_type_cb { } {
    set index [ $widget_list(vessel_type) index [ $widget_list(vessel_type) get ] ]
    set global_list(vessel_type) $index
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in structure intensity
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::vessel_intensity_cb { } {
    set index [ $widget_list(vessel_intensity) index [ $widget_list(vessel_intensity) get ] ]
    set global_list(vessel_intensity) $index
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Update input information on title bar
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::input_updateinfo { index } {

    set sel_input [ [ lindex $global_list(input_list) $index ] GetImage ]
    set sel_fname [ lindex $global_list(input_fname) $index ]
    
    scan [ $sel_input GetDimensions ] "%d %d %d" dx dy dz	       
    set nc [  $sel_input GetNumberOfScalarComponents ]
    set name [ file tail [ file root $sel_fname ] ]
    
    set global_list(result_fpreffix) "${name}_"

    $this SetTitle "$global_list(appname): $name (${dx}x${dy}x${dz}, $nc frames)"
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load mask
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::loadmask { } {

    ## browse for filename
    set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
    set fname  [tk_getOpenFile -title "Select mask" -filetypes $typelist -initialdir $global_list(result_fdir) -parent $basewidget ] 
    
    if { $fname != "" } {
	
	set global_list(result_fdir) [ file dirname $fname ]
	
	## load mask into memory
	LoadMask $fname
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Load in mask image 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::LoadMask { fname } {

    set MASK 1     
    set mask [ lindex $global_list(result_list) $MASK ]

    ## load series into memory
    set ana [ vtkpxAnalyzeImageSource  [ pxvtable::vnewobj ]]
    $this SetFilterCallbacks $ana "Reading in mask image..."
    
    WatchOn
    
    ## load series 
    $ana Load $fname 
    
    ## if mask not a unsigned char image, convert it
    set scalar_type [ [ $ana GetOutput ] GetScalarType ]    

    if { $scalar_type != 3 } {	
	
	set imcast [ vtkImageCast [ pxvtable::vnewobj ] ]
	$this SetFilterCallbacks $imcast "Converting mask to unsigned char values..."
	$imcast SetInput [ $ana GetOutput ]
	$imcast SetOutputScalarTypeToUnsignedChar
	$imcast ClampOverflowOn	
	$imcast Update
	
	$mask ShallowCopyImage [ $imcast GetOutput ]
	
	$imcast Delete
	
    } else {
	
	$mask SetFromAnalyzeSource $ana
    }
   
    set global_list(mask_fname) $fname

    WatchOff

    $ana Delete
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load image
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::loadinput { } {

    ## browse for filename
    set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
    set fname [tk_getOpenFile -title "Select image" -filetypes $typelist -initialdir $global_list(result_fdir) -parent $basewidget ] 
    
    if { $fname != "" } {
	
	set global_list(result_fdir) [ file dirname $fname ]
	
	## load mask into memory
	LoadInput $fname
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Load in tensor image 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::LoadInput { fname } {

    set INPUT 0

    set input [ lindex $global_list(result_list) $INPUT ]

    ## load series into memory
    set ana [ vtkpxAnalyzeImageSource  [ pxvtable::vnewobj ]]
    
    $this SetFilterCallbacks $ana "Reading input image..."
    
    WatchOn
    
    ## load series 
    $ana Load $fname 
    
    $input SetFromAnalyzeSource $ana
    
    # determine coordinate transform
    set or [ $ana GetOrientation ]
    
    set global_list(input_fname) $fname
    set global_list(result_fpreffix) "[file root [file tail $fname] ]_"
    
    WatchOff
    
    $ana Delete
    
    $widget_list(computebtn) configure -state normal
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Set result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::SetResultPreffix { preffix } {
    set global_list(result_fpreffix) $preffix
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Get result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::GetResultPreffix { } {
    return $global_list(result_fpreffix)
}


itcl::body mpjvesselutility::SetCurrentDirectory { fname } {
    set global_list(result_fdir) $fname
}
#-------------------------------------------------------------------------------------------
#  PUBLIC: Clear all results
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::ClearAllResults { } {
    
    foreach result $global_list(result_list) {
	$result Clear
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::SaveAllResults { } {

    foreach result $global_list(result_list) title $global_list(result_title) {
	
	set fname $global_list(result_fpreffix)
	set fname "$fname[ lindex $title 1 ]"
	
	$result Save $fname
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Save single result
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::SaveResult { index } {        

    set fname $global_list(result_fpreffix)
    set fname "$fname[ lindex [lindex $global_list(result_title) $index ] 1 ]"

    [lindex $global_list(result_list) $index] Save $fname
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::saveallresults { } {

    ## browse for directory
    set fdir [ tk_chooseDirectory -title " Select directory" -parent $basewidget -initialdir $global_list(result_fdir) ]
    
    if { $fdir != "" } {
	
	set global_list(result_fdir) $fdir
	
	foreach result $global_list(result_list) title $global_list(result_title) {
	    
	    set fname $global_list(result_fpreffix)
	    set fname "$fname[ lindex $title 1 ]"
	    
	    $result Save [ file join $fdir $fname ]
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save results
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::saveresult { } {
    
    ## get current selection 
    set sel_item [ $widget_list(result_list) getcurselection ]
    
    if { $sel_item != "" } { 

	## browse for directory
	set fdir [ tk_chooseDirectory -title " Select directory" -parent $basewidget -initialdir $global_list(result_fdir) ]
	
	if { $fdir != "" } {
	    
	    set global_list(result_fdir) $fdir
	    
	    foreach item $sel_item {
		
		set index [ $widget_list(result_list) index $item ]
		set result [ lindex $global_list(result_list) $index ]
		
		set fname $global_list(result_fpreffix)
		set fname "$fname[ lindex [lindex $global_list(result_title) $index ] 1 ]"
		
		$result Save [ file join $fdir $fname ]
	    }
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Display result
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::DisplayResult { index } {
    $parent SetImageFromObject \
	[ [ lindex $global_list(result_list) $index ] GetThisPointer ] $this
    $parent ShowWindow
}

itcl::body mpjvesselutility::GrabImage { index } {

    set img [ $parent GetDisplayedImage ]
    if { [ $img GetImageSize ] > 0 } {
	[ [ lindex $global_list(result_list) $index ] GetThisPointer ] ShallowCopy $img
	set fname [ $img cget -filename ]
	if { [ string length $fname ] < 1 } {
	    set fname "noname.hdr"
	}
	
	if { $index == 0 } {
	    set global_list(input_fname) $fname
	    set global_list(result_fpreffix) "[file root [file tail $fname] ]_"
	} else {
	    set global_list(mask_fname) $fname
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Display result
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::displayresult { } {

    set sel_item [ $widget_list(result_list) getcurselection ]
	    
    if { $sel_item != "" } {
	set index [ $widget_list(result_list) index [ lindex $sel_item 0 ] ]
	set result [ lindex $global_list(result_list) $index ]

	set range [ [ [ [ $result GetImage ] GetPointData ] GetScalars ] GetRange ]

	$this DisplayResult $index	
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: toggle result computation on/off
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::toggleresult { } {
   
    set color(0) \#dcdcdc
    set color(1) black

    set sel_item [ $widget_list(result_list) getcurselection ]

    if { $sel_item != "" } {

	set index [ $widget_list(result_list) index [ lindex $sel_item 0 ] ]

	if { $index > 4 } {
	    set enable [expr 1 - [ lindex [ lindex $global_list(result_title) $index ] 2 ] ]
	    
	    $widget_list(result_list) itemconfigure $index -foreground $color($enable)
	    
	    set item [ lindex $global_list(result_title) $index ]
	    set new_item [ lreplace $item 2 2 $enable ]
	    
	    set global_list(result_title) [ lreplace $global_list(result_title) \
						$index $index $new_item ]
	}
    }
    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Clear results
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::result_clear { start end } {
    
    for { set i $start } { $i <= $end } { incr i } {
	
	set title [ lindex [ lindex $global_list(result_title) $i ] 0 ]	    
	
	if { $title == "Region of interest mask" } {
	    if { $global_list(mask_auto) } {
		[ lindex $global_list(result_list) $i ] Clear
	    }
	} else { 
	    [ lindex $global_list(result_list) $i ] Clear
	}

    }

}

#-------------------------------------------------------------------------------------------
#  PRIVATE: returns a list of n equally spaced points btwn x1 and x2 inclusive
#-------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::linspace { x1 x2 n } {
    
    set linlist ""
    
    if { $n < 2 } { return $x1 }
    
    set delta [expr ($x2 - $x1) / ($n - 1) ]
    for { set i 0 } { $i <= [expr $n - 2] } { incr i } {
	lappend linlist [expr $x1 + $i*$delta]
    }
    lappend linlist $x2

    return $linlist    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: returns a list of n logarithmically spaced points btwn x1 and x2 inclusive
#-------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::logspace { x1 x2 n } {

    set loglist ""

    if { $n < 2 } { return $x1 }
    
    set linlist [ linspace [expr log10($x1)] [expr log10($x2)] $n ]
    
    foreach item $linlist {
	lappend loglist [expr pow(10,$item)]
    }
    
    return $loglist
}


#-------------------------------------------------------------------------------------------
#  PRIVATE: Create eigensystem
#-------------------------------------------------------------------------------------------

itcl::body mpjvesselutility::ResampleImage { img spacing } {

    puts stderr "Resampling at $spacing x $spacing x $spacing "

    set resamp [ vtkImageResample [ pxvtable::vnewobj ]]
    $resamp SetInput $img
    $resamp SetAxisOutputSpacing 0 $spacing
    $resamp SetAxisOutputSpacing 1 $spacing
    $resamp SetAxisOutputSpacing 2 $spacing
    $resamp Update
    set out [ vtkImageData [ pxvtable::vnewobj ]]
    $out ShallowCopy [ $resamp GetOutput ]
    $resamp Delete

    puts stderr " Dimensions Input [ $img GetDimensions ] --> Output [ $out GetDimensions ] "
    return $out
}


itcl::body mpjvesselutility::result_createhessian { } {

    set INPUT 0
    set MASK 1
    set HESSIAN 2
    set VESSEL 3
    set SIGMA 4

    set input [ lindex $global_list(result_list) $INPUT ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set hessian [ lindex $global_list(result_list) $HESSIAN ]
    set vessel [ lindex $global_list(result_list) $VESSEL ]
    set sigma [ lindex $global_list(result_list) $SIGMA ]

    $hessian CopyImageHeader [ $input GetImageHeader ]
    $vessel CopyImageHeader [ $input GetImageHeader ]
    $sigma CopyImageHeader [ $input GetImageHeader ]
    
    set scales 0

    set global_list(input_smoothsigma) [ $widget_list(input_smoothsigma) get ]
    set smoothsigma $global_list(input_smoothsigma)

    ## generate scales
    if { $global_list(input_logscale) == 0 } {
	set scales [ linspace \
			 $global_list(input_fromscale) \
			 $global_list(input_toscale)  \
			 $global_list(input_numscales) ]
    } else {
	set scales [ logspace \
			 $global_list(input_fromscale) \
			 $global_list(input_toscale)  \
			 $global_list(input_numscales) ]
    }
    
    if { [expr ([ $input GetImageSize ] == 1) ] } {
	puts "No image!"
	return 0
    }

    set doresample 0
    set newinput   0
    set newhess    0

    set cast [ vtkImageCast [ pxvtable::vnewobj ]]
    $cast SetInput [ $input GetImage ]
    $cast SetOutputScalarTypeToFloat
    $cast Update

    set spa [ lsort [ [ $input GetImage ] GetSpacing ] ]
    
    if { $global_list(input_isotropic) == 1 } {
	
	if { ([ lindex $spa 2 ] - [ lindex $spa 0 ]) > [ expr 0.05* [lindex $spa 1 ] ] } {
	    set doresample 1
	    set spx [ expr ( [ lindex $spa 0 ] + [ lindex $spa 1 ] + [ lindex $spa 2 ])/3.0 ]
	    
	    set newinput [ ResampleImage [ $cast GetOutput ] $spx ]
	    if { [ $mask GetImageSize ] > 1 } {
		set newmask [ ResampleImage [ $mask GetImage ] $spx ]
	    } else {
		set newmask 0
	    }
	}
    }

    if { $doresample == 0 } {
	set newinput [ $cast GetOutput ]
	if { [ $mask GetImageSize ] > 1 } {
	    set newmask [ $mask GetImage ]
	} else {
	    set newmask 0
	}
    }

    set firsttime 1
    ## hessian filter
       
    ## run the filter for all scales
    foreach sig $scales {

	set hess [ vtkmpjImageHessian [ pxvtable::vnewobj ] ]
	$hess SetInput $newinput
	if { $newmask !=0  } {
	    $hess SetMask $newmask
	}
	
	$hess EnableSmoothingOn
	$hess SetEnableThresholding $global_list(mask_autothresh)
	$hess SetThresholdValue $global_list(mask_threshval)
	$hess AppendSigmaImageOff

	set sgmlabel [format "%7.4f" $sig]
	$this SetFilterCallbacks $hess "Computing hessian at scale $sgmlabel..."
	$hess SetSigma $sig
	$hess Update

	puts stderr "\n\n ************** Hessian Done **************************"
	
	## vesselness filter
	set vess [ vtkmpjImageVesselEnhancement [ pxvtable::vnewobj ] ]
	$this SetFilterCallbacks $vess "Computing vesselness measure..."
	$vess SetInput [ $hess GetOutput ]
	if { $newmask != 0 } { 
	    $vess SetMask $newmask
	}
	
	$vess SetAlpha $global_list(vessel_alpha)
	$vess SetBeta  $global_list(vessel_beta)
	$vess SetGamma [ expr $global_list(vessel_gamma) * $global_list(vessel_maxintensity) ]
	$vess SetStructureType $global_list(vessel_type)
	$vess SetBrightStructure [expr 1 - $global_list(vessel_intensity) ]
	if { $global_list(input_scale_vesselness) == 1 } {
	    $vess SetVesselnessScaleFactor 1000.0
	} else {
	    $vess SetVesselnessScaleFactor 1.0
	}
	
	## compute vesselness measure
	$vess Update
	puts stderr "\n\n ************** Vesselness Done **************************"

    
	## store maximum
	if { $firsttime ==1 } {
	    
	    ## store hessian
	    $hessian ShallowCopyImage [ $hess GetOutput ]

	    ## store vesselness
	    $vessel ShallowCopyImage [ $vess GetOutput ]
	    
	    ## store sigma
	    [ $sigma GetImage ] CopyStructure  $newinput
	    [ $sigma GetImage ] SetScalarTypeToFloat 
	    [ $sigma GetImage ] AllocateScalars
	    [ [ [ $sigma GetImage ] GetPointData ] GetScalars ] FillComponent 0 $sig
	    set firsttime 0
	} else {
	    $vess UpdateHessianResponse [ $hessian GetImage ] [ $vessel GetImage ] [ $sigma GetImage ] [ $hess GetOutput ] [ $vess GetOutput ] $sig
	    puts stderr "\n\n ************** Max Done **************************"
	}

	$hess Delete
	$vess Delete
	puts stderr "\n\n ************** Cleanup Done **************************"

    }

    if { $smoothsigma > 0.0 } { 
	
	set s1  [ vtkImageGaussianSmooth [ pxvtable::vnewobj ]  ]
	$s1 SetStandardDeviations $smoothsigma $smoothsigma $smoothsigma
	$s1 SetInput [ $vessel GetImage ]
	$s1 Update
	$vessel ShallowCopyImage [ $s1 GetOutput ]
	$s1 Delete

    	set s2  [ vtkImageGaussianSmooth [ pxvtable::vnewobj ]  ]
	$s2 SetStandardDeviations $smoothsigma $smoothsigma $smoothsigma
	$s2 SetInput [ $sigma GetImage ]
	$s2 Update
	$sigma ShallowCopyImage [ $s2 GetOutput ]
	$s2 Delete
    }

    if { $doresample == 1 } {

	puts stderr "Scaling scales by $spx "

	set sc [ vtkImageShiftScale [ pxvtable::vnewobj ]]
	$sc SetInput [ $sigma GetImage ]
	$sc SetShift 0.0
	$sc SetScale $spx
	$sc Update
	$sigma ShallowCopyImage [ $sc GetOutput ]
	$sc Delete

	catch { $newinput Delete } 
	catch { $newmask  Delete  }
    }

    $cast Delete
    
    update idletasks

    $parent SetResultsFromObject [ $vessel GetThisPointer ] $this


    return 1
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Compute results
# -----------------------------------------------------------------------------------------
itcl::body mpjvesselutility::ComputeResults { } {
    
    set MASK 1
    
    WatchOn
    
    ## clear results
    result_clear $MASK [ expr [ llength $global_list(result_list) ] -1 ]
    
    $widget_list(notebook) view "Results"; update

    ## create maps
    result_createhessian
    
    WatchOff
}

#-------------------------------------------------------------------------------------------
#  Create tensor control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::CreateTensorControl { base } {
    
    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Input image"
    pack $base.frame0 -fill both -expand f -pady 5
    
    set frame0 [ $base.frame0 childsite ]
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 0
  
    set input_fname [ iwidgets::entryfield $subframe1.input_fname \
			   -textvariable [ itcl::scope global_list(input_fname) ] \
			   -labeltext "Filename:" ] 
    pack $input_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(input_fname) $input_fname
    
    set input_load [ iwidgets::buttonbox $subframe1.input_load -orient horizontal -padx 2 ]
    $input_load add load -text "Load..." -command [ itcl::code $this loadinput ] 
    $input_load add disp -text "Display" -command [ itcl::code $this DisplayResult 0 ]
    $input_load add grab -text "Grab" -command [ itcl::code $this GrabImage 0 ]
    pack $input_load -side left
    set widget_list(input_load) $input_load  

    iwidgets::labeledframe $base.frame1 \
	-labelpos nw  \
	-labeltext "Region of interest mask"
    pack $base.frame1 -fill both -expand f

    set frame1 [ $base.frame1 childsite ]

    set subframe0 [ frame $frame1.subframe0 ]
    pack $subframe0 -side top -fill both -expand f -pady 5

    set mask_auto [ checkbutton $subframe0.mask_auto -text "Use entire image for computation" \
			-variable [ itcl::scope global_list(mask_auto) ] \
			-command [ itcl::code $this mask_autochanged_cb ] ]
    pack $mask_auto -side left -fill both -padx 5 -pady 5
    
    set mask_threshval [ iwidgets::entryfield $subframe0.mask_threshval \
			     -validate real \
			     -width 5 \
			     -textvariable [ itcl::scope global_list(mask_threshval) ] \
			     -state disabled ]
    pack $mask_threshval -side right -pady 5 -padx 5
    set widget_list(mask_threshval) $mask_threshval

    set mask_autothresh [ checkbutton $subframe0.mask_autothresh \
			      -text "Threshold image at:" \
			      -variable [ itcl::scope global_list(mask_autothresh) ] \
			      -command [ itcl::code $this mask_autothreshchanged_cb ] ]    
    pack $mask_autothresh -side right -pady 5
    set widget_list(mask_autothresh) $mask_autothresh

    set subframe1 [ frame $frame1.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 0
    
    set mask_fname [ iwidgets::entryfield $subframe1.mask_fname \
			 -textvariable [ itcl::scope global_list(mask_fname) ] \
			 -labeltext "Filename:" ] 
    pack $mask_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(mask_fname) $mask_fname

    set mask_load [ iwidgets::buttonbox $subframe1.mask_load -orient horizontal -padx 2 ]
    $mask_load add load -text "Load..." -command [ itcl::code $this loadmask ] -state disabled  
    $mask_load add disp -text "Display" -command [ itcl::code $this DisplayResult 1 ] -state disabled
    $mask_load add grab -text "Grab" -command [ itcl::code $this GrabImage 1 ] -state disabled
    pack $mask_load -side left 
    set widget_list(mask_load) $mask_load
    



    
}

#-------------------------------------------------------------------------------------------
#  Create vessel enhancement control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::CreateVesselControl { base } {

    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Sensitivity parameters"
    pack $base.frame0 -fill both -expand f -pady 5
    
    set frame0 [ $base.frame0 childsite ]
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 2
    
    set vessel_alpha [ iwidgets::entryfield $subframe1.vessel_alpha \
			   -labeltext "Alpha:" \
			   -validate real \
			   -width 4 \
			   -textvariable [ itcl::scope global_list(vessel_alpha) ] ]
    pack $vessel_alpha -side left -pady 7 -padx 5
    set widget_list(vessel_alpha) $vessel_alpha

    set vessel_beta [ iwidgets::entryfield $subframe1.vessel_beta \
			   -labeltext "Beta:" \
			   -validate real \
			   -width 4 \
			   -textvariable [ itcl::scope global_list(vessel_beta) ] ]
    pack $vessel_beta -side left -pady 7 -padx 5
    set widget_list(vessel_beta) $vessel_beta

    set vessel_gamma [ iwidgets::entryfield $subframe1.vessel_gamma \
			   -labeltext "Gamma:" \
			   -validate real \
			   -width 4 \
			   -textvariable [ itcl::scope global_list(vessel_gamma) ] ]
    pack $vessel_gamma -side left -pady 7 -padx 5
    set widget_list(vessel_gamma) $vessel_gamma

    set vessel_maxintensity [ iwidgets::entryfield $subframe1.vessel_maxintensity \
				  -labeltext "Maximum intensity:" \
				  -validate real \
				  -width 7 \
				  -textvariable [ itcl::scope global_list(vessel_maxintensity) ] ]
    pack $vessel_maxintensity -side left -pady 7 -padx 5
    set widget_list(vessel_maxintensity) $vessel_maxintensity

    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -side top -fill both -expand f -pady 2
        
    set vessel_type \
	[ iwidgets::optionmenu $subframe2.vessel_type \
	      -command [ itcl::code $this vessel_type_cb ] \
	      -labeltext "Structure type:" ]
    $vessel_type insert 2 "Blob"
    $vessel_type insert 1 "Plate"
    $vessel_type insert 0 "Tubular"
    pack $vessel_type -side left -padx 5 -pady 5
    set widget_list(vessel_type) $vessel_type    
    $vessel_type select 0

    set vessel_intensity \
	[ iwidgets::optionmenu $subframe2.vessel_intensity \
	      -command [ itcl::code $this vessel_intensity_cb ] \
	      -labeltext "Structure intensity:" ]
    $vessel_intensity insert 1 "Dark"
    $vessel_intensity insert 0 "Bright"
    pack $vessel_intensity -side left -padx 5 -pady 5
    set widget_list(vessel_intensity) $vessel_intensity
    $vessel_intensity select 0
    

    iwidgets::labeledframe $base.frame1 \
	-labelpos nw  \
	-labeltext "Multiscale analysis"
    pack $base.frame1 -fill both -expand f -pady 0
    
    set frame0 [ $base.frame1 childsite ]
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 5

    set input_numscales [ iwidgets::spinint $subframe1.input_numscales \
			      -labeltext "Number of scales:" \
			      -fixed 3 -width 3 -range {1 100} -step 1 \
			      -textvariable [ itcl::scope global_list(input_numscales) ] ]
    pack $input_numscales -side left -pady 5 -padx 10
    set global_list(input_numscales) 5
    set widget_list(input_numscales) $input_numscales

    label $subframe1.dummyspace0 -text ""
    pack $subframe1.dummyspace0 -side left -padx 2

    set input_fromscale [ iwidgets::entryfield $subframe1.input_fromscale \
			      -labeltext "Range:" \
			      -validate real \
			      -width 4 \
			      -textvariable [ itcl::scope global_list(input_fromscale) ] ]
    pack $input_fromscale -side left -pady 5 -padx 0
    set widget_list(input_fromscale) $input_fromscale

    set input_toscale [ iwidgets::entryfield $subframe1.input_toscale \
			    -labeltext ":" \
			    -validate real \
			    -width 4 \
			    -textvariable [ itcl::scope global_list(input_toscale) ] ]
    pack $input_toscale -side left -pady 5 -padx 0
    set widget_list(input_toscale) $input_toscale

    label $subframe1.dummyspace1 -text "pixels"
    pack $subframe1.dummyspace1 -side left -padx 3

    set input_logscale [ checkbutton $subframe1.input_logscale \
			     -text "Logarithmic" \
			     -variable [ itcl::scope global_list(input_logscale) ] ]
    pack $input_logscale -side right -fill both -padx 10 -pady 5



    iwidgets::labeledframe $base.frame2 \
	-labelpos nw  \
	-labeltext "Other Processing"
    pack $base.frame2 -fill both -expand f -pady 0 -side top
    
    set frame2 [ $base.frame2 childsite ]

    set w1 [ checkbutton $frame2.iso -text "Isotropic voxels" \
		 -variable [ itcl::scope global_list(input_isotropic) ] ]
    set w2 [ checkbutton $frame2.vsc -text "Vesselness x1000" \
		 -variable [ itcl::scope global_list(input_scale_vesselness) ] ]

    set w [ iwidgets::optionmenu $frame2.sigma \
		-labeltext "Smooth Output:" ]
    $w insert 0 "0.0"
    $w insert 1 "0.5"
    $w insert 2 "1.0"
    $w insert 3 "1.5"
    $w insert 4 "2.0"
    pack $w2 $w1 $w -side left -padx 2 -pady 5
    $w select 2

    set widget_list(input_smoothsigma) $w


    
}

#-------------------------------------------------------------------------------------------
#  Create results control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::CreateResultsControl { base } {   

    set color(0) \#dcdcdc
    set color(1) black

    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Results"
    pack $base.frame0 -fill both -expand t 
    
    set frame0 [ $base.frame0 childsite ]
    
    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -fill both -expand t
    
    iwidgets::scrolledlistbox $subframe0.resultlist \
	-hscrollmode dynamic \
	-selectmode extended \
	-dblclickcommand [ itcl::code $this toggleresult ] \
	-visibleitems 40x10 
    
    pack $subframe0.resultlist -side left -fill both -expand t -padx 2 -pady 2
    set widget_list(result_list) $subframe0.resultlist
    
    foreach item $global_list(result_title) {
	set enable [ lindex $item 2 ]
     	$widget_list(result_list) insert end "[lindex $item 0]" 
	$widget_list(result_list) itemconfigure end -foreground $color($enable)
    }

    set subframe1 [ frame $subframe0.subframe1 ]
    pack $subframe1 -side right -fill both -padx 0
    
    set resultbbox [ iwidgets::buttonbox $subframe1.input -orient vertical ]
    $resultbbox add display -text "Display" -command [ itcl::code $this displayresult ]
    $resultbbox add save -text "Save..." -command [ itcl::code $this saveresult ]
    $resultbbox add saveall -text "Save all" -command [ itcl::code $this saveallresults ]    
    pack $resultbbox -side top -fill x -expand t -padx 0
    set widget_list(result_buttonbox) $resultbbox

    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -side top -fill x -padx 2 -pady 5

    set result_fpreffix [ iwidgets::entryfield $subframe2.result_fpreffix \
			      -textvariable [ itcl::scope global_list(result_fpreffix) ] \
			      -labeltext "Preffix:" ]
    pack $result_fpreffix -side left -fill x -expand t -padx 5 -pady 2
    set widget_list(result_fpreffix) $result_fpreffix    
}

# -------------------------------------------------------------------------------------------
#  Diffusion control inititialization
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::Initialize { widget } {
    
    if { $initialized == 1 } { return $basewidget }
    
    #  -------------------------------------
    #  Create User Interface
    #  -------------------------------------	
    set basewidget [toplevel $widget ]
    wm geometry $basewidget 610x450
    wm withdraw $basewidget

    set notebook $basewidget.notebook    
    iwidgets::tabnotebook $notebook -tabpos w         
    set widget_list(notebook) $notebook

    set mb [ frame $basewidget.mb ]
    pack $mb -side top -fill x -expand false
    
    #    CreateMenu $mb

    CreateTensorControl     [ $notebook add -label "Input"  ]
    CreateVesselControl     [ $notebook add -label "Vesselness" ]
    CreateResultsControl    [ $notebook add -label "Results" ] 
    
    pack $notebook -side top -fill both -expand t -padx 5

    set bottom [ frame $basewidget.bottom -relief ridge]
    pack $bottom -side top -fill x -expand false
    
    set pbar [ frame $basewidget.pbar -width 400 -height 20 ]
    pack $pbar -side bottom -expand f -fill x
    CreateProgressBar $pbar
    
    set w [ frame $basewidget.w -relief ridge -width 800]
    pack $w -side bottom -fill x -expand false
    
    eval "button $w.close -text Close -command { $this DismissWindow }"
    eval "button $w.help -text About -command { $this AboutCommand }"
    pack $w.close $w.help -side right -fill x -padx 10 -pady 10

    set widget_list(computebtn) [ button $w.compute -text "Compute!" \
				      -state normal \
				      -command [ itcl::code $this ComputeResults ] ]    
    pack $w.compute -side left -fill x -expand true -padx 20 -pady 10  
   

    

    set initialized 1

    SetTitle "Vessel Utility v1.0"
    eval "wm protocol $basewidget WM_DELETE_WINDOW { $this DismissWindow }"

    return $basewidget 
}

#-------------------------------------------------------------------------------------------
#  Add controls to menu button
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselutility::AddToMenuButton { mb args } {
    eval "$mb add command -label \"Vessel utility\" -command {$this ShowWindow \"Input\"}"
}

#-------------------------------------------------------------------------------------------
#  Create top level menu button
# ------------------------------------------------------------------------------------------
# itcl::body mpjvesselutility::CreateMenu { mb } {    
    
#     menubutton $mb.file          -text Setup       -menu $mb.file.m -underline 0
#     pack $mb.file $mb.file    -side left
#     menubutton $mb.results       -text Results     -menu $mb.results.m -underline 0
#     pack $mb.file $mb.results    -side left
#     menubutton $mb.help      -text Help         -menu $mb.help.m -underline 0 -padx 4
#     pack $mb.help  -side right
    
#     menu $mb.file.m -tearoff 0 
#     if { $parent == 0 } {
# 	eval "$mb.file.m add command -label Exit -command {  pxtkexit } -underline 1"
#     } else {
# 	eval "$mb.file.m add command -label Close -command {  $this HideWindow } -underline 1"
#     }

#     menu $mb.results.m -tearoff 0 
#     eval "$mb.results.m add command -label \"Compute All\" -command { $this ComputeResults } -underline 0 -state disabled"
#     $mb.results.m add command -label "Save All Results" -command [ itcl::code $this saveallresults ] -underline 0
    
#     menu $mb.help.m -tearoff 0
    
#     set widget_list(menu_results) $mb.results.m    
    
#     eval "$mb.help.m add command -label About -command { $this AboutCommand }"
# }

#-------------------------------------------------------------------------------------------
#  Main function for stand-alone execution
# ------------------------------------------------------------------------------------------
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {

    puts "\n[ file rootname $argv0 ] is not a stand-alone program.\n"
    exit
}

