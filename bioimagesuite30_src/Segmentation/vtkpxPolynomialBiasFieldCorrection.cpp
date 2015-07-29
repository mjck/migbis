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
  Module:    $RCSfile: vtkpxPolynomialBiasFieldCorrection.cpp,v $
  Language:  C++
  Date:      $Date: 2004/12/13 19:46:32 $
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
#include "vtkpxPolynomialBiasFieldCorrection.h"
#include "vtkObjectFactory.h"
#include "pxutil.h"
#include "vtkpxUtil.h"
#include "vtkbisImageReslice.h"
#include "vtkpxOptimizer.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkImageThreshold.h"
#include "vtkImageSeedConnectivity.h"
#include "vtkpxMath.h"
#include "vtkpxImageExtractVOI.h"
#include "vtkDoubleArray.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageMedian3D.h"
#include "vtkImageMathematics.h"
#include "pxisinf.h"


vtkpxPolynomialBiasFieldCorrection* vtkpxPolynomialBiasFieldCorrection::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxPolynomialBiasFieldCorrection");
  if(ret)
    {
    return (vtkpxPolynomialBiasFieldCorrection*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxPolynomialBiasFieldCorrection;
  
}


vtkpxPolynomialBiasFieldCorrection::vtkpxPolynomialBiasFieldCorrection()
{
  this->FixedNumberOfSlices=0;
  this->NumberOfIterations=10;
  this->Coefficients=vtkDoubleArray::New();
  this->Epsilon=0.003;
  this->RecomputeParameters=0;
  this->Degree=2;
  this->Metric=0;
  this->InitialCoefficients=NULL;
  this->OptimizationMethod=0;
  this->MinValue=0.5;
  this->MaxValue=2.5;
  strcpy(this->OutputStatus,"");
  this->TempBiasField=NULL;
  this->TempFixedImage=NULL;
}

vtkpxPolynomialBiasFieldCorrection::~vtkpxPolynomialBiasFieldCorrection() 
{
  if (this->Coefficients!=NULL)
    this->Coefficients->Delete();

  this->SetInitialCoefficients(NULL);

  if (this->TempBiasField!=NULL)
    this->TempBiasField->Delete();

  if (this->TempFixedImage!=NULL)
    this->TempFixedImage->Delete();
}

//-------------------------------------------------------------------------
int vtkpxPolynomialBiasFieldCorrection::InitializeAll() 
{
  strcpy(this->OutputStatus,"Initializing");
  if (this->StepSize>0.5)
    this->StepSize=0.5;
  this->InternalStepSize=Frange(this->StepSize,0.01,0.5);

  vtkpxAbstractBiasFieldCorrection::InitializeAll();
  switch (this->Degree)
    {
    case 3:
      this->Coefficients->SetNumberOfTuples(19);
      break;
    case 2:
      this->Coefficients->SetNumberOfTuples(9);
      break;
    default:
      this->Coefficients->SetNumberOfTuples(3);
      break;
    }


  this->Coefficients->FillComponent(0,0.0);

  if (this->InitialCoefficients!=NULL)
    {
      int nt=this->InitialCoefficients->GetNumberOfTuples();
      if (nt>this->Coefficients->GetNumberOfTuples())
	nt=this->Coefficients->GetNumberOfTuples();
      
      for (int i=0;i<nt;i++)
	this->Coefficients->SetComponent(i,0,this->InitialCoefficients->GetComponent(i,0));
    }


  return 1;
}
//-------------------------------------------------------------------------
int vtkpxPolynomialBiasFieldCorrection::FinalizeAll()
{
  this->Evaluate();
  this->GenerateOutput(-1,-1,-1,-1,this->LastSimilarity);
  return 1;
}
//-------------------------------------------------------------------------
// Compute Registration
int vtkpxPolynomialBiasFieldCorrection::ComputeBiasFit(int level)
{  
  
  double step=this->CurrentStepSize;

  vtkDoubleArray* grad=vtkDoubleArray::New();
  grad->SetNumberOfComponents(1);
  grad->SetNumberOfTuples(this->Coefficients->GetNumberOfTuples());

  for (int st=1;st<=this->NumberOfSteps;st++)
    {
      double old_similarity = this->Evaluate();
      double new_similarity=this->Optimize(grad,old_similarity,step,level,st);
      this->GenerateOutput(level,st,this->NumberOfIterations,step,new_similarity);
      step*=0.5;
    }

  grad->Delete();
  return 1;
}
//-------------------------------------------------------------------------

double vtkpxPolynomialBiasFieldCorrection::Optimize(vtkDoubleArray* grad,double old_similarity,
						    double step,int level,int numstep)

{
  OptimizationStructure.SetValues(old_similarity,step,level);
     
  vtkpxOptimizer* optimizer=vtkpxOptimizer::New();
  vtkDoubleArray* position=vtkDoubleArray::New();
  vtkDoubleArray* temp=vtkDoubleArray::New();

  this->GetCurrentParameters(position);
  optimizer->SetMasterObject(this);
  optimizer->SetTempArray(grad);
  optimizer->SetEvaluateFunctionMethod(&vtkpxPolynomialBiasFieldCorrection::CalculateFunction);
  optimizer->SetEvaluateGradientMethod(&vtkpxPolynomialBiasFieldCorrection::CalculateGradient);
  optimizer->SetBeginIterationMethod(&vtkpxAbstractBiasFieldCorrection::OnBeginIteration);
  optimizer->SetFeedbackMethod(&vtkpxPolynomialBiasFieldCorrection::OptimizerFeedback);
  
  double v1=this->Evaluate();
  double tol=this->Epsilon*0.01*fabs(v1);
  
  int olddegree=this->Degree;
  /*  this->Degree=1;
  if (level==1 && st==this->NumberOfSteps)
  this->Degree=olddegree;*/

  double v;

  switch (this->OptimizationMethod)
    {
    case 2:
      v=optimizer->ComputePowell(position,temp,this->NumberOfIterations,tol);
      break;
      
    case 3:
      v=optimizer->ComputeGradientDescent(position,this->NumberOfIterations,tol);
      break;

    case 4:
      v=optimizer->ComputeConjugateGradient(position,this->NumberOfIterations,tol);
      break;

    default:
      v=optimizer->ComputeConjugateGradient(position,this->NumberOfIterations,tol);
    }


  optimizer->Delete();
  position->Delete();
  temp->Delete();
  return v;
}


//-------------------------------------------------------------------------
double vtkpxPolynomialBiasFieldCorrection::ComputePolynomialValue(double x,double x2,double x3,
								  double y,double y2,double y3,
								  double z,double z2,double z3,
								  vtkDoubleArray* coefficients,int degree)
{
  double value= 
    coefficients->GetComponent(0,0)*x+
    coefficients->GetComponent(1,0)*y+
    coefficients->GetComponent(2,0)*z;

  if (degree==1)
    return value;

  value+= 
    0.5*coefficients->GetComponent(3,0)*(3*x2-1.0)+
    0.5*coefficients->GetComponent(4,0)*(3*y2-1.0)+
    0.5*coefficients->GetComponent(5,0)*(3*z2-1.0)+
    coefficients->GetComponent(6,0)*x*y+
    coefficients->GetComponent(7,0)*x*z+
    coefficients->GetComponent(8,0)*y*z;
  
  if (degree==2)
    return value;

  value+=
    0.5*(
	 coefficients->GetComponent( 9,0)*(5.0*x3-3.0*x)+
	 coefficients->GetComponent(10,0)*(5.0*y3-3.0*y)+
	 coefficients->GetComponent(11,0)*(5.0*z3-3.0*z)+
	 coefficients->GetComponent(12,0)*(3*x2-1.0)*y+
	 coefficients->GetComponent(13,0)*(3*x2-1.0)*z+
	 coefficients->GetComponent(14,0)*(3*y2-1.0)*x+
	 coefficients->GetComponent(15,0)*(3*y2-1.0)*z+
	 coefficients->GetComponent(16,0)*(3*z2-1.0)*x+
	 coefficients->GetComponent(17,0)*(3*z2-1.0)*y
	 )+
    coefficients->GetComponent(18,0)*x*y*z;

  return value;

}
//-------------------------------------------------------------------------
double vtkpxPolynomialBiasFieldCorrection::ComputePolynomialValue(double x,double x2,double x3,
								  double y,double y2,double y3,
								  double z,double z2,double z3,
								  double* coefficients,int degree)
{
  double value= 
    coefficients[0]*x+
    coefficients[1]*y+
    coefficients[2]*z;

  if (degree==1)
    return value;

  value+= 
    0.5*coefficients[3]*(3*x2-1.0)+
    0.5*coefficients[4]*(3*y2-1.0)+
    0.5*coefficients[5]*(3*z2-1.0)+
    coefficients[6]*x*y+
    coefficients[7]*x*z+
    coefficients[8]*y*z;
  
  if (degree==2)
    return value;

  value+=
    0.5*(
	 coefficients[ 9]*(5.0*x3-3.0*x)+
	 coefficients[10]*(5.0*y3-3.0*y)+
	 coefficients[11]*(5.0*z3-3.0*z)+
	 coefficients[12]*(3*x2-1.0)*y+
	 coefficients[13]*(3*x2-1.0)*z+
	 coefficients[14]*(3*y2-1.0)*x+
	 coefficients[15]*(3*y2-1.0)*z+
	 coefficients[16]*(3*z2-1.0)*x+
	 coefficients[17]*(3*z2-1.0)*y
	 )+
    coefficients[18]*x*y*z;

  return value;

}

//-------------------------------------------------------------------------

vtkImageData* vtkpxPolynomialBiasFieldCorrection::ComputeBiasFieldImage(vtkImageData* ref,vtkDoubleArray* tr,int degree,double minvalue,double maxvalue)
{
  if (ref==NULL || tr==NULL)
    return NULL;

 if (tr->GetNumberOfTuples()<3)
	return NULL;

  vtkImageData* bfield=vtkImageData::New();
  bfield->CopyStructure(ref);
  bfield->SetNumberOfScalarComponents(1);
  bfield->SetScalarTypeToFloat();
  bfield->AllocateScalars();

  int ok=vtkpxPolynomialBiasFieldCorrection::ComputeBiasFieldImage(ref,bfield,tr,degree,minvalue,maxvalue);
  //  fprintf(stderr,"ok=%d\n",ok);
  return bfield;
}


// --------------------------------------------------------------------------------------------------------------------------------------------

int vtkpxPolynomialBiasFieldCorrection::ComputeBiasFieldImage(vtkImageData* ref,vtkImageData* bfield,vtkDoubleArray* tr,int degree,double minvalue,double maxvalue)
{
  double ori[3],spa[3];
  int    dim[3];

  ref->GetSpacing(spa);
  ref->GetOrigin(ori);
  ref->GetDimensions(dim);

  if (bfield==NULL)
    {
      fprintf(stderr, " ************************** Big Problem No Bias Field ***************************\n");
      return 0;
    }

  int nt1=ref->GetPointData()->GetScalars()->GetNumberOfTuples();
  int nt2=bfield->GetPointData()->GetScalars()->GetNumberOfTuples();

  if (nt1!=nt2)
    {
      fprintf(stderr, " ************************** Allocating Bias Field ***************************\n");
      bfield->CopyStructure(ref);
      bfield->SetNumberOfScalarComponents(1);
      bfield->SetScalarTypeToFloat();
      bfield->AllocateScalars();
    }
  //  else
    //      fprintf(stderr,"nt1=nt2=%d\n",nt1);

  if (degree==-1)
    {
      if (tr->GetNumberOfTuples()==9)
	degree=2;
      else if (tr->GetNumberOfTuples()==19)
	degree=3;
      else
	degree=1;
    }
  else
    {
      if (degree==3 && tr->GetNumberOfTuples()<19)
	degree=2;

      if (degree ==2 && tr->GetNumberOfTuples()<9)
	degree=1;
    }

  double center[3],radius[3];
  for (int ia=0;ia<=2;ia++)
    {
      radius[ia]=0.5*double(dim[ia])*spa[ia];
      center[ia]=radius[ia];
      if (radius[ia]<0.001)
	radius[ia]=1.0;
    }

  vtkDataArray* bfield_dat=bfield->GetPointData()->GetScalars();
  int index=0;

  //  fprintf(stderr,"This far nt=%d vs %d x %d x %d\n",bfield_dat->GetNumberOfTuples(),dim[0],dim[1],dim[2]);

  double *coeff = (double*) tr->GetVoidPointer(0);
  int count=0;
  for (int k=0;k<dim[2];k++)
    {
      double z=(double(k)*spa[2]-center[2])/radius[2];
      double z2=z*z;
      double z3=z2*z;
      for (int j=0;j<dim[1];j++)
	{
	  double y=(double(j)*spa[1]-center[1])/radius[1];
	  double y2=y*y;
	  double y3=y2*y;

	  for (int i=0;i<dim[0];i++)
	    {
	      double x=(double(i)*spa[0]-center[0])/radius[0];
	      double x2=x*x;
	      double x3=x2*x;
	      
	      double s=vtkpxPolynomialBiasFieldCorrection::ComputePolynomialValue(x,x2,x3,y,y2,y3,z,z2,z3,coeff,degree)+1.0;
	      if (s<minvalue)
		s=minvalue;
	      else if (s>maxvalue)
		s=maxvalue;
	      bfield_dat->SetComponent(index,0,s);
	      ++index;
	    }
	}
      ++count;
      if (count==20)
	{
	  //	  fprintf(stderr,"Done with slice %d\n",k);
	  count=0;
	}
    }
	
  //  fprintf(stderr,"Done\n");
  
  return 1;

}
//-------------------------------------------------------------------------
double vtkpxPolynomialBiasFieldCorrection::ComputeErrorFunction(vtkImageData* image,vtkImageData* weights,
								vtkDoubleArray* coefficients,vtkFloatArray* params)
{
  if (this->TempBiasField==NULL)
    {
      this->TempBiasField=vtkImageData::New();
      this->TempBiasField->CopyStructure(image);
      this->TempBiasField->SetNumberOfScalarComponents(1);
      this->TempBiasField->AllocateScalars();
    }

  if (this->TempFixedImage==NULL)
    {
      this->TempFixedImage=vtkImageData::New();
      this->TempFixedImage->CopyStructure(image);
      this->TempFixedImage->SetNumberOfScalarComponents(image->GetNumberOfScalarComponents());
      this->TempFixedImage->AllocateScalars();
    }


  this->ComputeBiasFieldImage(image,this->TempBiasField,coefficients,this->Degree,this->MinValue,this->MaxValue);
  vtkpxAbstractBiasFieldCorrection::ComputeCorrectedImage(image,this->TempBiasField,this->TempFixedImage,1);



  double metric=0;
  vtkDataArray* wg=NULL;
  if (weights!=NULL)
    wg=weights->GetPointData()->GetScalars();

  if (this->Metric==0)
    {
      vtkDataArray* img=this->TempFixedImage->GetPointData()->GetScalars();
      int nt=img->GetNumberOfTuples();
      int nc=params->GetNumberOfTuples();
      
      double sum=0.0,nsum=0.0001;
      double scale_in=vtkpxAbstractBiasFieldCorrection::ComputeScaleFactor(image,this->TempBiasField);

      for (int i=0;i<nt;i++)
	{
	  double v=img->GetComponent(i,0)/scale_in;
	  double p=1.0;

	  for (int c=0;c<nc;c++)
	    {
	      double q=Valley(v-params->GetComponent(c,1),params->GetComponent(c,2));
	      if (!isnan(q))
		p=p*q;
	    }
	  
	  if (wg!=NULL)
	    {
	      sum+=wg->GetComponent(i,0)*p;
	      nsum+=wg->GetComponent(i,0);
	    }
	  else
	    {
	      sum+=p;
	      nsum+=1.0;
	    }
	}
      metric=sum/nsum;
    }
  else if (this->Metric==1)
    {
      double range[2]; this->TempFixedImage->GetPointData()->GetScalars()->GetRange(range);
      double lower=range[0];
      double upper=range[1];

      vtkpxImageRobustHistogram* hist=vtkpxImageRobustHistogram::New();
      hist->InterpolationOff();
      hist->DebugOff();
      hist->SetNumberOfBins(1024);
      hist->RobustModeOff();
      if (weights!=NULL)
	hist->SetWeightImage(weights);
      hist->SetInput(this->TempFixedImage);
      hist->Update();
      metric=-hist->ComputeEntropy();
      hist->Delete();
    }
  else if (this->Metric==2)
    {
      vtkImageMedian3D* smooth=vtkImageMedian3D::New();
      smooth->SetInput(this->TempFixedImage);
      smooth->SetKernelSize(3,3,3);
      smooth->Update();


      vtkImageMathematics* math=vtkImageMathematics::New();
      math->AddInput(smooth->GetOutput());
      math->AddInput(this->TempFixedImage);
      math->SetOperationToSubtract();
      math->Update();

      vtkDataArray* img=math->GetOutput()->GetPointData()->GetScalars();
      int nt=img->GetNumberOfTuples();
      int nc=img->GetNumberOfComponents();
      double range[2]; img->GetRange(range);

      double sum=0.0,sum2=0.0,nsum=0.0001;
      for (int i=0;i<nt;i++)
	{
	  double w=1.0;
	  if (wg!=NULL)
	    w=wg->GetComponent(i,0);

	  for (int j=0;j<nc;j++)
	    {
	      double p=fabs(img->GetComponent(i,j));
	      sum+=p*w;
	      nsum+=w;
	    }
	}
      metric=-sum/nsum;
      math->Delete();
      smooth->Delete();
    }
  return metric;

}
//-------------------------------------------------------------------------
float vtkpxPolynomialBiasFieldCorrection::ComputeEntropy(vtkImageData* img)
{
  double range[2]; img->GetPointData()->GetScalars()->GetRange(range);
  double lower=range[0];//0.99*range[0]+0.01*range[1];
  double upper=range[1];//0.01*range[0]+0.99*range[1];

  vtkpxImageRobustHistogram* hist=vtkpxImageRobustHistogram::New();
  hist->SetInput(img);
  hist->SaturationModeOff();
  hist->InterpolationOn();
  hist->RobustModeOff();
  hist->SetNumberOfBins(512);
  hist->Update();
  double metric=hist->ComputeEntropy();
  hist->Delete();
  return metric;
}

//-------------------------------------------------------------------------
double vtkpxPolynomialBiasFieldCorrection::Evaluate()
{
  this->LastSimilarity=this->ComputeErrorFunction(this->SampledReferenceImage,this->SampledWeightImage,
						  this->Coefficients,this->ClassParameters);
  return (this->LastSimilarity);
}
//-------------------------------------------------------------------------
void vtkpxPolynomialBiasFieldCorrection::GenerateOutput(int level, int step,int iter,double stepsize,double actualSimilarity)
{
  if (iter>=100)
    iter=99;

  sprintf(this->OutputStatus,"Level=%2d Step=%2d iter=%2d stepsize=%.3f:\t",
	  level,step,iter,stepsize);

  for (int i=0;i<this->Coefficients->GetNumberOfTuples();i++)
    sprintf(this->OutputStatus,"%s %.5f ", this->OutputStatus,this->Coefficients->GetComponent(i,0));

  sprintf(this->OutputStatus,"%s \t sim=%8.6f",this->OutputStatus, actualSimilarity);
  //  this->UpdateProgress(this->LastProgress);
  
  double sc=1.0/double(this->NumberOfLevels);
  double pog=(double(step)/double(this->NumberOfSteps+1) + double(this->NumberOfLevels-level))*sc;
  this->LastProgress=pog;
  //  this->UpdateProgress(pog);
  //  fprintf(stderr,"%s\n",this->OutputStatus);
  return;
}

//-------------------------------------------------------------------------
// Optimizer Stuff 
//-------------------------------------------------------------------------
double vtkpxPolynomialBiasFieldCorrection::OnBeginIteration(vtkDoubleArray* params)
{
  if (this->Metric==0 && this->RecomputeParameters==1)
    {
      sprintf(this->OutputStatus,"On Begin Iteration ... Computing Parameters ...\n");
      this->UpdateProgress(this->LastProgress);
      
      vtkImageData* bfield=this->ComputeBiasFieldImage(this->SampledReferenceImage,this->Coefficients);
      vtkImageData* tmp=ComputeCorrectedImage(this->SampledReferenceImage,bfield,1);
      this->ComputeAndSetClassParameters(tmp,this->ClassParameters);
      tmp->Delete();
      bfield->Delete();
      
      
      this->LastProgress+=0.001;
      sprintf(this->OutputStatus,"On Begin Iteration ... Computing Parameters Done\n");
      this->UpdateProgress(this->LastProgress);
    }
  return 0.0;
}
// -----------------------------------------------------------------------------------------------
void vtkpxPolynomialBiasFieldCorrection::SetCurrentParameters(vtkDoubleArray* param)
{
  int numt=this->Coefficients->GetNumberOfTuples();
  int num=param->GetNumberOfTuples();

  if (num!=numt)
    {
      vtkErrorMacro(<<"Bad Data for Optimization");
      fprintf(stderr,"Bad Data for Optimization\n");
      return;
    }

  for (int i=0;i<numt;i++)
    {
      double v=param->GetComponent(i,0);
      if (v<-2.0)
	v=2.0;
      else if (v>2.0)
	v=2.0;
      this->Coefficients->SetComponent(i,0,v);
    }
}
//-------------------------------------------------------------------------
void vtkpxPolynomialBiasFieldCorrection::GetCurrentParameters(vtkDoubleArray* param)
{
  param->DeepCopy(this->Coefficients);
}
//-------------------------------------------------------------------------
double vtkpxPolynomialBiasFieldCorrection::OptimizerValue(vtkDoubleArray* params)
{
  this->SetCurrentParameters(params);
  return -this->Evaluate();
}
//-------------------------------------------------------------------------
double vtkpxPolynomialBiasFieldCorrection::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)

{
  if (grad->GetNumberOfComponents()!=1 || grad->GetNumberOfTuples()!=this->Coefficients->GetNumberOfTuples())
    {
      grad->SetNumberOfComponents(1);
      grad->SetNumberOfTuples(this->Coefficients->GetNumberOfTuples());
    }

  OptimizationStructure.old_similarity=this->OptimizerValue(params);
  this->GenerateOutput(OptimizationStructure.level,-1,-1,OptimizationStructure.step,OptimizationStructure.old_similarity);

  // Calculate gradient 
  double GradientNorm = 0;

  grad->FillComponent(0,0.0);

  
  this->GenerateOutput(OptimizationStructure.level,
		       -1,-1,OptimizationStructure.step,
		       OptimizationStructure.old_similarity);
  fprintf(stderr,"\n\n%s\nOptimizer Computing Gradients : (%5.3f,%d) ... ",
	  OutputStatus,OptimizationStructure.step,
	  this->Coefficients->GetNumberOfTuples());


  int nt=this->Coefficients->GetNumberOfTuples();
  for (int i = 0; i < nt ; i++)
    {
      double p=this->Coefficients->GetComponent(i,0);

      this->Coefficients->SetComponent(i,0,p-OptimizationStructure.step);
      double s1 = this->Evaluate();

      this->Coefficients->SetComponent(i,0,p+OptimizationStructure.step);
      double s2 = this->Evaluate();

      this->Coefficients->SetComponent(i,0,p);

      // Minus sign for ascent 
      grad->SetComponent(i,0,-(s2-s1));
      GradientNorm+=pow(s2-s1,2.0);

    }
  GradientNorm = (sqrt(GradientNorm));
  double sc=OptimizationStructure.step/GradientNorm;

  // Normalize vector
  if (GradientNorm > 0)
    {
      for (int t=0;t<nt;t++)
	grad->SetComponent(t,0,grad->GetComponent(t,0) * sc);
    }
  else 
    grad->FillComponent(0,0.0);

  fprintf(stderr,"... Gradient Norm=%.5f scale=%f\n",GradientNorm,sc);
  return GradientNorm;
}
//-------------------------------------------------------------------------
int vtkpxPolynomialBiasFieldCorrection::CreateWeightImage(vtkImageData* ref) 
{
  if (this->WeightImage!=NULL)
    this->WeightImage->Delete();

  this->WeightImage=vtkImageData::New();
  this->WeightImage->CopyStructure(ref);
  this->WeightImage->SetScalarTypeToShort();
  this->WeightImage->SetNumberOfScalarComponents(1);
  this->WeightImage->AllocateScalars();

  vtkDataArray* wgt=this->WeightImage->GetPointData()->GetScalars();
  int nt=wgt->GetNumberOfTuples();

  if (this->MaskImage==NULL)
    {
      wgt->FillComponent(0,1);
    }
  else
    {
      vtkDataArray* msk=this->MaskImage->GetPointData()->GetScalars();
      double mrange[2]; msk->GetRange(mrange);
      double thr=0.5*(mrange[0]+mrange[1]);
      for (int i=0;i<nt;i++)
	{
	  float m=msk->GetComponent(i,0);
	  if (m>thr)
	    wgt->SetComponent(i,0,1.0);
	  else
	    wgt->SetComponent(i,0,0.0);
	}
    }
  //  vtkpxUtil::SaveAnalyze("poly_weight",this->WeightImage,0);
	return 1;
}
//-------------------------------------------------------------------------
void vtkpxPolynomialBiasFieldCorrection::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Input to vtkpxPolynomialBiasFieldCorrection");
      return;
    }

  this->CheckInputsAndCreateReferenceImage(input);

  fprintf(stderr,"Beginning Degree=%d\n",this->Degree);



  vtkDebugMacro(<<"Initializing All");
  int ok=this->InitializeAll();

  if (this->ClassParameters==NULL && this->Metric==0)
    {
      fprintf(stderr,"\n\t\tComputing Class Parameters from Original Image\n");
      this->ComputeAndSetClassParameters(this->ReferenceImage);
    } 
  else
    {
      fprintf(stderr,"\n\t\tNot Computing Class Parameters from Original Image Metric=%d\n",this->Metric);
    }

  if (this->ClassParameters!=NULL)
    {
      int nc=this->ClassParameters->GetNumberOfTuples();
      fprintf(stderr,"Initial Parameters : %d\n",nc);
      for (int c=0;c<nc;c++)
	fprintf(stderr,"Class %d \t Mean = %.1f , Sigma=%.1f\n",c,
		this->ClassParameters->GetComponent(c,1),
		this->ClassParameters->GetComponent(c,2));
      fprintf(stderr,"\n");
    }



  fprintf(stderr," -------------------- Done Setting Parameters ------------- \n");

  if (ok==0)
    {
      vtkErrorMacro(<<"Initialization Failed\n");
      fprintf(stderr,"Initialization Failed\n");
      return;
    }


  for (int level=this->NumberOfLevels;level>=1;level=level-1)
    {
      vtkDebugMacro(<<"Initializing Level="<<level);
      this->Initialize(level);
      vtkDebugMacro(<<"Compute Bias Fit Level="<<level);
      this->ComputeBiasFit(level);
      vtkDebugMacro(<<"Finalizing Level="<<level);
    }
  vtkDebugMacro(<<"Finalizing All");
  this->FinalizeAll();

  // Generate Output
  if (this->BiasField=NULL)
    this->BiasField->Delete();
  
  this->BiasField=vtkpxPolynomialBiasFieldCorrection::ComputeBiasFieldImage(input,this->Coefficients,this->Degree,this->MinValue,this->MaxValue);
  vtkImageData* tmp=vtkpxAbstractBiasFieldCorrection::ComputeMeanPreservingCorrectedImage(input,this->BiasField);

  output->ShallowCopy(tmp);
  tmp->Delete();

  fprintf(stderr,"Done Degree=%d\n",this->Degree);
 
}

