

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
#include "vtkmpjImageInvertTensor.h"
#include "vtkMath.h"
#include "math.h"

#define VTK_MPJ_SIGN(x) (( (x) < 0 )?( -1 ):( 1 ))

//------------------------------------------------------------------------------
vtkmpjImageInvertTensor* vtkmpjImageInvertTensor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageInvertTensor");
  if(ret)
    {
      return (vtkmpjImageInvertTensor*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageInvertTensor;
}

// Construct object with no children.
vtkmpjImageInvertTensor::vtkmpjImageInvertTensor()
{
  this->Mask = NULL;
  this->LargeValue = 1E6;
}

// ----------------------------------------------------------------------------
vtkmpjImageInvertTensor::~vtkmpjImageInvertTensor()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageInvertTensor::ExecuteInformation()
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
  
  output->SetNumberOfScalarComponents(6);
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageInvertTensor::SortEigenvectors(float w[3], float V[3][3])
{
  int i, j, k;
  float tmp, tmp2;
  
  // sort eigenfunctions by decreasing magnitude
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
              tmp = V[i][j];
              V[i][j] = V[i][k];
              V[i][k] = tmp;
            }
        }
    }
}

// ----------------------------------------------------------------------------
void vtkmpjImageInvertTensor::InvertReducedTensor(vtkImageData *input, vtkImageData *output)
{
  float T[3][3];
  float V[3][3];
  float w[3];

  float W[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
  float Vt[3][3], WVt[3][3], U[3][3];

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
  
  int inmask;
  int index=0;
  int nzeros=0;

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;
	  
	  if (inmask) {
	    
	    // set tensor image
	    T[0][0] = in->GetComponent(index, 0);           /* Dxx */
	    T[1][0] = T[0][1] = in->GetComponent(index, 1); /* Dxy */
	    T[2][0] = T[0][2] = in->GetComponent(index, 2); /* Dxz */	      
	    T[1][1] = in->GetComponent(index, 3);           /* Dyy */
	    T[2][1] = T[1][2] = in->GetComponent(index, 4); /* Dyz */	      
	    T[2][2] = in->GetComponent(index, 5);           /* Dzz */
	    
	    // determine its eigenvectors and eigenvalues	      
	    (void) vtkMath::Diagonalize3x3(T,w,V);
	   
	    this->SortEigenvectors(w,V);

	    // count zero eigenvalues
	    nzeros = 0;
	    nzeros += (fabs(w[0]) < 1e-12);
	    nzeros += (fabs(w[1]) < 1e-12);
	    nzeros += (fabs(w[2]) < 1e-12);
		       
	    if (nzeros < 2) {

	      // determine inverse = V*W*V', W = 1/w;
	      W[0][0] = 1.0 / w[0];  
	      W[1][1] = 1.0 / w[1];
	      W[2][2] = 1.0 / w[2]; 

	      if (!finite(W[2][2]))
		W[2][2] = this->LargeValue;
	      
	      printf("W=%f, %f, %f:%f %f %f\n",w[0],w[1],w[2],W[0][0],W[1][1],W[2][2]);
	      
	      // build inverse tensor
	      (void) vtkMath::Transpose3x3(V,Vt);
	      (void) vtkMath::Multiply3x3(W,Vt,WVt);
	      (void) vtkMath::Multiply3x3(V,WVt,U);

	    } else {

	      U[0][0] = T[0][0]; U[0][1]=U[1][0]=T[0][1]; U[0][2]=U[2][0]=T[0][2];
	      U[1][1] = T[1][1]; U[1][2]=U[2][1]=T[1][2];
	      U[2][2] = T[2][2];
	    }
	      
	    // write out inverse
	    out->SetComponent(index, 0, U[0][0]);       /* Dxx */
	    out->SetComponent(index, 1, U[0][1]);       /* Dxy */
	    out->SetComponent(index, 2, U[0][2]);       /* Dxz */	 
	    out->SetComponent(index, 3, U[1][1]);       /* Dyy */
	    out->SetComponent(index, 4, U[1][2]);       /* Dyz */
	    out->SetComponent(index, 5, U[2][2]);       /* Dzz */
	    
	  } else {
	    
	    // zero out points not in the mask
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
  //delete[] T[0];
  //delete[] T[1];
  //delete[] T[2];
  //delete[] T;
  
  // free eigenvectors memory
  //delete[] V[0];
  //delete[] V[1];
  //delete[] V[2];
  //delete[] V;

  // free eigenvalue memory
  //delete[] w;
}

// ----------------------------------------------------------------------------
void vtkmpjImageInvertTensor::InvertFullTensor(vtkImageData *input, vtkImageData *output)
{
  float T[3][3];
  float V[3][3];
  float w[3];

  float W[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
  float Vt[3][3], WVt[3][3], U[3][3];
  
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
  
  int inmask;
  int index=0;
  int nzeros=0;

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;
	  
	  if (inmask) {

	    T[0][0] = in->GetComponent(index, 0); /* Dxx */
	    T[1][0] = in->GetComponent(index, 1); /* Dxy */
	    T[2][0] = in->GetComponent(index, 2); /* Dxz */
	    T[0][1] = in->GetComponent(index, 3); /* Dyx */
	    T[1][1] = in->GetComponent(index, 4); /* Dyy */
	    T[2][1] = in->GetComponent(index, 5); /* Dyz */	    
	    T[0][2] = in->GetComponent(index, 6); /* Dzx */
	    T[1][2] = in->GetComponent(index, 7); /* Dzy */
	    T[2][2] = in->GetComponent(index, 8); /* Dzz */
	    
	    // determine its eigenvectors and eigenvalues	      
	    (void) vtkMath::Diagonalize3x3(T,w,V);
	    	       
	    this->SortEigenvectors(w,V);

	    // count zero eigenvalues
	    nzeros = 0;
	    nzeros += (fabs(w[0]) < 1e-12);
	    nzeros += (fabs(w[1]) < 1e-12);
	    nzeros += (fabs(w[2]) < 1e-12);	    
	    
	    if (nzeros < 2) {
	      
	      // determine inverse = V*W*V', W = 1/w;
	      W[0][0] = 1.0 / w[0];
	      W[1][1] = 1.0 / w[1];
	      W[2][2] = 1.0 / w[2];
	      	      
	      if (!finite(W[2][2]))
		W[2][2] = this->LargeValue;
	      
	      // build inverse tensor
	      (void) vtkMath::Transpose3x3(V,Vt);
	      (void) vtkMath::Multiply3x3(W,Vt,WVt);
	      (void) vtkMath::Multiply3x3(V,WVt,U);

	    } else {

	      U[0][0] = T[0][0]; U[0][1]=U[1][0]=T[0][1]; U[0][2]=U[2][0]=T[0][2];
	      U[1][1] = T[1][1]; U[1][2]=U[2][1]=T[1][2];
	      U[2][2] = T[2][2];
	    }
	      
	    // write out inverse
	    out->SetComponent(index, 0, U[0][0]);       /* Dxx */
	    out->SetComponent(index, 1, U[1][0]);       /* Dxy */
	    out->SetComponent(index, 2, U[2][0]);       /* Dxz */	 
	    out->SetComponent(index, 3, U[0][1]);       /* Dyx */
	    out->SetComponent(index, 4, U[1][1]);       /* Dyy */
	    out->SetComponent(index, 5, U[2][1]);       /* Dyz */
	    out->SetComponent(index, 6, U[0][2]);       /* Dzx */
	    out->SetComponent(index, 7, U[1][2]);       /* Dzy */
	    out->SetComponent(index, 8, U[2][2]);       /* Dzz */	 

	  } else {
	    
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
}

// ----------------------------------------------------------------------------
void vtkmpjImageInvertTensor::SimpleExecute(vtkImageData *input, vtkImageData *output)
{  
  // Get number of tensor elements
  int nc = input->GetNumberOfScalarComponents();
  
  if (nc > 6) 
    this->InvertFullTensor(input, output);
  else
    this->InvertReducedTensor(input, output);
}

// ----------------------------------------------------------------------------
