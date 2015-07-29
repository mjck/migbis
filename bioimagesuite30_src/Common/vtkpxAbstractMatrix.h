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
  Module:    $RCSfile: vtkpxAbstractMatrix.h,v $
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
// .NAME vtkpxAbstractMatrix - stores and manipulates a 2D Sparse Matrix (float) using STL map
// .SECTION Description
// .SECTION See Also
//  vtkpxMatrix
//

class vtkpxMatrix;

#ifndef __vtkpxAbstractMatrix_h
#define __vtkpxAbstractMatrix_h

#include "vtkObject.h"

class vtkpxAbstractMatrix : public vtkObject
{
public:
  vtkTypeMacro(vtkpxAbstractMatrix,vtkObject);
  
  // Description:
  // Gets the size of the matrix
  vtkGetVectorMacro(Size,int,2);

  // Description:
  // Matrix Density
  virtual float Density() { return 1.0;}

  // Description:
  // Routine to allocate memory and set equal to zero (alias for allocate ala matlab)
  virtual void Zeros(int rows,int cols)=0;

  // Description:
  // Routine to allocate memory 
  virtual void Allocate(int rows,int columns)=0;

  // Description:
  // Routines to allocate memory and set equal to one
  virtual void Ones(int rows,int cols)=0;

  // Description:
  // Routines to allocate memory for square matrix and initialize accordingly and set to identity
  virtual void Eye(int rowcol)=0;

  // Description:
  // Sets current matrix=identity matrix (if it is square)
  virtual void Identity()=0;

  // Description:
  // Set all elements = zero
  virtual void Zero()=0;


  // Description:
  // Set all elements = a
  virtual void Fill(float a)=0;

  // Description:
  // Print Matrix to stderr
  virtual void InternalPrint()=0;
  virtual void Print(const char* message)=0;
  
  //BTX
  // Description:
  // Get/Set Element in row=i column=j without range checking UNSAFE
  inline virtual float  GetDirectElement(const int i,const int j)=0;              
  inline virtual void   SetDirectElement(const int i,const int j,const float v)=0;
  //ETX

  // Description:
  // Get/Set Element in row=i column=j with range checking SAFE
  virtual float   GetElement(int i,int j);
  virtual void    SetElement(int i,int j,float v);
  virtual void    AddToElement(int i,int j,float v);

  // Description:
  // Multiply All elements in matrix by s
  virtual void  Scale(float s)=0;

  // Description:
  // this*b=c
  virtual void MultiplyThisWith(vtkpxMatrix* b,vtkpxMatrix* c)=0;

  // Description:
  // RPM Stuff
  virtual void MapPoints(vtkpxMatrix* Y,vtkpxMatrix* VY)=0;
  virtual void NormalizeMixtureMatrix(vtkpxMatrix* M_Outliers_Row)=0;
  virtual void NormalizeRPMMatrix(vtkpxMatrix* M_Outliers_Row,vtkpxMatrix* M_Outliers_Column)=0;
  virtual void MakeSymmetricByAddingTransposeOf(vtkpxAbstractMatrix* other);


protected:

  vtkpxAbstractMatrix();
  virtual ~vtkpxAbstractMatrix() {};

  int Size[2];
  int Irange(int a,int minv,int maxv) { if (a<minv) a=minv; else if (a>maxv) a=maxv;  return a; }
 
};
#endif

