

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
#include "vtkmpjImageDotProduct.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjImageDotProduct* vtkmpjImageDotProduct::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageDotProduct");
  if(ret)
    {
      return (vtkmpjImageDotProduct*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageDotProduct;
}

// Construct object with no children.
vtkmpjImageDotProduct::vtkmpjImageDotProduct()
{
  this->Mask = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjImageDotProduct::~vtkmpjImageDotProduct()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageDotProduct::SetInput1(vtkImageData *input)
{
  this->vtkProcessObject::SetNthInput(0, input);
}

// ----------------------------------------------------------------------------
void vtkmpjImageDotProduct::SetInput2(vtkImageData *input)
{
  this->vtkProcessObject::SetNthInput(1, input);
}

//----------------------------------------------------------------------------
vtkImageData *vtkmpjImageDotProduct::GetInput(int index)
{
  if (this->NumberOfInputs <= index)
    {
      return NULL;
    }
  
  return (vtkImageData *)(this->Inputs[index]);
}

// ----------------------------------------------------------------------------
void vtkmpjImageDotProduct::ExecuteInformation()
{
  vtkImageData *input1=this->GetInput(0);
  vtkImageData *input2=this->GetInput(1);
  vtkImageData *output=this->GetOutput();
  
  if ((input1==NULL) || (input2==NULL)) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  int dim[3];  input1->GetDimensions(dim);
  int nc1=input1->GetNumberOfScalarComponents();
  int nc2=input2->GetNumberOfScalarComponents();
  
  output->SetNumberOfScalarComponents(1);
  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageDotProduct::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  vtkImageData *input1 = this->GetInput(0);
  vtkImageData *input2 = this->GetInput(1);
  int nc = input1->GetNumberOfScalarComponents();
  int dim[3];  input1->GetDimensions(dim);
  
  vtkDataArray* in1=input1->GetPointData()->GetScalars(); 
  vtkDataArray* in2=input2->GetPointData()->GetScalars(); 
  vtkDataArray* out=output->GetPointData()->GetScalars();  
  
  vtkDataArray* mask=NULL;
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  float v1[3], v2[3], sum, dot, angle;
  int index=0;
  int inmask;
  
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;
	  
	  if (inmask) {
	    
	    v1[0] = in1->GetComponent(index, 0);
	    v1[1] = in1->GetComponent(index, 1);
	    v1[2] = in1->GetComponent(index, 2);

	    v2[0] = in2->GetComponent(index, 0);
	    v2[1] = in2->GetComponent(index, 1);
	    v2[2] = in2->GetComponent(index, 2);

	    (void)vtkMath::Normalize(v1);
	    (void)vtkMath::Normalize(v2);

	    dot = v1[0]*v2[0] + v1[1]*v2[1] + v1[2]*v2[2];

	    //angle = (acos(dot) / (2.0*vtkMath::Pi())) * 360.0;
	    //if (angle > 90) angle -= 90.0;

	    out->SetComponent(index, 0, fabs(dot));

	  } else {
	    out->SetComponent(index, 0, 0.0);
	  }
	  
	  ++index;	   	 
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
