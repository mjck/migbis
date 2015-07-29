

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


#include "vtkmpjDistanceMapNormals.h"

//------------------------------------------------------------------------------
vtkmpjDistanceMapNormals* vtkmpjDistanceMapNormals::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjDistanceMapNormals");
  if(ret)
    {
      return (vtkmpjDistanceMapNormals*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjDistanceMapNormals;
}

// Construct object with no children.
vtkmpjDistanceMapNormals::vtkmpjDistanceMapNormals()
{
  this->Mask = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjDistanceMapNormals::~vtkmpjDistanceMapNormals()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjDistanceMapNormals::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  int dim[3];  input->GetDimensions(dim);

  output->SetNumberOfScalarComponents(1);  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
int vtkmpjDistanceMapNormals::InsideMask(int index)
{
  return (this->mask != NULL)?(this->mask->GetComponent(index, 0) > 0) : 1;
}

// ----------------------------------------------------------------------------
void vtkmpjDistanceMapNormals::ComputeNormals(vtkImageData *input, vtkImageData *output,
					     int coord[3], int index)
{
  vtkDataArray *out=output->GetPointData()->GetScalars();
  vtkDataArray *in=output->GetPointData()->GetScalars();

  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  
  // number of voxels in a row and slice
  int factor[3] = {1, dim[0], dim0Tdim1};
  float D[3][2]={{0, 0},{0, 0},{0, 0}};

  // reset normal
  float normal[3];
  normal[0]=0.0; normal[1]=0.0; normal[2]=0.0;
  
  for (int j=0; j<3; j++)  // axis  
    for (int i=-1; i<2; i+=2) // direction 
      {	
	int neighbor=index + i*factor[j];	
	if ((coord[j]+i >= 0) && (coord[j]+i < dim[j]) && (this->InsideMask(neighbor))) {
	  D[j][i] = ((float)-i)*(in->GetComponent(index, 0)) + ((float)i)*(in->GetComponent(neighbor, 0));
	  printf("D=%f\n",D[j][i]);
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
  
  for(int i=0; i<8; i++) {
    float nn[3];
    nn[0]=n[i][0]; nn[1]=n[i][1]; nn[2]=n[i][2];
    vtkMath::Normalize(nn);
    normal[0] += nn[0];
    normal[1] += nn[1];
    normal[2] += nn[2];
    printf("nn=%f,%f,%f\n",nn[0],nn[1],nn[2]);
  }

  // average normal 
  normal[0] = -normal[0] / 8.0;
  normal[1] = -normal[1] / 8.0;
  normal[2] = -normal[2] / 8.0;
    
  out->SetComponent(index, 0, normal[0]);
  out->SetComponent(index, 1, normal[1]);
  out->SetComponent(index, 2, normal[2]);
  
  printf("normal= %f,%f,%f\n",normal[0],normal[1],normal[2]);
}


void vtkmpjDistanceMapNormals::ComputeMap(vtkImageData *input, vtkImageData *output,
					  int coord[3], int index)
{
  vtkDataArray *out=output->GetPointData()->GetScalars();
  vtkDataArray *in=output->GetPointData()->GetScalars();

  int dim[3]; input->GetDimensions(dim);
  int dim0Tdim1 = dim[0]*dim[1];
  
  // number of voxels in a row and slice
  int factor[3] = {1, dim[0], dim0Tdim1};

  double vector1[3];
  double vector2[3];

  vector1[0] = (double)in->GetComponent(index, 0);  
  vector1[1] = (double)in->GetComponent(index, 1);
  vector1[2] = (double)in->GetComponent(index, 2);
  vector2[0] = (double)in->GetComponent(index, 3);  
  vector2[1] = (double)in->GetComponent(index, 4);
  vector2[2] = (double)in->GetComponent(index, 5);

  (void) vtkMath::Normalize(vector1);
  (void) vtkMath::Normalize(vector2);
  
  double dot=vector1[0]*vector2[0] + vector1[1]*vector2[1] + vector1[2]*vector2[2];

  out->SetComponent(index, 0, dot*dot);
}

// ----------------------------------------------------------------------------
void vtkmpjDistanceMapNormals::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int dim[3]; input->GetDimensions(dim);
  int index=0;
    
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  // Get pointer to mask array
  this->mask = (this->Mask != NULL) ? this->Mask->GetPointData()->GetScalars() : NULL;
  
  for (int k=0;k<dim[2];k++) {    
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  int coord[3]={i, j, k};
	  ComputeMap(input, output, coord, index);
	  
	  ++index;	  
	}  	  
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }

  printf("Done with map!\n");

}
// ----------------------------------------------------------------------------
