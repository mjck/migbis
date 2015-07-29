#BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
#BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
#BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
#BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
#BIOIMAGESUITE_LICENSE  Medicine, http:#www.bioimagesuite.org.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
#BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
#BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
#BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
#BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
#BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
#BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
#BIOIMAGESUITE_LICENSE  See also  http:#www.gnu.org/licenses/gpl.html
#BIOIMAGESUITE_LICENSE  
#BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
#BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
#BIOIMAGESUITE_LICENSE 
#BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


# --------------------------------------------------------
#
# Various tools to make install and package tasks possible
#
# --------------------------------------------------------

SET(KIT installtools )
SET(KITLIBS )

SET (KITSRCS 
  #bis_createpipefiles.cpp
  bis_fixscripts_base.cpp
  bis_fixscripts.cpp
)

# Excluded TCL Code 

IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
SET (KITSRCS
${KITSRCS}
bis_creategnomeentries.cpp
)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")


SET (KITTCLSRCS )

SET (KITEXTRASFILES
  installtools/CPack.STGZ_Header.sh.in
  installtools/NSIS.template.in
  installtools/bisvtk.tcl
  installtools/bis_fix.tcl
)


# Create binaries/targets etc
bis_executable_list(installtools)
bis_sourcelist("installtools" ${KIT} ${FILELIST})


# -------------------------------------------------------------------
#
# On windows we also need this little thing to fix registry paths

IF (BIS_UNIXBUILD)

ELSE (BIS_UNIXBUILD)

#MESSAGE(INFO,"Looking to build bisvtk")

FIND_PROGRAM(FREEWRAP
    NAMES 
    freewrapTCLSH.exe	
    freewrapTCLSH
    )			

ADD_CUSTOM_COMMAND(
OUTPUT ${EXECUTABLE_OUTPUT_PATH}/bisvtk.exe
DEPENDS ${BIOIMAGESUITE3_SOURCE_DIR}/installtools/bisvtk.tcl
COMMENT "Creating bisvtk.exe using ${FREEWRAP}"
COMMAND ${FREEWRAP} 
ARGS ${BIOIMAGESUITE3_SOURCE_DIR}/installtools/bisvtk.tcl -o ${EXECUTABLE_OUTPUT_PATH}/bisvtk.exe)

add_custom_target(bisvtk
DEPENDS ${EXECUTABLE_OUTPUT_PATH}/bisvtk.exe)

install (PROGRAMS ${EXECUTABLE_OUTPUT_PATH}/bisvtk.exe DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/bin
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE )
ENDIF (BIS_UNIXBUILD)

#
# -------------------------------------------------------------------

