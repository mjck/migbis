# Install script for directory: /Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/base" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pkgIndex.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/feedbackplus.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/labelframe.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxtclreadsprheader.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxtclruncommand.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxtcluserprefs.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxtclutil.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxtclhelp.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxvtable.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/vtkImageInclude.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/vtkpxcontrib.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/vtknrcontrib.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxitclcamera.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxitclelectrodemultigrid.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxitclimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxitclmesh.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxitcllandmarks.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxitclobject.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxitclsurface.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxitcltransform.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/biscmdline.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/VisTree3.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/base/pxtclreadvarianheader.tcl"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

