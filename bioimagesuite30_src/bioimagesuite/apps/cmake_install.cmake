# Install script for directory: /Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/apps" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pkgIndex.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxappscommon.tcl"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/apps" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxtonifti.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxroitoroi.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_headerinfo.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_surfaceinfo.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_flip.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_reorientimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_smoothimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_thresholdimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_resampleimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_create4dimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_split4dimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_resliceimage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_reorient_tensor.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_inverttransform.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_displacementfield.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmultijacobian.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxsinglejacobian.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxcomputeaverage.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmultiregister_int.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxpairwiseregister.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_register.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_distortioncorrection.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_pointregister.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_integratedregistration.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_computedistance.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_computeoverlap.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_computesimilarity.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_computestrains.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_computeaveragebrain.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmultisegment.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_fastpostprocess.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_segment.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_biasfield.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/pxmat_reslicect.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/bisserver.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/bisclient.tcl"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/apps/biscomputecorrelations.tcl"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

