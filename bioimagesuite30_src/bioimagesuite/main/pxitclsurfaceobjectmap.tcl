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

package provide pxitclsurfaceobjectmap 1.0
package provide pxitclsurobj 1.0





# 	$Id: pxitclsurfaceobjectmap.tcl,v 1.1 2002/06/11 01:21:41 papad Exp papad $	

package require  labelframe 1.0
package require  pxtclutil  1.0
package require  pxvtable   1.0
package require  vtkpxcontrib 1.1
package require  pxitclbasecontrol 1.0


# ---------------------------------------------------------------------------
#  Ancilliary Class SurObj
# ---------------------------------------------------------------------------
itcl::class pxitclsurobj {

    public    variable outsurface  0
    protected variable surface  0
    protected variable basewidget 0
    protected variable initialized 0
    public    common surfaceutil 0
    private   common thisparam

    constructor { } {
	if { $surfaceutil == 0 } {
	    set surfaceutil [ vtkpxSurfaceUtil [ pxvtable::vnewobj ]]
	}
	Reset
    }

    destructor  { 
	{ catch $surface Delete     }
	{ catch $outsurface Delete  }
    }

    public method  Initialize { basewidget args  } 
    public method  Load       { fname }
    public method  Save   { fname }
    public method  Resample   { scalefactor }
    public method  Update     { }
    public method  GetThisPointer { } { return $this }
    public method  Reset { } 

    # Access for Load /Save
    # Get Methods
    public method  GetActive       { } { return $thisparam($this,active)       }
    public method  GetTargetLabel  { } { return $thisparam($this,targetlabel)  }
    public method  GetTargetSpacing { } { return $thisparam($this,targetspacing) }
    public method  GetFileName     { } { return $thisparam($this,filename)     }

    # Set Methods
    public method  SetActive       { s } { set thisparam($this,active)       $s }
    public method  SetTargetLabel  { s } { set thisparam($this,targetlabel)  $s }
    public method  SetTargetSpacing { s } { set thisparam($this,targetspacing) $s }
    public method  SetFileName     { s } { set thisparam($this,filename)     $s }
    
    
    # Internal Methods to do real work
    protected method  ResampleSurface   { scalefactor }
    protected method  CleanSurface   { input output distance  }
    protected method  ResampleLandmarks { scalefactor }

    protected method  LoadSurface   { numpoints }
    protected method  LoadLandmarks { numpoints }



}
# ---------------------------------------------------------------------------
itcl::body pxitclsurobj::Reset {  } {

    set thisparam($this,filename) ""
    
    # Input Parameters
    if { $surface !=0 } {
	catch { $surface Delete }
    }
    set surface [ vtkPolyData [ pxvtable::vnewobj ]]
    set thisparam($this,class)  "Surface"
    set thisparam($this,shortfilename) ""
    set thisparam($this,inpoints)    0
    
    # Output Parameters
    if { $outsurface !=0 } { 
	catch { $outsurface Delete }
    }
    set outsurface [ vtkPolyData [ pxvtable::vnewobj ]]
    set thisparam($this,label)     1
    set thisparam($this,points)    0
    set thisparam($this,active)    0
    
    # Target Parameters
    set thisparam($this,targetlabel)    -1
    set thisparam($this,targetspacing) "100"
}
# ---------------------------------------------------------------------------	
itcl::body pxitclsurobj::Load { fname } {

    set typelist { 
	{"Landmark/Surface Files" {".land" ".vtk" } }
	{"Landmarks" {.land}} 
	{"Surfaces"  {.vtk}}
	{"All Files" {*}}
    }
	    
    if { [ string length $fname ] < 2 } {
	set f1 $thisparam($this,filename)
	set f1path [ file dirname $f1 ]
	set f1tail [ file tail    $f1 ]

	set fname [tk_getOpenFile -title "Load Surface/Landmark" -initialfile $f1tail -initialdir $f1path -filetypes $typelist]
    }
    
    if { [string length $fname ] < 2 } {
	return 0;
    }
    
    if { [ file extension $fname  ]== ".land" } {
	set ok [ LoadLandmarks $fname  ]
	if { $ok == 1 } {
	    set thisparam($this,filename) $fname  
	}
    } else {
	set ok [ LoadSurface $fname ]
	set thisparam($this,filename) $fname 
    }

    set thisparam($this,shortfilename) [ file tail $thisparam($this,filename) ]
    Update
    return $ok
}

itcl::body pxitclsurobj::LoadLandmarks { fname } {

    if { [ $surface IsA vtkpxBaseCurve ] == 0 } {
	$surface Delete
	set surface [ vtkpxBaseCurve [ pxvtable::vnewobj ]]
    }
    set thisparam($this,class)  "Landmarks"
    return [ $surface Load $fname ]
}
# ---------------------------------------------------------------------------
itcl::body pxitclsurobj::LoadSurface { fname } {
    
    if { [ $surface IsA vtkpxBaseCurve ] == 1 } {
	$surface Delete
	set surface [ vtkPolyData [ pxvtable::vnewobj ]]
    }

    set rd [ vtkPolyDataReader [ pxvtable::vnewobj ]]
    $rd SetFileName $fname
    $rd Update

    $surface ShallowCopy [ $rd GetOutput ]
    set thisparam($this,class)  "Surface"
    $rd Delete
}
# ---------------------------------------------------------------------------
itcl::body pxitclsurobj::Save { fname } {

    set typelist { 
	{"Surfaces"  {.vtk}}
	{"All Files" {*}}
    }
	    
    set initial [ file dirname $thisparam($this,filename) ]
    set f2  [ file tail [ file root $thisparam($this,filename) ] ]
    set initial [ file join $initial "re_$f2" ]

    if { [ string length $fname ] < 2 } {
	set f1path [ file dirname $initial ]
	set f1tail [ file tail    $initial ]

	set fname [tk_getSaveFile -title "Save Surface" -initialfile $f1tail -initialdir $f1path -filetypes $typelist]
    }
    
    if { [string length $fname ] < 2 } {
	return 0;
    }

    set wr [ vtkPolyDataWriter [ pxvtable::vnewobj ]]
    $wr SetFileName $fname
    $wr SetInput $outsurface
    $wr  SetFileTypeToASCII
    $wr Update
    $wr Delete

}
# ---------------------------------------------------------------------------
itcl::body pxitclsurobj::Update  { } {

    if { [ $surface IsA vtkpxBaseCurve ] == 1 } {
	set thisparam($this,inpoints) [ $surface GetNumPoints ]
    } else {
	set thisparam($this,inpoints) [ $surface GetNumberOfPoints ]
    }
}
# ---------------------------------------------------------------------------
itcl::body pxitclsurobj::Resample { scalefactor } {

    Update

    if { $thisparam($this,targetspacing) < 0.1 } {
	set thisparam($this,targetspacing) 0.1
    } elseif { $thisparam($this,targetspacing) >= 20.0 } {
	set thisparam($this,targetspacing) 20.0
    }


    
    if { $thisparam($this,inpoints) < 2 } {
	return 0
    }

    if { [ $surface IsA vtkpxBaseCurve ] } {
	 ResampleLandmarks $scalefactor
    } else {
	ResampleSurface $scalefactor
    }
    $surfaceutil AddObjectLabelsToSurface $outsurface [ expr -1 * ($thisparam($this,targetlabel)) ]
    set thisparam($this,label) $thisparam($this,targetlabel)

}

itcl::body pxitclsurobj::ResampleLandmarks { scalefactor } {


    set length [ $surface GetLength ]
    #set sampl  [ expr $length / ( $thisparam($this,targetspacing) + 0.01 )]

    set sampl [ expr $thisparam($this,targetspacing)  *$scalefactor ]

    set newcur [ vtkpxBaseCurve [ pxvtable::vnewobj ]]
    $newcur Copy $surface
    $newcur Equispace $sampl
    #sampl
    $newcur Smooth 0.33
    set ok [ $newcur ExportToPolyData $outsurface -1.0 ]
    set thisparam($this,points) [ $outsurface GetNumberOfPoints ]
    $newcur Delete
    pxtkconsole
    pxtkprint "Resampled Curve length=$length sampl=$sampl outp=$thisparam($this,points)\t\tBounds [ $outsurface GetBounds ]\n"
    return $ok
}

itcl::body pxitclsurobj::CleanSurface { input output distance } {

    set cleanerFilter [ vtkCleanPolyData [ pxvtable::vnewobj ]]
    $cleanerFilter SetInput $input
    $cleanerFilter ToleranceIsAbsoluteOn
    $cleanerFilter SetAbsoluteTolerance $distance
    $cleanerFilter Update
    $output ShallowCopy [ $cleanerFilter GetOutput ]
    $cleanerFilter Delete
}

itcl::body pxitclsurobj::ResampleSurface { scalefactor } {
    
    #set dist 2.0 
    #    set thisparam($this,points) [ expr $thisparam($this,targetspacing) + 100 ]
    set dist [ expr $thisparam($this,targetspacing) * $scalefactor ]
    pxtkconsole
    #while { ( $dist < 15.0 ) && ( $thisparam($this,points) >  $thisparam($this,targetspacing) ) } {

    CleanSurface $surface $outsurface $dist
    #dist
    set thisparam($this,points) [ $outsurface GetNumberOfPoints ]
    pxtkprint "input=$thisparam($this,inpoints) --> $thisparam($this,points) dist=$dist\t\tBounds: [ $outsurface GetBounds ]\n"
    #set dist [ expr $dist * 1.1 ]
    #}

}
# ---------------------------------------------------------------------------

itcl::body pxitclsurobj::Initialize { basewid  args } {

    set clr "darkgray"
    if { [ llength $args ] > 0 } {
	set clr [ lindex $args 0 ]
    }

    if { $initialized == 1 } { return $basewidget }
    
    set basewidget  [ frame $basewid -bg $clr -bd 4 ]
    set initialized 1

    set left   [ frame $basewidget.left   ]
    set middle [ frame $basewidget.middle  ]
    set right  [ frame $basewidget.right  ]

    pack $left   -side left -expand t -fill both
    pack $middle -side left -padx 10 -expand t -fill both
    pack $right -side left  -expand t -fill both



    # Left is Input Frame 

    set tp [ frame $left.top ]
    set bt [ frame $left.bot ]
    pack $tp $bt -side top -expand t -fill both -pady 1

    
    iwidgets::entryfield $tp.name -labeltext "Fname:" -width 20 -textvariable [ itcl::scope thisparam($this,shortfilename) ]
    iwidgets::entryfield $bt.class -labeltext "Type:"   -width 10 -textvariable [ itcl::scope thisparam($this,class) ]
    iwidgets::entryfield $bt.pt -labeltext "Points:" -width 5 -textvariable [ itcl::scope thisparam($this,inpoints) ]
    [ $tp.name  component entry ] configure -state disabled -bg white
    [ $bt.pt    component entry ] configure -state disabled -bg white
    [ $bt.class component entry ] configure -state disabled -bg white
    eval "button $tp.load -text \"Load\" -command { $this Load \"\" } "
    pack $tp.load -side right -padx 2
    pack $tp.name -side left -padx 2 -expand f -fill x
    pack $bt.class $bt.pt -side left -padx 2 -expand f 

    set tp [ frame $middle.top ] ; set bt [ frame $middle.bot ] ; 
    pack $tp $bt -side top -expand t -fill both -pady 1
    
    iwidgets::entryfield $tp.targ -labeltext "Target Spacing:" -width 5 -textvariable [ itcl::scope thisparam($this,targetspacing) ]
    eval "button $tp.resamp -text \"Resample!\" -command { $this Resample 1.0 }"
    iwidgets::entryfield $bt.targ -labeltext "Target Label:" -width 2 -textvariable [ itcl::scope thisparam($this,targetlabel) ]
    eval "button $bt.save -text \"Save!\" -command { $this Save \"\"}"
    pack $tp.targ  -side left -expand f 
    pack $bt.targ -side left -expand f 
    pack $tp.resamp -side right -expand f 
    pack $bt.save -side right -expand f 

    set tp [ frame $right.top ] ; set bt [ frame $right.bot ]
    pack $tp $bt -side top -expand t -fill both -pady 1

    iwidgets::entryfield $tp.actual -labeltext "Points:" -width 5 -textvariable [ itcl::scope thisparam($this,points) ]
    iwidgets::entryfield $tp.lab    -labeltext "Out Label:" -width 2 -validate numeric -textvariable [ itcl::scope thisparam($this,label) ]
    [ $tp.lab component entry ] configure -state disabled -bg white
    [ $tp.actual component entry ] configure -state disabled -bg white
    

    checkbutton $bt.act -text "Active" -variable [ itcl::scope thisparam($this,active) ]
    pack $tp.actual $tp.lab -side right -expand f -padx 10
    pack $bt.act -side left -expand f 
    return $basewidget
}


# ------------------------------------------------------------------------------------------
#
# pxitclsurfaceobjectmap control
#
# ------------------------------------------------------------------------------------------

itcl::class pxitclsurfaceobjectmap {

    inherit pxitclbasecontrol

    private   common   thisparam
    protected variable polydatacontrol 0
    protected variable initialized 0
    protected variable combo_surface 0
    protected variable brain_utility 0 
    public    variable numberofsurfaces 
    protected common surface


    constructor { par } {
	::pxitclbasecontrol::constructor $par
    } { 
	set numberofsurfaces 15
	set thisparam($this,outputsurface) 1
	set thisparam($this,scalefactor) 1.0
    }

    protected method InitializeVariables  { }
    public    method GenerateComboSurface { }
    public    method Initialize  { basewidget args } 
    public    method SaveSetup   { fname } 
    public    method LoadSetup   { fname }
    public    method ResampleAll { }
    
}

# ------------------------------------------------------------------------------------------
itcl::body pxitclsurfaceobjectmap::Initialize { basewidg args } {

    if { $initialized == 1 } { return $basewidget }

    set basewidget [ toplevel $basewidg ] 
    wm withdraw $basewidget
    wm geometry $basewidget 800x400
    wm title $basewidget "Surface Object Map Generator"

    set w [ frame $basewidget.bottom ]
    set w2 [ frame $basewidget.bottom2 ]
    pack $w $w2 -side bottom -expand f -fill x -pady 2

    
    eval "button $w.1 -text Close -command { $this HideWindow } "
    pack $w.1 -side right

    set num 2; catch { set num [ $polydatacontrol  GetNumberOfSurfaces ]  }

    eval "button $w.l -text \"Load Setup\" -command { $this LoadSetup \"\" }"
    eval "button $w.s -text \"Save Setup\" -command { $this SaveSetup \"\" }"
    eval "button $w.r -text \"Resample All\" -command { $this ResampleAll  }"
    label $w.l1 -text "x"
    tk_optionMenu $w.opt [ itcl::scope thisparam($this,scalefactor) ] 0.25 0.5 0.75 0.9 1.0 1.1 1.2 1.25 1.33 1.5 2.0 2.5 3.0 3.5 4.0
    pack $w.l $w.s $w.r $w.l1 $w.opt  -side left -padx 5 

    eval "button $w2.p -text \"Create Combo Surface\" -command { $this GenerateComboSurface }"
    pack $w2.p -side left

    if { [ llength $args ] > 0 } {
	set polydatacontrol [ lindex $args 0 ]
	eval "button $w2.2 -text \"Show Surface Control\" -command { $polydatacontrol Show 1 }"
	pack $w2.2 -side left
    } else {
	set polydatacontrol 0
    }
    set l [ AddLogoLabel $w2 ];     pack $l -side right -padx 1 


    iwidgets::scrolledframe $basewidget.sf -width 4i -height 2i -labeltext "Components"
    pack $basewidget.sf -side top -expand t -fill both

    set win [ $basewidget.sf childsite]

    set a(0) "black"
    set a(1) "white"
    set count 0

    for { set i 1 } { $i <= $numberofsurfaces } { incr i } {
	
	set pr [ iwidgets::Labeledframe $win.$i -labelpos w -labeltext "# $i" ]
	pack $win.$i -side top

	set surface($this,$i) [  [ pxitclsurobj \#auto ] GetThisPointer ]
	if { $i == 1 } {
	    $surface($this,$i) SetTargetSpacing 3.0
	    $surface($this,$i) SetTargetLabel 1
	} else {
	    $surface($this,$i) SetTargetSpacing 0.33
	    $surface($this,$i) SetTargetLabel $i
	}
 
	lappend itclobjectlist $surface($this,$i)
	set fr [ $surface($this,$i) Initialize [ $pr childsite].1  $a($count) ]
	pack $fr -side top -expand f -fill x -pady 10
	set count [expr 1-$count ]
    }

    set initialized 1
    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget 
}

# ------------------------------------------------------------------------------------------     
itcl::body pxitclsurfaceobjectmap::GenerateComboSurface { } {


    set bad -1

    for { set i 1 } { $i <= $numberofsurfaces } { incr i } {

	set flag [ $surface($this,$i) GetActive ]
	if { $flag == 1 } { 
	    set np [ [ $surface($this,$i) cget -outsurface ] GetNumberOfPoints ]
	    if { $np < 2 } {
		set bad $i
		set i [ expr $numberofsurfaces+1 ]
	    } 
	}
    }

    if { $bad > 0 } {
	::pxtclutil::Warning "Surface $bad has no points! Cannot Generate Combo Surface!"
	return 0
    }

    set numinp 0
    set appnd [ vtkAppendPolyData [ pxvtable::vnewobj ] ]
    for { set i 1 } { $i <= $numberofsurfaces } { incr i } {

	set flag [ $surface($this,$i) GetActive ]
	if { $flag == 1 } { 
	    set sur [ $surface($this,$i) cget -outsurface ]; 
	    puts stdout "Adding Surface $i np = [ $sur GetNumberOfPoints ] \t\t Bounds = [ $sur GetBounds ]"
	    $appnd AddInput [ $surface($this,$i) cget -outsurface ]
	    incr numinp
	}
    }

    if { $numinp > 0 } {

	$appnd Update
	[ $appnd GetOutput ] ComputeBounds
	puts stdout "Creating Combo Surface Completed. The resulting surface is stored on the clipboard of the surface control\n(You must paste this into a surface before it becomes available) [ [ $appnd GetOutput ] GetBounds ]"
	[ $surface($this,1) cget -surfaceutil ] AddObjectMapLookupTableToSurface [ $appnd GetOutput ] 
	
	if { $polydatacontrol != 0 } {
	    $polydatacontrol  SetClipboardSurface [ $appnd GetOutput ] "combo_surface"
	    ::pxtclutil::Info "Creating Combo Surface Completed. The resulting surface is stored on the clipboard of the surface control\n(You must paste this into a surface before it becomes available) [ [ $appnd GetOutput ] GetBounds ]"
	}
    }
    $appnd Delete
}
# ------------------------------------------------------------------------------------------
itcl::body pxitclsurfaceobjectmap::SaveSetup { fname } {

    set typelist { 
	{"Surface Objectmap setup" {.sobj}} 
	{"All Files" {*}}
    }
	    
    if { [ string length $fname ] < 2 } {
	set fname [tk_getSaveFile -title "Save Setup for Surface Objectmap" -filetypes $typelist]
    }
    
    if { [string length $fname ] < 2 } {
	return 0;
    }

    set fileid [open $fname w]
    
    puts $fileid "#Surface Object Map Setup File\n-----------------------------------"

    set count 0
    for { set i 1 } { $i <= $numberofsurfaces } { incr i } {
	set fn [ $surface($this,$i) GetFileName ]
	if { [ string length $fn ] > 1 } { 
	    incr count
	}
    }

    puts $fileid "#Number Of Surfaces = $count"
    set index 1
    for { set i 1 } { $i <= $numberofsurfaces } { incr i } {
	set fn [ $surface($this,$i) GetFileName ]
	if { [ string length $fn ] > 1 } { 
	    puts $fileid "# ------------- Surface $index ----------------------------"
	    set act [ $surface($this,$i) GetActive ]
	    set lab [ $surface($this,$i) GetTargetLabel ]
	    set tp  [ $surface($this,$i) GetTargetSpacing ]
	    puts $fileid "Filename=$fn\nActive=$act\nTarget Label=$lab\nTarget Points=$tp"
	    incr index
	}
    }
    close $fileid
    return 1
}
# ------------------------------------------------------------------------------------------
itcl::body pxitclsurfaceobjectmap::LoadSetup { fname } {

    set typelist { 
	{"Surface Objectmap setup" {.sobj}} 
	{"All Files" {*}}
    }
	    
    if { [ string length $fname ] < 2 } {
	set fname [tk_getOpenFile -title "Load Setup for Surface Objectmap" -filetypes $typelist]
    }
    
    if { [string length $fname ] < 2 } {
	return 0;
    }

    if { [ file exists $fname ] == 0 } {
	return 0;
    }

    set fileid [open $fname r]
    
    set line ""
    gets $fileid line 
    
    # Get Number Of Surfaces
    if { [ string compare $line "#Surface Object Map Setup File" ] != 0} { 
	::pxtclutil::Warning "Cannot read setup file\n Bad Header = $line \n"
	close $fileid
	return 0
    }

    # Skip Next Line
    gets $fileid line ; 
   
    # Get Number Of Surfaces
    gets $fileid line ; set numsur [ lindex [ split $line "=" ] 1]

    if { $numsur > $numberofsurfaces } {
	set numsur $numberofsurfaces
    }

    for { set i 1 } { $i <= $numsur } { incr i } {
	gets $fileid line 

	gets $fileid line ; set fname [ lindex [ split $line "=" ] 1 ]
	$surface($this,$i) SetFileName $fname

	
	gets $fileid line ; set active [ lindex [ split $line "=" ] 1 ]
	$surface($this,$i) SetActive $active


	gets $fileid line ; set lab [ lindex [ split $line "=" ] 1 ]
	$surface($this,$i) SetTargetLabel $lab 

	gets $fileid line ; set tp [ lindex [ split $line "=" ] 1 ]

	if { $tp > 10.0 && $tp < 300 } { 
	    set tp [ expr 64.0 / ( 0.01 + $tp )  ]
	} elseif { $tp > 500 } {
	    set tp [ expr 15000.0 / ( 0.01+$tp) ]
	}
	set tp [ format "%.2f" $tp ]

	$surface($this,$i) SetTargetSpacing $tp

	$surface($this,$i) Load $fname
    }

    for { set i [ expr $numsur+1] } { $i <= $numberofsurfaces } { incr i } {
	$surface($this,$i) Reset
    }

    close $fileid
    return 1
}
# ------------------------------------------------------------------------------------------

itcl::body pxitclsurfaceobjectmap::ResampleAll { } {

    for { set i 1 } { $i <= $numberofsurfaces } { incr i } {
	$surface($this,$i) Resample $thisparam($this,scalefactor)
    }
}

