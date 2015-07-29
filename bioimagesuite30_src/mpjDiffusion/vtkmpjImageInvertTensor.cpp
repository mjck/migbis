

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
#include "vtkmpjImageTensorEigenAnalysis.h"
#include "vtkMath.h"
#include "math.h"

#define VTK_MPJ_SIGN(x) (( (x) < 0 )?( -1 ):( 1 ))

//------------------------------------------------------------------------------
vtkmpjImageTensorEigenAnalysis* vtkmpjImageTensorEigenAnalysis::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageTensorEigenAnalysis");
  if(ret)
    {
      return (vtkmpjImageTensorEigenAnalysis*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageTensorEigenAnalysis;
}

// Construct object with no children.
vtkmpjImageTensorEigenAnalysis::vtkmpjImageTensorEigenAnalysis()
{
  this->OutputType = VTK_MPJ_TENSOREIG_VALUES;
  this->MaskNegative = 0; 
  this->Mask = NULL;
  this->NegativeMask = NULL;
  this->MaskNegativeInValue = 1;
  this->MaskNegativeOutValue = 0;
  this->Ordering = VTK_MPJ_TENSOR_XXXYXZYYYZZZ;
  this->OutputAbsoluteEigenvalues = 1;
  this->SortByMagnitude = 0;
  this->FixZeroEigenvalues = 0;
}

// ----------------------------------------------------------------------------
vtkmpjImageTensorEigenAnalysis::~vtkmpjImageTensorEigenAnalysis()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
  if (this->NegativeMask != NULL)
    this->NegativeMask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorEigenAnalysis::ExecuteInformation()
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
  
  if ((nc != 6)&&(nc != 9)) {
    vtkErrorMacro(<<"A total of 6 (symmetric) or 9 tensor elements are required in the dataset!");
    return;
  }
  
  switch(this->OutputType) {
  case VTK_MPJ_TENSOREIG_VALUES:
    output->SetNumberOfScalarComponents(3);
    break;
  case VTK_MPJ_TENSOREIG_VECTORS:
    output->SetNumberOfScalarComponents(9);
    break;
  case VTK_MPJ_TENSOREIG_BOTH:
    output->SetNumberOfScalarComponents(12);
    break;
  };
  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorEigenAnalysis::SortEigenvectorsByMagnitude(float *w, float **v)
{
  int i, j, k;
  float tmp, tmp2;
  
  // sort eigenfunctions
  for (j=0; j<3; j++) 
    {
      k = j;
      tmp = w[k];
      tmp2 = fabs(tmp);
      for (i=j; i<3; i++)
	{
	  if (fabs(w[i]) >= tmp2) 
	    {
	      k = i;
	      tmp = w[k];
	      tmp2 = fabs(tmp);
	    }
	}
      if (k != j) 
	{
	  w[k] = w[j];
	  w[j] = tmp;
	  for (i=0; i<3; i++) 
	    {
	      tmp = v[i][j];
	      v[i][j] = v[i][k];
	      v[i][k] = tmp;
	    }
	}
    }
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorEigenAnalysis::ExecuteWithReducedTensor(vtkImageData *input, vtkImageData *output)
{
  float **t = new float *[3]; /* allocate space for tensor matrix */
  t[0] = new float[3];
  t[1] = new float[3];
  t[2] = new float[3];

  float **v = new float *[3]; /* allocate space for eigenvector matrix */
  v[0] = new float[3];
  v[1] = new float[3];
  v[2] = new float[3];

  float *w = new float[3];    /* allocate space for eigenvalues */

  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  printf("AbsoluteEigenvalues is %d\n",this->OutputAbsoluteEigenvalues);
  
  // Get number of tensor elements
  int nc = input->GetNumberOfScalarComponents();
  int ncout = output->GetNumberOfScalarComponents();

  vtkDataArray* in =input->GetPointData()->GetScalars();
  
  int dim[3];  input->GetDimensions(dim);

  vtkDataArray* out=output->GetPointData()->GetScalars();

  vtkDataArray* mask=NULL;
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  vtkDataArray* negmask=NULL;

  if (this->MaskNegative) {
    if (this->NegativeMask == NULL)
      this->NegativeMask=vtkImageData::New();
    this->NegativeMask->SetSpacing(input->GetSpacing());
    this->NegativeMask->SetOrigin(input->GetOrigin());
    this->NegativeMask->SetScalarTypeToUnsignedChar();//(input->GetScalarType());
    this->NegativeMask->SetNumberOfScalarComponents(1);
    this->NegativeMask->SetDimensions(dim[0],dim[1],dim[2]);
    this->NegativeMask->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
    this->NegativeMask->AllocateScalars();
    negmask=this->NegativeMask->GetPointData()->GetScalars();
  }

  int inmask;
  int index=0;

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;
	  
	  if (inmask) {
	    
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
	    
	    // determine its eigenvectors and eigenvalues	      
	    (void) vtkMath::Jacobi(t,w,v);
	    	   
	    int ccount = -1;
	    
	    // create mask for negative eigenvalues
	    if (this->MaskNegative) { 
	      if (w[0]<0 || w[1]<0 || w[2]<0)
		negmask->SetComponent(index, 0, MaskNegativeInValue);
	      else negmask->SetComponent(index, 0, MaskNegativeOutValue);
	    }

	    // this is for use in hessian matrices only
	    if (this->FixZeroEigenvalues) {
	      if (fabs(w[0]) < 1E-4) w[0] = VTK_MPJ_SIGN(w[0]);
	      if (fabs(w[1]) < 1E-4) w[1] = VTK_MPJ_SIGN(w[1]);
	      if (fabs(w[2]) < 1E-4) w[2] = VTK_MPJ_SIGN(w[2]);
	    }
	      
	    // re-sort eigenvectors instead by their absolute value	    
	    if (this->SortByMagnitude)
	      this->SortEigenvectorsByMagnitude(w,v);	    
	    
	    if (this->OutputAbsoluteEigenvalues) {
	      for (int i=0; i<3; i++) {
		if (w[i] < 0) {
		  v[0][i] = -v[0][i];
		  v[1][i] = -v[1][i];
		  v[2][i] = -v[2][i];
		  w[i] = -w[i];
		}
	      }
	    }
	    
	    // save eigenvalues
	    if (this->OutputType == VTK_MPJ_TENSOREIG_VALUES || 
		this->OutputType == VTK_MPJ_TENSOREIG_BOTH) {
	      
	      out->SetComponent(index, ++ccount, w[0]); /* 0 */
	      out->SetComponent(index, ++ccount, w[1]); /* 1 */
	      out->SetComponent(index, ++ccount, w[2]); /* 2 */
	      
	    }
	    
	    // save eigenvectors
	    if (this->OutputType == VTK_MPJ_TENSOREIG_VECTORS || 
		this->OutputType == VTK_MPJ_TENSOREIG_BOTH) {
	      
	      // save 1st eigenvector
	      out->SetComponent(index, ++ccount, v[0][0]*w[0]); /* 0 */
	      out->SetComponent(index, ++ccount, v[1][0]*w[0]); /* 0 */
	      out->SetComponent(index, ++ccount, v[2][0]*w[0]); /* 0 */
	      
	      // save 2nd eigenvector
	      out->SetComponent(index, ++ccount, v[0][1]*w[1]); /* 1 */
	      out->SetComponent(index, ++ccount, v[1][1]*w[1]); /* 1 */
	      out->SetComponent(index, ++ccount, v[2][1]*w[1]); /* 1 */
	      
	      // save 3rd eigenvector
	      out->SetComponent(index, ++ccount, v[0][2]*w[2]); /* 2 */
	      out->SetComponent(index, ++ccount, v[1][2]*w[2]); /* 2 */
	      out->SetComponent(index, ++ccount, v[2][2]*w[2]); /* 2 */	  
	      
	    }
	    
	  } else {
	    
	    // zero out points not in the mask
	    if (this->MaskNegative) { 
	      negmask->SetComponent(index, 0, MaskNegativeOutValue);
	    } 
	    
	    for(int l=0; l<ncout; l++) {
	      out->SetComponent(index, l, 0);
	    }
	    
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
  
  // free eigenvectors memory
  delete[] v[0];
  delete[] v[1];
  delete[] v[2];
  delete[] v;

  // free eigenvalue memory
  delete[] w;
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorEigenAnalysis::ExecuteWithFullTensor(vtkImageData *input, vtkImageData *output)
{
  float **t = new float *[3]; /* allocate space for tensor matrix */
  t[0] = new float[3];
  t[1] = new float[3];
  t[2] = new float[3];

  float **v = new float *[3]; /* allocate space for eigenvector matrix */
  v[0] = new float[3];
  v[1] = new float[3];
  v[2] = new float[3];

  float *w = new float[3];    /* allocate space for eigenvalues */

  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }

  // Get number of tensor elements
  int nc = input->GetNumberOfScalarComponents();
  int ncout = output->GetNumberOfScalarComponents();

  vtkDataArray* in =input->GetPointData()->GetScalars();
  
  int dim[3];  input->GetDimensions(dim);

  vtkDataArray* out=output->GetPointData()->GetScalars();

  vtkDataArray* mask=NULL;
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  vtkDataArray* negmask=NULL;

  if (this->MaskNegative) {
    if (this->NegativeMask == NULL)
      this->NegativeMask=vtkImageData::New();
    this->NegativeMask->SetSpacing(input->GetSpacing());
    this->NegativeMask->SetOrigin(input->GetOrigin());
    this->NegativeMask->SetScalarTypeToUnsignedChar();//(input->GetScalarType());
    this->NegativeMask->SetNumberOfScalarComponents(1);
    this->NegativeMask->SetDimensions(dim[0],dim[1],dim[2]);
    this->NegativeMask->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
    this->NegativeMask->AllocateScalars();
    negmask=this->NegativeMask->GetPointData()->GetScalars();
  }

  int inmask;
  int index=0;

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;
	  
	  if (inmask) {

	    t[0][0] = in->GetComponent(index, 0); /* Dxx */
	    t[1][0] = in->GetComponent(index, 1); /* Dxy */
	    t[2][0] = in->GetComponent(index, 2); /* Dxz */
	    t[0][1] = in->GetComponent(index, 3); /* Dyx */
	    t[1][1] = in->GetComponent(index, 4); /* Dyy */
	    t[2][1] = in->GetComponent(index, 5); /* Dyz */	    
	    t[0][2] = in->GetComponent(index, 6); /* Dzx */
	    t[1][2] = in->GetComponent(index, 7); /* Dzy */
	    t[2][2] = in->GetComponent(index, 8); /* Dzz */
	    
	    // determine its eigenvectors and eigenvalues	      
	    (void) vtkMath::Jacobi(t,w,v);
	    
	    int ccount = -1;
	    
	    // create mask for negative eigenvalues
	    if (this->MaskNegative) { 
	      if (w[0]<0 || w[1]<0 || w[2]<0)
		negmask->SetComponent(index, 0, MaskNegativeInValue);
	      else negmask->SetComponent(index, 0, MaskNegativeOutValue);
	    }

	    // re-sort eigenvectors instead by their absolute value	    
	    if (this->SortByMagnitude)
	      this->SortEigenvectorsByMagnitude(w,v);	    
	    
	    if (this->OutputAbsoluteEigenvalues) {
	      for (int i=0; i<3; i++) {
		if (w[i] < 0) {
		  v[0][i] = -v[0][i];
		  v[1][i] = -v[1][i];
		  v[2][i] = -v[2][i];
		  w[i] = -w[i];
		}
	      }
	    }
	    
	    // save eigenvalues
	    if (this->OutputType == VTK_MPJ_TENSOREIG_VALUES || 
		this->OutputType == VTK_MPJ_TENSOREIG_BOTH) {
	      
	      out->SetComponent(index, ++ccount, w[0]); /* 0 */
	      out->SetComponent(index, ++ccount, w[1]); /* 1 */
	      out->SetComponent(index, ++ccount, w[2]); /* 2 */
	      
	    }
	    
	    // save eigenvectors
	    if (this->OutputType == VTK_MPJ_TENSOREIG_VECTORS || 
		this->OutputType == VTK_MPJ_TENSOREIG_BOTH) {
	      
	      // save 1st eigenvector
	      out->SetComponent(index, ++ccount, v[0][0]*w[0]); /* 0 */
	      out->SetComponent(index, ++ccount, v[1][0]*w[0]); /* 0 */
	      out->SetComponent(index, ++ccount, v[2][0]*w[0]); /* 0 */
	      
	      // save 2nd eigenvector
	      out->SetComponent(index, ++ccount, v[0][1]*w[1]); /* 1 */
	      out->SetComponent(index, ++ccount, v[1][1]*w[1]); /* 1 */
	      out->SetComponent(index, ++ccount, v[2][1]*w[1]); /* 1 */
	      
	      // save 3rd eigenvector
	      out->SetComponent(index, ++ccount, v[0][2]*w[2]); /* 2 */
	      out->SetComponent(index, ++ccount, v[1][2]*w[2]); /* 2 */
	      out->SetComponent(index, ++ccount, v[2][2]*w[2]); /* 2 */	  
	      
	    }
	    
	  } else {
	    
	    // zero out points not in the mask
	    if (this->MaskNegative) { 
	      negmask->SetComponent(index, 0, MaskNegativeOutValue);
	    } 
	    
	    for(int l=0; l<ncout; l++) {
	      out->SetComponent(index, l, 0);
	    }
	    
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
  
  // free eigenvectors memory
  delete[] v[0];
  delete[] v[1];
  delete[] v[2];
  delete[] v;

  // free eigenvalue memory
  delete[] w;
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorEigenAnalysis::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  
  // Get number of tensor elements
  int nc = input->GetNumberOfScalarComponents();
  
  if (nc > 6) 
    this->ExecuteWithFullTensor(input, output);
  else
    this->ExecuteWithReducedTensor(input, output);
}

// ----------------------------------------------------------------------------
