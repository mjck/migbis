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

package provide pxitclLandmarkControl 1.0

# 	$Id: pxitclLandmarkControl.tcl,v 1.3 2002/07/16 16:16:36 papad Exp papad $	





package require pxvtable 1.0
package require Iwidgets 4.0

itcl::class pxitclLandmarkControl {

    inherit pxitclbaseimagecontrol
    
    private variable vtk_landmarkcontrol 0
    private variable vtk_viewer 0

    constructor { par } {
	pxitclbaseimagecontrol::constructor $par
    } {
	set vtk_landmarkcontrol [ vtkpxGUIBaseCurveControl [ pxvtable::vnewobj ] ]
    }
    
    public method Initialize { inpwidg viewer }
    protected method SetViewer  { viewer }
    public method GetLandmarkControl { } { return $vtk_landmarkcontrol } 
    public method SetImage { img } 
}
# -------------------------------------------------------------------------------------------
::itcl::body pxitclLandmarkControl::Initialize { inpwidg viewer } {

    if { $initialized } { return $basewidget } 

    SetViewer $viewer
    set basewidget [ $vtk_landmarkcontrol Initialize $inpwidg 0 ]
    set initialized 1
    SetTitle "Landmark Control"
   
    eval "wm protocol $basewidget WM_DELETE_WINDOW { wm withdraw $basewidget }"
    return $basewidget
}

# -------------------------------------------------------------------------------------------    
    
# Sets the input
::itcl::body pxitclLandmarkControl::SetViewer { viewer } {

    if { [ $viewer IsA vtkpxGUIBaseImageViewer ] == 0 } {
	return 0
    }

    set vtk_viewer $viewer
    
    if { [ $vtk_viewer IsA vtkpxGUIOrthogonalViewer ] == 1 } {
	$vtk_landmarkcontrol SetCallback $vtk_viewer 201
	$vtk_landmarkcontrol SetRenderer [ [ $vtk_viewer GetRenderer 0 ] GetRenderer ] 0 
	$vtk_landmarkcontrol SetRenderer [ [ $vtk_viewer GetRenderer 1 ] GetRenderer ] 1 
	$vtk_landmarkcontrol SetRenderer [ [ $vtk_viewer GetRenderer 2 ] GetRenderer ] 2 
	$vtk_landmarkcontrol SetRenderer [ [ $vtk_viewer GetRenderer 3 ] GetRenderer ] 3 
    } elseif { [ $vtk_viewer IsA vtkpxGUIMosaicViewer ] == 1 } {
	$vtk_landmarkcontrol SetCallback $vtk_viewer 301
	$vtk_viewer SetEditor $vtk_landmarkcontrol
	$vtk_viewer SetMouseToEditor 1
	$vtk_landmarkcontrol DisableLabels
    }
}

# -------------------------------------------------------------------------------------------
::itcl::body pxitclLandmarkControl::SetImage { img } {

    if { [ $img GetImageSize] < 2 } {return 0 }
    
    if { $vtk_landmarkcontrol !=0 } {
	$vtk_landmarkcontrol SetImage [ $img GetImage ] -1 -1 0
    }
}


