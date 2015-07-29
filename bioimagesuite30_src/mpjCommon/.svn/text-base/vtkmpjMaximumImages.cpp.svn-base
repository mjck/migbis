

//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
//BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkmpjMaximumImages.cpp,v $
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
#include "vtkmpjMaximumImages.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

vtkStandardNewMacro(vtkmpjMaximumImages);

// Construct object with no children.
vtkmpjMaximumImages::vtkmpjMaximumImages()
{
}

// ----------------------------------------------------------------------------
vtkmpjMaximumImages::~vtkmpjMaximumImages()
{
}

// ----------------------------------------------------------------------------
void vtkmpjMaximumImages::AddInput(vtkImageData *input)
{
  this->vtkProcessObject::AddInput(input);
}

//----------------------------------------------------------------------------
void vtkmpjMaximumImages::SetInput(int idx, vtkImageData *input)
{
  this->vtkProcessObject::SetNthInput(idx, input);
}

//----------------------------------------------------------------------------
vtkImageData *vtkmpjMaximumImages::GetInput()
{
  return this->GetInput(0);
}

//----------------------------------------------------------------------------
vtkImageData *vtkmpjMaximumImages::GetInput(int idx)
{
  if (this->NumberOfInputs <= idx)
    {
      return NULL;
    }
  
  return (vtkImageData*)(this->Inputs[idx]);
}

// ----------------------------------------------------------------------------
void vtkmpjMaximumImages::SimpleExecute(vtkImageData *input ,vtkImageData* output)
{
  if (this->NumberOfInputs==0 || this->GetInput(0)==NULL)
    {
      vtkErrorMacro(<<"No Input Image Data !!");
      return;
    }
  
  int dim_0[3];
  this->GetInput(0)->GetDimensions(dim_0);
  int num_comp=this->GetInput(0)->GetNumberOfScalarComponents();

  for (int i=1;i<this->NumberOfInputs;i++)
      {
	if (this->GetInput(i)==NULL)
	  {
	    vtkErrorMacro(<<"No Input Image Data " << i << " !!");
	    return;
	  }
	else
	  {
	    int dim_1[3];
	    this->GetInput(i)->GetDimensions(dim_1);
	    int sum=0;
	    for (int kk=0;kk<=2;kk++)
	      sum+=abs(dim_1[kk]-dim_0[kk]);
	    sum+=abs(num_comp-this->GetInput(i)->GetNumberOfScalarComponents());
	    if (sum!=0)
	      {
		vtkErrorMacro(<<"Multiple Images have different Dimensions !!");
		return;
	      }
	  }
      }


  // Create Output Array
  // -------------------
  int   dim[3];  this->GetInput(0)->GetDimensions(dim);
  vtkDebugMacro(<< "Averaging Images\n" <<  "Dimensions " << dim[0] <<","<< dim[1] <<","<< dim[2] << "Components " << num_comp << "\n");

  output->CopyStructure(this->GetInput(0));
  output->SetNumberOfScalarComponents(this->GetInput(0)->GetNumberOfScalarComponents());
  output->AllocateScalars();

  vtkDataArray* out=output->GetPointData()->GetScalars();
  int numscalars=out->GetNumberOfTuples();

  this->UpdateProgress(0.01);
  float pog=0.00;

  float tenth=(numscalars*num_comp)/10.0;
  int count=0;

  int domean=1;

  for (int component=0;component<num_comp;component++)
    {
      for (int n=0;n<numscalars;n++)
	{
	  float max=this->GetInput(0)->GetPointData()->GetScalars()->GetComponent(n,component);
	  for (int series=1;series<this->NumberOfInputs;series++)
	    {
	      float v=this->GetInput(series)->GetPointData()->GetScalars()->GetComponent(n,component);
	      if (v>max)
		max=v;
	    }
	  out->SetComponent(n,component,max);
	}
      
      count++;
      if (count==tenth)
	{
	  pog+=0.1;
	  this->UpdateProgress(pog);
	  count=0;
	}
    }
  this->UpdateProgress(1.0);
}
