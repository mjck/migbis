

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
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkmpjImageInterleaving.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "math.h"


//------------------------------------------------------------------------------
vtkmpjImageInterleaving* vtkmpjImageInterleaving::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageInterleaving");
  if(ret)
    {
    return (vtkmpjImageInterleaving*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageInterleaving;
}

// Construct object with no children.
vtkmpjImageInterleaving::vtkmpjImageInterleaving()
{
  this->Interleaving = VTK_MPJ_FRAMEINTERLEAVE;
  this->NumberOfFrames = 1;
}
// ----------------------------------------------------------------------------
vtkmpjImageInterleaving::~vtkmpjImageInterleaving()
{
}
// ----------------------------------------------------------------------------

void vtkmpjImageInterleaving::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();

  if (input==NULL)
    {
	vtkErrorMacro(<<"No Input Specified!!");
	return;
      }

  vtkImageData *output=this->GetOutput();
  vtkpxSimpleImageToImageFilter::ExecuteInformation();

  int dim[3];  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();

  if (this->Interleaving == VTK_MPJ_SLICEINTERLEAVE ) {    
    output->SetNumberOfScalarComponents(1);
    output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]*nc-1);
  } else {    
    output->SetNumberOfScalarComponents(this->NumberOfFrames);
    output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]/this->NumberOfFrames-1);
  }
    
}

// ----------------------------------------------------------------------------
void vtkmpjImageInterleaving::SimpleExecute(vtkImageData* input,vtkImageData* output)
{

  if (input==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }

  vtkDataArray* in =input->GetPointData()->GetScalars();
  

  int dim[3];  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();

  output->CopyStructure(input);
  
  //  if (this->Interleaving == VTK_MPJ_SLICEINTERLEAVE ) {    
  // output->SetNumberOfScalarComponents(this->NumberOfFrames);
  // output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]nc-1);
  // output->SetDimensions(dim[0],dim[1],dim[2]*nc);
  //} else {    
  output->SetNumberOfScalarComponents(this->NumberOfFrames);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]/this->NumberOfFrames-1);
  output->SetDimensions(dim[0],dim[1],dim[2]/this->NumberOfFrames);
  //}
  
  output->AllocateScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();
  
  if (this->Interleaving == VTK_MPJ_SLICEINTERLEAVE) {
    // VTK_MPJ_SLICEINTERLEAVE    
    
    for (int k=0;k<dim[2];k++) {
      int offset = k*dim[0]*dim[1];
      int noffset = (k / this->NumberOfFrames)*dim[0]*dim[1];
      int index = 0;
      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {		    
	    out->SetComponent(noffset+index, k % this->NumberOfFrames, 
			      in->GetComponent(offset+index,0));
	    ++index;
	  }
    }
    
  } else {
    
    int count = 0;
    int size = dim[0]*dim[1]*(dim[2] / this->NumberOfFrames);    
    for (int k=0;k<dim[2];k++) { 
      for (int j=0;j<dim[1];j++)
	for (int i=0;i<dim[0];i++)
	  {
	    out->SetComponent(count % size,count / size,in->GetComponent(count,0));
	    count++;
	  }
    }
    
    // VTK_MPJ_FRAMEINTERLEAVE
  }
  
  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
