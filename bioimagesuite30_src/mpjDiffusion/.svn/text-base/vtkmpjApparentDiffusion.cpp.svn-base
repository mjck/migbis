

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
#include "vtkmpjApparentDiffusion.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjApparentDiffusion* vtkmpjApparentDiffusion::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjApparentDiffusion");
  if(ret)
    {
      return (vtkmpjApparentDiffusion*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjApparentDiffusion;
}

// ----------------------------------------------------------------------------
vtkmpjApparentDiffusion::vtkmpjApparentDiffusion()
{
  this->ReplaceScalars = 1;
  this->Baseline = NULL;
  this->Mask = NULL;
  this->Shift = 0.0;
  this->bValue = 1.0;
}

// ----------------------------------------------------------------------------
vtkmpjApparentDiffusion::~vtkmpjApparentDiffusion()
{
  if (this->Baseline != NULL)
  this->Baseline->Delete();
  if (this->Mask != NULL)
    this->Mask->Delete();  
}

// ----------------------------------------------------------------------------
void vtkmpjApparentDiffusion::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  if (this->Baseline == NULL) {
   vtkErrorMacro(<<"No baseline (S0) image specified!");
   return;
  }
  
  int dim[3];  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();
    
  output->SetNumberOfScalarComponents(nc);
  output->SetScalarType(VTK_FLOAT);
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjApparentDiffusion::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkPointData *inPD = input->GetPointData();
  vtkDataArray* in = input->GetPointData()->GetScalars();
  vtkIdType numTuples = in->GetNumberOfTuples();
  int dim[3];  input->GetDimensions(dim);
  
  vtkPointData *outPD=output->GetPointData();
  vtkDataArray *out=outPD->GetScalars();
  vtkFloatArray *diffusion=NULL;
  vtkDataArray* baseline=NULL;
  vtkDataArray* mask=NULL;
  
  if (this->ReplaceScalars == 0) {
    diffusion = vtkFloatArray::New();
    diffusion->SetName("diffusion");
    diffusion->SetNumberOfComponents(in->GetNumberOfComponents());
    diffusion->SetNumberOfTuples(numTuples);
  }
  
  if (this->Baseline != NULL) 
    baseline = this->Baseline->GetPointData()->GetScalars();
  
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  int nc=in->GetNumberOfComponents();
  
  int index=0;

  for (int k=0;k<dim[2];k++) {    
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  int output = 1;
	  float baseln;
	  
	  baseln = float(baseline->GetComponent(index, 0)) + this->Shift;
	  output = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;
	  
	  if (output) {
	    
	    int abort = 0;	    	 

	    // Calculate ADCs
	    for(int n=0; n<nc && !abort; n++) {	      
	      float value = float(in->GetComponent(index, n)) + this->Shift;
	      abort = (value <= 0);
	      float adc = log(baseln / value) / this->bValue; 
	      if (this->ReplaceScalars) {
		out->SetComponent(index, n, adc);
	      } else {
		diffusion->SetComponent(index, n, adc);
	      }
	    }
	    
	    if (abort) {
	      for(int n=0; n<nc; n++) {
		if (this->ReplaceScalars) {
		  out->SetComponent(index, n, 0);
		} else {
		  diffusion->SetComponent(index, n, 0);
		}
	      }
	    }

	  }	  
	  
	  ++index;	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }

  // If not to replace data, simply add the new field
  if (this->ReplaceScalars == 0) {

    outPD->AddArray(diffusion);
    diffusion->Delete();

    outPD->PassData(inPD);
  }


  this->UpdateProgress(1.0);
   
}

// ----------------------------------------------------------------------------
