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


# ------------------------------------------------------------------------------------------------
# Get Some Base Directory Names Together
# ------------------------------------------------------------------------------------------------

SET (BIOPATH ${BIOIMAGESUITE3_INSTALL_EXTRAPATH})
SET (TMP "${VTK_DIR}/../../..")
GET_FILENAME_COMPONENT(BIOBASEPATH ${TMP} ABSOLUTE )        

#MESSAGE("BIOBASEPATH = ${BIOBASEPATH}, ${TMP}")


SET (CMAKE_MODULE_PATH ${BIOIMAGESUITE3_SOURCE_DIR}/installtools)

# ------------------------------------------------------------------------------------------------
#
#   Some more common variables
#
# ------------------------------------------------------------------------------------------------

SET (CPACK_PACKAGE_VERSION_MAJOR ${BIOIMAGESUITE3_VERSION_MAJOR})
SET (CPACK_PACKAGE_VERSION_MINOR ${BIOIMAGESUITE3_VERSION_MINOR})
SET (CPACK_PACKAGE_VERSION_PATCH ${BIOIMAGESUITE3_VERSION_PATCH})
SET (CPACK_PACKAGE_VERSION "${BIOIMAGESUITE3_VERSION_MAJOR}${BIOIMAGESUITE3_VERSION_MINOR}${BIOIMAGESUITE3_VERSION_PATCH}")
SET (CPACK_PACKAGE_VENDOR "Section of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of Medicine. (www.bioimagesuite.org)")
SET (CPACK_PACKAGE_NAME "bioimagesuite")
SET (CPACK_PACKAGE_SHORT_FILE_NAME "bioimagesuite${BIOIMAGESUITE3_VERSION_MAJOR}${BIOIMAGESUITE3_VERSION_MINOR}")

SET(TMPNAME "BioImage Suite ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "BioImage Suite ${TMPNAME}")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${BIOIMAGESUITE3_SOURCE_DIR}/BioImageSuite_Description.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${BIOIMAGESUITE3_SOURCE_DIR}/BioImageSuite_ShortLicense.txt")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "")

# ------------------------------------------------------------------------------------------------
# UNIX/APPLE STG setup


IF(BIS_UNIXBUILD)
  SET (CPACK_STGZ_HEADER_FILE ${BIOIMAGESUITE3_SOURCE_DIR}/installtools/BIS_CPack.STGZ_Header.sh.in)
  SET (CPACK_POSTINSTALL "")
  SET (CPACK_NEWPATH "\\\${newpath}/${BIOIMAGESUITE3_INSTALL_EXTRAPATH}")

  EXECUTE_PROCESS (
    COMMAND date +%d_%b_%Y
    OUTPUT_VARIABLE TMPD
    ERROR_VARIABLE TMPE
    )
  STRING(REGEX REPLACE "[\n\t]" "" TMPD "${TMPD}")



  IF(BIOIMAGESUITE3_SET_COMPILER_NAME)
    GET_FILENAME_COMPONENT(TMPE ${BIOIMAGESUITE3_COMPILER} NAME )    
  ELSE(BIOIMAGESUITE3_SET_COMPILER_NAME)
    GET_FILENAME_COMPONENT(TMPE ${CMAKE_CXX_COMPILER}  NAME )        
  ENDIF(BIOIMAGESUITE3_SET_COMPILER_NAME)

  IF (${CMAKE_CXX_FLAGS} MATCHES ".+m32.+")
    SET (TMPF "i386")
  ELSE(${CMAKE_CXX_FLAGS} MATCHES ".+m32.+")
    SET (TMPF ${CMAKE_SYSTEM_PROCESSOR})
  ENDIF(${CMAKE_CXX_FLAGS} MATCHES ".+m32.+")
  
  IF (${BIOIMAGESUITE3_EXCLUDE_CODE} MATCHES "ON")
    SET (TMPG "")
  ELSE (${BIOIMAGESUITE3_EXCLUDE_CODE} MATCHES "ON")
    SET (TMPG "-internal")
  ENDIF (${BIOIMAGESUITE3_EXCLUDE_CODE} MATCHES "ON")

#  MESSAGE ("TMPE=${TMPE}")


  SET (CPACK_PACKAGE_VERSION_PATCH
    "${CPACK_PACKAGE_VERSION_PATCH}_${TMPD}")
 SET (CPACK_PACKAGE_VERSION "${BIOIMAGESUITE3_VERSION_MAJOR}${BIOIMAGESUITE3_VERSION_MINOR}_${CPACK_PACKAGE_VERSION_PATCH}-${CMAKE_SYSTEM_NAME}-${TMPE}-${TMPF}${TMPG}" CACHE TYPE STRING)
 SET (CPACK_PACKAGE_VERSION "${BIOIMAGESUITE3_VERSION_MAJOR}${BIOIMAGESUITE3_VERSION_MINOR}_${CPACK_PACKAGE_VERSION_PATCH}-${CMAKE_SYSTEM_NAME}-${TMPE}-${TMPF}${TMPG}")



  SET (BUILDNAME "${CMAKE_SYSTEM_NAME}-${TMPE}-${TMPF}${TMPG}" CACHE TYPE STRING FORCE)

  MESSAGE("++ Package version = ${CPACK_PACKAGE_VERSION}, BUILDNAME=${BUILDNAME}")
  

  IF(BIOIMAGESUITE3_PACK_BASE)
    SET (CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}-complete")
    install (DIRECTORY ${BIOBASEPATH} DESTINATION "." USE_SOURCE_PERMISSIONS)
    set (CPACK_POSTINSTALL "${CPACK_POSTINSTALL}\n${CPACK_NEWPATH}/installtools/bis_fixscripts -package ${CPACK_NEWPATH}/lib/scriptfiles.txt ${CPACK_NEWPATH}\n")
    set (CPACK_POSTINSTALL "${CPACK_POSTINSTALL}\n${CPACK_NEWPATH}/installtools/bis_fixscripts_base \\\${newpath}\n")
    set (CPACK_POSTINSTALL "${CPACK_POSTINSTALL}\nchmod -R 755 \\\${newpath}/bioimagesuite3_base*")
  ELSE(BIOIMAGESUITE3_PACK_BASE)
        set (CPACK_POSTINSTALL "${CPACK_POSTINSTALL}\n${CPACK_NEWPATH}/installtools/bis_fixscripts -install ${CPACK_NEWPATH}/lib/scriptfiles.txt ${CPACK_NEWPATH}\n")
  ENDIF(BIOIMAGESUITE3_PACK_BASE)




# Excluded TCL Code 
  
  
  SET (CPACK_POSTINSTALL
    "${CPACK_POSTINSTALL}\n\necho \\\"Change permissions\\\"")
  IF(BIOIMAGESUITE3_PACK_BASE)
    set (CPACK_POSTINSTALL "${CPACK_POSTINSTALL}\nchmod -R 755 \\\${newpath}/bioimagesuite3_base*")
  ENDIF(BIOIMAGESUITE3_PACK_BASE)
  SET (CPACK_POSTINSTALL "${CPACK_POSTINSTALL}\nchmod -R 755 ${CPACK_NEWPATH}")



  IF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    IF(BIOIMAGESUITE3_INSTALL_ICONS)

    ELSE(BIOIMAGESUITE3_INSTALL_ICONS)      
      SET (CPACK_POSTINSTALL
	"${CPACK_POSTINSTALL}\necho \\\"Execute ${CPACK_NEWPATH}/createmenuentries.sh to create menu entries for GNOME and KDE\\\"")
    ENDIF(BIOIMAGESUITE3_INSTALL_ICONS)      
  ENDIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  INCLUDE(InstallRequiredSystemLibraries)
  


  #  MESSAGE("Version = ${CPACK_PACKAGE_VERSION}")
  SET(CPACK_PACKAGE_FILE_NAME "bioimagesuite-${CPACK_PACKAGE_VERSION}")

  SET(CPACK_GENERATOR "STGZ")
  INCLUDE(CPack)

ELSE (BIS_UNIXBUILD)

# -------------------------------------------------------------------------------------
#  Creates an installer for inno setup (complete distribution only)
# -------------------------------------------------------------------------------------


  #  include (InstallRequiredSystemLibraries)

  FIND_PROGRAM(INNO_COMPILER
    NAMES 
    ISCC.exe
    ISCC
    iscc.exe
    iscc
    )

  IF (INNO_COMPILER)

    EXECUTE_PROCESS (
      COMMAND cmd.exe /cdate /t
      OUTPUT_VARIABLE TMPD
      ERROR_VARIABLE TMPE
      )

    STRING(REGEX REPLACE "/" "_" TMPD "${TMPD}")
    STRING(REGEX REPLACE " " "-" TMPD "${TMPD}")
    STRING(REGEX REPLACE "[\n\t]" "" TMPD "${TMPD}")
    STRING(REGEX REPLACE "-" "" TMPD "${TMPD}")
    STRING(REGEX REPLACE "^[A-Za-z]+" "" TMPD "${TMPD}")
    #    MESSAGE("Date = ,${TMPD},")
    
    SET (INNOSCRIPT  "${BIOIMAGESUITE3_VERSION_MAJOR}${BIOIMAGESUITE3_VERSION_MINOR}_${BIOIMAGESUITE3_VERSION_PATCH}")
    SET (BUILDNAME "Windows")
    IF(CMAKE_GENERATOR MATCHES  "Visual Studio 9")	
      SET (INNOSCRIPT "${INNOSCRIPT}_${TMPD}-Windows_vs2008")
      SET (BUILDNAME  "Win32-Visual Studio 2008" CACHE TYPE STRING FORCE)
    ELSE(CMAKE_GENERATOR MATCHES  "Visual Studio 9")	
       IF (CMAKE_COMPILER_2005)	
	  SET (INNOSCRIPT "${INNOSCRIPT}_${TMPD}-Windows_vs2005")
	  SET (BUILDNAME  "Win32-Visual Studio 2005" CACHE TYPE STRING FORCE)
      ELSE (CMAKE_COMPILER_2005)	
            SET (INNOSCRIPT "${INNOSCRIPT}_${TMPD}-Windows_vs2003")
	    SET (BUILDNAME  "Win32-Visual Studio 2003" CACHE TYPE STRING FORCE)
      ENDIF (CMAKE_COMPILER_2005)
    ENDIF(CMAKE_GENERATOR MATCHES  "Visual Studio 9")	

    IF(BIOIMAGESUITE3_PACK_BASE)	
	SET(INNOSCRIPT "${INNOSCRIPT}_complete")
    ENDIF(BIOIMAGESUITE3_PACK_BASE)	

    SET (TAILNAME "bioimagesuite-${INNOSCRIPT}")

    SET (CPACK_PACKAGE_VERSION  ${INNOSCRIPT})
    SET (INNOSCRIPT "${BIOIMAGESUITE3_BINARY_DIR}/bioimagesuite-${INNOSCRIPT}.iss")


    MESSAGE("++ Package version = ${INNOSCRIPT} BUILDNAME=${BUILDNAME}")



    ADD_CUSTOM_TARGET(PACKAGE_WITH_INNOSETUP
      ${INNO_COMPILER} ${INNOSCRIPT}
      COMMENT "Creating package ${INNOSCRIPT}"
      )

    SET (VERSION ${BIOIMAGESUITE3_VERSION_MAJOR}${BIOIMAGESUITE3_VERSION_MINOR}${BIOIMAGESUITE3_VERSION_PATCH})
    #    GET_FILENAME_COMPONENT(BIOPATH ${CMAKE_INSTALL_PREFIX} NAME )        

    
    SET (FOUT ${INNOSCRIPT})
    FILE(WRITE  ${FOUT} "[Setup]\n")

    IF (BIOIMAGESUITE3_PACK_BASE)
      FILE(APPEND ${FOUT}  "AppVerName=BioImage Suite Complete ${VERSION}\n")
    ELSE(BIOIMAGESUITE3_PACK_BASE)
      FILE(APPEND ${FOUT}  "AppVerName=BioImage Suite Update ${VERSION}\n")
    ENDIF(BIOIMAGESUITE3_PACK_BASE)

    FILE(APPEND ${FOUT}  "AppPublisher=Bioimage Suite Project\n")
    FILE(APPEND ${FOUT}  "AppPublisherURL=http://www.bioimagesuite.org\n")
    FILE(APPEND ${FOUT}  "AppSupportURL=http://www.bioimagesuite.org\n")
    FILE(APPEND ${FOUT}  "AppUpdatesURL=http://www.bioimagesuite.org\n")
    FILE(APPEND ${FOUT}  "OutputDir=${BIOIMAGESUITE3_BINARY_DIR}\n")
    FILE(APPEND ${FOUT}  "DefaultDirName=${CMAKE_INSTALL_PREFIX}\n")
    FILE(APPEND ${FOUT}  "AppName=BioImage Suite ${VERSION}\n")
    FILE(APPEND ${FOUT}  "DefaultGroupName=BioImage_Suite_${VERSION}\n")
    FILE(APPEND ${FOUT}  "LicenseFile=${BIOIMAGESUITE3_SOURCE_DIR}\\BioImageSuite_License.txt\n")
    FILE(APPEND ${FOUT}  "SetupIconFile=${BIOIMAGESUITE3_SOURCE_DIR}\\bioimagesuite\\images\\bioimagesuite.ico\n")
    FILE(APPEND ${FOUT}  "OutputBaseFilename=${TAILNAME}\n")	
    FILE(APPEND ${FOUT}  "AllowRootDirectory=no\n")
    FILE(APPEND ${FOUT}  "AllowUNCPath=no\n")
    FILE(APPEND ${FOUT}  "SetupLogging=yes\n")

    
    FILE(APPEND ${FOUT}  "Compression=zip\n")
    FILE(APPEND ${FOUT}  "SolidCompression=yes\n")
    FILE(APPEND ${FOUT}  "RestartIfNeededByRun=no\n")
    FILE(APPEND ${FOUT}  "ChangesAssociations=yes\n")
    IF (BIOIMAGESUITE3_PACK_BASE)
       FILE (APPEND ${FOUT} "DirExistsWarning=yes\n")
    ELSE (BIOIMAGESUITE3_PACK_BASE)
       FILE (APPEND ${FOUT} "DirExistsWarning=no\n")
    ENDIF (BIOIMAGESUITE3_PACK_BASE)
    FILE (APPEND ${FOUT} "WizardImageFile=${BIOIMAGESUITE3_SOURCE_DIR}\\bioimagesuite\\images\\bioimagesuite_setuplogo.bmp\n\n\n")

    FILE(APPEND ${FOUT}  "\n[Languages]\n")
    FILE(APPEND ${FOUT}  "Name: \"english\"; MessagesFile: \"compiler:Default.isl\"\n\n")

    FILE(APPEND ${FOUT}  "[Files]\n")
    FILE(APPEND ${FOUT}  "Source: \"${CMAKE_INSTALL_PREFIX}\\${BIOPATH}\\*\"; DestDir: \"{app}\\${BIOPATH}\\\"; Flags: ignoreversion recursesubdirs createallsubdirs;Permissions: everyone-readexec\n")

    IF (BIOIMAGESUITE3_PACK_BASE)
      FILE(APPEND ${FOUT}  "Source: \"${BIOBASEPATH}\\*\";    DestDir: \"{app}\\bioimagesuite3_base\\\"; Flags: ignoreversion recursesubdirs createallsubdirs; Permissions: everyone-readexec\n")
    ENDIF (BIOIMAGESUITE3_PACK_BASE)
    
    FILE(APPEND ${FOUT}  "\n")
    FILE(APPEND ${FOUT}  "[Icons]\n")
    FILE(APPEND ${FOUT}  "Name: \"{group}\\BioImage Suite Menu\"; Filename: \"{app}\\${BIOPATH}\\bioimagesuite.bat\" \n")
    FILE(APPEND ${FOUT}  "Name: \"{group}\\BioImage Suite Console\"; Filename: \"{app}\\${BIOPATH}\\bisconsole.bat\" \n")
    FILE(APPEND ${FOUT}  "Name: \"{group}\\Links\\BioImage Suite Web Page\"; Filename: \"http://www.bioimagesuite.org\" \n")
    FILE(APPEND ${FOUT}  "Name: \"{group}\\Links\\BioImage Suite Forum\"; Filename: \"http://research.yale.edu/bioimagesuite/forum/\" \n")

    IF (BIOIMAGESUITE3_PACK_BASE)
      FILE(APPEND ${FOUT}  "Name: \"{group}\\Base\\VTK\"; Filename: \"{app}\\bioimagesuite3_base\\vtk52\\bin\\vtk.exe\" \n")
      FILE(APPEND ${FOUT}  "Name: \"{group}\\Base\\TCLSH\"; Filename: \"{app}\\bioimagesuite3_base\\tcl84\\bin\\tclsh84.exe\" \n")
      FILE(APPEND ${FOUT}  "Name: \"{group}\\Base\\WISH\"; Filename: \"{app}\\bioimagesuite3_base\\tcl84\\bin\\wish84.exe\" \n")
      FILE(APPEND ${FOUT}  "Name: \"{group}\\Base\\CMakeSetup\"; Filename: \"{app}\\bioimagesuite3_base\\cmake26\\bin\\CMakeSetup.exe\" \n")
    ENDIF (BIOIMAGESUITE3_PACK_BASE)				 
    FILE(APPEND ${FOUT}  "Name: \"{group}\\Admin\\Restore Associations\"; Filename: \"{app}\\${BIOPATH}\\setassociations.bat\" \n")


    FILE(APPEND ${FOUT}  "Name: \"{group}\\Uninstall\\{cm:UninstallProgram,BioImage Suite}\"; Filename: \"{uninstallexe}\" \n")
    FILE(APPEND ${FOUT}  "\n")
    FILE(APPEND ${FOUT}  "[Run]\n")

    IF (BIOIMAGESUITE3_PACK_BASE)
      FILE(APPEND ${FOUT}  "Filename: \"{app}\\${BIOPATH}\\installtools\\bis_fixscripts_base.exe\"; Parameters: \"{app}\" \n")
      FILE(APPEND ${FOUT}  "Filename: \"{app}\\${BIOPATH}\\installtools\\bis_fixscripts.exe\"; Parameters: \"-package {app}\\${BIOPATH}\\lib\\scriptfiles.txt {app}\\${BIOPATH}\"\n")
      FILE(APPEND ${FOUT}  "Filename: \"{app}\\${BIOPATH}\\setassociations.bat\" \n")
    ELSE (BIOIMAGESUITE3_PACK_BASE)
      FILE(APPEND ${FOUT}  "Filename: \"{app}\\${BIOPATH}\\installtools\\bis_fixscripts.exe\"; Parameters: \"-install {app}\\${BIOPATH}\\lib\\scriptfiles.txt {app}\\${BIOPATH}\"\n")
      FILE(APPEND ${FOUT}  "Filename: \"{app}\\${BIOPATH}\\setassociations.bat\" \n")
    ENDIF (BIOIMAGESUITE3_PACK_BASE)


    
  ENDIF (INNO_COMPILER)



  SET(CPACK_PACKAGE_FILE_NAME "bioimagesuite-${CPACK_PACKAGE_VERSION}")


  SET(CPACK_GENERATOR "ZIP")
  INCLUDE(CPack)

  
ENDIF(BIS_UNIXBUILD)




