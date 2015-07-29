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



SET(KITBASE database)
SET(KIT vtkbisDatabase)

INCLUDE_DIRECTORIES(${ITK_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/${KITBASE})
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/Common)

IF(BIOIMAGESUITE3_USE_MYSQL_DATABASE_MODULE)
INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})
ENDIF(BIOIMAGESUITE3_USE_MYSQL_DATABASE_MODULE)


# -------------------- Add Files Here --------------------
SET (KITSRCS
vtkbisDatabase.cpp
vtkbisBaseProcedure.cpp
vtkbisPatient.cpp
vtkbisProtocol.cpp
vtkbisQuery.cpp
vtkbisVisit.cpp
vtkbisSeries.cpp
vtkbisStudy.cpp
# Above tested with SQLite
vtkbisInjection.cpp
# Below is sqlite specific
vtkimSQLite.cpp
vtkbisSQLiteDatabase.cpp
vtkbisFilenameTable.cpp
vtkbisAlgorithmTable.cpp
)
  

IF(BIOIMAGESUITE3_USE_MYSQL_DATABASE_MODULE)
SET (KITSRCS
vtkbisMySQLDatabase.cpp
vtkbisMySQLQuery.cpp
${KITSRCS})
ENDIF(BIOIMAGESUITE3_USE_MYSQL_DATABASE_MODULE)

SET(KITLIBS
  vtkIO
  vtkIOTCL
  ${BIOIMAGESUITE3_COMMON_LIBS}
)

SET(KITJAVALIBS
  vtkIO
  vtkIOJava
  ${BIOIMAGESUITE3_COMMON_JAVALIBS}
)



# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

SET (KITEXTRAFILES)
SET (KITTCLSRCS )
bis_complex("database" ${KIT} ${FILELIST})


# -------------------------------------------------------------------
#MySQL Stuff
# -------------------------------------------------------------------

IF(BIOIMAGESUITE3_USE_MYSQL_DATABASE_MODULE)
#ADD_EXECUTABLE(addPatient addPatient.cpp)
#TARGET_LINK_LIBRARIES(addPatient ${KIT} ${KITLIBS})

#ADD_EXECUTABLE(addPatient2 addPatient2.cpp)
#TARGET_LINK_LIBRARIES(addPatient2 ${KIT} ${KITLIBS})

#ADD_EXECUTABLE(addPatient3 addPatient3.cpp)
#TARGET_LINK_LIBRARIES(addPatient3 ${KIT} ${KITLIBS})

#ADD_EXECUTABLE(searchImage searchImage.cpp)
#TARGET_LINK_LIBRARIES(searchImage ${KIT} ${KITLIBS})

#ADD_EXECUTABLE(testQuery testQuery.cpp)
#TARGET_LINK_LIBRARIES(testQuery ${KIT} ${KITLIBS})

# -----------------------------------------------------------
# SQLite stuff
# -----------------------------------------------------------

#ADD_EXECUTABLE(mysql2sqlite mysql2sqlite.cpp)
#TARGET_LINK_LIBRARIES(mysql2sqlite ${KIT} ${KITLIBS})

#ADD_EXECUTABLE(sqlite_test2 sqlite_test2.cpp)
#TARGET_LINK_LIBRARIES(sqlite_test2 ${KIT} ${KITLIBS})

#ADD_EXECUTABLE(sqlite_test3 sqlite_test3.cpp)
#TARGET_LINK_LIBRARIES(sqlite_test3 ${KIT} ${KITLIBS})

#ADD_EXECUTABLE(sqlite_test4 sqlite_test4.cpp)
#TARGET_LINK_LIBRARIES(sqlite_test4 ${KIT} ${KITLIBS})



#ADD_EXECUTABLE(vtkimsqlite_test vtkimsqlite_test.cpp)
#TARGET_LINK_LIBRARIES(vtkimsqlite_test ${KIT} ${KITLIBS})

ENDIF(BIOIMAGESUITE3_USE_MYSQL_DATABASE_MODULE)

