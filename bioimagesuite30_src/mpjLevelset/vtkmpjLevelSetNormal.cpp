

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


#include "vtkmpjLevelSetNormal.h"

//------------------------------------------------------------------------------
vtkmpjLevelSetNormal* vtkmpjLevelSetNormal::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjLevelSetNormal");
  if(ret)
    {
      return (vtkmpjLevelSetNormal*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjLevelSetNormal;
}

// Construct object with no children.
vtkmpjLevelSetNormal::vtkmpjLevelSetNormal()
{
  this->Mask = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjLevelSetNormal::~vtkmpjLevelSetNormal()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetNormal::ExecuteInformation()
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
  output->SetScalarType(VTK_DOUBLE);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
int vtkmpjLevelSetNormal::InsideMask(int index)
{
  return (this->mask != NULL)?(this->mask->GetComponent(index, 0) > 0) : 1;
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetNormal::ComputeNormal(vtkImageData *input, vtkImageData *output,
					 int coord[3], int index, double result[3])
{
  vtkDataArray *in=input->GetPointData()->GetScalars();
  
  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  
  // number of voxels in a row and slice
  int factor[3] = {1, dim[0], dim0Tdim1};
  double D[3][2];
  
  // phi value and position index
  double value1 = in->GetComponent(index, 0);

  for (int j=0; j<3; j++)  // axis  
    for (int i=-1; i<2; i+=2) // direction 
      {		
	int neighbor=index + (int)i*factor[j];	
	int i1 = i<0?0:i;

	D[j][i1] = VTK_LARGE_FLOAT;
	
	if ((coord[j]+i >= 0) && (coord[j]+i < dim[j]) && (this->InsideMask(neighbor))) {	  
	  double value2 = in->GetComponent(neighbor, 0);
	  D[j][i1] = ((double)-i)*value1 + ((double)i)*value2;
	}
	
      }

  double Dxm = D[0][0];
  double Dxp = D[0][1];
  double Dym = D[1][0];
  double Dyp = D[1][1];
  double Dzm = D[2][0];
  double Dzp = D[2][1];
  
  double n[8][3] ={{Dxp, Dyp, Dzp},
		  {Dxp, Dym, Dzp},
		  {Dxm, Dyp, Dzp},
		  {Dxm, Dym, Dzp},
		  {Dxp, Dyp, Dzm},
		  {Dxp, Dym, Dzm},
		  {Dxm, Dyp, Dzm},
		  {Dxm, Dym, Dzm}};
  
  double aux[3];
  double normal[3];
  
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
  normal[0] /= (double)count;
  normal[1] /= (double)count;
  normal[2] /= (double)count;
    
  //printf("normal= %f,%f,%f\n",normal[0],normal[1],normal[2]);

  result[0] = normal[0];
  result[1] = normal[1];
  result[2] = normal[2];
}

// ----------------------------------------------------------------------------
void vtkmpjLevelSetNormal::SimpleExecute(vtkImageData *input, vtkImageData *output)
{  
  int dim[3]; input->GetDimensions(dim);
  vtkDataArray *out = output->GetPointData()->GetScalars();
  vtkDataArray *in=input->GetPointData()->GetScalars();

  // Get pointer to mask array
  this->mask = (this->Mask != NULL) ? this->Mask->GetPointData()->GetScalars() : NULL;
  
  int index = 0;
  int coord[3];
  double normal[3];

  for (int k=0;k<dim[2];k++) {    
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  coord[0]=i; coord[1]=j; coord[2]=k;
	  normal[0]=0; normal[1]=0; normal[2]=0;

	  // compute normal
	  this->ComputeNormal(input, output, coord, index, normal);
	  
	  out->SetComponent(index, 0, normal[0]);
	  out->SetComponent(index, 1, normal[1]);
	  out->SetComponent(index, 2, normal[2]);
	  
	  ++index;	  
	}  	  
    
    double pog=double(k)/double(dim[2]);
    this->UpdateProgress(pog);	    
  }
}
// ----------------------------------------------------------------------------
