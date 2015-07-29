

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


#include "vtkmpjFastSweepingMethod.h"

//------------------------------------------------------------------------------
vtkmpjFastSweepingMethod* vtkmpjFastSweepingMethod::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjFastSweepingMethod");
  if(ret) {
    return (vtkmpjFastSweepingMethod*)ret;
  }
  
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjFastSweepingMethod;
}

// Construct object with no children.
vtkmpjFastSweepingMethod::vtkmpjFastSweepingMethod()
{
  this->SecondOrder = 0;
  this->KnownPoints = NULL;
  this->KnownValues = 1;
  this->NumberOfIterations = 2;
  this->Speed = NULL;  
  this->label = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjFastSweepingMethod::~vtkmpjFastSweepingMethod()
{
  if (this->KnownPoints != NULL)
    this->KnownPoints->Delete();
  if (this->LabelImage != NULL)
    this->LabelImage->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjFastSweepingMethod::SetSpeedImage(vtkImageData *speed)
{
  this->SetInput(speed);
}

// ----------------------------------------------------------------------------
void vtkmpjFastSweepingMethod::ExecuteInformation()
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
void vtkmpjFastSweepingMethod::SortTerms(double values[3]) 
{
  double tmp[3];
  
  if (values[0] < values[1]) {
    if (values[0] < values[2]) {
      if (values[1] < values[2]) {
	tmp[0] = values[0]; tmp[1] = values[1]; tmp[2] = values[2]; 
      } else {
	tmp[0] = values[0]; tmp[1] = values[2]; tmp[2] = values[1]; 
      }
    } else 
      {
	tmp[0] = values[2]; tmp[1] = values[0]; tmp[2] = values[1];
      }
  } else {
    if (values[1] < values[2]) {
      if (values[0] < values[2]) {
	tmp[0] = values[1]; tmp[1] = values[0]; tmp[2] = values[2];
      } else {
	tmp[0] = values[1]; tmp[1] = values[2]; tmp[2] = values[0];
      }
    } else
      {
	tmp[0] = values[2]; tmp[1] = values[1]; tmp[2] = values[0];
      }    
  }

  values[0] = tmp[0];
  values[1] = tmp[1];
  values[2] = tmp[2];
}

// ----------------------------------------------------------------------------
void vtkmpjFastSweepingMethod::Initialize(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  double spc[3]; input->GetSpacing(spc);
  vtkDataArray *in=input->GetPointData()->GetScalars();
  vtkDataArray *out=output->GetPointData()->GetScalars();

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

  // Initialize values to infinity
  out->FillComponent(0, VTK_LARGE_FLOAT);
  
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
    out->SetComponent(index, 0, (float)value);        // Set arrival time
  }
}

// ----------------------------------------------------------------------------
void vtkmpjFastSweepingMethod::Iterate(vtkImageData *input, vtkImageData *output)
{
  for(int s1=-1; s1<2; s1+=2)
    for(int s2=-1; s2<2; s2+=2)
      for(int s3=-1; s3<2; s3+=2) {

	int dir[3] = {s1, s2, s3};

	printf("sweeping %d %d %d\n",s1,s2,s3);
	
	this->Sweep(input, output, dir);
	
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
void vtkmpjFastSweepingMethod::Sweep(vtkImageData *input, vtkImageData *output, int dir[3])
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
	
	//if (this->label->GetComponent(index, 0) == VTK_MPJ_UNKNOWN) { 

	  double new_value = this->ComputeFirstOrder(input, output, coord, index);
	  
	  if (new_value < out->GetComponent(index, 0))
	    out->SetComponent(index, 0, (float)new_value);
	  //}
	
      } 
}

double vtkmpjFastSweepingMethod::Solve(double a, double b)
{  
  if (fabs(a - b) >= 1) {
    double min_ab = a < b ? a : b;
    return min_ab + 1;
  } else {
    double amb2 = (a-b)*(a-b);
    double result = (a+b+sqrt(2*1-amb2)) / 2.0;
    return result;
  }
}

// ----------------------------------------------------------------------------
double vtkmpjFastSweepingMethod::Solve2(double a, double b, double rhs)
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
double vtkmpjFastSweepingMethod::SolveN(int number, double term[3], double rhs)
{
  double coeff[3] = {-rhs*rhs, 0.0, 0.0};

  for (int j=0; j<3; j++) {
    if (term[j] != VTK_LARGE_FLOAT) {
      coeff[2] += 1.0;
      coeff[1] -= 2*term[j];
      coeff[0] += term[j]*term[j];
    }
  }
  
  int no_solutions = 1;
  double max_sol = VTK_LARGE_FLOAT;
  double sol[2] = {VTK_LARGE_FLOAT, VTK_LARGE_FLOAT};
  double discr = coeff[1]*coeff[1] - 4.0*coeff[2]*coeff[0];
  
  if (discr < 0) {
    printf("Solution is not differentiable! terms=%f, %f, %f, coeffs=%f, %f, %f\n", term[0], term[1], term[2], coeff[2], coeff[1], coeff[0]);
  }
  
  // solve quadratic equation
  vtkMath::SolveQuadratic(coeff[2], coeff[1], coeff[0], 
			  &sol[0], &sol[1], &no_solutions);
  if (no_solutions == 2) {
    max_sol = sol[0]>=sol[1]?sol[0]:sol[1];
  } else {
    max_sol = sol[0];
  }
  
  return max_sol;
}

// ----------------------------------------------------------------------------
double vtkmpjFastSweepingMethod::SolveQuadratic(int number, double term[3], double rhs)
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
void vtkmpjFastSweepingMethod::ComputeNormal(vtkImageData *input, vtkImageData *output, 
					    int coord[3], int index, double result[3])
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  vtkDataArray *in = input->GetPointData()->GetScalars(); 
  vtkDataArray *out = output->GetPointData()->GetScalars();
  
  double normal[3];
  result[0]=normal[0]=0.0;
  result[1]=normal[1]=0.0;
  result[2]=normal[2]=0.0;
  
  // process each one of the 26 neighbors
  int count = 0;
  for(int i=-1;i<2;i++)
    for(int j=-1;j<2;j++)
      for(int k=-1;k<2;k++) {
	if ((coord[0]+i)>=0 && ((coord[0]+i)<dim[0]) &&
	    (coord[1]+j)>=0 && ((coord[1]+j)<dim[1]) &&
	    (coord[2]+k)>=0 && ((coord[2]+k)<dim[2]))
	  {	  	    
	    int neighbor=index + i*factor[0] + j*factor[1] + k*factor[2];
	    if (this->label->GetComponent(neighbor, 0) == VTK_MPJ_KNOWN) {
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
double vtkmpjFastSweepingMethod::ComputeFirstOrder(vtkImageData *input, vtkImageData *output,
						  int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  vtkDataArray *in = input->GetPointData()->GetScalars(); 
  vtkDataArray *out = output->GetPointData()->GetScalars();
  
  // get speed value
  //double F = this->ComputeSpeed(input, output, coord, index);

  // estimate normal
  double normal[3];
  //this->ComputeNormal(input, output, coord, index, normal);
  //double F = ComputeDiffusion(input, index, normal);

  double F = 1.0;

  if (F == 0.0) 
    return VTK_LARGE_FLOAT;
    
  int num_terms = 0;
  double direction[3];
  double term[3];
  int term_index[3];
  
  // process each one of the six neighbors
  for (int j=0; j<3; j++) { // axis  
    
    direction[0] = 0.0;
    direction[1] = 0.0;
    direction[2] = 0.0;
    
    term[j] = VTK_LARGE_FLOAT;
    
    for (int i=-1; i<2; i+=2) { // direction 
      if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	
	direction[j] = (double)i;
	
	int neighbor=index + i*factor[j];
	
	// if value is frozen get its time
	//if (this->label->GetComponent(neighbor, 0) == VTK_MPJ_FROZEN) {
	  
	  double time = (double)out->GetComponent(neighbor, 0);
	  
	  if (time < term[j]) {
	    term[j] = time;
	    term_index[j] = neighbor;
	  }

	  //} // if
      } // if       
    } // for direction 
    

    // accumulate number of quadratic terms
    //    printf("term[%d]=%f\n",j,term[j]);
    num_terms += (term[j] != VTK_LARGE_FLOAT);
  
  } // for axis
  
  //printf("num_terms=%d\n",num_terms);
  
  // sort terms
  this->SortTerms(term);

  double solution = VTK_LARGE_FLOAT;

  /*  if (num_terms == 3) { 
      double sum_terms = term[0] + term[1] + term[2];
      double termsq0 = term[0]*term[0];
      double termsq1 = term[1]*term[1];
      double termsq2 = term[2]*term[2];
      double aux = 3.0 * ((1.0 / F*F) - termsq0 - termsq1 - termsq2);
      solution = (sum_terms + sqrt(aux + sum_terms*sum_terms)) / 3.0;
      } else if (num_terms == 2) {
      double sum_terms = term[0] + term[1];
      double aux = term[0] - term[1];
      solution = (sum_terms + sqrt(2.0 * (1.0 / F*F) - aux*aux)) / 2.0;
      } else {
      solution = term[0] + (1.0 / F);
      }*/

//this->SortTerms(diff);
  
  //double F = diffusion;
  //  printf("diffusion=%f\n",diffusion);
  //if (F == 0.0)
  //return VTK_LARGE_FLOAT;

  //printf("F=%f / %f=%f\n",sum_diff,(float)num_terms,F);
  
// F = diff[0];
//if (this->Debug) { 
//  printf("F=%f\n",F);
//}

  // solve quadratic for maximum solution
  solution = this->SolveQuadratic(num_terms, term, 1.0 / F);

  if (this->Debug)
    printf("sol=%f\n",solution);

  //double old_solution = (double)out->GetComponent(index, 0);
  //if (old_solution < solution) 
  //return old_solution;
  //else    
    return solution;
}


// ----------------------------------------------------------------------------
double vtkmpjFastSweepingMethod::Update(vtkImageData *input, vtkImageData *output, int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  // float spc[3]; input->GetSpacing(spc);
  float spc[3] = {1.0, 1.0, 1.0};
  int dim0Tdim1 = dim[0]*dim[1];
  vtkDataArray *out=output->GetPointData()->GetScalars();
  
  double min_value[3];
  double f = 1.0;
  double coeff[3] = {-1.0 / (f*f), 0.0, 0.0};        // coefficient array
  int factor[3] = {1, dim[0], dim0Tdim1};
  
  if ( f == 0.0 ) {
    return VTK_LARGE_FLOAT; 
    printf("Warning: F is %lf at %d,%d,%d\n",f,coord[0],coord[1],coord[2]); 
  }
  
  // process each one of the six neighbors
  for (int j=0; j<3; j++) { // axis  
    
    min_value[j] = VTK_LARGE_FLOAT;
    
    for (int i=-1; i<2; i+=2) { // direction 
      if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	
	int neighbor=index + i*factor[j];
	
	double value = (double)out->GetComponent(neighbor, 0);
	
	if (value < min_value[j]) 
	  min_value[j] = value;
	
      } // if
    } // for     
    
    // accumulate quadratic coefficients 
    /*if (min_value[j] != VTK_LARGE_FLOAT) {
      coeff[2] += 1.0;
      coeff[1] -= 2*min_value[j];
      coeff[0] += min_value[j]*min_value[j];
      }*/
  }
  
  double max_sol = VTK_LARGE_FLOAT;
  double sol[2] = {VTK_LARGE_FLOAT, VTK_LARGE_FLOAT};
  double discr = coeff[1]*coeff[1] - 4.0*coeff[2]*coeff[0];  /* discr = b^2-4ac */

  /* if discriminant is zero, shift center of sphere */
  /*  if (discr < 0) {*/
    
    //    printf("---Discriminant is negative---\n");
    //  printf("1: coeffs=%lf,%lf,%lf\n",coeff[2],coeff[1],coeff[0]);
    //  printf("1: sol=%lf  f=%lf\n",max_sol,f);
    //  printf("1:discr=%f\n",discr);
    //  printf("---------------\n");
    
    // sort values in increasing order
    //int order[3];
    //this->Sort(min_value,order);
    
  /*    if (min_value[0] > min_value[1]) {
	min_value[0] = min_value[1] + spc[0];
	} else {
	min_value[1] = min_value[0] + spc[1];
	}
	
	coeff[0]= -1.0 / (f*f); coeff[1]=coeff[2]=0.0;
	for (int j=0; j<2; j++) {
	coeff[2] += 1.0 / spc[j];
	coeff[1] -= 2*min_value[j] / spc[j];
	coeff[0] += min_value[j]*min_value[j] / spc[j];
	}
	
	}*/

  // needs reordering
  this->SortTerms(min_value);

  double r1 = Solve(min_value[0], min_value[1]);
  if (r1 <= min_value[2]) return r1;
  double r2 = Solve(min_value[1], min_value[2]);
  return r2;

  /*  double result = min_value[0] + 1;
  if (result <= min_value[1]) {
    return result;
  }
  
  if (fabs(min_value[0] - min_value[1]) >= 1) {
    double minab = min_value[0] < min_value[1] ? min_value[0] : min_value[1];
    return minab + 1;
  } else {
    double amb2 = (min_value[0]-min_value[1])*(min_value[0]-min_value[1]);
    double result = (min_value[0]+min_value[1]+sqrt(2-amb2)) / 2;
    return result;
    }*/
  
  /*else {*/
  
  // solve quadratic equation
  /*int no_solutions;
    vtkMath::SolveQuadratic(coeff[2], coeff[1], coeff[0], 
    &sol[0], &sol[1], &no_solutions);
    if (no_solutions == 2) {
    max_sol = sol[0]>=sol[1]?sol[0]:sol[1];
    } else {
    max_sol = sol[0];
    }
  
  return max_sol;*/
 
}

// ----------------------------------------------------------------------------
void vtkmpjFastSweepingMethod::SimpleExecute(vtkImageData *input, vtkImageData *output)
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

  this->Initialize(input, output);
  this->UpdateProgress(0.01);
  
  for(int j=0; j<this->NumberOfIterations; j++) {
    printf("iteration %d\n", j);
    this->Iterate(input, output);
  }
  
  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
