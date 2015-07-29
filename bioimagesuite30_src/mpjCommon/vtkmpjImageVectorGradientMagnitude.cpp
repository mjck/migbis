

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
#include "vtkmpjImageVectorGradientMagnitude.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjImageVectorGradientMagnitude* vtkmpjImageVectorGradientMagnitude::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageVectorGradientMagnitude");
  if(ret)
    {
      return (vtkmpjImageVectorGradientMagnitude*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageVectorGradientMagnitude;
}

// Construct object with no children.
vtkmpjImageVectorGradientMagnitude::vtkmpjImageVectorGradientMagnitude()
{
  this->Mask = NULL;
  this->Weight = NULL;
  this->MaskOutValue = 0;
  this->UsePrincipalComponents = 0;
}

// ----------------------------------------------------------------------------
vtkmpjImageVectorGradientMagnitude::~vtkmpjImageVectorGradientMagnitude()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
  if (this->Weight != NULL)
    this->Weight->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageVectorGradientMagnitude::ExecuteInformation()
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
    vtkErrorMacro(<< "At least a 3-component image is required!\n");
    return;
  }
  
  output->SetNumberOfScalarComponents(1);  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageVectorGradientMagnitude::ExecuteWithSumOfSquaredMagnitudes(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput() == NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkDataArray* in =input->GetPointData()->GetScalars();
  
  int dim[3];  input->GetDimensions(dim);
  float spc[3];  input->GetSpacing(spc);

  // stride in each dimension
  int factor[3] = {1, dim[0], dim[0]*dim[1]};

  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* mask =NULL;
  vtkDataArray *weight = NULL;
  
  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
  
  if (this->Weight != NULL)
    weight = this->Weight->GetPointData()->GetScalars();
  
  // Get number of components
  int nc = input->GetNumberOfScalarComponents();

  int index = 0;
  int prev, next;
  float sum, accum;

  float **J = new float *[3]; /* allocate space for jacobian */
  J[0] = new float[nc];
  J[1] = new float[nc];
  J[2] = new float[nc];

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  if ((mask != NULL)?(mask->GetComponent(index, 0) > 0):1)
	    {	      
	      int coord[3] = {i, j, k};

	      accum = 0.0;

	      // process each one of the six neighbors
	      for (int m=0; m<3; m++)  { // axis = { x, y, z }		
		
		  if ((coord[m] + 1) < dim[m])
		    next=index + factor[m];
		  else
		    next=index;
		  
		  if ((coord[m] - 1) >= 0)
		    prev=index - factor[m];
		  else
		    prev=index;
		  
		  sum = 0.0;
		  
		  // weights
		  float wnext = (weight != NULL)?(weight->GetComponent(next, 0)):1.0;
		  float wprev = (weight != NULL)?(weight->GetComponent(prev, 0)):1.0;
		  
		  // compute gradient for each channel
		  for(int p=0; p<nc; p++) {
		    J[m][p] = ((wnext*fabs(in->GetComponent(next, p))) - (wprev*fabs(in->GetComponent(prev, p)))) / (2 * 1);
		    sum += J[m][p] * J[m][p];
		  }
		  
		  accum += sum;
		  
	      } // process each axis
	      
	      out->SetComponent(index, 0, sqrt(accum));
	      
	    } else {
	      out->SetComponent(index, 0, this->MaskOutValue);
	    } // in mask
	  
	  ++index;
	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
  }
  
  this->UpdateProgress(1.0);
  
  // free jacobian
  delete[] J[0];
  delete[] J[1];
  delete[] J[2];
  delete[] J;
}

// ----------------------------------------------------------------------------
double vtkmpjImageVectorGradientMagnitude::CalculateDotProduct(float *a, float *b, int nc)
{
  float sum = 0.0;
  for (int i=0; i<nc; i++) {
    sum += a[i]*b[i];
  }
  return sum;
}

// ----------------------------------------------------------------------------
void vtkmpjImageVectorGradientMagnitude::ExecuteWithPrincipalComponents(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput() == NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkDataArray* in =input->GetPointData()->GetScalars();
  
  int dim[3];  input->GetDimensions(dim);
  float spc[3];  input->GetSpacing(spc);

  // stride in each dimension
  int factor[3] = {1, dim[0], dim[0]*dim[1]};

  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* mask =NULL;
  vtkDataArray *weight = NULL;
  
  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
  
  if (this->Weight != NULL)
    weight = this->Weight->GetPointData()->GetScalars();
  
  // Get number of components
  int nc = input->GetNumberOfScalarComponents();

  int index = 0;
  int prev, next;
  float sum, accum;

  printf("nc=%d\n",nc);

  float **J = new float *[3]; /* allocate space for jacobian */
  J[0] = new float[nc];
  J[1] = new float[nc];
  J[2] = new float[nc];

  float **S = new float *[3]; /* allocate space for S=J'*J */
  S[0] = new float[3];
  S[1] = new float[3];
  S[2] = new float[3];
  
  float **V = new float *[3]; /* allocate space for eigenvectors */
  V[0] = new float[3];
  V[1] = new float[3];
  V[2] = new float[3];
  
  float *W = new float[3]; /* allocate space for eigenvalues */

  
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  if ((mask != NULL)?(mask->GetComponent(index, 0) > 0):1)
	    {	      
	      int coord[3] = {i, j, k};
	      
	      // process each one of the dimensions
	      for (int m=0; m<3; m++)  { // axis = { x, y, z }		
		
		if ((coord[m] + 1) < dim[m])
		  next=index + factor[m];
		else
		  next=index;
		
		if ((coord[m] - 1) >= 0)
		  prev=index - factor[m];
		else
		  prev=index;
		
		// weights
		float wnext = (weight != NULL)?(weight->GetComponent(next, 0)):1.0;
		float wprev = (weight != NULL)?(weight->GetComponent(prev, 0)):1.0;
		
		// compute gradient for each channel
		for(int p=0; p<nc; p++) {
		  J[m][p] = ((wnext*fabs(in->GetComponent(next, p))) - (wprev*fabs(in->GetComponent(prev, p)))) / (2 * 1);
		}
		
	      } // process each axis
	      
	      // Calculate the symmetric metric tensor S=J'*J
	      for (int ii = 0; ii < 3; ii++)
		{
		  for (int jj = ii; jj < 3; jj++)
		    {
		      S[jj][ii] = S[ii][jj] = this->CalculateDotProduct(J[ii],J[jj],nc);
		    }
		}
	      
	      // Diagonalize
	      // Resulting eigenvalues/vectors are sorted in decreasing order; 
	      // eigenvectors are normalized.
	      (void) vtkMath::Jacobi(S, W, V);
	      
	      // write magnitude as sum of the eigenvalues
	      out->SetComponent(index, 0, W[0]+W[1]+W[2]);
	      
	    } else {
	      out->SetComponent(index, 0, this->MaskOutValue);
	    } // in mask
	  
	  ++index;
	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
  }
  
  this->UpdateProgress(1.0);
  
  // free jacobian
  delete[] J[0];
  delete[] J[1];
  delete[] J[2];
  delete[] J;

  // free gradient tensor
  delete[] S[0];
  delete[] S[1];
  delete[] S[2];
  delete[] S;

  // free eigenvectors
  delete[] V[0];
  delete[] V[1];
  delete[] V[2];
  delete[] V;

  // free eigenvalue memory
  delete[] W;

}

// ----------------------------------------------------------------------------
void vtkmpjImageVectorGradientMagnitude::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->UsePrincipalComponents) 
    this->ExecuteWithPrincipalComponents(input, output);
  else
    this->ExecuteWithSumOfSquaredMagnitudes(input, output);
}

// ----------------------------------------------------------------------------
