

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
#include "vtkmpjImageVesselMaximum.h"

//------------------------------------------------------------------------------
vtkmpjImageVesselMaximum* vtkmpjImageVesselMaximum::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageVesselMaximum");
  if(ret)
    {
      return (vtkmpjImageVesselMaximum*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageVesselMaximum;
}

// Construct object with no children.
vtkmpjImageVesselMaximum::vtkmpjImageVesselMaximum()
{
}

// ----------------------------------------------------------------------------
vtkmpjImageVesselMaximum::~vtkmpjImageVesselMaximum()
{
}

// ----------------------------------------------------------------------------
void vtkmpjImageVesselMaximum::AddInput(vtkImageData *input)
{
  this->vtkProcessObject::AddInput(input);
}

//----------------------------------------------------------------------------
void vtkmpjImageVesselMaximum::SetInput(int idx, vtkImageData *input)
{
  this->vtkProcessObject::SetNthInput(idx, input);
}

//----------------------------------------------------------------------------
vtkImageData *vtkmpjImageVesselMaximum::GetInput()
{
  return this->GetInput(0);
}

//----------------------------------------------------------------------------
vtkImageData *vtkmpjImageVesselMaximum::GetInput(int idx)
{
  if (this->NumberOfInputs <= idx)
    {
      return NULL;
    }
  
  return (vtkImageData*)(this->Inputs[idx]);
}

// ----------------------------------------------------------------------------
void vtkmpjImageVesselMaximum::ExecuteInformation()
{
  if (this->NumberOfInputs==0 || this->GetInput(0)==NULL) {
    vtkErrorMacro(<<"No Input Image Data !!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();  
}

// ----------------------------------------------------------------------------

void vtkmpjImageVesselMaximum::SimpleExecute(vtkImageData *input, vtkImageData *output)
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

  int numscalars=out->GetNumberOfTuples();
  int numcomp=output->GetNumberOfScalarComponents();
  int nc = this->GetInput(0)->GetNumberOfScalarComponents();
  
  int count = 0;
  float pog = 0.0;
  int tenth= (int)(numscalars / 10.0);
  
  this->UpdateProgress(0.01);
  
  vtkDataArray *in0 = this->GetInput(0)->GetPointData()->GetScalars();
  
  for(int n=0; n<numscalars; n++) 
    {
      double max = in0->GetComponent(n, 0);    
      int sermax = 0;
      for(int series=1; series<this->NumberOfInputs; series++) 
	{            
	  double v = this->GetInput(series)->GetPointData()->GetScalars()->GetComponent(n, 0);      
	  
	  if (v > max) 
	    {
	      max=v; 
	      sermax=series;   
	    }
	}
    
      // save maximum
      out->SetComponent(n, 0, max);
      
      if ( max > 0 ) 
	{
	  // save corresponding sigma
	  out->SetComponent(n, 1, this->GetInput(sermax)->GetPointData()->GetScalars()->GetComponent(n,1));
      
	  // save tensor
	  for(int i=2; i<8; i++) 
	    out->SetComponent(n, i, this->GetInput(sermax)->GetPointData()->GetScalars()->GetComponent(n,i));
	} 
      else 
	{
	  // save sigma
	  out->SetComponent(n, 1, 0.0);
	  // save tensor
	  for(int i=2; i<8; i++) 
	    out->SetComponent(n, i, 0.0);
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

