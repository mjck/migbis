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


lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] bis_algorithm ]


package require vtkpxcontrib 1.0
package require pxvtable 1.0
package require pxtclhelp  1.0
package require pxitclimage  1.0
package require pxitcltransform	 1.0
package require bis_datamanager	 1.0
package require bis_treewidget 1.0

# ------------------------------------
# Algorithms required
# ------------------------------------
package require bis_thresholdimage 1.0
package require bis_roimean 1.0
package require bis_csfwmmotioncorr 1.0
package require bis_temporalsmoothimage	  1.0
package require bis_removetemporaldrift 1.0
package require bis_removeslicemean 1.0
package require bis_imageremoveframes 1.0
package require bis_matrixcorrelation 1.0
package require bis_seedcorrelation 1.0
package require bis_cropimage 1.0


package require bis_intrinsicconnectivity 1.0

# ------------------------------------
# Provides
package provide bis_fmrisetup 1.0

#Class Definition
#----------------------------------------------------------------------------------------------------
itcl::class bis_fmrisetup {

    protected common parameters 
    private common progressbar 
    private common progress_lab 
    private common progress_val 
    
    protected variable numtasks 0
    protected variable numruns 0
    protected variable numblocks 0
    
    protected variable fmriruncount
    protected variable framecount
    protected variable fmriread_tframecount 0
    
    protected variable setupname "default.xmlg"
    
    protected variable processlist ""
    protected variable processnamelist ""
    protected variable restinglist ""
    protected variable restingnamelist ""
    protected variable referenceimagepaths ""
    protected variable referenceimagenames ""
    
    # Anywhere glmextra appears -- check!
    protected variable glmextralist ""
    protected variable glmextranamelist ""
    
    protected variable multisubjectcontrol 0
    protected variable datamanager 0
    protected variable datamanagerfilename ""
    protected variable treewidget 0
    
    # ------------------------------- Methods ---------------------------
    
    constructor { } { 
	set progressbar($this) 0
	set progress_lab($this) "Status"
	set progress_val($this) 0
	$this SetSetupDefaults
	pxtclvtkpxcontrib::LoadLibraries { vtkbisConnectivityTCL }
    }
    
    public method GetThisPointer { } { return $this }
    
    #Load/Save Files
    public method SaveSetupFile { { fname "" } }
    public method LoadSetupFile { { fname "" } }
    public method LoadParameterFile { { fname "" } }
    public method LoadOlderSetupFile { fname  }
    
    # Part methods
    protected method ImportParameterBlock { fileid { skipblank 0 }}
    
    #This is to import Pawel's old style setup file
    public method ImportfMRISetup { }
    #This is to output Rajeevan style XML File
    public method WriteXMLFile { { outdir 0 } }
    public method ReadXMLFile { fname { subjectno 1 } { onlyrun -1 }}
    
    # This method is used to update the GUI if present on load
    protected method UpdateGUI { { loadmode 1 }  } { return 1 }
    protected method HasGUI { { loadmode 1 }  } { return 0 }
    
    # Clear Stuff
    public method ClearAllArrays {par { doask 0 }}
    public method ClearAllData {ask }
    
    # Utilities
    public method StringParse {mode output_type end thistask thisrun}
    protected method IsWritable { fname }
    
    # Validate Block Definitions
    protected method ParseFramesString { val usingseconds tr numframes offset message { debug 0 } }
    public method ValidateBlockDefinitions { { creatematrix 0 } { debug 0 } }
    protected method ComputeEmptyGLMMatrixDimensions { { debug 0 }  }
    public  method CreateBaseGLMMatrix { { debug 0 } }
						       
	# AFNI Processing
	public method AFNICreateTimeFile { { tfname "" } { report 0 } }
	public method AFNICreateScript { { askfname 0 } {  report 0 } }
	public method AFNICreateScriptHelper { fout line { mirror 0 } }
	public method EnsureRunsAreNIFTI { }
	public method SetProcessingDefaults { }
	public method SetSetupDefaults { }

    # Internal GLM -- New! (years ago!)
    public method CreateWaverMatrix { }
    public method ComputeGLM { { askfname 0 } {  report 0 } }

    
    public method ComputeRateGLM { { askfname 0 } {  report 0 } }
    


    # GLM Pieces
    protected method PreprocessGLM { outnamestem { dodrift 1 } { numrate 0 } { singlerun 0 } }
    protected method ComputeGLMComponent { outnamestem { singlerun 0 } { setuptasklist "" } { dodrift 1 } { numrate 0 } }

    # Resting State algorithms
    public method ComputeSeedCorrelation { mode { askfname 0 } {  report 0 } } 
    public method ComputeSeedCorrelationSingle { mode inputimage maskimage filename { dosave 1 } }
    public method TaskBasedProcessing { task image maskimage vimg outname { addrange 0 } }
    public method TaskBasedComputeSeedCorrelation { { askfname 0 } }
   
    public method ComputeIntrinsicCorrelationSingle { mode allrunsimage maskimage outname } 
    public method ComputeComplexIntrinsicCorrelationSingle { mode allrunsimage maskimage outname } 
    public method ComputeIntrinsicCorrelation { mode { askfname 0 } {  report 0 } } 
    
    
    # Protected Methods utilities for Correlation
    # Need to be careful about these ...............
    protected method ConcatenateAndPreprocessRuns { mask { graywhitemask 0 } { checkformotioncorrection 0 } { enableskipframes 0 } }
    protected method PreprocessSingle4DImage { img mask { skipframeslist 0 } { motionmatrixname 0 } { graywhitemask 0 } }
    protected method CreateMask { allruns threshold }
    
    # VOI Stuff 
    protected method CheckTransformationsForVOI { { graywhitemaskmode 0  } }
    protected method CheckVOIImageSize { voiimage funcimg { graywhitemaskmode 0 } }
    protected method GetReslicedVOIImage { funcimg { graywhitemaskmode 0 } }
    
    # Protected Methods for MSB Files
    protected method GenerateOutputFilenames { { asfname 0	} }
    protected method CreateMSBFile { msbsetupname tasklist }
    protected method DisplayMSBFile { msbsetupname }
    
    # Methods for XML Stuff
    protected method CreateDataTreeStructure { { tasklist "" } { doupdate 1 } }
    public method UpdateDataTreeStructure { { imagename "" } }
    
    
    # Default Stuff
    public method ReorganizeData { newdir }
    
    
    # Add Logo Label
    public proc   AddLogoLabel { w }
    
    # create progress bar 
    protected method CreateProgressBar { w } 
    
    # Progress Stuff
    public method ProgressCallback	 { filter lab } 
    public method ShowProgress	 { filter lab } 
    public method ShowProgressVal	 { name	  val }
    public method SetFilterCallbacks { filter comment } 
    public method GetFirstInputObject { }  { return 0 }
    
    
    # Main Function
    public method MainFunction { numarg arglist }
    public method Usage	   { }
    public method InfoMessage { msg { title "" } { icon info } }
    
    
    # Get Data Manager
    public method GetDataManager { } { return $datamanager }
    public method GetDataManagerFilename { } { return $datamanagerfilename }

};






#Saves variable parameters to .xmlg file
#Called in CreateGUI
#Saves variable information for current xml generator session  so that user can load again later with load command
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::SaveSetupFile { { fname "" } } {



	if { $fname == "" } {
	set savename $setupname
	set filetype1 [ list "XML Gen Files" [ list .xmlg ] ]
	set savename [tk_getSaveFile -title "Save XML Gen File" -defaultextension .xmlg -initialfile [ file tail $setupname ] -initialdir [ file dirname $setupname ]  -filetypes [ list $filetype1 ] ]
	if { [ llength $savename ] < 1 } {
		return 
	}
	} else {
	set savename $fname
	}

	set setupname $savename
	set savefile [file tail $savename]

	set fileid [open $savename w+ ]
	
	set imagepath $::pxtclvtkpxcontrib::imagedir
	

	set ind [ lsearch -exact $referenceimagenames $parameters($this,display_reference_brain) ]
	if { $ind >=0 } {
	set parameters($this,reference_brain_path) [ lindex $referenceimagepaths $ind ] 
	} else { 
	set parameters($this,reference_brain_path) $parameters($this,reference_brain) 
	}   

	puts $fileid "#BioImageSuite Study Description File v2"	 
	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Study Title"
	puts $fileid "$parameters($this,study_title)"
	puts $fileid "#Subject ID"
	puts $fileid "$parameters($this,subject_id)"

	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Anatomical Data"
	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Reference Brain"
	puts $fileid "$parameters($this,reference_brain_path)"
	puts $fileid "#Reference Gray/White/CSF Map"
	puts $fileid "$parameters($this,reference_gw_map)"
	puts $fileid "#3D Anatomical Image"
	puts $fileid "$parameters($this,anatomical)"
	puts $fileid "#Conventional Image"
	puts $fileid "$parameters($this,conventional)"
	puts $fileid "#Ref-> 3D Anatomical Transformation"
	puts $fileid "$parameters($this,referenceTransform)"
	puts $fileid "#3D Anatomical -> Conventional Transformation"
	puts $fileid "$parameters($this,internalTransform)"
	puts $fileid "#Conventional -> EPI Transformation"
	puts $fileid "$parameters($this,distortionTransform)"

	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Basic Parameters"
	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Session ID"
	puts $fileid "$parameters($this,session_id)"
	puts $fileid "#Session Description"
	puts $fileid "$parameters($this,session_description)"
	puts $fileid "#Number of Runs"
	puts $fileid "$numruns"
	puts $fileid "#Number of Tasks"
	puts $fileid "$numtasks"
	puts $fileid "#Number of Outcomes"
	puts $fileid "$parameters($this,num_outcomes)"
	puts $fileid "#Repetition Time (TR)"
	puts $fileid "$parameters($this,tr)"
	puts $fileid "#Number of Slices"
	puts $fileid "$parameters($this,numslices)"
	puts $fileid "#Number of Frames"
	puts $fileid "$parameters($this,numframes)"
	puts $fileid "#Skip Frames On"
	puts $fileid "$parameters($this,checks)"
	puts $fileid "#Frames to Skip"
	puts $fileid "$parameters($this,skipFrames)"

	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Outputs Files/Locations"
	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#XML Output File"
	puts $fileid "$parameters($this,xml_output_file)"
	puts $fileid "#XML Output Directory"
	puts $fileid "$parameters($this,xml_output_dir)"
	puts $fileid "#Data Output Directory"
	puts $fileid "$parameters($this,output_dir)"

	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Runs"
	puts $fileid "#-----------------------------------------------------"

	for {set runcount 1} {$runcount <= $numruns} {incr runcount} {
	puts $fileid "#Run $runcount (first line=4D Image, second line=Matrix with Motion Parameters)"
	puts $fileid "$parameters($this,run_file_$runcount)"
	puts $fileid "$parameters($this,run_motionfile_$runcount)"
	if { $parameters($this,runskip_$runcount) == 0 } {
		puts $fileid "No Skip"
	} else {
		puts $fileid $parameters($this,skiprunsframe_${runcount})
	}
	}

	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Tasks"
	puts $fileid "#-----------------------------------------------------"
   
	for {set taskcount 1} {$taskcount <= $numtasks} {incr taskcount} {
	puts $fileid "#Task $taskcount"
	puts $fileid "$parameters($this,task_$taskcount)"
	
	}

	set numblocks [expr $numtasks*$numruns]
	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Blocks"
	puts $fileid "#-----------------------------------------------------"
	
	puts $fileid "#Block Definition Time (1=seconds, 0 =frames)"
	puts $fileid "$parameters($this,blockunits)"

	for {set runcount 1} {$runcount <= $numruns} {incr runcount} {
	for {set taskcount 1} {$taskcount <= $numtasks} {incr taskcount} {
		puts $fileid "#Block Task=$taskcount, Run=$runcount ($parameters($this,task_$taskcount),$parameters($this,run_file_$runcount))"
	    puts $fileid "$parameters($this,tframe_${taskcount}_${runcount})"
	}
	}

	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Outcomes"
	puts $fileid "#-----------------------------------------------------"
	
	for {set outcount 1} {$outcount <= $parameters($this,num_outcomes)} {incr outcount} {
	puts $fileid "#OutCome $outcount"
	puts $fileid "$parameters($this,outcome_$outcount)"
	}	  

	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#GLM Processing and AFNI Integration"
	puts $fileid "#-----------------------------------------------------"
	
	for { set i 0 } { $i < [ llength $processlist ] } { incr i } {
	set elem [ lindex $processlist $i ]
	set desc [ lindex $elem 0 ]
	set name [ lindex $elem 1 ]

	puts $fileid "$desc"
	puts $fileid "$name : $parameters($this,afni_$name)"
	}

	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Resting State Connectivity Extra Inputs"
	puts $fileid "#-----------------------------------------------------"
	
	puts $fileid "#VOI Definition"
	puts $fileid "$parameters($this,voiDefinitionImage)"
	puts $fileid "#VOI Definition Space (Reference,Anatomical,Conventional,Functional)"
	puts $fileid "$parameters($this,voiDefinitionSpace)"
	puts $fileid "#Inverse Reference Transformation"
	puts $fileid "$parameters($this,inverseReferenceTransform)"


	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Connectivity Parameters"
	puts $fileid "#-----------------------------------------------------"
	
	for { set i 0 } { $i < [ llength $restinglist ] } { incr i } {
	set elem [ lindex $restinglist $i ]
	set desc [ lindex $elem 0 ]
	set name [ lindex $elem 1 ]

	puts $fileid "$desc"
	puts $fileid "$name : $parameters($this,resting_$name)"
	}

	puts $fileid "#-----------------------------------------------------"
	puts $fileid "#Extra GLM Parameters Parameters"
	puts $fileid "#-----------------------------------------------------"

	for { set i 0 } { $i < [ llength $glmextralist ] } { incr i } {
	    set elem [ lindex $glmextralist $i ]
	    set desc [ lindex $elem 0 ]
	    set name [ lindex $elem 1 ]
	    
	    puts $fileid "$desc"
	    puts $fileid "$name : $parameters($this,glmextra_$name)"
	}
	
	close $fileid
	
	return [ $this UpdateGUI ]

    }
#Check if a file or directory is writable
#-----------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::IsWritable { fname } {
	
	if { [ file exists $fname ] } {
		set ok [ file writable $fname ]
	} else {
		set ok [ file writable [ file dirname [ file normalize $fname ] ] ]
	}
	return $ok
}
#Clears all parameters. If value of "ask" is 1 then a window appears asking
#user whether to clear contents. If ask=0 then the function is called
#internally and the user is not prompted
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::ClearAllData {ask } {
	
	if {$ask==1} {
	set ok [ tk_messageBox -type yesno -default no -title "WARNING!" -message "This will clear current parameters!" -icon question ]
	if { $ok=="no" } {
		return 0
	}
	}
	
	$this ClearAllArrays tframe
	$this ClearAllArrays runs
	$this ClearAllArrays task
	$this ClearAllArrays outcome
	#puts "Number of task, runs before zeroing $numtasks,$numruns"
	$this SetSetupDefaults
}

itcl::body bis_fmrisetup::SetSetupDefaults { } {
	set numruns 0
	set numtasks 0
	set numblocks 0
	set parameters($this,checks) 0
	set parameters($this,skipFrames) ""
	set parameters($this,num_outcomes) 0
	set parameters($this,study_title) ""
	set parameters($this,subject_id) ""
	set parameters($this,reference_brain) ""
	set parameters($this,display_reference_brain) ""
	set parameters($this,conventional) ""
	set parameters($this,display_conventional) ""
	set parameters($this,anatomical) ""
	set parameters($this,display_anatomical) ""
	set parameters($this,referenceTransform) ""
	set parameters($this,display_referenceTransform) ""
	set parameters($this,internalTransform) ""
	set parameters($this,display_internalTransform) ""
	set parameters($this,distortionTransform) ""
	set parameters($this,display_distortionTransform) ""
	set parameters($this,voiDefinitionImage) ""
	set parameters($this,display_voiDefinitionImage) ""
	set parameters($this,inverseReferenceTransform) ""
	set parameters($this,display_inverseReferenceTransform) ""
	set parameters($this,voiDefinitionSpace) "Functional"
	set parameters($this,display_reference_gw_map) ""
	set parameters($this,reference_gw_map) ""

	set parameters($this,session_id) ""
	set parameters($this,session_description) ""
	set parameters($this,tr) 1.0
	set parameters($this,numslices) ""
	set parameters($this,numframes) ""
	set parameters($this,blockunits) 1
	set parameters($this,xml_output_file) ""
	set parameters($this,xml_output_dir) ""
	set parameters($this,output_dir) ""
	


	set referenceimagepaths ""
	set referenceimagenames ""

	set imagepath $::pxtclvtkpxcontrib::imagedir
	lappend referenceimagenames "MNI_Colin_1mm" 
	lappend referenceimagepaths  [ file join $imagepath MNI_T1_1mm.nii.gz ]
	lappend referenceimagenames "MNI_Colin_1mm_Stripped" 
	lappend referenceimagepaths  [ file join $imagepath MNI_T1_1mm_stripped.nii.gz ]
	lappend referenceimagenames "MNI_Colin_2mm" 
	lappend referenceimagepaths  [ file join $imagepath MNI_T1_2mm.nii.gz ]
	lappend referenceimagenames "MNI_Colin_2mm_Stripped" 
	lappend referenceimagepaths  [ file join $imagepath MNI_T1_2mm_stripped.nii.gz	]



	# Afni parameters
	$this SetProcessingDefaults
	
	return 1
}

#Loads fMRI setup file by individually loading all variables based on parsed results
#----------------------------------------------------------------------------------------------------	 
itcl::body bis_fmrisetup::ImportfMRISetup {  } {

	set filetype2 [list "fMRI Setup Files" [list .*] ]
	set loadname2 [tk_getOpenFile -title "Load fMRI Setup File" -filetypes [list $filetype2] ]
	if { [ string length $loadname2] < 1  } {
	return
	}
	set loadfile2 [file tail $loadname2]

	set fmri_file_id [open $loadname2]
	seek $fmri_file_id 0 start
	set fmri_setup_file [read -nonewline $fmri_file_id]

	seek $fmri_file_id 0 start
	set start "Start Name"
	set study_name_line 16
	set headercount 1

	$this ClearAllData 0

	

	while {$headercount<12} {
	
	gets $fmri_file_id line
	if {[string match [string range $line 0 9] "Study Name"]==1} {
		
		set starter [string first "#" $line] 
		set ender [string last "#" $line]
		set parameters($this,study_title) [string range $line [expr $starter+2] [expr $ender-1]]
	}
	
	if {[string match [string range $line 0 11] "Study Number"]==1} {
		set starter [string first "#" $line] 
		set ender [string last "#" $line]
		set parameters($this,study_number) [string range $line [expr $starter+2] [expr $ender-2]]
	  }	
	
	if {[string match [string range $line 0 12] "series number"]==1} {
		#puts $parameters($this,series_number)
		set starter [string first "#" $line] 
		set ender [string last "#" $line]
		set parameters($this,series_number) [string range $line [expr $starter+2] [expr $ender-2]]
		
	}
	if {[string match [string range $line 0 23] "directory for saved data"]==1} {
		set starter [string first "#" $line] 
		set ender [string last "#" $line]
		set parameters($this,output_dir) [string range $line [expr $starter+18] [expr $ender-4]]
	}
	
	if {[string match [string range $line 0 15] "Number of slices"]==1} {
		set starter [string first "#" $line] 
		set ender [string last "#" $line]
		set parameters($this,numslices) [string range $line [expr $starter+2] [expr $starter+5]]
		
	}
	
	if {[string match [string range $line 0 25] "Number of images per slice"]==1} {
		set starter [string first "#" $line] 
		set ender [string last "#" $line]
		set parameters($this,numframes) [string range $line [expr $starter+2] [expr $starter+5]]
	}
	
	if {[string match [string range $line 0 23] "Number of different sets"]==1} {
		set starter [string first "#" $line] 
		set ender [string last "#" $line]
		set numtasks [string range $line [expr $starter+2] [expr $starter+5]]
		#  puts "NUMBER OF TASKS: $numtasks"
	}
	set headercount [expr $headercount+1]
	
	}
	
	
	for {set taskcount4 1} {$taskcount4<=$numtasks} {incr taskcount4} {
	set bodycount 0
#	puts "TASKCOUNT $taskcount4"
	set subbodycount 0
		set parameters($this,fmrirun_$taskcount4) 0
	set parameters($this,fmriruncount_$taskcount4) 0
	set parameters($this,tframecount_$taskcount4) 0
	while {$bodycount<2} {
		
		gets $fmri_file_id body_line
		#   puts "READ ONE LINE Loop1"
		if {[string match [string range $body_line 0 21] "List of images for set"]==1} {
		set starter [string first "#" $body_line] 
		set ender [string last "#" $body_line]
		set parameters($this,runs_list_$taskcount4) [string range $body_line [expr $starter+1] [expr $ender-1]]
		$this StringParse "runs_list_$taskcount4" "fmrirun_" $ender $taskcount4 0
		#     puts "Runcount for task $taskcount4 is $parameters($this,fmriruncount_$taskcount4)"
		#    puts "List of images for task $taskcount4 is $parameters($this,runs_list_$taskcount4)"
		set bodycount [expr $bodycount+1]
		}
		
		if {[string match [string range $body_line 0 11] "OFF set name"]==1} {
		set starter [string first "#" $body_line] 
		set ender [string last "#" $body_line]
		set parameters($this,task_$taskcount4) [string range $body_line [expr $starter+2] [expr $ender-1]]
		#   puts "Current Task name: $parameters($this,task_$taskcount4)"
		#$tasklistbox insert end "T$taskcount4-$parameters($this,task_$taskcount4)"

		set bodycount [expr $bodycount+1]
		}
		if {$subbodycount>=4} {
		set bodycount 3
		}
		set subbodycount [expr $subbodycount+1]
		
	}
	#	puts "OUT OF FIRST LOOP runcount- $parameters($this,fmriruncount_$taskcount4)"
	
	for {set bodycount2 1} {$bodycount2<=$parameters($this,fmriruncount_$taskcount4)} {incr bodycount2} {
		gets $fmri_file_id body_line2
		#    puts "READ ONE LINE Loop2"
		if {[string match [string range $body_line2 0 5] "ranges"]==1} {
		set start [string first "#" $body_line2]
		set ender [string last "#" $body_line2]
		set parameters($this,range_$taskcount4) [string range $body_line2 [expr $starter-3] [expr $ender-1]]
		set framecount 0		#	puts $parameters($this,range_$taskcount4)
		$this StringParse "range_$taskcount4" "fmriframe_" $ender $taskcount4 $bodycount2
		
		}
	}
	}
	set convcount 0
	
	while {$convcount < 12} {
	
		gets $fmri_file_id line
	#	puts $line
	if {[string match [string range $line 0 30] "Series/File Name for anatomical"]==1} {
		set starter [string first "#" $line] 
		set ender [string last "#" $line]
		set conv [string range $line [expr $starter+2] [expr $ender-2]]
		#	    puts "STUDY NUMBER $parameters($this,study_number)"
		#	    puts "CONVER: $conv"
		set parameters($this,conventional) "$parameters($this,study_number)/$conv.hdr"
		set parameters($this,display_conventional) [file tail $parameters($this,conventional)]
	}
	
	set convcount [expr $convcount+1]
	}


	set lasttask [expr $taskcount4-1]
   
	for {set runcount 1} {$runcount <= $parameters($this,fmriruncount_$lasttask)} {incr runcount} {
		#	    $runslistbox insert end "Run_$runcount"
		set bonanza "00"
		set letterbonanza $bonanza$parameters($this,fmrirun_$runcount)
		set countbonanza [string length $letterbonanza]
		if {$countbonanza==4} {
		set letterbonanza "0$parameters($this,fmrirun_$runcount)"
		} elseif {$countbonanza==5} {
		set letterbonanza "$parameters($this,fmrirun_$runcount)"
		}
		set parameters($this,run_file_$runcount) "$parameters($this,study_number)/$parameters($this,series_number)_S$letterbonanza.hdr"
		set parameters($this,display_run_file_$runcount) [file tail $parameters($this,run_file_$runcount)]

		set numruns [expr $numruns+1]
		
		set parameters($this,runskip_$runcount) 0
		}
	
	set numblocks [expr $numtasks*$numruns]
	set blockid 1

	for {set runcount2 1} {$runcount2 <= $numruns} {incr runcount2} {
	    for {set taskcount2 1} {$taskcount2 <= $numtasks} {incr taskcount2} {
		set parameters($this,skiprunsframe_$runcount2) ""
		set parameters($this,tframe_${taskcount2}_${runcount2}) $parameters($this,{t$taskcount2}_{r$runcount2})
	    }
	}

	return [ $this UpdateGUI ]
}

#Called in loadfMRI, parses strings based on spaces

#----------------------------------------------------------------------------------------------------  
itcl::body bis_fmrisetup::StringParse { mode outputtype end thistask thisrun} {
	set count 0
	set fmriread_tframecount 0
	set parameters($this,tframecount_$thistask) 0
	set parsecount 0
	
	while {$parsecount<$end} {
	set parameters($this,parselist_$parsecount) [string first " " $parameters($this,$mode) $parsecount]
	set parsecount [expr $parsecount+1]
	}
	
	
	set c 1
	
	while {$parameters($this,parselist_$c)!=-1} {
	set cadd1 [expr $c+1]
	set cadd2 [expr $c+2]
	set cadd3 [expr $c+3]
	set csub1 [expr $c-1]
	set csub2 [expr $c-2]
	
	if {$parameters($this,parselist_$c)==$parameters($this,parselist_$cadd1) && $parameters($this,parselist_$cadd1)!=$parameters($this,parselist_$cadd2) && $parameters($this,parselist_$c)!=$parameters($this,parselist_$cadd3) &&	 $parameters($this,parselist_$c)!=$parameters($this,parselist_$csub1) } {
		if {$outputtype=="fmrirun_"} {
		set parameters($this,fmriruncount_$thistask) [expr $parameters($this,fmriruncount_$thistask)+1]			  
		}
		if {$outputtype=="fmriframe_"} {
		set parameters($this,tframecount_$thistask) [expr $parameters($this,tframecount_$thistask)+1]
		}
		
		
		
		set count [expr $count+1]
		set parameters($this,$outputtype$count) [string range $parameters($this,$mode) $c $c]
	} elseif {$parameters($this,parselist_$c)==$parameters($this,parselist_$cadd1) && $parameters($this,parselist_$cadd1)==$parameters($this,parselist_$cadd2) && $parameters($this,parselist_$c)!=$parameters($this,parselist_$cadd3) &&  $parameters($this,parselist_$c)!=$parameters($this,parselist_$csub1) } {
		if {$outputtype=="fmrirun_"} {
		set parameters($this,fmriruncount_$thistask) [expr $parameters($this,fmriruncount_$thistask)+1]
		}   
		if {$outputtype=="fmriframe_"} {
		set parameters($this,tframecount_$thistask) [expr $parameters($this,tframecount_$thistask)+1]
		}
		
		
		set count [expr $count+1]
		set parameters($this,$outputtype$count) [string range $parameters($this,$mode) $c $cadd1]
		#	puts $parameters($this,$outputtype$count)
	} elseif {$parameters($this,parselist_$c)==$parameters($this,parselist_$cadd1) && $parameters($this,parselist_$cadd1)==$parameters($this,parselist_$cadd2) && $parameters($this,parselist_$c)==$parameters($this,parselist_$cadd3) &&  $parameters($this,parselist_$c)!=$parameters($this,parselist_$csub1) && $parameters($this,parselist_$c)!=$parameters($this,parselist_$csub2) } {
		if {$outputtype=="fmrirun_"} {
		set parameters($this,fmriruncount_$thistask) [expr $parameters($this,fmriruncount_$thistask)+1]
		}
		
		if {$outputtype=="fmriframe_"} {
		set parameters($this,tframecount_$thistask) [expr $parameters($this,tframecount_$thistask)+1]
		}
		
		set count [expr $count+1]
		set parameters($this,$outputtype$count) [string range $parameters($this,$mode) $c $cadd2]
		#	puts $parameters($this,$outputtype$count)
	}
	
	set c [expr $c+1]
	}
	
	set parameters($this,{t$thistask}_{r$thisrun}) ""
	
	if {$outputtype=="fmriframe_"} {
	for {set framecount2 1} {$framecount2<=$parameters($this,tframecount_$thistask)} {incr framecount2} {
		set framecount2plus1 [expr $framecount2+1]
		set parameters($this,{t$thistask}_{r$thisrun}) "$parameters($this,{t$thistask}_{r$thisrun}) $parameters($this,fmriframe_$framecount2) $parameters($this,fmriframe_$framecount2plus1); "
		set framecount2 [expr $framecount2+1]
		# puts "Task $thistask Run $thisrun: $parameters($this,{t$thistask}_{r$thisrun})"
		
		}
   }

}
   

#Loads variable parameters from .xmlg file
#Called in CreateGUI
#Loads variable information from previous xml generator session, asks user to specify .xmlg file, sets variable 
#information and displays values in each variable dialogue.	
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::LoadSetupFile { { fname "" } } {
	 
    if { [ string length $fname  ] == 0 } {
	set savename "" 
	set typelist { {"BioImage Suite Setup File" { .xmlg }} {"Old Style XML Setup File" { .xml }}}
	set loadname [tk_getOpenFile -title "Load Filename" -filetypes $typelist ]
	if { [ string length $loadname] < 1  } {
	    return 0
	}
    } else {
	set loadname $fname
    }
    
    if { [ file extension $fname ] == ".xml"  } {
	return [ $this ReadXMLFile $fname ]
    }

    if { [ file exists $loadname ] == 0 } {
	return 0
    }
    set loadfile [file tail $loadname ]
    set setupname $loadname
    
    $this ClearAllData 0
    

	set fileid [open $loadname r ]
	seek $fileid 0 start

	gets $fileid line
	if { [ string compare $line "#BioImageSuite Study Description File"  ] != 0 &&
	 [ string compare $line "#BioImageSuite Study Description File v2"  ] != 0 } {
	close $fileid
	if { [ string compare $line "#BioImageSuite Study Description Parameter File"  ] == 0 } {
		return 0
	}
	return [ $this LoadOlderSetupFile $loadfile ]
	}

	set motionmode 0
	if { [ string compare $line "#BioImageSuite Study Description File v2"	] == 0 } {
	set motionmode 1
	}


	# First Block Study Title etc
	gets $fileid line;    
	gets $fileid line;    gets $fileid parameters($this,study_title)
	gets $fileid line;    gets $fileid parameters($this,subject_id)

	# Second Block Anatomical Data
	gets $fileid line;    gets $fileid line;    gets $fileid line;	  
	gets $fileid line;	  gets $fileid parameters($this,reference_brain_path)
	if { $motionmode == 1 } {
	gets $fileid line;	  gets $fileid parameters($this,reference_gw_map)
	} else {
	set parameters($this,reference_gw_map) ""
	}
	gets $fileid line;	  gets $fileid parameters($this,anatomical)
	gets $fileid line;	  gets $fileid parameters($this,conventional)
	gets $fileid line;	  gets $fileid parameters($this,referenceTransform)
	gets $fileid line;	  gets $fileid parameters($this,internalTransform)
	gets $fileid line;	  gets $fileid parameters($this,distortionTransform)

	# Set some additional stuff
	set parameters($this,reference_brain) $parameters($this,reference_brain_path)
	set parameters($this,display_reference_brain) [file tail $parameters($this,reference_brain_path)]
	set parameters($this,display_reference_gw_map) [file tail $parameters($this,reference_gw_map)]
	set parameters($this,display_referenceTransform) [file tail $parameters($this,referenceTransform)]
	set parameters($this,display_internalTransform) [file tail $parameters($this,internalTransform)]
	set parameters($this,display_distortionTransform) [file tail $parameters($this,distortionTransform)]
	set parameters($this,display_conventional) [file tail $parameters($this,conventional)]
	set parameters($this,display_anatomical) [file tail $parameters($this,anatomical)]
	
	# Third Block Basic Parameters
	gets $fileid line;    gets $fileid line;    gets $fileid line;	  
	gets $fileid line;    gets $fileid parameters($this,session_id)
	gets $fileid line;    gets $fileid parameters($this,session_description)
	gets $fileid line;    gets $fileid numruns
	gets $fileid line;    gets $fileid numtasks
	gets $fileid line;    gets $fileid parameters($this,num_outcomes)
	gets $fileid line;    gets $fileid parameters($this,tr)
	gets $fileid line;    gets $fileid parameters($this,numslices)
	gets $fileid line;    gets $fileid parameters($this,numframes)
	gets $fileid line;    gets $fileid parameters($this,checks)
	set parameters($this,checks) [ expr $parameters($this,checks) > 0 ]

	#    if {$parameters($this,checks)==1} {
	#	$skipframesentry config -state normal -textbackground white
	#    }
	gets $fileid line;    gets $fileid parameters($this,skipFrames)

	# Fourth Block Output Locations
	gets $fileid line;    gets $fileid line;    gets $fileid line;	  
	gets $fileid line;    gets $fileid parameters($this,xml_output_file)
	gets $fileid line;    gets $fileid parameters($this,xml_output_dir)
	gets $fileid line;    gets $fileid parameters($this,output_dir)
	
	
	# Fifth Block Runs
	gets $fileid line;    gets $fileid line;    gets $fileid line;	  
	for {set runcount 1} {$runcount <= $numruns} {incr runcount} {
	#	puts stdout "Reading run $runcount / $numruns"
		#$runslistbox insert end "Run_$runcount"
	
	gets $fileid line;   gets $fileid parameters($this,run_file_$runcount)
	set parameters($this,display_run_file_$runcount) [file tail $parameters($this,run_file_$runcount)]
	if { $motionmode == 1 } {
		gets $fileid parameters($this,run_motionfile_$runcount)
	} else {
		set parameters($this,run_motionfile_$runcount) ""
	}
	set parameters($this,display_run_motionfile_$runcount) [file tail $parameters($this,run_motionfile_$runcount)]

	puts stdout "Read Run = $parameters($this,display_run_file_$runcount), $parameters($this,run_motionfile_$runcount)"

	gets $fileid tmp
	set tmp [ string trim $tmp ]
	if { $tmp == "No Skip" } {
		set parameters($this,runskip_$runcount) 0
		set parameters($this,skiprunsframe_$runcount) ""
	} else {
		set parameters($this,runskip_$runcount) 1
		set parameters($this,skiprunsframe_$runcount) $tmp
	}


	}
	
	# Sixth Block Tasks
	gets $fileid line;    gets $fileid line;    gets $fileid line;	  
	for {set taskcount 1} {$taskcount <= $numtasks} {incr taskcount} {
	gets $fileid line; gets $fileid parameters($this,task_$taskcount)
	#$tasklistbox insert end "T$taskcount-$parameters($this,task_$taskcount)"

	}
	
	# Seventh Block -- BLOCKS
	gets $fileid line;    gets $fileid line;    gets $fileid line;	  
	set numblocks [expr $numtasks*$numruns]
	gets $fileid line 
	gets $fileid parameters($this,blockunits)
	set parameters($this,blockunits) [ expr $parameters($this,blockunits) > 0 ]

	
	for {set runcount 1} {$runcount <= $numruns} {incr runcount} {
	    for {set taskcount 1} {$taskcount <= $numtasks} {incr taskcount} {
		gets $fileid line
		gets $fileid parameters($this,tframe_${taskcount}_${runcount})
	    }
	}
	
	# Eight Block -- Outcomes
	gets $fileid line;    gets $fileid line;    gets $fileid line;	  
	for {set outcount 1} {$outcount <= $parameters($this,num_outcomes)} {incr outcount} {
	gets $fileid line;    
		gets $fileid parameters($this,outcome_$outcount)
	}
	
	#   puts $numruns
	#    puts $numtasks


	set line ""
	gets $fileid line;   
	if { [ string length $line ] < 5 } {
	close $fileid; return 1
	}

	$this SetProcessingDefaults
	$this ImportParameterBlock $fileid 
	close $fileid
	puts stdout "On To Update GUI"
	return [ $this UpdateGUI ]
}
# -------------------------------------------------------------------------
itcl::body bis_fmrisetup::LoadParameterFile { { fname "" } } {
	 
	if { [ string length $fname  ] == 0 } {
	set savename "" 
	set typelist { {"BioImage Suite Setup PARAMETER Only File" { .xmlg }}}
	set loadname [tk_getOpenFile -title "Load Filename" -filetypes $typelist ]
	if { [ string length $loadname] < 1  } {
		return 0
	}
	} else {
	set loadname $fname
	}

	if { [ file exists $loadname ] == 0 } {
	return 0
	}
	set fileid [open $loadname r ]
	gets $fileid line
	if { [ string compare $line "#BioImageSuite Study Description Parameter File"  ] != 0 } {
	close $fileid
	puts stderr "Header of Extra File must be #BioImageSuite Study Description Parameter File"
	return 0
	}

	$this ImportParameterBlock $fileid 1
	return 1
}
# -------------------------------------------------------------------------
itcl::body bis_fmrisetup::ImportParameterBlock { fileid { skipblank 0 } } {

	# NINTH Block -- AFNI Stuff
	gets $fileid line;    gets $fileid line;    
	gets $fileid line

	set done 0

	while { [ gets $fileid line ] >=0 && $done == 0 } {
	set index [string first "#" $line ]
	if { $index == -1 } {
		set index [string first ":" $line]
		if { $index > 0 } {
		set len	  [string length $line]
		set key	  [string trim	 [string range $line 0 [expr $index - 1]]]
		set value [string trim [string range $line [expr $index +1] [expr $len -1]]]
		
		set ind [ lsearch  -exact $processnamelist $key ]
		if { $ind >= 0 } {
			set parameters($this,afni_$key) $value 
		} else {
			pxtkprint "Bad parameter \"$key\" specified in setup file. Ignored!\n"
			pxtkconsole
		}
		}
	} elseif { [ string first "Resting State" $line ] >=0 } {
		set done 1
	}
	}

	# Found Resting State ....

	gets $fileid line;    
	gets $fileid line 
	set line [ string trim $line ]
	if { $line !="" || $skipblank == 0 } {
	set parameters($this,voiDefinitionImage) $line
	set parameters($this,display_voiDefinitionImage) [file tail $parameters($this,voiDefinitionImage)]
	gets $fileid line;    
	}
	gets $fileid line
	set line [ string trim $line ]
	if { $line !="" || $skipblank == 0 } {
	set parameters($this,voiDefinitionSpace) $line
	set a $parameters($this,voiDefinitionSpace)
	if { $a != "Reference" && $a != "Conventional" && $a != "Anatomical" } {
		set parameters($this,voiDefinitionSpace) "Functional"
	}
	}

	gets $fileid line
	gets $fileid line
	set line [ string trim $line ]
	if { $line !="" || $skipblank == 0 } {
	set parameters($this,inverseReferenceTransform) $line
	set parameters($this,display_inverseReferenceTransform) [file tail $parameters($this,inverseReferenceTransform)]
	}

	# Parameters next
	gets $fileid line;     gets $fileid line;     gets $fileid line;


	set done 0

	while { [ gets $fileid line ] >=0 && $done == 0 } {
	set index [string first "#" $line ]
	if { $index == -1 } {
		set index [string first ":" $line]
		if { $index > 0 } {
		set len	  [string length $line]
		set key	  [string trim	 [string range $line 0 [expr $index - 1]]]
		set value [string trim [string range $line [expr $index +1] [expr $len -1]]]
		
		set ind [ lsearch  -exact $restingnamelist $key ]
		if { $ind >= 0 } {
			set parameters($this,resting_$key) $value 
		} else {
			pxtkprint "Bad parameter \"$key\" specified in setup file. Ignored!\n"
			pxtkconsole
		}
		}
	}
		if { [ string trim $line ] == "#Extra GLM Parameters Parameters" } {
			set done 1
		}
		
	}

	# GLM Extra next
	   gets $fileid line;


	set done 0

	while { [ gets $fileid line ] >=0 && $done == 0 } {
	    set index [string first "#" $line ]
	    if { $index == -1 } {
		set index [string first ":" $line]
		if { $index > 0 } {
		    set len	  [string length $line]
		    set key	  [string trim	 [string range $line 0 [expr $index - 1]]]
		    set value [string trim [string range $line [expr $index +1] [expr $len -1]]]
		    
		    set ind [ lsearch  -exact $glmextranamelist $key ]
		    if { $ind >= 0 } {
			set parameters($this,glmextra_$key) $value 
		    } else {
			pxtkprint "Bad parameter \"$key\" specified in setup file. Ignored!\n"
			pxtkconsole
		    }
		}
	    } 
	}
    }
#----------------------------------------------------------------------------------------------------  
itcl::body bis_fmrisetup::LoadOlderSetupFile { loadname } {
	 
	if { $loadname == "" } {
	return	0
	}

	$this ClearAllData 0

	set fileid [open $loadname r ]

	set setupname $loadname

	seek $fileid 0 start
	
	gets $fileid numruns 
	gets $fileid numtasks
	gets $fileid parameters($this,num_outcomes)
	gets $fileid parameters($this,study_title)
	gets $fileid parameters($this,subject_id)
	gets $fileid parameters($this,reference_brain_path)
	set parameters($this,reference_brain) $parameters($this,reference_brain_path)
	set parameters($this,display_reference_brain) [file tail $parameters($this,reference_brain_path)]
	gets $fileid parameters($this,conventional)
	gets $fileid parameters($this,anatomical)
	gets $fileid parameters($this,referenceTransform)
	gets $fileid parameters($this,internalTransform)
	gets $fileid parameters($this,distortionTransform)
	gets $fileid parameters($this,session_id)
	gets $fileid parameters($this,session_description)
	gets $fileid parameters($this,tr)
	gets $fileid parameters($this,numslices)
	gets $fileid parameters($this,numframes)
	gets $fileid parameters($this,checks)
	gets $fileid parameters($this,skipFrames)
	gets $fileid parameters($this,xml_output_file)
	gets $fileid parameters($this,xml_output_dir)
	gets $fileid parameters($this,output_dir)
	
	set parameters($this,display_referenceTransform) [file tail $parameters($this,referenceTransform)]
	set parameters($this,display_internalTransform) [file tail $parameters($this,internalTransform)]
	set parameters($this,display_distortionTransform) [file tail $parameters($this,distortionTransform)]
	
	set parameters($this,display_conventional) [file tail $parameters($this,conventional)]
	set parameters($this,display_anatomical) [file tail $parameters($this,anatomical)]
	
	
	#    if {$parameters($this,checks)==1} {
	#	$skipframesentry config -state normal -textbackground white
	#    }
	
	for {set runcount 1} {$runcount <= $numruns} {incr runcount} {
		#$runslistbox insert end "Run_$runcount"
		gets $fileid parameters($this,run_file_$runcount)
	set parameters($this,display_run_file_$runcount) [file tail $parameters($this,run_file_$runcount)]
	gets $fileid parameters($this,runskip_$runcount)
	gets $fileid parameters($this,skiprunsframe_$runcount)

	}
	
	for {set taskcount 1} {$taskcount <= $numtasks} {incr taskcount} {
		gets $fileid parameters($this,task_$taskcount)
		#$tasklistbox insert end "T$taskcount-$parameters($this,task_$taskcount)"

	}
	
	set numblocks [expr $numtasks*$numruns]
	
	for {set runcount 1} {$runcount <= $numruns} {incr runcount} {
	    for {set taskcount 1} {$taskcount <= $numtasks} {incr taskcount} {
		gets $fileid parameters($this,tframe_${taskcount}_${runcount})
	    }
	}
	
	for {set outcount 1} {$outcount <= $parameters($this,num_outcomes)} {incr outcount} {
		gets $fileid parameters($this,outcome_$outcount)
	}
	
	#   puts $numruns
	#    puts $numtasks
	close $fileid
	$this UpdateGUI 
	return 2
}

#----------------------------------------------------------------------------------------------------  
	



#----------------------------------------------------------------------------------------------------	   
itcl::body bis_fmrisetup::ClearAllArrays {par { doask 0 }} {

    if { $doask == 1 } {
	
	set ok [ tk_messageBox -type yesno -default no -title "WARNING!" -message "Are you sure you want to clear all $par arrays" ]
	if { $ok == "no" } {
	    return
	}
    }
    
    switch -exact $par {
	"tframe" {
	    for {set taskcount 1} {$taskcount<=$numtasks} {incr taskcount} {
		for {set runcount 1} {$runcount<=$numruns} {incr runcount} {
		    set parameters($this,tframe_${taskcount}_${runcount}) ""
		}
	    }
	} "runs" {
	    for {set c3 1} {$c3<=$numruns} {incr c3} {
		set parameters($this,run_file_$c3) ""
		set parameters($this,run_file_${c3}_length) 0
	    }
	} "task" {
		for {set c4 1} {$c4<=$numtasks} {incr c4} {
		set parameters($this,task_$c4) ""
		}
	} "outcome" {
	    for {set c5 1} {$c5<=$parameters($this,num_outcomes)} {incr c5} {
		set parameters($this,outcome_$c5) ""
	    }
	}
	
    }
}


#Prints all information to file
#Called in CreateGui
#Invoked when user presses "Create .xml file!" button in main window
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::WriteXMLFile { { outdir 0 } } {
	global tcl_platform

	set imagepath $::pxtclvtkpxcontrib::imagedir
  #  puts stdout "Imagepath = $imagepath"

	if { $outdir !=0 } {
	set parameters($this,xml_output_dir) $outdir
	}

	set ind [ lsearch -exact $referenceimagenames $parameters($this,display_reference_brain) ]
	if { $ind >=0 } {
	set parameters($this,reference_brain_path) [ lindex $referenceimagepaths $ind ] 
	} else { 
	set parameters($this,reference_brain_path) $parameters($this,reference_brain) 
	}   
		   
	set outname [ file join $parameters($this,xml_output_dir) $parameters($this,xml_output_file) ]
	set present [file exists $outname.xml]

	if { $present==1 } {
	set ok [ tk_messageBox -type yesno -default no -title "WARNING!" -message "The file $outname.xml already exists. Overwrite?" -icon question ]
	if { $ok=="no" } {
		return
	}
	} 

	set fileid [open $outname.xml w+ ]
	
	set dtdpath [ file normalize [	file join [ file join [ file dirname [ info script ]] .. ] vtknr ] ]
	set dtdfile [ file join $dtdpath fmri.dtd ]
	
	
	puts $fileid "<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\" ?>"
	
	
	
	puts $fileid "<!DOCTYPE setup SYSTEM \"$dtdfile\">"
	puts $fileid "<!-- xml begins here -->"
	puts $fileid " <setup>" 
	puts $fileid "	 <name> fMriGlm </name>"
	puts $fileid "	 <description> $parameters($this,study_title) </description>"
	puts $fileid "	 <fmri>"
	puts $fileid "	   <process id=\"fMriEventGlm\">"
	puts $fileid "	     <name> Event Related GLM Analysis </name> "
	puts $fileid "	     <description> Event Related GLM Analysis </description>"
	puts $fileid "	   </process>"
	puts $fileid "	   <referenceImage>"
	puts $fileid "	     $parameters($this,reference_brain_path)"
	puts $fileid "	   </referenceImage>"
	
	for {set taskprintcount 1} {$taskprintcount <= $numtasks} {incr taskprintcount} {
	puts $fileid "	   <task id=\"$parameters($this,task_$taskprintcount)\">"
	puts $fileid "	     <name> $parameters($this,task_$taskprintcount) </name>"
	puts $fileid "	     <description> $parameters($this,task_$taskprintcount) </description>"
	puts $fileid "	   </task>"
	}
	
	puts $fileid "	   <subject id=\"$parameters($this,subject_id)\">"
	puts $fileid "	      <name> $parameters($this,subject_id) </name>"
	puts $fileid "	      <description> Subject $parameters($this,subject_id) </description>"
	
	puts $fileid "	      <conventionalImage>"
	puts $fileid "	       $parameters($this,conventional)"
	puts $fileid "	      </conventionalImage>"
	puts $fileid "	      <anatomicalImage>"
	puts $fileid "		$parameters($this,anatomical)"
	puts $fileid "	      </anatomicalImage>"
	puts $fileid "	      <referenceTransform>"
	puts $fileid "	       $parameters($this,referenceTransform)"
	puts $fileid "	      </referenceTransform>"
	puts $fileid "	      <internalTransform>"
	puts $fileid "	       $parameters($this,internalTransform)"
	puts $fileid "	      </internalTransform>"
	puts $fileid "	      <distortionTransform>"
	puts $fileid "	       $parameters($this,distortionTransform)"
	puts $fileid "	      </distortionTransform>"
	
	puts $fileid "	      <session id=\"$parameters($this,session_id)\">"
	puts $fileid "		 <description> $parameters($this,session_description) </description>"
	puts $fileid "		 <tshift/>"
	if {$parameters($this,checks)==1} {
	puts $fileid "		 <skipFrames> $parameters($this,skipFrames) </skipFrames>"
	}
	puts $fileid "		 <TR>$parameters($this,tr)</TR>"
	puts $fileid "		 <numSlices> $parameters($this,numslices) </numSlices>"
	puts $fileid "		 <numFrames> $parameters($this,numframes) </numFrames>"
	
	for {set runcount2 1} {$runcount2 <= $numruns} {incr runcount2} {
	puts $fileid "	      <run id=\"r$runcount2\">"
	puts $fileid "		 <filename>"
	puts $fileid "		  $parameters($this,run_file_$runcount2)"
	puts $fileid "		 </filename>"
	if {$parameters($this,runskip_$runcount2)==1} {
		puts $fileid "		  <skipFrames> $parameters($this,skiprunsframe_$runcount2) </skipFrames>"
	}
	for {set taskcount2 1} {$taskcount2 <= $numtasks} {incr taskcount2} {
		set t t$taskcount2
		set r r$runcount2
		set tr "$t\_$r"
		puts $fileid "	      <task_block_id> $tr </task_block_id>"
	}
	puts $fileid "	      </run>"
	}
	
	set blockcount3 1

	set tfname "tFrames"
	if { $parameters($this,blockunits) == 1 } {
	set tfname "timeFrames"
	}


    for {set runcount3 1} {$runcount3 <= $numruns} {incr runcount3} {
	for {set taskcount3 1} {$taskcount3 <= $numtasks} {incr taskcount3} {
	    set t t$taskcount3
	    set r r$runcount3
	    set tr "$t\_$r"
	    puts $fileid "		<block id=\"$tr\">"
	    puts $fileid "		  <task_id> $parameters($this,task_$taskcount3) </task_id>"
	    puts $fileid "		  <activation>"
	    puts $fileid "		     <${tfname}>$parameters($this,tframe_${taskcount3}_${runcount3})</${tfname}>"
	    puts $fileid "		     <bValue> 1.0 </bValue>"
	    puts $fileid "		  </activation>"
	    puts $fileid "		 </block>"
	    set blockcount3 [expr $blockcount3+1]
	}    
    }
    
    puts $fileid "		</session>"
    
	for {set taskcount4 1} {$taskcount4 <= $parameters($this,num_outcomes)} {incr taskcount4} {
	puts $fileid "		<outcome> $parameters($this,outcome_$taskcount4) </outcome>"
	}
	
	puts $fileid "		<outputDirectory>"
	puts $fileid "		    $parameters($this,output_dir)"
	puts $fileid "		</outputDirectory>"
	puts $fileid "	     </subject>"
	puts $fileid "	   </fmri>"
	puts $fileid "	</setup>"
	
	close $fileid
	 
	$this InfoMessage "The xml file was saved as $outname.xml "  "File Saved"
	return 1
}

#Reads old style Rajeevan XML File -- complement to above
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::ReadXMLFile { fname { subjectno 1 } { onlyrun -1 } } {

    
    if { $onlyrun < 1 } {
	set onlyrun 0
    }

    #    set onlyrun 1

    set parse [ vtkXMLDataParser New ]
    $parse SetFileName $fname
    $parse SetIgnoreCharacterData 0
    $parse Parse

    set rootnode [ $parse GetRootElement ]
    if { $rootnode == "" } {
	$parse Delete
	return 0
    }

    set nm [ $rootnode GetName ]
    if { $nm != "setup" } {
	puts stderr "BAD XML File $nm"
	$parse Delete
	return 0
    }

    puts stdout "Nm=$nm"

    $this ClearAllData 0

    set fmnode [ $rootnode LookupElementWithName "fmri" ]
    puts stdout "fm=[ $fmnode GetName ]"

    set tmp [ $fmnode LookupElementWithName "referenceImage" ]
    set v [ string trim [ $tmp GetCharacterData ] ]
    puts stdout "v=$v"
    set parameters($this,reference_brain_path) [ file normalize $v ]
    

    # List all tasks
    set numtasks 0
    set xmltaskidlist ""
    for { set j 0 } { $j < [ $fmnode GetNumberOfNestedElements ] } { incr j } {
	set c [ $fmnode GetNestedElement $j ]
	if { [ $c GetName ] == "task" } {
	    incr numtasks
	    lappend xmltaskidlist  [ string trim [ $c GetAttribute id ] ]
	    set tmp [ $c LookupElementWithName "name" ]
	    set parameters($this,task_$numtasks) [ string trim [ $tmp GetCharacterData ] ]
	    puts stdout "Adding task $parameters($this,task_$numtasks) with id  [ string trim [ $c GetAttribute id ] ] "
	}
    }

    puts stdout "Added $numtasks tasks"

    set count 1
    set subject 0
    for { set j 0 } { $j < [ $fmnode GetNumberOfNestedElements ] } { incr j } {
	set c [ $fmnode GetNestedElement $j ]
	if { [ $c GetName  ] == "subject" } {
	    if { $count == $subjectno } {
		set subject $c
		set j  [ $fmnode GetNumberOfNestedElements ]
	    } else {
		incr count
	    }
	}
    }

    set session 0
    
    if { $subject == 0 || $numtasks == 0 } {
	puts stderr "Bad Setup File\n"
    } else {
	set parameters($this,subject_id) [ string trim [ $subject GetAttribute id ] ]
	puts stdout "Subject = $parameters($this,subject_id)"
	
	set chld [ $subject LookupElementWithName name ]
	set a1 [ string trim [ $chld GetCharacterData ] ]
	set chld [ $subject LookupElementWithName description ]
	set a2 [ string trim [ $chld GetCharacterData ] ]
	set parameters($this,study_title) "$a1,$a2"

	set lst1 [ list conventionalImage anatomicalImage  referenceTransform internalTransform distortionTransform skipFrames ]
	set lst2 [ list conventional      anatomical       referenceTransform internalTransform distortionTransform skipFrames ]
	
	for { set j 0 } { $j < [ $subject GetNumberOfNestedElements ] } { incr j } {
	    set e [ $subject GetNestedElement $j ]
	    set n [ $e GetName ]
	    set ind [ lsearch -exact $lst1 $n ]
	    if {   $ind > -1 } {
		set internal [ lindex $lst2 $ind ]
		set parameters($this,$internal) [ string trim [ $e GetCharacterData ] ]
		puts stdout "Setting $internal to $parameters($this,$internal)"
	    } elseif { $n == "session" } {
		set session $e 
		puts stdout "Setting Session to $n"
	    }
	
	    set parameters($this,reference_brain) $parameters($this,reference_brain_path)
	    set parameters($this,display_reference_brain) [file tail $parameters($this,reference_brain_path)]
	    set parameters($this,display_reference_gw_map) [file tail $parameters($this,reference_gw_map)]
	    set parameters($this,display_referenceTransform) [file tail $parameters($this,referenceTransform)]
	    set parameters($this,display_internalTransform) [file tail $parameters($this,internalTransform)]
	    set parameters($this,display_distortionTransform) [file tail $parameters($this,distortionTransform)]
	    set parameters($this,display_conventional) [file tail $parameters($this,conventional)]
	    set parameters($this,display_anatomical) [file tail $parameters($this,anatomical)]

	    if { [ string length $parameters($this,skipFrames)  ] > 1 } {
		set parameters($this,checks) 1
	    } else {
		set parameters($this,checks) 0
	    }
    	}
    }

    set runlst ""
    set blocklst ""
    

    if { $session !=0  } {
	# Parse session stuff now
	set parameters($this,session_id) [ string trim [ $session GetAttribute id ] ]
	set de [ $session LookupElementWithName description ]
	set parameters($this,session_description) [ string trim  [ $de GetCharacterData ] ]
	puts stdout "Desc = $parameters($this,session_description)"

	set lst1 [ list TR numSlices numFrames ]
	set lst2 [ list tr numslices numframes ]
	
	for { set j 0 } { $j < [ $session GetNumberOfNestedElements ] } { incr j } {
	    set e [ $session GetNestedElement $j ]
	    set n [ $e GetName ]
	    set ind [ lsearch -exact $lst1 $n ]
	    if {   $ind > -1 } {
		set internal [ lindex $lst2 $ind ]
		set parameters($this,$internal) [ string trim [ $e GetCharacterData ] ]
		puts stdout "Setting $internal to $parameters($this,$internal)"
	    } elseif { $n == "run" } {
		lappend runlst $e
		set xmlrunid([llength $runlst ]) [ string trim [ $e GetAttribute id ] ]
	    } elseif { $n == "block" } {
		lappend blocklst $e
		set xmlblockid([llength $blocklst ]) [ string trim [ $e GetAttribute id ] ]
	    }
	}
	puts stdout "Found [ llength $runlst ] runs and [ llength $blocklst ] blocks" 
    }

    #set xmlblocklist 
    set numruns 0
    for { set i 0 } { $i < [ llength $runlst ] } { incr i } {
	incr numruns
	set xmlblocklist($numruns) ""
	puts stdout "Run ID =  $xmlrunid([ expr $i+1 ])"
	set rel [ lindex $runlst $i ]
	for { set j 0 } { $j < [ $rel GetNumberOfNestedElements ] } { incr j } {
	    set e [ $rel GetNestedElement $j ]
	    set n [ $e GetName ]
	    if { $n == "filename" } {
		set parameters($this,run_file_$numruns) [ string trim [ $e GetCharacterData ] ]
		set parameters($this,display_run_file_$numruns) [file tail $parameters($this,run_file_$numruns)]
		set parameters($this,display_run_motionfile_$numruns) ""
		set parameters($this,run_motionfile_$numruns) ""
		set parameters($this,runskip_$numruns) 0
		set parameters($this,skiprunsframe_$numruns) ""
		for { set k 1 } { $k <= $numtasks } { incr k } {
		    set parameters($this,tframe_${k}_${numruns}) ""
		    puts stdout "setting parameters($this,tframe_${k}_${numruns}) to blank"
		}
		puts stdout "Adding run filename ($numruns) = $parameters($this,run_file_$numruns)"
	    } elseif { $n == "task_block_id" } {
		lappend xmlblocklist($numruns) [ string trim [ $e GetCharacterData ] ]
	    } elseif { $n ==  "skipFrames" } {
		set v [ string trim [ $e GetCharacterData ] ]
		if { [ string length $v ] < 1 } {
		    set parameters($this,runskip_$numruns) 0
		    set parameters($this,skiprunsframe_$numruns) ""
		} else {
		    set parameters($this,runskip_$numruns) 1
		    set parameters($this,skiprunsframe_$numruns) $v
		}
	    }
	}
    }
    puts stdout "\n\n"
    for { set i 1 } { $i <= $numruns } { incr i } {
	puts stdout "Blocklist for run $i = $xmlblocklist($i) "
    }

    puts stdout "\n\n"
    for { set i 0 } { $i < [ llength $blocklst ] } { incr i } {
	set blk [ lindex $blocklst $i ]
	set bid [ string trim [ $blk GetAttribute id ]]
	set whichrun 0
	set count 1
	
	#	puts stdout "Block id = $bid"

	while { $count <= $numruns && $whichrun == 0 } {
	    set ind [ lsearch -exact $xmlblocklist($count) $bid ]
	    if { $ind > -1 } {
		set whichrun $count
	    } else { 
		incr count
	    }
	    #   puts stdout "Looking for $bid in $xmlblocklist($count) (count=$count/ $numruns) ind=$ind (whichrun=$whichrun)"
	}

	if { $whichrun > 0 } {
	    set tmp [ $blk LookupElementWithName "task_id" ]
	    set tsk [ string trim [ $tmp GetCharacterData ] ]
	    set ind [ lsearch -exact $xmltaskidlist $tsk ]
	    if { $ind > -1 } {
		set taskno [ expr $ind +1 ]
		puts stdout "Adding $bid to task $taskno ($tsk), Run=$whichrun"

		set act [ $blk LookupElementWithName "activation" ]
		set tfr [ $act LookupElementWithName "tFrames" ]
		set tfr2 [ $act LookupElementWithName "timeFrames" ]
		set val ""
		if { $tfr != "" } {
		    set parameters($this,blockunits) 1
		    set val [ string trim [ $tfr GetCharacterData ] ]
		} elseif { $tfr2 != "" } {
		    set parameters($this,blockunits) 0
		    set val [ string trim [ $tfr2 GetCharacterData ] ]
		}
		set parameters($this,tframe_${taskno}_${whichrun}) $val
		puts stdout "Setting parameters($this,tframe_${taskno}_${whichrun}) $val\n"
	    } else {
		puts "Bad Run $whichrun  block with id $bid (i.e. $tsk not found in $xmltaskidlist, ind=$ind)\n"
	    }
	} else {
	    puts stdout "Bad Block $bid\n"
	}
    }

    set parameters($this,afni_hrfmode) triplegamma

    $parse Delete

    if { $onlyrun >=1 && $onlyrun <= $numruns } {
	if { $onlyrun != 1 } {
	    set parameters($this,run_file_1) $parameters($this,run_file_$whichrun)
	    set parameters($this,display_run_file_1) $parameters($this,display_run_file_$whichrun)
	    set parameters($this,run_motionfile_1) $parameters($this,run_motionfile_$whichrun)
	    set parameters($this,display_run_motionfile_1) $parameters($this,display_run_motionfile_$whichrun)
	}
	for {set runcount 2 } {$runcount<$numruns} {incr runcount} {
	    set parameters($this,run_file_${runcount}) ""
	    set parameters($this,display_run_file_${runcount})  ""
	    set parameters($this,run_motionfile_${runcount}) ""       
	    set parameters($this,display_run_motionfile_${runcount}) ""
	    for {set taskcount 1} {$taskcount<=$numtasks} {incr taskcount} {
		set parameters($this,tframe_${taskcount}_$runcount) ""
	    }
	}
	set numruns 1
    }
    set ok [ $this UpdateGUI ]
    $this SaveSetupFile "test.xmlg"
    exit
    return $ok
}


#----------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::ParseFramesString { val usingseconds tr numframes offset message  { debug 0 }} {
	
	regsub -all ";" $val "" val2
	regsub -all " +" $val2 " " val3
	set val3 [ string trim $val3 ]
	set n [ llength $val3 ]
	set m [ expr 2*(int($n/2)) - $n ]

	set plist ""

	if { $debug > 0 } {
	pxtkprint "+++++ Cleaned up  val=$val3 , number = $n (isodd = $m)\n"
	}

	if { $m != 0 } {
	$this InfoMessage "Bad Definition for $message, $n total elements ($val3), which is not an even number"	 "Error ..."  error
	return 0
	}
	set half [ expr int($n/2) ]
	for { set i 0 } { $i < $half } { incr i } {
	
	set ind(0) [ expr $i *2 ]
	set ind(1) [ expr $ind(0) +1 ]
	
	if { $debug > 1 } {
		pxtkprint "\t\t\t Checking Pair [ expr $i+1 ]/$half  $ind(0):$ind(1)"
	}
	
	
	for { set k 0 } { $k <= 1 } { incr k } {
		set op($k)   [ lindex $val3 $ind($k) ]
		if { $usingseconds > 0	} {
		set p($k) [ expr int($op($k)/$tr) ]
		} else {
		set p($k) [ expr int($op($k)) ]
		}
		
		if { $debug > 1 } {
		pxtkprint "p($k) = $op($k) sec --> $p($k) fr, "
		}
		
		
		if { $p($k) < 0 || $p($k) >= $numframes } {
		$this InfoMessage "Out of bounds Block Definition for $message, blockpart = [ expr $i+1 ] ( $op(0):$op(1))" "Error ..." error
		return 0
		}
		
		set p($k) [ expr $p($k) + $offset  ]
		if { $debug > 1 } {
		pxtkprint "--> + $offset = $p($k) "
		}
	}
	if { $debug > 0 } {
		pxtkprint "\n"
	}
	lappend plist [ list $p(0) $p(1) ]
	}
	return $plist

}

#----------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::CreateBaseGLMMatrix { { debug 0 } } {

    # If we are not specifying an external matrix then we are fine
    if { $parameters($this,glmextra_useexternalmatrix)  == 0 } {
	puts stdout "Creating Matrix From Block Definitions"
	return [ $this ValidateBlockDefinitions 1 $debug ]
    }

    set lst [ $this ComputeEmptyGLMMatrixDimensions $debug ]
    if { [ lindex $lst 0 ] == 0 || [ lindex $lst 1 ] == 0 } {
	return 0
    }
    puts stdout "Creating Empty Matrix [ lindex $lst 0 ] x [ lindex $lst 1 ]"
	
    set matrix [ vtkpxMatrix [ pxvtable::vnewobj ]]
	
    set ok [ $matrix Load $parameters($this,glmextra_externalmatrixfilename) ]
	
    if { $ok == 0 } {
	# Error message about bad matrix
	$this InfoMessage "Please provide matrix. Calculation is terminated" "Error ..." error
	$matrix Delete
	return 0
    }

    set sz [ $matrix GetSize ]
	
    if { [ lindex $sz 0 ] != [ lindex $lst 0 ] || [ lindex $sz 1 ] != [ lindex $lst 1 ] } {
	# Error message bad matrix dimensions should be $lst
	$this InfoMessage "Bad matrix dimensions. Should be [ lindex $lst 0 ] x [ lindex $lst 1 ]. Calculation is terminated. " "Error ..." error
	$matrix Delete
	return 0
    }

    puts stdout "Using External Matrix from $parameters($this,glmextra_externalmatrixfilename), dims = [ $matrix GetSize ]"

    return $matrix
}
# --------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::ComputeEmptyGLMMatrixDimensions { { debug 0 } } {

    set totalframes 0
    
    if { $debug > 0 } {
	pxtkconsole
    }
    
    # First Read Headers and Compute Total Number Of Frames
    for { set runcount 1 } { $runcount <= $numruns } { incr runcount } {
	set rname $parameters($this,run_file_$runcount)
	set hd [ vtkbisImageHeader [ pxvtable::vnewobj ]]
	set ok [ $hd ReadHeader $rname ]
	if { $ok == 0 } {
	    $this InfoMessage "Bad run file run=$runcount ($name)" "Error ..." error
	    return [ list 0 0 ]
	}
	set numframes [ $hd  GetNumberOfComponents ]
		puts $numframes
	
	if { $numframes < 2 } { 
	    $this InfoMessage "Bad run file run=$runcount (numframes=$numframes)" "Error ..." error
	    return [ list 0 0 ]
	}
	set parameters($this,run_file_${runcount}_length)   $numframes
	pxtkprint "Header $rname, frames = $numframes\n"
	$hd Delete
	set totalframes [ expr $totalframes + $numframes ]
    }
	
	
    if { $debug } {
	pxtkprint "Total Frames = $numframes\n "
    }
    
    return [ list $totalframes $numtasks ]
}

itcl::body bis_fmrisetup::ValidateBlockDefinitions { { creatematrix 0 } { debug 0 } } {

    
    set lst [ $this ComputeEmptyGLMMatrixDimensions $debug ]
    if { [ lindex $lst 0 ] == 0 } {
	return 0
    }

    set totalframes [ lindex $lst 0 ]
    set matrix 1
    if { $creatematrix != 0 } {
	set matrix [ vtkpxMatrix [ pxvtable::vnewobj ]]
	$matrix Zeros $totalframes $numtasks
    }

    set offset 0
    for {set runcount 1} {$runcount<=$numruns} {incr runcount} {
	set numframes $parameters($this,run_file_${runcount}_length)   
	if { $debug > 0 } {
	    pxtkprint "\nChecking Table for run = $runcount/ $numruns, numframes=$numframes\n-----------------------------------------------------------------\n\n"
	}
	
	for {set taskcount 1} {$taskcount<=$numtasks} {incr taskcount} {
	    set  val $parameters($this,tframe_${taskcount}_${runcount})
	    
	    if { $debug > 0 } {
		pxtkprint "Checking Table for run = $runcount/ $numruns, task = $taskcount/ $numtasks , val = $val\n"
	    }
	    
	    set plist [ $this ParseFramesString $val $parameters($this,blockunits) $parameters($this,tr) $numframes $offset \
			    "run=$runcount, task=$taskcount" $debug ]
	    
	    if { $plist == 0 } {
		catch { $matrix Delete }
		return 0
	    }
	    
	    for { set k 0 } { $k < [ llength $plist ] } { incr k } {
		set p(0) [ lindex [ lindex  $plist $k ]	 0 ]
		set p(1) [ lindex [ lindex  $plist $k ]	 1 ]
		set tskindex [ expr $taskcount -1 ]
		if { $debug > 1 } {
		    pxtkprint "....\t\t\t\t now filling with 1 between  $p(0):$p(1)\n"
		}
		if { $creatematrix > 0 } {
		    for { set j $p(0) } { $j <= $p(1) } { incr j } {
			$matrix SetElement $j $tskindex 1
		    }
		} 
	    }
	}
	
	if { $parameters($this,runskip_$runcount) == 1	|| $parameters($this,checks) > 0 } {
	    set val "$parameters($this,skipFrames) $parameters($this,skiprunsframe_$runcount)"
	    if { $debug > 0 } {
		pxtkprint "Checking Table for Skipframes run = $runcount/ $numruns, skipframes= $val\n"
	    }
	    
	    set plist [ $this ParseFramesString $val  0 1 $numframes $offset \
			    "run=$runcount, skipframes" $debug ]
	    
	    if { $plist == 0 } {
		catch { $matrix Delete }
		return 0
	    }
	    
	    for { set k 0 } { $k < [ llength $plist ] } { incr k } {
		set p(0) [ lindex [ lindex  $plist $k ]	 0 ]
		set p(1) [ lindex [ lindex  $plist $k ]	 1 ]
		set tskindex [ expr $taskcount -1 ]
		if { $debug > 1 } {
		    pxtkprint "....\t\t now filling with zeros all columns Between	$p(0):$p(1)\n"
		}
		if { $creatematrix > 0 } {
		    puts stdout "Zeroing rows (0-offset) $p(0):$p(1)"
		    for { set j $p(0) } { $j <= $p(1) } { incr j } {
			for { set ja 0 } { $ja < $numtasks } { incr ja } {
			    $matrix SetElement $j $ja 0
			}
		    }
		}
	    }
	}
	set offset [ expr $offset + $numframes ]
    }
    return $matrix
}
#----------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::AFNICreateTimeFile { { tname "" } { report 0 } } {

	if { $tname == "" } {
	set tname [ file join $parameters($this,output_dir) afni_timepoints ]
	}

	set stimname "[ file root $tname ]_startpoints.txt"
	set totalframes [ expr $numruns * $parameters($this,numframes) ]
	set matrix [ $this CreateBaseGLMMatrix 0 ]
	if { $matrix == 0 } {
	return "none"
	}

	set fout2 [ open $stimname w ]
	set index 0
	for { set runcount 1 } { $runcount <= $numruns } { incr runcount } {
	set numframes $parameters($this,run_file_${runcount}_length)   
	set tmpname ${tname}_${runcount}.txt
	set fout [ open $tmpname w ]
	#	puts stdout "run = $runcount/$numruns Index = $index, file = $tmpname, numframes=$numframes "
	puts $fout2 $index 
	for { set frame 1 } { $frame <= $numframes } { incr frame } {
		for {set taskcount 0} {$taskcount < [ expr $numtasks - 1 ] } {incr taskcount} {
		puts -nonewline $fout "[ $matrix GetElement $index $taskcount ]\t"
		}
		puts $fout "[ $matrix GetElement $index	 [ expr $numtasks -1 ] ]"
		incr index
	}
	close $fout
	}

	close $fout2

	$this InfoMessage "Saved timepoint files in [ file tail $tname ] and \n [ file tail $stimname ]\n in directory: [ file dirname $stimname ]"  "Block Design Files Saved"	 info

	return [ list $tname $stimname ]
}



# ----------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::SetProcessingDefaults {  } {

	set processlist ""
	set processnamelist ""

	lappend processlist { "#Ensure Inputs are NIFTI"  ensurenifti 0 }
	lappend processlist { "#Delete Temporary Files"	 deletetemp 1 }
	lappend processlist { "#Mask Level (% of max intensity)"  masklevel 0.05 }
	lappend processlist { "#HRF Mode -- one of wav, gamma, doublegamma, triplegamma spm" hrfmode wav }
	lappend processlist { "#Use AFNI Waver; if 0 use internal code which is cleaner/faster etc" useafniwaver 0 }
	lappend processlist { "#Use AFNI Commands for intensity normalization; if 0 use internal code which is cleaner/faster etc" useafninormalize 0 }
	lappend processlist { "#GLM Delaytime"	delaytime 2.0 }
	lappend processlist { "#GLM Risetime"  risetime 4.0 } 
	lappend processlist { "#GLM Falltime"  falltime 6.0 } 
	lappend processlist { "#GLM Undershoot"	 undershoot 0.2 } 
	lappend processlist { "#GLM Peak"  peak 1.0 } 
	lappend processlist { "#Polynomial Drift"  polort 3 } 
	lappend processlist { "#Output Baseline Coefficientts" bout 0 } 
	lappend processlist { "#Output F-Statistics" fout 0 } 
	lappend processlist { "#Output R^2-Statistics" rout 0 }	 
	lappend processlist { "#Output T-Statistics" tout 0 }  
	lappend processlist { "#Output Residual Time Series" saveresidual 0 }  
	lappend processlist { "#Output Drift Beta Maps" savedriftbeta 0 }  
	lappend processlist { "#Output Sample Variance Map" vout 0 }
	lappend processlist { "#GLM Single Run Output" glmsinglerunoutput 0 }
	lappend processlist { "#GLM Regress Motion Parameters" glmregressmotion 0 }

	lappend processlist { "#Rate GLM Terms" numrateterms 1 }
	lappend processlist { "#Rate GLM Global Drift" rateglobal 0 }
	

	for { set i 0 } { $i < [ llength $processlist ] } { incr i } {
	set elem [ lindex $processlist $i ]
	set name [ lindex $elem 1 ]
	set val	 [ lindex $elem 2 ]
	lappend processnamelist $name
	set parameters($this,afni_$name) $val
	}

	set restinglist ""
	set restingnamelist ""

	lappend restinglist { "#Mask Level (% of max intensity)"  masklevel 0.05 }
	lappend restinglist { "#Polynomial Drift"  driftpol 3 } 
	lappend restinglist { "# Slice Mean Removal "  slicemeanremoval 1 }
	lappend restinglist { "# Volume Mean Removal "	volumemeanremoval 0 }
	lappend restinglist { "#Raw Correlation"  rawcorrelation 0 } 
	lappend restinglist { "#Z Transform"  ztransform 0 }
	lappend restinglist { "#Process Runs Individually"  individualruns 1 } 
	lappend restinglist { "#Temporal Smoothing Sigma (Frames)"  temporalsigma 0.0 } 
	lappend restinglist { "#Intrinsic Connectivity Threshold"  threshold 0.0 } 
	lappend restinglist { "#Intrinsic Connectivity Range"  range "Both" } 
	lappend restinglist { "#Intrinsic Connectivity Mode"  mode "Sqr" } 
	lappend restinglist { "#Use VOI Image as mask For Intrinsic Connectivity"  usermask 1 } 
	lappend restinglist { "#Preprocess (Eliminate Motion Parameters)"  usemotionparams 0 } 
	lappend restinglist { "#Preprocess (Eliminate CSF/White Matter Mean)"  usecsfmean 0 } 
	

	for { set i 0 } { $i < [ llength $restinglist ] } { incr i } {
	set elem [ lindex $restinglist $i ]
	set name [ lindex $elem 1 ]
	set val	 [ lindex $elem 2 ]
	lappend restingnamelist $name
	set parameters($this,resting_$name) $val
	}


	set glmextralist ""
	set glmextranamelist ""

	lappend glmextralist { "#Use External Matrix File"   useexternalmatrix 0 }
	lappend glmextralist { "#External Matrix FileName"   externalmatrixfilename "" }
	lappend glmextralist { "#Do Convolution with HRF"    doconvolution 1 }

	for { set i 0 } { $i < [ llength $glmextralist ] } { incr i } {
	    set elem [ lindex $glmextralist $i ]
	    set name [ lindex $elem 1 ]
	    set val	 [ lindex $elem 2 ]
	    lappend glmextranamelist $name
	    set parameters($this,glmextra_$name) $val
	}


	
}

itcl::body bis_fmrisetup::AFNICreateScriptHelper { fout line { mirror 0 } } {
	
	if { $mirror == 1 } {
	regsub -all "\t" $line " " line2
	regsub -all "\n" $line2 " " line2
	regsub -all " +" $line2 " " line2
	regsub -all "\\\\" $line2 " " line2
	
	puts $fout "echo \"\""
	puts $fout "echo \"Executing:  $line2\" "
	puts $fout "echo \"\" "
	puts $fout "$line"
	} else {
	if { $line == "" } {
		puts $fout "\necho \"\""
		puts $fout "echo \"\"\n"
	} else {
		puts $fout "echo \"$line\""
	}
	}
}

itcl::body bis_fmrisetup::EnsureRunsAreNIFTI { } {

	for { set runcount 1 } { $runcount <= $numruns } { incr runcount } {
	set rname $parameters($this,run_file_$runcount)

	set hd [ vtkbisImageHeader [ pxvtable::vnewobj ]]
	$hd ReadHeader $rname
	if { [ $hd GetMode ] == 0 } {
		if { [ file writable $rname ] == 1  && [ file extension $rname ] == ".hdr" } {
		$hd SetModeToNIFTI_HDRIMG
		$hd WriteHeader $rname 0
		pxtkprint "Converted headerfile $rname to NIFTI\n"
		} else {
		$this InfoMessage  "Failed to convert headerfile $hd to NIFTI, either read-only or not a .hdr file" "Error ..." error
		}
		$hd Delete
	}
	}
}

# ---------------------------------------------------------------------------------------------------

itcl::body bis_fmrisetup::CreateWaverMatrix { } {

    set matrix [ $this CreateBaseGLMMatrix  0 ]
	if { $matrix == 0 } {
	return 0
	}
	set flist ""
	
	set breaks  [ vtkIntArray [ pxvtable::vnewobj ]]
	$breaks SetNumberOfComponents 1
	$breaks SetNumberOfTuples $numruns

	$breaks SetComponent 0 0 0.0
	for { set runcount 1 } { $runcount < $numruns } { incr runcount } {
	    set fr $parameters($this,run_file_${runcount}_length)	
	    $breaks SetComponent $runcount 0 [ expr $fr + [ $breaks GetComponent [ expr $runcount -1 ] 0 ]]
	}

	set biswaver [ vtkbisWaver [ pxvtable::vnewobj ]]

	if { $parameters($this,glmextra_useexternalmatrix) == 1 && $parameters($this,glmextra_doconvolution) == 0 } {
	    $biswaver SetWaveform_type 0
	} else {
	    switch -exact $parameters($this,afni_hrfmode) {
		"wav"  {
		    $biswaver SetWAV_delay_time  $parameters($this,afni_delaytime) 
		    $biswaver SetWAV_rise_time $parameters($this,afni_risetime)
		    $biswaver SetWAV_fall_time $parameters($this,afni_falltime) 
		    $biswaver SetWAV_undershoot $parameters($this,afni_undershoot)
		    $biswaver SetWaveform_type 1
		}
		"gamma"	       {    $biswaver SetWaveform_type 2 }
		"doublegamma"  {    $biswaver SetWaveform_type 3 }
		"triplegamma"  {    $biswaver SetWaveform_type 4 }
		"spm"	       {    $biswaver SetWaveform_type 5 }
	    }
	}
	pxtkprint "Using Waveform Type [ $biswaver GetWaveform_type]\n"
	#pxtkconsole
	$biswaver SetTR	  $parameters($this,tr)
	$biswaver SetPeak $parameters($this,afni_peak) 
	
	
	set newmatr [ vtkpxMatrix [ pxvtable::vnewobj ]]
	    
	    # First the raw stuff
	$biswaver CreateStackedWaveform $matrix $newmatr $breaks
	
	set status "[$biswaver GetStatus ]"

	$biswaver Delete
	return [ list $newmatr $breaks $matrix $status ]
}

# --------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::GenerateOutputFilenames { { askfname 0 } } {

	global tcl_platform

	if { $askfname > 0 } {
	set endsuffix ".msb"
	set typelist { {"MSB File" { *.msb} }}
	
	set tryname [ file join $parameters($this,output_dir)  $parameters($this,subject_id) ]${endsuffix}
	set fname [tk_getSaveFile -title "Specify Output Filename (and implicitly path)" \
			   -filetypes $typelist -initialfile [ file tail $tryname ] -initialdir $parameters($this,output_dir) ]
	if { [ string length $fname ] == 0 } {
		return 0
	}
	set fname  [ file rootname [ file normalize $fname ] ]
	set parameters($this,output_dir) [ file dirname $fname ]
	set outnamestem [ file join $parameters($this,output_dir)  "${fname}_bis" ]
	set msbsetupname  ${fname}.msb
	} else	{
	set outnamestem [ file join $parameters($this,output_dir)  "$parameters($this,subject_id)_bis" ]
	set msbsetupname  ${outnamestem}.msb
	}

	return [ list $outnamestem $msbsetupname ]
}

# -------------------------------------------------------------------------------------------------------------

itcl::body bis_fmrisetup::CreateMSBFile { msbsetupname tasklist	  } {
	
	set fileid [ open  $msbsetupname w ]
	puts $fileid "#Multisubject Description File v3"
	set nt [ llength $tasklist ]
	puts $fileid "Tasks\n$nt"
	puts $fileid "- - - - - - - - - - - - - - - - - - - -"
	for { set i 0 } { $i < $nt } { incr i } {
	set tmp [ lindex $tasklist $i ]
	puts $fileid "Task Name	  : [ lindex $tmp 0 ]"
	puts $fileid "Task Suffix : [ lindex $tmp 1 ]"
	puts $fileid "-------------------------------------------------------"
	}
	puts $fileid "Subjects\n1"
	puts $fileid "- - - - - - - - - - - - - - - - - - - -"
	puts $fileid "Subject Id : $parameters($this,subject_id)"
	puts $fileid "Anatomical Image : $parameters($this,anatomical)"
	puts $fileid "Conventional Image : $parameters($this,conventional)"
	puts $fileid "Functional Image :   [ lindex [ lindex $tasklist 0 ] 2]"
	puts $fileid "Reference Transformation : $parameters($this,referenceTransform)"
	puts $fileid "Internal Transformation : $parameters($this,internalTransform)" 
	puts $fileid "Echoplanar Image :  "
	puts $fileid "Distortion Transformation :  $parameters($this,distortionTransform)"
	puts $fileid "Fieldmap Image : "
	puts $fileid "-------------------------------------------------------"
	
	puts $fileid "Reference/Output Images"
	puts $fileid "--------------------------------------"
	
	puts $fileid "Reference Image : $parameters($this,reference_brain_path)"
	puts $fileid "VOI Image : "

	set olist [ list "Average Anatomical" "Std Anatomical" "Average Functional" "Std Functional" "Tscore Functional" ]
	for { set i 0 } { $i < [ llength $olist ] } { incr i } {
	puts $fileid "[ lindex $olist $i ] : "
	}
	puts $fileid "Legacy : "
	puts $fileid "Legacy : "
	puts $fileid "Legacy : "
	close $fileid

	set ok [     $this CreateDataTreeStructure $tasklist ]
	if { $ok > 0 } {
	set xmlname "[ file rootname $msbsetupname].xmltree"
	    $datamanager SaveXML $xmlname
	    set datamanagerfilename $xmlname
	}
}

# --------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::DisplayMSBFile { msbsetupname } { 

	return

	# this is obsolete, we now have the tree!
	if { $multisubjectcontrol !=0 } {
	$multisubjectcontrol ShowWindow Results " "
	update idletasks
	$multisubjectcontrol LoadSetup $msbsetupname
	$multisubjectcontrol LoadAll 1
	} 
}
# --------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::PreprocessGLM { outnamestem { dodrift 1 } { numrate 0 } { singlerun 0 } } {

    
    puts stdout "Preprocess GLM singlerun=$singlerun outnamestem=$outnamestem"

    if { $parameters($this,afni_ensurenifti) == 1 } {
	$this EnsureRunsAreNIFTI
    }
    
    puts stdout "Creating Waver Matrix ................."
    set lst [ $this CreateWaverMatrix ]
    if { $lst == 0 } {
	return 0
    }
    set newmatrix  [ lindex $lst 0 ]
    set breaks     [ lindex $lst 1 ]
    set rawmatrix	  [ lindex $lst 2 ]
    
    puts stdout "done\n"

    if { $singlerun !=0 } {
	set matname ${outnamestem}_raw.matr
	$rawmatrix Save $matname
	lappend flist $matname
    }
    
    set glmmatr [ vtkpxMatrix [ pxvtable::vnewobj ]]
    set biswaver [ vtkbisWaver [ pxvtable::vnewobj ]]
    set minrun 1
    set maxrun $numruns

    if { $singlerun > 0 && $singlerun <= $numruns } {
	puts stdout "In Single Run Mode ($singlerun) .... copying matrix\n"
	set minrun $singlerun
	set maxrun $singlerun

	# Also trim matrix at this point
	set sz [ $newmatrix GetSize ]
	set minrow [ $breaks GetComponent [ expr $singlerun -1 ] 0 ]

	if { $singlerun < $numruns } {
	    set maxrow [ expr [ $breaks GetComponent $singlerun 0 ] -1 ]
	} else {
	    set maxrow [ expr [ lindex $sz 0 ] -1 ]
	}
	set minrow [ expr int($minrow) ]
	set maxrow [ expr int($maxrow) ]
	

	set tmp [ vtkpxMatrix New ]
	puts stdout "Extracting Global Matrix rows  $minrow $maxrow -- for run $singlerun"
	$tmp Copy $newmatrix $minrow $maxrow -1 -1
	$newmatrix Delete
	set newmatrix $tmp

	set  ln ""
	for { set i 0 } { $i < [ $breaks GetNumberOfTuples ] } { incr i } {
	    append ln " [ $breaks GetComponent $i 0 ]"
	}
	#puts stdout "Output Size = [ $newmatrix GetSize ] orig=$sz (breaks=$ln)"


	$breaks SetNumberOfTuples 1
	$breaks SetComponent 0 0 0.0
    } else {
	puts "Using All Runs\n"
    }


    if {  $parameters($this,afni_deletetemp) == 0 } {
	set matname ${outnamestem}_afni.matr
	$newmatrix Save $matname
	lappend flist $matname
    }
    

    set ord $parameters($this,afni_polort) 
    puts stdout "Preprocessing GLM, dolocaldrift=($dodrift), numrate=$numrate, runs=$minrun:$maxrun, inmatrix [ $newmatrix GetSize ]" 

    if { $numrate == 0 } {
	$biswaver AddDriftTerms $newmatrix $glmmatr $breaks $ord
    } else {
	$biswaver AddDriftAndRateTerms $newmatrix $glmmatr $breaks $ord $numrate
    }
    
    set motnamelst ""
    set usemotion $parameters($this,afni_glmregressmotion)
    set badmotion 0
    set totalrows 0
    
    if { $usemotion > 0 } { 
	for { set runcount $minrun } { $runcount <= $maxrun } { incr runcount } {
	    set motionmat($runcount) [ vtkpxMatrix New ]
	    $motionmat($runcount) ImportFromMatlab $parameters($this,run_motionfile_$runcount) "Q_ser"
	    set m [ $motionmat($runcount) GetSize ]
	    if { [ lindex $m 1 ] == 6 } {
		set motionlen($runcount) [ lindex $m 0 ]
		set totalrows [ expr $totalrows + [ lindex $m 0 ] ]
	    } else {
		puts stderr "Bad Motion file $parameters($this,run_motionfile_$runcount) size = $m "
		set badmotion 0
	    }
	}

	set newglm [ vtkpxMatrix New ]
	set oldsz [ $glmmatr GetSize ]
	if {  [ lindex $oldsz 0 ] != $totalrows } {
	    puts stderr "overall motion size"
	    set badmotion 1
	}
	
	if { $badmotion == 1 } {
	    $newmatrix Delete
	    $breaks Delete
	    $rawmatrix Delete
	    $glmmatr Delete
	    $biswaver Delete
	}
	
	puts stdout "\n\n Old GLM Matrix without Motion  ([$glmmatr GetSize])"
	set newglm [ vtkpxMatrix New ]
	$newglm Zeros $totalrows [ expr [ lindex $oldsz 1 ] + 6 ]
	$newglm CopyInto $glmmatr -1 -1 -1 -1 0 6
	set row 0
	for { set runcount $minrun } { $runcount <= $maxrun } { incr runcount } {
	    $motionmat($runcount) Save "run_${runcount}.mat"
	    $newglm CopyInto $motionmat($runcount) -1 -1 -1 -1 $row  0
	    set row [ expr $row + $motionlen($runcount) ]
	}
	set matname ${outnamestem}_glm_beforemotion.matr
	$glmmatr Save $matname
	lappend flist $matname
	$glmmatr Delete
	set glmmatr $newglm


	puts stdout "\n\n GLM Matrix with Motion addons file saved in $matname ([$newglm GetSize])"
    }

    set catname ${outnamestem}_allruns.nii.gz
    
    

    set matname ${outnamestem}_glm.matr
    $glmmatr Save $matname
    lappend flist $matname
    puts stdout "\n\n GLM Matrix file saved in $matname"

    
    pxtkprint "\n\n Computing GLM\n\n"
    pxtkprint "Generated GLM Matrix (see $matname) if delete temp is 0\n"

    set appnd [ vtkpxAppendComponents [ pxvtable::vnewobj ]]

    
    for { set runcount $minrun } { $runcount <= $maxrun } { incr runcount } {
	set runimage($runcount)	 [  [ pxitclimage \#auto ] GetThisPointer ]
	set fn	$parameters($this,run_file_$runcount)
	$runimage($runcount) Load $fn
	

	set norm [ vtkbisTimeSeriesNormalize New ]	    
	if { $dodrift == 1 } {
	    pxtkprint "\t\t .................... Normalizing Run $runcount ($fn) (masklevel = [ expr $parameters($this,afni_masklevel) *100.0 ]%) \n"
	    $norm SetInput [ $runimage($runcount) GetImage ]
	} else {
	    pxtkprint "\t\t .................... Normalizing and doing Global Drift correction on Run $runcount ($fn) (masklevel = [ expr $parameters($this,afni_masklevel) *100.0 ]%), drift pol=$parameters($this,afni_polort) \n"
	    set drift [ vtkbisRemoveGlobalTemporalDrift New ]
	    $drift SetOutputNormalized 0
	    $drift SetOrder $parameters($this,afni_polort)
	    $drift SetUseLegendrePolynomials 1
	    $drift SetInput [ $runimage($runcount) GetImage ]
	    $drift SetThreshold 0.5
	    $drift SetNormalizedThreshold 1
	    $drift Update
	    $norm SetInput [ $drift GetOutput ]
	    $drift Delete
	}

	$norm SetThreshold $parameters($this,afni_masklevel)
	$norm SetNormalizedThreshold 1
	$norm Update
	
	$appnd AddInput [ $norm GetOutput ]
	$norm Delete
	if { $runcount > $minrun } {
	    itcl::delete obj $runimage($runcount) 
	}
    }
    
    pxtkprint "\t\t .................... Concatenating Runs\n"
    
    $appnd Update
    
    
    $runimage($minrun) ShallowCopyImage [ $appnd GetOutput ]
    pxtkprint "\t\t .................... Concatenating Runs Done\n"
    if {  $parameters($this,afni_deletetemp) == 0 } {
	pxtkprint "\t\t .................... Beginning to Save Concatenated Runs\n"
	$runimage($minrun) Save $catname
	puts stdout "\n\n Concatenated Run image saved in $catname [ file size $catname ]" 
	pxtkprint "........... Concatenated Run image saved in $catname [ file size $catname ]" 
    }
    pxtkprint "Generated Concatenated Normalized Run Stack Image\n.....................[ $runimage($minrun) GetShortDescription ]\n"
    $appnd Delete
    $biswaver  Delete
    $breaks	   Delete
    $newmatrix Delete
    $rawmatrix    Delete

    puts stdout "\t\t\t image = [ [ $runimage($minrun) GetImage ] GetNumberOfScalarComponents ] matrix= [ $glmmatr GetSize]"

    return [ list $runimage($minrun) $glmmatr ]
}
# --------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::ComputeGLMComponent { outnamestem { singlerun 0 } { tasklist "" } { dodrift 1 } { numrate 0 } } {

    if { $singlerun > 0 && $singlerun <= $numruns } {
	set outnamestem "${outnamestem}_run${singlerun}"
	set extraname ""
    } else {
	set singlerun 0
	set extraname ""
    }

    set outlst [ $this PreprocessGLM $outnamestem $dodrift $numrate $singlerun ]
    if { $outlst == 0 } {
	return 0
    }
    

    set allrunsimage [ lindex $outlst 0 ]
    set glmmatr  [ lindex $outlst 1 ]

    puts stdout "In Compute GLM Component (single=$singlerun) extra=$extraname (Frames = [ [ $allrunsimage GetImage ] GetNumberOfScalarComponents ] ) mat size = [ $glmmatr GetSize ], numtasks=$numtasks"

    set glm [ vtkbisComputeGLM  [ pxvtable::vnewobj ]]
    $glm SetRegressorMatrix $glmmatr
    $glm SetInput [ $allrunsimage GetImage ]
    $glm SetNumberOfTasks $numtasks
    $glm Update

    puts stdout "\t\t\t\ Done with vtkbisComputeGLM"

    
    set outimage  ""; lappend outimage [ $glm GetOutput ]
    set outsuffix ""; lappend outsuffix "beta"
    set outzero   ""; lappend outzero   "fstat"
    if { $parameters($this,afni_tout) > 0 } {
	lappend outimage [ $glm GetTmapImage ]
	lappend outsuffix "tmap"
	lappend outzero   "mse"
    }
    
    # Here is the big job -- create a single subject .msb file for this guy and be done
    
    set w [ vtkpxAnalyzeImageWriter [ pxvtable::vnewobj ]]
    set extr [ vtkImageExtractComponents [ pxvtable::vnewobj ]]
    set setuptasklist $tasklist
    
    for { set i 0 } { $i <= $numtasks } { incr i } {
	for { set pass 0 } { $pass < [ llength $outzero ] } { incr pass } {
	    
	    puts stdout "outimage = $outimage , i=$i, item =$pass, numcomp [ [ lindex $outimage $pass ] GetNumberOfScalarComponents ]"
	    $extr SetInput [ lindex $outimage $pass ]
	    
	    set sf [lindex $outsuffix $pass ]
	    
	    if { $i > 0 } {
		set tname $parameters($this,task_$i)
		set tname "${tname}${extraname}_${sf}"
	    } else {
		set tname "[ lindex $outzero $pass ]${extraname}"
	    }
	    $extr SetComponents $i
	    $extr Modified
	    $extr Update
	    
	    $w SetInput [ $extr GetOutput ]
	    $w SetImageHeader [ $allrunsimage GetImageHeader ]
	    $w Modified
	    
	    set fname ${outnamestem}_$tname.nii.gz	
	    lappend setuptasklist [ list $tname $tname $fname ] 
	    
	    $w Save $fname
	    pxtkprint  "Map for  $tname saved in $fname [ file size $fname ] (Component $i, pass=$pass)\n"
	}
    }
    
    # Add overall term
    
    if { $parameters($this,afni_saveresidual) > 0 } {
	set tname "residualtimeseries${extraname}"
	$w SetInput [ $glm GetUnfitTimeSeries ]
	$w SetImageHeader [ $allrunsimage GetImageHeader ]
	$w Modified
	
	set fname ${outnamestem}_$tname.nii.gz	
	lappend setuptasklist [ list $tname $tname $fname ] 
	$w Save $fname
    }
    
    if { $parameters($this,afni_savedriftbeta) > 0 } {
	
	set tname "driftbetas${extraname}"
	$w SetInput [ $glm GetDriftBetaValues ]
	$w SetImageHeader [ $allrunsimage GetImageHeader ]
	$w Modified
	
	set fname ${outnamestem}_$tname.nii.gz	
	lappend setuptasklist [ list $tname $tname $fname ] 
	$w Save $fname
    }
    
    pxtkprint "Done Saving .. clean up time\n"
    puts stdout "\t\t\t\ Done saving\n"
    
    $w Delete
    $extr Delete
    $glm Delete
    itcl::delete obj $allrunsimage 
    $glmmatr   Delete 
    
    return $setuptasklist
}
# ----------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::ComputeGLM { { askfname 0 } { report 0 } } {

    set ln [ $this GenerateOutputFilenames $askfname ]
    if { $ln == 0 } {
	return 0
    }
    
    set outnamestem [ lindex $ln 0 ]
    set msbsetupname [ lindex $ln 1 ]
    catch { file mkdir $parameters($this,output_dir) }

    pxtkprint "Beginning Stem =$outnamestem\n\t Setup=$msbsetupname\n"
    
    set setuptasklist [ $this ComputeGLMComponent $outnamestem 0 ]
    if { $parameters($this,afni_glmsinglerunoutput) > 0 } {
	for { set i 1 } { $i <= $numruns } { incr i } {
	    puts stdout "\n\n--------------------------------------------------------------\n Single Component Processing for Run = $i"
	    set tmp $setuptasklist 
	    set setuptasklist [ $this ComputeGLMComponent $outnamestem $i $tmp ]
	}
    }
    
    $this CreateMSBFile $msbsetupname $setuptasklist
    $this InfoMessage  "Done GLM -- see single subject file in $msbsetupname" "FYI"
    $this DisplayMSBFile $msbsetupname
    
    return $outnamestem
}
# ----------------------------------------------------------------------------------------

itcl::body bis_fmrisetup::ComputeRateGLM { { askfname 0 } { report 0 } } {

    set ln [ $this GenerateOutputFilenames $askfname ]
    if { $ln == 0 } {
	return 0
    }
    
    set numrate $parameters($this,afni_numrateterms)
    if { $numrate > 3 } {
	set numrate 3
    }

    set doglobal $parameters($this,afni_rateglobal)
    set dolocaldrift [ expr $doglobal <1 ]

    set outnamestem [ lindex $ln 0 ]
    set msbsetupname [ lindex $ln 1 ]
    catch { file mkdir $parameters($this,output_dir) }

    #    pxtkconsole
    pxtkprint "Beginning Stem =$outnamestem\n\t Setup=$msbsetupname\n"
    set outlst [ $this PreprocessGLM $outnamestem $dolocaldrift $numrate ]
    if { $outlst == 0 } {
	return 0
    }
    
    set allrunsimage [ lindex $outlst 0 ]

    set glmmatr  [ lindex $outlst 1 ]


    set glm [ vtkbisComputeGLM  [ pxvtable::vnewobj ]]
    $glm SetRegressorMatrix $glmmatr
    $glm SetInput [ $allrunsimage GetImage ]
    $glm SetNumberOfTasks [ expr $numtasks* ($numrate+1) ]
    $glm Update

    puts stdout "GLM Matrix size  [ $glmmatr GetSize ], numtasks=$numtasks, numframes out= [ [ $glm GetOutput ] GetNumberOfScalarComponents ]"
    
    set outimage  ""; lappend outimage [ $glm GetOutput ]
    set outsuffix ""; lappend outsuffix "beta"
    set outzero   ""; lappend outzero   "fstat"
    if { $parameters($this,afni_tout) > 0 } {
	puts stdout " TMap numframes, [ [ $glm GetTmapImage ] GetNumberOfScalarComponents ]"
	lappend outimage [ $glm GetTmapImage ]
	lappend outsuffix "tmap"
	lappend outzero   "mse"
    }
    
    puts stdout "GLM Output = [ $glm GetOutput ], numframes = [ [ $glm GetOutput ] GetNumberOfScalarComponents ]"
    puts stdout "TMAP Output = [ $glm GetTmapImage ], numframes = [ [ $glm GetTmapImage ] GetNumberOfScalarComponents ]"

    #    set ratepass [ llength $numrate ]


    #    for { set i 0 } { $i < $numrate } { incr i } {
    #	lappend outimage [ $glm GetOutput ]
    #	lappend outsuffix "beta_rate_[ lindex $ratenames $i ]"
    #	lappend outzero   "*none*"
    #    }
    set maxpass [ llength $outzero ] 
    
    # Here is the big job -- create a single subject .msb file for this guy and be done
    
    set w [ vtkpxAnalyzeImageWriter [ pxvtable::vnewobj ]]
    set extr [ vtkImageExtractComponents [ pxvtable::vnewobj ]]
    set setuptasklist ""	
    
    set outframes [ [ $glm GetOutput ] GetNumberOfScalarComponents ]
    set ratenames [ list "" "_linear" "_quadratic" "_cubic" ]

    for { set i 0 } { $i < $outframes } { incr i } {
	for { set pass 0 } { $pass < $maxpass } { incr pass } {
	    
	    $extr SetInput [ lindex $outimage $pass ]
	    
	    set sf [lindex $outsuffix $pass ]
	    set fr $i
	    
	    if { $i > 0 } {
		set j [ expr int($i-1)/int($numtasks) ]
		set k [ expr $i - $j * $numtasks ]
		puts stdout "\t\t i=$i, numtasks=$numtasks j=$j k=$k"
		set tname $parameters($this,task_$k)
		set tname "${tname}_${sf}[ lindex $ratenames $j ]"
	    } else {
		set tname [ lindex $outzero $pass ]
	    }

	    $extr SetComponents $fr
	    $extr Modified
	    $extr Update
	    
	    $w SetInput [ $extr GetOutput ]
	    $w SetImageHeader [ $allrunsimage GetImageHeader ]
	    $w Modified
	    
	    set fname ${outnamestem}_$tname.nii.gz	
	    lappend setuptasklist [ list $tname $tname $fname ] 
	    
	    puts stdout "outimage = [ lindex $outimage $pass ] , i=$i, pass =$pass \t numcomp=[ [ lindex $outimage $pass ] GetNumberOfScalarComponents ], using frame=$fr, fname=[ file tail $fname ]"
	    
		$w Save $fname
	    pxtkprint  "Map for  $tname saved in $fname [ file size $fname ] (Component $i, pass=$pass)\n"
	}
    }
    
    # Add overall term
    
    if { $parameters($this,afni_saveresidual) > 0 } {
	set tname "residualtimeseries"
	$w SetInput [ $glm GetUnfitTimeSeries ]
	$w SetImageHeader [ $allrunsimage GetImageHeader ]
	$w Modified
	
	set fname ${outnamestem}_$tname.nii.gz	
	lappend setuptasklist [ list $tname $tname $fname ] 
	$w Save $fname
    }
    
    if { $parameters($this,afni_savedriftbeta) > 0 } {
	
	set tname "driftbetas"
	$w SetInput [ $glm GetDriftBetaValues ]
	$w SetImageHeader [ $allrunsimage GetImageHeader ]
	$w Modified
	
	set fname ${outnamestem}_$tname.nii.gz	
	lappend setuptasklist [ list $tname $tname $fname ] 
	$w Save $fname
    }
    
    pxtkprint "Done Saving .. clean up time\n"
    
    $this CreateMSBFile $msbsetupname $setuptasklist
    
    $w Delete
    $extr Delete
    $glm Delete
    itcl::delete obj $allrunsimage 
    $glmmatr   Delete 
    
    $this InfoMessage  "Done GLM -- see single subject file in $msbsetupname" "FYI"
    $this DisplayMSBFile $msbsetupname
    
    
    return $outnamestem
}

# ----------------------------------------------------------------------------------------

itcl::body bis_fmrisetup::AFNICreateScript { { askfname 0 } { report 0 } } {

	set tempfiles ""

	if { $parameters($this,afni_ensurenifti) == 1 } {
	$this EnsureRunsAreNIFTI
	}

	catch { file mkdir $parameters($this,output_dir) }

	global tcl_platform
	set endsuffix ".sh"


	if { $askfname } {
	set typelist { {"AFNI Script File" { *.sh }}}
	set tryname [ file join $parameters($this,output_dir)  $parameters($this,subject_id) ]${endsuffix}
	set fname [tk_getSaveFile -title "Specify Script Filename (and implicitly path)" \
			   -filetypes $typelist -initialfile [ file tail $tryname ] -initialdir $parameters($this,output_dir) ]
	if { [ string length $fname ] == 0 } {
		return 0
	}
	set fname  [ file rootname [ file normalize $fname ] ]
	set parameters($this,output_dir) [ file dirname $fname ]
	set outnamestem [ file join $parameters($this,output_dir)  $fname ]
	} else	{
	set outnamestem [ file join $parameters($this,output_dir)  $parameters($this,subject_id) ]
	}

	set tr $parameters($this,tr)
	set outscript ${outnamestem}${endsuffix}

	set stimlist ""


	set lbreak "\\\n\t"    

	set timefiles  ${outnamestem}_timepoints
	if { $parameters($this,afni_useafniwaver) == 1 } {

	set comment "\# Using AFNI Waver"

	set waverout [ AFNICreateTimeFile $timefiles ]
	if { $waverout == 0 } {
		return 0
	}
	set inpname [ lindex $waverout 0  ]
	set startpointsfile [ lindex $waverout 1 ]
	if { [ file exists $startpointsfile ] == 0 } {
	    $this InfoMessage  "Waver input file $inpname does not exist!" "Error ..." error
	    return 0
	}

	#    puts stdout "inpname = $inpname, startpointsfile=$startpointsfile"
	for {set runcount 1} {$runcount <= $numruns} {incr runcount} {
		set runfile($runcount) ${inpname}_${runcount}.txt
		lappend tempfiles $runfile($runcount)
	}
	} else {
	# Do this simply in BioImage Suite !!!!
	pxtkprint "\n\n NOT Using AFNI Waver\n"
	puts stdout "\n\n NOT Using AFNI Waver\n"
	set lst [ $this CreateWaverMatrix ]
	if { $lst == 0 } {
		return 0 
	}
	set newmatrix  [ lindex $lst 0 ]
	set breaks     [ lindex $lst 1 ]
	set rawmat     [ lindex $lst 2 ] ; $rawmat Delete
	set comment    "Using vtkbisWaver [ lindex $lst 3 ]"
	set numrows    [ lindex [ $newmatrix GetSize ] 0 ]


	for {set taskcount 1} {$taskcount<= $numtasks} {incr taskcount} {
		set tname $parameters($this,task_$taskcount)
		set outname ${outnamestem}_task${tname}
		set col [ expr $taskcount -1 ]

		set bfout [ open $outname w ]
		for { set row 0 } { $row < $numrows } { incr row } {
		puts $bfout "[$newmatrix GetElement $row $col ]"
		}
		close $bfout
		lappend stimlist [ list $tname $outname ]    
	}
	set startpointsfile "${outnamestem}_startpoints.txt"
	set inpname "${outnamestem}_matrix.txt"
	$newmatrix Save $inpname
	
	set nt [ $breaks GetNumberOfTuples ]
	set bfout [ open $startpointsfile w ]
	for { set i 0 } { $i < $nt } { incr i } {
		puts $bfout "[ $breaks GetComponent $i 0 ]"
	}
	close $bfout
	$breaks Delete
	$newmatrix Delete
	}
	lappend tempfiles $startpointsfile 

	set fout [ open $outscript w ]
	AFNICreateScriptHelper $fout "\#---------------------------------------------------------------"
	AFNICreateScriptHelper $fout "\# AFNI Analysis Script created from BioImage Suite"
	AFNICreateScriptHelper $fout "\# Original time point files are in:"
	AFNICreateScriptHelper $fout "\#\t $inpname"
	AFNICreateScriptHelper $fout "\# and \t $startpointsfile"
	AFNICreateScriptHelper $fout "\# $comment"
	AFNICreateScriptHelper $fout "\#---------------------------------------------------------------"
	AFNICreateScriptHelper $fout "\# Using $numruns runs"
	#for { set runcount 1 } { $runcount <= $numruns } { incr runcount } {
	#	set rname $parameters($this,run_file_$runcount)
	#	AFNICreateScriptHelper $fout "\# Run $runcount : [ file tail $rname ], numframes = $parameters($this,run_file_${runcount}_length)"
	#    }
	AFNICreateScriptHelper $fout "\#---------------------------------------------------------------"
	AFNICreateScriptHelper $fout ""

	if { $parameters($this,afni_useafniwaver) == 1 } {
	
	for {set runcount 1} {$runcount <= $numruns} {incr runcount} {
		set runfile($runcount) ${inpname}_${runcount}.txt
		lappend tempfiles $runfile($runcount)

		# Step 1 Waver
		AFNICreateScriptHelper $fout "# Step 1 Waver"
		AFNICreateScriptHelper $fout ""
		for {set taskcount 1} {$taskcount<= $numtasks} {incr taskcount} {
		set tname $parameters($this,task_$taskcount)
		set i [ expr $taskcount -1 ]
		set tlist ""
		
		AFNICreateScriptHelper $fout "\# +++++ Task $taskcount/$numtasks ($tname) "
		for {set runcount 1} {$runcount <= $numruns} {incr runcount} {
			
			set numf $parameters($this,run_file_${runcount}_length)	  
			
			set iname "$runfile($runcount)\[$i\]"
			set outname ${outnamestem}_task${tname}_run${runcount}
			set cmd "waver	-delaytime $parameters($this,afni_delaytime) -risetime $parameters($this,afni_risetime) -falltime $parameters($this,afni_falltime) -undershoot $parameters($this,afni_undershoot) -TR $tr -peak $parameters($this,afni_peak) $lbreak -input \'${iname}\' $lbreak | head -$numf > $outname"
			AFNICreateScriptHelper $fout $cmd 1
			lappend tempfiles $outname
			lappend tlist $outname
		}
	
		set outname ${outnamestem}_task${tname}
		set cmd "cat "
		for { set i 0 } { $i < [ llength $tlist ] } { incr i } {
			set cmd "$cmd [ lindex $tlist $i ] $lbreak"
		}
		set cmd "$cmd > $outname"
		AFNICreateScriptHelper $fout $cmd 1
		lappend stimlist [ list $tname $outname ]
		lappend tempfiles $outname
		AFNICreateScriptHelper $fout ""
		}
	}
	} else {

	AFNICreateScriptHelper $fout "\# Stim Files Created using vtkbisWaver"
	for { set i 0 } { $i < [ llength $stimlist ] } { incr i } {
		AFNICreateScriptHelper $fout "\# [ lindex $stimlist $i ]"
	}
	}

	if { $parameters($this,afni_useafninormalize) == 1 } {

	AFNICreateScriptHelper $fout ""
	AFNICreateScriptHelper $fout "# Step 2: Normalize and Mask"
	AFNICreateScriptHelper $fout ""
	
	set allruns ""
	
	for { set runcount 1 } { $runcount <= $numruns } { incr runcount } {
		AFNICreateScriptHelper $fout "\# +++++ Run $runcount/$numruns "
		set fn	$parameters($this,run_file_$runcount)
		set tt [ file rootname [ file tail $fn ] ]
		if { [ file extension $tt ] == ".nii" } {
		set tt [ file rootname $tt ]
		}
		set tt ${outnamestem}_${tt}
		
		set meanname ${tt}_mean.nii.gz
		set normname ${tt}_norm.nii.gz 
		
		set cmd "3dTstat -mean -prefix $meanname $lbreak $fn\n"
		AFNICreateScriptHelper $fout $cmd 1
		lappend tempfiles $meanname
		set cmd "3dcalc -fscale -a $fn $lbreak -b $meanname $lbreak -expr \' (a/b * 100) \' -prefix $normname"
		AFNICreateScriptHelper $fout $cmd 1
		
		lappend tempfiles $normname
		lappend allruns $normname
		puts $fout "\n\n"
	}
	
	set catname ${outnamestem}_allruns.nii.gz
	AFNICreateScriptHelper $fout ""
	AFNICreateScriptHelper $fout "# Step 3: Concatenate"
	AFNICreateScriptHelper $fout ""
	set cmd "3dTcat -prefix $catname" 
	set n [ expr [ llength $allruns ] -1 ]
	for { set i 0 } { $i < [ llength $allruns ] } { incr i } {
		set cmd	 "$cmd $lbreak [ lindex $allruns $i ]"
	}
	AFNICreateScriptHelper $fout $cmd 1
	lappend tempfiles $catname
	
	AFNICreateScriptHelper $fout ""
	
	set cmd "3dinfo $catname"
	AFNICreateScriptHelper $fout $cmd 1
	} else {
	set catname ${outnamestem}_allruns.nii.gz


	set appnd [ vtkpxAppendComponents [ pxvtable::vnewobj ]]
#	$appnd DebugOn

	for { set runcount 1 } { $runcount <= $numruns } { incr runcount } {
		set runimage($runcount)	 [  [ pxitclimage \#auto ] GetThisPointer ]
		set fn	$parameters($this,run_file_$runcount)
		$runimage($runcount) Load $fn

		set norm [ vtkbisTimeSeriesNormalize [ pxvtable::vnewobj ]]
		$norm DebugOn
		$norm SetInput [ $runimage($runcount) GetImage ]
		set mname "${outnamestem}_mean_${runcount}.nii.gz"
		#	    $norm SetDebugName	$mname
		#	    lappend tempfiles $mname
		$norm Update
		
		
		$appnd AddInput [ $norm GetOutput ]
		$norm Delete
		if { $runcount > 1 } {
		itcl::delete obj $runimage($runcount) 
		}
	}
	$appnd Update
	$runimage(1) ShallowCopyImage [ $appnd GetOutput ]
	$runimage(1) Save $catname
	$appnd Delete
	itcl::delete obj $runimage(1) 
	}

	if { $parameters($this,afni_polort) < 1 } {
	set parameters($this,afni_polort) 1
	} elseif { $parameters($this,afni_polort) > 4 } {
	set parameters($this,afni_polort) 4
	}

	set decname ${outnamestem}_deconvolve_stats.nii.gz

	AFNICreateScriptHelper $fout ""
	AFNICreateScriptHelper $fout "\# Step 4: Run GLM"
	AFNICreateScriptHelper $fout ""
	set cmd "3dDeconvolve -input $catname $lbreak -polort $parameters($this,afni_polort) -concat $startpointsfile -num_stimts [ llength $stimlist ] $lbreak"
	for { set i 0 } { $i < [ llength $stimlist ] } { incr i } {
	set elem [ lindex $stimlist $i ]
	set fn	 [ lindex $elem 1 ]
	set tn	 [ lindex $elem 0 ]
	set j [ expr $i + 1 ]
	set cmd "$cmd -stim_file $j $fn -stim_label $j $tn $lbreak"
	}

	if { $parameters($this,afni_fout) > 0 } {	set cmd "$cmd -fout"	} 
	if { $parameters($this,afni_rout) > 0 } {	set cmd "$cmd -rout"	} 
	if { $parameters($this,afni_tout) > 0 } {	set cmd "$cmd -tout"	} 
	if { $parameters($this,afni_vout) > 0 } {	set cmd "$cmd -vout"	} 
	if { $parameters($this,afni_bout) > 0 } {	set cmd "$cmd -bout"	} 

	set parameters($this,afni_bucket) 1

	if { $parameters($this,afni_bucket) > 0 } {	
	set cmd "$cmd -bucket $decname"	  
	}  else {
	set cmd "$cmd -nobucket"
	}


	AFNICreateScriptHelper $fout $cmd 1

	AFNICreateScriptHelper $fout ""
 
	if { $parameters($this,afni_bucket) > 0 } {	
	set cmd "3dinfo $decname"
		AFNICreateScriptHelper $fout $cmd 1
	}


	if { $endsuffix == ".sh" } {
	exec chmod +x $outscript
	}

	set cleanfile ""
	puts $fout "\#---------------------------------------------------------------\n\n"
	
	set comment "\#\t\t" 
	if {  $parameters($this,afni_deletetemp) == 0 } {
	set comment ""
	close $fout
	set cleanfile [ file rootname $outscript ]_clean${endsuffix}
	set fout [ open $cleanfile w ]
	}
	
	set comment "$comment \\rm"

	puts $fout "\n\# Optional Cleanup"
	for { set i 0 } { $i < [ llength $tempfiles ] } { incr i } {
	puts $fout "$comment [ lindex $tempfiles $i ]"
	}
	close $fout

	if { $cleanfile != "" } {
	exec chmod +x $cleanfile
	set cleanfile "(and also $cleanfile)"
	}
	

	if { $report == 1 } {
	$this InfoMessage  "Saved afni script file in\n $outscript $cleanfile" "FYI"  info
	}
	puts stdout "\n\nOutput Script for AFNI saved in $outscript $cleanfile"

	return $outnamestem
}


# ----------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::ReorganizeData { dirname } {

	catch { file mkdir $dirname }
	if { [ file writable $dirname ] == 0 } {
	tk_messageBox -type ok -title "Error ..." -message "Bad Output Directory $dirname"
	return 0
	}



	# --------------------------------------------------------------------------------------

	set imglist [ list reference_brain anatomical conventional voiDefinitionImage reference_gw_map ]
	set trnlist [ list referenceTransform internalTransform distortionTransform inverseReferenceTransform ]

	for { set i 0 } { $i < [ llength $imglist ] } { incr i } {
	set img	 [  [ pxitclimage \#auto ] GetThisPointer ]
	set pname [ lindex $imglist $i ]
	set fn	$parameters($this,$pname) 
	if { [ string length $fn ] > 0 } {
		set ok [ $img Load $fn ]
	} else {
		set ok 1
	}
	if { $ok > 0 && [ string length $fn ] > 0 } {
		set fn2 [ file tail $fn ]
		set fn3 [ file join $dirname $fn2 ]
		$img Save $fn3
		set parameters($this,$pname) $fn3

		puts stdout "$fn2 -> $fn3"

	} else {
		set parameters($this,$pname) ""
	}
	itcl::delete obj $img
	}

	set parameters($this,reference_brain_path) $parameters($this,reference_brain) 

	for { set i 0 } { $i < [ llength $trnlist ] } { incr i } {
	set trn	 [  [ pxitcltransform \#auto ] GetThisPointer ]
	set pname [ lindex $trnlist $i ]
	set fn	$parameters($this,$pname) 
	if { [ string length $fn ] > 0 } {
		set ok [ $trn Load $fn ] 
	} else {
		set ok 1
	}
	if { $ok > 0 && [ string length $fn ] > 0 } {
		set fn2 [ file tail $fn ]
		set fn3 [ file join $dirname $fn2 ]
		$trn Save $fn3
		set parameters($this,$pname) $fn3
	} else {
		set parameters($this,$pname) ""
	}
	itcl::delete obj $trn
	}

	for { set runcount 1 } { $runcount <= $numruns } { incr runcount } {
	set img	 [  [ pxitclimage \#auto ] GetThisPointer ]
	set fn	$parameters($this,run_file_$runcount)
	set ok [ $img Load $fn ]
	if { $ok > 0 } {
		set fn2 [ file tail $fn ]
		set fn3 [ file join $dirname $fn2 ]
		$img Save $fn3
		set parameters($this,run_file_$runcount) $fn3
	} else {
		set parameters($this,run_file_$runcount) ""
	}
	itcl::delete obj $img
	
	set fn	$parameters($this,run_motionfile_$runcount)
	puts stdout "\n runcount=$runcount $fn=$fn"
	if { [ file exists $fn ] > 0 } {
		set fn2 [ file tail $fn ]
		set fn3 [ file join $dirname $fn2 ]
#	    puts stdout "\n $fn2 -> $fn3 "
		file copy $fn $fn3
		set parameters($this,run_motionfile_$runcount) $fn3
	} else {
		set parameters($this,run_motionfile_$runcount) ""
	}
	}
	
	set newsetupname [ file join $dirname [ file tail $setupname ]]
	return [ $this SaveSetupFile $newsetupname ]
}

# ----------------------------------------------------------------------------------------
# Progress Stuff
# ----------------------------------------------------------------------------------------
::itcl::body bis_fmrisetup::AddLogoLabel { w } {

	global tcl_platform

	set l [ label $w.[ pxvtable::vnewobj ] -image  $::pxtclvtkpxcontrib::smalllogo ]

	return $l
}

::itcl::body bis_fmrisetup::CreateProgressBar { w } {
	
	set l [ AddLogoLabel $w	 ]
	pack $l -side left -expand f -padx 1

	set progressbar($this) $w.pbar
	iwidgets::feedbackplus $w.pbar -steps 100  -barheight 10  -labelpos w -labeltext $progress_lab($this) 
	pack $w.pbar -side right -expand t -fill x

	#   $w configure -bg "\#8197c5" 
	#    [ $w.pbar component trough ] configure  -bg  "\#cccdf8"
	#    [ $w.pbar component percentage ] configure	 -fg  black 
	


}

::itcl::body bis_fmrisetup::ProgressCallback { filter lab } {


	#    puts stdout "Progress Callback ($this) = [ $filter GetClassName ] $label"
	if { $progressbar($this) != 0 } {
	$this ShowProgress $filter $lab
	}
	return 0

}
# ----------------------------------------------------------
::itcl::body bis_fmrisetup::ShowProgress { filter lab } {
	
	if { $progressbar($this) == 0 } {
	return
	}

	set progress_val($this) 50
	catch { set progress_val($this)	 [expr [$filter GetProgress]*100] }
	set progress_lab($this) $lab
	


	if {$lab == "Done" } { 
	set progress_val($this) 0
	}
	

	$progressbar($this) configure  -labeltext $progress_lab($this)
	$progressbar($this) setval [ expr round($progress_val($this)) ]

	update idletasks
}
# ----------------------------------------------------------
::itcl::body bis_fmrisetup::ShowProgressVal { name val } {
	
	if { $progressbar($this) == 0 } {
	return
	}
	
	set progress_val($this) [expr $val*100.0]
	set progress_lab($this) "$name"
	
	if {$name == "Done" } { 
	set progress_val($this) 0
	}

	$progressbar($this) configure  -labeltext $progress_lab($this)
	$progressbar($this) setval [ expr round($progress_val($this)) ]

	update idletasks
}
# ----------------------------------------------------------
itcl::body bis_fmrisetup::SetFilterCallbacks { filter comment } {
	
	set t [ itcl::scope $this ]

	eval "$filter AddObserver ProgressEvent { $t ProgressCallback $filter \"$comment\"}"
	eval "$filter AddObserver EndEvent	{ $t ProgressCallback $filter Done}"
}


# ----------------------------------------------------------------------------------------
#
#   Resting State Stuff
#
#
# ----------------------------------------------------------------------------------------
# Utility Methods
# ----------------------------------------------------------------------------------------

itcl::body bis_fmrisetup::PreprocessSingle4DImage { img mask { skipframeslist 0 } { motionmatrixname 0 } { graywhitemask 0 } } {

	# motionmatrixname is either 0 or name of matlab .mat file for image
	# graywhitemattermask is either 0 or pxitclimage of mask image

	#
	#    lappend restinglist { "#Mask Level (% of max intensity)"  masklevel 0.05 }
	#    lappend restinglist { "#Polynomial Drift"	driftpol 3 } 
	#    lappend restinglist { "#Slice Mean Removal"  slicemeanremoval 0 } 
	#    lappend restinglist { "#Process Runs Individually"	 individualruns 0 } 
	#    lappend restinglist { "#Temporal Smoothing Sigma (Frames)"	 temporalsigma 1.0 } 
	#    
	

	# Flag parameters($this,resting_usemotionparams)  ] -text "Regress Motion Parameters" 
	# Flag parameters($this,resting_usecsfmean)  ] -text "Regress CSF/White Matter Mean" 
	# graywhitemask = gray/white/csf mask in functional space
	# matrix is motion matrix

	puts stdout "using mask	 [ $mask cget -filename ]"
	set currentinput $img
	set csfwmmotion_alg [ bis_csfwmmotioncorr \#auto ]
	set tempsmooth_alg  [ bis_temporalsmoothimage \#auto ] 
	set tempdrift_alg   [ bis_removetemporaldrift  \#auto ]
	set tempremove_alg  [ bis_removeslicemean \#auto ]
	set skipframe_alg   [ bis_imageremoveframes \#auto ]
	
	# csf whitematter temporal drift removal
	if { $parameters($this,resting_usecsfmean) >0 } {
		puts stdout "we are doing csf whitematter temporal drift removal"
	$tempdrift_alg InitializeFromContainer 0 $this
	$tempdrift_alg SetInput $currentinput
	$tempdrift_alg SetSecondInput $graywhitemask
	$tempdrift_alg SetOptionValue order $parameters($this,resting_driftpol)
	$tempdrift_alg SetOptionValue usemask 1
	$tempdrift_alg SetOptionValue legendre 1
	$tempdrift_alg Execute
	set currentinput [ $tempdrift_alg GetOutput ]
	}
	
	#csf/wm/motion correction
	if { $parameters($this,resting_usemotionparams) > 0 || $parameters($this,resting_usecsfmean) >0 } {
		puts stdout "we are doing csfwmmotion correction"
		$csfwmmotion_alg InitializeFromContainer 0 $this
		$csfwmmotion_alg SetInput $currentinput
		
		if { $parameters($this,resting_usemotionparams) > 0 } {
			$csfwmmotion_alg SetOptionValue filename $motionmatrixname
			$csfwmmotion_alg SetOptionValue domotion 1
		}
		if { $parameters($this,resting_usecsfmean) > 0 } {
			$csfwmmotion_alg SetSecondInput $graywhitemask
			$csfwmmotion_alg SetOptionValue docsfwm 1
			$csfwmmotion_alg SetOptionValue usemask 1
		}
		$csfwmmotion_alg Execute
		set currentinput [ $csfwmmotion_alg GetOutput ]
	}
	

	# temporal smooth
	if { $parameters($this,resting_temporalsigma) > 0 } {
		puts stdout "we are doing temporal smooth"
	$tempsmooth_alg InitializeFromContainer 0 $this
	$tempsmooth_alg SetInput $currentinput
	$tempsmooth_alg SetSecondInput $mask
	$tempsmooth_alg SetOptionValue blursigma $parameters($this,resting_temporalsigma)
	$tempsmooth_alg SetOptionValue usemask 1
	$tempsmooth_alg Execute
	set currentinput [ $tempsmooth_alg GetOutput ]
	}
	
	# gray matter temporal drift removal
	if { $parameters($this,resting_driftpol) > -0.1 } {
		puts stdout "we are doing drift removal"
	$tempdrift_alg InitializeFromContainer 0 $this
	$tempdrift_alg SetInput $currentinput
	$tempdrift_alg SetSecondInput $mask
	$tempdrift_alg SetOptionValue order $parameters($this,resting_driftpol)
	$tempdrift_alg SetOptionValue usemask 1
	$tempdrift_alg SetOptionValue legendre 1
	$tempdrift_alg Execute
	set currentinput [ $tempdrift_alg GetOutput ]
	}
	
	
	# remove slice mean
	if { $parameters($this,resting_slicemeanremoval) > 0 || 
	 $parameters($this,resting_volumemeanremoval) > 0 } {
	$tempremove_alg InitializeFromContainer 0 $this
	$tempremove_alg SetInput $currentinput
	$tempremove_alg SetSecondInput $mask
	$tempremove_alg SetOptionValue usemask 1
	if { $parameters($this,resting_slicemeanremoval) > 0 } {
		$tempremove_alg SetOptionValue globalmean 0
		puts stdout "we are doing remove slice mean"
	} else {
		$tempremove_alg SetOptionValue globalmean 1
		puts stdout "we are doing remove volumn mean"
	}
	$tempremove_alg Execute
	set currentinput [ $tempremove_alg GetOutput ]
	}
	
	if { $skipframeslist !=0 } {
	$skipframe_alg InitializeFromContainer 0 $this
	$skipframe_alg SetInput $currentinput
	$skipframe_alg SetOptionValue unit $parameters($this,blockunits)
	$skipframe_alg SetOptionValue offset 1
	$skipframe_alg SetOptionValue tr $parameters($this,tr)
	$skipframe_alg SetOptionValue framelist $skipframeslist
	$skipframe_alg SetOptionValue keep 0
	$skipframe_alg Execute
	set currentinput [ $skipframe_alg GetOutput ]
	}


  #remove frames here----------------------------------
	# Copy last result
	set newimg [  [ pxitclimage \#auto ] GetThisPointer ]
	$newimg ShallowCopy $currentinput

	# Clean up objects
	itcl::delete obj $tempsmooth_alg
	itcl::delete obj $tempdrift_alg
	itcl::delete obj $tempremove_alg
	itcl::delete obj $skipframe_alg
	itcl::delete obj $csfwmmotion_alg

	return $newimg
}


# ------------------------------------------------------------------------------------------
#
# This concatenates all runs and does group preprocessing using the flags below
# 
# -------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::ConcatenateAndPreprocessRuns { mask { graywhitemask 0 } { checkformotioncorrection 0 } { enableskipframes 0 } } {

	set appnd [ vtkpxAppendComponents [ pxvtable::vnewobj ]]

	set comment "Concatenating :"
	set failed 0

	#    set numruns 2

	for { set runcount 1 } { $runcount <= $numruns } { incr runcount } {
	set runimage($runcount)	 [  [ pxitclimage \#auto ] GetThisPointer ]
	set fn	$parameters($this,run_file_$runcount)
	set ok [ $runimage($runcount) Load $fn ]
	if { $ok > 0 } {
		pxtkprint "\t\t .................... Adding Run $runcount ($fn) [ $runimage($runcount) GetShortDescription ] \n"
		puts stdout "\t\t .................... Adding Run $runcount ($fn) [ $runimage($runcount) GetShortDescription ] \n"

		
		set runskipframes 0
		if { $enableskipframes > 0 } {
		if { $parameters($this,runskip_$runcount) == 1	|| $parameters($this,checks) > 0 } {
			set runskipframes "$parameters($this,skipFrames) $parameters($this,skiprunsframe_$runcount)"
		}
		}
		puts stdout "\t\t\t\t skipframes for this run = $runskipframes"

		set motionmatrixname 0
		if { $parameters($this,resting_usemotionparams) > 0 && $checkformotioncorrection > 0 } {
		    set motionmatrixname $parameters($this,run_motionfile_$runcount)
		}
		
		set tmp [ $this PreprocessSingle4DImage $runimage($runcount) $mask $runskipframes $motionmatrixname $graywhitemask ]
		$appnd AddInput [ $tmp GetImage ]
		itcl::delete obj $tmp
		if { $runcount > 1 } {
		itcl::delete obj $runimage($runcount)
		}
		set comment "$comment $fn"
	} else {
		set runcount [ expr $numruns + 5 ]
		set failed 1
	}
	}

	if { $failed == 0 } {
	$appnd Update
	set conc  [  [ pxitclimage \#auto ] GetThisPointer ]
	$conc ShallowCopyImage [ $appnd GetOutput ]
	$conc CopyImageHeader  [ $runimage(1) GetImageHeader ]
	itcl::delete obj $runimage(1)
	[ $conc GetImageHeader ] AddComment "$comment" 0
	$appnd Delete
	return $conc
	}

	itcl::delete obj $runimage(1)
	return 0
}
# ----------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::CreateMask { img threshold } {

	set r [ [ [ [ $img GetImage ] GetPointData ] GetScalars	 ] GetRange ]

	set rmin [ lindex $r 0 ]
	set rmax [ lindex $r 1 ]
	
	puts stdout "image range $rmin $rmax"

	set lth [ expr $threshold*($rmax-$rmin) + $rmin ]
	set uth [ expr $rmax + 1.0 ]

	pxtkprint "Creating mask threshold = $threshold (thr=$lth)\n"
	puts stdout "Creating mask threshold = $threshold (thr=$lth)\n"
	
	puts stdout "Threshold = $lth, $uth "


	set extr [ vtkImageExtractComponents New ]
	$extr SetInput [ $img GetImage ]
	$extr SetComponents 0
	#    $extr DebugOn
	$extr Update
	
	set tthr [ vtkImageThreshold New ]
	$tthr SetInput [ $extr GetOutput ]
	$tthr ThresholdBetween $lth $uth
	$tthr ReplaceInOn
	$tthr ReplaceOutOn
	$tthr SetInValue 1
	$tthr SetOutValue 0
	#$tthr DebugOn
	$tthr Update

	set mask  [  [ pxitclimage \#auto ] GetThisPointer ]
	$mask ShallowCopy $img
	$mask ShallowCopyImage [ $tthr GetOutput ]
	$mask configure -filename "mask.nii.gz"

	$tthr Delete
	$extr Delete
	return [  $mask GetThisPointer ]
}



# ----------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::CheckTransformationsForVOI { { graywhitemaskmode 0 } } {

    set mname  $parameters($this,voiDefinitionSpace) 
    
    if { $graywhitemaskmode > 0 } {
	set mname "Reference"
    }
    
    if { $mname == "Functional" } { return [ vtkIdentityTransform New ] }
    
    set tlist [ list [ list 0 $parameters($this,distortionTransform) ] ]
    if { $mname == "Anatomical" || $mname == "Reference" } {
	lappend tlist [ list 0 $parameters($this,internalTransform) ]
    }
    if { $mname == "Reference" } {
	lappend tlist [ list $parameters($this,inverseReferenceTransform) $parameters($this,referenceTransform) ]
    }
    
    for { set i 0 } { $i < [ llength $tlist ] } { incr i } {
	set it [ lindex $tlist $i ]
	puts stdout "it($i) = $it "
    }
    
    set gen [ vtkGeneralTransform New ]
    $gen Identity
    $gen PostMultiply
    
    
    puts stdout "Transformation list = $tlist"


    for { set i 0 } { $i < [ llength $tlist ] } { incr i } {
	set pair [ lindex $tlist $i ]
	set first [ lindex $pair 0 ];	
	set second [ lindex $pair 1 ]
	
	set ok 0
	set doinvert 0
	set tr [ [ pxitcltransform \#auto ] GetThisPointer ]
	if { $first !=0 && $first != "" } {
	    set ok [ $tr Load $first ]
	    if { $ok > 0 } {
		puts stdout "\n\n Loading $i first = $first (ok=$ok), [ file size $first ]"
	    }
	}
		
	
	if { $ok == 0 } {
	    if { $second !=0 && $second != "" } {
		set ok	[ $tr Load $second ]
		puts stdout "Loading $i second = $second ($ok)"
		if { $ok > 0 } {
		    puts stdout "Inverting $second [ file size $second ]"
		    $tr Invert
		    
		    if { $i == 2 } {
			$tr Save
			set parameters($this,inverseReferenceTransform) [ $tr cget -filename ]
			set parameters($this,display_inverseReferenceTransform) [file tail [ $tr cget -filename ] ]
		    }
		}
	    }
	}
	
#	if { $ok == 1 } {
#	    if { [ $tr IsIdentity ] } {
#		set ok 0
#		puts stdout "Filename [ $tr GetFileName ]"
#	    }
#	}
	
	if { $ok == 1 } {
	    $gen Concatenate [ $tr GetTransformation ]
	} else {
	    puts stdout "Bad Transformation Set $tlist (or identity)"
	    $gen Delete
	    return 0
	}
	itcl::delete obj $tr
    }
    
    return $gen
}


# ----------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::CheckVOIImageSize { voi_image funcimg { graywhitemaskmode 0 } } {
	

	if { $parameters($this,voiDefinitionSpace) == "Functional" && $graywhitemaskmode == 0  } {
	if { [ $voi_image GetImageSize ] != [ $funcimg GetImageSize ] } {
		puts stdout "Bad VOI Image for Functional Space"
		return 0
	} else	{
		return 1
	}
	}

	set mname  $parameters($this,voiDefinitionSpace) 
	if { $graywhitemaskmode > 0 } {
	set mname "Reference"
	}
	set c_image [  [ pxitclimage \#auto ] GetThisPointer ]

	if { $mname == "Conventional"  } {
	set ok [ $c_image Load $parameters($this,conventional) ]
	} elseif { $mname == "Anatomical" } {
	set ok [ $c_image Load $parameters($this,anatomical) ]
	} elseif { $mname == "Reference" } {
	set ok [ $c_image Load $parameters($this,reference_brain_path) ]
	}

	if { $ok == 0 } {
	puts stdout "Failed to LOAD $mname Image"
	} elseif { [ $voi_image GetImageSize ] != [ $c_image GetImageSize ] } {
	puts stdout "Bad VOI or GrayWhiteMask Image for $mname Space"
	set ok 0
	}
	
	itcl::delete obj $c_image
	return $ok
}
# ----------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::GetReslicedVOIImage { funcimg { graywhitemaskmode 0 } } {

    set vname $parameters($this,voiDefinitionImage) 
    set vtname "VOI"
    if { $graywhitemaskmode > 0 } {
	set vname $parameters($this,reference_gw_map)
	set vtname "Gray/White/CSF"
    }
    
    set voi_image [ [ pxitclimage \#auto ] GetThisPointer ]
    
    set ok [ $voi_image Load $vname ]
    
    
    if { $ok == 0 } {
	puts stdout "Failed to LOAD $vtname  Image = $vname"
	return [ list 0	 0 ]
    }
    puts stdout "Loaded voi image from $vname  [ $voi_image GetShortDescription ]"
    
    set r [ [ [ [ $voi_image GetImage ] GetPointData ] GetScalars  ] GetRange ]
    set rmin [ lindex $r 0 ]
    set oldrange [ lindex $r 1 ]
    
    puts stdout "image range $rmin $oldrange"
    
    set ok [ $this CheckVOIImageSize $voi_image $funcimg $graywhitemaskmode ]
    puts stdout "Checked $vtname image size\n"
    
    if { $ok == 0 } {
	itcl::delete obj $voi_image 
	return [ list 0 0 ]
    }
    
    puts stdout "Checking transformations\n"
    
    set tr [ $this CheckTransformationsForVOI  $graywhitemaskmode ]
    if { $tr == 0 } {
	puts stdout "Transformation Check Failed"
	itcl::delete obj $voi_image 
	return [ list 0 0 ]
    }
    
    puts stdout "Reslicing $vtname Image, transformation type=[ $tr GetClassName ] input range=$rmin:$oldrange\n"
    
    # else tr is now the transformation
    set resl [ vtkbisImageReslice New ]

 #   puts stdout "EPI = [ [ $funcimg GetImage ] GetDimensions ]"
    $resl SetInformationInput [ $funcimg GetImage ]
#    puts stdout "VOI = [ [ $voi_image GetImage ] GetDimensions ]"
    $resl SetInput [ $voi_image GetImage ]
    $resl SetResliceTransform  $tr
    $resl SetInterpolationMode 0
    $resl SetBackgroundLevel 0
#    $resl OptimizationOff
    $resl Update
    
    $voi_image ShallowCopyImage [ $resl GetOutput ]
    $resl Delete
    
    
    set r  [ [ [ [ $voi_image GetImage ] GetPointData ] GetScalars ] GetRange ]
    if { [ lindex $r 0 ] == [ lindex $r 1 ] } {
	puts stdout "Reslicing $vtname failed, range= $r"
	itcl::delete obj $voi_image 
	return [ list 0 0 ]
    }
    
    puts stdout "Cleanup and return\n\n voi_image [ $voi_image GetShortDescription ] output range= [ [ [ [ $voi_image GetImage ] GetPointData ] GetScalars ] GetRange ]\n\n"
    
    $tr Delete
    
    return [ list $voi_image $oldrange ]
}

#  Main Function
#  
#  mode = 0   seed to image
#  mode = 1   seed to seed
# ----------------------------------------------------------------------------------------


itcl::body bis_fmrisetup::ComputeSeedCorrelationSingle { mode inputimage maskimage filename {dosave 1 } } {

	set lst	  [ $this GetReslicedVOIImage $inputimage ] 
	set vimg [ lindex $lst 0  ]
	set oldrange [ lindex $lst 1 ]
		   
	if { $vimg == 0 } {
	return 0
	}

	set tmpname [ file rootname $filename ]
	if { [ file extension $tmpname ] == ".nii" } { set tmpname [ file rootname $tmpname ] }

	if { $dosave == 1 } {
		set tmpname3 ${tmpname}_voi.nii.gz
		$vimg Save $tmpname3
		puts stdout "Resliced VOI image saved in $tmpname3"
		
		set tmpname4 ${tmpname}_mask.nii.gz
		$maskimage Save $tmpname4
		puts stdout "Resliced mask image saved in $tmpname4"
		
		#set tmpname2 ${tmpname}_preprocessedinput.nii.gz
		#$inputimage Save $tmpname2
		#puts stdout "Saved in $tmpname2"
	}
	
	puts stdout "\n\n\n We are doing seed correlation. inputimage = [ $inputimage GetDescription ]\n, mask = [ $maskimage GetDescription ]\n, output filename = $filename"

	set roimean_alg	 [ bis_roimean \#auto ] 
	$roimean_alg InitializeFromContainer 0 $this
	$roimean_alg SetInput $inputimage
	$roimean_alg SetSecondInput $vimg
	$roimean_alg SetOptionValue dotextfile 1
	$roimean_alg SetOptionValue filename ${tmpname}_roimean.txt
	$roimean_alg SetOptionValue filename2 ${tmpname}_xyzCoordinate.txt
	set r [ [ [ [ $vimg GetImage ] GetPointData ] GetScalars  ] GetRange ]
	set newrange [ lindex $r 1 ]
	if { $newrange != $oldrange } {
		puts stdout "add new region [ expr $oldrange - $newrange ]"
	$roimean_alg SetOptionValue addregion [ expr $oldrange - $newrange ]
	} 
	$roimean_alg Execute
	
	if { $dosave == 1 } {
	set tmpname1 ${tmpname}_roimean.nii.gz	
	[ $roimean_alg GetOutput ] Save $tmpname1
	puts stdout "ROI Mean saved in $tmpname1"
	}
	
	
	
	if { $mode == 0 } {
	set corr [ bis_seedcorrelation \#auto ]
	$corr InitializeFromContainer 0 $this
	$corr SetInput $inputimage  
	$corr SetOptionValue raw $parameters($this,resting_rawcorrelation)
	$corr SetSecondInput [ $roimean_alg GetOutput ]
	$corr SetInputObject third_image $maskimage
	$corr SetOptionValue usemask 1
	$corr SetOptionValue ztransform $parameters($this,resting_ztransform)
	$corr Execute
	} else {
	set corr [ bis_matrixcorrelation \#auto ]
	$corr InitializeFromContainer 0 $this
	$corr SetOptionValue raw $parameters($this,resting_rawcorrelation)
	$corr SetInput [ $roimean_alg GetOutput ]
	$corr SetOptionValue dotextfile 1
	$corr SetOptionValue filename ${tmpname}_matrix.txt
	$corr SetOptionValue filename2 ${tmpname}_outputForConnectivityViewer.txt
	$corr Execute
	}
	
	[ $corr GetOutput ] Save $filename
	
	itcl::delete obj $corr
	itcl::delete obj $roimean_alg

}
itcl::body bis_fmrisetup::TaskBasedProcessing { task image maskimage vimg outname { addrange 0 } } {
	#-----------get the block/task matrix------------------
    set matrix [ $this CreateBaseGLMMatrix 0 ]
	if { $matrix == 0 } {
	return 0
	}
	set tmpname [ file rootname $outname ]
	if { [ file extension $tmpname ] == ".nii" } { set tmpname [ file rootname $tmpname ] }
	
	set matname "${tmpname}_blocks.matr"
	$matrix Save $matname

	set cal [ vtkbisTaskBasedSeedCorrelation New ]
	$cal SetInput [ $image GetImage ]
	$cal SetImageROI [ $vimg GetImage ]
	$cal SetImageMask [ $maskimage GetImage ]
	$cal SetMatrix $matrix
	$cal SetTask $task
	$cal SetAddRegion $addrange
	$cal SetDoText 1
	set roimeanFilename "${tmpname}_roimean.txt"
	$cal SetFileName $roimeanFilename
	$cal Update
	
	set mp [ pxitclimage \#auto ]
	$mp ShallowCopyImage [ $cal GetOutput ]
	$mp Save $outname
	pxtkprint "Average run image saved in [ $mp GetShortDescription ]"
	itcl::delete object $mp
	$cal Delete
}
# ----------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::TaskBasedComputeSeedCorrelation { { askfname 0 } } {
	puts stdout "We are doing task based seed correlation."
	set ln [ $this GenerateOutputFilenames $askfname ]
	if { $ln == 0 } {
	return 0
	}
	set outnamestem [ lindex $ln 0 ]
	set msbsetupname [ lindex $ln 1 ]
	catch { file mkdir $parameters($this,output_dir) }
	set ok [ $this IsWritable $outnamestem ]
	if { $ok == 0 } {
		$this InfoMessage "You don't have write permission on $outnamestem. Calculation stops." "Error"
		return 1
	}
	pxtkprint "Beginning Stem =$outnamestem\n\t Setup=$msbsetupname\n"
	set extraname "seed"
	set setuptasklist ""
	
	#---------------Get mask------------------------------------ 
	set img	 [  [ pxitclimage \#auto ] GetThisPointer ]
	set whitecsfmask 0
	set fn	$parameters($this,run_file_1)
	set ok [ $img Load $fn ]
	if { $ok > 0 } {
	set maskimage [ $this CreateMask $img $parameters($this,resting_masklevel) ]
	if { $parameters($this,resting_usecsfmean) > 0 } {
		puts stdout "we are doing csf correction all runs"
		#
		# Note (1) at end this switches to gray/white mask instead of VOI Image
		# 
		set whitecsfmask [ lindex [ $this GetReslicedVOIImage $img 1 ] 0 ]
		set tmpname2 ${outnamestem}_allruns_whitecsfmask.nii.gz
		$whitecsfmask Save $tmpname2
		puts stdout "Resliced csf/white matter mask image saved in $tmpname2"
		}
	}
	itcl::delete obj $img
	#---------------Geberate giant 4D image----------------------

	# The 1 1 0 enable graywhitemask, enable checkformotioncorrection and disableskipframes
	# The skip frames here are dealt with in the creation of the GLM Matrix
	set allrunsimage [ $this ConcatenateAndPreprocessRuns $maskimage $whitecsfmask 1 0	]

	if { $allrunsimage == 0 } {
	tk_messageBox -type ok -title "Error ..." -message "Failed to Concatenate Runs"
	return 0
	}
	#----------get ROI image-------------------------------
	
	set lst	  [ $this GetReslicedVOIImage $allrunsimage ] 
	set vimg [ lindex $lst 0  ]
	set oldrange [ lindex $lst 1 ]
	if { $vimg == 0 } {
	return 0
	}
	set addrange 0
	set r [ [ [ [ $vimg GetImage ] GetPointData ] GetScalars  ] GetRange ]
	set newrange [ lindex $r 1 ]
	if { $newrange != $oldrange } {
	puts stdout "add new region [ expr $oldrange - $newrange ]"
	set addrange [ expr $oldrange - $newrange ]
	} 
	
	#---------------Calculate for each task-----------------------
	for { set task 0 } { $task < $numtasks } { incr task } {
	# ------Generate output filenames for computation --
	set taskcount [ expr $task + 1 ]
	set f $parameters($this,task_$taskcount) 
	set tname ${extraname}_task${f}
	
	set outname   ${outnamestem}_$tname.nii.gz	
	lappend setuptasklist [ list $tname $tname $outname ]
	
	$this TaskBasedProcessing $task $allrunsimage $maskimage $vimg $outname $addrange
	
	}
	itcl::delete obj $allrunsimage
	itcl::delete obj $maskimage
	pxtkprint "Done Saving .. clean up time\n"
	$this CreateMSBFile $msbsetupname $setuptasklist
	
	# -----------------------------------------------------------------------------------
	
	# Clean up

	# --------------------------------- Report ------------------------------
	$this InfoMessage "Done Task based Seed Correlation -- see single subejct file in $msbsetupname" "FYI"
	$this DisplayMSBFile $msbsetupname
	
	return $outnamestem
}
# ----------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::ComputeSeedCorrelation { mode { askfname 0 } { report 0 }  } { 
	# ---------------- Generate output filenames/directories --------------
	
	set ln [ $this GenerateOutputFilenames $askfname ]
	if { $ln == 0 } {
	return 0
	}
	set outnamestem [ lindex $ln 0 ]
	set msbsetupname [ lindex $ln 1 ]
	catch { file mkdir $parameters($this,output_dir) }
	set ok [ $this IsWritable $outnamestem ]
	if { $ok == 0 } {
		$this InfoMessage "You don't have write permission on $outnamestem. Calculation stops." "Error"
		return 1
	}

	if { $mode == 1 } {
	set extraname "matrix"
	} else {
	set extraname "seed"
	}
	
	pxtkprint "Beginning Stem =$outnamestem\n\t Setup=$msbsetupname\n"

	# ---------------- calculate each run individually -------------------
	set setuptasklist ""

	# Get Average as well 

	if { $parameters($this,resting_individualruns) > 0 } {
	set tname "${extraname}_average"
	set meanoutname	  ${outnamestem}_$tname.nii.gz	
	lappend setuptasklist [ list $tname $tname $meanoutname ]
	set whitecsfmask 0
	
	set imgnamelist ""
	
	for { set runcount 1 } { $runcount <= $numruns } { incr runcount } {
		set img	 [  [ pxitclimage \#auto ] GetThisPointer ]
		set motionmatrixname 0
		set fn	$parameters($this,run_file_$runcount)
		set ok [ $img Load $fn ]
		
		# provide motion matrix file location
		if { $parameters($this,resting_usemotionparams) > 0 } {
			set motionmatrixname $parameters($this,run_motionfile_$runcount)
		}
		
		if { $ok > 0 } {
			if { $runcount ==1 } {
				if { $parameters($this,resting_usecsfmean) > 0 } {
					puts stdout "we are doing csf correction individually"
					#
					# Note (1) at end this switches to gray/white mask instead of VOI Image
					#
					set whitecsfmask [ lindex [ $this GetReslicedVOIImage $img 1 ] 0 ]
					set tmpname2 ${outnamestem}_whitecsfmask.nii.gz
					$whitecsfmask Save $tmpname2
					puts stdout "Resliced csf/white matter mask image saved in $tmpname2"
					}
			}
		set msk [ $this CreateMask $img $parameters($this,resting_masklevel) ]
		
		# Handle skip frames here 
		set runskipframes 0
		if { $parameters($this,runskip_$runcount) == 1	|| $parameters($this,checks) > 0 } {
			set runskipframes "$parameters($this,skipFrames) $parameters($this,skiprunsframe_$runcount)"
		} 
		puts stdout "\t\t\t\t skipframes for this run = $runskipframes"
		
		
		# Yes to motion matrix and graywhite mask
		set tmp [ $this PreprocessSingle4DImage $img $msk $runskipframes $motionmatrixname $whitecsfmask ]
		set tname "${extraname}_$runcount"
		set outname ${outnamestem}_$tname.nii.gz	
		lappend setuptasklist [ list $tname $tname $outname ]
		$this ComputeSeedCorrelationSingle $mode $tmp $msk $outname
		lappend imgnamelist $outname
		
		#set tmpname3 ${outnamestem}_${tname}_input.nii.gz
		#$tmp Save $tmpname3
		#puts stdout "Saved in $tmpname3"
		
		itcl::delete obj $img
		itcl::delete obj $tmp
		itcl::delete obj $msk
		} else {
		tk_messageBox -type ok -title "Error ..." -message "Failed to Load  Run $runcount ($fn)"
		itcl::delete obj $img 
		return 0
		}
	}
	catch { itcl::delete obj $whitecsfmask }
	
	# Load all images in imgnamelist, compute average and save in meanoutname ....
	#
	#
	set combofmri [	 vtkpxAverageImages [ pxvtable::vnewobj ]]
	$combofmri SetMedian 0
	for { set index 0 } { $index < $numruns } { incr index } {
		set mp($index) [ pxitclimage \#auto ]
		$mp($index) Load [ lindex $imgnamelist $index ]
		pxtkprint "Adding image [ $mp($index) GetShortDescription ]\n"
		$combofmri AddInput [ $mp($index) GetImage ]
	}
	$combofmri Update
	
	$mp(0) ShallowCopyImage [ $combofmri GetOutput ]
	$mp(0) Save $meanoutname
	pxtkprint "Average run image saved in [ $mp(0) GetShortDescription ]"
	
	for { set index 0 } { $index < $numruns } { incr index } {
		itcl::delete object $mp($index)
	}
	# ---------------- Generate giant 4D Image and mask -------------------
	} else {
	set whitecsfmask 0
	set img	 [  [ pxitclimage \#auto ] GetThisPointer ]
	set fn	$parameters($this,run_file_1)
	set ok [ $img Load $fn ]
		if { $ok > 0 } {
			set maskimage [ $this CreateMask $img $parameters($this,resting_masklevel) ]
			if { $parameters($this,resting_usecsfmean) > 0 } {
				puts stdout "we are doing csf correction all runs"
				#
				# Note (1) at end this switches to gray/white mask instead of VOI Image
				# 
				set whitecsfmask [ lindex [ $this GetReslicedVOIImage $img 1 ] 0 ]
				set tmpname2 ${outnamestem}_allruns_whitecsfmask.nii.gz
				$whitecsfmask Save $tmpname2
				puts stdout "Resliced csf/ white matter mask image saved in $tmpname2"
				}
		}
	
	itcl::delete obj $img

	# enable gray/csf GLM, enable motion GLM, enable skip frames
	set allrunsimage [ $this ConcatenateAndPreprocessRuns $maskimage $whitecsfmask 1 1 ]
	if { $allrunsimage == 0 } {
		tk_messageBox -type ok -title "Error ..." -message "Failed to Concatenate Runs"
		return 0
	}
	# ------------- Generate output filenames for computation ------------------
	set tname "${extraname}"
	set outname   ${outnamestem}_${extraname}.nii.gz	
	lappend setuptasklist [ list $tname $tname $outname ]
	$this ComputeSeedCorrelationSingle $mode $allrunsimage $maskimage $outname
	
	itcl::delete obj $allrunsimage
	itcl::delete obj $maskimage
	catch { itcl::delete obj $whitecsfmask }
	}
	
	pxtkprint "Done Saving .. clean up time\n"
	$this CreateMSBFile $msbsetupname $setuptasklist

	# -----------------------------------------------------------------------------------
	
	# Clean up
	
	# --------------------------------- Report ------------------------------
	$this InfoMessage "Done Seed Correlation -- see single subejct file in $msbsetupname" "FYI"
	$this DisplayMSBFile $msbsetupname
	
	return $outnamestem
}



# ----------------------------------------------------------------------------------------
# Resting State algorithms
# ----------------------------------------------------------------------------------------

itcl::body bis_fmrisetup::ComputeIntrinsicCorrelationSingle { mode allrunsimage maskimage outname } {
	
    if { $mode == "Complex" } {
	return [ $this ComputeComplexIntrinsicCorrelationSingle $mode $allrunsimage $maskimage $outname ]
    }

    set tmpname [ file rootname $outname ]
    if { [ file extension $tmpname ] == ".nii" } { set tmpname [ file rootname $tmpname ] }
    
    set tmpname4 ${tmpname}_mask.nii.gz
    $maskimage Save $tmpname4
    puts stdout "Resliced mask image saved in $tmpname4"
    
    puts stdout "\n\n\n We are doing intrinsic correlation. Mode= $mode, inputimage = [ $allrunsimage GetDescription ]\n, mask = [ $maskimage GetDescription ]\n, output filename = $outname "
    
    set con [ bis_intrinsicconnectivity \#auto ]
    $con InitializeFromContainer 0 $this
    $con SetInput $allrunsimage
    $con SetSecondInput $maskimage
    $con SetOptionValue usemask 1
    $con SetOptionValue mode $mode
    #$con SetOptionValue raw $parameters($this,resting_rawcorrelation)
    $con SetOptionValue threshold $parameters($this,resting_threshold)
    $con SetOptionValue calculate $parameters($this,resting_mode)
    $con SetOptionValue range     $parameters($this,resting_range)
    #$con SetOptionValue filename  ${tmpname}_MeanAndStd.txt
    $con Execute
    
    
    [ $con	GetOutput ] Save $outname
    #puts stdout "Output saved in [ [ $con  GetOutput ] GetDescription ]"
    
    if { $parameters($this,resting_threshold) != 0.0 } {
	set outname2 ${tmpname}_Degree.nii.gz
	[ $con GetSecondOutput ] Save $outname2
    }
    
    
    
    itcl::delete obj $con
}

# -------------------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::ComputeComplexIntrinsicCorrelationSingle { mode allrunsimage maskimage outname } {
	

# Dustin Modify this

	set tmpname [ file rootname $outname ]
	if { [ file extension $tmpname ] == ".nii" } { set tmpname [ file rootname $tmpname ] }

	set tmpname4 ${tmpname}_mask.nii.gz
	$maskimage Save $tmpname4
	puts stdout "Resliced mask image saved in $tmpname4"

	puts stdout "\n\n\n We are doing intrinsic correlation. Mode= $mode, inputimage = [ $allrunsimage GetDescription ]\n, mask = [ $maskimage GetDescription ]\n, output filename = $outname "
	
	set con [ bis_intrinsicconnectivity \#auto ]
	$con InitializeFromContainer 0 $this
	$con SetInput $allrunsimage
	$con SetSecondInput $maskimage
	$con SetOptionValue usemask 1
	$con SetOptionValue mode $mode
	#$con SetOptionValue raw $parameters($this,resting_rawcorrelation)
	$con SetOptionValue threshold $parameters($this,resting_threshold)
	$con SetOptionValue calculate $parameters($this,resting_mode)
	$con SetOptionValue range     $parameters($this,resting_range)
	#$con SetOptionValue filename  ${tmpname}_MeanAndStd.txt
	$con Execute
	

	[ $con	GetOutput ] Save $outname
	#puts stdout "Output saved in [ [ $con  GetOutput ] GetDescription ]"
	
	if { $parameters($this,resting_threshold) != 0.0 } {
			set outname2 ${tmpname}_Degree.nii.gz
			[ $con GetSecondOutput ] Save $outname2
		}
	
	
	
	itcl::delete obj $con
}
# --------------------------------------------------------------------------------------------


itcl::body bis_fmrisetup::ComputeIntrinsicCorrelation { mode { askfname 0 } {  report 0 } }  {
	
	# ---------------- Generate output filenames/directories --------------
	
	set ln [ $this GenerateOutputFilenames $askfname ]
	if { $ln == 0 } {
	return 0
	}

	set outnamestem [ lindex $ln 0 ]
	set msbsetupname [ lindex $ln 1 ]
	catch { file mkdir $parameters($this,output_dir) }
	set ok [ $this IsWritable $outnamestem ]
	if { $ok == 0 } {
		$this InfoMessage "You don't have write permission on $outnamestem. Calculation stops." "Error"
		return 1
	}
	
	if { $mode == "Ipsilateral" } {
	set extraname "ipsilateral"
	} elseif { $mode == "Whole" } {
	set extraname "whole"
	} else {
	set extraname "contralateral"
	}
	
	pxtkprint "Beginning Stem =$outnamestem\n\t Setup=$msbsetupname\n"
	
	#----------------------Calculating--------------------------------------
	
	set setuptasklist ""
	
	if { $parameters($this,resting_individualruns) > 0 } {
	# ----------------Do each run individually -------------------
	set tname "${extraname}_average"
	set meanoutname	  ${outnamestem}_$tname.nii.gz	
	lappend setuptasklist [ list $tname $tname $meanoutname ]
	set imgnamelist ""
	set msk 0
	set whitecsfmask 0
	
	for { set runcount 1 } { $runcount <= $numruns } { incr runcount } {
		set img	 [  [ pxitclimage \#auto ] GetThisPointer ]
		set motionmatrixname 0
		set fn	$parameters($this,run_file_$runcount)
		set ok [ $img Load $fn ]
		
		if { $parameters($this,resting_usemotionparams) > 0 } {
		set motionmatrixname $parameters($this,run_motionfile_$runcount)
		}
		
		if { $ok > 0 } {
		if { $runcount == 1 } {
			if { $parameters($this,resting_usermask) } {
			set msk [ lindex [ $this GetReslicedVOIImage $img ]  0 ]
				if { $mode != "Whole" } {
					set r [ [ [ [ $maskimage GetImage ] GetPointData ] GetScalars  ] GetRange ]
					set range [ lindex $r 1 ]
					if { $range != 200.0 } {
						$this InfoMessage "Invalid mask. User needs to provide a mask with 0 = background, 100 = right brain, 200 = left brain" "Error"
						return 1
					}
				}
			} else {
				if { $mode == "Whole" } {
					set maskimage [ $this CreateMask $img $parameters($this,resting_masklevel) ]
				} else {
					$this InfoMessage "Invalid mask. User needs to provide a mask with 0 = background, 100 = right brain, 200 = left brain" "Error"
					return 1
				}
			}

			if { $parameters($this,resting_usecsfmean) > 0 } {
			puts stdout "we are doing csf correction individually"
			#
			# Note (1) at end this switches to gray/white mask instead of VOI Image
			#
			set whitecsfmask [ lindex [ $this GetReslicedVOIImage $img 1 ] 0 ]
			set tmpname2 ${outnamestem}_whitecsfmask.nii.gz
			$whitecsfmask Save $tmpname2
			puts stdout "Resliced csf/white matter mask saved in $tmpname2"
			}
		}
		
		set runskipframes 0
		if { $parameters($this,runskip_$runcount) == 1	|| $parameters($this,checks) > 0 } {
			set runskipframes "$parameters($this,skipFrames) $parameters($this,skiprunsframe_$runcount)"
		} 
		puts stdout "\t\t\t\t skipframes for this run = $runskipframes"
		
		# Yes to motion and graywhitemask
		set tmp [ $this PreprocessSingle4DImage $img $msk $runskipframes $motionmatrixname $whitecsfmask ]
		set tname "${extraname}_$runcountset_$parameters($this,resting_mode)_$parameters($this,resting_range)"
		set outname ${outnamestem}_$tname.nii.gz
		lappend setuptasklist [ list $tname $tname $outname ]
		$this ComputeIntrinsicCorrelationSingle $mode $tmp $msk $outname
		lappend imgnamelist $outname
		
		#set tmpname3 ${outnamestem}_${tname}_input.nii.gz
		#$img Save $tmpname3
		#puts stdout "Saved in $tmpname3"
		
		itcl::delete obj $img
		itcl::delete obj $tmp
		} else {
		tk_messageBox -type ok -title "Error ..." -message "Failed to Load  Run $runcount ($fn)"
		itcl::delete obj $img 
		return 0
		}
	}
	catch { itcl::delete obj $msk }
	catch { itcl::delete obj $whitecsfmask }
	# Load all images in imgnamelist, compute average and save in meanoutname ....
	#
	#
	set combofmri [	 vtkpxAverageImages [ pxvtable::vnewobj ]]
	$combofmri SetMedian 0
	for { set index 0 } { $index < $numruns } { incr index } {
		set mp($index) [ pxitclimage \#auto ]
		$mp($index) Load [ lindex $imgnamelist $index ]
		pxtkprint "Adding image [ $mp($index) GetShortDescription ]\n"
		$combofmri AddInput [ $mp($index) GetImage ]
	}
	$combofmri Update
	
	$mp(0) ShallowCopyImage [ $combofmri GetOutput ]
	$mp(0) Save $meanoutname
	pxtkprint "Average run image saved in [ $mp(0) GetShortDescription ]"
	
	for { set index 0 } { $index < $numruns } { incr index } {
		itcl::delete object $mp($index)
	}
    } else {
	set whitecsfmask 0
	# ---------------- Generate giant 4D Image and mask -------------------
	set img	 [  [ pxitclimage \#auto ] GetThisPointer ]
	set fn	$parameters($this,run_file_1)
	set ok [ $img Load $fn ]
	if { $ok > 0 } {
		if { $parameters($this,resting_usermask) } {
		set maskimage [ lindex [ $this GetReslicedVOIImage $img ]  0]
			if { $mode != "Whole" } {
				set r [ [ [ [ $maskimage GetImage ] GetPointData ] GetScalars  ] GetRange ]
				set range [ lindex $r 1 ]
				puts stdout "User provided mask max value:  $range"
					if { $range != 200.0 } {
						$this InfoMessage "Invalid mask. User needs to provide a mask with 0 = background, 100 = right brain, 200 = left brain" "Error"
						return 1
					}
				}
		} else {
			if { $mode == "Whole" } {
				set maskimage [ $this CreateMask $img $parameters($this,resting_masklevel) ]
			} else {
				$this InfoMessage "Invalid mask. User needs to provide a mask with 0 = background, 100 = right brain, 200 = left brain" "Error"
				return 1
			}
		}
		
		if { $parameters($this,resting_usecsfmean) > 0 } {
		puts stdout "we are doing csf correction all runs"
		#
		# Note (1) at end this switches to gray/white mask instead of VOI Image
		# 
		set whitecsfmask [ lindex [ $this GetReslicedVOIImage $img 1 ] 0 ]
		set tmpname2 ${outnamestem}_allruns_whitecsfmask.nii.gz
		$whitecsfmask Save $tmpname2
		puts stdout "Resliced csf/white matter mask saved in $tmpname2"
		}
	}
	
	itcl::delete obj $img
	
	#set tmpname4 ${outnamestem}_intrimask.nii.gz
	#$maskimage Save $tmpname4
	#puts stdout " Saved [ $maskimage GetDescription ]"
	
	set allrunsimage [ $this ConcatenateAndPreprocessRuns $maskimage $whitecsfmask 1 1 ]
	if { $allrunsimage == 0 } {
		tk_messageBox -type ok -title "Error ..." -message "Failed to Concatenate Runs"
		return 0
	}
	#set tmpname1 ${outnamestem}_intrinput.nii.gz
	#$allrunsimage Save $tmpname1
	#puts stdout "Preprocessed and concatenated input image [$allrunsimage GetDescription ]\n saved in $tmpname1"
	
	# ------------- Generate output filenames for computation ------------------
	set tname "${extraname}_$parameters($this,resting_mode)_$parameters($this,resting_range)"
	set outname ${outnamestem}_$tname.nii.gz
	lappend setuptasklist [ list $tname $tname $outname ]
		
	$this ComputeIntrinsicCorrelationSingle $mode $allrunsimage $maskimage $outname
	itcl::delete obj $allrunsimage
	itcl::delete obj $maskimage
	}
	
	pxtkprint "Done Saving .. clean up time\n"
	$this CreateMSBFile $msbsetupname $setuptasklist
	
	# -----------------------------------------------------------------------------------
	# Clean up
	# --------------------------------- Report ------------------------------
	$this InfoMessage  "Done Intrinsic Connectivity -- see single subejct file in $msbsetupname" "FYI"
	$this DisplayMSBFile $msbsetupname
	
	return $outnamestem
}


# ----------------------------------------------------------------------------------------
# Data Tree Stuff
# ----------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::CreateDataTreeStructure { { tasklist "" } { doupdate 1 } } {
	
	if { $datamanager == 0 } {
	if { $treewidget !=0 } {	
		set datamanager [ $treewidget GetDataManager ]
	} else {
		set datamanager [ [ bis_datamanager \#auto ] GetThisPointer ]
	}
	}

	$datamanager CleanXML
	set rootnode [ $datamanager GetRootNode ]

	set ref [ eval "$datamanager CreateChildElement $rootnode Image Reference_Image { Filename $parameters($this,reference_brain_path)}" ]
	set subjectdata [ $datamanager CreateChildElement $ref Subject "S-$parameters($this,subject_id)" {  Expanded "1"  IsIdentity 1 } ]
	
	set aname $parameters($this,anatomical)
	set cname $parameters($this,conventional)
	set ename ""
	set xname $parameters($this,referenceTransform)
	set iname $parameters($this,internalTransform)
	set dname $parameters($this,distortionTransform)

	set anat [ eval "$datamanager CreateChildElement $subjectdata Image \"3D_Anatomical_Image\" { Filename \"$aname\"   TransformationFilename \"$xname\" Expanded 1 }" ]
	set conv [ eval "$datamanager CreateChildElement $anat Image \"Conventional_Image\" { Filename \"$cname\"  TransformationFilename \"$iname\" Expanded 1 }" ]
	set epi [ eval "$datamanager CreateChildElement $conv Image  \"Echoplanar_Image\" { Filename \"$ename\"	 TransformationFilename \"$dname\" Expanded 1 } " ]
	set func [ $datamanager CreateChildElement $epi "Folder" "Input Runs" { Expanded 1 } ]

	for { set runcount 1 } { $runcount <= $numruns } { incr runcount } {
	set fn	$parameters($this,run_file_$runcount)
	eval "$datamanager CreateChildElement $func Image \"Run_${runcount}\" { Filename \"$fn\"   IsIdentity 1 } "
	}

	set outdata [ $datamanager CreateChildElement $epi "Folder" Outputs { IsIdentity 1  Expanded "1" } ]

	set voiname $parameters($this,voiDefinitionImage)
	set vspace $parameters($this,voiDefinitionSpace)
	set vtransform $parameters($this,inverseReferenceTransform)

	#    puts $fileid "#VOI Definition Space (Reference,Anatomical,Conventional,Functional)"
	if { $vspace == "Reference" } {
	set voi [ eval "$datamanager CreateChildElement $ref Image 0_VOI_Image { Filename $voiname IsIdentity 1 } " ]
	$anat SetAttribute InverseTransformationFilename $vtransform
	}  else {
	switch -exact  $vspace {
		"Anatomical"   {  set par $anat }
		"Conventional" { set par $conv }
		"Functional"   { set par $epi  }
	}
	set voi [ eval "$datamanager CreateChildElement $par Image 0_VOI_Image { Filename \"$voiname\" IsIdentity 1 } " ]
	}


	set nt [ llength $tasklist ]
	if { $nt > 0 } {
	for { set j 0 } { $j < $nt  } { incr j } {
		set task [ lindex $tasklist $j ]
		set tname   [ lindex $task 0 ]
		set tsuffix [ lindex $task 1 ] 
		set tfname  [ lindex $task 2 ]
	    if { [ llength $tasklist ] < 10 } {
		set tname "T[ expr $j +1 ]-$tname"
	    } elseif { [ llength $tasklist ] < 100 } {
		set tname [ format "T%02d-$tname" [ expr $j +1 ] ]
	    } else {
		set tname [ format "T%03d-$tname" [ expr $j +1 ] ]
	    }
	    set t [ $datamanager CreateChildElement $outdata Image $tname ]
	    $t SetAttribute Filename $tfname
	    $t SetAttribute IsIdentity 1
	}
	}


	if { $treewidget !=0 && $doupdate == 1 } {
	$treewidget UpdateAll
	}

	return 1

}
# ----------------------------------------------------------------------------------------

itcl::body bis_fmrisetup::UpdateDataTreeStructure { { imagename "" } } {

	if { $treewidget  == 0 } { return 0 }
	$treewidget UpdateExpandedFlag
	return [ $this CreateDataTreeStructure "" 1 ]
}


 
# ----------------------------------------------------------------------------------------
itcl::body bis_fmrisetup::InfoMessage { msg { title "" } { icon info } } {

	if { [ $this HasGUI ] == 1 } {
	tk_messageBox -type ok -message $msg -title $title -icon $icon
	} else {
	puts stdout "$msg"
	}
}

# -----------------------------------------------------------------------------------------

itcl::body bis_fmrisetup::Usage {  } {

	set line "\n This is the BioImage Suite (www.bioimagesuite.org) v3.0 fmri tool\n"
	set line "$line\nbis_fmrisetup setupfilename procedure \[ second setup file \] \[ output_directory \]\n"
	set line "$line\nprocedure = \t	 glm \t\t --> execute general linear model analysis"
	set line "$line\n\t\t  seed \t\t --> execute seed to image connectivity analysis"
	set line "$line\n\t\t  matrix \t --> execute seed to seed connectivity analysis"
	set line "$line\n\t\t  taskseed \t\t --> execute task-based seed to image connectivity analysis"
	


	set line "$line\n\t\t  whole \t --> execute intrinsic connectivity (whole) analysis"
	set line "$line\n\t\t  ipsi \t\t--> execute intrinsic connectivity (ipsilateral) analysis"
	set line "$line\n\t\t  contra \t--> execute intrinsic connectivity (contralateral) analysis"
	
	set line "$line\n\n If second_setup file is specified then parameters in this ovveride original"
	set line "$line\n\n If ouput_directory is not specified then the value from the setup file will be used"
	return $line
}

itcl::body bis_fmrisetup::MainFunction { numarg arglist } {

	update idletasks
	global pxtcl_pref_array 

	if { $numarg <	1 } {
	    puts stdout [ $this Usage ]
	    return [ list 0 "Bad Arguments" ]
	}

	puts stdout "numarg=$numarg, arglist=$arglist"

	set pxtcl_pref_array(MirrorConsole) 1

	set f [ file normalize [lindex $arglist 0]]
	set ok [ $this LoadSetupFile  [ file normalize [lindex $arglist 0]]]
	if { $ok == 0 } {
	    puts stdout "Bad Setup File [ file normalize [lindex $arglist 0]]]\n"
	    return [ list 0 "Bad Setup File [ file normalize [lindex $arglist 0]]" ]
	}


	set count 2
	if { $numarg > $count } {
	    set temp [ lindex $arglist $count ]
	    if { $temp == "_do_not_use" } {
		incr count
	    } elseif { [ file extension $temp ] == ".xmlg" } {
		puts "Read this as a second file"
		$this LoadParameterFile $temp
		incr count
	    }
	}
		
	if { $numarg > $count } {
	    set outdir [ file normalize [ lindex $arglist $count ]]
	    puts stdout "Using outdir=$outdir"
	    if { [ file exists $outdir ] == 1 } {
		set ok [ file isdirectory $outdir ]
		if { $ok == 0 } {
		    puts stdout "Bad output directory $outdir"
		    return [ list 0 "Bad output directory $outdir" ]
		}
	    } else {
		catch { file mkdir $outdir } 
	    }
	    
	    if { [ file writable $outdir ] == 0 } {
		puts stdout "Output directory $outdir is not writable by the current user"
		return [ list 0 "Output directory $outdir is not writable by the current user" ]
	    }
	    
	    set parameters($this,output_dir) $outdir
	    puts stdout "Using $parameters($this,output_dir) as output directory"
	}

	if { $numarg > 1 } {
	set mode [ lindex $arglist 1 ]
	switch -exact $mode {
		"seed" {
		$this ComputeSeedCorrelation 0
		}
		"matrix" { 
		$this ComputeSeedCorrelation 1
		}
		"taskseed" {
		$this TaskBasedComputeSeedCorrelation 0
		}
		"whole" {
		$this ComputeIntrinsicCorrelation "Whole" 
		} 
	    "ipsi" {
		$this ComputeIntrinsicCorrelation "Ipsilateral" 
		}
		"contra" {
		$this ComputeIntrinsicCorrelation "Contralateral" 
		}
	    "complex" { 
		$this ComputeIntrinsicCorrelation "Complex" 
	    }
		"glm" {
		$this ComputeGLM 0
		}

	}
	}
	return 1
}


if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
	wm withdraw .
	set element [bis_fmrisetup \#auto ]
	$element MainFunction $argc $argv
	exit
}



