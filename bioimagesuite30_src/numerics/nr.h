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


//
//
//
//  This File contains the following Numerical Recipes derived code that
//  will need to be removed prior to it becoming open source
//
//  Headers are OK .. but functions lubksb, ludcmp, cyclictridiag, tridiag are bad!
//
//---------------------------------------------------------------------------------------------------------
// Modified Header 26th December 1995 - ANSI C only to comply with C++

#include <math.h>
#include "nrutil.h"

#ifndef _NUM_RECIPES_FIXED
#define _NUM_RECIPES_FIXED
// Defining namespace for old code


//  *--*  *--*  *--*  *--*  *--*  *--*  *--*  *--*  *--*  *--*  *--* 
//  linsystems.cc  
//  *--*  *--*  *--*  *--*  *--*  *--*  *--*  *--*  *--*  *--*  *--* 

void  lubksb(double **a, int n, int *indx, double *b);
void  ludcmp(double **a, int n, int *indx, double *d,int& singular);

void vtkMath_LUFactorLinearSystem(double **A, int *index, int size,int& singular);
void vtkMath_LUSolveLinearSystem(double **A, int *index,double *x, int size);


int   sor(double **a,int n, double *b,double *oldx,
	  double thr,double omega);
int   conjugate(double** a,int n,double* b,double* oldx,double thr);
int   precondconjugate(double** a,int n,double* b,double* oldx,double thr);


/*void cyclictridiag(float a[],float b[],float c[], 
	    float alpha, float beta, 
	    float r[],float x[], long n);
void cyclictridiag_gsl(float a[],float b[],float c[], 
	    float alpha, float beta, 
	    float r[],float x[], long n);
void tridag(float a[], float b[], float c[], float r[], float u[],
long n);*/


int solve_cyc_tridiag_nonsym_gsl(float diag[],
				 float abovediag[], 
				 float belowdiag[], 
				 float rhs[], 
				 float x[], 
				 int    N);








#endif  /* _NUM_RECIPES */



