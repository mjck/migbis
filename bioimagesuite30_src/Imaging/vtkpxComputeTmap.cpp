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
  Module:    $RCSfile: vtkpxComputeTmap.cpp,v $
  Language:  C++
  Date:      $Date: 2002/09/26 19:05:57 $
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
#include "vtkpxComputeTmap.h"
#include "vtkpxUtil.h"
#include "vtkpxMath.h"
#include "pxisinf.h"

//------------------------------------------------------------------------------
vtkpxComputeTmap* vtkpxComputeTmap::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxComputeTmap");
  if(ret)
    {
    return (vtkpxComputeTmap*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxComputeTmap;
}

// Construct object with no children.
vtkpxComputeTmap::vtkpxComputeTmap()
{
  this->ScaleFactor=1000.0;
  this->NumberOfInputs=2;
  this->Input2=NULL;
  this->InputStandardDeviation=NULL;
  this->InputStandardDeviation2=NULL;
  this->NumberOfSamples1=-1;
  this->NumberOfSamples2=-1;
  this->OutputZScore=NULL;
  this->ComputeZScore=0;
}
// ----------------------------------------------------------------------------
vtkpxComputeTmap::~vtkpxComputeTmap()
{
  this->SetInput2(NULL);
  this->SetInputStandardDeviation(NULL);
  this->SetInputStandardDeviation2(NULL);
  if (this->OutputZScore!=NULL)
    this->OutputZScore->Delete();
}
// ----------------------------------------------------------------------------
void vtkpxComputeTmap::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();
  output->SetScalarTypeToFloat();
}

void vtkpxComputeTmap::SimpleExecute(vtkImageData* inp,vtkImageData* outp)
{
  if (this->NumberOfSamples1+this->NumberOfSamples2<2)
    {
      vtkErrorMacro(<<"The total number of samples must be greater than 2!\n");
      return;
    }

  if ( this->NumberOfInputs == 2 ) 
    {
      if (this->GetImageDataInput(0)==NULL    || this->Input2==NULL ||
	  this->InputStandardDeviation==NULL || ( this->InputStandardDeviation2==NULL && this->NumberOfSamples2>1))
	{
	  vtkErrorMacro(<<"Bad Inputs to vtkpxComputeTmap");
	  return;
	}
      if (this->NumberOfSamples2<1)
	this->NumberOfSamples2=1;
      
    } 
  else if (this->GetImageDataInput(0)==NULL || this->InputStandardDeviation==NULL )
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxComputeTmap");
      return;
    }

  int dim[3],dim1[3],dim2[3],dim3[3],comp[4];
  this->GetImageDataInput(0)->GetDimensions(dim);
  this->GetInputStandardDeviation()->GetDimensions(dim2);
  comp[0]=this->GetImageDataInput(0)->GetNumberOfScalarComponents();
  comp[1]=this->GetInputStandardDeviation()->GetNumberOfScalarComponents();

  if (this->NumberOfInputs==2)
    {
      this->GetInput2()->GetDimensions(dim1);
      comp[2]=this->GetInput2()->GetNumberOfScalarComponents();
      comp[3]=this->GetInputStandardDeviation2()->GetNumberOfScalarComponents();
      this->GetInputStandardDeviation2()->GetDimensions(dim3);
    }
  else
    {
      this->GetImageDataInput(0)->GetDimensions(dim1);
      this->GetInputStandardDeviation()->GetDimensions(dim3);
      comp[2]=this->GetImageDataInput(0)->GetNumberOfScalarComponents();
      comp[3]=this->GetInputStandardDeviation()->GetNumberOfScalarComponents();

    }
  
  int sum=0;
  for (int ia=0;ia<=2;ia++)
    sum+=abs(dim[ia]-dim1[ia])+abs(dim[ia]-dim2[ia])+abs(dim[ia]-dim3[ia]);

  for (int ib=1;ib<=3;ib++)
    sum+=abs(comp[ib]-comp[0]);

  

  if (sum!=0)
    {
      vtkErrorMacro(<<"Bad Input Dimensions");
      return;
    }
  //  else
  //    fprintf(stderr,"Components = %d(%d) , (%d,%d)\n",comp[0],comp[1],comp[2],comp[3]);

  

  vtkDataArray* out=outp->GetPointData()->GetScalars();

  int numscalars=out->GetNumberOfTuples();

  vtkDataArray* in1=this->GetImageDataInput(0)->GetPointData()->GetScalars();
  vtkDataArray* sig1=this->GetInputStandardDeviation()->GetPointData()->GetScalars();

  vtkDataArray* in2=NULL,*sig2=NULL;

  if ( this->NumberOfInputs==2 ) 
    {
      in2=this->GetInput2()->GetPointData()->GetScalars();
      sig2=this->GetInputStandardDeviation2()->GetPointData()->GetScalars();
    }

  this->UpdateProgress(0.01);
  float pog=0.00;
  float tenth=(numscalars*comp[0])/10.0;
  int count=0;

  float rootn1=sqrt(float(this->NumberOfSamples1));

  float scaledenominator=1.0;
  if ( this->NumberOfInputs==2  ) 
    {
      if (this->NumberOfSamples2>1 && this->NumberOfSamples1>1)
	scaledenominator=sqrt(1.0/float(this->NumberOfSamples1)+1.0/float(this->NumberOfSamples2));
      else if (this->NumberOfSamples2<2)
	scaledenominator=1.0;//sqrt(1.0/float(this->NumberOfSamples1));
      else
	scaledenominator=1.0;//sqrt(1.0/float(this->NumberOfSamples2));
    }

  fprintf(stderr,"Scaledenominator=%f (%d,%d) numinp=%d\n",scaledenominator,this->NumberOfSamples1,this->NumberOfSamples2,this->NumberOfInputs);
  fprintf(stderr,"sc=%f  rootn1=%f Samples=%d,%d\n",this->ScaleFactor,rootn1,this->NumberOfSamples1,this->NumberOfSamples2);

  //  int maxtval=(int)(200.0*this->ScaleFactor);

  for (int component=0;component<comp[0];component++)
    {
      out->FillComponent(component,-1.0);
      for (int pixel=0;pixel<numscalars;pixel++)
	{
	  float mean1=in1->GetComponent(pixel,component);
	  float sigma1=sig1->GetComponent(pixel,component);
	  double tvalue=0.0;
	  
	  if ( this->NumberOfInputs==2  ) 
	    {
	      float mean2=in2->GetComponent(pixel,component);
	      float sigma2=sig2->GetComponent(pixel,component);
	      
	      if ( fabs(sigma1)<0.00001 ||  fabs(sigma2)<0.00001)
		{
		  tvalue=0;
		}
	      else
		{
		  float combinedsigma=0.0;
		  if (this->NumberOfSamples2>1 && this->NumberOfSamples1>1)
		    combinedsigma=sqrt( ( (this->NumberOfSamples1-1.0)*sigma1*sigma1+(this->NumberOfSamples2-1.0)*sigma2*sigma2)/
					float(this->NumberOfSamples1+this->NumberOfSamples2-2.0));
		  else if (this->NumberOfSamples2<2)
		    combinedsigma=sigma1;
		  else
		    combinedsigma=sigma2;
		  
		  float tsigma=combinedsigma*scaledenominator;

		  if (this->Debug)
		    {
		      if (pixel==0)
			fprintf(stderr,"CombinedSigma=%.3f, mean1=%.3f,mean2=%.3f\n",tsigma,mean1,mean2);
		    }
		  
		  if ( fabs(mean1)<0.00001 || fabs(mean2)<0.00001 || fabs(tsigma)<0.00001 || sigma1<0.00001 || sigma2<0.00001 )
		    {
		      tvalue=0;
		    }
		  else 
		    {
		      if (tsigma<0.00001)
			tsigma=0.00001;
		      tvalue=this->ScaleFactor*((mean1-mean2)/tsigma);
		    }
		}
	    }
	  else  // Number Of Inputs = 1
	    {
	      /*      if (component==0 && pixel==0)
		      fprintf(stderr,"* One Inputs\n");*/

	      if ( fabs(mean1)<0.00001 && fabs(sigma1)<0.00001)
		{
		  tvalue=0;
		}
	      else if (isnan(sigma1) || isnan(mean1) || sigma1<0.00001)
		{
		  tvalue=0;
		}
	      else
		{
		  tvalue=this->ScaleFactor*(mean1*rootn1/sigma1);
		}
	    }
	  //	  tvalue=Irange(tvalue,-maxtval,maxtval);
	  out->SetComponent(pixel,component,double(tvalue));
	  count++;
	  if (count==tenth)
	    {
	      pog+=0.1;
	      this->UpdateProgress(pog);
	      count=0;
	    }
	}
    }
  this->UpdateProgress(0.99);
  if (this->ComputeZScore>0)
    this->ComputeTmapToZScoreConversion(outp);
  this->UpdateProgress(1.0);

}

void vtkpxComputeTmap::ComputeTmapToZScoreConversion(vtkImageData* tmap)
{
  if (this->OutputZScore!=NULL)
    this->OutputZScore->Delete();

  int dof=  this->NumberOfSamples1+this->NumberOfSamples2-2;

  this->OutputZScore=vtkpxComputeTmap::TmapToZScoreConversion(tmap,dof,this->ScaleFactor);
}

vtkImageData* vtkpxComputeTmap::TmapToZScoreConversion(vtkImageData* tmap,int dof,double scale)
{
  vtkImageData* out=vtkImageData::New();
  out->CopyStructure(tmap);
  out->AllocateScalars();

  vtkDataArray* tval=tmap->GetPointData()->GetScalars();
  vtkDataArray* zval=out->GetPointData()->GetScalars();

  int nt=tval->GetNumberOfTuples();


  for (int i=0;i<nt;i++)
    {
      double t=tval->GetComponent(i,0)/scale;
      double factor=1.0;
      if (t<0.0)
	{
	  t=-t;
	  factor=-1.0;
	}
      double p=vtkpxMath::TtestToPvalue(t,dof);
      double z=factor*vtkpxMath::PvalueToZscore(p);
      zval->SetComponent(i,0,z);
    }

  return out;
}

vtkImageData* vtkpxComputeTmap::TmapToPvalueConversion(vtkImageData* tmap,int dof,double scale)
{
  vtkImageData* out=vtkImageData::New();
  out->CopyStructure(tmap);
  out->AllocateScalars();

  vtkDataArray* tval=tmap->GetPointData()->GetScalars();
  vtkDataArray* pval=out->GetPointData()->GetScalars();

  int nt=tval->GetNumberOfTuples();


  for (int i=0;i<nt;i++)
    {
      double t=tval->GetComponent(i,0)/scale;
      double factor=1.0;
      if (t<0.0)
	{
	  t=-t;
	  factor=-1.0;
	}
      double p=vtkpxMath::TtestToPvalue(t,dof);
      pval->SetComponent(i,0,p);
    }

  return out;
}

