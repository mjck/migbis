# Install script for directory: /Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/mjack" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/pkgIndex.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjfastmarching.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjfibertracking.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjguiicosahedron.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjguilookuptable.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjguiobject.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjguisphere.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjmultiimagegui.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjmultiobjectgui.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjtensoranalysis.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjtensorutility.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjvesselanalysis.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjvesseltracking.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjvesselutility.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjmultibtensorutility.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/vtkmpjcontrib.tcl"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/mjack" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjdiffusiontool.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjinterleave.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjsigna2analyze.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjt2relax.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/mjack/mpjvesseltool.tcl"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

