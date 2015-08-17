#
# This module is provided as BIS_USE_FILE by BISConfig.cmake.
# It can be INCLUDEd in a project to load the needed compiler and linker
# settings to use BIS:
#   FIND_PACKAGE(BIS REQUIRED)
#   INCLUDE(${BIS_USE_FILE})

IF(NOT BIS_USE_FILE_INCLUDED)
  SET(BIS_USE_FILE_INCLUDED 1)

  # Add include directories needed to use BIS.
  INCLUDE_DIRECTORIES(${BIS_INCLUDE_DIRS})

  # Add link directories needed to use BIS.
  LINK_DIRECTORIES(${BIS_LIBRARY_DIRS})

  # Add cmake module path.
  SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${BIS_CMAKE_DIR}")

ENDIF(NOT BIS_USE_FILE_INCLUDED)
