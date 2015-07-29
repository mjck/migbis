

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
#  Modified
#
# Here is where you can add the name of your local common classes.
#
SET(KIT vtkmpjCommon)

INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/Common)
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/mpjCommon)

SET (KITSRCS
    vtkmpjFrameExtract.cpp
    vtkmpjFrameAppend.cpp
    vtkmpjFrameAverage.cpp
    vtkmpjFramePermutation.cpp
    vtkmpjImageInterleaving.cpp
    vtkmpjImageTwoNorm.cpp
    vtkmpjImageStatistics.cpp
    vtkmpjPriorityQueue.cpp
    vtkmpjTestOptimizer.cpp
#   vtkmpjTensorStreamline.cpp
    vtkmpjImageBoundary.cpp
    vtkmpjTensorStreamlineRegionFilter.cpp
    vtkmpjTensorStreamlineROIFilter.cpp
    vtkmpjTensorStreamlineStatistics.cpp
    vtkmpjStreamlineQuantization.cpp
    vtkmpjImagePointSource.cpp
    vtkmpjImageROISource.cpp
    vtkmpjScalarsToFieldData.cpp
    vtkmpjStreamer.cpp
    vtkmpjVectorStreamline.cpp
    vtkmpjImageSignalToNoiseRatio.cpp
    vtkmpjStreamlineDistance.cpp
    vtkmpjThresholdPolyData.cpp
    vtkmpjThresholdPolyDataByDistance.cpp
    vtkmpjVectorStreamlineStatistics.cpp
    vtkmpjImageHessian.cpp
    vtkmpjImageVesselEnhancement.cpp
    vtkmpjImageVesselMaximum.cpp
    vtkmpjDiffusionProfile.cpp
    vtkmpjVertexSource.cpp
    vtkmpjSphereSubdivisionSource.cpp
    vtkmpjIcosahedronSource.cpp
    vtkmpjImageT2Relax.cpp
    vtkmpjImageNormalize.cpp
    vtkmpjTensorStreamlineWriter.cpp
    vtkmpjTensorStreamlineHistogramWriter.cpp
    vtkmpjStreamlineMapQuantization.cpp
    vtkmpjTensorStreamlineAngleFilter.cpp
    vtkmpjImageExtractComponents.cpp
    vtkmpjImageThresholdConnectivity.cpp
    vtkmpjAbstractInterpolatedTensorField.cpp
    vtkmpjInterpolatedTensorField.cpp
    vtkmpjTensorStreamer.cpp
    vtkmpjTensorStreamLine.cpp
    vtkmpjTensorStreamOld.cpp
    vtkmpjPolyDataToImageFilter.cpp)

SET_SOURCE_FILES_PROPERTIES(
vtkmpjStreamer
vtkmpjAbstractInterpolatedTensorField
vtkmpjTensorStreamer
ABSTRACT
)

SET(KITLIBS
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


bis_complex("mpjCommon" ${KIT} ${FILELIST})


