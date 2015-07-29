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
//  Most of this file needs to be replaced!
//---------------------------------------------------------------------------------------------------------
/*
	This file contains the useful functions which manipulate the
	various matrix operation.

	Pengcheng Shi
	April, 1993(revised)
*/

#include "math.h"
#include "nrutil.h"
#include <stdio.h>




/* type definition */
typedef unsigned char uc;

/* structure definition */
typedef struct {              
      	double x;          	/* direction   */
      	double y;
}Vector_2d;

typedef struct {              
      	double x;          	/* direction   */
      	double y;
	double z;
}Vector_3d;


typedef struct {              
      	double x;          	/* position   */
      	double y;
	double z;
	double v;		/* grey value */
}Image_3d;

typedef struct {              
      	double x;          	/* position   */
      	double y;
}Node_2d;

typedef struct {              
      	int node1; 
      	int node2;
	int node3;
}Element_2d;

typedef struct {              
      	int node1; 
      	int node2;
	int node3;
	int node4;
}Element_3d;

typedef struct {              
      	int node1; 
      	int node2;
	int node3;
	int node4;
      	int node5; 
      	int node6;
	int node7;
	int node8;	
}Element_Cube;

typedef struct {              
      	double xx;          	
      	double yy;
	double zz;
	double xy;
      	double yz;
	double zx;		
}Strain_3d;

typedef struct {              
      	double p1;          	
      	double p2;
	double p3;
	double p1x;
      	double p1y;
	double p1z;
	double p2x;
      	double p2y;
	double p2z;
	double p3x;
      	double p3y;
	double p3z;		
}Principal_Strain_3d;


typedef struct {              
      	int link12; 
      	int link13;
	int link14;
      	int link23; 
      	int link24;
	int link34;	
}Type_3d;


typedef struct {		
	double x;		/* position 		*/
	double y;
	double z;
	double e1;
	double e2;
	double e3;
	double nx;		/* principal axis	*/
	double ny;
	double nz;
	double t1x;
	double t1y;
	double t1z;
	double t2x;
	double t2y;
	double t2z;
	double residual;
}fit_eigens;


// ---------------------------------------------------------
// Code from vtkMath to eliminate some linking dependencies
// ---------------------------------------------------------
#define VTK_SMALL_NUMBER 1.0e-12


// Factor linear equations Ax = b using LU decompostion A = LU where L is
// lower triangular matrix and U is upper triangular matrix. Input is 
// square matrix A, integer array of pivot indices index[0->n-1], and size
// of square matrix n. Output factorization LU is in matrix A. If error is 
// found, method returns 0.
//------------------------------------------------------------------
// For thread safe, temporary memory array tmpSize of length size
// must be passed in.
int vtkMath_pc_LUFactorLinearSystem(double **A, int *index, int size,
                                  double *tmpSize)
{
  int i, j, k;
  int maxI = 0;
  double largest, temp1, temp2, sum;

  //
  // Loop over rows to get implicit scaling information
  //
  for ( i = 0; i < size; i++ ) 
    {
    for ( largest = 0.0, j = 0; j < size; j++ ) 
      {
      if ( (temp2 = fabs(A[i][j])) > largest )
        {
        largest = temp2;
        }
      }

    if ( largest == 0.0 )
      {
      return 0;
      }
      tmpSize[i] = 1.0 / largest;
    }
  //
  // Loop over all columns using Crout's method
  //
  for ( j = 0; j < size; j++ ) 
    {
    for (i = 0; i < j; i++) 
      {
      sum = A[i][j];
      for ( k = 0; k < i; k++ )
        {
        sum -= A[i][k] * A[k][j];
        }
      A[i][j] = sum;
      }
    //
    // Begin search for largest pivot element
    //
    for ( largest = 0.0, i = j; i < size; i++ ) 
      {
      sum = A[i][j];
      for ( k = 0; k < j; k++ )
        {
        sum -= A[i][k] * A[k][j];
        }
      A[i][j] = sum;

      if ( (temp1 = tmpSize[i]*fabs(sum)) >= largest ) 
        {
        largest = temp1;
        maxI = i;
        }
      }
    //
    // Check for row interchange
    //
    if ( j != maxI ) 
      {
      for ( k = 0; k < size; k++ ) 
        {
        temp1 = A[maxI][k];
        A[maxI][k] = A[j][k];
        A[j][k] = temp1;
        }
      tmpSize[maxI] = tmpSize[j];
      }
    //
    // Divide by pivot element and perform elimination
    //
    index[j] = maxI;

    if ( fabs(A[j][j]) <= VTK_SMALL_NUMBER )
      {
      return 0;
      }

    if ( j != (size-1) ) 
      {
      temp1 = 1.0 / A[j][j];
      for ( i = j + 1; i < size; i++ )
        {
        A[i][j] *= temp1;
        }
      }
    }

  return 1;
}



int vtkMath_pc_LUFactorLinearSystem(double **A, int *index, int size)
{
  double scratch[10];
  double *scale = (size<10 ? scratch : new double[size]);

  int i, j, k;
  int maxI = 0;
  double largest, temp1, temp2, sum;

  //
  // Loop over rows to get implicit scaling information
  //
  for ( i = 0; i < size; i++ ) 
    {
    for ( largest = 0.0, j = 0; j < size; j++ ) 
      {
      if ( (temp2 = fabs(A[i][j])) > largest )
        {
        largest = temp2;
        }
      }

    if ( largest == 0.0 )
      {
      return 0;
      }
      scale[i] = 1.0 / largest;
    }
  //
  // Loop over all columns using Crout's method
  //
  for ( j = 0; j < size; j++ ) 
    {
    for (i = 0; i < j; i++) 
      {
      sum = A[i][j];
      for ( k = 0; k < i; k++ )
        {
        sum -= A[i][k] * A[k][j];
        }
      A[i][j] = sum;
      }
    //
    // Begin search for largest pivot element
    //
    for ( largest = 0.0, i = j; i < size; i++ ) 
      {
      sum = A[i][j];
      for ( k = 0; k < j; k++ )
        {
        sum -= A[i][k] * A[k][j];
        }
      A[i][j] = sum;

      if ( (temp1 = scale[i]*fabs(sum)) >= largest ) 
        {
        largest = temp1;
        maxI = i;
        }
      }
    //
    // Check for row interchange
    //
    if ( j != maxI ) 
      {
      for ( k = 0; k < size; k++ ) 
        {
        temp1 = A[maxI][k];
        A[maxI][k] = A[j][k];
        A[j][k] = temp1;
        }
      scale[maxI] = scale[j];
      }
    //
    // Divide by pivot element and perform elimination
    //
    index[j] = maxI;

    if ( fabs(A[j][j]) <= VTK_SMALL_NUMBER )
      {
      return 0;
      }

    if ( j != (size-1) ) 
      {
      temp1 = 1.0 / A[j][j];
      for ( i = j + 1; i < size; i++ )
        {
        A[i][j] *= temp1;
        }
      }
    }

  if (size >= 10 ) delete [] scale;

  return 1;
}


// Solve linear equations Ax = b using LU decompostion A = LU where L is
// lower triangular matrix and U is upper triangular matrix. Input is 
// factored matrix A=LU, integer array of pivot indices index[0->n-1],
// load vector x[0->n-1], and size of square matrix n. Note that A=LU and
// index[] are generated from method LUFactorLinearSystem). Also, solution
// vector is written directly over input load vector.
void vtkMath_pc_LUSolveLinearSystem(double **A, int *index, 
                                  double *x, int size)
{
  int i, j, ii, idx;
  double sum;
//
// Proceed with forward and backsubstitution for L and U
// matrices.  First, forward substitution.
//
  for ( ii = -1, i = 0; i < size; i++ ) 
    {
    idx = index[i];
    sum = x[idx];
    x[idx] = x[i];

    if ( ii >= 0 )
      {
      for ( j = ii; j <= (i-1); j++ )
        {
        sum -= A[i][j]*x[j];
        }
      }
    else if (sum)
      {
      ii = i;
      }

    x[i] = sum;
  }
//
// Now, back substitution
//
  for ( i = size-1; i >= 0; i-- ) 
    {
    sum = x[i];
    for ( j = i + 1; j < size; j++ )
      {
      sum -= A[i][j]*x[j];
      }
    x[i] = sum / A[i][i];
    }
}

// Invert input square matrix A into matrix AI. Note that A is modified during
// the inversion. The size variable is the dimension of the matrix. Returns 0
// if inverse not computed.
// -----------------------
// For thread safe behavior, temporary arrays tmp1SIze and tmp2Size
// of length size must be passsed in.
int vtkMath_pc_InvertMatrix(double **A, double **AI, int size,
                          int *tmp1Size, double *tmp2Size)
{
  int i, j;

  //
  // Factor matrix; then begin solving for inverse one column at a time.
  // Note: tmp1Size returned value is used later, tmp2Size is just working
  // memory whose values are not used in LUSolveLinearSystem
  //
  if ( vtkMath_pc_LUFactorLinearSystem(A, tmp1Size, size, tmp2Size) == 0 )
    {
    return 0;
    }
  
  for ( j=0; j < size; j++ )
    {
    for ( i=0; i < size; i++ )
      {
      tmp2Size[i] = 0.0;
      }
    tmp2Size[j] = 1.0;

    vtkMath_pc_LUSolveLinearSystem(A,tmp1Size,tmp2Size,size);

    for ( i=0; i < size; i++ )
      {
      AI[i][j] = tmp2Size[i];
      }
    }

  return 1;
}


// Invert input square matrix A into matrix AI. Note that A is modified during
// the inversion. The size variable is the dimension of the matrix. Returns 0
// if inverse not computed.
int vtkMath_pc_InvertMatrix(double **A, double **AI, int size)
{
  int *index=NULL, iScratch[10];
  double *column=NULL, dScratch[10];

  // Check on allocation of working vectors
  //
  if ( size < 10 ) 
    {
    index = iScratch;
    column = dScratch;
    } 

  int retVal = vtkMath_pc_InvertMatrix(A, AI, size, index, column);

  if ( size >= 10 ) 
    {
    delete [] index;
    delete [] column;
    } 
  
  return retVal;
}




#undef VTK_SMALL_NUMBER


/* given a matrix a[rowsize][colsize], get   */
/* its transpose matrix  b[colsize][rowsize] */
void matrix_transpose(double** a,double** b, int rowsize,int colsize)
{
  for(int i=0; i<rowsize; i++)
    for(int j=0; j<colsize; j++)
      b[j][i] = a[i][j];
}


/* given two matrices a[rowsize_a][colsize_a] and b[rowsize_b][colsize_b], */
/* get the product matrix p[rowsize_a][colsize_b]			   */
void matrix_product(double** p, double** a, int rowsize_a, int colsize_a, 
		    double** b,int /*rowsize_b*/,int colsize_b)
{
  for(int i=0; i<rowsize_a; i++)
    for(int j=0; j<colsize_b; j++)
      {
	p[i][j]=0.0;
	for(int m=0; m<colsize_a; m++)
	  p[i][j] += a[i][m] * b[m][j];
      }
}


// --------------------------------------------------------------------------------
//
//                  End of VTK Derived Code Here
//                
// --------------------------------------------------------------------------------



/* given matrix a[n][n], get the inverse matrix b[n][n] */

void matrix_inverse(double** a,double** b, int n)
{
  vtkMath_pc_InvertMatrix(a,b,n);
  return;

  //The following code is commented out, 
  //It is left there for Legacy purposes only

  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

}





void ludcmp0(double **a,int n,int* indx,double* d,int& singular)
{

  singular=vtkMath_pc_LUFactorLinearSystem(a,indx,n);
  return;


  //The following code is commented out, 
  //It is left there for Legacy purposes only

  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

}

void lubksb0(double **a,int n,int *indx,double b[])
{
  vtkMath_pc_LUSolveLinearSystem(a,indx,b,n);
  return;
  //The following code is commented out, 
  //It is left there for Legacy purposes only

  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

}



