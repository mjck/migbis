# Install script for directory: /Users/mjack/src3/bioimagesuite30_src/GUI

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/lib" TYPE STATIC_LIBRARY FILES "/Users/mjack/src3/bioimagesuite30_src/lib/libvtkpxGUI.a")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bioimagesuite30/lib/libvtkpxGUI.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bioimagesuite30/lib/libvtkpxGUI.a")
    EXECUTE_PROCESS(COMMAND "/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bioimagesuite30/lib/libvtkpxGUI.a")
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bioimagesuite30/lib" TYPE SHARED_LIBRARY FILES "/Users/mjack/src3/bioimagesuite30_src/lib/libvtkpxGUITCL.dylib")
  IF(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bioimagesuite30/lib/libvtkpxGUITCL.dylib" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bioimagesuite30/lib/libvtkpxGUITCL.dylib")
    EXECUTE_PROCESS(COMMAND "/usr/bin/install_name_tool"
      -id "libvtkpxGUITCL.dylib"
      -change "/Users/mjack/src3/bioimagesuite30_src/lib/libvtkmpjGraphicsTCL.dylib" "libvtkmpjGraphicsTCL.dylib"
      -change "/Users/mjack/src3/bioimagesuite30_src/lib/libvtkpxCommonTCL.dylib" "libvtkpxCommonTCL.dylib"
      -change "/Users/mjack/src3/bioimagesuite30_src/lib/libvtkpxGraphicsTCL.dylib" "libvtkpxGraphicsTCL.dylib"
      -change "/Users/mjack/src3/bioimagesuite30_src/lib/libvtkpxLegacyTCL.dylib" "libvtkpxLegacyTCL.dylib"
      -change "/Users/mjack/src3/bioimagesuite30_src/lib/libvtkpxRegistrationTCL.dylib" "libvtkpxRegistrationTCL.dylib"
      "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bioimagesuite30/lib/libvtkpxGUITCL.dylib")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bioimagesuite30/lib/libvtkpxGUITCL.dylib")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
