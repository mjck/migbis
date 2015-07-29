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

INCLUDE_DIRECTORIES(${ITK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/Common)
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/numerics)
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/igtlutil)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Registration)
INCLUDE_DIRECTORIES (${BIOIMAGESUITE3_SOURCE_DIR}/Imaging)


SET (KITSRCS 
bisVersionTest.cpp
)

SET (KITSRCS
${KITSRCS}
Test_vtkpxMatrix.cpp
Test_vnlmatrix.cpp
Test_vtkpxOptimizer.cpp
)

IF (BIOIMAGESUITE3_USE_CUDA)
SET (KITSRCS
${KITSRCS}
cudainfo.cpp
conjGrad.cpp
Test_vtkbisCUDAOptimizer.cpp
Test_vtkbisCUDASparseMatrix.cpp
)
ENDIF (BIOIMAGESUITE3_USE_CUDA)




SET(KIT Testing)


SET(KITLIBS
  vtkpxCommon
  vtkpxImaging
  vtkpxRegistration
  ${BIOIMAGESUITE3_COMMON_LIBS}
  numerics
)

IF (BIOIMAGESUITE3_USE_CUDA)
SET(KITLIBS
  vtkpxRegistration_Cuda
  vtkpxImaging_Cuda
  ${KITLIBS})
ENDIF (BIOIMAGESUITE3_USE_CUDA)

IF (APPLE)
SET(KITLIBS
${KITLIBS}
SystemStubs
)
ENDIF (APPLE)


SET (KITEXTRAFILES
Testing/test.h
Testing/bisruntests.bat.in
Testing/bisruntests.sh.in
)

# Macro 
bis_executable_list(installtools)

FOREACH(item ${KITSRCS})
  GET_FILENAME_COMPONENT(TAIL ${item} NAME_WE )        
  IF (UNIX)
    ADD_TEST(C-${TAIL} ${BIOIMAGESUITE3_BINARY_DIR}/biscppexec ${EXECUTABLE_OUTPUT_PATH}/${TAIL})
  ELSE (UNIX)
    ADD_TEST(C-${TAIL} ${BIOIMAGESUITE3_BINARY_DIR}/biscppexec.bat ${EXECUTABLE_OUTPUT_PATH}/${TAIL})
  ENDIF (UNIX)
ENDFOREACH(item $KITSRCS)


SET (DOSH 0)
IF (UNIX)
  SET (DOSH 1)
ELSEIF (BIOIMAGESUITE_USING_MINGW)
  SET (DOSH 1)
ENDIF (UNIX)
      
#message("Creating unix style .sh build = ${DOSH}")


IF (WIN32)
  STRING(REGEX REPLACE "/" "\\\\"  CMAKE_CTEST_COMMAND_BAT ${CMAKE_CTEST_COMMAND})
  STRING(REGEX REPLACE "/" "\\\\"  SVNCOMMAND_BAT ${SVNCOMMAND})
  configure_file( ${BIOIMAGESUITE3_SOURCE_DIR}/Testing/bisruntests.bat.in
    ${BIOIMAGESUITE3_BINARY_DIR}/bisruntests.bat @ONLY )
   install(FILES bisruntests.bat.in DESTINATION  ${BIOIMAGESUITE_INSTALL_EXTRAPATH}/testing)
ENDIF(WIN32)

IF (DOSH)
  configure_file( ${BIOIMAGESUITE3_SOURCE_DIR}/Testing/bisruntests.sh.in
    ${BIOIMAGESUITE3_BINARY_DIR}/bisruntests.sh @ONLY )
   EXECUTE_PROCESS (COMMAND chmod +x  ${BIOIMAGESUITE3_BINARY_DIR}/bisruntests.sh)
   install(FILES bisruntests.sh.in DESTINATION  ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/testing)
ENDIF (DOSH)

bis_sourcelist("Testing" ${KIT} ${FILELIST})

