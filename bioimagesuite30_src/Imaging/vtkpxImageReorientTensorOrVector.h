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
  Module:    $RCSfile: vtkpxImageReorientTensorOrVector.h,v $
  Language:  C++
  Date:      $Date: 2002/09/18 20:22:27 $
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
// .NAME vtkpxImageReorientTensorOrVector -- compute the difference and a normalized difference
// between two images
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>

#ifndef __vtkpxImageReorientTensorOrVector_h
#define __vtkpxImageReorientTensorOrVector_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkAbstractTransform.h"

class vtkMatrix4x4;
class vtkPoints;
class vtkLandmarkTransform;

class vtkpxImageReorientTensorOrVector : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkpxImageReorientTensorOrVector *New();
  vtkTypeMacro(vtkpxImageReorientTensorOrVector,vtkpxSimpleImageToImageFilter);

  // Description:
  // DisplacementField -- second input to compare with input
  vtkSetObjectMacro(DisplacementField,vtkImageData);
  vtkGetObjectMacro(DisplacementField,vtkImageData);

  // Description:
  // DisplacementField -- second input to compare with input
  vtkSetObjectMacro(Transformation,vtkAbstractTransform);
  vtkGetObjectMacro(Transformation,vtkAbstractTransform);

  // Description:
  // Use Only 7 Points
  vtkGetMacro(Use7Points,int);
  vtkSetClampMacro(Use7Points,int,0,1);
  vtkBooleanMacro(Use7Points,int);


  // Description:
  // Flip X,Y or Z axis
  vtkGetMacro(FlipX,int);
  vtkSetClampMacro(FlipX,int,0,1);
  vtkBooleanMacro(FlipX,int);

  vtkGetMacro(FlipY,int);
  vtkSetClampMacro(FlipY,int,0,1);
  vtkBooleanMacro(FlipY,int);

  vtkGetMacro(FlipZ,int);
  vtkSetClampMacro(FlipZ,int,0,1);
  vtkBooleanMacro(FlipZ,int);

protected:

  vtkpxImageReorientTensorOrVector();
  virtual ~vtkpxImageReorientTensorOrVector();
  vtkpxImageReorientTensorOrVector(const vtkpxImageReorientTensorOrVector&) {};
  void operator=(const vtkpxImageReorientTensorOrVector&) {};

  vtkImageData* DisplacementField;
  vtkAbstractTransform* Transformation;
  vtkMatrix4x4* GlobalMatrix;

  // Cached Data Structures to Eliminate allocate/deallocate
  vtkMatrix4x4* TempMatrix1;
  vtkMatrix4x4* TempMatrix2;
  vtkMatrix4x4* TempMatrix3;
  vtkPoints* TempPoints1;
  vtkPoints* TempPoints2;
  vtkLandmarkTransform* TempLandmarkTransform;
  int Use7Points;
  int FlipX;
  int FlipY;
  int FlipZ;
  double Flips[3];
  
  // Execute Warping
  virtual void SimpleExecute(vtkImageData* inp,vtkImageData* out);


  // Description:
  // Compute Local Rotation at each voxel
  virtual void ComputeLocalRotation(vtkImageData* inp,int i,int j,int k,double origin[3],double spacing[3],vtkMatrix4x4* matrix);
  virtual void WarpTensor(vtkImageData* inp,vtkImageData* output,int i,int j,int k,vtkMatrix4x4* matrix);
  virtual void WarpVector(vtkImageData* inp,vtkImageData* output,int i,int j,int k,vtkMatrix4x4* matrix);

  // Description:
  // Postprocess transformation to yield either displacement field or global matrix
  virtual void ProcessInputTransformation(vtkImageData* input);

};

#endif

