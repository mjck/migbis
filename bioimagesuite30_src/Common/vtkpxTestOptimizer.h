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
//
//
//
//---------------------------------------------------------------------------------------------------------



#ifndef __VTKPXREGISTRATION_H
#define __VTKPXREGISTRATION_H

#include "vtkProcessObject.h"
#include "vtkDoubleArray.h"

class vtkpxTestOptimizer : public vtkProcessObject {

public:

  vtkTypeMacro(vtkpxTestOptimizer,vtkProcessObject);

  // Description:
  // This one also 
  static vtkpxTestOptimizer* New();
  
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
  // Optimization Method
  vtkSetMacro(Dimension,int);
  vtkGetMacro(Dimension,int);


  // Description:
  // Shorthand for different optimization methods 
  virtual void SetOptimizationMethodToOldStyle() { this->SetOptimizationMethod(-1); }
  virtual void SetOptimizationMethodToDefault() { this->SetOptimizationMethod(0); }
  virtual void SetOptimizationMethodToSlowClimb() { this->SetOptimizationMethod(1); }
  virtual void SetOptimizationMethodToGradientDescent() { this->SetOptimizationMethod(2); }
  virtual void SetOptimizationMethodToConjugateGradient() { this->SetOptimizationMethod(4); }

  // Description:
  // Run Test
  virtual int Run();

protected:

  // Description:
  // Make all these protected 
  // Constructor
  vtkpxTestOptimizer();
  virtual ~vtkpxTestOptimizer();
  vtkpxTestOptimizer(const vtkpxTestOptimizer&) {};
  void operator=(const vtkpxTestOptimizer&) {};


  // Description:
  // Various Control Parameters
  int   OptimizationMethod;

  vtkDoubleArray* parameters;
  int   FunctionEvaluation;
  int   DerivEvaluation;
  int   Dimension;

  // Description:
  // Needed for Optimizer -- set,get,optimizer,optimizervalue
  virtual void SetCurrentParameters(vtkDoubleArray* par);
  virtual void GetCurrentParameters(vtkDoubleArray* par);
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);
  virtual void  SetOutputStatus(const char* line,double v);
};



#endif


