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









# 	$Id: pxmenu.tcl,v 1.9 2005/12/02 14:22:45 xenios Exp xenios $	

lappend auto_path [ file join [ file join [ file join  [ file dirname [ info script ]] .. ] base ]]
lappend auto_path [ file join [ file join [ file join  [ file dirname [ info script ]] .. ] main ]]

package require Iwidgets 
package require md5
package require pxtclutil 1.0


package require vtkpxcontrib 1.0
package require pxtcluserprefs 1.0
# -----------------------
# Dependencies and path
# -----------------------
lappend auto_path [ file dirname [ info script ]]

set location [ file normalize $::pxtclvtkpxcontrib::baselibrarypath  ]

set helpdialog 0

proc HelpCommand { } {

    global helpdialog

    if { $helpdialog != 0 } {
	::pxtclutil::ShowWindow $helpdialog
	return
    }

    set bname [ file normalize [ file join $::pxtclvtkpxcontrib::baselibrarypath  .. ] ]

    set b(1) [ file join $bname html ]
    set b(2) [ file join [ file join $bname .. ] build/html ]
    set b(3) [ file join [ file join $bname .. ] build64/html ]

    set i 1
    set rootname 0
    while { $i < 3 } {
	set f [ file normalize [ file join $b($i) index.html ] ]
#	puts stdout "Looking for $f"
	if { [ file exists $f ] > 0 } {
	    set i 4
	    set rootname $f
	}
	incr i
    }
    
    set w .
    set helpdialog  [ toplevel .[ pxvtable::vnewobj ]]
    wm geometry $helpdialog 800x600
    
    wm title $helpdialog "BioImage Suite Tools Manual Pages"
    
    set scr [ iwidgets::scrolledhtml $helpdialog.t  -labeltext "" \
		  -width 6i -height 3i \
		  -wrap word -padx 2 ]
    $scr configure -fontsize large
    
    pack $scr -side top -expand true -fill both
    #::bis_algorithm::SetWidgetTreeColors $scr "\#8197c5" white    
       
    if { $rootname != 0 } {
	$scr import $rootname
    } else {
	$scr insert end "No Man Pages Found"
    }
    
    ::pxtclutil::ShowWindow $helpdialog
}

proc MenuAboutCommand  { } { 
    set version $::pxtclvtkpxcontrib::version
    set aboutstring "(c) X. Papademetris, M. Jackowski, D. Scheinost, A. Joshi, I. Murphy, H.Okuda, R.T. Constable, L.H. Staib 1995-2011\nSection of Bioimaging Sciences, Department of Diagnostic Radiology, Yale University School of Medicine\n\n\nThis application is intended for research applications only! Use at your OWN RISK. If used for publication please acknowledge, see the webpage for more details. For additional support please visit the BioImage Suite forum at:\n\thttp://research.yale.edu/bioimagesuite/forum/\n " 
    tk_messageBox -title "About This Application" -type ok -message "$version\n $aboutstring"
}

proc MenuCheckForUpdates { } {
    ::pxtclvtkpxcontrib::CheckForUpdates
}


proc MenuWhereCommand { } {
    global location
    tk_messageBox -title "Path" -type ok -message "$location\n"
}


proc ReturnPath { app directory } {

    if { $app == "mpjdiffusiontool" || $app=="mpjvesseltool" || $app=="mpjfmmtool" } {
	return [ file join $directory [ file join .. [ file join "mjack" ] ] ]
    }

    if { $app == "bis_treeapp" } {
	return [ file join $directory [ file join .. [ file join "bis_algorithm" ] ] ]
    } 

    if { $app == "bis_dicom2nifti.tcl --dogui 1" } {
	return [ file join $directory [ file join .. [ file join "bis_algorithm" ] ] ]
    } 

    if { $app == "bis_varian2nifti.tcl --dogui 1" } {
	return [ file join $directory [ file join .. [ file join "bis_algorithm" ] ] ]
    } 


    if { $app =="biodatatree" } {
	return [ file join $directory [ file join .. [ file join "datatree" ] ] ]
    }



    return $directory
}

# -------------------------------------------------------------------------

proc CreateMenuPane { appname applabelname description basewidget directory } {
 

    set notebook [ iwidgets::tabnotebook $basewidget.top  -tabpos w -width 510 -angle 10 -gap 2 -margin 2 -raiseselect true]
    pack $notebook  -side top -expand t -fill both -padx 2 


    for { set k 0 } { $k < [llength $appname ]  } { incr k } {
	
	set app [ lindex $appname $k ]
	set applabel [ lindex $applabelname $k ]
	set des [ lindex $description $k ]
	set first [ string range $des 0 4 ]
	if { $first == "None:" } {
	    set name [ string range $des 5 end ]
	    set base [ $notebook add -label $name ]
	    if { $k ==0 } {
		$notebook view $name 
	    }
	} else {
	    set fr [ frame $base.$k ]
	    pack $fr -side top -expand f -fill both -pady 1
	    set dirname [ ReturnPath  $app $directory ]
	    set lc  [ file normalize [ file join "$dirname" $app ] ]

	    if { [ string first "--dogui" $lc ] == -1 } {
		append lc ".tcl"
		append app ".tcl"
	    }


	    frame $fr.top

	    set b [eval "button $fr.top.b -text \"$applabel\" -fg red -font { times 12 } -bg white -width 30 -command { catch { exec [info nameofexecutable] ${lc} & } }"]
	    set b2 [eval "button $fr.top.b2 -text \"?\"  -width 1 -command { tk_messageBox -type ok -message \"To execute this program on the command line: type ${app}\" -title \"Command Line Information\" -icon info} "]
	    pack $b2 -side right -expand false -padx 5
	    pack $b -side top -expand false -padx 20
	    
	    set a [ text $fr.t  -width 80 -height 2 -wrap word -relief flat ]
	    $a insert end $des
	    $a configure -state disabled
	    set c [ frame $fr.m -bg black -height 2 ]
	    pack $c -side bottom -expand f -fill x -pady 4
	    pack $fr.top -side top -expand true -fill x
	    pack  $a -side top -expand f -fill x -padx 2
	    
	}
    }
}

# -------------------------------------------------------------------------
proc CreateNewApps { basewidget } {

    set directory  [ file dirname [info script ]]

    set appname ""
    set applabelname ""
    set description ""

    lappend appname  "General Viewers"
    lappend applabelname  "General Viewers"
    lappend description  "None:General"

    lappend appname  "pxitclsimpleviewer"
    lappend applabelname  "Mosaic Viewer"
    lappend description "Simple Multi-slice/Multi-frame viewer which can display multiple slices at once."
    
    lappend appname  "pxitclorthoviewer"
    lappend applabelname  "Orthogonal Viewer"
    lappend description "Orthogonal 3D Viewer with functions for image pre-processing"

    lappend appname  "pxitclbrainregister"
    lappend applabelname  "Brain Register"
    lappend description "Coupled 3D Orthogonal viewers with access to image registration facilities"

    lappend appname  "pxitcldualmultisubject"
    lappend applabelname  "Dual MultiSubject"
    lappend description "Multiple (2) Group Comparisons"



    if { ( [ file exists [file join $directory "../mjack/mpjdiffusiontool.tcl" ] ] !=0 ) } {
	
	lappend appname "Diffusion Tensor/Angiography"
	lappend applabelname "Diffusion Tensor/Angiography"
	lappend description "None:DTI/Angiography"
	
	lappend appname "mpjdiffusiontool"
	lappend applabelname "Diffusion Tool"
	lappend description "Orthogonal 3D viewer with access to diffusion tensor processing, analysis and fiber tracking"

	if { ( [ file exists [file join $directory "../mjack/mpjvesseltool.tcl" ] ] !=0 ) } {

	    lappend appname "mpjvesseltool"
	    lappend applabelname "Vessel Tool"
	    lappend description "Orthogonal 3D viewer with vessel enhancement, tracking and analysis"
	}
	
	if { ( [ file exists [file join $directory "../mjack/mpjfmmtool.tcl" ] ] !=0 ) } {
	    lappend appname "mpjfmmtool"
	    lappend applabelname "FMM Tool"
	    lappend description "Fast Marching Tool with support for generic speed images"
	}

    }

    lappend appname "FMRI/Statistical Analysis"
    lappend applabelname "FMRI/Statistical Analysis"
    lappend description "None:fMRI"

    lappend appname "bis_fmri"
    lappend applabelname "fMRI Single Subject Analyzer"
    lappend description "Tool for running single subject GLM analysis as well as interface to AFNI (Also part of Brain Register)"

    set f [file join $directory "../vtknr/nritclmultisubject.tcl" ]
    if {  [ file exists $f ]  } {
	lappend appname "nritclmultisubject"
	lappend applabelname "fMRI Multi-Subject Analyzer"
	lappend description "fMRI Analysis/Multi Subject Statistical Analysis Tool -- this is provided as a preview only."
    }

   
    lappend appname  "Surface/Objectmap Editors"
    lappend applabelname  "Surface/Objectmap Editors"
    lappend description  "None:Editors"
    
    #    lappend appname  "pxitclobjectmapeditor"
    #   lappend description "Multi-Slice editor (2D View only) with the ability to edit splines/color regions on image slices."

    lappend appname  "pxitclsurfaceeditor"
    lappend applabelname  "Surface Editor"
    lappend description "3D Surface/Objectmap Editor"

    lappend appname  "pxitclsimpleeditor"
    lappend applabelname  "Mosaic Objectmap Editor"
    lappend description "MultiSlice Objectmap Editor"

    lappend appname  "pxitclbrainsegment"
    lappend applabelname  "Orthogonal Objectmap Editor"
    lappend description "Orthogonal Slice Objectmap Editor"

    lappend appname  "pxitclelectrodeeditor"
    lappend applabelname  "Electrode Editor"
    lappend description "Intracranial Electrode Localizer/Editor."

    lappend appname  "4D Dataset processing (cardiac)"
    lappend applabelname  "4D_Dataset_Processing"
    lappend description  "None:Cardiac"

    lappend appname  "pxitclvolumeviewer"
    lappend applabelname  "4D Volume Viewer"
    lappend description "Orthogonal 3D Viewer with ability to display volumes and cine-loops of multi-frame images"

    lappend appname  "pxitcl4dsurfaceeditor"
    lappend applabelname  "4D Surface Editor"
    lappend description "4D Multi-Slice editor (cardiac specific)"


    lappend appname "Data Tree Tool"
    lappend applabelname "DataTree Tool"
    lappend description "None:Data Tree"
    
    if { ( [ file exists [file join $directory "../datatree/biodatatree.tcl" ] ] !=0 ) } {
	lappend appname "biodatatree"
	lappend applabelname "Data Tree Manager"
	lappend description "Database tree front end with additional capabilities"
    }

    lappend appname "bis_treeapp"
    lappend applabelname "Data Tree Manager v2"
    lappend description "Database tree front end with additional capabilities -- enhanced"

    global pxtcl_pref_array
    if { $pxtcl_pref_array(VectorVisionLink)=="Enabled" } {
	lappend appname "VVLink"
	lappend applabelname "VVLink Tool"
	lappend description "None:VVLink"
	
	lappend appname  "pxitclvvlinkviewer"
	lappend applabelname  "Viewer for VVLink/Slicer Integration"
	lappend description "Reduced Functionality App for Slicer/VVLink Integration"
    }

    lappend appname "Mouse Image Analysis"
    lappend applabelname "Mouse Image Analysis"
    lappend description "None:MouseSuite"
    
    lappend appname "pxitclmousesegment"
    lappend applabelname "Mouse Segment"
    lappend description "Orthogonal 3D viewer with mouse segmentation functionality"

    lappend appname "pxitclmouseregister"
    lappend applabelname "Mouse Register"
    lappend description "Dual Orthogonal 3D viewer with mouse registration/segmentation functionality"

    lappend appname  "Utilities"
    lappend applabelname  "Image File Conversion"
    lappend description  "None:File Conversion Utilities"
    
    lappend appname "bis_dicom2nifti.tcl --dogui 1"
    lappend applabelname "Dicom2NIFTI"
    lappend description "Convert Images from DICOM to NIFTI"

    lappend appname "bis_varian2nifti.tcl --dogui 1"
    lappend applabelname "Varian2NIFTI"
    lappend description "Convert Images from Varian file format to NIFTI"


    CreateMenuPane $appname $applabelname $description $basewidget $directory

}
# -------------------------------------------------------------------------

if { $tcl_platform(platform) != "windows" } { 
    wm iconbitmap .  @$::pxtclvtkpxcontrib::bioimagesuiteicon 
}


frame .bottom
pack .bottom -side bottom -fill x -expand f

frame .top; pack .top -side top -fill x -expand f -padx 4

label .top.b -image $::pxtclvtkpxcontrib::logo -width 510
pack  .top.b -side left -expand t -fill x 

frame .topa -bg black -height 2
pack .topa -padx 4 -expand f -fill x -pady 2


frame .middle
pack .middle -side top -expand true -fill both -padx 1 -pady 1
#. configure -background darkblue

CreateNewApps .middle

set pref_dialog [ ::pxtcluserprefs::CreatePreferenceEditor ]

button .bottom.about -text "About!" -command MenuAboutCommand
button .bottom.upd -text "Check for Updates" -command MenuCheckForUpdates
button .bottom.here  -text "Which" -command MenuWhereCommand
button .bottom.pref  -text "Preferences" -command { wm deiconify $pref_dialog}
button .bottom.pref2  -text "Command Line Tools" -command { HelpCommand }
button .bottom.exit  -text "Exit!" -command exit
pack   .bottom.about .bottom.upd  .bottom.here .bottom.pref .bottom.pref2 .bottom.exit -side left -padx 5 -expand t -fill x


wm geometry . 650x520
set version $::pxtclvtkpxcontrib::version


wm title . $version


