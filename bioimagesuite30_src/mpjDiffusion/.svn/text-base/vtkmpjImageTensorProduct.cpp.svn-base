

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
#include "vtkmpjImageTensorProduct.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjImageTensorProduct* vtkmpjImageTensorProduct::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageTensorProduct");
  if(ret)
    {
      return (vtkmpjImageTensorProduct*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageTensorProduct;
}

// Construct object with no children.
vtkmpjImageTensorProduct::vtkmpjImageTensorProduct()
{
  this->KernelSize[0] = 3;
  this->KernelSize[1] = 3;
  this->KernelSize[2] = 3;
  this->Mask = NULL;
  this->Eigenvalues = NULL;
  this->MaskOutValue = 0;
}

// ----------------------------------------------------------------------------
vtkmpjImageTensorProduct::~vtkmpjImageTensorProduct()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
  if (this->Eigenvalues != NULL)
    this->Eigenvalues->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorProduct::ExecuteInformation()
{
  vtkImageData *eigvec=this->GetEigenvectors();
  vtkImageData *eigval=this->GetEigenvalues();
  vtkImageData *output=this->GetOutput();
  
  if (eigvec==NULL || eigval==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  int dim[3];  eigvec->GetDimensions(dim);
  int ncv=eigvec->GetNumberOfScalarComponents();
  int ncd=eigval->GetNumberOfScalarComponents();
  
  if (ncv < 9 || ncd < 3) {
    vtkErrorMacro(<< "Three eigenvectors and three eigenvalues are required as input!");
    return;
  }

  output->SetNumberOfScalarComponents(1);  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorProduct::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  vtkImageData *eigvec=input;
  vtkImageData *eigval=this->GetEigenvalues();
  vtkDataArray* inv =eigvec->GetPointData()->GetScalars();
  vtkDataArray *ind =eigval->GetPointData()->GetScalars();
 
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

  /*  printf("mini=%d,minj=%d,mink=%d\n",mini,minj,mink);*/
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  if ((mask != NULL)?(mask->GetComponent(index, 0) > 0):1)
	    {
	      float v[3][3];	      
	      v[0][0] = inv->GetComponent(index, 0);
	      v[1][0] = inv->GetComponent(index, 1);
	      v[2][0] = inv->GetComponent(index, 2);		      	      
	      v[0][1] = inv->GetComponent(index, 3);
	      v[1][1] = inv->GetComponent(index, 4);
	      v[2][1] = inv->GetComponent(index, 5);	      
	      v[0][2] = inv->GetComponent(index, 6);
	      v[1][2] = inv->GetComponent(index, 7);
	      v[2][2] = inv->GetComponent(index, 8);
	      
	      float d[3];
	      d[0]=ind->GetComponent(index, 0);
	      d[1]=ind->GetComponent(index, 1);
	      d[2]=ind->GetComponent(index, 2);

	      float md=(d[0]+d[1]+d[2]) / 3;

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
					
			float w[3][3];
			
			w[0][0] = inv->GetComponent(nindex, 0);
			w[1][0] = inv->GetComponent(nindex, 1);
			w[2][0] = inv->GetComponent(nindex, 2);		      
			
			w[0][1] = inv->GetComponent(nindex, 3);
			w[1][1] = inv->GetComponent(nindex, 4);
			w[2][1] = inv->GetComponent(nindex, 5);

			w[0][2] = inv->GetComponent(nindex, 6);
			w[1][2] = inv->GetComponent(nindex, 7);
			w[2][2] = inv->GetComponent(nindex, 8);
			
			float u[3];
			u[0] = ind->GetComponent(nindex, 0);
			u[1] = ind->GetComponent(nindex, 1);
			u[2] = ind->GetComponent(nindex, 2);
			
			float tp=0.0;	
			float dp=0.0;
			float mu=(u[0]+u[1]+u[2]) / 3;
			
			for(int s=0; s<3; s++)
			  for(int t=0; t<3; t++) {
			    dp = v[0][s]*w[0][t] + v[1][s]*w[1][t] + v[2][s]*w[2][t];
			    tp = tp + ((d[s]*u[t]*dp*dp) / (mu*md));
			  }
			
			sum = sum + tp;
			
			++count;
		      }
		    }
		  }
		}
	      }
	      
	      if ( count > 0 ) {
		sum = sum / (float)count; 
		out->SetComponent(index, 0, sum);
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
