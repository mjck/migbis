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

package provide pxitclmanualsurfacetransformation 1.0
package provide pxitclsurobj 1.0





# 	$Id: pxitclmanualsurfacetransformation.tcl,v 1.1 2002/06/11 01:21:41 papad Exp papad $	

package require  labelframe 1.0
package require  pxtclutil  1.0
package require  pxvtable   1.0
package require  vtkpxcontrib 1.1
package require  pxitcloverlaytool 1.0
package require  pxitclbasecontrol 1.0

# ------------------------------------------------------------------------------------------
#
# pxitclmanualsurfacetransformation control
#
# ------------------------------------------------------------------------------------------

itcl::class pxitclmanualsurfacetransformation {

    inherit pxitclbasecontrol

    private   common   thisparam
    protected variable initialized     0
    public variable input_surface   0
    public variable output_surface  0
    public variable overlaytool     0
    protected variable transformation  0
    public variable shape_model     0
    public variable shape_model_weights  0
    public variable updatepolydata       0
    protected variable parameter_names   0
    protected variable parameter_values  0
    public    variable energy_image      0
    public    variable reference_image      0
    protected variable energy_level     "no_image"
    public    variable segmentor  0

    constructor { par over } {
	::pxitclbasecontrol::constructor $par
    } { 
	set overlaytool $over
    }

 
    public    method Initialize  { basewidget args } 
    public    method InitializeVariables  { } 
    public    method IdentityTransformation  { } 
    public    method CreateSurface { }
    public    method CopySurfaceFromClipboard { }
    public    method PasteSurfaceFromClipboard { }
    public    method ToggleShowHide { }
    public    method UpdateStatus { }
    public    method GenerateTransformation { }
    public    method WarpSurface            { }
    public    method GrabImage { }
    public    method ComputeEnergyFunction  { }

    public    method SendTransformationToOverlayTool { }
    public    method GetTransformationFromOverlayTool { }

    
}

# ------------------------------------------------------------------------------------------
itcl::body pxitclmanualsurfacetransformation::Initialize { basewidg args } {

    if { $initialized == 1 } { return $basewidget }

    set basewidget [ toplevel $basewidg ] 
    wm withdraw $basewidget
    wm geometry $basewidget 450x700
    wm title $basewidget "Manual Surface Warp"

    set w [ frame $basewidget.bottom ]
    pack $w -side bottom -expand f -fill x -pady 2
    
    eval "button $w.1 -text Close -command { $this HideWindow } "
    pack $w.1 -side right

    eval "button $w.r -text \"Reset Xform\" -command { $this IdentityTransformation  }"
    eval "button $w.g -text \"Get Xform\" -command { $this GetTransformationFromOverlayTool  }"
    eval "button $w.s -text \"Store Xform\" -command { $this SendTransformationToOverlayTool  }"
    
    pack $w.r $w.g $w.s -side left -padx 2

    set thisparam($this,surfacestatus) ""
    set top [ frame $basewidget.top ]
    set top2 [ frame $basewidget.top2 ]
    pack $basewidget.top $top2 -side top -expand f -fill x -pady 2

    label $top.l -text "Surface:"
    entry $top.o -textvariable [ itcl::scope thisparam($this,surfacestatus) ] -relief raised -state disabled -width 45 -bg black -fg white
    eval "button $top2.cr -text \"Create \" -command { $this CreateSurface  }"
    eval "button $top2.c -text \"Copy \" -command { $this CopySurfaceFromClipboard  }"
    eval "button $top2.p -text \"Paste\" -command { $this PasteSurfaceFromClipboard  }"
    eval "button $top2.w -text \"Show/Hide\" -command { $this ToggleShowHide  }"
    eval "button $top2.wi -text \"Get Image\" -command { $this GrabImage  }"
    pack $top.l $top.o -side left -expand f -fill x
    pack $top2.p $top2.c $top2.cr $top2.w $top2.wi -side right 


    set middle [ frame $basewidget.sf  ]
    pack $basewidget.sf -side top -expand t -fill both -padx 4 -pady 4 

    set parameter_values [ list tx ty tz rx ry rz sx sy sz m1 m2 m3 m4 m5 ]
    set parameter_names  [ list TX TY TZ RX RY RZ SX SY SZ Mode1 Mode2 Mode3 Mode4 Mode5 ]
    set lower [ list  -64 -64 -64 -90 -90 -90 50 50 50 -2.0 -2.0 -2.0 -2.0 -2.0 ]
    set upper [ list  64 64 64 90 90 90 200 200 200 2.0 2.0 2.0 2.0 2.0 ]
    set initial [ list 0 0 0 0 0 0 100 100 100 0 0 0 0 0]
    set res     [ list 1 1 1 1 1 1   2   2   2 0.1 0.1 0.1 0.1 0.1 ]
    
    for { set i 0 } { $i < [ llength $parameter_values ] } { incr i } {
	set bf [ frame $middle.f$i ]

	pack $bf -side top -expand f -fill x
	label  $bf.1 -text [ lindex $parameter_names $i ]
	set   val [ lindex $parameter_values $i ]
	set thisparam($this,value_$val) [ lindex $initial $i ]
	set thisparam($this,guiscale_$val) [  scale $bf.2 -orient horizontal  -from [ lindex $lower $i ] -to [ lindex $upper $i ] -resolution [ lindex $res $i ] -showvalue 1 -length 250 -variable [ itcl::scope thisparam($this,value_$val) ]]
	pack $bf.1 -side left -expand f
	pack $bf.2 -side left -expand t -fill x
	eval "bind $bf.2 <ButtonRelease> { $this GenerateTransformation }"
    }

    $this InitializeVariables
    $this UpdateStatus

    SetTitle "Manual Surface Transformation"

    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget

}

# ------------------------------------------------------------------------------------------     
itcl::body pxitclmanualsurfacetransformation::InitializeVariables { } {

    set transformation [ vtkpxComboTransform [ pxvtable::vnewobj ]]
    $transformation SetNonLinearFirst 1
    
    set shape_model [ vtkpxShapePriorDistance [ pxvtable::vnewobj ]]
    $shape_model Load "/home/xenios/vtkpxcontrib/bioimagesuite/images/kidney_shape.model"
    
    set shape_model_weights [ vtkFloatArray [ pxvtable::vnewobj ]]
    $shape_model_weights SetNumberOfTuples 100
    $shape_model_weights FillComponent 0 0 

    set input_surface  [ vtkPolyData [ pxvtable::vnewobj ]]
    set output_surface [ vtkPolyData [ pxvtable::vnewobj ]]

    set source [ vtkSphereSource  [ pxvtable::vnewobj ]]
    $source Update
    $input_surface   ShallowCopy [ $source GetOutput ]
    $output_surface  ShallowCopy [ $source GetOutput ]
    $source Delete

    set orthoviewer [ $parent GetViewer ]

    for { set i 0 } { $i <= 3 } { incr i } {
	set map [ vtkPolyDataMapper [ pxvtable::vnewobj ]]
	$map SetInput $output_surface

	set thisparam($this,act$i) [ vtkActor [ pxvtable::vnewobj ]]
	$thisparam($this,act$i) SetVisibility 0
	$thisparam($this,act$i) SetMapper $map
	$map Delete

	set ren [ [ $orthoviewer GetRenderer $i ] GetRenderer ] 
	$ren AddActor $thisparam($this,act$i)
	
	if { $i != 3 } {
	    
	    set prop [ $thisparam($this,act$i) GetProperty ]
	    $prop SetRepresentationToWireframe
	    $prop SetLineWidth 2.0
	    $prop SetAmbient  1.0
	    $prop SetDiffuse 0.0
	    $prop SetSpecular 0.0
	}
    }

    set energy_image [ vtkImageData [ pxvtable::vnewobj ] ]
    set reference_image [ vtkImageData [ pxvtable::vnewobj ] ]
    set segmentor [ vtkpxPriorShapeModelSegmentation [ pxvtable::vnewobj ] ]
}
# ------------------------------------------------------------------------------------------     
itcl::body pxitclmanualsurfacetransformation::CreateSurface { } {


    set image_ref [ [ $overlaytool GetImage ] GetImage ]

    set dim [ $image_ref GetDimensions ]
    set ori [ $image_ref GetOrigin ]
    set spa [ $image_ref GetSpacing ]

    for { set i 0 } { $i <=2 } { incr i } {
	set pos($i) [ expr [ lindex $ori $i ] + 0.5 * [ lindex $dim $i ] * [ lindex $spa $i ]]
	set rad($i) [ expr 0.25 * [ lindex $dim $i ] * [ lindex $spa $i ]]
    }
    

    set source [ vtkSphereSource  [ pxvtable::vnewobj ]]

    $source SetCenter $pos(0) $pos(1) $pos(2)
    $source SetRadius $rad(0)
    $source SetThetaResolution 20
    $source SetPhiResolution 20
    $source Update



    $input_surface   ShallowCopy [ $source GetOutput ]
    $source Delete

    $this WarpSurface

    
    

    
}
# ------------------------------------------------------------------------------------------     
itcl::body pxitclmanualsurfacetransformation::IdentityTransformation { } {

    for { set i 0 } { $i < [ llength $parameter_values ] } { incr i } {
	set   val [ lindex $parameter_values $i ]
	if { $i < 6 || $i >8 } {
	    set thisparam($this,value_$val) 0
	} else {
	    set thisparam($this,value_$val) 100
	}
    }

    $this GenerateTransformation
}
# ------------------------------------------------------------------------------------------     
itcl::body pxitclmanualsurfacetransformation::CopySurfaceFromClipboard { } {
    
    set cntrl [ [ $parent GetPolyDataControl ] GetPolyDataControl ]
    $cntrl SetClipboardSurface  $output_surface "ManualXform.vtk"
    ::pxtclutil::Info "Output surface is stored on the clipboard of the surface control\n(You must paste this into a surface before it becomes available)"
}

itcl::body pxitclmanualsurfacetransformation::PasteSurfaceFromClipboard { } {
    set cntrl [ [ $parent GetPolyDataControl ] GetPolyDataControl ]
    set sur [ $cntrl GetClipboardSurface ]
    catch { $input_surface ShallowCopy $sur }
    $this WarpSurface
}
# ------------------------------------------------------------------------------------------     
itcl::body pxitclmanualsurfacetransformation::GenerateTransformation { } {

    set pnamelist [ list x y z ]
    set offsetlist [ list 0 3 6 ]
    set image_ref [ [ $overlaytool GetImage ] GetImage ]
    set spa [ $image_ref GetSpacing ]


    for { set i 0 } { $i <=2 } { incr i } {
	
	set pname [ lindex $pnamelist $i ]
	
	set   val [ lindex $parameter_values [ expr 6+$i ] ]
	set scale($pname) $thisparam($this,value_$val)

	if { $scale($pname) < 10 } { 
	    set scale($pname) 10 
	} elseif { $scale($pname) > 1000.0 } {
	    set scale($pname) 1000.0
	}
	
	set   val [ lindex $parameter_values $i ]

	set dshift($pname) [ expr $thisparam($this,value_$val) * [ lindex $spa $i ] ]
	if { $dshift($pname) < -2000 } { 
	    set dshift($pname) 2000
	} elseif { $dshift($pname) > 2000.0 } {
	    set dshift($pname) 2000.0
	}
				     
	set   val [ lindex $parameter_values [ expr 3+$i ] ]
	set drotate($pname) $thisparam($this,value_$val)
	if { $drotate($pname) < -360 } { 
	    set drotate($pname) 360
	} elseif { $drotate($pname) > 360.0 } {
	    set drotate($pname) 360.0
	}

    }

    # On to non-linear part
    for { set i 9 } { $i < 14 } { incr i } {
	set   val [ lindex $parameter_values $i ]
	puts stdout "$val , $thisparam($this,value_$val)"
	set j [ expr $i -9 ]
	$shape_model_weights SetComponent $j 0  $thisparam($this,value_$val)
	puts stdout "Weights $j = [ $shape_model_weights GetComponent $j 0 ]"
    }
    
    [ $transformation  GetGridTransform ] InitializeDisplacementGrid  [ [  $shape_model  GetAverageTransform ] GetGridTransform ] 
    $shape_model GenerateTransformation  $transformation $shape_model_weights

    # Next Linear Part
    set image_ref [ $overlaytool GetImage ]
    set image_trn [ $overlaytool GetFunctionalImage ]

    set xform [ vtkpxLinearTransform [ pxvtable::vnewobj ]]
    $xform ManualSetParameters $dshift(x) $dshift(y) $dshift(z) $drotate(x) $drotate(y) $drotate(z) $scale(x) $scale(y) $scale(z)  [ $image_ref GetImage ] [ $image_ref GetOrientation ] [  $image_trn GetImage ] [ $image_trn GetOrientation ] [ $transformation GetLinearTransform ]
    $xform Delete

    $shape_model DebugOn
    set d  [ $shape_model ComputeDistance $transformation ]
    $shape_model DebugOff
    puts stdout "Distance to Model =  $d"

    WarpSurface
}
# ------------------------------------------------------------------------------------------     
itcl::body pxitclmanualsurfacetransformation::UpdateStatus { } {

    set thisparam($this,surfacestatus) "points = [ $output_surface GetNumberOfPoints ] visible = [ $thisparam($this,act0) GetVisibility ] $energy_level"
}

itcl::body pxitclmanualsurfacetransformation::WarpSurface { } {

    $transformation Update
    puts stdout "Saving in sent.grd ( [ $transformation Save sent.grd ] )"
    set filter [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ]]
    $filter SetInput $input_surface
    $filter SetTransform  $transformation 
    $filter Update
    $output_surface ShallowCopy [ $filter GetOutput ]
    $output_surface  Update

    puts stdout "Point 1 [ $input_surface GetPoint 1 ] __ [ $output_surface GetPoint 1 ]"
    set a [ $input_surface GetPoint 1 ]



    set orthoviewer [ $parent GetViewer ]
    $orthoviewer UpdateDisplay 

    $this ComputeEnergyFunction 

    UpdateStatus
}
# ------------------------------------------------------------------------------------------     
itcl::body pxitclmanualsurfacetransformation::SendTransformationToOverlayTool { } {


    set tr [ vtkTransform [ pxvtable::vnewobj ]]
    [ $tr GetMatrix ] DeepCopy [ [ $transformation GetLinearTransform ] GetMatrix ]
    $tr Inverse
    $overlaytool SetTransformation  $transformation "manual"
}
# ------------------------------------------------------------------------------------------     
itcl::body pxitclmanualsurfacetransformation::ToggleShowHide { } {

    for { set i 0 } { $i <= 3 } { incr i } {
	set act $thisparam($this,act$i)
	$act SetVisibility [ expr 1 - [ $act GetVisibility ] ]
    }
    $this UpdateStatus
    set orthoviewer [ $parent GetViewer ]
    $orthoviewer UpdateDisplay 

}
# ------------------------------------------------------------------------------------------     
itcl::body pxitclmanualsurfacetransformation::GetTransformationFromOverlayTool { } {
    

    set image_ref [ $overlaytool GetImage ]
    set image_trn [ $overlaytool GetFunctionalImage ]
    set in_xform  [ $overlaytool GetTransformation ]
    set spa [ [ $image_ref GetImage ] GetSpacing ]
    
    set xform [ vtkpxLinearTransform [ pxvtable::vnewobj ]]
    $xform ExtractParameters $in_xform 9 [ $image_ref GetImage ] [ $image_ref GetOrientation ] [  $image_trn GetImage ] [ $image_trn GetOrientation ]
    for { set k 9 } { $k < 15 } { incr k } {
	$xform Put $k 0
    }

    for { set i 0 } { $i < [ llength $parameter_values ] } { incr i } {
	set   val [ lindex $parameter_values $i ]
	if { $i > 2 } {
	    set  thisparam($this,value_$val)  [ $xform Get $i ]
	} else {
	    set  thisparam($this,value_$val)  [ expr  ([ $xform Get $i ]) / ( [ lindex $spa $i ]) ]
	}
    }

    $xform Delete
    GenerateTransformation
}
# ------------------------------------------------------------------------------------------     
itcl::body pxitclmanualsurfacetransformation::GrabImage { } {

    set img [ [ $parent GetDisplayedImage ] GetImage ]
    $energy_image ShallowCopy $img

    $reference_image ShallowCopy [  [ $overlaytool GetImage ] GetImage ]

    ComputeEnergyFunction
    UpdateStatus
}
# ------------------------------------------------------------------------------------------     
itcl::body pxitclmanualsurfacetransformation::ComputeEnergyFunction  { } {

    set dim [ $energy_image GetDimensions ]
    if { [ lindex $dim 0 ] < 2 } {
	set energy_level "no_image"
    } else {
	set val [ $segmentor Evaluate $output_surface $energy_image ]
	set energy_level "dims : $dim, val=$val"
    }

}

# ------------------------------------------------------------------------------------------     

