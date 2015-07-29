
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

package provide mpjguiobject 1.0

package require vtkmpjcontrib 1.1
package require pxtclutil 1.0
package require Itcl 3.2
package require Iwidgets 4.0

itcl::class mpjguiobject {

    protected variable parentwidget 0
    protected variable basewidget 0
    
    constructor { args } {
    }
    
    destructor {}

    protected method InitializeWindow { parent } ; ## redefine this in derived classes

    public method Initialize { parent } { InitializeWindow $parent }
    public method Show { }
    public method Hide { }
    public method Destroy { } { destroy $basewidget }
    public method SetTitle { title }

    public method GetBaseWidget { } { return $basewidget }
    public method GetParentWidget { } { return $parentwidget }
}

#-------------------------------------------------------------------------------------
itcl::body mpjguiobject::InitializeWindow { parent } {

    set parentwidget $parent

    if { $parent == 0 } {

	set basewidget [ toplevel .[ pxvtable::vnewobj ] ]
	
	wm withdraw $basewidget	
	
    } else { 

	set basewidget [ frame $parent.[ pxvtable::vnewobj ] ]

    }
}

itcl::body mpjguiobject::Show { } {

    if { $basewidget == 0 } { 
	return 
    }

    if { $parentwidget != 0 } {
	pack $basewidget
    }

    if { [ winfo ismapped $basewidget ] == 1 } {
	raise $basewidget
	return $basewidget
    }
	
    if { $parentwidget == 0 } {
	wm deiconify $basewidget
    }
    return $basewidget
}


itcl::body mpjguiobject::Hide { } {
    catch { wm withdraw $basewidget }
}

itcl::body mpjguiobject::SetTitle { title } {
    if { $parentwidget == 0 } {
	wm title $basewidget $title
    }
}

# ---------------------------------------------------------------------------
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {
    
    option add *textBackground seashell

    wm withdraw .
        
    set guiobj [ mpjguiobject \#auto 0 ]   ; ## create object as top-level
    $guiobj Initialize 0

    $guiobj Show  ; ## map window
}

