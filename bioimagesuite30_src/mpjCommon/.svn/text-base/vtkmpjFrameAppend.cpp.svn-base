

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



#include "vtkObjectFactory.h"
#include "vtkmpjFrameAppend.h"

//------------------------------------------------------------------------------
vtkmpjFrameAppend* vtkmpjFrameAppend::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjFrameAppend");
  if(ret)
    {
      return (vtkmpjFrameAppend*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjFrameAppend;
}

// Construct object with no children.
vtkmpjFrameAppend::vtkmpjFrameAppend()
{
}

// ----------------------------------------------------------------------------
vtkmpjFrameAppend::~vtkmpjFrameAppend()
{
}

// ----------------------------------------------------------------------------
void vtkmpjFrameAppend::AddInput(vtkImageData *input)
{
  this->vtkProcessObject::AddInput(input);
}

//----------------------------------------------------------------------------
void vtkmpjFrameAppend::SetInput(int idx, vtkImageData *input)
{
  this->vtkProcessObject::SetNthInput(idx, input);
}

//----------------------------------------------------------------------------
vtkImageData *vtkmpjFrameAppend::GetInput()
{
  return this->GetInput(0);
}

//----------------------------------------------------------------------------
vtkImageData *vtkmpjFrameAppend::GetInput(int idx)
{
  if (this->NumberOfInputs <= idx)
    {
      return NULL;
    }
  
  return (vtkImageData*)(this->Inputs[idx]);
}

// ----------------------------------------------------------------------------
void vtkmpjFrameAppend::ExecuteInformation()
{
  if (this->NumberOfInputs==0 || this->GetInput(0)==NULL) {
    vtkErrorMacro(<<"No Input Image Data !!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  int ncomp=0;
  for(int i=0; i<this->NumberOfInputs; i++) 
      ncomp += this->GetInput(i)->GetNumberOfScalarComponents();
  
  vtkImageData *output=this->GetOutput();
  int dim[3];  this->GetInput(0)->GetDimensions(dim);
  
  output->CopyStructure(this->GetInput(0));
  output->SetNumberOfScalarComponents(ncomp);
}

// ----------------------------------------------------------------------------

void vtkmpjFrameAppend::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int dim_0[3];
  this->GetInput(0)->GetDimensions(dim_0);
  
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
	  if (sum!=0)
	    {
	      vtkErrorMacro(<<"Images have different Dimensions !!");
	      return;
	    }
	}
    }
  
  vtkDataArray* out=output->GetPointData()->GetScalars();

  int numcomp=output->GetNumberOfScalarComponents();

  this->UpdateProgress(0.01);

  int noutcomp = 0;
  for (int i=0; i<this->NumberOfInputs; i++) 
    {
      vtkDataArray *in=this->GetInput(i)->GetPointData()->GetScalars();
      int nc = this->GetInput(i)->GetNumberOfScalarComponents();
    
      for (int comp=0;comp<nc;comp++) 
	{
	  out->CopyComponent(noutcomp,in,comp);
	  noutcomp++;
	}
    }
  this->UpdateProgress(1.0);
}

