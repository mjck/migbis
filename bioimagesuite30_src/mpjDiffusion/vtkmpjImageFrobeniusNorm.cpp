

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
#include "vtkmpjImageFrobeniusNorm.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjImageFrobeniusNorm* vtkmpjImageFrobeniusNorm::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageFrobeniusNorm");
  if(ret)
    {
      return (vtkmpjImageFrobeniusNorm*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageFrobeniusNorm;
}

// Construct object with no children.
vtkmpjImageFrobeniusNorm::vtkmpjImageFrobeniusNorm()
{
  this->Ordering = VTK_MPJ_TENSOR_XXXYXZYYYZZZ;   
  this->Mask = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjImageFrobeniusNorm::~vtkmpjImageFrobeniusNorm()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageFrobeniusNorm::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  int dim[3];  input->GetDimensions(dim);
  
  output->SetNumberOfScalarComponents(1);
  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageFrobeniusNorm::ExecuteWithReducedTensor(vtkImageData *input, vtkImageData *output)
{
  float **t = new float *[3]; /* allocate space for tensor matrix */
  t[0] = new float[3];
  t[1] = new float[3];
  t[2] = new float[3];
 
  int nc = input->GetNumberOfScalarComponents();
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* in=input->GetPointData()->GetScalars(); 
  vtkDataArray* out=output->GetPointData()->GetScalars();  
  
  vtkDataArray* mask=NULL;
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  float v1[3], v2[3], sum;
  int index=0;
  int inmask;
  
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;
	  
	  if (inmask) {
	    
	    sum = 0.0;

	    // set tensor matrix
            if (this->Ordering == VTK_MPJ_TENSOR_XXXYXZYYYZZZ) {
	      
              t[0][0] = in->GetComponent(index, 0);           /* Dxx */
              t[1][0] = t[0][1] = in->GetComponent(index, 1); /* Dxy */
              t[2][0] = t[0][2] = in->GetComponent(index, 2); /* Dxz */
              t[1][1] = in->GetComponent(index, 3);           /* Dyy */
              t[2][1] = t[1][2] = in->GetComponent(index, 4); /* Dyz */
              t[2][2] = in->GetComponent(index, 5);           /* Dzz */
	      
            } else {
	      
              t[0][0] = in->GetComponent(index, 0);           /* Dxx */
              t[1][0] = t[0][1] = in->GetComponent(index, 3); /* Dxy */
              t[2][0] = t[0][2] = in->GetComponent(index, 5); /* Dxz */
              t[1][1] = in->GetComponent(index, 1);           /* Dyy */
              t[2][1] = t[1][2] = in->GetComponent(index, 4); /* Dyz */
              t[2][2] = in->GetComponent(index, 2);           /* Dzz */
	      
            }
	    
	    sum = t[0][0]*t[0][0] + 2*t[1][0]*t[1][0] + 2*t[2][0]*t[2][0] +
	      t[1][1]*t[1][1] + 2*t[2][1]*t[2][1] + t[2][2]*t[2][2];
	    
	    out->SetComponent(index, 0, (float)sqrt(sum));
	    
	  } else {
	    out->SetComponent(index, 0, 0.0);
	  }
	  
	  ++index;	   	 
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);
  
  // free tensor memory
  delete[] t[0];
  delete[] t[1];
  delete[] t[2];
  delete[] t;
}

// ----------------------------------------------------------------------------
void vtkmpjImageFrobeniusNorm::ExecuteWithFullTensor(vtkImageData *input, vtkImageData *output)
{
  float **t = new float *[3]; /* allocate space for tensor matrix */
  t[0] = new float[3];
  t[1] = new float[3];
  t[2] = new float[3];
 
  int nc = input->GetNumberOfScalarComponents();
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* in=input->GetPointData()->GetScalars(); 
  vtkDataArray* out=output->GetPointData()->GetScalars();  
  
  vtkDataArray* mask=NULL;
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  float v1[3], v2[3], sum;
  int index=0;
  int inmask;
  
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;
	  
	  if (inmask) {
	    
	    sum = 0.0;

	    t[0][0] = in->GetComponent(index, 0); /* Dxx */
            t[1][0] = in->GetComponent(index, 1); /* Dxy */
            t[2][0] = in->GetComponent(index, 2); /* Dxz */
            t[0][1] = in->GetComponent(index, 3); /* Dyx */
            t[1][1] = in->GetComponent(index, 4); /* Dyy */
            t[2][1] = in->GetComponent(index, 5); /* Dyz */
            t[0][2] = in->GetComponent(index, 6); /* Dzx */
            t[1][2] = in->GetComponent(index, 7); /* Dzy */
            t[2][2] = in->GetComponent(index, 8); /* Dzz */

	    sum = t[0][0]*t[0][0] + t[1][0]*t[1][0] + t[2][0]*t[2][0] +
	      t[0][1]*t[0][1] + t[1][1]*t[1][1] + t[2][1]*t[2][1] + 
	      t[0][2]*t[0][2] + t[1][2]*t[1][2] + t[2][2]*t[2][2];
	    
	    out->SetComponent(index, 0, (float)sqrt(sum));
	    
	  } else {
	    out->SetComponent(index, 0, 0.0);
	  }
	  
	  ++index;	   	 
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);
  
  // free tensor memory
  delete[] t[0];
  delete[] t[1];
  delete[] t[2];
  delete[] t;
}


// ----------------------------------------------------------------------------
void vtkmpjImageFrobeniusNorm::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
   
  // Get number of tensor elements
  int nc = input->GetNumberOfScalarComponents();
  
  if (nc > 6)
    this->ExecuteWithFullTensor(input, output);
  else
    this->ExecuteWithReducedTensor(input, output);
}


// ----------------------------------------------------------------------------
