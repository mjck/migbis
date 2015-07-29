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





# 	$Id: pxitclbrainregister.tcl,v 1.3 2003/02/26 16:29:58 papad Exp $	

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] bis_algorithm ]

package provide pxitclbrainregister 1.0

# -----------------------
# Dependencies and path
# -----------------------

package require pxtcluserprefs 1.0
package require labelframe 1.0
package require pxtclutil 1.0
package require pxvtable 1.0

package require pxitclbaseimageviewer 1.0
package require pxitclbasecontrol 1.0
package require pxitclmanualtransformation 1.0
package require pxitclmultisubjectaverage 1.0
package require pxitcldistributiontablegadget 1.0
package require bis_guioverlayutility 1.0
package require bis_guirpmcontainer 1.0
package require bis_fmri 1.0
package require pxitclElectrodeMultiControl 1.0
# -----------------------------------------------------
#  Intialize Variables
# -----------------------------------------------------

itcl::class pxitclbrainregister {

    inherit pxitclbasecontrol

    protected variable pref_dialog 0
    protected variable orthoviewer
    protected variable simpleviewer 0
    protected variable viewername
    protected variable longviewername 
    protected variable initialized 0
    protected variable standalone 1
    public    variable overlaytool 0
    public    variable manualtransformationtool 0
    public    variable rpmtool 0
    public    variable csigadget 0
    public    variable fmrigadget 0
    public    variable multigadget 0
    public    variable multi2gadget 0
    public    variable electrode_control 0
    public    variable electrode_control2 0
    public    variable enable_multi2gadget 0
    public    variable enable_multigadget 1
    public    variable enable_rpmtool 1
    public    variable enable_manualtransformation 1
    public    variable enable_csigadget 0
    public    variable enable_fmrigadget 1
    public    variable enable_electrodecontrol 0
    public    variable enable_electrodecontrol2 0
    public    variable enable_rendering_on_startup 1
    public    variable enable_simpleviewer 1
    public    variable enable_vvlink 1
    public    variable menubase 0
    public    variable functionalmenu 0
    public    variable donotcreatemenu 0
    public    variable tdistgadget 0

    private   common thisparam

    constructor { par } {
	pxitclbasecontrol::constructor $par
    } {
	InitializeBrainRegister
    }

    protected method InitializeBrainRegister { }
    public method LoadImage { f transflag } 

    public method HandleNewImage { viewer } 
    public method HandleMouseEvent { viewer args } 
    public method TransferImage { from_viewer args  } 
    public method SimpleViewerGrabImage { from_viewer index  } 
    public method CreateMenu { menubase }
    public method CreateChildMenus { { viewerbase 0 } { registrationbase 0 } { functionalbase 0 } { multiplebase 0 } { helpbase 0 } }
    public method CreateSimpleViewerMenu { viewm }
    public method PositionWindows {  } 
    public method Initialize { basewidget } 
    public method GetOrthoViewer { name } { return $orthoviewer($name) }
    public method ShowTTable { }
    public method HideViewers { }

}
# -----------------------------------------------------    
itcl::body pxitclbrainregister::InitializeBrainRegister { } {

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

    set appname "BioImage Suite::BrainRegister"

}
# ---------------------------------------------------------------------------------
#   I/O Functions
# ---------------------------------------------------------------------------------

itcl::body pxitclbrainregister::LoadImage { f transflag } {
    
    $orthoviewer($transflag) LoadImage $f 
}

# ---------------------------------------------------------------------------------
# Handle Mouse Press Callback
# ---------------------------------------------------------------------------------
itcl::body pxitclbrainregister::HandleNewImage { viewer } {

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




itcl::body pxitclbrainregister::HandleMouseEvent { viewer args  } {

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
		$otherviewer CopyCamera $thisviewer
		#		puts stderr "Setting ScaledCoordinates $px $py $pz"
		$otherviewer SetScaledCoordinates $px $py $pz
	    } 
	}
    }
	
    set upd1 [ $otherviewer GetMouseSliceChangeMode ]
    if { $upd1 ==1 } {
	$otherviewer CopyCamera $thisviewer
	$otherviewer Update
    }
    
    
}


itcl::body pxitclbrainregister::TransferImage { from_viewer args  } {

    if { $from_viewer == "help" } {
	tk_messageBox -title "About Transfer Images" -type ok -message "The image transfered is the one currently displayed.\n It is stored as \"image\" on the target viewer"
    }
    
    if { $from_viewer == "swap" } {
	set tmpimage       [  [ pxitclimage \#auto ] GetThisPointer ]
	$tmpimage ShallowCopy [ $orthoviewer(ref) GetDisplayedImage ]
	$orthoviewer(ref) SetImageFromObject [ $orthoviewer(trn) GetDisplayedImage ] $this
	$orthoviewer(trn) SetImageFromObject $tmpimage $this
	itcl::delete object $tmpimage
	return
    }
    
    if { [ llength $args ] > 0 } {
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

itcl::body pxitclbrainregister::SimpleViewerGrabImage { from_viewer index  } {


    set simpvtkviewer [ $simpleviewer GetViewer ]
    if { $from_viewer == "Clear" } {
	$simpvtkviewer ClearAuxImages
	return
    }

    
    if { $from_viewer != "trn" && $from_viewer != "ref" } {
	return
    }

    if { $index == 0 } {
	$simpleviewer SetImageFromObject [ $orthoviewer($from_viewer) GetDisplayedImage ] $this
	$simpleviewer SetLookupTable     [ [ $orthoviewer($from_viewer) GetViewer  ] GetLookupTable ]
	$simpleviewer SetValueImage      [ [ $orthoviewer($from_viewer) GetViewer  ] GetValueImage  ]
    } else {
	$simpvtkviewer SetAuxImage [ [ $orthoviewer($from_viewer) GetDisplayedImage ] GetImage ] [ expr $index -1 ]
    }

    $simpvtkviewer UpdateDisplay
    return

}


itcl::body pxitclbrainregister::HideViewers { } { 

    catch { 
	$orthoviewer(ref) HideWindow; 
	$orthoviewer(trn) HideWindow
	$simpleviewer     HideWindow;
    }
}

# ---------------------------------------------------------------------------------
# Create Menu
# ---------------------------------------------------------------------------------
itcl::body pxitclbrainregister::CreateSimpleViewerMenu { viewm } {

    eval "$viewm add command -label \"Simple Viewer\" -command { $simpleviewer ShowWindow }"
    
    set mbar [ $simpleviewer cget -menubase ]
    $mbar delete 0 1
    set mb [ menu $mbar.dispm -tearoff 0 ]; $mbar add cascade -underline 0  -label Display -menu $mb
    set mb2 [ menu $mbar.atlas -tearoff 0 ]; $mbar add cascade -underline 0  -label "Atlas Tool" -menu $mb2

    
    eval "$mb add command  -label \"Grab Image From Reference Viewer\"  -command { $this SimpleViewerGrabImage ref 0 } "
    eval "$mb add command  -label \"Grab Image From Transform Viewer\"  -command { $this SimpleViewerGrabImage trn 0 } "

    set vname [ list ref trn ]
    set name [ list Reference Transform ]

    for { set j 0 } { $j <=1 } { incr j } {
	$mb add separator
	for { set i 1 } { $i <= 4 } { incr i } {
	    eval "$mb add command  -label \"Grab Aux Image $i From [ lindex $name $j ] Viewer\"  -command { $this SimpleViewerGrabImage  [ lindex $vname $j ] $i } "
	}
    }
    $mb add separator
    eval "$mb add command -label \"Clear Aux Images\" -command {  $this SimpleViewerGrabImage Clear -1 } -underline 1"
    eval "$mb add command -label Close -command {  $simpleviewer HideWindow } -underline 0"
    [ $simpleviewer GetAtlasTool ] AddToMenuButton $mb2
}


itcl::body pxitclbrainregister::CreateMenu { menubase } {

#    menubutton $menubase.view      -text Viewers      -menu $menubase.view.m -underline 0
#    menubutton $menubase.tr      -text Transfer      -menu $menubase.tr.m -underline 0
#    menubutton $menubase.help      -text Help         -menu $menubase.help.m -underline 0 -padx 4
#    pack $menubase.view $menubase.tr  -side left
#    pack $menubase.help  -side right

    
    set viewm [ menu $menubase.view -tearoff 0 ];     $menubase add cascade -underline 0  -label "Viewers" -menu $viewm
    set trm   [ menu $menubase.tr   -tearoff 0 ];     $menubase add cascade -underline 0  -label "Transfer" -menu $trm


    if { $overlaytool !=0 } {
	set regm [ menu $menubase.regm -tearoff 0 ] ; $menubase add cascade -underline 0  -label "Registration"    -menu $regm ;
	$overlaytool AddToMenuButton $regm
	
	if { $manualtransformationtool !=0 } {
	    $regm add separator
	    eval "$regm add command -label \"Manual Registration II\" -command { $manualtransformationtool ShowWindow }"
	}

	if {  $rpmtool !=0 } {
	    $regm add separator
	    eval "$regm add command -label \"Point-Based Registration\" -command { $this HideViewers;  $rpmtool ShowWindow }"
	}
    }
    
    if { $fmrigadget !=0 || $csigadget !=0  } {

	set mb [ menu $menubase.fmri -tearoff 0 ]
	set functionalmenu $mb
	$menubase add cascade -underline 0  -label "Functional"  -menu $mb

	if { $fmrigadget !=0 }  { $fmrigadget AddToMenuButtonLite $mb }
	if { $csigadget !=0 }   { $csigadget AddToMenuButtonLite $mb;  }

	if { $electrode_control !=0 } {
	    $mb add separator
	    eval "$mb add command -label \"Electrode Control (Transform Viewer)\" -command { $electrode_control ShowWindow }"
	}
	if { $electrode_control2 !=0 } {
	    eval "$mb add command -label \"Electrode Control 2 (Transform Viewer)\" -command { $electrode_control2 ShowWindow }"
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
	CreateSimpleViewerMenu $viewm
    }

    $viewm add separator
    eval "$viewm add command -label \"Close Viewers\" -command { $this HideViewers  }"
	
    $viewm add separator
    if { $standalone == 1 } {
	    eval "$viewm add command -label Exit -command {  $this ExitCommand } -underline 1"
    } else {
	eval "$viewm add command -label Close -command { wm deiconify $basewidget }"
    }
    

    if { $multigadget !=0 } {
	set multi1 [ menu $menubase.multi1 -tearoff 0 ];     $menubase add cascade -underline 0  -label "Multi-subject" -menu $multi1
	$multigadget AddToMenuButton $multi1 "$this PositionWindows"
    }

    if { $multi2gadget !=0 } {
	set multi2 [ menu $menubase.multi2 -tearoff 0 ];     $menubase add cascade -underline 1  -label "Multi-subject 2" -menu $multi2
	$multi2gadget AddToMenuButton $multi2 "$this PositionWindows"
    }

    set helpm [ menu $menubase.help -tearoff 0 ];     $menubase add cascade -underline 0  -label "Help" -menu $helpm

    eval "$helpm add command -label \"Help Index\" -command { $this HelpCommand }"
    eval "$helpm add separator"
    eval "$helpm add command -label Preferences -command { wm deiconify $pref_dialog }"
    $helpm add separator
    eval "$helpm add command -label \"T-distribution table\" -command {  $this ShowTTable }"
    $helpm add separator
    $helpm add command -label "Console" -command {  pxtkconsole }
    $helpm add separator
    eval "$helpm add command -label \"Check for Updates\" -command { $this CheckForUpdates }"
    $helpm add separator
    eval "$helpm add command -label About -command { $this AboutCommand }"


}

# ---------------------------------------------------------------------------------

itcl::body pxitclbrainregister::CreateChildMenus { { viewerbase 0 } { registrationbase 0 } { functionalbase 0 } { multiplebase 0 } { helpbase 0 } } {

    if { $viewerbase == 0 } {
	set viewm [ menu $menubase.view -tearoff 0 ];     $menubase add cascade -underline 0  -label "Viewers" -menu $viewm
    } else {
	set viewm $viewerbase
    }


    if { $overlaytool !=0 } {
	if { $registrationbase == 0 } {
	    set regm [ menu $menubase.regm -tearoff 0 ] ; $menubase add cascade -underline 0  -label "Registration"    -menu $regm ;
	} else { 
	    set regm $registrationbase
	}
	
	$overlaytool AddToMenuButton $regm

	
	if {  $rpmtool !=0 } {
	    $regm add separator
	    eval "$regm add command -label \"Point-Based Registration\" -command { $this HideViewers; $rpmtool ShowWindow }"
	}
    }
    
    if { $fmrigadget !=0 || $csigadget !=0 } { 

	if { $functionalbase == 0 } {
	    set mb [ menu $menubase.fmri -tearoff 0 ]
	    set functionalmenu $mb
	    $menubase add cascade -underline 0  -label "Functional"  -menu $mb
	} else {
	    set functionalmenu $functionalbase
	}

	if { $fmrigadget !=0 }  { $fmrigadget AddToMenuButtonLite $functionalmenu }
	if { $csigadget !=0 }   { $csigadget AddToMenuButtonLite $functionalmenu;  }
	if { $electrode_control !=0 } {
	    $functionalmenu add separator
	    eval "$functionalmenu add command -label \"Electrode Control (Transform Viewer)\" -command { $electrode_control ShowWindow }"
	}
	if { $electrode_control2 !=0 } {
	    eval "$functionalmenu add command -label \"Electrode Control 2 (Transform Viewer)\" -command { $electrode_control2 ShowWindow }"
	}

    }





    eval "$viewm add command -label \"Reference Viewer\" -command { $orthoviewer(ref) ShowWindow }"
    eval "$viewm add command -label \"Transform Viewer\" -command { $orthoviewer(trn) ShowWindow }"
    
    if { $enable_simpleviewer == 1 } {
	$viewm add separator
	CreateSimpleViewerMenu $viewm
    }

    $viewm add separator
    eval "$viewm add command -label \"Close Viewers\" -command { $this HideViewers }"
 	

    if { $multigadget !=0 } {
	if { $multiplebase == 0 } {
	    set multi1 [ menu $menubase.multi1 -tearoff 0 ];     $menubase add cascade -underline 0  -label "Multi-subject" -menu $multi1
	} else {
	    set multi1 $multiplebase
	}
	$multigadget AddToMenuButton $multi1 "$this PositionWindows"
    }

    if { $multi2gadget !=0 } {
	if { $multiplebase == 0 } {
	    set multi2 [ menu $menubase.multi2 -tearoff 0 ];     $menubase add cascade -underline 1  -label "Multi-subject 2" -menu $multi2
	} else {
	    set multi2 $multiplebase
	}
	$multi2gadget AddToMenuButton $multi2 "$this PositionWindows"
    }

    if { $helpbase == 0 } {
	set helpm [ menu $menubase.help -tearoff 0 ];     $menubase add cascade -underline 0  -label "Help" -menu $helpm
    } else {
	set helpm $helpbase
    }


    eval "$helpm add command -label Preferences -command { wm deiconify $pref_dialog }"
    $helpm add separator
    eval "$helpm add command -label \"T-distribution table\" -command {  $this ShowTTable }"
    $helpm add separator
    

    $helpm add command -label "Console" -command {  pxtkconsole }
    $helpm add separator
    eval "$helpm add command -label \"Check for Updates\" -command { $this CheckForUpdates }"
    $helpm add separator
    eval "$helpm add command -label About -command { $this AboutCommand }"


}

# ---------------------------------------------------------------------------------
itcl::body pxitclbrainregister::ShowTTable {  } {

    if {  $tdistgadget == 0 } {

	set tdistgadget [ [ pxitcldistributiontablegadget \#auto $this ] GetThisPointer ]
	$tdistgadget RegisterViewer $orthoviewer(ref) "Ref"
	$tdistgadget RegisterViewer $orthoviewer(trn) "Trn"
	$tdistgadget Initialize $basewidget.[pxvtable::vnewobj ]
    }
    $tdistgadget ShowWindow

}
# ---------------------------------------------------------------------------------
itcl::body pxitclbrainregister::PositionWindows {  } {
    
    set geom [ winfo geometry $basewidget ] 
    scan $geom "%dx%d+%d+%d" w0 h0 x0 y0


    set ref [ $orthoviewer(ref) GetBaseWidget ]
    set trn [ $orthoviewer(trn) GetBaseWidget ]

    set mult 0
    set ok 0
    if { $enable_multigadget } {
	set mult [ $multigadget GetBaseWidget ] 
	catch { set ok   [ winfo ismapped $mult ] }
    }  elseif { $overlaytool != 0 } {
	set mult [ $overlaytool GetBaseWidget ]
	catch { set ok   [ winfo ismapped $mult ] }
    } else {
	set ok 0
    }

    if { $ok == 0 } {
	set y0 [ expr $y0 + $h0 + 25 ]
	set y1 $y0 
	set x1 [ expr $x0 + 550 ]
	set hgt 600
    } else {
	set y0 [ expr $y0 + $h0 + 15 ]
	set x1 $x0
	set y1 [ expr $y0 + 450 ]
	set x2 [ expr $x0 + 580 ]
	set y2 $y0
	set hgt 600
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






# ---------------
# User Interface 
# ---------------

itcl::body pxitclbrainregister::Initialize { basewidg } {

    if { $initialized == 1 } { return $basewidget }

    set basewidget [ toplevel $basewidg ]; 
    wm geometry $basewidg 700x40+10+10

    
    
    SetTitle "Brain Register"
    set standalone 1


    set menubase [ menu $basewidget.pxtclbrainreg -tearoff 0 ]
    $basewidget configure -menu $menubase
    set pbar     [ frame $basewidget.status -width 400 -height 30 ]
    pack  $pbar -side bottom -fill x 
 
    CreateProgressBar  $pbar

    set orthoviewer(ref) [ [ pxitclbaseimageviewer \#auto 0 ]  GetThisPointer ]
    $orthoviewer(ref) configure -enable_overlaytool 0 
    $orthoviewer(ref) configure -appname "Reference Viewer"
    $orthoviewer(ref) configure -show_standard_images 1
    $orthoviewer(ref) configure -enable_talaraichoption 1
    $orthoviewer(ref) configure -enable_vvlinkgadget $enable_vvlink 
    $orthoviewer(ref) configure -enable_rendering_on_startup  $enable_rendering_on_startup  
    $orthoviewer(ref) InitializeOrthogonalViewer $basewidget.[pxvtable::vnewobj] 0
    [ $orthoviewer(ref) GetViewer ] SetEnableRendering 0

    #    [ $orthoviewer(ref) cget -atlastool ] SetTitle "Ref Viewer Atlas Tool"

    eval "$orthoviewer(ref) configure -mousecallback \"$this HandleMouseEvent \""
    #eval "$orthoviewer(ref) configure -setnewimagecallback \"$this HandleNewImage\""
    $orthoviewer(ref) ShowWindow

    set orthoviewer(trn) [ [ pxitclbaseimageviewer \#auto 0 ]  GetThisPointer ]
    $orthoviewer(trn) configure -show_standard_images 1
    $orthoviewer(trn) configure -enable_overlaytool 0
    $orthoviewer(trn) configure -appname "Transform Viewer"
    $orthoviewer(trn) configure -enable_talaraichoption 1
    $orthoviewer(trn) configure -enable_vvlinkgadget $enable_vvlink 
    $orthoviewer(trn) configure -enable_rendering_on_startup  $enable_rendering_on_startup  
    $orthoviewer(trn) SetAtlasTool [ $orthoviewer(ref) GetAtlasTool ]
    $orthoviewer(trn) configure -enable_internaleditor 1
    $orthoviewer(trn) configure -enable_objectmapmenu   1

    $orthoviewer(trn) InitializeObjectmapViewer .[pxvtable::vnewobj] 0
    [ $orthoviewer(trn) GetViewer ] SetEnableRendering 0
    #    [ $orthoviewer(trn) cget -atlastool ] SetTitle "Ref Viewer Atlas Tool"
    eval "$orthoviewer(trn) configure -mousecallback \"$this HandleMouseEvent \""
    $orthoviewer(trn) ShowWindow

    if { $enable_simpleviewer == 1 } {
	set simpleviewer [ [ pxitclbaseimageviewer \#auto 0 ] GetThisPointer ]
	$simpleviewer configure -appname "Simple Viewer"
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
	$simpleviewer SetAtlasTool [ $orthoviewer(ref) GetAtlasTool ]
	$simpleviewer InitializeMultipleMosaicViewer .[pxvtable::vnewobj ] 0

    }

    if { $enable_electrodecontrol == 1 } {
	set electrode_control [ [ pxitclElectrodeMultiControl \#auto $this ]  GetThisPointer ]
	$electrode_control SetNoMouseInput
	$electrode_control Initialize $basewidget.[ pxvtable::vnewobj ] [ $orthoviewer(trn) GetViewer ]
	$electrode_control HideWindow
	$electrode_control ShowGrids none

	if { $enable_electrodecontrol2 == 1 } {
	    set electrode_control2 [ [ pxitclElectrodeMultiControl \#auto $this ]  GetThisPointer ]
	    $electrode_control2 SetNoMouseInput
	    $electrode_control2 Initialize $basewidget.[ pxvtable::vnewobj ] [ $orthoviewer(trn) GetViewer ]
	    $electrode_control2 HideWindow
	    $electrode_control2 SetTitle "Second Electrode Editor"
	    $electrode_control2 ShowGrids none
	}
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



    if { $enable_csigadget ==1 } {
	set  csigadget [ [  pxitclcsigadget \#auto $orthoviewer(ref) $orthoviewer(trn) ] GetThisPointer ]
	$csigadget Initialize $basewidget.[pxvtable::vnewobj ]
	if { $electrode_control !=0 } {
	    $csigadget configure -electrodetool $electrode_control
	}
    }


    if { $enable_multigadget == 1 } {
	set  multigadget [ [  pxitclmultisubjectaverage \#auto $orthoviewer(ref) $orthoviewer(trn) $overlaytool ] GetThisPointer ]
	$multigadget Initialize $basewidget.[pxvtable::vnewobj ]
		       
	if { $enable_multi2gadget == 1 } {
	    set  multi2gadget [ [  pxitclmultisubjectaverage \#auto $orthoviewer(ref) $orthoviewer(trn) $overlaytool ] GetThisPointer ]
	    $multi2gadget Initialize $basewidget.[pxvtable::vnewobj ]
	}
    }

    if { $enable_fmrigadget } {
	set w [ toplevel $basewidget.[pxvtable::vnewobj ] ]; wm withdraw $w
	set fmrigadget [  [ bis_fmri \#auto $w 0 $orthoviewer(trn) ] GetThisPointer ]
	$fmrigadget SetMultiSubjectControl $multigadget 
    }



    if { $enable_rpmtool  == 1 } {
	set rpmtool [ [ bis_guirpmcontainer \#auto  ] GetThisPointer ]
	$rpmtool InitializeFromContainer $overlaytool
	$rpmtool CreateGUI $basewidget.[pxvtable::vnewobj ]
    }


    if { $enable_manualtransformation  == 1 } {
	set manualtransformationtool [ [ pxitclmanualtransformation \#auto $orthoviewer(ref) $orthoviewer(trn)  $overlaytool ] GetThisPointer ]
	$manualtransformationtool Initialize $basewidget.[pxvtable::vnewobj ]
	update idletasks
    }

    


    if { $donotcreatemenu == 0 } {
	CreateMenu   $menubase
    }

    update idletasks
    PositionWindows
    set initialized 1

    eval "bind all <Control-d>  { $this ExitCommand }"
    wm protocol . WM_DELETE_WINDOW pxtkexit 

    return $basewidget

}


if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {

    wm withdraw .
    set breg [ pxitclbrainregister \#auto 0 ]
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





