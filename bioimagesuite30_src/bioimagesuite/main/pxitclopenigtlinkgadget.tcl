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






package provide pxitclopenigtlinkgadget 1.0

# 	$Id: pxitclopenigtlinkgadget.tcl,v 1.4 2005/11/02 15:29:19 xenios Exp xenios $	

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
package require  pxtcluserprefs         1.0

# ----------------------------------------------------------------------------------------------

itcl::class pxitclopenigtlinkgadget {

    inherit pxitclbaseimagecontrol

    # Generic Stuff 
    private   common   thisparam        


    # Open IGT Link
    protected variable igtlconnector 0
    protected common   igtlconnected  0
    protected variable enable_igtl 1
    protected variable vtk_viewer 0
    protected variable igtlserver 0
    protected variable igtlserverloopactive 0
    protected variable igtllastimagetime -1
    protected variable igtllasttransformtime -1

    protected variable cursor_x 0
    protected variable cursor_y 0
    protected variable cursor_z 0
    protected variable cursor_xform 0
    protected variable serverstatus ""
    protected variable imagestatus  ""
    protected variable pointstatus  ""
    protected variable internalpoints 0
    # ------------------------------------------------------------------------------

    constructor { par args } {
	pxitclbaseimagecontrol::constructor $par 
    } {
	InitializeOpenIGTLinkGadget
    }

    private method InitializeOpenIGTLinkGadget { } 



    # Client Related
    # --------------
    public method OpenIGTLinkConnect { }
    public method OpenIGTLinkDisconnect { }
    public method OpenIGTLinkSendImage { }
    public method ClientEventLoop { }    

    # Server Related
    # --------------
    public method CreateServer { }

    public method ServerCallback { }
    public method ServerCallbackHandleData { mode }
    public method ViewerUpdate {  }
    public method CursorUpdate {  }
    public method ImageUpdate {  }
    public method ServerEventLoop { }

    # GUI Relate
    public method CreateServerControl { parent } 
    public method CreateClientControl { parent } 
    public method CreateAboutTab  { parent }
    public method AddToMenuButton { mb args } 
    public method Initialize { widget viewer }

}

# ---------------------------------------------------------------------------

itcl::body pxitclopenigtlinkgadget::InitializeOpenIGTLinkGadget { } {

    set internalpoints [ vtkPoints  [ pxvtable::vnewobj ] ]
    $internalpoints SetNumberOfPoints 1

    set thisparam($this,openigtlserver) localhost
    set thisparam($this,openigtlport)   18944
    set thisparam($this,openigtlstatus) "Disconnected"
    set thisparam($this,clientactivelist)   ""
    set thisparam($this,clientinactivelist) ""
    set thisparam($this,openigtlenablecursor)   0
    set thisparam($this,openigtlslicermode)   0
    set thisparam($this,openigtlslicerbugfix) 0
    set thisparam($this,openigtltooldelay) 500


    set thisparam($this,openigtlserverstatus) "Server Disabled"
    set thisparam($this,serverport)         18944
    set thisparam($this,serverdelay)        500
    set thisparam($this,serverenablecursor) 0
    set thisparam($this,serverenableimage) 0
    set thisparam($this,serverimagestatus) ""
    set thisparam($this,serverpointstatus) ""
    set thisparam($this,serverbase)        0

    set cursor_xform [ vtkTransform  [ pxvtable::vnewobj ] ]
}

# --------------------------------------------------------------------------
itcl::body pxitclopenigtlinkgadget::ClientEventLoop { } {

    #    puts stderr "In Client Event Loop"


    
    if { $igtlconnected == 0 } {
	set thisparam($this,openigtlenablecursor) 0
	return 0
    }

    if { [ $igtlconnector GetIsConnected ] == 0 } {
	#	puts stderr "We are not connected breaking connection .....................\n\n"
	$this OpenIGTLinkDisconnect
	return 0
    }

    if { $thisparam($this,openigtlenablecursor) == 0 } {
	return 0
    }

    set img [ $vtk_viewer GetImage ]
    if  { $img == "" } { return }
    
    set lv [ $vtk_viewer GetLastClickedPointScaled ] 
    set px [ lindex $lv 0 ]
    set py [ lindex $lv 1 ]
    set pz [ lindex $lv 2 ]

    $internalpoints SetPoint 0 $px $py $pz
    $igtlconnector SendToolLocation $internalpoints $img [ expr $thisparam($this,openigtlslicermode)  + $thisparam($this,openigtlslicerbugfix) ]
    eval "after $thisparam($this,openigtltooldelay) $this ClientEventLoop"
}
#--------------------------------------------------------------------------------------------
itcl::body pxitclopenigtlinkgadget::CreateServer { } { 

    if { $igtlserver == 0 } {
	set igtlserver [ vtkpxOpenIGTLinkServer  [ pxvtable::vnewobj ] ]
    } else {
	set act [ $igtlserver GetIsServerActive    ]
	if { $act == 1 } {
	    return 0
	}
    }

    set ok [ $igtlserver InitializeConnection $thisparam($this,serverport) ]

    if { $ok == 0 } {
	::pxtclutil::Warning "Failed to Create OpenIGTLink Server on port $thisparam($this,serverport)."
	set thisparam($this,serverenablecursor) 0
	set thisparam($this,serverenableimage) 0
	set thisparam($this,openigtlserverstatus) "Server Disabled"
	return 0
    }

    while { [ $igtlserver GetThreadActive ] == 0 } { 
	# puts stderr "Waiting for receiving thread to become active ... "
    }

    
    set igtllasttransformtime      [ $igtlserver GetLastReceivedTransformTime ] 
    set igtllastimagetime          [ $igtlserver GetLastReceivedImageTime ]


    if { $igtlserverloopactive == 0 } {
	set igtlserverloopactive 1
	$this ServerEventLoop
    }
}




# ---------------------------------------------------------------------
itcl::body pxitclopenigtlinkgadget::ServerEventLoop { } {

    set con [ $igtlserver GetIsServerConnected ]
    set act [ $igtlserver GetIsServerActive    ]

    if { $con == 1 } {
	set st " **** STATUS: Server connected"
    } elseif { $act ==1 } {
	set st "Port=$thisparam($this,serverport)"
    } else {
	set thisparam($this,serverenablecursor) 0
	set thisparam($this,serverenableimage) 0
	set st "Server Disabled"
    }
    
    set serverstatus "$st (img=$igtllastimagetime, tr=$igtllasttransformtime)"


    set thisparam($this,openigtlserverstatus) $serverstatus
    set thisparam($this,serverimagestatus)    $imagestatus
    set thisparam($this,serverpointstatus)    $pointstatus


    set dl 1000
    if { $thisparam($this,serverenablecursor) == 1 || 
	 $thisparam($this,serverenableimage) == 1 } {
	set dl $thisparam($this,serverdelay)
    }

    # If needed start this ....
    if { $con == 1 && $act == 1 } {
	$igtlserver BeginReceivingDataInThread
	$this ServerCallback
    }

    eval "after $dl $this ServerEventLoop"
    return     
}

# ---------------------------------------------------------------------

itcl::body pxitclopenigtlinkgadget::ViewerUpdate {  } {
#    puts stderr "Viewer Update"
    set upd [ $vtk_viewer GetMouseSliceChangeMode ]    
    if { $thisparam($this,serverenablecursor) == 1 && $upd == 1 } {
	$vtk_viewer SetScaledCoordinates $cursor_x $cursor_y $cursor_z 0
    }
}

itcl::body pxitclopenigtlinkgadget::CursorUpdate {  } {

    $this ServerCallbackHandleData 0
    $vtk_viewer SetScaledCoordinates $cursor_x $cursor_y $cursor_z 0
}

itcl::body pxitclopenigtlinkgadget::ImageUpdate {  } {

    if { [ $parent isa bis_viewer ] } {
	return [ $parent SetImage $currentresults ]
    }
    
    return [ $this SendResultsToParent ]

}

itcl::body pxitclopenigtlinkgadget::ServerCallback { } {

    if { $igtllastimagetime < [ $igtlserver GetLastReceivedImageTime ] } {
	$this ServerCallbackHandleData 1
    }
    
    if { $igtllasttransformtime < [ $igtlserver GetLastReceivedTransformTime ] } {
	$this ServerCallbackHandleData 0
    }
}



itcl::body pxitclopenigtlinkgadget::ServerCallbackHandleData { mode } {
    

    set pointstatus ""
    set status 0

    if { $mode == 0 } {
	set tr [ $igtlserver GetLastReceivedTransform ]

	$cursor_xform Identity;
	$cursor_xform Concatenate $tr

	set tx [ $tr GetPosition ]

	set cursor_x [ expr double([ lindex $tx 0 ])]
	set cursor_y [ expr double([ lindex $tx 1 ])]
	set cursor_z [ expr double([ lindex $tx 2 ])]
	set pointstatus  [ format "(%.2f %.2f %.2f)" $cursor_x $cursor_y $cursor_z ]
	set status 1
	set igtllasttransformtime  [ $igtlserver GetLastReceivedTransformTime ] 
	if { $thisparam($this,serverenablecursor) == 1 } {
	    $this ViewerUpdate
	}
    } elseif { $mode ==1 } {
	$currentresults ShallowCopyImage  [ $igtlserver GetLastReceivedImage ]
	$currentresults SetOrientationDirect 0
	$currentresults configure -filename "openigtlimage_[$igtlserver GetLastReceivedImageTime]"
	set imagestatus  [ $currentresults GetShortDescription ]
	set igtllastimagetime  [ $igtlserver GetLastReceivedImageTime ]
	set status 1
	if { $thisparam($this,serverenableimage) == 1 } {
	    $this ImageUpdate
	}
    }

    return $status
    
}


#--------------------------------------------------------------------------------------------
#
#          GUI Stuff
#
#--------------------------------------------------------------------------------------------


itcl::body pxitclopenigtlinkgadget::OpenIGTLinkConnect {  } {

    if { $igtlconnector == 0 } {
	set igtlconnector [ vtkpxOpenIGTLinkClient [ pxvtable::vnewobj ]]
    }

    set igtlconnected 0
    set ok [ $igtlconnector InitializeConnection $thisparam($this,openigtlserver) $thisparam($this,openigtlport) ]
    if { $ok == 1 } {

	set igtlconnected 1
	EnableUI $thisparam($this,clientactivelist)
	DisableUI $thisparam($this,clientinactivelist)
	set thisparam($this,openigtlstatus) "Connected to $thisparam($this,openigtlserver):$thisparam($this,openigtlport)"
	::pxtclutil::Info "OpenIGTLink Connection to $thisparam($this,openigtlserver) established."
    } else {
	::pxtclutil::Warning "OpenIGTLink Connection to $thisparam($this,openigtlserver) failed."
	set thisparam($this,openigtlstatus) "Disconnected"
	DisableUI $thisparam($this,clientactivelist) 
	EnableUI $thisparam($this,clientinactivelist)
    }
    

    

}
#--------------------------------------------------------------------------------------------
itcl::body pxitclopenigtlinkgadget::OpenIGTLinkDisconnect { } {

    if { $igtlconnector == 0 } {
	return 0
    }

    $igtlconnector CloseConnection
    set thisparam($this,openigtlenablecursor) 0
    DisableUI $thisparam($this,clientactivelist) 
    EnableUI $thisparam($this,clientinactivelist)
}


#--------------------------------------------------------------------------------------------
itcl::body pxitclopenigtlinkgadget::OpenIGTLinkSendImage { } {

    if { $igtlconnector != 0 && $igtlconnected == 1 } {
	
	if {  [ $igtlconnector GetIsConnected ] == 0 } {
	    $this OpenIGTLinkDisconnect
	    return
	}

	set img [ $vtk_viewer GetImage ]
	if { $img != "" } {
	    set ok [ $igtlconnector SendImage $img ]
	}
    } 
}
#--------------------------------------------------------------------------------------------
itcl::body pxitclopenigtlinkgadget::CreateAboutTab { parent }  {

    $parent configure -bg black
    set txt [ iwidgets::scrolledtext $parent.st -labeltext "About OpenIGTLink" -vscrollmode dynamic -hscrollmode dynamic -width 5i -height 2i ]
    pack $parent.st -side top -expand true -fill both -padx 5 -pady 5



}
#--------------------------------------------------------------------------------------------
itcl::body pxitclopenigtlinkgadget::CreateClientControl { parent }  {

    entry $parent.2 -width 30  -textvariable [ itcl::scope thisparam($this,openigtlstatus) ] -relief sunken  -bg black -fg white 
    pack $parent.2 -side bottom -expand true -fill x

    set top  [ LabelFrame:create $parent.a -text "OpenIGTL Server Definition"]
    set bot  [ LabelFrame:create $parent.b -text "Additional Operations"]
    pack $parent.a  $parent.b -side top -expand t -fill both -pady 2


    iwidgets::entryfield $top.0 -labeltext "OpenIGTL Sever :"  -width 20  -textvariable [ itcl::scope thisparam($this,openigtlserver) ] -relief sunken 
    iwidgets::entryfield $top.1 -labeltext "OpenIGTL Port  :"  -width 10  -textvariable [ itcl::scope thisparam($this,openigtlport) ] -relief sunken 
    pack $top.0 $top.1 -side top -expand false -fill x

    set w [ frame $top.mid ]; pack $top.mid -side top -expand false -fill x
    

    set thisparam($this,clientinactivelist)  [ eval "button $w.1  -text \"Connect\" -command { $this OpenIGTLinkConnect }" ]
    set thisparam($this,clientactivelist) [ eval "button $w.2  -text \"Disconnect\" -command { $this OpenIGTLinkDisconnect }" ]
    pack $w.1 $w.2   -side left -expand true -fill x

    lappend thisparam($this,clientinactivelist) $top.0 $top.1


    checkbutton $bot.o -variable [itcl::scope thisparam($this,openigtlenablecursor)]   -text "Enable OpenIGTLink Tool Link" 
    checkbutton $bot.o2 -variable [itcl::scope thisparam($this,openigtlslicermode)]   -text "LPS to RAS Conversion" 
    checkbutton $bot.o3 -variable [itcl::scope thisparam($this,openigtlslicerbugfix)]   -text "Slicer Centering Bug Fix" 


    eval "$bot.o configure -command { $this ClientEventLoop }"
    frame $bot.a

    label $bot.a.2 -text "Update (ms): "
    tk_optionMenu $bot.a.3 [ itcl::scope thisparam($this,openigtltooldelay) ]    50 100 200 500 1000 2000 5000 10000
    pack  $bot.a.2 $bot.a.3  -side left -expand true -fill x

    eval "button $bot.b -text \"Send Current Image\" -command { $this OpenIGTLinkSendImage }"

    pack $bot.o $bot.o2 $bot.o3 $bot.a $bot.b -side top -expand false -fill x

    lappend thisparam($this,clientactivelist) $bot
    DisableUI $thisparam($this,clientactivelist) 
    EnableUI $thisparam($this,clientinactivelist)

}

#--------------------------------------------------------------------------------------------
itcl::body pxitclopenigtlinkgadget::CreateServerControl { parent }  {

    entry $parent.22 -width 30  -textvariable [ itcl::scope thisparam($this,serverpointstatus) ] -relief sunken  -bg black -fg white
    entry $parent.21 -width 30  -textvariable [ itcl::scope thisparam($this,serverimagestatus) ] -relief sunken  -bg black -fg white
    entry $parent.2 -width 30  -textvariable [ itcl::scope thisparam($this,openigtlserverstatus) ] -relief sunken  -bg black -fg white
    pack $parent.2 $parent.21 $parent.22 -side bottom  -expand false -fill x

    set top  [ LabelFrame:create $parent.a -text "OpenIGTL Server Definition"]
    set bot  [ LabelFrame:create $parent.b -text "Additional Operations"]
    pack $parent.a  $parent.b -side top -expand t -fill both -pady 2

    iwidgets::entryfield $top.1 -labeltext "Port to Listen On  :"  -width 10  -textvariable [ itcl::scope thisparam($this,serverport) ] -relief sunken 
    pack $top.1 -side top -expand false -fill x

    set w [ frame $top.mid ]; pack $top.mid -side top -expand false -fill x
    

    lappend thisparam($this,disconnectlist)    [ eval "button $w.1  -text \"Enable Server\" -command { $this CreateServer }" ]

    pack $w.1    -side left -expand true -fill x

    frame $bot.o
    label $bot.o.2 -text "Update (ms): "
    tk_optionMenu $bot.o.3 [ itcl::scope thisparam($this,serverdelay) ] 1 10 50 100  200 500 1000 2000 5000 10000
    pack  $bot.o.2 $bot.o.3  -side left -expand true -fill x

    checkbutton $bot.a -variable [itcl::scope thisparam($this,serverenablecursor)]   -text "Enable Cursor Link"
    checkbutton $bot.b -variable [itcl::scope thisparam($this,serverenableimage)]   -text "Enable Image Link (with care)!"
    #    eval "$bot.a configure -command { $this ServerEventLoop }"

    frame $bot.c

    eval "button $bot.c.0  -text \"Update Viewer Image\" -command { $this ImageUpdate }" 
    eval "button $bot.c.1  -text \"Update Cursor\" -command { $this CursorUpdate }" 

    pack $bot.c.1 $bot.c.0 -side left -expand false -fill x

    pack $bot.o $bot.a $bot.b  $bot.c -side top -expand false -fill x

}

#--------------------------------------------------------------------------------------------
# Main GUI Body 
# ---------------------------------------------------------------------------

itcl::body pxitclopenigtlinkgadget::Initialize { widget viewer } { 
	
    global pxtcl_pref_array

    set basewidget [ toplevel $widget ]
    wm geometry $basewidget 400x400
    wm withdraw $basewidget

    set vtk_viewer $viewer 
    
    set notebook $basewidget.notebook
    set bbar    [ frame $basewidget.bot    -width 300 -height 50 ]

    iwidgets::tabnotebook $notebook  -tabpos w
    pack $bbar -side bottom -expand f -fill x -pady 2 -pady 2
    pack $notebook -side top -expand t -fill both -pady 2 -padx 2
    eval "button $bbar.1  -text \"Close\" -command {  $this HideWindow }"
    pack $bbar.1 -side right -expand f -padx 10
    

    CreateServerControl             [ $notebook add -label "Server"  ] 
    CreateClientControl             [ $notebook add -label "Client"  ] 
    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"

    set canvas [ [ $notebook component tabset ] component canvas ]
    set l [ AddLogoLabel $canvas ]
    pack $l -side bottom -padx 1 


    SetTitle "Open IGTLINK Tool"
    $basewidget configure -bg black
    return $basewidget

}

::itcl::body pxitclopenigtlinkgadget::AddToMenuButton { mb args} {

    eval "$mb add command -label \"Open IGT Server\" -command {$this ShowWindow \"Server\"}"
    eval "$mb add command -label \"Open IGT Client\" -command {$this ShowWindow \"Client\"}"
}


# ---------------------------------------------------------------------------


