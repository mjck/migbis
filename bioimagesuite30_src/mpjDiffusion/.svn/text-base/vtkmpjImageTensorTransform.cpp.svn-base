

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
#include "vtkmpjImageTensorTransform.h"

//------------------------------------------------------------------------------
vtkmpjImageTensorTransform* vtkmpjImageTensorTransform::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageTensorTransform");

  if(ret) {
    return (vtkmpjImageTensorTransform*)ret;
  }

  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageTensorTransform;
}

// Construct object with no children.
vtkmpjImageTensorTransform::vtkmpjImageTensorTransform()
{
  this->Rotation[0]=0.0;
  this->Rotation[1]=0.0;
  this->Rotation[2]=0.0;
}

// ----------------------------------------------------------------------------
vtkmpjImageTensorTransform::~vtkmpjImageTensorTransform()
{
}

//----------------------------------------------------------------------------
void vtkmpjImageTensorTransform::PrintMatrix3x3(char *title, float M[3][3])
{
  printf("%s",title);
  printf("%f %f %f\n",M[0][0],M[0][1],M[0][2]);
  printf("%f %f %f\n",M[1][0],M[1][1],M[1][2]);
  printf("%f %f %f\n",M[2][0],M[2][1],M[2][2]);
  printf("===============================\n");
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorTransform::ExecuteInformation()
{
  // start with defaults
  vtkpxSimpleImageToImageFilter::ExecuteInformation();

  vtkImageData *output = this->GetOutput();
  vtkImageData *input = this->GetInput();
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
void vtkmpjImageTensorTransform::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int numcomp = input->GetNumberOfScalarComponents();
  int dim[3];  input->GetDimensions(dim);

  vtkDataArray* in=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();

  float T[3][3];                            /* tensor matrix */
  float R[3][3];                            /* aggregate rotation */
  float RT[3][3];                           /* its transpose */
  float D[3][3];                            /* transformed tensor */

  float Rx[3][3];
  float Ry[3][3];
  float Rz[3][3];

  float Temp[3][3];

  float Dxx;
  float Dyy;
  float Dzz;
  float Dxy;
  float Dxz;
  float Dyz;

  //float twopi= 2.0 * vtkMath::Pi();
  float theta;

  //printf("Rotation=%f,%f,%f\n",this->Rotation[0],this->Rotation[1],this->Rotation[2]);

  // rotation about x
#if ((VTK_MAJOR_VERSION >= 5)&&(VTK_MINOR_VERSION >= 4))
  theta = vtkMath::RadiansFromDegrees(Rotation[0]);
#else
  theta = Rotation[0] * vtkMath::DegreesToRadians();
#endif
  Rx[0][0] = 1;  Rx[0][1] = 0;           Rx[0][2] = 0;
  Rx[1][0] = 0;  Rx[1][1] = cos(theta);  Rx[1][2] = -sin(theta);
  Rx[2][0] = 0;  Rx[2][1] = sin(theta);  Rx[2][2] = cos(theta);

  // rotation about y
#if ((VTK_MAJOR_VERSION >= 5)&&(VTK_MINOR_VERSION >= 4))
  theta = vtkMath::RadiansFromDegrees(Rotation[1]);
#else
  theta = Rotation[1] * vtkMath::DegreesToRadians();
#endif
  Ry[0][0] = cos(theta);   Ry[0][1] = 0;  Ry[0][2] = sin(theta);
  Ry[1][0] = 0;            Ry[1][1] = 1;  Ry[1][2] = 0;
  Ry[2][0] = -sin(theta);  Ry[2][1] = 0;  Ry[2][2] = cos(theta);

  // rotation about z
#if ((VTK_MAJOR_VERSION >= 5)&&(VTK_MINOR_VERSION >= 4))
  theta = vtkMath::RadiansFromDegrees(Rotation[2]);
#else
  theta = Rotation[0] * vtkMath::DegreesToRadians();
#endif
  Rz[0][0] = cos(theta);  Rz[0][1] = -sin(theta);  Rz[0][2] = 0;
  Rz[1][0] = sin(theta);  Rz[1][1] = cos(theta);   Rz[1][2] = 0;
  Rz[2][0] = 0;           Rz[2][1] = 0;            Rz[2][2] = 1;

  // compute aggregate rotation matrix
  (void) vtkMath::Multiply3x3(Rz,Rx,Temp);
  (void) vtkMath::Multiply3x3(Temp,Ry,R);
  (void) vtkMath::Transpose3x3(R,RT);

  int index=0;

  //printf("number of components=%d\n", numcomp);

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  if (numcomp == 9) {

	    T[0][0] = Dxx = in->GetComponent(index, 0);
	    T[1][0] = Dxy = in->GetComponent(index, 1);
	    T[2][0] = Dxz = in->GetComponent(index, 2);
	    T[0][1] = Dxy = in->GetComponent(index, 3);
	    T[1][1] = Dyy = in->GetComponent(index, 4);
	    T[2][1] = Dyz = in->GetComponent(index, 5);
	    T[0][2] = Dxz = in->GetComponent(index, 6);
	    T[1][2] = Dyz = in->GetComponent(index, 7);
	    T[2][2] = Dzz = in->GetComponent(index, 8);

	  } else {
	    
	    T[0][0] = Dxx = in->GetComponent(index, 0);
	    T[1][0] = T[0][1] = Dxy = in->GetComponent(index, 1);
	    T[2][0] = T[0][2] = Dxz = in->GetComponent(index, 2);
	    T[1][1] = Dyy = in->GetComponent(index, 3);
	    T[2][1] = T[1][2] = Dyz = in->GetComponent(index, 4);
	    T[2][2] = Dzz = in->GetComponent(index, 5);

	  }
	  
	  // multiply tensor
	  (void) vtkMath::Multiply3x3(T,R,Temp);
	  (void) vtkMath::Multiply3x3(RT,Temp,D);

	  Dxx = D[0][0];
	  Dyy = D[1][1];
	  Dzz = D[2][2];
	  Dxy = D[0][1];
	  Dxz = D[0][2];
	  Dyz = D[1][2];
	  
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
