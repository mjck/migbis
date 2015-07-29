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







package provide pxitclmultisubjectaverage 1.0

# 	$Id: pxitclmultisubjectaverage.tcl,v 1.1 2003/07/29 14:27:36 xenios Exp xenios $	

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] bis_algorithm ]


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
package require  bis_createoverlayimage 1.0
package require  bis_roimean            1.0
package require  pxtcluserprefs         1.0
package require  pxitclbaseimageviewer  1.0
package require  pxitcllistselector  1.0
# ----------------------------------------------------------------------------------------------

itcl::class pxitclmultisubjectaverage {

    inherit pxitclbaseimagecontrol

    # Generic Stuff 
    private   common   thisparam        
    protected variable numberofstudies 0
    protected variable numberoftasks   0
    protected variable setupfilename   "multisubject.msb"
    protected variable second_viewer   0
    protected variable external_overlayutility 0

    # Subject Specific
    protected variable reference_control    0
    protected variable voi_control    0
    protected variable anat_control    0
    protected variable conv_control    0
    protected variable func_control    0
    protected variable epi_control    0
    protected variable field_control    0
    protected variable task_control    0
    protected variable refx_control    0
    protected variable intx_control    0
    protected variable distx_control    0

    protected variable subject_tree    0
    protected variable current_subject ""
    protected variable subjects        0    
    protected variable last_subject    -1
    protected variable last_added_subject 0


    # Task Specific
    protected variable task_tree    0
    protected variable current_task ""
    protected variable tasks        0    
    protected variable last_task    -1
    protected variable last_added_task 0
    protected variable task_listbox 0


    # Output Stuff
    protected variable output_control  0 
    protected variable output_listbox     0
    protected variable output_listbox_lastselected  0
    protected variable output_list [ list "Average Anatomical" "Std Anatomical" "Average Functional" "Std Functional" "Tscore Functional" ]

    protected variable overlaycreator     0
    protected variable outputimages 
    
    # Batch Mode Stuff

    public variable batch_selector 0




    constructor { par args } {
	pxitclbaseimagecontrol::constructor $par 
    } {
	if { [ llength $args] > 0  } {
	    set second_viewer [ lindex $args 0 ]
	}
	if { [ llength $args] > 1 } {
	    set external_overlayutility [ lindex $args 1 ]
	}
	InitializeMultiSubject
	set overlaycreator [ [ bis_createoverlayimage \#auto ] GetThisPointer ]
	$overlaycreator InitializeFromContainer $this
	$overlaycreator SetGUIMode managed

	if { $second_viewer !=0 } {
	    set thisparam($this,overlayviewer) "Transform"
	}
	
    }

    private method InitializeMultiSubject { } 
    public method Initialize { widget }
    public method DelayedInitialize { }

    public method ReorganizeData {  }
    public method SaveSetup { args }
    public method LoadSetup { args }
    public method GetSetupName {  } { return $setupfilename }

    public method AddDummyFunctionalName { }
    public method NewSetup { }


    # --------------------------------------------
    # Generic Callbacks
    # --------------------------------------------
    public method FillHierarchy    { items node } 
    public method FillSubjects     { node }  { return [ FillHierarchy $subjects $node ] }
    public method FillTasks        { node }  { return [ FillHierarchy $tasks    $node ] }


    public method SelectItem   { widget callbck tag status } 
    public method SelectSubject { tag status } { return [ SelectItem $subject_tree "SetNewSubject" $tag $status ]} 
    public method SelectTask    { tag status } { return [ SelectItem $task_tree "SetNewTask" $tag $status ]} 


    public method ExpandAll { widget items last_item mode }
    public method ExpandAllSubjects { mode } { return [ ExpandAll $subject_tree $subjects $last_subject $mode ] }
    public method ExpandAllTasks { mode } { return [ ExpandAll $task_tree $tasks $last_task $mode ] }
    
    # ----------------------------------------------
    # Subject Specific Callbacks
    # ----------------------------------------------

    public method ResetSubjectTree { args }
    public method AddSubject      { } 
    public method RemoveCurrentSubject  { } 
    public method RemoveAllSubjects { } 
    public method SetNewSubject   { uid } 
    protected method CreateSubjectControl { parent }

    # ----------------------------------------------
    # Task Specific Callbacks
    # ----------------------------------------------

    public method ResetTaskTree { args }
    public method AddTask      { } 
    public method RemoveCurrentTask  { } 
    public method SetNewTask   { uid } 
    public method RemoveAllTasks { } 
    protected method CreateBaseControl  { parent }


    # -----------------------------------------------
    # Results Stuff
    # -----------------------------------------------
    public    method ChangeCurrentTask { tasklistbox args } 
    public    method LoadAll { functionalonly }

    # Stuff from outside access
    public    method SetInterpolationAndResolution { interp resol }
    public    method SetCurrentTask   { number } 

    public    method ComputeTscoreMap {  } 
    public    method ComputeAverages { mode } 
    public    method WarpTasksToCommonSpace { mode { dirname "" } { saveanatomical 1 } }
    public    method DoVOIAnalysis { filename appendmode } 


    public    method SetOverlayCreatorInputs { } 
    public    method OutputListCallback { listbox args }
    protected method CreateResultsControl { parent } 

    public method VerifyRegistrationCallback { mode args }
    #    public method ComputeRegistrationCallback { mode args }
    public method GrabRegistrationCallback { mode args }


    # Get Output Files
    # 
    public method GetMeanAnatomicalOutput {  } {  return [ [ lindex  $outputimages 0 ] GetThisPointer ]  }
    public method GetSigmaAnatomicalOutput {  } {  return [ [ lindex  $outputimages 1 ] GetThisPointer ]  }
    public method GetMeanFunctionalOutput {  } {  return [ [ lindex  $outputimages 2 ] GetThisPointer ]  }
    public method GetSigmaFunctionalOutput {  } {  return [ [ lindex  $outputimages 3 ] GetThisPointer ]  }
    public method GetTscoreFunctionalOutput { } {  return [ [ lindex  $outputimages 4 ] GetThisPointer ]  }

    # -----------------------------------
    # Batch Mode Stuff
    # ------------------------------------

    protected method SelectTasksToProcess { comment }
    protected method SelectSubjectsToProcess { comment }

    public method ComputeAllResults { mode } 
    public method WarpAllTasks { mode } 

    public method Create4DImageForSingleFrameAndCurrentTask { imglist fr  { roimeanalg 0 } } 
    public method Create4DImagesAllTasksOld { mode }
    public method Create4DImagesAllTasks { mode }


    public method DoVOIAnalysisAllTasks { mode } 
    #    public method ComputeAllRegistrations { mode secondmode } 

    # -----------------------------------
    # Compare Stuff
    # ------------------------------------
    public method SetInfoToCompareTool { mode2 }
    public method ShowCompareTool { }
    
    # -------------------
    # OOP Stuff
    # --------------------

    public method SetResultsFromObject { img obj }
    public method SetValueImage        { vtkimg  }
    public method AddToMenuButton { mb args}
    public method SetLookupTable       { colormap }
    public method SetLookupTableMode   { index }
    public method GetViewer { }

}
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::SetResultsFromObject { img obj  } {

    if { [ $img GetImageSize ] < 2 } {	
	return 0    
    }
    
    if { $obj == $overlaycreator && $parent !=0 } {
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

itcl::body pxitclmultisubjectaverage::SetValueImage { vtkimg  } {
    if {$thisparam($this,overlayviewer) == "Transform" && $second_viewer !=0 } {
	$second_viewer SetValueImage $vtkimg
    } else {
	$parent SetValueImage $vtkimg
    }
    return 1
}

# ---------------------------------------------------------------------------
#  Lookup Table Stuff 
# ---------------------------------------------------------------------------

itcl::body pxitclmultisubjectaverage::SetLookupTable { colormap } {
    
    set v $parent
    if {$thisparam($this,overlayviewer) == "Transform" && $second_viewer !=0 } {
	set v $second_viewer
    }
    
    if { $v != 0 } {
	$v SetLookupTable $colormap
    }
}

itcl::body pxitclmultisubjectaverage::GetViewer { } {

    set v [  $parent GetViewer ]
    if {$thisparam($this,overlayviewer) == "Transform" && $second_viewer !=0 } {
	set v [  $second_viewer GetViewer ]
    }
    return $v
}

itcl::body pxitclmultisubjectaverage::SetLookupTableMode { index } {

    set v $parent
    if {$thisparam($this,overlayviewer) == "Transform" && $second_viewer !=0 } {
	set v $second_viewer
    }
    
    if { $v != 0 } {
	$v SetLookupTableMode $index
    }
}


# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------

itcl::body pxitclmultisubjectaverage::InitializeMultiSubject { } {
    


    set refx_control [ [  pxitcltransformGUI \#auto ] GetThisPointer ]
    $refx_control configure -description "Transformation Anatomical->Reference"

    set intx_control [ [  pxitcltransformGUI \#auto ] GetThisPointer ]
    $intx_control configure -description "Transformation Conventional->Anatomical"

    set distx_control [ [  pxitcltransformGUI \#auto ] GetThisPointer ]
    $distx_control configure -description "Transformation Echoplanar->Conventional"

    set anat_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $anat_control configure -description "Anatomical Image"

    set reference_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $reference_control configure -description "Reference Image"

    set voi_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $voi_control configure -description "VOI Image"


    set conv_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $conv_control configure -description "Conventional Image"

    set func_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $func_control configure -description "Functional Image"

    set epi_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $epi_control configure -description "Spin-echo Echoplanar Image"

    set field_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $field_control configure -description "Fieldmap Image"

    set task_control [ [ pxitcltaskGUI \#auto ] GetThisPointer ]

    set output_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $output_control configure -description "Results Image"

    lappend itclobjectlist $refx_control $intx_control $distx_control $reference_control $voi_control $anat_control $conv_control $func_control $task_control $output_control $epi_control $field_control
       
    set thisparam($this,current_subject) ""
    set thisparam($this,current_output)  ""
    set thisparam($this,current_task) "task1"
    set thisparam($this,current_taskname) "New_Task_1"
    set tt $thisparam($this,current_task)

    for { set i 0 } { $i < [ llength $output_list ] } { incr i } {
	set tmp [ [ pxitclimage \#auto ] GetThisPointer ]
	lappend outputimages $tmp
	lappend itclobjectlist $tmp

	switch -exact -- $i {
	    "0" { $tmp configure -filename "anat_average.hdr" }
	    "1" { $tmp configure -filename "anat_std.hdr" }
	    "2" { $tmp configure -filename "func_mean${tt}.hdr" }
	    "3" { $tmp configure -filename "func_std${tt}.hdr" }
	    "4" { $tmp configure -filename "func_tscore${tt}.hdr" }
	}
    }

    set thisparam($this,overlayviewer) "Reference"
    set thisparam($this,overlaybase) "Reference Image"
    set thisparam($this,overlayfunction) "T-Map"
    set thisparam($this,overlaysinglesubject) 0

    set thisparam($this,interpolation) "Cubic"
    set thisparam($this,outputresolution) "3.0x3.0x3.0"
    set thisparam($this,compositethreshold) "2000"
    set thisparam($this,compositemode) "Positive"
    set thisparam($this,compositesigma) 0.0

    set thisparam($this,autosave) 1

    set debugprogress 1

}
#--------------------------------------------------------------------------------------------
#
#           Setup File Stuff
#
#--------------------------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::SaveSetup { args } { 

    set tmptask [ lindex $tasks 0 ]
    if { $thisparam($this,current_task) != [ $tmptask cget -task_suffix ] } {
	set ok [ ::pxtclutil::Question "The first task ([ $tmptask cget -task_name ],[$tmptask cget -task_suffix]) is not the current task. This could cause problems. It is best to cancel and switch to this task (using the \"Change Current Task\" button in the Results pane and try again! Click YES to IGNORE this warning?" ]
	if { $ok ==0 } { 
	    return
	}
    }

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
	set fname  [tk_getSaveFile -title "Saving Setup File" -filetypes { {"Multi-Setup File" {.msb}}} -initialfile $f1tail  -initialdir $f1path ]
    }

    if { [ string length $fname ] > 0 } {

	set fileid [open $fname w]
	
	puts $fileid "#Multisubject Description File v3"
	if { $tasks !=0 } {
	    puts $fileid "Tasks\n[llength $tasks]"
	    puts $fileid "- - - - - - - - - - - - - - - - - - - -"
	    for { set i 0 } { $i < [ llength $tasks ] } { incr i } {
		set tmp [ lindex $tasks $i ]
		$tmp SaveToSetupFile $fileid
	    }
	}
	if { $subjects != 0 } {
	    puts $fileid "Subjects\n[llength $subjects]"
	    puts $fileid "- - - - - - - - - - - - - - - - - - - -"
	    for { set i 0 } { $i < [ llength $subjects ] } { incr i } {
		set tmp [ lindex $subjects $i ]
		$tmp SaveToSetupFile $fileid
	    }
	}

	puts $fileid "Reference/Output Images"
	puts $fileid "--------------------------------------"

	puts $fileid "Reference Image : [ [ $reference_control  GetObject ] cget -filename] "
	puts $fileid "VOI Image : [ [ $voi_control  GetObject ] cget -filename] "
	for { set i 0 } { $i < [ llength $output_list ] } { incr i } {
	    puts $fileid "[ lindex $output_list $i ] : [ [ lindex $outputimages $i ] cget -filename ]"
	}
	puts $fileid "Legacy : "
	puts $fileid "Legacy : "
	puts $fileid "Legacy : "
	
	
	close $fileid
	set setupfilename $fname
	SetTitle "MultiSubjectAverage $setupfilename"
    }
}
# ------------------------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::AddDummyFunctionalName {  } { 

    if { $subjects != 0 } {
	for { set i 0 } { $i < [ llength $subjects ] } { incr i } {
	    set tmp [ lindex $subjects $i ]
	    set fname1 [  [ $tmp cget -anatomical_image ] cget -filename ]
	    #	    puts stdout "Fname1=$fname1"
	    set fname2 [ file join [ file dirname $fname1 ] dummy.hdr ]
	    [ $tmp cget -functional_image  ] configure -filename $fname2
	}
    }
}
# ------------------------------------------------------------------------------------------

itcl::body pxitclmultisubjectaverage::NewSetup {  } { 
 
    set ok [ ::pxtclutil::Question "This will delete your current setup information. Are you sure?" ]
    if { $ok ==0 } { 
	return
    }


    for { set i 0 } { $i < [ llength $outputimages ] } { incr i } {
	[ lindex $outputimages $i ] UnLoad
    }

    [ $reference_control  GetObject ] UnLoad
    [ $voi_control  GetObject ] UnLoad
    $reference_control Update
    $voi_control Update
    OutputListCallback $output_listbox 0

    RemoveAllTasks
    RemoveAllSubjects
    
    AddSubject;    AddSubject;    AddSubject;    SetNewSubject 0
    AddTask;    AddTask;    SetNewTask 0
}

itcl::body pxitclmultisubjectaverage::LoadSetup { args } { 

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
	set fname  [tk_getOpenFile -title "Setup File" -filetypes { {"Multi-Setup File" {.msb}}} -initialfile $f1tail -initialdir $f1path ]
    }

    if { [ string length $fname ] < 1 } {
	return 0
    }

    if { [ file readable $fname ] == 0 } {
	::pxtclutil::Warning "Cannot read setupfile file from $fname\n Bad File"
	return 0
    }

    set fileid [open $fname r]
	
    gets $fileid line

    set setupmode 0

    if { [ string compare $line "#Multisubject Description File"  ] != 0 } {
	if { [ string compare $line "#Multisubject Description File v2"  ] != 0 } {
	    if { [ string compare $line "#Multisubject Description File v3"  ] != 0 } {
		close $fileid
		::pxtclutil::Warning "Cannot read setupfile file from $fname\n Bad File Type"
		return 0
	    } else {
		set setupmode 2
	    }
	} else {
	    set setupmode 1
	}
    }

    gets $fileid line ;     gets $fileid line  ;     set numtasks $line ;    gets $fileid line 
    RemoveAllTasks

    for { set i 0 } { $i < $numtasks } { incr i } {
	set tmp [ AddTask ]
	$tmp LoadFromSetupFile $fileid
    }


    gets $fileid line ;     gets $fileid line  ;     set numsub $line ;    gets $fileid line 
    RemoveAllSubjects

    for { set i 0 } { $i < $numsub } { incr i } {
	set tmp [ AddSubject ]
	$tmp LoadFromSetupFile $fileid $setupmode
    }

    gets $fileid line ; gets $fileid line 

    # Do the Reference Separately
    set name [ pxitclsubject::GetFileElement $fileid ]
    set img  [ $reference_control GetObject ];     $img UnLoad  ;  $img configure -filename $name ;    $reference_control Update

    # Next do the VOI Control
    if { $setupmode == 2 } {
	set name [ pxitclsubject::GetFileElement $fileid ]
    } else {
	set name ""
    }
    set img  [ $voi_control GetObject ];     $img UnLoad  ;  $img configure -filename $name ;    $voi_control Update
    

    for { set i 0 } { $i < [ llength $output_list ] } { incr i } {
	set name [ pxitclsubject::GetFileElement $fileid ]
	set img  [ lindex $outputimages $i ]
	$img UnLoad
	$img configure -filename $name
    }
	
    close $fileid
    set setupfilename $fname

    ResetSubjectTree -1
    ResetTaskTree -1

    SetNewSubject 0
    SetNewTask 0
    OutputListCallback $output_listbox 0

    set tmptask [ lindex $tasks 0 ]
    set thisparam($this,current_task) [ $tmptask cget -task_suffix ]
    set thisparam($this,current_taskname) [ $tmptask cget -task_name  ]

    SetTitle "MultiSubjectAverage $setupfilename"
    return 1

}
# ------------------------------------------------------------------------------------------
#
#  Can we make these generic ?
#
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::FillHierarchy { items node } {

    if { $node == "" } {
	set slist ""
	if { $items != 0 } {
	    for { set i 0 } { $i < [ llength $items ] } { incr i } {
		set tmp [ lindex $items $i ]
		lappend slist [ list $i [ $tmp GetBaseName ]]
	    }
	}
	return $slist
    }
    
    if { $node >=0 && $node < [ llength $items ] && $items!=0 } {
	return [ [  lindex $items $node ] GetNameList ]
    }

    return ""
}

# ----------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::SelectItem { widget clback uid status} {

    if { [ string match "::*" $uid ] == 1 } {
	return 0
    }

    $widget selection clear
    $widget selection add  $uid
    eval "$clback  $uid"
    $this SetOverlayCreatorInputs 

}


# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::ExpandAll { widget items last_item mode } {

    if { $items ==0  || [ llength $items ] == 0  } {
	return
    }

    if { $mode == 2 } {
	ExpandAll $widget $items $last_item 0
	catch { $widget expand $last_item }
	return 
    }

    set len [ llength $items ]
    if { $mode == 1 } {
	for { set i 0 } { $i < $len } { incr i } {
	    $widget expand $i
	}
    } else {
	for { set i 0 } { $i < $len } { incr i } {
	    $widget collapse $i
	}
    }
}


# ----------------------------------------------------------------------
#
#           Subject Specific
#
# ----------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::SetNewSubject { uid} {

    if { $uid >=0 && $uid < [ llength $subjects ] && $subjects!=0 } {
	
	set last_subject $uid	
	set tmp [ lindex $subjects $uid ]
		
	$anat_control SetImage [ $tmp cget -anatomical_image ] 
	$conv_control SetImage [ $tmp cget -conventional_image ]
	$func_control SetImage [ $tmp cget -functional_image ]
	$epi_control SetImage [ $tmp cget -echoplanar_image ]
	$field_control SetImage [ $tmp cget -fieldmap_image ]

	$refx_control SetTransformation [ $tmp cget -reference_transform ]
	$intx_control SetTransformation [ $tmp cget -internal_transform ]
	$distx_control SetTransformation [ $tmp cget -distortion_transform ]

	set thisparam($this,current_subject) "[expr $uid+1 ]:[ $tmp GetBaseName ]"
    }
    $this SetOverlayCreatorInputs 
}
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::RemoveCurrentSubject  { } {

    if { $last_subject == -1 || $subjects == 0 } { 
	return 0 
    }

    if { $last_subject >= [ llength $subjects ] } {
	return 0
    }

    set tmp [ lindex $subjects $last_subject ]
    set name $thisparam($this,current_subject) 

    if { [ pxtclutil::Question "Remove Subject $name" ] == 0 } {
	return
    }
    

    set subjects [ lreplace $subjects  $last_subject $last_subject ]
    if { [ llength $subjects ] == 0  } {
	set subjects 0
    }
    catch { itcl::delete object $tmp }
    
    ResetSubjectTree
    SetNewSubject 0
}
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::RemoveAllSubjects  { } {

    if { $subjects == 0 } { 
	return 
    }

    
    for { set i 0 } { $i < [ llength $subjects ] } { incr i } {
	set tmp [ lindex $subjects $i ]
	catch { itcl::delete object $tmp }
    }
    
    set subjects 0
    ResetSubjectTree
}
# ---------------------------------------------------------------------------

itcl::body pxitclmultisubjectaverage::AddSubject { } {

    set tmp [ [ pxitclsubject \#auto ]  GetThisPointer ]
    lappend itclobjectlist $tmp
    if { $subjects !=0 } {
	lappend subjects $tmp
    } else {
	set subjects [ list $tmp ]
    }

    incr last_added_subject
    [ $tmp cget -anatomical_image ] configure -filename "New_Subject_${last_added_subject}"

    ResetSubjectTree 

    return $tmp
}
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::ResetSubjectTree { args } {

    set mode 0
    if { [ llength $args ] > 0 } {
	set mode [ lindex $args 0 ]
    }

    #eval "$subject_tree configure -querycommand \"$this FillSubjects %n\" "
    $subject_tree draw -now

    catch {
	set tmp [ lindex $subjects $last_subject ]
	set thisparam($this,current_subject) "[expr $last_subject+1]:[ $tmp GetBaseName ]"
    }

}
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::CreateSubjectControl { widget } {

    frame $widget.left
    frame $widget.mid -bg black -width 5
    set wr [ frame $widget.right ]

    pack $widget.right $widget.mid -side right -expand f -fill y 
    pack $widget.left  -side left -expand f -fill y -padx 10

    iwidgets::entryfield $wr.entry -labeltext "Current Subject:"  -width 25  -textvariable [ itcl::scope thisparam($this,current_subject) ] -relief sunken  


    iwidgets::tabnotebook $wr.base  -tabpos n
    pack $wr.entry -side top -fill x -expand f
    pack $wr.base -side top -fill both -expand true
    $wr.base configure -width 500

    set b1 [ $wr.base add -label "Main" ]
    set b2 [ $wr.base add -label "Distortion Correction" ]
    $wr.base view "Main"

    $anat_control InitializeLite $b1.1 3 
    $func_control InitializeLite $b1.2 3; $b1.2 configure -background darkgray
    $refx_control Initialize $b1.3;  $b1.3 configure -background darkgray
    $conv_control InitializeLite $b1.4 3; 
    $intx_control Initialize $b1.5
    $epi_control   InitializeLite $b2.6 3  
    $distx_control Initialize $b2.7
    $field_control   InitializeLite $b2.8 3   ; $b2.8 configure -background darkgray
    pack $b1.1 $b1.2 $b1.3 $b1.4 $b1.5  -side top -expand f -fill x -pady 0
    pack $b2.6 $b2.7 $b2.8 -side top -expand false -fill x -pady 0

    if { $parent != 0 } {
	if { $second_viewer == 0 } {
	    $anat_control AddFunction "$parent SetResultsFromObject" "Display" "$this"
	    $conv_control AddFunction "$parent SetResultsFromObject" "Display" "$this"
	    $func_control AddFunction "$parent SetResultsFromObject" "Display" "$this"
	    $epi_control AddFunction "$parent SetResultsFromObject" "Display" "$this"
	    $field_control AddFunction "$parent SetResultsFromObject" "Display" "$this"
	} else {
	    $anat_control AddFunction "$parent SetImageFromObject" "Display Ref" "$this"
	    $conv_control AddFunction "$parent SetImageFromObject" "Display Ref" "$this"
	    $func_control AddFunction "$parent SetImageFromObject" "Display Ref" "$this"
	    $epi_control AddFunction "$parent SetImageFromObject" "Display Ref" "$this"
	    $field_control AddFunction "$parent SetImageFromObject" "Display Ref" "$this"


	    $anat_control AddFunction "$second_viewer SetImageFromObject" "Display Trn" "$this"
	    $conv_control AddFunction "$second_viewer SetImageFromObject" "Display Trn" "$this"
	    $func_control AddFunction "$second_viewer SetImageFromObject" "Display Trn" "$this"
	    $epi_control AddFunction "$second_viewer SetImageFromObject" "Display Trn" "$this"
	    $field_control AddFunction "$second_viewer SetImageFromObject" "Display Trn" "$this"

	    $refx_control AddFunction "$this VerifyRegistrationCallback 1" "Check" "$this"
	    $intx_control AddFunction "$this VerifyRegistrationCallback 2" "Check" "$this"
	    $distx_control AddFunction "$this VerifyRegistrationCallback 3" "Check" "$this"
	}
	
	if { $external_overlayutility != 0 } {
#	    $refx_control AddFunction "$this ComputeRegistrationCallback 1" "Go to Compute" "showcontrols"
#	    $intx_control AddFunction "$this ComputeRegistrationCallback 2" "Go to Compute" "showcontrols"
#	    $distx_control AddFunction "$this ComputeRegistrationCallback 3" "Go to Compute" "showcontrols"
	    
	    $refx_control AddFunction "$this GrabRegistrationCallback 1 \"\"" "Grab" "$this"
	    $intx_control AddFunction "$this GrabRegistrationCallback 2 \"\"" "Grab" "$this"
	    $distx_control AddFunction "$this GrabRegistrationCallback 3 \"\"" "Grab" "$this"
	}
    }


    
    set subject_tree $widget.left.h
    eval "iwidgets::hierarchy $subject_tree -querycommand \"$this FillSubjects %n\" -visibleitems 60x15 -labeltext \"Subject List\" -selectcommand \"$this SelectSubject %n %s\" "
    $subject_tree configure -alwaysquery yes
    pack $subject_tree -side top -expand yes -fill both

    eval "$subject_tree  component itemMenu add command -label \"Reset\"           -command { $this ResetSubjectTree }"
    eval "$subject_tree  component itemMenu add command -label \"Expand Current Only\"     -command { $this ExpandAllSubjects 2 }"
    eval "$subject_tree  component itemMenu add command -label \"Expand All\"      -command { $this ExpandAllSubjects 1 }"
    eval "$subject_tree  component itemMenu add command -label \"Collapse All\"    -command { $this ExpandAllSubjects 0 }"



    set bot1 [ frame $widget.left.bot1 ]; 
    pack   $bot1 -side bottom -expand f -fill x



    eval "button $bot1.b1 -text \"Add Subject\"     -command { $this AddSubject}"
    eval "button $bot1.b2 -text \"Remove Subject\"  -command { $this RemoveCurrentSubject }"
    pack $bot1.b1 $bot1.b2  -side left



}
# ---------------------------------------------------------------------------
#
#           Task Specific
#
# ----------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::SetNewTask { uid} {

    if { $uid >=0 && $uid < [ llength $tasks ] && $tasks!=0 } {

	set last_task $uid	
	set tmp [ lindex $tasks $uid ]
	$task_control SetTask $tmp
    }
}
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::RemoveCurrentTask  { } {

    if { $last_task == -1 || $tasks == 0 } { 
	return 0 
    }

    if { $last_task >= [ llength $tasks ] } {
	return 0
    }

    set tmp [ lindex $tasks $last_task ]
    set name [ $tmp GetBaseName ]

    if { [ $tmp cget -task_suffix ] == $thisparam($this,current_task)  } {
	pxtclutil::Warning "Cannot Remove task $name, as it is the current task!"
	return 0
    }


    if { [ pxtclutil::Question "Remove Task $name" ] == 0 } {
	return 0
    }
    
    set tasks [ lreplace $tasks  $last_task $last_task ]
    if { [ llength $tasks ] == 0  } {
	set tasks 0
    }

    catch { itcl::delete object $tmp }
    
    ResetTaskTree
    SetNewTask 0
}
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::RemoveAllTasks  { } {

    if { $tasks == 0 } { 
	return 
    }

    
    for { set i 0 } { $i < [ llength $tasks ] } { incr i } {
	set tmp [ lindex $tasks $i ]
	catch { itcl::delete object $tmp }
    }
    
    set tasks 0
    set last_added_task 0
    ResetTaskTree

}
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::AddTask { } {

    set tmp [ [ pxitcltask \#auto ]  GetThisPointer ]
    lappend itclobjectlist $tmp
    if { $tasks !=0 } {
	lappend tasks $tmp
    } else {
	set tasks [ list $tmp ]
    }

    incr last_added_task
    $tmp configure -task_name "New_Task_${last_added_task}"
    $tmp configure -task_suffix "task${last_added_task}"
    ResetTaskTree 
    return $tmp
}
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::ResetTaskTree { args } {

    set mode 0
    if { [ llength $args ] > 0 } {
	set mode [ lindex $args 0 ]
    }

    #eval "$task_tree configure -querycommand \"$this FillTasks %n\" "
    $task_tree draw -now

    if { $task_listbox != 0 && $tasks !=0 } {
	$task_listbox clear
	for { set i 0 } { $i < [ llength $tasks ] } { incr i } {
	    $task_listbox insert end [ [ lindex $tasks $i ] cget -task_name ]
	}
    }

}
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::CreateBaseControl { widget } {
    # -----------
    # Task Editor
    # -----------

    $reference_control Initialize $widget.1 
    $voi_control Initialize $widget.11

    if { $parent !=0 } {
	if { $second_viewer == 0 } {
	    $reference_control AddFunction "$parent SetResultsFromObject" "Display" "$this"
	    $voi_control AddFunction "$parent SetResultsFromObject" "Display" "$this"
	} else {
	    $reference_control AddFunction "$parent SetImageFromObject" "Display Ref" "$this"
	    $reference_control AddFunction "$second_viewer SetImageFromObject" "Display Trn" "$this"
	    $voi_control AddFunction "$parent SetImageFromObject" "Display Ref" "$this"
	    $voi_control AddFunction "$second_viewer SetImageFromObject" "Display Trn" "$this"
	}
    }
    [ $reference_control GetObject ] configure -filename "reference.hdr"
    [ $voi_control GetObject ] configure -filename "voi.hdr"

    frame $widget.15 -bg black -height 15
    pack $widget.1 $widget.11 $widget.15 -side top -expand f -fill x -padx 10 -pady 10
    frame $widget.2 ;   pack $widget.2 -side top -expand f -fill x

    set base $widget.2


    frame $base.left
    frame $base.mid -bg black -width 2
    set wr [ frame $base.right ]

    pack $base.left -side left -expand f -pady 20 -padx 5
    pack $base.mid  -side left -expand f -pady 20 -fill y -padx 2
    pack $base.right -side left -expand f -pady 20 -padx 5 

    $task_control Initialize $wr.1
    pack $wr.1 -side left -expand f -fill x
    
    set task_tree $base.left.h
    eval "iwidgets::hierarchy $task_tree -querycommand \"$this FillTasks %n\" -visibleitems 20x15 -labeltext \"Task List\" -selectcommand \"$this SelectTask %n %s\" "
    $task_tree configure -alwaysquery yes
    eval "$task_tree component itemMenu add command -label  \"Expand Curr\"   -command { $this ExpandAllTasks 2 }"
    eval "$task_tree component itemMenu add command -label  \"Reset\"        -command { $this ResetTaskTree }"
    eval "$task_tree component itemMenu add command -label  \"Expand All\"   -command { $this ExpandAllTasks 1 }"
    eval "$task_tree component itemMenu add command -label  \"Collapse All\"   -command { $this ExpandAllTasks 0 }"
    pack $task_tree -side top -expand yes -fill both

    set bot1 [ frame $base.left.bot1 ]; 
    pack  $bot1 -side bottom -expand f -fill x

    eval "button $bot1.b1 -text \"Add Task\"  -command { $this AddTask}"
    eval "button $bot1.b2 -text \"Remove Task\"  -command { $this RemoveCurrentTask }"
    pack $bot1.b1 $bot1.b2  -side left


}
# ---------------------------------------------------------------------------
#   
# Output Stuff
#
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::SetOverlayCreatorInputs { } {

    set tmode 0
    set fmode 0
    
    switch -exact -- $thisparam($this,overlaybase) {
	"Reference Image" { $overlaycreator SetInput [ $reference_control GetObject  ] }
	"Average Anatomical" { $overlaycreator SetInput [ lindex $outputimages 0 ] }
	"Individual Anatomical" { $overlaycreator SetInput [ $anat_control GetObject  ]; set tmode 1 }
	"Individual Conventional" { $overlaycreator SetInput [ $conv_control GetObject  ]; set tmode 2 }
	"Individual Echoplanar" { $overlaycreator SetInput [ $epi_control GetObject  ]; set tmode 3 }
    }


    if { $tmode >0 }  {
	set thisparam($this,overlayfunction) "Single Map"
	set fmode 3
    } 
	

    switch -exact -- $thisparam($this,overlayfunction) {
	"Average Map"  { $overlaycreator SetSecondInput [ lindex $outputimages 2 ] }
	"T-Map" { $overlaycreator SetSecondInput [ lindex $outputimages 4 ] } 
	"Single Map" { $overlaycreator SetSecondInput [ $func_control GetObject  ]; set fmode 3 }
    }

    $overlaycreator UpdateInputRanges


    if { $fmode == 3 } {
	pack $thisparam($this,overlaysinglesubject) -side right -padx 2
    } else {
	pack forget $thisparam($this,overlaysinglesubject) 
    }



    # ----------------------------------------
    # Identical Space, i.e. echoplanar to indvidual (3,3) or 
    # Global to global (0,0)
    # -----------------------------------------

    
    #    puts stderr "Current Subjec sutff [ [ $anat_control GetImage ] cget -filename ]\n [ [ $conv_control GetImage ] cget -filename ]\n [[	$func_control GetImage ] cget -filename ]"
    #   puts stderr "Overlays $thisparam($this,overlayfunction)\n"
    #  puts stderr "Anatomical Image = [ [ $overlaycreator GetImage ] cget -filename ]"
    # puts stderr "Functional Image = [ [ $overlaycreator GetFunctionalImage ] cget -filename ]"
    #puts stderr "\t\t Range [ [ [ [ [ $overlaycreator GetFunctionalImage ] GetImage ] GetPointData ] GetScalars ] GetRange ]"

    if { ($tmode == 3 && $fmode == 3) || ( $tmode==0 && $fmode==0) } {

	#puts stderr "Simple mode tmode=$tmode fmode=$fmode identity transformation"
	set tr_id [ vtkTransform [ pxvtable::vnewobj ]]
	$tr_id Identity
	$overlaycreator SetVTKTransformation $tr_id
	return
    }


    # ----------------------------------------
    # Otherwise Concatenate Tranforms
    # -----------------------------------------

    set trans [ vtkGeneralTransform [ pxvtable::vnewobj ]]
    $trans Identity
    $trans PostMultiply

    set numt 1

    switch -exact -- $tmode  {
	0 { # Global Average or Composite Average -- we need everything
	    $trans Concatenate [ [ $refx_control GetObject ] GetTransformation ]
	    #puts stderr "Concatenating [ [ $refx_control GetObject ] cget -filename ]"
	    $trans Concatenate [ [ $intx_control GetObject ] GetTransformation ]
	    #puts stderr "Concatenating [ [ $intx_control GetObject ] cget -filename ]"
	    $trans Concatenate [ [ $distx_control GetObject ] GetTransformation ]
	    #puts stderr "Concatenating [ [ $distx_control GetObject ] cget -filename ]"
	    set numt 3
	}
	1 { # Invdividual 3d
	    $trans Concatenate [ [ $intx_control GetObject ] GetTransformation ]
	    #puts stderr "Concatenating [ [ $intx_control GetObject ] cget -filename ]"
	    $trans Concatenate [ [ $distx_control GetObject ] GetTransformation ]
	    #puts stderr "Concatenating [ [ $distx_control GetObject ] cget -filename ]"
	    
	    set numt 2
	}
	2 { # Nothing To Add
	}
	3 { # Nothing To Add
	}
    }
    #    puts stderr "More Complex tmode=$tmode fmode=$fmode number of concatenated transformations = $numt"


    $overlaycreator SetVTKTransformation $trans
    $trans Delete


}


itcl::body pxitclmultisubjectaverage::OutputListCallback { listbox args } {

    if { [ llength  $args ] > 0 } {
	set uid [ lindex $args 0 ]
	$listbox selection set $uid
    } 
	
    set uid [ lindex [ $listbox curselection ] 0 ]
    if { [ llength $uid ] == 0 } {
	set uid 0
	$listbox selection set 0
    }   

    $output_control configure -description [ lindex $output_list $uid ]
    $output_control SetImage [ lindex $outputimages $uid ]
    if { $listbox == $output_listbox } {
	set output_listbox_lastselected  $uid
    }
}



# ------------------------------------------------------------------
# Load All Images
# ------------------------------------------------------------------

itcl::body pxitclmultisubjectaverage::LoadAll { functionalonly } {
	
    set beg 3;
    set end 4;
    if { $functionalonly <1 } {
	set beg 0
	set end 4
    }

    set scalefactor [expr 1.0 / ( 1.0 + [ llength $subjects ] + [ llength $outputimages ] +1 ) ]
    set progress 0.0

    if { $functionalonly < 1 } {
	[ $reference_control GetObject ] Load 
	$reference_control Update
	[ $voi_control GetObject ] Load 
	$voi_control Update
    }

   
    for { set i $beg } { $i <= $end } { incr i } {
	[ lindex $outputimages $i ]  Load
	ShowProgressVal "Loading Output Image $i" $progress
	set progress [ expr $progress +$scalefactor ]
    }
    

    for { set sub 0 } { $sub < [ llength $subjects ] } { incr sub } {
	
	set current [ lindex $subjects $sub ] 
	
	if { $functionalonly <1 } {
	    
	    [ $current cget -anatomical_image ] Load
	    [ $current cget -conventional_image ] Load
	    [ $current cget -echoplanar_image ] Load
	    [ $current cget -reference_transform ] Load
	    [ $current cget -internal_transform ] Load
	    [ $current cget -distortion_transform ] Load
	}

	set ok [ [ $current cget -functional_image ] Load ]
	if { $ok == 0 } {
	    puts stdout "Failed to load functional image"
	}
	ShowProgressVal "Loading Subject Image(s) $sub" $progress
	set progress [ expr $progress +$scalefactor ]

    }

    ShowProgressVal  "Done"  1.0
    update idletasks

    SetNewSubject 0

    OutputListCallback $output_listbox $output_listbox_lastselected 
    ShowProgressVal  "Done" 0.0
    $this SetOverlayCreatorInputs 
}

# ------------------------------------------------------------------
# Compute Averages
# ------------------------------------------------------------------

itcl::body pxitclmultisubjectaverage::ComputeTscoreMap {  } {

    set immean [ lindex $outputimages 2 ]
    set imvar  [ lindex $outputimages 3 ]
    
    if { [ $immean GetImageSize ] < 2  } {
	::pxtclutil::Warning "No Composite Task In Memory"
	return 0;
    }

    if { [ $imvar GetImageSize ] < 2  }  {
	::pxtclutil::Warning "No Composite Task Standard Deviation In Memory"
	return 0;
    }
    
    set cast [ vtkImageCast [ pxvtable::vnewobj ]]
    set cast2 [ vtkImageCast [ pxvtable::vnewobj ]]

    $cast  SetInput [ $immean GetImage ]
    $cast2 SetInput [ $imvar  GetImage ]
    $cast  SetOutputScalarTypeToFloat
    $cast2 SetOutputScalarTypeToFloat

    $cast  Update
    $cast2 Update

    set tcomp [  vtkpxComputeTmap [ pxvtable::vnewobj ]]
    $tcomp SetInput [ $cast GetOutput ]
    $tcomp SetInputStandardDeviation [ $cast2 GetOutput ]
    $tcomp SetNumberOfInputs 1
    $tcomp SetNumberOfSamples1 [ llength $subjects ]
    SetFilterCallbacks $tcomp  "Computing Composite TMap"
    $tcomp Update

    set tscoreimage [ lindex $outputimages 4 ]
    $tscoreimage ShallowCopyImage [ $tcomp GetOutput]
    [ $tscoreimage GetImage ] SetOrigin 0.0 0.0 0.0

    $tscoreimage CopyImageHeader [ $immean GetImageHeader ]
    if { $thisparam($this,autosave) == 1 } {
	if { [ $tscoreimage cget -filename ] == "" } {
	    $tscoreimage Save "" "T-Score Map"
	} else {
	    $tscoreimage Save
	}
    }
    $tcomp Delete 
    $cast Delete
    $cast2 Delete
    OutputListCallback $output_listbox 0
    $this SetOverlayCreatorInputs
}

# --------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::SetCurrentTask   { number }  {

    set numtasks [llength $tasks]
    set c $number
    if { $c < 0 || $c >= $numtasks } {
	set c 0
    }
    $task_listbox selection clear 0 [ expr $numtasks -1 ]
    $task_listbox selection set $c $c
    ChangeCurrentTask $task_listbox "vocal"
    set newtname [ [ lindex $tasks $c ] cget -task_name ]
    set newtsuffix [ [ lindex $tasks $c ] cget -task_suffix ]

    return [ list $c $newtname $newtsuffix ]
}

itcl::body pxitclmultisubjectaverage::SetInterpolationAndResolution { interp resol } {

    set thisparam($this,interpolation) $interp
    switch -exact -- $resol {
	"Ref" { set thisparam($this,outputresolution) "Ref" }
	"1.5" { set thisparam($this,outputresolution) "1.5x1.5x1.5"}
	"2.0" { set thisparam($this,outputresolution) "2.0x2.0x2.0"}
	"3.0" { set thisparam($this,outputresolution) "3.0x3.0x3.0"}
	"4.5" { set thisparam($this,outputresolution) "4.5x4.5x4.5"}
    }

    set thisparam($this,autosave) 0
}

# --------------------------------------------------------------
# ---------------- Compute Averages ------ -------- ------------
# --------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::ComputeAverages { mode  } {
    

    set referenceImage [ $reference_control GetObject ]

    set beginsub 0
    set endsub   [ expr [ llength $subjects ]  -1 ]

    set interp 0
    if { $thisparam($this,interpolation) == "Linear" } {
	set interp 1 
    } elseif { $thisparam($this,interpolation) == "Cubic" } {
	set interp 3
    }
    
    if { $endsub <= $beginsub } {
	::pxtclutil::Warning "Cannot Average Images as specified ($beginsub:$endsub)!"
	return;
    }


    set ref_image [ $referenceImage  GetImage ]
    set delete_ref_image 0
    
    if { $thisparam($this,outputresolution) != "Ref" } {
	set sp 1.0
	if { $thisparam($this,outputresolution) == "2.0x2.0x2.0" } {
	    set sp 2.0
	} elseif { $thisparam($this,outputresolution) == "3.0x3.0x3.0" } {
	    set sp 3.0
	} elseif { $thisparam($this,outputresolution) == "4.5x4.5x4.5" } {
	    set sp 4.5
	} elseif { $thisparam($this,outputresolution) == "1.5x1.5x1.5" } {
	    set sp 1.5
	}
	ShowProgressVal "Resampling Reference to $sp x $sp x $sp" 0.01
	set resl [ vtkImageResample [ pxvtable::vnewobj ] ]
	$resl SetAxisOutputSpacing 0 $sp
	$resl SetAxisOutputSpacing 1 $sp
	$resl SetAxisOutputSpacing 2 $sp
	$resl InterpolateOff
	$resl SetDimensionality 3
	$resl SetInput $ref_image 
	$resl Update

	set ref_image [ vtkImageData [ pxvtable::vnewobj ]]
	$ref_image ShallowCopy [ $resl GetOutput ]
	$resl Delete
	set delete_ref_image 1
    }

    if { $debugprogress == 1 } {
	puts stdout "ref_image  Dimensions = [ $ref_image GetDimensions ]"
    }

    set util [ vtkpxUtil [ pxvtable::vnewobj ]]	
    
    set scalefactor [expr 2.0 / ( 2.0 + 2.0* ( $endsub-$beginsub +1 )) ]
    set progress 0.0
    
    switch -exact -- $mode { 
	"Images"  {
	    set outputMean [ lindex $outputimages 0 ]
	    set outputStd  [ lindex $outputimages 1 ]
	    set value   -1.0
	    set valueout 0.0
	    set averaging "Anatomical"
	}
	"Functional" {
	    set outputMean [ lindex $outputimages 2 ]
	    set outputStd  [ lindex $outputimages 3 ]
	    set value -32767
	    set valueout 0.0
	    set averaging "Functional"
	}
	"Composite" {
	    set outputMean [ lindex $outputimages 5 ]
	    set outputStd  0
	    set value -32767
	    set valueout -1.0
	    set averaging "Composite"
	}

    }

    set combofmri [  vtkpxAverageImages [ pxvtable::vnewobj ]]
    $combofmri SetMedian 0
    $combofmri SetIgnoreValue $value
    $combofmri SetIgnoreValueOutput $valueout
    $combofmri IgnoreFlagOn
    
    for { set i $beginsub } { $i <= $endsub } { incr i } {
	
	ShowProgressVal "Computing Averages image=$i" $progress 
	
	set subject [ lindex $subjects $i ]
	set clean_trans 1
	if { $averaging == "Functional" || $mode == "Composite" } {
	    set trans [ vtkGeneralTransform [ pxvtable::vnewobj ]]
	    $trans PostMultiply
	    $trans Identity
	    $trans Concatenate [ [ $subject cget -reference_transform ] GetTransformation ]
	    $trans Concatenate [ [ $subject cget -internal_transform ] GetTransformation ]
	    $trans Concatenate [ [ $subject cget -distortion_transform ] GetTransformation ]
	} else {
	    set trans [ [ $subject cget -reference_transform ] GetTransformation ]
	    set clean_trans 0
	}
	
	set clean_trn_image 0
	
	switch -exact -- $mode { 
	    "Images" { 
		set trn_image [ [ $subject cget -anatomical_image ] GetImage ] 
	    }
	    "Functional" { 	
		set trn_image [ [ $subject cget -functional_image ] GetImage ] 
	    }
	}
	
	set tmp_im($i)  [ vtkImageData [pxvtable::vnewobj ] ]
	
	$util ResliceImage $tmp_im($i) $trn_image $ref_image $trans $interp $value
	set progress [ expr $progress+$scalefactor ]

	if { $debugprogress == 1 } {
	    puts stdout "Image $i dimensions = [ $tmp_im($i) GetDimensions ] "
	}

	$combofmri AddInput $tmp_im($i)

	if { $clean_trans == 1 } {
	    $trans Delete
	}
	if { $clean_trn_image == 1 } {
	    $trn_image Delete
	}


    }
	
    ShowProgressVal  "Computing Averages image=$i" $progress 
    set progress [ expr $progress+$scalefactor ]
    if { $outputStd != 0 } {
	$combofmri ComputeStandardDeviationOn
    }

    #puts stderr "On to combofmri Update"
    $combofmri Update
    #puts stderr "On to combofmri Done"
    ShowProgressVal "Computed Averages image=$i" $progress 

    $outputMean ShallowCopyImage [ $combofmri GetOutput ]

    [ $outputMean GetImage ] SetOrigin 0.0 0.0 0.0
    $outputMean CopyImageHeader [ $referenceImage GetImageHeader ] 
    if { $thisparam($this,autosave) == 1 } {
	#puts stdout "Saving Mean"
	if { [ $outputMean cget -filename ] == "" } {
	    $outputMean Save "" "Average Functional Map"
	} else {
	    $outputMean Save 
	}
    }
    
    if { $outputStd !=0 } {
	$outputStd ShallowCopyImage [ $combofmri GetOutputStandardDeviation ]
	[ $outputStd GetImage ] SetOrigin 0.0 0.0 0.0
	$outputStd CopyImageHeader [ $referenceImage GetImageHeader ] 
        if { $thisparam($this,autosave) == 1 } {
	    #   puts stdout "Saving STD"
	    if { [ $outputStd cget -filename ] == "" } {
		$outputStd Save "" "STD Functional Map"
	    } else {
		$outputStd Save 
	    }
	}
    }
	
    for { set i $beginsub } { $i <= $endsub } { incr i } {
	$tmp_im($i) Delete
    }
    
    if { $mode == "Functional" } {
	#puts stdout "Computing Tscore MAP"
	ComputeTscoreMap
    }
    #puts stdout "Cleaning Up"
    $combofmri Delete
    ShowProgressVal  "Done"  1.0
    #puts stdout "Cleaning Up 2"
    if { $delete_ref_image ==  1 } {
	$ref_image Delete 
    }
    
    $util Delete 
    #puts stdout "Cleaning Up 3"
    $this SetOverlayCreatorInputs
    #puts stdout "Cleaning Up 4"
    ShowProgressVal  "Done" 0.0
}

# ---------------------------------------------------------------------------
# mode = warp which returns a list of warped images or save which saves them!
# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::WarpTasksToCommonSpace { mode { dirname "" } { saveanatomical 1 } } {

    set savingfiles 1
    set imagelist ""


    if { $mode == "warp" } {
	set savingfiles 0
    } else {
	# ------------------- Save Code ---------------------------------------
	if { [ string length $dirname ] < 1 } {
	    set dirname  [tk_chooseDirectory -title "Select Directory to output warped tasks Setup File" ]
	}
	
	if { [ string length $dirname ] < 1 } {
	    return 0
	}
	
	if { [ file exists $dirname ] == 0 } {
	    catch { file mkdir $dirname }
	}
	if { [ file exists $dirname ] == 0 } {
	    ::pxtclutil::Warning "Cannot create directory $dirname"
	    return 0 
	}
	
	if { [ file writable $dirname ] == 0 } {
	    ::pxtclutil::Warning "Cannot write in directory $dirname"
	    return 0 
	}

	set tail1 "Common"
	set fname_base [ file join $dirname $tail1 ]
	# ------------------- End Save Code ---------------------------------------
    }

    set referenceImage  [ $reference_control GetObject ]

    set beginsub 0
    set endsub   [ expr [ llength $subjects ]  -1 ]

    set interp 0
    if { $thisparam($this,interpolation) == "Linear" } {
	set interp 1 
    } elseif { $thisparam($this,interpolation) == "Cubic" } {
	set interp 3
    }
    
    if { $endsub <= $beginsub } {
	::pxtclutil::Warning "Cannot Warp Task Images as specified ($beginsub:$endsub)!"
	return;
    }

    set ref_image [ $referenceImage  GetImage ]
    set delete_ref_image 0
    
    if { $thisparam($this,outputresolution) != "Ref" } {
	set sp 1.0
	if { $thisparam($this,outputresolution) == "2.0x2.0x2.0" } {
	    set sp 2.0
	} elseif { $thisparam($this,outputresolution) == "3.0x3.0x3.0" } {
	    set sp 3.0
	} elseif { $thisparam($this,outputresolution) == "4.5x4.5x4.5" } {
	    set sp 4.5
	} elseif { $thisparam($this,outputresolution) == "1.5x1.5x1.5" } {
	    set sp 1.5
	}
	ShowProgressVal "Resampling Reference to $sp x $sp x $sp" 0.01
	set resl [ vtkImageResample [ pxvtable::vnewobj ] ]
	$resl SetAxisOutputSpacing 0 $sp
	$resl SetAxisOutputSpacing 1 $sp
	$resl SetAxisOutputSpacing 2 $sp
	$resl InterpolateOff
	$resl SetDimensionality 3
	$resl SetInput $ref_image 
	$resl Update

	set ref_image [ vtkImageData [ pxvtable::vnewobj ]]
	$ref_image ShallowCopy [ $resl GetOutput ]
	$resl Delete
	set delete_ref_image 1
    }

    if { $debugprogress == 1 } {
	puts stdout "ref_image  Dimensions = [ $ref_image GetDimensions ]"
    }


    set ctask $thisparam($this,current_task)

    if { $savingfiles == 1 } {
	# -------------------  Save Code ---------------------------------------
	set fout [ open ${fname_base}_${ctask}_log.txt w ]
	
	puts $fout "Using Setup File: $setupfilename"
	puts $fout "Saving task $thisparam($this,current_task) warped to reference space as defined by [ $referenceImage cget -filename ]"
	puts $fout "Reference space dimensions [ $ref_image GetDimensions ], resolution [ $ref_image GetSpacing ]"
	set anasave [ vtkpxAnalyzeImageWriter [ pxvtable::vnewobj ]]
	$anasave SetImageHeader [ $referenceImage GetImageHeader ]

	set hd [ $anasave GetImageHeader ]
	set md [ $hd GetMode ]

	#	puts stderr "ref hd=[ $hd GetMode ]"

	# This is probably overkill
	if { $md ==0 && $forceimageoutputformat == 2 } {
	    $hd SetModeToNIFTI_HDRIMG
	} elseif { $md > 0 && $forceimageoutputformat == 1 } {
	    $hd SetModeToAnalyze
	}
	
	set md [ $hd GetMode ]
	switch -exact -- $md {
	    "0" { set outmode "Analyze" }
	    "1" { set outmode "Nifti .hdr/.img" }
	    "2" { set outmode "Nifti .nii"}
	}

	#    $anasave SetOrientation [ $referenceImage GetOrientation ]

	if { $saveanatomical == 1 } {
	    $anasave SetInput $ref_image
	    $anasave Save "${fname_base}_reference"

	    puts $fout "Saving anatomical reference in ${fname_base}_reference (mode=$outmode)"
	    pxtkprint "Saving anatomical reference in ${fname_base}_reference (mode=$outmode)\n"
	} 
	# -------------------  End Save Code ---------------------------------------
    }
    set util [ vtkpxUtil [ pxvtable::vnewobj ]]	
    
    set scalefactor [expr 1.0 / ( 1.0 + 1.0* ( $endsub-$beginsub +1 )) ]
    set progress 0.0
        
    for { set i $beginsub } { $i <= $endsub } { incr i } {
	
	ShowProgressVal "Warping Tasks image=$i" $progress 
	set subject [ lindex $subjects $i ]
	set clean_trans 1
	set value 0.0

	set trans [ vtkGeneralTransform [ pxvtable::vnewobj ]]
	$trans PostMultiply
	$trans Concatenate [ [ $subject cget -reference_transform ] GetTransformation ]
	$trans Concatenate [ [ $subject cget -internal_transform ] GetTransformation ]
	$trans Concatenate [ [ $subject cget -distortion_transform ] GetTransformation ]
	set trn_image [ [ $subject cget -functional_image ] GetImage ] 
	puts stdout "\nDesc = [ [ $subject cget -functional_image ]  GetShortDescription ]"

	set tmpimage($i)  [ vtkImageData [pxvtable::vnewobj ] ]

	
	set dd [ $trn_image GetNumberOfPoints ]
	if { $dd > 2 } {
	    set ok [ $util ResliceImage $tmpimage($i) $trn_image $ref_image $trans $interp $value ]
	} else {
	    set ok 0
	    puts stderr "Bad Task image [ $trn_image GetDimensions ]"
	}

	if { $ok == 0 } {
	    puts stderr "Making up an empty image to replace this ...."
	    $tmpimage($i) CopyStructure $ref_image
	    $tmpimage($i) SetNumberOfScalarComponents 1
	    if { $i > $beginsub } {
		$tmpimage($i) SetScalarType [ $tmpimage($beginsub) GetScalarType ]
	    }
	    $tmpimage($i) AllocateScalars
	    [ [ $tmpimage($i) GetPointData ] GetScalars ] FillComponent 0 0.0 
	}



	
	set progress [ expr $progress+$scalefactor ]

	if { $debugprogress == 1 } {
	    puts stdout "Image $i dimensions = [ $tmpimage($i) GetDimensions ] "
	}

	if { $savingfiles == 1 } {
	    # -------------------   Save Code ---------------------------------------

	    set subjnamef [ file tail [ [ $subject cget -functional_image ] cget -filename ] ]
	    set md 2
	    set ext [ file extension $subjnamef ]	
	    if { $ext == ".gz" } {
		set subjnamef [ file rootname $subjnamef ]
		set ext [ file extension $subjnamef ]	
		if { $ext == ".nii" } {
		    set ext ".nii.gz"
		}
	    } 
	    set subjnamef [ file rootname $subjnamef ]

	    set hd [ $anasave GetImageHeader ]
	    $hd CopyExtensionsOnly  [ [ $subject cget -functional_image ] GetImageHeader ]
	    
	    if { $ext ==".hdr" && $forceimageoutputformat == 2 } {
		set md 1
	    } elseif { $forceimageoutputformat == 1 } {
		set md 0
	    }  elseif { $ext == ".nii.gz"  || $forceimageoutputformat == 2 } {
		set md 2
	    } 

	    switch -exact -- $md {
		"0" { set outmode "Analyze"; set ext ".hdr";  $hd SetModeToAnalyze }
		"1" { set outmode "Nifti .hdr/.img"; set ext ".hdr" ; $hd SetModeToNIFTI_HDRIMG }
		"2" { set outmode "Nifti .nii"; set ext ".nii.gz" ; $hd SetModeToNIFTI_NII }
	    }

	    set finalname "${fname_base}_${subjnamef}_orig${ext}"
	    
	    $anasave SetInput $tmpimage($i)
	    $anasave Save $finalname
	    puts $fout "Saving warped task for subject [ expr $i +1 ]  in ${finalname} (mode=$outmode)"
	    pxtkprint  "Saving warped task for subject [ expr $i +1 ]  in ${finalname} (mode=$outmode)\n"
#	    puts stderr "Deleting temporary image $i"
	    $tmpimage($i) Delete
	    # ------------------- End Save Code ---------------------------------------
	} else {
	#    puts stderr "Appending temporary image $i"
	    lappend imagelist $tmpimage($i) 
	}
	$trans Delete
    }

    if { $savingfiles == 1 } {
	# ------------------- Save Code ---------------------------------------
	close $fout
	$anasave Delete 
	set imagelist 0
	# ------------------- End Save Code ---------------------------------------
    }

    if { $delete_ref_image == 1 } {
	$ref_image Delete
    }

    $util Delete 
    ShowProgressVal  "Done" 0.0

    #    puts stderr "Returning $imagelist"

    return $imagelist
}

# ------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::DoVOIAnalysis { fname appendmode } {
    
    if { [ string length $fname ] < 1 } {
	set fname  [tk_getSaveFile -title "Output VOI File" -filetypes { {"VOI Text File" {.txt}}} ]
    }
    
    if { [ string length $fname ] < 1 } {
	return 0
    }

    set referenceImage  [ $reference_control GetObject ]
    set voiImage  [ $voi_control GetObject ]

    set beginsub 0
    set endsub   [ expr [ llength $subjects ]  -1 ]

    set interp 0
    if { $thisparam($this,interpolation) == "Linear" } {
	set interp 1 
    } elseif { $thisparam($this,interpolation) == "Cubic" } {
	set interp 3
    }
    
    if { [ $voiImage GetImageSize ] < 2 } {
	::pxtclutil::Warning "Cannot Do VOI Analysis. Bad VOI Image"
	return;
    }

    set ref_image [ $referenceImage  GetImage ]
    if { [ $referenceImage GetImageSize ] < 2 } {
	::pxtclutil::Warning "Cannot Do VOI Analysis. Bad Reference Image"
	return;
    }

    
    set util [ vtkpxUtil [ pxvtable::vnewobj ]]	
    ShowProgressVal "Resampling VOI Image to Match Reference (using identity transform)" 0.01

    set voi_img  [ vtkImageData [pxvtable::vnewobj ] ]
    set ident    [ vtkIdentityTransform  [pxvtable::vnewobj ] ]
    $util ResliceImage $voi_img [ $voiImage GetImage ] $ref_image $ident 0 -1
    $ident Delete

    
    # ---------------------------------------------
    # Somewhere here add code for voi names .......
    # ---------------------------------------------

    set mname [ $voiImage cget -filename ]
    set tname [ ::pxtclutil::GenerateVOIFileNameFromImageName $mname ]
    set mlist [ ::pxtclutil::LoadVOINamesList $tname 255 ]
    array set voinames $mlist



    if { $debugprogress == 1 } {
	puts stdout "ref_image  Dimensions = [ $ref_image GetDimensions ]"
    }

    set thr [ vtkpxMergeFmriConventional [ pxvtable::vnewobj ]]
    set arr [ vtkFloatArray [ pxvtable::vnewobj ]]

    set ctask $thisparam($this,current_task) 
    if { $fname != "_do_not_save" } {
	if { $appendmode == 0 } {
	    set fout [ open $fname w ]
	    #       puts $fout "\#Subj ID\t Image Name \t Task \t Level \t Mean \t Sigma \t nvox \t volume(mm^3)\n"
	    #       pxtkconsole
	    #       pxtkprint "Starting VOI Analysis"
	} else {
	    set fout [ open $fname a ]
	}
    } else {
	set fout 0
    }
    
    pxtkprint "\#Doing VOI Analysis on task $thisparam($this,current_task)\n"
    pxtkprint "\#Reference space dimensions [ $ref_image GetDimensions ], resolution [ $ref_image GetSpacing ]\n"
    #    pxtkprint "\#Subj ID\t Image Name \t Task \t Level \t Mean \t Sigma \t nvox \t volume(mm^3)\n"


    set scalefactor [expr 1.0 / ( 1.0 + 1.0* ( $endsub-$beginsub +1 )) ]
    set progress 0.0

    set ident [ vtkIdentityTransform [pxvtable::vnewobj ]]

    set outimg [ vtkImageData New ]
    $outimg SetScalarTypeToFloat

        
    for { set i $beginsub } { $i <= $endsub } { incr i } {
	
	ShowProgressVal "Warping Tasks image=$i" $progress 
	set subject [ lindex $subjects $i ]
	set clean_trans 1
	set value 0.0

	set trans [ vtkGeneralTransform [ pxvtable::vnewobj ]]
	$trans PostMultiply
	$trans Concatenate [ [ $subject cget -reference_transform ] GetTransformation ]
	$trans Concatenate [ [ $subject cget -internal_transform ] GetTransformation ]
	$trans Concatenate [ [ $subject cget -distortion_transform ] GetTransformation ]
	set trn_image [ [ $subject cget -functional_image ] GetImage ] 
	set name      [ $subject  GetBaseName ]
	
	set tmpimage($i)  [ vtkImageData [pxvtable::vnewobj ] ]
	$util ResliceImage $tmpimage($i) $trn_image $ref_image $trans $interp $value
	set progress [ expr $progress+$scalefactor ]

	set numberofframes [ $tmpimage($i) GetNumberOfScalarComponents ]
	
	if { $debugprogress == 1 } {
	    puts stdout "Image $i dimensions = [ $tmpimage($i) GetDimensions ] "
	}
	
	puts stderr "Doing ROI Stats [ $tmpimage($i) GetDimensions ]"
	set nt [  $thr ComputeROIStatistics $tmpimage($i) $voi_img $ident $arr ]
	set subjnamef [ file root [ file tail [ [ $subject cget -functional_image ] cget -filename ] ] ]
	
#	for { set aa 0 } { $aa < [ $arr GetNumberOfTuples ] } { incr aa } {
#	    for { set ab 0 } { $ab < [ $arr GetNumberOfComponents ] } { incr ab } {
#		puts stdout "Component $aa,$ab= [ $arr GetComponent $aa $ab]"
#	    }
#	}


	if { $i == $beginsub } {
	    #	    puts stdout "Output ROI Image Dimensions = [ expr $endsub-$beginsub +1 ] $numberofframes [ $arr GetNumberOfTuples ]"
	    $outimg SetDimensions [ expr $endsub-$beginsub + 2 ] [ expr $numberofframes + 1 ] [ expr [ $arr GetNumberOfTuples ] + 1 ]
	    $outimg AllocateScalars 
	    [ [ $outimg GetPointData ] GetScalars ] FillComponent 0 0.0
	}

	if { $appendmode == 0 && $i == $beginsub } {
	    set line "\#Subj ID\t Image Name \t Task "
	    if { $numberofframes > 1 } {
		set line "$line\t Frame"
	    }
	
	    for { set j 0 } { $j < [ $arr GetNumberOfTuples ] } { incr j } {
		set n1 [ expr int([ $arr GetComponent $j 2 ])]
		set a2 [ expr round([ $arr GetComponent $j 1 ])]
		set frame [ $arr GetComponent $j 0 ]
		set rname "Region $a2"
		catch { set rname $voinames($a2) }
		if { $n1 > 0 && $a2 > 0 && $frame == 0 } {
		    set line "$line \t $rname "
		} else {
		    puts stderr "Ignoring VOI $rname "
		}
		    
	    }
	    if { $fout !=0 } {	    puts $fout "$line\n" }
	    pxtkprint "$line\n"
	}
	
	set front "$name \t $subjnamef \t $ctask"
	puts stdout $front
	
	for { set fr 0 } { $fr < $numberofframes } { incr fr } { 
	    pxtkprint "$front"
	    if { $fout != 0 } {	puts -nonewline $fout $front } 

	    if { $numberofframes > 1 } {
		set line [ expr $fr +1 ]
		if { $fout !=0 } { puts  -nonewline $fout "\t$line" }
		pxtkprint "\t$line"
	    }
	    
	    for { set j 0 } { $j < [ $arr GetNumberOfTuples ] } { incr j } {
		set mean  [ $arr GetComponent $j 3 ]
		set frame [ $arr GetComponent $j 0 ]
		set n1 [ expr int([ $arr GetComponent $j 2 ])]
		set a2 [ expr round([ $arr GetComponent $j 1 ])]
		set rname "Region $a2"
		catch { set rname $voinames($a2) }

		if { $fr == $frame && $n1 > 0 && $a2 > 0 } {
		    set line [ format "\t%9.4f" $mean ]
		    if { $fout !=0 } { puts  -nonewline $fout "$line" }
		    pxtkprint "$line"
		    #puts "$outimg SetScalarComponentFromDouble [ expr $i - $beginsub ] $fr $j 0 $mean"
		    $outimg SetScalarComponentFromDouble [ expr $i - $beginsub ] $fr $j 0 $mean
		} else {
		    puts stderr "Ignoring VOI $rname frame=$fr, n1=$n1 "
		}
	    }
	    if { $fout !=0 } { puts $fout " " }
	    pxtkprint "\n"
	}
	$trans Delete
	$tmpimage($i) Delete
    }
    if { $fout !=0 } { 
	close $fout
    }
    $ident Delete
    $voi_img Delete
    $util Delete 
    $arr  Delete 
    $thr  Delete
    ShowProgressVal  "Done" 0.0

    if { $fname == "_do_not_save" } {
	puts stdout "Returning image dimensions = [ $outimg GetDimensions ]"
	return $outimg
    }
    
    $outimg Delete
    return 1

}

# ------------------------------------------------------------------
#   Batch Mode Operations
# ------------------------------------------------------------------

itcl::body pxitclmultisubjectaverage::SelectTasksToProcess { comment } {

    set l ""
    for { set i 0 } { $i < [ llength $tasks ] } { incr i } {
	set tmp [ lindex $tasks $i ]
	lappend l "[ $tmp cget -task_name ]:[$tmp cget -task_suffix ]"
    }
    
    $batch_selector Activate $l "" $comment "Select Tasks"
    return [ $batch_selector GetSelectedIndices ]
}

itcl::body pxitclmultisubjectaverage::SelectSubjectsToProcess { comment } {

    set l ""
    for { set i 0 } { $i < [ llength $subjects ] } { incr i } {
	set tmp [ lindex $subjects $i ]
	lappend l [ $tmp GetBaseName ]
    }
    
    $batch_selector Activate $l "" $comment "Select Subjects"
    return [ $batch_selector GetSelectedIndices ]
}


# ------------------------------------------------------------------------------

itcl::body pxitclmultisubjectaverage::ComputeAllResults { mode } { 

    LoadAll 0
    set numtasks [llength $tasks]
    if { $mode == 0 } {
	set nmode Functional
	set dmode "Average Tmaps"
    }
    
    set comment  "Select taks to compute $dmode images using the current parameters. This will take a while! Are you sure?"
    set lst [ SelectTasksToProcess $comment ]

    if { [ string length $lst ] ==0 } {
	return
    }

    set thisparam($this,autosave) 1
    pxtkconsole

    for { set i 0 } { $i < [ llength $lst ] } { incr i } {
	set c [ lindex $lst $i ]
	pxtkprint "Beginning Computation for task [ expr $c +1 ] ($lst)"
	$task_listbox selection clear 0 [ expr $numtasks -1 ]
	$task_listbox selection set $c $c
	ChangeCurrentTask $task_listbox "silent"
	ComputeAverages $nmode
    }
}
# ------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::Create4DImageForSingleFrameAndCurrentTask { imglist fr  { roimeanalg 0 } } {

    set appnd [ vtkImageAppendComponents New ]	
    set nc [ [ lindex $imglist 0 ] GetNumberOfScalarComponents ]
    #    pxtkprint "\nCreating 4D Image for task [ expr $c +1 ], $ctask and frame [ expr $fr + 1 ]\n"
    set numfiles [ llength $imglist ]
    for { set k 0 } { $k < $numfiles } { incr k } {
	set img [ lindex $imglist $k ]
	if { $nc > 1 } {
	    #		    puts stderr "fr=$fr, k=$k, nc=$nc, img=$img"
	    #		    puts stderr "img=$img [ $img GetClassName ] [ $img GetDimensions ], [ $img GetNumberOfScalarComponents ]"
	    set ext [ vtkImageExtractComponents  New ]
	    $ext SetInput  $img
	    $ext SetComponents $fr
	    $ext Update
	    $appnd AddInput [ $ext GetOutput ]
	    $ext Delete
	} else {
	    $appnd AddInput  $img
	}
    }
    $appnd Update
    
    set comb [ [ pxitclimage \#auto ] GetThisPointer ]
    $comb ShallowCopyImage [ $appnd GetOutput ]

    set referenceImage [ $reference_control GetObject ]
    $comb CopyImageHeader [ $referenceImage GetImageHeader ]
    
    if { $roimeanalg != 0 } {
	#	puts stdout "Here [ $comb GetShortDescription ]"
	$roimeanalg SetInput $comb
	set voiImage  [ $voi_control GetObject ]
	$roimeanalg SetSecondInput $voiImage
	$roimeanalg SetOptionValue dotextfile 0
	$roimeanalg SetOptionValue addregion 0
	$roimeanalg SetOptionValue fullsizeoutput 1
	$roimeanalg Execute
	$comb ShallowCopy [ $roimeanalg GetOutput ]
    }
    $appnd Delete
    return $comb
}
# ------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::Create4DImagesAllTasks { mode } { 

    LoadAll 0
    set lst [ SelectTasksToProcess "Select taks to warp individual subject task images to the create common 4d images using the current transformations and resolution settings. This will take a while! Are you sure?"]
    if { [ string length $lst ] == 0 } {
	return 0
    }

    set dirname  [tk_chooseDirectory -title "Select Directory to output warped tasks Setup File" ]
    if { [ string length $dirname ] < 1 } {
	return 0
    }

    set oldmode $thisparam($this,outputresolution) 
    set roimeanalg 0
    if { $mode == "voi" } {
	set thisparam($this,outputresolution) "Ref"
	set voiImage  [ $voi_control GetObject ]
	if { [ $voiImage GetImageSize ] < 2 } {
	    ::pxtclutil::Warning "Cannot Do VOI Warp. Bad VOI Image"
	    return 0
	}
    	set roimeanalg [ [ bis_roimean \#auto ] GetThisPointer ]
	$roimeanalg InitializeFromContainer $this
    }

    set numtasks [llength $tasks]
    pxtkconsole

#    puts stderr "lst=$lst"

    for { set i 0 } { $i < [ llength $lst ] } { incr i } {
	set c [ lindex $lst $i ]
	$task_listbox selection clear 0 [ expr $numtasks -1 ]
	$task_listbox selection set $c $c
	ChangeCurrentTask $task_listbox "silent"
	set imglist [ WarpTasksToCommonSpace warp $dirname 0 ]

	set numfiles [ llength $imglist ]
	set ctask $thisparam($this,current_task)	    

	set nc [ [ lindex $imglist 0 ] GetNumberOfScalarComponents ]
	set sname [  file tail [ file rootname $setupfilename ]]
		
	for { set fr 0 } { $fr < $nc } { incr fr } {
	    set comb [ $this Create4DImageForSingleFrameAndCurrentTask $imglist $fr $roimeanalg ]
	    
	    set suffix $ctask
	    if { $nc > 1 } {
		set suffix [ format "%s_%03d" $ctask [ expr $fr +1 ] ]
	    }

	    if { $mode == "voi" } {
		append suffix "_voi"
	    }
	    set fname [ file join $dirname "${sname}_${suffix}.nii.gz" ]

	    set ok [ $comb Save $fname ]
	    itcl::delete object $comb 
	    
	    if { $fr == 0 } {
		set tname [ file join $dirname "${sname}_${ctask}.txt" ]
		set fileid [open $tname w]
		puts $fileid "\# Subject Order "
		for { set k 0 } { $k < $numfiles } { incr k } {
		    set tmp [ lindex $subjects $k ]
		    puts $fileid "[ $tmp cget -subject_id ] \t [ [ $tmp cget -functional_image ] cget -filename ] \t [ [ [ $tmp cget -functional_image ] GetImage ] GetDimensions ]"
		}
		close $fileid
		pxtkprint "\t Combined Saved in $fname, details in $tname\n"
	    } else {
		pxtkprint "\t Combined Saved in $fname\n"
	    }
	}
	
	# cleanup
	for { set ai 0 } { $ai < [ llength $imglist ] } { incr ai } {
	    [ lindex $imglist $ai ] Delete
	}
    }

    set thisparam($this,outputresolution)  $oldmode
    if { $roimeanalg !=0 } {
	itcl::delete object $roimeanalg
    }
}
# ------------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::Create4DImagesAllTasksOld { mode } { 

#    puts stdout "Loading all ............."
    LoadAll 0
#    puts stdout "Loading all done ............."
    set lst [ SelectTasksToProcess "Select taks to warp individual subject task images to the create common 4d images using the current transformations and resolution settings. This will take a while! Are you sure?"]
    if { [ string length $lst ] == 0 } {
	return 0
    }

    set dirname  [tk_chooseDirectory -title "Select Directory to output warped tasks Setup File" ]
    if { [ string length $dirname ] < 1 } {
	return 0
    }

    set oldmode $thisparam($this,outputresolution) 
    set roimeanalg 0
    if { $mode == "voi" } {
	set thisparam($this,outputresolution) "Ref"
	set voiImage  [ $voi_control GetObject ]
	if { [ $voiImage GetImageSize ] < 2 } {
	    ::pxtclutil::Warning "Cannot Do VOI Warp. Bad VOI Image"
	    return 0
	}
    	set roimeanalg [ [ bis_roimean \#auto ] GetThisPointer ]
	$roimeanalg InitializeFromContainer $this
    }

    set numtasks [llength $tasks]
    pxtkconsole

#    puts stderr "lst=$lst"

    for { set i 0 } { $i < [ llength $lst ] } { incr i } {
	set c [ lindex $lst $i ]
	$task_listbox selection clear 0 [ expr $numtasks -1 ]
	$task_listbox selection set $c $c
#	puts stdout "Changing task ............."
	ChangeCurrentTask $task_listbox "silent"
#	puts stdout "Changing task done ............."
	set imglist [ WarpTasksToCommonSpace warp $dirname 0 ]
#	puts stdout "Warp task done ............."

	set numfiles [ llength $imglist ]
	set ctask $thisparam($this,current_task)	    

	set nc [ [ lindex $imglist 0 ] GetNumberOfScalarComponents ]
	set sname [  file tail [ file rootname $setupfilename ]]
		
	for { set fr 0 } { $fr < $nc } { incr fr } {
	    set appnd [ vtkImageAppendComponents New ]	
	    pxtkprint "\nCreating 4D Image for task [ expr $c +1 ], $ctask and frame [ expr $fr + 1 ]\n"
	    for { set k 0 } { $k < $numfiles } { incr k } {
		set img [ lindex $imglist $k ]
		if { $nc > 1 } {
		    #		    puts stderr "img=$img [ $img GetClassName ] [ $img GetDimensions ], [ $img GetNumberOfScalarComponents ]"
		    set ext [ vtkImageExtractComponents  New ]
		    $ext SetInput  $img
		    $ext SetComponents $fr
		    $ext Update
		    $appnd AddInput [ $ext GetOutput ]
		    $ext Delete
		} else {
		    $appnd AddInput  $img
		}
	    }
	    $appnd Update
	
	    set comb [ [ pxitclimage \#auto ] GetThisPointer ]
	    $comb ShallowCopyImage [ $appnd GetOutput ]
	    
	    set referenceImage [ $reference_control GetObject ]
	    $comb CopyImageHeader [ $referenceImage GetImageHeader ]
	    
	    set suffix $ctask
	    if { $nc > 1 } {
		set suffix [ format "%s_%03d" $ctask [ expr $fr +1 ] ]
	    }

	    if { $mode == "voi" } {
		append suffix "_voi"
		
		$roimeanalg SetInput $comb
		$roimeanalg SetSecondInput $voiImage
		$roimeanalg SetOptionValue dotextfile 0
		$roimeanalg SetOptionValue addregion 0
		$roimeanalg SetOptionValue fullsizeoutput 1
		$roimeanalg Execute
		$comb ShallowCopy [ $roimeanalg GetOutput ]
	    }
	    set fname [ file join $dirname "${sname}_${suffix}.nii.gz" ]

	    set ok [ $comb Save $fname ]
	    itcl::delete object $comb 
	    $appnd Delete
	    
	    if { $fr == 0 } {
		set tname [ file join $dirname "${sname}_${ctask}.txt" ]
		set fileid [open $tname w]
		puts $fileid "\# Subject Order "
		for { set k 0 } { $k < $numfiles } { incr k } {
		    set tmp [ lindex $subjects $k ]
		    puts $fileid "[ $tmp cget -subject_id ] \t [ [ $tmp cget -functional_image ] cget -filename ] \t [ [ [ $tmp cget -functional_image ] GetImage ] GetDimensions ]"
		}
		close $fileid
		pxtkprint "\t Combined Saved in $fname, details in $tname\n"
	    } else {
		pxtkprint "\t Combined Saved in $fname\n"
	    }
	}
	
	# cleanup
	for { set ai 0 } { $ai < [ llength $imglist ] } { incr ai } {
	    [ lindex $imglist $ai ] Delete
	}
    }

    set thisparam($this,outputresolution)  $oldmode
    if { $roimeanalg !=0 } {
	itcl::delete object $roimeanalg
    }
}
# ------------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::WarpAllTasks { mode } { 

    LoadAll 0
    set lst [ SelectTasksToProcess "Select taks to warp individual subject task images to the common space using the current transformations and resolution settings. This will take a while! Are you sure?"]
    if { [ string length $lst ] == 0 } {
	return 0
    }

    set dirname  [tk_chooseDirectory -title "Select Directory to output warped tasks Setup File" ]
    if { [ string length $dirname ] < 1 } {
	return 0
    }

    set numtasks [llength $tasks]
    pxtkconsole


    for { set i 0 } { $i < [ llength $lst ] } { incr i } {
	set c [ lindex $lst $i ]
	pxtkprint "Beginning Computation for task [ expr $c +1 ]"
	$task_listbox selection clear 0 [ expr $numtasks -1 ]
	$task_listbox selection set $c $c
	ChangeCurrentTask $task_listbox "silent"
	if { $i == 0 } {
	    WarpTasksToCommonSpace save $dirname 1
	} else {
	    WarpTasksToCommonSpace save $dirname 0
	}
    }
}
# ------------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::DoVOIAnalysisAllTasks { mode } { 

    LoadAll 0
    set lst [ SelectTasksToProcess "Select taks to perform VOI Analysis On" ]
    if { [ string length $lst ] == 0 } {
	return 0
    }

    set fname  [tk_getSaveFile -title "Output VOI File" -filetypes { {"VOI Text File" {.txt}}} ]
    if { [ string length $fname ] < 1 } {
	return 0
    }

    set numtasks [llength $tasks]
    pxtkconsole


    for { set i 0 } { $i < [ llength $lst ] } { incr i } {
	set c [ lindex $lst $i ]
	pxtkprint "Beginning Computation for task [ expr $c +1 ]"
	$task_listbox selection clear 0 [ expr $numtasks -1 ]
	$task_listbox selection set $c $c
	ChangeCurrentTask $task_listbox "silent"
	if { $i == 0 } {
	    DoVOIAnalysis $fname 0
	} else {
	    DoVOIAnalysis $fname 1
	}
    }
}
# ------------------------------------------------------------------------------


# itcl::body pxitclmultisubjectaverage::ComputeAllRegistrations { mode secondmode } { 

#     LoadAll 0
#     set lmode "inter-subject registrations"
#     if { $mode == 2 } {
# 	set lmode "within-subject registrations"
#     }

#     set lst [ SelectSubjectsToProcess "Select subjects to compute $lmode using the current transformations and resolution settings. This will take a long time! Are you sure? (Press Cancel to change the parameters!)"]
#     if { [ string length $lst ] == 0 } {
# 	$external_overlayutility DisableRegistrationButtons
# 	if { $mode == "nonlinear" } {
# 	    $external_overlayutility ShowWindow  "NonLinear Reg" 
# 	} elseif { $mode == "distortion" } {
# 	    $external_overlayutility ShowWindow  "Distortion Cor" 
# 	} else {
# 	    $external_overlayutility ShowWindow  "Linear Reg"	
# 	}
# 	return 
#     }

#     set dirname  [tk_chooseDirectory -title "Select Directory to Save Registrations In" ]
#     if { [ string length $dirname ] < 1 } {
# 	return 0
#     }

#     pxtkconsole
#     pxtkprint "\n\nBeginning computation of all $lmode"

#     $external_overlayutility SetAutoSave 0

#     for { set sub 0 } { $sub < [ llength $lst ] } { incr sub } {
	
# 	SetNewSubject [ lindex $lst $sub ]
# 	ComputeRegistrationCallback $mode "donotshowcontrols"
	
# 	if { $secondmode == "nonlinear" } {
# 	    $external_overlayutility ComputeCombinedRegistration "Registration"
# 	} elseif { $secondmode == "distortion" } {
# 	    $external_overlayutility ComputeCombinedRegistration "Distortion"
# 	} else {
# 	    $external_overlayutility ComputeAffineRegistration
# 	}

# 	GrabRegistrationCallback $mode $dirname

# 	pxtkprint "Done with Subject [ expr $sub +1 ] \n ---------------------------------------------------\n\n"
#     }

#     $external_overlayutility SetAutoSave 1
# }

# ------------------------------------------------------------------
#  Verify Registration Stuff
# ------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::VerifyRegistrationCallback { mode args } { 

    switch -exact -- $mode { 
	1 {
	    set image_ref  [ $reference_control GetObject ]
	    set image_trn  [ $anat_control GetObject ] 
	    set tr         [ $refx_control GetObject ]
	} 
	2 {
	    set image_ref [ $anat_control GetObject ] 
	    set image_trn [ $conv_control GetObject ] 
	    set tr        [  $intx_control GetObject ]
	}
	3 { 
	    set image_ref [  $conv_control GetObject ] 
	    set image_trn [  $epi_control GetObject ] 
	    set tr        [  $distx_control GetObject ]
	} 
    }

    set orient_ref [ $image_ref GetOrientation ]
    set orient_trn [ $image_trn GetOrientation ]
    
    set dim1 [ [ $image_ref GetImage ] GetDimensions ]
    set dim2 [ [ $image_trn GetImage ] GetDimensions ]
    if { [lindex $dim2 0 ] <2 || [ lindex $dim1 0 ] < 2 } { 
	::pxtclutil::Warning "No Images in Memory\n Cannot Verify Transformation!"
	return  0
    }

    WatchOn

    set ut [ vtkpxUtil [ pxvtable::vnewobj ]]

    set tmp  [ [  pxitclimage \#auto ] GetThisPointer ]
    $ut ResliceImage [ $tmp GetImage ] [ $image_trn GetImage ] [ $image_ref GetImage ] [ $tr GetTransformation ] 1 0.0
    
    $tmp CopyImageHeader [ $image_ref GetImageHeader ]
    set f1 [ file tail [ file root [ $image_ref cget -filename ]]]

    $tmp configure -filename "warpto_${f1}_[ file tail [$image_trn cget -filename ]]"

    $parent SetImageFromObject $image_ref $this
    $second_viewer SetImageFromObject $tmp $this
    catch { itcl::delete object $tmp }
    
    WatchOff
    return 1
}

# ------------------------------------------------------------------
#  Compute Registration Stuff
# ------------------------------------------------------------------
# itcl::body pxitclmultisubjectaverage::ComputeRegistrationCallback { mode args } { 

#     if { [ llength $args] > 0 } {
# 	set smode [ lindex $args 0 ]
#     }


#     switch -exact -- $mode { 
# 	1 {
# 	    set image_ref [ $reference_control GetObject ]
# 	    set image_trn [ $anat_control GetObject ] 
# 	    set swindow "NonLinear Reg"
# 	} 
# 	2 {
# 	    set image_ref  [ $anat_control GetObject ] 
# 	    set image_trn [ $conv_control GetObject ]
# 	    set swindow "Linear Reg"
# 	}
# 	3 {
# 	    set image_ref [ $conv_control GetObject ]
# 	    set image_trn  [ $epi_control GetObject ] 
# 	    set swindow "Distortion Cor"
# 	} 
	
#     }

#     set dim1 [ [ $image_ref GetImage ] GetDimensions ]
#     set dim2 [ [ $image_trn GetImage ] GetDimensions ]
#     if { [lindex $dim2 0 ] <2 || [ lindex $dim1 0 ] < 2 } { 
# 	::pxtclutil::Warning "No Images in Memory\n Cannot Compute Transformation!"
# 	return  0
#     }

#     $parent SetImageFromObject $image_ref $this
#     $second_viewer SetImageFromObject $image_trn $this
    
#     if { $smode != "donotshowcontrols" } {
# 	$external_overlayutility EnableRegistrationButtons
# 	$external_overlayutility ShowWindow $swindow
#     }

#     return 1
# }
# ------------------------------------------------------------------------------------------------

itcl::body pxitclmultisubjectaverage::GrabRegistrationCallback { mode args } { 

    set pathname ""
    if { [ llength $args ] > 0 } {
	set pathname [ lindex $args 0 ]
    }

    switch -exact -- $mode {
	1 {
	    set t_control $refx_control
	    set image_ref [ $reference_control GetObject ]
	    set image_trn [ $anat_control GetObject ] 
	}
	2 { 
	    set t_control $intx_control
	    set image_ref  [ $anat_control GetObject ] 
	    set image_trn [ $conv_control GetObject ]
	} 
	3 {
	    set t_control $distx_control
	    set image_ref  [ $conv_control GetObject ] 
	    set image_trn [ $epi_control GetObject ]
	}
    }
	
    set tr [ [ $external_overlayutility GetTransformation ] GetTransformation ]
    [ $t_control GetObject ]  CopyTransformation $tr

    if { $pathname == "" } {
	set mname [ [ $external_overlayutility GetTransformation ] cget -filename ]
	if { [ string length $mname  ] > 0 } {
	    [ $t_control GetObject ]  configure -filename $mname 
	    $t_control Update
	    return 1
	} 
	set n1  [ file rootname [ $image_ref cget -filename ]]	
    } else {
	set n1 [ file tail [ file rootname [ $image_ref cget -filename ]]]
    }
    
    set n2 [ file tail [ file rootname [ $image_trn cget -filename ]]] 
    set suffix "matr"

    if { [ $tr IsA "vtkGridTransform" ] == 1 } {
	set suffix "grd"
    } elseif { [ $tr IsA "vtkpxComboTransform" ] == 1 } {
	set suffix "grd"
    } elseif { [ $tr IsA "vtkThinPlateSplineTransform" ] == 1 } {
	set suffix "tps"
    }
    
    set mname [ file join $pathname "${n1}_${n2}.${suffix}" ]
    
    
    set ok 1
    
    if { [ file exists $mname ] == 1} {
	if { [ pxtclutil::Question "Saving Transformation Will overwrite existing file $mname" ] == 1 } {
	    [ $t_control GetObject ] Save $mname 
	} else {
	    [ $t_control GetObject ] configure -filename $mname 
	}
    } else {
	[ $t_control GetObject ] Save $mname 
    }
    $t_control Update
    return 1
}

# ------------------------------------------------------------------
# Compare Tool Stuff
# ------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::ShowCompareTool { } {
    $external_overlayutility ShowWindow  "Image Compare" 
}

itcl::body pxitclmultisubjectaverage::SetInfoToCompareTool { mode2 } {
    


    if { $mode2 ==1 || $mode2 == 2 } {
	$external_overlayutility ShowWindow  "Image Compare" 
	[ $external_overlayutility GetCompareUtility ]  SetCompareMean${mode2}  [ lindex $outputimages 2 ]
	[ $external_overlayutility GetCompareUtility ] SetCompareSigma${mode2} [ lindex $outputimages 3 ]
	[ $external_overlayutility GetCompareUtility ] SetCompareN${mode2}     [ llength $subjects ]
	return
    }

    if { $last_subject == - 1 } {
	::pxtclutil::Warning "No subject is currently selected!"
	return
    }

    set referenceImage [ $reference_control GetObject ]
    set interp 0
    if { $thisparam($this,interpolation) == "Linear" } {
	set interp 1 
    } elseif { $thisparam($this,interpolation) == "Cubic" } {
	set interp 3
    }

    set ref_image [ $referenceImage  GetImage ]
    set delete_ref_image 0
    
    if { $thisparam($this,outputresolution) != "Ref" } {
	set sp 1.0
	if { $thisparam($this,outputresolution) == "2.0x2.0x2.0" } {
	    set sp 2.0
	} elseif { $thisparam($this,outputresolution) == "3.0x3.0x3.0" } {
	    set sp 3.0
	} elseif { $thisparam($this,outputresolution) == "4.5x4.5x4.5" } {
	    set sp 4.5
	} elseif { $thisparam($this,outputresolution) == "1.5x1.5x1.5" } {
	    set sp 1.5
	}
	ShowProgressVal "Resampling Reference to $sp x $sp x $sp" 0.01
	set resl [ vtkImageResample [ pxvtable::vnewobj ] ]
	$resl SetAxisOutputSpacing 0 $sp
	$resl SetAxisOutputSpacing 1 $sp
	$resl SetAxisOutputSpacing 2 $sp
	$resl InterpolateOff
	$resl SetDimensionality 3
	$resl SetInput $ref_image 
	$resl Update

	set ref_image [ vtkImageData [ pxvtable::vnewobj ]]
	$ref_image ShallowCopy [ $resl GetOutput ]
	$resl Delete
	set delete_ref_image 1
    }



    set util [ vtkpxUtil [ pxvtable::vnewobj ]]	
    set value    0.0
    set valueout 0.0

    
    set subject [ lindex $subjects $last_subject ]
    set trans [ vtkGeneralTransform [ pxvtable::vnewobj ]]
    $trans PostMultiply
    $trans Identity
    $trans Concatenate [ [ $subject cget -reference_transform ] GetTransformation ]
    $trans Concatenate [ [ $subject cget -internal_transform ] GetTransformation ]
    $trans Concatenate [ [ $subject cget -distortion_transform ] GetTransformation ]
    set trn_image [ [ $subject cget -functional_image ] GetImage ] 

    set tmp  [ vtkImageData [pxvtable::vnewobj ] ]
    set tmp2  [ vtkImageData [pxvtable::vnewobj ] ]

    set maskim  [ vtkImageData [pxvtable::vnewobj ] ]
    $maskim CopyStructure $trn_image
    $maskim AllocateScalars
    [ [ $maskim GetPointData ] GetScalars ] FillComponent 0 0.05
	
    $util ResliceImage $tmp $trn_image $ref_image $trans $interp $value
    $util ResliceImage $tmp2 $maskim  $ref_image $trans $interp 0.0

    $maskim Delete

    set newtmp  [ [  pxitclimage \#auto ] GetThisPointer ]
    set newtmp2  [ [  pxitclimage \#auto ] GetThisPointer ]
    $newtmp ShallowCopyImage $tmp
    $newtmp CopyImageHeader [ $referenceImage GetImageHeader ]
    $newtmp  configure -filename "Single_Subject_[ $subject GetBaseName]"


    $newtmp2 configure -filename "None_specified"
    $newtmp2 ShallowCopyImage $tmp2


    $external_overlayutility ShowWindow  "Image Compare" 
    [ $external_overlayutility GetCompareUtility ] SetCompareMean2  $newtmp
    [ $external_overlayutility GetCompareUtility ] SetCompareSigma2 $newtmp2
    [ $external_overlayutility GetCompareUtility ] SetCompareN2     1

    catch { itcl::delete object $newtmp  }
    catch { itcl::delete object $newtmp2 }

    if { $delete_ref_image == 1 } {
	$ref_image Delete
    }
    $tmp2 Delete
    $tmp Delete
}



# ------------------------------------------------------------------
# New Task Name
# ------------------------------------------------------------------

itcl::body pxitclmultisubjectaverage::ChangeCurrentTask { tasklistbox args }  {

    set olselect $output_listbox_lastselected 
    set newindex [ $tasklistbox curselection ]
    if { [ llength $newindex ] < 1 } {
	return 0
    }

    set newsuffix [ [ lindex $tasks $newindex ] cget -task_suffix ]
    #puts stderr "New Suffix = $newsuffix, index=$newindex"

    set oldsuffix $thisparam($this,current_task) 
    set mode "vocal"
    if { [ llength $args]  > 0 } {
	set mode [ lindex $args 0 ]
    }


    if { $newsuffix == "" } {
	if { $mode == "vocal" } {
	    puts stderr "No new task selected!"
	}
	return 0
    }

    if { $newsuffix == $oldsuffix } {
#	if { $mode == "vocal" } {
#	    pxtclutil::Warning "New task is the same as the old task!"
#	}
	return 1;
    }

    if { $mode == "vocal" } {
	puts stdout "Selecting new task: changing suffix from $oldsuffix to $newsuffix"
    }
    ShowProgressVal "Renaming functional images" 0.0

    set progress 0.0
    set total [ expr [ llength $subjects ] + 3 ]
    set scalefactor [expr 1.0 / $total ]


    for { set sub 0 } { $sub < $total} { incr sub } {

	if { $sub < [ llength $subjects ] } {
	    set current [ lindex $subjects $sub ] 
	    set functional [ $current cget -functional_image ]
	} else {
	    set index [ expr $sub - [ llength $subjects ] + 2 ]
	    set functional [ lindex $outputimages $index ]
	}

	set f   [ $functional cget -filename ]	
	set f2  $f
	set ind [ string last $oldsuffix $f ]
	set load 0
	
	if { $ind >=0 } {
	    regsub -start $ind $oldsuffix $f $newsuffix f2
	    $functional configure -filename $f2
	    pxtkprint "Renaming [ file tail $f ]  to [ file tail $f2]"
	    set okf 0
	    if { $mode=="vocal" } {
		puts stdout "Renaming [ file tail $f ]  to [ file tail $f2]"
	    }
	    if { [ file exists $f2 ] } {
		set okf [ $functional Load ]
		pxtkprint " ******* and loading\n"
		set load 1
	    } else {
		::pxtclutil::Warning "File $f2 does not exist"
		$functional configure -filename $f2
		set okf 0
	    }

	    if { $okf == 0 } {
		set img [ vtkImageData New ]
		$img SetDimensions 2 2 2
		$img SetSpacing 200 200 200
		$img AllocateScalars
		[ [ $img  GetPointData ] GetScalars ] FillComponent 0 -100000
		$functional ShallowCopyImage $img
		$img Delete
		set load 0
	    }

	    set progress [ expr $progress + $scalefactor ]
	    ShowProgressVal "Renaming functional images [ expr $sub +1 ] (load=$load)" $progress
	}
    }
    SetNewSubject 0

    OutputListCallback $output_listbox $olselect 

    ShowProgressVal "Done" 1.0
    set thisparam($this,current_taskname) [ [ lindex $tasks $newindex ] cget -task_name ]
    set thisparam($this,current_task) $newsuffix

}

# ----------------------------------------------------------------------------------------------
#   Create GUI 
# ----------------------------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::CreateResultsControl { widget } {


    iwidgets::Labeledframe $widget.t1 -labelpos n -labeltext "Output Images"
    frame $widget.t15 -bg black -height 5
    iwidgets::Labeledframe $widget.t2 -labelpos n -labeltext "Compute Output"
    frame $widget.t25 -bg black -height 5
    iwidgets::Labeledframe $widget.t3 -labelpos n -labeltext "Output Overlay"
    pack $widget.t2 $widget.t15 $widget.t1 $widget.t25  -side top -expand f -fill x -pady 0
    pack $widget.t3 -side bottom -expand true -fill both -pady 0


    # ---------------------------------------
    # Reference/Output Images
    # --------------------------------------
    set c1 [$widget.t1 childsite  ]
    set output_listbox [ listbox $c1.list -width 30 -height 8 -borderwidth 2 -relief raised -selectmode single  -takefocus 0 ]
    $output_listbox configure -exportselection 0
    eval "bind $output_listbox <ButtonRelease-1> { $this OutputListCallback $output_listbox }"

    for { set i 0 } { $i < [ llength $output_list  ] } { incr i } {
	$output_listbox insert end [ lindex $output_list $i ] 
    }
    $output_listbox see end
    $output_control Initialize $c1.b ;
    if { $parent != 0 } {
	if { $second_viewer == 0 } {
	    $output_control AddFunction "$parent SetImageFromObject" "Display" "$this"
	} else {
	    $output_control AddFunction "$parent SetImageFromObject" "Display Ref" "$this"
	    $output_control AddFunction "$second_viewer SetImageFromObject" "Display Trn" "$this"
	}
    }

    pack $output_listbox $c1.b -padx 5 -pady 3 -side left -expand f -fill y -fill x


    # --------------------------------------
    # Compute Output Stuff
    # --------------------------------------

    set base [ $widget.t2 childsite ]
    iwidgets::Labeledframe $base.task -labelpos n -labeltext "Current Active Task"
    iwidgets::Labeledframe $base.right -labelpos n -labeltext "Composite Operations"
    pack $base.task $base.right  -side left -fill both -expand true -padx 5 -pady 1

    # Do Task First ------------------------------------------------------------------

    set win [ $base.task childsite ]
    set task_listbox [ iwidgets::scrolledlistbox $win.slb -labeltext "Available Tasks" \
			   -vscrollmode dynamic -hscrollmode none \
			   -selectmode single \
			   -labelpos nw -height 125]
    $task_listbox configure -exportselection 0
    
    eval "button $win.but -text \"Change Current Task\" -command { $this ChangeCurrentTask $task_listbox} "
    frame $win.bot;      pack $win.bot -side bottom  -pady 1 
    pack $win.slb $win.but -side top -expand t -fill x  -padx 5 
    
    iwidgets::entryfield $win.bot.0 -labeltext "Current:"  -width 18  -textvariable [ itcl::scope thisparam($this,current_taskname) ] -relief sunken 
    [ $win.bot.0 component entry ] configure -state disabled
    iwidgets::entryfield $win.bot.1 -labeltext "Suffix:"  -width 8  -textvariable [ itcl::scope thisparam($this,current_task) ] -relief sunken 
    [ $win.bot.1 component entry ] configure -state disabled
    pack $win.bot.1 $win.bot.0 -side right -expand t -fill x -padx 2 -pady 1

    # Do Results Next  ------------------------------------------------------------------

    set win [ $base.right childsite ]

    frame $win.m1 ; frame $win.m2 ; frame $win.m3 -height 2 -bg black

    pack $win.m1 $win.m2 $win.m3  -side top -expand t -fill x -pady 2
    label $win.m1.lab -text "Interpolation:"
    tk_optionMenu $win.m1.opt [ itcl::scope thisparam($this,interpolation)] "NearestNeighbor" "Linear" "Cubic"
    pack $win.m1.lab $win.m1.opt  -side left -expand t -fill x -padx 1
    
    label $win.m2.lab -text "Output Sampling:"
    tk_optionMenu $win.m2.opt [ itcl::scope thisparam($this,outputresolution)] "Ref" "1.0x1.0x1.0" "1.5x1.5x1.5" "2.0x2.0x2.0" "3.0x3.0x3.0" "4.5x4.5x4.5"
    pack $win.m2.lab $win.m2.opt  -side left -expand f -fill x -padx 1

    checkbutton $win.0 -text "Autosave Results" -variable [ itcl::scope thisparam($this,autosave) ]

    eval "button $win.b -text \"Compute Average Statistics\" -command { $this ComputeAverages Functional }"
#    eval "button $win.f -text \"Warp Task to Common Space\" -command { $this WarpTasksToCommonSpace save \"\" 1 }"
#    eval "button $win.f1 -text \"Do VOI Analysis\" -command { $this DoVOIAnalysis  \"\" 0 }"
#    pack $win.0  $win.b  $win.f $win.f1 $win.f2 -side top -expand f -fill x -padx 2 -pady 4
    pack $win.0  $win.b   -side top -expand f -fill x -padx 2 -pady 4

#    pack $base.right.bottom.a -side left -expand f -fill x -padx 2 

    # --------------------------
    # Visualization Output Stuff
    # --------------------------
    set c3 [$widget.t3 childsite]
    $c3 configure -width 600 -height 400
    

    set w [ frame $c3.top ]
    set w2 [ frame $c3.right -height 400 ];

#    grid $w -row 0 -column 0 -stick ne
#    grid $w2 -row 0 -column 1 -sticky nes
    pack $w -side left -expand false -fill y -padx 10
    pack $w2 -side right -expand true -fill both

#    grid columnconfigure $c3 0 -weight  100
#    grid columnconfigure $c3 1 -weight  300


#    $overlaycreator SetMainTabHeight 250
# 
#    $overlaycreator SetMainTabScrollMode 1
    $overlaycreator CreateGUI $w2
    pack forget [ $overlaycreator GetParameterBar ]


    frame $w.m0 -width 200; frame $w.m15 -width 200; frame $w.m1 -width 200 ; frame $w.m2 -width 200
    pack $w.m0 $w.m1 $w.m15 $w.m2 -side top -expand true -fill x

    if { $second_viewer != 0 } {
	label $w.m0.ml -text "Output Viewer:"
	tk_optionMenu $w.m0.mopt [ itcl::scope thisparam($this,overlayviewer)] "Reference" "Transform"
	pack $w.m0.ml $w.m0.mopt -side top -expand t -fill x
    }

    label $w.m1.l -text "Base (Underlay):"
    tk_optionMenu $w.m1.opt [ itcl::scope thisparam($this,overlaybase)] "Reference Image" "Average Anatomical" "Individual Anatomical" "Individual Conventional"
    pack $w.m1.l $w.m1.opt -side top -expand t -fill x
    
    label $w.m15.l -text "Function (Overlay):"
    tk_optionMenu $w.m15.opt [ itcl::scope thisparam($this,overlayfunction) ] "Average Map" "T-Map"  "Single Map"
    pack $w.m15.l $w.m15.opt  -side top -expand t -fill x

    set thisparam($this,overlaysinglesubject) [ iwidgets::entryfield $w.m2.subj -labeltext "Current Overlay:"  -width 25  -textvariable [ itcl::scope thisparam($this,current_subject) ] -relief sunken ]


    pack $w.m2.subj -side left -expand t -fill x
    pack forget $w.m2.subj

    for { set k 0 } { $k <=4 } { incr k } {
	if { $k<=3} {
	    eval "$w.m1.opt.menu entryconfigure $k -command { $this SetOverlayCreatorInputs }"
	}
	eval "$w.m15.opt.menu entryconfigure $k -command { $this SetOverlayCreatorInputs }"
    }



}
# ---------------------------------------------------------------------------
# Main GUI Body 
# ---------------------------------------------------------------------------

itcl::body pxitclmultisubjectaverage::Initialize { widget } { 
	
    if { $fullyinitialized == 1 } {
	return
    }

    set basewidget [ toplevel $widget ]
    wm geometry $basewidget 800x800
    wm withdraw $basewidget

    SetTitle "Multi Subject Tool"

    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget
}

itcl::body pxitclmultisubjectaverage::DelayedInitialize { } { 

    if { $fullyinitialized == 1 } {
	return
    }

    global pxtcl_pref_array

    set menubase     [ menu $basewidget.menu ]; $basewidget configure -menu $menubase
    set pbar         [ frame $basewidget.pbar    -width 400 -height 20 ]

    set batch_selector [ pxitcllistselector \#auto $basewidget.[ pxvtable::vnewobj ] ]


    set notebook $basewidget.notebook
    iwidgets::tabnotebook $notebook  -tabpos w

    #    pack $menubase -side top -expand f -fill x -pady 1
    pack $pbar     -side bottom -expand f -fill x -pady 1
    pack $notebook -side top -expand t -fill both -pady 1

    CreateBaseControl    [ $notebook add -label "Base"     ]
    CreateSubjectControl [ $notebook add -label "Subjects"  ] 
    CreateResultsControl [ $notebook add -label "Results"   ]
    CreateProgressBar  $pbar

    $notebook view "Subjects"
    $anat_control configure -callback "$this ResetSubjectTree -1"        
    $conv_control configure -callback "$this ResetSubjectTree -1"        
    $func_control configure -callback "$this ResetSubjectTree -1"        
    $epi_control configure -callback "$this ResetSubjectTree -1"        
    $field_control configure -callback "$this ResetSubjectTree -1"        
    $refx_control configure -callback "$this ResetSubjectTree -1"        
    $intx_control configure -callback "$this ResetSubjectTree -1"        
    $distx_control configure -callback "$this ResetSubjectTree -1"        
    $task_control configure -callback "$this ResetTaskTree -1"        

    menu $menubase.filem -tearoff 0 ;      $menubase add cascade -label Setup      -menu $menubase.filem
    menu $menubase.resultsm -tearoff 0;    $menubase add cascade -label Images     -menu $menubase.resultsm
    menu $menubase.batchm -tearoff 0;      $menubase add cascade -label Batch     -menu $menubase.batchm 
    menu $menubase.comparem -tearoff 0;    $menubase add cascade -label Comparisons    -menu $menubase.comparem 

    if { $pxtcl_pref_array(EnableBetaFeatures) == "Enabled" } { 

    }
      


    
    eval "$menubase.filem add command -label \"New Setup\" -command { $this NewSetup }  "
    $menubase.filem add separator
    eval "$menubase.filem add command -label \"Load Setup\" -command { $this LoadSetup \"\" }  "
    eval "$menubase.filem add command -label \"Save Setup\" -command { $this SaveSetup \"\" } "
    $menubase.filem add separator

    global env
    set thisparam($this,custsetups)  [ pxitclfilelistmenu \#auto $menubase.filem "Setup Files" "$this LoadSetup" "$this GetSetupName" ]
    $thisparam($this,custsetups) SetModeToCustom "Setup Files" .msb [ file join $env(HOME) .msbfiles ] 1 
    $thisparam($this,custsetups) InitializeDisplay
    $menubase.filem add separator


    eval "$menubase.filem add command -label \"Reorganize Data\" -command { $this ReorganizeData } "

    $menubase.filem add separator
    if { $parent == 0 } {
	eval "$menubase.filem add command -label Exit -command {  pxtkexit } -underline 1"
    } else {
	eval "$menubase.filem add command -label Close -command {  $this HideWindow } -underline 1"
    }
    
    eval "$menubase.resultsm add command -label \"Load All Images\" -command { $this LoadAll 0 }  "
    eval "$menubase.resultsm add command -label \"Load Functional Only\" -command { $this LoadAll 2 } "

    eval "$menubase.batchm add command -label \"Compute VOI Analysis  for Multiple Tasks\" -command { $this DoVOIAnalysisAllTasks 0 } "
    eval "$menubase.batchm add command -label \"Compute Average Statistics for Multiple Tasks\" -command { $this ComputeAllResults 0 }  "

    eval "$menubase.batchm add command -label \"Warp To Common Space for Multiple Tasks\" -command { $this WarpAllTasks 0 } "
    $menubase.batchm add separator
    eval "$menubase.batchm add command -label \"Create Combined Output Image (4D, one frame per subject)\" -command { $this Create4DImagesAllTasksOld values } "
    eval "$menubase.batchm add command -label \"Create Combined Output VOI-Averaged Image (4D, one frame per subject)\" -command { $this Create4DImagesAllTasksOld voi } "
    $menubase.batchm add separator
    eval "$menubase.batchm add command -label \"Compute Average Anatomical Images\" -command { $this ComputeAverages Images } "


    eval "$menubase.comparem add command -label \"Show Compare Tool\" -command { $this ShowCompareTool }"
    $menubase.comparem add separator
    eval "$menubase.comparem add command -label \"Send Mean/Std Functional to Compare Tool as Set 1\" -command { $this SetInfoToCompareTool  1 }"
    eval "$menubase.comparem add command -label \"Send Mean/Std Functional to Compare Tool as Set 2\" -command { $this SetInfoToCompareTool  2 }"
    $menubase.comparem add separator
    eval "$menubase.comparem add command -label \"Send Current Subject Functional to Compare Tool as Set 2\" -command { $this SetInfoToCompareTool  -1 }"

    
    AddSubject;    AddSubject;    AddSubject;    SetNewSubject 0
    AddTask;    AddTask;    SetNewTask 0
    OutputListCallback $output_listbox 0
    
    set fullyinitialized 1

}

::itcl::body pxitclmultisubjectaverage::AddToMenuButton { mb args} {

    if { [ llength $args ] > 0 } {
	set nxt ";[ lindex $args 0 ]"
    } else {
	set nxt ""
    }
    
    eval "$mb add command -label \"Base\" -command {$this ShowWindow Base $nxt}"
    eval "$mb add command -label \"Subjects\" -command {$this ShowWindow \"Subjects\" $nxt}"
    eval "$mb add command -label \"Results\" -command {$this ShowWindow Results $nxt}"

}

# ---------------------------------------------------------------------------
itcl::body pxitclmultisubjectaverage::ReorganizeData {  } {

    set dirname  [tk_chooseDirectory -title "Select Directory to reorganize/relocate data" ]
    if { [ string length $dirname ] < 1 } {
	return 0
    }
    
    if { [ file exists $dirname ] } {
	set newtail [ file root [ file tail $setupfilename ] ]
	set newtail "${newtail}data"
	set dirname [ file join $dirname $newtail ] 
    }
    set ok [ ::pxtclutil::Question "This operation will COPY your current files from their existing locations and reorganize them under $dirname. This can use a lot of disk space. Are you sure?" ]
    if { $ok == 0 } {
	return
    }
    file mkdir $dirname
    $this LoadAll 0
    
    set numtasks [llength $tasks]
    pxtkconsole

    set beginsub 0
    set endsub   [ expr [ llength $subjects ]  -1 ]

    set lasttask [ expr $numtasks -1 ]

    for { set c 0 } { $c < $numtasks } { incr c } {
	
	pxtkprint "\n ++++++++++++++++++++++++++++++++++++++\n Migratiting Task [ expr $c +1 ]\n------------------------------------------------\n"
	$task_listbox selection clear 0 [ expr $numtasks -1 ]
	$task_listbox selection set $c $c
	ChangeCurrentTask $task_listbox "silent"

	for { set i $beginsub } { $i <= $endsub } { incr i } {
	    set subject [ lindex $subjects $i ]
	    pxtkprint "\nSubject $i\n"
	    set newtail  [ format "%04d_%s" [expr $i+1 ] [ $subject GetBaseName ] ]
	    set newdir [ file join $dirname $newtail ]
#	    if { $numtasks != 1 } {
	    if { $c == 0  } {
		file mkdir $newdir
	    }
	    if { $c < $lasttask } {
		$subject SaveToDirectory $newdir 0 0
	    } else {
		$subject SaveToDirectory $newdir 1 1
	    }
#	    } else {
#		file mkdir $newdir
#		$subject SaveToDirectory $newdir 1 1
	    #	    }
	}


	pxtkprint "\n************ Common Files For Task\n"
	set outlist [ list [ lindex  $outputimages 2 ] [ lindex  $outputimages 3 ] [ lindex  $outputimages 4 ] ]

	if { $c==0 } {
	    set basedir [ file join $dirname base ]
	    set resultsdir [ file join $dirname results ]
	    file mkdir $basedir
	    file mkdir $resultsdir
	}

	if { $c == $lasttask } {
	    lappend outlist [ $reference_control  GetObject ]
	    lappend outlist [ $voi_control  GetObject ] 
	    lappend outlist [ lindex  $outputimages 0 ]
	    lappend outlist [ lindex  $outputimages 1 ] 
	    
	}
	
	#puts stderr "$outlist"

	for { set i 0 } { $i < [ llength $outlist ] } { incr i } {
	    
	    set obj [ lindex $outlist $i ]
	    set fn [ $obj cget -filename ]
	    if { [ string length $fn ] > 1 } {
		
		if { $i ==3 || $i == 4 } {
		    set newname [ file join $basedir [ file tail $fn ]]
		} else {
		    set newname [ file join $resultsdir [ file tail $fn ]]
		}
		$obj configure -filename $newname
		$obj Save
		pxtkprint "Mapping $fn --> $newname\n"
		if { $c != $lasttask && $i <= 2 } {
		    # Restore filename for next task
		    $obj configure -filename $fn
		}
	    }
	}
    }
    
    $task_listbox selection clear 0 [ expr $numtasks -1 ]
    $task_listbox selection set 0 0
    ChangeCurrentTask $task_listbox "silent"

    
    set newfilename [ file join $dirname [ file tail $setupfilename ] ]
    SaveSetup $newfilename

    ::pxtclutil::Info "The new setupfile can be found in $dirname ($newfilename)"
    
    SetNewSubject 0
    SetNewTask 0
    $reference_control Update
    $voi_control Update
    OutputListCallback $output_listbox 0

    return
}

# ---------------------------------------------------------------------------

if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {

    wm withdraw .
    set breg [ pxitclbaseimageviewer \#auto 0 ]
    $breg configure -appname "BioImage Suite::MultiSubject Tool"
    $breg configure -show_standard_images 1
    $breg InitializeDisplay .[pxvtable::vnewobj ] 1
    
    set mult [  pxitclmultisubjectaverage \#auto $breg ]
    $mult Initialize [ $breg GetBaseWidget].[pxvtable::vnewobj ]
    
    set menubase [ $breg cget -menubase ]
    set mb [ menu $menubase.multim -tearoff 0 ]
    $menubase add cascade -label "Multi-Subject"  -menu $mb

    $mult AddToMenuButton $mb "Multi-Subject Control" 
    $breg AddControl $mult
    
    set argc [llength $argv]
    
    if { $argc > 0 } {   $breg LoadImage [lindex $argv 0]   }
    if { $argc > 1 } {   $mult LoadSetup [lindex $argv 1]   }
    
    $breg ShowWindow
    $mult ShowWindow
}



