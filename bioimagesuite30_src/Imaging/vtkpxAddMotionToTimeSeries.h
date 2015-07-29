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
  Module:    $RCSfile: vtkpxAddMotionToTimeSeries.h,v $
  Language:  C++
  Date:      $Date: 2002/02/26 17:28:15 $
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
// .NAME vtkpxAddMotionToTimeSeries
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>

#ifndef __vtkpxAddMotionToTimeSeries_h
#define __vtkpxAddMotionToTimeSeries_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkAbstractTransform.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
class vtkTransform;


class vtkpxAddMotionToTimeSeries : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkpxAddMotionToTimeSeries *New();
  vtkTypeMacro(vtkpxAddMotionToTimeSeries,vtkpxSimpleImageToImageFilter);

  // Description:
  // Set value of the block period e.g 8on/8off
  vtkGetMacro(BlockPeriod,int);
  vtkSetClampMacro(BlockPeriod,int,1,100);

  // Description:
  // Set value of the block period e.g 8on/8off
  vtkGetMacro(UseMotionParameters,int);
  vtkSetClampMacro(UseMotionParameters,int,0,1);


  // Description:
  // Transformation Parameters -- used for computing motion for sequence 
  vtkSetObjectMacro(TransformationParameters,vtkFloatArray);
  vtkGetObjectMacro(TransformationParameters,vtkFloatArray);


  // Description:
  // Transformation Parameters -- used for computing motion for sequence 
  vtkSetObjectMacro(ReferenceImage,vtkImageData);
  vtkGetObjectMacro(ReferenceImage,vtkImageData);


  // Description:
  // Transformation Parameters -- used for computing motion for sequence 
  vtkSetObjectMacro(ObliqueTransform,vtkAbstractTransform);
  vtkGetObjectMacro(ObliqueTransform,vtkAbstractTransform);

  // Description:
  // Set the slice thickness if different from slice spacing
  vtkGetMacro(SliceThickness,float);
  vtkSetClampMacro(SliceThickness,float,0.0,100.0);

  // Description:
  // Create Gaussain random variable
  static float GaussianRandom();


  // Description:
  // Create Sagging Head Motion Path 
  static int CreateMotionPath(vtkFloatArray* newScalars,int numframes,
			      float rotation,float translation,int orientation,
			      float noise1,float noise2);
  static int CreateMotionPath(vtkFloatArray* newScalars,int numframes,
			      vtkFloatArray* parameters);

  static int SampleMotionPath(vtkFloatArray* param_in,vtkFloatArray* param_out,int samplerate);

  // Description:
  // Multiply Transformation Parameters and Compare Them 
  // These are used for validation of motion correction schemes 
  static int MultiplyTransformationParameters(int beginp,int endp,float factor,vtkFloatArray* scal);
  static int CompareTransformationParameters(vtkFloatArray* scal1,vtkFloatArray* scal2,
					     vtkFloatArray *ouput);


protected:

  vtkpxAddMotionToTimeSeries();
  virtual ~vtkpxAddMotionToTimeSeries();
  vtkpxAddMotionToTimeSeries(const vtkpxAddMotionToTimeSeries&) {};
  void operator=(const vtkpxAddMotionToTimeSeries&) {};

  void SimpleExecute(vtkImageData *input,vtkImageData *out);
  void ExecuteInformation();

  vtkAbstractTransform* ObliqueTransform;
  vtkFloatArray*        TransformationParameters;
  vtkImageData*  ReferenceImage;
  int                   BlockPeriod;
  int                   UseMotionParameters;  
  float                 SliceThickness;


  // Description:
  // ResampleImage image in space defined by ref using tr=xform and Cubic Interpolation
  static void ResampleAndSmoothImage(vtkImageData* image,
				     vtkImageData* ref,
				     float SliceThickness,
				     vtkAbstractTransform* xform1,
				     vtkAbstractTransform* xform2);
  

  // Description:
  // Copy Component of input  into component of output 
  static void CopyComponent(vtkImageData* input,vtkImageData* output,
			    int incomp,int outcomp);


  // Description:
  // Set Transformation Needed 
  static void SetMotionTransform(vtkTransform* transform,
				 vtkFloatArray* parameters,
				 int frame,float minusshift[3],float shift[3]);



};

#endif

