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




package provide pxitclvvlinkgadget 1.0

# 	$Id: pxitclvvlinkgadget.tcl,v 1.4 2005/11/02 15:29:19 xenios Exp xenios $	

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
package require  pxitclvvlinkprojectorgadget 1.0
package require pxitclLandmarkControl    1.0
package require pxitclPolyDataControl    1.0


# ----------------------------------------------------------------------------------------------

itcl::class pxitclvvlinkgadget {

    inherit pxitclbaseimagecontrol

    # Generic Stuff 
    private   common   thisparam        
    protected variable setupfilename   ""

    # Other Common Stuff
    protected common   vvlink_channel 0
    protected common   vvlink_connected  0
    protected common   vvlink_imagelist  ""

    # Various
    protected variable vvlink_transform  0
    protected variable vtk_viewer 0
    protected variable vvl_connector 0
    protected variable igtlconnector 0
    protected common   igtlconnected  0

    protected variable oldtx 0
    protected variable oldty 0
    protected variable oldtz 0



    protected variable enable_igtl 1

    protected variable vvlink_ztouchpointlist ""
    protected variable vvlink_xformlist       ""
    protected variable vvlink_logfilename  "LogFile.log"

    protected variable img_control      0
    protected variable xform_control    0

    protected variable vv_pointersource 
    protected variable vv_pointermapper 
    protected variable vv_pointeractor  
    protected variable internalpoints
    protected variable internaltransform
    
    protected variable vvlink_projector 0

    protected variable advanced_gui 0
    protected variable conecolor { 0.2 0.8 0.0 }
    protected variable go
    protected variable oldCenter
    
    protected variable addserver_gui 0
    protected variable landmarkcontrol 0
    protected variable polydatacontrol 0

    # ------------------------------------------------------------------------------

    constructor { par args } {
	pxitclbaseimagecontrol::constructor $par 
    } {
	InitializeVVLinkGadget
    }

    private method InitializeVVLinkGadget { } 
    public method Initialize { widget viewer }


    public method VVLinkConnect {  } 
    public method VVLinkUpdateImageList { } 
    public method VVLinkDisconnect { } 
    public method VVLinkServerAdd { mode } 

    public method OpenIGTLinkConnect { }
    public method OpenIGTLinkDisconnect { }
    public method OpenIGTLinkSendTool { pts }
    public method OpenIGTLinkSendImage { }
    public method OpenIGTLinkSendCrossHairs { }


    public method VVLinkSendImage { mode } 
    public method VVLinkGetImage { index }
    public method VVLinkGetLabeledPointsOrTools { mode }
	public method VVLinkGetTrajectoryPointsOrTools { mode }
    public method VVLinkMoveCamera { bx by bz tx ty tz }
    public method VVLinkAcquireLandmark { }
    public method VVLinkGetLabeledPointsOrToolsAsSurface { mode }
    public method VVLinkGetToolCoordinates { mode } 
    public method VVLinkRotateCamera { cx cy cz } 

    public method VVLinkSaveLabeledPointsOrTools { mode args }
    public method AddToMenuButton { mb args } 

    public method SelectedAxial { } { set thisparam($this,flipfront) 0 ; set thisparam($this,fliptop) 1 ;  set thisparam($this,flipleft) 1 }
    public method SelectedCoronal { } { set thisparam($this,flipfront) 1 ; set thisparam($this,fliptop) 0 ; set thisparam($this,flipleft) 1 }
    public method WarnAboutFlipLeft { }
    public method ShowAdvanced { }

    # ----------------------------
    # Setup Stuff
    # ----------------------------
    public method LoadSetup { args } 
    public method SaveSetup { args } 

    # ----------------------------
    #  Z-Touch Parser
    # ----------------------------
    public method ParseLogFile { filename  }
    public method ParseRegistrationIni { filename }
    public method RecoverZTouchSurface { }


    # ----------------------------
    # Strip Control
    # ----------------------------
    public method PredictElectrodeStrip { }

    # ---------------------------
    # GUI Stuff
    # ---------------------------

    public method CreateConfigurationControl { parent } 

    public method CreateServerControl { parent } 
    public method CreateOpenIGTLControl { parent } 
    public method CreateFeatureControl { parent } 
	public method CreateTrajectoryControl { parent }
    public method CreateZTouchControl { parent }
    public method CreateStripControl { parent }
    public method CreatePointerControl { parent }
    public method CreateDataSetControl { parent }
    public method CreateAboutControl { parent }

    public method PointerColor { }
    public method TipColor { index }
    public method EnablePointerControls {  } 
    public method ShowHideCone { }
    public method ShowControl { mode }
    public method ShowProjectorControl { }
    public method crossProduct { list1 list2 } 
    public method OnPoint { tip0 tip1 tip2 pt0 pt1 pt2 } 


    public method CreateLandmarkAndSurfaceControls { }
    public method GetLandmarkControl { }
    public method GetPolyDataControl { }
}

# ---------------------------------------------------------------------------

itcl::body pxitclvvlinkgadget::InitializeVVLinkGadget { } {

    global env

    set oldCenter(x) ""
    set oldCenter(y) ""

    set thisparam($this,openigtlserver) 192.168.0.22
    set thisparam($this,openigtlport)   18944
    set thisparam($this,openigtlstatus) "Disconnected"
    set thisparam($this,openigtlbase)   0
    set thisparam($this,openigtlenablecursor)   0
    set thisparam($this,openigtlslicermode)   1
    set thisparam($this,openigtlslicercentering) 1


    set thisparam($this,coneradius) 2
    set thisparam($this,conelength) 100
    set thisparam($this,coneextension) 0

    set vvl_connector [ vtkpxVVLinkConnectorNew [ pxvtable::vnewobj ]]

    set thisparam($this,streamdelay) 500
    set thisparam($this,tooldelay) 500
    set thisparam($this,showcross) 0
    set thisparam($this,cylinderenabled) 0
    set thisparam($this,crosslength) 3.0
    set thisparam($this,crossthickness) 0.3
    set thisparam($this,rollangle) 1.0
    set thisparam($this,showsphere) 0    
    set thisparam($this,updateviewer) 1
    set thisparam($this,vvlinkserver) "hostip:password"
    set thisparam($this,listoftrajectories) ""
    set thisparam($this,servermenu) ""
    set fn ".vvlinkservers"
    catch { set fn $env(VVLINKLIST) }
    set setupfilename [ file join $env(HOME) $fn ]
    set thisparam(thisparam,connectlist) ""
    set thisparam(thisparam,disconnectlist) ""

    set thisparam($this,vvlink_keepsendingimage)  0
    set thisparam($this,vvlink_keepgettingcursor) 0
    set thisparam($this,orientation) -1
    set thisparam($this,flipleft)   1
    set thisparam($this,flipfront)  0
    set thisparam($this,fliptop)    1
    set thisparam($this,forceaxial) 1

    set thisparam($this,samplerate) 2


    set thisparam($this,stripstep)       5.0
    set thisparam($this,striplength)    50.0
    set thisparam($this,stripsmoothness) 0.5

    set thisparam($this,stripbrainsurface) 1
    set thisparam($this,stripoutput)       2


    set thisparam($this,striplandmarkoffset)   -1

    set thisparam($this,inline) 0

    set internalpoints [ vtkPoints  [ pxvtable::vnewobj ] ]
    $internalpoints SetNumberOfPoints 2
    set internaltransform [ vtkTransform  [ pxvtable::vnewobj ] ]



}
#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::CreateLandmarkAndSurfaceControls { } {

    puts stdout "Creating landmark and surface controls"

    set landmarkcontrol [ [ pxitclLandmarkControl \#auto $this ]  GetThisPointer ]
    $landmarkcontrol Initialize $basewidget $vtk_viewer

    set polydatacontrol [ [ pxitclPolyDataControl \#auto $this ]  GetThisPointer ]
    $polydatacontrol Initialize $basewidget $vtk_viewer
    

}

itcl::body pxitclvvlinkgadget::GetLandmarkControl { } {

    if { $landmarkcontrol !=0 } {
	return $landmarkcontrol
    }

    catch {
	set landmarkcontrol [ $parent GetLandmarkControl ]
	return $landmarkcontrol
    }

    $this CreateLandmarkAndSurfaceControls
    return $landmarkcontrol

}

itcl::body pxitclvvlinkgadget::GetPolyDataControl { } {

    if { $polydatacontrol !=0 } {
	return $polydatacontrol
    }

    catch {
	set polydatacontrol [ $parent GetPolydataControl ]
	return $polydatacontrol
    }

    $this CreateLandmarkAndSurfaceControls
    return $polydatacontrol

}

#--------------------------------------------------------------------------------------------
#
#         VV Link Code
#
# ---------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::VVLinkConnect { }  {
    
    if { $vvlink_connected == 0 } {

	if { $vvl_connector== 0 } {
	    ::pxtclutil::Info "Cannot Connect to VVLink No vvl_connector specified\n" 
	    return 
	}
	
	set tmp  [ split $thisparam($this,vvlinkserver) : ]
	if { [ llength $tmp ] <=2  } {
	    set vvlink_server [ lindex $tmp 0 ]
	    set vvlink_password [ lindex $tmp 1 ]
	} else {
	    set vvlink_server [ lindex $tmp 1 ]
	    set vvlink_password [ lindex $tmp 2 ]
	}
    	
	set vvlink_channel [ pid ]

	set ok [ $vvl_connector InitializeVVLConnection $vvlink_server $vvlink_password $vvlink_channel ] 
	if  { $ok == 1 } {
	    ::pxtclutil::Info "VVLink Connection to $vvlink_server established.\nNext transfer an image to initialize the tool."
	    set vvlink_connected 1
	    incr vvlink_channel
	} elseif { $ok == 2 } {
	    ::pxtclutil::Info "VVLink Connection to $vvlink_server already established.\nNext transfer an image to initialize the tool."
	    incr vvlink_channel
	} else {
	    ::pxtclutil::Warning "VVLink Connection to $vvlink_server Failed!"
	    $vvl_connector Delete
	    set vvl_connector [ vtkpxVVLinkConnectorNew [ pxvtable::vnewobj ]]
	    return 0
	}
		
	set namelist  [ $vvl_connector GetVVLinkImageNameList  ]
	set length    [ string length namelist ]
	set namelist  [ string range $namelist 0 end-1 ]

	set vvlink_imagelist  [ split $namelist , ]
	$xform_control ClearObject
	set vvlink_transform 0
    } else {
	set tmp  [ split $thisparam($this,vvlinkserver) : ]
	set vvlink_server [ lindex $tmp 0 ]
	set vvlink_password [ lindex $tmp 1 ]
	set ok [ $vvl_connector InitializeVVLConnection $vvlink_server $vvlink_password $vvlink_channel ] 
	$xform_control ClearObject
	set vvlink_transform 0
    }
    

    if { $vvlink_connected == 1 } {

	DisableUI  $thisparam($this,connectlist)
	EnableUI $thisparam($this,disconnectlist)
	DisableUI $thisparam($this,streamonlist)
	DisableUI $thisparam($this,linkonlist)
	
	set num_trajectories [ $vvl_connector GetVVNumberOfTrajectories ] 
	puts "num_traj is $num_trajectories"
	
	set ok  [$vvl_connector GetVVNamesOfTrajectories $num_trajectories ]
	

	for { set i 0 } { $i < [ llength $vvlink_imagelist ] } { incr i } {
	    $thisparam($this,vvimagelist) insert end [ lindex $vvlink_imagelist $i ]
	}
	
	$thisparam($this,vvimagelist) selection clear 0 0
	$thisparam($this,vvimagelist) selection set   0 0


	SetTitle "VVLINK Tool: [ $vvl_connector GetVVLinkPatientName ], $vvlink_server, Channel: [ expr [ $vvl_connector GetVVLinkChannel ] +1 ]" 
	$basewidget configure -bg red
    }


    return 1
}

# ---------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::VVLinkUpdateImageList { } {
    if { $vvlink_connected == 0 } {
	return 0
    }

    set num [ $vvl_connector UpdateImageList ]
    set namelist  [ $vvl_connector GetVVLinkImageNameList  ]
    set length    [ string length namelist ]
    set namelist  [ string range $namelist 0 end-1 ]
    
    $thisparam($this,vvimagelist) delete 0 end
    $thisparam($this,vvimagelist) selection clear 0 0
    $thisparam($this,vvimagelist) selection set   0 0
    
    set vvlink_imagelist  [ split $namelist , ]
    for { set i 0 } { $i < [ llength $vvlink_imagelist ] } { incr i } {
	$thisparam($this,vvimagelist) insert end [ lindex $vvlink_imagelist $i ]
    }

    ::pxtclutil::Info "Image List Updated from Server (numimages=$num)\n"

}
# ---------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::VVLinkDisconnect { } {

    if { $vvlink_connected == 1 } {

	$vvl_connector CloseVVLConnection	
	set vvlink_channel 0
	set vvlink_connected  0
	set vvlink_imagelist  ""

	$thisparam($this,vvimagelist) delete 0 end

	EnableUI  $thisparam($this,connectlist)
	DisableUI $thisparam($this,disconnectlist)

	SetTitle "VVLINK Tool: Disconnected" 
	$basewidget configure -bg black

    }
}
# ---------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::VVLinkSendImage { mode } {

    if { $vtk_viewer == 0 } {
	return 0
    }

    if { $mode ==0 || $mode == 1 } {
	$vvl_connector SendViewerToVVLChannel [ [ $vtk_viewer GetRenderWidgetName ] GetRenderWindow ]
    }

    if { $mode == 1 } { 
	set thisparam($this,vvlink_keepsendingimage) 1
	DisableUI $thisparam($this,streamofflist)
	EnableUI $thisparam($this,streamonlist)
    } elseif { $mode == 2 } {
	set thisparam($this,vvlink_keepsendingimage) 0
	DisableUI $thisparam($this,streamonlist)
	EnableUI $thisparam($this,streamofflist)
    }

    if { $thisparam($this,vvlink_keepsendingimage) == 1 }  {
	eval "after $thisparam($this,streamdelay) $this VVLinkSendImage 0"
    }

}
# ---------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::VVLinkGetImage { index } { 

    if { $vvlink_connected == 0  || [ llength $vvlink_imagelist ] == 0 } {    
	return
    }

    if { $index == - 1 || $index >= [ llength $vvlink_imagelist ] } {
	set fname [ $thisparam($this,vvimagelist) getcurselection ]
	set index [ $thisparam($this,vvimagelist) curselection ]
    } else {
	set fname  [ lindex $vvlink_imagelist $index ]
    }

    #    puts stdout "Loading $fname as index $index\n"

    
    WatchOn
    $parent WatchOn
    set img [ vtkImageData [ pxvtable::vnewobj ]]
    set lut [ vtkLookupTable [ pxvtable::vnewobj ]]

    set tmp_transform [ vtkTransform [ pxvtable::vnewobj ]]

#    set ok [ $vvl_connector GetVVLinkImage $fname $img $tmp_transform $lut $thisparam($this,orientation) $thisparam($this,samplerate) ]

    set outputorient $thisparam($this,orientation)
    if { $thisparam($this,forceaxial) ==1 } {
	set outputorient 0
    }

    set ok [ $vvl_connector GetVVLinkImage $fname $img $tmp_transform $lut $thisparam($this,orientation) $thisparam($this,flipleft) $thisparam($this,flipfront) $thisparam($this,fliptop) $outputorient $thisparam($this,samplerate) ]
    
     if { $ok == 1 } {
	 set tmp    [  $img_control GetObject ]
	 $tmp ShallowCopyImage $img
	 $tmp configure -filename    $fname
	 $tmp CopyImageHeader $outputorient
	 #	 puts stderr "Transfered Image = [ $tmp GetLongDescription ]"
	 $img_control Update
	 $parent SetImageFromObject $tmp $this
	 $parent SetLookupTable $lut
	 
	 set tr [ $xform_control GetObject ]
	 $tr CopyTransformation $tmp_transform
	 $tr configure -filename "${fname}.matr"
	 $xform_control Update
	 set vvlink_transform [ $tr GetTransformation ]
	 set failed 0
     } else {
	 set failed 1
     }

    
    WatchOff
    $parent WatchOff

    if { $failed == 1 }  {
	::pxtclutil::Warning "Image Transfer of $fname Failed\n"
    }
    
}
# ---------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::VVLinkGetLabeledPointsOrTools { mode } {

    if { $vvlink_connected == 0  || $vvlink_transform == 0 } {    
	::pxtclutil::Warning "Either VVLink is not connected or current transform is not valid! (Most likely you need to transfer an image first.)"
	return 0
    }

    set  landmarkcontrol [ $this GetLandmarkControl ]
   
    if { $landmarkcontrol ==0 } {
	return
    }


    set curve [ vtkpxBaseCurve [ pxvtable::vnewobj ]]
    
    if { $mode == "tools" } {
	set sur      [ vtkPolyData [ pxvtable::vnewobj ] ]
	catch { set ok [ $vvl_connector GetVVLinkTrackedToolsAsPolyData $sur $vvlink_transform ] }
	$curve SetFromPoints [ $sur GetPoints ]
	set line " [ $sur GetNumberOfPoints ] Landmarks from the server"
	$sur Delete
    } else {
	set pts [ vtkPoints [ pxvtable::vnewobj ]]
	set ok [ $vvl_connector GetVVLabeledPoints $pts $vvlink_transform ]
        $curve SetFromPoints $pts
	set line " [ $pts GetNumberOfPoints ] Landmarks from the server"
	$pts Delete
    }

    set lcontrol [ $landmarkcontrol GetLandmarkControl ]
    $lcontrol SetClipboardCollection $curve 1

    ::pxtclutil::Info "Obtained $line. These are stored in the clipboard of the landmark control"
    

    $curve Delete

    set cur [ $lcontrol GetCollection -1 ]
    $cur Update

    $landmarkcontrol ShowWindow
    return 1
}

itcl::body pxitclvvlinkgadget::VVLinkGetTrajectoryPointsOrTools { mode } {
	

    if { $vvlink_connected == 0  || $vvlink_transform == 0 } {    
	::pxtclutil::Warning "Either VVLink is not connected or current transform is not valid! (Most likely you need to transfer an image first.)"
	return 0
    }

    set  landmarkcontrol [ $this GetLandmarkControl ]
   
    if { $landmarkcontrol ==0 } {
	return
    }

    set curve [ vtkpxBaseCurve [ pxvtable::vnewobj ]]
    

	set pts [ vtkPoints [ pxvtable::vnewobj ]]
	# set ok [ $vvl_connector GetVVTrajectoryPoints $pts $vvlink_transform ]
	puts "in gettrajectory 1"
	
	catch { set ok [ $vvl_connector GetVVTrajectoryPoints $pts $vvlink_transform ] }

        $curve SetFromPoints $pts
	
	puts "in gettrajectory 2"
	
	set line " [ $pts GetNumberOfPoints ] Landmarks from the server"
	$pts Delete


    set lcontrol [ $landmarkcontrol GetLandmarkControl ]
    $lcontrol SetClipboardCollection $curve 1

    ::pxtclutil::Info "Obtained $line. These are stored in the clipboard of the landmark control"

    $curve Delete

    set cur [ $lcontrol GetCollection -1 ]
    $cur Update

    $landmarkcontrol ShowWindow
    return 1
}

# -------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::ShowHideCone { } { 

    set a [ $vv_pointeractor(3) GetVisibility ]
    for { set i 0 } { $i <=3 } { incr i } {
	$vv_pointeractor($i) SetVisibility [ expr 1-$a ]
    }
    if { [ $vtk_viewer IsA "vtkpxGUIOrthogonalViewer" ] ==1 } {
	 $vtk_viewer UpdateDisplay
    }
}

::itcl::body pxitclvvlinkgadget::PointerColor { } {

    if { $vv_pointeractor(3) == 0 } {
	return
    }

    set c [ [ $vv_pointeractor(3) GetProperty ] GetColor ]
    set conecolor [ pxtclutil::ColorBox "Set Active Cone display color" $c $basewidget ]

    for { set i 0 } { $i <=3 } { incr i } {
	[ $vv_pointeractor($i) GetProperty ] SetColor [ lindex $conecolor 0 ] [ lindex $conecolor 1 ] [ lindex $conecolor 2 ]
	$vv_pointeractor($i) Modified
    }
    $vtk_viewer UpdateDisplay
}
# ---------------------------------------------------------------------------
::itcl::body pxitclvvlinkgadget::TipColor { index } {

    if { $vv_pointeractor($index) == 0 } {
	return
    }

    set c [ [ $vv_pointeractor($index) GetProperty ] GetColor ]
    set conecolor [ pxtclutil::ColorBox "Set Active Cone display color" $c $basewidget ]
    
    [ $vv_pointeractor($index) GetProperty ] SetColor [ lindex $conecolor 0 ] [ lindex $conecolor 1 ] [ lindex $conecolor 2 ]
    $vv_pointeractor($index) Modified

#    for { set i 0 } { $i <=3 } { incr i } {
#	[ $vv_pointeractor($i) GetProperty ] SetColor [ lindex $conecolor 0 ] [ lindex $conecolor 1 ] [ lindex $conecolor 2 ]
#	$vv_pointeractor($i) Modified
#    }
    $vtk_viewer UpdateDisplay
}

# ---------------------------------------------------------------------------
::itcl::body pxitclvvlinkgadget::EnablePointerControls {  } {

    set showcross_local $thisparam($this,showcross)
    if { $showcross_local == 1 } {
	EnableUI $thisparam($this,pointerlist)
    } else { 
    	DisableUI $thisparam($this,pointerlist)
    }    
}
# ---------------------------------------------------------------------------


itcl::body pxitclvvlinkgadget::VVLinkRotateCamera { cx cy cz } {

    if { $oldCenter(x) == "" || $oldCenter(y) == "" } {
	set oldCenter(x) $cx
	set oldCenter(y) $cy
    }
    
    set ren [ $vtk_viewer GetRenderer 3 ] 

    if { [ expr abs($oldCenter(x) - $cx) ] > 1  } {
	$ren Rotate [ expr $oldCenter(x) - $cx * .001 ] 0
    }

    if { [ expr abs($oldCenter(y) - $cy) ] > 1 } {
	$ren Rotate [ expr $oldCenter(x) - $cx * .001 ] 1
    }
    
    set oldCenter(y) $cy ; set oldCenter(x) $cx 
}
# ---------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::VVLinkMoveCamera { bx by bz tx ty tz } {


    set cam [ [ [ $vtk_viewer GetRenderer 3 ] GetRenderer ] GetActiveCamera ]
    set ren [ [ $vtk_viewer GetRenderer 3 ] GetRenderer ]
#     if { $sphere1 == 0 } {
# 	for { set i 1 } { $i <=2 } { incr i } {
# 	    set sphere($i) [ vtkSphereSource New ]
# 	    set map($i) [ vtkPolyDataMapper New ]
# 	    $map($i) SetInput [ $sphere($i) GetOutput ]
# 	    set act($i) [ vtkActor New ]
# 	    $act($i) SetMapper $map($i)
# 	    $ren AddActor $act($i)
# 	}
# 	[ $act(1) GetProperty ] SetColor 1 0 0
# 	[ $act(2) GetProperty ] SetColor 0 1 0
# 	set sphere1 $sphere(1)
# 	set sphere2 $sphere(2)
# 	$sphere1 SetRadius 2
# 	$sphere2 SetRadius 1
#     }
    
#     $sphere1 SetCenter $bx $by $bz
#     $sphere2 SetCenter $tx $ty $tz
    
#  get scalevalue 
   set rollangle_local $thisparam($this,rollangle)
   $cam SetRoll $rollangle_local  
		
   set diffx [ expr  pow($oldtx  - $tx, 2) ]
   set diffy [ expr  pow($oldty  - $ty, 2) ]
   set diffz [ expr  pow($oldtz  - $tz, 2) ]
    
   set tip_difference [ expr sqrt($diffx + $diffy + $diffz) ] 
    
   if { $tip_difference < 2.0 } {
   	return; 
   }
    
    set scalevalue [ $cam GetParallelScale ]
    $cam SetPosition $bx $by $bz
    $cam SetFocalPoint $tx $ty $tz
  

    if { $thisparam($this,inline) == 2 } {
	set dirvector [ $cam GetDirectionOfProjection ] 
	$cam Azimuth -45.0 
    }
    if { $thisparam($this,inline) == 3 } {
	set dirvector [ $cam GetDirectionOfProjection ] 
	$cam Azimuth 45.0 
    }

    set oldtx $tx 
    set oldty $ty 
    set oldtz $tz

     
    $ren ResetCamera

#  set scalevalue 
    $cam SetParallelScale $scalevalue
   
    #$cam SetFocalPoint [ expr $tip(0)+ [lindex $nrm 0]]  [ expr $tip(1) + [lindex $nrm 1]] [ expr $tip(2) + [lindex $nrm 2]] 
    

		

}
# ---------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::OnPoint { tip0 tip1 tip2 pt0 pt1 pt2 } {

    #puts "DIFF=-===[ expr $tip0 - $pt0 * 0.86 ] [ expr $tip1 - $pt1 * 0.86 ] [ expr $tip2 - $pt2 * 2]"
    set magnitude_pt [ expr sqrt(pow(($tip0 - $pt0 * 0.86),2) + pow(($tip1 - $pt1 * 0.86),2) + pow(($tip2 - $pt2 * 2),2)) ] 
    puts "Distance = $magnitude_pt"

    if { $magnitude_pt < 3  } {
	set go 1
	return 1
    } else {
	return 0 
    }
}

itcl::body pxitclvvlinkgadget::VVLinkGetToolCoordinates { mode } {

    if { $vvlink_connected == 0 || $vtk_viewer == 0  || $vvlink_transform == 0 } {    
	::pxtclutil::Warning "Either VVLink is not connected or current transform is not valid! (Most likely you need to transfer an image first.)"
	return 0
    }

    if { $mode == 1 } { 
	set thisparam($this,vvlink_keepgettingcursor) 1
	DisableUI $thisparam($this,linkofflist)
	EnableUI $thisparam($this,linkonlist)
	for { set i 0 } { $i <=3 } { incr i } {
	    [ $vv_pointeractor($i) GetProperty ] SetColor 0 1 0
	}
    } elseif { $mode == 2 } {
	set thisparam($this,vvlink_keepgettingcursor) 0
	DisableUI $thisparam($this,linkonlist)
	EnableUI $thisparam($this,linkofflist)
    } 

    if { $thisparam($this,vvlink_keepgettingcursor) ==0 } {
	for { set i 0 } { $i <=3 } { incr i } {
	    [ $vv_pointeractor($i) GetProperty ] SetColor 0.8 0.8 0.8
	}
    }
    
    if { $mode ==1 || $mode == 0 } {
	
	if { $vvlink_transform !=0 } {
	    $internaltransform Identity
	    $internaltransform Concatenate $vvlink_transform
	}
	
	if { $thisparam($this,coneextension) == "" } {
	    set thisparam($this,coneextension) 0
	}
	set ok [ $vvl_connector GetAndDrawVVLinkPointer $internalpoints $internaltransform $thisparam($this,conelength) $thisparam($this,coneradius) $thisparam($this,coneextension) $vv_pointersource(3) $vv_pointersource(4) $vv_pointersource(5) ]
	
	if { $ok ==1 } {
	
	    if { $thisparam($this,updateviewer) == 1 } {
 		
 		# Get vv_pointersource(3) values before changing it from cone to cylinder or vice versa. 
 		set cheight [ $vv_pointersource(3) GetHeight ] 
		set cradius [ $vv_pointersource(3) GetRadius ] 
		set ccenter [ $vv_pointersource(3) GetCenter ] 
		set cnormal [ $vv_pointersource(3) GetDirection ] 
  			
	    	if { $thisparam($this,cylinderenabled) == 1 } {
		    	set vv_pointersource(3) [ vtkbisCylinderSource [ pxvtable::vnewobj]]
	    	} else {
		    	set vv_pointersource(3) [ vtkConeSource [ pxvtable::vnewobj] ]
	    	}
	    	
	    	# Set parameters for Cone/Cylinder 
	    	$vv_pointersource(3) SetHeight $cheight
	    	$vv_pointersource(3) SetRadius $cradius
		$vv_pointersource(3) SetCenter [ lindex $ccenter 0 ] [ lindex $ccenter 1 ] [ lindex $ccenter 2 ]
	    	$vv_pointersource(3) SetDirection [ lindex $cnormal 0 ] [ lindex $cnormal 1 ] [ lindex $cnormal 2 ]
	    	$vv_pointersource(3) Update
	    	
	    	set pmapper [ $vv_pointeractor(3) GetMapper ]
		$pmapper SetInput [ $vv_pointersource(3) GetOutput ] 

		$vv_pointeractor(3) SetMapper $pmapper
		$vv_pointeractor(3)  VisibilityOn

		set cnt [ $vv_pointersource(3) GetCenter ]
		set len [ $vv_pointersource(3) GetHeight ]
		set nrm [ $vv_pointersource(3) GetDirection ]

		$vv_pointersource(4) Update
		$vv_pointeractor(4)  VisibilityOff

		set crosslength_local $thisparam($this,crosslength) 
		$vv_pointersource(5) SetLength  $crosslength_local
		set crossthickness_local $thisparam($this,crossthickness)
		$vv_pointersource(5) SetThickness  $crossthickness_local
		set showsphere_local $thisparam($this,showsphere)
		$vv_pointersource(5) SetShowSphere $showsphere_local 
		set showcross_local $thisparam($this,showcross)
		
		# [ $vv_pointeractor(5) GetProperty ] SetOpacity $thisparam($this,crossthickness)
		
		$vv_pointeractor(5) SetVisibility $showcross_local
		$vv_pointersource(5) Update

		set tip(0) "error"

		catch { 
		    for { set i 0 } { $i <=2 } { incr i } {
			set tip($i)  [ expr [ lindex $cnt $i ] + 0.5* $len * [ lindex $nrm $i ] ]
			set base($i)  [ expr $tip($i) - 1 * [ lindex $nrm $i ] ]
		    }
		}

		if { $tip(0) != "error" } {

		    # [ $vv_pointeractor($i) GetProperty ] SetColor 0 1 0

		    for { set i 0 } { $i <=2 } { incr i } {
			$vv_pointersource($i) SetPosition $tip(0) $tip(1) $tip(2)
			$vv_pointersource($i) SetLength [ expr 2.0*$thisparam($this,coneradius)]
			$vv_pointersource($i) Modified
			$vv_pointeractor($i)  VisibilityOn
			$vv_pointersource($i) Update
		    }
		    
		    if { [ $internalpoints GetNumberOfPoints ] >= 2 } {
			set px [ lindex [ $internalpoints GetPoint 0 ] 0 ]
			set py [ lindex [ $internalpoints GetPoint 0 ] 1 ]
			set pz [ lindex [ $internalpoints GetPoint 0 ] 2 ]
			catch { $vtk_viewer SetScaledCoordinates $px $py $pz 0 }
		    }
		}
		
				    
		if { $thisparam($this,inline) > 0 && $tip(0) != "error" } {
		    
		    #$this VVLinkRotateCamera [ lindex $cnt 0 ] [ lindex $cnt 1 ] [ lindex $cnt 2 ]
		    $this VVLinkMoveCamera $base(0) $base(1) $base(2) $tip(0) $tip(1) $tip(2) 
		}
            
        if { $vvlink_projector != 0 } {
            $vvlink_projector Update $tip(0) $tip(1) $tip(2) [lindex $nrm 0] [lindex $nrm 1] [lindex $nrm 2]
        }
		
		if { $igtlconnector != 0 && $igtlconnected == 1 } {
		    if { $thisparam($this,openigtlenablecursor) == 1 } {
			$this OpenIGTLinkSendTool $internalpoints
		    }
		}
	    }
	} else {
	    for { set i 0 } { $i <= 4 } { incr i } {
		$vv_pointeractor($i)  VisibilityOff
		$vtk_viewer UpdateDisplay
	    }

	}		
	
	if { $thisparam($this,vvlink_keepgettingcursor) == 1 }  {
	    #	puts stderr "after $thisparam($this,tooldelay) $this VVLinkGetToolCoordinates 0"
	    eval "after $thisparam($this,tooldelay) $this VVLinkGetToolCoordinates 0"
	}
    }
}

itcl::body pxitclvvlinkgadget::crossProduct { list1 list2 }  {

    set returnList [ list ]
    
    lappend returnList [ expr [ lindex $list1 1 ] * [ lindex $list2 2 ] - [ lindex $list1 2 ]  * [  lindex $list2 1 ] ]
    lappend returnList [ expr [ lindex $list1 2 ] * [ lindex $list2 0 ] - [ lindex $list1 0 ]  * [  lindex $list2 2 ] ]
    lappend returnList [ expr [ lindex $list1 0 ] * [ lindex $list2 1 ] - [ lindex $list1 1 ]  * [  lindex $list2 0 ] ]

    return $returnList
}

# ---------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::VVLinkAcquireLandmark { } {

    set  landmarkcontrol [ $this GetLandmarkControl ]

    if { $vvlink_connected == 0 || $vtk_viewer == 0  || $vvlink_transform == 0  || $landmarkcontrol == 0 } {    
	::pxtclutil::Warning "Either VVLink is not connected or current transform is not valid! (Most likely you need to transfer an image first.)"
	return 0
    }

    set m [ vtkTransform [ pxvtable::vnewobj ]]
    if { $vvlink_transform !=0 } {
	$m Identity;
	$m Concatenate $vvlink_transform
    }
	
    set pts [ vtkPoints [ pxvtable::vnewobj ]]

    set ok [ $vvl_connector GetVVLinkPointer $pts $m 1.0 ]
    
    if { [ $pts GetNumberOfPoints ] >= 2 } {
	set px [  lindex [  $pts  GetPoint 0 ] 0 ]
	set py [  lindex [  $pts  GetPoint 0 ] 1 ]
	set pz [  lindex [  $pts  GetPoint 0 ] 2 ]
	catch { $vtk_viewer SetScaledCoordinates $px $py $pz 0 }
	[ $landmarkcontrol GetLandmarkControl ]  HandleClickedPoint $px $py $pz 1 2
    }
    
    $pts Delete
    $m Delete
}

# ---------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::VVLinkSaveLabeledPointsOrTools { mode args } {

    if { $vvlink_connected == 0  || $vvlink_transform == 0 } {    
	::pxtclutil::Warning "Either VVLink is not connected or current transform is not valid! (Most likely you need to transfer an image first.)"
	return 0
    }


    set fname ""
    set len [ llength $args ]
    if { $len > 0 } {
	set fname [ lindex $args 0 ]
    }
    
    set name "Labeld Points"
    if { $mode == "tools" } {
	set name "Tracked Tools"
    }

    if { $fname == "auto" } {
	set fname [ clock format [ clock seconds ] -format "%H%M%S_%d%b%Y" ]
	set fname "tools_${fname}.txt"
    } elseif { [ string length $fname  ] == 0  } {
	set fname  [tk_getSaveFile -title "Saving $name in Text File"  -filetypes { {"Text Files" {*.txt}} { "All Files" {*.*}} } ]
    }
    
    if { [ string length $fname ] < 1 } {
	return 0
    }
    
    set ok 0
    if { $mode == "tools" } {
	set dt [ clock format [ clock seconds ] -format "%H %M %S on %d %b %Y" ]
	catch { set ok [ $vvl_connector SaveVVLinkTrackedTools $fname $vvlink_transform $dt ] }
    } else {
	catch { set ok [ $vvl_connector SaveVVLinkLabeledPoints $fname $vvlink_transform ] }
    }

    if { $ok == 0 } {
	::pxtclutil::Warning "Failed to save $name in $fname"
    } elseif { $mode == "tools" } {
	pxtkconsole
	pxtkprint "Tools Saved in $fname\n"
    }
    return $ok
}

# -------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::VVLinkGetLabeledPointsOrToolsAsSurface { mode } {

    if { $vvlink_connected == 0  || $vvlink_transform == 0 } {    
	::pxtclutil::Warning "Either VVLink is not connected or current transform is not valid! (Most likely you need to transfer an image first.)"
	return 0
    }

    set pts [ vtkPoints [ pxvtable::vnewobj ]]

    set ok [ $vvl_connector GetVVLabeledPoints $pts $vvlink_transform ]
    
    set  polydatacontrol [ $this GetPolyDataControl ]
    
    if { $polydatacontrol ==0 } {
	$pts Delete
	return
    }

    set pcontrol [ $polydatacontrol GetPolyDataControl ]
    set sur      [ vtkPolyData [ pxvtable::vnewobj ] ]

    if { $mode == "tools" } {
	catch { set ok [ $vvl_connector GetVVLinkTrackedToolsAsPolyData $sur $vvlink_transform ] }
    } else {
	catch { set ok [ $vvl_connector GetVVLinkLabeledPointsAsPolyData $sur $vvlink_transform ] }
    }

    [ $polydatacontrol GetPolyDataControl ]  SetClipboardSurface $sur "vvlink_${mode}.vtk"
    ::pxtclutil::Info "Obtained [ $sur GetNumberOfPoints ]. These are stored in the clipboard of the surface control"

    $polydatacontrol ShowWindow

    for { set i 0 } { $i < [ $sur GetNumberOfPoints ] } { incr i } {
	puts stderr "Point $i [ $sur GetPoint $i ]"
    }
    puts stderr "\n"

    $sur Delete
    return 1
}



# ---------------------------------------------------------------------------
# Load and Save Setup Files
# ---------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::SaveSetup { args } { 

    set len [ llength $args ]
    
    if { $len < 1 } {
	set fname $setupfilename 
    } else {
	set fname [ lindex $args 0 ]
    }


    if { [ string length $fname  ] == 0  } {
	set f1 $setupfilename
	set f1path [ file dirname $f1 ]
	set f1tail [ file tail    $f1 ]
	set fname  [tk_getSaveFile -title "Saving Setup File"  -initialfile $f1tail -initialdir $f1path ]
    }

    if { [ string length $fname ] > 0 } {

	set fileid [open $fname w]
	
	puts $fileid "#VVLINK Host Description File"
	puts $fileid "#---------------------------------------------"

	set len [ $thisparam($this,servermenu) index last ]
	#	puts stdout "Number of Servers [ expr $len+1 ]"
	for { set i 0 } { $i <= $len } { incr i } {
	    puts $fileid [ $thisparam($this,servermenu) entrycget $i -label ]
	}
	close $fileid

    }
}
# ------------------------------------------------------------------------------------------ 
itcl::body pxitclvvlinkgadget::VVLinkServerAdd { mode } {


    if { $addserver_gui == 0 && $mode != 0 } {
	return
    }

    if { $addserver_gui == 0 } {

	set thisparam($this,newserverdescription) "Server Description"
	set thisparam($this,newserverhostname) "localhost"
	set thisparam($this,newserverpassword) "111111"

	set addserver_gui  [ toplevel $basewidget.[ pxvtable::vnewobj ]]
	eval "wm protocol $addserver_gui WM_DELETE_WINDOW { wm withdraw $addserver_gui }"
    	
	wm title $addserver_gui "Define New VVLink Server"
	
	set w $addserver_gui

	iwidgets::entryfield $w.0 -labeltext "Description:"  -width 20  -textvariable [ itcl::scope thisparam($this,newserverdescription) ] -relief sunken 
	iwidgets::entryfield $w.1 -labeltext "Hostname:"  -width 20  -textvariable [ itcl::scope thisparam($this,newserverhostname) ] -relief sunken 
	iwidgets::entryfield $w.2 -labeltext "Password:"  -width 20  -textvariable [ itcl::scope thisparam($this,newserverpassword) ] -relief sunken 
	eval "button $w.3  -text \"Add Server\" -command { $this VVLinkServerAdd 1 }" 
	pack $w.3 -side bottom -expand true -fill x
	pack $w.0 $w.1 $w.2 -side top -expand true -fill x

    }

    if { $mode == 0 } {
	wm deiconify $addserver_gui
	return
    }

    set a $thisparam($this,newserverdescription) 
    set b $thisparam($this,newserverhostname)
    set c $thisparam($this,newserverpassword)

    wm withdraw $addserver_gui
    set line "${a}:${b}:${c}"
    $thisparam($this,servermenu) add radio  -label $line  -variable [ itcl::scope thisparam($this,vvlinkserver) ]
}

# ------------------------------------------------------------------------------------------ 
itcl::body pxitclvvlinkgadget::LoadSetup { args } { 

    set len [ llength $args ]
    
    if { $len < 1 } {
	set fname $setupfilename 
    } else {
	set fname [ lindex $args 0 ]
    }

    if { [ string length $fname  ] == 0 } {
	set f1 $setupfilename
	set f1path [ file dirname $f1 ]
	set f1tail [ file tail    $f1 ]
	
	set fname  [tk_getOpenFile -title "Setup File" -filetypes { {"VVLink-Setup File" {*}}} -initialfile $f1tail -initialdir $f1path ]
    }

    if { [ string length $fname ] < 1 } {
	return
    }

    if { [ file readable $fname ] == 0 } {
	::pxtclutil::Warning "Cannot read setupfile file from $fname\n Bad File"
	return 0
    }

    set fileid [open $fname r]
	
    gets $fileid line
    if { [ string compare $line "#VVLINK Host Description File"  ] != 0 } {
	close $fileid
	::pxtclutil::Warning "Cannot read setupfile file from $fname\n Bad File Type"
	return 0
    }

    set len [ $thisparam($this,servermenu) index last ]
    $thisparam($this,servermenu) delete 0 $len
    set count 0 
    while { [ gets $fileid line ] >=0 } {
	if { [  string range $line 0 0 ] != "\#" } {
	    $thisparam($this,servermenu) add radio  -label $line  -variable [ itcl::scope thisparam($this,vvlinkserver) ]
	    if { $count == 0 } {
		set thisparam($this,vvlinkserver) $line
	    }
	    incr count
	}
    }
    close $fileid
    set setupfilename $fname


}
#--------------------------------------------------------------------------------------------
#  Point Parser
#--------------------------------------------------------------------------------------------

itcl::body pxitclvvlinkgadget::ParseLogFile { filename } {

    if { $filename == "" } { set filename  [tk_getOpenFile -title "Log File" -filetypes { {"Log Files" {*.log}} { "All Files" {*.*}} } -initialdir [ file dirname $vvlink_logfilename] -initialfile [ file tail $vvlink_logfilename ] ] }
    if { [ string length $filename ] < 1 } { return 0  }
    if { [ file readable $filename ] == 0 } { return 0 }

    set header "ZTouch-point Registrar"
    set fileid [open $filename r]
    set vvlink_ztouchpointlost ""
    set count 0
    while { [ gets $fileid line ] >=0 } {
	set findex [ string first $header $line ]
	if { $findex >=0 } {
	    gets $fileid np
	    lappend vvlink_ztouchpointlost [ list $np $line $count ]
	    incr count
	}
	incr count
    }
    close $fileid

    if { [ llength $vvlink_ztouchpointlost ] == 0 } {
	::pxtclutil::Warning "No ZTouch Points Found In Log File $filename"
	return 0
    }

    set vvlink_logfilename $filename
    $thisparam($this,vvztouchpointlist)  delete 0 end

    for { set i 0 } { $i < [ llength $vvlink_ztouchpointlost ] } { incr i } {
	set m [ lindex $vvlink_ztouchpointlost $i ]
	#	puts stdout "Set $i: $m"
	$thisparam($this,vvztouchpointlist) insert end $m
    }

    $thisparam($this,vvztouchpointlist) selection clear 0 0
    $thisparam($this,vvztouchpointlist) selection set   0 0

     
    return 1
}

# ------------------------------------------------------------------------------------------

itcl::body pxitclvvlinkgadget::ParseRegistrationIni { filename  } {
    
#    if { $filename == "" } { set filename  [ tk_getOpenFile -title "Ini File" -filetypes { {"Ini Files" {*.ini}}} ] }

    if { $filename == "" } { set filename  [tk_getOpenFile -title "Registration Ini File" -filetypes { {"Ini Files" {*.ini}} { "All Files" {*.*}} } -initialdir [ file dirname $vvlink_logfilename] -initialfile Registration.ini ] }

    if { [ string length $filename ] < 1 }  {	return 0 }
    if { [ file readable $filename ] == 0 } {	return 0 }
    
    set header  "VVCranialENTztouch"
    set header2 "RefToWorldTransformation="
    set fileid [open $filename r]
    set vvlink_xformlist ""

    set foundend 0
    while { [ gets $fileid line ] >=0 && $foundend == 0 } {

	if { [ string first $header $line ] >=0 } {
	    set found2 0
	    while { [ gets $fileid line ] >=0 && $foundend == 0 } {
		if {  [ string first $header2 $line ] >=0 } {
		    
		    gets $fileid line2
		    gets $fileid line3
		    lappend vvlink_xformlist [ list $line3 $line2 $line ]
		    set found2 1
		}
	    }
	    
	    if { $found2 == 0 } {
		set foundend 1
	    }
	}
    }
    close $fileid

    $thisparam($this,vvztouchxformlist)  delete 0 end

    for { set i 0 } { $i < [ llength $vvlink_xformlist ] } { incr i } {
	set m [ lindex $vvlink_xformlist $i ]
	#	puts stdout "Set $i: $m"
	$thisparam($this,vvztouchxformlist) insert end $m
	# puts stdout "i=$i, xform=$m"
    }

    $thisparam($this,vvztouchxformlist) selection clear 0 0
    $thisparam($this,vvztouchxformlist) selection set   0 0

    return 1
}

itcl::body pxitclvvlinkgadget::RecoverZTouchSurface {  } {

    set pointname [ $thisparam($this,vvztouchpointlist) getcurselection ]
    set xformname [ $thisparam($this,vvztouchxformlist) getcurselection ]

    if { $pointname == "" || $xformname == "" } {
	::pxtclutil::Warning "Cannot reconstruct ztouch surface\n pointname=$pointname\n xformname=$xformname"
	return 0
    }

    set lineindex [ lindex $pointname 2 ]    
    pxtkconsole
    pxtkprint "\nReconstructing ztouch surface\n pointname=$pointname ($lineindex), xformname=$xformname\n"


    # Step 1 Get Points
    #


    set fileid [open $vvlink_logfilename r ]
    for { set i 0 } { $i <= $lineindex } { incr i } {
	gets $fileid line
    }
    
    set numpoints 0
    gets $fileid numpoints

    pxtkprint "line=$line, Numpoints = $numpoints"
    set pts [ vtkPoints [ pxvtable::vnewobj ]]
    $pts SetNumberOfPoints $numpoints
    for { set i 0 } { $i < $numpoints } { incr i } {
	gets $fileid line
	scan $line "%f %f %f %f %f %f" dx dy dz x y z
	$pts SetPoint $i $x $y $z
	pxtkprint "Point $i = ($x $y $z)\n"
    }
    close $fileid

    # Step 2 Form Transformation 
    set xformline [ lindex $xformname 2 ]
    set ind [ string first "=" $xformline ]
    set xformline [ string range $xformline [expr 1+ $ind ] end ]
    
    set xform  [ split $xformline , ]
    
    pxtkprint "\nxform ( [ llength $xform ] ) = $xform\n"

    set mat [ vtkMatrix4x4 [ pxvtable::vnewobj ]]
    set count 0
    for { set row 0 } { $row <= 3 } { incr row } {
	pxtkprint "  "
	for { set column 0 } { $column <= 3 } { incr column } {
	    $mat SetElement $row $column [ lindex $xform $count ]
	    set m [ $mat GetElement $row $column ]
	    set line [ format "%6.3f " $m ]
	    incr count
	    pxtkprint $line
	}
	pxtkprint "\n"
    }

    # Ref to World
    # Image to World

    set sur [ vtkPolyData [ pxvtable::vnewobj ]]
    $sur SetPoints $pts
    $pts Delete

    set tr [ vtkTransform [ pxvtable::vnewobj ]]
    $tr Identity
    $tr PostMultiply
    $tr Concatenate $mat ; $mat Delete
    catch { $tr Concatenate [ $vvlink_transform GetLinearInverse ] }
    
    set trf [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ]]
    $trf SetInput $sur;    $sur Delete
    $trf SetTransform $tr; $tr Delete
    $trf Update

    #    puts stderr [ [ $trf GetOutput ] GetBounds ]

#     set recon [ vtkSurfaceReconstructionFilter [ pxvtable::vnewobj ] ]
#     $recon SetInput [ $trf GetOutput ]
#     $recon DebugOn

#     $recon Update

#     set skinExtractor [ vtkContourFilter [ pxvtable::vnewobj ] ]
#     $skinExtractor SetUseScalarTree 0
#     $skinExtractor ComputeScalarsOff
#     $skinExtractor ComputeGradientsOff
#     $skinExtractor SetInput [  $recon GetOutput ] 
#     $skinExtractor SetValue 0 0.0
#     $skinExtractor Update


#    $trf Delete

    set  landmarkcontrol [ $this GetLandmarkControl ]
    
    if { $landmarkcontrol ==0 } {
	$trf Delete
	return
    }

    set lcontrol [ $landmarkcontrol GetLandmarkControl ]
    set curve [ vtkpxBaseCurve [ pxvtable::vnewobj ]]
    $curve SetFromPoints [ [ $trf GetOutput ] GetPoints ]
    $lcontrol SetClipboardCollection $curve 1

    ::pxtclutil::Info "Obtained [ [ $trf GetOutput ] GetNumberOfPoints ] Landmarks from the server. These are stored in the clipboard of the landmark control"
    
    $trf Delete
    $curve Delete

    set cur [ $lcontrol GetCollection -1 ]
    $cur Update

    $landmarkcontrol ShowWindow

    #    set  polydatacontrol [ $this GetPolyDataControl ]
    
    #     if { $polydatacontrol !=0 } {
    # 	[ $polydatacontrol GetPolyDataControl ]  SetClipboardSurface [ $trf GetOutput ] "ztouch_recon.vtk"
    # 	::pxtclutil::Info "Obtained [ [ $trf GetOutput ] GetNumberOfPoints ] from z-touch points. These are stored in the clipboard of the surface control"
    # 	$polydatacontrol ShowWindow
    #     }

    #    $skinExtractor Delete
    #   $recon Delete
    return 1
}

# -------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::PredictElectrodeStrip { } {

    
    set  polydatacontrol [ [ $this GetPolyDataControl ] GetPolyDataControl ]
    set  landmarkcontrol [ [ $this GetLandmarkControl ] GetLandmarkControl ]
    
    set tempc_lv [ vtkpxBaseCurve [ pxvtable::vnewobj ]] 
    $tempc_lv Copy  [ $landmarkcontrol GetCollection -1 ]
    $tempc_lv Compact
    set pts [ $tempc_lv GetPoints ]

    set nc [ $pts GetNumberOfPoints ]
    if { $nc < 2 } {
	::pxtclutil::Warning "Not Enough Points ($nc) in seed point set. Need at least 2 seed points to predict electrode strip position"
	return 0
	catch { $tempc_lv Delete }
    }
    
    set sur  [  $polydatacontrol  GetSurface [ expr $thisparam($this,stripbrainsurface) - 1 ] ]
    set np [ $sur GetNumberOfPoints ]
    if { $np < 1000 } {
	::pxtclutil::Warning "Not Enough Points ($np) on Selected Brain Surface ( $thisparam($this,stripbrainsurface). Cannot predict electrode strip position"
	return 0
    }
    
    set pred [ vtkpxPredictElectrodeStrip  [ pxvtable::vnewobj ]]
    $pred SetInputSurface $sur
    $pred SetStep   $thisparam($this,stripstep)
    $pred SetLength $thisparam($this,striplength)
    $pred SetSmoothness $thisparam($this,stripsmoothness)

    set thisparam($this,stripsmoothness) [ $pred GetSmoothness ]
    set thisparam($this,stripstep)       [ $pred GetStep ]
    set thisparam($this,striplength)     [ $pred GetLength ]

    set offset $thisparam($this,striplandmarkoffset)  
    if { $offset < 0  || $offset > [ expr $nc -2 ] } {
	set offset [ expr $nc -2 ]
    } 

    set p1 [ $pts GetPoint $offset ] 
    set p2 [ $pts GetPoint [ expr $offset +1 ]]
    
    $pred SetPoint1 [ lindex $p1 0 ] [ lindex $p1 1 ] [ lindex $p1 2 ]
    $pred SetPoint2 [ lindex $p2 0 ] [ lindex $p2 1 ] [ lindex $p2 2 ]
    $pred Update

    $polydatacontrol SetClipboardSurface [ $pred GetOutput ] "predicted.vtk"
    ::pxtclutil::Info "Create strip polygon with [ [ $pred GetOutput ] GetNumberOfPoints ]. These are stored in the clipboard of the surface control"
    
    [ $this GetPolyDataControl ] ShowWindow

    catch { $tempc_lv Delete ; $pts Delete }
    $pred Delete
    return 1
}


#--------------------------------------------------------------------------------------------
#
#          GUI Stuff
#
#--------------------------------------------------------------------------------------------

itcl::body pxitclvvlinkgadget::ShowControl { mode }  {

    if { $mode == "landmark" } {
	set  landmarkcontrol [ $this GetLandmarkControl ]
	if { $landmarkcontrol !=0 } {
	    $landmarkcontrol ShowWindow
	}
    } elseif { $mode == "surface" } {
	set  polydatacontrol [ $this GetPolyDataControl ]
	if { $polydatacontrol !=0 } {
	    $polydatacontrol ShowWindow
	}
    }
}


itcl::body pxitclvvlinkgadget::ShowProjectorControl { }  {
    if { $vvlink_projector != 0 } {
        $vvlink_projector ShowWindow
    }
}

#--------------------------------------------------------------------------------------------

itcl::body pxitclvvlinkgadget::CreateAboutControl { parent }  {

    $parent configure -bg black
    set txt [ iwidgets::scrolledtext $parent.st -labeltext "About VectorVisionLink" -vscrollmode dynamic -hscrollmode dynamic -width 5i -height 2i ]
    pack $parent.st -side top -expand true -fill both -padx 5 -pady 5

    $txt insert end "VVLink is a network based software interface to the VectorVision navigation system that enables researchers and skilled practitioners to download medical images, segmentation results, surgical planning and real-time tool tracking data from the navigation system as well as to upload view content. It was originally developped by Markus Neff (Brainlag AG http://www.brainlab.com) during his stay at Yale Image Processing and Analysis Group (http://noodle.med.yale.edu) \n\n"

    $txt insert end "This tool implements the VVLink interface between the BioImage Suite software suite and VectorVisionCranial.\n\n It was primarily authored by Xenios Papademetris (xenophon.papademetris@yale.edu) with assistance from Sven Flossman of Brainlab.\n\n"

    $txt insert end "The tool also implements an optional OpenIGTLink bridge server developped with the help of Junichi Tokuda and Steve Pieper\n\n"

    $txt insert end "The list of servers to connect is defined in a file .vvlinkservers place in each user's home directory. An example follows:\n\n"
    $txt insert end "\#VVLINK Host Description File\n\#---------------------------------------------\n\#Host:Password\nOR1:192.168.1.3:7A5F81\nOR2:192.168.1.6:8F10AD\n"

}


itcl::body pxitclvvlinkgadget::CreateFeatureControl { parent }  {
    
    set base $parent
    set w1 [ frame $parent.operations ]
    set w2 [ frame $parent.tools ]

    pack $parent.tools      -side bottom -expand true -fill x
    pack $parent.operations -side top -expand true -fill both

    eval "button $w1.1  -text \"Get Labeled Points As Landmarks\" -command { $this VVLinkGetLabeledPointsOrTools points }" 
    eval "button $w1.2  -text \"Get Labeled Points As Surface\" -command { $this VVLinkGetLabeledPointsOrToolsAsSurface points }" 
    eval "button $w1.3  -text \"Save Labeled Points in Text File \" -command { $this VVLinkSaveLabeledPointsOrTools points  }"
    frame $w1.4 -bg black -height 5
    eval "button $w1.51  -text \"Get Tracked Tool Tips As Landmarks\" -command { $this VVLinkGetLabeledPointsOrTools tools }" 
    eval "button $w1.5  -text \"Get Tracked Tools As Surface\" -command { $this VVLinkGetLabeledPointsOrToolsAsSurface tools }" 
    eval "button $w1.6  -text \"Save Tracked Tools in Text File \" -command { $this VVLinkSaveLabeledPointsOrTools tools  auto }"
    frame $w1.7 -bg black -height 5
    eval "button $w1.8  -text \"Acquire Landmark (Control-L)\" -command { $this VVLinkAcquireLandmark  }"	
    eval "bind all <Control-l> { $this VVLinkAcquireLandmark } "

    pack $w1.1 $w1.2 $w1.3 $w1.4 $w1.51 $w1.5 $w1.6 $w1.7 $w1.8 -side top -expand false -fill x -padx 10 -pady 2

    eval "button $w2.land  -text \"Landmark Control\" -command { $this ShowControl landmark}"
    eval "button $w2.poly  -text \"Surface Control\" -command  { $this ShowControl surface }"
    pack $w2.land $w2.poly -side left -fill x -expand true -padx 5
    
    lappend thisparam($this,disconnectlist) $thisparam($this,vvimagelist) $w1

}
#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::CreateTrajectoryControl { parent }  {
    
    set base $parent
    set w1 [ frame $parent.operations ]
    set w2 [ frame $parent.tools ]

    pack $parent.tools      -side bottom -expand true -fill x
    pack $parent.operations -side top -expand true -fill both
	
    #	eval "set nTr [ $vvl_connector GetVVNumberOfTrajectories ]"
    #	puts stderr "The total number of tr are $nTr"
    
    #	if { $vvlink_connected == 1 } {
    #		label $w45.inlinelabel -text "inline"
    #   	tk_optionMenu $w45.inline2 [ itcl::scope thisparam($this,inline2) ] 0 
    #}
    
    eval "button $w1.1  -text \"Get Trajectory Points \" -command { $this VVLinkGetTrajectoryPointsOrTools points }"
    eval "bind all <Control-l> { $this VVLinkAcquireLandmark } "
    
    #eval "label $w45.inlinelabel -text \"Number of Trajectories\" -command { $this GetVVNumberOfTrajectories }" 
    #set nTr { $this GetVVNumberOfTrajectories}
    #tk_optionMenu $w45.inline [ itcl::scope thisparam($this,inline) ] 0 1 2 3
    
    
    #tk_optionMenu $w45.inline 0 1 2 3
    #if { $vvlink_connected == 1 } {
    #	pack $w1.1 $w45.inlinelabel $w45.inline2 -side top -expand false -fill x -padx 10 -pady 2
    #} else {
    pack $w1.1 -side top -expand false -fill x -padx 10 -pady 2
    #}
    

    eval "button $w2.land  -text \"Landmark Control\" -command { $this ShowControl landmark}"
    eval "button $w2.poly  -text \"Surface Control\" -command  { $this ShowControl surface }"
    pack $w2.land $w2.poly -side left -fill x -expand true -padx 5
    
    lappend thisparam($this,disconnectlist) $thisparam($this,vvimagelist) $w1 

}
#--------------------------------------------------------------------------------------------

itcl::body pxitclvvlinkgadget::CreateZTouchControl { parent }  { 

    set base [ frame $parent.a ]
    $parent configure -bg black 
    pack $base -side top -expand t -fill both -padx 3 -pady 3
    

    set w5 [ frame $base.3 ];    pack $w5 -side bottom -expand true -fill x -padx 3 -pady 3

    set thisparam($this,vvztouchpointlist) [ iwidgets::scrolledlistbox $base.1 -labeltext "Z-Touch Point Sets"  -vscrollmode static -hscrollmode dynamic -selectmode single -exportselection 0 -height 1i ]

    
    set thisparam($this,vvztouchxformlist) [ iwidgets::scrolledlistbox $base.2 -labeltext "Z-Touch Pre-Registrations"  -vscrollmode static -hscrollmode dynamic -selectmode single -exportselection 0 -height 1i ]

    pack $base.1 $base.2 -side top -fill both -expand true -pady 5

    eval "button $w5.a  -text \"Log File\" -command { $this ParseLogFile \"\"  }"
    eval "button $w5.b  -text \"Reg Ini\" -command { $this ParseRegistrationIni \"\"  }"
    eval "button $w5.c  -text \"Reconstruct Z-Touch Surface \" -command { $this RecoverZTouchSurface }"

    pack $w5.a $w5.b $w5.c -side left -expand true -padx 3 -pady 2 -fill x 
}
#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::CreateDataSetControl { base }  { 
    
    set img_control [ [ pxitclimageGUI \#auto  ] GetThisPointer ]
    $img_control configure -description "Reference Image"
    
    set xform_control [ [  pxitcltransformGUI \#auto ] GetThisPointer ]
    $xform_control configure -description "Transformation World->Reference"

    $img_control Initialize $base.1
    $img_control AddFunction "$parent SetImageFromObject" "Display" "$this "


    $xform_control Initialize $base.2

    pack $base.1 $base.2 -side top -expand false -fill x -pady 2
}
#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::WarnAboutFlipLeft { } {

    if { $thisparam($this,flipleft) == 0 } {
	set ok [ pxtclutil::Question "Modifying Image Left/Right Flip is a dangerous operation. Are you sure?" ]
	if { $ok == 0 } {
	    set thisparam($this,flipleft) 1
	}
    }
}
#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::ShowAdvanced { } {

    if { $advanced_gui == 0 } {
	return
    }

    wm deiconify $advanced_gui
    scan  [ winfo geometry $basewidget ] "%dx%d+%d+%d" w1 h1 x1 y1 
    catch {
	set sc [ format "+%d+%d" [ expr $x1+$w1-100 ] [ expr $y1+$h1-100 ] ]
	eval "wm geometry $advanced_gui $sc"
    } 
}
#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::OpenIGTLinkConnect {  } {

#    if { $vvlink_connected == 0 } {
#	return
#    }

    if { $igtlconnector == 0 } {
	set igtlconnector [ vtkpxOpenIGTLinkClient [ pxvtable::vnewobj ]]
    }

    set igtlconnected 0
    set ok [ $igtlconnector InitializeConnection $thisparam($this,openigtlserver) $thisparam($this,openigtlport) ]
    if { $ok == 1 } {
	::pxtclutil::Info "OpenIGTLink Connection to $thisparam($this,openigtlserver) established."
	set igtlconnected 1
	EnableUI $thisparam($this,openigtlbase) 
	set thisparam($this,openigtlstatus) "Connected to $thisparam($this,openigtlserver):$thisparam($this,openigtlport)"
    } else {
	::pxtclutil::Warning "OpenIGTLink Connection to $thisparam($this,openigtlserver) failed."
	set thisparam($this,openigtlstatus) "Disconnected"
	DisableUI $thisparam($this,openigtlbase) 
    }
    
}
#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::OpenIGTLinkDisconnect { } {

    if { $igtlconnector == 0 } {
	return 0
    }
    $igtlconnector CloseConnection
    set thisparam($this,openigtlenablecursor) 0
    DisableUI $thisparam($this,openigtlbase) 
    set thisparam($this,openigtlstatus) "Disconnected"
}


#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::OpenIGTLinkSendTool { pts } {
    
    if { $igtlconnector != 0 && $igtlconnected == 1 } {
	if {  $thisparam($this,openigtlslicermode)  == 1 } {
	    set img [ [ $img_control GetImage ] GetImage ]
	    if { $img != "" } {

		# slicer bug
		set numtimes 1
		for { set i 0 } { $i < $numtimes } { incr i } {
		    $igtlconnector SendToolLocation $pts $img [ expr $thisparam($this,openigtlslicermode)  + $thisparam($this,openigtlslicercentering) ]
		}
	    }
	} else {
	    ## puts stderr "Sending Raw Points"
	    $igtlconnector SendToolLocation $pts
	}
    }
}
#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::OpenIGTLinkSendImage { } {
    if { $igtlconnector != 0 && $igtlconnected == 1 } {
	set img [ [ $img_control GetImage ] GetImage ]
	if { $img != "" } {
	    # puts stderr "Good Image for Sending Image\n"
	    set ok [ $igtlconnector SendImage $img ]
	}
    } else {
	# puts stderr "Bad Connection for Sending Image\n"
    }
}
#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::OpenIGTLinkSendCrossHairs { } {

    set img [ $vtk_viewer GetImage ]
    if  { $img == "" } { return }

    if { $igtlconnector != 0 && $igtlconnected == 1 } {
	set lv  [ $vtk_viewer GetLastClickedPointScaled ]
	set img [ $vtk_viewer GetImage ]
	set ptt [ vtkPoints [ pxvtable::vnewobj ]]
	$ptt SetNumberOfPoints 1
	$ptt SetPoint 0 [ lindex $lv 0 ] [ lindex $lv 1 ] [ lindex $lv 2 ]
	# slicer bug
	set numtimes 1
	for { set i 0 } { $i < $numtimes } { incr i } {
	    $igtlconnector SendToolLocation $ptt $img [ expr $thisparam($this,openigtlslicermode)  + $thisparam($this,openigtlslicercentering) ]
	}

    } else {
	# puts stderr "Bad Connection for Sending Image\n"
    }
}
#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::CreateOpenIGTLControl { parent }  {

    entry $parent.2 -width 30  -textvariable [ itcl::scope thisparam($this,openigtlstatus) ] -relief sunken  -bg black -fg white -state disabled
    pack $parent.2 -side bottom 

    set top  [ LabelFrame:create $parent.a -text "OpenIGTL Server Definition"]
    set bot  [ LabelFrame:create $parent.b -text "Additional Operations"]
    pack $parent.a  $parent.b -side top -expand t -fill both -pady 2


    iwidgets::entryfield $top.0 -labeltext "OpenIGTL Sever :"  -width 20  -textvariable [ itcl::scope thisparam($this,openigtlserver) ] -relief sunken 
    iwidgets::entryfield $top.1 -labeltext "OpenIGTL Port  :"  -width 10  -textvariable [ itcl::scope thisparam($this,openigtlport) ] -relief sunken 
    pack $top.0 $top.1 -side top -expand false -fill x

    set w [ frame $top.mid ]; pack $top.mid -side top -expand false -fill x
    

    eval "button $w.1  -text \"Connect\" -command { $this OpenIGTLinkConnect }" 
    eval "button $w.2  -text \"Disconnect\" -command { $this OpenIGTLinkDisconnect }" 
    pack $w.1 $w.2   -side left -expand true -fill x



    checkbutton $bot.a -variable [itcl::scope thisparam($this,openigtlenablecursor)]   -text "Enable OpenIGTLink Tool Link"
    checkbutton $bot.a2 -variable [itcl::scope thisparam($this,openigtlslicermode)]   -text "LPS to RAS Conversion" 
    checkbutton $bot.a3 -variable [itcl::scope thisparam($this,openigtlslicercentering)]   -text "Slicer Centering" 

    eval button "$bot.b -text \"Send Current Image to OpenIGTLink Server\" -command { $this OpenIGTLinkSendImage }"
    eval button "$bot.c -text \"Send Cross Hairs OpenIGTLink Server\" -command { $this OpenIGTLinkSendCrossHairs }"

    pack $bot.a $bot.a2 $bot.a3 $bot.b $bot.c -side top -expand false -fill x

    set thisparam($this,openigtlbase) $bot
    #DisableUI $thisparam($this,openigtlbase) 

}

#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::CreateServerControl { parent }  {

    set base $parent


    set w  [ frame $base.1 ]; pack $w -side top -expand f -fill x
    set w2  [ frame $base.11 ]; pack $w2 -side top -expand f -fill x
    frame $base.110 -bg black -height 3 ; pack $base.110 -side top -expand f -fill x
    set w1  [ frame $base.111 ]; pack $w1 -side top -expand f -fill x
    frame $base.15 -bg black -height 3 ; pack $base.15 -side top -expand f -fill x

    label $w.1 -text "Server:"
    set thisparam($this,servermenu) [ tk_optionMenu $w.2 [ itcl::scope thisparam($this,vvlinkserver) ] $thisparam($this,vvlinkserver) ]
    pack $w.1 $w.2  -side left -expand true -fill x
    
    lappend thisparam($this,connectlist)    [ eval "button $w1.3  -text \"Connect\" -command { $this VVLinkConnect  }" ]
    lappend thisparam($this,disconnectlist) [ eval "button $w1.4  -text \"Disconnect\" -command { $this VVLinkDisconnect }" ]
    lappend thisparam($this,disconnectlist) [ eval "button $w1.5  -text \"Update ImageList\" -command { $this VVLinkUpdateImageList }" ]
    pack $w1.3 $w1.4 $w1.5   -side left -expand true -fill x

    lappend thisparam($this,connectlist) [ eval "button $w2.1  -text \"Add Server\" -command { $this VVLinkServerAdd 0 }" ]
    lappend thisparam($this,connectlist) [ eval "button $w2.2  -text \"Re-Load Setup\" -command { $this LoadSetup }" ]
    lappend thisparam($this,connectlist) [ eval "button $w2.3  -text \"Save Setup\" -command { $this SaveSetup }" ]

    pack $w2.1 $w2.2 $w2.3 -side left -expand false -fill x -padx 2
    

    set thisparam($this,vvimagelist) [ iwidgets::scrolledlistbox $base.st -labeltext "Current VectorVision Image List"  -vscrollmode static -hscrollmode dynamic -selectmode single -exportselection 0 -height 2i ]
    set basea [ frame $base.orient ]
    set baseb [ frame $base.orient2 ]
    set basec $base.orient3 

    lappend thisparam($this,disconnectlist) $thisparam($this,vvimagelist) $basea $baseb

    radiobutton $basea.2o -variable [itcl::scope thisparam($this,orientation)] -value "-1" -text "Auto-Detect"
    eval "radiobutton $basea.2a -variable [itcl::scope thisparam($this,orientation)] -value 0 -text Axial -command { $this SelectedAxial }"
    eval "radiobutton $basea.2b -variable [itcl::scope thisparam($this,orientation)] -value 1 -text Coronal -command { $this SelectedCoronal }"
    radiobutton $basea.2c -variable [itcl::scope thisparam($this,orientation)] -value "2" -text "Sagittal" 
    eval "button  $basea.2f -text Advanced -command { $this ShowAdvanced }"
    pack  $basea.2o $basea.2a $basea.2b $basea.2c  $basea.2f -side left -expand t -fill x
   


    eval "button $baseb.1  -text \"Transfer Image\" -command { $this VVLinkGetImage -1 }" 
    pack $baseb.1  -side left -expand t -fill x -padx 10

    pack $baseb $basea -side bottom -expand false -fill x -padx 10
    pack $base.st -side top -expand true -fill both -pady 2 

    set advanced_gui $basec
    toplevel $basec; wm withdraw $basec; 
    eval "wm protocol $basec WM_DELETE_WINDOW { wm withdraw $basec }"
    wm title $basec "Advanced Controls"

    eval "checkbutton $basec.2a -variable [itcl::scope thisparam($this,flipleft)]   -text \"Flip Left\" -command { $this WarnAboutFlipLeft }"
    checkbutton $basec.2b -variable [itcl::scope thisparam($this,flipfront)]  -text "Flip Anterior"
    checkbutton $basec.2c -variable [itcl::scope thisparam($this,fliptop)]    -text "Flip Superior" 
    checkbutton $basec.2d -variable [itcl::scope thisparam($this,forceaxial)] -text "Force Axial" 
    eval "button  $basec.2e -text Close -command { wm withdraw $basec }"
    set wf [ frame $basec.2f ]
    label $wf.2d -text "Sample:"
    tk_optionMenu $wf.2e [ itcl::scope thisparam($this,samplerate) ] 1 2 3 4 8 
    pack $wf.2d $wf.2e -side left -expand t -fill x

    pack  $basec.2a $basec.2b $basec.2c $basec.2d  $wf $basec.2e  -side top -expand t -fill x


}
#--------------------------------------------------------------------------------------------

itcl::body pxitclvvlinkgadget::CreateConfigurationControl { parent }  {

    set base [ frame $parent.1 ]
    pack $base -side top -expand t -fill both
    
    set top  [ LabelFrame:create $base.a -text "Stream Viewer"]
    set bot  [ LabelFrame:create $base.b -text "Link Viewer Cursor With Navigational Pointer"]
    pack $base.a $base.b -side top -expand t -fill both -pady 2

    
    set w1  [ frame $top.1 ]
    set w2  [ frame $top.2 ]
    pack $w1 $w2 -side top -expand true -fill x
    
    label $w1.1 -text "Image Update Time (ms): "
    tk_optionMenu $w1.2 [ itcl::scope thisparam($this,streamdelay) ]  "50" "100" "200" "500" "1000" "2000" "5000" "10000"
    pack $w1.1 $w1.2  -side left -expand true -fill x

    lappend thisparam($this,disconnectlist) [ eval "button $w2.2 -text Test -command { $this VVLinkSendImage 0 }" ]
    lappend thisparam($this,streamofflist)  $w2.2

    lappend thisparam($this,disconnectlist) [ eval "button $w2.3 -text Enable -command { $this VVLinkSendImage 1 }" ]
    lappend thisparam($this,streamofflist)  $w2.3

    lappend thisparam($this,disconnectlist) [ eval "button $w2.4 -text Disable -command { $this VVLinkSendImage 2 }" ]
    lappend thisparam($this,streamonlist)  $w2.4
    pack $w2.2 $w2.3 $w2.4 -side left -expand true -fill x



    set w3  [ frame $bot.1 ]
    set w4  [ frame  $bot.2 ]
    set w45 [ frame $bot.21 ]
    set w5  [ frame $bot.3 ]
    pack $w3 $w4 $w45 $w5 -side top -expand true -fill x

    label $w3.0 -text "Update (ms): "
    tk_optionMenu $w3.1 [ itcl::scope thisparam($this,tooldelay) ]  50 100 200 500 1000 2000 5000 10000
    checkbutton $w3.2 -variable [itcl::scope thisparam($this,updateviewer)]   -text "Update Viewer" 
    pack $w3.0 $w3.1 $w3.2 -side left -expand true -fill x

    label $w45.1 -text "Radius: "
    tk_optionMenu $w45.2 [ itcl::scope thisparam($this,coneradius) ]  0.25 0.5 1 2 3 5 10
    label $w45.3 -text "Length:"
    tk_optionMenu $w45.4 [ itcl::scope thisparam($this,conelength) ]  10 20 30 40 50 75 100 200
    label $w45.inlinelabel -text "inline"
    tk_optionMenu $w45.inline [ itcl::scope thisparam($this,inline) ] 0 1 2 3
    pack $w45.1 $w45.2 $w45.3 $w45.4 $w45.inlinelabel $w45.inline -side left -expand true -fill x

    iwidgets::entryfield $w4.1 -labeltext "Extension:" -width 3 -textvariable [ itcl::scope thisparam($this,coneextension) ] -relief sunken -validate real 
    lappend thisparam($this,disconnectlist) [ eval "button $w4.2 -text \"Show/Hide\" -command { $this ShowHideCone }" ]


    pack $w4.1  $w4.2 -side left -expand true -fill x

    lappend thisparam($this,disconnectlist) [ eval "button $w5.1 -text Test -command { $this VVLinkGetToolCoordinates 0 }" ]
    lappend thisparam($this,linkofflist)  $w5.1

    lappend thisparam($this,disconnectlist) [ eval "button $w5.15 -text Color -command { $this TipColor 3 }" ]

    lappend thisparam($this,disconnectlist) [ eval "button $w5.2 -text Enable -command { $this VVLinkGetToolCoordinates 1 }" ]
    lappend thisparam($this,linkofflist)  $w5.2

    lappend thisparam($this,disconnectlist) [ eval "button $w5.3 -text Disable -command { $this VVLinkGetToolCoordinates 2 }" ]
    lappend thisparam($this,linkonlist)  $w5.3


    # Eliminate this for now
    set ok 0
    #    set ok [ pxtclvtkpxcontrib::ConditionalLoadLibrary vtkjoStereoVisionTCL vtkjoProbeVisualization ]
    if { $ok != 0 } {
	#       lappend thisparam($this,disconnectlist) [ eval "button $w5.4 -text Projector -command { $this ShowProjectorControl }" ]
        eval "button $w5.4 -text Projector -command { $this ShowProjectorControl }" 
	pack $w5.4 -side right -expand false -fill x
    }

    pack $w5.1 $w5.15 $w5.2 $w5.3 -fill x -side left -expand true


}
## 
#--------------------------------------------------------------------------------------------

itcl::body pxitclvvlinkgadget::CreatePointerControl { parent }  {

    set base [ frame $parent.1 ]
    pack $base -side top -expand t -fill both 
    
    set top  [ LabelFrame:create $base.a -text "Pointer Controls"]

    pack $base.a  -side top -expand t -fill both -pady 2
    
    set w0  [ frame $top.1 ]
    set w1  [ frame $top.2 ]
    set w2  [ frame $top.3 ]
    set w3  [ frame $top.4 ]
    set w4  [ frame $top.5 ]
    set w5  [ frame $top.6 ]
    
    pack $w0 $w5 $w4 $w1 $w2 $w3 -side top -expand true -fill both

#    label $w0.1 -text "Cylinder Enable" -state disabled
    
    eval "checkbutton $w0.1 -variable [ itcl::scope thisparam($this,cylinderenabled) ] -text {Enable Cylinder} "
    
    label $w1.1 -text "Cross Length " -state disabled
    scale $w1.2 -variable  [ itcl::scope thisparam($this,crosslength) ] -orient horizontal -length 628 -from 2.0 -to 10.0  -resolution 0.25 -state disabled
    label $w2.1 -text "Cross Thickness " -state disabled
    scale $w2.2 -variable  [ itcl::scope thisparam($this,crossthickness) ] -orient horizontal -length 628 -from 0.2 -to 1.0  -resolution 0.01 -state disabled
    label $w3.1 -text "Set Roll Angle" -state disabled
    scale $w3.2 -variable  [ itcl::scope thisparam($this,rollangle) ] -orient horizontal -length 628 -from -180.0 -to 180.0  -resolution 0.05 -state disabled
    
    eval "button $w4.2 -text {Cross Color} -command { $this TipColor 5 } -state disabled" 

    # lappend thisparam($this,pointerlist)  $w1.1 $w1.2 $w2.1 $w2.2 $w3.1 $w3.2 $w4.1 $w4.2 # Show Sphere 
    lappend thisparam($this,pointerlist)  $w0.1 $w1.1 $w1.2 $w2.1 $w2.2 $w3.1 $w3.2 $w4.2  

    eval "checkbutton $w5.1 -variable [ itcl::scope thisparam($this,showcross) ] -command { $this EnablePointerControls  } -text {Show Cross} "
  
     # pack $w1.1 $w1.2 $w2.1 $w2.2 $w3.1 $w3.2 $w4.1 $w4.2 $w5.1 -side top -expand true # Show Sphere 
     pack $w0.1 $w1.1 $w1.2 $w2.1 $w2.2 $w3.1 $w3.2 $w4.2 $w5.1 -side top -expand true  
    
    	    
}

##

#--------------------------------------------------------------------------------------------
itcl::body pxitclvvlinkgadget::CreateStripControl { parent }  {

    set base $parent

    set w  [ LabelFrame:create $base.a -text "Strip Properties"]; pack $base.a -side top -expand t -fill x
    

    iwidgets::entryfield $w.1 -labeltext "Strip Length (mm):"  -width 2 -validate integer -textvariable [ itcl::scope thisparam($this,striplength) ] -relief sunken 
    iwidgets::entryfield $w.2 -labeltext "Step Size (mm):"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,stripstep) ] -relief sunken 

    iwidgets::entryfield $w.3 -labeltext "Smoothness:"  -width 4 -validate real -textvariable [ itcl::scope thisparam($this,stripsmoothness) ] -relief sunken 

    pack $w.1 $w.2 $w.3 -side top -expand t -fill x

    frame $base.15 -bg black -height 3 ; pack $base.15 -side top -expand f -fill x

    set w  [ LabelFrame:create $base.b -text "Surface \& Seed Points"]; pack $base.b -side top -expand t -fill x
    

    iwidgets::entryfield $w.1 -labeltext "Use Points from Current Set (-1=Last 2) First Point" -width 4 -validate integer -textvariable [ itcl::scope thisparam($this,striplandmarkoffset) ] -relief sunken 
    iwidgets::entryfield $w.2 -labeltext "Outer Brain Surface:"  -width 1 -validate integer -textvariable [ itcl::scope thisparam($this,stripbrainsurface) ] -relief sunken 

    pack $w.1 $w.2 -side top -expand true -fill x

    frame $base.25 -bg black -height 3 ; pack $base.25 -side top -expand f -fill x

    set w2 [ frame $base.c ] ; pack $w2 -side bottom -expand t -fill x -pady 2
    eval "button $w2.land  -text \"Landmark Control\" -command { $this ShowControl landmark}"
    eval "button $w2.poly  -text \"Surface Control\" -command  { $this ShowControl surface }"
    pack $w2.poly $w2.land -side right  -expand false -padx 5

    eval "button $w2.create  -text \"Create Strip\" -command  { $this PredictElectrodeStrip }"
    pack $w2.create -side left  -expand false -padx 5

}


#--------------------------------------------------------------------------------------------
# Main GUI Body 
# ---------------------------------------------------------------------------

itcl::body pxitclvvlinkgadget::Initialize { widget viewer } { 
	
    global pxtcl_pref_array

    set basewidget [ toplevel $widget ]
    wm geometry $basewidget 500x400
    wm withdraw $basewidget

    set vtk_viewer $viewer 
    
    set notebook $basewidget.notebook
    set bbar    [ frame $basewidget.bot    -width 400 -height 50 ]

    iwidgets::tabnotebook $notebook  -tabpos w
    pack $bbar -side bottom -expand f -fill x -pady 2 -pady 2
    pack $notebook -side top -expand t -fill both -pady 2 -padx 2
    eval "button $bbar.1  -text \"Close\" -command {  $this HideWindow }"
    pack $bbar.1 -side right -expand f -padx 10
    

    CreateServerControl             [ $notebook add -label Server  ] 
    CreateConfigurationControl      [ $notebook add -label "Link"  ] 
    CreateDataSetControl            [ $notebook add -label "Data"  ] 
    CreateFeatureControl            [ $notebook add -label "Points"  ]
    CreatePointerControl            [ $notebook add -label "Pointer Controls"  ]  

    if { $pxtcl_pref_array(EnableBetaFeatures) == "Enabled" } {
	CreateZTouchControl             [ $notebook add -label "Z-Touch"  ] 
	CreateStripControl              [ $notebook add -label "Strip"  ] 
    }
    if { $enable_igtl ==1 && $pxtcl_pref_array(OpenIGTLink)=="Enabled" } {
	CreateOpenIGTLControl             [ $notebook add -label "IGTL Bridge"  ] 
    }
	CreateTrajectoryControl         [ $notebook add -label "Trajectories"  ]
    CreateAboutControl              [ $notebook add -label "About"  ] 


    if { $vvlink_connected == 1 } {
	DisableUI $thisparam($this,connectlist)    
    } else {
	DisableUI $thisparam($this,disconnectlist)    
    }


    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"

    if { [ file exists $setupfilename ] } {
	LoadSetup $setupfilename
    }

    for { set i 0 } { $i <= 5 } { incr i } {
	if { $i == 3 } {		
	    	set vv_pointersource($i) [ vtkConeSource [ pxvtable::vnewobj ]]
	    	$vv_pointersource($i) SetResolution 36
	} elseif { $i == 4 } {
	    set vv_pointersource($i) [ vtkSphereSource [ pxvtable::vnewobj ]]
	    $vv_pointersource($i) SetThetaResolution 6
	    $vv_pointersource($i) SetPhiResolution 6 	
	} elseif { $i == 5 } {
	    set vv_pointersource($i) [ vtkpxCrossHairSource [ pxvtable::vnewobj ]]
	   $vv_pointersource($i) SetLength 3.0 
	} else {
	    set vv_pointersource($i) [ vtkpxCrossHairSource [ pxvtable::vnewobj ]]
	    $vv_pointersource($i) SetShowSphere 1
	    $vv_pointersource($i) SetLength 2.0
	}

	set vv_pointermapper($i) [ vtkPolyDataMapper [ pxvtable::vnewobj ]]
	$vv_pointermapper($i) SetInput [ $vv_pointersource($i) GetOutput ]
	
	set vv_pointeractor($i)  [ vtkActor [ pxvtable::vnewobj ]]
	$vv_pointeractor($i) SetMapper $vv_pointermapper($i)
	$vv_pointeractor($i) VisibilityOff
	
	if { $i == 3 } {
	    [ $vv_pointeractor($i) GetProperty ] SetSpecularPower 0.5
	    [ $vv_pointeractor($i) GetProperty ] SetColor 0 0 1
	} elseif { $i == 4 } {
	    [ $vv_pointeractor($i) GetProperty ] SetSpecularPower 10.5
	    [ $vv_pointeractor($i) GetProperty ] SetColor 0.5 0.5 1.0
	    [ $vv_pointeractor($i) GetProperty ] SetRepresentationToWireframe
	} elseif { $i == 5 } {
	    [ $vv_pointeractor($i) GetProperty ] SetSpecularPower 100.5
	    [ $vv_pointeractor($i) GetProperty ] SetColor 1.0 0.0 0.0
	    [ $vv_pointeractor($i) GetProperty ] SetRepresentationToSurface 
	} else {
	    [ $vv_pointeractor($i) GetProperty ] SetSpecularPower 0.0
	    [ $vv_pointeractor($i) GetProperty ] SetColor 1 0 0
	    [ $vv_pointeractor($i) GetProperty ] SetAmbient 1.0
	    [ $vv_pointeractor($i) GetProperty ] SetDiffuse 0.0
	    [ $vv_pointeractor($i) GetProperty ] SetRepresentationToWireframe 
	}
	    
	
	if { [ $vtk_viewer IsA "vtkpxGUIOrthogonalViewer" ] ==1 } {
	    [ [ $vtk_viewer GetRenderer $i ] GetRenderer ] AddActor $vv_pointeractor($i)
	}
    }

    set canvas [ [ $notebook component tabset ] component canvas ]
    set l [ AddLogoLabel $canvas ]
    pack $l -side bottom -padx 1 

    set ok 0
    #set ok [ pxtclvtkpxcontrib::ConditionalLoadLibrary vtkjoStereoVisionTCL vtkjoProbeVisualization ]
    #if { $vvlink_projector == 0  && $ok != 0 } {
    #    set vvlink_projector [ pxitclvvlinkprojectorgadget [ pxvtable::vnewobj ] $this ]
    #}
	
    SetTitle "VVLINK Tool: Disconnected" 
    $basewidget configure -bg black



    return $basewidget

}

::itcl::body pxitclvvlinkgadget::AddToMenuButton { mb args} {

    eval "$mb add command -label \"VV Link Tool\" -command {$this ShowWindow \"Server\"}"
}


# ---------------------------------------------------------------------------


