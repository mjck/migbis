/*=========================================================================

  Program:   vtkmpjAbstractInterpolatedTensorField.h

  Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkmpjAbstractInterpolatedTensorField.h"

#include "vtkMath.h"
#include "vtkDataSet.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "vtkGenericCell.h"
#include "vtkObjectFactory.h"

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
const double vtkmpjAbstractInterpolatedTensorField::TOLERANCE_SCALE = 1.0E-8;

//---------------------------------------------------------------------------
vtkmpjAbstractInterpolatedTensorField::vtkmpjAbstractInterpolatedTensorField()
{
  this->NumFuncs     = 3; // vx, vy, vz
  this->NumIndepVars = 4; // x, y, z, t
  this->Weights      = 0;
  this->WeightsSize  = 0;
 
  this->Caching    = true; // Caching on by default
  this->CacheHit   = 0;
  this->CacheMiss  = 0;
  
  this->LastCellId = -1;
  this->LastDataSet= 0;
  this->LastDataSetIndex = 0;
  this->LastPCoords[0] = 0.0;
  this->LastPCoords[1] = 0.0;
  this->LastPCoords[2] = 0.0;
  
  this->Eigenvector = VTK_MPJ_INTERPOLATE_MAJOR;
  this->NormalEigenvector = VTK_MPJ_INTERPOLATE_MEDIUM;
  this->BinormalEigenvector = VTK_MPJ_INTERPOLATE_MINOR;
  this->FixEigenvectors = false;

  this->FlipX = false;
  this->FlipY = false;
  this->FlipZ = false;

  this->Cell     = vtkGenericCell::New();
  this->GenCell  = vtkGenericCell::New();
  this->DataSets = new vtkmpjAbstractInterpolatedTensorFieldDataSetsType;
}

//---------------------------------------------------------------------------
vtkmpjAbstractInterpolatedTensorField::~vtkmpjAbstractInterpolatedTensorField()
{
  this->NumFuncs     = 0;
  this->NumIndepVars = 0;
  
  this->LastDataSet  = 0;
  //this->SetVectorsSelection( 0 );
  
  if ( this->Weights )
    {
    delete[] this->Weights;
    this->Weights = 0;
    }
  
  if ( this->Cell )
    {
    this->Cell->Delete();
    this->Cell = NULL;
    }
  
  if ( this->GenCell )
    {
    this->GenCell->Delete();
    this->GenCell = NULL;
    }
  
  if ( this->DataSets )
    {    
    delete this->DataSets;
    this->DataSets = NULL;
    }
}

//---------------------------------------------------------------------------
int vtkmpjAbstractInterpolatedTensorField::TensorValues
  ( vtkDataSet * dataset, double * x, double * u, double ** t )
{
  int i, j, subId , numPts, id;
  vtkDataArray * tensors = NULL;
  double *v[3], v0[3], v1[3], v2[3], w[3];
  double *m[3], m0[3], m1[3], m2[3], tensor[9];
  double dist2;
  int ret;
  
  v[0] = v0; v[1] = v1; v[2] = v2;
  m[0] = m0; m[1] = m1; m[2] = m2;

  // See if a dataset has been specified 
  if ( !dataset || 
       !(  tensors = 
           dataset->GetPointData()->GetScalars()
        )
     )
    {
    vtkErrorMacro( << "Can't evaluate tensor dataset!" );
    tensors = NULL;
    return 0;
    }

  int numcomp = tensors->GetNumberOfComponents();
  
  // reset tensor matrix
  for (j=0; j<3; j++)
    for (i=0; i<3; i++)
      {
        m[i][j] = 0.0;
      }


  double tol2 = dataset->GetLength() * 
                vtkmpjAbstractInterpolatedTensorField::TOLERANCE_SCALE;

  int found = 0;

  if ( this->Caching )
    {
    // See if the point is in the cached cell
    if ( this->LastCellId == -1 || 
         !(  ret = this->GenCell->EvaluatePosition
                   ( x, 0, subId, this->LastPCoords, dist2, this->Weights)
          )
        || ret == -1
       )
      {
      // if not, find and get it
      if ( this->LastCellId != - 1 )
        {
        this->CacheMiss ++;

        dataset->GetCell( this->LastCellId, this->Cell );
        
        this->LastCellId = 
          dataset->FindCell( x, this->Cell, this->GenCell, this->LastCellId, 
                             tol2, subId, this->LastPCoords, this->Weights );
                             
        if ( this->LastCellId != -1 )
          {
          dataset->GetCell( this->LastCellId, this->GenCell );
          found = 1;
          }
        }
      }
    else
      {
      this->CacheHit ++;
      found = 1;
      }
    }

  if ( !found )
    {
    // if the cell is not found, do a global search (ignore initial
    // cell if there is one)
    this->LastCellId = 
      dataset->FindCell( x, 0, this->GenCell, -1, tol2, 
                         subId, this->LastPCoords, this->Weights );
                         
    if ( this->LastCellId != -1 )
      {
      dataset->GetCell( this->LastCellId, this->GenCell );
      }
    else
      {
      tensors = NULL;
      return  0;
      }
    }
                                
  // if the cell is valid
  if ( this->LastCellId >= 0 )
    {
    numPts = this->GenCell->GetNumberOfPoints();
    
    // interpolate the tensors
    for ( j = 0; j < numPts; j ++ )
      {
      
      id = this->GenCell->PointIds->GetId( j );

      tensors->GetTuple( id, tensor );
      
      if (numcomp == 6) {

        m[0][0] += tensor[0] * this->Weights[j];
        m[1][0] += tensor[1] * this->Weights[j]; 
        m[0][1] += tensor[1] * this->Weights[j];
        m[2][0] += tensor[2] * this->Weights[j]; 
        m[0][2] += tensor[2] * this->Weights[j];    
        m[1][1] += tensor[3] * this->Weights[j]; 
        m[2][1] += tensor[4] * this->Weights[j]; 
        m[1][2] += tensor[4] * this->Weights[j];
        m[2][2] += tensor[5] * this->Weights[j];      

        //printf("%f %f %f %f %f %f -- ",m[0][0],m[1][0],m[2][0],m[1][1],m[2][1],m[2][2]);  

      } else {
        
        m[0][0] += tensor[0] * this->Weights[j];
        m[1][0] += tensor[1] * this->Weights[j]; 
        m[0][1] += tensor[2] * this->Weights[j];
        m[2][0] += tensor[3] * this->Weights[j]; 
        m[0][2] += tensor[4] * this->Weights[j];  
        m[1][1] += tensor[5] * this->Weights[j]; 
        m[2][1] += tensor[6] * this->Weights[j]; 
        m[1][2] += tensor[7] * this->Weights[j];
        m[2][2] += tensor[8] * this->Weights[j];  
      }
    }

    // diagonalize interpolated tensor
    // eigenvectors are sorted in decreasing order and are normalized
    vtkMath::Jacobi(m, w, v);
    if ( this->FixEigenvectors ) 
    {
      this->FixVectors(v, this->Eigenvector, this->NormalEigenvector, this->BinormalEigenvector);
    }

    // return eigenvalues
    u[0] = w[0];
    u[1] = w[1];
    u[2] = w[2];

    // return interpolated tensor
    for (i=0; i<3; i++)
      {
         t[this->Eigenvector][i] = v[this->Eigenvector][i];
         t[this->NormalEigenvector][i] = v[this->NormalEigenvector][i];
         t[this->BinormalEigenvector][i] = v[this->BinormalEigenvector][i];
      }

    }
  // if not, return false
  else
    {
    tensors = NULL;
    return  0;
    }

  tensors = NULL;
  return  1;
}

//---------------------------------------------------------------------------
int vtkmpjAbstractInterpolatedTensorField::FunctionValues
  ( vtkDataSet * dataset, double * x, double * f )
{
  int i, j, subId , numPts, id;
  vtkDataArray * tensors = NULL;
  double *v[3], v0[3], v1[3], v2[3], w[3];
  double *m[3], m0[3], m1[3], m2[3], tensor[9];
  double dist2;
  int ret;
  
  f[0] = f[1] = f[2] = 0.0;
  v[0] = v0; v[1] = v1; v[2] = v2;
  m[0] = m0; m[1] = m1; m[2] = m2;

  // See if a dataset has been specified 
  if ( !dataset || 
       !(  tensors = 
           dataset->GetPointData()->GetScalars()
        )
     )
    {
    vtkErrorMacro( << "Can't evaluate tensor dataset!" );
    tensors = NULL;
    return 0;
    }

  int numcomp = tensors->GetNumberOfComponents();
  
  // reset tensor matrix
  for (j=0; j<3; j++)
    for (i=0; i<3; i++)
      {
        m[i][j] = 0.0;
      }


  double tol2 = dataset->GetLength() * 
                vtkmpjAbstractInterpolatedTensorField::TOLERANCE_SCALE;

  int found = 0;

  if ( this->Caching )
    {
    // See if the point is in the cached cell
    if ( this->LastCellId == -1 || 
         !(  ret = this->GenCell->EvaluatePosition
                   ( x, 0, subId, this->LastPCoords, dist2, this->Weights)
          )
        || ret == -1
       )
      {
      // if not, find and get it
      if ( this->LastCellId != - 1 )
        {
        this->CacheMiss ++;

        dataset->GetCell( this->LastCellId, this->Cell );
        
        this->LastCellId = 
          dataset->FindCell( x, this->Cell, this->GenCell, this->LastCellId, 
                             tol2, subId, this->LastPCoords, this->Weights );
                             
        if ( this->LastCellId != -1 )
          {
          dataset->GetCell( this->LastCellId, this->GenCell );
          found = 1;
          }
        }
      }
    else
      {
      this->CacheHit ++;
      found = 1;
      }
    }

  if ( !found )
    {
    // if the cell is not found, do a global search (ignore initial
    // cell if there is one)
    this->LastCellId = 
      dataset->FindCell( x, 0, this->GenCell, -1, tol2, 
                         subId, this->LastPCoords, this->Weights );
                         
    if ( this->LastCellId != -1 )
      {
      dataset->GetCell( this->LastCellId, this->GenCell );
      }
    else
      {
      tensors = NULL;
      return  0;
      }
    }
                                
  // if the cell is valid
  if ( this->LastCellId >= 0 )
    {
    numPts = this->GenCell->GetNumberOfPoints();
    
    // interpolate the tensors
    for ( j = 0; j < numPts; j ++ )
      {
      
      id = this->GenCell->PointIds->GetId( j );

      tensors->GetTuple( id, tensor );
      //tensor = tensors->GetTuple(j);

      //for ( i = 0; i < numcomp; i ++ )
        //{
        //g[i] +=  tensor[i] * this->Weights[j];
       // }
      //}
      
      if (numcomp == 6) {

        m[0][0] += tensor[0] * this->Weights[j];
        m[1][0] += tensor[1] * this->Weights[j]; 
        m[0][1] += tensor[1] * this->Weights[j];
        m[2][0] += tensor[2] * this->Weights[j]; 
        m[0][2] += tensor[2] * this->Weights[j];    
        m[1][1] += tensor[3] * this->Weights[j]; 
        m[2][1] += tensor[4] * this->Weights[j]; 
        m[1][2] += tensor[4] * this->Weights[j];
        m[2][2] += tensor[5] * this->Weights[j];      

        //printf("%f %f %f %f %f %f -- ",m[0][0],m[1][0],m[2][0],m[1][1],m[2][1],m[2][2]);  

      } else {
        
        m[0][0] += tensor[0] * this->Weights[j];
        m[1][0] += tensor[1] * this->Weights[j]; 
        m[0][1] += tensor[2] * this->Weights[j];
        m[2][0] += tensor[3] * this->Weights[j]; 
        m[0][2] += tensor[4] * this->Weights[j];  
        m[1][1] += tensor[5] * this->Weights[j]; 
        m[2][1] += tensor[6] * this->Weights[j]; 
        m[1][2] += tensor[7] * this->Weights[j];
        m[2][2] += tensor[8] * this->Weights[j];  
      }
    }

    // diagonalize interpolated tensor
    // eigenvectors are sorted in decreasing order and are normalized
    vtkMath::Jacobi(m, w, v);
    if ( this->FixEigenvectors ) 
    {
      this->FixVectors(v, this->Eigenvector, this->NormalEigenvector, this->BinormalEigenvector);
    }

    f[0] = v[0][this->Eigenvector];
    f[1] = v[1][this->Eigenvector];
    f[2] = v[2][this->Eigenvector];

    }
  // if not, return false
  else
    {
    tensors = NULL;
    return  0;
    }

  tensors = NULL;
  return  1;
}

//----------------------------------------------------------------------------
int vtkmpjAbstractInterpolatedTensorField::GetLastWeights( double * w )
{
  if ( this->LastCellId < 0 )
    {
    return 0;
    }
  
  int   numPts = this->GenCell->GetNumberOfPoints();
  for ( int i = 0; i < numPts; i ++ )
    {
    w[i] = this->Weights[i];
    }
    
  return 1;
}

//----------------------------------------------------------------------------
int vtkmpjAbstractInterpolatedTensorField::GetLastLocalCoordinates( double pcoords[3] )
{
  if ( this->LastCellId < 0 )
    {
    return 0;
    }
    
  pcoords[0] = this->LastPCoords[0];
  pcoords[1] = this->LastPCoords[1];
  pcoords[2] = this->LastPCoords[2];
    
  return 1;
}

//----------------------------------------------------------------------------
void vtkmpjAbstractInterpolatedTensorField::FastCompute
  ( vtkDataArray * tensors, double * f )
{
  int    i, j, pntIdx;
  int    numPts = this->GenCell->GetNumberOfPoints();
  double *v[3], v0[3], v1[3], v2[3], w[3];
  double *m[3], m0[3], m1[3], m2[3], tensor[9];

  int numcomp = tensors->GetNumberOfComponents();

  f[0] = f[1] = f[2] = 0.0;
  v[0] = v0; v[1] = v1; v[2] = v2;
  m[0] = m0; m[1] = m1; m[2] = m2;

  // reset tensor matrix
  for (j=0; j<3; j++)
    for (i=0; i<3; i++)
      {
        m[i][j] = 0.0;
      }

  for ( i = 0; i < numPts; i ++ )
    {
      pntIdx = this->GenCell->PointIds->GetId( i );
      tensors->GetTuple( pntIdx, tensor );

      if (numcomp == 6) {

          m[0][0] += tensor[0] * this->Weights[j];
          m[1][0] += tensor[1] * this->Weights[j]; 
          m[0][1] += tensor[1] * this->Weights[j];
          m[2][0] += tensor[2] * this->Weights[j]; 
          m[0][2] += tensor[2] * this->Weights[j];    
          m[1][1] += tensor[3] * this->Weights[j]; 
          m[2][1] += tensor[4] * this->Weights[j]; 
          m[1][2] += tensor[4] * this->Weights[j];
          m[2][2] += tensor[5] * this->Weights[j];      

          //printf("%f %f %f %f %f %f -- ",m[0][0],m[1][0],m[2][0],m[1][1],m[2][1],m[2][2]);  

        } else {
          
          m[0][0] += tensor[0] * this->Weights[j];
          m[1][0] += tensor[1] * this->Weights[j]; 
          m[0][1] += tensor[2] * this->Weights[j];
          m[2][0] += tensor[3] * this->Weights[j]; 
          m[0][2] += tensor[4] * this->Weights[j];  
          m[1][1] += tensor[5] * this->Weights[j]; 
          m[2][1] += tensor[6] * this->Weights[j]; 
          m[1][2] += tensor[7] * this->Weights[j];
          m[2][2] += tensor[8] * this->Weights[j];  
        }
    }

    // diagonalize interpolated tensor
    // eigenvectors are sorted in decreasing order and are normalized
    vtkMath::Jacobi(m, w, v);
    if ( this->FixEigenvectors ) {
      this->FixVectors(v, this->Eigenvector, this->NormalEigenvector, this->BinormalEigenvector);
    }

    f[0] = v[0][this->Eigenvector];
    f[1] = v[1][this->Eigenvector];
    f[2] = v[2][this->Eigenvector];

}

//----------------------------------------------------------------------------
bool vtkmpjAbstractInterpolatedTensorField::InterpolatePoint
  ( vtkPointData * outPD, vtkIdType outIndex )
{
  if ( !this->LastDataSet )
    {
    return 0;
    }
    
  outPD->InterpolatePoint( this->LastDataSet->GetPointData(), outIndex, 
                           this->GenCell->PointIds, this->Weights );
  return 1;
}

//----------------------------------------------------------------------------
void vtkmpjAbstractInterpolatedTensorField::SetLastVectors(double **current, int iv, int ix, int iy)
{
  double v0[3], v1[3], v2[3];
  double p0[3], p1[3], p2[3];
  double temp[3];
  int i;

  for (i=0; i<3; i++) 
  {
    v0[i] = this->LastVectors[i][iv] = current[i][iv]; 
    v1[i] = this->LastVectors[i][ix] = current[i][ix];
    v2[i] = this->LastVectors[i][iy] = current[i][iy];
  } 

  vtkMath::Cross(v0,v1,temp);
  if ( vtkMath::Dot(v2,temp) < 0.0 )
    {
      for (i=0; i<3; i++)
      { 
        this->LastVectors[i][iy] *= -1.0;
      }
    }
}

//----------------------------------------------------------------------------
void vtkmpjAbstractInterpolatedTensorField::FixVectors(double **current, int iv, int ix, int iy)
{
  double p0[3], p1[3], p2[3];
  double v0[3], v1[3], v2[3];
  double temp[3];
  int i, flip[3];

  flip[0] = this->FlipX ? -1 : 1;
  flip[1] = this->FlipY ? -1 : 1;
  flip[2] = this->FlipZ ? -1 : 1;

  for (i=0; i<3; i++) 
  {
    v0[i] = current[i][iv] * flip[i]; 
    v1[i] = current[i][ix] * flip[i];
    v2[i] = current[i][iy] * flip[i];
  }
  
  for (i=0; i<3; i++)
  {
    p0[i] = this->LastVectors[i][iv];
    p1[i] = this->LastVectors[i][ix];
    p2[i] = this->LastVectors[i][iy];
  }

  if ( vtkMath::Dot(p0,v0) < 0.0 )
  {
    for (i=0; i<3; i++)
    {
      current[i][iv] *= -1.0;
    }
  }
  if ( vtkMath::Dot(p1,v1) < 0.0 )
  {
    for (i=0; i<3; i++)
    {
      current[i][ix] *= -1.0;
    }
  }
  if ( vtkMath::Dot(p2,v2) < 0.0 )
  {
    for (i=0; i<3; i++)
    {
      current[i][iy] *= -1.0;
    }
  }

  for (i=0; i<3; i++)
  {
    this->LastVectors[i][iv] = current[i][iv];
    this->LastVectors[i][ix] = current[i][ix];
    this->LastVectors[i][iy] = current[i][iy];
  }

  
}

//----------------------------------------------------------------------------
void vtkmpjAbstractInterpolatedTensorField::PrintSelf( ostream & os, vtkIndent indent )
{
  this->Superclass::PrintSelf( os, indent );
  
  os << indent << "Eigenvector: "        << this->Eigenvector      << endl; 
  
  os << indent << "FlipX: "              << this->FlipX       << endl;
  os << indent << "FlipY: "              << this->FlipY       << endl;
  os << indent << "FlipZ: "              << this->FlipZ       << endl;

  os << indent << "Caching Status: "     << ( this->Caching ? "on." : "off." )
     << endl;
  os << indent << "Cache Hit: "          << this->CacheHit         << endl;
  os << indent << "Cache Miss: "         << this->CacheMiss        << endl;
  os << indent << "Weights Size: "       << this->WeightsSize      << endl;
  
  os << indent << "DataSets: "           << this->DataSets         << endl;
  os << indent << "Last Dataset Index: " << this->LastDataSetIndex << endl;
  os << indent << "Last Dataset: "       << this->LastDataSet      << endl;
  
  os << indent << "Last Cell Id: "       << this->LastCellId       << endl;
  os << indent << "Last Cell: "          << this->Cell             << endl;
  os << indent << "Current Cell: "       << this->GenCell          << endl;
  os << indent << "Last P-Coords: "      << this->LastPCoords[0]
               << ", "                   << this->LastPCoords[1]
               << ", "                   << this->LastPCoords[2]   << endl;
  os << indent << "Last Weights: "       << this->Weights          << endl;
}
