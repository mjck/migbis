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

package provide pxitclmanualtransformation 1.0

# 	$Id: pxitclmanualtransformation.tcl,v 1.8 2004/01/03 19:24:02 xenios Exp xenios $	





package require  labelframe 1.0
package require  pxtclutil  1.0
package require  pxvtable   1.0
package require  pxitclbasecontrol 1.0
package require  pxitcltransform 1.0
package require  pxitclsurface 1.0

itcl::class pxitclmanualtransformation {

    inherit pxitclbasecontrol

    protected variable original_surface 0
    protected variable transformed_surface 0
    protected variable back_transformed_surface 0
    protected variable vtk_viewer    0
    protected variable vtk_viewer2    0
    protected variable parent_transform_control 0
    protected variable spinlist ""
    
    public    variable transformation 0
    public    variable surface_control 0
    private   common thisparam

    constructor { par args } {
	pxitclbasecontrol::constructor $par
    } {
	if { [ [ $parent GetViewer ] IsA vtkpxGUIOrthogonalViewer ] != 1}  {
	    puts stderr "Must Have Orthogonal Viewer as parent to manual transformation"
	    pxtkexit
	}
	
	set vtk_viewer          [ $parent GetViewer ]
	if { [ llength $args ] > 0  } {
	    set par2 [ lindex $args 0 ]
	    if { [ [ $par2 GetViewer ] IsA vtkpxGUIOrthogonalViewer ] != 1}  {
		puts stderr "Must Have Orthogonal Viewer as parent to manual transformation"
		pxtkexit
	    }
	    set vtk_viewer2 [ $par2 GetViewer ]
	} else {
	    set vtk_viewer2 $vtk_viewer
	}

	if { [ llength $args ] > 1  } {
	    set parent_transform_control [ lindex $args 1 ]
	}


	InitializeManualTransformation  
    }

    # ------------ Methods --------------------
    
    private  method InitializeManualTransformation  { } 
    public   method Initialize { inpwidg args }

    # Transformation Related
    # ----------------------
    public    method GetTransformation { } 
    public    method TransformSurface {  }

    # Manual Transformation Related
    # -----------------------------
    protected method CreateSpinBox { par lab var rng { rng2 -1 } }
    public    method ChangeSpinBoxIncrement { } 
    public   method CreateManualTransformGUI { parent}
    public   method ClearManualTransformGUI { }
    public   method SetManualTransform { }


    # GUI Stuff
    # ---------
    public    method CreateTemplateControl { parent } 
    public    method InitializeSurfaceActors { }

    # Callbacks
    #------------
    public    method Update { mode } 

    # More Stuff
    # ------------
    public    method UpdateParentTransformControl { }
    public    method AutoCreateSurface { } 
}



itcl::body pxitclmanualtransformation::InitializeManualTransformation  { } {
	
    global pxtcl_pref_array

    # ----------------------------
    # Transform Stuff
    # ----------------------------
    set transformation [ vtkTransform [ pxvtable::vnewobj ]]
    set thisparam($this,show_surfaces) 0
    set thisparam($this,numclasses) 3
    set thisparam($this,minclass) 1
    set thisparam($this,maxdim) 32
    set thisparam($this,sigma) 2.0

    set thisparam($this,show_transformedsurface) 0
    set thisparam($this,transformmode) 1
    set thisparam($this,actors_initialized) 0
}

# ---------------------------------------------------------------------------------------
# Initialize
# ---------------------------------------------------------------------------------------

itcl::body pxitclmanualtransformation::Initialize { inpwidg args } {

    if { $fullyinitialized == 1 } { return $basewidget }

    set basewidget $inpwidg
    toplevel $basewidget ; wm withdraw $basewidget
    wm geometry $basewidget 600x250

    wm title $basewidget "Manual Registration Tool"

    CreateTemplateControl $basewidget
    

    $this InitializeSurfaceActors
    set fullyinitialized 1
    return $basewidget
}



# -------------------------------------------------------------
#  Set Surface Stuff -- insert here
# -------------------------------------------------------------
itcl::body pxitclmanualtransformation::InitializeSurfaceActors { } {

    set original_surface [ vtkPolyData [ pxvtable::vnewobj ]]
    set transformed_surface [ vtkPolyData [ pxvtable::vnewobj ]]
    set back_transformed_surface [ vtkPolyData [ pxvtable::vnewobj ]]

    for { set sur 0 } { $sur <= 1 } { incr sur } {
	if { $sur ==0 } {
	    set current $back_transformed_surface
	} else {
	    set current $transformed_surface
	}

	$current ShallowCopy  [ [  $surface_control  GetObject ]  GetSurface ]

	for { set i 0 } { $i <= 3 } { incr i } {
	    set map [ vtkPolyDataMapper [ pxvtable::vnewobj ]]
	    $map SetInput $current

	    set thisparam($this,act${sur}_${i}) [ vtkActor [ pxvtable::vnewobj ]]
	    set actor $thisparam($this,act${sur}_${i})
	    $actor SetVisibility 0
	    $actor SetMapper $map
	    $map Delete

	    if { $sur == 0 } {
		set ren [ [ $vtk_viewer GetRenderer $i ] GetRenderer ] 
	    } else {
		set ren [ [ $vtk_viewer2 GetRenderer $i ] GetRenderer ] 
	    }
	    $ren AddActor $actor
	    set prop [ $actor GetProperty ]
	    
	    if { $sur ==0 } {
		$prop SetColor 1 0 0
		
		
	    } else {
		$prop SetColor 1 0.5 0
	    }

	    if { $i != 3 } {
		
		$prop SetRepresentationToWireframe
		$prop SetLineWidth 3.0
		$prop SetAmbient  1.0
		$prop SetDiffuse 0.0
		$prop SetSpecular 0.0

	    }
	}
    }
}

# ----------------------------------------------------------------------------
itcl::body pxitclmanualtransformation::Update { mode } {

    if { $mode == "full" } {
        TransformSurface
    }

    set visflag $thisparam($this,show_surfaces)    
    for { set sur 0 } { $sur <= 1 } { incr sur } {
	for { set i 0 } { $i <= 3 } { incr i } {
	    set actor $thisparam($this,act${sur}_${i})
	    $actor SetVisibility $visflag
	}
    }

    $vtk_viewer2 UpdateDisplay 
    if { $vtk_viewer != $vtk_viewer2 } {
	$vtk_viewer UpdateDisplay
    }

}

# --------------------------------------------------------
#    Create Functional Image Control 
# --------------------------------------------------------

itcl::body pxitclmanualtransformation::CreateTemplateControl { parent } {

    set base  [ frame $parent.top ]
    set xform [ frame $parent.bot ]
    frame $parent.mid -bg black -height 2
    pack $base $parent.mid $xform -side top -expand false -fill x

    frame $base.3; pack $base.3 -side bottom -expand false -fill x
    frame $base.2; pack $base.2 -side bottom -expand false -fill x
    frame $base.1; pack $base.1 -side top  -expand true -fill x

    set surface_control [ [ pxitclsurfaceGUI \#auto  ] GetThisPointer ]
    $surface_control configure -description "Edge Surface"
    $surface_control InitializeLite $base.1.1
    set cl [ $surface_control cget -clearbutton ]
    pack forget $cl
    pack $base.1.1 -side top -expand false -fill both
    

    eval "checkbutton $base.2.c1 -variable [ itcl::scope thisparam($this,show_surfaces) ] -text \"Show Surfaces\" -command { $this Update full }"
    eval "checkbutton $base.2.c2 -variable [ itcl::scope thisparam($this,transformmode) ] -text \"Template in Transform Space\" -command { $this Update full }"
    pack $base.2.c1 $base.2.c2  -side left -expand f -padx 5

    
    
    label $base.3.1 -text "NumClasses:"
    eval "spinbox $base.3.2 -from 2 -to 6 -width 2 -format %.0f -increment 1 -textvariable [ itcl::scope thisparam($this,numclasses) ]"
    label $base.3.3 -text "Thr:"
    eval "spinbox $base.3.4 -from 1 -to 5 -width 2 -format %.0f -increment 1 -textvariable [ itcl::scope thisparam($this,minclass) ]"
    label $base.3.5 -text "Sigma:"
    eval "spinbox $base.3.6 -from 1.0 -to 4.0 -width 4 -increment 0.5 -format %.1f -textvariable [ itcl::scope thisparam($this,sigma) ]"

    label $base.3.61 -text "Res:"
    eval "tk_optionMenu $base.3.62  [ itcl::scope thisparam($this,maxdim) ]  16 32 48 64 96 128 "
    
    eval "button $base.3.7 -text \"Auto Create Surface\" -command { $this AutoCreateSurface }"
    pack $base.3.1 $base.3.2 $base.3.3 $base.3.4 $base.3.5 $base.3.6 $base.3.61 $base.3.62 $base.3.7 -side left -expand f -padx 1


    CreateManualTransformGUI $xform

}

# -------------------------------------------------------------
# Transform Operations
# -------------------------------------------------------------

itcl::body pxitclmanualtransformation::GetTransformation { } {

    return $transformation
}

# -----------------------------------------------------------------------
#   Manual Transform GUI
# -----------------------------------------------------------------------
itcl::body pxitclmanualtransformation::ClearManualTransformGUI { } {

    for { set i 0 } { $i <=2 } { incr i } {

	if { $i == 0 } { 
	    set pname "x"
	} elseif { $i == 1 } {
	    set pname "y"
	} else {
	    set pname "z"
	}
	
	set thisparam($this,rotate$pname) 0
	set thisparam($this,shift$pname) 0
    }
    set thisparam($this,scale) 100
}

# ----------------------------------------------------------------------
itcl::body pxitclmanualtransformation::SetManualTransform { } {
    
    set image_ref [ $vtk_viewer GetImage ]
    set image_trn [ $vtk_viewer2 GetImage ]

    set dim1 0; set dim2 0
    set ori1 0; set ori2 0
    catch {
	set dim1 [  $image_ref GetDimensions ]
	set dim2 [  $image_trn GetDimensions ]
	set ori1 [  $vtk_viewer GetOrientation ]
	set ori2 [  $vtk_viewer2 GetOrientation ]
    }

    if { $dim1 == 0 || $dim2 ==0 } {
	return
    }
    

    for { set i 0 } { $i <=2 } { incr i } {
	
	if { $i == 0 } { 
	    set pname "x"
	} elseif { $i == 1 } {
	    set pname "y"
	} else {
	    set pname "z"
	}

	set dshift($pname) $thisparam($this,shift$pname)
	if { $dshift($pname) < -2000 } { 
	    set dshift($pname) 2000
	} elseif { $dshift($pname) > 2000.0 } {
	    set dshift($pname) 2000.0
	}
	
	set drotate($pname) $thisparam($this,rotate$pname)
	if { $drotate($pname) < -180 } { 
	    set drotate($pname) 180
	} elseif { $drotate($pname) > 180.0 } {
	    set drotate($pname) 180.0
	}
	
    }

    if { $thisparam($this,scale) < 25 } {
	set thisparam($this,scale) 25
    } elseif { $thisparam($this,scale) > 400 } {
	set thisparam($this,scale) 400
    }


    $transformation Identity
    set lxform [ vtkpxLinearTransform [ pxvtable::vnewobj ]]
    $lxform ManualSetParameters $dshift(x) $dshift(y) $dshift(z) $drotate(x) $drotate(y) $drotate(z) $thisparam($this,scale) $thisparam($this,scale) $thisparam($this,scale) $image_ref $ori1 $image_trn $ori2 $transformation
    $lxform Delete

    set thisparam($this,show_surfaces) 1
    Update full
}

# --------------------------------------------------------------------
itcl::body pxitclmanualtransformation::ChangeSpinBoxIncrement { } {

    for { set i 0 } { $i < [ llength $spinlist ] } { incr i } {
	set w [ lindex $spinlist $i ]
	$w configure -increment $thisparam($this,spinincr)
    }

}
# --------------------------------------------------------------------
itcl::body pxitclmanualtransformation::CreateSpinBox { par lab var rng {rng2 -1 } } {

    if { $rng2 == -1 } {
	set rng2 [ expr -1.0* $rng ]
    }
    set minval $rng2
    set maxval $rng

    label ${par}1 -text $lab
    eval "spinbox ${par}2 -from $minval -to $maxval -increment 5.0  -width 4 -format %.0f -textvariable [ itcl::scope  thisparam($this,$var) ] -command { $this SetManualTransform }"
    pack ${par}1 ${par}2 -side left -padx 1
    return ${par}2
}

itcl::body pxitclmanualtransformation::CreateManualTransformGUI { parentw  } {


    set f2 [ frame $parentw.bot] 
    pack $f2 -side bottom -expand false -fill x

    set f [ frame $parentw.top ] ; pack $f -side top -expand false -fill x

    set index 0
    lappend spinlist [ $this CreateSpinBox $f.$index tx shiftx 100]; incr index
    lappend spinlist [ $this CreateSpinBox $f.$index ty shifty 100; ]; incr index
    lappend spinlist [ $this CreateSpinBox $f.$index tz shiftz 100; ]; incr index
    lappend spinlist [ $this CreateSpinBox $f.$index rx rotatex 180; ]; incr index
    lappend spinlist [ $this CreateSpinBox $f.$index ry rotatey 180; ]; incr index
    lappend spinlist [ $this CreateSpinBox $f.$index rz rotatez 180; ]; incr index
    lappend spinlist [ $this CreateSpinBox $f.$index sc scale 200 50.0; ]; incr index

    label $f2.a -text "Inc:"
    pack $f2.a -side left; incr index

    set thisparam($this,spinincr) 5
    eval "tk_optionMenu $f2.b  [ itcl::scope thisparam($this,spinincr) ] 1 3 5 10 20 "
    update idletasks

    for { set t 0 } { $t < 5 } { incr t } {
	eval "$f2.b.menu entryconfigure $t -command { $this ChangeSpinBoxIncrement }"
    }
    pack $f2.b -side left -padx 2

    if { $parent_transform_control != 0 } {
	eval "button $f2.3 -text \"Update Main Application\" -command { $this UpdateParentTransformControl }"
	pack $f2.3 -side right -padx 20 -pady 5
    }
    
    eval "button $f2.1 -text \"Set Xformation\" -command { $this SetManualTransform }"
    eval "button $f2.2 -text \"Clear Values\" -command { $this ClearManualTransformGUI; $this SetManualTransform }"
    pack $f2.1 $f2.2 -side left 
    
    $this ClearManualTransformGUI
}


itcl::body pxitclmanualtransformation::TransformSurface { } {

    WatchOn
    set current_transform $transformation

    $original_surface ShallowCopy  [ [  $surface_control  GetObject ]  GetSurface ]

    set filter [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ]]
    $filter SetInput $original_surface

    if { $thisparam($this,transformmode) == 0 } {
	$filter SetTransform  $current_transform 
	$filter Update
	$transformed_surface ShallowCopy [ $filter GetOutput ]
	$back_transformed_surface ShallowCopy $original_surface
    } else {
	$filter SetTransform  [ $current_transform  GetLinearInverse ]
	$filter Update
	$back_transformed_surface ShallowCopy [ $filter GetOutput ]
	$transformed_surface ShallowCopy $original_surface
    }


    $filter Delete

    $back_transformed_surface  Update
    $transformed_surface  Update

    WatchOff
    return 1
}


itcl::body pxitclmanualtransformation::UpdateParentTransformControl { } {

    $this SetManualTransform
    $parent_transform_control SetTransformation $transformation "add"
    $parent_transform_control ShowWindow Transformations
}

itcl::body pxitclmanualtransformation::AutoCreateSurface { }  { 

    puts stderr "In auto create"

    set img [ $vtk_viewer GetImage ]
    if { $thisparam($this,transformmode) == 1 } {
	set img [ $vtk_viewer2 GetImage ]
    }


    set np 0
    catch { set np [ $img GetNumberOfPoints ] }
    if { $np < 10 } {
	return 0
    }
    puts stderr "Canny 0"

    # 1 Run Canny
    set segm [ vtkpxHistogramSegmentation [ pxvtable::vnewobj ]]
    $segm SetInput $img
    $segm SetNumberOfClasses $thisparam($this,numclasses)
    $segm SetIterations 20
    $segm SetNumberOfBins 256
    $segm SetConvergence 0.05
    $segm SetSigma 0.05
    $segm GenerateLabelsOn
    $segm SetOutlierDetection 0
    $segm SetPreprocessingMode 0 
    $segm SetDistributionModel 0
    $segm SetMaxSigmaRatio     0.2
    $segm Update

    if { $thisparam($this,minclass) >= $thisparam($this,numclasses) } {
	set thisparam($this,minclass) [ expr $thisparam($this,numclasses) -1 ]
    }

    #    puts stderr "Canny 0"
    set imgthr [ vtkImageThreshold [ pxvtable::vnewobj ]]
    $imgthr SetInput [ $segm GetOutput ]
    $imgthr ThresholdBetween $thisparam($this,minclass) $thisparam($this,numclasses)
    $imgthr SetInValue 100
    $imgthr SetOutValue 0
    $imgthr ReplaceInOn
    $imgthr ReplaceOutOn
    $imgthr Update
    #puts stderr "Canny 0.5"

    set smooth  [ vtkImageGaussianSmooth [ pxvtable::vnewobj ]  ]
    $smooth SetInput [ $imgthr GetOutput ]
    $smooth SetStandardDeviations $thisparam($this,sigma) $thisparam($this,sigma) $thisparam($this,sigma)
    $smooth Update
    #puts stderr "Canny 1"	

    set dim [ [ $smooth GetOutput ] GetDimensions ]
    set spa [ [ $smooth GetOutput ] GetSpacing ]
    set ori [ [ $smooth GetOutput ] GetOrigin ]

    for { set i 0 } { $i <=2 } { incr i } {
	set d  [ lindex $dim $i ]
	set sp [ lindex $spa $i ]
	set r  [ expr double($d)*double($sp) ]
	if { $d > $thisparam($this,maxdim) } {
	    set outsp($i) [ expr $r/double($thisparam($this,maxdim)) ]
	} else {
	    set outsp($i) $sp
	}
	set outdim($i) [ expr int($r/$sp)+2 ]
	set outori($i) [ expr [ lindex $ori $i ] - $outsp($i) ]
    }
    
    #    set resl [ vtkImageResample [ pxvtable::vnewobj ]]
    #    $resl SetAxisOutputSpacing 0 $outsp(0)
    #    $resl SetAxisOutputSpacing 1 $outsp(1)
    #    $resl SetAxisOutputSpacing 2 $outsp(2)

    set resl [ vtkImageReslice [ pxvtable::vnewobj ]]
    $resl SetOutputSpacing $outsp(0) $outsp(1) $outsp(2)
    $resl SetOutputOrigin $outori(0) $outori(1) $outori(2)
    $resl SetOutputExtent 0 $outdim(0) 0 $outdim(1) 0 $outdim(2)
    $resl SetBackgroundLevel 100
    $resl SetInterpolationModeToLinear
    

    $resl SetInput [ $smooth GetOutput ]
    $resl Update

    set dim [ [ $resl GetOutput ] GetDimensions ]
    set spa [ [ $resl GetOutput ] GetSpacing ]
    puts stderr "Dimensions at resl =$dim, $spa"

    set cont [ vtkContourFilter [ pxvtable::vnewobj ]]
    $cont SetInput [ $resl GetOutput ]
    $cont SetValue 0 50.0
    $cont ComputeScalarsOff
    $cont ComputeGradientsOff
    $cont ComputeNormalsOff
    $cont Update
    
    #puts stderr "Canny 2"



    [  $surface_control  GetObject ]  ShallowCopySurface [ $cont GetOutput ]
    $surface_control Update

    $resl Delete
    $cont Delete
    $imgthr Delete
    $smooth Delete
    $segm Delete
    WatchOff
    puts stderr "Canny 3"
    $this SetManualTransform
}

