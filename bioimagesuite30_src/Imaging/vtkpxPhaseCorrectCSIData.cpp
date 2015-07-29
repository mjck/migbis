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
  Module:    $RCSfile: vtkpxPhaseCorrectCSIData.cpp,v $
  Language:  C++
  Date:      $Date: 2004/03/23 19:20:08 $
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


// vtk includes
#include "pxutil.h"
#include "vtkObjectFactory.h"
#include "vtkpxPhaseCorrectCSIData.h"
#include "vtkMath.h"
#include "vtkpxUtil.h"
#include "vtkpxOptimizer.h"
#include "vtkImageExtractComponents.h"
#include "vtkImageAppendComponents.h"
#include "vtkImageFFT.h"
#include "vtkImageFourierCenter.h"
#include "vtkpxAnalyzeImageWriter.h"
#include "vtkImageGaussianSmooth.h"

vtkpxPhaseCorrectCSIData* vtkpxPhaseCorrectCSIData::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxPhaseCorrectCSIData");
  if(ret)
    {
      return (vtkpxPhaseCorrectCSIData*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxPhaseCorrectCSIData;
}


vtkpxPhaseCorrectCSIData::vtkpxPhaseCorrectCSIData()
{
  this->OptimizationMethod=4;
  this->parameters=vtkDoubleArray::New();
  this->parameters->SetNumberOfComponents(1);
  this->parameters->SetNumberOfTuples(2);
  this->parameters->FillComponent(0,0.0);
  this->FunctionEvaluation=0;
  this->DerivEvaluation=0;
  this->Step=0.01;
  this->Tolerance=0.001;
  this->Iterations=25;
  this->InputReal=NULL;
  this->InputImag=NULL;
  this->OutputReal=NULL;
  this->OutputImag=NULL;
  this->FirstOrderPhase=0.0;
  this->OptimizeZeroOrder=0;
  this->FirstOrderPhases=vtkDoubleArray::New();
  this->ZeroOrderPhases=vtkDoubleArray::New();
}

vtkpxPhaseCorrectCSIData::~vtkpxPhaseCorrectCSIData()
{
  this->parameters->Delete();
  this->FirstOrderPhases->Delete();
  this->ZeroOrderPhases->Delete();
  
  if (this->InputReal!=NULL)
    this->InputReal->Delete();

  if (this->InputImag!=NULL)
    this->InputImag->Delete();
}

//-------------------------------------------------------------------------
void vtkpxPhaseCorrectCSIData::SetOutputStatus(const char* line,double v)
{
  fprintf(stderr,"**Opt=%s (%.8f)\n",line,v);
}

//-------------------------------------------------------------------------
double vtkpxPhaseCorrectCSIData::CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output)
{
  return ((vtkpxPhaseCorrectCSIData*)self)->OptimizerGradient(position,output);
}

double vtkpxPhaseCorrectCSIData::CalculateFunction(vtkObject* self,vtkDoubleArray* position)
{
  return ((vtkpxPhaseCorrectCSIData*)self)->OptimizerValue(position);
}

void vtkpxPhaseCorrectCSIData::OptimizerFeedback(vtkObject* self,const char* line,double val)
{
  ((vtkpxPhaseCorrectCSIData*)self)->SetOutputStatus(line,val);
}
//-------------------------------------------------------------------------
void vtkpxPhaseCorrectCSIData::SetCurrentParameters(vtkDoubleArray* par)
{
  static int firsttime=0;

  int numt=this->parameters->GetNumberOfTuples();
  int num=par->GetNumberOfTuples();

  if (num!=numt)
    {
      vtkErrorMacro(<<"Bad Data for Optimization");
      return;
    }

  for (int i=0;i<numt;i++)
    {
      this->parameters->SetComponent(i,0,par->GetComponent(i,0));
      if (firsttime)
	fprintf(stderr,"Setting %d to %f to parameter=%f\n",i,par->GetComponent(i,0),this->parameters->GetComponent(i,0));
    }
  
  //  firsttime=0;
}
 
void vtkpxPhaseCorrectCSIData::GetCurrentParameters(vtkDoubleArray* par)
{
  int numt=this->parameters->GetNumberOfTuples();
  int num=par->GetNumberOfTuples();
  static int firsttime=0;

  if (num!=numt)
    par->SetNumberOfTuples(numt);

  for (int i=0;i<numt;i++)
    {
      par->SetComponent(i,0,this->parameters->GetComponent(i,0)*1.0);
      if (firsttime)
	fprintf(stderr,"Getting %d, parameter=%f to %f\n",i,this->parameters->GetComponent(i,0),par->GetComponent(i,0));
    }
  //  firsttime=0;
}
//-------------------------------------------------------------------------
double vtkpxPhaseCorrectCSIData::OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad)
{
  int np=params->GetNumberOfTuples();
  int nt=grad->GetNumberOfTuples();

  if (np!=nt)
    grad->SetNumberOfTuples(np);

  double magn=0.0;

  for (int i=0;i<np;i++)
    {
      double orig=params->GetComponent(i,0);
      params->SetComponent(i,0,orig+this->Step);
      double vp=this->OptimizerValue(params);
      params->SetComponent(i,0,orig-this->Step);
      double vm=this->OptimizerValue(params);
      params->SetComponent(i,0,orig);
      double g=(vp-vm)/this->Step;
      magn+=g*g;
      grad->SetComponent(i,0,g);
    }


  ++this->DerivEvaluation;
  return sqrt(magn);
}
//------------------------------------------------------------------------- 
double vtkpxPhaseCorrectCSIData::OptimizerValue(vtkDoubleArray* params)
{
  ++this->FunctionEvaluation;
  this->SetCurrentParameters(params);
  double sum=this->GenerateOutputImages(this->InputReal,this->InputImag,
				       this->OutputReal,this->OutputImag,
				       this->parameters);

  return sum;
}
//------------------------------------------------------------------------- 
void vtkpxPhaseCorrectCSIData::GenerateInputs(vtkImageData* real,vtkImageData* imag, vtkImageData* outreal,vtkImageData* outimag)
{
  if (this->InputReal==NULL)
    this->InputReal=vtkImageData::New();
  this->InputReal->ShallowCopy(real);

  this->OutputReal=outreal;
  this->OutputReal->DeepCopy(this->InputReal);

  if (this->InputImag==NULL)
    this->InputImag=vtkImageData::New();
  this->InputImag->ShallowCopy(imag);

  this->OutputImag=outimag;
  this->OutputImag->DeepCopy(this->InputImag);
}

//------------------------------------------------------------------------- 
void vtkpxPhaseCorrectCSIData::RunFirstOrderPhase(vtkImageData* real,vtkImageData* imag, vtkImageData* outreal,vtkImageData* outimag)
{
  this->GenerateInputs(real,imag,outreal,outimag);

  int dim[3];
  real->GetDimensions(dim);
  int nvoxels=dim[0]*dim[1]*dim[2];

  this->FirstOrderPhases->SetNumberOfTuples(nvoxels);
  this->FirstOrderPhases->FillComponent(0,this->FirstOrderPhase);

  int voxel=0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{

	  double pinit=this->FirstOrderPhases->GetComponent(voxel,0);
	  double pmax=pinit+1.0;
	  double pmin=pinit-1.0;
	  double pstep=0.05;
	  double pstep2=0.05;
	  double best=this->CorrectVoxel(this->InputReal,this->InputImag,this->OutputReal,this->OutputImag,
					i,j,k,0,pmin,voxel);
	  double bestp=pmin;

	  pmin+=pstep;
	  
	  for (int level=0;level<=9;level++)
	    {
	      for (double p1=pmin;p1<=pmax;p1+=pstep)
		{
		  double v=this->CorrectVoxel(this->InputReal,this->InputImag,this->OutputReal,this->OutputImag,
					     i,j,k,0,p1,voxel);
		  
		  if (v<best)
		    {
		      best=v;
		      bestp=p1;
		    }
		  //fprintf(stderr,"Evaluating %.5f %f \t\t\t best=%.5f %f\n",p1,v,bestp,best);
		}
	      if (level==9)
		fprintf(stderr,"Searched P1 level=%d (%d,%d,%d) %.2f:%.2f:%.2f --> best=%.5f,%f\n",level,
			i,j,k,
		      pmin,pstep,pmax,bestp,best);
	      if (2*(level/2)==level)
		{
		  if (4*(level/4)==level)
		    pstep2=0.666*pstep2;
		  pmin=bestp-9*pstep2;
		  pmax=bestp+95*pstep2;
		  pstep=pstep2;
		}
	      else 
		{
		  pmin=Frange(bestp-4.0,this->FirstOrderPhase-4.0,this->FirstOrderPhase+4.0);
		  pmax=Frange(bestp+4.0,this->FirstOrderPhase-4.0,this->FirstOrderPhase+4.0);
		  pstep=1.0;
		}
	    }
	  
	  this->FirstOrderPhases->SetComponent(voxel,0,bestp);
	  if (i==dim[0]/2 && j==dim[1]/2 && k==dim[2]/2)
	    this->FirstOrderPhase=bestp;
	  ++voxel;
	}

  this->GenerateFirstOrderPhaseOutputImages(this->InputReal,this->InputImag,this->OutputReal,this->OutputImag,this->FirstOrderPhases);
}



void vtkpxPhaseCorrectCSIData::RunZeroOrderPhase(vtkImageData* real,vtkImageData* imag, vtkImageData* outreal,vtkImageData* outimag)
{
    this->GenerateInputs(real,imag,outreal,outimag);

  int dim[3];
  real->GetDimensions(dim);
  int nvoxels=dim[0]*dim[1]*dim[2];


  this->ZeroOrderPhases->SetNumberOfTuples(nvoxels);
  this->ZeroOrderPhases->FillComponent(0,0.0);

  int voxel=0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  double pfirst=this->FirstOrderPhases->GetComponent(voxel,0);
	  double pinit=this->ZeroOrderPhases->GetComponent(voxel,0);
	  double pmax=pinit+0.3;
	  double pmin=pinit-0.3;
	  double pstep=0.03;
	  double pstep2=0.03;
	  double best=this->CorrectVoxel(this->InputReal,this->InputImag,this->OutputReal,this->OutputImag,
					i,j,k,pmin,pfirst,voxel);
	  double bestp=pmin;

	  pmin+=pstep;
	  
	  for (int level=0;level<=4;level++)
	    {
	      for (double p0=pmin;p0<=pmax;p0+=pstep)
		{
		  double v=this->CorrectVoxel(this->InputReal,this->InputImag,this->OutputReal,this->OutputImag,
					     i,j,k,p0,pfirst,voxel);
		  
		  if (v<best)
		    {
		      best=v;
		      bestp=p0;
		    }
		}
	      if (level==4)
		fprintf(stderr,"Searched P0 level=%d (%d,%d,%d) %.2f:%.2f:%.2f --> best=%.5f,%f\n",level,
			i,j,k,
			pmin,pstep,pmax,bestp,best);
	      
	      pstep2=0.1*pstep;
	      pmin=bestp-1.9*pstep;
	      pmax=bestp+1.9*pstep;
	      pstep=pstep2;
	    }
	
	  this->ZeroOrderPhases->SetComponent(voxel,0,bestp);
	  ++voxel;
	}

  this->GenerateZeroAndFirstOrderPhaseOutputImages(this->InputReal,this->InputImag,this->OutputReal,this->OutputImag,
						   this->ZeroOrderPhases,this->FirstOrderPhases);





}


void vtkpxPhaseCorrectCSIData::Run(vtkImageData* real,vtkImageData* imag, vtkImageData* outreal,vtkImageData* outimag)
{
  // Create Real and Imaginary Stuff 
  // -------------------------------
  this->GenerateInputs(real,imag,outreal,outimag);
  
  // one parameter for each voxel (zero-order phase) + 1 global first-order phase 
  // ----------------------------------------------------------------------------
  int dim[3]; this->InputReal->GetDimensions(dim);
  int numparam=dim[0]*dim[1]*dim[2];

  this->parameters->SetNumberOfTuples(numparam*2);
  this->parameters->FillComponent(0,0);
  
  vtkpxOptimizer* optimizer=vtkpxOptimizer::New();

  vtkDoubleArray* position=vtkDoubleArray::New();
  this->GetCurrentParameters(position);

  vtkDoubleArray* temp=vtkDoubleArray::New();


  optimizer->SetMasterObject(this);
  optimizer->SetEvaluateFunctionMethod(&vtkpxPhaseCorrectCSIData::CalculateFunction);
  optimizer->SetEvaluateGradientMethod(&vtkpxPhaseCorrectCSIData::CalculateGradient);
  optimizer->SetFeedbackMethod(&vtkpxPhaseCorrectCSIData::OptimizerFeedback);

  fprintf(stderr,"Starting meth=%d\n",this->OptimizationMethod);
  this->FunctionEvaluation=0;
  this->DerivEvaluation=0;

  double st=this->Step;

  if (numparam==1 && this->OptimizationMethod==5)
    {
      optimizer->Compute1DMinimization(position,-1.0,1.0,this->Tolerance);
    }
  else
    {
      switch (this->OptimizationMethod)
	{
	case -1:
	case 0:
	  optimizer->ComputeOldGradientDescent(position,this->Step,this->Tolerance);
	  break;
	case 2:
	  optimizer->ComputeGradientDescent(position,this->Iterations,this->Tolerance);
	  break;
	case 1:
	  
	  {
	    for (int j=0;j<4;j++)
	      {
		fprintf(stderr,"\n\t\t j=%d Step=%f\n",j,st);
		optimizer->ComputeSlowClimb(position,st,this->Iterations);
		st=st*0.5;
	      }
	  }
	  break;
	case 3:
	  optimizer->ComputePowell(position,temp,this->Iterations,this->Tolerance);
	  break;
	case 4:
	  optimizer->ComputeConjugateGradient(position,this->Iterations,this->Tolerance);
	  break;
	}
    }

  fprintf(stderr,"First Value %f %f\n",position->GetComponent(0,0),this->parameters->GetComponent(0,0));
  this->parameters->DeepCopy(position);
  fprintf(stderr,"First Value After %f %f\n",position->GetComponent(0,0),this->parameters->GetComponent(0,0));

  optimizer->Delete();
  position->Delete();
  temp->Delete();

  this->GenerateOutputImages(this->InputReal,this->InputImag,this->OutputReal,this->OutputImag,this->parameters);
 
}

//----------------------------------------------------------------------------------------------------------
double vtkpxPhaseCorrectCSIData::GenerateOutputImages(vtkImageData* inreal,vtkImageData* inimag,
						     vtkImageData* outreal,vtkImageData* outimag,
						     vtkDoubleArray* params)
{
  int dim[3]; inreal->GetDimensions(dim);
  double sum=0.0;

  int index=0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  double p0=parameters->GetComponent(index,0);
	  sum+=this->CorrectVoxel(inreal,inimag,outreal,outimag,i,j,k,p0,this->FirstOrderPhase,index);
	  ++index;
	}
  
  return sum;
}

// ------------------------------------------------------------------------------------------------------------------

double vtkpxPhaseCorrectCSIData::GenerateFirstOrderPhaseOutputImages(vtkImageData* inreal,vtkImageData* inimag,
								    vtkImageData* outreal,vtkImageData* outimag,
								    vtkDoubleArray* p1_params)
{
  int dim[3]; inreal->GetDimensions(dim);
  double sum=0.0;

  int index=0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  sum+=this->CorrectVoxel(inreal,inimag,outreal,outimag,i,j,k,0.0,p1_params->GetComponent(index,0),index);
	  ++index;
	}
  
  return sum;
}

//----------------------------------------------------------------------------------------------------------
double vtkpxPhaseCorrectCSIData::GenerateZeroAndFirstOrderPhaseOutputImages(vtkImageData* inreal,vtkImageData* inimag,
									   vtkImageData* outreal,vtkImageData* outimag,
									   vtkDoubleArray* p0_params,
									   vtkDoubleArray* p1_params)
{
  int dim[3]; inreal->GetDimensions(dim);
  double sum=0.0;

  int index=0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  sum+=this->CorrectVoxel(inreal,inimag,outreal,outimag,i,j,k,
				  p0_params->GetComponent(index,0),
				  p1_params->GetComponent(index,0),
				  index);
	  ++index;
	}
  
  return sum;
}

//----------------------------------------------------------------------------------------------------------
double vtkpxPhaseCorrectCSIData::CorrectVoxel(vtkImageData* inreal,vtkImageData* inimag,
					    vtkImageData* outreal,vtkImageData* outimag,
					    int x,int y,int z,double p0,double p1,int index)
{

  int nc=inreal->GetNumberOfScalarComponents();
  double sum=0.0;


  vtkDoubleArray* outr=(vtkDoubleArray*)outreal->GetPointData()->GetScalars();
  vtkDoubleArray* outi=(vtkDoubleArray*)outimag->GetPointData()->GetScalars();

  double scalefactor=2.0*vtkMath::Pi()/double(nc);

  for (int c=0;c<nc;c++)
    {
      double X=inreal->GetScalarComponentAsDouble(x,y,z,c);
      double Y=inimag->GetScalarComponentAsDouble(x,y,z,c);

      double Theta = atan2(Y, X);
      double R = sqrt(X*X + Y*Y);
	      
      double Theta2= Theta + p0 + p1 *double(c) * scalefactor;

      double X2=R*cos(Theta2);
      double Y2=R*sin(Theta2);
	      
      outr->SetComponent(index,c,X2);
      outi->SetComponent(index,c,Y2);

      /*  if (c==nc/2)
	fprintf(stderr,"\nc=%d p0=%f p1=%f Input(%.0f,%.0f) --> Output (%.0f,%.0f) Theta2-Theta=%f\n",
	c,p0,p1,X,Y,X2,Y2,Theta2-Theta);*/

      sum+=X2;
      
      /*      if (c<nc/6 || 5*c>nc/6)
	      sum-=fabs(X2);*/

    }

  //  fprintf(stderr,"Corrected Voxel (%d,%d,%d, ind=%d) p0=%f p1=%f sum=%f\n",x,y,z,index,p0,p1,fabs(sum));

  return -(sum);
}
//----------------------------------------------------------------------------------------------------------
int vtkpxPhaseCorrectCSIData::ReadFIDFile(const char* fname,int width,int height,int depth,int components,
					  vtkImageData* Real,vtkImageData* Imaginary)
{
  gzFile fp = gzsuffixopen(fname,"rb");//fopen(fname,"rb");
  if ( fp==Z_NULL)
    {
      fprintf(stderr,"Cannot Open File %s\n", fname );
      return 0;
    }
  
  fprintf(stderr,"\nReading %s, Size of long = %d\n",fname,sizeof(long));
  int numelements=2*width*height*depth*components;
  long* data=new long[numelements];
  //  int np=fread(&data[0],sizeof(long),numelements,fp);
  int np=gzread(fp,&data[0],sizeof(long)*numelements);
  gzclose(fp);

  fprintf(stderr,"np=%d numelements=%d  bytes=%d\n",np,numelements,np*sizeof(long));

  int depth_components=depth*components;
  int depth_components_width=depth_components*width;

  fprintf(stderr,"\n Read %d values (%d)\n",np,numelements);

  Real->SetScalarTypeToDouble();
  Real->SetNumberOfScalarComponents(components);
  Real->SetDimensions(width,height,depth);
  Real->SetWholeExtent(0,width-1,0,height-1,0,depth-1);
  Real->SetExtent(0,width-1,0,height-1,0,depth-1);
  Real->SetSpacing(1.0,1.0,double(height)/double(depth));
  Real->AllocateScalars();
  
  Imaginary->SetScalarTypeToDouble();
  Imaginary->SetNumberOfScalarComponents(components);
  Imaginary->SetDimensions(width,height,depth);
  Imaginary->SetWholeExtent(0,width-1,0,height-1,0,depth-1);
  Imaginary->SetExtent(0,width-1,0,height-1,0,depth-1);
  Imaginary->SetSpacing(1.0,1.0,double(height)/double(depth));
  Imaginary->AllocateScalars();

  fprintf(stderr,"Output Real/Imag (dim=%dx%dx%d, %d)\n",
	  Real->GetDimensions()[0],
	  Real->GetDimensions()[1],
	  Real->GetDimensions()[2],
	  Real->GetNumberOfScalarComponents());
  

  vtkImageData* img=vtkImageData::New();
  img->SetScalarTypeToDouble();
  img->SetNumberOfScalarComponents(2);
  img->SetDimensions(width,height,components);
  img->SetWholeExtent(0,width-1,0,height-1,0,components-1);
  img->SetExtent(0,width-1,0,height-1,0,components-1);
  img->SetSpacing(1.0,1.0,double(height)/double(components));
  img->AllocateScalars();

  vtkDoubleArray* array=(vtkDoubleArray*)(img->GetPointData()->GetScalars());
  vtkDoubleArray* real_array=(vtkDoubleArray*)(Real->GetPointData()->GetScalars());
  vtkDoubleArray* imag_array=(vtkDoubleArray*)(Imaginary->GetPointData()->GetScalars());

  // Flip Left right and flip sign for odd voxels 

  for (int sl=0;sl<depth;sl++)
    {
      // If Even
      int sliceno=0;

      if (2*int(sl/2)==sl)
	{
	  fprintf(stderr,"*");
	  sliceno=sl/2;
	}
      else
	{
	  fprintf(stderr,"+");
	  sliceno=sl/2+depth/2;
	  if (2*int(depth/2)!=depth)
	    sliceno+=1;
	}

      fprintf(stderr,"\tReal Slice = %2d, Imaged Slice= %2d\t components=%d depth_components=%d depth_components_width=%d\n",
	      sl,sliceno,components,depth_components,depth_components_width);

      int index=0;
      for (int c=0;c<components;c++)
	for (int j=0;j<height;j++)
	  for (int i=0;i<width;i++)
	    {
	      int offset= ((components-1)-c) + sliceno*components +  (width-1-i) * depth_components +  j * depth_components_width;

	      /*	      if (i==0 && j==0 && c==components-1)
			      fprintf(stderr,"\tInner offset =%d\n",offset);*/
	      
	      offset=2*offset;
	      array->SetComponent(index,0,data[offset]);
	      array->SetComponent(index,1,data[offset+1]);
	      ++index;
	    }


      vtkImageFFT* fft=vtkImageFFT::New();
      fft->SetDimensionality(3);
      fft->SetInput(img);

      vtkImageFourierCenter* fft_c=vtkImageFourierCenter::New();
      fft_c->SetDimensionality(3);
      fft_c->SetInput(fft->GetOutput());
      fft_c->Update();

      vtkImageData* sliceimage=fft_c->GetOutput();

      int out_index=sl*width*height;

      for (int j=0;j<height;j++)
	for (int i=0;i<width;i++)
	  {
	    for (int c=0;c<components;c++)
	      {
		double re=sliceimage->GetScalarComponentAsDouble(i,j,c,0);
		double im=sliceimage->GetScalarComponentAsDouble(i,j,c,1);
		real_array->SetComponent(out_index,c,re);
		imag_array->SetComponent(out_index,c,im);
	      }
	    ++out_index;
	  }

      fft->Delete();
      fft_c->Delete();
    }
  delete [] data;
  return 1;
}
// --------------------------------------------------------------------------------------------------
int vtkpxPhaseCorrectCSIData::SaveFIDFile(const char* fname,vtkImageData* Real,vtkImageData* Imaginary,int mode)
{
  char line[400];

  if (mode==0)
    {
      vtkpxAnalyzeImageWriter* ana=vtkpxAnalyzeImageWriter::New();
      ana->SetInput(Real);
      ana->SetCompression(6);
      sprintf(line,"%s_real.hdr",fname);
      ana->Save(line);

      fprintf(stderr,"Output Saved in %s (dim=%dx%dx%d, %d)\n",
	      line,
	      Real->GetDimensions()[0],
	      Real->GetDimensions()[1],
	      Real->GetDimensions()[2],
	      Real->GetNumberOfScalarComponents());


      ana->SetInput(Imaginary);
      ana->SetCompression(6);
      sprintf(line,"%s_imag.hdr",fname);
      ana->Save(line);

      fprintf(stderr,"Output Saved in %s (dim=%dx%dx%d, %d)\n",
	      line,
	      Imaginary->GetDimensions()[0],
	      Imaginary->GetDimensions()[1],
	      Imaginary->GetDimensions()[2],
	      Imaginary->GetNumberOfScalarComponents());


      ana->Delete();
    }
  else
    {
      int dim[3]; Real->GetDimensions(dim);
      int components=Real->GetNumberOfScalarComponents();

      vtkImageData* magn=vtkImageData::New();
      magn->SetScalarTypeToDouble();
      magn->SetNumberOfScalarComponents(components);
      magn->SetDimensions(dim);
      magn->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
      magn->SetExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
      magn->SetSpacing(Real->GetSpacing());
      magn->AllocateScalars();
 
      vtkDoubleArray* array=(vtkDoubleArray*)(magn->GetPointData()->GetScalars());
      vtkDoubleArray* real_array=(vtkDoubleArray*)(Real->GetPointData()->GetScalars());
      vtkDoubleArray* imag_array=(vtkDoubleArray*)(Imaginary->GetPointData()->GetScalars());
      
      int nt=dim[0]*dim[1]*dim[2];

      for (int i=0;i<nt;i++)
	for (int c=0;c<components;c++)
	  {
	    double v1=real_array->GetComponent(i,c);
	    double v2=imag_array->GetComponent(i,c);
	    double m=sqrt(v1*v1+v2*v2);
	    array->SetComponent(i,c,m);
	  }


      vtkpxAnalyzeImageWriter* ana=vtkpxAnalyzeImageWriter::New();

      if (mode==1)
	{
	  ana->SetInput(magn);
	  ana->SetCompression(6);
	  ana->Save(fname);

	  fprintf(stderr,"Output Saved in %s (dim=%dx%dx%d, %d)\n",
		  fname,
		  magn->GetDimensions()[0],
		  magn->GetDimensions()[1],
		  magn->GetDimensions()[2],
		  magn->GetNumberOfScalarComponents());
	}
      else
	{
	  vtkImageData* temp=vtkImageData::New();
	  vtkpxUtil::ReorderImage(temp,magn);
	  ana->SetInput(temp);
	  ana->SetCompression(6);
	  ana->Save(fname);
	  temp->Delete();
	  fprintf(stderr,"Output Saved in %s (dim=%dx%dx%d, %d)\n",
		  fname,
		  temp->GetDimensions()[0],
		  temp->GetDimensions()[1],
		  temp->GetDimensions()[2],
		  temp->GetNumberOfScalarComponents());
	}
      ana->Delete();
      magn->Delete();
    }

  return 1;
}
// --------------------------------------------------------------------------------------------------
int vtkpxPhaseCorrectCSIData::SaveIntegralFIDFile(const char* fname,vtkImageData* Real,vtkImageData* Imaginary)
{
  int components=Real->GetNumberOfScalarComponents();
  double* sum=new double[components];

  vtkDoubleArray* real_array=(vtkDoubleArray*)(Real->GetPointData()->GetScalars());
  vtkDoubleArray* imag_array=(vtkDoubleArray*)(Imaginary->GetPointData()->GetScalars());
      
  int nt=real_array->GetNumberOfTuples();

  for (int c=0;c<components;c++)
    {
      sum[c]=0.0;
      for (int i=0;i<nt;i++)
	{
	  double v1=real_array->GetComponent(i,c);
	  double v2=imag_array->GetComponent(i,c);
	  double m=sqrt(v1*v1+v2*v2);
	  sum[c]+=m;
	}
      sum[c]/=double(nt);
    }
  
  FILE* fout=fopen(fname,"w");
  int ok=0;
  if (fout)
    {
      fprintf(fout,"# Component \t Average\n");
      for (int c=0;c<components;c++)
	fprintf(fout,"%5d \t %.2f\n",c,sum[c]);

      fclose(fout);
      ok=1;
    }

  delete [] sum;
  return ok;
}

// --------------------------------------------------------------------------------------------------
int vtkpxPhaseCorrectCSIData::ExtractSingleVoxel(vtkImageData* Real,vtkImageData* Imaginary,vtkImageData* OutReal,vtkImageData* OutImag,
						 vtkImageData* Magn,int x,int y,int z)
{
  
  int nc=Real->GetNumberOfScalarComponents();

  OutReal->SetDimensions(1,1,nc);
  OutReal->SetNumberOfScalarComponents(1);
  OutReal->AllocateScalars();

  OutImag->SetDimensions(1,1,nc);
  OutImag->SetNumberOfScalarComponents(1);
  OutImag->AllocateScalars();

  Magn->SetDimensions(1,1,nc);
  Magn->SetNumberOfScalarComponents(1);
  Magn->AllocateScalars();

  vtkDataArray* data_real=OutReal->GetPointData()->GetScalars();
  vtkDataArray* data_imag=OutImag->GetPointData()->GetScalars();
  vtkDataArray* data_magn=Magn->GetPointData()->GetScalars();

  for (int i=0;i<nc;i++)
    {
      double X=Real->GetScalarComponentAsDouble(x,y,z,i);
      double Y=Imaginary->GetScalarComponentAsDouble(x,y,z,i);
      data_real->SetComponent(i,0,X);
      data_imag->SetComponent(i,0,Y);
      data_magn->SetComponent(i,0,sqrt(X*X+Y*Y));
    }

  return 1;
}

int vtkpxPhaseCorrectCSIData::ExtractSingleVoxelAsComponent(vtkImageData* Real,vtkImageData* Imaginary,vtkImageData* OutReal,vtkImageData* OutImag,
							    int x,int y,int z)
{
  int nc=Real->GetNumberOfScalarComponents();

  OutReal->SetDimensions(1,1,1);
  OutReal->SetNumberOfScalarComponents(nc);
  OutReal->AllocateScalars();

  OutImag->SetDimensions(1,1,1);
  OutImag->SetNumberOfScalarComponents(nc);
  OutImag->AllocateScalars();

  vtkDataArray* data_real=OutReal->GetPointData()->GetScalars();
  vtkDataArray* data_imag=OutImag->GetPointData()->GetScalars();

  for (int i=0;i<nc;i++)
    {
      data_real->SetComponent(0,i,Real->GetScalarComponentAsDouble(x,y,z,i));
      data_imag->SetComponent(0,i,Imaginary->GetScalarComponentAsDouble(x,y,z,i));
    }

  return 1;
}

// --------------------------------------------------------------------------------------------------
double vtkpxPhaseCorrectCSIData::AddPhaseOffsets(vtkImageData* realimg,vtkImageData* imagimg,double theta0,double theta1)
{
  int dim[3]; realimg->GetDimensions(dim);

  vtkDoubleArray* data_real=(vtkDoubleArray*)realimg->GetPointData()->GetScalars();
  vtkDoubleArray* data_imag=(vtkDoubleArray*)imagimg->GetPointData()->GetScalars();

  int index=0;
  double offset0=theta0;

  double sumreal=0.0;
  double sumimag=0.0;

  for (int k=0;k<dim[2];k++)
    {
      double offset1 =theta1*double(k)*2.0*vtkMath::Pi()/double(dim[2]);


      if (k==dim[2]/2)
	fprintf(stderr,"Offsets = %f %f\n",offset0,offset1);

      for (int i=0;i<dim[0];i++)
	for (int j=0;j<dim[1];j++)
	  {
	    double X = data_real->GetComponent(index,0);
	    double Y = data_imag->GetComponent(index,0);
	    
	    double Theta = atan2(Y, X);
	    double R = sqrt(X*X + Y*Y);
	      
	    double Theta2= Theta + offset0 + offset1;
	    double X2=R*cos(Theta2);
	    double Y2=R*sin(Theta2);
	  
	    sumreal+=X2;
	    sumimag+=Y2;

	    data_real->SetComponent(index,0,X2);
	    data_imag->SetComponent(index,0,Y2);
	    ++index;
	  }
    }

  return fabs(sumreal);
}
// --------------------------------------------------------------------------------------------------


double vtkpxPhaseCorrectCSIData::GlobalPhaseCorrection(vtkImageData* realimg,vtkImageData* imagimg,double theta1)
{
  int dim[3]; realimg->GetDimensions(dim);

  vtkDoubleArray* data_real=(vtkDoubleArray*)realimg->GetPointData()->GetScalars();
  vtkDoubleArray* data_imag=(vtkDoubleArray*)imagimg->GetPointData()->GetScalars();

  int index=0;

  double sumreal=0.0;
  double sumimag=0.0;

  int numcomp=data_real->GetNumberOfComponents();

  fprintf(stderr,"slice :");
  for (int slices=0;slices<dim[2];slices++)
    {
      fprintf(stderr,"%d/%d ",slices+1,dim[2]);
      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {
	    int evensum=i+j;
	    double scale=1.0;
	    if (evensum % 2 == 0)
	      scale=1.0;
	    
	    for (int k=0;k<numcomp;k++)
	      {
		double offset =(theta1)*double(k)*2.0*vtkMath::Pi()/double(numcomp);
		
		double X = scale*data_real->GetComponent(index,k);
		double Y = scale*data_imag->GetComponent(index,k);
		
		double Theta = atan2(Y, X);
		double R = sqrt(X*X + Y*Y);
		
		double Theta2= Theta + offset;
		double X2=R*cos(Theta2);
		double Y2=R*sin(Theta2);
		
		sumreal+=X2;
		sumimag+=Y2;
		
		data_real->SetComponent(index,k,X2);
		data_imag->SetComponent(index,k,Y2);
	      }
	    ++index;
	  }
    }
  fprintf(stderr,"\n");

  return fabs(sumreal);

}
// --------------------------------------------------------------------------------------------------
void vtkpxPhaseCorrectCSIData::IntegrateReal(vtkImageData* realimg,vtkImageData* output,
					     double water1,double water2,
					     double sat1,double sat2,
					     double unsat1,double unsat2)
{
  output->CopyStructure(realimg);
  output->SetScalarTypeToDouble();
  output->SetNumberOfScalarComponents(3);
  output->AllocateScalars();

  
  int dim[3]; realimg->GetDimensions(dim);
  int nc=realimg->GetNumberOfScalarComponents();

  vtkDoubleArray* data=(vtkDoubleArray*)output->GetPointData()->GetScalars();

  int voxel=0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  double wat=0.0,ufat=0.0,sfat=0.0;
	  for (int c=0;c<nc;c++)
	    {
	      double v=realimg->GetScalarComponentAsDouble(i,j,k,c);

	      if (c>=water1 && c<=water2)
		wat+=v;
	      else if (c>=sat1 && c<=sat2)
		sfat+=v;
	      else if (c>=unsat1 && c<=unsat2)
		ufat+=v;
	    }
	  data->SetComponent(voxel,0,wat);
	  data->SetComponent(voxel,1,sfat);
	  data->SetComponent(voxel,2,ufat);
	  ++voxel;
	}
}


void vtkpxPhaseCorrectCSIData::IntegrateMagnitudeAndComputeRatio(vtkImageData* realimg,vtkImageData* imagimg,
								 vtkImageData* output,
								 double waterwidth)
						  
{
  output->CopyStructure(realimg);
  output->SetScalarTypeToDouble();
  output->SetNumberOfScalarComponents(2);
  output->AllocateScalars();
  
  
  int dim[3]; realimg->GetDimensions(dim);
  int nc=realimg->GetNumberOfScalarComponents();

  vtkDoubleArray* data=(vtkDoubleArray*)output->GetPointData()->GetScalars();
  
  int lower=int(0.5+nc*(0.5-waterwidth));
  int upper=int(0.0+nc*(0.5+waterwidth));
  fprintf(stderr,"\t\t Integrating Water range = %d:%d (%d,%.2f)\n",lower,upper,nc,waterwidth);

  int voxel=0;
  fprintf(stderr,"slice :");
  for (int k=0;k<dim[2];k++)
    {
      fprintf(stderr,"%d/%d ",k+1,dim[2]);
      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {
	    double wat=0.0,fat=0.0;
	    for (int c=0;c<nc;c++)
	      {
		double v1=realimg->GetScalarComponentAsDouble(i,j,k,c);
		double v2=imagimg->GetScalarComponentAsDouble(i,j,k,c);
		double v=sqrt(v1*v1+v2*v2);
		
		if (c>=lower && c<=upper)
		  wat+=v;
		else 
		  fat+=v;
	      }
	    data->SetComponent(voxel,0,wat);
	    data->SetComponent(voxel,1,fat);
	    ++voxel;
	  }
    }
}

void vtkpxPhaseCorrectCSIData::IntegrateMagnitude(vtkImageData* realimg,vtkImageData* imagimg,
						   vtkImageData* output,
						  double water1,double water2,
						  double sat1,double sat2,
						  double unsat1,double unsat2)
{
  output->CopyStructure(realimg);
  output->SetScalarTypeToDouble();
  output->SetNumberOfScalarComponents(2);
  output->AllocateScalars();

  
  int dim[3]; realimg->GetDimensions(dim);
  int nc=realimg->GetNumberOfScalarComponents();

  vtkDoubleArray* data=(vtkDoubleArray*)output->GetPointData()->GetScalars();

  int voxel=0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  double wat=0.0,ufat=0.0,sfat=0.0;
	  for (int c=0;c<nc;c++)
	    {
	      double v1=realimg->GetScalarComponentAsDouble(i,j,k,c);
	      double v2=imagimg->GetScalarComponentAsDouble(i,j,k,c);
	      double v=sqrt(v1*v1+v2*v2);

	      if (c>=water1 && c<=water2)
		wat+=v;
	      else if (c>=sat1 && c<=sat2)
		sfat+=v;
	      else if (c>=unsat1 && c<=unsat2)
		ufat+=v;
	    }
	  data->SetComponent(voxel,0,wat);
	  data->SetComponent(voxel,1,sfat);
	  data->SetComponent(voxel,2,ufat);
	  ++voxel;
	}
}



void vtkpxPhaseCorrectCSIData::IntegrateMagnitudeGlobal(vtkImageData* realimg,vtkImageData* imagimg,
							vtkImageData* output,
							double water_peak,double fat_peak,double scalefactor)
{
  output->CopyStructure(realimg);
  output->SetScalarTypeToDouble();
  output->SetNumberOfScalarComponents(2);
  output->AllocateScalars();
  
  
  int dim[3]; realimg->GetDimensions(dim);
  int nc=realimg->GetNumberOfScalarComponents();

  vtkDoubleArray* data=(vtkDoubleArray*)output->GetPointData()->GetScalars();
  
  double gap=0.5*fabs(fat_peak-water_peak);

  double lower=water_peak-gap;
  double middle=water_peak+gap;
  double upper=fat_peak+gap;

  fprintf(stderr,"\t\t Integrating Water range = %.1f:%.1f fat_range %.1f:%.1f, water_p=%.1f fat_p=%.1f\n",lower,middle,middle,upper,water_peak,fat_peak);

  int voxel=0;
  fprintf(stderr,"slice :");
  for (int k=0;k<dim[2];k++)
    {
      fprintf(stderr,"%d/%d ",k+1,dim[2]);
      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {
	    double wat=0.0,fat=0.0;
	    for (int c=0;c<nc;c++)
	      {
		double v1=realimg->GetScalarComponentAsDouble(i,j,k,c);
		double v2=imagimg->GetScalarComponentAsDouble(i,j,k,c);
		double v=sqrt(v1*v1+v2*v2);
		
		if (c>=lower && c<=middle)
		  wat+=v;
		else if (c>=middle && c<=upper)
		  fat+=v;
	      }
	    data->SetComponent(voxel,0,wat*scalefactor);
	    data->SetComponent(voxel,1,fat*scalefactor);
	    ++voxel;
	  }
    }
}
// --------------------------------------------------------------------------------------------------
// ----------------------------------------------------- Local Stuff --------------------------------
// --------------------------------------------------------------------------------------------------

void vtkpxPhaseCorrectCSIData::GenerateLocalPeaks(vtkImageData* realimg,vtkImageData* imagimg,
						  vtkImageData* output,
						  double water_peak,double fat_peak,double sigma)
{
  output->CopyStructure(realimg);
  output->SetScalarTypeToDouble();
  output->SetNumberOfScalarComponents(1);
  output->AllocateScalars();

  
  int dim[3]; realimg->GetDimensions(dim);
  int nc=realimg->GetNumberOfScalarComponents();

  vtkDoubleArray* data=(vtkDoubleArray*)output->GetPointData()->GetScalars();
  
  int gap=int(0.4*fabs(fat_peak-water_peak)+0.5);

  fprintf(stderr,"Input looking for water peak in %.1f:%.1f (%.1f) and fat_peak in %.1f:%.1f (%.1f)\n",
	  water_peak-gap,water_peak+gap,water_peak,
	  fat_peak-gap,fat_peak+gap,fat_peak);

  int voxel=0;
  int offset[5] = { -2, -1 , 0 , 1 , 2 };
  double weights[5]={ 0.05,0.25,0.4,0.25,0.05 };

  for (int k=0;k<dim[2];k++)
    {
      fprintf(stderr,"%d/%d ",k+1,dim[2]);
      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {

	    //
	    //  Identify Peaks via shift 
	    //
	    double max_sum=0.0;
	    int shift=0;

	    for (int s=-gap;s<=gap;s++)
	      {
		double v=0.0;
		for (int conv=0;conv<=4;conv++)
		  {
		    int c[2];
		    c[0]=Irange(int(water_peak)+s+offset[conv],0,nc-1);
		    c[1]=Irange(int(fat_peak)+s+offset[conv],0,nc-1);
		
		    for (int q=0;q<=1;q++)
		      {
			double v1=realimg->GetScalarComponentAsDouble(i,j,k,c[q]);
			double v2=imagimg->GetScalarComponentAsDouble(i,j,k,c[q]);
			v+=weights[conv]*sqrt(v1*v1+v2*v2);
		      }
		  }
		
		if (v>max_sum)
		  {
		    max_sum=v;
		    shift=s;
		  }
	      }
	    data->SetComponent(voxel,0,shift);
	    ++voxel;
	  }
    }

  double gp[3],sp[3];
  output->GetSpacing(sp);

  gp[0]=sigma;
  gp[1]=sigma*sp[0]/sp[1];
  gp[2]=sigma*sp[0]/sp[2];

  vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
  smooth->SetInput(output);
  smooth->SetStandardDeviations(gp);
  smooth->Update();
  fprintf(stderr,"Done Smoothing Local Peaks\n");

  output->ShallowCopy(smooth->GetOutput());
  smooth->Delete();

  fprintf(stderr,"Done Generating Local Peaks\n");

}



void vtkpxPhaseCorrectCSIData::IntegrateMagnitudeLocal(vtkImageData* realimg,vtkImageData* imagimg,
						       vtkImageData* output,vtkDoubleArray* volint,
						       double water_peak,double fat_peak,double scalefactor)
{
  int dim[3]; realimg->GetDimensions(dim);
  int nc=realimg->GetNumberOfScalarComponents();

  volint->SetNumberOfComponents(3);
  volint->SetNumberOfTuples(nc);
  volint->FillComponent(0,0.0);
  volint->FillComponent(1,0.0);
  volint->FillComponent(2,0.0);

  output->CopyStructure(realimg);
  output->SetScalarTypeToDouble();
  output->SetNumberOfScalarComponents(3);
  output->AllocateScalars();

  vtkImageData* peaks=vtkImageData::New();
  vtkpxPhaseCorrectCSIData::GenerateLocalPeaks(realimg,imagimg,peaks,water_peak,fat_peak,1.5);
  
  vtkDoubleArray* data=(vtkDoubleArray*)output->GetPointData()->GetScalars();
  
  double gap=0.5*fabs(fat_peak-water_peak);
  double lw=water_peak-gap;
  double uw=water_peak+gap;
  double lf=fat_peak-gap;
  double uf=fat_peak+gap;

  int c2high=nc-int(gap);
  int c2low=int(gap);
  double nt=double(dim[2]*dim[1]*dim[0]);
  int  globshift=int(water_peak-nc/2);


  fprintf(stderr,"Input looking for water peak in %.1f:%.1f (%.1f) and fat_peak in %.1f:%.1f (%.1f) globshift=%d\n",
	  lw,uw,water_peak,lf,uf,fat_peak,globshift);


  int voxel=0;



  fprintf(stderr,"slice :");

  for (int k=0;k<dim[2];k++)
    {
      fprintf(stderr,"%d/%d ",k+1,dim[2]);
      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {
	    double shift=peaks->GetScalarComponentAsDouble(i,j,k,0);
	    data->SetComponent(voxel,2,shift);
	    double wat_p=water_peak+shift;
	    double fat_p=fat_peak+shift;

	    int  dshift=int(wat_p-nc/2);


	    
	    double gp=fabs(fat_p-wat_p)*0.5;
	    double l1=wat_p-gp;
	    double m1=wat_p+gp;
	    double u1=fat_p+gp;
	    
	    if (i==dim[0]/2 && j==dim[0]/2)
	      {
		fprintf(stderr,"k=%d,water = %.1f:%.1f fat=%.1f:%.1f\n",
			k,l1,m1,m1,u1);
	      }
	    
	    double wat=0.0,fat=0.0;
	    for (int c=0;c<nc;c++)
	      {
		double v1=realimg->GetScalarComponentAsDouble(i,j,k,c);
		double v2=imagimg->GetScalarComponentAsDouble(i,j,k,c);
		double v=sqrt(v1*v1+v2*v2);

		int c2=c-dshift;
		if (c2>c2low && c2<c2high)
		  volint->SetComponent(c2,0,volint->GetComponent(c2,0)+v);

		int c3=c-globshift;
		if (c3>c2low && c3<c2high)
		  volint->SetComponent(c3,1,volint->GetComponent(c3,1)+v);

		volint->SetComponent(c,2,volint->GetComponent(c,2)+v);
		if (c>=l1 && c<=m1)
		  wat+=v;
		else if (c>=m1 && c<=u1)
		  fat+=v;
	      }
	    
	    data->SetComponent(voxel,0,wat*scalefactor);
	    data->SetComponent(voxel,1,fat*scalefactor);
	    
	    if (i==dim[0]/2 && j==dim[0]/2)
	      {
		fprintf(stderr,"k=%d, %.1f %.1f %.1f\n",k,
			data->GetComponent(voxel,0),
			data->GetComponent(voxel,1),
			data->GetComponent(voxel,2));
	      }

	    ++voxel;
	  }
    }


  for (int c=0;c<nc;c++)
    for (int d=0;d<=2;d++)
      volint->SetComponent(c,d,volint->GetComponent(c,d)/nt);

  peaks->Delete();

}

