

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
#include "vtkmpjImageTensorSimilarity.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjImageTensorSimilarity* vtkmpjImageTensorSimilarity::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageTensorSimilarity");
  if(ret)
    {
      return (vtkmpjImageTensorSimilarity*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageTensorSimilarity;
}

// Construct object with no children.
vtkmpjImageTensorSimilarity::vtkmpjImageTensorSimilarity()
{
  this->KernelSize[0] = 3;
  this->KernelSize[1] = 3;
  this->KernelSize[2] = 3;
  this->Mask = NULL;
  this->MaskOutValue = 0;
  this->Ordering = VTK_MPJ_TENSOR_XXXYXZYYYZZZ;   
}

// ----------------------------------------------------------------------------
vtkmpjImageTensorSimilarity::~vtkmpjImageTensorSimilarity()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorSimilarity::ExecuteInformation()
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
  
  if (nc < 9) {
    if (nc < 6) {
      vtkErrorMacro(<< "A tensor image is required as input!\n");
      return;
    }
  }
  
  output->SetNumberOfScalarComponents(1);  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorSimilarity::TensorDifference(float T[3][3], float U[3][3], float R[3][3])
{
  for(int i=0;i<3;i++)
    for(int j=0;j<3;j++)
      R[i][j]=T[i][j]-U[i][j];
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorSimilarity::GetReducedTensor(vtkDataArray *in, int index, float T[3][3]) 
{
  // set tensor matrix
  if (this->Ordering == VTK_MPJ_TENSOR_XXXYXZYYYZZZ) {
    
    T[0][0] = in->GetComponent(index, 0);           /* Dxx */
    T[1][0] = T[0][1] = in->GetComponent(index, 1); /* Dxy */
    T[2][0] = T[0][2] = in->GetComponent(index, 2); /* Dxz */
    T[1][1] = in->GetComponent(index, 3);           /* Dyy */
    T[2][1] = T[1][2] = in->GetComponent(index, 4); /* Dyz */
    T[2][2] = in->GetComponent(index, 5);           /* Dzz */
    
  } else {
    
    T[0][0] = in->GetComponent(index, 0);           /* Dxx */
    T[1][0] = T[0][1] = in->GetComponent(index, 3); /* Dxy */
    T[2][0] = T[0][2] = in->GetComponent(index, 5); /* Dxz */
    T[1][1] = in->GetComponent(index, 1);           /* Dyy */
    T[2][1] = T[1][2] = in->GetComponent(index, 4); /* Dyz */
    T[2][2] = in->GetComponent(index, 2);           /* Dzz */
    
  }  
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorSimilarity::GetFullTensor(vtkDataArray *in, int index, float T[3][3])
{
  T[0][0] = in->GetComponent(index, 0); /* Dxx */
  T[1][0] = in->GetComponent(index, 1); /* Dxy */
  T[2][0] = in->GetComponent(index, 2); /* Dxz */
  T[0][1] = in->GetComponent(index, 3); /* Dyx */
  T[1][1] = in->GetComponent(index, 4); /* Dyy */
  T[2][1] = in->GetComponent(index, 5); /* Dyz */
  T[0][2] = in->GetComponent(index, 6); /* Dzx */
  T[1][2] = in->GetComponent(index, 7); /* Dzy */
  T[2][2] = in->GetComponent(index, 8); /* Dzz */
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorSimilarity::ExecuteWithFullTensor(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkDataArray* in =input->GetPointData()->GetScalars();
 
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* mask =NULL;
  
  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
  
  int mink = this->KernelSize[2] / 2;
  int minj = this->KernelSize[1] / 2;
  int mini = this->KernelSize[0] / 2;
  int total_ij = dim[0]*dim[1];
  int index = 0;
  float T[3][3], U[3][3], R[3][3];
  float dist;

  /*  printf("mini=%d,minj=%d,mink=%d\n",mini,minj,mink);*/
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  if ((mask != NULL)?(mask->GetComponent(index, 0) > 0):1)
	    {
	      this->GetFullTensor(in, index, T);

	      int count = 0;
	      float sum=0;

	      /* calculate neghborhood coherence */
	      for (int nk=-mink; nk<=mink; nk++) {
		int total_k = (k + nk) * total_ij;

		for (int nj=-minj; nj<=minj; nj++) {
		  int total_jk = total_k + (j + nj)*dim[0];

		  for (int ni=-mini; ni<=mini; ni++) {

		    /* skip center voxel */
		    if (nk==0 && nj==0 && ni==0) continue;
		    
		    /* determine neighbor's address in image array */
		    int nindex = total_jk + (i + ni);
		    
		    /* handle boundaries */
		    if ((k+nk)>=0 && (j+nj)>=0 && (i+ni)>=0 &&
			(k+nk)<dim[2] && (j+nj)<dim[1] && (i+ni)<dim[0] ) {
		      
		      /* if neighbor is in mask */
		      if ((mask != NULL)?(mask->GetComponent(nindex, 0) > 0):1) {
			
			// get neighboring tensor
			this->GetReducedTensor(in, nindex, U);
			
			// compute tensor difference
			this->TensorDifference(T, U, R);
			
			dist = R[0][0]*R[0][0] + R[1][1]*R[1][1] + R[2][2]*R[2][2];
			
			sum = sum + sqrt(dist);

			++count;
		      }
		    }
		  }
		}
	      }
	      
	      if ( count > 0 ) {
		sum = sum / (float)count; 
		out->SetComponent(index, 0, sum == 0.0 ? 100.0 : 1.0 / sum);
	      } else {
		out->SetComponent(index, 0, this->MaskOutValue);
	      }
	      
	    } else {
	      out->SetComponent(index, 0, this->MaskOutValue);
	    }
	  
	  ++index;
	 
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
  }
  
  this->UpdateProgress(1.0);

}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorSimilarity::ExecuteWithReducedTensor(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkDataArray* in =input->GetPointData()->GetScalars();
 
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* mask =NULL;
  
  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
  
  int mink = this->KernelSize[2] / 2;
  int minj = this->KernelSize[1] / 2;
  int mini = this->KernelSize[0] / 2;
  int total_ij = dim[0]*dim[1];
  int index = 0;
  float T[3][3], U[3][3], R[3][3];
  float dist;

  /*  printf("mini=%d,minj=%d,mink=%d\n",mini,minj,mink);*/
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  if ((mask != NULL)?(mask->GetComponent(index, 0) > 0):1)
	    {
	      this->GetReducedTensor(in, index, T);

	      int count = 0;
	      float sum=0;

	      /* calculate neghborhood coherence */
	      for (int nk=-mink; nk<=mink; nk++) {
		int total_k = (k + nk) * total_ij;

		for (int nj=-minj; nj<=minj; nj++) {
		  int total_jk = total_k + (j + nj)*dim[0];

		  for (int ni=-mini; ni<=mini; ni++) {

		    /* skip center voxel */
		    if (nk==0 && nj==0 && ni==0) continue;
		    
		    /* determine neighbor's address in image array */
		    int nindex = total_jk + (i + ni);
		    
		    /* handle boundaries */
		    if ((k+nk)>=0 && (j+nj)>=0 && (i+ni)>=0 &&
			(k+nk)<dim[2] && (j+nj)<dim[1] && (i+ni)<dim[0] ) {
		      
		      /* if neighbor is in mask */
		      if ((mask != NULL)?(mask->GetComponent(nindex, 0) > 0):1) {
			
			// get neighboring tensor
			this->GetReducedTensor(in, nindex, U);
			
			// compute tensor difference
			this->TensorDifference(T, U, R);
			
			dist = R[0][0]*R[0][0] + R[1][1]*R[1][1] + R[2][2]*R[2][2];
			
			sum = sum + sqrt(dist);
			
			++count;
		      }
		    }
		  }
		}
	      }
	      
	      if ( count > 0 ) {
		sum = sum / (float)count; 
		out->SetComponent(index, 0, sum == 0.0 ? 100.0 : 1.0 / sum);
	      } else {
		out->SetComponent(index, 0, this->MaskOutValue);
	      }
	      
	    } else {
	      out->SetComponent(index, 0, this->MaskOutValue);
	    }
	  
	  ++index;
	 
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
  }
  
  this->UpdateProgress(1.0);

}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorSimilarity::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
   
  // Get number of tensor elements
  int nc = input->GetNumberOfScalarComponents();
  
  if (nc > 6)
    this->ExecuteWithFullTensor(input, output);
  else
    this->ExecuteWithReducedTensor(input, output);
}

// ----------------------------------------------------------------------------
