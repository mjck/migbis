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
  Module:    $RCSfile: vtkpxImageTensorSpline.h,v $
  Language:  C++
  Date:      $Date: 2002/03/12 16:50:25 $
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

#ifndef __vtkpxImageTensorSpline_h
#define __vtkpxImageTensorSpline_h

#include "vtkpxTensorBSplineTransform.h"
#include "vtkImageData.h"

class vtkpxImageTensorSpline : public vtkpxTensorBSplineTransform
{
public:
  static vtkpxImageTensorSpline *New();
  vtkTypeMacro(vtkpxImageTensorSpline,vtkpxTensorBSplineTransform);

  // Description:
  // Approximate Image Value
  virtual int  Run();
  virtual int  ApproximateImage(vtkImageData* img,float resolution,float spacing,float offest,float smoothness,float initial);
  virtual int  ApproximateFieldMap(double scalefactor,int phasedirection=1);

  // Description:
  // Output Image
  vtkGetObjectMacro(Output,vtkImageData);

  // Description:
  // Image To be Approximated
  vtkGetObjectMacro(Input,vtkImageData);
  vtkSetObjectMacro(Input,vtkImageData);

  // Description:
  // Weight Image -- auto generate if Mode = 1
  vtkGetObjectMacro(WeightImage,vtkImageData);
  vtkSetObjectMacro(WeightImage,vtkImageData);


  // Description:
  // Mask Image
  vtkGetObjectMacro(MaskImage,vtkImageData);
  vtkSetObjectMacro(MaskImage,vtkImageData);

  // Description:
  // Lambda is smoothness constant (default=0.0)
  vtkGetMacro(Lambda,double);
  vtkSetMacro(Lambda,double);

  // Description:
  // Sigma is image space smoothness (default =0.0)
  vtkGetMacro(Sigma,double);
  vtkSetMacro(Sigma,double);

  // Description:
  // Step Size for Gradient Computation
  vtkSetMacro(StepSize,double);
  vtkGetMacro(StepSize,double);


  // Description:
  // Control Point Spacing
  vtkSetMacro(ControlPointSpacing,double);
  vtkGetMacro(ControlPointSpacing,double);


  // Description:
  // Control Point Spacing
  vtkSetMacro(Resolution,double);
  vtkGetMacro(Resolution,double);


  // Description:
  // Control Point Spacing
  vtkSetMacro(PreSmoothIterations,int);
  vtkGetMacro(PreSmoothIterations,int);
  

  // Description:
  // Control Point Spacing
  vtkSetClampMacro(Mode,int,0,1);
  vtkGetMacro(Mode,int);


  // Description:
  // Control Point Spacing
  vtkSetClampMacro(AutoInitialize,int,0,1);
  vtkGetMacro(AutoInitialize,int);


  // Description:
  // Control Point Spacing
  vtkSetClampMacro(Iterations,int,1,100);
  vtkGetMacro(Iterations,int);


  // Description:
  // Control Point Spacing
  vtkSetClampMacro(Epsilon,double,0.0,100.0);
  vtkGetMacro(Epsilon,double);


  // Description:
  // Needed for Optimization
  virtual void  SetCurrentParameters(vtkDoubleArray* par);
  virtual void  GetCurrentParameters(vtkDoubleArray* par);
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);

  // Description:
  // Needed for Optimization
  static void   OptimizerFeedback(vtkObject* self,const char* line,double val);
  static double  CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output);
  static double  CalculateFunction(vtkObject* self,vtkDoubleArray* position);

  // Description:
  // Optimization Output
  double   LastError;
  double   LastSmoothness;


protected:
  vtkpxImageTensorSpline();
  virtual ~vtkpxImageTensorSpline();
  vtkpxImageTensorSpline(const vtkpxImageTensorSpline&) {};
  void operator=(const vtkpxImageTensorSpline&) {};


  // Description:
  // Members 
  vtkImageData* Output;
  vtkImageData* Input;
  vtkImageData* WeightImage;

  vtkImageData* SampledInputImage;
  vtkImageData* SampledWeightImage;
  vtkImageData* MaskImage;

  double   Lambda;
  double   StepSize;
  double   ControlPointSpacing;
  double   Resolution;
  int      PreSmoothIterations;
  int      Mode;
  int      AutoInitialize;
  int      Iterations;
  double   Epsilon;
  double   Sigma;

  // Description:
  // Helper Routines 
  virtual vtkImageData* ResampleAndSmoothImage(vtkImageData* source,double res,double sigma);
  virtual vtkImageData* GenerateOutput(vtkImageData* img);

  virtual double ComputeAverageSquaredDifference(vtkImageData* x,vtkImageData* y,vtkImageData* w=NULL);
  virtual double ComputeAverageSquaredFittingError(vtkImageData* x,vtkImageData* w=NULL);
  virtual double EvaluatePiece(vtkImageData* reference,vtkImageData* weights,int cpoint);


  virtual int SetInputForBiasFieldApproximation(vtkImageData* ref,double res);
  virtual int GenerateLaplacianExtentedFieldWeight(vtkImageData* field,vtkImageData* weight,double res,int iter);
  
};



#endif

