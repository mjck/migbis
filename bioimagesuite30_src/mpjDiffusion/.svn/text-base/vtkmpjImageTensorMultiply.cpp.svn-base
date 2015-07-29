

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
#include "vtkmpjImageTensorMultiply.h"

//------------------------------------------------------------------------------
vtkmpjImageTensorMultiply* vtkmpjImageTensorMultiply::New() 
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageTensorMultiply");
  
  if(ret) {
    return (vtkmpjImageTensorMultiply*)ret;
  }
  
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageTensorMultiply;
}

// Construct object with no children.
vtkmpjImageTensorMultiply::vtkmpjImageTensorMultiply()
{  
  this->NoiseLevel = 5.0;
}

// ----------------------------------------------------------------------------
vtkmpjImageTensorMultiply::~vtkmpjImageTensorMultiply()
{
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorMultiply::SetInput1(vtkImageData *input)
{
  this->vtkProcessObject::SetNthInput(0, input);
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorMultiply::SetInput2(vtkImageData *input)
{
  this->vtkProcessObject::SetNthInput(1, input);
}

//----------------------------------------------------------------------------
vtkImageData *vtkmpjImageTensorMultiply::GetInput(int index)
{
  if (this->NumberOfInputs <= index)
    {
      return NULL;
    }
   
  return (vtkImageData *)(this->Inputs[index]);
}

// ----------------------------------------------------------------------------
float vtkmpjImageTensorMultiply::GetGaussianDeviate()
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

//----------------------------------------------------------------------------
void vtkmpjImageTensorMultiply::PrintMatrix3x3(char *title, float M[3][3])
{
  printf("%s",title);
  printf("%f %f %f\n",M[0][0],M[0][1],M[0][2]);
  printf("%f %f %f\n",M[1][0],M[1][1],M[1][2]);
  printf("%f %f %f\n",M[2][0],M[2][1],M[2][2]);
  printf("===============================\n");
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorMultiply::ExecuteInformation()
{
  // start with defaults
  vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  vtkImageData *output = this->GetOutput();
  vtkImageData *input = this->GetInput(0);
  int dim[3];  input->GetDimensions(dim);
  
  if (input->GetNumberOfScalarComponents() == 9) {
    output->SetNumberOfScalarComponents(9);
  } else {
    output->SetNumberOfScalarComponents(6);
  }
    
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorMultiply::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  vtkImageData *input1 = this->GetInput(0);
  vtkImageData *input2 = this->GetInput(1);
  int numcomp = input1->GetNumberOfScalarComponents();
  int dim[3];  input1->GetDimensions(dim);

  vtkDataArray* in1=input1->GetPointData()->GetScalars(); 
  vtkDataArray* in2=input2->GetPointData()->GetScalars(); 
  vtkDataArray* out=output->GetPointData()->GetScalars();  

  float T1[3][3], T2[3][3];                            /* tensor matrix */
  float R[3][3];
  float noise[6];

  float Dxx;
  float Dyy;
  float Dzz;
  float Dxy;
  float Dxz;
  float Dyz;

  int index=0;

  printf("number of components=%d\n", numcomp);

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{		
	  if (numcomp == 9) {

	    T1[0][0] = Dxx = in1->GetComponent(index, 0);
	    T1[1][0] = Dxy = in1->GetComponent(index, 1);
	    T1[2][0] = Dxz = in1->GetComponent(index, 2);
	    T1[0][1] = Dxy = in1->GetComponent(index, 3);
	    T1[1][1] = Dyy = in1->GetComponent(index, 4);
	    T1[2][1] = Dyz = in1->GetComponent(index, 5);
	    T1[0][2] = Dxz = in1->GetComponent(index, 6);
	    T1[1][2] = Dyz = in1->GetComponent(index, 7);
	    T1[2][2] = Dzz = in1->GetComponent(index, 8);

	    
	    T2[0][0] = Dxx = in2->GetComponent(index, 0);
	    T2[1][0] = Dxy = in2->GetComponent(index, 1);
	    T2[2][0] = Dxz = in2->GetComponent(index, 2);
	    T2[0][1] = Dxy = in2->GetComponent(index, 3);
	    T2[1][1] = Dyy = in2->GetComponent(index, 4);
	    T2[2][1] = Dyz = in2->GetComponent(index, 5);
	    T2[0][2] = Dxz = in2->GetComponent(index, 6);
	    T2[1][2] = Dyz = in2->GetComponent(index, 7);
	    T2[2][2] = Dzz = in2->GetComponent(index, 8);

	  } else {
	  
	    T1[0][0] = Dxx = in1->GetComponent(index, 0);
	    T1[1][0] = T1[0][1] = Dxy = in1->GetComponent(index, 1);
	    T1[2][0] = T1[0][2] = Dxz = in1->GetComponent(index, 2);
	    T1[1][1] = Dyy = in1->GetComponent(index, 3);
	    T1[2][1] = T1[1][2] = Dyz = in1->GetComponent(index, 4);
	    T1[2][2] = Dzz = in1->GetComponent(index, 5);
	    
	    T2[0][0] = Dxx = in2->GetComponent(index, 0);
	    T2[1][0] = T2[0][1] = Dxy = in2->GetComponent(index, 1);
	    T2[2][0] = T2[0][2] = Dxz = in2->GetComponent(index, 2);
	    T2[1][1] = Dyy = in2->GetComponent(index, 3);
	    T2[2][1] = T2[1][2] = Dyz = in1->GetComponent(index, 4);
	    T2[2][2] = Dzz = in2->GetComponent(index, 5);
	    
	  }
	  
	  (void) vtkMath::Multiply3x3(T1,T2,R);

	   for(int l=0; l<6; l++)
	     noise[l] = this->NoiseLevel * this->GetGaussianDeviate();

	   if (R[0][0]==1.0 && R[1][1]==1.0 && R[2][2]==1.0) {
	     R[0][0]=0; R[1][1]=0; R[2][2]=0;
	   }

	  Dxx = R[0][0] + noise[0];
	  Dyy = R[1][1] + noise[3];
	  Dzz = R[2][2] + noise[5];
	  Dxy = R[0][1] + noise[1];
	  Dxz = R[0][2] + noise[2];
	  Dyz = R[1][2] + noise[4];
	  
	  if (numcomp == 9) {
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
