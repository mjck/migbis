

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


#include "vtkmpjLaxFriedrichSweeping.h"

#define VTK_MPJ_LARGE_FLOAT 32767

//------------------------------------------------------------------------------
vtkmpjLaxFriedrichSweeping* vtkmpjLaxFriedrichSweeping::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjLaxFriedrichSweeping");
  if(ret) {
    return (vtkmpjLaxFriedrichSweeping*)ret;
  }
  
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjLaxFriedrichSweeping;
}

// Construct object with no children.
vtkmpjLaxFriedrichSweeping::vtkmpjLaxFriedrichSweeping()
{
  this->KnownPoints = NULL;
  this->KnownValues = 1;
  this->NumberOfIterations = 2;
  this->SymmetricTensor = 1;
  this->label = NULL;
  this->LabelImage = NULL;
  this->Solution = NULL;
  this->ConvergenceImage = NULL;
  this->convergence = NULL;
  this->Gradient = NULL;
  this->gradient = NULL;
  this->ErrorValue = 0.0;
  this->MaximumError = 1.0;
  this->StopOnMaximumError = 0;
  this->IterationNumber = 0;
  this->Spacing[0] = 1.0;
  this->Spacing[1] = 1.0;
  this->Spacing[2] = 1.0;
}

// ----------------------------------------------------------------------------
vtkmpjLaxFriedrichSweeping::~vtkmpjLaxFriedrichSweeping()
{
  if (this->KnownPoints != NULL)
    this->KnownPoints->Delete();
  if (this->LabelImage != NULL)
    this->LabelImage->Delete();
  if (this->ConvergenceImage != NULL) 
    this->ConvergenceImage->Delete();
  if (this->Solution != NULL)
    this->Solution->Delete();
  if (this->Gradient != NULL) 
    this->Gradient->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjLaxFriedrichSweeping::ExecuteInformation()
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
void vtkmpjLaxFriedrichSweeping::Initialize(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  double spc[3]; input->GetSpacing(spc);
  vtkDataArray *in=input->GetPointData()->GetScalars();
  vtkDataArray *out=output->GetPointData()->GetScalars();
  
  // Create label image
  if (this->LabelImage == NULL) {
    printf("initializing label...\n");
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
  } else {
    this->label = this->LabelImage->GetPointData()->GetScalars();    
  }
  
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
  this->convergence->FillComponent(0, 0);
  
  // Create gradient image
  this->Gradient = vtkImageData::New();
  this->Gradient->SetSpacing(input->GetSpacing());
  this->Gradient->SetOrigin(input->GetOrigin());
  this->Gradient->SetDimensions(input->GetDimensions());
  this->Gradient->SetScalarType(VTK_FLOAT);
  this->Gradient->SetNumberOfScalarComponents(3);
  this->Gradient->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  this->Gradient->AllocateScalars();
  this->gradient = this->Gradient->GetPointData()->GetScalars();
  this->gradient->FillComponent(0, 0); 
  this->gradient->FillComponent(1, 0);
  this->gradient->FillComponent(2, 0);

  // Initialize values to infinity if initial solution is not given
  if (this->Solution == NULL) {
    out->FillComponent(0, VTK_MPJ_LARGE_FLOAT);
  } else {
    // otherwise initialize with given values
    vtkDataArray *sol = this->Solution->GetPointData()->GetScalars();
    int numtuples = sol->GetNumberOfTuples();
    for(int i=0; i<numtuples; i++)
      out->SetComponent(i, 0, sol->GetComponent(i, 0));
  }
  
  int coord[3]; 
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  
  // Set initial conditions
  if (this->KnownPoints != NULL) {
    
    int nc = this->KnownPoints->GetNumberOfComponents();    
    int num = this->KnownPoints->GetNumberOfTuples();
    
    for(int i=0; i<num; i++) {
      
      coord[0] = (int)this->KnownPoints->GetComponent(i, 0);
      coord[1] = (int)this->KnownPoints->GetComponent(i, 1);
      coord[2] = (int)this->KnownPoints->GetComponent(i, 2);
      float value = (nc > 3) ? this->KnownPoints->GetComponent(i, 3) : 0.0;
      
      int index = dim0Tdim1*coord[2] + dim[0]*coord[1] + coord[0];
      
      this->label->SetComponent(index, 0, VTK_MPJ_KNOWN);// Freeze voxel    
      this->convergence->SetComponent(index, 0, 0.0); // Boundary point has exact value
      out->SetComponent(index, 0, (float)value);  // Set arrival time
    }
  }
}

// ----------------------------------------------------------------------------
void vtkmpjLaxFriedrichSweeping::Iterate(vtkImageData *input, vtkImageData *output)
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
	    this->pog+=(4.0/(this->NumberOfIterations*8));
	    this->UpdateProgress(this->pog);
	    this->count=0;
	  }
      }
}

void vtkmpjLaxFriedrichSweeping::SweepBoundaries(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  double spc[3]; input->GetSpacing(spc);
  int dim0Tdim1 = dim[0]*dim[1];
  vtkDataArray *out=output->GetPointData()->GetScalars();
  
  int index;
  float val0, val1, val2, result;
  float error;
  
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
	  error = fabs(result - val0);
	  if (error > this->ErrorValue) this->ErrorValue = error;
	  this->convergence->SetComponent(index, 0, error);
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
	  error = fabs(result - val0);
	  if (error > this->ErrorValue) this->ErrorValue = error;
	  this->convergence->SetComponent(index, 0, error);
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
	  error = fabs(result - val0);
	  if (error > this->ErrorValue) this->ErrorValue = error;
	  this->convergence->SetComponent(index, 0, error);
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
	  error = fabs(result - val0);
	  if (error > this->ErrorValue) this->ErrorValue = error;
	  this->convergence->SetComponent(index, 0, error);
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
	  error = fabs(result - val0);
	  if (error > this->ErrorValue) this->ErrorValue = error;
	  this->convergence->SetComponent(index, 0, error);
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
	  error = fabs(result - val0);
	  if (error > this->ErrorValue) this->ErrorValue = error;
	  this->convergence->SetComponent(index, 0, error);
	} else 
	  this->convergence->SetComponent(index, 0, 0.0);	
      }  
}

// ----------------------------------------------------------------------------
void vtkmpjLaxFriedrichSweeping::Sweep(vtkImageData *input, vtkImageData *output, int dir[3])
{
  int dim[3]; input->GetDimensions(dim);
  double spc[3]; input->GetSpacing(spc);
  int dim0Tdim1 = dim[0]*dim[1];
  vtkDataArray *out=output->GetPointData()->GetScalars();

  double n[3];
  vtkDataArray *domain = NULL;

  // Sweep image
  for(int k=(dir[2]<0?dim[2]-2:1);(dir[2]<0?k>0:k<dim[2]-1);k+=dir[2])     
    for(int j=(dir[1]<0?dim[1]-2:1);(dir[1]<0?j>0:j<dim[1]-1);j+=dir[1])
      for(int i=(dir[0]<0?dim[0]-2:1);(dir[0]<0?i>0:i<dim[0]-1);i+=dir[0]) {
	
	int index = dim0Tdim1*k + dim[0]*j + i;
	int coord[3] = {i, j, k};
	
	// only update point if value is unknown and is part of the
	// computational domain
	if (this->label->GetComponent(index, 0) == VTK_MPJ_UNKNOWN) {
	  
	  double new_value = this->Update(input, output, coord, index, n);
	  
	  // only update if it's value is less than before
	  if (new_value < out->GetComponent(index, 0)) {
	    
	    // compute convergence 
	    double old_value = out->GetComponent(index, 0);	    
	    double error = fabs(new_value - old_value);

	    if (error > this->ErrorValue) this->ErrorValue = error;
	    this->convergence->SetComponent(index, 0, error);
	    
	    out->SetComponent(index, 0, (float)new_value);	    
	    
	    gradient->SetComponent(index, 0, -n[0]);
	    gradient->SetComponent(index, 1, -n[1]);
	    gradient->SetComponent(index, 2, -n[2]);	    
	    
	  } else 
	    this->convergence->SetComponent(index, 0, 0.0);
	}	
      }   
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichSweeping::SolveHp(int coord[3], int index, double p, double q, double r,double v)
{
  double eps = 1E-12;
  double a = 1.0, b=1, c=1;
  //double a = 1.0, b=0.5, c=0.1;
  double result = 1+a*v; //1+3*a;
  return result;
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichSweeping::SolveHq(int coord[3], int index, double p, double q, double r, double v)
{
  double eps = 1E-12;
  double a = 1.0, b=1, c=1;
  //double a = 1.0, b=0.5, c=0.1;
  double result = 1+b*v; //1+3*b;
  return result;
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichSweeping::SolveHr(int coord[3], int index, double p, double q, double r, double v)
{
  double eps = 1E-12;
  double a = 1.0, b=1, c=1;
  //double a = 1.0, b=0.5, c=0.1;
  double result = 1+c*v; //1+3*c;
  return result;
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichSweeping::SolveH(int coord[3], int index, double p, double q, double r, double v)
{
  double a = 1.0, b=1, c=1;
  //double a = 1.0, b=0.5, c=0.1;
  //double a=1.0/1, b=1.0/4, c=1.0/9;
  //double a=9.0/(9+4+1), b=4.0/(9+4+1), c=1.0/(9+4+1);
  double eps = 1E-12;
  double grad = p*p + q*q + r*r;
  
  
  //double result = p*p + sqrt(grad) / (sqrt(grad)+eps);
  double result = sqrt(a*p*p + b*q*q + c*r*r) * v;
 
  return result;
}

// ----------------------------------------------------------------------------
double vtkmpjLaxFriedrichSweeping::Update(vtkImageData *input, vtkImageData *output, int coord[3], int index, double n[3])
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  vtkDataArray *out=output->GetPointData()->GetScalars();
  vtkDataArray *in=input->GetPointData()->GetScalars();
  
  // Lax-Friedrichs Method
  double hx = this->Spacing[0]; //(float) (dim[0]-2) / (dim[0]-1);
  double hy = this->Spacing[1]; //(float) (dim[1]-2) / (dim[1]-1);
  double hz = this->Spacing[2]; //(float) (dim[2]-2) / (dim[2]-1);

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
  if (fabs(pp) > fabs(pm)) {   /* >= */
    pmax = fabs(pp); pmin = fabs(pm);
  } else {
    pmax = fabs(pm); pmin = fabs(pp);
  }
  
  double qmin, qmax;
  if (fabs(qp) > fabs(qm)) {   /* >= */
    qmax = fabs(qp); qmin = fabs(qm);
  } else {
    qmax = fabs(qm); qmin = fabs(qp);
  }
  
  double rmin, rmax;
  if (fabs(rp) > fabs(rm)) {   /* >= */
    rmax = fabs(rp); rmin = fabs(rm);
  } else {
    rmax = fabs(rm); rmin = fabs(rp);
  }

  double speed = in->GetComponent(index, 0);
  double H = SolveH(coord, index, (fabs(pp)+fabs(pm))/2.0, (fabs(qp)+fabs(qm))/2.0, (fabs(rp)+fabs(rm))/2.0, speed);
  double ox = SolveHp(coord, index, pmax, qmin, rmin, speed);
  double oy = SolveHq(coord, index, pmin, qmax, rmin, speed);
  double oz = SolveHr(coord, index, pmin, qmin, rmax, speed);
  
  double visc = 1.0 / (ox/hx + oy/hy + oz/hz);
  double phi = visc * ( 1.0 - H + (ox/2)*phi_p + (oy/2)*phi_q + (oz/2)*phi_r );
  
  n[0] = (fabs(pp)+fabs(pm))/2.0;
  n[1] = (fabs(qp)+fabs(qm))/2.0;
  n[2] = (fabs(rp)+fabs(rm))/2.0;

  return phi;
}

// ----------------------------------------------------------------------------
void vtkmpjLaxFriedrichSweeping::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int dim[3];  input->GetDimensions(dim);
  double spc[3]; input->GetSpacing(spc);
  
  if (this->GetInput()==NULL) {
    vtkErrorMacro(<<"No Input Image\n Exiting\n");
    return;
  }
  
  this->pog = 0.0; 
  this->tenth=4;//(int)(this->NumberOfIterations*8);
  this->count=0;
  
  this->Initialize(input, output);
  
  for(int j=0; j<this->NumberOfIterations; j++) {
    
    // reset error and iteration number
    this->ErrorValue = 0;
    this->IterationNumber = j+1;   
    
    this->Iterate(input, output);
    
    if ((this->StopOnMaximumError) && (this->ErrorValue <= this->MaximumError))
      break;    
  }
  
  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
