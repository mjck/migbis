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





#
# Source files
#
# Here is where you can add the name of your local common classes.
#

SET(KITSCRIPTS
    pkgIndex.tcl
    pxitclHistogramWidget.tcl
    pxitclLandmarkControl.tcl
    pxitclbaseimagecontrol.tcl
    pxitclbrainstrip.tcl
    pxitclatlasgadget.tcl
    pxitclyaleatlasgadget.tcl
    pxitclwfuatlasgadget.tcl
    pxitclanimationgadget.tcl
    pxitcldistributiontablegadget.tcl
    pxitclcreatesolidcontrol.tcl
    pxitclimagenormalization.tcl
    pxitclsplineobjectmap.tcl
    pxitclsubjectentry.tcl
    pxitclsurfaceobjectmap.tcl
    pxitclElectrodeMultiControl.tcl
    pxitclPolyDataControl.tcl
    pxitclarticulatedmodel.tcl
    pxitclbase4dimageviewer.tcl
    pxitclbasecontrol.tcl
    pxitclbaseimageviewer.tcl
    pxitcloverlaytool.tcl
    pxitclfilelistmenu.tcl
    pxitclcsigadget.tcl
    pxitclexpfit.tcl
    pxitclimageimport.tcl
    pxitcllistselector.tcl
    pxitclmanualsurfacetransformation.tcl
    pxitclmousechopper.tcl
    pxitclrpm.tcl
    pxitclsplinestackobjectmap.tcl
    pxitclvvlinkgadget.tcl
    pxitclopenigtlinkgadget.tcl
    pxitclmanualtransformation.tcl 
    alarkutility.tcl
    pxitclvvlinkprojectorgadget.tcl    
)

SET(KITAPPS
    pxitcl4dsurfaceeditor.tcl
    pxitclbaresimpleviewer.tcl
    pxitclbrainregister.tcl
    pxitclbrainsegment.tcl
    pxitcldualmultisubject.tcl
    pxitclelectrodeeditor.tcl
    pxitclmouseregister.tcl
    pxitclmousesegment.tcl
    pxitclmultisubjectaverage.tcl
    pxitclsimpleeditor.tcl
    pxitclsimpleviewer.tcl
    pxitclsplineeditor.tcl
    pxitclsurfaceeditor.tcl
    pxitclvolumeviewer.tcl
    pxitclorthoviewer.tcl
    bis.tcl
    bis_fmrisetup.tcl
    bis_fmri.tcl
    bis_databasetool.tcl
    alarkviewer.tcl
    bis_parameterGUI.tcl
    pxitclvvlinkviewer.tcl
)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line
bis_complex_tcl(bioimagesuite/main ${FILELIST2} ${FILELIST})

