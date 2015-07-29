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
  Module:    $RCSfile: vtkpxMergeFmriConventional.h,v $
  Language:  C++
  Date:      $Date: 2002/09/26 19:00:04 $
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

// .NAME vtkpxMergeFmriConventional - merges and creates overlays between t-maps and anatomic images
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION Caveats
// .SECTION See Also
//  vtkpxProcessFmri


#ifndef __vtkpxMergeFmriConventional_h
#define __vtkpxMergeFmriConventional_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"

class vtkTransform;
class vtkAbstractTransform;

class vtkpxMergeFmriConventional : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkpxMergeFmriConventional *New();
  vtkTypeMacro(vtkpxMergeFmriConventional,vtkpxSimpleImageToImageFilter);


  // Description:
  // SetInputs coventional and fmri t-maps 
  virtual void SetConventional(vtkImageData* img);
  virtual void SetFmriInput(vtkImageData* fmri);

  virtual vtkImageData* GetFunctionalInput() { return this->fmriInput; }


  // Description:
  // Set Thresholds for overlays 
  vtkGetMacro(MinimumThreshold,float);
  vtkSetMacro(MinimumThreshold,float);

  vtkGetMacro(MaximumThreshold,float);
  vtkSetMacro(MaximumThreshold,float);

  vtkGetMacro(IntensityThreshold,int);
  vtkSetMacro(IntensityThreshold,int);


  // Description:
  // Select one frame or multiple frames
  vtkSetClampMacro(SingleFrameMode,int,0,1);
  vtkGetMacro(SingleFrameMode,int);
  vtkBooleanMacro(SingleFrameMode,int);

  // Description:
  // Select Frame 
  vtkSetMacro(CurrentFrame,int);
  vtkGetMacro(CurrentFrame,int);

  // Description:
  // Mode 0 or 1 , 0 0:55 for anatomy and 56:63 for function
  // 1 0:191 for anatomy and 192:255 for function 
  // 2 0:225 for anatomy and 226:255 for function
  vtkGetMacro(OutputMode,int);
  vtkSetClampMacro(OutputMode,int,0,2);

  // Description:
  // Set Threhold Mode 
  vtkSetMacro(ThresholdMode,int);
  vtkGetMacro(ThresholdMode,int);
  virtual void ThresholdPositiveOnly();
  virtual void ThresholdNegativeOnly();
  virtual void ThresholdBoth();
  virtual void ThresholdNone();

  // Description:
  // L-R Flip of Fmri
  vtkGetMacro(FlipFmri,int);
  vtkSetMacro(FlipFmri,int);

  // Description:
  // Do Median Filter of t-maps before overlay 
  vtkGetMacro(MedianFiltering,int);
  vtkSetMacro(MedianFiltering,int);

  // Description:
  // Normalize Anatomical Intensity
  vtkSetClampMacro(NormalizeAnatomical,int,0,1);
  vtkGetMacro(NormalizeAnatomical,int);


  // Description:
  // Set Transformation  for transformation 1
  virtual void SetTransformation1(vtkAbstractTransform* trans);
  virtual vtkAbstractTransform* GetTransformation1();

  // Description:
  // Set Transformation  for transformation 2
  virtual void SetTransformation2(vtkAbstractTransform* trans);
  virtual vtkAbstractTransform* GetTransformation2();

  
  // Description:
  // Create Bounds Transform from Bounds (Todd's current practice similarity!)
  vtkSetVectorMacro(SourceBounds,int,6);
  vtkGetVectorMacro(SourceBounds,int,6);
  vtkSetVectorMacro(TargetBounds,int,6);
  vtkGetVectorMacro(TargetBounds,int,6);
  virtual vtkTransform* GetBoundsTransform();


  // Description:
  // Get Laterality Indices
  vtkGetMacro(PositiveLaterality,double);
  vtkGetMacro(NegativeLaterality,double);


  // Description:
  // Equi-transform a `target'-image to have same dimensions as source using transform xform
  static vtkImageData* EquiResample(vtkImageData* source,vtkImageData* target,
					   vtkAbstractTransform* xform,
					   int median=0,int flipx=0);

  // Description:
  // Compute ROI Stats
  static int ComputeROIStatistics(vtkImageData* input,vtkImageData* target,vtkAbstractTransform* tr,vtkFloatArray* stats);
  //  static int ComputeROIStatisticsComplex(vtkImageData* input,vtkImageData* roidefinigion,vtkFloatArray* stats);
  //  static int ComputeROIStatisticsComplex(vtkImageData* input,vtkImageData* roidefinigion,vtkFloatArray* ranges,vtkFloatArray* stats);


  static int ModifyMask(vtkImageData* input,int x,int y,int z);
  
protected:

  vtkpxMergeFmriConventional();
  virtual ~vtkpxMergeFmriConventional();
  vtkpxMergeFmriConventional(const vtkpxMergeFmriConventional&) {};
  void operator=(const vtkpxMergeFmriConventional&) {};

  virtual void SimpleExecute(vtkImageData*,vtkImageData*);

  vtkImageData* DealWithTransformStuff();

  vtkImageData* fmriInput;
  int NormalizeAnatomical;
  int ThresholdMode;
  float MinimumThreshold;
  float MaximumThreshold;
  int IntensityThreshold;
  int SourceBounds[6];
  int TargetBounds[6];
  int MedianFiltering;
  int FlipFmri;
  int OutputMode;
  int  SingleFrameMode;
  int  CurrentFrame;
  
  double PositiveLaterality;
  double NegativeLaterality;

  vtkAbstractTransform* Transform1;
  vtkAbstractTransform* Transform2;
  
  // Description:
  // This is the function actually used to generate the Bounds Transform!
  static vtkTransform*  GetBoundsTransform(vtkImageData* conventional,
					   vtkImageData* fmriInput,
					   int SourceBounds[6],int TargetBounds[6]);

  // Description:
  // Find Step Function for anatomical and normalize intensity
  void ComputeStepBounds(vtkImageData* image,double range[2],int mode);
  float ComputeAnatomicalVoxelValue(float vox,double range[2],float min,float max);


  // Description:
  // Set fmriInput Image
  vtkSetObjectMacro(fmriInput,vtkImageData);


};

#endif

