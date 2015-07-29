

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


#include "vtkmpjFastMarchingAdvection.h"

//------------------------------------------------------------------------------
vtkmpjFastMarchingAdvection* vtkmpjFastMarchingAdvection::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjFastMarchingAdvection");
  if(ret)
    {
      return (vtkmpjFastMarchingAdvection*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjFastMarchingAdvection;
}

// Construct object with no children.
vtkmpjFastMarchingAdvection::vtkmpjFastMarchingAdvection()
{
  this->Mask = NULL;
  this->MaskOutValue = 0;
  this->Debug = 0;
  this->Seeds = NULL;
  this->State = NULL;
  this->queue = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjFastMarchingAdvection::~vtkmpjFastMarchingAdvection()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
  if (this->Seeds != NULL)
    this->Seeds->Delete();
  if (this->queue != NULL)
    this->queue->Delete();
  if (this->State != NULL)
    this->State->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingAdvection::ExecuteInformation()
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
int vtkmpjFastMarchingAdvection::InsideMask(int index)
{
  return (this->mask != NULL)?(this->mask->GetComponent(index, 0) > 0) : 1;
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingAdvection::F(vtkImageData *input, vtkImageData *output,
				      int coord[3], int index)
{
  return 1.0;
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingAdvection::ComputeTFirstOrder(vtkImageData *input, vtkImageData *output, int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  vtkDataArray *in=input->GetPointData()->GetScalars();
  vtkDataArray *out=output->GetPointData()->GetScalars();

  double vector[3];  
  double fa = (double)in->GetComponent(index, 0);
  vector[0] = (double)in->GetComponent(index, 1);  
  vector[1] = (double)in->GetComponent(index, 2);
  vector[2] = (double)in->GetComponent(index, 3);
  
  /*  printf("vector=%f,%f,%f\n",vector[0],vector[1],vector[2]);*/

  double coeff[3] = {0.0, 0.0, 0.0};   // coefficient array
  double a=0.0;
  double b=0.0;
  double c=0.0;
  double d=0.0;
  double e=0.0;

  // number of voxels in a row and slice
  int factor[3] = {1, dim[0], dim0Tdim1};

  // we must solve:
  //
  //  max(u, 0)*D^-x + min(u, 0)*D^+x +
  //  max(v, 0)*D^-y + min(v, 0)*D^+y +
  //  max(w, 0)*D^-z + min(w, 0)*D^+z = 1

  // process each one of the six neighbors
  for (int j=0; j<3; j++)  { // axis = { x, y, z }    

    double min_time = VTK_LARGE_FLOAT;
    int quadrant = 0;

    for (int i=-1; i<2; i+=2) { // direction +, -
      if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	
	int neighbor=index + i*factor[j];
	
	// if value is frozen get its time
	if (this->InsideMask(neighbor) && this->State->GetValue(neighbor) == VTK_MPJ_FROZEN) { 
	  
	  double time = (double)out->GetComponent(neighbor, 0);
	  	  
	  /*if (vector[j] > 0) {
	    a += vector[j];
	    b += vector[j] * -time;
	    } else {
	    a += -vector[j];
	    b += vector[j] * time;
	    }*/
	  
	  /* (i < 0) {  
	     double v = (vector[j] > 0) ? vector[j]:0.0;
	     a += v;
	     b -= v*time;
	     }
	     else	   
	     {
	     double v = (vector[j] < 0) ? vector[j]:0.0;
	     a -= v;
	     b += v*time;
	     }*/
	  
	  /*if (time < min_time) {
	    min_time = time;
	    quadrant = i;
	    }*/

	  //if (i < 0) {
	    a += vector[j];
	    b += -vector[j] * time;
	    //} else {
	    //a += -vector[j];
	    //b += vector[j] * time;
	    //}
	  
	} // if 
      } // if 
    } // for     
    
    /*    if (min_time != VTK_LARGE_FLOAT) {      
	  a += vector[j];
	  if (Debug) printf("min_time=%f, vector[%d]=%f, b'=%f\n",min_time,j,vector[j],-vector[j]*min_time);
	  b += -vector[j] * min_time;*/
    
      /*c += 1.0;
	d += -2*min_time;
	e += min_time*min_time;*/
    }

/*  coeff[2] = a*a;         
    coeff[1] = 2*a*b;       
    coeff[0] = (b*b - 1.0);
    
    if (Debug) printf("a=%f, b=%f\n",a,b);
    
    int no_solutions = 1;
    double max_sol = VTK_LARGE_FLOAT;
    double sol[2] = {VTK_LARGE_FLOAT, VTK_LARGE_FLOAT};
    double discr = coeff[1]*coeff[1] - 4.0*coeff[2]*coeff[0];   
    
    if (discr >= 0) {
    vtkMath::SolveQuadratic(coeff[2], coeff[1], coeff[0], 
    &sol[0], &sol[1], &no_solutions);
    if (no_solutions == 2) {
    max_sol = sol[0]>=sol[1]?sol[0]:sol[1];
    } else {
    max_sol = sol[0];
    }
    } else max_sol = VTK_LARGE_FLOAT;*/
  
  /*sol[0] = (1 - b) / a;
    printf("1: max_sol = %f\n", sol[0]);
    sol[1] = (1 + b) / -a;
    printf("2: max_sol = %f\n", sol[1]);      
    return (sol[0] > sol[1]? sol[0]:sol[1]);*/
  
  double f = 1.0;
 double max_sol = (1.0 - b) / a;

   if (this->Debug) {
     /*    printf("1: coeffs=%lf,%lf,%lf\n",coeff[2],coeff[1],coeff[0]);*/
     printf("1: sol=%lf\n",max_sol);
     /*    printf("1:discr=%f\n",discr);*/
     /*    printf("---------------\n");*/
   }
   
  return max_sol;
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingAdvection::ComputeNewTFirstOrder(vtkImageData *input, vtkImageData *output, int coord[3], int index)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  vtkDataArray *in=input->GetPointData()->GetScalars();
  vtkDataArray *out=output->GetPointData()->GetScalars();

  double vector[3];  
  double fa = (double)in->GetComponent(index, 0);
  vector[0] = (double)in->GetComponent(index, 1);  
  vector[1] = (double)in->GetComponent(index, 2);
  vector[2] = (double)in->GetComponent(index, 3);
  
  double coeff[3] = {0.0, 0.0, 0.0};   // coefficient array

  // number of voxels in a row and slice
  int factor[3] = {1, dim[0], dim0Tdim1};

  double time_x = -1;
  double time_y = -1;
  double time_z = -1;

  coeff[0] = -1;

  // x
  if ((coord[0]+1)>=0 && (coord[0]+1)<dim[0]) {
    int neighbor=index - 1*factor[0];    
    // if value is frozen get its time
    if (this->InsideMask(neighbor) && this->State->GetValue(neighbor) == VTK_MPJ_FROZEN) { 
      time_x = (double)out->GetComponent(neighbor, 0);
      double tmp_x = vector[0]*vector[0];
      coeff[2] += tmp_x;
      coeff[1] += -2*time_x*tmp_x;
      coeff[0] += time_x*time_x*tmp_x;
    }
  }

  // y
  if ((coord[1]+1)>=0 && (coord[1]+1)<dim[1]) {
    int neighbor=index - 1*factor[1];    
    // if value is frozen get its time
    if (this->InsideMask(neighbor) && this->State->GetValue(neighbor) == VTK_MPJ_FROZEN) { 
      time_y = (double)out->GetComponent(neighbor, 0);
      double tmp_y = vector[1]*vector[1];
      coeff[2] += tmp_y;
      coeff[1] += -2*time_y*tmp_y;
      coeff[0] += time_y*time_y*tmp_y;
      }
  }

  // z
  if ((coord[2]+1)>=0 && (coord[2]+1)<dim[2]) {
    int neighbor=index - 1*factor[2];    
    // if value is frozen get its time
    if (this->InsideMask(neighbor) && this->State->GetValue(neighbor) == VTK_MPJ_FROZEN) { 
      time_z = (double)out->GetComponent(neighbor, 0);
      double tmp_z = vector[2]*vector[2];
      coeff[2] += tmp_z;
      coeff[1] += -2*time_z*tmp_z;
      coeff[0] += time_z*time_z*tmp_z;
      }
  }

  // x-y
  if ((time_x>=0) && (time_y>=0)) {
    double tmp_xy = vector[0]*vector[1];
    coeff[2] += 2*tmp_xy;
    coeff[1] += -2*tmp_xy*(time_x+time_y);
    coeff[0] += time_x*time_y;
  }

  // x-z
  if ((time_x>=0) && (time_z>=0)) {
    double tmp_xz = vector[0]*vector[2];
    coeff[2] += 2*tmp_xz;
    coeff[1] += -2*tmp_xz*(time_x+time_z);
    coeff[0] += time_x*time_z;
  }

  // y-z
  if ((time_y>=0) && (time_z>=0)) {
    double tmp_yz = vector[1]*vector[2];
    coeff[2] += 2*tmp_yz;
    coeff[1] += -2*tmp_yz*(time_y+time_z);
    coeff[0] += time_y*time_z;
  }
  int no_solutions = 1;
  double max_sol = VTK_LARGE_FLOAT;
  double sol[2] = {VTK_LARGE_FLOAT, VTK_LARGE_FLOAT};
  double discr = coeff[1]*coeff[1] - 4.0*coeff[2]*coeff[0];   
  
  if (discr >= 0) {
     vtkMath::SolveQuadratic(coeff[2], coeff[1], coeff[0], 
			    &sol[0], &sol[1], &no_solutions);
     if (no_solutions == 2) {
       max_sol = sol[0]>=sol[1]?sol[0]:sol[1];
     } else {
       max_sol = sol[0];
     }
  } else max_sol = VTK_LARGE_FLOAT;

  if (Debug) {
    printf("time_x=%f, time_y=%f, time_z=%f\n",time_x,time_y,time_z);
    printf("coeff[2]=%f, coeff[1]=%f, coeff[0]=%f, sol=%f\n",coeff[2],coeff[1],coeff[0],max_sol);
  }
   
  return max_sol;
}

// ----------------------------------------------------------------------------
double vtkmpjFastMarchingAdvection::ComputeTime(vtkImageData *input, vtkImageData *output, 
					     int coord[3], int index)
{
  // compute time of arrival at neighbor
  double time = this->ComputeNewTFirstOrder(input, output, coord, index);
  
  // insert neighbor in the narrow band
  if (this->State->GetValue(index) != VTK_MPJ_NARROW_BAND) {
    this->State->SetValue(index, VTK_MPJ_NARROW_BAND);
    this->queue->Insert(time, index);
  } else {
    // if already in the narrow band, store new time
    this->queue->DeleteId(index);
    this->queue->Insert(time, index);
  }

  return time;
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingAdvection::InitializeFront(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  vtkDataArray *in=input->GetPointData()->GetScalars();
  vtkDataArray *out=output->GetPointData()->GetScalars();

  // Create priority queue
  this->queue = vtkmpjPriorityQueue::New();
  this->queue->Allocate(1000);

  // Create State array
  this->State = vtkUnsignedCharArray::New();
  this->State->Allocate(in->GetNumberOfTuples());
  this->State->FillComponent(0, VTK_MPJ_UNVISITED);
  out->FillComponent(0, VTK_LARGE_FLOAT);

  int index;
  int coord[3], ncoord[3];
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  double T;

  // Freeze seed point(s)
  for(int i=0; i<Seeds->GetNumberOfTuples(); i++) {

    coord[0] = ncoord[0] = (int)Seeds->GetComponent(i, 0);
    coord[1] = ncoord[1] = (int)Seeds->GetComponent(i, 1);
    coord[2] = ncoord[2] = (int)Seeds->GetComponent(i, 2);
    index = dim0Tdim1*coord[2] + dim[0]*coord[1] + coord[0];

    if (this->InsideMask(index)) {
      this->State->SetValue(index, VTK_MPJ_FROZEN);   // Freeze voxel
      out->SetComponent(index, 0, (float)0.0);        // Set arrival time to 0
      this->count++;
      
      this->Debug=1;

      // process each one of the six neighbors

      /********* changed into 2D **********/
      for (int j=0; j<3; j++) { // axis  
	for (int i=1; i<=1; i+=2)  { // direction	  	

	  if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  	    

	    int neighbor=index + i*factor[j];
	    ncoord[j] = coord[j] + i;   // update coordinate
	    
	    // compute arrival time
	    T = this->ComputeTime(input, output, ncoord, neighbor);

	    ncoord[j] = coord[j]; // reset
	  }
	}
      }

      this->Debug=0;
    } else {
      vtkErrorMacro(<<"Seed point "<<coord[0]<<","<<coord[1]<<","<<coord[2]<<" outside mask!");
    }
  }
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingAdvection::PropagateFront(vtkImageData *input, vtkImageData *output)
{
  double time;
  vtkDataArray *out=output->GetPointData()->GetScalars();

  int coord[3], ncoord[3];
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];

  // front propagation
  int index;
  while ((index = this->queue->Pop(time, 0)) != -1) {
    
    this->State->SetValue(index, VTK_MPJ_FROZEN);    
    out->SetComponent(index, 0, (float)time);
    printf("time=%f\n",time);

    this->count++;
    if (this->count==this->tenth)
      {
	this->pog+=0.1;
	this->UpdateProgress(this->pog);
	this->count=0;
      }
       
    // decompose index into x, y, z coordinates
    coord[2] = ncoord[2] = index / dim0Tdim1; 
    int r = index % dim0Tdim1;
    coord[1] = ncoord[1] = r / dim[0];
    coord[0] = ncoord[0] = r % dim[0];

    if (time == VTK_LARGE_FLOAT || time < 0) {
      /*printf("%d %d %d\n",coord[0],coord[1],coord[2]);*/
      continue;
    }
    
    // number of voxels in a row and slice
    int factor[3] = {1, dim[0], dim0Tdim1};

    // process each one of the six neighbors
    /* *********** changed into 2d ************/
    for (int j=0; j<3; j++)  // axis  
      for (int i=1; i<=1; i+=2)  { // direction 
	
	if ((coord[j]+i)>=0 && (coord[j]+i)<dim[j]) {	  
	  int neighbor=index + i*factor[j];
	  
	  if (this->InsideMask(neighbor) && this->State->GetValue(neighbor)!=VTK_MPJ_FROZEN) { 
	    
	    ncoord[j] = coord[j] + i;   // update coordinate

	    // compute arrival time
	    (void) this->ComputeTime(input, output, ncoord, neighbor);

	    ncoord[j] = coord[j]; // reset
	  }	  
	}
      }
    } // while  
}

// ----------------------------------------------------------------------------
void vtkmpjFastMarchingAdvection::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int dim[3];  input->GetDimensions(dim);
  int totaltuples;
  
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  // If mask exists, get number of voxels in mask
  if (this->Mask != NULL) {
    vtkImageAccumulate *accum=vtkImageAccumulate::New();
    accum->SetInput(this->Mask);
    accum->Update();  
    totaltuples = (int)(accum->GetOutput())->GetPointData()->GetScalars()->GetComponent(1, 0);
    accum->Delete();
  } else {
    totaltuples = input->GetPointData()->GetScalars()->GetNumberOfTuples();
  }
  
  // Get pointer to mask array
  this->mask = (this->Mask != NULL) ? this->Mask->GetPointData()->GetScalars() : NULL;
 
  this->pog = 0.0;  
  this->tenth=(int)(totaltuples / 10.0);
  this->count=0;

  // Initialize front
  this->InitializeFront(input, output);
  
  this->UpdateProgress(0.01);
  
  // Propagate front
  this->PropagateFront(input, output);
  
  this->UpdateProgress(1.0);

}
// ----------------------------------------------------------------------------
