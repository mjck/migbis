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

#ifndef JAMA_CHOLESKY_H
#define JAMA_CHOLESKY_H

#include "math.h"
	/* needed for sqrt() below. */


namespace JAMA
{

using namespace TNT;

/** 
   <P>
   For a symmetric, positive definite matrix A, this function
   computes the Cholesky factorization, i.e. it computes a lower 
   triangular matrix L such that A = L*L'.
   If the matrix is not symmetric or positive definite, the function
   computes only a partial decomposition.  This can be tested with
   the is_spd() flag.

   <p>Typical usage looks like:
   <pre>
	Array2D<double> A(n,n);
	Array2D<double> L;

	 ... 

	Cholesky<double> chol(A);

	if (chol.is_spd())
		L = chol.getL();
		
  	else
		cout << "factorization was not complete.\n";

	</pre>


   <p>
	(Adapted from JAMA, a Java Matrix Library, developed by jointly 
	by the Mathworks and NIST; see  http://math.nist.gov/javanumerics/jama).

   */

template <class Real>
class Cholesky
{
	Array2D<Real> L_;		// lower triangular factor
	int isspd;				// 1 if matrix to be factored was SPD

public:

	Cholesky();
	Cholesky(const Array2D<Real> &A);
	Array2D<Real> getL() const;
	Array1D<Real> solve(const Array1D<Real> &B);
	Array2D<Real> solve(const Array2D<Real> &B);
	int is_spd() const;

};

template <class Real>
Cholesky<Real>::Cholesky() : L_(0,0), isspd(0) {}

/**
	@return 1, if original matrix to be factored was symmetric 
		positive-definite (SPD).
*/
template <class Real>
int Cholesky<Real>::is_spd() const
{
	return isspd;
}

/**
	@return the lower triangular factor, L, such that L*L'=A.
*/
template <class Real>
Array2D<Real> Cholesky<Real>::getL() const
{
	return L_;
}

/**
	Constructs a lower triangular matrix L, such that L*L'= A.
	If A is not symmetric positive-definite (SPD), only a
	partial factorization is performed.  If is_spd()
	evalutate true (1) then the factorizaiton was successful.
*/
template <class Real>
Cholesky<Real>::Cholesky(const Array2D<Real> &A)
{


   	int m = A.dim1();
	int n = A.dim2();
	
	isspd = (m == n);

	if (m != n)
	{
		L_ = Array2D<Real>(0,0);
		return;
	}

	L_ = Array2D<Real>(n,n);


      // Main loop.
     for (int j = 0; j < n; j++) 
	 {
        double d = 0.0;
        for (int k = 0; k < j; k++) 
		{
            Real s = 0.0;
            for (int i = 0; i < k; i++) 
			{
               s += L_[k][i]*L_[j][i];
            }
            L_[j][k] = s = (A[j][k] - s)/L_[k][k];
            d = d + s*s;
            isspd = isspd && (A[k][j] == A[j][k]); 
         }
         d = A[j][j] - d;
         isspd = isspd && (d > 0.0);
         L_[j][j] = sqrt(d > 0.0 ? d : 0.0);
         for (int k = j+1; k < n; k++) 
		 {
            L_[j][k] = 0.0;
         }
	}
}

/**

	Solve a linear system A*x = b, using the previously computed
	cholesky factorization of A: L*L'.

   @param  B   A Matrix with as many rows as A and any number of columns.
   @return     x so that L*L'*x = b.  If b is nonconformat, or if A
   				was not symmetric posidtive definite, a null (0x0)
   						array is returned.
*/
template <class Real>
Array1D<Real> Cholesky<Real>::solve(const Array1D<Real> &b)
{
	int n = L_.dim1();
	if (b.dim1() != n)
		return Array1D<Real>();


	Array1D<Real> x = b.copy();


      // Solve L*y = b;
      for (int k = 0; k < n; k++) 
	  {
         for (int i = 0; i < k; i++) 
               x[k] -= x[i]*L_[k][i];
		 x[k] /= L_[k][k];
		
      }

      // Solve L'*X = Y;
      for (int k = n-1; k >= 0; k--) 
	  {
         for (int i = k+1; i < n; i++) 
               x[k] -= x[i]*L_[i][k];
         x[k] /= L_[k][k];
      }

	return x;
}


/**

	Solve a linear system A*X = B, using the previously computed
	cholesky factorization of A: L*L'.

   @param  B   A Matrix with as many rows as A and any number of columns.
   @return     X so that L*L'*X = B.  If B is nonconformat, or if A
   				was not symmetric posidtive definite, a null (0x0)
   						array is returned.
*/
template <class Real>
Array2D<Real> Cholesky<Real>::solve(const Array2D<Real> &B)
{
	int n = L_.dim1();
	if (B.dim1() != n)
		return Array2D<Real>();


	Array2D<Real> X = B.copy();
	int nx = B.dim2();

// Cleve's original code
#if 0
      // Solve L*Y = B;
      for (int k = 0; k < n; k++) {
         for (int i = k+1; i < n; i++) {
            for (int j = 0; j < nx; j++) {
               X[i][j] -= X[k][j]*L_[k][i];
            }
         }
         for (int j = 0; j < nx; j++) {
            X[k][j] /= L_[k][k];
         }
      }

      // Solve L'*X = Y;
      for (int k = n-1; k >= 0; k--) {
         for (int j = 0; j < nx; j++) {
            X[k][j] /= L_[k][k];
         }
         for (int i = 0; i < k; i++) {
            for (int j = 0; j < nx; j++) {
               X[i][j] -= X[k][j]*L_[k][i];
            }
         }
      }
#endif


      // Solve L*y = b;
  	  for (int j=0; j< nx; j++)
	  {
      	for (int k = 0; k < n; k++) 
		{
			for (int i = 0; i < k; i++) 
               X[k][j] -= X[i][j]*L_[k][i];
		    X[k][j] /= L_[k][k];
		 }
      }

      // Solve L'*X = Y;
     for (int j=0; j<nx; j++)
	 {
      	for (int k = n-1; k >= 0; k--) 
	  	{
         	for (int i = k+1; i < n; i++) 
               X[k][j] -= X[i][j]*L_[i][k];
         	X[k][j] /= L_[k][k];
		}
      }



	return X;
}


}
// namespace JAMA

#endif
// JAMA_CHOLESKY_H

