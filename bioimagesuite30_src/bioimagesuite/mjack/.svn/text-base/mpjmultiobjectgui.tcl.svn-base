package provide mpjmultiobjectgui 1.0

package require pxitclobject 1.0


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



# --------------------------------------------------------------
#  abstract base GUI Class for mpjmultiimageGUI
# --------------------------------------------------------------

itcl::class mpjmultiobjectGUI {

    public    variable  itclobjectlist "" 
    public    variable  currentobject -1
    protected variable  objectGUI 0
    protected variable  basewidget  0
    protected variable  initialized 0
    private   common    thisparam
    public    variable  description "Object"
    public    variable  callback 0
    public    variable  listwidget 0

    constructor { } {
    }
    
    destructor {
	set l [ llength $itclobjectlist ]
	for { set a 0 } { $a < $l } { incr a } {
	    catch { itcl::delete object [ lindex $itclobjectlist $a ] }
	}
    }
    
    public    method GetThisPointer { } { return $this }
    public    method Initialize { basewidg } 

    public    method GetObjectGUI { } { return $objectGUI }

    public    method AddObject { descr }    
    public    method SetObject { index }
    public    method GetObject { index }     
    public    method DeleteObject { index }

    public    method Update {}

    public    method AddFunction { command name post }	    
    # -----------------------------------------
    # Function that must be overriden
    protected method CreateNewObject { } { puts stdout "Error!"}
    protected method CreateNewObjectGUI { widget } { puts stdout "Error!"}

}

itcl::body mpjmultiobjectGUI::GetObject { index } {

    if { $initialized == 0 } { 	return 0  }
    
    if { $index == - 1} {
	set index [ $listwidget index [ $listwidget get] ]
    }
    
    return [ lindex $itclobjectlist $index ]
}

itcl::body mpjmultiobjectGUI::SetObject { index } {
    
    if { $initialized == 0 } { 	return 0  }
    
    if { $index == -1 } {
	set index [$listwidget index [ $listwidget get ]]
    }
    
    set ln [ llength $itclobjectlist  ]
    if { $index < 0 || $index >= $ln } { return 0 } 

#    $listbox selection clear 0 [ expr $ln -1 ]
#    $listbox selection set $index $index
    
    $listwidget select $index

    $objectGUI SetObject [ lindex $itclobjectlist $index ]
    set currentobject $index
    return $index 
}

itcl::body mpjmultiobjectGUI::AddObject { descr } {

    if { $initialized == 0 } { return 0 }

    set new_itclobject [ $this CreateNewObject ]    
        
    $listwidget insert [llength $itclobjectlist] $descr

    lappend itclobjectlist $new_itclobject

    return $new_itclobject
}

itcl::body mpjmultiobjectGUI::DeleteObject { index } {
    
    if { $initialized == 0 } { return 0 }
    
    set ln [ llength $itclobjectlist ]
    
    if { $index < 0 || $index >= $ln } { return 0 } 

    catch { itcl::delete object [ lindex $itclobjectlist $index ] }
    set itclobjectlist [ lreplace $itclobjectlist $index $index ]
    
    $listwidget delete $index $index
}

itcl::body mpjmultiobjectGUI::Update {} {
    if { $basewidget !=0 } {
	set a [ $basewidget cget -labeltext ]
	if { $a != $description } {
	    $basewidget configure -labeltext  $description
	}
	SetObject $currentobject
    }
}

itcl::body mpjmultiobjectGUI::AddFunction { command name post }	{
    $objectGUI AddFunction $command $name $post
}

itcl::body mpjmultiobjectGUI::Initialize { basewidg } {
    
    if { $initialized == 1 } { 
	return $basewidget 
    }
    
    set basewidget [ iwidgets::Labeledframe $basewidg -labelpos nw -labeltext $description ]
    set base [ $basewidget childsite ]
    
#    frame $base.left; 
#    frame $base.right;
#    pack $base.left -side left -expand true -fill both -padx 5 -pady 5
#    pack $base.right -side right -expand true -fill both -padx 5 -pady 5

    set listwidget [ iwidgets::optionmenu $base.lw \
		      -command "itcl::code $this SetObject -1"]
    pack $listwidget -side top -expand t
    
    set objectGUI [ $this CreateNewObjectGUI $base ]
	     
#    set listbox [ iwidgets::scrolledlistbox $base.left.slb -vscrollmode \
#dynamic -hscrollmode dynamic -selectmode single -labelpos nw ]
#    eval "$listbox configure -selectioncommand { $this SetObject -1 }"
    
    return $basewidget
}
    

		
    
