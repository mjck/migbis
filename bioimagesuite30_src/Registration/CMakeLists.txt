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


# ------------------------------------------------------------------------
#
# Some of the code in this directory file derives from original work
# in the vtkCISG Toolkit
# that was released under the terms of 
# the GNU General Public License (either version 2,
# of the License, or any later version.)
# 
#  Authors   : Thomas Hartkens <thomas@hartkens.de>
#              Daniel Rueckert <d.rueckert@doc.ic.ac.uk>
#	        Julia Schnabel  <julia.schnabel@kcl.ac.uk>
#	        under the supervision of Dave Hawkes and Derek Hill
#  Web       : http:#www.image-registration.com
#  Copyright : King's College London
#              Div. of Radiological Sciences, 
#              Computational Imaging Science Group, 1997 - 2000
#	        http://www-ipg.umds.ac.uk/cisg
#
# -----------------------------------------------------------------------

#
# Source files
#
# Here is where you can add the name of your local common classes.
#
SET(KITBASE Registration)
SET(KIT vtkpx${KITBASE})

INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/${KITBASE})
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Common)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Imaging)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Graphics)
INCLUDE_DIRECTORIES(${ITK_INCLUDE_DIRS})

SET (KITSRCS
# Basic Code
vtkpxTransformationUtil.cpp
vtkbisTransformationTestUtil.cpp
vtkpxJointHistogram.cpp
vtkpxRegistration.cpp
vtkpxSparseMatrix.cpp
#
# Transformation
#
vtkpxLinearTransform.cpp
vtkpxBaseGridTransform.cpp
vtkpxGridTransform.cpp
vtkpxComboTransform.cpp
vtkpxWeightedLandmarkTransform.cpp

#
# Intensity Based
#
vtkpxAbstractIntensityRegistration.cpp
vtkpxLinearRegistration.cpp
vtkpxMidlineSymmetricRegistration.cpp
vtkpxNonLinearRegistration.cpp
vtkpxNonLinearLabelRegistration.cpp
vtkpxDistortionCorrection.cpp
# 
# RPM Stuff
#
vtkpxRPMCorrespondenceFinder.cpp
vtkpxAbstractRPMRegistration.cpp
vtkpxLinearRPMRegistration.cpp
vtkpxTPSRPMRegistration.cpp
vtkpxBSplineRPMRegistration.cpp
vtkpxBSplinePriorRPMRegistration.cpp
#
# Integrated Stuff
#
vtkpxNonLinearIntegratedRegistration.cpp
#
vtkpxLinearMotionCorrect.cpp
vtkokKochCoefficientSource.cpp
#
vtkpxSimilarityTransform.cpp
vtkpxPolyAbstractTransform.cpp
vtkpxPolySimilarityTransform.cpp
vtkpxArticulatedJointTransform.cpp



)

SET_SOURCE_FILES_PROPERTIES(
vtkpxBaseGridTransform.cpp
ABSTRACT
)

# Excluded TCL Code 



# ----------------------------------------------
# CUDA Conditional Compilation
# -----------------------------------------------

IF (BIOIMAGESUITE3_USE_CUDA)

  SET (KITSRCS
   ${KITSRCS}
   vtkbisCUDAFloatVector.cpp
   vtkbisCUDAOptimizer.cpp
   vtkbisCUDATestOptimizer.cpp
   vtkbisCUDAJointHistogram.cpp
   vtkbisCUDAApproximateGridTransform.cpp
   vtkbisCUDALinearRegistration.cpp
   vtkbisCUDANonLinearRegistration.cpp
   vtkbisCUDARegistrationUtil.cpp)
 
  SET(CUDAKIT vtkpx${KITBASE}_Cuda)

  CUDA_INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    )
  
  
  CUDA_ADD_LIBRARY(${CUDAKIT} STATIC 
    koch.cu
    warmup.cu   
    linearreslice.cu 
    biscudabendingenergy.cu
    biscudaregistrationutil.cu
    biscudajointhistogram.cu
    biscudavectorutil.cu
    )
  TARGET_LINK_LIBRARIES (${CUDAKIT} ${KITLIBS})
  install (TARGETS ${CUDAKIT} DESTINATION lib)

  SET_SOURCE_FILES_PROPERTIES(
    #ssd.cu  
    koch.cu
    warmup.cu   
 # demonregistration.cu
    linearreslice.cu 
bisCUDA_BendingEnergy.cu 
    WRAP_EXCLUDE
    )

  INCLUDE_DIRECTORIES(${CUDA_INSTALL_PREFIX}/../include)
  INCLUDE_DIRECTORIES(${FOUND_CUDA_NVCC_INCLUDE})
  
ELSE (BIOIMAGESUITE3_USE_CUDA)
  SET (CUDAKIT )
ENDIF (BIOIMAGESUITE3_USE_CUDA)
# -----------------------------------------------


SET(KITLIBS
  numerics
  vtkpxCommon
  ${CUDAKIT}
  vtkpxImaging
  vtkpxLegacy
  vtkpxCommonTCL
  vtkCommonTCL
  ${BIOIMAGESUITE3_COMMON_LIBS}
)

SET(KITJAVALIBS
  numerics
  vtkpxCommon
  ${CUDAKIT}
  vtkpxImaging
  vtkpxLegacy
  vtkpxCommonJava
   ${BIOIMAGESUITE3_COMMON_JAVALIBS}
)


# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line


IF (BIOIMAGESUITE3_USE_CUDA)
SET (KITEXTRAFILES
Registration/linearreslice.cu 
Registration/warmup.cu   			      
Registration/dvSparseMatrix.h
Registration/koch.cu
Registration/biscudaregistrationutil.cu
Registration/biscudaregistrationutil.h
Registration/biscudajointhistogram.cu
Registration/biscudajointhistogram.h
Registration/biscudabendingenergy.cu
Registration/biscudavectorutil.cu
)
ENDIF (BIOIMAGESUITE3_USE_CUDA)

SET (KITTCLSRCS )
bis_complex("Registration" ${KIT} ${FILELIST})




