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




# 	$Id: pxitclfilelistmenu.tcl,v 1.3 2005/06/22 13:01:09 xenios Exp $	

package provide pxitclfilelistmenu 1.0


# -----------------------
# Dependencies and path
# -----------------------
package require vtkpxcontrib  1.1
package require pxtclutil 1.0
package require pxtcluserprefs 1.0
package require pxitclbasecontrol 1.0
package require pxitcllistselector 1.0
# -----------------------------------------------------
#  Intialize Variables
# -----------------------------------------------------

itcl::class pxitclfilelistmenu {

    # menubase is new menu
    protected variable menubase      0

    # variable for editing list
    protected variable item_selector      0

    # parent is parent menu
    protected variable parent       0

    # suffix,suffixname specify the type of file to be added
    public variable    suffix      { .hdr .hdr.gz .nii .nii.gz }
    public variable    suffixname  "Image Files"

    # filename is the current log file
    public    variable filename      ""

    # callback is the name of the command to execute (i.e. $callback $fname)
    protected variable callback      0
    protected variable addcurrentnamecallback      0

    # menuname is the actual label of the menu
    protected variable menuname    "menu"

    # filelist is a list of lists each having a key,value pair
    protected variable filelist    ""
    
    # if this is 1 then an add button is also added to the menu
    protected variable enableadd    1

    # flag to use sqlite vs file
    public variable usedb 1
    protected variable dbfilename ""
    protected variable dbase 0 
    protected variable typename ""
    protected variable initialized 0

    # ------------------------------------------------------

    constructor { par name callbackname {  currentnameclb 0 } } {
	set parent  $par
	set menuname $name
	set callback $callbackname
	set addcurrentnamecallback $currentnameclb
    }

    # Initialize Variables etc 
    public method InitializeDisplay {  }
    public method SetModeToImages { }
    public method SetModeToObjectmaps { }
    public method SetModeToDirectories { }
    public method SetModeToStandardImages { }
    public method SetModeToCustom { suffixname suffix filename { doadd 1 } }
    public method SetForceFileNames { } { if { $initialized == 0 } { set usedb 0 }}
    public method ReadData { fn }
    protected method ReadDataBase { tpname }
    protected method ReadFile { fname }
    public method AddFile { } 
    public method AddCurrent { } 
    protected method AddItem { newitem } 
    public method Info { }
    public method Import { }
    public method GetThisPointer { } { return $this }

    public method ButtonPressed  { value }
    public method AddMenuEntries { } 
    public method DeleteItems    { } 
    public method SaveAll    { } 
}

# ----------------------------------------------------------------------------------
itcl::body pxitclfilelistmenu::SetModeToImages { } {
    
    global env


    set    suffix      { .hdr .hdr.gz .nii .nii.gz }
    set suffixname "Image Files"
    
    set fn ".pximagelist"
    catch { set fn $env(BIODIRLIST) }
    set fn [ file join $env(HOME) $fn ]
    if { [ file exists $fn  ] == 0 } {
	set fn ".bioimagelist"
	catch { set fn $env(BIODIRLIST) }
	set fn [ file join $env(HOME) $fn ]
	if { [ file exists $fn ] == 0 } {
	    set fn ".pximagelist"
	}
    }

    set enableadd 1
    ReadData $fn
}


itcl::body pxitclfilelistmenu::SetModeToObjectmaps { } {
    
    global env


    set  suffix      { .hdr .hdr.gz .nii .nii.gz }
    set  suffixname "Objectmap Files"
    set fn ".bioobjectmaplist"
    set fn [ file join $env(HOME) $fn ]
    set enableadd 1
    ReadData $fn
}



itcl::body pxitclfilelistmenu::SetModeToStandardImages { } {

    global pxtcl_pref_array
    set    suffix      { .hdr .hdr.gz .nii .nii.gz }
    set suffixname "Image Files"
    set filename ""
    set enableadd 0
    set filelist ""
    
    set p1 [ file dirname [ info script ]]
    set p2 [ file normalize [ file join $p1 [ file join ".." images ] ] ]

    lappend filelist [ list "MNI T1 1mm"           "[ file join $p2 MNI_T1_1mm.nii.gz ]" ]
    lappend filelist [ list "MNI T1 1mm_stripped"  "[ file join $p2 MNI_T1_1mm_stripped.nii.gz ]" ]
    lappend filelist [ list "MNI T1 1mm_mask"      "[ file join $p2 MNI_T1_1mm_mask.nii.gz ]" ]
    lappend filelist [ list "MNI T1 1mm_graywhite" "[ file join $p2 MNI_T1_1mm_graywhite.nii.gz ]" ]
    lappend filelist [ list "" "" ]
    lappend filelist [ list "MNI T1 2mm"          "[ file join $p2 MNI_T1_2mm.nii.gz ]" ]
    lappend filelist [ list "MNI T1 2mm_stripped" "[ file join $p2 MNI_T1_2mm_stripped.nii.gz ]" ]
    lappend filelist [ list "MNI T1 2mm_mask"     "[ file join $p2 MNI_T1_2mm_mask.nii.gz ]" ]
    lappend filelist [ list "MNI T1 2mm_graywhite" "[ file join $p2 MNI_T1_2mm_graywhite.nii.gz ]" ]
    lappend filelist [ list "" "" ]
    lappend filelist [ list "avg152_NIFTI_Test"     "[ file join $p2 avg152T1_LR_nifti.nii.gz]" ]
    
}

itcl::body pxitclfilelistmenu::SetModeToDirectories { } {
    
    global env


    set suffix ""
    set suffixname "Directories"
    
    set fn ".biodirlist"
    catch { set fn $env(BIODIRLIST) }
    set fn [ file join $env(HOME) $fn ]

    set enableadd 1
    ReadData $fn
}

itcl::body pxitclfilelistmenu::SetModeToCustom { sfxname sfx fname { doadd 1 } } {
    set suffix $sfx
    set suffixname $sfxname
    set fn $fname
    set enableadd [ expr $doadd > 0 ]
    ReadData $fn
}


itcl::body pxitclfilelistmenu::Info { } {
    ::pxtclutil::Info "The contents of this menu are in the file $filename -- this can be edited with your favorite text editor (e.g. notepad/emacs/vi etc.)"
}

itcl::body pxitclfilelistmenu::Import { } {

    if { $usedb == 0 } { 
	return
    }

    global env
    set fn $typename
    catch { set fn $env(BIODIRLIST) }
    set fn [ file join $env(HOME) $fn ]
    ReadFile $fn

    if { $menubase !=0 } {
	$menubase delete 0 end
	$this AddMenuEntries
    }


}


itcl::body pxitclfilelistmenu::AddFile { } {

    if { $suffix == "" } {
	set dirname  [tk_chooseDirectory -title "Select Directory" ]
	if { [ string length $dirname ] < 1 } {
	    return 0
	}
	set dirname [ file normalize $dirname ]
	set n [ llength $filelist ]
	set newitem  [ list [ file tail $dirname ] $dirname ]
    } else {
	set typelist "{[ list  $suffixname  $suffix ]}"
	lappend typelist {"All Files" {*}}
	#puts stderr "Typelist = $typelist"
	set fname  [tk_getOpenFile -title "Specify new  Filename"  -filetypes $typelist ]
	if { [  string length $fname ] < 1 } {
	    return 0 
	}
	set newitem [ list [ file tail [ file root $fname ] ] $fname ]
    }

    return [ $this AddItem $newitem ]

}

itcl::body pxitclfilelistmenu::AddCurrent { } {

    set line "set fname \[ $addcurrentnamecallback \] "
    eval $line
    
    if { [ file exists $fname ] == 0 } {
	return 0
    }

    set newitem [ list [ file tail [ file root $fname ] ] $fname ]
#    puts stderr "Adding $newitem"

    return [ $this AddItem $newitem ]
}


itcl::body pxitclfilelistmenu::AddItem { newitem } {

    if { [ llength $newitem ] !=2 } {
	return 0
    }

    if { $usedb == 0 } {
	if { [ file exists  $filename ] == 0  } {
	    set fout [ open $filename w ]
	    if { $fout !=0 } {
		puts $fout "\#BioImage Suite File\n\# Format\n\# $suffixname : Full Filename\n\# e.g\n\# MyFile : myfile${suffix}"
		close $fout
	    }
	}

	if { [ file writable  $filename ] > 0 } {
	    pxtkprint  "adding [ lindex $newitem 0 ] : [ lindex $newitem 1 ] to $filename"
	    set fout [ open $filename a ]
	    puts $fout "[ lindex $newitem 0 ] : [ lindex $newitem 1 ]"
	    close $fout
	}  else {
	    pxtclutil::Warning "System File $filename is not writeable"
	}
    } else {
	
	$dbase AddItem $typename "[ lindex $newitem 0 ]" "[ lindex $newitem 1 ]"
    }

    
    set key [ lindex $newitem 0 ]
    set value [ lindex $newitem 1 ]
    lappend filelist $newitem

    eval "$menubase add command -label \"$key\" -command { $callback \"$value\" }"
    return 1
}




itcl::body pxitclfilelistmenu::SaveAll { } {

    if { $usedb == 1 } {
	$dbase DeleteAll $typename 
	for { set i 0 } { $i < [ llength $filelist ] } { incr i } {
	    set obj [ lindex $filelist $i ]
	    set key [ lindex $obj 0 ] 
	    set value [ lindex $obj 1 ] 
	    if { [ string length $key ] > 1 } {
		#		puts stderr "AddintItem $typename $key $value "
		$dbase AddItem $typename $key $value 
	    }
	}
	return
    }

    set fout [ open $filename w ]
    if { $fout !=0 } {
	puts $fout "\#BioImage Suite File\n\# Format\n\# $suffixname : Full Filename\n\# e.g\n\# MyFile : myfile${suffix}"
	for { set i 0 } { $i < [ llength $filelist ] } { incr i } {
	    set obj [ lindex $filelist $i ]
	    set key [ lindex $obj 0 ] 
	    set value [ lindex $obj 1 ] 
	    puts $fout "$key : $value"
	}
	close $fout
    } else {
	pxtclutil::Warning "System File $filename is not writeable"
    }
}
# -------------------------------------------------------------
itcl::body pxitclfilelistmenu::ReadDataBase { tpname } {

    if { $tpname == "" } {
	return 0
    }

    if { $dbase == 0 } {

	global env
	set fn ".bioimagesuite3"
	catch { set fn $env(BIODBASE) }
	set fn [ file join $env(HOME) $fn ]
	
	set connector [ vtkbisDatabase New ]
	$connector InitializeSQLite "$fn" 
	set ok [ $connector Connect ]

	#	puts stderr "Connector $fn , ok=$ok"

	if { $ok > 0 } {
	    set dbase [ vtkbisFilenameTable New ]
	    $dbase SetConnector $connector
	    $dbase CreateTable
	    #puts stdout "List=for $tpname =[ $dbase QueryAll $tpname ]\n\n"
	    set lst [ split [ $dbase QueryAll $tpname ] "|" ]
	    set typename $tpname
	    
	    set max [ expr [ llength $lst ] -1 ]

	    if { $tpname == "biodirlist" } {
		puts stdout "max=$max, lst=$lst"
	    }
	    if { [ llength $lst ] > 0 } {
		for { set i 0 } { $i < [ llength $lst ] } { incr i } {
		    set line [ lindex $lst $i ]
		    set lst2 [  split $line "," ]
		    if { [ llength $lst2 ] == 3 } {
			set key   [string trim   [ lindex $lst2 1 ] ]
			set value [string trim   [ lindex $lst2 2 ] ]
			lappend filelist [ list $key $value ]
		    } else {
			#		puts stdout "Ignoring $line ( [ llength $lst2 ])"
		    }
		}
	    } else {
		$this Import
	    }
	} else {
	    puts stderr "Failed to Connect to database"
	}
    }

#    puts stdout "Filelist=$filelist"
    return 1
}


# -------------------------------------------------------------------
itcl::body pxitclfilelistmenu::ReadData { fn } {

    if { $fn == "" } {	return 0  }
    set initialized 1

    if { $usedb == 1 } {
	return [ $this ReadDataBase [ file tail $fn ] ]
    } 
	
    return [ $this ReadFile $fn ]
}
# -------------------------------------------------------------------
itcl::body pxitclfilelistmenu::ReadFile { fn } {

    set filelist ""
    set filename $fn
    
    if { [ file exists $fn ] == 0 } {
	return 0
    }

    set fileid [open $fn r]
    set found 0


    
    while { [ gets $fileid line ] >=0 } {
	if { [  string range $line 0 0 ] == "\#" } {
	    
	} else {
	    set index [string first ":" $line]
	    
	    if { $index > 0 }  {
		set len   [string length $line]
		set key   [string trim   [string range $line 0 [expr $index - 1]]]
		set value [string trim [string range $line [expr $index +1] [expr $len -1]]]
		lappend filelist [ list $key $value ]
	    } elseif { $index == 0 } {
		lappend filelist [ list "" "" ]
	    }
	}
    }
    close $fileid


    if { $usedb == 1 } {
	$this SaveAll
    }

    return 1
}




itcl::body pxitclfilelistmenu::InitializeDisplay { } {

    set tmp $parent.[ pxvtable::vnewobj ]
    
    $parent add cascade -underline 0  -label $menuname -menu $tmp
    set menubase [ menu $tmp -tearoff 0  ]

    $this AddMenuEntries
}

itcl::body pxitclfilelistmenu::AddMenuEntries { } {

    set m2 $menubase
    set add 0

    if { $usedb == 0 } {
	if { [ string length $filename ] > 0 } {
	    eval "$m2 add command -label Info -command { $this Info } -background white"
	    incr add
	}
    } 
    #   else {
    #	eval "$m2 add command -label Import -command { $this Import } -background white"
    #   }

    if { $enableadd == 1 } {
	eval "$m2 add command -label Add -command { $this AddFile } -background white"
	if { $addcurrentnamecallback !=0 } {
	    eval "$m2 add command -label \"Add Current\" -command { $this AddCurrent } -background white"
	}
	eval "$m2 add command -label Edit -command { $this DeleteItems } -background white"
	incr add
    }
    if { $add > 0 } {
	$m2 add separator
    }
    
    for { set i 0 } { $i < [ llength $filelist ] } { incr i } {
	set obj [ lindex $filelist $i ]
	set key [ lindex $obj 0 ] 
	set value [ lindex $obj 1 ] 
	if { [ string length $key ] > 1 } {
	    if { [ file exists $value ] } {
		set cmd "$m2 add command -label \"$key\" -command { $this ButtonPressed \"$value\" }"
		eval $cmd
	    } 
	} else {
	    $m2 add separator
	}
    }
}

itcl::body pxitclfilelistmenu::ButtonPressed { value } {
    eval "$callback {$value} "
}

itcl::body pxitclfilelistmenu::DeleteItems { } {

    if { [ llength $filelist ] < 1 } {
	pxtclutil::Warning "You need to only have at least one item in this list before you can delete any!"
	return 
    }

    if { $item_selector == 0 } {
	set item_selector [ pxitcllistselector \#auto $menubase.[ pxvtable::vnewobj ] ]
    }
    
    set l ""
    set slist ""
    for { set i 0 } { $i < [ llength $filelist ] } { incr i } {
	set obj [ lindex $filelist $i ]
	set key [ lindex $obj 0 ] 
	set value [ lindex $obj 1 ] 
	lappend l "[expr $i+1 ]: $key : $value"
	lappend slist $i
    }
    
    $item_selector Activate $l $slist "Editing $suffixname list" "UNSELECT Items to Delete them from the list"
    set lst [ $item_selector GetSelectedIndices ]

    # Cancel Pressed
    if { [ $item_selector  GetLastState ] == 0 } {
	return
    }


    set oldlist $filelist

    set filelist ""
    for { set i 0 } { $i < [ llength $lst ] } { incr i } {
	set c [ lindex $lst $i ]
	set obj [ lindex $oldlist $c ]
	lappend filelist $obj
    }

    $this SaveAll
    $menubase delete 0 end
    $this AddMenuEntries
}

