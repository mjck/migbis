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

#ifndef _ZNZLIB_H_
#define _ZNZLIB_H_

/*
znzlib.h  (zipped or non-zipped library)

*****            This code is released to the public domain.            *****

*****  Author: Mark Jenkinson, FMRIB Centre, University of Oxford       *****
*****  Date:   September 2004                                           *****

*****  Neither the FMRIB Centre, the University of Oxford, nor any of   *****
*****  its employees imply any warranty of usefulness of this software  *****
*****  for any purpose, and do not assume any liability for damages,    *****
*****  incidental or otherwise, caused by any use of this document.     *****

*/

/*

This library provides an interface to both compressed (gzip/zlib) and
uncompressed (normal) file IO.  The functions are written to have the
same interface as the standard file IO functions.  

To use this library instead of normal file IO, the following changes
are required:
 - replace all instances of FILE* with znzFile
 - change the name of all function calls, replacing the initial character
   f with the znz  (e.g. fseek becomes znzseek)
 - add a third parameter to all calls to znzopen (previously fopen)
   that specifies whether to use compression (1) or not (0)
 - use znz_isnull rather than any (pointer == NULL) comparisons in the code
 
NB: seeks for writable files with compression are quite restricted

*/


/*=================*/
#ifdef  __cplusplus
extern "C" {
#endif
/*=================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

  /* Hack Xenios Papademetris for BioImage Suite (where ZLIB Exists) */
  /* Eliminated testing for zlib etc. */
#include <vtk_zlib.h>
  /* End of Hack */

struct znzptr {
  int withz;
  FILE* nzfptr;
  gzFile zfptr;
} ;

/* the type for all file pointers */
typedef struct znzptr * znzFile;


/* int znz_isnull(znzFile f); */
/* int znzclose(znzFile f); */
#define znz_isnull(f) ((f) == NULL)
#define znzclose(f)   Xznzclose(&(f))

/* Note extra argument (use_compression) where 
   use_compression==0 is no compression
   use_compression!=0 uses zlib (gzip) compression
*/

znzFile znzopen(const char *path, const char *mode, int use_compression);

znzFile znzdopen(int fd, const char *mode, int use_compression);

int Xznzclose(znzFile * file);

size_t znzread(void* buf, size_t size, size_t nmemb, znzFile file);

size_t znzwrite(const void* buf, size_t size, size_t nmemb, znzFile file);

long znzseek(znzFile file, long offset, int whence);

int znzrewind(znzFile stream);

long znztell(znzFile file);

int znzputs(const char *str, znzFile file);

char * znzgets(char* str, int size, znzFile file);

int znzputc(int c, znzFile file);

int znzgetc(znzFile file);

#if !defined(WIN32)
int znzprintf(znzFile stream, const char *format, ...);
#endif

/*=================*/
#ifdef  __cplusplus
}
#endif
/*=================*/

#endif

