# Install script for directory: /Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/main" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pkgIndex.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclHistogramWidget.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclLandmarkControl.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclbaseimagecontrol.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclbrainstrip.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclatlasgadget.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclyaleatlasgadget.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclwfuatlasgadget.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclanimationgadget.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitcldistributiontablegadget.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclcreatesolidcontrol.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclimagenormalization.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclsplineobjectmap.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclsubjectentry.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclsurfaceobjectmap.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclElectrodeMultiControl.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclPolyDataControl.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclarticulatedmodel.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclbase4dimageviewer.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclbasecontrol.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclbaseimageviewer.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitcloverlaytool.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclfilelistmenu.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclcsigadget.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclexpfit.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclimageimport.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitcllistselector.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclmanualsurfacetransformation.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclmousechopper.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclrpm.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclsplinestackobjectmap.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclvvlinkgadget.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclopenigtlinkgadget.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclmanualtransformation.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/alarkutility.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclvvlinkprojectorgadget.tcl"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/main" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitcl4dsurfaceeditor.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclbaresimpleviewer.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclbrainregister.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclbrainsegment.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitcldualmultisubject.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclelectrodeeditor.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclmouseregister.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclmousesegment.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclmultisubjectaverage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclsimpleeditor.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclsimpleviewer.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclsplineeditor.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclsurfaceeditor.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclvolumeviewer.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclorthoviewer.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/bis.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/bis_fmrisetup.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/bis_fmri.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/bis_databasetool.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/alarkviewer.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/bis_parameterGUI.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/main/pxitclvvlinkviewer.tcl"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

