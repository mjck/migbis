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

// vtk includes
#include "vtkObjectFactory.h"
#include "vtkpxTestOptimizer.h"
#include "vtkMath.h"
#include "vtkpxOptimizer.h"

vtkpxTestOptimizer* vtkpxTestOptimizer::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxTestOptimizer");
  if(ret)
    {
      return (vtkpxTestOptimizer*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxTestOptimizer;
}


vtkpxTestOptimizer::vtkpxTestOptimizer()
{
  this->OptimizationMethod=0;
  this->parameters=vtkDoubleArray::New();
  this->parameters->SetNumberOfComponents(1);
  this->parameters->SetNumberOfTuples(2);
  this->parameters->SetComponent(0,0,5);
  this->parameters->SetComponent(1,0,5);
  this->FunctionEvaluation=0;
  this->DerivEvaluation=0;
  this->Dimension=2;
}

vtkpxTestOptimizer::~vtkpxTestOptimizer()
{
  this->parameters->Delete();
}

//-------------------------------------------------------------------------
void vtkpxTestOptimizer::SetOutputStatus(const char* line,double v)
{
  fprintf(stdout,"**Opt=%s (%.5f)\n",line,v);
}

//-------------------------------------------------------------------------
double vtkpxTestOptimizer::CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output)
{
  return ((vtkpxTestOptimizer*)self)->OptimizerGradient(position,output);
}

double vtkpxTestOptimizer::CalculateFunction(vtkObject* self,vtkDoubleArray* position)
{
  return ((vtkpxTestOptimizer*)self)->OptimizerValue(position);
}

void vtkpxTestOptimizer::OptimizerFeedback(vtkObject* self,const char* line,double val)
{
  ((vtkpxTestOptimizer*)self)->SetOutputStatus(line,val);
}
//-------------------------------------------------------------------------
void vtkpxTestOptimizer::SetCurrentParameters(vtkDoubleArray* par)
{
  this->parameters->DeepCopy(par);
}
 
void vtkpxTestOptimizer::GetCurrentParameters(vtkDoubleArray* par)
{
  par->DeepCopy(this->parameters);
}

double vtkpxTestOptimizer::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)
{
  if (grad->GetNumberOfTuples()!=params->GetNumberOfTuples())
    grad->SetNumberOfTuples(params->GetNumberOfTuples());

  if (this->Dimension==2)
    {
      double x=params->GetComponent(0,0);
      double y=params->GetComponent(1,0);
      
      double dx=2*(x-9);
      double dy=2*y;
      double s=dx*dx+dy*dy;
      
      s=sqrt(s);
      grad->SetComponent(0,0,dx/s);
      grad->SetComponent(1,0,dy/s);
      ++this->DerivEvaluation;
      fprintf(stdout,"\t x=%.2f y=%.2f --> dx=%.2f dy=%.2f --> v=[%.2f %.2f]\n",x,y,dx,dy,dx/s,dy/s);
      return (s);
    }
  else
    {
      double x=params->GetComponent(0,0);
      double dx=2*(x-9);
      grad->SetComponent(0,0,dx);
      ++this->DerivEvaluation;
      fprintf(stdout,"\t x=%.2f  --> dx=%.2f  --> v=[%.2f]\n",x,dx,dx);
      return fabs(dx);
    }

}
 
double vtkpxTestOptimizer::OptimizerValue(vtkDoubleArray* params)
{
  double x=params->GetComponent(0,0);
  double v=0.0,y=0.0;
  if (this->Dimension==2)
    y=params->GetComponent(1,0);
  v=(x-9)*(x-9)+y*y;
  ++this->FunctionEvaluation;

  //  fprintf(stdout,"\t x=%.2f y=%.2f --> v=%.2f\n",x,y,v);
  return v;

}

int vtkpxTestOptimizer::Run()
{
  vtkpxOptimizer* optimizer=vtkpxOptimizer::New();
  this->parameters->SetNumberOfTuples(this->Dimension);
  vtkDoubleArray* position=vtkDoubleArray::New();
  vtkDoubleArray* temp=vtkDoubleArray::New();
  this->GetCurrentParameters(position);
  optimizer->SetMasterObject(this);

  optimizer->SetEvaluateFunctionMethod(&vtkpxTestOptimizer::CalculateFunction);
  optimizer->SetEvaluateGradientMethod(&vtkpxTestOptimizer::CalculateGradient);
  optimizer->SetFeedbackMethod(&vtkpxTestOptimizer::OptimizerFeedback);

  fprintf(stdout,"Starting meth=%d\n",this->OptimizationMethod);
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
    case 4:
      optimizer->ComputeConjugateGradient(position,50,0.001);
      break;

    case 5:
      if (this->Dimension==1)
	optimizer->Compute1DMinimization(position,-25,25,0.01);
      break;
    }


  float error=0.0;
  if (this->Dimension==2)
    {
      fprintf(stdout,"\n\n Final Output = (%.2f %.2f) func=%.5f eval= f=%d d=%d\n",
	      position->GetComponent(0,0),
	      position->GetComponent(1,0),
	      this->OptimizerValue(position),
	      this->FunctionEvaluation,
	      this->DerivEvaluation);
      error+=pow(position->GetComponent(0,0)-9.0,2.0)+pow(position->GetComponent(1,0),2.0);
    }
  else
    {
      fprintf(stdout,"\n\n Final Output = (%.2f) func=%.5f eval= f=%d d=%d\n",
	      position->GetComponent(0,0),
	      this->OptimizerValue(position),
	      this->FunctionEvaluation,
	      this->DerivEvaluation);
      error+=pow(position->GetComponent(0,0)-9.0,2.0);
    }
  optimizer->Delete();
  position->Delete();
  temp->Delete();
  if (error<0.001)
    return 1;
  return 0;
}

