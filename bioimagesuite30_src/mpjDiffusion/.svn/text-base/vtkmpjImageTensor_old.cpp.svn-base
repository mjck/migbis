
#include "vtkObjectFactory.h"
#include "vtkmpjImageTensor.h"
#include "vtkMath.h"
#include "math.h"


//------------------------------------------------------------------------------
vtkmpjImageTensor* vtkmpjImageTensor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageTensor");
  if(ret)
    {
      return (vtkmpjImageTensor*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageTensor;
}

// ----------------------------------------------------------------------------
vtkmpjImageTensor::vtkmpjImageTensor()
{
  this->Baseline = NULL;
  this->Mask = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjImageTensor::~vtkmpjImageTensor()
{
  if (this->Baseline != NULL)
    this->Baseline->Delete();
  if (this->Mask != NULL)
    this->Mask->Delete();  
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensor::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();

  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  if (this->Baseline == NULL) {
    vtkErrorMacro(<<"No Baseline image specified!");
  }
    
  int dim[3];  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();
  
  if (nc < 6) {
    vtkErrorMacro(<<"There should be exactly 6 diffusion-weighted images in the dataset!");
    return;
  }
  
  output->SetNumberOfScalarComponents(6);
  output->SetScalarType(VTK_FLOAT);
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensor::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
   if (this->GetInput()==NULL)
   {
    vtkErrorMacro(<<"No Input Image\n Exiting\n");
    return;
  }

   vtkDataArray* in =input->GetPointData()->GetScalars();
   int dim[3];  input->GetDimensions(dim);
   
  vtkDataArray* out=output->GetPointData()->GetScalars();
  
  vtkDataArray* baseline=NULL;
  vtkDataArray* mask=NULL;
  
  if (this->Baseline != NULL) 
    baseline = this->Baseline->GetPointData()->GetScalars();
  
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  /*
    Compute IM[i] images:
    IM_i = ln (A(G^i) / A(G^0 = 0)) 
  */
  
  int index=0;

  for (int k=0;k<dim[2];k++) {
    
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  int output = 1;
	  float baseln;
	  
	  baseln = float(baseline->GetComponent(index, 0));
	  output = (mask != NULL)?(mask->GetComponent(index, 0) > 0):1;
	  
	  if (output) {
	    
	    float IM1 =  log(float(in->GetComponent(index, 0))) - log(baseln); 	    
	    float IM2 =  log(float(in->GetComponent(index, 1))) - log(baseln); 
	    float IM3 =  log(float(in->GetComponent(index, 2))) - log(baseln); 
	    float IM4 =  log(float(in->GetComponent(index, 3))) - log(baseln); 
	    float IM5 =  log(float(in->GetComponent(index, 4))) - log(baseln); 
	    float IM6 =  log(float(in->GetComponent(index, 5))) - log(baseln); 
	    
	    float Dxx = -IM1 - IM2 + IM3 + IM4 - IM5 - IM6;
	    float Dxy = -IM5 + IM6;
	    float Dxz = -IM1 + IM2;
	    float Dyy = +IM1 + IM2 - IM3 - IM4 - IM5 - IM6;
	    float Dyz = -IM3 + IM4;
	    float Dzz = -IM1 - IM2 - IM3 - IM4 + IM5 + IM6;
	    
	    out->SetComponent(index, 0, Dxx);
	    out->SetComponent(index, 1, Dxy);
	    out->SetComponent(index, 2, Dxz);
	    out->SetComponent(index, 3, Dyy);
	    out->SetComponent(index, 4, Dyz);
	    out->SetComponent(index, 5, Dzz);
	  } else {

	    for(int z=0;z<6;z++) 
	      out->SetComponent(index, z, (float)0);
	  }	  
	  
	  ++index;	  
	}

    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);
}

// ----------------------------------------------------------------------------
