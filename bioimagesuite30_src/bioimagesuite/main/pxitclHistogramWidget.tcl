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

package provide pxitclHistogramWidget 1.0

# 	$Id: pxitclHistogramWidget.tcl,v 1.3 2004/12/09 15:50:45 xenios Exp xenios $	





package require vtkImageInclude 1.0
package require pxvtable 1.0

itcl::class pxitclHistogramWidget {

    inherit pxitclbaseimagecontrol

    public variable NUM_BINS 64
    public variable LOWER_BOUND -10000000
    public variable UPPER_BOUND -10000000
    public variable LOWER_LEVEL -10000000
    public variable UPPER_LEVEL -10000000
    public variable MAX_BINS     128
    private variable DRAGGING 0

    public variable accumulate 0
    private variable drawcanvas 0
    private variable hasinput 0
    public  variable callback 0
    public  variable entropy 0.0
    
    private common   showmax
    private common   showaccurate
    private common   lowerlevel 
    private common   upperlevel 
    private common   maxbins

        # create a histogram object
    constructor { par } {
	pxitclbaseimagecontrol::constructor $par
    } {
	set accumulate [  vtkpxImageRobustHistogram [ pxvtable::vnewobj ]]
    }

    public method Initialize { inpwidg }
    public method UpdateRange { } 
    public method Render { } 
    public method Reset  { }
    public method SetImage { img } 
    public method Bind { probe }
    public method Save { }

    public method StartInteraction { x y b} 
    public method EndInteraction { } 
    public method UpdateInteraction { x y butno} 

    public method ZeroMaximum { data } { } 

}
# -------------------------------------------------------------------------------------------
::itcl::body pxitclHistogramWidget::Initialize { inpwidg } {

    if { $initialized } { return $basewidget } 

    set basewidget $inpwidg
    set initialized 1
    set showmax($this) 0
    set showaccurate($this) 0

    frame $basewidget -bg red -bd 2 -relief ridge
    set topf [ frame $basewidget.top ]
    set bottomf [ frame $basewidget.bottom ]
    label $bottomf.label1 -bg darkgray -fg white
    set drawcanvas [ canvas $basewidget.canvas -width 650 -height 150  ]

    pack $topf -side bottom -fill x
    pack $bottomf -side top -fill x
    pack $drawcanvas -side top -fill both -expand true

    set lowerlevel($this) $LOWER_BOUND 
    set upperlevel($this) $UPPER_BOUND 
    set maxbins($this) $MAX_BINS 

    label $topf.label1 -bg black -fg white

    iwidgets::entryfield $topf.ent1 -labeltext "Range:" -width 5 -textvariable [ itcl::scope lowerlevel($this) ] -validate real
    iwidgets::entryfield $topf.ent2 -labeltext ""  -width 5 -textvariable [ itcl::scope upperlevel($this) ] -validate real
    iwidgets::entryfield $topf.ent25 -labeltext "Bins:"  -width 3 -textvariable [ itcl::scope maxbins($this) ] -validate real

    eval  "bind  [ $topf.ent1 component entry ]  <Return> { $this UpdateRange } "
    eval  "bind  [ $topf.ent2 component entry ]  <Return> { $this UpdateRange } "
    eval  "bind  [ $topf.ent25 component entry ]  <Return> { $this UpdateRange } "

    eval "button $topf.but2 -text Upd -command { $this UpdateRange; $this Render } -pady 1"
    eval "button $topf.but3 -text Reset -command { $this Reset }  -pady 1 "
    pack $topf.label1 $topf.ent1 $topf.ent2 $topf.ent25 $topf.but2 $topf.but3 -side left -fill x -pady 0
    
	
    set str1 [format "Levels: \[%.1f, %.1f\] Bins=%d" $LOWER_LEVEL $UPPER_LEVEL $NUM_BINS]
    $topf.label1 configure -text  $str1

    eval "checkbutton $bottomf.but31 -variable [ itcl::scope showaccurate($this) ] -text \"HighRes\" -command { $this SetImage -1 } "
    eval "checkbutton $bottomf.but3 -variable [ itcl::scope showmax($this) ] -text \"ShowMax\" -command { $this UpdateRange } "


    eval "button $bottomf.but5 -text Save  -command { $this Save }  -pady 1 "

    set l [ AddLogoLabel $bottomf ]

    pack $l $bottomf.but5  $bottomf.but3 $bottomf.but31 -side right -expand f -padx 0 -pady 0
    pack $bottomf.label1  -side left -expand true -padx 1 -pady 0 -fill x
    $topf.label1 configure -text "(Left: Drag, Middle: Gray, Right: White)"

    set cwidth  [ $drawcanvas cget -width  ]
    set cheight [ $drawcanvas cget -height ]

    $drawcanvas create rect 0 0 $cwidth $cheight -fill blue -tag background
    $drawcanvas create rect 0 [expr $cheight/2] $cwidth $cheight -fill white -tag histogram
    $drawcanvas create rect [expr $cwidth/3] 0 [expr $cwidth/3 +1 ] $cheight -fill gray -outline gray -tag grayslider
    $drawcanvas create rect [expr $cwidth/2] 0 [expr $cwidth/2 +1 ] $cheight -fill white -outline white -tag whiteslider
   

    return $basewidget
}
# -------------------------------------------------------------------------------------------    
    
# Sets the input
::itcl::body pxitclHistogramWidget::SetImage { image } {


    if { $image != -1 } {
	set ok [ pxitclbaseimagecontrol::SetImage $image ]
	if { $ok == 0 } { 	
	    catch { $drawcanvas delete all } 
	    set hasinput 0
	    return 0 
	}
    }
    set input [ $currentimage GetImage ]
    set hasinput 1

    set sp   [ $input GetSpacing ]
    set extr [ vtkImageExtractComponents [ pxvtable::vnewobj ]]
    $extr SetInput $input
    $extr SetComponents 0
    $extr Update

    if { $showaccurate($this) == 0 } {
	set resl [ vtkImageResample [ pxvtable::vnewobj ]]
	$resl InterpolateOff
	$resl SetAxisOutputSpacing 0 [ expr [ lindex $sp 0 ] *4 ]
	$resl SetAxisOutputSpacing 1 [ expr [ lindex $sp 1 ] *4 ]
	$resl SetAxisOutputSpacing 2 [ expr [ lindex $sp 2 ] *4 ]
	$resl SetInput [ $extr GetOutput ]
	$resl Update 
	$accumulate SetInput [ $resl GetOutput ]	
	$resl Delete
    } else {
	$accumulate SetInput [ $extr GetOutput ]
    }
    Reset
    
    #set data [ [ $input  GetPointData ] GetScalars ]
    #set inputRange0 [lindex [$data GetRange] 0]
    #set inputRange1 [lindex [$data GetRange] 1]

    #set LOWER_BOUND $inputRange0
    #set UPPER_BOUND $inputRange1
#    $resl Delete
    $extr Delete
}
    
# -------------------------------------------------------------------------------------------
::itcl::body pxitclHistogramWidget::UpdateRange {  } {

    set LOWER_BOUND $lowerlevel($this)
    set UPPER_BOUND $upperlevel($this)
    set MAX_BINS    $maxbins($this)
    Render

}

::itcl::body pxitclHistogramWidget::Reset {  } {

    set NUM_BINS 64
    set MAX_BINS     128
    set DRAGGING 0

    set lowerlevel($this) -1
    set upperlevel($this) -1
    set maxbins($this) $MAX_BINS 

    $this UpdateRange
}
# -------------------------------------------------------------------------------------------
# Render
::itcl::body pxitclHistogramWidget::Render { } {

    if { [ $currentimage GetImageSize  ] < 2 } { 
	catch { $drawcanvas delete all }
	return 0 
    } 

    # get the size of the histogram window
    set width [lindex  [$basewidget configure -width] 4]
    set height [lindex [$basewidget configure -height] 4]
    
    set data [ [ [ $accumulate GetInput]  GetPointData ] GetScalars ]
    set inputRange0 [lindex [$data GetRange] 0]
    set inputRange1 [lindex [$data GetRange] 1]
    set inputRangeOrig0 [lindex [$data GetRange] 0]
    set inputRangeOrig1 [lindex [$data GetRange] 1]
    
    set lower_bound $LOWER_BOUND  
    set upper_bound $UPPER_BOUND  
    set max_bins    $MAX_BINS

    if { $lower_bound == - 1 && $upper_bound == - 1 } {
	set upper_bound $inputRange1
	set lower_bound $inputRange0
    } else {
	if { $lower_bound >= $inputRange0  && $lower_bound < $inputRange1  } {
	    set inputRange0 $lower_bound
	} else {
	    set lower_bound $inputRange0
	}
    
	if { $upper_bound >= $inputRange0  && $upper_bound < $inputRange1  } {
	    set inputRange1 $upper_bound
	} else {
	    set upper_bound $inputRange1
	}
    }

    if { $upper_bound == $lower_bound } {
	set upper_bound [ expr $lower_bound +1 ]
    }

    
    set numBins [ expr  $upper_bound - $lower_bound + 1 ] 

    if {  [ [ $accumulate GetInput] GetScalarType ] == 10 } { 
	set numBins $max_bins
    } else {
	if { $numBins == 0 } { 
	    set numBins 1 
	} else {
	    # Force numbins in range 2-128
	    if { $numBins > $max_bins } { 
		set numBins $max_bins
	    } elseif { $numBins <  2 } { 
		set numBins 2
	    }
	}
    }

    set numBins [ expr int($numBins) ]
    set NUM_BINS $numBins
    set maxbins($this) $NUM_BINS


    set lowerlevel($this) $lower_bound 
    set upperlevel($this) $upper_bound 


    # Display Stuff ; create Histogram
    # --------------------------------
    #    set origin  $lower_bound
    #   set spacing [expr 1.0 * ($upper_bound - $origin) / $numBins]


    
    $accumulate SetNumberOfBins $numBins
    $accumulate SetLowerThreshold $lower_bound
    $accumulate SetUpperThreshold $upper_bound
    $accumulate InterpolationOn
    $accumulate RobustModeOff
    $accumulate SaturationModeOff
    $accumulate Update
    set entropy [ $accumulate ComputeEntropy ]
    

    # get the histogram data
    
    set data [ vtkImageData [ pxvtable::vnewobj ]]
    $data DeepCopy [$accumulate GetOutput]



    if { $showmax($this) == 0 && $lower_bound == $inputRangeOrig0 && $upper_bound == $inputRangeOrig1 } {
	ZeroMaximum $data
    }
    
    

    # initialize the canvas
    # Get the height and the width
    set cwidth  [ $drawcanvas cget -width  ]
    set cheight [ $drawcanvas cget -height ]
    
    $drawcanvas delete background histogram grayslider whiteslider
    $drawcanvas create rect 0 0 $cwidth $cheight -fill blue -tag background
    
    # scale the histogram max to fit the window
    set histRange [[[$data GetPointData] GetScalars] GetRange]
    #    puts stderr "histRange = $histRange"
    
    set histRange1 [ lindex $histRange 1 ]
    if  { $histRange1 < 1 } { set histRange1 1 }
    set scale  [expr 0.75 * double($cheight) / $histRange1]
    set wscale [ expr $cwidth / $numBins ]
    



    # Draw Histogram as altenating color bars
    for {set idx 0} {$idx < $numBins} {incr idx} {
	
	set y [$data GetScalarComponentAsDouble $idx 0 0 0]
	set y1 [expr $cheight - $y * $scale]
	set y2 [lindex [split $y1 .] 0]
	
	set x1 [ expr $idx * $wscale ]
	set x2 [ expr $x1 +  $wscale ]

	$drawcanvas create rect $x1 $cheight $x2 $y2 -fill yellow -outline red -tag histogram
    }
    
    $data Delete
    
    set origin  [lindex [ [ $accumulate GetOutput ] GetOrigin] 0]
    set spacing [lindex [ [ $accumulate GetOutput ] GetSpacing] 0]
    
    if { $LOWER_LEVEL <=   $inputRangeOrig0  || $LOWER_LEVEL >= $inputRangeOrig1  } { 
	set lower_bin   [ expr 0.2*$numBins]
	set LOWER_LEVEL [ expr $origin + $spacing * $lower_bin ]
    } else {
	if { $LOWER_LEVEL <=   $inputRange0  || $LOWER_LEVEL >= $inputRange1  } { 
	    set lower_bin 0
	} else {
	    set lower_bin   [ expr ($LOWER_LEVEL - $lower_bound)/$spacing ]
	}
    }
    
    set lower_pos   [ expr $lower_bin * $wscale ] 
    $drawcanvas create rect  $lower_pos 0 [expr $lower_pos +1 ] $cheight -fill gray -outline gray -tag grayslider
    
    
    if { $UPPER_LEVEL <=   $inputRangeOrig0  || $UPPER_LEVEL >=  $inputRangeOrig1  } { 
	set upper_bin   [ expr 0.9*$numBins]
	set UPPER_LEVEL [ expr $origin + $spacing * $upper_bin ]
    } else {
	if { $UPPER_LEVEL <=   $inputRange0  || $UPPER_LEVEL >=  $inputRange1  } { 
	    set upper_bin $numBins
	} else {
	    set upper_bin [ expr ($UPPER_LEVEL- $lower_bound)/$spacing ] 
	}
    }
    
    set upper_pos   [ expr $upper_bin * $wscale ] 
    $drawcanvas create rect  $upper_pos 0 [expr $upper_pos +1 ] $cheight -fill white -outline white -tag whiteslider
    
    
    set str1 [format "Range \[%.1f:%.1f\] Lv=%.1f,%.1f Ent=%4.3f" $lower_bound $upper_bound $LOWER_LEVEL $UPPER_LEVEL $entropy]
    $basewidget.bottom.label1 configure -text $str1
    
}
    
# -------------------------------------------------------------------------------------------        
# ---- Bindings and interaction procedures ----

::itcl::body pxitclHistogramWidget::Bind {  probe } {

    eval "bind $basewidget <Expose> { $this Render }"

	
    if {$probe > 0 } {
	eval "bind $drawcanvas <ButtonPress>   { $this StartInteraction   %x %y %b}"
	eval "bind $drawcanvas <B1-Motion>     { $this UpdateInteraction  %x %y 1}"
	eval "bind $drawcanvas <ButtonRelease> { $this EndInteraction }  "
    }
}
    
# -------------------------------------------------------------------------------------------    
::itcl::body pxitclHistogramWidget::StartInteraction { x y b} {
    UpdateInteraction  $x $y $b 
}
    
# -------------------------------------------------------------------------------------------    
::itcl::body pxitclHistogramWidget::EndInteraction { } {

    if { $hasinput == 0 } { return }

    set data [ [ [ $accumulate GetInput]  GetPointData ] GetScalars ]

    set inputRange0 [lindex [$data GetRange] 0]
    set inputRange1 [lindex [$data GetRange] 1]
    
    set lower_bound $LOWER_BOUND  
    set upper_bound $UPPER_BOUND  

    if { $lower_bound >= $inputRange0  && $lower_bound < $inputRange1  } {
	set inputRange0 $lower_bound
    } 
    
    if { $upper_bound >= $inputRange0  && $upper_bound < $inputRange1  } {
	set inputRange1 $upper_bound
    } 
    
    set str1 [format "Range \[%.1f:%.1f\] Gray=%.1f, White=%.1f Bins=%d" $inputRange0 $inputRange1 $LOWER_LEVEL $UPPER_LEVEL $NUM_BINS]
    $basewidget.bottom.label1 configure -text $str1
    set DRAGGING 0

    if { $callback !=0 } {
	catch { eval $callback }
    }
}
    
# ------------------------------------------------------------------------------------------ -   
::itcl::body pxitclHistogramWidget::UpdateInteraction { x y butno} {

    # compute the bin selected by the mouse
    if { $hasinput == 0 } { return }

    set lower_bound $LOWER_BOUND  
    set upper_bound $UPPER_BOUND  

    set cwidth  [ $drawcanvas cget -width  ]
    set cheight  [ $drawcanvas cget -height  ]
    set numBins $NUM_BINS
    set wscale [ expr $cwidth / $numBins ]

    set xcoord $x
    set x [expr $x / $wscale]

    set origin [lindex [ [ $accumulate GetOutput ] GetOrigin ] 0 ]
    set spacing [lindex [ [ $accumulate GetOutput ] GetSpacing ] 0 ]
    set binMin [expr $origin + $spacing * $x]
    set binMax [expr $binMin + $spacing]
    set gap [ expr 2.0* $spacing ]
    
    # now get the height of the histogram
    set data [$accumulate GetOutput]
    # $data Update
    # make sure value is in extent
    set max [lindex [$data GetExtent] 1]
    if {$x < 0 || $x > $max} {
	return
    }
    


    if { $butno == 1 } {
	set dr  $DRAGGING 
	if { $dr > 0 } { 
	    set butno [ expr $dr + 1 ] 
	} else {
	    set butno 1
	    set diff1 [ expr $wscale*abs($binMin- $LOWER_LEVEL) ]
	    if { $diff1 < $gap } { 
		set butno 2 
	    } else {
		set diff1 [ expr $wscale*abs($binMin- $UPPER_LEVEL) ]
		if { $diff1 < $gap } { 
		    set butno 3 
		}
	    }
	}
    }
    
    
    set y [$data GetScalarComponentAsDouble $x 0 0 0] 
    
    switch -exact $butno {
	1 {
	    set y [$data GetScalarComponentAsDouble $x 0 0 0] 
	    # format the string to display
	    set str1 [format "Bin: \[%.1f, %.1f) value=%.1f" $binMin $binMax $y]
	    $basewidget.bottom.label1 configure -text $str1
	    set DRAGGING 0
	} 
	"2" {
	    set str1 "Setting Lower Level $binMin (value = $y)"
	    $basewidget.bottom.label1 configure -text $str1
	    $drawcanvas coords grayslider $xcoord 0 [expr $xcoord + 1 ] $cheight 
	    #		[$actor GetPositionCoordinate] SetValue $xcoord  0
	    set LOWER_LEVEL $binMin
	    set DRAGGING 1
	} 
	"3" {
	    set str1 "Setting Upper Level $binMin (value=$y)"
	    $basewidget.bottom.label1 configure -text $str1
	    $drawcanvas coords whiteslider $xcoord 0 [expr $xcoord +1 ] $cheight
	    set UPPER_LEVEL $binMin
	    set DRAGGING 2
	}
    }
}

# -------------------------------------------------------------------------------------------
::itcl::body pxitclHistogramWidget::Save { } {
    
    set f  [tk_getSaveFile -title "Histogram Filename" -filetypes { {"Histogram Data File" {.dat}}} -parent $basewidget ]
    if  { [string length $f] < 1 } { return 0 }

    set ok [ $accumulate SaveGnuplot $f ]
    if { $ok == 0 } {
	::pxtclutil::Warning "Failed to save histogram in $f"
    } else {
	::pxtclutil::Info "Histogram saved in $f ([file size $f])"
    }
    

}
# -------------------------------------------------------------------------------------------
::itcl::body pxitclHistogramWidget::ZeroMaximum { data } {

    set scal [ [ $data GetPointData ] GetScalars ]
    set rmax [ lindex [ $scal GetRange ]  1 ]
    
    set nt   [ $scal GetNumberOfTuples ]
    for { set i 0 } { $i < $nt } { incr i } {
	set v [ $scal GetComponent $i 0 ]
	if { $v == $rmax } {
	    $scal SetComponent $i 0 0.0
	}
    }

    $scal Modified
    set rmax [ lindex [ $scal GetRange ]  1 ]
}

