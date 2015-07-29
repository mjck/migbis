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



#ifndef TNT_FORTRAN_ARRAY2D_UTILS_H
#define TNT_FORTRAN_ARRAY2D_UTILS_H

#include <iostream>

namespace TNT
{


/**
	Write an array to a character outstream.  Output format is one that can
	be read back in via the in-stream operator: two integers
	denoting the array dimensions (m x n), followed by m
	lines of n  elements.

*/
template <class T>
std::ostream& operator<<(std::ostream &s, const Fortran_Array2D<T> &A)
{
    int M=A.dim1();
    int N=A.dim2();

    s << M << " " << N << "\n";

    for (int i=1; i<=M; i++)
    {
        for (int j=1; j<=N; j++)
        {
            s << A(i,j) << " ";
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
std::istream& operator>>(std::istream &s, Fortran_Array2D<T> &A)
{

    int M, N;

    s >> M >> N;

	Fortran_Array2D<T> B(M,N);

    for (int i=1; i<=M; i++)
        for (int j=1; j<=N; j++)
        {
            s >>  B(i,j);
        }

	A = B;
    return s;
}




} // namespace TNT

#endif

