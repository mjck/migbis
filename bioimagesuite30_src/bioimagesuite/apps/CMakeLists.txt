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
    pxappscommon.tcl
)

SET(KITAPPS
    pxtonifti.tcl
    pxroitoroi.tcl
    pxmat_headerinfo.tcl
    pxmat_surfaceinfo.tcl
    pxmat_flip.tcl
    pxmat_reorientimage.tcl
    pxmat_smoothimage.tcl
    pxmat_thresholdimage.tcl
    pxmat_resampleimage.tcl
    pxmat_create4dimage.tcl
    pxmat_split4dimage.tcl
    pxmat_resliceimage.tcl
    pxmat_reorient_tensor.tcl
    pxmat_inverttransform.tcl
    pxmat_displacementfield.tcl
    pxmultijacobian.tcl
    pxsinglejacobian.tcl
    pxcomputeaverage.tcl
    pxmultiregister_int.tcl
    pxpairwiseregister.tcl
    pxmat_register.tcl
    pxmat_distortioncorrection.tcl
    pxmat_pointregister.tcl
    pxmat_integratedregistration.tcl
    pxmat_computedistance.tcl
    pxmat_computeoverlap.tcl
    pxmat_computesimilarity.tcl
    pxmat_computestrains.tcl
    pxmat_computeaveragebrain.tcl
    pxmultisegment.tcl
    pxmat_fastpostprocess.tcl
    pxmat_segment.tcl
    pxmat_biasfield.tcl
    pxmat_reslicect.tcl
    bisserver.tcl
    bisclient.tcl
    biscomputecorrelations.tcl
)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line
bis_complex_tcl(bioimagesuite/apps ${FILELIST2} ${FILELIST})
