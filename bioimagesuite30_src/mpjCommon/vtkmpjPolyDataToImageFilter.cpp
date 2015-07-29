/*=========================================================================

  Program:   vtkmpjPolyDataToImageFilter.cpp

  Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkmpjPolyDataToImageFilter.h"

#include "vtkMath.h"
#include "vtkImageData.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkSmartPointer.h"
#include "vtkStreamingDemandDrivenPipeline.h"

#include <math.h>

vtkStandardNewMacro(vtkmpjPolyDataToImageFilter);

//----------------------------------------------------------------------------
vtkmpjPolyDataToImageFilter::vtkmpjPolyDataToImageFilter()
{
  this->SetNumberOfInputPorts(2);
  this->SetNumberOfOutputPorts(2); 
  this->Epsilon = 0.03;
}

//----------------------------------------------------------------------------
void vtkmpjPolyDataToImageFilter::SetPolyData(vtkPolyData *polydata)
{
  this->SetInput(1, polydata); 
}

//----------------------------------------------------------------------------
vtkPolyData *vtkmpjPolyDataToImageFilter::GetPolyData()
{
  if (this->GetNumberOfInputConnections(1) < 1)
    {
    return NULL;
    }
  else
    {
    return vtkPolyData::SafeDownCast(
      this->GetExecutive()->GetInputData(1, 0));
    }
}

//----------------------------------------------------------------------------
void vtkmpjPolyDataToImageFilter::ConvertCoord(vtkImageData *image, double x[3], double c[3])
{
    int r, dim[3];
    vtkIdType index;
    image->GetDimensions(dim);

    // find index
    index = image->FindPoint(x);

    // decompose index into x, y, z coordinates
    c[2] = index / (dim[0]*dim[1]); 
    r = index % (dim[0]*dim[1]);
    c[1] = r / dim[0];
    c[0] = r % dim[0];
}

int vtkmpjPolyDataToImageFilter::BarycentricCoords(double x[3], double x1[3], double x2[3], double x3[3], double bcoords[3])
{
  double *A[3], p[3], a1[3], a2[3], a3[3];
  int i;

  // Homogenize the variables; load into arrays.
  //
  a1[0] = x1[0]; 
  a1[1] = x2[0]; 
  a1[2] = x3[0]; 

  a2[0] = x1[1]; 
  a2[1] = x2[1]; 
  a2[2] = x3[1]; 

  a3[0] = x1[2]; 
  a3[1] = x2[2]; 
  a3[2] = x3[2]; 

  p[0] = x[0]; 
  p[1] = x[1]; 
  p[2] = x[2];

  //   Now solve system of equations for barycentric coordinates
  //
  A[0] = a1;
  A[1] = a2;
  A[2] = a3;

  if ( vtkMath::SolveLinearSystem(A,p,3) )
    {
      for (i=0; i<3; i++)
      {
        bcoords[i] = p[i];
      }
      return 1;
    }
  else
    {
      return 0;
    }
}



//----------------------------------------------------------------------------
void vtkmpjPolyDataToImageFilter::DrawLine(vtkImageData *output1, vtkImageData *output2, double a[3], double b[3], double c[3], 
                                            double x1[3], double x2[3], double m1, double m2, double m3)
{
  double p[3],d[3],grad[3],val,value=0;
  double coords[3] = {1, 0, 0};
  double maxdist=0.0;
  int k, maxindex=0;
  int dim[3];
  vtkIdType index;
  output1->GetDimensions(dim);

  //inPts->GetPoint(a,x1); //m1 = map->GetTuple1(pts[i-1]);
  //inPts->GetPoint(b,x2); // m2 = map->GetTuple1(pts[i]);
  
  for (k=0;k<=2;k++)
  {
    d[k]=x2[k]-x1[k];
    if (fabs(d[k])>fabs(maxdist))
    {
      maxdist=d[k];
      maxindex=k;
    }
  }
  
  for (k=0;k<=2;k++)
  {
    if (d[k]!=0.0)
      grad[k]=d[k]/maxdist;
    else
      grad[k]=0.0;
  }
    
  for (float dt=0.0;dt<fabs(maxdist);dt+=0.25)
    {
      if (maxdist==0)
      {
        for (k=0;k<=2;k++) 
          p[k]=x1[k]; 
        //value = m1;
      }
      else
      {
        for (k=0;k<=2;k++)
          p[k]=x1[k]+(dt/fabs(maxdist))*d[k];
        //value=m1+(dt/fabs(maxdist))*(m2-m1);
      }
      
      // find point location
      index = output1->FindPoint(p);
      BarycentricCoords(p, a, b, c, coords);
      value = coords[0] * m1 + coords[1] * m2 + coords[2] * m3;

      if (index >= 0) {
          //val = out->GetComponent(index, 0);
          //val = output2->GetPointData()->GetScalars()->GetComponent(index, 0);
          output1->GetPointData()->GetScalars()->SetComponent(index, 0, 1);
          //if (value > val)
          output2->GetPointData()->GetScalars()->SetComponent(index, 0, value);
      }
    
    }

}
//----------------------------------------------------------------------------
int vtkmpjPolyDataToImageFilter::RequestInformation (
  vtkInformation * vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
 // Get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkInformation *outInfo1 = outputVector->GetInformationObject(1);
 
  // Get the input and ouptut
  //vtkImageData *input = vtkImageData::SafeDownCast(
  //    inInfo->Get(vtkDataObject::DATA_OBJECT()));
 
  //vtkImageData *output = vtkImageData::SafeDownCast(
  //    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  int extent[6];
  double spacing[3];
  double origin[3];

  inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent);
  inInfo->Get(vtkDataObject::SPACING(), spacing);
  inInfo->Get(vtkDataObject::ORIGIN(), origin);

  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent, 6);
  outInfo->Set(vtkDataObject::SPACING(), spacing, 3);
  outInfo->Set(vtkDataObject::ORIGIN(), origin, 3);

  outInfo1->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent, 6);
  outInfo1->Set(vtkDataObject::SPACING(), spacing, 3);
  outInfo1->Set(vtkDataObject::ORIGIN(), origin, 3);

  //printf( "spc=%lf, %lf, %lf\n",spacing[0] ,spacing[1], spacing[2]);
  //printf( "org=%lf, %lf, %lf\n",origin[0] ,origin[1], origin[2]);

  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo1, VTK_FLOAT, -1);

  return 1;
}

//----------------------------------------------------------------------------
int vtkmpjPolyDataToImageFilter::RequestData(
  vtkInformation* vtkNotUsed(request),
  vtkInformationVector** inputVector,
  vtkInformationVector* outputVector)
{
  
    // get the background input
  vtkInformation* inInfo0 = inputVector[0]->GetInformationObject(0);
  vtkInformation* inInfo1 = inputVector[1]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkInformation *outInfo1 = outputVector->GetInformationObject(1);

  // that's the background image
  vtkImageData *input = vtkImageData::SafeDownCast(
    inInfo0->Get(vtkDataObject::DATA_OBJECT()));

  vtkPolyData *poly = vtkPolyData::SafeDownCast(
    inInfo1->Get(vtkDataObject::DATA_OBJECT()));

  // get the output
  vtkImageData *output = vtkImageData::SafeDownCast(
      outInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkImageData *output1 = vtkImageData::SafeDownCast(
      outInfo1->Get(vtkDataObject::DATA_OBJECT()));

  vtkSmartPointer<vtkImageData> image =
    vtkSmartPointer<vtkImageData>::New();
  image->DeepCopy(input);

  //image->SetScalarComponentFromDouble(0,0,0,0,100.0);
  output->ShallowCopy(input);
  output1->ShallowCopy(image);
  output1->GetPointData()->GetScalars()->FillComponent(0, 0);

  printf("Number of points = %lld\n", poly->GetNumberOfPoints());
  printf("Number of triangles = %lld\n", poly->GetNumberOfCells());
  printf("Epsilon = %lf\n", this->Epsilon);

  vtkIdType numberOfCellArrays = poly->GetCellData()->GetNumberOfArrays();
  std::cout << "Number of CellData arrays: " << numberOfCellArrays << std::endl;

  vtkIdType numberOfPointArrays = poly->GetPointData()->GetNumberOfArrays();
  std::cout << "Number of PointData arrays: " << numberOfPointArrays << std::endl;

  for(vtkIdType i = 0; i < numberOfPointArrays; i++)
    {
    // The following two lines are equivalent
    //arrayNames.push_back(polydata->GetPointData()->GetArray(i)->GetName());
    //arrayNames.push_back(polydata->GetPointData()->GetArrayName(i));
    int dataTypeID = poly->GetPointData()->GetArray(i)->GetDataType();
    std::cout << "Array " << i << ": " << poly->GetPointData()->GetArrayName(i)
              << " (type: " << dataTypeID << ")" << std::endl;
    }

  for(vtkIdType i = 0; i < numberOfCellArrays; i++)
    {
    // The following two lines are equivalent
    //polydata->GetPointData()->GetArray(i)->GetName();
    //polydata->GetPointData()->GetArrayName(i);
    int dataTypeID = poly->GetCellData()->GetArray(i)->GetDataType();
    std::cout << "Array " << i << ": " << poly->GetCellData()->GetArrayName(i)
              << " (type: " << dataTypeID << ")" << std::endl;
    }

  vtkCellArray *inFaces = poly->GetPolys();
  vtkPointData *pdpoly = poly->GetPointData();
  //tkCellData *pdpoly = poly->GetCellData();
  vtkDataArray *scalars = NULL;
  if (pdpoly) 
    scalars = pdpoly->GetScalars();

  vtkIdType npts=0, *pts=NULL;
  int i, j, k, abort=0;
  vtkIdType inCellId;

  vtkPoints *inPts = NULL;
  vtkCell *cell;
  vtkIdType numFaces = 0;
  vtkIdType numPts = 0;
  if (!(inPts = poly->GetPoints()) || 
       (numPts = inPts->GetNumberOfPoints()) < 1 ||
       !(inFaces = poly->GetPolys()) || 
       (numFaces = inFaces->GetNumberOfCells()) < 1 )
  {
      return 1;
  }

  //double x[3], x0[3],x1[3], x2[3], p[3], va, vb, vc;
  //double a[3], b[3], c[3], p1[3], p2[3], bcoords[3];
  //double spc[3], org[3];
  double v0, v1, v2;
  double x0[3], x1[3], x2[3];
  double p1[3], p2[3];
  //int min[3], max[3], dim[3];

  //image->GetSpacing(spc);
  //image->GetOrigin(org);
  //image->GetDimensions(dim);

  for (inCellId=0, inFaces->InitTraversal(); 
       inFaces->GetNextCell(npts,pts) && !abort; inCellId++)
  {
    this->UpdateProgress((float)inCellId/numFaces);
    abort = this->GetAbortExecute();

    // determine enclosing box iin image coordinates
    //inPts->GetPoint(pts[2], x); 
    //va = scalars->GetTuple1(pts[0]);
    //a[0] = (x[0] - org[0]) / spc[0]; 
    //a[1] = (x[1] - org[1]) / spc[1];
    //a[2] = (x[2] - org[2]) / spc[2];
  
    //inPts->GetPoint(pts[1], x); 
    //vb = scalars->GetTuple1(pts[1]);
    //b[0] = (x[0] - org[0]) / spc[0]; 
    //b[1] = (x[1] - org[1]) / spc[1];
    //b[2] = (x[2] - org[2]) / spc[2];

    //inPts->GetPoint(pts[0], x);       
    //vc = scalars->GetTuple1(pts[2]);
    //c[0] = (x[0] - org[0]) / spc[0]; 
    //c[1] = (x[1] - org[1]) / spc[1];
    //c[2] = (x[2] - org[2]) / spc[2];  

    inPts->GetPoint(pts[0], x0);
    inPts->GetPoint(pts[1], x1);
    inPts->GetPoint(pts[2], x2);
    v0 = scalars->GetTuple1(pts[0]);
    v1 = scalars->GetTuple1(pts[1]);
    v2 = scalars->GetTuple1(pts[2]);

    //double c[3];
    //this->ConvertCoord(image, x0, c);
    //printf("%lf %lf %lf\n", c[0], c[1], c[2]);
    //this->ConvertCoord(image, x1, c);
    //printf("%lf %lf %lf\n", c[0], c[1], c[2]);
    //this->ConvertCoord(image, x2, c);
    //printf("%lf %lf %lf\n", c[0], c[1], c[2]);



    //printf("%lf %lf %lf\n",v0,v1,v2);

    //this->DrawLine(image, x0, x1, output1);
    //this->DrawLine(image, x1, x2, output1);
    //this->DrawLine(image, x2, x0, output1);

    //v0 += 10; v1 += 10; v2 += 10;
    for(float dt=0.0; dt<=1.0; dt+= this->Epsilon) 
    {
      for(i=0;i<3;i++) 
      {
        p1[i] = x0[i] + dt * (x2[i] - x0[i]);
        p2[i] = x1[i] + dt * (x2[i] - x1[i]);
        //v1 = v0 + dt * (v2 - v0); 
        //v2 = v1 + dt * (v2 - v1);
      } 
      this->DrawLine(output1, output, x0, x1, x2, p1, p2, v0, v1, v2);//v1, v2);
    }

  }

  return 1;
}

//----------------------------------------------------------------------------
int vtkmpjPolyDataToImageFilter::FillInputPortInformation(int port, vtkInformation* info)
{
  if (port == 0)
  {
       info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkImageData" );
      return 1;
  } else if (port == 1)
    {
      info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkPolyData");
      //info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
      return 1;
    }
  return 0;
}


void vtkmpjPolyDataToImageFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Epsilon: " << this->Epsilon << "\n";

}


