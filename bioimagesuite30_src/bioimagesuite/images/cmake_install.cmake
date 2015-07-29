# Install script for directory: /Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/images" TYPE FILE PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE FILES
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/bioimagesuite.ico"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/bioimagesuite.png"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/bioimagesuite.xbm"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/bioimagesuite_setuplogo.bmp"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/bioimagesuite_logo_small_www.gif"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/bioimagesuite_logo_www.gif"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/colin_talairach_lookup.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/colinlookup.hdr"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/colinlookup.img.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/MNI_T1_1mm.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/MNI_T1_1mm_mask.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/MNI_T1_1mm_stripped.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/MNI_T1_1mm_graywhite.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/MNI_T1_2mm.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/MNI_T1_2mm_mask.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/MNI_T1_2mm_stripped.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/MNI_T1_2mm_graywhite.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/avg152T1_LR_nifti.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/yale_broadmann.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/yale_broadmann.txt"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/MNI_T1_1mm_graywhite.txt"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/bioimagesuite_atlas.txt"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/README.txt"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/ISASHN_Standard_Deviation.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/ISAS_SPECT_Template.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/ISAS_Standard_Deviation.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/ISAS_Mean_Diff_SPECT.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/ISAS_SPECT_Mask.nii.gz"
    "/Users/mjack/src3/bioimagesuite30_src/bioimagesuite/images/sampledatabase.sqlite"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

