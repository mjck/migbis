

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
#include "vtkmpjQBallImaging.h"
#include "vtkMath.h"
#include "math.h"


//------------------------------------------------------------------------------
vtkmpjQBallImaging* vtkmpjQBallImaging::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjQBallImaging");
  if(ret)
    {
      return (vtkmpjQBallImaging*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjQBallImaging;
}

// ----------------------------------------------------------------------------
vtkmpjQBallImaging::vtkmpjQBallImaging()
{
  this->Baseline = NULL;
  this->Mask = NULL;
  this->Gradients = NULL;
  this->Reconstruction = NULL;
  this->Shift = 0.0;
  this->bValue = 1.0;
}

// ----------------------------------------------------------------------------
vtkmpjQBallImaging::~vtkmpjQBallImaging()
{
  if (this->Baseline != NULL)
    this->Baseline->Delete();

  if (this->Mask != NULL)
    this->Mask->Delete();  

  printf("before gradients!\n");

  if (this->Gradients != NULL)
    this->Gradients->Delete();

  if (this->Reconstruction != NULL)
    this->Reconstruction->Delete();

  printf("destruction complete!\n");
}

// ----------------------------------------------------------------------------
void vtkmpjQBallImaging::ExecuteInformation()
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
  
  if (this->Reconstruction == NULL) {
    vtkErrorMacro(<<"No reconstruction points specified!");
    return;
  }

  int gnc = this->Gradients->GetNumberOfComponents();
  int gnt = this->Gradients->GetNumberOfTuples();

  if (gnc < 3) {
    vtkErrorMacro(<<"There should be exactly 3 (gx,gy,gz) components for each gradient direction!");
    return;
  }

  int rnt = this->Reconstruction->GetNumberOfPoints();
    
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
  
  // number of components in the result equals the number of 
  // reconstruction points
  output->SetNumberOfScalarComponents(rnt);

  printf("number of components=%d\n",rnt);

  output->SetScalarType(VTK_FLOAT);
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
double vtkmpjQBallImaging::Smooth(double x, double sigma)
{
  return exp(-(x*x) / (sigma*sigma));
}

// ----------------------------------------------------------------------------
void vtkmpjQBallImaging::ComputeReconstructionMatrix(double **A, double **B, double **C, int n, int m, int p) 
{
  // C[n,p] = A[n,m] * B[m,p]
  for (int i=0;i<n;i++)
    for (int j=0;j<p;j++) {
      C[i][j] = 0;
      for (int k=0;k<m;k++)
	C[i][j] += A[i][k] * B[k][j];
      C[i][j] = this->Smooth(acos(fabs(C[i][j])),1.0);
    }
}

// ----------------------------------------------------------------------------
void vtkmpjQBallImaging::VectorMultiply(double **A, double *b, double *c, int n, int m)
{
  // c[n] = A[n,m] * b[m,1]
  for (int i=0;i<n;i++) {
    c[i] = 0;
    for (int j=0;j<m;j++) {
      c[i] += A[i][j] * b[j];
    }
  }        
}

// ----------------------------------------------------------------------------
void vtkmpjQBallImaging::SimpleExecute(vtkImageData *input, vtkImageData *output)
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
  
  int m = this->Gradients->GetNumberOfTuples();
  int n = this->Reconstruction->GetNumberOfPoints();
  
  /* set up reconstruction matrix */  
  double** U = new double* [n];
  for (int i=0; i<n; i++) {
    double *p = this->Reconstruction->GetPoint(i);
    double x = p[0];
    double y = p[1];
    double z = p[2];
    U[i] = new double[3];
    U[i][0] = x;
    U[i][1] = y;
    U[i][2] = z;
  }
  
  /* set up gradient matrix */
  double** Q = new double* [3];
  for (int i=0; i<3; i++) {
    Q[i] = new double[m];
    for(int j=0; j<m; j++) {
      double x = this->Gradients->GetComponent(j, i);
      Q[i][j] = x;
    }
  }
  
  /* set up reconstruction matrix */
  double **A = new double* [n];
  for (int i=0; i<n; i++) {
    A[i] = new double[m];
  }

  /* set up wavevectors */
  double *e = new double[m];

  /* set up odf vector */
  double *odf = new double[n];

  /* compute reconstruction matrix */
  this->ComputeReconstructionMatrix(U,Q,A,n,3,m);
  
  /* build reconstruction matrix A = phi(arccos(|U'*Q|)) */
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
	    for(int l=0; l<m && !abort; l++) {
	      
	      float value = float(in->GetComponent(index, l)) + this->Shift;
	      abort = (value <= 0);
	      e[l] = log(baseln / value) / this->bValue; 
	      
	    }
	    
	    if (abort == 0) {
	      
	      // Compute ODF
	      this->VectorMultiply(A,e,odf,n,m);
	      
	      for (int z=0;z<n;z++) 
		out->SetComponent(index, z, odf[z]);
	      
	    } else {
	      
	      for(int z=0;z<n;z++) 
		out->SetComponent(index, z, (float)0);
	      
	    }
	    
	  } else {
	    
	    for(int z=0;z<n;z++) 
	      out->SetComponent(index, z, (float)0);
	  }	  
	  
	  ++index;	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);
  
  // free storage
  delete[] odf;
  delete[] e;
  
  for (int i=0; i<n; i++) {
    delete[] U[i];
    delete[] A[i];
  }
  
  for (int i=0; i<3; i++) {
    delete[] Q[i];
  }

}

// ----------------------------------------------------------------------------
