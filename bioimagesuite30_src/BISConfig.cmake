#-----------------------------------------------------------------------------
#
# BISConfig.cmake - BIS CMake configuration file for external projects.
#
# This file is configured by BIS and used by the UseBIS.cmake module
# to load VTK's settings for an external project.

# Compute the installation prefix from BIS_DIR.
SET(BIS_INSTALL_PREFIX "${BIS_DIR}")
GET_FILENAME_COMPONENT(BIS_INSTALL_PREFIX "${BIS_INSTALL_PREFIX}" PATH)

# The set of BIS libraries
SET(BIS_LIBRARIES "igtlutil;numerics;pxtklib;vtkbisConnectivity;vtkbisConnectivityTCL;vtkbisDatabase;vtkbisDatabaseTCL;vtkmpjCommon;vtkmpjCommonTCL;vtkmpjDiffusion;vtkmpjDiffusionTCL;vtkmpjGraphics;vtkmpjGraphicsTCL;vtkmpjGraphics;vtkmpjGraphicsTCL;vtkmpjLevelset;vtkmpjLevelsetTCL;vtkpxCommon;vtkpxCommonTCL;vtkpxGUI;vtkpxGUITCL;vtkpxGraphics;vtkpxGraphicsTCL;vtkpxImaging;vtkpxImagingTCL;vtkpxLegacy;vtkpxLegacyTCL;vtkpxRegistration;vtkpxRegistrationTCL;vtkpxSegmentation;vtkpxSegmentationTCL")

# The BIS include file directories.
SET(BIS_INCLUDE_DIRS "${BIS_INSTALL_PREFIX}/include")

# The BIS library directories.
SET(BIS_LIBRARY_DIRS "${BIS_INSTALL_PREFIX}/lib")

# The BIS binary executable directories.  Note that if
# BIS_CONFIGURATION_TYPES is set (see below) then these directories
# will be the parent directories under which there will be a directory
# of runtime binaries for each configuration type.
SET(BIS_EXECUTABLE_DIRS "${BIS_INSTALL_PREFIX}/bin")

# The BIS runtime library directories.  Note that if
# BIS_CONFIGURATION_TYPES is set (see below) then these directories
# will be the parent directories under which there will be a directory
# of runtime libraries for each configuration type.
SET(BIS_RUNTIME_LIBRARY_DIRS "${BIS_INSTALL_PREFIX}/lib")

# The runtime library path variable name e.g. LD_LIBRARY_PATH,
# this environment variable should be set to BIS_RUNTIME_LIBRARY_DIRS
SET(BIS_RUNTIME_PATH_VAR_NAME "DYLD_LIBRARY_PATH")

# The C and C++ flags added by BIS to the cmake-configured flags.
SET(BIS_REQUIRED_C_FLAGS "")
SET(BIS_REQUIRED_CXX_FLAGS "")
SET(BIS_REQUIRED_EXE_LINKER_FLAGS "")
SET(BIS_REQUIRED_SHARED_LINKER_FLAGS "")
SET(BIS_REQUIRED_MODULE_LINKER_FLAGS "")

# The VTK version number
SET(BIS_MAJOR_VERSION "3")
SET(BIS_MINOR_VERSION "0")
SET(BIS_BUILD_VERSION "1")

# The location of the UseBIS.cmake file.
SET(BIS_USE_FILE "${BIS_INSTALL_PREFIX}/lib/UseBIS.cmake")

# Relative install paths in the BIS install tree
SET(BIS_INSTALL_BIN_DIR "/bin")
SET(BIS_INSTALL_INCLUDE_DIR "/include")
SET(BIS_INSTALL_LIB_DIR "/lib")
SET(BIS_INSTALL_PACKAGE_DIR "/lib")

# The old, less clear name for BIS_RUNTIME_LIBRARY_DIRS.  Kept here
# for compatibility.
SET(BIS_RUNTIME_DIRS ${BIS_RUNTIME_LIBRARY_DIRS})
