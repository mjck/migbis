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
//  ludcmp needs to be replaced, so does tridiag, and cyclictridiag
//
//---------------------------------------------------------------------------------------------------------
#include <math.h>
#include <stdio.h>
#include "nr.h"
#include "nrutil.h"
#include <stdlib.h>
#include "bioimagesuite_settings.h"

#define TINY 1.0e-20;

// ---------------------------------------------------------------------
// 1. LU decomposition from Numerical Recipes
// ---------------------------------------------------------------------

void ludcmp(double **a,int n,int* indx,double* d,int& singular)
{
  vtkMath_LUFactorLinearSystem(a,indx,n,singular);
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

void lubksb(double **a,int n,int *indx,double b[])
{
  vtkMath_LUSolveLinearSystem(a,indx,b,n);
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

// This is replacement code from vtkMath .... the names are kept
// Code Changed to reflect 1..N as opposed to 0..N-1 range!

#define VTK_SMALL_NUMBER 1.0e-12

// Factor linear equations Ax = b using LU decompostion A = LU where L is
// lower triangular matrix and U is upper triangular matrix. Input is 
// square matrix A, integer array of pivot indices index[0->n-1], and size
// of square matrix n. Output factorization LU is in matrix A. If error is 
// found, method returns 0. 
void vtkMath_LUFactorLinearSystem(double **A, int *index, int size,int &singular)
{
  double scratch[12];
  double *scale = (size<10 ? scratch : new double[size+1]);
  singular=0;

  int i, j, k;
  int maxI = 0;
  double largest, temp1, temp2, sum;

  //
  // Loop over rows to get implicit scaling information
  //
  for ( i = 1; i <= size; i++ ) 
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
	  singular=1;
	  return;
	}
      scale[i] = 1.0 / largest;
    }
  //
  // Loop over all columns using Crout's method
  //
  for ( j = 1; j <= size; j++ ) 
    {
      for (i = 1; i < j; i++) 
	{
	  sum = A[i][j];
	  for ( k = 1; k < i; k++ )
	    {
	      sum -= A[i][k] * A[k][j];
	    }
	  A[i][j] = sum;
	}
      //
      // Begin search for largest pivot element
      //
      for ( largest = 0.0, i = j; i <= size; i++ ) 
	{
	  sum = A[i][j];
	  for ( k = 1; k < j; k++ )
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
	  for ( k = 1; k <= size; k++ ) 
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
	  singular=1;
	  return;
	}
      
      if ( j != (size) ) 
	{
	  temp1 = 1.0 / A[j][j];
	  for ( i = j + 1; i <= size; i++ )
	    {
	      A[i][j] *= temp1;
	    }
	}
    }
  
  if (size >= 10 ) delete [] scale;
  
  singular=0;
}


// Solve linear equations Ax = b using LU decompostion A = LU where L is
// lower triangular matrix and U is upper triangular matrix. Input is 
// factored matrix A=LU, integer array of pivot indices index[0->n-1],
// load vector x[0->n-1], and size of square matrix n. Note that A=LU and
// index[] are generated from method LUFactorLinearSystem). Also, solution
// vector is written directly over input load vector.
void vtkMath_LUSolveLinearSystem(double **A, int *index,double *x, int size)
{
  int i, j, ii, idx;
  double sum;
  //
  // Proceed with forward and backsubstitution for L and U
  // matrices.  First, forward substitution.
  //
  for ( ii = 0, i = 1; i <= size; i++ ) 
    {
      idx = index[i];
      sum = x[idx];
      x[idx] = x[i];
      
      if ( ii >= 1 )
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
  for ( i = size; i >= 1; i-- ) 
    {
      sum = x[i];
      for ( j = i + 1; j <= size; j++ )
	{
	  sum -= A[i][j]*x[j];
	}
      x[i] = sum / A[i][i];
    }
}
#undef VTK_SMALL_NUMBER 
// ---------------------------------------------------------------------
// 2. SOR -- code written for ME340 class
// ---------------------------------------------------------------------

int sor(double **a,int n, double *b,double *oldx,
	 double thr,double omega)
     
{
  //     matrix a is nxn  vector b is nx1
  const int MAXITER=100;
  int maxiter=MAXITER*n;
  int row,col,iter=0;
  double moddif,modx,res;
  double *newx,r;
  newx=dvector(1,n);
  res=thr+1.0;
  
  while ( res>thr && iter < maxiter) 
    {
      iter++;
      for (row=1;row<=n;row++)
	{
	  r=0.0;
	  
	  //     form gaus-seidel best estimate;
	  //     for elements before leading diagonal;
	  for (col=1;col<row;col++)
            r=r+a[row][col]*newx[col];

	  //     for elements after leading diagonal;

	  for (col=row+1;col<=n;col++)
            r=r+a[row][col]*oldx[col];

	  //     form sor (gaus seidel omega=1);

	  newx[row]=omega*((b[row]-r)/a[row][row]);
	  newx[row]=newx[row]+(1-omega)*oldx[row];
	}

      //     calculate ||x(k+1)-x(k)||(inf) moddif and ||x(k)||(inf)=modx;
      //     also do oldx=x to prepare for next iteration;
      
      moddif=0.0;
      modx=0.0;
      
      for (row=1;row<=n;row ++)
	{
	  if (fabs(oldx[row]-newx[row]) > moddif) 
	    moddif=fabs(oldx[row]-newx[row]);
	  if (fabs(newx[row]) > modx) 
	    modx=fabs(newx[row]);
	  oldx[row]=newx[row];
	}
      res=moddif/modx;

    }

  free_dvector(newx,1,n);
  return iter;
}

// ---------------------------------------------------------------------
// 3. Conjugate Gradient Descent -- code written for ME340 class
// ---------------------------------------------------------------------
int conjugate(double** a,int n,double* b,double* oldx,double thr)

{

  //     matrix a is nxn  vector b is nx1

  double moddif,modx,res,betak,rkrk,pkapk,alphak;
  double *newx=dvector(1,n);
  double *rk=dvector(1,n);
  double *rkm2=dvector(1,n);
  double *rkm1=dvector(1,n);
  double *pk=dvector(1,n);
  double *apk=dvector(1,n);
  double *rold=dvector(1,n);
  
  int i,j,iter=0;

  for (i=1;i<=n;i++)
    rold[i]=b[i];
  
  res=thr+1.0;

  while (res>thr && iter<=n) 
    {
      iter++;
      if (iter==1)
	{
	  rkrk=0;
	  for (i=1;i<=n;i++)
	    {
	      pk[i]=rold[i];
	      rkrk=rkrk+rold[i]*rold[i];
	      rkm1[i]=rold[i];
	    }
	}
      else
	{
	  rkrk=0;
	  betak=0;
	  for (i=1;i<=n;i++)
	    {
	      rkrk =rkrk +rkm1[i]*rkm1[i];
	      betak=betak+rkm2[i]*rkm2[i];
	    }
	  betak=rkrk/betak;
	  for (i=1;i<=n;i++)
	    pk[i]=rkm1[i]+betak*pk[i];
	}
      
      //     form a*pk and pk*a*pk;
      for (i=1;i<=n;i++)
	{
	  apk[i]=0;
	  for (j=1;j<=n;j++)
	    apk[i]=apk[i]+a[i][j]*pk[j];
	}  
      
      pkapk=0;
      for (i=1;i<=n;i++)
	pkapk=pkapk+pk[i]*apk[i];
      
      alphak=rkrk/pkapk;
      
      for(i=1;i<=n;i++)
	{
	  newx[i]=oldx[i]+alphak*pk[i];
	  rk[i]=rkm1[i]-alphak*apk[i];
	}
      
      modx=0;
      moddif=0;
      
      
      for(i=1;i<=n;i++)
	{
	  if (fabs(oldx[i]-newx[i]) >moddif) 
	    moddif=fabs(oldx[i]-newx[i]);
	  if (fabs(newx[i]) > modx) 
	    modx=fabs(newx[i]);
	}
      
      //     print*, 'moddif ',moddif ,'modx', modx;
      res=moddif/modx;
      
      for (i=1;i<=n;i++)
	{
	  rkm2[i]=rkm1[i];
	  rkm1[i]=rk[i];
	  oldx[i]=newx[i];
	}
    }
  free_dvector(newx,1,n);
  free_dvector(rk,1,n);
  free_dvector(rkm2,1,n);
  free_dvector(rkm1,1,n);
  free_dvector(pk,1,n);
  free_dvector(apk,1,n);
  free_dvector(rold,1,n);
  return iter;
}


// ---------------------------------------------------------------------
// 4. Pre-Conjugate Gradient Descent -- code written for ME340 class
// ---------------------------------------------------------------------
int precondconjugate(double** a,int n,double* b,double* oldx,double thr)

{

  //     matrix a is nxn  vector b is nx1

  double *newx=dvector(1,n);
  double *rkm2=dvector(1,n);
  double *rkm1=dvector(1,n);
  double *pk=dvector(1,n);
  double *zkm1=dvector(1,n);
  double *zkm2=dvector(1,n);
  double *rk=dvector(1,n);
  double *apk=dvector(1,n);

  double  moddif,modx,res=0.0,betak=0.0,rkzk=0.0,pkapk=0.0,alphak=0.0;
  int i,j,iter=0;

  for (i=1;i<=n;i++)
    {
      rkm1[i]=b[i];
      zkm1[i]=rkm1[i]/a[i][i];
      pk[i]=0.0;
    }
  res=thr+1;

  while(res>thr && iter<=n)
    {
      iter++;
      if (iter==1) 
	{
	  rkzk=0;
	  for (i=1;i<=n;i++)
	    {
	      pk[i]=zkm1[i];
	      rkzk =rkzk+rkm1[i]*zkm1[i];
	    }
	}
      else
	{
	  rkzk=0;
	  betak=0;
	  for (i=1;i<=n;i++)
	    {
	      rkzk =rkzk +rkm1[i]*zkm1[i];
	      betak=betak+rkm2[i]*zkm2[i];
	    }
	  betak=rkzk/betak;

	  for (i=1;i<=n;i++)
            pk[i]=zkm1[i]+betak*pk[i];
	}
	
      //     form a*pk and pk*a*pk;
      
      pkapk=0;
      for (i=1;i<=n;i++)
	{
	  apk[i]=0;
	  for (j=1;j<=n;j++)
	    {
	      apk[i]=apk[i]+a[i][j]*pk[j];
	    }
	  pkapk=pkapk+pk[i]*apk[i];
	}
      
      alphak=rkzk/pkapk;
      
      for (i=1;i<=n;i++)
	{
	  newx[i]=oldx[i]+alphak*pk[i];
	  rk[i]=rkm1[i]-alphak*apk[i];
	}
      
      modx=0;      moddif=0;
      
      for (i=1;i<=n;i++)
	{
	  
	  if (fabs(oldx[i]-newx[i]) > moddif) 
	    moddif=fabs(oldx[i]-newx[i]);
	  if (fabs(newx[i]) > modx)
	    modx=fabs(newx[i]);
	}
      
      res=moddif/modx;
      
      for (i=1;i<=n;i++)
	{
	  zkm2[i]=zkm1[i];
	  rkm2[i]=rkm1[i];
	  rkm1[i]=rk[i];
	  zkm1[i]=rkm1[i]/a[i][i];
	  oldx[i]=newx[i];
	}
    }

   free_dvector(newx,1,n);
   free_dvector(rkm2,1,n);
   free_dvector(rkm1,1,n);
   free_dvector(pk,1,n);
   free_dvector(zkm1,1,n);
   free_dvector(zkm2,1,n);
   free_dvector(rk,1,n);
   free_dvector(apk,1,n);

  return iter;
}

// ------------------------------------------------------------
//  [5]	Routine from Numerical Recipes for solving 3-diag sys  
// ------------------------------------------------------------
// These should be replaced by code from the GSL, in particular
// tridiag.c

/*
void tridag(float a[], float b[], float c[], float r[], float u[],
	    long n)
{
#ifdef BIS_USE_NR
  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

#endif
}

// -------------------------------------------------------------------------
//  [5b]	Routine from Numerical Recipes for cyclic solving 3-diag sys  
// -------------------------------------------------------------------------

void cyclictridiag(float a[],float b[],float c[], 
		   float alpha, float beta, 
		   float r[],float x[], long n)
{
#ifdef BIS_USE_NR
  //This code derives from Numerical Recipes in C
		//BEGIN OMITTED
		// Some lines of  code have been removed as they
		// derive/depend on the presence of code from the book Numerical Recipes in C
		// which can not be distributed in source form.
		// We have REPLACED the missing code with an alternative version
		// ... see below. The missing code is not mostly OBSOLETE.
		//END OMITTED

#else
  cyclictridiag_gsl(a,b,c,alpha,beta,r,x,n);
#endif
}

void cyclictridiag_gsl(float a[],float b[],float c[], 
		       float alpha, float beta, 
		       float r[],float x[], long n)
{
  // Reroute to GSL ...
  // a = belowdiag
  // b = diag
  // c = above diag 

  // belowdiag counting is different !!!!
  float* anew=new float[n+1];

  for (int i=1;i<n;i++)
    anew[i]=a[i+1];
  anew[n]=a[1];

  solve_cyc_tridiag_nonsym_gsl(b,c,anew,r,x,n);
  delete [] anew;
}
*/

/*
The following code is edited from below

 * 
 * Copyright (C) 1996, 1997, 1998, 1999, 2000, 2002, 2004 Gerard Jungman,
 * Brian Gough, David Necas
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */


/* solve following system w/o the corner elements and then use
 * Sherman-Morrison formula to compensate for them
 *
 *        diag[0]  abovediag[0]             0   .....  belowdiag[N-1]
 *   belowdiag[0]       diag[1]  abovediag[1]   .....
 *              0  belowdiag[1]       diag[2]
 *              0             0  belowdiag[2]   .....
 *            ...           ...
 * abovediag[N-1]           ...
 */
int solve_cyc_tridiag_nonsym_gsl( float in_diag[],
				  float in_abovediag[], 
				  float in_belowdiag[], 
				  float in_rhs[], 
				 float in_x[], 
				 int    N)
{
  int status;
  float *alpha = (float *) malloc (N * sizeof (float));
  float *zb = (float *) malloc (N * sizeof (float));
  float *zu = (float *) malloc (N * sizeof (float));
  float *w = (float *) malloc (N * sizeof (float));

  int a_stride=1, d_stride=1, b_stride=1, r_stride=1, x_stride=1;

  // Next Step is to move stuff by 1 to use 0-offset, remember input is 1-offset
  //
  float* diag=&in_diag[0]; diag++;
  float* abovediag=&in_abovediag[0]; abovediag++;
  float* belowdiag=&in_belowdiag[0]; belowdiag++;
  float* rhs=&in_rhs[0]; rhs++;
  float* x=&in_x[0];  x++;
  
  /*  fprintf(stderr,"\n bg \t dg \t ad \t rh \t x \n");
  for (int i=0;i<N;i++)
    fprintf(stderr,"%.2f \t %.2f \t %.2f \t %.2f \t %.2f\n",
	    belowdiag[i],diag[i],abovediag[i],rhs[i],x[i]);
	    fprintf(stderr,"\n\n");*/
  

  float beta;

  if (alpha == 0 || zb == 0 || zu == 0 || w == 0)
    {
      status = 0;
    }
  else
    {
      /* Bidiagonalization (eliminating belowdiag)
         & rhs update
         diag' = alpha
         rhs' = zb
         rhs' for Aq=u is zu
       */
      zb[0] = rhs[0];
      if (diag[0] != 0) beta = -diag[0]; else beta = 1;
      {
        const float q = 1 - abovediag[0]*belowdiag[0]/(diag[0]*diag[d_stride]);
        if (fabs(q/beta) > 0.5 && fabs(q/beta) < 2) {
          beta *= (fabs(q/beta) < 1) ? 0.5 : 2;
        }
      }
      zu[0] = beta;
      alpha[0] = diag[0] - beta;


      { 
        size_t i;
        for (i = 1; i+1 < N; i++)
        {
          const float t = belowdiag[b_stride*(i - 1)]/alpha[i-1];
          alpha[i] = diag[d_stride*i] - t*abovediag[a_stride*(i - 1)];
          zb[i] = rhs[r_stride*i] - t*zb[i-1];
          zu[i] = -t*zu[i-1];
          /* FIXME!!! */
          if (alpha[i] == 0) {
            status = 0;
            goto solve_cyc_tridiag_nonsym_END;
          }
        }
      }

      {
        const size_t i = N-1;
        const float t = belowdiag[b_stride*(i - 1)]/alpha[i-1];
        alpha[i] = diag[d_stride*i]
                   - abovediag[a_stride*i]*belowdiag[b_stride*i]/beta
                   - t*abovediag[a_stride*(i - 1)];
        zb[i] = rhs[r_stride*i] - t*zb[i-1];
        zu[i] = abovediag[a_stride*i] - t*zu[i-1];
        /* FIXME!!! */
        if (alpha[i] == 0) {
          status = 0;
          goto solve_cyc_tridiag_nonsym_END;
        }
      }


      /* backsubstitution */
      {
        size_t i, j;
        w[N-1] = zu[N-1]/alpha[N-1];
        x[N-1] = zb[N-1]/alpha[N-1];
        for (i = N - 2, j = 0; j <= N - 2; j++, i--)
          {
            w[i] = (zu[i] - abovediag[a_stride*i] * w[i+1])/alpha[i];
            x[i*x_stride] = (zb[i] - abovediag[a_stride*i] * x[x_stride*(i + 1)])/alpha[i];
          }
      }
      
      /* Sherman-Morrison */
      {
        const float vw = w[0] + belowdiag[b_stride*(N - 1)]/beta * w[N-1];
        const float vx = x[0] + belowdiag[b_stride*(N - 1)]/beta * x[x_stride*(N - 1)];
        /* FIXME!!! */
        if (vw + 1 == 0) {
          status = 0;
          goto solve_cyc_tridiag_nonsym_END;
        }

        {
          size_t i;
          for (i = 0; i < N; i++)
            x[i] -= vx/(1 + vw)*w[i];
        }
      }

      status = 1;
    }

solve_cyc_tridiag_nonsym_END:
  if (zb != 0)
    free (zb);
  if (zu != 0)
    free (zu);
  if (w != 0)
    free (w);
  if (alpha != 0)
    free (alpha);

  return status;
}

#undef TINY

