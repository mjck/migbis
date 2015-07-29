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

//
// This file derives from original work
// in the vtkCISG Toolkit
// that was released under the terms of 
// the GNU General Public License (either version 2,
// of the License, or any later version.)
//
// It may consist of work derived from more than one source
// file in the vtkCISG Toolkit. The original copyright notice
// was:
//
//  Authors   : Thomas Hartkens <thomas@hartkens.de>
//              Daniel Rueckert <d.rueckert@doc.ic.ac.uk>
//	        Julia Schnabel  <julia.schnabel@kcl.ac.uk>
//	        under the supervision of Dave Hawkes and Derek Hill
//  Web       : http://www.image-registration.com
//  Copyright : King's College London
//              Div. of Radiological Sciences, 
//              Computational Imaging Science Group, 1997 - 2000
//	        http://www-ipg.umds.ac.uk/cisg





#ifndef __VTKPXABSREGISTRATION_H
#define __VTKPXABSREGISTRATION_H

#include "vtkObject.h"
#include "vtkAbstractTransform.h"
#include "vtkpxJointHistogram.h"
#include "vtkProcessObject.h"
#include "vtkDoubleArray.h"

class vtkpxAbstractIntensityRegistration : public vtkProcessObject {

public:

  vtkTypeMacro(vtkpxAbstractIntensityRegistration,vtkProcessObject);
  virtual void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // This one also 
  static vtkpxAbstractIntensityRegistration* New();
  
  // Description:
  // Sets input for the registration filter
  virtual void SetReferenceImage(vtkImageData* targ,int frame);
  virtual void SetReferenceImage(vtkImageData* targ) { this->SetReferenceImage(targ,0);}
  
  virtual void SetTransformImage(vtkImageData* src ,int frame);
  virtual void SetTransformImage(vtkImageData* src ) { this->SetTransformImage(src,0); }

  // Description:
  // Set Reference and Target Weights (Target Weights unused mostly)
  vtkSetObjectMacro(ReferenceWeightImage,vtkImageData);
  vtkGetObjectMacro(ReferenceWeightImage,vtkImageData);
  vtkSetObjectMacro(TransformWeightImage,vtkImageData);
  vtkGetObjectMacro(TransformWeightImage,vtkImageData);


  // Description:
  // Set Orientations if needed
  vtkGetMacro(ReferenceOrientation,int);
  vtkSetClampMacro(ReferenceOrientation,int,0,100);
  vtkGetMacro(TransformOrientation,int);
  vtkSetClampMacro(TransformOrientation,int,0,100);


  // Description:
  // Feedback Stuff
  vtkSetMacro(ProvideFeedback,int);
  vtkGetMacro(ProvideFeedback,int);
  vtkBooleanMacro(ProvideFeedback,int);

  // Description:
  // Feedback Stuff
  vtkSetClampMacro(LogarithmMode,int,0,1);
  vtkGetMacro(LogarithmMode,int);
  vtkBooleanMacro(LogarithmMode,int);


  // Description:
  // Feedback Stuff
  vtkSetClampMacro(InterpolationMode,int,-5,3);
  vtkGetMacro(InterpolationMode,int);

  // Description:
  // Positive Intensities Only
  vtkSetClampMacro(PositiveIntensitiesOnly,int,0,1);
  vtkGetMacro(PositiveIntensitiesOnly,int);
  vtkBooleanMacro(PositiveIntensitiesOnly,int);

  // Description:
  // Normalize Intensities
  vtkSetClampMacro(AutoNormalizeIntensities,int,0,1);
  vtkGetMacro(AutoNormalizeIntensities,int);
  vtkBooleanMacro(AutoNormalizeIntensities,int);

  // Description:
  // Intra Modality Use same range
  vtkSetClampMacro(JointlyNormalizeIntensities,int,0,1);
  vtkGetMacro(JointlyNormalizeIntensities,int);
  vtkBooleanMacro(JointlyNormalizeIntensities,int);
  

  // Description:
  // Histogram Stuff
  vtkSetObjectMacro(Histogram,vtkpxJointHistogram);
  vtkGetObjectMacro(Histogram,vtkpxJointHistogram);


  // Description:
  // GetTransformation
  virtual vtkAbstractTransform* GetTransformation() { return NULL;}


  // Description:
  // NumberOfLevels, NumberOfSteps, Resolution
  vtkSetMacro(NumberOfLevels,int);
  vtkGetMacro(NumberOfLevels,int);

  // Description:
  // Number Of Extra Levels
  vtkSetMacro(NumberOfExtraLevels,int);
  vtkGetMacro(NumberOfExtraLevels,int);

  vtkSetMacro(NumberOfBins,int);
  vtkGetMacro(NumberOfBins,int);
  vtkSetMacro(NumberOfIterations,int);
  vtkGetMacro(NumberOfIterations,int);
  vtkSetMacro(NumberOfSteps,int);
  vtkGetMacro(NumberOfSteps,int);
  vtkSetMacro(Resolution,double);
  vtkGetMacro(Resolution,double);
  vtkSetClampMacro(BlurFactor,double,0.0,5.0);
  vtkGetMacro(BlurFactor,double);
  vtkSetMacro(StepSize,double);
  vtkGetMacro(StepSize,double);

  // Description:
  // Similarity Measure
  vtkGetMacro(SimilarityMeasure,int);
  vtkSetMacro(SimilarityMeasure,int);

  virtual void SetSimilarityMeasureToJointEntropy() { this->SetSimilarityMeasure(1);}
  virtual void SetSimilarityMeasureToCorrelation()  { this->SetSimilarityMeasure(2);}
  virtual void SetSimilarityMeasureToGradientCorrelation() { this->SetSimilarityMeasure(3);}
  virtual void SetSimilarityMeasureToMutualInformation() { this->SetSimilarityMeasure(4);}
  virtual void SetSimilarityMeasureToNormalizedMutualInformation() { this->SetSimilarityMeasure(5);}
  virtual void SetSimilarityMeasureToSumofSquaredDifferences() { this->SetSimilarityMeasure(6);}
  virtual void SetSimilarityMeasureToCorrelationRatioYX() { this->SetSimilarityMeasure(7);}
  virtual void SetSimilarityMeasureToCorrelationRatioXY() { this->SetSimilarityMeasure(8);}
  virtual void SetSimilarityMeasureToProduct() { this->SetSimilarityMeasure(9);}
  virtual void SetSimilarityMeasureToDifferenceEntropy() { this->SetSimilarityMeasure(10);}

  // Description:
  // Runs the registration filter
  virtual void Run();

  // Description:
  // Returns the name of the similarity measure
  const char *NameOfMeasure();

  // Description:
  // Prints information about the progress of the registration
  virtual void PrintInformation() {};

  // Description:
  // GetOutputStatus 
  const char *GetOutputStatus();

  // Description:
  // Get Update Transformation
  vtkAbstractTransform* GetUpdateTransformation() { return this->GetTransformation(); }

  // Description:
  // These are needed for the Optimizer 
  static void  OptimizerFeedback(vtkObject* self,const char* line,double val);
  static double CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output);
  static double CalculateFunction(vtkObject* self,vtkDoubleArray* position);
  static double OnBeginIteration(vtkObject* self,vtkDoubleArray* position);

  // Description:
  // Optimization Method
  vtkSetMacro(OptimizationMethod,int);
  vtkGetMacro(OptimizationMethod,int);

  // Description:
  // Shorthand for different optimization methods 
  virtual void SetOptimizationMethodToOldStyle() { this->SetOptimizationMethod(-1); }
  virtual void SetOptimizationMethodToDefault() { this->SetOptimizationMethod(0); }
  virtual void SetOptimizationMethodToSlowClimb() { this->SetOptimizationMethod(1); }
  virtual void SetOptimizationMethodToPowell() { this->SetOptimizationMethod(2); }
  virtual void SetOptimizationMethodToGradientDescent() { this->SetOptimizationMethod(3); }
  virtual void SetOptimizationMethodToConjugateGradient() { this->SetOptimizationMethod(4); }
  virtual void SetOptimizationMethodToHybrid() { this->SetOptimizationMethod(5); }
 
  // Description:
  // Get Last Output Measures
  vtkGetMacro(LastSimilarity,double);
  vtkGetMacro(LastSmoothness,double);

  // Description:
  // Set Grid Spacing at highest resolution
  vtkSetClampMacro(ResolutionRate,double,1.01,2.0);
  vtkGetMacro(ResolutionRate,double);

protected:

  // Description:
  // Make all these protected 
  // Constructor
  vtkpxAbstractIntensityRegistration();
  virtual ~vtkpxAbstractIntensityRegistration();
  vtkpxAbstractIntensityRegistration(const vtkpxAbstractIntensityRegistration&) {};
  void operator=(const vtkpxAbstractIntensityRegistration&) {};



  // Description:
  // First input image. This image is denoted as target image and its 
  // coordinate system defines the frame of reference for the registration.
  vtkImageData *ReferenceImage;
  vtkImageData *SampledReferenceImage;
  vtkImageData *ReferenceWeightImage;
  vtkImageData *SampledReferenceWeightImage;
  int           ReferenceOrientation;

  // Description:
  // Second input image. This image is denoted as source image. The goal of
  // the registration is to find the transformation which maps the source 
  // image into the coordinate system of the target image.
  vtkImageData *TransformImage;
  vtkImageData *TransformWeightImage;
  vtkImageData *SampledTransformImage;
  vtkImageData *SampledTransformWeightImage;
  int           TransformOrientation;

  // Description:
  // Histogram
  vtkpxJointHistogram *Histogram;

  // Description:
  // Various Control Parameters
  int   NumberOfLevels;
  int   NumberOfExtraLevels;
  int   NumberOfIterations;
  int   NumberOfSteps;
  int   NumberOfBins;
  int   LogarithmMode;
  int   InterpolationMode;
  int   PositiveIntensitiesOnly;
  int   AutoNormalizeIntensities;
  int   JointlyNormalizeIntensities;
  int   OptimizationMethod;
  int   MidlineSymmetricMode;

  // Description:
  // Make these private
  double StepSize;
  double Resolution;
  double BlurFactor;

  double ResolutionRate;
  double InternalResolution;
  double InternalStepSize;
  int   ProvideFeedback;
  double CurrentResolution;
  double CurrentStepSize;
  double InternalBlurFactor;
  int   SimilarityMeasure;
  int   KeepSameNumberOfSlices;
  char  OutputStatus[800];
  double LastProgress;
  double BaseResolution[3];
  double MinimumResolution;
  double MeanResolution;

  //BTX
  double LastSimilarity,LastSmoothness;
  //ETX

  // Description:
  // Initial set up for the registration at all levels
  virtual int InitializeAll();

  // Description:
  // Initial set up for the registration at a multiresolution level
  virtual int Initialize(int Level);

  // Description:
  // Run Optimization for Level level
  virtual int ComputeRegistration(int level) { return 0 ;}
  
  // Description:
  // Final set up for the registration at a multiresolution level
  virtual int Finalize(int level);

  // Description:
  // Final set up for the registration at all levels
  virtual int FinalizeAll();

  // Description:
  // Generate Output
  virtual void GenerateOutput(int level, int step,int iter,double stepsize,double simil);
  
  // Description:
  // Evaluates the similarity measure. This function returns the value of the 
  // appropriate similarity measure from the joint histogram of the target 
  // and source image. if NULL uses this->Histogram
  virtual double Similarity(vtkpxJointHistogram* histogram=NULL);

  // Description:
  // Evaluates the registration. This function evaluates the registration by
  // looping over the target image and interpolating the transformed source 
  // image while filling the joint histogram. This function returns the value
  // of the similarity measure using Similarity().
  double Evaluate();
  double WeightedEvaluate();
  
  
  // Description:
  // Set The OutputStatus 
  virtual void SetOutputStatus(const char* line,double val);


  // Description:
  // Needed for Optimizer -- set,get,optimizer,optimizervalue
  virtual void SetCurrentParameters(vtkDoubleArray* par) {}
  virtual void GetCurrentParameters(vtkDoubleArray* par) {}
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad) {return 0.0;}
  virtual double OptimizerValue(vtkDoubleArray* params) {return 0.0;}
  virtual double OnBeginIteration(vtkDoubleArray* params) {return 0.0;}

};



#endif


