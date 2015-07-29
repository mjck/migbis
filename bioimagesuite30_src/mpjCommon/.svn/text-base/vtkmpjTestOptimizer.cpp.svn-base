

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



// vtk includes
#include "pxutil.h"
#include "vtkObjectFactory.h"
#include "vtkmpjTestOptimizer.h"
#include "vtkMath.h"
#include "vtkpxUtil.h"
#include "vtkpxOptimizer.h"

vtkmpjTestOptimizer* vtkmpjTestOptimizer::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjTestOptimizer");
  if(ret)
    {
      return (vtkmpjTestOptimizer*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjTestOptimizer;
}


vtkmpjTestOptimizer::vtkmpjTestOptimizer()
{
  this->OptimizationMethod=0;
  this->FunctionIndex=0;
  this->parameters=vtkDoubleArray::New();
  this->parameters->SetNumberOfComponents(1);
  this->parameters->SetNumberOfTuples(2);
  this->parameters->SetComponent(0,0,5);
  this->parameters->SetComponent(1,0,5);
  this->FunctionEvaluation=0;
  this->DerivEvaluation=0;

}

vtkmpjTestOptimizer::~vtkmpjTestOptimizer()
{
  this->parameters->Delete();
}

//-------------------------------------------------------------------------
void vtkmpjTestOptimizer::SetOutputStatus(const char* line,double v)
{
  fprintf(stderr,"**Opt=%s (%.5f)\n",line,v);
}

//-------------------------------------------------------------------------
double vtkmpjTestOptimizer::CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output)
{
  return ((vtkmpjTestOptimizer*)self)->OptimizerGradient(position,output);
}

double vtkmpjTestOptimizer::CalculateFunction(vtkObject* self,vtkDoubleArray* position)
{
  return ((vtkmpjTestOptimizer*)self)->OptimizerValue(position);
}

void vtkmpjTestOptimizer::OptimizerFeedback(vtkObject* self,const char* line,double val)
{
  ((vtkmpjTestOptimizer*)self)->SetOutputStatus(line,val);
}
//-------------------------------------------------------------------------
void vtkmpjTestOptimizer::SetCurrentParameters(vtkDoubleArray* par)
{
  this->parameters->DeepCopy(par);
}
 
void vtkmpjTestOptimizer::GetCurrentParameters(vtkDoubleArray* par)
{
  par->DeepCopy(this->parameters);
}

double vtkmpjTestOptimizer::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)
{
  if (grad->GetNumberOfTuples()!=params->GetNumberOfTuples())
    grad->SetNumberOfTuples(params->GetNumberOfTuples());

  double x=params->GetComponent(0,0);
  double y=params->GetComponent(1,0);

  double dx=2*(x-1);
  double dy=2*y;
  double s=dx*dx+dy*dy;

  s=sqrt(s);
  grad->SetComponent(0,0,dx/s);
  grad->SetComponent(1,0,dy/s);
  ++this->DerivEvaluation;
  fprintf(stderr,"\t x=%.2f y=%.2f --> dx=%.2f dy=%.2f --> v=[%.2f %.2f]\n",x,y,dx,dy,dx/s,dy/s);
  return (s);

}
 
double vtkmpjTestOptimizer::OptimizerValue(vtkDoubleArray* params)
{
  double x=params->GetComponent(0,0);
  double y=params->GetComponent(1,0);
  double v=(x-1)*(x-1)+y*y;
  ++this->FunctionEvaluation;

  //  fprintf(stderr,"\t x=%.2f y=%.2f --> v=%.2f\n",x,y,v);
  return v;

}

void vtkmpjTestOptimizer::Run()
{
  vtkpxOptimizer* optimizer=vtkpxOptimizer::New();
  vtkDoubleArray* position=vtkDoubleArray::New();
  vtkDoubleArray* temp=vtkDoubleArray::New();
  this->GetCurrentParameters(position);
  optimizer->SetMasterObject(this);

  optimizer->SetEvaluateFunctionMethod(&vtkmpjTestOptimizer::CalculateFunction);
  optimizer->SetEvaluateGradientMethod(&vtkmpjTestOptimizer::CalculateGradient);
  optimizer->SetFeedbackMethod(&vtkmpjTestOptimizer::OptimizerFeedback);

  fprintf(stderr,"Starting meth=%d\n",this->OptimizationMethod);
  this->FunctionEvaluation=0;
  this->DerivEvaluation=0;

  switch (this->OptimizationMethod)
    {
    case -1:
    case 0:
      optimizer->ComputeOldGradientDescent(position,0.5,0.0001);
      break;
    case 2:
      optimizer->ComputeGradientDescent(position,50,0.0001);
      break;
    case 1:
      optimizer->DebugOn();
      optimizer->ComputeSlowClimb(position,0.25,50);
      optimizer->DebugOff();
      break;
    case 3:
      optimizer->ComputePowell(position,temp,50,0.001);
      break;
    case 4:
      optimizer->ComputeConjugateGradient(position,50,0.001);
      break;
    }

  fprintf(stderr,"\n\n Final Output = (%.2f %.2f) func=%.5f eval= f=%d d=%d\n",
	  position->GetComponent(0,0),
	  position->GetComponent(1,0),
	  this->OptimizerValue(position),
	  this->FunctionEvaluation,
	  this->DerivEvaluation);
  optimizer->Delete();
  position->Delete();
  temp->Delete();
  
}
