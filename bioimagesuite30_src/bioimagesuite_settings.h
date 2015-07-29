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

/* 
 * Here is where system computed values get stored.
 * These values should only change when the target compile platform changes.
 */

/* #undef BIOIMAGESUITE_USE_NUMERICAL_RECIPES */
#ifdef BIOIMAGESUITE_USE_NUMERICAL_RECIPES
#define BIS_USE_NR 1
#endif

/* #undef BIOIMAGESUITE_EXPERIMENTAL */
/* #undef BIOIMAGESUITE3_USE_CUDA */
/* #undef BIOIMAGESUITE3_USE_CUDA_ADVANCED */
/* #undef BIOIMAGESUITE3_USE_PYTHON */

#ifdef BIOIMAGESUITE3_USE_CUDA
/* #undef BIOIMAGESUITE3_USE_CUDA_VOLUME_RENDERING */
#define BIOIMAGESUITE3_CUDA_VERSION ""
#endif

// This is the version of BioImage Suite !!!
#define BIOIMAGESUITE_STRING_VERSION "3.01"

#define BIOIMAGESUITE_PACK_VERSION ""

/* #undef CMAKE_COMPILER_2005 */

/* #undef BIOIMAGESUITE3_USE_MYSQL_DATABASE_MODULE */

/* #undef BIOIMAGESUITE3_EXCLUDE_CODE */


