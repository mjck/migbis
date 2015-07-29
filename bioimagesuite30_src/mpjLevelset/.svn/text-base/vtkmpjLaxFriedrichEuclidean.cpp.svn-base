

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


#include "vtkmpjLaxFriedrichEuclidean.h"

#define VTK_MPJ_LARGE_FLOAT 32767

//------------------------------------------------------------------------------
vtkmpjLaxFriedrichEuclidean* vtkmpjLaxFriedrichEuclidean::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjLaxFriedrichEuclidean");
  if(ret) {
    return (vtkmpjLaxFriedrichEuclidean*)ret;
  }
  
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjLaxFriedrichEuclidean;
}

// Construct object with no children.
vtkmpjLaxFriedrichEuclidean::vtkmpjLaxFriedrichEuclidean()
{
  this->KnownPoints = NULL;
  this->KnownValues = 1;
  this->NumberOfIterations = 2;
  this->Speed = NULL;  
  this->label = NULL;
  this->Solution=NULL;
}

// ----------------------------------------------------------------------------
vtkmpjLaxFriedrichEuclidean::~vtkmpjLaxFriedrichEuclidean()
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
void vtkmpjLaxFriedrichEuclidean::SetSpeedImage(vtkImageData *speed)
{
  this->SetInput(speed);
}

// ----------------------------------------------------------------------------
void vtkmpjLaxFriedrichEuclidean::ExecuteInformation()
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
void vtkmpjLaxFriedrichEuclidean::Initialize(vtkImageData *input, vtkImageData *output)
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
void vtkmpjLaxFriedrichEuclidean::Iterate(vtkImageData *input, vtkImageData *output)
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
void vtkmpjLaxFriedrichEuclidean::SweepBoundaries(vtkImageData *input, vtkImageData *output)
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
void vtkmpjLaxFriedrichEuclidean::Sweep(vtkImageData *input, vtkImageData *output, int dir[3])
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
double vtkmpjLaxFriedrichEuclidean::SolveHp(int coord[3], int index, double p, double q, double r)
{
  double eps = 1E-12;
  double grad = p*p + q*q + r*r;
  double result = p/(sqrt(grad)+eps);
  return result;
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichEuclidean::SolveHq(int coord[3], int index, double p, double q, double r)
{
  double eps = 1E-12;
  double grad = p*p + q*q + r*r;
  double result = q/(sqrt(grad)+eps);
  return result;
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichEuclidean::SolveHr(int coord[3], int index, double p, double q, double r)
{
  double eps = 1E-12;
  double grad = p*p + q*q + r*r;
  double result = r/(sqrt(grad)+eps); 
  return result;
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichEuclidean::SolveH(int coord[3], int index, double p, double q, double r)
{
  // 3D Eikonal
  double result = sqrt(p*p + q*q + r*r);
  return result;
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichEuclidean::Update(vtkImageData *input, vtkImageData *output, int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  vtkDataArray *out=output->GetPointData()->GetScalars();
  vtkDataArray *in=input->GetPointData()->GetScalars();
  
  // Lax-Friedrichs Method
  double hx = 0.25; //(float) (dim[0]-2) / (dim[0]-1);
  double hy = 0.25; //(float) (dim[1]-2) / (dim[1]-1);
  double hz = 0.25; //(float) (dim[2]-2) / (dim[2]-1);

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

  double H = SolveH(coord, index, (pp+pm)/2.0, (qp+qm)/2.0, (rp+rm)/2.0);   
  double ox = SolveHp(coord, index, pmax, qmin, rmin);
  double oy = SolveHq(coord, index, pmin, qmax, rmin);
  double oz = SolveHr(coord, index, pmin, qmin, rmax);
  
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
void vtkmpjLaxFriedrichEuclidean::SimpleExecute(vtkImageData *input, vtkImageData *output)
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

  printf("initializing...\n");
  this->Initialize(input, output);

  this->UpdateProgress(0.01);
  
  for(int j=0; j<this->NumberOfIterations; j++) {
    printf("sweeping iteration %d\n", j);
    this->Iterate(input, output);
  }
  
  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
