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
  Module:    $RCSfile: vtkpxMatrix.h,v $
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
// .NAME vtkpxMatrix - stores and manipulates a 2D Matrix (float)
// .SECTION Description
// This class is an interface for manipulation of 2D Matrices. It is an interface to routines from
// <a href="www.netlib.org/clapack">CLAPACK</a>
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION Caveats
// Requires <a href="www.netlib.org/clapack">CLAPACK</a> and <a href="www.netlib.org/f2c">f2c</a>
// <br> Internal storage is 1-d array in column major order (i.e. Fortran convention)
// .SECTION See Also
//  vtkMath
//
#ifndef __vtkpxMatrix_h
#define __vtkpxMatrix_h

#include "vtkpxAbstractMatrix.h"
#include "pxutil.h"

class vtkpxMatrix : public vtkpxAbstractMatrix
{
public:
  static vtkpxMatrix *New();
  vtkTypeMacro(vtkpxMatrix,vtkpxAbstractMatrix);
  

  // Description:
  // Get The Full Storage Size of the Matrix
  vtkGetMacro(TrueSize,int);

  // Description:
  // Get The Total Size i.e. rows*columns
  vtkGetMacro(TotalSize,int);

  // Description:
  // Squeeze Memory Usage to Minimum
  virtual void Squeeze();

  // Description:
  // Routine to allocate memory 
  virtual void Allocate(int rows,int columns);

  // Description:
  // Routine to allocate memory and set equal to zero (alias for allocate ala matlab)
  virtual void Zeros(int rows,int cols);

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
  virtual void Print(const char* message);

  virtual void PrintRange(int row1,int row2,int col1,int col2);
  virtual void PrintRange(const char* message,int row1,int row2,int col1,int col2);
  virtual void Print(const char* message,const char* format);


  // Description:
  // Load/Save Matrix
  virtual int Load(const char* fname);
  virtual int Load(gzFile fin);

  virtual int Save(const char* fname);
  virtual int Save(const char* fname,const char* vname,int compression=0);
  virtual int Save(gzFile fout,const char* varname);
  virtual int Save(FILE*  fout,const char* varname);
  virtual int ExportAFNI(char* fname,const char* formatstring);

  // Description:
  // Matlab Read/Write
  virtual int ImportFromMatlab(const char* fname,const char* matrixname);
  virtual int ImportFromMatlab2(const char* fname,const char* matrixname,int debug);
  virtual int ExportToMatlab(const char* fname,const char* matrixname);

  // Description:
  // Copy oldmat
  virtual void  Copy(vtkpxMatrix* oldmat);

  // Description:
  // Copy Part of  oldmat(x1:x2,y1:y2)
  virtual void  Copy(vtkpxMatrix* oldmat,int x1,int x2,int y1,int y2);
  virtual int   CopyInto(vtkpxMatrix* oldmat,int x1,int x2,int y1,int y2,int newx,int newy);

  
  // Description:
  // Set Matrix from double** array (as used by vtkMath)
  virtual void SetMatrix(double** mat,int x,int y);
  virtual void  Copy(double** mat,int x,int y);
  virtual void  Copy(double** mat,int x1,int x2,int y1,int y2);

  //BTX
  // Description:
  // Get/Set Element in row=i column=j without range checking UNSAFE
  inline virtual float  GetDirectElement(const int i,const int j)               { return Matrix[j*Size[0]+i] ;}
  inline virtual void   SetDirectElement(const int i,const int j,const float v) { Matrix[j*Size[0]+i]=v;}
  //ETX


  // Description:
  // Get Row
  int  GetRow(int row,int maxval,float* values);  
  int  GetRow(int row,int maxval,double* values);

  int  GetColumn(int column,int maxval,float* values);
  int  GetColumn(int column,int maxval,double* values);

  int  SetRow(int row,int maxval,float* values);
  int  SetRow(int row,int maxval,double* values);
  int  SetColumn(int column,int maxval,float* values);
  int  SetColumn(int column,int maxval,double* values);

  // Description:
  // Export matrix to new double** pointer for use in vtkMath
  double**  GetNewMatrixPointer();

  // Description:
  // Multiply All elements in matrix by s
  virtual void  Scale(float s);
  virtual void  ScaleAdd(float multi,float offset);

  // Description:
  // Transpose Matrix
  virtual void  Transpose();

  // Description:
  // Static methods: a+b=results, a*b=results 
  static  void  Add(vtkpxMatrix* a,vtkpxMatrix* b,vtkpxMatrix* result);
  static  void  Add(float wa,vtkpxMatrix* a,float wb,vtkpxMatrix* b,vtkpxMatrix* result);
  static  void  Multiply(vtkpxMatrix* a,vtkpxMatrix* b,vtkpxMatrix* result);
  static  int   IsEqual(vtkpxMatrix* a,vtkpxMatrix* b, double tolerance);
  virtual void  MultiplyThisWith(vtkpxMatrix* b,vtkpxMatrix* result) { vtkpxMatrix::Multiply(this,b,result);}
  

  // Description:
  // Static methods: a*b*c=results
  static  void  Multiply3(vtkpxMatrix* a,vtkpxMatrix* b,vtkpxMatrix* c,vtkpxMatrix* result);

  // Description:
  // Static Method result=a'*b*a
  static  void  MultiplyTripleProduct(vtkpxMatrix* a,vtkpxMatrix* b,vtkpxMatrix* result);

  // Description:
  // Normalize columns of matrix to make them magnitude 1
  virtual void NormalizeColumns(int begin=-1,int end=-1);

  // Description:
  // Find Maximum values in matrix or column=col or row=row
  virtual float Max();
  virtual float MaxColumn(int col);
  virtual float MaxRow(int row);

  // Description:
  // Sum Matrix
  virtual float Sum();
  virtual float Median();

  // Description:
  virtual float SumSquares();
  virtual float SumMagnitude();

  // Description:
  // Output is vtkpxMatrix* containing sums of rows or columns
  virtual vtkpxMatrix* RowSums();
  virtual vtkpxMatrix* ColumnSums();


  // Description:
  // Diagonalization into Eigenvalues D and Eigenvectors U (D,U must be non-null);
  virtual int Diagonalize(vtkpxMatrix* D,vtkpxMatrix* U);
  virtual int Eigenvalues(vtkpxMatrix* D);
  
  // Description:
  // Diagonalization into Eigenvalues D and Eigenvectors U (D,U must be non-null), M is the average vector
  virtual int DoPCA(vtkpxMatrix* D,vtkpxMatrix* U,vtkpxMatrix* M);

  // Description:
  // Using lapack routines perform QR Decomposition (Q,R must be NON-NULL)
  virtual int   QRDecomposition(vtkpxMatrix* Q,vtkpxMatrix* R);
  virtual int   Q1Q2RDecomposition(vtkpxMatrix* Q1,vtkpxMatrix* Q2,vtkpxMatrix* R);

  // Description:
  // Invert matrix if it is square
  virtual int   Invert();
  
  // Description:
  // Solution of Least Squares and Weighted Least Squares Problems
  // of the form Min |b-Ax|2 this=A
  // and of the form | W(b-Ax)|2 this=A
  virtual int SolveLeastSquares(vtkpxMatrix* B,vtkpxMatrix* X);
  virtual int SolveWeightedLeastSquares(vtkpxMatrix* W,vtkpxMatrix* B,vtkpxMatrix* X);

  // Description:
  // Solution of Linear System of Equations of the form
  // Ax=b where this=A
  virtual int SolveLinearSystem(vtkpxMatrix* B,vtkpxMatrix* X);
  virtual int SolveSymmetricLinearSystem(vtkpxMatrix* B,vtkpxMatrix* X);
  virtual int SolveUpperSymmetricLinearSystem(vtkpxMatrix* B,vtkpxMatrix* X);
  virtual int SolveLowerSymmetricLinearSystem(vtkpxMatrix* B,vtkpxMatrix* X);

  // Description:
  // Succesive Over-relaxation code set omeaga=1.0 for Gaus-Seidel and thr<0.01
  virtual int SOR(vtkpxMatrix* B,vtkpxMatrix* X,float thr,float omega);

  // Description:
  // Create Smoothness Matrix
  virtual int BuildSmoothness(int ndim,int width,int height,int depth,float weight);

  //BTX
  virtual float* GetMatrixPointer() { return Matrix;}

  // -----------------------------------------------------
  // Description:
  // RPM Related Stuff;
  virtual void MapPoints(vtkpxMatrix* Y,vtkpxMatrix* VY);
  virtual void NormalizeMixtureMatrix(vtkpxMatrix* M_Outliers_Row);
  virtual void NormalizeRPMMatrix(vtkpxMatrix* M_Outliers_Row,vtkpxMatrix* M_Outliers_Column);


  // -----------------------------------------------------
  // Static Functions :: Can be called from other C++ Code
  // -----------------------------------------------------

  static double** NewMatrix(int x, int y) ;
  static void     DeleteMatrix(double** m, int x, int vtkNotUsed(y));
  static void     FillMatrixWithZeros(double** m, int x, int y);
  static void     MatrixMultiply(double** a, double** b, double** c,int ar, int ac, int br, int bc) ;
  static void     PrintMatrix(double **m, int x, int y);

  static void     InPlaceTransposeMatrix(double** m,int x, int y);
  static double** TransposeMatrix(double** m,int x, int y);

  static void     CopyMatrix(double** dest,double** src,int x ,int y);
  static double** CopyMatrix(double** src,int x ,int y);

  static double** SubMatrix(double** src,int x1,int x2,int y1,int y2);
  //ETX 

protected:

  //BTX
  vtkpxMatrix();
  virtual ~vtkpxMatrix();

  vtkpxMatrix(const vtkpxMatrix&) {};
  void operator=(const vtkpxMatrix&) {};

  int TrueSize;
  int TotalSize;
  float* Matrix;

  int InternalSolveLinearSystem(vtkpxMatrix* B,vtkpxMatrix* X,int symmetric);


  //ETX

  // Description
  // Unfinished for now -- but mostly works
  virtual int ImportFromMatlabV6(const char* fname,const char* matrixname,int debug=0);

  // Description
  // For cleaning up tab or , delimited files
  void ReplaceWithSpace(char* line,const char* t);
};
#endif

