

#BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
#BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
#BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
#BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
#BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
#BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


#
# Source files
#


SET(KITSCRIPTS
    pkgIndex.tcl
    mpjfastmarching.tcl
    mpjfibertracking.tcl
    mpjfibertrackingold.tcl
    mpjguiicosahedron.tcl
    mpjguilookuptable.tcl
    mpjguiobject.tcl
    mpjguisphere.tcl
    mpjmultiimagegui.tcl
    mpjmultiobjectgui.tcl
    mpjtensoranalysis.tcl
    mpjtensorutility.tcl
    mpjvesselanalysis.tcl
    mpjvesseltracking.tcl
    mpjvesselutility.tcl
    mpjmultibtensorutility.tcl
    mpjsheettracking.tcl
    vtkmpjcontrib.tcl
)

SET(KITAPPS
    mpjcardiacanalysis.tcl
    mpjcardiacangle.tcl
    mpjconnectivity.tcl
    mpjdelaunay3d.tcl
    mpjepicardium.tcl
    mpjestimates0.tcl
    mpjfmmtool.tcl
    mpjimage2points.tcl
    mpjinterleave.tcl
    mpjarithmetic.tcl
    mpjdiffusiontool.tcl
    mpjextractframe.tcl
    mpjinterleave.tcl
    mpjlevelsetnormal.tcl
    mpjraw2analyze.tcl
    mpjsigna2analyze.tcl
    mpjsurfacetoimage.tcl
    mpjsurfacexform.tcl
    mpjt2relax.tcl
    mpjvesseltool.tcl
)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line
bis_complex_tcl(bioimagesuite/mjack ${FILELIST2} ${FILELIST})
