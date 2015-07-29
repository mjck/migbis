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
  Module:    $RCSfile: vtkpxMRFSegmentation.h,v $
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
// .NAME vtkpxMRFSegmentation -- compute segmentation betwenn images using a simplified mono-exponential model
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>

#ifndef __vtkpxMRFSegmentation_h
#define __vtkpxMRFSegmentation_h

#include "vtkpxHistogramSegmentation.h"
#include "vtkImageData.h"
#include "vtkFloatArray.h"
#include "vtkAbstractTransform.h"
#include "vtkpxImageTensorSpline.h"
//BTX
class vtkpxMRFSegmentationOptimizationStructure {

public:
  vtkDataArray* image_data;
  vtkDataArray* labels;
  vtkDataArray* biasfield;
  vtkFloatArray* params;
  int   voxel_index;
  int   incr[6];
  float wgth[6];
  float smoothness;
  float noisesigma;
  float currentfraction;
  int   currentclass[2];
  int   numberofcomponentclasses;
  int   debug;

  void SetBaseValues(vtkDataArray* image_data0,
		     vtkDataArray* labels0,
		     vtkDataArray* biasfield0,
		     vtkFloatArray* params0,
		     int* incr0,
		     float* wght0,
		     float smoothness0,
		     float noisesigma0)
  {
    this->image_data=image_data0;
    this->labels=labels0;
    this->params=params0;
    this->biasfield=biasfield0;
    for (int i=0;i<=5;i++)
      {
	this->incr[i]=incr0[i];
	this->wgth[i]=wght0[i];
      }
    this->smoothness=smoothness0;
    this->noisesigma=noisesigma0;

  }
  
  void SetIndex(int voxel_index0,int debug0=0)
  {
    this->voxel_index=voxel_index0;
    this->debug=debug0;
  }
  


};
//ETX

class vtkpxMRFSegmentation : public vtkpxHistogramSegmentation
{
public:
  static vtkpxMRFSegmentation *New();
  vtkTypeMacro(vtkpxMRFSegmentation,vtkpxHistogramSegmentation);

  // Description:
  // Single Channel i.e. force Single Channel Mode
  vtkGetMacro(SingleChannel,int);
  vtkSetClampMacro(SingleChannel,int,0,1);
  vtkBooleanMacro(SingleChannel,int);

  // Description:
  // Compute Bias Field
  vtkGetMacro(ComputeBiasField,int);
  vtkSetClampMacro(ComputeBiasField,int,0,1);
  vtkBooleanMacro(ComputeBiasField,int);


  // Description:
  // Number Of Iterations
  vtkGetMacro(MRFIterations,int);
  vtkSetClampMacro(MRFIterations,int,0,20);

  // Description:
  // Weight of MRF
  vtkGetMacro(Smoothness,float);
  vtkSetClampMacro(Smoothness,float,0,100.0);


  // Description:
  // Weight of MRF
  vtkGetMacro(BiasFieldFactor,float);
  vtkSetClampMacro(BiasFieldFactor,float,-20.0,20.0);

  // Description:
  // Weight of MRF
  vtkGetMacro(MRFThreshold,float);
  vtkSetClampMacro(MRFThreshold,float,0,100.0);

  // Description:
  // Weight of MRF
  vtkGetMacro(ImageNoiseSigma,float);
  vtkSetClampMacro(ImageNoiseSigma,float,0,10000.0);

  // Description:
  // Partial Step
  vtkGetMacro(PartialStep,float);
  vtkSetClampMacro(PartialStep,float,0.05,0.5);

  // Description:
  // Do Histogram Normalization 0.01:0.99
  vtkSetClampMacro(HistogramNormalization,int,0,1);
  vtkGetMacro(HistogramNormalization,int);

  // Description:
  // Max Percentage of Voxels changing needed for iteration to converge (0 == all voxels)
  vtkGetMacro(MRFConvergencePercentage,float);
  vtkSetClampMacro(MRFConvergencePercentage,float,0,0.1);

  // Description:
  // Debug Voxel
  vtkGetMacro(DebugVoxel,int);
  vtkSetMacro(DebugVoxel,int);

  // Description:
  // Fuzzy Classification
  vtkSetClampMacro(FuzzyClassification,int,0,1);
  vtkGetMacro(FuzzyClassification,int);

  // Description:
  // MultiFrameOutput
  vtkGetObjectMacro(MultiFrameOutput,vtkImageData);

  // Description:
  // BiasField
  vtkGetObjectMacro(BiasField,vtkImageData);

  // Description:
  // BiasFieldWeights
  vtkGetObjectMacro(BiasFieldWeights,vtkImageData);

  // Description:
  // Fit Quality Map
  vtkSetObjectMacro(FitQualityMap,vtkImageData);
  vtkGetObjectMacro(FitQualityMap,vtkImageData);
  


  
protected:

  vtkpxMRFSegmentation();
  virtual ~vtkpxMRFSegmentation();
  vtkpxMRFSegmentation(const vtkpxMRFSegmentation&) {};
  void operator=(const vtkpxMRFSegmentation&) {};

  virtual void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData* in,vtkImageData* out);


  // Description:
  // Master Optimization
  // ----------------------------------------------------------------------------------------------------------
  virtual void FuzzyClassificationOptimization(vtkImageData* cleanLabels,vtkImageData* cleanInput,
					       vtkImageData* fittedImage,vtkImageData* fitQuality,
					       vtkImageData* biasField,float noise_sigma);

  virtual void Optimization(vtkImageData* cleanLabels,vtkImageData* cleanInput,vtkImageData* fitted_image,
			    vtkImageData* fit_quality,vtkImageData* biasField,
			    float noise_sigma);
			      

  // Initialization and Final Cleaning Stuff
  // ----------------------------------------------------------------------------------------------------------
  virtual int CheckInitialParameters(vtkImageData* input);
  virtual vtkImageData* CreateInitialLabels(vtkImageData* output);
  virtual vtkImageData* CreateSingleChannel(vtkImageData* input);

  virtual void GenerateInitialCleanLabelImageFromFirstFrame(vtkImageData* tmpLabels,vtkImageData* cleanLabels);
  virtual void GenerateLabelOutputs(vtkImageData* cleanLabels,vtkImageData* output);
  virtual void InitializeSegmentation(vtkImageData* single_channel,vtkImageData* fitted_image,vtkImageData* fit_quality,vtkImageData* cleanLabels);

  virtual void GenerateFittedParametersAndCleanInput(vtkImageData* input,vtkImageData* cleanInput,vtkImageData* fitted_image,vtkImageData* fit_quality);


  // M-Step Is Easy for the most_part
  // Compute Parameter Estimates Given Classication and r2,density estimates
  // -----------------------------------------------------------------------
  virtual void UpdateFitQualityMapFromMRF(vtkImageData* label_image,vtkImageData* fit_quality,float threshold=1.0);
  virtual void ComputeClassSums(vtkImageData* label_image,
				vtkImageData* fitted_image,
				vtkImageData* fit_quality,
				vtkImageData* bias_field,
				float partial_threshold,
				vtkFloatArray* params,int numclasses);
  virtual int FitModelToSums(vtkFloatArray* params,int numclasses);
  virtual float GetChannelEchoTime(int i) { return float(i+1);}
  virtual void ComputeClassMeansAndStandardDeviations(vtkFloatArray* params,int numclasses,int domeans=1,int doprint=1);
  virtual void DoExpectationStep(vtkImageData* label_image,
				 vtkImageData* intensity_image,
				 vtkImageData* fit_quality,
				 vtkImageData* bias_field,
				 float partial_threshold,
				 vtkFloatArray* params,int numclasses);

  
  // E-Step
  // -----------------------------------
  // Description:
  // Compute Invididual Probability Functions
  virtual void CombineMeansAndStandardDeviations(vtkFloatArray* params,int class1,int class2,float fraction1,float* theta,int debug=0);
  virtual double ComputeLogLikelihoodProbability(vtkDataArray* image_data,int vox,float* theta,float noise_sigma,float bfield,float& newbfield,int debug=0);
  virtual double ComputeMRFPrior(vtkDataArray* labels,int vox,float currentlab[20],int incr[6],float wgth[6],int debug=0);
  virtual double ComputeTotalMinusLogProbability(vtkDataArray* image_data,
						 vtkDataArray* labels,
						 int voxel_index,
						 float *currentlab,
						 float* theta,
						 float noise_sigma,
						 float bfield,
						 int incr[6],float wgth[6],float smoothness,int debug=0);

  virtual double ComputeTotalMinusLogProbabilityOutlier(vtkDataArray* labels,
						       int voxel_index,
						       float *currentlab,
						       float* theta,
						       int incr[6],float wgth[6],float smoothness,int debug=0);
  
  // Description:
  // Optimize For Each Voxel
  virtual float DoVoxelFit(int voxel_index);
  virtual int   DoHardClassification(int voxel_index,int actual_num_classes,float prob[20],float& bestprob,
				     float new_labels[20]);
  virtual int   DoFuzzyClassification(int voxel_index,int actual_num_classes,float prob[20],float& bestprob,
				      float new_labels[20]);
  
  // M-Step
  // -----------------------------------------
  virtual int  UpdateVoxel(int eveni,int evenj,int evenk,int pass);
  virtual int  ComputeMRFIncrementsAndWeights(vtkImageData* img,int incr[6],float wgt[6]);
  virtual int  DebugPrint(int vox);
  virtual float DoMaximizationStep(vtkImageData* label_image,vtkImageData* intensity_image,vtkImageData* biasfield,
				   float noise_sigma,
				   vtkFloatArray* params,int numclasses,
				   float smoothness,int maxiter);


  // Bias Field Stuff
  // ------------------------------------------
  virtual void   ComputeInitialBiasField(vtkImageData* image_data,vtkImageData* clean_labels,vtkImageData* fit_quality,
					 vtkImageData* bias_field_image,vtkImageData* weight_image,
					 vtkFloatArray* params);
  virtual void   TensorFitBiasFieldImage(vtkImageData* bias_field,vtkImageData* weight_image);
  virtual void   ComputeSmoothBiasField(vtkImageData* bias_field,vtkImageData* fit_quality,float Sigma);



  
  float       Smoothness;
  float       PartialStep;
  float       MRFConvergencePercentage;
  float       ImageNoiseSigma;
  float       MRFThreshold;
  int         MRFIterations;
  int         FuzzyClassification;
  int         DebugVoxel;
  int         HistogramNormalization;
  int         NumberOfChannels; 
  int         SingleChannel;
  int         MRFBasedFitQuality;
  float       BiasFieldFactor;
  float       BiasFieldSigma;
  int         ComputeBiasField;
  int         PerformingFuzzyClassification;

  //BTX
  vtkpxMRFSegmentationOptimizationStructure OptimizationStructure;
  vtkImageData* MultiFrameOutput;
  vtkImageData*  FitQualityMap;
  vtkImageData*  BiasField;
  vtkImageData*  BiasFieldWeights;
  vtkpxImageTensorSpline* TensorSpline;
  //ETX

  static int ChannelMean(int ch) { return ch*2+1; }
  static int ChannelSigma(int ch) { return ch*2+2; }

};

#endif

