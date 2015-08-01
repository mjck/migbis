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


# --------------------------------------------------------------------------------------------------
IF (BIOIMAGESUITE3_WRAP_TCL)
  INCLUDE(${VTK_CMAKE_DIR}/vtkWrapTcl.cmake)
ENDIF (BIOIMAGESUITE3_WRAP_TCL)



IF (BIOIMAGESUITE3_USE_JAVA)
    INCLUDE (${CMAKE_ROOT}/Modules/FindJava.cmake)
  INCLUDE(${VTK_CMAKE_DIR}/vtkWrapJava.cmake)
  SET(VTK_WRAP_JAVA3_INIT_DIR "${BIOIMAGESUITE3_SOURCE_DIR}/include")
  SET(VTK_JAVA_HOME ${BIOIMAGESUITE3_BINARY_DIR}/java/vtk)
  MAKE_DIRECTORY(${VTK_JAVA_HOME})
  SET (BISJAVALIST ${BIOIMAGESUITE3_BINARY_DIR}/java/bioimagesuite_javafiles.txt)
  FILE(WRITE ${BISJAVALIST} "")

  SET (BISJAVACLASSLIST ${BIOIMAGESUITE3_BINARY_DIR}/java/bioimagesuite_javaclassfiles.txt)
  FILE(WRITE ${BISJAVACLASSLIST} "")

  SET (BIOIMAGESUITE3_ECLIPSE_BISVTK_PATH
    ${BIOIMAGESUITE3_BINARY_DIR}/bin
    CACHE PATH
    "If set then bisvtk.jar will also be copied to this location for the use of Eclipse/Java projects")
  MARK_AS_ADVANCED(BIOIMAGESUITE_INSTALL_SRCPATH)


  

   get_filename_component(BIS_JAVA_INCL ${JAVA_COMPILE} PATH)
#   INCLUDE_DIRECTORIES(${BIS_JAVA_INCL}/../include)
#   IF(WIN32)
#      INCLUDE_DIRECTORIES(${BIS_JAVA_INCL}/../include/win32)
#      MESSAGE("Info on Win32")
#   ENDIF(WIN32)
  MESSAGE("Info using JAVA -- ${VTK_JAVA_HOME} -- ${BISJAVALIST} -- ${BISJAVACLASSLIST} -- ${BIS_JAVA_INCL}/../include")


ENDIF (BIOIMAGESUITE3_USE_JAVA)

macro(bis_static LIBNAME)
  #  message ("Building static library= ${LIBNAME}")
  ADD_LIBRARY (${LIBNAME} STATIC ${KITSRCS})
  TARGET_LINK_LIBRARIES (${LIBNAME} ${KITLIBS})
  install (TARGETS ${LIBNAME} DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/lib)
endmacro(bis_static)

macro(bis_dynamic LIBNAME)
  ADD_LIBRARY (${LIBNAME}  ${KITSRCS})
  TARGET_LINK_LIBRARIES (${LIBNAME} ${KITLIBS})
  IF (BIOIMAGESUITE3_DEBIAN)
    SET_TARGET_PROPERTIES(${LIBNAME}TCL PROPERTIES
      SOVERSION ${BIOIMAGESUITE3_VERSION_MAJOR}.${BIOIMAGESUITE3_VERSION_MINOR}
      VERSION ${BIOIMAGESUITE3_VERSION_MAJOR}.${BIOIMAGESUITE3_VERSION_MINOR}.${BIOIMAGESUITE3_VERSION_PATCH}
    )
  ENDIF(BIOIMAGESUITE3_DEBIAN)
  install (TARGETS ${LIBNAME} DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/lib)
endmacro(bis_dynamic)

# --------------------------------------------------------------------------------------------------

macro(bis_executable NAME OUTDIR)
#  MESSAGE(Info "executable = ${NAME}, OUTDIR=${OUTDIR}")
  ADD_EXECUTABLE(${NAME} ${NAME}.cpp)
  TARGET_LINK_LIBRARIES (${NAME} ${KITLIBS})

  if (MSVC)
    get_target_property(FVAR ${NAME} LOCATION)
    ADD_CUSTOM_COMMAND(
      TARGET ${NAME}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
      ARGS ${FVAR} ${EXECUTABLE_OUTPUT_PATH})
  endif (MSVC)

  install (TARGETS ${NAME} DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/${OUTDIR})
endmacro(bis_executable)

# --------------------------------------------------------------------------------------------------
macro(bis_executable_list OUTPUTDIR)
  FOREACH (item ${KITSRCS})
    STRING(REGEX REPLACE ".cpp$" "" OUTNAME ${item})
    bis_executable(${OUTNAME} ${OUTPUTDIR})
  ENDFOREACH (item ${KITSRCS})
endmacro(bis_executable_list)

# --------------------------------------------------------------------------------------------------
macro(bis_wraplibraries LIBNAME)
  #  message ("Building tcl library: LIBNAME= ${LIBNAME}")
  IF (VTK_WRAP_TCL)
    VTK_WRAP_TCL2 (${LIBNAME}TCL TCL_SRCS ${KITSRCS})
    ADD_LIBRARY (${LIBNAME}TCL ${TCL_SRCS} ${KITTCLSRCS})
    TARGET_LINK_LIBRARIES (${LIBNAME}TCL ${LIBNAME} ${KITLIBS})
  
    IF (BIOIMAGESUITE3_DEBIAN)
      SET_TARGET_PROPERTIES(${LIBNAME}TCL PROPERTIES
	SOVERSION ${BIOIMAGESUITE3_VERSION_MAJOR}.${BIOIMAGESUITE3_VERSION_MINOR}
	VERSION ${BIOIMAGESUITE3_VERSION_MAJOR}.${BIOIMAGESUITE3_VERSION_MINOR}.${BIOIMAGESUITE3_VERSION_PATCH}
	)
    ENDIF(BIOIMAGESUITE3_DEBIAN)
    install (TARGETS ${LIBNAME}TCL DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/lib)
    
    #  get_target_property(FVAR ${LIBNAME}TCL LOCATION)
    
    #  if (MSVC)
    #    get_target_property(FVAR ${LIBNAME}TCL LOCATION)
    #    MESSAGE("FVAR=${FVAR}")
    #    ADD_CUSTOM_COMMAND(
    #      TARGET ${LIBNAME}TCL
    #      POST_BUILD
    #      COMMAND ${CMAKE_COMMAND} -E copy
    #      ARGS ${FVAR} ${LIBRARY_OUTPUT_PATH})
    #  endif (MSVC)
  ENDIF (VTK_WRAP_TCL)

  IF (NOT KITBASE MATCHES GUI)
    IF (BIOIMAGESUITE3_USE_JAVA)
      VTK_WRAP_JAVA3 (${LIBNAME}Java JAVA_SRCS "${KITSRCS}")
      ADD_LIBRARY (${LIBNAME}Java ${JAVA_SRCS} ${KIIJAVASRCS})
      TARGET_LINK_LIBRARIES (${LIBNAME}Java ${LIBNAME} ${KITJAVALIBS})
      
      IF (BIOIMAGESUITE3_DEBIAN)
	SET_TARGET_PROPERTIES(${LIBNAME}Java PROPERTIES
	  SOVERSION ${BIOIMAGESUITE3_VERSION_MAJOR}.${BIOIMAGESUITE3_VERSION_MINOR}
	  VERSION ${BIOIMAGESUITE3_VERSION_MAJOR}.${BIOIMAGESUITE3_VERSION_MINOR}.${BIOIMAGESUITE3_VERSION_PATCH}
	  )
      ENDIF(BIOIMAGESUITE3_DEBIAN)
      install (TARGETS ${LIBNAME}Java DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/lib)

      if (MSVC)
	get_target_property(FVAR ${LIBNAME}Java LOCATION)
#	MESSAGE("FVAR2=${FVAR}")
	ADD_CUSTOM_COMMAND(
	  TARGET ${LIBNAME}Java
	  POST_BUILD
	  COMMAND ${CMAKE_COMMAND} -E copy
	  ARGS ${FVAR} ${LIBRARY_OUTPUT_PATH})
      endif (MSVC)



      
      FOREACH (jitem ${JAVA_SRCS})
	GET_FILENAME_COMPONENT(jitem2 ${jitem} NAME_WE)
	GET_FILENAME_COMPONENT(extitem ${jitem} EXT)

	STRING(REGEX REPLACE "Java$" "" jitem2 ${jitem2})

#	IF (UNIX)
	  FILE (APPEND ${BISJAVALIST} "vtk/${jitem2}.java\n")
	  FILE (APPEND ${BISJAVACLASSLIST} "vtk/${jitem2}.class\n")
	  FILE (APPEND ${BISJAVACLASSLIST} "vtk/${jitem2}.java\n")
#	ELSE(UNIX)
#	  FILE (APPEND ${BISJAVALIST} "${VTK_JAVA_HOME}\\${jitem2}.java\n")
#	  FILE (APPEND ${BISJAVACLASSLIST} "vtk\\${jitem2}.class\n")
#	ENDIF(UNIX)
      ENDFOREACH (jitem ${KITSRCS})
    ENDIF (BIOIMAGESUITE3_USE_JAVA)
  ENDIF (NOT KITBASE MATCHES GUI)
endmacro(bis_wraplibraries)

# --------------------------------------------------------------------------------------------------

macro(bis_libraries LIBNAME)
  bis_static(${LIBNAME} ${KITSRCS} ${KITLIBS})
  bis_wraplibraries(${LIBNAME} ${KITSRCS} ${KITTCLSRCS} ${KITLIBS})
endmacro(bis_libraries)

# --------------------------------------------------------------------------------------------------

macro(bis_sourcelist_full PATHNAME LIBNAME FILEHANDLE EXTRAPATH)
  
  #  message("sourcelist, path=${PATHNAME}, lib=${LIBNAME}")

  FILE(APPEND ${FILEHANDLE} "#-------------------------\n# ** ${LIBNAME} \n#-----------------------------\n")
  
  SET (HEADERLIST )

  SET (COMPLIST 
    ${KITEXTRAFILES}
    ${PATHNAME}/CMakeLists.txt
    )
  
  
  SET (TMPLIST 
    ${KITSRCS} 
    ${KITTCLSRCS}
    )

  #message("sourcelist\n\ncomplist=${COMPLIST}\n\ntmplist=${TMPLIST}")

  FOREACH (item ${TMPLIST})
    SET(COMPLIST 
      ${COMPLIST}
      ${PATHNAME}/${item}
      )
    
    SET (HEADER 0)
    IF (${item} MATCHES "txx$")
      STRING(REGEX REPLACE "txx$" "h" HEADER ${item})
    ELSE(${item} MATCHES "txx$")
      IF (${item} MATCHES "cxx$")
	STRING(REGEX REPLACE "cxx$" "h" HEADER ${item})
      ELSE(${item} MATCHES "cxx$")
	IF (${item} MATCHES "cpp$")
	  STRING(REGEX REPLACE "cpp$" "h" HEADER ${item})
	ELSE(${item} MATCHES "cpp$")
	  IF (${item} MATCHES "c$")
	    STRING(REGEX REPLACE "c$" "h" HEADER ${item})
	  ENDIF(${item} MATCHES "c$")
	ENDIF(${item} MATCHES "cpp$")
      ENDIF(${item} MATCHES "cxx$")
    ENDIF(${item} MATCHES "txx$")
    #message("\n\t\tforeach item=${item}, header=${HEADER}")

    IF (NOT HEADER)
      #      message("**************** foreach item=${item}, header=${HEADER} not found")
    ELSE (NOT HEADER)
      SET(HEADERNAME ${BIOIMAGESUITE3_SOURCE_DIR}/${PATHNAME}/${HEADER})
      #message("**************** header=${HEADERNAME}")
      IF (EXISTS ${HEADERNAME})
      	SET(COMPLIST 
      	  ${COMPLIST}
      	  ${PATHNAME}/${HEADER}
      	  )

        SET(HEADERLIST
          ${HEADERLIST}
          ${HEADERNAME}
          )  
      ELSE (EXISTS ${HEADERNAME})
	
      ENDIF (EXISTS ${HEADERNAME})
    ENDIF (NOT HEADER)
  ENDFOREACH (item ${TMPLIST})



  FOREACH (item ${COMPLIST})
    FILE(APPEND ${FILEHANDLE} "${item}\n")
  ENDFOREACH (item ${COMPLIST})


  SET( COMPLIST2 "")
  FOREACH (item ${COMPLIST})
    set(item2 ${EXTRAPATH}/${item})
    SET(COMPLIST2 
      ${COMPLIST2}
      ${item2})
  ENDFOREACH (item ${COMPLIST})
  
  install (FILES ${HEADERLIST} DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/include)

  IF (BIOIMAGESUITE3_INSTALL_SOURCE)
    FOREACH (item ${COMPLIST2})
      GET_FILENAME_COMPONENT(EPATH ${item} PATH )        
            #message("installing ${item}  in ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/${PATHNAME}/${EPATH}")
      install(FILES ${item}  DESTINATION ${BIOIMAGESUITE3_INSTALL_SRCPATH}/${PATHNAME}/${EPATH})
    ENDFOREACH (item ${COMPLIST2})
  ENDIF (BIOIMAGESUITE3_INSTALL_SOURCE)
endmacro(bis_sourcelist_full)

# --------------------------------------------------------------------------------------------------

macro(bis_sourcelist PATHNAME LIBNAME FILEHANDLE)
  bis_sourcelist_full(${PATHNAME} ${LIBNAME} ${FILEHANDLE} "..")
endmacro(bis_sourcelist)

macro(bis_sourcelist_tcl PATHNAME LIBNAME FILEHANDLE)
  bis_sourcelist_full(${PATHNAME} ${LIBNAME} ${FILEHANDLE} "../..")
endmacro(bis_sourcelist_tcl)
# --------------------------------------------------------------------------------------------------
macro(bis_simplesourcelist LIBNAME FILEHANDLE)

  FILE(APPEND ${FILEHANDLE} "#-------------------------\n# ** ${LIBNAME} \n#-----------------------------\n")
  FOREACH (item ${KITSRCS})
    FILE(APPEND ${FILEHANDLE} "${item}\n")
  ENDFOREACH (item ${COMPLIST})


  IF (BIOIMAGESUITE3_INSTALL_SOURCE)
    install (FILES ${KITSRCS} DESTINATION ${BIOIMAGESUITE3_INSTALL_SRCPATH})
  ENDIF (BIOIMAGESUITE3_INSTALL_SOURCE)
endmacro(bis_simplesourcelist)
# --------------------------------------------------------------------------------------------------
#  Implicitly defined KITSRCS KITLIBS KITTCLSRCS KITEXTRAFILES
# --------------------------------------------------------------------------------------------------
macro(bis_complex PATHNAME LIBNAME  FILEHANDLE)

  IF (BIOIMAGESUITE3_BUILD_SHARED_LIBS)
    bis_dynamic(${LIBNAME})
  ELSE (BIOIMAGESUITE3_BUILD_SHARED_LIBS)
    bis_static(${LIBNAME})
  ENDIF (BIOIMAGESUITE3_BUILD_SHARED_LIBS)

  bis_wraplibraries(${LIBNAME})
  bis_sourcelist(${PATHNAME} ${LIBNAME}  ${FILEHANDLE})
  
endmacro(bis_complex)

# --------------------------------------------------------------------------------------------------
# Macros for the TCL side of things
# --------------------------------------------------------------------------------------------------
#  Implicitly defined KITSCRIPTS KITAPPS  
# --------------------------------------------------------------------------------------------------
macro(bis_complex_tcl3src PATHNAME SHORTPATH  FILEHANDLE)

  IF (BIOIMAGESUITE3_INSTALL_SOURCE)
    install(FILES ${KITSCRIPTS} DESTINATION  ${BIOIMAGESUITE3_INSTALL_SRCPATH}/${PATHNAME})
    install(PROGRAMS ${KITAPPS} DESTINATION  ${BIOIMAGESUITE3_INSTALL_SRCPATH}/${PATHNAME})
    install(FILES CMakeLists.txt DESTINATION ${BIOIMAGESUITE3_INSTALL_SRCPATH}/${PATHNAME})


    SET (TMP  CMakeLists.txt ${KITSCRIPTS} ${KITAPPS} )
    FILE(APPEND ${FILEHANDLE} "#-------------------------\n# ** ${SHORTPATH} \n#-----------------------------\n")
    FOREACH (item ${TMP})
      FILE(APPEND ${FILEHANDLE} "bioimagesuite/${SHORTPATH}/${item}\n")
    ENDFOREACH (item ${SCRIPTLIST})



  ENDIF (BIOIMAGESUITE3_INSTALL_SOURCE)
endmacro(bis_complex_tcl3src PATHNAME SHORTPATH  FILEHANDLE)


macro(bis_complex_tcl3 PATHNAME SHORTPATH  FILEHANDLE FILEHANDLE2)

  GET_FILENAME_COMPONENT(SHORTPATH ${PATHNAME} NAME )        

  install(FILES ${KITSCRIPTS}  DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/${SHORTPATH}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE )
  install(FILES ${KITAPPS}  DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/${SHORTPATH}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE )

  SET (TMP  ${KITSCRIPTS} ${KITAPPS} )
  FILE(APPEND ${FILEHANDLE} "#-------------------------\n# ** ${SHORTPATH} \n#-----------------------------\n")
  FOREACH (item ${TMP})
    FILE(APPEND ${FILEHANDLE} "${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/${SHORTPATH}/${item}\n")
  ENDFOREACH (item ${SCRIPTLIST})


  bis_complex_tcl3src(${PATHNAME} ${SHORTPATH} ${FILEHANDLE2})

  

endmacro(bis_complex_tcl3)


macro(bis_complex_tcl PATHNAME   FILEHANDLE FILEHANDLE2 )
  GET_FILENAME_COMPONENT(SHORTPATH ${PATHNAME} NAME )        
  bis_complex_tcl3(${PATHNAME} ${SHORTPATH} ${FILEHANDLE} ${FILEHANDLE2})
endmacro(bis_complex_tcl)

