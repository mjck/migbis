
#!/bin/sh
# the next line restarts using wish \
    exec vtk "$0" "$@"

#BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
#BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
#BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
#BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
#BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
#BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


# -----------------------
# Dependencies and path
# -----------------------
lappend auto_path [ file dirname [ info script ] ]
lappend auto_path [ file join [file join [ file dirname [ info script ]] ".."] base ]
lappend auto_path [ file join [file join [ file dirname [ info script ]] ".."] main ]

package provide mpjguisphere 1.0

package require mpjguiobject 1.0
package require  vtkinteraction

itcl::class mpjguisphere {

    inherit mpjguiobject

    private variable iren 0          ; ## vtkRenderWindowInteractor
    private variable renderer 0      ; ## vtkRenderer
    private variable renwin          ; ## vtkRenderWindow
    private variable spheresource 0  ; ## vtkSphereSource
    private variable spheremapper 0  ; ## vtkPolyDataMapper
    private variable sphereactor 0   ; ## vtkSphereActor
    private variable wiremapper 0    ; ## vtkPolyDataMapper
    private variable wireactor 0     ; ## vtkSphereActor - wireframe
    
    private variable preview_widget 0; ## vtkTkRenderWidget
    
    private variable resolution 10
    private variable radius 1.0
    private variable numpoints 12
    private variable width 100
    private variable height 100

    private variable filename "sphere.txt"
    
    private variable menubase 0
    private variable buttonbox 0
    private variable propertybox 0
    private variable preview 0
    
    ## constructor
    constructor { args } { 
	mpjguiobject::constructor $args
    } {
	
	set spheresource [ vtkSphereSource [ pxvtable::vnewobj ] ]
	$spheresource SetThetaResolution $resolution
	$spheresource SetPhiResolution $resolution
	$spheresource SetRadius 1.0
		
	## sphere
	set spheremapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
	$spheremapper SetInput [ $spheresource GetOutput ] 
	
	## sphere actor
	set sphereactor [ vtkActor [ pxvtable::vnewobj ] ]
	$sphereactor SetMapper $spheremapper
	
	## sphere wireframe
	set wiremapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
	$wiremapper SetInput [ $spheresource GetOutput ] 
	
	set wireactor [ vtkActor [ pxvtable::vnewobj ] ]
	$wireactor SetMapper $wiremapper
	[ $wireactor GetProperty ] SetRepresentationToWireframe
	[ $wireactor GetProperty ] SetColor 0.5 0.5 0.5
	$wireactor SetScale 1.0125 1.0125 1.0125
    }
    
    ## initialization
    protected method InitializeWindow { parent } 
    
    public method Reset { }
    public method Update { }
    public method Save { }
    public method Resize { w h } { set width $w; set height $h }

    ## sphere properties
    public method SetSphereOn { }  { $sphereactor VisibilityOn }
    public method SetSphereOff { }  { $sphereactor VisibilityOff }
    public method SetSphereColor { red green blue } 
    public method SetSphereOpacity { opacity }

    public method SetResolution { res } { set resolution $res }
    public method GetResolution { } { return $resolution }
    public method GetNumberOfPoints { } { return $numpoints }

    ## wireframe properties
    public method SetWireframeOn { }  { $wireactor VisibilityOn }
    public method SetWireframeOff { }  { $wireactor VisibilityOff }
    public method SetWireframeColor { red green blue }
    public method SetWireframeOpacity { opacity }
    public method SetWireframeLineWidth { width }
    
    ## widgets
    public method GetButtonBoxWidget { } { return $buttonbox }
    public method GetPropertyWidget { } { return $propertybox }
    public method GetPreviewWidget { } { return $preview }

    ## actor retrieval
    public method GetSphereActor { } { return $sphereactor }
    public method GetWireframeActor { } { return $wireactor }

    ## import
    public method AddActor { actor } { $renderer AddActor $actor }
    public method AddActor2D { actor } { $renderer AddActor2D $actor }
    public method RemoveActor { actor } { $renderer RemoveActor $actor }

    ## private methods
    private method create_preview_frame { basewidget }
    private method create_property_box { basewidget }
    private method create_button_box { basewidget }
    private method create_pipeline { basewidget }
    private method spin_int { widget direction }
    private method save_sphere { fname }
    
    destructor {
	catch { 
	    $spheresource Delete
	    $spheremapper Delete
	    $wiremapper Delete
	    $sphereactor Delete
	    $wireactor Delete
	    $iren Delete
	    $renwin Delete
	    $renderer Delete	   
	}
    }
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::SetSphereColor { red green blue } { 
    [ $sphereactor GetProperty ] SetColor $red $green $blue
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::SetSphereOpacity { opacity } { 
    [ $sphereactor GetProperty ] SetOpacity $opacity
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::SetWireframeColor { red green blue } { 
    [ $wireactor GetProperty ] SetColor $red $green $blue
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::SetWireframeOpacity { opacity } {    
    [ $wireactor GetProperty ] SetOpacity $opacity
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::SetWireframeLineWidth { width } {
    [ $wireactor GetProperty ] SetLineWidth $width
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::Reset { } {
    
    set resolution 10

    Update
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::Update { } {
    
    ## update sphere source
    $spheresource SetThetaResolution $resolution
    $spheresource SetPhiResolution $resolution
    $spheresource Update
    
    set numpoints [ [ $spheresource GetOutput ] GetNumberOfPoints ]
    
    $renderer ResetCamera
    $renwin Render
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::Save { } {
    
    set fname [tk_getSaveFile -title "Save as" -initialfile $filename \
		   -filetypes {{"Text" {.txt}}} ]
    if { [ string length $fname ] < 1 } {
        return 0
    }
    
    save_sphere $fname
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::save_sphere { fname } {
 
    set points [ [ $spheresource GetOutput ] GetPoints ]
    set total [ $points GetNumberOfPoints ]
    
    set fileid [ open $fname w ]
    
    puts $fileid $total
    
    for { set i 0 } { $i < $total } { incr i } {
	set pt [ $points GetPoint $i ]
	puts $fileid "$pt"
    }

    close $fileid
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::spin_int { widget direction } {    
    
    set new_value [expr [$widget get] + $direction]
    
    if { $new_value > 20 } { set new_value 20 }
    if { $new_value < 1 } { set new_value 1 }
    
    $widget clear
    $widget insert 0 $new_value

    Update
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::create_button_box { basewidget } {

    set buttonbox [ iwidgets::buttonbox $basewidget.buttons -pady 5 ]
    
    $buttonbox add update -text "Reset" -command [itcl::code $this Reset ]
    $buttonbox add print -text "Save" -command [itcl::code $this Save ]    
    $buttonbox add dismiss -text "Dismiss" -command [itcl::code $this Hide ]
    
    pack $buttonbox -side top
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::create_property_box { basewidget } {

    set propertybox [ frame $basewidget.param ]
    pack $propertybox -side top -fill both -pady 2 -expand f

    set res $propertybox.res

    iwidgets::spinint $res \
	-labeltext "Resolution: " \
	-width 3 \
	-range { 1 20 } \
	-wrap no \
	-command [ itcl::code $this Update ] \
	-decrement [itcl::code $this spin_int $res -1] \
	-increment [itcl::code $this spin_int $res +1] \
   	-textvariable [ itcl::scope resolution ] 

    pack $propertybox.res -side left -padx 5

    $propertybox.res clear
    $propertybox.res insert 0 10
    
    label $propertybox.dummy1
    pack $propertybox.dummy1 -side left -padx 2

    label $propertybox.dummy2 -text "Number of points:"
    pack $propertybox.dummy2 -side left

    set points $propertybox.points
    label $points -textvariable [ itcl::scope numpoints ]
    pack $points -side left
}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::create_preview_frame { basewidget } {
    
    set preview [ iwidgets::labeledframe $basewidget.render \
		      -relief sunken -labeltext "Preview" -labelpos nw]
    pack $preview -side top -fill both -expand t
    
    set preview_widget [vtkTkRenderWidget [$preview childsite].renderwidget \
			    -width $width -height $height ]
    pack $preview_widget -side top -fill both -expand true


    set renwin [ $preview_widget GetRenderWindow ]
    $renwin AddRenderer $renderer

    eval "bind $preview_widget <Expose> { $renwin Render }"
    $renwin Render

    update idletasks



    ## create render window and interactor
    #    set iren [ vtkRenderWindowInteractor [ pxvtable::vnewobj ] ]
    #    $iren SetRenderWindow $renwin
    set evlist { ButtonPress-2 B2-Motion ButtonRelease-2 Shift-ButtonPress-2 Shift-B2-Motion Shift-ButtonRelease-2 Control-ButtonPress-2 Control-B2-Motion Control-ButtonRelease-2 }

    ::vtk::bind_tk_widget $preview_widget $renwin
    foreach ev $evlist { 
	set evl "<${ev}>"
	eval "bind $preview_widget $evl { }" 
    }


    
    #   set trackball [ vtkInteractorStyleTrackballCamera [ pxvtable::vnewobj ] ]
    #    [ $renwin GetInteractor ] SetInteractorStyle $trackball

}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::create_pipeline { basewidget } {    
    
    ## create renderer
    set renderer [ vtkRenderer [ pxvtable::vnewobj ] ]

    ## add actors
    $renderer AddActor $sphereactor
    $renderer AddActor $wireactor

    ## set renderer options
    #[ $renderer GetActiveCamera ] Azimuth -35
    #[ $renderer GetActiveCamera ] Elevation -35
    [ $renderer GetActiveCamera ] Zoom 1.4
    $renderer SetBackground 0.1 0.2 0.4

    ## add actors
    $renderer AddActor $sphereactor
    $renderer AddActor $wireactor

}

#-------------------------------------------------------------------------------------
itcl::body mpjguisphere::InitializeWindow { parent } {

    mpjguiobject::InitializeWindow $parent

    if { $parent == 0 } {
	SetTitle "mpjguisphere: $filename" 
    }

    ## set graphics pipeline
    create_pipeline $basewidget

    if { $parentwidget != 0 } {
	pack $basewidget -side left -fill both -expand t
    }

    ## construct interface
    create_preview_frame $basewidget
    create_property_box $basewidget
    create_button_box $basewidget

    $renwin Render
    
    if { $parentwidget == 0 } {
	eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    }
}

# ---------------------------------------------------------------------------
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    
    option add *textBackground seashell
    
    wm withdraw .
        
    set sph [ mpjguisphere \#auto 0 ]   ; ## create object as top-level
    $sph Initialize 0
    
    
    $sph Show ; ## map window
}

