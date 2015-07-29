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

/*
*
* Template Numerical Toolkit (TNT)
*
* Mathematical and Computational Sciences Division
* National Institute of Technology,
* Gaithersburg, MD USA
*
*
* This software was developed at the National Institute of Standards and
* Technology (NIST) by employees of the Federal Government in the course
* of their official duties. Pursuant to title 17 Section 105 of the
* United States Code, this software is not subject to copyright protection
* and is in the public domain. NIST assumes no responsibility whatsoever for
* its use by other parties, and makes no guarantees, expressed or implied,
* about its quality, reliability, or any other characteristic.
*
*/


#ifndef TNT_SPARSE_MATRIX_CSR_H
#define TNT_SPARSE_MATRIX_CSR_H

#include "tnt_array1d.h"

namespace TNT
{


/**
	Read-only view of a sparse matrix in compressed-row storage
	format.  Neither array elements (nonzeros) nor sparsity
	structure can be modified.  If modifications are required,
	create a new view.

	<p>
	Index values begin at 0.

	<p>
	<b>Storage requirements:</b> An (m x n) matrix with
	nz nonzeros requires no more than  ((T+I)*nz + M*I)
	bytes, where T is the size of data elements and
	I is the size of integers.
	

*/
template <class T>
class Sparse_Matrix_CompRow {

private:
	Array1D<T>    val_;       // data values (nz_ elements)
    Array1D<int>  rowptr_;    // row_ptr (dim_[0]+1 elements)
    Array1D<int>  colind_;    // col_ind  (nz_ elements)

    int dim1_;        // number of rows
    int dim2_;        // number of cols
  
public:

	Sparse_Matrix_CompRow(const Sparse_Matrix_CompRow &S);
	Sparse_Matrix_CompRow(int M, int N, int nz, const T *val, 
						const int *r, const int *c);
    


    inline   const T&      val(int i) const { return val_[i]; }
    inline   const int&         row_ptr(int i) const { return rowptr_[i]; }
    inline   const int&         col_ind(int i) const { return colind_[i];}

    inline   int    dim1() const {return dim1_;}
    inline   int    dim2() const {return dim2_;}
       int          NumNonzeros() const {return val_.dim1();}


    Sparse_Matrix_CompRow& operator=(
					const Sparse_Matrix_CompRow &R);



};

/**
	Construct a read-only view of existing sparse matrix in
	compressed-row storage format.

	@param M the number of rows of sparse matrix
	@param N the  number of columns of sparse matrix
	@param nz the number of nonzeros
	@param val a contiguous list of nonzero values
	@param r row-pointers: r[i] denotes the beginning position of row i
		(i.e. the ith row begins at val[row[i]]).
	@param c column-indices: c[i] denotes the column location of val[i]
*/
template <class T>
Sparse_Matrix_CompRow<T>::Sparse_Matrix_CompRow(int M, int N, int nz,
	const T *val, const int *r, const int *c) : val_(nz,val), 
		rowptr_(M, r), colind_(nz, c), dim1_(M), dim2_(N) {}


}
// namespace TNT

#endif

