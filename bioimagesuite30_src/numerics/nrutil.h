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
// The top 90% is slightly tweaked from public domain Numerical Recipes Code
// The bottom 10% is from Pengcheng Shi's code (peng_cheng_util.cpp) and needs to be eliminated
// (The header file is fine!)
//
//---------------------------------------------------------------------------------------------------------
// New version of nrutil.h function prototypes for use with ANSI C
// April 3rd 1995

#ifndef _NUM_UTILITIES
#define _NUM_UTILITIES

#include <math.h>

#ifndef M_PI
#define M_PI 3.1415927
#endif

typedef struct FCOMPLEX {float r,i;} fcomplex;
typedef struct IMMENSE {unsigned long l,r;} immense;
typedef struct GREAT {unsigned short l,c,r;} great;

void nrerror(const char* error_text);

// Allocation and Freeing of matrices and Vectors
// ----------------------------------------------
float  *vector(long nl,long nh);
double *dvector(long nl,long nh);
int    *ivector(long nl,long nh);

unsigned char *cvector(long nl, long nh);
unsigned long *lvector(long nl, long nh);

float **matrix(long nrl, long nrh,long ncl,long nch);
double **dmatrix(long nrl,long nrh,long ncl,long nch);
int **imatrix(long nrl,long nrh,long ncl,long nch);

float **submatrix(float a,int oldrl,int oldrh,int oldcl,int oldch,
		  long newrl,long newcl);
double **subdmatrix(double a,int oldrl,int oldrh,int oldcl,int oldch,
		  long newrl,long newcl);

float **convert_matrix(float *a, long nrl, long nrh, long ncl, long nch);
float ***f3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh);

void free_vector(float* v,long nl,long nh);
void free_dvector(double* v,long nl,long nh);
void free_ivector(int *v,long nl,long nh);
void free_cvector(unsigned char *v, long nl, long nh);
void free_lvector(unsigned long *v, long nl, long nh);

void free_matrix(float **m,long nrl,long nrh,long ncl,long nch);
void free_dmatrix(double **m,long nrl,long nrh,long ncl,long nch);
void free_imatrix(int **m,long nrl,long nrh,long ncl,long nch);
void free_submatrix(float** b,long nrl,long nrh,long ncl,long nch);


void free_convert_matrix(float **b,long nrl,long nrh,long ncl,long nch);
void free_subdmatrix(double** b,long nrl,long nrh,long ncl,long nch);
double **convert_dmatrix(double* a,long nrl,long nrh,long ncl,long nch);
void free_convert_dmatrix(double **b,long nrl,long nrh,long ncl,long nch);
void free_f3tensor(float ***t, long nrl, long nrh, long ncl, long nch,
	long ndl, long ndh);

// ---------------------------------------------------------
// Copy Vectrors and make matrices symmetric (upper triangular = input)
// ---------------------------------------------------------

void  symmetric(float **a, int n);
void  copymatrix(float **a,float **b,int rows,int columns);
void  copyvector(float *a,float *b,int rows);

void  dsymmetric(double **a, int n);
void  copydmatrix(double **a,double **b,int rows,int columns);
void  copydvector(double *a,double *b,int rows);

void  isymmetric(int **a, int n);
void  copyimatrix(int **a,int **b,int rows,int columns);
void  copyivector(int *a,int *b,int rows);


// -----------------------------------------------------------
//  Diagnostic Routines - print formatted vectors and matrices
// -----------------------------------------------------------
void printvector(float* v,int n,const char* fmt,const char* name,int wd=8);
void printivector(int*  v,int n,const char* fmt,const char* name,int wd=8);
void printdvector(double* v,int n,const char* fmt,const char* name,int wd=8);
void printmatrix(float** a,int row,int col,const char* fmt,const char* name,int wd=8,int off=1);
void printimatrix(int** a,int row,int col,const char* fmt,const char* name,int wd=8,int off=1);
void printdmatrix(double** a,int row,int col,const char* fmt,const char* name,int wd=8,int off=1);

// -------------------------------------------------------------
// Shift Vectors
// -------------------------------------------------------------
void shiftdvector(double* v,int shift,int np);
void shiftvector(float* v,int shift,int np);
void shiftivector(int* v,int shift,int np);

// -------------------------------------------------------------
// Peng-Cheng Utilities 
// -------------------------------------------------------------

typedef struct {              
      	double x;          	/* position 		*/
      	double y;
      	double z;
      	double K;		/* gaussian curvature   */
      	double H;		/* mean curvature	*/
      	double k1;		/* principal curvatures */
      	double k2;      
      	double k1x;		/* principal directions */
      	double k1y;
	double k1z;
	double k2x;
	double k2y;
	double k2z;
	double nx;		/* normal direction 	*/
	double ny;
	double nz;
}Vertex_3d;


typedef struct {              
      	double x;          	/* position   */
      	double y;
	double z;
}Node_3d;

void matrix_transpose(double** a,double** b, int rowsize,int colsize);
void matrix_product(double** p, double** a, int rowsize_a, int colsize_a, 
		    double** b,int /*rowsize_b*/,int colsize_b);
void matrix_inverse(double** a,double** b, int n);
void get_eigenvalues_vectors(double** a,int n,double d[],double** v,int* nrot);
void sort_eigenvalues_vectors(double* d,double** v, int n);
void lubksb0(double **a,int n,int *indx,double b[]);
void ludcmp0(double **a,int n,int* indx,double* d,int& singular);



// This came out of vtkMath, and placed here to eliminate dependency on VTK for px.. files which are
// also needed for abaqus
int vtkMath_JacobiN(float **a, int n, float *w, float **v);
int vtkMath_JacobiN(double **a, int n, double *w, double **v);
int vtkMath_Jacobi(float **a, float *w, float **v);
int vtkMath_Jacobi(double **a, double *w, double **v);

#endif /* _NUM_UTILITIES */




