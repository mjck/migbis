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
  Module:    $RCSfile: vtkpxExponentialFitT1.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:02:52 $
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
#include "vtkObjectFactory.h"

#include "vtkpxExponentialFitT1.h"
#include "vtkpxOptimizer.h"
#include "vtkpxUtil.h"
#include "vtkpxMath.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "pxutil.h"
#include "pxisinf.h"
#include "math.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"

//#include "vnl/vnl_gamma.h"

//extern double vnl_gamma_q(double a, double x);

//------------------------------------------------------------------------------
vtkpxExponentialFitT1* vtkpxExponentialFitT1::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxExponentialFitT1");
  if(ret)
    {
      return (vtkpxExponentialFitT1*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxExponentialFitT1;
}

// Construct object with no children.
vtkpxExponentialFitT1::vtkpxExponentialFitT1()
{
  this->RelaxationTimes=NULL;
  this->R1Map=vtkImageData::New();
  this->DensityMap=vtkImageData::New();
  this->AlphaMap=vtkImageData::New();
  this->MaxT1Value=2000.0;
  this->IntensityThreshold=0.0;
  this->DebugOpt=0;
  this->ThreeParameterFit=1;
  this->alphascale=10.0;
  this->r1scale=100.0;
}
// ----------------------------------------------------------------------------
vtkpxExponentialFitT1::~vtkpxExponentialFitT1()
{
  if (this->R1Map!=NULL)
    this->R1Map->Delete();

  if (this->DensityMap!=NULL)
    this->DensityMap->Delete();

  if (this->AlphaMap!=NULL)
    this->AlphaMap->Delete();

  if (this->RelaxationTimes!=NULL)
    this->SetRelaxationTimes(NULL);
}
// ----------------------------------------------------------------------------
int vtkpxExponentialFitT1::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector, 
					       vtkInformationVector *outputVector)
{
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), VTK_FLOAT, 1);
  return 1;
}
// ----------------------------------------------------------------------------
int vtkpxExponentialFitT1::CreateRelaxationTimes(float FirstRelaxationTime,float  RelaxationSpacing,int numechos)
{
  if (numechos==-1 && this->GetImageDataInput(0)==NULL)
    {
      vtkErrorMacro(<<"No Input Image Specified, specify number of echos explicitly\n");
      return 0;
    }

  if (numechos<1)
    numechos=this->GetImageDataInput(0)->GetNumberOfScalarComponents();

  vtkFloatArray* ar=vtkFloatArray::New();
  ar->SetNumberOfTuples(numechos);
  
  for (int i=0;i<numechos;i++)
    ar->SetComponent(i,0,FirstRelaxationTime+float(i)*RelaxationSpacing);

  this->SetRelaxationTimes(ar);
  ar->Delete();
  return 1;
}



void vtkpxExponentialFitT1::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"No Input for vtkpxExponentialFitT1 Specified\n");
      return;
    }

  if (input->GetNumberOfScalarComponents() < 3)
    {
      vtkErrorMacro(<<"Not enough components in input for vtkpxExponentialFitT1 to perform fit\n");
      return;
    }

  if (this->RelaxationTimes==NULL)
    {
      this->CreateRelaxationTimes(15.0,15.0,-1);
    }

  if (this->RelaxationTimes->GetNumberOfTuples()<input->GetNumberOfScalarComponents())
    {
      vtkErrorMacro(<<"Bad Relaxation Train Specified\n");
      return;
    }
  
  if (this->DensityMap!=NULL)
    this->DensityMap->Delete();

  this->DensityMap=vtkImageData::New();
  this->DensityMap->CopyStructure(output);
  this->DensityMap->SetNumberOfScalarComponents(1);
  this->DensityMap->AllocateScalars();

  if (this->R1Map!=NULL)
    this->R1Map->Delete();

  this->R1Map=vtkImageData::New();
  this->R1Map->CopyStructure(this->DensityMap);
  this->R1Map->SetScalarTypeToFloat();
  this->R1Map->SetNumberOfScalarComponents(1);
  this->R1Map->AllocateScalars();

  if (this->AlphaMap!=NULL)
    this->AlphaMap->Delete();

  this->AlphaMap=vtkImageData::New();
  this->AlphaMap->CopyStructure(this->DensityMap);
  this->AlphaMap->SetScalarTypeToFloat();
  this->AlphaMap->SetNumberOfScalarComponents(1);
  this->AlphaMap->AllocateScalars();


  vtkDataArray* inp_v=input->GetPointData()->GetScalars();
  vtkDataArray* den_v=this->DensityMap->GetPointData()->GetScalars();
  vtkDataArray* alpha_v=this->AlphaMap->GetPointData()->GetScalars();

  den_v->FillComponent(0,0);
  alpha_v->FillComponent(0,1.0);

  vtkDataArray* r1_v=this->R1Map->GetPointData()->GetScalars();
  r1_v->FillComponent(0,0);
  vtkDataArray* t1_v=output->GetPointData()->GetScalars();
  t1_v->FillComponent(0,0);

  /*  fprintf(stderr,"output %d,%d\n",
	  t1_v->GetNumberOfTuples(),
	  t1_v->GetNumberOfComponents());*/

  double range[2]; inp_v->GetRange(range);

  double threshold=  this->IntensityThreshold*(range[1]-range[0])+range[0];
  fprintf(stderr,"Intensity Threshold=%f threshold=%f\n",this->IntensityThreshold,threshold);

  int nt=inp_v->GetNumberOfTuples();
  int nc=inp_v->GetNumberOfComponents();

  this->xvalues=new double[nc];
  int* ind=new int[nc];
  this->yvalues=new double[nc];

  fprintf(stderr,"nc=%d\n\n");
  this->numgoodvalues=-1;
  for (int j=0;j<nc;j++)
    {
      float t=this->RelaxationTimes->GetComponent(j,0);
      fprintf(stderr,"Testing %d %f\n",j,t);
      if (t>0.0)
	{
	  ++numgoodvalues;
	  this->xvalues[numgoodvalues]=t;
	  ind[numgoodvalues]=j;
	}
    }
  ++numgoodvalues;

  fprintf(stdout,"Numgood=%d:",numgoodvalues);
  for (int j=0;j<numgoodvalues;j++)
    fprintf(stdout,"j=%d  xvalues[j]=%.2f indvalues[j]=%d\t",j,xvalues[j],ind[j]);
  fprintf(stdout,"\n");

  if (numgoodvalues<1)
    return;
  int tenth=int(nt/10);
  int count=0;


  this->UpdateProgress(0.0);


  vtkpxOptimizer* optimizer=vtkpxOptimizer::New();
  vtkDoubleArray* position=vtkDoubleArray::New();
  vtkDoubleArray* temp=vtkDoubleArray::New();
  this->GetCurrentParameters(position);
  optimizer->SetMasterObject(this);

  optimizer->SetEvaluateFunctionMethod(&vtkpxExponentialFitT1::CalculateFunction);
  optimizer->SetEvaluateGradientMethod(&vtkpxExponentialFitT1::CalculateGradient);
  optimizer->SetFeedbackMethod(&vtkpxExponentialFitT1::OptimizerFeedback);
  optimizer->SetTempArray(temp);

  for (int voxel=0;voxel<nt;voxel++)
    {
      if (inp_v->GetComponent(voxel,0)>=threshold)
	{
	  /*	  if (voxel==0)
	    this->DebugOpt=1;
	  else
	  this->DebugOpt=0;*/


	  for (int j=0;j<numgoodvalues;j++)
	    {
	      yvalues[j]=inp_v->GetComponent(voxel,ind[j]);
	      if (this->DebugOpt)
		fprintf(stderr,"j=%d xvalues=%f yvalues=%f\n",j,xvalues[j],yvalues[j]);
	    }
	  

	  this->InitializeFit(position);
	  optimizer->ComputeConjugateGradient(position,10,0.001);
	  double density=position->GetComponent(0,0);
	  double r1=position->GetComponent(1,0)/this->r1scale;
	  double alpha=1.0;
	  if (this->ThreeParameterFit)
	    alpha=position->GetComponent(2,0)/this->alphascale;


	  if (this->DebugOpt)
	    fprintf(stderr,"First Estimate d=%.1f, r1=%.5f (%.2f), alpha=%.2f",
		    density,r1,1.0/r1,alpha);
	  

	  /*	  this->InitializeFit2(position,position->GetComponent(0,0));
	  	  optimizer->ComputeConjugateGradient(position,10,0.001);


	  	  density=position->GetComponent(0,0);
	  r1=position->GetComponent(1,0)/this->r1scale;
	  alpha=1.0;
	  if (this->ThreeParameterFit)
	  alpha=position->GetComponent(2,0)/this->alphascale;*/

	  if (this->DebugOpt)
	    fprintf(stderr,"Final Estimate d=%.1f, r1=%.5f (%.2f), alpha=%.2f",
		    density,r1,1.0/r1,alpha);
	  
	  if (r1<1.0/this->MaxT1Value)
	    r1=1.0/this->MaxT1Value;
	  
	  if (r1>10000.0)
	    r1=10000.0;
	  
	  if (r1>0.0)
	    t1_v->SetComponent(voxel,0,1.0/r1);
	  else
	    t1_v->SetComponent(voxel,0,0.0);
	  
	  if (isnan(density) || isinf(density))
	    density=0.0;
	
	  r1_v->SetComponent(voxel,0,1000.0*r1);
	  den_v->SetComponent(voxel,0,density);
	  alpha_v->SetComponent(voxel,0,alpha);
	}
      ++count;
      if (count==tenth)
	{
	  this->UpdateProgress(0.1+this->GetProgress());
	  count=0;
	}
    }
  this->UpdateProgress(1.0);
  

  delete [] this->xvalues;
  delete [] this->yvalues;
  delete [] ind;

  optimizer->Delete();
  position->Delete();
  temp->Delete();
}


//-------------------------------------------------------------------------
double vtkpxExponentialFitT1::CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output)
{
  return ((vtkpxExponentialFitT1*)self)->OptimizerGradient(position,output);
}

double vtkpxExponentialFitT1::CalculateFunction(vtkObject* self,vtkDoubleArray* position)
{
  return ((vtkpxExponentialFitT1*)self)->OptimizerValue(position);
}

void vtkpxExponentialFitT1::OptimizerFeedback(vtkObject* self,const char* line,double val)
{
  ((vtkpxExponentialFitT1*)self)->SetOutputStatus(line,val);
}
//-------------------------------------------------------------------------
void vtkpxExponentialFitT1::SetOutputStatus(const char* line,double v)
{
  if (this->DebugOpt)
    fprintf(stderr,"**Opt=%s (%.8f)\n",line,v);
}

//-------------------------------------------------------------------------

void vtkpxExponentialFitT1::SetCurrentParameters(vtkDoubleArray* par)
{
  this->currentdensity=par->GetComponent(0,0);
  this->currentr1=par->GetComponent(1,0)/this->r1scale;
  if (this->currentr1<0.0)
    this->currentr1=0.0;
  if (this->ThreeParameterFit)
    this->currentalpha=par->GetComponent(2,0)/this->alphascale;
}
 
void vtkpxExponentialFitT1::GetCurrentParameters(vtkDoubleArray* par)
{
  if (par->GetNumberOfTuples()!= 2+this->ThreeParameterFit)
    par->SetNumberOfTuples(2+this->ThreeParameterFit);
  par->SetComponent(0,0,this->currentdensity);
  par->SetComponent(1,0,this->currentr1*this->r1scale);
  if (this->ThreeParameterFit)
    par->SetComponent(2,0,this->currentalpha*this->alphascale);

}

double vtkpxExponentialFitT1::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)
{
  if (grad->GetNumberOfTuples()!= 2+this->ThreeParameterFit)
    grad->SetNumberOfTuples(2+this->ThreeParameterFit);

  double den=params->GetComponent(0,0);
  double r1=params->GetComponent(1,0)/this->r1scale;
  if (r1<0.000001)
    r1=0.000001;
  double alpha=1.0;
  if (den<0.0)
    den=0.0;
  if (this->ThreeParameterFit)
    {
      alpha=params->GetComponent(2,0)/this->alphascale;
      if (alpha<0.0)
	alpha=0.0;
    }

  if (this->DebugOpt)
    fprintf(stderr,"Opt Gradient Current Values =%f , %f (t1=%f), %f\n",
  	    den,r1,1.0/r1,alpha);


  int np=this->ThreeParameterFit+2;

  double sum[3];
  for (int ia=0;ia<=2;ia++)
    sum[ia]=0.0;

  for (int ia=0;ia<this->numgoodvalues;ia++)
    {
      double tx=this->xvalues[ia]*r1;
      double exponent=exp(-tx);
      double v=den*(1.0-alpha*exponent);
	
      //      if (this->DebugOpt)
      //fprintf(stderr,"\t\t grad ia=%d xvalues=%f yalues=%f tx=%f exp=%f v=%f\n",ia,xvalues[ia],yvalues[ia],tx,exponent,v);


      double err=(this->yvalues[ia]-v);
      // grad w.r.t to density
      // 
      sum[0]+= 2.0*err*(alpha*exponent-1.0);
      sum[1]+= -2.0*err*(den*alpha*exponent*this->xvalues[ia]);
      if (this->ThreeParameterFit)
	sum[2]+= 2.0*err*(den*exponent);
    }
  
  double magn=0.0;
  for (int ia=0;ia<np;ia++)
    magn+=sum[ia]*sum[ia];
  magn=sqrt(sqrt(magn));

  if (magn>0.0) 
    {
      for (int ia=0;ia<np;ia++)
	{
	  sum[ia]=(sum[ia]/magn);
	  //  if (this->DebugOpt)
	  //fprintf(stderr,"Normalized Gradient %d = %f\n",ia,sum[ia]);
	}
    }

  grad->SetComponent(0,0,sum[0]);
  grad->SetComponent(1,0,sum[1]);
  if (this->ThreeParameterFit)
    grad->SetComponent(2,0,sum[2]);

  //  if (this->DebugOpt)
  //    fprintf(stderr,"magn=%f\n\n\n",magn);

  return magn;
}
 
double vtkpxExponentialFitT1::OptimizerValue(vtkDoubleArray* params)
{
  double den=params->GetComponent(0,0);
  double r1=params->GetComponent(1,0)/this->r1scale;
  double alpha=1.0;

  if (den<0.0)
    den=0.0;
  if (r1<0.000001)
    r1=0.000001;


  if (this->ThreeParameterFit)
    {
      alpha=params->GetComponent(2,0)/this->alphascale;
      if (alpha<0.0)
	alpha=0.0;
    }


  if (this->DebugOpt)
    fprintf(stderr,"Opt Value Current Values =%f , %f (t1=%f) %f \t",
	    den,r1,1.0/r1,alpha);

  double sum=0.0;
  for (int ia=0;ia<this->numgoodvalues;ia++)
    {
      double v=den*(1.0-alpha*exp(-this->xvalues[ia]*r1));
      //      if (this->DebugOpt)
      //	fprintf(stderr,"\t\t ia=%d \t x=%f \t y=%f \t est=%f\n",ia,this->xvalues[ia],this->yvalues[ia],v);
      sum+=pow(v-this->yvalues[ia],2.0);
    }
  sum=sum*0.001;
  if (this->DebugOpt)
    fprintf(stderr," sum=%f\n\n",sum);
  return sum;
}

void vtkpxExponentialFitT1::InitializeFit2(vtkDoubleArray* position,double den) 
{
  double denestimate=den;

  double maxy=this->yvalues[0];
  double secondy=0.0;
  int opt=0;
  int opt2=0;
  for (int ia=1;ia<this->numgoodvalues;ia++) 
    {
      if (maxy<this->yvalues[ia])
	{
	  opt2=opt;
	  secondy=maxy;
	  
	  maxy=this->yvalues[ia];
	  opt=ia;
	}
    }
  
  double r1estimate=0.1;
  double alphaestimate=1.0;
  
  for (int it=0;it<1;it++)
    {
      double y1=this->yvalues[opt2];
      double y2=this->yvalues[opt];
      
      double x1=this->xvalues[opt2];
      double x2=this->xvalues[opt];
      
      double rhs1=(1.0-y1/denestimate);
      double rhs2=(1.0-y2/denestimate);
      double ratio=rhs2/(rhs1*alphaestimate);
      double logratio=log(ratio);
      r1estimate=-logratio/fabs(x2-x1);
      if (r1estimate<0.000001)
	r1estimate=0.000001;
      
      if (this->ThreeParameterFit)
	{
	  alphaestimate=(y1-y2)/(y1*exp(-x2*r1estimate)-y2*exp(-x1*r1estimate));
	  if (alphaestimate<0.00001)
	    alphaestimate=0.00001;
	}

      if (this->DebugOpt)
	fprintf(stderr,"\t\t Initializing ..(%d).. x1=%f, x2=%f , y1=%f y2=%f  logratio=%f t1estimate=%f (%f) denestimate=%f alpha=%f\n",
		it,x1,x2,y1,y2,logratio,1.0/r1estimate,r1estimate,denestimate,alphaestimate);
    }
  
  position->SetComponent(0,0,denestimate);
  position->SetComponent(1,0,r1estimate*this->r1scale);
  position->SetComponent(2,0,alphaestimate*this->alphascale);



}

void vtkpxExponentialFitT1::InitializeFit(vtkDoubleArray* position) 
{
  double denestimate=this->yvalues[0];
  int opt=0;
  for (int ia=1;ia<this->numgoodvalues;ia++)
    if (denestimate<this->yvalues[ia])
      {
	denestimate=this->yvalues[ia];
	opt=ia;
      }
  
  if (denestimate<0.00001)
    denestimate=0.00001;
  
  int minx1=0,minx2=0;
  double miny1=this->yvalues[opt];
  double miny2=this->yvalues[opt];
  for (int ia=0;ia<this->numgoodvalues;ia++)
    {
      if (miny1>this->yvalues[ia])
	{
	  miny2=miny1;
	  minx2=minx1;
	  minx1=ia;
	  miny1=this->yvalues[ia];
	}
      else if (miny2>this->yvalues[ia])
	{
	  minx2=ia;
	  miny2=this->yvalues[ia];
	}
      /*      fprintf(stderr,"ia=%d, yvalues[ia]=%f \t\t minx1=%d minx2=%d miny1=%f miny2=%f\n",
	      ia,yvalues[ia],minx1,minx2,miny1,miny2);*/
    }


  double y1=this->yvalues[minx2];
  double y2=this->yvalues[minx1];

  double x1=this->xvalues[minx2];
  double x2=this->xvalues[minx1];

  double rhs1=(1.0-y1/denestimate);
  double rhs2=(1.0-y2/denestimate);

  double logratio=log(rhs2/rhs1);
  double r1estimate=logratio/fabs(x2-x1);

  if (r1estimate<0.000001)
    r1estimate=0.000001;

  //  denestimate=yvalues[opt]/(1.0-exp(-xvalues[opt]*r1estimate));

  double alphaestimate=1.0;
  /*  if (this->ThreeParameterFit)
    {
      alphaestimate=(1.0-this->yvalues[opt]/denestimate)/exp(-this->xvalues[opt]*r1estimate);
      if (alphaestimate<0.00001)
	alphaestimate=0.00001;
	}*/
  
  if (this->ThreeParameterFit)
    {
      alphaestimate=(y1-y2)/(y1*exp(-x2*r1estimate)-y2*exp(-x1*r1estimate));
      if (alphaestimate<0.00001)
	alphaestimate=0.00001;
    }

  denestimate=yvalues[opt]/(1.0-alphaestimate*exp(-xvalues[opt]*r1estimate));

  if (this->DebugOpt)
    {
      fprintf(stderr,"\t\t Initializing ..(%d, %d,%d).. x1=%f, x2=%f , y1=%f y2=%f  logratio=%f t1estimate=%f (%f) denestimate=%f alpha=%f\n",
	      opt,minx1,minx2,x1,x2,y1,y2,logratio,1.0/r1estimate,r1estimate,denestimate,alphaestimate);
    }



  int it=1;
  int opt2=minx1;
  if (this->ThreeParameterFit)
    {
      alphaestimate=(1.0-this->yvalues[opt2]/denestimate)/exp(-this->xvalues[opt2]*r1estimate);
      if (alphaestimate<0.00001)
	alphaestimate=0.00001;
    }
  
  r1estimate=log((1.0-this->yvalues[opt2]/denestimate)/alphaestimate)/(-this->xvalues[opt2]);
  
  if (r1estimate<0.000001)
    r1estimate=0.000001;
  
  
  if (this->DebugOpt)
    fprintf(stderr,"\t\t Initializing ..(%d).%d/%d. x=%f, , y=%f t1estimate=%f (%f) denestimate=%f alpha=%f\n",
	    it,opt,opt2,xvalues[opt2],yvalues[opt2],1.0/r1estimate,r1estimate,denestimate,alphaestimate);


  position->SetComponent(0,0,denestimate);
  position->SetComponent(1,0,r1estimate*this->r1scale);
  position->SetComponent(2,0,alphaestimate*this->alphascale);


  

}

