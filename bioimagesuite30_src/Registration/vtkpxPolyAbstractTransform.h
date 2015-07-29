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
  Module:    $RCSfile: vtkpxPolyAbstractTransform.h,v $
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

 ========================================================================*/

#ifndef __vtkpxPolyAbstractTransform_h
#define __vtkpxPolyAbstractTransform_h

#include "vtkWarpTransform.h"
#include "vtkCollection.h"
#include "vtkImageData.h"
#include "vtkAbstractTransform.h"
#include "vtkTransform.h"
#include "vtkThinPlateSplineTransform.h"

class vtkpxPolyAbstractTransform : public vtkAbstractTransform
{
public:
  vtkTypeMacro(vtkpxPolyAbstractTransform,vtkAbstractTransform);
  void PrintSelf(ostream& os, vtkIndent indent);
  static vtkpxPolyAbstractTransform *New();

  // Description:
  // Set Image that functions as a lookup table 
  // 1-component == no blending
  // 4-component == (1=id1,2=wgt1 (x100),3=id2,4=wgt2(x100))
  vtkGetObjectMacro(RegionLabel,vtkImageData);
  vtkSetObjectMacro(RegionLabel,vtkImageData);

  // Description:
  // Get Thin Plate Spline Transform
  vtkGetObjectMacro(ThinPlateSplineTransform,vtkThinPlateSplineTransform);


  
  // Description:
  // Get Transformation Collection
  vtkGetObjectMacro(TransformationCollection,vtkCollection);

  // Description:
  // Make another transform of the same type.
  vtkAbstractTransform *MakeTransform();
  
  // Description:
  // Get Linear Transform
  //  virtual vtkAbstractTransform* GetTransform(int i) ;
  //  virtual void SetTransform(int i,vtkAbstractTransform* i);

  // Description:
  // Save & Load Transform into .tal file
  int Load(const char* fname);
  int Save(const char* fname);

  // Description:
  // Get MTime
  unsigned long GetMTime();


  // Description:
  // Inverse
  virtual void Inverse();

  // Description:
  // Identity
  virtual void Identity();


protected:
  vtkpxPolyAbstractTransform();
  virtual ~vtkpxPolyAbstractTransform();
  vtkpxPolyAbstractTransform(const vtkpxPolyAbstractTransform&) {};
  void operator=(const vtkpxPolyAbstractTransform&) {};

  vtkCollection* TransformationCollection;
  vtkImageData*  RegionLabel;
  vtkThinPlateSplineTransform* ThinPlateSplineTransform;

  // Description:
  virtual int IsValid();

  // Description:
  // Prepare the transformation for application.
  void InternalUpdate();

  // Description:
  // This method does no type checking, use DeepCopy instead.
  void InternalDeepCopy(vtkAbstractTransform *transform);

  // Description:
  // Internal Transform Point -- OK
  void InternalTransformPoint(const float in[3], float out[3]);
  void InternalTransformPoint(const double in[3], double out[3]);

  // Description:
  // Internal Transform Derivative (Not done yet) -- OK
  void InternalTransformDerivative(const float in[3], float out[3],
				  float derivative[3][3]);
  void InternalTransformDerivative(const double in[3], double out[3],
				  double derivative[3][3]);




};

#endif






