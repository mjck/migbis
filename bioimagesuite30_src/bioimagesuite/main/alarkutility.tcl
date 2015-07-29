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




# -----------------------
# Dependencies and path
# -----------------------
package provide alarkutility 1.0
package require  Itcl 3.2
package require  Iwidgets 4.0

package require  pxitclbaseimagecontrol 1.0



itcl::class alarkutility {


    # derives from pxitclbaseimagecontrol

    inherit pxitclbaseimagecontrol
    protected common thisparam    

    #-----------------------------------
    # construction and descruction
    #-----------------------------------
    constructor { par } {
	pxitclbaseimagecontrol::constructor $par
    } { InitializeControl }
    
    destructor { }

    #-----------------------------------
    # initialization methods
    #-----------------------------------

    # This must exist it creates the interface 
    public method InitializeControl { }
    public method Initialize { inpwidg }
      
    #-----------------------------------
    # interface creation methods
    #-----------------------------------
    protected method CreateConnectivityControl { name }

    # ----------------------------------
    # Computational Utility Stuff
    # ----------------------------------
    public method CreateConnectivity { }
    public method RenderGraph { alg ren red green blue size stippleStyle}

    # ----------------------------------
    # Add this control to a Menu Button
    # ----------------------------------
    public method AddToMenuButton { mb args }    

}  

#-------------------------------------------------------------------------------------------
#  Initialize tool
# -------------------------------------------------------------------------------------------
itcl::body alarkutility::InitializeControl { } {

    set appname "My Tool v1.0"
    set aboutstring "(c)  2006"

    set global_list(appname) "My Utility"
    set global_list(radioselection) selected_positive


    # All Parameters that need to be appear in the GUI need to be part of this array
    # ------------------------------------------------------------------------------
    set thisparam($this,radius) 10.0
    set thisparam($this,lowerthreshold) 0.2
    set thisparam($this,higherthreshold) 1.0
    set thisparam($this,linewidth) 3.0
    set thisparam($this,radiochoice) selected_positive
    
}

itcl::body alarkutility::CreateConnectivity { } {

    set sphere [ vtkSphereSource New ]
    $sphere SetRadius $thisparam($this,radius)
    $sphere SetCenter 100.0 100.0 0.0
    $sphere Update

    set map [ vtkPolyDataMapper New ]
    $map SetInput [ $sphere GetOutput ]

    set act [ vtkActor New ]
    $act SetMapper $map
    
    
	# Create a graph
	set graph [ vtkGraph New]
	$graph SetDirected 0

	# Create points
	set pts [vtkPoints New]
	     
	#puts "The input file name is [.fsb get]"
	puts "in create connectivity selected $thisparam($this,radiochoice)"
	
    set inputfile [open [.fsb get] "r"]
    gets $inputfile fileType
        
    set checkstring "#Connectivity file"
    # trimright trims trailing white spaces
	set filecheck [string compare [ string trimright $fileType ] $checkstring]   

	if {$filecheck == 0} {
		puts "Connectivity file loaded"
	} else {
		puts "Incorrect Connectivity File Format"
		return
	}
    
    gets $inputfile pointsType
    #puts $pointsType
    
    gets $inputfile num_of_centroids
    #puts "Number of points: $num_of_centroids"
    
    set nameArray [vtkStringArray New]
    $nameArray SetName "name"
       
    for {set i 0} {$i < $num_of_centroids} {incr i} {
	    #puts "testing $i" 
	    gets $inputfile pt1
		scan $pt1 "%d %f %f %f %s" index px py pz name_of_roi

		set xval [expr {$px * 1.17}]
		set yval [expr {$py * 1.17}]
		set zval [expr {$pz * 1.17}]

		$nameArray InsertNextValue $name_of_roi
		
		$graph AddVertex
		$pts InsertNextPoint $xval $yval $zval

    }    
   
    $graph SetPoints $pts
    
    [$graph GetVertexData] AddArray $nameArray    
    
    ## Process edges now 
    
    gets $inputfile connections
    #puts $connections 
    
    gets $inputfile num_of_connections
    puts $num_of_connections 
    
    set strength_array [vtkDoubleArray New]
        
    for {set i 0} {$i < $num_of_connections} {incr i} {
   
	   gets $inputfile edge_info
	   scan $edge_info "%d %d %f" edge_src edge_dist edge_strength
	   $graph AddEdge $edge_src $edge_dist 
	   $strength_array InsertNextValue $edge_strength

	}
	
  $strength_array SetName "value"
  
  [$graph GetEdgeData] AddArray $strength_array
	
	#puts "Testing threshold selection..." 
	set threshold [vtkSelection New]
	$threshold SetContentType 8
	$threshold SetFieldType 0
	
	set thresholdArr [vtkDoubleArray New]
	$thresholdArr SetName "value"
	$thresholdArr InsertNextValue $thisparam($this,lowerthreshold)
	$thresholdArr InsertNextValue $thisparam($this,higherthreshold)
	
	$threshold SetSelectionList $thresholdArr 
	
	set layout [vtkGraphLayout New]
	$layout SetInput $graph
	
	set passthrough [vtkPassThroughLayoutStrategy New]
  	$layout SetLayoutStrategy $passthrough 
		
	set extractThreshold [vtkExtractSelectedGraph New]
	$extractThreshold SetInputConnection 0 [$layout GetOutputPort]	
	$extractThreshold SetInput 1 $threshold	
	
	
	# 2nd graph 
	set threshold2 [vtkSelection New]
		$threshold2 SetContentType 8
		$threshold2 SetFieldType 0
	
		set thresholdArr2 [vtkDoubleArray New]
		$thresholdArr2 SetName "value"
		$thresholdArr2 InsertNextValue 0.0
		$thresholdArr2 InsertNextValue 0.1
		
	$threshold2 SetSelectionList $thresholdArr2 
	
	set layout2 [vtkGraphLayout New]
		$layout2 SetInput $graph
		
		set passthrough2 [vtkPassThroughLayoutStrategy New]
	  	$layout2 SetLayoutStrategy $passthrough2 
			
		
	set extractThreshold2 [vtkExtractSelectedGraph New]
	$extractThreshold2 SetInputConnection 0 [$layout GetOutputPort]		
	$extractThreshold2 SetInput 1 $threshold2

	

    # Getting the Images
    # Two things

    set viewer [ $parent GetViewer ]
    puts stderr "Viewer = [ $viewer GetClassName ]"

    set img $currentimage 
    puts stderr "[$img GetDescription ]"

    set imagedata [ $img GetImage ]
    puts "ImageData = [ $imagedata  GetClassName ]"

    set objectmap [ $viewer GetObjectMapImage ]
    puts "ImageData = [ $objectmap  GetClassName ], [ [ [ $objectmap  GetPointData ] GetScalars] GetRange]"

    # parent = master application
    # the viewer
 
    set guiren [ $viewer GetRenderer 3 ]
    puts stderr "Viewer = [ $guiren GetClassName ]"

    # Raw Viewer
    set ren [ $guiren GetRenderer ]
    puts stderr "Viewer = [ $ren GetClassName ]"

	# Actor showing a small sphere 
	# $ren AddActor $act
	
	# Grey lines
	#RenderGraph $layout $ren 0.4 0.4 0.4 3.0 65535 
	#RenderGraph $layout $ren 0.2 0.2 0.2 3.0 65535 
		
	RenderGraph $layout $ren 0.5 0.5 0.5 3.0 65535 
	
	RenderGraph $extractThreshold2 $ren 0.4 0.4 1.0 $thisparam($this,linewidth) 65535
			
	# Green stippled lines
	#RenderGraph $extractThreshold $ren 0.8 1.0 0.2 $thisparam($this,linewidth) 61680
	RenderGraph $extractThreshold $ren 1.0 0.4 0.4 $thisparam($this,linewidth) 65535
	
	#$ren AddActor $vertexActor
	#$ren AddActor $edgeActor	    

    $viewer  SetDisplayMode3D

    $guiren Render

}


itcl::body alarkutility::RenderGraph { alg ren red green blue size stippleStyle} {
	
	 set centroid_sphere [vtkSphereSource New]
	  $centroid_sphere SetRadius 4
	  $centroid_sphere SetPhiResolution 12
	  $centroid_sphere  SetThetaResolution 12 

	set graphToPoly [vtkGraphToPolyData New]
	$graphToPoly SetInputConnection [$alg GetOutputPort]
	
	set edgeMapper [vtkPolyDataMapper New]
	$edgeMapper SetInputConnection [$graphToPoly GetOutputPort]
	
	set edgeActor [vtkActor New]
	$edgeActor SetMapper $edgeMapper
	[$edgeActor GetProperty] SetColor $red $green $blue
  	[$edgeActor GetProperty] SetLineWidth $size
 	[$edgeActor GetProperty] SetLineStipplePattern $stippleStyle
	[$edgeActor GetProperty] SetLineStippleRepeatFactor 2  	  	

	set vertex [vtkGlyphSource2D New]
	$vertex SetGlyphTypeToVertex
	
	set glyph [vtkGlyph3D New]
	$glyph SetInputConnection 0 [$graphToPoly GetOutputPort]
	$glyph SetInputConnection 1 [$centroid_sphere GetOutputPort]

	set vertMapper [vtkPolyDataMapper New]
	$vertMapper SetInputConnection [$glyph GetOutputPort]
	
	set vertActor [vtkLODActor New]
	$vertActor SetMapper $vertMapper
  	[$vertActor GetProperty] SetColor $red $green $blue
  	#[$vertActor GetProperty] SetPointSize $size  	
   
  	 set labelMapper [vtkLabeledDataMapper New]
	 $labelMapper SetInputConnection [$graphToPoly GetOutputPort]
	 $labelMapper SetLabelFormat "%s"
	 $labelMapper SetLabelModeToLabelFieldData	 
 	 $labelMapper SetFieldDataName "name"
	 #[$labelMapper GetLabelTextProperty ] SetJustificationToLeft
  	 #[$labelMapper GetLabelTextProperty ] SetColor 0 0 0  	 
	 		
	set labelActor [vtkActor2D New]
	$labelActor SetMapper $labelMapper  	
	
	$ren AddActor $edgeActor;
	$ren AddActor $vertActor
	$ren AddActor $labelActor
}


# image is a vtkImageData Here !!!!
# $this StoreResult [ $myfilter GetOutput ] somename someorientation

itcl::body alarkutility::CreateConnectivityControl { base } {

    iwidgets::labeledframe $base.frame0 \
	-labelpos nw  \
	-labeltext "Parameters"
    pack $base.frame0 -fill both -expand f -pady 5
    
    set frame0 [ $base.frame0 childsite ]

    #iwidgets::entryfield $frame0.e -labeltext "Radius:" -textvariable [ itcl::scope thisparam($this,radius) ] -relief sunken -width 6 -validate real
    #pack $frame0.e -side left -padx 2 -fill x -expand false
     
    iwidgets::entryfield $frame0.lt -labeltext "Lower threshold:" -textvariable [ itcl::scope thisparam($this,lowerthreshold) ] -relief sunken -width 6 -validate real
    pack $frame0.lt -side left -padx 2 -fill x -expand false
    
    iwidgets::entryfield $frame0.gt -labeltext "Higher threshold:" -textvariable [ itcl::scope thisparam($this,higherthreshold) ] -relief sunken -width 6 -validate real
    pack $frame0.gt -side left -padx 2 -fill x -expand false

    iwidgets::entryfield $frame0.linewidth -labeltext "Line width:" -textvariable [ itcl::scope thisparam($this,linewidth) ] -relief sunken -width 6 -validate real
    pack $frame0.linewidth -side bottom -padx 2 -fill x -expand false

    iwidgets::labeledframe $base.frame1 \
	-labelpos nw  \
	-labeltext "Parameters"
    pack $base.frame1 -fill both -expand f -pady 5
    
    set frame1 [ $base.frame1 childsite ]
        
	iwidgets::radiobox $frame1.rb -labeltext "Size:" -labelpos nw  -command { puts "test" }
	
	
	#-command { set global_list(radioselection) [$frame1.rb get] }
	pack $frame1.rb -padx 4 -pady 4 -fill both

	$frame1.rb add selected_positive -text "Positive" 
	$frame1.rb add selected_negative -text "Negative"
	$frame1.rb add selected_both -text "Both"

	$frame1.rb select selected_positive
	    
    puts " selection [$frame1.rb get]"
    
	iwidgets::optionmenu $frame1.pos -labeltext "Position:" 
	#-command {
	#    .lw configure -labelpos [.pos get]
	#}
	pack $frame1.pos -padx 4 -pady 4
	
	$frame1.pos insert end e n ne nw s se sw w
	$frame1.pos select w
	
    	
	pack $frame1.rb -padx 10 -pady 10 -fill both -expand yes
	
	eval "button $base.but -text \"Show Connectivity\" -command { $this CreateConnectivity }"
	pack $base.but -side bottom -expand t -fill x
		
	
	iwidgets::extfileselectiondialog .fsb -modality application 	

	button $base.select -text "Select Connectivity File" -command {
		if {[.fsb activate]} {
			puts "selected: [.fsb get]"	     
		} else {
			puts "no file selected" 
		}
	}
		
	puts "check box ? - [$frame1.pos get]"
	
	pack $base.select -side bottom -expand t -fill x
		
	iwidgets::optionmenu .cb -labeltext "Font:" -labelpos w -command {
	    puts "selected: [.cb get]"
	}
	pack .cb
	
	.cb insert end Ariel Courier Helvetica Knarly Lucida \
	    Rumpus Symbol Times "Zapf Dingbats"
	     
}

# -------------------------------------------------------------------------------------------
#  Diffusion control inititialization
# ------------------------------------------------------------------------------------------
itcl::body alarkutility::Initialize { widget } {
    
    if { $initialized == 1 } { return $basewidget }
    
    #  -------------------------------------
    #  Create User Interface
    #  -------------------------------------	
    set basewidget [toplevel $widget ]
    wm geometry $basewidget 610x450
    wm withdraw $basewidget


    set notebook $basewidget.notebook    
    iwidgets::tabnotebook $notebook -tabpos w         
    set widget_list(notebook) $notebook

    set mb [ frame $basewidget.mb ]
    pack $mb -side top -fill x -expand false
    
    #    CreateMenu $mb
    CreateConnectivityControl    [ $notebook add -label "Add Connectivity" ] 
    pack $notebook -side top -fill both -expand t -padx 5
    
     #option add *textBackground seashell

    set initialized 1

    SetTitle "Connection Utility"

    # this is critical 
    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget 
}

#-------------------------------------------------------------------------------------------
#  Add controls to menu button
# ------------------------------------------------------------------------------------------
itcl::body alarkutility::AddToMenuButton { mb args } {
    eval "$mb add command -label \"Add Connectivity\" -command {$this ShowWindow \"Add Connectivity\"}"   
}

#-------------------------------------------------------------------------------------------
#  Main function for stand-alone execution
# ------------------------------------------------------------------------------------------
if { [ file rootname $argv0 ] == [ file rootname [ info script ] ] } {

    puts "\n[ file rootname $argv0 ] is not a stand-alone program.\n"
    exit
}


