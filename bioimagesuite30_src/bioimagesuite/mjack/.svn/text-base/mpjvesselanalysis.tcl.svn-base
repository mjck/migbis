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

package provide mpjvesselanalysis 1.3

package require  Itcl 3.2
package require  Iwidgets 4.0

package require  pxitcllistselector 1.0
package require  pxitclbaseimagecontrol 1.0
package require  pxitclbaseimageviewer 1.0

package require  vtkmpjcontrib 1.1
package require  mpjguilookuptable 1.0

itcl::class mpjvesselanalysis {

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
    public method LoadTensor { fname }
    public method LoadEigenvalues { fname } 
    public method LoadEigenvectors { fname }
    public method ComputeResults { }
    public method SaveAllResults { }
    public method SaveResult { index }
    public method ClearAllResults { }
    public method DisplayResult { index }
    public method ClearDisplay { }
    public method SetResultPreffix { preffix }
    public method GetResultPreffix { } 
    public method SetCurrentDirectory { fname } 
    public method SaveStatistics { fname }
    
    #-----------------------------------
    # interface creation methods
    #-----------------------------------
    private method CreateDialogs { base }
    private method CreateStatisticsControl { base }
    private method CreateTransformControl { base }
    private method CreateResultsControl { base }
    private method CreateEigenControl { base }
    private method CreateTensorControl { base }
    private method CreateVesselControl { base }
    private method CreateDisplayControl { base }
    private method CreateEigenDisplayControl { base }
    private method CreateTensorDisplayControl { base }
    private method CreateResultDisplayControl { base }
    private method CreateDirectionDisplayControl { base }
    private method CreateDisplayTypeMenu { menubase }
    private method CreateDisplayModeMenu { menubase }
    private method CreateDisplayTransformMenu { menubase }
    private method CreateDisplayColorbarMenu { menubase }
    private method CreateMenu { mb }
    private method Clone { }
    public method AddToMenuButton { mb args }    
 
    private method loadmask { }
    private method loadtensor { }
    private method loadeigenval { }
    private method loadeigenvec { }
    private method displayresult { }
    private method saveresult { }
    private method toggleresult { }
    private method saveallresults { }
    private method savestatistics { }
 
    private method mask_autochanged_cb { }
    private method eigen_autochanged_cb { }

    private method initialize_glyphs { }
    private method input_updateinfo { index }    

    private method vessel_intensity_cb { }

    private method tensor_ordering_cb { }
    private method view_mode_cb { } 
    private method view_eigen_filter_cb { }
    private method view_eigen_glyph_cb { }
    private method view_eigen_maxscale_cb { }
    private method view_eigen_fromto_cb { }
    private method view_eigen_cmapcopy_cb { }
    private method view_eigen_cmapcopybtn_cb { }
    private method view_eigen_cmapresetbtn_cb { }
    private method view_eigen_cmaploadbtn_cb { }
    private method view_eigen_cmapsavebtn_cb { }
    private method view_tensor_filter_cb { }
    private method view_tensor_glyph_cb { }
    private method view_tensor_fromto_cb { }
    private method view_tensor_maxscale_cb { }
    private method view_tensor_cmapcopy_cb { }
    private method view_tensor_cmapcopybtn_cb { }
    private method view_tensor_cmapresetbtn_cb { }
    private method view_tensor_cmaploadbtn_cb { }
    private method view_tensor_cmapsavebtn_cb { }
    private method view_direction_filter_cb { }
    private method view_direction_scheme_cb { }
    private method view_direction_update_cb { }
    private method view_colorbar_cb { } 
    private method view_update_display_cb { }
    private method view_results_colormap_cb { }
    private method view_clear_display { }
    private method view_update_display { }
    private method view_type_toggle_slow_eigenvector { }
    private method view_type_toggle_med_eigenvector { }
    private method view_type_toggle_fast_eigenvector { }
    private method view_type_toggle_tensor { }
    private method view_type_toggle_colorbar { }
    private method view_extract_roi { exts image } 
    private method view_compute_roi_extents { orient dim levels }
    private method view_create_eigenvector_glyph { index vectors scalar glyph where }
    private method view_create_eigenvector { index title }    
    private method view_create_tensor_glyph { tensor scalar glyph where }
    private method view_create_tensor { }
    private method view_create_colorbar { }

    private method result_clear { start end }
    private method result_createmask { }
    private method result_createeigen { }
    private method result_createmaps { }
    private method result_rotatetensor { }
    private method result_add_stat { title fmt tag results }
    private method result_statistics { image mask title } 
}  

#-------------------------------------------------------------------------------------------
#  Close window 
# -------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::DismissWindow { } {
    
    set TENSOR 0
    set MASK 1

    set ok "yes"
    
    set tensorsz [ [ lindex $global_list(result_list) $TENSOR ] GetImageSize ]
    set masksz [ [ lindex $global_list(result_list) $MASK ] GetImageSize ]

    if { [expr ($tensorsz > 1) || ($masksz > 1) ] } {
	
	set ok [ tk_messageBox -type yesnocancel -default no \
		     -parent $basewidget -title "Think again ..." \
		     -message "Unload all images from memory?" -icon question  ]
    }
    
    if { $ok == "yes" } {	
	
	$this view_clear_display
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
itcl::body mpjvesselanalysis::InitializeControl { } {

    set appname "Vessel Analysis v1.0"
    set aboutstring "(c) Marcel Jackowski 2006"

    set global_list(appname) "Vessel Analysis"

    # create image titles
    set global_list(result_title) \
	[ list \
	      { "Hessian matrix"          "hessian" 1 } \
	      { "Region of interest mask" "mask" 1 } \
	      { "Eigenvalues"             "eigenval" 1 } \
	      { "Eigenvectors"            "eigenvec" 1 } \
	      { "Trace"                   "trace" 1 } \
	      { "Vessel likelihood"       "tubular" 1 } \
	      { "Plate likelihood"        "plate" 1 } \
	      { "Blob likelihood"         "blob" 1 } ]

    ## create empty images
    foreach item $global_list(result_title) {
 	lappend result_list [ pxitclimage \#auto ]
	set global_list(enable_list,[lindex $item 1]) 0
    }
    
    set global_list(result_fpreffix) "preffix_"
    set global_list(result_fdir) [ pwd ] 
    set global_list(result_list) $result_list
    
    set global_list(mask_fname) ""
    set global_list(mask_auto) 1
    set global_list(mask_nreg) 1
    set global_list(mask_negative) 0
    
    set global_list(tensor_fname) ""
    set global_list(tensor_symm) 1
    set global_list(tensor_ordering) 0
    set global_list(tensor_flipx) 0
    set global_list(tensor_flipy) 0
    set global_list(tensor_flipz) 0
    set global_list(tensor_rotation) 0
    set global_list(tensor_rotx) 0.0
    set global_list(tensor_roty) 0.0
    set global_list(tensor_rotz) 0.0
    
    set global_list(vessel_alpha) 0.5
    set global_list(vessel_beta) 0.5
    set global_list(vessel_gamma) 0.25
    set global_list(vessel_intensity) 1
    set global_list(vessel_maxintensity) 100.0

    set global_list(eigenval_fname) ""
    set global_list(eigenvec_fname) ""
    set global_list(eigen_auto) 1
    set global_list(eigen_positive) 0
    set global_list(eigen_sortbymagnitude) 0
    set global_list(eigen_recompute) 1

    set global_list(stat_overwrite) 1

    set global_list(view_mode) "3-slice"
    set global_list(view_mode_index) 3
    set global_list(view_type_fast) 0
    set global_list(view_type_medium) 0
    set global_list(view_type_slow) 0
    set global_list(view_type_tensor) 0
    set global_list(view_type_colorbar) 0
    set global_list(view_colorbar) "Eigenvectors"
    set global_list(view_colorbar_label) "Results"
    set global_list(view_eigen_to) 1.0
    set global_list(view_eigen_from) 0.5
    set global_list(view_eigen_between) 1
    set global_list(view_eigen_maxscale) 1.0
    set global_list(view_eigen_flip) 1
    set global_list(view_eigen_cmapcopy) 0
    set global_list(view_tensor_to) 1.0
    set global_list(view_tensor_from) 0.5
    set global_list(view_tensor_between) 1
    set global_list(view_tensor_maxscale) 1.0
    set global_list(view_tensor_scale) 1.0
    set global_list(view_tensor_clampscale) 1
    set global_list(view_tensor_cmapcopy) 0
    set global_list(view_direction_satscale) 0.5
    set global_list(view_direction_scaling) 1.0
    set global_list(view_direction_numcolors) 256
    set global_list(view_direction_mask) 1
    set global_list(view_transparent) 1
    set global_list(view_transpcolor) 0.0
    
    ## create actors for visualization
    set global_list(actor_fast) [ list -1 -1 -1 -1 ]
    set global_list(actor_med) [ list -1 -1 -1 -1 ]
    set global_list(actor_slow) [ list -1 -1 -1 -1 ]
    set global_list(actor_tensor) [ list -1 -1 -1 -1 ]
    set global_list(actor_colorbar) -1

    for { set i 0 } { $i < 3 } { incr i } { ## fast med slow
	for { set j 0 } { $j < 4 } { incr j } { ## xy xz yz vol
	    set global_list(vtk_eigen_glypher,$i,$j) [ vtkGlyph3D [ pxvtable::vnewobj ] ]
	    set global_list(vtk_eigen_threshold,$i,$j) [ vtkThresholdPoints [ pxvtable::vnewobj ] ]
	    set global_list(vtk_eigen_merge,$i,$j) [ vtkMergeFilter [ pxvtable::vnewobj ] ]
	}
    }
    
    for { set i 0 } { $i < 4 } { incr i } {
	set global_list(vtk_tensor_glypher,$i) [ vtkTensorGlyph [ pxvtable::vnewobj ] ]
	set global_list(vtk_tensor_threshold,$i) [ vtkThresholdPoints [ pxvtable::vnewobj ] ]
	set global_list(vtk_tensor_merge,$i) [ vtkMergeFilter [ pxvtable::vnewobj ] ]
    }
    
    initialize_glyphs
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create visualization glyphs
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::initialize_glyphs { } {

    ## create line source
    set line_src [ vtkLineSource [ pxvtable::vnewobj ] ]
    $line_src SetPoint1 0.0 0 0
    $line_src SetPoint2 2.0 0 0    

    set line_xfrm [ vtkTransform [ pxvtable::vnewobj ] ]
    $line_xfrm Translate -1.0 0.0 0.0

    set line [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $line SetInput [ $line_src GetOutput ]
    $line SetTransform $line_xfrm
    lappend global_list(glyph_list) $line 

    $line_src Delete
    $line_xfrm Delete
    
    # create tube source
    set tube_src [ vtkCylinderSource [ pxvtable::vnewobj ] ]
    $tube_src SetHeight 2.0
    $tube_src SetRadius 0.35
    $tube_src CappingOn
    
    set tube_xfrm [ vtkTransform [ pxvtable::vnewobj ] ]
    $tube_xfrm RotateZ -90.0
    
    set tube [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $tube SetInput [ $tube_src GetOutput ]
    $tube SetTransform $tube_xfrm
    lappend global_list(glyph_list) $tube
    
    $tube_src Delete
    $tube_xfrm Delete
    
    ## create arrow source
    set arrow_src [ vtkArrowSource [ pxvtable::vnewobj ] ]
    $arrow_src SetTipLength 0.35
    $arrow_src SetTipResolution 10
    $arrow_src SetTipRadius 0.20
    $arrow_src SetShaftResolution 10
    $arrow_src SetShaftRadius 0.1

    set arrow_xfrm [ vtkTransform [ pxvtable::vnewobj ] ]
    $arrow_xfrm Translate -1.0 0.0 0.0
    $arrow_xfrm Scale 2.0 2.0 2.0

    set arrow [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $arrow SetInput [ $arrow_src GetOutput ]
    $arrow SetTransform $arrow_xfrm
    lappend global_list(glyph_list) $arrow

    $arrow_src Delete
    $arrow_xfrm Delete
    
    ## create ellipsoid source
    set ellipsoid_src [ vtkSphereSource [ pxvtable::vnewobj ] ]
    set ellipsoid_xfrm [ vtkTransform [ pxvtable::vnewobj ] ]
    #$ellipsoid_xfrm Scale -2.0 -2.0 2.0
    
    set ellipsoid [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $ellipsoid SetInput [ $ellipsoid_src GetOutput ]
    $ellipsoid SetTransform $ellipsoid_xfrm
    lappend global_list(glyph_list) $ellipsoid
    
    $ellipsoid_src Delete
    $ellipsoid_xfrm Delete
    
    ## create cuboid source
    set cuboid_src [ vtkCubeSource [ pxvtable::vnewobj ] ]
    set cuboid_xfrm [ vtkTransform [ pxvtable::vnewobj ] ]
    
    set cuboid [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $cuboid SetInput [ $cuboid_src GetOutput ]
    $cuboid SetTransform $cuboid_xfrm
    lappend global_list(glyph_list) $cuboid
    
    $cuboid_src Delete
    $cuboid_xfrm Delete

    ## create sheet source
    set sheet_src [ vtkCubeSource [ pxvtable::vnewobj ] ]
    set sheet_xfrm [ vtkTransform [ pxvtable::vnewobj ] ]
    $sheet_xfrm Scale 1.0 1.0 0.0
    
    set sheet [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $sheet SetInput [ $sheet_src GetOutput ]
    $sheet SetTransform $sheet_xfrm
    lappend global_list(glyph_list) $sheet
    
    $sheet_src Delete
    $sheet_xfrm Delete  
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in mask auto status
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::mask_autochanged_cb { } {
    set state(0) disabled; set state(1) normal        
    $widget_list(mask_load) buttonconfigure load -state \
	$state([expr 1 - $global_list(mask_auto)])
    $widget_list(mask_load) buttonconfigure disp -state \
	$state([expr 1 - $global_list(mask_auto)])
    $widget_list(mask_fname) configure -state \
	$state([expr 1 - $global_list(mask_auto)])
    $widget_list(mask_nreg) configure -state $state([expr 1 - $global_list(mask_auto)])
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in mask auto status
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::eigen_autochanged_cb { } {
    set state(0) disabled; set state(1) normal        
    $widget_list(eigenval_load) buttonconfigure load -state \
	$state([expr 1 - $global_list(eigen_auto)])
    $widget_list(eigenvec_load) buttonconfigure load -state \
	$state([expr 1 - $global_list(eigen_auto)])    

    if { $global_list(eigen_auto) == 0 } {
	set global_list(mask_negative) 0
    }

    if { $global_list(eigen_auto) == 1 } {
	set global_list(eigen_totalinput) 0
    }
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
itcl::body mpjvesselanalysis::input_updateinfo { index } {

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
itcl::body mpjvesselanalysis::loadmask { } {

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
itcl::body mpjvesselanalysis::LoadMask { fname } {

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
   
    ## determine range of values in mask
    set range [ [ [ [ $mask GetImage ] GetPointData ] GetScalars ] GetRange ]

    ## take the maximum range as the number of regions in mask
    set global_list(mask_nreg) [ lindex $range 1 ]    
    $widget_list(mask_nreg) configure -range [ list 1 $global_list(mask_nreg) ]
    
    set global_list(mask_fname) $fname

    WatchOff

    $ana Delete
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in tensor component ordering
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::tensor_ordering_cb { } {
    
    ## current ordering
    set global_list(tensor_ordering) [ $widget_list(tensor_ordering) index [ $widget_list(tensor_ordering) get ] ]   
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load tensor
# -----------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::loadtensor { } {

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
itcl::body mpjvesselanalysis::LoadTensor { fname } {

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
    set global_list(result_fpreffix) "[file root [file tail $fname] ]_"
    set global_list(tensor_symm) [ expr $nc == 6 ]
    
    WatchOff
    
    $ana Delete
    
    $widget_list(computebtn) configure -state normal
    $widget_list(menu_results) entryconfigure 0 -state normal
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load eigenvalues
# -----------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::loadeigenval { } {

    ## browse for filename
    set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
    set fname  [tk_getOpenFile -title "Select eigenvalues" -filetypes $typelist -initialdir $global_list(result_fdir) -parent $basewidget ] 
    
    if { $fname != "" } {
	
	set global_list(result_fdir) [ file dirname $fname ]
	
	## load mask into memory
	LoadEigenvalues $fname
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Load in eigenvalue image 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::LoadEigenvalues { fname } {

    set EIGENVALUES 2

    set eigenval [ lindex $global_list(result_list) $EIGENVALUES ]

    ## load series into memory
    set ana [ vtkpxAnalyzeImageSource  [ pxvtable::vnewobj ]]
    
    $this SetFilterCallbacks $ana "Reading in eigenvalue image..."
    
    WatchOn
    
    ## load series 
    $ana Load $fname 
    
    $eigenval SetFromAnalyzeSource $ana
    
    # determine coordinate transform
    #    set or [ $ana GetOrientation ]   
    #    set nc [ [ $tensor GetImage ] GetNumberOfScalarComponents ]
    
    set global_list(eigenval_fname) $fname
    set global_list(result_fpreffix) "[file root [file tail $fname] ]_"

    #    set global_list(tensor_symm) [ expr $nc == 6 ]
    
    WatchOff
    
    $ana Delete
    
    $widget_list(eigenval_fname) configure -state normal

    #    $widget_list(computebtn) configure -state normal
    #    $widget_list(menu_results) entryconfigure 0 -state normal
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load eigenvectors
# -----------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::loadeigenvec { } {

    ## browse for filename
    set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
    set fname  [tk_getOpenFile -title "Select eigenvectors" -filetypes $typelist -initialdir $global_list(result_fdir) -parent $basewidget ] 
    
    if { $fname != "" } {
	
	set global_list(result_fdir) [ file dirname $fname ]
	
	## load mask into memory
	LoadEigenvectors $fname
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Load in eigenvalue image 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::LoadEigenvectors { fname } {

    set EIGENVECTORS 3

    set eigenvec [ lindex $global_list(result_list) $EIGENVECTORS ]
    
    ## load series into memory
    set ana [ vtkpxAnalyzeImageSource  [ pxvtable::vnewobj ]]
    
    $this SetFilterCallbacks $ana "Reading in eigenvector image..."
    
    WatchOn
    
    ## load series 
    $ana Load $fname 
    
    $eigenvec SetFromAnalyzeSource $ana
    
    # determine coordinate transform
    #    set or [ $ana GetOrientation ]   
    #    set nc [ [ $tensor GetImage ] GetNumberOfScalarComponents ]
    
    set global_list(eigenvec_fname) $fname
    set global_list(result_fpreffix) "[file root [file tail $fname] ]_"
    
    WatchOff
    
    $ana Delete
    
    $widget_list(eigenvec_fname) configure -state normal

    #    $widget_list(computebtn) configure -state normal
    #    $widget_list(menu_results) entryconfigure 0 -state normal
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Set result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::SetResultPreffix { preffix } {
    set global_list(result_fpreffix) $preffix
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Get result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::GetResultPreffix { } {
    return $global_list(result_fpreffix)
}

itcl::body mpjvesselanalysis::SetCurrentDirectory { fname } {
    set global_list(result_fdir) $fname
}


#-------------------------------------------------------------------------------------------
#  PUBLIC: Clear all results
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::ClearAllResults { } {
    
    foreach result $global_list(result_list) {
	$result Clear
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::SaveAllResults { } {

    foreach result $global_list(result_list) title $global_list(result_title) {
	
	set fname $global_list(result_fpreffix)
	set fname "$fname[ lindex $title 1 ]"
	
	$result Save $fname
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Save single result
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::SaveResult { index } {        

    set fname $global_list(result_fpreffix)
    set fname "$fname[ lindex [lindex $global_list(result_title) $index ] 1 ]"

    [lindex $global_list(result_list) $index] Save $fname
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::saveallresults { } {

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
itcl::body mpjvesselanalysis::saveresult { } {
    
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
itcl::body mpjvesselanalysis::DisplayResult { index } {
    $parent SetImageFromObject \
	[ [ lindex $global_list(result_list) $index ] GetThisPointer ] $this
    $parent ShowWindow
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Display result
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::displayresult { } {

    set sel_item [ $widget_list(result_list) getcurselection ]
	    
    if { $sel_item != "" } {
	set index [ $widget_list(result_list) index [ lindex $sel_item 0 ] ]
	set result [ lindex $global_list(result_list) $index ]

	set range [ [ [ [ $result GetImage ] GetPointData ] GetScalars ] GetRange ]
	$widget_list(view_resultluk) SetTableRange [ lindex $range 0 ] [ lindex $range 1 ]
	#	$widget_list(view_resultluk) Update

	$this DisplayResult $index
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: toggle result computation on/off
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::toggleresult { } {
   
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
#  PUBLIC: Save statistics
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::SaveStatistics { fname } {
      $widget_list(stat_text) export $fname
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save statistics
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::savestatistics { } {

    set statfname $global_list(result_fpreffix)
    set initfname "${statfname}stat"
    set initfname [ file join $global_list(result_fdir) $initfname ]

    ## browse for filename
    set fname  [tk_getSaveFile -title "Save statistics"\
		    -filetypes {{"Text" {.txt*}}} \
		    -parent $basewidget \
		    -defaultextension ".txt" \
		    -initialfile $initfname ]  
    
    if { [ string length $fname ] < 1 } {
	return 0
    }
    
    $this SaveStatistics $fname
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: add statistics line
#-------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::result_add_stat { title fmt tag results } {   
    
    set line " $title |"
    
    foreach item $results {
	set fitem [ format $fmt $item ] 
	set line "$line $fitem |"
    }

    $widget_list(stat_text) insert end " $line\n" $tag
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Compute statistics
#-------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::result_statistics { image mask title } {
    
    ## compute statitics from given image and mask
    set stat [ vtkmpjImageStatistics [ pxvtable::vnewobj ] ]
    $stat SetMask $mask
    $stat SetInput $image
    $stat SetNumberOfRegions $global_list(mask_nreg)
    $stat Update
    
    ## write header information
    set border [ string repeat "----------+" $global_list(mask_nreg) ] 
    
    set regions ""
    for { set i 1 } { $i <= $global_list(mask_nreg) } { incr i } {	
	set regions "$regions|Region \#[format %2d $i]"	
    }

    $widget_list(stat_text) insert \
	end "-----------------------+$border\n" even
    $widget_list(stat_text) insert \
	end " $title $regions|\n" odd
    $widget_list(stat_text) insert \
	end "-----------------------+$border\n" even

    ## Write out stats
    set textlist [  $widget_list(stat_text) component text ]
    $textlist tag configure even -background "\#d0ffd0"

    set results ""
    for { set i 0 } { $i < $global_list(mask_nreg) } { incr i } {	
	lappend results [ $stat GetRegionMin $i ]
    }    
    result_add_stat " MINIMUM            " "%8.4f" odd $results
    
    set results ""
    for { set i 0 } { $i < $global_list(mask_nreg) } { incr i } {	
	lappend results [ $stat GetRegionMax $i ]
    }    
    result_add_stat " MAXIMUM            " "%8.4f" even $results
    
    set results ""
    for { set i 0 } { $i < $global_list(mask_nreg) } { incr i } {	
	lappend results [ $stat GetRegionMean $i ]
    }    
    result_add_stat " MEAN               " "%8.4f" odd $results
    
    set results ""
    for { set i 0 } { $i < $global_list(mask_nreg) } { incr i } {	
	lappend results [ $stat GetRegionVariance $i ]
    }    
    result_add_stat " VARIANCE           " "%8.4f" even $results
    
    $widget_list(stat_text) insert end "\n" odd 

    set results ""
    for { set i 0 } { $i < $global_list(mask_nreg) } { incr i } {	
	lappend results [ $stat GetRegionVoxelCount $i ]
    }    

    $stat Delete

    return $results
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Clear results
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::result_clear { start end } {
    
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
#  PRIVATE: Rotate tensor
#-------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::result_rotatetensor { } {

    set TENSOR 0
    
    ## get tensor image
    set tensor [ lindex $global_list(result_list) $TENSOR ]
    
    ## rotate tensor
    set tensor_xform [ vtkmpjImageTensorTransform [ pxvtable::vnewobj ] ]
    $tensor_xform SetInput [ $tensor GetImage ]
    $tensor_xform SetRotation $global_list(tensor_rotx) $global_list(tensor_roty) $global_list(tensor_rotz) 
    $this SetFilterCallbacks $tensor_xform "Transforming tensor..."
    
    # rotate tensor
    $tensor_xform Update
    
    # replace tensor
    $tensor ShallowCopyImage [ $tensor_xform GetOutput ]
    
    $tensor_xform Delete
    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create mask image
#-------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::result_createmask { }  {

    set TENSOR 0
    set MASK 1
    set EIGENVALUES 2
    
    ## get tensor image    
    set tensor [ lindex $global_list(result_list) $TENSOR ]
    
    set mask [ lindex $global_list(result_list) $MASK ]    
    $mask CopyImageHeader \
	[ [ lindex $global_list(result_list) $TENSOR ] GetImageHeader ]

    ## create sigma
    set maskimg [ vtkImageData [ pxvtable::vnewobj ] ]
    $maskimg CopyStructure [ $tensor GetImage ]   
    $maskimg SetScalarTypeToUnsignedChar
    $maskimg SetNumberOfScalarComponents 1
    $maskimg AllocateScalars
    [ [ $maskimg GetPointData ] GetScalars ] \
	FillComponent 0 1
    
    ## save mask
    $mask ShallowCopyImage $maskimg
    
    $maskimg Delete    
    
    update idletasks
    
    return 1
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create eigensystem
#-------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::result_createeigen { } {

    set TENSOR 0
    set MASK 1
    set EIGENVALUES 2
    set EIGENVECTORS 3

    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]
    
    $eigenvalues CopyImageHeader [ $tensor GetImageHeader ]
    $eigenvectors CopyImageHeader [ $tensor GetImageHeader ]

    if { [expr ([ $mask GetImageSize ] == 1) || ([ $tensor GetImageSize ] == 1) ] } {
	return 0
    }

    ## compute eigenvalues and eigenvectors
    set eigen [ vtkmpjImageTensorEigenAnalysis [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $eigen "Computing eigenvalues and eigenvectors..."
    $eigen SetInput [ $tensor GetImage ]
    $eigen SetOrdering $global_list(tensor_ordering)
    $eigen SetMask [ $mask GetImage ]
    $eigen SetOutputTypeToBoth    
    $eigen MaskNegativeOn
    $eigen SetMaskNegativeInValue 0
    $eigen SetMaskNegativeOutValue 255
    $eigen SetOutputAbsoluteEigenvalues 1
#$global_list(eigen_positive)
    $eigen SetSortByMagnitude 0
#$global_list(eigen_sortbymagnitude)
    $eigen FixZeroEigenvaluesOn
    $eigen Update
    
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
    
    #$rad Delete
    $eigenvectors ShallowCopyImage [ $fex GetOutput ]

    $fex Delete

    $eigen Delete

    update idletasks

    return 1
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create maps
#-------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::result_createmaps { } {
    
    set TENSOR 0
    set MASK 1
    set EIGENVALUES 2
    set EIGENVECTORS 3
    set TRACE 4
    set TUBE 5
    set PLATE 6
    set BLOB 7

    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]

    set tracemap [ lindex $global_list(result_list) $TRACE ]
    $tracemap CopyImageHeader [ $tensor GetImageHeader ]
    
    set tubemap [ lindex $global_list(result_list) $TUBE ]
    $tubemap CopyImageHeader [ $tensor GetImageHeader ]
   
    set platemap [ lindex $global_list(result_list) $PLATE ]
    $platemap CopyImageHeader [ $tensor GetImageHeader ]

    set blobmap [ lindex $global_list(result_list) $BLOB ]
    $blobmap CopyImageHeader [ $tensor GetImageHeader ]

    set mapmask [ pxitclimage \#auto ]
    $mapmask CopyImageHeader [ $tensor GetImageHeader ]
    
    $mapmask ShallowCopyImage [ $mask GetImage ]
    
    ## compute invariants
    set invar [ vtkmpjImageTensorInvariants [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $invar "Computing hessian maps..."
    $invar SetInput [ $eigenvalues GetImage ]
    $invar SetMask [ $mapmask GetImage ]
    $invar Update

    ## clear textbox
    if { $global_list(stat_overwrite) } { 
	$widget_list(stat_text) delete 1.0 end
    }
    
    set results 0

    set enable [ lindex [ lindex $global_list(result_title) $TRACE ] 2 ]
    if { $enable } {
	################### extract Trace 
	set trace [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	$this ShowProgressVal "Extracting maps..." [expr 2.0 / 6]
	$trace SetInput [ $invar GetOutput ]
	$trace SetFrameNumber 3
	$trace Update
	
	$tracemap ShallowCopyImage [ $trace GetOutput ]
	$trace Delete
	
	set results [ result_statistics [ $tracemap GetImage ] [ $mapmask GetImage ] "TRACE" ]
    }
    
    set enable1 [ lindex [ lindex $global_list(result_title) $TUBE ] 2 ]
    set enable2 [ lindex [ lindex $global_list(result_title) $PLATE ] 2 ]
    set enable3 [ lindex [ lindex $global_list(result_title) $BLOB ] 2 ]
    
    if { [expr $enable1 || $enable2 || $enable3 ] } {
	
	##################### compute vessel indices #######################
	set vessel [ vtkmpjImageVesselEnhancement [ pxvtable::vnewobj ] ]
	$this ShowProgressVal "Extracting maps..." [expr 3.0 / 6]
	$vessel SetInput [ $tensor GetImage ]
	$vessel SetMask [ $mapmask GetImage ]
	$vessel SetAlpha $global_list(vessel_alpha)
	$vessel SetBeta $global_list(vessel_beta)
	$vessel SetGamma \
	    [ expr $global_list(vessel_gamma) * $global_list(vessel_maxintensity) ]
	$vessel SetStructureTypeToAll
	$vessel SetBrightStructure [ expr 1 - $global_list(vessel_intensity) ]
	$vessel Update

	if { $enable1 } {
	    set tube [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	    $this ShowProgressVal "Extracting maps..." [expr 4.0 / 6]
	    $tube SetInput [ $vessel GetOutput ]
	    $tube SetFrameNumber 0
	    $tube Update  

	    $tubemap ShallowCopyImage [ $tube GetOutput ]
	    $tube Delete
	    
	    set results [ result_statistics [ $tubemap GetImage ] [ $mapmask GetImage ] "TUBE LIKELIHOOD   "]
	}

	if { $enable2 } {
	    set plate [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	    $this ShowProgressVal "Extracting maps..." [expr 5.0 / 6]
	    $plate SetInput [ $vessel GetOutput ]
	    $plate SetFrameNumber 1
	    $plate Update  
	    
	    $platemap ShallowCopyImage [ $plate GetOutput ]
	    $plate Delete
	    
	    set results [ result_statistics [ $platemap GetImage ] [ $mapmask GetImage ] "PLATE LIKELIHOOD   "]
	}

	if { $enable3 } {
	    set blob [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	    $this ShowProgressVal "Extracting maps..." [expr 6.0 / 6]
	    $blob SetInput [ $vessel GetOutput ]
	    $blob SetFrameNumber 2
	    $blob Update  
	    
	    $blobmap ShallowCopyImage [ $blob GetOutput ]
	    $blob Delete
	    
	    set results [ result_statistics [ $blobmap GetImage ] [ $mapmask GetImage ] "BLOB LIKELIHOOD   "]
	}

	$vessel Delete
	
    }
    
    ##################### Voxel count ###############################
    set border [ string repeat "==========+" $global_list(mask_nreg) ] 
    $widget_list(stat_text) insert end "=======================+$border\n" even
    
    result_add_stat " TOTAL VOXELS       " "%8d" odd $results  

    $invar Delete

    $mapmask Clear
    itcl::delete object $mapmask

    $this ShowProgressVal "Done." 0
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Compute results
# -----------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::ComputeResults { } {
    
    set MASK 1
    set TRACE 4
    set TUBE 5

    WatchOn
    
    ## clear display
    view_clear_display
    
    ## clear results
    if { $global_list(eigen_recompute) } {
	if { $global_list(eigen_auto) == 1 } { 
	    result_clear $MASK [ expr [ llength $global_list(result_list) ] -1 ]
	} else {
	    result_clear $TRACE [ expr [ llength $global_list(result_list) ] -1 ]
	}
    } else {
	result_clear $TRACE [ expr [ llength $global_list(result_list) ] -1 ]
    }

    $widget_list(notebook) view "Statistics"; update

    ## create mask if not loaded in
    if { $global_list(mask_auto) } {
	
	set result [ result_createmask ]

	if { $result == 0 } {
	    WatchOff; return
	}
    }

    if { $global_list(eigen_auto) == 1 } {

	if { $global_list(eigen_recompute) } {

	    ## compute eigevalues and eigenvectors
	    set result [ result_createeigen ]
	    
	    if { $result == 0 } {
		WatchOff; return
	    }
	}
    }

    ## create maps
    result_createmaps
    
    # send currently selected result to parent
    $widget_list(result_list) selection set $TUBE $TUBE
    
    set tubemap [ lindex $global_list(result_list) $TUBE ]
    $parent SetImageFromObject [ $tubemap  GetThisPointer ] $this

    set range [ [ [ [ $tubemap GetImage ] GetPointData ] GetScalars ] GetRange ]
    $widget_list(view_resultluk) SetTableRange [ lindex $range 0 ] [ lindex $range 1 ]
    $widget_list(view_resultluk) Update
    
    ## update display
    view_update_display

    WatchOff
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Clear display
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_clear_display { } {
    
    ## remove eigenvectors and tensors
    foreach item "slow med fast tensor" {
	foreach actor $global_list(actor_${item}) {	    
	    if { $actor != "-1" } {
		[ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor	    
		$actor Delete	    
	    }
	}
	set global_list(actor_${item}) [list -1 -1 -1 -1 ]	
    }
    
    ## remove colorbars
    set actor $global_list(actor_colorbar)
    if { $actor != "-1" } {
	[ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor	    
	$actor Delete	   
	set global_list(actor_colorbar) -1
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Update display
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_update_display { } {

    if { $global_list(view_type_fast) } {	
	view_create_eigenvector fast 0
    }
    
    if { $global_list(view_type_medium) } {	
	view_create_eigenvector med 1
    }
    
    if { $global_list(view_type_slow) } { 
	view_create_eigenvector slow 2
    }    
    
    if { $global_list(view_type_tensor) } {
	view_create_tensor 
    }
    
    if { $global_list(view_type_colorbar) } {
	view_create_colorbar
    }

    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Compute ROI extents
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_compute_roi_extents { orient dim levels } {
   
    set x0(0) 0; set x0(1) 0; set x0(2) 0;
    set x1(0) [expr [lindex $dim 0] -1]
    set x1(1) [expr [lindex $dim 1] -1]
    set x1(2) [expr [lindex $dim 2] -1]
    
    if { $orient < 3 } {

	## set up slice geometry
	for { set i 0 } { $i < 3 } { incr i } {
	    if { $i == $orient } {
		set x0($i) [lindex $levels $i]
		set x1($i) [lindex $levels $i]
	    }
	}
    }
    
    return "$x0(0) $x0(1) $x0(2) $x1(0) $x1(1) $x1(2)"
}


#-------------------------------------------------------------------------------------------
#  PRIVATE: Extract ROI given image and computed extents
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_extract_roi { exts image } {
    
    set dim [ [ $image GetImage ] GetDimensions ]
    set spac [ [ $image GetImage ] GetSpacing ]
    set orig [ [ $image GetImage ] GetOrigin ]
    set ospac "1 1 1"
    
    set x0(0) [lindex $exts 0]; set x0(1) [lindex $exts 1]; set x0(2) [lindex $exts 2];
    set x1(0) [lindex $exts 3]; set x1(1) [lindex $exts 4]; set x1(2) [lindex $exts 5];
    
    ## determine correct origin and spacing
    for { set i 0 } { $i <= 2 } { incr i } {	
 	if { $x1($i) < $x0($i) } { set x1($i) [ expr $x0($i) +1 ] }
 	set dx [ lindex $ospac $i ]
 	if { $dx < 1 } { set dx 1 } elseif { $dx > 10 } { set dx 10 }	
 	set origin($i)  [ expr [lindex $orig $i ] + $x0($i) * [ lindex $spac $i ]]
 	set spacing($i) [ expr [lindex $spac $i ] * $dx ]
 	set ext($i)     [ expr ($x1($i)-$x0($i))/$dx ]
    }

    set identity [ vtkIdentityTransform [ pxvtable::vnewobj ] ]

    ## extract roi
    set roi [ vtkbisImageReslice [ pxvtable::vnewobj ] ]
    $roi SetInput [ $image GetImage ]
    $roi SetResliceTransform $identity
    $roi SetOutputSpacing $spacing(0) $spacing(1) $spacing(2)
    $roi SetOutputOrigin $origin(0)  $origin(1)  $origin(2)    
    $roi SetOutputExtent 0 $ext(0) 0 $ext(1) 0 $ext(2)
    $roi SetInterpolationMode 0 
    $roi Update

    $identity Delete

    return $roi
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create tensor glyphs
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_create_tensor_glyph { tensor scalar glyph where } {

    ## convert image into tensor attribute
    set imtoten [ vtkmpjImageToTensors [ pxvtable::vnewobj ] ]
    $imtoten SetInput $tensor
    $imtoten SetSymmetricTensor $global_list(tensor_symm)
#    $imtoten SetFlipX $global_list(tensor_flipx)
#    $imtoten SetFlipY $global_list(tensor_flipy)
#    $imtoten SetFlipZ $global_list(tensor_flipz)
    $imtoten Update
    
    ## combine geometry information
    set merge $global_list(vtk_tensor_merge,$where)
    $merge SetGeometry $tensor
    $merge SetScalars $scalar
    $merge SetTensors [ $imtoten GetOutput ]
    $merge Update
    
    $imtoten Delete
    
    ## threshold resulting dataset 
    set thresh $global_list(vtk_tensor_threshold,$where)
    $thresh SetInput [ $merge GetOutput ]
    if { $global_list(view_tensor_between) == 1 } {
	$thresh ThresholdBetween $global_list(view_tensor_from) $global_list(view_tensor_to)
    } else {
	$thresh ThresholdByUpper $global_list(view_tensor_from)
    }
    $thresh Update

    ## create glyph field
    set glypher $global_list(vtk_tensor_glypher,$where)
    $glypher SetInput [ $thresh GetOutput ]
    $glypher SetSource [ $glyph GetOutput ]
    $glypher SymmetricOn
    #$glypher ExtractEigenvaluesOff
    $glypher SetScaleFactor $global_list(view_tensor_scale)
    $glypher SetClampScaling $global_list(view_tensor_clampscale)
    $glypher SetMaxScaleFactor $global_list(view_tensor_maxscale)
    $glypher Update

    ## compute surface normals
    set glyphNormals [ vtkPolyDataNormals [ pxvtable::vnewobj ] ]
    $glyphNormals SetInput [ $glypher GetOutput ]

    ## set up glyph mapper
    set glyphMapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
    $glyphMapper SetInput [ $glyphNormals GetOutput ]
    $glyphMapper SetLookupTable [ $widget_list(view_tensorluk) GetLookupTable ]
    $glyphMapper UseLookupTableScalarRangeOn
    $glyphMapper SetColorModeToMapScalars
    $glyphMapper ScalarVisibilityOn

    $glyphNormals Delete

    ## create glyph actor
    set glyphActor [ vtkActor [ pxvtable::vnewobj ] ]
    $glyphActor SetMapper $glyphMapper
    
    $glyphMapper Delete
    
    return $glyphActor
}

#-------------------------------------------------------------------------------------------
#  Create eigenvector glyphs
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_create_eigenvector_glyph { index vectors scalar glyph where } {

    ## extract eigenvector subset 
    set subset [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
    $subset SetInput $vectors
    $subset SetStartFrame [expr $index * 3]
    $subset SetEndFrame [expr $index * 3 + 2]
    $subset Update
    
    ## convert image into vector attribute
    set imtovec [ vtkmpjImageToVectors [ pxvtable::vnewobj ] ]
    $imtovec SetInput [ $subset GetOutput ]
    if { $global_list(view_eigen_flip) } {
	$imtovec SetFlipX $global_list(tensor_flipx)
	$imtovec SetFlipY $global_list(tensor_flipy)
	$imtovec SetFlipZ $global_list(tensor_flipz)
    }
    $imtovec Update

    $subset Delete
    
    ## combine geometry information
    set merge $global_list(vtk_eigen_merge,$index,$where)
    $merge SetGeometry $vectors
    $merge SetScalars $scalar
    $merge SetVectors [ $imtovec GetOutput ]
    $merge Update
    
    $imtovec Delete
    
    ## threshold resulting dataset 
    set thresh $global_list(vtk_eigen_threshold,$index,$where)
    $thresh SetInput [ $merge GetOutput ]
    if { $global_list(view_eigen_between) == 1 } {
	$thresh ThresholdBetween $global_list(view_eigen_from) $global_list(view_eigen_to)
    } else {
	$thresh ThresholdByUpper $global_list(view_eigen_from)
    }
    $thresh Update
    
    ## create glypher field   
    set glypher $global_list(vtk_eigen_glypher,$index,$where)
    $glypher SetInput [ $thresh GetOutput ]
    $glypher SetSource [ $glyph GetOutput ]
    $glypher ScalingOn
    $glypher ClampingOn
    $glypher SetScaleFactor $global_list(view_eigen_maxscale)
    $glypher SetScaleModeToDataScalingOff
    $glypher SetColorModeToColorByScalar
    $glypher SetVectorModeToUseVector
    $glypher Update

    ## set up glyph mapper
    set glyphMapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
    $glyphMapper SetInput [ $glypher GetOutput ]
    $glyphMapper SetLookupTable [ $widget_list(view_vectorluk) GetLookupTable ]
    $glyphMapper UseLookupTableScalarRangeOn
    $glyphMapper SetColorModeToMapScalars
    $glyphMapper ScalarVisibilityOn

    ## create glyph actor
    set glyphActor [ vtkActor [ pxvtable::vnewobj ] ]
    $glyphActor SetMapper $glyphMapper
    
    $glyphMapper Delete
    
    return $glyphActor
}

#-------------------------------------------------------------------------------------------
#  Toggle On/Off fast eigenvector visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_type_toggle_fast_eigenvector { } {                
    
    if { $global_list(view_type_fast) } {	
	view_create_eigenvector fast 0
    } else {
	foreach actor $global_list(actor_fast) {	    
	    if { $actor != "-1" } {
		[ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor	    
		$actor Delete	    
	    }
	}
	set global_list(actor_fast) [ list -1 -1 -1 -1 ]
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  Toggle On/Off medium-speed eigenvector visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_type_toggle_med_eigenvector { } {                

    if { $global_list(view_type_medium) } {	
	view_create_eigenvector med 1
    } else {
	foreach actor $global_list(actor_med) {	    
	    if { $actor != "-1" } {
		[ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor	    
		$actor Delete	    
	    }
	}
	set global_list(actor_med) [ list -1 -1 -1 -1 ]
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  Toggle On/Off medium-speed eigenvector visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_type_toggle_slow_eigenvector { } {                

    if { $global_list(view_type_slow) } {	
	view_create_eigenvector slow 2
    } else { 
	foreach actor $global_list(actor_slow) {	    
	    if { $actor != "-1" } {
		[ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor	    
		$actor Delete	    
	    }
	}
	set global_list(actor_slow) [list -1 -1 -1 -1 ]
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  Toggle On/Off tensor visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_type_toggle_tensor { } {                

    if { $global_list(view_type_tensor) } {	
	view_create_tensor
    } else { 
	foreach actor $global_list(actor_tensor) {
	    if { $actor != "-1" } {
		[ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor
		$actor Delete	    
	    }
	}
	set global_list(actor_tensor) [ list -1 -1 -1 -1 ]
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  Toggle On/Off colorbar visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_type_toggle_colorbar { } {
    
    if { $global_list(view_type_colorbar) } {
	view_create_colorbar
    } else {
	set actor $global_list(actor_colorbar)
	[ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor
	$actor Delete

	set global_list(actor_colorbar) -1
    }

    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  View tensors
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_create_tensor { } {
    
    set TENSOR 0
    set EIGENVECTORS 3

    ## get tensor information
    set tensor_image [ lindex $global_list(result_list) $TENSOR ]
    
    ## get current slice numbers
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set dims [ [ $tensor_image GetImage ] GetDimensions ]
    
    ## get current eigenvector scalar filter
    set filter [ $widget_list(view_tensor_filter) index [ $widget_list(view_tensor_filter) get ] ]
    set tensor_filter [ lindex $global_list(result_list) $filter ] 

    ## get current glyph
    set cur_glyph [ $widget_list(view_tensor_glyph) index [ $widget_list(view_tensor_glyph) get ] ]
    set glyph_source [ lindex $global_list(glyph_list) [ expr $cur_glyph + 3 ] ]
    #    set glyph_source [ lindex $global_list(glyph_list) 3 ]
    
    if { $global_list(view_mode_index) < 4 } {
	    
	    ## plot on slices
	    for { set i 0 } { $i < 3 } { incr i } {

		if { [expr ($global_list(view_mode_index) == $i) || \
			  ($global_list(view_mode_index) == 3) ] } {
		    
		    set ext [ view_compute_roi_extents $i $dims $levels ]
		    set tensors [ [ view_extract_roi $ext $tensor_image ] GetOutput ]
		    set scalars [ [ view_extract_roi $ext $tensor_filter ] GetOutput ]
		    
		    set actor [ view_create_tensor_glyph $tensors $scalars $glyph_source $i ]
		    set global_list(actor_tensor) [ lreplace $global_list(actor_tensor) $i $i $actor ]
		    
		    $tensors Delete
		    $scalars Delete
		    
		    [ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] AddActor $actor
		    
		}
		
	    }
    
    } else {
	
	## plot on entire volume
	set tensors [ $tensor_image GetImage ]
	set scalars [ $tensor_filter GetImage ]

	## create glyph
	set actor [ view_create_tensor_glyph $tensors $scalars $glyph_source 3 ]
	
	set global_list(actor_tensor) [ lreplace $global_list(actor_tensor) 3 3 $actor ]
	
	[ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] AddActor $actor
	
    }
}

#-------------------------------------------------------------------------------------------
#  View eigenvectors
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_create_eigenvector { title index } {
    
    ## which: (0) view_type_fast, (1) view_type_med, (2) view_type_slow, (3) view_type_tensor
    set TENSOR 0
    set MASK 1
    set EIGENVALUES 2
    set EIGENVECTORS 3
    
    set actornm "actor_$title"
        
    ## get eigenvalues and eigenvectors
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]

    ## get current slice numbers
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set dims [ [ $eigenvalues GetImage ] GetDimensions ]
    
    ## get current eigenvector scalar filter
    set filter [ $widget_list(view_eigen_filter) index [ $widget_list(view_eigen_filter) get ] ]
    set eigenfilter [ lindex $global_list(result_list) $filter ] 
    
    ## get current glyph
    set cur_glyph [ $widget_list(view_eigen_glyph) index [ $widget_list(view_eigen_glyph) get ] ]
    set glyph_source [ lindex $global_list(glyph_list) $cur_glyph ]
    
    if { $global_list(view_mode_index) < 4 } {
	
	## plot on slices
	for { set i 0 } { $i < 3 } { incr i } {
	    
	    if { [ expr ($global_list(view_mode_index) == $i) || \
		       ($global_list(view_mode_index) == 3) ] } {
		
		set ext [ view_compute_roi_extents $i $dims $levels ]
		set vectors [ [ view_extract_roi $ext $eigenvectors ] GetOutput ]
		set scalars [ [ view_extract_roi $ext $eigenfilter ] GetOutput ]
		
		set actor [ view_create_eigenvector_glyph $index $vectors $scalars $glyph_source $i ]
		set global_list($actornm) [ lreplace $global_list($actornm) $i $i $actor ]
		
		$vectors Delete
		$scalars Delete
		
		[ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] AddActor $actor
	    }
	    
	}
    
    } else {

	## plot on entire volume
	set vectors [ $eigenvectors GetImage ]
	set scalars [ $eigenfilter GetImage ]

	## create glyph
	set actor [ view_create_eigenvector_glyph $index $vectors $scalars $glyph_source 3 ]
	
	set global_list($actornm) [ lreplace $global_list($actornm) 3 3 $actor ]
	
	[ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] AddActor $actor
	
    }       
}

#-------------------------------------------------------------------------------------------
#  View colorbar
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_create_colorbar { } {
    
    set cbar(Eigenvectors) 0
    set cbar(Tensor) 1
    set cbar(Results) 2

    set title "colorbar"

    set colorbar $cbar($global_list(view_colorbar))
        
    set filter 0
    if { $colorbar == 0 } {
	set filter [ $widget_list(view_eigen_filter) index [ $widget_list(view_eigen_filter) get ] ]
    } elseif { $colorbar == 1 } {
	set filter [ $widget_list(view_tensor_filter) index [ $widget_list(view_tensor_filter) get ] ]
    } 

    if { $colorbar < 2 } {
	set title [ lindex [ lindex $global_list(result_title) $filter ] 0 ] 
    } else {
	set title $global_list(view_colorbar_label)
    }
    
    set bar [ vtkScalarBarActor [ pxvtable::vnewobj ] ]
    
    if { $colorbar == 0 } {
	$bar SetLookupTable [ $widget_list(view_vectorluk) GetLookupTable ]
    } elseif { $colorbar == 1 } { 
	$bar SetLookupTable [ $widget_list(view_tensorluk) GetLookupTable ]
    } elseif { $colorbar == 2 } {
	$bar SetLookupTable [ $widget_list(view_resultluk) GetLookupTable ]
    }
    
    $bar SetTitle $title
    [$bar GetPositionCoordinate] SetCoordinateSystemToNormalizedViewport
    [$bar GetPositionCoordinate] SetValue 0.1 0.01
    $bar SetOrientationToHorizontal
    $bar SetWidth 0.8
    $bar SetHeight 0.12
    
    set global_list(actor_colorbar) $bar
    
    [ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] AddActor2D $bar    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in display type
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_mode_cb { } {
        
    set mode(XZ-plane) 0
    set mode(YZ-plane) 1
    set mode(XY-plane) 2
    set mode(3-slice) 3
    set mode(Volume) 4
    
    set global_list(view_mode_index) $mode($global_list(view_mode))

    view_clear_display

    view_update_display
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in tensor colormap selection
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_tensor_cmapcopy_cb { } {
    
    ## current tensor filter
    set cpfrom [ $widget_list(view_tensor_cmapcopy) index [ $widget_list(view_tensor_cmapcopy) get ] ]

    set global_list(view_tensor_cmapcopy) $cpfrom
    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon copy eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_tensor_cmapcopybtn_cb { } {
    
    if { $global_list(view_tensor_cmapcopy) == 0 } {
	$widget_list(view_tensorluk) Copy [ $widget_list(view_vectorluk) GetLookupTable ]
    } else {
	$widget_list(view_tensorluk) Copy [ $widget_list(view_resultluk) GetLookupTable ]
    }
        
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon copy eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_tensor_cmapresetbtn_cb { } {
    
    ## current tensor filter
    $widget_list(view_tensorluk) SetHueRange 0.5 0.0
    $widget_list(view_tensorluk) Update

}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon loading eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_tensor_cmaploadbtn_cb { } {   

    $widget_list(view_tensorluk) Load
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon saving eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_tensor_cmapsavebtn_cb { } {   

    $widget_list(view_tensorluk) SaveAs
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in tensor filter selection
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_tensor_filter_cb { } {
    
    set TENSOR 0
    
    ## current eigenvector filter
    set filter [ $widget_list(view_tensor_filter) index [ $widget_list(view_tensor_filter) get ] ]   
    set tensor_filter [ lindex $global_list(result_list) $filter ] 

    ## get eigenvalues and eigenvectors
    set tensor_image [ lindex $global_list(result_list) $TENSOR ]
    
    ## get current slice numbers
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set dims [ [ $tensor_image GetImage ] GetDimensions ]
    
    if { $global_list(view_mode_index) < 4 } {	
	
	## plot on slices
	for { set i 0 } { $i < 3 } { incr i } {
	    
	    if { [expr ($global_list(view_mode_index) == $i ) || \
		      ($global_list(view_mode_index) == 3) ] } {		
		
		set ext [ view_compute_roi_extents $i $dims $levels ]
		set scalar [ [ view_extract_roi $ext $tensor_filter ] GetOutput ]
		
		set merge $global_list(vtk_tensor_merge,$i)
		$merge SetScalars $scalar
		
		$scalar Delete	    	    
	    }
	}	
	
    } else {
	
	## plot on entire volume
	set scalar [ [ lindex $global_list(result_list) $filter ] GetImage ]
	
	set merge $global_list(vtk_tensor_merge,3)
	$merge SetScalars $scalar
    }
    
    set result [ [ lindex $global_list(result_list) $filter ] GetImage ]
    set range [ [ [ $result GetPointData ] GetScalars ] GetRange ]
    $widget_list(view_tensorluk) SetTableRange [ lindex $range 0 ] [ lindex $range 1 ]
#    $widget_list(view_tensorluk) Update
    
    ## update scalar bar    
    if { $global_list(view_colorbar) == "Hessian" } {
	set title [ lindex [ lindex $global_list(result_title) $filter ] 0 ] 
	if { $global_list(actor_colorbar) != -1 } { 
	    $global_list(actor_colorbar) SetTitle $title
	}
    }

    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in eigen colormap selection
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_eigen_cmapcopy_cb { } {
    
    ## current eigenvector filter
    set cpfrom [ $widget_list(view_eigen_cmapcopy) index [ $widget_list(view_eigen_cmapcopy) get ] ]

    set global_list(view_eigen_cmapcopy) $cpfrom
    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon copy eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_eigen_cmapcopybtn_cb { } {
    
    if { $global_list(view_eigen_cmapcopy) == 0 } {
	$widget_list(view_vectorluk) Copy [ $widget_list(view_tensorluk) GetLookupTable ]
    } else {
	$widget_list(view_vectorluk) Copy [ $widget_list(view_resultluk) GetLookupTable ]
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon resetting eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_eigen_cmapresetbtn_cb { } {
    
    ## current tensor filter
    $widget_list(view_vectorluk) SetHueRange 0.75 0.0
    $widget_list(view_vectorluk) Update

}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon loading eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_eigen_cmaploadbtn_cb { } {

    $widget_list(view_vectorluk) Load
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon saving eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_eigen_cmapsavebtn_cb { } {

    $widget_list(view_vectorluk) SaveAs
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in vector filter selection
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_eigen_filter_cb { } {    
      
    set EIGENVALUES 2
    set EIGENVECTORS 3

    ## current eigenvector filter
    set filter [ $widget_list(view_eigen_filter) index [ $widget_list(view_eigen_filter) get ] ]   
    
    ## get eigenvalues and eigenvectors
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]

    ## get current slice numbers
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set dims [ [ $eigenvalues GetImage ] GetDimensions ]

    if { $global_list(view_mode_index) < 4 } {
	
	## plot on slices
	for { set i 0 } { $i < 3 } { incr i } {
	    
	    if { [expr ($global_list(view_mode_index) == $i ) || \
		      ($global_list(view_mode_index) == 3) ] } {
		
		set ext [ view_compute_roi_extents $i $dims $levels ]
		set scalar [ view_extract_roi $ext [ lindex $global_list(result_list) $filter ] ]
	    
		## loop through eigenvectors
		for { set j 0 } { $j < 3 } { incr j } {
		    set merge $global_list(vtk_eigen_merge,$j,$i)
		    $merge SetScalars [ $scalar GetOutput ]
		}
		
		$scalar Delete
	    }
	}
    } else {
	
	## plot on entire volume
	#	set ext [ view_compute_roi_extents 3 $dims $levels ]
	set scalar [ [ lindex $global_list(result_list) $filter ] GetImage ]
	
	## loop through eigenvectors
	for { set j 0 } { $j < 3 } { incr j } {
	    set merge $global_list(vtk_eigen_merge,$j,3)
	    $merge SetScalars $scalar
	}		
    }

    set result [ [ lindex $global_list(result_list) $filter ] GetImage ]
    set range [ [ [ $result GetPointData ] GetScalars ] GetRange ]
    $widget_list(view_vectorluk) SetTableRange [ lindex $range 0 ] [ lindex $range 1 ]
#    $widget_list(view_vectorluk) Modified
    #$widget_list(view_vectorluk) Update

    ## update scalar bar    
    if { $global_list(view_colorbar) == "Eigenvectors" } {
	set title [ lindex [ lindex $global_list(result_title) $filter ] 0 ] 
	if { $global_list(actor_colorbar) != -1 } { 
	    $global_list(actor_colorbar) SetTitle $title
	}
    }

    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in threshold values
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_tensor_fromto_cb { } {
    for { set j 0 } { $j < 4 } { incr j } { 
	if { $global_list(view_tensor_between) == 1 } {
	    $global_list(vtk_tensor_threshold,$j) ThresholdBetween \
		$global_list(view_tensor_from) $global_list(view_tensor_to)
	} else {
	    $global_list(vtk_tensor_threshold,$j) ThresholdByUpper \
		$global_list(view_tensor_from)
	}
    }
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in threshold values
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_eigen_fromto_cb { } {
    for { set i 0 } { $i < 3 } { incr i } {	
	for { set j 0 } { $j < 4 } { incr j } { 
	    if { $global_list(view_eigen_between) == 1 } {
		$global_list(vtk_eigen_threshold,$i,$j) ThresholdBetween \
		    $global_list(view_eigen_from) $global_list(view_eigen_to)
	    } else {
		$global_list(vtk_eigen_threshold,$i,$j) ThresholdByUpper \
		    $global_list(view_eigen_from)
	    }
	}
    }
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in glyph type
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_eigen_glyph_cb { } {    
    set index [ $widget_list(view_eigen_glyph) index [ $widget_list(view_eigen_glyph) get ] ]
    set glyph [ lindex $global_list(glyph_list) $index ]
    for { set i 0 } { $i < 3 } { incr i } {
	for { set j 0 } { $j < 4 } { incr j } { 
	    $global_list(vtk_eigen_glypher,$i,$j) SetSource [ $glyph GetOutput ]
	}
    }
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in glyph type
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_tensor_glyph_cb { } {    
    set index [ $widget_list(view_tensor_glyph) index [ $widget_list(view_tensor_glyph) get ] ]
    set glyph [ lindex $global_list(glyph_list) [expr $index + 3 ] ]
    for { set j 0 } { $j < 4 } { incr j } { 
	$global_list(vtk_tensor_glypher,$j) SetSource [ $glyph GetOutput ]
    }
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in threshold values
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_tensor_maxscale_cb { } {
    for { set j 0 } { $j < 4 } { incr j } {
	$global_list(vtk_tensor_glypher,$j) \
	    SetScaleFactor $global_list(view_tensor_scale)
	$global_list(vtk_tensor_glypher,$j) \
	    SetMaxScaleFactor $global_list(view_tensor_maxscale)
	$global_list(vtk_tensor_glypher,$j) \
	    SetClampScaling $global_list(view_tensor_clampscale)
    }
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in threshold values
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_eigen_maxscale_cb { } {
    for { set i 0 } { $i < 3 } { incr i } {
	for { set j 0 } { $j < 4 } { incr j } {
	    $global_list(vtk_eigen_glypher,$i,$j) \
		SetScaleFactor $global_list(view_eigen_maxscale)
	}
    }
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in colorbar selection
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_colorbar_cb { } {

    ## recreate colorbar
    set actor $global_list(actor_colorbar)
    if { $actor != -1 } {
	[ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor
	$actor Delete
    }

    if { $global_list(view_type_colorbar) } {
	view_create_colorbar
    }

    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon an update button press
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_update_display_cb { } { 
    
    view_clear_display

    view_update_display
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in colormaps for result images
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_results_colormap_cb { } {

    set luk [ $widget_list(view_resultluk) GetLookupTable ]
    
    if { $global_list(view_transparent) } {
#	set rgba [ $luk GetTableValue 0 ]
	set index [ $luk GetIndex $global_list(view_transpcolor) ]
#	puts "color: $global_list(view_transpcolor) $index"
	set rgba [ $luk GetTableValue $index ]
	$luk SetTableValue $index [ lindex $rgba 0 ] [ lindex $rgba 1 ] [ lindex $rgba 2 ] 0.0
    } else {
	$widget_list(view_resultluk) Update
	set luk [ $widget_list(view_resultluk) GetLookupTable ]
    }
        
    [ $parent GetViewer ] SetLookupTable $luk
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon selection of tensor direction coloring
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::view_direction_update_cb { } {
    
    set TENSOR 0
    set MASK 1
    set EIGENVECTORS 3
    set DEC_MAP 12
    
    set filter [ $widget_list(view_direction_filter) index \
		     [ $widget_list(view_direction_filter) get ] ]   
    
    set scheme [ $widget_list(view_direction_scheme) index \
		     [ $widget_list(view_direction_scheme) get ] ]   
    
    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]
    
    set magmap [ lindex $global_list(result_list) $filter ]
    
    set imct [ vtkmpjImageTensorColor [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $imct "Computing colormap..."
    $imct SetInput [ $eigenvectors GetImage ]
    $imct SetColorSchemeType $scheme
    $imct SetSaturationScale $global_list(view_direction_satscale)
    $imct SetScaling $global_list(view_direction_scaling)
    $imct SetMagnitude [ $magmap GetImage ]
    
    if { $global_list(view_direction_mask) == 1 } {
	$imct SetMask [ $mask GetImage ]
    }
    
    $imct Update
    
    set quant [ vtkImageQuantizeRGBToIndex [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $quant "Quantizing colormap..."
    $quant SetInput [ $imct GetOutput ]
    $quant SetNumberOfColors $global_list(view_direction_numcolors)
    $quant Update
       
    set imcast [ vtkImageCast [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $imcast "Converting colormap to floating point..."
    $imcast SetInput [ $quant GetOutput ]
    $imcast SetOutputScalarTypeToFloat
    $imcast Update

    set imctmap [ lindex $global_list(result_list) $DEC_MAP ]
    $imctmap ShallowCopyImage [ $imcast GetOutput ]    
    set range [ [ [ [ $imcast GetOutput ] GetPointData ] GetScalars ] GetRange ]
    $imctmap CopyImageHeader [ $tensor GetImageHeader ]

    $parent SetImageFromObject \
	[ $imctmap GetThisPointer ] $this

    set temp_luk [ vtkLookupTable [ pxvtable::vnewobj ] ]
    $temp_luk DeepCopy [ $quant GetLookupTable ]
    
    $quant Delete
    
    $widget_list(view_resultluk) SetTableRange [ lindex $range 0 ] [ lindex $range 1 ]
    $widget_list(view_resultluk) SetLookupTable $temp_luk

    #    [ $parent GetViewer ] SetLookupTable [ $quant GetLookupTable ]
    [ $parent GetViewer ] SetLookupTable [ $widget_list(view_resultluk) GetLookupTable ]
    [ $parent GetViewer ] UpdateDisplay
        
    $imct Delete  
    $imcast Delete
}

#-------------------------------------------------------------------------------------------
#  Create tensor control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateTensorControl { base } {
    
    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Hessian image"
    pack $base.frame0 -fill both -expand f -pady 5
    
    set frame0 [ $base.frame0 childsite ]
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 0
  
    set tensor_fname [ iwidgets::entryfield $subframe1.mask_fname \
			   -textvariable [ itcl::scope global_list(tensor_fname) ] \
			   -labeltext "Filename:" ] 
    pack $tensor_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(tensor_fname) $tensor_fname
    
    set tensor_load [ iwidgets::buttonbox $subframe1.tensor_load -orient horizontal -padx 2 ]
    $tensor_load add load -text "Load..." -command [ itcl::code $this loadtensor ] 
    $tensor_load add disp -text "Display" -command [ itcl::code $this DisplayResult 0 ]
    pack $tensor_load -side left
    set widget_list(tensor_load) $tensor_load  

    set tensor_ordering \
	[ iwidgets::optionmenu $subframe1.tensor_ordering -labeltext "Components:" ]
    $tensor_ordering configure -command [ itcl::code $this tensor_ordering_cb ]
    $tensor_ordering insert 0 "xx-xy-xz-yy-yz-zz"
    $tensor_ordering insert 1 "xx-yy-zz-xy-yz-xz"
    pack $tensor_ordering -side right -padx 5
    set widget_list(tensor_ordering) $tensor_ordering

    iwidgets::labeledframe $base.frame1 \
	-labelpos nw  \
	-labeltext "Region of interest mask"
    pack $base.frame1 -fill both -expand f
    
    set frame1 [ $base.frame1 childsite ]
    
    set subframe0 [ frame $frame1.subframe0 ]
    pack $subframe0 -side top -fill both -expand f -pady 5
    
    set mask_auto [ checkbutton $subframe0.mask_auto \
			-text "Use entire image for computation" \
			-variable [ itcl::scope global_list(mask_auto) ] \
			-command [ itcl::code $this mask_autochanged_cb ] ]
    pack $mask_auto -side left -fill both -padx 5 -pady 2
    
    set subframe1 [ frame $frame1.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 0
    
    set mask_fname [ iwidgets::entryfield $subframe1.mask_fname \
			 -textvariable [ itcl::scope global_list(mask_fname) ] \
			 -state disabled \
			 -labeltext "Filename:" ] 
    pack $mask_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(mask_fname) $mask_fname

    set mask_load [ iwidgets::buttonbox $subframe1.mask_load -orient horizontal -padx 2 ]
    $mask_load add load -text "Load..." -command [ itcl::code $this loadmask ]    
    $mask_load add disp -text "Display" -command [ itcl::code $this DisplayResult 1 ]
    pack $mask_load -side left 
    $mask_load buttonconfigure load -state disabled
    $mask_load buttonconfigure disp -state disabled
    set widget_list(mask_load) $mask_load
    
    set mask_nreg [ iwidgets::spinint $subframe1.mask_nreg \
			-labeltext "Number of regions in mask:" \
			-fixed 4 -width 4 -range {1 1000} -step 1 \
			-textvariable [ itcl::scope global_list(mask_nreg) ] \
			-state disabled ]
    pack $mask_nreg -side right -padx 5 -pady 5
    set widget_list(mask_nreg) $mask_nreg

    set frame2 [ frame $base.frame2 ]
    pack $frame2 -side top -fill both -expand f 

    set widget_list(computebtn) [ button $frame2.compute -text "Compute!" \
				      -state disabled \
				      -command [ itcl::code $this ComputeResults ] ]    
    pack $frame2.compute -side left -fill x -expand f -padx 5 -pady 10  
    
    set eigen_recompute [ checkbutton $frame2.eigen_recompute \
			      -text "Always recompute eigensystem" \
			      -variable [ itcl::scope global_list(eigen_recompute) ] ]
    pack $eigen_recompute -side right -fill both -padx 5 -pady 5

}

#-------------------------------------------------------------------------------------------
#  Create eigenvalues/vectors control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateEigenControl { base } {

    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Eigenvalues"
    pack $base.frame0 -fill both -expand f -pady 5
    
    set frame0 [ $base.frame0 childsite ]
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 0
          
    set eigenval_fname [ iwidgets::entryfield $subframe1.eigenval_fname \
			     -textvariable [ itcl::scope global_list(eigenval_fname) ] \
			     -state disabled \
			     -labeltext "Filename:" ] 
    pack $eigenval_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(eigenval_fname) $eigenval_fname
    
    set eigenval_load [ iwidgets::buttonbox $subframe1.eigenval_load -orient horizontal -padx 2 ]
    $eigenval_load add load -text "Load..." -command [ itcl::code $this loadeigenval ]    
    $eigenval_load add disp -text "Display" -command [ itcl::code $this DisplayResult 2 ]
    pack $eigenval_load -side left 
    $eigenval_load buttonconfigure load -state disabled
    set widget_list(eigenval_load) $eigenval_load

    iwidgets::labeledframe $base.frame1 \
	-labelpos nw  \
	-labeltext "Eigenvectors"
    pack $base.frame1 -fill both -expand f -pady 5
    
    set frame1 [ $base.frame1 childsite ]
    
    set subframe1 [ frame $frame1.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 0
    
    set eigenvec_fname [ iwidgets::entryfield $subframe1.eigenvec_fname \
			     -textvariable [ itcl::scope global_list(eigenvec_fname) ] \
			     -state disabled \
			     -labeltext "Eigenvectors:" ] 
    pack $eigenvec_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(eigenvec_fname) $eigenvec_fname
    
    set eigenvec_load [ iwidgets::buttonbox $subframe1.eigenvec_load -orient horizontal -padx 2 ]
    $eigenvec_load add load -text "Load..." -command [ itcl::code $this loadeigenvec ]    
    $eigenvec_load add disp -text "Display" -command [ itcl::code $this DisplayResult 3 ]
    pack $eigenvec_load -side left 
    $eigenvec_load buttonconfigure load -state disabled
    set widget_list(eigenvec_load) $eigenvec_load    
    
    set subframe2 [ frame $base.subframe2 ]
    pack $subframe2 -side top -fill x -expand f -pady 0

    set eigen_auto [ checkbutton $subframe2.eigen_auto \
			 -text "Compute eigenvectors from tensor" \
			 -variable [ itcl::scope global_list(eigen_auto) ]  \
			 -command [ itcl::code $this eigen_autochanged_cb ] ]
    pack $eigen_auto -side left -fill both -padx 5 -pady 5

    set subframe3 [ frame $base.subframe3 ]
    pack $subframe3 -side top -fill x -expand f -pady 0

    set eigen_positive [ checkbutton $subframe3.eigen_absolute \
			     -text "Absolute eigenvalues" \
			     -variable [ itcl::scope global_list(eigen_positive) ] ] 
    pack $eigen_positive -side left -fill both -padx 5 -pady 5
    
    set subframe4 [ frame $base.subframe4 ]
    pack $subframe4 -side top -fill x -expand f -pady 0

    set eigen_sortbymagnitude [ checkbutton $subframe4.eigen_sortbymagnitude \
				    -text "Sort by magnitude" \
				    -variable [ itcl::scope global_list(eigen_sortbymagnitude) ] ]
    pack $eigen_sortbymagnitude -side left -fill both -padx 5 -pady 5
    
}

#-------------------------------------------------------------------------------------------
#  Create vessel enhancement control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateVesselControl { base } {

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
        
    set vessel_intensity \
	[ iwidgets::optionmenu $subframe2.vessel_intensity \
	      -command [ itcl::code $this vessel_intensity_cb ] \
	      -labeltext "Structure intensity:" ]
    $vessel_intensity insert 1 "Dark"
    $vessel_intensity insert 0 "Bright"
    pack $vessel_intensity -side left -padx 5 -pady 5
    set widget_list(vessel_intensity) $vessel_intensity
    $vessel_intensity select 0
       
}

#-------------------------------------------------------------------------------------------
#  Create transform control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateTransformControl { base } {
    
    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Hessian transformation"
    pack $base.frame0 -fill both -expand t -pady 5
    
    set frame0 [ $base.frame0 childsite ]
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 0

    set tensorflipx [ checkbutton $subframe1.tensorflipx \
			  -text "Flip X" \
			  -command [ itcl::code $this view_update_display_cb ] \
			  -variable [ itcl::scope global_list(tensor_flipx) ] ]
    pack $tensorflipx -side left -padx 5 -pady 7
    set widget_list(tensor_flipx) $tensorflipx
    
    set tensorflipy [ checkbutton $subframe1.tensorflipy \
			  -text "Flip Y" \
			  -command [ itcl::code $this view_update_display_cb ] \
			  -variable [ itcl::scope global_list(tensor_flipy) ] ]
    pack $tensorflipy -side left -padx 5 -pady 7
    set widget_list(tensor_flipy) $tensorflipy   
    
    set tensorflipz [ checkbutton $subframe1.tensorflipz \
			  -text "Flip Z" \
			  -command [ itcl::code $this view_update_display_cb ] \
			  -variable [ itcl::scope global_list(tensor_flipz) ] ]
    pack $tensorflipz -side left -padx 5 -pady 7
    set widget_list(tensor_flipz) $tensorflipz

    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -side top -fill both -expand f -pady 0

    iwidgets::entryfield $subframe2.tensor_rotx \
	-width 5 \
	-validate real \
	-labeltext "Tensor rotation  Rx:" \
	-textvariable [ itcl::scope global_list(tensor_rotx) ]
    pack $subframe2.tensor_rotx -side left -fill x -padx 5 -pady 5
    
    iwidgets::entryfield $subframe2.tensor_roty \
	-width 5 \
	-validate real \
	-labeltext "Ry:" \
	-textvariable [ itcl::scope global_list(tensor_roty) ]
    pack $subframe2.tensor_roty -side left -fill x -pady 5
    
    iwidgets::entryfield $subframe2.tensor_rotz \
	-width 5 \
	-validate real \
	-labeltext "Rz:" \
	-textvariable [ itcl::scope global_list(tensor_rotz) ]
    pack $subframe2.tensor_rotz -side left -fill x -pady 5

    label $subframe2.dummy -text "degrees"
    pack $subframe2.dummy -side left
        
    set tensor_rotation [ button $subframe2.tensor_rotation \
			      -command [ itcl::code $this result_rotatetensor ] \
			      -text "Rotate!" ]
    pack $tensor_rotation -side left -fill both -padx 5 -pady 5
}

#-------------------------------------------------------------------------------------------
#  Create results control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateResultsControl { base } {   

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
    $resultbbox add mark -text "On/Off" -command [ itcl::code $this toggleresult ]
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

#-------------------------------------------------------------------------------------------
#  Create statistics control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateStatisticsControl { base } {

    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Statistics"
    pack $base.frame0 -fill both -expand t 
    
    set frame0 [ $base.frame0 childsite ]

    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -fill both -expand t

    set stat_text [iwidgets::scrolledtext $subframe0.stat_text \
		       -visibleitems 70x19 \
		       -foreground black \
		       -vscrollmode static \
		       -hscrollmode dynamic \
		       -wrap none ]
    pack $stat_text -side top -expand t -fill both -padx 2 -pady 2
    set widget_list(stat_text) $stat_text

    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -fill both -expand t

    set statbbox [ iwidgets::buttonbox $subframe1.statbbox -orient horizontal -pady 2 -padx 2 ]
    $statbbox add save -text "Save..." -command [ itcl::code $this savestatistics ]
    $statbbox add clear -text "Clear" -command "$stat_text delete 1.0 end"
    pack $statbbox -side left -fill both 

    set stat_overwrite [ checkbutton $subframe1.stat_overwrite -text "Overwrite" \
			     -variable [ itcl::scope global_list(stat_overwrite) ] ]
    pack $stat_overwrite -side right -padx 5       
}

#-------------------------------------------------------------------------------------------
#  Create eigenvector display control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateEigenDisplayControl { base } {

    set FA_MAP 5

    set subframe0 [ frame $base.subframe0 ]
    pack $subframe0 -fill x -side top -padx 5 -expand f -pady 5
        
    set view_eigen_filter \
	[ iwidgets::optionmenu $subframe0.view_eigen_filter -labeltext "Filter:" ]
    $view_eigen_filter configure -command [ itcl::code $this view_eigen_filter_cb ]
    for { set i 0 } { $i < [ llength $global_list(result_title) ] } { incr i } {
	$view_eigen_filter insert $i [ lindex [ lindex $global_list(result_title) $i ] 0 ]
    }
    pack $view_eigen_filter -side left -padx 5
    set widget_list(view_eigen_filter) $view_eigen_filter

    $view_eigen_filter select $FA_MAP
    
    iwidgets::entryfield $subframe0.view_eigen_from \
	-width 5 \
	-validate real \
	-labeltext "From:" \
	-textvariable [ itcl::scope global_list(view_eigen_from) ] \
	-command [ itcl::code $this view_eigen_fromto_cb ]
    pack $subframe0.view_eigen_from -side left -fill x -padx 5
    
    checkbutton $subframe0.view_eigen_between \
	-text "to:"  \
	-variable [ itcl::scope global_list(view_eigen_between) ] \
	-command [ itcl::code $this view_eigen_fromto_cb ]
    pack $subframe0.view_eigen_between -side left -fill x -padx 0
    
    iwidgets::entryfield $subframe0.view_eigen_to \
	-width 5 \
	-validate real \
	-textvariable [ itcl::scope global_list(view_eigen_to) ] \
	-command [ itcl::code $this view_eigen_fromto_cb ]
    pack $subframe0.view_eigen_to -side left -fill x -padx 0
    
    set subframe1 [ frame $base.subframe1 ]
    pack $subframe1 -side top -fill both -padx 5 
    
    set view_eigen_glyph [ iwidgets::optionmenu $subframe1.view_eigen_glyph \
			       -command [ itcl::code $this view_eigen_glyph_cb ] \
			       -labeltext "View eigenvector as:" ]
    pack $view_eigen_glyph -side left -padx 5
    $view_eigen_glyph insert end "Lines"
    $view_eigen_glyph insert end "Tubes"
    $view_eigen_glyph insert end "Arrows"
    set widget_list(view_eigen_glyph) $view_eigen_glyph

    iwidgets::entryfield $subframe1.view_eigen_maxscale \
	-width 5 \
	-validate real \
	-labeltext "Glyph scale factor:" \
	-textvariable [ itcl::scope global_list(view_eigen_maxscale) ] \
	-command [ itcl::code $this view_eigen_maxscale_cb ]
    pack $subframe1.view_eigen_maxscale -side left -padx 5 
    
    #    checkbutton $subframe1.view_eigen_flip \
	#	-text "Flip"  \
	#	-variable [ itcl::scope global_list(view_eigen_flip) ] \
	#	-command [ itcl::code $this view_update_display_cb ]
    #    pack $subframe1.view_eigen_flip -side left -fill x -padx 0
    
    set subframe2 [ frame $base.subframe2 -relief flat ]
    pack $subframe2 -side left -padx 5 -pady 5 
    
    set vector_luk [ mpjguilookuptable \#auto 0 ]
    $vector_luk Initialize $subframe2
    pack forget [ $vector_luk GetButtonBoxWidget ]
#    pack forget [ $vector_luk GetMenuBar ]     
    $vector_luk SetHueRange 0.75 0.0
    $vector_luk Update
    $vector_luk Show

    set widget_list(view_vectorluk) $vector_luk
        
    set view_eigen_cmap_copy [ iwidgets::optionmenu [ $vector_luk GetMenuBar ].copy_cmap \
				   -labeltext "from:" \
				   -command [ itcl::code $this view_eigen_cmapcopy_cb ] ]
    pack $view_eigen_cmap_copy  -side right -padx 2 -pady 2
    $view_eigen_cmap_copy insert end "Hessians"
    $view_eigen_cmap_copy insert end "Results"
    
    set widget_list(view_eigen_cmapcopy) $view_eigen_cmap_copy

    set view_eigen_cmap_copybtn [ button [ $vector_luk GetMenuBar ].copybtn -text "Copy" \
				      -command [ itcl::code $this view_eigen_cmapcopybtn_cb ] ]
    pack $view_eigen_cmap_copybtn -side right -padx 0 -pady 5 
}

#-------------------------------------------------------------------------------------------
#  Create tensor display control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateTensorDisplayControl { base } {

    set FA_MAP 5

    set subframe0 [ frame $base.subframe0 ]
    pack $subframe0 -fill both -padx 5 -expand f -pady 5
        
    set view_tensor_filter \
	[ iwidgets::optionmenu $subframe0.view_tensor_filter -labeltext "Filter:" ]
    $view_tensor_filter configure -command [ itcl::code $this view_tensor_filter_cb ]
    for { set i 0 } { $i < [ llength $global_list(result_title) ] } { incr i } {
	$view_tensor_filter insert $i [ lindex [ lindex $global_list(result_title) $i ] 0 ]
    }
    pack $view_tensor_filter -side left -padx 5
    set widget_list(view_tensor_filter) $view_tensor_filter

    $view_tensor_filter select $FA_MAP
    
    iwidgets::entryfield $subframe0.view_tensor_from \
	-width 5 \
	-validate real \
	-labeltext "From:" \
	-textvariable [ itcl::scope global_list(view_tensor_from) ] \
	-command [ itcl::code $this view_tensor_fromto_cb ]
    pack $subframe0.view_tensor_from -side left -fill x -padx 5
    
    checkbutton $subframe0.view_tensor_between \
	-text "to:"  \
	-variable [ itcl::scope global_list(view_tensor_between) ] \
	-command [ itcl::code $this view_tensor_fromto_cb ]
    pack $subframe0.view_tensor_between -side left -fill x -padx 0
    
    iwidgets::entryfield $subframe0.view_tensor_to \
	-width 5 \
	-validate real \
	-textvariable [ itcl::scope global_list(view_tensor_to) ] \
	-command [ itcl::code $this view_tensor_fromto_cb ]
    pack $subframe0.view_tensor_to -side left -fill x -padx 0
    
    set subframe1 [ frame $base.subframe1 ]
    pack $subframe1 -side top -fill both -padx 5 

    set view_tensor_glyph [ iwidgets::optionmenu $subframe1.view_tensor_glyph \
				-command [ itcl::code $this view_tensor_glyph_cb ] \
				-labeltext "View tensor as:" ]
    pack $view_tensor_glyph -side left -padx 5
    $view_tensor_glyph insert end "Ellipsoids"
    $view_tensor_glyph insert end "Cuboids"
    $view_tensor_glyph insert end "Sheets"
    set widget_list(view_tensor_glyph) $view_tensor_glyph

    iwidgets::entryfield $subframe1.view_tensor_scale \
	-width 5 \
	-validate real \
	-labeltext "Scale:" \
	-textvariable [ itcl::scope global_list(view_tensor_scale) ] \
	-command [ itcl::code $this view_tensor_maxscale_cb ]
    pack $subframe1.view_tensor_scale -side left -padx 5 -pady 5
    
    set view_tensor_clampscale [ checkbutton $subframe1.view_tensor_clampscale \
				     -text "Clamp at:" \
				     -variable \
				     [ itcl::scope global_list(view_tensor_clampscale) ] \
				     -command [ itcl::code $this view_tensor_maxscale_cb ] ]
    pack $view_tensor_clampscale -side left -pady 5 -padx 5       
    set widget_list(view_tensor_clampscale) $view_tensor_clampscale

    iwidgets::entryfield $subframe1.view_tensor_maxscale \
	-width 5 \
	-validate real \
	-textvariable [ itcl::scope global_list(view_tensor_maxscale) ] \
	-command [ itcl::code $this view_tensor_maxscale_cb ]
    pack $subframe1.view_tensor_maxscale -side left -padx 2 -pady 5
    
    set subframe2 [ frame $base.subframe2 -relief flat ]
    pack $subframe2 -side left -padx 5 -pady 5

    set tensor_luk [ mpjguilookuptable \#auto 0 ]
    $tensor_luk Initialize $subframe2
    pack forget [ $tensor_luk GetButtonBoxWidget ]
#    pack forget [ $tensor_luk GetMenuBar ] 
    $tensor_luk SetHueRange 0.5 0.0
    $tensor_luk Update
    $tensor_luk Show
    
    set widget_list(view_tensorluk) $tensor_luk
    
    set view_tensor_cmap_copy [ iwidgets::optionmenu [ $tensor_luk GetMenuBar ].copy_cmap \
				    -labeltext "from:" \
				    -command [ itcl::code $this view_tensor_cmapcopy_cb ] ]
    pack $view_tensor_cmap_copy  -side right -padx 2 -pady 5
    $view_tensor_cmap_copy insert end "Eigenvectors"
    $view_tensor_cmap_copy insert end "Results"
    
    set widget_list(view_tensor_cmapcopy) $view_tensor_cmap_copy
    
    set view_tensor_cmap_copybtn [ button [ $tensor_luk GetMenuBar ].copybtn -text "Copy" \
				       -command [ itcl::code $this view_tensor_cmapcopybtn_cb ] ]
    pack $view_tensor_cmap_copybtn -side right -padx 0 -pady 5 
    
}

#-------------------------------------------------------------------------------------------
#  Create result display control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateResultDisplayControl { base } {
    
    set subframe1 [ frame $base.subframe1 ]
    pack $subframe1 -side bottom -padx 5 -pady 5
    
    iwidgets::entryfield $subframe1.view_colorbar_label \
	-width 15 \
	-labeltext "Colormap label:" \
	-textvariable [ itcl::scope global_list(view_colorbar_label) ]
    pack $subframe1.view_colorbar_label -side top -padx 5    

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

#-------------------------------------------------------------------------------------------
#  Create directionality encoding display control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateDirectionDisplayControl { base } {
    
    set FA_MAP 5
    
    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Directionally encoded colormaps"
    pack $base.frame0 -fill both -pady 5 -expand f
    
    set frame0 [ $base.frame0 childsite ]

    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -fill x -expand f -padx 5 -pady 5
  
    set view_direction_filter \
	[ iwidgets::optionmenu $subframe0.view_direction_filter \
	      -labeltext "Modulate intensity by:" ]
    for { set i 0 } { $i < [ llength $global_list(result_title) ] } { incr i } {
	$view_direction_filter insert $i [ lindex [ lindex $global_list(result_title) $i ] 0 ]
    }
    pack $view_direction_filter -side left -padx 5
    set widget_list(view_direction_filter) $view_direction_filter

    $view_direction_filter select $FA_MAP

    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -fill x -expand f -padx 5 -pady 5
  
    set view_direction_scheme \
	[ iwidgets::optionmenu $subframe2.view_direction_scheme -labeltext "Color scheme:" ]
    set widget_list(view_direction_scheme) $view_direction_scheme

    $view_direction_scheme insert 0 "Absolute value"
    $view_direction_scheme insert 1 "Rotational symmetry"
    $view_direction_scheme insert 2 "No symmetry"
    $view_direction_scheme insert 3 "Mirror symmetry"

    pack $view_direction_scheme -side left -padx 5
    
    $view_direction_scheme select 0    

    set subframe3 [ frame $frame0.subframe3 ]
    pack $subframe3 -fill x -expand f -padx 5 -pady 5
    
    iwidgets::entryfield $subframe3.view_direction_numcolors \
	-width 4 \
	-validate integer \
	-labeltext "Number of colors for quantization:" \
	-textvariable [ itcl::scope global_list(view_direction_numcolors) ]
    pack $subframe3.view_direction_numcolors -side left -padx 5 -pady 5

    set subframe4 [ frame $frame0.subframe4 ]
    pack $subframe4 -fill x -expand f -padx 5 -pady 5
    
    iwidgets::entryfield $subframe4.view_direction_satscale \
	-width 5 \
	-validate real \
	-labeltext "Saturation factor:" \
	-textvariable [ itcl::scope global_list(view_direction_satscale) ]
    pack $subframe4.view_direction_satscale -side left -padx 5 -pady 5
    
    iwidgets::entryfield $subframe4.view_direction_scaling \
	-width 5 \
	-validate real \
	-labeltext "Brightness:" \
	-textvariable [ itcl::scope global_list(view_direction_scaling) ]
    pack $subframe4.view_direction_scaling -side left -padx 5 -pady 5

    set view_direction_mask [ checkbutton $subframe4.view_direction_mask \
				  -text "Use anatomical mask" \
				  -variable [ itcl::scope global_list(view_direction_mask) ] ]
    pack $view_direction_mask -side right -pady 5 -padx 5       
    set widget_list(view_direction_mask) $view_direction_mask

    set subframe1 [ frame $base.subframe1 ]
    pack $subframe1 -fill both -expand f -padx 5   

    iwidgets::buttonbox $subframe1.decdisplaybbox -orient vertical 
    $subframe1.decdisplaybbox add display -text "Apply!" -command \
	[ itcl::code $this view_direction_update_cb ]
    pack $subframe1.decdisplaybbox -side left -fill x -expand f
   
}

#-------------------------------------------------------------------------------------------
#  Create display control
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateDisplayControl { base } {
        
#    frame $base.dummy -relief sunken -borderwidth 1 -height 2
#    pack $base.dummy -side top -fill x -expand f -padx 12 -pady 0

    set frame0 [ frame $base.frame0 ]
    pack $base.frame0 -pady 0 -fill both -expand t 
    
    set display_notebook [ iwidgets::tabnotebook $frame0.display_notebook -tabpos n ]     
    set widget_list(display_notebook) $display_notebook
    pack $display_notebook -fill both -expand t -pady 5 -padx 5
    
    CreateEigenDisplayControl [ $display_notebook add -label "Eigenvectors" ]
    CreateTensorDisplayControl [ $display_notebook add -label "Hessians" ]
    CreateDirectionDisplayControl [ $display_notebook add -label "Directionality" ]
    CreateResultDisplayControl [ $display_notebook add -label "Results" ]

    $display_notebook view "Eigenvectors"   
}

# ------------------------------------------------------------------------------------------
#  Create Transform Menu
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateDisplayTransformMenu { menubase } {

    menubutton $menubase.transform -text "Transform" -menu $menubase.transform.m -underline 0
    pack $menubase.transform -side left

    menu $menubase.transform.m -tearoff 0

    $menubase.transform.m add check -label "Flip X" -underline 0 \
	-variable [ itcl::scope global_list(tensor_flipx) ] \
	-command [ itcl::code $this view_update_display_cb ]
    $menubase.transform.m add check -label "Flip Y" -underline 0 \
	-variable [ itcl::scope global_list(tensor_flipy) ] \
	-command [ itcl::code $this view_update_display_cb ]
    $menubase.transform.m add check -label "Flip Z" -underline 0 \
	-variable [ itcl::scope global_list(tensor_flipz) ] \
	-command [ itcl::code $this view_update_display_cb ]

}

# ------------------------------------------------------------------------------------------
#  Create Colorbar Menu
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateDisplayColorbarMenu { menubase } {  

    menubutton $menubase.colorbar -text "Colorbar" -menu $menubase.colorbar.m -underline 0
    pack $menubase.colorbar -side left

    menu $menubase.colorbar.m -tearoff 0

    $menubase.colorbar.m add radio -label "Eigenvectors" -underline 0 \
	-variable [ itcl::scope global_list(view_colorbar) ] \
	-command [ itcl::code $this view_colorbar_cb ]
    $menubase.colorbar.m add radio -label "Hessian" -underline 0 \
	-variable [ itcl::scope global_list(view_colorbar) ] \
	-command [ itcl::code $this view_colorbar_cb ]
    $menubase.colorbar.m add radio -label "Results" -underline 0 \
	-variable [ itcl::scope global_list(view_colorbar) ] \
	-command [ itcl::code $this view_colorbar_cb ]
}

# ------------------------------------------------------------------------------------------
#  Create Visualization Menu
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateDisplayModeMenu { menubase } {  
    
    menubutton $menubase.mode -text "Location" -menu $menubase.mode.m -underline 0
    pack $menubase.mode -side left 

    menu $menubase.mode.m -tearoff 0

    $menubase.mode.m add radio -label "XZ-plane" -underline 0 \
	-variable [ itcl::scope global_list(view_mode) ] \
	-command [ itcl::code $this view_mode_cb ]
    $menubase.mode.m add radio -label "YZ-plane" -underline 0 \
	-variable [ itcl::scope global_list(view_mode) ] \
	-command [ itcl::code $this view_mode_cb ]
    $menubase.mode.m add radio -label "XY-plane" -underline 0 \
	-variable [ itcl::scope global_list(view_mode) ] \
	-command [ itcl::code $this view_mode_cb ]
    $menubase.mode.m add radio -label "3-slice" -underline 0 \
	-variable [ itcl::scope global_list(view_mode) ] \
	-command [ itcl::code $this view_mode_cb ]    
    $menubase.mode.m add separator
    $menubase.mode.m add radio -label "Volume" -underline 0 \
	-variable [ itcl::scope global_list(view_mode) ] \
	-command [ itcl::code $this view_mode_cb ]
}

# ------------------------------------------------------------------------------------------
#  Create Visualization Menu
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::CreateDisplayTypeMenu { menubase } {  
    
    menubutton $menubase.type -text "View" -menu $menubase.type.m -underline 0
    pack $menubase.type -side left

    menu $menubase.type.m -tearoff 0

    $menubase.type.m add check -label "Fast eigenvector" -underline 0 \
	-variable [ itcl::scope global_list(view_type_fast) ] \
	-command [ itcl::code $this view_type_toggle_fast_eigenvector ]
    $menubase.type.m add check -label "Medium eigenvector" -underline 0 \
	-variable [ itcl::scope global_list(view_type_medium) ] \
	-command [ itcl::code $this view_type_toggle_med_eigenvector ]
    $menubase.type.m add check -label "Slow eigenvector" -underline 0 \
	-variable [ itcl::scope global_list(view_type_slow) ] \
	-command [ itcl::code $this view_type_toggle_slow_eigenvector ]
    $menubase.type.m add check -label "Hessian" -underline 0 \
	-variable [ itcl::scope global_list(view_type_tensor) ] \
	-command [ itcl::code $this view_type_toggle_tensor ]    
    $menubase.type.m add separator
    $menubase.type.m add check -label "Colorbar" -underline 0 \
	-variable [ itcl::scope global_list(view_type_colorbar) ] \
	-command [ itcl::code $this view_type_toggle_colorbar ]
}

# -------------------------------------------------------------------------------------------
#  Diffusion control inititialization
# ------------------------------------------------------------------------------------------
itcl::body mpjvesselanalysis::Initialize { widget } {
    
    if { $initialized == 1 } { return $basewidget }
    
    #  -------------------------------------
    #  Create User Interface
    #  -------------------------------------	
    set basewidget [toplevel $widget ]
    wm geometry $basewidget 600x480
    wm withdraw $basewidget

    set notebook $basewidget.notebook    
    iwidgets::tabnotebook $notebook -tabpos w         
    set widget_list(notebook) $notebook

    set mb [ frame $basewidget.mb ]
    pack $mb -side top -fill x -expand false
    
    CreateMenu $mb
    CreateDisplayTypeMenu $mb
    CreateDisplayModeMenu $mb
    CreateDisplayColorbarMenu $mb
    
    button $mb.refresh -text "Refresh" -borderwidth 0 \
	-command [ itcl::code $this view_update_display_cb ] -underline 2
    pack $mb.refresh -side left -padx 0

    CreateTensorControl     [ $notebook add -label "Hessian" ]
    CreateVesselControl     [ $notebook add -label "Vesselness" ]
    CreateResultsControl    [ $notebook add -label "Results" ] 
    CreateStatisticsControl [ $notebook add -label "Statistics" ]
    CreateDisplayControl    [ $notebook add -label "Display" ]
    
    pack $notebook -side top -fill both -expand t -padx 5
    $notebook view "Hessian"

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
itcl::body mpjvesselanalysis::CreateMenu { mb } {    
    
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
itcl::body mpjvesselanalysis::AddToMenuButton { mb args } {
    eval "$mb add command -label \"Vessel analysis\" -command {$this ShowWindow \"Hessian\"}"
}

#-------------------------------------------------------------------------------------------
#  Main function for stand-alone execution
# ------------------------------------------------------------------------------------------
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    puts "\n[ file rootname $argv0 ] is not a stand-alone program.\n"
    exit
}


