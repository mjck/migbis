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

package provide pxitclexpfit 1.0

# 	$Id: pxitclexpfit.tcl,v 1.0 2005/10/18 21:25:14 xenios Exp xenios $	





package require  labelframe 1.0
package require  pxtclutil  1.0
package require  pxvtable   1.0
package require vtkpxcontrib  1.1
package require pxitclbaseimagecontrol   1.0


itcl::class pxitclexpfit {

    # Base Class
    inherit pxitclbaseimagecontrol 

    protected common thisparam

    protected variable vtk_viewer 0

    constructor { par } {
	::pxitclbaseimagecontrol::constructor $par
    } {
	InitializeExpfit
    }

    # Initialize and Set Up
    public method InitializeExpfit { } 
    public method Initialize { inpwidg }
    public method DelayedInitialize {  }

    # ----------------
    # Create GUI Stuff
    # ----------------
    public method CreateFitControl { parent }
    public method CreateFitOutputControl { parent }
    public method CreateSegmentationControl { parent }

    # ------------------------------
    # OOP Override
    # ------------------------------
    public    method SetImage { img } 
    
    # ----------------------------
    # Real Code
    # ----------------------------
    public method RunExpFit { }
    public method RunExpFitT1 { }
    public method RunExpFitSegmentation { }
    public method RunOldStyleExpFitSegmentation { }


    public method GenTimePoints { }
    public method ValidateTimePoints { }
    public method CreateTimePointArray { }
    public method LoadEchoList { }
    public method SaveEchoList { }


    # -----------------------------
    # Morphology Operations
    # -----------------------------
    public method CheckImage {  image name operation verbosity } 

    # ----------------------------
    public method AddToMenuButton { mb args } 
}

# ----------------------------------------------------------------------------------------------
itcl::body pxitclexpfit::InitializeExpfit {  } {

    # ------------------------------------------------------------
    #  Multi-echo Stuff
    # ------------------------------------------------------------
    set thisparam($this,expfit_timepointlist)  "15.0 30.0 45.0 60.0 75.0 90.0"
    set thisparam($this,expfit_firsttimepoint)  15.0
    set thisparam($this,expfit_dtimepoint)      15.0
    set thisparam($this,expfit_maxt2)     250
    set thisparam($this,expfit_thr)       0.02
    set thisparam($this,expfit_noisesigma)       300
    set thisparam($this,expfit_median)       0
    set thisparam($this,expfit_imagenoise)  150
    
    set thisparam($this,expfit_densitygui) 0
    set thisparam($this,expfit_t2mapgui) 0
    set thisparam($this,expfit_errorgui) 0
    set thisparam($this,expfit_t2mode) 1
    set thisparam($this,segm_outputgui) 0
    set thisparam($this,segm_outputgui2) 0

    # -------------------------------------------------------------------

    set thisparam($this,histo_numclasses) 3
    set thisparam($this,histo_sigmaratio) 0.2

    set thisparam($this,histo_iterations) 50
    set thisparam($this,histo_numbins)   256
    set thisparam($this,histo_epsilon)    0.05
    set thisparam($this,histo_sigma) 0.05

    
    set thisparam($this,mrf_iterations)   10
    set thisparam($this,mrf_smoothness)  1.0
    set thisparam($this,mrf_convergence) 0.25
    set thisparam($this,mrf_keychannel)    1
    set thisparam($this,mrf_fuzzy)         0

    set thisparam($this,mrf_dobiasfield) 0
    set thisparam($this,mrf_biasfieldsigma) 5.0



}
# -------------------------------------------------------------------------------

itcl::body pxitclexpfit::Initialize { inpwidg  } {

    if { $initialized == 1 } { return $basewidget } 

    set basewidget $inpwidg
    toplevel $basewidget ; wm geometry $basewidget 600x450; wm withdraw $basewidget


    SetTitle "Exponential Fit Control"
    set initialized 1

    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget
}

itcl::body pxitclexpfit::DelayedInitialize {  } {

    if { $fullyinitialized == 1 } {
	return
    }

    set notebook $basewidget.notebook
    iwidgets::tabnotebook $notebook -width 5i -height 3i -tabpos w


    CreateFitControl  [ $notebook add -label "Fitting" ]
    CreateFitOutputControl  [ $notebook add -label "Fit. Output" ]

    set segm 0
    catch { 
	set segm [ vtkpxMonoExponentialSegmentation  [ pxvtable::vnewobj ]]
	$segm Delete
    }

    if { $segm != 0 } {
	CreateSegmentationControl  [ $notebook add -label "Segmentation" ]
    }

    set w  [ frame $basewidget.buttonbar  ]
    pack $w -side bottom -fill x -expand f
    pack $notebook -side top -fill both -expand true

    set canvas [ [ $notebook component tabset ] component canvas ];     set l [ AddLogoLabel $canvas ];     pack $l -side bottom -padx 1 

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
#    GUI Stuff
# ------------------------------------------------------------------------------------------------


itcl::body pxitclexpfit::CreateSegmentationControl { base } {
    set thisparam($this,segm_outputgui2) [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $thisparam($this,segm_outputgui2) configure -description "Segmentation Output 2"
    $thisparam($this,segm_outputgui2) Initialize $base.out2
    $thisparam($this,segm_outputgui2) AddFunction "$parent SetResultsFromObject" "Display" "$this"

    set thisparam($this,segm_outputgui) [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $thisparam($this,segm_outputgui) configure -description "Segmentation Output"
    $thisparam($this,segm_outputgui) Initialize $base.out
    $thisparam($this,segm_outputgui) AddFunction "$parent SetResultsFromObject" "Display" "$this"

    pack $base.out2 $base.out -side bottom -expand false -fill x

    set nt [ iwidgets::tabnotebook $base.nt -tabpos s -angle 45 -margin 2 -backdrop gray -raiseselect true ]
    pack $nt -side top -expand t -fill both

    set topbar  [ $nt add -label Standard   ]
    set topbar2 [ $nt add -label Experimental ]
    $topbar2 configure -bg darkgreen
    $nt view "Standard"

    set mainbar  [ frame $topbar.1 ]
    set mrfbar   [ frame $topbar.2 ]
    set bbar     [ frame $topbar.3 ]

    set sbar     [ frame $topbar2.1 ]
    set sbar2   [ frame $topbar2.2 ]

    pack $mainbar $mrfbar  $bbar -side top -expand false -fill x -pady 0
    pack  $sbar $sbar2 -side top -expand false -fill x -pady 0

    # ----------------------- Common ---------------------------------------------
    label         $mainbar.1  -text "Number of Classes:"
    tk_optionMenu $mainbar.2 [ itcl::scope thisparam($this,histo_numclasses) ] "1" "2" "3" "4" "5" "6" "7"
    iwidgets::entryfield $mainbar.3 -labeltext "sigmaratio:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,histo_sigmaratio) ] -relief sunken 
    pack $mainbar.1 $mainbar.2 $mainbar.3 -side left -expand t -fill x -padx 2


    iwidgets::entryfield $mrfbar.1 -labeltext "Iterations"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,mrf_iterations) ] -relief sunken 
    iwidgets::entryfield $mrfbar.2 -labeltext "Smoothness:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,mrf_smoothness) ] -relief sunken 
    iwidgets::entryfield $mrfbar.3 -labeltext "Convergence:"  -width 5 -validate real -textvariable [ itcl::scope thisparam($this,mrf_convergence) ] -relief sunken 
    iwidgets::entryfield $mrfbar.4 -labeltext "Channel:"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,mrf_keychannel) ] -relief sunken 
    pack $mrfbar.1 $mrfbar.2 $mrfbar.3 $mrfbar.4  -side left -expand t -fill x  -padx 2


    # -------------------
    # MRF Bar
    # -------------------

    eval "button $bbar.0 -text \"Do Exponential Segmentation\"  -command { $this RunExpFitSegmentation  }"
    pack $bbar.0   -side left -fill x -expand false -padx 10 -pady 1

    # -------------------------- Experimental ------------------------------------

    iwidgets::entryfield $sbar.1 -labeltext "KMeans: Bins:"  -width 3 -validate integer -textvariable [ itcl::scope thisparam($this,histo_numbins) ] -relief sunken 
    iwidgets::entryfield $sbar.2 -labeltext "Iterations:"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,histo_iterations) ] -relief sunken 
    iwidgets::entryfield $sbar.3 -labeltext "Sigma:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,histo_sigma) ] -relief sunken 
    iwidgets::entryfield $sbar.4 -labeltext "Convergence:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,histo_epsilon) ] -relief sunken 
    pack  $sbar.1 $sbar.2 $sbar.3 $sbar.4 -side left -expand t -fill x  -padx 0

    label                $sbar2.0 -text "Bias Field Correction Parameters:"
    checkbutton          $sbar2.1 -variable [ itcl::scope thisparam($this,mrf_dobiasfield) ] -text "B1 Correction"
    iwidgets::entryfield $sbar2.2 -labeltext "B1 Sigma:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,mrf_biasfieldsigma) ] -relief sunken 
    pack $sbar2.0 $sbar2.1 $sbar2.2  -side left -expand t -fill x  -padx 2
    # ---------------------------------------------------------------------------------

    ::pxitclbasecontrol::SetWidgetTreeColors $topbar2 "\#aaabd7" "black"

}

itcl::body pxitclexpfit::CreateFitControl { base } {

    #    $base configure -bg green

    set   tbar [ LabelFrame:create $base.bar1  -width 300  -text "Time Course" ]
    set   obar [ LabelFrame:create $base.bar2  -width 300  -text "Fitting Operations" ]
    pack $base.bar1 $base.bar2 -side top -pady 4 -padx 2 -expand t -fill both
    
    set   midbar    [ frame $obar.1 ] 
    set   bbar      [ frame $obar.2 ]
    pack $midbar $bbar -side top -expand f -fill x -pady 1

    set   midbar1   [ frame $tbar.1 ]
    set   midbar2   [ frame $tbar.2 ]
    set   midbar3   [ frame $tbar.3 ]


    pack   $midbar1 $midbar2  $midbar3   -side top -expand false -fill x  -pady 0 -padx 2
    

    iwidgets::entryfield $midbar1.4 -labeltext "First T:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,expfit_firsttimepoint) ] -relief sunken
    iwidgets::entryfield $midbar1.5 -labeltext "Dt:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,expfit_dtimepoint) ] -relief sunken

    eval "button $midbar1.6 -text \"Generate\"  -command { $this GenTimePoints   }"
    pack $midbar1.4 $midbar1.5 $midbar1.6  -side left -expand t -fill x 

    iwidgets::entryfield $midbar2.0 -labeltext "Time Points:"  -width 30  -textvariable [ itcl::scope thisparam($this,expfit_timepointlist) ] -relief sunken
    eval "button $midbar2.1 -text \"Validate\"  -command { $this ValidateTimePoints   }"
    pack $midbar2.1 -side right -expand f -pady 2
    pack $midbar2.0 -side left -expand t -fill x 


    eval "button $midbar3.0 -text \" Load Timecourse\"  -command { $this LoadEchoList }"
    eval "button $midbar3.1 -text \" Save Timecourse\"  -command { $this SaveEchoList }"
    pack $midbar3.0 $midbar3.1 -side left -padx 5 -pady 1
    


    # Button Bar
    # ----------
    checkbutton   $midbar.0 -variable [ itcl::scope thisparam($this,expfit_t2mode) ] -text "Use T2" 
    iwidgets::entryfield $midbar.1 -labeltext "Max T2"  -width 4 -validate integer -textvariable [ itcl::scope thisparam($this,expfit_maxt2) ] -relief sunken
    iwidgets::entryfield $midbar.2 -labeltext "Threshold"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,expfit_thr) ] -relief sunken
    iwidgets::entryfield $midbar.3 -labeltext "NoiseSigma"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,expfit_noisesigma) ] -relief sunken

    checkbutton   $midbar.4 -variable [ itcl::scope thisparam($this,expfit_median) ] -text "Robust Fit"

    pack $midbar.0 $midbar.1 $midbar.2 $midbar.3   -side left -expand t -fill x 

    eval "button $bbar.0 -text \" Fit T2\"  -command { $this RunExpFit   }"

    set fit 0
    catch { 
	set fit [ vtkpxExponentialFitT1  [ pxvtable::vnewobj ]]
	$fit Delete
    }

    if { $fit !=0 } {
	eval "button $bbar.1 -text \" Fit T1\"  -command { $this RunExpFitT1 }"
	pack $bbar.0 $bbar.1   -side left -fill x -expand t -padx 10 -pady 2
    } else {
	pack $bbar.0  -side left -fill x -expand t -padx 10 -pady 2
    }
    
 
}

# ----------------------------------------------------------------------------------------

itcl::body pxitclexpfit::CreateFitOutputControl { base } {

    label $base.o -bg black -fg white -text "Output of Fitting Operations"
    set   outputbar [ frame $base.4 ]
    pack $base.o -side top -expand f -fill x -pady 2
    pack $outputbar -side bottom -expand t -fill both

    set thisparam($this,expfit_densitygui) [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $thisparam($this,expfit_densitygui) configure -description "Density Image"
    $thisparam($this,expfit_densitygui) InitializeLite $outputbar.1 1
    $thisparam($this,expfit_densitygui) AddFunction "$parent SetResultsFromObject" "Display" "$this"

    set thisparam($this,expfit_t2mapgui) [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $thisparam($this,expfit_t2mapgui) configure -description "Relaxation Time Image"
    $thisparam($this,expfit_t2mapgui) InitializeLite $outputbar.2 1
    $thisparam($this,expfit_t2mapgui) AddFunction "$parent SetResultsFromObject" "Display" "$this"

    set thisparam($this,expfit_errorgui) [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $thisparam($this,expfit_errorgui) configure -description "Error"
    $thisparam($this,expfit_errorgui) InitializeLite $outputbar.3 1
    $thisparam($this,expfit_errorgui) AddFunction "$parent SetResultsFromObject" "Display" "$this"

    pack $outputbar.1 $outputbar.2 $outputbar.3 -side top -pady 0 -ipady 0 -padx 2
}

# ----------------------------------------------------------------------------
#  Morphology Stuff 
# ----------------------------------------------------------------------------
itcl::body pxitclexpfit::CheckImage {  image name operation verbosity } {
    
    set ok 0
    
    catch { if { [ $image GetImageSize ] > 1 } {  set ok 1 }   }
    
    if { $verbosity >0 && $ok == 0 } {
	::pxtclutil::Warning "No $name in Memory\n Cannot perform $operation !"
    }

    return $ok
}

# ------------------------------------------------------------------------------------------------
#   GUI Support
# ------------------------------------------------------------------------------------------------
itcl::body pxitclexpfit::GenTimePoints {  } {

    set nt [ [ $currentimage GetImage  ] GetNumberOfScalarComponents ]
    if { $nt < 2 } {
    	::pxtclutil::Warning "No image in Memory or currentimage has less than 2 frames\n Cannot generate valid timepoint train !"
    }

    set t0 $thisparam($this,expfit_firsttimepoint)  
    set dt $thisparam($this,expfit_dtimepoint)     
    

    set thisparam($this,expfit_timepointlist)  ""

    set t $t0
    for { set i 0 } { $i < $nt } { incr i } {
	lappend thisparam($this,expfit_timepointlist) $t
	set t [ expr $t + $dt ]
    }
}

itcl::body pxitclexpfit::ValidateTimePoints {  } {

    set nt [ [ $currentimage GetImage  ] GetNumberOfScalarComponents ]
    if { $nt < 2 } {
    	::pxtclutil::Warning "No image in Memory or currentimage has less than 2 frames\n Cannot validate timepoint train !"
    }

    set qlist "" 
    foreach n $thisparam($this,expfit_timepointlist) {
	catch { lappend qlist [ expr abs($n) ] }
    }

    set thisparam($this,expfit_timepointlist) [ lsort -real $qlist ]

    set np [ llength $thisparam($this,expfit_timepointlist) ]
    if { $np < $nt } {
	::pxtclutil::Warning "$np timepoints specified, need at least $nt to fit this image"
    } else {
	::pxtclutil::Info "Time Point List OK"
    }
	
}

itcl::body pxitclexpfit::CreateTimePointArray { } {

    set fl [ vtkFloatArray [ pxvtable::vnewobj ]]
    set vlist $thisparam($this,expfit_timepointlist)
    $fl SetNumberOfTuples  [ llength $vlist ]

    for { set i 0 } { $i < [ llength $vlist ] } { incr i } {
	$fl SetComponent $i 0 [ lindex $vlist $i ]
    }

    return $fl
}


itcl::body pxitclexpfit::LoadEchoList { } {

    set fname [tk_getOpenFile -title "Filename"   -filetypes { {"Echo List Text Files" {.txt}}}]
    if { [ llength $fname ] > 0 } {
	if { [ file exists $fname ] > 0 } {
	    set fileid [open $fname r ]
	    if { $fileid > 0 } {
		gets $fileid line
		if { $line == "\#Echo List File"  } {
		    gets $fileid line
		    set thisparam($this,expfit_timepointlist) $line
		    close $fileid
		} else {
		    pxtclutil::Warning "Failed to Read Echo List from $fname"
		}
	    }
	}
    }
}
    
    
itcl::body pxitclexpfit::SaveEchoList { } {

    set lst $thisparam($this,expfit_timepointlist)
    set fname [tk_getSaveFile -title "Filename" -defaultextension .txt  -filetypes { {"Echo List Text Files" {.txt}}}]
    if { [ llength $fname ] > 0 } {
	set fileid [open $fname w]
	puts $fileid "\#Echo List File"
	puts $fileid $lst
	close $fileid
	pxtclutil::Info "Echo List Saved in $fname"
    }
}



# ------------------------------------------------------------------------------------------------
#    Fitting Code
# ------------------------------------------------------------------------------------------------


itcl::body pxitclexpfit::RunExpFit {  } {


    if { [ CheckImage $currentimage "Image" "Mono-exponential Fitting" 1 ] == 0 } { 
	return 0  
    }

    set tarray [ $this CreateTimePointArray ]    
    set nc [ [ $currentimage GetImage ]  GetNumberOfScalarComponents ]
    set np [ $tarray  GetNumberOfTuples ]
    if { $nc < 3 || $np < $nc } {
	::pxtclutil::Warning "Images has too few ($nc) components for mono-exponential fitting or not enough time array values ($np < $nc) !"
	$tarray Delete
	return 0
    }

    WatchOn    



    pxtkprint ".... Monoexponential Fitting\n"
    set fit [ vtkpxLogExponentialFit  [ pxvtable::vnewobj ]]
    $fit SetInput [ $currentimage GetImage ] 
    $fit SetEchoTrain $tarray
    $fit SetMaxT2Value $thisparam($this,expfit_maxt2)
    $fit SetMedianFit $thisparam($this,expfit_median)
    $fit SetIntensityThreshold $thisparam($this,expfit_thr)
    $fit SetImageNoiseSigma $thisparam($this,expfit_noisesigma)
    pxtkconsole

    SetFilterCallbacks $fit "Mono-exponential fitting"
    $fit Update

    set densityimage [ $thisparam($this,expfit_densitygui) GetObject ]
    $densityimage  ShallowCopyImage [ $fit GetDensityMap ]
    $densityimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "den" ]
    $densityimage  CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,expfit_densitygui) Update


    set t2mapimage [ $thisparam($this,expfit_t2mapgui) GetObject ]
    if { $thisparam($this,expfit_t2mode) == 1 } {
	$t2mapimage  ShallowCopyImage [ $fit GetOutput ]
	$t2mapimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "t2" ]
    } else {
	$t2mapimage  ShallowCopyImage [ $fit GetR2Map ]
	$t2mapimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "r2" ]
    }
    $t2mapimage  CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,expfit_t2mapgui) Update

    set errorimage [ $thisparam($this,expfit_errorgui) GetObject ]
    $errorimage  ShallowCopyImage [ $fit GetErrorMap ]
    $errorimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "err" ]
    $errorimage  CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,expfit_errorgui) Update

    $fit Delete

    WatchOff

    $tarray Delete

    $parent SetResultsFromObject $t2mapimage $this
}

# -------------------------------------------------------------------------------------------

itcl::body pxitclexpfit::RunExpFitT1 {  } {


    if { [ CheckImage $currentimage "Image" "T1-exponential Fitting" 1 ] == 0 } { 
	return 0  
    }
    
    set tarray [ $this CreateTimePointArray ]    
    set nc [ [ $currentimage GetImage ]  GetNumberOfScalarComponents ]
    set np [ $tarray  GetNumberOfTuples ]
    if { $nc < 3 || $np < $nc } {
	::pxtclutil::Warning "Images has too few ($nc) components for mono-exponential fitting or not enough time array values ($np < $nc) !"
	$tarray Delete
	return 0
    }


    WatchOn    

    pxtkprint ".... Monoexponential Fitting\n"
    set fit [ vtkpxExponentialFitT1  [ pxvtable::vnewobj ]]

    $fit SetInput [ $currentimage GetImage ] 
    $fit SetIntensityFractionalThreshold $thisparam($this,expfit_thr)
    $fit SetThreeParameterFit $thisparam($this,expfit_median)
    $fit SetRelaxationTimes  $tarray
    pxtkconsole

    SetFilterCallbacks $fit "T1 Mono-exponential fitting"
    $fit Update

    set densityimage [ $thisparam($this,expfit_densitygui) GetObject ]
    $densityimage  ShallowCopyImage [ $fit GetDensityMap ]
    $densityimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "den" ]
    $densityimage  CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,expfit_densitygui) Update


    set t2mapimage [ $thisparam($this,expfit_t2mapgui) GetObject ]
    $t2mapimage  ShallowCopyImage [ $fit GetOutput ]
    $t2mapimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "t1" ]
    $t2mapimage  CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,expfit_t2mapgui) Update

    set errorimage [ $thisparam($this,expfit_errorgui) GetObject ]
    $errorimage  ShallowCopyImage [ $fit GetQMap ]
    $errorimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "qmap" ]
    $errorimage  CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,expfit_errorgui) Update

    $fit Delete
    WatchOff

    $tarray Delete
    $parent SetResultsFromObject $t2mapimage $this
}

# -------------------------------------------------------------------------------------------


itcl::body pxitclexpfit::RunExpFitSegmentation {  } {


    if { [ CheckImage $currentimage "Image" "Mono-exponential Fat Segmentation" 1 ] == 0 } { 
	return 0  
    }
    
    set tarray [ $this CreateTimePointArray ]    
    set nc [ [ $currentimage GetImage ]  GetNumberOfScalarComponents ]
    set np [ $tarray  GetNumberOfTuples ]
    if { $nc < 4 || $np < $nc } {
	::pxtclutil::Warning "Images has too few ($nc) components for mono-exponential fitting or not enough time array values ($np < $nc) !"
	$tarray Delete
	return 0
    }


    WatchOn    

    pxtkprint ".... Monoexponential Segmentation \n"

    set segm [ vtkpxMonoExponentialSegmentation  [ pxvtable::vnewobj ]]
    $segm SetInput [ $currentimage GetImage ] 

    $segm SetFuzzyClassification $thisparam($this,mrf_fuzzy) 
    $segm SetDistributionModel 0
    $segm SetNumberOfClasses $thisparam($this,histo_numclasses)
    $segm SetIterations $thisparam($this,histo_iterations)
    $segm SetNumberOfBins $thisparam($this,histo_numbins)
    $segm SetConvergence $thisparam($this,histo_epsilon)
    $segm SetSigma $thisparam($this,histo_sigma)
    $segm SetOutlierDetection 0
    $segm SetMaxSigmaRatio      $thisparam($this,histo_sigmaratio)
    $segm SetMRFIterations $thisparam($this,mrf_iterations)
    $segm SetSmoothness $thisparam($this,mrf_smoothness)
    $segm SetMRFConvergencePercentage $thisparam($this,mrf_convergence)
    $segm SetImageNoiseSigma $thisparam($this,expfit_imagenoise)
    $segm SetTEArray  $tarray
    $segm SetComputeBiasField  $thisparam($this,mrf_dobiasfield)
    $segm SetBiasFieldFactor $thisparam($this,mrf_biasfieldsigma)

    SetFilterCallbacks $segm   "Monoexponential MRF Segmentation of image"
    $segm SetDebugVoxel -1
    $segm DebugOff
    $segm Update

    pxtkconsole

    if {  $thisparam($this,mrf_dobiasfield) == 1 } {
	set densityimage [ $thisparam($this,expfit_densitygui) GetObject ]
	$densityimage  ShallowCopyImage [ $segm GetBiasField ]
	$densityimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "biasfield" ]
	$densityimage  CopyImageHeader [ $currentimage GetImageHeader ]
	$thisparam($this,expfit_densitygui) Update
    }
   
    set errorimage [ $thisparam($this,segm_outputgui2) GetObject ]
    $errorimage  ShallowCopyImage [ $segm GetMultiFrameOutput ]
    $errorimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "msegm" ]
    $errorimage  CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,segm_outputgui2) Update

    set outputimage [ $thisparam($this,segm_outputgui) GetObject ]
    $outputimage  ShallowCopyImage [ $segm GetOutput ]
    $outputimage configure -filename    [ AddPrefix [ $currentimage cget -filename  ] "segm" ]
    $outputimage  CopyImageHeader [ $currentimage GetImageHeader ]
    $thisparam($this,segm_outputgui) Update

    $segm Delete
    WatchOff

    $tarray Delete
    $parent SetResultsFromObject $outputimage $this
}
    
# -------------------------------------------------------------------------------------------
itcl::body pxitclexpfit::SetImage { img  } {
	
    DelayedInitialize
    
    $currentimage ShallowCopy $img
    $currentresults UnLoad
    $currentresults CopyImageHeader [ $currentimage GetImageHeader ]

}
# ----------------------------------------------------------------------------
itcl::body pxitclexpfit::AddToMenuButton { mb args } {
    eval "$mb add command -command {$this ShowWindow \"Fitting\" } -label \"Exp. Fitting\" "
    eval "$mb add command -command {$this ShowWindow \"Segmentation\" } -label \"Exp. Segmentation\" "
}

