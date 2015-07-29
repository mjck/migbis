

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
#include "vtkmpjImageBoundary.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjImageBoundary* vtkmpjImageBoundary::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageBoundary");
  if(ret)
    {
      return (vtkmpjImageBoundary*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageBoundary;
}

// Construct object with no children.
vtkmpjImageBoundary::vtkmpjImageBoundary()
{
  this->Mask = NULL;
  this->BoundaryOutValue = 200;
  this->MaskOutValue = 100;
  this->BoundaryPoints = NULL;
  this->Boundary2D = 0;
}

// ----------------------------------------------------------------------------
vtkmpjImageBoundary::~vtkmpjImageBoundary()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageBoundary::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  int dim[3];  input->GetDimensions(dim);
  //int nc=input->GetNumberOfScalarComponents();
  
  output->SetNumberOfScalarComponents(1);
  output->SetScalarType(VTK_SHORT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageBoundary::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkDataArray* in =input->GetPointData()->GetScalars();
 
  int dim[3];  input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  //int factor[3] = {1, dim[0], dim0Tdim1};
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* mask =NULL;
  
  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
  
  if (this->BoundaryPoints) 
    this->BoundaryPoints->SetNumberOfComponents(3);

  int index=0;
  float value, neighbor[6];
  
  for (int k=0;k<dim[2];k++) 
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{		    
	  value = in->GetComponent(index, 0);
	  float coord[3] = {i, j, k};
	  
	  out->SetComponent(index, 0, this->MaskOutValue);

	  // check if it is a boundary point
	  if (value > 0.0) {
	    
	    if (this->Boundary2D == 0) {
	      if (k > 0)
		neighbor[0] = in->GetComponent(dim0Tdim1*(k-1) + dim[0]*j + i, 0);
	      else neighbor[0] = 0.0;	    
	      if (k < dim[2]-1)
		neighbor[1] = in->GetComponent(dim0Tdim1*(k+1) + dim[0]*j + i, 0);	    
	      else neighbor[1] = 0.0;
	    } else {
	      neighbor[0] = 0.0;
	      neighbor[1] = 0.0;
	    }
	    
	    if (j > 0) 
	      neighbor[2] = in->GetComponent(dim0Tdim1*k + dim[0]*(j-1) + i, 0);
	    else neighbor[2] = 0.0;	    
	    if (j < dim[1]-1)
	      neighbor[3] = in->GetComponent(dim0Tdim1*k + dim[0]*(j+1) + i, 0);	    
	    else neighbor[3] = 0.0;
	    
	    if (i > 0) 
	      neighbor[4] = in->GetComponent(dim0Tdim1*k + dim[0]*j + i-1, 0);
	    else neighbor[4] = 0.0;
	    if (i < dim[0]-1)
	      neighbor[5] = in->GetComponent(dim0Tdim1*k + dim[0]*j + i+1, 0);
	    else neighbor[5] = 0.0;
	    
	    int num_neighbors=0;
	    for(int r=0;r<6;r++) {
	      num_neighbors += ( neighbor[r]>0 );
	    }
	    
	    if (num_neighbors == 0 ) {
	      out->SetComponent(index, 0, 0.0);
	    } else if (this->Boundary2D == 0 && num_neighbors == 6) {
		out->SetComponent(index, 0, this->MaskOutValue);
	    } else if (this->Boundary2D == 1 && num_neighbors == 4) {
		out->SetComponent(index, 0, this->MaskOutValue);
	    } else {
	      out->SetComponent(index, 0, this->BoundaryOutValue);
	      this->BoundaryPoints != NULL ? this->BoundaryPoints->InsertNextTuple(coord) : 0;
	    }
	    /*switch(num_neighbors) 
	      {
	      case 0: out->SetComponent(index, 0, 0.0);
		break;
	      case 4:
	      case 6: out->SetComponent(index, 0, this->MaskOutValue);
		break;
	      default:
		out->SetComponent(index, 0, this->BoundaryOutValue);
		this->BoundaryPoints != NULL ? this->BoundaryPoints->InsertNextTuple(coord) : 0;
		};*/
	  }
	  
	  ++index;	  
	  
	  float pog=float(k)/float(dim[2]);
	  this->UpdateProgress(pog);	
	  
	}
  
  this->UpdateProgress(1.0);
  
}

// ----------------------------------------------------------------------------
