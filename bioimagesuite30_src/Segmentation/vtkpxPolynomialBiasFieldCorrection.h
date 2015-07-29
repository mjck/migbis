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
  Module:    $RCSfile: vtkpxPolynomialBiasFieldCorrection.h,v $
  Language:  C++
  Date:      $Date: 2004/12/13 19:46:39 $
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
// .NAME vtkpxPolynomialBiasFieldCorrection - Filter for ffd registration based on voxel similarity measures.

// .SECTION Description
// This class implements a registration filter for the non-rigid registration
// of images.


#ifndef _VTKPXPolynomialBiasFieldCorrection_H
#define _VTKPXPolynomialBiasFieldCorrection_H

#include "vtkImageData.h"
#include "vtkDoubleArray.h"
#include "vtkDoubleArray.h"
#include "vtkpxAbstractBiasFieldCorrection.h"
#include "vtkImageData.h"
#include "vtkpxImageRobustHistogram.h"
//BTX
class vtkpxPolynomialBiasFieldCorrectionOptimizationStructure {
public:
  double old_similarity;
  double step;
  int    level;

  void SetValues(double old_similarity0,
		 double step0,
		 int level0)
  {
    this->old_similarity=old_similarity0;
    this->step=step0;
    this->level=level0;
  }
};
//ETX


class  vtkpxPolynomialBiasFieldCorrection : public vtkpxAbstractBiasFieldCorrection {

public:
  static vtkpxPolynomialBiasFieldCorrection* New();
  vtkTypeMacro(vtkpxPolynomialBiasFieldCorrection,vtkpxAbstractBiasFieldCorrection);

  // Description:
  // Get Last Output Measures
  vtkGetMacro(LastSimilarity,double);

  // Run Optimization for Level level
  virtual int ComputeBiasFit(int level);
  
  // Description:
  // Get The Transformation
  vtkGetObjectMacro(Coefficients,vtkDoubleArray);

  // Description:
  // Initial Parameters
  vtkGetObjectMacro(InitialCoefficients,vtkDoubleArray);
  vtkSetObjectMacro(InitialCoefficients,vtkDoubleArray);

  // Description:
  // RecomputeParameters
  // 1 == update parameters & pabic
  vtkSetClampMacro(RecomputeParameters,int,0,1);
  vtkGetMacro(RecomputeParameters,int);

  // Description:
  // MinValue/MaxValue
  // Minimum/Maximum Value of the Bias Field =0.2,5.0
  vtkSetClampMacro(MinValue,double,0.1,0.8);
  vtkGetMacro(MinValue,double);
  vtkSetClampMacro(MaxValue,double,1.25,10.0);
  vtkGetMacro(MaxValue,double);

  // Description:
  // Degre Linear or Quadratic
  vtkSetClampMacro(Degree,int,1,3);
  vtkGetMacro(Degree,int);

  // Description:
  // Create Bias Field Image
  static vtkImageData* ComputeBiasFieldImage(vtkImageData* ref,vtkDoubleArray* tr,int degree=-1,double minvalue=0.5,double maxvalue=2.5);
  static int ComputeBiasFieldImage(vtkImageData* ref,vtkImageData* bf,vtkDoubleArray* tr,int degree=1,double minvalue=0.5,double maxvalue=2.5);

  // Description:
  // Compute Entropy
  static float ComputeEntropy(vtkImageData* tmp);

  // Description:
  // Optimization Method
  vtkSetMacro(OptimizationMethod,int);
  vtkGetMacro(OptimizationMethod,int);

  // Description:
  // Shorthand for different optimization methods 
  virtual void SetOptimizationMethodToPowell() { this->SetOptimizationMethod(2); }
  virtual void SetOptimizationMethodToGradientDescent() { this->SetOptimizationMethod(3); }
  virtual void SetOptimizationMethodToConjugateGradient() { this->SetOptimizationMethod(4); }


protected:

  // Make all these protected 
  // Constructor
  vtkpxPolynomialBiasFieldCorrection();
  virtual ~vtkpxPolynomialBiasFieldCorrection();
  vtkpxPolynomialBiasFieldCorrection(const vtkpxPolynomialBiasFieldCorrection&) {};
  void operator=(const vtkpxPolynomialBiasFieldCorrection&) {};

  // Description:
  // Execute Stuff 
  virtual void SimpleExecute(vtkImageData* in,vtkImageData* out);

  // Description:
  // This controls the optimization method 0=default,1=Cong Gradient
  vtkpxPolynomialBiasFieldCorrectionOptimizationStructure OptimizationStructure;

  // Description:
  // Actual Transformation
  vtkDoubleArray*   Coefficients;
  vtkDoubleArray*   InitialCoefficients;
  vtkImageData*     TempBiasField;
  vtkImageData*     TempFixedImage;
  double            MinValue;
  double            MaxValue;

  // Description:
  // Degree of Polynomial 1:3
  int               Degree;

  // Description:
  // Mode See Set Mode above
  int RecomputeParameters;

  // Initial set up for the registration at all levels
  virtual int InitializeAll();

  // Description:
  // Optimize for step size = step
  virtual double Optimize(vtkDoubleArray* grad,double old_similarity,double step,int level,int numstep);


  // Description:
  // Evaluate the metric
  virtual double Evaluate();

  // Description:
  // Create Weight Image based on partial voluming
  virtual int CreateWeightImage(vtkImageData* ref);

  // Description:
  // Compute Error Function
  virtual double ComputeErrorFunction(vtkImageData* image,vtkImageData* weights,vtkDoubleArray* coefficients,vtkFloatArray* params);

  // Description:
  // Final set up for the registration at a multiresolution level
  virtual int FinalizeAll();


  // Description:
  // Compute Polynomial Value
  static double ComputePolynomialValue(double x,double x2,double x3,
				       double y,double y2,double y3,
				       double z,double z2,double z3,
				       vtkDoubleArray* coefficients,int degree);

  static double ComputePolynomialValue(double x,double x2,double x3,
				       double y,double y2,double y3,
				       double z,double z2,double z3,
				       double* coefficients,int degree);


  // Description:
  // Generate Output
  virtual void GenerateOutput(int level, int step,int iter,double stepsize,double simil);


  // Description:
  // Needed for Optimizer
  virtual void SetCurrentParameters(vtkDoubleArray* par);
  virtual void GetCurrentParameters(vtkDoubleArray* par);
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);
  virtual double OnBeginIteration(vtkDoubleArray* params);

  // Description:
  int OptimizationMethod;


};


#endif

