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

package provide pxitclbasecontrol 1.0

# 	$Id: pxitclbasecontrol.tcl,v 1.1 2002/05/09 14:04:36 papad Exp papad $	





package require pxtclutil  1.0
package require Itcl 3.2
package require feedbackplus 1.0
package require vtkpxcontrib 1.2
package require http
package require pxtclhelp 

itcl::class pxitclbasecontrol { 

    # --- begin variable def -----
    
    protected variable basewidget   0
    protected variable notebook 0
    protected variable itclobjectlist ""
    protected variable vtkobjectlist ""
    public    variable debugprogress 0
    public    variable fullyinitialized 0
    protected variable aboutdialog 0
    protected variable helpdialog 0
    protected variable parent 0
    protected variable isdialog 1

    private common progressbar 
    private common progress_lab 
    private common progress_val 


    public    variable appname "BaseControl"
    public    variable version $::pxtclvtkpxcontrib::version
    public    variable aboutstring "(c) X. Papademetris, M. Jackowski, A. Joshi, D. Scheinost, H.Okuda, R.T. Constable, L.H. Staib 1995-2011\nSection of Bioimaging Sciences, Department of Diagnostic Radiology, Yale University School of Medicine\n\n\nThis application is intended for research applications only! Use at your OWN RISK. If used for publication please acknowledge, see the webpage for more details. For additional support please visit the BioImage Suite forum at:\n\thttp://research.yale.edu/bioimagesuite/forum/\n BioImage Suite is made available under the terms of the GPL v2.\n" 

    # --- end of variable def -----

    constructor { par } {

	set dopar 0 
	catch { 
	    if { [ $par isa pxitclbasecontrol ] == 1 } {
		set parent $par
	    } 
	}
	set itclobjectlist ""
	set vtkobjectlist ""

	set progressbar($this) 0
	set progress_lab($this) "Status"
	set progress_val($this) 0
    }

    destructor  {
	for { set i 0 } { $i < [ llength $itclobjectlist ] } { incr i } {
	    catch { delete object [ lindex $itclobjectlist $i ] }
	}

	for { set i 0 } { $i < [ llength $vtkobjectlist ] } { incr i } {
	    catch { [ lindex $vtkobjectlist $i ] Delete }
	}
    }

    # --- begin method def -----

    # GUI Stuff
    public method AddToMenuButton { mb args}
    public method ShowWindow { args }
    public method DelayedInitialize { }
    public method HideWindow { }
    public method DestroyWindow { }
    public method SetTitle { title }
    public method WatchOn { } 
    public method WatchOff { } 
    public method SetCurrentDirectory { fname } { }

    # About Command 
    public method HelpCommand { }
    public method AboutCommand { }
    public method PrintDatabase { }
    public method CheckForUpdates { }
    public method ExitCommand { }

    # Enable/Disable pieces of the User Interface
    public proc   DisableWidget { widget  }
    public proc   DisableUI     { widgetlist }
    public proc   EnableWidget { widget }
    public proc   EnableUI     { widgetlist  }
    public proc   SetStateOfWidget { widget mode }
    public proc   SetStateOfUI     { widgetlist mode  }

    # Configure Whole Trees
    public proc   ConfigureWidgetTree { widget optionlist }
    public proc   SetWidgetTreeColors { widget backc forec }

    # For Global Scoping
    public method GetThisPointer { } { return $this }
    public method GetBaseWidget  { } { return $basewidget }
    public method GetParent  { } { return $parent }

    # Add Logo Label
    public proc   AddLogoLabel { w }
    
    # create progress bar 
    protected method CreateProgressBar { w } 
    public method ProgressCallback   { filter lab } 
    public method ShowProgress       { filter lab } 
    public method ShowProgressVal    { name   val }
    public method SetFilterCallbacks { filter comment } 
    # --- end method def -----
}
# ------------------- End of Header ---------------------------------
# -------------------------------------------------------------------------------------------
#  
#    GUI Stuff 
#
# -------------------------------------------------------------------------------------------
::itcl::body pxitclbasecontrol::AddToMenuButton { mb args} {

    if { $isdialog == 0 } { return 0 }
    set name "Show Control" 

    if { [ llength $args ] > 0 } {
	set name [ lindex $args 0 ]
    }

    eval "$mb add command -command { $this ShowWindow } -label \"$name\""
} 


::itcl::body pxitclbasecontrol::DelayedInitialize { } {

    set fullyinitialized 1
}

::itcl::body pxitclbasecontrol::ShowWindow { args } {

    if { $isdialog == 0 } { return 0 }

    if { $basewidget == 0 } { 
	return 
    }

    if { $fullyinitialized == 0 } {
	$this DelayedInitialize
    }


    if { $notebook !=0 && [ llength $args ] > 0 } {
	$notebook view [ lindex $args 0 ]
    }

    catch  {
	if { [ winfo ismapped $basewidget ] == 1 } {
	    wm withdraw $basewidget
	}
	wm deiconify $basewidget
    }
    return $basewidget
}
::itcl::body pxitclbasecontrol::DestroyWindow { } {
    if { $isdialog == 0 } { return 0 }
    catch { destroy $basewidget }
}
# ----------------------------------------------------------
::itcl::body pxitclbasecontrol::HideWindow { } {
    if { $isdialog == 0 } { return 0 }
    catch { wm withdraw $basewidget }
}
# ----------------------------------------------------------
::itcl::body pxitclbasecontrol::SetTitle { title } {
    if { $isdialog == 0 } { return 0 }
    catch {  wm title $basewidget $title }
}
# ----------------------------------------------------------
::itcl::body pxitclbasecontrol::SetStateOfWidget { widget state } {
    
    set children [ winfo children $widget ]
    set a [ llength $children ]
    if { $a == 0 } {
	catch { $widget configure -state $state  }
	return 0
    } else {
	for { set i 0 } { $i < $a } { incr i } {
	    SetStateOfWidget [ lindex $children $i ] $state
	}
    }
}
# ----------------------------------------------------------

::itcl::body pxitclbasecontrol::ConfigureWidgetTree { widget optionlist } {

    catch { eval "$widget configure $optionlist" }

    set children [ winfo children $widget ]
    set a [ llength $children ]
    if { $a == 0 } {
	catch { eval " $widget configure $optionlist" }
	return 0
    } else {
	for { set i 0 } { $i < $a } { incr i } {
	    ConfigureWidgetTree [ lindex $children $i ] $optionlist
	}
    }

}
# ----------------------------------------------------------
itcl::body pxitclbasecontrol::SetWidgetTreeColors { widget backc forec } {

    ::pxitclbasecontrol::ConfigureWidgetTree $widget "-background $backc"
    ::pxitclbasecontrol::ConfigureWidgetTree $widget "-foreground $forec"
    ::pxitclbasecontrol::ConfigureWidgetTree $widget "-highlightbackground $backc"
    ::pxitclbasecontrol::ConfigureWidgetTree $widget "-highlightcolor $forec"
}
# ----------------------------------------------------------
::itcl::body pxitclbasecontrol::SetStateOfUI { disablelist state } {
	
    set a [ llength $disablelist ]
    for { set i 0 } { $i < $a } { incr i } {
	SetStateOfWidget [ lindex $disablelist $i ] $state
    }
}
# ----------------------------------------------------------	
::itcl::body pxitclbasecontrol::DisableWidget { widget  }    { SetStateOfWidget $widget disabled }
::itcl::body pxitclbasecontrol::DisableUI     { widgetlist } { SetStateOfUI $widgetlist disabled }
::itcl::body pxitclbasecontrol::EnableWidget  { widget }     { SetStateOfWidget $widget normal   }
::itcl::body pxitclbasecontrol::EnableUI      { widgetlist } { SetStateOfUI $widgetlist normal   }

# ----------------------------------------------------------
::itcl::body pxitclbasecontrol::WatchOn { } {
    $basewidget config -cursor watch; 
    
    if { $parent != 0 } {
	$parent WatchOn
    }
}
# ----------------------------------------------------------
::itcl::body pxitclbasecontrol::WatchOff { } {

    $basewidget config -cursor ""
    ShowProgressVal "Done" 0.0
    if { $parent !=0 } {
	$parent WatchOff
	$parent ShowProgressVal "Done" 0.0
    }
}
# -------------------------------------------------------------------------------------------
#  
#    Progress Bar Stuff 
#
# -------------------------------------------------------------------------------------------
::itcl::body pxitclbasecontrol::AddLogoLabel { w } {

    global tcl_platform

    set l [ label $w.[ pxvtable::vnewobj ] -image  $::pxtclvtkpxcontrib::smalllogo ]

    return $l
}

::itcl::body pxitclbasecontrol::CreateProgressBar { w } {
    
    #set l [ AddLogoLabel $w  ]
    #pack $l -side left -expand f -padx 1

    set progressbar($this) $w.pbar
    iwidgets::feedbackplus $w.pbar -steps 100  -barheight 10  -labelpos w -labeltext $progress_lab($this) 
    pack $w.pbar -side right -expand t -fill x

    #   $w configure -bg "\#8197c5" 
    #    [ $w.pbar component trough ] configure  -bg  "\#cccdf8"
    #    [ $w.pbar component percentage ] configure  -fg  black 
    


}
# ----------------------------------------------------------
::itcl::body pxitclbasecontrol::ProgressCallback { filter label } {

#    puts stderr "Progress Callback ($this) = [ $filter GetClassName ] $label"

    if { $progressbar($this) != 0 } {
	ShowProgress $filter $label
    }

    if { $parent !=0 } {
	$parent ProgressCallback $filter $label
    }
}
# ----------------------------------------------------------
::itcl::body pxitclbasecontrol::ShowProgress { filter lab } {
	
    if { $progressbar($this) == 0 } {
	return
    }
    
    set progress_val($this) 50
    catch { set progress_val($this)  [expr [$filter GetProgress]*100] }
    set progress_lab($this) $lab
    

 #   puts stderr "Progress ($this) = $progress_val($this), $progress_lab($this)"

    if {$lab == "Done" } { 
	set progress_val($this) 0
    }
    
    if { $debugprogress == 1 } {
	#	puts stdout "$lab $progress_val($this)"
    }

    $progressbar($this) configure  -labeltext $progress_lab($this)
    $progressbar($this) setval [ expr round($progress_val($this)) ]

    update idletasks
}
# ----------------------------------------------------------
::itcl::body pxitclbasecontrol::ShowProgressVal { name val } {
	
    if { $progressbar($this) == 0 } {
	return
    }
	
    set progress_val($this) [expr $val*100.0]
    set progress_lab($this) "$name"
	
    if {$name == "Done" } { 
	set progress_val($this) 0
    }
#    if { $debugprogress == 1 } {
#	puts stdout "$name $val"
#    }

    $progressbar($this) configure  -labeltext $progress_lab($this)
    $progressbar($this) setval [ expr round($progress_val($this)) ]

    update idletasks
}
# ----------------------------------------------------------
itcl::body pxitclbasecontrol::SetFilterCallbacks { filter comment } {
    
    set t [ itcl::scope $this ]

    eval "$filter AddObserver ProgressEvent { $t ProgressCallback $filter \"$comment\"}"
    eval "$filter AddObserver EndEvent      { $t ProgressCallback $filter Done}"
}
# ----------------------------------------------------------

itcl::body pxitclbasecontrol::CheckForUpdates { } {
    
    ::pxtclvtkpxcontrib::CheckForUpdates
}

# ----------------------------------------------------------
itcl::body pxitclbasecontrol::PrintDatabase { } {

    global env
    set fn ".bioimagesuite3"
    catch { set fn $env(BIODBASE) }
    set fn [ file join $env(HOME) $fn ]
    
    set connector [ vtkbisDatabase New ]
    $connector InitializeSQLite "$fn" 
    set ok [ $connector Connect ]
    pxtkconsole
    pxtkprint "Reading database from $fn status = $ok\n"

    if { $ok > 0 } {
	set dbase [ vtkbisFilenameTable New ]
	$dbase SetConnector $connector
	set line [ $dbase PrintTable ]
	set dbline [ split $line "\n" ]
	set dbline [ lsort  -ascii -index 0 $dbline ]
	for { set i 0 } { $i < [ llength $dbline ] } { incr i } {
	    pxtkprint "[lindex $dbline $i ]\n"
	}
	$dbase Delete
    }
    $connector Delete
    return 

}
# ----------------------------------------------------------
itcl::body pxitclbasecontrol::AboutCommand { } {

    if { $aboutdialog == 0 } {
	
	set w .
	if { [ string length $basewidget ] > 2 } { set w $basewidget }
    
	set aboutdialog  [ toplevel $w.[ pxvtable::vnewobj ]] 
	
    
	::pxtclhelp::HelpDialog $aboutdialog
	wm title $aboutdialog "About This Application"
    
	set extra ""

	set opt [ vtkpxOptimizer [ pxvtable::vnewobj ]]
	if { [ $opt UsesNumericalRecipes ] == 1 } {
	    set extra "Uses a small amount of code from Numerical Recipes in C\n"
	}
	$opt Delete

	set line "$appname\nVersion: $version\n$aboutstring \n$extra\n\n------------------------------------\n System Information\n------------------------------------\n[ ::pxtclutil::GetAllInfo ]"
	                                                
	::pxtclhelp::HelpDialogAddText $aboutdialog $line

	set logoframe [ ::pxtclhelp::GetLogoFrame  $aboutdialog ]
	set imgname [ file join $::pxtclvtkpxcontrib::imagedir  bioimagesuite_logo.gif ]

	label $logoframe.b -image $::pxtclvtkpxcontrib::logo
	pack  $logoframe.b -side left -expand t -fill x 

 	set textbox [  ::pxtclhelp::GetTextBox  $aboutdialog ]

	$textbox configure -font "$::pxtclvtkpxcontrib::fontname"
	::pxitclbasecontrol::SetWidgetTreeColors $aboutdialog "\#8197c5" white
    } 

    ::pxtclutil::ShowWindow $aboutdialog

    #    option clear
    #    global pxtcl_pref_array
    #    set pxtcl_pref_array(ColorScheme)  "Suite Blue"
    #    ::pxtcluserprefs::InitializeBioImageSuiteColorScheme

}

itcl::body pxitclbasecontrol::ExitCommand { } {
    set ok [ tk_messageBox -title "Exiting this Application" -type yesno  -message "Are you sure you want to close $appname. Any unsaved work will be lost."  ]
    if { $ok == "yes" } {
	pxtkexit
    }
    return 0
}


# ------------------------------------------------------------------------------------

itcl::body pxitclbasecontrol::HelpCommand { } {

    if { $helpdialog != 0 } {
	::pxtclutil::ShowWindow $helpdialog
	return
    }

    set bname [ file normalize [ file join $::pxtclvtkpxcontrib::baselibrarypath  .. ] ]

    set b(1) [ file join $bname html ]
    set b(2) [ file join [ file join $bname .. ] build/html ]
    set b(3) [ file join [ file join $bname .. ] build64/html ]

    set i 1
    set rootname 0
    while { $i < 3 } {
	set f [ file normalize [ file join $b($i) index.html ] ]
#	puts stdout "Looking for $f"
	if { [ file exists $f ] > 0 } {
	    set i 4
	    set rootname $f
	}
	incr i
    }
    
    set w .
    if { [ string length $basewidget ] > 2 } { set w $basewidget }
    
    set helpdialog  [ toplevel $w.[ pxvtable::vnewobj ]] 
    wm geometry $helpdialog 800x600
    
    wm title $helpdialog "BioImage Suite Tools Manual Pages"
    
    set scr [ iwidgets::scrolledhtml $helpdialog.t  -labeltext "" \
		  -width 6i -height 3i \
		  -wrap word -padx 2 ]
    $scr configure -fontsize large
    
    pack $scr -side top -expand true -fill both

       
    if { $rootname != 0 } {
	$scr import $rootname
    } else {
	$scr insert end "No Man Pages Found"
    }
    
    ::pxtclutil::ShowWindow $helpdialog
}

