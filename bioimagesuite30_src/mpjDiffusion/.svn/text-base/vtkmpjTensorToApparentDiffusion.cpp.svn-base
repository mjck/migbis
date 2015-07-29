

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
#include "vtkmpjTensorToApparentDiffusion.h"
#include "vtkMath.h"
#include "math.h"


//------------------------------------------------------------------------------
vtkmpjTensorToApparentDiffusion* vtkmpjTensorToApparentDiffusion::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjTensorToApparentDiffusion");
  if(ret)
    {
      return (vtkmpjTensorToApparentDiffusion*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjTensorToApparentDiffusion;
}

// ----------------------------------------------------------------------------
vtkmpjTensorToApparentDiffusion::vtkmpjTensorToApparentDiffusion()
{
  this->Baseline = NULL;
  this->Mask = NULL;
  this->Gradients = NULL;
  this->Shift = 0.0;
  this->bValue = 1.0;
  this->NumberOfDirections = 6;
  this->SymmetricTensor = 0;
  this->BaselineValue = 10;
  this->AddNoise = 0;
  this->NoiseLevel = 0.1;
}

// ----------------------------------------------------------------------------
vtkmpjTensorToApparentDiffusion::~vtkmpjTensorToApparentDiffusion()
{
  if (this->Baseline != NULL)
    this->Baseline->Delete();
  if (this->Mask != NULL)
    this->Mask->Delete();  
  if (this->Gradients != NULL)
    this->Gradients->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjTensorToApparentDiffusion::ExecuteInformation()
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

  if (gnc < 3) {
    vtkErrorMacro(<<"There should be exactly 3 (gx,gy,gz) components for each gradient direction!");
    return;
  }
  
  int dim[3];  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();
    
  if (nc < 6) {
    vtkErrorMacro(<<"There should be at least 6 elements in the dataset!");
    return;
  }
  
  output->SetNumberOfScalarComponents(gnt + 1);
  output->SetScalarType(VTK_FLOAT);
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
float vtkmpjTensorToApparentDiffusion::GaussianDeviate()
{
  static int iset=0;
  static float gset;
  float fac,rsq,v1,v2;

  if  (iset == 0) 
    {
      do 
	{
	  v1=2.0*vtkMath::Random()-1.0;
	  v2=2.0*vtkMath::Random()-1.0;
	  rsq=v1*v1+v2*v2;
	} 
      while (rsq >= 1.0 || rsq == 0.0);
      
      fac=sqrt(-2.0*log(rsq)/rsq);
      gset=v1*fac;
      iset=1;
      return v2*fac;
    } 
  else
    {
      iset=0;
      return gset;
    }
}

// ----------------------------------------------------------------------------
void vtkmpjTensorToApparentDiffusion::SimpleExecute(vtkImageData *input, vtkImageData *output)
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

  int index=0;

  for (int k=0;k<dim[2];k++) {    
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  int inmask = 1;
	  float baseln;
	  float noise = 0.0;

	  // read in tensor
	  if (this->SymmetricTensor) {
	    t[0][0] = in->GetComponent(index, 0); /* xx */
	    t[1][0] = in->GetComponent(index, 3); /* yy */
	    t[2][0] = in->GetComponent(index, 5); /* zz */
	    t[3][0] = in->GetComponent(index, 1); /* xy */
	    t[4][0] = in->GetComponent(index, 2); /* xz */
	    t[5][0] = in->GetComponent(index, 4); /* yz */
	  } else {
	    t[0][0] = in->GetComponent(index, 0); /* xx */
	    t[1][0] = in->GetComponent(index, 4); /* yy */
	    t[2][0] = in->GetComponent(index, 8); /* zz */
	    t[3][0] = in->GetComponent(index, 1); /* xy */
	    t[4][0] = in->GetComponent(index, 2); /* xz */
	    t[5][0] = in->GetComponent(index, 5); /* yz */
	  }
	  
	  baseln = (baseline != NULL) ? 
	    (baseline->GetComponent(index, 0) + this->Shift) : this->BaselineValue;

	  inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;

	  // write out baseline image
	  if (this->AddNoise) 
	    noise = this->NoiseLevel * this->GaussianDeviate(); 
	  else noise = 0;
	  
	  if (inmask)
	    baseln = 1+noise;
	  else baseln = 0+fabs(noise);

	  out->SetComponent(index, 0, 1.0);

	  float total = 0;

	  //if (output) {
	    
	    float sum; 

	    // calculate ADCs
	    for(int n=0; n<this->NumberOfDirections; n++) {
	      
	      // write out ADCs
	      if (this->AddNoise) {
	      	noise = this->NoiseLevel * this->GaussianDeviate();
	      }	else noise = 0.0;

	      sum = 0.0;
	      for(int m=0; m<6; m++) 
		sum += alpha[n][m] * t[m][0];// * (noise);
	      	   	      
	      //adc[n][0] = baseln * exp(-(sum + noise));
	      adc[n][0] = baseln * exp(-sum); // + noise);

	      //printf("%f ",adc[n][0]);
	      
	      //out->SetComponent(index, n+1, adc[n][0] + noise*adc[n][0]);
	      //printf("%f ", exp(sum));
	      out->SetComponent(index, n+1, adc[n][0]);
	    }
	    	    
	    // printf("\n*");

	    //printf("\n");

	    // calculate baseline
	    //baseln = total / (float)this->NumberOfDirections;
	    
	    //printf("baseln=%f \n",baseln);
	    
	    // write out baseline image
	    //if (this->AddNoise) 
	    //noise = this->NoiseLevel * this->GaussianDeviate(); 
	    //else noise = 0;
	    
	    //out->SetComponent(index, 0, baseln + noise*baseln);
	    
	    // write out normalized ADCs
	    //for(int n=0; n<this->NumberOfDirections; n++) 
	    //out->SetComponent(index, n+1, out->GetComponent(index, n+1)*baseln);
	    
	    // } else {
	    
	    //for(int m=0; m<=this->NumberOfDirections; m++) 
	    //out->SetComponent(index, m, (float)0);	    
	    
	    //}
		  
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
