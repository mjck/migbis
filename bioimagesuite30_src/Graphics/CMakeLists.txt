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
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/Graphics)
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/CudaVolumeRendering)

SET(KIT vtkpxGraphics)

SET (3D_TEXTURE_MAP 0)

SET (KITSRCS
vtkpxSurfaceUtil.cpp
vtkpxAppendAndLabelPolyData.cpp
vtkpxAddDisplacementVectorToPolyData.cpp
vtkpxColorMapUtil.cpp
vtkpxCurveEditUtil.cpp
vtkpxEnclosedPipeline.cpp
vtkpxImageSlice.cpp
vtkpx2DImageSlice.cpp
vtkpxMapActivationToPolyData.cpp
vtkpxNormalShrinkPolyData.cpp
vtkpxOrthoImageSlice.cpp
vtkpxVolume.cpp
vtkpxObliqueImageSlice.cpp
vtkpxCameraInfo.cpp
#vtkCroppedVolumeRayCastCompositeFunction.cxx
#vtkCroppedVolumeRayCastFunction.cxx
#vtkCroppedVolumeRayCastMapper.cxx
vtkEllipseSource.cxx
vtkCropVolumeRayCastMapper.cxx
vtkCropVolumeRayCastCompositeHelper.cxx
vtkCropVolumeRayCastCompositeShadeHelper.cxx
vtkCropVolumeRayCastCompositeGOHelper.cxx
vtkCropVolumeRayCastCompositeGOShadeHelper.cxx
#
# Electrode Stuff
#
vtkpxElectrodeGridToSurfaceFilter.cpp
vtkpxElectrodeSource.cpp
vtkpxElectrodeGridSource.cpp
vtkpxElectrodeMultiGridSource.cpp
vtkpxPredictElectrodeStrip.cpp
)

#SET_SOURCE_FILES_PROPERTIES(	
#vtkCroppedVolumeRayCastFunction.cxx
#vtkCroppedVolumeRayCastCompositeFunction.cxx
#ABSTRACT
#)

SET_SOURCE_FILES_PROPERTIES(
#vtkCroppedVolumeRayCastCompositeFunction.cxx
#vtkCroppedVolumeRayCastFunction.cxx
#vtkCroppedVolumeRayCastMapper.cxx
vtkCropVolumeRayCastMapper.cxx
vtkCropVolumeRayCastCompositeHelper.cxx
vtkCropVolumeRayCastCompositeShadeHelper.cxx
vtkCropVolumeRayCastCompositeGOHelper.cxx
vtkCropVolumeRayCastCompositeGOShadeHelper.cxx
WRAP_EXCLUDE
)


SET(KITLIBS
  vtkVolumeRendering
  vtkpxCommon
  vtkpxCommonTCL
  numerics
  ${BIOIMAGESUITE3_COMMON_LIBS}
)

SET(KITJAVALIBS
  vtkVolumeRendering
  vtkpxCommon
  vtkpxCommonJava
  numerics
  ${BIOIMAGESUITE3_COMMON_JAVALIBS}
)




# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

SET (KITEXTRAFILES)
SET (KITTCLSRCS )
bis_complex("Graphics" ${KIT} ${FILELIST})


