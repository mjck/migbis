

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
#include "vtkmpjImageTensorVariance.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjImageTensorVariance* vtkmpjImageTensorVariance::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageTensorVariance");
  if(ret)
    {
      return (vtkmpjImageTensorVariance*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageTensorVariance;
}

// Construct object with no children.
vtkmpjImageTensorVariance::vtkmpjImageTensorVariance()
{
  this->KernelSize[0] = 3;
  this->KernelSize[1] = 3;
  this->KernelSize[2] = 3;
  this->Mask = NULL;
  this->MaskOutValue = 0;
}

// ----------------------------------------------------------------------------
vtkmpjImageTensorVariance::~vtkmpjImageTensorVariance()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorVariance::ExecuteInformation()
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
  
  output->SetNumberOfScalarComponents(1);  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorVariance::SimpleExecute(vtkImageData *input, vtkImageData *output)
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
  float v[3], r[3];

  float *vals = new float[this->KernelSize[2]*this->KernelSize[1]*this->KernelSize[0]]; 

  /*  printf("mini=%d,minj=%d,mink=%d\n",mini,minj,mink);*/
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  if ((mask != NULL)?(mask->GetComponent(index, 0) > 0):1)
	    {
	      r[0] = in->GetComponent(index, 0);
	      r[1] = in->GetComponent(index, 1);
	      r[2] = in->GetComponent(index, 2);
	      (void)vtkMath::Normalize(r);

	      int count = 0;

	      /* calculate neghborhood variance */
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
			
			v[0] = in->GetComponent(nindex, 0);
			v[1] = in->GetComponent(nindex, 1);
			v[2] = in->GetComponent(nindex, 2);
			(void)vtkMath::Normalize(v);

			float cossine = fabs(r[0]*v[0] + r[1]*v[1] + r[2]*v[2]);
			//			printf("g\n");
			vals[count++] = cossine;
		      }
		    }
		  }
		}
	      }	      
	      
	      //	      printf("count=%d\n",count);

	      // calculate variance
	      if ( count > 0 ) {
		
		// calc mean
		float sum = 0;

		for(int n=0;n<count;n++)
		  sum += vals[n];		

		float mean = sum / (float)count;
		
		//printf("h!\n");
		sum = 0;
		for(int n=0; n<count;n++){
		  sum += (vals[n]-mean)*(vals[n]-mean);
		}
		
		sum = sum / ((float)count-1.0); 
		
		out->SetComponent(index, 0, sqrt(sum));
		
		
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

  delete[] vals;
}
// ----------------------------------------------------------------------------
