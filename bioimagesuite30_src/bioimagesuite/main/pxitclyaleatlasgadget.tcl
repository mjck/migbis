#!/bin/sh
# the next line restarts using wish \
    exec vtk "$0" "$@"

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




# This tool loads and interogates a custom atlas tool
#
# No part of the atlas software/images is integrated by BioImage Suite, it simply
# looks for the atlas files ( bioimagesuite_atlas.txt is the master file)
#

package provide pxitclyaleatlasgadget 1.0



lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]


package require  vtkpxcontrib 1.1
package require  labelframe   1.0
package require  pxtclutil    1.0
package require  pxvtable     1.0
package require  pxtclhelp    1.0
package require  Itcl         3.2
package require  Iwidgets     4.0
package require  pxitclimage            1.0
package require  pxitcltransform        1.0
package require  pxitclbaseimagecontrol 1.0

# ----------------------------------------------------------------------------------------------

itcl::class pxitclyaleatlasgadget {

    inherit pxitclbaseimagecontrol
    
    # Generic Stuff 

    private   common   thisparam        
    protected common   atlas_initialized 0
    protected common   atlasimages
    protected common   atlaslabels
    protected common   atlastext ""
    protected common   atlaspath ""
    protected common   numatlases 0
    protected common   atlasnotfound 0

    # public 

    
    #
    constructor { par args } {
	pxitclbaseimagecontrol::constructor $par
    } {
	InitializeYaleatlasgadget
    }

    protected method InitializeYaleatlasgadget { }
    public method Initialize { widget }
    public method DelayedInitialize { }

    public method AddToMenuButton { mb args }



    public method IdentifyPoint { x y z } 
    public method IdentifyInternalPoint { }

    public method LoadAtlas { }
    public method SetError { txt }

    #Not yet implemented
    public method FindAtlas { basedir imagelist }
    public method FindAtlasMasterPath {  }

}


::itcl::body pxitclyaleatlasgadget::AddToMenuButton { mb args} {

    eval "$mb add command -label \"Yale Brodmann Areas Atlas Tool\" -command {$this ShowWindow }"
}

itcl::body pxitclyaleatlasgadget::InitializeYaleatlasgadget { } {

    set thisparam($this,cx) 91
    set thisparam($this,cy) 110
    set thisparam($this,cz) 91
    set thisparam($this,textbox) 0
    set thisparam($this,aboutbox) 0
    set thisparam($this,atlaspath) ""
}

itcl::body pxitclyaleatlasgadget::Initialize { widget } { 
	
    set basewidget [ toplevel $widget ]
    wm geometry $basewidget 500x300
    wm withdraw $basewidget

    SetTitle "BioImage Suite Yale Atlas Tool"
    

    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget

}

itcl::body pxitclyaleatlasgadget::DelayedInitialize { } { 

    set pbar         [ frame $basewidget.pbar    -width 400 -height 20 ]
    set notebook $basewidget.notebook


    iwidgets::tabnotebook $notebook  -tabpos n

   set thisparam($this,atlaspath) [ label $basewidget.bot -text "Atlas not loaded." -bg black -fg white ]

    pack $pbar $basewidget.bot    -side bottom -expand f -fill x -pady 1
    pack $notebook -side top -expand t -fill both -pady 1

    set idframe    [ $notebook add -label "Identify" ]
    set aboutframe [ $notebook add -label "About" ]
    $notebook view "Identify"

    set f1 [ frame $idframe.t] ; 
    set f15 [ label $idframe.t2 -text "" -bg black -fg white ]

    pack $idframe.t $idframe.t2 -side top -expand false -fill x 
    $idframe.t2 configure -text "Coordinates are in mm (R->L,A->P,I->S),origin=bottom corner of image"
    $idframe.t2 configure -font { fixed 8}

    set f2 [ frame $idframe.b] ; pack $idframe.b -side bottom -expand true -fill both    


    label $f1.l -text "Point:"
    iwidgets::entryfield $f1.x -labeltext "X (mm):"  -width 5 -validate real -textvariable [ itcl::scope thisparam($this,cx) ] -relief sunken 
    iwidgets::entryfield $f1.y -labeltext "Y (mm):"  -width 5 -validate real -textvariable [ itcl::scope thisparam($this,cy) ] -relief sunken 
    iwidgets::entryfield $f1.z -labeltext "Z (mm):"  -width 5 -validate real -textvariable [ itcl::scope thisparam($this,cz) ] -relief sunken 
    eval "button $f1.b -text Identify -command { $this IdentifyInternalPoint }"
    pack $f1.b -side right -expand f -padx 2
    pack $f1.l $f1.x $f1.y $f1.z  -side left -expand true -fill x

    update idletasks

 
    for { set i 0 } { $i <= 1 } { incr i } {
	frame $f2.topframe    
	set textbox $f2.topframe.textbox; 
	set ybar    $f2.topframe.ybar 
	set xbar    $f2.xbar 
	
	eval "scrollbar $ybar  -command { $textbox yview } -orient vertical"
	eval "scrollbar $xbar  -command { $textbox xview } -orient horizontal"
	eval "text $textbox  -width 60 -height 10  -wrap word -relief ridge -yscrollcommand { $ybar set} -xscrollcommand {$xbar set}"
	pack $xbar -side bottom -expand f -fill x 
	pack $f2.topframe -side top -expand true -fill both -padx 0 -pady 5
	pack $ybar -side right -expand false -fill y
	pack $textbox -side left -expand true -fill both
	$textbox configure -font { fixed 10 } 
	if { $i == 0 } {
	    set thisparam($this,textbox) $textbox
	    set f2  $aboutframe
	} else {
	    set thisparam($this,aboutbox) $textbox
	}
    }

    ::pxitclbasecontrol::SetWidgetTreeColors $idframe.b  "\#ffccdd" "black"

    eval "button $pbar.r -text  Close -command {  $this HideWindow }"
    eval "button $pbar.l -text \"Load Atlas Files\" -command {  $this LoadAtlas }"
    pack $pbar.r  -side right -padx 10
    pack $pbar.l -side left -padx 10

    set canvas [ [ $notebook component tabset ] component canvas ];     
    set l [ AddLogoLabel $canvas ];     
    pack $l -side right -padx 1 


    set fullyinitialized 1

    if { $atlas_initialized == 1 } {
	# Just update the textboxes ...
	LoadAtlas
    }
    
}


::itcl::body pxitclyaleatlasgadget::LoadAtlas { } {

    if { $atlas_initialized == 1 } {
	set textbox $thisparam($this,aboutbox)
	if { $textbox != 0 } {
	    $textbox configure -state normal
	    $textbox delete 1.0 end
	    $textbox insert end $atlastext
	    $textbox configure -state disabled
	    $notebook view "About"
	    $thisparam($this,atlaspath) configure -text $atlaspath
	    update idletasks
	}
	return
    }


    set mfilename [ file normalize [ FindAtlasMasterPath ] ]

    if { $mfilename == "" } {
	set  atlasnotfound 1
	return 0
    }


    set basedir [ file dirname $mfilename ]
#    puts stderr "basedir = $basedir"

    set fileid [open $mfilename r]
    gets $fileid line
#    puts stderr "line = $line"
    if { [ string compare $line "#Atlas Description File"  ] != 0 } {
	close $fileid
	pxtclutil::Warning "Bad Atlas Description File $mfilename"
	set  atlasnotfound 1
	return 0
    }



    set imglist ""
    set imgname ""


    while { [ gets $fileid line ] >=0 } {
#	puts stderr "line = $line"
	if { [ string  first "#" $line ] != 0 } {
	    set index [string first ":" $line]
	    if { $index > 0 }  {
		set len   [string length $line]
		set key   [string trim   [string range $line 0 [expr $index - 1]]]
		set value [string trim [string range $line [expr $index +1] [expr $len -1]]]
#		puts stderr  "Parsing line $key $value"
		lappend imglist $value
		lappend imgname $key
	    }
	}
    }
    close $fileid
    
    set found 0
    if { [ llength $imglist ] > 0 } {
        set found [ FindAtlas $basedir $imglist ]
    }
    if { $found == 0 } {
	set  atlasnotfound 1
	return 0
    }
    set atlaspath "Atlas Location: [ file root [ file normalize $basedir]]"
    set numatlases [ llength $imgname ]

    pxtkprint "Loading Custom Yale Atlas\n"
    for { set i 0 } { $i < [ llength $imgname ] } { incr i } {
	set atlasimages($i) [  [ pxitclimage \#auto ] GetThisPointer ]
	set fname [ file join $basedir [ lindex $imglist $i ] ]
	$atlasimages($i) Load ${fname}.nii.gz
	pxtkprint "Loaded [ lindex $imgname $i ] : [ $atlasimages($i) GetShortDescription ]\t"

	set fname [ file join $basedir [ lindex $imglist $i ].txt ]
	set fileid [open $fname r]
	gets $fileid line
	set ind [ string first "\[" $line ]; if { $ind < 0 } { set ind -1 }
	set inp [ string first "\]" $line ]; if { $ind < 0 } { set inp [ string length $line ] }
	set line [ string trim [ string range $line [ expr $ind +1 ] [ expr $inp -1 ]]]
	set atlaslabels($i,description) $line
	pxtkprint ", $atlaslabels($i,description)\n"
	set atlaslabels($i,0) "None"
	set atlasimagenameslen   [string length $line]

	while { [ gets $fileid line ] >=0 } {
	    regsub -all "\t" $line ":" line
	    set index [string first ":" $line]
	    if { $index > 0 }  {
		set len   [string length $line]
		set key   [string trim   [string range $line 0 [expr $index - 1]]]
		set line [string trim [string range $line [expr $index +1] [expr $len -1]]]
		set index [string first ":" $line]
		if { $index > 0 } {
		    set len   [string length $line]
		    set value   [string trim   [string range $line 0 [expr $index - 1]]]
		} else {
		    set value $line
		}
		set atlaslabels($i,$key) $value
	    }
	}
	close $fileid
    }
    
    set atlastext ""
    set atlastext "$atlastext BioImage Suite relies on a custom atlas for labels.\n"
    set atlastext "$atlastext ------------------------------------------------------\n\n"
    
    set readme [ file join $basedir README.txt ]
    if { [ file exists $readme ] } {
	set fileid [open $readme r]
	while { [ gets $fileid line ] >=0 } {
	    set atlastext "$atlastext $line\n"
	}
	close $fileid
    } 

    set atlas_initialized 1

    set textbox $thisparam($this,aboutbox)
    if { $textbox != 0 } {
	$textbox configure -state normal
	$textbox delete 1.0 end
	$textbox insert end $atlastext
	$textbox configure -state disabled
	$notebook view "About"
	$thisparam($this,atlaspath) configure -text $mfilename
	update idletasks
    }
    return
}

itcl::body pxitclyaleatlasgadget::IdentifyPoint { x y z }  {

    set vx 0;    catch { set vx [ expr  ($x) ] }
    set vy 0;    catch { set vy [ expr  ($y) ] }
    set vz 0;    catch { set vz [ expr  ($z) ] }
    set thisparam($this,cx) $vx
    set thisparam($this,cy) $vy
    set thisparam($this,cz) $vz

    if { $atlas_initialized == 0 } { 
	global pxtcl_pref_array
	
	set val $pxtcl_pref_array(YaleAtlasAutoInitialize)
	    if { $val == "Enabled" } {
		$this LoadAtlas
	    }
    }
    return [ $this IdentifyInternalPoint ]
}

itcl::body pxitclyaleatlasgadget::IdentifyInternalPoint { } {

    set textbox $thisparam($this,textbox) 
    if { $textbox == 0 } {
	return 0
    }

    $notebook view "Identify"
    if { $atlas_initialized == 0 } {
	set textbox $thisparam($this,textbox) 
	$textbox configure -state normal
	$textbox delete 1.0 end
	$textbox insert end "Atlas Not Initialized!\n"
	$textbox insert end "This tool requires that the Yale Atlas be installed\n"
	$textbox insert end "This is extenral software and not distributed with BioImage Suite\n"
	$textbox insert end "If it installed but cannot be found try setting the value of the\n environment variable YALEATLASDIR to point to the atlas location."
	$textbox configure -state disabled
	return 
    } 


    set namelist [ list cx cy cz ]
    set maxlist  [ list 180 216 180 ]
    for { set i 0 } { $i <=2 } { incr i } {
	set key [ lindex $namelist $i ]
	set max [ lindex $maxlist  $i ]
	if { $thisparam($this,$key) < 0 } {
	    set thisparam($this,$key) 0
	} elseif { $thisparam($this,$key) > $max } {
	    set thisparam($this,$key) $max

	}
    }

    set x [ expr round($thisparam($this,cx)) ]
    set y [ expr round($thisparam($this,cy)) ]
    set z [ expr round($thisparam($this,cz)) ]

    #    set x [ expr  90 - round($thisparam($this,cx)/2.0) ]
    #    set y [ expr 108 - round($thisparam($this,cy)/2.0) ]
    #    set z [ expr round($thisparam($this,cz)/2.0) ]

    $textbox configure -state normal
    $textbox delete 1.0 end

    for { set atlas 0 } { $atlas < $numatlases } { incr atlas } {
	set v [ expr round([ [ $atlasimages($atlas) GetImage ] GetScalarComponentAsDouble $x $y $z 0 ])]
	set lab "$atlaslabels($atlas,$v)"
	set txt [ format "%-15s  %-35s value=%d\n" "$atlaslabels($atlas,description):" $lab $v ]
	$textbox insert end $txt
#	puts stderr "text=*${txt}*"
    }

    $textbox insert end "\n(Original yale atlas location ijk=$x $y $z)\n"
    $textbox insert end "Please note that all this is approximate .....\n"

    $textbox configure -state disabled
}

itcl::body pxitclyaleatlasgadget::SetError { txt } { 

    set textbox $thisparam($this,textbox) 
    if { $textbox == 0 } {
	return 0
    }
    $notebook view "Identify"
    $textbox configure -state normal
    $textbox delete 1.0 end
    $textbox insert end $txt
    $textbox configure -state disabled
}

itcl::body pxitclyaleatlasgadget::FindAtlas { basedir imagelist } {

    set numfound 0
    for { set j 0 } { $j < [ llength $imagelist ] } { incr j } {
	set imgname [ file join $basedir [ lindex $imagelist $j].nii.gz ]
	#	puts stderr "Looking for $imgname"

	set ok [ file exists $imgname ] 
	set numfound [ expr $numfound + $ok ]
    }
    
    if { $numfound != [ llength $imagelist ] } {
	pxtclutil::Warning "Custom Atlas Not Found in the standard places\n Try Setting the value of the environment variable YALEATLASDIR to point to this."
	return 0
    }

    return 1
}

itcl::body pxitclyaleatlasgadget::FindAtlasMasterPath { } {

    global tcl_platform 
    global env
    set pathlist ""

    set a ""
    catch { set a $env(YALEATLASDIR) }
    if { $a != "" } {
	lappend pathlist $a
    }

    set base $::pxtclvtkpxcontrib::imagedir
    lappend pathlist [ file normalize $base ]
    set base [ file normalize "${base}/../../yale_atlas" ]
    lappend pathlist $base


    if { $tcl_platform(platform) == "windows" } { 
	lappend pathlist "c:/yale/yale_atlas"
    } else {
	lappend pathlist /usr/local/yale_atlas/ 
    }

    set found 0
    set index 0
    set output ""

    #    puts stdout "pathlist=$pathlist, base=$base"

    while { $index < [ llength $pathlist ] } {
	set testpath [ lindex $pathlist $index ]
	set numfound 0

	set fname [ file join $testpath bioimagesuite_atlas.txt ] 
#	puts stderr "Looking for $fname"
	set found [ file exists $fname ] 
	if { $found == 1 } {
	    set output $fname
	    set index [ llength $pathlist ]
	}
#	puts stderr "$fname , $found"
	incr index
    }

    if { $found == 0 } {
	pxtclutil::Warning "Yale Atlas Not Found in the standard places\n Looked in $pathlist\n Try Setting the value of the environment variable YALEATLASDIR to point to this."
    }

    return $output
}
