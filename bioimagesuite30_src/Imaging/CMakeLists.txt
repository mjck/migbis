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
SET(KITBASE Imaging)
SET(KIT vtkpx${KITBASE})

INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/${KITBASE})
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Common)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Graphics)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Connectivity)
INCLUDE_DIRECTORIES(${ITK_INCLUDE_DIRS})



SET (KITSRCS
vtkbisImageAverageComponents.cpp
vtkpxBinaryImageMedian3D.cpp
vtkpxCreateCheckerBoardImage.cpp
vtkpxCSIStackImages.cpp
vtkpxImageAlgebra.cpp
vtkpxImageCylindricalCrop.cpp
vtkpxImageIDFilter.cpp
vtkpxImageAddGaussianNoise.cpp
vtkpxImageVariableSmoothing.cpp
vtkpxImageFeatureExtract.cpp
vtkpxImageGaussianNoiseSource.cpp
vtkpxImageNormalizeIntensity.cpp
vtkpxImageComputeNormalizedDifference.cpp
vtkpxImageComputeNormalizedRatio.cpp
vtkpxImageMask.cpp
vtkpxImageNormalizeIntensityToReference.cpp
vtkpxImageComputeVOIStats.cpp
vtkpxImagePolarToEuclidean.cpp
vtkpxImageSincInterpolation.cpp
vtkpxImageStitch.cpp
vtkpxImageHistogramEqualization.cpp
vtkpxSimpleImageThreshold.cpp
vtkpxWeightedSmoothImage.cpp
vtkpxImageCorrectNonUniformZ.cpp
vtkpxImageAttenuationCorrection.cpp
vtkpxMaximumImages.cpp
vtkpxImageThresholdedIntersection.cpp
#vtkpxImageAnisotropicDiffusion3D.cpp
vtkpxNormalizedSmoothImage.cpp
vtkxqImageHessian.cpp
vtkxqImageVesselEnhancement.cpp
vtkpxImageReorientTensorOrVector.cpp
vtkdsSpectUtil.cpp
vtkbisObjectmapRegularization.cpp
vtkxqImageKMeansVesselnessFilter.cpp
vtkxqImageODFVesselnessFilter.cpp
vtkxqImageFourierConvolution.cpp
vtkpxAverageImages.cpp
#
#
# From old Nmrlib
#
vtkpxAddMotionToTimeSeries.cpp
#vtkpxComputeFieldMap.cpp
#vtkpxComputeFieldConstant.cpp
#vtkpxComputeShims.cpp
#vtkpxComputeShimsNmr47.cpp
vtkpxComputeTmap.cpp
vtkpxCreateSyntheticFmri.cpp
vtkpxCreateThickSlices.cpp
vtkpxImageAddPhaseCorrection.cpp
vtkpxImageRemoveLinearPhase.cpp
vtkpxMergeFmriConventional.cpp
vtkpxProcessFmri.cpp
vtkpxProcessRawBrukerFIDImage.cpp
#vtkpxExtractMarkersUtil.cpp
#vtkpxExtractCylindricalMarkers.cpp
#vtkpxEpiDistortionModel.cpp
vtknrImageCluster.cpp
#
# MRS Stuff
#
vtkpxPhaseCorrectCSIData.cpp
vtkpxCSIOffsetCorrection.cpp
#pet stuff
#vtkbisMRTM.cpp
#vtkbisMRTM2.cpp
#vtkbisMRTM3.cpp
#vtkmsMRTM2.cpp
# AFNI Inspired/Derived Modules
#
vtkbisWaver.cpp
vtkbisTimeSeriesNormalize.cpp
vtkbisComputeGLM.cpp
#vtkbisImageCluster.cpp
#vtkbisMonteCarlo.cpp
vtkbisCudaFFT.cpp
vtkbisCudaRFFT.cpp
vtkbisCudaConvolution.cpp
vtkbisCudaEdgeDetect.cpp
vtkbisCudaHessian.cpp
#
vtkbisZscoretoPvalue.cpp
vtkbisRtoTmap.cpp
vtkbisFDR.cpp
vtkbisImageUtil.cpp
vtkbisRtoPvalue.cpp
#
vtkbisSiemensMosaicConverter.cpp
vtkbisSiemensDWIArrange.cpp
)

IF(BIOIMAGESUITE3_HAVE_PET_SOURCE)
SET (KITSRCS
${KITSRCS}
vtkbisMRTM.cpp
vtkbisMRTM2.cpp
)
ENDIF(BIOIMAGESUITE3_HAVE_PET_SOURCE)

# Excluded TCL Code 

# ----------------------------------------------
# CUDA Conditional Compilation
# -----------------------------------------------

IF (BIOIMAGESUITE3_USE_CUDA)

  SET (KITSRCS
    ${KITSRCS}
    vtkbisCUDAUtil.cpp
    vtkbisCUDASparseMatrix.cpp
    vtkbisCUDAROICorrelation.cpp
    vtkbisCUDAIntrinsicCorrelation.cpp
    )

  SET(CUDAKIT vtkpx${KITBASE}_Cuda)

  CUDA_INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    )
  
  
  CUDA_ADD_LIBRARY(${CUDAKIT} STATIC 
    edgedetection.cu
    convolution.cu
    convolutionFFT.cu
    convolutionRFFT.cu
    biscudautil.cu
    biscorrelation.cu
    rglm.cu	
    )
  TARGET_LINK_LIBRARIES (${CUDAKIT} ${KITLIBS})
  install (TARGETS ${CUDAKIT} DESTINATION lib)

  


  SET_SOURCE_FILES_PROPERTIES(
    edgedetection.cu
    convolution.cu
    convolutionFFT.cu
    convolutionRFFT.cu
    rglm.cu
    biscudautil.cu
    WRAP_EXCLUDE
    )


  
ELSE(BIOIMAGESUITE3_USE_CUDA)
  SET (CUDAKIT )
ENDIF(BIOIMAGESUITE3_USE_CUDA)
# -----------------------------------------------



SET(KITLIBS
  vtkpxCommon
  vtkpxCommonTCL
  ${CUDAKIT}
  numerics
  vtkCommonTCL
  ${BIOIMAGESUITE3_COMMON_LIBS}
)

SET(KITJAVALIBS
  vtkpxCommonJava
  vtkpxCommon
  ${CUDAKIT}
  numerics
  vtkFilteringJava
  ${BIOIMAGESUITE3_COMMON_JAVALIBS}
)


# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

IF (BIOIMAGESUITE3_USE_CUDA)
SET (KITEXTRAFILES
  Imaging/edgedetection.cu
  Imaging/convolutionFFT.cu
  Imaging/convolutionRFFT.cu
  Imaging/convolution.cu
  Imaging/rglm.cu
  Imaging/book.h
  Imaging/biscudautil.cu
  Imaging/biscudacommon.h
  Imaging/biscorrelation.cu
  Imaging/biscorrelation.h
  )
ENDIF (BIOIMAGESUITE3_USE_CUDA)

SET (KITTCLSRCS )
bis_complex("Imaging" ${KIT} ${FILELIST})


