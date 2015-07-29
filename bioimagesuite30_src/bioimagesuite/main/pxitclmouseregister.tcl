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





# 	$Id: pxitclmouseregister.tcl,v 1.3 2003/02/26 16:29:58 papad Exp $	

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]

package provide pxitclmouseregister 1.0

# -----------------------
# Dependencies and path
# -----------------------

package require labelframe 1.0
package require pxtclutil 1.0
package require pxvtable 1.0
package require vtkpxcontrib  1.1
package require pxitclbaseimageviewer 1.0
package require pxitclbasecontrol 1.0
package require pxitclrpm 1.0
package require pxitclmousechopper 1.0
package require bis_guioverlayutility 1.0
# -----------------------------------------------------
#  Intialize Variables
# -----------------------------------------------------

itcl::class pxitclmouseregister {

    inherit pxitclbasecontrol

    protected variable pref_dialog 0
    protected variable orthoviewer
    protected variable simpleviewer 0
    protected variable viewername
    protected variable longviewername 
    protected variable initialized 0
    protected variable standalone 1
    public    variable overlaytool 0

    public    variable rpmtool 0
    public    variable enable_rpmtool 1
    public    variable enable_rendering_on_startup 1
    public    variable enable_simpleviewer 1
    public    variable ttable 0
    public    variable menubase 0
    private   common thisparam

    constructor { par } {
	pxitclbasecontrol::constructor $par
    } {
	InitializeMouseRegister
    }

    protected method InitializeMouseRegister { }
    public method LoadImage { f transflag } 

    public method HandleNewImage { viewer } 
    public method HandleMouseEvent { viewer args } 
    public method TransferImage { from_viewer args  } 
    public method CreateMenu { menubase } 
    public method PositionWindows {  } 
    public method Initialize { basewidget } 
    public method GetOrthoViewer { name } { return $orthoviewer($name) }
    public method ShowTTable { }
    public method ComputeTValue { mode }
    public method GrabTvalueFromViewer { viewerno }

}
# -----------------------------------------------------    
itcl::body pxitclmouseregister::InitializeMouseRegister { } {

    global pxtcl_pref_array
    
    # -----------------------------
    # Global Vtk Objects
    # -----------------------------

    set pref_dialog       [ ::pxtcluserprefs::CreatePreferenceEditor ]
    set viewername(0)     "ref"
    set viewername(1)     "trn"
    set longviewername(0) "Reference"
    set longviewername(1) "Transform"
    set orthoviewer(ref) 0
    set orthoviewer(trn) 0
    set overlaytool 0

    set ttable  0
    set thisparam($this,ttable_mathobject)  0
    set thisparam($this,ttable_tvalue)  1.323
    set thisparam($this,ttable_df)         21
    set thisparam($this,ttable_x1000)        0
    set thisparam($this,ttable_pv1)      0.10
    set thisparam($this,ttable_pv2)      0.20
    set appname "BioImage Suite::MouseRegister"
    set enable_vvlink 0

}
# ---------------------------------------------------------------------------------
#   I/O Functions
# ---------------------------------------------------------------------------------

itcl::body pxitclmouseregister::LoadImage { f transflag } {
    
    $orthoviewer($transflag) LoadImage $f 
}

# ---------------------------------------------------------------------------------
# Handle Mouse Press Callback
# ---------------------------------------------------------------------------------
itcl::body pxitclmouseregister::HandleNewImage { viewer } {

    set img [ $viewer cget -currentimage ]
    set fname [ $img cget -filename ]
    set or    [ $img GetOrientation ]

    if { $viewer == $orthoviewer(trn) } {
	set thisname  "trn"
	$overlaytool SetFunctionalImage $img 
    } else {
	$overlaytool SetImage $img 
    }
}

itcl::body pxitclmouseregister::HandleMouseEvent { viewer args } {

    # Assume it is the reference viewer
    set thisname "ref"
    set othername "trn"

    # Check whether widgetname matches transform viewer
    if { $viewer == $orthoviewer(trn) } {
	set thisname  "trn"
	set othername "ref"
    }

    # Label Viewers etc
    set thisviewer   [ $orthoviewer($thisname)  GetViewer ]
    set otherviewer  [ $orthoviewer($othername) GetViewer ]

    set lv "" 
    catch { set lv [ $thisviewer GetLastClickedPoint ] }

    if { [ llength $lv ] > 1 } {
	if { [ lindex $lv 0 ] > -1 } {
	    # PolyData Control needs scaled points
	    set lv [ $thisviewer GetLastClickedPointScaled ] 
	    set px [ lindex $lv 0 ]
	    set py [ lindex $lv 1 ]
	    set pz [ lindex $lv 2 ]
	    set upd0 [ $thisviewer GetMouseSliceChangeMode ]
	    set upd1 [ $otherviewer GetMouseSliceChangeMode ]
	    if { $upd0 == 1 && $upd1 == 1 } {
		$otherviewer SetScaledCoordinates $px $py $pz
	    } 
	}
    }
}


itcl::body pxitclmouseregister::TransferImage { from_viewer args  } {

    if { $from_viewer == "help" } {
	tk_messageBox -title "About Transfer Images" -type ok -message "The image transfered is the one currently displayed.\n It is stored as `image' on the target viewer"
    }

    if { $from_viewer == "swap" } {
	set tmpimage       [  [ pxitclimage \#auto ] GetThisPointer ]
	$tmpimage ShallowCopy [ $orthoviewer(ref) GetDisplayedImage ]
	$orthoviewer(ref) SetImageFromObject [ $orthoviewer(trn) GetDisplayedImage ] $this
	$orthoviewer(trn) SetImageFromObject $tmpimage $this
	itcl::delete object $tmpimage
	return
    }
	
    if { [ llength $args ] == 0 } {
	if { $from_viewer == "trn" || $from_viewer == "ref" } {
	    $simpleviewer SetImageFromObject [ $orthoviewer($from_viewer) GetDisplayedImage ] $this
	    $simpleviewer SetLookupTable [ [ $orthoviewer($from_viewer) GetViewer  ] GetLookupTable ]
	    [ $simpleviewer GetViewer ] UpdateDisplay
	}
    } else {
	set to_viewer [ lindex $args 0 ]
	if { ( $from_viewer == "trn" || $from_viewer == "ref" ) &&
	     ( $to_viewer == "trn" || $to_viewer == "ref" ) && 
	     ( $from_viewer != $to_viewer) } {
	    $orthoviewer($to_viewer) SetImageFromObject [ $orthoviewer($from_viewer) GetDisplayedImage ] $this
	    $orthoviewer($to_viewer) SetLookupTable [ [ $orthoviewer($from_viewer) GetViewer  ] GetLookupTable ]
	    [ $orthoviewer($to_viewer) GetViewer ] UpdateDisplay
	}
    }
}

# ---------------------------------------------------------------------------------
# Create Menu
# ---------------------------------------------------------------------------------

itcl::body pxitclmouseregister::CreateMenu { menubase } {

    
    set viewm [ menu $menubase.view -tearoff 0 ];     $menubase add cascade -underline 0 -label "Viewers" -menu $viewm
    set trm   [ menu $menubase.tr   -tearoff 0 ];     $menubase add cascade -underline 0 -label "Transfer" -menu $trm


    if { $overlaytool !=0 } {
	set regm [ menu $menubase.regm -tearoff 0 ] ; $menubase add cascade -underline 0 -label "Registration"    -menu $regm ;
	$overlaytool AddToMenuButton $regm
	
	if {  $rpmtool !=0 } {
	    $regm add separator
	    eval "$regm add command -label \"Point-Based Registration\" -command { $orthoviewer(ref) HideWindow; $orthoviewer(trn) HideWindow ; $rpmtool ShowWindow }"
	}
    }
    


    eval "$trm add command  -label \"Help with Transfer Features\"  -command { $this TransferImage help help } "
    $trm add separator
    eval "$trm add command  -label \"Displayed Image Reference->Transform\"  -command { $this TransferImage ref trn } "
    eval "$trm add command  -label \"Displayed Image Transform->Reference\"  -command { $this TransferImage trn ref } "
    $trm add separator
    eval "$trm add command  -label \"Swap Displayed Transform->Reference\"  -command  { $this TransferImage  swap } "


    eval "$viewm add command -label \"Reference Viewer\" -command { $orthoviewer(ref) ShowWindow; $this PositionWindows}"
    eval "$viewm add command -label \"Transform Viewer\" -command { $orthoviewer(trn) ShowWindow ; $this PositionWindows}"
    
    if { $enable_simpleviewer == 1 } {
	$viewm add separator
	eval "$viewm add command -label \"Simple Viewer\" -command { $simpleviewer ShowWindow }"

	set mbar [ $simpleviewer cget -menubase ]
	$mbar delete 0 1
	set mb [ menu $mbar.dispm -tearoff 0 ]; $mbar add cascade -underline 0 -label Display -menu $mb

	eval "$mb add command  -label \"Grab Image From Reference Viewer\"  -command { $this TransferImage ref } "
	eval "$mb add command  -label \"Grab Image From Transform Viewer\"  -command { $this TransferImage trn } "
	$mb add separator
	eval "$mb add command -label Close -command {  $simpleviewer HideWindow } -underline 1"
    }

    $viewm add separator
    if { $enable_simpleviewer == 1 } {
	eval "$viewm add command -label \"Close Viewers\" -command { $orthoviewer(ref) HideWindow; $orthoviewer(trn) HideWindow ; $simpleviewer HideWindow }"
    } else {
	eval "$viewm add command -label \"Close Viewers\" -command { $orthoviewer(ref) HideWindow; $orthoviewer(trn) HideWindow  }"
    }

    $viewm add separator
    if { $standalone == 1 } {
	$viewm add command -label Exit -command {  pxtkcallback 0 0 } -underline 1
    } else {
	eval "$viewm add command -label Close -command { wm deiconify $basewidget }"
    }

    set helpm [ menu $menubase.help -tearoff 0 ];     $menubase add cascade -underline 0 -label "Help" -menu $helpm


    eval "$helpm add command -label Preferences -command { wm deiconify $pref_dialog }"
    $helpm add separator
    eval "$helpm add command -label \"T-distribution table\" -command {  $this ShowTTable }"
    $helpm add separator
    $helpm add command -label "Console" -command {  pxtkconsole }

    eval "$helpm add command -label About -command { $this AboutCommand }"

}

# ---------------------------------------------------------------------------------

itcl::body pxitclmouseregister::PositionWindows {  } {
    
    set geom [ winfo geometry $basewidget ] 
    scan $geom "%dx%d+%d+%d" w0 h0 x0 y0


    set ref [ $orthoviewer(ref) GetBaseWidget ]
    set trn [ $orthoviewer(trn) GetBaseWidget ]

    set mult 0
    set ok 0
    if { $overlaytool != 0 } {
	set mult [ $overlaytool GetBaseWidget ]
	catch { set ok   [ winfo ismapped $mult ] }
    } else {
	set ok 0
    }

    if { $ok == 0 } {
	set y0 [ expr $y0 + $h0 + 25 ]
	set y1 $y0 
	set x1 [ expr $x0 + 550 ]
	set hgt 550
    } else {
	set y0 [ expr $y0 + $h0 + 15 ]
	set x1 $x0
	set y1 [ expr $y0 + 450 ]
	set x2 [ expr $x0 + 580 ]
	set y2 $y0
	set hgt 450
    }

    if  { [ winfo ismapped $ref ] } {
	wm geometry $ref "550x${hgt}+$x0+$y0"
	raise $ref
	if  { [ winfo ismapped $trn ] } {
	    wm geometry $trn "550x${hgt}+$x1+$y1"
	    raise $trn
	}
    } else {
	if  { [ winfo ismapped $trn ] } {
	    wm geometry $trn "550x${hgt}+$x0+$y0"
	    raise $trn
	}
    }

    if { $ok == 1 } {
	wm geometry $mult "+$x2+$y2"
	raise $mult
    }
	
}


itcl::body pxitclmouseregister::ShowTTable { } {

    if { $ttable == 0 } {

	set ttable [ toplevel $basewidget.[pxvtable::vnewobj ]]

	wm geometry $ttable 400x100
	wm title $ttable "Critical Points of the T-distribution"
	eval "wm protocol $ttable WM_DELETE_WINDOW { wm withdraw $ttable }"

	frame $ttable.top
	frame $ttable.top2
	frame $ttable.bot
	pack $ttable.top $ttable.top2 $ttable.bot -side top -expand false -fill x -pady 2

	iwidgets::entryfield $ttable.top.1 -labeltext "T-value:"  -width 6 -validate real -textvariable [ itcl::scope thisparam($this,ttable_tvalue) ] -relief sunken 
	checkbutton   $ttable.top.15  -text "x1000"  -variable [ itcl::scope thisparam($this,ttable_x1000) ]
	iwidgets::entryfield $ttable.top.2 -labeltext "Degr. of Fredom:"  -width 3 -validate integer -textvariable [ itcl::scope thisparam($this,ttable_df) ] -relief sunken 


	iwidgets::entryfield $ttable.top2.3 -labeltext "One Tail:"  -width 6 -validate real -textvariable [ itcl::scope thisparam($this,ttable_pv1) ] -relief raised 
	iwidgets::entryfield $ttable.top2.4 -labeltext "Two Tail:"  -width 6 -validate real -textvariable [ itcl::scope thisparam($this,ttable_pv2) ] -relief sunken

	pack $ttable.top.1 $ttable.top.15 $ttable.top.2  -side left -expand true -fill x
	pack $ttable.top2.3 $ttable.top2.4  -side left -expand true -fill x


	eval "button $ttable.bot.1 -text  \"T-> P\" -command { $this ComputeTValue forward }"
	eval "button $ttable.bot.15 -text \"P(2t) -> T\" -command { $this ComputeTValue  reverse }"
	eval "button $ttable.bot.2 -text \"Grab Ref\" -command { $this GrabTvalueFromViewer ref }"
	eval "button $ttable.bot.3 -text \"Grab Trn\" -command { $this GrabTvalueFromViewer trn }"
	eval "button $ttable.bot.4 -text \"Close\"   -command { wm withdraw $ttable }"

	pack $ttable.bot.4 -side right -padx 3
	pack $ttable.bot.1 $ttable.bot.15 $ttable.bot.2 $ttable.bot.3 -side left -padx 1


	set thisparam($this,ttable_mathobject) [ vtkpxMath [ pxvtable::vnewobj ]]
    }

    wm deiconify $ttable
}

itcl::body pxitclmouseregister::GrabTvalueFromViewer { viewername } {

    set thisviewer [  $orthoviewer($viewername) GetViewer ]
    set lv [ $thisviewer GetLastClickedPoint ] 
    set fr [ $thisviewer GetCurrentFrame ] 
    set px [ lindex $lv 0 ]
    set py [ lindex $lv 1 ]
    set pz [ lindex $lv 2 ]
    set img [ $thisviewer GetImage ] 

    if { $img != "" } {
	set thisparam($this,ttable_tvalue) [ $img GetScalarComponentAsDouble $px $py $pz $fr ]
	set thisparam($this,ttable_x1000)  1
	ComputeTValue forward
    }
}

itcl::body pxitclmouseregister::ComputeTValue { mode } {

    if { $mode == "forward" } {
	if { $thisparam($this,ttable_x1000) == 1 } {
	    set x [ expr 0.001 * $thisparam($this,ttable_tvalue) ]
	} else {
	    set x  $thisparam($this,ttable_tvalue) 
	}
	
	set thisparam($this,ttable_pv2) [ $thisparam($this,ttable_mathobject) TtestToPvalue $x $thisparam($this,ttable_df) ]
	set thisparam($this,ttable_pv1) [ expr $thisparam($this,ttable_pv2) * 0.5 ]
    } else {
	set x $thisparam($this,ttable_pv2)
	set thisparam($this,ttable_pv1) [ expr $thisparam($this,ttable_pv2) * 0.5 ]
	
	set t [ $thisparam($this,ttable_mathobject) PvalueToTvalue $x $thisparam($this,ttable_df) ]
	if { $thisparam($this,ttable_x1000) == 1 } {
	    set t [ expr $t * 1000.0 ]
	}
	set thisparam($this,ttable_tvalue) $t
    }
   

    update idletasks
}




# ---------------
# User Interface 
# ---------------

itcl::body pxitclmouseregister::Initialize { basewidg } {

    if { $initialized == 1 } { return $basewidget }

    set basewidget [ toplevel $basewidg ]; 
    wm geometry $basewidg 700x40+10+10

    
    
    SetTitle "BioImage Suite::Mouse Register"
    set standalone 1


    set menubase [ menu $basewidget.pxtclbrainreg  ]
    $basewidget configure -menu $menubase
    set pbar     [ frame $basewidget.status -width 400 -height 30 ]
    pack  $pbar -side top -fill x 
 
    CreateProgressBar  $pbar

    set orthoviewer(ref) [ [ pxitclbaseimageviewer \#auto 0 ]  GetThisPointer ]
    $orthoviewer(ref) configure -enable_overlaytool 0 
    $orthoviewer(ref) configure -appname "Reference Viewer"
    $orthoviewer(ref) configure -show_standard_images 2
    $orthoviewer(ref) configure -enable_talaraichoption 0
    $orthoviewer(ref) configure -enable_vvlinkgadget 0
    $orthoviewer(ref) configure -enable_brainstriptool 2
    $orthoviewer(ref) configure -enable_expfittool 1
    $orthoviewer(ref) configure -enable_landmarkcontrol  1
    $orthoviewer(ref) configure -enable_rendering_on_startup  $enable_rendering_on_startup  
    $orthoviewer(ref) InitializeObjectmapViewer $basewidget.[pxvtable::vnewobj] 0
    [ $orthoviewer(ref) GetViewer ] SetEnableRendering 0

    eval "$orthoviewer(ref) configure -mousecallback \"$this HandleMouseEvent \""
    #eval "$orthoviewer(ref) configure -setnewimagecallback \"$this HandleNewImage\""
    $orthoviewer(ref) ShowWindow

    set orthoviewer(trn) [ [ pxitclbaseimageviewer \#auto 0 ]  GetThisPointer ]
    $orthoviewer(trn) configure -show_standard_images 2
    $orthoviewer(trn) configure -enable_overlaytool 0
    $orthoviewer(trn) configure -enable_brainstriptool 2
    $orthoviewer(trn) configure -appname "Transform Viewer"
    $orthoviewer(trn) configure -enable_talaraichoption 0
    $orthoviewer(trn) configure -enable_expfittool 1
    $orthoviewer(trn) configure -enable_vvlinkgadget 0
    $orthoviewer(trn) configure -enable_landmarkcontrol  1
    $orthoviewer(trn) configure -enable_rendering_on_startup  $enable_rendering_on_startup  
    $orthoviewer(trn) InitializeObjectmapViewer .[pxvtable::vnewobj] 0
    [ $orthoviewer(trn) GetViewer ] SetEnableRendering 0
    eval "$orthoviewer(trn) configure -mousecallback \"$this HandleMouseEvent \""
    $orthoviewer(trn) ShowWindow

    set chopper [ [ pxitclmousechopper \#auto $orthoviewer(ref) ] GetThisPointer ]
    $chopper configure -appname "Reference Virtual Mouse Butcher"
    $chopper AttachToBaseControl $orthoviewer(ref)


    set chopper2 [ [ pxitclmousechopper \#auto $orthoviewer(trn) ] GetThisPointer ]
    $chopper2 configure -appname "Transform Virtual Mouse Butcher"
    $chopper2 AttachToBaseControl $orthoviewer(trn)



    if { $enable_simpleviewer == 1 } {
	set simpleviewer [ [ pxitclbaseimageviewer \#auto 0 ] GetThisPointer ]
	$simpleviewer configure -appname "Simple Viewer"
	$simpleviewer configure -show_standard_images 0
	$simpleviewer configure -enable_multisubjectcontrol 0
	$simpleviewer configure -enable_csigadget 0
	$simpleviewer configure -enable_rendering_on_startup  0
	$simpleviewer configure -enable_headercontrol  0
	$simpleviewer configure -enable_importcontrol  0
	$simpleviewer configure -enable_imageutility  0
	$simpleviewer configure -enable_landmarkcontrol  0
	$simpleviewer configure -enable_overlaytool  0
	$simpleviewer configure -enable_histcontrol  0
	$simpleviewer configure -enable_displaymenu  0
	$simpleviewer configure -enable_brainstriptool  0
	$simpleviewer configure -enable_helpmenu  0
	$simpleviewer configure -enable_talaraichoption 0
	$simpleviewer InitializeMosaicViewer .[pxvtable::vnewobj ] 0
    }

    set overlaytool [ [ bis_guioverlayutility \#auto ] GetThisPointer ]
    $overlaytool SetSecondContainerObject $orthoviewer(ref)
    $overlaytool InitializeFromContainer $orthoviewer(trn)
    $overlaytool CreateGUI $basewidget.[pxvtable::vnewobj ]
    $orthoviewer(ref) AddControl $overlaytool
    $orthoviewer(trn) AddControl $overlaytool

    [ $orthoviewer(ref) GetLandmarkControl ] SetTitle "Reference Landmarks"
    [ $orthoviewer(ref) GetPolyDataControl ] SetTitle "Reference Surfaces"
    [ $orthoviewer(trn) GetLandmarkControl ] SetTitle "Transform Landmarks"
    [ $orthoviewer(trn) GetPolyDataControl ] SetTitle "Transform Surfaces"


    if { $enable_rpmtool  == 1 } {
	set rpmtool [ [ pxitclrpm \#auto $this [ $orthoviewer(ref) GetPolyDataControl ] [ $orthoviewer(trn) GetPolyDataControl ] $overlaytool [ $orthoviewer(trn) GetLandmarkControl ] ]  GetThisPointer ] 
	$rpmtool configure -enablearticulated 1
	$rpmtool Initialize $basewidget.[pxvtable::vnewobj ]
    }

    CreateMenu   $menubase

    update idletasks
    PositionWindows
    set initialized 1

    eval "bind all <Control-d>  { pxtkexit }"
    wm protocol . WM_DELETE_WINDOW pxtkexit 

    return $basewidget

}


if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {

    wm withdraw .
    set breg [ pxitclmouseregister \#auto 0 ]
    $breg configure -enable_rendering_on_startup  0
    $breg Initialize .[pxvtable::vnewobj ]
    
    set argc [llength $argv]; set ok 0
    if { $argc > 0 } {  set ok [ $breg LoadImage [lindex $argv 0]  "ref" ] }
    if { $argc > 1 } {  set ok [ $breg LoadImage [lindex $argv 1]  "trn" ] }
    if { $argc > 2 } {  
	[ $breg cget -overlaytool ] LoadAndAddTransformation [ lindex $argv 2 ]
    } 
    if { $argc > 3 } {
	set ok [ [ [ $breg GetOrthoViewer ref ] GetPolyDataControl ] LoadSurface [lindex $argv 3] ]
    }
    if { $argc > 4 } {
	set ok [ [ [ $breg GetOrthoViewer trn ] GetPolyDataControl ] LoadSurface [lindex $argv 4] ]
    }


    [ [ $breg GetOrthoViewer ref ] GetViewer ] SetEnableRendering 1
    [ [ $breg GetOrthoViewer trn ] GetViewer ] SetEnableRendering 1
    [ [ $breg GetOrthoViewer trn ] GetViewer ] UpdateDisplay
    [ [ $breg GetOrthoViewer ref ] GetViewer ] UpdateDisplay


}





