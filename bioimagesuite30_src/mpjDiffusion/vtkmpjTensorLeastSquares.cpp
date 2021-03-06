

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
#include "vtkmpjTensorLeastSquares.h"
#include "vtkMath.h"
#include "math.h"


//------------------------------------------------------------------------------
vtkmpjTensorLeastSquares* vtkmpjTensorLeastSquares::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjTensorLeastSquares");
  if(ret)
    {
      return (vtkmpjTensorLeastSquares*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjTensorLeastSquares;
}

// ----------------------------------------------------------------------------
vtkmpjTensorLeastSquares::vtkmpjTensorLeastSquares()
{
  this->Baseline = NULL;
  this->Mask = NULL;
  this->Gradients = NULL;
  this->Shift = 0.0;
  this->bValue = 1.0;
  this->NumberOfDirections = 6;
  this->SymmetricTensor = 0;
}

// ----------------------------------------------------------------------------
vtkmpjTensorLeastSquares::~vtkmpjTensorLeastSquares()
{
  if (this->Baseline != NULL)
    this->Baseline->Delete();
  if (this->Mask != NULL)
    this->Mask->Delete();  
  if (this->Gradients != NULL)
    this->Gradients->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjTensorLeastSquares::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  if (this->Baseline == NULL) {
    vtkErrorMacro(<<"No baseline (S0) image specified!");
    return;
  }
  
  if (this->Gradients == NULL) {
    vtkErrorMacro(<<"No gradient directions specified!");
    return;
  }
  
  int gnc = this->Gradients->GetNumberOfComponents();
  int gnt = this->Gradients->GetNumberOfTuples();

  if (gnc < 3) {
    vtkErrorMacro(<<"There should be exactly 3 (gx,gy,gz) components for each gradient direction!");
    return;
  }
  
  int dim[3];  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();
    
  if (nc < 6) {
    vtkErrorMacro(<<"There should be at least 6 diffusion-weighted images in the dataset!");
    return;
  }
  
  if (gnt != nc) {
    vtkErrorMacro(<<"There should be exactly the same number of DWs (" <<nc<<") as the number of gradient directions ("<<gnt<<") !");
    return;
  }
  
  if (this->SymmetricTensor) {
    output->SetNumberOfScalarComponents(6);
  } else {
    output->SetNumberOfScalarComponents(9);
  }
  output->SetScalarType(VTK_FLOAT);
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjTensorLeastSquares::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkDataArray* in =input->GetPointData()->GetScalars();
  
  int dim[3];  input->GetDimensions(dim);
    vtkDataArray* out=output->GetPointData()->GetScalars();
    
  vtkDataArray* baseline=NULL;
  vtkDataArray* mask=NULL;
    
  if (this->Baseline != NULL) 
    baseline = this->Baseline->GetPointData()->GetScalars();
  
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  /* compute gradient matrix */
  double** alpha = new double* [this->NumberOfDirections];
  for (int i=0; i<this->NumberOfDirections; i++) {
    double gx = this->Gradients->GetComponent(i, 0);
    double gy = this->Gradients->GetComponent(i, 1);
    double gz = this->Gradients->GetComponent(i, 2);
    alpha[i] = new double[6];
    alpha[i][0] = gx*gx;
    alpha[i][1] = gy*gy;
    alpha[i][2] = gz*gz;
    alpha[i][3] = 2*gx*gy;
    alpha[i][4] = 2*gx*gz;
    alpha[i][5] = 2*gy*gz;
  }
  
  /* allocate space for adc components */
  double **adc = new double* [this->NumberOfDirections];
  for (int i=0; i<this->NumberOfDirections; i++) {
    adc[i] = new double[1];    
  }
  
  /* allocate memory for tensor */
  double **t = new double* [6];
  for (int i=0; i<6; i++) {
    t[i] = new double[1];    
  }

  int nc = output->GetNumberOfScalarComponents();

  int inmask;
  int index=0;

  for (int k=0;k<dim[2];k++) {    
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  float baseln;
	  
	  baseln = float(baseline->GetComponent(index, 0)) + this->Shift;
	  inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;
	  
	  if (inmask) {
	    
	    int abort = 0;	    	 

	    // Calculate ADCs
	    for(int n=0; n<this->NumberOfDirections && !abort; n++) {
	      
	      float value = float(in->GetComponent(index, n)) + this->Shift;
	      abort = (value <= 0);
	      adc[n][0] = log(baseln / value) / this->bValue; 
	      //printf("%f ",adc[n][0]);	     
	    }

	    /*	    if (k==19 && ((i==58)||(i==75)) && ((j==58)||(j==75))) {
		    float *p = input->GetPoint(index);
		    printf("[%f, %f, %f]:",p[0],p[1],p[2]);
		    for(int n=0; n<this->NumberOfDirections; n++) {
		    printf("%f ",adc[n][0]);
		    }
		    printf("\n");
		    }*/

	    //printf("\n");

	    if (abort == 0) {
	      
	      // Solve system of equations by least-squares	      
	      (void) vtkMath::SolveLeastSquares(this->NumberOfDirections,alpha,6,adc,1,t);
	      
	      float Dxx = t[0][0];
	      float Dyy = t[1][0];
	      float Dzz = t[2][0];
	      float Dxy = t[3][0];
	      float Dxz = t[4][0];
	      float Dyz = t[5][0];
        if (!std::isfinite(Dxx) || !std::isfinite(Dyy) || 
            !std::isfinite(Dzz) || !std::isfinite(Dxy) ||
            !std::isfinite(Dxz) || !std::isfinite(Dyz))
        { Dxx = Dyy = Dzz = Dxy = Dxz = Dyz = 0; }
      
	      if (this->SymmetricTensor) {
		out->SetComponent(index, 0, Dxx);
		out->SetComponent(index, 1, Dxy);
		out->SetComponent(index, 2, Dxz);
		out->SetComponent(index, 3, Dyy);
		out->SetComponent(index, 4, Dyz);
		out->SetComponent(index, 5, Dzz);
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
	      }
	      
	    } else {
	      for(int z=0;z<nc;z++) 
		out->SetComponent(index, z, (float)0);
	      
	    }
	    
	  } else {
	    
	    for(int z=0;z<nc;z++) 
	      out->SetComponent(index, z, (float)0);
	  }	  
	  
	  ++index;	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);

  // free storage
  //for (int i=0; i<this->NumberOfDirections; i++) {
  //delete[] alpha[i];
  // delete[] adc[i];
  //}

  delete[] alpha;
  delete[] adc;
  
  //for (int i=0; i<6; i++) {
  // delete[] t[i];
  //}
  
  delete[] t;
  
}

// ----------------------------------------------------------------------------
