

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
void vtkmpjImageCharacteristics::NormalizeTensor3x3(float T[3][3], float W[3], float V[3][3], float N[3][3])
{
  float R[3][3], S[3][3];
  float D[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

  // diagonalize tensor
  this->Jacobi3x3(T,W,V);

  // force eigenvalues be positive
  for (int i=0; i<3; i++) {
    if (W[i] < 0) {
      V[0][i] = -V[0][i];
      V[1][i] = -V[1][i];
      V[2][i] = -V[2][i];
      W[i] = -W[i];
    }
  }

  // make tensor unit size
  D[0][0] = 1.0; D[1][1] = W[1]/W[0]; D[2][2] = W[2]/W[0];

  // normalized tensor = V * D * V' 
  (void) vtkMath::Transpose3x3(V,R);
  (void) vtkMath::Multiply3x3(D,R,S);
  (void) vtkMath::Multiply3x3(V,S,N);
}


// ----------------------------------------------------------------------------
void vtkmpjImageCharacteristics::FindEllipsoidPoint(float n[3], float w[3], float T[3][3], float N[3][3], float V[3][3], float FA)
{
  float eps = 1E-12;

  double u, v, av;
  double a = this->CoefficientA;
  double b = this->CoefficientB;
  double c = this->CoefficientC;
  //double a = 3, b = 2, c = 1;
  double aux;// = (n[1]*b) / (n[0]*a);

  if ((n[0] == 0) && (n[1] == 0) && (n[2] == 0)) {
    w[0] = 0;
    w[1] = 0;
    w[2] = 0;
  }

  // calculate u
  //aux = (n[1]*b) / (n[0]*a+eps);
  // u = atan(aux);

  //if (n[0] <= 0) 
  //  u += M_PI; 
  //else if(n[1] < 0)
  //  u += 2*M_PI;
  
  //if (n[0] <= 0) u += M_PI;
  //if (n[0] > 0 && n[1] <= 0) u += 2*M_PI;
  //if (u < 0 || u>2*M_PI) printf("PROBLEM=%f!, n0=%f, n1=%f a=%f, b=%f\n",u,n[0],n[1],w[0],w[1]);
  
  //  aux = sqrt(n[0] / (cos(u)*b*c+eps));
  //  if ((aux < -1) || (aux > 1)) printf("v=%f\n",aux);
  //v = acos(aux);
  
  //if (u < M_PI) av = -v; else av = v;
  //if ((u > M_PI/2.) && (u < 3*M_PI/2.))
  //av = -v;
  //else av = v;

  //p[0] = a*cos(v)*cos(u);
  //p[1] = b*cos(v)*sin(u);
  //p[2] = c*sin(v);

  //printf("speed=%f\n",speed);
  double p=n[0], q=n[1], r=n[2];
  double FA2 = FA * FA;
  double grad = sqrt(p*p+q*q+r*r) + eps;
  double grad23 = pow(p*p + q*q + r*r, 3.0/2.0);
  double diff = (p*p*N[0][0]+q*q*N[1][1]+r*r*N[2][2]+2*p*q*N[0][1]+2*p*r*N[0][2]+2*q*r*N[1][2]);

  w[0] = -(FA2/grad23)*diff*p + (FA2/grad)*(2*p*N[0][0] + 2*q*N[0][1] + 2*r*N[0][2]);
  w[1] = -(FA2/grad23)*diff*q + (FA2/grad)*(2*p*N[0][1] + 2*q*N[1][1] + 2*r*N[1][2]);
  w[2] = -(FA2/grad23)*diff*r + (FA2/grad)*(2*p*N[0][2] + 2*q*N[1][2] + 2*r*N[2][2]);
  
  //p[0] = p[0] / sqrt(a*n[0]*n[0]+b*n[1]*n[1]+c*n[2]*n[2]);
  //p[1] = p[1] / sqrt(a*n[0]*n[0]+b*n[1]*n[1]+c*n[2]*n[2]);
  //p[2] = p[2] / sqrt(a*n[0]*n[0]+b*n[1]*n[1]+c*n[2]*n[2]);

  //if (n[2] < 0) p[2] = -p[2];

  //  printf("u=%f, v=%f, a=%f,%f,%f\n",u,v,a,b,c);
  //(void) vtkMath::Multiply3x3(T, p, a);
}

// ----------------------------------------------------------------------------
void vtkmpjImageCharacteristics::ComputeCharacteristics(vtkImageData *input, vtkImageData *output, int coord[3], int index)
{
  vtkDataArray *out=output->GetPointData()->GetScalars();
  vtkDataArray *in=input->GetPointData()->GetScalars();
  //vtkDataArray *tensor=this->Tensor->GetPointData()->GetScalars();
  vtkDataArray *gradient=this->Gradient->GetPointData()->GetScalars();

  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];

  // number of voxels in a row and slice
  int factor[3] = {1, dim[0], dim0Tdim1};

  float w[3], n[3], p[3];
  float T[3][3], V[3][3], N[3][3];
  float W[3];

  T[0][0] = in->GetComponent(index, 0);           /* Dxx */
  T[1][0] = T[0][1] = in->GetComponent(index, 1); /* Dxy */
  T[2][0] = T[0][2] = in->GetComponent(index, 2); /* Dxz */
  T[1][1] =  in->GetComponent(index, 4);          /* Dyy */
  T[2][1] = T[1][2] = in->GetComponent(index, 5); /* Dyz */
  T[2][2] = in->GetComponent(index, 8);           /* Dzz */

  // diagonalize and normalize tensor 
  this->NormalizeTensor3x3(T,W,V,N);
   
  // calculate FA
  float md = (W[0] + W[1] + W[2]) / 3.0;
  float a=W[0]-md;
  float b=W[1]-md;
  float c=W[2]-md;
  float FA = sqrt(3*(a*a + b*b + c*c)) / sqrt(2*(W[0]*W[0] + W[1]*W[1] + W[2]*W[2]));

  //D[0][0] = tensor->GetComponent(index, 0);           /* Dxx */
  //D[1][0] = D[0][1] = tensor->GetComponent(index, 1); /* Dxy */
  //D[2][0] = D[0][2] = tensor->GetComponent(index, 2); /* Dxz */
  //D[1][1] = tensor->GetComponent(index, 4);           /* Dyy */
  //D[2][1] = D[1][2] = tensor->GetComponent(index, 5); /* Dyz */
  //D[2][2] = tensor->GetComponent(index, 8);           /* Dzz */

  // diagonalize and normalize tensor 
  //this->DiagonalizeTensor3x3(D,w,V,T);

  //double speed = in->GetComponent(index, 0);

  // determin coordinates of point on the ellipsoid
  n[0]= gradient->GetComponent(index, 0);
  n[1]= gradient->GetComponent(index, 1);
  n[2]= gradient->GetComponent(index, 2);
  
  // normalize normal
  double nm = vtkMath::Normalize(n);
  
  this->FindEllipsoidPoint(n,p,T,N,V,FA);
  //  this->EllipsoidPoint(w,n,V,a);

  double nmp = vtkMath::Normalize(p);

  //printf("w=%f %f %f, n= %f %f %f char= %f %f %f\n",w[0],w[1],w[2],n[0],n[1],n[2],a[0],a[1],a[2]);
    
  out->SetComponent(index, 0, nm*p[0]);
  out->SetComponent(index, 1, nm*p[1]);
  out->SetComponent(index, 2, nm*p[2]);
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
