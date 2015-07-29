#include "vtkmpjFastMarchingMethod.h"
#include <time.h>

//------------------------------------------------------------------------------
vtkmpjFastMarchingMethod* vtkmpjFastMarchingMethod::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjFastMarchingMethod");
  if(ret)
    {
      return (vtkmpjFastMarchingMethod*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjFastMarchingMethod;
}

// Construct object with no children.
vtkmpjFastMarchingMethod::vtkmpjFastMarchingMethod()
{
  this->KnownPoints = NULL;
  this->KnownValues = 1;
  this->SecondOrder = 0;
  this->LabelImage = NULL;
  this->FrontPoints = NULL;
  this->PropagationTime = VTK_LARGE_FLOAT;
  this->Infinity = VTK_LARGE_FLOAT;
  this->Spacing[0] = 1.0;
  this->Spacing[1] = 1.0;
  this->Spacing[2] = 1.0;
  this->queue = NULL;
  this->label = NULL;
  this->neighbor_index[0]=0;
  this->neighbor_index[1]=0;
  this->neighbor_index[2]=0;
  this->ElapsedTime = 0;
}

// ----------------------------------------------------------------------------
vtkmpjFastMarchingMethod::~vtkmpjFastMarchingMethod()
{
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
int vtkmpjFastMarchingMethod::GetNeighborIndex(int j)
{
  return this->neighbor_index[j];
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingMethod::SetSpeedImage(vtkImageData *speed)
{
  this->SetInput(speed);
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingMethod::ExecuteInformation()
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

  output->SetNumberOfScalarComponents(1);  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingMethod::SortTerms(double term[3], double spc[3]) 
{
  double tmp[3];
  double tmp_spc[3];

  if (term[0] < term[1]) {
    if (term[0] < term[2]) {
      if (term[1] < term[2]) {
	tmp[0] = term[0]; tmp[1] = term[1]; tmp[2] = term[2]; 
	tmp_spc[0] = spc[0]; tmp_spc[1] = spc[1]; tmp_spc[2] = spc[2]; 
      } else {
	tmp[0] = term[0]; tmp[1] = term[2]; tmp[2] = term[1]; 
	tmp_spc[0] = spc[0]; tmp_spc[1] = spc[2]; tmp_spc[2] = spc[1]; 
      }
    } else 
      {
	tmp[0] = term[2]; tmp[1] = term[0]; tmp[2] = term[1];
	tmp_spc[0] = spc[2]; tmp_spc[1] = spc[0]; tmp_spc[2] = spc[1];
      }
  } else {
    if (term[1] < term[2]) {
      if (term[0] < term[2]) {
	tmp[0] = term[1]; tmp[1] = term[0]; tmp[2] = term[2];
	tmp_spc[0] = spc[1]; tmp_spc[1] = spc[0]; tmp_spc[2] = spc[2];
      } else {
	tmp[0] = term[1]; tmp[1] = term[2]; tmp[2] = term[0];
	tmp_spc[0] = spc[1]; tmp_spc[1] = spc[2]; tmp_spc[2] = spc[0];
      }
    } else
      {
	tmp[0] = term[2]; tmp[1] = term[1]; tmp[2] = term[0];
	tmp_spc[0] = spc[2]; tmp_spc[1] = spc[1]; tmp_spc[2] = spc[0];
      }    
  }

  term[0] = tmp[0];
  term[1] = tmp[1];
  term[2] = tmp[2];

  spc[0] = tmp_spc[0];
  spc[1] = tmp_spc[1];
  spc[2] = tmp_spc[2];
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingMethod::Solve2(double a, double b, double spc[3], double rhs)
{  
  double amb = a - b;
  if (fabs(amb) >= (rhs*spc[0])) {
    return (a < b ? a : b) + rhs*spc[0];
  } else {
    double amb2 = amb*amb;
    return (a+b+sqrt(2.0*rhs*rhs*spc[0]-amb2)) / 2.0;
  }
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingMethod::SolveN(int number, double term[3], double spc[3], double rhs)
{
  double coeff[3] = {-rhs*rhs, 0.0, 0.0};
  double spc2;

  for (int j=0; j<3; j++) {
    if (term[j] != VTK_LARGE_FLOAT) {
      spc2 = spc[j];
      coeff[2] += 1.0 / spc2;
      coeff[1] -= (2*term[j]) / spc2;
      coeff[0] += (term[j]*term[j]) / spc2;
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
  vtkPolynomialSolversUnivariate::SolveQuadratic(coeff[2], coeff[1], coeff[0], 
			  &sol[0], &sol[1], &no_solutions);
  if (no_solutions == 2) {
    max_sol = sol[0]>=sol[1]?sol[0]:sol[1];
  } else {
    max_sol = sol[0];
  }
  
  return max_sol;
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingMethod::RegularSolve(double coeff[3])
{ 
  int no_solutions = 1;
  double max_sol = VTK_LARGE_FLOAT;
  double sol[2] = {VTK_LARGE_FLOAT, VTK_LARGE_FLOAT};
  double discr = coeff[1]*coeff[1] - 4.0*coeff[2]*coeff[0];
  
  if (discr < 0) {
    printf("Solution is not differentiable! coeffs=%f, %f, %f\n",coeff[2], coeff[1], coeff[0]);
  }
  
  // solve quadratic equation
  vtkPolynomialSolversUnivariate::SolveQuadratic(coeff[2], coeff[1], coeff[0], 
			  &sol[0], &sol[1], &no_solutions);
  if (no_solutions == 2) {
    max_sol = sol[0]>=sol[1]?sol[0]:sol[1];
  } else {
    max_sol = sol[0];
  }
  
  return max_sol;
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingMethod::SolveQuadratic(int number, double term[3], double spc[3], double rhs)
{  
  double result;
  
  switch(number) {
  case 1:
    //result = term[0] + rhs*spc[0];
    //break;
    // case 2:
    //result = Solve2(term[0], term[1], spc, rhs);
    //break;
  case 3:
  default:
    //if ((term[0] + rhs*spc[0]) > term[1]) {
    //result = term[0] + rhs*spc[0];      
    //} else {
    //result = Solve2(term[0], term[1], spc, rhs);
    //}
    //if (result > term[2])
      result = SolveN(number, term, spc, rhs);
    break;
  };
      
  return(result);
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingMethod::ComputeSpeed(vtkImageData *input, vtkImageData *output, 
					      int coord[3], int index)
{
  vtkDataArray *in = input->GetPointData()->GetScalars();
  return in->GetComponent(index, 0);
}

// ----------------------------------------------------------------------------
// double vtkmpjFastMarchingMethod::ComputeFirstOrder(vtkImageData *input, vtkImageData *output,
// 						  int coord[3], int index)
// {
//   int dim[3]; input->GetDimensions(dim);
//   double spc[3]; input->GetSpacing(spc);
//   int dim0Tdim1 = dim[0]*dim[1];
//   int factor[3] = {1, dim[0], dim0Tdim1};
//   vtkDataArray *in = input->GetPointData()->GetScalars(); 
//   vtkDataArray *out = output->GetPointData()->GetScalars();
  
//   // get speed value
//   double F = this->ComputeSpeed(input, output, coord, index);
  
//   if (F == 0.0) 
//     return VTK_LARGE_FLOAT;
    
//   int num_terms = 0;
//   double term[3];
  
//   // process each one of the six neighbors
//   for (int j=0; j<3; j++) { // axis  
    
//     term[j] = VTK_LARGE_FLOAT;
    
//     for (int i=-1; i<2; i+=2) { // direction 
//       if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	
// 	int neighbor=index + i*factor[j];
	
// 	// if value is frozen get its time
// 	if (this->label->GetComponent(neighbor, 0) == VTK_MPJ_FROZEN) {
	  
// 	  double time = (double)out->GetComponent(neighbor, 0);
	  
// 	  if (time < term[j]) {
// 	    term[j] = time;
// 	    this->neighbor_index[j] = neighbor;
// 	  }
	  
// 	} // if
//       } // if
      
      
//     } // for direction 
    
//     term[j] /= spc[j];

//     // accumulate number of quadratic terms
//     num_terms += (term[j] != VTK_LARGE_FLOAT);
    
//   } // for axis
  
//   // sort terms
//   /this->SortTerms(term);

//   // solve quadratic for maximum solution
//   double solution = this->SolveQuadratic(num_terms, term, 1.0 / F);   // this->Spacing / F

//   return solution;
// }

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingMethod::ComputeFirstOrder(vtkImageData *input, vtkImageData *output,
						  int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  vtkDataArray *in = input->GetPointData()->GetScalars(); 
  vtkDataArray *out = output->GetPointData()->GetScalars();
  
  // get speed value
  double F = this->ComputeSpeed(input, output, coord, index);
  
  if (F == 0.0) 
    return this->Infinity;//VTK_LARGE_FLOAT;
    
  int num_terms = 0;
  double term[3];
  double rhs = 1.0 / F;
  double spc[3] = {this->Spacing[0],this->Spacing[1],this->Spacing[2]};
  double coeff[3] = {-rhs*rhs, 0, 0};

  // process each one of the six neighbors
  for (int j=0; j<3; j++) { // axis  
    
    term[j] = VTK_LARGE_FLOAT;
    
    for (int i=-1; i<2; i+=2) { // direction 
      if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	
	int neighbor=index + i*factor[j];
	
	// if value is frozen get its time
	if (this->label->GetComponent(neighbor, 0) == VTK_MPJ_FROZEN) {
	  
	  double time = (double)out->GetComponent(neighbor, 0);
	  
	  if (time < term[j]) {
	
	    term[j] = time;	    
	    this->neighbor_index[j] = neighbor;
	    
	  }	  
	} // if
      } // if
      
      
    } // for direction 
    
    double value1 = term[j];
    //double spc2 = this->Spacing[j] * this->Spacing[j];
    
    num_terms += (value1 != VTK_LARGE_FLOAT);
    
    //if (value1 != VTK_LARGE_FLOAT) {
    //coeff[2] += 1 / spc2;
    // coeff[1] -= (2*value1) / spc2;
    //coeff[0] += (value1*value1) / spc2;
    //}
    
  } // for axis
    
  // solve quadratic for maximum solution
  this->SortTerms(term, spc);
  
  //for(int j=0;j<3;j++) {  
  //double value1 = term[j];
  //double spc2 = spc[j] * spc[j];    
  //if (value1 != VTK_LARGE_FLOAT) {
  //  coeff[2] += 1 / spc2;
  //  coeff[1] -= (2*value1) / spc2;
  //  coeff[0] += (value1*value1) / spc2;
  //}
  //}

  double solution = this->SolveQuadratic(num_terms, term, spc, 1.0 / F);  

  // this->Spacing / F;
  //double solution = this->RegularSolve(coeff);

  return solution;
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingMethod::ComputeSecondOrder(vtkImageData *input, vtkImageData *output,
						    int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  vtkDataArray *in = input->GetPointData()->GetScalars(); 
  vtkDataArray *out = output->GetPointData()->GetScalars();
  
  // get speed value
  double F = this->ComputeSpeed(input, output, coord, index);

  if (F == 0.0) 
    return this->Infinity;//VTK_LARGE_FLOAT;
    
  int num_terms = 0;
  int num_2ndterms = 0;
  double term[3], term2[3];
  double spc[3] = {this->Spacing[0],this->Spacing[1],this->Spacing[2]};
  double value1, value2;
  double rhs = 1.0 / F;
  double coeff[3] = {-rhs*rhs, 0, 0};
  
  // process each one of the six neighbors
  for (int j=0; j<3; j++) { // axis  
    
    value1 = VTK_LARGE_FLOAT;
    value2 = VTK_LARGE_FLOAT;
    
    for (int i=-1; i<2; i+=2) { // direction 
      if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	
	int neighbor=index + i*factor[j];
	
	// if value is frozen get its time
	if (this->label->GetComponent(neighbor, 0) == VTK_MPJ_FROZEN) {
	  
	  double tmp1 = (double)out->GetComponent(neighbor, 0);
	  
	  if (tmp1 < value1) {
	    
	    term[j] = value1 = tmp1;
	    
	    this->neighbor_index[j] = neighbor;
	    
	    // examine 2nd order neighbor
	    if ((coord[j]+2*i)>=0 && (coord[j]+2*i)<dim[j]) {
	      
	      // pick 2nd order neighbor
	      int neighbor2=index + 2*i*factor[j];
	      
	      // read time
	      double tmp2 = (double)out->GetComponent(neighbor2, 0);
	      
	      if ((this->label->GetComponent(neighbor2, 0) == VTK_MPJ_FROZEN) && (tmp2 <= tmp1)) 
		value2 = tmp2;
	      else
		value2 = VTK_LARGE_FLOAT;
	    } // if ((coord
	  }	  		  
	} // if
      } // if 
      
    } // direction
    
    double spc2 = this->Spacing[j] * this->Spacing[j];
    
    if (value2 != VTK_LARGE_FLOAT) {
      
      num_2ndterms++;
      
      double tp = (1.0/3.0)*(4.0*value1-value2);
      double a = 9.0/4.0;
      
      coeff[2] += a / spc2;
      coeff[1] -= (2*a*tp) / spc2;
      coeff[0] += (a*tp*tp) / spc2;
      
    } else  
      if (value1 != VTK_LARGE_FLOAT) {
	
	num_terms++;

	coeff[2] += 1 / spc2;
	coeff[1] -= (2*value1) / spc2;
	coeff[0] += (value1*value1) / spc2;
      }
    
  } // for axis
  
  double solution;

  // solve quadratic for maximum solution
  solution = this->RegularSolve(coeff);
  
  return solution;
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingMethod::ComputeTime(vtkImageData *input, vtkImageData *output,
					     int coord[3], int index)
{
  double time;

  // compute time of arrival at neighbor
  if (this->SecondOrder == 0)
    time = this->ComputeFirstOrder(input, output, coord, index);
  else 
    time = this->ComputeSecondOrder(input, output, coord, index);
  
  // insert neighbor in the narrow band
  if (this->label->GetComponent(index, 0) != VTK_MPJ_NARROW_BAND) {
    this->label->SetComponent(index, 0, VTK_MPJ_NARROW_BAND);
    this->queue->Insert(time, index);
  } else {
    // if already in the narrow band, store new time
    this->queue->DeleteId(index);
    this->queue->Insert(time, index);
  }

  return time;
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingMethod::InitializeFront(vtkImageData *input, vtkImageData *output)
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
  this->label->FillComponent(0, VTK_MPJ_UNVISITED);

  out->FillComponent(0, this->Infinity);
  
  int index, coord[3], ncoord[3];
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  
  // Freeze known points
  int num_known = this->KnownPoints->GetNumberOfTuples();
  for(int i=0; i<num_known; i++) {
    
    coord[0] = ncoord[0] = (int)this->KnownPoints->GetComponent(i, 0);
    coord[1] = ncoord[1] = (int)this->KnownPoints->GetComponent(i, 1);
    coord[2] = ncoord[2] = (int)this->KnownPoints->GetComponent(i, 2);
    float value = this->KnownPoints->GetComponent(i, 3);
    
    index = dim0Tdim1*coord[2] + dim[0]*coord[1] + coord[0];
    
    this->label->SetComponent(index, 0, VTK_MPJ_FROZEN);   // Freeze voxel

    out->SetComponent(index, 0, this->KnownValues ? value : 0.0);              
    
    this->count++;
  }

  // Process boundary neighbors
  for(int i=0; i<this->KnownPoints->GetNumberOfTuples(); i++) {
  
    coord[0] = ncoord[0] = (int)KnownPoints->GetComponent(i, 0);
    coord[1] = ncoord[1] = (int)KnownPoints->GetComponent(i, 1);
    coord[2] = ncoord[2] = (int)KnownPoints->GetComponent(i, 2);
    index = dim0Tdim1*coord[2] + dim[0]*coord[1] + coord[0];

    // process each one of the six neighbors
    for (int j=0; j<3; j++) { // axis  
      for (int i=-1; i<2; i+=2)  { // direction	  	
	
	if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  	    
	  
	  int neighbor=index + i*factor[j];
	  ncoord[j] = coord[j] + i;   // update coordinate
	  
	  // compute arrival time
	  if (this->label->GetComponent(neighbor, 0) != VTK_MPJ_FROZEN)
	    this->ComputeTime(input, output, ncoord, neighbor);
	  
	  ncoord[j] = coord[j]; // reset
	}
      }
    }
  }
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingMethod::PropagateFront(vtkImageData *input, vtkImageData *output)
{
  double time;
  int index, coord[3], ncoord[3];
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  vtkDataArray *out=output->GetPointData()->GetScalars();

  while ((index = this->queue->Pop(time, 0)) != -1) {
    
    this->label->SetComponent(index, 0, VTK_MPJ_FROZEN);    
    out->SetComponent(index, 0, (float)time);

    this->count++;
    if (this->count==this->tenth)
      {
	this->pog+=0.1;
	this->UpdateProgress(this->pog);
	this->count=0;
      }
    
    // stop if arrival time is greater or equal to max time
    if ((time == VTK_LARGE_FLOAT) || 
	(this->PropagationTime != VTK_LARGE_FLOAT) && (time >= this->PropagationTime)) {
      break;
    }

    // decompose index into x, y, z coordinates
    coord[2] = ncoord[2] = index / dim0Tdim1; 
    int r = index % dim0Tdim1;
    coord[1] = ncoord[1] = r / dim[0];
    coord[0] = ncoord[0] = r % dim[0];
    
    //    if (time == VTK_LARGE_FLOAT) {
      /*printf("%d %d %d\n",coord[0],coord[1],coord[2]);*/
      //continue;
    //}
    
    // process each one of the six neighbors
    for (int j=0; j<3; j++)  // axis  
      for (int i=-1; i<2; i+=2)  { // direction 
	
	if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	  int neighbor=index + i*factor[j];
	  
	  if (this->label->GetComponent(neighbor, 0) != VTK_MPJ_FROZEN) { 
	   
	    ncoord[j] = coord[j] + i;   // update coordinate

	    this->ComputeTime(input, output, ncoord, neighbor);

	    ncoord[j] = coord[j]; // reset
	  }	  
	}
      }
    } // while  
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingMethod::SaveFrontPoints(vtkImageData *input)
{
  int total = this->queue->GetNumberOfItems();
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int index, coord[3];
  double value;
  
  if (this->FrontPoints != NULL) 
    this->FrontPoints->Delete();
  
  this->FrontPoints = vtkFloatArray::New(); 
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
    this->FrontPoints->InsertNextTuple4((float)coord[0], (float)coord[1], (float)coord[2], 
					(float)value);
  }
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingMethod::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  
  if (this->GetInput()==NULL) {
    vtkErrorMacro(<<"No Speed Image\n Exiting\n");
    return;
  }
  
  int total = input->GetPointData()->GetScalars()->GetNumberOfTuples();  
  this->pog = 0.0;  
  this->tenth = (int)(total / 10.0);
  this->count = 0;
  
  this->UpdateProgress(0.01);

  clock_t start = clock();

  // initialize
  this->InitializeFront(input, output);  
  
  // propagate front
  this->PropagateFront(input, output);

  clock_t end = clock();

  this->ElapsedTime = (end-start)/(double)CLOCKS_PER_SEC;

  //printf("time1=%f\n",end-start);
  //  printf("time=%f\n",(end-start)/(double)CLOCKS_PER_SEC);

  // save front points
  this->SaveFrontPoints(input);

  this->UpdateProgress(1.0);
}

// ----------------------------------------------------------------------------
