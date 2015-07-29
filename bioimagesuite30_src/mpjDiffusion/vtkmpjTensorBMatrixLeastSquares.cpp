

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
#include "vtkmpjTensorBMatrixLeastSquares.h"
#include "vtkMath.h"
#include "math.h"


//------------------------------------------------------------------------------
vtkmpjTensorBMatrixLeastSquares* vtkmpjTensorBMatrixLeastSquares::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjTensorBMatrixLeastSquares");
  if(ret)
    {
      return (vtkmpjTensorBMatrixLeastSquares*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjTensorBMatrixLeastSquares;
}

// ----------------------------------------------------------------------------
vtkmpjTensorBMatrixLeastSquares::vtkmpjTensorBMatrixLeastSquares()
{
  this->Mask = NULL;
  this->Gradients = NULL;
  this->Shift = 0.0;
  this->bValues = NULL;
  this->NumberOfDirections = 6;
  this->NumberOfbValues = 7;
  this->SymmetricTensor = 0;
}

// ----------------------------------------------------------------------------
vtkmpjTensorBMatrixLeastSquares::~vtkmpjTensorBMatrixLeastSquares()
{
  if (this->Mask != NULL)
    this->Mask->Delete();  
  if (this->Gradients != NULL)
    this->Gradients->Delete();
  if (this->bValues != NULL)
    this->bValues->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjTensorBMatrixLeastSquares::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  if (this->Gradients == NULL) {
    vtkErrorMacro(<<"No gradient directions specified!");
    return;
  }
  
  int gnc = this->Gradients->GetNumberOfComponents();
  int gnt = this->Gradients->GetNumberOfTuples();
  int bnt = this->bValues->GetNumberOfTuples();

  if (gnc < 3) {
    vtkErrorMacro(<<"There should be exactly 3 (gx,gy,gz) components for each gradient direction!");
    return;
  }
  
  if (bnt < 7) {
    vtkErrorMacro(<<"There should be at least 7 b-values (one b==0 + six b>0" );
    return;
  }

  int dim[3];  input->GetDimensions(dim);
  int nc = input->GetNumberOfScalarComponents();

  if (this->SymmetricTensor) {
    output->SetNumberOfScalarComponents(7);
  } else {
    output->SetNumberOfScalarComponents(10);
  }

  if (nc != bnt) {
    vtkErrorMacro(<<"Number of DWs is not enough");
    return;
  }
  
  output->SetScalarType(VTK_FLOAT);
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjTensorBMatrixLeastSquares::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkDataArray* in =input->GetPointData()->GetScalars();
  
  int dim[3];  input->GetDimensions(dim);
    vtkDataArray* out=output->GetPointData()->GetScalars();
    
  vtkDataArray* mask=NULL;
  
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();

  int nc = input->GetNumberOfScalarComponents();
  
  /* compute B Matrix */
  double** B = new double* [nc];
  
  int k = 0;

  for (int j = 0; j<this->NumberOfDirections && k<nc; j++) {
    
    double gx = this->Gradients->GetComponent(j, 0);
    double gy = this->Gradients->GetComponent(j, 1);
    double gz = this->Gradients->GetComponent(j, 2);
    double bvalue = this->bValues->GetComponent(j, 0);
    
    B[k] = new double[7];
    B[k][0] = 1.0;
    B[k][1] = -bvalue * gx*gx;
    B[k][2] = -bvalue * gy*gy;
    B[k][3] = -bvalue * gz*gz;
    B[k][4] = -2.0 * bvalue * gx*gy;
    B[k][5] = -2.0 * bvalue * gx*gz;
    B[k][6] = -2.0 * bvalue * gy*gz;
    //B[k][6] = 1.0;

    k++;

  }

  if (k != nc)
    printf("Oops! something is wrong with the B Matrix!\n" );
  
  /* allocate space for Nx1 vector x */
  double **x = new double* [nc];
  for (int i=0; i<nc; i++) {
    x[i] = new double[1];
  }
  
  /* allocate memory for tensor */
  double **t = new double* [7];
  for (int i=0; i<7; i++) {
    t[i] = new double[1];  
  
  }

  int inmask;
  int index=0;

  for (int k=0;k<dim[2];k++) {    
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  float baseln;
	  
	  //	  baseln = float(baseline->GetComponent(index, 0)) + this->Shift;
	  inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;
	  
	  if (inmask) {
	    
	    int abort = 0;	    	 

	    // Calculate ln(Si), 1<=i<=N
	    for(int n=0; n<nc; n++) {
	      
	      float value = float(in->GetComponent(index, n)) + this->Shift;
	      abort = (value <= 0);
	      x[n][0] = log(value);

	    }

	    if (abort == 0) {
	      
	      // Solve system of equations by least-squares	      
	      (void) vtkMath::SolveLeastSquares(nc,B,7,x,1,t);
	      
        float lnS0 = t[0][0]; 
	      float Dxx = t[1][0];
	      float Dyy = t[2][0];
	      float Dzz = t[3][0];
	      float Dxy = t[4][0];
	      float Dxz = t[5][0];
	      float Dyz = t[6][0];
        //float lnS0 = t[6][0]; 
	      	      	      	      	    
	      if (this->SymmetricTensor) {
      		out->SetComponent(index, 0, Dxx);
      		out->SetComponent(index, 1, Dxy);
      		out->SetComponent(index, 2, Dxz);
      		out->SetComponent(index, 3, Dyy);
      		out->SetComponent(index, 4, Dyz);
      		out->SetComponent(index, 5, Dzz);
          out->SetComponent(index, 6, exp(lnS0));
	      } else {
      		out->SetComponent(index, 0, Dxx);
      		out->SetComponent(index, 1, Dxy);
      		out->SetComponent(index, 2, Dxz);
      		out->SetComponent(index, 3, Dxy);
      		out->SetComponent(index, 4, Dyy);
      		out->SetComponent(index, 5, Dyz);
      		out->SetComponent(index, 6, Dxz);
      		out->SetComponent(index, 7, Dyz);
      		out->SetComponent(index, 8, Dzz);
          out->SetComponent(index, 9, exp(lnS0));
	      }
	      
	    } else {
	      for(int z=0;z<(this->SymmetricTensor?7:10);z++) 
		      out->SetComponent(index, z, (float)0);	      
	    }
	    
	  } else {
	    
	    for(int z=0;z<(this->SymmetricTensor?7:10);z++) 
	      out->SetComponent(index, z, (float)0);
	  }	  
	  
	  ++index;	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);

  // free storage
  delete[] B;
  delete[] x;  
  delete[] t;
  
}

// ----------------------------------------------------------------------------
