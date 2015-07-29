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



SET(KITBASE DemonsRegistration)
SET(KIT vtkpx${KITBASE})

SET(KIT vtkjws${KITBASE})

INCLUDE_DIRECTORIES(${ITK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/${KITBASE})
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/Common)
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/numerics)
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/Segmentation)
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/Registration)


# -------------------- Add Files Here --------------------
SET (SYMMETRIC 1)

IF(${SYMMETRIC})
  SET (KITSRCS
    vtkjwsSymmetricDemonsRegistration.cpp
    OtherFunctions.cxx
    #DeformableRegistration.cxx 
    #itkjwsDemonsRegistrationFilter.txx  
    #itkjwsDemonsRegistrationFunction.txx
    itkjwsPDEDeformableRegistrationFilter.txx
    itkjwsDenseFiniteDifferenceImageFilter.txx
    itkjwsSymmetricForcesDemonsRegistrationFilter.txx
    itkjwsSymmetricForcesDemonsRegistrationFunction.txx
    )
  
ELSE (${SYMMETRIC})
  SET (KITSRCS
    vtkjwsDemonsRegistration.cpp
    OtherFunctions.cxx
    DeformableRegistration.cxx 
    itkjwsDemonsRegistrationFilter.txx  
    itkjwsDemonsRegistrationFunction.txx
    itkjwsPDEDeformableRegistrationFilter.txx
    itkjwsDenseFiniteDifferenceImageFilter.txx
    itkjwsSymmetricForcesDemonsRegistrationFilter.txx
    itkjwsSymmetricForcesDemonsRegistrationFunction.txx
    )
ENDIF (${SYMMETRIC})


SET_SOURCE_FILES_PROPERTIES(
OtherFunctions.cxx
DeformableRegistration.cxx 
itkjwsDemonsRegistrationFilter.txx  
itkjwsDemonsRegistrationFunction.txx
itkjwsPDEDeformableRegistrationFilter.txx
itkjwsDenseFiniteDifferenceImageFilter.txx
itkjwsSymmetricForcesDemonsRegistrationFilter.txx
itkjwsSymmetricForcesDemonsRegistrationFunction.txx
WRAP_EXCLUDE
)



SET(KITLIBS
  vtkpxCommon
  vtkpxCommonTCL
  numerics
  vtkpxRegistration
  vtkpxRegistrationTCL
  vtkpxSegmentation
  vtkpxSegmentationTCL
  ${BIOIMAGESUITE3_COMMON_LIBS}
)


# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

SET (KITEXTRAFILES
DemonsRegistration/wook.h
DemonsRegistration/jwsNewMain.cxx
DemonsRegistration/jwsSymmetricNewMain.cxx
DemonsRegistration/jwsReadWriteImages.cpp
DemonsRegistration/jwsReadWriteImages.h
DemonsRegistration/jwsReadWriteImagesTest.cpp
DemonsRegistration/jwsForRewritingPreprocess.cpp
DemonsRegistration/DeformableRegistration.cxx 
DemonsRegistration/itkjwsDemonsRegistrationFilter.txx  
DemonsRegistration/itkjwsDemonsRegistrationFunction.txx
DemonsRegistration/itkjwsPDEDeformableRegistrationFilter.txx
DemonsRegistration/itkjwsDenseFiniteDifferenceImageFilter.txx
DemonsRegistration/itkjwsSymmetricForcesDemonsRegistrationFilter.txx
DemonsRegistration/itkjwsSymmetricForcesDemonsRegistrationFunction.txx
DemonsRegistration/itkBinaryMorphologicalClosingImageFilter.txx
DemonsRegistration/itkjwsDemonsRegistrationFilter.h  
DemonsRegistration/itkjwsDemonsRegistrationFunction.h
DemonsRegistration/itkjwsPDEDeformableRegistrationFilter.h
DemonsRegistration/itkjwsDenseFiniteDifferenceImageFilter.h
DemonsRegistration/itkjwsSymmetricForcesDemonsRegistrationFilter.h
DemonsRegistration/itkjwsSymmetricForcesDemonsRegistrationFunction.h
DemonsRegistration/itkBinaryMorphologicalClosingImageFilter.h
DemonsRegistration/itkjwsMultiResolutionPDEDeformableRegistration.h    
DemonsRegistration/itkjwsPDEDeformableRegistrationFilter.txx
DemonsRegistration/itkjwsMultiResolutionPDEDeformableRegistration.txx  
DemonsRegistration/itkjwsPDEDeformableRegistrationFunction.h
DemonsRegistration/itkjwsPDEDeformableRegistrationFilter.h
DemonsRegistration/firstExtendedDemon.h
DemonsRegistration/secondExtendedDemon.h
DemonsRegistration/thirdDemon.h
DemonsRegistration/firstExtendedSymmetricDemon.h
DemonsRegistration/secondExtendedSymmetricDemon.h
DemonsRegistration/thirdSymmetricDemon.h
)

SET (KITTCLSRCS )
bis_complex("DemonsRegistration" ${KIT} ${FILELIST})

ADD_EXECUTABLE(SerialEDemon_intensity 
	jwsNewMain.cxx
	jwsReadWriteImages.cpp
	jwsReadWriteImagesTest.cpp
	jwsForRewritingPreprocess.cpp
	OtherFunctions.cxx
	DeformableRegistration.cxx 
	itkjwsDemonsRegistrationFilter.txx  
	itkjwsDemonsRegistrationFunction.txx
	itkjwsPDEDeformableRegistrationFilter.txx
	itkjwsDenseFiniteDifferenceImageFilter.txx
) 
TARGET_LINK_LIBRARIES(SerialEDemon_intensity ITKNumerics ITKIO ITKStatistics)


ADD_EXECUTABLE(SerialSymmetricEDemon_intensity 
	jwsSymmetricNewMain.cxx
	jwsReadWriteImages.cpp
	jwsReadWriteImagesTest.cpp
	jwsForRewritingPreprocess.cpp
	OtherFunctions.cxx
#	DeformableRegistration.cxx 
#	itkjwsDemonsRegistrationFilter.txx  
#	itkjwsDemonsRegistrationFunction.txx
	itkjwsPDEDeformableRegistrationFilter.txx
	itkjwsDenseFiniteDifferenceImageFilter.txx
	itkjwsSymmetricForcesDemonsRegistrationFilter.txx
	itkjwsSymmetricForcesDemonsRegistrationFunction.txx
) 
TARGET_LINK_LIBRARIES(SerialSymmetricEDemon_intensity ITKNumerics ITKIO ITKStatistics)




