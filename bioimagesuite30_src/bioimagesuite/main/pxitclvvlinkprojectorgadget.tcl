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






package provide pxitclvvlinkprojectorgadget 1.0

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
package require  pxitcllandmarks        1.0
package require  pxitclsurface          1.0
package require  pxitcltransform        1.0
package require  pxitclbaseimagecontrol 1.0
package require  pxtcluserprefs         1.0

# ----------------------------------------------------------------------------------------------

itcl::class pxitclvvlinkprojectorgadget {

    inherit pxitclbasecontrol

    # Generic Stuff 
    private   common   thisparam        

    # VTKViewer is a vtkpxGUIOrthogonalViewer
    protected variable VTKViewer        0
    protected variable VVLinkGadget     0
    protected variable RenderWindow     0

    protected variable estimatecontrol  0 
    protected variable landmarkcontrol  0 
    protected variable surfacecontrol   0 
    
    # Booleans for if we should be projecting
#    protected variable ProjectPolyData  0
#    protected variable ProjectVolume    0
    
    protected variable Representation   0
    protected variable ProbeVisualization 0
    
    protected variable Context          0
    
#    protected variable PolyDataProjectorSource  0
#    protected variable PolyDataProjectorMapper  0
#    protected variable PolyDataProjectorActor   0
    
#    protected variable VolumeProjectorSource  0
#    protected variable VolumeProjectorMapper  0
#    protected variable VolumeProjectorActor   0

#    protected variable EllipseSource    0
#    protected variable EllipseMapper    0
#    protected variable EllipseActor     0
#    protected variable EllipseTransform 0
    
    # parentObject is the vvlinkgadget that owns this window
    constructor { vvlinkgadget } {
	   ::pxitclbasecontrol::constructor $vvlinkgadget
    } {
        $this InitializeVVLinkProjectorGadget $vvlinkgadget
    }

    
    private method InitializeVVLinkProjectorGadget { vvlinkgadget } 
    public method DelayedInitialize { }
    protected method CreateProjectorControl { parent } 
    
    public method UseVisualizationCallback { } 
    public method ContextCallback { } 
    public method DisplayContextCallback { } 
    public method UpdateEstimateCallback { }
    public method ContextAddPointsCallback { }
    public method UpdateLandmarksCallback { }
#    public method SetVisibility { isVisible } { set IsVisible $isVisible }
    # (tx,ty,tz) are the tool tip coordinates and (bx,by,bz) are the 
    # tool base coordinates.
    public method Update { x y z nx ny nz }
}

# ---------------------------------------------------------------------------


itcl::body pxitclvvlinkprojectorgadget::InitializeVVLinkProjectorGadget { vvlinkgadget } {
    puts stderr "vvlinkprojectorgadget::InitializeVVLinkProjector"
    
    pxtclvtkpxcontrib::LoadLibraries { vtkjoStereoVisionTCL }
    
    if { $vvlinkgadget != 0 } {
        set VVLinkGadget $vvlinkgadget
    } else {
        puts stderr "Cannot use ProjectorGadget with NULL parent VVLinkGadget"
        return
    }
    
    # Create the window
    set basewidget [ toplevel [ $VVLinkGadget GetBaseWidget ].[pxvtable::vnewobj] ]
    wm withdraw $basewidget
    wm geometry $basewidget 350x450
    
    SetTitle "Projector Tool"
    
    # Get the base viewer so we can update the render window
    set baseviewer [ $VVLinkGadget GetParent ]
    
#    set PolyDataControl [ $baseviewer GetPolyDataControl ]
#    set thisparam($this,surface)    1
    
#    set thisparam($this,dovolumeproject)          0
#    set thisparam($this,dosurfaceproject)         0
    set thisparam($this,dovisualization)          0

    set thisparam($this,projectorfocallength)     25.0
    set thisparam($this,projectorsearchdistance)  100.0
    set thisparam($this,projectorparallel)        0

    set thisparam($this,crosshairlength)          1.0
    set thisparam($this,crosshairresolution)      10
    set thisparam($this,circleradius)             1.0
    set thisparam($this,circleresolution)         36
    
    set thisparam($this,usecontext)               0
    set thisparam($this,contextcapturerange)      3.0
    set thisparam($this,displaycontext)           0
    
    # Create the VTK pipelines
    
#    set Context [ vtkjoLandmarkDistanceContext New ]
#    set Context [ vtkjoStatisticalLandmarkDistanceContext New ]
#    set Context [ vtkjoRegistrationErrorContext New ]
    
#    set VolumeProjectorSource [ vtkjoVolumeProjector New ]
#    $VolumeProjectorSource SetTemplate $Template
#    $VolumeProjectorSource SetContext $Context

#    set lut [ vtkLookupTable New ]
#    $lut SetNumberOfTableValues 2
#    $lut SetTableRange 0 1 
#    $lut SetTableValue 0 0.0 0.0 1.0 1.0
#    $lut SetTableValue 1 1.0 0.0 0.0 1.0
    
#    set VolumeProjectorMapper [ vtkPolyDataMapper New ]
#    $VolumeProjectorMapper SetInput [ $VolumeProjectorSource GetOutput ]
#    $VolumeProjectorMapper SetLookupTable $lut
#    $lut Delete
#    $VolumeProjectorMapper SetScalarModeToUsePointFieldData
#    $VolumeProjectorMapper SelectColorArray "Context"
#    $VolumeProjectorMapper ScalarVisibilityOff
    
#    set VolumeProjectorActor  [ vtkActor New ]
#    $VolumeProjectorActor SetMapper $VolumeProjectorMapper
#    set prop [ $VolumeProjectorActor GetProperty ]
#    $prop SetLineWidth 4.0
#    $prop SetColor 0.0 0.0 1.0
#    $VolumeProjectorActor VisibilityOff
    
    # Setup the pipeline to display the ellipsoid
#    set EllipseSource [ vtkEllipseSource New ]
#    $EllipseSource SetThetaResolution 15
#    $EllipseSource SetPhiResolution 15
#    set EllipseTransform [ vtkTransform New ]
#    set EllipseMapper [ vtkPolyDataMapper New ]
#    $EllipseMapper SetInput [ $EllipseSource GetOutput ]
#    set EllipseActor [ vtkActor New ]
#    $EllipseActor SetMapper $EllipseMapper
#    [ $EllipseActor GetProperty ] SetColor 0.0 0.5 0.0
#    [ $EllipseActor GetProperty ] SetOpacity 0.9
#    [ $EllipseActor GetProperty ] SetRepresentationToWireframe
#    $EllipseActor VisibilityOff
    
    set ProbeVisualization [ vtkjoProbeVisualization New ]
    $ProbeVisualization ContextVisualizationOff
    set Representation [ $ProbeVisualization GetProjectionRepresentation ]
    
    set VTKViewer [ $baseviewer GetViewer ] 
    if { [ $VTKViewer IsA "vtkpxGUIOrthogonalViewer" ] == 1 } {
#        [ [ $VTKViewer GetRenderer 3 ] GetRenderer ] AddActor $VolumeProjectorActor
#        [ [ $VTKViewer GetRenderer 3 ] GetRenderer ] AddActor $EllipseActor
        [ [ $VTKViewer GetRenderer 3 ] GetRenderer ] AddActor $ProbeVisualization
    }
    
    return $basewidget
}


itcl::body pxitclvvlinkprojectorgadget::Update { x y z nx ny nz } {

    if { $thisparam($this,dovisualization) == 1 } {
        
        $Representation SetLength $thisparam($this,crosshairlength)
        $Representation SetResolution $thisparam($this,crosshairresolution)
        
#        if { $thisparam($this,usecontext) == 1 } {
#            $Context SetSigmaScalar $thisparam($this,contextcapturerange)
#            $Context SetProbeTip $x $y $z
            
#            if { $thisparam($this,displaycontext) == 1 } {
#            set sigmas [ $Context GetProbeTipStd ]
#            set scalar [ $Context GetSigmaScalar ]
#            puts stderr "ellipse stds = $sigmas"
#            set range [ $Context GetCaptureRange ]
#            $EllipseSource SetRadiusX [ expr {$scalar * sqrt ( [ lindex $sigmas 0 ] ) } ]
#            $EllipseSource SetRadiusY [ expr {$scalar * sqrt ( [ lindex $sigmas 1 ] ) } ]
#            $EllipseSource SetRadiusZ [ expr {$scalar * sqrt ( [ lindex $sigmas 2 ] ) } ]
            
#            $Context GetTransformation $EllipseTransform
#            $EllipseActor SetPosition $x $y $z
#            $EllipseActor VisibilityOn
#            }
#        }
        
        $ProbeVisualization SetTarget [ $VTKViewer GetImage ]
        $ProbeVisualization SetPosition $x $y $z
        $ProbeVisualization SetOrientation $nx $ny $nz
        catch {
            [ $ProbeVisualization GetProjector ] SetParallelProjection $thisparam($this,projectorparallel)
            [ $ProbeVisualization GetProjector ] SetFocalDistance $thisparam($this,projectorfocallength)
            [ $ProbeVisualization GetProjector ] SetMaxProjectionDistance $thisparam($this,projectorsearchdistance)
        }
    }
    }

#----------------------------------------------------------------------------

itcl::body pxitclvvlinkprojectorgadget::UseVisualizationCallback { } {
    if { $thisparam($this,dovisualization) == 0 } {
        $ProbeVisualization VisibilityOff
    } else {
        $ProbeVisualization VisibilityOn
    }
}

#----------------------------------------------------------------------------

itcl::body pxitclvvlinkprojectorgadget::ContextCallback { } {
    if { $thisparam($this,usecontext) == 0 } {
        $ProbeVisualization ContextVisualizationOff
    } else {
        $ProbeVisualization ContextVisualizationOn
    }
}

#----------------------------------------------------------------------------

itcl::body pxitclvvlinkprojectorgadget::DisplayContextCallback { } {
    if { $thisparam($this,displaycontext) == 0 } {
        $EllipseActor VisibilityOff
    } else {
        $EllipseActor VisibilityOn
    }
}

#----------------------------------------------------------------------------

itcl::body pxitclvvlinkprojectorgadget::UpdateEstimateCallback { } {
    # Update the point estimate that the context uses.
    set curve [[ $estimatecontrol GetLandmarks ] GetObject ]
    $curve Compact
    puts stderr "Updating context with estimate (uses [[ $curve GetPoints ] GetNumberOfPoints ] samples)"
    $Context SetPointEstimate [ $curve GetPoints ]
    $Context UpdateEstimate

}

#----------------------------------------------------------------------------

itcl::body pxitclvvlinkprojectorgadget::ContextAddPointsCallback { } {
    # Update the point estimate that the context uses.
    set curve [[ $estimatecontrol GetLandmarks ] GetObject ]
    $curve Compact
    puts stderr "Add points to context (uses [[ $curve GetPoints ] GetNumberOfPoints ] samples)"
    $Context AddPoints [ $curve GetPoints ]
    $Context UpdateEstimate

}

#----------------------------------------------------------------------------

itcl::body pxitclvvlinkprojectorgadget::UpdateLandmarksCallback { } {
    # Update the point estimate that the context uses.
    set curve [[ $landmarkcontrol GetLandmarks ] GetObject ]
    $curve Compact
    puts stderr "Updating landmarks (uses [[ $curve GetPoints ] GetNumberOfPoints ] samples)"
#    $Context SetLandmarks [ $curve GetPoints ]
    $ProbeVisualization SetContextPoints [ $curve GetPoints ]
}

#----------------------------------------------------------------------------

itcl::body pxitclvvlinkprojectorgadget::CreateProjectorControl { parent } {
    
    # Create the projector type GUI components
    set typeFrame [ LabelFrame:create $parent.type -text "Projector" - expand true -fill x ]
    
    frame $typeFrame.vis
    frame $typeFrame.parallel
    frame $typeFrame.options
    
    eval "checkbutton $typeFrame.vis.check -text \"Use\" -variable [ itcl::scope thisparam($this,dovisualization) ] \
        -command { $this UseVisualizationCallback }"
    pack $typeFrame.vis.check -side left -expand false -padx 5

    checkbutton $typeFrame.parallel.check -text "Parallel Projection" -variable [ itcl::scope thisparam($this,projectorparallel) ] 
    pack $typeFrame.parallel.check -side left -expand false -padx 5
    
    iwidgets::entryfield $typeFrame.options.focal \
        -labeltext "Focal Length" -width 5 \
        -textvariable [ itcl::scope thisparam($this,projectorfocallength)] \
        -relief sunken -validate real
    iwidgets::entryfield $typeFrame.options.search \
        -labeltext "Search Distance" -width 5 \
        -textvariable [ itcl::scope thisparam($this,projectorsearchdistance)] \
        -relief sunken -validate real
    pack $typeFrame.options.focal $typeFrame.options.search -side left -expand false -padx 5
    
    pack $typeFrame.vis $typeFrame.parallel $typeFrame.options -side top -expand false -fill x -pady 3

    
    # Create the template GUI components
    set templateFrame [ LabelFrame:create $parent.template -text "Template" - expand true -fill x ]
    frame $templateFrame.crosshair
    frame $templateFrame.circle

    iwidgets::entryfield $templateFrame.crosshair.length \
        -labeltext "Crosshair Length" -width 5 \
        -textvariable [ itcl::scope thisparam($this,crosshairlength)] \
        -relief sunken -validate real
    iwidgets::entryfield $templateFrame.crosshair.res \
        -labeltext "Resolution" -width 5 \
        -textvariable [ itcl::scope thisparam($this,crosshairresolution)] \
        -relief sunken -validate integer
    pack $templateFrame.crosshair.length $templateFrame.crosshair.res \
        -side left -expand false -fill x -pady 3

    iwidgets::entryfield $templateFrame.circle.length \
        -labeltext "Cirlce Radius" -width 5 \
        -textvariable [ itcl::scope thisparam($this,circleradius)] \
        -relief sunken -validate real
    iwidgets::entryfield $templateFrame.circle.res \
        -labeltext "Resolution" -width 5 \
        -textvariable [ itcl::scope thisparam($this,circleresolution)] \
        -relief sunken -validate integer
    pack $templateFrame.circle.length $templateFrame.circle.res \
        -side left -expand false -fill x -pady 3
    
    pack $templateFrame.crosshair $templateFrame.circle -side top -expand false -fill x
    
    
    # Create the context GUI components
    set contextFrame [ LabelFrame:create $parent.context -text "Context" - expand true -fill x ]
    frame $contextFrame.use
    frame $contextFrame.landmarks
    frame $contextFrame.estimate
    frame $contextFrame.options

    eval "checkbutton $contextFrame.use.check -text \"Use\" -variable [ itcl::scope thisparam($this,usecontext) ] \
        -command { $this ContextCallback }"
    pack $contextFrame.use.check -side left -expand false -fill x -pady 3
    
    eval "button $contextFrame.landmarks.button -text \"Update\" -command { $this UpdateLandmarksCallback }"
    set landmarkcontrol [ [ pxitcllandmarksGUI \#auto ] GetThisPointer ]
    $landmarkcontrol configure -description "Landmarks"
    $landmarkcontrol InitializeLite $contextFrame.landmarks.1 2
    pack $contextFrame.landmarks.1 $contextFrame.landmarks.button \
        -side left -expand false -fill x -pady 3

#    eval "button $contextFrame.estimate.button -text \"Update\" -command { $this UpdateEstimateCallback }"
    eval "button $contextFrame.estimate.button -text \"Add\" -command { $this ContextAddPointsCallback }"
    set estimatecontrol [ [ pxitcllandmarksGUI \#auto ] GetThisPointer ]
    $estimatecontrol configure -description "Point Estimate"
    $estimatecontrol InitializeLite $contextFrame.estimate.1 2
    pack $contextFrame.estimate.1 $contextFrame.estimate.button \
        -side left -expand false -fill x -pady 3

    iwidgets::entryfield $contextFrame.options.capture \
        -labeltext "Scalar" -width 5 \
        -textvariable [ itcl::scope thisparam($this,contextcapturerange)] \
        -relief sunken -validate real
    eval "checkbutton $contextFrame.options.check -text \"Display\" -variable [ itcl::scope thisparam($this,displaycontext) ] \
        -command { $this DisplayContextCallback }"
    pack $contextFrame.options.capture $contextFrame.options.check \
        -side left -expand false -fill x -pady 3

     #Pack the context frames
    pack $contextFrame.use $contextFrame.landmarks $contextFrame.estimate $contextFrame.options \
        -side top -expand false -fill x

    
    
    # Pack the 3 main labeled frames in the main window
    pack $parent.type $parent.template $parent.context -side top -expand false -fill both
}



itcl::body pxitclvvlinkprojectorgadget::DelayedInitialize { } {
    puts stderr "vvlinkprojectorgadget::DelayedInit"

    if { $fullyinitialized == 1} {
        return 
    }
    
    # Create the GUI
    set mainFrame [ frame $basewidget.main ]
    pack $mainFrame -side top -expand t -fill both -pady 2 -padx 2
    
    CreateProjectorControl $mainFrame 
    
    set fullyinitialized 1
}


