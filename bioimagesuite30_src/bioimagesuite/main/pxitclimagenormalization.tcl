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

package provide pxitclimagenormalization 1.0

# 	$Id: pxitclimagenormalization.tcl,v 1.4 2003/07/28 18:59:09 xenios Exp xenios $	





package require pxitclbaseimagecontrol 1.0
package require pxitclHistogramWidget 1.0

itcl::class pxitclimagenormalization {

    inherit pxitclbaseimagecontrol
    
    variable updatehistogram 0
    variable histogramwidget 0
    variable histogram 0 

    private common histinput1 
    private common histinput2 
    private common histinput3 
    private common histinput4 
    private common histoutput1
    private common histoutput2
    private common histoutput3
    private common histoutput4

    protected variable entrylist ""
    protected variable entrymode 0

    constructor { par } { 
	pxitclbaseimagecontrol::constructor $par 
    } {

	set histinput1($this)   -1; 	set histinput2($this)  100
	set histinput3($this)  200;	set histinput4($this)   -1
	set histoutput1($this) 115;	set histoutput2($this) 200
	set histoutput3($this) 300;	set histoutput4($this)  -1
    }
    
    # ---------------------------------------------------------------------------------
    #   Create Actual GUI
    # ---------------------------------------------------------------------------------

    public method Initialize { inpwidg } 
    public method UpdateHistogram { }
    public method NormalizeIntensity { }
    public method HistogramEqualization { }
    public method ResetHistogram { }
    public method UpdateModes { mode }
    public method ComputeAuto { }

    public method SetImage { img } 
}


::itcl::body pxitclimagenormalization::SetImage { img  } {
    set ok [ pxitclbaseimagecontrol::SetImage $img ]
    if { $ok == 0 } { return 0 }
    set updatehistogram 1
    UpdateHistogram
    ResetHistogram
}

# ---------------------------------------------------------------------------------
#
# Create Histogram
#
# ---------------------------------------------------------------------------------
itcl::body pxitclimagenormalization::Initialize { widg } {

    if { $initialized == 1 } {
	return $basewidget
    }

    set basewidget $widg

    set hist $basewidget ; 	toplevel $hist ; wm withdraw $hist; 
    wm geometry $hist 700x270 ; wm resizable $hist false false
    wm title $hist "Histogram Control"
    wm resizable $hist 0 0

    set bottomf [ frame  $hist.b  -bd 1 -bg white] ; 	
    set topf    [ frame  $hist.a ]
    set middlef [ frame  $hist.c -bd 1  ]
    
    pack $topf $middlef $bottomf -side top -fill x -expand t -pady 0
    
    label $middlef.left -text "Input:" -padx 3;  	label $middlef.right -text "Output: " -padx 3
    grid $middlef.left -row 0 -column 0; 	grid $middlef.right -column 3 -row 0
    
    for { set k 1 } { $k <=2 } { incr k } {
	if { $k==1 } {
	    set vaa "histinput"
	    set direction "left"
	    set offset 0
	} else {
	    set vaa "histoutput"
	    set direction "right"
	    set offset 5
	}
	
	for {set i 0 }  {$i <= 3 } { incr i } {
	    set j [expr $i +1 ]

	    if { $i == 0 } {
		if { $k == 1 } {
		    set labname "In:"
		} else {
		    set labname "Out:"
		}
	    } else {
		set labname ""
	    }
	    
	    set vbb "${vaa}${j}"
	    set vb [ itcl::scope ${vbb}($this) ]
	    eval "iwidgets::entryfield $middlef.${direction}$i -labeltext \"$labname\" -textvariable  $vb -width 5 -validate real"
	    grid $middlef.${direction}$i -column  [expr $i + $offset ]    -row 0

	    if { $i==0 || $i ==3 } {
		lappend entrylist $middlef.${direction}$i
	    }
	}
	
    }
    
    eval "button $middlef.bu1 -text M1 -pady 1 -padx 3 -command { $this UpdateModes  1  }"
    eval "button $middlef.bu2 -text M2 -pady 1 -padx 1 -command { $this UpdateModes  2  }"
    eval "button $middlef.bu3 -text M3 -pady 1 -padx 1 -command { $this UpdateModes  3  }"
    eval "button $middlef.bu4 -text All -pady 1 -padx 1 -command { $this UpdateModes -2  }"

    grid $middlef.bu1 -column 16 -row 0
    grid $middlef.bu2 -column 17 -row 0
    grid $middlef.bu3 -column 18 -row 0
    grid $middlef.bu4 -column 19 -row 0
    
    eval "button $bottomf.btn3  -text \"Normalize\" -command { $this NormalizeIntensity }"
    eval "button $bottomf.btn3b  -text \"Histogram Eq\" -command { $this HistogramEqualization }"
    pack $bottomf.btn3 $bottomf.btn3b -fill x -expand f -side left -padx 2
    if { $parent != 0 } {
	eval "button $bottomf.l1  -text \"Copy Results To Image\" -command { $parent CopyResultsToImage }"
	eval "button $bottomf.l2  -text \"Undo Copy \" -command { $parent RevertToPreviousImage }"
	pack $bottomf.l1 $bottomf.l2 -side left -padx 2
    }

    eval "button $bottomf.btn2  -text  Close -command { $this HideWindow }"
    

    pack $bottomf.btn2 -fill x -expand f -side right
    
    set histogram [ ::pxitclHistogramWidget \#auto $this ]
    set histogramwidget [ $histogram Initialize  $topf.r2]
    $histogram SetImage  $currentimage 
    pack $histogramwidget -side left -padx 3 -pady 3 -fill both -expand t	    
    $histogram Bind  1
    set updatehistogram 1
    set initialized 1
   
    eval "$histogram configure -callback \"$this UpdateModes -1\" "

    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"

    DisableUI $entrylist

    return $basewidget
}

itcl::body pxitclimagenormalization::UpdateHistogram {  } {
	
    if {$updatehistogram == 1 }   {
	$histogram SetImage $currentimage 
	set updatehistogram 0
    }
}

# ---------------------------------------------------------------------------------
#   Normalize Intensity
# ---------------------------------------------------------------------------------

itcl::body pxitclimagenormalization::NormalizeIntensity { } {
	
    set dim2 [ $currentimage GetImageSize ]
    if { $dim2  < 2 } { 
	::pxtclutil::Warning "No Image in Memory\n Cannot Normalize Intensity!"
	return 
    }
	
    WatchOn 
	
    set norm [  vtkpxImageNormalizeIntensity [ pxvtable::vnewobj ] ]

    $norm SetInput [ $currentimage GetImage ]
    $norm SetInputLevel1   $histinput1($this) ;    $norm SetInputLevel2   $histinput2($this) 
    $norm SetInputLevel3   $histinput3($this) ;    $norm SetInputLevel4   $histinput4($this) 
    $norm SetOutputLevel1  $histoutput1($this);    $norm SetOutputLevel2  $histoutput2($this)
    $norm SetOutputLevel3  $histoutput3($this);    $norm SetOutputLevel4  $histoutput4($this)
    $norm SetOutputLevel0  0

    $this ProgressCallback $norm "Normalizing Intensity"
    SetFilterCallbacks $norm "Normalizing Intensity"
    $norm Update
	
    $currentresults ShallowCopyImage  [$norm GetOutput]
    $currentresults CopyImageHeader [ $currentimage GetImageHeader ]
    $norm Delete 
    $currentresults configure -filename [ AddPrefix [ $currentimage cget -filename ] Norm ]
    SendResultsToParent
    $currentresults UnLoad
    WatchOff
}
    
itcl::body pxitclimagenormalization::ResetHistogram  { } {

    if { $histogramwidget != 0 } {
	$histogram configure -LOWER_LEVEL -1
	$histogram configure -UPPER_LEVEL -1
	$histogram configure -LOWER_BOUND -1
	$histogram configure -UPPER_BOUND -1
	$histogram Render
	set updatehistogram 0
    }
}

    
itcl::body pxitclimagenormalization::UpdateModes { val } {
    
    set lowv  [ $histogram cget -LOWER_LEVEL ]
    set highv [ $histogram cget -UPPER_LEVEL ]

    
    set histinput2($this) $lowv
    set histinput3($this) $highv


    if { $val == - 1 } {
	return
    }
    
    if { $val == -2 } {
	EnableUI $entrylist
	return
    }

	
    switch -exact $val {
	1 {
	    set histoutput1($this) 115
	    set histoutput2($this) 200
	    set histoutput3($this) 275
	    set histoutput4($this) 350
	}
	2 {
	    set histoutput1($this) 115
	    set histoutput2($this) 200
	    set histoutput3($this) 285
	    set histoutput4($this) 365
	}
	3 {
	    set histoutput1($this) 115
	    set histoutput2($this) 200
	    set histoutput3($this) 300
	    set histoutput4($this) 400
	}
    }
	
    if { $val <=3  } {
	
	set diffi [expr $highv - $lowv ]
	set diffo [expr $histoutput3($this)-$histoutput2($this) ]
	set a [ expr ( $histoutput2($this) - $histoutput1($this) ) / $diffo ]
	set histoutput4($this) [ expr $histoutput3($this) + $diffo ]
	set histinput1($this)  [ expr $lowv-$a*$diffi ]
	set histinput4($this)  [ expr $highv+$diffi ]
	DisableUI $entrylist
    }
}


itcl::body pxitclimagenormalization::ComputeAuto { } {

    return
}

# ---------------------------------------------------------------------------------
#   Histogram Equalization
# ---------------------------------------------------------------------------------

itcl::body pxitclimagenormalization::HistogramEqualization { } {
	
    set dim2 [ $currentimage GetImageSize ]
    if { $dim2  < 2 } { 
	::pxtclutil::Warning "No Image in Memory\n Cannot Perform Histogram Equalization!"
	return 
    }
	
    WatchOn 
	
    set histeq [  vtkpxImageHistogramEqualization [ pxvtable::vnewobj ] ]
    $histeq SetInput [ $currentimage GetImage ]
    $histeq SetLowerThreshold   $histinput2($this) 
    $histeq SetUpperThreshold   $histinput3($this)
    $histeq SetNumberOfBins     256
    $this ProgressCallback $histeq "Histogram Equalization"
    SetFilterCallbacks $histeq "Histogram Equalization"
    $histeq Update
	
    $currentresults ShallowCopyImage  [$histeq GetOutput]
    $currentresults CopyImageHeader [ $currentimage GetImageHeader ]
    $histeq Delete 
    $currentresults configure -filename [ AddPrefix [ $currentimage cget -filename ] Histeq ]
    SendResultsToParent
    $currentresults UnLoad
    WatchOff
}

