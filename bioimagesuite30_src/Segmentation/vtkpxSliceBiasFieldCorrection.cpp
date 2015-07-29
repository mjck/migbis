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
  Module:    $RCSfile: vtkpxSliceBiasFieldCorrection.cpp,v $
  Language:  C++
  Date:      $Date: 2004/11/23 18:19:05 $
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
#include "vtkpxSliceBiasFieldCorrection.h"
#include "vtkObjectFactory.h"
#include "pxutil.h"
#include "vtkpxUtil.h"
#include "vtkExtractVOI.h"
#include "vtkpxOptimizer.h"
#include "vtkbisImageResample.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageGradientMagnitude.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkpxMath.h"
#include "vtkpxImageExtract.h"
#include "vtkpxHistogramSegmentation.h"
#include "pxisinf.h"
#include "vtkpxMatrix.h"

vtkpxSliceBiasFieldCorrection* vtkpxSliceBiasFieldCorrection::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxSliceBiasFieldCorrection");
  if(ret)
    {
    return (vtkpxSliceBiasFieldCorrection*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxSliceBiasFieldCorrection;
  
}


vtkpxSliceBiasFieldCorrection::vtkpxSliceBiasFieldCorrection()
{
  this->SliceParameters=vtkDoubleArray::New();
  this->RobustMode=0;
  this->Threshold=0.02;
  this->Axis=2;
  this->PureScaling=0;
}

vtkpxSliceBiasFieldCorrection::~vtkpxSliceBiasFieldCorrection() 
{
  this->SliceParameters->Delete();
}
//-------------------------------------------------------------------------
int vtkpxSliceBiasFieldCorrection::CreateWeightImage(vtkImageData* ref) 
{
  if (this->WeightImage!=NULL)
    this->WeightImage->Delete();

  this->WeightImage=vtkImageData::New();
  this->WeightImage->CopyStructure(ref);
  this->WeightImage->SetScalarTypeToFloat();
  this->WeightImage->SetNumberOfScalarComponents(1);
  this->WeightImage->AllocateScalars();

  vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
  smooth->SetInput(ref);
  smooth->SetStandardDeviations(1.0,1.0,0.0);

  vtkImageGradientMagnitude* grad=vtkImageGradientMagnitude::New();
  grad->HandleBoundariesOn();
  grad->SetInput(smooth->GetOutput());
  grad->SetDimensionality(2);
  grad->Update();

  vtkImageData* tmp=grad->GetOutput();

  vtkDataArray* magn=tmp->GetPointData()->GetScalars();
  double r[2]; magn->GetRange(r);
  double sigma2=pow(r[1]*0.1,2.0);


  vtkDataArray* in=ref->GetPointData()->GetScalars();
  vtkDataArray* wgt=this->WeightImage->GetPointData()->GetScalars();
  int nt=wgt->GetNumberOfTuples();

  double range[2];in->GetRange(range);
  double m_range[2];
  vtkDataArray* mask=NULL;
  if (this->MaskImage!=NULL)
    {
      mask=this->MaskImage->GetPointData()->GetScalars();
      mask->GetRange(m_range);
    }

  for (int i=0;i<nt;i++)
    {
      double v=in->GetComponent(i,0);
     
      double w=0.0;
      if (v>range[0])
	w=1000.0;

      if (this->MaskImage!=NULL)
	{
	  if (mask->GetComponent(i,0)>m_range[0])
	    w=1000.0;
	  else
	    w=0.0;
	}
      
      wgt->SetComponent(i,0,w*(1.0-vtkpxAbstractBiasFieldCorrection::Valley2(magn->GetComponent(i,0),sigma2)));
    }

  grad->Delete();
  smooth->Delete();
  return 1;
}
// ---------------------------------------------------------------------------------------------------
void vtkpxSliceBiasFieldCorrection::PrintInformation() 
{
  
}
//-------------------------------------------------------------------------
vtkImageData* vtkpxSliceBiasFieldCorrection::GenerateSynthetic(int width,double factor)
{
  if (width<16)
    width=16;
  else if (width>256)
    width=256;

  if (factor < 0.000001)
    factor=0.000001;
  else if (factor>0.1)
    factor=0.1;

  int offset=width/7;

  vtkImageData* tmp=vtkImageData::New();
  tmp->SetDimensions(width,width,width);
  tmp->SetScalarTypeToShort();
  tmp->AllocateScalars();

  int mid=width/2;

  for (int slice=0;slice<width;slice++)
    {
      float bias=vtkpxMath::GaussianRandom()*0.1+1.0;
      for (int j=0;j<width;j++)
	{
	  int count=0;
	  int mode=0;
	  for (int i=0;i<width;i++)
	    {
	      float v=0.0;
	      if (mode==0)
		v=vtkpxMath::GaussianRandom()*5.0+100.0;
	      else
		v=vtkpxMath::GaussianRandom()*5.0+50.0;
	      
	      double newv=bias*v;

	      if (newv<0.0)
		newv=0.0;
	      
	      tmp->SetScalarComponentFromDouble(i,j,slice,0,newv);
	      ++count;
	      if (count==offset)
		{
		  mode=1-mode;
		  count=0;
		}
	    }
	}
    }
  return tmp;
}
//-------------------------------------------------------------------------
int vtkpxSliceBiasFieldCorrection::ComputeWeightedImageRatio(vtkImageData* img1,vtkImageData* img2,vtkImageData* weight1,vtkImageData* weight2,
							     float threshold, 
							     float *x,float* y,float* t,float& a,float& b)
{
 vtkDataArray* data1=img1->GetPointData()->GetScalars();
 vtkDataArray* data2=img2->GetPointData()->GetScalars();
  
 vtkDataArray* wgt1=weight1->GetPointData()->GetScalars();
 vtkDataArray* wgt2=weight2->GetPointData()->GetScalars();

  int nt1=data1->GetNumberOfTuples();
  if (nt1!=data2->GetNumberOfTuples() || nt1!=wgt1->GetNumberOfTuples() || nt1!=wgt2->GetNumberOfTuples())
    {
      fprintf(stderr,"**** Bad Images for Ratio\n");
      a=0.0;
      b=0.0;
      return 0;
    }

  int index=0;
  int numgood=0;
  for (int pixel=0;pixel<nt1;pixel++)
    {
      double xin=data1->GetComponent(pixel,0);
      double yin=data2->GetComponent(pixel,0);
      
      if (xin>threshold && yin>threshold)
	++numgood;

      if (wgt1->GetComponent(pixel,0)>750.0 && wgt2->GetComponent(pixel,0)> 750.0)
	{
	  x[index]=xin;
	  y[index]=yin;
	  ++index;
	}
    }

  float abdev=0.0;

  if (index > nt1/20)
    {
      int maxpass=1;
      if (this->PureScaling==0)
	maxpass=0;
      for (int pass=0;pass<=maxpass;pass++)
	{
	  if (this->RobustMode==1)
	    vtkpxMath::Medfit(x,y,index,a,b,abdev,t);
	  else
	    vtkpxMath::Meanfit(x,y,index,a,b,abdev);
	  if (pass==0 && maxpass>0)
	    {
	      for (int i=0;i<index;i++)
		y[i]=y[i]-a;
	    }
	}
    }
  else
    {
      a=0.0;
      b=1.0;
    }

  if (this->PureScaling==1)
    a=0.0;

  return numgood;
}
//-------------------------------------------------------------------------
void vtkpxSliceBiasFieldCorrection::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxSliceBiasFieldCorrection");
      return;
    }

  this->CheckInputsAndCreateReferenceImage(input);

  if (this->BiasField!=NULL)
    {
      this->BiasField->Delete();
      this->BiasField=NULL;
    }
  

  vtkDebugMacro(<<"Initializing All");
  int ok=this->InitializeAll();
  if (ok==0)
    {
      vtkErrorMacro(<<"Initialization Failed\n");
      return;
    }

  int dim[3]; this->ReferenceImage->GetDimensions(dim);

  if (dim[this->Axis]<2)
    {
      output->ShallowCopy(input);
      return;
    }
  

  double int_range[2]; this->ReferenceImage->GetPointData()->GetScalars()->GetRange(int_range);
  int nonnegative=0;
  if (int_range[0]>=0.0)
    nonnegative=1;

  float threshold=this->Threshold*int_range[1]+(1.0-this->Threshold)*int_range[0];

  int perdim[3];
  perdim[2]=dim[this->Axis];
  for (int k=1;k<=2;k++)
    {
      int taxis=this->Axis+k;
      if (taxis>2)
	taxis-=3;
      perdim[k-1]=dim[taxis];
    }

  fprintf(stderr,"Axis=%d dim=%d,%d,%d perdim=%d,%d,%d\n",
	  this->Axis,
	  dim[0],dim[1],dim[2],
	  perdim[0],perdim[1],perdim[2]);

  int slicesize=perdim[0]*perdim[1];
  float* xa=new float[slicesize];
  float* ya=new float[slicesize];
  float* ta=new float[slicesize];

  this->SliceParameters->SetNumberOfComponents(3);
  this->SliceParameters->SetNumberOfTuples(perdim[2]);
  this->SliceParameters->FillComponent(0,0.0);
  this->SliceParameters->FillComponent(1,1.0);
  this->SliceParameters->FillComponent(2,1.0);

  this->CreateWeightImage(this->ReferenceImage);
  
  vtkpxImageExtract* slice_voi=vtkpxImageExtract::New(); 
  slice_voi->SetInput(this->ReferenceImage);   slice_voi->SetCurrentPlane(this->Axis);
  
  vtkpxImageExtract* slice_voi2=vtkpxImageExtract::New(); 
  slice_voi2->SetInput(this->ReferenceImage);   slice_voi2->SetCurrentPlane(this->Axis);
  
  vtkpxImageExtract* weight_voi=vtkpxImageExtract::New(); 
  weight_voi->SetInput(this->WeightImage);   weight_voi->SetCurrentPlane(this->Axis);
  
  vtkpxImageExtract* weight_voi2=vtkpxImageExtract::New(); 
  weight_voi2->SetInput(this->WeightImage);   weight_voi2->SetCurrentPlane(this->Axis);
  

  for (int pass=0;pass<=1;pass++)
    {
      double current_a=0.0;
      double current_b=1.0;

      int slice=perdim[2]/2+1; 
      int endslice=perdim[2];
      int increment=1;

      
      if (pass==1)
	{
	  slice=perdim[2]/2-1;   
	  endslice=-1;
	  increment=-1;
	}

      int maxcount=abs(endslice-slice);
      if (slice>=perdim[2] || slice<0)
	maxcount=-1;

      fprintf(stderr,"Slice=%d endslice=%d increment=%d maxcount=%d\n",
	      slice,endslice,increment,maxcount);

      for (int count=0;count<maxcount;count++)
	{
	  slice_voi->SetSliceNo(slice);	    slice_voi->Update();
	  slice_voi2->SetSliceNo(slice-increment);  slice_voi2->Update();
	  weight_voi->SetSliceNo(slice);    weight_voi->Update();
	  weight_voi2->SetSliceNo(slice-increment); weight_voi2->Update();
	  
	  
	  float a1,b1;
	  int nv1=this->ComputeWeightedImageRatio(slice_voi2->GetOutput(),slice_voi->GetOutput(),weight_voi2->GetOutput(),weight_voi->GetOutput(),
						  threshold,xa,ya,ta,a1,b1);
	  float ah1,bh1;
	  
	  int nv2=this->ComputeWeightedImageRatio(slice_voi->GetOutput(),slice_voi2->GetOutput(),weight_voi->GetOutput(),weight_voi2->GetOutput(),
						  threshold,xa,ya,ta,ah1,bh1);
	  
	  double a0=current_a;
	  double b0=current_b;

	  

	  //	  fprintf(stderr,"Orig Pairs nv1=%d (%.2f,%.2f) vs nv2=%d (%.2f,%.2f)\t",nv1,a1,b1,nv2,ah1,bh1);
	  
	  if (nv1>0 && nv2>0 && (b1*bh1)>0.5)
	    {
	      float olda1=a1,oldb1=b1;

	      a1=0.5*(a1-ah1/bh1);
	      b1=0.5*(b1+1.0/bh1);

	      if (b1>1.5 || b1 <0.666)
		{
		  b1=Frange(b1,0.666,1.5);
		  a1=0.0;
		}

	      if ( !isnan(a1) && !isnan(b1) )
		{
		  current_a=a1+b1*a0;
		  current_b=b1*b0;
		}
	      else
		{
		  a1=0.0;
		  b1=0.0;
		}
	    }
	  else
	    {
	      a1=0.0;
	      b1=0.0;
	    }
	  
	  /*	  fprintf(stderr,"Slice = %02d to %02d \t ",slice-increment,slice);
	  fprintf(stderr,"y=%.4f + %.4f * x (%.2f,%.2f), \n***********************\t overall \t  %.2f + %.2f * x --> \t %.2f + %.2f * x\n\n",
		  a1,b1,
		  100.0*nv1/double(perdim[0]*perdim[1]),
		  100.0*nv2/double(perdim[0]*perdim[1]),
		  a0,b0,
		  current_a,current_b);*/
	  this->SliceParameters->SetComponent(slice,0,current_a);
	  this->SliceParameters->SetComponent(slice,1,current_b);
	  this->SliceParameters->SetComponent(slice,2,double(nv1)/double(slicesize));
	  slice+=increment;
	}
    }

  if (perdim[2]>3)
    {
      double a1=this->SliceParameters->GetComponent(perdim[2]/2-1,2);
      double a2=this->SliceParameters->GetComponent(perdim[2]/2+1,2);
      this->SliceParameters->SetComponent(perdim[2]/2,2,0.5*(a1+a2));
    }

  slice_voi->Delete();
  slice_voi2->Delete();
  weight_voi->Delete();
  weight_voi2->Delete();
  delete [] xa;
  delete [] ya;
  delete [] ta;

  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* dat=output->GetPointData()->GetScalars();
  
  double sum_in=0.0,sum_out=0.0;
  int nc=dat->GetNumberOfComponents();

  double drange[2]; dat->GetDataTypeRange(drange);
  if (nonnegative==1)
    drange[0]=0.0;

  int index=0;
  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  double a=0.0,b=1.0;
	  switch (this->Axis)
	    {
	    case 0:
	      a=this->SliceParameters->GetComponent(i,0);
	      b=this->SliceParameters->GetComponent(i,1);
	      break;
	    case 1:
	      a=this->SliceParameters->GetComponent(j,0);
	      b=this->SliceParameters->GetComponent(j,1);
	      break;
	    case 2:
	      a=this->SliceParameters->GetComponent(k,0);
	      b=this->SliceParameters->GetComponent(k,1);
	      break;
	    }

	  for (int c=0;c<nc;c++)
	    {
	      double y=inp->GetComponent(index,c);
	      if (y>threshold)
		{
		  double x=(y-a)/b;
		  
		  if (x<drange[0])
		    x=drange[0];
		  else if (x>drange[1])
		    x=drange[1];
		  
		  if (c==0)
		    {
		      sum_in+=y;
		      sum_out+=x;
		    }
		  dat->SetComponent(index,c,x);
		}
	      else
		{
		  dat->SetComponent(index,c,y);		  
		}
	    }
	  ++index;
	}

  this->BiasField=vtkImageData::New();  
  this->BiasField->CopyStructure(output);
  this->BiasField->SetNumberOfScalarComponents(1);
  this->BiasField->SetScalarTypeToFloat();
  this->BiasField->AllocateScalars();
  vtkDataArray* bias=this->BiasField->GetPointData()->GetScalars();

  double scale=sum_in/sum_out;
  fprintf(stderr,"Overall scale=%.2f\n",scale);
  int    nt=dat->GetNumberOfTuples();
  for (int vx=0;vx<nt;vx++)
    for (int c=0;c<nc;c++)
      {
	dat->SetComponent(vx,c,dat->GetComponent(vx,c)*scale);
	if (c==0)
	  {
	    double y=inp->GetComponent(vx,0);
	    double x=dat->GetComponent(vx,0);
	    double b=0.0;
	    if (x>0.0)
	      b=100.0*y/x;
	    else 
	      b=100.0;
	    bias->SetComponent(vx,0,b);
	  }
      }
}


int vtkpxSliceBiasFieldCorrection::PolynomialFitToParameters(int order,vtkFloatArray* params)
{
  if (params==NULL || this->SliceParameters->GetNumberOfTuples()<2)
    {
      vtkErrorMacro(<<"Either Bad Output Array or no SliceParameters\n");
      return 0;
    }
  
  if (order<1)
    order=1;
  else if (order>3)
    order=3;

  params->SetNumberOfComponents(1);
  params->SetNumberOfTuples(order);

  int rows=this->SliceParameters->GetNumberOfTuples();

  double* y=new double[rows];

  double sum=0.0;
  for (int i=0;i<rows;i++)
    sum+=this->SliceParameters->GetComponent(i,1);
  double mean=sum/double(rows);

  for (int i=0;i<rows;i++)
    y[i]=this->SliceParameters->GetComponent(i,1)-mean;


  vtkpxMatrix* A=vtkpxMatrix::New();
  vtkpxMatrix* B=vtkpxMatrix::New();
  vtkpxMatrix* W=vtkpxMatrix::New();
  vtkpxMatrix* D=vtkpxMatrix::New();
  A->Zeros(rows,order+1);
  B->Zeros(rows,1);
  D->Zeros(rows,1);
  W->Eye(rows);

  double half=0.5*double(rows);

  for (int i=0;i<rows;i++)
    {
      double w=this->SliceParameters->GetComponent(i,2)+0.01;
      B->SetDirectElement(i,0,y[i]);
      W->SetDirectElement(i,i,w);
      D->SetDirectElement(i,0,w);
      double x=(double(i)-half)/half;
      
      for (int j=0;j<=order;j++)
	{
	  switch (j)
	    {
	    case 0:
	      A->SetDirectElement(i,0,1.0);
	    case 1:
	      A->SetDirectElement(i,1,x);
	      break;
	    case 2:
	      A->SetDirectElement(i,2,0.5*(3.0*x*x-1.0));
	      break;
	    case 3:
	      A->SetDirectElement(i,3,0.5*(5.0*x*x*x-3.0*x));
	      break;
	    }
	}
    }

  vtkpxMatrix* Wa=vtkpxMatrix::New();
  vtkpxMatrix::Multiply(W,A,Wa);

  vtkpxMatrix* Wb=vtkpxMatrix::New();
  vtkpxMatrix::Multiply(W,B,Wb);

  vtkpxMatrix* X=vtkpxMatrix::New();
  X->Zeros(order+1,1);

  A->SolveLeastSquares(B,X);
  X->Print("Ordinary Params");

  for (int i=0;i<order;i++)
    params->SetComponent(i,0,X->GetDirectElement(i+1,0));

  A->Delete();
  B->Delete();
  X->Delete();
  Wa->Delete();
  Wb->Delete();

  delete [] y;
  return order;
}

