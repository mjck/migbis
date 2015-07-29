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
  Module:    $RCSfile: vtkpxImageComputeNormalizedRatio.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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
#include "vtkpxImageComputeNormalizedRatio.h"
#include "vtkpxUtil.h"
#include "vtkFloatArray.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkpxUtil.h"
//------------------------------------------------------------------------------
vtkpxImageComputeNormalizedRatio* vtkpxImageComputeNormalizedRatio::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageComputeNormalizedRatio");
  if(ret)
    {
    return (vtkpxImageComputeNormalizedRatio*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageComputeNormalizedRatio;
}

// Construct object with no children.
vtkpxImageComputeNormalizedRatio::vtkpxImageComputeNormalizedRatio()
{
  this->Threshold=0.1;
  this->NormalizedThreshold=0.05;
  this->Input2=NULL;
  this->NormalizedOutput=NULL;
  this->AverageRatio=1;
  this->Sigma=1.0;
}
// ----------------------------------------------------------------------------
vtkpxImageComputeNormalizedRatio::~vtkpxImageComputeNormalizedRatio()
{
  this->SetInput2(NULL);
  if (this->NormalizedOutput!=NULL)
    this->NormalizedOutput->Delete();

}
// ----------------------------------------------------------------------------
void vtkpxImageComputeNormalizedRatio::SimpleExecute(vtkImageData* ,vtkImageData* )
{

  if (this->GetImageDataInput(0)==NULL    || this->Input2==NULL )
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxImageComputeNormalizedRatio");
      return;
    }


  int dim[3],dim2[3];
  this->GetImageDataInput(0)->GetDimensions(dim);
  this->GetInput2()->GetDimensions(dim2);
  
  int ssum=0;
  for (int ia=0;ia<=2;ia++)
    ssum+=abs(dim[ia]-dim2[ia]);
  if (ssum!=0)
    {
      vtkErrorMacro(<<"Bad Input Dimensions");
      return;
    }
    
  vtkImageData* output=this->GetOutput();
  output->SetSpacing(this->GetImageDataInput(0)->GetSpacing());
  output->SetOrigin(this->GetImageDataInput(0)->GetOrigin());
  output->SetScalarType(VTK_FLOAT);
  output->SetNumberOfScalarComponents(1);
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  output->AllocateScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();


  if (this->NormalizedOutput==NULL)
    this->NormalizedOutput=vtkImageData::New();

  this->NormalizedOutput->SetSpacing(this->GetImageDataInput(0)->GetSpacing());
  this->NormalizedOutput->SetOrigin(this->GetImageDataInput(0)->GetOrigin());
  this->NormalizedOutput->SetScalarType(VTK_FLOAT);
  this->NormalizedOutput->SetNumberOfScalarComponents(1);
  this->NormalizedOutput->SetDimensions(dim[0],dim[1],dim[2]);
  this->NormalizedOutput->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  this->NormalizedOutput->AllocateScalars();
  vtkDataArray* outper=this->NormalizedOutput->GetPointData()->GetScalars();


  vtkImageGaussianSmooth* sm=vtkImageGaussianSmooth::New();
  sm->SetInput(this->GetImageDataInput(0));
  sm->SetStandardDeviations(this->Sigma,this->Sigma,this->Sigma);
  sm->Update();

  vtkImageGaussianSmooth* sm2=vtkImageGaussianSmooth::New();
  sm2->SetInput(this->GetInput2());
  sm2->SetStandardDeviations(this->Sigma,this->Sigma,this->Sigma);
  sm2->Update();


  vtkDataArray* in1=sm->GetOutput()->GetPointData()->GetScalars();  
  vtkDataArray* in2=sm2->GetOutput()->GetPointData()->GetScalars();

  float tlow,thigh;
  vtkpxUtil::ImageRobustRange(this->GetImageDataInput(0),0.02,0.98,tlow,thigh);
  float threshold=this->Threshold*(thigh-tlow)+tlow;
  float perthreshold=this->NormalizedThreshold*(thigh-tlow)+tlow;

  vtkpxUtil::ImageRobustRange(this->GetInput2(),0.02,0.98,tlow,thigh);
  float perthreshold2=this->NormalizedThreshold*(thigh-tlow)+tlow;

  fprintf(stderr,"Minimum thresholds PET=%.2f SPECT=%.2f\n",perthreshold2,perthreshold);

  int numscalars=out->GetNumberOfTuples();

  this->UpdateProgress(0.01);
  float pog=0.00;
  float tenth=numscalars/10.0;
  int count=0;
  int pixel=0;

  float sxx=0.0,sxy=0.0;

  for (pixel=0;pixel<numscalars;pixel++)
    {
      float v1=in1->GetComponent(pixel,0);
      float v2=in2->GetComponent(pixel,0);
      float r=0.0;
      if (fabs(v1)>0.0001 && v1>perthreshold && v2>perthreshold2 )
	{
	  r=v2/v1;
	  if (v1>threshold)
	    {
	      sxx+=v1*v1;
	      sxy+=v1*v2;
	    }
	}
      out->SetComponent(pixel,0,r);
      count++;
      if (count==tenth)
	{
	  pog+=0.05;
	  this->UpdateProgress(pog);
	  count=0;
	}
    }

  this->AverageRatio=sxy/sxx;

  for (pixel=0;pixel<numscalars;pixel++)
    {
      float v1=in1->GetComponent(pixel,0);
      float v2=in2->GetComponent(pixel,0);
      float r=0.0;
      if (v1>perthreshold && fabs(v1)>0.0001 )
	r=(v2/v1)/this->AverageRatio-1.0;

      outper->SetComponent(pixel,0,r);
      count++;
      if (count==tenth)
	{
	  pog+=0.05;
	  this->UpdateProgress(pog);
	  count=0;
	}
    }
  this->UpdateProgress(1.0);

  sm2->Delete();
  sm->Delete();
}

