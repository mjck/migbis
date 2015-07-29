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

package provide mpjtensoranalysis 1.3

package require  Itcl 3.2
package require  Iwidgets 4.0

package require  pxitcllistselector 1.0
package require  pxitclbaseimagecontrol 1.0
package require  pxitclbaseimageviewer 1.0

package require  vtkmpjcontrib 1.1
package require  mpjguilookuptable 1.0

itcl::class mpjtensoranalysis {

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
    private method CreateStatisticsControl { base }
    private method CreateTransformControl { base }
    private method CreateToroidalControl { base }
    private method CreateResultsControl { base }
    private method CreateTensorControl { base }
    private method CreateDisplayControl { base }
    private method CreateEigenDisplayControl { base }
    private method CreateTensorDisplayControl { base }
    private method CreateResultDisplayControl { base }
    private method CreateDirectionDisplayControl { base }
    private method CreateDisplayTypeMenu { menubase }
    private method CreateDisplayModeMenu { menubase }
    private method CreateDisplayTransformMenu { menubase }
    private method CreateDisplayColorbarMenu { menubase }
    private method CreateDisplayUpdateMenu { menubase }
    private method CreateMenu { mb }
    private method Clone { }
    public method AddToMenuButton { mb args }    
 
    private method loadmask { }
    private method loadtensor { }
    private method displayresult { { mode image } }
    private method loadresult { }
    private method saveresult { }
    private method toggleresult { }
    private method saveallresults { }
    private method savestatistics { }
 
    private method mask_autochanged_cb { }

    private method initialize_glyphs { }
    private method input_updateinfo { index }    

    private method tensor_ordering_cb { }
    private method view_mode_cb { } 
    private method view_eigen_filter_cb { }
    private method view_eigen_glyph_cb { }
    private method view_eigen_material_cb { base }
    private method view_eigen_maxscale_cb { }
    private method view_eigen_fromto_cb { }
    private method view_eigen_cmapcopy_cb { }
    private method view_eigen_cmapcopybtn_cb { }
    private method view_eigen_cmapresetbtn_cb { }
    private method view_eigen_cmaploadbtn_cb { }
    private method view_eigen_cmapsavebtn_cb { }
    private method view_eigen_tubesrc_cb { } { }
    private method view_tensor_filter_cb { }
    private method view_tensor_glyph_cb { }
    private method view_tensor_resol_cb { }    
    private method view_tensor_material_cb { base }
    private method view_tensor_gamma_xi_cb { }

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
    public method view_update_crosshair_cb { args }
    private method view_results_colormap_cb { }
    private method view_clear_display { }
    private method view_update_display { }
    private method view_toggle_autoupdate { }
    private method view_type_toggle_slow_eigenvector { }
    private method view_type_toggle_med_eigenvector { }
    private method view_type_toggle_fast_eigenvector { }
    private method view_type_toggle_tensor { }
    private method view_type_toggle_colorbar { }
    private method view_extract_roi { exts image } 
    private method view_compute_roi_extents { orient dim levels }
    private method view_create_eigenvector_glyph { index vectors scalar glyph where }
    private method view_create_eigenvector_polydata { index vectors scalar glyph where }
    private method view_create_eigenvector { index title }   
    private method view_save_eigenvector { fdir index title } 
    private method view_save_glyphs { } 
    private method view_create_tensor_glyph { tensor scalar glyph where }
    private method view_create_tensor_polydata { tensor scalar glyph where }
    private method view_create_tensor { }
    private method view_save_tensor { fdir }
    private method view_create_colorbar { }

    private method result_clear { start end }
    private method result_createmask { }
    private method result_createeigen { }
    private method result_createmaps { }
    private method result_createstats { }
    private method result_createtoroidalmaps { }
    private method result_rotatetensor { }
    
    private method result_createpie { }    
    private method result_add_stat { title fmt tag results }
    private method result_statistics { image mask title } 

    private method material_dialog { base }

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
itcl::body mpjtensoranalysis::DismissWindow { } {
    
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

	$this view_clear_display
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
itcl::body mpjtensoranalysis::InitializeControl { } {

    set appname "Diffusion Tensor Analysis $version"
    set aboutstring "(c) Marcel Jackowski 2014"

    set global_list(appname) "Diffusion Tensor Analysis"

    # create image titles
    set global_list(result_title) \
	[ list \
	      { "Diffusion tensor"        "dti_tensor" 1 } \
	      { "Region of interest mask" "dti_mask" 1 } \
	      { "Eigenvalues"             "dti_eigenval" 1 } \
	      { "Eigenvectors"            "dti_eigenvec" 1 } \
	      { "Negative eigenvalues"    "dti_neigenval" 1 } \
	      { "Fractional anisotropy"   "dti_fractani" 1 } \
	      { "FA with mean lambda"     "dti_fractaniml" 1 } \
	      { "Mean diffusivity"        "dti_meandiff" 1 } \
	      { "Volume ratio"            "dti_volratio" 1 } \
	      { "Fiber coherence"         "dti_fiberci" 1 } \
	      { "Lattice index"           "dti_lattindex" 0 } \
	      { "Lattice anisotropy"      "dti_lattani" 0 } \
	      { "Directionality"          "dti_dec" 0 } \
	      { "Toroidal volume"            "dti_tv" 1 } \
	      { "Toroidal volume normalized" "dti_tvn" 1 } \
	      { "Toroidal ratio"             "dti_tvr" 1 } \
	      { "Toroidal fraction"          "dti_tvf" 1 } \
	      { "Toroidal classification"    "dti_tcl" 1 } \
	      { "Fiber angle"                "dti_angle" 1 } \
	      { "Glyph curvature"            "dti_curv" 1 } ]

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
    set global_list(mask_threshval) 1E-12
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
    
    set global_list(toroidal_bot) 0.0
    set global_list(toroidal_top) 100.0
    set global_list(toroidal_n1) 1.0
    set global_list(toroidal_n2) 1.0    
    set global_list(toroidal_gamma) 0.1
    set global_list(toroidal_usegammamap) 0
    set global_list(toroidal_gamma2) 0.1
    set global_list(toroidal_usegammamap2) 0
    set global_list(toroidal_inequality) 1
    set global_list(toroidal_swappowers) 0
    set global_list(toroidal_xi) 0.5

    set global_list(fiber_useapexbase) 0 

    set global_list(pie_anchorpx) 0
    set global_list(pie_anchorpy) 0
    set global_list(pie_anchorpz) 0
    
    set global_list(pie_apexpx) 0
    set global_list(pie_apexpy) 0
    set global_list(pie_apexpz) 0
    set global_list(pie_basepx) 0
    set global_list(pie_basepy) 0
    set global_list(pie_basepz) 0
    set global_list(pie_numberofslices) 3
    set global_list(pie_numberofpies) 2

    set global_list(eigen_recompute) 1
    set global_list(eigen_threshval) 0.7

    set global_list(stat_overwrite) 1
    set global_list(stat_normalize) 0

    set global_list(mat_ambk) 0.0
    set global_list(mat_diffk) 1.0
    set global_list(mat_speck) 0.0
    set global_list(mat_specp) 1.0

    set global_list(eigen_mat_ambk) 0.0
    set global_list(eigen_mat_diffk) 1.0
    set global_list(eigen_mat_speck) 0.0
    set global_list(eigen_mat_specp) 1.0

    set global_list(tensor_mat_ambk) 0.0
    set global_list(tensor_mat_diffk) 1.0
    set global_list(tensor_mat_speck) 0.0
    set global_list(tensor_mat_specp) 1.0

    set global_list(view_mode) "3-slice"
    set global_lisr(view_autoupdate) 0
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
    set global_list(view_eigen_tubelen) 2.0
    set global_list(view_eigen_tuberad) 0.3
    set global_list(view_eigen_tuberes) 10
    set global_list(view_eigen_mapping) "default"

    set global_list(view_tensor_to) 1.0
    set global_list(view_tensor_from) 0.5
    set global_list(view_tensor_between) 1
    set global_list(view_tensor_maxscale) 1.0
    set global_list(view_tensor_scale) 1.0
    set global_list(view_tensor_clampscale) 1
    set global_list(view_tensor_cmapcopy) 0
    set global_list(view_tensor_resol) 10
    set global_list(view_tensor_mapping) "default"

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
	    set global_list(vtk_eigen_mapper,$i,$j) [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
	}
    }
    
    for { set i 0 } { $i < 4 } { incr i } {
	set global_list(vtk_tensor_glypher,$i) [ vtkmpjTensorGlyphs [ pxvtable::vnewobj ] ]
	set global_list(vtk_tensor_threshold,$i) [ vtkThresholdPoints [ pxvtable::vnewobj ] ]
	set global_list(vtk_tensor_merge,$i) [ vtkMergeFilter [ pxvtable::vnewobj ] ]
	set global_list(vtk_tensor_mapper,$i) [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
    }
    
    initialize_glyphs
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create visualization glyphs
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::initialize_glyphs { } {

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
    $tube_src SetHeight $global_list(view_eigen_tubelen)
    $tube_src SetRadius $global_list(view_eigen_tuberad)
    $tube_src SetResolution $global_list(view_eigen_tuberes)
    $tube_src CappingOn
    set global_list(glyph_tubesrc) $tube_src

    set tube_xfrm [ vtkTransform [ pxvtable::vnewobj ] ]
    $tube_xfrm RotateZ -90.0
    
    set tube [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $tube SetInput [ $tube_src GetOutput ]
    $tube SetTransform $tube_xfrm
    lappend global_list(glyph_list) $tube
    
    $tube_xfrm Delete
    
    ## create arrow source
    set arrow_src [ vtkArrowSource [ pxvtable::vnewobj ] ]
    $arrow_src SetTipLength 0.35
    $arrow_src SetTipResolution $global_list(view_eigen_tuberes)
    $arrow_src SetTipRadius 0.20
    $arrow_src SetShaftResolution $global_list(view_eigen_tuberes)
    $arrow_src SetShaftRadius 0.1
    set global_list(glyph_arrowsrc) $arrow_src

    set arrow_xfrm [ vtkTransform [ pxvtable::vnewobj ] ]
    $arrow_xfrm Translate -1.0 0.0 0.0
    $arrow_xfrm Scale 2.0 2.0 2.0

    set arrow [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $arrow SetInput [ $arrow_src GetOutput ]
    $arrow SetTransform $arrow_xfrm
    lappend global_list(glyph_list) $arrow

    #$arrow_src Delete
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

    ## create sheet source normal to L1
    set sheet1_src [ vtkCubeSource [ pxvtable::vnewobj ] ]
    set sheet1_xfrm [ vtkTransform [ pxvtable::vnewobj ] ]
    $sheet1_xfrm Scale 0.05 1.0 1.0
    
    set sheet1 [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $sheet1 SetInput [ $sheet1_src GetOutput ]
    $sheet1 SetTransform $sheet1_xfrm
    lappend global_list(glyph_list) $sheet1
    
    $sheet1_src Delete
#   $sheet1_xfrm Delete


    ## create sheet source normal to L2
    set sheet2_src [ vtkCubeSource [ pxvtable::vnewobj ] ]
    set sheet2_xfrm [ vtkTransform [ pxvtable::vnewobj ] ]
    $sheet2_xfrm Scale 1.0 0.05 1.0
    
    set sheet2 [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $sheet2 SetInput [ $sheet2_src GetOutput ]
    $sheet2 SetTransform $sheet2_xfrm
    lappend global_list(glyph_list) $sheet2
    
    $sheet2_src Delete
#   $sheet2_xfrm Delete


    ## create sheet source normal to L3
    set sheet3_src [ vtkCubeSource [ pxvtable::vnewobj ] ]
    set sheet3_xfrm [ vtkTransform [ pxvtable::vnewobj ] ]
    $sheet3_xfrm Scale 1.0 1.0 0.05
    
    set sheet3 [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $sheet3 SetInput [ $sheet3_src GetOutput ]
    $sheet3 SetTransform $sheet3_xfrm
    lappend global_list(glyph_list) $sheet3
    
    $sheet3_src Delete
#   $sheet3_xfrm Delete


    ## sheets L1/L2
    set sheets1 [ vtkAppendPolyData [ pxvtable::vnewobj ] ]
    $sheets1 AddInput [ $sheet1 GetOutput ]
    $sheets1 AddInput [ $sheet2 GetOutput ]
    lappend global_list(glyph_list) $sheets1

    ## sheets L1/L3 
    set sheets2 [ vtkAppendPolyData [ pxvtable::vnewobj ] ]
    $sheets2 AddInput [ $sheet1 GetOutput ]
    $sheets2 AddInput [ $sheet3 GetOutput ]
    lappend global_list(glyph_list) $sheets2

    ## sheets L2/L3 
    set sheets3 [ vtkAppendPolyData [ pxvtable::vnewobj ] ]
    $sheets3 AddInput [ $sheet2 GetOutput ]
    $sheets3 AddInput [ $sheet3 GetOutput ]
    lappend global_list(glyph_list) $sheets3


    ## sheets L1/L2/L3 
    set sheets4 [ vtkAppendPolyData [ pxvtable::vnewobj ] ]
    $sheets4 AddInput [ $sheet1 GetOutput ]
    $sheets4 AddInput [ $sheet2 GetOutput ]
    $sheets4 AddInput [ $sheet3 GetOutput ]
    lappend global_list(glyph_list) $sheets4


    $sheet1_xfrm Delete
    $sheet2_xfrm Delete
    $sheet3_xfrm Delete  
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in mask auto status
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::mask_autochanged_cb { } {
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
itcl::body mpjtensoranalysis::input_updateinfo { index } {

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
itcl::body mpjtensoranalysis::loadmask { } {

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
itcl::body mpjtensoranalysis::LoadMask { fname } {

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
    # -- force integer!
    set global_list(mask_nreg) [ expr int([ lindex $range 1 ])]
    $widget_list(mask_nreg) configure -range [ list 1 $global_list(mask_nreg) ]
    
    set global_list(mask_fname) $fname

    WatchOff

    $ana Delete
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in tensor component ordering
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::tensor_ordering_cb { } {
    
    ## current ordering
    set global_list(tensor_ordering) [ $widget_list(tensor_ordering) index [ $widget_list(tensor_ordering) get ] ]   
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load tensor
# -----------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::loadtensor { } {

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
itcl::body mpjtensoranalysis::LoadTensor { fname } {

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
itcl::body mpjtensoranalysis::SetResultPreffix { preffix } {
    #puts "mpjtensoranalysis::SetCurrentDirectory $fname"
    set global_list(result_fpreffix) $preffix
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Get result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::GetResultPreffix { } {
    return $global_list(result_fpreffix)
}

itcl::body mpjtensoranalysis::SetCurrentDirectory { fname } {
    #puts $fname
    set global_list(result_fdir) $fname
}


#-------------------------------------------------------------------------------------------
#  PUBLIC: Clear all results
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::ClearAllResults { } {
    
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
itcl::body mpjtensoranalysis::SaveAllResults { } {

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
itcl::body mpjtensoranalysis::loadresult { } {

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
itcl::body mpjtensoranalysis::SaveResult { index } {        

    set fname $global_list(result_fpreffix)
    set suffix $global_list(result_fsuffix)	
    set fname "$fname[ lindex [lindex $global_list(result_title) $index ] 1 ]${suffix}"

    [lindex $global_list(result_list) $index] Save $fname
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::saveallresults { } {

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
itcl::body mpjtensoranalysis::saveresult { } {
    
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
itcl::body mpjtensoranalysis::DisplayResult { index { mode image } } {

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
itcl::body mpjtensoranalysis::displayresult { { mode image } } {

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
itcl::body mpjtensoranalysis::toggleresult { } {
   
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
itcl::body mpjtensoranalysis::SaveStatistics { fname } {
      $widget_list(stat_text) export $fname
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save statistics
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::savestatistics { } {

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
itcl::body mpjtensoranalysis::result_add_stat { title fmt tag results } {   
    
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
itcl::body mpjtensoranalysis::result_statistics { image mask title } {
    
    ## compute statitics from given image and mask
    set stat [ vtkmpjImageStatistics [ pxvtable::vnewobj ] ]
    $stat SetMask $mask
    $stat SetInput $image
    $stat SetNumberOfRegions $global_list(mask_nreg)
    $stat Update
    
  set norm [ vtkmpjImageNormalize [ pxvtable::vnewobj ] ]
	   
    ## normalization if necessary
    if { $global_list(stat_normalize) == 1 } {
    
       set minmax [ vtkFloatArray [ pxvtable::vnewobj ] ]
       $minmax SetNumberOfComponents 2
       $minmax SetNumberOfTuples $global_list(mask_nreg)
       
       set min 0
       set max 0
       for { set i 0 } { $i < $global_list(mask_nreg) } { incr i } {	
	       set min [ $stat GetRegionMin $i ]
	       set max [ $stat GetRegionMax $i ]
#             puts "$min $max"
	       $minmax SetComponent $i 0 $min
	       $minmax SetComponent $i 1 $max	       
        }    
    
	   ##set norm [ vtkmpjImageNormalize [ pxvtable::vnewobj ] ]
	   $norm SetMask $mask
	   $norm SetInput $image
	   $norm SetRegionStatistics $minmax
	   $norm SetNumberOfRegions $global_list(mask_nreg)
	   $norm Update
	   
	   $minmax Delete
	   
	   $stat Delete

	   ##  compute new statistics	  
         set stat [ vtkmpjImageStatistics [ pxvtable::vnewobj ] ]
         $stat SetMask $mask
         $stat SetInput [ $norm GetOutput ]
         $stat SetNumberOfRegions $global_list(mask_nreg)
         $stat Update
 
    }

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
    
    set results ""
    for { set i 0 } { $i < $global_list(mask_nreg) } { incr i } {	       
	set var [ $stat GetRegionVariance $i ]
	if { $var >= 0 } {
		set var [ expr sqrt($var) ]
	    } else {
		set var "0.0"
	    }
	    lappend results $var
    }    
    result_add_stat " STANDARD DEVIATION " "%8.4f" even $results

    $widget_list(stat_text) insert end "\n" odd 

    set results ""
    for { set i 0 } { $i < $global_list(mask_nreg) } { incr i } {	
	lappend results [ $stat GetRegionVoxelCount $i ]
    }    

    $norm Delete
    $stat Delete

    return $results
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Clear results
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::result_clear { start end } {
    
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
itcl::body mpjtensoranalysis::result_rotatetensor { } {

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
#  PRIVATE: Create pie
#-------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::result_createpie { } {

    set TENSOR 0
    set MASK 1
    
    ## get tensor image
    set tensor [ lindex $global_list(result_list) $TENSOR ]
    
    set mask [ lindex $global_list(result_list) $MASK ]     
    $mask CopyImageHeader \
	[ [ lindex $global_list(result_list) $TENSOR ] GetImageHeader ]
	
    set center [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set cx [ lindex $center 0 ]
    set cy [ lindex $center 1 ]
    set cz [ lindex $center 2 ]
	
    set pie_mask [ vtkmpjImagePieMask [ pxvtable::vnewobj ] ]
    $pie_mask SetInput [ $tensor GetImage ]
    $pie_mask SetMask [ $mask GetImage ]
    $pie_mask SetAnchor \
		$global_list(pie_anchorpx) $global_list(pie_anchorpy) $global_list(pie_anchorpz)	
	$pie_mask SetApex \
		$global_list(pie_apexpx) $global_list(pie_apexpy) $global_list(pie_apexpz)
	$pie_mask SetBase \
		$global_list(pie_basepx) $global_list(pie_basepy) $global_list(pie_basepz)		
	$pie_mask SetNumberOfSlices $global_list(pie_numberofslices)
	$pie_mask SetNumberOfPies $global_list(pie_numberofpies)
    $this SetFilterCallbacks $pie_mask "Creating pie..."
    $pie_mask Update
      
     ## save mask
    $mask ShallowCopyImage [ $pie_mask GetOutput ] 
    $mask CopyImageHeader [ $tensor GetImageHeader ]
       
    $pie_mask Delete
    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create mask image
#-------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::result_createmask { }  {

    set TENSOR 0
    set MASK 1
    
    ## get tensor image
    set tensor [ lindex $global_list(result_list) $TENSOR ]
    
    set mask [ lindex $global_list(result_list) $MASK ]    
    $mask CopyImageHeader \
	[ [ lindex $global_list(result_list) $TENSOR ] GetImageHeader ]
    
    set frm_filter [ vtkImageExtractComponents [ pxvtable::vnewobj ] ]
    $frm_filter SetInput [ $tensor GetImage ]
    $frm_filter SetComponents 0
    $frm_filter Update

#    puts stderr "Spacing [ [ $frm_filter GetOutput ] GetSpacing ]"
    
    ## threshold tensor image
    set thresh [ vtkImageThreshold [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $thresh "Creating mask..."
    $thresh SetInput [ $frm_filter GetOutput ]
    $thresh ThresholdByUpper $global_list(mask_threshval)
    $thresh SetInValue 1
    $thresh SetOutValue 0
    $thresh SetOutputScalarTypeToUnsignedChar
    $thresh Update

#    puts stderr "Spacing [ [ $thresh GetOutput ] GetSpacing ]"
    
    $frm_filter Delete

    ## save mask
    $mask ShallowCopyImage [ $thresh GetOutput ] 
    $mask CopyImageHeader  [ $tensor GetImageHeader ]
#    puts stderr "Mask = [ $mask GetDescription ]"
    $thresh Delete    
    
    update idletasks
    
    return 1
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create eigensystem
#-------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::result_createeigen { } {

    set TENSOR 0
    set MASK 1
    set EIGENVALUES 2
    set EIGENVECTORS 3
    set NEGATIVEMASK 4

    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]
    set negative [ lindex $global_list(result_list) $NEGATIVEMASK ]
    
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
    $eigen SetSortByMagnitude 1
    $eigen SetMaskNegativeInValue 0
    $eigen SetMaskNegativeOutValue 255
    $eigen Update
    
    ## extract mask with negative eigenvalues
    $negative ShallowCopyImage [ $eigen GetNegativeMask ]
    
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
    $negative CopyImageHeader [ $mask GetImageHeader ]


    $fex Delete

    $eigen Delete

    update idletasks

    return 1
}

#-------------------------------------------------------------------------------------------
#  Private: Create statistics
#-------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::result_createstats { } {

    set TENSOR 0
    set MASK 1
    set EIGENVALUES 2
    set EIGENVECTORS 3
    set NEGATIVEMASK 4
    set FA_MAP 5
    set RA_MAP 6
    set MD_MAP 7
    set VR_MAP 8
    set CI_MAP 9
    set LI_MAP 10
    set ADD_MAP 11
    set DIR_MAP 12
    set TOV_MAP 13
    set TVN_MAP 14
    set TVR_MAP 15
    set TVF_MAP 16
    set DIS_MAP 17
    set ANGLE_MAP 18
    set CURV_MAP 19

    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]
    set negative [ lindex $global_list(result_list) $NEGATIVEMASK ]

    set famap [ lindex $global_list(result_list) $FA_MAP ]
    set ramap [ lindex $global_list(result_list) $RA_MAP ]
    set mdmap [ lindex $global_list(result_list) $MD_MAP ]
    set vrmap [ lindex $global_list(result_list) $VR_MAP ]
    set cimap [ lindex $global_list(result_list) $CI_MAP ]
    set limap [ lindex $global_list(result_list) $LI_MAP ]
    set addmap [ lindex $global_list(result_list) $ADD_MAP ]
    set tovmap [ lindex $global_list(result_list) $TOV_MAP ]
    set tvnmap [ lindex $global_list(result_list) $TVN_MAP ]
    set tvrmap [ lindex $global_list(result_list) $TVR_MAP ]
    set tvfmap [ lindex $global_list(result_list) $TVF_MAP ]
    set dismap [ lindex $global_list(result_list) $DIS_MAP ]
    set anglemap [ lindex $global_list(result_list) $ANGLE_MAP ]
    set curvmap [ lindex $global_list(result_list) $CURV_MAP ]

    set mapmask [ pxitclimage \#auto ]
    $mapmask CopyImageHeader [ $tensor GetImageHeader ] 
    
    if { $global_list(mask_negative) == 1 } {	
	
	## compute mask and void pixels in the mask having negative eigenvalues
	set imlogic [ vtkImageLogic [ pxvtable::vnewobj ] ]
	$this SetFilterCallbacks $imlogic "Combining mask (step 1)..."
	$imlogic SetInput1 [ $mask GetImage ]
	$imlogic SetInput2 [ $negative GetImage ]
	$imlogic SetOperationToAnd
	$imlogic SetOutputTrueValue 1
	$imlogic Update	
	
	set newmask [ vtkImageMathematics [ pxvtable::vnewobj ] ]
	$this SetFilterCallbacks $imlogic "Combining mask (step 2)..."
	$newmask SetInput1 [ $imlogic GetOutput ]
	$newmask SetInput2 [ $mask GetImage ]
	$newmask SetOperationToMultiply
	$newmask Update

	$imlogic Delete
	
	$mapmask ShallowCopyImage [ $newmask GetOutput ]

	$newmask Delete

    } else {

	$mapmask ShallowCopyImage [ $mask GetImage ]
    }
    
    ## clear textbox
    if { $global_list(stat_overwrite) } { 
	$widget_list(stat_text) delete 1.0 end
    }
    
    set results 0

    set enable [ lindex [ lindex $global_list(result_title) $FA_MAP ] 2 ]
    if { $enable } {
	################### extract Fractional Anisotropy map #####################
	set results [ result_statistics [ $famap GetImage ] [ $mapmask GetImage ] "FRACTIONAL ANISOTROPY" ]
    }
    
    set enable [ lindex [ lindex $global_list(result_title) $RA_MAP ] 2 ]
    if { $enable } {
	################### extract Relative Anisotropy map #####################
	set results [ result_statistics [ $ramap GetImage ] [ $mapmask GetImage ] "FA WITH MEAN LAMBDA  " ]
    }

    set enable [ lindex [ lindex $global_list(result_title) $MD_MAP ] 2 ]
    if { $enable } {
	##################### extract Mean diffusity map #####################
	set results [ result_statistics [ $mdmap GetImage ] [ $mapmask GetImage ] "MEAN DIFFUSIVITY     " ]
    }

    set enable [ lindex [ lindex $global_list(result_title) $VR_MAP ] 2 ]
    if { $enable } {
	##################### extract Volume Ratio map ###################
	set results [ result_statistics [ $vrmap GetImage ] [ $mapmask GetImage ] "VOLUME RATIO         " ]
    }
    
    set enable [ lindex [ lindex $global_list(result_title) $CI_MAP ] 2 ]
    if { $enable } {
	##################### extract Intervoxel Coherence map ########################
	set results [ result_statistics [ $cimap GetImage ] [ $mapmask GetImage ] "COHERENCE INDEX      " ]
    }

    set enable1 [ lindex [ lindex $global_list(result_title) $LI_MAP ] 2 ]
    set enable2 [ lindex [ lindex $global_list(result_title) $ADD_MAP ] 2 ]
    if { [expr $enable1 || $enable2 ] } {
	
	if { $enable1 } {
	    ##################### Lattice anisotropy ########################
	    set results [ result_statistics [ $limap GetImage ] [ $mapmask GetImage ] "LATT ANISOTROPY (LI) " ]
	}
	
	if { $enable2 } {
	    ##################### Add #######################################
	    set results [ result_statistics [ $addmap GetImage ] [ $mapmask GetImage ] "LATT ANISOTROPY (Add)" ]
	}
    }
        
    set slice [ [ $parent GetViewer ] GetOrthoImageSlice ]
    
    set center 0
    if { [ llength $slice ] > 0 } {
	set center [ $slice GetLevels ]
    } else {
	set dim [ [ $tensor GetImage ] GetDimensions ]
	set center "[expr [lindex $dim 0] / 2] [expr [lindex $dim 1] / 2] [expr [lindex $dim 2] / 2]"
    }

    set cx [ lindex $center 0 ]
    set cy [ lindex $center 1 ]
    set cz [ lindex $center 2 ]
    
    ########### Heart fiber angle
    set results [ result_statistics [ $anglemap GetImage ] [ $mapmask GetImage ] "FIBER ANGLE          " ]

	########### Toroidal volume
    set enable [ lindex [ lindex $global_list(result_title) $TOV_MAP ] 2 ]
    if { $enable } {
	    set results [ result_statistics [ $tovmap GetImage ] [ $mapmask GetImage ] "TOROIDAL VOLUME      " ]
	    set results [ result_statistics [ $tvrmap GetImage ] [ $mapmask GetImage ] "TOROIDAL RATIO      " ]
	    set results [ result_statistics [ $tvfmap GetImage ] [ $mapmask GetImage ] "TOROIDAL FRACTION   "]
	}            
        
	############ Curvature
	#set enable [ lindex [ lindex $global_list(result_title) $CURV_MAP 2 ] ]
	#if { $enable } {
	
		#set curv [ vtkmpjToroidalCurvature [ pxvtable::vnewobj ] ]
		#$curv SetInput [ $eigenvalues GetImage ]
		#$curv SetMask [ $mask GetImage ]
		#$curv Update
	
		#$curvmap ShallowCopyImage [ $curv GetOutput ]
		#$curv Delete
	
		#set results [ result_statistics [ $curvmap GetImage ] [ $mapmask GetImage ] "CURVATURE INDEX      " ]
	#}
	
    ##################### Voxel count ###############################
    set border [ string repeat "==========+" $global_list(mask_nreg) ] 
    $widget_list(stat_text) insert end "=======================+$border\n" even
	
    result_add_stat " TOTAL VOXELS       " "%8d" odd $results  

    $mapmask Clear
    itcl::delete object $mapmask

    $this ShowProgressVal "Done." 0
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create maps
#-------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::result_createmaps { } {
    
    set TENSOR 0
    set MASK 1
    set EIGENVALUES 2
    set EIGENVECTORS 3
    set NEGATIVEMASK 4
    set FA_MAP 5
    set RA_MAP 6
    set MD_MAP 7
    set VR_MAP 8
    set CI_MAP 9
    set LI_MAP 10
    set ADD_MAP 11
    set DIR_MAP 12
    set TOV_MAP 13
    set TVN_MAP 14
    set TVR_MAP 15
    set TVF_MAP 16
    set DIS_MAP 17
    set ANGLE_MAP 18
    set CURV_MAP 19

    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]
    set negative [ lindex $global_list(result_list) $NEGATIVEMASK ]


#    puts stderr "Tensor spacing = [  [ $tensor GetImage ] GetSpacing ]"
#    puts stderr "Tensor description = [ $tensor GetDescription ]\n Mask = [ $mask GetDescription ]\n"

    set famap [ lindex $global_list(result_list) $FA_MAP ]
    set ramap [ lindex $global_list(result_list) $RA_MAP ]
    set mdmap [ lindex $global_list(result_list) $MD_MAP ]
    set vrmap [ lindex $global_list(result_list) $VR_MAP ]
    set cimap [ lindex $global_list(result_list) $CI_MAP ]
    set limap [ lindex $global_list(result_list) $LI_MAP ]
    set addmap [ lindex $global_list(result_list) $ADD_MAP ]
    set tovmap [ lindex $global_list(result_list) $TOV_MAP ]
    set tvnmap [ lindex $global_list(result_list) $TVN_MAP ]
    set tvrmap [ lindex $global_list(result_list) $TVR_MAP ]
    set tvfmap [ lindex $global_list(result_list) $TVF_MAP ]
    set dismap [ lindex $global_list(result_list) $DIS_MAP ]
    set anglemap [ lindex $global_list(result_list) $ANGLE_MAP ]
    set curvmap [ lindex $global_list(result_list) $CURV_MAP ]
    
    set deletemapmask 0
    set mapmask 0 
    #    $mapmask CopyImageHeader [ $mask GetImageHeader ]    
    

    if { $global_list(mask_negative) == 1 } {	
	
	#	puts stderr  "In Imlogic mask = [ [ $mask GetImage ] GetSpacing ], negative= [ [ $negative GetImage ] GetSpacing ]"

	## compute mask and void pixels in the mask having negative eigenvalues
	set imlogic [ vtkImageLogic [ pxvtable::vnewobj ] ]
	$this SetFilterCallbacks $imlogic "Combining mask (step 1)..."
	$imlogic SetInput1 [ $mask GetImage ]
	$imlogic SetInput2 [ $negative GetImage ]
	$imlogic SetOperationToAnd
	$imlogic SetOutputTrueValue 1
	$imlogic Update	
	
#	puts stderr  "In New imlogic = [ [ $imlogic GetOutput ] GetSpacing ], mask [ [ $mask GetImage ] GetSpacing ]"

	set newmask [ vtkImageMathematics [ pxvtable::vnewobj ] ]
	$this SetFilterCallbacks $imlogic "Combining mask (step 2)..."
	$newmask SetInput1 [ $imlogic GetOutput ]
	$newmask SetInput2 [ $mask GetImage ]
	$newmask SetOperationToMultiply
	$newmask Update

	$imlogic Delete
	
	set mapmask [ pxitclimage \#auto ]
	$mapmask ShallowCopyImage [ $newmask GetOutput ]
	set deletemapmask 1
	$newmask Delete

    } else {
	set mapmask $mask
#	puts stderr "Here we are done ....."
	set deletemapmask 0
    }

#    puts stderr "Eigenvalues = [ $eigenvalues GetDescription ] , spa=[ [ $eigenvalues GetImage ] GetSpacing ]"
#    puts stderr "MapMask     = [ $mapmask GetDescription ] , spa= [ [ $mapmask GetImage ] GetSpacing ]"

    
    ## compute invariants
    set invar [ vtkmpjImageTensorInvariants [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $invar "Computing tensor maps..."
    $invar SetInput [ $eigenvalues GetImage ]
    $invar SetMask [ $mapmask GetImage ]
    $invar Update

    ## clear textbox
    if { $global_list(stat_overwrite) } { 
	$widget_list(stat_text) delete 1.0 end
    }
    
    set results 0

 #### MPJ - 21/10/2009
    ################### extract axial diffusivity map #####################
    set axrad [ vtkmpjAxialRadialDiffusivity [ pxvtable::vnewobj ] ]
    $axrad SetInput [ $eigenvalues GetImage ] 		
    $axrad SetMask [ $mask GetImage ]
    $axrad Update
    
    set axial [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
    $axial SetInput [ $axrad GetOutput ]
    $axial SetFrameNumber 0
    $axial Update    
	    
    ##$axialmap ShallowCopyImage [ $axial GetOutput ]
    ##$axial Delete

    set results [ result_statistics [ $axial GetOutput ] [ $mapmask GetImage ] "AXIAL DIFFUSIVITY    " ]    
    $axial Delete

    set radial [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
    $radial SetInput [ $axrad GetOutput ]
    $radial SetFrameNumber 1
    $radial Update    
	    
    ##$radialmap ShallowCopyImage [ $radial GetOutput ]
    ##$radial Delete
 
    set results [ result_statistics [ $radial GetOutput ] [ $mapmask GetImage ] "RADIAL DIFFUSIVITY   " ]
    $radial Delete
    $axrad Delete
    ##### MPJ - 21/10/2009

    set enable [ lindex [ lindex $global_list(result_title) $FA_MAP ] 2 ]
    if { $enable } {
	################### extract Fractional Anisotropy map #####################
	set fa [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	$this ShowProgressVal "Computing statistics..." [expr 2.0 / 9]
	$fa SetInput [ $invar GetOutput ]
	$fa SetFrameNumber 0
	$fa Update
	
	$famap ShallowCopyImage [ $fa GetOutput ]
	$famap CopyImageHeader [ $mask GetImageHeader ]
	$fa Delete
	
	set results [ result_statistics [ $famap GetImage ] [ $mapmask GetImage ] "FRACTIONAL ANISOTROPY" ]
    }
    

    set enable [ lindex [ lindex $global_list(result_title) $RA_MAP ] 2 ]
    if { $enable } {
	################### extract Relative Anisotropy map #####################
	set ra [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	$this ShowProgressVal "Computing statistics..." [expr 3.0 / 9]
	$ra SetInput [ $invar GetOutput ]
	$ra SetFrameNumber 1
	$ra Update    
	
	$ramap ShallowCopyImage [ $ra GetOutput ]
	$ramap CopyImageHeader [ $mask GetImageHeader ]

    
	
	$ra Delete
	
	set results [ result_statistics [ $ramap GetImage ] [ $mapmask GetImage ] "FA WITH MEAN LAMBDA  " ]
    }

    set enable [ lindex [ lindex $global_list(result_title) $MD_MAP ] 2 ]
    if { $enable } {
	##################### extract Mean diffusity map #####################
	set md [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	$this ShowProgressVal "Computing statistics..." [expr 4.0 / 9]
	$md SetInput [ $invar GetOutput ]
	$md SetFrameNumber 3
	$md Update    
	
	$mdmap ShallowCopyImage [ $md GetOutput ]
	$mdmap CopyImageHeader [ $mask GetImageHeader ]
	$md Delete
	
	set results [ result_statistics [ $mdmap GetImage ] [ $mapmask GetImage ] "MEAN DIFFUSIVITY     " ]
    }

    set enable [ lindex [ lindex $global_list(result_title) $VR_MAP ] 2 ]
    if { $enable } {

	##################### extract Volume Ratio map ###################
	set vr [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	$this ShowProgressVal "Computing statistics..." [expr 5.0 / 9]
	$vr SetInput [ $invar GetOutput ]
	$vr SetFrameNumber 2
	$vr Update    
	
	$vrmap ShallowCopyImage [ $vr GetOutput ]
	$vrmap CopyImageHeader [ $mask GetImageHeader ]
	$vr Delete
	
	set results [ result_statistics [ $vrmap GetImage ] [ $mapmask GetImage ] "VOLUME RATIO         " ]
    }
    
    set enable [ lindex [ lindex $global_list(result_title) $CI_MAP ] 2 ]
    if { $enable } {
	##################### extract Intervoxel Coherence map ########################
	set ci [ vtkmpjImageTensorCoherence [ pxvtable::vnewobj ] ]
	$this ShowProgressVal "Computing statistics..." [expr 6.0 / 9]
	$ci SetInput [ $eigenvectors GetImage ]
	$ci SetMask [ $mapmask GetImage ]
	$ci SetKernelSize 3 3 3
	$ci Update
	
	$cimap ShallowCopyImage [ $ci GetOutput ]
	$cimap CopyImageHeader [ $mask GetImageHeader ]
	$ci Delete
	
	set results [ result_statistics [ $cimap GetImage ] [ $mapmask GetImage ] "COHERENCE INDEX      " ]
    }

    set enable1 [ lindex [ lindex $global_list(result_title) $LI_MAP ] 2 ]
    set enable2 [ lindex [ lindex $global_list(result_title) $ADD_MAP ] 2 ]
    if { [expr $enable1 || $enable2 ] } {
	
	if { $enable1 } {
	    ##################### Lattice anisotropy ########################
	    set la [ vtkmpjLatticeAnisotropy [ pxvtable::vnewobj ] ]
	    $this ShowProgressVal "Computing statistics..." [expr 9.0 / 9]
	    $la SetEigenvectors [ $eigenvectors GetImage ]
	    $la SetEigenvalues [ $eigenvalues GetImage ]
	    $la SetMask [ $mapmask GetImage ]
	    $la SetKernelSize 3 3 3
	    $la SetMethodToBasserAndSkare
	    $la Update
	    
	    set li [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	    $li SetInput [ $la GetOutput ]
	    $li SetFrameNumber 0
	    $li Update    
	    
	    $limap ShallowCopyImage [ $li GetOutput ]
	    $limap CopyImageHeader [ $mask GetImageHeader ]
	    $li Delete
	    
	    set results [ result_statistics [ $limap GetImage ] [ $mapmask GetImage ] "LATT ANISOTROPY (LI) " ]
	}
	
	if { $enable2 } {
	    ##################### Add #######################################
	    set add [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	    $add SetInput [ $la GetOutput ]
	    $add SetFrameNumber 1
	    $add Update    
	    
	    $addmap ShallowCopyImage [ $add GetOutput ]
	    $addmap CopyImageHeader [ $mask GetImageHeader ]
	    $add Delete
	    
	    $la Delete
	    
	    set results [ result_statistics [ $addmap GetImage ] [ $mapmask GetImage ] "LATT ANISOTROPY (Add)" ]
	}
    }
    
    set slice 0

    
    set center 0
    catch {
	set slice [ [ $parent GetViewer ] GetOrthoImageSlice ]
	set center [ $slice GetLevels ]
    } 

    if { $center == 0 } {
	set dim [ [ $tensor GetImage ] GetDimensions ]
	set center "[expr [lindex $dim 0] / 2] [expr [lindex $dim 1] / 2] [expr [lindex $dim 2] / 2]"
    }

    set cx [ lindex $center 0 ]
    set cy [ lindex $center 1 ]
    set cz [ lindex $center 2 ]
    
    ########### Heart fiber angle
    
     ## get fiber angle filter
    set angle_type [ $widget_list(fiber_angle_rep) index [ $widget_list(fiber_angle_rep) get ] ] 
    
    set fiberangle [ vtkmpjHeartFiberAngle [  pxvtable::vnewobj ] ]
    $fiberangle SetInput [ $eigenvectors GetImage ]
    $fiberangle SetMask [ $mask GetImage ]
####$fiberangle SetOutputCoordinates 1
    $fiberangle SetAngleRepresentation $angle_type

    ### added by mjack on Feb 19, 2010.
    if { $global_list(fiber_useapexbase) == 0 } {
       $fiberangle SetCenter $cx $cy $cz
       $fiberangle SetCenter2 $cx $cy [expr $cz + 1]
    } else {
       $fiberangle SetCenter $global_list(pie_basepx) $global_list(pie_basepy) $global_list(pie_basepz)
       $fiberangle SetCenter2 $global_list(pie_apexpx) $global_list(pie_apexpy) $global_list(pie_apexpz)
    }

    $fiberangle Update

    ####### For fiber coordinates ###########
    #set fex [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
    #$this SetFilterCallbacks $fex "Extracting coordinates..."
    #$fex SetInput [ $fiberangle GetOutput ]
    #$fex SetStartFrame 1
    #$fex SetEndFrame 9
    #$fex Update
    #$eigenvectors ShallowCopyImage [ $fex GetOutput ]
    #$fex Delete
    
    $anglemap ShallowCopyImage [ $fiberangle GetOutput ]
    $anglemap CopyImageHeader [ $mask GetImageHeader ]
    $fiberangle Delete
    
    set results [ result_statistics [ $anglemap GetImage ] [ $mapmask GetImage ] "FIBER ANGLE          " ]
    
	########### Toroidal volume
    set enable [ lindex [ lindex $global_list(result_title) $TOV_MAP ] 2 ]
    if { $enable } {
	    
	    set tov [ vtkmpjToroidalVolume [ pxvtable::vnewobj ] ]
	    $tov SetInput [ $eigenvalues GetImage ] 		
	    $tov SetMask [ $mask GetImage ]
	    $tov Update
	    
    	    set tv [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	    $tv SetInput [ $tov GetOutput ]
	    $tv SetFrameNumber 0
	    $tv Update    
	    
	    $tovmap ShallowCopyImage [ $tv GetOutput ]
	    $tovmap CopyImageHeader [ $mask GetImageHeader ]
	    $tv Delete
	    
	    set results [ result_statistics [ $tovmap GetImage ] [ $mapmask GetImage ] "TOROIDAL VOLUME      " ]
	    
	    set tvr [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	    $tvr SetInput [ $tov GetOutput ]
	    $tvr SetFrameNumber 2
	    $tvr Update    
	    
	    $tvrmap ShallowCopyImage [ $tvr GetOutput ]
	    $tvrmap CopyImageHeader [ $mask GetImageHeader ]
	    $tvr Delete		
	    
	    set results [ result_statistics [ $tvrmap GetImage ] [ $mapmask GetImage ] "TOROIDAL RATIO      " ]
	    
	    set tvf [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	    $tvf SetInput [ $tov GetOutput ]
	    $tvf SetFrameNumber 3
	    $tvf Update    
	    
	    $tvfmap ShallowCopyImage [ $tvf GetOutput ]
	    $tvfmap CopyImageHeader [ $mask GetImageHeader ]
	    $tvf Delete		
	    
	    set results [ result_statistics [ $tvfmap GetImage ] [ $mapmask GetImage ] "TOROIDAL FRACTION   "];
	    
	    $tov Delete		
	}            
        
	############ Curvature
	#set enable [ lindex [ lindex $global_list(result_title) $CURV_MAP 2 ] ]
	#if { $enable } {
	
		#set curv [ vtkmpjToroidalCurvature [ pxvtable::vnewobj ] ]
		#$curv SetInput [ $eigenvalues GetImage ]
		#$curv SetMask [ $mask GetImage ]
		#$curv Update

		#$curvmap ShallowCopyImage [ $curv GetOutput ]
		#$curv Delete
	
		#set results [ result_statistics [ $curvmap GetImage ] [ $mapmask GetImage ] "CURVATURE INDEX      " ]
	#}




    ##################### Voxel count ###############################
    set border [ string repeat "==========+" $global_list(mask_nreg) ] 
    $widget_list(stat_text) insert end "=======================+$border\n" even
    
    result_add_stat " TOTAL VOXELS       " "%8d" odd $results  

    $invar Delete

    #    $mapmask Clear
    if { $deletemapmask > 0 } {
	itcl::delete object $mapmask
    }

    $this ShowProgressVal "Done." 0
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Compute toroidal maps
# -----------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::result_createtoroidalmaps { } {

    set TENSOR 0
    set MASK 1
    set EIGENVALUES 2
    set EIGENVECTORS 3
    set TOV_MAP 13
    set TVN_MAP 14
    set TVR_MAP 15
    set TVF_MAP 16
    set DIS_MAP 17
    
    set tensor [ lindex $global_list(result_list) $TENSOR ]    
    set mask [ lindex $global_list(result_list) $MASK ]
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]
        
    set tovmap [ lindex $global_list(result_list) $TOV_MAP ]
    $tovmap CopyImageHeader [ $tensor GetImageHeader ] 
    
    set tvnmap [ lindex $global_list(result_list) $TVN_MAP ]
    $tvnmap CopyImageHeader [ $tensor GetImageHeader ] 
    
    set tvrmap [ lindex $global_list(result_list) $TVR_MAP ]
    $tvrmap CopyImageHeader [ $tensor GetImageHeader ] 
    
    set tvfmap [ lindex $global_list(result_list) $TVF_MAP ]
    $tvfmap CopyImageHeader [ $tensor GetImageHeader ]
        
    set dismap [ lindex $global_list(result_list) $DIS_MAP ]
    $dismap CopyImageHeader [ $tensor GetImageHeader ]
        	
	########### Toroidal dissociation map - Tvc
    set enable [ lindex [ lindex $global_list(result_title) $DIS_MAP ] 2 ]
    if { $enable } {
		
		## get magnitude filter
        set filter [ $widget_list(toroidal_dissoc_filter) index [ $widget_list(toroidal_dissoc_filter) get ] ]        
        set toroidal_filter 0 
        set magmap 0
    
		set dis [ vtkmpjToroidalDissociation [ pxvtable::vnewobj ] ]
		$dis SetInput [ $eigenvalues GetImage ] 
		$dis SetMask [ $mask GetImage ]
		$dis SetCaseL1GreaterL2GreaterL3 1
		$dis SetCaseL1GreaterL2EqualL3 2
		$dis SetCaseL1EqualL2GreaterL3 3
		$dis SetCaseL1EqualL2EqualL3 4
		$dis SetEigenvalueThreshold $global_list(eigen_threshval)
		
		if { $filter > 0 } {
			set toroidal_filter [ lindex $global_list(result_list) [expr $filter -1 ] ]
			set magmap [ lindex $global_list(result_list) [expr $filter -1] ]
			$dis SetMagnitude [ $magmap GetImage ]
		}
		
		$dis Update
    
		$dismap ShallowCopyImage [ $dis GetOutput ]
		$dis Delete		
	}
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Compute results
# -----------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::ComputeResults { } {
    
    set MASK 1
    set FA_MAP 5

    WatchOn
    
    ## clear display
    view_clear_display
    
    ## clear results
    if { $global_list(eigen_recompute) } {
	result_clear $MASK [ expr [ llength $global_list(result_list) ] -1 ]
    } else {
	result_clear $FA_MAP [ expr [ llength $global_list(result_list) ] -1 ]
    }

    $widget_list(notebook) view "Statistics"; update

    ## create mask if not loaded in
    if { $global_list(mask_auto) } {
	
	set result [ result_createmask ]
	
	if { $result == 0 } {
	    WatchOff; return
	}
    }
    
    if { $global_list(eigen_recompute) } {

	## compute eigevalues and eigenvectors
	set result [ result_createeigen ]
 
	if { $result == 0 } {
	    WatchOff; return
	}
    }

    ## create maps
    result_createmaps
    
    # send currently selected result to parent
    $widget_list(result_list) selection set $FA_MAP $FA_MAP
    
    set fa_map [ lindex $global_list(result_list) $FA_MAP ]
    $parent SetImageFromObject [ $fa_map  GetThisPointer ] $this

    set range [ [ [ [ $fa_map GetImage ] GetPointData ] GetScalars ] GetRange ]
    #$widget_list(view_resultluk) SetTableRange [ lindex $range 0 ] [ lindex $range 1 ]
    #$widget_list(view_resultluk) Update
    
    ## update display
    view_update_display

    WatchOff
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Clear display
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_clear_display { } {
    
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
#  PRIVATE: Save eigenvector fields
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_save_glyphs { } {

    ## browse for directory
    set fdir [ tk_chooseDirectory -title " Select directory" -parent $basewidget -initialdir $global_list(result_fdir) ]
    
    if { $fdir != "" } {
        
        set global_list(result_fdir) $fdir

        if { $global_list(view_type_fast) } {   
            view_save_eigenvector $fdir fast 0
        }
    
        if { $global_list(view_type_medium) } { 
            view_save_eigenvector $fdir med 1
        }
    
        if { $global_list(view_type_slow) } { 
            view_save_eigenvector $fdir slow 2
        }

        if { $global_list(view_type_tensor) } {
            view_save_tensor $fdir 
        }
    }   
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Update display
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_update_display { } {

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

    if { $bis_algorithmmode == 0 } {
	[ $parent GetViewer ] SetDisplayMode3DIfNeeded
        [ $parent GetViewer ] UpdateDisplay
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Compute ROI extents
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_compute_roi_extents { orient dim levels } {
   
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
itcl::body mpjtensoranalysis::view_extract_roi { exts image } {
    
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
#  PRIVATE CALLBACK: Invoked upon a change in bundle material properties
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::material_dialog { base } {

    global amb_k
    global diff_k
    global spec_k
    global spec_power

    set amb_k $global_list(mat_ambk)
    set diff_k $global_list(mat_diffk)
    set spec_k $global_list(mat_speck)
    set spec_power $global_list(mat_specp)

    set w .chooseMaterial
    toplevel $w

    set frame0 [ frame $w.frame0 ]
    pack $frame0 -side top -fill x -padx 10 -pady 10 -expand false

    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -side top -fill x -expand t


    ### ambient
    iwidgets::entryfield $subframe0.mat_ambk \
    -width 5 \
    -validate real \
    -labeltext "Ambient K:" \
    -textvariable amb_k
    pack $subframe0.mat_ambk -side right -padx 5 -pady 5  

 
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill x -expand t

    ### diffuse
    iwidgets::entryfield $subframe1.mat_diffk \
    -width 5 \
    -validate real \
    -labeltext "Diffuse K:" \
    -textvariable diff_k
    pack $subframe1.mat_diffk -side right -padx 5 -pady 5  


    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -side top -fill x -expand t

    ### specular
    iwidgets::entryfield $subframe2.mat_speck \
    -width 5 \
    -validate real \
    -labeltext "Specular K:" \
    -textvariable spec_k
    pack $subframe2.mat_speck -side right -padx 5 -pady 5  

    ### specular power
    set subframe3 [ frame $frame0.subframe3 ]
    pack $subframe3 -side top -fill x -expand t

    iwidgets::entryfield $subframe3.mat_specpower \
    -width 5 \
    -validate real \
    -labeltext "Specular power:" \
    -textvariable spec_power
    pack $subframe3.mat_specpower -side right -padx 5 -pady 5  


    set separator [ frame $frame0.sep -height 2 -borderwidth 1 -relief sunken ]
    pack $separator -side top -fill x -pady 10

    set bottom [ frame $frame0.bottom ]
    pack $bottom -side top -fill x


    global mat_confirm

    ## Ok and cancel buttons
    button $bottom.ok -text "OK" -command { set mat_confirm 1 }
    pack $bottom.ok -side left -padx 5 -pady 5 

    button $bottom.cancel -text "Cancel" -command { set mat_confirm 0 } 
    pack $bottom.cancel -side left -padx 5 -pady 5

    focus $bottom.ok

    bind modal_dialog <ButtonPress> { wm deiconify %W; raise %W }
    if { [lsearch [ bindtags $w ] modal_dialog ] < 0 } {
        bindtags $w [ linsert [ bindtags $w ] 0 modal_dialog ]
    }     

    set x [ expr [ winfo rootx $base ] +50 ]
    set y [ expr [ winfo rooty $base ] +50 ]
    wm geometry $w "+$x+$y"

    wm title $w "Material"
    wm resizable $w 0 0
    wm deiconify $w
    wm protocol $w WM_DELETE_WINDOW "$bottom.cancel invoke"

    grab set $w
    vwait mat_confirm
    grab release $w

    if { $mat_confirm } {

        set global_list(mat_ambk) $amb_k
        set global_list(mat_diffk) $diff_k
        set global_list(mat_speck) $spec_k
        set global_list(mat_specp) $spec_power

    }

    #catch { tkwait visibility $w }
    #catch { grab $w }
    
    wm withdraw $w

    destroy $w

    return $mat_confirm
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create tensor glyphs
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_create_tensor_polydata { tensor scalar glyph where } {

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
    
    ## is it rgb or default luk mapping ?
    set maptype $global_list(view_tensor_mapping)

    ## threshold resulting dataset 
    set thresh $global_list(vtk_tensor_threshold,$where)
    $thresh SetInput [ $merge GetOutput ]
    if { $maptype == "rgb" } {
    $thresh ThresholdBetween 0 255
    } else { 
    if { $global_list(view_tensor_between) == 1 } {
       $thresh ThresholdBetween $global_list(view_tensor_from) $global_list(view_tensor_to)
        } else {
       $thresh ThresholdByUpper $global_list(view_tensor_from)
        }
    }
    $thresh Update

    ## create glyph field
    set glypher $global_list(vtk_tensor_glypher,$where)
    $glypher SetInput [ $thresh GetOutput ]

    if { $glyph == "ellipsoid" } {
            $glypher SetGlyphTypeToEllipsoid
            #$glypher ThreeGlyphsOff
    } elseif { $glyph == "superquadric" } {
            $glypher SetGlyphTypeToSuperQuadric
            #$glypher ThreeGlyphsOff
    } elseif { $glyph == "toroid" } {
            $glypher SetGlyphTypeToToroid
            #$glypher ThreeGlyphsOn
    } elseif { $glyph == "toroidext" } {
            $glypher SetGlyphTypeToToroidExtended
            #$glypher ThreeGlyphsOn             
    } elseif { $glyph == "supertoroid" } {
            $glypher SetGlyphTypeToSuperToroid      
            #$glypher ThreeGlyphsOn
    } elseif { $glyph == "supertoroidext" } {
            $glypher SetGlyphTypeToSuperToroidExtended      
            #$glypher ThreeGlyphsOn
    } elseif { $glyph == "supertoroidext2" } {
            $glypher SetGlyphTypeToSuperToroidExtended2     
            #$glypher ThreeGlyphsOn     
            puts "here" 
        } elseif { $glyph == "supertoroidext3" } {
                    $glypher SetGlyphTypeToSuperToroidExtended3
                        puts "ext3"
    } elseif { $glyph == "supertoroidext4" } {
                    $glypher SetGlyphTypeToSuperToroidExtended4
                        puts "ext4"
    } else {        
            $glypher SetGlyphTypeToSource
            #$glypher ThreeGlyphsOff
            $glypher SetSource [ $glyph GetOutput ]
    }       
    

    set filtergamma [ $widget_list(view_toroid_gammamap) index \
        [ $widget_list(view_toroid_gammamap) get ] ] 
    set gammamap [ [ lindex $global_list(result_list) $filtergamma ] GetImage ]
        
    ## current gammamap2 filter
    set filtergamma2 [ $widget_list(view_toroid_gammamap2) index \
        [ $widget_list(view_toroid_gammamap2) get ] ] 
    set gammamap2 [ [ lindex $global_list(result_list) $filtergamma2 ] GetImage ]

    ##$glypher SetSource [ $glyph GetOutput ]

    ##$glypher SymmetricOn
    $glypher SetEigenvalueThreshold $global_list(eigen_threshval) 
    #$glypher ExtractEigenvaluesOff
    $glypher SetScaleFactor $global_list(view_tensor_scale)
    $glypher SetClampScaling $global_list(view_tensor_clampscale)
    $glypher SetMaxScaleFactor $global_list(view_tensor_maxscale)
    $glypher SetUResolution $global_list(view_tensor_resol)
    $glypher SetVResolution $global_list(view_tensor_resol)
    $glypher SetSmoothness $global_list(toroidal_gamma)
    $glypher SetUseConstantSmoothness \
    [ expr 1 - $global_list(toroidal_usegammamap) ]
    $glypher SetSmoothness2 $global_list(toroidal_gamma2)
    $glypher SetUseConstantSmoothness2 \
    [ expr 1 - $global_list(toroidal_usegammamap2) ]
    $glypher SetSmoothnessImage $gammamap
    $glypher SetSmoothnessImage2 $gammamap2
    $glypher SetInequality $global_list(toroidal_inequality)
    $glypher SetSwapPowers $global_list(toroidal_swappowers)
    $glypher SetTubeness $global_list(toroidal_xi)
    $glypher Update
 
    ## compute surface normals
    set glyphNormals [ vtkPolyDataNormals [ pxvtable::vnewobj ] ]
    $glyphNormals SetInput [ $glypher GetOutput ]
    $glyphNormals Update

    set polydata [ vtkPolyData [ pxvtable::vnewobj ] ]
    $polydata ShallowCopy [ $glyphNormals GetOutput ]
    $glyphNormals Delete

    return $polydata
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create tensor glyphs
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_create_tensor_glyph { tensor scalar glyph where } {

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
    
    ## is it rgb or default luk mapping ?
    set maptype $global_list(view_tensor_mapping)

    ## threshold resulting dataset 
    set thresh $global_list(vtk_tensor_threshold,$where)
    $thresh SetInput [ $merge GetOutput ]
    if { $maptype == "rgb" } {
	$thresh ThresholdBetween 0 255
    } else { 
	if { $global_list(view_tensor_between) == 1 } {
	   $thresh ThresholdBetween $global_list(view_tensor_from) $global_list(view_tensor_to)
        } else {
	   $thresh ThresholdByUpper $global_list(view_tensor_from)
        }
    }
    $thresh Update

    ## create glyph field
    set glypher $global_list(vtk_tensor_glypher,$where)
    $glypher SetInput [ $thresh GetOutput ]

    if { $glyph == "ellipsoid" } {
			$glypher SetGlyphTypeToEllipsoid
			#$glypher ThreeGlyphsOff
	} elseif { $glyph == "superquadric" } {
			$glypher SetGlyphTypeToSuperQuadric
			#$glypher ThreeGlyphsOff
	} elseif { $glyph == "toroid" } {
			$glypher SetGlyphTypeToToroid
			#$glypher ThreeGlyphsOn
	} elseif { $glyph == "toroidext" } {
			$glypher SetGlyphTypeToToroidExtended
			#$glypher ThreeGlyphsOn				
	} elseif { $glyph == "supertoroid" } {
			$glypher SetGlyphTypeToSuperToroid		
			#$glypher ThreeGlyphsOn
	} elseif { $glyph == "supertoroidext" } {
			$glypher SetGlyphTypeToSuperToroidExtended		
			#$glypher ThreeGlyphsOn
	} elseif { $glyph == "supertoroidext2" } {
			$glypher SetGlyphTypeToSuperToroidExtended2		
			#$glypher ThreeGlyphsOn		
			puts "here"	
        } elseif { $glyph == "supertoroidext3" } {
	                $glypher SetGlyphTypeToSuperToroidExtended3
                        puts "ext3"
	} elseif { $glyph == "supertoroidext4" } {
	                $glypher SetGlyphTypeToSuperToroidExtended4
                        puts "ext4"
	} else {		
			$glypher SetGlyphTypeToSource
			#$glypher ThreeGlyphsOff
			$glypher SetSource [ $glyph GetOutput ]
	}		
    

    set filtergamma [ $widget_list(view_toroid_gammamap) index \
	    [ $widget_list(view_toroid_gammamap) get ] ] 
    set gammamap [ [ lindex $global_list(result_list) $filtergamma ] GetImage ]
        
    ## current gammamap2 filter
    set filtergamma2 [ $widget_list(view_toroid_gammamap2) index \
	    [ $widget_list(view_toroid_gammamap2) get ] ] 
    set gammamap2 [ [ lindex $global_list(result_list) $filtergamma2 ] GetImage ]

    ##$glypher SetSource [ $glyph GetOutput ]

    ##$glypher SymmetricOn
    $glypher SetEigenvalueThreshold $global_list(eigen_threshval) 
    #$glypher ExtractEigenvaluesOff
    $glypher SetScaleFactor $global_list(view_tensor_scale)
    $glypher SetClampScaling $global_list(view_tensor_clampscale)
    $glypher SetMaxScaleFactor $global_list(view_tensor_maxscale)
    $glypher SetUResolution $global_list(view_tensor_resol)
    $glypher SetVResolution $global_list(view_tensor_resol)
    $glypher SetSmoothness $global_list(toroidal_gamma)
    $glypher SetUseConstantSmoothness \
	[ expr 1 - $global_list(toroidal_usegammamap) ]
    $glypher SetSmoothness2 $global_list(toroidal_gamma2)
    $glypher SetUseConstantSmoothness2 \
	[ expr 1 - $global_list(toroidal_usegammamap2) ]
    $glypher SetSmoothnessImage $gammamap
    $glypher SetSmoothnessImage2 $gammamap2
    $glypher SetInequality $global_list(toroidal_inequality)
    $glypher SetSwapPowers $global_list(toroidal_swappowers)
    $glypher SetTubeness $global_list(toroidal_xi)
    $glypher Update
 
    ## compute surface normals
    set glyphNormals [ vtkPolyDataNormals [ pxvtable::vnewobj ] ]
    $glyphNormals SetInput [ $glypher GetOutput ]

    ## set up glyph mapper
    set glyphMapper $global_list(vtk_tensor_mapper,$where)
    ##set glyphMapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
    $glyphMapper SetInput [ $glyphNormals GetOutput ]
    $glyphMapper SetLookupTable [ $widget_list(view_tensorluk) GetLookupTable ]
    $glyphMapper UseLookupTableScalarRangeOn
    if { $maptype == "rgb" } {
       $glyphMapper SetColorModeToDefault
    } else {
       $glyphMapper SetColorModeToMapScalars
    }
    $glyphMapper ScalarVisibilityOn

    $glyphNormals Delete

    ## create glyph actor
    set glyphActor [ vtkActor [ pxvtable::vnewobj ] ]
    $glyphActor SetMapper $glyphMapper

    [$glyphActor GetProperty] SetAmbient $global_list(tensor_mat_ambk)
    [$glyphActor GetProperty] SetDiffuse $global_list(tensor_mat_diffk)
    [$glyphActor GetProperty] SetSpecular $global_list(tensor_mat_speck)
    [$glyphActor GetProperty] SetSpecularPower $global_list(tensor_mat_specp)
    
    ##$glyphMapper Delete
    
    return $glyphActor
}

#-------------------------------------------------------------------------------------------
#  Create eigenvector glyphs
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_create_eigenvector_polydata { index vectors scalar glyph where } {

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
    
    ## is it rgb or default luk mapping ?
    set maptype $global_list(view_eigen_mapping)

    ## threshold resulting dataset 
    set thresh $global_list(vtk_eigen_threshold,$index,$where)
    $thresh SetInput [ $merge GetOutput ]
    if { $maptype == "rgb" } {
    $thresh ThresholdBetween 0 255
    } else { 
    if { $global_list(view_eigen_between) == 1 } {
       $thresh ThresholdBetween $global_list(view_eigen_from) $global_list(view_eigen_to)
        } else {
       $thresh ThresholdByUpper $global_list(view_eigen_from)
        }
    }
    $thresh Update
    
    ## update tube glyph
    $global_list(glyph_tubesrc) SetRadius $global_list(view_eigen_tuberad)
    $global_list(glyph_tubesrc) SetHeight $global_list(view_eigen_tubelen)
    $global_list(glyph_tubesrc) SetResolution $global_list(view_eigen_tuberes)
    $global_list(glyph_arrowsrc) SetShaftResolution $global_list(view_eigen_tuberes)
    $global_list(glyph_arrowsrc) SetTipResolution $global_list(view_eigen_tuberes)

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

    set polydata [ vtkPolyData [ pxvtable::vnewobj ] ]
    $polydata ShallowCopy [ $glypher GetOutput ]
    
    return $polydata
}

#-------------------------------------------------------------------------------------------
#  Create eigenvector glyphs
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_create_eigenvector_glyph { index vectors scalar glyph where } {

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
    
    ## is it rgb or default luk mapping ?
    set maptype $global_list(view_eigen_mapping)

    ## threshold resulting dataset 
    set thresh $global_list(vtk_eigen_threshold,$index,$where)
    $thresh SetInput [ $merge GetOutput ]
    if { $maptype == "rgb" } {
	$thresh ThresholdBetween 0 255
    } else { 
	if { $global_list(view_eigen_between) == 1 } {
	   $thresh ThresholdBetween $global_list(view_eigen_from) $global_list(view_eigen_to)
        } else {
	   $thresh ThresholdByUpper $global_list(view_eigen_from)
        }
    }
    $thresh Update
    
    ## update tube glyph
    $global_list(glyph_tubesrc) SetRadius $global_list(view_eigen_tuberad)
    $global_list(glyph_tubesrc) SetHeight $global_list(view_eigen_tubelen)
    $global_list(glyph_tubesrc) SetResolution $global_list(view_eigen_tuberes)
    $global_list(glyph_arrowsrc) SetShaftResolution $global_list(view_eigen_tuberes)
    $global_list(glyph_arrowsrc) SetTipResolution $global_list(view_eigen_tuberes)

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
    set glyphMapper $global_list(vtk_eigen_mapper,$index,$where)
    ##set glyphMapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
    $glyphMapper SetInput [ $glypher GetOutput ]
    $glyphMapper SetLookupTable [ $widget_list(view_vectorluk) GetLookupTable ]
    $glyphMapper UseLookupTableScalarRangeOn
    if { $maptype == "rgb" } {
	$glyphMapper SetColorModeToDefault
    } else {
	$glyphMapper SetColorModeToMapScalars
    }
    $glyphMapper ScalarVisibilityOn

    ## create glyph actor
    set glyphActor [ vtkActor [ pxvtable::vnewobj ] ]
    $glyphActor SetMapper $glyphMapper

    [$glyphActor GetProperty] SetAmbient $global_list(eigen_mat_ambk)
    [$glyphActor GetProperty] SetDiffuse $global_list(eigen_mat_diffk)
    [$glyphActor GetProperty] SetSpecular $global_list(eigen_mat_speck)
    [$glyphActor GetProperty] SetSpecularPower $global_list(eigen_mat_specp)

    ##$glyphMapper Delete
    
    return $glyphActor
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in eigenvector material properties
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_eigen_material_cb { base } {

  set global_list(mat_ambk) $global_list(eigen_mat_ambk)
  set global_list(mat_diffk) $global_list(eigen_mat_diffk)
  set global_list(mat_speck) $global_list(eigen_mat_speck)
  set global_list(mat_specp) $global_list(eigen_mat_specp)

  set result [ material_dialog $base ]
  set actor -1

    if { $result } {

        set global_list(eigen_mat_ambk) $global_list(mat_ambk)
        set global_list(eigen_mat_diffk) $global_list(mat_diffk)
        set global_list(eigen_mat_speck) $global_list(mat_speck)
        set global_list(eigen_mat_specp) $global_list(mat_specp)

        for { set j 0 } { $j < 4 } { incr j } {
            set actor [lindex $global_list(actor_fast) $j ]
            if { $actor != "-1" } {
                [ $actor GetProperty] SetAmbient $global_list(eigen_mat_ambk)
                [ $actor GetProperty] SetDiffuse $global_list(eigen_mat_diffk)
                [ $actor GetProperty] SetSpecular $global_list(eigen_mat_speck)
                [ $actor GetProperty] SetSpecularPower $global_list(eigen_mat_specp)
            }
        }

        for { set j 0 } { $j < 4 } { incr j } {
            set actor [lindex $global_list(actor_med) $j ]
            if { $actor != "-1" } {
                [ $actor GetProperty] SetAmbient $global_list(eigen_mat_ambk)
                [ $actor GetProperty] SetDiffuse $global_list(eigen_mat_diffk)
                [ $actor GetProperty] SetSpecular $global_list(eigen_mat_speck)
                [ $actor GetProperty] SetSpecularPower $global_list(eigen_mat_specp)
            }
        }

        for { set j 0 } { $j < 4 } { incr j } {
            set actor [lindex $global_list(actor_slow) $j ]
            if { $actor != "-1" } {
                [ $actor GetProperty] SetAmbient $global_list(eigen_mat_ambk)
                [ $actor GetProperty] SetDiffuse $global_list(eigen_mat_diffk)
                [ $actor GetProperty] SetSpecular $global_list(eigen_mat_speck)
                [ $actor GetProperty] SetSpecularPower $global_list(eigen_mat_specp)
            }
        }

        [ $parent GetViewer ] UpdateDisplay

    }

}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in eigenvector material properties
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_tensor_material_cb { base } {

  set global_list(mat_ambk) $global_list(tensor_mat_ambk)
  set global_list(mat_diffk) $global_list(tensor_mat_diffk)
  set global_list(mat_speck) $global_list(tensor_mat_speck)
  set global_list(mat_specp) $global_list(tensor_mat_specp)

  set result [ material_dialog $base ]
  set actor -1

    if { $result } {

        set global_list(tensor_mat_ambk) $global_list(mat_ambk)
        set global_list(tensor_mat_diffk) $global_list(mat_diffk)
        set global_list(tensor_mat_speck) $global_list(mat_speck)
        set global_list(tensor_mat_specp) $global_list(mat_specp)

        for { set j 0 } { $j < 4 } { incr j } {
            set actor [lindex $global_list(actor_tensor) $j ]
            if { $actor != "-1" } {
                [ $actor GetProperty] SetAmbient $global_list(tensor_mat_ambk)
                [ $actor GetProperty] SetDiffuse $global_list(tensor_mat_diffk)
                [ $actor GetProperty] SetSpecular $global_list(tensor_mat_speck)
                [ $actor GetProperty] SetSpecularPower $global_list(tensor_mat_specp)
            }
        }

        [ $parent GetViewer ] UpdateDisplay

    }

}

#-------------------------------------------------------------------------------------------
#  Toggle On/Off fast eigenvector visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_type_toggle_fast_eigenvector { } {                
    
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
itcl::body mpjtensoranalysis::view_type_toggle_med_eigenvector { } {                

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
itcl::body mpjtensoranalysis::view_type_toggle_slow_eigenvector { } {                

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
itcl::body mpjtensoranalysis::view_type_toggle_tensor { } {                

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
itcl::body mpjtensoranalysis::view_type_toggle_colorbar { } {
    
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
itcl::body mpjtensoranalysis::view_save_tensor { fdir } {
    
    set TENSOR 0
    set EIGENVECTORS 3

    ## get tensor information
    set tensor_image [ lindex $global_list(result_list) $TENSOR ]
    
    ## get current slice numbers
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set dims [ [ $tensor_image GetImage ] GetDimensions ]
    
    ## get current eigenvector scalar filter
    if { [ $widget_list(view_tensor_filter) get ] == "Directionality" } {
    set global_list(view_tensor_mapping) "rgb"
    } else {
        set global_list(view_tensor_mapping) "default"
    }

    ## get current eigenvector scalar filter
    set filter [ $widget_list(view_tensor_filter) index [ $widget_list(view_tensor_filter) get ] ]
    set tensor_filter [ lindex $global_list(result_list) $filter ] 

    ## get current glyph
    set cur_glyph [ $widget_list(view_tensor_glyph) index [ $widget_list(view_tensor_glyph) get ] ]
    set glyph_source [ lindex $global_list(glyph_list) [ expr $cur_glyph + 3 ] ]
    #    set glyph_source [ lindex $global_list(glyph_list) 3 ]
    
   if { $cur_glyph == 0 } {
        set glyph_source "ellipsoid"
    }
    
    if { $cur_glyph == 9 } {   
        set glyph_source "superquadric"
    }
    
    if { $cur_glyph == 10 } {
        set glyph_source "toroid"
    }
    
    if { $cur_glyph == 11 } {
        set glyph_source "toroidext"
    }
    
    if { $cur_glyph == 12 } {
        set glyph_source "supertoroid"
    }
    
    if { $cur_glyph == 13 } {
        set glyph_source "supertoroidext"
    }
    
    if { $cur_glyph == 14 } {
        set glyph_source "supertoroidext2"
    }
 
        if { $cur_glyph == 15 } {
        set glyph_source "supertoroidext3"
    }
 
        if { $cur_glyph == 16 } {
        set glyph_source "supertoroidext4"
    }

    ## for accumulating polydatas
    set accum [ vtkAppendPolyData [ pxvtable::vnewobj ] ]
    ## for accumulating polydatas

    if { $global_list(view_mode_index) < 4 } {
        
        ## plot on slices
        for { set i 0 } { $i < 3 } { incr i } {

        if { [expr ($global_list(view_mode_index) == $i) || \
              ($global_list(view_mode_index) == 3) ] } {
            
            set ext [ view_compute_roi_extents $i $dims $levels ]
            set tensors [ [ view_extract_roi $ext $tensor_image ] GetOutput ]
            set scalars [ [ view_extract_roi $ext $tensor_filter ] GetOutput ]
            
            set glyph [ view_create_tensor_polydata $tensors $scalars $glyph_source $i ]
            $accum AddInput $glyph
            $glyph Delete
        
            $tensors Delete
            $scalars Delete
              
        }
        
        }
    
    } else {
    
    ## plot on entire volume
    set tensors [ $tensor_image GetImage ]
    set scalars [ $tensor_filter GetImage ]

    ## create glyph
    set glyph [ view_create_tensor_polydata $tensors $scalars $glyph_source 3 ]
    $accum AddInput $glyph
    $glyph Delete
    
    }

    set glyphvtk [ vtkPolyDataWriter [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $glyphvtk "Saving tensor field as a vtk file ..."
    $glyphvtk SetInput [ $accum GetOutput ]
    $glyphvtk SetFileTypeToBinary
    $glyphvtk SetFileName [ file join $fdir "tensor.vtk" ]
    $glyphvtk Write

    $accum Delete
    $glyphvtk Delete
}


#-------------------------------------------------------------------------------------------
#  View tensors
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_create_tensor { } {
    
    set TENSOR 0
    set EIGENVECTORS 3

    ## get tensor information
    set tensor_image [ lindex $global_list(result_list) $TENSOR ]
    
    ## get current slice numbers
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set dims [ [ $tensor_image GetImage ] GetDimensions ]
    
    ## get current eigenvector scalar filter
    if { [ $widget_list(view_tensor_filter) get ] == "Directionality" } {
	set global_list(view_tensor_mapping) "rgb"
    } else {
        set global_list(view_tensor_mapping) "default"
    }

    ## get current eigenvector scalar filter
    set filter [ $widget_list(view_tensor_filter) index [ $widget_list(view_tensor_filter) get ] ]
    set tensor_filter [ lindex $global_list(result_list) $filter ] 

    ## get current glyph
    set cur_glyph [ $widget_list(view_tensor_glyph) index [ $widget_list(view_tensor_glyph) get ] ]
    set glyph_source [ lindex $global_list(glyph_list) [ expr $cur_glyph + 3 ] ]
    #    set glyph_source [ lindex $global_list(glyph_list) 3 ]
    
   if { $cur_glyph == 0 } {
		set glyph_source "ellipsoid"
	}
	
    if { $cur_glyph == 9 } {   
		set glyph_source "superquadric"
	}
	
	if { $cur_glyph == 10 } {
		set glyph_source "toroid"
	}
	
	if { $cur_glyph == 11 } {
		set glyph_source "toroidext"
	}
	
	if { $cur_glyph == 12 } {
		set glyph_source "supertoroid"
	}
	
	if { $cur_glyph == 13 } {
		set glyph_source "supertoroidext"
	}
	
	if { $cur_glyph == 14 } {
		set glyph_source "supertoroidext2"
	}
 
        if { $cur_glyph == 15 } {
		set glyph_source "supertoroidext3"
	}
 
        if { $cur_glyph == 16 } {
		set glyph_source "supertoroidext4"
	}

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
#  Save eigenvectors
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_save_eigenvector { fdir title index } {

     ## which: (0) view_type_fast, (1) view_type_med, (2) view_type_slow, (3) view_type_tensor
    set TENSOR 0
    set MASK 1
    set EIGENVALUES 2
    set EIGENVECTORS 3
        
    ## get eigenvalues and eigenvectors
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]

    ## get current slice numbers
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set dims [ [ $eigenvalues GetImage ] GetDimensions ]
    
    ## get current eigenvector scalar filter
    if { [ $widget_list(view_eigen_filter) get ] == "Directionality" } {
    set global_list(view_eigen_mapping) "rgb"
    } else {
        set global_list(view_eigen_mapping) "default"
    }

    set filter [ $widget_list(view_eigen_filter) index [ $widget_list(view_eigen_filter) get ] ]
    set eigenfilter [ lindex $global_list(result_list) $filter ] 
    
    ## get current glyph
    set cur_glyph [ $widget_list(view_eigen_glyph) index [ $widget_list(view_eigen_glyph) get ] ]
    set glyph_source [ lindex $global_list(glyph_list) $cur_glyph ]

    ## for accumulating polydatas
    set accum [ vtkAppendPolyData [ pxvtable::vnewobj ] ]
    ## for accumulating polydatas

    if { $global_list(view_mode_index) < 4 } {
    
    ## plot on slices
    for { set i 0 } { $i < 3 } { incr i } {
        
        if { [ expr ($global_list(view_mode_index) == $i) || \
               ($global_list(view_mode_index) == 3) ] } {
        
        set ext [ view_compute_roi_extents $i $dims $levels ]
        set vectors [ [ view_extract_roi $ext $eigenvectors ] GetOutput ]
        set scalars [ [ view_extract_roi $ext $eigenfilter ] GetOutput ]
        
        set glyph [ view_create_eigenvector_polydata $index $vectors $scalars $glyph_source $i ]
        $accum AddInput $glyph
        $glyph Delete
        
        $vectors Delete
        $scalars Delete
        
        }
        
    }
    
    } else {

        ## plot on entire volume
        set vectors [ $eigenvectors GetImage ]
        set scalars [ $eigenfilter GetImage ]

        ## create glyph
        set glyph [ view_create_eigenvector_polydata $index $vectors $scalars $glyph_source 3 ]
        $accum AddInput $glyph
        $glyph Delete
    
    }  

    set glyphvtk [ vtkPolyDataWriter [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $glyphvtk "Saving eigenvector field as a vtk file ..."
    $glyphvtk SetInput [ $accum GetOutput ]
    $glyphvtk SetFileTypeToBinary
    $glyphvtk SetFileName [ file join $fdir "eigenvector_$title.vtk" ]
    $glyphvtk Write

    $accum Delete
    $glyphvtk Delete

}


#-------------------------------------------------------------------------------------------
#  View eigenvectors
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_create_eigenvector { title index } {
    
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
    if { [ $widget_list(view_eigen_filter) get ] == "Directionality" } {
	set global_list(view_eigen_mapping) "rgb"
    } else {
        set global_list(view_eigen_mapping) "default"
    }

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
itcl::body mpjtensoranalysis::view_create_colorbar { } {
    
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
itcl::body mpjtensoranalysis::view_mode_cb { } {
        
    set mode(XZ-plane) 0
    set mode(YZ-plane) 1
    set mode(XY-plane) 2
    set mode(3-slice) 3
    set mode(Volume) 4
    set mode(None) 5
    
    set global_list(view_mode_index) $mode($global_list(view_mode))

    view_clear_display

    view_update_display
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in tensor colormap selection
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_tensor_cmapcopy_cb { } {
    
    ## current tensor filter
    set cpfrom [ $widget_list(view_tensor_cmapcopy) index [ $widget_list(view_tensor_cmapcopy) get ] ]

    set global_list(view_tensor_cmapcopy) $cpfrom
    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon copy eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_tensor_cmapcopybtn_cb { } {
    
    if { $global_list(view_tensor_cmapcopy) == 0 } {
	$widget_list(view_tensorluk) Copy [ $widget_list(view_vectorluk) GetLookupTable ]
    } else {
	$widget_list(view_tensorluk) Copy [ $widget_list(view_resultluk) GetLookupTable ]
    }
        
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon copy eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_tensor_cmapresetbtn_cb { } {
    
    ## current tensor filter
    $widget_list(view_tensorluk) SetHueRange 0.5 0.0
    $widget_list(view_tensorluk) Update

}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon loading eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_tensor_cmaploadbtn_cb { } {   

    $widget_list(view_tensorluk) Load
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon saving eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_tensor_cmapsavebtn_cb { } {   

    $widget_list(view_tensorluk) SaveAs
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in tensor filter selection
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_tensor_filter_cb { } {
    
    set TENSOR 0
    
    ## current eigenvector filter
    set filter [ $widget_list(view_tensor_filter) index [ $widget_list(view_tensor_filter) get ] ]   
      ## get current eigenvector scalar filter
    if { [ $widget_list(view_tensor_filter) get ] == "Directionality" } {
	    set global_list(view_tensor_mapping) "rgb"
    } else {
            set global_list(view_tensor_mapping) "default"
    }

    set tensor_filter [ lindex $global_list(result_list) $filter ] 

    ## get eigenvalues and eigenvectors
    set tensor_image [ lindex $global_list(result_list) $TENSOR ]
    
    ## get current slice numbers
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set dims [ [ $tensor_image GetImage ] GetDimensions ]
    
    set result [ [ lindex $global_list(result_list) $filter ] GetImage ]
    set range [ [ [ $result GetPointData ] GetScalars ] GetRange ]
    
    ## set ranges
    set global_list(view_tensor_from) [ expr ([ lindex $range 1 ] - [ lindex $range 0 ]) / 2.0 ]
    set global_list(view_tensor_to) [ lindex $range 1 ] 

    if { $global_list(view_mode_index) < 4 } {	
	
	## plot on slices
	for { set i 0 } { $i < 3 } { incr i } {
	    
	    if { [expr ($global_list(view_mode_index) == $i ) || \
		      ($global_list(view_mode_index) == 3) ] } {		
		
		set ext [ view_compute_roi_extents $i $dims $levels ]
		set scalar [ [ view_extract_roi $ext $tensor_filter ] GetOutput ]
		
		set merge $global_list(vtk_tensor_merge,$i)
		$merge SetScalars $scalar

		set mapper $global_list(vtk_tensor_mapper,$i)
		if { $global_list(view_tensor_mapping) == "rgb" } {
		       $mapper SetColorModeToDefault
		} else {
		       $mapper SetColorModeToMapScalars
		}
		
		$scalar Delete	    	    
	    }
	}	
	
    } else {
	
	## plot on entire volume
	set scalar [ [ lindex $global_list(result_list) $filter ] GetImage ]
	
	set merge $global_list(vtk_tensor_merge,3)
	$merge SetScalars $scalar

	set mapper $global_list(vtk_tensor_mapper,3)
	if { $global_list(view_tensor_mapping) == "rgb" } {
		$mapper SetColorModeToDefault
	    } else {
		$mapper SetColorModeToMapScalars
	    }
    }
    
    #set result [ [ lindex $global_list(result_list) $filter ] GetImage ]
    #set range [ [ [ $result GetPointData ] GetScalars ] GetRange ]
    $widget_list(view_tensorluk) SetTableRange [ lindex $range 0 ] [ lindex $range 1 ]
#    $widget_list(view_tensorluk) Update
    
    ## update scalar bar    
    if { $global_list(view_colorbar) == "Tensor" } {
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
itcl::body mpjtensoranalysis::view_eigen_cmapcopy_cb { } {
    
    ## current eigenvector filter
    set cpfrom [ $widget_list(view_eigen_cmapcopy) index [ $widget_list(view_eigen_cmapcopy) get ] ]

    set global_list(view_eigen_cmapcopy) $cpfrom
    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon copy eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_eigen_cmapcopybtn_cb { } {
    
    if { $global_list(view_eigen_cmapcopy) == 0 } {
	$widget_list(view_vectorluk) Copy [ $widget_list(view_tensorluk) GetLookupTable ]
    } else {
	$widget_list(view_vectorluk) Copy [ $widget_list(view_resultluk) GetLookupTable ]
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon resetting eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_eigen_cmapresetbtn_cb { } {
    
    ## current tensor filter
    $widget_list(view_vectorluk) SetHueRange 0.75 0.0
    $widget_list(view_vectorluk) Update

}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon loading eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_eigen_cmaploadbtn_cb { } {

    $widget_list(view_vectorluk) Load
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon saving eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_eigen_cmapsavebtn_cb { } {

    $widget_list(view_vectorluk) SaveAs
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in vector filter selection
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_eigen_filter_cb { } {    
      
    set EIGENVALUES 2
    set EIGENVECTORS 3

    ## current eigenvector filter
    set filter [ $widget_list(view_eigen_filter) index [ $widget_list(view_eigen_filter) get ] ]   
    
       ## get current eigenvector scalar filter
    if { [ $widget_list(view_eigen_filter) get ] == "Directionality" } {
	    set global_list(view_eigen_mapping) "rgb"
    } else {
            set global_list(view_eigen_mapping) "default"
    }
	
    ## get eigenvalues and eigenvectors
    set eigenvalues [ lindex $global_list(result_list) $EIGENVALUES ]
    set eigenvectors [ lindex $global_list(result_list) $EIGENVECTORS ]

    ## get current slice numbers
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set dims [ [ $eigenvalues GetImage ] GetDimensions ]

    set result [ [ lindex $global_list(result_list) $filter ] GetImage ]
    set range [ [ [ $result GetPointData ] GetScalars ] GetRange ]
    
    ## set ranges
    set global_list(view_eigen_from) [ expr ([ lindex $range 1 ] - [ lindex $range 0 ]) / 2.0 ]
    set global_list(view_eigen_to) [ lindex $range 1 ] 

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

		    set mapper $global_list(vtk_eigen_mapper,$j,$i)
		    if { $global_list(view_eigen_mapping) == "rgb" } {
		       $mapper SetColorModeToDefault
		    } else {
		       $mapper SetColorModeToMapScalars
		    }

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

	    set mapper $global_list(vtk_eigen_mapper,$j,3)
	    if { $global_list(view_eigen_mapping) == "rgb" } {
		 $mapper SetColorModeToDefault
	    } else {
		 $mapper SetColorModeToMapScalars
	    }
	}		
    }

    $widget_list(view_vectorluk) SetTableRange [ lindex $range 0 ] [ lindex $range 1 ]
#    $widget_list(view_vectorluk) Modified
    $widget_list(view_vectorluk) Update

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
itcl::body mpjtensoranalysis::view_tensor_fromto_cb { } {
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
itcl::body mpjtensoranalysis::view_eigen_tubesrc_cb { } {

	$global_list(glyph_tubesrc) SetRadius $global_list(view_eigen_tuberad)
	$global_list(glyph_tubesrc) SetHeight $global_list(view_eigen_tubelen)
    $global_list(glyph_tubesrc) SetResolution $global_list(view_eigen_tuberes)
	$global_list(glyph_arrowsrc) SetShaftResolution $global_list(view_eigen_tuberes)
    $global_list(glyph_arrowsrc) SetTipResolution $global_list(view_eigen_tuberes)

	[ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in threshold values
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_eigen_fromto_cb { } {
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
itcl::body mpjtensoranalysis::view_eigen_glyph_cb { } {    
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
#  PRIVATE CALLBACK: Invoked upon change in glyph resolution
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_tensor_gamma_xi_cb { } {
    
    ## current gammamap filter
    set filtergamma [ $widget_list(view_toroid_gammamap) index \
	[ $widget_list(view_toroid_gammamap) get ] ] 

    set gammamap [ [ lindex $global_list(result_list) $filtergamma ] GetImage ]
        
    ## current gammamap2 filter
    set filtergamma2 [ $widget_list(view_toroid_gammamap2) index \
	[ $widget_list(view_toroid_gammamap2) get ] ] 

    set gammamap2 [ [ lindex $global_list(result_list) $filtergamma2 ] GetImage ]

    for { set j 0 } { $j < 4 } { incr j } { 
	
      $global_list(vtk_tensor_glypher,$j) SetUseConstantSmoothness \
	  [expr 1 - $global_list(toroidal_usegammamap) ]
      $global_list(vtk_tensor_glypher,$j) SetUseConstantSmoothness2 \
	  [expr 1 - $global_list(toroidal_usegammamap2) ]

      $global_list(vtk_tensor_glypher,$j) SetSmoothnessImage $gammamap
      $global_list(vtk_tensor_glypher,$j) SetSmoothnessImage2 $gammamap2
        
      $global_list(vtk_tensor_glypher,$j) SetSmoothness $global_list(toroidal_gamma)
      $global_list(vtk_tensor_glypher,$j) SetSmoothness2 $global_list(toroidal_gamma2)

      $global_list(vtk_tensor_glypher,$j) SetInequality $global_list(toroidal_inequality)
      $global_list(vtk_tensor_glypher,$j) SetSwapPowers $global_list(toroidal_swappowers)
      $global_list(vtk_tensor_glypher,$j) SetTubeness $global_list(toroidal_xi)
  }	
  
  [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in glyph resolution
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_tensor_resol_cb { } {

  for { set j 0 } { $j < 4 } { incr j } { 
	$global_list(vtk_tensor_glypher,$j) SetUResolution $global_list(view_tensor_resol)
	$global_list(vtk_tensor_glypher,$j) SetVResolution $global_list(view_tensor_resol)
  }	

  [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in glyph type
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_tensor_glyph_cb { } {    
    set index [ $widget_list(view_tensor_glyph) index [ $widget_list(view_tensor_glyph) get ] ]
    set glyph [ lindex $global_list(glyph_list) [expr $index + 3 ] ]
    for { set j 0 } { $j < 4 } { incr j } { 
	if { $index == 0 } {
		$global_list(vtk_tensor_glypher,$j) SetGlyphTypeToEllipsoid
	    } elseif { $index == 9 } {
		$global_list(vtk_tensor_glypher,$j) SetGlyphTypeToSuperQuadric			
	    } elseif { $index == 10 } {
		$global_list(vtk_tensor_glypher,$j) SetGlyphTypeToToroid
	    } elseif { $index == 11 } {
		$global_list(vtk_tensor_glypher,$j) SetGlyphTypeToToroidExtended
	    } elseif { $index == 12 } {
		$global_list(vtk_tensor_glypher,$j) SetGlyphTypeToSuperToroid		
	    } elseif { $index == 13 } {
		$global_list(vtk_tensor_glypher,$j) SetGlyphTypeToSuperToroidExtended
	    } elseif { $index == 14 } {
		$global_list(vtk_tensor_glypher,$j) SetGlyphTypeToSuperToroidExtended2
            } elseif { $index == 15 } {
                $global_list(vtk_tensor_glypher,$j) SetGlyphTypeToSuperToroidExtended3
	    } elseif { $index == 16 } {
                $global_list(vtk_tensor_glypher,$j) SetGlyphTypeToSuperToroidExtended4
	    } else {		
		$global_list(vtk_tensor_glypher,$j) SetGlyphTypeToSource
			#$global_list(vtk_tensor_glypher,$j) ThreeGlyphsOff
		$global_list(vtk_tensor_glypher,$j) SetSource [ $glyph GetOutput ]
	    }		
##	$global_list(vtk_tensor_glypher,$j) SetSource [ $glyph GetOutput ]
    }
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in threshold values
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_tensor_maxscale_cb { } {
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
itcl::body mpjtensoranalysis::view_eigen_maxscale_cb { } {
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
itcl::body mpjtensoranalysis::view_colorbar_cb { } {

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
#  PRIVATE CALLBACK: Toggles display auto-update
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_toggle_autoupdate { } {
    
    if { $global_list(view_autoupdate) } {
	
        eval "$parent configure -mousecallback \"$this view_update_crosshair_cb\""
	
	view_update_display_cb 
	
    } else {
	
	eval "$parent configure -mousecallback 0"
	
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC CALLBACK: Invoked upon an crosshair update from viewer
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_update_crosshair_cb { args } {

    set mousebutton [ lindex $args 1 ]
    set mousestate  [ lindex $args 2 ]

    if { $global_list(view_mode_index) < 4 } {

         ## get last clicked point in the viewer
	 set lv [ lindex [ [ $parent GetViewer ] GetLastClickedPointScaled ] 0 ]
	    
         ##puts stderr "mouse = $mousebutton / $mousestate , point = $lv "
	    
	 if { $lv != -1 } {	    

	     view_clear_display
		 
	     view_update_display
	 }
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC CALLBACK: Invoked upon an update button press
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_update_display_cb { } { 
    
       view_clear_display

       view_update_display
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in colormaps for result images
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_results_colormap_cb { } {

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
#  PRIVATE CALLBACK: Invoked upon selection of tensor direction coloring
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::view_direction_update_cb { } {
    
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

    set imctmap [ lindex $global_list(result_list) $DEC_MAP ]
    $imctmap ShallowCopyImage [ $imct GetOutput ]    
    $imctmap CopyImageHeader [ $tensor GetImageHeader ]

    if { $bis_orientationimage == 0 } {
	set bis_orientationimage [ [ pxitclimage \#auto ] GetThisPointer ]
    }
    $bis_orientationimage ShallowCopyImage [ $imct GetOutput ]    
    $bis_orientationimage CopyImageHeader [ $tensor GetImageHeader ]


    $parent SetImageFromObject	[ $imctmap GetThisPointer ] $this
    $imct Delete  

    return 1
}

#-------------------------------------------------------------------------------------------
#  Create tensor control
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::CreateTensorControl { base } {
    
    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Tensor image"
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
			-text "Estimate from tensor" \
			-variable [ itcl::scope global_list(mask_auto) ] \
			-command [ itcl::code $this mask_autochanged_cb ] ]
    pack $mask_auto -side left -fill both -padx 5 -pady 2
    
    set mask_threshval [ iwidgets::entryfield $subframe0.mask_threshval \
			     -labeltext "Threshold tensor at:" \
			     -validate real -width 5  \
			     -textvariable [ itcl::scope global_list(mask_threshval) ] ]
    pack $mask_threshval -side right -pady 5 -padx 5
    set widget_list(mask_threshval) $mask_threshval

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

    set subframe2 [ frame $frame1.subframe2 ]
    pack $subframe2 -side top -fill both -expand f -pady 0

    set mask_negative [ checkbutton $subframe2.mask_negative \
			    -text "Mask out regions with negative eigenvalues" \
			    -variable [ itcl::scope global_list(mask_negative) ] ]
    pack $mask_negative -side left -fill both -padx 5 -pady 5
 
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

    set stat_normalize [ checkbutton $frame2.stat_normalize \
	-text "Normalize statistics" \
	-variable [ itcl::scope global_list(stat_normalize) ] ]
    pack $stat_normalize -side right -fill both -padx 5 -pady 5
}

#-------------------------------------------------------------------------------------------
#  Create toroidal control
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::CreateToroidalControl { base } {

  iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Toroidal maps"
    pack $base.frame0 -fill both -expand t -pady 5
    
    set frame0 [ $base.frame0 childsite ]
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 0
        
    set eigen_threshval [ iwidgets::entryfield $subframe1.eigen_threshval \
			     -labeltext "Eigenvalue Equality Epsilon:" \
			     -validate real -width 7  \
			     -textvariable [ itcl::scope global_list(eigen_threshval) ] ]
    pack $eigen_threshval -side left -pady 5 -padx 5
    set widget_list(eigen_threshval) $eigen_threshval
    
    #set subframe2 [ frame $frame0.subframe2 ]
    #pack $subframe2 -side top -fill both -expand f -pady 0
    
    #iwidgets::entryfield $subframe2.toroidal_top \
	#-width 5 \
	#-validate real \
	#-labeltext "Volume Normalization   Min %: " \
	#-textvariable [ itcl::scope global_list(toroidal_bot) ]
    #pack $subframe2.toroidal_top -side left -fill x -padx 5 -pady 5
    
    #iwidgets::entryfield $subframe2.toroidal_bot \
	#-width 5 \
	#-validate real \
	#-labeltext "Max %: " \
	#-textvariable [ itcl::scope global_list(toroidal_top) ]
    #pack $subframe2.toroidal_bot -side left -fill x -padx 5 -pady 5
 
    set subframe3 [ frame $frame0.subframe3 ]
    pack $subframe3 -side top -fill both -expand f -pady 0
        
    set toroidal_dissoc_filter \
	[ iwidgets::optionmenu $subframe3.toroidal_dissoc_filter \
	      -labeltext "Modulate classification by:" ]
	$toroidal_dissoc_filter insert 0 "Nothing"
    for { set i 0 } { $i < [expr [ llength $global_list(result_title) ] -1 ] } { incr i } {
	$toroidal_dissoc_filter insert [expr $i + 1] [ lindex [ lindex $global_list(result_title) $i ] 0 ]
    }
    pack $toroidal_dissoc_filter -side left -padx 5
    set widget_list(toroidal_dissoc_filter) $toroidal_dissoc_filter
    $toroidal_dissoc_filter select 0

    button $subframe3.calculate -text "Calculate" -command [ itcl::code $this result_createtoroidalmaps ]  
    pack $subframe3.calculate -side left -fill x -expand f -padx 5 -pady 10  
    
}

#-------------------------------------------------------------------------------------------
#  Create transform control
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::CreateTransformControl { base } {
    
    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Tensor transformation"
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

   set subframe3 [ frame $frame0.subframe3 ]
    pack $subframe3 -side top -fill both -expand f -pady 0
    
    set fiber_angle_rep \
	[ iwidgets::optionmenu $subframe3.fiber_angle_rep \
	      -labeltext "Fiber angle representation:" ]
	$fiber_angle_rep insert 0 " Absolute 0 to 90"
	$fiber_angle_rep insert 1 " Relative -90 to +90"
	$fiber_angle_rep insert 2 " Relative +90 to -90"
	$fiber_angle_rep insert 3 " Full 0 to 180"
    pack $fiber_angle_rep -side left -padx 5
    set widget_list(fiber_angle_rep) $fiber_angle_rep
    $fiber_angle_rep select 0
    
    ### apex point
    set subframe4 [ frame $frame0.subframe4 ]
    pack $subframe4 -side top -fill both -expand f -pady 7
    
    iwidgets::entryfield $subframe4.apex_px \
	-width 5 \
	-validate integer \
	-labeltext "Apex point  i:" \
	-textvariable [ itcl::scope global_list(pie_apexpx) ]
    pack $subframe4.apex_px -side left -fill x -padx 5 -pady 5
    
    iwidgets::entryfield $subframe4.apex_py \
	-width 5 \
	-validate integer \
	-labeltext "j:" \
	-textvariable [ itcl::scope global_list(pie_apexpy) ]
    pack $subframe4.apex_py -side left -fill x -pady 5
    
    iwidgets::entryfield $subframe4.apex_pz \
	-width 5 \
	-validate integer \
	-labeltext "k:" \
	-textvariable [ itcl::scope global_list(pie_apexpz) ]
    pack $subframe4.apex_pz -side left -fill x -pady 5    
    
    iwidgets::entryfield $subframe4.anchorapex_px \
	-width 5 \
	-validate integer \
	-labeltext "   Anchor point  i:" \
	-textvariable [ itcl::scope global_list(pie_anchorpx) ]
    pack $subframe4.anchorapex_px -side left -fill x -padx 5 -pady 5
    
    iwidgets::entryfield $subframe4.anchorapex_py \
	-width 5 \
	-validate integer \
	-labeltext "j:" \
	-textvariable [ itcl::scope global_list(pie_anchorpy) ]
    pack $subframe4.anchorapex_py -side left -fill x -pady 5
    
    iwidgets::entryfield $subframe4.anchorapex_pz \
	-width 5 \
	-validate integer \
	-labeltext "k:" \
	-textvariable [ itcl::scope global_list(pie_anchorpz) ]
    pack $subframe4.anchorapex_pz -side left -fill x -pady 5

    ## base point
    set subframe5 [ frame $frame0.subframe5 ]
    pack $subframe5 -side top -fill both -expand f -pady 5
    
    iwidgets::entryfield $subframe5.base_px \
	-width 5 \
	-validate integer \
	-labeltext "Base point  i:" \
	-textvariable [ itcl::scope global_list(pie_basepx) ]
    pack $subframe5.base_px -side left -fill x -padx 5 -pady 5
    
    iwidgets::entryfield $subframe5.base_py \
	-width 5 \
	-validate integer \
	-labeltext "j:" \
	-textvariable [ itcl::scope global_list(pie_basepy) ]
    pack $subframe5.base_py -side left -fill x -pady 5
    
    iwidgets::entryfield $subframe5.base_pz \
	-width 5 \
	-validate integer \
	-labeltext "k:" \
	-textvariable [ itcl::scope global_list(pie_basepz) ]
    pack $subframe5.base_pz -side left -fill x -pady 5    
    	    
    ### anchor point for pie
    set subframe6 [ frame $frame0.subframe6 ]
    pack $subframe6 -side top -fill both -expand f -pady 5
            
    set fiber_useapexbase [ checkbutton $subframe5.fiber_useapexbase \
	-text "Use apex and base in fiber angle" \
	-variable [ itcl::scope global_list(fiber_useapexbase) ] ]
    pack $fiber_useapexbase -side left -padx 5 -pady 7
    set widget_list(fiber_useapexbase) $fiber_useapexbase

    iwidgets::entryfield $subframe6.pie_nslices \
	-width 5 \
	-validate integer \
	-labeltext "Number of slices:" \
	-textvariable [ itcl::scope global_list(pie_numberofslices) ]
    pack $subframe6.pie_nslices -side left -fill x -pady 5
   
	#iwidgets::entryfield $subframe6.pie_npies \
	#-width 5 \
	#-validate integer \
	#-labeltext "Number of pies (minimum=2):" \
	#-textvariable [ itcl::scope global_list(pie_numberofpies) ]
    #pack $subframe6.pie_npies -side left -fill x -pady 5
    
    
    set pie_create [ button $subframe6.pie_create \
			      -command [ itcl::code $this result_createpie ] \
			      -text "Create pie!" ]
    pack $pie_create -side left -fill both -padx 5 -pady 5     
}

#-------------------------------------------------------------------------------------------
#  Create results control
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::CreateResultsControl { base } {   

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
    $resultbbox add recalc -text "Recalculate" -command [ itcl::code $this result_createstats ]

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
itcl::body mpjtensoranalysis::CreateStatisticsControl { base } {

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
itcl::body mpjtensoranalysis::CreateEigenDisplayControl { base } {

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
    
    button $subframe0.view_eigen_material \
    -text "Material" \
    -command [ itcl::code $this view_eigen_material_cb $base ]
    pack $subframe0.view_eigen_material -side left -padx 10 -pady 5
    set widget_list(view_eigen_material) $subframe0.view_eigen_material


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
	-labeltext "Scale:" \
	-textvariable [ itcl::scope global_list(view_eigen_maxscale) ] \
	-command [ itcl::code $this view_eigen_maxscale_cb ]
    pack $subframe1.view_eigen_maxscale -side left 
    
    #    checkbutton $subframe1.view_eigen_flip \
	#	-text "Flip"  \
	#	-variable [ itcl::scope global_list(view_eigen_flip) ] \
	#	-command [ itcl::code $this view_update_display_cb ]
    #    pack $subframe1.view_eigen_flip -side left -fill x -padx 0
    
       iwidgets::entryfield $subframe1.view_eigen_tuberad \
	-width 5 \
	-validate real \
	-labeltext "Radius:" \
	-textvariable [ itcl::scope global_list(view_eigen_tuberad) ] \
	-command [ itcl::code $this view_eigen_tubesrc_cb ]
    pack $subframe1.view_eigen_tuberad -side left
    
    iwidgets::entryfield $subframe1.view_eigen_tubelen \
	-width 5 \
	-validate real \
	-labeltext "Length:" \
	-textvariable [ itcl::scope global_list(view_eigen_tubelen) ] \
	-command [ itcl::code $this view_eigen_tubesrc_cb ]
    pack $subframe1.view_eigen_tubelen -side left

    iwidgets::entryfield $subframe1.view_eigen_tuberes \
    -width 5 \
    -validate real \
    -labeltext "Resolution:" \
    -textvariable [ itcl::scope global_list(view_eigen_tuberes) ] \
    -command [ itcl::code $this view_eigen_tubesrc_cb ]
    pack $subframe1.view_eigen_tuberes -side left

    set subframe2 [ frame $base.subframe2 -relief flat ]
    pack $subframe2 -side left -padx 5 -pady 5 
    
    update idletasks

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
    $view_eigen_cmap_copy insert end "Tensors"
    $view_eigen_cmap_copy insert end "Results"
    
    set widget_list(view_eigen_cmapcopy) $view_eigen_cmap_copy

    set view_eigen_cmap_copybtn [ button [ $vector_luk GetMenuBar ].copybtn -text "Copy" \
				      -command [ itcl::code $this view_eigen_cmapcopybtn_cb ] ]
    pack $view_eigen_cmap_copybtn -side right -padx 0 -pady 5 
}

#-------------------------------------------------------------------------------------------
#  Create tensor display control
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::CreateTensorDisplayControl { base } {

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
    
    button $subframe0.view_tensor_material \
    -text "Material" \
    -command [ itcl::code $this view_tensor_material_cb $base ]
    pack $subframe0.view_tensor_material -side left -padx 10 -pady 5
    set widget_list(view_tensor_material) $subframe0.view_tensor_material

    set subframe1 [ frame $base.subframe1 ]
    pack $subframe1 -side top -fill both -padx 5 

    set view_tensor_glyph [ iwidgets::optionmenu $subframe1.view_tensor_glyph \
				-command [ itcl::code $this view_tensor_glyph_cb ] \
				-labeltext "View as:" ]
    pack $view_tensor_glyph -side left -padx 5
    $view_tensor_glyph insert end "Ellipsoids"
    $view_tensor_glyph insert end "Cuboids"

    $view_tensor_glyph insert end "Sheets (L1)"
    $view_tensor_glyph insert end "Sheets (L2)"
    $view_tensor_glyph insert end "Sheets (L3)"
    $view_tensor_glyph insert end "Sheets (L1,L2)"
    $view_tensor_glyph insert end "Sheets (L1,L3)"
    $view_tensor_glyph insert end "Sheets (L2,L3)"
    $view_tensor_glyph insert end "Sheets (L1,L2,L3)"

    $view_tensor_glyph insert end "Superquadrics"
    $view_tensor_glyph insert end "Toroids-1"    
    $view_tensor_glyph insert end "Toroids-2"
    $view_tensor_glyph insert end "Supertoroids-1"
    $view_tensor_glyph insert end "Supertoroids-2"
    $view_tensor_glyph insert end "Supertoroids-3"
    $view_tensor_glyph insert end "Supertoroids-4"
    $view_tensor_glyph insert end "Supertoroids-5"
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
    
       iwidgets::entryfield $subframe1.view_tensor_resol \
    -labeltext "Resolution:" \
	-width 3 \
	-validate integer \
	-textvariable [ itcl::scope global_list(view_tensor_resol) ] \
	-command [ itcl::code $this view_tensor_resol_cb ]
    pack $subframe1.view_tensor_resol -side left -padx 2 -pady 5

    ### supertoroid
    
    set subframe11 [ frame $base.subframe11 ]
    pack $subframe11 -side top -fill both -padx 5 

    iwidgets::entryfield $subframe11.view_toroid_gamma \
		-width 5 \
		-validate real \
		-labeltext "Ring smoothness: " \
		-command [ itcl::code $this view_tensor_gamma_xi_cb ] \
		-textvariable [ itcl::scope global_list(toroidal_gamma) ]
    pack $subframe11.view_toroid_gamma -side left -fill x -padx 5 -pady 5
    
    set view_toroid_usegammamap [ checkbutton $subframe11.view_toroid_usegammamap \
	-text "Use map:" \
	-variable \
	[ itcl::scope global_list(toroidal_usegammamap) ] \
	-command [ itcl::code $this view_tensor_gamma_xi_cb ] ]
    pack $subframe11.view_toroid_usegammamap -side left -pady 5 -padx 5       
    
    set view_toroid_gammamap \
	[ iwidgets::optionmenu $subframe11.view_toroid_gammamap ]
    $view_toroid_gammamap configure -command \
	[ itcl::code $this view_tensor_gamma_xi_cb ]
    for { set i 0 } { $i < [ llength $global_list(result_title) ] } { incr i } {
	$view_toroid_gammamap insert $i [ lindex [ lindex $global_list(result_title) $i ] 0 ]
    }
    pack $view_toroid_gammamap -side left -padx 0
    set widget_list(view_toroid_gammamap) $view_toroid_gammamap
    $view_toroid_gammamap select $FA_MAP

    set subframe12 [ frame $base.subframe12 ]
    pack $subframe12 -side top -fill both -padx 5 

    iwidgets::entryfield $subframe12.view_toroid_gamma2 \
		-width 5 \
		-validate real \
		-labeltext "Cross-section smoothness: " \
		-command [ itcl::code $this view_tensor_gamma_xi_cb ] \
		-textvariable [ itcl::scope global_list(toroidal_gamma2) ]
    pack $subframe12.view_toroid_gamma2 -side left -fill x -padx 5 -pady 5
    
    set view_toroid_usegammamap2 [ checkbutton $subframe12.view_toroid_usegammamap2 \
	-text "Use map:" \
	-variable \
	[ itcl::scope global_list(toroidal_usegammamap2) ] \
	-command [ itcl::code $this view_tensor_gamma_xi_cb ] ]
    pack $subframe12.view_toroid_usegammamap2 -side left -pady 5 -padx 5   
    
    set view_toroid_gammamap2 \
	[ iwidgets::optionmenu $subframe12.view_toroid_gammamap2  ]
    $view_toroid_gammamap2 configure -command \
	[ itcl::code $this view_tensor_gamma_xi_cb ]
    for { set i 0 } { $i < [ llength $global_list(result_title) ] } { incr i } {
	$view_toroid_gammamap2 insert $i [ lindex [ lindex $global_list(result_title) $i ] 0 ]
    }
    pack $view_toroid_gammamap2 -side left -padx 0
    set widget_list(view_toroid_gammamap2) $view_toroid_gammamap2
    $view_toroid_gammamap2 select $FA_MAP



    set subframe13 [ frame $base.subframe13 ]
    pack $subframe13 -side top -fill both -padx 5 

    iwidgets::entryfield $subframe13.view_toroid_xi \
		-width 5 \
		-validate real \
		-labeltext "Xi: " \
		-command [ itcl::code $this view_tensor_gamma_xi_cb ] \
		-textvariable [ itcl::scope global_list(toroidal_xi) ]
    pack $subframe13.view_toroid_xi -side left -fill x -padx 5 -pady 5
        
    checkbutton $subframe13.view_toroid_inequality \
	-text "Consider inequality"  \
	-variable [ itcl::scope global_list(toroidal_inequality) ] \
	-command [ itcl::code $this view_tensor_gamma_xi_cb ]
    pack $subframe13.view_toroid_inequality -side left -fill x -padx 5

    checkbutton $subframe13.view_toroid_swappowers \
	-text "Swap N1 and N2"  \
	-variable [ itcl::scope global_list(toroidal_swappowers) ] \
	-command [ itcl::code $this view_tensor_gamma_xi_cb ]
    pack $subframe13.view_toroid_swappowers -side left -fill x -padx 5

    #iwidgets::entryfield $subframe1.view_toroid_n1 \
	#-width 5 \
	#-validate real \
	#-labeltext "N1: " \
	#-textvariable [ itcl::scope global_list(toroidal_n1) ]
    #pack $subframe1.view_toroid_n1 -side left -fill x -padx 5 -pady 5

    #iwidgets::entryfield $subframe1.view_toroid_n2 \
	#-width 5 \
	#-validate real \
	#-labeltext "N2: " \
	#-textvariable [ itcl::scope global_list(toroidal_n2) ]
    #pack $subframe1.view_toroid_n2 -side left -fill x -padx 5 -pady 5

    ### supertoroid

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
itcl::body mpjtensoranalysis::CreateResultDisplayControl { base } {
    
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
itcl::body mpjtensoranalysis::CreateDirectionDisplayControl { base } {
    
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
    
    #iwidgets::entryfield $subframe3.view_direction_numcolors \
	#-width 4 \
	#-validate integer \
	#-labeltext "Number of colors for quantization:" \
	#-textvariable [ itcl::scope global_list(view_direction_numcolors) ]
    #pack $subframe3.view_direction_numcolors -side left -padx 5 -pady 5

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
itcl::body mpjtensoranalysis::UpdateLookupControlsOnMap { } {

    set mlist [ list  [ $widget_list(view_resultluk) GetRenderWidget ]  [ $widget_list(view_vectorluk) GetRenderWidget ]  [ $widget_list(view_tensorluk) GetRenderWidget ] ]

    foreach widg $mlist {
	[ $widg GetRenderWindow ] Render
	update idletasks
    }
}

itcl::body mpjtensoranalysis::CreateDisplayControl { base } {
        
#    frame $base.dummy -relief sunken -borderwidth 1 -height 2
#    pack $base.dummy -side top -fill x -expand f -padx 12 -pady 0

    set frame0 [ frame $base.frame0 ]
    pack $base.frame0 -pady 0 -fill both -expand t 
    
    set display_notebook [ iwidgets::tabnotebook $frame0.display_notebook -tabpos n ]     
    set widget_list(display_notebook) $display_notebook
    pack $display_notebook -fill both -expand t -pady 5 -padx 5
    
    CreateEigenDisplayControl [ $display_notebook add -label "Eigenvectors" ]
    CreateTensorDisplayControl [ $display_notebook add -label "Tensors" ]
    CreateDirectionDisplayControl [ $display_notebook add -label "Directionality" ]
    CreateResultDisplayControl [ $display_notebook add -label "Results" ]

    eval "bind $base <Map> { $this UpdateLookupControlsOnMap }"

    $display_notebook view "Eigenvectors"   
}

# ------------------------------------------------------------------------------------------
#  Create Transform Menu
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::CreateDisplayTransformMenu { menubase } {

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
itcl::body mpjtensoranalysis::CreateDisplayColorbarMenu { menubase } {  

    menubutton $menubase.colorbar -text "Colorbar" -menu $menubase.colorbar.m -underline 0
    pack $menubase.colorbar -side left

    menu $menubase.colorbar.m -tearoff 0

    $menubase.colorbar.m add radio -label "Eigenvectors" -underline 0 \
	-variable [ itcl::scope global_list(view_colorbar) ] \
	-command [ itcl::code $this view_colorbar_cb ]
    $menubase.colorbar.m add radio -label "Tensor" -underline 0 \
	-variable [ itcl::scope global_list(view_colorbar) ] \
	-command [ itcl::code $this view_colorbar_cb ]
    $menubase.colorbar.m add radio -label "Results" -underline 0 \
	-variable [ itcl::scope global_list(view_colorbar) ] \
	-command [ itcl::code $this view_colorbar_cb ]
}

# ------------------------------------------------------------------------------------------
#  Create Visualization Menu
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::CreateDisplayModeMenu { menubase } {  
    
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
    $menubase.mode.m add separator
    $menubase.mode.m add radio -label "None" -underline 0 \
	-variable [ itcl::scope global_list(view_mode) ] \
	-command [ itcl::code $this view_mode_cb ]

}

# ------------------------------------------------------------------------------------------
#  Create Visualization Menu
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::CreateDisplayTypeMenu { menubase } {  
    
    menubutton $menubase.type -text "Display" -menu $menubase.type.m -underline 0
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
    $menubase.type.m add check -label "Tensor" -underline 0 \
	-variable [ itcl::scope global_list(view_type_tensor) ] \
	-command [ itcl::code $this view_type_toggle_tensor ]    
    $menubase.type.m add separator
    $menubase.type.m add check -label "Colorbar" -underline 0 \
	-variable [ itcl::scope global_list(view_type_colorbar) ] \
	-command [ itcl::code $this view_type_toggle_colorbar ]
    $menubase.type.m add separator
    $menubase.type.m add command -label "Save .vtk" -underline 0 \
    -command [ itcl::code $this view_save_glyphs ]
}

# ------------------------------------------------------------------------------------------
#  Create Update Menu
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::CreateDisplayUpdateMenu { menubase } {

    menubutton $menubase.update -text "Update" -menu $menubase.update.m -underline 0
    pack $menubase.update -side left

    menu $menubase.update.m -tearoff 0

    $menubase.update.m add command -label "Now" -underline 0 \
	-command [ itcl::code $this view_update_display_cb ] 
    $menubase.update.m add separator
    $menubase.update.m add check -label "Auto update" -underline 0 \
	-variable [ itcl::scope global_list(view_autoupdate) ] \
	-command [ itcl::code $this view_toggle_autoupdate ]

}

# -------------------------------------------------------------------------------------------
#  Diffusion control inititialization
# ------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::Initialize { widget } {
    
    if { $initialized == 1 } { return $basewidget }
    
    #  -------------------------------------
    #  Create User Interface
    #  -------------------------------------	
    set basewidget [toplevel $widget ]
    wm geometry $basewidget 650x480
    wm withdraw $basewidget

    set notebook $basewidget.notebook    
    iwidgets::tabnotebook $notebook -tabpos w         
    set widget_list(notebook) $notebook

    set mb [ frame $basewidget.mb ]
    pack $mb -side top -fill x -expand false
    
    CreateMenu $mb
    CreateDisplayTypeMenu $mb
    CreateDisplayModeMenu $mb
#    CreateDisplayTransformMenu $mb
    CreateDisplayColorbarMenu $mb
    CreateDisplayUpdateMenu $mb
    
    #button $mb.refresh -text "Refresh" -borderwidth 0 \
    #	-command [ itcl::code $this view_update_display_cb ] -underline 2
    #pack $mb.refresh -side left -padx 0

    CreateTensorControl     [ $notebook add -label "Tensor" ]
    CreateTransformControl  [ $notebook add -label "Transform" ]
    CreateToroidalControl   [ $notebook add -label "Toroid" ]
    CreateResultsControl    [ $notebook add -label "Results" ] 
    CreateStatisticsControl [ $notebook add -label "Statistics" ]
    CreateDisplayControl [ $notebook add -label "Display" ]

    
    pack $notebook -side top -fill both -expand t -padx 5
    $notebook view "Tensor"

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
itcl::body mpjtensoranalysis::CreateMenu { mb } {    
    
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
itcl::body mpjtensoranalysis::AddToMenuButton { mb args } {
    eval "$mb add command -label \"Tensor analysis\" -command {$this ShowWindow \"Tensor\"}"
}
#-------------------------------------------------------------------------------------------
itcl::body mpjtensoranalysis::SetParametersFromBisAlgorithmAndCompute { cscheme satfactor brightness usemask } {

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
itcl::body mpjtensoranalysis::GetOutputImage { index } {
    return [ [ lindex $global_list(result_list) $index ] GetThisPointer]
}

#-------------------------------------------------------------------------------------------
#  Main function for stand-alone execution
# ------------------------------------------------------------------------------------------
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    puts "\n[ file rootname $argv0 ] is not a stand-alone program.\n"
    exit
}



