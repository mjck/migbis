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
  Module:    $RCSfile: vtkpxComboTransform.h,v $
  Language:  C++
  Date:      $Date: 2003/01/03 16:42:27 $
  Version:   $Revision: 1.2 $


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
#ifndef __vtkpxComboTransform_h
#define __vtkpxComboTransform_h

#include "vtkGeneralTransform.h"
#include "vtkpxGridTransform.h"
#include "vtkTransform.h"
#include "pxutil.h"

class vtkCollection;

class vtkpxComboTransform : public vtkGeneralTransform
{
 public:
  static vtkpxComboTransform *New();
  vtkTypeMacro(vtkpxComboTransform,vtkGeneralTransform);
  
  // Description:
  // Manipulate the Linear Transformation 
  vtkGetObjectMacro(LinearTransform,vtkTransform);
  virtual void ChangeLinearTransform(vtkTransform* LinearTransform);


  // Description:
  // Change The Grid Transformations 
  virtual int GetNumberOfGridTransformations();
  virtual vtkpxGridTransform* GetGridTransform();
  virtual vtkpxGridTransform* GetGridTransformByIndex(int index);

  virtual void ChangeGridTransform(vtkpxGridTransform* GridTransform);
  virtual void ChangeGridTransformByIndex(vtkpxGridTransform* GridTransform,int index);

  virtual void SetGridTransform(vtkpxGridTransform* GridTransform);
  virtual int  AddGridTransform(vtkpxGridTransform* GridTransform);

  // Description:
  // Change The Internal Transformations -- if more than one grid manipulate the last Grid Transform
  virtual void SetTransformations(vtkTransform* LinearTransform,vtkpxGridTransform* GridTransform);

  // Description:
  virtual int ClearGridTransformationsCollection();
  virtual int CollapseTransormationsCollection();
  
  // Description:
  // Load and Save Functionality 
  virtual int Load(const char* fname);
  virtual int Save(const char* fname);
  virtual int Save(const char* fname,int compression);
  
  //BTX
  virtual int Load(gzFile fin);
  virtual int Save(FILE* fout);
  virtual int Save(gzFile fout);
  //ETX
  
  // Description:
  // Virtual Do Copy (Shallow)
  virtual void Copy(vtkpxComboTransform *transform);

  // Description:
  // NonLinear First
  vtkGetMacro(NonLinearFirst,int);
  virtual void SetNonLinearFirst(int s);

  // Description:
  // Invert Transformation
  void Inverse();


 protected:
  // Description:
  // Update Stack
  virtual void UpdateStack();

  // Description:
  // Constructors and Destructors
  vtkpxComboTransform();
  virtual ~vtkpxComboTransform();

  // Description:
  // Set the Internal Transformations
  vtkSetObjectMacro(LinearTransform,vtkTransform);


  vtkpxComboTransform(const vtkpxComboTransform&);  
  void operator=(const vtkpxComboTransform&);  

  // Description:
  // Copy Transformation
  void InternalDeepCopy(vtkAbstractTransform *t);

  // Description:
  // Call this to initialize default mode
  virtual void InitializeTransformations();


  // Description:
  // These are the two Grid Transformations 
  vtkTransform*       LinearTransform;
  int                 NonLinearFirst;
  vtkCollection*      GridTransformationsCollection;


  //BTX
  // Description:
  // Most of the following simply protects methods from outside use ...
  // see vtkGeneralTransform.h for comments
  //  void Inverse() { vtkGeneralTransform::Inverse();}
  void Translate(double x, double y, double z) {  vtkGeneralTransform::Translate(x,y,z);}
  void RotateWXYZ(double angle, double x, double y, double z) { vtkGeneralTransform::RotateWXYZ(angle,x,y,z);}


  // Description:
  // Create a rotation matrix about the X, Y, or Z axis and concatenate
  // it with the current transformation according to PreMultiply or
  // PostMultiply semantics.  The angle is expressed in degrees.
  void RotateX(double angle) { vtkGeneralTransform::RotateWXYZ(angle, 1, 0, 0); }
  void RotateY(double angle) { vtkGeneralTransform::RotateWXYZ(angle, 0, 1, 0); }
  void RotateZ(double angle) { vtkGeneralTransform::RotateWXYZ(angle, 0, 0, 1); }

  // Description:
  // Create a scale matrix (i.e. set the diagonal elements to x, y, z)
  // and concatenate it with the current transformation according to
  // PreMultiply or PostMultiply semantics.
  void Scale(double x, double y, double z) { vtkGeneralTransform::Scale(x,y,z); }
  void Scale(const double s[3]) { vtkGeneralTransform::Scale(s[0], s[1], s[2]); }
  void Scale(const float s[3])  { vtkGeneralTransform::Scale(s[0], s[1], s[2]); }

  // Description:
  // Concatenates the matrix with the current transformation according
  // to PreMultiply or PostMultiply semantics.
  void Concatenate(vtkMatrix4x4 *matrix) { vtkGeneralTransform::Concatenate(matrix); }
  void Concatenate(const double elements[16]) { vtkGeneralTransform::Concatenate(elements); }


  // Description:
  // Concatenate the specified transform with the current transformation
  // according to PreMultiply or PostMultiply semantics.
  // The concatenation is pipelined, meaning that if any of the
  // transformations are changed, even after Concatenate() is called,
  // those changes will be reflected when you call TransformPoint().
  void Concatenate(vtkAbstractTransform *transform) { vtkGeneralTransform::Concatenate(transform);}

  // Description:
  // Sets the internal state of the transform to PreMultiply. All subsequent
  // operations will occur before those already represented in the
  // current transformation.  In homogeneous matrix notation, M = M*A where
  // M is the current transformation matrix and A is the applied matrix.
  // The default is PreMultiply.
  void PreMultiply()  { vtkGeneralTransform::PreMultiply(); }
  void PostMultiply() { vtkGeneralTransform::PostMultiply(); }

  // Description:
  void SetInput(vtkAbstractTransform *input) { vtkGeneralTransform::SetInput(input); }

  // Description:
  // Pushes the current transformation onto the transformation stack.
  void Push() { vtkGeneralTransform::Push();}

  // Description:
  // Deletes the transformation on the top of the stack and sets the top 
  // to the next transformation on the stack.
  void Pop() { vtkGeneralTransform::Pop(); }
  //ETX
};



#endif




