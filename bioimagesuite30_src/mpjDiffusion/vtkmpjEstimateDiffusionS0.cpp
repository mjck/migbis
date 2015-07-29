

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
#include "vtkmpjEstimateDiffusionS0.h"
//#include "vtkMath.h"
#include <math.h>

//------------------------------------------------------------------------------
vtkmpjEstimateDiffusionS0* vtkmpjEstimateDiffusionS0::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjEstimateDiffusionS0");
  if(ret)
    {
      return (vtkmpjEstimateDiffusionS0*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjEstimateDiffusionS0;
}

// Construct object with no children.
vtkmpjEstimateDiffusionS0::vtkmpjEstimateDiffusionS0()
{
  this->LowBValue = 100;
  this->LowBValueIndex = 0;
  this->HighBValue = 1000;
  this->HighBValueIndex = 1;
}

// ----------------------------------------------------------------------------
vtkmpjEstimateDiffusionS0::~vtkmpjEstimateDiffusionS0()
{
}

// ----------------------------------------------------------------------------
void vtkmpjEstimateDiffusionS0::ExecuteInformation()
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
  
  output->SetNumberOfScalarComponents(1);
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjEstimateDiffusionS0::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkDataArray* in =input->GetPointData()->GetScalars();
 
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  
  int index=0;
  
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	    {	
	
	     float SLow = in->GetComponent(index, this->LowBValueIndex);
	     float SHigh = in->GetComponent(index, this->HighBValueIndex);

       float delta = (log(SHigh) - log(SLow)) / (this->LowBValue - this->HighBValue);
       float lnS0 = log(SLow) + this->LowBValue * delta;
    
        //if (!isnan(lnS0))
        //   printf("[%f, %f], %f, %f\n",this->LowBValue,this->HighBValue, lnS0,exp(lnS0));
        if (!isnan(lnS0) && !isinf(lnS0))
	         out->SetComponent(index, 0, exp(lnS0)); 
        else
           out->SetComponent(index, 0, 0.0); 
	 
	    ++index;	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);

}
// ----------------------------------------------------------------------------
