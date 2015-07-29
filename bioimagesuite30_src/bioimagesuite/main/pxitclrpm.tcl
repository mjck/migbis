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

package provide pxitclrpm 1.0

# 	$Id: pxitclrpm.tcl,v 1.2 2003/08/29 18:56:21 xenios Exp xenios $	





package require  labelframe 1.0
package require  pxtclutil  1.0
package require  pxvtable   1.0
package require  pxtclhelp 1.0
package require  pxitclbasecontrol 1.0
package require  pxitclarticulatedmodel 1.0

itcl::class pxitclrpm {

    inherit pxitclbasecontrol

    protected variable polydatacontrol1 0
    protected variable polydatacontrol2 0
    protected variable landmarkcontrol  0
    protected variable overlaytool      0
    protected variable renWin           0

    protected variable initialized      0
    private   common   thisparam        
    public    variable registrationmethod 0
    public    variable enablearticulated 0
    public    variable sourcesurface 0
    public    variable targetsurface 0
    public    variable vtk_viewer    0
    public    variable renderwindow  0
    public    variable notebook      0
    public    variable tmodelgui     0
    
    protected variable parameterlist ""

    constructor { par pcontrol1 pcontrol2 overlay landcontrol } {
	pxitclbasecontrol::constructor $par 
    } {
	set polydatacontrol1 [ $pcontrol1 GetPolyDataControl ]
	set polydatacontrol2 [ $pcontrol2 GetPolyDataControl ] 
	set landmarkcontrol  [ $landcontrol GetLandmarkControl ]
	set overlaytool      $overlay
	InitializeRpmTool
    }

    private method InitializeRpmTool { } 
    public method Initialize { widget }
    public method DelayedInitialize {  }
    public method CreateUpdateControl { par }
    public method Continue { } 
    public method Stop {  } 
    public method SetSurfaceMode { sur } 
    public method ShowSurfaces { { applytransformation 0 } }
    public method UpdateRPMViewer { visib { mode rpm } } 
    public method UpdateRPMProgress { } 

    public method ComputeLinearRPM  {   }
    public method ComputeNonLinearRPM  {   } 
    public method ComputeArticulatedRPM  {   } 
    public method ComputeCommonRPM { registrationmethod button1 }

    public method InitializeArticulatedModel  { }     


    public method ShowWindow { args }

    public method SaveSetup {  fname }
    public method LoadSetup {  fname }

    # Callback Handlers
    public method HandleUpdateEvent { args }
    public method HandleMouseEvent { mouse stat x1 x2 widgetname args }
    
    # Parameter Stuff
    public method SetDefaultParameters { mode }

    # Create Initial GUIs
    public method CreateCommonRPMGUI { widget }
    public method CreateLinearRPMGUI { widget }
    public method CreateNonLinearRPMGUI { widget }
    public method CreateArticulatedRPMGUI { widget }

    # Set Transformation
    public method SetTransformationToParentControl { xform name }

}

# ---------------------------------------------------------------------------

itcl::body pxitclrpm::SetDefaultParameters { mode } {

    if { $mode == "all" || $mode == "nonlinear" } {

	set md $thisparam($this,nonlinear_transformmode) 
	set thisparam($this,nonlinear_fastthreshold) 3.0
	set thisparam($this,nonlinear_attributesweight) 0.05
	set thisparam($this,nonlinear_corr_mode)  "Labels"
	set thisparam($this,nonlinear_useinitialtransform) 1
	set thisparam($this,nonlinear_matchmode)    "Full RPM (Fast)"
	set thisparam($this,nonlinear_usewls) 1
	
	
	if { $md == "BSpline" } {
	    set thisparam($this,nonlinear_maxnumlandmarks) 1000
	    set thisparam($this,nonlinear_initialtemp) 4
	    set thisparam($this,nonlinear_finaltemp)   2
	    set thisparam($this,nonlinear_initialcps)  24
	    set thisparam($this,nonlinear_finalcps)    12
	    set thisparam($this,nonlinear_annealrate) 0.9;
	    set thisparam($this,nonlinear_initialsmoothfactor) 5.0
	    set thisparam($this,nonlinear_finalsmoothfactor)   1.0
	    set thisparam($this,nonlinear_uselabels)  0

	} elseif { $md == "TPS" } {
	    set thisparam($this,nonlinear_maxnumlandmarks) 70
	    set thisparam($this,nonlinear_initialtemp) 3
	    set thisparam($this,nonlinear_finaltemp)   1
	    set thisparam($this,nonlinear_annealrate) 0.9;
	    set thisparam($this,nonlinear_initialsmoothfactor) 0.25
	    set thisparam($this,nonlinear_uselabels)  1
	} 
    }

    if { $mode == "all" || $mode == "linear" } {

	set thisparam($this,attributesweight) 0.05
	set thisparam($this,fastthreshold) 3.0
	set thisparam($this,matchmode)    "Full RPM (Fast)"
	set thisparam($this,maxnumlandmarks) 600
	set thisparam($this,initialtemp) 6
	set thisparam($this,finaltemp)   3
	set thisparam($this,annealrate) 0.9;
	set thisparam($this,uselabels)  1
	set thisparam($this,corr_mode)  "Labels"
	set thisparam($this,useinitialtransform) 0
	set thisparam($this,usewls) 1
    }

    if { $mode == "all" || $mode == "articulated" } {

	set thisparam($this,artic_useinitialtransform) 0
	set thisparam($this,artic_corr_mode)  "Labels"
	set thisparam($this,artic_attributesweight) 0.05
	set thisparam($this,artic_landmarkset) 1

	set thisparam($this,artic_initialtemp) 10.0
	set thisparam($this,artic_finaltemp)    4.0
	set thisparam($this,artic_annealrate)   0.8
	set thisparam($this,artic_numpoints)    1000
	set thisparam($this,artic_matchmode)    "Full RPM (Fast)"
	set thisparam($this,artic_tolerance)    2.0
	set thisparam($this,artic_jointmode)    "All"
	set thisparam($this,artic_uselandmarks) 0
	set thisparam($this,artic_maxnumlandmarks) 600
	set thisparam($this,artic_fastthreshold) 3.0

	set thisparam($this,gui_artic_thetablend) 0.5
	set thisparam($this,gui_artic_zblend)     5.0
	set thisparam($this,gui_artic_convergence) 0.5
	set thisparam($this,gui_artic_modelname) "none"
	set thisparam($this,gui_artic_numjoints) 0
    }
	
}

itcl::body pxitclrpm::InitializeRpmTool { } {

    set registrationmethod [ vtkpxLinearRPMRegistration  [ pxvtable::vnewobj ]]
    

    set thisparam($this,transformmode) "Rigid"
    set thisparam($this,nonlinear_transformmode) "BSpline"

    set thisparam($this,gui_actmode1) "Points"
    set thisparam($this,gui_actmode2) "Points"

    set vtk_viewer                 [ vtkpxGUIRenderer [ pxvtable::vnewobj ] ]
    set thisparam($this,gui_actor1)    0
    set thisparam($this,gui_actor2)    0
    set thisparam($this,gui_util)                    [ vtkpxSurfaceUtil [ pxvtable::vnewobj]]
    	
    set thisparam($this,gui_match_next) 0
    set thisparam($this,gui_match_count) 0
    set thisparam($this,gui_match_maxcount) Never
    set thisparam($this,gui_renfilt) 0;
    
    set thisparam($this,gui_control1) 0
    set thisparam($this,gui_control2) 0
    set thisparam($this,gui_control3) 0
    
    set thisparam($this,gui_polydatacontrol_ref) $polydatacontrol1
    set thisparam($this,gui_polydatacontrol_trn) $polydatacontrol2
    set thisparam($this,gui_landmarkcontrol) $landmarkcontrol
    set thisparam($this,gui_overlaytool)    $overlaytool


    set thisparam($this,gui_surface_ref) 1;     
    set thisparam($this,gui_surface_trn) 1 ;
    set thisparam($this,gui_pointset) 1;     
    set thisparam($this,gui_matchmatrixmode) "Auto"
    set thisparam($this,gui_enablefeedback) 1;
    set thisparam($this,gui_extradebug) "Normal";

    $this SetDefaultParameters all

    set parameterlist [ list annealrate artic_annealrate artic_attributesweight artic_corr_mode artic_fastthreshold artic_finaltemp artic_initialtemp artic_jointmode artic_landmarkset artic_matchmode artic_maxnumlandmarks artic_numpoints artic_tolerance artic_useinitialtransform artic_uselandmarks attributesweight corr_mode fastthreshold finaltemp gui_artic_convergence gui_artic_modelname gui_artic_numjoints gui_artic_thetablend gui_artic_zblend initialtemp matchmode maxnumlandmarks nonlinear_annealrate nonlinear_attributesweight nonlinear_corr_mode nonlinear_fastthreshold nonlinear_finalcps nonlinear_finalsmoothfactor nonlinear_finaltemp nonlinear_initialcps nonlinear_initialsmoothfactor nonlinear_initialtemp nonlinear_matchmode nonlinear_maxnumlandmarks nonlinear_useinitialtransform nonlinear_uselabels nonlinear_usewls useinitialtransform uselabels usewls ]

}
# ---------------------------------------------------------------------------
itcl::body pxitclrpm::CreateLinearRPMGUI { w } {
	    

    frame $w.c; pack $w.c -side top -expand f -fill x
    checkbutton $w.c.3 -variable [ itcl::scope thisparam($this,useinitialtransform) ] -text "Use Initial Transformation" 
    checkbutton $w.c.4 -variable [ itcl::scope thisparam($this,usewls) ] -text "Weighted Least Squares" 
    pack    $w.c.3 $w.c.4 -side top -fill x -expand f
	
    set base $w

    # Ref/Transform - Surface

    set k 0; 

    # Transform Type
    set cw [ [ iwidgets::labeledwidget $base.$k  -labeltext "Transformation:"  ] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,transformmode) ] "Rigid" "Similarity" "Affine" 
    pack $cw.optmenu  -expand f
    pack $base.$k -side top -expand f  ;incr k
    for { set i 0 } { $i <= 2 } { incr i } { eval "$cw.optmenu.menu entryconfigure $i -command { $this SetDefaultParameters linear }"    }

    set cw [ [ iwidgets::labeledwidget $base.$k -labeltext  "Correspondence:"  ] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,matchmode) ] "ICP" "RPM" "Full RPM" "RPM (Fast)" "Full RPM (Fast)" 
    pack $cw.optmenu  -expand t -fill x
    pack $base.$k -side top -expand f  ;incr k

    set cw [ [ iwidgets::labeledwidget  $base.$k -labeltext "Metric:"] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,corr_mode) ] "Distance" "Labels" "Attributes"
    tk_optionMenu $cw.weight  [ itcl::scope thisparam($this,attributesweight) ] "0.0" "0.01" "0.02" "0.05" "0.1" "0.25" "0.5" "1.0" "5.0" "10.0"
    pack $cw.optmenu $cw.weight -expand f -fill x -side left
    pack $base.$k -side top -expand f  ;incr k



    iwidgets::entryfield $base.$k -labeltext "Max Num Landmarks:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,maxnumlandmarks) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Initial Temperature:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,initialtemp) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Final Temperature:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,finaltemp) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Anneal Rate:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,annealrate) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k


    iwidgets::entryfield $base.$k -labeltext "Search Threshold:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,fastthreshold) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::Labeledwidget::alignlabels $base.3 $base.4 $base.5 $base.6 $base.7 

    frame $w.com; 
    pack $w.com -side bottom -fill x -pady 10
    set thisparam($this,gui_control1) [ button $w.com.b1 -text "Start RPM" ]
    eval "$thisparam($this,gui_control1) configure -command { $this ComputeLinearRPM }"

    eval "button $w.com.load -text \"Load Setup\" -command { $this LoadSetup  \"\" } "
    eval "button $w.com.save -text \"Save Setup\" -command { $this SaveSetup  \"\" } "

    pack $w.com.b1 $w.com.load $w.com.save -side top -fill x -expand t
}

# ---------------------------------------------------------------------------

itcl::body pxitclrpm::CreateArticulatedRPMGUI { w } {

    set tmodelgui [ [ pxitclarticulatedmodel \#auto $this 0  ] GetThisPointer ]
    $tmodelgui Initialize $w.[pxvtable::vnewobj ] 


    set b [ LabelFrame:create  $w.a -text "Model" ]
    pack $w.a -side top -expand f -fill x
    

    #eval "button $b.1 -text \"Initialize Model\" -command { $this InitializeArticulatedModel } "
    eval "button $b.2 -text \"Edit Model\" -command { $tmodelgui ShowWindow \"\" } "
    #    pack $b.1 $b.2  -side left -expand f -fill x
    pack  $b.2  -side left -expand f -fill x



    frame $w.c; pack $w.c -side top -expand f -fill x

    if { $landmarkcontrol !=0 } {
	
	set b [ LabelFrame:create  $w.c.0 -text "Initial Joint Positions" ]

	eval "button $b.0 -text \"Show Landmark Control\" -command { $thisparam($this,gui_landmarkcontrol) Show 1}"
	checkbutton $b.1 -variable [ itcl::scope thisparam($this,artic_uselandmarks) ] -text "Use Initial Joints" 

	set cw [ [ iwidgets::labeledwidget  $b.2 -labeltext "Joint Landmarks:"] childsite ]
	tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,gui_pointset) ] "1" "2" "3" "4" "5" "6" "7" 
	pack $cw.optmenu  -expand f -fill x
	pack $b.2 $b.1 $b.0 -side top -expand f -fill x -padx 2
	pack $w.c.0 -side top -expand false -fill x
    }
    checkbutton $w.c.3 -variable [ itcl::scope thisparam($this,artic_useinitialtransform) ] -text "Use Initial Transformation" 
    pack  $w.c.3 -side top -fill x -expand f
	
    set base $w

    set k 0; 

    # Transform Type
    set cw [ [ iwidgets::labeledwidget $base.$k  -labeltext "Transformation:"  ] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,artic_jointmode) ] "All" "Global" "SpineTail" "Hips" "Knees" 
    pack $cw.optmenu  -expand f
    pack $base.$k -side top -expand f  ;incr k

    set cw [ [ iwidgets::labeledwidget $base.$k -labeltext  "Correspondence:"  ] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,artic_matchmode) ] "ICP" "RPM" "Full RPM" "RPM (Fast)" "Full RPM (Fast)" 
    pack $cw.optmenu  -expand t -fill x
    pack $base.$k -side top -expand f  ;incr k

    set cw [ [ iwidgets::labeledwidget  $base.$k -labeltext "Metric:"] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,artic_corr_mode) ] "Distance" "Labels" "Attributes"
    tk_optionMenu $cw.weight  [ itcl::scope thisparam($this,artic_attributesweight) ] "0.0" "0.01" "0.02" "0.05" "0.1" "0.25" "0.5" "1.0" "5.0" "10.0"
    pack $cw.optmenu $cw.weight -expand f -fill x -side left
    pack $base.$k -side top -expand f  ;incr k



    iwidgets::entryfield $base.$k -labeltext "Max Num Landmarks:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,artic_maxnumlandmarks) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Initial Temperature:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,artic_initialtemp) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Final Temperature:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,artic_finaltemp) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Anneal Rate:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,artic_annealrate) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k


    iwidgets::entryfield $base.$k -labeltext "Search Threshold:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,artic_fastthreshold) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Tolerance:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,artic_tolerance) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k



    iwidgets::Labeledwidget::alignlabels $base.3 $base.4 $base.5 $base.6 $base.7  $base.8

    frame $base.$k -height 2 -bg black; pack $base.$k -side top -expand f -fill x ; incr k



    frame $w.com; 
    pack $w.com -side bottom -fill x -pady 10
    set thisparam($this,artic_gui_control1) [ button $w.com.b1 -text "Start RPM" ]
    eval "$thisparam($this,artic_gui_control1) configure -command { $this ComputeArticulatedRPM }"

    eval "button $w.com.load -text \"Load Setup\" -command { $this LoadSetup \"\" } "
    eval "button $w.com.save -text \"Save Setup\" -command { $this SaveSetup \"\" } "

    pack $w.com.b1 $w.com.load $w.com.save -side top -fill x -expand t
}
# --------------------------------------------------------------------------
itcl::body pxitclrpm::CreateNonLinearRPMGUI { w } {

    frame $w.c
    pack $w.c -side top -fill x -pady 5

    set cw [ [ iwidgets::labeledwidget  $w.c.2 -labeltext "Mode:"] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,nonlinear_corr_mode) ] "None" "Labels" "Attributes"
    tk_optionMenu $cw.weight  [ itcl::scope thisparam($this,nonlinear_attributesweight) ] "0.0" "0.01" "0.02" "0.05" "0.1" "0.25" "0.5" "1.0" "5.0" "10.0"
    pack $cw.optmenu $cw.weight -expand f -fill x -side left

    checkbutton $w.c.3 -variable [ itcl::scope thisparam($this,nonlinear_useinitialtransform) ] -text "Use Initial Transformation" 
    checkbutton $w.c.4 -variable [ itcl::scope thisparam($this,nonlinear_usewls) ] -text "Weighted Least Squares" 
    pack  $w.c.2  $w.c.3 $w.c.4 -side top -fill x -expand f
	
    set base $w

    # Ref/Transform - Surface

    set k 0; 

    # Transform Type
    set cw [ [ iwidgets::labeledwidget $base.$k  -labeltext "Transformation:"  ] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,nonlinear_transformmode) ] "TPS" "BSpline"
    pack $cw.optmenu  -expand f
    pack $base.$k -side top -expand f  ;incr k

    for { set i 0 } { $i <=1 } { incr i } {
	eval "$cw.optmenu.menu entryconfigure $i -command { $this SetDefaultParameters nonlinear }"
    }
    set cw [ [ iwidgets::labeledwidget $base.$k -labeltext  "Correspondence:"  ] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,nonlinear_matchmode) ] "ICP" "RPM" "ICPPlus" "Full RPM" "RPM (Fast)" "Full RPM (Fast)" 
    pack $cw.optmenu  -expand t -fill x
    pack $base.$k -side top -expand f  ;incr k


    iwidgets::entryfield $base.$k -labeltext "Max Num Landmarks:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,nonlinear_maxnumlandmarks) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Initial Temperature:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,nonlinear_initialtemp) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Final Temperature:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,nonlinear_finaltemp) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Anneal Rate:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,nonlinear_annealrate) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Initial Control Spacing:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,nonlinear_initialcps) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Final Control Spacing:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,nonlinear_finalcps) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Initial Smoothness:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,nonlinear_initialsmoothfactor) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k

    iwidgets::entryfield $base.$k -labeltext "Final Smoothness:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,nonlinear_finalsmoothfactor) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k


    iwidgets::entryfield $base.$k -labeltext "Search Threshold:"  -width 10 -validate real -textvariable [ itcl::scope thisparam($this,nonlinear_fastthreshold) ] -relief sunken 
    pack $base.$k -side top -expand f  ;incr k


    iwidgets::Labeledwidget::alignlabels $base.3 $base.4 $base.5 $base.6 $base.7  $base.8 $base.9 $base.10

    frame $w.com; 
    pack $w.com -side bottom -fill x -pady 10
    set thisparam($this,nonlinear_gui_control1) [ button $w.com.b1 -text "Start RPM" ]
    eval "$thisparam($this,nonlinear_gui_control1) configure -command { $this ComputeNonLinearRPM }"

    eval "button $w.com.load -text \"Load Setup\" -command { $this LoadSetup  \"\" } "
    eval "button $w.com.save -text \"Save Setup\" -command { $this SaveSetup  \"\" } "

    pack $w.com.b1 $w.com.load $w.com.save  -side top -fill x -expand t

}

itcl::body pxitclrpm::Initialize { widget } {
	
    set basewidget [ toplevel $widget ]
    wm geometry $basewidget 820x800
    wm withdraw $basewidget
    SetTitle "RPM - Registration"
    set fullyinitialized 0

    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget

}

itcl::body pxitclrpm::DelayedInitialize {  } {

    if { $fullyinitialized ==1 } {
	return
    }

    set pbar [ frame $basewidget.bot ]
    set mbar [ frame $basewidget.top ]
    pack $pbar -side bottom -expand f -fill x -pady 1
    pack $mbar -side top -expand t -fill both -pady 1
   
    frame $mbar.right 

    frame $mbar.left -width 450
    pack $mbar.left -side left -expand f -fill y
    pack $mbar.right -side right -expand t -fill both -padx 2

    CreateProgressBar $pbar
    CreateUpdateControl $mbar.right

    set base [ LabelFrame:create  $mbar.left.1 -text "Common" ]
    pack $mbar.left.1 -side top -expand false -fill x

    set k 0
    set cw [ [ iwidgets::labeledwidget  $base.$k -labeltext "Reference Surface:"] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,gui_surface_ref) ] "1"
    set  vv 0
    catch { set vv [  $polydatacontrol1 GetNumberOfSurfaces ] }
    for { set i 2 } { $i <= $vv } { incr i } { $cw.optmenu.menu add radio -label "$i" -variable [ itcl::scope thisparam($this,gui_surface_ref) ] }

    eval "button $cw.b1 -text \"Ref Sur\" -command { $thisparam($this,gui_polydatacontrol_ref) Show 1}"
    pack $cw.b1 -side right  -expand f -fill x;   
    pack $cw.optmenu  -expand f -fill x;   
    pack $base.$k -side top -expand f  ;incr k

    


    set cw [ [ iwidgets::labeledwidget  $base.$k -labeltext "Transform Surface:"] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,gui_surface_trn) ] "1" 
    set  vv 0
    catch { set vv [  $polydatacontrol2 GetNumberOfSurfaces ] }
    for { set i 2 } { $i <= $vv } { incr i } { $cw.optmenu.menu add radio -label "$i" -variable [ itcl::scope thisparam($this,gui_surface_trn) ] }
    eval "button $cw.b2 -text \"Trn Sur\" -command { $thisparam($this,gui_polydatacontrol_trn) Show 1}"
    pack $cw.b2 -side right  -expand f -fill x;   
    pack $cw.optmenu  -expand f -fill x
    pack $base.$k -side top -expand f  ;incr k



    set cw [ [ iwidgets::labeledwidget $base.$k -labeltext  "Match Matrix Type:"  ] childsite ]
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,gui_matchmatrixmode) ] "Auto" "Sparse" "Dense" 
    pack $cw.optmenu  -expand f
    pack $base.$k -side top -expand f  ;incr k

    set cw [ [ iwidgets::labeledwidget $base.$k -labeltext  "Visual Feedback:"  ] childsite ]
    pack $base.$k -side top -expand f  ;incr k
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,gui_enablefeedback) ] "1" "2" "5" "10" "None"
    pack $cw.optmenu  -expand f -fill x

    set cw [ [ iwidgets::labeledwidget $base.$k -labeltext  "Debug Mode:"  ] childsite ]
    pack $base.$k -side top -expand f  ;incr k
    tk_optionMenu $cw.optmenu [ itcl::scope thisparam($this,gui_extradebug) ] "None" "Normal" "Extra"
    pack $cw.optmenu  -expand f -fill x 


    eval "button $mbar.left.3 -text \"Close Window\" -command {  wm withdraw $basewidget } -fg red"
    pack $mbar.left.3 -side bottom -expand false -fill x -pady 2
    

    set notebook [ iwidgets::tabnotebook $mbar.left.2  -tabpos n -width 250 ]
    pack $notebook -side bottom -expand t -fill both
    CreateLinearRPMGUI       [ $notebook add -label "Linear" ]

    set ok 0
    catch { set ok [ vtkpxPolySimilarityRPMRegistration [ pxvtable::vnewobj ]]; $ok Delete }

    if { $ok == 0 } {
	set enablearticulated 0
    }

    if { $enablearticulated == 1 } {
	CreateArticulatedRPMGUI  [ $notebook add -label "Articulated" ]
    }
    CreateNonLinearRPMGUI    [ $notebook add -label "NonLinear" ]

    $notebook view "Linear"


    set fullyinitialized 1
}
# ---------------------------------------------------------------------------
itcl::body pxitclrpm::CreateUpdateControl { par } {

    set w [ frame $par.rtop -bg black ]
    pack $w -side top -expand f -fill x -pady 1 
    

    set b [ LabelFrame:create  $w.cont -text "" ]
    pack $w.cont -side top -fill x -pady 0 -expand true

    set thisparam($this,gui_control2) [ button $b.b3 -text "Show Surfaces" ]
    set thisparam($this,gui_control3) [ button $b.b4 -text "Apply Transformation" ]
    eval "$thisparam($this,gui_control2) configure -command {  $this ShowSurfaces 0 }"
    eval "$thisparam($this,gui_control3) configure -command {  $this ShowSurfaces 1 }"
    eval "button $b.b5 -text \"Show Transform Control\" -command { $thisparam($this,gui_overlaytool) ShowWindow Transformations }"
    pack $b.b3 $b.b4 $b.b5  -side left -fill x -padx 2 -expand true
    
    
    set wtop [ LabelFrame:create  $par.top -text "Viewer" ]
    pack $par.top -side top -fill both -expand true 
    $par.top configure 

    set ren [ vtkRenderer [ pxvtable::vnewobj ]]

    frame $wtop.bot
    pack $wtop.bot -side bottom -expand f -fill x
    set renWidg [ vtkTkRenderWidget $wtop.r ]
    pack $renWidg -side top -expand true -fill both
    set renderwindow [ $renWidg GetRenderWindow ]
    $renderwindow AddRenderer $ren

    $vtk_viewer BindMouseEvents $renWidg "$this HandleMouseEvent" "$this HandleUpdateEvent"
    $vtk_viewer SetNoPolyClipControls 1
    $vtk_viewer Initialize $wtop.bot $ren 1
    
    set disp [  LabelFrame:create  $par.b1 -text "Display Controls" ]
    pack $par.b1 -side bottom -fill x
    set w   [ frame $disp.b1 ]
    set w2  [ frame $disp.b2 ]
    pack $w $w2 -side bottom -fill x 
    
    label $w2.m -text "Reference:"
    tk_optionMenu $w2.opt   [ itcl::scope thisparam($this,gui_actmode1) ] "Surface" "Wireframe" "Points" "None"
    label $w2.m2 -text "Target:"
    tk_optionMenu $w2.opt2  [ itcl::scope thisparam($this,gui_actmode2) ] "Surface" "Wireframe" "Points" "None"
    pack $w2.m $w2.opt $w2.m2 $w2.opt2 -side left -fill x -expand t
    
    for { set k 0 } { $k <=2 } { incr k } {
	eval "$w2.opt.menu   entryconfigure $k -command { $this SetSurfaceMode 1 }"
	eval "$w2.opt2.menu  entryconfigure $k -command { $this SetSurfaceMode 2 }"
    }
    
    eval "button $w.next -text Continue -command { $this Continue }"
    eval "button $w.stop -text Stop! -command { $this Stop }"
    
    label $w.m -text "Stop every:"
    tk_optionMenu $w.opt [ itcl::scope thisparam($this,gui_match_maxcount) ] "1" "3" "5" "8" "10" "20" "Never"
    pack $w.m $w.opt $w.next $w.stop -side left -fill x -expand t
    
    for { set j 1 } { $j <= 2 } { incr j } {
	set thisparam($this,gui_actor$j) [ vtkActor [ pxvtable::vnewobj ] ]
	set act $thisparam($this,gui_actor$j)
	
	set map [ vtkPolyDataMapper [ pxvtable::vnewobj ] ]
	$act SetMapper $map
	
	$map Delete
	$act SetVisibility 0
	
	[ $act GetProperty] SetRepresentationToPoints
	[ $act GetProperty ] SetAmbient 1.0 
	[ $act GetProperty ] SetDiffuse 0.0
	[ $act GetProperty ] SetOpacity 1.0
	if { $j == 1 } {
	    [ $act GetProperty] SetColor 1.0 0.0 0.0
	} else {
	    [ $act GetProperty] SetColor 0.0 1.0 0.0
	}
	
	[ $vtk_viewer GetRenderer ] AddActor $act
    }
    [ $vtk_viewer GetRenderer ] SetBackground 0.2 0.2 0.3
}
# ---------------------------------------------------------------------------    
# ------------------------
# callback addon commands
# ------------------------

itcl::body pxitclrpm::Continue { } {
    set  thisparam($this,gui_match_next) [ expr  1 - $thisparam($this,gui_match_next) ]
}
    
itcl::body pxitclrpm::Stop {  } {

    if {$thisparam($this,gui_renfilt) != 0 } {
	$thisparam($this,gui_renfilt) SetAbortExecute 1 
	set thisparam($this,gui_match_next) [ expr  1 - $thisparam($this,gui_match_next) ]
    }
}

itcl::body pxitclrpm::SetSurfaceMode { sur } {
	
    set act $thisparam($this,gui_actor$sur)
    set mode $thisparam($this,gui_actmode$sur)
    
    if { $mode != "None" } {
	$act SetVisibility 1
	[ $act GetProperty ] SetRepresentationTo$mode
	if { [  [ $act GetProperty ] GetRepresentationAsString ] == "Surface" } {
	    [ $act GetProperty ] SetAmbient 0.0 
	    [ $act GetProperty ] SetDiffuse 1.0
	    [ $act GetProperty ] SetOpacity 1.0
	} else {
	    [ $act GetProperty ] SetAmbient 1.0 
	    [ $act GetProperty ] SetDiffuse 0.0
	    [ $act GetProperty ] SetOpacity 1.0
	    [ $act GetProperty ] SetLineWidth 1.0
	    [ $act GetProperty ] SetPointSize 3.0
	}
    } else {
	$act SetVisibility 0
    }
    [ [ $vtk_viewer GetRenderer ] GetRenderWindow ] Render 
}


itcl::body pxitclrpm::ShowSurfaces { { applytransformation 0 } } {
 
    set sourcesurface [ $thisparam($this,gui_polydatacontrol_ref) GetSurface [expr $thisparam($this,gui_surface_ref) -1 ]]
    set targetsurface [ $thisparam($this,gui_polydatacontrol_trn) GetSurface [expr $thisparam($this,gui_surface_trn) -1 ]]

    set dat1 $sourcesurface
    set dat2 $targetsurface
    set dat_trn 0

    [ $thisparam($this,gui_actor2) GetMapper ] SetInput $dat2
    [ $thisparam($this,gui_actor2) GetMapper ] Update

    if { $applytransformation !=0 } {

	if { $applytransformation == "articulatedmodel" } {
	    set dat_trn [ $tmodelgui GetTransformation ]
	} else {
	    catch { 
		if { [ [  $overlaytool GetTransformation ] IsA vtkAbstractTransform ] ==1 } {
		    set dat_trn [  $overlaytool GetTransformation ]
		}
	    }
	}
    }
    
    if { $dat_trn == 0 } {
	[ $thisparam($this,gui_actor1) GetMapper ] SetInput $dat1
    } else {
	puts stderr "Using transformation $dat_trn"

	set filt2m [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
	$filt2m SetInput $dat1
	$filt2m SetTransform $dat_trn
	$filt2m Update
	set sur [ vtkPolyData [ pxvtable::vnewobj ]]
	$sur ShallowCopy [ $filt2m GetOutput ]

	[ $thisparam($this,gui_actor1) GetMapper ] SetInput $sur
	$filt2m Delete 
	$sur Delete
    }
    [ $thisparam($this,gui_actor1) GetMapper ] Update
    
    [ $thisparam($this,gui_actor2) GetMapper ]  ScalarVisibilityOn
    [ $thisparam($this,gui_actor1) GetMapper ]  ScalarVisibilityOn
    set r0 0
    set r1 1
    catch {
	set r [ [ [ $dat1 GetPointData ] GetScalars ] GetRange ] 
	set r0 [lindex $r 0 ]
	set r1 [lindex $r 1 ]
    }
    [ $thisparam($this,gui_actor1) GetMapper ]  SetScalarRange $r0 $r1
    [ $thisparam($this,gui_actor2) GetMapper ]  SetScalarRange $r0 $r1
    $thisparam($this,gui_actor1) SetVisibility 1
    $thisparam($this,gui_actor2) SetVisibility 1

    [ $vtk_viewer GetRenderer ] ResetCamera
    [ [ $vtk_viewer GetRenderer ] GetRenderWindow ] Render
}

# ------------------------
#  Update  commands
# -----------------------------------------------------------------------------------

itcl::body pxitclrpm::UpdateRPMViewer { visib { mode rpm } } {

    ShowProgress $thisparam($this,gui_renfilt) "RPM Transform"

    catch {
	if { [ $thisparam($this,gui_renfilt) IsA "vtkpxPolySimilarityRPMRegistration"  ] ==1 } {
	    $tmodelgui UpdateGUIFromTransformation 0
	}
    }
	
    set sourcesurface [ $thisparam($this,gui_polydatacontrol_ref) GetSurface [expr $thisparam($this,gui_surface_ref) -1 ]]
    set targetsurface [ $thisparam($this,gui_polydatacontrol_trn) GetSurface [expr $thisparam($this,gui_surface_trn) -1 ]]

    set dat1 $sourcesurface
    set dat2 $targetsurface

    if { $mode == "rpm" } {
	set dat_trn  [ $thisparam($this,gui_renfilt) GetCurrentTransformation ]
    } elseif { $mode == "articulatedmodel" } {
	set dat_trn [ $tmodelgui GetTransformation ]
    } elseif { $notebook !=0 } {
	set uset 0
	set nt [ $notebook view ]

	if { $nt == 2 } {
	    set uset $thisparam($this,artic_useinitialtransform) 
	} elseif { $nt ==1 } {
	    set uset $thisparam($this,nonlinear_useinitialtransform) 
	} else {
	    set uset $thisparam($this,useinitialtransform) 
	}

	if { $uset == 0 } {
	    set dat_trn 0
	} else {
	    set dat_trn [   $overlaytool GetTransformation ]
	}
    } 
         
    
    [ $thisparam($this,gui_actor2) GetMapper ] SetInput $dat2
    [ $thisparam($this,gui_actor2) GetMapper ] Update
    
    
    #$thisparam($this,gui_util) AddObjectMapLookupTableToSurface $dat1

    if { $dat_trn == 0 } {
	[ $thisparam($this,gui_actor1) GetMapper ] SetInput $dat1
    } else {
	set filt2m [ vtkTransformPolyDataFilter [ pxvtable::vnewobj ] ]
	$filt2m SetInput $dat1
	$filt2m SetTransform $dat_trn
	$filt2m Update
	set sur [ vtkPolyData [ pxvtable::vnewobj ]]
	$sur ShallowCopy [ $filt2m GetOutput ]
	[ $thisparam($this,gui_actor1) GetMapper ] SetInput $sur
	$filt2m Delete 
	$sur Delete
    }

    [ $thisparam($this,gui_actor1) GetMapper ] Update
    
    if { $visib > 0 } {
	[ $thisparam($this,gui_actor2) GetMapper ]  ScalarVisibilityOn
	[ $thisparam($this,gui_actor1) GetMapper ]  ScalarVisibilityOn
	
	set r0 0
	set r1 1
	catch {
	    set r [ [ [ $dat1 GetPointData ] GetScalars ] GetRange ] 
	    set r0 [lindex $r 0 ]
	    set r1 [lindex $r 1 ]
	}
	[ $thisparam($this,gui_actor1) GetMapper ]  SetScalarRange $r0 $r1
	[ $thisparam($this,gui_actor2) GetMapper ]  SetScalarRange $r0 $r1
    }
    
    $thisparam($this,gui_actor1) SetVisibility 1
    $thisparam($this,gui_actor2) SetVisibility 1
    
    [ [ $vtk_viewer GetRenderer ] GetRenderWindow ] Render
    
}

itcl::body pxitclrpm::UpdateRPMProgress { } {
    
    UpdateRPMViewer 0
    update idletasks
    
    incr thisparam($this,gui_match_count)
    if { $thisparam($this,gui_match_count) == $thisparam($this,gui_match_maxcount) } {
	ShowProgress $thisparam($this,gui_renfilt) "RPM Transform -- Paused"
	vwait  [ itcl::scope thisparam($this,gui_match_next) ]
	set thisparam($this,gui_match_count) 0 
	ShowProgress $thisparam($this,gui_renfilt) "RPM Transform"
	incr thisparam($this,gui_match_count) 
    }
    
    if { $thisparam($this,gui_match_count) > $thisparam($this,gui_match_maxcount) } {
	set thisparam($this,gui_match_count) 0
    }
    
    return 0
}

    
# ---------------------------------------------------------------------------------
#  Run RPM !!!
# ---------------------------------------------------------------------------------

itcl::body pxitclrpm::ComputeCommonRPM  { registrationmethod  button1 } { 

    WatchOn 

    set thisparam($this,gui_renfilt) $registrationmethod    
    set thisparam($this,gui_match_count) 0

    $registrationmethod EnableFeedbackOn
    eval "$registrationmethod AddObserver ProgressEvent { $this UpdateRPMProgress }"
    eval "$registrationmethod AddObserver EndEvent      { $this UpdateRPMProgress }"
	
    $registrationmethod SetSource [ $thisparam($this,gui_polydatacontrol_ref) GetSurface [expr $thisparam($this,gui_surface_ref) -1 ]]
    $registrationmethod SetTarget [ $thisparam($this,gui_polydatacontrol_trn) GetSurface [expr $thisparam($this,gui_surface_trn) -1 ]]

    if { $thisparam($this,gui_enablefeedback) == "None" } {
	$registrationmethod SetEnableFeedback 0 
    } else {
	$registrationmethod SetEnableFeedback $thisparam($this,gui_enablefeedback)
    }

    UpdateRPMViewer 1
    [ $vtk_viewer GetRenderer ] ResetCamera
    [ [ $vtk_viewer GetRenderer ] GetRenderWindow ] Render
    
    $button1 configure -state disabled
    $thisparam($this,gui_control2) configure -state disabled
    $thisparam($this,gui_control3) configure -state disabled
    
    switch -exact $thisparam($this,gui_extradebug) {
	"None" {
	    $registrationmethod SetExtraDebug 0
	    $registrationmethod DebugOff
	}
	"Normal" {
	    $registrationmethod SetExtraDebug 0
	    $registrationmethod DebugOn
	}
	"Extra" {
	    $registrationmethod SetExtraDebug 1
	    $registrationmethod DebugOn
	}
    }
    
    $registrationmethod Run
    $button1  configure -state normal
    $thisparam($this,gui_control2) configure -state normal
    $thisparam($this,gui_control3) configure -state normal
    
    if { $thisparam($this,gui_overlaytool) != 0 } {
	set ind [ $this SetTransformationToParentControl [ $registrationmethod GetCurrentTransformation ] rpm ]
	if { $ind >=0 } {
	    ::pxtclutil::Info "RPM Registration Algorithm Completed. The resulting transformation is stored on the overlay tool (index=$ind)"
	}
    } else {
	::pxtclutil::Info "RPM Registration Algorithm Completed!"
    }
	
    set thisparam($this,gui_renfilt) 0
    
    WatchOff
}

# ---------------------------------------------------------------------------------
itcl::body pxitclrpm::SetTransformationToParentControl { xform name } {

    if { $thisparam($this,gui_overlaytool) != 0 } {
	return [ $thisparam($this,gui_overlaytool) AddTransformation $xform  $name ]
    } 

    return -1
}

# ---------------------------------------------------------------------------------

itcl::body pxitclrpm::ComputeLinearRPM  {   } { 

    set attrib 0
    set attribw $thisparam($this,attributesweight) 

    if { $thisparam($this,corr_mode) == "Labels" } { 
	set thisparam($this,uselabels)  1
    } elseif { $thisparam($this,corr_mode) == "Attributes" } { 
	set attrib 1
    }

    catch { $registrationmethod Delete }

    set registrationmethod [ vtkpxLinearRPMRegistration [ pxvtable::vnewobj ]]

    switch -exact $thisparam($this,transformmode) {
	"Rigid"      { 
	    $registrationmethod  SetTransformModeToRigid 
	}
	"Similarity" { 
	    $registrationmethod  SetTransformModeToSimilarity
	}
	"Affine"     { 
	    $registrationmethod  SetTransformModeToAffine 
	}
    }

    
    switch -exact $thisparam($this,matchmode) {
	"ICP"         { $registrationmethod   SetMatchModeToICP }
	"RPM" { $registrationmethod  SetMatchModeToMixture }
	"Full RPM"    { $registrationmethod   SetMatchModeToRPM }
	"RPM (Fast)"       { $registrationmethod   SetMatchModeToMixtureFast }
	"Full RPM (Fast)"    { $registrationmethod   SetMatchModeToRPMFast }
    }
    
    $registrationmethod SetMaximumNumberOfLandmarks $thisparam($this,maxnumlandmarks)
    
    switch -exact $thisparam($this,gui_matchmatrixmode) {
	"Auto"   {    $registrationmethod SetMatchMatrixTypeToAuto }
	"Dense"  {    $registrationmethod SetMatchMatrixTypeToDense }
	"Sparse" {    $registrationmethod SetMatchMatrixTypeToSparse }
    }
    
    $registrationmethod SetUseSymmetricCorrespondence 0
    $registrationmethod SetUseLabels                  $thisparam($this,uselabels)
    $registrationmethod SetUseAttributes              $attrib
    $registrationmethod SetAttributesWeight           $attribw
    $registrationmethod SetUseWeightedLeastSquares    $thisparam($this,usewls)
    $registrationmethod SetUseInitialTransform        $thisparam($this,useinitialtransform)
    $registrationmethod SetInitialTemperature         $thisparam($this,initialtemp)
    $registrationmethod SetFinalTemperature           $thisparam($this,finaltemp)
    $registrationmethod SetFastThreshold              $thisparam($this,fastthreshold)
    $registrationmethod SetAnnealRate                 $thisparam($this,annealrate)
    
    if { $thisparam($this,useinitialtransform) == 1 } {
	$registrationmethod SetInitialTransform [  $overlaytool GetTransformation ]
    }

    ComputeCommonRPM $registrationmethod     $thisparam($this,gui_control1)

}
# ------------------------------------------------------------------------------------------

itcl::body pxitclrpm::ComputeNonLinearRPM  {   } { 


    set attrib 0
    set attribw $thisparam($this,nonlinear_attributesweight) 

    if { $thisparam($this,nonlinear_corr_mode) == "Labels" } { 
	set thisparam($this,nonlinear_uselabels)  1
    } elseif { $thisparam($this,corr_mode) == "Attributes" } { 
	set attrib 1
    }
    catch { $registrationmethod Delete }

    if {  $thisparam($this,transformmode) ==  "TPS" } {
	set registrationmethod [ vtkpxTPSRPMRegistration [ pxvtable::vnewobj ]]
    } else {
	set registrationmethod [ vtkpxBSplineRPMRegistration [ pxvtable::vnewobj ]]
	$registrationmethod SetInitialControlPointSpacing   $thisparam($this,nonlinear_initialcps)
	$registrationmethod SetFinalControlPointSpacing     $thisparam($this,nonlinear_finalcps)
    }

    switch -exact $thisparam($this,nonlinear_matchmode) {
	"ICP"         { $registrationmethod   SetMatchModeToICP }
	"ICPPlus"     { $registrationmethod   SetMatchModeToICPPlus }
	"RPM" { $registrationmethod  SetMatchModeToMixture }
	"Full RPM"    { $registrationmethod   SetMatchModeToRPM }
	"RPM (Fast)"       { $registrationmethod   SetMatchModeToMixtureFast }
	"Full RPM (Fast)"    { $registrationmethod   SetMatchModeToRPMFast }
    }
    
    $registrationmethod SetMaximumNumberOfLandmarks $thisparam($this,nonlinear_maxnumlandmarks)

    switch -exact $thisparam($this,gui_matchmatrixmode) {
	"Auto"   {    $registrationmethod SetMatchMatrixTypeToAuto }
	"Dense"  {    $registrationmethod SetMatchMatrixTypeToDense }
	"Sparse" {    $registrationmethod SetMatchMatrixTypeToSparse }
    }
    
    $registrationmethod SetUseSymmetricCorrespondence 0
    $registrationmethod SetUseLabels                  $thisparam($this,nonlinear_uselabels)
    $registrationmethod SetUseAttributes              $attrib
    $registrationmethod SetAttributesWeight           $attribw
    $registrationmethod SetUseWeightedLeastSquares    $thisparam($this,nonlinear_usewls)
    $registrationmethod SetUseInitialTransform        $thisparam($this,nonlinear_useinitialtransform)
    $registrationmethod SetInitialTemperature         $thisparam($this,nonlinear_initialtemp)
    $registrationmethod SetFinalTemperature           $thisparam($this,nonlinear_finaltemp)
    $registrationmethod SetFastThreshold              $thisparam($this,nonlinear_fastthreshold)
    $registrationmethod SetInitialSmoothnessFactor    $thisparam($this,nonlinear_initialsmoothfactor)
    $registrationmethod SetFinalSmoothnessFactor      $thisparam($this,nonlinear_finalsmoothfactor)
    $registrationmethod SetAnnealRate                 $thisparam($this,nonlinear_annealrate)
    

    if { $thisparam($this,nonlinear_useinitialtransform) == 1 } {
	$registrationmethod SetInitialTransform [  $overlaytool GetTransformation ]
	puts stdout "Using Initial Transform of type [ [ $overlaytool GetTransformation ] GetClassName ]\n\n\n\n"
    } else {
	puts stdout "********** Not Using Initial Transform of type [ [ $overlaytool GetTransformation ] GetClassName ]\n\n\n\n"
    }

    ComputeCommonRPM $registrationmethod  $thisparam($this,nonlinear_gui_control1)
}


::itcl::body pxitclrpm::ShowWindow { args } {

    if { $fullyinitialized == 0 } {
	$this DelayedInitialize
    }
    ::pxitclbasecontrol::ShowWindow $args
    ShowSurfaces 0

}
# ------------------------------------------------------------------------------------------
itcl::body pxitclrpm::InitializeArticulatedModel  { } {

    set curve [ vtkpxBaseCurve [ pxvtable::vnewobj ]]
    set origcurve [ $thisparam($this,gui_landmarkcontrol) GetCollection [expr $thisparam($this,gui_pointset) -1 ]]
    $curve Copy $origcurve 
    $curve Compact
    set points [ $curve GetPoints ]
    
    set tr [  $overlaytool GetTransformation ]
    if { [ $tr IsA "vtkLinearTransform " ] ==1 } {
	puts stderr "Initializing Using Initial Transformation"
	[ $tmodelgui GetTransformation ] InitializeRearLimbPresetRotations $points  $tr
    } else {
	[ $tmodelgui GetTransformation ] InitializeRearLimbPresetRotations $points 
    }
    $curve Delete
}
# ------------------------------------------------------------------------------------------
itcl::body pxitclrpm::ComputeArticulatedRPM  {   } { 


    set tmodel [ $tmodelgui GetTransformation ] 
    if { $tmodel == 0 } {
	::pxtclutil::Warning "Cannot Compute Articulated RPM Registration. No Model Defined\n"
	$tmodelgui ShowWindow \"\"
	return
    }


    set attrib 0
    set attribw $thisparam($this,artic_attributesweight) 

    if { $thisparam($this,artic_corr_mode) == "Labels" } { 
	set thisparam($this,artic_uselabels)  1
    } elseif { $thisparam($this,artic_corr_mode) == "Attributes" } { 
	set attrib 1
    }
    catch { $registrationmethod Delete }

    set registrationmethod [ vtkpxPolySimilarityRPMRegistration [ pxvtable::vnewobj ]]

    switch -exact $thisparam($this,artic_matchmode) {
	"ICP"         { $registrationmethod   SetMatchModeToICP }
	"ICPPlus"     { $registrationmethod   SetMatchModeToICPPlus }
	"RPM" { $registrationmethod  SetMatchModeToMixture }
	"Full RPM"    { $registrationmethod   SetMatchModeToRPM }
	"RPM (Fast)"       { $registrationmethod   SetMatchModeToMixtureFast }
	"Full RPM (Fast)"    { $registrationmethod   SetMatchModeToRPMFast }
    }
    
    $registrationmethod SetMaximumNumberOfLandmarks $thisparam($this,artic_maxnumlandmarks)

    switch -exact $thisparam($this,gui_matchmatrixmode) {
	"Auto"   {    $registrationmethod SetMatchMatrixTypeToAuto }
	"Dense"  {    $registrationmethod SetMatchMatrixTypeToDense }
	"Sparse" {    $registrationmethod SetMatchMatrixTypeToSparse }
    }
    
    $registrationmethod SetUseSymmetricCorrespondence 0
    $registrationmethod SetUseLabels                  $thisparam($this,artic_uselabels)
    $registrationmethod SetUseAttributes              $attrib
    $registrationmethod SetAttributesWeight           $attribw
    $registrationmethod SetUseWeightedLeastSquares    1


    $registrationmethod SetInitialTemperature         $thisparam($this,artic_initialtemp)
    $registrationmethod SetFinalTemperature           $thisparam($this,artic_finaltemp)
    $registrationmethod SetFastThreshold              $thisparam($this,artic_fastthreshold)
    $registrationmethod SetAnnealRate                 $thisparam($this,artic_annealrate)
    $registrationmethod SetTolerance                  $thisparam($this,artic_tolerance)
    $registrationmethod SetUseInitialTransform        $thisparam($this,artic_useinitialtransform)    

    if { $thisparam($this,artic_useinitialtransform) == 1 } {
	
	if { $thisparam($this,artic_uselandmarks) == 1 } {
	    $this InitializeArticulatedModel 
	}

	if { $thisparam($this,artic_useinitialtransform) == 1 } {
	    $registrationmethod SetInitialTransform [  $overlaytool GetTransformation ]
	}
    } 
   
    set tr [ $registrationmethod GetCurrentTransformation ]
    $tr SetRegionLabel [ $tmodel GetRegionLabel ]
    set col [ $tr  GetTransformationCollection ]
    $col RemoveAllItems

    set numberofparts [ $tmodel GetNumberOfTransformations ]
    set maxpart $numberofparts

    $tr SetTransform 0 [ $tmodel GetTransform 0 ]

    
    for { set i 1 } { $i< $numberofparts } { incr i } {
	set tsim [ $tmodel GetTransform $i ]
	set name [ $tsim GetJointName ]
	set flag "enabled"

	switch -exact $thisparam($this,artic_jointmode) {
	    "Global" {
		set flag "disabled"
	    }
	    "SpineTail" {
		if { [ string first "Spine" $name ] == -1 && 
		     [ string first "Tail"  $name ] == - 1 } {
		    set flag "disabled"
		}
	    }
	    "Hips" {
		if { [ string first "Knee" $name ] >=0 || 
		     [ string first "Foot"  $name ] >=0 } {
		    set flag "disabled"
		}
		
	    }
	    "Knees" {
		if { [ string first "Foot"  $name ] >=0 } {
		    set flag "disabled"
		}
	    }
	}

	if { $flag=="disabled" } {
	    $tsim SetActive 0
	}

	set blend($i) [ $tsim GetBlendedMode ]
	$tsim SetBlendedMode 0
	$tr SetTransform $i $tsim
	puts stderr "Name $name --> $flag  [ [ $tsim GetSimilarityTransform ] GetTranslationScale ]"
    }
    puts stderr "\n"

    
    

    ComputeCommonRPM $registrationmethod  $thisparam($this,artic_gui_control1)


    for { set i 1 } { $i< $numberofparts } { incr i } {
	set tsim [ $tmodel GetTransform $i ]
	$tsim SetActive 1
	$tsim SetBlendedMode $blend($i)
    }
}


# ------------------------------------------------------------------------------------------
itcl::body pxitclrpm::SaveSetup {  fname } {

    set typelist { 
	{"Rpm setup file" {.srpm}} 
	{"All Files" {*}}
    }
	    
    if { [ string length $fname ] < 2 } {
	set fname [tk_getSaveFile -title "Save RPM Parameters" -filetypes $typelist]
    }
    
    if { [string length $fname ] < 2 } {
	return 0;
    }

    set fileid [open $fname w]
    
    puts $fileid "#RPM Parameter File\n-----------------------------------"
    for { set i 0 } { $i < [ llength $parameterlist ] } { incr i } {
	set key [ lindex $parameterlist $i ]
	puts $fileid "$key:$thisparam($this,$key)"
    }
    close $fileid
    return 1
}
# ------------------------------------------------------------------------------------------
itcl::body pxitclrpm::LoadSetup { fname } {

    set typelist { 
	{"Rpm setup file" {.srpm}} 
	{"All Files" {*}}
    }
	    
    if { [ string length $fname ] < 2 } {
	set fname [tk_getOpenFile -title "Load RPM Parameters" -filetypes $typelist]
    }
    
    if { [string length $fname ] < 2 } {
	return 0;
    }
    
    if { [ file exists $fname ] == 0 } {
	return 0
    }

    set fileid [open $fname r]
    set line ""
    gets $fileid line

    if { [ string compare $line "#RPM Parameter File" ] != 0 } {
	::pxtclutil::Warning "Cannot read RPM Parameter File\n Bad Header = $line \n"
	close $fileid
	return 0
    }
    # Skip Underline !!!

    while { [ gets $fileid line ] >=0 } {
	set index [string first ":" $line]
	if { $index > 0 }  {
	    set len   [string length $line]
	    set key   [string trim   [string range $line 0 [expr $index - 1]]]
	    set value [string trim [string range $line [expr $index +1] [expr $len -1]]]
	    set thisparam($this,$key)  $value 
	}
    }
    close $fileid
    return 1
}
# ------------------------------------------------------------------------------------------
itcl::body pxitclrpm::HandleUpdateEvent { args } {
    
    if {$renderwindow != 0 } {
	$renderwindow Render
    }
}
# ---------------------------------------------------------------------------------
itcl::body pxitclrpm::HandleMouseEvent { mouse stat x1 x2 widgetname args} {
    
    if { $vtk_viewer == 0 } {
	return
    }
    
    # Need to flip y-axis vtk counts from bottom tk counts from top !!!
    set wd [ $renderwindow GetSize ]
    set x2 [ expr [lindex $wd 1 ] - $x2 ]
    $vtk_viewer HandleMouseButtonEvent $mouse $stat $x1 $x2
    
}
# ---------------------------------------------------------------------------------


