

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
#include "vtkmpjTensorGeometryMetrics.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjTensorGeometryMetrics* vtkmpjTensorGeometryMetrics::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjTensorGeometryMetrics");
  if(ret)
    {
      return (vtkmpjTensorGeometryMetrics*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjTensorGeometryMetrics;
}

// Construct object with no children.
vtkmpjTensorGeometryMetrics::vtkmpjTensorGeometryMetrics()
{
  this->Mask = NULL;
  this->Operation = VTK_MPJ_TENSOR_GEOM_ALL;
  this->MaskOutValue = 0;
}

// ----------------------------------------------------------------------------
vtkmpjTensorGeometryMetrics::~vtkmpjTensorGeometryMetrics()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjTensorGeometryMetrics::ExecuteInformation()
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
  
  if (this->Operation == VTK_MPJ_TENSOR_GEOM_ALL) 
    output->SetNumberOfScalarComponents(3);
  else output->SetNumberOfScalarComponents(1);
  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjTensorGeometryMetrics::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkDataArray* in =input->GetPointData()->GetScalars();
 
  int dim[3];  input->GetDimensions(dim);
  int op = this->GetOperation();
  
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
	  float TR = l1 + l2 + l3;
	  float L, P, S;
	  int output=1;
	  
	  output = (mask != NULL)?(mask->GetComponent(index, 0) > 0):1;
	  if (output)
	    {
	      L = (l1 - l2) / TR;
	      P = (2.0 * (l2 - l3)) / TR;
	      S = (3.0 * l3) / TR;
	    } else {
	      L = P = S = this->MaskOutValue;	      
	    }
	  
	  // Invariant information
	  switch(op) 
	    {
	    case VTK_MPJ_TENSOR_GEOM_L:
	      out->SetComponent(index, 0, L);	   
	      break;
	    case VTK_MPJ_TENSOR_GEOM_P:
	      out->SetComponent(index, 0, P);
	      break;
	    case VTK_MPJ_TENSOR_GEOM_S:
	      out->SetComponent(index, 0, S);
	      break;
	    default:
	      out->SetComponent(index, 0, L);     /* linear diffusion */
	      out->SetComponent(index, 1, P);     /* planar diffusion */
	      out->SetComponent(index, 2, S);     /* spherical diffusion */
	    };
	  
	  ++index;	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);

}
// ----------------------------------------------------------------------------
