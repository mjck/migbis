

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


#include "vtkmpjImageCharacteristics.h"

//------------------------------------------------------------------------------
vtkmpjImageCharacteristics* vtkmpjImageCharacteristics::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageCharacteristics");
  if(ret)
    {
      return (vtkmpjImageCharacteristics*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageCharacteristics;
}

// Construct object with no children.
vtkmpjImageCharacteristics::vtkmpjImageCharacteristics()
{
  this->Mask = NULL;
  this->Tensor = NULL;
  this->Gradient = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjImageCharacteristics::~vtkmpjImageCharacteristics()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
  if (this->Tensor != NULL)
    this->Tensor->Delete();
  if (this->Gradient != NULL)
    this->Gradient->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageCharacteristics::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  int dim[3];  input->GetDimensions(dim);

  output->SetNumberOfScalarComponents(3);  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
int vtkmpjImageCharacteristics::InsideMask(int index)
{
  return (this->mask != NULL)?(this->mask->GetComponent(index, 0) > 0) : 1;
}

// ----------------------------------------------------------------------------
void vtkmpjImageCharacteristics::Jacobi3x3(float a[3][3], float w[3], float v[3][3])
{
  float **aa = new float *[3]; /* allocate space for tensor matrix */
  aa[0] = new float[3];
  aa[1] = new float[3];
  aa[2] = new float[3];
   
  float **vv = new float *[3]; /* allocate space for eigenvector matrix */
  vv[0] = new float[3];
  vv[1] = new float[3];
  vv[2] = new float[3];
  
  for(int i=0;i<3;i++)
    for(int j=0;j<3;j++) 
      aa[i][j] = a[i][j];

  // call the real method
  (void) vtkMath::Jacobi(aa,w,vv);

  for(int i=0;i<3;i++)
    for(int j=0;j<3;j++) 
      v[i][j] = vv[i][j];

  // free aa
  delete[] aa[0];
  delete[] aa[1];
  delete[] aa[2];
  delete[] aa;
   
  delete[] vv[0];
  delete[] vv[1];
  delete[] vv[2];
  delete[] vv;
}

// ----------------------------------------------------------------------------
void vtkmpjImageCharacteristics::DiagonalizeTensor3x3(float T[3][3], float w[3], float V[3][3], float N[3][3])
{
  float R[3][3], S[3][3];
  float D[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

  // diagonalize tensor
  this->Jacobi3x3(T,w,V);

  // force eigenvalues be positive
  for (int i=0; i<3; i++) {
    if (w[i] < 0) {
      V[0][i] = -V[0][i];
      V[1][i] = -V[1][i];
      V[2][i] = -V[2][i];
      w[i] = -w[i];
    }
  }

  // make tensor unit size
  D[0][0] = 1.0; D[1][1] = w[1]/w[0]; D[2][2] = w[2]/w[0];

  // normalized tensor = V * D * V' 
  (void) vtkMath::Transpose3x3(V,R);
  (void) vtkMath::Multiply3x3(D,R,S);
  (void) vtkMath::Multiply3x3(V,S,N);
}

// ----------------------------------------------------------------------------
void vtkmpjImageCharacteristics::FindEllipsoidPoint(float n[3], float a[3])
{
  float p[3];
  float eps = 1E-12;

  double u, v;
  double a = 3, b = 2, c = 1;
  double aux = (n[1]*b) / (n[0]*a);

  // calculate u
  aux = (n[]1*b) / (n[0]*a);
  u = atan(aux);

  if (n[0] < 0) 
    u += M_PI; 
  else if(n[1] < 0)
    u += 2*M_PI;
  
  //if (n[0] <= 0) u += M_PI;
  //if (n[0] > 0 && n[1] <= 0) u += 2*M_PI;
  //if (u < 0 || u>2*M_PI) printf("PROBLEM=%f!, n0=%f, n1=%f a=%f, b=%f\n",u,n[0],n[1],w[0],w[1]);
  
  v = acos(sqrt(n[0] / (cos(u)*b*c+eps)));
  if (u < M_PI) v = -v;
  
  a[0] = a*cos(v)*cos(u);
  a[1] = b*cos(v)*sin(u);
  a[2] = c*sin(v);

  printf("u=%f, v=%f, a=%f,%f,%f\n",u,v,a,b,c);
  //(void) vtkMath::Multiply3x3(T, p, a);
}

// ----------------------------------------------------------------------------
void vtkmpjImageCharacteristics::ComputeCharacteristics(vtkImageData *input, vtkImageData *output, int coord[3], int index)
{
  vtkDataArray *out=output->GetPointData()->GetScalars();
  vtkDataArray *in=output->GetPointData()->GetScalars();
  vtkDataArray *tensor=this->Tensor->GetPointData()->GetScalars();
  vtkDataArray *gradient=this->Gradient->GetPointData()->GetScalars();

  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];

  // number of voxels in a row and slice
  int factor[3] = {1, dim[0], dim0Tdim1};

  float w[3], n[3], a[3];
  float T[3][3], V[3][3], D[3][3];
    
  //D[0][0] = tensor->GetComponent(index, 0);           /* Dxx */
  //D[1][0] = D[0][1] = tensor->GetComponent(index, 1); /* Dxy */
  //D[2][0] = D[0][2] = tensor->GetComponent(index, 2); /* Dxz */
  //D[1][1] = tensor->GetComponent(index, 4);           /* Dyy */
  //D[2][1] = D[1][2] = tensor->GetComponent(index, 5); /* Dyz */
  //D[2][2] = tensor->GetComponent(index, 8);           /* Dzz */

  // diagonalize and normalize tensor 
  //this->DiagonalizeTensor3x3(D,w,V,T);

  // determine coordinates of point on the ellipsoid
  n[0]= gradient->GetComponent(index, 0);
  n[1]= gradient->GetComponent(index, 1);
  n[2]= gradient->GetComponent(index, 2);
  
  // normalize normal
  (void)vtkMath::Normalize3(n);
  
  this->FindEllipsoidPoint(n,a);
  //  this->EllipsoidPoint(w,n,V,a);
  
  //printf("w=%f %f %f, n= %f %f %f char= %f %f %f\n",w[0],w[1],w[2],n[0],n[1],n[2],a[0],a[1],a[2]);
    
  out->SetComponent(index, 0, a[0]);
  out->SetComponent(index, 1, a[1]);
  out->SetComponent(index, 2, a[2]);
}

// ----------------------------------------------------------------------------
void vtkmpjImageCharacteristics::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  int index=0;
    
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  // Get pointer to mask array
  this->mask = (this->Mask != NULL) ? this->Mask->GetPointData()->GetScalars() : NULL;
  
  for (int k=0;k<dim[2];k++) {    
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  int coord[3]={i, j, k};
	  ComputeCharacteristics(input, output, coord, index);
	  
	  ++index;	  
	}  	  
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }

  printf("Done with map!\n");

}
// ----------------------------------------------------------------------------
