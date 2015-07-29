

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
#include "vtkmpjImageTensorInvariants.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjImageTensorInvariants* vtkmpjImageTensorInvariants::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageTensorInvariants");
  if(ret)
    {
      return (vtkmpjImageTensorInvariants*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageTensorInvariants;
}

// Construct object with no children.
vtkmpjImageTensorInvariants::vtkmpjImageTensorInvariants()
{
  this->Mask = NULL;
  this->Operation = VTK_MPJ_TENSORINV_ALL;
  this->MaskOutValue = 0;
}

// ----------------------------------------------------------------------------
vtkmpjImageTensorInvariants::~vtkmpjImageTensorInvariants()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorInvariants::ExecuteInformation()
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
  
  if (this->Operation == VTK_MPJ_TENSORINV_ALL) 
    output->SetNumberOfScalarComponents(5);
  else output->SetNumberOfScalarComponents(1);
  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorInvariants::SimpleExecute(vtkImageData *input, vtkImageData *output)
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
  
  float sqrt1over2 = sqrt(1.0/2.0); 

  int index=0;
  
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	
	  float l1 = in->GetComponent(index, 0);
	  float l2 = in->GetComponent(index, 1);
	  float l3 = in->GetComponent(index, 2);
	  float ml = (l1 + l2 + l3) / 3.0;
	  float l1m = l1 - ml;
	  float l2m = l2 - ml;
	  float l3m = l3 - ml;
	  float FA, FAml, VR, MD, SK;
	  int output=1;
	
	  if (l1 < 0 || l2 < 0 || l3 < 0) {
	    printf("l1=%f, l2=%f, l3=%f @ %d,%d,%d\n",l1,l2,l3,i,j,k);
	  }

	  output = (mask != NULL)?(mask->GetComponent(index, 0) > 0):1;
	  
	  if (output)
	    {
	      FA = sqrt1over2 *
		(sqrt((l1-l2)*(l1-l2) + (l2-l3)*(l2-l3) + (l1-l3)*(l1-l3)) / 
		 sqrt(l1*l1 + l2*l2 + l3*l3));
	      FAml = 
		sqrt(3*(l1m*l1m + l2m*l2m + l3m*l3m)) / 
		sqrt(2*(l1*l1 + l2*l2 + l3*l3));	    
	      //RA = sqrt(l1m*l1m + l2m*l2m + l3m*l3m) / (sqrt(double(3.0))*ml);
	      VR = 1.0 - ((l1*l2*l3) / (ml*ml*ml));
	      SK = (l1m*l1m*l1m + l2m*l2m*l2m + l3m*l3m*l3m) / (l1+l2+l3);
	      MD = ml;
	    } else {
	      FA = FAml = VR = MD = SK = this->MaskOutValue;	      
	    }
	  
	  // Invariant information
	  switch(op) 
	    {
	    case VTK_MPJ_TENSORINV_FA:
	      out->SetComponent(index, 0, FA);	   
	      break;
	    case VTK_MPJ_TENSORINV_RA:
	      out->SetComponent(index, 0, FAml);
	      break;
	    case VTK_MPJ_TENSORINV_VR:
	      out->SetComponent(index, 0, VR);
	      break;
	    case VTK_MPJ_TENSORINV_MD:
	      out->SetComponent(index, 0, MD);
	      break;
	    case VTK_MPJ_TENSORINV_SK:
	      out->SetComponent(index, 0, SK);
	    default:
	      out->SetComponent(index, 0, FA);     /* fractional anisotropy*/
	      out->SetComponent(index, 1, FAml);     /* relative anisotropy */
	      out->SetComponent(index, 2, VR);     /* 1 - volume ratio */
	      out->SetComponent(index, 3, MD);     /* mean diffusity */
	      out->SetComponent(index, 4, SK);
	    };
	  
	  ++index;	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);

}
// ----------------------------------------------------------------------------
