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






# -----------------------
# Dependencies and path
# -----------------------
lappend auto_path [ file dirname [ info script ] ]
lappend auto_path [ file join [file join [ file dirname [ info script ]] ".."] base ]
lappend auto_path [ file join [file join [ file dirname [ info script ]] ".."] main ]

package provide mpjcardiacanalysis 1.2

package require  Itcl 3.2
package require  Iwidgets 4.0

package require  pxitcllistselector 1.0
package require  pxitclbaseimagecontrol 1.0
package require  pxitclbaseimageviewer 1.0

package require  vtkmpjcontrib 1.1
package require  mpjguilookuptable 1.0

itcl::class mpjcardiacanalysis {

    inherit pxitclbaseimagecontrol
    
    #-----------------------------------
    # construction and descruction
    #-----------------------------------
    constructor { par } {
	pxitclbaseimagecontrol::constructor $par
    } { InitializeControl }
    
    destructor { }

    #-----------------------------------
    # common variables
    #-----------------------------------

    #-----------------------------------
    # global variables
    #-----------------------------------
    private variable widget_list
    private variable global_list
    protected variable bis_algorithmmode 0
    protected variable bis_orientationimage 0
    
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
    public method LoadHeart { fname }
    public method LoadEpi { fname }
    public method LoadTensor { fname }
    public method ComputeResults { }
    public method SaveAllResults { }
    public method SaveResult { index }
    public method ClearAllResults { }
    public method DisplayResult { index { mode image } }
    public method ClearDisplay { }
    public method SetResultPreffix { preffix }
    public method GetResultPreffix { } 
    public method SetCurrentDirectory { fname } 
    public method SaveStatistics { fname }

    public method UpdateLookupControlsOnMap { }

    #-----------------------------------
    # interface creation methods
    #-----------------------------------
    private method CreateDialogs { base }
    private method CreateResultsControl { base }
    private method CreateDisplayControl { base }
    private method CreateInputControl { base }

    private method CreateMenu { mb }
    private method Clone { }
    public method AddToMenuButton { mb args }    
 
    private method loadmask { }
    private method loadheart { }
    private method loadepi { }
    private method loadtensor { }
    private method displayresult { { mode image } }
    private method loadresult { }
    private method saveresult { }
    private method toggleresult { }
    private method saveallresults { }
 
    private method mask_autochanged_cb { }

    private method solution_method_cb { } 
    private method view_update_display { }
    private method view_update_display_cb { }
    private method view_results_colormap_cb { }

    private method angle_type_cb { } 

    private method input_updateinfo { index }    

    private method result_clear { start end }
    private method result_createeigen { }
    private method result_createepi { }
    private method result_createspeed { }
    private method result_createangle { }
    private method result_propagate { }

    # Methods for bis_algorithm
    # -------------------------
    public method SetParametersFromBisAlgorithmAndCompute { cscheme satfactor brightness usemask } 
    public method GetOutputImage { index }
    public method GetBisDirectionImage { } { return $bis_orientationimage }
    public method SetBisAlgorithmMode { } { set bis_algorithmmode 1 }
    public method BisComputeDirectionMaps { } { set bis_algorithmmode 1; $this view_direction_update_cb; set bis_algorithmmode 0 }

}  


#-------------------------------------------------------------------------------------------
#  Close window 
# -------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::DismissWindow { } {
    
    if { $bis_algorithmmode == 1 } {
	$this HideWindow
	return
    }

    set TENSOR 0
    set MASK 1

    set ok "yes"
    
    set tensorsz [ [ lindex $global_list(result_list) $TENSOR ] GetImageSize ]
    set masksz [ [ lindex $global_list(result_list) $MASK ] GetImageSize ]

    if { [expr ($tensorsz > 1) || ($masksz > 1) ] } {
    
       set ok [ tk_messageBox -type yesnocancel -default no \
             -parent $basewidget -title "Think again ..." \
             -message "Your work session will be lost. Are you sure ?" -icon question  ]

    }
    
    if { $ok != "yes" } {
        return
    }

	$this ClearAllResults
	
	$this SetTitle $global_list(appname)
	
	set global_list(mask_fname) ""
    set global_list(heart_fname) ""
    set global_list(epi_fname) ""
	set global_list(tensor_fname) ""
    
    [ $parent GetViewer ] UpdateDisplay

	$this DestroyWindow
    
}

#-------------------------------------------------------------------------------------------
#  Initialize tool
# -------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::InitializeControl { } {

    set appname "Cardiac Diffusion Analysis $version"
    set aboutstring "(c) Marcel Jackowski 2014"

    set global_list(appname) "Cardiac Tensor Analysis"

    # create image titles
    set global_list(result_title) \
	[ list \
	      { "Diffusion tensor"        "dti_tensor" 1 } \
	      { "Region of interest mask" "dti_mask" 1 } \
          { "Heart mask"              "dti_heartmask" 1 } \
          { "Epicardium"              "dti_epimask" 1 } \
	      { "Eigenvalues"             "dti_eigenval" 1 } \
	      { "Eigenvectors"            "dti_eigenvec" 1 } \
          { "Propagation speed"       "fmm_speed" 1 } \
          { "Propagation solution"    "fmm_solution" 1 } \
          { "Propagation gradient"    "fmm_gradient" 1 } \
          { "Cardiac tensor"          "fam_axes_tensor" 1 } \
          { "Projection e1-C"         "fam_e1c" 1 } \
          { "Projection e1-R"         "fam_e1r" 1 } \
          { "Projection e1-L"         "fam_e1l" 1 } \
          { "Projection e2-C"         "fam_e2c" 1 } \
          { "Projection e2-R"         "fam_e2r" 1 } \
          { "Projection e2-L"         "fam_e2l" 1 } \
          { "Projection e3-C"         "fam_e3c" 1 } \
          { "Projection e3-R"         "fam_e3r" 1 } \
          { "Projection e3-L"         "fam_e3l" 1 } ]

    ## create empty images
    foreach item $global_list(result_title) {
 	lappend result_list [ pxitclimage \#auto ]
	set global_list(enable_list,[lindex $item 1]) 0
    }
    
    set global_list(result_fpreffix) "preffix_"
    set global_list(result_fsuffix) ".nii.gz"
    set global_list(result_fdir) [ pwd ] 
    set global_list(result_list) $result_list
    
    set global_list(mask_fname) ""
    set global_list(mask_auto) 1
    set global_list(mask_nreg) 1

    set global_list(epi_fname) ""
    set global_list(heart_fname) ""
    set global_list(tensor_fname) ""
    
    set global_list(eigen_recompute) 1
    set global_list(eigen_threshval) 0.7
    set global_list(eigen1) 1.0
    set global_list(eigen2) 0.6
    set global_list(eigen3) 0.3

    set global_list(solution_inf) 32767
    set global_list(solution_method) 0
    set global_list(solution_seeds) [ vtkFloatArray [ pxvtable::vnewobj ] ]
    $global_list(solution_seeds) SetNumberOfComponents 4
    $global_list(solution_seeds) InsertNextTuple4 0 0 0 0

    set global_list(angle_type) 0

    set global_list(stat_overwrite) 1
    set global_list(stat_normalize) 0

    set global_list(view_transparent) 1
    set global_list(view_transpcolor) 0.0

}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in mask auto status
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::mask_autochanged_cb { } {
    set state(0) disabled; set state(1) normal        
    $widget_list(mask_load) buttonconfigure load -state \
	$state([expr 1 - $global_list(mask_auto)])
    $widget_list(mask_load) buttonconfigure disp -state \
	$state([expr 1 - $global_list(mask_auto)])
    $widget_list(mask_fname) configure -state \
	$state([expr 1 - $global_list(mask_auto)])
    $widget_list(mask_threshval) configure -state \
	$state($global_list(mask_auto))
    $widget_list(mask_nreg) configure -state $state([expr 1 - $global_list(mask_auto)])
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Update input information on title bar
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::input_updateinfo { index } {

    set sel_input [ [ lindex $global_list(input_list) $index ] GetImage ]
    set sel_fname [ lindex $global_list(input_fname) $index ]
    
    scan [ $sel_input GetDimensions ] "%d %d %d" dx dy dz	       
    set nc [  $sel_input GetNumberOfScalarComponents ]
    set name [ file tail [ file root $sel_fname ] ]
    
    if { [ file extension $name ] == ".gz" } {
	set name [ file rootname $name ]
    }
    if { [ file extension $name ] == ".hdr" } {
	set name [ file rootname $name ]
	set global_list(result_fsuffix) ".hdr"
    } elseif { [ file extension $name ] == ".nii" } {
	set name [ file rootname $name ]
	set global_list(result_fsuffix) ".nii.gz"
    }
    
    set global_list(result_fpreffix) "${name}_"

    $this SetTitle "$global_list(appname): $name (${dx}x${dy}x${dz}, $nc frames)"
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load mask
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::loadmask { } {

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
itcl::body mpjcardiacanalysis::LoadMask { fname } {

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
	
    ## fixed on 11/1/04
	$mask CopyImageHeader [ $ana GetImageHeader ]
	$mask ShallowCopyImage [ $imcast GetOutput ]
	
	$imcast Delete
	
    } else {
	
	$mask SetFromAnalyzeSource $ana
    }
   
    ## determine range of values in mask
    #set range [ [ [ [ $mask GetImage ] GetPointData ] GetScalars ] GetRange ]

    ## take the maximum range as the number of regions in mask
    # -- force integer!
    #set global_list(mask_nreg) [ expr int([ lindex $range 1 ])]
    #$widget_list(mask_nreg) configure -range [ list 1 $global_list(mask_nreg) ]
    
    set global_list(mask_fname) $fname

    WatchOff

    $ana Delete
}


#-------------------------------------------------------------------------------------------
#  PRIVATE: Load mask
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::loadheart { } {

    ## browse for filename
    set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}

    set fname  [tk_getOpenFile -title "Select mask" -filetypes $typelist -initialdir $global_list(result_fdir) -parent $basewidget ] 
    
    if { $fname != "" } {
    
    set global_list(result_fdir) [ file dirname $fname ]
    
    ## load mask into memory
    LoadHeart $fname
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Load in heart mask image 
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::LoadHeart { fname } {

    set HEART 2     
    set heart [ lindex $global_list(result_list) $HEART ]

    ## load series into memory
    set ana [ vtkpxAnalyzeImageSource  [ pxvtable::vnewobj ]]
    $this SetFilterCallbacks $ana "Reading in heart mask image..."
    
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
    
    ## fixed on 11/1/04
    $heart CopyImageHeader [ $ana GetImageHeader ]
    $heart ShallowCopyImage [ $imcast GetOutput ]
    
    $imcast Delete
    
    } else {
    
        $heart SetFromAnalyzeSource $ana
    }
   
    ## determine range of values in mask
    #set range [ [ [ [ $mask GetImage ] GetPointData ] GetScalars ] GetRange ]

    ## take the maximum range as the number of regions in mask
    # -- force integer!
    #set global_list(mask_nreg) [ expr int([ lindex $range 1 ])]
    #$widget_list(mask_nreg) configure -range [ list 1 $global_list(mask_nreg) ]
    
    set global_list(heart_fname) $fname

    WatchOff

    $ana Delete
}
#-------------------------------------------------------------------------------------------
#  PRIVATE: Load mask
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::loadepi { } {

    ## browse for filename
    set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}

    set fname  [tk_getOpenFile -title "Select mask" -filetypes $typelist -initialdir $global_list(result_fdir) -parent $basewidget ] 
    
    if { $fname != "" } {
    
    set global_list(result_fdir) [ file dirname $fname ]
    
    ## load mask into memory
    LoadEpi $fname
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Load in mask image 
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::LoadEpi { fname } {

    set EPICARDIUM 3    
    set epicardium [ lindex $global_list(result_list) $EPICARDIUM ]

    ## load series into memory
    set ana [ vtkpxAnalyzeImageSource  [ pxvtable::vnewobj ]]
    $this SetFilterCallbacks $ana "Reading in epi image..."
    
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
        
        ## fixed on 11/1/04
        $epicardium CopyImageHeader [ $ana GetImageHeader ]
        $epicardium ShallowCopyImage [ $imcast GetOutput ]
        
        $imcast Delete
    
    } else {
    
        $epicardium SetFromAnalyzeSource $ana

    }
    
    set global_list(epi_fname) $fname

    WatchOff

    $ana Delete
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load tensor
# -----------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::loadtensor { } {

    ## browse for filename
    set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}

    set fname  [tk_getOpenFile -title "Select tensor" -filetypes $typelist -initialdir $global_list(result_fdir) -parent $basewidget ] 
    
    if { $fname != "" } {
	
	set global_list(result_fdir) [ file dirname $fname ]
	
	## load mask into memory
	LoadTensor $fname
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Load in tensor image 
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::LoadTensor { fname } {

    set TENSOR 0

    set tensor [ lindex $global_list(result_list) $TENSOR ]

    ## load series into memory
    set ana [ vtkpxAnalyzeImageSource  [ pxvtable::vnewobj ]]
    
    $this SetFilterCallbacks $ana "Reading in tensor image..."
    
    WatchOn
    
    ## load series 
    $ana Load $fname 
    
    $tensor SetFromAnalyzeSource $ana
    
    # determine coordinate transform
    set or [ $ana GetOrientation ]
    if { $or == 0 } {
     	set global_list(tensor_flipx) 1
     	set global_list(tensor_flipy) 1
    } elseif { $or == 1 } {
     	set global_list(tensor_flipx) 1
    } 
    
    set nc [ [ $tensor GetImage ] GetNumberOfScalarComponents ]

    set global_list(tensor_fname) $fname


    set name $fname
    if { [ file extension $name ] == ".gz" } {
	set name [ file rootname $name ]
    }
    if { [ file extension $name ] == ".hdr" } {
	set name [ file rootname $name ]
	set global_list(result_fsuffix) ".hdr"
    } elseif { [ file extension $name ] == ".nii" } {
	set name [ file rootname $name ]
	set global_list(result_fsuffix) ".nii.gz"
    }
    
    set global_list(result_fpreffix) "[file tail $name ]"
    set global_list(tensor_symm) [ expr $nc == 6 ]
    
    WatchOff
    
    $ana Delete
    
    $widget_list(computebtn) configure -state normal
    $widget_list(menu_results) entryconfigure 0 -state normal
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Set result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::SetResultPreffix { preffix } {
    #puts "mpjcardiacanalysis::SetCurrentDirectory $fname"
    set global_list(result_fpreffix) $preffix
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Get result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::GetResultPreffix { } {
    return $global_list(result_fpreffix)
}

itcl::body mpjcardiacanalysis::SetCurrentDirectory { fname } {
    #puts $fname
    set global_list(result_fdir) $fname
}


#-------------------------------------------------------------------------------------------
#  PUBLIC: Clear all results
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::ClearAllResults { } {
    
    foreach result $global_list(result_list) {
	$result Clear
    }

    if { $bis_orientationimage != 0 } {
	$bis_orientationimage Clear
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::SaveAllResults { } {

    foreach result $global_list(result_list) title $global_list(result_title) {
	

	set fname $global_list(result_fpreffix)
	set suffix $global_list(result_fsuffix)	
	set fname "$fname[ lindex $title 1 ]${suffix}"
	set dim [ lindex [ [ $result GetImage ] GetDimensions ] 0 ]
	if { $dim >1 } {
	    $result Save $fname
	}

    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: load results
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::loadresult { } {

    ## get current selection 
    set sel_item [ $widget_list(result_list) getcurselection ]
    
    if { $sel_item != "" } { 
	
	## browse for filename
	set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
	set fname  [tk_getOpenFile -title "Select image" -filetypes $typelist -initialdir $global_list(result_fdir) -parent $basewidget ] 
	
	if { $fname != "" } {
   	    
	    set item [ lindex $sel_item 0 ]
	    
	    set index [ $widget_list(result_list) index $item ]
	    set result [ lindex $global_list(result_list) $index ]
	    
	    $result Load $fname
	}
    }    
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Save single result
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::SaveResult { index } {        

    set fname $global_list(result_fpreffix)
    set suffix $global_list(result_fsuffix)	
    set fname "$fname[ lindex [lindex $global_list(result_title) $index ] 1 ]${suffix}"

    [lindex $global_list(result_list) $index] Save $fname
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::saveallresults { } {

    ## browse for directory
    set fdir [ tk_chooseDirectory -title " Select directory" -parent $basewidget -initialdir $global_list(result_fdir) ]

    if { $fdir != "" } {
	
	set global_list(result_fdir) $fdir
	
	foreach result $global_list(result_list) title $global_list(result_title) {
	    
	    set fname $global_list(result_fpreffix)
	    set suffix $global_list(result_fsuffix)	
	    set fname "$fname[ lindex $title 1 ]${suffix}"
	    
	    set dim [ lindex [ [ $result GetImage ] GetDimensions ] 0 ]
	    if { $dim >1 } {
		$result Save [ file join $fdir $fname ]
	    }
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save results
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::saveresult { } {
    
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
		set suffix $global_list(result_fsuffix)	
		set fname "$fname[ lindex [lindex $global_list(result_title) $index ] 1 ]${suffix}"
		
		$result Save [ file join $fdir $fname ]
	    }
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Display result
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::DisplayResult { index { mode image } } {

    # ----------------------------------------
    # Xenios -- this is the image display part
    # ----------------------------------------

    set imgtodisplay [ [ lindex $global_list(result_list) $index ] GetThisPointer]
    set rawimg  [ $imgtodisplay GetImage ] 
    set r [ [ [ $rawimg GetPointData ] GetScalars ] GetRange ]

    set rmin [ lindex $r 0 ]
    set rmax [ lindex $r 1 ]


    # Step 1.
    # Update Lookup table range and lookup table
    
    # this is in the object resultluk -- see roughly line 2893
    # SetSaturationRange -- use this from image range!
    # Update

    set resultluk  $widget_list(view_resultluk) 

    #$resultluk SetSaturationRange $rmin $rmax
    #$resultluk SetTableRange $rmin $rmax
    #$resultluk Update

    #[ $parent GetViewer ] SetLookupTable $luk
    #[ $parent GetViewer ] UpdateDisplay


    # Step 2.
    # Ship Image and Lookup Table 

    if { $mode == "image" } {      
	$parent SetImageFromObject $imgtodisplay $this
	[ $parent GetViewer ] SetLookupTable [ $resultluk GetLookupTable ]
	[ $parent GetViewer ] UpdateDisplay
	return
    }
     
    if { $mode == "input"} {
    $parent SetImageFromObject $imgtodisplay $this
    [ $parent GetViewer ] UpdateDisplay
    return
    }

    $parent SetMaskFromObject $imgtodisplay $this
    #[ $parent GetViewer ]  SetObjectLookupTable [ $resultluk GetLookupTable ]
    [ $parent GetViewer ] UpdateDisplay

    return
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Display result
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::displayresult { { mode image } } {

    set sel_item [ $widget_list(result_list) getcurselection ]
	    
    if { $sel_item != "" } {
	set index [ $widget_list(result_list) index [ lindex $sel_item 0 ] ]
	set result [ lindex $global_list(result_list) $index ]

	#set range [ [ [ [ $result GetImage ] GetPointData ] GetScalars ] GetRange ]
	#$widget_list(view_resultluk) SetTableRange [ lindex $range 0 ] [ lindex $range 1 ]
	#	$widget_list(view_resultluk) Update

	$this DisplayResult $index $mode
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: toggle result computation on/off
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::toggleresult { } {
   
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
itcl::body mpjcardiacanalysis::result_clear { start end } {
    for { set i $start } { $i <= $end } { incr i } {
	    [ lindex $global_list(result_list) $i ] Clear
	}
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create eigensystem
#-------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::result_createeigen { } {

    set TENSOR 0
    set MASK 1
    set HEART 2
    set EPICARDIUM 3
    set EIGENVALUES 4
    set EIGENVECTORS 5

    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]
    
    if { [expr ([ $mask GetImageSize ] == 1) || ([ $tensor GetImageSize ] == 1) ] } {
	return 0
    }

    ## compute eigenvalues and eigenvectors
    set eigen [ vtkmpjImageTensorEigenAnalysis [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $eigen "Computing eigenvalues and eigenvectors..."
    $eigen SetInput [ $tensor GetImage ]
    $eigen SetOrdering 0
    $eigen SetMask [ $mask GetImage ]
    $eigen SetOutputTypeToBoth    
    $eigen MaskNegativeOn
    $eigen SetSortByMagnitude 1
    $eigen SetMaskNegativeInValue 0
    $eigen SetMaskNegativeOutValue 255
    $eigen Update
    
    ## extract mask with negative eigenvalues
    #$negative ShallowCopyImage [ $eigen GetNegativeMask ]
    
    ## extract eigenvalues
    set fex [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $fex "Extracting eigenvalues..."
    $fex SetInput [ $eigen GetOutput ]
    $fex SetStartFrame 0
    $fex SetEndFrame 2
    $fex Update

    $eigenvalues ShallowCopyImage [ $fex GetOutput ]
    
    $fex Delete
    
    ## extract eigenvectors
    set fex [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $fex "Extracting eigenvectors..."
    $fex SetInput [ $eigen GetOutput ]
    $fex SetStartFrame 3
    $fex SetEndFrame 11
    $fex Update
    
    #### TEMP CHANGE #####
    #set center [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    #set cx [ lindex $center 0 ]
    #set cy [ lindex $center 1 ]
    #set cz [ lindex $center 2 ]

    #set rad [ vtkmpjRadialCoordinates [ pxvtable::vnewobj ] ]
    #$rad SetInput [ $fex GetOutput ]
    #$rad SetMask [ $mask GetImage ]
    #$rad SetCenter $cx $cy $cz
    #$rad Update

    #$eigenvectors ShallowCopyImage [ $rad GetOutput ]

    #$rad Delete
    $eigenvectors ShallowCopyImage [ $fex GetOutput ]

    #### END TEMP CHANGE

    $eigenvalues CopyImageHeader [ $mask GetImageHeader ]
    $eigenvectors CopyImageHeader [ $mask GetImageHeader ]


    $fex Delete

    $eigen Delete

    update idletasks

    return 1
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create maps
#-------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::result_createepi { } {
    
    set TENSOR 0
    set MASK 1
    set HEART 2
    set EPICARDIUM 3
    set EIGENVALUES 4
    set EIGENVECTORS 5

    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]
    set epicardium [ lindex $global_list(result_list) $EPICARDIUM ]

#    puts stderr "Tensor spacing = [  [ $tensor GetImage ] GetSpacing ]"
#    puts stderr "Tensor description = [ $tensor GetDescription ]\n Mask = [ $mask GetDescription ]\n"
#    set deletemapmask 0
#    $mapmask CopyImageHeader [ $mask GetImageHeader ]    
#   puts stderr "Here we are done ....."
#    puts stderr "Eigenvalues = [ $eigenvalues GetDescription ] , spa=[ [ $eigenvalues GetImage ] GetSpacing ]"
#    puts stderr "MapMask     = [ $mapmask GetDescription ] , spa= [ [ $mapmask GetImage ] GetSpacing ]"
    
    ## calculate epicardium
    #set epi [ vtkmpjImageBoundary [ pxvtable::vnewobj ] ]
    #$this SetFilterCallbacks $epi "Extracting epicardium..."
    #$epi SetInput [ $mask GetImage ]
    #$epi SetBoundary2D 0
    #$epi SetMaskOutValue 0
    #$epi SetBoundaryOutValue 1
    #$epi Update

    #$epicardium ShallowCopyImage [ $epi GetOutput ]
    #$epicardium CopyImageHeader [ $mask GetImageHeader ]

    # populate solution seeds
    $global_list(solution_seeds) Reset
    $global_list(solution_seeds) SetNumberOfComponents 4

    if { [ $epicardium GetImageSize ] == 1 } {
        return 0
    }

    set image [ $epicardium GetImage ]
    set dim [ $image GetDimensions ]
    set dimx [ lindex $dim 0 ]
    set dimy [ lindex $dim 1 ]
    set dimz [ lindex $dim 2 ]

    for { set i 0} { $i < [expr $dimx - 1] } { incr i } {
        for { set j 0} { $j < [expr $dimy - 1]} { incr j } {
            for { set k 0} { $k < [expr $dimz - 1]} { incr k } {
                set value [ $image GetScalarComponentAsDouble $i $j $k 0 ]
                    if { $value > 0 } {
                        $global_list(solution_seeds) InsertNextTuple4 $i $j $k 0.0
                        #puts "$i $j $k 0.0"
                    }
                }
            }
        }


    #$epi Delete

    #    $mapmask Clear
    #if { $deletemapmask > 0 } {
    #itcl::delete object $mapmask
    #}

    update idletasks

    return 1
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create maps
#-------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::result_createspeed { } {

    set TENSOR 0
    set MASK 1
    set HEART 2
    set EPICARDIUM 3
    set EIGENVALUES 4
    set EIGENVECTORS 5
    set SPEED 6

    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set heart [ lindex $global_list(result_list) $HEART ]
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]
    set epicardium [ lindex $global_list(result_list) $EPICARDIUM ]
    set speed [ lindex $global_list(result_list) $SPEED ]

    ## threshold baseline image
    set thresh [ vtkImageThreshold [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $thresh "Creating propagation speed image..."
    $thresh SetInput [ $heart GetImage ]
    $thresh ThresholdByUpper 1
    $thresh SetInValue 1
    $thresh SetOutValue 0
    $thresh SetOutputScalarTypeToUnsignedChar
    $thresh Update

    $speed ShallowCopyImage [ $thresh GetOutput ]
    $speed CopyImageHeader [ $heart GetImageHeader ]

}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create maps
#-------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::result_propagate { } {

    set VTK_MPJ_LARGE_FLOAT 32767
    set VTK_MPJ_UNKNOWN     0 

    set TENSOR 0
    set MASK 1
    set HEART 2
    set EPICARDIUM 3
    set EIGENVALUES 4
    set EIGENVECTORS 5
    set SPEED 6
    set SOLUTION 7
    set GRADIENT 8
    set AXES 9

    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set heart [ lindex $global_list(result_list) $HEART ]
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]
    set epicardium [ lindex $global_list(result_list) $EPICARDIUM ]
    set speed [ lindex $global_list(result_list) $SPEED ]
    set solution [ lindex $global_list(result_list) $SOLUTION ]
    set gradient [ lindex $global_list(result_list) $GRADIENT ]
    set axes [ lindex $global_list(result_list) $AXES ]
  
    WatchOn

    update idletasks   
    
    result_createspeed

    set spc [ [ $speed GetImage ] GetSpacing ]
    set dim [ [ $speed GetImage ] GetDimensions ]

    #set levels [ [ $viewer GetOrthoImageSlice ] GetLevels ]
    #set x [ lindex $levels 0 ]
    #set y [ lindex $levels 1 ]
    #set z [ lindex $levels 2 ]    
    
    ## set seed point
    #set seeds [ vtkFloatArray [ pxvtable::vnewobj ] ]
    #$seeds SetNumberOfComponents 4
    #$seeds InsertNextTuple4 $x $y $z 0
    
    ## FMM
    set sol_method [ vtkmpjFastMarchingMethod [ pxvtable::vnewobj ] ]
    $sol_method SetKnownPoints $global_list(solution_seeds)
    $sol_method SetInput [ $speed GetImage ]    
    #if { [ $mask GetImageSize ] > 1 } {
        #if { $global_list(solution_usemask) } {
    #        $sol_method SetMask [ $mask GetImage ]  
        #}
    #} 
    ## accuracy
    $sol_method SetSecondOrder $global_list(solution_method)

    ## set spacing
    $sol_method SetSpacing \
        [lindex $spc 0 ] \
        [lindex $spc 1 ] \
        [lindex $spc 2 ]

    $sol_method SetInfinity $global_list(solution_inf)

    ## FMM
    $this SetFilterCallbacks $sol_method "Propagating..."
    
    ## execute
    $sol_method Update

    set time [ $sol_method GetElapsedTime ]
    
    puts "After propagation"

    ## calculate gradient
    #set imgrad [ vtkImageGradient [ pxvtable::vnewobj ] ]
    #$imgrad SetDimensionality 3
    set imgrad [ vtkmpjLevelSetNormal [ pxvtable::vnewobj ] ]
    $imgrad SetInput [ $sol_method GetOutput ]
    $this SetFilterCallbacks $imgrad "Calculating gradient..."
    $imgrad Update

    $gradient ShallowCopyImage [ $imgrad GetOutput ]  
    $gradient CopyImageHeader [ $solution GetImageHeader ]

    ## calculate projection plane
    set imgpl [ vtkmpjCardiacCoordinates [ pxvtable::vnewobj ] ]
    $imgpl SetInput [ $imgrad GetOutput ]
    $imgpl SetMask [ $heart GetImage ]
    $imgpl SetOutputTensor 1
    $imgpl SetEigenvalues $global_list(eigen1) $global_list(eigen2) $global_list(eigen3)
    $this SetFilterCallbacks $imgpl "Calculating coordinate system..."
    $imgpl Update
    
    $imgrad Delete

    $axes ShallowCopyImage [ $imgpl GetOutput ]  
    $axes CopyImageHeader [ $solution GetImageHeader ]
    
    #$imgrad Delete
    $imgpl Delete
    
    ## threshold result for display purposes
    set imthr [ vtkImageThreshold [ pxvtable::vnewobj ] ]
    $imthr SetInput [ $sol_method GetOutput ]
    $imthr ThresholdBetween 0 $global_list(solution_inf)
    $imthr ReplaceOutOn  
    $imthr SetOutValue $global_list(solution_inf)
    $imthr SetOutputScalarTypeToFloat
    $this SetFilterCallbacks $imthr "Thresholding solution..."
    $imthr Update

    $solution ShallowCopyImage [ $imthr GetOutput ]  
    $solution CopyImageHeader [ $solution GetImageHeader ]
    
    $sol_method Delete 
    
    $imthr Delete

    # send currently selected result to parent
    #$widget_list(result_list) selection set $SOLUTION $SOLUTION
    
    puts "after gradient computation"

    #set sol_map [ lindex $global_list(result_list) $SOLUTION ]
#    $parent SetImageFromObject [ $sol_map  GetThisPointer ] $this

    WatchOff

    $this ShowProgressVal "Done ($time secs)." 0
}   

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create maps
#-------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::result_createangle { } {

    set TENSOR 0
    set MASK 1
    set HEART 2
    set EPICARDIUM 3
    set EIGENVALUES 4
    set EIGENVECTORS 5
    set SPEED 6
    set SOLUTION 7
    set GRADIENT 8
    set AXES 9

    set ANGLE_E1C 10
    set ANGLE_E1R 11
    set ANGLE_E1L 12

    set ANGLE_E2C 13
    set ANGLE_E2R 14
    set ANGLE_E2L 15

    set ANGLE_E3C 16
    set ANGLE_E3R 17
    set ANGLE_E3L 18

    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set heart [ lindex $global_list(result_list) $HEART ]
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]
    set epicardium [ lindex $global_list(result_list) $EPICARDIUM ]
    set speed [ lindex $global_list(result_list) $SPEED ]
    set solution [ lindex $global_list(result_list) $SOLUTION ]
    set gradient [ lindex $global_list(result_list) $GRADIENT ]
    set axes [ lindex $global_list(result_list) $AXES ]
  
    set angle_e1C [ lindex $global_list(result_list) $ANGLE_E1C ]
    set angle_e1R [ lindex $global_list(result_list) $ANGLE_E1R ]
    set angle_e1L [ lindex $global_list(result_list) $ANGLE_E1L ]

    set angle_e2C [ lindex $global_list(result_list) $ANGLE_E2C ]
    set angle_e2R [ lindex $global_list(result_list) $ANGLE_E2R ]
    set angle_e2L [ lindex $global_list(result_list) $ANGLE_E2L ]

    set angle_e3C [ lindex $global_list(result_list) $ANGLE_E3C ]
    set angle_e3R [ lindex $global_list(result_list) $ANGLE_E3R ]
    set angle_e3L [ lindex $global_list(result_list) $ANGLE_E3L ]

    WatchOn

    #puts "Calculating local cardiac coordinates..."
    set coords [ vtkmpjCardiacCoordinates [ pxvtable::vnewobj ] ]
    $coords SetInput [ $gradient GetImage ]
    $coords SetMask [ $mask GetImage ]
    $coords Update

    puts "Calculating fiber angle..."
    set fiberangle [ vtkmpjCardiacAngle [ pxvtable::vnewobj ] ]
    $fiberangle SetInput [ $coords GetOutput ]
    $fiberangle SetMask [ $mask GetImage ]
    $fiberangle SetEigenvectors [ $eigenvectors GetImage ]
    $fiberangle SetAngleType $global_list(angle_type)

    ## e1C
    $fiberangle SetEigenvectorType 0
    $fiberangle SetReferencePlane 0
    $fiberangle Update

    $angle_e1C ShallowCopyImage [ $fiberangle GetOutput ]  
    $angle_e1C CopyImageHeader [ $mask GetImageHeader ]
    
    ## e1R
    $fiberangle SetEigenvectorType 0
    $fiberangle SetReferencePlane 1
    $fiberangle Update

    $angle_e1R ShallowCopyImage [ $fiberangle GetOutput ]  
    $angle_e1R CopyImageHeader [ $mask GetImageHeader ]

    ## e1L
    $fiberangle SetEigenvectorType 0
    $fiberangle SetReferencePlane 2
    $fiberangle Update

    $angle_e1L ShallowCopyImage [ $fiberangle GetOutput ]  
    $angle_e1L CopyImageHeader [ $mask GetImageHeader ]


    ## angle_e2C
    $fiberangle SetEigenvectorType 1
    $fiberangle SetReferencePlane 0
    $fiberangle Update

    $angle_e2C ShallowCopyImage [ $fiberangle GetOutput ]  
    $angle_e2C CopyImageHeader [ $mask GetImageHeader ]

    ## angle_e2R
    $fiberangle SetEigenvectorType 1
    $fiberangle SetReferencePlane 1
    $fiberangle Update

    $angle_e2R ShallowCopyImage [ $fiberangle GetOutput ]  
    $angle_e2R CopyImageHeader [ $mask GetImageHeader ]

    ## angle_e2L
    $fiberangle SetEigenvectorType 1
    $fiberangle SetReferencePlane 2
    $fiberangle Update

    $angle_e2L ShallowCopyImage [ $fiberangle GetOutput ]  
    $angle_e2L CopyImageHeader [ $mask GetImageHeader ]


    ## angle_e3C
    $fiberangle SetEigenvectorType 2
    $fiberangle SetReferencePlane 0
    $fiberangle Update

    $angle_e3C ShallowCopyImage [ $fiberangle GetOutput ]  
    $angle_e3C CopyImageHeader [ $mask GetImageHeader ]

    ## angle_e3R
    $fiberangle SetEigenvectorType 2
    $fiberangle SetReferencePlane 1
    $fiberangle Update

    $angle_e3R ShallowCopyImage [ $fiberangle GetOutput ]  
    $angle_e3R CopyImageHeader [ $mask GetImageHeader ]

    ## angle_e3L
    $fiberangle SetEigenvectorType 2
    $fiberangle SetReferencePlane 2
    $fiberangle Update

    $angle_e3L ShallowCopyImage [ $fiberangle GetOutput ]  
    $angle_e3L CopyImageHeader [ $mask GetImageHeader ]

    $coords Delete

    $fiberangle Delete

    #if { $plane_type == 0 } {
    #    anaout Save "${filename1}_e${eigen_type}_circ_anglemap"
    #} elseif { $plane_type == 1 } {
    #    anaout Save "${filename1}_e${eigen_type}_rad_anglemap"  
    #} else {
    #    anaout Save "${filename1}_e${eigen_type}_long_anglemap"
    #}

    WatchOff

    return 1
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Compute results
# -----------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::ComputeResults { } {
 
    set TENSOR 0
    set MASK 1
    set HEART 2
    set EPICARDIUM 3
    set EIGENVALUES 4
    set EIGENVECTORS 5
    set SPEED 6
    set SOLUTION 7
    set GRADIENT 8
    set AXES 9
    set ANGLE_E1C 10

    WatchOn
    
    ## clear results
    #if { $global_list(eigen_recompute) } {
	   #result_clear $MASK [ expr [ llength $global_list(result_list) ] -1 ]
    #} else {
	   #result_clear $EIGENVAL [ expr [ llength $global_list(result_list) ] -1 ]
    #}

    $widget_list(notebook) view "Results"; update
    
    if { $global_list(eigen_recompute) } {

        ## compute eigevalues and eigenvectors
        set result [ result_createeigen ]

        if { $result == 0 } {
            WatchOff; return
        }
    }

    ## create maps
    #result_createmaps
    set result [ result_createepi ]
    if { $result == 0 } {
        WatchOff; return
    }

    result_propagate

    result_createangle

    $this ShowProgressVal "Done." 0
    

    # send currently selected result to parent
    $widget_list(result_list) selection set $ANGLE_E1C $ANGLE_E1C
    
    set anglee1c_map [ lindex $global_list(result_list) $ANGLE_E1C ]
    $parent SetImageFromObject [ $anglee1c_map  GetThisPointer ] $this

    #set range [ [ [ [ $tensor_map GetImage ] GetPointData ] GetScalars ] GetRange ]
    #$widget_list(view_resultluk) SetTableRange [ lindex $range 0 ] [ lindex $range 1 ]
    #$widget_list(view_resultluk) Update
    
    ## update colormaps
    view_results_colormap_cb
    
    ## update display
    view_update_display

    WatchOff
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in colormaps for result images
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::view_results_colormap_cb { } {

    set luk [ $widget_list(view_resultluk) GetLookupTable ]
    
    if { $global_list(view_transparent) } {
#   set rgba [ $luk GetTableValue 0 ]
    set index [ $luk GetIndex $global_list(view_transpcolor) ]
#   puts "color: $global_list(view_transpcolor) $index"
    set rgba [ $luk GetTableValue $index ]
    $luk SetTableValue $index [ lindex $rgba 0 ] [ lindex $rgba 1 ] [ lindex $rgba 2 ] 0.0
    } else {
    $widget_list(view_resultluk) Update
    set luk [ $widget_list(view_resultluk) GetLookupTable ]
    }

    # --------------------------
    # Update Image Lookup Table
    # --------------------------

    # Manipulate luk on the fly to set the range
    #    [ $parent GetViewer ] SetLookupTable $luk
    # Send this to overlay lookup table
    [ $parent GetViewer ] SetLookupTable $luk
    [ $parent GetViewer ] UpdateDisplay


}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in solution methods
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::angle_type_cb { } {
    set global_list(angle_type) \
    [ $widget_list(angle_type) index [ $widget_list(angle_type) get ] ]    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in solution methods
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::solution_method_cb { } {
    set global_list(solution_method) \
    [ $widget_list(solution_method) index [ $widget_list(solution_method) get ] ]    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Update display
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::view_update_display { } {

    if { $bis_algorithmmode == 0 } {
    ##[ $parent GetViewer ] SetDisplayMode3DIfNeeded
        [ $parent GetViewer ] UpdateDisplay
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC CALLBACK: Invoked upon an update button press
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::view_update_display_cb { } { 

       view_update_display
}

#-------------------------------------------------------------------------------------------
#  Create tensor control
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::CreateInputControl { base } {
    
    iwidgets::labeledframe $base.frame0 \
    -labelpos nw  \
    -labeltext "Tensor image"
    pack $base.frame0 -fill both -expand f -pady 5
    
    set frame0 [ $base.frame0 childsite ]
    
    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -side top -fill both -expand f -pady 0
    
    set tensor_fname [ iwidgets::entryfield $subframe0.tensor_fname \
               -textvariable [ itcl::scope global_list(tensor_fname) ] \
               -labeltext "Filename:" ] 
    pack $tensor_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(tensor_fname) $tensor_fname
    
    set tensor_load [ iwidgets::buttonbox $subframe0.tensor_load -orient horizontal -padx 2 ]
    $tensor_load add load -text "Load..." -command [ itcl::code $this loadtensor ] 
    $tensor_load add disp -text "Display" -command [ itcl::code $this DisplayResult 0 "input" ]
    pack $tensor_load -side left
    set widget_list(tensor_load) $tensor_load

    set eigen_recompute [ checkbutton $subframe0.eigen_recompute \
                  -text "Always recompute eigensystem" \
                  -variable [ itcl::scope global_list(eigen_recompute) ] ]
    pack $eigen_recompute -side right -fill both -padx 5 -pady 5


    iwidgets::labeledframe $base.frame1 \
    -labelpos nw  \
    -labeltext "Region of interest mask"
    pack $base.frame1 -fill both -expand f -pady 5
    

    set frame1 [ $base.frame1 childsite ]

    set subframe1 [ frame $frame1.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 0
    
    set mask_fname [ iwidgets::entryfield $subframe1.mask_fname \
             -textvariable [ itcl::scope global_list(mask_fname) ] \
             -labeltext "Filename:" ] 
    pack $mask_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(mask_fname) $mask_fname
    
    set mask_load [ iwidgets::buttonbox $subframe1.mask_load -orient horizontal -padx 2 ]
    $mask_load add load -text "Load..." -command [ itcl::code $this loadmask ]    
    $mask_load add disp -text "Display" -command [ itcl::code $this DisplayResult 1 "input" ]
    pack $mask_load -side left 
    set widget_list(mask_load) $mask_load


    iwidgets::labeledframe $base.frame2 \
    -labelpos nw  \
    -labeltext "Heart mask (filled)"
    pack $base.frame2 -fill both -expand f -pady 5
    

    set frame2 [ $base.frame2 childsite ]

    set subframe2 [ frame $frame2.subframe2 ]
    pack $subframe2 -side top -fill both -expand f -pady 0
    
    set heart_fname [ iwidgets::entryfield $subframe2.heart_fname \
             -textvariable [ itcl::scope global_list(heart_fname) ] \
             -labeltext "Filename:" ] 
    pack $heart_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(heart_fname) $heart_fname
    
    set heart_load [ iwidgets::buttonbox $subframe2.heart_load -orient horizontal -padx 2 ]
    $heart_load add load -text "Load..." -command [ itcl::code $this loadheart ]    
    $heart_load add disp -text "Display" -command [ itcl::code $this DisplayResult 2 "input" ]
    pack $heart_load -side left 
    set widget_list(heart_load) $heart_load


    iwidgets::labeledframe $base.frame3 \
    -labelpos nw  \
    -labeltext "Left ventricular boundary"
    pack $base.frame3 -fill both -expand f -pady 5
    
    set frame3 [ $base.frame3 childsite ]

    set subframe3 [ frame $frame3.subframe3 ]
    pack $subframe3 -side top -fill both -expand f -pady 0
    
    set epi_fname [ iwidgets::entryfield $subframe3.epi_fname \
             -textvariable [ itcl::scope global_list(epi_fname) ] \
             -labeltext "Filename:" ] 
    pack $epi_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(epi_fname) $epi_fname
    
    set epi_load [ iwidgets::buttonbox $subframe3.epi_load -orient horizontal -padx 2 ]
    $epi_load add load -text "Load..." -command [ itcl::code $this loadepi ]    
    $epi_load add disp -text "Display" -command [ itcl::code $this DisplayResult 3 "input "]
    pack $epi_load -side left 
    set widget_list(epi_load) $epi_load


    set frame4 [ frame $base.frame4 ]
    pack $frame4 -side top -fill both -expand f 

    set solution_method [ iwidgets::optionmenu $frame4.solution_method \
                  -command [ itcl::code $this solution_method_cb ] \
                  -labeltext "Propagation accuracy:" ]
    pack $solution_method -side left -padx 5 -pady 5
    $solution_method insert end "First-order"
    $solution_method insert end "Second-order"
    set widget_list(solution_method) $solution_method

    set angle_type [ iwidgets::optionmenu $frame4.angle_type \
                  -command [ itcl::code $this angle_type_cb ] \
                  -labeltext "Angle type:" ]
    pack $angle_type -side right -padx 10 -pady 5
    $angle_type insert end "0, 90"
    $angle_type insert end "-90, 90"
    $angle_type insert end "90, -90"
    $angle_type insert end "0, 180"
    set widget_list(angle_type) $angle_type

    #set eigen_recompute [ checkbutton $frame2.eigen_recompute \
    #              -text "Always recompute eigensystem" \
    #              -variable [ itcl::scope global_list(eigen_recompute) ] ]
    #pack $eigen_recompute -side right -fill both -padx 5 -pady 5

    set frame5 [ frame $base.frame5 ]
    pack $frame5 -side top -fill both -expand f 

    set widget_list(computebtn) [ button $frame5.compute -text "Compute!" \
                      -state disabled \
                      -command [ itcl::code $this ComputeResults ] ]    
    pack $frame5.compute -side left -fill x -expand f -padx 5 -pady 10  


}

#-------------------------------------------------------------------------------------------
#  Create results control
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::CreateResultsControl { base } {   

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
    $resultbbox add display -text "Display" -command [ itcl::code $this displayresult image ]
    $resultbbox add load -text "Load..." -command [ itcl::code $this loadresult ]
    $resultbbox add odisplay -text "Overlay" -command [ itcl::code $this displayresult overlay ]
    $resultbbox add save -text "Save..." -command [ itcl::code $this saveresult ]
    $resultbbox add saveall -text "Save all" -command [ itcl::code $this saveallresults ]    
    $resultbbox add mark -text "On/Off" -command [ itcl::code $this toggleresult ]

    pack $resultbbox -side top -fill x -expand t -padx 0
    set widget_list(result_buttonbox) $resultbbox

    set subframe3 [ frame $frame0.subframe3 ]
    pack $subframe3 -side top -fill x -padx 2 -pady 5

    set result_fpreffix [ iwidgets::entryfield $subframe3.result_fpreffix \
                  -textvariable [ itcl::scope global_list(result_fpreffix) ] \
                  -labeltext "Preffix:" ]
    pack $result_fpreffix -side left -fill x -expand t -padx 5 -pady 2
    set widget_list(result_fpreffix) $result_fpreffix    


    set subframe4 [ frame $frame0.subframe4 ]
    pack $subframe4 -side top -fill x -padx 2 -pady 5

    iwidgets::labeledframe $subframe4.fam \
    -labelpos nw  \
    -labeltext "Fiber architecture matrix"
    pack $subframe4.fam -fill both -expand t 

    set cs [ $subframe4.fam childsite ]
    set subframe_fam [ frame $cs.subframe_fam ]
    pack $subframe_fam -side top -fill both -padx 2 -pady 5
    
    set fam_label [ label $subframe_fam.fam_label -text "FAM = " ]

    set e1C_label [ label $subframe_fam.e1c_label -text "|  Proj e1-C" ]
    set e1R_label [ label $subframe_fam.e1r_label -text "  Proj e1-R" ]
    set e1L_label [ label $subframe_fam.e1l_label -text "  Proj e1-L  |" ]

    set e2C_label [ label $subframe_fam.e2c_label -text "|  Proj e2-C" ]
    set e2R_label [ label $subframe_fam.e2r_label -text "  Proj e2-R" ]
    set e2L_label [ label $subframe_fam.e2l_label -text "  Proj e2-L  |" ]

    set e3C_label [ label $subframe_fam.e3c_label -text "|  Proj e3-C" ]
    set e3R_label [ label $subframe_fam.e3r_label -text "  Proj e3-R" ]
    set e3L_label [ label $subframe_fam.e3l_label -text "  Proj e3-L  |" ]

    grid $fam_label -row 1 -column 0 -sticky e

    grid $e1C_label -row 0 -column 1 -sticky ne
    grid $e1R_label -row 0 -column 2 -sticky n
    grid $e1L_label -row 0 -column 3 -sticky nw

    grid $e2C_label -row 1 -column 1 -sticky e
    grid $e2R_label -row 1 -column 2 -sticky nsew
    grid $e2L_label -row 1 -column 3 -sticky w

    grid $e3C_label -row 2 -column 1 -sticky se
    grid $e3R_label -row 2 -column 2 -sticky s
    grid $e3L_label -row 2 -column 3 -sticky sw

    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -side top -fill x -padx 2 -pady 5

    iwidgets::entryfield $subframe2.eigen1\
    -width 5 \
    -validate real \
    -labeltext " Cardiac tensor - Eigenvalue 1:" \
    -textvariable [ itcl::scope global_list(eigen1) ]
    pack $subframe2.eigen1 -side left -pady 5

    iwidgets::entryfield $subframe2.eigen2\
    -width 5 \
    -validate real \
    -labeltext "Eigenvalue 2:" \
    -textvariable [ itcl::scope global_list(eigen2) ]
    pack $subframe2.eigen2 -side left -pady 5

    iwidgets::entryfield $subframe2.eigen3\
    -width 5 \
    -validate real \
    -labeltext "Eigenvalue 3:" \
    -textvariable [ itcl::scope global_list(eigen3) ]
    pack $subframe2.eigen3 -side left -pady 5
}

#-------------------------------------------------------------------------------------------
#  Create display control
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::UpdateLookupControlsOnMap { } {

    set mlist [ list  [ $widget_list(view_resultluk) GetRenderWidget ]  [ $widget_list(view_vectorluk) GetRenderWidget ]  [ $widget_list(view_tensorluk) GetRenderWidget ] ]

    foreach widg $mlist {
	[ $widg GetRenderWindow ] Render
	update idletasks
    }
}

#-------------------------------------------------------------------------------------------
#  Create result display control
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::CreateDisplayControl { base } {
    
    set subframe1 [ frame $base.subframe1 ]
    pack $subframe1 -side top -padx 5 -pady 5   

    set resultluk [ mpjguilookuptable \#auto 0 ]
    $resultluk Initialize $subframe1
    pack forget [ $resultluk GetButtonBoxWidget ]
#    pack forget [ $resultluk GetMenuBar ]
    $resultluk SetHueRange 0.0 0.2
    $resultluk SetSaturationRange 1.0 0.5
    $resultluk SetValueRange 0.5 1.0
    $resultluk Update
    $resultluk Show

    set widget_list(view_resultluk) $resultluk    
    
    set resultdisplaybtn [ button [ $resultluk GetMenuBar ].resultdisplaybtn \
                   -text "Apply!" \
                   -command [ itcl::code $this view_results_colormap_cb ]  ]
    pack $resultdisplaybtn -side left -padx 5 -pady 5
    

    set subframe0 [ frame [ $resultluk GetMenuBar ].subframe0 ]
    pack $subframe0 -fill x -side bottom -expand f -padx 5 -pady 5

    set transparent [ checkbutton $subframe0.transparent \
              -variable [ itcl::scope global_list(view_transparent) ] ]    
    pack $transparent -side left -pady 5       
    set widget_list(view_transparent) $transparent

    iwidgets::entryfield $subframe0.view_transpcolor \
    -width 5 \
    -validate real \
    -labeltext "Transparent color:" \
    -textvariable [ itcl::scope global_list(view_transpcolor) ]
    pack $subframe0.view_transpcolor -side left -pady 5
    
}

# -------------------------------------------------------------------------------------------
#  Diffusion control inititialization
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::Initialize { widget } {
    
    if { $initialized == 1 } { return $basewidget }
    
    #  -------------------------------------
    #  Create User Interface
    #  -------------------------------------	
    set basewidget [toplevel $widget ]
    wm geometry $basewidget 650x650
    wm withdraw $basewidget

    set notebook $basewidget.notebook    
    iwidgets::tabnotebook $notebook -tabpos w         
    set widget_list(notebook) $notebook

    set mb [ frame $basewidget.mb ]
    pack $mb -side top -fill x -expand false
    
    CreateMenu $mb
    
    #button $mb.refresh -text "Refresh" -borderwidth 0 \
    #	-command [ itcl::code $this view_update_display_cb ] -underline 2
    #pack $mb.refresh -side left -padx 0

    CreateInputControl     [ $notebook add -label "Input" ]
    CreateResultsControl   [ $notebook add -label "Results" ] 
    CreateDisplayControl   [ $notebook add -label "Display" ]
    
    pack $notebook -side top -fill both -expand t -padx 5
    $notebook view "Input"

    set bottom [ frame $basewidget.bottom -relief ridge]
    pack $bottom -side top -fill x -expand false
    
    set pbar [ frame $basewidget.pbar -width 400 -height 20 ]
    pack $pbar -side bottom -expand f -fill x
    CreateProgressBar $pbar
    
    set w [ frame $basewidget.w -relief ridge -width 800 ]
    pack $w -side bottom -fill x -expand false
    
    eval "button $w.close -text Close -command { $this DismissWindow }"

    pack $w.close -side right -fill x -padx 5 -pady 10
    
    set initialized 1
    
    SetTitle $appname

    eval "wm protocol $basewidget WM_DELETE_WINDOW { $this DismissWindow }"

    return $basewidget 
}

#-------------------------------------------------------------------------------------------
#  Create top level menu button
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::CreateMenu { mb } {    
    
    menubutton $mb.results       -text Results     -menu $mb.results.m -underline 0
    pack $mb.results    -side left
    menubutton $mb.help      -text Help         -menu $mb.help.m -underline 0 -padx 4
    pack $mb.help  -side right

    menu $mb.results.m -tearoff 0 
    eval "$mb.results.m add command -label \"Compute All\" -command { $this ComputeResults } -underline 0 -state disabled"
    $mb.results.m add command -label "Save All Results" -command [ itcl::code $this saveallresults ] -underline 0
    $mb.results.m add separator
    if { $parent == 0 } {
	eval "$mb.results.m add command -label Exit -command {  pxtkexit } -underline 1"
    } else {
	eval "$mb.results.m add command -label Close -command {  $this HideWindow } -underline 1"
    }

    menu $mb.help.m -tearoff 0

    set widget_list(menu_results) $mb.results.m    

    eval "$mb.help.m add command -label About -command { $this AboutCommand }"
   
}

#-------------------------------------------------------------------------------------------
#  Add controls to menu button
# ------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::AddToMenuButton { mb args } {
    eval "$mb add command -label \"Fiber Architecture\" -command {$this ShowWindow \"Input\"}"
}
#-------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::SetParametersFromBisAlgorithmAndCompute { cscheme satfactor brightness usemask } {

    $this SetBisAlgorithmMode
    set lst [ list AbsoluteValue RotSymmetry NoSymmetry MirrorSymmetry ]
    set cf [ lsearch -exact $lst $cscheme ]
    if { $cf > -1 } {
	$widget_list(view_direction_scheme) select $cf
    }
    set global_list(view_direction_satscale) $satfactor
    set global_list(view_direction_scaling)  $brightness
    set global_list(view_direction_mask) $usemask
    set global_list(view_mode_index) 5
    $this ComputeResults
    set bis_algorithmmode 0
}

#-------------------------------------------------------------------------------------------
itcl::body mpjcardiacanalysis::GetOutputImage { index } {
    return [ [ lindex $global_list(result_list) $index ] GetThisPointer]
}

#-------------------------------------------------------------------------------------------
#  Main function for stand-alone execution
# ------------------------------------------------------------------------------------------
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    puts "\n[ file rootname $argv0 ] is not a stand-alone program.\n"
    exit
}



