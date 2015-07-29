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

package provide pxitclbrainstrip 1.1

# 	$Id: pxitclbrainstrip.tcl,v 1.1 2005/10/18 21:25:14 xenios Exp xenios $	





package require  labelframe 1.0
package require  pxtclutil  1.0
package require  pxvtable   1.0
package require  pxtclhelp  1.0
#package require  notebook   1.0
package require  pxtclruncommand 1.0
package require vtkpxcontrib  1.1
package require pxitclbaseimagecontrol   1.0
package require pxitclLandmarkControl    1.0
package require pxitclPolyDataControl    1.0

itcl::class pxitclbrainstrip {

    # Base Class
    inherit pxitclbaseimagecontrol 

    protected common thisparam
    protected variable levelsetimagecontrol
    protected variable landmarkcontrol 0
    protected variable polydatacontrol 0
    protected variable curve_extractor 0
    protected variable brain_util      0
    protected variable currentmask 0
    protected variable oldmask 0
    protected variable runcommandgui 0
    protected variable vtk_viewer 0

    public variable enable_fsl            1
    public variable enable_duallevelset   0
    public variable enable_singlelevelset 1
    public variable enable_mousemode      0

    protected variable has_duallevelset   0
    protected variable has_singlelevelset 0
    protected variable has_mousemode      0


    constructor { par } {
	::pxitclbaseimagecontrol::constructor $par
    } {
	InitializeBrainStrip
    }

    # Initialize and Set Up
    public method InitializeBrainStrip { } 
    public method SetPolyDataAndLandmarkControls { landmarkcontrol polydatacontrol }
    public method Initialize { inpwidg vtk_viewer }
    public method DelayedInitialize {  }
    public method DisableLevelSets { } 
    # ----------------
    # Create GUI Stuff
    # ----------------
    public method CreateSimpleControl { parent } 
    public method CreateBETTool       { parent } 
    public method CreateFASTTool       { parent } 
    public method CreateLevelsetControl { parent } 
    public method CreateHistogramSegmentationControl { parent }
    public method CreateBiasFieldCorrectionControl { parent }

    # ------------------------------
    # OOP Override
    # ------------------------------
    public    method SetImage { img } 
    
    # ----------------------------
    # Real Code
    # ----------------------------
    public method Help { } 
    public method ProcessImageResults { mode } 

    public method RunSingleLevelSet { mode } 

    public method UpdateFromLevelSet { args } 

    public method HistogramSegmentation { }
    public method MRFSegmentation { }
    public method DisplayHistogramParameters { }
    public method GenerateMaskFromSegmentedOutput { mode } 
    
    public method RunManualShim {  }
    public method RunSplinePabic { }
    public method RunPolynomialPabic { }
    public method ClearPolynomial { }
    public method PrintPolynomial { { fileid 0} }
    public method LoadPolynomial { }
    public method SavePolynomial { }

    public method RunSliceHomogeneity   { }
    public method RunTripleSliceHomogeneity   { }
    public method RunImageApproximation { }
    public method ApplyB1Field { args }
    public method GrabMorphologyMask { arg1 arg2 }
    public method SetMorphologyMask { arg1 arg2 }
    public method SetMorphologyMaskImage { img }



    # -----------------------------
    # Morphology Operations
    # -----------------------------
    public method CheckImage {  image name operation verbosity } 
    public method ThresholdImage { }
    public method SimpleSegment { }
    public method ErodeMap { } 
    public method DilateMap { } 
    public method DilateErodeMap {  posval minval  } 
    public method BinaryMedianMap {  } 
    public method ConnectMap { mode }
    public method UndoLastMorphologyOperation { }
    public method RequestUpdatedSeed { { force 1  }  } 
    public method ProcessMorphologyResults { filteroutput args } 
    public method DisplayMorphologyResults {  } 


    # -----------------------------
    # BET Operations
    # -----------------------------
    public method EnsureCurrentImageIsSaved { operation }
    public method GenerateMaskFromBETOutput { mode }

    public method RunBET { do }
    public method BETDone { }
    public method FixOriginSpacingOrientation { newimg }

    # -----------------------------
    # FAST Operations
    # -----------------------------
    public method RunFAST { do }
    public method FASTDone { }
    public method FASTStatistics { mode }
    public method FASTNormalizeBrain { }
    

    # ----------------------------
    public method AddToMenuButton { mb args } 
}

# ----------------------------------------------------------------------------------------------
itcl::body pxitclbrainstrip::DisableLevelSets {  } {

    if { $initialized == 1 } { return 0 } 

    set enable_singlelevelset 0
    set enable_duallevelset   0
}


itcl::body pxitclbrainstrip::InitializeBrainStrip {  } {

    set thisparam($this,helpdialog) ""
    set currentmask   [[ pxitclimage \#auto ] GetThisPointer ]
    set oldmask [[ pxitclimage \#auto ] GetThisPointer ]

    set polydatacontrol  0
    set landmarkcontrol  0

    AddImagesToLists  $currentmask $oldmask

    set thisparam($this,currentsurface) 1
    
    # Single Levelset Parameters
    # ------------------------------------------------------------
    set thisparam($this,sn_inmean)  100
    set thisparam($this,sn_outmean) 0
    set thisparam($this,sn_outmean2) 0
    set thisparam($this,sn_insigma)   20
    set thisparam($this,sn_outsigma)   20
    set thisparam($this,sn_outsigma2)   20
    set thisparam($this,sn_biexpoutside) 0
    set thisparam($this,sn_numiter) "200"
    set thisparam($this,sn_updateper) "0.1"
    set thisparam($this,sn_distance) 3.0
    set thisparam($this,sn_advection)   0.0
    set thisparam($this,sn_curvature)   0.2
    set thisparam($this,sn_propagation)  1.0
    set thisparam($this,sn_conv)     0.02
    set thisparam($this,sn_updateparams) 0
    set thisparam($this,sn_beginlevel)   2
    set thisparam($this,sn_endlevel)    1

    # ------------------------------------------------------------
    # Extract Image Smooth and Resample Parameters
    # ------------------------------------------------------------
    set thisparam($this,extr_sigma) 1.0
    set thisparam($this,extr_res)   2.0
    set thisparam($this,extr_do)     0

    # ------------------------------------------------------------
    # Morphology Parameters
    # ------------------------------------------------------------
    set thisparam($this,lowthreshold) 0; set thisparam($this,highthreshold) 100;
    set thisparam($this,seedx)        0; set thisparam($this,seedy)           0; set thisparam($this,seedz) 0
    set thisparam($this,seedautoupdate) 1;
    set thisparam($this,gui_lowthreshold) 0; 
    set thisparam($this,gui_highthreshold) 0; 
    set thisparam($this,gui_morphkernelsize) 3 ; 
    set thisparam($this,gui_displaymask) 1
    set thisparam($this,gui_morphology2d) 0
    # ------------------------------------------------------------
    # BET Parameters
    # ------------------------------------------------------------
    set thisparam($this,bet_fth) 0.3
    set thisparam($this,bet_gth) 0.0
    set thisparam($this,bet_center) 0
    set thisparam($this,bet_radius) -1.0
    set thisparam($this,bet_braingui) 0
    set thisparam($this,bet_overlaygui) 0
    set thisparam($this,bet_outputroot) 0

    # ------------------------------------------------------------
    # FAST Parameters
    # ------------------------------------------------------------
    set thisparam($this,fast_type) "T1-Weighted"
    set thisparam($this,fast_numclasses) "3"
    
    set thisparam($this,fast_segmentedimagegui) 0
    set thisparam($this,fast_outputroot) 0

    set thisparam($this,fslbet)  ""
    set thisparam($this,fslfast) ""


    # ------------------------------------------------------------
    # Histogram Segmentation Parameters
    # ------------------------------------------------------------
    set thisparam($this,histo_numclasses) 3
    set thisparam($this,histo_outlier) 0
    set thisparam($this,histo_premode) "Positive"
    set thisparam($this,histo_distmode) "Gaussian"
    set thisparam($this,histo_outlierprobability) 0.0001
    set thisparam($this,histo_sigmaratio) 0.2
    set thisparam($this,histo_frame) 1
    set thisparam($this,histo_numbins) 256
    set thisparam($this,histo_sigma) 0.05
    set thisparam($this,histo_iterations) 50
    set thisparam($this,histo_epsilon) 0.05
    set thisparam($this,histo_robustscalefactor) 4.0
    set thisparam($this,histo_params) [ vtkFloatArray [ pxvtable::vnewobj ]]
    set thisparam($this,histo_details) "No parameters in memory"

    set thisparam($this,histo_mean1) 1
    set thisparam($this,histo_mean2) 2
    set thisparam($this,histo_mean3) 3
    set thisparam($this,histo_mean4) 4
    set thisparam($this,histo_mean5) 5
    set thisparam($this,histo_mean6) 6
    set thisparam($this,histo_mean7) 7
    set thisparam($this,histo_vsigma) 200
    set thisparam($this,histo_useinitial) 0



    set thisparam($this,mrf_iterations)   20
    set thisparam($this,mrf_smoothness)   1.0
    set thisparam($this,mrf_histonorm)    0
    set thisparam($this,mrf_convergence)  0.25
    set thisparam($this,mrf_fuzzy)        0
    set thisparam($this,mrf_dobiasfield)    0
    set thisparam($this,mrf_biasfieldsigma) 5.0
    set thisparam($this,mrf_singlechannel) 1
    set thisparam($this,mrf_imagenoise)  15

    set thisparam($this,segm_useclass1)   0
    set thisparam($this,segm_useclass2)   1
    set thisparam($this,segm_useclass3)   1
    set thisparam($this,segm_useclass4)   0
    set thisparam($this,segm_useclass5)   0
    set thisparam($this,segm_useclass6)   0
    set thisparam($this,segm_useclass7)   0

    set thisparam($this,segmoutput_gui) 0
    
    # ------------------------------------------------------------
    #   PABIC Bias Field Correction 
    # ------------------------------------------------------------
    set thisparam($this,pabic_maskgui)      0
    set thisparam($this,pabic_restoredgui)  0
    set thisparam($this,pabic_biasfieldgui) 0

    set thisparam($this,pabic_slice_usemask)     0
    set thisparam($this,pabic_slice_robust)      1
    set thisparam($this,pabic_slice_purescaling) 0
    set thisparam($this,pabic_optmethod) "ConjugateGradient"

    set thisparam($this,pabic_usemask)          0
    set thisparam($this,pabic_usefatmask)       1
    set thisparam($this,pabic_slicecorrection)  0
    set thisparam($this,pabic_multiplicative)   1
    set thisparam($this,pabic_sample)           3
    set thisparam($this,pabic_iterations)      10

    set thisparam($this,pabic_spl_metric)       0
    set thisparam($this,pabic_spl_spacing)   30.0
    set thisparam($this,pabic_spl_lambda)     1.0
    set thisparam($this,pabic_spl_presmooth)    8
    set thisparam($this,pabic_spl_stepsize)   5.0
    set thisparam($this,pabic_spl_fatmaskweight) 2


    # ---------------- Shim Algorithm ---------------
    # ----------------------------------------------
    set thisparam($this,pabic_shim_degree)   2
    set thisparam($this,pabic_shim_useinitial) 0
    set thisparam($this,pabic_shim_updatesegm) 0
    set thisparam($this,pabic_shim_metric) 0
    # Hidden Variables (advanced)
    for { set i 0 } { $i <=18 } { incr i } {
	set thisparam($this,pabic_shimval_$i) 0.0
    }

    set thisparam($this,pabic_shim_stepsize)   0.05
    set thisparam($this,pabic_shim_numsteps)   1
    set thisparam($this,pabic_shim_numlevels)  1
    set thisparam($this,pabic_shim_resolution) 3
    set thisparam($this,pabic_shim_iterations) 10
    set thisparam($this,pabic_shim_usemask)    1
    set thisparam($this,pabic_min) 0.2
    set thisparam($this,pabic_max) 5.0
    
    # ------------------------------------------------------------

    global tcl_platform

    set gm 0
    set g [ vtkpxTissueLevelSetFilter  [ pxvtable::vnewobj ]]    
    set gm 1
    $g Delete


    if { $gm == 0 } {
	set enable_singlelevelset 0
    }

    set enable_duallevelset 0
    set brain_util 0

    if { $enable_singlelevelset == 1 } {
	set has_singlelevelset 1
    }
}


# -------------------------------------------------------------------------------

itcl::body pxitclbrainstrip::SetPolyDataAndLandmarkControls { landmarkc polydatac } {

    set polydatacontrol  $landmarkc
    set landmarkcontrol $polydatac
}


# -------------------------------------------------------------------------------

itcl::body pxitclbrainstrip::Initialize { inpwidg viewer } {

    global tcl_platform

    if { $initialized == 1 } { return $basewidget } 

    if { $enable_fsl == 1 } {
	
	if { $tcl_platform(platform) == "windows"} {
	    if {  [ ::pxtclutil::FindCommandInPath bet.exe ] !=0  } {
		set thisparam($this,fslbet)  "bet.exe"
		set thisparam($this,fslfast) ""
	    } elseif {  [ ::pxtclutil::FindCommandInPath bet2.exe ] !=0  } {
		set thisparam($this,fslbet)  "bet2.exe"
		set thisparam($this,fslfast) "bash -l -e fast"
	    } elseif {  [ ::pxtclutil::FindCommandInPath bet2.exe ] !=0  } {
		set thisparam($this,fslbet)  "bet.exe"
		set thisparam($this,fslfast) "bash -l -e fast"
	    }
	} else {
	    if { ( [ ::pxtclutil::FindCommandInPath fast_bis ] !=0 ) &&
		 ( [ ::pxtclutil::FindCommandInPath bet2_bis ] !=0 ) } {
		set thisparam($this,fslbet)  bet2_bis
		set thisparam($this,fslfast) fast_bis
	    } elseif { ( [ ::pxtclutil::FindCommandInPath fast ] !=0 ) &&
		       ( [ ::pxtclutil::FindCommandInPath bet2 ] !=0 ) } {
		set thisparam($this,fslbet)  bet2
		set thisparam($this,fslfast) fast
	    } elseif { ( [ ::pxtclutil::FindCommandInPath fast ] !=0 ) &&
		       ( [ ::pxtclutil::FindCommandInPath bet ] !=0 ) } {
		set thisparam($this,fslbet) bet
		set thisparam($this,fslfast) fast
	    }
	}
    }

    #    puts stderr "BET = $thisparam($this,fslbet), FAST = $thisparam($this,fslfast)"

    set vtk_viewer $viewer
    set basewidget $inpwidg
    toplevel $basewidget ; wm geometry $basewidget 700x550; wm withdraw $basewidget


    SetTitle "Segmentation Control"
    set initialized 1

    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget
}

itcl::body pxitclbrainstrip::DelayedInitialize {  } {

    global pxtcl_pref_array

    if { $fullyinitialized == 1 } {
	return
    }

    set notebook $basewidget.notebook
    iwidgets::tabnotebook $notebook -width 5i -height 3i -tabpos w

    CreateSimpleControl        [ $notebook add -label "Math_Morphology" ]
    CreateHistogramSegmentationControl        [ $notebook add -label "Histogram Segmentation" ]
    CreateBiasFieldCorrectionControl          [ $notebook add -label "Bias Field Correction" ]

    if {  $thisparam($this,fslbet) !=  "" || $thisparam($this,fslfast) != "" } {
	set  runcommandgui [  [ pxtclruncommand \#auto ] GetThisPointer ]
	$runcommandgui Initialize $basewidget.runcommandgui 0 0
	$runcommandgui Hide
	if { $thisparam($this,fslbet) !=  "" } {
	    CreateBETTool              [ $notebook add -label "Brain Extraction Tool"    ]
	} 
	if { $thisparam($this,fslfast) !=  "" } {
	    CreateFASTTool              [ $notebook add -label "Gray/White Segmentation" ]
	}
    }

    if { $enable_singlelevelset  == 1 } {
	CreateLevelsetControl [ $notebook add -label "Levelset Segmentation" ]
    }
	

    set canvas [ [ $notebook component tabset ] component canvas ];     set l [ AddLogoLabel $canvas ];     pack $l -side bottom -padx 1 
    set w  [ frame $basewidget.buttonbar  ]
    pack $w -side bottom -fill x -expand f
    pack $notebook -side top -fill both -expand true

    if { $parent != 0 } {
	eval "button $w.l1  -text \"Copy Results To Image\" -command { $parent CopyResultsToImage }"
	eval "button $w.l2  -text \"Undo Copy Results\" -command { $parent RevertToPreviousImage }"
	pack $w.l1 $w.l2 -side left -padx 5
    }

    eval "button $w.close   -text Close -command { $this HideWindow } "
    pack  $w.close -side right -fill x -padx 5 -pady 5 -expand f
    set fullyinitialized 1
}

# ------------------------------------------------------------------------------------------------
#    Create GUI Stuff 
# ------------------------------------------------------------------------------------------------
itcl::body pxitclbrainstrip::CreateLevelsetControl { par } {
    
    set levelsetimagecontrol [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $levelsetimagecontrol configure -description "Level Set Image"
    $levelsetimagecontrol InitializeLite $par.lvlimg 2
    $levelsetimagecontrol AddFunction "$parent SetResultsFromObject" "Display" "$this"
    pack $par.lvlimg -side bottom -expand false -fill x -pady 5

    label $par.o -bg black -fg white -text "Using Current Image And Current Landmark Collection."
    set w [ frame $par.o3 ]
    pack $par.o  $par.o3 -side top -expand f -fill x -padx 1 -pady 0
    
    label $w.lab  -text "To Generate Seeds Use:" -bg darkgrey
    eval "button $w.land  -text \"Landmark Control\" -command { $landmarkcontrol Show 1}"
    pack $w.lab $w.land -side left -fill x -padx 2 -expand t -pady 2
    
    set w [LabelFrame:create $par.t -text "Tissue Description" -expand false -fill x ]
    set w2 [LabelFrame:create $par.ta -text "Levelset Weights" -expand false -fill x ]
    set w3 [LabelFrame:create $par.tb -text "Optimization Parameters" -expand false -fill x ]
    set w4 [LabelFrame:create $par.sur -text "Output Surface Destination" -expand false -fill x ]

    set butbar [ frame $par.but ]

    # ------------------------- Tissue description -------------------------

    pack $par.t $par.ta $par.tb $par.sur $butbar -side top -expand f -fill x -padx 2 -pady 1

    frame $w.t; frame $w.t2 ; frame $w.t25; frame $w.t3

    pack $w.t $w.t2 $w.t25 $w.t3 -side top -expand false -fill x

    iwidgets::entryfield $w.t.1 -labeltext "Inside  Mean/Sigma:" -textvariable [ itcl::scope thisparam($this,sn_inmean) ] -relief sunken -width 6 -validate real 
    iwidgets::entryfield $w.t.2 -labeltext "" -textvariable [ itcl::scope thisparam($this,sn_insigma) ] -relief sunken -width 6 -validate real

    pack $w.t.1 $w.t.2  -side left -expand f -padx 2 -fill x
    
    iwidgets::entryfield $w.t2.1 -labeltext "Outside Mean/Sigma:" -textvariable [ itcl::scope thisparam($this,sn_outmean) ] -relief sunken -width 6 -validate real
    iwidgets::entryfield $w.t2.2 -labeltext "" -textvariable [ itcl::scope thisparam($this,sn_outsigma) ] -relief sunken -width 6 -validate real

    pack $w.t2.1  $w.t2.2 -side left -expand f -padx 2 -fill x

    
    iwidgets::entryfield $w.t2.11 -labeltext "Outside (2) Mean/Sigma:" -textvariable [ itcl::scope thisparam($this,sn_outmean2) ] -relief sunken -width 6 -validate real
    iwidgets::entryfield $w.t2.12 -labeltext "" -textvariable [ itcl::scope thisparam($this,sn_outsigma2) ] -relief sunken -width 6 -validate real
    pack $w.t2.11 $w.t2.12 -side left -expand f -padx 2 -fill x

    checkbutton $w.t3.1 -text "Bi-exponential Outside" -variable [ itcl::scope thisparam($this,sn_biexpoutside) ]
    checkbutton $w.t3.2 -variable [ itcl::scope thisparam($this,sn_updateparams) ] -text "Dynamicly Update Parameters"
    pack $w.t3.1 $w.t3.2  -side left -expand f -padx 2 -fill x


    # ------------------------------------------------------------------------- ----------------------
    # Optimization
    # ------------------------------------------------------------------------------------------------

    set top [ frame $w3.1 ];     set bot [ frame $w3.2 ]; pack $top $bot -side top -expand f -fill x

    label $top.1 -text "Levels:"
    tk_optionMenu $top.2  [ itcl::scope thisparam($this,sn_beginlevel) ]  1 2 3 4
    tk_optionMenu $top.2a [ itcl::scope thisparam($this,sn_endlevel) ]  1 2 3 4

    label $top.3 -text "Initial Radius:"
    tk_optionMenu $top.4 [ itcl::scope thisparam($this,sn_distance) ]  "1.0" "2.0" "3.0" "4.0" "5.0" "8.0" "12.0" "15.0" "20.0"
    pack  $top.1 $top.2 $top.2a $top.3 $top.4 -side left -padx 2 -expand f -fill x
  
    label $bot.1 -text "Iterations:"
    tk_optionMenu $bot.2 [ itcl::scope thisparam($this,sn_numiter) ]  "2" "20" "50" "100" "200" "400" "800" "1000" "2000"
    label $bot.3 -text "Error:"
    tk_optionMenu $bot.4 [ itcl::scope thisparam($this,sn_conv) ]  "0.0" "0.01" "0.1" "0.02" "0.05" "0.1"
    label $bot.5 -text "Upd::"
    tk_optionMenu $bot.6 [ itcl::scope thisparam($this,sn_updateper) ]  "0.05" "0.1" "0.2" "0.33" "0.5" "1.01"
    pack $bot.1 $bot.2 $bot.3 $bot.4 $bot.5 $bot.6 -side left -expand f -padx 1 -fill x

    # ------------------------------------------------------------------------- ----------------------
    # Levelset Weights
    # ------------------------------------------------------------------------------------------------
    label $w2.1 -text "Propagtion"
    tk_optionMenu $w2.2 [ itcl::scope thisparam($this,sn_propagation) ] "0.0" "0.01" "0.02" "0.05" "0.1" "0.25" "0.5" "1.0" "2.0" 
    label $w2.3 -text "Smoothness"
    tk_optionMenu $w2.4 [ itcl::scope thisparam($this,sn_curvature) ] "0.0" "0.01" "0.02" "0.05" "0.1" "0.25" "0.5" "1.0" "2.0" 
    label $w2.5 -text "Advection"
    tk_optionMenu $w2.6 [ itcl::scope thisparam($this,sn_advection) ] "0.0" "0.01" "0.02" "0.05" "0.1" "0.25" "0.5" "1.0" "2.0" 
    pack $w2.1 $w2.2 $w2.3 $w2.4 $w2.5 $w2.6 -side left -expand f -padx 5 -fill x


    label $w4.a -text "Outer Surface: "
    set menu1 [ tk_optionMenu $w4.b [ itcl::scope thisparam($this,currentsurface) ] 1  ]

    set num 2; catch { set num [ $polydatacontrol  GetNumberOfSurfaces ]  }
    for { set k 2 } { $k <= $num } { incr k } {
	$menu1 add radio -label "$k" -variable [ itcl::scope thisparam($this,currentsurface) ]
    }
    
    eval "button $w4.sc  -text \"Show Surface Control\" -command { $polydatacontrol Show 1}"
    pack $w4.sc -side right -padx 2
    pack $w4.a $w4.b   -side left -fill x -padx 10    
    
    eval "button $butbar.1 -text \"Run Single Levelset\" -command { $this RunSingleLevelSet 0 }"
    eval "button $butbar.2 -text \"Continue Current\" -command { $this RunSingleLevelSet 1 }"
    eval "button $butbar.3 -text \"Fast March\" -command { $this RunSingleLevelSet -1 }"
    pack $butbar.1 $butbar.2 $butbar.3 -side left -expand t -padx 10 -fill x -pady 10



}
# --------------------------------------------------------
#    Help
# --------------------------------------------------------

itcl::body pxitclbrainstrip::Help { } {

    if { [ string length $thisparam($this,helpdialog) ] > 2 } { 
	::pxtclutil::ShowWindow $thisparam($this,helpdialog)
	return
    }
    
    set w .
    if { [ string length $basewidget ] > 2 } { set w $basewidget }
    
    set thisparam($this,helpdialog)  [ toplevel $w.helpdialog ]
    
    ::pxtclhelp::HelpDialog $thisparam($this,helpdialog)
    wm title $thisparam($this,helpdialog) "Help on Extracting Curves"
    
    set line "To extract sulcal curves the following are needed:\n\n"
    set line "$line \t 1. The outer brain surface with curvature information.\n"
    set line "$line \t 2. Two initial points at either end of the curve\n\n"
    set line "$line \t The outer brain surface is obtained from the surface control, and it is the surface specified by the \"outer brain surface\" option menu in the surfaces pane. The two initial points are the last two points of the current point set in the landmark control.\n\n"
    set line "$line \t The final output is stored in the clipboard of the surface control. To see/access the curve, first select an unused surface in the surface control and then click on Edit/Paste on the surface control menu.\n\n"
    ::pxtclhelp::HelpDialogAddText $thisparam($this,helpdialog) $line
    
    
    set line "To extract sulcal ribbons the following are needed:\n\n"
    set line "$line \t 1. The outer   brain surface with curvature information.\n"
    set line "$line \t 2. The wrapper brain surface with curvature information.\n"
    set line "$line \t3. Two initial points at either end of the curve\n"
    set line "$line \t4. The levelset outer distance function as produced by brainstrip\n\n"
    set line "$line \tThe outer brain surface and the outer wrapper surface are obtained from the surface control, and are the surfaces specified by the \"outer brain surface\" and \"wrapper brain surface\" option menus respectively, in the surfaces pane. The two initial points are the last two points of the current point set in the landmark control. The levelset outer distance function must be loaded using the \"Load\" button in the \"Levelset Distance Image\" pane.\n\n"
    set line "$line \tThe final output is stored in the clipboard of the surface control. To see/access the ribbon, first select an unused surface in the surface control and then click on Edit/Paste on the surface control menu.\n\n"
    ::pxtclhelp::HelpDialogAddText $thisparam($this,helpdialog) $line
    
    $thisparam($this,helpdialog) configure -width 650	
    ::pxtclutil::ShowWindow $thisparam($this,helpdialog)
}

# --------------------------------------------------------
#   Generate Output Images 
# --------------------------------------------------------
itcl::body pxitclbrainstrip::ProcessImageResults { mode } {
    
    $currentresults configure -filename    [ AddPrefix [ $currentimage cget -filename  ] $mode ]
    $currentresults CopyImageHeader [ $currentimage GetImageHeader ]
    $parent SetResultsFromObject $currentresults $this
}


# ----------------------------------------------------------------------------
# Single Levelset Options
# ----------------------------------------------------------------------------
itcl::body pxitclbrainstrip::UpdateFromLevelSet { args } {

    set current_levelset [ lindex $args 0 ]
            
    pxtkprint "[ $current_levelset GetOutputStatus ]\n"

    
    set pd [ vtkImageData New ] 
    $pd ShallowCopy [  $current_levelset GetOutput ] 

    set skinExtractor [ vtkContourFilter [ pxvtable::vnewobj ] ]
    $skinExtractor SetUseScalarTree 0
    $skinExtractor ComputeScalarsOff
    $skinExtractor ComputeGradientsOff
    $skinExtractor SetInput $pd
    $skinExtractor SetValue 0 0.0
    $skinExtractor Update
    $pd Delete
    

    set surnum [ expr $thisparam($this,currentsurface) - 1 ]
    $polydatacontrol  SetSurface $surnum [ $skinExtractor GetOutput ] "levelset"

    $vtk_viewer UpdateDisplay
    $skinExtractor Delete

    $this ProgressCallback  $current_levelset "Running levelset" 
    update idletasks

}
# ----------------------------------------------------------------------------
itcl::body pxitclbrainstrip::RunSingleLevelSet { mode } {

    if {  [$currentimage GetImageSize ]  < 2 } {
	::pxtclutil::Warning "No Image In Memory! Cannot Segment!"
	return 0 
    }
    
    set nm "out" 

    set usepoints 1

    if { $mode == 1 } {

	set outimage [ $levelsetimagecontrol GetObject ]
	set dim1 [ $outimage      GetImageSize ]
	set dim2 [ $currentimage  GetImageSize ]
	if { $dim1 == $dim2 } {
	    set usepoints 0
	}
    }

    if { $usepoints == 1 } {
	set temp [ [ $landmarkcontrol GetCollection -1 ] GetNumPoints ] 
	if { $temp< 1 } {
	    ::pxtclutil::Warning "Current point set on landmark control must have at least 1 seed point to initialize the segmentation!"
	    return 0
	}
    }
    
    set baseCurve [ vtkpxBaseCurve [ pxvtable::vnewobj ]]
    $baseCurve Copy [ $landmarkcontrol GetCollection -1 ] 
    $baseCurve SetSpacing 1.0 1.0 1.0
    $baseCurve SetOrigin 0.0 0.0 0.0
    $baseCurve Compact

    set tmp [ vtkImageData [ pxvtable::vnewobj ]]

    WatchOn 
    after 100
    update idletasks

    set outimage [ $levelsetimagecontrol GetObject ]

    if { $mode == -1 } {

	set lv [ vtkpxLevelSetUtil [ pxvtable::vnewobj ]]
	set img [ $lv GenerateInputLevelSet [ $currentimage GetImage ] [ $baseCurve GetPoints ] $thisparam($this,sn_distance)  1.0 ]
	$outimage ShallowCopyImage $img
	$img Delete
    } else {
	set g [ vtkpxTissueLevelSetFilter  [ pxvtable::vnewobj ]]
	$g SetInput [ $currentimage GetImage ]
	if { $usepoints == 1 } {
	    $g SetInitialPoints [ $baseCurve GetPoints ]
	    $g SetInitialDistance  $thisparam($this,sn_distance) 
	} else {
	    $tmp ShallowCopy [ $outimage GetImage ]
	    $g SetInitialLevelSet $tmp
	}
	
	$g SetBeginLevel $thisparam($this,sn_beginlevel)
	$g SetEndLevel $thisparam($this,sn_endlevel)
	$g SetOutsideMean $thisparam($this,sn_outmean)
	$g SetOutsideSigma $thisparam($this,sn_outsigma)
	$g SetInsideMean $thisparam($this,sn_inmean)
	$g SetInsideSigma $thisparam($this,sn_insigma)
	$g SetBiExponentialOutside $thisparam($this,sn_biexpoutside)
	$g SetOutside2Mean $thisparam($this,sn_outmean2)
	$g SetOutside2Sigma $thisparam($this,sn_outsigma2)
	
	$g SetLevelsetIterations $thisparam($this,sn_numiter)
	$g SetDynamicParameterUpdate $thisparam($this,sn_updateparams)
	$g SetIterationSave 0
	$g SetRMSError $thisparam($this,sn_conv)
	$g SetAdvectionWeight $thisparam($this,sn_advection)
	$g SetCurvatureWeight  $thisparam($this,sn_curvature)
	$g SetPropagationWeight $thisparam($this,sn_propagation)
	$g SetUpdatePercentage $thisparam($this,sn_updateper)
	
	if { $thisparam($this,sn_numiter) < 100 } {
	    if { $thisparam($this,sn_updateper) < 0.5 } {
		$g SetUpdatePercentage 0.5
	    }
	}
	
	pxtkconsole
	update idletasks
	pxtkprint [ $g GetOutputStatus ]
	eval "$g AddObserver ProgressEvent { $this UpdateFromLevelSet $g }"
	eval "$g AddObserver EndEvent      { $this UpdateFromLevelSet $g }"
	$g Update



	$outimage ShallowCopyImage  [ $g GetOutput ] 
	$g Delete
    }  
    
    $outimage CopyImageHeader [ $currentimage GetImageHeader ]
    $levelsetimagecontrol Update
    $baseCurve Delete
    
    $tmp Delete

    if { $mode == - 1 } {
	$currentresults ShallowCopyImage $outimage
	ProcessImageResults  "Levelset" 
    }
    WatchOff
}

# ----------------------------------------------------------------------------

# ----------------------------------------------------------------------------
itcl::body pxitclbrainstrip::CreateBETTool { base } {

    set a $thisparam($this,fslbet) 
    label $base.o1 -bg black -fg white -text "Brain Extraction Tool  (FSL)"
    label $base.o2 -bg black -fg white -text "From FMRIB Analysis Group, Oxford (cmdline=$a)"
    label $base.o3 -bg black -fg white -text "(This calls the FSL $a program as an external process)"

    pack $base.o1 $base.o2 $base.o3  -side top -fill x -expand f -padx 1 -pady 0

#    set   thresholdbar [ LabelFrame:create $base.bar2  -width 300  -text "Parameters" ]
    set   thresholdbar [ frame $base.bar2  ]
    set   midbar [ frame $base.bar22 -bg black -height 3  ]
    set   outputbar    [ frame $base.bar25 ]

    pack  $base.bar25 $midbar  -side bottom -expand t -fill x  -pady 5 -padx 2
    pack  $base.bar2   -side top -expand t -fill both  -pady 5 -padx 2

    frame $thresholdbar.t ; frame $thresholdbar.b ;
    pack $thresholdbar.t $thresholdbar.b -side top -expand f -fill x
    ::pxtclutil::CreateTriple $thresholdbar.t.y  "Fractional Intensity Threshold (Smaller value=Larger Brain)" [ itcl::scope thisparam($this,bet_fth) ] 0.0 0.99 0.05 0.5 170 
    ::pxtclutil::CreateTriple $thresholdbar.b.y  "Gradient Threshold (+ve =Large Brain at Bottom, Small at Top)" [ itcl::scope thisparam($this,bet_gth) ] -0.99 0.99 0.05 0.0 170 
    frame $thresholdbar.z; frame $thresholdbar.h
    pack $thresholdbar.z $thresholdbar.h  -side top -expand f -fill x
    checkbutton   $thresholdbar.z.1 -variable [ itcl::scope thisparam($this,bet_center) ] -text "Use Current Cross-Hairs"
    label         $thresholdbar.z.2  -text "Initial Radius:"
    entry         $thresholdbar.z.3  -textvariable [ itcl::scope thisparam($this,bet_radius) ] -relief sunken -width 5
    pack $thresholdbar.z.1 $thresholdbar.z.2 $thresholdbar.z.3 -side left -expand f -fill x

    eval "button  $thresholdbar.h.1  -text \"Execute Brain Extraction Tool\" -command { $this RunBET 1 }"    
    eval "button  $thresholdbar.h.2  -text \"Show Console\" -command { $this RunBET 0 }"    
    pack $thresholdbar.h.1 $thresholdbar.h.2 -side left -padx 2 -pady 5




    
    set thisparam($this,bet_braingui) [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $thisparam($this,bet_braingui) configure -description "Stripped Brain"
    $thisparam($this,bet_braingui) InitializeLite $outputbar.1 2
    $thisparam($this,bet_braingui) AddFunction "$parent SetResultsFromObject" "Display" "$this"

    set thisparam($this,bet_overlaygui) [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $thisparam($this,bet_overlaygui) configure -description "Brain/Skull Overlay"
    $thisparam($this,bet_overlaygui) InitializeLite $outputbar.2 2
    $thisparam($this,bet_overlaygui) AddFunction "$parent SetResultsFromObject" "Display" "$this"

    pack $outputbar.1 $outputbar.2 -side top -pady 0 -ipady 0

    ::pxitclbasecontrol::SetWidgetTreeColors $thresholdbar  "\#ffdddd" "black"
}
# ----------------------------------------------------------------------------
itcl::body pxitclbrainstrip::CreateFASTTool { base } {

    set a $thisparam($this,fslfast) 
    label $base.o1 -bg black -fg white -text "Automated Segmentation Tool (FSL)"
    label $base.o2 -bg black -fg white -text "From FMRIB Analysis Group, Oxford (cmdline=$a)"
    label $base.o3 -bg black -fg white -text "(This calls the FSL fast program as an external process)"

    
    pack $base.o1 $base.o2 $base.o3  -side top -fill x -expand f -padx 2 -pady 0

    set   thresholdbar [ frame $base.bar2  ]
    set   midbar [ frame $base.bar22 -bg black -height 3  ]
    set   outputbar    [ frame $base.bar25 ]

    pack $outputbar $midbar -side bottom -expand f -fill x -pady 2
    pack  $base.bar2   -side top -expand t -fill both  -pady 10 -padx 2

    frame $thresholdbar.z; frame $thresholdbar.h
    pack $thresholdbar.z $thresholdbar.h  -side top -expand f -fill x

    label         $thresholdbar.z.1  -text "Input Image Type:"
    tk_optionMenu $thresholdbar.z.2 [ itcl::scope thisparam($this,fast_type) ]  "T1-Weighted" "T2-Weighted" "PD-Weighted"
    label         $thresholdbar.z.3  -text "Number Of Classes:"
    tk_optionMenu $thresholdbar.z.4 [ itcl::scope thisparam($this,fast_numclasses) ] "2" "3" "4" "5" "6"

    pack $thresholdbar.z.1 $thresholdbar.z.2 $thresholdbar.z.3 $thresholdbar.z.4 -side left -expand f -fill x

    eval "button  $thresholdbar.h.1  -text \"Execute Automated Segmentation Tool\" -command { $this RunFAST 1 }"    
    eval "button  $thresholdbar.h.2  -text \"Show Console\" -command { $this RunFAST 0 }"    
    pack $thresholdbar.h.1 $thresholdbar.h.2 -side left -padx 2 -pady 5


    ::pxitclbasecontrol::SetWidgetTreeColors $thresholdbar  "\#ffdddd" "black"
    
    set thisparam($this,fast_segmentedimagegui) [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $thisparam($this,fast_segmentedimagegui) configure -description "Classification Map"
    $thisparam($this,fast_segmentedimagegui) InitializeLite $outputbar.1 2
    $thisparam($this,fast_segmentedimagegui) AddFunction "$parent SetResultsFromObject" "Display" "$this"

    pack $outputbar.1  -side top -pady 0 -ipady 0

    set w [LabelFrame:create $outputbar.3 -text "Post-process Output" -expand false -fill x ]
    pack $outputbar.3 -side bottom -expand f -fil x

    eval "button  $w.1  -text \"Statistics\" -command { $this FASTStatistics cleanup }"    
#    eval "button  $w.2  -text \"Normalize Brain Intensity\" -command { $this FASTNormalizeBrain }"    
    pack $w.1  -side left -padx 2 -pady 5
    

}
# ----------------------------------------------------------------------------
itcl::body pxitclbrainstrip::CreateSimpleControl { base } {

    set   thresholdbar [ LabelFrame:create $base.bar2  -width 300  -text "Thresholds (to create mask)" ]
    set   morphbar     [ LabelFrame:create $base.bar3  -width 300  -text "Morphology (to manipulate mask)" ]
    pack  $base.bar2 $base.bar3 -side top -expand t -fill both  -pady 2


    frame $thresholdbar.t ; frame $thresholdbar.b ; frame $thresholdbar.b2

    pack $thresholdbar.t $thresholdbar.b $thresholdbar.b2 -side top -expand f -fill x
    set thisparam($this,gui_lowthreshold)  [  ::pxtclutil::CreateTriple $thresholdbar.t.y  "Low Threshold" [ itcl::scope thisparam($this,lowthreshold) ] 0.0 1000.0 0.1 10 110 ]
    set thisparam($this,gui_highthreshold) [  ::pxtclutil::CreateTriple $thresholdbar.t.z "High Threshold" [ itcl::scope thisparam($this,highthreshold) ]  0.0 1000.0 0.11 10 110 ]
    eval "button  $thresholdbar.b.h  -text Threshold -command { $this ThresholdImage }"
    eval "button  $thresholdbar.b.h2  -text \"Simple-Segment\" -command { $this SimpleSegment }"
    eval "button  $thresholdbar.b.h3  -text \"Display\" -command { $this DisplayMorphologyResults }"
    checkbutton $thresholdbar.b.l -variable [ itcl::scope thisparam($this,gui_displaymask) ] -text "Display Mask"
    pack $thresholdbar.b.l -side left -padx 2
    pack $thresholdbar.b.h3 $thresholdbar.b.h2 $thresholdbar.b.h -side right -padx 2


    if { $thisparam($this,fslbet) != "" } {
	eval "button  $thresholdbar.b2.h22  -text \"Generate Mask from Brain Extract Tool Output\" -command { $this GenerateMaskFromBETOutput 1 }"
	pack  $thresholdbar.b2.h22 -side right -padx 2
    }

    frame $morphbar.1; frame $morphbar.2;  frame $morphbar.4 ; frame $morphbar.35; frame $morphbar.32 -height 2 -bg black

    eval "button $morphbar.5 -text \"Undo Last Operation\"   -command { $this UndoLastMorphologyOperation }"
    pack $morphbar.5  -side bottom -expand f -padx 20 -pady 5
    pack $morphbar.1 $morphbar.2 $morphbar.32 $morphbar.35 $morphbar.4 -side top -fill x
    
    label         $morphbar.1.1  -text "Kernel Size:"
    tk_optionMenu $morphbar.1.2  [ itcl::scope thisparam($this,gui_morphkernelsize) ]  "1" "3" "5" "7"
    checkbutton   $morphbar.1.3 -variable [ itcl::scope thisparam($this,gui_morphology2d) ] -text "2D"
    eval          "button $morphbar.1.4 -text \"BMedian\"  -command { $this BinaryMedianMap  }"
    pack          $morphbar.1.1 $morphbar.1.2 $morphbar.1.3 $morphbar.1.4 -side left -expand t -padx 3 -fill x 
    
    eval "button $morphbar.2.3 -text Erode -command { $this ErodeMap  }"
    eval "button $morphbar.2.4 -text Dilate      -command { $this DilateMap }"
    pack          $morphbar.2.3 $morphbar.2.4 -side left -expand t -padx 3 -fill x 
    
    set b $morphbar.35
    label  $b.1  -text "Seed Location:"
    iwidgets::entryfield $b.2 -labeltext "x" -textvariable [ itcl::scope thisparam($this,seedx) ] -relief sunken -width 4 -validate integer
    iwidgets::entryfield $b.3 -labeltext "y" -textvariable [ itcl::scope thisparam($this,seedy) ] -relief sunken -width 4 -validate integer
    iwidgets::entryfield $b.4 -labeltext "z" -textvariable [ itcl::scope thisparam($this,seedz) ] -relief sunken -width 4 -validate integer
    checkbutton   $b.5 -variable [ itcl::scope thisparam($this,seedautoupdate) ] -text "Auto Update"
    eval "button $b.6 -text \"Update\"  -command { $this RequestUpdatedSeed 1 }"
    pack $b.1 $b.2 $b.3 $b.4 $b.5 $b.6 -side left -expand t -fill x


    set b   $morphbar.4 


    eval "button $b.41 -text \"Connect Foreground\"  -command { $this ConnectMap 1 }"
    eval "button $b.42 -text \"Connect Background\"  -command { $this ConnectMap 0 }"
    pack  $b.41 $b.42 -side left -fill x -expand t -pady 0 -padx 1

}

# ----------------------------------------------------------------------------
#  Histogram Segmentation Control
# ----------------------------------------------------------------------------
itcl::body pxitclbrainstrip::CreateHistogramSegmentationControl { base } {

    global pxtcl_pref_array
    
    entry $base.out -textvariable [ itcl::scope thisparam($this,histo_details)] -relief raised -state disabled -width 40 -background black -foreground white
    
    set   origtopbar  [ LabelFrame:create $base.bar1   -text "Histogram Segmentation (K-Means Clustering)" ]
    set   mrfbar      [ LabelFrame:create $base.bar2   -text "Region Segmentation (extends Histogram Segmentation)" ]
    set   maskbar     [  LabelFrame:create $base.bar3   -text "Create Mask" ]
    pack  $base.bar3 -side bottom -expand f -fill x -pady 0
    pack  $base.out $base.bar1 $base.bar2  -side top -expand false -fill x -pady 0


    # ----------------------------------------------------------------------------
    # Output Fields
    # ----------------------------------------------------------------------------

    set thisparam($this,segmoutput_gui) [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $thisparam($this,segmoutput_gui) configure -description "Segmentation Output"
    $thisparam($this,segmoutput_gui) InitializeLite $maskbar.top 2
    $thisparam($this,segmoutput_gui) AddFunction "$parent SetResultsFromObject" "Display" "$this"
    pack $maskbar.top -side top -expand false -fill x
    frame $maskbar.bot;     pack $maskbar.bot -side bottom -expand false -fill x

    label $maskbar.bot.0 -text "Use Class:"
    pack $maskbar.bot.0 -side left -padx 2

    for { set i 1 } { $i <= 7 } { incr i } {
	set j [ expr $i -1 ]
	eval "checkbutton  $maskbar.bot.$i -variable [ itcl::scope thisparam($this,segm_useclass$i) ] -text $j"
	pack $maskbar.bot.$i -side left -padx 1
    }
    eval "button $maskbar.bot.8 -text \"Generate\"  -command { $this GenerateMaskFromSegmentedOutput 1 }"

    pack $maskbar.bot.8 -side right -padx 2

    # --------------------------------------------------------------------------
    # Histogram Segmentation
    # --------------------------------------------------------------------------

    set bbar [  frame $origtopbar.hb ]
    pack $bbar -side bottom -expand f -fill x

    set nt [ iwidgets::tabnotebook $origtopbar.nt -tabpos s -height 125 -angle 45 -margin 2 -backdrop gray -raiseselect true ]
    pack $nt -side top -expand f  -fill x

    set topbar  [ $nt add -label Standard   ]
    set topbar2 [ $nt add -label Experimental ]
    set advanced $topbar2
    $nt view "Standard"

    set mainbar  [ frame $topbar.1 ]
    set meanbar  [ frame $topbar.2 ]

    set sbar     [ frame $topbar2.1 ]
    set probar   [ frame $topbar2.2 ]
    set outbar   [ frame $topbar2.3 ]

    pack $mainbar $meanbar -side top -expand false -fill x -pady 0
    pack  $sbar $outbar  $probar -side top -expand false -fill x -pady 0

    # ----------------------- Common ---------------------------------------------
    label         $mainbar.1  -text "Classes:"
    tk_optionMenu $mainbar.2 [ itcl::scope thisparam($this,histo_numclasses) ] "1" "2" "3" "4" "5" "6" "7"
    iwidgets::entryfield $mainbar.3 -labeltext "sigmaratio:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,histo_sigmaratio) ] -relief sunken 
    label         $mainbar.4  -text "Preprocessing Mode:"
    tk_optionMenu $mainbar.5  [ itcl::scope thisparam($this,histo_premode) ] "None" "Positive" "Autorange" 


    pack $mainbar.1 $mainbar.2 $mainbar.3 $mainbar.3 $mainbar.4 $mainbar.5  -side left -expand false -fill x -pady 3

    label $meanbar.0 -text "Means:"
    pack $meanbar.0 -side left -padx 2
    for { set i 1 } { $i <=7 } { incr i } {
	eval "iwidgets::entryfield $meanbar.$i -labeltext \"\"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,histo_mean$i) ] -relief sunken "
	pack $meanbar.$i -side left -expand false -padx 0
    }

    checkbutton   $meanbar.a -variable [ itcl::scope thisparam($this,histo_useinitial) ] -text "Use Initial"
    pack $meanbar.a -side left

    eval "button $bbar.0 -text \"Segment\"  -command { $this HistogramSegmentation  }"
    eval "button $bbar.1 -text \"Show Params\"  -command { $this     DisplayHistogramParameters  }"
    pack $bbar.0 $bbar.1  -side left -fill x -expand false -padx 10 -pady 0

    # -------------------------- Experimental ------------------------------------


    
    label         $probar.1  -text "Distributions:"
    tk_optionMenu $probar.2  [ itcl::scope thisparam($this,histo_distmode) ] "Gaussian" "Robust" 

    eval "iwidgets::entryfield $probar.3 -labeltext \"Valley\"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,histo_vsigma) ] -relief sunken "
    eval "iwidgets::entryfield $probar.4 -labeltext \"RSigma\"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,histo_robustscalefactor) ] -relief sunken "
    pack $probar.1 $probar.2 $probar.3 $probar.4 -side left -expand false -fill x 

    
    iwidgets::entryfield $sbar.1 -labeltext "Bins:"  -width 4 -validate integer -textvariable [ itcl::scope thisparam($this,histo_numbins) ] -relief sunken 
    iwidgets::entryfield $sbar.2 -labeltext "Iterations:"  -width 4 -validate integer -textvariable [ itcl::scope thisparam($this,histo_iterations) ] -relief sunken 
    iwidgets::entryfield $sbar.3 -labeltext "Sigma:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,histo_sigma) ] -relief sunken 
    iwidgets::entryfield $sbar.4 -labeltext "Convergence:"  -width 5 -validate real -textvariable [ itcl::scope thisparam($this,histo_epsilon) ] -relief sunken 
    
    pack $sbar.1 $sbar.2 $sbar.3 $sbar.4 -side left -expand false -fill x 

    checkbutton   $outbar.1 -variable [ itcl::scope thisparam($this,histo_outlier) ] -text "Enable Outlier Detection"
    iwidgets::entryfield $outbar.2 -labeltext "p(outlier):"  -width 6 -validate real -textvariable [ itcl::scope thisparam($this,histo_outlierprobability) ] -relief sunken 
    iwidgets::entryfield $outbar.3 -labeltext "Frame:"  -width 3 -validate integer -textvariable [ itcl::scope thisparam($this,histo_frame) ] -relief sunken 
    pack $outbar.1 $outbar.2 $outbar.3 -side left


    # -------------------
    # MRF Bar
    # -------------------
    label $mrfbar.l -bg black -fg white -text "Using All parameters from Histogram Segmentation as well"
    pack $mrfbar.l  -side top -pady 1 -fill x -expand t
    set bbar    [ frame $mrfbar.2 ]
    pack $bbar  -side bottom -pady 1 -fill x -expand t

    eval "button $bbar.0 -text \"Do MRF Segmentation\"  -command { $this MRFSegmentation  }"
    eval "button $bbar.1 -text \"Show Params\"  -command { $this     DisplayHistogramParameters  }"
    pack $bbar.0 $bbar.1  -side left -fill x -expand false -padx 10 -pady 0
    
    set nt [ iwidgets::tabnotebook $mrfbar.nt -tabpos s -angle 45 -margin 2 -backdrop gray -raiseselect true ]
    pack $nt -side top -expand t -fill both

    set simpane  [ $nt add -label Standard   ]
    set exppane [ $nt add -label Experimental ]
    $nt view "Standard"

    set mainbar   $simpane
    set mainbar2   $exppane
    #    pack $mainbar2 -side top -expand false -fill x -pady 0

    # Simple --
    # ---------
    
    iwidgets::entryfield $mainbar.1 -labeltext "Iterations"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,mrf_iterations) ] -relief sunken 
    iwidgets::entryfield $mainbar.2 -labeltext "Smoothness:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,mrf_smoothness) ] -relief sunken 
    iwidgets::entryfield $mainbar.3 -labeltext "Convergence:"  -width 5 -validate real -textvariable [ itcl::scope thisparam($this,mrf_convergence) ] -relief sunken 
    iwidgets::entryfield $mainbar.4 -labeltext "Noise Sigma"  -width 4 -validate integer -textvariable [ itcl::scope thisparam($this,mrf_imagenoise) ] -relief sunken 
    pack $mainbar.1 $mainbar.2 $mainbar.3 $mainbar.4  -side left -expand false -fill x 



    # Advanced ------
    # ----------------
    checkbutton   $mainbar2.4 -variable [ itcl::scope thisparam($this,mrf_fuzzy) ] -text "Fuzzy"
    checkbutton   $mainbar2.5 -variable [ itcl::scope thisparam($this,mrf_singlechannel) ] -text "Single"
    iwidgets::entryfield $mainbar2.6 -labeltext "Channel:"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,histo_frame) ] -relief sunken 
    pack $mainbar2.4 $mainbar2.5 $mainbar2.6  -side left -expand false -fill x 

    if { $pxtcl_pref_array(EnableBetaFeatures) == "Enabled" } {
	checkbutton   $mainbar2.7 -variable [ itcl::scope thisparam($this,mrf_dobiasfield) ] -text "B1 Correction"
	iwidgets::entryfield $mainbar2.8 -labeltext "B1 Sigma:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,mrf_biasfieldsigma) ] -relief sunken 
	pack  $mainbar2.7 $mainbar2.8  -side left -expand false -fill x 
    }




    ::pxitclbasecontrol::SetWidgetTreeColors $exppane  "\#aaabd7" "black"
    ::pxitclbasecontrol::SetWidgetTreeColors $advanced "\#aaabd7" "black"
}

# ---------------------------------------------------------------------------------

itcl::body pxitclbrainstrip::CreateBiasFieldCorrectionControl { base } {


    set   pbar      [ frame $base.1  ]
    set   midbar    [ frame $base.2 -bg black -height 3  ]
    set   outputbar [ frame $base.3 ]


    pack  $base.3 $base.2  -side bottom -expand false -fill x  -pady 1 -padx 2
    pack  $base.1  -side top -expand true -fill both  -pady 1 -padx 2

   
    label $pbar.a -bg black -fg white -text "Using modified PABIC method (Styner et al, TMI 2000)"
    pack $pbar.a -side top -expand false -fill x -pady 0 -padx 1

    set slbar   [LabelFrame:create $pbar.1 -text "Slice Homogeneity" -expand false -fill x ]
    set polybar [LabelFrame:create $pbar.2 -text "Polynomial Homogeneity" -expand false -fill x ]

    pack  $pbar.1 $pbar.2  -side top -expand false -fill x

    # ------------- Slice Homogeneity GUI ---------------------
    checkbutton   $slbar.1 -variable [ itcl::scope thisparam($this,pabic_slice_usemask) ] -text "Use Mask"
    checkbutton   $slbar.2 -variable [ itcl::scope thisparam($this,pabic_slice_robust)  ] -text "Median"	    
    checkbutton   $slbar.25 -variable [ itcl::scope thisparam($this,pabic_slice_purescaling)  ] -text "Pure Scaling"	    
    eval  "button $slbar.3 -text \"Run Slice\"  -command { $this RunSliceHomogeneity  }"
    eval  "button $slbar.4 -text \"Run All Slices\"  -command { $this RunTripleSliceHomogeneity  }"
    pack $slbar.4 $slbar.3 -side right -padx 4
    pack $slbar.1 $slbar.2 $slbar.25 -side left -padx 2

    
    
    # ------------- Polynomial Fitting -------------------------

    set butbar    [ frame $polybar.nt2 ]
    set nt [ iwidgets::tabnotebook $polybar.nt -tabpos s -height 100 -angle 45 -margin 2 -backdrop gray -raiseselect true ]
    pack $nt  -side top -expand t -fill both
    pack $butbar  -side top -expand f -fill x -pady 1

    set topbar  [ $nt add -label Standard   ]
    set topbar2 [ $nt add -label Advanced ]
    $nt view "Standard"


    set parambar  [ frame $topbar.2 ]
    set parambar2  [ frame $topbar.3 ]

    pack $parambar2 $parambar  -side top -expand false -fill x -pady 1

    set advbar   [ frame $topbar2.1 ]
    set advbar2   [ frame $topbar2.2 ]

    pack $advbar $advbar2  -side top -expand false -fill x -pady 1

    # ----------------------------------------------------------------------------------------------

    label $parambar.0 -text "Parameters:"
    iwidgets::entryfield $parambar.1 -labeltext "x"  -width 5 -validate real -textvariable [ itcl::scope thisparam($this,pabic_shimval_0) ] -relief sunken 
    iwidgets::entryfield $parambar.2 -labeltext "y"  -width 5 -validate real -textvariable [ itcl::scope thisparam($this,pabic_shimval_1) ] -relief sunken 
    iwidgets::entryfield $parambar.3 -labeltext "z"  -width 5 -validate real -textvariable [ itcl::scope thisparam($this,pabic_shimval_2) ] -relief sunken 
    iwidgets::entryfield $parambar.4 -labeltext "x2"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,pabic_shimval_3) ] -relief sunken 
    iwidgets::entryfield $parambar.5 -labeltext "y2"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,pabic_shimval_4) ] -relief sunken 
    iwidgets::entryfield $parambar.6 -labeltext "z2"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,pabic_shimval_5) ] -relief sunken 
    pack $parambar.0 $parambar.1 $parambar.2 $parambar.3 $parambar.4  $parambar.5 $parambar.6 -side left -expand t -fill x -padx 0

    label $parambar2.1 -text "Resolution:" 
    tk_optionMenu $parambar2.2 [ itcl::scope thisparam($this,pabic_shim_resolution) ] 1 2 3 4 
    label         $parambar2.3 -text "Degree"
    tk_optionMenu  $parambar2.4 [ itcl::scope thisparam($this,pabic_shim_degree) ] 1 2 3
    checkbutton   $parambar2.5 -variable [ itcl::scope thisparam($this,pabic_shim_usemask) ] -text "Mask"	    
    checkbutton   $parambar2.6 -variable [ itcl::scope thisparam($this,pabic_shim_useinitial) ] -text "Initial Params"
#    checkbutton   $parambar2.7 -variable [ itcl::scope thisparam($this,pabic_shim_updatesegm) ] -text "Upd Segm"

    pack  $parambar2.1 $parambar2.2 $parambar2.3 $parambar2.4  $parambar2.5 $parambar2.6 -side left -expand true -fill x -padx 0

    eval "button $butbar.1 -text \"Apply\"  -command { $this RunManualShim  }"
    eval "button $butbar.2 -text \"Optimize\"  -command { $this RunPolynomialPabic  }"

    #    $butbar.1 configure -fg red
    #    $butbar.2 configure -fg red

    frame $butbar.25 -bg black
    eval "button $butbar.3 -text \"Clear \"  -command { $this ClearPolynomial  }"
    eval "button $butbar.4 -text \"Print\"  -command { $this PrintPolynomial   }"
    eval "button $butbar.5 -text \"Load\"  -command { $this LoadPolynomial  }"
    eval "button $butbar.6 -text \"Save\"  -command { $this SavePolynomial  }"

    pack $butbar.6 $butbar.5 $butbar.4 $butbar.3 $butbar.25 -side right -expand t -padx 0  -fill y
    pack $butbar.1 $butbar.2  -side left -expand t -padx 5 -fill both

    # ----------------------------------------------------------------------------------------------
    # Experimental
    # ----------------------------------------------------------------------------------------------
    label $advbar.0 -text "Lvls/Stps:" 
    tk_optionMenu $advbar.1 [ itcl::scope thisparam($this,pabic_shim_numlevels) ] 1 2 3
    tk_optionMenu $advbar.2 [ itcl::scope thisparam($this,pabic_shim_numsteps) ]  1 2 3 4
    label $advbar.3 -text "Step:" 
    tk_optionMenu $advbar.4 [ itcl::scope thisparam($this,pabic_shim_stepsize) ] 0.2 0.1 0.05 0.025 0.01 0.005
    label $advbar.5 -text "Iter:" 
    tk_optionMenu $advbar.6 [ itcl::scope thisparam($this,pabic_shim_iterations) ] 1 2 5 10 20
    iwidgets::entryfield $advbar.7 -labeltext "Frame (4D Images):"  -width 3 -validate integer -textvariable [ itcl::scope thisparam($this,histo_frame) ] -relief sunken 

    pack  $advbar.0 $advbar.1 $advbar.2 $advbar.3 $advbar.4  $advbar.5 $advbar.6 $advbar.7   -side left -expand false -fill x -padx 0

    set opt_methods [ list  "Powell" "GradientDescent" "ConjugateGradient" ]
    label $advbar2.1 -text "Opt Method"
    eval "tk_optionMenu $advbar2.2 [ itcl::scope thisparam($this,pabic_optmethod) ] $opt_methods"
    iwidgets::entryfield $advbar2.3 -labeltext "Min B1:" -textvariable [ itcl::scope thisparam($this,pabic_min) ] -relief sunken -width 4 -validate real 
    iwidgets::entryfield $advbar2.4 -labeltext "Max B1:" -textvariable [ itcl::scope thisparam($this,pabic_max) ] -relief sunken -width 4 -validate real 


    pack  $advbar2.1 $advbar2.2  $advbar2.3 $advbar2.4 -side left -expand false -fill x -padx 0

    # ----------------------------------------------------------------------------------------------
    set thisparam($this,pabic_maskgui) [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $thisparam($this,pabic_maskgui) configure -description "Mask"
    $thisparam($this,pabic_maskgui) InitializeLite $pbar.3 2
    $thisparam($this,pabic_maskgui) AddFunction "$parent SetResultsFromObject" "Display" "$this"
    $thisparam($this,pabic_maskgui) AddFunction "$this GrabMorphologyMask" "Grab Morph" "$this"
    $thisparam($this,pabic_maskgui) AddFunction "$this SetMorphologyMask" "Set Morph" "$this"
    pack $pbar.3  -side top -pady 0 -ipady 0 -padx 2

    # ----------------------------------------------------------------------------------------------

    set thisparam($this,pabic_restoredgui) [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $thisparam($this,pabic_restoredgui) configure -description "Bias Corrected Image"
    $thisparam($this,pabic_restoredgui) InitializeLite $outputbar.2 2
    $thisparam($this,pabic_restoredgui) AddFunction "$parent SetResultsFromObject" "Display" "$this"

    set thisparam($this,pabic_biasfieldgui) [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $thisparam($this,pabic_biasfieldgui) configure -description "Bias Field Estimate"
    $thisparam($this,pabic_biasfieldgui) InitializeLite $outputbar.3 2
    $thisparam($this,pabic_biasfieldgui) AddFunction "$parent SetResultsFromObject" "Display" "$this"
    $thisparam($this,pabic_biasfieldgui) AddFunction "$this ApplyB1Field 0" "Apply" ""
    $thisparam($this,pabic_biasfieldgui) AddFunction "$this ApplyB1Field 1" "Msk_Apply" ""



    pack $outputbar.2 $outputbar.3 -side top -pady 0 -ipady 0 -padx 2


    ::pxitclbasecontrol::SetWidgetTreeColors $topbar2  "\#aaabd7" "black"

}

# ----------------------------------------------------------------------------
#  Morphology Stuff 
# ----------------------------------------------------------------------------
itcl::body pxitclbrainstrip::CheckImage {  image name operation verbosity } {
    
    set ok 0
    
    catch { if { [ $image GetImageSize ] > 1 } {  set ok 1 }   }
    
    if { $verbosity >0 && $ok == 0 } {
	::pxtclutil::Warning "No $name in Memory\n Cannot perform $operation !"
    }

    return $ok
}

# ----------------------------------------------------
#  Histogram Segmentation
# ----------------------------------------------------
itcl::body pxitclbrainstrip::HistogramSegmentation { } {

    if { [ CheckImage $currentimage "Image" "Histogram Segmentation" 1 ] == 0 } {
	return 0
    }

    WatchOn
    set segm [ vtkpxHistogramSegmentation [ pxvtable::vnewobj ]]
    $segm SetInput [ $currentimage GetImage ]
    $segm SetNumberOfClasses $thisparam($this,histo_numclasses)
    $segm SetIterations $thisparam($this,histo_iterations)
    $segm SetNumberOfBins $thisparam($this,histo_numbins)
    $segm SetConvergence $thisparam($this,histo_epsilon)
    $segm SetSigma $thisparam($this,histo_sigma)
    $segm SetKeyChannel [ expr $thisparam($this,histo_frame) -1 ]
    $segm GenerateLabelsOn
    $segm SetOutlierDetection $thisparam($this,histo_outlier)

    if { $thisparam($this,histo_premode) == "None" } {
	$segm SetPreprocessingMode 0 
    } elseif { $thisparam($this,histo_premode) == "Positive" } {
	$segm SetPreprocessingMode 1
    } elseif { $thisparam($this,histo_premode) == "Autorange" } {
	$segm SetPreprocessingMode 2
    } 

    if { $thisparam($this,histo_distmode) ==  "Gaussian" } {
	$segm SetDistributionModel 0
    } else {
	$segm SetDistributionModel 1
    }

    $segm SetOutlierProbability $thisparam($this,histo_outlierprobability)
    $segm SetMaxSigmaRatio      $thisparam($this,histo_sigmaratio)
    $segm SetRobustScaleFactor  $thisparam($this,histo_robustscalefactor)

    if { $thisparam($this,histo_useinitial) == 1 } {
	set ar [ vtkFloatArray [ pxvtable::vnewobj ]]
	set np $thisparam($this,histo_numclasses)
	$ar SetNumberOfComponents 3
	$ar SetNumberOfTuples $np

	for { set i 1 } { $i <= $np } { incr i } {
	    set j [ expr $i -1 ]
	    $ar SetComponent $j 1 $thisparam($this,histo_mean$i)
	    $ar SetComponent $j 2 $thisparam($this,histo_vsigma)
	    $ar SetComponent $j 0 100
	}
	$segm SetInitialParameters $ar
	$ar Delete
    }
    
    SetFilterCallbacks $segm   "Histogram Segmenting image"


    $segm Update



    $currentresults ShallowCopyImage [ $segm GetOutput ]


    set outimage [ $thisparam($this,segmoutput_gui) GetObject ]
    $outimage ShallowCopyImage [ $segm GetOutput ]
    $outimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "histo" ]
    $outimage  CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,segmoutput_gui) Update

    $this GenerateMaskFromSegmentedOutput 0

    ProcessImageResults "histo"
    $thisparam($this,histo_params) DeepCopy [ $segm GetParameters ]


    if { $thisparam($this,histo_outlier) == 1 } {
	set nt [ expr [ $thisparam($this,histo_params) GetNumberOfTuples ] -1 ]
	$thisparam($this,histo_params) SetComponent $nt 2 -1
    }

    $segm Delete
    WatchOff
    DisplayHistogramParameters
}


itcl::body pxitclbrainstrip::MRFSegmentation { } {

    if { [ CheckImage $currentimage "Image" "MRF Segmentation" 1 ] == 0 } {
	return
    }

    WatchOn
    set segm [ vtkpxMRFSegmentation [ pxvtable::vnewobj ]]
    $segm SetInput [ $currentimage GetImage ]
    $segm SetNumberOfClasses $thisparam($this,histo_numclasses)
    $segm SetIterations $thisparam($this,histo_iterations)
    $segm SetNumberOfBins $thisparam($this,histo_numbins)
    $segm SetConvergence $thisparam($this,histo_epsilon)
    $segm SetSigma $thisparam($this,histo_sigma)
    $segm SetImageNoiseSigma $thisparam($this,mrf_imagenoise)
    $segm SetOutlierDetection $thisparam($this,histo_outlier)
    $segm SetComputeBiasField  $thisparam($this,mrf_dobiasfield)
    $segm SetBiasFieldFactor $thisparam($this,mrf_biasfieldsigma)
    $segm SetKeyChannel [ expr $thisparam($this,histo_frame) -1 ]

    $segm SetPreprocessingMode 0 
    if { $thisparam($this,histo_premode) == "None" } {
	$segm SetPreprocessingMode 0 
    } elseif { $thisparam($this,histo_premode) == "Positive" } {
	$segm SetPreprocessingMode 1
    }  elseif { $thisparam($this,histo_premode) == "Autorange" } {
	$segm SetPreprocessingMode 2
    } 

    if { $thisparam($this,histo_distmode) ==  "Gaussian" } {
	$segm SetDistributionModel 0
    } else {
	$segm SetDistributionModel 1
    }
    
    $segm SetRobustScaleFactor  $thisparam($this,histo_robustscalefactor)
    $segm SetMaxSigmaRatio      $thisparam($this,histo_sigmaratio)
    $segm SetOutlierProbability $thisparam($this,histo_outlierprobability)
    $segm SetMRFIterations $thisparam($this,mrf_iterations)
    $segm SetSmoothness $thisparam($this,mrf_smoothness)
    $segm SetHistogramNormalization $thisparam($this,mrf_histonorm)
    $segm SetMRFConvergencePercentage $thisparam($this,mrf_convergence)
    $segm SetFuzzyClassification $thisparam($this,mrf_fuzzy)
    $segm SetSingleChannel       $thisparam($this,mrf_singlechannel)
    SetFilterCallbacks $segm   "Histogram Segmenting image"
    $segm SetDebugVoxel 1
    $segm Update

    if { $thisparam($this,mrf_fuzzy) == 0 } {
	$currentresults ShallowCopyImage [ $segm GetOutput ]
    } else {
	$currentresults ShallowCopyImage [ $segm GetMultiFrameOutput ]
    }

    ProcessImageResults "histo"
    $thisparam($this,histo_params) DeepCopy [ $segm GetParameters ]


    if { $thisparam($this,histo_outlier) == 1 } {
	set nt [ expr [ $thisparam($this,histo_params) GetNumberOfTuples ] -1 ]
	$thisparam($this,histo_params) SetComponent $nt 2 -1
    }

    set outimage [ $thisparam($this,segmoutput_gui) GetObject ]
    $outimage ShallowCopyImage [ $segm GetOutput ]
    $outimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "histo" ]
    $outimage  CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,segmoutput_gui) Update

    if {  $thisparam($this,mrf_dobiasfield) == 1 } {
	set biasfieldimage [ $thisparam($this,pabic_biasfieldgui) GetObject ]
	$biasfieldimage  ShallowCopyImage [ $segm GetBiasField ]
	$biasfieldimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "biasfield" ]
	$biasfieldimage  CopyImageHeader [ $currentimage GetImageHeader ]
	$thisparam($this,pabic_biasfieldgui) Update
    }



    $this GenerateMaskFromSegmentedOutput 0
    $segm Delete
    WatchOff
    DisplayHistogramParameters
}

itcl::body pxitclbrainstrip::DisplayHistogramParameters { } {

    pxtkconsole
    pxtkprint "\n Histogram Segmentation Results\n---------------------------\nClass \t Number of Voxels \t\t Mean \t Standard Deviation\n\n"
    
    set hasoutlier 0

    for { set i 1 } { $i <=7 } { incr i } {
	set thisparam($this,histo_mean$i) $i
    }

    set arr $thisparam($this,histo_params)
    if { $arr != "" } {
	if { [ $arr GetNumberOfTuples ] > 0 } {
	    for { set i 0 } { $i < [ $arr GetNumberOfTuples ] } { incr i } {
		set v [ $arr GetComponent $i 2 ]
		if { $v >= 0.0 } {
		    pxtkprint "Class [ expr $i+1 ]\t"
		    set k [ expr $i +1 ]
		    set thisparam($this,histo_mean$k) [ $arr GetComponent $i 1 ]
		} else {
		    pxtkprint "Outlier\t"
		    set hasoutlier 1
		}
		for { set j 0 } { $j < [ $arr GetNumberOfComponents ] } { incr j } {
		    pxtkprint " [ $arr GetComponent $i $j ] \t"
		}

		set mean [ $arr GetComponent  $i 1 ]
		set sigma [ $arr GetComponent $i 2 ]
		pxtkprint [ format "ratio=%.2f " [ expr $mean / $sigma ] ]

		pxtkprint "\n"
	    }
	    set thisparam($this,histo_details) " [ expr [ $arr GetNumberOfTuples ] - $hasoutlier ] classes in memory (outlier=$hasoutlier)"
	} else {
	    set thisparam($this,histo_details) "No parameters in memory"
	}

	set hist [ vtkpxHistogramSegmentation [ pxvtable::vnewobj ]]

	# Do Thresholds 
	for { set i 0 } { $i < [ expr [  $arr GetNumberOfTuples ] - 1 ] } { incr i } {
	    set k1 [ expr $i    ]
	    set k2 [ expr $i +1 ] 
	    set v [ $arr GetComponent $k2 1 ]
	    if { $v >= 0.0 } {
		pxtkprint "Threshold [ expr $i+1 ]:[ expr $i+2 ]:"
		set m1 [ $arr GetComponent $k1 1 ]
		set m2 [ $arr GetComponent $k2 1 ]
		set s1 [ $arr GetComponent $k1 2 ]
		set s2 [ $arr GetComponent $k2 2 ]
		
		set t1 [ $hist GetThreshold $m1 $m2 $s1 $s2 ]

		pxtkprint  "Threshold $k2 = $t1 \n" 
	    }
	}
	
    } else {
	set thisparam($this,histo_details) "No parameters in memory"
    }

}

itcl::body pxitclbrainstrip::GenerateMaskFromSegmentedOutput { mode } {
    
    set ok 0
    set img1 [ $thisparam($this,segmoutput_gui) GetObject ]
    catch { if { [ $img1 GetImageSize ] > 1 } {  set ok 1 }   }
    if { $ok == 0 } {
	pxtkconsole "No Segmentation; Running Histogram Segmentation First!"
	set ok [ HistogramSegmentation ]
	pxtkconsole "Histogram Segmentation Done!"
    }
    
    if { $ok == 0 } {
	return
    }

    set numgood 0
    for { set i 1 } { $i <= 7 } { incr i } {
	if { $thisparam($this,segm_useclass$i) == 1 } {
	    incr numgood
	}
    }

    if { $numgood == 0 } {
	::pxtclutil::Warning "At least one class must be selected to create a morphology mask!"
	return 0
    }

    set map [ vtkDoubleArray [ pxvtable::vnewobj ]]
    $map SetNumberOfComponents 3
    $map SetNumberOfTuples 14
    
    $map SetTuple3 0 -0.5 0.0 0.0
    for { set l 0 } { $l <= 6 } { incr l } {
	set t1  [ expr $l -0.1 ]
	set t2  [ expr $l +0.1 ]
	set k  0.0
	set c  0.0 
	set lp [ expr $l + 1 ]
	if { $thisparam($this,segm_useclass$lp) == 1 } {
	    set c 1.0
	}
	$map SetTuple3 [ expr 2*$l   ] $t1 0  $k
	$map SetTuple3 [ expr 2*$l+1 ] $t2 $c $k
    }
       
    WatchOn
    set sh  [ vtkpxImageAlgebra [ pxvtable::vnewobj ]  ]
    $sh SetInput [ $img1 GetImage ]
    $sh SetMap $map
    $sh SetOutputScalarTypeToUnsignedChar
    SetFilterCallbacks $sh "Creating Mask From Segmented Data"
    $sh Update
    
    $oldmask ShallowCopy $currentmask
    if { $mode == 0 } {
	$currentmask ShallowCopyImage  [ $sh GetOutput ]
    }  else {
	ProcessMorphologyResults [ $sh GetOutput ]
    }
    WatchOff
    catch { $sh Delete }
    $map Delete
    WatchOff
}    

# ----------------------------------------------------
#  Pabic Bias Field Correction
# ----------------------------------------------------
itcl::body pxitclbrainstrip::RunSliceHomogeneity   {  } { 

    if { [ CheckImage $currentimage "Image" "Bias Field Correction" 1 ] == 0 } { 
	return 0  
    }
    
    if { $thisparam($this,pabic_slice_usemask) == 1  } {
	set img1 [ $thisparam($this,pabic_maskgui) GetObject ]
	if { [ $img1 GetImageSize ] != [ $currentimage GetImageSize ] } {
	    ::pxtclutil::Warning "Mask does not have the same size as the image to be corrected! Cannot perform bias field correction!"
	    return 0
	}
    }

    WatchOn    


    pxtkprint ".... Slice Inhomogeneity Correction\n"
    set fit [ vtkpxSliceBiasFieldCorrection  [ pxvtable::vnewobj ]]
    if { $thisparam($this,pabic_slice_usemask) == 1  } {
	pxtkprint ".... Using mask image\n"
	$fit SetMaskImage [ [ $thisparam($this,pabic_maskgui) GetObject ] GetImage ]
    }
	
    $fit SetRobustMode $thisparam($this,pabic_slice_robust)
    $fit SetPureScaling $thisparam($this,pabic_slice_purescaling) 
    $fit SetInput [ $currentimage GetImage ] 
    $fit SetFrame  [ expr $thisparam($this,histo_frame) -1 ]
    
    pxtkconsole
    eval "$fit AddObserver ProgressEvent { pxtkprint  \"Sl:\[ $fit GetOutputStatus \] \n\" }"
    $fit Update

    set outimage [ $thisparam($this,pabic_restoredgui) GetObject ]
    set b1image  [ $thisparam($this,pabic_biasfieldgui) GetObject ]

    $outimage ShallowCopyImage [ $fit GetOutput ]
    $b1image  ShallowCopyImage [ $fit GetBiasField ]
    $fit Delete
    WatchOff

    $outimage  CopyImageHeader [ $currentimage GetImageHeader ]
    $outimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "slice" ]
    $b1image  CopyImageHeader [ $currentimage GetImageHeader ]
    $b1image  configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "b1_slice" ]


    $thisparam($this,pabic_restoredgui) Update
    $thisparam($this,pabic_biasfieldgui) Update
    $parent SetResultsFromObject $outimage $this

}

# ----------------------------------------------------------------------------------------------------


itcl::body pxitclbrainstrip::RunTripleSliceHomogeneity   {  } { 

    if { [ CheckImage $currentimage "Image" "Bias Field Correction" 1 ] == 0 } { 
	return 0  
    }
    
    if { $thisparam($this,pabic_slice_usemask) == 1  } {
	set img1 [ $thisparam($this,pabic_maskgui) GetObject ]
	if { [ $img1 GetImageSize ] != [ $currentimage GetImageSize ] } {
	    ::pxtclutil::Warning "Mask does not have the same size as the image to be corrected! Cannot perform bias field correction!"
	    return 0
	}
    }

    WatchOn    

    set axislist { x y z }

    set output [ vtkImageData [ pxvtable::vnewobj ]]
    $output ShallowCopy [ $currentimage GetImage ] 

    for { set axis 0 } { $axis <=2 } { incr axis } {
	pxtkprint ".... Slice Inhomogeneity Correction orientation  axis= [ lindex $axislist $axis ]\n"
	
	set fit [ vtkpxSliceBiasFieldCorrection  [ pxvtable::vnewobj ]]
	
	if { $thisparam($this,pabic_slice_usemask) == 1  } {
	    pxtkprint ".... Using mask image\n"
	    $fit SetMaskImage [ [ $thisparam($this,pabic_maskgui) GetObject ] GetImage ]
	}

	$fit SetAxis $axis
	$fit SetRobustMode $thisparam($this,pabic_slice_robust) 
	$fit SetPureScaling $thisparam($this,pabic_slice_purescaling) 
	$fit SetInput $output
	$fit SetFrame  [ expr $thisparam($this,histo_frame) -1 ]
	
	pxtkconsole
	eval "$fit AddObserver ProgressEvent { pxtkprint  \"Sl:\[ $fit GetOutputStatus \] \n\" }"
	$fit Update
	$output ShallowCopy [ $fit GetOutput ]
	$fit Delete
    }

    set fit [ vtkpxSliceBiasFieldCorrection  [ pxvtable::vnewobj ]]
    set b1tmp [ $fit ComputeRatioBiasFieldImage [ $currentimage GetImage ] $output ]
    $fit Delete

    set b1image  [ $thisparam($this,pabic_biasfieldgui) GetObject ]
    $b1image ShallowCopyImage $b1tmp
    $b1tmp Delete
    $b1image  configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "b1_triple" ]
    $b1image  CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,pabic_biasfieldgui) Update

    set outimage [ $thisparam($this,pabic_restoredgui) GetObject ]
    $outimage ShallowCopyImage $output;
    $output Delete
    WatchOff


    $outimage  CopyImageHeader [ $currentimage GetImageHeader ]
    $outimage  configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "tripleslice" ]

    $thisparam($this,pabic_restoredgui) Update
    $parent SetResultsFromObject $outimage $this
}

# ----------------------------------------------------------------------------------------------------


itcl::body pxitclbrainstrip::RunImageApproximation {  } { 

    if { [ CheckImage $currentimage "Image" "Bias Field Correction" 1 ] == 0 } { 
	return 0  
    }

    set mask [ $thisparam($this,pabic_maskgui) GetObject ]
    if { [ $mask GetImageSize ] != [ $currentimage GetImageSize ] } {
	::pxtclutil::Warning "Image Approximation Requires a Mask. The mask does not have the same size as the image to be approximated!"
	return 0
    }
    
    WatchOn
    set bspl [ vtkpxImageTensorSpline [ pxvtable::vnewobj ]]
    $bspl SetMaskImage [ $mask GetImage ]
    $bspl SetInput [ $currentimage GetImage ]
    $bspl SetLambda [ expr 100.0*$thisparam($this,pabic_spl_lambda) ]
    $bspl SetStepSize $thisparam($this,pabic_spl_stepsize)
    $bspl SetResolution $thisparam($this,pabic_sample)
    $bspl SetPreSmoothIterations $thisparam($this,pabic_spl_presmooth);
    $bspl SetMode 1
    $bspl SetControlPointSpacing $thisparam($this,pabic_spl_spacing)
    $bspl SetAutoInitialize 1
    $bspl SetIterations $thisparam($this,pabic_iterations)
    $bspl SetEpsilon 0.01
    $bspl Run

    set b1image  [ $thisparam($this,pabic_biasfieldgui) GetObject ]
    $b1image  ShallowCopyImage [ $bspl GetOutput ]
    $b1image  configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "b1_approx" ]
    $b1image  CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,pabic_biasfieldgui) Update

    set fit [ vtkpxAbstractBiasFieldCorrection  [ pxvtable::vnewobj ]]
    set img [ $fit ComputeMeanPreservingCorrectedImage [ $currentimage GetImage ] [ $b1image GetImage ] 

    set outimage [ $thisparam($this,pabic_restoredgui) GetObject ]
    $outimage ShallowCopyImage $img
    $outimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "approx" ]
    $outimage CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,pabic_restoredgui) Update
    $img Delete

    WatchOff
    $parent SetResultsFromObject $outimage $this
}

# ----------------------------------------------------------------------------------------------------
itcl::body pxitclbrainstrip::GrabMorphologyMask { arg1 arg2 } {

    [ $thisparam($this,pabic_maskgui) GetObject ]  ShallowCopy $currentmask
    [ $thisparam($this,pabic_maskgui) GetObject ] configure -filename [ AddPrefix [ $currentimage cget -filename  ] "smask" ]
    $thisparam($this,pabic_maskgui) Update
}

itcl::body pxitclbrainstrip::SetMorphologyMask { arg1 arg2 } {

    set sh [ vtkImageCast [ pxvtable::vnewobj ]]
    $sh SetInput [ [ $thisparam($this,pabic_maskgui) GetObject ]  GetImage ]
    $sh SetOutputScalarTypeToUnsignedChar
    $sh Update

    $currentmask ShallowCopyImage  [ $sh GetOutput ]
    $sh Delete
}

itcl::body pxitclbrainstrip::SetMorphologyMaskImage { img } {

    set sh [ vtkImageCast [ pxvtable::vnewobj ]]
    $sh SetInput $img
    $sh SetOutputScalarTypeToUnsignedChar
    $sh Update

    $currentmask ShallowCopyImage  [ $sh GetOutput ]

    puts stdout "Getting mask from $img [ [ [ $img GetPointData ] GetScalars ] GetRange ]"
    $sh Delete
}

itcl::body pxitclbrainstrip::ApplyB1Field { args } { 

    if { [ CheckImage $currentimage "Image" "Bias Field Correction" 1 ] == 0 } { 
	return 0  
    }

    set b1image  [ $thisparam($this,pabic_biasfieldgui) GetObject ]

    if { [ $b1image GetImageSize ] != [ $currentimage GetImageSize ] } {
	::pxtclutil::Warning "B1 - Field  does not have the same size as the image to be corrected! Cannot apply the bias field!"
	return 0
    }

    set domask 0
    if { [ llength $args ] > 0 } {
	if { [ lindex $args 0 ] > 0 } {
	    set domask 1
	}
    }

    if { $domask == 1 } {
	set mask_image [ $thisparam($this,pabic_maskgui) GetObject ]
	if { [ $mask_image GetImageSize ] != [ $currentimage GetImageSize ] } {
	    ::pxtclutil::Warning "Mask does not have the same size as the image to be corrected! Cannot apply masked bias field correction!"
	    return 0
	}
    }

    WatchOn    
    
    set fit [ vtkpxAbstractBiasFieldCorrection  [ pxvtable::vnewobj ]]
    if { $domask == 0  } {
	pxtkprint "Unmasked bias field correction !\n"
	set img [ $fit ComputeCorrectedImage [ $currentimage GetImage ] [ $b1image GetImage ] 0 ]
    } else {
	pxtkprint "Msked bias field correction!\n"
	set img [ $fit ComputeMaskCorrectedImage [ $currentimage GetImage ] [ $b1image GetImage ] [ $mask_image GetImage ] $thisparam($this,pabic_multiplicative) ]
    }
    WatchOff

    $currentresults ShallowCopyImage $img
    $currentresults configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "Corr" ]
    $currentresults CopyImageHeader [ $currentimage GetImageHeader ]
    $parent SetResultsFromObject $currentresults $this
    $img Delete



}

# ----------------------------------------------------------------------------------------------------

itcl::body pxitclbrainstrip::RunSplinePabic {  } {


    if { [ CheckImage $currentimage "Image" "Bias Field Correction" 1 ] == 0 } { 
	return 0  
    }
    
    if { $thisparam($this,pabic_usemask) == 1  } {
	set img1 [ $thisparam($this,pabic_maskgui) GetObject ]
	if { [ $img1 GetImageSize ] != [ $currentimage GetImageSize ] } {
	    ::pxtclutil::Warning "Mask does not have the same size as the image to be corrected! Cannot perform bias field correction!"
	    return 0
	}
    }


    WatchOn    

    set outimage [ $thisparam($this,pabic_restoredgui) GetObject ]
    set b1image  [ $thisparam($this,pabic_biasfieldgui) GetObject ]

    pxtkprint ".... Slice Inhomogeneity Correction\n"
    set fit [ vtkpxSplineFitBiasFieldCorrection  [ pxvtable::vnewobj ]]
    if { $thisparam($this,pabic_usemask) == 1 || $thisparam($this,pabic_usefatmask) == 1 } {
	pxtkprint ".... Using mask image\n"
	if { $thisparam($this,pabic_usefatmask) == 1 } {
	    $fit SetFatMask [ [ $thisparam($this,pabic_maskgui) GetObject ] GetImage ]
	} else {
	    $fit SetMaskImage [ [ $thisparam($this,pabic_maskgui) GetObject ] GetImage ]
	}
    }
    $fit SetMode [ expr 1-$thisparam($this,pabic_multiplicative) ]
    $fit SetInput [ $currentimage GetImage ] 
    $fit SetMetric $thisparam($this,pabic_spl_metric) 
    $fit SetLambda [ expr $thisparam($this,pabic_spl_lambda)*0.01 ]
    $fit SetStepSize $thisparam($this,pabic_spl_stepsize)
    $fit SetNumberOfLevels 1
    $fit SetFatMaskWeight $thisparam($this,pabic_spl_fatmaskweight)
    $fit SetResolution $thisparam($this,pabic_sample)
    $fit SetControlPointSpacing $thisparam($this,pabic_spl_spacing)
    $fit SetNumberOfIterations $thisparam($this,pabic_iterations)
    $fit SetEpsilon 0.1
    

    $fit SetHisto_NumberOfClasses $thisparam($this,histo_numclasses)
    $fit SetHisto_Iterations $thisparam($this,histo_iterations)
    $fit SetHisto_NumberOfBins $thisparam($this,histo_numbins)
    $fit SetHisto_Convergence $thisparam($this,histo_epsilon)
    $fit SetHisto_Sigma $thisparam($this,histo_sigma)
    $fit SetHisto_OutlierDetection $thisparam($this,histo_outlier)
    $fit SetHisto_OutlierProbability $thisparam($this,histo_outlierprobability)


    if { $thisparam($this,histo_premode) == "None" } {
	$fit SetHisto_PreprocessingMode 0
    } elseif { $thisparam($this,histo_premode) == "Positive Only" } {
	$fit SetHisto_PreprocessingMode 1
    } else {
	$fit SetHisto_PreprocessingMode 2
    }

    
    #$fit SetNumberOfLevels 1
    $fit SetNumberOfSteps  1
    
    pxtkconsole

    eval "$fit AddObserver ProgressEvent { pxtkprint  \"Sl:\[ $fit GetOutputStatus \] \n\" }"
    $fit Update

    $outimage ShallowCopyImage [ $fit GetOutput ]
    $b1image  ShallowCopyImage [ $fit GetBiasField ]
    $fit Delete
    WatchOff

    $outimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "spline" ]
    $outimage CopyImageHeader [ $currentimage GetImageHeader ]
    $b1image  configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "b1_spline" ]
    $b1image  CopyImageHeader [ $currentimage GetImageHeader ]


    $thisparam($this,pabic_restoredgui) Update
    $thisparam($this,pabic_biasfieldgui) Update
    $parent SetResultsFromObject $outimage $this
}

# ----------------------------------------------------------------------------------------------------
itcl::body pxitclbrainstrip::ClearPolynomial {  } {

    for { set i 0 } { $i <=18 } { incr i } {
	set thisparam($this,pabic_shimval_$i) 0.0
    }
}

itcl::body pxitclbrainstrip::PrintPolynomial { { fileid 0 }  } {

    set line "\#Polynomial Bias Field Correction Parameters"
    if { $fileid != 0 } {
	puts $fileid $line
    } else {
	pxtkconsole
	pxtkprint "\n\n------------------------------------------\n$line\n"
    }

    set pnamelist "x y z x2 y2 z2 xy xz yz  x3-x y3-y z3-z x2y x2z y2z y2z z2x z2y xyz"

    set count 1
    for { set i 0 } { $i <= 18 } { incr i } {
	set line [ format "%10s : %7.2f " [ lindex $pnamelist $i ] $thisparam($this,pabic_shimval_$i)  ]
	if { $fileid != 0 } {
	    puts $fileid $line
	} else {
	    pxtkprint "$line"
	    incr count
	    if { $count == 5 } {
		pxtkprint "\n"
		set count 1
	    } else {
		pxtkprint "\t"
	    }
	}
    }

    if { $fileid == 0 } {
	pxtkprint "\n\n"
    }

}

itcl::body pxitclbrainstrip::LoadPolynomial {  } {

    set fname [tk_getOpenFile -title "Filename"   -filetypes { {"Polynomial Parameter Files" {.plist}}}]
    if { [ llength $fname ] > 0 } {
	if { [ file exists $fname ] > 0 } {
	    set fileid [open $fname r ]
	    gets $fileid line
	    if { $line == "\#Polynomial Bias Field Correction Parameters" } {
		$this ClearPolynomial
		for { set i 0 } { $i <= 18 } { incr i } {
		    gets $fileid line
		    set index [string first ":" $line]
		    if { $index > 0 }  {
			set len   [string length $line]
			set key   [string trim   [string range $line 0 [expr $index - 1]]]
			set value [string trim [string range $line [expr $index +1] [expr $len -1]]]	
			set thisparam($this,pabic_shimval_$i) $value
		    }
		}
		close $fileid
	    } else {
		pxtclutil::Warning "Failed to Read Polynomial Parameters from $fname"
	    }
	}
    }
}

itcl::body pxitclbrainstrip::SavePolynomial {  } {

    set fname [tk_getSaveFile -title "Filename" -defaultextension .plist  -filetypes { {"Polynomial Parameter Files" {.plist}}}]
    if { [ llength $fname ] > 0 } {
	set fileid [open $fname w]
	$this PrintPolynomial $fileid
	close $fileid
	pxtclutil::Info "Polynomial Parameters Saved in $fname"
    }
}


itcl::body pxitclbrainstrip::RunPolynomialPabic {  } {


    if { [ CheckImage $currentimage "Image" "Polynomial Bias Field Correction" 1 ] == 0 } { 
	return 0  
    }
    
    if { $thisparam($this,pabic_shim_usemask) == 1  } {
	set img1 [ $thisparam($this,pabic_maskgui) GetObject ]
	if { [ $img1 GetImageSize ] != [ $currentimage GetImageSize ] } {
	    ::pxtclutil::Warning "Mask does not have the same size as the image to be corrected! Cannot perform bias field correction!"
	    return 0
	}
    }

    WatchOn    
    set outimage [ $thisparam($this,pabic_restoredgui) GetObject ]
    set b1image  [ $thisparam($this,pabic_biasfieldgui) GetObject ]

    pxtkprint ".... Polynomial Inhomogeneity Correction\n"
    set fit [ vtkpxPolynomialBiasFieldCorrection  [ pxvtable::vnewobj ]]

    if { $thisparam($this,pabic_shim_usemask) == 1 } {
	pxtkprint ".... Using mask image\n"
	$fit SetMaskImage [ [ $thisparam($this,pabic_maskgui) GetObject ] GetImage ]
    }

    $fit SetInput   [ $currentimage GetImage ] 
    $fit SetMetric   $thisparam($this,pabic_shim_metric) 
    $fit SetNumberOfLevels     $thisparam($this,pabic_shim_numlevels)
    $fit SetNumberOfSteps      $thisparam($this,pabic_shim_numsteps)
    $fit SetResolution         $thisparam($this,pabic_shim_resolution)
    $fit SetStepSize           $thisparam($this,pabic_shim_stepsize)
    $fit SetNumberOfIterations $thisparam($this,pabic_shim_iterations)
    $fit SetEpsilon 0.1
    $fit SetDegree $thisparam($this,pabic_shim_degree)
    $fit SetOptimizationMethodTo$thisparam($this,pabic_optmethod)
    $fit SetFrame  [ expr $thisparam($this,histo_frame) -1 ]
    $fit SetMinValue $thisparam($this,pabic_min)
    $fit SetMaxValue $thisparam($this,pabic_max)
    $fit SetRecomputeParameters 0
    $fit SetHisto_NumberOfClasses $thisparam($this,histo_numclasses)
    $fit SetHisto_Iterations $thisparam($this,histo_iterations)
    $fit SetHisto_NumberOfBins $thisparam($this,histo_numbins)
    $fit SetHisto_Convergence $thisparam($this,histo_epsilon)
    $fit SetHisto_Sigma $thisparam($this,histo_sigma)
    $fit SetHisto_MaxSigmaRatio $thisparam($this,histo_sigmaratio)
    $fit SetHisto_OutlierDetection $thisparam($this,histo_outlier)
    $fit SetHisto_OutlierProbability $thisparam($this,histo_outlierprobability)

    if { $thisparam($this,histo_premode) == "None" } {
	$fit SetHisto_PreprocessingMode 0
    } elseif { $thisparam($this,histo_premode) == "Positive Only" } {
	$fit SetHisto_PreprocessingMode 1
    } else {
	$fit SetHisto_PreprocessingMode 2
    }

    
    if { $thisparam($this,pabic_shim_useinitial) == 1 } {
	set db [ vtkDoubleArray [ pxvtable::vnewobj ]]
	$db SetNumberOfTuples 19
	for { set i 0 } { $i <=18 } { incr i } {
	    $db SetComponent $i 0 $thisparam($this,pabic_shimval_$i) 
	}
	$fit SetInitialCoefficients $db
	$db Delete
    }
    
    pxtkconsole

    eval "$fit AddObserver ProgressEvent { pxtkprint  \"Sl:\[ $fit GetOutputStatus \] \n\" }"
    $fit Update

    $outimage ShallowCopyImage [ $fit GetOutput ]
    $b1image  ShallowCopyImage [ $fit GetBiasField ]


    set db [ $fit GetCoefficients ]
    for { set i 0 } { $i <=18 } { incr i } {
	set thisparam($this,pabic_shimval_$i) 0.0
    }

    set maxn 3
    if { $thisparam($this,pabic_shim_degree) == 3 } {
	set maxn 18
    } elseif { $thisparam($this,pabic_shim_degree) == 2 } {
	set maxn 8
    }
	
    for { set i 0 } { $i <= $maxn } { incr i } {
	set thisparam($this,pabic_shimval_$i) [ $db GetComponent $i 0 ]
    }

    $fit Delete
    WatchOff

    $outimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "poly" ]
    $outimage CopyImageHeader [ $currentimage GetImageHeader ]
    $b1image  configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "b1_poly" ]
    $b1image  CopyImageHeader [ $currentimage GetImageHeader ]


    $thisparam($this,pabic_restoredgui) Update
    $thisparam($this,pabic_biasfieldgui) Update
    $parent SetResultsFromObject $outimage $this

}

# ----------------------------------------------------------------------------------------------------
itcl::body pxitclbrainstrip::RunManualShim {  } {


    if { [ CheckImage $currentimage "Image" "Manual Bias Field Correction" 1 ] == 0 } { 
	return 0  
    }
    

    WatchOn    


    pxtkprint ".... Manual Inhomogeneity Correction\n"
    set fit [ vtkpxPolynomialBiasFieldCorrection  [ pxvtable::vnewobj ]]

    set db [ vtkDoubleArray [ pxvtable::vnewobj ]]
    $db SetNumberOfTuples 18
    $db FillComponent 0 0.0
    for { set i 0 } { $i <= 5 } { incr i } {
	$db SetComponent $i 0 $thisparam($this,pabic_shimval_$i) 
    }
    set tmp [ $fit ComputeBiasFieldImage [ $currentimage GetImage ] $db  2 $thisparam($this,pabic_min) $thisparam($this,pabic_max) ]
    puts stdout "Computing Corrected Image"
    set img [ $fit ComputeMeanPreservingCorrectedImage [ $currentimage GetImage ] $tmp  ]
    puts stdout "Done ...\n"
    set outimage [ $thisparam($this,pabic_restoredgui) GetObject ]
    set b1image  [ $thisparam($this,pabic_biasfieldgui) GetObject ]

    set entropy [ $fit ComputeEntropy $img ]

    pxtkconsole
    pxtkprint "\nEntropy = $entropy\n"

    $outimage ShallowCopyImage  $img
    $b1image  ShallowCopyImage  $tmp 

    $db Delete
    $fit Delete
    WatchOff

    $outimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "shim" ]
    $outimage CopyImageHeader [ $currentimage GetImageHeader ]

    $b1image  configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "shim_b1" ]
    $b1image  CopyImageHeader [ $currentimage GetImageHeader ]

    $thisparam($this,pabic_restoredgui) Update
    $thisparam($this,pabic_biasfieldgui) Update
    $parent SetResultsFromObject $outimage $this
}

# ----------------------------------------------------
itcl::body pxitclbrainstrip::SimpleSegment {  } {
    ThresholdImage
    ErodeMap
    ConnectMap 1
    DilateMap
}

itcl::body pxitclbrainstrip::ThresholdImage {  } {

    if { [ CheckImage $currentimage "Image" "thresholding" 1 ] == 0 } {
	return
    }
    
    WatchOn
    
 #   puts stderr "Beginning to threshold image spacing = [ [ $currentimage GetImage ] GetSpacing ]\n\n\n"

    set castF [ vtkImageCast [ pxvtable::vnewobj ]]
    $castF SetInput  [ $currentimage GetImage ]
    $castF SetOutputScalarTypeToFloat
    $castF Update

    set thresholdF  [  vtkImageThreshold [ pxvtable::vnewobj ] ]
    $thresholdF ThresholdBetween  $thisparam($this,lowthreshold) $thisparam($this,highthreshold)
    $thresholdF SetInValue    1
    $thresholdF SetOutValue   0
    $thresholdF ReplaceInOn
    $thresholdF ReplaceOutOn
    $thresholdF SetInput [ $castF GetOutput ]
    $thresholdF SetOutputScalarTypeToUnsignedChar
    SetFilterCallbacks $thresholdF   "Thresholding image"
    $thresholdF Update

#    puts stderr "Done Thresholding image on to processing "

    ProcessMorphologyResults [ $thresholdF GetOutput ]
    catch { $thresholdF Delete }
    catch { $castF Delete }
    WatchOff
}
# ----------------------------------------------------------------------------------------------------
#  
#    Morphology
#
# -------------------------------------------------------------------------------------------

itcl::body pxitclbrainstrip::ErodeMap {  } {
    
    if { [ CheckImage $currentmask "Segmentation Mask" "erosion" 1 ] == 0 } {
	return
    }
	
    WatchOn
    set erode  [  vtkImageContinuousErode3D [ pxvtable::vnewobj ] ]
    $erode SetInput [ $currentmask GetImage ]
    if {  $thisparam($this,gui_morphology2d) == 0  } {
	$erode SetKernelSize $thisparam($this,gui_morphkernelsize) $thisparam($this,gui_morphkernelsize) $thisparam($this,gui_morphkernelsize)
    } else {
	$erode SetKernelSize $thisparam($this,gui_morphkernelsize) $thisparam($this,gui_morphkernelsize) 1
    }
    SetFilterCallbacks $erode "Eroding image"
    $erode Update
    ProcessMorphologyResults  [ $erode  GetOutput ]
    catch { $erode Delete }
    WatchOff
}
# -------------------------------------------------------------------------------------------
itcl::body pxitclbrainstrip::DilateMap {  } {
    
    if { [ CheckImage $currentmask "Segmentation Mask" "dilation" 1 ] == 0 } {
	return
    }
    WatchOn
    
    set dilate  [  vtkImageContinuousDilate3D [ pxvtable::vnewobj ] ]
    $dilate SetInput [ $currentmask GetImage ]
    
    if {  $thisparam($this,gui_morphology2d) == 0  } {
	$dilate SetKernelSize $thisparam($this,gui_morphkernelsize) $thisparam($this,gui_morphkernelsize) $thisparam($this,gui_morphkernelsize)
    } else {
	$dilate SetKernelSize $thisparam($this,gui_morphkernelsize) $thisparam($this,gui_morphkernelsize) 1
    }
    SetFilterCallbacks $dilate "Dilating image"

    $dilate Update
    ProcessMorphologyResults [   $dilate  GetOutput ]
    catch { $dilate Delete }
    WatchOff
    
}
# -------------------------------------------------------------------------------------------    
itcl::body pxitclbrainstrip::DilateErodeMap {  posval minval  } {
    
    if { [ CheckImage $currentmask "Segmentation Mask" "dilation/erosion" 1 ] == 0 } {
	return
    }
    
    WatchOn
    
    set DilateErode  [  vtkImageDilateErode3D [ pxvtable::vnewobj ]  ]
    $DilateErode SetInput [ $currentmask GetImage ]
    if {  $thisparam($this,gui_morphology2d) == 0  } {
	$DilateErode SetKernelSize $thisparam($this,gui_morphkernelsize) $thisparam($this,gui_morphkernelsize) $thisparam($this,gui_morphkernelsize)
    } else {
	$DilateErode SetKernelSize $thisparam($this,gui_morphkernelsize) $thisparam($this,gui_morphkernelsize) 1
    }
    $DilateErode SetDilateValue $posval
    $DilateErode SetErodeValue  $minval
    
    SetFilterCallbacks $DilateErode "Dilating-Eroding image"
    $DilateErode Update
    ProcessMorphologyResults [ $DilateErode  GetOutput ]
    catch { $DilateErode Delete }
    WatchOff
}
# -------------------------------------------------------------------------------------------    
itcl::body pxitclbrainstrip::BinaryMedianMap { } {
	
    if { [ CheckImage $currentmask "Segmentation Mask" "median filtering" 1 ] == 0 } {
	return
    }
    
    WatchOn
    
    set BMedian  [  vtkpxBinaryImageMedian3D [ pxvtable::vnewobj ]  ]
    $BMedian SetInput [ $currentmask GetImage ]
    if {  $thisparam($this,gui_morphology2d) == 0  } {
	$BMedian SetKernelSize $thisparam($this,gui_morphkernelsize) $thisparam($this,gui_morphkernelsize) $thisparam($this,gui_morphkernelsize)
    } else {
	$BMedian SetKernelSize $thisparam($this,gui_morphkernelsize) $thisparam($this,gui_morphkernelsize) 1
    }
    SetFilterCallbacks $BMedian "BMedian-Filtering image"
    $BMedian Update
    ProcessMorphologyResults [ $BMedian GetOutput ]
    catch { $BMedian Delete }
    WatchOff
}
# -------------------------------------------------------------------------------------------        
itcl::body pxitclbrainstrip::ConnectMap { mode  } {
    
    if { [ CheckImage $currentmask "Segmentation Mask" "connectivity" 1 ] == 0 } {
	return
    }


    RequestUpdatedSeed 0
    
    
    set seedF  [  vtkImageSeedConnectivity [ pxvtable::vnewobj ] ]
    $seedF SetInput [ $currentmask GetImage ]

    pxtkprint "\n Seed: $thisparam($this,seedx) $thisparam($this,seedy) $thisparam($this,seedz) mode=$mode\n"
    $seedF AddSeed $thisparam($this,seedx) $thisparam($this,seedy) $thisparam($this,seedz)
    $seedF SetDimensionality 3

    if { $mode == 1 } { 
	$seedF SetInputConnectValue 1
	$seedF SetOutputConnectedValue 1
	$seedF SetOutputUnconnectedValue 0 
    } else {
	$seedF SetInputConnectValue      0
	$seedF SetOutputConnectedValue   0
	$seedF SetOutputUnconnectedValue 1
    }

    SetFilterCallbacks $seedF "Connecting image mode=$mode seed=$thisparam($this,seedx) $thisparam($this,seedy) $thisparam($this,seedz)"
    $seedF Update
    ProcessMorphologyResults [ $seedF GetOutput ]
    catch { seedF Delete }
    WatchOff
}
# -------------------------------------------------------------------------------------------        
itcl::body pxitclbrainstrip::ProcessMorphologyResults { img args } { 

    if { [ CheckImage $currentimage "Image" "overlay" 1 ] == 0 } {
	return
    }
    set backup 1
    if { [ llength $args ] > 0 } {
	if { [ lindex $args 0 ] == "NoBackup" } {
	    set backup 0 
	}
    }

    if { $backup == 1 } {
	$oldmask ShallowCopy $currentmask
    }

#    puts stderr "------------------------------------------------------------------------------------------------------"
#    puts stderr "Going to shallow copy  mask = [ [ $currentmask GetImage ] GetSpacing ] , result = [ $img GetSpacing ]"

    $currentmask ShallowCopyImage $img


    # puts stderr done with current mask
    #    $currentmask Save "test.nii.gz"
#    exit

    if { [ CheckImage $currentmask "Segmentation Mask" "overlay" 1 ] == 0 } {
	return
    }	

    DisplayMorphologyResults
}

itcl::body pxitclbrainstrip::DisplayMorphologyResults {  } { 

    

    if { $thisparam($this,gui_displaymask) == 0 } {
	WatchOn
	set mask  [  vtkpxImageMask [ pxvtable::vnewobj ] ]
	$mask SetInput [ $currentimage GetImage ]
	$mask SetMask  [ $currentmask GetImage ]
	#$mask SetMaskedOutputValue 1
	SetFilterCallbacks $mask "Creating Overlay"
	$mask Update
	$currentresults ShallowCopyImage [ $mask GetOutput ]
	catch { $mask Delete }    
	WatchOff
	$parent SetResultsFromObject  $currentresults $this
    } else {
	$currentresults ShallowCopy $currentmask
	if { [ $parent  CanAcceptMask ] } {
	    $parent SetMaskFromObject $currentresults $this
	} else {
	    $parent SetResultsFromObject  $currentresults $this
	}	
    }
}

# ----------------------------------------------------------------------------

itcl::body pxitclbrainstrip::UndoLastMorphologyOperation {  } {
	
    ProcessMorphologyResults [ $oldmask GetImage ] "NoBackup"
}
# -------------------------------------------------------------------------------------------
itcl::body pxitclbrainstrip::SetImage { img  } {
	
    DelayedInitialize
    
    $currentimage ShallowCopy $img
    $currentresults UnLoad
    $currentresults CopyImageHeader [ $currentimage GetImageHeader ]
    $currentmask UnLoad
    $currentmask CopyImageHeader [ $currentimage GetImageHeader ]
    $oldmask UnLoad
    $oldmask CopyImageHeader [ $currentimage GetImageHeader ]

    set dim2 [ [ $currentimage GetImage ] GetDimensions ]
    set thisparam($this,seedx)  [expr [lindex $dim2 0 ] /2 ]
    set thisparam($this,seedy)  [expr [lindex $dim2 1 ] /2 ]
    set thisparam($this,seedz)  [expr [lindex $dim2 2 ] /2 ]

    set range [ [ [ [ $currentimage GetImage ] GetPointData] GetScalars] GetRange ]
    set lowrange  [ expr [ lindex $range 0] -1.0 ]
    set highrange [ expr [ lindex $range 1] +1.0 ]

    # Set Segmentation Algorithm Parameters 
    if { $initialized == 1 } {
	$thisparam($this,gui_lowthreshold)  configure -from $lowrange -to $highrange
	$thisparam($this,gui_highthreshold) configure -from $lowrange -to $highrange
    }
    
    set thisparam($this,lowthreshold)  [ expr  0.05*($highrange-$lowrange) + $lowrange ] 
    set thisparam($this,highthreshold)  $highrange 

}
# -------------------------------------------------------------------------------------------
itcl::body pxitclbrainstrip::RequestUpdatedSeed { { force 1 }  } {

    if { $parent == 0 } { return 0 }
    if { $force == 0 &&  $thisparam($this,seedautoupdate) == 0 } {
	return
    }

    set done 0

    set currentpoint [ $parent cget -currentpoint ]
    scan $currentpoint "%f %f %f " thisparam($this,seedx) thisparam($this,seedy) thisparam($this,seedz)
    
    set dim2 [ [ $currentimage GetImage ] GetDimensions ]
    set indlist [ list x y z ]
    for { set i 0 } { $i <=2 } { incr i } {
	set a [ lindex $indlist $i ]
	set thisparam($this,seed$a) [ expr round ($thisparam($this,seed$a)) ]
	if { $thisparam($this,seed$a) < 0 } {
	    set thisparam($this,seed$a) 0
	} elseif { $thisparam($this,seed$a) >= [ lindex $dim2 $i ] } {
	    set thisparam($this,seed$a) [ lindex $dim2 $i ]
	}

    }


    return 1
}
# ----------------------------------------------------------------------------
itcl::body pxitclbrainstrip::EnsureCurrentImageIsSaved { operation } {


    set exists [ $currentimage DoesImageExistOnDisk ]
    if { $exists == 0 } {
	set ok [ pxtclutil::Question "The current image has not been saved\n Would you like to save the current image\n (No Cancels $operation)" ]
	if { $ok == 0 } {
		return 0
	    }
	$currentimage Save
    }

    set exists [ $currentimage DoesImageExistOnDisk ]
    if { $exists == 0 } {
	set ok [ pxtclutil::Warning "The current image has not been saved\n as $fname does not exist!\n Canceling $operation!" ]
	return 0
    }

    set fname [ $currentimage cget -filename ]
    set outname $fname
    set ext [ file extension $outname ]
    if { $ext == ".gz" } {
	set outname [ file rootname $outname ]
	set ext [ file extension $outname ]
    }
    if { $ext == ".nii" || $ext == ".hdr"  } {
	set outname [ file rootname $outname ]
    }
    
    return  [ list $fname $outname ]
}

itcl::body pxitclbrainstrip::RunBET { mode } {
    
    set lst [ $this EnsureCurrentImageIsSaved "Brain Segmentation" ]
    if {  [ lindex $lst 0 ] == 0 } {
	return 0
    }
    
    set fname [ lindex $lst 0 ]
    set outname [ lindex $lst 1 ]

    set spa [ [ $currentimage GetImage ] GetSpacing ]
    set ori [ [ $currentimage GetImage ] GetOrigin ]

    RequestUpdatedSeed


    set p(0) [ expr $thisparam($this,seedx) *  [ lindex $spa 0 ] + [ lindex $ori 0 ] ] 
    set p(1) [ expr $thisparam($this,seedy) *  [ lindex $spa 1 ] + [ lindex $ori 1 ] ] 
    set p(2) [ expr $thisparam($this,seedz) *  [ lindex $spa 2 ] + [ lindex $ori 2 ] ] 

    set cmdline "$thisparam($this,fslbet) $fname ${outname}_stripped -v -o -f $thisparam($this,bet_fth) -g $thisparam($this,bet_gth)"

    if { $thisparam($this,bet_center) } {
	set cmdline "${cmdline} -c $p(0) $p(1) $p(2)"
    }

    if { $thisparam($this,bet_radius) > 0 } {
	set cmdline "$cmdline -r $thisparam($this,bet_radius)"
    }
 

    set thisparam($this,bet_outputroot) $outname
    $runcommandgui SetCommand  $cmdline
    $runcommandgui Show
    $runcommandgui SetCallback  "$this BETDone"
    if { $mode > 0 } {
	$runcommandgui Run "Stripping Brain"
    }
    

}




itcl::body pxitclbrainstrip::BETDone { } {
    
    set out $thisparam($this,bet_outputroot) 
    set outb ${out}_stripped
    set outo ${out}_stripped_overlay

    set img1 [ $thisparam($this,bet_braingui) GetObject ]
    $img1 Load ${outb}.hdr
    $this FixOriginSpacingOrientation $img1
    $thisparam($this,bet_braingui)  Update

    set img2 [ $thisparam($this,bet_overlaygui) GetObject ]
    $img2 Load ${outo}.hdr
    $this FixOriginSpacingOrientation $img2
    $thisparam($this,bet_overlaygui) Update

    # This is a temporary file -- delete it 
    catch { file delete ${outo}.img }
    catch { file delete ${outo}.hdr }
    catch { file delete ${outb}.img }
    catch { file delete ${outb}.hdr }

    $parent SetResultsFromObject $img1 $this
    
    $this GenerateMaskFromBETOutput 0

    set cast [ vtkImageShiftScale [ pxvtable::vnewobj ]]
    $cast SetInput [ $img1 GetImage ]
    $cast SetScale 1.0
    $cast SetOutputScalarType [ [ $currentimage GetImage ] GetScalarType ] 
    $cast Update
    $img1 ShallowCopyImage [ $cast GetOutput ]
    $cast Delete

    set fname [ $currentimage cget -filename ]
    set ext [ file extension $fname ]
    set ext2 ""
    if { $ext == ".gz" } {
	set ext   [ file extension [ file rootname $fname ]]
	set fname [ file rootname $fname ]
	set ext2 ".gz"
    }
    if { $ext == ".nii" } {
	$img1 Save ${out}_stripped.nii${ext2}
	$img2 configure -filename ${out}_stripped_overlay.nii${ext2}
    } else {
        $img1 Save ${out}_stripped.hdr
	$img2 configure -filename ${out}_stripped_overlay.hdr
    }
    $thisparam($this,bet_braingui)  Update
    $thisparam($this,bet_overlaygui)  Update
    pxtclutil::Info "Brain Extraction Done\n (Mask for Morphology Updated Also)\n"

}

itcl::body pxitclbrainstrip::FixOriginSpacingOrientation { newimg } {

    set spacing  [ [ $currentimage GetImage ] GetSpacing ]
    set origin   [ [ $currentimage GetImage ] GetOrigin ]

    $newimg CopyImageHeader [ $currentimage GetImageHeader ]
    [ $newimg GetImage ] SetSpacing [ lindex $spacing 0 ] [ lindex $spacing 1 ] [ lindex $spacing 2 ]
    [ $newimg GetImage ] SetOrigin [ lindex $origin 0 ] [ lindex $origin 1 ] [ lindex $origin 2 ]
}


itcl::body pxitclbrainstrip::GenerateMaskFromBETOutput { mode } {
    

    set img1 [ $thisparam($this,bet_braingui) GetObject ]
    if { [ CheckImage $img1 "Brain Extraction Results" "Create Mask from BET Output" 1 ] == 0 } {
	return
    }


    WatchOn
    set thresholdF  [  vtkImageThreshold [ pxvtable::vnewobj ] ]
    $thresholdF ThresholdBetween  1 1000000
    $thresholdF SetInValue    1
    $thresholdF SetOutValue   0
    $thresholdF ReplaceInOn
    $thresholdF ReplaceOutOn
    $thresholdF SetInput [ $img1 GetImage ]
    $thresholdF SetOutputScalarTypeToUnsignedChar
    SetFilterCallbacks $thresholdF   "Thresholding brain image"
    $thresholdF Update

    $oldmask ShallowCopy $currentmask
    if { $mode == 0 } {
	$currentmask ShallowCopyImage  [ $thresholdF GetOutput ]
    }  else {
	ProcessMorphologyResults [ $thresholdF GetOutput ]
    }
    WatchOff
    catch { $thresholdF Delete }
    
    

}
# ----------------------------------------------------------------------------
itcl::body pxitclbrainstrip::RunFAST { mode  } {
    
    set lst [ $this EnsureCurrentImageIsSaved "Brain Segmentation" ]

    if {  $lst == 0 } {
	return 0
    }

    set fname [ lindex $lst 0 ]
    set outname [ lindex $lst 1 ]
    
    set imgtype 3
    if { $thisparam($this,fast_type) == "T2-Weighted" } {
	set imgtype 2
    } elseif { $thisparam($this,fast_type) == "T1-Weighted" } {
	set imgtype 1
    }
    set numc    $thisparam($this,fast_numclasses) 
    
    set cmdline "$thisparam($this,fslfast) -c $numc -t$imgtype -v 5 -or -op -od ${outname}_segm $fname |& tee "
    puts stderr $cmdline

    set thisparam($this,fast_outputroot) ${outname}_segm
    set thisparam($this,fast_outputclasses) $numc
    $runcommandgui SetCommand  $cmdline
    $runcommandgui Show
    $runcommandgui SetCallback  "$this FASTDone"
    if { $mode > 0 } {
	$runcommandgui Run "Segmenting Brain"
    }
    
}

itcl::body pxitclbrainstrip::FASTDone { } {
    
    set out  $thisparam($this,fast_outputroot) 
    set img2 [ $thisparam($this,fast_segmentedimagegui) GetObject ]
   
    set outo ${out}_seg
    	
    if { [ file exists ${outo}.hdr ] } {
	$img2 Load ${outo}.hdr
    } else { 
	tk_messageBox -type ok -title Error! -message "Could not find $outo"
	return
    }

    set exp ${out}*
    foreach filename [glob $exp] {
	pxtkprint "deleting $filename \n"
	file delete $filename
    }
     
    $this FixOriginSpacingOrientation $img2
    set fname [ $currentimage cget -filename ]
    set ext [ file extension $fname ]
    set ext2 ""
    if { $ext == ".gz" } {
	set ext   [ file extension [ file rootname $fname ]]
	set fname [ file rootname $fname ]
	set ext2 ".gz"
    }
    if { $ext == ".nii" } {
	$img2 Save ${out}seg_classification.nii${ext2}
    } else {
        $img2 Save ${out}seg_classification.hdr
    }
    
    $thisparam($this,fast_segmentedimagegui) Update
    pxtclutil::Info "Brain Segmentation Done\n"

    $parent SetResultsFromObject $img2 $this
}


itcl::body pxitclbrainstrip::FASTStatistics { mode } {

    set img $currentimage
    if { [ $currentimage GetImageSize ] < 2 } {
	::pxtclutil::Warning "No image  in memory! Cannot compute Gray/White statistics!"
	return 0 
    }

    set img1 [ $thisparam($this,fast_segmentedimagegui) GetObject ]
    if { [ CheckImage $img1 "Brain Segmentation Results" "Generate Gray/White Statistics" 1 ] == 0 } {
	return 0
    }

    set results [ vtkFloatArray [ pxvtable::vnewobj ]]
    set util    [ vtkpxHistogramSegmentation [ pxvtable::vnewobj ]]
    $util ComputeSegmentedMapStatistics [ $img GetImage ] [ $img1 GetImage ] $results 

    pxtkconsole
    pxtkprint "\n ---------------------------------------------------------\n"
    pxtkprint "Current Image Statistics [ file tail [ $currentimage cget -filename ]]\n\n"

    set numcomp [  [ $currentimage GetImage ] GetNumberOfScalarComponents ]
    set numlabels [ $results GetNumberOfTuples ]
    
    for { set c 0 } { $c < $numcomp } { incr c } {
	if { $numcomp > 1 } {
	    pxtkprint "Frame [ expr $c +1 ]\n"
	}
	set indexv [ expr $c*4 ]
	set indexm [ expr $c*4+2 ]
	set indexs [ expr $c*4+3 ]

	for { set l 0 } { $l < $numlabels } { incr l } {
	    pxtkprint [ format "label = %2d  volume=%.2f cc  mean=%.2f  std=%.2f\n" [ expr $l +1 ]  [ expr [ $results GetComponent $l $indexv ]*0.001 ] [ $results GetComponent $l $indexm ] [ $results GetComponent $l $indexs ] ]
	}
    }
    $util Delete

    if { $mode == "cleanup" } {
        $results Delete
	return 0
    }

    return $results

}

itcl::body pxitclbrainstrip::FASTNormalizeBrain { } {

    set results [ FASTStatistics "test" ]
    if { $results == 0 } {
	return 0
    }

    puts stdout "Onto normalizing brain intensities ...."

    set numcomp   [ $results GetNumberOfComponents ]
    set numlabels [ $results GetNumberOfTuples ]

    puts stdout "Numcomp=$numcomp  numlabels=$numlabels "

    if { $numcomp == 4 && $numlabels == 3 } {

	set csf_m   [ $results GetComponent 1 2 ]
	set csf_s   [ $results GetComponent 1 3 ]
	set gray_m  [ $results GetComponent 2 2 ]
	set gray_s  [ $results GetComponent 2 3 ]
	set white_m [ $results GetComponent 3 2 ]
	set white_s [ $results GetComponent 3 3 ]

	set r  [ [ [ [$currentimage GetImage ] GetPointData ] GetScalars ] GetRange ]

	set upper [ expr $white_m + 2.0*$white_s ]
        if { $upper > [ lindex $r 1 ] } {
	    ::pxtclutil::Info "Image is well adjusted no need to normalize it\n"
	} else {
	    set scalefactor [ expr 200.0/$gray_m ]
	    set sc_upper    [ expr $scalefactor * $upper ]

	    pxtkprint "Scalefactor = $scalefactor = (200/$gray_m) scaled_upper=$sc_upper "

	    set map [ vtkDouble [ pxvtable::vnewobj ]]
	    $map SetNumberOfComponents 3
	    $map SetNumberOfTuples 3

	    $map SetComponent 0 0 [ expr $gray_m * 0.5 ]
	    $map SetComponent 0 1 0.0
	    $map SetComponent 0 2 0.0
    
	    $map SetComponent 1 0 $upper
	    $map SetComponent 1 1 0.0
	    $map SetComponent 1 2 $scalefactor 

	    $map SetComponent 2 0 [ expr [ lindex $r 1 ] + 10 ] 
	    $map SetComponent 2 1 $sc_upper
	    $map SetComponent 2 2 0.0

	    for { set i 0 } { $i <= 2 } { incr i } {
		puts -nonewline stdout "Row $i:"
		for { set j 0 } { $j <= 2 } { incr j } {
		    puts -nonewline stdout "[ $map GetComponent $i $j ],"
		}
		puts stdout ""
	    }

	    set sh  [ vtkpxImageAlgebra [ pxvtable::vnewobj ]  ]
	    $sh SetInput [ $currentimage GetImage ]
	    $sh SetMap $map
	    $sh SetOutputScalarTypeToShort
	    SetFilterCallbacks $sh "Normalizing Image"
	    $sh Update
    
	    pxtkprint "Saturating at $upper"

	    $currentresults ShallowCopyImage [ $sh GetOutput ] 
	    ProcessImageResults  "Norm" 
	    catch { $sh Delete }
	    $map Delete
	    WatchOff
	}
    } else {
	::pxtclutil::Warning "Failed to Normalize Brain using FAST Output"
    }

    $results Delete
    
    return 1
}

# ----------------------------------------------------------------------------
itcl::body pxitclbrainstrip::AddToMenuButton { mb args } {

    global pxtcl_pref_array

    eval "$mb add command -command {$this ShowWindow \"Math_Morphology\" } -label \"Math_Morphology\" "
    eval "$mb add command -command {$this ShowWindow \"Histogram Segmentation\" } -label \"Histogram Segmentation\" "
    eval "$mb add command -command {$this ShowWindow \"Bias Field Correction\" } -label \"Bias Field Correction\" "
    if { $enable_mousemode == 1 } {
	eval "$mb add command -command {$this ShowWindow \"Exponential Fit\"} -label \"Exponential Fit\" "
    }
    
    if { $enable_fsl == 1 } {

	if { $thisparam($this,fslbet) !=  ""  ||  $thisparam($this,fslfast) !=  "" } {
	    $mb add separator
	    if { $thisparam($this,fslbet) !=  "" } {
		eval "$mb add command -command {$this ShowWindow \"Brain Extraction Tool\" } -label \"Brain Extraction Tool\"  " 
	    }
	    if { $thisparam($this,fslfast) !=  "" } {
		eval "$mb add command -command {$this ShowWindow \"Gray/White Segmentation\" } -label \"Gray/White Segmentation\"  "
	    }
	} else {
	    $mb add separator
	    eval "$mb add command -command { pxtclutil::Info \"FSL installation not found, make sure bet or bet2 and fast are in your path.\" } -label \"FSL Not Found!\"  " 
	}
    }
    
    if { $has_singlelevelset == 1 } {
	$mb add separator
	eval "$mb add command -command {$this ShowWindow \"Levelset Segmentation\" } -label \"Levelset Segmentation\" "
    }
}

