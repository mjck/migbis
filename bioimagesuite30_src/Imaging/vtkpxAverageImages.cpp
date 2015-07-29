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
  Module:    $RCSfile: vtkpxAverageImages.cpp,v $
  Language:  C++
  Date:      $Date: 2002/09/26 18:09:25 $
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
#include "vtkSource.h"
#include "vtkpxAverageImages.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
//------------------------------------------------------------------------------
vtkpxAverageImages* vtkpxAverageImages::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxAverageImages");
  if(ret)
    {
    return (vtkpxAverageImages*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxAverageImages;
}

// Construct object with no children.
vtkpxAverageImages::vtkpxAverageImages()
{
  this->Median=1;
  this->IgnoreFlag=0;
  this->IgnoreValue=-32767.0;
  this->IgnoreValueOutput=0.0;
  this->ComputeStandardDeviation=0;
  this->OutputStandardDeviation=NULL;
  this->SumOnly=0;
}
// ----------------------------------------------------------------------------
vtkpxAverageImages::~vtkpxAverageImages()
{
  if (this->OutputStandardDeviation!=NULL)
    this->OutputStandardDeviation->Delete();
}
// ----------------------------------------------------------------------------
void vtkpxAverageImages::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();
  output->SetScalarTypeToFloat();
}

void vtkpxAverageImages::SimpleExecute(vtkImageData* ,vtkImageData* output)
{

  if (this->GetNumberOfInputPorts()==0 || this->GetImageDataInput(0)==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }

  int dim_0[3];
  this->GetImageDataInput(0)->GetDimensions(dim_0);
  int num_comp=this->GetImageDataInput(0)->GetNumberOfScalarComponents();

  for (int i=1;i<this->GetNumberOfInputPorts();i++)
      {
	if (this->GetImageDataInput(i)==NULL)
	  {
	    vtkErrorMacro(<<"No Input Image Data " << i << " !!");
	    return;
	  }
	else
	  {
	    int dim_1[3];
	    this->GetImageDataInput(i)->GetDimensions(dim_1);
	    int sum=0;
	    for (int kk=0;kk<=2;kk++)
	      sum+=abs(dim_1[kk]-dim_0[kk]);
	    sum+=abs(num_comp-this->GetImageDataInput(i)->GetNumberOfScalarComponents());
	    if (sum!=0)
	      {
		vtkErrorMacro(<<"Multiple Images have different Dimensions !!");
		return;
	      }
	  }
      }


  // Create Output Array
  // -------------------
  int   dim[3];  this->GetImageDataInput(0)->GetDimensions(dim);
  vtkDebugMacro(<< "Averaging Images\n" <<  "Dimensions " << dim[0] <<","<< dim[1] <<","<< dim[2] << "Components " << num_comp << "\n");
  
  output->SetSpacing(this->GetImageDataInput(0)->GetSpacing());
  output->SetOrigin(this->GetImageDataInput(0)->GetOrigin());
  output->SetScalarType(VTK_FLOAT);
  output->SetNumberOfScalarComponents(num_comp);
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  output->AllocateScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();
  int numscalars=out->GetNumberOfTuples();

  this->UpdateProgress(0.01);
  float pog=0.00;

  float tenth=(numscalars*num_comp)/10.0;
  int count=0;

  int domean=1;

  if (this->Median==1 && this->GetNumberOfInputPorts()>=3 && this->ComputeStandardDeviation==0)
    domean=0;

  fprintf(stderr,"domean = %d\n",domean);

  vtkDataArray* outsigma=NULL;
  if (this->ComputeStandardDeviation)
    {
      if (this->OutputStandardDeviation==NULL)
	this->OutputStandardDeviation=vtkImageData::New();
      this->OutputStandardDeviation->SetSpacing(this->GetImageDataInput(0)->GetSpacing());
      this->OutputStandardDeviation->SetOrigin(this->GetImageDataInput(0)->GetOrigin());
      this->OutputStandardDeviation->SetScalarType(VTK_FLOAT);
      this->OutputStandardDeviation->SetNumberOfScalarComponents(num_comp);
      this->OutputStandardDeviation->SetDimensions(dim[0],dim[1],dim[2]);
      this->OutputStandardDeviation->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
      this->OutputStandardDeviation->AllocateScalars();
      outsigma=this->OutputStandardDeviation->GetPointData()->GetScalars();
      outsigma->FillComponent(0,-1.0);
    }


  // If number of inputs < 3 median = mean

  float correction=sqrt((float(this->GetNumberOfInputPorts())))/sqrt(float(this->GetNumberOfInputPorts()-1));

  fprintf(stderr,"Sum Only=%d\n",SumOnly);

  for (int component=0;component<num_comp;component++)
    {
      if (domean)
	{
	  float factor=float(1.0/this->GetNumberOfInputPorts());
	  if (this->SumOnly)
	    factor=1.0;
	  for (int n=0;n<numscalars;n++)
	    {
	      int flag=1;
	      if (this->IgnoreFlag>0)
		{
		  int series=0;
		  while(series < this->GetNumberOfInputPorts() && flag==1) 
		    {
		      if (this->GetImageDataInput(series)->GetPointData()->GetScalars()->GetComponent(n,component)==
			  this->IgnoreValue)
			{
			  flag=0;
			  out->SetComponent(n,component,this->IgnoreValueOutput);		      
			}
		      ++series;
		    }
		}
	      
	      if (flag==1) 
		{
		  float sum=0.0;
		  float sum2=0.0;
		  for (int series=0;series<this->GetNumberOfInputPorts();series++)
		    {
		      float v=this->GetImageDataInput(series)->GetPointData()->GetScalars()->GetComponent(n,component);
		      sum+=v;
		      sum2+=v*v;
		    }
		  float mean =sum*factor;
		  out->SetComponent(n,component,mean);
		  
		  if (this->ComputeStandardDeviation)
		    {
		      float sigma=sqrt(sum2*factor-mean*mean)*correction;
		      outsigma->SetComponent(n,component,sigma);
		    }
		}
	      
	      count++;
	      if (count==tenth)
		{
		  pog+=0.1;
		  this->UpdateProgress(pog);
		  count=0;
		}
	    }
	} 
      else 
	{
	  float *v=new float[this->GetNumberOfInputPorts()+1];
	  
	  int numhalf=int(this->GetNumberOfInputPorts())/2;
	  int odd=1;
	  
	  if (numhalf*2 == this->GetNumberOfInputPorts())
	    odd=0;

	  if (odd)
	    numhalf+=1;

	  //	  fprintf(stderr,"numhalf=%d,odd=%d, numinpts=%d\n",numhalf,odd,this->GetNumberOfInputPorts());
	  
	  for (int n=0;n<numscalars;n++)
	    {
	      int flag=1;
	      for (int series=0;series<this->GetNumberOfInputPorts();series++)
		{
		  v[series]=this->GetImageDataInput(series)->GetPointData()->GetScalars()->GetComponent(n,component);
		  if ( v[series]==this->IgnoreValue && this->IgnoreFlag==1)
		    flag=0;
		}
	      
	      float max=0.0,prevmax=0.0;
	      int position;
	      
	      if (flag==1)
		{
		  for(int outer=0;outer<numhalf;outer++)
		    {
		      max=v[0];
		      position=0;
		      for(int inner=1;inner<this->GetNumberOfInputPorts();inner++)
			{
			  if (v[inner]>max)
			    {
			      prevmax=max;
			      max=v[inner];
			      position=inner;
			    }
			}
		      v[position]=-1.0e+38; // Large Negative Number
		    }
		  
		  if (odd) 
		    {
		      out->SetComponent(n,component,max);
		    } 
		  else 
		    {
		      out->SetComponent(n,component,0.5*(max+prevmax));
		}
		}
	      else 
		out->SetComponent(n,component,this->IgnoreValueOutput);
	      
	      count++;
	      if (count==tenth)
		{
		  pog+=0.1;
		  this->UpdateProgress(pog);
		  count=0;
		}
	    }
	}
    }
  this->UpdateProgress(1.0);
}


