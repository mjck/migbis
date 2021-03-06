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
# Here is where you can add the name of your local imaging classes.
#

SET(KITBASE numerics)
SET(KIT ${KITBASE})

INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/${KITBASE})
INCLUDE_DIRECTORIES(${BIOIMAGESUITE3_SOURCE_DIR}/include)
INCLUDE_DIRECTORIES(${ITK_INCLUDE_DIRS})

#MESSAGE("DIRS =  ${ITK_INCLUDE_DIRS}")

SET (KITSRCS
  # Fitpack
  #
  fitpack.c
  #
  # Legacy Stuff
  #
  linsystems.cpp
  pxabaqusstructures.cpp
  pxtriangulatedstack.cpp
  nrutil.cpp
  pxchamferdistancemap.cpp
  pxutil.cpp
  peng_cheng_util.cpp
  pxcontour.cpp
  pxgeometrystruct.cpp
  px3dimage.cpp
  pxcontourstack.cpp
  pximage.cpp
  pxabaqussolid.cpp
  pxshapetrackingclass.cpp
  pxmarkerclass.cpp
  pxcrystalabaqus.cpp
  pxfloatimage.cpp
  pxfloatmatrix.cpp
  pxfpbspline.cpp
  pxfpbsplinestack.cpp
  pxregion.cpp
  # 
  # libf2c fix
  #
  f2cfix.c
  )


SET (KITEXTRAFILES
numerics/tnt/jama_cholesky.h
numerics/tnt/tnt_array1d.h
numerics/tnt/tnt_array3d_utils.h
numerics/tnt/tnt_fortran_array2d_utils.h
numerics/tnt/tnt_sparse_matrix_csr.h
numerics/tnt/jama_eig.h
numerics/tnt/tnt_array1d_utils.h
numerics/tnt/tnt_cmat.h
numerics/tnt/tnt_fortran_array3d.h
numerics/tnt/tnt_stopwatch.h
numerics/tnt/jama_lu.h
numerics/tnt/tnt_array2d.h
numerics/tnt/tnt_fortran_array1d.h
numerics/tnt/tnt_fortran_array3d_utils.h
numerics/tnt/tnt_subscript.h
numerics/tnt/jama_qr.h
numerics/tnt/tnt_array2d_utils.h
numerics/tnt/tnt_fortran_array1d_utils.h
numerics/tnt/tnt.h
numerics/tnt/tnt_vec.h
numerics/tnt/jama_svd.h
numerics/tnt/tnt_array3d.h
numerics/tnt/tnt_fortran_array2d.h
numerics/tnt/tnt_math_utils.h
numerics/tnt/tnt_version.h
numerics/nr.h
)

SET (KITTCLSRCS )


# --------------------------------------------------------------------------
# You probably do not need to modify anything below this line

bis_static(${KIT})
bis_sourcelist("numerics" ${KIT} ${FILELIST})

