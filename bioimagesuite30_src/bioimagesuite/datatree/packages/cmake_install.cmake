# Install script for directory: /Users/mjack/src3/bioimagesuite30_src/bioimagesuite/datatree/packages

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/datatree/packages" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/datatree/packages/ElectrodeAttrProg.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/datatree/packages/ImageDialog.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/datatree/packages/VisTree2.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/datatree/packages/chellyDB.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/datatree/packages/mdImageUtils.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/datatree/packages/metakitDB.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/datatree/packages/pkgIndex.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/datatree/packages/pxitclchellypackages.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/datatree/packages/pxitcloverlaytab.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/datatree/packages/SPECTtools.tcl"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

