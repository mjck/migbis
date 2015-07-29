 
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

package provide mpjguiicosahedron 1.0

package require mpjguiobject 1.0
package require vtkinteraction 

itcl::class mpjguiicosahedron {

    inherit mpjguiobject

    private variable iren 0          ; ## vtkRenderWindowInteractor
    private variable renderer 0      ; ## vtkRenderer
    private variable renwin          ; ## vtkRenderWindow
    private variable spheresource 0  ; ## vtkSphereSource
    private variable spheremapper 0  ; ## vtkPolyDataMapper
    private variable sphereactor 0   ; ## vtkSphereActor

    private variable xaxis 0
    private variable xaxismapper
    private variable xaxisactor
    private variable yaxis 0
    private variable yaxismapper
    private variable yaxisactor
    private variable zaxis 0
    private variable zaxismapper
    private variable zaxisactor

    private variable wiremapper 0    ; ## vtkPolyDataMapper
    private variable wireactor 0     ; ## vtkSphereActor - wireframe
        
    private variable scale 100.0

    private variable preview_widget 0; ## vtkTkRenderWidget
    
    private variable resolution 1
    private variable tiprad 8
    private variable tiplength 8
    private variable shaftrad 3

    private variable radius 1.0
    private variable numpoints 12
    private variable width 150
    private variable height 150

    private variable filename "icosahedron.txt"

    private variable menubase 0
    private variable buttonbox 0
    private variable propertybox 0
    private variable preview 0
    
    private variable showicosa 0
    private variable showicowire 0
    
    ## constructor
    constructor { args } { 
	mpjguiobject::constructor $args
    } {

	## x - axes
	set xaxis [ vtkArrowSource [ pxvtable::vnewobj ] ]	
	$xaxis SetTipLength [ expr $tiplength / $scale ]
        $xaxis SetTipRadius [ expr $tiprad / $scale ]
        $xaxis SetShaftRadius [ expr $shaftrad / $scale ]
        $xaxis SetTipResolution 100
        $xaxis SetShaftResolution 100
	
	set xaxismapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
	$xaxismapper SetInput [ $xaxis GetOutput ]

	set xaxisactor  [ vtkActor [ pxvtable::vnewobj ] ]
	$xaxisactor SetMapper $xaxismapper
	$xaxisactor SetScale 1.4 0.9 0.9
	[ $xaxisactor GetProperty ] SetColor 1.0 0.0 0.0

	## y - axes
	set yaxis [ vtkArrowSource [ pxvtable::vnewobj ] ]	
	$yaxis SetTipLength [ expr $tiplength / $scale ]
        $yaxis SetTipRadius [ expr $tiprad / $scale ]
        $yaxis SetShaftRadius [ expr $shaftrad / $scale ]
        $yaxis SetTipResolution 100
        $yaxis SetShaftResolution 100
	
	set yaxismapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
	$yaxismapper SetInput [ $yaxis GetOutput ]

	set yaxisactor  [ vtkActor [ pxvtable::vnewobj ] ]
	$yaxisactor SetMapper $yaxismapper
	$yaxisactor RotateZ 90.0
	$yaxisactor SetScale 1.4 0.9 0.9
	[ $yaxisactor GetProperty ] SetColor 0.0 1.0 0.0

	## z - axes
	set zaxis [ vtkArrowSource [ pxvtable::vnewobj ] ]	
	$zaxis SetTipLength [ expr $tiplength / $scale ]
        $zaxis SetTipRadius [ expr $tiprad / $scale ]
        $zaxis SetShaftRadius [ expr $shaftrad / $scale ]
        $zaxis SetTipResolution 100
        $zaxis SetShaftResolution 100
	
	set zaxismapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
	$zaxismapper SetInput [ $zaxis GetOutput ]

	set zaxisactor  [ vtkActor [ pxvtable::vnewobj ] ]
	$zaxisactor SetMapper $zaxismapper
	$zaxisactor RotateY 90.0
	$zaxisactor SetScale 1.4 0.9 0.9
	[ $zaxisactor GetProperty ] SetColor 0.0 0.0 1.0

	## sphere
	set spheresource [ vtkmpjIcosahedronSource [ pxvtable::vnewobj ] ]
	$spheresource SetDepth 1
	$spheresource AllowDuplicatePointsOff

	set spheremapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
	$spheremapper SetInput [ $spheresource GetOutput ] 

	set sphereactor [ vtkActor [ pxvtable::vnewobj ] ]
	[ $sphereactor GetProperty ] SetDiffuse 0.5
        $sphereactor SetScale 0.75 0.75 0.75
	$sphereactor SetMapper $spheremapper
	
	## sphere wireframe
	set wiremapper [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
	$wiremapper ScalarVisibilityOff
	$wiremapper SetResolveCoincidentTopologyToPolygonOffset
	$wiremapper SetInput [ $spheresource GetOutput ] 
	
	set wireactor [ vtkActor [ pxvtable::vnewobj ] ]
	$wireactor SetMapper $wiremapper
        $wireactor SetScale 0.75 0.75 0.75
	[ $wireactor GetProperty ] SetRepresentationToWireframe
	[ $wireactor GetProperty ] SetColor 0.5 0.5 0.5
	#$wireactor SetScale 1.0125 1.0125 1.0125	
    }
    
    ## initialization
    protected method InitializeWindow { parent } 
    
    public method Reset { }
    public method Update { }
    public method Save { }

    ## sphere properties
    public method SetSphereOn { }  { $sphereactor VisibilityOn }
    public method SetSphereOff { }  { $sphereactor VisibilityOff }
    public method SetSphereColor { red green blue } 
    public method SetSphereOpacity { opacity }

    public method SetDepth { res } { set resolution $res }
    public method GetDepth { } { return $resolution }
    public method GetPoints { } 
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
    public method SetPreviewLabel { label } { $preview configure -labeltext $label }
    
    ## source retrieval
    public method GetIcosahedron { } { return $spheresource }

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
    private method spin { wiget direction } 
    private method save_icosahedron { fname }
    private method showicosa_cb { }
    private method showicowire_cb { }

    destructor {
	catch { 
	    $spheresource Delete
	    $spheremapper Delete
	    $wiremapper Delete
	    $sphereactor Delete
	    $wireactor Delete
	    
	    $xaxisactor Delete
	    $xaxismapper Delete
	    $xaxis Delete
	    $yaxisactor Delete
	    $yaxismapper Delete
	    $yaxis Delete
	    $zaxisactor Delete
	    $zaxismapper Delete
	    $zaxis Delete

	    $iren Delete
	    $renwin Delete
	    $renderer Delete	   
	}
    }
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::GetPoints { } {
    
    return [ [ $spheresource GetOutput ] GetPoints ]
   
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::SetSphereColor { red green blue } { 
    [ $sphereactor GetProperty ] SetColor $red $green $blue
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::SetSphereOpacity { opacity } { 
    [ $sphereactor GetProperty ] SetOpacity $opacity
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::SetWireframeColor { red green blue } { 
    [ $wireactor GetProperty ] SetColor $red $green $blue
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::SetWireframeOpacity { opacity } {    
    [ $wireactor GetProperty ] SetOpacity $opacity
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::SetWireframeLineWidth { width } {
    [ $wireactor GetProperty ] SetLineWidth $width
}

itcl::body mpjguiicosahedron::showicosa_cb { } {
 
    if { $showicosa == 0 } {
	RemoveActor [ GetSphereActor ] 
    } else {
        AddActor [ GetSphereActor ]
    }

   Update
}

itcl::body mpjguiicosahedron::showicowire_cb { } {

    if { $showicowire == 0 } {
        RemoveActor [ GetWireframeActor ]
    } else {
        AddActor [ GetWireframeActor ]
    }
   
    Update
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::Reset { } {

    set resolution 1
    
    $spheresource SetDepth $resolution 
    
    Update
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::Update { } {
    
    ## update arrows
    $xaxis SetTipLength [ expr $tiplength / $scale ]
    $xaxis SetTipRadius [ expr $tiprad / $scale ]
    $xaxis SetShaftRadius [ expr $shaftrad / $scale ]
    $xaxis Update

    $yaxis SetTipLength [ expr $tiplength / $scale ]
    $yaxis SetTipRadius [ expr $tiprad / $scale ]
    $yaxis SetShaftRadius [ expr $shaftrad / $scale ]
    $yaxis Update

    $zaxis SetTipLength [ expr $tiplength / $scale ]
    $zaxis SetTipRadius [ expr $tiprad / $scale ]
    $zaxis SetShaftRadius [ expr $shaftrad / $scale ]
    $zaxis Update

    ## update sphere source
    $spheresource SetDepth $resolution
    $spheresource Update
    
    set numpoints [ [ $spheresource GetOutput ] GetNumberOfPoints ]
    
    $renwin Render
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::Save { } {
    
    set fname [tk_getSaveFile -title "Save as" -initialfile $filename \
		   -filetypes {{"Text" {.txt}}} ]
    if { [ string length $fname ] < 1 } {
        return 0
    }
    
    save_icosahedron $fname
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::spin_int { widget direction } {    

    set new_value [expr [$widget get] + $direction]

    if { $new_value > 20 } { set new_value 20 }
    if { $new_value < 1 } { set new_value 1 }
    
    $widget clear
    $widget insert 0 $new_value

    Update
}

itcl::body mpjguiicosahedron::spin { widget direction } {   

    set new_value [expr [$widget get] + $direction]
    
    if { $new_value > 1000 } { set new_value 1000 }
    if { $new_value < 1 } { set new_value 1 }
    
    $widget clear
    $widget insert 0 $new_value

    Update
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::create_button_box { basewidget } {

    set buttonbox [ iwidgets::buttonbox $basewidget.buttons -pady 5 ]
    
    $buttonbox add update -text "Reset" -command [itcl::code $this Reset ]
    $buttonbox add print -text "Save" -command [itcl::code $this Save ]
    $buttonbox add dismiss -text "Dismiss" -command [itcl::code $this Hide ]
   
    pack $buttonbox -side top
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::create_property_box { basewidget } {

    set propertybox [ frame $basewidget.param ]
    pack $propertybox -side top -fill both -pady 2 -expand f

    set res $propertybox.res

    iwidgets::spinint $res \
	-labeltext "Depth: " \
	-width 3 \
	-range { 1 20 } \
	-wrap no \
	-command [ itcl::code $this Update ] \
	-decrement [itcl::code $this spin_int $res -1] \
	-increment [itcl::code $this spin_int $res +1] \
   	-textvariable [ itcl::scope resolution ] 

    pack $propertybox.res -side left -padx 5

    $propertybox.res clear
    $propertybox.res insert 0 1
    
    label $propertybox.dummy1
    pack $propertybox.dummy1 -side left -padx 2

    label $propertybox.dummy2 -text "Number of points:"
    pack $propertybox.dummy2 -side left

    set points $propertybox.points
    label $points -textvariable [ itcl::scope numpoints ]
    pack $points -side left

    label $propertybox.dummy3
    pack $propertybox.dummy3 -side left -padx 2
    
    set showicosa [ checkbutton $propertybox.showicosa \
			-text "Show icosahedron"  \
			-variable [ itcl::scope showicosa ] \
			-command [ itcl::code $this showicosa_cb ] ]
    pack $showicosa -side left

    set showicowire [ checkbutton $propertybox.showicowire \
			 -text "Show wireframe" \
			 -variable [ itcl::scope showicowire ] \
			 -command [ itcl::code $this showicowire_cb ] ]
    pack $showicowire -side left

    set extframe [ frame $basewidget.extframe ]
    pack $extframe -side top -expand f -fill both

    set tipradwidget $extframe.tipradwidget
    iwidgets::spinint $tipradwidget \
	-labeltext "Tip radius: " \
	-width 4 \
	-range { 1 1000 } \
	-wrap no \
	-decrement [itcl::code $this spin $tipradwidget -1] \
	-increment [itcl::code $this spin $tipradwidget +1] \
	-textvariable [ itcl::scope tiprad ] \
	-command [ itcl::code $this Update ]
    pack $extframe.tipradwidget -side left -padx 5
    $tipradwidget clear
    $tipradwidget insert 0 8
    
    set shaftradwidget $extframe.shaftradwidget
    iwidgets::spinint $shaftradwidget \
	-labeltext "Shaft radius: " \
	-width 4 \
	-range { 1 1000 } \
	-wrap no \
	-decrement [itcl::code $this spin $shaftradwidget -1] \
	-increment [itcl::code $this spin $shaftradwidget +1] \
        -textvariable [ itcl::scope shaftrad ] \
        -command [ itcl::code $this Update ]		    
    pack $extframe.shaftradwidget -side left -padx 5
    $shaftradwidget clear
    $shaftradwidget insert 0 3

    set tiplengthwidget $extframe.tiplengthwidget
    iwidgets::spinint $tiplengthwidget \
	-labeltext "Tip length: " \
	-width 4 \
	-range { 1 1000 } \
	-wrap no \
	-decrement [itcl::code $this spin $tiplengthwidget -1] \
	-increment [itcl::code $this spin $tiplengthwidget +1] \
        -textvariable [ itcl::scope tiplength ] \
	-command [ itcl::code $this Update ]   
    pack $extframe.tiplengthwidget -side left -padx 5
    $tiplengthwidget clear
    $tiplengthwidget insert 0 8
    
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::save_icosahedron { fname } {
 
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
itcl::body mpjguiicosahedron::create_preview_frame { basewidget } {
    
    set preview [ iwidgets::labeledframe $basewidget.render \
		      -relief sunken -labeltext "Preview" -labelpos nw]
    pack $preview -side top -fill both -expand t
    
    set preview_widget [vtkTkRenderWidget [$preview childsite].renderwidget \
			    -width $width -height $height ]
    pack $preview_widget -side top -fill both -expand t

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
    
 #      set trackball [ vtkInteractorStyleTrackballCamera [ pxvtable::vnewobj ] ]
  #      [ $renwin GetInteractor ] SetInteractorStyle $trackball

}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::create_pipeline { basewidget } {    
    
    ## create renderer
    set renderer [ vtkRenderer [ pxvtable::vnewobj ] ]

    ## add actors
    $renderer AddActor $xaxisactor
    $renderer AddActor $yaxisactor
    $renderer AddActor $zaxisactor    
    $renderer AddActor $sphereactor
    $renderer AddActor $wireactor

    ## set renderer options
    [ $renderer GetActiveCamera ] Zoom 1.4
    [ $renderer GetActiveCamera ] SetFocalPoint 0.0 0.0 0.0
    #$renderer SetBackground 0.1 0.2 0.4
    $renderer SetBackground 0 0 0

    ## create render window and interactor
    ##set renwin [ vtkRenderWindow [ pxvtable::vnewobj ] ]
    ##$renwin AddRenderer $renderer

    ##set iren [ vtkRenderWindowInteractor [ pxvtable::vnewobj ] ]
    ##$iren SetRenderWindow $renwin

    ##set trackball [ vtkInteractorStyleTrackballCamera [ pxvtable::vnewobj ] ]
    ##$iren SetInteractorStyle $trackball
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiicosahedron::InitializeWindow { parent } {

    mpjguiobject::InitializeWindow $parent

    if { $parent == 0 } {
       SetTitle "mpjguiicosahedron: $filename" 
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

    RemoveActor [ GetSphereActor ]
    RemoveActor [ GetWireframeActor ]

    $renwin Render
    
    if { $parentwidget == 0 } {
	eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    }
}

# ---------------------------------------------------------------------------
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    
    option add *textBackground seashell
    
    wm withdraw .
        
    set sph [ mpjguiicosahedron \#auto 0 ]   ; ## create object as top-level
    $sph Initialize 0
    
    #$sph SetPreviewLabel "test"

    $sph Show ; ## map window
}

