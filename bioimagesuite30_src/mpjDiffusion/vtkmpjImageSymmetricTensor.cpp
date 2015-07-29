

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
#include "vtkmpjImageSymmetricTensor.h"

//------------------------------------------------------------------------------
vtkmpjImageSymmetricTensor* vtkmpjImageSymmetricTensor::New() 
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageSymmetricTensor");
  
  if(ret) {
    return (vtkmpjImageSymmetricTensor*)ret;
  }
  
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageSymmetricTensor;
}

// Construct object with no children.
vtkmpjImageSymmetricTensor::vtkmpjImageSymmetricTensor()
{  
  Angle = 0.0;
  AddNoise = 0;
  NoiseLevel = 10.0;
  Eigenvalues[0]=1.0;
  Eigenvalues[1]=1.0;
  Eigenvalues[2]=1.0;
  Direction[0]=1.0;
  Direction[1]=0.0;
  Direction[2]=0.0;
  Mask = NULL;
  InverseMask = 0;
  UseInputForDirection = 0;
  OutputType = VTK_MPJ_OUTPUT_FULL_TENSOR;
}

// ----------------------------------------------------------------------------
vtkmpjImageSymmetricTensor::~vtkmpjImageSymmetricTensor()
{
  if (this->Mask != NULL)
    this->Mask->Delete();  
}

// ----------------------------------------------------------------------------
float vtkmpjImageSymmetricTensor::GetGaussianDeviate()
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
void vtkmpjImageSymmetricTensor::PrintMatrix3x3(char *title, float M[3][3])
{
  printf("%s",title);
  printf("%f %f %f\n",M[0][0],M[0][1],M[0][2]);
  printf("%f %f %f\n",M[1][0],M[1][1],M[1][2]);
  printf("%f %f %f\n",M[2][0],M[2][1],M[2][2]);
  printf("===============================\n");
}

// ----------------------------------------------------------------------------
void vtkmpjImageSymmetricTensor::ExecuteInformation()
{
  // start with defaults
  vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  vtkImageData *output = this->GetOutput();
  vtkImageData *input = this->GetInput();
  int dim[3];  input->GetDimensions(dim);
  
  if (this->OutputType == VTK_MPJ_OUTPUT_FULL_TENSOR) {
    output->SetNumberOfScalarComponents(9);
  } else {
    output->SetNumberOfScalarComponents(6);
  }
    
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageSymmetricTensor::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* in=input->GetPointData()->GetScalars(); 
  vtkDataArray* out=output->GetPointData()->GetScalars();  

  int nc = input->GetNumberOfScalarComponents();
  
  vtkDataArray* mask = (this->Mask != NULL) ? 
    this->Mask->GetPointData()->GetScalars() : NULL;
  
  float D[3][3]={{0,0,0},{0,0,0},{0,0,0}};  /* matrix of eigenvalues in mask */
  float V[3][3];                            /* matrix of eigenvectors in mask */
  float v1[3], v2[3], v3[3];
  float R1[3][3], R2[3][3];
  float T[3][3];                            /* tensor matrix */

  float Dxx;
  float Dyy;
  float Dzz;
  float Dxy;
  float Dxz;
  float Dyz;

  int index=0;

  D[0][0] = this->Eigenvalues[0];
  D[1][1] = this->Eigenvalues[1];
  D[2][2] = this->Eigenvalues[2];

  this->Direction[0] = 1.0;
  this->Direction[1] = 0.0;
  this->Direction[2] = 0.0;
 
  //  (void) vtkMath::Normalize(this->Direction);
  
  //printf("Direction=%f %f %f\n",this->Direction[0],this->Direction[1],this->Direction[2]);
  //printf("Eigenvalues=%f %f %f\n",this->Eigenvalues[0],this->Eigenvalues[1],this->Eigenvalues[2]);

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{		
	  float noise[6] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
	  
	  /* generate noise */
	  if (this->AddNoise) {
	    for(int p=0; p<6; p++)
	      noise[p] = this->NoiseLevel * this->GetGaussianDeviate();
	  }	    	    

	  if ((k > 24) && (k < 40) && (j > 24) && (j < 40) && (i > 8) && (i < 56)) {	   	    
	    D[0][0] = this->Eigenvalues[0] + noise[0];
	    D[1][1] = this->Eigenvalues[1] + noise[1];
	    D[2][2] = this->Eigenvalues[2] + noise[2];
	  } else {
	    D[0][0] = this->Eigenvalues[2] + noise[0];
	    D[1][1] = this->Eigenvalues[2] + noise[1];
	    D[2][2] = this->Eigenvalues[2] + noise[2];
	  }
	  
	  //float maskval = mask->GetComponent(index, 0);
	  //int output = (mask != NULL)?((this->InverseMask ? maskval <= 0 : maskval > 0)):1;
	  //	  int output=1;

// 	  if (this->UseInputForDirection) {	   
// 	    v1[0] = in->GetComponent(index, 0);
// 	    v1[1] = in->GetComponent(index, 1);
// 	    v1[2] = in->GetComponent(index, 2);
// 	    (void) vtkMath::Normalize(v1);
// 	    V[0][0] =  v1[0];
// 	    V[1][0] =  v1[1];
// 	    V[2][0] =  v1[2];
// 	    } else {
	    V[0][0] =  v1[0] = this->Direction[0];
	    V[1][0] =  v1[1] = this->Direction[1];
	    V[2][0] =  v1[2] = this->Direction[2];
	    //	  }
	  
	    //if (v1[0] == 0 && v1[1] == 0 && v1[2] == 0)
	    //output = 0;
	    //else {
	    // printf("%f %f %f\n",v1[0],v1[1],v1[2]);
	    //}

	    //if (output) {	    	  	    

	    //if (nc == 3) {
	      /* get two perpendicular vectors */
	      (void) vtkMath::Perpendiculars(v1,v2,v3,this->Angle);

	      //} else {

	      /* get other two perpendicular vectors */
	      //v2[0] = in->GetComponent(index, 3);
	      //v2[1] = in->GetComponent(index, 4);
	      //v2[2] = in->GetComponent(index, 5);
	      //(void) vtkMath::Normalize(v2);
	      
	      //v3[0] = in->GetComponent(index, 6);
	      //v3[1] = in->GetComponent(index, 7);
	      //v3[2] = in->GetComponent(index, 8);
	      //(void) vtkMath::Normalize(v3);	  
	      
	      //}
	    
	    
	    V[0][1]=v2[0]=0.0; 
	    V[1][1]=v2[1]=1.0;    /* second eigenvector */
	    V[2][1]=v2[2]=0.0;
	    
	    V[0][2]=v3[0]=0.0; 
	    V[1][2]=v3[1]=0.0;    /* third eigenvector */
	    V[2][2]=v3[2]=1.0;   	    
	    
	    /* Tensor = V * D * V' */
	    (void) vtkMath::Transpose3x3(V,R1);
	    (void) vtkMath::Multiply3x3(D,R1,R2);
	    (void) vtkMath::Multiply3x3(V,R2,T);
	    
	    // (this->Debug)
	    //this->PrintMatrix3x3("T=\n",T);
	    
	    /* generate noise */
	    //if (this->AddNoise) {
	    //for(int p=0; p<6; p++)
	    //noise[p] = this->NoiseLevel * this->GetGaussianDeviate();
	    //}	    	    
	 	  
	    Dxx = T[0][0]; //+ noise[0];
	    Dyy = T[1][1]; //+ noise[3];
	    Dzz = T[2][2];// + noise[5];
	    Dxy = T[1][0];// + noise[1];
	    Dxz = T[2][0];// + noise[2];
	    Dyz = T[2][1];// + noise[4];

	    //} else {
	    
	    //Dxx = 0 + noise[0];
	    //Dyy = 0 + noise[3];
	    //Dzz = 0 + noise[5];
	    //Dxy = 0 + noise[1];
	    //Dxz = 0 + noise[2];
	    //Dyz = 0 + noise[4];
	    
	    //}

	  if (this->OutputType == VTK_MPJ_OUTPUT_FULL_TENSOR) {
	    out->SetComponent(index, 0, Dxx);
	    out->SetComponent(index, 1, Dxy);
	    out->SetComponent(index, 2, Dxz);
	    out->SetComponent(index, 3, Dxy);
	    out->SetComponent(index, 4, Dyy);
	    out->SetComponent(index, 5, Dyz);
	    out->SetComponent(index, 6, Dxz);
	    out->SetComponent(index, 7, Dyz);
	    out->SetComponent(index, 8, Dzz); 

	  } else {
	    out->SetComponent(index, 0, Dxx);
	    out->SetComponent(index, 1, Dxy);
	    out->SetComponent(index, 2, Dxz);
	    out->SetComponent(index, 3, Dyy);
	    out->SetComponent(index, 4, Dyz);
	    out->SetComponent(index, 5, Dzz);	    
	  }

	  ++index;	   	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);
}

// ----------------------------------------------------------------------------
