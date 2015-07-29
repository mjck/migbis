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



package provide pxitclanimationgadget 1.0


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
package require  pxitclcamera           1.0
package require  pxitcltransform        1.0
package require  pxitclbaseimagecontrol 1.0

# ----------------------------------------------------------------------------------------------

itcl::class pxitclanimationgadget {

    inherit pxitclbasecontrol
    
    # Generic Stuff 

    protected common   thisparam        
    protected common   animation_initialized 0

    # Animation Stuff
    public    variable animation_frame     1
    public    variable animation_stop      0
    protected variable animation_manualstatus  0
    protected variable animation_manualbutton 0
    protected variable animation_startbutton 0
    protected variable animation_stopbutton 0
    protected variable vtk_viewer 0
    protected variable spinning   0
    protected variable cameracontrol 0
    protected variable setupfilename ""
    protected variable titlestring "Animation Tool"

    # public 

    
    #
    constructor { par args } {
	pxitclbasecontrol::constructor $par
    } {
	InitializeAnimationGadget
    }
    protected method InitializeAnimationGadget { }
    public method Initialize { widget }
    public method DelayedInitialize { }
    public method SetViewer { vr }  { set vtk_viewer $vr }
    public method AddToMenuButton { mb args }
    public method GetAnimationName { }
    public method SaveAnimationFrame { mode }
    public method ManualGrabNextFrame { }
    public method ShowViewerControls { }
    public method SetViewerTo3DMode { }

    public method LoadScript { args } 
    public method SaveScript { args } 
    public method GrabFromViewer { }
    public method UpdateFromViewer { }
    public method SendToViewer { forcemode } 
    public method AddSpin { angle }
    public method AnimateScript { }

    protected method SetRendererClips { camera }
    protected method SetWeightedRendererClips { camera camera2 factor }
    protected method GetRendererClips { camera }


}


::itcl::body pxitclanimationgadget::AddToMenuButton { mb args} {
    eval "$mb add command -label \"Animation Tool\" -command { $this ShowWindow }"
}

itcl::body pxitclanimationgadget::InitializeAnimationGadget { } {

    set thisparam($this,animationdelay) 200
    set thisparam($this,animationname) "grabbedframe.jpg"
    set thisparam($this,autosave) 0
    set thisparam($this,scriptdivide) 15
    set thisparam($this,scriptbegin) -1
    set thisparam($this,scriptend) 1000
    set thisparam($this,scriptunit)   0.2
    set thisparam($this,autoupdate) 0
    set thisparam($this,animateandsave) 0
    set thisparam($this,spinpositive) 1
    set thisparam($this,spinazimuth) 1
}

itcl::body pxitclanimationgadget::Initialize { widget } { 
	

    set basewidget [ toplevel $widget ]
    wm geometry $basewidget 600x400
    wm withdraw $basewidget

    SetTitle $titlestring
    set initialized 1    

    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget

}

itcl::body pxitclanimationgadget::DelayedInitialize { } { 

    if { $fullyinitialized == 1 } {
	return
    }

    set menubase     [ menu $basewidget.menu ]; $basewidget configure -menu $menubase

    set pbar     [ frame $basewidget.pbar    -width 450 -height 20 ]
    set notebook $basewidget.notebook
    iwidgets::tabnotebook $notebook  -tabpos n

    pack $pbar     -side bottom -expand f -fill x -pady 1
    pack $notebook -side top -expand t -fill both -pady 1

    set w  [ $notebook add -label "Main" ]
    set w2 [ $notebook add -label "Script " ]

    frame $w.1 ; frame $w.2 ; frame $w.3
    pack $w.1 $w.2 $w.3 -side top -expand false -fill x -pady 1 -padx 2
    
    iwidgets::entryfield $w.1.1 -labeltext "Base name:" -textvariable [ itcl::scope thisparam($this,animationname) ] -relief sunken -width 40 
    eval "button $w.1.2  -text \"Browse\" -command { $this GetAnimationName }"
    pack $w.1.1 $w.1.2 -side left -padx 3
    
    label $w.2.1 -text "Delay Time (ms): "
    tk_optionMenu $w.2.2 [ itcl::scope thisparam($this,animationdelay) ]  "50" "100" "200" "500" "1000" "2000" "Manual"
    eval "button $w.2.3 -text \"Grab Next <ctrl-a>\" -command { $this ManualGrabNextFrame  }" 
    set animation_manualbutton $w.2.3
    pack $w.2.1 $w.2.2 $w.2.3 -side left -padx 3
    
    pack forget $animation_manualbutton 
    
    
    eval "button $w.3.1  -text \"Start Grabbing\" -command { $this SaveAnimationFrame begin }"
    eval "button $w.3.2  -text \"Stop Grabbing\" -command { $this  SaveAnimationFrame end }"
    pack $w.3.1 $w.3.2  -side left -padx 1
    
    set animation_startbutton $w.3.1
    set animation_stopbutton $w.3.2
    
    DisableWidget $animation_stopbutton
    eval "bind all <Control-a> { $this ManualGrabNextFrame} "


    eval "button $pbar.1  -text \"Show Viewer Controls\" -command { $this ShowViewerControls }"
    eval "button $pbar.2  -text \"Switch Viewer to 3D Mode\" -command { $this SetViewerTo3DMode }"
    set l [ AddLogoLabel $pbar ]
    pack $l -side right -padx 1
    pack $pbar.1 $pbar.2  -side left -padx 4

    frame $w2.d; pack $w2.d -side bottom -expand false -fill x -pady 2
    frame $w2.c; pack $w2.c -side bottom -expand false -fill x -pady 2
    frame $w2.bc -bg black -height 3 ; pack $w2.bc -side bottom -expand false -fill x -pady 1
    frame $w2.b; pack $w2.b -side bottom -expand false -fill x -pady 2

    set cameracontrol [ [ pxitclmultiCameraGUI \#auto  ] GetThisPointer ]
    $cameracontrol configure -enableadd 0
    $cameracontrol configure -enabledeleteall 1
    $cameracontrol Initialize $w2.a
    $cameracontrol configure -callback "$this SendToViewer 0"
    
    set cm [ $cameracontrol GetObjectGUI ]
    set lb [ $cm cget -loadbutton ]; pack forget $lb
    set lb [ $cm cget -savebutton ]; pack forget $lb
    set lb [ $cm cget -clearbutton ]; pack forget $lb
    $cameracontrol AddCamera "None"

    menu $menubase.filem -tearoff 0 ;      $menubase add cascade -label Setup      -menu $menubase.filem
    menu $menubase.spinm -tearoff 0 ;      $menubase add cascade -label Spin      -menu $menubase.spinm

    eval "$menubase.filem add command -label \"Load Script\" -command { $this LoadScript \"\" }"
    eval "$menubase.filem add command -label \"Save Script\" -command { $this SaveScript \"\" }"
    $menubase.filem add separator
    eval "$menubase.filem add command -label \"Close\" -command { wm withdraw $basewidget }"

    set mb $menubase.spinm
    eval "$mb add check -label \"Positive Rotation\" -variable [ itcl::scope thisparam($this,spinpositive)] "
    eval "$mb add check -label \"Do Azimuth\" -variable [ itcl::scope thisparam($this,spinazimuth)] "
    $mb add separator
    eval "$mb add command -label \"Spin 10\" -command { $this AddSpin 10 }"
    eval "$mb add command -label \"Spin 20\" -command { $this AddSpin 20 }"
    eval "$mb add command -label \"Spin 30\" -command { $this AddSpin 30 }"


    eval "button $w2.b.1 -text \"Add\" -command { $this GrabFromViewer }"
    eval "button $w2.b.2 -text \"Update\" -command { $this UpdateFromViewer }"
    eval "button $w2.b.3 -text \"Send To Viewer\" -command { $this SendToViewer 1 }"
    checkbutton $w2.b.6 -variable [ itcl::scope thisparam($this,autoupdate) ] -text "Auto Update"

    pack $w2.b.1 $w2.b.2 $w2.b.3 $w2.b.6 -side left -expand false -padx 1
    pack $w2.a -side top -expand false -fill x

   
    iwidgets::entryfield $w2.c.1 -labeltext "Subdivision:" -textvariable [ itcl::scope thisparam($this,scriptdivide) ] -relief sunken -width 4 -validate integer
    iwidgets::entryfield $w2.c.2 -labeltext "Duration (s):" -textvariable [ itcl::scope thisparam($this,scriptunit) ] -relief sunken -width 4 -validate real
    iwidgets::entryfield $w2.c.3 -labeltext "Begin :" -textvariable [ itcl::scope thisparam($this,scriptbegin) ] -relief sunken -width 4 -validate integer
    iwidgets::entryfield $w2.c.4 -labeltext "End   :" -textvariable [ itcl::scope thisparam($this,scriptend) ] -relief sunken -width 4 -validate integer

    pack $w2.c.1 $w2.c.2 $w2.c.3 $w2.c.4 -side left -padx 2


    eval "button $w2.d.1  -text \"Animate\" -command { $this AnimateScript }"
    checkbutton $w2.d.2 -variable [ itcl::scope thisparam($this,animateandsave) ] -text "Save While Animating"
    pack $w2.d.1 $w2.d.2  -side left -padx 2

    $notebook view "Main"

    set fullyinitialized 1
}
# ---------------------------------------------------------------------------------
itcl::body pxitclanimationgadget::GetAnimationName { } {
    
    
    set fname [tk_getSaveFile -title "Filename" -defaultextension .jpg -initialfile $thisparam($this,animationname)  -filetypes { {"JPG FILES" {.jpg}}} ]


    if { [ string length $fname ] < 1 } {
	return 0
    }

    set ext [ file extension $fname ]
    if { $ext != ".jpg" } {
	set fname "[ file root $fname ].jpg"
    }

    set thisparam($this,animationname) $fname
    return 1
	
}

itcl::body pxitclanimationgadget::ManualGrabNextFrame {  } {

    if { $animation_manualstatus == 1 } {
	$this SaveAnimationFrame "manual"
    }
}

itcl::body pxitclanimationgadget::SaveAnimationFrame { mode } {

    
    if { $mode == "end" } {
	wm title $basewidget $titlestring
	set animation_stop 1
	set animation_manualstatus 0
	catch { pack forget $animation_manualbutton  }
	DisableWidget $animation_stopbutton
	EnableWidget $animation_startbutton
	return
    }

    if { $mode == "continue" && $animation_stop == 1 } {
	wm title $basewidget $titlestring
	set animation_stop 1
	set animation_manualstatus 0
	catch { pack forget $animation_manualbutton  }
	return
    }

    if { $mode == "begin" } {
	set animation_frame 1
	set animation_stop  0
	if { $thisparam($this,animationdelay) == "Manual" } {
	    pack  $animation_manualbutton -side left -padx 2
	    wm title $basewidget  "Manual Grabbing in Progress .... "
	    set animation_manualstatus 1
	} else {
	    wm title $basewidget  "Grabbing in Progress .... "
	    set animation_manualstatus 0
	}
	DisableWidget $animation_startbutton
	EnableWidget $animation_stopbutton
    }

    set fname $thisparam($this,animationname) 
    set ext [ file extension $fname  ]
    if { $ext == ".jpg" } {
	set fname [ file root $fname ]
    }

    set outname [ format "%s_%04d.jpg" $fname $animation_frame ]
    wm title $basewidget  "Grabbing [ file tail $outname ]"

    incr animation_frame

    catch { 
	set renwin [ [ $vtk_viewer GetRenderWidgetName ] GetRenderWindow ]
	$vtk_viewer SaveAsTiff $renwin $outname 
    }

    if { $thisparam($this,animationdelay) != "Manual" } {
	eval "after $thisparam($this,animationdelay) $this SaveAnimationFrame continue"
    }
}

::itcl::body pxitclanimationgadget::SetViewerTo3DMode {} {
    if { $vtk_viewer == 0 } {
	return
    }

    $vtk_viewer SetDisplayMode3D
}

::itcl::body pxitclanimationgadget::ShowViewerControls { } {

    if { $vtk_viewer == 0 } {
	return
    }

    [ $vtk_viewer GetRenderer 3 ] ShowCameraControls
}

# ----------------------------------------------------------------------
itcl::body pxitclanimationgadget::SetRendererClips { camera } {


    set clipvalues [ $camera cget -polyclipvalues ]
    set ren 0
    catch { set ren [ $vtk_viewer GetRenderer 3 ] }
    if { $ren == 0  || [ llength $clipvalues ] !=6 } { return }

    for { set i 0 } { $i <= 5 } { incr i } {
	$ren SetClipScaleValue $i [ expr round([ lindex $clipvalues $i ])]
    }
    $ren UpdateClippingPlanes

    set clipvalues [ $camera cget -volclipvalues ]
    set vol 0
    catch { set vol [ $vtk_viewer GetVolumeControl ] }
    if { $ren == 0  || [ llength $clipvalues ] !=6 } { return }

    for { set i 0 } { $i <= 5 } { incr i } {
	$vol SetClipScaleValue $i [ expr round([ lindex $clipvalues $i ] )]
    }
    $vol UpdateCrop
}

itcl::body pxitclanimationgadget::SetWeightedRendererClips { camera camera2 factor } {

    set c1 [ $camera cget -polyclipvalues ]
    set c2 [ $camera2 cget -polyclipvalues ]
    if { [ llength $c1 ]  !=6  || [ llength $c2 ] !=6 } {
	puts stderr "Bad Clip Values"
	return
    }

    set ren [ $vtk_viewer GetRenderer 3 ]
    for { set i 0 } { $i <= 5 } { incr i } {
	set vv [ expr round($factor*[ lindex $c1 $i ]  + (1.0-$factor)*[lindex $c2 $i ] )]
	$ren SetClipScaleValue $i $vv 
    }
    $ren UpdateClippingPlanes

    set c1 [ $camera cget -volclipvalues ]
    set c2 [ $camera2 cget -volclipvalues ]
    if { [ llength $c1 ]  !=6  || [ llength $c2 ] !=6 } {
	puts stderr "Bad Clip Values"
	return
    }

    set vol 0
    catch { 	set vol [ $vtk_viewer GetVolumeControl ]    }
    if { $vol == 0 } {
	return
    }

    for { set i 0 } { $i <= 5 } { incr i } {
	set vv [ expr round($factor*[ lindex $c1 $i ]  + (1.0-$factor)*[lindex $c2 $i ] )]
	$vol SetClipScaleValue $i $vv 
    }
    $vol UpdateCrop
}

# ----------------------------------------------------------------------
itcl::body pxitclanimationgadget::GetRendererClips { camera } {

    set a { -1 -1 -1 -1 -1 -1  } 
    catch { 
	set vol [ $vtk_viewer GetVolumeControl ]
	set a ""
	for { set i 0 } { $i <= 5 } { incr i } {
	    lappend a [ $vol GetClipScaleValue $i ]
	}
    }
    $camera configure -volclipvalues $a

    set a { -1 -1 -1 -1 -1 -1  } 
    set ren 0
    catch { set ren [ $vtk_viewer GetRenderer 3 ] }
    if { $ren !=0 } {
	set a ""
	for { set i 0 } { $i <= 5 } { incr i } {
	    lappend a [ $ren GetClipScaleValue $i ]
	}
    }
    $camera configure -polyclipvalues $a
}
# ----------------------------------------------------------------------
itcl::body pxitclanimationgadget::GrabFromViewer { } {
    
    set old $thisparam($this,autoupdate)
    set thisparam($this,autoupdate) 0

    set ren [ $vtk_viewer GetRenderer 3 ]
    set incam [ [ $ren GetRenderer ] GetActiveCamera ]

    if { $spinning == 0 } {
	set f [ format "CM_%d" [ $cameracontrol GetNextObjectIndex ] ]
    } else {
	set f [ format "SP_%d" [ $cameracontrol GetNextObjectIndex ] ]
    }
    $cameracontrol AddObject  $f
    set newcam [ $cameracontrol GetCurrentObject ]
    $newcam configure -filename $f
    $newcam CopyCamera $incam
    $newcam configure -delay 1.0
    
    $this GetRendererClips $newcam
    update idletasks
    $cameracontrol CurrentObjectModified
    set thisparam($this,autoupdate) $old
}

itcl::body pxitclanimationgadget::UpdateFromViewer { } {
    set old $thisparam($this,autoupdate)
    set thisparam($this,autoupdate) 0
    
    set ren [ $vtk_viewer GetRenderer 3 ]
    set incam [ [ $ren GetRenderer ] GetActiveCamera ]
    set newcam  [ $cameracontrol GetCamera -1 ]
    $this GetRendererClips $newcam
    $newcam CopyCamera $incam
    update idletasks
    $cameracontrol CurrentObjectModified
    set thisparam($this,autoupdate) $old
}

itcl::body pxitclanimationgadget::SendToViewer { forcemode } {

    if { $forcemode == 0  &&  $thisparam($this,autoupdate)  == 0 } {
	return
    }
    
    $this SetRendererClips [ $cameracontrol GetCamera -1 ] 
    set ren [ $vtk_viewer GetRenderer 3 ]
    set newcam [ [ $ren GetRenderer ] GetActiveCamera ]
    $ren CopyCamera $newcam [ [ $cameracontrol GetCamera -1 ] GetCamera ]

    $ren CameraModifiedCallback
    $ren Render
}

itcl::body pxitclanimationgadget::LoadScript { args } {

    set len [ llength $args ]
   
    if { $len < 1 } {
	set fname $setupfilename 
    } else {
	set fname [ lindex $args 0 ]
    }

    if { [ string length $fname  ] == 0 } {
	set f1 $setupfilename
	set f1path [ file dirname $f1 ]
	set f1tail [ file tail    $f1 ]
	set fname  [tk_getOpenFile -title "Animation Script File" -filetypes { {"Animation Script File" {.anim}}} -initialfile $f1tail -initialdir $f1path ]
    }

    if { [ string length $fname ] < 1 } {
	return 0
    }

    if { [ file readable $fname ] == 0 } {
	::pxtclutil::Warning "Cannot read animation script file from $fname\n Bad File"
	return 0
    }

    set fileid [open $fname r]
	
    gets $fileid line

    set setupmode 0

    set mode "none"

    if { $line == "#Animation Script File"  } {
	set mode "old"
    } elseif {  $line == "#Animation Script File v2" } {
	set mode "new"
    }

    if { $mode == "none" } {
	close $fileid
	::pxtclutil::Warning "Cannot read animation script file from $fname\n Bad File"
	return
    }


    gets $fileid line
    gets $fileid numcameras

    if { $numcameras > 0 } {
	
	$cameracontrol DeleteAllObjects 1 0
	set count 1
	
	for { set i 0 } { $i < $numcameras } { incr i } {
	    set f [ format "C_%d" [ expr $i +1 ]]
	    $cameracontrol AddObject  $f
	    set newcam [ $cameracontrol GetCurrentObject ]
	    $newcam configure -filename $f
	    $newcam LoadStream $fileid $mode
	}
	close $fileid
	set titlestring "Animation Tool : $fname"
	wm title $basewidget $titlestring
	set setupfilename $fname
	$cameracontrol SetObject 0
    }
}

itcl::body pxitclanimationgadget::SaveScript { args } {


    set len [ llength $args ]
    if { $len < 1 } {
	set fname $setupfilename 
    } else {
	set fname [ lindex $args 0 ]
    }

    if { [ string length $fname  ] == 0  } {
	set f1 $setupfilename
	set f1path [ file dirname $f1 ]
	set f1tail [ file tail    $f1 ]
	set fname  [tk_getSaveFile  -title "Animation Script File" -filetypes { {"Animation Script File" {.anim}}} -initialfile $f1tail -initialdir $f1path ]
    }

    if { [ string length $fname ] > 0 } {

	set fileid [open $fname w]
	
	puts $fileid "#Animation Script File v2"

	set lst [ $cameracontrol cget -itclobjectlist ]
	puts $fileid "Focal Point \t Position \t ViewUp \t Clipping Range \t ParallelProjection \t Parallel Scale \t Duration\t Clipping Planes"
	puts $fileid "[llength $lst]"
	for { set i 0 } { $i < [ llength $lst ] } { incr i } {
	    set cm [ lindex $lst $i ]
	    $cm SaveStream $fileid
	}
	
	close $fileid
	set titlestring "Animation Tool : $fname"
	wm title $basewidget $titlestring
	set setupfilename $fname
    }
}



itcl::body pxitclanimationgadget::AnimateScript { } {

    set dosave $thisparam($this,animateandsave)

    if { $dosave == 1 } {
	set ok [ $this GetAnimationName ]
	if { $ok == 0 } {
	    return
	}

	set thisparam($this,animationdelay) "Manual"
	set animation_manualstatus 1
    }

    
	
    set ren [ $vtk_viewer GetRenderer 3 ]
    set newcam [ [ $ren GetRenderer ] GetActiveCamera ]

    if { $thisparam($this,scriptdivide)  > 30 } {
	set thisparam($this,scriptdivide) 30 
    } elseif { $thisparam($this,scriptdivide) < 1 } {
	set thisparam($this,scriptdivide) 1
    }

    if { $thisparam($this,scriptunit)  > 5} {
	set thisparam($this,scriptunit) 5 
    } elseif { $thisparam($this,scriptunit) < 0.1 } {
	set thisparam($this,scriptunit) 0.1
    }


    set numdiv  $thisparam($this,scriptdivide) 

    set del [ expr round((1000 * $thisparam($this,scriptunit)) / $numdiv) ]
    set lst [ $cameracontrol cget -itclobjectlist ]
    set numlst [ llength $lst ]
    set maxi [ expr $numlst -1 ]

    set endframe [ expr $thisparam($this,scriptend) -1 ]
    if { $endframe> $maxi } {
	set endframe $maxi
    } 

    set begframe [ expr $thisparam($this,scriptbegin) -1 ]
    if { $begframe < 0 } {
	set begframe 0
    }
	
    if { $begframe > $endframe } {
	set begframe [ expr $endframe -1 ]
	if { $begframe < 0 } {
	    set begframe 0
	}
    }
	
    for { set i $begframe } { $i < $endframe  } { incr i } {
	set cm [ lindex $lst $i ]
	$ren CopyCamera $newcam [ $cm GetCamera ]

	$this SetRendererClips $cm
	$ren Render

	set dv [ $cm cget -delay ]
	set ndiv [ expr round($dv * $numdiv)]
	update idletasks
	if { $dosave == 1 } {
	    if { $i == 0 } {
		$this SaveAnimationFrame begin
	    } else {
		$this ManualGrabNextFrame
	    }
	    wm withdraw $basewidget
	}
	after $del
	

	if { $i < $endframe } {
	    set cm2 [ lindex $lst [ expr $i +1 ] ]
	    for { set j 1 } { $j < $ndiv } { incr j } {
		set factor [ expr ($j+0.01)/($ndiv+0.01)]

		$this SetWeightedRendererClips $cm2 $cm $factor 
		$ren InterpolateCamera $newcam [ $cm2 GetCamera ] [ $cm GetCamera ] $factor 
		$ren Render
		update idletasks
		if { $dosave == 1 } {
		    $this ManualGrabNextFrame
		}
		after $del
	    }
	}
    }
    if { $dosave == 1 } {
	$this SaveAnimationFrame end
	$this ShowWindow
    }
    $ren CameraModifiedCallback
}


itcl::body pxitclanimationgadget::AddSpin { angle } {

    set spinning 1
    $this UpdateFromViewer
    set ren [ $vtk_viewer GetRenderer 3 ]

    set md $thisparam($this,spinazimuth)



    set angle [ expr abs($angle) ]
    set nsteps [ expr round(360.0/$angle) ]
    
    if { $thisparam($this,spinpositive) == 0 } {
	set angle [ expr -1.0*$angle]
    }

    after 200
    for { set i 1 } { $i <= $nsteps } { incr i } {
	$ren Rotate $angle $md
	update idletasks
	$this GrabFromViewer
	after 200
    }
    set spinning 0
}
