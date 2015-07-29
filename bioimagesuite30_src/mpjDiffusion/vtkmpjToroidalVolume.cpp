

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
#include "vtkmpjToroidalVolume.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjToroidalVolume* vtkmpjToroidalVolume::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjToroidalVolume");
  if(ret)
    {
      return (vtkmpjToroidalVolume*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjToroidalVolume;
}

// Construct object with no children.
vtkmpjToroidalVolume::vtkmpjToroidalVolume()
{
  this->Mask = NULL;
  this->MaskOutValue = 0;
}

// ----------------------------------------------------------------------------
vtkmpjToroidalVolume::~vtkmpjToroidalVolume()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjToroidalVolume::ExecuteInformation()
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
  
  if (nc < 3) {
    vtkErrorMacro(<< "Three eigenvalues per voxel are required as input!");
    return;
  }
  
  output->SetNumberOfScalarComponents(4);
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjToroidalVolume::SimpleExecute(vtkImageData *input, vtkImageData *output)
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
  
  int index=0;
  
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	
	  float l1 = in->GetComponent(index, 0);
	  float l2 = in->GetComponent(index, 1);
	  float l3 = in->GetComponent(index, 2);

	  float alpha = (l2 + l3) / 2.0;
	  float beta = l3 / 2.0;
	  float gamma = l1 / 2.0;
	  float PI = vtkMath::Pi();
	  float TV, TV_ADC, TVR, TVF;
	  float ADC = (l1 + l2 + l3) / 3.0;
	  
	  int output=1;
	
	  if (l1 < 0 || l2 < 0 || l3 < 0) {
	    printf("l1=%f, l2=%f, l3=%f @ %d,%d,%d\n",l1,l2,l3,i,j,k);
	  }

	  output = (mask != NULL)?(mask->GetComponent(index, 0) > 0):1;
	  
	  if (output)
	    {

			TV = ((PI * l1) / 3.0) * (l2*l3 + (l3*l3) / 2.0);			
			TV_ADC = (PI / 2.0) * ADC * ADC * ADC;
			TVR = (TV / TV_ADC);			
			TVF = 1.0 - TVR;

	    } else {
			TV = TV_ADC = TVR = TVF = this->MaskOutValue;	      
	    }
	  
	    out->SetComponent(index, 0, TV);     
		out->SetComponent(index, 1, TV_ADC);     
		out->SetComponent(index, 2, TVR);     
		out->SetComponent(index, 3, TVF);     
	  
	    ++index;	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);

}
// ----------------------------------------------------------------------------
