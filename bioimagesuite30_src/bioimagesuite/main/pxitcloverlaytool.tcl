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

package provide pxitcloverlaytool 1.0
# 	$Id: pxitcloverlaytool.tcl,v 1.1 2005/11/28 16:01:07 xenios Exp xenios $	




package require  labelframe 1.0
package require  pxtclutil  1.0
package require  pxvtable   1.0
package require  pxitclbaseimagecontrol 1.0
package require  pxitcltransform 1.0
package require  vtknrcontrib 1.0

itcl::class pxitcloverlaytool {

    inherit pxitclbaseimagecontrol

    protected variable tmap 0
    protected variable resliced_tmap 0
    protected variable lastjacobian 0
    protected variable vtk_viewer    0
    protected variable image_parent 0
    protected variable compact_reslice_control 1
    public    variable show_standard_images 1
    public    variable disable_colormapchoice 0
    public    variable transform_control 0

    public    variable meanimagecontrol1 0
    public    variable meanimagecontrol2 0
    public    variable sigmaimagecontrol1 0
    public    variable sigmaimagecontrol2 0
    public    variable ref_viewer 0
    public    variable trn_viewer 0
    public    variable scalarbar 

    public    variable fourdimagecontrol1 0
    public    variable fourdimagecontrol2 0


    protected common thisparam

    public    variable enable_distortioncorrection 1

    private common reg_measures [ list "Joint Entropy" "Correlation" "Gradient Correlation" "Mutual Information" "Normalized Mutual Information" "Sum of Squared Differences" "Correlation ratio C(X|Y)" "Correlation ratio C(Y|X)" "Product" "DifferenceEntropy" ]

    private common opt_methods [ list "OldStyle" "SlowClimb" "Powell" "GradientDescent" "ConjugateGradient" "Hybrid" ]
    private common nopt_methods [ list "GradientDescent" "ConjugateGradient" ]
    
    constructor { par args } {
	pxitclbaseimagecontrol::constructor $par
    } {
	set tmap                  [ [ pxitclimage \#auto ] GetThisPointer ]
	set resliced_tmap         [ [ pxitclimage \#auto ] GetThisPointer ]
	set lastjacobian          [ [ pxitclimage \#auto ] GetThisPointer ]
	AddImagesToLists          $tmap  $resliced_tmap  $lastjacobian
	InitializeOverlayTool 

	if { [ llength $args ] > 0 } {
	    set image_parent [ lindex $args 0]
	} else {
	    set image_parent 0
	}
    }

    # ------------ Methods --------------------
    
    private  method InitializeOverlayTool { } 
    public   method Initialize { inpwidg args }
    public   method DelayedInitialize { }
    public   method SetViewers { ref trn } 

    # ------------ Display --------------------
    public method AddToMenuButton { mb args}

    # -------- OOP Stuff ---------------------
    public    method SetImage { img } 
    public    method GetImage { }
    public    method SetImageFromObject { img obj } 
    protected method InitializeResults { } 
    public    method ProcessResults { filter opname { doextensions 0 } } 
    public    method ProcessResultsImage { img opname { doextensions 0 }  } 
    public    method Update { } 


    # Functional Image Related
    # =-----------------------
    public    method SetFunctionalImageFromSource { anasource } 
    public    method SetFunctionalImage { image args} 
    public    method GetFunctionalImage { }
    public    method LoadFunctionalImage { args } 
    public    method DisplayFunctionalImage { } 
    public    method ResliceFunctionalImage { force display } 
    public    method ComputeSimilarity { } 
    public    method ComputeOverlap { } 

    # Image Compare Tasks
    # -------------------
    public    method SetImageToCompareControl { ctrl image }
    public    method SetCompareMean1  { image args } { SetImageToCompareControl $meanimagecontrol1   $image } 
    public    method SetCompareMean2  { image args } { SetImageToCompareControl $meanimagecontrol2   $image } 
    public    method SetCompareSigma1 { image args } { SetImageToCompareControl $sigmaimagecontrol1  $image } 
    public    method SetCompareSigma2 { image args } { SetImageToCompareControl $sigmaimagecontrol2  $image } 
    public    method SetCompareN1     { num } { set thisparam($this,computetmapsamples1) $num} 
    public    method SetCompareN2     { num } { set thisparam($this,computetmapsamples2) $num}

    public    method ClearCompareSigma1 {  args } { catch { $sigmaimagecontrol1 ClearObject } }
    public    method ClearCompareSigma2 {  args } { catch { $sigmaimagecontrol2 ClearObject } }
    public    method ComputeCompareOperations { mode }
    public    method ComputeCompare4DOperations { mode }
    public    method ComputeIntersection { }


    # Transformation Related
    # ----------------------
    public    method SetTransformation { newtr args } 
    public    method LoadAndAddTransformation { fname } 
    public    method GetTransformation { } 
    public    method GetTransformationName { } 
    public    method SynchronizeTransformation {  } 

    # Manual Transformation Related
    public   method CreateManualTransformGUI { parent}
    public   method ClearManualTransformGUI { }
    public   method SetManualTransform { }
    public   method SetManualTransformCrossHairs { }
    public   method ShowManualTransformGUI { dummy window args }
    public   method ExtractLinearTransform { in_xform image_ref image_trn numparam }
    public   method SetManualTransformGUI { xform }

    # Minor Stuff
    # -----------
    public    method CreateResliceControl { parent } 




    # GUI Stuff
    # ---------
    public    method CreateInputControl { parent } 
    public    method CreateTransformationControl { parent } 
    public    method CreateStructuralOverlayControl { parent } 
    public    method CreateFunctionalOverlayControl { parent }
    public    method UpdateOverlayRange {  } 

    protected method ClusterFunctionalImage { funcImage fmriOverlayCreator clusterthreshold minthreshold probscale  } 
    public    method CreateOverlay { } 
    public    method ToggleScalarBar { }
    public    method CreateImageMergeControl { parent } 
    public    method CreateImageJoinControl { parent } 
    public    method CreateImageAffineControl { parent } 
    public    method CreateImageNonLinearControl { parent } 
    public    method CreateImageDistortionCorrectionControl { parent }
    public    method CreateCombineControl { parent } 
    public    method CreateImageCompareControl { parent } 
    public    method CreateImageFourDCompareControl { parent } 



    # Callbacks
    #------------
    public    method UpdateFromRegistration { mmvutil } 
    public    method UpdateFromRegistrationDone { mmvutil } 
    public    method DisableRegistrationButtons { } 
    public    method EnableRegistrationButtons { } 
    public    method SetAutoSave { mode }  


    # Registration Stuff
    # ------------------
    public    method AutoSaveTransformation { } 
    public    method AffineControlSetMode { mode } 
    public    method ComputeAffineRegistration { } 
    public    method ComputeReorientationRegistration { } 
    public    method ComputeNonLinearRegistration { } 
    public    method ComputeDistortionCorrection { } 
    public    method ComputeCombinedRegistration { mode } 

    # Various Other
    # --------------
    public    method CheckWhetherToUpdateBlend { } 
    public    method CreateCheckerBoard { } 
    public    method CreateImageBlend { } 
    public    method CreateMaskedImage { } 
    public    method ComputeROIStatistics { } 
    public    method AddImages { } 
    public    method CombineObjectMaps { } 
    public    method StripImage { } 
    public    method CreateGridVisualization { } 
    public    method JoinImages { }

    # Jacobian Stuff
    # --------------
    public    method ComputeJacobian { mode } 
    public    method DisplayJacobian { } 
    public    method DisplayJacobianOverlay { } 
    public    method UpdateJacobianOverlayRange {  } 
}



itcl::body pxitcloverlaytool::InitializeOverlayTool { } {
	
    global pxtcl_pref_array

    set thisparam($this,tmapstatus) "0 0 0"
    
    # ---------------------
    # Overlay Stuff
    # ---------------------
    set thisparam($this,overlaymode) "Both"
    set thisparam($this,overlaycmapmode) $pxtcl_pref_array(FmriMode)
    set thisparam($this,overlayanatcmapmode) $pxtcl_pref_array(NormalizeAnatomical)
    set thisparam($this,overlaymin)  2000
    set thisparam($this,overlaymax)  5000
    set thisparam($this,overlayinten) 5.0
    set thisparam($this,overlaysingleframe) 1
    set thisparam($this,overlaycurrentframe) 0

    set thisparam($this,overlay_minthresholdscale) 0;
    set thisparam($this,overlay_maxthresholdscale) 0;
    set thisparam($this,overlay_componentscale) 0;
    set thisparam($this,cluster_threshold) 0
    set thisparam($this,cluster_scale) 0

    
    # --------------------------
    # Jacobian Overlay Stuff
    # --------------------------
    set thisparam($this,jackoverlaymode) "Both"
    set thisparam($this,jackoverlaycmapmode) "0"
    if { $pxtcl_pref_array(FmriMode)!=64 } { set thisparam($this,jackoverlaycmapmode) 1   }
    set thisparam($this,jackoverlayanatcmapmode) $pxtcl_pref_array(NormalizeAnatomical)
    set thisparam($this,jackoverlaymin)   0.2
    set thisparam($this,jackoverlaymax)   2.0
    set thisparam($this,jackoverlayinten) 5.0
    set thisparam($this,jackoverlaycurrentframe) 0
    set thisparam($this,jacobian_threshold) 0.05
    set thisparam($this,jacobian_resolution) 3.0
    
    set thisparam($this,jackoverlay_minthresholdscale) 0;
    set thisparam($this,jackoverlay_maxthresholdscale) 0;
    set thisparam($this,jackoverlay_componentscale) 0;


    
    # -------------------------
    # Reslice Stuff
    # -------------------------
    set thisparam($this,resliceinterp)  $pxtcl_pref_array(Interpolation)
    set thisparam($this,reslicewrappad) 0;
    set thisparam($this,autoreslice)  1
    set thisparam($this,autoreorient) 0
    
    # ----------------------------
    # Transform Stuff
    # ----------------------------
    set thisparam($this,matrix_text) 0

    # ------------------------------
    # Image Merge Stuff
    # ------------------------------
    set thisparam($this,checkerboardsize) 8
    set thisparam($this,blendopacity) 0.5
    set thisparam($this,autoupdateblend) 0.5
    set thisparam($this,maskdilation) 1
    set thisparam($this,gridspacing1) 12
    set thisparam($this,gridspacing2) 0
    set thisparam($this,gridopacity)  0.5
    set thisparam($this,gridintensity) 0.5
    set thisparam($this,imagejoindirection) "x"
    set thisparam($this,imagejoinflip)       0
    set thisparam($this,imagejoinpad)        0
    set thisparam($this,imagejoinboundary) "-1"

    
    # ------------------------------
    # Mmvreg Registration Stuff
    # ------------------------------
    set thisparam($this,reg_usecurrent) 0
    set thisparam($this,reg_updatecount)   0
    set thisparam($this,reg_updatesample)  10
    set thisparam($this,reg_autosave) 1
    set thisparam($this,reg_autooverwrite) 0
    
    # ------------------------------
    # Affine Registration Stuff
    # ------------------------------
    set thisparam($this,reg_measure) "Normalized Mutual Information"
    set thisparam($this,dist_measure) "Normalized Mutual Information"

    set thisparam($this,areg_positive) 1
    set thisparam($this,areg_autonormalize) 1
    set thisparam($this,areg_jointlynormalize) 0
    set thisparam($this,areg_usecurrent) 0
    set thisparam($this,areg_tx) 1 ; set thisparam($this,areg_ty) 1; set thisparam($this,areg_tz) 1 ;
    set thisparam($this,areg_rx) 1 ; set thisparam($this,areg_ry) 1; set thisparam($this,areg_rz) 1 ;
    set thisparam($this,areg_sx) 0 ; set thisparam($this,areg_sy) 0; set thisparam($this,areg_sz) 0 ;
    set thisparam($this,areg_ax) 0 ; set thisparam($this,areg_ay) 0; set thisparam($this,areg_az) 0 ;
    set thisparam($this,areg_resolution)    1.5
    set thisparam($this,areg_numlevels)     3
    set thisparam($this,areg_stepsize)      2.0
    set thisparam($this,areg_autosave) 1
    set thisparam($this,areg_autooverwrite) 0
    set thisparam($this,areg_numlevels) 3
    set thisparam($this,areg_numsteps) 1
    set thisparam($this,areg_iterations) 15
    set thisparam($this,areg_optmethod) "ConjugateGradient"
    set thisparam($this,areg_logmode) 0
    set thisparam($this,areg_interpmode) 1

    if { $pxtcl_pref_array(RigidRegistration) == "Stitch" } {    
	set thisparam($this,areg_optmethod) "Hybrid"
	set thisparam($this,areg_numsteps) 4
	set thisparam($this,areg_stepsize) 1.0
	set thisparam($this,areg_numlevels) 2
	set thisparam($this,areg_usecurrent) 0
	set thisparam($this,areg_autosave) 0
    }


    # -------------------------------------------------------------
    # Non Linear Registration Stuff
    # -------------------------------------------------------------
    set thisparam($this,nreg_resolution) 1.5 ; 
    set thisparam($this,nreg_stepsize) 1.0 ; 
    set thisparam($this,nreg_autosave) 1
    set thisparam($this,nreg_autooverwrite) 0
    set thisparam($this,nreg_spacing) 20.0;
    set thisparam($this,nreg_spacingrate) 2.0;
    set thisparam($this,nreg_numlevels) 3
    set thisparam($this,nreg_numsteps) 1
    set thisparam($this,nreg_usecurrent) 1
    set thisparam($this,nreg_useprior) 0
    set thisparam($this,nreg_iterations) 15
    set thisparam($this,nreg_gscale) 1.0
    set thisparam($this,nreg_optmethod) "ConjugateGradient"
    set thisparam($this,nreg_smoothness) 0.001
    set thisparam($this,nreg_positive) 0
    set thisparam($this,nreg_autonormalize) 1
    set thisparam($this,nreg_jointlynormalize) 0

    set thisparam($this,nreg_logmode) 0
    # -------------------------------------------------------------
    # Distortion Correction Stuff
    # -------------------------------------------------------------

    set thisparam($this,dist_resolution) 2.0
    set thisparam($this,dist_stepsize) 3.0 
    set thisparam($this,dist_autosave) 1
    set thisparam($this,dist_autooverwrite) 0
    set thisparam($this,dist_spacing) 15.0;
    set thisparam($this,dist_spacingrate) 2.0;
    set thisparam($this,dist_numlevels) 3
    set thisparam($this,dist_numsteps) 1
    set thisparam($this,dist_usecurrent) 1
    set thisparam($this,dist_useprior) 0
    set thisparam($this,dist_iterations) 15
    set thisparam($this,dist_optmethod) "ConjugateGradient"
    set thisparam($this,dist_smoothness) 0.0005
    set thisparam($this,dist_warp) "Y"
    set thisparam($this,dist_logmode) 1
    set thisparam($this,dist_jackmode) "None" 

    # ------------------------------   ------------------------------
    set thisparam($this,updateoperation) "Reslice"
    set thisparam($this,updateviewer) "Reference"
    # ------------------------------   ------------------------------
}

# ---------------------------------------------------------------------------------------
# Initialize
# ---------------------------------------------------------------------------------------
itcl::body pxitcloverlaytool::SetViewers { ref trn } {
    set ref_viewer $ref
    set trn_viewer $trn
}

itcl::body pxitcloverlaytool::Initialize { inpwidg args } {

    if { $initialized == 1 } { return $basewidget }
    
    set basewidget $inpwidg
    toplevel $basewidget ; wm withdraw $basewidget
    wm geometry $basewidget 650x565

    SetTitle "Registration/Overlay Tool"
    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    
    set initialized 1
    return $basewidget
}


itcl::body pxitcloverlaytool::DelayedInitialize {  } {

    if { $fullyinitialized == 1 || $initialized == 0 } {
	return
    }
    set fullyinitialized 1


    set w  [ frame $basewidget.buttonbar  ]
    pack $w -side bottom -fill x -expand f

    if { $parent != 0 } {
	eval "button $w.l1  -text \"Copy Results To Image\" -command { $parent CopyResultsToImage }"
	eval "button $w.l2  -text \"Undo Copy Results\" -command { $parent RevertToPreviousImage }"
	pack $w.l1 $w.l2 -side left -padx 5
    }

    eval "button $w.close   -text Close -command { $this HideWindow } "
    eval "button $w.unload   -text \"Unload Results\" -command { $this UnloadImages ask }"
    pack $w.close $w.unload -side right -fill x -padx 25 -pady 10

    set notebook $basewidget.notebook
    iwidgets::tabnotebook $notebook  -tabpos w
	
    #    CreateInputControl          [ $notebook add -label  "Functional Image" ]
    CreateTransformationControl [ $notebook add -label  "Transformation" ]
    CreateStructuralOverlayControl [ $notebook add -label  "Jacobian" ]
    CreateFunctionalOverlayControl [ $notebook add -label  "Func. Overlay" ]
    CreateImageCompareControl       [ $notebook add -label  "Image Compare" ]
    CreateImageFourDCompareControl   [ $notebook add -label  "4D Image Compare" ]
    CreateImageAffineControl    [ $notebook add -label  "Linear Reg" ]
    CreateImageNonLinearControl [ $notebook add -label  "NonLinear Reg" ]
    if { $enable_distortioncorrection == 1 } {
	CreateImageDistortionCorrectionControl  [ $notebook add -label  "Distortion Cor" ]
    }

    pack $notebook -side top -fill both -expand true

    set canvas [ [ $notebook component tabset ] component canvas ];     set l [ AddLogoLabel $canvas ];     pack $l -side bottom -padx 1 
}


# -------------------------------------------------------
# SetCurrentImage
# -------------------------------------------------------

itcl::body pxitcloverlaytool::SetImageFromObject { img obj } {
    if { $image_parent == 0 } {
	return [ SetImage $img ]
    } elseif { $obj == $parent } {
	return [ SetFunctionalImage $img ]
    }
    return [ SetImage $img ]

}

itcl::body pxitcloverlaytool::GetImage { } {
    return $currentimage
}

itcl::body pxitcloverlaytool::SetImage { img } {
	
    if { [ $img GetImageSize  ] < 2 } { return 0 }
    $currentimage ShallowCopy $img
    return [ InitializeResults ]
}
# -------------------------------------------------------

itcl::body pxitcloverlaytool::InitializeResults { } {
    
    $resliced_tmap UnLoad
    $lastjacobian  UnLoad
    $resliced_tmap CopyImageHeader [ $currentimage GetImageHeader ]
    $lastjacobian  CopyImageHeader [ $currentimage GetImageHeader ]
    return 1
}

# -------------------------------------------------------
# Process Results and Send to Parent
# -------------------------------------------------------

itcl::body pxitcloverlaytool::ProcessResults { filter opname { doextensions 0 }  } {
	
    ProcessResultsImage [ $filter GetOutput ] $opname $doextensions
}

itcl::body pxitcloverlaytool::ProcessResultsImage { img opname { doextensions 0 } } {
	
    $currentresults ShallowCopyImage $img
    $currentresults CopyImageHeader [ $currentimage GetImageHeader ]
    if { $doextensions == 1 } {
	[ $currentresults GetImageHeader ] CopyExtensionsOnly  [ $tmap GetImageHeader ]
    }
    $currentresults configure -filename [ AddPrefix [ $tmap cget -filename ] $opname ]
    
    SendResultsToParent 
}


# -------------------------------------------------------------
#  Set Transformation Routine
# -------------------------------------------------------------
itcl::body pxitcloverlaytool::LoadAndAddTransformation { fname } {

    $transform_control AddInternalObject 
    set basetr [ $transform_control GetCurrentObject ]
    $basetr Load [ pxtclutil::GetAbsoluteFileName $fname ]
    $transform_control CurrentObjectModified
    update idletasks
    InitializeResults

}
    
itcl::body pxitcloverlaytool::SetTransformation { newtr args } {

    set mode "modify"

    if { [ llength $args ] > 0 } {
	set mode [ lindex $args 0 ]
    }

    $this DelayedInitialize


    if { $mode == "add" } {
	$transform_control AddInternalObject 
    }

    set basetr [ $transform_control GetCurrentObject ]
    $basetr CopyTransformation $newtr
    $transform_control CurrentObjectModified
    update idletasks
    InitializeResults
}

# -------------------------------------------------------------
#  Set Tmap Routine
# -------------------------------------------------------------

itcl::body pxitcloverlaytool::GetFunctionalImage { } {
    return $tmap
}

itcl::body pxitcloverlaytool::SetFunctionalImage { image args} {
	
    if { [  $image GetImageSize  ] < 2 } {
	return
    }

    $tmap ShallowCopy $image
    set thisparam($this,tmapstatus) [ $tmap cget -status ]
    set thisparam($this,tmapfilename) [ $tmap cget -filename ]
    InitializeResults 
    UpdateOverlayRange 
    if { $image_parent != 0 } {
	$parent SetImageFromObject $tmap $this 
    }
}

itcl::body pxitcloverlaytool::SetFunctionalImageFromSource { anasource } {

    $tmap SetFromAnalyzeSource $anasource
    set thisparam($this,tmapstatus) [ $tmap cget -status ]
    set thisparam($this,tmapfilename) [ $tmap cget -filename ]
    InitializeResults 
    UpdateOverlayRange 
    if { $image_parent != 0 } {
	$parent SetImageFromObject $tmap $this 
    }
}   

# ----------------------------------------------------------------------------
itcl::body pxitcloverlaytool::Update { } {

    switch  $thisparam($this,updateoperation) {
	"Reslice" { ResliceFunctionalImage 1 1 }
	"Overlay" { CreateOverlay  }
	"CheckerBoard" { CreateCheckerBoard }
	"Blend" { CreateImageBlend }
    }
}

# --------------------------------------------------------
#    Create Functional Image Control 
# --------------------------------------------------------

itcl::body pxitcloverlaytool::CreateInputControl { parent } {

    set base  [ LabelFrame:create $parent.a -text "Input Image"]
    set base_upd  [ LabelFrame:create $parent.c -text "Auto Update"]
    pack $parent.a $parent.c -side top -fill x -expand f

   
    frame $base.top; frame $base.bot; frame $base.mid
    pack $base.top $base.mid $base.bot -side top -pady 1 -fill x -expand f
    
    label $base.top.l1 -text "Name:"
    entry $base.top.t1 -textvariable [ itcl::scope thisparam($this,tmapfilename) ] -width 100 -relief sunken -font { fixed 10 }
    pack $base.top.l1 $base.top.t1  -side left -expand t -fill x
    
    label $base.mid.l2 -text "Dim:"
    entry $base.mid.t2 -textvariable [ itcl::scope thisparam($this,tmapstatus) ] -width 25  -relief sunken -font { fixed 10 }
    pack $base.mid.l2  $base.mid.t2  -side left -expand f -fill x
    
    if { $image_parent == 0 } {
	eval "button $base.bot.l -text \"Load\" -command { $this LoadFunctionalImage}  -width 6"
	pack $base.bot.l   -side left -padx 5 -fill x -expand f    
    }

    eval "button $base.bot.d -text \"Display\" -command { $this DisplayFunctionalImage} -width 8"
    pack  $base.bot.d  -side left -padx 5 -fill x -expand f    


    # Update Box 
    label ${base_upd}.1 -text "Update Type:"
    tk_optionMenu ${base_upd}.opt [ itcl::scope thisparam($this,updateoperation) ] "Reslice" "Overlay"  "Blend"
    pack ${base_upd}.1 ${base_upd}.opt  -side left -expand f

    
}
# --------------------------------------------------------
itcl::body pxitcloverlaytool::CreateImageCompareControl { origpar } {

    label $origpar.top -bg black -fg white -text "Statistical Comparison"
    pack $origpar.top -fill x -side top -expand f

    set statpar $origpar

    frame $statpar.bot2;     frame $statpar.bot3;
    frame $statpar.bot -bg gray; pack $statpar.bot $statpar.bot3  $statpar.bot2 -side bottom -expand f -fill x
   
    set meanimagecontrol1  [[ pxitclimageGUI \#auto  ] GetThisPointer ]
    set sigmaimagecontrol1 [[ pxitclimageGUI \#auto  ] GetThisPointer ]
    
    set meanimagecontrol2 [[ pxitclimageGUI \#auto  ] GetThisPointer ]
    set sigmaimagecontrol2 [[ pxitclimageGUI \#auto  ] GetThisPointer ]

    set nt1 [ iwidgets::tabnotebook $statpar.1 -tabpos n  -angle 45 -margin 2 -backdrop gray -raiseselect true  ]
    set nt2 [ iwidgets::tabnotebook $statpar.2 -tabpos n  -angle 45 -margin 2 -backdrop gray -raiseselect true   ]
    pack $nt1 $nt2 -side top -expand true -fill both -pady 5

   
    set clist [ list $meanimagecontrol1 $sigmaimagecontrol1 $meanimagecontrol2 $sigmaimagecontrol2 ]
    set dlist [ list "Mean 1" "Standard Deviation 1" "Mean 2" "Standard Deviation 2" ]
    set elist [ list $nt1 $nt1 $nt2 $nt2 ] 

    for { set i 0 } { $i<=3 } { incr i } {
	
	set ctrl [ lindex $clist $i ]
	set desc [ lindex $dlist $i ]
	set nt   [ lindex $elist $i ]

	set fr [ $nt add -label $desc ]

	$ctrl configure -description $desc
	$ctrl Initialize $fr.a
	$ctrl AddFunction "$this SetFunctionalImage" "Display" "$this" 
	if { $i == 1 || $i == 3 } { 
	    $fr configure -background darkgray
	}
	pack $fr.a -side top -expand f -fill x -pady 0 -padx 0
    }

    $nt1 view "Mean 1"
    $nt2 view "Mean 2"

    set thisparam($this,computetmapsamples1) "20"
    set thisparam($this,computetmapsamples2) "20"
    set thisparam($this,intersectionthreshold) "5"
    set thisparam($this,intersectionmode) "Positive"

    eval "button $statpar.bot2.a -text \"Mean1 + Mean2\" -command { $this ComputeCompareOperations add}"
    eval "button $statpar.bot2.b -text \"Mean1 - Mean2\" -command { $this ComputeCompareOperations diff}"
    pack $statpar.bot2.a $statpar.bot2.b   -side left -expand f -fill x -pady 5 -padx 2


    iwidgets::entryfield $statpar.bot3.0 -labeltext "Intersection Threshold:" -fixed 6 -width 6 -validate real -textvariable [ itcl::scope  thisparam($this,intersectionthreshold) ]
    label $statpar.bot3.1 -text "Mode:"
    tk_optionMenu $statpar.bot3.2 [ itcl::scope thisparam($this,intersectionmode) ] "Positive" "Negative" "Both"
    eval "button $statpar.bot3.3 -text \"Compute Intrsctn\" -command { $this ComputeIntersection }"
    pack $statpar.bot3.3  -side right -expand f -fill x -pady 2 -padx 2
    pack $statpar.bot3.0 $statpar.bot3.1 $statpar.bot3.2  -side left -expand true -fill x -pady 2 -padx 0

    iwidgets::entryfield $statpar.bot.c3 -labeltext "N1:" -fixed 6 -width 6 -validate integer -textvariable [ itcl::scope  thisparam($this,computetmapsamples1) ]
    iwidgets::entryfield $statpar.bot.c4 -labeltext "N2:" -fixed 6 -width 6 -validate integer -textvariable [ itcl::scope  thisparam($this,computetmapsamples2) ]

    eval "button $statpar.bot.d2 -text \"Compute Tmap \" -command { $this ComputeCompareOperations tmap }"
    pack $statpar.bot.c3 $statpar.bot.c4 -side left -expand f -fill x -pady 5 -padx 2
    pack $statpar.bot.d2   -side right -expand f -fill x -pady 5 -padx 0
    
}
# --------------------------------------------------------
itcl::body pxitcloverlaytool::CreateImageFourDCompareControl { origpar } {

    label $origpar.top -bg black -fg white -text "4D Image Statistical Comparison"
    pack $origpar.top -fill x -side top -expand f

    set statpar $origpar

    frame $statpar.bot -bg gray; pack $statpar.bot -side bottom -expand f -fill x
   
    set fourdimagecontrol1 [[ pxitclimageGUI \#auto  ] GetThisPointer ]
    set fourdimagecontrol2 [[ pxitclimageGUI \#auto  ] GetThisPointer ]

    set clist [ list $fourdimagecontrol1 $fourdimagecontrol2 ]
    set dlist [ list "4D Image 1" "4D Image 2" ]

    for { set i 0 } { $i<=1 } { incr i } {
	
	set ctrl [ lindex $clist $i ]
	set desc [ lindex $dlist $i ]

	set nm $statpar.fr$i 


	$ctrl configure -description $desc
	$ctrl Initialize $nm
	$ctrl AddFunction "$this SetFunctionalImage" "Display" "$this" 
	pack $nm -side top -expand f -fill x -pady 0 -padx 0
    }

    eval "button $statpar.bot.d1 -text \"Compute T-test \" -command { $this ComputeCompare4DOperations normal }"
    eval "button $statpar.bot.d2 -text \"Compute Paired T-test \" -command { $this ComputeCompare4DOperations paired }"

    pack $statpar.bot.d1 $statpar.bot.d2  -side right -expand f -fill x -pady 5 -padx 0
}
# --------------------------------------------------------
#  Transformation Control
# --------------------------------------------------------
itcl::body pxitcloverlaytool::CreateResliceControl { parent } {

    # Reslice Controls
    # ----------------
    set w $parent
    frame $w.m; frame $w.p ;  pack $w.m $w.p -side top -expand f -pady 1 -padx 5
    label $w.m.1 -text "Interpolation Type:"
    tk_optionMenu $w.m.opt [ itcl::scope thisparam($this,resliceinterp) ] "NearestNeighbor" "Linear" "Cubic"
    pack $w.m.1 $w.m.opt  -side left -expand f

    if { $compact_reslice_control == 1 } {
	checkbutton   $w.m.11 -variable [ itcl::scope thisparam($this,reslicewrappad) ] -text "Wrap"
	pack $w.m.11  -side left -expand f -fill x
	pack forget $w.p
    } else {
	checkbutton   $w.p.11 -variable [ itcl::scope thisparam($this,reslicewrappad) ] -text "Wrap"
	pack $w.p.11  -side top -expand f -fill x
    }

    frame $w.c; pack $w.c -side bottom -expand f -pady 10 -padx 5
    eval "button  $w.c.1  -text \"Reslice!\" -command { $this ResliceFunctionalImage 1 1 } "
    if { $compact_reslice_control == 1 } {
	eval "button  $w.c.2  -text \"Compute Similarity!\" -command { $this ComputeSimilarity } "
	eval "button  $w.c.3  -text \"Compute ROI Stats\" -command { $this ComputeROIStatistics } "
	eval "button  $w.c.4 -text \"Compute Overlap\" -command { $this ComputeOverlap } "
	pack $w.c.1 $w.c.2 $w.c.3 $w.c.4 -side left -expand f -fill x
    } else {
	pack $w.c.1 -side left -expand f -fill x
    }

}


itcl::body pxitcloverlaytool::CreateTransformationControl { parent } {

    set man_xform [ toplevel $parent.man ]
    eval "wm protocol $man_xform WM_DELETE_WINDOW { wm withdraw $man_xform }"
    wm withdraw $man_xform
    wm title $man_xform "Manual Transformation Control"
    CreateManualTransformGUI $man_xform

    # No do the Tranformation Stuff 
    


    # -- Begin Old Comment Out
    
#     set thisparam($this,matrix_text) [ text $base.top -width 36 -height 4 -borderwidth 2 -relief raised -font { fixed 10 }]
#     SynchronizeTransformation 
    
#     entry $base.mid -textvariable [ itcl::scope thisparam($this,current_transformname) ] -width 50 -justify left -relief sunken -font { fixed 10 }
    


    # -- End Old Comment Out

    set transform_control [ [ pxitclmultiTransformGUI \#auto  ] GetThisPointer ]
    $transform_control Initialize $parent.b
    $transform_control AddTransformation "Default"

    $transform_control  AddFunction "$this ShowManualTransformGUI" "Manual" "$man_xform"
    pack $parent.b -side top -expand false -fill x
    SynchronizeTransformation
    $transform_control configure -callback "$this SynchronizeTransformation"

    # Reslice Control
    # ---------------
    #set base_trn  [ LabelFrame:create $parent.b -text "Transformation"]
    set base_resl [ LabelFrame:create $parent.b1 -text "Reslice Options"]
    pack  $parent.b1  -side top -fill x -expand f



    CreateResliceControl $base_resl
    CreateImageMergeControl $parent
    CreateImageJoinControl $parent


    
    #    CreateStructuralOverlayControl $w4

    

}

# ------------------------------------------------------------
# Load / Display Tmap
# ------------------------------------------------------------
itcl::body pxitcloverlaytool::LoadFunctionalImage { args } {

    set f $thisparam($this,tmapfilename)
    set name "Tmap"
    
    if { [ llength $args ] > 0 } {
	set f [ lindex $args 0 ]
    } else {
	set f1path [ file dirname $f ]
	set f1tail [ file tail    $f ]
	set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
	set f  [tk_getOpenFile -title $name -initialfile $f1tail -initialdir $f1path -filetypes $typelist]
    }
    if { [ string length $f ] < 1 } {
	return 0
    }
	
    
    set anasource [  vtkpxAnalyzeImageSource [ pxvtable::vnewobj ]]
    SetFilterCallbacks $anasource "Reading $name Image"
    WatchOn
    $anasource Load $f

    scan [ [ $anasource GetOutput ] GetDimensions ] "%d %d %d" dx dy dz
    if { ($dx <2 || $dy < 2 || $dz < 2) } {
	::pxtclutil::Warning "Failed to Load $name Image from $f!"
	WatchOff
	return 0;
    } else {
	SetFunctionalImageFromSource $anasource
	set thisparam($this,tmapfilename) $f
    }
    catch { $anasource Delete }
    WatchOff
    return 1
}

itcl::body pxitcloverlaytool::DisplayFunctionalImage { } {

    if { $parent == 0 } { return }    
    set dx [ $tmap GetImageSize ]
    if { $dx <2 } { 
	::pxtclutil::Warning "No tmap in memory! Cannot Display!"
	return
    } 
	
    
    $parent SetResultsFromObject $tmap $this    
}

# -------------------------------------------------------------
# Transform Operations
# -------------------------------------------------------------

itcl::body pxitcloverlaytool::GetTransformation { } {

    if { $initialized == 0 } { 
	return 0
    } elseif { $fullyinitialized == 0 } {
	$this DelayedInitialize
    }
    set tr [ [ $transform_control GetCurrentObject ] GetTransformation ]
    return $tr
}

itcl::body pxitcloverlaytool::GetTransformationName { } {

    catch { return  [ [ $transform_control GetCurrentObject ] cget -filename ]  }
    return ""
}


itcl::body pxitcloverlaytool::SynchronizeTransformation {  } {

    if { $initialized == 0 } { 	return    }

    $this DelayedInitialize
    InitializeResults
    return 
}

# -----------------------------------------------------------------------
#   Manual Transform GUI
# -----------------------------------------------------------------------
itcl::body pxitcloverlaytool::ClearManualTransformGUI { } {

    for { set i 0 } { $i <=2 } { incr i } {

	if { $i == 0 } { 
	    set pname "x"
	} elseif { $i == 1 } {
	    set pname "y"
	} else {
	    set pname "z"
	}

	set thisparam($this,manxf_scale$pname) 100
	set thisparam($this,manxf_rotate$pname) 0
	set thisparam($this,manxf_shift$pname) 0
    }
}

itcl::body pxitcloverlaytool::ShowManualTransformGUI { dummy window args } {
    wm deiconify $window
}

itcl::body pxitcloverlaytool::SetManualTransform { } {
    
    for { set i 0 } { $i <=2 } { incr i } {
	
	if { $i == 0 } { 
	    set pname "x"
	} elseif { $i == 1 } {
	    set pname "y"
	} else {
	    set pname "z"
	}

	set scale($pname) $thisparam($this,manxf_scale$pname)
	if { $scale($pname) < 10 } { 
	    set scale($pname) 10 
	} elseif { $scale($pname) > 1000.0 } {
	    set scale($pname) 1000.0
	}

	set dshift($pname) $thisparam($this,manxf_shift$pname)
	if { $dshift($pname) < -2000 } { 
	    set dshift($pname) -2000
	} elseif { $dshift($pname) > 2000.0 } {
	    set dshift($pname) 2000.0
	}
				     
	set drotate($pname) $thisparam($this,manxf_rotate$pname)
	if { $drotate($pname) < -360 } { 
	    set drotate($pname) 360
	} elseif { $drotate($pname) > 360.0 } {
	    set drotate($pname) 360.0
	}

    }

    set image_trn  $tmap
    set image_ref  $currentimage

    set sp(x) [ lindex [ [  $image_ref GetImage ] GetSpacing  ] 0 ]
    set sp(y) [ lindex [ [  $image_ref GetImage ] GetSpacing  ] 1 ]
    set sp(z) [ lindex [ [  $image_ref GetImage ] GetSpacing  ] 2 ]



    set tr2 [ vtkTransform [ pxvtable::vnewobj ]]
    set xform [ vtkpxLinearTransform [ pxvtable::vnewobj ]]
    $xform ManualSetParameters [expr $dshift(x)*$sp(x) ] [ expr $dshift(y) * $sp(y) ] [ expr $dshift(z)*$sp(z) ] $drotate(x) $drotate(y) $drotate(z) $scale(x) $scale(y) $scale(z)  [ $image_ref GetImage ] [ $image_ref GetOrientation ] [  $image_trn GetImage ] [ $image_trn GetOrientation ] $tr2
    SetTransformation $tr2
    $xform Delete
    $tr2 Delete
}

itcl::body pxitcloverlaytool::SetManualTransformCrossHairs { } {

    if { $ref_viewer == 0 || $trn_viewer == 0 } {
	return 
    }
    
    set lv [ [ $ref_viewer GetViewer ] GetLastClickedPointScaled ] 
    set px(0) [ lindex $lv 0 ];   set py(0) [ lindex $lv 1 ];   set pz(0) [ lindex $lv 2 ]

    set lv [ [ $trn_viewer GetViewer ] GetLastClickedPointScaled ] 
    set px(1) [ lindex $lv 0 ];   set py(1) [ lindex $lv 1 ];   set pz(1) [ lindex $lv 2 ]

    set dx [ expr $px(1) - $px(0) ]
    set dy [ expr $py(1) - $py(0) ]
    set dz [ expr $pz(1) - $pz(0) ]

    set img [ [ $ref_viewer GetViewer ] GetImage ]
    set spa [ $img GetSpacing ]

    set thisparam($this,manxf_shiftx) [ expr $dx / [ lindex $spa 0 ]]
    set thisparam($this,manxf_shifty) [ expr $dy / [ lindex $spa 1 ]]
    set thisparam($this,manxf_shiftz) [ expr $dz / [ lindex $spa 2 ]]
}

itcl::body pxitcloverlaytool::CreateManualTransformGUI { parentw  } {

    for { set i 0 } { $i <= 2 } { incr i } {

	if { $i ==0 } { 
	    set name "x"
	} elseif { $i ==1 } {
	    set name "y"
	} else {
	    set name "z"
	}

	set f [ frame ${parentw}.$name ] ; pack $f -side top -expand t -fill x

	iwidgets::entryfield $f.1 -labeltext "${name}-Shift:" -fixed 6 -width 6 -validate real -textvariable [ itcl::scope  thisparam($this,manxf_shift${name}) ]
	iwidgets::entryfield $f.2 -labeltext "${name}-Rotate:" -fixed 6 -width 6 -validate real -textvariable [ itcl::scope  thisparam($this,manxf_rotate${name}) ]
	iwidgets::entryfield $f.3 -labeltext "${name}-Scale:" -fixed 6 -width 6 -validate real -textvariable [ itcl::scope  thisparam($this,manxf_scale${name}) ]
	pack $f.1 $f.2 $f.3 -side left -expand f -fill x
    }
    
    $this ClearManualTransformGUI
    set f [ frame $parentw.bot] 
    pack $f -side bottom -expand t -fill x
    
    eval "button $f.1 -text \"Set Xformation\" -command { $this SetManualTransform }"
    eval "button $f.2 -text \"Set & Apply\" -command { $this SetManualTransform ; $this ResliceFunctionalImage 1 1}"
    eval "button $f.3 -text \"Extract!\" -command { $this SetManualTransformGUI 0 }"
    eval "button $f.4 -text \"Clear Values\" -command { $this ClearManualTransformGUI }"
    eval "button $f.5 -text \"Close\" -command { wm withdraw $parentw }"
    pack $f.5 -side right
    pack $f.1 $f.2 $f.3 $f.4 -side left -padx 5

    if { $ref_viewer !=0 && $trn_viewer !=0 } {
	eval "button $f.45 -text \"Auto Cross Hairs\" -command { $this SetManualTransformCrossHairs }"
	pack $f.45 -side left -padx 5
    }
}

itcl::body pxitcloverlaytool::ExtractLinearTransform { in_xform image_ref image_trn numparam } {

    set image_ref  $currentimage
    set image_trn  $tmap
    
    set xform [ vtkpxLinearTransform [ pxvtable::vnewobj ]]
    $xform ExtractParameters $in_xform 9 [ $image_ref GetImage ] [ $image_ref GetOrientation ] [  $image_trn GetImage ] [ $image_trn GetOrientation ]

    for { set k $numparam } { $k < 15 } { incr k } {
	$xform Put $k 0
    }

    return $xform
}

itcl::body pxitcloverlaytool::SetManualTransformGUI { in_xform } {

    set current_transform [ $this GetTransformation ]

    if { $in_xform == 0 } {
	set in_xform $current_transform
    }

    set xform [ ExtractLinearTransform $in_xform $currentimage $tmap 9 ]
    set or [ list [ $xform  Get 3 ] [ $xform  Get 4 ] [ $xform  Get 5 ] ]
    set sc [ list [ $xform  Get 6 ] [ $xform  Get 7 ] [ $xform  Get 8 ] ]
    set tr [ list [ $xform  Get 0 ] [ $xform  Get 1 ] [ $xform  Get 2 ] ]

    set image_ref  $currentimage
    set sp(x) [ lindex [ [  $image_ref GetImage ] GetSpacing  ] 0 ]
    set sp(y) [ lindex [ [  $image_ref GetImage ] GetSpacing  ] 1 ]
    set sp(z) [ lindex [ [  $image_ref GetImage ] GetSpacing  ] 2 ]
    
    for { set i 0 } { $i <=2 } { incr i } {
	
	if { $i == 0 } { 
	    set pname "x"
	} elseif { $i == 1 } {
	    set pname "y"
	} else {
	    set pname "z"
	}
	
	set thisparam($this,manxf_scale$pname) [lindex $sc $i ]
	set thisparam($this,manxf_rotate$pname) [ lindex $or $i ]
	set thisparam($this,manxf_shift$pname) [ expr [ lindex $tr $i ] / $sp($pname) ]
    }
    $xform Delete

}


# --------------------------------------------------------
#     Reslice Functional Image
# --------------------------------------------------------

itcl::body pxitcloverlaytool::ComputeOverlap { } {


    set sigma 1.0
    set xform [ $this GetTransformation ]

    set image(1) [ $currentimage GetImage ]
    set image(2) [ $tmap GetImage ]

    scan [ $image(1) GetDimensions ] "%f %f %f" x1 y1 z1
    scan [ $image(2) GetDimensions ] "%f %f %f" x2 y2 z2

    if { $x1 < 2 || $x2  < 2 } { 
	::pxtclutil::Warning  "No  Images in Memory\n Cannot Compute Overlap!"
	return  0
    }


    set n(1) [ $currentimage cget -filename ]
    set n(2) [ $tmap cget -filename ]

    set comment "[ file tail [ file rootname  $n(1) ]]_[ file tail [ file rootname  $n(2)  ]]"

    # Step One Threshold both 
    WatchOn

    pxtkconsole



    set range [ [ [ $image(1) GetPointData ] GetScalars ] GetRange ]

    set low_thr [ expr [ lindex $range 0 ] + 1 ]
    set high_thr  [ lindex $range 1 ]

    pxtkprint "\nComputing Overlap [ GetTransformationName ] $comment range=${low_thr}:${high_thr} ( xform = [ file tail [ $this GetTransformationName ] ])\n\n"

    # Step One Threshold both 
    
    for { set thr $low_thr } { $thr <=$high_thr } { set thr [ expr $thr +1.0 ] } {
	
	for { set k 1 } { $k <= 2 } { incr k } {
	
	    set cast($k)  [  vtkImageCast [ pxvtable::vnewobj ] ]
	    $cast($k) SetInput $image($k)
	    $cast($k) SetOutputScalarTypeToFloat
	    $cast($k) Update

	    set prethreshold($k)  [  vtkImageThreshold [ pxvtable::vnewobj ] ]
	    
	    set low  [ expr $thr - 0.1 ]
	    set high [ expr $thr + 0.1 ]

	    $prethreshold($k) SetInput [$cast($k)  GetOutput ]
	    $prethreshold($k) ThresholdBetween  $low $high
	    $prethreshold($k) SetInValue    100
	    $prethreshold($k) SetOutValue   0
	    $prethreshold($k) ReplaceInOn
	    $prethreshold($k) ReplaceOutOn
	    $prethreshold($k) SetOutputScalarTypeToUnsignedChar
	    $prethreshold($k) Update

	    $cast($k) Delete

	    set spa [ $image($k)  GetSpacing ]
	    
	    for { set i 0 } { $i <= 2 } { incr i } {
		set smsigma($i) [ expr $sigma / [ lindex $spa $i ]]
	    }
	    
	    set smooth($k)  [ vtkImageGaussianSmooth [ pxvtable::vnewobj ]  ]
	    $smooth($k) SetStandardDeviations $smsigma(0) $smsigma(1) $smsigma(2)
	    $smooth($k) SetInput [ $prethreshold($k) GetOutput ] 
	    $smooth($k) SetRadiusFactors 1.5 1.5 1.5
	    $smooth($k) Update
	    $prethreshold($k) Delete
	    
	    set threshold($k)  [  vtkImageThreshold [ pxvtable::vnewobj ] ]

	    if { $k == 2 } {
		set resl [ vtkbisImageReslice [ pxvtable::vnewobj ]]
		$resl SetInput [ $smooth($k)  GetOutput ]
		$resl SetInformationInput [ $smooth(1) GetOutput ]
		$resl SetInterpolationMode 1
		$resl SetBackgroundLevel 0.0

		$resl SetResliceTransform $xform
		$resl Update
		$threshold($k) SetInput [ $resl GetOutput ]		
		$resl Delete
	    } else {
		$threshold($k) SetInput [ $smooth($k) GetOutput ]
	    }
	    

	    set r [ [ [ [ $threshold($k) GetInput ] GetPointData ] GetScalars ] GetRange ]
	    set lowr [ expr 0.5* ([lindex $r 0 ] + [ lindex $r 1 ]) ]
	    set highr [ lindex $r 1 ]
	    $threshold($k) ThresholdBetween  $lowr $highr
	    $threshold($k) SetInValue    1
	    $threshold($k) SetOutValue   0
	    $threshold($k) ReplaceInOn
	    $threshold($k) ReplaceOutOn
	    $threshold($k) SetOutputScalarTypeToUnsignedChar
	    $threshold($k) Update
	    
	    set finalimage($k) [ $threshold($k) GetOutput ]
	
	    set accum($k)  [  vtkImageAccumulate [ pxvtable::vnewobj ] ]
	    
	    $accum($k) SetInput $finalimage($k)
	    $accum($k) SetComponentExtent 0 1 0 0 0 0
	    $accum($k) SetComponentOrigin 0 0 0 
	    $accum($k) SetComponentSpacing 1.0 1.0 1.0
	    $accum($k) Update
	    
	    if { $k == 1 } {
		set ve [ [ [ [ $accum($k) GetOutput ] GetPointData ] GetScalars ] GetComponent 1 0 ]
	    } else {
		set va [ [ [ [ $accum($k) GetOutput ] GetPointData ] GetScalars ] GetComponent 1 0 ]
	    }
	}
	
	set compare1 [ vtkImageMathematics [ pxvtable::vnewobj ]]
	$compare1 SetInput1 $finalimage(1)
	$compare1 SetInput2 $finalimage(2)
	$compare1 SetOperationToMultiply
	$compare1 Update
	
	set accumcompare1  [  vtkImageAccumulate [ pxvtable::vnewobj ] ]
	$accumcompare1 SetInput [ $compare1 GetOutput ]
	$accumcompare1 SetComponentExtent 0 1 0 0 0 0
	$accumcompare1 SetComponentOrigin 0 0 0 
	$accumcompare1 SetComponentSpacing 1.0 1.0 1.0
	$accumcompare1 Update
	
	# vae == intesection 
	set vae [ [ [ [ $accumcompare1 GetOutput ] GetPointData ] GetScalars ] GetComponent 1 0 ]
	
	set overlap [ expr 100.0 * $vae / ( $va + $ve - $vae ) ]
	set overlap2 [ expr 100.0* $vae / ( 0.5 * ( $va + $ve )  ) ]

	set line1 "thr=$thr overlap=$overlap\n"
	catch { set line1 [ format "thr:%.1f\t Va:\t %d \t Ve:\t %d \t Vae:\t %d \t Overlap:\t %.2f \t Overlap2:\t %.2f \t Sigma:\t %.3f " $thr $va $ve $vae $overlap $overlap2 $sigma ] }
	
	pxtkprint  "$comment:OV\t  $line1\n"
	
	for { set k 1 } { $k <= 2 } { incr k } {
	    $threshold($k) Delete
	    $accum($k) Delete
	    $smooth($k) Delete

	}
	
	${compare1} Delete
	${accumcompare1} Delete

	    
	
    }

    WatchOff
    return 1

}


itcl::body pxitcloverlaytool::ComputeSimilarity { } {

    set reg [ vtkpxRegistration [ pxvtable::vnewobj ]]
    set current_transform [ $this GetTransformation ]
    set image_ref [ $currentimage GetImage ]
    set image_trn [ $tmap GetImage ]
    

    set arr [ vtkFloatArray [ pxvtable::vnewobj ]]
    $reg ComputeAllSimilarityMetricsBetweenImages $image_ref $image_trn $current_transform $arr

    pxtkconsole
    pxtkprint "Similarity between ref and resliced transform (using current transformation)\n"
    for { set i 0 } { $i < [ $arr GetNumberOfTuples ] } { incr i } {
	if { $i <2 || $i==4 || $i==5 || $i==9 } {
	    pxtkprint [ format "%40s : %8.5f \n" [ lindex $reg_measures $i ] [ $arr GetComponent $i 0 ] ]
	}
    }

    if { [ $current_transform IsA vtkpxComboTransform  ] ==1 } {
	set sm [ [ $current_transform GetGridTransform ] ComputeTotalBendingEnergy ]
	pxtkprint [ format "\t\tTotal Bending Energy = %.2f \n" $sm ]
    }

    pxtkprint "\n"
    $arr Delete

}


itcl::body pxitcloverlaytool::ResliceFunctionalImage { force display } {

    set current_transform [ $this GetTransformation ]

    set image_ref [ $currentimage GetImage ]
    set image_trn [ $tmap GetImage ]
    
    set orient_ref [ $currentimage GetOrientation ]
    set orient_trn [ $tmap GetOrientation ]

    set dim1 [ $image_ref GetDimensions ]
    set dim2 [ $image_trn GetDimensions ]
    if { [lindex $dim2 0 ] <2 || [ lindex $dim1 0 ] < 2 } { 
	::pxtclutil::Warning "No Image in Memory\n Cannot Reslice!"
	return  0
    }
    
    if { $force == 0 } {
	
	scan [ $image_ref GetDimensions ] "%d %d %d" dx1 dy1 dz1
	scan [ [ $resliced_tmap GetImage ] GetDimensions ] "%d %d %d" dx2 dy2 dz2
	
	if { $dx1==$dx2 && $dy1==$dy2 && $dz1 == $dz2 } {
	    return 1
	}
    }
    
	
    WatchOn 

    set reslice [  vtkbisImageReslice [ pxvtable::vnewobj ]]
    
    set intmode $thisparam($this,resliceinterp)

    if { $intmode == "Linear" } {
	set range [ [ [ $image_trn  GetPointData ] GetScalars ] GetRange ]
	if { [ lindex $range 1 ] < 20.0 } {
	    puts stderr "Switching to Cubic Interpolation to avoid 0..1 bug"
	    set intmode "Cubic"
	}
    }

    $reslice SetInterpolationModeTo$intmode
    $reslice SetWrap $thisparam($this,reslicewrappad)
    $reslice SetBackgroundLevel 0.0
    $reslice OptimizationOff
    
    $reslice SetInformationInput $image_ref


    set reorient ""
    if {$thisparam($this,autoreorient) == 0 || $orient_trn==$orient_ref  } {
	$reslice SetInput $image_trn
	$reslice SetResliceTransform $current_transform
    } else {
	set tr2 [ vtkTransform [ pxvtable::vnewobj ]]
	$tr2 Identity
	scan [ $image_trn GetSpacing    ] "%f %f %f" spx spy spz
	scan [ $image_trn GetDimensions ] "%d %d %d" dimx dimy dimz
	$tr2 Translate [ expr -0.5*$spx*$dimx ] [ expr -0.5*$spy*$dimy ] [ expr -0.5*$spz*$dimz ]
	
	set imtmp [ vtkImageData [ pxvtable::vnewobj ]]
	set ut [ vtkpxUtil [ pxvtable::vnewobj ]]
	$ut ReOrientImage $imtmp $image_trn $orient_trn $orient_ref 
	$ut Delete
	
	set tr3 [ vtkTransform [ pxvtable::vnewobj ]]
	$tr3 Identity
	scan [ $imtmp GetSpacing    ] "%f %f %f" spx spy spz
	scan [ $imtmp GetDimensions ] "%d %d %d" dimx dimy dimz
	$tr3 Translate [ expr 0.5*$spx*$dimx ] [ expr 0.5*$spy*$dimy ] [ expr 0.5*$spz*$dimz ]
	    
	set genetr [ vtkGeneralTransform [ pxvtable::vnewobj ]]
	$genetr Identity
	$genetr PostMultiply
	$genetr Concatenate $tr2
	$genetr Concatenate $current_transform
	$genetr Concatenate $tr3
	
	$reslice SetResliceTransform $genetr
	$reslice SetInput $imtmp
	set reorient "With Reorientation"
	
	catch { $tr2 Delete }
	catch { $tr3 Delete }
	catch { $genetr Delete }
	catch { $imtmp Delete }
	
    }

    SetFilterCallbacks $reslice "Reslicing Image $reorient"
    $reslice Update

    if { $display == 1 } {
	ProcessResults $reslice "resliced" 1
	set thisparam($this,updateoperation) "Reslice"
    }

    $resliced_tmap ShallowCopyImage [ $reslice GetOutput ]
    [ $resliced_tmap GetImageHeader ] CopyExtensionsOnly  [ $tmap GetImageHeader ]
    
    catch { $reslice Delete }
    WatchOff
    return 1
}


# --------------------------------------------------------
#    Create Overlay Control 
# --------------------------------------------------------
itcl::body pxitcloverlaytool::CreateFunctionalOverlayControl { parent } {


    if { $compact_reslice_control == 1 } {

	if { $trn_viewer == 0 } {
	    set w4  [ LabelFrame:create $parent.1 -text "Functional Image"]
	    CreateInputControl $w4
	    pack $parent.1 -side top -expand false -pady 10 -fill x
	}
	set w  [ LabelFrame:create $parent.2 -text "Overlay Control"]
	pack $parent.2 -side top -expand true -pady 10 -fill x
    } else {
	set w $parent
    }

    frame $w.m;  	frame $w.m2;  	frame $w.f0 ; frame $w.f1;	frame $w.f2 ;
    pack $w.m $w.m2 $w.f0 $w.f1 $w.f2 -side top -expand f
    
    label $w.m.1 -text "Overlay Type:"
    tk_optionMenu $w.m.opt [ itcl::scope thisparam($this,overlaymode) ] "Positive" "Negative" "Both" 
    pack $w.m.1 $w.m.opt -side left -expand f
    
    if { $disable_colormapchoice == 0 } {
	
	label $w.m.c1 -text "Colormap:"
	tk_optionMenu $w.m.copt [ itcl::scope thisparam($this,overlaycmapmode) ] "F1" "F2" "F4" 
	pack $w.m.c1 $w.m.copt -side left -expand f
    } else {
	set thisparam($this,overlaycmapmode) F1
    }
    
    checkbutton $w.m.chk2 -variable [ itcl::scope thisparam($this,overlayanatcmapmode) ] -text "Normalize Anatomical" 
    pack $w.m.chk2 -side left -expand f
    
    # Clustering Stuff
    # ----------------
     label $w.m2.1 -text "Clustering:"

    set thisparam($this,cluster_threshold_scale) [ scale  $w.m2.s -orient horizontal  -from 0 -to 1000 -resolution 1.0 -showvalue 0 -length 150 -variable [ itcl::scope thisparam($this,cluster_threshold) ] ]
    entry $w.m2.e -textvariable [ itcl::scope thisparam($this,cluster_threshold) ] -width 6 -relief sunken -font { fixed 10 }
    checkbutton $w.m2.a -variable [ itcl::scope thisparam($this,cluster_scale) ] -text "Orig Voxels" 
    
#    label $w.m2.l1 -text "Prob: "
#    set thisparam($this,cluster_threshold_prob) [ label $w.m2.l2 -text "1.0" -fg "blue" ]
    
    pack  $w.m2.1 $w.m2.s $w.m2.e $w.m2.a -side left -expand t -fill x
    #$w.m2.l1  $w.m2.l2 

    set thisparam($this,overlay_minthresholdscale)  [ ::pxtclutil::CreateTriple $w.f0.0  "Low Threshold"   [ itcl::scope thisparam($this,overlaymin) ]  0.0 1000.0 0.01 10 95 ]
    set thisparam($this,overlay_maxthresholdscale)  [ ::pxtclutil::CreateTriple $w.f0.1  "High Threshold"  [ itcl::scope thisparam($this,overlaymax) ]  0.0 1000.0 0.01 10 95 ]
    ::pxtclutil::CreateTriple $w.f0.2  "Inten. Thr"  [ itcl::scope thisparam($this,overlayinten) ]  -1.0 255.0 5 1 60
    
    checkbutton $w.f1.chk -variable [ itcl::scope thisparam($this,overlaysingleframe) ] -text "Single Component Only"
    pack $w.f1.chk -side left -padx 5
    set thisparam($this,overlay_componentscale)  [ ::pxtclutil::CreateTriple $w.f1.1  "Select Component:"  [ itcl::scope thisparam($this,overlaycurrentframe) ]  1 10 1 10 95 ]
    eval "bind $thisparam($this,overlay_componentscale) <ButtonRelease> \"$this UpdateOverlayRange\" "

    frame $w.c; pack $w.c -side bottom -expand f -pady 2
    eval "button  $w.c.1  -text \"Force  Reslice!\" -command { $this ResliceFunctionalImage 1 1 } "
    eval "button  $w.c.2  -text \"Create Overlay!\" -command { $this CreateOverlay   } "
    eval "button  $w.c.3  -text \"Toggle Colorbar!\" -command { $this ToggleScalarBar   } "
    pack $w.c.1 $w.c.2 $w.c.3 -side left -expand f -fill x



}

# --------------------------------------------------------
itcl::body pxitcloverlaytool::CreateStructuralOverlayControl { parent } {
    
    set w3  [ LabelFrame:create $parent.0 -text "Compute Jacobian"]
    set w4  [ LabelFrame:create $parent.1 -text "Overlay Jacobian"]
    set w5  [ LabelFrame:create $parent.grid -text "Visualize Transform"]
    pack $parent.0 $parent.1 $parent.grid -side top -expand f -pady 10 -fill x


    # Compute Buttons first and flags


    iwidgets::entryfield $w3.a -labeltext "Intensity Thresshold (fraction of max):" -fixed 6 -width 6 -validate real \
	-textvariable [ itcl::scope  thisparam($this,jacobian_threshold) ]
    iwidgets::entryfield $w3.b -labeltext "Resolution Reduction Factor (x native):" -fixed 6 -width 6 -validate real \
	-textvariable [ itcl::scope  thisparam($this,jacobian_resolution) ]
    pack $w3.a $w3.b -side top -expand f -pady 2 



    frame $w3.c;  pack $w3.c -side top -expand f -pady 5 -fill x
    eval "button $w3.c.1 -text \"Comp Jacobian\" -command { $this ComputeJacobian simple }"
    eval "button $w3.c.2 -text \"Comp Tensor\" -command { $this ComputeJacobian tensor }"
    eval "button $w3.c.3 -text \"Comp Strains\" -command { $this ComputeJacobian strains }"
    pack $w3.c.1 $w3.c.2 $w3.c.3 -side left -expand true -fill x -padx 5

      


    frame $w4.m;  	frame $w4.m2;  	frame $w4.f0 ; frame $w4.f1;	frame $w4.f2 ;
    pack $w4.m $w4.m2 $w4.f0 $w4.f1 $w4.f2 -side top -expand f
    
    label $w4.m.1 -text "Overlay Type:"
    tk_optionMenu $w4.m.opt [ itcl::scope thisparam($this,jackoverlaymode) ] "Positive" "Negative" "Both" 
    pack $w4.m.1 $w4.m.opt -side left -expand f
    
    checkbutton $w4.m2.chk -variable [ itcl::scope thisparam($this,jackoverlaycmapmode) ] -text "Use 256 Colors" 
    checkbutton $w4.m2.chk2 -variable [ itcl::scope thisparam($this,jackoverlayanatcmapmode) ] -text "Normalize Anatomical" 
    pack $w4.m2.chk $w4.m2.chk2  -side left -expand f
	       
    set thisparam($this,jackoverlay_minthresholdscale)  [ ::pxtclutil::CreateTriple $w4.f0.0  "Low Threshold"   [ itcl::scope thisparam($this,jackoverlaymin) ]  0.0 2.0 0.05 10 95 ]
    set thisparam($this,jackoverlay_maxthresholdscale)  [ ::pxtclutil::CreateTriple $w4.f0.1  "High Threshold"  [ itcl::scope thisparam($this,jackoverlaymax) ]  0.0 2.0 0.05 10 95 ]
    ::pxtclutil::CreateTriple $w4.f0.2  "Inten. Thr"  [ itcl::scope thisparam($this,jackoverlayinten) ]  -1.0 255.0 5 1 60 
    

    set thisparam($this,jackoverlay_componentscale)  [ ::pxtclutil::CreateTriple $w4.f1.1  "Select Component:"  [ itcl::scope thisparam($this,jackoverlaycurrentframe) ]  1 10 1 10 95 ]
    eval "bind $thisparam($this,jackoverlay_componentscale) <ButtonRelease> \"$this UpdateJacobianOverlayRange\" "


    frame $w4.c;  pack $w4.c -side top -expand f -pady 5 -fill x
    eval "button $w4.c.p1 -text \"Display Jacobian\" -command { $this DisplayJacobian }"
    eval "button $w4.c.p2 -text \"Overlay\" -command { $this DisplayJacobianOverlay }"
    pack $w4.c.p1 $w4.c.p2 -side left -expand true


    # Visualize Xform
    #-----------------

    label $w5.1 -text "Grid Spacing:"
    tk_optionMenu $w5.opt1 [ itcl::scope thisparam($this,gridspacing1) ]  "4" "8" "12" "16" 
    tk_optionMenu $w5.opt2 [ itcl::scope thisparam($this,gridspacing2) ] "0" "1" "2" "4" "8" "16"

    label $w5.a1 -text "Grid Intensity/Opacity:"
    tk_optionMenu $w5.opta1 [ itcl::scope thisparam($this,gridintensity) ] 0.25 0.5 0.75 1.0 1.25 1.5
    tk_optionMenu $w5.opta2 [ itcl::scope thisparam($this,gridopacity) ]  0.1 0.3 0.5 0.7 0.9

    eval "button  $w5.2  -text \"Create!\" -command { $this CreateGridVisualization  } "
    pack $w5.1 $w5.opt1 $w5.opt2 $w5.a1 $w5.opta1 $w5.opta2 $w5.2 -side left -expand f


}

itcl::body pxitcloverlaytool::UpdateOverlayRange {  } {

    if { [  $tmap GetImageSize  ] < 2 } {
	return
    }

    DelayedInitialize


    set nc [ [ $tmap GetImage ] GetNumberOfScalarComponents ]
    $thisparam($this,overlay_componentscale) configure -from 1 -to $nc 

    if { $thisparam($this,overlaycurrentframe) >= $nc } {
	set thisparam($this,overlay_coponentscale) 1
    }

    set range [ [ [ [ $tmap GetImage ] GetPointData] GetScalars] GetRange [ expr $thisparam($this,overlaycurrentframe) -1 ] ]

    set lowrange  [ expr  abs ([ lindex $range 0])]
    set highrange [ expr  abs ([ lindex $range 1])]
    
    if { $highrange<$lowrange } {
	set highrange $lowrange
    }
    
    catch { $thisparam($this,overlay_minthresholdscale) configure -from 0 -to $highrange }
    catch { $thisparam($this,overlay_maxthresholdscale) configure -from 0 -to $highrange }
    
    if { $highrange > 2000 } {
	set thisparam($this,overlaymin) 2000
	set thisparam($this,overlaymax) $highrange
    } elseif { $highrange > 1000 } {
	set thisparam($this,overlaymin) 1000
	set thisparam($this,overlaymax) $highrange
    } else {
	set thisparam($this,overlaymin) [expr $highrange / 2.0 ] 
	set thisparam($this,overlaymax) $highrange
    } 	
}

# ---------------------------------------------
# Compute Overlay
# ---------------------------------------------
itcl::body pxitcloverlaytool::ClusterFunctionalImage { funcImage fmriOverlayCreator scaledclusterthreshold minthreshold probscale } {

    set prob 1.0
    set clusterThreshold [ vtknrImageCluster [ pxvtable::vnewobj ] ]

    if { [ $funcImage GetNumberOfScalarComponents ] > 1 } {
	set extr [ vtkImageExtractComponents [ pxvtable::vnewobj ]]
	$extr SetInput $funcImage
	$extr SetComponents [ expr $thisparam($this,overlaycurrentframe)  -1 ]
	$clusterThreshold SetInput [ $extr GetOutput ]
	$extr Delete
    } else {
	$clusterThreshold SetInput $funcImage
    }
    $clusterThreshold SetIntensityThreshold $thisparam($this,overlaymin)
    $clusterThreshold Update
	
    set cluster_mask_image [ vtkImageData [ pxvtable::vnewobj ] ]
    $cluster_mask_image ShallowCopy [ $clusterThreshold GetOutput ]
    
    scan [ [ [ $cluster_mask_image GetPointData ] GetScalars ] GetRange ] "%f %f" size0 size1

    pxtkprint "Output Range $size0 $size1" 
    set max_cluster_size  [ expr int($size1 - 1) ]
    if { $max_cluster_size > 400000 } {
       set max_cluster_size 400000
     }

	
    set imageThreshold [ vtkImageThreshold [ pxvtable::vnewobj ] ]
    $imageThreshold SetInput [ $clusterThreshold GetOutput ]
    $imageThreshold ThresholdByUpper $scaledclusterthreshold
    $imageThreshold ReplaceInOn
    $imageThreshold SetInValue 255
    $imageThreshold ReplaceOutOn
    $imageThreshold SetOutValue 0
    $imageThreshold SetOutputScalarTypeToUnsignedChar
    $imageThreshold Update
    
    # Multi Frame Mask
    set imageMask [ vtkpxImageMask [ pxvtable::vnewobj ] ]
    $imageMask SetInput $funcImage
    $imageMask SetMask  [ $imageThreshold GetOutput ]
    $imageMask Update
    
    $fmriOverlayCreator SetFmriInput [ $imageMask GetOutput ] 

    $clusterThreshold Delete
    $imageThreshold Delete 
    $imageMask Delete

    return $prob
}

# ---------------------------------------------------------------------------

itcl::body pxitcloverlaytool::CreateOverlay { } {
    
    set upd [ ResliceFunctionalImage 0 0 ]
    if { $upd == 0 } {
	return 0
    }
    
    WatchOn
    set fmriOverlayCreator [ vtkpxMergeFmriConventional [ pxvtable::vnewobj ]]
    $fmriOverlayCreator SetConventional [ $currentimage GetImage ]
    
    if { $thisparam($this,cluster_threshold) > 0 } {

	set clustthr $thisparam($this,cluster_threshold)	

	if { $thisparam($this,cluster_scale)  == 1 } {

	    set sp1 [ [ $tmap GetImage ] GetSpacing ] 
	    set sp2 [ [ $resliced_tmap GetImage ] GetSpacing ]
	    
	    set vox1 [ expr [ lindex $sp1 0 ] *  [ lindex $sp1 1 ] *  [ lindex $sp1 2 ]  ]
	    set vox2 [ expr [ lindex $sp2 0 ] *  [ lindex $sp2 1 ] *  [ lindex $sp2 2 ]  ]
	    set scale [ expr $vox1 / $vox2 ]
	    set clustthr [ expr round($scale*$clustthr) ]
	} 


	set prob [ $this ClusterFunctionalImage [ $resliced_tmap GetImage ] $fmriOverlayCreator $clustthr $thisparam($this,overlaymin) $thisparam($this,cluster_threshold_scale)  ]
	set thisparam($this,overlaysingleframe) 1    
    } else {
	$fmriOverlayCreator SetFmriInput    [ $resliced_tmap GetImage ]
	set prob 1.0
    }
    #    catch { $thisparam($this,cluster_threshold_prob) configure -text $prob }


    $fmriOverlayCreator SetSingleFrameMode $thisparam($this,overlaysingleframe)
    $fmriOverlayCreator SetCurrentFrame [ expr $thisparam($this,overlaycurrentframe)  -1 ]
    $fmriOverlayCreator SetIntensityThreshold [expr round($thisparam($this,overlayinten))]
    $fmriOverlayCreator SetMinimumThreshold  $thisparam($this,overlaymin)
    $fmriOverlayCreator SetMaximumThreshold  $thisparam($this,overlaymax)
    switch -exact $thisparam($this,overlaycmapmode) {
	"F1"  {  $fmriOverlayCreator SetOutputMode 0 }
	"F2"  {  $fmriOverlayCreator SetOutputMode 1 }
	"F4"  {  $fmriOverlayCreator SetOutputMode 2 }
    }
	    
    $fmriOverlayCreator SetNormalizeAnatomical $thisparam($this,overlayanatcmapmode)
    
    switch -exact -- $thisparam($this,overlaymode) {
	"Positive" { $fmriOverlayCreator SetThresholdMode 1}
	"Both"     { $fmriOverlayCreator SetThresholdMode 3}
	"Negative" { $fmriOverlayCreator SetThresholdMode 2}
    }
    SetFilterCallbacks $fmriOverlayCreator "Creating Overlays"
    $fmriOverlayCreator Update

    ProcessResultsImage  [ $fmriOverlayCreator GetOutput ] "Overlay" 1
    $this SetValueImage  [ $fmriOverlayCreator GetFunctionalInput ]

    WatchOff 
    if { $parent !=0 } {
	set md 0;
	switch -exact $thisparam($this,overlaycmapmode) {
	    "F1"  {  $parent SetLookupTableMode 1 ; set md 0} 
	    "F2"  {  $parent SetLookupTableMode 2 ; set md 1} 
	    "F4"  {  $parent SetLookupTableMode 6 ; set md 5} 
	}

	set viewer 0
	catch { set viewer [ $parent GetViewer ] }
	if { $viewer !=0 } {
	    set scalarbar [ $viewer GetScalarBar ]
	    set positive 1
	    if { $thisparam($this,overlaymode) == "Negative"  } { set positive 0 }
	    set pcmap [ vtkLookupTable [ pxvtable::vnewobj ]] 
	    set cutil [ vtkpxColorMapUtil [ pxvtable::vnewobj ]] 
	    $cutil SetScalarBarColormap $pcmap $md $thisparam($this,overlaymin) $thisparam($this,overlaymax) $positive
	    
	    $scalarbar SetTitle ""
	    $scalarbar SetLookupTable $pcmap
	    $pcmap Delete ; $cutil Delete
	    [$scalarbar GetPositionCoordinate] SetCoordinateSystemToNormalizedViewport
	    [$scalarbar GetPositionCoordinate] SetValue 0.1 0.1
	    $scalarbar SetOrientationToHorizontal
	    $scalarbar SetWidth 0.8
	    $scalarbar SetHeight 0.9
	    $scalarbar VisibilityOn
	}
    }

    set thisparam($this,updateoperation) "Overlay"
    $fmriOverlayCreator Delete
    
    return 1
}

itcl::body pxitcloverlaytool::ToggleScalarBar { } { 

    if { $parent != 0 } {
	[ $parent GetViewer ] ToggleKeyRenderer
    }
}


# --------------------------------------------------------
#    Create Image Merge Control 
# --------------------------------------------------------

itcl::body pxitcloverlaytool::CreateImageMergeControl { parent } {
    
    set par [ LabelFrame:create $parent.addp -text "Additional Procedures"]
    set w [ frame $par.b  ]
    set w2 [ frame $par.c  ]
    set w3 [ frame $par.d ]
    pack $parent.addp -side bottom -fill x -expand f -pady 5

    pack $w $w2  $w3 -side top -expand f -fill x  -pady 2
    
    
    # Image Checker Board
    # -------------------
    frame $w.m; pack $w.m  -side top -expand f
    label $w.m.1 -text "Checker Board Spacing:"
    tk_optionMenu $w.m.opt [ itcl::scope thisparam($this,checkerboardsize) ] "2" "5" "8" "11" "16" "23" "32"
    checkbutton $w.m.2 -variable [ itcl::scope thisparam($this,overlayanatcmapmode) ] -text "Normalize Images" 
    pack $w.m.1 $w.m.opt $w.m.2 -side left -expand f
    eval "button  $w.m.b1  -text \"Create Checker Board!\" -command { $this CreateCheckerBoard  } "
    pack $w.m.b1  -side right -expand f -fill x
    
    # Image Blend
    # -------------------
    frame $w2.top ; pack $w2.top -side top -padx 20 -pady 2
    
    label  $w2.top.l -text "Opacity:"
    scale  $w2.top.s -orient horizontal  -from 0 -to 1 -resolution 0.025 -showvalue 1 -length 150 -variable [ itcl::scope thisparam($this,blendopacity) ]
    checkbutton $w2.top.c -variable [ itcl::scope thisparam($this,autoupdateblend) ] -text "Auto Update"
    pack $w2.top.l $w2.top.s $w2.top.c -side left
    eval "bind $w2.top.s <ButtonRelease> { $this CheckWhetherToUpdateBlend }"
    eval "button  $w2.top.b1  -text \"Create Blended Images!\" -command { $this CreateImageBlend  } "
    pack $w2.top.b1  -side left -expand f -fill x
    
    
    # Mask Dilation
    # -------------------
    label $w3.1 -text "Dilation of Mask:"
    tk_optionMenu $w3.opt [ itcl::scope thisparam($this,maskdilation) ]  "1" "2" "3"  "5" "7" "9" "11" "15" "20"
    eval "button  $w3.2  -text \"Mask Image!\" -command { $this CreateMaskedImage } "
    pack $w3.1 $w3.opt $w3.2 -side left -expand t -padx 5
    
}


itcl::body pxitcloverlaytool::CheckWhetherToUpdateBlend { } {
    if { $thisparam($this,autoupdateblend) == 1 } {
	$this CreateImageBlend
    }
}

itcl::body pxitcloverlaytool::CreateCheckerBoard { } {
    
    set upd [ ResliceFunctionalImage  0 0 ]
    if { $upd == 0 } {
	return 0
    }
    
    
    WatchOn
	set checkerBoardCreator [ vtkpxCreateCheckerBoardImage [ pxvtable::vnewobj ]]
    $checkerBoardCreator SetInput   [ $currentimage GetImage ]
    $checkerBoardCreator SetInput2  [ $resliced_tmap GetImage ]
    $checkerBoardCreator SetSquareSize [expr round($thisparam($this,checkerboardsize))]
    $checkerBoardCreator SetNormalizeImages  $thisparam($this,overlayanatcmapmode)
    
    SetFilterCallbacks $checkerBoardCreator "Creating Checker Board"
    $checkerBoardCreator Update
    
    ProcessResults  $checkerBoardCreator "CheckerBoard" 
    $checkerBoardCreator Delete
    WatchOff 

    return 1
}

itcl::body pxitcloverlaytool::CreateImageBlend { } {
    
    set upd [ ResliceFunctionalImage 0 0 ]
    if { $upd == 0 } {
	return 0
    }
    
    WatchOn
    
    set imcast2 [ vtkImageCast [ pxvtable::vnewobj ]]
    $imcast2 SetInput [ $resliced_tmap GetImage ]
    $imcast2 SetOutputScalarType [ [ $currentimage GetImage ] GetScalarType ]
    $imcast2 Update

    set blendCreator [ vtkImageBlend [ pxvtable::vnewobj ]]
    $blendCreator AddInput [ $currentimage GetImage ]
    $blendCreator AddInput [ $imcast2 GetOutput ]
    $imcast2 Delete 
    
    $blendCreator SetOpacity 1 $thisparam($this,blendopacity)
    SetFilterCallbacks $blendCreator "Blending Images"
    $blendCreator Update
    
    ProcessResults  $blendCreator "Blend" 
    $blendCreator Delete
    WatchOff 
    set thisparam($this,updateoperation) "Blend"
    return 1
}

itcl::body pxitcloverlaytool::CreateMaskedImage { } {
	
    set upd [ ResliceFunctionalImage 0 0 ]
    if { $upd == 0 } {
	return 0
    }
    
    WatchOn
    
    set img [ vtkImageData [ pxvtable::vnewobj ]]
    set thr [ vtkImageThreshold [ pxvtable::vnewobj ]]
    $thr ThresholdByUpper 1.0
    $thr SetInput [ $resliced_tmap GetImage ]
    $thr SetInValue 1.0
    $thr SetOutValue 0.0
    $thr ReplaceInOn
    $thr ReplaceOutOn
    
    if { $thisparam($this,maskdilation) > 0 } {
	set dilate  [  vtkImageContinuousDilate3D [ pxvtable::vnewobj ] ]
	$dilate SetInput [ $thr GetOutput ] 
	$dilate SetKernelSize $thisparam($this,maskdilation) $thisparam($this,maskdilation) $thisparam($this,maskdilation)
	SetFilterCallbacks $dilate "Dilating Mask"
	$dilate Update
	$img ShallowCopy [ $dilate GetOutput ]
	$dilate Delete
    } else {
	$thr Update
	$img ShallowCopy [ $thr GetOutput ]
    }
    
    
    set mask  [  vtkpxImageMask [ pxvtable::vnewobj ] ]
    $mask SetInput [ $currentimage GetImage ]
    $mask SetMask  $img
    SetFilterCallbacks $mask  "Mask x Image"
    $mask Update
    
    ProcessResults  $mask "mask" 
    $mask Delete
    $thr Delete
    
    WatchOff 
    
    return 1
}
# -------------------------------------------------------------------------------
#   Image Join
# -------------------------------------------------------------------------------
itcl::body pxitcloverlaytool::CreateImageJoinControl { base }  {

    set basea [ frame $base.a ]
    pack $basea -side bottom -expand true -fill both

    label $basea.1 -text "Stitching Axis:"
    radiobutton $basea.2a -variable [itcl::scope thisparam($this,imagejoindirection)] -value "x" -text "X"
    radiobutton $basea.2b -variable [itcl::scope thisparam($this,imagejoindirection)] -value "y" -text "Y"
    radiobutton $basea.2c -variable [itcl::scope thisparam($this,imagejoindirection)] -value "z" -text "Z" 
    iwidgets::entryfield $basea.2d -labeltext "Seam" -fixed 4 -width 4 -validate integer -textvariable [ itcl::scope  thisparam($this,imagejoinboundary) ]
    checkbutton   $basea.2e  -text "Flip"  -variable [ itcl::scope thisparam($this,imagejoinflip) ]
    iwidgets::entryfield $basea.2f -labeltext "Pad" -fixed 4 -width 4 -validate integer -textvariable [ itcl::scope  thisparam($this,imagejoinpad) ]
    eval "button $basea.but  -text \"Stitch Images\" -command { $this JoinImages }"
    pack $basea.1 $basea.2a $basea.2b $basea.2c $basea.2d $basea.2e $basea.2f $basea.but -side left -expand f -fill x
}

itcl::body pxitcloverlaytool::JoinImages { } {

    set upd [ ResliceFunctionalImage 0 0 ]
    if { $upd == 0 } {
	return 0
    }
    
    set image(1) [ $currentimage GetImage ]
    set image(2) [ $resliced_tmap GetImage ]

    WatchOn
    set axis 0
    switch  $thisparam($this,imagejoindirection) {
	"y" { set axis 1 } 
	"z" { set axis 2 } 
    }


    if { $thisparam($this,imagejoinpad) == 0 } {
	set stitch [ vtkpxImageStitch [ pxvtable::vnewobj ]]
	$stitch SetAxis $axis
	$stitch SetInput  $image(1)
	$stitch SetInput2 $image(2)
	$stitch SetSeamCoordinate $thisparam($this,imagejoinboundary)
	$stitch SetFlipOrientation $thisparam($this,imagejoinflip)
	SetFilterCallbacks $stitch "Stitching Images"
	$stitch Update
    
	ProcessResults  $stitch "Stitched" 
	$stitch Delete
    } else {
	set resl [ vtkbisImageReslice [ pxvtable::vnewobj ]]
	$resl OptimizationOff
	set trans  [  vtkIdentityTransform [ pxvtable::vnewobj ] ]	

	set spa [ [ $currentimage GetImage ] GetSpacing ]
	set ori [ [ $currentimage GetImage ] GetOrigin ]
	set dim [ [ $currentimage GetImage ] GetDimensions ]
	set voi0 [ list   0 0 0 ]
	set voi1 [ list   [ expr [ lindex $dim 0 ] -1 ]  [ expr [ lindex $dim 1 ] -1 ]  [ expr [ lindex $dim 2 ] -1 ]  ]

	set seampoint $thisparam($this,imagejoinboundary)
	
	for { set i 0 } { $i <=2 } { incr i } {
	    set x0 [ expr round([ lindex $voi0 $i ]) ]
	    set x1 [ expr round([ lindex $voi1 $i ]) ]

	    if { $axis == $i } {
		set dpad [ expr abs($thisparam($this,imagejoinpad)) ]
		if {  $thisparam($this,imagejoinflip) == 0 } {
		    set x1 [ expr $x1 + $dpad ]
		} else {
		    set x0 [ expr $x0 - $dpad ]
		    if { $seampoint > 0 } {
			set seampoint [ expr $seampoint + $dpad ]
		    }
		}
	    }

	    if { $x1 < $x0 } { set x1 [ expr $x0 ] }
	    
	    set origin($i)  [ expr [lindex $ori $i ] + $x0 * [ lindex $spa $i ]]
	    set spacing($i) [lindex $spa $i ]
	    set ext($i)     [ expr ($x1-$x0) ]
	}

	#puts stdout "origin = $origin(0) $origin(1) $origin(2) vs  $ori "
	#puts stdout "spacing = $spacing(0) $spacing(1) $spacing(2) vs  $spa "
	#puts stdout "ext = $ext(0) $ext(1) $ext(2) vs  $dim "
    
	$resl SetInput [ $currentimage GetImage ]
	$resl SetResliceTransform $trans
	$resl SetOutputSpacing $spacing(0) $spacing(1) $spacing(2)
	$resl SetOutputOrigin  $origin(0)  $origin(1)  $origin(2)
	$resl SetOutputExtent  0 $ext(0) 0 $ext(1) 0 $ext(2)
	$resl SetInterpolationMode 0 
	SetFilterCallbacks $resl "Padding Reference Image"
	$resl Update

	set resl2 [ vtkbisImageReslice [ pxvtable::vnewobj ]]
	$resl2 OptimizationOff
	set current_transform [ $this GetTransformation ]
	set image_trn [ $tmap GetImage ]
	
	$resl2 SetInformationInput [ $resl GetOutput ]
	$resl2 SetInput $image_trn
	$resl2 SetInterpolationModeTo$thisparam($this,resliceinterp)
	$resl2 SetBackgroundLevel 0.0
	$resl2 SetResliceTransform $current_transform
	SetFilterCallbacks $resl "Reslicing Target Image"
	$resl2 Update

	set stitch [ vtkpxImageStitch [ pxvtable::vnewobj ]]
	$stitch SetAxis            $axis
	$stitch SetInput           [ $resl GetOutput ] 
	$stitch SetInput2          [ $resl2 GetOutput ]
	$stitch SetSeamCoordinate  $seampoint 
	$stitch SetFlipOrientation $thisparam($this,imagejoinflip)
	SetFilterCallbacks $stitch "Stitching Images"
	$stitch Update
    
	ProcessResults  $stitch "Stitched" 
	$stitch Delete

	$resl Delete
	$resl2 Delete
	$trans Delete
    }
	
    WatchOff 
    return 1
}
# -------------------------------------------------------------------------------

itcl::body pxitcloverlaytool::ComputeROIStatistics { } {
	

    set current_transform [ $this GetTransformation ]
    set image_ref [ $currentimage GetImage ]
    set image_trn [ $tmap GetImage ]
    set dim1 [ $image_ref GetDimensions ]
    set dim2 [ $image_trn GetDimensions ]
    if { [lindex $dim2 0 ] <2 || [ lindex $dim1 0 ] < 2 } { 
	::pxtclutil::Warning "No Image in Memory\n Cannot Reslice!"
	return  0
    }

    WatchOn
    
    set thr [ vtkpxMergeFmriConventional [ pxvtable::vnewobj ]]
    set arr [ vtkFloatArray [ pxvtable::vnewobj ]]
    set nt [  $thr ComputeROIStatistics $image_ref $image_trn $current_transform $arr]

    if { $nt > 0 } {
	pxtkconsole
	pxtkprint "ROI Statistics for reference image  (and roi image resliced using current transformation)\n"
	set total  0
	set totalv 0 

	for { set i 0 } { $i < [ $arr GetNumberOfTuples ] } { incr i } {
	    set a1 [ expr round([ $arr GetComponent $i 0 ])]
	    set a2 [ expr round([ $arr GetComponent $i 1 ])]
	    set a3 [ $arr GetComponent $i 3 ]
	    set a4 [ $arr GetComponent $i 4 ]
	    set n1 [ expr round([ $arr GetComponent $i 2 ])]
	    set v1 [ expr round([ $arr GetComponent $i 6 ])]
	    set n2 [ expr round([ $arr GetComponent $i 5 ])]
	    set v2 [ expr round([ $arr GetComponent $i 7 ])]

	    if { $n1 >  0 } {
		pxtkprint [ format "Frame %3d\t Level=%5.1f:\t m=%5.2f s=%5.2f\t nvox=%7d (%.1f mm^3) nvox>0=%7d (%.1f mm^3)\n" $a1 $a2 $a3 $a4 $n1 $v1 $n2 $v2 ]
	    }

	    if { $a2 > 0 } {
		set total [ expr $total + $n2 ]
		set totalv [ expr $totalv + $v2 ]
	    }
	}
	pxtkprint [ format "Total volume for levels >0  nvox>0 = %d  (%.1f mm^3)\n\n" $total $totalv ]
    }

    $thr Delete
    $arr Delete
    
    WatchOff 
    
    return 1
}



# -------------------------------------------------------------------------------
#  Registration Stuff
# -------------------------------------------------------------------------------

itcl::body pxitcloverlaytool::UpdateFromRegistration { current_registration } {

    if { [ $current_registration GetProgress ] == 1.0 } {
	$this UpdateFromRegistrationDone $current_registration
	return
    } 

    puts stderr "[ $current_registration GetOutputStatus ]"

    set thisparam($this,reg_updatecount)  [ expr $thisparam($this,reg_updatecount) + 1 ]
    if { $thisparam($this,reg_updatecount) >= $thisparam($this,reg_updatesample) } {
	SetTransformation  [ $current_registration GetUpdateTransform ]
	Update
	WatchOn
	set thisparam($this,reg_updatecount) 0
    }
    $this ProgressCallback  $current_registration "Registering Images"
}

itcl::body pxitcloverlaytool::UpdateFromRegistrationDone { current_registration } {

    pxtkprint "[ $current_registration GetOutputStatus ]"
    if { [ $current_registration IsA "vtkpxLinearRegistration" ] == 1 } {
	SetTransformation   [ $current_registration GetUpdateTransform ]
    } else {
	SetTransformation   [ $current_registration GetTransformation ]
    }
    eval "$this ProgressCallback  $current_registration Done" 
    Update
}

itcl::body pxitcloverlaytool::AutoSaveTransformation { } {

    set newtr [ $this GetTransformation ]
    set n0 [ file dirname [ $tmap cget -filename ] ]
    set n1 [ file tail [ $currentimage cget -filename ] ]
    set n2 [ file tail [ file rootname [ $tmap cget -filename ] ] ]
    set suffix "matr"

    
    if { [ $newtr IsA "vtkGridTransform" ] == 1 } {
	set suffix "grd"
    } elseif { [ $newtr IsA "vtkpxComboTransform" ] == 1 } {
	set suffix "grd"
    } elseif { [ $newtr IsA "vtkThinPlateSplineTransform" ] == 1 } {
	set suffix "tps"
    }

    set mname [ file join $n0 "${n1}_${n2}.${suffix}" ]
    set ok 1
    
    if { $thisparam($this,reg_autooverwrite) == 0 } {
	if { [ file exists $mname ] ==1 } {
	    set ok [ ::pxtclutil::Question "Saving transformation in $mname will overwrite existing file. Are you sure?"]
	}
    }

    if { $ok == 1 } {

	set ok [ [ $transform_control GetCurrentObject ] Save $mname "" ]
    
	if { $ok == 1 } {
	    ::pxtclutil::Info " Transformation Saved in $mname!"
	    $transform_control CurrentObjectModified
	}
    }
}

# -----------------------------------------------------
# Affine transformation
# -------------------------------------------------------------------------------

itcl::body pxitcloverlaytool::AffineControlSetMode { mode } {
    
    switch $mode {
	"rigid2d" {
	    set thisparam($this,areg_tx) 1 ; 	set thisparam($this,areg_ty) 1; 	set thisparam($this,areg_tz) 0 ;
	    set thisparam($this,areg_rx) 0 ; 	set thisparam($this,areg_ry) 0; 	set thisparam($this,areg_rz) 1 ;
	    set thisparam($this,areg_sx) 0 ; 	set thisparam($this,areg_sy) 0; 	set thisparam($this,areg_sz) 0 ;
	    set thisparam($this,areg_ax) 0 ; 	set thisparam($this,areg_ay) 0; 	set thisparam($this,areg_az) 0 ;
	}
	"rigid" {
	    set thisparam($this,areg_tx) 1 ; 	set thisparam($this,areg_ty) 1; 	set thisparam($this,areg_tz) 1 ;
	    set thisparam($this,areg_rx) 1 ; 	set thisparam($this,areg_ry) 1; 	set thisparam($this,areg_rz) 1 ;
	    set thisparam($this,areg_sx) 0 ; 	set thisparam($this,areg_sy) 0; 	set thisparam($this,areg_sz) 0 ;
	    set thisparam($this,areg_ax) 0 ; 	set thisparam($this,areg_ay) 0; 	set thisparam($this,areg_az) 0 ;
	}
	"similarity" {
	    set thisparam($this,areg_tx) 1 ; 	set thisparam($this,areg_ty) 1; 	set thisparam($this,areg_tz) 1 ;
	    set thisparam($this,areg_rx) 1 ; 	set thisparam($this,areg_ry) 1; 	set thisparam($this,areg_rz) 1 ;
	    set thisparam($this,areg_sx) 1 ; 	set thisparam($this,areg_sy) 1; 	set thisparam($this,areg_sz) 1 ;
	    set thisparam($this,areg_ax) 0 ; 	set thisparam($this,areg_ay) 0; 	set thisparam($this,areg_az) 0 ;
	}
	"affine" {
	    set thisparam($this,areg_tx) 1 ; 	set thisparam($this,areg_ty) 1; 	set thisparam($this,areg_tz) 1 ;
	    set thisparam($this,areg_rx) 1 ; 	set thisparam($this,areg_ry) 1; 	set thisparam($this,areg_rz) 1 ;
	    set thisparam($this,areg_sx) 1 ; 	set thisparam($this,areg_sy) 1; 	set thisparam($this,areg_sz) 1 ;
	    set thisparam($this,areg_ax) 1 ; 	set thisparam($this,areg_ay) 1; 	set thisparam($this,areg_az) 1 ;
	}
    }
}

itcl::body pxitcloverlaytool::CreateImageAffineControl { parent } {

    label $parent.o -bg black -fg white -text "Rigid/Affine Registration"
    pack $parent.o -side top -expand f -fill x
    
    set nt $parent.note 
    iwidgets::tabnotebook $nt -tabpos n -angle 45 -margin 2 -backdrop gray -raiseselect true
    pack $nt -side bottom -expand t -fill both

    set simp [ $nt add -label "Simple" ]
    set adv    [ $nt add -label "Advanced" ]

    $nt view "Simple" 
	     
    # Simple Controls

    set p1 [ frame $simp.top ]
    set p2 [ frame $simp.mid ]
    set p2a [ frame $simp.mid2 ]
    set p3 [ frame $simp.bot ]
    pack $p1 $p2 $p2a $p3 -side top -expand f -fill x -pady 5 -padx 10

    label $p1.lab2 -text "Resolution"
    tk_optionMenu $p1.res [ itcl::scope thisparam($this,areg_resolution) ] "-1.0" "0.5" "0.8" "1.0" "1.1" "1.2" "1.5" "2.0" "4.0" "6.0" "8.0" "10.0" "40.0"
    pack $p1.lab2 $p1.res -side left -expand f -fill x	

    checkbutton $p2.c1 -variable [ itcl::scope thisparam($this,areg_autosave) ] -text "Auto Save Results" 
    checkbutton $p2.c2 -variable [ itcl::scope thisparam($this,areg_autooverwrite) ] -text "Overwrite Older Results" 
    pack $p2.c1 $p2.c2 -side left -expand f -fill x -padx 5 -pady 5

    checkbutton $p2a.c -variable [ itcl::scope thisparam($this,areg_usecurrent) ] -text "Use Current Transform For Initialization" 
    pack $p2a.c -side left -expand t -fill x -padx 10

    eval "button $p3.b1 -text \"Rigid\" -command { $this AffineControlSetMode rigid; $this ComputeAffineRegistration } "
    eval "button $p3.b1n -text \"Rigid 2D\" -command { $this AffineControlSetMode rigid2d; $this ComputeAffineRegistration } "
    #    eval "button $p3.b2 -text \"Reorient Image\" -command { $this ComputeReorientationRegistration } "
    eval "button $p3.b3 -text \"Affine\" -command { $this AffineControlSetMode affine; $this ComputeAffineRegistration} "

    pack $p3.b1 $p3.b1n $p3.b3 -side left -padx 10 -expand t -fill x 
   
    lappend thisparam($this,buttonlist) $p3.b1 $p3.b1n $p3.b3

    # Advance Stuff 

    frame $adv.m ; frame $adv.n ; frame $adv.o ;frame $adv.p ; frame $adv.p2 ; frame $adv.p3; frame $adv.u ; frame $adv.v ; frame $adv.i ;
    pack  $adv.m -pady 20
    pack  $adv.n $adv.o $adv.p $adv.p2 $adv.p3 $adv.i $adv.v $adv.u -side top -pady 2
  
    
    checkbutton $adv.m.tx -variable [ itcl::scope thisparam($this,areg_tx) ] -text "X-Trans" ; grid $adv.m.tx -column 0 -row 0
    checkbutton $adv.m.ty -variable [ itcl::scope thisparam($this,areg_ty) ] -text "Y-Trans" ; grid $adv.m.ty -column 0 -row 1
    checkbutton $adv.m.tz -variable [ itcl::scope thisparam($this,areg_tz) ] -text "Z-Trans" ; grid $adv.m.tz -column 0 -row 2
    
    checkbutton $adv.m.rx -variable [ itcl::scope thisparam($this,areg_rx) ] -text "X-Rot" ; grid $adv.m.rx -column 1 -row 0
    checkbutton $adv.m.ry -variable [ itcl::scope thisparam($this,areg_ry) ] -text "Y-Rot" ; grid $adv.m.ry -column 1 -row 1
    checkbutton $adv.m.rz -variable [ itcl::scope thisparam($this,areg_rz) ] -text "Z-Rot" ; grid $adv.m.rz -column 1 -row 2
    
    checkbutton $adv.m.sx -variable [ itcl::scope thisparam($this,areg_sx) ] -text "X-Scale" ; grid $adv.m.sx -column 2 -row 0
    checkbutton $adv.m.sy -variable [ itcl::scope thisparam($this,areg_sy) ] -text "Y-Scale" ; grid $adv.m.sy -column 2 -row 1
    checkbutton $adv.m.sz -variable [ itcl::scope thisparam($this,areg_sz) ] -text "Z-Scale" ; grid $adv.m.sz -column 2 -row 2
    
    checkbutton $adv.m.ax -variable [ itcl::scope thisparam($this,areg_ax) ] -text "X-Shear" ; grid $adv.m.ax -column 3 -row 0
    checkbutton $adv.m.ay -variable [ itcl::scope thisparam($this,areg_ay) ] -text "Y-Shear" ; grid $adv.m.ay -column 3 -row 1
    checkbutton $adv.m.az -variable [ itcl::scope thisparam($this,areg_az) ] -text "Z-Shear" ; grid $adv.m.az -column 3 -row 2
    
    eval "button $adv.m.b1 -text Rigid -command { $this AffineControlSetMode rigid} "
    eval "button $adv.m.b2 -text Similarity -command { $this AffineControlSetMode similarity} "
    eval "button $adv.m.b3 -text Affine -command { $this AffineControlSetMode affine} "
    grid $adv.m.b1 -column 4 -row 0
    grid $adv.m.b2 -column 4 -row 1
    grid $adv.m.b3 -column 4 -row 2
    
    label $adv.n.lab2 -text "Resolution"
    tk_optionMenu $adv.n.res [ itcl::scope thisparam($this,areg_resolution) ] "-1.0" "0.5" "0.8" "1.0" "1.1" "1.2" "1.5" "2.0" "4.0" "6.0" "8.0" "10.0"
    pack $adv.n.lab2 $adv.n.res -side left -expand f -fill x	
    
    
    label $adv.n.lab3 -text "Mode:"
    eval "tk_optionMenu $adv.n.res3 [ itcl::scope thisparam($this,reg_measure)] $reg_measures "
    pack $adv.n.lab3 $adv.n.res3  -side left -expand f -fill x	

    checkbutton $adv.o.auton  -text "Auto Normalize" -variable [ itcl::scope thisparam($this,areg_autonormalize) ]
    checkbutton $adv.o.jointn -text "Jointly Normlz" -variable [ itcl::scope thisparam($this,areg_jointlynormalize) ]
    checkbutton $adv.o.posm -text "Pos Inten" -variable [ itcl::scope thisparam($this,areg_positive) ]
    checkbutton $adv.o.logm -text "Logarithm" -variable [ itcl::scope thisparam($this,areg_logmode) ]
    pack $adv.o.auton $adv.o.jointn $adv.o.posm $adv.o.logm -side left -expand f -fill x	        


    label $adv.p.lab -text "Number Of Levels/Steps"
    tk_optionMenu $adv.p.res [ itcl::scope thisparam($this,areg_numlevels) ] 1 2 3
    tk_optionMenu $adv.p.res1 [ itcl::scope thisparam($this,areg_numsteps) ] 1 2 3 4
    label $adv.p.lab2 -text "Step Size(vx)"
    tk_optionMenu $adv.p.res2 [ itcl::scope thisparam($this,areg_stepsize) ] 0.05 0.1 0.25 0.5 1.0 2.0 3.0 4.0
    pack $adv.p.lab $adv.p.res $adv.p.res1 $adv.p.lab2 $adv.p.res2 -side left -expand f -fill x	
    
    label $adv.p2.lab2 -text "Opt Method"
    eval "tk_optionMenu $adv.p2.res2 [ itcl::scope thisparam($this,areg_optmethod) ] $opt_methods"
    label $adv.p2.lab3 -text "Iterations"
    tk_optionMenu $adv.p2.res3 [ itcl::scope thisparam($this,areg_iterations) ] 5 10 15 20 25 30 

    pack $adv.p2.lab2 $adv.p2.res2 $adv.p2.lab3 $adv.p2.res3  -side left -expand f -fill x	

    #pack $adv.p2.lab2 $adv.p2.res2  -side left -expand f -fill x	


    label $adv.p3.lab2 -text "Interpolation Mode"
    tk_optionMenu $adv.p3.res2 [ itcl::scope thisparam($this,areg_interpmode) ] -5 -4 -3 -2 0 1 3
    pack $adv.p3.lab2 $adv.p3.res2 -side left -expand f -fill x	
    

    checkbutton $adv.v.c1 -variable [ itcl::scope thisparam($this,areg_autosave) ] -text "Auto Save Results" 
    checkbutton $adv.v.c2 -variable [ itcl::scope thisparam($this,areg_autooverwrite) ] -text "Overwrite Older Results" 
    pack $adv.v.c1 $adv.v.c2 -side left -expand t -fill x -padx 5 -pady 5
    
    checkbutton $adv.i.c -variable [ itcl::scope thisparam($this,areg_usecurrent) ] -text "Use Current Transform For Initialization" 
    pack $adv.i.c -side left -expand t -fill x -padx 10
    
    label $adv.u.lab1 -text "Update Display Every:"
    tk_optionMenu $adv.u.param [ itcl::scope thisparam($this,reg_updatesample) ] "1" "2" "5" "10" "20" "100"  
    label $adv.u.lab2 -text "Iterations"
    pack $adv.u.lab1 $adv.u.param $adv.u.lab2 -side left -expand f -fill x -pady 5
    
    
    eval "button $adv.c -text \"Compute Linear Registration\" -command { $this ComputeAffineRegistration }"
    pack $adv.c -side bottom -expand f -fill x -padx 20
    lappend thisparam($this,buttonlist) $adv.c

    ::pxitclbasecontrol::SetWidgetTreeColors $adv "\#aaabd7" "black"
}


itcl::body pxitcloverlaytool::CreateImageNonLinearControl { parent } {

    label $parent.o -bg black -fg white -text "Non-rigid Intensity Based-Registration"
    pack $parent.o -side top -expand f -fill x
    
    set nt $parent.note 
    iwidgets::tabnotebook $nt -tabpos n  -angle 45 -margin 2 -backdrop gray -raiseselect true
    pack $nt -side bottom -expand t -fill both

    set simp [ $nt add -label "Simple" ]
    set w    [ $nt add -label "Advanced" ]
    $nt view "Simple"
    set adv $w 
    
    # Simple Controls
    set p1 [ frame $simp.top ]
    set p2 [ frame $simp.mid ]
    set p3 [ frame $simp.bot ]
    pack $p1 $p2 $p3 -side top -expand f -fill x -pady 5 -padx 10
    

    label $p1.lab2 -text "Resolution (vx)"
    tk_optionMenu $p1.res2 [ itcl::scope thisparam($this,nreg_resolution) ] "0.0" "1.0" "1.1" "1.2" "1.5" "2.0" "2.5" "3.0" "3.5" "4.0" "5.0"
    label $p1.lab1 -text "Control Spacing (mm)"
    tk_optionMenu $p1.res1 [ itcl::scope thisparam($this,nreg_spacing) ] 1.0 1.1 1.2 1.5 2.0 3.0 5.0 6.0 8.0 10.0 12.0 15.0 20.0 24.0 30.0 32.0 40.0 45.0 50.0 
    pack $p1.lab2 $p1.res2 $p1.lab1 $p1.res1  -side left -expand f -fill x	

    checkbutton $p2.c1 -variable [ itcl::scope thisparam($this,nreg_autosave) ] -text "Auto Save" 
    checkbutton $p2.c2 -variable [ itcl::scope thisparam($this,nreg_autooverwrite) ] -text "Overwrite Older"
    pack $p2.c1 $p2.c2  -side left -expand t -fill x -padx 5 -pady 5
    
    eval "button $p3.2 -text \"Compute Linear + Non Linear Registration\" -command { $this ComputeCombinedRegistration registration }"
    pack $p3.2   -side left -expand f -fill x -padx 5 -pady 5
    lappend thisparam($this,buttonlist) $p3.2

 
    # Advanced Controls
    frame $w.m ; frame $w.n ; frame $w.o ; frame $w.p ; frame $w.p2 ; frame $w.u ; frame $w.v ; frame $w.i
    pack $w.n $w.o $w.m $w.p $w.p2 $w.i $w.v $w.u -side top -pady 2
    
    label $w.n.lab2 -text "Resolution (vx)"
    tk_optionMenu $w.n.res2 [ itcl::scope thisparam($this,nreg_resolution) ] "0.0" "1.0" "1.1" "1.2" "1.5" "2.0" "2.5" "3.0" "3.5" "4.0" "5.0"
    label $w.n.lab1 -text "Control Spacing/Rate (mm)"
    tk_optionMenu $w.n.res1 [ itcl::scope thisparam($this,nreg_spacing) ] 1.0 1.1 1.2 1.5 2.0 3.0 5.0 6.0 8.0 10.0 12.0 15.0 20.0 24.0 30.0 32.0 40.0 45.0 50.0 
    tk_optionMenu $w.n.res3 [ itcl::scope thisparam($this,nreg_spacingrate) ] 1.0 1.5 2.0
    pack $w.n.lab2 $w.n.res2 $w.n.lab1 $w.n.res1 $w.n.res3 -side left -expand f -fill x	
    
    checkbutton $w.o.auton  -text "Auto Normalize" -variable [ itcl::scope thisparam($this,nreg_autonormalize) ]
    checkbutton $w.o.jointn -text "Jointly Normlz" -variable [ itcl::scope thisparam($this,nreg_jointlynormalize) ]
    checkbutton $w.o.posm -text "Pos Inten" -variable [ itcl::scope thisparam($this,nreg_positive) ]
    checkbutton $w.o.logm -text "Logarithm" -variable [ itcl::scope thisparam($this,nreg_logmode) ]
    pack $w.o.auton $w.o.jointn $w.o.posm $w.o.logm -side left -expand f -fill x	        


    label $w.m.lab3 -text "Levels/Steps"
    tk_optionMenu $w.m.res3 [ itcl::scope thisparam($this,nreg_numlevels) ] 1 2 3
    tk_optionMenu $w.m.res4 [ itcl::scope thisparam($this,nreg_numsteps) ] 1 2 3 4
    label $w.m.lab5 -text "Step Size (vx)"
    tk_optionMenu $w.m.res5 [ itcl::scope thisparam($this,nreg_stepsize) ] 0.25 0.5 1.0 2.0 3.0 4.0 8.0 12.0 16.0
    pack $w.m.lab3 $w.m.res3 $w.m.res4  $w.m.lab5 $w.m.res5 -side left -expand f -fill x	
    
    label $w.p.lab3 -text "Mode:"
    eval "tk_optionMenu $w.p.res3 [ itcl::scope thisparam($this,reg_measure) ] $reg_measures"

    pack $w.p.lab3 $w.p.res3   -side left -expand f -fill x	
    label $w.p.lab4 -text "Smoothness:"
    entry $w.p.res4 -textvariable [ itcl::scope thisparam($this,nreg_smoothness) ] -width 5 -relief sunken
    pack $w.p.lab4 $w.p.res4 -side left -expand f -fill x	
   
    label $w.p2.lab2 -text "Opt Method"
    eval "tk_optionMenu $w.p2.res2 [ itcl::scope thisparam($this,nreg_optmethod) ] $nopt_methods"

    label $w.p2.lab3 -text "Iterations"
    tk_optionMenu $w.p2.res3 [ itcl::scope thisparam($this,nreg_iterations) ] 1 5 10 15 20 25 30 

    label $w.p2.lab4 -text "GScale:"
    tk_optionMenu $w.p2.res4 [ itcl::scope thisparam($this,nreg_gscale) ] 0.1 0.5 1.0 2.0 4.0 8.0 16.0

    pack $w.p2.lab2 $w.p2.res2 $w.p2.lab3 $w.p2.res3 $w.p2.lab4 $w.p2.res4 -side left -expand f -fill x	
       
       
    checkbutton $w.v.c1 -variable [ itcl::scope thisparam($this,nreg_autosave) ] -text "Auto Save Results" 
    checkbutton $w.v.c2 -variable [ itcl::scope thisparam($this,nreg_autooverwrite) ] -text "Overwrite Older Results" 
    pack $w.v.c1 $w.v.c2 -side left -expand t -fill x -padx 5 -pady 5
    
    checkbutton $w.i.c -variable [ itcl::scope thisparam($this,nreg_usecurrent) ] -text "Use Current Transform For Initialization" 
    checkbutton $w.i.d -variable [ itcl::scope thisparam($this,nreg_useprior) ] -text "For Prior" 
    pack $w.i.c $w.i.d -side left -expand t -fill x -padx 5


    label $w.u.lab1 -text "Update Display Every:"
    tk_optionMenu $w.u.param [ itcl::scope thisparam($this,reg_updatesample) ] "1" "2" "5" "10" "20" "100"  
    label $w.u.lab2 -text "Iterations"
    pack $w.u.lab1 $w.u.param $w.u.lab2 -side left -expand f -fill x -pady 5
	
    eval "button $w.c -text \"Compute Non Linear Registration\" -command { $this ComputeNonLinearRegistration  }"
    eval "button $w.d -text \"Compute Linear + Non Linear Registration\" -command { $this ComputeCombinedRegistration registration }"
    lappend  thisparam($this,buttonlist) $w.c
    lappend  thisparam($this,buttonlist) $w.d
    pack $w.d $w.c -side bottom -expand f -fill x -padx 5

    ::pxitclbasecontrol::SetWidgetTreeColors $adv "\#aaabd7" "black"
}


itcl::body pxitcloverlaytool::CreateImageDistortionCorrectionControl { parent } {

    label $parent.o -bg black -fg white -text "Distortion Correction for EPI"
    pack $parent.o -side top -expand f -fill x
    
    set nt $parent.note 
    iwidgets::tabnotebook $nt -tabpos n  -angle 45 -margin 2 -backdrop gray -raiseselect true
    pack $nt -side bottom -expand t -fill both

    set simp [ $nt add -label "Simple" ]
    set w    [ $nt add -label "Advanced" ]

    $nt view "Simple"
    set adv $w
    
    # Simple Controls
    set p1 [ frame $simp.top ]
    set p2 [ frame $simp.mid ]
    set p3 [ frame $simp.bot ]
    pack $p1 $p2 $p3 -side top -expand f -fill x -pady 5 -padx 10
    

    label $p1.lab2 -text "Resolution (vx)"
    tk_optionMenu $p1.res2 [ itcl::scope thisparam($this,dist_resolution) ] "0.0" "1.0" "1.1" "1.2" "1.5" "2.0" "2.5" "3.0" "3.5" "4.0" "5.0" "8.0"
    label $p1.lab1 -text "Control Spacing (mm)"
    tk_optionMenu $p1.res1 [ itcl::scope thisparam($this,dist_spacing) ] 5.0 6.0 8.0 10.0 12.0 15.0 20.0 24.0 30.0 32.0 40.0 45.0 50.0 
    pack $p1.lab2 $p1.res2 $p1.lab1 $p1.res1 -side left -expand f -fill x	
    
    #    label $p1.lab5 -text "Step Size (vx)"
    #    tk_optionMenu $p1.res5 [ itcl::scope thisparam($this,dist_stepsize) ] 0.25 0.5 1.0 2.0 3.0 4.0 8.0 12.0 16.0
    #    pack  $p1.lab5 $p1.res5 -side left -expand f -fill x	
    
    checkbutton $p2.c1 -variable [ itcl::scope thisparam($this,dist_autosave) ] -text "Auto Save" 
    checkbutton $p2.c2 -variable [ itcl::scope thisparam($this,dist_autooverwrite) ] -text "Overwrite Older"
    pack $p2.c1 $p2.c2  -side left -expand t -fill x -padx 5 -pady 5
    
    #eval "button $p3.1 -text \"Compute Distortion Correction\" -command { $this ComputeDistortionCorrection  }"
    eval "button $p3.2 -text \"Compute Linear + Distortion Correction\" -command { $this ComputeCombinedRegistration distortion }"
    pack $p3.2   -side left -expand f -fill x -padx 5 -pady 5
    lappend thisparam($this,buttonlist) $p3.2 

    # Advanced Stuff
    frame $w.m ; frame $w.n ; frame $w.p ; frame $w.p2 ; frame $w.u ; frame $w.v ; frame $w.i ; frame $w.j
    pack  $w.n $w.m $w.p $w.p2 $w.j $w.i $w.v $w.u -side top -pady 2
    
    label $w.n.lab2 -text "Resolution (vx)"
    tk_optionMenu $w.n.res2 [ itcl::scope thisparam($this,dist_resolution) ] "0.0" "1.0" "1.1" "1.2" "1.5" "2.0" "2.5" "3.0" "3.5" "4.0" "5.0" "8.0"
    label $w.n.lab1 -text "Control Spacing/Rate (mm)"
    tk_optionMenu $w.n.res1 [ itcl::scope thisparam($this,dist_spacing) ] 5.0 6.0 8.0 10.0 12.0 15.0 20.0 24.0 30.0 32.0 40.0 45.0 50.0 
    tk_optionMenu $w.n.res3 [ itcl::scope thisparam($this,dist_spacingrate) ] 1.0 1.5 2.0
    pack $w.n.lab2 $w.n.res2 $w.n.lab1 $w.n.res1 $w.n.res3 -side left -expand f -fill x	
    
    
    label $w.m.lab3 -text "Levels/Steps"
    tk_optionMenu $w.m.res3 [ itcl::scope thisparam($this,dist_numlevels) ] 1 2 3
    tk_optionMenu $w.m.res4 [ itcl::scope thisparam($this,dist_numsteps) ] 1 2 3 4
    label $w.m.lab5 -text "Step Size (vx)"
    tk_optionMenu $w.m.res5 [ itcl::scope thisparam($this,dist_stepsize) ] 0.25 0.5 1.0 2.0 3.0 4.0 8.0 12.0 16.0
    pack $w.m.lab3 $w.m.res3 $w.m.res4  $w.m.lab5 $w.m.res5 -side left -expand f -fill x	
    
    label $w.p.lab4 -text "Smoothness:"
    entry $w.p.res4 -textvariable [ itcl::scope thisparam($this,dist_smoothness) ] -width 5 -relief sunken
    checkbutton $w.p.chk3 -variable [ itcl::scope thisparam($this,dist_logmode) ] -text "Logarithm Mode" 

    label $w.p.lab5 -text "Adjust:"
    tk_optionMenu $w.p.res5 [ itcl::scope thisparam($this,dist_jackmode) ] "None" "Signal Conservation" 
    #"Signal Loss"

    #checkbutton $w.p.chk4 -variable [ itcl::scope thisparam($this,dist_jackmode) ] -text "Jacobian Mode" 
    pack $w.p.lab4 $w.p.res4 $w.p.chk3 $w.p.lab5 $w.p.res5 -side left -expand f -fill x	
   
    label $w.p2.lab2 -text "Opt Method"
    eval "tk_optionMenu $w.p2.res2 [ itcl::scope thisparam($this,dist_optmethod) ] $nopt_methods"

    label $w.p2.lab3 -text "Iterations"
    tk_optionMenu $w.p2.res3 [ itcl::scope thisparam($this,dist_iterations) ] 5 10 15 20 25 30 
    pack $w.p2.lab2 $w.p2.res2 $w.p2.lab3 $w.p2.res3  -side left -expand f -fill x	
       

    label $w.j.lab2 -text "Phase - encode direction:"
    tk_optionMenu $w.j.res2 [ itcl::scope thisparam($this,dist_warp) ] "X" "Y" "Z"
    pack $w.j.lab2 $w.j.res2 -side left -expand f -fill x
       
    checkbutton $w.v.c1 -variable [ itcl::scope thisparam($this,dist_autosave) ] -text "Auto Save Results" 
    checkbutton $w.v.c2 -variable [ itcl::scope thisparam($this,dist_autooverwrite) ] -text "Overwrite Older Results" 
    pack $w.v.c1 $w.v.c2 -side left -expand t -fill x -padx 5 -pady 5
    
    checkbutton $w.i.c -variable [ itcl::scope thisparam($this,dist_usecurrent) ] -text "Use Current Transform For Initialization" 
    checkbutton $w.i.d -variable [ itcl::scope thisparam($this,dist_useprior) ] -text "For Prior" 
    pack $w.i.c $w.i.d -side left -expand t -fill x -padx 5


    label $w.u.lab1 -text "Update Display Every:"
    tk_optionMenu $w.u.param [ itcl::scope thisparam($this,reg_updatesample) ] "1" "2" "5" "10" "20" "100"  
    label $w.u.lab2 -text "Iterations"
    pack $w.u.lab1 $w.u.param $w.u.lab2 -side left -expand f -fill x -pady 5

	
    eval "button $w.c -text \"Compute Distortion Correction\" -command { $this ComputeDistortionCorrection  }"
    eval "button $w.d -text \"Compute Linear + Distortion Correction\" -command { $this ComputeCombinedRegistration distortion }"

    lappend  thisparam($this,buttonlist) $w.c
    lappend  thisparam($this,buttonlist) $w.d
    pack $w.d $w.c -side bottom -expand f -fill x -padx 5

    ::pxitclbasecontrol::SetWidgetTreeColors $adv "\#aaabd7" "black"
}

itcl::body pxitcloverlaytool::EnableRegistrationButtons { }  {
    EnableUI $thisparam($this,buttonlist)
}

itcl::body pxitcloverlaytool::DisableRegistrationButtons { } {
    DisableUI $thisparam($this,buttonlist)
}

itcl::body pxitcloverlaytool::SetAutoSave { mode } {
    set thisparam($this,reg_autosave) $mode
    set thisparam($this,areg_autosave) $mode
    set thisparam($this,nreg_autosave) $mode
    set thisparam($this,dist_autosave) $mode
}

itcl::body pxitcloverlaytool::ComputeAffineRegistration { } {

    set current_transform [ $this GetTransformation ]
    $transform_control AddObject  "lin.[ $transform_control GetNextObjectIndex ]"

    global pxtcl_pref_array
    set image_ref $currentimage
    set image_trn $tmap
    
    set orient_ref [ $currentimage GetOrientation ]
    set orient_trn [ $tmap GetOrientation ]
    
    puts stderr "Orientations = ref=$orient_ref trg=$orient_trn"

    set dim1 [ [ $image_ref GetImage ] GetDimensions ]
    set dim2 [ [ $image_trn GetImage ] GetDimensions ]
    if { [lindex $dim2 0 ] <2 || [ lindex $dim1 0 ] < 2 } { 
	::pxtclutil::Warning "No Image in Memory\n Cannot Register!"
	return  0
    }
    
    WatchOn
    
    set mat [ vtkMatrix4x4 [ pxvtable::vnewobj ]]
    $mat Identity
    
    set md [ lsearch -exact $reg_measures $thisparam($this,reg_measure) ]
    incr md
    
    set areg [ vtkpxLinearRegistration [ pxvtable::vnewobj ]]
    
    if { $thisparam($this,areg_usecurrent) == 1 } {
	set xform [ ExtractLinearTransform $current_transform $image_ref $image_trn 9 ]
	$areg SetInitialTransform $xform
	$xform Delete
    }
    
    $areg SetTranslationX $thisparam($this,areg_tx)
    $areg SetTranslationY $thisparam($this,areg_ty)
    $areg SetTranslationZ $thisparam($this,areg_tz)
    $areg SetRotationX $thisparam($this,areg_rx)
    $areg SetRotationY $thisparam($this,areg_ry)
    $areg SetRotationZ $thisparam($this,areg_rz)
    $areg SetScalingX $thisparam($this,areg_sx)
    $areg SetScalingY $thisparam($this,areg_sy)
    $areg SetScalingZ $thisparam($this,areg_sz)
    $areg SetShearingXY $thisparam($this,areg_ax)
    $areg SetShearingYX $thisparam($this,areg_ax)
    $areg SetShearingYZ $thisparam($this,areg_ay)
    $areg SetShearingZY $thisparam($this,areg_ay)
    $areg SetShearingZX $thisparam($this,areg_az)
    $areg SetShearingXZ $thisparam($this,areg_az)
    $areg SetSimilarityMeasure $md
    $areg SetReferenceImage [ $image_ref GetImage ]
    $areg SetTransformImage [ $image_trn GetImage ]
    $areg SetReferenceOrientation $orient_ref
    $areg SetTransformOrientation $orient_trn

    $areg SetResolution $thisparam($this,areg_resolution)
    $areg SetNumberOfLevels $thisparam($this,areg_numlevels)
    $areg SetNumberOfSteps $thisparam($this,areg_numsteps)
    $areg SetStepSize       $thisparam($this,areg_stepsize) 
    $areg SetOptimizationMethodTo$thisparam($this,areg_optmethod)
    $areg SetNumberOfIterations $thisparam($this,areg_iterations)
    $areg SetPositiveIntensitiesOnly $thisparam($this,areg_positive)
    $areg SetLogarithmMode $thisparam($this,areg_logmode)
    $areg SetJointlyNormalizeIntensities $thisparam($this,areg_jointlynormalize)
    $areg SetAutoNormalizeIntensities $thisparam($this,areg_autonormalize)
    $areg SetInterpolationMode $thisparam($this,areg_interpmode)
    $areg DebugOff
    eval "$areg AddObserver ProgressEvent { $this UpdateFromRegistration $areg }"
    eval "$areg AddObserver EndEvent      { $this UpdateFromRegistrationDone $areg }"
    
    if { $pxtcl_pref_array(MirrorConsole) == 0 } { 
	pxtkconsole 
	pxtksetdebugmode 0
    } else {
	pxtksetdebugmode 1
    }

    update idletasks
    set thisparam($this,reg_updatecount) 0
    pxtkprint "\n\n Beginning Affine Registration , measure = $thisparam($this,reg_measure) Mode=$md\n"
    
    WatchOn
    $areg Run
    UpdateFromRegistrationDone $areg
    WatchOff

    SetTransformation  [ $areg GetTransformation ]
    
    if { $thisparam($this,areg_autosave) == 1 } {
	AutoSaveTransformation
    }
    

    $areg Delete

    WatchOff
    return 1
}


# --------------------------------------------------------------
itcl::body pxitcloverlaytool::ComputeReorientationRegistration { } {

    $this AffineControlSetMode affine; 
    $this ComputeAffineRegistration
    $this SetManualTransformGUI 0
    set thisparam($this,manxf_scalex) 100.0
    set thisparam($this,manxf_scaley) 100.0
    set thisparam($this,manxf_scalez) 100.0
    $this SetManualTransform ; 
    $this ResliceFunctionalImage 1 1
}


# --------------------------------------------------------------
# Non - Linear Registration
# --------------------------------------------------------------
itcl::body pxitcloverlaytool::ComputeNonLinearRegistration { } {

    set current_transform [ $this GetTransformation ]
    $transform_control AddObject  "nreg.[ $transform_control GetNumberOfObjects ]"

    global pxtcl_pref_array
    set image_ref $currentimage
    set image_trn $tmap
    
    set orient_ref [ $currentimage GetOrientation ]
    set orient_trn [ $tmap GetOrientation ]
    
    set dim1 [ $image_ref GetImageSize ]
    set dim2 [ $image_trn GetImageSize ]
    if { $dim2 <2 ||  $dim1 < 2 } { 
	::pxtclutil::Warning "No Image in Memory\n Cannot Register!"
	return  0
    }
    
    if { $orient_ref != $orient_trn } {
	::pxtclutil::Warning "Different Orientations ($orient_ref, $orient_trn) ! Cannot Register Images!"
	return 0;
    }
    
    set md [ lsearch -exact $reg_measures $thisparam($this,reg_measure) ]
    incr md
    
   
    pxtkprint "\n\n ------------------------------ \n\n\nBeginning NonLinear Registration , measure = $thisparam($this,reg_measure) Mode=$md\n"
    set nreg [ vtkpxNonLinearRegistration [ pxvtable::vnewobj ]]
    $nreg SetSimilarityMeasure $md
    $nreg SetLambda [ expr 0.01 * $thisparam($this,nreg_smoothness) ]


    if { $thisparam($this,nreg_usecurrent) == 1 } {
	$nreg SetInitialTransform $current_transform 
	#if { [ $current_transform IsA "vtkpxComboTransform" ] == 1 } {
	#    $nreg SetUsePrior $thisparam($this,nreg_useprior)
	#    $nreg SetPriorTransform $current_transform
	#    $nreg SetInternalEnergyModeToBendingEnergy
	#}
    } else {
	#$nreg SetUsePrior 0
    }
    $nreg DebugOff
    
    $nreg SetReferenceImage [  $image_ref GetImage ]
    $nreg SetTransformImage [ $image_trn GetImage  ]
    $nreg SetNumberOfSteps  $thisparam($this,nreg_numsteps)
    $nreg SetNumberOfLevels $thisparam($this,nreg_numlevels)
    $nreg SetStepSize       $thisparam($this,nreg_stepsize) 
    $nreg SetResolution  $thisparam($this,nreg_resolution) 
    $nreg SetControlPointSpacing $thisparam($this,nreg_spacing)
    $nreg SetControlPointSpacingRate $thisparam($this,nreg_spacingrate)
    $nreg SetOptimizationMethodTo$thisparam($this,nreg_optmethod)
    $nreg SetNumberOfIterations $thisparam($this,nreg_iterations)
    $nreg SetGradientScale $thisparam($this,nreg_gscale)
    $nreg SetPositiveIntensitiesOnly $thisparam($this,nreg_positive)
    $nreg SetJointlyNormalizeIntensities $thisparam($this,nreg_jointlynormalize)
    $nreg SetAutoNormalizeIntensities $thisparam($this,nreg_autonormalize)

    $nreg SetLogarithmMode $thisparam($this,nreg_logmode)

    eval "$nreg AddObserver ProgressEvent { $this UpdateFromRegistration $nreg }"
    eval "$nreg AddObserver EndEvent      { $this UpdateFromRegistrationDone $nreg }"
    
    if { $pxtcl_pref_array(MirrorConsole) == 0 } { 
	pxtkconsole 
	pxtksetdebugmode 0
    } else {
	pxtksetdebugmode 1
    }

    set thisparam($this,reg_updatecount) 0
    
    WatchOn
    set old $thisparam($this,reg_updatesample)  
    set thisparam($this,reg_updatesample)   [ expr $old * 5 ]
    $nreg Run
    UpdateFromRegistrationDone $nreg
    set thisparam($this,reg_updatesample) $old
    WatchOff
    
    SetTransformation  [ $nreg GetTransformation ]
    
    if { $thisparam($this,nreg_autosave) == 1 } {
	AutoSaveTransformation
    }
    
    $nreg Delete
    return 1
}

# --------------------------------------------------------------
# Non - Linear Registration
# --------------------------------------------------------------
itcl::body pxitcloverlaytool::ComputeDistortionCorrection { } {

    set current_transform [ $this GetTransformation ]
    $transform_control AddObject  "dist.[ $transform_control GetNumberOfObjects ]"

    global pxtcl_pref_array
    set image_ref $currentimage
    set image_trn $tmap
    
    set orient_ref [ $currentimage GetOrientation ]
    set orient_trn [ $tmap GetOrientation ]
    
    set dim1 [ $image_ref GetImageSize ]
    set dim2 [ $image_trn GetImageSize ]
    if { $dim2 <2 ||  $dim1 < 2 } { 
	::pxtclutil::Warning "No Image in Memory\n Cannot Register!"
	return  0
    }
    
    if { $orient_ref != $orient_trn } {
	::pxtclutil::Warning "Different Orientations! Cannot Register Images!"
	return 0;
    }
    
    set md [ lsearch -exact $reg_measures $thisparam($this,dist_measure) ]
    incr md
    
    
    pxtkprint "\n\n ------------------------------ \n\n\nBeginning Distortion Correction , measure = $thisparam($this,reg_measure) Mode=$md\n"
    set nreg [ vtkpxDistortionCorrection [ pxvtable::vnewobj ]]

    if { $thisparam($this,dist_jackmode) != "None" } {
	$nreg SetUseJacobian 1 
	if { $thisparam($this,dist_jackmode) == "Signal Loss" } {
	    $nreg SetUseSignalLossCorrection 1
	} else {
	    $nreg SetUseSignalLossCorrection 0
	}
    } else {
	$nreg SetUseSignalLossCorrection 0
	$nreg SetUseJacobian 0
    }

    pxtkprint "nreg jacobian [ $nreg GetUseJacobian ] signal correction [ $nreg GetUseSignalLossCorrection ]\n"

    $nreg SetSimilarityMeasure $md
    $nreg SetLambda [ expr 0.01 * $thisparam($this,dist_smoothness) ]
    $nreg SetLogarithmMode $thisparam($this,dist_logmode)


    if { $thisparam($this,dist_usecurrent) == 1 } {
	$nreg SetInitialTransform $current_transform 
	#if { [ $current_transform IsA "vtkpxComboTransform" ] == 1 } {
	#    $nreg SetUsePrior $thisparam($this,dist_useprior)
	#    $nreg SetPriorTransform $current_transform
	#    $nreg SetInternalEnergyModeToBendingEnergy
	#}
    } else {
	$nreg SetUsePrior 0
    }
    
    $nreg SetPhaseEncodeAxisTo$thisparam($this,dist_warp)
    $nreg SetReferenceImage [  $image_ref GetImage ]
    $nreg SetTransformImage [ $image_trn GetImage  ]
    $nreg SetNumberOfSteps  $thisparam($this,dist_numsteps)
    $nreg SetNumberOfLevels $thisparam($this,dist_numlevels)
    $nreg SetStepSize       $thisparam($this,dist_stepsize) 
    $nreg SetResolution  $thisparam($this,dist_resolution) 
    $nreg SetControlPointSpacing $thisparam($this,dist_spacing)
    $nreg SetControlPointSpacingRate $thisparam($this,dist_spacingrate)
    $nreg SetOptimizationMethodTo$thisparam($this,dist_optmethod)
    $nreg SetNumberOfIterations $thisparam($this,dist_iterations)


    eval "$nreg AddObserver ProgressEvent { $this UpdateFromRegistration $nreg }"
    eval "$nreg AddObserver EndEvent      { $this UpdateFromRegistrationDone $nreg }"
    
    if { $pxtcl_pref_array(MirrorConsole) == 0 } { 
	pxtkconsole 
	pxtksetdebugmode 0
    } else {
	pxtksetdebugmode 1
    }

    set thisparam($this,reg_updatecount) 0
    
    WatchOn
    set old $thisparam($this,reg_updatesample)  
    set thisparam($this,reg_updatesample)   [ expr $old * 5 ]
    $nreg Run
    UpdateFromRegistrationDone $nreg
    set thisparam($this,reg_updatesample) $old
    WatchOff
    
    SetTransformation  [ $nreg GetTransformation ]
    
    if { $thisparam($this,dist_autosave) == 1 } {
	AutoSaveTransformation
    }
    
    $nreg Delete
    return 1
}
# ------------------------------------------------
# Combo Stuff
# ------------------------------------------------

itcl::body pxitcloverlaytool::ComputeCombinedRegistration { mode } {

    set oldlog    $thisparam($this,areg_logmode)
    set oldlevels $thisparam($this,areg_numlevels)  
    set oldstep   $thisparam($this,areg_stepsize)
    set oldres    $thisparam($this,areg_resolution) 

    if { $mode == "registration" } {
	set thisparam($this,areg_logmode) $thisparam($this,nreg_logmode)
	AffineControlSetMode "affine"
    } else {
	AffineControlSetMode "rigid"
	set thisparam($this,areg_logmode) $thisparam($this,dist_logmode)
	set thisparam($this,areg_numlevels)  2
	set thisparam($this,areg_stepsize)   1.0
	set thisparam($this,areg_resolution)  $thisparam($this,dist_resolution) 
    }
    set  thisparam($this,areg_logmode) $oldlog

    set thisparam($this,nreg_usecurrent) 1
    set thisparam($this,areg_usecurrent) 0
    set thisparam($this,areg_autosave) 0

    ComputeAffineRegistration

    if { $mode == "registration" } {
	ComputeNonLinearRegistration
    } else {
	ComputeDistortionCorrection
    }
    
    set thisparam($this,areg_logmode) $oldlog
    set thisparam($this,areg_numlevels)  $oldlevels
    set thisparam($this,areg_stepsize) $oldstep
    set thisparam($this,areg_resolution) $oldres
}

# ------------------------------------------------
# Combine Control   Add or Strip Images 
# ------------------------------------------------


itcl::body pxitcloverlaytool::CreateCombineControl { parent } {
    
    set w  [ LabelFrame:create $parent.b -text "Add Images"]
    set w2  [ LabelFrame:create $parent.c -text "Strip Image"]
    pack $parent.b $parent.c -side top -expand t -fill both -padx 10 -pady 5
    
    
    frame $w.m; pack $w.m  -side top -expand f
    eval "button $w.m.1 -text \"Add Images\" -command { $this AddImages }"
    eval "button $w.m.2 -text \"Concatenate Object Maps\" -command { $this CombineObjectMaps }"
    pack $w.m.1 $w.m.2 -side left -expand f
    
    frame $w2.top ; pack $w2.top -side top -padx 20 -pady 2
    
    eval "button  $w2.top.1 -text \"Non-zero strip\" -command { $this StripImage }"
    pack $w2.top.1  -side left
    
}


itcl::body pxitcloverlaytool::AddImages { } {

    set upd [ ResliceFunctionalImage 0 0 ]
    if { $upd == 0 } {
	return 0
    }
    
    set image_ref [ $currentimage GetImage ]
    set image_trn [ $resliced_tmap GetImage ]

    WatchOn
    set math [ vtkImageMathematics [ pxvtable::vnewobj ]]
    $math SetInput1   $image_ref
    $math SetInput2   $image_trn
    $math SetOperationToAdd
    SetFilterCallbacks $math "Adding Images"
    $math Update
    ProcessResults  $math "Add" 
    $math Delete
    WatchOff 
    return 1
}

itcl::body pxitcloverlaytool::CombineObjectMaps { } {
    
    set upd [ ResliceFunctionalImage 0 0 ]
    if { $upd == 0 } {
	return 0
    }
       
    set image_ref [ $currentimage GetImage ]
    set image_trn [ $resliced_tmap GetImage ]
    
    set range [[[ $image_ref  GetPointData ] GetScalars ] GetRange ]
    set addc [ expr [ lindex $range 1 ] +1 ]
    
    WatchOn
    set sh  [ vtkImageShiftScale [ pxvtable::vnewobj ]  ]
    $sh SetInput $image_trn
    $sh SetShift $addc
    $sh SetScale 1
    SetFilterCallbacks $sh "Shifing Image"
    $sh Update
    
    set math [ vtkImageMathematics [ pxvtable::vnewobj ]]
    $math SetInput1  $image_ref
    $math SetInput2  [ $sh GetOutput ] 
    $math SetOperationToAdd
    SetFilterCallbacks $math "SAdding Images"
    $math Update
    ProcessResults  $math "Combo" 
    $math Delete
    $sh Delete
    WatchOff 
    
    return 1
}

itcl::body pxitcloverlaytool::StripImage { } {

    set upd [ ResliceFunctionalImage 0 0 ]
    if { $upd == 0 } {
	return 0
    }
    
    set range [[[  [ $currentimage GetImage ]  GetPointData ] GetScalars ] GetRange ]
    set addc [ expr [ lindex $range 1 ] +1 ]
    
    WatchOn
    set tr  [ vtkImageThreshold [ pxvtable::vnewobj ]  ]
    $tr SetInput [ $resliced_tmap GetImage ]
    $tr ThresholdByUpper 1.0
    $tr SetInValue 1.0
    $tr SetOutValue 0.0
    $tr ReplaceInOn
    $tr ReplaceOutOn
    SetFilterCallbacks $tr "Creating Mask"
    $tr Update

    
    set math [ vtkImageMathematics [ pxvtable::vnewobj ]]
    $math SetInput1 [ $currentimage GetImage ]
    $math SetInput2  [ $tr GetOutput ] 
    $math SetOperationToMultiply
    SetFilterCallbacks $math "Multiplying Images"
    $math Update
    ProcessResults  $math "Strip" 
    $math Delete
    $tr Delete
    WatchOff 
    
    return 1
}

# ----------------------------------------------------------------------------------------
itcl::body pxitcloverlaytool::SetImageToCompareControl { ctrl image } {
    $this DelayedInitialize
     [ $ctrl GetObject ] ShallowCopy $image; $ctrl Update 
}


itcl::body pxitcloverlaytool::ComputeCompareOperations { mode } {


    set img1 [ [ $meanimagecontrol1 GetObject ] GetImage ]
    set img2 [ [ $meanimagecontrol2 GetObject ] GetImage ]
    set sig1 [ [ $sigmaimagecontrol1 GetObject ] GetImage ]
    set sig2 [ [ $sigmaimagecontrol2 GetObject ] GetImage ]
    $currentresults CopyImageHeader [ [ $meanimagecontrol1 GetObject  ] GetImageHeader ]

    if { [ $img1 GetNumberOfPoints ] != [ $img2 GetNumberOfPoints ] } {
	::pxtclutil::Warning "Unequal Image Sizes\n Cannot Perform Compare Operation $mode!"
	return 0
    }

    if { $mode == "tmap" } {
	if { [ $img1 GetNumberOfPoints ] != [ $sig1 GetNumberOfPoints ] ||
	     ( [ $img1 GetNumberOfPoints ] != [ $sig2 GetNumberOfPoints ] && $thisparam($this,computetmapsamples2) > 1 ) } {
	    ::pxtclutil::Warning "Unequal Image Sizes\n Cannot Perform Compare Operation $mode!"
	    return 0
	}
    }
	
    WatchOn

    if { $mode == "tmap" } {
	set math [ vtkpxComputeTmap [ pxvtable::vnewobj ]]
	$math SetInput  $img1
	$math SetInput2 $img2
	$math SetInputStandardDeviation  $sig1
	$math SetInputStandardDeviation2 $sig2
	$math SetNumberOfSamples1 $thisparam($this,computetmapsamples1)
	$math SetNumberOfSamples2 $thisparam($this,computetmapsamples2)
	$math SetScaleFactor 1000.0
	$math Update
	$currentresults ShallowCopyImage [ $math GetOutput ]
	if { $thisparam($this,computetmapsamples1)  >1 && $thisparam($this,computetmapsamples2) > 1 } {
	    $currentresults configure -filename "tmap"  
	} else {
	    $currentresults configure -filename "zcore" 
	}
		
	puts stderr " Results Range: [ [ [ [ $currentresults GetImage ] GetPointData ] GetScalars ] GetRange ]"
	$math Delete
    } else {
	set imcast2 [ vtkImageCast [ pxvtable::vnewobj ]]
    	$imcast2 SetInput $img2
	$imcast2 SetOutputScalarType [ $img1 GetScalarType ]
	$imcast2 Update

	set math [ vtkImageMathematics [ pxvtable::vnewobj ]]
	$math SetInput1 $img1
	$math SetInput2 [ $imcast2 GetOutput ]
	
	if { $mode == "add" } {
	    $math SetOperationToAdd
	} else {
	    $math SetOperationToSubtract
	}
	$math Update
	$currentresults ShallowCopyImage [ $math GetOutput ]
	$math Delete
	$imcast2 Delete 
    }

    SetFunctionalImage $currentresults
    WatchOff 
    
    return 1
}
# ----------------------------------------------------------------------------------------
itcl::body pxitcloverlaytool::ComputeCompare4DOperations { mode } {

    set img1 [ [ $fourdimagecontrol1 GetObject ] GetImage ]
    set img2 [ [ $fourdimagecontrol2 GetObject ] GetImage ]
    $currentresults CopyImageHeader [ [ $fourdimagecontrol1 GetObject  ] GetImageHeader ]

    if { [ $img1 GetNumberOfPoints ] != [ $img2 GetNumberOfPoints ] } {
	::pxtclutil::Warning "Unequal Image Sizes\n Cannot Perform Compare $mode t-test!"
	return 0
    }

    if { $mode == "paired" } {
	if { [ $img1 GetNumberOfScalarComponents ] != [ $img2 GetNumberOfScalarComponents ] } {
	    ::pxtclutil::Warning "Unequal Number of Image Frames\n Cannot Perform paired t-test!"
	    return 0
	}
    }
	
    WatchOn

    set tlist [ list $img1 $img2 ]

    if { $mode == "paired" } {
	set imcast2 [ vtkImageCast [ pxvtable::vnewobj ]]
    	$imcast2 SetInput $img2
	$imcast2 SetOutputScalarType [ $img1 GetScalarType ]
	$imcast2 Update
	
	set math [ vtkImageMathematics [ pxvtable::vnewobj ]]
	$math SetInput1 $img1
	$math SetInput2 [ $imcast2 GetOutput ]
	$math SetOperationToSubtract
	$math Update

	set timg [ vtkImageData New ]
	$timg ShallowCopy [ $math GetOutput ]
	$math Delete
	$imcast2 Delete
	set tlist $timg
    }

    set n [ llength $tlist ] 
    #    puts stderr "tlist=$tlist n=$n"
    set math [ vtkpxComputeTmap [ pxvtable::vnewobj ]]
    #    $math DebugOn
    for { set i 0 } { $i < $n } { incr i } {
	
	#	puts stderr "Computing average $i , [ lindex $tlist $i ], [ [ lindex $tlist $i ]  GetDimensions ]"

	set ave($i) [ vtkbisImageAverageComponents New ]
	$ave($i) SetInput [ lindex $tlist $i ]
	$ave($i) Update

	if { $i == 0 } {
	    $math SetInput  [ $ave($i) GetOutput ]
	    $math SetInputStandardDeviation  [ $ave($i) GetStandardDeviationImage ]
	    $math SetNumberOfSamples1 [ $img1 GetNumberOfScalarComponents ]
	} else {
	    $math SetInput2  [ $ave($i) GetOutput ]
	    $math SetInputStandardDeviation2  [ $ave($i) GetStandardDeviationImage ]
	    $math SetNumberOfSamples2 [ $img2 GetNumberOfScalarComponents ]
	}
	$ave($i) Delete
    }
    
    $math SetNumberOfInputs $n
    $math SetScaleFactor 1000.0
    $math Update
    $currentresults ShallowCopyImage [ $math GetOutput ]

    set f(1)  [ [ $fourdimagecontrol1 GetObject ]  cget -filename ]
    set f(2)  [ [ $fourdimagecontrol2 GetObject ]  cget -filename ]

    set outname ""
    for { set i 1 } { $i <= 2 } { incr i } {
	set tnm [ file tail $f($i) ]
	set ext [ file extension $f($i) ]
	set tnm [ file rootname $tnm ]
	if { $ext == ".gz" } {
	    set tnm [ file rootname $tnm ]
	}
	if { $i == 1 } {
	    set outname $tnm
	} else {
	    set outname "${outname}_${tnm}"
	}
    }

    if { $n == 1 } {
	$currentresults configure -filename [ file join [ file dirname $f(1) ] "${outname}_paired_4D_tmap.nii.gz" ]
    } else {
	$currentresults configure -filename [ file join [ file dirname $f(1) ] "${outname}_4D_tmap.nii.gz" ]
    }
		
    puts stderr "out = [ $currentresults cget -filename ]"
    puts stderr "Results Range: [ [ [ [ $currentresults GetImage ] GetPointData ] GetScalars ] GetRange ]"
    $math Delete

    if { $n == 1 } {
	[ lindex $tlist 0 ] Delete
    }
	
    SetFunctionalImage $currentresults
    WatchOff 
    return 1
}

# ----------------------------------------------------------------------------------------


itcl::body pxitcloverlaytool::ComputeIntersection { } {


    set img1 [ [ $meanimagecontrol1 GetObject ] GetImage ]
    set img2 [ [ $meanimagecontrol2 GetObject ] GetImage ]
    $currentresults CopyImageHeader [ [ $meanimagecontrol1 GetObject  ] GetImageHeader ]

    if { [ $img1 GetNumberOfPoints ] != [ $img2 GetNumberOfPoints ] } {
	::pxtclutil::Warning "Unequal Image Sizes\n Cannot Perform Compare Operation $mode!"
	return 0
    }

    WatchOn
    set math [ vtkpxImageThresholdedIntersection [ pxvtable::vnewobj ]]
    $math AddInput $img1
    $math AddInput $img2
    $math SetThreshold  $thisparam($this,intersectionthreshold)
    switch -exact -- $thisparam($this,intersectionmode) {
	"Positive" { $math SetThresholdMode 0}
	"Negative" { $math SetThresholdMode 1}
	"Both"     { $math SetThresholdMode 2}
    }
    SetFilterCallbacks $math "Computing Intersection"    
    $math Update
    $currentresults ShallowCopyImage [ $math GetOutput ]
    $currentresults configure -filename "inter"  
    
    $math Delete
    SetFunctionalImage $currentresults
    WatchOff

    return 1
}


# ----------------------------------------------------------------------------------------
#  Create Grid Visualization
# ----------------------------------------------------------------------------------------
itcl::body pxitcloverlaytool::CreateGridVisualization {  } {

    set current_transform [ $this GetTransformation ]    
    set image_ref [ $currentimage GetImage ]
    set image_trn [ $tmap GetImage ]

    set dim1 [ $image_ref GetDimensions ]
    set dim2 [ $image_trn GetDimensions ]
    if { [lindex $dim2 0 ] <2 || [ lindex $dim1 0 ] < 2 } { 
	::pxtclutil::Warning "No Images in Memory\n Cannot Reslice! ($dim1,$dim2)"
	return  0
    }
    
    WatchOn
    
    scan [ $image_trn GetExtent ]  "%d %d %d %d %d %d" e1 e2 e3 e4 e5 e6
    scan [ $image_trn GetSpacing ] "%f %f %f" s1 s2 s3
    scan [ $image_trn GetOrigin ]  "%f %f %f" o1 o2 o3
    set range [ lindex [[ [ [ $currentimage GetImage ] GetPointData ] GetScalars ] GetRange ] 1]
    
    set grdsource [ vtkImageGridSource [ pxvtable::vnewobj ]]
    $grdsource SetDataExtent  $e1 $e2 $e3 $e4 $e5 $e6
    $grdsource SetDataSpacing $s1 $s2 $s3 
    $grdsource SetDataOrigin  $o1 $o2 $o3 
    $grdsource SetGridSpacing $thisparam($this,gridspacing1) $thisparam($this,gridspacing1) $thisparam($this,gridspacing2) 
    $grdsource SetDataScalarType [ [ $currentimage GetImage ] GetScalarType ]
    $grdsource SetLineValue [ expr $range * $thisparam($this,gridintensity) ]
    $grdsource Update
    
    set blendCreator [ vtkImageBlend [ pxvtable::vnewobj ]]
    $blendCreator AddInput [ $tmap GetImage ]
    $blendCreator AddInput [ $grdsource GetOutput ]
    $blendCreator SetOpacity 1 $thisparam($this,gridopacity)
    SetFilterCallbacks $blendCreator "Blending Grid Images"
    $blendCreator Update
    
    set reslice [  vtkbisImageReslice [ pxvtable::vnewobj ]]
    $reslice OptimizationOff
    $reslice SetInterpolationMode 0
    $reslice SetBackgroundLevel 0.0
    $reslice SetInformationInput $image_ref
    $reslice SetResliceTransform $current_transform
    $reslice SetInput [ $blendCreator GetOutput ]
    SetFilterCallbacks $reslice "Reslicing Grid Source"
    $reslice Update


    ProcessResults  $reslice "resliced-grid" 
    catch { $reslice   Delete }
    catch { $grdSource Delete }
    catch { $blendCreator Delete }
    WatchOff
    return 1
}

itcl::body pxitcloverlaytool::ComputeJacobian { mode } {
    
    set current_transform [ $this GetTransformation ]
    set image_ref [ $currentimage GetImage ]
    set dim1 [ $image_ref GetDimensions ]
    if { [ lindex $dim1 0 ] < 2 } { 
	::pxtclutil::Warning "No Image in Memory\n Cannot Compute Jacobian!"
	return  0
    }
    
    set reg [ vtkpxRegistration [ pxvtable::vnewobj ]]
    WatchOn ; update idletasks

    set thr $thisparam($this,jacobian_threshold)


    set newimg [ vtkImageData [ pxvtable::vnewobj ]]


    set res $thisparam($this,jacobian_resolution) 
    if { $res > 1.0 } {

	if { $res > 10.0 } {
	    set res 10.0
	    set thisparam($this,jacobian_resolution) $res
	} 

	set spa [ $newimg GetSpacing ]
	for { set i 0 } { $i <=2 } { incr i } {
	    set sp($i) [ expr $res*[ lindex $spa $i ] ]
	}

	set resl [ vtkImageResample [ pxvtable::vnewobj ] ]
	$resl SetAxisOutputSpacing 0 $sp(0)
	$resl SetAxisOutputSpacing 1 $sp(1)
	$resl SetAxisOutputSpacing 2 $sp(2)
	$resl InterpolateOff
	$resl SetDimensionality 3
	$resl SetInput $image_ref
	$resl Update
	$newimg ShallowCopy [ $resl GetOutput ]
	$resl Delete
    } else {
	$newimg ShallowCopy $image_ref
	set thisparam($this,jacobian_resolution) 1.0
    }


    if { $mode == "tensor" } {
	$reg ComputeJacobianImage $newimg [ $lastjacobian GetImage ] $current_transform 1 1.0 $thr
    } elseif { $mode == "strains" } {
	$reg ComputePrincipalStrains    $newimg [ $lastjacobian GetImage ] $current_transform $thr
    } else {
	$reg ComputeSimpleJacobianImage $newimg [ $lastjacobian GetImage ] $current_transform 100.0 1.0 $thr
    }


    $lastjacobian CopyImageHeader [ $currentimage GetImageHeader ]
    $newimg Delete
    $reg Delete
    
    UpdateJacobianOverlayRange
    ProcessResultsImage [ $lastjacobian GetImage ] "Jacobian"

    

    WatchOff
}

# ----------------------------------------------------------------

itcl::body pxitcloverlaytool::UpdateJacobianOverlayRange {  } {
    
    set dim1 [ [ $lastjacobian GetImage ] GetDimensions ]
    if { [ lindex $dim1 0 ] < 2 } { 
	return  0
    }

    set nc [ [ $lastjacobian GetImage ] GetNumberOfScalarComponents ]
    $thisparam($this,jackoverlay_componentscale) configure -from 1 -to $nc 

    if { $thisparam($this,jackoverlaycurrentframe) >= $nc } {
	set thisparam($this,jackoverlay_coponentscale) 1
    }

    set range [ [ [ [ $lastjacobian GetImage ] GetPointData] GetScalars] GetRange [ expr $thisparam($this,jackoverlaycurrentframe) -1 ] ]
    set lowrange  [ expr  abs ([ lindex $range 0])]
    set highrange [ expr  abs ([ lindex $range 1])]
    
    puts stdout "Final range = $range "
    
    if { $highrange<$lowrange } {
	set highrange $lowrange
    }
	
    $thisparam($this,jackoverlay_minthresholdscale) configure -from 0 -to $highrange
    $thisparam($this,jackoverlay_maxthresholdscale) configure -from 0 -to $highrange

    if { $highrange > 20 } {
	set thisparam($this,jackoverlaymin) 20
	set thisparam($this,jackoverlaymax) $highrange
    } elseif { $highrange > 200 } {
	set thisparam($this,jackoverlaymin) 50
	set thisparam($this,jackoverlaymax) $highrange
    } else {
	set thisparam($this,jackoverlaymin) [expr $highrange / 2.0 ] 
	set thisparam($this,jackoverlaymax) $highrange
    } 	
    
    
}

itcl::body pxitcloverlaytool::DisplayJacobian { } {

    set dim1 [ $lastjacobian GetImageSize ]
    if {  $dim1 < 2 } { 
	::pxtclutil::Warning "No Jacobian Image in Memory\n Cannot Display!"
	return  0
    }
    
    ProcessResultsImage [ $lastjacobian GetImage ] "Jacobian"
}


itcl::body pxitcloverlaytool::DisplayJacobianOverlay { } {
    
    set image_ref [  $currentimage GetImage ]
    set dim2 [ $image_ref GetDimensions ]
    set dim1 [ [ $lastjacobian GetImage ] GetDimensions ]
    if { [ lindex $dim1 0 ] < 2 || [ lindex $dim2 0 ] < 2 } { 
	::pxtclutil::Warning "No Image or Jacobian in Memory\n Cannot Display Overlay!"
	return  0
    }
    
    WatchOn
    set fmriOverlayCreator [ vtkpxMergeFmriConventional [ pxvtable::vnewobj ]]
    $fmriOverlayCreator SetConventional [ $currentimage GetImage ]
    $fmriOverlayCreator SetFmriInput    [ $lastjacobian GetImage ]
	
    $fmriOverlayCreator SetIntensityThreshold [expr round($thisparam($this,jackoverlayinten))]
    $fmriOverlayCreator SetMinimumThreshold  $thisparam($this,jackoverlaymin)
    $fmriOverlayCreator SetMaximumThreshold  $thisparam($this,jackoverlaymax)
    $fmriOverlayCreator SetOutputMode $thisparam($this,jackoverlaycmapmode)
    $fmriOverlayCreator SetNormalizeAnatomical $thisparam($this,jackoverlayanatcmapmode)
    $fmriOverlayCreator SetSingleFrameMode 0
    
    switch -exact -- $thisparam($this,jackoverlaymode) {
	"Positive" { $fmriOverlayCreator SetThresholdMode 1}
	"Both"     { $fmriOverlayCreator SetThresholdMode 3}
	"Negative" { $fmriOverlayCreator SetThresholdMode 2}
    }
    SetFilterCallbacks $fmriOverlayCreator "Creating Overlays"
    $fmriOverlayCreator Update
    
    ProcessResults  $fmriOverlayCreator "Overlay" 
    $this SetValueImage  [ $fmriOverlayCreator GetFunctionalInput ]
    $fmriOverlayCreator Delete
    WatchOff 

    if { $parent !=0 } {
	if { $thisparam($this,jackoverlaycmapmode) == 0 } {
	    $parent SetLookupTableMode 1 
	} else {
	    $parent SetLookupTableMode 2
	}
    }

    return 1
}

# ----------------------------------------------------------------------------------------
::itcl::body pxitcloverlaytool::AddToMenuButton { mb args} {

    eval "$mb add command -label \"Transformation\" -command {$this ShowWindow Transformation}"
    eval "$mb add command -label \"Visualize Transformation\" -command { $this ShowWindow Jacobian}"
    $mb add separator
    eval "$mb add command -label \"Functional Overlay\" -command { $this ShowWindow \"Func. Overlay\"}"
    $mb add separator
    eval "$mb add command -label \"Image Compare\" -command {$this ShowWindow \"Image Compare\"}"
    eval "$mb add command -label \"4D Image Compare\" -command {$this ShowWindow \"4D Image Compare\"}"
    $mb add separator
    eval "$mb add command -label \"Linear Registration\" -command {$this ShowWindow \"Linear Reg\"}"
    eval "$mb add command -label \"Non-Linear Registration\" -command {$this ShowWindow \"NonLinear Reg\"}"
    if { $enable_distortioncorrection == 1 } {
	eval "$mb add command -label \"Distortion Correction\" -command {$this ShowWindow \"Distortion Cor\"}"
    }
}
    
# ------------------------------------------------------------------------------------------
#  Overlay tool plus is a stripped down version  -- counterintuitive perhaps but that's life
# ------------------------------------------------------------------------------------------

itcl::class pxitcloverlaytoolplus {

    inherit pxitcloverlaytool

    public variable inputimagescallback 0
    public variable itcl_transform 0

    constructor { par args } {
	pxitcloverlaytool::constructor $par $args
    } {
	set compact_reslice_control 0
	set itcl_transform [ [  pxitcltransform \#auto ] GetThisPointer ]
    }

    public  method ResliceFunctionalImage { force display } {
	if { $inputimagescallback != 0 } {
	    eval $inputimagescallback
	}
	return [ ::pxitcloverlaytool::ResliceFunctionalImage $force $display ]
    }

    public method CreateFunctionalOverlayControl { parent } {
	set basewidget $parent
	return [ ::pxitcloverlaytool::CreateFunctionalOverlayControl $parent ]
    }
	
    public method CreateResliceControl { par } {
	set basewidget $par
	return [ ::pxitcloverlaytool::CreateResliceControl $par ]
    }

    public method GetTransformation { } {
	return [ $itcl_transform GetTransformation ]
    }

    public method SetTransformation { newtr args } {
	$itcl_transform CopyTransformation $newtr
    }

    public    method GetTransformationName { } {
	return [ $itcl_transform configure -filename ]
    }

}

