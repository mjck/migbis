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
// While this file contains a ton of NR-derived code, all of this has been placed in the public domain
//       hence no issues here!
//
//---------------------------------------------------------------------------------------------------------

// New Version of nrutil.c(c) with proper ANSI C 
// Uses header nrutil.h
// April 3rd 1995

#ifdef HAVE_MALLOC_H
#include <malloc.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "nrutil.h"

#define NR_END 1
#define FREE_ARG char*

// --------------------------------------------------------
// Error Handling or not as the case may be -- exit(1)
// --------------------------------------------------------
void nrerror(const char* error_text)
{
  fprintf(stderr,"Numerical Recipes run-time error...\n");
  fprintf(stderr,"%s\n",error_text);
  fprintf(stderr,"...now exiting to system...\n");
  //  exit(1);
}

// ------------------------------------------------------------------
// Original Numerical Recipes Code for allocating and releasing data
// ------------------------------------------------------------------
float *vector(long nl, long nh)
/* allocate a float vector with subscript range v[nl..nh] */
{
	float *v;

	v=(float *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(float)));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl+NR_END;
}

int *ivector(long nl, long nh)
/* allocate an int vector with subscript range v[nl..nh] */
{
	int *v;

	v=(int *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(int)));
	if (!v) nrerror("allocation failure in ivector()");
	return v-nl+NR_END;
}

unsigned char *cvector(long nl, long nh)
/* allocate an unsigned char vector with subscript range v[nl..nh] */
{
	unsigned char *v;

	v=(unsigned char *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(unsigned char)));
	if (!v) nrerror("allocation failure in cvector()");
	return v-nl+NR_END;
}

unsigned long *lvector(long nl, long nh)
/* allocate an unsigned long vector with subscript range v[nl..nh] */
{
	unsigned long *v;

	v=(unsigned long *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(long)));
	if (!v) nrerror("allocation failure in lvector()");
	return v-nl+NR_END;
}

double *dvector(long nl, long nh)
/* allocate a double vector with subscript range v[nl..nh] */
{
	double *v;

	v=(double *)malloc((size_t) ((nh-nl+1+NR_END)*sizeof(double)));
	if (!v) nrerror("allocation failure in dvector()");
	return v-nl+NR_END;
}

float **matrix(long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	float **m;

	/* allocate pointers to rows */
	m=(float **) malloc((size_t)((nrow+NR_END)*sizeof(float*)));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl]=(float *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float)));
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

double **dmatrix(long nrl, long nrh, long ncl, long nch)
/* allocate a double matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	double **m;

	/* allocate pointers to rows */
	m=(double **) malloc((size_t)((nrow+NR_END)*sizeof(double*)));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;

	/* allocate rows and set pointers to them */
	m[nrl]=(double *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(double)));
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) 
	    m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

int **imatrix(long nrl, long nrh, long ncl, long nch)
/* allocate a int matrix with subscript range m[nrl..nrh][ncl..nch] */
{
	long i, nrow=nrh-nrl+1,ncol=nch-ncl+1;
	int **m;

	/* allocate pointers to rows */
	m=(int **) malloc((size_t)((nrow+NR_END)*sizeof(int*)));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m += NR_END;
	m -= nrl;


	/* allocate rows and set pointers to them */
	m[nrl]=(int *) malloc((size_t)((nrow*ncol+NR_END)*sizeof(int)));
	if (!m[nrl]) nrerror("allocation failure 2 in matrix()");
	m[nrl] += NR_END;
	m[nrl] -= ncl;

	for(i=nrl+1;i<=nrh;i++) m[i]=m[i-1]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

float **submatrix(float **a, long oldrl, long oldrh, long oldcl, long /*oldch*/,
	long newrl, long newcl)
/* point a submatrix [newrl..][newcl..] to a[oldrl..oldrh][oldcl..oldch] */
{
	long i,j,nrow=oldrh-oldrl+1,ncol=oldcl-newcl;
	float **m;

	/* allocate array of pointers to rows */
	m=(float **) malloc((size_t) ((nrow+NR_END)*sizeof(float*)));
	if (!m) nrerror("allocation failure in submatrix()");
	m += NR_END;
	m -= newrl;

	/* set pointers to rows */
	for(i=oldrl,j=newrl;i<=oldrh;i++,j++) m[j]=a[i]+ncol;

	/* return pointer to array of pointers to rows */
	return m;
}

float **convert_matrix(float *a, long nrl, long nrh, long ncl, long nch)
/* allocate a float matrix m[nrl..nrh][ncl..nch] that points to the matrix
declared in the standard C manner as a[nrow][ncol], where nrow=nrh-nrl+1
and ncol=nch-ncl+1. The routine should be called with the address
&a[0][0] as the first argument. */
{
	long i,j,nrow=nrh-nrl+1,ncol=nch-ncl+1;
	float **m;

	/* allocate pointers to rows */
	m=(float **) malloc((size_t) ((nrow+NR_END)*sizeof(float*)));
	if (!m) nrerror("allocation failure in convert_matrix()");
	m += NR_END;
	m -= nrl;

	/* set pointers to rows */
	m[nrl]=a-ncl;
	for(i=1,j=nrl+1;i<nrow;i++,j++) m[j]=m[j-1]+ncol;
	/* return pointer to array of pointers to rows */
	return m;
}

float ***f3tensor(long nrl, long nrh, long ncl, long nch, long ndl, long ndh)
/* allocate a float 3tensor with range t[nrl..nrh][ncl..nch][ndl..ndh] */
{
	long i,j,nrow=nrh-nrl+1,ncol=nch-ncl+1,ndep=ndh-ndl+1;
	float ***t;

	/* allocate pointers to pointers to rows */
	t=(float ***) malloc((size_t)((nrow+NR_END)*sizeof(float**)));
	if (!t) nrerror("allocation failure 1 in f3tensor()");
	t += NR_END;
	t -= nrl;

	/* allocate pointers to rows and set pointers to them */
	t[nrl]=(float **) malloc((size_t)((nrow*ncol+NR_END)*sizeof(float*)));
	if (!t[nrl]) nrerror("allocation failure 2 in f3tensor()");
	t[nrl] += NR_END;
	t[nrl] -= ncl;

	/* allocate rows and set pointers to them */
	t[nrl][ncl]=(float *) malloc((size_t)((nrow*ncol*ndep+NR_END)*sizeof(float)));
	if (!t[nrl][ncl]) nrerror("allocation failure 3 in f3tensor()");
	t[nrl][ncl] += NR_END;
	t[nrl][ncl] -= ndl;

	for(j=ncl+1;j<=nch;j++) t[nrl][j]=t[nrl][j-1]+ndep;
	for(i=nrl+1;i<=nrh;i++) {
		t[i]=t[i-1]+ncol;
		t[i][ncl]=t[i-1][ncl]+ncol*ndep;
		for(j=ncl+1;j<=nch;j++) t[i][j]=t[i][j-1]+ndep;
	}

	/* return pointer to array of pointers to rows */
	return t;
}

void free_vector(float *v, long nl, long /*nh*/)
/* free a float vector allocated with vector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_ivector(int *v, long nl, long /*nh*/)
/* free an int vector allocated with ivector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_cvector(unsigned char *v, long nl, long /*nh*/)
/* free an unsigned char vector allocated with cvector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_lvector(unsigned long *v, long nl, long /*nh*/)
/* free an unsigned long vector allocated with lvector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_dvector(double *v, long nl, long /*nh*/)
/* free a double vector allocated with dvector() */
{
	free((FREE_ARG) (v+nl-NR_END));
}

void free_matrix(float **m, long nrl, long /* nrh */, long ncl, long /* nch */)
/* free a float matrix allocated by matrix() */
{
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}

void free_dmatrix(double **m, long nrl, long /* nrh */, long ncl, long /* nch */)
/* free a double matrix allocated by dmatrix() */
{
  free((FREE_ARG) (m[nrl]+ncl-NR_END));
  free((FREE_ARG) (m+nrl-NR_END));
}

void free_imatrix(int **m, long nrl, long /* nrh */, long ncl, long /* nch */)
/* free an int matrix allocated by imatrix() */
{
	free((FREE_ARG) (m[nrl]+ncl-NR_END));
	free((FREE_ARG) (m+nrl-NR_END));
}

void free_submatrix(float **b, long nrl, long /* nrh */, long /* ncl */, long /* nch */)
/* free a submatrix allocated by submatrix() */
{
	free((FREE_ARG) (b+nrl-NR_END));
}

void free_convert_matrix(float **b, long nrl, long /* nrh */, long /*ncl*/, long /* nch */)
/* free a matrix allocated by convert_matrix() */
{
	free((FREE_ARG) (b+nrl-NR_END));
}

void free_f3tensor(float ***t, long nrl, long /* nrh */, long ncl, long /* nch */,
	long ndl, long /* ndh */)
/* free a float f3tensor allocated by f3tensor() */
{
	free((FREE_ARG) (t[nrl][ncl]+ndl-NR_END));
	free((FREE_ARG) (t[nrl]+ncl-NR_END));
	free((FREE_ARG) (t+nrl-NR_END));
}

// ------------------ Added Utilities  ------------------------------
// Copy and Symmetric Operations
// ------------------------------------------------------------------

void copyvector(float *a,float *b,int rows)
{
  for (int i=1;i<=rows;i++)
    a[i]=b[i];
}

void copydvector(double *a,double *b,int rows)

{
  for (int i=1;i<=rows;i++)
    a[i]=b[i];
}

void copyivector(int *a,int *b,int rows)

{
  for (int i=1;i<=rows;i++)
    a[i]=b[i];
}


// ------------------ Matrices -----------------------------
void symmetric(float **a, int n)
// Assume top half including leading diagonal filled
{
for (int i=2; i<=n;i++)
  for (int j=1; j<i; j++)
    a[i][j]=a[j][i];
}

void copymatrix(float **a,float **b,int rows,int columns)

{
for (int i=1;i<=rows;i++)
  for (int j=1;j<=columns;j++)
    a[i][j]=b[i][j];
}

void dsymmetric(double **a, int n)
// Assume top half including leading diagonal filled
{
for (int i=2; i<=n;i++)
  for (int j=1; j<i; j++)
    a[i][j]=a[j][i];
}

void copydmatrix(double **a,double **b,int rows,int columns)

{
for (int i=1;i<=rows;i++)
  for (int j=1;j<=columns;j++)
    a[i][j]=b[i][j];
}



void isymmetric(int **a, int n)
// Assume top half including leading diagonal filled
{
for (int i=2; i<=n;i++)
  for (int j=1; j<i; j++)
    a[i][j]=a[j][i];
}

void copyimatrix(int **a,int **b,int rows,int columns)

{
for (int i=1;i<=rows;i++)
  for (int j=1;j<=columns;j++)
    a[i][j]=b[i][j];
}


// -----------------------------------------------------------------
//           Print Operations
// -----------------------------------------------------------------

void printivector(int* v,int n,const char* fmt,const char* name,int wd)
{
  printdvector((double*)v,n,fmt,name,wd);  
}

void printvector(float* v,int n,const char* fmt,const char* name,int wd)
{
  printdvector((double*)v,n,fmt,name,wd);  
}

void printdvector(double* v,int n,const char* fmt,const char* name,int wd)
{
  char ll[200];
  int cnt=0;
  
  fprintf(stdout,"%s = [ ",name);
  //  printf(" =  [ ");
  for (int i=1;i<=n;i++)
    {
      if (fabs(v[i])<1e-10) v[i]=0.0;

      if (fabs(v[i])<1e+6)
	sprintf(ll,fmt,v[i]);
      else
	sprintf(ll," ###### ");

      fprintf(stdout,"%s",ll);
      cnt++;
      if (cnt==wd) 
	{
	  fprintf(stdout,"\n\t\t");
	  cnt=0;
	}
    }
  fprintf(stdout," ]\n");
}
// -----------------------------------------------------------------

void printmatrix(float** a,int row,int col,const char* fmt,const char* name,int wd,int off)
{
  printdmatrix((double**)a,row,col,fmt,name,wd,off);  
}

void printimatrix(int** a,int row,int col,const char* fmt,const char* name,int wd,int off)
{
  printdmatrix((double**)a,row,col,fmt,name,wd,off);  
}

void printdmatrix(double** a,int row,int col,const char* fmt,const char* name,int wd,int off)
{
  //  char ll[20];
  int cnt=0;
  
  for (int j=off;j<row+off;j++)
    {
      if (j==off) 
	  fprintf(stderr,"%s = ",name);
      else
	  for(int k=1;k<=(int)strlen(name)+3;k++)
	      fprintf(stderr," ");
      fprintf(stderr," [ ");
      for (int i=off;i<col+off;i++)
	{
	  //	  sprintf(ll,
	  fprintf(stderr,fmt,a[j][i]);
	  cnt++;
	  if (cnt==wd) 
	    {
	      fprintf(stderr,"\n\t\t");
	      cnt=0;
	    }
	}
      cnt=0;
      fprintf(stderr," ]\n");
    }
  fprintf(stderr,"\n");
}
// -------------------------------------------------------------
void shiftdvector(double* v,int shift,int np)
{

  if (shift==0)
      return;
  
  if (shift>0)
      {
	for (int j=1;j<=shift;j++)
	    {
	      float vnp=v[np];
	      for (int i=np;i>1;i=i-1)
		  v[i]=v[i-1];
	      v[1]=vnp;
	    }
      }
  if (shift<0)
      {
	for (int j=1;j<=abs(shift);j++)
	    {
	      float v1=v[1];
	      for (int i=1;i<np;i++)
		  v[i]=v[i+1];
	      v[np]=v1;
	    }
      }
}
// -------------------------------------------------------------
void shiftvector(float* v,int shift,int np)
{

  if (shift==0)
      return;
  
  if (shift>0)
      {
	for (int j=1;j<=shift;j++)
	    {
	      float vnp=v[np];
	      for (int i=np;i>1;i=i-1)
		  v[i]=v[i-1];
	      v[1]=vnp;
	    }
      }
  if (shift<0)
      {
	for (int j=1;j<=abs(shift);j++)
	    {
	      float v1=v[1];
	      for (int i=1;i<np;i++)
		  v[i]=v[i+1];
	      v[np]=v1;
	    }
      }
}
// -------------------------------------------------------------
void shiftivector(int* v,int shift,int np)
{

  if (shift==0)
      return;
  
  if (shift>0)
      {
	for (int j=1;j<=shift;j++)
	    {
	      int vnp=v[np];
	      for (int i=np;i>1;i=i-1)
		  v[i]=v[i-1];
	      v[1]=vnp;
	    }
      }
  if (shift<0)
      {
	for (int j=1;j<=abs(shift);j++)
	    {
	      int v1=v[1];
	      for (int i=1;i<np;i++)
		  v[i]=v[i+1];
	      v[np]=v1;
	    }
      }
}
// -------------------------------------------------------------


#define VTK_J_ROTATE(a,i,j,k,l) g=a[i][j];h=a[k][l];a[i][j]=g-s*(h+g*tau);\
        a[k][l]=h+s*(g-h*tau)

#define VTK_J_MAX_ROTATIONS 20

//#undef VTK_J_MAX_ROTATIONS

//#define VTK_J_MAX_ROTATIONS 50

// Jacobi iteration for the solution of eigenvectors/eigenvalues of a nxn
// real symmetric matrix. Square nxn matrix a; size of matrix in n;
// output eigenvalues in w; and output eigenvectors in v. Resulting
// eigenvalues/vectors are sorted in decreasing order; eigenvectors are
// normalized.
template<class T>
static inline int vtk_JacobiN(T **a, int n, T *w, T **v)
{
  int i, j, k, iq, ip, numPos;
  T tresh, theta, tau, t, sm, s, h, g, c, tmp;
  T bspace[4], zspace[4];
  T *b = bspace;
  T *z = zspace;

  // only allocate memory if the matrix is large
  if (n > 4)
    {
    b = new T[n];
    z = new T[n]; 
    }

  // initialize
  for (ip=0; ip<n; ip++) 
    {
    for (iq=0; iq<n; iq++)
      {
      v[ip][iq] = 0.0;
      }
    v[ip][ip] = 1.0;
    }
  for (ip=0; ip<n; ip++) 
    {
    b[ip] = w[ip] = a[ip][ip];
    z[ip] = 0.0;
    }

  // begin rotation sequence
  for (i=0; i<VTK_J_MAX_ROTATIONS; i++) 
    {
    sm = 0.0;
    for (ip=0; ip<n-1; ip++) 
      {
      for (iq=ip+1; iq<n; iq++)
        {
        sm += fabs(a[ip][iq]);
        }
      }
    if (sm == 0.0)
      {
      break;
      }

    if (i < 3)                                // first 3 sweeps
      {
      tresh = 0.2*sm/(n*n);
      }
    else
      {
      tresh = 0.0;
      }

    for (ip=0; ip<n-1; ip++) 
      {
      for (iq=ip+1; iq<n; iq++) 
        {
        g = 100.0*fabs(a[ip][iq]);

        // after 4 sweeps
        if (i > 3 && (fabs(w[ip])+g) == fabs(w[ip])
        && (fabs(w[iq])+g) == fabs(w[iq]))
          {
          a[ip][iq] = 0.0;
          }
        else if (fabs(a[ip][iq]) > tresh) 
          {
          h = w[iq] - w[ip];
          if ( (fabs(h)+g) == fabs(h))
            {
            t = (a[ip][iq]) / h;
            }
          else 
            {
            theta = 0.5*h / (a[ip][iq]);
            t = 1.0 / (fabs(theta)+sqrt(1.0+theta*theta));
            if (theta < 0.0)
              {
              t = -t;
              }
            }
          c = 1.0 / sqrt(1+t*t);
          s = t*c;
          tau = s/(1.0+c);
          h = t*a[ip][iq];
          z[ip] -= h;
          z[iq] += h;
          w[ip] -= h;
          w[iq] += h;
          a[ip][iq]=0.0;

          // ip already shifted left by 1 unit
          for (j = 0;j <= ip-1;j++) 
            {
            VTK_J_ROTATE(a,j,ip,j,iq);
            }
          // ip and iq already shifted left by 1 unit
          for (j = ip+1;j <= iq-1;j++) 
            {
            VTK_J_ROTATE(a,ip,j,j,iq);
            }
          // iq already shifted left by 1 unit
          for (j=iq+1; j<n; j++) 
            {
            VTK_J_ROTATE(a,ip,j,iq,j);
            }
          for (j=0; j<n; j++) 
            {
            VTK_J_ROTATE(v,j,ip,j,iq);
            }
          }
        }
      }

    for (ip=0; ip<n; ip++) 
      {
      b[ip] += z[ip];
      w[ip] = b[ip];
      z[ip] = 0.0;
      }
    }

  //// this is NEVER called
  if ( i >= VTK_J_MAX_ROTATIONS )
    {
      fprintf(stderr,"vtkMath_Jacobi: Error extracting eigenfunctions\n");
    return 0;
    }

  // sort eigenfunctions                 these changes do not affect accuracy 
  for (j=0; j<n-1; j++)                  // boundary incorrect
    {
    k = j;
    tmp = w[k];
    for (i=j+1; i<n; i++)                // boundary incorrect, shifted already
      {
      if (w[i] >= tmp)                   // why exchage if same?
        {
        k = i;
        tmp = w[k];
        }
      }
    if (k != j) 
      {
      w[k] = w[j];
      w[j] = tmp;
      for (i=0; i<n; i++) 
        {
        tmp = v[i][j];
        v[i][j] = v[i][k];
        v[i][k] = tmp;
        }
      }
    }
  // insure eigenvector consistency (i.e., Jacobi can compute vectors that
  // are negative of one another (.707,.707,0) and (-.707,-.707,0). This can
  // reek havoc in hyperstreamline/other stuff. We will select the most
  // positive eigenvector.
  int ceil_half_n = (n >> 1) + (n & 1);
  for (j=0; j<n; j++)
    {
    for (numPos=0, i=0; i<n; i++)
      {
      if ( v[i][j] >= 0.0 )
        {
        numPos++;
        }
      }
//    if ( numPos < ceil(double(n)/double(2.0)) )
    if ( numPos < ceil_half_n)
      {
      for(i=0; i<n; i++)
        {
        v[i][j] *= -1.0;
        }
      }
    }

  if (n > 4)
    {
    delete [] b;
    delete [] z;
    }
  return 1;
}

#undef VTK_J_ROTATE
#undef VTK_J_MAX_ROTATIONS

int vtkMath_JacobiN(float **a, int n, float *w, float **v)
{
  return vtk_JacobiN(a,n,w,v);
}

int vtkMath_JacobiN(double **a, int n, double *w, double **v)
{
  return vtk_JacobiN(a,n,w,v);
}
// Jacobi iteration for the solution of eigenvectors/eigenvalues of a 3x3
// real symmetric matrix. Square 3x3 matrix a; output eigenvalues in w;
// and output eigenvectors in v. Resulting eigenvalues/vectors are sorted
// in decreasing order; eigenvectors are normalized.
int vtkMath_Jacobi(float **a, float *w, float **v)
{
  return vtkMath_JacobiN(a, 3, w, v);
}

int vtkMath_Jacobi(double **a, double *w, double **v)
{
  return vtkMath_JacobiN(a, 3, w, v);
}


