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
  Module:    $RCSfile: vtkpxAbstractBiasFieldCorrection.h,v $
  Language:  C++
  Date:      $Date: 2002/04/16 13:19:58 $
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
// .NAME vtkpxAbstractBiasFieldCorrection - Filter for ffd registration based on voxel similarity measures.

// .SECTION Description
// This class implements a registration filter for the non-rigid registration
// of images.


#ifndef _VTKPXAbstractBiasFieldCorrection_H
#define _VTKPXAbstractBiasFieldCorrection_H

#include "vtkImageData.h"
#include "vtkDoubleArray.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkFloatArray.h"


class  vtkpxAbstractBiasFieldCorrection : public vtkpxSimpleImageToImageFilter {

public:
  static vtkpxAbstractBiasFieldCorrection* New();
  vtkTypeMacro(vtkpxAbstractBiasFieldCorrection,vtkpxSimpleImageToImageFilter);

  // Description:
  // Print information about the progress of the registration
  void PrintInformation();

  // Description:
  // Feedback Stuff
  vtkSetMacro(ProvideFeedback,int);
  vtkGetMacro(ProvideFeedback,int);
  vtkBooleanMacro(ProvideFeedback,int);


  // Description:
  // Feedback Stuff
  vtkSetMacro(FixedNumberOfSlices,int);
  vtkGetMacro(FixedNumberOfSlices,int);
  vtkBooleanMacro(FixedNumberOfSlices,int);


  // Description:
  // NumberOfLevels, NumberOfSteps, Resolution
  vtkSetMacro(NumberOfLevels,int);
  vtkGetMacro(NumberOfLevels,int);
  vtkSetMacro(NumberOfIterations,int);
  vtkGetMacro(NumberOfIterations,int);
  vtkSetMacro(NumberOfSteps,int);
  vtkGetMacro(NumberOfSteps,int);
  vtkSetMacro(Resolution,double);
  vtkGetMacro(Resolution,double);
  vtkSetMacro(StepSize,double);
  vtkGetMacro(StepSize,double);

  // Description:
  // Frame
  // Selects the frame to use for correction in 4D images (3D + t)
  vtkSetMacro(Frame,int);
  vtkGetMacro(Frame,int);


  // Description:
  // These are needed for the Optimizer 
  static void  OptimizerFeedback(vtkObject* self,const char* line,double val);
  static double CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output);
  static double CalculateFunction(vtkObject* self,vtkDoubleArray* position);
  static double OnBeginIteration(vtkObject* self,vtkDoubleArray* position);
  

  // Run Optimization for Level level
  virtual int ComputeBiasFit(int level);
  
  // Description:
  // Get The Weight Image
  vtkGetObjectMacro(WeightImage,vtkImageData);
  vtkGetObjectMacro(BiasField,vtkImageData);

  // Description:
  // Get/Set the Mask Image
  vtkSetObjectMacro(MaskImage,vtkImageData);
  vtkGetObjectMacro(MaskImage,vtkImageData);

  // Description:
  // Get Internal Transformation Stuff
  vtkGetObjectMacro(ClassParameters,vtkFloatArray);
  vtkSetObjectMacro(ClassParameters,vtkFloatArray);

  // Description:
  // Epsilon is the convergence limit (default=0.0001);
  vtkGetMacro(Epsilon,double);
  vtkSetClampMacro(Epsilon,double,0.0001,0.1);

  // Description:
  // Lambda is the smoothness weight
  vtkGetMacro(Lambda,double);
  vtkSetClampMacro(Lambda,double,0.0,1.0);

  // Description:
  // Metric
  // 0 -- pabic multi-valley stuff
  // 1 -- entropy
  // 2 -- input itself is bias field
  vtkGetMacro(Metric,int);
  vtkSetClampMacro(Metric,int,0,2);
  virtual void SetMetricToPABIC()           { this->SetMetric(0);}
  virtual void SetMetricToEntropy()         { this->SetMetric(1);}
  virtual void SetMetricToMinimumVariance() { this->SetMetric(2);}


  // Description:
  // Valley Function
  static double Valley(double x,double sigma) { double x2=x*x; return x2/(x2+sigma*sigma+0.001);}
  static double Valley2(double x,double sigma2) { double x2=x*x; return x2/(x2+sigma2+0.001);}

  // GetOutputStatus 
  virtual void SetOutputStatus(const char* line,double v);
  const char *GetOutputStatus();

  // Description:
  // Synthetic Data
  vtkImageData* GenerateSynthetic(int width,double factor);


    // Description:
  // Outlier Class
  vtkSetClampMacro(Histo_OutlierDetection,int,0,1);
  vtkGetMacro(Histo_OutlierDetection,int);
  vtkBooleanMacro(Histo_OutlierDetection,int);


  // Description:
  // Outlier Probability
  vtkSetMacro(Histo_OutlierProbability,float);
  vtkGetMacro(Histo_OutlierProbability,float);


  // Description:
  // Outlier Probability
  vtkSetClampMacro(Histo_MaxSigmaRatio,float,0.1,1.0);
  vtkGetMacro(Histo_MaxSigmaRatio,float);

  // Description:
  // Number Of Classes
  vtkGetMacro(Histo_NumberOfClasses,int);
  vtkSetClampMacro(Histo_NumberOfClasses,int,2,10);


  // Description:
  // Number Of Iterations
  vtkGetMacro(Histo_NumberOfBins,int);
  vtkSetClampMacro(Histo_NumberOfBins,int,4,1024);

  // Description:
  // Convergence for CMeans Values (<1.0)
  vtkGetMacro(Histo_Convergence,float);
  vtkSetClampMacro(Histo_Convergence,float,0.05,100);

  // Description:
  // Histogram Sigma
  vtkGetMacro(Histo_Sigma,float);
  vtkSetClampMacro(Histo_Sigma,float,0.05,5.0);

  // Description:
  // Number Of Iterations
  vtkGetMacro(Histo_Iterations,int);
  vtkSetClampMacro(Histo_Iterations,int,0,200);

  // Description:
  // Number Of Iterations
  vtkGetMacro(Histo_PreprocessingMode,int);
  vtkSetClampMacro(Histo_PreprocessingMode,int,0,2);

  // Description:
  // Compute Corrected Image 
  static double        ComputeScaleFactor(vtkImageData* ref,vtkImageData* biasfield);  
  static vtkImageData* ComputeCorrectedImage(vtkImageData* ref,vtkImageData* biasfield);
  static vtkImageData* ComputeMeanPreservingCorrectedImage(vtkImageData* ref,vtkImageData* biasfield);
  static vtkImageData* ComputeCorrectedImage(vtkImageData* ref,vtkImageData* biasfield,int meanpreserving);

  static int ComputeCorrectedImage(vtkImageData* ref,vtkImageData* biasfield,vtkImageData* fixedimage,int meanpreserving);


  // Separate
  static vtkImageData* ComputeMaskCorrectedImage(vtkImageData* ref,vtkImageData* biasfield,vtkImageData* Mask,int meanpreserving);

  // Description:
  // Compute Corrected Log Image 
  static vtkImageData* ComputeCorrectedLogarithmImage(vtkImageData* logref,vtkImageData* logbiasfield);
  static vtkImageData* ComputeMeanPreservingCorrectedLogarithmImage(vtkImageData* logref,vtkImageData* logbiasfield);
  static vtkImageData* ComputeCorrectedLogarithmImage(vtkImageData* logref,vtkImageData* logbiasfield,int meanpreserving);

  // Description:
  // Exponentiate Bias Field
  static int ExponentiateBiasField(vtkImageData* biasfield);

  // Description:
  static vtkImageData* CreateLogReferenceImage(vtkImageData* input);

  // Description:
  // Compute Bias Field From Two Images
  static vtkImageData* ComputeRatioBiasFieldImage(vtkImageData* original,vtkImageData* corrected);
  

protected:

  // Make all these protected 
  // Constructor
  vtkpxAbstractBiasFieldCorrection();
  virtual ~vtkpxAbstractBiasFieldCorrection();
  vtkpxAbstractBiasFieldCorrection(const vtkpxAbstractBiasFieldCorrection&) {};
  void operator=(const vtkpxAbstractBiasFieldCorrection&) {};

  // Description:
  // Execute Stuff 
  virtual void SimpleExecute(vtkImageData* in,vtkImageData* out);

  // Description:
  // Set Convergence Limit
  double Epsilon;

  // Description:
  // Initial set up for the registration at all levels
  virtual int InitializeAll();

  // Description:
  // Optimize for step size = step
  virtual double Optimize(vtkDoubleArray* grad,double old_similarity,double step,int level,
			  double origin[3],double spacing[3],int dimensions[3],
			  double gridspacing[3]);

  // Description:
  virtual double Evaluate();

  // Description:
  // Create Bias Field Image
  virtual vtkImageData* ComputeBiasFieldImage(vtkImageData* ref);


  // Description:
  // Compute Class Parameters
  virtual vtkFloatArray* ComputeLogClassParameters(vtkFloatArray* parameters);
  virtual vtkFloatArray* ComputeClassParameters(vtkImageData* input,vtkFloatArray* initial=NULL);
  virtual int ComputeAndSetClassParameters(vtkImageData* input,vtkFloatArray* initial=NULL);

  // Description:
  // Check Mask Image
  virtual int CheckInputsAndCreateReferenceImage(vtkImageData* input);

  // Description:
  // Initial set up for the registration at a multiresolution level
  virtual int Initialize(int);
  
  // Description:
  // Called from Initialize to Initialize the Transformation
  virtual int InitializeTransformation(int);

  // Description:
  // Create Weight Image based on partial voluming
  virtual int CreateWeightImage(vtkImageData* ref);

  // Description:
  // Final set up for the registration at a multiresolution level
  virtual int FinalizeAll();


  // Description:
  // Generate Output
  virtual void GenerateOutput(int level, int step,int iter,double stepsize,double simil);


  // Description:
  // Needed for Optimizer
  virtual void   SetCurrentParameters(vtkDoubleArray* par);
  virtual void   GetCurrentParameters(vtkDoubleArray* par);
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);
  virtual double OnBeginIteration(vtkDoubleArray* params);

  // Description:
  // More Variables
  int ProvideFeedback;
  int NumberOfLevels;
  int NumberOfIterations;
  int NumberOfSteps;
  int FixedNumberOfSlices;
  double Resolution;
  double MinimumResolution;
  double InternalResolution;
  double InternalStepSize;
  double LastSmoothness;
  double LastSimilarity;
  double CurrentResolution;
  double CurrentStepSize;
  double StepSize;
  double Lambda;
  int  Metric;

  vtkImageData* SampledReferenceImage;
  vtkImageData* ReferenceImage;
  vtkImageData* SampledWeightImage;
  vtkImageData* WeightImage;
  vtkImageData* BiasField;
  vtkImageData* MaskImage;
  vtkFloatArray* ClassParameters;

  char  OutputStatus[500];
  double LastProgress;


  int   Histo_OutlierDetection;
  float Histo_OutlierProbability;
  int   Histo_NumberOfClasses;
  int   Histo_NumberOfBins;
  float Histo_Convergence;
  float Histo_MaxSigmaRatio;
  float Histo_Sigma;
  int   Histo_Iterations;
  int   Histo_PreprocessingMode;
  int   Frame;


};


#endif

