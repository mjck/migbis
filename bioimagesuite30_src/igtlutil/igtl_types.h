//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
//BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
//BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
//BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
//BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
//BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
//BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//BIOIMAGESUITE_LICENSE  See also  http://www.gnu.org/licenses/gpl.html
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
//BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
//BIOIMAGESUITE_LICENSE 
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------

/*=========================================================================

  Program:   Open IGT Link Library
  Module:    $RCSfile: $
  Language:  C
  Date:      $Date: $
  Version:   $Revision: $

  Copyright (c) Insight Software Consortium. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __IGTL_TYPES_H
#define __IGTL_TYPES_H

#include "vtkConfigure.h"

/* 8-bit integer type */
#if VTK_SIZEOF_CHAR == 1
  typedef unsigned char igtl_uint8;
  typedef char          igtl_int8;
#else
  # error "No native data type can represent an 8-bit integer."
#endif

/* 16-bit integer type */
#if VTK_SIZEOF_SHORT == 2
  typedef unsigned short igtl_uint16;
  typedef signed short   igtl_int16;
#elif VTK_SIZEOF_INT == 2
  typedef unsigned int   igtl_uint16;
  typedef signed int     igtl_int16;
#else
  # error "No native data type can represent a 16-bit integer."
#endif

/* 32-bit integer type */
#if VTK_SIZEOF_INT == 4
  typedef unsigned int   igtl_uint32;
  typedef signed int     igtl_int32;
#elif VTK_SIZEOF_LONG == 4
  typedef unsigned long  igtl_uint32;
  typedef signed long    igtl_int32;
#else
  # error "No native data type can represent a 32-bit integer."
#endif

/* 64-bit integer type */
#if defined(VTK_TYPE_USE_LONG_LONG) && VTK_SIZEOF_LONG_LONG == 8
  typedef unsigned long long igtl_uint64;
  typedef signed long long   igtl_int64;
#elif VTK_SIZEOF_INT == 8
  typedef unsigned int       igtl_uint64;
  typedef signed int         igtl_int64;
#elif VTK_SIZEOF_LONG == 8
  typedef unsigned long      igtl_uint64;
  typedef signed long        igtl_64;
#elif defined(VTK_TYPE_USE___INT64) && VTK_SIZEOF___INT64 == 8
  typedef unsigned __int64   igtl_uint64;
  typedef signed __int64     igtl_int64;
#elif defined(VTK_TYPE_USE_INT64_T) && VTK_SIZEOF_INT64_T == 8
  typedef unsigned int64_t   igtl_uint64;
  typedef signed int64_t     igtl_int64;
#else
  # error "No native data type can represent a 64-bit integer."
#endif

/* 32-bit floating point type */
#if VTK_SIZEOF_FLOAT == 4
  typedef float              igtl_float32;
#else
# error "No native data type can represent a 32-bit floating point value."
#endif

/* 64-bit floating point type */
#if VTK_SIZEOF_DOUBLE == 8
  typedef double             igtl_float64;
#else
# error "No native data type can represent a 64-bit floating point value."
#endif


#endif /* __VTK_TYPES_H */

