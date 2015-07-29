

//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
//BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


#ifndef __vtkmpjTestOptimizer_h
#define __vtkmpjTestOptimizer_h

#include "vtkProcessObject.h"
#include "vtkDoubleArray.h"

class vtkmpjTestOptimizer : public vtkProcessObject {

public:

  vtkTypeMacro(vtkmpjTestOptimizer,vtkProcessObject);

  // Description:
  // This one also 
  static vtkmpjTestOptimizer* New();
  
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
  vtkSetMacro(FunctionIndex,int);
  vtkGetMacro(FunctionIndex,int);

  // Description:
  // Shorthand for different optimization methods 
  virtual void SetOptimizationMethodToOldStyle() { this->SetOptimizationMethod(-1); }
  virtual void SetOptimizationMethodToDefault() { this->SetOptimizationMethod(0); }
  virtual void SetOptimizationMethodToSlowClimb() { this->SetOptimizationMethod(1); }
  virtual void SetOptimizationMethodToGradientDescent() { this->SetOptimizationMethod(2); }

  // Description:
  // Run Test
  virtual void Run();

protected:

  // Description:
  // Make all these protected 
  // Constructor
  vtkmpjTestOptimizer();
  virtual ~vtkmpjTestOptimizer();
  vtkmpjTestOptimizer(const vtkmpjTestOptimizer&) {};
  void operator=(const vtkmpjTestOptimizer&) {};


  // Description:
  // Various Control Parameters
  int   OptimizationMethod;
  int   FunctionIndex;
  vtkDoubleArray* parameters;
  int   FunctionEvaluation;
  int   DerivEvaluation;

  // Description:
  // Needed for Optimizer -- set,get,optimizer,optimizervalue
  virtual void SetCurrentParameters(vtkDoubleArray* par);
  virtual void GetCurrentParameters(vtkDoubleArray* par);
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);
  virtual void  SetOutputStatus(const char* line,double v);
};



#endif

