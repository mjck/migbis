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





# 	$Id: pxitclsurfaceeditor.tcl,v 1.3 2003/09/03 13:59:52 xenios Exp xenios $	
lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]

package provide pxitclsurfaceeditor 1.0

# -----------------------
# Dependencies and path
# -----------------------
package require pxtclutil 1.0
package require vtkpxcontrib  1.1
package require pxitclbaseimageviewer 1.0
package require pxitclsplinestackobjectmap 1.0

# -----------------------------------------------------
#  Intialize Variables
# -----------------------------------------------------
itcl::class pxitclsurfaceeditor {

    inherit pxitclbaseimageviewer 

    protected variable splinestackcontrol 0
    protected variable splineobjectmapeditor       0 
    protected variable surfaceobjectmapcontrol   0 
    public    variable gui_splineobjectmapeditor   0
    public    variable gui_surfaceobjectmapcontrol   0
    public    variable numberofsurfaces   4
    public    variable objectmapname "objectmap.hdr"
    public    variable gui_fatobjectmap 0
    public    variable gui_saraobjectmap 0
    protected variable simpleviewer 0

    constructor { par } {
	::pxitclbaseimageviewer::constructor $par
    } {
	InitializeSurfaceEditor
    }
    
    private method InitializeSurfaceEditor { }
    public method SetImageFromObject { this obj } 
    
    # Display Initialization 
    protected method InitializeViewer  {  } 
    public    method InitializeDisplay { widg standalone } 


    # Get Surface from Spline Stack Control
    public    method GetSurface       { index } 
    public    method GetSplineStackControl { } { return splinestackcontrol }
    # Objectmap Stuff
    public method LoadImage { f } 
    public method GrabObjectmap  { }
    public method SendToMorphologyMask { }
    public method ObjectmapSurfaceFatCompute { mode }
    public method ObjectmapHelp { }

    public method SaraObjectmapCompute { mode }
    public method SaraObjectmapHelp { }

    public method ObjectmapSimpleViewer { mode } 

}
# -----------------------------------------------------
itcl::body pxitclsurfaceeditor::InitializeSurfaceEditor { } {

    set appname "BioImage Suite::Surface Editor"
    global pxtcl_pref_array

    set splinestackcontrol [ vtkpxGUIObjectmapSplineStackControl [ pxvtable::vnewobj ]]
    set splineobjectmapeditor  [ vtkpxGUI2DSplineObjectmapViewer [ pxvtable::vnewobj ]]

    set surfaceobjectmapcontrol  [ [ pxitclsplinestackobjectmap \#auto $this ] GetThisPointer ]
    set numberofsurfaces 4
    catch { 
	#	puts stdout "$pxtcl_pref_array(SurfaceEditorMaxSurfaces) , $pxtcl_pref_array(SplineEditorControlPointScale)"
	set numberofsurfaces $pxtcl_pref_array(SurfaceEditorMaxSurfaces) 
	set defaultscale     $pxtcl_pref_array(SplineEditorControlPointScale)
	if { $numberofsurfaces > 50 } {
	    set numberofsurfaces 45
	}
	$splinestackcontrol SetNumberOfSplineStacks $numberofsurfaces
	$splineobjectmapeditor    SetNumberOfSplines      $numberofsurfaces
	$splineobjectmapeditor    SetDefaultScaleMode     $defaultscale

    }

    if { $normalize_anatomical  == 1 } {
	$splineobjectmapeditor SetPresetLookupTableMode 0
    } else {
	$splineobjectmapeditor SetPresetLookupTableMode -1
    }
    $splinestackcontrol SetSplineEditor $splineobjectmapeditor

    set enable_headercontrol   1
    set enable_polydatacontrol 1
    set enable_overlaytool     0
    set enable_objectmapmenu   1

    set thisparam($this,helpdialog) ""
    set thisparam($this,autosave) 0
    set can_accept_mask 0

}

    
# ---------------------------------------------------------------------------------
#  GUI Stuff 
# ---------------------------------------------------------------------------------

itcl::body pxitclsurfaceeditor::SetImageFromObject { img obj } {

    set a [ pxitclbaseimageviewer::SetImageFromObject $img $obj ]
    if { $a==0 } { 
	return 0 
    }
    
    if { $splinestackcontrol != 0 } {
	$splineobjectmapeditor    LinkLookupTable [ $vtk_viewer GetLookupTable ]
	$splinestackcontrol SetImage [ $currentimage GetImage ] [ $currentimage GetOrientation ] 0 0
    }
}
    
# ---------------------------------------------------------------------------------
#  Update Stuff -- for change frame communication between image and splinestack
# ---------------------------------------------------------------------------------
itcl::body pxitclsurfaceeditor::GetSurface { index } {
    return [ $splinestackcontrol GetSplineStack $index ]
}

itcl::body pxitclsurfaceeditor::InitializeViewer {  } {



    if { $vtk_viewer ==0 } { return }

    set tframe [ frame $viewerbase.top ]
    set bframe [ frame $viewerbase.bot ]
    pack $bframe -side bottom -expand false -fill x
    pack $tframe -side top -expand true -fill both

    set masterframe [ frame  $bframe.[pxvtable::vnewobj ] -relief raised]
    set masterframe2 [ frame $bframe.[pxvtable::vnewobj ] ]
    set masterframe3 [ frame $bframe.[pxvtable::vnewobj ] ]
    $masterframe2 configure -height 3 -bg black
    $masterframe3 configure -height 3 -bg black
    pack $masterframe3 $masterframe $masterframe2 -side bottom -expand f -fill x

    set vframe [ frame $tframe.left ]
    set note $tframe.notebook
    iwidgets::tabnotebook $note -tabpos n
    pack $note -side right -expand f -fill y
    pack $vframe -side left -expand true -fill both

    set cframe [ $note add -label "Image" ] 
    set dframe [ $note add -label "Surface" ] 
    set eframe [ $note add -label "Surface +" ] 

    $note view "Surface"



    

    set vr [ $vtk_viewer Initialize $vframe $cframe 1 ]

    $splinestackcontrol SetBaseViewer $vtk_viewer
    set ct [ $splinestackcontrol Initialize $dframe $eframe $masterframe 1 ]




    set gui_splineobjectmapeditor [ $splineobjectmapeditor Initialize $viewerbase 0 ]
    wm title $gui_splineobjectmapeditor "Objectmap/Spline Editor"
    wm geometry $gui_splineobjectmapeditor 700x600

    # SetControlsViewer $vtk_viewer $splinestackcontrol
    $splinestackcontrol SetRenderer [ [ $vtk_viewer GetRenderer 3 ] GetRenderer ] 0 0
    $splinestackcontrol SetRenderer [ [ $vtk_viewer GetRenderer 0 ] GetRenderer ] 1 1
    $splinestackcontrol SetRenderer [ [ $vtk_viewer GetRenderer 1 ] GetRenderer ] 2 1
    $splinestackcontrol SetRenderer [ [ $vtk_viewer GetRenderer 2 ] GetRenderer ] 3 1

    if { $bind_mouse_events == 1 } {
	$vtk_viewer BindMouseEvents [ $vtk_viewer GetRenderWidgetName ] "$this HandleMouseEvent" "" 
    }
}

# ---------------------------------------------------------------------------------
#  Objectmap GUI
# ---------------------------------------------------------------------------------

itcl::body pxitclsurfaceeditor::LoadImage { f } {

    set a [ pxitclbaseimageviewer::LoadImage $f ]
    if { $a > 0 } {
	$this ClearObjectmap
    }
}
    
 
itcl::body pxitclsurfaceeditor::GrabObjectmap  { } {

    set transp [ $splineobjectmapeditor  GetObjectmapTransparency ]
    $splineobjectmapeditor SetObjectmapTransparency 0
    
    set mask [ $splineobjectmapeditor GetMaskImage ]
    set ok [ $this QuestionReplaceObjectmap $mask "Grab Objectmap from current view. This will erase the current objectmap. Are you sure?" ]

    if { $ok == 1 } {
	set img [ GetDisplayedImage ]

	$splineobjectmapeditor SetMaskImage [ $img GetImage ] [ $img GetOrientation ]

	pxtclutil::Warning "Setting autosave to off, please enable if this is desired"
	set thisparam($this,autosave) 0
	$splineobjectmapeditor SetObjectmapTransparency $transp
	$this ShowImage Image
    } else {
	$splineobjectmapeditor SetObjectmapTransparency $transp
    }
}

itcl::body pxitclsurfaceeditor::SendToMorphologyMask { } {

    if { $brainstriptool == 0 } {
	return 
    }

    WatchOn
    $brainstriptool SetMorphologyMaskImage  [ $splineobjectmapeditor GetMaskImage ]
    WatchOff

}

itcl::body pxitclsurfaceeditor::ObjectmapHelp { } {

    if { [ string length $thisparam($this,helpdialog) ] > 2 } { 
	::pxtclutil::ShowWindow $thisparam($this,helpdialog)
	return
    }
    
    set w ""
    if { [ string length $basewidget ] > 2 } { set w $basewidget }
    
    set thisparam($this,helpdialog)  [ toplevel $w.helpdialog ]
    eval "wm protocol $w.hdelpdialog WM_DELETE_WINDOW { wm withdraw $w.helpdialog }"
    
    ::pxtclhelp::HelpDialog $thisparam($this,helpdialog)
    wm title $thisparam($this,helpdialog) "Help on Objectmaps"
    
    set line "An objectmap is a binary representation of different objects taking range 0-9. These can be either manually created using\n\n"
    set line "$line 1. The Spline/Objectmap Editor -- directly editing either with brush tools or filling splines. "
    set line "$line (The objectmap editor can also be used to edit the appearance of the objectmap in terms of the colors assigned to each value)\n\n"
    set line "$line 2. The \"Create from Surfaces\" option above to fill in 3D for the whole surface. This will produce a temporary image which then is stored in the actual objectmap using the \"Grab from Current Displayed Image\" option\n\n"
    set line "$line 3. The Segmentation tool. The output of anyo of these operations
 greats a temporary objectmap which is only made permanent when the the \"Grab from Current Displayed Image\" option is invoked \n\n"
    set line "$line Additional processing to the objectmap can be made by sending it to the math_morphology tool using the option \"Store in Morphology Mask\" in this menu.\n\n"
    set line "$line Naturally objectmaps can be loaded, saved and reinitialized using options in these menu (\"New Objectmap\", \"Load Objectmap\", \"Save Objectmap\")\n\n"
    set line "$line Further the current objectmap is autosaved every 2 minutes when the \"Autosave\" checkbox on this menu is enabled\n"

    ::pxtclhelp::HelpDialogAddText $thisparam($this,helpdialog) $line

    wm  geometry $thisparam($this,helpdialog) 680x600
    ::pxtclutil::ShowWindow $thisparam($this,helpdialog)
}


itcl::body pxitclsurfaceeditor::ObjectmapSurfaceFatCompute { mode } {

    if { $mode == 0 } {
	if { $gui_fatobjectmap == 0 } {
	    set gui_fatobjectmap  $basewidget.[ pxvtable::vnewobj ]
	    set w $gui_fatobjectmap

	    toplevel $w -width 400 -height 250
	    wm title $w "Fat Computation"
	    frame $w.bot; 	    pack $w.bot -side bottom -expand f -fill x

	    set thisparam($this,fat_visc) 3
	    set thisparam($this,fat_artf) 8
	    set thisparam($this,fat_subq) 2
	    set thisparam($this,fat_abdsurface) 1
	    set thisparam($this,fat_numclasses) 4

	    iwidgets::entryfield $w.1 -labeltext "Visceral Fat Value:"  -width 3 -validate integer -textvariable [ itcl::scope thisparam($this,fat_visc) ] -relief sunken 
	    iwidgets::entryfield $w.2 -labeltext "Artifact Value:"  -width 3 -validate integer -textvariable [ itcl::scope thisparam($this,fat_artf) ] -relief sunken 
	    iwidgets::entryfield $w.3 -labeltext "Abdominal Surface Index:"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,fat_abdsurface) ] -relief sunken 
	    iwidgets::entryfield $w.4 -labeltext "Subcutaneous Fat (Output) Value:"  -width 3 -validate integer -textvariable [ itcl::scope thisparam($this,fat_subq) ] -relief sunken 
	    iwidgets::entryfield $w.5 -labeltext "Num Classes:"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,fat_numclasses) ] -relief sunken 
	    pack $w.1 $w.2 $w.3 $w.4 $w.5 -side top -expand true -fill x

	    eval "button $w.bot.c -text Close   -command {  wm withdraw $w }"
	    eval "button $w.bot.b -text Compute -command { $this ObjectmapSurfaceFatCompute 1 } "
	    pack $w.bot.c $w.bot.b -side left -padx 5
	} else {
	    wm deiconify $gui_fatobjectmap
	    eval "wm protocol $gui_fatobjectmap WM_DELETE_WINDOW { grab release $gui_fatobjectmap ; wm withdraw $gui_fatobjectmap } "
	}
	return 
    }


    # Step 1 -- run Histogram Segmentation to get body
    # Connect -- Background to 0

    WatchOn
    set hist [ vtkpxHistogramSegmentation [ pxvtable::vnewobj ]]
    set img [ $hist ComputeFatValues [ $currentimage GetImage ] [ $splineobjectmapeditor GetMaskImage ]  [ $this GetSurface [ expr $thisparam($this,fat_abdsurface) -1 ] ] $thisparam($this,fat_visc) $thisparam($this,fat_artf) 3 $thisparam($this,fat_numclasses) ]
    WatchOff

    ObjectmapHistogram $img

    $currentresults ShallowCopyImage $img
    $img Delete
    $hist Delete

    $currentresults CopyImageHeader [ $currentimage GetImageHeader ]
    $currentresults configure -filename    "fat_objectmap.hdr"
    ShowImage "Results" 1

    $splineobjectmapeditor SetObjectmapTransparency 0
	      
}
# ---------------------------------------------------------------------------------

itcl::body pxitclsurfaceeditor::SaraObjectmapCompute { mode } {

    if { $mode == 0 } {
	if { $gui_saraobjectmap == 0 } {
	    set gui_saraobjectmap [ toplevel $basewidget.[ pxvtable::vnewobj ]  -width 400 -height 250 ]
	    set w $gui_saraobjectmap
	    eval "wm protocol $w WM_DELETE_WINDOW { wm withdraw $w }"
	    
	    wm title $w "4-Component Fat Computation"
	    frame $w.bot;   pack $w.bot -side bottom -expand f -fill x

	    set thisparam($this,sara_abdominal_surface) 1
	    set thisparam($this,sara_visceral_surface) 2
	    set thisparam($this,sara_fascia_surface) 3
	    set thisparam($this,sara_skin_surface) 4
	    set thisparam($this,sara_fat_min) -1
	    set thisparam($this,sara_fat_max) -1
	    set thisparam($this,sara_fat_minsubq) -1

	    iwidgets::entryfield $w.1 -labeltext "Visceral Surface:"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,sara_abdominal_surface) ] -relief sunken 
	    iwidgets::entryfield $w.2 -labeltext "Abdominal Surface:"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,sara_visceral_surface) ] -relief sunken 
	    iwidgets::entryfield $w.3 -labeltext "Fascia Surface:"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,sara_fascia_surface) ] -relief sunken 
	    iwidgets::entryfield $w.35 -labeltext "Skin Surface:"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,sara_skin_surface) ] -relief sunken 
	    frame $w.4

	    pack $w.1 $w.2 $w.3 $w.35 $w.4  -side top -expand true -fill x


	    iwidgets::entryfield $w.4.1 -labeltext "Fat Min:"  -width 4 -validate integer -textvariable [ itcl::scope thisparam($this,sara_fat_min) ] -relief sunken 
	    iwidgets::entryfield $w.4.2 -labeltext "Max:"  -width 4 -validate integer -textvariable [ itcl::scope thisparam($this,sara_fat_max) ] -relief sunken 
	
	    
	    pack $w.4.1 $w.4.2   -side left -expand true -fill x -pady 2

	    eval "button $w.bot.c -text Close   -command {  wm withdraw $w }"
	    eval "button $w.bot.b -text Compute -command { $this SaraObjectmapCompute 1 } "
	    pack $w.bot.c $w.bot.b -side left -padx 5
	} else {
	    wm deiconify $gui_saraobjectmap
	    eval "wm protocol $gui_saraobjectmap WM_DELETE_WINDOW { grab release $gui_saraobjectmap ; wm withdraw $gui_saraobjectmap } "
	}
	return 
    }

    set ok [ pxtclutil::Question "Grab Initial Objectmap from current view. This will erase the current objectmap. Are you sure?" ]
    if { $ok == 0 } {
	return
    }

    set hist [ vtkpxHistogramSegmentation [ pxvtable::vnewobj ]]
    
    set imgin  [ [ $this GetDisplayedImage ] GetImage ]
    set orient [ [ $this GetDisplayedImage ] GetOrientation ]
    set sur1  [ $this GetSurface [ expr $thisparam($this,sara_abdominal_surface) -1 ] ]
    set sur2  [ $this GetSurface [ expr $thisparam($this,sara_visceral_surface) -1 ] ]
    set sur3  [ $this GetSurface [ expr $thisparam($this,sara_fascia_surface) -1 ] ]
    set sur4  [ $this GetSurface [ expr $thisparam($this,sara_skin_surface) -1 ] ]

    WatchOn; update idletasks
    set img [ $hist ComputeCaprioFatValues $imgin $sur1 $sur2 $sur3 $sur4 $thisparam($this,sara_fat_min) $thisparam($this,sara_fat_max) ]
    WatchOff

    ObjectmapHistogram $img

    $currentresults ShallowCopyImage $img

    $splineobjectmapeditor SetMaskImage  $img  $orient
    $splineobjectmapeditor SetObjectmapTransparency 50
    $this ShowImage Image
    $img Delete
    $hist Delete
}

# ---------------------------------------------------------------------------------
#  Initialize Display Stuff
# ---------------------------------------------------------------------------------

itcl::body pxitclsurfaceeditor::InitializeDisplay { widg st } {
    
    if { $initialized == 1 } { return $basewidget }

    #$this configure -appname pxitclsurfaceeditor
    $this configure -show_standard_images 1
    $this configure -enable_helpmenu 0
    $this configure -enable_multisubjectcontrol 0
    $this configure -enable_overlaytool 0
    $this configure -enable_talaraichoption 1
    #$this configure -appname "Surface Editor"
    $this configure -enable_rendering_on_startup  0

    set vtk_viewer [ vtkpxGUIObjectmapOrthogonalViewer [ pxvtable::vnewobj ]]
    $vtk_viewer SetSplineObjectmapEditor $splineobjectmapeditor
    set  bind_mouse_events 1
    

    InitializeInterface $widg $st


    set gui_surfaceobjectmapcontrol [ $surfaceobjectmapcontrol Initialize $widg.objectmap $this ]
    wm title $gui_surfaceobjectmapcontrol "Surface Objectmap Tool"

    eval "bind all <Control-e> { wm deiconify $gui_splineobjectmapeditor }"


    set mb2 $objectmapmenu
    eval "$mb2 add command  -label \"Store in Morphology Mask\" -command {  $this SendToMorphologyMask }"
    $mb2 add separator

    $mb2 add cascade -underline 0  -label "Create From Data" -menu $mb2.cascade1
    set casc1 [ menu $mb2.cascade1 -tearoff 0  ]
    
    eval "$casc1 add command  -label \"Grab From Current Displayed Image \" -command {  $this GrabObjectmap }"
    eval "$casc1 add command -label \"Create from Surfaces\" -command {  wm deiconify $gui_surfaceobjectmapcontrol }"

    $mb2 add cascade -underline 0  -label "Fat Objectmap Operations" -menu $mb2.cascade2
    set casc2 [ menu $mb2.cascade2 -tearoff 0  ]
    eval "$casc2 add command -label \"Create Full Fat Objectmap\" -command {$this ObjectmapSurfaceFatCompute 0 }"
    eval "$casc2 add command -label \"Create 4 Class Objectmap\" -command {  $this SaraObjectmapCompute 0 }"

    $mb2 add separator
    eval "$mb2 add command -label \"Display in Simple Objectmap Viewer\"  -command { $this ObjectmapSimpleViewer display }"

    #   $mb2 add separator
    #    eval "$mb2 add command -label \"Help\" -command { $this ObjectmapHelp }"

    #    eval "button $masterframe.edit  -text Edit -command  {  wm deiconify $gui_splineobjectmapeditor }"
    #    pack $masterframe.edit -side right -padx 2

    wm geometry $basewidget 700x705
    $this CreateHelpMenu
}



# ---------------------------------------------------------------------
itcl::body pxitclsurfaceeditor::ObjectmapSimpleViewer { mode } {
    
    if { $simpleviewer == 0 } {
	set simpleviewer [ [ pxitclbaseimageviewer \#auto 0 ] GetThisPointer ]
	$simpleviewer configure -appname "Simple Objectmap Viewer"
	$simpleviewer configure -show_standard_images 0
	$simpleviewer configure -enable_multisubjectcontrol 0
	$simpleviewer configure -enable_csigadget 0
	$simpleviewer configure -enable_rendering_on_startup  1
	$simpleviewer configure -enable_headercontrol  0
	$simpleviewer configure -enable_importcontrol  0
	$simpleviewer configure -enable_imageutility  0
	$simpleviewer configure -enable_landmarkcontrol  0
	$simpleviewer configure -enable_overlaytool  0
	$simpleviewer configure -enable_histcontrol  0
	$simpleviewer configure -enable_displaymenu  0
	$simpleviewer configure -enable_brainstriptool  0
	$simpleviewer configure -enable_helpmenu  0
	$simpleviewer configure -enable_talaraichoption 1
	
	$simpleviewer InitializeObjectmapMosaicViewer ${basewidget}.[pxvtable::vnewobj ] 0
	update idletasks

	set mbar [ $simpleviewer cget -menubase ]
	$mbar delete 0 1
	set mb [ menu $mbar.dispm -tearoff 0 ]; $mbar add cascade -underline 0  -label Display -menu $mb
	eval "$mb add command  -label \"Grab Image \& Objectmap From 3D Viewer\"  -command { $this ObjectmapSimpleViewer grab } "
	$mb add separator
	eval "$mb add command -label Close -command {  $simpleviewer HideWindow } -underline 1"
    } 
    
    $simpleviewer ShowWindow

    if { $mode == "grab" } {
	set viewer [ $this GetViewer ]
	$simpleviewer SetImageFromObject [ $this GetDisplayedImage ] $this
	$simpleviewer SetLookupTable [ $viewer GetLookupTable ]
	[ $simpleviewer GetViewer ] SetObjectMapImage [  $viewer GetObjectMapImage ]
	[ $simpleviewer GetViewer ] SetObjectLookupTable  [ $viewer  GetObjectLookupTable ]
	[ $simpleviewer GetViewer ] SetObjectmapTransparency  [ $viewer  GetObjectmapTransparency ]
	[ $simpleviewer GetViewer ] UpdateDisplay
    }
}


if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    wm withdraw .    
    set imp [ pxitclsurfaceeditor \#auto 0 ] 
    $imp InitializeDisplay .a 1
    $imp ShowWindow
    if { $argc > 0 } {  
	set ok [ $imp LoadImage [lindex $argv 0]  ] ; 
    }
    if { $argc > 1 } {
	set ok [ $imp LoadObjectmap [lindex $argv 1]  ] ; 
    }

    [ $imp GetViewer ] SetEnableRendering 1
    [ $imp GetViewer ] UpdateDisplay

}



