

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
# Here is where you can add the name of your local common classes.
#
SET(KIT vtkmpjGraphics)

INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/Common)
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/mpjCommon)
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/mpjGraphics)
	
SET (KITSRCS
     vtkmpjTensorGlyphs.cpp
     vtkmpjDiffusionProfile.cpp
     vtkmpjTubeFilter.cpp
     vtkmpjRibbonFilter.cpp
     vtkmpjRationalGaussianSheet.cpp
     vtkmpjVertexSource.cpp
     vtkmpjSphereSubdivisionSource.cpp
     vtkmpjIcosahedronSource.cpp
     vtkmpjToroidSource.cpp
     vtkmpjToroidalSource.cpp
     vtkmpjToroidalExtSource.cpp
     vtkmpjSuperToroidalSource.cpp
     vtkmpjSuperToroidalExtSource.cpp
     vtkmpjEllipsoidSource.cpp
     vtkmpjSuperquadricsSource.cpp
     vtkmpj3DOrientationMarkerWidget.cpp
)

SET(KITLIBS
  vtkmpjGraphics
  vtkmpjCommon
  numerics
  vtkpxCommon
  vtkpxCommonTCL
  vtkCommonTCL
  ${BIOIMAGESUITE3_COMMON_LIBS}
)

SET(KITJAVALIBS
  vtkpxCommonJava
  vtkpxCommon
  numerics 
  ${BIOIMAGESUITE3_COMMON_JAVALIBS}
)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

SET (KITEXTRAFILES)
SET (KITTCLSRCS )


bis_complex("mpjGraphics" ${KIT} ${FILELIST})
