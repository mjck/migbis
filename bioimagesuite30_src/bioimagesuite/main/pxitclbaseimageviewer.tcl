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





# 	$Id: pxitclbaseimageviewer.tcl,v 1.3 2005/06/22 13:01:09 xenios Exp $	

package provide pxitclbaseimageviewer 1.0


lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] bis_algorithm ]

# -----------------------
# Dependencies and path
# -----------------------
package require vtkpxcontrib  1.1
package require pxtcluserprefs 1.0
package require pxitclbaseimagecontrol   1.0
package require pxitclimagenormalization 1.0
package require pxitclimageimport        1.0
package require bis_guiimageutility      1.0
package require bis_guioverlayutility 1.0



package require pxitclbrainstrip         1.0
package require pxitclLandmarkControl    1.0
package require pxitclPolyDataControl    1.0
package require pxitclsurfaceobjectmap   1.0
package require pxitclmultisubjectaverage 1.0

package require pxitclcsigadget 1.0
package require pxitclatlasgadget 1.0

package require pxitclanimationgadget 1.0
package require pxitclvvlinkgadget 1.0




package require pxitclopenigtlinkgadget 1.0
package require pxitclexpfit 1.0
package require pxitclfilelistmenu 1.0
package require bis_fmri 1.0
package require bis_databasetool 1.0


# -----------------------------------------------------
#  Intialize Variables
# -----------------------------------------------------

itcl::class pxitclbaseimageviewer {

    #initialize vtable
    inherit pxitclbaseimagecontrol 

    protected common   thisparam
    protected variable vtk_viewer  0
    protected variable standalone  0

    public    variable mousecallback 0
    public    variable setnewimagecallback 0
    public    variable viewerbase 0
    public    variable menubase   0
    public    variable objectmapmenu   0
    public    variable bind_mouse_events 0
    public    variable enable_rendering_on_startup 1
    protected variable pref_dialog 0
    protected variable normalize_anatomical   0
    protected variable import_filetype 0
    protected variable in_expose 0
    protected variable can_accept_mask 1

    public    variable viewermode "neuro"
    public    variable objectmapname "objectmap.nii.gz"

    public variable show_standard_images 0

    
    protected variable controllist        ""
    protected variable showcontrollist    ""


    public    variable enable_displaymenu   1
    public    variable enable_helpmenu      1

    protected variable histcontrol            0
    public    variable enable_histcontrol     1

    public    variable overlaytool            0
    public    variable enable_overlaytool     1

    public    variable brainstriptool         0
    public    variable enable_brainstriptool  1

    # enabled by enable_talairachoption
    public    variable enable_talaraichoption 0
    protected variable atlastool              0


    public    variable expfittool         0
    public    variable enable_expfittool  0


    protected variable headercontrol          0
    public    variable enable_headercontrol   1

    protected variable importcontrol          0
    public    variable enable_importcontrol   1

    protected variable imageutility           0
    protected variable imageutility2          0
    public    variable enable_imageutility    1

    protected variable landmarkcontrol    0
    public    variable enable_landmarkcontrol 1
    public    variable enable_landmarkclicks  0

    protected variable polydatacontrol     0
    protected variable surfacecontrol     0
    public    variable enable_polydatacontrol 1

    protected variable surfaceobjectmap    0
    public    variable enable_surfaceobjectmap    1

    protected variable multisubjectcontrol 0
    public    variable enable_multisubjectcontrol 0


    protected variable fmrigadget 0
    public    variable enable_fmrigadget 0

    protected variable vvlinkgadget 0
    public    variable enable_vvlinkgadget  0

    protected variable openigtlinkgadget 0
    public    variable enable_openigtlinkgadget  1

    protected variable databasegadget 0
    public    variable enable_databasegadget  1


    protected variable csigadget 0
    public    variable enable_csigadget 0

    public    variable enable_internaleditor 0
    public    variable enable_objectmapmenu 0

    protected variable animationgadget 0
    public    variable enable_animationgadget 1


    public   variable  featuremenu 0

    # -------------------------------------------------
    
    constructor { par } {
	::pxitclbaseimagecontrol::constructor $par
    } {
	InitializeBaseImageControl
    }

    # Initialize Variables etc 
    private method InitializeBaseImageControl {  }

    # Access Key Data Structures
    public method GetLandmarkControl { } { return $landmarkcontrol }
    public method GetPolyDataControl { } { return $polydatacontrol }
    public method GetViewer { }

    # Set/Display Image
    public method SetResultsFromObject { img obj }
    public method SetValueImage        { vtkimg  }
    public method SetImageFromObject   { img obj } 
    public method SetMaskFromObject   { img obj } 
    public method CanAcceptMask { }

    public method UpdateControlsWithNewImage   { obj } 
    public method SetLookupTable       { colormap }
    public method SetLookupTableMode   { index }
    public method GetDisplayedImage { }
    public method ShowImage { flag args }
    public method SetSnapshotResolution { factor }
    public method SetViewerBackground { }

    # Load/Save Images 
    public method GetImageTypeList { mode } 
    public method LoadImage { f }
    public method QueryImage { }
    public method ImportImage { args }
    public method SaveImage { }
    public method ExportImage { }
    public method SwitchDirectory { { newdir ""  } }
    public method CopyMaskToResults { }
    public method GetImageName { } { return [ $currentimage cget -filename ] }

    # Objectmap Stuff
    public method ObjectmapInfo { }
    public method QuestionReplaceObjectmap { objectmap { message "" } { forceask 0 } }
    public method LoadObjectmap  {  f }
    public method GetObjectmapName  { } 
    public method LoadBrodmann  {  }
    public method GrabObjectmap { }
    public method SaveObjectmap  { fname comment }
    public method ClearObjectmap { }
    public method ShowPaintControls { }
    public method AutosaveEvent  { }
    public method AutosaveInvoke  { }
    public method ObjectmapHistogram { inp }



    # Talairach Stuff
    public method ResliceImageToTalairach {  mode }
    public method SetOldStyleMNITalairachTransform { mode }
    public method SetAtlasTool { atl } { set atlastool $atl }
    public method GetAtlasTool {  } { return $atlastool }

    # Header Stuff
    public method PrintHeader { imgno detail }

    # Create GUI Stuff
    public method CreateMenu { menubase }
    public method CreateControls { inpwidget }
    public method CreateHelpMenu { }

    # Callback Handlers
    public method HandleUpdateEvent { args }
    public method HandleMouseEvent { mouse stat x1 x2 widgetname args }
    public method HandleCoordinatesChangeEvent { { mouse -1 } { stat -1 } }
    public method SetShiftMouseToEditor { } 
    public method EnableVolumeTrace { }
    public method EnableSurfaceTrace { }
    public method HeaderControlCallback { }
    public method CurrentLandmarkToTalairach { } 

    # Add To Control List
    public method AddControl { control }
    public method AddShowControl { control }

    # Main Display Initialization Routines 
    protected method InitializeInterface { widget standalone  }
    protected method InitializeViewer    {  } 

    # Access Methods to create different kinds of viewers
    public method InitializeOrthogonalViewer { widget standalone }
    public method InitializeObjectmapViewer { widget standalone }
    public method InitializeMosaicViewer { widget standalone }
    public method InitializeObjectmapMosaicViewer { widget standalone }
    public method InitializeMultipleMosaicViewer { widget standalone }
    public method InitializeOrthogonal4DViewer { widget standalone } 

    # Generic Initialize Display Method --> InitializeOrthogonalViewer
    public method InitializeDisplay { widget standalone }


}

# -----------------------------------------------------------
# Main Body Definitions
# -----------------------------------------------------------
itcl::body pxitclbaseimageviewer::InitializeBaseImageControl { } {

    global pxtcl_pref_array
    
    if { $pxtcl_pref_array(NormalizeAnatomical) > 0 } {
	set normalize_anatomical 1
    }
    
    set thisparam($this,display) "Image";	
    set thisparam($this,snapshotresolution) 0	
    set thisparam($this,mousetolandmarks) 1;

    set thisparam($this,volumetrace) 1
    set thisparam($this,geometrytrace) 0
    set thisparam($this,toolbarhelp) ""


#     if { $enable_databasegadget == 1 } {
	
# 	set db 0
# 	catch { 
# 	    set db [ vtkbisMySQLDatabase New ]
# 	    $db Delete
# 	}
# 	if { $db == 0 } {
# 	    set enable_databasegadget 0
# 	}
#     }


}

# -----------------------------------------------------------
# Control Objects Return
# -----------------------------------------------------------
itcl::body pxitclbaseimageviewer::CopyMaskToResults { } {

    set img 0
    catch {
	set image [ $vtk_viewer GetObjectMapImage ]
	set img   [ [ pxitclimage \#autor ] GetThisPointer ]
	$img ShallowCopyImage $image
	$img CopyImageHeader [ $currentimage GetImageHeader ]
	$this SetResultsFromObject $img $this
	itcl::delete object $img
    }
}

itcl::body pxitclbaseimageviewer::GetViewer { } {
    return $vtk_viewer
}

itcl::body pxitclbaseimageviewer::SetLookupTable { colormap } {
    if { $vtk_viewer != 0 } {
	$vtk_viewer SetLookupTable $colormap
    }
}

itcl::body pxitclbaseimageviewer::SetLookupTableMode { index } {
    if { $vtk_viewer != 0 } {
	$vtk_viewer SetPresetLookupTable $index
    }
}
# ---------------------------------------------------------------------------------
#   Utility Functions for Setting New Image and Undo Features
# ---------------------------------------------------------------------------------



itcl::body pxitclbaseimageviewer::SetImageFromObject { img obj } {
    
    set a [ pxitclbaseimagecontrol::SetImageFromObject $img $obj ]
    SetTitle "No Image"
    if { $a == 0 } { return 0 } 
    
    $this UpdateControlsWithNewImage $obj

}
itcl::body pxitclbaseimageviewer::UpdateControlsWithNewImage   { obj }  {

    for { set i 0 } { $i < [ llength $controllist ] } { incr i }  {
	set cnt [ lindex $controllist $i ]
	if { $cnt != $obj  } {
	 #   puts stderr "Updating control $cnt with image = [ $currentimage GetShortDescription ]"
	    $cnt SetImageFromObject $currentimage $this
	}
    }

    if { $headercontrol != 0 && $obj != $headercontrol } {
	$headercontrol SetHeader [ $currentimage GetImageHeader ]
    }

    if { $setnewimagecallback != 0 } {
	eval "$setnewimagecallback $this"
    }
    
    $vtk_viewer SetEnableRendering 0
    ShowImage  "Image" 1
    SetTitle "$appname [ $currentimage GetShortDescription ]"
    $vtk_viewer SetEnableRendering 1
    return 1
}

itcl::body pxitclbaseimageviewer::SetResultsFromObject { img obj  } {
    if { [ $img GetImageSize ] < 2 } {	return 0    }
    $vtk_viewer SetEnableRendering 0
    SetResults $img
    $vtk_viewer SetEnableRendering 1
    ShowImage "Results" 1

    return 1
}

itcl::body pxitclbaseimageviewer::SetValueImage { vtkimg  } {
    set ok 0

    catch { 
	if { [ $vtkimg GetNumberOfPoints ] > 1 } {	
	    set ok 1
	}
    }
    if { $ok == 0 } {
	return
    }
    $vtk_viewer SetEnableRendering 0
    $vtk_viewer SetValueImage $vtkimg
    $vtk_viewer SetEnableRendering 1

    

    if { $parent != 0 } { 
	$parent SetValueImage $vtkimg 
    }
    return 1
}

itcl::body pxitclbaseimageviewer::GetDisplayedImage { } {

    if { $thisparam($this,display) == "Image"} { 
	set img $currentimage
    } else {
	set img $currentresults
    }

    return $img
}

itcl::body pxitclbaseimageviewer::CanAcceptMask { } {

    if { $vtk_viewer == 0 } {	return  0  }
    if { [ $vtk_viewer IsA "vtkpxGUIObjectmapOrthogonalViewer"  ] == 0
	    && [ $vtk_viewer IsA "vtkpxGUIObjectmapMosaicViewer" ] == 0 } {
	    return 0
	}

    if { $can_accept_mask == 0 } {
	return 0
    }

    return 1

}

itcl::body pxitclbaseimageviewer::SetMaskFromObject { img obj } {
    
    if { [ $this CanAcceptMask ] == 0 } {
	return 0
    }

    if { [ $img GetImageSize ] < 2 } {	return 0    }
    set disp [ $this GetDisplayedImage ]
    if { [ $disp GetImageSize ] < 2 } {	return 0    }

    set nc [ [  $disp GetImage ] GetNumberOfScalarComponents ]
    set oc [ [  $img  GetImage ] GetNumberOfScalarComponents ]


    # Scenario 1
    if { $oc != $nc } {
	set tmp [ vtkImageData [ pxvtable::vnewobj ]]
	$tmp CopyStructure [ $img GetImage ]
	$tmp SetNumberOfScalarComponents $nc
	$tmp AllocateScalars

	set t_data [ [ $tmp GetPointData ] GetScalars ] 
	set i_data [ [ [ $img GetImage ] GetPointData ] GetScalars ]

	set mval $nc
	if { $oc < $nc } {
	    set mval $oc 
	}

	for { set i 0 } { $i < $mval  } { incr i } {
	    $t_data CopyComponent $i $i_data $i
	}

	if { $nc > $oc } {
	    for { set i $oc } { $i < $nc  } { incr i } {
		$t_data CopyComponent $i $i_data [ expr $oc -1 ]
	    }
	}
    } else {
	set tmp [ $img GetImage ]
    }
	
    if { [ $img GetImageSize ] == [ $disp GetImageSize ] } {
	$vtk_viewer SetObjectMapImage $tmp
    } else {
	set resl [ vtkbisImageReslice [ pxvtable::vnewobj ]]
	$resl SetInput $tmp
	$resl SetInterpolationMode 0
	$resl SetInformationInput [ $disp GetImage ]
	$resl Update
	$vtk_viewer SetObjectMapImage [ $resl GetOutput ]
	
	$resl Delete
    }

    if { $oc!=$nc } {
	$tmp Delete
    }
    

}


# ---------------------------------------------------------------------------------
#   Display Code
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::SetSnapshotResolution { mode } {

    if { $vtk_viewer == 0 } {
	return 0
    }

    if { $mode < 1 } { set mode 1 } 
    if { $mode > 4 } { set mode 4 } 

    $vtk_viewer SetMagnificationFactor $mode
    puts stderr "Setting Magnification to $mode"
    return $mode
}

itcl::body pxitclbaseimageviewer::SetViewerBackground { } {
    set rw [ $vtk_viewer GetRenderWidgetName ]
    set renwin [ $rw GetRenderWindow ]
    $vtk_viewer SetBackgroundColor $renwin
}

itcl::body pxitclbaseimageviewer::ShowImage { flag args } {
    
    set doupdate 1
    if { [ llength $args ] > 0 } {
	set doupdate [ expr [ lindex $args 0 ] > 0 ]
    }

    set thisparam($this,display) $flag
    set img [ GetDisplayedImage ]
    if {  [ $img GetImageSize ] < 2 } { 
	if { $img == $currentresults } {
	    set thisparam($this,display) "Image"
	    return 0 
	} 
    }

    #    puts stdout " [ $img GetShortDescription ], or=[ [ $img GetImage ] GetOrigin ] sp=[ [ $img GetImage ] GetSpacing ]"

    set cmap [ $img GetLookupTable ]

    if { $vtk_viewer != 0 } {

	if { $cmap !=0 } {
	    $vtk_viewer SetImage [ $img GetImage ] [ $img GetOrientation ] $cmap [ $img GetIntensityOffset ] [ $img GetIntensityScale ]
	} else {
	    $vtk_viewer SetImage [ $img GetImage ] [ $img GetOrientation ] [ $img GetIntensityOffset ] [ $img GetIntensityScale ]
	}
    }

    if { $doupdate > 0 && $cmap == 0  } {
	
	set r [ [ [ [ $img GetImage ] GetPointData ] GetScalars ] GetRange ]
	set d [ expr [ lindex $r 1 ]- [ lindex $r 0 ] ]

	#	puts stderr "Setting Automatic Lookup Table $normalize_anatomical $d "

	if { $normalize_anatomical > 0 && $d > 20 } {
	 #   puts stderr "Lookup Table Mode 0"
	    SetLookupTableMode 0
	} else {
	  #  puts stderr "Lookup Table Mode 1"
	    SetLookupTableMode 0
	}
    }
   
    for { set i 0 } { $i < [ llength $showcontrollist ] } { incr i }  {
	set cnt [ lindex $showcontrollist $i ]
	$cnt SetImage $img 
    }
    

    return 1
}

# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::PrintHeader { imageno detail } {

    pxtkconsole
    set img $currentimage
    set name "Current Image"
    if { $imageno == 1 } {
	set img $currentresults
	set name "Current Results"
    }

    set a [ [ $img GetImageHeader ] GetHeaderDescription  $detail]
    pxtkprint "$name Header\n\t $a"

}

# ---------------------------------------------------------------------------------
#   Talairach Stuff
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::SetOldStyleMNITalairachTransform { mode }  {
    set tal 0
    catch { set tal [ $vtk_viewer GetTalairachTransform ] }

    if { $tal == 0 } { return }

    $tal SetMontrealMode $mode
    catch { $vtk_viewer MoveAxis 1 }
}

itcl::body pxitclbaseimageviewer::ResliceImageToTalairach { mode } {

    set txform [ $vtk_viewer GetTalairachTransform ]
    
    set tmp  [  [ pxitclimage \#auto ] GetThisPointer ]
    set resl [  vtkbisImageReslice [ pxvtable::vnewobj ]]
    
    if { $mode == 0 } {
	SetFilterCallbacks $resl "Reslicing Image To Talairach Space"
    } else {
	SetFilterCallbacks $resl "Reorienting Image To AC-PC Space"
    }
    WatchOn

    $tmp ShallowCopyImage [ $txform ResliceImage [ $currentimage GetImage ] $mode $resl ]
    $tmp CopyImageHeader 0

    if { $mode == 0 } {
	$tmp configure -filename [ AddPrefix [ $currentimage cget -filename ] "tal" ]
    } else {
	$tmp configure -filename [ AddPrefix [ $currentimage cget -filename ] "acpc" ]
    }

    $this SetResultsFromObject $tmp $this
    itcl::delete object $tmp 
    WatchOff
    $resl Delete
    return 

}
# ---------------------------------------------------------------------------------
#   I/O Functions
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::SwitchDirectory { { dirname "" } } {

    if { [ string length $dirname ] == 0 } {
	set dirname  [tk_chooseDirectory -title "Select Current Directory" ]
    }

    if { [ string length $dirname ] < 1 } {
	return 0
    }

    cd $dirname
    set dirname [ pwd ]
    pxtkprint "Switched directory to $dirname"
    for { set i 0 } { $i < [ llength $controllist ] } { incr i }  {
	set cnt [ lindex $controllist $i ]
	catch { $cnt SetCurrentDirectory $dirname  }
    }
}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::GetImageTypeList { mode } {
    return [ ::pxitclimage::GetImageTypeList $mode ]
}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::QueryImage { } {

    if { $databasegadget ==0 } {
	set databasegadget [ bis_databasetool \#auto $this ]
	set w $basewidget.databasecontrol
	$databasegadget InitializeGUI $w 0
    }
    $databasegadget ShowWindow 

}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::LoadImage { f } {

    set img [ [ pxitclimage \#auto ] GetThisPointer ]
    set ok [ $img Load $f "Image" $this ]
    if { $ok == 1 } {
	SetImageFromObject $img $this
    }
    
    itcl::delete object $img 
    return $ok
}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::ImportImage { args } {
    
    if { $importcontrol == 0 } {
	set importcontrol [ pxitclimageimport \#auto $this ]
	set w $basewidget.$importcontrol
	$importcontrol Initialize $w
	$this AddControl $importcontrol
    }

    if { ( $import_filetype != 0) && ( [ llength $args ] ==0 ) } {
	$importcontrol SetFileType $import_filetype	
	set import_filetype 0
    }

    if { [ llength $args ] > 0 } {
	set import_filetype [ $importcontrol GetFileType ]
	$importcontrol SetFileType [ lindex $args 0 ]
    }

    $importcontrol ShowWindow 

}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::SaveImage { } {
    
    set img [ GetDisplayedImage ]
    if { $img == 0 } {
	return
    }

    $img SetLookupTable [ $vtk_viewer GetLookupTable ]    

    WatchOn
    set ok [ $img Save "" $thisparam($this,display) $this $forceimageoutputformat ]
    if { $thisparam($this,display) == "Image" && $ok > 0 } { 
	$this UpdateControlsWithNewImage $this
    }
    WatchOff
    return $ok
}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::ExportImage { } {

    set img [ GetDisplayedImage ]
    if { $img == 0 } {
	return
    }
    
    set a $thisparam($this,display)
    set f  [tk_getSaveFile -title "Output Filename for $a" -initialfile export  -filetypes { {"JPEG Images" {.jpg}}}]   
    if  { [string length $f] < 1} { return }

    WatchOn
    ::pxtclutil::ExportJPEGImage $f [ $img GetImage ]
    WatchOff
}

# ---------------------------------------------------------------------------------
# Objectmap Stuff
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::ObjectmapInfo  {  } {

    set mask [ $vtk_viewer GetObjectMapImage ]
    [ [ $mask GetPointData ] GetScalars ] Modified 
    set range [ [ [ $mask GetPointData ] GetScalars ] GetRange ]
    tk_messageBox -title "Objectmap Info!" -type ok -message "Objectmap Information\n$objectmapname\n Range $range"
}

itcl::body pxitclbaseimageviewer::QuestionReplaceObjectmap { objectmap { message "" } { forceask 0 } } {


    if { $forceask == 0 } {
	[ [ $objectmap GetPointData ] GetScalars ] Modified
	set range [ [ [ $objectmap GetPointData ] GetScalars ] GetRange ]
	set r1 [ lindex $range 1 ]
	set r0 [ lindex $range 0 ]
	
	
	if { [ expr abs($r1-$r0) ] < 0.001  } {
	    return 1
	}
    }

    if { $message == "" } {
	set message "This operation will erase the current objectmap. Are you sure?"
    }
    set ok [ pxtclutil::Question $message ]
    if { $ok == 0 } {
	return 0
    }

    return 1
}


itcl::body pxitclbaseimageviewer::GrabObjectmap  { } {

    set mask [ $vtk_viewer GetObjectMapImage ]
    set img [ $this GetDisplayedImage ]
    set range [ [ [ [ $img GetImage ] GetPointData ] GetScalars ] GetRange  ]
    if { [ lindex $range 1 ] > 20  } {
	set warningtext "RANGE IS RATHER HIGH = $range"
	set forceask 1
    } else {
	set warningtext "Range = $range"
	set forceask 0
    }

    set ok [ $this QuestionReplaceObjectmap $mask "Grab Objectmap from current displayed image ($warningtext). This will erase the current objectmap. Are you sure?" $forceask ]
    
    if { $ok == 1 } {

	$vtk_viewer SetObjectMapImage [ $img GetImage ] 

	pxtclutil::Warning "Setting autosave to off, please enable if this is desired"
	set thisparam($this,autosave) 0
	$this ShowImage Image
    } 
}

itcl::body pxitclbaseimageviewer::LoadBrodmann  { } {

    global auto_path
    
    set m5 [ lsearch -inline -glob $auto_path "*/../base*" ]
    set m5 [ file normalize [ file join [ file dirname $m5 ] images ] ]

    set f [ file join $m5   yale_broadmann.nii.gz ]
    #puts stderr "f=$f"
    $this LoadObjectmap $f
}

itcl::body pxitclbaseimageviewer::LoadObjectmap  { f } {

    set mask [ $vtk_viewer GetObjectMapImage ]
    if { $mask != "" } {
	set ok [ $this QuestionReplaceObjectmap $mask "This will clear your current objectmap. Are you sure?" ]
	if { $ok == 0 } {
	    return
	}
    }

    if  { [string length $f] < 1 } { 
	
	set typelist { 
	    {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }}
	    {"VTK Files" {.vt*}}
	}

	set f  [tk_getOpenFile -title "Load Objectmap" -filetypes $typelist ]
	if { [ string length $f ] < 1 } {
	    return 0
	}
    }

    set ext [ file extension $f ]
    set img [ [ pxitclimage \#auto ] GetThisPointer ]
    

    if { $ext == ".vt" || $ext == ".vtk" } {
	set ana [ vtkStructuredPointsReader [ pxvtable::vnewobj ]]
	$ana SetFileName $f
	SetFilterCallbacks $ana "Reading Image"
	WatchOn
	$ana Update
	$img ShallowCopyImage [ $ana GetOutput ]
	$img configure -filename $f
	$img CopyImageHeader 0
	$ana Delete
    } else {
	set ana [vtkpxAnalyzeImageSource [ pxvtable::vnewobj ] ]
	SetFilterCallbacks $ana "Reading Image"
	WatchOn
	$ana Load $f
	set img [ [ pxitclimage \#auto ] GetThisPointer ]
	$img SetFromAnalyzeSource $ana
	$ana Delete
    }
    
    $vtk_viewer SetObjectMapImage [ $img GetImage ] 
    set objectmapname [ $img cget -filename ]
    $vtk_viewer UpdateDisplay

    set tname [ ::pxtclutil::GenerateVOIFileNameFromImageName $objectmapname ]
    set ineditor [ $vtk_viewer GetInternalEditor ]
    if { $ineditor != "" &&  [ file exists $tname ] ==1 } {
	
	set numc [ $ineditor GetNumberOfVOIS ]
	set mlist [ ::pxtclutil::LoadVOINamesList $tname $numc ]
	array set tmpname $mlist
	for { set i 0 } { $i < $numc } { incr i } {
	    $ineditor SetVOIName $i $tmpname($i)
	}
    }

    itcl::delete object $img 
    WatchOff
    return 1

}

itcl::body pxitclbaseimageviewer::GetObjectmapName  { } {
    return $objectmapname
}

itcl::body pxitclbaseimageviewer::SaveObjectmap  { fname comment } {

    set mask [ $vtk_viewer GetObjectMapImage ]

    if { [ string length $fname ] < 2 } {
	set f2  $objectmapname 
	set f2path [ file dirname $f2 ]
	set f2tail [ file tail    $f2 ]
	
	set typelist { 	    {"Analyze/NIFT1 Files" { .hdr .hdr.gz .nii .nii.gz }} }

	set fname  [tk_getSaveFile -title "Objectmap Filename" -filetypes $typelist -initialfile $f2tail -initialdir $f2path ]
    }
    
    if { [  string length $fname ] < 2 } {
	return 0
    }

    set ext [ file extension $fname ]

    set ok [ file writable [ file dirname $fname ] ]
    if { [ file exists $fname ] } {
	set ok [ file writable $fname ]
    }
    if { $ok ==0 } {
	tk_messageBox -title "Cannot save objectmap!" -type ok -message "You can not save objectmap in $fname [ file writable $fname ]"
	return 0
    }

    if { [ string length $fname ] > 0 } {
	set vtkwriter [ vtkpxAnalyzeImageWriter [ pxvtable::vnewobj ] ]
	$vtkwriter SetFileName    $fname
	$vtkwriter SetInput       $mask
	$vtkwriter SetImageHeader [ $currentimage GetImageHeader ]
	
	set hd [ $vtkwriter GetImageHeader ]
	set md [ $hd GetMode ]
	if { $md ==0 && $forceimageoutputformat == 2 } {
	    $hd SetModeToNIFTI_HDRIMG
	} elseif { $md > 0 && $forceimageoutputformat == 1 } {
	    $hd SetModeToAnalyze
	}
	
	if { $ext == ".gz" } {
	    $vtkwriter SetCompression 9
	    set line "Compressed"
	    set objectmapname [ file root $fname ]
	} else {
	    set line ""
	    set objectmapname  $fname 
	}

	SetFilterCallbacks $vtkwriter "${comment}Saving ${line} $ Image"
	WatchOn 
	$vtkwriter Update
	set status [ $vtkwriter GetSaveStatus ]
	
	if { $status == 0 } {
	    ::pxtclutil::Warning "Failed to save objectmap in $fname"
	} else {
	    set tname [ ::pxtclutil::GenerateVOIFileNameFromImageName $objectmapname ]

	    set ineditor [ $vtk_viewer GetInternalEditor ]
	    if { $ineditor != "" } {
		set numc [ $ineditor GetNumberOfVOIS ]
		set fileid [open $tname w]
		puts $fileid "\[ Objectmap Colors for $objectmapname \]"
		for { set i 0 } { $i < $numc } { incr i } {
		    puts $fileid "$i : [ $ineditor GetVOIName $i ]"
		}
		close $fileid
	    }
	}
	WatchOff 
	catch { $vtkwriter Delete }
    }
}

itcl::body pxitclbaseimageviewer::ShowPaintControls { } {
    set ineditor [ $vtk_viewer GetInternalEditor ]
    #    puts stderr "ineditor=$ineditor"
    $ineditor Show 1
}

itcl::body pxitclbaseimageviewer::ClearObjectmap { } {

    set mask [ $vtk_viewer GetObjectMapImage ]
    set ok [ $this QuestionReplaceObjectmap $mask "This will clear your current objectmap. Are you sure?" ]

    if { $ok == 0 } {
	return
    }

    set tmp [ vtkImageData [ pxvtable::vnewobj ]]
    $tmp CopyStructure [ [ $this GetDisplayedImage ] GetImage ]
    $tmp SetScalarTypeToUnsignedChar
    $tmp SetNumberOfScalarComponents 1
    $tmp AllocateScalars
    [ [ $tmp GetPointData ] GetScalars ] FillComponent 0 0.0
    $vtk_viewer SetObjectMapImage $tmp
    set  objectmapname    "objectmap.hdr"
    $tmp Delete

    set ineditor [ $vtk_viewer GetInternalEditor ]
    if { $ineditor != "" } {
	    set numc [ $ineditor GetNumberOfVOIS ]
	    for { set i 0 } { $i < $numc } { incr i } {
		$ineditor SetVOIName $i "Region $i"
	    }
	}

}

itcl::body pxitclbaseimageviewer::AutosaveInvoke  { } {

    if { $thisparam($this,autosave) == 1 } {
	$this AutosaveEvent
    }
}


itcl::body pxitclbaseimageviewer::AutosaveEvent  { } {

    if { $thisparam($this,autosave) == 1 } {
	set oldname $objectmapname
	
	set newname $objectmapname
	set ext [ file extension $newname ]
	set ext2 ""
	set outn [ file root $newname ]
	if { $ext == ".gz" } {
	    set ext2 ".gz"
	    set ext  [ file extension $outn ]
	    set outn [ file root $outn   ]
	}
	
	set newname "${outn}_autosave${ext}${ext2}"

	$this SaveObjectmap $newname "Auto-"
	pxtkprint "**** Autosaved objectmap in\n\t [ file normalize $newname ] [ exec date ]\n"
	set objectmapname $oldname
	eval "after 60000 $this AutosaveEvent"
    }
}

itcl::body pxitclbaseimageviewer::ObjectmapHistogram { inp } { 

    set image_ref [ $currentimage GetImage ]

    if { $inp == 1 } {
	set dognuplot 1
	set inp 0
    } else {
	set dognuplot 0
    }

    if { $inp == 0 || $inp == 1 } {
	set inp   [ $vtk_viewer GetObjectMapImage ]
    }

    
    if { [ $currentimage  GetImageSize ] < 2 } {
	if { [ $currentresults  GetImageSize ] < 2 } {
	    puts stderr "Bailing Out ..."
	    return
	}
	set image_ref [  $currentresults GetImage ]
    }

    catch {
	set timg [ $vtk_viewer GetValueImage ]
	set d [ $timg GetDimensions ]
	if { [ lindex $d 0 ] > 1 } {
	    set image_ref $timg
	    pxtkprint "Using Value Image for ROI Analysis"
	}
    }

    set rmin 0
    set rmax 0

    catch {
	[ [ $inp GetPointData ] GetScalars ] Modified
	set r   [ [ [ $inp GetPointData ] GetScalars ] GetRange ]
	set rmin [ lindex $r 0 ]
	set rmax [ lindex $r 1 ]
    }

    if { $rmax == $rmin } {
	pxtclutil::Warning "No Objectmap in memory (uniform level)\n"
	puts stderr "Bailing Out ..."
	return
    }
    
    set ident [ vtkIdentityTransform [pxvtable::vnewobj ]]
    set thr [ vtkpxMergeFmriConventional [ pxvtable::vnewobj ]]
    set arr [ vtkFloatArray [ pxvtable::vnewobj ]]
    set nt [  $thr ComputeROIStatistics $image_ref $inp  $ident $arr ]
    $thr Delete
    $ident Delete
    

    if { $dognuplot != 1 } {
	set fname  [tk_getSaveFile -title "Output VOI File" -filetypes { {"VOI Text File" {.txt}}} ]
	set fout 0
	if { [ string length $fname ] > 0 } {
	    set fout [ open $fname w ]
	}
	
	set spa [ $inp GetSpacing ]
	set vox [ expr [ lindex $spa 0 ] * [ lindex $spa 1 ] * [ lindex $spa 2 ]]
	pxtkconsole
	set line "\n\nVOI Statistics Output\n-------------------------------------------------------------------------------------------------------------------------\n\n"
	pxtkprint $line
	
        set nc [ $image_ref GetNumberOfScalarComponents ]
	
	if { $nc > 1 } {
	    set line "\#VOI                \t Frame \t Volume (mm^3) \t Volume (voxels) \t Centroid (Vox) i \t j \t k \t  Mean Value \t Std Value \t Max Value \t Max Location i \t j \t k \n"
	} else {
	    set line "\#VOI             \t Volume (mm^3)\t Vol (voxels)\tCentroid i\tCentroid j\tCentroid k\t Mean Value \t Std Value \t Max Value \t Max Loc i \t Max Loc j \t Max Loc k \n"
	}
	pxtkprint $line
	if { $fout!=0 } { puts -nonewline $fout $line }
	
	
	set doeditor 0
	set ineditor [ $vtk_viewer GetInternalEditor ]
	if { $ineditor != "" } {
	    set doeditor 1
	}
	
	
	pxtkprint "\n---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n"
	for { set i 0 } { $i < $nt } { incr i } {
	    set n [ $arr GetComponent $i 2 ]
	    set ind [ expr int([ $arr GetComponent $i 1 ]) ]
	    if { $n > 0 } {
		#	    puts stderr "ind = $ind , [ $arr GetComponent $i 0 ]"
		if { $ind > 0 } {
		    if { $doeditor ==1 } { 
			set name [ $ineditor GetVOIName $ind ]
		    } else {
			set name "VOI $ind "
		    }

		    set frame [ $arr GetComponent $i 0 ]
		    set line [ format "%-20s " $name ]
		    if { $nc > 1 } {
			set line "$line \t $frame"
		    }
		    
		    # volume in mm^3 and voxels
		    set line [ format "%s \t %-8.2f \t %-8d "  $line [ expr [ $arr GetComponent $i 6 ] ] [ expr int([ $arr GetComponent $i 2 ])] ]
		    
		    # Centroid is next
		    set line [ format "%s \t %-06.2f \t %-06.2f \t %-06.2f " $line [ $arr GetComponent $i 12 ] [ $arr GetComponent $i 13 ] [ $arr GetComponent $i 14 ]  ]
		    
		    # Mean and Sigma and Max Value
		    set line [ format "%s \t %-010.4f \t %-010.4f \t %-010.4f " $line [ $arr GetComponent $i 3 ] [ $arr GetComponent $i 4 ] [ $arr GetComponent $i 8 ] ]
		    set line [ format "%s \t %-010.4f \t %-010.4f \t %-010.4f " $line [ $arr GetComponent $i 9 ] [ $arr GetComponent $i 10 ] [ $arr GetComponent $i 11 ] ]
		    
		    
		    pxtkprint "$line\n"
		    if { $fout !=0 } { puts $fout $line }
		}
	    } else {
		puts stdout "Ignoring VOI=$ind"
	    }
	}

	if { $fout != 0 } {
	    close $fout
	}
    } else {

	set nc [ $image_ref GetNumberOfScalarComponents ]
	set tname "/tmp/[ pid].dat"
	set numusing 0
#	puts stdout "Writing to $tname nc=$nc nt=$nt numtuples=[ $arr GetNumberOfTuples ]"
	set fout [ open $tname w  ]

	set doeditor 0
	set ineditor [ $vtk_viewer GetInternalEditor ]
	if { $ineditor != "" } {
	    set doeditor 1
	}
	
	for { set i 0 } { $i < $nt } { incr i  } {
	    set using($i) 0
	}

	set line "\# Frame\t"
	set f 1
	for { set i 0 } { $i < $nt } { incr i  } {

#	    puts "Checking line $i/$nt"
	    set n [ $arr GetComponent $i 2 ]
	    set ind [ expr int([ $arr GetComponent $i 1 ]) ]
	    set frame [ expr int([ $arr GetComponent $i 0 ])]
	    if { $n > 0 && $ind > 0 && $frame == 0 } {
		if { $doeditor ==1 } { 
		    set name [ $ineditor GetVOIName $ind ]
		} else {
		    set name "VOI $ind "
		}
		append line "$name\t"
		set using($ind) 1
		set voiname($f) $name
		incr f
		incr numusing
	    } 		
	}
	puts $fout "$line"
#	puts stdout "$line"

	set line ""
	set currentframe 0
	for { set it 0 } { $it < $nt } { incr it } {
	    set ind [ expr int([ $arr GetComponent $it 1 ]) ]
	    set frame [ expr int([ $arr GetComponent $it 0 ])]
	    set val [ $arr GetComponent $it 3 ]
	    if { $ind == 0 } {
		set currentframe $frame
		if { $currentframe > 0 } {
		    append line "\n[expr $frame+1 ]\t"
		} else {
		    append line "[expr $frame+1]\t"
		}
	    }
	    
	    if { $using($ind) > 0 } {
		if {  $frame == $currentframe } {
		    append line "$val\t"
#		    puts stderr "Using it=$it, ind=$ind, frame=$frame current=$currentframe, line=$line"
		} else {
	#	    puts stderr "Skipping it=$it, ind=$ind, frame=$frame current=$currentframe, line=$line"
		} 
	    }
	}
	puts $fout $line
	close $fout

	set sname "/tmp/[ pid].gpl"
	set fout [ open $sname w ]
	puts $fout "set grid"
	puts $fout "set xlabel 'Time (frames)'"
	puts $fout "set ylabel 'ROI Value'"
	puts $fout "set title 'VOI Plot' "
	set line "plot"
	for { set i 1 } { $i <=$numusing } { incr i } {
	    append line " '$tname' using 1:[expr $i+1 ] w lp title \"$voiname($i)\" " 
	    if { $i != $numusing } {
		append line ", "
	    }
	}
	puts $fout $line
#	puts stdout "$line\n"
	puts $fout "pause -1 'Press return to close gnuplot "
	close $fout
	catch { exec xterm -geometry 40x2 -bg blue -fg white -title gnuplot -e  gnuplot $sname & }
	after 500
	file delete $tname
	file delete $sname
    }

    $arr Delete
}


# -----------------------
#  Basic Menu
# -----------------------

# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::AddControl { control } {
    
    lappend controllist $control
}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::AddShowControl { control } {
    
    lappend showcontrollist $control
}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::CreateControls { inpwidget } {

    global pxtcl_pref_array

    if { $enable_talaraichoption == 1 } {
	set p2 $::pxtclvtkpxcontrib::imagedir 

	set fname [ file join $p2 colinlookup ]
	set tal [ vtkpxTalairachTransform [ pxvtable::vnewobj ]]
	$tal SetColinLookupFileName $fname 0
	set fname [ file join $p2 colin_talairach_lookup.nii.gz ]
	$tal SetColinLookupFileName $fname 1
	$tal Delete
    }

    if { $enable_histcontrol ==1 || $enable_imageutility == 1  } {

	set cntrlm [ menu $menubase.cntrl -tearoff 0  ] 
	$menubase add cascade -underline 0  -label "Image Processing"  -menu $cntrlm
    }

    if { $enable_histcontrol == 1 } {

	set histcontrol [ pxitclimagenormalization \#auto $this ] 
	set w $inpwidget.$histcontrol
	set histcontrol [ $histcontrol GetThisPointer ]
	$histcontrol Initialize $w
	AddControl $histcontrol
	$histcontrol AddToMenuButton $cntrlm "Histogram Control"
	$cntrlm add separator
    }


    if { $enable_imageutility == 1 } {
	set imageutility [ bis_guiimageutility \#auto  0 ]
	$imageutility InitializeFromContainer $this
	set w $inpwidget.$imageutility

	set imageutility [ $imageutility GetThisPointer ]
	$imageutility CreateGUI $w 
	AddControl $imageutility
	$imageutility AddToMenuButton $cntrlm

	set imageutility2 [ [ bis_guiimageutility \#auto 1 ] GetThisPointer ]
	$imageutility2 InitializeFromContainer $this
	set w $inpwidget.[ pxvtable::vnewobj ]
	$imageutility2 CreateGUI $w 
	$cntrlm add separator
	AddControl $imageutility2
	$imageutility2 AddToMenuButton $cntrlm

    }


    if { $enable_landmarkcontrol == 1 } {
	set landmarkcontrol [ [ pxitclLandmarkControl \#auto $this ]  GetThisPointer ]
	$landmarkcontrol Initialize $inpwidget $vtk_viewer

	if { [ $vtk_viewer IsA "vtkpxGUIOrthogonalViewer"  ] } {
#	    puts stderr "Adding here ................... [ $landmarkcontrol GetLandmarkControl ], [ $vtk_viewer GetVolumeControl ]"
	    [ $vtk_viewer GetVolumeControl ] SetLandmarkControl [ $landmarkcontrol GetLandmarkControl ]
#	    puts stderr "Added here ................... [ [ $vtk_viewer GetVolumeControl ] GetLandmarkControl ] [ [ [ $vtk_viewer GetVolumeControl ] GetLandmarkControl ] GetClassName ]"
	}
    }
    if { $enable_polydatacontrol == 1 } {
	set polydatacontrol [ [ pxitclPolyDataControl \#auto $this ]  GetThisPointer ]
	$polydatacontrol Initialize $inpwidget $vtk_viewer
    }




    if { ( $enable_brainstriptool > 0 ) || $enable_expfittool==1 } {

	set mb [ menu $menubase.seg -tearoff 0  ]
	$menubase add cascade -underline 0  -label "Segmentation" -menu $mb
    
        if { $enable_brainstriptool > 0 } {
	    set brainstriptool [ pxitclbrainstrip \#auto $this ]
	    set w ${inpwidget}.${brainstriptool}
	    set brainstriptool [ $brainstriptool GetThisPointer ]

	    if { $enable_brainstriptool == 2 } {
		$brainstriptool configure -enable_fsl 0
		$brainstriptool configure -enable_duallevelset 0
	    } 

	    if { $enable_polydatacontrol == 1 && $enable_landmarkcontrol == 1 } {
		$brainstriptool SetPolyDataAndLandmarkControls [  $polydatacontrol GetPolyDataControl ] [ $landmarkcontrol GetLandmarkControl ] 
	    } else { 
		$brainstriptool DisableLevelSets
	    }
	    $brainstriptool Initialize $w $vtk_viewer
	    AddControl $brainstriptool
	    $brainstriptool AddToMenuButton $mb
	}
	
	if { $enable_expfittool == 1 } {

	    if { $brainstriptool != 0 } {
		$mb add separator
	    }
	    
	    set expfittool [ pxitclexpfit \#auto $this ]
	    set w ${inpwidget}.[ pxvtable::vnewobj ] 
	    set expfittool [ $expfittool GetThisPointer ]
	    
	    $expfittool Initialize $w 
	    AddControl $expfittool
	    $expfittool AddToMenuButton $mb

	}

    }

    if { $enable_landmarkcontrol ==1 || $enable_polydatacontrol == 1 } {

	#	menubutton $menubase.features -text "Features" -menu $menubase.features.m 
	#	set packfeatures 1
	#	set mb [ menu $menubase.features.m -tearoff 0  ]
	set mb [ menu $menubase.features -tearoff 0  ]
	$menubase add cascade -underline 0  -label "Features" -menu $mb
	set featuremenu $mb

	if { ($enable_landmarkcontrol == 1 || $enable_landmarkclicks == 1 ) && $vtk_viewer != 0 } {
	    if { [ $vtk_viewer IsA vtkpxGUIOrthogonalViewer ] } {
		eval "$mb add check -label \"Shift/Click to Landmark\" -variable [ itcl::scope thisparam($this,mousetolandmarks)] "
		eval "$mb add check  -variable [ itcl::scope thisparam($this,volumetrace) ] -label \"Enable Volume Trace\" -command { $this EnableVolumeTrace  } "
		eval "$mb add check  -variable [ itcl::scope thisparam($this,geometrytrace) ] -label \"Enable Geometry Trace\" -command { $this EnableSurfaceTrace  } "
	    } else {
		eval "$mb add check -label \"Shift/Click to Landmark\" -variable [ itcl::scope thisparam($this,mousetolandmarks)] -command { $this SetShiftMouseToEditor  }"
		SetShiftMouseToEditor
	    }
	    $mb add separator
	    
	    if { $enable_landmarkcontrol == 1 } {
		AddShowControl $landmarkcontrol
		$landmarkcontrol AddToMenuButton $mb "Landmark Control"
	    }
	}
	
	if { $enable_polydatacontrol == 1 } {
	    AddShowControl $polydatacontrol
	    $polydatacontrol AddToMenuButton $mb "Surface Control"

	    if { $enable_surfaceobjectmap == 1 } {
		set surfaceobjectmap [ [ pxitclsurfaceobjectmap \#auto $this ] GetThisPointer ]
		$surfaceobjectmap Initialize $inpwidget.[pxvtable::vnewobj] [ $polydatacontrol GetPolyDataControl ]
		$surfaceobjectmap AddToMenuButton $mb "Surface Objectmap Control"
	    }


	}
	
	

	set val $pxtcl_pref_array(ManualTalairach)
	if { $enable_talaraichoption == 1 && $val == "On" } {
	    
	    $mb add separator
	    set mb2 [ menu $menubase.features.tal -tearoff 0  ]
	    $menubase.features add cascade -underline 0  -label "Talairach"  -menu $mb2
	
	    if { $landmarkcontrol !=0  } {
		eval "$mb2 add command -label \"Current Landmark -> Talairach\" -command {  $this CurrentLandmarkToTalairach }"
		$mb2 add separator
	    }
	    eval "$mb2 add command -label \"Load Talairach Transform\" -command { $vtk_viewer LoadTalairachTransform \"\" } "
	    eval "$mb2 add command -label \"Save Talairach Transform\" -command { $vtk_viewer SaveTalairachTransform \"\" } "
	    $mb2 add separator
	    eval "$mb2 add command -label \"Old Style Mni2Talairach AP\" -command { $this SetOldStyleMNITalairachTransform 1 }"
	    eval "$mb2 add command -label \"Old Style Mni2Talairach PA\" -command { $this SetOldStyleMNITalairachTransform 0 }"
	}

	if { $enable_vvlinkgadget == 1 && $pxtcl_pref_array(VectorVisionLink)=="Enabled" } {
	    set vvlinkgadget [ pxitclvvlinkgadget  \#auto $this ]
	    set w ${basewidget}.${vvlinkgadget}
	    set vvlinkgadget [ $vvlinkgadget GetThisPointer ]
	    $vvlinkgadget Initialize $w $vtk_viewer
	    $mb add separator
	    $vvlinkgadget AddToMenuButton $mb
	}

	    if { $enable_openigtlinkgadget == 1 &&  $pxtcl_pref_array(OpenIGTLink)=="Enabled" } {
	    set openigtlinkgadget [ pxitclopenigtlinkgadget  \#auto $this ]
	    set w ${basewidget}.${openigtlinkgadget}
	    set openigtlinkgadget [ $openigtlinkgadget GetThisPointer ]
	    $openigtlinkgadget Initialize $w $vtk_viewer
	    $mb add separator
	    $openigtlinkgadget AddToMenuButton $mb
	}

    }


    if { $enable_talaraichoption == 1 } {
	set mb [ menu $menubase.atl -tearoff 0  ]
	$menubase add cascade -underline 0  -label "Atlas Tools"   -menu $mb
	if { $atlastool == 0 } {
	    set atlastool [ pxitclatlasgadget \#auto $this ]
	    set w ${basewidget}.${atlastool}
	    set atlastool [ $atlastool GetThisPointer ]
	    $atlastool Initialize $w
	    AddControl $atlastool
	}
	$atlastool AddToMenuButton $mb
    }

    if { $enable_overlaytool == 1 } {

	set mb [ menu $menubase.reg -tearoff 0  ]
	$menubase add cascade -underline 0  -label Registration   -menu $mb

	set overlaytool [ [ bis_guioverlayutility \#auto ] GetThisPointer ]
	set w ${inpwidget}.${overlaytool}
	$overlaytool InitializeFromContainer $this
	$overlaytool CreateGUI $w 
	AddControl $overlaytool
	$overlaytool AddToMenuButton $mb
    }

    if { $enable_multisubjectcontrol == 1 } {
	
	set mb [ menu $menubase.mt -tearoff 0  ]
	$menubase add cascade -underline 0  -label Multi-Subject -menu $mb

	set multisubjectcontrol [ pxitclmultisubjectaverage \#auto $this ]
	set w ${inpwidget}.${multisubjectcontrol}
	set multisubjectcontrol [ $multisubjectcontrol GetThisPointer ]
	$multisubjectcontrol Initialize $w 
	AddControl $multisubjectcontrol
	$multisubjectcontrol AddToMenuButton $mb
    }

    if { $enable_objectmapmenu == 1 } {
	set mb2 [ menu $menubase.fmrim -tearoff 0  ]
	$menubase add cascade -underline 0  -label  "Objectmap"  -menu $mb2
	eval "$mb2 add check -variable [ itcl::scope thisparam($this,autosave) ] -label \"Autosave\"  -command { $this AutosaveInvoke }"
	$mb2 add separator
	eval "$mb2 add command -label \"Show Paint Controls \" -command {  $this ShowPaintControls }"
	$mb2 add separator
	eval "$mb2 add command -label \"New Objectmap \" -command {  $this ClearObjectmap }"
	eval "$mb2 add command -label \"Load Objectmap \" -command {  $this LoadObjectmap \"\" }"
	eval "$mb2 add command -label \"Save Objectmap \" -command {  $this SaveObjectmap \"\" \"\" }"
	$mb2 add separator
	set thisparam($this,custobjectmaps)  [ pxitclfilelistmenu \#auto $mb2 "Custom Objectmaps" "$this LoadObjectmap  " "$this GetObjectmapName" ]
	$thisparam($this,custobjectmaps) SetModeToObjectmaps
	$thisparam($this,custobjectmaps) InitializeDisplay

	if { $enable_talaraichoption == 1 } {
	    $mb2 add separator
	    eval "$mb2 add command  -label \"Load Brodmann Atlas \" -command {  $this LoadBrodmann }"
	}
	$mb2 add separator
	eval "$mb2 add command  -label \"Grab From Current Displayed Image \" -command {  $this GrabObjectmap }"
	$mb2 add separator
	eval "$mb2 add command -label \"Objectmap Info \" -command {  $this ObjectmapInfo }"
	eval "$mb2 add command -label \"VOI Volume/Stats Computation\"  -command { $this ObjectmapHistogram 0 }"
	if {  [ ::pxtclutil::FindCommandInPath gnuplot ] !=0  } {
	    if {  [ ::pxtclutil::FindCommandInPath xterm ] !=0  } {
		eval "$mb2 add command -label \"Plot 4D Voi MAP\"  -command { $this ObjectmapHistogram 1 }"
	    }
	}
	set objectmapmenu $mb2
    }


    if { $enable_fmrigadget ==1 || $enable_csigadget ==1 } {

	set mb [ menu $menubase.fmrim -tearoff 0  ]
	$menubase add cascade -underline 1  -label  "Functional"  -menu $mb

	if { $enable_fmrigadget == 1 } {
	    set w ${inpwidget}.${fmrigadget}
	    toplevel $w; wm withdraw $w
	    update idletasks 
	    set fmrigadget [ bis_fmri \#auto $w 0 $vtk_viewer ]
	    set fmrigadget [ $fmrigadget GetThisPointer ]
	    $fmrigadget AddToMenuButtonLite $mb
	}


	if { $enable_csigadget == 1 } {
	    set csigadget [ pxitclcsigadget \#auto $this ]
	    set w ${inpwidget}.${csigadget}
	    set csigadget [ $csigadget GetThisPointer ]
	    $csigadget Initialize $w 
	    AddControl $csigadget
	    $csigadget AddToMenuButtonLite $mb
	}

    }
}

# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::CreateHelpMenu { } { 

    set helpm [ menu $menubase.help -tearoff 0  ]
    $menubase add cascade -underline 0  -label "Help" -menu $helpm 
    
    eval "$helpm add command -label \"Help Index\" -command { $this HelpCommand }"
    eval "$helpm add separator"
    eval "$helpm add command -label Preferences -command { wm deiconify $pref_dialog}"
    eval "$helpm add command -label ShowDBase -command { $this PrintDatabase }"
    $helpm add command -label Console -command { pxtkconsole }
    eval "$helpm add separator"
    eval "$helpm add command -label \"Check for Updates\" -command { $this CheckForUpdates }"
    eval "$helpm add separator"
    eval "$helpm add command -label About -command { $this AboutCommand }"
}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::CreateMenu { menubase } {

    global env
    global pxtcl_pref_array

    #    menubutton $menubase.file          -text File      -menu $menubase.file.m 
    #   pack $menubase.file   -side left

    set filem [ menu $menubase.file -tearoff 0  ]
    $menubase add cascade -underline 0  -label "File" -menu $filem

    eval "$filem add command -label Load -command { $this LoadImage \"\" }  "
    if { $enable_databasegadget == 1 } {
	eval "$filem add command -label Query -command { $this QueryImage  }  "
    }
    eval "$filem add command -label Save -command { $this SaveImage } "
    

    

    $filem add separator
    eval "$filem add radio -variable [ itcl::scope forceimageoutputformat ] -value 0 -label \"Auto-Output Format\" -command { $this SetImageOutputFormat 0 } "
    eval "$filem add radio -variable [ itcl::scope forceimageoutputformat ] -value 1 -label \"Force Analyze Format on Save\" -command { $this SetImageOutputFormat 1 }"
    eval "$filem add radio -variable [ itcl::scope forceimageoutputformat ] -value 2 -label \"Force NIFTI Format on Save\" -command { $this SetImageOutputFormat 2 } "

    $filem add separator
    eval "$filem add command -label \"Switch Directory\" -command { $this SwitchDirectory } "


    set thisparam($this,standimages) 0
    $filem add separator

    if { $show_standard_images == 1 } {
	set thisparam($this,standimages) [ pxitclfilelistmenu \#auto $filem "Standard Images" "$this LoadImage"  ]
	$thisparam($this,standimages) SetModeToStandardImages
	$thisparam($this,standimages) InitializeDisplay
    }

    set thisparam($this,custimages)  [ pxitclfilelistmenu \#auto $filem "Custom Images" "$this LoadImage" "$this GetImageName" ]
    $thisparam($this,custimages) SetModeToImages
    $thisparam($this,custimages) InitializeDisplay
    
    
    set thisparam($this,custdirectories)  [ pxitclfilelistmenu \#auto $filem "Custom Directories" "$this SwitchDirectory" ]
    $thisparam($this,custdirectories) SetModeToDirectories
    $thisparam($this,custdirectories) InitializeDisplay

    if { $enable_headercontrol == 1 } {
	$filem add separator
	set headercontrol   [ vtkbisGUINIFTIHeaderEditor [ pxvtable::vnewobj ] ]
	$headercontrol SetCallback "$this HeaderControlCallback"
	$headercontrol Initialize $basewidget 0 
	eval "$filem add command -label \"Image Header Editor\" -command { $headercontrol Show 1 } "
	eval "bind all <Control-h>  { $headercontrol Show 1} "
    }
    
    if { $enable_importcontrol == 1 } {
	$filem add separator
	eval "$filem add command -label Import -command { $this ImportImage }  ";
	eval "$filem add command -label Export -command { $this ExportImage }  "
    }
    


    $filem add separator
    if { $standalone == 1 } {
	    eval "$filem add command -label Exit -command {  $this ExitCommand } -underline 1"
    } else {
	eval "$filem add command -label Close -command {  $this HideWindow } -underline 1"
    }


    if { $enable_displaymenu == 1 } {

	set displaym [ menu $menubase.display ]
	$menubase add cascade -underline 0  -label "Display" -menu $displaym

	#	menubutton $menubase.display       -text Display   -menu $menubase.display.m 
	#	pack $menubase.display  -side left
	#	menu $$displaym -tearoff 1
	eval "$displaym add radio -variable [ itcl::scope thisparam($this,display)] -label Image  -command { $this ShowImage Image } "
	eval "$displaym add radio -variable [ itcl::scope thisparam($this,display)] -label Results  -command { $this ShowImage Results } "
	$displaym add separator
	eval "$displaym add radio -variable [ itcl::scope thisparam($this,snapshotresolution) ] -value 0 -label \"Low Res Snapshots\" -command { $this SetSnapshotResolution 1 } "
	eval "$displaym add radio -variable [ itcl::scope thisparam($this,snapshotresolution) ] -value 1 -label \"Low Medium Res Snapshots\" -command { $this SetSnapshotResolution 2 } "
	eval "$displaym add radio -variable [ itcl::scope thisparam($this,snapshotresolution) ] -value 2 -label \"High Medium Res Snapshots\" -command { $this SetSnapshotResolution 3 } "
	eval "$displaym add radio -variable [ itcl::scope thisparam($this,snapshotresolution) ] -value 3 -label \"High Res Snapshots\" -command { $this SetSnapshotResolution 4 } "
	$displaym add separator
	eval "$displaym add command -label \"Copy Results to Image\" -command { $this CopyResultsToImage }"
	eval "$displaym add command -label \"Undo Copy Results\" -command { $this RevertToPreviousImage }"
	if { [ $vtk_viewer IsA "vtkpxGUIObjectmapOrthogonalViewer"  ] == 1 ||
	     [ $vtk_viewer IsA "vtkpxGUIObjectmapMosaicViewer" ] == 1 } {
	    $displaym add separator
	    eval "$displaym add command -label \"Copy Mask to Results\" -command { $this CopyMaskToResults }"
	}
      
	$displaym add separator
	eval "$displaym add command -label \"Set Viewer Background\" -command { $this SetViewerBackground }"


	if { $enable_animationgadget == 1 } {
	    if { [  $vtk_viewer IsA "vtkpxGUIOrthogonalViewer"] ==1 } {
		$displaym add separator
		set animationgadget [ [ pxitclanimationgadget \#auto $this ] GetThisPointer ]
		$animationgadget Initialize ${basewidget}.[ pxvtable::vnewobj ]
		$animationgadget SetViewer $vtk_viewer
		$animationgadget AddToMenuButton $displaym
	    }
	}
    }
}

# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::HandleUpdateEvent { args } {
    
    if { $in_expose == 1 } { 
	return
    }

    if {$vtk_viewer != 0 } {
	set in_expose 1
	update idletasks
	$vtk_viewer UpdateDisplay
	set in_expose 0
    }
}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::SetShiftMouseToEditor { } {
    
    if { $vtk_viewer == 0 } {
	return
    }
    

    if { [ $vtk_viewer IsA vtkpxGUIMosaicViewer ] ==1 } {
	$vtk_viewer SetShiftMouseToEditor $thisparam($this,mousetolandmarks)
    }
}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::HandleMouseEvent { mouse stat x1 x2 widgetname args} {
    
    if { $vtk_viewer == 0 } {
	return
    }

    # Need to flip y-axis vtk counts from bottom tk counts from top !!!
    set wd [ [ $widgetname GetRenderWindow ] GetSize ]
    set x2 [ expr [lindex $wd 1 ] - $x2 ]
    $vtk_viewer HandleMouseButtonEvent $mouse $stat $x1 $x2
    $this HandleCoordinatesChangeEvent $mouse $stat
}

itcl::body pxitclbaseimageviewer::HandleCoordinatesChangeEvent { { mouse -1 } { stat -1 } } {    

    catch { set lv [ $vtk_viewer GetLastClickedPoint ] }

    set currentpoint $lv
    
    if { [ llength $lv ] > 1 } {
	if { [ lindex $lv 0 ] > -1 } {
	    
	    set lv [ $vtk_viewer GetLastClickedPointScaled ] 
	    set px [ lindex $lv 0 ]
	    set py [ lindex $lv 1 ]
	    set pz [ lindex $lv 2 ]
	    

	    if { [ $vtk_viewer PaintingEnabled ] == 0 } {
		if { $landmarkcontrol != 0 } {
		    if {$mouse == 4 || $thisparam($this,mousetolandmarks) == 0 } {
			[ $landmarkcontrol GetLandmarkControl ]  HandleClickedPoint $px $py $pz 1 $stat
		    }
		}
	    }
	    
	    set lv [ $vtk_viewer GetLastClickedPointScaled ] 
	    set px [ lindex $lv 0 ]
	    set py [ lindex $lv 1 ]
	    set pz [ lindex $lv 2 ]

	    #	    puts stderr "Raw = [ $vtk_viewer GetLastClickedPoint ], Scaled = [ $vtk_viewer GetLastClickedPointScaled ] mouse=$mouse status=$stat"
	    
	    if { $polydatacontrol != 0 } {
		[ $polydatacontrol GetPolyDataControl ] SetLastPoint $px $py $pz
	    }
	    
	    if { $atlastool != 0 } {
		set tal [ $vtk_viewer GetTalairachTransform ]
		if { $tal != "" } {
		    set md [ $tal GetColinMode ]
		    set ysz [ $tal GetColinYSize ]

		    if { $ysz != 216 } {
			set md 0
		    }
			
		    if { $md == 1 } {
			$atlastool IdentifyPoint $px $py $pz
		    } elseif { $md == -1 } {
			$atlastool IdentifyPoint [ expr 180 - $px ] [ expr 216.0 - $py ] $pz
		    } elseif { $ysz == 219 } {
			$atlastool SetError "You are using the Yale-modified Colin brain which is  Not in MNI Space, Cannot Identify Point.\n"
		    } else {
			$atlastool SetError "Image Not in MNI Space, Cannot Identify Point.\n"
		    }
		}
	    }
	    
	    if { $brainstriptool !=0 } {
		$brainstriptool RequestUpdatedSeed 0
	    }

	    

	    
	    if { [ $vtk_viewer IsA "vtkpxGUIOrthogonalViewer" ] == 1} {
		set upd [ $vtk_viewer GetMouseSliceChangeMode ]
		if { $upd ==0 } {
		    $vtk_viewer UpdateDisplay
		}
	    }
	}
	
	if { $mousecallback != 0 } {
	    eval "$mousecallback $this $mouse $stat"
	}
    }

}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::CurrentLandmarkToTalairach { } {
	
    set baseCurve [ vtkpxBaseCurve [ pxvtable::vnewobj ]]
    $baseCurve Copy [ [ $landmarkcontrol GetLandmarkControl ] GetCollection -1 ]
    $baseCurve Compact 
    
    set np [ [ $baseCurve GetPoints ] GetNumberOfPoints ]
    if { $np < 4 } {
	::pxtclutil::Warning "Need at least 4 points for talairach coordinate setup!"
	$baseCurve Delete
	return
    }

    $vtk_viewer SetTalairachPoints [ $baseCurve GetPoints]
    $vtk_viewer MoveAxis 1
    $baseCurve Delete 
    tk_messageBox -title "For your information!" -type ok -message "Talairach Transformation Set!\n Make sure you save it before exiting the program"
}

itcl::body pxitclbaseimageviewer::EnableVolumeTrace {  } {
    
    if { $vtk_viewer == 0 } { return } 
    if { [ $vtk_viewer IsA vtkpxGUIOrthogonalViewer  ] == 0 } { return }
    $vtk_viewer SetTraceVolume $thisparam($this,volumetrace)
    if { $thisparam($this,volumetrace) == 1 } { 
	set thisparam($this,geometrytrace) 0 
	$vtk_viewer SetTraceGeometry $thisparam($this,geometrytrace) 
    }
}

itcl::body pxitclbaseimageviewer::EnableSurfaceTrace { } {
    if { $vtk_viewer == 0 } { return } 
    if { [ $vtk_viewer IsA vtkpxGUIOrthogonalViewer  ] == 0 } { return }
    
    $vtk_viewer SetTraceGeometry $thisparam($this,geometrytrace) 
    if { $thisparam($this,geometrytrace) == 1 } { 
	set thisparam($this,volumetrace) 0 
	$vtk_viewer SetTraceVolume $thisparam($this,volumetrace)
    }
}

itcl::body pxitclbaseimageviewer::HeaderControlCallback { } {
    
    set hdr [ $headercontrol GetHeader ]
    set img [ [ pxitclimage \#auto ] GetThisPointer ]
    $img ShallowCopy $currentimage
    $img CopyImageHeaderAndFixImage $hdr
    $this SetResultsFromObject $img $headercontrol
    itcl::delete object $img

}

# ---------------------------------------------------------------------------------
    
itcl::body pxitclbaseimageviewer::InitializeOrthogonal4DViewer { widg standalone } {
    if { $initialized == 1 } { return $basewidget }
    set enable_polydatacontrol 0
    set enable_overlaytool 0
    set enable_brainstriptool 0
    set enable_surfaceobjctmap 0
    set  bind_mouse_events 1
    set  enable_helpmenu 0
    set vtk_viewer [ vtkpxGUIOrthogonal4DViewer [ pxvtable::vnewobj ]]
    set w [ InitializeInterface $widg $standalone ]
    set cn [ $vtk_viewer InitializeMovieControl $w 0 ]
    eval "bind all <Control-m> { wm deiconify $cn} "
    return $w
}

itcl::body pxitclbaseimageviewer::InitializeOrthogonalViewer { widg standalone } {
    if { $initialized == 1 } { return $basewidget }
    set vtk_viewer [ vtkpxGUIOrthogonalViewer [ pxvtable::vnewobj ]]
    if { $enable_talaraichoption == 0 } {
	$vtk_viewer DisableTalairachButtons
    }
    set  bind_mouse_events 1
    return [ InitializeInterface $widg $standalone ]
}

itcl::body pxitclbaseimageviewer::InitializeObjectmapViewer { widg standalone } {
    if { $initialized == 1 } { return $basewidget }
    set vtk_viewer [ vtkpxGUIObjectmapOrthogonalViewer [ pxvtable::vnewobj ]]

    if { $enable_internaleditor == 1 } {
	$vtk_viewer  SetCreateInternalEditorOn
	set enable_landmarkcontrol 1
	set enable_brainstriptool 1
    }
    
    if { $enable_talaraichoption == 0 } {
	$vtk_viewer DisableTalairachButtons
    }

    set  bind_mouse_events 1
    return [ InitializeInterface $widg $standalone ]
}


itcl::body pxitclbaseimageviewer::InitializeMosaicViewer { widg standalone } {
    if { $initialized == 1 } { return $basewidget }
    set vtk_viewer [ vtkpxGUIMosaicViewer [ pxvtable::vnewobj ]]
    set enable_polydatacontrol 0
    set  bind_mouse_events 0


    if { $enable_talaraichoption == 0 } {
	$vtk_viewer DisableTalairachButtons
    } else {
	$vtk_viewer EnableTalairachButtons
    }

    return [ InitializeInterface $widg $standalone ]
}

itcl::body pxitclbaseimageviewer::InitializeMultipleMosaicViewer { widg standalone } {
    if { $initialized == 1 } { return $basewidget }
    set vtk_viewer [ vtkpxGUIMultipleMosaicViewer [ pxvtable::vnewobj ]]
    set enable_polydatacontrol 0
    set  bind_mouse_events 0

    if { $enable_talaraichoption == 0 } {
	$vtk_viewer DisableTalairachButtons
    } else {
	$vtk_viewer EnableTalairachButtons
    }

    return [ InitializeInterface $widg $standalone ]
}


itcl::body pxitclbaseimageviewer::InitializeObjectmapMosaicViewer { widg standalone } {
    if { $initialized == 1 } { return $basewidget }
    set vtk_viewer [ vtkpxGUIObjectmapMosaicViewer [ pxvtable::vnewobj ]]
    set enable_polydatacontrol 0
    set bind_mouse_events 0
    set enable_landmarkcontrol 1
    set enable_brainstriptool 1

    if { $enable_talaraichoption == 0 } {
	$vtk_viewer DisableTalairachButtons
    } else {
	$vtk_viewer EnableTalairachButtons
    }
    if { $enable_internaleditor == 1 } {
	$vtk_viewer  SetCreateInternalEditorOn
    }
    return [ InitializeInterface $widg $standalone ]
}

# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::InitializeViewer {  } {

    if { $vtk_viewer ==0 } { return }

    $vtk_viewer SetEnableRendering $enable_rendering_on_startup 
    $vtk_viewer Initialize $viewerbase 1
    if { $bind_mouse_events == 1 } {
	$vtk_viewer BindMouseEvents [ $vtk_viewer GetRenderWidgetName ] "$this HandleMouseEvent" "" 
    }
    
    $vtk_viewer SetCoordinatesChangeCallback "$this HandleCoordinatesChangeEvent" 
    $vtk_viewer SetEnableCoordinatesChangeCallback 1

}

# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::InitializeInterface { widg st } {

    if { $initialized == 1 } { return $basewidget }
    
    set basewidget $widg
    toplevel $basewidget ; wm withdraw $basewidget
    wm geometry $basewidget 650x590
    wm title $basewidget $appname
    set standalone $st

    #    set menubasef    [ frame $basewidget.top     -width 400 -height 50 ]
    set menubase     [ menu $basewidget.top -tearoff 0  ]
    $basewidget configure -menu $menubase
    set viewerbase   [ frame $basewidget.viewer1 -width 400 -height 300 ]
    set pbar         [ frame $basewidget.pbar    -width 400 -height 20 ]

    InitializeViewer
	
    CreateProgressBar  $pbar
    
    pack $pbar       -side bottom -expand false -fill x
    pack $viewerbase -side top -expand true -fill both
    
    bind all <F1>  { pxtkconsole }

    if { $standalone == 1 } {
	eval "bind all <Control-d>  { $this ExitCommand }"
	eval "wm protocol . WM_DELETE_WINDOW { $this ExitCommand } "
	eval "wm protocol $basewidget WM_DELETE_WINDOW { $this ExitCommand  }"
	set pref_dialog [ ::pxtcluserprefs::CreatePreferenceEditor ]
    } else {
	eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    }


    #    set menubase [ menu $menubasef.menubar -tearoff 0  ]
    #puts stderr "$menubase $menubasef"
    #pack $menubasef.m -side top -expand t -fill both
    #$basewidget configure -menu $menubase
    CreateMenu $menubase
    CreateControls $viewerbase

    set initialized 1

    if { $standalone == 1 &&  $enable_helpmenu == 1 } {
	CreateHelpMenu
    }


    

}
# ---------------------------------------------------------------------------------
itcl::body pxitclbaseimageviewer::InitializeDisplay { widget st } {
	
    return [ InitializeOrthogonalViewer $widget $st ]
}



