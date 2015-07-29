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


#!/bin/sh
# the next line restarts using wish \
    exec vtk "$0" "$@"

# -----------------------
# Dependencies and path
# -----------------------
lappend auto_path [ file dirname [ info script ] ]
lappend auto_path [ file join [file join [ file dirname [ info script ]] ".."] base ]
lappend auto_path [ file join [file join [ file dirname [ info script ]] ".."] main ]

package provide mpjfibertracking 1.2

package require  Itcl 3.2
package require  Iwidgets 4.0

package require  pxitcllistselector 1.0
package require  pxitclbaseimagecontrol 1.0
package require  pxitclbaseimageviewer 1.0

package require  vtkmpjcontrib 1.1
package require  mpjguilookuptable 1.0

itcl::class mpjfibertracking {

    inherit pxitclbaseimagecontrol
    
    #-----------------------------------
    # construction and destruction
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
    # input/output
    #-----------------------------------
    public method LoadMap { fname }
    public method LoadMask { fname }
    public method LoadTensor { fname }
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
    private method CreateDirectionControl { base }
    private method CreateTrackingControl { base }
    private method CreateDisplayControl { base }
    private method CreateFiberControl { base }
    private method CreateStatisticsControl { base }    
    private method CreateResultsControl { base }
    private method CreateFiberDisplayControl { base }
    private method CreateMenu { mb }
    public method AddToMenuButton { mb args}    
    public method CreateViewMenu { menubase }
 
    private method loadmap { }
    private method loadtensor { }
    private method loadmask { }
    private method displayresult { { mode image } }
    private method loadresult { }
    private method saveresult { }
    private method toggleresult { }
    private method saveallresults { }
 
    private method initialize_glyphs { }
    private method input_updateinfo { index }    
    private method input_selectioncommand_cb { }   
    
    private method track_seedtype_cb { }
    private method track_seedradius_cb { }
    private method track_integration_cb { }
    private method track_eigenvector_cb { }
    private method track_regionfilter_cb { }

    private method view_colorbar_cb { } 
    private method view_directionscheme_cb { }
    private method view_update_display_cb { }

    private method view_clear_display { }
    private method view_update_display { }
    private method view_toggle_colorbar { }
    private method view_toggle_fiber { }

    private method view_create_fiber_glyph { index }
    private method view_create_fibers { }
    private method view_create_colorbar { }

    private method view_fiber_glyph_cb { }
    private method view_fiber_glyphsize_cb { }
    private method view_fiber_pointsize_cb { }
    private method view_fiber_colormap_cb { }
    private method view_fiber_cmapresetbtn_cb { }
    private method view_fiber_cmaploadbtn_cb { }
    private method view_fiber_cmapsavebtn_cb { }
    private method view_fiber_filter_cb { }
          
    private method fiber_add { polydata }
    private method fiber_addwithname { name polydata }
    private method fiber_remove { index } 
    private method fiber_removeselected { }
    private method fiber_clear { }
    private method fiber_load { }
    private method fiber_save { index name }
    private method fiber_save_vtk { index name }
    private method fiber_saveall { }
    private method fiber_saveselected { }
    private method fiber_track { }

    private method fiber_statistics { }
    private method fiber_quantize { }
    private method fiber_filterangle { }
    private method fiber_addstatistics { title results tag }
    private method fiber_beginstatistics { title overwrite }
    private method fiber_savestatistics { }
    private method fiber_selection_cb { }
    private method fiber_material_cb { base }
    private method fiber_color_cb { }
    private method fiber_name_cb { }
    private method fiber_display_cb { }

    private method ispositive { x }  
    private method result_clear { }
    private method update_list { node }    

    private method material_dialog { base }
    private method savedir_dialog { base initdir title }
    private method openfile_dialog { base initdir title mask }
}  

#-------------------------------------------------------------------------------------------
#  Close window 
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::DismissWindow { } {
    
    set ok "yes"

    if { $global_list(tensor_fname) != "" } {

	   set ok [ tk_messageBox -type yesnocancel -default no \
             -parent $basewidget -title "Think again ..." \
             -message "Your work session will be lost. Are you sure ?" -icon question  ]

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
itcl::body mpjfibertracking::InitializeControl { } {

    set global_list(appname) "Fiber Tracking Utility"
    
    set appname "$global_list(appname) $version <ST2>"
    set aboutstring "(c) Marcel Jackowski 2014"
    
    # create list of basic colors
    set global_list(tkcolor_list) "\#ff0000 \#ffff00 \#00ff00 \#00ffff \#0000ff \#ff00ff"
    
    # create image titles
    set global_list(result_title) \
	[ list \
	      { "Diffusion tensor"       "dti_tensor" 0 } \
	      { "Regions of interest"    "dti_roi" 0 } \
	      { "Map for analysis"       "dti_map" 0 } \
	      { "Fiber quantization"     "dti_fiber" 1 } \
          { "Input map quantization" "dti_input" 1 } ]
    
    foreach item $global_list(result_title) {
 	lappend result_list [ pxitclimage \#auto ]
    }  
    
    set global_list(result_fdir) [ pwd ]
    set global_list(result_fpreffix) "preffix_"
    set global_list(result_fsuffix) ".nii.gz"
    set global_list(result_list) $result_list
    
    set global_list(mask_fname) ""
    set global_list(mask_nreg) 1
    
    set global_list(tensor_fname) ""    
    set global_list(map_fname) ""
    
    set global_list(track_lastpoint) { 0 0 0 }
    set global_list(track_seedtype) 0
    set global_list(track_seedregion) 1
    set global_list(track_seedwidth) 3
    set global_list(track_seedheight) 3
    set global_list(track_seeddepth) 3
    set global_list(track_seedradius) 3
    set global_list(track_seedradius2) 3
    set global_list(track_seeddens) 1.0  
    set global_list(track_integration) 1
    set global_list(track_eigenvector) 0
    set global_list(track_norm_compute) 0
    set global_list(track_norm_eigenvector1) 1
    set global_list(track_norm_eigenvector2) 2
    set global_list(track_steplen) 0.25
    set global_list(track_maxdist) 100.0
    set global_list(track_mindist) 10.0
    set global_list(track_maxangle) 45.0
    set global_list(track_maxtorsion) 70.0
    set global_list(track_minFA) 0.2
    set global_list(track_maxFA) 1.0
    set global_list(track_minMD) 0.0
    set global_list(track_maxMD) 10.0   
    set global_list(track_regionfilter) 0
    set global_list(track_regionnumber) 1
    set global_list(track_clipping) 0
    set global_list(track_autostat) 1
    set global_list(track_flipping) 1
    set global_list(track_dec) 1
    set global_list(track_flipx) 0
    set global_list(track_flipy) 0
    set global_list(track_flipz) 0
    set global_list(track_constraintomask) 0
    set global_list(track_numthreads) 4

    set global_list(fiber_display) 1
    set global_list(fiber_color) "\#ff0000"
    set global_list(fiber_list) {}
    set global_list(fiber_color_list) {}
    
    set global_list(fiber_ambk_list) {}
    set global_list(fiber_diffk_list) {}
    set global_list(fiber_speck_list) {}
    set global_list(fiber_specp_list) {}

    set global_list(fiber_disp_list) {}
    set global_list(fiber_minangle) 0.0
    set global_list(fiber_maxangle) 35.0
    set global_list(fiber_mintorsion) 0.0
    set global_list(fiber_maxtorsion) 45.0
    set global_list(fiber_invinterval) 0   
    set global_list(fiber_invtorsioninterval) 0
    set global_list(fiber_enablehist) 0
    set global_list(fiber_enablevtk) 0
    set global_list(fiber_histbins) 100

    set global_list(mat_ambk) 0.0
    set global_list(mat_diffk) 1.0
    set global_list(mat_speck) 0.0
    set global_list(mat_specp) 1.0

    set global_list(stat_overwrite) 0

    set global_list(view_colorbar) 0 
    set global_list(view_colorbar_label) "Map"
    set global_list(view_fiber) 0
    set global_list(view_fiber_glyphsize) 0.20
    set global_list(view_fiber_eigenscale1) 1.0
    set global_list(view_fiber_eigenscale2) 1.0
    set global_list(view_fiber_pointsize) 1.0
    set global_list(view_fiber_resolution) 10
    set global_list(view_fiber_tiplength) 0.35
    set global_list(view_fiber_tipradius) 0.2
    set global_list(view_fiber_shaftradius) 0.1
    set global_list(view_fiber_eigenscale) 0
    
    set global_list(view_direction_scheme) 0
    set global_list(view_direction_saturation) 0.5
    set global_list(view_direction_scaling) 1.0
    set global_list(view_direction_modulate) 0
    set global_list(view_direction_alpha) 0
    

    ## create actors for visualization
    set global_list(actor_colorbar) -1
    set global_list(actor_fiber) {}

    ## vtk data
    set global_list(vtk_fiber_tube) {}
    set global_list(vtk_fiber_polydata) {}
    set global_list(vtk_fiber_ribbon) {}
    set global_list(vtk_fiber_arrows) {}
    
    initialize_glyphs

     [ $parent GetViewer ] SetNavigationPointerGeometryToDoubleSphere \
            $global_list(track_seedradius) $global_list(track_seedradius2)
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create visualization glyphs
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::initialize_glyphs { } {

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
    $arrow_src SetTipLength $global_list(view_fiber_tiplength)
    $arrow_src SetTipResolution $global_list(view_fiber_resolution)
    $arrow_src SetTipRadius $global_list(view_fiber_tipradius)
    $arrow_src SetShaftResolution $global_list(view_fiber_resolution)
    $arrow_src SetShaftRadius $global_list(view_fiber_shaftradius)
    set global_list(glyph_arrowsrc) $arrow_src

    set arrow_xfrm [ vtkTransform [ pxvtable::vnewobj ] ]
    $arrow_xfrm Translate -1.0 0.0 0.0
    $arrow_xfrm Scale 2.0 2.0 2.0

    set arrow [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
    $arrow SetInput [ $arrow_src GetOutput ]
    $arrow SetTransform $arrow_xfrm
    lappend global_list(glyph_list) $arrow

    set assign [ vtkAssignAttribute [ pxvtable::vnewobj ] ]
    $assign Assign "binormal" VECTORS POINT_DATA
    set global_list(glyph_assign) $assign

    #$arrow_src Delete
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
#  PRIVATE: Load mask
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::loadmask { } {
    
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
itcl::body mpjfibertracking::LoadMask { fname } {

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
itcl::body mpjfibertracking::loadmap { } {

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
itcl::body mpjfibertracking::LoadMap { fname } {

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
itcl::body mpjfibertracking::loadtensor { } {

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
itcl::body mpjfibertracking::LoadTensor { fname } {

    set TENSOR 0
    set MASK 1
    set MAP 2
    set FIBERS 3
    set MAPQUANT 4

    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set fibers [ lindex $global_list(result_list) $FIBERS ]
    set map [lindex $global_list(result_list) $MAP ]
    set mapquant [ lindex $global_list(result_list) $MAPQUANT ]

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
#    if { $or == 0 } {
# 	set global_list(track_flipx) 1
# 	set global_list(track_flipy) 1
#    } elseif { $or == 1 } {
# 	set global_list(track_flipx) 1
#    } 
    
    $ana Delete

    scan [ [ $tensor GetImage ] GetDimensions ] "%d %d %d" dx dy dz	       
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
    
    set global_list(result_fdir) "[ file dir $fname ]"
    set global_list(result_fpreffix) "[file tail $name ]"

    ## create fiber image
    set ext [ vtkImageExtractComponents [ pxvtable::vnewobj ] ]
    $ext SetInput [ $tensor GetImage ]
    $ext SetComponents 0
    $ext Update

     ## create map quantization image
    set ext2 [ vtkImageExtractComponents [ pxvtable::vnewobj ] ]
    $ext2 SetInput [ $tensor GetImage ]
    $ext2 SetComponents 0
    $ext2 Update
    
    $mapquant CopyImageHeader [ $tensor GetImageHeader ]
    $mapquant ShallowCopyImage [ $ext2 GetOutput ]
    $ext2 Delete

    $fibers CopyImageHeader [ $tensor GetImageHeader ]
    $fibers ShallowCopyImage [ $ext GetOutput ]
    $ext Delete

    #set global_list(result_fdir) [ file dir $fname ]
    #set global_list(result_fpreffix) "[file tail $name ]"

    WatchOff

    $widget_list(trackbtn) buttonconfigure 0 -state normal
    $widget_list(menu_tracking) entryconfigure 0 -state normal
    $widget_list(menu_tracking) entryconfigure 2 -state normal
    
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Set result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::SetResultPreffix { preffix } {
    set global_list(result_fpreffix) $preffix
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Get result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::GetResultPreffix { } {
    return $global_list(result_fpreffix)
}

itcl::body mpjfibertracking::SetCurrentDirectory { fname } {
    set global_list(result_fdir) $fname
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Clear all results
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::ClearAllResults { } {
    
    foreach result $global_list(result_list) {
	$result Clear
    }

}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::SaveAllResults { } {

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
itcl::body mpjfibertracking::loadresult { } {

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
itcl::body mpjfibertracking::SaveResult { index } {        

    set fname $global_list(result_fpreffix)
    set fname "$fname[ lindex [lindex $global_list(result_title) $index ] 1 ]"

    [lindex $global_list(result_list) $index] Save $fname
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::saveallresults { } {

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
itcl::body mpjfibertracking::saveresult { } {
    
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
itcl::body mpjfibertracking::DisplayResult { index { mode image } } {
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
itcl::body mpjfibertracking::displayresult { { mode image } } {
  
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
itcl::body mpjfibertracking::toggleresult { } {
   
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
itcl::body mpjfibertracking::result_clear { } {
    for { set i 0 } { $i < [ llength $global_list(result_list) ] } { incr i } {		
	if { $i > 0 } {	    
	    [ lindex $global_list(result_list) $i ] Clear	    
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Clear display
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_clear_display { } {
    
    ## remove fibers
    foreach actor $global_list(actor_fiber) {	    
	if { $actor != "-1" } {
	    [ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor	    
	    $actor Delete	    
	}
	set global_list(actor_fiber) {}
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
itcl::body mpjfibertracking::view_update_display { } {
    
    if { $global_list(view_colorbar) } {
	view_create_colorbar
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  Toggle On/Off colorbar visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_toggle_colorbar { } {
    
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
#  View fiber bundle
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_create_fiber_glyph { index } {

    set field(0) fractani
    set field(1) meandiff
    set field(2) distance

    set field(3) angle
    set field(4) torsion

    set field(5) roi
    set field(6) other
    set field(7) minangle
    set field(8) maxangle
    set field(9) medangle
    set field(10) meanangle
    set field(11) varangle

    set field(12) mintorsion
    set field(13) maxtorsion
    set field(14) medtorsion 
    set field(15) meantorsion
    set field(16) vartorsion

    set field(17) minother
    set field(18) maxother
    set field(19) medother
    set field(20) meanother
    set field(21) varother

    set field(22) rgb

    ## create mapper
    set fiberMapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
    
    ## get current glyph
    set cur_glyph [ $widget_list(view_fiber_glyph) \
			index [ $widget_list(view_fiber_glyph) get ] ]
        
    ## get current fiber colormap filter    
    set filter [ $widget_list(view_fiber_filter) index [ $widget_list(view_fiber_filter) get ]]

    set assign $global_list(glyph_assign)
    $assign SetInput [ lindex $global_list(vtk_fiber_polydata) $index ]

    if { $cur_glyph >= 5 } {

	## vectors
	set arrows [ lindex $global_list(vtk_fiber_arrows) $index ]
	$arrows SetInput [ lindex $global_list(vtk_fiber_polydata) $index ]
	$arrows SetSource [ [ lindex $global_list(glyph_list) 2 ] GetOutput ]
	$arrows ScalingOn
	$arrows ClampingOn
	$arrows SetScaleFactor $global_list(view_fiber_glyphsize)
	$arrows SetScaleModeToDataScalingOff
	$arrows SetColorModeToColorByScalar

	if { $cur_glyph == 7 } {
	    $arrows SetInput [ $assign GetOutput ]
	    $arrows SetVectorModeToUseVector	    
	} elseif { $cur_glyph == 6 } {
	    $arrows SetVectorModeToUseNormal
	} else {
	    $arrows SetVectorModeToUseVector
	}

	## set up glyph mapper
	$fiberMapper SetInput [ $arrows GetOutput ]
	$fiberMapper SetLookupTable [ $widget_list(view_fiberluk) GetLookupTable ]
	$fiberMapper UseLookupTableScalarRangeOn
	$fiberMapper SetScalarModeToUsePointFieldData

	if { [ [ [ $arrows GetOutput ] GetPointData ] GetArray $field($filter) ] != "" } {
	    $fiberMapper ColorByArrayComponent $field($filter) 0
	}
	
	if { $global_list(view_fiber_luk) } {	    
	    $fiberMapper ScalarVisibilityOn
	} else {
	    $fiberMapper ScalarVisibilityOff
	}
    }

    if { $cur_glyph == 4 } {
	## tubes
	set tube [ lindex $global_list(vtk_fiber_tube) $index ]
	
	$tube SetInput [ lindex $global_list(vtk_fiber_polydata) $index ]
	$tube SetRadius $global_list(view_fiber_glyphsize)
	$tube SetNormalEigenvector $global_list(track_norm_eigenvector1)
	$tube SetBinormalEigenvector $global_list(track_norm_eigenvector2)
	##if { $global_list(track_norm_switch) == 0 } {
	    $tube SetNormalScale $global_list(view_fiber_eigenscale1)
	    $tube SetBinormalScale $global_list(view_fiber_eigenscale2)
	##} else {
	##    $tube SetNormalScale $global_list(view_fiber_eigenscale2)
	##    $tube SetBinormalScale $global_list(view_fiber_eigenscale1)
	##}
	$tube SetVaryRadius $global_list(view_fiber_eigenscale)
	$tube SetCapping 1
	$tube SetNumberOfSides $global_list(view_fiber_resolution)
	$tube Update

	
	$fiberMapper SetInput [ $tube GetOutput ]	
	$fiberMapper SetLookupTable [ $widget_list(view_fiberluk) GetLookupTable ]
	$fiberMapper UseLookupTableScalarRangeOn
	$fiberMapper SetScalarModeToUsePointFieldData

	if { [ [ [ $tube GetOutput ] GetPointData ] GetArray $field($filter) ] != "" } {
	    $fiberMapper ColorByArrayComponent $field($filter) 0
	}
	
	if { $global_list(view_fiber_luk) } {	    
	    $fiberMapper ScalarVisibilityOn
	} else {
	    $fiberMapper ScalarVisibilityOff
	}

    } 

    if { $cur_glyph == 3 } {
	## solid ribbons
	set sribbon [ lindex $global_list(vtk_fiber_sribbon) $index ]
	
	$sribbon SetInput [ lindex $global_list(vtk_fiber_polydata) $index ]
	$sribbon SetRadius $global_list(view_fiber_glyphsize)
	$sribbon SetVaryRadius $global_list(view_fiber_eigenscale)
	$sribbon SetNormalScale $global_list(view_fiber_eigenscale1)
	$sribbon SetBinormalScale $global_list(view_fiber_eigenscale2)
	$sribbon SetNormalEigenvector $global_list(track_norm_eigenvector1)
	$sribbon SetBinormalEigenvector $global_list(track_norm_eigenvector2)
	$sribbon SetSidesShareVertices 0
	$sribbon SetCapping 1
	$sribbon Update

	
	$fiberMapper SetInput [ $sribbon GetOutput ]	
	$fiberMapper SetLookupTable [ $widget_list(view_fiberluk) GetLookupTable ]
	$fiberMapper UseLookupTableScalarRangeOn
	$fiberMapper SetScalarModeToUsePointFieldData

	if { [ [ [ $sribbon GetOutput ] GetPointData ] GetArray $field($filter) ] != "" } {
	    $fiberMapper ColorByArrayComponent $field($filter) 0
	}
	
	if { $global_list(view_fiber_luk) } {	    
	    $fiberMapper ScalarVisibilityOn
	} else {
	    $fiberMapper ScalarVisibilityOff
	}

    } 

    if { $cur_glyph == 2 } {
	## ribbons
	set ribbon [ lindex $global_list(vtk_fiber_ribbon) $index ]

	$ribbon SetInput [ lindex $global_list(vtk_fiber_polydata) $index ]
	$ribbon SetWidth $global_list(view_fiber_glyphsize)
	$ribbon SetWidthFactor 5
	$ribbon UseDefaultNormalOff
	$ribbon Update

	$fiberMapper SetInput [ $ribbon GetOutput ]	
	$fiberMapper SetLookupTable [ $widget_list(view_fiberluk) GetLookupTable ]
	$fiberMapper UseLookupTableScalarRangeOn
	$fiberMapper SetScalarModeToUsePointFieldData

	if { [ [ [ $ribbon GetOutput ] GetPointData ] GetArray $field($filter) ] != "" } {
	    $fiberMapper ColorByArrayComponent $field($filter) 0
	}
	
	if { $global_list(view_fiber_luk) } {	    
	    $fiberMapper ScalarVisibilityOn
	} else {
	    $fiberMapper ScalarVisibilityOff
	}

    } 

    if { $cur_glyph < 2 } {
	
	set polydata [ lindex $global_list(vtk_fiber_polydata) $index ]

	$fiberMapper SetInput $polydata
	$fiberMapper SetLookupTable [ $widget_list(view_fiberluk) GetLookupTable ]
	$fiberMapper UseLookupTableScalarRangeOn
	$fiberMapper SetScalarModeToUsePointFieldData
    $fiberMapper SetScalarMaterialModeToAmbient
	
	if { [ [ $polydata GetPointData ] GetArray $field($filter) ] != "" } {
	    $fiberMapper ColorByArrayComponent $field($filter) 0
	}

	if { $global_list(view_fiber_luk) } {	    
	    $fiberMapper ScalarVisibilityOn
	} else {
	    $fiberMapper ScalarVisibilityOff
	}
    }


    ## create actor
    set fiberActor [ vtkActor [ pxvtable::vnewobj ] ]
    $fiberActor SetMapper $fiberMapper
    
    if { $cur_glyph == 0 } {
	[ $fiberActor GetProperty ] SetRepresentationToPoints
	[ $fiberActor GetProperty ] SetPointSize $global_list(view_fiber_pointsize)
	[ $fiberActor GetProperty ] ShadingOff
    } elseif { $cur_glyph == 1 } {
	[ $fiberActor GetProperty ] SetRepresentationToWireframe
	[ $fiberActor GetProperty ] ShadingOff
    } else {
	[ $fiberActor GetProperty ] SetRepresentationToSurface
	[ $fiberActor GetProperty ] ShadingOn
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
    
    [$fiberActor GetProperty] SetAmbientColor $red $green $blue
    [$fiberActor GetProperty] SetDiffuseColor $red $green $blue
    [$fiberActor GetProperty] SetSpecularColor $red $green $blue

    set ambk [ lindex $global_list(fiber_ambk_list) $index ]
    set diffk [ lindex $global_list(fiber_diffk_list) $index ]
    set speck [ lindex $global_list(fiber_speck_list) $index ]
    set specp [ lindex $global_list(fiber_specp_list) $index ]

    [$fiberActor GetProperty] SetAmbient $ambk
    [$fiberActor GetProperty] SetDiffuse $diffk
    [$fiberActor GetProperty] SetSpecular $speck
    [$fiberActor GetProperty] SetSpecularPower $specp
    
    return $fiberActor
}

#-------------------------------------------------------------------------------------------
#  View fibers
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_create_fibers { } {    

    set num [ llength $global_list(fiber_list) ]
    
    for {set j 0} {$j < $num} {incr j} {
	
	set actor [ view_create_fiber_glyph $j ]
	
	lappend global_list(actor_fiber) $actor
	
	## add actor if to be displayed
	if { [ lindex $global_list(fiber_disp_list) $j ] == 1 } { 
	    [ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] AddActor $actor
	}
    }
    
    [ $parent GetViewer ] UpdateDisplay
    
}

#-------------------------------------------------------------------------------------------
#  View colorbar
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_create_colorbar { } {
        
    set title [ $widget_list(view_fiber_filter) get ]
    if { $title == "Input map" } {
	set title $global_list(view_colorbar_label)
    }

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
#  PRIVATE CALLBACK: Invoked upon change in seed type
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::track_seedradius_cb { } {

    set radius $global_list(track_seedradius)
    set radius2 $global_list(track_seedradius2)

    [ $parent GetViewer ] SetNavigationPointerGeometryToDoubleSphere $radius $radius2
    [ $parent GetViewer ] UpdateDisplay

}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in seed type
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::track_seedtype_cb { } {

    set state(0) disabled; set state(1) normal    
    set global_list(track_seedtype) \
	[ $widget_list(track_seedtype) index [ $widget_list(track_seedtype) get ] ]

    if { [expr $global_list(track_seedtype) < 2 || \
               $global_list(track_seedtype) == 3 || \
               $global_list(track_seedtype) == 4 ]} {
    	$widget_list(track_seedwidth) configure -state disabled
    	$widget_list(track_seedheight) configure -state disabled
    	$widget_list(track_seeddepth) configure -state disabled

    	if { [expr $global_list(track_seedtype) == 1 || $global_list(track_seedtype) == 4 ] } {
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

    if { $global_list(track_seedtype) == 4 } {
       $widget_list(track_seedradius) configure -state normal
       set radius $global_list(track_seedradius)
       set radius2 $global_list(track_seedradius2)
       if { [ [ $parent GetViewer ] GetImage ] != "" } {
            [ $parent GetViewer ] SetNavigationPointerGeometryToDoubleSphere $radius $radius2
            [ $parent GetViewer ] RenderNavigationPointer 3
        }
    } else {
       if { [ [ $parent GetViewer ] GetImage ] != "" } { 
         [ $parent GetViewer ] RenderNavigationPointer 0 
       }
        $widget_list(track_seedradius) configure -state disabled
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in region filtering option
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::track_regionfilter_cb { } {

    set state(1) normal
    set state(0) disabled

    $widget_list(track_seedradius2) configure -state \
	$state($global_list(track_regionfilter))
    
    $widget_list(track_clipping) configure -state \
	$state($global_list(track_regionfilter))
    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in color schemes
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_directionscheme_cb { } {
    set global_list(view_direction_scheme) \
	[ $widget_list(view_direction_scheme) index [ $widget_list(view_direction_scheme) get ] ]
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in integration
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::track_integration_cb { } {
    set global_list(track_integration) \
	[ $widget_list(track_integration) index [ $widget_list(track_integration) get ] ]
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in eigenvector
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::track_eigenvector_cb { } {
    set global_list(track_eigenvector) \
	[ $widget_list(track_eigenvector) index [ $widget_list(track_eigenvector) get ] ]
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon an update button press
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_update_display_cb { } { 
    
    view_clear_display
    view_update_display
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in bundle radius
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_fiber_glyphsize_cb { } {
    
    
    $global_list(glyph_arrowsrc) SetTipRadius $global_list(view_fiber_tipradius)
    $global_list(glyph_arrowsrc) SetShaftRadius $global_list(view_fiber_shaftradius)
    $global_list(glyph_arrowsrc) SetTipLength $global_list(view_fiber_tiplength)
    $global_list(glyph_arrowsrc) SetTipResolution $global_list(view_fiber_resolution)
    $global_list(glyph_arrowsrc) SetShaftResolution $global_list(view_fiber_resolution)

   ## puts "eigenscale1: $global_list(view_fiber_eigenscale1)"
   ## puts "eigenscale2: $global_list(view_fiber_eigenscale2)"

    foreach \
	tube $global_list(vtk_fiber_tube) \
	arrows $global_list(vtk_fiber_arrows) \
	sribbon $global_list(vtk_fiber_sribbon) \
	ribbon $global_list(vtk_fiber_ribbon) {

	$tube SetRadius $global_list(view_fiber_glyphsize)
	$tube SetNumberOfSides $global_list(view_fiber_resolution)
	$tube SetVaryRadius $global_list(view_fiber_eigenscale)
	$tube SetNormalScale $global_list(view_fiber_eigenscale1)
	$tube SetBinormalScale $global_list(view_fiber_eigenscale2)

        $ribbon SetWidth $global_list(view_fiber_glyphsize)

	$sribbon SetRadius $global_list(view_fiber_glyphsize)
	$sribbon SetVaryRadius $global_list(view_fiber_eigenscale)
	$sribbon SetNormalScale $global_list(view_fiber_eigenscale1)
	$sribbon SetBinormalScale $global_list(view_fiber_eigenscale2)

	$arrows SetScaleFactor $global_list(view_fiber_glyphsize)
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in point size
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_fiber_pointsize_cb { } {
    
    foreach actor $global_list(actor_fiber) {
	[ $actor GetProperty ] SetPointSize $global_list(view_fiber_pointsize)
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon copy eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_fiber_cmapresetbtn_cb { } {
    
    ## current fiber filter
    $widget_list(view_fiberluk) SetHueRange 0.5 0.0
    $widget_list(view_fiberluk) Update

}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon loading eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_fiber_cmaploadbtn_cb { } {   

    $widget_list(view_fiberluk) Load
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon saving eigen colormap 
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_fiber_cmapsavebtn_cb { } {   

    $widget_list(view_fiberluk) SaveAs
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in colormap for fibers
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_fiber_colormap_cb { } {
    
    foreach \
	actor $global_list(actor_fiber) {
	    
	    ## create mapper
	    set fiberMapper [ $actor GetMapper ]

	    if { $global_list(view_fiber_luk) } {
		$fiberMapper ScalarVisibilityOn
        #$fiberMapper SetScalarMaterialModeToAmbientAndDiffuse
	    } else {
		$fiberMapper ScalarVisibilityOff
	    }
	}
    
      [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in bundle glyph
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_fiber_glyph_cb { } {

    set field(0) fractani
    set field(1) meandiff
    set field(2) distance

    set field(3) angle
    set field(4) torsion

    set field(5) roi
    set field(6) other
    set field(7) minangle
    set field(8) maxangle
    set field(9) medangle
    set field(10) meanangle
    set field(11) varangle

    set field(12) mintorsion
    set field(13) maxtorsion
    set field(14) medtorsion 
    set field(15) meantorsion
    set field(16) vartorsion

    set field(17) minother
    set field(18) maxother
    set field(19) medother
    set field(20) meanother
    set field(21) varother

    set field(22) rgb

    ## get current fiber colormap filter    
    set filter [ $widget_list(view_fiber_filter) index [ $widget_list(view_fiber_filter) get ]]

    ## get current glyph
    set cur_glyph [ $widget_list(view_fiber_glyph) \
			index [ $widget_list(view_fiber_glyph) get ] ]
    
    foreach \
	actor $global_list(actor_fiber) \
	tube $global_list(vtk_fiber_tube) \
        ribbon $global_list(vtk_fiber_ribbon) \
	sribbon $global_list(vtk_fiber_sribbon) \
	arrows $global_list(vtk_fiber_arrows) \
	polydata $global_list(vtk_fiber_polydata) {
	    
	    ## create mapper
	    set fiberMapper [ $actor GetMapper ]
	    

	    set assign $global_list(glyph_assign)
	    $assign SetInput $polydata

	    if { $cur_glyph >=  5 } {

		$arrows SetSource [ [ lindex $global_list(glyph_list) 2 ] GetOutput ]
		$arrows ScalingOn
		$arrows ClampingOn
		$arrows SetScaleFactor $global_list(view_fiber_glyphsize)
		$arrows SetScaleModeToDataScalingOff
		$arrows SetColorModeToColorByScalar

		if { $cur_glyph == 7 } {
		    $arrows SetInput [ $assign GetOutput ]
		    $arrows SetVectorModeToUseVector	    
		} elseif { $cur_glyph == 6 } {
		    $arrows SetInput $polydata
		    $arrows SetVectorModeToUseNormal
		} else {
		    $arrows SetInput $polydata
		    $arrows SetVectorModeToUseVector
	        }

		## set up glyph mapper
		$fiberMapper SetInput [ $arrows GetOutput ]
		$fiberMapper SetLookupTable [ $widget_list(view_fiberluk) GetLookupTable ]
		$fiberMapper UseLookupTableScalarRangeOn
		$fiberMapper SetScalarModeToUsePointFieldData
		
		if { [ [ [ $arrows GetOutput ] GetPointData ] GetArray $field($filter) ] != "" } {
		    $fiberMapper ColorByArrayComponent $field($filter) 0
		}
		
		if { $global_list(view_fiber_luk) } {	    
		    $fiberMapper ScalarVisibilityOn
		} else {
		    $fiberMapper ScalarVisibilityOff
		}
	    }

	    if { $cur_glyph == 4 } {
		
		$tube SetInput $polydata
		$tube SetRadius $global_list(view_fiber_glyphsize)
		$tube SetNormalEigenvector $global_list(track_norm_eigenvector1)
		$tube SetBinormalEigenvector $global_list(track_norm_eigenvector2)

		##if { $global_list(track_norm_switch) == 0 } {
		    $tube SetNormalScale $global_list(view_fiber_eigenscale1)
		    $tube SetBinormalScale $global_list(view_fiber_eigenscale2)
		##} else {
		##    $tube SetNormalScale $global_list(view_fiber_eigenscale2)
		##    $tube SetBinormalScale $global_list(view_fiber_eigenscale1)
		##}
		$tube SetVaryRadius $global_list(view_fiber_eigenscale)
		$tube SetCapping 1
		$tube SetNumberOfSides $global_list(view_fiber_resolution)
		$tube Update
		
		if { [ [ [ $tube GetOutput ] GetPointData ] GetArray $field($filter) ] != "" } {
		    $fiberMapper ColorByArrayComponent $field($filter) 0
		}

		$fiberMapper SetInput [ $tube GetOutput ]
		if { $global_list(view_fiber_luk) } {
		    $fiberMapper ScalarVisibilityOn
		} else {
		    $fiberMapper ScalarVisibilityOff
		}
	    }

	    if { $cur_glyph == 3 } {
		
		$sribbon SetInput $polydata
		$sribbon SetRadius $global_list(view_fiber_glyphsize)
		$sribbon SetNormalScale $global_list(view_fiber_eigenscale1)
		$sribbon SetBinormalScale $global_list(view_fiber_eigenscale2)
		$sribbon SetVaryRadius $global_list(view_fiber_eigenscale)
		$sribbon SetNormalEigenvector $global_list(track_norm_eigenvector1)
		$sribbon SetBinormalEigenvector $global_list(track_norm_eigenvector2)
		$sribbon SetSidesShareVertices 0
		$sribbon SetCapping 1
		$sribbon Update
		
		if { [ [ [ $sribbon GetOutput ] GetPointData ] GetArray $field($filter) ] != "" } {
		    $fiberMapper ColorByArrayComponent $field($filter) 0
		}

		$fiberMapper SetInput [ $sribbon GetOutput ]
		if { $global_list(view_fiber_luk) } {
		    $fiberMapper ScalarVisibilityOn
		} else {
		    $fiberMapper ScalarVisibilityOff
		}
	    }

	    if { $cur_glyph == 2 } {
		
		$ribbon SetInput $polydata
		$ribbon SetWidth $global_list(view_fiber_glyphsize)
		$ribbon SetWidthFactor 5
		$ribbon UseDefaultNormalOff
		$ribbon Update
		
		if { [ [ [ $ribbon GetOutput ] GetPointData ] GetArray $field($filter) ] != "" } {
		    $fiberMapper ColorByArrayComponent $field($filter) 0
		}

		$fiberMapper SetInput [ $ribbon GetOutput ]
		if { $global_list(view_fiber_luk) } {
		    $fiberMapper ScalarVisibilityOn
		} else {
		    $fiberMapper ScalarVisibilityOff
		}
	    }

	     if { $cur_glyph < 2 } {

		
		if { [ [ $polydata GetPointData ] GetArray $field($filter) ] != "" } {
		    $fiberMapper ColorByArrayComponent $field($filter) 0
		}

		$fiberMapper SetInput $polydata
		if { $global_list(view_fiber_luk) } {
		    $fiberMapper ScalarVisibilityOn
		} else {
		    $fiberMapper ScalarVisibilityOff
		}

	    }

	    if { $cur_glyph == 0 } {
		[ $actor GetProperty ] SetRepresentationToPoints
		[ $actor GetProperty ] SetPointSize $global_list(view_fiber_pointsize)
		[ $actor GetProperty ] ShadingOff
	        } elseif { $cur_glyph == 1 } {
		    [ $actor GetProperty ] SetRepresentationToWireframe
		    [ $actor GetProperty ] ShadingOff
		} else {
		    [ $actor GetProperty ] SetRepresentationToSurface
		    [ $actor GetProperty ] ShadingOn
		}
	}
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in fiber filter selection
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_fiber_filter_cb { } {

    set field(0) fractani
    set field(1) meandiff
    set field(2) distance

    set field(3) angle
    
    set field(4) torsion

    set field(5) roi
    set field(6) other
    set field(7) minangle
    set field(8) maxangle
    set field(9) medangle
    set field(10) meanangle
    set field(11) varangle
   
    set field(12) mintorsion
    set field(13) maxtorsion
    set field(14) medtorsion 
    set field(15) meantorsion
    set field(16) vartorsion

    set field(17) minother
    set field(18) maxother
    set field(19) medother
    set field(20) meanother
    set field(21) varother

    set field(22) rgb

    ## current fiber colormap filter    
    set filter [ $widget_list(view_fiber_filter) index [ $widget_list(view_fiber_filter) get ]]
    
    foreach \
	actor $global_list(actor_fiber) {
	    
	    ## create mapper
	    set fiberMapper [ $actor GetMapper ]	   	    
	    $fiberMapper SetScalarModeToUsePointFieldData
	    
	    if { [ [ [ $fiberMapper GetInput ] GetPointData ] GetArray $field($filter) ] != "" } {
		$fiberMapper ColorByArrayComponent $field($filter) 0
	    }

	}
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  Toggle On/Off fiber visualization
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::view_toggle_fiber { } {
    
    if { $global_list(view_fiber) } {	
	view_create_fibers	
    } else { 
	foreach \
	    actor $global_list(actor_fiber) \
	    disp $global_list(fiber_disp_list) {
		if { $disp == 1 } {
		    [ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor
		}
		$actor Delete	    
	    }
	set global_list(actor_fiber) {}
    }

    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in bundle material properties
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::material_dialog { base } {

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
#  PRIVATE CALLBACK: Invoked upon a change in bundle material properties
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_material_cb { base } {

    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    if { $sel_item != "" } {
        
        set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 

        set result [ material_dialog $base ]

        if { $result } {
            
            set global_list(fiber_ambk_list) \
            [ lreplace $global_list(fiber_ambk_list) $sel_index $sel_index $global_list(mat_ambk)  ]

            set global_list(fiber_diffk_list) \
            [ lreplace $global_list(fiber_diffk_list) $sel_index $sel_index $global_list(mat_diffk)  ]

            set global_list(fiber_speck_list) \
            [ lreplace $global_list(fiber_speck_list) $sel_index $sel_index $global_list(mat_speck)  ]

            set global_list(fiber_specp_list) \
            [ lreplace $global_list(fiber_specp_list) $sel_index $sel_index $global_list(mat_specp)  ]

            set fiberActor [ lindex $global_list(actor_fiber) $sel_index ]

            #puts "A: $global_list(mat_ambk) D: $global_list(mat_diffk) S: $global_list(mat_speck)"

            [$fiberActor GetProperty] SetAmbient $global_list(mat_ambk)
            [$fiberActor GetProperty] SetDiffuse $global_list(mat_diffk)
            [$fiberActor GetProperty] SetSpecular $global_list(mat_speck)
            [$fiberActor GetProperty] SetSpecularPower $global_list(mat_specp)

             [ $parent GetViewer ] UpdateDisplay

        }

    }
  
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon a change in bundle color
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_color_cb { } {

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
	    #[$fiberActor GetProperty] SetColor $red $green $blue

    [$fiberActor GetProperty] SetAmbientColor $red $green $blue
    [$fiberActor GetProperty] SetDiffuseColor $red $green $blue
    [$fiberActor GetProperty] SetSpecularColor $red $green $blue

	    [ $parent GetViewer ] UpdateDisplay	    
	    
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon selection of bundles
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_selection_cb { } {
    
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

    set global_list(mat_ambk) [ lindex $global_list(fiber_ambk_list) $sel_index ]
    set global_list(mat_diffk) [ lindex $global_list(fiber_diffk_list) $sel_index ]
    set global_list(mat_speck) [ lindex $global_list(fiber_speck_list) $sel_index ]
    set global_list(mat_specp) [ lindex $global_list(fiber_specp_list) $sel_index ]

	set fiber_color [ image create photo -width 15 -height 15 ]
	$fiber_color put $global_list(fiber_color) -to 0 0 15 15	
	$widget_list(fiber_color) configure -image $fiber_color
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon change in bundle name
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_name_cb { } {

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
itcl::body mpjfibertracking::fiber_display_cb { } {

    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	## get current index
	set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 
	set global_list(fiber_disp_list) \
	    [ lreplace $global_list(fiber_disp_list) $sel_index $sel_index \
		  $global_list(fiber_display) ]
	
	set fiberActor [ lindex $global_list(actor_fiber) $sel_index ]
	
	if { $global_list(fiber_display) } {
	    [ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] AddActor $fiberActor
	} else {
	    [ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $fiberActor
	}
	
	[ $parent GetViewer ] UpdateDisplay	    
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Add bundle with a given name
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_addwithname { name polydata } {

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

    ## append material
    lappend global_list(fiber_ambk_list) $global_list(mat_ambk)
    lappend global_list(fiber_diffk_list) $global_list(mat_diffk)
    lappend global_list(fiber_speck_list) $global_list(mat_speck)
    lappend global_list(fiber_specp_list) $global_list(mat_specp)

    lappend global_list(vtk_fiber_polydata) $polydata	    
    lappend global_list(vtk_fiber_tube) [ vtkmpjTubeFilter [ pxvtable::vnewobj ] ]
    lappend global_list(vtk_fiber_ribbon) [ vtkRibbonFilter [ pxvtable::vnewobj ] ]
    lappend global_list(vtk_fiber_sribbon) [ vtkmpjRibbonFilter [ pxvtable::vnewobj ] ]
    lappend global_list(vtk_fiber_arrows) [ vtkGlyph3D [ pxvtable::vnewobj ] ]
 
    $widget_list(fiber_list) selection clear 0 end
    $widget_list(fiber_list) selection set $num_fibers $num_fibers

    return $num_fibers
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Add fiber
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_add { polydata } {

    ## insert new fiber
    set num_fibers [ llength $global_list(fiber_list) ]
    set name "Bundle_[expr $num_fibers + 1]"
    
    set index [ fiber_addwithname $name $polydata ]
    
    return $index
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Clear all fibers
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_clear { } {
    
    set num_fibers [ llength $global_list(fiber_list) ]
    for { set i 0 } { $i < $num_fibers } { incr i } { 
	fiber_remove 0
    }
    
    [ $parent GetViewer ] UpdateDisplay
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Remove fiber
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_remove { index } {

    if { $global_list(view_fiber) } {
	set display [ lindex $global_list(fiber_disp_list) $index ]
	if { $display } {
	    set actor [ lindex $global_list(actor_fiber) $index ]
	    [ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] RemoveActor $actor
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

    set ribbon [ lindex $global_list(vtk_fiber_ribbon) $index ]
    $ribbon Delete

    set sribbon [ lindex $global_list(vtk_fiber_sribbon) $index ]
    $sribbon Delete

    set arrows [ lindex $global_list(vtk_fiber_arrows) $index ]
    $arrows Delete

    set global_list(actor_fiber) \
	[ lreplace $global_list(actor_fiber) $index $index ]
    set global_list(vtk_fiber_tube) \
	[ lreplace $global_list(vtk_fiber_tube) $index $index ]
    set global_list(vtk_fiber_ribbon) \
	[ lreplace $global_list(vtk_fiber_ribbon) $index $index ]
    set global_list(vtk_fiber_sribbon) \
	[ lreplace $global_list(vtk_fiber_sribbon) $index $index ]
    set global_list(vtk_fiber_arrows) \
	[ lreplace $global_list(vtk_fiber_arrows) $index $index ]
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
itcl::body mpjfibertracking::fiber_removeselected { } {
    
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
itcl::body mpjfibertracking::fiber_load { } {
    
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
itcl::body mpjfibertracking::fiber_save_vtk { index name } {
    
    set field(0) "fractani"
    set field(1) "meandiff"
    set field(2) "distance"

    set field(3) "angle"
    
    set field(4) "torsion"

    set field(5) "roi"
    set field(6) "other"
    set field(7) "minangle"
    set field(8) "maxangle"
    set field(9) "medangle"
    set field(10) "meanangle"
    set field(11) "varangle"
   
    set field(12) "mintorsion"
    set field(13) "maxtorsion"
    set field(14) "medtorsion" 
    set field(15) "meantorsion"
    set field(16) "vartorsion"

    set field(17) "minother"
    set field(18) "maxother"
    set field(19) "medother"
    set field(20) "meanother"
    set field(21) "varother"

    set field(22) rgb

    ## get current glyph
    set cur_glyph [ $widget_list(view_fiber_glyph) \
            index [ $widget_list(view_fiber_glyph) get ] ]

    if { [expr $cur_glyph < 2 || $cur_glyph > 4 ] } {
        return
    }

    set fibvtk [ vtkPolyDataWriter [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $fibvtk "Saving fiber as vtk surface ..."
    $fibvtk SetFileTypeToBinary
    $fibvtk SetFileName $name.vtk

    ## current fiber colormap filter    
    set filter [ $widget_list(view_fiber_filter) index [ $widget_list(view_fiber_filter) get ]]

    set aa  [ vtkAssignAttribute [ pxvtable::vnewobj ] ]
    $aa Assign $field($filter) SCALARS POINT_DATA 

    set polydata [ lindex $global_list(vtk_fiber_polydata) $index ]

    if { $cur_glyph == 4 } {
    
        ## tubes
        set tube [ lindex $global_list(vtk_fiber_tube) $index ]
        
        $tube SetInput $polydata
        $tube SetRadius $global_list(view_fiber_glyphsize)
        $tube SetNormalEigenvector $global_list(track_norm_eigenvector1)
        $tube SetBinormalEigenvector $global_list(track_norm_eigenvector2)
        $tube SetNormalScale $global_list(view_fiber_eigenscale1)
        $tube SetBinormalScale $global_list(view_fiber_eigenscale2)
        $tube SetVaryRadius $global_list(view_fiber_eigenscale)
        $tube SetCapping 1
        $tube SetNumberOfSides $global_list(view_fiber_resolution)
        $tube SetGenerateTCoords 1
        $tube Update

        $aa SetInput [ $tube GetOutput ]

        $fibvtk SetInput [ $aa GetOutput ]

    }

    if { $cur_glyph == 3 } {
        
        set sribbon [ lindex $global_list(vtk_fiber_sribbon) $index ]

        $sribbon SetInput $polydata
        $sribbon SetRadius $global_list(view_fiber_glyphsize)
        $sribbon SetNormalScale $global_list(view_fiber_eigenscale1)
        $sribbon SetBinormalScale $global_list(view_fiber_eigenscale2)
        $sribbon SetVaryRadius $global_list(view_fiber_eigenscale)
        $sribbon SetNormalEigenvector $global_list(track_norm_eigenvector1)
        $sribbon SetBinormalEigenvector $global_list(track_norm_eigenvector2)
        $sribbon SetSidesShareVertices 0
        $sribbon SetCapping 1
        $sribbon SetGenerateTCoords 1
        $sribbon Update

        $aa SetInput [ $sribbon GetOutput ]

        $fibvtk SetInput [ $aa GetOutput ]
        
    }

    if { $cur_glyph == 2 } {
        
        set ribbon [ lindex $global_list(vtk_fiber_ribbon) $index ]

        $ribbon SetInput $polydata
        $ribbon SetWidth $global_list(view_fiber_glyphsize)
        $ribbon SetWidthFactor 5
        $ribbon UseDefaultNormalOff
        $ribbon SetGenerateTCoords 1
        $ribbon Update
        
        $aa SetInput [ $ribbon GetOutput ]

        $fibvtk SetInput [ $aa GetOutput ]

    }

    $fibvtk Write 
    
    $aa Delete

    $fibvtk Delete   

}

#-------------------------------------------------------------------------------------------
#  PRIVATE: save individual fiber 
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_save { index name } {
    
    set polydata [ lindex $global_list(vtk_fiber_polydata) $index ]
    
    set fibw [ vtkXMLPolyDataWriter [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $fibw "Saving fiber..."

    $fibw SetFileName $name.fib
    $fibw SetInput $polydata
    $fibw Write	
    
    $fibw Delete    

    if { $global_list(fiber_enablehist) == 1 } {
        ## save fibers in ascii formats
        set fibhist [ vtkmpjTensorStreamlineHistogramWriter [ pxvtable::vnewobj ] ]
        $this SetFilterCallbacks $fibhist "Saving fiber histogram as text..."

        puts [file join $name ".txt" ]

        $fibhist SetFileName $name.txt
        $fibhist SetInput $polydata
        $fibhist SetNumberOfBins $global_list(fiber_histbins)
        $fibhist Update

        $fibhist Delete    
    } else {
        ## save fibers in ascii formats
        set fibascii [ vtkmpjTensorStreamlineWriter [ pxvtable::vnewobj ] ]
        $this SetFilterCallbacks $fibascii "Saving fiber as text..."

        puts [file join $name ".txt" ]

        $fibascii SetFileName $name.txt
        $fibascii SetInput $polydata
        $fibascii Update

        $fibascii Delete    
    }

    if { $global_list(fiber_enablevtk) == 1 } {

        fiber_save_vtk $index $name
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: save selected fiber
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_saveselected { } {
    
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
	    
	    fiber_save $sel_index [ file join $fdir $item ]	
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: save all fibers
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_saveall { } {
    
    ## browse for directory    
    set fdir [ tk_chooseDirectory -title " Select directory" -parent $basewidget -initialdir $global_list(result_fdir) ]

    ## browse for directory
    if { $fdir != "" } { 
	
	## get number of fiber bundles
	set num [ llength $global_list(fiber_list) ]
	
	## save all fibers
	for { set i 0 } { $i < $num } { incr i } {	
	    set item [ lindex $global_list(fiber_list) $i ]	
	    fiber_save $i [ file join $fdir $item ]	
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save statistics
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_savestatistics { } {

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
itcl::body mpjfibertracking::fiber_beginstatistics { title overwrite } {   
    
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
itcl::body mpjfibertracking::fiber_addstatistics { title results tag } {   
    
    set min  [ format %8.4f [ lindex $results 0 ] ]
    set max  [ format %8.4f [ lindex $results 1 ] ]
    set mean [ format %8.4f [ lindex $results 2 ] ]
    set var  [ format %8.4f [ lindex $results 3 ] ]
    set dev  [ format %8.4f [ lindex $results 4 ] ]
    
    $widget_list(stat_text) insert end " $title |  $min  |  $max  |  $mean  |  $var  |  $dev  \n" $tag
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: is NaN
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::ispositive { x } {
    if { ![string is double $x] || $x != $x } {
        return 0
    } else {
        if { $x >= 0 } {
            return 1
        } else {
            return 0
        }
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: compute fiber statistics
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_statistics { } {

    set FIBERS 3
    set MAPQUANT 4

    set fiber_image [ lindex $global_list(result_list) $FIBERS ] 
    set map_image [ lindex $global_list(result_list) $MAPQUANT ]

#    $widget_list(notebook) view "Statistics"
    
    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	## get current index
	set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 
	
	## name
	set item [ lindex $global_list(fiber_list) $sel_index ]	
	
	# compute fiber statistics
	set fstat [ vtkmpjTensorStreamlineStatistics [ pxvtable::vnewobj ] ]
	$fstat SetInput [ lindex $global_list(vtk_fiber_polydata) $sel_index ]
	$fstat Update
	
	set vol "n/a"
	set volmm "n/a"

	if { [ $fiber_image GetImageSize ] > 1 } {
	    
	    ## quantize fibers
	    set quant [ vtkmpjStreamlineQuantization [ pxvtable::vnewobj ] ]
	    $quant SetInput [ lindex $global_list(vtk_fiber_polydata) $sel_index ]
	    $quant SetImage [ $fiber_image GetImage ]
	    $quant SetMaskValue 1.0
	    $quant Update
	    
	    $fiber_image ShallowCopyImage [ $quant GetOutput ]

	    ## get volume 
	    set vol [ $quant GetNumberOfVoxels ]

	    ## get spacing information
	    scan [ [ $fiber_image GetImage ] GetSpacing ] "%f %f %f" spc0 spc1 spc2

	    ## calculate volume in mm
	    set volmm [ expr $vol * $spc0 * $spc1 * $spc2 ]
	    set volmm [ format %6.2f $volmm ]

	    $quant Delete
	}
	
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

	set fa [ $fstat GetAnisotropy ]
	set tmpfa [ lindex $fa 3 ]
	if { [ $this ispositive $tmpfa ] } {
	    set tmpfa [ expr sqrt($tmpfa) ]
	} else {
	    set tmpfa 0
	}
	lappend fa $tmpfa
	fiber_addstatistics "FRACTIONAL ANISOTROPY" $fa odd

	set md [ $fstat GetDiffusivity ]
	set tmpmd [ lindex $md 3 ]
	if { [ $this ispositive $tmpmd ] } {
	    set tmpmd [ expr sqrt($tmpmd) ]
	} else {
	    set tmpmd 0
	}
	lappend md $tmpmd
	fiber_addstatistics "MEAN DIFFUSIVITY     " $md even	
	
	set dist [ $fstat GetDistance ]
	set tmpdist [ lindex $dist 3 ]
	if { [ $this ispositive $tmpdist ] } {
	    set tmpdist [ expr sqrt($tmpdist) ]
	} else {
	    set tmpdist 0
	}
	lappend dist $tmpdist
	fiber_addstatistics "MEAN FIBER LENGTH    " $dist odd
	
	set angle [ $fstat GetAngle ]
	set tmpangle [ lindex $angle 3 ]
	if { [ $this ispositive $tmpangle ] } {
	    set tmpangle [ expr sqrt($tmpangle) ]
	} else {
	    set tmpangle 0
	}
	lappend angle $tmpangle
	fiber_addstatistics "MEAN FIBER ANGLE     " $angle even	
	
	set other [ $fstat GetOther ]
	set tmpother [ lindex $other 3 ]
	if { [ $this ispositive $tmpother ] } {
	    set tmpother [ expr sqrt($tmpother) ]
	} else {
	    set tmpother 0
	}
	lappend other $tmpother
	fiber_addstatistics "MEAN INPUT MAP       " $other odd

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
itcl::body mpjfibertracking::fiber_filterangle { } {

    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	## create new fiber set
	set new_fiber [ vtkPolyData [ pxvtable::vnewobj ] ]

	## get current index
	set sel_index [ $widget_list(fiber_list) index [ lindex $sel_item 0 ] ] 
	
	## name
	set item [ lindex $global_list(fiber_list) $sel_index ]	
	
	# compute fiber statistics
	set anglefilter [ vtkmpjTensorStreamlineAngleFilter [ pxvtable::vnewobj ] ]
	$anglefilter SetInput [ lindex $global_list(vtk_fiber_polydata) $sel_index ]
	$anglefilter SetMinimumAngle $global_list(fiber_minangle)
	$anglefilter SetMaximumAngle $global_list(fiber_maxangle)
	$anglefilter SetMinimumTorsion $global_list(fiber_mintorsion)
	$anglefilter SetMaximumTorsion $global_list(fiber_maxtorsion)
	$anglefilter SetAngleOutside $global_list(fiber_invinterval)
	$anglefilter SetTorsionOutside $global_list(fiber_invtorsioninterval)	
	$anglefilter Update
	
	$new_fiber ShallowCopy [ $anglefilter GetOutput ]

	$anglefilter Delete

	## add new fiber bundle
	set index [ fiber_add $new_fiber ]
	
	set actor [ view_create_fiber_glyph $index ]
	lappend global_list(actor_fiber) $actor
	
	## add actor if to be displayed
	if { [ lindex $global_list(fiber_disp_list) $index ] == 1 } { 
	    [ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] AddActor $actor
	}
	
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: quantize fibers
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_quantize { } {

    set field(0) fractani
    set field(1) meandiff
    set field(2) distance

    set field(3) angle
    set field(4) torsion

    set field(5) roi
    set field(6) other
    set field(7) minangle
    set field(8) maxangle
    set field(9) medangle
    set field(10) meanangle
    set field(11) varangle

    set field(12) mintorsion
    set field(13) maxtorsion
    set field(14) medtorsion 
    set field(15) meantorsion
    set field(16) vartorsion

    set field(17) minother
    set field(18) maxother
    set field(19) medother
    set field(20) meanother
    set field(21) varother

    set field(22) rgb
    set FIBERS 3
    set MAPQUANT 4

    set fiber_image [ lindex $global_list(result_list) $FIBERS ]
    set map_image [ lindex $global_list(result_list) $MAPQUANT ] 

    ## get current selection 
    set sel_item [ $widget_list(fiber_list) getcurselection ]
    
    ## whether or not to add images
    set accum 0

    ##puts $sel_item

    if { [ $map_image GetImageSize ] > 1 } {

	if { $sel_item != "" } {

	    ## get current fiber colormap filter    
	    set filter [ $widget_list(view_fiber_filter) index [ $widget_list(view_fiber_filter) get ]]
	    
	    ## append bundles
	    set append [ vtkAppendPolyData [ pxvtable::vnewobj ] ]
	    
	    foreach bundle $sel_item {
	
		## get current index
		set sel_index [ $widget_list(fiber_list) index [ lindex $bundle 0 ] ] 
	    
		## name
		##set item [ lindex $global_list(fiber_list) $sel_index ]	

		## append individual bundle
		$append AddInput [ lindex $global_list(vtk_fiber_polydata) $sel_index ]

	    }

	    ## append all bundles
	    $append Update

	    ## quantize fibers
	    set quant [ vtkmpjStreamlineMapQuantization [ pxvtable::vnewobj ] ]
	    ##$quant SetInput [ lindex $global_list(vtk_fiber_polydata) $sel_index ]
	    $quant SetInput [ $append GetOutput ]
	    $quant SetImage [ $map_image GetImage ]
	    $quant SetQuantizationField $field($filter)
	    $quant Update
	    
	    ## add new map
	    $map_image ShallowCopyImage [ $quant GetOutput ]
	    
	    ## quantize fiber mask
	    set quantmask [ vtkmpjStreamlineQuantization [ pxvtable::vnewobj ] ]
	    $quantmask SetInput [ $append GetOutput ]
	    $quantmask SetImage [ $fiber_image GetImage ]
	    $quantmask SetMaskValue 1.0
	    $quantmask Update
	    
	    ## add new mask
	    $fiber_image ShallowCopyImage [ $quantmask GetOutput ]

	    $quantmask Delete
	    $quant Delete
	    $append Delete
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: fiber track
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::fiber_track { } {

    set TENSOR 0
    set MASK 1
    set MAP 2
    set FIBERS 3
    set MAPQUANT 4
    
    set tensor [ lindex $global_list(result_list) $TENSOR ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set map [ lindex $global_list(result_list) $MAP ]
    set fibers [ lindex $global_list(result_list) $FIBERS ] 
    set mapquant [ lindex $global_list(result_list) $MAPQUANT ]

    if { [ $tensor GetImageSize ] == 1 } {
	
	tk_messageBox -type ok \
	    -parent $basewidget -title "Tensor" \
	    -message "You need a tensor image to start tracking!" -icon info

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
    set levels [ [ [ $parent GetViewer ] GetOrthoImageSlice ] GetLevels ]
    set cx [ lindex $levels 0 ]
    set cy [ lindex $levels 1 ]
    set cz [ lindex $levels 2 ]

    ## get origin and spacing information
    scan [ [ $tensor GetImage ] GetSpacing ] "%f %f %f" spc0 spc1 spc2
    scan [ [ $tensor GetImage ] GetOrigin ] "%f %f %f" org0 org1 org2

    ## initialize streamer
    set streamer [ vtkmpjTensorStreamLine [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $streamer "Fiber tracking..."
    $streamer SetInput [ $tensor GetImage ]

    if { [ $map GetImageSize ] > 1 } {
    $streamer SetScalars [ $map GetImage ]
    }
    
    if { [ $mask GetImageSize ] > 1 } {
	$streamer SetMask [ $mask GetImage ]
    }
    
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
	
	set source [ vtkmpjImageROISource [ pxvtable::vnewobj ] ]
    $source SetMask [ $mask GetImage ]
    $source SetSourceToBox
	$source SetCenter $ox $oy $oz
	$source SetSize $width $height $depth
	$source SetOrigin $org0 $org1 $org2
	$source SetSpacing $spc0 $spc1 $spc2
	$source SetDensity $dens
	$source Update

	## assign source points
	$streamer SetSource [ $source GetOutput ]

	$source Delete

    } elseif { $global_list(track_seedtype) == 3 } {
	
    	## landmark control
    	set landmarkcontrol [ [ $parent GetLandmarkControl ] GetLandmarkControl ]
    	
    	set lmarks [ $landmarkcontrol GetCollection -1 ]	
    	$lmarks Compact
    	set numlmarks [ $lmarks GetNumPoints ]
    	
    	if { $numlmarks > 0 } {
    	    
    	    set pts [ vtkPolyData [ pxvtable::vnewobj ] ]
    	    $pts SetPoints [ $lmarks GetPoints ]
    	    
    	    $streamer SetSource $pts
    	    
    	    $pts Delete
    	}
    } elseif { $global_list(track_seedtype) == 4 } {

        ## sphere from locator 
        set ox [expr $cx * $spc0 + $org0 ]
        set oy [expr $cy * $spc1 + $org1 ]
        set oz [expr $cz * $spc2 + $org2 ]

        set center [ [ [ $parent GetViewer ] GetNavigationPointer ] GetPoint1 ]
        set ox [ lindex $center 0 ]
        set oy [ lindex $center 1 ]
        set oz [ lindex $center 2 ]

        set width $global_list(track_seedwidth)
        set height $global_list(track_seedheight)
        set depth $global_list(track_seeddepth)
        set dens $global_list(track_seeddens)
        set radius $global_list(track_seedradius) 
        
        #   set numx [ expr int(1.0 / $res) * $width ]
        #   set numy [ expr int(1.0 / $res) * $height ]
        #   set numz [ expr int(1.0 / $res) * $depth ]
        
        set source [ vtkmpjImageROISource [ pxvtable::vnewobj ] ]
        $source SetMask [ $mask GetImage ]
        $source SetSourceToSphere
        $source SetRadius $radius
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
    
#    puts "num seed points: [ [ [ $streamer GetSource ] GetPoints ] GetNumberOfPoints ]"
    
    ## integrate fibers
    if { $global_list(track_flipping) } {
	$streamer SetFlipX $global_list(track_flipx)
	$streamer SetFlipY $global_list(track_flipy)
	$streamer SetFlipZ $global_list(track_flipz)
    }

    if { $global_list(track_eigenvector) == 0 } { 
	$streamer IntegrateMajorEigenvector
	set global_list(track_norm_eigenvector1) 1
	set global_list(track_norm_eigenvector2) 2
    } elseif { $global_list(track_eigenvector) == 1 } {
	$streamer IntegrateMediumEigenvector
	set global_list(track_norm_eigenvector1) 0
	set global_list(track_norm_eigenvector2) 2
    } else {
	$streamer IntegrateMinorEigenvector
	set global_list(track_norm_eigenvector1) 0
	set global_list(track_norm_eigenvector2) 1
    }

    $streamer SetComputeNormals $global_list(track_norm_compute)
    $streamer SetNormalEigenvector $global_list(track_norm_eigenvector1)
    $streamer SetBinormalEigenvector $global_list(track_norm_eigenvector2)

    $streamer SetIntegrationDirectionToIntegrateBothDirections
    $streamer SetMaximumDistance $global_list(track_maxdist)
    $streamer SetMinimumDistance $global_list(track_mindist)
    if { $global_list(track_integration) == 0 } {
        $streamer SetIntegrator [ vtkRungeKutta2 [ pxvtable::vnewobj ] ]
    } elseif { $global_list(track_integration) == 1 } {
        $streamer SetIntegrator [ vtkRungeKutta4 [ pxvtable::vnewobj ] ]
    } else {
        $streamer SetIntegrator [ vtkRungeKutta45 [ pxvtable::vnewobj ] ]
    }
    #$streamer SetIntegrationOrder $global_list(track_integration)
    $streamer SetMaximumAngle $global_list(track_maxangle)
    $streamer SetMaximumTorsion $global_list(track_maxtorsion)
    $streamer SetMinimumFA $global_list(track_minFA);
    $streamer SetMaximumFA $global_list(track_maxFA);
    $streamer SetMinimumMD $global_list(track_minMD);
    $streamer SetMaximumMD $global_list(track_maxMD);
    $streamer SetIntegrationStepLength $global_list(track_steplen)
    $streamer SetNumberOfThreads $global_list(track_numthreads)
    $streamer SetConstrainPropagationToMask $global_list(track_constraintomask)

    $streamer SetComputeStatistics 1

    $streamer SetDirectionalColormap $global_list(track_dec)
    $streamer SetDirectionalColorScheme $global_list(view_direction_scheme)
    $streamer SetDirectionalColorSaturation $global_list(view_direction_saturation)
    $streamer SetDirectionalColorScaling $global_list(view_direction_scaling)
    $streamer SetDirectionalColorModulate $global_list(view_direction_modulate)
    $streamer SetDirectionalColorAlphaBlending $global_list(view_direction_alpha)

    $streamer Update
    
    ## create new fiber set
    set new_fiber [ vtkPolyData [ pxvtable::vnewobj ] ]

    ## filter fibers
    if { $global_list(track_regionfilter) } {	
	
        set center1 [ [ [ $parent GetViewer ] GetNavigationPointer ] GetPoint1 ]
        set cx1 [ lindex $center1 0 ]
        set cy1 [ lindex $center1 1 ]
        set cz1 [ lindex $center1 2 ]
        set radius1 $global_list(track_seedradius) 

        set center2 [ [ [ $parent GetViewer ] GetNavigationPointer ] GetPoint2 ]
        set cx2 [ lindex $center2 0 ]
        set cy2 [ lindex $center2 1 ]
        set cz2 [ lindex $center2 2 ]
        set radius2 $global_list(track_seedradius2) 

    	set strfilter [ vtkmpjTensorStreamlineROIFilter [ pxvtable::vnewobj ] ]
    	
    	$strfilter SetInput [ $streamer GetOutput ]
        $strfilter SetComputeStatistics 1
        $strfilter SetCenter1 $cx1 $cy1 $cz1
        $strfilter SetRadius1 $radius1
        $strfilter SetCenter2 $cx2 $cy2 $cz2
        $strfilter SetRadius2 $radius2
    	$strfilter SetClipping $global_list(track_clipping)
    	$strfilter Update

    	$new_fiber ShallowCopy [ $strfilter GetOutput ]

    	$strfilter Delete	

    } else {

	   $new_fiber ShallowCopy [ $streamer GetOutput ]

    }

    $streamer Delete
    
    set index [ fiber_add $new_fiber ]
    
    #puts "points: [[$new_fiber GetPoints] GetNumberOfPoints]"

    ## display new bundle
    if { $global_list(view_fiber) } {	
	
	   set actor [ view_create_fiber_glyph $index ]
	   lappend global_list(actor_fiber) $actor

	   ## add actor if to be displayed
	   if { [ lindex $global_list(fiber_disp_list) $index ] == 1 } { 
	        [ [ [ $parent GetViewer ] GetRenderer 3 ] GetRenderer ] AddActor $actor
	   }

	   [ $parent GetViewer ] UpdateDisplay
    }

    ## compute statistics
    if { $global_list(track_autostat) } {
	   fiber_statistics
    }

    
    WatchOff
}

#-------------------------------------------------------------------------------------------
#  Create results control
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::CreateInputControl { base } {   

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
itcl::body mpjfibertracking::CreateResultsControl { base } {   

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

    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -side top -fill x -padx 2 -pady 5

    set result_fpreffix [ iwidgets::entryfield $subframe2.result_fpreffix \
			      -textvariable [ itcl::scope global_list(result_fpreffix) ] \
			      -labeltext "Preffix:" ]
    pack $result_fpreffix -side left -fill x -expand t -padx 5 -pady 2
    set widget_list(result_fpreffix) $result_fpreffix    
}

#-------------------------------------------------------------------------------------------
#  Create directionality encoding display control
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::CreateDirectionControl { base } {
    
    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Directionally encoded colormaps"
    pack $base.frame0 -fill both -expand f
    
    set frame0 [ $base.frame0 childsite ]

    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -fill x -expand f -padx 5 -pady 5

    ## color encoding
    set track_dec [ checkbutton $subframe0.track_dec \
			-text "Enable fiber direction colormaps" \
			-variable [ itcl::scope global_list(track_dec) ] ]
    pack $track_dec -side left -padx 5 -pady 5      
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -fill both -expand f -padx 5      
    
    set view_direction_modulate [ checkbutton $subframe1.view_direction_modulate \
				      -text "Modulate intensity by fractional anisotropy" \
				      -variable [ itcl::scope global_list(view_direction_modulate) ] ]
    pack $view_direction_modulate -side left -padx 5 -pady 5

    set view_direction_alpha [ checkbutton $subframe1.view_direction_alpha \
				   -text "Enable transparency" \
				   -variable [ itcl::scope global_list(view_direction_alpha) ] ]
    pack $view_direction_alpha -side right -padx 5 -pady 5

    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -fill x -expand f -padx 5 -pady 5
  
    set view_direction_scheme [ iwidgets::optionmenu $subframe2.view_direction_scheme \
				    -labeltext "Color scheme:" \
				    -command [ itcl::code $this view_directionscheme_cb ] ]
    set widget_list(view_direction_scheme) $view_direction_scheme
    
    $view_direction_scheme insert 0 "Absolute value"
    $view_direction_scheme insert 1 "Rotational symmetry"
    $view_direction_scheme insert 2 "No symmetry"
    $view_direction_scheme insert 3 "Mirror symmetry"

    pack $view_direction_scheme -side left -padx 5
    
    $view_direction_scheme select 0    

    set subframe4 [ frame $frame0.subframe4 ]
    pack $subframe4 -fill x -expand f -padx 5 -pady 5
    
    iwidgets::entryfield $subframe4.view_direction_satscale \
	-width 5 \
	-validate real \
	-labeltext "Saturation factor:" \
	-textvariable [ itcl::scope global_list(view_direction_saturation) ]
    pack $subframe4.view_direction_satscale -side left -padx 5 -pady 5
    
    iwidgets::entryfield $subframe4.view_direction_scaling \
	-width 5 \
	-validate real \
	-labeltext "Brightness:" \
	-textvariable [ itcl::scope global_list(view_direction_scaling) ]
    pack $subframe4.view_direction_scaling -side left -padx 5 -pady 5

  
}

#-------------------------------------------------------------------------------------------
#  Create fiber tracking control
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::CreateTrackingControl { base } {

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
    $track_seedtype insert end "Landmark control"
    $track_seedtype insert end "Navigation widget"
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

     set track_seedradius [ iwidgets::entryfield $subframe1.track_seedradius \
                  -labeltext "Radius:" \
                  -validate integer \
                  -width 4 \
                  -state disabled \
                  -command [ itcl::code $this track_seedradius_cb ] \
                  -textvariable [ itcl::scope global_list(track_seedradius) ] ]
    pack $subframe1.track_seedradius -side left -padx 5 -pady 5 
    set widget_list(track_seedradius) $track_seedradius
    
    set track_seeddens [ iwidgets::entryfield $subframe1.track_seeddens \
			    -labeltext "Density:" \
			    -validate real \
			    -width 6 \
			    -state disabled \
			    -textvariable [ itcl::scope global_list(track_seeddens) ] ]
    pack $subframe1.track_seeddens -side left -padx 5 -pady 5
    set widget_list(track_seeddens) $track_seeddens

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
    pack $integration -side left -padx 2 -pady 5
    $integration insert end "Runge-Kutta 2"
    $integration insert end "Runge-Kutta 4"
    $integration insert end "Runge-Kutta 5"
    set widget_list(track_integration) $integration
    $widget_list(track_integration) select $global_list(track_integration)
    
    iwidgets::entryfield $subframe2.track_steplen \
	-width 5 \
	-validate real \
	-labeltext "Step length:" \
	-textvariable [ itcl::scope global_list(track_steplen) ] 
    pack $subframe2.track_steplen -side left -padx 2 -pady 5  
    
    set eigenvector [ iwidgets::optionmenu $subframe2.eigenvector \
			  -command [ itcl::code $this track_eigenvector_cb ] \
			  -labeltext "Eigenvector:" ]	
    pack $eigenvector -side right -padx 2 -pady 5
    $eigenvector insert end "Primary"
    $eigenvector insert end "Secondary"
    $eigenvector insert end "Tertiary"
    set widget_list(track_eigenvector) $eigenvector

    set subframe3 [ frame $frame1.subframe3 ]
    pack $subframe3 -fill both -expand f
    
    iwidgets::entryfield $subframe3.track_numthreads \
    -width 4 \
    -validate integer \
    -labeltext "Working threads:" \
    -textvariable [ itcl::scope global_list(track_numthreads) ] 
    pack $subframe3.track_numthreads -side left -padx 5 -pady 5  

    label $subframe3.dummy -text "Transform:"
    pack $subframe3.dummy -side left -expand f

    checkbutton $subframe3.flipx -text "Flip X" -variable [ itcl::scope global_list(track_flipx) ]
    pack $subframe3.flipx -pady 5 -side left
    checkbutton $subframe3.flipy -text "Flip Y" -variable [ itcl::scope global_list(track_flipy) ]
    pack $subframe3.flipy -pady 5 -side left
    checkbutton $subframe3.flipz -text "Flip Z" -variable [ itcl::scope global_list(track_flipz) ]
    pack $subframe3.flipz -pady 5 -side left

    checkbutton $subframe3.normalcompute -text "Track normals" -variable [ itcl::scope global_list(track_norm_compute) ]
    pack $subframe3.normalcompute -padx 5 -pady 5 -side right

    iwidgets::labeledframe $base.frame2 \
	-labelpos nw  \
	-labeltext "Stopping criteria"
    pack $base.frame2 -fill both -expand f
    
    set frame2 [ $base.frame2 childsite ]

    set subframe4 [ frame $frame2.subframe4 ]
    pack $subframe4 -fill both -expand f
    
    iwidgets::entryfield $subframe4.track_minFA \
	-width 4 \
	-validate real \
	-labeltext "Min FA:" \
	-textvariable [ itcl::scope global_list(track_minFA) ] 
    pack $subframe4.track_minFA -side left -padx 5 -pady 5  

    iwidgets::entryfield $subframe4.track_maxFA \
	-width 4 \
	-validate real \
	-labeltext "Max FA:" \
	-textvariable [ itcl::scope global_list(track_maxFA) ] 
    pack $subframe4.track_maxFA -side left -padx 5 -pady 5  

    iwidgets::entryfield $subframe4.track_minMD \
	-width 5 \
	-validate real \
	-labeltext "Min MD:" \
	-textvariable [ itcl::scope global_list(track_minMD) ] 
    pack $subframe4.track_minMD -side left -padx 5 -pady 5  
    
    iwidgets::entryfield $subframe4.track_maxMD \
	-width 5 \
	-validate real \
	-labeltext "Max MD:" \
	-textvariable [ itcl::scope global_list(track_maxMD) ] 
    pack $subframe4.track_maxMD -side left -padx 5 -pady 5  
    
    set subframe5 [ frame $frame2.subframe5 ]
    pack $subframe5 -fill both -expand f

    iwidgets::entryfield $subframe5.track_mindist \
	-width 5 \
	-validate real \
	-labeltext "Min length:" \
	-textvariable [ itcl::scope global_list(track_mindist) ] 
    pack $subframe5.track_mindist -side left -padx 5 -pady 5        

    iwidgets::entryfield $subframe5.track_maxdist \
	-width 5 \
	-validate real \
	-labeltext "Max length:" \
	-textvariable [ itcl::scope global_list(track_maxdist) ] 
    pack $subframe5.track_maxdist -side left -padx 5 -pady 5    

    iwidgets::entryfield $subframe5.track_maxangle \
	-width 5 \
	-validate real \
	-labeltext "Max angle:" \
	-textvariable [ itcl::scope global_list(track_maxangle) ] 
    pack $subframe5.track_maxangle -side left -padx 5 -pady 5  

  iwidgets::entryfield $subframe5.track_maxtorsion \
	-width 5 \
	-validate real \
	-labeltext "Max torsion:" \
	-textvariable [ itcl::scope global_list(track_maxtorsion) ] 
    pack $subframe5.track_maxtorsion -side left -padx 5 -pady 5  


    set subframe7 [ frame $frame2.subframe7 ]
    pack $subframe7 -fill both -expand f

    checkbutton $subframe7.track_regionfilter \
	-text "Keep fibers that intersect sphere radius:" \
	-variable [ itcl::scope global_list(track_regionfilter) ] \
	-command [ itcl::code $this track_regionfilter_cb ]    
    pack $subframe7.track_regionfilter -side left -padx 5

   set track_seedradius2 [ iwidgets::entryfield $subframe7.track_seedradius2 \
                  -validate integer \
                  -width 4 \
                  -state disabled \
                  -command [ itcl::code $this track_seedradius_cb ] \
                  -textvariable [ itcl::scope global_list(track_seedradius2) ] ]
    pack $subframe7.track_seedradius2 -side left -padx 0 -pady 5 
    set widget_list(track_seedradius2) $track_seedradius2
    
    ## clipping
    set track_clipping [ checkbutton $subframe7.track_clipping \
                 -text "Clipping" -state disabled\
                 -variable [ itcl::scope global_list(track_clipping) ] ]
    pack $track_clipping -side left -padx 10       
    set widget_list(track_clipping) $track_clipping  

    checkbutton $subframe7.track_constraintomask \
    -text "Constrain to mask" \
    -variable [ itcl::scope global_list(track_constraintomask) ] 
    pack $subframe7.track_constraintomask -side left -padx 10
    
    set trackbtn [ iwidgets::buttonbox $base.trackbtn -orient horizontal ]
    $trackbtn add track -text "Track!" -command [ itcl::code $this fiber_track ] -state disabled
    pack $trackbtn -side left -expand f
    set widget_list(trackbtn) $trackbtn

    ## auto stat
    set track_autostat [ checkbutton $base.track_autostat \
			     -text "Auto fiber statistics" \
			     -variable [ itcl::scope global_list(track_autostat) ] ]
    pack $track_autostat -side right -padx 5  
}

#-------------------------------------------------------------------------------------------
#  Create fiber tracking control
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::CreateFiberControl { base } {
    
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

    button $subframe1.fiber_material \
    -text "Material" \
    -command [ itcl::code $this fiber_material_cb $base ]
    pack $subframe1.fiber_material -side left -padx 10 -pady 5
    set widget_list(fiber_material) $subframe1.fiber_material

    set fiber_name [ iwidgets::entryfield $subframe1.fiber_name \
			 -textvariable [ itcl::scope global_list(fiber_name) ] \
			 -labeltext "Label:" \
			 -width 30 \
			 -command [ itcl::code $this fiber_name_cb ] ] 
    pack $fiber_name -side left -pady 5 -fill x -expand t
    set widget_list(fiber_name) $fiber_name
    
    checkbutton $subframe1.fiber_display \
	-text "Display" \
	-variable [ itcl::scope global_list(fiber_display) ] \
	-command [ itcl::code $this fiber_display_cb ]    
    pack $subframe1.fiber_display -side left -padx 5


    set beforebottom [ frame $frame0.beforebottom ]
    pack $beforebottom -fill both -expand f -side bottom -pady 2

    iwidgets::entryfield $beforebottom.fiber_mintorsion\
	-width 5 \
	-validate real \
	-labeltext "Torsion Min:" \
	-textvariable [ itcl::scope global_list(fiber_mintorsion) ] 
    pack $beforebottom.fiber_mintorsion -side left -padx 5 -pady 5  

    iwidgets::entryfield $beforebottom.fiber_maxtorsion \
	-width 5 \
	-validate real \
	-labeltext "Max:" \
	-textvariable [ itcl::scope global_list(fiber_maxtorsion) ] 
    pack $beforebottom.fiber_maxtorsion -side left -padx 5 -pady 5  

    set fiber_invtorsioninterval [ checkbutton $beforebottom.fiber_invtorsioninterval \
				-text "Outside" \
				-variable [ itcl::scope global_list(fiber_invtorsioninterval) ] ]
    pack $fiber_invtorsioninterval -side left       
    set widget_list(track_invtorsioninterval) $fiber_invtorsioninterval

  set bottom [ frame $frame0.bottom ]
    pack $bottom -fill both -expand f -side bottom -pady 2

    iwidgets::entryfield $bottom.fiber_minangle \
	-width 5 \
	-validate real \
	-labeltext "Angle Min:" \
	-textvariable [ itcl::scope global_list(fiber_minangle) ] 
    pack $bottom.fiber_minangle -side left -padx 5 -pady 5  

    iwidgets::entryfield $bottom.fiber_maxangle \
	-width 5 \
	-validate real \
	-labeltext "Max:" \
	-textvariable [ itcl::scope global_list(fiber_maxangle) ] 
    pack $bottom.fiber_maxangle -side left -padx 5 -pady 5  

    set fiber_invinterval [ checkbutton $bottom.fiber_invinterval \
				-text "Outside" \
				-variable [ itcl::scope global_list(fiber_invinterval) ] ]
    pack $fiber_invinterval -side left       
    set widget_list(track_invinterval) $fiber_invinterval


    iwidgets::entryfield $bottom.fiber_histbins \
    -width 5 \
    -validate integer \
    -labeltext "Bins:" \
    -textvariable [ itcl::scope global_list(fiber_histbins) ] 
    pack $bottom.fiber_histbins -side right -padx 10 -pady 5  


    set fiber_enablehist [ checkbutton $bottom.enablehist \
                -text "Calculate histogram" \
                -variable [ itcl::scope global_list(fiber_enablehist) ] ]
    pack $fiber_enablehist -side right -padx 2 -pady 5  


    button $beforebottom.fiber_filter -text "Filter fibers" \
	-command [ itcl::code $this fiber_filterangle ]
    pack $beforebottom.fiber_filter -side left -padx 5

    set fiber_enablevtk [ checkbutton $beforebottom.enablevtk \
                -text "Save as surface" \
                -variable [ itcl::scope global_list(fiber_enablevtk) ] ]
    pack $fiber_enablevtk -side right -padx 5 -pady 5  
    

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

    set fiberbbox [ iwidgets::buttonbox $subframe3.fiberbbox -orient vertical ]
    $fiberbbox add load -text "Load" -command [ itcl::code $this fiber_load ]
    $fiberbbox add save -text "Save" -command [ itcl::code $this fiber_saveselected ]
    $fiberbbox add saveall -text "Save all" -command [ itcl::code $this fiber_saveall ]
    $fiberbbox add remove -text "Remove" -command [ itcl::code $this fiber_removeselected ]
    $fiberbbox add clear -text "Clear" -command [ itcl::code $this fiber_clear ]
    $fiberbbox add stat -text "Statistics" -command [ itcl::code $this fiber_statistics ]
    $fiberbbox add quant -text "Quantize" -command [ itcl::code $this fiber_quantize ]
    pack $fiberbbox -side left -expand t
    set widget_list(fiber_buttonbox) $fiberbbox        
}

#-------------------------------------------------------------------------------------------
#  Create statistics control
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::CreateStatisticsControl { base } {

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
#  Create fiber display control
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::CreateFiberDisplayControl { base } {

    set subframe0 [ frame $base.subframe0 ]
    pack $subframe0 -fill x -side top -expand f -padx 5 -pady 5
    
    set view_fiber_filter \
	[ iwidgets::optionmenu $subframe0.view_fiber_filter -labeltext "Filter:" ]
    
    $view_fiber_filter configure -command [ itcl::code $this view_fiber_filter_cb ]
    $view_fiber_filter insert end "Fractional anisotropy"
    $view_fiber_filter insert end "Mean diffusivity"
    $view_fiber_filter insert end "Fiber distance"
    $view_fiber_filter insert end "Fiber angle"
    $view_fiber_filter insert end "Fiber torsion"
    $view_fiber_filter insert end "Region number"
    $view_fiber_filter insert end "Input map"

    $view_fiber_filter insert end "Min fiber angle"
    $view_fiber_filter insert end "Max fiber angle"
    $view_fiber_filter insert end "Median fiber angle"
    $view_fiber_filter insert end "Mean fiber angle"
    $view_fiber_filter insert end "Variance of fiber angle"

    $view_fiber_filter insert end "Min torsion"
    $view_fiber_filter insert end "Max torsion"
    $view_fiber_filter insert end "Median torsion"
    $view_fiber_filter insert end "Mean torsion"
    $view_fiber_filter insert end "Variance of torsion"

    $view_fiber_filter insert end "Min input map"
    $view_fiber_filter insert end "Max input map"
    $view_fiber_filter insert end "Median input map"
    $view_fiber_filter insert end "Mean input map"
    $view_fiber_filter insert end "Variance of input map"
    $view_fiber_filter insert end "Directionality"

    
    pack $view_fiber_filter -side left -padx 5
    set widget_list(view_fiber_filter) $view_fiber_filter
    
    set view_fiber_glyph [ iwidgets::optionmenu $subframe0.view_fiber_glyph \
				-labeltext "View fibers as:"   \
				-command [ itcl::code $this view_fiber_glyph_cb ] ] \
	
    pack $view_fiber_glyph -side left -padx 5
    $view_fiber_glyph insert end "Points"
    $view_fiber_glyph insert end "Lines"
    $view_fiber_glyph insert end "Ribbons"
    $view_fiber_glyph insert end "Solid ribbons"
    $view_fiber_glyph insert end "Tubes"
    $view_fiber_glyph insert end "Tangents"
    $view_fiber_glyph insert end "Normals"
    $view_fiber_glyph insert end "Binormals"
    set widget_list(view_fiber_glyph) $view_fiber_glyph
    
    set subframe1 [ frame $base.subframe1 ]
    pack $subframe1 -side top -fill both -padx 5 -expand f
    
    iwidgets::entryfield $subframe1.view_fiber_glyphscale \
	-width 5 \
	-validate real \
	-labeltext "Glyph scale:" \
	-textvariable [ itcl::scope global_list(view_fiber_glyphsize) ]  \
	-command [ itcl::code $this view_fiber_glyphsize_cb ]
    pack $subframe1.view_fiber_glyphscale -side left -padx 5    


    set view_fiber_eigenscale [ checkbutton $subframe1.view_fiber_eigenscale \
				    -text "Eigenscale:" \
				    -variable [ itcl::scope global_list(view_fiber_eigenscale) ] \
				    -command [ itcl::code $this view_fiber_glyphsize_cb ] ]
    pack $view_fiber_eigenscale -side left -pady 5 -padx 5       
    set widget_list(view_fiber_eigenscale) $view_fiber_eigenscale

    
    iwidgets::entryfield $subframe1.view_fiber_eigenscale1 \
	-width 5 \
	-validate real \
	-textvariable [ itcl::scope global_list(view_fiber_eigenscale1) ]  \
	-command [ itcl::code $this view_fiber_glyphsize_cb ]
    pack $subframe1.view_fiber_eigenscale1 -side left -padx 2    
    
    iwidgets::entryfield $subframe1.view_fiber_eigenscale2 \
	-width 5 \
	-validate real \
	-textvariable [ itcl::scope global_list(view_fiber_eigenscale2) ]  \
	-command [ itcl::code $this view_fiber_glyphsize_cb ]
    pack $subframe1.view_fiber_eigenscale2 -side left -padx 2  


    iwidgets::entryfield $subframe1.view_fiber_resolution \
	-width 5 \
	-validate integer \
	-labeltext "Resolution:" \
	-textvariable [ itcl::scope global_list(view_fiber_resolution) ] \
	-command [ itcl::code $this view_fiber_glyphsize_cb ]
    pack $subframe1.view_fiber_resolution -side left -padx 5 

    #set view_fiber_luk [ checkbutton $subframe1.view_fiber_luk \
#			     -text "Apply colormap" \
#			     -variable [ itcl::scope global_list(view_fiber_luk) ] \
#			     -command [ itcl::code $this view_fiber_colormap_cb ] ]
 #   pack $view_fiber_luk -side left -pady 5 -padx 5       
  #  set widget_list(view_fiber_luk) $view_fiber_luk

    set subframe11 [ frame $base.subframe11 ]
    pack $subframe11 -side top -fill both -padx 5

    iwidgets::entryfield $subframe11.view_fiber_tipradius \
	-width 5 \
	-validate real \
	-labeltext "Tip radius:" \
	-textvariable [ itcl::scope global_list(view_fiber_tipradius) ] \
	-command [ itcl::code $this view_fiber_glyphsize_cb ]
    pack $subframe11.view_fiber_tipradius -side left -padx 5 
    
    iwidgets::entryfield $subframe11.view_fiber_shaftradius \
	-width 5 \
	-validate real \
	-labeltext "Shaft radius:" \
	-textvariable [ itcl::scope global_list(view_fiber_shaftradius) ] \
	-command [ itcl::code $this view_fiber_glyphsize_cb ]
    pack $subframe11.view_fiber_shaftradius -side left -padx 5 

    iwidgets::entryfield $subframe11.view_fiber_tiplength \
	-width 5 \
	-validate real \
	-labeltext "Tip length:" \
	-textvariable [ itcl::scope global_list(view_fiber_tiplength) ] \
	-command [ itcl::code $this view_fiber_glyphsize_cb ]
    pack $subframe11.view_fiber_tiplength -side left -padx 5 

    set view_fiber_luk [ checkbutton $subframe11.view_fiber_luk \
			     -text "Apply colormap" \
			     -variable [ itcl::scope global_list(view_fiber_luk) ] \
			     -command [ itcl::code $this view_fiber_colormap_cb ] ]
    pack $view_fiber_luk -side left -pady 5 -padx 5       
    set widget_list(view_fiber_luk) $view_fiber_luk


    set subframe2 [ frame $base.subframe2 ]
    pack $subframe2 -side left -fill both -padx 5 
    
    set subframe21 [ frame $subframe2.subframe21 ]
    pack $subframe21 -side top -fill both -expand t -padx 5 

    set fiber_luk [ mpjguilookuptable \#auto 0 ]
    $fiber_luk Initialize $subframe21
    pack forget [ $fiber_luk GetButtonBoxWidget ]
#    pack forget [ $fiber_luk GetMenuBar ]     
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
itcl::body mpjfibertracking::CreateDisplayControl { base } {

    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Properties"
    pack $base.frame0 -fill both -expand t 

    set frame0 [ $base.frame0 childsite ]
    
    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -fill both -padx 5 -pady 5 -expand f
    
    checkbutton $subframe0.view_fibers -text "View fibers" \
	-variable [ itcl::scope global_list(view_fiber) ] \
	-command [ itcl::code $this view_toggle_fiber ]   
    pack $subframe0.view_fibers -side left -padx 5

    checkbutton $subframe0.view_colorbar -text "View colorbar" \
	-variable [ itcl::scope global_list(view_colorbar) ] \
	-command [ itcl::code $this view_toggle_colorbar ]   
    pack $subframe0.view_colorbar -side left -padx 5
    
    iwidgets::entryfield $subframe0.view_colorbar_label \
	-width 15 \
	-labeltext "Input Map label:" \
	-textvariable [ itcl::scope global_list(view_colorbar_label) ]
    pack $subframe0.view_colorbar_label -side left -padx 5    

    set subframe1 [ frame $frame0.subframe1 ] 
    pack $subframe1 -fill both -expand t -padx 2 -pady 2

    CreateFiberDisplayControl $subframe1
}

# -------------------------------------------------------------------------------------------
#  Diffusion control inititialization
# ------------------------------------------------------------------------------------------
itcl::body mpjfibertracking::Initialize { widget } {
    
    if { $initialized == 1 } { return $basewidget }
    
    #  -------------------------------------
    #  Create User Interface
    #  -------------------------------------	
    set basewidget [toplevel $widget ]
    wm geometry $basewidget 650x520
    wm withdraw $basewidget

    set notebook $basewidget.notebook    
    iwidgets::tabnotebook $notebook -tabpos w         
    set widget_list(notebook) $notebook

    set mb [ frame $basewidget.mb ]
    pack $mb -side top -fill x -expand false
    
    CreateMenu $mb

    CreateInputControl      [ $notebook add -label "Input" ] 
    CreateDirectionControl  [ $notebook add -label "Directionality" ]    
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
    menubutton $mb.tracking -text Tracking -menu $mb.tracking.m -underline 0
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
    eval "$mb add command -label \"Fiber tracking\" -command {$this ShowWindow \"Input\"}"
}

#-------------------------------------------------------------------------------------------
#  Main function for stand-alone execution
# ------------------------------------------------------------------------------------------
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    puts "\n[ file rootname $argv0 ] is not a stand-alone program.\n"
    exit

}

