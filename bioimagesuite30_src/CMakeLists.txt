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
#  Master File for BioImage Suite build
#
#
# Set the project name.
#
cmake_minimum_required(VERSION 2.6)
if( COMMAND cmake_policy )
  cmake_policy( VERSION 2.6 )
endif( COMMAND cmake_policy )

PROJECT (BIOIMAGESUITE3)

SET (BIOIMAGESUITE3_VERSION_MAJOR 3)
SET (BIOIMAGESUITE3_VERSION_MINOR 0)
SET (BIOIMAGESUITE3_VERSION_PATCH 1)
SET (BIOIMAGESUITE3_STRING_VERSION  "${BIOIMAGESUITE_VERSION_MAJOR}${BIOIMAGESUITE_VERSION_MINOR}${BIOIMAGESUITE_VERSION_PATCH}")
#SET (BIOIMAGESUITE3_JAVASOURCES CACHE STRING "")





# Compiler Specific Things
# -------------------------------------------------------------------------------------
IF(CMAKE_COMPILER_2005)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE)
#  SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /NODEFAULTLIB:msvcrt.lib ")
ENDIF(CMAKE_COMPILER_2005)

IF (UNIX)
IF( CMAKE_COMPILER_IS_GNUCXX )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC" )
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC" )
ENDIF( CMAKE_COMPILER_IS_GNUCXX )
ENDIF (UNIX)
# ------------------------------------------------------------------------------------


# Excluded TCL Code 

INCLUDE (${CMAKE_ROOT}/Modules/FindVTK.cmake)
INCLUDE(${VTK_CMAKE_DIR}/vtkWrapTcl.cmake)

IF (BIOIMAGESUITE3_USE_PYTHON)
  INCLUDE(${VTK_CMAKE_DIR}/vtkWrapPython.cmake)
ENDIF (BIOIMAGESUITE3_USE_PYTHON)
# Excluded TCL Code 

#
# Assume everything is set up correctly for build.
#
# If some requirements are not met, set it to 0.
# In that case, the build won't be done.
#

SET (BIOIMAGESUITE3_CAN_BUILD 1)

#
# Load CMake commands that you probably should not modify.
#
INCLUDE (${BIOIMAGESUITE3_SOURCE_DIR}/CMakeOptions.cmake)
INCLUDE (${BIOIMAGESUITE3_SOURCE_DIR}/BioImageSuiteMacros.cmake)
#
# If everything is OK, then go into the sub directories and build.
#

IF (BIOIMAGESUITE3_CAN_BUILD)
  #
  # Here is where you can list the sub-directories holding your local 
  # classes. Sorting classes by 'package' type like VTK does (Common, 
  # Rendering, Filtering, Imaging, IO, etc.) is a good thing and prevents 
  # numerous dependencies problems.
  #

  INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/include)
  INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_BINARY_DIR})
  INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/numerics)
  INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Common)

  SUBDIRS (
    include
    numerics
    pxtklib
    ${VVLINK_API}
    Common
    database
    Graphics
    Legacy
    Imaging
    Registration
    Segmentation
    GUI	   
    mpjCommon
    mpjGraphics
    mpjLevelset
    mpjDiffusion
    ${BIOIMAGESUITE3_APPLICATIONS}
    ${STEREOVISION_SOURCE}
    ${DBFEM_SOURCE} 
    Connectivity
#    ShapeModels
# Excluded TCL Code 
)

if (BIOIMAGESUITE3_USE_SYSTEM_NIFTI)
  include_directories( /usr/include/nifti/ )
endif (BIOIMAGESUITE3_USE_SYSTEM_NIFTI)

if (BIOIMAGESUITE3_USE_SYSTEM_OPENIGTLINK)
     include_directories( /usr/include/openigtlink/ )
     include_directories( /usr/include/openigtlink/igtlutil )
else (BIOIMAGESUITE3_USE_SYSTEM_OPENIGTLINK)
   subdirs (
      igtlutil
   )
endif (BIOIMAGESUITE3_USE_SYSTEM_OPENIGTLINK)

IF(BIOIMAGESUITE3_MOUSEREG)
add_subdirectory(DemonsRegistration)
ENDIF(BIOIMAGESUITE3_MOUSEREG)

add_subdirectory(installtools)
# Excluded TCL Code 
add_subdirectory(bioimagesuite)

IF (BIOIMAGESUITE3_USE_JAVA)
  add_subdirectory(java)
ENDIF(BIOIMAGESUITE3_USE_JAVA)


CONFIGURE_FILE(
    ${BIOIMAGESUITE3_SOURCE_DIR}/include/vtkbisConfigure.h.in 
    ${BIOIMAGESUITE3_BINARY_DIR}/bioimagesuite_settings.h
  )

  # -------------------------------------------------------------------------------------------------
  # Make Install Stuff
# -------------------------------------------------------------------------------------------------

set(KIT "root")
set(KITTCLSRCS)
SET (KITSRCS 
  CMakeLists.txt
  BioImageSuite_License.txt
  BioImageSuite_ShortLicense.txt
  CMakeOptions.cmake
  CPackOptions.cmake
  BioImageSuiteMacros.cmake
  CompilingBioImageSuite.html
  BioImageSuite_Description.txt
  CTestCustom.cmake.in
)

install(FILES BioImageSuite_License.txt DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH})


bis_simplesourcelist("root" ${FILELIST})



# Cpack stuff
IF (BIOIMAGESUITE3_CREATE_INSTALLER)
  INCLUDE (${BIOIMAGESUITE3_SOURCE_DIR}/CPackOptions.cmake)
ENDIF (BIOIMAGESUITE3_CREATE_INSTALLER)


ENDIF (BIOIMAGESUITE3_CAN_BUILD)



