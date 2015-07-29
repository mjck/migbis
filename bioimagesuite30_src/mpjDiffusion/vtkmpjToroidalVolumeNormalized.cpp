

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
#include "vtkmpjToroidalVolumeNormalized.h"
#include "vtkImageAccumulate.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjToroidalVolumeNormalized* vtkmpjToroidalVolumeNormalized::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjToroidalVolumeNormalized");
  if(ret)
    {
      return (vtkmpjToroidalVolumeNormalized*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjToroidalVolumeNormalized;
}

// Construct object with no children.
vtkmpjToroidalVolumeNormalized::vtkmpjToroidalVolumeNormalized()
{
  this->Mask = NULL;
  this->MaskOutValue = 0;
  this->TopPercentage = 1.0;
  this->BottomPercentage = 0.0;
  this->OutMaxValue = 1.0;
  this->OutMinValue = 0.0;
}

// ----------------------------------------------------------------------------
vtkmpjToroidalVolumeNormalized::~vtkmpjToroidalVolumeNormalized()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjToroidalVolumeNormalized::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  int dim[3];  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();
  
  output->SetNumberOfScalarComponents(2);
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjToroidalVolumeNormalized::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkDataArray* in =input->GetPointData()->GetScalars();
 
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* mask =NULL;
  
  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
  
  vtkImageAccumulate *hist = vtkImageAccumulate::New();
  hist->SetInput(input);
  hist->Update();

  double min[3];
  hist->GetMin(min);
  
  double max[3];
  hist->GetMax(max);

  double new_min = min[0] + (max[0] - min[0]) * this->BottomPercentage;
  double new_max = max[0] - (max[0] - min[0]) * (1.0 - this->TopPercentage);

  hist->Delete();

  int index=0;
  float value;

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
		
		float value = in->GetComponent(index, 0);

		if (value < new_min)
			value = this->OutMinValue;
		else if (value > new_max)
			value = this->OutMaxValue;
		else
			value = this->OutMinValue + 
			(this->OutMaxValue - this->OutMinValue) * 
			((value - new_min) / (new_max - new_min));
	     
	    int output=1;

	    output = (mask != NULL)?(mask->GetComponent(index, 0) > 0):1;
	  
	    if (output)
	    {
			out->SetComponent(index, 0, value);   
			out->SetComponent(index, 1, this->OutMaxValue - value);
	    } else {
			out->SetComponent(index, 0, this->MaskOutValue);	
			out->SetComponent(index, 1, this->MaskOutValue);	
	    }	 	
	  
	    ++index;	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);

}
// ----------------------------------------------------------------------------
