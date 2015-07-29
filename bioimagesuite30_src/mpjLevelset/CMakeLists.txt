

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
SET(KIT vtkmpjLevelset)

INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/Common)
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/mpjCommon)
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/mpjLevelset)

SET (KITSRCS
     vtkmpjExtractLevelSet.cpp
     vtkmpjFastMarchingMethod.cpp
     vtkmpjFastMarchingExtension.cpp
     vtkmpjLevelSetNormal.cpp
     vtkmpjLevelSetNarrowBandFilter.cpp
     vtkmpjLevelSetSignedDistance.cpp
     vtkmpjLevelSetMethod.cpp
     vtkmpjLevelSetTest.cpp
     vtkmpjFastSweepingMethod.cpp
     vtkmpjLaxFriedrichSweepingMethod.cpp
     vtkmpjImageCharacteristics.cpp
     vtkmpjLaxFriedrichEuclidean.cpp
     vtkmpjLaxFriedrichTest.cpp
     vtkmpjLaxConnectivity.cpp
     vtkmpjLaxFriedrichSweeping.cpp
     vtkmpjFastMarchingConnectivity.cpp
     vtkmpjFastMarchingImageFilter.cpp
     vtkmpjLevelSetNeighborhoodExtractor.cpp
     vtkmpjReinitializeLevelSetImageFilter.cpp
     vtkmpjFastMarchingVesselConnectivity.cpp
)

SET(KITLIBS
  vtkpxCommonTCL
  vtkmpjCommon
  vtkmpjCommonTCL
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
SET (KITEXTRAFILES)
SET (KITTCLSRCS )


bis_complex("mpjLevelset" ${KIT} ${FILELIST})


