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




#ifndef _VTKPXJOINTHISTOGRAM_2D_H

#define _VTKPXJOINTHISTOGRAM_2D_H

#include "vtkObject.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkpxJointHistogram : public vtkObject 
{
public:
  static vtkpxJointHistogram *New();
  vtkTypeMacro(vtkpxJointHistogram,vtkObject);
  
  // Description:
  // Backup and Reset Histogram
  virtual void ResetHistogram();
  virtual void BackupHistogram();

  // Description:
  // Get/Set number of bins
  vtkGetVector2Macro(NumberOfBins,int);
  virtual int  GetNumberOfBinsX();
  virtual int  GetNumberOfBinsY();

  // Description:
  // Clear histogram
  virtual void Reset();

  // Description:
  // Allocate Histogram
  virtual void Allocate(int numx,int numy);

  // Description:
  // Get number of samples in histogram
  vtkGetMacro(NumberOfSamples,int);

  // Description:
  // Add counts to bins
  int AddToBin(short x,short y,short count=1);
  int AddToBin(unsigned char x,unsigned char y,unsigned char count=1);

  // Description:
  // Delete counts from bins
  int DelToBin(short x,short y,short count=1);
  int DelToBin(unsigned char x,unsigned char y,unsigned char count=1);


  // Description:
  // Fill Histogram From Images 
  int FillHistogram(vtkImageData* ref,vtkImageData* targ,int reset);
  int WeightedFillHistogram(vtkImageData* ref,vtkImageData* targ,vtkImageData* weights,int reset);
  int DualWeightedFillHistogram(vtkImageData* ref,vtkImageData* targ,vtkImageData* weight1,vtkImageData* weights2,int reset);
  int FillHistogramComponent(vtkImageData* ref,vtkImageData* targ,int component1,int component2,int reset);

  // Description:
  // Fill Histogram from Combination of Image and Intensity Gradient for Optimization
  int FillGradientHistogram(vtkImageData* ref,vtkImageData* targ,vtkImageData* grad,double step,int component,int reset);

  // This removes an original image
  int RemoveFromHistogram(vtkImageData* ref,vtkImageData* targ);
  int RemoveFromHistogramComponent(vtkImageData* ref,vtkImageData* targ,int component1,int component2);
  int WeightedRemoveFromHistogram(vtkImageData* ref,vtkImageData* targ,vtkImageData* weights);
  int DualWeightedRemoveFromHistogram(vtkImageData* ref,vtkImageData* targ,vtkImageData* weight1,vtkImageData* weights2);


  // Description:
  // Get Bin Count
  int GetBinCount(int,int);
  void SetBinCount(int a,int b,int val);

  // Description:
  // Statistics Stuff 
  // Description:
  // Calculate joint probability p(x, y)
  float JointProbability(int, int);

  // Description:
  // Calculate marginal probability p(x)
  float MarginalProbabilityX(int);

  // Description:
  // Calculate marginal probability p(y)
  float MarginalProbabilityY(int);

  // Description:
  // Calculate conditional probability p(x|y)
  float ConditionalProbabilityXY(int, int);

  // Description:
  // Calculate conditional probability p(y|x)
  float ConditionalProbabilityYX(int, int);

  // Description:
  // Calculate mean
  float MeanX();

  // Description:
  // Calculate mean
  float MeanY();

  // Description:
  // Calculate conditional mean
  float ConditionalMeanXY(int);

  // Description:
  // Calculate conditional mean
  float ConditionalMeanYX(int);

  // Description:
  // Calculate variance
  float VarianceX();

  // Description:
  // Calculate variance
  float VarianceY();

  // Description:
  // Calculate covariance
  float Covariance();

  // Description:
  // Calculate standard deviation
  float StandardDeviationX();

  // Description:
  // Calculate standard deviation
  float StandardDeviationY();

  // Description:
  // Calculate marginal entropy
  float EntropyX();

  // Description:
  // Calculate marginal entropy
  float EntropyY();

  // Description:
  // Calculate joint entropy
  float JointEntropy();

  // Description:
  // Calculate mutual information
  float MutualInformation();

  // Description:
  // Calculate normalized mutual information
  float NormalizedMutualInformation();

  // Description:
  // Calculate cross correlation
  float CrossCorrelation();

  // Description:
  // Calculate correlation ratio
  float CorrelationRatioXY();

  // Description:
  // Calculate correlation ratio
  float CorrelationRatioYX();

  // Description:
  // Calculate sums of squared differences
  float SumsOfSquaredDifferences();

  // Description:
  // Compute Product (ala Convolution)
  float ComputeProduct();

  // Description:
  // Compute Entropy of (y-x)
  float ComputeEntropyOfDifference();

  // Description:
  // Compute Similarity
  static  const char *NameOfMeasure(int measureno);
  virtual float Similarity(int measureno);

  // Description:
  // Image Input/Output
  virtual vtkImageData* GetHistogramAsImage();
  virtual int   SetHistogramFromImage(vtkImageData* img);
  
  // Description:
  // One Stop Shop
  virtual void CreateHistogram(vtkImageData* ref, int numbinsref,vtkImageData* trg, int numbinstrg);
  virtual void CreateDualWeightedHistogram(vtkImageData* ref, int numbinsref,vtkImageData* trg, int numbinstrg,
					   vtkImageData* refw,vtkImageData* trgw);

			       

protected:
  
  // Description:
  // Number of bins 
  int NumberOfBins[2];
  
  // Description:
  // Number of samples
  int NumberOfSamples;
  int OldNumberOfSamples;
  
  //BTX
  // Description:
  // Dynamic memory for bins
  int**  Bins;
  int**  OldBins;
  //ETX

  // Description:
  // Copy contructor.
  vtkpxJointHistogram();
  vtkpxJointHistogram(const vtkpxJointHistogram &) { }
  virtual ~vtkpxJointHistogram();


  // Description:
  // Check and Return
  virtual int CheckHistogram();

  // Description:
  // Check for single frame short
  virtual int AreBothImagesShort(vtkImageData* img1,vtkImageData* img2);
  virtual int AreBothImagesUnsignedChar(vtkImageData* img1,vtkImageData* img2);
  

};


#endif

