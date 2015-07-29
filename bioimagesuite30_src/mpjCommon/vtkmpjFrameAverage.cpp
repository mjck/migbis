

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
#include "vtkmpjFrameAverage.h"

//------------------------------------------------------------------------------
vtkmpjFrameAverage* vtkmpjFrameAverage::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjFrameAverage");
  if(ret)
    {
      return (vtkmpjFrameAverage*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjFrameAverage;
}

// Construct object with no children.
vtkmpjFrameAverage::vtkmpjFrameAverage()
{
}

// ----------------------------------------------------------------------------
vtkmpjFrameAverage::~vtkmpjFrameAverage()
{
}

// ----------------------------------------------------------------------------
void vtkmpjFrameAverage::AddInput(vtkImageData *input)
{
  this->vtkProcessObject::AddInput(input);
}

//----------------------------------------------------------------------------
void vtkmpjFrameAverage::SetInput(int idx, vtkImageData *input)
{
  this->vtkProcessObject::SetNthInput(idx, input);
}

//----------------------------------------------------------------------------
vtkImageData *vtkmpjFrameAverage::GetInput()
{
  return this->GetInput(0);
}

//----------------------------------------------------------------------------
vtkImageData *vtkmpjFrameAverage::GetInput(int idx)
{
  if (this->NumberOfInputs <= idx)
    {
      return NULL;
    }
  
  return (vtkImageData*)(this->Inputs[idx]);
}

// ----------------------------------------------------------------------------
void vtkmpjFrameAverage::ExecuteInformation()
 {   
   this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
   this->GetOutput()->SetScalarType(VTK_FLOAT);
 }

// ----------------------------------------------------------------------------

void vtkmpjFrameAverage::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->NumberOfInputs==0 || this->GetInput(0)==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }
  
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
	      vtkErrorMacro(<<"Multiple Images have different Dimensions !!");
	      return;
	    }
	}
    }
  

  // Create Output Array
  // -------------------
  int dim[3];  
  input->GetDimensions(dim);
  int numframes = input->GetNumberOfScalarComponents();
  
  vtkDebugMacro(<< "Averaging Frames\n" <<  "Dimensions " << dim[0] <<","<< dim[1] <<","<< dim[2] << "\n");
  
  /*vtkStructuredPoints* output=this->GetOutput();
    output->SetSpacing(this->GetInput(0)->GetSpacing());
    output->SetOrigin(this->GetInput(0)->GetOrigin());
    output->SetScalarType(VTK_FLOAT);
    output->SetNumberOfScalarComponents(numframes);
    output->SetDimensions(dim[0],dim[1],dim[2]);
    output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
    output->AllocateScalars();*/

  vtkDataArray* out=output->GetPointData()->GetScalars();

  int numscalars=out->GetNumberOfTuples();
  int count = 0;
  float pog = 0.0;
  int tenth= (int)(numscalars*numframes / 10.0);

  this->UpdateProgress(0.01);

  for (int comp=0;comp<numframes;comp++)
    for (int n=0;n<numscalars;n++)      
      {
	float sum=0.0;
	float v;
	//int c=0;
	
	for (int series=0;series<this->NumberOfInputs;series++)
	  {
	      v = GetInput(series)->GetPointData()->GetScalars()->GetComponent(n,comp);	    
	      sum += v;
	  }
	
	float mean = sum / float(this->NumberOfInputs);

	out->SetComponent(n,comp,mean);
     	
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

