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





package provide pxitcldistributiontablegadget 1.0

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
package require  pxitclbaseimagecontrol 1.0
package require  pxitclbaseimageviewer  1.0
# ----------------------------------------------------------------------------------------------

itcl::class pxitcldistributiontablegadget {

    inherit pxitclbasecontrol
    
    # Generic Stuff 

    private   common   thisparam        
    protected variable ttable_dofentry 0
    protected variable ttable_buttonlist ""
    protected variable viewernames ""
    protected variable viewers
    # public 

    
    #
    constructor { par args } {
	pxitclbasecontrol::constructor $par
    } {
	InitializeDistributionTableGadget
    }

    public method RegisterViewer { vtk_viewer name }
    public method Initialize { widget } 
    public method ComputeTValue { mode }
    public method GrabTvalueFromViewer { viewerno }
    public method SwitchZScore { } 
    public method AddToMenuButton { mb args }

    protected method InitializeDistributionTableGadget { }
}



::itcl::body pxitcldistributiontablegadget::AddToMenuButton { mb args} {

    eval "$mb add command -label \"T-Distribution Table Tool\" -command {$this ShowWindow }"
}

::itcl::body pxitcldistributiontablegadget::RegisterViewer { vtk_viewer name} {

    lappend viewernames   $name
    set viewers($name) $vtk_viewer
}

itcl::body pxitcldistributiontablegadget::InitializeDistributionTableGadget { } { 

    set ttable_dofentry 0
    set thisparam($this,ttable_mathobject)  0
    set thisparam($this,ttable_tvalue)   2.0
    set thisparam($this,ttable_df)         21
    set thisparam($this,ttable_x1000)        0
    set thisparam($this,ttable_zscore)       0
    set thisparam($this,ttable_pv1)      0.10
    set thisparam($this,ttable_pv2)      0.20
}

itcl::body pxitcldistributiontablegadget::Initialize { widget } { 
	
    set basewidget [ toplevel $widget ]
    set ttable $basewidget

    wm geometry $ttable 400x100
    wm title $ttable "Critical Points of the T-distribution"
    eval "wm protocol $ttable WM_DELETE_WINDOW { wm withdraw $ttable }"
    
    frame $ttable.top
    frame $ttable.top2
    frame $ttable.bot
    pack $ttable.top $ttable.top2 $ttable.bot -side top -expand false -fill x -pady 2
    
    lappend ttable_buttonlist [ iwidgets::entryfield $ttable.top.1 -labeltext "T-value:"  -width 6 -validate real -textvariable [ itcl::scope thisparam($this,ttable_tvalue) ] -relief sunken  ]
    checkbutton   $ttable.top.15  -text "x1000"  -variable [ itcl::scope thisparam($this,ttable_x1000) ]
    eval "checkbutton   $ttable.top.16  -text \"Z-score\"  -variable [ itcl::scope thisparam($this,ttable_zscore) ] -command { $this SwitchZScore }"
    set ttable_dofentry [ iwidgets::entryfield $ttable.top.2 -labeltext "DOF"  -width 3 -validate integer -textvariable [ itcl::scope thisparam($this,ttable_df) ] -relief sunken  ]
    iwidgets::entryfield $ttable.top2.3 -labeltext "One Tail:"  -width 6 -validate real -textvariable [ itcl::scope thisparam($this,ttable_pv1) ] -relief raised 
    iwidgets::entryfield $ttable.top2.4 -labeltext "Two Tail:"  -width 6 -validate real -textvariable [ itcl::scope thisparam($this,ttable_pv2) ] -relief sunken
    
    pack $ttable.top.1 $ttable.top.15 $ttable.top.16 $ttable.top.2  -side left -expand true -fill x
    pack $ttable.top2.3 $ttable.top2.4  -side left -expand true -fill x
    

    eval "button $ttable.bot.4 -text \"Close\"   -command { wm withdraw $ttable }"
    pack $ttable.bot.4 -side right -padx 3
	
    lappend ttable_buttonlist [ eval "button $ttable.bot.1 -text  \"T-> P\" -command { $this ComputeTValue forward }" ]
    lappend ttable_buttonlist [ eval "button $ttable.bot.15 -text \"P(2t) -> T\" -command { $this ComputeTValue  reverse }" ]
    pack $ttable.bot.1 $ttable.bot.15 -side left -padx 1

    for { set i 0 } { $i < [ llength $viewernames  ] } { incr i } {
	set name [ lindex $viewernames $i ]
	set w "2_$i"
	eval "button $ttable.bot.$w -text \"Grab $name\" -command { $this GrabTvalueFromViewer $name }"
	pack $ttable.bot.$w  -side left -padx 1
    }
    
    set thisparam($this,ttable_mathobject) [ vtkpxMath [ pxvtable::vnewobj ]]
    set fullyinitialized 1        
    return $basewidget
}

itcl::body pxitcldistributiontablegadget::SwitchZScore { } {

    if { $thisparam($this,ttable_zscore) == 1 } {
	$ttable_dofentry configure -state disabled
	[lindex $ttable_buttonlist 0 ] configure -labeltext "Z-Value"
	[lindex $ttable_buttonlist 1 ] configure -text "Z->P"
	[lindex $ttable_buttonlist 2 ] configure -text "P(2t)->Z"
    } else {
	$ttable_dofentry configure -state normal
	[lindex $ttable_buttonlist 0 ] configure -labeltext "T-Value"
	[lindex $ttable_buttonlist 1 ] configure -text "T->P"
	[lindex $ttable_buttonlist 2 ] configure -text "P(2t)->T"
    }
}

itcl::body pxitcldistributiontablegadget::GrabTvalueFromViewer { viewername } {

    set thisviewer [  $viewers($viewername) GetViewer ]

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

itcl::body pxitcldistributiontablegadget::ComputeTValue { mode } {

    if { $mode == "forward" } {
	if { $thisparam($this,ttable_x1000) == 1 } {
	    set x [ expr 0.001 * $thisparam($this,ttable_tvalue) ]
	} else {
	    set x  $thisparam($this,ttable_tvalue) 
	}
	
	if { $thisparam($this,ttable_zscore) == 0 } {
	    set thisparam($this,ttable_pv2) [ $thisparam($this,ttable_mathobject) TtestToPvalue_itk $x $thisparam($this,ttable_df) ]
	} else {
	    set thisparam($this,ttable_pv2) [ $thisparam($this,ttable_mathobject) ZscoreToPvalue $x ]
	}
	set thisparam($this,ttable_pv1) [ expr $thisparam($this,ttable_pv2) * 0.5 ]
	#set thisparam($this,ttable_pv1) [ $thisparam($this,ttable_mathobject) TtestToPvalue_itk $x $thisparam($this,ttable_df) ]
    } else {
	set x $thisparam($this,ttable_pv2)
	set thisparam($this,ttable_pv1) [ expr $thisparam($this,ttable_pv2) * 0.5 ]
	if { $thisparam($this,ttable_zscore) == 0 } {
	    set t [ $thisparam($this,ttable_mathobject) PvalueToTvalue $x $thisparam($this,ttable_df) ]
	} else {
	    set t [ $thisparam($this,ttable_mathobject) PvalueToZscore $x  ]
	}
	if { $thisparam($this,ttable_x1000) == 1 } {
	    set t [ expr $t * 1000.0 ]
	}
	set thisparam($this,ttable_tvalue) $t
    }
   

    update idletasks
}


