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

package provide pxitclarticulatedmodel 1.0

# 	$Id: pxitclarticulatedmodel.tcl,v 1.2 2005/10/05 20:36:33 xenios Exp xenios $	






package require pxvtable 1.0
package require pxitclimage 1.0

itcl::class pxitclarticulatedmodel {

    inherit pxitclbasecontrol

    public variable jointlist     [ list ox oy oz rx ry rz  ]
    public variable lowerlist [ list -50  -50   -50 -60 -60 -60 ]
    public variable upperlist [ list  100  100  100  60  60  60  ]
    public variable numberofparts 2
    public variable olist  ""

    public variable updatecallback 0
    public variable tmodel 0

    # ---------------------
    protected variable jointmenu 0
    protected variable jointgui  0
    protected variable translationlabel 0
    protected variable nonlineargui 0
    private   common   thisparam        



    # ------------------------------------------------------------------

    constructor { par callback } {
	pxitclbasecontrol::constructor $par 
    } {
	set updatecallback "$par ShowSurfaces articulatedmodel"
	InitializeArticulatedModelTool
    }

    private method InitializeArticulatedModelTool { } 
    public method  Initialize { widget { isinside 0 } }

    # --------------------------------------------------------------------

    public method Load { fname } 
    public method Save { fname } 
    public method FitNonLinear { mode }
    public method Identity { part } 

    public method ResetParameters { }
    public method InitializeTransformation { } 
    public method UpdateTransformation { }
    public method UpdateAllTransformations { }

    public method UpdateGUIFromTransformation { updatedisplay }
    public method InitializeGUIFromTransformation { updatedisplay } 
    public method CreateJointGUI { widget } 

    public method GetTransformation { }  { return $tmodel }


}
# --------------------------------------------------------------------------------------------------------------------------
itcl::body pxitclarticulatedmodel::Identity { part } {

    if { $tmodel == 0 } {
	return
    }

    if { $part == "all" } {
	 $tmodel Identity

    } else {
	set index [ $jointmenu index $thisparam($this,jointindex) ]
	set tsim [ $tmodel GetTransform $index ]
	$tsim Identity
    }

    UpdateGUIFromTransformation 1 
    return
}

# ------------------------------------------------------------------------------------------------------------------------
itcl::body pxitclarticulatedmodel::FitNonLinear { mode  } {

    if { $tmodel == 0 } {
	return
    }

    if { $mode == "setup" } {
	if { $nonlineargui == 0 } {
	    set thisparam($this,bsplinesmoothness) 0.001
	    set thisparam($this,bsplinespacing) 10
	    set thisparam($this,bsplinenumpoints) 5000
	    set thisparam($this,bsplinestep) 0.05
	    set thisparam($this,bsplinenumsteps) 1
	    
	    
	    set nonlineargui [ toplevel $basewidget.[pxvtable::vnewobj ] ]
	    wm withdraw $nonlineargui
	    eval "wm protocol $nonlineargui WM_DELETE_WINDOW { wm withdraw $nonlineargui }"
	    wm  title $nonlineargui "Articulated --> BSpline Converter"
	    wm  geometry $nonlineargui 400x200
	    
	    set w $nonlineargui
	    
	    set top [ LabelFrame:create  $w.a -text "Parameters" ]
	    set bot [ frame  $w.b ]
	    pack $bot -side bottom -expand false -fill x
	    pack $w.a -side top -expand false -fill x
	    
	    set cw [ [ iwidgets::labeledwidget $top.0  -labeltext "Number of Points:"  ] childsite ]
	    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,bsplinenumpoints) ] 1000 2000 5000 10000
	    pack $cw.optmenu  -expand f
	    pack $top.0 -side top -expand f

	    set cw [ [ iwidgets::labeledwidget $top.1  -labeltext "Spacing (mm):"  ] childsite ]
	    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,bsplinespacing) ] "1" "1.5" "2" "2.5" "3" "4" "5" "6" "10" "20" "30" "40" 
	    pack $cw.optmenu  -expand f
	    pack $top.1 -side top -expand f
	    
	    set cw [ [ iwidgets::labeledwidget $top.2  -labeltext "Smoothness:"  ] childsite ]
	    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,bsplinesmoothness) ] "0.001" "0.01" "0.1" "0.5" "1.0" 
	    pack $cw.optmenu  -expand f
	    pack $top.2 -side top -expand f

	    set cw [ [ iwidgets::labeledwidget $top.3  -labeltext "Num Steps :"  ] childsite ]
	    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,bsplinenumsteps) ] 1 2 4 8 16 32
	    pack $cw.optmenu  -expand f
	    pack $top.3 -side top -expand f
	    
	    eval "button $bot.1 -text \"Fit\" -command { $this FitNonLinear compute  } "	    
	    eval "button $bot.2 -text \"Close\" -command { wm withdraw $nonlineargui  } "	    
	    pack $bot.2 -side right -padx 10
	    pack $bot.1 -side left -padx 10
	}
	wm deiconify $nonlineargui
	return
    }
    
    if { $mode != "compute" } { return }
    
    set ident [ vtkIdentityTransform [ pxvtable::vnewobj ]]
    
    set combo [ vtkpxComboTransform [ pxvtable::vnewobj ]]
    set grid [ $combo GetGridTransform ]
    $grid InitializeDisplacementGrid  [ $tmodel GetRegionLabel ] $thisparam($this,bsplinespacing) 0.05 $ident 
    $ident Delete
    
    $tmodel InitializeComboTransform  $combo $thisparam($this,bsplinenumpoints) \
	$thisparam($this,bsplinenumsteps) $thisparam($this,bsplinesmoothness)
    
    $parent SetTransformationToParentControl $combo "nonl_artic"

    set fname  [tk_getSaveFile -title "Saving B-Spline Transformation"  -filetypes { {"Combo Transform Files" {.grd}} } ]
    if { [ string length $fname  ] > 0 } {
	set ok [ $combo Save $fname ]
	if { $ok == 0 } {
	    ::pxtclutil::Warning "Failed to save combo transform in $fname\n"
	}
    }
    $combo Delete
    return
}
# ------------------------------------------------------------------------------------------------------------------------
itcl::body pxitclarticulatedmodel::Load { fname } {

    if { $fname == "" } {
	
	set typelist { 
	    {"PolySimilarity Transforms" {.psim}} 
	    {"All Files" {*}}}
	
	set f [tk_getOpenFile -title "Load Transformation" -filetypes $typelist ]
	
	if { [ string length $f ] > 0 } {
	    set fname $f
	}
    }

    if { [ string length $f ] > 0 } {
	if { $tmodel == 0 } {
	    set tmodel [ vtkpxPolySimilarityTransform [ pxvtable::vnewobj ] ]
	}
	set ok [ $tmodel Load $fname ]
	if { $ok ==1 } {
	    $this InitializeGUIFromTransformation 1
	    SetTitle "Articulated Tool [ file tail $fname ]"
	}
    }
}

itcl::body pxitclarticulatedmodel::Save { fname } {

    if { $tmodel == 0 } {
	return
    }


    if { [ string length $fname ] == 0 } {
	set fname  [tk_getSaveFile -title "Saving Transformation"  -filetypes { {"PolySimilarity Transform Files" {.psim}} } ]
    }
    if { [ string length $fname ] > 0 } {
	$tmodel Save $fname
	$this UpdateGUIFromTransformation 0
    }
}

# -------------------------------------------------------------------------------------------------------------------------
itcl::body pxitclarticulatedmodel::InitializeArticulatedModelTool {  } {

    ResetParameters

}
    

# --------------------------------------------------------------------------------------------------------------------------

itcl::body pxitclarticulatedmodel::ResetParameters { } {
    
    set thisparam($this,jointindex) "Global"

    set thisparam($this,rx) 0.0
    set thisparam($this,ry) 0.0
    set thisparam($this,rz) 0.0
    set thisparam($this,ox) 0.0
    set thisparam($this,oy) 0.0
    set thisparam($this,oz) 0.0

    set thisparam($this,p_sectorangle) 5.0
    set thisparam($this,p_blend)       2
    set thisparam($this,p_blendname)   "Both"
    set thisparam($this,p_cubic)       0
    set thisparam($this,p_smooth)      0
    set thisparam($this,p_smoothradius) 2.0
    set thisparam($this,p_thetablend)   1.0
    set thisparam($this,p_zblend)       3.0


    set thisparam($this,enableguiupdate) 1
}
# --------------------------------------------------------------------------------------------------------------------------
itcl::body pxitclarticulatedmodel::UpdateTransformation { } {

    if { $tmodel == 0 } {
	return
    }

    set index [ $jointmenu index $thisparam($this,jointindex) ]
    set tsim [ $tmodel GetTransform $index ]

    set nlist [ list "None" "Fold" "Both" ]
    set thisparam($this,p_blend) [ lsearch -exact $nlist $thisparam($this,p_blendname) ]

    $tsim SetFreezeUpdates 1    
    $tsim SetBlendedMode     $thisparam($this,p_blend)
    $tsim SetCubicBlending   $thisparam($this,p_cubic)
    $tsim SetSmoothingMode   $thisparam($this,p_smooth)      
    $tsim SetSmoothingRadius $thisparam($this,p_smoothradius)


    $tsim SetBlendingWindow $thisparam($this,p_thetablend)
    $tsim SetZBlendingSize $thisparam($this,p_zblend)
    $tsim SetMinimumSectorAngle $thisparam($this,p_sectorangle)

    
    if { $index == 0 } {
	$tsim Put 0 $thisparam($this,ox)
	$tsim Put 1 $thisparam($this,oy)
	$tsim Put 2 $thisparam($this,oz)
    } else {
	$tsim Put 3 $thisparam($this,ox)
	$tsim Put 4 $thisparam($this,oy)
	$tsim Put 5 $thisparam($this,oz)
    }
    
    $tsim Put 6 $thisparam($this,rx)
    $tsim Put 7 $thisparam($this,ry)
    $tsim Put 8 $thisparam($this,rz)
    
    $tsim SetFreezeUpdates 0
    $tsim Put 9 100.0
    $tsim Modified

    $tmodel Modified
    if { $updatecallback !=0 && $thisparam($this,enableguiupdate) ==1 } { 
	#	puts stderr "Calling $updatecallback"
	eval $updatecallback 
    } 

    UpdateGUIFromTransformation 0
}
# --------------------------------------------------------------------
itcl::body pxitclarticulatedmodel::UpdateAllTransformations { } {

    if { $tmodel == 0 } {
	return
    }

#    puts stderr "Update All Transformation Init $thisparam($this,p_blend) , $thisparam($this,p_blendname) "

    set nlist [ list "None" "Fold" "Both" ]
    set thisparam($this,p_blend) [ lsearch -exact $nlist $thisparam($this,p_blendname) ]

#    puts stderr "Update All Transformation $thisparam($this,p_blend) , $thisparam($this,p_blendname) "


    set numberofparts [ $tmodel GetNumberOfTransformations ]
    for { set i 0 } { $i < $numberofparts } { incr i } {

	set tsim [ $tmodel GetTransform $i ]
	$tsim SetFreezeUpdates 1    
	$tsim SetBlendedMode    $thisparam($this,p_blend)
	$tsim SetCubicBlending   $thisparam($this,p_cubic)
	$tsim SetSmoothingMode   $thisparam($this,p_smooth)      
	$tsim SetSmoothingRadius $thisparam($this,p_smoothradius)
	$tsim SetBlendingWindow $thisparam($this,p_thetablend)
	$tsim SetZBlendingSize $thisparam($this,p_zblend)
	$tsim SetMinimumSectorAngle $thisparam($this,p_sectorangle)
	$tsim SetFreezeUpdates 0
	$tsim Modified
    }
    $tmodel Modified
    if { $updatecallback !=0 && $thisparam($this,enableguiupdate) ==1 } { 
	eval $updatecallback 
    } 

    UpdateGUIFromTransformation 0
}
# --------------------------------------------------------------------
itcl::body pxitclarticulatedmodel::InitializeGUIFromTransformation {  updatedisplay } {

    if { $tmodel == 0 } {
	return
    }

    set numberofparts [ $tmodel GetNumberOfTransformations ]
    set abdomenpieces [ expr $numberofparts - 7 ]
    EnableUI $jointgui
    
    set numoptions [ expr [ $jointmenu index last ] +1 ]

    for { set i [ expr $numoptions -1 ] } { $i >= 1 } { set i [ expr $i -1 ] } {
	$jointmenu delete $i
    }

    for { set i 1 } { $i < $numberofparts } { incr i } {
	
	set name "Joint_${i}"
	set name2 [ [ $tmodel GetTransform $i ] GetJointName ]
	if { $name2 != "Joint" } {
	    set name $name2
	}
#	puts stdout "Adding $name $i"
	$jointmenu add radio -label $name -variable [ itcl::scope thisparam($this,jointindex) ] 
	eval "$jointmenu entryconfigure $i -command { $this UpdateGUIFromTransformation 0 }"
    }

    set thisparam($this,currentjoint) "Global"
    $this UpdateGUIFromTransformation $updatedisplay
}


itcl::body pxitclarticulatedmodel::UpdateGUIFromTransformation { updatedisplay  } {

    if { $tmodel == 0 } {
	return
    }

 
    set index [ $jointmenu index $thisparam($this,jointindex) ]

#    puts stderr "Current Transformation Index = $index"

    set tsim [ $tmodel GetTransform $index ]
#    puts stderr "[ $tsim GetClassName ] "

    set thisparam($this,p_blend) [ $tsim GetBlendedMode ]
    set thisparam($this,p_theablend) [ $tsim GetBlendingWindow ]
    set thisparam($this,p_zblend) [ $tsim GetZBlendingSize ]
    set thisparam($this,p_sectorangle) [ $tsim GetMinimumSectorAngle ]
    set thisparam($this,p_cubic) [ $tsim GetCubicBlending ]
    set thisparam($this,p_smooth)       [ $tsim GetSmoothingMode ]
    set thisparam($this,p_smoothradius) [ $tsim GetSmoothingRadius ]



    set nlist [ list "None" "Fold" "Both" ]
    set thisparam($this,p_blendname) [ lindex $nlist $thisparam($this,p_blend) ]


    if { $index == 0 } {
	set thisparam($this,ox) [ $tsim Get 0 ]
	set thisparam($this,oy) [ $tsim Get 1 ]
	set thisparam($this,oz) [ $tsim Get 2 ]
	$translationlabel configure -text "Global Translation:"
	


    } else {
	set thisparam($this,ox) [ $tsim Get 3 ]
	set thisparam($this,oy) [ $tsim Get 4 ]
	set thisparam($this,oz) [ $tsim Get 5 ]
	$translationlabel configure -text "Joint Position:"
    }
    
    set thisparam($this,rx) [ $tsim Get 6 ]
    set thisparam($this,ry) [ $tsim Get 7 ]
    set thisparam($this,rz) [ $tsim Get 8 ]
    
    if { $updatedisplay == 1 } {
	if { $updatecallback !=0 } { eval $updatecallback } 
    }

}
# --------------------------------------------------------------------

itcl::body pxitclarticulatedmodel::CreateJointGUI { w  } {

    set b [ LabelFrame:create  $w.a -text "Joint Parameters" ]
    set g [ LabelFrame:create  $w.b -text "Global Parameters" ]
    pack $w.a $w.b -side top -expand false -fill x

    set top [ frame $b.1 ] 
    set md1 [ frame $b.2 ] 
    set md2 [ frame $b.3 ] 
    set md3 [ frame $g.4 ] 
    set bot [ frame $g.5 ] 
    
    pack $bot $md3 -side bottom -expand true -fill x
    pack $top $md1 $md2  -side top -expand true -fill x


    set cw [ [ iwidgets::labeledwidget $top.1  -labeltext "Joint:"  ] childsite ]
    set jointmenu [ tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,jointindex) ] "Global" ]
    pack $cw.optmenu  -expand f
    pack $top.1 -side left -expand f
    
    eval "$cw.optmenu.menu entryconfigure 0 -command { $this UpdateGUIFromTransformation 0 }"

    checkbutton $top.2 -variable [ itcl::scope thisparam($this,enableguiupdate) ] -text "Update Display" 
    pack $top.2 -side right -expand f
    

    set translationlabel [ label $md1.1 -text "Global Translation:" ]
    pack  $md1.1 -side left 
    
    # Origin 
    for { set i 2 } { $i <=4 } { incr i } {

	set index [ expr $i -2 ]
	set vname [ lindex $jointlist $index ]
	set lower [ lindex $lowerlist $index ]
	set upper [ lindex $upperlist $index ]
	scale $md1.$i -variable  [ itcl::scope thisparam($this,$vname) ]  -from $lower -to $upper -orient horizontal  -digits 5 -resolution 0.1
	eval "bind $md1.$i <ButtonRelease> \{ $this UpdateTransformation \}"
	pack $md1.$i -side left  -expand f -fill x
    }

    label $md2.5 -text "Rotations:"
    pack $md2.5 -side left 

    for { set i 6 } { $i <=8 } { incr i } {

	set index [ expr $i -3 ]
	set vname [ lindex $jointlist $index ]
	set lower [ lindex $lowerlist $index ]
	set upper [ lindex $upperlist $index ]
	scale $md2.$i -variable [ itcl::scope  thisparam($this,$vname) ] -from $lower -to $upper -orient horizontal  -digits 5 -resolution 0.01 
	eval "bind $md2.$i <ButtonRelease> \{ $this UpdateTransformation \}"
	pack $md2.$i -side left  -expand f -fill x
    }
    eval "button $md2.10 -text \"Identity\" -command { $this Identity current  } "	    
    pack $md2.10 -side right -padx 2


    iwidgets::entryfield $md3.16 -labeltext "Blend Theta:"  -width 2 -validate real -textvariable [ itcl::scope thisparam($this,p_thetablend) ] -relief sunken 
    iwidgets::entryfield $md3.17 -labeltext "Blend Z:"  -width 2 -validate real -textvariable [ itcl::scope thisparam($this,p_zblend) ] -relief sunken 
    
    iwidgets::entryfield $md3.18 -labeltext "Sector Angle:"  -width 2 -validate real -textvariable [ itcl::scope thisparam($this,p_sectorangle) ] -relief sunken 
											
    eval "button $md3.19 -text \"Upd\" -command { $this UpdateTransformation } "	    

    pack $md3.19 -side right -padx 2
    pack  $md3.16 $md3.17 $md3.18  -side left -padx 1 -expand t -fill x

    
    set cw [ [ iwidgets::labeledwidget $bot.12  -labeltext "Mode:"  ] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,p_blendname) ] "None" "Fold" "Both"
    pack $cw.optmenu  -expand f
    
    checkbutton $bot.13 -variable [ itcl::scope  thisparam($this,p_cubic) ] -text Cubic 
    checkbutton $bot.14 -variable [ itcl::scope  thisparam($this,p_smooth) ] -text Smooth 
    iwidgets::entryfield $bot.15 -labeltext "Radius:"  -width 2 -validate real -textvariable [ itcl::scope thisparam($this,p_smoothradius) ] -relief sunken 
    eval "button $bot.16 -text \"Upd All\" -command { $this UpdateAllTransformations } "

    pack $bot.12 $bot.13 $bot.14 $bot.15 $bot.16  -side left -padx 1 -expand t -fill x

}

itcl::body pxitclarticulatedmodel::Initialize { widget { isinside 0 } } {

    if { $isinside == 0 } {
        set basewidget [ toplevel $widget ]
	wm geometry $basewidget 460x270
	wm withdraw $basewidget
	SetTitle "Articulated Tool"
	eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    } else {
	set basewidget [ frame $widget ]
    }

    set bbar [ frame $basewidget.bot ]; pack $bbar -side bottom -expand f -fill x    
    
    set jointgui [ frame $basewidget.top -height 200 ]; pack $widget.top -side top -expand t -fill both 
    CreateJointGUI $jointgui
    DisableUI $jointgui

    
    eval "button $bbar.load  -text \"Load\"     -command { $this Load \"\" } "
    eval "button $bbar.save  -text \"Save\"     -command { $this Save \"\" } "
    eval "button $bbar.ident -text \"Identity\" -command { $this Identity all  } "
    eval "button $bbar.fit   -text \"FitNonLinear \" -command { $this FitNonLinear setup  } "


    eval "button $bbar.close -text \"Close\"    -command { $this HideWindow  } "
    pack $bbar.close -side right 
    pack $bbar.load $bbar.save $bbar.ident $bbar.fit -side left -expand f -fill x -padx 1

}
# --------------------------------------------------------------------------------------------------------------------------

