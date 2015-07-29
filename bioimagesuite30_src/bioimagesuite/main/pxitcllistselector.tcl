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

package provide pxitcllistselector 1.0

# 	$Id: pxitcllistselector.tcl,v 1.2 2003/05/09 14:05:45 papad Exp papad $	






package require  Iwidgets 4.0.0


# ---------------------------------------------------------------------------
#  Ancilliary Class pxitclsubject
# ---------------------------------------------------------------------------
itcl::class pxitcllistselector {

    public variable basewidget    0 
    public variable rightspace    0
    public variable listbox       0
    public variable singleoutput  0
    protected variable laststate  0
    protected variable textbox    0
    protected variable buttonlist 0
    protected variable done       0
    protected variable selectallbutton 0

    constructor { widget } {
	Initialize $widget
    }

    destructor  { 

	wm destroy $basewidget
    }

    protected method Initialize { widget } 
    public method Cancel { } { set laststate  0 ; set done 1}
    public method OK { }   { set laststate  1  ; set done 1}
    public method Activate { listofobjects {defaultlist "" } { comment "" } { title "" } }
    public method SelectAll { }   { $listbox selection set 0 end }
    public method SelectNone { } {  $listbox selection clear 0 end }
    public method GetSelectedIndices { }
    public method SetSingleSelect { mode }
    public method GetLastState { } { return $laststate }
}

# ---------------------------------------------------------------------------
# Get Description
# ---------------------------------------------------------------------------
itcl::body pxitcllistselector::Initialize { widget  } {

    set basewidget $widget
    iwidgets::dialogshell $basewidget
    $basewidget configure -modality application
    
    set base [ $basewidget childsite ]

    set textbox [ text $base.top  -width 45 -height 6 -font { fixed 10 } ]
    set bot [ frame $base.bot ]
    pack $base.top -side top -expand f -fill x
    pack $bot -side bottom -expand true -fill both

    set listbox [ iwidgets::scrolledlistbox $bot.a -vscrollmode dynamic -hscrollmode none -selectmode multiple -labelpos nw -visibleitems 15x10 ]
    set rightspace [ frame  $bot.b ]
    pack $listbox $rightspace -side left -expand t -fill both -padx 1

    set selectallbutton [ eval "button $rightspace.all -text \"Select All\" -command { $this SelectAll }" ]
    eval "button $rightspace.none -text \"Select None\" -command { $this SelectNone }"
    pack $rightspace.all $rightspace.none -side bottom -expand false -fill x -pady 5 -padx 2

    set buttonlist [ list $rightspace.all $rightspace.none ]
    
    eval "$basewidget add ok -text OK -command { $this OK ; wm withdraw $basewidget }"
    eval "$basewidget add dismiss -text Cancel -command { $this Cancel ; wm withdraw $basewidget }"

    $basewidget default dismiss
    $basewidget configure -title "Selection List"
}

# ---------------------------------------------------------------------------

itcl::body pxitcllistselector::Activate { listofobjects {defaultlist "" } { comment "" } { title "" }  } {

    if { [ string length $title ] > 0 } {
	$basewidget configure -title $title
    }

    if { [ string length $comment ] > 0 } {
	$textbox delete 1.0 end
	$textbox insert end $comment
	$textbox see 1.0
    }

    $listbox delete 0 end

    for { set i 0 } { $i < [ llength $listofobjects ] } { incr i } {
	$listbox insert end [ lindex $listofobjects $i ]
    }

    # Fix for single output/multiple output
    if { $singleoutput == 1 } {
	set mode "single"
	set stmode "disabled"
    } else {
	set mode "multiple"
	set stmode "normal"
    }
    
    $listbox configure -selectmode $mode
    for { set i 0 } { $i < [ llength $buttonlist ] } { incr i }  {
	[ lindex $buttonlist $i ] configure -state $stmode
    }


    if { $defaultlist != "" } {
	for { set i 0 } { $i < [ llength $defaultlist ] } { incr i } {
	    set c [ lindex $defaultlist $i ]
	    $listbox selection set $c $c
	}
    }



    set done 0
    wm deiconify $basewidget
    tkwait variable [itcl::scope done ]
    if { $laststate == 0 } {
	$listbox selection clear 0 end
    }
	
    return [ $listbox getcurselection  ]
}

# ---------------------------------------------------------------------------

itcl::body pxitcllistselector::GetSelectedIndices { } {
    return [ [ $listbox component listbox ] curselection ]
}


