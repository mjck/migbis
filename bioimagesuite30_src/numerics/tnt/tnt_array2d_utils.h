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



#ifndef TNT_ARRAY2D_UTILS_H
#define TNT_ARRAY2D_UTILS_H

#include <cstdlib>
#include <cassert>

namespace TNT
{


/**
	Write an array to a character outstream.  Output format is one that can
	be read back in via the in-stream operator: two integers
	denoting the array dimensions (m x n), followed by m
	lines of n  elements.

*/
template <class T>
std::ostream& operator<<(std::ostream &s, const Array2D<T> &A)
{
    int M=A.dim1();
    int N=A.dim2();

    s << M << " " << N << "\n";

    for (int i=0; i<M; i++)
    {
        for (int j=0; j<N; j++)
        {
            s << A[i][j] << " ";
        }
        s << "\n";
    }


    return s;
}

/**
	Read an array from a character stream.  Input format
	is two integers, denoting the dimensions (m x n), followed
	by m*n whitespace-separated elments in "row-major" order
	(i.e. right-most dimension varying fastest.)  Newlines
	are ignored.

	<p>
	Note: the array being read into references new memory
	storage. If the intent is to fill an existing conformant
	array, use <code> cin >> B;  A.inject(B) ); </code>
	instead or read the elements in one-a-time by hand.

	@param s the charater to read from (typically <code>std::in</code>)
	@param A the array to read into.
*/
template <class T>
std::istream& operator>>(std::istream &s, Array2D<T> &A)
{

    int M, N;

    s >> M >> N;

	Array2D<T> B(M,N);

    for (int i=0; i<M; i++)
        for (int j=0; j<N; j++)
        {
            s >>  B[i][j];
        }

	A = B;
    return s;
}


/**
	Matrix Multiply:  compute C = A*B, where C[i][j]
	is the dot-product of row i of A and column j of B.
	

	@param A an (m x n) array
	@param B an (n x k) array
	@return the (m x k) array A*B, or a null array (0x0)
		if the matrices are non-conformant (i.e. the number 
		of columns of A are different than the number of rows of B.)
	

*/
template <class T>
Array2D<T> matmult(const Array2D<T> &A, const Array2D<T> &B)
{
	if (A.dim2() != B.dim1())
		return Array2D<T>();

	int M = A.dim1();
	int N = A.dim2();
	int K = B.dim2();

	Array2D<T> C(M,K);

	for (int i=0; i<M; i++)
		for (int j=0; j<K; j++)
		{
			T sum = 0;

			for (int k=0; k<N; k++)
				sum += A[i][k] * B [k][j];

			C[i][j] = sum;
		}

	return C;

}



} // namespace TNT

#endif

