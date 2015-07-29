

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


#include "vtkmpjFastMarchingVesselConnectivity.h"

#define VTK_MPJ_LARGE_DOUBLE 32767

#ifndef VTK_MPJ_SIGN
#define VTK_MPJ_SIGN(x) (x < 0 ? -1 : (x > 0 ? 1 : 0))
#endif

//------------------------------------------------------------------------------
vtkmpjFastMarchingVesselConnectivity* vtkmpjFastMarchingVesselConnectivity::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjFastMarchingVesselConnectivity");
  if(ret)
    {
      return (vtkmpjFastMarchingVesselConnectivity*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjFastMarchingVesselConnectivity;
}

// Construct object with no children.
vtkmpjFastMarchingVesselConnectivity::vtkmpjFastMarchingVesselConnectivity()
{ 
  this->Mask = NULL;
  this->KnownPoints = NULL;
  this->KnownValues = 1;
  this->LabelImage = NULL;
  this->FrontPoints = NULL;
  this->PropagationTime = VTK_MPJ_LARGE_DOUBLE;
  this->Spacing[0] = 1.0;
  this->Spacing[1] = 1.0;
  this->Spacing[2] = 1.0;
  this->queue = NULL;
  this->label = NULL;
  this->Infinity = 32767;
  this->MaxIntensity = 100;
}

// ----------------------------------------------------------------------------
vtkmpjFastMarchingVesselConnectivity::~vtkmpjFastMarchingVesselConnectivity()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
  if (this->KnownPoints != NULL)
    this->KnownPoints->Delete();
  if (this->LabelImage != NULL)
    this->LabelImage->Delete();
  if (this->FrontPoints != NULL)
    this->FrontPoints->Delete();
  if (this->queue != NULL)    
    this->queue->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingVesselConnectivity::SetSpeedImage(vtkImageData *speed)
{
  this->SetInput(speed);
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingVesselConnectivity::ExecuteInformation()
{
  vtkImageData *input = this->GetInput();
  vtkImageData *output = this->GetOutput();
   
  if (input==NULL) {
    vtkErrorMacro(<<"No Speed Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  
  int dim[3]; input->GetDimensions(dim);
  
  if (this->KnownPoints==NULL) {
    vtkErrorMacro(<<"Set of known points is empty!");
    return;
  }

  printf("dims=%d %d %d\n", dim[0],dim[1],dim[2]);

  output->SetNumberOfScalarComponents(1);  
  output->SetScalarType(VTK_DOUBLE);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
int vtkmpjFastMarchingVesselConnectivity::InMask(int index)
{
  return (this->mask != NULL)?(this->mask->GetComponent(index, 0) > 0) : 1;
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingVesselConnectivity::PrintMatrix3x3(char *title, double M[3][3])
{
  printf("%s",title);
  printf("%f %f %f\n",M[0][0],M[0][1],M[0][2]);
  printf("%f %f %f\n",M[1][0],M[1][1],M[1][2]);
  printf("%f %f %f\n",M[2][0],M[2][1],M[2][2]);
  printf("===============================\n");
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingVesselConnectivity::SortTerms(double term[3]) 
{
  double tmp[3];

  if (term[0] < term[1]) {
    if (term[0] < term[2]) {
      if (term[1] < term[2]) {
	tmp[0] = term[0]; tmp[1] = term[1]; tmp[2] = term[2]; 
      } else {
	tmp[0] = term[0]; tmp[1] = term[2]; tmp[2] = term[1]; 
      }
    } else 
      {
	tmp[0] = term[2]; tmp[1] = term[0]; tmp[2] = term[1];
      }
  } else {
    if (term[1] < term[2]) {
      if (term[0] < term[2]) {
	tmp[0] = term[1]; tmp[1] = term[0]; tmp[2] = term[2];
      } else {
	tmp[0] = term[1]; tmp[1] = term[2]; tmp[2] = term[0];
      }
    } else
      {
	tmp[0] = term[2]; tmp[1] = term[1]; tmp[2] = term[0];
      }    
  }

  term[0] = tmp[0];
  term[1] = tmp[1];
  term[2] = tmp[2];
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingVesselConnectivity::Solve2(double a, double b, double rhs)
{  
  double amb = a - b;
  if (fabs(amb) >= rhs) {
    return (a < b ? a : b) + rhs;
  } else {
    double amb2 = amb*amb;
    return (a+b+sqrt(2.0*rhs*rhs-amb2)) / 2.0;
  }
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingVesselConnectivity::SolveN(int number, double term[3], double rhs)
{
  double coeff[3] = {-rhs*rhs, 0.0, 0.0};

  for (int j=0; j<3; j++) {
    if (term[j] != (double)VTK_MPJ_LARGE_DOUBLE) {
      coeff[2] += 1.0;
      coeff[1] -= 2*term[j];
      coeff[0] += term[j]*term[j];
    }
  }
  
  int no_solutions = 1;
  double max_sol = (double)VTK_MPJ_LARGE_DOUBLE;
  double sol[2] = {(double)VTK_MPJ_LARGE_DOUBLE, (double)VTK_MPJ_LARGE_DOUBLE};
  double discr = coeff[1]*coeff[1] - 4.0*coeff[2]*coeff[0];
  
  if (discr < 0) {
    printf("Solution is not differentiable! terms=%f, %f, %f, coeffs=%f, %f, %f\n", term[0], term[1], term[2], coeff[2], coeff[1], coeff[0]);
  }
  
  // solve quadratic equation
  vtkMath::SolveQuadratic(coeff[2], coeff[1], coeff[0], &sol[0], &sol[1], &no_solutions);
  if (no_solutions == 2) {
    max_sol = sol[0]>=sol[1]?sol[0]:sol[1];
  } else {
    max_sol = sol[0];
  }
  
  return max_sol;
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingVesselConnectivity::SolveQuadratic(int number, double term[3], double rhs)
{  
  double result;
  
  switch(number) {
  case 1:
    result = term[0] + rhs;
    break;
  case 2:
    result = Solve2(term[0], term[1], rhs);
    break;
  case 3:
  default:
    if ((term[0] + rhs) > term[1]) {
      result = term[0] + rhs;      
    } else {
      result = Solve2(term[0], term[1], rhs);
    }
    if (result > term[2])
      result = SolveN(number, term, rhs);
    break;
  };
      
  return(result);
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingVesselConnectivity::ComputeNormal(vtkImageData *input, vtkImageData *output, 
					     int coord[3], int index, double result[3])
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  //vtkDataArray *in = input->GetPointData()->GetScalars(); 
  //vtkDataArray *out = output->GetPointData()->GetScalars();
  
  double normal[3];
  result[0]=normal[0]=0.0;
  result[1]=normal[1]=0.0;
  result[2]=normal[2]=0.0;
  
  // process each one of the 26 neighbors
  //int count = 0;
  for(int i=-1;i<2;i++)
    for(int j=-1;j<2;j++)
      for(int k=-1;k<2;k++) {
	if ((coord[0]+i)>=0 && ((coord[0]+i)<dim[0]) &&
	    (coord[1]+j)>=0 && ((coord[1]+j)<dim[1]) &&
	    (coord[2]+k)>=0 && ((coord[2]+k)<dim[2]))
	  {	  	    
	    int neighbor=index + i*factor[0] + j*factor[1] + k*factor[2];
	    if (this->InMask(neighbor) && this->label->GetComponent(neighbor, 0) == (double)VTK_MPJ_FROZEN) {
	      normal[0]+=i; normal[1]+=j; normal[2]+=k;	      
	    }
	  }	
      }
  
  result[0]=normal[0];
  result[1]=normal[1];
  result[2]=normal[2];
  
  (void) vtkMath::Normalize(result);  

  //printf("normal at %d is [%f,%f,%f], [%f,%f,%f]\n",index,result[0],result[1],result[2],normal[0],normal[1],normal[2]);
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingVesselConnectivity::Jacobi3x3(double a[3][3], double w[3], double v[3][3])
{
  double **aa = new double *[3]; /* allocate space for tensor matrix */
  aa[0] = new double[3];
  aa[1] = new double[3];
  aa[2] = new double[3];
   
  double **vv = new double *[3]; /* allocate space for eigenvector matrix */
  vv[0] = new double[3];
  vv[1] = new double[3];
  vv[2] = new double[3];
  
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
void vtkmpjFastMarchingVesselConnectivity::SortEigenvectors(double w[3], double V[3][3])
{
  int i, j, k;
  double tmp, tmp2;
  
  // sort eigenfunctions by decreasing magnitude
  for (j=0; j<3; j++)
    {
      k = j;
      tmp = w[k];
      tmp2 = fabs(tmp);
      for (i=j; i<3; i++)
        {
          if (fabs(w[i]) < tmp2)
            {
              k = i;
              tmp = w[k];
              tmp2 = fabs(tmp);
            }
        }
      if (k != j)
        {
          w[k] = w[j];
          w[j] = tmp;
          for (i=0; i<3; i++)
            {
              tmp = V[i][j];
              V[i][j] = V[i][k];
              V[i][k] = tmp;
            }
        }
    }
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingVesselConnectivity::NormalizeTensor3x3(double T[3][3], double W[3], double V[3][3], double N[3][3])
{
  double R[3][3], S[3][3];
  double D[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

  // diagonalize tensor
  vtkMath::Diagonalize3x3(T,W,V);
  //this->Jacobi3x3(T,W,V);
 
  // sort eigenvectors by decreasing absolute magnitude
  this->SortEigenvectors(W,V);
  
  // force eigenvalues be positive
  //for (int i=0; i<3; i++) {
  //if (W[i] < 0) {
  //  V[0][i] = -V[0][i];
  //  V[1][i] = -V[1][i];
  //  V[2][i] = -V[2][i];
  //  W[i] = -W[i];
  //}
  //}

  // make tensor unit size
  //D[0][0] = 1.0*VTK_MPJ_SIGN(W[0]); 
  //D[1][1] = fabs(W[1]/W[0])*VTK_MPJ_SIGN(W[1]);  
  //D[2][2] = fabs(W[2]/W[0])*VTK_MPJ_SIGN(W[2]);

  //printf("W=%f, %f, %f, D=%f, %f, %f\n",W[0],W[1],W[2],D[0][0],D[1][1],D[2][2]);

  // normalized tensor = V * D * V' 
  //(void) vtkMath::Transpose3x3(V,R);
  //(void) vtkMath::Multiply3x3(D,R,S);
  //(void) vtkMath::Multiply3x3(V,S,N);
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingVesselConnectivity::ComputeWeight(double value, double max, 
							   double steepness, double center)
{
  // compute weight
  return (1.0 / ( (1.0 / max) + exp(-steepness * (value - center))));
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingVesselConnectivity::ComputeSpeed(vtkImageData *input, vtkImageData *output, 
							  int parent, int coord[3], int index, 
							  double normal[3])
{
  vtkDataArray *in = input->GetPointData()->GetScalars();

  double result;
  
  double T[3][3], N[3][3];
  double V[3][3], V0[3];
  double W[3]; 

  double eps=1E-12;

  T[0][0] = in->GetComponent(index, 0);           /* Dxx */
  T[1][0] = T[0][1] = in->GetComponent(index, 1); /* Dxy */
  T[2][0] = T[0][2] = in->GetComponent(index, 2); /* Dxz */
  T[1][1] =  in->GetComponent(index, 3);          /* Dyy */
  T[2][1] = T[1][2] = in->GetComponent(index, 4); /* Dyz */
  T[2][2] = in->GetComponent(index, 5);           /* Dzz */

  // diagonalize and normalize tensor 
  this->NormalizeTensor3x3(T,W,V,N);
  
  // if eigenvalues are all zero, return 0
  if (fabs(W[0])<eps && fabs(W[1])<eps && fabs(W[2])<eps) {
    return 1E-6;
  }
  
  // get major eigenvector
  V0[0] = V[0][0]; 
  V0[1] = V[1][0]; 
  V0[2] = V[2][0];
  
  double p = normal[0];
  double q = normal[1];
  double r = normal[2];
  double grad = p*p + q*q + r*r;
  
  //if (W[2] == 0.0)
  //this->PrintMatrix3x3("N=",V);  

  //double diff = (p*p*T[0][0]+q*q*T[1][1]+r*r*T[2][2]+2*p*q*T[0][1]+2*p*r*T[0][2]+2*q*r*T[1][2]);  
  //double diff = (p*p*T[0][0]+q*q*T[1][1]+r*r*T[2][2]+2*p*q*T[0][1]+2*p*r*T[0][2]+2*q*r*T[1][2]);
  //double ndiff = (p*p*N[0][0]+q*q*N[1][1]+r*r*N[2][2]+2*p*q*N[0][1]+2*p*r*N[0][2]+2*q*r*N[1][2]);
  //  printf("W=%f, %f, %f; diff=%f, ndiff=%f, true=%f\n",W[0],W[1],W[2],diff,ndiff,1-fabs(ndiff));

  double diff = p*V0[0] + q*V0[1] + r*V0[2];

  //printf("diff=%f\n",diff);

  //if (fabs(diff) > 1e-12) 
  // diff = log(1.0 / fabs(diff));
  //else 
  //diff = this->Infinity;
   

  //printf("diff=%f\n",diff);

  //double diff = (p*p*N[0][0]+q*q*N[1][1]+r*r*N[2][2]+2*p*q*N[0][1]+2*p*r*N[0][2]+2*q*r*N[1][2]);
  //double ndiff = diff / (grad+eps);  
  //result = sqrt(grad) * FA*FA * exp(ndiff * ndiff);
  // latest:
  //result = FA * FA * (diff / sqrt(grad + eps));
  
  double Alpha = 0.5;
  double Beta = 0.5; 
  double Gamma = 0.25 * this->MaxIntensity;
  
  double Ra = fabs(W[1]/W[2]);
  double Rb = fabs(W[0]/sqrt(fabs(W[1]*W[2])));
  double Rs = sqrt(W[0]*W[0] + W[1]*W[1] + W[2]*W[2]);

  //printf("W[0]=%f, W[1]=%f, W[2]=%f, Ra=%f,Rb=%f,Rc=%f\n",W[0],W[1],W[2],Ra,Rb,Rs);
  
  double a2=-1.0/(Alpha*Alpha*2.0);
  double b2=-1.0/(Beta*Beta*2.0);
  double c2=-1.0/(Gamma*Gamma*2.0);
  
  double vessel;
  
  if (W[1] > eps || W[2] > eps) {
    vessel = 0.0;
  } else {  
    vessel = (1.0-exp(Ra*Ra*a2));
    vessel *= exp(Rb*Rb*b2);
    vessel *= (1.0-exp(Rs*Rs*c2));
  }
  
  //printf("vessel=%f,diff=%f\n",vessel,diff);

  double a = this->ComputeWeight(vessel, 1.0, 20.0, 0.4);
  double b = this->ComputeWeight(diff, 1.0, 20.0, 0.5);

  result = sqrt(grad) * a + b;

  //printf("result=%f, a=%f, b=%f, diff=%f, vessel=%f\n",result,a,b,diff,vessel);

  //result =  FA*FA*exp(result0*result0*result0);

  //result = FA * (1./(1.+exp(-10*(FA-0.5)))) * (result1*result1 + 0.1);
  //result = FA * (1./(1.+exp(-10*(FA-0.5)))) * exp(pow(result0,3));
  //result = exp(result0*result0);
  //result = (result1*result1);
  //result = result1;

  //result = FA*FA * exp(result*result);

  //result = tan(result)+1.0;  
  //else result=0.0;
  //if (FA >= 0.2)  
  //result = 1.0 / (1.0 - result*result);
  //else result = 0.0;
  
  //if (this->Debug) {
    //printf("direction=[%f,%f,%f],e1=[%f,%f,%f], result=%f\n",
    //   direction[0], direction[1], direction[2], V0[0], V0[1], V0[2], result);
  //}
  
  return result;
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingVesselConnectivity::ComputeFirstOrder(vtkImageData *input, vtkImageData *output,
						  int parent, int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  //vtkDataArray *in = input->GetPointData()->GetScalars(); 
  vtkDataArray *out = output->GetPointData()->GetScalars();
  
  // estimate normal
  double normal[3];
  this->ComputeNormal(input, output, coord, index, normal);

  // get speed value
  double F = this->ComputeSpeed(input, output, parent, coord, index, normal);
  //double F = 1.0;

  if (F == 0.0) {
    return this->Infinity;
  }
    
  int num_terms = 0;
  double direction[3];
  double term[3];
  int term_index[3];
  
  // process each one of the six neighbors
  for (int j=0; j<3; j++) { // axis  
    
    direction[0] = 0.0;
    direction[1] = 0.0;
    direction[2] = 0.0;
    
    term[j] = (double)VTK_MPJ_LARGE_DOUBLE;
    
    for (int i=-1; i<2; i+=2) { // direction 
      if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	
	direction[j] = (double)i;
	
	int neighbor=index + i*factor[j];
	
	// if value is frozen get its time
	if (this->InMask(neighbor) && this->label->GetComponent(neighbor, 0) == (double)VTK_MPJ_FROZEN) {
	  
	  double time = (double)out->GetComponent(neighbor, 0);
	  
	  if (time < term[j]) {
	    term[j] = time;
	    term_index[j] = neighbor;
	  }

	} // if
      } // if       
    } // for direction 
    

    // accumulate number of quadratic terms
    num_terms += (term[j] < (double)VTK_MPJ_LARGE_DOUBLE);
  
  } // for axis
  
  // sort terms
  this->SortTerms(term);

  double solution = this->Infinity;//VTK_MPJ_LARGE_DOUBLE;

  // solve quadratic for maximum solution
  solution = this->SolveQuadratic(num_terms, term, this->Spacing[0] / F);

  return (solution < this->Infinity ? solution : this->Infinity);
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingVesselConnectivity::ComputeTime(vtkImageData *input, vtkImageData *output,
					   int parent, int coord[3], int index) 
{
  // vtkDataArray *out=output->GetPointData()->GetScalars();

  // compute time of arrival at neighbor
  double time = this->ComputeFirstOrder(input, output, parent, coord, index);
  
  // insert neighbor in the narrow band
  if (this->label->GetComponent(index, 0) != (double)VTK_MPJ_NARROW_BAND) {
    this->label->SetComponent(index, 0, (double)VTK_MPJ_NARROW_BAND);
    this->queue->Insert(time, index);
  } else {
    // if already in the narrow band, store new time
    this->queue->DeleteId(index);
    this->queue->Insert(time, index);
  }

  return time;
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingVesselConnectivity::InitializeFront(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  vtkDataArray *out=output->GetPointData()->GetScalars();

  // Create priority queue
  this->queue = vtkmpjPriorityQueue::New();
  this->queue->Allocate(1000);

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
  this->label->FillComponent(0, (double)VTK_MPJ_UNVISITED);

  out->FillComponent(0, this->Infinity);
  
  int index, coord[3], ncoord[3];
  int dim0Tdim1 = dim[0]*dim[1];
  //int factor[3] = {1, dim[0], dim0Tdim1};
  
  // Freeze known points
  int num_known = this->KnownPoints->GetNumberOfTuples();
  for(int i=0; i<num_known; i++) {
    
    coord[0] = ncoord[0] = (int)this->KnownPoints->GetComponent(i, 0);
    coord[1] = ncoord[1] = (int)this->KnownPoints->GetComponent(i, 1);
    coord[2] = ncoord[2] = (int)this->KnownPoints->GetComponent(i, 2);
    double value = this->KnownPoints->GetComponent(i, 3);
    
    index = dim0Tdim1*coord[2] + dim[0]*coord[1] + coord[0];

    // Tag initial points as trial points
    this->label->SetComponent(index, 0, (double)VTK_MPJ_NARROW_BAND);   

    out->SetComponent(index, 0, this->KnownValues ? value : 0.0);              

    // Insert into narrow band queue
    this->queue->Insert(0, index);
   
    this->count++;
  }
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingVesselConnectivity::PropagateFront(vtkImageData *input, vtkImageData *output)
{
  double time;
  int index, coord[3], ncoord[3];
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  vtkDataArray *out=output->GetPointData()->GetScalars();

  while ((index = this->queue->Pop(time, 0)) != -1) {
    
    this->label->SetComponent(index, 0, (double)VTK_MPJ_FROZEN);    
    out->SetComponent(index, 0, (double)time);

    this->count++;
    if (this->count==this->tenth)
      {
	this->pog+=0.1;
	this->UpdateProgress(this->pog);
	this->count=0;
      }
    
    // stop if arrival time is greater or equal to max time
    if ((time >= VTK_MPJ_LARGE_DOUBLE) || 
	((this->PropagationTime < (double)VTK_MPJ_LARGE_DOUBLE) && (time >= this->PropagationTime))) {
      continue;
    }

    // decompose index into x, y, z coordinates
    coord[2] = ncoord[2] = index / dim0Tdim1; 
    int r = index % dim0Tdim1;
    coord[1] = ncoord[1] = r / dim[0];
    coord[0] = ncoord[0] = r % dim[0];
    
    // process each one of the six neighbors
    for (int j=0; j<3; j++)  // axis  
      for (int i=-1; i<2; i+=2)  { // direction 
	
	if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	  int neighbor=index + i*factor[j];
	  
	  if (this->InMask(neighbor) && this->label->GetComponent(neighbor, 0) != (double)VTK_MPJ_FROZEN) { 
	   
	    ncoord[j] = coord[j] + i;   // update coordinate
	    this->ComputeTime(input, output, index, ncoord, neighbor);
	    ncoord[j] = coord[j]; // reset
	  }	  
	}
      }
    } // while  
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingVesselConnectivity::SaveFrontPoints(vtkImageData *input)
{
  int total = this->queue->GetNumberOfItems();
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int index, coord[3];
  double value;
  
  if (this->FrontPoints != NULL) 
    this->FrontPoints->Delete();
  
  this->FrontPoints = vtkDoubleArray::New(); 
  this->FrontPoints->SetNumberOfComponents(4);
  
  for(int j=0; j<total; j++) {
    
    // get point index
    index = queue->Peek(value, j);
    
    // decompose index
    coord[2] = index / dim0Tdim1; 
    int r = index % dim0Tdim1;
    coord[1] = r / dim[0];
    coord[0] = r % dim[0];
    
    // insert point
    this->FrontPoints->InsertNextTuple4((double)coord[0], (double)coord[1], (double)coord[2], 
					(double)value);
  }
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingVesselConnectivity::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  
  if (this->GetInput()==NULL) {
    vtkErrorMacro(<<"No Speed Image\n Exiting\n");
    return;
  }

  symmetric = (input->GetNumberOfScalarComponents() == 6);

  int total = input->GetPointData()->GetScalars()->GetNumberOfTuples();  
  this->pog = 0.0;  
  this->tenth = (int)(total / 10.0);
  this->count = 0;
  
  this->UpdateProgress(0.01);
  
  // Get pointer to mask array
  this->mask = (this->Mask != NULL) ? this->Mask->GetPointData()->GetScalars() : NULL;

  // initialize
  this->InitializeFront(input, output);  
  
  // propagate front
  this->PropagateFront(input, output);

  // save front points
  this->SaveFrontPoints(input);

  this->UpdateProgress(1.0);
}

// ----------------------------------------------------------------------------
