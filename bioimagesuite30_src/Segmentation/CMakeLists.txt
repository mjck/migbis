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
SET(KITBASE Segmentation)
SET(KIT vtkpx${KITBASE})

INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/${KITBASE})
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Common)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Imaging)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Legacy)
INCLUDE_DIRECTORIES(${ITK_INCLUDE_DIRS})

SET (KITSRCS
#
#
vtkpxLogExponentialFit.cpp
vtkpxImageRobustHistogram.cpp
vtkpxHistogramSegmentation.cpp
vtkpxMRFSegmentation.cpp
#
vtkpxBrainPreSegment.cpp
#
#
vtkpxAbstractBiasFieldCorrection.cpp
vtkpxSliceBiasFieldCorrection.cpp
vtkpxPolynomialBiasFieldCorrection.cpp
#
vtkpxImageTensorSpline.cpp
#
# Old Levelset stuff
#
vtkpxLevelSetUtil.cpp
vtkpxBaseLevelSetFilter.cpp
vtkpxTissueLevelSetFilter.cpp
vtkpxCoupledTissueLevelSetFilter.cpp
itkpxLevelSetCallback.h
itkImageToVTKImageFilter.txx  
itkVTKImageToImageFilter.txx
vtkjwsSignedDanielssonDistanceMapImageFilter.cpp
vtkbisBinaryThinningFilter.cpp
#
# John's Skull Stripping Stuff
#
vtkjoBETHistogram.cpp
#vtkjoBETSurface.cpp
vtkjoBETImageFilter.cpp
#vtkjoRobustBETImageFilter.cpp
vtkjoPolyDataRobustBETTraining.cpp
vtkjoScalePolyData.cpp
#
vtkpxExponentialFitT1.cpp
)

IF (BIOIMAGESUITE3_EXPERIMENTAL)
  SET (KITSRCS
    ${KITSRCS}
    vtkpxApproximateRayleigh.cpp
    vtkpxExponentialFit.cpp
    vtkpxFuzzyCMeansSegmentation.cpp	
    vtkpxMultiEchoSegmentation.cpp
    #vtkpxMonoExponentialFatSegmentation
    #vtkpxBiExponentialFatSegmentation
    #vtkpxSplineFitBiasFieldCorrection
    vtkpxRegionSegmentation.cpp
    #vtkpxCreateFatMap.cpp
    vtkpxBrainStripUtil.cpp
    vtkpxNonLinearFit.cpp
    #
    vtkpxDualResolutionCoupledLevelSetFilter.cpp)
ENDIF (BIOIMAGESUITE3_EXPERIMENTAL)


SET_SOURCE_FILES_PROPERTIES(
vtkpxBaseLevelSetFilter.cpp
ABSTRACT
)

SET_SOURCE_FILES_PROPERTIES(
itkpxLevelSetCallback.h
itkImageToVTKImageFilter.txx
itkVTKImageToImageFilter.txx
 WRAP_EXCLUDE
)

SET(KITLIBS
  vtkpxImagingTCL
  vtkpxImaging
  vtkpxLegacyTCL
  vtkpxLegacy
  vtkpxCommon
  vtkpxCommonTCL
  numerics
  vtkCommonTCL
  ${BIOIMAGESUITE3_COMMON_LIBS}
)

SET(KITJAVALIBS
  vtkpxImagingJava
  vtkpxImaging
  vtkpxLegacyJava
  vtkpxLegacy
  vtkpxCommon
  vtkpxCommonJava
  numerics
  ${BIOIMAGESUITE3_COMMON_JAVALIBS}
)


SET (KITEXTRAFILES
  Segmentation/itkpxSaturateImageFilter.h
  Segmentation/itkpxSaturateImageFilter.txx  
  Segmentation/itkpxTissueSegmentationCoupledLevelSetFunction.h
  Segmentation/itkpxTissueSegmentationCoupledLevelSetFunction.txx
  Segmentation/itkpxTissueSegmentationCoupledLevelSetImageFilter.h
  Segmentation/itkpxTissueSegmentationCoupledLevelSetImageFilter.txx
  Segmentation/itkpxTissueSegmentationLevelSetFunction.h
  Segmentation/itkpxTissueSegmentationLevelSetFunction.txx
  Segmentation/itkpxTissueSegmentationLevelSetImageFilter.h
  Segmentation/itkpxTissueSegmentationLevelSetImageFilter.txx
  Segmentation/itkBinaryThinningImageFilter3D.h
  Segmentation/itkBinaryThinningImageFilter3D.txx
  Segmentation/vtkITKMyUtility.h
  Segmentation/vtkITKUtility.h
  )


# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line


SET (KITTCLSRCS )
bis_complex("Segmentation" ${KIT} ${FILELIST})


IF(BIOIMAGESUITE3_HAVE_COUPLEDLEVELSET_SOURCE)
ADD_EXECUTABLE (TestXiaolanSegmentation TestXiaolanSegmentation.cpp)
ENDIF(BIOIMAGESUITE3_HAVE_COUPLEDLEVELSET_SOURCE)

