

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


#include "vtkmpjLevelSetMethod.h"

//------------------------------------------------------------------------------
vtkmpjLevelSetMethod* vtkmpjLevelSetMethod::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjLevelSetMethod");
  if (ret) {
    return (vtkmpjLevelSetMethod*)ret;
  }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjLevelSetMethod;
}

// Construct object with no children.
vtkmpjLevelSetMethod::vtkmpjLevelSetMethod()
{
  this->TimeStepSize = 0.5;
  this->NarrowBandwidth = 12;
  this->PropagationTime = VTK_LARGE_FLOAT;
  this->Spacing = 1.0;
  this->InputImage = NULL;
  this->OutputImage = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjLevelSetMethod::~vtkmpjLevelSetMethod()
{
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetMethod::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  //output->SetNumberOfScalarComponents(1);  
  //output->SetScalarType(VTK_FLOAT);
  //output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
double vtkmpjLevelSetMethod::ComputeEntropyGradientMagnitude(vtkImageData *input, double speed,
							     int index, int coord[3])
{
  vtkDataArray *phi = input->GetPointData()->GetScalars();  
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  int factor[3] = {1, dim[0], dim0Tdim1};
  
  double magnitude = 0.0;    
  double value = input->GetPointData()->GetScalars()->GetComponent(index, 0);
  double diff_val = 0.0;
  
  for(int j=0; j<3; j++) {
    
    // compute backward differences
    if (coord[j] > 0) {      

      diff_val = value - phi->GetComponent(index-factor[j], 0);
      
      if ((speed > 0 && diff_val > 0) ||
	  (speed < 0 && diff_val < 0)) {
	
	magnitude += diff_val * diff_val;
      }
    }

    // compute forward differences
    if (coord[j] < dim[j]-1) {
      
      diff_val = phi->GetComponent(index+factor[j], 0) - value;
      
      if ((speed > 0 && diff_val < 0) ||
	  (speed < 0 && diff_val > 0)) {
	
	magnitude += diff_val * diff_val;
      }
    }
  }

  magnitude = sqrt(magnitude);

  return magnitude;
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetMethod::ComputeNormal(vtkImageData *input, int index, int coord[3], float result[3])
{
  vtkDataArray *in=input->GetPointData()->GetScalars();
  
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  
  // number of voxels in a row and slice
  int factor[3] = {1, dim[0], dim0Tdim1};
  float D[3][2];
  
  // phi value and position index
  float value1 = in->GetComponent(index, 0);

  for (int j=0; j<3; j++)  // axis  
    for (int i=-1; i<2; i+=2) // direction 
      {		
	int neighbor=index + (int)i*factor[j];	
	int i1 = i<0?0:i;

	D[j][i1] = VTK_LARGE_FLOAT;
	
	if ((coord[j]+i >= 0) && (coord[j]+i < dim[j])) {	  
	  float value2 = in->GetComponent(neighbor, 0);
	  D[j][i1] = ((float)-i)*value1 + ((float)i)*value2;
	}
	
      }

  float Dxm = D[0][0];
  float Dxp = D[0][1];
  float Dym = D[1][0];
  float Dyp = D[1][1];
  float Dzm = D[2][0];
  float Dzp = D[2][1];
  
  float n[8][3] ={{Dxp, Dyp, Dzp},
		  {Dxp, Dym, Dzp},
		  {Dxm, Dyp, Dzp},
		  {Dxm, Dym, Dzp},
		  {Dxp, Dyp, Dzm},
		  {Dxp, Dym, Dzm},
		  {Dxm, Dyp, Dzm},
		  {Dxm, Dym, Dzm}};
  
  float aux[3];
  float normal[3];
  
  normal[0]=0.0; normal[1]=0.0; normal[2]=0.0;

  int count = 0;
  for(int k=0; k<8; k++) {
   
    if (n[k][0] == VTK_LARGE_FLOAT || 
	n[k][1] == VTK_LARGE_FLOAT ||
	n[k][2] == VTK_LARGE_FLOAT)
      continue;

    aux[0]=n[k][0]; 
    aux[1]=n[k][1]; 
    aux[2]=n[k][2];
    
    vtkMath::Normalize(aux);
    
    normal[0] += aux[0];
    normal[1] += aux[1];
    normal[2] += aux[2];

    count++;
  }

  // average normal 
  normal[0] /= (float)count;
  normal[1] /= (float)count;
  normal[2] /= (float)count;
    
  result[0] = normal[0];
  result[1] = normal[1];
  result[2] = normal[2];
}

// ----------------------------------------------------------------------------
double vtkmpjLevelSetMethod::ComputeSpeed(vtkImageData *input, int index, int coord[3])
{
  double speed = this->ComputeEntropyGradientMagnitude(input, 1.0, index, coord);
  return speed;
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetMethod::Initialize(vtkImageData *input, vtkImageData *output)
{    
  this->InputImage = input;
  this->OutputImage = output;
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetMethod::Propagate(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  //int factor[3] = {1, dim[0], dim0Tdim1};
  int total = input->GetPointData()->GetScalars()->GetNumberOfTuples();
  int coord[3];

  double time = 0.0;

  while (time <= this->PropagationTime) {
    
    vtkDataArray *in = this->InputImage->GetPointData()->GetScalars();
    vtkDataArray *out = this->OutputImage->GetPointData()->GetScalars();
    
    this->UpdateProgress(time * this->tenth);
    
    printf("time=%f\n",time);

    for(int index=0; index < total; index++) {

      // decompose index into point coordinates
      coord[2] = index / dim0Tdim1; 
      coord[1] = (index % dim0Tdim1) / dim[0];
      coord[0] = (index % dim0Tdim1) % dim[0];

      // compute propagation speed
      double speed = this->ComputeSpeed(this->InputImage, index, coord);
      
      // compute new phi value
      double newphi =  in->GetComponent(index, 0) - this->TimeStepSize * speed;
      out->SetComponent(index, 0, (float)newphi);
      
    }
    
    // advance time
    time += this->TimeStepSize;
    
    this->SwapImages();
  }
  
  this->SwapImages();

  this->CopyImage(this->OutputImage, output);
  //  output->DeepCopy(this->OutputImage);  
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetMethod::CopyImage(vtkImageData *source, vtkImageData *dest)
{
  vtkDataArray *src = source->GetPointData()->GetScalars();
  vtkDataArray *dst = dest->GetPointData()->GetScalars();
  int total = src->GetNumberOfTuples();
  for (int j=0; j<total; j++) {
    dst->SetComponent(j, 0, src->GetComponent(j, 0));
  }
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetMethod::SwapImages()
{
  vtkImageData *temp = this->OutputImage;
  this->OutputImage = this->InputImage;
  this->InputImage = temp;
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetMethod::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  this->UpdateProgress(0.01);
  
  this->tenth=(int)(this->PropagationTime / this->TimeStepSize);

  this->Initialize(input, output);  
  this->Propagate(input, output);

  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
