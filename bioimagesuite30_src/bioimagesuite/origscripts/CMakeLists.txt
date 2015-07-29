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
# Source files
#
# Here is where you can add the name of your local common classes.
#

SET(KITSCRIPTS 
  setpaths.sh.in
  setpaths.csh.in  
  setpaths_console.sh.in  
  runscript.in
)

SET(WINAPPS
  setpaths.bat.in
  bisconsole.bat.in
  bisexec.bat.in
  biscppexec.bat.in
  bioimagesuite.bat.in
  runscript.bat.in
  )

SET(UNIXAPPS
  start_bioimagesuite.in
  bis_console.in
  bisexec.in
  biscppexec.in
 )	

IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
SET (UNIXAPPS
${UNIXAPPS}
createmenuentries.sh.in
)
ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")


SET(KITAPPS 
  ${WINAPPS}
  ${UNIXAPPS}
)



# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line
#bis_complex_tcl3src(PATHNAME SHORTPATH FILEHANDLE)
bis_complex_tcl3src(bioimagesuite/origscripts origscripts ${FILELIST})



# ---------------------------------------------------------------------------
#  Put the main files 
# ---------------------------------------------------------------------------


IF (BIOIMAGESUITE_INSTALL_SOURCE)
  install(FILES ${KITSCRIPTS} DESTINATION  ${BIOIMAGESUITE_INSTALL_SRCPATH}/bioimagesuite/origscripts)
  install(PROGRAMS ${KITAPPS} DESTINATION  ${BIOIMAGESUITE_INSTALL_SRCPATH}/bioimagesuite/origscripts)
ENDIF (BIOIMAGESUITE_INSTALL_SOURCE)

IF (UNIX)
  SET(KITAPPS
    ${UNIXAPPS})
ELSE(UNIX)
  SET(KITAPPS
    ${WINAPPS})

  IF (BIS_UNIXBUILD)
    SET(KITAPPS
      ${KITAPPS}
      setpaths.sh.in
      bisexec.in
      biscppexec.in
      )
  ENDIF (BIS_UNIXBUILD)
    
  SET(KITSCRIPTS )
ENDIF(UNIX)




SET (TMP  ${KITSCRIPTS} ${KITAPPS} )
FOREACH (item ${TMP})

  STRING (REGEX REPLACE ".in+" "" item2 ${item})
#  MESSAGE("Mapping ${item} to ${item2}")
  configure_file( ${BIOIMAGESUITE3_SOURCE_DIR}/bioimagesuite/origscripts/${item}
    ${BIOIMAGESUITE3_BINARY_DIR}/${item2} @ONLY )
  
  install(FILES ${BIOIMAGESUITE3_BINARY_DIR}/${item2}  DESTINATION ${BIOIMAGESUITE3_INSTALL_EXTRAPATH}/${SHORTPATH}
    PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE )
  FILE(APPEND ${SCRIPTFILELIST} "${item2}\n")
ENDFOREACH(item ${TMP})

IF (UNIX)
  EXECUTE_PROCESS (COMMAND chmod +x  ${BIOIMAGESUITE3_BINARY_DIR}/bisexec)
  EXECUTE_PROCESS (COMMAND chmod +x  ${BIOIMAGESUITE3_BINARY_DIR}/biscppexec)
ELSE (UNIX)
  IF (BIS_UNIXBUILD)
    EXECUTE_PROCESS (COMMAND chmod +x  ${BIOIMAGESUITE3_BINARY_DIR}/bisexec)
    EXECUTE_PROCESS (COMMAND chmod +x  ${BIOIMAGESUITE3_BINARY_DIR}/biscppexec)
  ENDIF (BIS_UNIXBUILD)
ENDIF (UNIX)



install(SCRIPT ${BIOIMAGESUITE3_BINARY_DIR}/bioimagesuitepostinstall.cmake )

