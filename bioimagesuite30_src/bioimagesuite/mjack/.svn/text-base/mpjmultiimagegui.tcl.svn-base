package provide mpjmultiimagegui 1.0

package require pxitclimage 1.0


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



# ----------------------------------------------------------
#  GUI for multiple image object
# ----------------------------------------------------------
itcl::class mpjmultiimageGUI {

    inherit mpjmultiobjectGUI

    constructor { } {
	mpjmultiobjectGUI::constructor
    } {
    }
    
    destructor { }
    
    public    method Initialize { basewidg } 

    public    method AddImage { descr }
    public    method SetImage { index } 
    public    method GetImage { index } { return [ GetObject $index ] }
    public    method GetCurSelection { } 
    public    method DeleteImage { index } { DeleteObject $index }

    public    method SetDisplayCommand { cmd }

    protected method CreateNewObject { }
    protected method CreateNewObjectGUI { widget }

}

# ----------------------------------------------------------
itcl::body mpjmultiimageGUI::CreateNewObject { } {

    return  [ [  pxitclimage \#auto ] GetThisPointer ]
}

itcl::body mpjmultiimageGUI::CreateNewObjectGUI { widget } {
    set itclimageGUI [ [ pxitclimageGUI \#auto ] GetThisPointer ]
    pack [ $itclimageGUI Initialize $widget.[ pxvtable::vnewobj ] ] -side left -fill x
    return $itclimageGUI
}

itcl::body mpjmultiimageGUI::AddImage { descr } {
    
    set newobj [ $this AddObject $descr ]
    $this SetImage [ expr [ llength $itclobjectlist ] -1 ]    
    return $newobj
}

itcl::body mpjmultiimageGUI::SetImage { index } {

    $this SetObject $index
    $objectGUI configure -description [ $listwidget get $currentobject ]
    $objectGUI Update
}

itcl::body mpjmultiimageGUI::SetDisplayCommand { cmd } {
    $objectGUI configure -displaycommand $cmd
}

itcl::body mpjmultiimageGUI::GetCurSelection { } {    
    return [ $listwidget index [ $listwidget get ]]
}

itcl::body mpjmultiimageGUI::Initialize { basewidg } {
    
    if { $initialized == 1 } { 
	return $basewidget 
    }
    
    mpjmultiobjectGUI::Initialize $basewidg

    #$listbox configure -selectmode single
    $listwidget configure -command [itcl::code $this SetImage -1]
    
    set initialized 1
    
    return $basewidget
}
 



