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

#
# Build the documentation
#
INCLUDE (${CMAKE_ROOT}/Modules/Documentation.cmake OPTIONAL)

IF (BUILD_DOCUMENTATION)

  #
  # Configure the script and the doxyfile, then add target
  #

  CONFIGURE_FILE(
    ${BIOIMAGESUITE3_SOURCE_DIR}/Utilities/Doxygen/doc_makeall.in
    ${BIOIMAGESUITE3_SOURCE_DIR}/Utilities/Doxygen/doc_makeall.sh)

  CONFIGURE_FILE(
    ${BIOIMAGESUITE3_SOURCE_DIR}/Utilities/Doxygen/doxyfile.in
    ${BIOIMAGESUITE3_SOURCE_DIR}/Utilities/Doxygen/doxyfile)

  ADD_CUSTOM_TARGET(DoxygenDoc
    ${BASH}
    ${BIOIMAGESUITE3_SOURCE_DIR}/Utilities/Doxygen/doc_makeall.sh
    )

ENDIF (BUILD_DOCUMENTATION)

#INCLUDE (${BIOIMAGESUITE3_SOURCE_DIR}/Utilities/Doxygen/LocalUserOptions.cmake OPTIONAL)

#INSTALL_FILES(/lib/vtk/doxygen "\\.css$")
#INSTALL_FILES(/lib/vtk/doxygen "\\.gif$")
#INSTALL_FILES(/lib/vtk/doxygen "\\.html$")
#INSTALL_FILES(/lib/vtk/doxygen "\\.pl$")
#INSTALL_FILES(/lib/vtk/doxygen "\\.stop$")
#INSTALL_FILES(/lib/vtk/doxygen .txt authors doc_readme)

