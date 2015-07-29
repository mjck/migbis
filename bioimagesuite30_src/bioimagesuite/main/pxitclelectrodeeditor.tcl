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





package provide pxitclelectrodeeditor 1.0

# 	$Id: pxitclelectrodeeditor.tcl,v 1.2 2004/03/11 16:42:14 xenios Exp xenios $	

lappend auto_path [ file dirname [ info script ]]
lappend auto_path [file join [file join [ file dirname [ info script ]] ".."] base ]


package require vtkpxcontrib  1.1
package require pxitclbaseimageviewer 1.0
package require pxitclElectrodeMultiControl 1.0

itcl::class pxitclelectrodeeditor {

    inherit pxitclbaseimageviewer
    public  variable electrode_control 0

    constructor { par } {
	::pxitclbaseimageviewer::constructor $par
    } {
	set appname "BioImage Suite::Electrode Editor"

    }
    
    public method HandleMouseEvent { mouse stat x1 x2 widgetname args }
    public method InitializeElectrodeEditor { widget standalone}
    public method PositionWindows { }
}

itcl::body pxitclelectrodeeditor::InitializeElectrodeEditor { widget st } {

    set enable_landmarkcontrol 1
    set enable_surfaceobjectmap 0
    set enable_multisubjectcontrol 0
    set enable_csigadget 0
    set enable_fmrigadget 0
    set enable_vvlinkgadget  0
    set enable_talairachoption 0
    set enable_landmarkclicks  1
    set enable_overlaytool 0

    set ok [ InitializeOrthogonalViewer $widget $st ]
    set electrode_control [ [ pxitclElectrodeMultiControl \#auto $this ]  GetThisPointer ]
    $electrode_control Initialize $widget.[ pxvtable::vnewobj ] $vtk_viewer

    #    $csigadget configure -electrodetool $electrode_control

    set mb  $menubase.features 
    $mb add separator
    eval "$mb add command -label \"Electrode Control\" -command { $electrode_control ShowWindow; $this PositionWindows}"

    $electrode_control ShowWindow
    return $ok



}

itcl::body pxitclelectrodeeditor::HandleMouseEvent { mouse stat x1 x2 widgetname args} {
    
    if { $vtk_viewer == 0 } {
	return
    }

    # Need to flip y-axis vtk counts from bottom tk counts from top !!!
    set wd [ [ $widgetname GetRenderWindow ] GetSize ]
    set x2 [ expr [lindex $wd 1 ] - $x2 ]
    $vtk_viewer HandleMouseButtonEvent $mouse $stat $x1 $x2
    
    set lv "" 
    catch { set lv [ $vtk_viewer GetLastClickedPoint ] }
    set currentpoint $lv
    
    if { [ llength $lv ] > 1 } {
	if { [ lindex $lv 0 ] > -1 } {
	    
	    set lv [ $vtk_viewer GetLastClickedPoint ] 
	    set px [ lindex $lv 0 ]
	    set py [ lindex $lv 1 ]
	    set pz [ lindex $lv 2 ]
	    
	    if { $polydatacontrol != 0 } {
		[ $polydatacontrol GetPolyDataControl ] SetLastPoint $px $py $pz
	    }
	    
	    if { ( $mouse == 1 || $mouse == 4 ) && ( $electrode_control != 0 ) } {

		set mode 0
		if { $mouse == 4 || $thisparam($this,mousetolandmarks) == 0 }  {
		    set  mode 1
		}

		$electrode_control HandleClickedPoint $px $py $pz $mode $stat
	    }
	    	    
	    set upd [ $vtk_viewer GetMouseSliceChangeMode ]
	    if { $upd ==0 } {
		$vtk_viewer UpdateDisplay
	    }
	}
	
	if { $mousecallback != 0 } {
	    eval "$mousecallback $this"
	}
    }
}

itcl::body pxitclelectrodeeditor::PositionWindows {  } {
    
    set geom [ winfo geometry $basewidget ] 
    scan $geom "%dx%d+%d+%d" w0 h0 x0 y0

    set ew [ $electrode_control GetBaseWidget ]

    set geom [ winfo geometry $ew ] 
    scan $geom "%dx%d+%d+%d" w1 h1 x1 y1
    
    set y1 [ expr $y0 + 5 ]
    set x1 [ expr $x0 + $w0 + 10 ]
    set newgeom "${w1}x${h1}+$x1+$y1"
    wm geometry $ew $newgeom
    raise $ew
}


# ---------------------------------------------------------------------------------

if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {

    wm withdraw .
    set editor [ pxitclelectrodeeditor \#auto 0 ]
    $editor configure -show_standard_images 1
    $editor InitializeElectrodeEditor .[pxvtable::vnewobj ] 1
    $editor ShowWindow
    update

    $editor PositionWindows


    set argc [llength $argv]
    
    if { $argc > 0 } {   
	$editor LoadImage [lindex $argv 0]  
    }

    [ $editor GetViewer ]  AdjustViewports 6

    #if { [ $editor cget -enable_overlaytool ] == 1 } {
    #	if { $argc > 1 } {  [ $editor cget -electrode_control ] LoadElectrodeMultiGrid [lindex $argv 1] }
    #}
}






