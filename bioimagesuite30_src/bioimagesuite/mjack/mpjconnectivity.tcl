# -----------------------
#!/bin/sh
# the next line restarts using wish \
    exec vtk "$0" "$@"

# Dependencies and path
# -----------------------
lappend auto_path [ file dirname [ info script ] ]
lappend auto_path [ file join [file join [ file dirname [ info script ]] ".."] base ]
lappend auto_path [ file join [file join [ file dirname [ info script ]] ".."] main ]

package provide mpjconnectivity 1.0

package require  Itcl 3.2
package require  Iwidgets 4.0

package require  pxitcllistselector 1.0
package require  pxitclbaseimagecontrol 1.0
package require  pxitclbaseimageviewer 1.0

package require  vtkmpjcontrib 1.1
package require  mpjguilookuptable 1.0

itcl::class mpjconnectivity {

    inherit pxitclbaseimagecontrol
    
    #-----------------------------------
    # construction and destruction
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
    # input/output
    #-----------------------------------
    public method LoadMap { fname }
    public method LoadMask { fname }
    public method LoadTensor { fname }
    public method LoadSolution { fname }
    public method SaveAllResults { }
    public method SaveResult { index }
    public method ClearAllResults { }
    public method DisplayResult { index { mode image } }
    public method ClearDisplay { }
    public method SetResultPreffix { preffix }
    public method GetResultPreffix { } 
    public method SetCurrentDirectory { fname } 
    
    #-----------------------------------
    # interface creation methods
    #-----------------------------------
    private method CreateInputControl { base }
    private method CreateConnectivityControl { base }
    private method CreateTrackingControl { base }
    private method CreateDisplayControl { base }
    private method CreateFiberControl { base }
    private method CreateStatisticsControl { base }
    private method CreateResultsControl { base }
    private method CreateFiberDisplayControl { base }
    private method CreateFrontDisplayControl { base }
    private method CreateMenu { mb }
    public method AddToMenuButton { mb args}    
    public method CreateViewMenu { menubase }
 
    private method loadmap { }
    private method loadtensor { }
    private method loadmask { }
    private method loadsolution { }
    private method displayresult { { mode image } }
    private method saveresult { }
    private method toggleresult { }
    private method saveallresults { }
 
    private method initialize_glyphs { }
    private method input_updateinfo { index }    
    private method input_selectioncommand_cb { }
        
    private method track_seedtype_cb { }
    private method track_map_cb { }
    private method track_integration_cb { }
    private method track_regionfilter_cb { }
    private method track_splinetype_cb { }
    private method track_splinesubdivision_cb { }

    private method view_colorbar_cb { } 
    private method view_update_display_cb { }
    private method view_clear_display { }
    private method view_update_display { }
    private method view_toggle_colorbar { }
    private method view_toggle_contour { }
    private method view_toggle_fiber { }
    private method view_toggle_gradient { }
    private method view_toggle_character { }

    private method view_create_fiber_glyph { index }
    private method view_create_fibers { }
    private method view_create_colorbar { }
    private method view_create_contour { }
    private method view_create_contour_glyph { scalars } 
    private method view_create_gradient { }
    private method view_create_gradient_glyph { scalars glyph }
    private method view_create_character { }
    private method view_create_character_glyph { scalars vectors glyph }

    private method view_extract_roi { exts image } 
    private method view_compute_roi_extents { orient dim levels }

    private method solution_method_cb { }

    private method view_fiber_glyph_cb { }
    private method view_fiber_tuberadius_cb { }
    private method view_fiber_pointsize_cb { }
    private method view_fiber_colormap_cb { }
    private method view_fiber_filter_cb { }
    private method view_fiber_luktype_cb { }
      
    private method fiber_add { polydata }
    private method fiber_addwithname { name polydata }
    private method fiber_remove { index } 
    private method fiber_removeselected { }
    private method fiber_clear { }
    private method fiber_load { }
    private method fiber_save { index name }
    private method fiber_saveall { }
    private method fiber_saveselected { }
    private method fiber_track { }
    private method fiber_statistics { }
    private method fiber_error { }
    private method fiber_threshold { }
    private method fiber_quantize { }
    private method fiber_addstatistics { title results tag }
    private method fiber_beginstatistics { title overwrite }
    private method fiber_savestatistics { }
    private method fiber_selection_cb { }
    private method fiber_color_cb { }
    private method fiber_name_cb { }
    private method fiber_display_cb { }

    private method result_clear { }
    private method result_propagate { }
    private method update_list { node }    
    public method PropagationCallback { filter comment } 

    private method savedir_dialog { base initdir title }
    private method openfile_dialog { base initdir title mask }
}  

#-------------------------------------------------------------------------------------------
#  Close window 
# -------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::DismissWindow { } {
    
    set ok "yes"

    if { $global_list(tensor_fname) != "" } {

	set ok [ tk_messageBox -type yesnocancel -default no \
		     -parent $basewidget -title "Think again ..." \
		     -message "Unload all images from memory?" -icon question  ]
    }
    
    if { $ok != "yes" } {	
        return
    }

	fiber_clear
	
	view_clear_display 
	
	$this ClearAllResults
	
	$this SetTitle $global_list(appname)
	
	set global_list(mask_fname) ""
	set global_list(tensor_fname) ""
       
    [ $parent GetViewer ] UpdateDisplay
    $this DestroyWindow
}

#-------------------------------------------------------------------------------------------
#  Initialize tool
# -------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::InitializeControl { } {

    set global_list(appname) "Tensor Connectivity"
    
    set appname "$global_list(appname) $version"
    set aboutstring "(c) Marcel Jackowski 2014-2016"

    # create list of basic colors
    set global_list(tkcolor_list) "\#ff0000 \#ffff00 \#00ff00 \#00ffff \#0000ff \#ff00ff"
    
    # create image titles
    set global_list(result_title) \
	[ list \
	      { "Diffusion tensor"      "dti_tensor" 0 } \
	      { "Regions of interest"   "dti_roi" 0 } \
	      { "Map for analysis"      "dti_map" 0 } \
	      { "Propagation solution"  "dti_solution" 1 } \
	      { "Convergence image"     "dti_converge" 1 } \
	      { "Fiber quantization"    "dti_fiber" 1 }  \
	      { "Gradient"              "dti_grad" 1 } \
	      { "Characteristics"       "dti_char" 1 } \
	      { "Dot Product"           "dti_dot" 1 } \
              { "Auxiliary"             "dti_aux" 1 } ]
    
    foreach item $global_list(result_title) {
 	lappend result_list [ pxitclimage \#auto ]
    }  
    
    set global_list(result_fdir) [ pwd ]
    set global_list(result_fpreffix) "preffix_"
    set global_list(result_list) $result_list
    
    set global_list(mask_fname) ""
    set global_list(mask_nreg) 1
    
    set global_list(tensor_fname) ""    
    set global_list(map_fname) ""

    set global_list(tensor_symm) 1
        
    set global_list(solution_fname) ""
    set global_list(solution_reuse) 0
    set global_list(solution_maxiter) 10
    set global_list(solution_maxerror) 1.0
    set global_list(solution_spacing) 1.0
    set global_list(solution_error) 0
    set global_list(solution_errorlabel) "Error: 0.0"
    set global_list(solution_method) 0
    set global_list(solution_inf) 32767
    set global_list(solution_a) 3
    set global_list(solution_b) 2
    set global_list(solution_c) 1
    
    set global_list(track_seedtype) 0
    set global_list(track_map) 0
    set global_list(track_seedregion) 1
    set global_list(track_seedwidth) 3
    set global_list(track_seedheight) 3
    set global_list(track_seeddepth) 3
    set global_list(track_seeddens) 1.0  
    set global_list(track_integration) 0
    set global_list(track_steplen) 0.25
    set global_list(track_minspeed) 1.0
    set global_list(track_maxdist) 100.0
    set global_list(track_mindist) 10.0
    set global_list(track_fitspline) 1
    set global_list(track_splinetype) 0
    set global_list(track_splinesubdivision) 0
    set global_list(track_splinelength) 0.1
    set global_list(track_splinenumber) 100
    set global_list(track_regionfilter) 0
    set global_list(track_regionnumber) 1
    set global_list(track_autostat) 1
    set global_list(track_flipping) 0
    set global_list(track_flipx) 0
    set global_list(track_flipy) 0
    set global_list(track_flipz) 0

    set global_list(fiber_display) 1
    set global_list(fiber_color) "\#ff0000"
    set global_list(fiber_list) {}
    set global_list(fiber_color_list) {}
    set global_list(fiber_disp_list) {}
    set global_list(fiber_thresh_val) 0.0
    set global_list(fiber_thresh_dist) 1.0
    
    set global_list(stat_overwrite) 0

    set global_list(view_colorbar) 0 
    set global_list(view_fiber) 0
    set global_list(view_fiber_tuberadius) 0.20
    set global_list(view_fiber_pointsize) 1.0
    set global_list(view_contour) 0
    set global_list(view_contour_levels) 10
    set global_list(view_contour_from) 0
    set global_list(view_contour_to) 1000
    set global_list(view_contour_width) 2.0
    set global_list(view_gradient) 0
    set global_list(view_character) 0
    set global_list(view_fiber_luktype) 0
    
    ## create actors for visualization
    set global_list(actor_colorbar) -1
    set global_list(actor_fiber) {}
    set global_list(actor_contour) [list -1 -1 -1 ]
    set global_list(actor_gradient) [list -1 -1 -1]
    set global_list(actor_character) [list -1 -1 -1]

    ## vtk data
    set global_list(vtk_fiber_tube) {}
    set global_list(vtk_fiber_polydata) {}
    
    initialize_glyphs
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create visualization glyphs
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::initialize_glyphs { } {

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
    
    ## create tube source
    set tube_src [ vtkCylinderSource [ pxvtable::vnewobj ] ]
    $tube_src SetHeight 2.0
    $tube_src SetRadius 0.25
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
    $ellipsoid_xfrm Scale 2.0 2.0 2.0

    set ellipsoid [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $ellipsoid SetInput [ $ellipsoid_src GetOutput ]
    $ellipsoid SetTransform $ellipsoid_xfrm
    lappend global_list(glyph_list) $ellipsoid

    $ellipsoid_src Delete
    $ellipsoid_xfrm Delete
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load tensor
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::loadsolution { } {

    ## browse for filename
    set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
    set fname  [tk_getOpenFile -title "Select solution" -filetypes $typelist -initialdir $global_list(result_fdir) -parent $basewidget ]
    
    if { $fname != "" } {
	
	set global_list(result_fdir) [ file dirname $fname ]
	
	## load mask into memory
	LoadSolution $fname
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Load in tensor image 
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::LoadSolution { fname } {
    
    set SOLUTION 3
    
    set sol [ lindex $global_list(result_list) $SOLUTION ]
    
    ## load series into memory
    set ana [ vtkpxAnalyzeImageSource  [ pxvtable::vnewobj ]]
    
    $this SetFilterCallbacks $ana "Reading in solution image..."
    
    WatchOn
    
    ## load series 
    $ana Load $fname 
    
    $sol SetFromAnalyzeSource $ana    
    set global_list(solution_fname) $fname
    set global_list(result_fpreffix) "[file root [file tail $fname] ]_"
    
    WatchOff
    
    $ana Delete    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load mask
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::loadmask { } {
    
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
itcl::body mpjconnectivity::LoadMask { fname } {

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
    set range [ [ [ [ $mask GetImage ] GetPointData ] GetScalars ] GetRange ]

    ## take the maximum range as the number of regions in mask
    set global_list(mask_nreg) [ expr int([ lindex $range 1 ])] 
    $widget_list(mask_nreg) configure -range [ list 1 $global_list(mask_nreg) ]
    
    set global_list(mask_fname) $fname

    WatchOff

    $ana Delete
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load map
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::loadmap { } {

    ## browse for filename
    set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
    set fname  [tk_getOpenFile -title "Select map" -filetypes $typelist -initialdir $global_list(result_fdir) -parent $basewidget ] 
    
    if { $fname != "" } {
	
	set global_list(result_fdir) [ file dirname $fname ]
	
	## load map into memory
	LoadMap $fname
    }    
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Load in map image 
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::LoadMap { fname } {

    ## load series into memory
    set ana [ vtkpxAnalyzeImageSource  [ pxvtable::vnewobj ]]
    
    $this SetFilterCallbacks $ana "Reading in map image..."
    
    WatchOn
    
    ## load series 
    $ana Load $fname 
    
    [ lindex $global_list(result_list) 2 ] SetFromAnalyzeSource $ana
    
    set global_list(map_fname) $fname
    
    WatchOff
    
    $ana Delete
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load tensor
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::loadtensor { } {

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
itcl::body mpjconnectivity::LoadTensor { fname } {

    set TENSOR 0
    set MASK 1
    set MAP 2
    set FIBERS 3

    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set fibers [ lindex $global_list(result_list) $FIBERS ]
    set map [lindex $global_list(result_list) $MAP ]

    WatchOn

    ## load series into memory
    set ana [ vtkpxAnalyzeImageSource  [ pxvtable::vnewobj ]]
    
    $this SetFilterCallbacks $ana "Reading in tensor image..."
    
    ## load series 
    $ana Load $fname 

    $tensor SetFromAnalyzeSource $ana

    set global_list(tensor_fname) $fname

    ## determine coordinate transform
    set or [ $ana GetOrientation ]
    #if { $or == 0 } {
 	#set global_list(track_flipx) 1
 	#set global_list(track_flipy) 1
    #} elseif { $or == 1 } {
 	#set global_list(track_flipx) 1
    #} 
    
    $ana Delete

    scan [ [ $tensor GetImage ] GetDimensions ] "%d %d %d" dx dy dz	       
    set nc [ [ $tensor GetImage ] GetNumberOfScalarComponents ]
    set name [ file tail [ file root $fname ] ]
        
    set global_list(tensor_symm) [ expr $nc == 6 ]
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

    set global_list(result_fdir) "[ file dir $fname ]"
    set global_list(result_fpreffix) "[file tail $name ]"

    ## create fiber image
    set ext [ vtkImageExtractComponents [ pxvtable::vnewobj ] ]
    $ext SetInput [ $tensor GetImage ]
    $ext SetComponents 0
    $ext Update

    $fibers CopyImageHeader [ $tensor GetImageHeader ]
    $fibers ShallowCopyImage [ $ext GetOutput ]
    $ext Delete

    #set global_list(result_fdir) [ file dir $fname ]
    #set global_list(result_fpreffix) [ file join $global_list(result_fdir) "${name}" ]

    WatchOff

    #$this SetTitle "$global_list(appname): $name (${dx}x${dy}x${dz}, $nc frames)"
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Set result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::SetResultPreffix { preffix } {
    set global_list(result_fpreffix) $preffix
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Get result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::GetResultPreffix { } {
    return $global_list(result_fpreffix)
}

itcl::body mpjconnectivity::SetCurrentDirectory { fname } {
    set global_list(result_fdir) $fname
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Clear all results
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::ClearAllResults { } {
    
    foreach result $global_list(result_list) {
	$result Clear
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::SaveAllResults { } {

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
#  PUBLIC: Save single result
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::SaveResult { index } {        

    set fname $global_list(result_fpreffix)
    set fname "$fname[ lindex [lindex $global_list(result_title) $index ] 1 ]"

    [lindex $global_list(result_list) $index] Save $fname
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::saveallresults { } {

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
itcl::body mpjconnectivity::saveresult { } {
    
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
itcl::body mpjconnectivity::DisplayResult { index { mode image } } {
      # -------------------------------------D---
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

    #set resultluk  $widget_list(view_resultluk) 

    #$resultluk SetSaturationRange $rmin $rmax
    #$resultluk SetTableRange $rmin $rmax
    #$resultluk Update


    # Step 2.
    # Ship Image and Lookup Table 

    if { $mode == "image" } {
    $parent SetImageFromObject $imgtodisplay $this
    #$parent SetLookupTable [ $resultluk GetLookupTable ]
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
itcl::body mpjconnectivity::displayresult { { mode image } } {

     set sel_item [ $widget_list(result_list) getcurselection ]
        
    if { $sel_item != "" } {
    set index [ $widget_list(result_list) index [ lindex $sel_item 0 ] ]
    set result [ lindex $global_list(result_list) $index ]

    #set range [ [ [ [ $result GetImage ] GetPointData ] GetScalars ] GetRange ]
    #$widget_list(view_resultluk) SetTableRange [ lindex $range 0 ] [ lindex $range 1 ]
    #   $widget_list(view_resultluk) Update

    #puts "$index $mode"
    $this DisplayResult $index $mode
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: toggle result computation on/off
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::toggleresult { } {
   
    set color(0) \#dcdcdc
    set color(1) black

    set sel_item [ $widget_list(result_list) getcurselection ]

    if { $sel_item != "" } {

	set index [ $widget_list(result_list) index [ lindex $sel_item 0 ] ]

	if { $index > 2 } {
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
itcl::body mpjconnectivity::result_clear { } {
    for { set i 0 } { $i < [ llength $global_list(result_list) ] } { incr i } {		
	if { $i > 0 } {	    
	    
	    set title [ lindex [ lindex $global_list(result_title) $i ] 0 ]	    	    
	    
	    if { $title == "Region of interest mask" } {
		if { $global_list(mask_auto) } {
		    [ lindex $global_list(result_list) $i ] Clear
		}
	    } elseif { $title != "Propagation solution" } {
		[ lindex $global_list(result_list) $i ] Clear
	    }
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Clear display
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_clear_display { } {
    
    ## remove fibers
    foreach actor $global_list(actor_fiber) {	    
	if { $actor != "-1" } {
	    [ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor	    
	    $actor Delete	    
	}
	set global_list(actor_fiber) {}
    }
    
    ## remove contours
    foreach actor $global_list(actor_contour) {	    
	if { $actor != "-1" } {
	    [ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor	    
	    $actor Delete	    
	}
	set global_list(actor_contour) [list -1 -1 -1]
    }

    ## remove colorbar
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
itcl::body mpjconnectivity::view_update_display { } {
    
    if { $global_list(view_colorbar) } {
	view_create_colorbar
    }
    
    if { $global_list(view_contour) } {
	view_create_contour
    }

    if { $global_list(view_gradient) } {
	view_create_gradient
    }
    
    if { $global_list(view_character) } {
	view_create_character
    }    
    
    [ $parent GetViewer ] UpdateDisplay
}


#-------------------------------------------------------------------------------------------
#  Toggle On/Off colorbar visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_toggle_colorbar { } {
    
    if { $global_list(view_colorbar) } {
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
#  Toggle On/Off contour visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_toggle_contour { } {
    
    if { $global_list(view_contour) } {	
	view_create_contour
    } else { 
	foreach actor $global_list(actor_contour) {
	    if { $actor != "-1" } {
		for { set j 0 } { $j < 4 } { incr j } {
		    [ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] RemoveActor $actor
		}
		$actor Delete	    
	    }
	}
	set global_list(actor_contour) [ list -1 -1 -1 ]
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  Toggle On/Off gradient visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_toggle_gradient { } {
    
    if { $global_list(view_gradient) } {	
	view_create_gradient
    } else { 
	foreach actor $global_list(actor_gradient) {
	    if { $actor != "-1" } {
		for { set j 0 } { $j < 4 } { incr j } {
		    [ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] RemoveActor $actor
		}
		$actor Delete	    
	    }
	}
	set global_list(actor_gradient) [ list -1 -1 -1 ]
    }
    
    [ $parent GetViewer ] UpdateDisplay
}


#-------------------------------------------------------------------------------------------
#  Toggle On/Off characteristic visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_toggle_character { } {
    
    if { $global_list(view_character) } {	
	view_create_character
    } else { 
	foreach actor $global_list(actor_character) {
	    if { $actor != "-1" } {
		for { set j 0 } { $j < 4 } { incr j } {
		    [ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] RemoveActor $actor
		}
		$actor Delete	    
	    }
	}
	set global_list(actor_character) [ list -1 -1 -1 ]
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  View fiber bundle
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_create_fiber_glyph { index } {

    set field(0) fractani
    set field(1) meandiff
    set field(2) distance
    set field(3) angle
    set field(4) roi
    set field(5) other

    ## create mapper
    set fiberMapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
    
    ## get current glyph
    set cur_glyph [ $widget_list(view_fiber_glyph) \
			index [ $widget_list(view_fiber_glyph) get ] ]
    
    ## get current fiber colormap filter    
#    set filter [ $widget_list(view_fiber_filter) index [ $widget_list(view_fiber_filter) get ]]

    if { $cur_glyph == 1 } {
	set tube [ lindex $global_list(vtk_fiber_tube) $index ]
	
	$tube SetInput [ lindex $global_list(vtk_fiber_polydata) $index ]
	$tube SetRadius $global_list(view_fiber_tuberadius)
	$tube SetNumberOfSides 6
	$tube Update

	$fiberMapper SetInput [ $tube GetOutput ]	
	$fiberMapper SetLookupTable [ $widget_list(view_fiberluk) GetLookupTable ]
	$fiberMapper UseLookupTableScalarRangeOn
	
	if { $global_list(view_fiber_luktype) == 0 } {
	    $fiberMapper SetScalarModeToUsePointData
	} else {
	    $fiberMapper SetScalarModeToUseCellData
	}

	#$fiberMapper ColorByArrayComponent $field($filter) 0

	if { $global_list(view_fiber_luk) } {	    
	    $fiberMapper ScalarVisibilityOn
	} else {
	    $fiberMapper ScalarVisibilityOff
	}

	

    } 

    if { [expr $cur_glyph == 0 || $cur_glyph == 2 ] } {
	
	set polydata [ lindex $global_list(vtk_fiber_polydata) $index ]
	$fiberMapper SetInput $polydata
	$fiberMapper SetLookupTable [ $widget_list(view_fiberluk) GetLookupTable ]
	$fiberMapper UseLookupTableScalarRangeOn
	
	if { $global_list(view_fiber_luktype) == 0 } {
	    $fiberMapper SetScalarModeToUsePointData
	} else {
	    $fiberMapper SetScalarModeToUseCellData
	}

	#$fiberMapper SetScalarModeToUseCellData
	#$fiberMapper ColorByArrayComponent $field($filter) 0

	if { $global_list(view_fiber_luk) } {	    
	    $fiberMapper ScalarVisibilityOn
	} else {
	    $fiberMapper ScalarVisibilityOff
	}
    }


    ## create actor
    set fiberActor [ vtkActor [ pxvtable::vnewobj ] ]
    $fiberActor SetMapper $fiberMapper
    
    if { $cur_glyph == 2 } {
	[ $fiberActor GetProperty ] SetRepresentationToPoints
	[ $fiberActor GetProperty ] SetPointSize $global_list(view_fiber_pointsize)
    }
    
    $fiberMapper Delete
    
    ## convert tk color spec to vtk
    set color [ lindex $global_list(fiber_color_list) $index ]
    set red "0x[string range $color 1 2]"
    set green "0x[string range $color 3 4]"
    set blue "0x[string range $color 5 6]"
    set red [ expr [ format "%d" $red ] / 255.0 ]
    set green [ expr [ format "%d" $green ] / 255.0 ]
    set blue [ expr [ format "%d" $blue ] / 255.0 ]
    
    [ $fiberActor GetProperty ] SetColor $red $green $blue
    
    return $fiberActor
}

#-------------------------------------------------------------------------------------------
#  View fibers
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_create_fibers { } {    

    set num [ llength $global_list(fiber_list) ]
    
    for {set j 0} {$j < $num} {incr j} {
	
	set actor [ view_create_fiber_glyph $j ]
	
	lappend global_list(actor_fiber) $actor
	
	## add actor if to be displayed
	if { [ lindex $global_list(fiber_disp_list) $j ] == 1 } { 
	    for { set j 0 } { $j < 4 } { incr j } {
		[ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] AddActor $actor	
	    }
	}
    }
    
    [ $parent GetViewer ] UpdateDisplay
    
}

#-------------------------------------------------------------------------------------------
#  View colorbar
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_create_colorbar { } {
        
    set title [ $widget_list(view_fiber_filter) get ]
    
    set bar [ vtkScalarBarActor [ pxvtable::vnewobj ] ]
    $bar SetLookupTable [ $widget_list(view_fiberluk) GetLookupTable ]
    
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
#  PRIVATE: Compute ROI extents
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_compute_roi_extents { orient dim levels } {
   
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
itcl::body mpjconnectivity::view_extract_roi { exts image } {
    
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
    set roi [ vtkImageReslice [ pxvtable::vnewobj ] ]
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
#  View contour
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_create_contour_glyph { scalars } {
        
    set range [ [ $widget_list(view_frontluk) GetLookupTable ] GetTableRange ]
    set global_list(view_contour_from) [ lindex $range 0 ]
    set global_list(view_contour_to) [ lindex $range 1 ]
    
    set contour [ vtkMarchingContourFilter [ pxvtable::vnewobj ] ]
    $contour SetInput $scalars
    $contour GenerateValues $global_list(view_contour_levels) $global_list(view_contour_from) $global_list(view_contour_to)
    $contour Update
    
    set contourMapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
    $contourMapper SetInput [ $contour GetOutput ]
    $contourMapper SetLookupTable [ $widget_list(view_frontluk) GetLookupTable]
    $contourMapper UseLookupTableScalarRangeOn
    $contourMapper ScalarVisibilityOn
    
    ## create contour actor
    set contourActor [ vtkActor [ pxvtable::vnewobj ] ]
    $contourActor SetMapper $contourMapper
    [ $contourActor GetProperty ] SetLineWidth $global_list(view_contour_width)
    $contourMapper Delete

    $contour Delete

    return $contourActor
}

#-------------------------------------------------------------------------------------------
#  View contour
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_create_contour { } {
    
    set SOLUTION 3
    set solution [ lindex $global_list(result_list) $SOLUTION ]

    ## get current slice numbers
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set dims [ [ $solution GetImage ] GetDimensions ]
    
    ## plot on slices
    for { set i 0 } { $i < 3 } { incr i } {
	
	set ext [ view_compute_roi_extents $i $dims $levels ]
	set scalars [ [ view_extract_roi $ext $solution ] GetOutput ]
	
	set actor [ view_create_contour_glyph $scalars ]
	set global_list(actor_contour) [ lreplace $global_list(actor_contour) $i $i $actor ]
	
	$scalars Delete
	
	for { set j 0 } { $j < 4 } { incr j } {
	    [ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] AddActor $actor	
	}
    }    
}

#-------------------------------------------------------------------------------------------
#  View gradient
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_create_gradient_glyph { scalars glyph } {    
    
    ## compute gradient of propagation map
#    set imgrad [ vtkImageGradient [ pxvtable::vnewobj ] ]
#    $imgrad SetInput $scalars
#    $imgrad SetDimensionality 3
#    $imgrad Update
    
    ## convert into vector field
    set imtovec [ vtkmpjImageToVectors [ pxvtable::vnewobj ] ]
    $imtovec SetInput $scalars
    $imtovec Update
    
    ## merge data
    set merge [ vtkMergeFilter [ pxvtable::vnewobj ] ]
    $merge SetGeometry $scalars
    $merge SetVectors [ $imtovec GetOutput ]
    $merge SetScalars $scalars
    $merge Update
    
#    $imgrad Delete
    $imtovec Delete
    
    ## create glypher field   
    set glypher [ vtkGlyph3D [ pxvtable::vnewobj ] ] 
    $glypher SetInput [ $merge GetOutput ]
    $glypher SetSource [ $glyph GetOutput ]
    $glypher ScalingOn
    $glypher ClampingOn
    #    $glypher SetScaleFactor $global_list(view_eigen_maxscale)
    $glypher SetScaleModeToDataScalingOff
    $glypher SetVectorModeToUseVector
    $glypher Update
    
    ## set up glyph mapper
    set glyphMapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
    $glyphMapper SetInput [ $glypher GetOutput ]
    #$glyphMapper SetLookupTable [ $widget_list(view_fiberluk) GetLookupTable ]
    #$glyphMapper UseLookupTableScalarRangeOn
    $glyphMapper ScalarVisibilityOff
    
    ## create glyph actor
    set glyphActor [ vtkActor [ pxvtable::vnewobj ] ]
    $glyphActor SetMapper $glyphMapper
    [ $glyphActor GetProperty ] SetColor 0.0 1.0 1.0
    
    $glypher Delete
    $glyphMapper Delete
    
    return $glyphActor
}

#-------------------------------------------------------------------------------------------
#  View gradient
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_create_gradient { } {
    
    set GRADIENT 6
    set gradient [ lindex $global_list(result_list) $GRADIENT ]

    ## get current slice numbers
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set dims [ [ $gradient GetImage ] GetDimensions ]
    
    ## get current glyph
    set cur_glyph [ $widget_list(view_fiber_glyph) index [ $widget_list(view_fiber_glyph) get ] ]
    set glyph_source [ lindex $global_list(glyph_list) 0 ]
    
    ## plot on slices
    for { set i 0 } { $i < 3 } { incr i } {
	
	set ext [ view_compute_roi_extents $i $dims $levels ]
	set scalars [ [ view_extract_roi $ext $gradient ] GetOutput ]
	
	set actor [ view_create_gradient_glyph $scalars $glyph_source ]
	set global_list(actor_gradient) [ lreplace $global_list(actor_gradient) $i $i $actor ]
	
	$scalars Delete

	for { set j 0 } { $j < 4 } { incr j } {
	    [ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] AddActor $actor	
	}
    }    
}

#-------------------------------------------------------------------------------------------
#  View characteristic
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_create_character_glyph { scalars vectors glyph } {    
    
    ## compute gradient of propagation map
    #set imgrad [ vtkImageGradient [ pxvtable::vnewobj ] ]
    #$imgrad SetInput $scalars
    #$imgrad SetDimensionality 3
    #$imgrad Update
    
    ## compute characteristics
    #set imchar [ vtkmpjImageCharacteristics [ pxvtable::vnewobj ] ]
    #$imchar SetInput $scalars
    #$imchar SetGradient [ $imgrad GetOutput ]
    #$imchar Update
    
    ## convert into vector field
    set imtovec [ vtkmpjImageToVectors [ pxvtable::vnewobj ] ]
    $imtovec SetInput $vectors
    $imtovec Update
    
    ## merge data
    set merge [ vtkMergeFilter [ pxvtable::vnewobj ] ]
    $merge SetGeometry $scalars
    $merge SetVectors [ $imtovec GetOutput ]
    $merge SetScalars $scalars
    $merge Update
    
    #$imchar Delete
    $imtovec Delete
    
    ## create glypher field   
    set glypher [ vtkGlyph3D [ pxvtable::vnewobj ] ] 
    $glypher SetInput [ $merge GetOutput ]
    $glypher SetSource [ $glyph GetOutput ]
    $glypher ScalingOn
    $glypher ClampingOn
    #    $glypher SetScaleFactor $global_list(view_eigen_maxscale)
    $glypher SetScaleModeToDataScalingOff
    $glypher SetVectorModeToUseVector
    $glypher Update
    
    ## set up glyph mapper
    set glyphMapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
    $glyphMapper SetInput [ $glypher GetOutput ]
#    $glyphMapper SetLookupTable [ $widget_list(view_fiberluk) GetLookupTable ]
#    $glyphMapper UseLookupTableScalarRangeOn
    $glyphMapper ScalarVisibilityOff
    
    ## create glyph actor
    set glyphActor [ vtkActor [ pxvtable::vnewobj ] ]
    $glyphActor SetMapper $glyphMapper
    [ $glyphActor GetProperty ] SetColor 0.0 1.0 0.0
    
    $glyphMapper Delete
    
    return $glyphActor
}

#-------------------------------------------------------------------------------------------
#  View characteristics
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_create_character { } {
    
    set SOLUTION 3
    set CHARACTER 7
    
    set solution [ lindex $global_list(result_list) $SOLUTION ]
    set character [ lindex $global_list(result_list) $CHARACTER ]
    
    ## get current slice numbers
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set dims [ [ $solution GetImage ] GetDimensions ]
    
    ## get current glyph
    set cur_glyph [ $widget_list(view_fiber_glyph) index [ $widget_list(view_fiber_glyph) get ] ]
    set glyph_source [ lindex $global_list(glyph_list) 0 ]
    
    ## plot on slices
    for { set i 0 } { $i < 3 } { incr i } {
	
	set ext [ view_compute_roi_extents $i $dims $levels ]
	
	set vectors [ [ view_extract_roi $ext $character ] GetOutput ]
	set scalars [ [ view_extract_roi $ext $solution ] GetOutput ]
	
	set actor [ view_create_character_glyph $scalars $vectors $glyph_source ]
	set global_list(actor_character) [ lreplace $global_list(actor_character) $i $i $actor ]
	
	$scalars Delete
	
	for { set j 0 } { $j < 4 } { incr j } {
	    [ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] AddActor $actor	
	}
    }    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in map type
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::track_map_cb { } {    
    set global_list(track_map) \
	[ $widget_list(track_map) index [ $widget_list(track_map) get ] ]    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in solution methods
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::solution_method_cb { } {
    set global_list(solution_method) \
	[ $widget_list(solution_method) index [ $widget_list(solution_method) get ] ]    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in seed type
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::track_seedtype_cb { } {

    set state(0) disabled; set state(1) normal    
    set global_list(track_seedtype) \
	[ $widget_list(track_seedtype) index [ $widget_list(track_seedtype) get ] ]

    if { $global_list(track_seedtype) < 2 } {
	$widget_list(track_seedwidth) configure -state disabled
	$widget_list(track_seedheight) configure -state disabled
	$widget_list(track_seeddepth) configure -state disabled
	if { $global_list(track_seedtype) == 1 } {
	    $widget_list(track_seeddens) configure -state normal
	} else {
	    $widget_list(track_seeddens) configure -state disabled
	}
    } else {
	$widget_list(track_seedwidth) configure -state normal
	$widget_list(track_seedheight) configure -state normal
	$widget_list(track_seeddepth) configure -state normal
	$widget_list(track_seeddens) configure -state normal
    }
    
    if { $global_list(track_seedtype) == 1 } {
	$widget_list(track_seedregion) configure -state normal
    } else {
	$widget_list(track_seedregion) configure -state disabled	
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in region filtering option
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::track_regionfilter_cb { } {

    set state(1) normal
    set state(0) disabled

    $widget_list(track_regionnumber) configure -state \
	$state($global_list(track_regionfilter))
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in integration
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::track_integration_cb { } {
    set global_list(track_integration) \
	[ $widget_list(track_integration) index [ $widget_list(track_integration) get ] ]
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in spline types
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::track_splinetype_cb { } {
    set global_list(track_splinetype) \
	[ $widget_list(track_splinetype) index [ $widget_list(track_splinetype) get ] ]
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in spline subdivision
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::track_splinesubdivision_cb { } {
    set global_list(track_splinesubdivision) \
	[ $widget_list(track_splinesubdivision) index [ $widget_list(track_splinesubdivision) get ] ]
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon an update button press
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_update_display_cb { } { 
    
    view_clear_display
    view_update_display
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in bundle radius
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_fiber_tuberadius_cb { } {
    
    foreach item $global_list(vtk_fiber_tube) {
	$item SetRadius $global_list(view_fiber_tuberadius)
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in point size
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_fiber_pointsize_cb { } {
    
    foreach actor $global_list(actor_fiber) {
	[ $actor GetProperty ] SetPointSize $global_list(view_fiber_pointsize)
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in colormap for fibers
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_fiber_colormap_cb { } {
    
    foreach \
	actor $global_list(actor_fiber) {
	    
	    ## create mapper
	    set fiberMapper [ $actor GetMapper ]

	    if { $global_list(view_fiber_luk) } {
		$fiberMapper ScalarVisibilityOn
	    } else {
		$fiberMapper ScalarVisibilityOff
	    }

	    if { $global_list(view_fiber_luktype) == 0 } {
		$fiberMapper SetScalarModeToUsePointData
	    } else {
		$fiberMapper SetScalarModeToUseCellData
	    }
	    

	}
    
      [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in bundle glyph
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_fiber_glyph_cb { } {
    
    ## get current glyph
    set cur_glyph [ $widget_list(view_fiber_glyph) \
			index [ $widget_list(view_fiber_glyph) get ] ]
    
    foreach \
	actor $global_list(actor_fiber) \
	tube $global_list(vtk_fiber_tube) \
	polydata $global_list(vtk_fiber_polydata) {
	    
	    ## create mapper
	    set fiberMapper [ $actor GetMapper ]
	    
	    if { $cur_glyph == 1 } {
		
		$tube SetInput $polydata
		$tube SetRadius $global_list(view_fiber_tuberadius)
		$tube SetNumberOfSides 6
		$tube Update
		
		$fiberMapper SetInput [ $tube GetOutput ]

		if { $global_list(view_fiber_luktype) == 0 } {
		    $fiberMapper SetScalarModeToUsePointData
		} else {
		    $fiberMapper SetScalarModeToUseCellData
		}
	    		
		if { $global_list(view_fiber_luk) } {
		    $fiberMapper ScalarVisibilityOn
		} else {
		    $fiberMapper ScalarVisibilityOff
		}

		
		
	    } else {
		
		$fiberMapper SetInput $polydata

		if { $global_list(view_fiber_luktype) == 0 } {
		    $fiberMapper SetScalarModeToUsePointData
		} else {
		    $fiberMapper SetScalarModeToUseCellData
		}
		
		if { $global_list(view_fiber_luk) } {
		    $fiberMapper ScalarVisibilityOn
		} else {
		    $fiberMapper ScalarVisibilityOff
		}
				
	    }
	    
	    if { $cur_glyph == 2 } {
		[ $actor GetProperty ] SetRepresentationToPoints
		[ $actor GetProperty ] SetPointSize $global_list(view_fiber_pointsize)
	    } else {
		[ $actor GetProperty ] SetRepresentationToSurface 
	    }

	}
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in fiber filter selection
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_fiber_filter_cb { } {
    
    set field(0) validity

    ## current fiber colormap filter    
    set filter [ $widget_list(view_fiber_filter) index [ $widget_list(view_fiber_filter) get ]]    
    
    foreach \
	actor $global_list(actor_fiber) {	    
	    
	    ## create mapper
	    #set fiberMapper [ $actor GetMapper ]	   	    
	    #$fiberMapper SetScalarModeToUsePointFieldData
	    #$fiberMapper ColorByArrayComponent $field($filter) 0
	}
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in fiber filter selection
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_fiber_luktype_cb { } {
    
    ## current fiber colormap filter    
    set filter [ $widget_list(view_fiber_luktype) index [ $widget_list(view_fiber_luktype) get ]]    
    set global_list(view_fiber_luktype) $filter

    puts "filter: $filter"

    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  Toggle On/Off fiber visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::view_toggle_fiber { } {
    
    if { $global_list(view_fiber) } {	
	view_create_fibers	
    } else { 
	foreach \
	    actor $global_list(actor_fiber) \
	    disp $global_list(fiber_disp_list) {
		if { $disp == 1 } {
		    for { set j 0 } { $j < 4 } { incr j } {
			[ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] RemoveActor $actor	
		    }
		}
		$actor Delete	    
	    }
	set global_list(actor_fiber) {}
    }

    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in bundle color
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_color_cb { } {

    set new_color [ tk_chooseColor -initialcolor $global_list(fiber_color) ]
    
    if { [ llength $new_color ] > 0 } {
	
	set global_list(fiber_color) $new_color
	set fiber_color [ image create photo -width 15 -height 15 ]
	$fiber_color put $global_list(fiber_color) -to 0 0 15 15	
	$widget_list(fiber_color) configure -image $fiber_color
	
	## get current selection 
	set sel_item [ $widget_list(fiber_list) getcurselection ]
	
	if { $sel_item != "" } {
	    
	    set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 
	    set global_list(fiber_color_list) \
		[ lreplace $global_list(fiber_color_list) $sel_index $sel_index $new_color ]
	    
	    ## convert tk color spec to vtk
	    set color $new_color
	    set red "0x[string range $color 1 2]"
	    set green "0x[string range $color 3 4]"
	    set blue "0x[string range $color 5 6]"
	    set red [ expr [ format "%d" $red ] / 255.0 ]
	    set green [ expr [ format "%d" $green ] / 255.0 ]
	    set blue [ expr [ format "%d" $blue ] / 255.0 ]
	    
	    set fiberActor [ lindex $global_list(actor_fiber) $sel_index ]
	    [$fiberActor GetProperty] SetColor $red $green $blue

	    [ $parent GetViewer ] UpdateDisplay	    
	    
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon selection of bundles
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_selection_cb { } {
    
    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	## get current index
	set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 
	set sel_input [ lindex $global_list(fiber_list) $sel_index ]
	
	## get number of components
	set global_list(fiber_name) $sel_input
	set global_list(fiber_color) [ lindex $global_list(fiber_color_list) $sel_index ]
	set global_list(fiber_display) [ lindex $global_list(fiber_disp_list) $sel_index ]

	set fiber_color [ image create photo -width 15 -height 15 ]
	$fiber_color put $global_list(fiber_color) -to 0 0 15 15	
	$widget_list(fiber_color) configure -image $fiber_color
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in bundle name
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_name_cb { } {

    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	set new_name [ $widget_list(fiber_name) get ]
	
	## get current index
	set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 
	set global_list(fiber_list) \
	    [ lreplace $global_list(fiber_list) $sel_index $sel_index $new_name ]
	
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in bundle display option
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_display_cb { } {

    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	## get current index
	set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 
	
	puts "$sel_index"

	set global_list(fiber_disp_list) \
	    [ lreplace $global_list(fiber_disp_list) $sel_index $sel_index \
		  $global_list(fiber_display) ]
	
	set actor [ lindex $global_list(actor_fiber) $sel_index ]
	
	if { $global_list(fiber_display) } {
	    for { set j 0 } { $j < 4 } { incr j } {
		[ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] AddActor $actor	
	    }
	} else {
	    for { set j 0 } { $j < 4 } { incr j } {
		[ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] RemoveActor $actor	
	    }
	}
	
	[ $parent GetViewer ] UpdateDisplay	    
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Add bundle with a given name
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_addwithname { name polydata } {

    ## insert new fiber set
    set num_fibers [ llength $global_list(fiber_list) ]
    set next_color [lindex $global_list(tkcolor_list) [expr int(fmod($num_fibers,6))]]
    
    set global_list(fiber_color) $next_color
    set global_list(fiber_display) 1

    set fiber_color [ image create photo -width 15 -height 15 ]
    $fiber_color put $global_list(fiber_color) -to 0 0 15 15	
    $widget_list(fiber_color) configure -image $fiber_color
    
    set global_list(fiber_name) $name
    lappend global_list(fiber_list) $global_list(fiber_name)
    lappend global_list(fiber_disp_list) $global_list(fiber_display)    
    lappend global_list(fiber_color_list) $global_list(fiber_color)
    lappend global_list(vtk_fiber_polydata) $polydata	    
    lappend global_list(vtk_fiber_tube) [ vtkTubeFilter [ pxvtable::vnewobj ] ]
 
    $widget_list(fiber_list) selection clear 0 end
    $widget_list(fiber_list) selection set $num_fibers $num_fibers

    return $num_fibers
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Add fiber
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_add { polydata } {

    ## insert new fiber
    set num_fibers [ llength $global_list(fiber_list) ]
    set name "Bundle_[expr $num_fibers + 1]"
    
    set index [ fiber_addwithname $name $polydata ]
    
    return $index
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Clear all fibers
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_clear { } {
    
    set num_fibers [ llength $global_list(fiber_list) ]
    for { set i 0 } { $i < $num_fibers } { incr i } { 
	fiber_remove 0
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Remove fiber
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_remove { index } {

    if { $global_list(view_fiber) } {
	set display [ lindex $global_list(fiber_disp_list) $index ]
	if { $display } {
	    set actor [ lindex $global_list(actor_fiber) $index ]
	    for { set j 0 } { $j < 4 } { incr j } {
		[ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] RemoveActor $actor	
	    }
	    $actor Delete
	}
    }
    
    set global_list(fiber_list) \
	[ lreplace $global_list(fiber_list) $index $index ]
    set global_list(fiber_color_list) \
	[ lreplace $global_list(fiber_color_list) $index $index ]
    set global_list(fiber_disp_list) \
	[ lreplace $global_list(fiber_disp_list) $index $index ]
    
    set polydata [ lindex $global_list(vtk_fiber_polydata) $index ]
    $polydata Delete
    
    set tube [ lindex $global_list(vtk_fiber_tube) $index ]
    $tube Delete

    set global_list(actor_fiber) \
	[ lreplace $global_list(actor_fiber) $index $index ]
    set global_list(vtk_fiber_tube) \
	[ lreplace $global_list(vtk_fiber_tube) $index $index ]
    set global_list(vtk_fiber_polydata) \
	[ lreplace $global_list(vtk_fiber_polydata) $index $index ]
    
    set num_fibers [ llength $global_list(fiber_list) ]
    
    if { $num_fibers > 0 } {
	
	$widget_list(fiber_list) selection clear 0 end
	
	if { $index < $num_fibers } {
	    $widget_list(fiber_list) selection set $index $index
	} else {
	    $widget_list(fiber_list) selection set \
		[expr $num_fibers -1] [expr $num_fibers -1]
	}
	
	fiber_selection_cb
	
    } else {
	
	set global_list(fiber_fname) ""
	set global_list(fiber_display) 1
	set global_list(fiber_name) ""
	set global_list(fiber_color) [ lindex $global_list(tkcolor_list) 0 ]
	set fiber_color [ image create photo -width 15 -height 15 ]
	$fiber_color put $global_list(fiber_color) -to 0 0 15 15	
	$widget_list(fiber_color) configure -image $fiber_color
    }

    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Remove selected bundle
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_removeselected { } {
    
    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	## get current index
	set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 
	
	fiber_remove $sel_index
	
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: load fiber
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_load { } {
    
   set fname  [tk_getOpenFile -title "Select fiber file" -filetypes { {"Fibers" {.fib}}} -initialdir $global_list(result_fdir) -parent $basewidget ] 

    ## browse for filename
    if { $fname != "" } { 
	
	## open fiber file
	set fibr [ vtkXMLPolyDataReader [ pxvtable::vnewobj ] ]
	$this SetFilterCallbacks $fibr "Reading fiber..."
	$fibr SetFileName $fname
	$fibr Update
	
	set polydata [ vtkPolyData [ pxvtable::vnewobj ] ]
	$polydata ShallowCopy [ $fibr GetOutput ]
	
	$fibr Delete
	
	fiber_addwithname [ file root [ file tail $fname ] ] $polydata
	
	## keep fname
	set global_list(fiber_fname) $fname
	
	if { $global_list(view_fiber) } {	
	    view_create_fibers	
	} 
    }	
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: save individual fiber 
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_save { index name } {
    
    set polydata [ lindex $global_list(vtk_fiber_polydata) $index ]
    
    set fibw [ vtkXMLPolyDataWriter [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $fibw "Saving fiber..."

    $fibw SetFileName $name
    $fibw SetInput $polydata
    $fibw Write	
    
    $fibw Delete    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: save selected fiber
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_saveselected { } {
    
    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    if { $sel_item != "" } {
	
    ## browse for directory
    set fdir [ tk_chooseDirectory -title " Select directory" -parent $basewidget -initialdir $global_list(result_fdir) ]
    	
	## browse for directory
	if { $fdir != "" } {
	    	    
	    ## get current index
	    set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 
	    
	    set item [ lindex $global_list(fiber_list) $sel_index ]	
	    
	    fiber_save $sel_index [ file join $fdir "$item.fib" ]	
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: save all fibers
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_saveall { } {
    
  ## browse for directory    
    set fdir [ tk_chooseDirectory -title " Select directory" -parent $basewidget -initialdir $global_list(result_fdir) ]

    ## browse for directory
    if { $fdir != "" } { 
	
	## get number of fiber bundles
	set num [ llength $global_list(fiber_list) ]
	
	## save all fibers
	for { set i 0 } { $i < $num } { incr i } {	
	    set item [ lindex $global_list(fiber_list) $i ]	
	    fiber_save $i [ file join $fdir "$item.fib" ]	
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save statistics
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_savestatistics { } {

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
    
    $widget_list(stat_text) export $fname

}

#-------------------------------------------------------------------------------------------
#  PRIVATE: compute fiber statistics
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_beginstatistics { title overwrite } {   
    
    if { $overwrite } {
	$widget_list(stat_text) delete 1.0 end
    }    

    $widget_list(stat_text) insert \
	end "-----------------------+------------+------------+------------+------------+------------\n" even
    $widget_list(stat_text) insert \
	end "$title      MIN          MAX         MEAN        VARIANCE      STD DEV\n" odd
    $widget_list(stat_text) insert \
	end "-----------------------+------------+------------+------------+------------+------------\n" even
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: compute fiber statistics
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_addstatistics { title results tag } {   
    
    set min  [ format %8.4f [ lindex $results 0 ] ]
    set max  [ format %8.4f [ lindex $results 1 ] ]
    set mean [ format %8.4f [ lindex $results 2 ] ]
    set var  [ format %8.4f [ lindex $results 3 ] ]
    set dev  [ format %8.4f [ lindex $results 4 ] ]
    
    $widget_list(stat_text) insert end " $title |  $min  |  $max  |  $mean  |  $var  |  $dev  \n" $tag
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: compute fiber statistics
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_error { } {

    ## get current selection     
    set sel_item [ $widget_list(fiber_list) getcurselection ]

    if { $sel_item != "" } {
	
	## get current index
	set target_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ]

	set dist [ vtkmpjStreamlineDistance [ pxvtable::vnewobj ] ]
	$dist SetReference [ lindex $global_list(vtk_fiber_polydata) 0 ]
	$dist SetTarget [ lindex $global_list(vtk_fiber_polydata) $target_index ]
	$dist SetReferenceIndex 0
	$dist SetTargetIndex 0
	$dist Update
	
	puts "Min dist: [ $dist GetMinimum ]"
	puts "Max dist: [ $dist GetMaximum ]"
	puts "Mean dist: [ $dist GetMean ]"
	
	set polydata [ vtkPolyData [ pxvtable::vnewobj ] ]
	$polydata ShallowCopy [ $dist GetOutput ]
	
	$dist Delete
	
	set index [ fiber_addwithname "Error" $polydata ]

	if { $global_list(view_fiber) } {	
	    
	    set actor [ view_create_fiber_glyph $index ]
	    lappend global_list(actor_fiber) $actor
	    
	    ## add actor if to be displayed
	    if { [ lindex $global_list(fiber_disp_list) $index ] == 1 } { 
		[ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] AddActor $actor
	    }
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: threshold validity
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_threshold { } {

    set TENSOR 0
    set tensor [ lindex $global_list(result_list) $TENSOR ]

    set viewer [ $parent GetViewer ]      

    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	## get current index
	set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 
	
	## name
	set item [ lindex $global_list(fiber_list) $sel_index ]	

	puts "number of fibers before=[ [ lindex $global_list(vtk_fiber_polydata) $sel_index ] GetNumberOfCells ]"

	set thresh 0

	set levels [ [ $viewer GetOrthoImageSlice ] GetLevels ]
	set x [ lindex $levels 0 ]
	set y [ lindex $levels 1 ]
	set z [ lindex $levels 2 ]    

	scan [ [ $tensor GetImage ] GetSpacing ] "%f %f %f" spc0 spc1 spc2
	scan [ [ $tensor GetImage ] GetOrigin ] "%f %f %f" org0 org1 org2

	set ox [expr $x * $spc0 + $org0 ]
	set oy [expr $y * $spc1 + $org1 ]
	set oz [expr $z * $spc2 + $org2 ]

	#puts "$ox $oy $oz"
    
#	if { $global_list(fiber_thresh_type) == 0 } {
	    set thresh [ vtkmpjThresholdPolyData [ pxvtable::vnewobj ] ]
	    $this SetFilterCallbacks $thresh "Filtering..."
	    $thresh SetInput [ lindex $global_list(vtk_fiber_polydata) $sel_index ]
	    $thresh ThresholdByUpper $global_list(fiber_thresh_val)
	    $thresh SetAttributeModeToUseCellData
	    $thresh Update
#	} else {
#	    set thresh [ vtkmpjThresholdPolyDataByDistance [ pxvtable::vnewobj ] ]
#	    $this SetFilterCallbacks $thresh "Filtering..."
#	    $thresh SetInput [ lindex $global_list(vtk_fiber_polydata) $sel_index ]
#	    $thresh SetMinimumDistance $global_list(fiber_thresh_dist)
#	    $thresh SetOrigin $ox $oy $oz
#	    $thresh Update
#	}

	## replace fiber
	fiber_removeselected
	
	set new_fiber [ vtkPolyData [ pxvtable::vnewobj ] ]
	$new_fiber ShallowCopy [ $thresh GetOutput ]
	
	puts "number of fibers after threshold=[ $new_fiber GetNumberOfCells ]"

	$thresh Delete
	
	set index [ fiber_add $new_fiber ]
	
	## display new bundle
	if { $global_list(view_fiber) } {	
	    
	    set actor [ view_create_fiber_glyph $index ]
	    lappend global_list(actor_fiber) $actor
	    
	    ## add actor if to be displayed
	    if { [ lindex $global_list(fiber_disp_list) $index ] == 1 } { 
		for { set j 0 } { $j < 4 } { incr j } {
		    [ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] AddActor $actor
		}
	    }

	    [ $parent GetViewer ] UpdateDisplay	    
	}		
    }
    
}


#-------------------------------------------------------------------------------------------
#  PRIVATE: compute fiber statistics
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_statistics { } {

    set FIBERS 3

    set fiber_image [ lindex $global_list(result_list) $FIBERS ] 

    $widget_list(notebook) view "Statistics"
    
    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	## get current index
	set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 
	
	## name
	set item [ lindex $global_list(fiber_list) $sel_index ]	
	
	# compute fiber statistics
	set fstat [ vtkmpjVectorStreamlineStatistics [ pxvtable::vnewobj ] ]
	$fstat SetInput [ lindex $global_list(vtk_fiber_polydata) $sel_index ]
	$fstat Update
	
	set vol "n/a"
	set volmm "n/a"

	#if { [ $fiber_image GetImageSize ] > 1 } {
	    
	    ## quantize fibers
	    #set quant [ vtkmpjStreamlineQuantization [ pxvtable::vnewobj ] ]
	    #$quant SetInput [ lindex $global_list(vtk_fiber_polydata) $sel_index ]
	    #$quant SetImage [ $fiber_image GetImage ]
	    #$quant SetMaskValue 1.0
	    #$quant Update
	    
	    #$fiber_image ShallowCopyImage [ $quant GetOutput ]

	    ## get volume 
	    #set vol [ $quant GetNumberOfVoxels ]

	    ## get spacing information
	    #scan [ [ $fiber_image GetImage ] GetSpacing ] "%f %f %f" spc0 spc1 spc2

	    ## calculate volume in mm
	    #set volmm [ expr $vol * $spc0 * $spc1 * $spc2 ]
	    #set volmm [ format %6.2f $volmm ]

	    #$quant Delete
	#}
	
	## Write out stats
	set textlist [  $widget_list(stat_text) component text ]
	$textlist tag configure even -background "\#e0e0ff"
	
	set strlen [ string length $item ]
	if { $strlen > 23 } {
	    set item [ string range $item 0 22 ]
	} else {
	    set spc [ string repeat " " [ expr (23 - $strlen) / 2 ] ] 
	    set item [ string range "$spc$item$spc" 0 22 ]
	}

	fiber_beginstatistics $item $global_list(stat_overwrite)

	set min [ $fstat GetMinimum ]
	set max [ $fstat GetMaximum ]
	set mean [ $fstat GetMean ]
	set var [ $fstat GetVariance ]
	set dev [ expr sqrt($var) ]

	set result [ list $min $max $mean $var $dev ]
	fiber_addstatistics "Validity index" $result odd

	$widget_list(stat_text) insert \
	    end "-----------------------+------------+------------+------------+------------+------------\n" even

	set num [ $fstat GetNumberOfFibers ]

	$widget_list(stat_text) insert end " Fiber Count = $num, Volume = $vol ($volmm mm^3)\n" odd

	$fstat Delete
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: quantize fibers
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_quantize { } {

    set FIBERS 3

    set fiber_image [ lindex $global_list(result_list) $FIBERS ] 

    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	## get current index
	set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 
	
	## name
	set item [ lindex $global_list(fiber_list) $sel_index ]	

	## quantize fibers
	set quant [ vtkmpjStreamlineQuantization [ pxvtable::vnewobj ] ]
	$quant SetInput [ lindex $global_list(vtk_fiber_polydata) $sel_index ]
	$quant SetImage [ $fiber_image GetImage ]
	$quant SetMaskValue 1.0
	$quant Update

	$fiber_image ShallowCopyImage [ $quant GetOutput ]

	$quant Delete
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Front propagation callback function
# -----------------------------------------------------------------------------------------
itcl::body mpjconnectivity::PropagationCallback { filter comment } {
    
    set iter [ $filter GetIterationNumber ]
    
#   ShowProgress $filter "$comment (Iteration $iter)"
    
    update idletasks

    if { $comment != "Done" } {
	set label [ format "%8.4f" [ $filter GetErrorValue ] ]
	set global_list(solution_errorlabel) "Error: $label"
    }

    update idletasks
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Front propagation
# -----------------------------------------------------------------------------------------
itcl::body mpjconnectivity::result_propagate { }  {
    
    set VTK_MPJ_LARGE_FLOAT 32767
    set VTK_MPJ_UNKNOWN     0 

    set TENSOR 0
    set MASK 1
    set MAP 2
    set SOLUTION 3
    set CONVERGENCE 4
    set FIBERS 5
    set GRADIENT 6
    set CHARACTER 7
    set DOT 8
    set AUX 9
        
    WatchOn

    update idletasks

    set viewer [ $parent GetViewer ]      
    
    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set solution [ lindex $global_list(result_list) $SOLUTION ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set converge [ lindex $global_list(result_list) $CONVERGENCE ]
    set gradient [ lindex $global_list(result_list) $GRADIENT ]
    set character [ lindex $global_list(result_list) $CHARACTER ]
    set dot [ lindex $global_list(result_list) $DOT ]
    set aux [ lindex $global_list(result_list) $AUX ]

    puts "size: [ [$tensor GetImage ] GetDimensions ]"
    set levels [ [ $viewer GetOrthoImageSlice ] GetLevels ]
    set x [ lindex $levels 0 ]
    set y [ lindex $levels 1 ]
    set z [ lindex $levels 2 ]    
    puts "$x $y $z"
    
    ## set seed point
    set seeds [ vtkFloatArray [ pxvtable::vnewobj ] ]
    $seeds SetNumberOfComponents 4
    $seeds InsertNextTuple4 $x $y $z 0

    set sol_method 0
    
    if { $global_list(solution_method) == 0 } { 
	set sol_method [ vtkmpjLaxConnectivity [ pxvtable::vnewobj ] ]
    } else {
	set sol_method [ vtkmpjFastMarchingConnectivity [ pxvtable::vnewobj ] ]
    }

    ## if mask is defined, make region outside mask unreachable
    if { [ $mask GetImageSize ] > 1 } {

	if { $global_list(solution_method) == 0 } { 

	    ## LFS
	    set cast [ vtkImageCast [ pxvtable::vnewobj ] ]
	    $cast SetInput [ $mask GetImage ]
	    $cast SetOutputScalarTypeToFloat
	    $cast Update
	    
	    ## create label image
	    set thresh [ vtkImageThreshold [ pxvtable::vnewobj ] ]
	    $thresh SetInput [ $cast GetOutput ]
	    $thresh ThresholdByUpper 0.5
	    $thresh SetInValue $VTK_MPJ_UNKNOWN
	    $thresh SetOutValue 100
	    $thresh SetOutputScalarTypeToUnsignedChar
	    $thresh Update
	    
	    $cast Delete
	    
	    ## set initial label
	    $sol_method SetLabelImage [ $thresh GetOutput ]
	    
	    $thresh Delete
	    
	    ##$converge ShallowCopyImage [ $lax GetLabelImage ]
	    ##$solution configure -orientation [ $solution cget -orientation ]    
	    ###puts "initial solution"
	} else {
	    
	    ## FMM
	    $sol_method SetMask [ $mask GetImage ]
	}
    } 
        
    ## set reuse flag
    if { $global_list(solution_reuse) } {
	if { $global_list(solution_method) == 0 } {
	    $sol_method SetSolution [ $solution GetImage ]
	}
    }
    
    ## set seedss
    $sol_method SetKnownPoints $seeds
    $sol_method SetInput [ $tensor GetImage ]
    $sol_method SetSymmetricTensor $global_list(tensor_symm)
    $sol_method SetSpacing $global_list(solution_spacing) \
	$global_list(solution_spacing) $global_list(solution_spacing)
    $sol_method SetInfinity $global_list(solution_inf)
    
    ## set input parameters
    if { $global_list(solution_method) == 0 } {
	
	## Lax-Friedrichs sweeping
	$sol_method SetMaximumError $global_list(solution_maxerror)
	$sol_method SetStopOnMaximumError $global_list(solution_error)
	$sol_method SetNumberOfIterations [ expr int($global_list(solution_maxiter)) ]
	$this SetFilterCallbacks $sol_method "Propagating..."
	#	$sol_method SetProgressMethod [ itcl::code $this PropagationCallback $sol_method "Propagating..." ]
	#	$sol_method SetEndMethod      [ itcl::code $this PropagationCallback $sol_method "Done" ]

    } else {
	## FMM
	$this SetFilterCallbacks $sol_method "Propagating..."
    }
    
    ## execute
    $sol_method Update
    
    $solution ShallowCopyImage [ $sol_method GetOutput ]  
##  $solution configure -orientation [ $solution cget -orientation ]
    
    ## calculate gradient
    #set imgrad [ vtkImageGradient [ pxvtable::vnewobj ] ]
    set imgrad [ vtkmpjLevelSetNormal [ pxvtable::vnewobj ] ]
    $imgrad SetInput [ $sol_method GetOutput ]
    #    $imgrad SetDimensionality 3
    $this SetFilterCallbacks $imgrad "Calculating gradient..."
    $imgrad Update
    
    $gradient ShallowCopyImage [ $imgrad GetOutput ]  
    #$gradient ShallowCopyImage [ $lax GetGradient ]  
##  $gradient configure -orientation [ $solution cget -orientation ]
    
    ## calculate characteristics
    set imchar [ vtkmpjImageCharacteristics [ pxvtable::vnewobj ] ]
    $imchar SetInput [ $tensor GetImage ]
    $imchar SetGradient [ $imgrad GetOutput ]
    #$imchar SetGradient [ $lax GetGradient ]
    $imchar SetCoefficientA $global_list(solution_a)
    $imchar SetCoefficientB $global_list(solution_b) 
    $imchar SetCoefficientC $global_list(solution_c)
    $this SetFilterCallbacks $imchar "Calculating characteristics..."
    $imchar Update
    
    $character ShallowCopyImage [ $imchar GetOutput ]  
##  $character configure -orientation [ $solution cget -orientation ] 
	
    $imgrad Delete
    $imchar Delete

    ## get eigenvalues
    set eigen [ vtkmpjImageTensorEigenAnalysis [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $eigen "Computing eigenvalues and eigenvectors..."
    $eigen SetInput [ $tensor GetImage ]
    $eigen SetMask [ $mask GetImage ]
    $eigen SetOutputTypeToEigenvectors
    $eigen Update
    
    set imdot [ vtkmpjImageDotProduct [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $imdot "Computing angle..."
    $imdot SetInput1 [ $eigen GetInput ]
    $imdot SetInput2 [ $character GetImage ]
    $imdot Update
    
    $dot ShallowCopyImage [ $imdot GetOutput ]  
##  $dot configure -orientation [ $solution cget -orientation ] 

    $imdot Delete

    set imdot [ vtkmpjImageDotProduct [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $imdot "Computing angle2..."
    $imdot SetInput1 [ $eigen GetInput ]
    $imdot SetInput2 [ $gradient GetImage ]
    $imdot Update

    $eigen Delete
    
    $converge ShallowCopyImage [ $imdot GetOutput ]  
##  $converge configure -orientation [ $solution cget -orientation ] 

    $imdot Delete

    set imdot [ vtkmpjImageDotProduct [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $imdot "Computing angle between characteristics and gradient..."
    $imdot SetInput1 [ $gradient GetImage ]
    $imdot SetInput2 [ $character GetImage ]
    $imdot Update

    $aux ShallowCopyImage [ $imdot GetOutput ]  
##  $aux configure -orientation [ $solution cget -orientation ] 

    $imdot Delete

    #$converge ShallowCopyImage [ $lax GetLabelImage ]
    #$solution configure -orientation [ $solution cget -orientation ]    
    
    $sol_method Delete
    $seeds Delete    

    # send currently selected result to parent
    $widget_list(result_list) selection set $SOLUTION $SOLUTION
    
    set sol_map [ lindex $global_list(result_list) $SOLUTION ]
    $parent SetImageFromObject [ $sol_map  GetThisPointer ] $this
    
    WatchOff
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: fiber track
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::fiber_track { } {

    set TENSOR 0
    set MASK 1
    set MAP 2
    set SOLUTION 3
    set CONVERGENCE 4
    set FIBERS 5
    set GRADIENT 6
    set CHARACTER 7
    set DOT 8
    set AUX 9
    
    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set map [ lindex $global_list(result_list) $MAP ]
    set solution [ lindex $global_list(result_list) $SOLUTION ]
    set converge [ lindex $global_list(result_list) $CONVERGENCE ]
    set fibers [ lindex $global_list(result_list) $FIBERS ] 
    set gradient [ lindex $global_list(result_list) $GRADIENT ] 
    set character [ lindex $global_list(result_list) $CHARACTER ] 
    set dot [ lindex $global_list(result_list) $DOT ] 
    set aux [ lindex $global_list(result_list) $AUX ]

    if { [ $solution GetImageSize ] == 1 } {
	
	tk_messageBox -type ok \
	    -parent $basewidget -title "Solution" \
	    -message "You need a propagation solution to start tracking!" -icon info
	
	return
    }
     
    if { [ $map GetImageSize ] == 1 } {
	
## create dummy map
    $map CopyImageHeader [ $fibers GetImageHeader ]
    $map ShallowCopyImage [ $fibers GetImage ]
    }

    ## if tracking, one wants to see the fibers
    set global_list(view_fiber) 1

    WatchOn

    ## get slice numbers
    # Viewer Cross Hairs
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set cx [ lindex $levels 0 ]
    set cy [ lindex $levels 1 ]
    set cz [ lindex $levels 2 ]

    ## get origin and spacing information
    scan [ [ $solution GetImage ] GetSpacing ] "%f %f %f" spc0 spc1 spc2
    scan [ [ $solution GetImage ] GetOrigin ] "%f %f %f" org0 org1 org2

    set imtovec [ vtkmpjImageToVectors [ pxvtable::vnewobj ] ]

    if { $global_list(track_map) == 0 } {
	## convert into vector field
	$imtovec SetInput [ $gradient GetImage ]
	$imtovec Update
    } else {
	$imtovec SetInput [ $character GetImage ]
	$imtovec Update
    }
    
    set merge [ vtkMergeFilter [ pxvtable::vnewobj ] ]
    $merge SetGeometry [ $dot GetImage ]
    $merge SetVectors [ $imtovec GetOutput ]
    if { $global_list(track_map) == 0 } {
	$merge SetScalars [ $converge GetImage ]
    } else {
	$merge SetScalars [ $dot GetImage ]
    }
    $merge Update
    
#    $imgrad Delete
    $imtovec Delete

    ## initialize streamer ##
    set streamer [ vtkmpjVectorStreamline [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $streamer "Fiber tracking..."
    $streamer SetInput [ $merge GetOutput ]
    
    if { $global_list(track_seedtype) == 0 } {

	## single input point
	set ox [expr $cx * $spc0 + $org0 ]
	set oy [expr $cy * $spc1 + $org1 ]
	set oz [expr $cz * $spc2 + $org2 ]
	
	set source [ vtkPolyData [ pxvtable::vnewobj ] ] 
	set points [ vtkPoints [ pxvtable::vnewobj ] ]
	
	$points InsertNextPoint $ox $oy $oz 
	$source SetPoints $points
	
	$points Delete
	
	## assign source point
	$streamer SetSource $source

	$source Delete

    } elseif { $global_list(track_seedtype) == 1 } {
	
	## region 
	set region $global_list(track_seedregion)
	
	set imthr [ vtkImageThreshold [ pxvtable::vnewobj ] ]
	$imthr SetInput [ $mask GetImage ]
	$imthr ThresholdBetween $region $region
	$imthr SetInValue 1.0
	$imthr SetOutValue 0.0
	$imthr SetOutputScalarTypeToUnsignedChar
	$imthr Update
	
 	set dens $global_list(track_seeddens)

    set source [ vtkmpjImageROISource [ pxvtable::vnewobj ] ]
    $source SetMask [ $imthr GetOutput ]
    $source SetSourceToMask
    $source SetOrigin $org0 $org1 $org2
    $source SetSpacing $spc0 $spc1 $spc2
    $source SetDensity $dens
    $source Update
	
 	## assign source points
	$streamer SetSource [ $source GetOutput ]
	$source Delete
	
	#	set impts [ vtkThresholdPoints [ pxvtable::vnewobj ] ]
	#	$impts SetInput [ $imthr GetOutput ]
	#	$impts ThresholdByUpper 1.0
	#	$impts Update    
	
	$imthr Delete
	
	## assign region source points
#	$streamer SetSource [ $impts GetOutput ]
#	$impts Delete
	
    } elseif { $global_list(track_seedtype) == 2 } {
	
	## volume	
	set ox [expr $cx * $spc0 + $org0 ]
	set oy [expr $cy * $spc1 + $org1 ]
	set oz [expr $cz * $spc2 + $org2 ]
	
	set width $global_list(track_seedwidth)
	set height $global_list(track_seedheight)
	set depth $global_list(track_seeddepth)
	set dens $global_list(track_seeddens)
	
	#	set numx [ expr int(1.0 / $res) * $width ]
	#	set numy [ expr int(1.0 / $res) * $height ]
	#	set numz [ expr int(1.0 / $res) * $depth ]
	
	set source [ vtkmpjImagePointSource [ pxvtable::vnewobj ] ]
	$source SetCenter $ox $oy $oz
	$source SetSize $width $height $depth
	$source SetOrigin $org0 $org1 $org2
	$source SetSpacing $spc0 $spc1 $spc2
	$source SetDensity $dens
	$source Update

	## assign source points
	$streamer SetSource [ $source GetOutput ]

	$source Delete
    }
    
    puts "num seed points: [ [ [ $streamer GetSource ] GetPoints ] GetNumberOfPoints ]"
    puts "min speed = $global_list(track_minspeed)"
    
    $streamer SetTerminalSpeed $global_list(track_minspeed)
    $streamer SetIntegrationDirectionToBackward
    $streamer SetMaximumPropagationTime $global_list(track_maxdist)
    if { $global_list(track_integration) == 0 } { 
	$streamer SetIntegrator [ vtkRungeKutta2 [ pxvtable::vnewobj ] ]
    } else {
	$streamer SetIntegrator [ vtkRungeKutta4 [ pxvtable::vnewobj ] ]
    }
    $streamer SetIntegrationStepLength $global_list(track_steplen)
    $streamer SetStepLength $global_list(track_steplen)
    #$streamer SpeedScalarsOn
    $streamer Update

    set new_fiber [ vtkPolyData [ pxvtable::vnewobj ] ]
    
    ## create new fiber set
    if { $global_list(track_fitspline) == 1 } {
	
	set spline [ vtkSplineFilter [ pxvtable::vnewobj ] ]
	$this SetFilterCallbacks $spline "Spline fitting..."
	$spline SetInput [ $streamer GetOutput ]
	$spline SetSubdivide $global_list(track_splinesubdivision)
	$spline SetLength $global_list(track_splinelength)
	$spline SetNumberOfSubdivisions $global_list(track_splinenumber)
	$spline Update

	$new_fiber ShallowCopy [ $spline GetOutput ]

	$spline Delete

    } else {	
	
	$new_fiber ShallowCopy [ $streamer GetOutput ]
    }
    
    $streamer Delete
    
    set index [ fiber_add $new_fiber ]
    
#    puts "points: [[$new_fiber GetPoints] GetNumberOfPoints]"

    ## display new bundle
    if { $global_list(view_fiber) } {	
	
	set actor [ view_create_fiber_glyph $index ]
	lappend global_list(actor_fiber) $actor
	
	## add actor if to be displayed
	if { [ lindex $global_list(fiber_disp_list) $index ] == 1 } { 
	    for { set j 0 } { $j < 4 } { incr j } {
		[ [ [ $parent GetViewer ] GetRenderer $j ] GetRenderer ] AddActor $actor
	    }
	}
    }

    puts "before statistics"

    ## compute statistics
    #if { $global_list(track_autostat) } {
	#fiber_statistics
    #}

    WatchOff
}

#-------------------------------------------------------------------------------------------
#  Create results control
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::CreateInputControl { base } {   

    iwidgets::labeledframe $base.frame1 \
	-labelpos nw  \
	-labeltext "Tensor image"
    pack $base.frame1 -fill both -expand f -pady 5
    
    set frame1 [ $base.frame1 childsite ]
    
    set subframe0 [ frame $frame1.subframe0 ]
    pack $subframe0 -side top -fill both -expand f -pady 0
    
    set tensor_fname [ iwidgets::entryfield $subframe0.tensor_fname \
			   -textvariable [ itcl::scope global_list(tensor_fname) ] \
			   -labeltext "Filename:" ] 
    pack $tensor_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(tensor_fname) $tensor_fname
    
    set tensor_load [ iwidgets::buttonbox $subframe0.tensor_load -orient horizontal -padx 2 ]
    $tensor_load add load -text "Load..." -command [ itcl::code $this loadtensor ] 
    $tensor_load add disp -text "Display" -command [ itcl::code $this DisplayResult 0 ]
    pack $tensor_load -side left
    set widget_list(tensor_load) $tensor_load
    
    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Region of interest mask"
    pack $base.frame0 -fill both -expand f -pady 5
    
    set frame0 [ $base.frame0 childsite ]
  
    iwidgets::labeledframe $base.frame2 \
	-labelpos nw  \
	-labeltext "Map for analysis"
    pack $base.frame2 -fill both -expand f -pady 5
    
    set frame2 [ $base.frame2 childsite ]
    
    set subframe0 [ frame $frame2.subframe0 ]
    pack $subframe0 -side top -fill x -expand f -pady 0
    
    set map_fname [ iwidgets::entryfield $subframe0.map_fname \
			-textvariable [ itcl::scope global_list(map_fname) ] \
			-labeltext "Filename:" ] 
    pack $map_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(map_fname) $map_fname
    
    set map_load [ iwidgets::buttonbox $subframe0.map_load -orient horizontal -padx 2 ]
    $map_load add load -text "Load..." -command [ itcl::code $this loadmap ] 
    $map_load add disp -text "Display" -command [ itcl::code $this DisplayResult 2 ]
    pack $map_load -side left 
    set widget_list(map_load) $map_load

    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 0
    
    set mask_fname [ iwidgets::entryfield $subframe1.mask_fname \
			 -textvariable [ itcl::scope global_list(mask_fname) ] \
			 -labeltext "Filename:" ] 
    pack $mask_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(mask_fname) $mask_fname
    
    set mask_load [ iwidgets::buttonbox $subframe1.mask_load -orient horizontal -padx 2 ]
    $mask_load add load -text "Load..." -command [ itcl::code $this loadmask ]    
    $mask_load add disp -text "Display" -command [ itcl::code $this DisplayResult 1 ]
    pack $mask_load -side left 
    set widget_list(mask_load) $mask_load
    
    set mask_nreg [ iwidgets::spinint $subframe1.mask_nreg \
			-labeltext "Number of regions in mask:" \
			-fixed 4 -width 4 -range {1 1000} -step 1 \
			-textvariable [ itcl::scope global_list(mask_nreg) ] ]
    pack $mask_nreg -side right -pady 5 -padx 5
    set widget_list(mask_nreg) $mask_nreg

  
}

#-------------------------------------------------------------------------------------------
#  Create results control
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::CreateResultsControl { base } {   

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
#  Create tensor control
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::CreateConnectivityControl { base } {
    
    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Solution"
    pack $base.frame0 -fill both -expand f -pady 5
    
    set frame0 [ $base.frame0 childsite ]
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 0
  
    set solution_fname [ iwidgets::entryfield $subframe1.mask_fname \
			     -state disabled \
			     -textvariable [ itcl::scope global_list(solution_fname) ] \
			     -labeltext "Filename:" ] 
    pack $solution_fname -side top -expand t -fill x -padx 5 -pady 5
    set widget_list(solution_fname) $solution_fname
    
    set solution_load [ iwidgets::buttonbox $subframe1.solution_load -orient horizontal -padx 2 ]
    $solution_load add load -text "Load..." -command [ itcl::code $this loadsolution ] 
    $solution_load add disp -text "Display" -command [ itcl::code $this DisplayResult 3 ]
    pack $solution_load -side left
    set widget_list(solution_load) $solution_load
    
    iwidgets::labeledframe $base.frame1 \
	-labelpos nw  \
	-labeltext "Front propagation"
    pack $base.frame1 -fill both -expand f 
    
    set frame0 [ $base.frame1 childsite ]

    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -side top -fill both -expand f -pady 5

    set solution_reuse [ checkbutton $subframe0.tractreuse \
			    -text "Reuse solution" \
			    -variable [ itcl::scope global_list(solution_reuse) ]]    
    pack $solution_reuse -side left -padx 5  

    set solution_maxiter [ iwidgets::spinint $subframe0.solution_maxiter \
			       -labeltext "Maximum number of iterations:" \
			       -fixed 3 -width 4 -step 1 -range {1 1000} ]
    pack $solution_maxiter -side right -padx 5  
    $solution_maxiter configure -textvariable [ itcl::scope global_list(solution_maxiter) ]
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill both -expand f
    
    set solution_error [ checkbutton $subframe1.solution_error \
			     -text "Stop when error gets below:" \
			     -variable [ itcl::scope global_list(solution_error) ] ]
    pack $solution_error -side left -padx 5
    
    set solution_maxerror [ iwidgets::entryfield $subframe1.solution_maxerror \
				-validate real -width 7  \
				-textvariable [ itcl::scope global_list(solution_maxerror) ] ]
    pack $solution_maxerror -side left -padx 0
    
    set solution_errorlabel [ label $subframe1.solution_errorlabel \
				  -textvariable [ itcl::scope global_list(solution_errorlabel) ] ]
    pack $solution_errorlabel -side right -padx 5
			      		
    set subframe3 [ frame $frame0.subframe3 ]
    pack $subframe3 -side top -fill both -expand f

    set solution_method [ iwidgets::optionmenu $subframe3.solution_method \
			     -command [ itcl::code $this solution_method_cb ] \
			     -labeltext "Method:" ]
    pack $solution_method -side left -padx 5 -pady 5
    $solution_method insert end "Lax-Friedrichs Sweeping"
    $solution_method insert end "Fast Marching Method"
    set widget_list(solution_method) $solution_method

    set solution_spacing [ iwidgets::entryfield $subframe3.solution_spacing \
			       -labeltext "Grid spacing" \
			       -validate real -width 7  \
			       -textvariable [ itcl::scope global_list(solution_spacing) ] ]
    pack $solution_spacing -side right -padx 5 -pady 5

    set subframe4 [ frame $frame0.subframe4 ]
    pack $subframe4 -side top -fill both -expand f

    set solution_inf [ iwidgets::entryfield $subframe4.solution_inf \
			 -labeltext "Value for infinity: " \
			 -validate real -width 7  \
			 -textvariable [ itcl::scope global_list(solution_inf) ] ]
    pack $solution_inf -side left -padx 5 -pady 5
    
    # set solution_a [ iwidgets::entryfield $subframe3.solution_a \
# 			 -labeltext "a: " \
# 			 -validate real -width 7  \
# 			 -textvariable [ itcl::scope global_list(solution_a) ] ]
#     pack $solution_a -side right -padx 0
    
#     set solution_b [ iwidgets::entryfield $subframe3.solution_b \
# 			 -labeltext "b: " \
# 			 -validate real -width 7  \
# 			 -textvariable [ itcl::scope global_list(solution_b) ] ]
#     pack $solution_b -side right -padx 0
    
#     set solution_c [ iwidgets::entryfield $subframe3.solution_c \
# 			 -labeltext "c: " \
# 			 -validate real -width 7  \
# 			 -textvariable [ itcl::scope global_list(solution_c) ] ]
#     pack $solution_c -side right -padx 0

    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -side top -fill both -expand f
    
    set solution_btn [ iwidgets::buttonbox $subframe2.solution_btn -orient horizontal -padx 2 ]
    $solution_btn add compute -text "Compute!" -command [ itcl::code $this result_propagate ] 
    pack $solution_btn -side left

    set widget_list(compute_btn) $solution_btn
}

#-------------------------------------------------------------------------------------------
#  Create fiber tracking control
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::CreateTrackingControl { base } {

    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Seed points"
    pack $base.frame0 -fill both -expand f 
    
    set frame0 [ $base.frame0 childsite ]
    
    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -fill both -expand f
    
    set track_seedtype [ iwidgets::optionmenu $subframe0.track_seedtype \
			     -command [ itcl::code $this track_seedtype_cb ] \
			     -labeltext "Seed from:" ]
    pack $track_seedtype -side left -padx 5 -pady 5
    $track_seedtype insert end "Single point"
    $track_seedtype insert end "Region"
    $track_seedtype insert end "Volume"
    set widget_list(track_seedtype) $track_seedtype

    set track_seedregion [ iwidgets::spinint $subframe0.track_seedregion \
			       -labeltext "Region number:" \
			       -fixed 4 -width 4 -range {1 1000} -step 1 \
			       -textvariable \
			       [ itcl::scope global_list(track_seedregion) ] ]
    pack $track_seedregion -side left -pady 5 -padx 2
    set widget_list(track_seedregion) $track_seedregion  
    $widget_list(track_seedregion) configure -state disabled
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -fill both -expand f
    
    set track_seedwidth [ iwidgets::entryfield $subframe1.track_seedwidth \
			      -labeltext "Width:" \
			      -validate integer \
			      -width 4 \
			      -state disabled \
			      -textvariable [ itcl::scope global_list(track_seedwidth) ] ]
    pack $subframe1.track_seedwidth -side left -padx 5 -pady 5 
    set widget_list(track_seedwidth) $track_seedwidth
    
    set track_seedheight [ iwidgets::entryfield $subframe1.track_seedheight \
			       -labeltext "Height:" \
			       -validate integer \
			       -width 4 \
			       -state disabled \
			       -textvariable [ itcl::scope global_list(track_seedheight) ] ]
    pack $subframe1.track_seedheight -side left -padx 5 -pady 5
    set widget_list(track_seedheight) $track_seedheight
    
    set track_seeddepth [ iwidgets::entryfield $subframe1.track_seeddepth \
			      -labeltext "Depth:" \
			      -validate integer \
			      -width 4 \
			      -state disabled \
			      -textvariable [ itcl::scope global_list(track_seeddepth) ] ]
    pack $subframe1.track_seeddepth -side left -padx 5 -pady 5 
    set widget_list(track_seeddepth) $track_seeddepth
    
    set track_seeddens [ iwidgets::entryfield $subframe1.track_seeddens \
			    -labeltext "Density:" \
			    -validate real \
			    -width 6 \
			    -state disabled \
			    -textvariable [ itcl::scope global_list(track_seeddens) ] ]
    pack $subframe1.track_seeddens -side left -padx 5 -pady 5
    set widget_list(track_seeddens) $track_seeddens

    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -fill both -expand f
    
    set track_map [ iwidgets::optionmenu $subframe2.track_map \
			-command [ itcl::code $this track_map_cb ] \
			-labeltext "Source map:" ]
    pack $track_map -side left -padx 5 -pady 5
    $track_map insert end "Gradient"
    $track_map insert end "Characteristics"
    set widget_list(track_map) $track_map

    iwidgets::labeledframe $base.frame1 \
	-labelpos nw  \
	-labeltext "Integration parameters"
    pack $base.frame1 -fill both -expand f
    
    set frame1 [ $base.frame1 childsite ]
    
    set subframe2 [ frame $frame1.subframe2 ]
    pack $subframe2 -fill both -expand f
    
    set integration [ iwidgets::optionmenu $subframe2.integration \
			  -command [ itcl::code $this track_integration_cb ] \
			  -labeltext "Type:" ]	
    pack $integration -side left -padx 5 -pady 5
    $integration insert end "Runge-Kutta 2"
    $integration insert end "Runge-Kutta 4"
    set widget_list(track_integration) $integration
    
    iwidgets::entryfield $subframe2.track_steplen \
	-width 5 \
	-validate real \
	-labeltext "Step length:" \
	-textvariable [ itcl::scope global_list(track_steplen) ] 
    pack $subframe2.track_steplen -side left -padx 5 -pady 5     


    set subframe5 [ frame $frame1.subframe5 ]
    pack $subframe5 -fill both -expand f

    iwidgets::entryfield $subframe5.track_maxdist \
	-width 5 \
	-validate real \
	-labeltext "Max length:" \
	-textvariable [ itcl::scope global_list(track_maxdist) ] 
    pack $subframe5.track_maxdist -side left -padx 5 -pady 5    

    iwidgets::entryfield $subframe5.track_minspeed \
	-width 5 \
	-validate real \
	-labeltext "Minimum speed:" \
	-textvariable [ itcl::scope global_list(track_minspeed) ] 
    pack $subframe5.track_minspeed -side left -padx 5 -pady 5  

    iwidgets::labeledframe $base.frame2 \
	-labelpos nw  \
	-labeltext "Fiber representation"
    pack $base.frame2 -fill both -expand f
    
    set frame2 [ $base.frame2 childsite ]

    set subframe6 [ frame $frame2.subframe6 ]
    pack $subframe6 -fill both -expand f
    
    set track_fitspline [ checkbutton $subframe6.track_fitspline \
			      -text "Fit splines to fibers" \
			      -variable [ itcl::scope global_list(track_fitspline) ] ]
    pack $track_fitspline -side left -pady 2 -padx 5 
    
    set splinetype [ iwidgets::optionmenu $subframe6.splinetype \
			 -command [ itcl::code $this track_splinetype_cb ] \
			 -labeltext "Type:" ]	
    pack $splinetype -side left -padx 5 -pady 2
    $splinetype insert end "Cardinal"
    $splinetype insert end "Kochaneck"
    set widget_list(track_splinetype) $splinetype
 
    set subframe7 [ frame $frame2.subframe7 ]
    pack $subframe7 -fill both -expand f
    
    set splinesubdivision [ iwidgets::optionmenu $subframe7.splinesubdivision \
				-command [ itcl::code $this track_splinesubdivision_cb ] \
				-labeltext "Subdivision:" ]	
    pack $splinesubdivision -side left -padx 5 -pady 2
    $splinesubdivision insert end "Specified"
    $splinesubdivision insert end "Length"
    set widget_list(track_splinesubdivision) $splinesubdivision

    iwidgets::entryfield $subframe7.track_splinelength \
	-width 5 \
	-validate real \
	-labeltext "Length:" \
	-textvariable [ itcl::scope global_list(track_splinelength) ] 
    pack $subframe7.track_splinelength -side left -pady 5  

    iwidgets::entryfield $subframe7.track_splinenumber \
	-width 4 \
	-validate integer \
	-labeltext "Subdivisions:" \
	-textvariable [ itcl::scope global_list(track_splinenumber) ] 
    pack $subframe7.track_splinenumber -side left -padx 5 -pady 5  

    set trackbtn [ iwidgets::buttonbox $base.trackbtn -orient horizontal ]
    $trackbtn add track -text "Track!" -command [ itcl::code $this fiber_track ]
    pack $trackbtn -side left -expand f

    ## auto stat
    set track_autostat [ checkbutton $base.track_autostat \
			     -text "Auto fiber statistics" \
			     -variable [ itcl::scope global_list(track_autostat) ] ]
    pack $track_autostat -side right -padx 5       
}

#-------------------------------------------------------------------------------------------
#  Create fiber tracking control
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::CreateFiberControl { base } {
    
    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Fibers"
    pack $base.frame0 -fill both
    
    set frame0 [ $base.frame0 childsite ]
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -fill both -expand t -side top

    label $subframe1.dummy0 -text "Color:"
    pack $subframe1.dummy0 -side left -padx 5 -pady 5
        
    set fiber_color [ image create photo -width 15 -height 15 ]
    $fiber_color put $global_list(fiber_color) -to 0 0 15 15
    
    button $subframe1.fiber_color -image $fiber_color \
	-command [ itcl::code $this fiber_color_cb ]
    pack $subframe1.fiber_color -side left
    set widget_list(fiber_color) $subframe1.fiber_color

    set fiber_name [ iwidgets::entryfield $subframe1.fiber_name \
			 -textvariable [ itcl::scope global_list(fiber_name) ] \
			 -labeltext "Name:" \
			 -width 30 \
			 -command [ itcl::code $this fiber_name_cb ] ] 
    pack $fiber_name -side left -pady 5 -padx 5 -fill x -expand t
    set widget_list(fiber_name) $fiber_name
    
    checkbutton $subframe1.fiber_display \
	-text "Display" \
	-variable [ itcl::scope global_list(fiber_display) ] \
	-command [ itcl::code $this fiber_display_cb ]    
    pack $subframe1.fiber_display -side left -padx 5

    set middle [ frame $frame0.middle ] 
    pack $middle -fill x -expand t -side top -pady 2
    
    set subframe2 [ frame $middle.subframe2 ]
    pack $subframe2 -fill both -expand t -side left -pady 2

    iwidgets::scrolledlistbox $subframe2.fiberlist \
	-hscrollmode dynamic \
	-visibleitems 40x15 \
	-listvariable [ itcl::scope global_list(fiber_list) ] \
	-selectmode extended \
	-selectioncommand [ itcl::code $this fiber_selection_cb ]    
    pack $subframe2.fiberlist -side left -fill both -expand t -padx 5
    set widget_list(fiber_list) $subframe2.fiberlist
   
    set subframe3 [ frame $middle.subframe3 ]
    pack $subframe3 -expand f -side left
    
    iwidgets::entryfield $subframe3.fiber_thresh_val \
	-width 4 \
	-validate real \
	-labeltext "Value:" \
	-textvariable [ itcl::scope global_list(fiber_thresh_val) ]
    pack $subframe3.fiber_thresh_val -side bottom -padx 0   

    iwidgets::entryfield $subframe3.fiber_thresh_dist \
	-width 4 \
	-validate real \
	-labeltext "Dist:" \
	-textvariable [ itcl::scope global_list(fiber_thresh_dist) ]
    pack $subframe3.fiber_thresh_dist -side bottom -padx 0   

    set fiberbbox [ iwidgets::buttonbox $subframe3.fiberbbox -orient vertical ]
    $fiberbbox add load -text "Load" -command [ itcl::code $this fiber_load ]
    $fiberbbox add save -text "Save" -command [ itcl::code $this fiber_saveselected ]
    $fiberbbox add saveall -text "Save all" -command [ itcl::code $this fiber_saveall ]
    $fiberbbox add remove -text "Remove" -command [ itcl::code $this fiber_removeselected ]
    $fiberbbox add clear -text "Clear" -command [ itcl::code $this fiber_clear ]
    $fiberbbox add stat -text "Statistics" -command [ itcl::code $this fiber_statistics ]
    $fiberbbox add err -text "Error" -command [ itcl::code $this fiber_error ]
    $fiberbbox add thresh -text "Threshold" -command [ itcl::code $this fiber_threshold ]
    pack $fiberbbox -side left -expand t
    set widget_list(fiber_buttonbox) $fiberbbox                   

}

#-------------------------------------------------------------------------------------------
#  Create statistics control
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::CreateStatisticsControl { base } {

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
		       -hscrollmode static \
		       -wrap none ]
    pack $stat_text -side top -expand t -fill both -padx 2 -pady 2
    set widget_list(stat_text) $stat_text

    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -fill both -expand t

    set statbbox [ iwidgets::buttonbox $subframe1.statbbox -orient horizontal -pady 2 -padx 2 ]
    $statbbox add save -text "Save..." -command [ itcl::code $this fiber_savestatistics ]
    $statbbox add clear -text "Clear" -command "$stat_text delete 1.0 end"
    pack $statbbox -side left -fill both 

    set stat_overwrite [ checkbutton $subframe1.stat_overwrite -text "Overwrite" \
			     -variable [ itcl::scope global_list(stat_overwrite) ] ]
    pack $stat_overwrite -side right -padx 5       
}

#-------------------------------------------------------------------------------------------
#  Create wavefront display control
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::CreateFrontDisplayControl { base } {

    set subframe0 [ frame $base.subframe0 ]
    pack $subframe0 -fill x -side top -expand f -padx 5 -pady 5
    
    checkbutton $subframe0.view_gradient -text "View gradient" \
	-variable [ itcl::scope global_list(view_gradient) ] \
	-command [ itcl::code $this view_toggle_gradient ]   
    pack $subframe0.view_gradient -side left -padx 5
    
    checkbutton $subframe0.view_character -text "View characteristic" \
	-variable [ itcl::scope global_list(view_character) ] \
	-command [ itcl::code $this view_toggle_character ]   
    pack $subframe0.view_character -side left -padx 5     
    
    set subframe1 [ frame $base.subframe1 ]
    pack $subframe1 -fill x -side top -expand f -padx 5 -pady 5
    
    iwidgets::entryfield $subframe1.view_contour_levels \
	-width 4 \
	-validate integer \
	-labeltext "Number of levels:" \
	-textvariable [ itcl::scope global_list(view_contour_levels) ] \
	-command [ itcl::code $this view_update_display_cb ]
    pack $subframe1.view_contour_levels -side left -padx 5   
    
    iwidgets::entryfield $subframe1.view_contour_width \
	-width 4 \
	-validate real \
	-labeltext "Contour width:" \
	-textvariable [ itcl::scope global_list(view_contour_width) ]
    pack $subframe1.view_contour_width -side left -padx 5   
    
    set subframe2 [ frame $base.subframe2 ]
    pack $subframe2 -fill x -side top -expand f -padx 5 -pady 5

    set front_luk [ mpjguilookuptable \#auto 0 ]
    $front_luk Initialize $subframe2  
    pack forget [ $front_luk GetButtonBoxWidget ]
    pack forget [ $front_luk GetMenuBar ]     
    $front_luk SetHueRange 0.5 0.92
    $front_luk SetSaturationRange 0.75 1.0
    $front_luk Update
    $front_luk Show
    
    set widget_list(view_frontluk) $front_luk    
}


#-------------------------------------------------------------------------------------------
#  Create fiber display control
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::CreateFiberDisplayControl { base } {

    set subframe0 [ frame $base.subframe0 ]
    pack $subframe0 -fill x -side top -expand f -padx 5 -pady 5
    
#    set view_fiber_filter \
#	[ iwidgets::optionmenu $subframe0.view_fiber_filter -labeltext "Filter:" ]
    
#    $view_fiber_filter configure -command [ itcl::code $this view_fiber_filter_cb ]
#    $view_fiber_filter insert end "Validity index"
    
#    pack $view_fiber_filter -side left -padx 5
#    set widget_list(view_fiber_filter) $view_fiber_filter
    
    set view_fiber_glyph [ iwidgets::optionmenu $subframe0.view_fiber_glyph \
				-labeltext "View fibers as:"   \
				-command [ itcl::code $this view_fiber_glyph_cb ] ] \
	
    pack $view_fiber_glyph -side left -padx 5
    $view_fiber_glyph insert end "Lines"
    $view_fiber_glyph insert end "Tubes"
    $view_fiber_glyph insert end "Points"
    set widget_list(view_fiber_glyph) $view_fiber_glyph
    
    iwidgets::entryfield $subframe0.view_fiber_tuberadius \
	-width 5 \
	-validate real \
	-labeltext "Tube radius:" \
	-textvariable [ itcl::scope global_list(view_fiber_tuberadius) ]  \
	-command [ itcl::code $this view_fiber_tuberadius_cb ]
    pack $subframe0.view_fiber_tuberadius -side left -padx 5    

    iwidgets::entryfield $subframe0.view_fiber_pointsize \
	-width 5 \
	-validate real \
	-labeltext "Point size:" \
	-textvariable [ itcl::scope global_list(view_fiber_pointsize) ]  \
	-command [ itcl::code $this view_fiber_pointsize_cb ]
    pack $subframe0.view_fiber_pointsize -side left -padx 5    
    
    set subframe2 [ frame $base.subframe2 ]
    pack $subframe2 -side top -fill both -padx 5 -expand f
    
    set view_fiber_luk [ checkbutton $subframe2.view_fiber_luk \
			     -text "Apply colormap" \
			     -variable [ itcl::scope global_list(view_fiber_luk) ] \
			     -command [ itcl::code $this view_fiber_colormap_cb ] ]
    pack $view_fiber_luk -side left -pady 5 -padx 5       
    set widget_list(view_fiber_luk) $view_fiber_luk
    
    set view_fiber_luktype \
	[ iwidgets::optionmenu $subframe2.view_fiber_luktype -labeltext "to:" ]    
    $view_fiber_luktype configure -command [ itcl::code $this view_fiber_luktype_cb ]
    $view_fiber_luktype insert end "Fiber points"
    $view_fiber_luktype insert end "Fiber pathways"
    pack $view_fiber_luktype -side left -pady 5 -padx 0       
    set widget_list(view_fiber_luktype) $view_fiber_luktype

    set subframe3 [ frame $base.subframe3 ]
    pack $subframe3 -side left -fill both -padx 5 
    
    set fiber_luk [ mpjguilookuptable \#auto 0 ]
    $fiber_luk Initialize $subframe3  
    pack forget [ $fiber_luk GetButtonBoxWidget ]
    pack forget [ $fiber_luk GetMenuBar ]     
    #    [ $fiber_luk GetLookupTable ] SetVectorModeToComponent
    #    [ $fiber_luk GetLookupTable ] SetVectorComponent 0
    $fiber_luk SetHueRange 0.5 0.92
    $fiber_luk SetSaturationRange 0.75 1.0
    $fiber_luk Update
    $fiber_luk Show
    
    set widget_list(view_fiberluk) $fiber_luk
}

#-------------------------------------------------------------------------------------------
#  Create display control
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::CreateDisplayControl { base } {

    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Properties"
    pack $base.frame0 -fill both -expand t 

    set frame0 [ $base.frame0 childsite ]
    
    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -fill both -padx 5 -pady 2 -expand f

    checkbutton $subframe0.view_fibers -text "View fibers" \
	-variable [ itcl::scope global_list(view_fiber) ] \
	-command [ itcl::code $this view_toggle_fiber ]   
    pack $subframe0.view_fibers -side left -padx 5
    
    checkbutton $subframe0.view_contour -text "View front" \
	-variable [ itcl::scope global_list(view_contour) ] \
	-command [ itcl::code $this view_toggle_contour ]   
    pack $subframe0.view_contour -side left -padx 5

    checkbutton $subframe0.view_colorbar -text "View colorbar" \
	-variable [ itcl::scope global_list(view_colorbar) ] \
	-command [ itcl::code $this view_toggle_colorbar ]   
    pack $subframe0.view_colorbar -side left -padx 5
    
    set subframe1 [ frame $frame0.subframe1 ] 
    pack $subframe1 -fill both -expand t -padx 5 -pady 5
    
    set display_notebook [ iwidgets::tabnotebook $subframe1.display_notebook -tabpos n ]     
    set widget_list(display_notebook) $display_notebook
    pack $display_notebook -fill both -expand t -padx 10 -pady 5
    
    CreateFrontDisplayControl [ $display_notebook add -label "Wavefront" ]
    CreateFiberDisplayControl [ $display_notebook add -label "Fibers" ]

    $display_notebook view "Fibers"   
}

# -------------------------------------------------------------------------------------------
#  Diffusion control inititialization
# ------------------------------------------------------------------------------------------
itcl::body mpjconnectivity::Initialize { widget } {
    
    if { $initialized == 1 } { return $basewidget }
    
    #  -------------------------------------
    #  Create User Interface
    #  -------------------------------------	
    set basewidget [toplevel $widget ]
    wm geometry $basewidget 620x520
    wm withdraw $basewidget

    set notebook $basewidget.notebook    
    iwidgets::tabnotebook $notebook -tabpos w         
    set widget_list(notebook) $notebook
    
    CreateInputControl      [ $notebook add -label "Input" ] 
    CreateConnectivityControl [ $notebook add -label "Connectivity" ]
    CreateTrackingControl   [ $notebook add -label "Tracking" ]
    CreateFiberControl      [ $notebook add -label "Fibers" ]
    CreateStatisticsControl [ $notebook add -label "Statistics" ]
    CreateResultsControl    [ $notebook add -label "Results" ]
    CreateDisplayControl    [ $notebook add -label "Display" ]
    
    pack $notebook -side top -fill both -expand t -padx 5
    
    $notebook view "Input"
    
    set pbar [ frame $basewidget.pbar -width 400 -height 20 ]
    pack $pbar -side bottom -expand f -fill x
    CreateProgressBar $pbar
    
    set w [ frame $basewidget.w -relief ridge -width 800]
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
itcl::body mpjfibertracking::CreateMenu { mb } {    
    
    menubutton $mb.results       -text Results     -menu $mb.results.m -underline 0
    pack $mb.results    -side left
    menubutton $mb.tracking -text Connectivity -menu $mb.tracking.m -underline 0
    pack $mb.tracking -side left
    menubutton $mb.help      -text Help         -menu $mb.help.m -underline 0 -padx 4
    pack $mb.help  -side right

    menu $mb.results.m -tearoff 0 
#    eval "$mb.results.m add command -label \"Track fibers\" -command { $this fiber_track } -underline 0 -state disabled"
    $mb.results.m add command -label "Save All Results" -command [ itcl::code $this saveallresults ] -underline 0
    $mb.results.m add separator
    if { $parent == 0 } {
    eval "$mb.results.m add command -label Exit -command {  pxtkexit } -underline 1"
    } else {
    eval "$mb.results.m add command -label Close -command {  $this HideWindow } -underline 1"
    }

    menu $mb.tracking.m -tearoff 0
    $mb.tracking.m add command -label "Track fibers" -command [ itcl::code $this fiber_track ] -underline 0 -state disabled

    menu $mb.help.m -tearoff 0

    set widget_list(menu_results) $mb.results.m    
    set widget_list(menu_tracking) $mb.tracking.m

    eval "$mb.help.m add command -label About -command { $this AboutCommand }"
   
}

#-------------------------------------------------------------------------------------------
#  Add controls to menu button
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::AddToMenuButton { mb args } {
    eval "$mb add command -label \"Connectivity\" -command {$this ShowWindow \"Input\"}"
}

#-------------------------------------------------------------------------------------------
#  Main function for stand-alone execution
# ------------------------------------------------------------------------------------------
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    puts "\n[ file rootname $argv0 ] is not a stand-alone program.\n"
    exit

}



