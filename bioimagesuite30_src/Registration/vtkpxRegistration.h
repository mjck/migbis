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

// ------------------------------------------------------------------------
//
// This file derives from original work
// in the vtkCISG Toolkit
// that was released under the terms of 
// the GNU General Public License (either version 2,
// of the License, or any later version.)
// 
// The code has been modified for inclusion in BioImage Suite
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
//
// ------------------------------------------------------------------------

#ifndef __VTKPXREGISTRATION_H
#define __VTKPXREGISTRATION_H

#include "vtkObject.h"
#include "vtkAbstractTransform.h"
#include "vtkpxJointHistogram.h"
#include "vtkProcessObject.h"
#include "vtkLinearTransform.h"
#include "vtkFloatArray.h"

class vtkpxComboTransform;
class vtkPolyData;

class vtkpxRegistration : public vtkProcessObject {

public:

  vtkTypeMacro(vtkpxRegistration,vtkProcessObject);


  // Description:
  // This one also 
  static vtkpxRegistration* New();
  

  // Description:
  // Generate Jacobian Image using transformation tr and reference ref and transform tr
  static int ComputeJacobianImage(vtkImageData* ref,vtkImageData* jacobian,vtkAbstractTransform* tr);
  static int ComputeSimpleJacobianImage(vtkImageData* ref,vtkImageData* jacobian,vtkAbstractTransform* tr,float scale=100,float offset=1);
  static int ComputeSimpleJacobianImage(vtkImageData* ref,vtkImageData* jacobian,vtkAbstractTransform* tr,float scale,float offset,float threshold);

  // Surface Area Expansions (Jacobians)
  static int   ConvertAreaExpansionImageToSurface(vtkImageData* img, vtkPolyData* surface);
  static float ComputeAreaExpansionSurface(vtkPolyData* surface,vtkAbstractTransform* tr,float scale=100.0,float offset=1.0);
  static int   SmoothAreaExpansionSurface(vtkPolyData* surface,int iterations=10,float lambda=0.5);
  static vtkImageData* ComputeAreaExpansionImage(vtkPolyData* surface);



  static int ComputePrincipalStrains(vtkImageData* ref,vtkImageData* jacobian,vtkAbstractTransform* tr,float threshold=0.05);

  static int ComputeJacobianImage(vtkImageData* ref,vtkImageData* jacobian,vtkAbstractTransform* tr,float scale,float shift=0.0);
  static int ComputeJacobianImage(vtkImageData* ref,vtkImageData* jacobian,vtkAbstractTransform* tr,float scale,float shift,float threshold);
  static int ComputeCompleteJacobianImage(vtkImageData* ref,vtkImageData* jacobian,vtkpxComboTransform* tr,int nonlinearonly);

  // Description:
  // Compute Average Jacobian for image
  static float ComputeAverageJacobian(vtkImageData* ref,vtkAbstractTransform* tr);
  static float ComputeAverageJacobian(vtkImageData* ref,vtkAbstractTransform* tr,float srate);

  // Description:
  // Center Image Origin
  static void CenterImageOrigin(vtkImageData* img);


  // Description:
  // Voxel Wise Estimation of Group Entropy from Miller et al 
  // Intensity Correction   y(t)=x*(a*t+b) -- defaults a=0 b=1;
  static double ComputeEntropy(unsigned long n,short* data,double drift=1.0,short ignore=-20000);
  static double ComputeTotalGroupEntropy(vtkImageData* img,double drift=1.0,short ignore=-20000);
  static double ComputeWeightedTotalGroupEntropy(vtkImageData* img,vtkImageData* wgt,double drift=1.0,short ignore=-20000);
  static vtkImageData* ComputeVoxelwiseEntropyImage(vtkImageData* img,double drift=1.0,short ignore=-20000);


  // Description:
  // Compute Similarity given two images and a resolution
  static float ComputeSimilarityBetweenImages(vtkImageData* img1,vtkImageData* img2,
					      vtkAbstractTransform* tr,
					      double sp[3],float sigma,int metric,int interpolation,int numbins);

  static float ComputeSimpleSimilarityBetweenImages(vtkImageData* img1,vtkImageData* img2,
						    vtkAbstractTransform* tr);


  static void ComputeAllSimilarityMetricsBetweenImages(vtkImageData* img1,vtkImageData* img2,
						       vtkAbstractTransform* tr,vtkFloatArray* results);

  static void ComputeAllSimilarityMetricsBetweenImages(vtkImageData* img1,vtkImageData* img2,
						       vtkAbstractTransform* tr,vtkFloatArray* results,int numbins);


  // Description:
  // This function adjusts the image and computes appropriate number of bins to use
  static int CalculateNumberOfBinsAndAdjustImage(vtkImageData *image, int maxbin,
						 short& minv,short& maxv,
						 int logmode,int posmode=0,int autonorm=0);  

  static int CalculateNumberOfBinsAndAdjustImages(vtkImageData *image1,vtkImageData* image2, 
						  int maxbin,
						  short& minv,short& maxv,
						  int logmode,int posmode=0,int autonorm=0);  

  static int CalculateRange(vtkImageData* image, short& minv,short& maxv,int autonorm=0);  
  static int AdjustImage(vtkImageData* image,int maxbins,short minv,short maxv,int logmode,int posmode=0,int autonorm=0);

  static int ScaleWeightImage(vtkImageData* weightimage,vtkImageData* sampledimage,double maxvalue=100.0);
  

  // Description:
  // Create A Level in the Multi-scale pyramid
  static int ResampleAndSmoothImage(vtkImageData* destination,
				    vtkImageData* source,
				    double baseresolution[3],
				    float gaussian,
				    float resolution,int sameslices=0);
  static int ExtractComponentAndCastImageToShort(vtkImageData* destination,vtkImageData* source,int frame,int doscale=1);

  // Description:
  // Reslice Image using transformation and fill in histogram
  static int ResliceImage(vtkImageData* destination,vtkImageData* source,vtkImageData* ref,
			  vtkAbstractTransform* transform,
			  int interp);

  static int MultiplyAndOverWriteImage(vtkImageData* inout,vtkImageData* scale);

  // Description:
  // Re-orient Image
  static int ReOrientImage(vtkImageData* destination,vtkImageData* source,int inputorient,int outputorient);

  // Description::
  // Evaluate Histogram Based Similarity
  static float HistogramEvaluate(vtkImageData* ref,vtkImageData* targ,vtkAbstractTransform* trans,
				 vtkpxJointHistogram* histo,int similaritymeasure,int interp=1,int reset=1);

  static float WeightedHistogramEvaluate(vtkImageData* ref,vtkImageData* wgt,vtkImageData* targ,vtkImageData* wgt2,
					 vtkAbstractTransform* trans,
					 vtkpxJointHistogram* histo,int similaritymeasure,int interp=1,int reset=1);



  
  // Description::
  // Seam Images 
  static int SeamImages(vtkImageData* ref,vtkImageData* targ,vtkLinearTransform* tr,vtkImageData* output,int intensitymap);

  
  // Description:
  // Auto Reorient Image
  static int AutoReorientImages(vtkImageData* orig_ref,int ref_orient,
				vtkImageData* targ,int trg_orient,
				vtkImageData* output);



  // Description:
  // Debugging Aid 
  static void PrintImageInfo(vtkImageData* tmp,const char* comment);

  // Description:
  // Fix Pipeline Spacing and Origin
  static void FixPipelineSpacingAndOrigin(vtkImageData* img);
  

protected:

  // Description:
  // Make all these protected 
  // Constructor
  vtkpxRegistration();
  virtual ~vtkpxRegistration();
  vtkpxRegistration(const vtkpxRegistration&) {};
  void operator=(const vtkpxRegistration&) {};


};



#endif


