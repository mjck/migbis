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

//BTX
/*
  Compatibility Hacks for MacOSX and SGI IRIX
*/

#ifndef PXISINF_INCLUDE
#define PXISINF_INCLUDE


#ifdef __APPLE__

  #if ( __GNUC__ >=4 ) 
    #include <cmath>
    #define isnan(x) std::isnan(x)
    #define isinf(x) std::isinf(x)
  #elif (( __GNUC__ ==3 ) && ( __GNUC_MINOR__ >3 ))
    #include  <math.h>
  #else
    extern "C" {  int isnan(double); }
    extern "C" {  int isinf(double);  }
  #endif
#endif

#ifndef __APPLE__
    #ifndef isnan
       # define isnan(x) \
           (sizeof (x) == sizeof (long double) ? isnan_ld (x) \
           : sizeof (x) == sizeof (double) ? isnan_d (x) \
           : isnan_f (x))
       static inline int isnan_f  (float       x) { return x != x; }
       static inline int isnan_d  (double      x) { return x != x; }
       static inline int isnan_ld (long double x) { return x != x; }
     #endif

     #ifndef isinf
        # define isinf(x) \
          (sizeof (x) == sizeof (long double) ? isinf_ld (x) \
           : sizeof (x) == sizeof (double) ? isinf_d (x) \
           : isinf_f (x))
       static inline int isinf_f  (float       x) { return isnan (x - x); }
       static inline int isinf_d  (double      x) { return isnan (x - x); }
       static inline int isinf_ld (long double x) { return isnan (x - x); }
     #endif
#endif

#ifndef M_PI
#define M_PI 3.1415926854
#endif

#endif /* PXISINF_INCLUDE */

//ETX

