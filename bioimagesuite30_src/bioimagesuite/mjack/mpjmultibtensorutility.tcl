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

package provide mpjmultibtensorutility 1.3

package require  Itcl 3.2
package require  Iwidgets 4.0

package require  pxitcllistselector 1.0
package require  pxitclbaseimagecontrol 1.0
package require  pxitclbaseimageviewer 1.0

package require  vtkmpjcontrib 1.1
package require  mpjguilookuptable 1.0
package require  mpjguisphere 1.0
package require  vtkinteraction

itcl::class mpjmultibtensorutility {

    inherit pxitclbaseimagecontrol
    
    #-----------------------------------
    # construction and descruction
    #-----------------------------------
    constructor { par } {
	pxitclbaseimagecontrol::constructor $par
    } { InitializeControl }
    
    destructor { }

    # Generic Stuff 
    protected variable setupfilename "subject.dti"

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
      
    # ----------------------------
    # Setup Stuff
    # ----------------------------
    public method LoadSetup { args } 
    public method SaveSetup { args } 
    protected method GetFileElement { fileid } 

    #-----------------------------------
    # diffusion series input/output
    #-----------------------------------
    public method AddInput { fname }
    public method RemoveInput { index }
    public method RemoveAllInputs { }
    public method DisplayInput { index frame }
    public method LoadGradient { fname }
    public method LoadMask { fname }
    public method ComputeResults { }
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
    private method CreateMaskControl { base }
    private method CreateResultsControl { base }
    private method CreateGradientControl { base }
    private method CreateTransformControl { base }
    private method CreateDistributionControl { base }
    private method CreateMenu { mb }
    public method AddToMenuButton { mb args}    
 
    private method addinput { }
    private method removeinput { }
    private method displayinput { }
    private method loadgradient { }
    private method checkgradient { }
    private method loadmask { }
    private method displayresult { { mode image } }
    private method saveresult { }
    private method saveallresults { }

    private method addbvalue { }
    private method removebvalue { }
  
    private method input_updateinfo { index }    
    private method input_selectioncommand_cb { } 
    private method gradient_searchlist { dir }
    private method gradient_populatelist { index }
    private method gradient_increment_ndir { dir }
    private method gradient_showantipodal { }
    
    private method result_clear { }
    private method result_createbaseline { }
    private method result_createtensor { }
    private method result_createeigen { }
}  

#-------------------------------------------------------------------------------------------
#  Close window 
# -------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::DismissWindow { } {

    set ok "yes"
    
    if { [ llength $global_list(input_list) ] > 0 } {
    
       set ok [ tk_messageBox -type yesnocancel -default no \
             -parent $basewidget -title "Think again ..." \
             -message "Your work session will be lost. Are you sure ?" -icon question  ]

    }
    
    if { $ok != "yes" } {
        return
    }

	$this ClearAllResults
	$this RemoveAllInputs

	$global_list(gradient_points) Delete
	$global_list(gradient_polydata) Delete
	$global_list(gradient_actor) Delete
	$global_list(gradient_anti_actor) Delete
	    
	$this SetTitle $global_list(appname)
		
	set global_list(mask_fname) "select"
	set global_list(tensor_fname) ""

    [ $parent GetViewer ] UpdateDisplay
	$this DestroyWindow
}

#-------------------------------------------------------------------------------------------
#  Initialize tool
# -------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::InitializeControl { } {
    
    set global_list(appname) "(EXPERIMENTAL) Multiple b-Value Diffusion Tensor Utility"
    
    set appname "$global_list(appname) $version"
    set aboutstring "(c) Marcel Jackowski 2013"
        
    set dir6 "{0.707 0.0 0.707} {-0.707 0.0 0.707} {0.0 0.707 0.707} {0.0 0.707 -0.707} {0.707 0.707 0.0} {-0.707 0.707 0.0}"

    set dir6i "{0.000000  0.525731 -0.850651} {0.000000 -0.525731 -0.850651} \
               {0.525731 -0.850651 0.000000} {0.525731  0.850651 0.000000} \
               {-0.850651 0.000000 0.525731} {-0.850651 0.000000 -0.525731}"

    set dir10i "{-0.577300 0.577300 0.577300} {-0.577300 0.577300 -0.577300} \
                {0.577300 0.577300 0.577300} {0.577300 0.577300 -0.577300} \
                {0.000000 -0.934200 0.356800} {0.000000 0.934200 0.356800} \
                {-0.356800 0.000000 0.934200} {0.356800 0.000000 0.934200} \
                {-0.934200 0.356800 0.000000} {0.934200 0.356800 0.000000}"

    set dir15i "{1.000000 0.000000 0.000000} {0.000000 1.000000 0.000000} \
                {0.000000 0.000000 1.000000} {0.309017 -0.809017 -0.500000} \
                {0.309017 0.809017 -0.500000} {-0.309017 -0.809017 -0.500000} \
                {0.309017 -0.809017 0.500000} {-0.500000 0.309017 0.809017} \
                {0.500000 -0.309017 0.809017} {0.500000 0.309017 -0.809017} \
                {0.500000 0.309017 0.809017} {-0.809017 0.500000 0.309017} \
                {0.809017 -0.500000 0.309017} {0.809017 0.500000 -0.309017} \
                {-0.809017 -0.500000 -0.309017}"

    set dir21i "{1.000000 0.000000 0.000000} {0.000000 1.000000 0.000000} \
                {0.000000 0.000000 1.000000} {0.000000 0.525731 -0.850651} \
                {0.000000 -0.525731 -0.850651} {0.525731 -0.850651 0.000000} \
                {0.525731 0.850651 0.000000} {-0.850651 0.000000 0.525731} \
                {-0.850651 0.000000 -0.525731} {0.309017 -0.809017 -0.500000} \
                {0.309017 0.809017 -0.500000} {-0.309017 -0.809017 -0.500000} \
                {0.309017 -0.809017 0.500000} {-0.500000 0.309017 0.809017} \
                {0.500000 -0.309017 0.809017} {0.500000 0.309017 -0.809017} \
                {0.500000 0.309017 0.809017} {-0.809017 0.500000 0.309017} \
                {0.809017 -0.500000 0.309017} {0.809017 0.500000 -0.309017} \
                {-0.809017 -0.500000 -0.309017}"

    set dir24 "{0.134985506170 -0.985952791173 0.098366695086} \
               {0.650088694587 -0.462793906466 -0.602666150790} \
               {-0.206347275695 0.662332382064 0.720233724205} \
               {-0.435854930194 0.350113596741 -0.829126618318} \
               {0.985232180539 0.063955280317 -0.158830955895} \ 
               {-0.614137826481 -0.392074584561 -0.684917695951} \
               {0.353418655052 -0.587198098366 0.728212638957} \ 
               {-0.944785803171 0.119466198906 -0.305135401821} \ 
               {0.021302190861 -0.808690754556 -0.587848177815} \
               {0.090322707483 0.786736539321 -0.610645090220} \
               {0.019144141922 0.992112273707 0.123881952644} \
               {0.685499579269 0.715694962504 -0.133682637122} \
               {-0.492664112126 -0.044270667252 0.869092734203} \
               {-0.582890874715 -0.806420953907 -0.099616631511} \
               {0.048922860209 -0.155267715919 -0.986660270885} \
               {0.542444409187 0.622999428166 0.563582980089} \
               {0.763772109691 -0.640056077552 0.083548680702} \
               {-0.913538674752 -0.294880095995 0.280165698682} \
               {-0.347061443006 -0.702169846181 0.621695956149} \
               {0.842177009528 -0.028740243511 0.538434659941} \
               {0.207815962624 0.070701024559 0.975609497086} \
               {-0.578727030552 0.786414267629 -0.215934304310} \
               {-0.804218513225 0.456634532848 0.380417515892} \
               {0.575100475666 0.281355246534 -0.768178799588}"

    set dir32 "{0.127839994324 0.905079981288 -0.405570170652} {0.160624455635 0.551408286854 0.818626096237} {-0.660076326339 -0.746272212908 0.085889624794} {0.429182719745 -0.024323513182 0.902890115008} {0.017138782122 0.483370449508 -0.875248119501} {-0.017138782496 -0.483370449559 0.875248119465} {0.879670240941 -0.126024235489 0.458582772542} {0.372449269958 -0.928034884037 0.005726719673} {-0.615867796457 -0.445295752660 0.649937343096} {-0.420650772436 0.691083196582 0.587755853268} {-0.857022691548 0.512647555396 0.052005673898} {0.977673466973 0.189673233358 -0.090435924964} {-0.372449268149 0.928034884765 -0.005726719413} {-0.271608831281 0.099041670392 0.957297963173} {0.660076322812 0.746272216769 -0.085889618361} {-0.518036777328 0.644904845847 -0.561903583490} {0.420650774711 -0.691083195657 -0.587755852727} {0.713253817668 0.491474092923 0.499722130356} {0.271608830136 -0.099041670365 -0.957297963501} {-0.429182720907 0.024323508230 -0.902890114589} {0.518036776546 -0.644904846374 0.561903583606} {-0.790991214336 0.168073897971 0.588289098713} {0.615867795454 0.445295757555 -0.649937340692} {-0.713253819444 -0.491474095234 -0.499722125549} {-0.977673467286 -0.189673230240 0.090435928118} {-0.160624455963 -0.551408291328 -0.818626093159} {0.857022693105 -0.512647552636 -0.052005675458} {0.790991215299 -0.168073890326 -0.588289099602} {-0.188027854368 -0.933619777304 -0.304958419146} {-0.879670240234 0.126024236041 -0.458582773747} {-0.127839994442 -0.905079980679 0.405570171973} {0.188027854008 0.933619775705 0.304958424265}"

    # create initial set of gradient directions
    set global_list(gradient_list) "{6 ME6 {$dir6}} {6 ICO6 {$dir6i}} \
                                    {10 ICO10 {$dir10i}} \
                                    {15 ICO15 {$dir15i}} {21 ICO21 {$dir21i}} \
                                    {24 ME24 {$dir24}} {32 ME32 {$dir32}}"
    
    # create image titles
    set global_list(result_title) \
	[ list \
	      { "Baseline image"                    "dti_baseline" } \
	      { "Anatomical mask"                   "dti_mask" }  \
	      { "Diffusion tensor"                  "dti_tensor" } \
	      { "Mean diffusion-weighted"           "dti_mean" } ]

    foreach item $global_list(result_title) {
 	lappend result_list [ pxitclimage \#auto ]
    }  
    
    set global_list(result_fpreffix) "preffix_"
    set global_list(result_fdir) [ pwd ]
    set global_list(result_fmask) "*.hdr"
    set global_list(result_list) $result_list
    
    set global_list(input_fname) {}   ; # list of filenames
    set global_list(input_list) {}    ; # list of input images
    set global_list(input_blist) {0 1000}    ; list of b factors
    set global_list(input_ndw) 1
    set global_list(input_dw) 6
    set global_list(input_frameno) 0
    set global_list(input_tensorsymm) 1
    set global_list(input_curbvalue) 1000
    set global_list(input_bfactorenable) 0
    
    set global_list(tensor_ordering) 0
    set global_list(tensor_flipx) 0
    set global_list(tensor_flipy) 0
    set global_list(tensor_flipz) 0
    set global_list(tensor_swap) 0
    set global_list(tensor_swapdir) 0
    set global_list(tensor_swapyz) 0
    set global_list(tensor_swapxz) 0
    set global_list(tensor_swapxy) 0

    set global_list(tensor_rotation) 0
    set global_list(tensor_rotx) 0.0
    set global_list(tensor_roty) 0.0
    set global_list(tensor_rotz) 0.0
    
    set global_list(gradient_fname) "select"
    set global_list(gradient_fdir) [ pwd ]
    set global_list(gradient_tol) 1E-6
    set global_list(gradient_showanti) 1
    
    set global_list(mask_fname) "select"

    set global_list(gradient_points) [ vtkPoints [ pxvtable::vnewobj ] ]
    set global_list(gradient_polydata) [ vtkmpjVertexSource [ pxvtable::vnewobj ] ]
    set global_list(gradient_actor) [ vtkActor [ pxvtable::vnewobj ] ]
    set global_list(gradient_anti_actor) [ vtkActor [ pxvtable::vnewobj ] ]

    [ $global_list(gradient_anti_actor) GetProperty ] SetRepresentationToPoints
    [ $global_list(gradient_anti_actor) GetProperty ] SetColor 0.0 0.0 1.0
    [ $global_list(gradient_anti_actor) GetProperty ] SetPointSize 3.0
    
    [ $global_list(gradient_actor) GetProperty ] SetRepresentationToPoints
    [ $global_list(gradient_actor) GetProperty ] SetColor 1.0 0.0 0.0
    [ $global_list(gradient_actor) GetProperty ] SetPointSize 3.0
    
}

# ---------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::SaveSetup { args } {

    set len [ llength $args ]
    
    if { $len < 1 } {
	set fname $setupfilename 
    } else {
	set fname [ lindex $args 0 ]
    }

    if { [ string length $fname  ] == 0  } {
	set f1 $setupfilename
	set f1path [ file dirname $f1 ]
	set f1tail [ file tail    $f1 ]
	set fname  [tk_getSaveFile -title "Saving Setup File" -filetypes { {"DTI-Setup File" {.dti}}} -initialfile $f1tail -initialdir $global_list(result_fdir) ]
    }

    if { [ string length $fname ] > 0 } {
	
	set fileid [open $fname w]
	
	puts $fileid "#DTI Subject Description File Version 1.3"
	puts $fileid "#---------------------------------------------"
	puts $fileid "# Images"
	puts $fileid "#---------------------------------------------"

	puts $fileid "Total : [ llength $global_list(input_fname) ]"
	foreach item $global_list(input_fname) {	    
	    puts $fileid "Image : $item "
	}

	puts $fileid "#---------------------------------------------"
	puts $fileid "# Gradient directions"
	puts $fileid "#---------------------------------------------"
	puts $fileid "Filename : $global_list(gradient_fname)"
		
	puts $fileid "#---------------------------------------------"
	puts $fileid "# Parameters"
	puts $fileid "#---------------------------------------------"
	puts $fileid "Number of T2 images : $global_list(input_ndw)"
	puts $fileid "Number of DW images : $global_list(input_dw)"
	puts $fileid "Index : [ $widget_list(gradient_ndir) cget -clientdata ]"
	
	puts $fileid "#---------------------------------------------"
	puts $fileid "# Mask image"
	puts $fileid "#---------------------------------------------"
	puts $fileid "Filename : $global_list(mask_fname)"

	puts $fileid "#---------------------------------------------"
	puts $fileid "# Transformation"
	puts $fileid "#---------------------------------------------"
	puts $fileid "Swapping : $global_list(tensor_swap)"
	puts $fileid "Swap axes : $global_list(tensor_swapdir)"
	#	puts $fileid "Swap X-Y : [expr $global_list(tensor_swapdir) == 1]"
	#	puts $fileid "Swap X-Z : [expr $global_list(tensor_swapdir) == 2]"
	#	puts $fileid "Swap Y-Z : [expr $global_list(tensor_swapdir) == 3]"
	puts $fileid "Flip X : $global_list(tensor_flipx)"
	puts $fileid "Flip Y : $global_list(tensor_flipy)"
	puts $fileid "Flip Z : $global_list(tensor_flipz)"
	puts $fileid "Rotation : $global_list(tensor_rotation)"
	puts $fileid "RotX : $global_list(tensor_rotx)"
	puts $fileid "RotY : $global_list(tensor_roty)"
	puts $fileid "RotZ : $global_list(tensor_rotz)"

	close $fileid

	set setupfilename $fname

	SetTitle "(EXPERIMENTAL) Multi-B Diffusion Tensor Utility $setupfilename"

    }
}

# ---------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::GetFileElement { fileid } {

   gets $fileid line 
   set index [string first ":" $line]
   if { $index > 0 }  {
       set len   [string length $line]
       set value [string trim [string range $line [expr $index +1] [expr $len -1]]]
       return $value
   }
   return ""
}

# ---------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::LoadSetup { args } { 

    set len [ llength $args ]
    
    if { $len < 1 } {
	set fname $setupfilename 
    } else {
	set fname [ lindex $args 0 ]
    }

    if { [ string length $fname  ] == 0 } {
	set f1 $setupfilename
	set f1path [ file dirname $f1 ]
	set f1tail [ file tail    $f1 ]
	
	set fname  [tk_getOpenFile -title "Setup File" -filetypes { {"DTI-Setup File" {.dti}}} -initialfile $f1tail -initialdir $global_list(result_fdir) -parent $basewidget ]
    }

    if { [ string length $fname ] < 1 } {
	return
    }

    if { [ file readable $fname ] == 0 } {
	::pxtclutil::Warning "Cannot read setupfile file from $fname\n Bad File"
	return 0
    }

    set fileid [open $fname r]

    gets $fileid line
    if { [ string compare -length 29 $line "#DTI Subject Description File"  ] != 0 } {
	close fileid
	::pxtclutil::Warning "Cannot read setupfile file from $fname\n Bad File Type"
	return 0
    }

    set version 0
    
    ## determine version
    if { [ string length $line ] < 30 } {
	set version "1.2"
    } else {
	set version [ string range $line 38 end ]
    }

    ## read input images
    if { [ llength $global_list(input_fname) ] > 0 } {
	$this RemoveAllInputs
    }

    gets $fileid line ;     gets $fileid line  ;   gets $fileid line

    set total [ GetFileElement $fileid ]
    
    for { set i 0 } { $i < $total } { incr i } {
	set name [ GetFileElement $fileid ]
	if { [ string length $name ] > 0 } {
	    $this AddInput $name
	}
    }
    
    ## read gradients
    gets $fileid line ;     gets $fileid line  ;   gets $fileid line
    
    set name [ GetFileElement $fileid ]
    if { $name != "select" } {
	$this LoadGradient $name
    }

    gets $fileid line ;     gets $fileid line  ;   gets $fileid line
    
    ## read number of ndws, dws, index
    set number_ndw [ GetFileElement $fileid ]
    set number_dw [ GetFileElement $fileid ]
    set dir_index [ GetFileElement $fileid ]
    
    set global_list(input_ndw) $number_ndw
    set global_list(input_dw) $number_dw
    
    ## populate list of directions
    gradient_populatelist $dir_index
    
    set dirset [ lindex [ lindex $global_list(gradient_list) $dir_index ] 1 ]

    ## set default number of directions
    $widget_list(gradient_ndir) delete 0 end
    $widget_list(gradient_ndir) insert 0 $dirset
    $widget_list(gradient_ndir) configure -clientdata $dir_index
    
    ## set default number of directions
    $widget_list(gradient_ndir1) delete 0 end
    $widget_list(gradient_ndir1) insert 0 $dirset
    $widget_list(gradient_ndir1) configure -clientdata $dir_index
    
    gets $fileid line ;     gets $fileid line  ;   gets $fileid line

    ## read mask image
    set name [ GetFileElement $fileid ]
    if { $name != "select" } {
	$this LoadMask $name
    }
    
    gets $fileid line ;     gets $fileid line  ;   gets $fileid line

    ## read in transformations

    if { $version == "1.2" } {	
	set global_list(tensor_swapxz) [ GetFileElement $fileid ]
	set global_list(tensor_swapyz) [ GetFileElement $fileid ]
	if { $global_list(tensor_swapxz) == 1} {
	    set global_list(tensor_swap) 1
	    set global_list(tensor_swapdir) 2
	}
	if { $global_list(tensor_swapyz) == 1} {
	    set global_list(tensor_swap) 1
	    set global_list(tensor_swapdir) 3
	}
	set global_list(tensor_flipx) [ GetFileElement $fileid ]
	set global_list(tensor_flipy) [ GetFileElement $fileid ]
	set global_list(tensor_flipz) [ GetFileElement $fileid ]
	set global_list(tensor_rotation) 0
	set global_list(tensor_rotx) 0.0
	set global_list(tensor_roty) 0.0
	set global_list(tensor_rotz) 0.0
    } else {
	## 1.3
	set global_list(tensor_swap) [ GetFileElement $fileid ]
	set global_list(tensor_swapdir) [ GetFileElement $fileid ]
	set global_list(tensor_flipx) [ GetFileElement $fileid ]
	set global_list(tensor_flipy) [ GetFileElement $fileid ]
	set global_list(tensor_flipz) [ GetFileElement $fileid ]
	set global_list(tensor_rotation) [ GetFileElement $fileid ]
	set global_list(tensor_rotx) [ GetFileElement $fileid ]
	set global_list(tensor_roty) [ GetFileElement $fileid ]
	set global_list(tensor_rotz) [ GetFileElement $fileid ]
    }
    
    
    close $fileid
    set setupfilename $fname
    
    SetTitle "Diffusion Tensor Utility $setupfilename"
}

#-------------------------------------------------------------------------------------------
#  PRIVATE CALLBACK: Invoked upon input selection
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::input_selectioncommand_cb { } {
    
    ## get current selection 
    set sel_item [ $widget_list(input_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	## get current index
	set sel_index [ $widget_list(input_list) index [ lindex $sel_item 0 ] ] 
	set sel_input [ lindex $global_list(input_list) $sel_index ]
	
	## get number of components
	set nc [ expr [ [ $sel_input GetImage ] GetNumberOfScalarComponents ] -1 ]
	$widget_list(input_frameno) configure -range [ list 0 $nc ]
	
	## update information
	input_updateinfo $sel_index	
    }

}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Update input information on title bar
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::input_updateinfo { index } {

    set sel_input [ [ lindex $global_list(input_list) $index ] GetImage ]
    set sel_fname [ lindex $global_list(input_fname) $index ]
    
    scan [ $sel_input GetDimensions ] "%d %d %d" dx dy dz	       
    set nc [  $sel_input GetNumberOfScalarComponents ]
    set name [ file tail [ file root $sel_fname ] ]
    
    set global_list(result_fpreffix) "${name}_"

    $this SetTitle "$global_list(appname): $name (${dx}x${dy}x${dz}, $nc frames)"
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Add input
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::addinput { } {

    ## browse for filename
    set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
    set fname  [tk_getOpenFile -title "Add diffusion images" -filetypes $typelist -initialdir $global_list(result_fdir) -multiple yes -parent $basewidget ] 
    
    if { $fname != "" } {
	
	foreach item $fname {
	    
	    ## load series into memory
	    AddInput $item 
	}

	set global_list(result_fdir) [ file dirname [ lindex $fname 0 ]]
	set global_list(result_fmask) "*[ file extension [ lindex $fname 0]]"
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Remove selected input
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::removeinput { } {
    
    ## get current selection 
    set sel_item [ $widget_list(input_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	## get current index
	set sel_index [ $widget_list(input_list) index [ lindex $sel_item 0 ] ] 
	
	## remove series
	RemoveInput $sel_index
	
    }    
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Display selected input
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::displayinput { } {

    ## get current selection 
    set sel_item [ $widget_list(input_list) getcurselection ]
    
    if { $sel_item != "" } {
	
	## get current index
	set sel_index [ $widget_list(input_list) index [ lindex $sel_item 0 ] ] 
	
	## display series
	DisplayInput $sel_index $global_list(input_frameno)
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Display specific image and frame, use frame value -1 to display all frames
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::DisplayInput { index frame } {

    set input [ lindex $global_list(input_list) $index ]
    
    ## display multi-frame image
    if { $frame == -1 } {
	
	$parent SetImageFromObject [ $input GetThisPointer ] $this

    } else {
	
	set ncm [ [ $input GetImage ] GetNumberOfScalarComponents ]

	## display single-frame image
	if { [expr $frame >= 0 && $frame < $ncm] } { 
	    
	    set fex [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
	    $fex SetInput [ $input GetImage ]
	    $fex SetFrameNumber $frame
	    $fex Update
	    
	    set new_img [ pxitclimage \#auto ]
	    $new_img CopyImage [ $fex GetOutput ] 0
	    
	    $fex Delete
	    
	    $new_img CopyImageHeader [ $input GetImageHeader ]	    
	    $parent SetImageFromObject [ $new_img GetThisPointer ] $this
	}
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Add diffusion image series
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::AddInput { fname } {

    ## load series into memory
    set input [ pxitclimage \#auto ]    
    set ana [ vtkpxAnalyzeImageSource  [ pxvtable::vnewobj ]]

    $this SetFilterCallbacks $ana "Reading input..."

    WatchOn

    ## load input
    $ana Load $fname 
    $input SetFromAnalyzeSource $ana

    set nc [ [$input GetImage] GetNumberOfScalarComponents ] 
    
    if { $nc < 7 } {
	
	set ok [ tk_messageBox -type ok \
		     -parent $basewidget -title "Adding input." \
		     -message "This image has only $nc frames!" -icon info ]
	
	WatchOff
	
	return
    }
    
    if { [ llength $global_list(input_list) ] > 0 } {
	
	scan [ [ $input GetImage ] GetDimensions ] "%d %d %d" dx dy dz
	
	if { $dx != [lindex $global_list(input_dim) 0] || 
	     $dy != [lindex $global_list(input_dim) 1] ||
	     $dz != [lindex $global_list(input_dim) 2] } {
	    
	    set ok [ tk_messageBox -type ok \
			 -parent $basewidget -title "Reading input." \
			 -message "This image have different dimensions than other previously loaded images!" -icon info ]
	    
	    WatchOff
	    return
	}
    } else {
	set global_list(input_dim) [ [$input GetImage] GetDimensions ]
    }

    ## determine coordinate transform
    set or [ $ana GetOrientation ]

    if { $or == 0 } {
	#	set global_list(tensor_swapyz) 0
	#	set global_list(tensor_swapxz) 0
	set global_list(tensor_swap) 0
	set global_list(tensor_swapdir) 0
    } elseif { $or == 1 } {
	set global_list(tensor_swap) 1
	set global_list(tensor_swapdir) 3
	#	set global_list(tensor_swapyz) 1
	#	set global_list(tensor_swapxz) 0
    } elseif { $or == 2 } {
	set global_list(tensor_swap) 1
	set global_list(tensor_swapdir) 2
	#	set global_list(tensor_swapxz) 1
	#	set global_list(tensor_swapyz) 0
    }
    
    $ana Delete
    
    ## guess the number of nondiffusion components
    set global_list(input_ndw) [expr 2 - ($nc % 2) ] 
    set global_list(input_dw) [expr $nc - $global_list(input_ndw) ]

    if { $nc == 22 } {
	set global_list(input_ndw) 1
	set global_list(input_dw) 21
    }

    ## populate list of directions
    set dir_index [ gradient_searchlist $global_list(input_dw) ]
    gradient_populatelist $dir_index

    set dirset [ lindex [ lindex $global_list(gradient_list) $dir_index ] 1 ]

    ## set default number of directions
    $widget_list(gradient_ndir) delete 0 end
    $widget_list(gradient_ndir) insert 0 $dirset
    $widget_list(gradient_ndir) configure -clientdata $dir_index

    ## set default number of directions
    $widget_list(gradient_ndir1) delete 0 end
    $widget_list(gradient_ndir1) insert 0 $dirset
    $widget_list(gradient_ndir1) configure -clientdata $dir_index
        
    WatchOff
    
    ## enable interface buttons
    $widget_list(input_buttonbox) buttonconfigure remove -state normal
    $widget_list(input_buttonbox) buttonconfigure display -state normal
    $widget_list(input_frameno) configure -state normal
    
    $widget_list(menu_results) entryconfigure 0 -state normal
    $widget_list(computebtn) configure -state normal
    
    lappend global_list(input_fname) $fname
    lappend global_list(input_list) $input
    
    ## set new index
    set new_index [ expr [ llength $global_list(input_list) ] -1 ]
    
    ## update information
    input_updateinfo $new_index
    
    ## invoke selection callback
    input_selectioncommand_cb
    
    ## display input if autodisplay is on
    $this displayinput
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Remove all diffusion image series
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::RemoveAllInputs { } {
    
    set num [ llength $global_list(input_list) ]

    for { set i 0 } { $i < $num } { incr i } {

	RemoveInput 0

    }

}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Remove diffusion image series
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::RemoveInput { index } {
    
    ## remove series into memory
    [ lindex $global_list(input_list) $index ] Clear    
    itcl::delete object [ lindex $global_list(input_list) $index ]
    
    ## update image lists
    set global_list(input_list) [ lreplace $global_list(input_list) $index $index ]
    set global_list(input_fname) [ lreplace $global_list(input_fname) $index $index ]
    
    ## disable remove button if no elements in the list 
    if { [ llength $global_list(input_list) ] == 0 } {	
	
	$widget_list(input_buttonbox) buttonconfigure "remove" -state disabled	
	$widget_list(input_buttonbox) buttonconfigure "display" -state disabled
	$widget_list(input_frameno) configure -state disabled
	
	$this SetTitle $global_list(appname)
	
	set global_list(result_fpreffix) "preffix"
	
	$widget_list(computebtn) configure -state disabled
	
	## clear results
	result_clear

	[ $parent GetViewer ] UpdateDisplay
    }    
}


#-------------------------------------------------------------------------------------------
#  PUBLIC: Add a b-value
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::addbvalue { } {

    set index [ $widget_list(input_blist) curselection ]
    
    if { $index != "" } {

        if { $global_list(input_curbvalue) != ""  } {        
           
          set global_list(input_blist) \
              [ linsert $global_list(input_blist) $index $global_list(input_curbvalue) ]
        }

   } else {
  
        lappend global_list(input_blist) $global_list(input_curbvalue)

   }
     
}
 

#-------------------------------------------------------------------------------------------
#  PUBLIC: Remove a selected b-value
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::removebvalue { } {

    ## get current selection 
    set index [ $widget_list(input_blist) curselection ]
    
    if { $index != "" } {
	
            set global_list(input_blist) [ lreplace $global_list(input_blist) $index $index ]

    }
	
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Show/hide antipodal points
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::gradient_showantipodal { } {        
    $global_list(gradient_anti_actor) SetVisibility $global_list(gradient_showanti)
    $widget_list(gradient_sphere) Update
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Search gradient list
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::gradient_searchlist { dir } {

    set num [ llength $global_list(gradient_list) ]

    for { set i 0 } { $i < $num } { incr i } {
	set item [ lindex [lindex $global_list(gradient_list) $i ] 0]
	if { $item == $dir } {
	    return $i
	}
    }
    
    return 0
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Populate gradient list
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::gradient_populatelist { index } {

    set vector_list [lindex [lindex $global_list(gradient_list) $index] 2]
    set vector_length [ llength $vector_list ]

    ## clear list 
    $widget_list(gradient_list) clear
    
    set numpts [ $global_list(gradient_points) GetNumberOfPoints ]

    ## reset number of points
    if { $numpts > 0 } {
	$global_list(gradient_points) Reset	
    }
    
    $global_list(gradient_points) SetNumberOfPoints $vector_length
    
    set count 0

    ## insert gradient vectors
    foreach vector $vector_list {
	scan $vector "%f %f %f" gx gy gz
	$widget_list(gradient_list) insert end [format "% 2.5f % 2.5f % 2.5f" $gx $gy $gz]
	$global_list(gradient_points) InsertPoint $count $gx $gy $gz 

	incr count
    }

    ## update sphere
    $global_list(gradient_polydata) SetPoints $global_list(gradient_points)
    $global_list(gradient_polydata) SetNumberOfPoints $vector_length
    $global_list(gradient_polydata) Modified
    
    ## update sphere
    $widget_list(gradient_sphere) Update
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Check for colinearity
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::checkgradient { } {

    set tol $global_list(gradient_tol)
    set index [$widget_list(gradient_ndir) cget -clientdata]
    set vector_list [lindex [lindex $global_list(gradient_list) $index] 2]
    set vector_length [ llength $vector_list ]
    
    WatchOn
    
    set numcol 0
    
    ## check list
    for { set i 0 } { $i < $vector_length } { incr i } {
	set vector1 [ lindex $vector_list $i ]
	scan $vector1 "%f %f %f" gx1 gy1 gz1
	for { set j 0 } { $j < $vector_length } { incr j } {
	    if { $i != $j } {
		set vector2 [ lindex $vector_list $j ]
		scan $vector2 "%f %f %f" gx2 gy2 gz2
		if { [expr abs($gx1+$gx2)<$tol && abs($gy1+$gy2)<$tol && abs($gz1+$gz2)<$tol] } {
		    puts "Pairs ($i) \[$gx1 $gy1 $gz1\] and ($j) \[$gx2 $gy2 $gz2\] are colinear!"
		    incr numcol
		}
	    }
	}
    }

    WatchOff

    set numcol [ expr $numcol / 2 ]

    if { $numcol > 0 } {
	set ok [ tk_messageBox -type ok \
		     -parent $basewidget -title "Colinearity check" \
		     -message "This set of gradient directions\nhas $numcol colinear vectors!" \
		     -icon info ]
    } else {
	set ok [ tk_messageBox -type ok \
		     -parent $basewidget -title "Colinearity check" \
		     -message "No colinear vectors were found." \
		     -icon info ]
	
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Increment number of directions
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::gradient_increment_ndir { dir } {
    
    set max [expr [llength $global_list(gradient_list)] - 1 ]
    set index [expr [$widget_list(gradient_ndir) cget -clientdata] + $dir ]

    ## check for boundary values
    if { $index < 0 } { set index 0 }
    if { $index > $max } { set index $max }

    ## Replace number
    $widget_list(gradient_ndir1) delete 0 end
    $widget_list(gradient_ndir1) insert 0 [lindex [lindex $global_list(gradient_list) $index] 1]
    $widget_list(gradient_ndir1) configure -clientdata $index    
    
    ## Replace number - again
    $widget_list(gradient_ndir) delete 0 end
    $widget_list(gradient_ndir) insert 0 [lindex [lindex $global_list(gradient_list) $index] 1]
    $widget_list(gradient_ndir) configure -clientdata $index    
    
    ## Record number of DW images the same as number of gradient directions
    set global_list(input_dw) [lindex [lindex $global_list(gradient_list) $index] 0 ] 
    
    ## populate list of directions
    gradient_populatelist $index
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load gradient list
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::loadgradient { } {

    ## browse for filename
    set fname  [tk_getOpenFile -title "Select file" -filetypes { {"Gradient directions" {.dat}} {"Text file" {*.txt}}} -initialdir $global_list(result_fdir) -parent $basewidget] 
     
    if { $fname != "" } {
	
	set global_list(gradient_fdir) [ file dirname $fname ]

	## load gradients into memory
	set result [ LoadGradient $fname ]
	
	if { $result == 0 } { 
	    ::pxtclutil::Warning "Cannot read gradient file from $fname\n Bad File"
	}	
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Load gradient list
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::LoadGradient { fname } {

    ## check if file is ready for reading
    if { [ string length $fname ] < 1 || [ file readable $fname ] == 0 } {
	return 0
    }
    
    ## open file for reading 
    set fileid [open $fname r]
    
    ## clear current gradient directons
    set global_list(gradient_list) ""

    ## read in gradient vectors 
    while { [ eof $fileid ] == 0 } {

	## read line
	gets $fileid line
	
	## skip any comments
	while { [string index $line 0] == "\#"} {
	    gets $fileid line
	}

	## attempt to read number of directions
	set numdir $line
	
	if { [string is integer $numdir ] == 0 } break
	if { $numdir < 3 } break 

	## read in gradient directions
	for {set i 0} { $i < $numdir } {incr i} {
	    gets $fileid line
	    lappend grad$numdir $line
	} 

	set gradname grad$numdir
	eval "lappend global_list(gradient_list) \"$numdir $numdir {$$gradname} \""
    }

    ## close file
    close $fileid

    ## set default number of directions
    $widget_list(gradient_ndir) delete 0 end
    $widget_list(gradient_ndir) insert 0 [lindex [lindex $global_list(gradient_list) 0] 1]
    $widget_list(gradient_ndir) configure -clientdata 0

    ## set default number of directions
    $widget_list(gradient_ndir1) delete 0 end
    $widget_list(gradient_ndir1) insert 0 [lindex [lindex $global_list(gradient_list) 0] 1]
    $widget_list(gradient_ndir1) configure -clientdata 0

    set global_list(gradient_fname) $fname

    ## populate list of directions
    gradient_populatelist 0

    ## Record number of DW images the same as number of gradient directions
    set global_list(input_dw) [lindex [lindex $global_list(gradient_list) 0] 1 ] 

    ## Enabled filename entry
    $widget_list(gradient_fname) configure -state normal
    
    return 1
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Load mask
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::loadmask { } {

    ## browse for filename
    set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
    set fname  [tk_getOpenFile -title "Select mask" -filetypes $typelist -initialdir $global_list(result_fdir) -parent $basewidget ] 
    
    if { $fname != "" } {
	
	set global_list(result_fdir) [ file dirname $fname ]
	set global_list(result_fmask) "*[ file extension $fname ]"
	
	## load mask into memory
	LoadMask $fname
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Load in mask image 
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::LoadMask { fname } {
    
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
    
    set global_list(mask_fname) $fname

    WatchOff

    $ana Delete
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Set result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::SetResultPreffix { preffix } {
    set global_list(result_fpreffix) $preffix
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Get result filename preffix 
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::GetResultPreffix { } {
    return $global_list(result_fpreffix)
}

itcl::body mpjmultibtensorutility::SetCurrentDirectory { fname } {
    set global_list(result_fdir) $fname
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Clear all results
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::ClearAllResults { } {
    
    foreach result $global_list(result_list) {
	$result Clear
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::SaveAllResults { } {

    foreach result $global_list(result_list) title $global_list(result_title) {
	
	set fname $global_list(result_fpreffix)
	set fname "$fname[ lindex $title 1 ]"
	
	$result Save $fname
    }
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Save single result
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::SaveResult { index } {        

    set fname $global_list(result_fpreffix)
    set fname "$fname[ lindex [lindex $global_list(result_title) $index ] 1 ]"

    [lindex $global_list(result_list) $index] Save $fname
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Save all results
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::saveallresults { } {

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
itcl::body mpjmultibtensorutility::saveresult { } {
    
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
itcl::body mpjmultibtensorutility::DisplayResult { index { mode image } } {

    # ----------------------------------------
    # Xenios -- this is the image display part
    # ----------------------------------------

    set imgtodisplay [ [ lindex $global_list(result_list) $index ] GetThisPointer]
    #set rawimg  [ $imgtodisplay GetImage ] 
    #set r [ [ [ $rawimg GetPointData ] GetScalars ] GetRange ]

    #set rmin [ lindex $r 0 ]
    #set rmax [ lindex $r 1 ]


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
itcl::body mpjmultibtensorutility::displayresult { { mode image } } {

    set sel_item [ $widget_list(result_list) getcurselection ]
        
    if { $sel_item != "" } {
    set index [ $widget_list(result_list) index [ lindex $sel_item 0 ] ]
    set result [ lindex $global_list(result_list) $index ]

    #set range [ [ [ [ $result GetImage ] GetPointData ] GetScalars ] GetRange ]
    #$widget_list(view_resultluk) SetTableRange [ lindex $range 0 ] [ lindex $range 1 ]
    #   $widget_list(view_resultluk) Update

    $this DisplayResult $index $mode
    }
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Clear results
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::result_clear { } {

    for { set i 0 } { $i < [ llength $global_list(result_list) ] } { incr i } {	

	set title [ lindex [ lindex $global_list(result_title) $i ] 0 ]	    

	if { $title != "Anatomical mask" } {
	    [ lindex $global_list(result_list) $i ] Clear
      }
    }

}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create baseline image
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::result_createbaseline { }  {
    
    set BASELINE 0
    set MASK 1
    set TENSOR 2
    set MEAN 3
    set ADC 4

    ## the baseline repository
    set baseline [ lindex $global_list(result_list) $BASELINE ]  
    $baseline CopyImageHeader \
	[ [ lindex $global_list(input_list) 0 ] GetImageHeader ]
    
    ## the mean image
    set mean [ lindex $global_list(result_list) $MEAN ]  
    $mean CopyImageHeader [ $baseline GetImageHeader ]
        
    set avg_filter1 [ vtkmpjFrameAverage [ pxvtable::vnewobj ] ]    
    
    $this SetFilterCallbacks $avg_filter1 "Extracting baseline image..."
    
    ## add images into average filter
    set num [ llength $global_list(input_list) ]
    
    for { set i 0 } { $i < $num } { incr i } {
	set input [ lindex $global_list(input_list) $i ]		
	$avg_filter1 AddInput [ $input GetImage ]	
    }
    
    ## compute average
    $avg_filter1 Update

    ##
    ## Testing
    $mean ShallowCopyImage [ $avg_filter1 GetOutput ]

    ## count number of diffusion (b-value > 0)
    ## and non-diffusion components (b-value == 0)
    set input_dw 0
    set input_ndw 0
    foreach item $global_list(input_blist) {
	if { $item > 0 } {
		incr input_dw
	    } else {
		incr input_ndw
	    }
    }
    
    set global_list(input_ndw) $input_ndw
    set global_list(input_dw) $input_dw

    ##puts stderr "bvalues=0 ($input_ndw), bvalues>0 ($input_dw)"

    ## get average diffusion components
    set frm_filter [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $frm_filter "Extracting mean diffusion components..."
    $frm_filter SetInput [ $avg_filter1 GetOutput ]
    $frm_filter SetStartFrame $global_list(input_ndw)
    $frm_filter SetEndFrame [expr $global_list(input_dw) + $global_list(input_ndw) - 1 ]
    $frm_filter Update
    
    ## save average
    #$mean ShallowCopyImage [ $frm_filter GetOutput ]

    ## get rid off the filter
    $frm_filter Delete
    
    ## if more than one nondiffusion image per series, average all them out too
    if { $global_list(input_ndw) > 1 } {
	
	set avg_filter2 [ vtkmpjFrameAverage [ pxvtable::vnewobj ] ]
	
	for { set j 0 } { $j < $global_list(input_ndw) } { incr j } {
	    
	    set frm_filter [ vtkImageExtractComponents [ pxvtable::vnewobj ] ]
	    $frm_filter SetInput [ $avg_filter1 GetOutput ]
	    $frm_filter SetComponents $j
	    $frm_filter Update
	    
	    $avg_filter2 AddInput [ $frm_filter GetOutput ]
	    
	    $frm_filter Delete
	}
	
	## get average nondiffusion image
	$avg_filter2 Update
	
	$baseline ShallowCopyImage [ $avg_filter2 GetOutput ]
	
	## discard filter
	$avg_filter2 Delete
	
    } else {
	
	set frm_filter [ vtkImageExtractComponents [ pxvtable::vnewobj ] ]
	$frm_filter SetInput [ $avg_filter1 GetOutput ]
	$frm_filter SetComponents 0
	$frm_filter Update
	
	$baseline ShallowCopyImage [ $frm_filter GetOutput ]
	
	## discard filter
	$frm_filter Delete
    }

    ## get rid off mean filter
    $avg_filter1 Delete

    update idletasks

    return 1
}

#-------------------------------------------------------------------------------------------
#  PRIVATE: Create tensor image
#-------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::result_createtensor { }  {
    
    set BASELINE 0
    set MASK 1
    set TENSOR 2
    set MEAN 3
    set ADC 4
    
    set baseline [ lindex $global_list(result_list) $BASELINE ]
    set mask [ lindex $global_list(result_list) $MASK ]
    set mean [ lindex $global_list(result_list) $MEAN ]
    set tensor [ lindex $global_list(result_list) $TENSOR ]
    $tensor CopyImageHeader \
	[ [ lindex $global_list(result_list) $BASELINE ] GetImageHeader ]
    
    set index [ $widget_list(gradient_ndir) cget -clientdata ]
    set table [ lindex [ lindex $global_list(gradient_list) $index ] 2 ]

    ## populate the b list of values
    set bvalues [ vtkDoubleArray [ pxvtable::vnewobj ] ]
    
    set count 0
    for { set i 0 } { $i < [llength $global_list(input_blist)] } { incr i } {

        set btuple [ lindex $global_list(input_blist) $i ]

	    $bvalues InsertTuple1 $count $btuple
            ##puts "$btuple"
	    incr count       
    }
    
    ## populate the gradient direction array
    set gradients [ vtkDoubleArray [ pxvtable::vnewobj ] ]
    $gradients SetNumberOfComponents 3
    $gradients SetNumberOfTuples [ llength $table ]
    
    set flipx [ expr $global_list(tensor_flipx) > 0 ? -1 : 1 ]
    set flipy [ expr $global_list(tensor_flipy) > 0 ? -1 : 1 ]
    set flipz [ expr $global_list(tensor_flipz) > 0 ? -1 : 1 ]
    
    set gx 0; set gy 0; set gz 0;
    
    for {set i 0} {$i < [llength $table]} {incr i} {
	
	set tuple [ lindex $table $i ]
	
	if { $global_list(tensor_swap) == 1 } {
	    if { $global_list(tensor_swapdir) == 1 } {
		set gx [expr [lindex $tuple 1] * $flipy ]
		set gy [expr [lindex $tuple 0] * $flipx ]
		set gz [expr [lindex $tuple 2] * $flipz ]
	    }
	    if { $global_list(tensor_swapdir) == 2 } {
		set gx [expr [lindex $tuple 2] * $flipz ]
		set gy [expr [lindex $tuple 1] * $flipy ]
		set gz [expr [lindex $tuple 0] * $flipx ]
	    }
	    if { $global_list(tensor_swapdir) == 3 } {
		set gx [expr [lindex $tuple 0] * $flipx ]
		set gy [expr [lindex $tuple 2] * $flipz ]
		set gz [expr [lindex $tuple 1] * $flipy ]
	    }	   
	} else {
	    set gx [expr [lindex $tuple 0] * $flipx ]
	    set gy [expr [lindex $tuple 1] * $flipy ]
	    set gz [expr [lindex $tuple 2] * $flipz ]
	}
	
	#if { $global_list(tensor_swapyz) > 0 } {
	#	    set gx [expr [lindex $tuple 0] * $flipx ]
	#	    set gy [expr [lindex $tuple 2] * $flipz ]
	#	    set gz [expr [lindex $tuple 1] * $flipy ]
	#	} elseif { $global_list(tensor_swapxz) > 0 } {
	#	    set gx [expr [lindex $tuple 2] * $flipz ]
	#	    set gy [expr [lindex $tuple 1] * $flipy ]
	#	    set gz [expr [lindex $tuple 0] * $flipx ]
	#	} else {
	#	    set gx [expr [lindex $tuple 0] * $flipx ]
	#	    set gy [expr [lindex $tuple 1] * $flipy ]
	#	    set gz [expr [lindex $tuple 2] * $flipz ]
	#	}
		
	$gradients SetTuple3 $i $gx $gy $gz
    }
 
    set baseline [ lindex $global_list(result_list) $BASELINE ]
    set mask [ lindex $global_list(result_list) $MASK ]   

    ## compute tensor
    set tensor_filter [ vtkmpjTensorBMatrixLeastSquares [ pxvtable::vnewobj ] ]
    $this SetFilterCallbacks $tensor_filter "Computing diffusion tensor..."
    $tensor_filter SetGradients $gradients
    $tensor_filter SetNumberOfDirections [ $gradients GetNumberOfTuples ]
    $tensor_filter SetInput [ $mean GetImage ]   
    #$tensor_filter SetBaseline [ $baseline GetImage ]
    $tensor_filter SetbValues $bvalues
    $tensor_filter SetMask [ $mask GetImage ]
    #if { $global_list(input_bfactorenable) == 1 } {
	#$tensor_filter SetbValue $global_list(input_bfactor) 
    #} else {
	#$tensor_filter SetbValue 1.0
    #}
    $tensor_filter SetSymmetricTensor $global_list(input_tensorsymm)
    
    $gradients Delete
    
    ## compute tensor
    $tensor_filter Update
    

    set frm_filter [ vtkImageExtractComponents [ pxvtable::vnewobj ] ]
    $frm_filter SetInput [ $tensor_filter GetOutput ]
    $frm_filter SetComponents 6
    $frm_filter Update
    
    $baseline ShallowCopyImage [ $frm_filter GetOutput ]
    $frm_filter Delete


    if { $global_list(tensor_rotation) == 1 } {
	
    set frm_filter [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
    $frm_filter SetInput [ $tensor_filter GetOutput ]
    $frm_filter SetStartFrame 0
    $frm_filter SetEndFrame 5
    $frm_filter Update 

	## rotate tensor
	set tensor_xform [ vtkmpjImageTensorTransform [ pxvtable::vnewobj ] ]
	$tensor_xform SetInput [ $frm_filter GetOutput ]
	$tensor_xform SetRotation $global_list(tensor_rotx) $global_list(tensor_roty) $global_list(tensor_rotz) 
	$this SetFilterCallbacks $tensor_xform "Transforming tensor..."
	
	# rotate tensor
	$tensor_xform Update
	
	$tensor ShallowCopyImage [ $tensor_xform GetOutput ]
	
	$tensor_xform Delete
    $frm_filter Delete

    } else {	
	
    set frm_filter [ vtkmpjFrameExtract [ pxvtable::vnewobj ] ]
    $frm_filter SetInput [ $tensor_filter GetOutput ]
    $frm_filter SetStartFrame 0
    $frm_filter SetEndFrame 5
    $frm_filter Update 

	$tensor ShallowCopyImage [ $frm_filter GetOutput ]

    $frm_filter Delete
	
    }

    $tensor_filter Delete

    update idletasks   
}

#-------------------------------------------------------------------------------------------
#  PUBLIC: Compute results
# -----------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::ComputeResults { } {

    set BASELINE 0
    set MASK 1
    set TENSOR 2
    set MEAN 3

    WatchOn
    
    ## clear results
    result_clear
    
    ## create baseline image
    set result [ result_createbaseline ]
    
    if { $result == 0 } {
	WatchOff; return
    }
    
    ## create mask if not loaded in
    #if { $global_list(mask_auto) } {
	
	#set result [ result_createmask ]
	
	#if { $result == 0 } {
	#    WatchOff; return
	#}
    #}

    ## create adc
    #if { $global_list(adc_compute) } {	
	#set result [ result_createadc ]   
    #} 
    
    ## create tensor 
    set result [ result_createtensor ]
    
    if { $result == 0 } {
	WatchOff; return
    }
    
    set tensor_map [ lindex $global_list(result_list) $TENSOR ]
    $parent SetResultsFromObject [ $tensor_map GetThisPointer ] $this
    
    $widget_list(notebook) view "Results"; update
    
    WatchOff
}

#-------------------------------------------------------------------------------------------
#  Create diffusion input control
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::CreateInputControl { base } {      

    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Diffusion-weighted images and b-values (s/mm^2)"
    pack $base.frame0 -fill both -expand t 
    
    set frame0 [ $base.frame0 childsite ]
   
    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -fill both -expand t -side top

    set subframe0left [ frame $subframe0.subframeleft ]
    pack $subframe0left -side left -fill both -expand t -padx 0

    iwidgets::scrolledlistbox $subframe0left.inputlist \
	-hscrollmode dynamic \
	-visibleitems 30x10 \
	-listvariable [ itcl::scope global_list(input_fname) ] \
	-selectmode extended \
	-selectioncommand [ itcl::code $this input_selectioncommand_cb ]    
    pack $subframe0left.inputlist -side left -fill both -expand t -padx 2 -pady 2
    set widget_list(input_list) $subframe0left.inputlist

    set subframe0right [ frame $subframe0.subframeright ]
    pack $subframe0right -side right -fill y -expand f -padx 0

#    iwidgets::scrolledlistbox $subframe0right.bvaluelist 
    set bvaluelist [ listbox $subframe0right.bvaluelist \
                         -listvariable [ itcl::scope global_list(input_blist) ] ]
    pack $bvaluelist -side top -fill both -expand t -padx 2 -pady 2    
    set widget_list(input_blist) $subframe0right.bvaluelist

    button $subframe0right.addbvalue -text "+" -command [ itcl::code $this addbvalue ]
    pack $subframe0right.addbvalue -side left -padx 2 -pady 2


    iwidgets::entryfield $subframe0right.bvalcur \
	-width 5 \
        -labeltext "b:" \
	-validate numeric \
	-textvariable [ itcl::scope global_list(input_curbvalue) ]
    pack $subframe0right.bvalcur -side left -pady 2

    button $subframe0right.removebvalue -text "-" -command [ itcl::code $this removebvalue ] 
    pack $subframe0right.removebvalue -side left -padx 2 -pady 2

    
#    set inputbbox [ iwidgets::buttonbox $subframe0right.input -orient vertical ]
#    $inputbbox add add -text "Add..." -command [ itcl::code $this addinput ]
#    $inputbbox add remove -text "Remove" -command [ itcl::code $this removeinput ] -state disabled
#    $inputbbox add display -text "Display" -command [ itcl::code $this displayinput ] -state disabled   
#    pack $inputbbox -side top -fill x -expand f -padx 0
#    set widget_list(input_buttonbox) $inputbbox    
      
#    iwidgets::spinint $subframe0right.input_dispframe -labeltext "Frame:"  \
#	-fixed 2 -width 3 -step 1 -range {0 100} -state disabled \
#	-textvariable [ itcl::scope global_list(input_frameno) ] 
#    pack $subframe0right.input_dispframe -side top -padx 5 -pady 0
#    set widget_list(input_frameno) $subframe0right.input_dispframe     
    
#    set tensorsymm [ checkbutton $subframe0right.tensor_symm \
#			 -text "Symmetric" \
#			 -variable [ itcl::scope global_list(input_tensorsymm) ] ]
#    pack $tensorsymm -side top -padx 5 -pady 10
#    set widget_list(tensorsymm) $subframe0right.tensorsymm

#    set computeadc [ checkbutton $subframe0right.computeadc \
#			 -text "Compute ADC" \
#			 -variable [ itcl::scope global_list(adc_compute) ] ]
#    pack $computeadc -side top -padx 5 -pady 5
    

    set subframe3 [ frame $frame0.subframe3 ]
    pack $subframe3 -side top -fill x -expand f

    set inputbbox [ iwidgets::buttonbox $subframe3.inputbbox -orient horizontal ]
    $inputbbox add add -text "Add..." -command [ itcl::code $this addinput ]
    $inputbbox add remove -text "Remove" -command [ itcl::code $this removeinput ] -state disabled
    $inputbbox add display -text "Display" -command [ itcl::code $this displayinput ] -state disabled   
    pack $inputbbox -side left -fill both -expand f -padx 0
    set widget_list(input_buttonbox) $inputbbox     

    iwidgets::spinint $subframe3.input_dispframe -labeltext "Frame:"  \
	-fixed 2 -width 3 -step 1 -range {0 100} -state disabled \
	-textvariable [ itcl::scope global_list(input_frameno) ] 
    pack $subframe3.input_dispframe -side left -padx 5 -pady 2
    set widget_list(input_frameno) $subframe3.input_dispframe        
       


    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill x -expand f
     
#    iwidgets::spinint $subframe1.input_ndw -labeltext "Number of T2 images:" \
#	-fixed 2 -width 3 -step 1 -range {1 10} \
#	-textvariable [ itcl::scope global_list(input_ndw) ]
#
#    pack $subframe1.input_ndw -side left -padx 5 -pady 5  
#    set widget_list(input_ndw) $frame0.input_ndw
#    $subframe1.input_ndw up

    set gradient_ndir1 [ iwidgets::spinner $subframe1.gradient_ndir1 \
			    -labeltext "Set of gradient directions:" \
			    -width 6 \
			    -clientdata 0 \
			    -decrement [itcl::code $this gradient_increment_ndir -1 ] \
			    -increment [itcl::code $this gradient_increment_ndir +1 ] ]
    pack $gradient_ndir1 -side left -padx 5
    set widget_list(gradient_ndir1) $gradient_ndir1       
    $gradient_ndir1 clear
    $gradient_ndir1 insert 0 "ME6" 

    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -side top -fill x -expand f
    
    set widget_list(computebtn) [ button $subframe2.compute -text "Compute!" \
				      -state disabled \
				      -command [ itcl::code $this ComputeResults ] ]    
    pack $subframe2.compute -side left -fill x -padx 5 -pady 10    


#    iwidgets::entryfield $subframe2.input_bfactor \
#	-width 6 \
#	-validate real \
#	-textvariable [ itcl::scope global_list(input_bfactor) ]
#    pack $subframe2.input_bfactor -side right -padx 5 -pady 5  

#
#    checkbutton $subframe2.input_bfactorenable \
#	-text "b-value (s/mm^2):" \
#	-variable [ itcl::scope global_list(input_bfactorenable) ]
#    pack $subframe2.input_bfactorenable -side right -padx 2 -pady 5
}

#-------------------------------------------------------------------------------------------
#  Create gradient control
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::CreateGradientControl { base } {   
    
    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Diffusion gradient directions "
    pack $base.frame0 -fill both -expand t 
    
    set frame0 [ $base.frame0 childsite ]

    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -fill both -expand f

    ## create gradient file gadget
    set gradient_fname [ iwidgets::entryfield $subframe0.gradient_fname -state disabled \
			     -labeltext "Filename:"  \
			     -textvariable [ itcl::scope global_list(gradient_fname) ] ]
    pack $gradient_fname -side left -expand t -fill x -padx 5 -pady 0
    set widget_list(gradient_fname) $gradient_fname
    
    set gradient_load [ iwidgets::buttonbox $subframe0.gradient_load -orient horizontal -padx 2 -pady 2 ]
    $gradient_load add load -text "Load..." -command [ itcl::code $this loadgradient ]
    pack $gradient_load -side left -fill x -pady 0
    
    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -fill both -pady 0

    set gradient_ndir [ iwidgets::spinner $subframe1.gradient_ndir \
			    -labeltext "Set of directions:" \
			    -width 6 \
			    -clientdata 0 \
			    -decrement [itcl::code $this gradient_increment_ndir -1 ] \
			    -increment [itcl::code $this gradient_increment_ndir +1 ] ]
    pack $gradient_ndir -side left -padx 5 -pady 0
    set widget_list(gradient_ndir) $gradient_ndir

    set gradient_check [ iwidgets::buttonbox $subframe1.gradient_check -orient horizontal -padx 2 -pady 2]
    $gradient_check add load -text "Check" -command [ itcl::code $this checkgradient ]
    pack $gradient_check -side right -fill x -pady 0
 
    iwidgets::entryfield $subframe1.gradient_tol \
	-width 5 \
	-validate real \
	-labeltext "Colinearity tolerance:" \
	-textvariable [ itcl::scope global_list(gradient_tol) ]
    pack $subframe1.gradient_tol -side right -fill x -pady 5
    
    set subframe15 [ frame $frame0.subframe15 ]
    pack $subframe15 -fill both -side top -expand f
    
    set subframe15l [ frame $subframe15.subframe15l ]
    pack $subframe15l -fill both -side left -expand f

    set subframe15r [ frame $subframe15.subframe15r ]
    pack $subframe15r -fill both -side left -expand t
    
    ## initialize number of directions
    $gradient_ndir clear
    $gradient_ndir insert 0 "ME6"
    
    ## create list box 
    set gradient_list [ iwidgets::scrolledlistbox $subframe15l.gradient_list -visibleitems 42x15 -textfont {fixed 8} ]
    pack $gradient_list -side left -expand f -fill both -pady 0 -padx 2    
    set widget_list(gradient_list) $gradient_list
    
    set grad_showanti [ checkbutton $subframe15r.grad_showanti \
			    -text "Show antipodal directions" \
			    -command [itcl::code $this gradient_showantipodal ] \
			    -variable [ itcl::scope global_list(gradient_showanti) ] ]
    pack $grad_showanti -side top -anchor w -padx 5 -pady 2         
    
    ## create gradient points
    set grad_mapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
    $grad_mapper SetResolveCoincidentTopologyToPolygonOffset
    $grad_mapper SetInput [ $global_list(gradient_polydata) GetOutput ]
    $global_list(gradient_actor) SetMapper $grad_mapper
    $global_list(gradient_anti_actor) SetVisibility $global_list(gradient_showanti)
    $global_list(gradient_anti_actor) SetMapper $grad_mapper
    $global_list(gradient_anti_actor) SetScale -1 -1 -1
    $grad_mapper Delete
            
    ## create gradient sphere
    set grad_sphere [ mpjguisphere \#auto 0 ]
    $grad_sphere Resize 150 150
    $grad_sphere Initialize $subframe15r
    pack forget [ $grad_sphere GetButtonBoxWidget ]
    pack forget [ $grad_sphere GetPropertyWidget ] 
    [ [ $grad_sphere GetWireframeActor ] GetProperty ] SetColor 0.5 0.5 0.5
    [ [ $grad_sphere GetWireframeActor ] GetProperty ] SetLineWidth 1.0
    [ [ $grad_sphere GetWireframeActor ] GetProperty ] SetOpacity 0.05
    $grad_sphere SetResolution 20
    $grad_sphere AddActor $global_list(gradient_actor)
    $grad_sphere AddActor $global_list(gradient_anti_actor)
    $grad_sphere Update
    $grad_sphere Show
    
    set widget_list(gradient_sphere) $grad_sphere   

    ## populate list of directions
    gradient_populatelist 0    
    
 
    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -fill both -expand t
   
}

#-------------------------------------------------------------------------------------------
#  Create input mask control
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::CreateMaskControl { base } {        

    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Mask image (Anatomy)"
    pack $base.frame0 -fill both -expand t 
    
    set frame0 [ $base.frame0 childsite ]

    set subframe1 [ frame $frame0.subframe1 ]
    pack $subframe1 -side top -fill both -expand f -pady 0
  
    set mask_fname [ iwidgets::entryfield $subframe1.mask_fname \
			 -textvariable [ itcl::scope global_list(mask_fname) ] \
			 -labeltext "Filename:" ] 
    pack $mask_fname -side left -expand t -fill x -padx 5 -pady 2
    set widget_list(mask_fname) $mask_fname

    set mask_load [ iwidgets::buttonbox $subframe1.mask_load -orient horizontal -padx 2 ]
    $mask_load add load -text "Load..." -command [ itcl::code $this loadmask ] 
    pack $mask_load -side left -fill x -pady 2
    set widget_list(mask_load) $mask_load
}

#-------------------------------------------------------------------------------------------
#  Create transform control
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::CreateTransformControl { base } {

    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Tensor transformation"
    pack $base.frame0 -fill both -expand t 
    
    set frame0 [ $base.frame0 childsite ]

    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -fill both -expand f -padx 5 -pady 5
        
    set tensorflipx [ checkbutton $subframe2.tensorflipx \
			  -text "Flip X" \
			  -variable [ itcl::scope global_list(tensor_flipx) ] ]
    pack $tensorflipx -side left -padx 5 -pady 2
    set widget_list(tensor_flipx) $tensorflipx
    
    set tensorflipy [ checkbutton $subframe2.tensorflipy \
			  -text "Flip Y" \
			  -variable [ itcl::scope global_list(tensor_flipy) ] ]
    pack $tensorflipy -side left -padx 5 -pady 2
    set widget_list(tensor_flipy) $tensorflipy   
    
    set tensorflipz [ checkbutton $subframe2.tensorflipz \
			  -text "Flip Z" \
			  -variable [ itcl::scope global_list(tensor_flipz) ] ]
    pack $tensorflipz -side left -padx 5 -pady 2
    set widget_list(tensor_flipz) $tensorflipz


    set subframe3 [ frame $frame0.subframe3 ]
    pack $subframe3 -fill both -expand f -padx 5 -pady 5
    
    set tensor_swap [ checkbutton $subframe3.tensor_swap \
			  -text "Swap axes" \
			  -variable [ itcl::scope global_list(tensor_swap) ] ]
    pack $tensor_swap -side left -padx 5 -pady 4
    set widget_list(tensor_swap) $tensor_swap

    set tensorswapxy [ radiobutton $subframe3.tensorswapxy \
			   -text "Swap X-Y" \
			   -value 1 \
			   -variable [ itcl::scope global_list(tensor_swapdir) ] ]
    pack $tensorswapxy -side left -padx 5 -pady 2
    set widget_list(tensor_swapxy) $tensorswapxy

    set tensorswapxz [ radiobutton $subframe3.tensorswapxz \
			   -text "Swap X-Z" \
			   -value 2 \
			   -variable [ itcl::scope global_list(tensor_swapdir) ] ]
    pack $tensorswapxz -side left -padx 5 -pady 2
    set widget_list(tensor_swapxz) $tensorswapxz

    set tensorswapyz [ radiobutton $subframe3.tensorswapyz \
			   -text "Swap Y-Z" \
			   -value 3 \
			   -variable [ itcl::scope global_list(tensor_swapdir) ] ]
    pack $tensorswapyz -side left -padx 5 -pady 2
    set widget_list(tensor_swapyz) $tensorswapyz

    set subframe0 [ frame $frame0.subframe0 ]
    pack $subframe0 -fill x -expand f -padx 5 -pady 5

    set tensor_rotation [ checkbutton $subframe0.tensor_rotation \
			      -text "Rotate tensor" \
			      -variable [ itcl::scope global_list(tensor_rotation) ] ]
    pack $tensor_rotation -side left -padx 5 -pady 4
    set widget_list(tensor_rotation) $tensor_rotation

    iwidgets::entryfield $subframe0.tensor_rotx \
	-width 5 \
	-validate real \
	-labeltext "Rx:" \
	-textvariable [ itcl::scope global_list(tensor_rotx) ]
    pack $subframe0.tensor_rotx -side left -fill x -pady 5
    
    iwidgets::entryfield $subframe0.tensor_roty \
	-width 5 \
	-validate real \
	-labeltext "Ry:" \
	-textvariable [ itcl::scope global_list(tensor_roty) ]
    pack $subframe0.tensor_roty -side left -fill x -pady 5
    
    iwidgets::entryfield $subframe0.tensor_rotz \
	-width 5 \
	-validate real \
	-labeltext "Rz:" \
	-textvariable [ itcl::scope global_list(tensor_rotz) ]
    pack $subframe0.tensor_rotz -side left -fill x -pady 5

    label $subframe0.dummy -text "degrees"
    pack $subframe0.dummy -side left
}

#-------------------------------------------------------------------------------------------
#  Create results control
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::CreateResultsControl { base } {   

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
	-visibleitems 40x10 
    
    pack $subframe0.resultlist -side left -fill both -expand t -padx 2 -pady 2
    set widget_list(result_list) $subframe0.resultlist
    
    foreach item $global_list(result_title) {
	$widget_list(result_list) insert end [lindex $item 0]
    }
    
    set subframe1 [ frame $subframe0.subframe1 ]
    pack $subframe1 -side right -fill both -padx 0
    
    set resultbbox [ iwidgets::buttonbox $subframe1.input -orient vertical ]
    $resultbbox add display -text "Display" -command [ itcl::code $this displayresult image ]
    $resultbbox add save -text "Save..." -command [ itcl::code $this saveresult ]
    $resultbbox add saveall -text "Save all" -command [ itcl::code $this saveallresults ]    
    pack $resultbbox -side top -fill x -expand t -padx 0
    set widget_list(result_buttonbox) $resultbbox

    set subframe2 [ frame $frame0.subframe2 ]
    pack $subframe2 -side top -fill x -padx 2 -pady 5

    set result_fpreffix [ iwidgets::entryfield $subframe2.result_fpreffix \
			      -textvariable [ itcl::scope global_list(result_fpreffix) ] \
			      -labeltext "Preffix:" ]
    pack $result_fpreffix -side left -expand t -fill x -padx 5 -pady 2
    set widget_list(result_fpreffix) $result_fpreffix    
}

# -------------------------------------------------------------------------------------------
#  Diffusion control inititialization
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::Initialize { widget } {
    
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
    
    CreateInputControl        [ $notebook add -label "Diffusion" ]
    CreateMaskControl         [ $notebook add -label "Mask" ]    
    CreateGradientControl     [ $notebook add -label "Gradients" ]
    CreateTransformControl    [ $notebook add -label "Transform" ]
    CreateResultsControl      [ $notebook add -label "Results" ] 

    pack $notebook -side top -fill both -expand t -padx 5
    $notebook view "Diffusion"

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
itcl::body mpjmultibtensorutility::CreateMenu { mb } {    
    
    menubutton $mb.file          -text Setup       -menu $mb.file.m -underline 0
    pack $mb.file $mb.file    -side left
    menubutton $mb.results       -text Results     -menu $mb.results.m -underline 0
    pack $mb.file $mb.results    -side left
    menubutton $mb.help      -text Help         -menu $mb.help.m -underline 0 -padx 4
    pack $mb.help  -side right

    
    menu $mb.file.m -tearoff 0 
    eval "$mb.file.m add command -label \"Load Setup\" -command { $this LoadSetup \"\" } -underline 0 "
    eval "$mb.file.m add command -label \"Save Setup\" -command { $this SaveSetup \"\" } -underline 0"
    $mb.file.m add separator
    if { $parent == 0 } {
	eval "$mb.file.m add command -label Exit -command {  pxtkexit } -underline 1"
    } else {
	eval "$mb.file.m add command -label Close -command {  $this HideWindow } -underline 1"
    }

    menu $mb.results.m -tearoff 0 
    eval "$mb.results.m add command -label \"Compute All\" -command { $this ComputeResults } -underline 0 -state disabled"
    $mb.results.m add command -label "Save All Results" -command [ itcl::code $this saveallresults ] -underline 0

    menu $mb.help.m -tearoff 0

    set widget_list(menu_results) $mb.results.m    

    eval "$mb.help.m add command -label About -command { $this AboutCommand }"
}

#-------------------------------------------------------------------------------------------
#  Add controls to menu button
# ------------------------------------------------------------------------------------------
itcl::body mpjmultibtensorutility::AddToMenuButton { mb args } {
    eval "$mb add command -label \"Multi-b Tensor utility\" -command {$this ShowWindow \"Diffusion\"}"
}

#-------------------------------------------------------------------------------------------
#  Main function for stand-alone execution
# ------------------------------------------------------------------------------------------
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {

    puts "\n[ file rootname $argv0 ] is not a stand-alone program.\n"
    exit
}




