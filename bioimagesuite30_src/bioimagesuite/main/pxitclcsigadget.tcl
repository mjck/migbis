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






package provide pxitclcsigadget 1.0

# 	$Id: pxitclcsigadget.tcl,v 1.2 2004/02/03 17:36:31 xenios Exp xenios $	

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]


package require  vtkpxcontrib 1.1
package require  labelframe   1.0
package require  pxtclutil    1.0
package require  pxvtable     1.0
package require  pxtclhelp    1.0
package require  Itcl         3.2
package require  Iwidgets     4.0
package require  pxitclimage            1.0
package require  pxitcltransform        1.0
package require  pxitclsubjectentry     1.0
package require  pxitclbaseimagecontrol 1.0
package require  pxitclbaseimageviewer  1.0
# ----------------------------------------------------------------------------------------------

itcl::class pxitclcsigadget {

    inherit pxitclbaseimagecontrol

    # Generic Stuff 
    private   common   thisparam        
    protected variable setupfilename   "subject.csi"
    

    # Subject Specific
    protected variable anat_control    0
    protected variable conv_control   0
    protected variable raw_control   0
    protected variable pmap_control   0
    protected variable refx_control    0
    protected variable smooth_control1  0
    protected variable smooth_control2  0


    protected variable second_viewer   0

    protected variable image_controllist ""
    protected variable xform_controllist ""

    protected variable overlaytool     0
    public    variable electrodetool   0

    constructor { par args } {
	pxitclbaseimagecontrol::constructor $par 
    } {

	if { [ llength $args] > 0  } {
	    set second_viewer [ lindex $args 0 ]
	}
	InitializeCsiGadget

	set overlaytool [ [ pxitcloverlaytoolplus \#auto $this ] GetThisPointer ]
	$overlaytool configure -disable_colormapchoice 1
    }

    private method InitializeCsiGadget { } 
    public method Initialize { widget }
    public method DelayedInitialize { }


    public method AddToMenuButton { mb args } 
    public method AddToMenuButtonLite { mb args } 
    public method SetResultsFromObject { img obj  } 
    public method SetLookupTable       { colormap }
    public method SetLookupTableMode   { index }
    
    # ----------------------------
    # Setup Stuff
    # ----------------------------
    public method LoadSetup { args } 
    public method SaveSetup { args } 
    public method ReadExtraInfo { fname } 
    protected method GetFileElement { fileid } 

    # ----------------------------
    # Computational Stuff
    # ----------------------------
    public method ComputeSmooth    { } 
    public method SampleElectrodes { args } 

    # ---------------------------
    # Subject Specific Callbacks
    # ---------------------------

    public method VerifyRegistrationCallback { mode args }
    public method VerifyRegistration { imgcontrol1 imgcontrol2 xformcontrol }

    protected method CreateImageControl     { widget }
    protected method CreateImportControl    { widget }
    protected method CreateTransformControl { widget }
    protected method CreateOverlayControl   { widget }
    protected method CreateSamplingControl     { widget }

    public    method SetOverlayToolInputs { } 

    public    method UpdateFromRegistration { mmvutil } 
    public    method UpdateFromRegistrationDone { mmvutil } 


    # -------------------------------
    # Import Stuff
    # -------------------------------
    public   method BrowseImport { mode } 
    public   method RunImport    { } 
    public   method ResetImport  { } 

}

# ---------------------------------------------------------------------------

itcl::body pxitclcsigadget::InitializeCsiGadget { } {
    

    set refx_control [ [  pxitcltransformGUI \#auto ] GetThisPointer ]
    $refx_control configure -description "Transformation Anatomical->CSI Conventional"

    set anat_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $anat_control configure -description "Anatomical (3D) Image"

    set conv_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $conv_control configure -description "Conventional Image"

    set raw_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $raw_control configure -description "Raw CSI Image"

    set pmap_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $pmap_control configure -description "PMAP CSI Image"

    set smooth_control1 [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $smooth_control1 configure -description "Normalized Smooth 1"

    set smooth_control2 [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $smooth_control2 configure -description "Normalized Smooth 2"

    lappend itclobjectlist $refx_control $anat_control $conv_control $raw_control   $pmap_control $smooth_control1 $smooth_control2

    set thisparam($this,overlayviewer) "Reference"
    if { $second_viewer !=0 } {
	set thisparam($this,overlayviewer) "Transform"
    }

    set thisparam($this,overlayanatomy) "3D Anatomical" 
    set thisparam($this,overlayfunction) "Raw-CSI"
    set thisparam($this,autosave) 1
    set thisparam($this,smooth_sigma) 2.0
    set thisparam($this,smooth_control) 1
    set thisparam($this,smooth_sigma1) 0.0
    set thisparam($this,smooth_sigma2) 0.0

    set thisparam($this,csi_categories) "row\tcolumn\tdelta\tpgm\tratio\tCh\tCr\tNAA"
    set thisparam($this,csi_scalefactors) "1000\t1000\t1000\t.001\t.001\t.001"
    set thisparam($this,autosavesetup) 1

    set thisparam($this,csi_importinplane) 1.0
    set thisparam($this,csi_importslice)   1.25
    set thisparam($this,csi_importscout) ""
    set thisparam($this,csi_outputstem) ""


    lappend image_controllist $anat_control $conv_control $raw_control $pmap_control $smooth_control1 $smooth_control2
    lappend xform_controllist $refx_control 
}
#--------------------------------------------------------------------------------------------
itcl::body pxitclcsigadget::SetResultsFromObject { img obj  } {

    if { [ $img GetImageSize ] < 2 } {	
	return 0    
    }
    
    if { $obj == $overlaytool && $parent !=0 } {

	if {$thisparam($this,overlayviewer) == "Transform" && $second_viewer !=0 } {
	    $second_viewer SetResultsFromObject $img $this
	} else {
	    $parent SetResultsFromObject $img $this
	}
    } else {
        SetResults $img
    }
    return 1
}

# ---------------------------------------------------------------------------
#  Lookup Table Stuff 
# ---------------------------------------------------------------------------

itcl::body pxitclcsigadget::SetLookupTable { colormap } {
    
    set v $parent
    if {$thisparam($this,overlayviewer) == "Transform" && $second_viewer !=0 } {
	set v $second_viewer
    }
    
    if { $v != 0 } {
	$v SetLookupTable $colormap
    }
}

itcl::body pxitclcsigadget::SetLookupTableMode { index } {

    set v $parent
    if {$thisparam($this,overlayviewer) == "Transform" && $second_viewer !=0 } {
	set v $second_viewer
    }
    
    if { $v != 0 } {
	$v SetLookupTableMode [ expr 0+$index ]
    }
}


# ---------------------------------------------------------------------------
itcl::body pxitclcsigadget::SaveSetup { args } { 

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
	set fname  [tk_getSaveFile -title "Saving Setup File" -filetypes { {"CSI-Setup File" {.csi}}} -initialfile $f1tail -initialdir $f1path ]
    }
    if { [ string length $fname ] > 0 } {

	set fileid [open $fname w]
	
	puts $fileid "#CSI Subject Description File"
	puts $fileid "#---------------------------------------------"
	puts $fileid "# Transformations"
	puts $fileid "#---------------------------------------------"
	for { set i 0 } { $i < [ llength $xform_controllist ] } { incr i } {
	    set tmp [ lindex $xform_controllist $i ]
	    puts $fileid "[ $tmp cget -description] : [ [ $tmp GetObject ] cget -filename ]"
	}

	puts $fileid "#---------------------------------------------"
	puts $fileid "# Images"
	puts $fileid "#---------------------------------------------"
	
	for { set i 0 } { $i < [ llength $image_controllist ] } { incr i } {
	    set tmp [ lindex $image_controllist $i ]
	    puts $fileid "[ $tmp cget -description] : [ [ $tmp GetObject ] cget -filename ]"
	}
	
	puts $fileid "#---------------------------------------------"
	puts $fileid "# Extra Information"
	puts $fileid "#---------------------------------------------"
	puts $fileid "Categories    : $thisparam($this,csi_categories)"
	puts $fileid "Scale Factors : $thisparam($this,csi_scalefactors)"
	puts $fileid "Smooth1       : $thisparam($this,smooth_sigma1)"
	puts $fileid "Smooth2       : $thisparam($this,smooth_sigma2)"

	close $fileid
	set setupfilename $fname
	SetTitle "CSI Tool $setupfilename"
    }
}
# ------------------------------------------------------------------------------------------
itcl::body pxitclcsigadget::GetFileElement { fileid } {

   gets $fileid line 
   set index [string first ":" $line]
   if { $index > 0 }  {
       set len   [string length $line]
       set value [string trim [string range $line [expr $index +1] [expr $len -1]]]
       return $value
   }
   return ""
}

itcl::body pxitclcsigadget::LoadSetup { args } { 

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
	
	set fname  [tk_getOpenFile -title "Setup File" -filetypes { {"Csi-Setup File" {.csi}}} -initialfile $f1tail -initialdir $f1path ]
    }

    if { [ string length $fname ] < 1 } {
	return
    }

    if { [ file readable $fname ] == 0 } {
	::pxtclutil::Warning "Cannot read setupfile file from $fname\n Bad File"
	return 0
    }

    WatchOn; update idletasks

    set fileid [open $fname r]
	
    gets $fileid line
    if { [ string compare $line "#CSI Subject Description File"  ] != 0 } {
	close fileid
	::pxtclutil::Warning "Cannot read setupfile file from $fname\n Bad File Type"
	return 0
    }

    gets $fileid line ;     gets $fileid line  ;   gets $fileid line

    for { set i 0 } { $i < [ llength $xform_controllist ] } { incr i } {
	set tmp [ lindex $xform_controllist $i ]
	set name [ GetFileElement $fileid ]
	if { [ string length $name ] > 0 } {
	    [ $tmp GetObject ] Load $name 
	    $tmp Update
	}
    }
	  
    gets $fileid line ;     gets $fileid line  ;   gets $fileid line

    for { set i 0 } { $i < [ llength $image_controllist ] } { incr i } {
	set tmp [ lindex $image_controllist $i ]
	set name [ GetFileElement $fileid ]
	if { [ string length $name ] > 0 } {
	    [ $tmp GetObject ] Load $name 
	    $tmp Update
	}
    }

    catch {
	gets $fileid line ;     gets $fileid line  ;   gets $fileid line
	set thisparam($this,csi_categories) [ GetFileElement $fileid ]
	set thisparam($this,csi_scalefactors) [ GetFileElement $fileid ]
	set thisparam($this,smooth_sigma1) [ GetFileElement $fileid ]
	set thisparam($this,smooth_sigma2) [ GetFileElement $fileid ]
    }
    close $fileid
    set setupfilename $fname

    SetTitle "CSI Tool $setupfilename"
    WatchOff
}

# ---------------------------------------------------------------------------
itcl::body pxitclcsigadget::BrowseImport { mode } { 

    set f1 $setupfilename
    set f1path [ file dirname $f1 ]
    
    if { $mode == "Raw CSI" || $mode == "CSI PMAP" } {
	set fname  [tk_getOpenFile -title "Reading $mode File" -filetypes { {"Raw Text File" {.txt}}}  -initialdir $f1path ]
	if { [ llength $fname ] > 0 } {

	    if { $mode == "Raw CSI" } {
		$thisparam($this,csi_importraw) insert end $fname
	    } else {
		$thisparam($this,csi_importpmap) insert end $fname
	    }
	}
    } elseif { $mode == "scout" } {
	set typelist { {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
	set fname  [tk_getOpenFile -title "Conventional/Scout Anatomical Image" -filetypes $typelist  -initialdir $f1path ]
	if { [ llength $fname ] > 0 } {
	    set thisparam($this,csi_importscout) $fname
	}
    } else {
	set fname  [tk_getSaveFile -title "Output Setup File" -filetypes { {"Setup File" {.csi}}}  -initialdir $f1path ]
	if { [ llength $fname ] > 0 } {
	    set thisparam($this,csi_outputstem) [ file root $fname ]
	}
    }
}
# ---------------------------------------------------------------------------
itcl::body pxitclcsigadget::RunImport { } {

    # Check that there are enough data first 


    if { [ file exists $thisparam($this,csi_importscout) ] == 0 } {
	::pxtclutil::Warning "No/or bad Scout Image specified\n"
	return 0
    }

    set np  [ $thisparam($this,csi_importraw)  index end ]
    set np2 [ $thisparam($this,csi_importpmap) index end ]
    if { $np <1 && $np2 <1 } {
	::pxtclutil::Warning "Must specifiy at least one csi slice\n"
	return 0
    }

    WatchOn
    #    puts stdout "Input Anatomical $thisparam($this,csi_importscout)"

    set ana [ vtkpxAnalyzeImageSource [ pxvtable::vnewobj ]]
    $ana DebugOn
    $ana  Load $thisparam($this,csi_importscout)
    $ana DebugOff

    set maxpass 1
    if { $np > 0 && $np2 > 0 } {
	set maxpass 2
    }

    set stem $thisparam($this,csi_outputstem)

    for { set pass 0 } { $pass < $maxpass } { incr pass } {

	set dt "importraw"
	set cont $raw_control
	set name "csi"
	
	if { $pass == 1 || ( $pass == 0 && $np == 0 ) } {
	    set dt "importpmap"
	    set cont $pmap_control
	    set name "pmap"
	}

	set stack [ vtkpxCSIStackImages [ pxvtable::vnewobj ]]
	for { set i 0 } { $i < $np } { incr i } {
	    set it($i) [  vtkpxEpilepsyCSIReader [ pxvtable::vnewobj ]]
	    set fname [ $thisparam($this,csi_$dt) get $i $i ]
	    puts stdout "Loading  $fname"
	    $it($i) Load $fname
	    $stack AddInput [ $it($i) GetOutput ]
	}
	
	$stack SetResolutionXY $thisparam($this,csi_importinplane)
	$stack SetResolutionZ  $thisparam($this,csi_importslice)
	if { $pass == 0 } {
	    $stack SetAnatomicalInput [ $ana GetOutput ]
	} else {
	    $stack SetAnatomicalInput [ [ $conv_control  GetObject ] GetImage ]
	}
	$stack Update
	set anaw [ vtkpxAnalyzeImageWriter [ pxvtable::vnewobj ]]
	$anaw SetInput [ $stack GetOutput ]
	$anaw Save "${stem}_${name}.hdr"
	
	[ $cont GetObject ] Load "${stem}_${name}.hdr"; $cont Update
	$anaw Delete
	
	if { $pass == 0 } {
	    set anaw [ vtkpxAnalyzeImageWriter [ pxvtable::vnewobj ]]
	    $anaw SetInput [ $stack GetAnatomicalOutput ]
	    $anaw Save "${stem}_convscout.hdr"
	    $anaw Delete
	    [ $conv_control GetObject ] Load "${stem}_convscout.hdr"; $conv_control Update
	}

	if { $pass == 0 && $np > 0 } {
	    set thisparam($this,csi_categories) [ string range [ $it(0) GetCategoryNames ] 1 end-1  ]
	    set sc [ $it(0) GetScaleFactors ]
	    set n  [ $sc GetNumberOfTuples ]
	    set thisparam($this,csi_scalefactors) ""
	    for { set i 0 } { $i <  $n } { incr i } {
		set a $thisparam($this,csi_scalefactors) 
		set thisparam($this,csi_scalefactors) "$a \t [$sc GetComponent $i 0 ]"
	    }
	}
	
	$stack Delete
	for { set i 0 } { $i < $np } { incr i } {
	    $it($i) Delete
	}
    }
    $ana Delete
    SaveSetup "${stem}.csi"
    WatchOff
}
# ---------------------------------------------------------------------------

itcl::body pxitclcsigadget::ResetImport  { } {
    
    $thisparam($this,csi_importraw)   delete 0 end
    $thisparam($this,csi_importpmap)  delete 0 end
    set thisparam($this,csi_importscout) ""
    set thisparam($this,csi_output) "patient"

}

# ---------------------------------------------------------------------------

itcl::body pxitclcsigadget::ReadExtraInfo { fname } { 

    if { [ string length $fname  ] == 0 } {
	set f1 $setupfilename
	set f1path [ file dirname $f1 ]
	set f1tail [ file tail    $f1 ]
	set fname  [tk_getOpenFile -title "Extra Info File" -filetypes { {"Csi-Extra Info File" {_extra.txt}}} -initialfile $f1tail -initialdir $f1path ]
    }

    if { [ string length $fname ] < 1 } {
	return
    }

    if { [ file readable $fname ] == 0 } {
	::pxtclutil::Warning "Cannot read extra info file from $fname\n Bad File"
	return 0
    }

    set fileid [open $fname r]
	
    gets $fileid line
    if { [ string compare $line "#CSI Extra Information" ] != 0 } {
	close fileid
	::pxtclutil::Warning "Cannot read extra info file from $fname\n Bad File Type"
	return 0
    }

    gets $fileid line ;     gets $fileid line  ;
    gets $fileid thisparam($this,csi_categories)
    gets $fileid line  ;
    gets $fileid thisparam($this,csi_scalefactors)
    close $fileid
}
# ---------------------------------------------------------------------------
itcl::body pxitclcsigadget::VerifyRegistrationCallback { mode args } { 

    if { $mode == 1 } {
	$this VerifyRegistration $anat_control $conv_control $refx_control
    } 
}

itcl::body pxitclcsigadget::UpdateFromRegistration { mmvutil } {

    if { [ $mmvutil GetProgress ] == 1.0 } {
	$this UpdateFromRegistrationDone $mmvutil
	return
    } 
    
    pxtkprint "[ $mmvutil GetOutputStatus ]"
    $this ProgressCallback  $mmvutil "Registering Images"
}

itcl::body pxitclcsigadget::UpdateFromRegistrationDone { mmvutil } {

    pxtkprint "[ $mmvutil GetOutputStatus ]"
    $this ProgressCallback  $mmvutil Done
}

# ---------------------------------------------------------------------------------------
itcl::body pxitclcsigadget::VerifyRegistration { imgcontrol1 imgcontrol2 xformcontrol } {

    global pxtcl_pref_array

    set image_ref [ $imgcontrol1 GetObject ]
    set image_trn [ $imgcontrol2 GetObject ]

    set orient_ref [ $image_ref GetOrientation ]
    set orient_trn [ $image_trn GetOrientation ]
    
    set dim1 [ [ $image_ref GetImage ] GetDimensions ]
    set dim2 [ [ $image_trn GetImage ] GetDimensions ]
    if { [lindex $dim2 0 ] <2 || [ lindex $dim1 0 ] < 2 } { 
	::pxtclutil::Warning "No Image in Memory\n Cannot Verify Transformation!"
	return  0
    }

    WatchOn

    set ut [ vtkpxUtil [ pxvtable::vnewobj ]]
    set tr [ $xformcontrol GetTransformation ]
    set tmp  [ [  pxitclimage \#auto ] GetThisPointer ]
    $ut ResliceImage [ $tmp GetImage ] [ $image_trn GetImage ] [ $image_ref GetImage ] [ $tr GetTransformation ] 1 0.0
    $ut Delete


    $tmp CopyImageHeader [ $image_ref GetImageHeader ]
    $tmp configure -filename "trn_[$image_trn cget -filename ]"

    $parent SetResultsFromObject $image_ref $this
    $second_viewer SetResultsFromObject $tmp $this
    catch { itcl::delete object $tmp }
    
    WatchOff
    return 1
}
#--------------------------------------------------------------------------------------------
#
#          GUI Stuff
#
#--------------------------------------------------------------------------------------------
itcl::body pxitclcsigadget::CreateImportControl { widget } {

    frame $widget.t1; frame $widget.t15 -height 3 -bg black
    frame $widget.t2; frame $widget.t25 -height 3 -bg black
    frame $widget.t3; frame $widget.t35 -height 3 -bg black
    frame $widget.t5; frame $widget.t55 -height 3 -bg black
    frame $widget.t4

    pack $widget.t1 $widget.t15 $widget.t2 $widget.t25 $widget.t3 $widget.t5 $widget.t55 $widget.t4 -side top -expand true -fill x
    
    set thisparam($this,csi_importraw) [ iwidgets::scrolledlistbox $widget.t1.a -labeltext "RAW CSI Text Files"  -vscrollmode static -hscrollmode dynamic -selectmode single -exportselection 0  ]

    eval "button $widget.t1.b -text \"Add\" -command { $this BrowseImport \"Raw CSI\"} "
    pack $widget.t1.b -side right -expand false
    pack $widget.t1.a -side left  -expand true -fill both    
    

    set thisparam($this,csi_importpmap) [ iwidgets::scrolledlistbox $widget.t2.a -labeltext "PMAP Text Files"  -vscrollmode static -hscrollmode dynamic -selectmode single -exportselection 0 ]
    eval "button $widget.t2.b -text \"Add\" -command { $this BrowseImport \"CSI PMAP\" } "
    pack $widget.t2.b -side right -expand false
    pack $widget.t2.a -side left -expand true -fill both    

    iwidgets::entryfield  $widget.t3.a -labeltext "Scout:" -textvariable [ itcl::scope  thisparam($this,csi_importscout) ] -width 50 
    eval "button $widget.t3.b -text \"Browse\" -command { $this BrowseImport scout} "
    pack $widget.t3.b -side right -expand false
    pack $widget.t3.a -side left -expand true -fill both    

    iwidgets::entryfield  $widget.t5.a -labeltext "Output:" -textvariable [ itcl::scope  thisparam($this,csi_outputstem) ] -width 50 
    eval "button $widget.t5.b -text \"Browse\" -command { $this BrowseImport output} "
    pack $widget.t5.b -side right -expand false
    pack $widget.t5.a -side left -expand true -fill both    


    label $widget.t4.lab2 -text "In-Plane (mm):"
    tk_optionMenu $widget.t4.res2 [ itcl::scope thisparam($this,csi_importinplane) ] "0.5" "0.75" "1.0" "2.0" "4.0" "8.0"
    pack $widget.t4.lab2 $widget.t4.res2 -side left -expand f -fill x	

    label $widget.t4.lab3 -text "Slice (mm):"
    tk_optionMenu $widget.t4.res3 [ itcl::scope thisparam($this,csi_importslice) ] "0.25" "0.5" "0.625" "0.75" "1.0" "1.25" "1.5"  "2.0" "2.5" "5.0" "10.0"
    pack $widget.t4.lab3 $widget.t4.res3 -side left -expand f -fill x	

    eval "button $widget.t4.d -text \"Import\" -command { $this RunImport} "
    eval "button $widget.t4.d2 -text \"Reset\" -command { $this ResetImport} "
    pack $widget.t4.d2 $widget.t4.d -side right -expand f -fill x -padx 10 -pady 5




}
#--------------------------------------------------------------------------------------------
itcl::body pxitclcsigadget::CreateImageControl { widget } {

    set wr $widget

    $anat_control Initialize $wr.1 
    $conv_control Initialize $wr.2 
    $raw_control Initialize $wr.4
    $pmap_control Initialize $wr.5

    pack $wr.1 $wr.2 $wr.4 $wr.5 -side top -expand f -fill x -pady 2

    if { $parent != 0 } {

	if { $second_viewer == 0 } {
	    $anat_control AddFunction "$parent SetResultsFromObject" "Display" "$this"
	    $conv_control AddFunction "$parent SetResultsFromObject" "Display" "$this"
	    $raw_control AddFunction "$parent SetResultsFromObject" "Display" "$this"
	    $pmap_control AddFunction "$parent SetResultsFromObject" "Display" "$this"
	} else {
	    $raw_control AddFunction "$parent SetImageFromObject" "DRef" "$this"
	    $raw_control AddFunction "$second_viewer SetImageFromObject" "DTrn" "$this"

	    $conv_control AddFunction "$second_viewer SetImageFromObject" "DTrn" "$this"
	    $conv_control AddFunction "$parent SetImageFromObject" "DRef" "$this"

	    $anat_control AddFunction "$parent SetImageFromObject" "DRef" "$this"
	    $anat_control AddFunction "$second_viewer SetImageFromObject" "DTrn" "$this"

	    $pmap_control AddFunction "$parent SetImageFromObject" "DRef" "$this"
	    $pmap_control AddFunction "$second_viewer SetImageFromObject" "DTrn" "$this"
	}
    }

}
    
# ---------------------------------------------------------------------------
#
#           Task Specific
#
# ----------------------------------------------------------------------
itcl::body pxitclcsigadget::CreateTransformControl { widget } {


    set wr $widget

    $refx_control Initialize $wr.2 

    pack  $wr.2 -side top -expand f -fill x -pady 10

    if { $parent !=0 && $second_viewer !=0 } {
	$refx_control AddFunction "$this VerifyRegistrationCallback 1" "Check!" "$this"
    }

}
#---------------------------------------------------------------------------
#   
# Output Stuff
#
# ---------------------------------------------------------------------------
itcl::body pxitclcsigadget::SetOverlayToolInputs { } {

    $overlaytool SetImage [ $anat_control GetObject ]
    set fmode 0
    
    if { $thisparam($this,overlayfunction) == "Raw-CSI" } {
	$overlaytool SetFunctionalImage [ $raw_control GetObject ]
	set fmode 0
    } elseif { $thisparam($this,overlayfunction) == "PMAP-CSI" } {
	$overlaytool SetFunctionalImage [ $pmap_control GetObject ] 
	set fmode 0
    } elseif { $thisparam($this,overlayfunction) == "Smooth1" } {
	$overlaytool SetFunctionalImage [ $smooth_control1 GetObject ] 
	set thisparam($this,overlayanatomy) "3D Anatomical" 
	set fmode 1
    } elseif { $thisparam($this,overlayfunction) == "Smooth2" } {
	$overlaytool SetFunctionalImage [ $smooth_control2 GetObject ] 
	set thisparam($this,overlayanatomy) "3D Anatomical" 
	set fmode 1
    }

    set tmode 1

    if { $thisparam($this,overlayanatomy) == "3D Anatomical" } {
	$overlaytool SetImage [ $anat_control GetObject ]
	set tmode 1
    } else {
	puts stdout  "Conv=$thisparam($this,overlayanatomy)"
	$overlaytool SetImage [ $conv_control GetObject ] 
	set tmode 0
    }
    
    
    if { $tmode != $fmode } {
	$overlaytool SetTransformation [[ $refx_control GetObject ] GetTransformation ]
    } else {
	set tr [ vtkIdentityTransform [ pxvtable::vnewobj ]]
	$overlaytool SetTransformation $tr
	$tr Delete
    }

}
# ----------------------------------------------------------------------------------------------
#  Compute Smooth maps
# ----------------------------------------------------------------------------------------------

itcl::body pxitclcsigadget::SampleElectrodes { args }  {

    set control [ lindex $args 1 ]
    puts stdout "Use smoothing control = $control, electrodetool = $electrodetool"

    set electrodes [ $electrodetool GetMultiGrid ]
    puts stdout "Electrodes = [ $electrodes GetClassName ]"

    set ctrl $smooth_control1
    if { $control == 2 } {
	set ctrl $smooth_control2
    }

    set img [ [ $ctrl GetObject ] GetImage ]
    puts stdout "Image Components = [ $img GetNumberOfScalarComponents ] [ [ $ctrl GetObject ] cget -filename ]"
    $electrodes PickFunction $img 

    set typelist { 
	{"Text Files" {".txt" } }
	{"All Files" {*}}
    }
    
    set l_filename "[ file root $setupfilename ].txt"
    set f1 $l_filename
    set f1path [ file dirname $f1 ]
    set f1tail [ file tail    $f1 ]
    set fname [tk_getSaveFile -title "Export picked function in text file" -filetypes $typelist -initialfile $f1tail -initialdir $f1path ]
    if { [ string length $fname ] > 0 } {
	$electrodes ExportValues $fname "\#Scale Factors: \t \t \t \t $thisparam($this,csi_scalefactors)" $thisparam($this,smooth_sigma${control}) $thisparam($this,csi_categories)
    }
}
# ----------------------------------------------------------------------------------------------
itcl::body pxitclcsigadget::ComputeSmooth { } {

    set ok 0
    set err "No anatomical and no raw-csi image in memory"    

    catch { if { [ [ $anat_control GetImage ] GetImageSize ] > 1 } {  incr ok }  }
    catch { if { [ [ $raw_control GetImage ] GetImageSize ] > 1 } {  incr ok; incr ok }  }

    if { $ok != 3 } {
	if { $ok == 1 } { set err "No raw-csi image in memory" }
	if { $ok == 2 } { set err "No 3D anatomical image in memory" }

	::pxtclutil::Warning "$err\n Cannot smooth raw csi !"
	return 0
    }


    WatchOn

    set sigma $thisparam($this,smooth_sigma)

    set resl [ vtkbisImageReslice [ pxvtable::vnewobj ]]
    $resl SetInput [ [ $raw_control GetObject ] GetImage ]
    $resl SetInformationInput [ [ $anat_control GetObject ] GetImage ]
    $resl SetResliceTransform [ [ $refx_control GetObject ] GetTransformation ]
    $resl SetInterpolationMode 0
    SetFilterCallbacks $resl "Reslicing Image"
    $resl Update
    


    set smooth  [ vtkpxNormalizedSmoothImage [ pxvtable::vnewobj ]  ]
    $smooth SetSigma $sigma
    $smooth SetInput [ $resl GetOutput ]
    $smooth SetNormalizeScale 1000.0
    SetFilterCallbacks $smooth "Smoothing Image"
    $smooth Update
    
    set vl $thisparam($this,smooth_control)
    set control $smooth_control1
    if { $thisparam($this,smooth_control) == 2 } {
	set control $smooth_control2
    }

    set n1 [ file rootname [ [ $raw_control GetObject ] cget -filename ] ]
    set name "${n1}_sm_${sigma}.hdr"

    set thisparam($this,smooth_sigma$vl) $thisparam($this,smooth_sigma)

    set out [ $control GetImage ]
    $out ShallowCopyImage [ $smooth GetOutput ]
    $out CopyImageHeader [ [ $raw_control GetObject ] GetImageHeader ]
    $out configure -filename $name
    $control Update

    catch { $smooth Delete }
    catch { $resl Delete }
    
    WatchOff
    return 1
}

# ----------------------------------------------------------------------------------------------
#   Create GUI 
# ----------------------------------------------------------------------------------------------

itcl::body pxitclcsigadget::CreateSamplingControl { widget } {

    $smooth_control1 Initialize $widget.1 
    $smooth_control2 Initialize $widget.2 

    frame $widget.3 -bg black -height 5
    frame $widget.4


    pack $widget.1 $widget.2 $widget.3  -side top -expand f -fill x -pady 2
    pack $widget.4 -side bottom -expand f -fill x -padx 2 -pady 2

    if { $parent != 0 } {

	if { $second_viewer == 0 } {
	    $smooth_control1 AddFunction "$parent SetResultsFromObject" "Display!" "$this"
	    $smooth_control2 AddFunction "$parent SetResultsFromObject" "Display!" "$this"
	} else {
	    $smooth_control2 AddFunction "$second_viewer SetImageFromObject" "DTrn" "$this"
	    $smooth_control2 AddFunction "$parent SetImageFromObject" "DRef" "$this"

	    $smooth_control1 AddFunction "$parent SetImageFromObject" "DRef" "$this"
	    $smooth_control1 AddFunction "$second_viewer SetImageFromObject" "DTrn" "$this"

	}
    }


    $smooth_control1 AddFunction "$this SampleElectrodes" "Electrode" "1"
    $smooth_control2 AddFunction "$this SampleElectrodes" "Electrode" "2"
	

    label $widget.3.lab2 -text "Smoothing Kernel (mm):"
    tk_optionMenu $widget.3.res2 [ itcl::scope thisparam($this,smooth_sigma) ] "1.0" "2.0" "4.0" "8.0" "12.0" "16.0" "20.0" "24.0" "32.0"
    pack $widget.3.lab2 $widget.3.res2 -side left -expand f -fill x	

    label $widget.3.lab3 -text "Output:"
    tk_optionMenu $widget.3.res3 [ itcl::scope thisparam($this,smooth_control) ] "1" "2"
    pack $widget.3.lab3 $widget.3.res3 -side left -expand f -fill x	

    eval "button $widget.t4 -text \"Compute Smooth Raw CSI!\" -command { $this ComputeSmooth} "
    pack $widget.t4 -side top -expand f -fill x -padx 10 -pady 5

    
    iwidgets::entryfield  $widget.e1 -labeltext "Categories:" -textvariable [ itcl::scope  thisparam($this,csi_categories) ] -width 80 
    iwidgets::entryfield  $widget.e2 -labeltext "Scale Factors:" -textvariable [ itcl::scope  thisparam($this,csi_scalefactors) ] -width 80 
    pack $widget.e2 $widget.e1 -side bottom -fill x -expand true

}
# ----------------------------------------------------------------------------------------------
itcl::body pxitclcsigadget::CreateOverlayControl { widget } {

   frame $widget.t3
   pack $widget.t3  -side top -expand f -fill x -pady 4
    # --------------------------
    # Visualization Output Stuff
    # --------------------------
    iwidgets::Labeledframe $widget.t3.a -labelpos n -labeltext "Reslice Output"
    iwidgets::Labeledframe $widget.t3.b -labelpos n -labeltext "Output Overlay"
    set c2 [$widget.t3.a childsite]
    set c3 [$widget.t3.b childsite]
    pack $widget.t3.a $widget.t3.b -side top -fill both -padx 5 -pady 2

   set w2 [ frame $c3.ttop ]
   set w  [ frame $c3.top]; 
   pack $w2 $w -side top -expand f -fill x
    
   if { $second_viewer != 0 } {
       label $w2.ml -text "Output Viewer:"
       tk_optionMenu $w2.mopt [ itcl::scope thisparam($this,overlayviewer)] "Reference" "Transform"
       pack $w2.ml $w2.mopt -side left -expand f -fill x
   }

    label $w.l -text "Anatomy:"
    tk_optionMenu $w.opt [ itcl::scope thisparam($this,overlayanatomy) ] "3D Anatomical" "Conventional"
    pack $w.l $w.opt -side left -expand f -fill x

    label $w.l2 -text "Function:"
    tk_optionMenu $w.opt2 [ itcl::scope thisparam($this,overlayfunction) ] "Raw-CSI" "PMAP-CSI" "Smooth1" "Smooth2"
    pack $w.l2 $w.opt2 -side left -expand f -fill x


    for { set k 0 } { $k <=1 } { incr k } {
	eval "$w.opt.menu entryconfigure $k -command { $this SetOverlayToolInputs }"
    }
    for { set k 0 } { $k <=3 } { incr k } {
	eval "$w.opt2.menu entryconfigure $k -command { $this SetOverlayToolInputs }"
    }

    $overlaytool CreateResliceControl $c2
    $overlaytool CreateFunctionalOverlayControl $c3


}


# ---------------------------------------------------------------------------
# Main GUI Body 
# ---------------------------------------------------------------------------

itcl::body pxitclcsigadget::Initialize { widget } { 
	
    set basewidget [ toplevel $widget ]
    wm geometry $basewidget 575x600
    wm withdraw $basewidget

    SetTitle "CSI Tool"

    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget
}

itcl::body pxitclcsigadget::DelayedInitialize { } { 

    set menubase     [ menu $basewidget.menu ]; $basewidget configure -menu $menubase

    set pbar         [ frame $basewidget.pbar    -width 400 -height 20 ]
    set notebook $basewidget.notebook

    iwidgets::tabnotebook $notebook  -tabpos w
    #    pack $menubase -side top -expand f -fill x -pady 1
    pack $pbar     -side bottom -expand f -fill x -pady 1
    pack $notebook -side top -expand t -fill both -pady 1
   
    CreateImageControl      [ $notebook add -label "Images"  ] 
    CreateTransformControl  [ $notebook add -label "Registrations" ]
    CreateSamplingControl   [ $notebook add -label "Sampling"   ]
    CreateOverlayControl    [ $notebook add -label "Overlays"   ]
    CreateImportControl     [ $notebook add -label "Import"   ]

    CreateProgressBar  $pbar


    #    menubutton $menubase.file          -text Setup      -menu $menubase.file.m -underline 0
    #   pack $menubase.file   -side left
    menu $menubase.filem -tearoff 0 
    $menubase add cascade -label Setup -menu $menubase.filem

    eval "$menubase.filem add command -label \"Load Setup\" -command { $this LoadSetup \"\" } -underline 0 "
    eval "$menubase.filem add command -label \"Save Setup\" -command { $this SaveSetup \"\" } -underline 0"
    $menubase.filem add separator
    eval "$menubase.filem add check -variable [ itcl::scope thisparam($this,autosavesetup) ] -label \"Auto Save Setup File\" "
    $menubase.filem add separator
    if { $parent == 0 } {
	eval "$menubase.filem add command -label Exit -command {  pxtkexit } -underline 1"
    } else {
	eval "$menubase.filem add command -label Close -command {  $this HideWindow } -underline 1"
    }
    
    $notebook view "Images"

    set fullyinitialized 1
}

::itcl::body pxitclcsigadget::AddToMenuButton { mb args} {

    eval "$mb add command -label \"Images\" -command {$this ShowWindow \"Images\"}"
    eval "$mb add command -label \"Registrations\" -command {$this ShowWindow \"Registrations\"}"
    eval "$mb add command -label \"Sampling\" -command { $this ShowWindow Sampling}"
    eval "$mb add command -label \"Overlays\" -command { $this ShowWindow Overlays}"
    eval "$mb add command -label \"Import Raw Data\" -command { $this ShowWindow Import}"
}

::itcl::body pxitclcsigadget::AddToMenuButtonLite { mb args} {

    eval "$mb add command -label \"CSI Tool\" -command {$this ShowWindow \"Images\"}"
}


# ---------------------------------------------------------------------------

