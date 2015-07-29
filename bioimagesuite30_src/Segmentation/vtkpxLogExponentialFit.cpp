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
  Module:    $RCSfile: vtkpxLogExponentialFit.cpp,v $
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

#include "vtkpxLogExponentialFit.h"
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
vtkpxLogExponentialFit* vtkpxLogExponentialFit::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxLogExponentialFit");
  if(ret)
    {
      return (vtkpxLogExponentialFit*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxLogExponentialFit;
}

// Construct object with no children.
vtkpxLogExponentialFit::vtkpxLogExponentialFit()
{
  this->EchoTrain=NULL;
  this->R2Map=vtkImageData::New();
  this->DensityMap=vtkImageData::New();
  this->MaxT2Value=200.0;
  this->IntensityThreshold=0.0;

}
// ----------------------------------------------------------------------------
vtkpxLogExponentialFit::~vtkpxLogExponentialFit()
{
  if (this->R2Map!=NULL)
    this->R2Map->Delete();

  if (this->DensityMap!=NULL)
    this->DensityMap->Delete();

  if (this->EchoTrain!=NULL)
    this->SetEchoTrain(NULL);
}
// ----------------------------------------------------------------------------
int vtkpxLogExponentialFit::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector, 
					       vtkInformationVector *outputVector)
{
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), VTK_FLOAT, 1);
  return 1;
}
// ----------------------------------------------------------------------------
int vtkpxLogExponentialFit::CreateEchoTrain(float FirstEchoTime,float  EchoSpacing,int numechos)
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
    ar->SetComponent(i,0,FirstEchoTime+float(i)*EchoSpacing);

  this->SetEchoTrain(ar);
  ar->Delete();
  return 1;
}



void vtkpxLogExponentialFit::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"No Input for vtkpxLogExponentialFit Specified\n");
      return;
    }

  if (input->GetNumberOfScalarComponents() < 3)
    {
      vtkErrorMacro(<<"Not enough components in input for vtkpxLogExponentialFit to perform fit\n");
      return;
    }

  if (this->EchoTrain==NULL)
    {
      this->CreateEchoTrain(15.0,15.0,-1);
    }

  if (this->EchoTrain->GetNumberOfTuples()<input->GetNumberOfScalarComponents())
    {
      vtkErrorMacro(<<"Bad Echo Train Specified\n");
      return;
    }
  
  if (this->DensityMap!=NULL)
    this->DensityMap->Delete();

  this->DensityMap=vtkImageData::New();
  this->DensityMap->CopyStructure(output);
  this->DensityMap->SetNumberOfScalarComponents(1);
  this->DensityMap->AllocateScalars();

  if (this->R2Map!=NULL)
    this->R2Map->Delete();

  this->R2Map=vtkImageData::New();
  this->R2Map->CopyStructure(this->DensityMap);
  this->R2Map->SetScalarTypeToFloat();
  this->R2Map->SetNumberOfScalarComponents(1);
  this->R2Map->AllocateScalars();


  vtkDataArray* inp_v=input->GetPointData()->GetScalars();
  vtkDataArray* den_v=this->DensityMap->GetPointData()->GetScalars();
  den_v->FillComponent(0,0);
  vtkDataArray* r2_v=this->R2Map->GetPointData()->GetScalars();
  r2_v->FillComponent(0,0);
  vtkDataArray* t2_v=output->GetPointData()->GetScalars();
  t2_v->FillComponent(0,0);

  /*  fprintf(stderr,"output %d,%d\n",
	  t2_v->GetNumberOfTuples(),
	  t2_v->GetNumberOfComponents());*/

  double range[2]; inp_v->GetRange(range);

  double threshold=  this->IntensityThreshold*(range[1]-range[0])+range[0];
  //  fprintf(stderr,"Intensity Threshold=%f threshold=%f\n",this->IntensityThreshold,threshold);

  int nt=inp_v->GetNumberOfTuples();
  int nc=inp_v->GetNumberOfComponents();

  float* temp=new float[nc];
  float*    x=new float[nc];


  int*     ind=new int[nc];

  int numgood=-1;
  for (int j=0;j<nc;j++)
    {
      float t=-this->EchoTrain->GetComponent(j,0);
      if (t<0.0)
	{
	  ++numgood;
	  x[numgood]=t;
	  ind[numgood]=j;
	}
    }
  ++numgood;

  fprintf(stdout,"Numgood=%d:",numgood);
  for (int j=0;j<numgood;j++)
    fprintf(stdout,"j=%d  x[j]=%.2f ind[j]=%d\t",j+1,-x[j],ind[j]);
  fprintf(stdout,"\n");

  int tenth=int(nt/10);
  int count=0;

  double ndata=double(numgood);

  this->UpdateProgress(0.0);
  for (int voxel=0;voxel<nt;voxel++)
    {
      double density=0.0,r2=0.0;
      if (inp_v->GetComponent(voxel,0)>=threshold)
	{
	  double sx=0.0,sy=0.0,sxy=0.0,sxx=0.0;
	  for (int j=0;j<numgood;j++)
	    {
	      double iv=inp_v->GetComponent(voxel,ind[j]);
	      double y=0.0;
	      double t=x[j];


	      if (iv>1.0)
		y=log(iv);
	      else
		y=0.0;

	      if (voxel==75952)
		fprintf(stderr,"voxel=%d \t j=%d ind[j]=%d x[j]=%.2f iv=%.2f t=%.2f y=%.2f \n",
		voxel,j,ind[j],x[j],iv,t,y);

	      
	      sx += t;
	      sy += y;
	      sxy += t*y;
	      sxx += t*t;
	    }
	      
	  double del=ndata*sxx-sx*sx;
	      
	  if (fabs(del)>1e-12)
	    {
	      density=(sxx*sy-sx*sxy)/del;
	      r2=(ndata*sxy-sx*sy)/del;
	      //	      fprintf(stderr,"Density=%.2f r2=%.2f del=%.2f\n");
	    }
	  else
	    {
	      density=0.0;
	      r2=1.0/this->MaxT2Value;
	    }

	  
	  if (r2<1.0/this->MaxT2Value)
	    r2=1.0/this->MaxT2Value;
	  
	  if (r2>10000.0)
	    r2=10000.0;
	  
	  if (r2>0.0)
	    t2_v->SetComponent(voxel,0,1.0/r2);
	  else
	    t2_v->SetComponent(voxel,0,0.0);
	  
	  if (isnan(density) || isinf(density))
	    density=0.0;

	  /*	  if (voxel==2184)
	    fprintf(stderr,"r2=%f den=%f t2=%f\n",r2,exp(density),t2_v->GetComponent(voxel,0));*/
	}
      r2_v->SetComponent(voxel,0,1000.0*r2);
      den_v->SetComponent(voxel,0,exp(density));
      
      ++count;
      if (count==tenth)
	{
	  this->UpdateProgress(0.1+this->GetProgress());
	  count=0;
	}
    }
  this->UpdateProgress(1.0);
  

  delete [] x;
  delete [] ind;
  delete [] temp;
}


