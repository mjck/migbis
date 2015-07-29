

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



#include "vtkObjectFactory.h"
#include "vtkmpjImageMedialnessResponse.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkImageGradient.h"
#include "vtkMath.h"
#include "vtkCell.h"
#include "vtkIdType.h"
#include "vtkVoxel.h"

//------------------------------------------------------------------------------
vtkmpjImageMedialnessResponse* vtkmpjImageMedialnessResponse::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageMedialnessResponse");
  if(ret)
    {
      return (vtkmpjImageMedialnessResponse*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageMedialnessResponse;
}

// Construct object with no children.
vtkmpjImageMedialnessResponse::vtkmpjImageMedialnessResponse()
{
  this->Radius = 1.0;
  this->Hessian = NULL;
  this->CalculateGradient = 0;
  this->NumberOfCirclePoints = 10;
}

// ----------------------------------------------------------------------------
vtkmpjImageMedialnessResponse::~vtkmpjImageMedialnessResponse()
{
  if (this->Hessian)
    this->Hessian->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageMedialnessResponse::SortEigenvectors(float w[3], float V[3][3])
{
  int i, j, k;
  float tmp, tmp2;
   
  // sort eigenfunctions by decreasing magnitude
  for (j=0; j<3; j++)
    {
      k = j;
      tmp = w[k];
      tmp2 = fabs(tmp);
      for (i=j; i<3; i++)
        {
          if (fabs(w[i]) >= tmp2)
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
void vtkmpjImageMedialnessResponse::ComputeEigenSystem(vtkDataArray *hess, int index, float lambda[3], float W[3][3])
{
  float A[3][3];

  A[0][0]=hess->GetComponent(index,0);
  A[0][1]=hess->GetComponent(index,1);
  A[1][0]=A[0][1];
  
  A[0][2]=hess->GetComponent(index,2);
  A[2][0]=A[0][2];
  
  A[1][1]=hess->GetComponent(index,3);
  A[1][2]=hess->GetComponent(index,4);
  A[2][1]=A[1][2];
       
  A[2][2]=hess->GetComponent(index,5);

  // diagonalize hessian matrix
  vtkMath::Diagonalize3x3(A,lambda,W);
 
  // sort eigenvectors by decreasing magnitude
  this->SortEigenvectors(lambda, W);
}

// ----------------------------------------------------------------------------
void vtkmpjImageMedialnessResponse::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();
  output->SetScalarTypeToFloat();
}

// ----------------------------------------------------------------------------
float vtkmpjImageMedialnessResponse::ComputeMedialness(float point[3], float radius, float v1[3], float v2[3], vtkDataArray *scalars, vtkDataArray *cell_scalars)
{  
  // cell info
  float tol2 = this->GetInput()->GetLength() / 1000.0;
  tol2 = tol2 * tol2;

  vtkCell *cell;
  vtkIdType cellId;
  int subId=0;
  float pcoords[3];
  float weights[8];

  // find cell corresponding to point
  cellId = this->GetInput()->FindCell(point, NULL, -1, 0.0, subId, pcoords, weights);
  
  // retrieve cell
  cell = this->GetInput()->GetCell(cellId);

  float total = 0.0;  
  float value;

  int i;
  float ncp=this->NumberOfCirclePoints;
  float cpoint[3];
  float alpha;
  float v[3];
  
  float deriv[3];

  //vtkFloatArray *cellscalars = vtkFloatArray::New();
  //cellscalars->SetNumberOfTuples(8);
  //cellscalars->SetNumberOfComponents(3);
  
  float sum = 0;

  // compute gradient
  for (int m=0; m<this->NumberOfCirclePoints; m++) {
    
    alpha = 2.0 * vtkMath::Pi() * float(m) / ncp;
    
    // plane defined by the two negative eigenvectors
    v[0] = cos(alpha)*v1[0] + sin(alpha)*v2[0];
    v[1] = cos(alpha)*v1[1] + sin(alpha)*v2[1];
    v[2] = cos(alpha)*v1[2] + sin(alpha)*v2[2];
    
    // point on the circle at given radius
    cpoint[0] = this->Radius*v[0] + point[0];
    cpoint[1] = this->Radius*v[1] + point[1];
    cpoint[2] = this->Radius*v[2] + point[2];
    
    // find cell corresponding to new point 
    cellId = this->GetInput()->FindCell(cpoint, cell, cellId, tol2, subId, pcoords, weights);
    
    // if cell inside volume compute derivatives
    if ( cellId > 0 ) {
      
      cell = this->GetInput()->GetCell(cellId);
      
      // fetch gradient values
      scalars->GetTuples(cell->PointIds, cell_scalars);
      
      // compute gradient at specific point
      deriv[0] = deriv[1] = deriv[2] = 0.0;
      for(int j=0; j<3; j++) {
	for (int i=0; i < cell->GetNumberOfPoints(); i++)
	  deriv[j] += cell_scalars->GetComponent(i, j) * weights[i];
	//deriv[j] += scalars->GetComponent(cell->GetPointId(i),j) * weights[i];	
      }
      
      // derivatives
      sum += deriv[0]*v[0] + deriv[1]*v[1] + deriv[2]*v[2];
      
      // normalize derivative ?
      //vtkMath::Normalize(deriv);
      
      //if (deriv[0] != 0 || deriv[1] != 0 || deriv[2] != 0)
      //printf("coord = %f,%f,%f, circle point %d at %f,%f,%f [%f,%f,%f] %d d=[%f,%f,%f]\n",
      //       point[0],point[1],point[2],m,cpoint[0],cpoint[1],cpoint[2],
      //       pcoords[0],pcoords[1],pcoords[2],cellId,deriv[0],deriv[1],deriv[2]);
      
      total++;
      
      //    } else {      
      //printf("out of bounds %f,%f,%f\n",point[0],point[1],point[2]);      
      //    }	   
    }
    
  }

  value = sum / total;

  return value;
}

 
void vtkmpjImageMedialnessResponse::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"No Input Image Data !!");
      return;
    }
  
  int dim[3]; input->GetDimensions(dim);
  float sp[3]; input->GetSpacing(sp);
  float org[3]; input->GetOrigin(org);

  vtkDataArray *out=output->GetPointData()->GetScalars();

  printf("dim=%d,%d,%d\n",dim[0],dim[1],dim[2]);

  if (this->Hessian==NULL) 
    {
      vtkErrorMacro(<<"No Hessian Image Data !!");
      return;
    }
  
  if (this->CalculateGradient == 0) {
    if (input->GetNumberOfScalarComponents()!=3)
      {
	vtkErrorMacro(<<"Invalid gradient image. Not enough components\n");
	return;
      }
  }
  
  vtkImageData *gradient = vtkImageData::New();

  // compute gradient if necessary
  if (this->CalculateGradient) {

    vtkImageGradient *grad = vtkImageGradient::New();
    grad->SetInput(input);
    grad->SetDimensionality(3);
    grad->Update();    
    gradient->ShallowCopy(grad->GetOutput());
    grad->Delete();
    
  } else {
    
    gradient->ShallowCopy(input);

  }
  
  // get gradient scalars
  vtkDataArray *scalars = gradient->GetPointData()->GetScalars();
  
  // construct cell scalars
  vtkFloatArray *cell_scalars = vtkFloatArray::New();
  cell_scalars->SetNumberOfComponents(3);
  cell_scalars->SetNumberOfTuples(8);
  //  cell_scalars->Allocate();
  
  // get hessian matrix
  vtkDataArray *hessian=this->GetHessian()->GetPointData()->GetScalars();
  
  // Progress computation
  int numscalars=input->GetNumberOfPoints();
  
  int count = 0;
  float pog = 0.0;
  int tenth= (int)(numscalars / 10.0);
  
  int index = 0;
  float point[3];
  float lambda[3], W[3][3];
  
  // testing plane x-y
  float v1[3], v2[3];
  v1[0]=1.0; v1[1]=0.0; v1[2]=0.0;
  v2[0]=0.0; v2[1]=1.0; v2[2]=0.0;

  float value;

  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++) 
      for (int i=0;i<dim[0];i++)
	{
	  // compute eigen system
	  this->ComputeEigenSystem(hessian,index,lambda,W);	  
	  
	  v1[0]=W[0][0]; v1[1]=W[1][0]; v1[2]=W[2][0];
	  v2[0]=W[0][1]; v2[1]=W[1][1]; v2[2]=W[2][1];
	  
	  point[0] = i*sp[0] + org[0]; 
	  point[1] = j*sp[1] + org[1]; 
	  point[2] = k*sp[2] + org[2];
	  
	  // compute medialness
	  value = this->ComputeMedialness(point, this->Radius, v1, v2, scalars, cell_scalars);
	  
	  //printf("value=%f\n",value);
	  out->SetComponent(index, 0, fabs(value));

	  count++;
	  
	  if (count==tenth)
	    {
	      pog+=0.1;
	      this->UpdateProgress(pog);
	      count=0;
	    }
	  
	  index++;
	}
  
  gradient->Delete();

  cell_scalars->Delete();
  
  this->UpdateProgress(1.0);
}

