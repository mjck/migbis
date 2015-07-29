# Install script for directory: /Users/mjack/src3/bioimagesuite30_src

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/local")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/lib" TYPE FILE FILES "/Users/mjack/src3/bioimagesuite30_src/scriptfiles.txt")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/lib" TYPE FILE FILES "/Users/mjack/src3/bioimagesuite30_src/tclfilelist.tcl")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30" TYPE FILE FILES "/Users/mjack/src3/bioimagesuite30_src/BioImageSuite_License.txt")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/include/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/numerics/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/pxtklib/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/Common/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/database/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/Graphics/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/Legacy/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/Imaging/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/Registration/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/Segmentation/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/GUI/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/mpjCommon/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/mpjGraphics/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/mpjLevelset/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/mpjDiffusion/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/Connectivity/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/igtlutil/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/installtools/cmake_install.cmake")
  INCLUDE("/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "/Users/mjack/src3/bioimagesuite30_src/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "/Users/mjack/src3/bioimagesuite30_src/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
