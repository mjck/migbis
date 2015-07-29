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
  Module:    $RCSfile: vtkpxImageComputeNormalizedDifference.cpp,v $
  Language:  C++  Date:      $Date: 2002/02/20 18:06:52 $
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
#include "vtkpxImageComputeNormalizedDifference.h"
#include "vtkpxUtil.h"
#include "vtkFloatArray.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkpxUtil.h"
//------------------------------------------------------------------------------
vtkpxImageComputeNormalizedDifference* vtkpxImageComputeNormalizedDifference::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageComputeNormalizedDifference");
  if(ret)
    {
    return (vtkpxImageComputeNormalizedDifference*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageComputeNormalizedDifference;
}

// Construct object with no children.
vtkpxImageComputeNormalizedDifference::vtkpxImageComputeNormalizedDifference()
{
  this->Threshold=0.1;
  this->PercentageThreshold=0.05;
  this->Input2=NULL;
  this->PercentageOutput=NULL;
  this->NormalizedSecondImage=NULL;
  this->Scale=1.0;
  this->Shift=0.0;
  this->Sigma=1.0;
}
// ----------------------------------------------------------------------------
vtkpxImageComputeNormalizedDifference::~vtkpxImageComputeNormalizedDifference()
{
  this->SetInput2(NULL);
  if (this->PercentageOutput!=NULL)
    this->PercentageOutput->Delete();

  if (this->NormalizedSecondImage!=NULL)
    this->NormalizedSecondImage->Delete();
}
// ----------------------------------------------------------------------------
void vtkpxImageComputeNormalizedDifference::SimpleExecute(vtkImageData* input,vtkImageData* output)
{

  if (this->GetImageDataInput(0)==NULL    || this->Input2==NULL )
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxImageComputeNormalizedDifference");
      return;
    }


  int dim[3],dim2[3];
  input->GetDimensions(dim);
  this->Input2->GetDimensions(dim2);
  
  int sum=0;
  for (int ia=0;ia<=2;ia++)
    sum+=abs(dim[ia]-dim2[ia]);
  if (sum!=0)
    {
      vtkErrorMacro(<<"Bad Input Dimensions");
      return;
    }
    
  vtkImageData* tmp=vtkImageData::New();
  tmp->CopyStructure(input);
  tmp->SetScalarType(VTK_FLOAT);
  tmp->SetNumberOfScalarComponents(1);
  tmp->AllocateScalars();
  vtkDataArray* out=tmp->GetPointData()->GetScalars();

  if (this->PercentageOutput==NULL)
    this->PercentageOutput=vtkImageData::New();

  this->PercentageOutput->CopyStructure(tmp);
  this->PercentageOutput->AllocateScalars();


  vtkDataArray* outper=this->PercentageOutput->GetPointData()->GetScalars();

  vtkDataArray* in1=input->GetPointData()->GetScalars();  
  vtkDataArray* in2=this->Input2->GetPointData()->GetScalars();


  float tlow,thigh;
  vtkpxUtil::ImageRobustRange(input,0.02,0.98,tlow,thigh);
  float threshold=this->Threshold*(thigh-tlow)+tlow;
  float perthreshold=this->PercentageThreshold*(thigh-tlow)+tlow;

  int numscalars=out->GetNumberOfTuples();

  float sxx=0.0,sxy=0.0;
  for (int jj=0;jj<numscalars;jj++) 
    {
      float x=in1->GetComponent(jj,0);
      float y=in2->GetComponent(jj,0);
      
      if (x>=threshold)
	{
	  sxy += x*y;
	  sxx += x*x;
	}
    }

  this->Shift=0.0;
  this->Scale=sxy/sxx;
  
  this->UpdateProgress(0.01);
  float pog=0.00;
  float tenth=numscalars/10.0;
  int count=0;

  if (this->NormalizedSecondImage!=NULL)
    this->NormalizedSecondImage->Delete();

  this->NormalizedSecondImage=vtkImageData::New();
  this->NormalizedSecondImage->CopyStructure(this->Input2);
  this->NormalizedSecondImage->SetNumberOfScalarComponents(this->Input2->GetNumberOfScalarComponents());
  this->NormalizedSecondImage->AllocateScalars();
  vtkDataArray* norm2=  this->NormalizedSecondImage->GetPointData()->GetScalars();

  //  fprintf(stderr,"Minimum thresholds both SPECTS =%.2f\n",perthreshold);

  for (int pixel=0;pixel<numscalars;pixel++)
    {
      float v1=in1->GetComponent(pixel,0);
      float v2=in2->GetComponent(pixel,0);

      v2=(v2-this->Shift)/this->Scale;

      if (v1>perthreshold && v2>perthreshold)
	{
	  out->SetComponent(pixel,0,float(v2-v1));
	  norm2->SetComponent(pixel,0,v2);
	}
      else
	{
	  norm2->SetComponent(pixel,0,0.0);
	}
    
      count++;
      if (count==tenth)
	{
	  pog+=0.05;
	  this->UpdateProgress(pog);
	  count=0;
	}
    }


  vtkImageGaussianSmooth* sm=vtkImageGaussianSmooth::New();
  sm->SetInput(tmp);
  sm->SetStandardDeviations(this->Sigma,this->Sigma,this->Sigma);
  sm->Update();
  tmp->Delete();

  output->ShallowCopy(sm->GetOutput());
  sm->Delete();


  vtkDataArray* diff=output->GetPointData()->GetScalars();

  for (int pix=0;pix<numscalars;pix++)
    {
      float d=diff->GetComponent(pix,0);
      float v1=in1->GetComponent(pix,0);

      float p=0.0;

      if (v1>perthreshold)
	p=100.0*d/v1;
      outper->SetComponent(pix,0,p);
      
      count++;
      if (count==tenth)
	{
	  pog+=0.05;
	  this->UpdateProgress(pog);
	  count=0;
	}
    }
  this->UpdateProgress(1.0);
}

