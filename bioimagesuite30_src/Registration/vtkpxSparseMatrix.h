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





/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxSparseMatrix.h,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
// .NAME vtkpxSparseMatrix - stores and manipulates a 2D Sparse Matrix (float) using STL map
// .SECTION Description
// .SECTION See Also
//  vtkpxMatrix
//
#ifndef __vtkpxSparseMatrix_h
#define __vtkpxSparseMatrix_h

#include "vtkObject.h"
#include "dvSparseMatrix.h"
#include "vtkpxAbstractMatrix.h"

class vtkpxSparseMatrix : public vtkpxAbstractMatrix
{
public:
  static vtkpxSparseMatrix *New();
  vtkTypeMacro(vtkpxSparseMatrix,vtkpxAbstractMatrix);

  // Description:
  // Matrix Density
  virtual float Density();
  
  // Description:
  // Routine to allocate memory and set equal to zero (alias for allocate ala matlab)
  virtual void Zeros(int rows,int cols);

  // Description:
  // Routine to allocate memory 
  virtual void Allocate(int rows,int columns);

  // Description:
  // Routines to allocate memory and set equal to one
  virtual void Ones(int rows,int cols);

  // Description:
  // Routines to allocate memory for square matrix and initialize accordingly and set to identity
  virtual void Eye(int rowcol);

  // Description:
  // Sets current matrix=identity matrix (if it is square)
  virtual void Identity();

  // Description:
  // Set all elements = zero
  virtual void Zero();


  // Description:
  // Set all elements = a
  virtual void Fill(float a);

  // Description:
  // Print Matrix to stderr
  virtual void InternalPrint();
  virtual void PrintBW();
  virtual void Print(const char* message);
  
  //BTX
  // Description:
  // Get/Set Element in row=i column=j without range checking UNSAFE
  inline virtual float  GetDirectElement(const int i,const int j)               { return this->Matrix[i][j]; }
  inline virtual void   SetDirectElement(const int i,const int j,const float v) { this->Matrix[i][j]=v;}
  //ETX

  // Description:
  // Get/Set Element in row=i column=j with range checking SAFE
  virtual float   GetElement(int i,int j);
  virtual void    SetElement(int i,int j,float v);
  virtual void    AddToElement(int i,int j,float v);

  // Description:
  // Multiply All elements in matrix by s
  virtual void  Scale(float s);


  // Description:
  // Static methods: a+b=results, a*b=results 
  static  void Add(vtkpxSparseMatrix* a,vtkpxSparseMatrix* b,vtkpxSparseMatrix* result);
  static  void Add(float wa,vtkpxSparseMatrix* a,float wb,vtkpxSparseMatrix* b,vtkpxSparseMatrix* result);
  static  void Multiply(vtkpxSparseMatrix* a,vtkpxMatrix* b,vtkpxMatrix* result);

  // Description:
  // this*b=c
  virtual void MultiplyThisWith(vtkpxMatrix* b,vtkpxMatrix* c);

  //BTX
  // Description:
  //  dvSparseMatrix<float>* GetMatrixPointer() { return &Matrix; }
  dvSparseMatrix* GetMatrixPointer() { return &Matrix; }
  //ETX


  // Description:
  // Stuff Needed for RPM
  virtual void MapPoints(vtkpxMatrix* Y,vtkpxMatrix* VY);
  virtual void NormalizeMixtureMatrix(vtkpxMatrix* M_Outliers_Row);
  virtual void NormalizeRPMMatrix(vtkpxMatrix* M_Outliers_Row,vtkpxMatrix* M_Outliers_Column);


protected:

  //BTX
  vtkpxSparseMatrix();
  virtual ~vtkpxSparseMatrix();

  vtkpxSparseMatrix(const vtkpxSparseMatrix&) {};
  void operator=(const vtkpxSparseMatrix&) {};

  //  int Index(int i,int j) { return j*Size[0]+i; }
  //dvSparseMatrix<float> Matrix; 
  dvSparseMatrix Matrix; 
  //ETX

  
};
#endif

