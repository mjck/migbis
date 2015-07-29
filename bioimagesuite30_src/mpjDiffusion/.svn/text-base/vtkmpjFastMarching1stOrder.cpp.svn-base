

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


#include "vtkmpjFastMarching1stOrder.h"

//------------------------------------------------------------------------------
vtkmpjFastMarching1stOrder* vtkmpjFastMarching1stOrder::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjFastMarching1stOrder");
  if(ret)
    {
      return (vtkmpjFastMarching1stOrder*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjFastMarching1stOrder;
}

// Construct object with no children.
vtkmpjFastMarching1stOrder::vtkmpjFastMarching1stOrder()
{
  this->Mask = NULL;
  this->MaskOutValue = 0;
  this->Seeds = NULL;
  this->state = NULL;
  this->queue = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjFastMarching1stOrder::~vtkmpjFastMarching1stOrder()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
  if (this->Seeds != NULL)
    this->Seeds->Delete();
  if (this->queue != NULL)
    this->queue->Delete();
  if (this->state != NULL)
    this->state->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarching1stOrder::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  int dim[3];  input->GetDimensions(dim);

  if (this->Seeds==NULL) {
      vtkErrorMacro(<<"No Seeds specified!!");
    return;
  }

  output->SetNumberOfScalarComponents(1);  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
int vtkmpjFastMarching1stOrder::InMask(int index)
{
  return (this->mask != NULL)?(this->mask->GetComponent(index, 0) > 0) : 1;
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarching1stOrder::F(vtkImageData *input, vtkImageData *output,
				   int coord[3], int index)
{
  return 1; // the higher the speed, the lower arrival time.
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarching1stOrder::ComputeArrivalTime(vtkImageData *input, vtkImageData *output, 
						    int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  vtkDataArray *out=output->GetPointData()->GetScalars();
  
  double min_time1;
  double f = this->F(input, output, coord, index); // compute speed function
  double coeff[3] = {-1.0 / (f*f), 0.0, 0.0};        // coefficient array
  
  // number of voxels in a row and slice
  int factor[3] = {1, dim[0], dim0Tdim1};

  // process each one of the six neighbors
  for (int j=0; j<3; j++) { // axis  
    
    min_time1 = VTK_LARGE_FLOAT;
    
    for (int i=-1; i<2; i+=2) { // direction 
      if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	
	int neighbor=index + i*factor[j];
	
	// if value is frozen get its time
	if (this->InMask(neighbor) && this->state->GetValue(neighbor) == VTK_MPJ_FROZEN) { 
	  
	  double time1 = (double)out->GetComponent(neighbor, 0);
	  
	  if (time1 < min_time1) 
	    min_time1 = time1;
	  
	} // if 
      } // if
    } // for     
    
    // accumulate quadratic coefficients 
    if (min_time1 != VTK_LARGE_FLOAT) {
      coeff[2] += 1.0;
      coeff[1] -= 2*min_time1;
      coeff[0] += min_time1*min_time1;    
    }
  }
  
  // compute arrival time
  int no_solutions = 0;
  double max_sol = VTK_LARGE_FLOAT;
  double sol[2] = {VTK_LARGE_FLOAT, VTK_LARGE_FLOAT};
  
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
void vtkmpjFastMarching1stOrder::ComputeNeighbor(vtkImageData *input, vtkImageData *output, 
					       int coord[3], int index)
{
  // compute time of arrival at neighbor
  double time = this->ComputeArrivalTime(input, output, coord, index);
  
  // insert neighbor in the narrow band
  if (this->state->GetValue(index) != VTK_MPJ_NARROW_BAND) {
    this->state->SetValue(index, VTK_MPJ_NARROW_BAND);
    this->queue->Insert(time, index);
  } else {
    // if already in the narrow band, store new time
    this->queue->DeleteId(index);
    this->queue->Insert(time, index);
  }
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarching1stOrder::InitializeFront(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  vtkDataArray *in=input->GetPointData()->GetScalars();
  vtkDataArray *out=output->GetPointData()->GetScalars();

  // Create priority queue
  this->queue = vtkmpjPriorityQueue::New();
  this->queue->Allocate(1000);

  // Create state array
  this->state = vtkUnsignedCharArray::New();
  this->state->Allocate(in->GetNumberOfTuples());
  this->state->FillComponent(0, VTK_MPJ_UNVISITED);

  int index;
  int coord[3], ncoord[3];
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};

  // Freeze seed point(s)
  for(int i=0; i<Seeds->GetNumberOfTuples(); i++) {

    coord[0] = ncoord[0] = (int)Seeds->GetComponent(i, 0);
    coord[1] = ncoord[1] = (int)Seeds->GetComponent(i, 1);
    coord[2] = ncoord[2] = (int)Seeds->GetComponent(i, 2);
    index = dim0Tdim1*coord[2] + dim[0]*coord[1] + coord[0];

    if (this->InMask(index)) {
      this->state->SetValue(index, VTK_MPJ_FROZEN);   // Freeze voxel
      out->SetComponent(index, 0, (float)0.0);               // Set arrival time to 0
      this->count++;
      
      // process each one of the six neighbors
      for (int j=0; j<3; j++)  // axis  
	for (int i=-1; i<2; i+=2)  {// direction 
	  //fprintf(stderr,"coord %d %d %d\n",coord[0],coord[1],coord[2]);
	  if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	    int neighbor=index + i*factor[j];
	    ncoord[j] = coord[j] + i;   // update coordinate
	    //fprintf(stderr,"ncoord %d %d %d %d\n",ncoord[0],ncoord[1],ncoord[2],neighbor);
	    
	    this->ComputeNeighbor(input, output, ncoord, neighbor);
	    ncoord[j] = coord[j]; // reset
	  }
	}
    } else {
      vtkErrorMacro(<<"Seed point "<<coord[0]<<","<<coord[1]<<","<<coord[2]<<" outside mask!");
    }
  }
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarching1stOrder::PropagateFront(vtkImageData *input, vtkImageData *output)
{
  double time;
 
  vtkDataArray *out=output->GetPointData()->GetScalars();

  int coord[3], ncoord[3];
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];

  // front propagation
  int index;
  while ((index = this->queue->Pop(time, 0)) != -1) {
    
    this->state->SetValue(index, VTK_MPJ_FROZEN);
    out->SetComponent(index, 0, (float)time);

    this->count++;
    if (this->count==this->tenth)
      {
	this->pog+=0.1;
	this->UpdateProgress(pog);
	this->count=0;
      }
       
    // decompose index into x, y, z coordinates
    coord[2] = ncoord[2] = index / dim0Tdim1; 
    int r = index % dim0Tdim1;
    coord[1] = ncoord[1] = r / dim[0];
    coord[0] = ncoord[0] = r % dim[0];

    // number of voxels in a row and slice
    int factor[3] = {1, dim[0], dim0Tdim1};

    // process each one of the six neighbors
    for (int j=0; j<3; j++)  // axis  
      for (int i=-1; i<2; i+=2)  // direction 
	if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	  int neighbor=index + i*factor[j];
	  if (this->InMask(neighbor) && this->state->GetValue(neighbor)!=VTK_MPJ_FROZEN) { 
	    ncoord[j] = coord[j] + i;   // update coordinate
	    this->ComputeNeighbor(input, output, ncoord, neighbor);
	    ncoord[j] = coord[j]; // reset
	  }
	}

    } // while  
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarching1stOrder::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }

  this->mask = (this->Mask != NULL) ? this->Mask->GetPointData()->GetScalars() : NULL;

  vtkDataArray* in =input->GetPointData()->GetScalars();  
  
  int numtuples=in->GetNumberOfTuples();
  int numcomp=in->GetNumberOfComponents();

  this->pog = 0.0;  
  this->tenth=(int)(numtuples * numcomp / 10.0);
  this->count=0;

  // Initialize front
  this->InitializeFront(input, output);

  this->UpdateProgress(0.01);

  // Propagate front
  this->PropagateFront(input, output);
  
  this->UpdateProgress(1.0);

}
// ----------------------------------------------------------------------------
