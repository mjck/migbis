/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkmpjRationalGaussianSheet.cpp

  Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkmpjRationalGaussianSheet.h"

#include "vtkCellArray.h"
#include "vtkMath.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"

vtkStandardNewMacro(vtkmpjRationalGaussianSheet);

vtkmpjRationalGaussianSheet::vtkmpjRationalGaussianSheet()
{
  this->SigmaValue = 0.05;
  this->Resolution[0] = 10;
  this->Resolution[1] = 10;
  this->ComputeStatistics = false;
  this->DirectionalColormap = false;
  this->VaryThickness = false;
  this->NormalEigenvector = 1;
  this->NormalScale = 1.0;
  this->Scale = 0.5;
  this->HighQuality = false;
  this->InterpolateScalars = false;
}

void vtkmpjRationalGaussianSheet::DuplicateFields(vtkIdType index, int ComputeDEC)
{
    double value, par[2], nrm[3], rgba[4];

    // copy normals
    this->newNormals->GetTuple(index, nrm);
    this->newNormals->InsertNextTuple(nrm);

    // copy parameters
    this->newParams->GetTuple(index, par);
    this->newParams->InsertNextTuple(par);

    // copy fields
    this->newFA->GetTuple(index, &value);
    this->newFA->InsertNextTuple(&value);

    this->newMD->GetTuple(index, &value);
    this->newMD->InsertNextTuple(&value);

    this->newDist->GetTuple(index, &value);
    this->newDist->InsertNextTuple(&value);

    this->newAngle->GetTuple(index, &value);
    this->newAngle->InsertNextTuple(&value);

    this->newTorsion->GetTuple(index, &value);
    this->newTorsion->InsertNextTuple(&value);

    this->newRoi->GetTuple(index, &value);
    this->newRoi->InsertNextTuple(&value);

    this->newOther->GetTuple(index, &value);
    this->newOther->InsertNextTuple(&value);

    if (this->ComputeStatistics) 
    {
      this->minangle->GetTuple(index, &value);
      this->minangle->InsertNextTuple(&value);
      this->maxangle->GetTuple(index, &value);
      this->maxangle->InsertNextTuple(&value);
      this->medangle->GetTuple(index, &value);
      this->medangle->InsertNextTuple(&value);
      this->meanangle->GetTuple(index, &value);
      this->meanangle->InsertNextTuple(&value);
      this->varangle->GetTuple(index, &value);
      this->varangle->InsertNextTuple(&value);

      this->mintorsion->GetTuple(index, &value);
      this->mintorsion->InsertNextTuple(&value);
      this->maxtorsion->GetTuple(index, &value);
      this->maxtorsion->InsertNextTuple(&value);
      this->medtorsion->GetTuple(index, &value);
      this->medtorsion->InsertNextTuple(&value);
      this->meantorsion->GetTuple(index, &value);
      this->meantorsion->InsertNextTuple(&value);
      this->vartorsion->GetTuple(index, &value);
      this->vartorsion->InsertNextTuple(&value);
    
      this->minother->GetTuple(index, &value);
      this->minother->InsertNextTuple(&value);
      this->maxother->GetTuple(index, &value);
      this->maxother->InsertNextTuple(&value);
      this->medother->GetTuple(index, &value);
      this->medother->InsertNextTuple(&value);
      this->meanother->GetTuple(index, &value);
      this->meanother->InsertNextTuple(&value);
      this->varother->GetTuple(index, &value);
      this->varother->InsertNextTuple(&value);              
    }

    if (ComputeDEC)
    {
      this->dec->GetTuple(index, rgba);
      this->dec->InsertNextTuple(rgba);
    }
}

void vtkmpjRationalGaussianSheet::ComputeSideNormals(vtkPoints *newPoints, int j, vtkIdType offset, double norm1[3], double norm2[3])
{
  double pt1[3], pt2[3], pt3[3], pt4[3];
  double va[3], vb[3];

  newPoints->GetPoint(offset, pt1);
  newPoints->GetPoint(offset+1, pt2);

  if (j == 0 || j < Resolution[0] - 1) {

    newPoints->GetPoint(offset+2, pt3);
    newPoints->GetPoint(offset+3, pt4);

    va[0] = pt2[0]-pt1[0]; va[1] = pt2[1]-pt1[1]; va[2] = pt2[2]-pt1[2];
    vb[0] = pt3[0]-pt1[0]; vb[1] = pt3[1]-pt1[1]; vb[2] = pt3[2]-pt1[2];
    vtkMath::Cross(vb, va, norm1); vtkMath::Normalize(norm1);

    va[0] = pt2[0]-pt1[0]; va[1] = pt2[1]-pt1[1]; va[2] = pt2[2]-pt1[2];
    vb[0] = pt4[0]-pt2[0]; vb[1] = pt4[1]-pt2[1]; vb[2] = pt4[2]-pt2[2];
    vtkMath::Cross(vb, va, norm2); vtkMath::Normalize(norm2);

  } else if (j == Resolution[0] - 1) {

    newPoints->GetPoint(offset-2, pt3);
    newPoints->GetPoint(offset-1, pt4);
    va[0] = pt2[0]-pt1[0]; va[1] = pt2[1]-pt1[1]; va[2] = pt2[2]-pt1[2];
    vb[0] = pt1[0]-pt3[0]; vb[1] = pt1[1]-pt3[1]; vb[2] = pt1[2]-pt3[2];
    vtkMath::Cross(vb, va, norm1); vtkMath::Normalize(norm1);

    va[0] = pt2[0]-pt1[0]; va[1] = pt2[1]-pt1[1]; va[2] = pt2[2]-pt1[2];
    vb[0] = pt2[0]-pt4[0]; vb[1] = pt2[1]-pt4[1]; vb[2] = pt2[2]-pt4[2];
    vtkMath::Cross(vb, va, norm2); vtkMath::Normalize(norm2);
  }
}

void vtkmpjRationalGaussianSheet::ComputeUpsideNormals(vtkPoints *newPoints, int i, vtkIdType offset, double norm1[3], double norm2[3])
{
  double pt1[3], pt2[3], pt3[3], pt4[3];
  double va[3], vb[3];

  newPoints->GetPoint(offset, pt1);
  newPoints->GetPoint(offset+1, pt2);

  if (i == 0 || i < this->Resolution[1]) {

    newPoints->GetPoint(offset+2, pt3);
    newPoints->GetPoint(offset+3, pt4);

    va[0] = pt2[0]-pt1[0]; va[1] = pt2[1]-pt1[1]; va[2] = pt2[2]-pt1[2];
    vb[0] = pt3[0]-pt1[0]; vb[1] = pt3[1]-pt1[1]; vb[2] = pt3[2]-pt1[2];
    vtkMath::Cross(va, vb, norm1); vtkMath::Normalize(norm1);

    va[0] = pt2[0]-pt1[0]; va[1] = pt2[1]-pt1[1]; va[2] = pt2[2]-pt1[2];
    vb[0] = pt4[0]-pt2[0]; vb[1] = pt4[1]-pt2[1]; vb[2] = pt4[2]-pt2[2];
    vtkMath::Cross(va, vb, norm2); vtkMath::Normalize(norm2);

  } else if (i == this->Resolution[1] - 1) {

    newPoints->GetPoint(offset-2, pt3);
    newPoints->GetPoint(offset-1, pt4);
    va[0] = pt2[0]-pt1[0]; va[1] = pt2[1]-pt1[1]; va[2] = pt2[2]-pt1[2];
    vb[0] = pt1[0]-pt3[0]; vb[1] = pt1[1]-pt3[1]; vb[2] = pt1[2]-pt3[2];
    vtkMath::Cross(va, vb, norm1); vtkMath::Normalize(norm1);
    //norm1[0] = norm1[1] = norm1[2] = 0;

    //va[0] = pt2[0]-pt1[0]; va[1] = pt2[1]-pt1[1]; va[2] = pt2[2]-pt1[2];
    //vb[0] = -pt2[0]+pt4[0]; vb[1] = -pt2[1]+pt4[1]; vb[2] = -pt2[2]+pt4[2];
    //vtkMath::Cross(va, vb, norm2); vtkMath::Normalize(norm2);
    //norm2[0] = norm2[1] = norm2[2] = 0;
    norm2[0] = norm1[0]; 
    norm2[1] = norm1[1]; 
    norm2[2] = norm1[2];
  }
}

void vtkmpjRationalGaussianSheet::ComputeNormal(vtkPoints *newPoints, int i, int j, double norm[3])
{
  double x[3], a[3], b[3], c[3], d[3];
  double va[3], vb[3], vc[3], vd[3];
  double norm1[3], norm2[3], norm3[3], norm4[3];

  // get point at (i, j)
  newPoints->GetPoint(i*this->Resolution[0]+j, x);

  if (i == 0) 
  {
    // left border
    newPoints->GetPoint((i+1)*this->Resolution[0]+j, b);  // i+1, j
    vb[0] = b[0] - x[0]; vb[1] = b[1] - x[1]; vb[2] = b[2] - x[2];

    if (j == 0) 
    {
      newPoints->GetPoint(i*this->Resolution[0]+j+1, d);  // j+1, i
      vd[0] = d[0] - x[0]; vd[1] = d[1] - x[1]; vd[2] = d[2] - x[2];
      vtkMath::Cross(vd, vb, norm);
      vtkMath::Normalize(norm);
    } else if (j == this->Resolution[0]-1) 
    {
      newPoints->GetPoint(i*this->Resolution[0]+j-1, c);  // j-1, i
      vc[0] = -c[0] + x[0]; vc[1] = -c[1] + x[1]; vc[2] = -c[2] + x[2];
      vtkMath::Cross(vc, vb, norm);
      vtkMath::Normalize(norm);
    } else 
    {
      newPoints->GetPoint(i*this->Resolution[0]+j-1, c);  // j-1, i
      vc[0] = -c[0] + x[0]; vc[1] = -c[1] + x[1]; vc[2] = -c[2] + x[2];
      newPoints->GetPoint(i*this->Resolution[0]+j+1, d);  // j+1, i
      vd[0] = d[0] - x[0]; vd[1] = d[1] - x[1]; vd[2] = d[2] - x[2];
      vtkMath::Cross(vc, vb, norm1); vtkMath::Normalize(norm1);
      vtkMath::Cross(vd, vb, norm2); vtkMath::Normalize(norm2);
      norm[0] = (norm1[0] + norm2[0]) / 2.0;
      norm[1] = (norm1[1] + norm2[1]) / 2.0;
      norm[2] = (norm1[2] + norm2[2]) / 2.0;
    } 
  } else if (i == this->Resolution[1]-1) 
  { 
    // right border
    newPoints->GetPoint((i-1)*this->Resolution[0]+j, a);  // i-1, j
    va[0] = -a[0] + x[0]; va[1] = -a[1] + x[1]; va[2] = -a[2] + x[2];

    if (j == 0) 
    {
      newPoints->GetPoint(i*this->Resolution[0]+j+1, d);  // j+1, i
      vd[0] = d[0] - x[0]; vd[1] = d[1] - x[1]; vd[2] = d[2] - x[2];
      vtkMath::Cross(vd, va, norm);
      vtkMath::Normalize(norm);
    } else if (j == this->Resolution[0]-1) 
    {
      newPoints->GetPoint(i*this->Resolution[0]+j-1, c);  // j-1, i
      vc[0] = -c[0] + x[0]; vc[1] = -c[1] + x[1]; vc[2] = -c[2] + x[2];
      vtkMath::Cross(vc, va, norm);
      vtkMath::Normalize(norm);
    } else 
    {
      newPoints->GetPoint(i*this->Resolution[0]+j-1, c);  // j-1, i
      vc[0] = -c[0] + x[0]; vc[1] = -c[1] + x[1]; vc[2] = -c[2] + x[2];
      newPoints->GetPoint(i*this->Resolution[0]+j+1, d);  // j+1, i
      vd[0] = d[0] - x[0]; vd[1] = d[1] - x[1]; vd[2] = d[2] - x[2];
      vtkMath::Cross(vc, va, norm1); vtkMath::Normalize(norm1);
      vtkMath::Cross(vd, va, norm2); vtkMath::Normalize(norm2);
      norm[0] = (norm1[0] + norm2[0]) / 2.0;
      norm[1] = (norm1[1] + norm2[1]) / 2.0;
      norm[2] = (norm1[2] + norm2[2]) / 2.0;
    } 
  } else 
  {
    // middle
    newPoints->GetPoint((i-1)*this->Resolution[0]+j, a);  // i-1, j
    va[0] = -a[0] + x[0]; va[1] = -a[1] + x[1]; va[2] = -a[2] + x[2];
    newPoints->GetPoint((i+1)*this->Resolution[0]+j, b);  // i+1, j
    vb[0] = b[0] - x[0]; vb[1] = b[1] - x[1]; vb[2] = b[2] - x[2];

    if (j == 0) 
    {
      newPoints->GetPoint(i*this->Resolution[0]+j+1, d);  // j+1, i
      vd[0] = d[0] - x[0]; vd[1] = d[1] - x[1]; vd[2] = d[2] - x[2];
      vtkMath::Cross(vd, va, norm1); vtkMath::Normalize(norm1);
      vtkMath::Cross(vd, vb, norm2); vtkMath::Normalize(norm2);
      norm[0] = (norm1[0] + norm2[0]) / 2.0;
      norm[1] = (norm1[1] + norm2[1]) / 2.0;
      norm[2] = (norm1[2] + norm2[2]) / 2.0;
    } else if (j == this->Resolution[0]-1) 
    {
      newPoints->GetPoint(i*this->Resolution[0]+j-1, c);  // j-1, i
      vc[0] = -c[0] + x[0]; vc[1] = -c[1] + x[1]; vc[2] = -c[2] + x[2];
      vtkMath::Cross(vc, va, norm1); vtkMath::Normalize(norm1);
      vtkMath::Cross(vc, vb, norm2); vtkMath::Normalize(norm2);
      norm[0] = (norm1[0] + norm2[0]) / 2.0;
      norm[1] = (norm1[1] + norm2[1]) / 2.0;
      norm[2] = (norm1[2] + norm2[2]) / 2.0;
    } else 
    {
      newPoints->GetPoint(i*this->Resolution[0]+j-1, c);  // j-1, i
      vc[0] = -c[0] + x[0]; vc[1] = -c[1] + x[1]; vc[2] = -c[2] + x[2];
      newPoints->GetPoint(i*this->Resolution[0]+j+1, d);  // j+1, i
      vd[0] = d[0] - x[0]; vd[1] = d[1] - x[1]; vd[2] = d[2] - x[2];
      vtkMath::Cross(vc, va, norm1); vtkMath::Normalize(norm1);
      vtkMath::Cross(vd, va, norm2); vtkMath::Normalize(norm2);
      vtkMath::Cross(vc, vb, norm3); vtkMath::Normalize(norm3);
      vtkMath::Cross(vd, vb, norm4); vtkMath::Normalize(norm4);
      norm[0] = (norm1[0] + norm2[0] + norm3[0] + norm4[0]) / 4.0;
      norm[1] = (norm1[1] + norm2[1] + norm3[1] + norm4[1]) / 4.0;
      norm[2] = (norm1[2] + norm2[2] + norm3[2] + norm4[2]) / 4.0;
    }
  }
}

inline double vtkmpjRationalGaussianSheet::Gaussian(double u, double v, double ui, double vi, double sigma)
{
  return exp(-((u - ui) * (u - ui) + (v - vi) * (v - vi)) / (2.0 * sigma * sigma));
}

/*double vtkmpjRationalGaussianSheet::g(double u, double v, double ui, double vi, double sigma, 
  vtkDataArray *params, vtkIdType numPts)
{
  vtkIdType k;
  double uv[2];
  double sum = 0.0;
  double Gi = this->Gaussian(u, v, ui, vi, sigma);
  double tsig = 3.0 * sigma;
  //double dist = VTK_DOUBLE_MAX;

  for(k=0; k<numPts; k++) 
  {
    params->GetTuple(k, uv);
    if (fabs(uv[0] - u) < tsig || fabs(uv[1] - v) < tsig)
      sum += this->Gaussian(u, v, uv[0], uv[1], sigma);
  }

  return Gi / sum;
}*/

double vtkmpjRationalGaussianSheet::g(double u, double v, double ui, double vi, double sigma, 
  vtkDataArray *params, vtkIdType numPts)//, vtkIdType *mink)
{
  vtkIdType k;
  double uv[2];
  double sum = 0.0;
  double Gi = this->Gaussian(u, v, ui, vi, sigma);
  double tsig = 3.0 * sigma;
  double dist;

  for(k=0; k<numPts; k++) 
  {
    params->GetTuple(k, uv);
    dist=(uv[0]-u)*(uv[0]-u)+(uv[1]-v)*(uv[1]-v);
    if (dist < tsig*tsig)
      sum += this->Gaussian(u, v, uv[0], uv[1], sigma);
  }

  return Gi / sum;
}


vtkmpjRationalGaussianSheet::~vtkmpjRationalGaussianSheet()
{
  //this->Ids->Delete();
}

int vtkmpjRationalGaussianSheet::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the input and output
  vtkPolyData *input = vtkPolyData::SafeDownCast(
    inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  vtkPointData *inPD=input->GetPointData();
  vtkPointData *outPD=output->GetPointData();

  if (inPD->GetArray("parameters") == NULL) {
    vtkErrorMacro(<< "Need parametric coordinates")
    return 1;
  }

  if (this->Resolution[0] < 1)
  {
    vtkErrorMacro(<< "Resolution[0] must be greater than 0");
    return 1;
  }
  
  if (this->Resolution[1] < 1)
  {
     vtkErrorMacro(<< "Resolution[1] must be greater than 0");
     return 1;
  }

  vtkPoints *inPts = input->GetPoints();
  int numPts = inPts->GetNumberOfPoints();

  vtkDataArray *inScalars = inPD->GetScalars();
  vtkDataArray *inParams = inPD->GetArray("parameters");
  vtkDataArray *inFA = inPD->GetArray("fractani");
  vtkDataArray *inMD = inPD->GetArray("meandiff");
  vtkDataArray *inDist = inPD->GetArray("distance");
  vtkDataArray *inAngle = inPD->GetArray("angle");
  vtkDataArray *inTorsion = inPD->GetArray("torsion");
  vtkDataArray *inRoi = inPD->GetArray("roi");
  vtkDataArray *inOther = inPD->GetArray("other");

  vtkDataArray *inMinAngle = inPD->GetArray("minangle");
  vtkDataArray *inMaxAngle = inPD->GetArray("maxangle");
  vtkDataArray *inMedAngle = inPD->GetArray("medangle");
  vtkDataArray *inMeanAngle = inPD->GetArray("meanangle");
  vtkDataArray *inVarAngle = inPD->GetArray("varangle");

  vtkDataArray *inMinTorsion = inPD->GetArray("mintorsion");
  vtkDataArray *inMaxTorsion = inPD->GetArray("maxtorsion");
  vtkDataArray *inMedTorsion = inPD->GetArray("medtorsion");
  vtkDataArray *inMeanTorsion = inPD->GetArray("meantorsion");
  vtkDataArray *inVarTorsion = inPD->GetArray("vartorsion");

  vtkDataArray *inMinOther = inPD->GetArray("minother");
  vtkDataArray *inMaxOther = inPD->GetArray("maxother");
  vtkDataArray *inMedOther = inPD->GetArray("medother");
  vtkDataArray *inMeanOther = inPD->GetArray("meanother");
  vtkDataArray *inVarOther = inPD->GetArray("varother");

  vtkDataArray *inDec = inPD->GetArray("rgb");

  this->newPoints = NULL;
  this->newNormals = NULL;
  this->newParams = NULL;
  this->newPolys = NULL;

  this->newFA = NULL;
  this->newMD = NULL;
  this->newDist = NULL;
  this->newAngle = NULL;
  this->newTorsion = NULL;
  this->newRoi = NULL;
  this->newOther = NULL;

  this->minangle = NULL;
  this->maxangle = NULL;
  this->medangle = NULL;
  this->meanangle = NULL;
  this->varangle = NULL;

  this->mintorsion = NULL;
  this->maxtorsion = NULL;
  this->medtorsion = NULL;
  this->meantorsion = NULL;
  this->vartorsion = NULL;

  this->minother = NULL;
  this->maxother = NULL;
  this->medother = NULL;
  this->meanother = NULL;
  this->varother = NULL;

  this->dec = NULL;

  vtkIdType i, j, k, index, offset, numNewPts, sheetSize;

  double pt[3], pt1[3], pt2[3], pt3[3], pt4[3];
  double s1[3], s2[3], uv[2];
  double sum[3], suma[3], sumb[3];
  double nrm[3], nrma[3], nrmb[3];
  double du = 1.0 / (this->Resolution[0]-1);
  double dv = 1.0 / (this->Resolution[1]-1);
  double deltaU = du / 4.0;
  double deltaV = dv / 4.0;
  double u = j * du; 
  double v = i * dv;
  double g = 0.0;
  double gu = 0.0;
  double gv = 0.0;

  double value, rgba[4];

  double sumFA, sumMD, sumDist, sumAngle, sumTorsion, sumRoi, sumOther;
  double sumMinAngle, sumMaxAngle, sumMedAngle, sumMeanAngle, sumVarAngle;
  double sumMinTorsion, sumMaxTorsion, sumMedTorsion, sumMeanTorsion, sumVarTorsion;
  double sumMinOther, sumMaxOther, sumMedOther, sumMeanOther, sumVarOther;
  double sumRGBA[4];

  int ComputeDEC = 0;

  double range[2];
  double sFactor[3];
   
  if ( inScalars ) {
    inScalars->GetRange(range, this->NormalEigenvector);  // range of lambda1
  } else {
    range[0] = 0; range[1]= 1.0;
  }


  //
  // Set things up; allocate memory
  //
  sheetSize = this->Resolution[0] * this->Resolution[1];
  numNewPts = 2 * this->Resolution[0] * this->Resolution[1] + 
              4 * this->Resolution[0] +
              4 * this->Resolution[1];

  this->newPoints = vtkPoints::New();
  this->newPoints->Allocate(numNewPts);

  this->newScalars = vtkFloatArray::New();
  this->newScalars->SetNumberOfComponents(3);
  this->newScalars->Allocate(3*numNewPts);

  this->newNormals = vtkFloatArray::New();
  this->newNormals->SetNumberOfComponents(3);
  this->newNormals->Allocate(3*numNewPts);
  this->newNormals->SetName("Normals");

  // parameters
  this->newParams = vtkFloatArray::New();
  this->newParams->SetNumberOfComponents(2);
  this->newParams->SetName("parameters");
  this->newParams->Allocate(2*numNewPts);

  // traditional measures
  this->newFA = vtkFloatArray::New();
  this->newFA->SetNumberOfComponents(1);
  this->newFA->SetName("fractani");
  this->newFA->Allocate(numNewPts);

  this->newMD = vtkFloatArray::New();
  this->newMD->SetName("meandiff");
  this->newMD->SetNumberOfComponents(1);
  this->newMD->Allocate(numNewPts);

  this->newDist = vtkFloatArray::New();
  this->newDist->SetName("distance");
  this->newDist->SetNumberOfComponents(1);
  this->newDist->Allocate(numNewPts);
    
  this->newAngle = vtkFloatArray::New();
  this->newAngle->SetName("angle");
  this->newAngle->SetNumberOfComponents(1);
  this->newAngle->Allocate(numNewPts);
  
  this->newTorsion = vtkFloatArray::New();
  this->newTorsion->SetName("torsion");
  this->newTorsion->SetNumberOfComponents(1);
  this->newTorsion->Allocate(numNewPts);

  this->newRoi = vtkFloatArray::New();
  this->newRoi->SetName("roi");
  this->newRoi->SetNumberOfComponents(1);
  this->newRoi->Allocate(numNewPts);

  this->newOther = vtkFloatArray::New();
  this->newOther->SetName("other");
  this->newOther->SetNumberOfComponents(1);
  this->newOther->Allocate(numNewPts);

  // set up stats 
  if (this->ComputeStatistics) 
  {
    this->minangle = vtkFloatArray::New();
    this->minangle->SetName("minangle");
    this->minangle->SetNumberOfComponents(1);
    this->minangle->Allocate(numNewPts);
    
    this->maxangle = vtkFloatArray::New();
    this->maxangle->SetName("maxangle");
    this->maxangle->SetNumberOfComponents(1);
    this->maxangle->Allocate(numNewPts);

    this->medangle = vtkFloatArray::New();
    this->medangle->SetName("medangle");
    this->medangle->SetNumberOfComponents(1);
    this->medangle->Allocate(numNewPts);
  
    this->meanangle = vtkFloatArray::New();
    this->meanangle->SetName("meanangle");
    this->meanangle->SetNumberOfComponents(1);
    this->meanangle->Allocate(numNewPts);

    this->varangle = vtkFloatArray::New();
    this->varangle->SetName("varangle");
    this->varangle->SetNumberOfComponents(1);
    this->varangle->Allocate(numNewPts);
  
    this->mintorsion = vtkFloatArray::New();
    this->mintorsion->SetName("mintorsion");
    this->mintorsion->SetNumberOfComponents(1);
    this->mintorsion->Allocate(numNewPts);

    this->maxtorsion = vtkFloatArray::New();
    this->maxtorsion->SetName("maxtorsion");
    this->maxtorsion->SetNumberOfComponents(1);
    this->maxtorsion->Allocate(numNewPts);

    this->medtorsion = vtkFloatArray::New();
    this->medtorsion->SetName("medtorsion");
    this->medtorsion->SetNumberOfComponents(1);
    this->medtorsion->Allocate(numNewPts);
  
    this->meantorsion = vtkFloatArray::New();
    this->meantorsion->SetName("meantorsion");
    this->meantorsion->SetNumberOfComponents(1);
    this->meantorsion->Allocate(numNewPts);

    this->vartorsion = vtkFloatArray::New();
    this->vartorsion->SetName("vartorsion");
    this->vartorsion->SetNumberOfComponents(1);
    this->vartorsion->Allocate(numNewPts);

    this->minother = vtkFloatArray::New();
    this->minother->SetName("minother");
    this->minother->SetNumberOfComponents(1);
    this->minother->Allocate(numNewPts);

    this->maxother = vtkFloatArray::New();
    this->maxother->SetName("maxother");
    this->maxother->SetNumberOfComponents(1);
    this->maxother->Allocate(numNewPts);
  
    this->medother = vtkFloatArray::New();
    this->medother->SetName("medother");
    this->medother->SetNumberOfComponents(1);
    this->medother->Allocate(numNewPts);

    this->meanother = vtkFloatArray::New();
    this->meanother->SetName("meanother");
    this->meanother->SetNumberOfComponents(1);
    this->meanother->Allocate(numNewPts);
  
    this->varother = vtkFloatArray::New();
    this->varother->SetName("varother");
    this->varother->SetNumberOfComponents(1);
    this->varother->Allocate(numNewPts);
    
  }

  if (inDec != NULL && this->DirectionalColormap)
   {
    this->dec = vtkUnsignedCharArray::New();
    this->dec->SetName("rgb");
    this->dec->SetNumberOfComponents(inDec->GetNumberOfComponents());
    this->dec->Allocate(inDec->GetNumberOfComponents()*numNewPts);
    ComputeDEC = inDec->GetNumberOfComponents();
  }

  // polygons
  this->newPolys = vtkCellArray::New();
  this->newPolys->Allocate(4 * (this->Resolution[0]-1) * (this->Resolution[1]-1) +
                     4 * (this->Resolution[1]-1) +
                     4 * (this->Resolution[0]-1));

  double pg = 0.1;
  int tenth = 0.1 * sheetSize;
  int counter = 0;

  double sumPt[3], w[3], sumW[3];
  double uvdist, minuvdist;
  double scaling;
  vtkIdType mink;

  // generate points and scalars
  for(i=0; i<this->Resolution[1]; i++) // dv
    for(j=0; j<this->Resolution[0]; j++) // du
      { 
        if ( counter++ % tenth == 0) {
            this->UpdateProgress(pg);
            pg += 0.1;
        }  

        u = j * du; 
        v = i * dv; 

        //printf("%lf, %lf\n",u,v);

        sumPt[0] = sumPt[1] = sumPt[2] = 0.0;

        sumFA = sumMD = sumDist = 0.0;
        sumAngle = sumTorsion = sumRoi = sumOther = 0.0; 
        sumMinAngle = 0.0;  sumMinTorsion = 0.0;  sumMinOther = 0.0; 
        sumMaxAngle = 0.0;  sumMaxTorsion = 0.0;  sumMaxOther = 0.0; 
        sumMedAngle = 0.0;  sumMedTorsion = 0.0;  sumMedOther = 0.0; 
        sumMeanAngle = 0.0; sumMeanTorsion = 0.0; sumMeanOther = 0.0; 
        sumVarAngle = 0.0;  sumVarTorsion = 0.0;  sumVarOther = 0.0; 
        sumRGBA[0] = sumRGBA[1] = sumRGBA[2] = sumRGBA[3] = 0.0;
        sumW[0] = sumW[1] = sumW[2] = 0.0;     

        inParams->GetTuple(0, uv); 
        minuvdist = (u-uv[0])*(u-uv[0]) + (v-uv[1])*(v-uv[1]);
        mink = 0;

        // calculate contribution of all points
        for(k=0; k<numPts; k++) {

          // get point and parameters
          inPts->GetPoint(k, pt);

          //printf("getting param\n");
          inParams->GetTuple(k, uv);

          // calculate distance
          uvdist = (u-uv[0])*(u-uv[0]) + (v-uv[1])*(v-uv[1]);
          if (uvdist < minuvdist) {
            minuvdist = uvdist;
            mink = k;
          }

          //printf("calc gassian\n");
          // calculate RaG function
          g = this->g(u, v, uv[0], uv[1], this->SigmaValue, inParams, numPts);

          // calculate new point coordinates
          sumPt[0] += pt[0] * g;
          sumPt[1] += pt[1] * g;
          sumPt[2] += pt[2] * g;

          if (this->InterpolateScalars) 
          {
            inScalars->GetTuple(k, w);
            sumW[0] += w[0] * g;
            sumW[1] += w[1] * g;
            sumW[2] += w[2] * g;

            // calculate associated fields
            //printf("get fields\n");
            inFA->GetTuple(k, &value);         sumFA += value * g; 
            inMD->GetTuple(k, &value);         sumMD += value * g;
            inDist->GetTuple(k, &value);       sumDist += value * g;
            inAngle->GetTuple(k, &value);      sumAngle += value * g;
            inTorsion->GetTuple(k, &value);    sumTorsion += value * g;
            inRoi->GetTuple(k, &value);        sumRoi += value * g;
            inOther->GetTuple(k, &value);      sumOther += value * g;

            // calculate stats
            if (this->ComputeStatistics) {
                //printf( "get stats\n");
                inMinAngle->GetTuple(k, &value);      sumMinAngle += value * g;
                inMaxAngle->GetTuple(k, &value);      sumMaxAngle += value * g;
                inMedAngle->GetTuple(k, &value);      sumMedAngle += value * g;
                inMeanAngle->GetTuple(k, &value);     sumMeanAngle += value * g;
                inVarAngle->GetTuple(k, &value);      sumVarAngle += value * g;
                //printf( "get stats2\n");
                inMinTorsion->GetTuple(k, &value);    sumMinTorsion += value * g;
                inMaxTorsion->GetTuple(k, &value);    sumMaxTorsion += value * g;
                inMedTorsion->GetTuple(k, &value);    sumMedTorsion += value * g;
                inMeanTorsion->GetTuple(k, &value);   sumMeanTorsion += value * g;
                inVarTorsion->GetTuple(k, &value);    sumVarTorsion += value * g;
                //printf( "get stats3\n");
                inMinAngle->GetTuple(k, &value);      sumMinOther += value * g;
                inMaxOther->GetTuple(k, &value);      sumMaxOther += value * g;
                inMedOther->GetTuple(k, &value);      sumMedOther += value * g;
                inMeanOther->GetTuple(k, &value);     sumMeanOther += value * g;
                inVarOther->GetTuple(k, &value);      sumVarOther += value * g;
            }

            // calculate RGBA
            if (ComputeDEC) {
                //printf("get dec \n");
                inDec->GetTuple(k, rgba);
                for(int t=0; t<ComputeDEC; t++)
                  sumRGBA[t] += rgba[t] * g;
            }

          }
          
        }

        if (!this->InterpolateScalars) 
        {
            inScalars->GetTuple(mink, sumW);

            inFA->GetTuple(mink, &sumFA);
            inMD->GetTuple(mink, &sumMD);
            inDist->GetTuple(mink, &sumDist);
            inAngle->GetTuple(mink, &sumAngle);
            inTorsion->GetTuple(mink, &sumTorsion);
            inRoi->GetTuple(mink, &sumRoi);
            inOther->GetTuple(mink, &sumOther);

            // calculate stats
            if (this->ComputeStatistics) {
                //printf( "get stats\n");
                inMinAngle->GetTuple(mink, &sumMinAngle);
                inMaxAngle->GetTuple(mink, &sumMaxAngle);
                inMedAngle->GetTuple(mink, &sumMedAngle);
                inMeanAngle->GetTuple(mink, &sumMeanAngle);
                inVarAngle->GetTuple(mink, &sumVarAngle);
                //printf( "get stats2\n");
                inMinTorsion->GetTuple(mink, &sumMinTorsion);
                inMaxTorsion->GetTuple(mink, &sumMaxTorsion);
                inMedTorsion->GetTuple(mink, &sumMedTorsion);
                inMeanTorsion->GetTuple(mink, &sumMeanTorsion);
                inVarTorsion->GetTuple(mink, &sumVarTorsion);
                //printf( "get stats3\n");
                inMinAngle->GetTuple(mink, &sumMinOther);
                inMaxOther->GetTuple(mink, &sumMaxOther);
                inMedOther->GetTuple(mink, &sumMedOther);
                inMeanOther->GetTuple(mink, &sumMeanOther);
                inVarOther->GetTuple(mink, &sumVarOther);
            }

            // calculate RGBA
            if (ComputeDEC) {
                //printf("get dec \n");
                inDec->GetTuple(mink, sumRGBA);
            }
        }

        // insert computed point and fake normal
        this->newPoints->InsertNextPoint(sumPt);
        this->newNormals->InsertNextTuple(sumPt);
        uv[0] = u; uv[1] = v; 
        this->newParams->InsertNextTuple(uv); 

        // insert associated data
        this->newScalars->InsertNextTuple(sumW);

        this->newFA->InsertNextTuple(&sumFA);                   
        this->newMD->InsertNextTuple(&sumMD);                   
        this->newDist->InsertNextTuple(&sumDist);               
        this->newAngle->InsertNextTuple(&sumAngle);             
        this->newTorsion->InsertNextTuple(&sumTorsion);         
        this->newRoi->InsertNextTuple(&sumRoi);                 
        this->newOther->InsertNextTuple(&sumOther);             

        if (this->ComputeStatistics) 
        {
            //printf("insert stats\n");
          this->minangle->InsertNextTuple(&sumMinAngle);        
          this->maxangle->InsertNextTuple(&sumMaxAngle);        
          this->medangle->InsertNextTuple(&sumMedAngle);        
          this->meanangle->InsertNextTuple(&sumMeanAngle);      
          this->varangle->InsertNextTuple(&sumVarAngle);        
          
          this->mintorsion->InsertNextTuple(&sumMinTorsion);    
          this->maxtorsion->InsertNextTuple(&sumMaxTorsion);    
          this->medtorsion->InsertNextTuple(&sumMedTorsion);    
          this->meantorsion->InsertNextTuple(&sumMeanTorsion);  
          this->vartorsion->InsertNextTuple(&sumVarTorsion);    

          this->minother->InsertNextTuple(&sumMinOther);        
          this->maxother->InsertNextTuple(&sumMaxOther);        
          this->medother->InsertNextTuple(&sumMedOther);        
          this->meanother->InsertNextTuple(&sumMeanOther);      
          this->varother->InsertNextTuple(&sumVarOther);                 
        }

        if (ComputeDEC)
        {
          //printf("inserting dec\n");
          this->dec->InsertNextTuple(sumRGBA); 
        }

      } /* for i.. for j.. */

  // duplicate fields
  for(i=0; i<sheetSize; i++) { 
    this->DuplicateFields(i, ComputeDEC);
  }

  // adjust normals and scale the sheet according to 
  // given thickness
  for(i=0; i<this->Resolution[1]; i++) // dv
    for(j=0; j<this->Resolution[0]; j++) // du
      { 
        index = i*this->Resolution[0]+j;

        this->ComputeNormal(this->newPoints, i, j, nrm);

        this->newPoints->GetPoint(index, pt);

        this->newScalars->GetTuple(index, w);

        sFactor[0] =  1.0 +
          (w[0] - range[0]) / (range[1]-range[0]);
        sFactor[1] =  1.0 + 
          (w[1] - range[0]) / (range[1]-range[0]);
        sFactor[2] =  1.0 +
          (w[2] - range[0]) / (range[1]-range[0]);

        //printf("%lf %lf %lf %d\n", sFactor[0], sFactor[1], sFactor[2], this->NormalEigenvector);

        if (this->VaryThickness)
          scaling = sFactor[this->NormalEigenvector] * this->NormalScale;
        else
          scaling = this->NormalScale;

        pt1[0] = pt[0] - this->Scale * scaling * nrm[0];
        pt1[1] = pt[1] - this->Scale * scaling * nrm[1];
        pt1[2] = pt[2] - this->Scale * scaling * nrm[2];
        this->newPoints->SetPoint(index, pt1);
        this->newNormals->SetTuple(index, nrm);

        pt2[0] = pt[0] + this->Scale * scaling * nrm[0];
        pt2[1] = pt[1] + this->Scale * scaling * nrm[1];
        pt2[2] = pt[2] + this->Scale * scaling * nrm[2];
        this->newPoints->InsertNextPoint(pt2);
        this->newNormals->SetTuple(index+sheetSize, nrm);
      }

  //------------------ Compute side rims ---------------------
  // duplicate points at left
  for(j=0; j<this->Resolution[0]; j++)
  {
    this->newPoints->GetPoint(j, pt);
    this->newPoints->InsertNextPoint(pt);
    this->DuplicateFields(j, ComputeDEC);

    this->newPoints->GetPoint(j + sheetSize, pt);
    this->newPoints->InsertNextPoint(pt);
    this->DuplicateFields(j + sheetSize, ComputeDEC);
  }

  // calculate left normals
  for(j=0; j<this->Resolution[0]; j++)
  {
    double nrm1[3], nrm2[3];

    vtkIdType offset = 2 * sheetSize + j*2;
    this->ComputeSideNormals(this->newPoints, j, offset, nrm1, nrm2);
    this->newNormals->SetTuple(offset, nrm1);
    this->newNormals->SetTuple(offset+1, nrm2);
  }

  // duplicate points at right
  for(j=0; j<this->Resolution[0]; j++)
  {
    this->newPoints->GetPoint(j + sheetSize - this->Resolution[0], pt);
    this->newPoints->InsertNextPoint(pt);
    this->DuplicateFields(j + sheetSize - this->Resolution[0], ComputeDEC);

    this->newPoints->GetPoint(j + 2*sheetSize - this->Resolution[0], pt);
    this->newPoints->InsertNextPoint(pt);
    this->DuplicateFields(j + 2*sheetSize - this->Resolution[0], ComputeDEC);
  }

  // calculate right normals
  for(j=0; j<this->Resolution[0]; j++)
  {
    double nrm1[3], nrm2[3];

    vtkIdType offset = 2 * sheetSize + j*2 + 2 * this->Resolution[0];
    this->ComputeSideNormals(this->newPoints, j, offset, nrm1, nrm2);
    this->newNormals->SetTuple(offset, nrm1);
    this->newNormals->SetTuple(offset+1, nrm2);
  }

  //------------------ Compute bottom and top rims ---------------------
  // duplicate points at bottom
  for(i=0; i<this->Resolution[1]; i++)
  {
    int offset = i * this->Resolution[0];
    this->newPoints->GetPoint(offset, pt);
    this->newPoints->InsertNextPoint(pt);
    this->DuplicateFields(offset, ComputeDEC);

    this->newPoints->GetPoint(offset + sheetSize, pt);
    this->newPoints->InsertNextPoint(pt);
    this->DuplicateFields(offset + sheetSize, ComputeDEC);
  }

 // calculate bottom normals
  for(i=0; i<this->Resolution[1]; i++)
  {
    double nrm1[3], nrm2[3];

    vtkIdType offset = 2 * sheetSize + 4 * this->Resolution[0] + i*2;
    this->ComputeUpsideNormals(this->newPoints, i, offset, nrm1, nrm2);
    this->newNormals->SetTuple(offset, nrm1);
    this->newNormals->SetTuple(offset+1, nrm2);
  }

  // duplicate points at top
  for(i=0; i<this->Resolution[1]; i++)
  {
    int offset = (this->Resolution[0] - 1) + i * this->Resolution[0];
    this->newPoints->GetPoint(offset, pt);
    this->newPoints->InsertNextPoint(pt);
    this->DuplicateFields(offset, ComputeDEC);

    this->newPoints->GetPoint(offset + sheetSize, pt);
    this->newPoints->InsertNextPoint(pt);
    this->DuplicateFields(offset + sheetSize, ComputeDEC);
  }

  // calculate bottom normals
  for(i=0; i<this->Resolution[1]; i++)
  {
    double nrm1[3], nrm2[3];

    vtkIdType offset = 2 * sheetSize + 4 * this->Resolution[0] + 2 * this->Resolution[1] + i*2;
    this->ComputeUpsideNormals(this->newPoints, i, offset, nrm1, nrm2);
    this->newNormals->SetTuple(offset, nrm1);
    this->newNormals->SetTuple(offset+1, nrm2);
  }
  
  // mesh!
  vtkIdType ptidx[3];

  // front
  for(i = 0; i < this->Resolution[1]-1; i++)
  {
    for (j = 0; j < this->Resolution[0]-1; j++)
    {
      vtkIdType a = i * this->Resolution[0] + j;
      vtkIdType b = a + this->Resolution[0];
      ptidx[0] = a; ptidx[1] = a+1; ptidx[2] = b;
      this->newPolys->InsertNextCell(3, ptidx);
      ptidx[0] = b; ptidx[1] = a+1; ptidx[2] = b+1;
      this->newPolys->InsertNextCell(3, ptidx);
    }
  }

  // back
  for(i = 0; i < this->Resolution[1]-1; i++)
  {
    for (j = 0; j < this->Resolution[0]-1; j++)
    {
      vtkIdType a = i * this->Resolution[0] + j + sheetSize;
      vtkIdType b = a + this->Resolution[0];
      ptidx[0] = a; ptidx[1] = a+1; ptidx[2] = b;
      this->newPolys->InsertNextCell(3, ptidx);
      ptidx[0] = b; ptidx[1] = a+1; ptidx[2] = b+1;
      this->newPolys->InsertNextCell(3, ptidx);
    }
  }

  // new left and right
  for(j = 0; j < this->Resolution[0]-1; j++)
  {
    vtkIdType a = j * 2 + 2 * sheetSize;
    ptidx[0] = a; ptidx[1] = a+2; ptidx[2] = a+1;
    //printf("%d %d %d\n",ptidx[0],ptidx[1],ptidx[2]);
    this->newPolys->InsertNextCell(3, ptidx);
    ptidx[0] = a+1; ptidx[1] = a+2; ptidx[2] = a+3;
    this->newPolys->InsertNextCell(3, ptidx);
    //printf("%d %d %d\n",ptidx[0],ptidx[1],ptidx[2]);

    a += 2 * this->Resolution[0];
    ptidx[0] = a; ptidx[1] = a+2; ptidx[2] = a+1;
    //printf("%d %d %d\n",ptidx[0],ptidx[1],ptidx[2]);
    this->newPolys->InsertNextCell(3, ptidx);
    ptidx[0] = a+1; ptidx[1] = a+2; ptidx[2] = a+3;
    this->newPolys->InsertNextCell(3, ptidx);
  }

  // new bottom and top
  for(i = 0; i < this->Resolution[1]-1; i++)
  {
    vtkIdType a = i * 2 + 2 * sheetSize + 4 * this->Resolution[0];
    ptidx[0] = a; ptidx[1] = a+1; ptidx[2] = a+2;
    //printf("%d %d %d\n",ptidx[0],ptidx[1],ptidx[2]);
    this->newPolys->InsertNextCell(3, ptidx);
    ptidx[0] = a+2; ptidx[1] = a+1; ptidx[2] = a+3;
    this->newPolys->InsertNextCell(3, ptidx);
    //printf("%d %d %d\n",ptidx[0],ptidx[1],ptidx[2]);

    a += 2 * this->Resolution[1];
    ptidx[0] = a; ptidx[1] = a+1; ptidx[2] = a+2;
    //printf("%d %d %d\n",ptidx[0],ptidx[1],ptidx[2]);
    this->newPolys->InsertNextCell(3, ptidx);
    ptidx[0] = a+2; ptidx[1] = a+1; ptidx[2] = a+3;
    this->newPolys->InsertNextCell(3, ptidx);
  }

  output->SetPoints(this->newPoints);

  outPD->SetScalars(this->newScalars);

  this->newPoints->Delete();

  if (this->HighQuality) {
    outPD->SetNormals(this->newNormals);
  }
  this->newNormals->Delete();
  
  outPD->AddArray(this->newParams);
  this->newParams->Delete();

  outPD->AddArray(this->newFA);
  this->newFA->Delete();

  outPD->AddArray(this->newMD);
  this->newMD->Delete();

  outPD->AddArray(this->newDist);
  this->newDist->Delete();

  outPD->AddArray(this->newAngle);
  this->newAngle->Delete();

  outPD->AddArray(this->newTorsion);
  this->newTorsion->Delete();

  outPD->AddArray(this->newRoi);
  this->newRoi->Delete();

  outPD->AddArray(this->newOther);
  this->newOther->Delete();

  // insert statistics
  if (this->ComputeStatistics) 
  {
    outPD->AddArray(this->minangle);
    this->minangle->Delete(); 

    outPD->AddArray(this->maxangle);
    this->maxangle->Delete(); 

    outPD->AddArray(this->medangle);
    this->medangle->Delete();   

    outPD->AddArray(this->meanangle);
    this->meanangle->Delete(); 

    outPD->AddArray(this->varangle);
    this->varangle->Delete();  

    outPD->AddArray(this->mintorsion);
    this->mintorsion->Delete(); 

    outPD->AddArray(this->maxtorsion);
    this->maxtorsion->Delete(); 

    outPD->AddArray(this->medtorsion);
    this->medtorsion->Delete();   

    outPD->AddArray(this->meantorsion);
    this->meantorsion->Delete(); 

    outPD->AddArray(this->vartorsion);
    this->vartorsion->Delete(); 

    outPD->AddArray(this->minother);
    this->minother->Delete(); 

    outPD->AddArray(this->maxother);
    this->maxother->Delete(); 

    outPD->AddArray(this->medother);
    this->medother->Delete();   

    outPD->AddArray(this->meanother);
    this->meanother->Delete(); 

    outPD->AddArray(this->varother);
    this->varother->Delete(); 
  }

  if (ComputeDEC) {
    outPD->AddArray(this->dec);
    this->dec->Delete();
  }

  output->SetPolys(this->newPolys);
  this->newPolys->Delete();

  return 1;
}

void vtkmpjRationalGaussianSheet::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);

  os << indent << "Sigma Value: " << this->SigmaValue << "\n";
  os << indent << "Resolution: (" << this->Resolution[0]
     << ", " << this->Resolution[1] << ")" << endl;
}

