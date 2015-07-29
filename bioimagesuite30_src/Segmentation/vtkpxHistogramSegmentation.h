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
  Module:    $RCSfile: vtkpxHistogramSegmentation.h,v $
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
// .NAME vtkpxHistogramSegmentation -- compute segmentation betwenn images using a simplified mono-exponential model
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
//
//
//  Best Approximation to Rayleigh Beta for Right hand side tail of peak
// x~R(beta), x~N(1.14*beta,0.744*beta)
//
//   
//  Useful Gnuplot Code
//
//   r(x,b)=x/(b*b)*exp(-(0.5*x*x)/(b*b));
//   g(x,m,s)=1.0/sqrt(2.0*pi*s*s)*exp(-0.5*(x-m)*(x-m)/(s*s));
//
// plot r(x,1000) w l, g(x,1140,760),0.56*g(x,1120,653)+0.44*g(x,1283,841)
//
//

#ifndef __vtkpxHistogramSegmentation_h
#define __vtkpxHistogramSegmentation_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkFloatArray.h"

class vtkpxSplineStackSource;


class vtkpxHistogramSegmentation : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkpxHistogramSegmentation *New();
  vtkTypeMacro(vtkpxHistogramSegmentation,vtkpxSimpleImageToImageFilter);

  // Description:
  // Number Of Classes
  vtkGetMacro(DistributionModel,int);
  vtkSetClampMacro(DistributionModel,int,0,1);

  // Description:
  // Sigma Scale for Robust Distribution Model (i.e. Sum of Gaussians)
  vtkSetClampMacro(RobustScaleFactor,float,1.0,20.0);
  vtkGetMacro(RobustScaleFactor,float);

  // Description:
  // Number Of Classes
  vtkGetMacro(MaxSigmaRatio,float);
  vtkSetClampMacro(MaxSigmaRatio,float,0.0001,0.9);

  // Description:
  // Number Of Classes
  vtkGetMacro(NumberOfClasses,int);
  vtkSetClampMacro(NumberOfClasses,int,1,10);

  // Description:
  // Number Of Iterations
  vtkGetMacro(Iterations,int);
  vtkSetClampMacro(Iterations,int,0,200);

  // Description:
  // Key Channel i.e. channel to perform initial clustering on
  vtkGetMacro(KeyChannel,int);
  vtkSetMacro(KeyChannel,int);

  // Description:
  // Number Of Iterations
  vtkGetMacro(NumberOfBins,int);
  vtkSetClampMacro(NumberOfBins,int,4,1024);

  // Description:
  // Convergence for CMeans Values (<1.0)
  vtkGetMacro(Convergence,float);
  vtkSetClampMacro(Convergence,float,0.05,100);

  // Description:
  // Histogram Sigma
  vtkGetMacro(Sigma,float);
  vtkSetClampMacro(Sigma,float,0.05,5.0);

  // Description:
  // Number Of Iterations
  vtkGetMacro(GenerateLabels,int);
  vtkSetClampMacro(GenerateLabels,int,0,1);
  vtkBooleanMacro(GenerateLabels,int);


  // Description:
  // Get Outputs
  vtkGetObjectMacro(InitialParameters,vtkFloatArray);
  vtkSetObjectMacro(InitialParameters,vtkFloatArray);

  // Description:
  // Get Outputs
  vtkGetObjectMacro(MaskImage,vtkImageData);
  vtkSetObjectMacro(MaskImage,vtkImageData);


  // Description:
  // Get Outputs
  vtkGetObjectMacro(Parameters,vtkFloatArray);
  vtkGetObjectMacro(Histogram,vtkImageData);


  // Description:
  // Outlier Class
  vtkSetClampMacro(OutlierDetection,int,0,1);
  vtkGetMacro(OutlierDetection,int);
  vtkBooleanMacro(OutlierDetection,int);


  // Description:
  // Outlier Probability
  vtkSetMacro(OutlierProbability,float);
  vtkGetMacro(OutlierProbability,float);

  // Description:
  // Outlier Class
  vtkSetClampMacro(PreprocessingMode,int,0,2);
  vtkGetMacro(PreprocessingMode,int);

  /*  // Description:
  // Outlier Class
  vtkSetClampMacro(RayleighDistributedBackground,int,0,1);
  vtkGetMacro(RayleighDistributedBackground,int);*/


  // Description:
  // Normalize Image Using Histogram
  static int HistogramNormalizeImage(vtkImageData* image,float pmin,float pmax);

  // Description:
  // Fat Computation Stuff
  static vtkImageData* ComputeFatValues(vtkImageData* orig_image,vtkImageData* orig_map,vtkpxSplineStackSource* surface,
					int abd_fat_value,int artifact_value,int subq_fat_value,int numclasses);

  static vtkImageData* ComputeCaprioFatValues(vtkImageData* orig_map,
					      vtkpxSplineStackSource* abdomen_surface,
					      vtkpxSplineStackSource* visceral_surface,
					      vtkpxSplineStackSource* fascia_surface,
					      vtkpxSplineStackSource* skin_surface,
					      float minv,float maxv);

  // Description:
  // ROI Analysis Stuff
  static int ComputeSegmentedMapStatistics(vtkImageData* image,vtkImageData* map,vtkFloatArray* output_values);

  // Description:
  // Create Synthetic Two Channel Image (Three concentric circles) with Rayleigh Noise
  static vtkImageData*   CreateRayleighSynthetic(float beta,float m1,float sigma1,float m2,float sigma2);

  // Description:
  // Get Threshold
  static float GetThreshold(float mean1,float mean2,float sigma1,float sigma2);
  static float GetGaussianDifference(float x,float m1,float m2,float var1,float var2);
protected:

  vtkpxHistogramSegmentation();
  virtual ~vtkpxHistogramSegmentation();
  vtkpxHistogramSegmentation(const vtkpxHistogramSegmentation&) {};
  void operator=(const vtkpxHistogramSegmentation&) {};

  virtual void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData* in,vtkImageData* out);

  virtual void HistogramSegmentation(vtkImageData* singlechanngel,vtkImageData* labels,vtkFloatArray* initial_parameters,int useinitial=1,int usevar=1);


  // Description:
  // Metric
  virtual float Metric(float x,float m,float sigma2);


  // Description:
  // Components 
  virtual int CreateHistogram(vtkImageData* input,int NumBins);
  virtual int InitializeParameters(vtkImageData* histogram,int numclasses,vtkFloatArray* params);

  int           NumberOfClasses;
  //  int           RayleighDistributedBackground;
  int           Iterations;
  float         Convergence;
  float         HistogramOrigin;
  float         HistogramSpacing;
  float         Sigma;
  vtkFloatArray* Parameters;
  vtkFloatArray* InitialParameters;
  vtkImageData* Histogram;
  vtkImageData* MaskImage;
  int           NumberOfBins;
  int           GenerateLabels;
  int           OutlierDetection;
  int           PreprocessingMode;
  float         OutlierProbability;
  int           DistributionModel;
  float         MaxSigmaRatio;
  float         RobustScaleFactor;
  int           KeyChannel;
};

#endif

