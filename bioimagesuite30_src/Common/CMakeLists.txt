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

INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/Common)
INCLUDE_DIRECTORIES(${ITK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(/opt/X11/lib)

IF (BIOIMAGESUITE3_USE_SYSTEM_NIFTI)
   INCLUDE_DIRECTORIES( /usr/include/nifti/ )
ELSE (BIOIMAGESUITE3_USE_SYSTEM_NIFTI)
ENDIF (BIOIMAGESUITE3_USE_SYSTEM_NIFTI)

IF (BIOIMAGESUITE3_USE_SYSTEM_OPENIGTLINK)
   INCLUDE_DIRECTORIES( /usr/include/openigtlink/igtlutil/ )
ELSE (BIOIMAGESUITE3_USE_SYSTEM_OPENIGTLINK)
   INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/igtlutil)
ENDIF (BIOIMAGESUITE3_USE_SYSTEM_OPENIGTLINK)


SET(KIT vtkpxCommon)

SET (KITSRCS
# ---------------------------
# NIFTI1 -- Header Stuff
# ---------------------------
vtkbisNIFTIExtension.cpp
vtkbisNIFTIExtensionsList.cpp
# -------------------------
# Basic Image To Image Filter
# -------------------------
vtkpxSimpleImageToImageFilter.cpp
# -------------------------
# Read / Write To File
# -------------------------
vtkbisImageHeader.cpp
vtkpxAnalyzeImageSource.cpp
#vtkbisVarianSource.cpp
vtkpxAnalyzeImageSourceOld.cpp
vtkpxAnalyzeImageWriter.cpp
vtkpxPickerPrismSource.cpp
vtkpxSignaLXImageSource.cpp
vtkpxEpilepsyCSIReader.cpp
vtkafBVImageReader.cpp
vtkafBVImageWriter.cpp
# -------------------------
# General Utility Code
# -------------------------
vtkpxUtil.cpp
vtkbisTestUtil.cpp
vtkpxGenericUndoStack.cpp
vtkpxDataArrayUndoStack.cpp
# --------------------------
# Math Based Stuff
# --------------------------
vtkpxMath.cpp
vtkpxOptimizer.cpp
vtkpxTestOptimizer.cpp
vtkpxAbstractMatrix.cpp
vtkpxMatrix.cpp
#vtknrMatrix.cpp
vtkpxBSpline.cpp
# ---------------------------
# Image Filters
# --------------------------
vtkpxImageComponentsToFrames.cpp
vtkpxStructuredPointsToStructuredPointsFilter.cpp
vtkpxImageExtract.cpp
vtkpxAppendComponents.cpp
vtkpxImageExtractVOI.cpp
vtkpxImageReplaceSlice.cpp
vtkpxCrossHairSource.cpp
vtkbisCylinderSource.cpp
vtkpxBaseCurve.cpp
# ----------------------------
# Transformations
# ----------------------------
vtkpxTensorBSplineTransform.cpp
# ----------------------------
# Basic Image Filters
# ----------------------------
vtkokImageAccumulate.cpp
vtkbisImageReslice.cpp
vtkbisImageResample.cpp
vtkbisImageReOrient.cpp
# ----------------------------
# Sphere Coloring Utility
# ----------------------------
vtkjoSphereSource.cxx
vtkjoColoredSphereSource.cpp
# ----------------------------
# New Stuff
# ----------------------------
vtkjoStatistics.cpp
vtkjoROCFilter.cpp
)

IF( "${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}" LESS 5.4 )
#
# OpenIGTLink
#
SET (KITSRCS
${KITSRCS}
vtkpxOpenIGTLinkClient.cpp
vtkpxOpenIGTLinkServer.cpp
vtkpxOpenIGTLinkServerObserver.cpp
vtkpxOpenIGTLinkClientObserver.cpp
)
ENDIF( "${VTK_MAJOR_VERSION}.${VTK_MINOR_VERSION}" LESS 5.4 )

IF (NOT BIOIMAGESUITE3_USE_SYSTEM_NIFTI)
  SET(KITSRCS ${KITSRCS}
  # ----------------------------
  # From NIFT1
  # ----------------------------
  nifti1_io.c
  znzlib.c
  )
ENDIF (NOT BIOIMAGESUITE3_USE_SYSTEM_NIFTI)


SET_SOURCE_FILES_PROPERTIES(
  vtkpxOpenIGTLinkServerObserver.cpp	
  vtkpxOpenIGTLinkClientObserver.cpp
  WRAP_EXCLUDE
  )


SET_SOURCE_FILES_PROPERTIES(	
vtkpxSimpleImageToImageFilter
ABSTRACT
)

SET_SOURCE_FILES_PROPERTIES(	
znzlib.c
nifti1_io.c
#nifticdf.c
WRAP_EXCLUDE
)



IF (BIOIMAGESUITE3_USE_VVLINK)
   SET (KITSRCS
       ${KITSRCS}
       vtkpxVVLinkConnectorNew.cpp
      )
   SET (VVLINKLIB vvlcppapi)
   INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/vvlcppapi5)
ENDIF (BIOIMAGESUITE3_USE_VVLINK)



SET_SOURCE_FILES_PROPERTIES(
vtkpxAbstractMatrix
vtkpxGenericUndoStack
ABSTRACT
)

SET(KITLIBS
  ${VVLINKLIB}
  igtlutil
  numerics
  vtkIOTCL
  vtkHybridTCL
  vtkCommonTCL
  ${BIOIMAGESUITE3_COMMON_LIBS}
)

SET(KITJAVALIBS
  ${VVLINKLIB}
  igtlutil
  numerics
  vtkIOTCL
  vtkHybridJava
  vtkCommonJava
   ${BIOIMAGESUITE3_COMMON_JAVALIBS}
)

#IF (NOT APPLE)
SET (KITSRCS ${KITSRCS}
  vtkbisGDCMImageReader.cpp
  vtkbisDICOMReader.cpp)
SET(KITLIBS ${KITLIBS} vtkgdcm gdcmMSFF)
SET(KITJAVALIBS ${KITJAVALIBS} vtkgdcm gdcmMSFF)
#ENDIF (NOT APPLE)

# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

SET (KITEXTRAFILES
Common/vtkTemplateAliasMacro.h)

SET (KITTCLSRCS )
bis_complex("Common" ${KIT} ${FILELIST})




