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

# ---------------------------------------------------------------------------------------
#                  Allows Running this from outside the tree
# ---------------------------------------------------------------------------------------

set biopath  [ file normalize [ file join [ file dirname [ info script ]] .. ]  ]
lappend auto_path [ file join $biopath base ]
lappend auto_path [ file join $biopath main ]
lappend auto_path [ file join $biopath bis_algorithm ]
unset biopath



#---------------------------------------------------------------------------------------
package require bis_fmrisetup 1.0
package require bis_colorblendimage 1.0 
package require Iwidgets 
package require pxitclsubjectentry 1.0
package require vtk
package require vtkinteraction
package require vtkpxcontrib 1.0
package require pxvtable 1.0
package require pxtclhelp  1.0
package require pxtcluserprefs 
package require pxitclfilelistmenu 1.0
package require  pxtclruncommand 1.0
package require pxtcluserprefs 1.0
package require bis_viewer 1.0 



package provide bis_fmri 1.0


#Class Definition
#----------------------------------------------------------------------------------------------------
itcl::class bis_fmri {

	inherit bis_fmrisetup

	private common hold
	protected common canvasdata


	protected variable aboutdialog 0
	public    variable appname "BioImage Suite::fMRI Single Subject Analyzer"
	public    variable version $::pxtclvtkpxcontrib::version
	public    variable aboutstring "(c) X. Papademetris, M. Jackowski, I. Murphy, H.Okuda, R.T. Constable, L.H. Staib 1995-2011\nSection of Bioimaging Sciences, Department of Diagnostic Radiology, Yale University School of Medicine\n\n\nThis application is intended for research applications only! Use at your OWN RISK. If used for publication please acknowledge, see the webpage for more details. For additional support please visit the BioImage Suite forum at:\n\thttp://research.yale.edu/bioimagesuite/forum/\n " 

	protected variable runwidget 0
	protected variable taskwidget 0
	protected variable outcomewidget 0

	protected variable session 0
	protected variable tasklistbox 0
	protected variable runslistbox 0
	protected variable plotwidget  0
	protected variable plotmatrix  0
	protected variable drawcanvas  0
	protected variable hrfcanvas  0

	protected variable replacewidget 0

	protected variable orthoviewer 0
	protected variable bis_viewer 0
	protected variable ownsorthoviewer 0
	protected variable skipframesentry 0
	protected variable subjectframe 0
	protected variable blockwidget 0
	protected variable directory_changed 0
		
	protected variable basewidget 0
	protected variable notebook   0
	protected variable runcommandgui 0
	protected variable standalone 1

	# ------------------------------- Methods ---------------------------

	constructor { parent { standal 1 } { viewer 0 } } {
	::bis_fmrisetup::constructor 
	} { set standalone [ expr $standal > 0 ]
	if { $viewer !=0 } {
		set orthoviewer $viewer
	}

	$this CreateGUI $parent }
	destructor { } 
	
	#---------------------------------------------
	#GUI Stuff
	#--------------------------------------------
	public method CreateGUI { base } 
	public method CreateDescriptionTab {base}
	public method CreateImageDataTab {base}
	public method CreateSessionTab {base}
	public method CreateXMLOutputTab {base}
	public method CreateAFNITab {base}
	public method CreateSingleSubjectTab {base}
	public method CreateRateGLMTab {base}

	public method CreatePreprocessingFrame { base { mode seed } }
	public method SetGlobalDrift { mode }
	public method CreateSeedBasedTab {base}
	public method SetOutputType { mode }
	public method CreateIntrinsicTab {base}
	public method CreateTreeTab {base}
	public method AddToMenuButtonLite { mb args} 

	public method SetMultiSubjectControl { msbcontrol } { set multisubjectcontrol $msbcontrol }


	#Browse for filenames 
	public method GetName {mode}
	# Use this for external glm matrix filename
	public method GetMatrixName {}
	public method GetSetupName { } { return $setupname }

	#Change Directory
	public method SwitchDirectory { dirname }

	# Run Related Stuff
	public method EditRunGUI { { index -1 } }
	public method RemoveRun { rmtype}
	public method AddOrEditRun { { index -1 } }
	public method GetRunFilename {currentrun mode}
	public method GetRunMotionFilename {currentrun mode}
	public method OperationOnCurrentRun { m }
	public method AddAllRuns { }


	# Task Related Stuff
	public method AddOrEditTask { }
	public method EditTaskGUI { { index -1 } }
	public method RemoveTask { rmtype}
	public method OperationOnCurrentTask {p }

	public method AddAllTasks { }

	# Override from parent
	public method ClearAllData {ask }

	# Block Editing
	public method CreateBlockEditingDialog { }
	public method CloseBlockWidget { }
	public method CreatePlotWidget { }
	public method PlotRun { run }

	# Outcome Stuff
	public method CreateOutcomeWidget { }

	#  Plot HRF Stuff
	public method PlotHRF {  }
	protected method AdjustHRFGUI { }
	protected method RestrictHRFMode { }
	public method HRFCanvasUpdate { x y b } 
	public method CreateGLMTclImage { }

	# Plot Widget Stuff 
	public method StartInteraction { x y b} 
	public method EndInteraction { } 
	public method UpdateInteraction { x y butno} 


	# Utilities
	public method ToggleFrameEntry { entrywidget framebutton } 
	public method StringParse {mode output_type end thistask thisrun}
	public method ReplaceStringGUI { }
	public method ReplaceString { }

	# File Name Utility
	public method DisplayFilenames { }




	# Commands for about and exit
	public method AboutCommand { }
	public method ExitCommand { }
	public method ShowCommandLineOptions { } 
	# Command to load and display image
	public method DisplayImage { thisimage }
	public method CheckTransformation { thistransformation } 
	public method OverlayVOIImage { { doblend 1 } }

	# Update GUI after load/save setup file
	protected method UpdateGUI { { loadmode 1 } } 
	protected method HasGUI { { loadmode 1 }  } { return 1 }

	# Reorganize Data
	public method ReorganizeDataGUI { }

	# Display MSB
	public method DisplayMSBFile { msbsetupname } 

};




::itcl::body bis_fmri::AddToMenuButtonLite { mb args} {

	eval "$mb add command -label \"FMRI Analyzer Tool\" -command { wm deiconify $basewidget }"
	
}

#Creates the Main GUI
#Called in constructor
#Creates main (toplevel) widget and sets absolute dimensions (geometry). Creates tab notebook packed
#in toplevel widget and sets absolute dimensions. Calls individual methods to fill each tab.
#Creates main "File" menu with load, save, and exit commands, calling methods for each
#----------------------------------------------------------------------------------------------------

itcl::body bis_fmri::CreateGUI { base } {

	global tcl_platform

	set parameters($this,change_to_directory) [ pwd ]
	set basewidget $base

	#    puts stderr "Base is .... "

	wm title $base $appname; update

	set tabs [frame $base.main -width 600 -bg white]
	pack  $tabs  -side top -expand true -fill both
	

	if { $standalone == 1 } {
	eval "wm protocol $base WM_DELETE_WINDOW { $this ExitCommand }"
	eval "bind all <Control-d>  { $this ExitCommand }"
	}
	
	#Makes the bottom execute frame
	set bottomframe [ frame $base.bot ]
	pack $bottomframe -side bottom -expand false -fill x
	$this CreateProgressBar $bottomframe    


	if { $orthoviewer == 0 } {
	wm geometry $base 1200x620
	set nt [ panedwindow $tabs.1 -orient horizontal -showhandle 1 \
			 -borderwidth 2 -relief raised -sashwidth 4 -sashpad 4 \
			 -handlesize 5 -handlepad 50  ]
	pack $tabs.1 -side top -expand true -fill both
	
	set guibase   [ frame $nt.c  -width 700 ]
	set viewbase  [ frame $nt.d  -width 400 ]
	
	$nt add $guibase  -minsize  500
	$nt add $viewbase -minsize  400
	update idletasks
	   
	set notebook [iwidgets::tabnotebook $guibase.1 -state normal -width 5i -height 5i -tabpos w]
	pack $notebook -padx 5 -pady 5 -expand true -fill both

	set bis_viewer [ [ bis_viewer \#auto ] GetThisPointer ]
	$bis_viewer CreateGUI $viewbase
	update idletasks
	set orthoviewer [ $bis_viewer GetViewer ]
	set ownsorthoviewer 1
	} else {
	wm geometry $base 600x620
	

	set notebook [iwidgets::tabnotebook $tabs.1 -state normal -width 5i -height 5i -tabpos w]
	pack $notebook -padx 5 -pady 5 -expand true -fill both
	}

	#Description Tab
	$this CreateDescriptionTab [$notebook add -label "Study Definition"]

	#Image Data Tab
	$this CreateImageDataTab [$notebook add -label "Anatomical Data"]

	#Session Tab
	$this CreateSessionTab [$notebook add -label "Session Information"]

	# Single Subject
	$this CreateSingleSubjectTab [$notebook add -label "Single Subject GLM"]
	$this CreateAFNITab [$notebook add -label "AFNI GLM"]



	global pxtcl_pref_array
	#AFNI Tab


	$this CreateSeedBasedTab [$notebook add -label "Seed-based Correlation"]
	$this CreateIntrinsicTab [$notebook add -label "Intrinsic Connectivity"]
# Excluded TCL Code 
	update idletasks
	$this CreateTreeTab [$notebook add -label "DataTree"]

	#Viewer Tab

   

	$notebook view "Study Definition"

	#Creates the menu
	set menubase [menu $base.top]; $base configure -menu $menubase
	set file_menu [menu $menubase.file -tearoff 0]
	$menubase add cascade -underline 0 -label "File" -menu $file_menu
	eval "$file_menu add command -label \"New\" -command {$this ClearAllData 1}"
	eval "$file_menu add separator"
	eval "$file_menu add command -label \"Load Setup File\" -command {$this LoadSetupFile }"
	eval "$file_menu add command -label \"Save Setup File\" -command {$this SaveSetupFile  }"
	eval "$file_menu add separator"
	eval "$file_menu add command -label \"Reorganize Data\" -command {$this ReorganizeDataGUI  }"
	eval "$file_menu add separator"
	eval "$file_menu add command -label \"Import Parameter fMRI Overwrite File\" -command { $this LoadParameterFile  }"
	eval "$file_menu add command -label \"Import Legacy fMRI Setup File\" -command { $this ImportfMRISetup  }"
	eval "$file_menu add separator"

	eval "$file_menu add command -label \"Change Directory...\" -command {$this GetName work_dir} "
	set thisparam($this,custdirectories)  [ pxitclfilelistmenu \#auto $file_menu "Custom Directories" "$this SwitchDirectory" ]
	$thisparam($this,custdirectories) SetModeToDirectories
	$thisparam($this,custdirectories) InitializeDisplay
	global env

	set tthis [ itcl::scope $this ]
	set thisparam($this,custsetups)  [ pxitclfilelistmenu \#auto $file_menu "Setup Files" "$tthis LoadSetupFile" "$tthis GetSetupName" ]
	$thisparam($this,custsetups) SetModeToCustom "Setup Files" .xmlg [ file join $env(HOME) .xmlgfiles ] 1 
	$thisparam($this,custsetups) InitializeDisplay
	$file_menu add separator

	if { $standalone == 1 } {
	eval "$file_menu add command -label Exit -command { $this ExitCommand }"
	} else {
	eval "$file_menu add command -label Close -command { wm withdraw $basewidget }"
	}
	set tools_menu [menu $menubase.tools -tearoff 0]
	$menubase add cascade -underline 0 -label "Tools" -menu $tools_menu

	
	
	eval "$tools_menu add command -label \"Global Search and Replace\" -command {$this ReplaceStringGUI}"

	set help_menu [menu $menubase.help -tearoff 0]
	$menubase add cascade -underline 0 -label "Help" -menu $help_menu

	eval "$help_menu add command -label \"Command Line Usage\" -command { $this ShowCommandLineOptions } "
	eval "$help_menu add command -label \"Console\" -command { pxtkconsole} "
	if { $standalone == 1 } {
	set pref_dialog [ ::pxtcluserprefs::CreatePreferenceEditor ]
	eval "$help_menu add command -label Preferences -command { wm deiconify $pref_dialog}"
	}	

	$help_menu add separator
	eval "$help_menu add command -label \"About\" -command {$this AboutCommand} "


	$this SetProcessingDefaults
}


#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::CheckTransformation { thistransformation } {

	set name1 ""
	set mode $thistransformation
	
	switch -exact $mode {
	"referenceTransform" {
		set name1 "reference_brain_path"
		set name2 "anatomical"
	}
	"internalTransform" {
		set name1 "anatomical"
		set name2 "conventional"
	}
	"distortionTransform" {
		set name1 "conventional"
		set name2 "run1"
	}
	"inverseReferenceTransform" {
		set name2 "reference_brain_path"
		set name1 "anatomical"
	}
	}
	
	if { $name1 == "" } {
	return 0
	}

	set img1 [ [ pxitclimage \#auto ] GetThisPointer ]
	puts stderr "name1=$name1, $parameters($this,$name1) "
	set ok1  [ $img1 Load $parameters($this,$name1) ]
	
	set img2 [  [ pxitclimage \#auto ] GetThisPointer ]
	if { $name2 != "run1" } {
	set ok2  [ $img2 Load $parameters($this,$name2) ]
	puts stderr "name2=$name2, $parameters($this,$name2) "
	} else {
	set fn  $parameters($this,run_file_1)
	set ok2 [ $img2 Load $fn ]
	if { $ok2 > 0 } {
		set extr [ vtkImageExtractComponents New ]
		$extr SetComponents 0
		$extr SetInput [ $img2 GetImage ]
		$extr Update
		$img2 ShallowCopyImage [ $extr GetOutput ]
		$extr Delete
	}
	}

	set tr [ [ pxitcltransform \#auto ] GetThisPointer ]
	set ok3 [ $tr Load $parameters($this,$thistransformation) ]
	puts stderr "Loading tr from $parameters($this,$thistransformation)"


	if { $ok1 ==0 || $ok2 ==0 || $ok3 ==0 } {
	tk_messageBox -type ok -title "Can't Check Transformation" -message "name1=$ok1, name2=$ok2, transform=$ok3"
	return 0
	} else {

	puts stderr "Img1 = $img1, range = [ [ [ [ $img1 GetImage ] GetPointData ] GetScalars ] GetRange ]"
	puts stderr "Img2 = $img2, range = [ [ [ [ $img2 GetImage ] GetPointData ] GetScalars ] GetRange ]"

	set check_alg [ bis_colorblendimage \#auto ] 
	$check_alg InitializeFromContainer 0 $this
	$check_alg SetInput $img1
	$check_alg SetSecondInput $img2
	$check_alg SetTransformation $tr
	$check_alg Execute

	set img [ $check_alg GetOutput ]
	$orthoviewer SetImage [ $img GetImage ] [ $img GetOrientation ]
				
	itcl::delete obj $check_alg

	}

	# --------------------------------------------------------------------------------------------

	itcl::delete obj $img1
	itcl::delete obj $img2
	itcl::delete obj $tr
}

# --------------------------------------------------------------------------------------------

itcl::body bis_fmri::OverlayVOIImage { { doblend 1 } } {

	set name1 "run1"
	set img1 [ [ pxitclimage \#auto ] GetThisPointer ]
	if { $name1 != "run1" } {
	set ok2  [ $img1 Load $parameters($this,$name2) ]
	} else {
	set fn  $parameters($this,run_file_1)
	set ok2 [ $img1 Load $fn ]
	if { $ok2 > 0 } {
		set extr [ vtkImageExtractComponents New ]
		$extr SetComponents 0
		$extr SetInput [ $img1 GetImage ]
		$extr Update
		$img1 ShallowCopyImage [ $extr GetOutput ]
		$extr Delete
	}
	}


	set img2 [ lindex [ $this GetReslicedVOIImage $img1 ] 0 ]
	if { $img2 == 0 } {
	set ok [ tk_messageBox -type ok  -title "WARNING!" -message "Bad VOI Dimensions" ]
	catch { itcl:delete obj $img1 }
	return 0
	}

#
#    set thr [ vtkImageThreshold New ]
#    $thr SetInput [ $img2 GetImage ]
#    $thr ThresholdBetween 1 100000000000000000000000000000
#    $thr ReplaceInOn
#    $thr ReplaceOutOn
#    $thr SetInValue 100
#    $thr SetOutValue 0
#    $thr Update
#    $img2 ShallowCopyImage [ $thr GetOutput ]
#    $thr Delete



	if { $doblend == 1 } {

	set extr [ vtkImageExtractComponents New ]
	$extr SetComponents 0
	$extr SetInput [ $img1 GetImage ]
	$extr Update
	$img1 ShallowCopyImage [ $extr GetOutput ]
	$extr Delete
	
	set tr [ [ pxitcltransform \#auto ] GetThisPointer ]
	
	
	set check_alg [ bis_colorblendimage \#auto ] 
	$check_alg InitializeFromContainer 0 $this
	$check_alg SetInput $img2
	$check_alg SetSecondInput $img1
	$check_alg SetTransformation $tr
	
	$check_alg Execute
	
	set img [ $check_alg GetOutput ]
	$orthoviewer SetImage [ $img GetImage ] [ $img GetOrientation ]
	itcl::delete obj $check_alg
	itcl::delete obj $tr
	} else {
	$orthoviewer SetImage [ $img2 GetImage ] [ $img2 GetOrientation ]
	}
	
	$notebook view "DataTree"
				

	itcl::delete obj $img1
	itcl::delete obj $img2

}

# --------------------------------------------------------------------------------------------

itcl::body bis_fmri::DisplayImage {thisimage } {
  

	set imagepath $::pxtclvtkpxcontrib::imagedir

	set ind [ lsearch -exact $referenceimagenames $parameters($this,$thisimage) ]
	if { $ind >=0 } {
	set thisimage  "reference_brain_path"
	set parameters($this,$thisimage) [ file join $imagepath [ lindex $referenceimagepaths $ind ] ]
	} elseif {$thisimage=="display_reference_brain"} { 
	set thisimage "reference_brain_path"
	set parameters($this,$thisimage) $parameters($this,reference_brain) 
	} elseif {$thisimage=="display_reference_gw_map"} { 
	set thisimage "reference_gw_map"
	set parameters($this,$thisimage) $parameters($this,reference_gw_map) 
	}

	if { $parameters($this,$thisimage)!="" && [file exists $parameters($this,$thisimage)]==1} {

	set img [ [ pxitclimage \#auto ]  GetThisPointer ]
	$img Load $parameters($this,$thisimage)
	set parameters($this,viewerfilename) $parameters($this,$thisimage)

	if { $ownsorthoviewer > 0 } {
		$orthoviewer SetImage [ $img GetImage ] [ $img GetOrientation ]
	} else {
		$orthoviewer SetImageFromObject $img $this
	}
	itcl::delete object $img

	} else {   
	tk_messageBox -type ok -title "Can't View!" -message "The filename is not specified or the image does not exist! " 
	}
}


#Sets up replace string window with a pair of entrywidgets, one the search string and the second with content with which to replace
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::ReplaceStringGUI { } {


	set parameters($this,replacer) ""
	set parameters($this,searchstring) ""

	if { $replacewidget != 0 } {
	wm withdraw  $replacewidget
	wm deiconify $replacewidget
	return
	}

	set replacewidget [ toplevel $basewidget.[ pxvtable::vnewobj ]]
	wm title $replacewidget "Search And Replace"
	
	set parameters($this,replacer) ""
	set parameters($this,searchstring) ""
		
	iwidgets::Labeledframe $replacewidget.frame -labelpos nw -labeltext "Search and Replace" -relief ridge
	pack $replacewidget.frame -side top -expand true -fill both -pady 2
	
	set replace_frame [$replacewidget.frame childsite]
	
	frame $replace_frame.1
	frame $replace_frame.2
	frame $replace_frame.3
	pack  $replace_frame.1 $replace_frame.2 $replace_frame.3 -side top

	eval "iwidgets::entryfield $replace_frame.1.1 -labeltext \"Search for:   \" -width 60 -textvariable [itcl::scope parameters($this,searchstring)] -relief sunken -textbackground white"
	eval "iwidgets::entryfield $replace_frame.2.1 -labeltext \"Replace with: \" -width 60 -relief sunken -textvariable [itcl::scope parameters($this,replacer)] -textbackground white"
	eval "button $replace_frame.3.1 -text \"Replace!\" -command {$this ReplaceString}"
	
	pack $replace_frame.1.1 -side left -expand true -padx 5 -pady 6
	pack $replace_frame.2.1 -side left -expand true -padx 5 -pady 6
	pack $replace_frame.3.1 -side bottom -expand false -padx 30 -pady 6


}

#Does the actual replacing specified in "ReplaceStringGUI" method
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::ReplaceString { } {

	# ------ -start ind (0)
	# oldsuffix == searchstring
	# f = old text variable
	# newsuffix = relacer
	# f2 (note the lack of a $ sign) is the neame of the variable to store the output
	#  regsub "input" $f1 output f2 ; set f1 $f2; unset f2
	


	set ok [ tk_messageBox -type yesno -default no -title "WARNING!" -message "Replace $parameters($this,searchstring) with $parameters($this,replacer)?" ]
	if { $ok=="no" } {
	return
	}

	catch { wm withdraw  $replacewidget }

	
	set parlist "subject_id session_id"
	foreach {list_mem} {subject_id study_title display_conventional conventional display_anatomical anatomical display_referenceTransform referenceTransform display_internalTransform internalTransform display_distortionTransform distortionTransform session_id session_description xml_output_file output_dir  xml_output_dir } {
	regsub -all $parameters($this,searchstring) $parameters($this,$list_mem) $parameters($this,replacer) f2
	set parameters($this,$list_mem) $f2; unset f2
	}
		
	for {set runcount 1} {$runcount <= $numruns} {incr runcount} {
	regsub -all $parameters($this,searchstring) $parameters($this,run_file_$runcount) $parameters($this,replacer) f3
	regsub -all $parameters($this,searchstring) $parameters($this,display_run_file_$runcount) $parameters($this,replacer) f4
	set parameters($this,run_file_$runcount) $f3; unset f3
	set parameters($this,display_run_file_$runcount) $f4; unset f4
	}

	

	$runslistbox clear 
	for {set runcount 1} {$runcount <= $numruns } {incr runcount} {
	$runslistbox insert end "Run_${runcount}_$parameters($this,display_run_file_${runcount})"
	}


	regsub -all $parameters($this,searchstring) $setupname $parameters($this,replacer) f3
	set setupname $f3

	wm title $basewidget "BioImage Suite::$appname [ file tail $setupname ]"
	update idletasks
}
#----------------------------------------------------------------------------------------------------  
itcl::body bis_fmri::CreateTreeTab {base} {

	set treewidget [ [ bis_treewidget \#auto ] GetThisPointer ]
	if { $bis_viewer!= 0 } {
	$treewidget SetBisViewer $bis_viewer
	} else {
	$treewidget SetViewer $orthoviewer
	}
	$treewidget CreateGUI $base 1 1
}

#Creates description 
#Called in CreateGUI
#This tab asks for the study description information, i.e. study title, subject ID, reference (colin) brain path and filename
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::CreateDescriptionTab {base} {

	set w $base
	eval "iwidgets::entryfield $w.1 -textbackground white -textvariable  [ itcl::scope parameters($this,study_title) ] -width 18 -labeltext \"Study Title:           \""
	eval "iwidgets::entryfield $w.2 -textbackground white -textvariable [ itcl::scope parameters($this,subject_id)] -width 18 -labeltext \"Subject ID:           \""
	
	frame $w.3 
	frame $w.4
	 
	pack $w.1 $w.2 $w.3 $w.4 -side top -fill x -padx 5 -pady 5
	set parameters($this,reference_brain) ""
	
	eval "iwidgets::combobox $w.3.1 -textbackground white  -labeltext \"Reference Brain: \"  -width 50  -textvariable [ itcl::scope parameters($this,display_reference_brain) ] -relief sunken"
	for { set i 0 } { $i < [ llength $referenceimagenames ] } { incr i } {
	$w.3.1 insert list end [ lindex $referenceimagenames $i ]
	}
	eval "button $w.3.3 -text \"View\" -command {$this DisplayImage display_reference_brain}"
	eval "button $w.3.2 -text \"...\" -command {  $this GetName reference_brain}"
	



	pack $w.3.1 -side left -expand true -fill x -padx 2 -pady 1
	pack $w.3.3 -side right -expand false -fill x -padx 4 -pady 1
	pack $w.3.2 -side right  -expand false -fill x -padx 4 -pady 1


	eval "iwidgets::entryfield $w.4.1 -state disabled  -labeltext \"GrayCSFWhite Map: \"  -width 40  -textvariable [ itcl::scope parameters($this,display_reference_gw_map) ] -relief sunken"

	eval "button $w.4.3 -text \"View\" -command {$this DisplayImage display_reference_gw_map}"
	eval "button $w.4.2 -text \"...\" -command {  $this GetName reference_gw_map}"
	



	pack $w.4.1 -side left -expand true -fill x -padx 2 -pady 1
	pack $w.4.3 -side right -expand false -fill x -padx 4 -pady 1
	pack $w.4.2 -side right  -expand false -fill x -padx 4 -pady 1

	::iwidgets::Labeledwidget::alignlabels $w.1 $w.2 $w.3.1 $w.4.1
   
}

#----------------------------------------------------------------------------------------------------  
itcl::body bis_fmri::DisplayFilenames { } {
	$subjectframe.1.conventional clear
	puts $parameters($this,conventional)
}

#Creates image tab
#Called in CreateGUI
#Asks the user to input image and transformation fully specified filenames
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::CreateImageDataTab {base} {
	
	set parameters($this,conventional) ""
	set parameters($this,anatomical) ""
	set parameters($this,referenceTransform) ""     
	set parameters($this,internalTransform) ""
	set parameters($this,distortionTransform) ""
	set parameters($this,voiDefinitionImage) ""
	set parameters($this,inverseReferenceTransform) ""
	set parameters($this,reference_brain_path) ""
	
	set subjectframe  $base
	frame $subjectframe.1;
	frame $subjectframe.2;
	frame $subjectframe.3;
	frame $subjectframe.4;
	frame $subjectframe.5;
	frame $subjectframe.6 -bg black -height 2;
	frame $subjectframe.7;
	frame $subjectframe.8;
	
	
	set convtail [file tail $parameters($this,conventional)]
	pack $subjectframe.1  $subjectframe.2 $subjectframe.3 $subjectframe.4 $subjectframe.5 $subjectframe.6 $subjectframe.7 $subjectframe.8 -side top -fill x -padx 2 -pady 5
	
	eval "iwidgets::entryfield $subjectframe.1.conventional -state disabled  -labeltext \"Conventional Image:  \"  -width 40  -textvariable [ itcl::scope  parameters($this,display_conventional)] -relief sunken"
	eval "button $subjectframe.1.1 -text \"...\" -command { $this GetName conventional}"
	eval "button $subjectframe.1.2 -text \"View\" -command {$this DisplayImage conventional }"
   
	eval "iwidgets::entryfield $subjectframe.2.anatomical -state disabled -labeltext   \"Anatomical Image:     \"  -width 40  -textvariable [ itcl::scope parameters($this,display_anatomical)] -relief sunken"
	eval "button $subjectframe.2.2 -text \"...\" -command {$this GetName anatomical}"
	eval "button $subjectframe.2.3 -text \"View\" -command {$this DisplayImage anatomical }"
	eval "iwidgets::entryfield $subjectframe.3.referenceTransform -state disabled -labeltext \"Reference Transform:\"  -width 40  -textvariable [ itcl::scope parameters($this,display_referenceTransform)] -relief sunken"
	eval "button $subjectframe.3.3 -text \"...\" -command {$this GetName referenceTransform}"
	eval "button $subjectframe.3.4 -text \"Check\" -command {$this CheckTransformation referenceTransform }"

	eval "iwidgets::entryfield $subjectframe.4.internalTransform -state disabled -labeltext  \"Internal Transform:     \"  -width 40 -textvariable [ itcl::scope parameters($this,display_internalTransform)] -relief sunken"
	eval "button $subjectframe.4.4 -text \"...\" -command {$this GetName internalTransform}"
	eval "button $subjectframe.4.5 -text \"Check\" -command {$this CheckTransformation internalTransform }"

	eval "iwidgets::entryfield $subjectframe.5.distortionTransform -state disabled -labeltext \"Distortion Transform:  \"  -width 40  -textvariable [ itcl::scope parameters($this,display_distortionTransform)] -relief sunken"
	eval "button $subjectframe.5.5 -text \"...\" -command {$this GetName distortionTransform}"
	eval "button $subjectframe.5.6 -text \"Check\" -command {$this CheckTransformation distortionTransform }"

	eval "iwidgets::entryfield $subjectframe.7.voiDefinitionImage -state disabled -labeltext \"VOI  Image:  \"  -width 40  -textvariable [ itcl::scope parameters($this,display_voiDefinitionImage)] -relief sunken"
	eval "button $subjectframe.7.7 -text \"...\" -command {$this GetName voiDefinitionImage}"
	eval "button $subjectframe.7.8 -text \"View\" -command {$this DisplayImage voiDefinitionImage }"

	eval "iwidgets::entryfield $subjectframe.8.inverseReferenceTransform -state disabled -labeltext \"Inverse Reference Transform:\"  -width 40  -textvariable [ itcl::scope parameters($this,display_inverseReferenceTransform)] -relief sunken"
	eval "button $subjectframe.8.8 -text \"...\" -command {$this GetName inverseReferenceTransform}"
	eval "button $subjectframe.8.9 -text \"Check\" -command {$this CheckTransformation inverseReferenceTransform }"

	

	pack $subjectframe.1.2 -side right -expand false -padx 2 -pady 1
	pack $subjectframe.1.1 -side right -expand false -padx 2 -pady 1
	pack $subjectframe.1.conventional -side left -expand true -fill x -padx 5 -pady 1 

	pack $subjectframe.2.3 -side right -expand false -padx 2 -pady 1
	pack $subjectframe.2.2 -side right -expand false -padx 2 -pady 1
	pack $subjectframe.2.anatomical -side left -expand true -fill x -padx 5 -pady 1

	pack $subjectframe.3.4 -side right -expand false -padx 5 -pady 1  
	pack $subjectframe.3.3 -side right -expand false -padx 5 -pady 1  
	pack $subjectframe.3.referenceTransform -side left -expand true -fill x -padx 5 -pady 1

	pack $subjectframe.4.5 $subjectframe.4.4 -side right -expand false -padx 5 -pady 1 
	pack $subjectframe.4.internalTransform -side left -expand true -fill x -padx 5 -pady 1

	pack $subjectframe.5.6 $subjectframe.5.5 -side right -expand false -padx 5 -pady 1
	pack $subjectframe.5.distortionTransform -side left -expand true -fill x -padx 5 -pady 1 



	pack $subjectframe.7.8 -side right -expand false -padx 2 -pady 1
	pack $subjectframe.7.7 -side right -expand false -padx 2 -pady 1
	pack $subjectframe.7.voiDefinitionImage -side left -expand true -fill x -padx 5 -pady 1 


	pack $subjectframe.8.9 $subjectframe.8.8 -side right -expand false -padx 5 -pady 1
	pack $subjectframe.8.inverseReferenceTransform -side left -expand true -fill x -padx 5 -pady 1 

	::iwidgets::Labeledwidget::alignlabels $subjectframe.1.conventional $subjectframe.2.anatomical $subjectframe.3.referenceTransform $subjectframe.4.internalTransform $subjectframe.5.distortionTransform $subjectframe.7.voiDefinitionImage $subjectframe.8.inverseReferenceTransform


	set fr [  frame $subjectframe.9 ]; pack $fr -side top -expand false -fill x

	label $fr.0 -text "VOI Space:"
	tk_optionMenu $fr.1  [ itcl::scope parameters($this,voiDefinitionSpace) ] Reference Anatomical Conventional Functional
	eval "button $fr.2 -command { $this OverlayVOIImage  1 } -text \"Check VOI Image \" "
	eval "button $fr.3 -command { $this OverlayVOIImage  0 } -text \"Check VOI Image (no overlay)\" "
	pack $fr.2 $fr.3 -side right -padx 2

	for { set i 0 } { $i <= 3 } { incr i } {
	eval "$fr.1.menu  entryconfigure $i -command \"$this UpdateDataTreeStructure\" "
	}


	pack $fr.0 $fr.1 -side left -padx 5


	
}


#Creates Session tab
#Called in CreateGUI
#Asks the user to input descriptive session information for the desired fMRI session, as well as runs fully specified pathnames and 
#task descriptions
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::CreateSessionTab {base} {

	set session $base
	frame $session.sessionid
   
	
	eval "iwidgets::entryfield $session.sessionid.1 -textbackground white -labeltext \"Session ID:               \" -width 60 -textvariable [itcl::scope parameters($this,session_id)] -relief sunken"
	eval "iwidgets::entryfield $session.sessionid.2 -textbackground white -labeltext \"Session Description: \" -width 60 -textvariable [itcl::scope parameters($this,session_description)] -relief sunken"
	eval "iwidgets::entryfield $session.sessionid.3 -textbackground white -labeltext \"Repetition Time:       \" -width 60 -textvariable [itcl::scope parameters($this,tr)] -relief sunken  -validate real"
	eval "iwidgets::entryfield $session.sessionid.4 -textbackground white -labeltext \"Number of Slices:     \" -width 60 -textvariable [itcl::scope parameters($this,numslices)] -relief sunken -validate integer"
	eval "iwidgets::entryfield $session.sessionid.5 -textbackground white  -labeltext \"Number of Frames:   \" -width 60 -textvariable [itcl::scope parameters($this,numframes)] -relief sunken -validate integer"
	checkbutton $session.sessionid.6 -highlightthickness 0  -variable [ itcl::scope parameters($this,blockunits) ] -justify left -text "Blocks Defined in Seconds" -underline 0 

	pack  $session.sessionid.1 $session.sessionid.2 $session.sessionid.3 $session.sessionid.4 $session.sessionid.5 $session.sessionid.6 -side top -expand true -fill x -padx 10 -pady 2

	frame $session.mid -bg black -height 5
	frame $session.mid2 -bg black -height 5
	frame $session.taskorruns
	frame $session.skipframes
	frame $session.taskorruns.task
	frame $session.taskorruns.mid -bg black -width 5
	frame $session.taskorruns.runs
	eval "button $session.blocks -text \"Define Blocks\" -command {$this CreateBlockEditingDialog }"
	option add *$session.blocks.borderWidth: 3
	$session.blocks configure -borderwidth 3

	pack  $session.sessionid $session.skipframes $session.mid  $session.taskorruns $session.mid2 $session.blocks -side top -expand false -fill x -padx 10 -pady 2


	# Add new buttons here
	#
	iwidgets::Labeledframe $session.glmextra -labelpos nw -labeltext "Use Predefined Matrix" -relief ridge
	pack $session.glmextra -side bottom -expand false -fill x -pady 5
	set w [ $session.glmextra childsite ]
	checkbutton $w.0 -highlightthickness 0  -variable [ itcl::scope parameters($this,glmextra_useexternalmatrix) ] -justify left -text "Use External Task Matrix" -underline 0 
	checkbutton $w.1 -highlightthickness 0  -variable [ itcl::scope parameters($this,glmextra_doconvolution) ] -justify left -text "Convolve External Matrix with HRF" -underline 0 
	frame $w.2
	pack $w.0 $w.1 $w.2 -side top -expand false -fill x -padx 3

	eval "iwidgets::entryfield $w.2.fname -state disabled  -labeltext \"Matrix Filename:  \"  -width 40  -textvariable [ itcl::scope  parameters($this,glmextra_externalmatrixfilename)] -relief sunken"
	eval "button $w.2.bt -text \"..\" -command { $this GetMatrixName}"
	pack $w.2.bt -side right -padx 2 -expand false
	pack $w.2.fname -side left -expand false -fill x




	# 

	
	set skipframesentry $session.skipframes.2
	eval "iwidgets::entryfield $skipframesentry -width 60  -state disabled  -textvariable [itcl::scope parameters($this,skipFrames)]"
	set checkb $session.skipframes.1

	set parameters($this,checks) 0

	::iwidgets::Labeledwidget::alignlabels $session.sessionid.1 $session.sessionid.2 $session.sessionid.3 $session.sessionid.4 $session.sessionid.5 $skipframesentry

	eval "checkbutton $checkb -highlightthickness 0  -variable [ itcl::scope parameters($this,checks) ] -justify left -text \"Skip Frames?\" -underline 0 -command { $this ToggleFrameEntry $skipframesentry $checkb }"
	pack $session.skipframes.1 $session.skipframes.2 -side left  -expand true -fill x -padx 2 -pady 0
	pack $session.taskorruns.task -side left -expand true 
	pack $session.taskorruns.mid  -side left -expand false -fill y
	pack $session.taskorruns.runs -side left -expand true 

	set runslistbox [eval "iwidgets::scrolledlistbox $session.taskorruns.runs.slb -labeltext \"Define Runs: \"  \
		 -vscrollmode dynamic -hscrollmode none \
		 -selectmode single \
		 -labelpos nw \
		 -textbackground white\
		 -dblclickcommand {$this OperationOnCurrentRun 3 }" ]
	
	set w [ frame $session.taskorruns.runs.slb_buttons ]
	
	pack $runslistbox $w  -side top -expand false -fill x
	
	eval "button $w.add -width 4 -text \"New\" -command {$this EditRunGUI } -padx 1"
	eval "button $w.delete -width 4 -text \"Del\" -command { $this OperationOnCurrentRun 1} -padx 1"
	eval "button $w.view -width 6 -text \"Display\" -command {$this OperationOnCurrentRun 2 } -padx 1"
	eval "button $w.edit -width 4 -text \"Edt\" -command {$this OperationOnCurrentRun 3 } -padx 1"
	eval "button $w.printall -width 6 -text \"Show All\" -command {$this OperationOnCurrentRun 4 } -padx 1"
	pack $w.add $w.delete $w.edit $w.view $w.printall -side left -expand false -fill x      
	
	  
	set tasklistbox [eval "iwidgets::scrolledlistbox $session.taskorruns.task.slb -labeltext \"Define Tasks: \"  \
		 -vscrollmode dynamic -hscrollmode none \
		 -selectmode single \
		 -labelpos nw \
		 -textbackground white\
		 -dblclickcommand {$this OperationOnCurrentTask 0 }" ]
	
	set w [ frame $session.taskorruns.task.slb_buttons ]
	pack $tasklistbox $w  -side top -expand false -fill x
	
	eval "button $w.add -width 8  -text \"New Task\" -command {$this EditTaskGUI} -padx 0"
	eval "button $w.delete -width 8 -text \"Delete\" -command { $this OperationOnCurrentTask 1} -padx 0"
	eval "button $w.edit -width 8 -text \"Edit\" -command { $this OperationOnCurrentTask 0 } -padx 0"
		
	pack $w.add $w.edit $w.delete -side left -expand true -fill x           
}

#Skip Frame toggle control
#Called in CreateSessionTab
#When user selects checkbox to skip frames in main window or runs window, changes entrywidget from disabled to active, and vice-versa
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::ToggleFrameEntry { entryw checkb } {

	set var [ $checkb cget -variable ]
	eval "set state  $$var "
   
	if {$state== 0 } {
	$entryw configure -state disabled 
	} else {
	$entryw configure -state normal -textbackground white
	}
}

#Determines the cursor location where user clicks desired task
#Called in CreateSessionTab
#When user clicks, reads the line and keeps the task number
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::OperationOnCurrentTask {p} {

	set taskcurse [$tasklistbox getcurselection]
	set ind [ string first "-" $taskcurse ]
	if { $ind > 0 } {
	set tasknum [string range $taskcurse 1 [ expr $ind -  1  ]]
	} else {
	set tasknum ""
	}
	if {$tasknum==""} {
	tk_messageBox -type ok -title "Error!" -message "No task selected"
	return
	}

	if {$p==0} {
	$this EditTaskGUI $tasknum
	} elseif {$p==1} {
	RemoveTask $tasknum
	}  
}
#Determines the cursor location where user clicked
#Called in CreateSessionTab
#When user clicks, reads the line and keeps the run number
#----------------------------------------------------------------------------------------------------
# RUN Stuff
# ---------------------------------------------------------------------------------------------------
itcl::body bis_fmri::OperationOnCurrentRun { m } {

	if { $m!=4 } {
	set curse [$runslistbox getcurselection]
	set num [string range $curse 4 5 ]
#	puts stderr "Num1=$num"
	if { [ string range $num 1 1 ] == "_" } {
		set num [ string range $num 0 0 ]
	}
#	puts stderr "Num2=$num"

	if {$num==""} {
		tk_messageBox -type ok -title "Error!" -message "Please select a run!"
		return
	} 
	
	if { $m==0} {
		EditRunGUI $num
	} elseif {$m==1} {
		RemoveRun $num
	} elseif {$m==2} {
		$this DisplayImage run_file_$num 
	} elseif {$m==3 } {
		$this EditRunGUI $num
	} 
	} else {
	pxtkconsole
	pxtkprint "\n\n Run Information\n"
	
	if { $parameters($this,checks) > 0 } {
		pxtkprint "#Frames to Skip in Allframes\n"
		pxtkprint "$parameters($this,skipFrames)\n"
	} 
	for {set runcount 1} {$runcount <= $numruns} {incr runcount} {
		pxtkprint "\n#Run $runcount/$numruns\n"
		pxtkprint "$parameters($this,run_file_$runcount)\n"
		if { $parameters($this,runskip_$runcount) == 0 } {
		pxtkprint "No Skip Frames\n"
		} else {
		pxtkprint "Skip Frames:$parameters($this,skiprunsframe_${runcount})\n"
		}
	}
	pxtkprint "\n\n"
	}


}
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::RemoveRun { rmtype} {
	
	if { $numruns < 1 } {
	return 0
	}

	set ok [ tk_messageBox -type yesno -default no -title "WARNING!" -message "Are you sure you want to delete this run?" ]
	if { $ok == "no" } {return }
	set hold($this,run) " "
	set hold($this,display_run) ""
	set parameters($this,run_file_$rmtype) ""
	set parameters($this,run_motionfile_$rmtype) ""
	set parameters($this,skiprunsframe_$rmtype) ""
	set parameters($this,runskip_$rmtype) ""
	set parameters($this,display_run_file_$rmtype) ""
	set parameters($this,display_run_motionfile_$rmtype) ""
	for {set c $rmtype} {$c<$numruns} {incr c} {
	set cplus1 [expr $c+1]
	set parameters($this,run_file_$c) $parameters($this,run_file_$cplus1)
	set parameters($this,display_run_file_$c) $parameters($this,display_run_file_$cplus1)
	set parameters($this,run_motionfile_$c) $parameters($this,run_motionfile_$cplus1)
	set parameters($this,display_run_motionfile_$c) $parameters($this,display_run_motionfile_$cplus1)
	

	}
	for {set runcount $rmtype} {$runcount<$numruns} {incr runcount} {
	for {set taskcount 1} {$taskcount<=$numtasks} {incr taskcount} {
		set taskcountplus1 [expr $taskcount+1]
		set runcountplus1 [expr $runcount+1]
		
		set parameters($this,tframe_${taskcount}_$runcount) $parameters($this,tframe_${taskcount}_$runcountplus1)
		set parameters($this,tframe_${taskcount}_$runcountplus1) ""
		
		
	}
	}
	
	
	set numruns [expr $numruns-1]
	$runslistbox clear
	$this AddAllRuns
	$this CloseBlockWidget
	$this UpdateDataTreeStructure ""
}

#Pop-up window when run is added
#Called in AddOrEditTaskorRun, OperationOnCurrentRun
#Pop up when run added, asks used for fully specified filename for the run
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::EditRunGUI { { index -1 } } {
	
#    puts stderr "Edit Run GUI = $index"

	if { $index == -1 } {
	set trueindex [expr $numruns+1]
	set title "Define New Run"
	set hold($this,run) " "
	set hold($this,runmotion) " "
	set hold($this,display_run) ""
	set hold($this,display_runmotion) ""
	set hold($this,skipframe) " "
	set hold($this,runskip) 0
	set hold($this,runindex) -1
	} else {
	set trueindex $index
	if { $index < 1 } { 
		set trueindex 1
	} elseif { $index > $numruns } {
		set trueindex $numruns
	}
	set title "Editing Run $trueindex"
	set hold($this,run)  $parameters($this,run_file_$trueindex)
	set hold($this,runmotion)  $parameters($this,run_motionfile_$trueindex)
	set hold($this,display_run) [file tail $parameters($this,run_file_$trueindex)]
	set hold($this,display_runmotion) [file tail $parameters($this,run_motionfile_$trueindex)]
	set hold($this,skipframe) $parameters($this,skiprunsframe_$trueindex)
	set hold($this,runskip)   $parameters($this,runskip_$trueindex)
	set hold($this,runindex) $trueindex
	}


	if { [ winfo exists $runwidget  ] > 0 } {
	wm withdraw $runwidget
	wm deiconify $runwidget
	wm title $runwidget $title
	if { $hold($this,runskip) == 1 } {
		$hold($this,run_frame_entry) configure -state normal
	} else {
		$hold($this,run_frame_entry) configure -state disabled
		set hold($this,skipframe) ""
	}
	return 0
	} 
	
	set runwidget [ toplevel $basewidget.[ pxvtable::vnewobj ]]
	wm title $runwidget $title
	iwidgets::Labeledframe ${runwidget}.frame -labelpos nw -labeltext "Filename:" -relief ridge
	pack ${runwidget}.frame -side top -expand true -fill both -pady 2
 
	set run_frame [${runwidget}.frame childsite]
   
	frame $run_frame.1
	frame $run_frame.15
	frame $run_frame.2
	eval "button $run_frame.3 -text \"Apply Changes\" -command {$this AddOrEditRun }"
	pack $run_frame.3 -side bottom -padx 10 -expand false -fill x

	pack $run_frame.1 $run_frame.15 $run_frame.2 -side top

	eval "iwidgets::entryfield $run_frame.1.1 -labeltext \"Run Filename\" -width 60 -textvariable [itcl::scope hold($this,display_run)] -relief sunken -state disabled"
	eval "button $run_frame.1.2 -text \"...\" -command {$this GetRunFilename -1 tohold}"

	eval "iwidgets::entryfield $run_frame.15.1 -labeltext \"Motion Filename\" -width 60 -textvariable [itcl::scope hold($this,display_runmotion)] -relief sunken -state disabled"
	eval "button $run_frame.15.2 -text \"...\" -command {$this GetRunMotionFilename -1 tohold}"

	eval "iwidgets::entryfield $run_frame.2.1  -width 70 -relief sunken  -textvariable [itcl::scope hold($this,skipframe)]"
	set hold($this,run_frame_entry) $run_frame.2.1
	checkbutton $run_frame.2.2 -highlightthickness 0 -variable [ itcl::scope hold($this,runskip) ] -justify left -text "Skip Frames?"
	eval "$run_frame.2.2 configure -command { $this ToggleFrameEntry $run_frame.2.1 $run_frame.2.2 }"


	if { $hold($this,runskip) == 1 } {
	$hold($this,run_frame_entry) configure -state normal
	} else {
	$hold($this,run_frame_entry) configure -state disabled
	}

	pack $run_frame.1.2 -side right -expand false -padx 4 -pady 6   
	pack $run_frame.1.1 -side left -expand true -padx 5 -pady 6
	pack $run_frame.15.2 -side right -expand false -padx 4 -pady 6   
	pack $run_frame.15.1 -side left -expand true -padx 5 -pady 6


	pack $run_frame.2.2 $run_frame.2.1 -side left -expand true
	set run_frame [${runwidget}.frame childsite]
}


#Adds run to parameters variables and resets
#Called in Runs Path
#Closes runwidget, set parameters, updates runcount 
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::AddOrEditRun { { index -1 } } {
	catch { wm withdraw ${runwidget} }

	if { $index == -1 } {
	set index $hold($this,runindex) 
	}
	
	if { $index == -1 } {
	set index [expr $numruns+1]
	set numruns [ expr $numruns +1 ]
	set adding 1
	} else {
	set adding 0
	}

#    puts stderr "Using index = $index"

	set parameters($this,run_file_$index)         $hold($this,run)
	set parameters($this,display_run_file_$index) [file tail $hold($this,run)]

	set parameters($this,run_motionfile_$index)         $hold($this,runmotion)
	set parameters($this,display_run_motionfile_$index) [file tail $hold($this,runmotion)]


	set parameters($this,runskip_$index)          $hold($this,runskip)
	if { $parameters($this,runskip_$index) == 0 } {
	set hold($this,skipframe) ""
	}
	set parameters($this,skiprunsframe_$index) $hold($this,skipframe)

	if { $adding == 1 } {
	$runslistbox insert end "Run_${index}_$parameters($this,display_run_file_${index})"
		for {set taskcount 1} {$taskcount<=$numtasks} {incr taskcount} {
		set parameters($this,tframe_${taskcount}_${index}) ""	
		puts "parameters($this,tframe_${taskcount}_${index})"
		
	}
	} else {
	$runslistbox delete [ expr $index -1 ] [ expr  $index -1 ]
	$runslistbox insert [ expr $index -1 ] "Run_${index}_$parameters($this,display_run_file_${index})"
	}

	$this CloseBlockWidget
	$this UpdateDataTreeStructure ""
}

#Pop-up window when task is added
#Called in AddOrEditTaskorRun
#Pop up when task added, asks user to type a task description 
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::EditTaskGUI { { index -1 } } {

	if { $index == -1 } {
	set hold($this,task) ""
	set hold($this,taskindex) -1
	set title "Adding New Task"
	} else {
	
	if { $index < 1 } { 
		set index 1
	} elseif { $index > $numtasks } {
		set index $numtasks
	}
	set hold($this,taskindex) $index
	set hold($this,task) $parameters($this,task_$index)
	set title "Editing Task $index"
	}

	set d $taskwidget
	if { [ winfo exists $d  ] } {
	wm withdraw $d
	wm deiconify $d
	wm title $d $title
	} else {
	set taskwidget $basewidget.[ pxvtable::vnewobj ]
	toplevel $taskwidget
	wm title $taskwidget $title
	
	iwidgets::Labeledframe ${taskwidget}.frame -labelpos nw -labeltext "Description for Task" -relief ridge
	pack ${taskwidget}.frame -side top -expand true -fill both -pady 2
	
	set task_frame [${taskwidget}.frame childsite]
	eval "iwidgets::entryfield $task_frame.1 -labeltext \"Task\" -width 60 -textvariable [itcl::scope hold($this,task)] -textbackground white -relief sunken"
	eval "button $task_frame.3 -text \"Apply Changes\" -command {$this AddOrEditTask }"
	
	pack $task_frame.1 -side left -expand true -padx 5 -pady 6
	pack $task_frame.3 -side right -expand true     
	}
}


	
#Sets task description filename in list
#Called in EditTaskGUI
#Default is ' ', once filename specifies replaces default in Session tab with filename    
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::AddOrEditTask { } {
	 wm withdraw ${taskwidget}

	if { $hold($this,taskindex) != -1 } {
	set index $hold($this,taskindex)
	set parameters($this,task_$index) $hold($this,task) 
	} else {
	set numtasks [expr $numtasks+1]
	set parameters($this,task_$numtasks) $hold($this,task)
	for {set runcount 1} {$runcount<=$numruns} {incr runcount} {
		set parameters($this,tframe_${numtasks}_${runcount}) ""	
	}
	}

	$tasklistbox clear
	$this AddAllTasks
	
	$this CloseBlockWidget
}
	
	

#Removes a task  run if specified by user
#Called in CreateSessionTab
#If user chooses to remove task or run, removes the last from the list and decrements the number or tasks or runs, resp.
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::RemoveTask { rmtype} {

	if { $numtasks< 1 } {
	return
	}
	
	set ok [ tk_messageBox -type yesno -default no -title "WARNING!" -message "Are you sure you want to delete this task?" ]
	if { $ok == "no" } {return }

	set hold($this,task) ""
	set parameters($this,task_$rmtype)  ""
	for {set b $rmtype} { $b<$numtasks} {incr b} {
	set bplus1 [expr $b+1]
	set parameters($this,task_$b) $parameters($this,task_$bplus1)
	
	}
	for {set runcount 1} {$runcount<=$numruns} {incr runcount} {
	for {set taskcount $rmtype} {$taskcount<$numtasks} {incr taskcount} {
		set taskcountplus1 [expr $taskcount+1]
		set runcountplus1 [expr $runcount+1]
		
		set parameters($this,tframe_${taskcount}_${runcount}) $parameters($this,tframe_${taskcountplus1}_${runcount})
		set parameters($this,tframe_${taskcountplus1}_${runcount}) ""
		
	}
	}
	set numtasks [expr $numtasks-1]
	$tasklistbox clear
	$this AddAllTasks
	$this CloseBlockWidget
	return
}

#Adds all current tasks
#Called in ChangeTask
#Adds all current tasks from beginning when a task is changed to change the name
#--------------------------------------------------------------------------------------------------
itcl::body bis_fmri::AddAllTasks { } {
	for {set c 1} {$c<=$numtasks} {incr c} { 
		$tasklistbox insert end "T$c-$parameters($this,task_$c)"
	}
}

#----------------------------------------------------------------------------------------------------  
itcl::body bis_fmri::AddAllRuns { } {
	for {set rm 1} {$rm<=$numruns} {incr rm} {
	$runslistbox insert end "Run_${rm}_$parameters($this,display_run_file_${rm})"
	}
}


	
#Sets the run filename 
#Called in EditRunGUI
#Dialog which allows user to browse for runs file
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::GetRunFilename {currentrun mode} {
	set runfname ""
	set runfpath ""
	set filetype  {{"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}}
	set runfpath [tk_getOpenFile -title "Specify Run Filename" -filetypes $filetype]
	
	if { [string length $runfpath ]<1}  {
		return 0
	}
   
	if {$mode=="tohold"} {
	set hold($this,run) $runfpath
	set hold($this,display_run) [file tail $runfpath]
	} else {
	set parameters($this,run_file_$currentrun) $runfpath
	set parameters($this,display_run_file_$currentrun) [file tail $runfpath]
	}
}

itcl::body bis_fmri::GetRunMotionFilename {currentrun mode} {
	set runfname ""
	set runfpath ""
	set filetype  {{"Matlab Files" { .mat }}}
	set runfpath [tk_getOpenFile -title "Specify Run Motion Filename" -filetypes $filetype]
	
	if { [string length $runfpath ]<1}  {
		return 0
	}
   
	if {$mode=="tohold"} {
	set hold($this,runmotion) $runfpath
	set hold($this,display_runmotion) [file tail $runfpath]
	} else {
	set parameters($this,run_motionfile_$currentrun) $runfpath
	set parameters($this,display_run_motionfile_$currentrun) [file tail $runfpath]
	}
}


#Sets files for everything other than the runs
#Called in multiple methods
#Dialog which allows user to browse for all but runs files
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::GetName { mode } {
	set fname ""
	set filetype1 [ list "Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz } ]
	set filetype2 [ list "All Files" "*"  ]
	set filetype3 [ list "Transformation Files" [list .matr .grd]]
	set updatetree 1
	
	switch -exact $mode {
	    "reference_brain" {
		set filepath [tk_getOpenFile -title "Load Filename" -initialdir $parameters($this,change_to_directory) -filetypes [ list $filetype1 $filetype2 ] ]
		set fname [file tail $filepath]
	    }
	    "reference_gw_map" {
		set filepath [tk_getOpenFile -title "Load Filename" -initialdir $parameters($this,change_to_directory) -filetypes [ list $filetype1 $filetype2 ] ]
		set fname [file tail $filepath]
	    }
	    "conventional" {	  
		set filepath [tk_getOpenFile -title "Load Filename" -initialdir $parameters($this,change_to_directory) -filetypes [ list $filetype1 $filetype2 ]]
		set fname [file tail $filepath]  
	    }
	    "anatomical" {	  
		set filepath [tk_getOpenFile -title "Load Filename" -initialdir $parameters($this,change_to_directory) -filetypes [ list $filetype1 $filetype2] ]
		set fname [file tail $filepath] 
	    }
	    "voiDefinitionImage" {	  
		set filepath [tk_getOpenFile -title "Load Filename" -initialdir $parameters($this,change_to_directory) -filetypes [ list $filetype1 $filetype2] ]
		set fname [file tail $filepath] 
	    }
	    "referenceTransform" {	    
		set filepath [tk_getOpenFile -title "Load Filename" -initialdir $parameters($this,change_to_directory) -filetypes [ list $filetype3 ] ]
		set fname [file tail $filepath] 
	    }
	    "inverseReferenceTransform" {	    
		set filepath [tk_getOpenFile -title "Load Filename" -initialdir $parameters($this,change_to_directory) -filetypes [ list $filetype3 ] ]
		set fname [file tail $filepath] 
	    }
	    "internalTransform" {	  
		set filepath  [tk_getOpenFile -title "Load Filename" -initialdir $parameters($this,change_to_directory) -filetypes [ list  $filetype3 ] ]
		set fname [file tail $filepath] 
	    }
	    "distortionTransform" {	    
		set filepath [tk_getOpenFile -title "Load Filename" -initialdir $parameters($this,change_to_directory) -filetypes [ list  $filetype3 ] ]
		set fname [file tail $filepath] 
	    }
	    "output_dir" {
		set fname " "
		set filepath [tk_chooseDirectory -title "Select Current Directory" ]    
		set updatetree 0
	    }
	    "xml_output_dir" {
		set fname " "
		set filepath [tk_chooseDirectory -title "Select Current Directory"]
		set updatetree 0
	    }
	    "work_dir" {
		set fname " "
		set filepath [tk_chooseDirectory -title "Select Working Directory"]
		set updatetree 0
	    }
	}
    
	if { [ string length $fname ] < 1 } {
	   return
	}

	if { [ string length $filepath ] < 1} {
	return
	}



	set parameters($this,$mode) $filepath
	set parameters($this,display_$mode) $fname

	if {  $mode=="work_dir"  } {
	$this SwitchDirectory $filepath
	} 

	if { $updatetree > 0 } {
	$this UpdateDataTreeStructure ""
	}

}

itcl::body bis_fmri::GetMatrixName {  } {
	set fname ""
	set filetype1 [ list "Matrix File Name" { .matr } ]
	set filetype2 [ list "All Files" "*"  ]

    set f [tk_getOpenFile -title "External GLM Matrix" -initialdir $parameters($this,change_to_directory) -filetypes [ list $filetype1 $filetype2 ] ]
    set parameters($this,glmextra_externalmatrixfilename) $f
}

itcl::body bis_fmri::SwitchDirectory { filepath } {
	cd $filepath
	set parameters($this,change_to_directory) $filepath
}

	
#Creates a pop-up window for blocks
#Called in CreateSessionTab
#Pop-up widget when user presses "Create Block" button, calculates the number of block and asks for relevant tFrame
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::CreateBlockEditingDialog {} {
   
	set numblocks [expr $numtasks*$numruns]
	if {$numblocks == 0} {
		tk_messageBox -type ok -title "Error ..." -message "Please define runs and tasks!" -icon error
	return
	}
   
	if { $blockwidget !=0 } {
	wm withdraw $blockwidget
	destroy $blockwidget
	}

	set blockwidget  $basewidget.[ pxvtable::vnewobj ]

	set dlg [ toplevel $blockwidget ]
	wm geometry $dlg 700x550
	wm title $dlg "Edit Blocks"
	set blockwidget_open 1
	
	frame $dlg.bott    
	pack   $dlg.bott -side bottom -fill x -pady 2 -padx 0
	eval "button $dlg.bott.done -text \"Close\" -command {$this CloseBlockWidget }"
	eval "button $dlg.bott.clear -text \"Clear All\" -command {$this ClearAllArrays tframe 1}"
	eval "button $dlg.bott.plot -text \"Plot\" -command {$this CreatePlotWidget }"
	eval "button $dlg.bott.val -text \"Validate Blocks\" -command { $this ValidateBlockDefinitions 0 2 }"
	pack $dlg.bott.done $dlg.bott.plot $dlg.bott.clear $dlg.bott.val -side left -expand true -fill x -padx 2

	iwidgets::scrolledframe $dlg.frame -width 7i -height 5i -labelpos nw -labeltext "Define Blocks" -relief ridge
	pack $dlg.frame -side top -fill both -pady 2 -padx 2 -expand true
	
	set blocki [$dlg.frame childsite]
	set blockid 1
	
	set index 0
	for {set runcount 1} {$runcount <= $numruns} {incr runcount} {
	for {set taskcount 1} {$taskcount <= $numtasks} {incr taskcount} {
		set t t$taskcount
		set r r$runcount
		
		set tn $parameters($this,task_$taskcount)
		set rn [ file tail [ file root $parameters($this,run_file_$runcount) ]]
		set tr "($t,$r)"
		
		eval "set parameters($this,block_$taskcount$runcount) $tr";
		if { $index > 0 } {
		set fr [ frame $blocki.0$index -bg black -height 4 ]
		pack $fr  -side top -expand false -fill x
		}
		set fr2 [ frame $blocki.1$index ]
		pack $fr2 -side top -expand false -fill x		
		
		eval "iwidgets::entryfield $fr2.ent -textbackground white -labeltext \"$parameters($this,block_$taskcount$runcount) tframe:\" -width 60 -textvariable [itcl::scope parameters($this,tframe_${taskcount}_${runcount})] -relief sunken"
		set v "button $fr2.but  -text \"?\" -command \{ tk_messageBox -type ok -message \"Task = $tn\nRun=$rn\" -title \"Block Information\" -icon info\} -width 1"
		eval $v
		pack $fr2.but -side right -padx 1
		pack $fr2.ent -side left -padx 1 -expand true -fill x
		set blockid [expr $blockid+1]
		incr index
	}
	}
}

itcl::body bis_fmri::CloseBlockWidget { } {
	
	catch { wm withdraw $blockwidget }
	catch { wm withdraw $plotwidget  }
}



#Creates output tab
#Called in CreateGUI
#Asks user for number of outcomes, to define outcomes, to output xml file, xml output directory, fMRI output directory
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::CreateXMLOutputTab {base} {
	

	iwidgets::Labeledframe $base.output -labelpos nw -labeltext "Output XML File for Rajeevan GLM" -relief ridge 
	pack $base.output -side top -expand true -fill both -pady 2
	set parameters($this,output_dir) ""
	
	set execute_frame [frame $base.bottom222 -width 200]
	pack $execute_frame -side bottom -expand false  -fill both -pady 2 -padx 2
	 

	set directory [ $base.output childsite ]
	frame $directory.filename
	frame $directory.xml_directory
	frame $directory.directory
	frame $directory.outcomes
	frame $directory.line
	
	pack $directory.outcomes -expand false -side top -padx 2 -pady 5
	pack $directory.directory -expand false -fill x  -side top -padx 2 -pady 5
	pack  $directory.line  -expand false -side top -padx 2 -pady 40
	pack $directory.filename $directory.xml_directory -fill x -expand false -side top -padx 2 -pady 5
	
	$directory.line configure -bg grey -relief sunken -height 0.012i -width 7i

	eval "iwidgets::entryfield $directory.filename.1 -textbackground white -labeltext \"XML Output File:\"  -textvariable [ itcl::scope parameters($this,xml_output_file) ] -relief sunken"
	pack $directory.filename.1 -side top -expand true -fill x -padx 5 -pady 5
	
	eval "iwidgets::entryfield $directory.xml_directory.1 -textbackground white -labeltext \"XML Output Directory:\" -textvariable [itcl::scope parameters($this,xml_output_dir)] -relief sunken"
	eval "button $directory.xml_directory.2 -text \"...\" -command {$this GetName xml_output_dir}" 
	pack $directory.xml_directory.1 -side left -expand true -fill x -padx 5 -pady 5 
	pack $directory.xml_directory.2 -side right  -expand false -padx 5 -pady 5


	eval "iwidgets::entryfield $directory.directory.1 -textbackground white -labeltext \"fMRI Output Directory:\"  -textvariable [ itcl::scope parameters($this,output_dir) ] -relief sunken"
	eval "button $directory.directory.2 -text \"...\" -command {$this GetName output_dir}"     
	pack $directory.directory.1 -side left -expand true -fill x -padx 5 -pady 5
	pack $directory.directory.2 -side right  -expand false  -padx 5 -pady 5
	
	eval "iwidgets::combobox $directory.outcomes.1 -labeltext \"     Number of Outcomes: \" -width 7 -textbackground white  -textvariable [itcl::scope parameters($this,num_outcomes)] -relief sunken"
	for {set i 0} {$i <=100} {incr i} {
		$directory.outcomes.1 insert list end $i
	}

	::iwidgets::Labeledwidget::alignlabels $directory.filename.1 $directory.xml_directory.1 $directory.directory.1
	update
	$directory.outcomes.1 invoke
	eval "button $directory.outcomes.2 -text \"Define Outcomes\" -command {$this CreateOutcomeWidget }"
	pack $directory.outcomes.1 -side left -expand true -padx 4  -pady 5
	pack $directory.outcomes.2 -side top  -expand false -padx 4 -pady 5

	eval "button $execute_frame.button -text \"Create .xml file!\" -command {$this WriteXMLFile }"
	pack $execute_frame.button -side top -expand false -fill both -padx 5 -pady 1
	option add *$execute_frame.button task.borderWidth: 3
	$execute_frame.button configure -borderwidth 3
}


#Sets the outcomes
#Called in CreateXMLOutputTab
#Creates the number of outcome queries as specified by user
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::CreateOutcomeWidget {} {
	
	if { $outcomewidget !=0 } {
	wm withdraw $outcomewidget
	destroy $outcomewidget
	}

	set num $parameters($this,num_outcomes)
	if {$num == 0} {
		tk_messageBox -type ok -title "Error ..." -message "Need at least one outcome!" -icon error
	return 0
	}
	
	set outcomewidget [ toplevel ${basewidget}.[ pxvtable::vnewobj ]]
	wm title $outcomewidget "Define Outcomes"
	if { $num > 13 } {

	iwidgets::scrolledframe $outcomewidget.frame -width 7i -height 5i -hscrollmode none -labelpos nw -labeltext "Define Outcomes" -relief ridge
	frame $outcomewidget.bott
	} else {
	iwidgets::Labeledframe $outcomewidget.frame -labelpos nw -labeltext "Define Outcomes" -relief ridge
	frame $outcomewidget.bott
	}
	
	pack $outcomewidget.frame $outcomewidget.bott -side top -fill both -pady 2 -padx 2
	
	
	set outcomew [$outcomewidget.frame childsite]
	for {set outcomecount 1}  {$outcomecount <= $num} {incr outcomecount} {
	
	eval "iwidgets::entryfield $outcomew.$outcomecount -labeltext \"Outcome_$outcomecount:\" -textbackground white -width 60 -textvariable [itcl::scope parameters($this,outcome_$outcomecount)] -relief sunken"
	pack $outcomew.$outcomecount -side top -expand true -padx 5 -pady 6
	}
	
	eval "button $outcomewidget.bott.done -text \"Done\" -command {wm withdraw $outcomewidget }"
	eval "button $outcomewidget.bott.clear -text \"Clear All\" -command {$this ClearAllArrays outcome 1}"
	pack  $outcomewidget.bott.done $outcomewidget.bott.clear -side left -expand true 

}


#Plots the outcomes
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::CreatePlotWidget {} {
	
	if { $blockwidget == 0 } {
	return
	}

	if { $plotwidget !=0 } {
	catch { 
		wm withdraw $plotwidget
		destroy $plotwidget
	}
	}


	catch { $plotmatrix Delete } 
	set plotmatrix 0
	set plotmatrix [ $this CreateBaseGLMMatrix  0 ]
	if { $plotmatrix == 0 } {
	tk_messageBox -type ok -title "Error!" -message "Bad Block Definitions. Cannot plot"
	return 0
	}


	set plotwidget [ toplevel ${blockwidget}.[ pxvtable::vnewobj ]]
	wm title $plotwidget "Plotting Time Scales"
	

	set fr [ frame $plotwidget.fr ]
	pack $fr -side bottom -expand false -fill x

	set tfr [ frame $plotwidget.fr0 ]
	pack $tfr -side top -expand false -fill x


	set parameters($this,plot_xtics)  [ expr round($parameters($this,run_file_1_length)/3 )]
	set canvasdata($this,level)  [ expr round($parameters($this,run_file_1_length)/2 )]
	set canvasdata($this,info)  ""


	iwidgets::entryfield $tfr.0 -labeltext "Set Line (frames):" -width 4 -textvariable [itcl::scope canvasdata($this,level)] -relief sunken -textbackground white -validate real
	eval "button $tfr.1 -text \"Upd Line:\" -command { $this EndInteraction }"
	iwidgets::entryfield $tfr.2 -labeltext "Info:" -width 4 -textvariable [itcl::scope canvasdata($this,plot_info)] -relief sunken -textbackground white -state disabled
	pack $tfr.0 $tfr.1   -side left -expand false -padx 2
	pack $tfr.2   -side left -expand true -fill x

	set drawcanvas [ canvas $plotwidget.canvas -width 800 -height 700  ]
	pack $drawcanvas -side top -expand true -fill both
	
	label $fr.0 -text "Run:"

	set n ""
	for { set runcount 1 } { $runcount <= $numruns } { incr runcount } {
	set n "$n $runcount"
	}

	eval "tk_optionMenu $fr.1  [ itcl::scope parameters($this,plot_run) ] $n"
	update idletasks

	for { set t 0 } { $t < $numruns } { incr t } {
	eval "$fr.1.menu entryconfigure $t -command { $this PlotRun [ expr $t +1 ] }"
	}

	set parameters($this,plot_xunits) 0
	eval "checkbutton $fr.2 -highlightthickness 0 -variable [ itcl::scope parameters($this,plot_xunits) ]  -text \"X-Axis Frames\"  -command { $this PlotRun -2 }"


	iwidgets::entryfield $fr.4 -labeltext "Grid:" -width 4 -textvariable [itcl::scope parameters($this,plot_xtics)] -relief sunken -textbackground white -validate real
	eval "button $fr.5 -text \"Update\" -command { $this PlotRun -1 }"

	eval "button $fr.6 -text \"Update Plot From Edits\" -command { $this PlotRun -3 }"


	eval "button $fr.7 -text \"Close\" -command {wm withdraw $plotwidget }"
	pack $fr.7 -side right -padx 2
	pack $fr.0 $fr.1 $fr.2 $fr.4 $fr.5 $fr.6 -side left -expand false -fill x
	wm deiconify $plotwidget

	eval "bind $drawcanvas <ButtonPress>   { $this StartInteraction   %x %y %b}"
	eval "bind $drawcanvas <B1-Motion>     { $this UpdateInteraction  %x %y 1}"
	eval "bind $drawcanvas <ButtonRelease> { $this EndInteraction }  "

	$this PlotRun -1
}



itcl::body bis_fmri::PlotRun { run } {

	if { $plotwidget == 0 || $plotmatrix == 0 } {
	return 0
	}

	set v $parameters($this,plot_xtics)
	if { $v < 10 } {
	set parameters($this,plot_xtics) 10
	}

	set orun $run
	if { $orun <= - 1 } {
	set run [ lindex [ split $parameters($this,plot_run) ":" ] 0 ]
	if { $orun == -3 } {
		catch { $plotmatrix Delete } 
		set plotmatrix 0
		#	    puts stderr "Recreatting Blocks \n\n\n\n"
		set plotmatrix [ $this CreateBaseGLMMatrix  0 ]
		if { $plotmatrix == 0 } {
		tk_messageBox -type ok -title "Error!" -message "Bad Block Definitions. Cannot plot"
		return 0
		}
	} elseif { $orun == -2 } {
		set v $parameters($this,plot_xtics)
		set u $parameters($this,plot_xunits)
		if { $u == 1 } {
		set parameters($this,plot_xtics) [ expr $v / $parameters($this,tr) ]
		} else {
		set parameters($this,plot_xtics) [ expr $v * $parameters($this,tr) ]
		}
	}
	}

	if { $run < 1 } { set run 1 }
	if { $run > $numruns } { set run $numruns }
	

	set runcount $parameters($this,plot_run)
	set rname [ file rootname [ file tail $parameters($this,run_file_$runcount) ]]
	wm title $plotwidget  "Plotting $parameters($this,plot_run), $rname Frames=$parameters($this,run_file_${run}_length), TR=$parameters($this,tr)"

	
	set cwidth  [ $drawcanvas cget -width  ]
	set cheight [ $drawcanvas cget -height ]


	catch { $drawcanvas delete all } 
	$drawcanvas create rect 0 0 $cwidth $cheight -fill darkblue 

	set baseframe 0
	for { set runcount 1 } { $runcount < $run } { incr  runcount } {
	set baseframe [ expr $baseframe +  $parameters($this,run_file_${runcount}_length) ]
	}
	set upperframe [  expr $baseframe +  $parameters($this,run_file_${run}_length) - 1]

	
	set numframes [ expr $upperframe - $baseframe + 1  ]
	set scale  [ expr int(double($cheight-30) / double($numtasks)) ]
	set wscale [ expr int(double($cwidth- 30)/ double($numframes) ) ]


#    puts stderr "BaseFrame : Upper =  $baseframe:$upperframe"

	set canvasdata($this,cwidth) $cwidth
	set canvasdata($this,cheight) $cheight
	set canvasdata($this,scale) $scale
	set canvasdata($this,wscale) $wscale
	set canvasdata($this,numframes) $numframes
	set canvasdata($this,baseframe) $baseframe
	set canvasdata($this,upperframe) $upperframe
	set canvasdata($this,dragging) 0
	set canvasdata($this,level) -1
	set canvasdata($this,minx) 20
	set canvasdata($this,maxx) [ expr round(($numframes  * $wscale) -1 + $canvasdata($this,minx))]

	
#    puts stderr "Width = $cwidth, height=$cheight, scales = $scale, $wscale, numframes=$numframes "
	set x3 [ expr $cwidth -1 ]
	

	set frlist ""
	set frloclist ""
	set frlast 0
	set maxfr [ expr $numframes -1 ]
	if { $parameters($this,plot_xunits) !=1 } {
	set maxfr [ expr $maxfr * $parameters($this,tr) ]
	} else {
	set parameters($this,plot_xtics) [ expr round($parameters($this,plot_xtics)) ]
	}
	
	for { set fr 0 } { $fr < $maxfr } { set fr [ expr $fr + $parameters($this,plot_xtics) ] } {

	if { $parameters($this,plot_xunits) == 1 } {
		lappend frloclist $fr
	} else {
		lappend frloclist [ expr $fr / $parameters($this,tr) ]
	}
	lappend frlist $fr
	}

	set yp0 [ expr 0.1 * $scale ]
	set yp1 [ expr $numtasks*$scale +0.93 ]
	set yp2 [ expr $numtasks*$scale +0.98 ]
	for { set j 1 } { $j  < [ llength $frlist ] } { incr j } {
	set x  [ lindex $frloclist $j ]
	set x1 [ expr $x * $wscale + 20 ]
	$drawcanvas create line $x1 $yp0 $x1 $yp2 -fill pink -dash $wscale
	
	set t  [ lindex $frlist $j ]
	if { $parameters($this,plot_xunits) !=1 } {
		set t [ format "%.1f sec" $t ]
	} else { 
		set t [ format "%d frames" $t ]
	}
	$drawcanvas create text [ expr round($x1+2) ] $yp2 -text $t  -anchor nw  -fill yellow
	}

	set klist [ list  $canvasdata($this,minx) $canvasdata($this,maxx) ]
	if { $parameters($this,plot_xunits) ==1 } {
	set knamelist [ list  "0 frames" "$maxfr frames" ]
	} else {
	set knamelist [ list  "0 sec" "$maxfr sec" ]
	}

	for { set j 0 } { $j <=1 } { incr j } {
	$drawcanvas create line [ lindex $klist $j ] 0 [ lindex $klist $j ] $cheight -fill red -dash [ expr round(0.8*$wscale) ]
	$drawcanvas create text [ expr round([ lindex $klist $j]+2) ] $yp2 -text [ lindex $knamelist $j ]  -anchor nw  -fill red
	}


	set numf2  0
	set canvasdata($this,level) $numf2
	$drawcanvas create line 0  0  0  $cheight -fill green  -tag grayslider -dash [ expr round(1.5 * $wscale) ]
	$drawcanvas create text 0  0.02 -text $numf2 -tags slidertext -anchor s -fill green
	$this EndInteraction
	
	for { set i 0 } { $i < $numtasks } { incr i } {
	set yp [ expr ($i+0.01)*$scale ]
	set y0 [ expr ($i+0.15)*$scale ]
	set y1 [ expr ($i+0.90)*$scale ]
	$drawcanvas create line 20 $y1 $x3 $y1 -arrow last -fill red   -dash [ expr round( 1.0*$wscale) ]

	set tsk [ expr $i +1 ]
	$drawcanvas create text 22 $yp -text "Task [expr $i+1]:$parameters($this,task_$tsk)"  -anchor nw -fill yellow
	
	for { set frame 0 } { $frame < $numframes } { incr frame } {
		if { [ $plotmatrix GetElement [ expr $baseframe + $frame ] $i ] > 0 } {
		set x  $frame
		set x1 [ expr $x * $wscale + $canvasdata($this,minx) ]
		set x2 [ expr $x1 + $wscale -1 ]
		$drawcanvas create rect $x1 $y0 $x2 $y1 -fill yellow -outline yellow  
		}
	}
	}
}

# -------------------------------------------------------------------------------------------    
itcl::body bis_fmri::CreateGLMTclImage { } {

	set lst [ $this CreateWaverMatrix ]
	if { $lst == 0 } {
	return 0 
	}
	set newmatrix  [ lindex $lst 0 ]
	set breaks     [ lindex $lst 1 ]
	set rawmatrix     [ lindex $lst 2 ]
	$rawmatrix Delete

	set glmmatr [ vtkpxMatrix [ pxvtable::vnewobj ]]
	set biswaver [ vtkbisWaver [ pxvtable::vnewobj ]]
	$biswaver AddDriftTerms $newmatrix $glmmatr $breaks $parameters($this,afni_polort) 

	$newmatrix Delete
	
	set nt [ $breaks GetNumberOfTuples ]
	for { set i 0 } { $i < $nt } { incr i } {
	# puts stderr "Break = $i [ $breaks GetComponent $i 0 ]"
	}
	set sz [ $glmmatr GetSize ]



	set numrows [ lindex $sz 0 ] 
	set wscale [ expr int($numrows/[lindex $sz 1]) ]
	if { $wscale > 10 } { set wscale 10 }
	set numcols [ expr [ lindex $sz 1 ] * $wscale ]

	set maxiy [ expr $wscale -2 ]
	set countx 0
	set targetx [ expr [ $breaks GetComponent 1 0 ] -1 ]
	#    puts stderr "Countx =$countx, targetx=$targetx"
	set data [list]

	set lrow [ list ]
	for { set i 0 } { $i < $numcols } { incr i } {
	lappend lrow [format "#%02x%02x%02x" 128 255 128 ]
	}


	pxtkconsole
	pxtkprint "Scanning GLM Model to generate image = $numrows x $numcols \n"

	for {set x 0} { $x < $numrows } {incr x} {
	set row [list]
	for {set y 0} {$y < [ lindex $sz 1 ] } { incr y} {
		lappend row [format "#%02x%02x%02x" 128 255 128 ]
		for { set iy 0 } { $iy < $maxiy } { incr iy } {
		set v [ expr 256.0*[ $glmmatr GetElement $x $y ] ]
		set r 0; set g 0; set b 0
		if { $v > 0.0 } {
			set  r [ expr int($v) ]; if { $r > 255 } { set r 255 }
		} else {
			set  b [ expr -1*int($v) ]; if { $b > 255 } { set b 255 }
		}
		lappend row [format "#%02x%02x%02x" $r $g $b ]
		}
		lappend row [format "#%02x%02x%02x" 128 255 128 ]
	}
	lappend data $row

	if { $x == $targetx } {
		lappend data $lrow
		lappend data $lrow
		lappend data $lrow
		incr countx
		if { $countx < [ expr $nt -1 ] } {
		set targetx [ expr [ $breaks GetComponent  [ expr $countx +1 ]  0] -1 ]
		#		puts stderr "Added Countx =$countx, targetx=$targetx"
		}
	}
	}

	#    puts stderr "Data = [ llength $data ], numrows=$numrows, numcols=$numcols [ expr $numrows * $numcols ]"

	$glmmatr Delete
	$breaks  Delete

	set img [ image create photo ]
	$img put $data

	set bot [ toplevel $basewidget.[ pxvtable::vnewobj ]]
	wm geometry $bot [ expr $numcols + 5 ]x[ expr $numrows + 40 ];
	wm title $bot "GLM Image (Drift=$parameters($this,afni_polort), runs=$nt, tasks=$numtasks)"
	update idletasks
	set lab [ label $bot.top -image $img ]
	eval "button $bot.bot -text Close -command { wm withdraw $bot; destroy $bot }"
	pack $bot.bot -side bottom -expand false -fill x
	pack $bot.top -side top -expand true -fill both
	
	#    unset data 
}


# -------------------------------------------------------------------------------------------    
itcl::body bis_fmri::PlotHRF { } {

	if { $hrfcanvas == 0  } {
	return 0
	}

	if { $parameters($this,afni_useafniwaver) == 1 } {
	if { $parameters($this,afni_hrfmode) != "wav" && $parameters($this,afni_hrfmode) != "gamma" } {
		pxtclutil::Warning "Switching hrf_mode to wav as  AFNI's waver does not support $parameters($this,afni_hrfmode)"
		set parameters($this,afni_hrfmode) "wav" 
	}
	}

	set biswaver [ vtkbisWaver [ pxvtable::vnewobj ]]
	switch -exact $parameters($this,afni_hrfmode) {
	"wav"  {
		$biswaver SetWAV_delay_time  $parameters($this,afni_delaytime) 
		$biswaver SetWAV_rise_time $parameters($this,afni_risetime)
		$biswaver SetWAV_fall_time $parameters($this,afni_falltime) 
		$biswaver SetWAV_undershoot $parameters($this,afni_undershoot)
		$biswaver SetWaveform_type 1
	}
	"gamma"        {    $biswaver SetWaveform_type 2 }
	"doublegamma"  {    $biswaver SetWaveform_type 3 }
	"triplegamma"  {    $biswaver SetWaveform_type 4 }
	"spm"          {    $biswaver SetWaveform_type 5 }
	}

	$this AdjustHRFGUI

	set tr [ expr $parameters($this,tr)*0.25 ]
	if { $tr < 0.0 || $tr > 10.0 } {
	set tr 0.5
	}

	$biswaver SetTR   $tr
	$biswaver SetPeak $parameters($this,afni_peak) 



	set arr [ $biswaver GenerateResponseFunction ]
	
	set cwidth  [ $hrfcanvas cget -width  ]
	set cheight [ $hrfcanvas cget -height ]


	catch { $hrfcanvas delete all } 
	$hrfcanvas create rect 0 0 $cwidth $cheight -fill darkblue 
	
	set p(0) 0.0
	set p(1) 0.0
	
	set nt [ $arr GetNumberOfTuples ]

	set range [ $arr GetRange ]
	set rmax  [ lindex $range 1 ]
	set rmin  [ lindex $range 0 ]

	if { $rmin > [ expr -0.3 * $rmax ] } {
	set rmin [ expr -0.3 *  $rmax ] 
	}
	set rdiff [ expr $rmax - $rmin ]



	set wscale [ expr  ($cwidth-20.0) /double($nt) ]
	set scale  [ expr int( (40-$cheight)/$rdiff) ]
	set sint   [ expr int(40-$rmax*$scale) ]

	set canvasdata($this,hrfwscale) $wscale
	set canvasdata($this,hrfscale)  $scale
	set canvasdata($this,hrfsint)   $sint
	set canvasdata($this,hrfxoff)   10.0


	for { set i 0 } { $i < $nt  } { incr i } {
	set y [ $arr GetComponent $i 0 ]
	set x [ expr $p(0) + 1.0 ]

	set x1 [ expr $x * $wscale  + $canvasdata($this,hrfxoff) ]
	set x0 [ expr $p(0) * $wscale + $canvasdata($this,hrfxoff) ]
	set p(0) $x
	
	set y1 [ expr $y * $scale + $sint ]
	set y0 [ expr $p(1) * $scale + $sint ]
	set p(1) $y

	$hrfcanvas create line $x0 $y0 $x1 $y1 -fill yellow
	}
	
	set xmin  [ expr int($canvasdata($this,hrfxoff))]
	set xmax  [ expr int($nt * $wscale) + $canvasdata($this,hrfxoff) ]
	set ymin  [ expr int($rmin * $scale + $sint) ]
	set ymax  [ expr int($rmax * $scale + $sint) ]
	set ymax2  [ expr int(0.5*$rmax * $scale + $sint) ]
	set yzero $sint 

	$hrfcanvas create line $xmin $yzero $xmax $yzero -arrow last -fill red 
	$hrfcanvas create line $xmin $ymin $xmin $ymax -arrow both -fill red    

	set xmin [ expr $xmin + 2 ]
	$hrfcanvas create line $xmin $ymin $xmax $ymin -arrow none -fill pink -dash 10
	$hrfcanvas create line $xmin $ymax2 $xmax $ymax2 -arrow none -fill pink -dash 10
	$hrfcanvas create line $xmin $ymax $xmax $ymax -arrow none -fill pink -dash 10
	$hrfcanvas create line $xmax $ymin $xmax $ymax -arrow none -fill pink -dash 10


	$hrfcanvas create text [ expr $xmin + 10 ] [ expr $ymax - 5 ]   -text "" -tags hrftext -anchor sw -fill green



	set dv 4
	while { [ expr $nt/ $dv ] > 5 } {
	set dv [ expr $dv + 4 ]
	}

	for { set i $dv  } { $i < $nt  } { set i [ expr $i+ $dv  ] } {
	set x  [ expr  int(double($i) * $wscale) + 10 ]
	set t  [ format "%d TR" [ expr round(double($i/$dv)) ]]
	$hrfcanvas create line $x $ymin $x $ymax -arrow none -fill red   -dash 10
	$hrfcanvas create text $x $ymin -text $t   -anchor s  -fill yellow
	}
	$arr Delete
	$biswaver Delete

	$hrfcanvas create text [ expr $xmax - 30 ] [ expr $ymax + 40 ]   -text "TR = $parameters($this,tr) sec"  -anchor se -fill cyan

	eval "bind $hrfcanvas <B1-ButtonPress>   { $this HRFCanvasUpdate %x %y 1 }"
	eval "bind $hrfcanvas <B1-Motion>     { $this HRFCanvasUpdate %x %y 2 }"
	eval "bind $hrfcanvas <B1-ButtonRelease>     { $this HRFCanvasUpdate %x %y 3 }"

}

	
::itcl::body bis_fmri::HRFCanvasUpdate { x y b } { 

	if { $hrfcanvas == 0 } {
	return 0
	}

	$hrfcanvas dchars hrftext 0 end
	if { $b == 3 } {
	return
	}

	set vx [ expr (double($x) - double($canvasdata($this,hrfxoff)) ) / double($canvasdata($this,hrfwscale)) ]
	set vy [ expr (double($y) - double($canvasdata($this,hrfsint)) ) / double($canvasdata($this,hrfscale)) ]

	set tr  [ expr $vx/8.0 ] 
	set t   [ expr $tr * $parameters($this,tr) ]

	


	$hrfcanvas insert hrftext 0 [ format "t=%.2f sec (%.1f TR), y=%.2f" $t $tr $vy ]
}

# -------------------------------------------------------------------------------------------    
::itcl::body bis_fmri::AdjustHRFGUI { } {     
	
	for { set i 1 } { $i <=2 } { incr i } {
	set ok 0
	catch { set ok  $parameters($this,afni_wavergui$i)   }

	if { $ok != 0 } {
		if { $parameters($this,afni_hrfmode) == "wav" } {
		::pxitclbasecontrol::EnableUI $ok
		} else {
		::pxitclbasecontrol::DisableUI $ok
		}
	} 
	}

}
# -------------------------------------------------------------------------------------------    
::itcl::body bis_fmri::StartInteraction { x y b} {
	UpdateInteraction  $x $y $b 
}
	
# -------------------------------------------------------------------------------------------    
::itcl::body bis_fmri::EndInteraction { } {

	set x [ expr ($canvasdata($this,level))*$canvasdata($this,wscale) + $canvasdata($this,minx) ]
	if { $x < $canvasdata($this,minx) } {
	set x $canvasdata($this,minx)
	} elseif { $x > $canvasdata($this,maxx) } {
	set x $canvasdata($this,maxx)
	}

	$drawcanvas coords grayslider  $x 0  $x  [ expr 0.98 * $canvasdata($this,cheight) ]
	$drawcanvas coords slidertext [ expr $x + 2 ]  $canvasdata($this,cheight) 
	$drawcanvas dchars slidertext 0 end

	if { $canvasdata($this,level) < 0 } {
	set canvasdata($this,level) 0
	} elseif { $canvasdata($this,level) > $canvasdata($this,numframes) } {
	set canvasdata($this,level) [ expr $canvasdata($this,numframes) -1 ]
	}

	set l [ format "%.1f sec" [ expr $canvasdata($this,level) *$parameters($this,tr) ]]
	$drawcanvas insert slidertext 0 $l

	set canvasdata($this,plot_info) "$l"

	if { $plotmatrix == 0 || $plotwidget == 0 } { return }
	set fr [ expr $canvasdata($this,level) + $canvasdata($this,baseframe) ]
	
	set qt "$l (fr=$fr) Tasks Active:"
	for { set i 1 } { $i <= $numtasks } { incr i } {
	set nm $parameters($this,task_$i)
	if { [ $plotmatrix GetElement  $fr [ expr  $i -1 ] ] > 0 } {
		set qt "$qt $i ($nm)"	
	} 
	}
	set canvasdata($this,plot_info) $qt
}
	
# ------------------------------------------------------------------------------------------ -   
::itcl::body bis_fmri::UpdateInteraction { x y butno} {

	# compute the bin selected by the mouse
	if { $plotmatrix == 0 || $plotwidget == 0 } { return }
	
	if { $butno ==3 } {
	return
	}
	
	set canvasdata($this,level) [ expr ($x - $canvasdata($this,minx)) / $canvasdata($this,wscale)  ]
	$this EndInteraction
	
}
# ----------------------------------------------------------------------------------------


itcl::body bis_fmri::AboutCommand {  } {

	if { $aboutdialog == 0 } {
	set w .
	if { [ string length $basewidget ] > 2 } { set w $basewidget }
	
	set aboutdialog  [ toplevel $w.[ pxvtable::vnewobj ]] 
	
	::pxtclhelp::HelpDialog $aboutdialog
	wm title $aboutdialog "About This Application"
	
	set version $::pxtclvtkpxcontrib::version

	set line "$appname by Isabella Murphy, Thomas Teisseyre  and Xenios Papademetris is part of $version.\n\n $aboutstring \n\n------------------------------------\n System Information\n------------------------------------\n[ ::pxtclutil::GetAllInfo ]"
	
	::pxtclhelp::HelpDialogAddText $aboutdialog $line
	
	set logoframe [ ::pxtclhelp::GetLogoFrame  $aboutdialog ]
	set imgname [ file join $::pxtclvtkpxcontrib::imagedir  bioimagesuite_logo.gif ]
	
	label $logoframe.b -image $::pxtclvtkpxcontrib::logo
	pack  $logoframe.b -side left -expand t -fill x 
	
	set textbox [  ::pxtclhelp::GetTextBox  $aboutdialog ]
	
	$textbox configure -font "$::pxtclvtkpxcontrib::fontname"
	::pxitclbasecontrol::SetWidgetTreeColors $aboutdialog "\#8197c5" white
	}
	::pxtclutil::ShowWindow $aboutdialog
	
	puts stderr "$aboutdialog, [ $this GetSetupName ]"
}

#Creates output tab
#Called in CreateGUI
#Asks user for number of outcomes, to define outcomes, to output xml file, xml output directory, fMRI output directory
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::CreateAFNITab {base} {
	
	label $base.oo -bg black -fg white -text "Operations on this tab require that AFNI be installed (see http://afni.nimh.nih.gov/afni/)"
	label $base.oo2 -bg black -fg white -text "All this functionality is also available under the Single Subject GLM Tab"
	pack $base.oo $base.oo2 -side top -expand false -fill x

	iwidgets::Labeledframe $base.output -labelpos nw -labeltext "Output Specifications" -relief ridge 
	pack $base.output -side top -expand true -fill both -pady 2

	set cs [ $base.output childsite ]

	set fr  [ frame $cs.1 ]; 
	set fr2 [ frame $cs.2 ]; 
	pack $fr $fr2 -side top -expand false -fill x



	eval "iwidgets::entryfield $fr.1 -textbackground white -labeltext \"Results Directory:\"  -textvariable [ itcl::scope parameters($this,output_dir) ] -relief sunken"
	eval "button $fr.2 -text \"...\" -command {$this GetName output_dir}"     
	pack $fr.2 -side right  -expand false  -padx 5 -pady 5
	pack $fr.1 -side left -expand true -fill x -padx 5 -pady 5

	checkbutton $fr2.a -highlightthickness 0  -variable [ itcl::scope parameters($this,afni_ensurenifti) ] -text "Ensure all Input 4D Images are in NIFTI format"    
	checkbutton $fr2.b -highlightthickness 0  -variable [ itcl::scope parameters($this,afni_deletetemp) ] -text "Delete Temporary Files created for/by AFNI"
	checkbutton $fr2.c -highlightthickness 0  -variable [ itcl::scope parameters($this,afni_useafninormalize) ] -text "Use AFNI Normalize"
	pack $fr2.a $fr2.b $fr2.c -side top -expand false -padx 20

	iwidgets::Labeledframe $base.output0 -labelpos nw -labeltext "GLM Specifications" -relief ridge 
	pack $base.output0 -side top -expand true -fill both -pady 2
	set w [ $base.output0 childsite ]

	frame $w.0; pack $w.0 -side top
	label $w.0.a -text "HRF Mode :"
	tk_optionMenu $w.0.b  [ itcl::scope parameters($this,afni_hrfmode) ] "wav" "gamma" "doublegamma" "triplegamma" "spm" 
	for { set t 0 } { $t < 5 } { incr t } {
	eval "$w.0.b.menu entryconfigure $t -command { $this PlotHRF  }"
	}


	eval "checkbutton $w.0.d -highlightthickness 0  -variable [ itcl::scope parameters($this,afni_useafniwaver) ] -text \"Use AFNI Waver\" -command { $this PlotHRF }"

	pack $w.0.a $w.0.b  $w.0.d  -side left -expand false -fill x


	set parameters($this,afni_wavergui1) [ frame $w.1 ]
	iwidgets::Labeledframe $w.2 -labelpos nw -labeltext "WAV Parameters" -relief ridge
	set w3 [ frame $w.3  ]
	pack $w.1  $w.2 $w.3  -side top -expand false 

	set ws [ $w.2 childsite ]

	iwidgets::entryfield $ws.1 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_delaytime) ] -validate real -width 5 -labeltext "Delay Time:"
	iwidgets::entryfield $ws.2 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_risetime) ] -validate real -width 5 -labeltext "Rise Time:"
	iwidgets::entryfield $ws.3 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_falltime) ] -validate real -width 5 -labeltext "Fall Time:"
	iwidgets::entryfield $ws.4 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_undershoot) ] -validate real -width 5 -labeltext "Undershoot:"
	iwidgets::entryfield $ws.5 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_peak) ] -validate real -width 5 -labeltext "Peak:"
#    pack $ws.1 $ws.2 $ws.3 $ws.4 $ws.5 -side left -expand false -fill x
	grid $ws.1 -row 0 -column 0
	grid $ws.2 -row 0 -column 1
	grid $ws.3 -row 0 -column 2
	grid $ws.4 -row 1 -column 0
	grid $ws.5 -row 1 -column 1


	iwidgets::entryfield $w3.1 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_polort) ] -validate integer -width 2 -labeltext "Drift Polynomial Order:"
	iwidgets::entryfield $w3.2 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_masklevel) ] -validate real -width 5 -labeltext "Mask Threshold (0-1):"
	pack $w3.1 $w3.2 -side left -padx 10



	iwidgets::Labeledframe $base.output12 -labelpos nw -labeltext "AFNI Outputs" -relief ridge 
	pack $base.output12 -side top -expand true -fill both -pady 2

	set w [ $base.output12 childsite ]
	set wa [ frame $w.row1 ]; set wb [ frame $w.row2 ]; pack $wa $wb -side top -expand false -fill x

	checkbutton $wa.0 -variable [ itcl::scope parameters($this,afni_bout)  ] -text "Baseline Coeffs"
	checkbutton $wa.1 -variable [ itcl::scope parameters($this,afni_fout)  ] -text "F-Statistics"
	checkbutton $wa.2 -variable [ itcl::scope parameters($this,afni_rout)  ] -text "R^2 Statistics"
	pack $wa.0 $wa.1 $wa.2 -side left -expand false -fill x -padx 2


	checkbutton $wb.0 -variable [ itcl::scope parameters($this,afni_tout)  ] -text "T-Statistics"
	checkbutton $wb.1 -variable [ itcl::scope parameters($this,afni_vout)  ] -text "Variance Map"
	checkbutton $wb.2 -variable [ itcl::scope parameters($this,afni_bucket)  ] -text "Single 4D Image"
	pack $wb.0 $wb.1  -side left -expand false -fill x -padx 2


	iwidgets::Labeledframe $base.output2 -labelpos nw -labeltext "GLM Processing" -relief ridge 
	pack $base.output2 -side top -expand true -fill both -pady 2

	set cs [ $base.output2 childsite ]

	eval "button $cs.0 -text \"Reset Defaults\" -command { $this SetProcessingDefaults }"
	eval "button $cs.1 -text \"Create AFNI Script File for Single Subject GLM\" -command { $this AFNICreateScript 1 1  }"
	pack $cs.0 -side right -padx 2
	pack $cs.1 -side left -padx 0 -expand true -fill x

}


#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::CreateSingleSubjectTab {base} {
	
	iwidgets::Labeledframe $base.output0 -labelpos nw -labeltext "GLM Specifications" -relief ridge 
	set w [ $base.output0 childsite ]


	frame $w.0; pack $w.0 -side top -expand true -fill x
	set w1 [ frame $w.0.l ]
	set hrfcanvas [ canvas $w.1 -width 256 -height 300  ]
	pack $w.1 -side right
	pack $w1 -side left -expand true -fill both
	$hrfcanvas create rect 0 0 256 300 -fill darkblue 

	

	label $w1.a -text "HRF Mode :"
	tk_optionMenu $w1.b  [ itcl::scope parameters($this,afni_hrfmode) ] "wav" "gamma" "doublegamma" "triplegamma" "spm" 
	for { set t 0 } { $t < 5 } { incr t } {
	eval "$w1.b.menu entryconfigure $t -command { $this PlotHRF  }"
	}

	eval "button $w1.c -text Plot -command { $this PlotHRF }"
	pack $w1.a $w1.b $w1.c -side left -padx 5

	iwidgets::Labeledframe $w.2 -labelpos nw -labeltext "WAV Parameters" -relief ridge
	set parameters($this,afni_wavergui2) $w.2 
	pack $w.2 -side top -expand true -fill both

	set ws [ $w.2 childsite ]

	iwidgets::entryfield $ws.1 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_delaytime) ] -validate real -width 5 -labeltext "Delay Time:"
	iwidgets::entryfield $ws.2 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_risetime) ] -validate real -width 5 -labeltext "Rise Time:"
	iwidgets::entryfield $ws.3 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_falltime) ] -validate real -width 5 -labeltext "Fall Time:"
	iwidgets::entryfield $ws.4 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_undershoot) ] -validate real -width 5 -labeltext "Undershoot:"
	iwidgets::entryfield $ws.5 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_peak) ] -validate real -width 5 -labeltext "Peak:"
	pack $ws.1 $ws.2 $ws.3 $ws.4 $ws.5 -side top -expand false 
	::iwidgets::Labeledwidget::alignlabels $ws.1 $ws.2 $ws.3 $ws.4 $ws.5


	iwidgets::entryfield $w.4 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_masklevel) ] -validate real -width 5 -labeltext "Mask Threshold (0-1):"
	iwidgets::entryfield $w.6 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_polort) ] -validate integer -width 2 -labeltext "Drift Polynomial Order:"
	checkbutton $w.8 -variable [ itcl::scope parameters($this,afni_glmregressmotion)  ] -text "Regress Motion Parameters"
	pack  $w.2 $w.4 $w.6 $w.8  -side top -expand false 
	::iwidgets::Labeledwidget::alignlabels $w.4 $w.6




	iwidgets::Labeledframe $base.output -labelpos nw -labeltext "Outputs" -relief ridge 


	set cs [ $base.output childsite ]

	set fr  [ frame $cs.1 ]; 
	pack $fr -side top -expand false -fill x


	eval "iwidgets::entryfield $fr.1 -textbackground white -labeltext \"Results Directory:\"  -textvariable [ itcl::scope parameters($this,output_dir) ] -relief sunken"
	eval "button $fr.2 -text \"...\" -command {$this GetName output_dir}"     
	pack $fr.2 -side right  -expand false  -padx 5 -pady 5
	pack $fr.1 -side left -expand true -fill x -padx 5 -pady 5

	checkbutton $cs.2 -variable [ itcl::scope parameters($this,afni_tout)  ] -text "Compute T-Statistics for each beta value"
	checkbutton $cs.25 -variable [ itcl::scope parameters($this,afni_saveresidual)  ] -text "Save Residual Image"
	checkbutton $cs.26 -variable [ itcl::scope parameters($this,afni_savedriftbeta)  ] -text "Save 4D Drift Beta Maps"
	checkbutton $cs.27 -variable [ itcl::scope parameters($this,afni_glmsinglerunoutput)  ] -text "Compute Individual Run Beta Maps"
	checkbutton $cs.3 -highlightthickness 0  -variable [ itcl::scope parameters($this,afni_deletetemp) ] -text "Do Not Save Temporary Files"
	pack $cs.2 $cs.25 $cs.26 $cs.27 $cs.3 -side top -expand false -padx 20



	iwidgets::Labeledframe $base.output2 -labelpos nw -labeltext "GLM Processing" -relief ridge 
	pack $base.output2 -side bottom -expand true -fill both -pady 2

	pack $base.output0 -side top -expand true -fill both -pady 2
	pack $base.output -side top -expand true -fill both -pady 2

	set cs [ $base.output2 childsite ]

	eval "button $cs.0 -text \"Reset Defaults\" -command { $this SetProcessingDefaults }"
	eval "button $cs.2 -text \"Show Model as Image\" -command  { $this CreateGLMTclImage }"

	eval "button $cs.1 -text \"Compute GLM\" -command { $this ComputeGLM 1 1  }"
	pack $cs.0 $cs.2 -side right -padx 2
	pack $cs.1 -side left -padx 0 -expand true -fill x




	set parameters($this,tr) 1.0
	$this PlotHRF
}

#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::CreateRateGLMTab {base} {
    
    iwidgets::Labeledframe $base.output0 -labelpos nw -labeltext "Rate GLM Specifications" -relief ridge 
    set w [ $base.output0 childsite ]
    $w configure -bg red
    
    frame $w.0; pack $w.0 -side top -expand true -fill x
    set w1 [ frame $w.0.l ]
#    set hrfcanvas [ canvas $w.1 -width 256 -height 300  ]
#    pack $w.1 -side right
    pack $w1 -side left -expand true -fill both
#    $hrfcanvas create rect 0 0 256 300 -fill darkblue 
    
    
    
    label $w1.a -text "HRF Mode :"
    tk_optionMenu $w1.b  [ itcl::scope parameters($this,afni_hrfmode) ] "wav" "gamma" "doublegamma" "triplegamma" "spm" 
    for { set t 0 } { $t < 5 } { incr t } {
	eval "$w1.b.menu entryconfigure $t -command { $this PlotHRF  }"
    }
    
    #    eval "button $w1.c -text Plot -command { $this PlotHRF }"
    pack $w1.a $w1.b  -side left -padx 5
    
    iwidgets::Labeledframe $w.2 -labelpos nw -labeltext "WAV Parameters" -relief ridge
    set parameters($this,afni_wavergui2) $w.2 
    pack $w.2 -side top -expand true -fill both
    
    set ws [ $w.2 childsite ]
    
    iwidgets::entryfield $ws.1 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_delaytime) ] -validate real -width 5 -labeltext "Delay Time:"
    iwidgets::entryfield $ws.2 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_risetime) ] -validate real -width 5 -labeltext "Rise Time:"
    iwidgets::entryfield $ws.3 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_falltime) ] -validate real -width 5 -labeltext "Fall Time:"
    iwidgets::entryfield $ws.4 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_undershoot) ] -validate real -width 5 -labeltext "Undershoot:"
    iwidgets::entryfield $ws.5 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_peak) ] -validate real -width 5 -labeltext "Peak:"
    pack $ws.1 $ws.2 $ws.3 $ws.4 $ws.5 -side top -expand false 
    ::iwidgets::Labeledwidget::alignlabels $ws.1 $ws.2 $ws.3 $ws.4 $ws.5
    
    
    iwidgets::entryfield $w.4 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_masklevel) ] -validate real -width 5 -labeltext "Mask Threshold (0-1):"
    iwidgets::entryfield $w.6 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_polort) ] -validate integer -width 2 -labeltext "Drift Polynomial Order:"
    iwidgets::entryfield $w.8 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_rateglobal) ] -validate integer -width 2 -labeltext "Do Global Drift Correction:"
    iwidgets::entryfield $w.10 -textbackground white -textvariable  [ itcl::scope parameters($this,afni_numrateterms) ] -validate integer -width 2 -labeltext "Number of Rate Terms:"
    pack  $w.2 $w.4 $w.6 $w.8 $w.10  -side top -expand false 
    ::iwidgets::Labeledwidget::alignlabels  $w.4 $w.6 $w.8 $w.10
    iwidgets::Labeledframe $base.output -labelpos nw -labeltext "Outputs" -relief ridge 
    
    set cs [ $base.output childsite ]
    
    set fr  [ frame $cs.1 ]; 
    pack $fr -side top -expand false -fill x
    
    eval "iwidgets::entryfield $fr.1 -textbackground white -labeltext \"Results Directory:\"  -textvariable [ itcl::scope parameters($this,output_dir) ] -relief sunken"
    eval "button $fr.2 -text \"...\" -command {$this GetName output_dir}"     
    pack $fr.2 -side right  -expand false  -padx 5 -pady 5
    pack $fr.1 -side left -expand true -fill x -padx 5 -pady 5
    
    checkbutton $cs.2 -variable [ itcl::scope parameters($this,afni_tout)  ] -text "Compute T-Statistics for each beta value"
    checkbutton $cs.25 -variable [ itcl::scope parameters($this,afni_saveresidual)  ] -text "Save Residual Image"
    checkbutton $cs.26 -variable [ itcl::scope parameters($this,afni_savedriftbeta)  ] -text "Save 4D Drift Beta Maps"
    checkbutton $cs.3 -highlightthickness 0  -variable [ itcl::scope parameters($this,afni_deletetemp) ] -text "Do Not Save Temporary Files"
    pack $cs.2 $cs.25 $cs.26 $cs.3 -side top -expand false -padx 20
    
    
    
    iwidgets::Labeledframe $base.output2 -labelpos nw -labeltext "GLM Processing" -relief ridge 
    pack $base.output2 -side bottom -expand true -fill both -pady 2
    
    pack $base.output0 -side top -expand true -fill both -pady 2
    pack $base.output -side top -expand true -fill both -pady 2
    
    set cs [ $base.output2 childsite ]
    
    eval "button $cs.0 -text \"Reset Defaults\" -command { $this SetProcessingDefaults }"
    eval "button $cs.2 -text \"Show Model as Image\" -command  { $this CreateGLMTclImage }"
    
    eval "button $cs.1 -text \"Compute Rate GLM\" -command { $this ComputeRateGLM 1 1  }"
    pack $cs.0 $cs.2 -side right -padx 2
    pack $cs.1 -side left -padx 0 -expand true -fill x
    set parameters($this,tr) 1.0
    $this PlotHRF
}


#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::SetGlobalDrift { mode } { 

	if { $mode == 0 && $parameters($this,resting_slicemeanremoval) == 1 } {
	set parameters($this,resting_volumemeanremoval) 0
	} elseif { $mode == 1 && $parameters($this,resting_volumemeanremoval) == 1 } {
	set parameters($this,resting_slicemeanremoval) 0
	}
}

itcl::body bis_fmri::SetOutputType { mode } {
	
	if { $mode == 0 && $parameters($this,resting_rawcorrelation) == 1 } {
		set parameters($this,resting_ztransform) 0
	} elseif { $mode == 1 && $parameters($this,resting_ztransform) == 1 } {
		set parameters($this,resting_rawcorrelation) 0
	}
}


itcl::body bis_fmri::CreatePreprocessingFrame { ws { mode seed } } {

	iwidgets::Labeledframe $ws.top -labelpos nw -labeltext "Global Options" -relief ridge 
	pack $ws.top -side top -expand false -fill x

	frame $ws.bot;
	pack $ws.top $ws.bot -side top -expand false -fill x

	iwidgets::Labeledframe $ws.bot.1 -labelpos nw -labeltext "Global Pre-Processing" -relief ridge 
	iwidgets::Labeledframe $ws.bot.2 -labelpos nw -labeltext "Local  Pre-Processing" -relief ridge 
	pack $ws.bot.1 $ws.bot.2 -side left -expand true -fill both

	set w [ $ws.top childsite ]
	
	checkbutton $w.1 -variable [ itcl::scope parameters($this,resting_individualruns)  ] -text "Process Runs Individually"
	grid $w.1 -row 0 -column 0 -padx 0
#    pack $w.1 -side left -expand true -fill x
	
	if { $mode != "seed" } {
	checkbutton $w.2 -variable [ itcl::scope parameters($this,resting_usermask)  ] -text "Use VOI Image As Mask"	
#	pack $w.3 -side right -padx 2
	grid $w.2 -row 1 -column 0 -padx 0
	}
	
	if { $mode == "seed" } {
	checkbutton $w.3 -variable [ itcl::scope parameters($this,resting_rawcorrelation)  ] -text "Compute Raw Correlation"
	eval "$w.3 configure -command  { $this SetOutputType 0 } "
		
	checkbutton $w.4 -variable [ itcl::scope parameters($this,resting_ztransform)  ] -text "Gaussian Normalization (Can't compute Raw if it's selected)" 
	eval "$w.4 configure -command  { $this SetOutputType 1 } "
	grid $w.3 -row 1 -column 0 -padx 0
	grid $w.4 -row 2 -column 0 -padx 0
	
	
	
	
		
	
		
	}
	


	set w [ $ws.bot.2 childsite ]
	iwidgets::entryfield $w.1 -textbackground white -textvariable  [ itcl::scope parameters($this,resting_masklevel) ] -validate real -width 5 -labeltext "Mask Threshold:"
	iwidgets::entryfield $w.2 -textbackground white -textvariable  [ itcl::scope parameters($this,resting_driftpol) ] -validate integer -width 5 -labeltext "Drift Polynomial Order:"    
	iwidgets::entryfield $w.3 -textbackground white -textvariable  [ itcl::scope parameters($this,resting_temporalsigma) ] -validate real -width 5 -labeltext "Temporal Smoothing Sigma:"
	
	pack $w.1 $w.2 $w.3  -side top -expand false 
	::iwidgets::Labeledwidget::alignlabels $w.1 $w.2 $w.3


	set w [ $ws.bot.1 childsite ]
	checkbutton $w.1 -variable [ itcl::scope parameters($this,resting_slicemeanremoval)  ] -text "Remove Slice Mean" 
	eval "$w.1 configure -command { $this SetGlobalDrift 0 }"
	checkbutton $w.2 -variable [ itcl::scope parameters($this,resting_volumemeanremoval)  ] -text "Remove Volume Mean" 
	eval "$w.2 configure -command { $this SetGlobalDrift 1 }"

	#if { $mode != "seed" } {
	checkbutton $w.3 -variable [ itcl::scope parameters($this,resting_usemotionparams)  ] -text "Regress Motion Parameters" 
	eval "$w.3 configure -command { $this SetGlobalDrift 1 }"
	
	checkbutton $w.4 -variable [ itcl::scope parameters($this,resting_usecsfmean)  ] -text "Regress CSF/White Matter Mean" 
	eval "$w.4 configure -command { $this SetGlobalDrift 1 }"
	pack $w.1 $w.2 $w.3 $w.4   -side top -expand false 
	#} else {
	#pack $w.1 $w.2   -side top -expand false 
	#}
	#    ::iwidgets::Labeledwidget::alignlabels $w.1 $w.2 


}
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmri::CreateSeedBasedTab {base} {
	
	iwidgets::Labeledframe $base.output0 -labelpos nw -labeltext "Seed Based Correlation Specifications" -relief ridge 
	pack $base.output0 -side top -expand true -fill both -pady 2
	set w [ $base.output0 childsite ]

	iwidgets::Labeledframe $w.2 -labelpos nw -labeltext "Preprocessing Parameters" -relief ridge
	pack $w.2 -side top -expand true -fill x
	set ws [ $w.2 childsite ]

	$this CreatePreprocessingFrame $ws "seed"


	iwidgets::Labeledframe $base.output -labelpos nw -labeltext "Outputs" -relief ridge 
	pack $base.output -side top -expand true -fill both -pady 2

	set cs [ $base.output childsite ]

	set fr  [ frame $cs.1 ]; 
	pack $fr -side top -expand false -fill x


	eval "iwidgets::entryfield $fr.1 -textbackground white -labeltext \"Results Directory:\"  -textvariable [ itcl::scope parameters($this,output_dir) ] -relief sunken"
	eval "button $fr.2 -text \"...\" -command {$this GetName output_dir}"     
	pack $fr.2 -side right  -expand false  -padx 5 -pady 5
	pack $fr.1 -side left -expand true -fill x -padx 5 -pady 5

	#    checkbutton $cs.2 -variable [ itcl::scope parameters($this,afni_tout)  ] -text "Compute T-Statistics for each beta value"
	#    checkbutton $cs.3 -highlightthickness 0  -variable [ itcl::scope parameters($this,afni_deletetemp) ] -text "Do Not Save Temporary Files"
	#    pack $cs.2 $cs.3 -side top -expand false -padx 20

	
	iwidgets::Labeledframe $base.output2 -labelpos nw -labeltext "Connectivity Processing" -relief ridge 
	pack $base.output2 -side top -expand true -fill both -pady 2

	set cs [ $base.output2 childsite ]
	
	frame $cs.top ; pack $cs.top -side top -expand true -fill x -padx 5
	frame $cs.bot ; pack $cs.bot -side bottom -fill x -expand true -padx 5

	eval "button $cs.top.1 -text \"Seed to Image\" -command { $this ComputeSeedCorrelation 0 1 1 }"
	eval "button $cs.top.2 -text \"Seed to Seed\" -command { $this ComputeSeedCorrelation 1 1 1 }"
	eval "button $cs.top.3 -text \"Task Based Seed to Image\" -command { $this TaskBasedComputeSeedCorrelation 1 }"
	pack $cs.top.1 $cs.top.2 $cs.top.3 -side left -padx 0 -expand true -fill x
	
	eval "button $cs.bot.1 -text \"Reset Defaults\" -command { $this SetProcessingDefaults }"
	pack  $cs.bot.1 -side right -padx 5

}



itcl::body bis_fmri::CreateIntrinsicTab {base} {
	iwidgets::Labeledframe $base.output2 -labelpos nw -labeltext "Intrinsic Connectivity Processing" -relief ridge 
	pack $base.output2 -side bottom -expand true -fill x -pady 2


	iwidgets::Labeledframe $base.output0 -labelpos nw -labeltext "Intrinsic Correlation Specifications (SLOW!)" -relief ridge 
	pack $base.output0 -side top -expand true -fill both -pady 2
	set w [ $base.output0 childsite ]

	iwidgets::Labeledframe $w.2 -labelpos nw -labeltext "Preprocessing Parameters" -relief ridge
	pack $w.2 -side top -expand true -fill x
	set ws [ $w.2 childsite ]

	$this CreatePreprocessingFrame $ws "intrinsic"
	
	#------------------------Isabella's try-------------------------------------
	iwidgets::Labeledframe $w.3 -labelpos nw -labeltext "Connectivity Parameters" -relief ridge
	pack $w.3 -side bottom -expand true -fill x
	set wss [ $w.3 childsite ]
	
	iwidgets::entryfield $wss.1 -textbackground white -textvariable [ itcl::scope parameters($this,resting_threshold) ] -validate real -width 5 -labeltext "Threshold:"
	pack $wss.1 -side top -expand false
	#    ::iwidgets::Labeledwidget::alignlabels $ws.1 $ws.2 $ws.3 $wss.1
	
	iwidgets::Labeledframe $wss.2 -labelpos nw -labeltext "Range" -relief ridge
	pack $wss.2 -padx 10 -pady 10 -fill both -expand yes -side left
	
	set index 0
	set w [ $wss.2 childsite ]
	foreach f { Both Positive Negative } {
	radiobutton $w.$index -text [string totitle $f] -value $f -variable [ itcl::scope parameters($this,resting_range) ] 
	pack $w.$index -side left  -expand false -fill x -pady 2
	incr index
	}
	
	iwidgets::Labeledframe $wss.3 -labelpos nw -labeltext "Calculate" -relief ridge
	pack $wss.3 -padx 10 -pady 10 -fill both -expand yes -side right 
	
	set index 0
	set w [ $wss.3 childsite ]
	foreach f {Abs Sqr} {
	radiobutton $w.$index -text [string totitle $f]  -value $f -variable [ itcl::scope parameters($this,resting_mode) ] 
	pack $w.$index -side left -expand false -fill x -pady 2
	incr index
	}
	#---------------------------------------------------------------------------
	
	iwidgets::Labeledframe $base.output -labelpos nw -labeltext "Outputs" -relief ridge 
	pack $base.output -side top -expand true -fill both -pady 2

	set cs [ $base.output childsite ]

	set fr  [ frame $cs.1 ]; 
	pack $fr -side top -expand false -fill x


	eval "iwidgets::entryfield $fr.1 -textbackground white -labeltext \"Results Directory:\"  -textvariable [ itcl::scope parameters($this,output_dir) ] -relief sunken"
	eval "button $fr.2 -text \"...\" -command {$this GetName output_dir}"     
	pack $fr.2 -side right  -expand false  -padx 5 -pady 5
	pack $fr.1 -side left -expand true -fill x -padx 5 -pady 5

	
	
	set cs [ $base.output2 childsite ]
	
	set w1 [ frame $cs.0 ]
	set w2 [ frame $cs.1 ]
	pack $w1 $w2 -side top -expand true -fill x

	eval "button $w2.0 -text \"Reset Defaults\" -command { $this SetProcessingDefaults }"
	eval "button $w1.1 -text \"Intrinsic Correlation\" -command { $this ComputeIntrinsicCorrelation Whole 1 1  }"
	eval "button $w1.2 -text \"Ipsilateral Correlation\" -command { $this ComputeIntrinsicCorrelation Ipsilateral 1 1  }"
	eval "button $w1.3 -text \"Contralateral Correlation\" -command { $this ComputeIntrinsicCorrelation Contralateral 1 1  }"
	pack $w1.1 $w1.2 $w1.3  -side left -padx 0 -expand true -fill x
	pack $w2.0  -side right -padx 2
	
}


itcl::body bis_fmri::ClearAllData {ask } {
	set ok [ bis_fmrisetup::ClearAllData $ask ]
	if { $ok == 0 } {
	return $ok
	}

	return 1
}


itcl::body bis_fmri::UpdateGUI { { loadmode 1 } } {

	if { $basewidget == 0 } {
	return 0
	}

	wm title $basewidget "BioImage Suite::$appname [ file tail $setupname ]"
	update idletasks

	if { $loadmode == 0 } {
	return 1
	}

	$skipframesentry config -state disabled
	$runslistbox clear 
	$tasklistbox clear
	
	for {set runcount 1} {$runcount <= $numruns } {incr runcount} {
	$runslistbox insert end "Run_${runcount}_$parameters($this,display_run_file_${runcount})"
	}
	for {set taskcount 1} {$taskcount <= $numtasks} {incr taskcount} {
		$tasklistbox insert end "T$taskcount-$parameters($this,task_$taskcount)"

	}
	
	$this CloseBlockWidget
	$this PlotHRF
	$this CreateDataTreeStructure ""

	return 1
	

}

itcl::body bis_fmri::ExitCommand { } {
	set ok [ tk_messageBox -title "Exiting this Application" -type yesno  -message "Are you sure you want to close $appname. Any unsaved work will be lost."  ]
	if { $ok == "yes" } {
	pxtkexit
	}
	return 0
}


itcl::body bis_fmri::ShowCommandLineOptions { } {
	pxtkconsole
	pxtkprint "\n\n[ $this Usage]\n\n"
}

# -------------------------------------------------------------------------------------------------------------------

itcl::body bis_fmri::ReorganizeDataGUI { } {

	set dirname  [tk_chooseDirectory -title "Select Directory to reorganize/relocate data" ]
	if { [ string length $dirname ] < 1 } {
	return 0
	}
	
	if { [ file exists $dirname ] } {
	set newtail [ file root [ file tail $setupname ] ]
	set newtail "${newtail}data"
	set dirname [ file join $dirname $newtail ] 
	}
	set ok [ ::pxtclutil::Question "This operation will COPY your current files from their existing locations and reorganize them under $dirname. This can use a lot of disk space. Are you sure?" ]
	if { $ok == 0 } {
	return
	}
	
	return [ $this ReorganizeData $dirname ]
}


itcl::body bis_fmri::DisplayMSBFile { msbsetupname } { 

	if { $multisubjectcontrol !=0 } {
	$multisubjectcontrol ShowWindow Results " "
	update idletasks
	$multisubjectcontrol LoadSetup $msbsetupname
	$multisubjectcontrol LoadAll 1
	} elseif { $ownsorthoviewer > 0 } {
	$notebook view "DataTree"
	}
}


# ----------------------------------------------------------

if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {

	wm withdraw .
	set br [ toplevel .[ pxvtable::vnewobj ]]
	set element [bis_fmri \#auto $br 1  ]
	update idletasks
	if { $argc > 0 } {
	$element MainFunction $argc $argv
	}
}

