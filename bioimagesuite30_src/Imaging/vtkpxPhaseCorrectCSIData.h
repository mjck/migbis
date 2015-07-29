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
  Module:    $RCSfile: vtkpxPhaseCorrectCSIData.h,v $
  Language:  C++
  Date:      $Date: 2004/03/23 19:19:45 $
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

#ifndef __VTKPXCOMPUTEFIELDCONSTANT_H
#define __VTKPXCOMPUTEFIELDCONSTANT_H

#include "vtkProcessObject.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"


class vtkpxPhaseCorrectCSIData : public vtkProcessObject {

public:

  vtkTypeMacro(vtkpxPhaseCorrectCSIData,vtkProcessObject);

  // Description:
  // This one also 
  static vtkpxPhaseCorrectCSIData* New();
  
  // Description:
  // These are needed for the Optimizer 
  static void  OptimizerFeedback(vtkObject* self,const char* line,double val);
  static double CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output);
  static double CalculateFunction(vtkObject* self,vtkDoubleArray* position);

  // Description:
  // Optimization Method
  vtkSetMacro(OptimizationMethod,int);
  vtkGetMacro(OptimizationMethod,int);

  
  // Description:
  // Global First order phase correction
  vtkSetMacro(FirstOrderPhase,double);
  vtkGetMacro(FirstOrderPhase,double);


  // Description:
  // Optimization Method Parameters
  vtkSetMacro(Step,double);
  vtkGetMacro(Step,double);

  vtkSetMacro(Tolerance,double);
  vtkGetMacro(Tolerance,double);

  vtkSetMacro(Iterations,int);
  vtkGetMacro(Iterations,int);

  vtkSetMacro(OptimizeZeroOrder,int);
  vtkGetMacro(OptimizeZeroOrder,int);


  // Description:
  // Shorthand for different optimization methods 
  virtual void SetOptimizationMethodToOldStyle() { this->SetOptimizationMethod(-1); }
  virtual void SetOptimizationMethodToDefault() { this->SetOptimizationMethod(0); }
  virtual void SetOptimizationMethodToSlowClimb() { this->SetOptimizationMethod(1); }
  virtual void SetOptimizationMethodToGradientDescent() { this->SetOptimizationMethod(2); }

  // Description:
  // Run Optimization
  virtual void Run(vtkImageData* real,vtkImageData* imag, vtkImageData* outreal,vtkImageData* outimag);
  virtual void RunFirstOrderPhase(vtkImageData* real,vtkImageData* imag, vtkImageData* outreal,vtkImageData* outimag);
  virtual void RunZeroOrderPhase(vtkImageData* real,vtkImageData* imag, vtkImageData* outreal,vtkImageData* outimag);


  
  // Description:
  // Load Brucker Data
  static int ReadFIDFile(const char* fname,int width,int height,int depth,int components,
			 vtkImageData* Real,vtkImageData* Imaginary);


  // Description:
  // Save FID Stuff
  static int SaveFIDFile(const char* fname,vtkImageData* Real,vtkImageData* Imaginary,int mode);
  static int SaveIntegralFIDFile(const char* fname,vtkImageData* Real,vtkImageData* Imaginary);

  // Description:
  // Extract Single Row for Plotting (x_const,y_const,z_const,c) --> (0,0,c)
  static int ExtractSingleVoxel(vtkImageData* Real,vtkImageData* Imaginary,vtkImageData* OutReal,vtkImageData* OutImag,vtkImageData* Magn,
				int x,int y, int z);
  static int ExtractSingleVoxelAsComponent(vtkImageData* Real,vtkImageData* Imaginary,vtkImageData* OutReal,vtkImageData* OutImag,
					   int x,int y, int z);


  // Description:
  // Add Phase Offsets to Image 
  static double AddPhaseOffsets(vtkImageData* realimg,vtkImageData* imagimg,double theta0,double theta1);
  static double GlobalPhaseCorrection(vtkImageData* realimg,vtkImageData* imagimg,double theta1);

  // Description:
  // Access Parameters 
  virtual void  SetCurrentParameters(vtkDoubleArray* par);
  virtual void  GetCurrentParameters(vtkDoubleArray* par);

  // Description:
  // FirstOrderPhases
  vtkGetObjectMacro(FirstOrderPhases,vtkDoubleArray);
  vtkGetObjectMacro(ZeroOrderPhases,vtkDoubleArray);


  // Description:
  // Integrate 
  static void IntegrateReal(vtkImageData* realimg,vtkImageData* output,
			    double water1,double water2,
			    double sat1,double sat2,
			    double unsat1,double unsat2);

  static void IntegrateMagnitude(vtkImageData* realimg,vtkImageData* imagimg,vtkImageData* output,
				 double water1,double water2,
				 double sat1,double sat2,
				 double unsat1,double unsat2);


  static void IntegrateMagnitudeAndComputeRatio(vtkImageData* realimg,vtkImageData* imagimg,
						vtkImageData* output,
						double waterwidth=0.02);
						  

  static void IntegrateMagnitudeGlobal(vtkImageData* realimg,vtkImageData* imagimg,
				       vtkImageData* output,
				       double water_peak,double fat_peak,double scalefactor=1.0);
						  

  static void GenerateLocalPeaks(vtkImageData* realimg,vtkImageData* imagimg,
				 vtkImageData* output,
				 double water_peak,double fat_peak,double sigma=1.0);

  static void IntegrateMagnitudeLocal(vtkImageData* realimg,vtkImageData* imagimg,
				      vtkImageData* output,vtkDoubleArray* volint,
				      double water_peak,double fat_peak,double scalefactor=1.0);
						  


protected:

  // Description:
  // Make all these protected 
  // Constructor
  vtkpxPhaseCorrectCSIData();
  virtual ~vtkpxPhaseCorrectCSIData();
  vtkpxPhaseCorrectCSIData(const vtkpxPhaseCorrectCSIData&) {};
  void operator=(const vtkpxPhaseCorrectCSIData&) {};


  // Description:
  // Various Control Parameters
  int   OptimizationMethod;
  vtkDoubleArray* parameters;
  vtkDoubleArray* FirstOrderPhases;
  vtkDoubleArray* ZeroOrderPhases;
  int   FunctionEvaluation;
  int   DerivEvaluation;
  double Step;
  double Tolerance;
  int   Iterations;
  int   OptimizeZeroOrder;
  double FirstOrderPhase;

  // Description:
  // Image Pointers
  vtkImageData* InputReal;
  vtkImageData* InputImag;
  vtkImageData* OutputReal;
  vtkImageData* OutputImag;
  

  // Description:
  // Needed for Optimizer -- set,get,optimizer,optimizervalue
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);
  virtual void  SetOutputStatus(const char* line,double v);

  virtual double CorrectVoxel(vtkImageData* inreal,vtkImageData* inimag,
			     vtkImageData* outreal,vtkImageData* outimag,
			     int x,int y,int z,double p0,double p1,int index);

  virtual double GenerateOutputImages(vtkImageData* inreal,vtkImageData* inimag,
				     vtkImageData* outreal,vtkImageData* outimag,
				     vtkDoubleArray* parameters);

  virtual double GenerateFirstOrderPhaseOutputImages(vtkImageData* inreal,vtkImageData* inimag,
						    vtkImageData* outreal,vtkImageData* outimag,
						    vtkDoubleArray* p1);

  virtual double GenerateZeroAndFirstOrderPhaseOutputImages(vtkImageData* inreal,vtkImageData* inimag,
							   vtkImageData* outreal,vtkImageData* outimag,
							   vtkDoubleArray* p0,
							   vtkDoubleArray* p1);

  virtual void GenerateInputs(vtkImageData* real,vtkImageData* imag, vtkImageData* outreal,vtkImageData* outimag);
};

#endif


