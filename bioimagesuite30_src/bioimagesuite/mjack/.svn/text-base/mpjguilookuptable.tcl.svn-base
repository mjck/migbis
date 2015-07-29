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

package provide mpjguilookuptable 1.0

package require mpjguiobject 1.0

itcl::class mpjguilookuptable {

    inherit mpjguiobject

    private variable iren 0          ; ## vtkRenderWindowInteractor
    private variable renderer 0      ; ## vtkRenderer
    private variable renwin          ; ## vtkRenderWindow
    private variable lookuptable 0   ; ## vtkLookupTable
    private variable scalarbar 0     ; ## vtkScalarBarActor 
    private variable preview_widget 0; ## vtkTkRenderWidget

    private variable hue_min 0.0
    private variable hue_max 1.0
    private variable sat_min 1.0
    private variable sat_max 1.0
    private variable val_min 1.0
    private variable val_max 1.0
    private variable alpha_min 1.0
    private variable alpha_max 1.0
    private variable num_colors 256
    private variable min_value 0.0
    private variable max_value 1.0
    private variable scale 0
    private variable filename "new_lookuptable.cmap"

    private variable menubase 0
    private variable buttonbox 0
    private variable tablerange 0
    private variable colorrange 0
    private variable preview 0
    
    constructor { args } { 
	mpjguiobject::constructor $args
    } {

	set lookuptable [ vtkLookupTable [ pxvtable::vnewobj ] ]	
	$lookuptable SetHueRange 0.0 1.0
	$lookuptable Build
	
	set scalarbar [ vtkScalarBarActor [ pxvtable::vnewobj ] ]
	$scalarbar SetLookupTable $lookuptable
    }
    
    protected method InitializeWindow { parent } 
    
    public method Reset { }
    public method Load { }
    public method SaveAs { }
    public method Update { }
    public method GetLookupTable { } { return $lookuptable }
    public method GetButtonBoxWidget { } { return $buttonbox }
    public method GetColorRangeWidget { } { return $colorrange }
    public method GetPreviewWidget { } { return $preview }
    public method GetRenderWidget { } { return $preview_widget }
    public method GetMenuBar { } { return $menubase }
    public method SetTableRange { min max } { set min_value $min; set max_value $max; }
    public method SetHueRange { min max } { set hue_min $min; set hue_max $max; }
    public method SetValueRange { min max } { set val_min $min; set val_max $max; }
    public method SetSaturationRange { min max } { set sat_min $min; set sat_max $max; }
    public method SetLookupTable { luk } 
    public method Copy { luk }

    private method file_new { }
    private method file_load { }
    private method file_save { }
    private method file_save_as { }
    private method load_lookuptable { fname }
    private method save_lookuptable { fname }
    private method create_menu { basewidget }
    private method create_menu_buttons { basewidget }
    private method create_color_frame { basewidget }
    private method create_preview_frame { basewidget }
    private method create_button_box { basewidget }
    private method create_pipeline { basewidget }
    private method scale_select { widget }
    private method spin_float { widget direction }
    private method spin_validate { widget value }

    destructor {
	catch { 
	    $iren Delete
	    $renwin Delete
	    $renderer Delete
	    $lookuptable Delete
	    $scalarbar Delete
	}
    }
}

itcl::body mpjguilookuptable::Reset { } {

    set num_colors 256
    set scale 0
    set min_value 0.0; set max_value 1.0
    set hue_min 0.0; set hue_max 1.0
    set sat_min 1.0; set sat_max 1.0
    set val_min 1.0; set val_max 1.0
    set alpha_min 1.0; set alpha_max 1.0

    ## unfortunately objects need to be recreated
    $renderer RemoveActor $scalarbar    
    $scalarbar Delete

    $lookuptable Delete
    set lookuptable [ vtkLookupTable [ pxvtable::vnewobj ] ]
    $lookuptable ForceBuild

    set scalarbar [ vtkScalarBarActor [ pxvtable::vnewobj ] ]
    [$scalarbar GetPositionCoordinate] SetCoordinateSystemToNormalizedViewport
    [$scalarbar GetPositionCoordinate] SetValue 0.075 0.30
    $scalarbar SetOrientationToHorizontal
    $scalarbar SetNumberOfLabels 3
    $scalarbar SetLabelFormat "%5.2f"
    $scalarbar SetWidth 0.85
    $scalarbar SetHeight 0.75
    $scalarbar SetLookupTable $lookuptable    
    
    $renderer AddActor2D $scalarbar

    Update
}

itcl::body mpjguilookuptable::Update { } {
    
    $lookuptable Modified;  ## make sure it will update

    $lookuptable SetScale $scale
    $lookuptable SetTableRange $min_value $max_value
    $lookuptable SetNumberOfTableValues $num_colors
    $lookuptable SetHueRange $hue_min $hue_max
    $lookuptable SetSaturationRange $sat_min $sat_max
    $lookuptable SetValueRange $val_min $val_max
    $lookuptable SetAlphaRange $alpha_min $alpha_max

    $lookuptable ForceBuild

    $renwin Render
}

itcl::body mpjguilookuptable::SetLookupTable { luk } {

    $renderer RemoveActor $scalarbar    
    $scalarbar Delete

    $lookuptable Delete
    set lookuptable $luk

    set scalarbar [ vtkScalarBarActor [ pxvtable::vnewobj ] ]
    [$scalarbar GetPositionCoordinate] SetCoordinateSystemToNormalizedViewport
    [$scalarbar GetPositionCoordinate] SetValue 0.075 0.30
    $scalarbar SetOrientationToHorizontal
    $scalarbar SetNumberOfLabels 3
    $scalarbar SetLabelFormat "%5.2f"
    $scalarbar SetWidth 0.85
    $scalarbar SetHeight 0.75
    $scalarbar SetLookupTable $lookuptable    
    
    $renderer AddActor2D $scalarbar

    $renwin Render
}

itcl::body mpjguilookuptable::Copy { luk } {
    
    #set luk [ $guiluk GetLookupTable ]
    set num_colors [ $luk GetNumberOfTableValues ]

    $lookuptable SetNumberOfTableValues $num_colors

    set table_range [ $luk GetTableRange ]
    set min_value [ lindex $table_range 0 ] 
    set max_value [ lindex $table_range 1 ]
    $lookuptable SetTableRange $min_value $max_value

    for { set i 0} { $i < $num_colors } { incr i } {
	set rgba [ $luk GetTableValue $i ]
	$lookuptable SetTableValue $i \
	    [lindex $rgba 0] [lindex $rgba 1] [lindex $rgba 2] [lindex $rgba 3]	
    }
    
    $renwin Render
    
}

itcl::body mpjguilookuptable::scale_select { widget } {
    set scale [$widget index [$widget get]]
    Update
}

itcl::body mpjguilookuptable::spin_validate { widget value } {    
    ## validate if it's a real number and in [0, 1] 
    if {[regexp {^[-+]?[0-9]*\.?[0-9]*([0-9]\.?[eE][-+]?[0-9]*)?$} $value]} {
	return [expr $value >= 0 && $value <= 1.0]
    } else {
	return 0
    }
}

itcl::body mpjguilookuptable::spin_float { widget direction } {    
    set new_value [expr [$widget get] + $direction]
    if { $new_value > 1.0 } { set new_value 0 }
    if { $new_value < 0.0 } { set new_value 1.0 }
    $widget clear
    $widget insert 0 $new_value
    Update
}

itcl::body mpjguilookuptable::Load { } {

    $this file_load
}

itcl::body mpjguilookuptable::SaveAs { } {
    
    $this file_save_as
}

itcl::body mpjguilookuptable::file_new { } {
    
    set filename "new_lookuptable"
    SetTitle "mpjguilookuptable: $filename"    
    Reset
}

itcl::body mpjguilookuptable::file_load { } {
    set fname [tk_getOpenFile -title "Load" -initialfile $filename -filetypes {{"Colormap" {.cmap}}} ]
    if { [ string length $fname ] < 1 } {
	return 0
    }
    load_lookuptable $fname
}

itcl::body mpjguilookuptable::file_save { } {
    save_lookuptable $filename
}

itcl::body mpjguilookuptable::file_save_as { } {
    set fname [tk_getSaveFile -title "Save as" -initialfile $filename -filetypes {{"Colormap" {.cmap}}} ]
    if { [ string length $fname ] < 1 } {
	return 0
    }    
    save_lookuptable $fname
}

itcl::body mpjguilookuptable::load_lookuptable { fname } {

    if { [ string length $fname ] < 1 || [ file readable $fname ] == 0 } {
	::pxtclutil::Warning "Cannot read color table from $fname\n Bad File"
	return 0
    }

    set fileid [ open $fname r ]

    gets $fileid line; if { $line != "#IPAGRGBAColormap" } { close $fileid; return 0 }
    
    gets $fileid line;  ## skip comment
    gets $fileid num_colors 
    $lookuptable SetNumberOfTableValues $num_colors
    
    gets $fileid line;  ## skip comment
    gets $fileid table_range
    set min_value [ lindex $table_range 0 ] 
    set max_value [ lindex $table_range 1 ]
    $lookuptable SetTableRange $min_value $max_value
    
    gets $fileid line; gets $fileid line; ## skip comments
    for { set i 0} { $i < $num_colors } { incr i } {
	gets $fileid rgba
	$lookuptable SetTableValue [lindex $rgba 0] \
	    [lindex $rgba 1] [lindex $rgba 2] [lindex $rgba 3] [lindex $rgba 4]	
    }

    close $fileid

    set filename $fname
    SetTitle "mpjguilookuptable: [file tail $filename]"

    $renwin Render
}

itcl::body mpjguilookuptable::save_lookuptable { fname } {

    set fileid [ open $fname w ]

    puts $fileid "#IPAGRGBAColormap"
    puts $fileid "#Number of Colors"
    puts $fileid $num_colors

    puts $fileid "\#Range"
    puts $fileid "$min_value $max_value"

    puts $fileid "\#Colors"
    puts $fileid "\#Index  R     G     B   Alpha"
    
    for { set i 0} { $i < $num_colors } { incr i } {
	set rgba [ $lookuptable GetTableValue $i ]
	puts $fileid "$i $rgba"
    }

    close $fileid
}

itcl::body mpjguilookuptable::create_menu_buttons { basewidget } {

    set menubase [ frame $basewidget.menu -width 400 -height 50 ]
    pack $menubase -side bottom -expand f -fill x -pady 1

    button $menubase.file_new -text "Reset" -command [ itcl::code $this file_new ]
    pack $menubase.file_new -side left

    button $menubase.file_load -text "Load" -command [ itcl::code $this file_load ]
    pack $menubase.file_load -side left
    
    button $menubase.file_save -text "Save" -command [ itcl::code $this file_save_as ]
    pack $menubase.file_save -side left
    
}

itcl::body mpjguilookuptable::create_menu { basewidget } {

    set menubase [ frame $basewidget.menu -width 400 -height 50 ]
    pack $menubase -side top -expand f -fill x -pady 1

    menubutton $menubase.file -text "File" -menu $menubase.file.m -underline 0
    pack $menubase.file -side left

    menu $menubase.file.m -tearoff 0
    $menubase.file.m add command -label "New" -underline 0 \
	-command [ itcl::code $this file_new ]
    $menubase.file.m add separator
    $menubase.file.m add command -label "Load..." -underline 0 \
	-command [ itcl::code $this file_load ]
    $menubase.file.m add command -label "Save" -underline 0 \
	-command [ itcl::code $this file_save ]
    $menubase.file.m add command -label "Save as..." -underline 5 \
	-command [ itcl::code $this file_save_as ]
    $menubase.file.m add separator
    $menubase.file.m add command -label "Close" -underline 1 \
	-command  [ itcl::code $this Hide ]

}

itcl::body mpjguilookuptable::create_button_box { basewidget } {

    set buttonbox [ iwidgets::buttonbox $basewidget.buttons -pady 5 ]
    
    $buttonbox add update -text "Update" -command [itcl::code $this Update]
    $buttonbox add dismiss -text "Dismiss" -command [itcl::code $this Hide ]
   
    pack $buttonbox -side top
}

itcl::body mpjguilookuptable::create_preview_frame { basewidget } {
    
    set preview [ iwidgets::labeledframe $basewidget.render \
		      -relief sunken -labeltext "Preview" -labelpos nw]
    pack $preview -side top -fill x -expand t
    
    set preview_widget [vtkTkRenderWidget [$preview childsite].renderwidget \
			    -width 400 -height 100 ]
    pack $preview_widget -side top -fill x -expand true
    update idletasks
    
    set renwin [ $preview_widget GetRenderWindow ]

    $renwin AddRenderer $renderer
    eval "bind $preview_widget <Expose> { $renwin Render }"
    eval "bind $preview_widget <Map> { $renwin Render }"


}

itcl::body mpjguilookuptable::create_pipeline { basewidget } {    
    
    [$scalarbar GetPositionCoordinate] SetCoordinateSystemToNormalizedViewport
    [$scalarbar GetPositionCoordinate] SetValue 0.075 0.30
    $scalarbar SetOrientationToHorizontal
    $scalarbar SetNumberOfLabels 3
    $scalarbar SetLabelFormat "%5.2f"
    $scalarbar SetWidth 0.85
    $scalarbar SetHeight 0.75

    set renderer [ vtkRenderer [ pxvtable::vnewobj ] ]
    $renderer AddActor2D $scalarbar
    $renderer SetBackground 0.5 0.5 0.5
    
    #    set renwin [ vtkRenderWindow [ pxvtable::vnewobj ] ]
    #    $renwin AddRenderer $renderer

    #    set iren [ vtkRenderWindowInteractor [ pxvtable::vnewobj ] ]
    #    $iren SetRenderWindow $renwin

}

itcl::body mpjguilookuptable::create_color_frame { basewidget } {

    set range [ iwidgets::labeledframe $basewidget.range -labeltext "Color range" -labelpos nw]
    pack $range -side top -fill both 
    set colorrange $range

    set buildfr [ frame [$range childsite].buildfr ]
    pack $buildfr -side top 
    
    set numcolors $buildfr.numcolors
    iwidgets::entryfield $numcolors \
	-width 5 \
	-validate numeric \
	-command [ itcl::code $this Update ] \
	-textvariable [ itcl::scope num_colors ] \
	-labeltext "Colors:" 
    pack $numcolors -side left -padx 5 -pady 5     
      
    set minscalar [ iwidgets::entryfield $buildfr.minscalar \
			-width 7 \
			-validate real \
			-textvariable [ itcl::scope min_value ] \
			-command [ itcl::code $this Update ] \
			-labeltext "Scalars:" ] 
    pack $minscalar -side left -padx 5 -pady 5 

    set maxscalar [ iwidgets::entryfield $buildfr.maxscalar \
			-width 7 \
			-validate real \
			-textvariable [ itcl::scope max_value ] \
			-command [ itcl::code $this Update ] \
			-labeltext "to:" ] 
    pack $maxscalar -side left -padx 0 -pady 5 


    set scalemenu $buildfr.scalemenu
    iwidgets::optionmenu $scalemenu \
	-command [itcl::code $this scale_select $scalemenu] 
    pack $scalemenu -side left -padx 5 -pady 5
    $scalemenu insert end Linear
    $scalemenu insert end Log10
    $scalemenu select Linear

    set from [ frame [$range childsite].from ]   
    pack $from -side top -fill x
    
    label $from.l0 -text "1:"
    pack $from.l0 -side left -fill x -padx 5
    
    set spinner $from.hue
    iwidgets::spinner $spinner \
	-labeltext "H: " \
	-width 6 \
	-command [itcl::code $this Update] \
	-validate [itcl::code $this spin_validate $spinner %P] \
	-decrement [itcl::code $this spin_float $spinner -0.01] \
	-increment [itcl::code $this spin_float $spinner +0.01] \
	-textvariable [itcl::scope hue_min] 
    pack $from.hue -side left -pady 5 -padx 2
    $from.hue clear
    $from.hue insert 0 0.0

    set spinner $from.sat
    iwidgets::spinint $spinner \
	-labeltext "S: " \
	-width 6 \
	-command [itcl::code $this Update] \
	-validate [itcl::code $this spin_validate $spinner %P] \
	-decrement [itcl::code $this spin_float $spinner -0.01] \
	-increment [itcl::code $this spin_float $spinner +0.01] \
	-textvariable [itcl::scope sat_min]
    pack $from.sat -side left -pady 5 -padx 2
    $from.sat clear
    $from.sat insert 0 1.0

    set spinner $from.val
    iwidgets::spinint $from.val \
	-labeltext "V: " \
	-width 6 \
	-command [itcl::code $this Update] \
	-validate [itcl::code $this spin_validate $spinner %P] \
	-decrement [itcl::code $this spin_float $spinner -0.01] \
	-increment [itcl::code $this spin_float $spinner +0.01] \
	-textvariable [itcl::scope val_min]
    pack $from.val -side left -pady 5 -padx 2
    $from.val clear
    $from.val insert 0 1.0
  
    set spinner $from.alpha
    iwidgets::spinint $spinner \
	-labeltext "A: " \
	-width 6 \
	-command [itcl::code $this Update] \
	-validate [itcl::code $this spin_validate $spinner %P] \
	-decrement [itcl::code $this spin_float $spinner -0.01] \
	-increment [itcl::code $this spin_float $spinner +0.01] \
	-textvariable [itcl::scope alpha_min]
    pack $from.alpha -side left -pady 5 -padx 2
    $from.alpha clear
    $from.alpha insert 0 1.0

    set to [ frame [$range childsite].to ]
    pack $to -side top -fill x

    label $to.l1 -text "2:" -anchor e
    pack $to.l1 -side left -fill x -padx 5
    
    set spinner $to.hue
    iwidgets::spinint $spinner \
	-labeltext "H: " \
	-width 6 \
	-command [itcl::code $this Update] \
	-validate [itcl::code $this spin_validate $spinner %P] \
	-decrement [itcl::code $this spin_float $spinner -0.01] \
	-increment [itcl::code $this spin_float $spinner +0.01] \
	-textvariable [itcl::scope hue_max]
    pack $to.hue -side left -pady 5 -padx 2
    $to.hue clear
    $to.hue insert 0 1.0

    set spinner $to.sat
    iwidgets::spinint $spinner \
	-labeltext "S: " \
	-width 6 \
	-command [itcl::code $this Update] \
	-validate [itcl::code $this spin_validate $spinner %P] \
	-decrement [itcl::code $this spin_float $spinner -0.01] \
	-increment [itcl::code $this spin_float $spinner +0.01] \
	-textvariable [itcl::scope sat_max]
    pack $to.sat -side left -pady 5 -padx 2
    $to.sat clear
    $to.sat insert 0 1.0

    set spinner $to.val
    iwidgets::spinint $spinner \
	-labeltext "V: " \
	-width 6 \
	-command [itcl::code $this Update] \
	-validate [itcl::code $this spin_validate $spinner %P] \
	-decrement [itcl::code $this spin_float $spinner -0.01] \
	-increment [itcl::code $this spin_float $spinner +0.01] \
	-textvariable [itcl::scope val_max]    
    pack $to.val -side left -pady 5 -padx 2
    $to.val clear
    $to.val insert 0 1.0

    set spinner $to.alpha
    iwidgets::spinint $spinner \
	-labeltext "A: " \
	-width 6 \
	-command [itcl::code $this Update] \
	-validate [itcl::code $this spin_validate $spinner %P] \
	-decrement [itcl::code $this spin_float $spinner -0.01] \
	-increment [itcl::code $this spin_float $spinner +0.01] \
	-textvariable [itcl::scope alpha_max]
    pack $to.alpha -side left -pady 5 -padx 2
    $to.alpha clear
    $to.alpha insert 0 1.0   
}

#-------------------------------------------------------------------------------------
itcl::body mpjguilookuptable::InitializeWindow { parent } {

    mpjguiobject::InitializeWindow $parent

    SetTitle "mpjguilookuptable: $filename" 

    create_pipeline $basewidget

    if { $parentwidget == 0 } {
	create_menu $basewidget
    } else {
	create_menu_buttons $basewidget
    }
    create_color_frame $basewidget
    create_preview_frame $basewidget
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
        
    set luk [ mpjguilookuptable \#auto 0 ]   ; ## create object as top-level
    $luk Initialize 0

    
    $luk Show ; ## map window
}

