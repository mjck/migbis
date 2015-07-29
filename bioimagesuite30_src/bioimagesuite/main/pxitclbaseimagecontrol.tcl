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

package provide pxitclbaseimagecontrol 1.0

# 	$Id: pxitclbaseimagecontrol.tcl,v 1.1 2002/05/09 14:04:36 papad Exp papad $	





package require pxtclutil  1.0
package require pxvtable 1.0
package require Itcl 3.2
package require pxitclimage 1.0
package require pxitclbasecontrol 1.0


itcl::class pxitclbaseimagecontrol { 

    inherit pxitclbasecontrol

    # --- begin variable def -----
    protected common forceimageoutputformat 0
    protected variable currentresults 0
    public    variable currentimage 0 
    protected variable undoimage 0 
    protected variable imagelist ""
    public    variable currentpoint [ list 0 0 0 ]
    protected variable initialized 0

    # --- end of variable def -----

    constructor { par } {
	::pxitclbasecontrol::constructor $par
    } {
	set currentresults  [  [ pxitclimage \#auto ] GetThisPointer ]
	set currentimage    [  [ pxitclimage \#auto ] GetThisPointer ]
	set undoimage       [  [ pxitclimage \#auto ] GetThisPointer ]
      
	lappend itclobjectlist $currentresults
	lappend itclobjectlist $currentimage 

	set imagelist [ list $currentresults $currentimage $undoimage ]

	$this SetDefaultOutputImageFormat
    }
	
    # --- begin method def -----
    protected method AddImagesToLists { args }
    public    method UnloadImages { args } 
    public    method Debug { } 

    # --- Set Image/Results Methods 
    # -----------------------------
    public method SetImage   { img }
    public method SetResults { img }
    public method SetValueImage { vtkimg  }
    public method SetImageFromObject   { img obj }
    public method SetResultsFromObject { img obj }

    # --------------------
    # parent communication
    # ---------------------
    public proc   AddPrefix { oldname prefix }
    public method SendResultsToParent { args }
    public method SendImageToParent { args } 

    # Change Image
    # ------------
    public method CopyResultsToImage { }
    public method RevertToPreviousImage { }  

    public method SetImageOutputFormat { mode } 
    protected method SetDefaultOutputImageFormat { }
    # --- end method def -----
}
# ------------------- End of Header ---------------------------------
::itcl::body pxitclbaseimagecontrol::AddImagesToLists { args } {

    for { set i 0 } { $i < [ llength $args ] } { incr i } {
	set obj [ lindex $args $i ] 
	append imagelist $obj
	append itclobjectlist $obj
    }
}
# -----------------------------------------------------------------
::itcl::body pxitclbaseimagecontrol::UnloadImages { args } {

    set dounload 0

    if { $basewidget == 0 } {
	set dounload 1 
    } elseif { [ llength $args ] > 0 } {
	if { [ lindex $args 0 ] == "force" } {
	    set dounload 1
	}
    }
    
    if { $dounload == 0 } {
	set ok [ tk_messageBox -type yesno -default no -parent $basewidget -title "Think again ..." -message "Unload all images (for this control) from memory?" -icon question  ]
	if { $ok == "yes" } {
	    set dounload 1
	}
    }
    
    if { $dounload == 0 } { return   }
    
    for { set i 0 } { $i < [ llength $imagelist ] } { incr i } {
	[ lindex $imagelist $i ] UnLoad
    }
}
# ----------------------------------------------------------
::itcl::body pxitclbaseimagecontrol::Debug { } {
    for { set i 0 } { $i < [ llength $imagelist ] } { incr i } {
	puts stdout "[ [ lindex $imagelist $i ] GetDescription ]\n"
    }
    puts stdout "\n"
}

# -------------------------------------------------------------------------------------------
#  
#    Programming Interface for master objects
#
# -------------------------------------------------------------------------------------------

::itcl::body pxitclbaseimagecontrol::SetImage { img  } {

    if { [ $img GetImageSize  ] < 2 } {	return 0    }
    $undoimage ShallowCopy $currentimage
    $currentimage ShallowCopy $img
    return 1
}

::itcl::body pxitclbaseimagecontrol::SetResults { img  } {
    if { [ $img GetImageSize  ] < 2 } {	return 0    }
    $currentresults ShallowCopy $img
    return 1
}

itcl::body pxitclbaseimagecontrol::SetValueImage { vtkimg  } {
    if { $parent == 0 } { 
	return  0
    } 
    return [ $parent SetValueImage $vtkimg ]
}

::itcl::body pxitclbaseimagecontrol::SetImageFromObject { img obj  } {
    if { [ $img GetImageSize  ] < 2 } {	return 0    }
    $undoimage ShallowCopy $currentimage
    SetImage $img
    return 1
}

::itcl::body pxitclbaseimagecontrol::SetResultsFromObject { img obj  } {
    if { [ $img GetImageSize  ] < 2 } {	return 0    }
    SetResults $img
    return 1
}
   



# -------------------------------------------------------------------------------------------
#  
#    Send Results/Info/Requests To Parent
#
# -------------------------------------------------------------------------------------------

::itcl::body pxitclbaseimagecontrol::AddPrefix { oldname prefix } {

    set dname [ file dirname $oldname ]
    set tname [ file tail $oldname ]
    
    set newname [ file join $dname $prefix ]
    set newname "${newname}_${tname}"
    return $newname 
}

::itcl::body pxitclbaseimagecontrol::SendResultsToParent { } {

    if { $parent == 0 } { return  } 
    if { [ $currentresults GetImageSize ] < 2 } {
	::pxtclutil::Warning "No results image in Memory! Cannot send to parent!"
	return 0
    }

    puts stderr "Here $parent=$parent"
    return [ $parent SetResultsFromObject $currentresults $this ]
    
}
# -------------------------------------------------------------------------------------------    
::itcl::body pxitclbaseimagecontrol::SendImageToParent { } {

    if { $parent == 0 } { return  } 
    if { [ $currentresults GetImageSize ] < 2 } {
	::pxtclutil::Warning "No results image in Memory! Cannot send to parent!"
	return 0
    }

    return [ $parent SetImageFromObject $currentresults $this ]
    
}
# -------------------------------------------------------------------------------------------    
 
::itcl::body pxitclbaseimagecontrol::RevertToPreviousImage { }  {

    if {  [ $undoimage GetImageSize ] < 2 } {
	return 0
    }
    set tmp  [  [ pxitclimage \#auto ] GetThisPointer ]
    $tmp ShallowCopy $undoimage
    set ok [ SetImageFromObject $tmp $this ]
    itcl::delete object $tmp 
    return $ok
}
# -------------------------------------------------------------------------------------------    
itcl::body pxitclbaseimagecontrol::CopyResultsToImage { } {
    
    return [ SetImageFromObject $currentresults $this  ]
}


# -------------------------------------------------------------------------------------------    
itcl::body pxitclbaseimagecontrol::SetImageOutputFormat { mode } {
    set forceimageoutputformat [ ::pxitclimage::SetOutputFormat $mode ]
}

itcl::body pxitclbaseimagecontrol::SetDefaultOutputImageFormat { } {

    global pxtcl_pref_array
    if {  $pxtcl_pref_array(ForceOutputFormat) == "Analyze" } {
	set forceimageoutputformat 1
    } elseif { $pxtcl_pref_array(ForceOutputFormat) == "NIFTI" } {
	set forceimageoutputformat 2
    } else {
	set forceimageoutputformat 0
    }
    set forceimageoutputformat [ ::pxitclimage::SetOutputFormat $forceimageoutputformat ]
}

