

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


#include "vtkmpjLaxFriedrichSweepingMethod.h"

#define VTK_MPJ_LARGE_FLOAT 32767

//------------------------------------------------------------------------------
vtkmpjLaxFriedrichSweepingMethod* vtkmpjLaxFriedrichSweepingMethod::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjLaxFriedrichSweepingMethod");
  if(ret) {
    return (vtkmpjLaxFriedrichSweepingMethod*)ret;
  }
  
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjLaxFriedrichSweepingMethod;
}

// Construct object with no children.
vtkmpjLaxFriedrichSweepingMethod::vtkmpjLaxFriedrichSweepingMethod()
{
  this->SecondOrder = 0;
  this->KnownPoints = NULL;
  this->KnownValues = 1;
  this->NumberOfIterations = 2;
  this->Speed = NULL;  
  this->label = NULL;
  this->Solution=NULL;
}

// ----------------------------------------------------------------------------
vtkmpjLaxFriedrichSweepingMethod::~vtkmpjLaxFriedrichSweepingMethod()
{
  if (this->KnownPoints != NULL)
    this->KnownPoints->Delete();
  if (this->LabelImage != NULL)
    this->LabelImage->Delete();
  if (this->ConvergenceImage != NULL) 
    this->ConvergenceImage->Delete();
  if (this->Solution != NULL)
    this->Solution->Delete();
}

// ----------------------------------------------------------------------------
void PrintMatrix3x3(char *title, double *M[3])
{
  printf("%s",title);
  printf("%f %f %f\n",M[0][0],M[0][1],M[0][2]);
  printf("%f %f %f\n",M[1][0],M[1][1],M[1][2]);
  printf("%f %f %f\n",M[2][0],M[2][1],M[2][2]);
  printf("===============================\n");
}

// ----------------------------------------------------------------------------
void vtkmpjLaxFriedrichSweepingMethod::Jacobi3x3(float a[3][3], float w[3], float v[3][3])
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
void vtkmpjLaxFriedrichSweepingMethod::NormalizeTensor3x3(float T[3][3], float W[3], float V[3][3], float N[3][3])
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
void vtkmpjLaxFriedrichSweepingMethod::SetSpeedImage(vtkImageData *speed)
{
  this->SetInput(speed);
}

// ----------------------------------------------------------------------------
void vtkmpjLaxFriedrichSweepingMethod::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  int dim[3];  input->GetDimensions(dim);
   
  if (this->KnownPoints==NULL) {
    vtkErrorMacro(<<"Set of known points is empty!");
    return;
  }

  output->SetNumberOfScalarComponents(1);  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjLaxFriedrichSweepingMethod::Initialize(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  double spc[3]; input->GetSpacing(spc);
  vtkDataArray *in=input->GetPointData()->GetScalars();
  vtkDataArray *out=output->GetPointData()->GetScalars();

  vtkDataArray *sol = NULL;
  if (this->Solution != NULL) {
    sol = this->Solution->GetPointData()->GetScalars();
  }

  // Create label image
  this->LabelImage = vtkImageData::New();
  this->LabelImage->SetSpacing(input->GetSpacing());
  this->LabelImage->SetOrigin(input->GetOrigin());
  this->LabelImage->SetDimensions(input->GetDimensions());
  this->LabelImage->SetScalarType(VTK_SHORT);
  this->LabelImage->SetNumberOfScalarComponents(1);
  this->LabelImage->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  this->LabelImage->AllocateScalars();
  this->label = this->LabelImage->GetPointData()->GetScalars();
  this->label->FillComponent(0, VTK_MPJ_UNKNOWN);

  // Create convergence image
  this->ConvergenceImage = vtkImageData::New();
  this->ConvergenceImage->SetSpacing(input->GetSpacing());
  this->ConvergenceImage->SetOrigin(input->GetOrigin());
  this->ConvergenceImage->SetDimensions(input->GetDimensions());
  this->ConvergenceImage->SetScalarType(VTK_FLOAT);
  this->ConvergenceImage->SetNumberOfScalarComponents(1);
  this->ConvergenceImage->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  this->ConvergenceImage->AllocateScalars();
  this->convergence = this->ConvergenceImage->GetPointData()->GetScalars();
  this->convergence->FillComponent(0, VTK_MPJ_LARGE_FLOAT);

  // Initialize values to infinity
  if (this->Solution == NULL)
    out->FillComponent(0, VTK_MPJ_LARGE_FLOAT);
  else {
    int numtuples = sol->GetNumberOfTuples();
    for(int i=0; i<numtuples; i++)
      out->SetComponent(i, 0, sol->GetComponent(i, 0));
  }
  
  int coord[3]; 
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  
  // Set initial conditions
  for(int i=0; i<this->KnownPoints->GetNumberOfTuples(); i++) {
    
    coord[0] = (int)this->KnownPoints->GetComponent(i, 0);
    coord[1] = (int)this->KnownPoints->GetComponent(i, 1);
    coord[2] = (int)this->KnownPoints->GetComponent(i, 2);
    float value = this->KnownValues ? this->KnownPoints->GetComponent(i, 3) : 0.0;
    
    int index = dim0Tdim1*coord[2] + dim[0]*coord[1] + coord[0];
    
    this->label->SetComponent(index, 0, VTK_MPJ_KNOWN);  // Freeze voxel    
    this->convergence->SetComponent(index, 0, 0.0);      // Boundary point has exact value
    out->SetComponent(index, 0, (float)value);           // Set arrival time
  }
}

// ----------------------------------------------------------------------------
void vtkmpjLaxFriedrichSweepingMethod::Iterate(vtkImageData *input, vtkImageData *output)
{
  for(int s1=-1; s1<2; s1+=2)
    for(int s2=-1; s2<2; s2+=2)
      for(int s3=-1; s3<2; s3+=2) {

	int dir[3] = {s1, s2, s3};

	//printf("sweeping at direction: %d %d %d\n",s1,s2,s3);
	
	this->Sweep(input, output, dir);
	this->SweepBoundaries(input, output);
	
	this->count++;
	if (this->count==this->tenth)
	  {
	    this->pog+=0.1;
	    this->UpdateProgress(this->pog);
	    this->count=0;
	  }
      }
}

// ----------------------------------------------------------------------------
void vtkmpjLaxFriedrichSweepingMethod::SweepBoundaries(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  double spc[3]; input->GetSpacing(spc);
  int dim0Tdim1 = dim[0]*dim[1];
  vtkDataArray *out=output->GetPointData()->GetScalars();

  int index;
  float val0, val1, val2, result;

  // rows
  for(int k=0; k<dim[2]; k++)  // for all slices
    for(int i=0; i<dim[0]; i++)  // for all columns in this row
      {
	// top row
	index = dim0Tdim1*k + i;
	val0 = out->GetComponent(index, 0);
	val1 = out->GetComponent(index + dim[0]*1, 0);
	val2 = out->GetComponent(index + dim[0]*2, 0);
	result = 2.0*val1 - val2;
	if (result <= val2) 
	  result = val2;
	if (result < val0) {
	  out->SetComponent(index, 0, result);
	  this->convergence->SetComponent(index, 0, fabs(result - val0));
	} else {
	  this->convergence->SetComponent(index, 0, 0.0);
	}

	// bottom row
	index = dim0Tdim1*k + dim[0]*(dim[1]-1) + i;
	val0 = out->GetComponent(index, 0);
	val1 = out->GetComponent(index - dim[0]*1, 0);
	val2 = out->GetComponent(index - dim[0]*2, 0);
	result = 2.0*val1 - val2;
	if (result <= val2) 
	  result = val2;
	if (result < val0) {
	  out->SetComponent(index, 0, result);
	  this->convergence->SetComponent(index, 0, fabs(result - val0));
	} else {
	  this->convergence->SetComponent(index, 0, 0.0);
	}
      }

  // for columns
  for(int k=0; k<dim[2]; k++)  // for all slices
    for(int j=0; j<dim[1]; j++)  // for all rows
      {
	// left column
	index = dim0Tdim1*k + dim[0]*j;
	val0 = out->GetComponent(index, 0);
	val1 = out->GetComponent(index + 1, 0);
	val2 = out->GetComponent(index + 2, 0);
	result = 2.0*val1 - val2;
	if (result <= val2) 
	  result = val2;
	if (result < val0) {
	  out->SetComponent(index, 0, result);
	  this->convergence->SetComponent(index, 0, fabs(result - val0));
	} else {
	  this->convergence->SetComponent(index, 0, 0.0);
	}

	// right column
	index = dim0Tdim1*k + dim[0]*j + dim[0]-1;
	val0 = out->GetComponent(index, 0);
	val1 = out->GetComponent(index - 1, 0);
	val2 = out->GetComponent(index - 2, 0);
	result = 2.0*val1 - val2;
	if (result <= val2) 
	  result = val2;
	if (result < val0) {
	  out->SetComponent(index, 0, result);
	  this->convergence->SetComponent(index, 0, fabs(result - val0));
	} else {
	  this->convergence->SetComponent(index, 0, 0.0);
	}
      }
  
  // for slices
  for(int i=0; i<dim[0]; i++)  // for all columns
    for(int j=0; j<dim[1]; j++) // for all rows      
      { 
	// first slice
	index = dim[0]*j + i;
	val0 = out->GetComponent(index, 0);
	val1 = out->GetComponent(index + dim0Tdim1*1, 0);
	val2 = out->GetComponent(index + dim0Tdim1*2, 0);
	result = 2.0*val1 - val2;
	if (result <= val2) 
	  result = val2;
	if (result < val0) {
	  out->SetComponent(index, 0, result);
	  this->convergence->SetComponent(index, 0, fabs(result - val0));
	} else {
	  this->convergence->SetComponent(index, 0, 0.0);
	}
	
	// last slice
	index = dim0Tdim1*(dim[2]-1) + dim[0]*j + i;
	val0 = out->GetComponent(index, 0);
	val1 = out->GetComponent(index - dim0Tdim1*1, 0);
	val2 = out->GetComponent(index - dim0Tdim1*2, 0);
	result = 2.0*val1 - val2;
	if (result <= val2) 
	  result = val2;
	if (result < val0) {
	  out->SetComponent(index, 0, result);
	  this->convergence->SetComponent(index, 0, fabs(result - val0));
	} else 
	  this->convergence->SetComponent(index, 0, 0.0);
					  
      }  
}

// ----------------------------------------------------------------------------
void vtkmpjLaxFriedrichSweepingMethod::Sweep(vtkImageData *input, vtkImageData *output, int dir[3])
{
  int dim[3]; input->GetDimensions(dim);
  double spc[3]; input->GetSpacing(spc);
  int dim0Tdim1 = dim[0]*dim[1];
  vtkDataArray *out=output->GetPointData()->GetScalars();

  // Sweep image
  for(int k=(dir[2]<0?dim[2]-2:1);(dir[2]<0?k>0:k<dim[2]-1);k+=dir[2])     
    for(int j=(dir[1]<0?dim[1]-2:1);(dir[1]<0?j>0:j<dim[1]-1);j+=dir[1])
      for(int i=(dir[0]<0?dim[0]-2:1);(dir[0]<0?i>0:i<dim[0]-1);i+=dir[0]) {
	
	int index = dim0Tdim1*k + dim[0]*j + i;
	int coord[3] = {i, j, k};
	
	if (this->label->GetComponent(index, 0) == VTK_MPJ_UNKNOWN) { 
	  
	  double new_value = this->Update(input, output, coord, index);

	  
	  if (new_value < out->GetComponent(index, 0)) {
	    
	    // compute convergence 
	    double old_value = out->GetComponent(index, 0);
	    this->convergence->SetComponent(index, 0, fabs(new_value - old_value));
	    
	    out->SetComponent(index, 0, (float)new_value);	    

	  } else 
	    this->convergence->SetComponent(index, 0, 0.0);
	}	
      }   
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichSweepingMethod::SolveHp(int coord[3], int index, double p, double q, double r, float N[3][3], float V[3][3], float FA)
{
  double eps = 1E-12;

  double grad = p*p + q*q + r*r;
  double prod = (p*V[0][0] + q*V[0][1] + r*V[0][2]) / (sqrt(grad)+eps);
  double diff = (p*p*N[0][0] + q*q*N[1][1] + r*r*N[2][2] + 2*p*q*N[0][1] + 2*p*r*N[0][2] + 2*q*r*N[1][2]);
  double ndiff = diff / (grad+eps);
  double e = exp(ndiff * ndiff * ndiff);
  
  double rest = (grad * (2*N[0][0]*p + 2*N[0][1]*q + 2*N[0][2]*r) - (2*p * diff)) / ((grad * grad)+eps);
  double term = e * 3.0 * ndiff * ndiff * rest;

  double result = FA*FA*exp(prod*prod*prod);//*(3*V[0][0]*prod*prod/grad); // + 3*p*prod*prod*prod/pow(grad,2));
  //double result = FA*FA * ((sqrt(grad)*term));// + e*(p/(sqrt(grad)+eps)));
  //  double result =  (FA*FA*exp(pow(ndiff,15)));// + e*(p/(sqrt(grad)+eps)));
  //double f = (p*p) / sqrt(grad);
  //double g = sqrt(grad);
  //double df = (sqrt(grad)*2*p - p*p*p)/(grad+eps);
  //double dg = p/(sqrt(grad)+eps);
  //  double result = p*(3*q*q + 3*r*r + 2*p*p) / (pow(p*p+q*q+r*r, 1.5)+eps);
    
  return result;
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichSweepingMethod::SolveHq(int coord[3], int index, double p, double q, double r, float N[3][3], float V[3][3], float FA)
{
   double eps = 1E-12;
   double grad = p*p + q*q + r*r;
   double prod = (p*V[0][0] + q*V[0][1] + r*V[0][2]) / (sqrt(grad)+eps);
   
   double diff = (p*p*N[0][0] + q*q*N[1][1] + r*r*N[2][2] + 2*p*q*N[0][1] + 2*p*r*N[0][2] + 2*q*r*N[1][2]);
   double ndiff = diff / (grad+eps);
   double e = exp(ndiff * ndiff * ndiff);

   double rest = (grad * (2*N[1][1]*q + 2*N[0][1]*p+ 2*N[1][2]*r) - (2*q * diff)) / ((grad * grad)+eps);
   double term = e * 3.0 * ndiff * ndiff * rest;
   
   //   double result = FA*FA * ((sqrt(grad)*term));// + e*(q/(sqrt(grad)+eps)));
   //   double result = (FA*FA*exp(pow(ndiff,15)));
   double result = FA*FA*exp(prod*prod*prod);//*(3*V[0][1]*prod*prod/grad); // + 3*q*prod*prod*prod/pow(grad,2));

   //double f = (p*p) / sqrt(grad);
   //double g = sqrt(grad);
   //double df = (sqrt(grad)*2*p - p*p*q/(grad+eps));
   //double dg = q/(sqrt(grad)+eps);
   //double result = q*(q*q + r*r) / (pow(p*p+q*q+r*r, 1.5)+eps);

   return result;
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichSweepingMethod::SolveHr(int coord[3], int index, double p, double q, double r, float N[3][3], float V[3][3], float FA)
{
  double eps = 1E-12;
  double grad = p*p + q*q + r*r;
  double prod = (p*V[0][0] + q*V[0][1] + r*V[0][2]) / (sqrt(grad)+eps);
  double diff = (p*p*N[0][0] + q*q*N[1][1] + r*r*N[2][2] + 2*p*q*N[0][1] + 2*p*r*N[0][2] + 2*q*r*N[1][2]);
  double ndiff = diff / (grad+eps);
  double e = exp(ndiff * ndiff * ndiff);

  double rest = (grad * (2*N[2][2]*r + 2*N[0][2]*p + 2*N[1][2]*q) - (2*r * diff)) / ((grad * grad)+eps);
  double term = e * 3.0 * ndiff * ndiff * rest;
  
  //  double result = FA*FA * ((sqrt(grad)*term));// + e*(r/(sqrt(grad)+eps)));
  //double result =  (FA*FA*exp(pow(ndiff,15)));
  
  double result = FA*FA*exp(prod*prod*prod);//*(3*V[0][2]*prod*prod/grad);// + 3*r*prod*prod*prod/pow(grad,2));

  //double f = (p*p) / sqrt(grad);
  //double g = sqrt(grad);
  //double df = (sqrt(grad)*2*p - p*p*r/(grad+eps));
  //double dg = r/(sqrt(grad)+eps);
  //double result = g*df + f*dg;
  //double result = r*(r*r + q*q) / (pow(p*p+q*q+r*r, 1.5) + eps);
 
  return result;
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichSweepingMethod::SolveH(int coord[3], int index, double p, double q, double r, float N[3][3], float V[3][3], float FA)
{
  double eps = 1E-12;
  double grad = p*p + q*q + r*r;
  double prod = (p*V[0][0] + q*V[0][1] + r*V[0][2]) / (sqrt(grad)+eps);
  double diff = (p*p*N[0][0] + q*q*N[1][1] + r*r*N[2][2] + 2*p*q*N[0][1] + 2*p*r*N[0][2] + 2*q*r*N[1][2]);
  double ndiff = diff / (grad+eps);
  //double result = sqrt(grad) * FA*FA * exp(ndiff * ndiff *ndiff);
  double result = FA*FA * sqrt(grad) * exp(prod * prod * prod);

  //  double result = (p*p / sqrt(grad)) + sqrt(grad);

  //double dot = (V0[0]*p + V0[1]*q + V0[2]*r) / norm;
  //double result = norm * exp(dot * dot);
  //  double result = norm + ((dot * dot) / norm);
  //  double result = norm * fabs(dot) / norm;  
  // 3-d eikonal
  //double result = sqrt(p*p + q*q + r*r);  
  // 3-d full tensor
  //double a=1, b=1, c=1, d=1, e=1, f=1;
  // double result = (a*p*p + b*q*q + c*r*r + 2*d*p*q + 2*e*p*r + 2*f*q*r) / norm;  
  //double result = (sqrt(p*p + q*q + r*r) + 2*fabs(r))*(1 + fabs(2*p*q) / (p*p + q*q));    
  //if (coord[0]==15 && coord[1]==16 && coord[2]==16) {
  //printf("%d %d %d ",coord[0],coord[1],coord[2]);
  // printf("%f %f %f = %f\n",p,q,r,result);
  //}
  return result;
}

int cmpfunc(const void *a, const void *b)
{
  double *x=(double *)a;
  double *y=(double *)b;
  if (*x < *y) return -1;
  if (*x == *y) return 0; else return 1;
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichSweepingMethod::Update(vtkImageData *input, vtkImageData *output, int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  vtkDataArray *out=output->GetPointData()->GetScalars();
  vtkDataArray *in=input->GetPointData()->GetScalars();
  
  float T[3][3], N[3][3], V[3][3];
  float W[3], R[3];
  float FA;

  // compute tensor
  T[0][0] = in->GetComponent(index, 0);           /* Dxx */
  T[1][0] = T[0][1] = in->GetComponent(index, 1); /* Dxy */
  T[2][0] = T[0][2] = in->GetComponent(index, 2); /* Dxz */
  T[1][1] = in->GetComponent(index, 4);           /* Dyy */
  T[2][1] = T[1][2] = in->GetComponent(index, 5); /* Dyz */
  T[2][2] = in->GetComponent(index, 8);           /* Dzz */
  
  // diagonalize and normalize tensor 
  this->NormalizeTensor3x3(T,W,V,N);

  // calculate FA
  float md = (W[0] + W[1] + W[2]) / 3.0;
  float a=W[0]-md;
  float b=W[0]-md;
  float c=W[1]-md;
  FA = sqrt(3*(a*a + b*b + c*c)) / sqrt(2*(W[0]*W[0] + W[1]*W[1] + W[2]*W[2]));

  // Lax-Friedrichs method
  double hx = 1.0; //(float) (dim[0]-2) / (dim[0]-1);
  double hy = 1.0; //(float) (dim[1]-2) / (dim[1]-1);
  double hz = 1.0; //(float) (dim[2]-2) / (dim[2]-1);

  double pp, pm, qp, qm, rp, rm;
  double phi_p, phi_q, phi_r;
  int neighbor[3];

  // for p
  neighbor[0] = index - factor[0];
  neighbor[1] = index; 
  neighbor[2] = index + factor[0];
  
  pp = ((double)out->GetComponent(neighbor[2], 0) - (double)out->GetComponent(neighbor[1], 0)) / hx;
  pm = ((double)out->GetComponent(neighbor[1], 0) - (double)out->GetComponent(neighbor[0], 0)) / hx;
  phi_p = ((double)out->GetComponent(neighbor[0], 0) + (double)out->GetComponent(neighbor[2], 0)) / hx;

  // for q
  neighbor[0] = index - factor[1];
  neighbor[1] = index; 
  neighbor[2] = index + factor[1];
  
  qp = ((double)out->GetComponent(neighbor[2], 0) - (double)out->GetComponent(neighbor[1], 0)) / hy;
  qm = ((double)out->GetComponent(neighbor[1], 0) - (double)out->GetComponent(neighbor[0], 0)) / hy;
  phi_q = ((double)out->GetComponent(neighbor[0], 0) + (double)out->GetComponent(neighbor[2], 0)) /hy;
  
  // for r
  neighbor[0] = index - factor[2];
  neighbor[1] = index; 
  neighbor[2] = index + factor[2];
  
  rp = ((double)out->GetComponent(neighbor[2], 0) - (double)out->GetComponent(neighbor[1], 0)) / hz;
  rm = ((double)out->GetComponent(neighbor[1], 0) - (double)out->GetComponent(neighbor[0], 0)) / hz;
  phi_r = ((double)out->GetComponent(neighbor[0], 0) + (double)out->GetComponent(neighbor[2], 0)) /hz;

  double pmin, pmax;
  if (fabs(pp) >= fabs(pm)) {
    pmax = fabs(pp); pmin = fabs(pm);
  } else {
    pmax = fabs(pm); pmin = fabs(pp);
  }

  double qmin, qmax;
  if (fabs(qp) >= fabs(qm)) {
    qmax = fabs(qp); qmin = fabs(qm);
  } else {
    qmax = fabs(qm); qmin = fabs(qp);
  }
  
  double rmin, rmax;
  if (fabs(rp) >= fabs(rm)) {
    rmax = fabs(rp); rmin = fabs(rm);
  } else {
    rmax = fabs(rm); rmin = fabs(rp);
  }

  double H = SolveH(coord, index, (pp+pm)/2.0, (qp+qm)/2.0, (rp+rm)/2.0, N, V, FA); 

  double ox = SolveHp(coord, index, pmax, qmin, rmin, N, V, FA);
  double oy = SolveHq(coord, index, pmin, qmax, rmin, N, V, FA);
  double oz = SolveHr(coord, index, pmin, qmin, rmax, N, V, FA);

  double visc = 1.0 / (ox/hx + oy/hy + oz/hz);
  double phi = visc * ( 1.0 - H + (ox/2)*phi_p + (oy/2)*phi_q + (oz/2)*phi_r );

  //printf("pm=%f,pp=%f,qm=%f,qp=%f,rm=%f,rp=%f\n",pm,pp,qm,qp,rm,rp);
  //if (coord[0]==15 && coord[1]==16 && coord[2]==16) {
  //printf("H=%f c=%f, ox=%f, oy=%f, oz=%f, hx=%f, hy=%f, hz=%f, phi=%f\n",H,c,ox,oy,oz,hx,hy,hz,phi);
  //}

  //printf("phi=%f\n",phi);
  return phi;
}

// ----------------------------------------------------------------------------
void vtkmpjLaxFriedrichSweepingMethod::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int dim[3];  input->GetDimensions(dim);
  double spc[3]; input->GetSpacing(spc);
  
  if (this->GetInput()==NULL) {
    vtkErrorMacro(<<"No Input Image\n Exiting\n");
    return;
  }
  
  this->pog = 0.0;  
  this->tenth=(int)((8*this->NumberOfIterations) / 8.0);
  this->count=0;

  printf("before init\n");
  this->Initialize(input, output);

  printf("after init\n");
  this->UpdateProgress(0.01);
  
  for(int j=0; j<this->NumberOfIterations; j++) {
    printf("iteration %d\n", j);
    this->Iterate(input, output);
  }
  
  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
