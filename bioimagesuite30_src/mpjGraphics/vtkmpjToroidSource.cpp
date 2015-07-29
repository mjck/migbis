

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


#include "vtkmpjToroidSource.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"

#include <math.h>

vtkStandardNewMacro(vtkmpjToroidSource);

//----------------------------------------------------------------------------
vtkmpjToroidSource::vtkmpjToroidSource()
{
	this->Alpha = 1.0;
	this->Beta = 0.5;
	this->Gamma = 0.5;

	this->UResolution = 10;
	this->VResolution = 10;
	this->GenerateTextureCoordinates = 0;
	this->ScalarMode = vtkmpjToroidSource::SCALAR_NONE;
	
	this->MinimumU = 0;
	this->MinimumV = 0;
	this->MaximumU = 2 * vtkMath::Pi();
	this->MaximumV = 2 * vtkMath::Pi();

	this->JoinU = 1;
	this->JoinV = 1;
	this->TwistU = 0;
	this->TwistV = 0;
	this->ClockwiseOrdering = 1;	
	this->DerivativesAvailable = 1;
}

//----------------------------------------------------------------------------
void vtkmpjToroidSource::Evaluate(double uvw[3], double Pt[3], double Duvw[9])
{
	double u = uvw[0];
	double v = uvw[1];
	double *Du = Duvw;
	double *Dv = Duvw + 3;

	double cu = cos(u);
	double su = sin(u);
	double cv = cos(v);
	double sv = sin(v);
	double t = this->Alpha + this->Beta * cv;

	// The point
	Pt[0] = t * cu;
	Pt[1] = t * su;
	Pt[2] = this->Gamma * sv;

	// The derivatives are:
	Du[0] = -t * su;
	Du[1] = t * cu;
	Du[2] = 0;
	Dv[0] = -this->Beta * sv * cu;
	Dv[1] = -this->Beta * sv * su;
	Dv[2] = this->Gamma * cv;
}

//----------------------------------------------------------------------------
void vtkmpjToroidSource::MakeTriangleStrips ( vtkCellArray * strips, int PtsU, int PtsV )
{
  int id1;
  int id2;

  vtkDebugMacro(<< "Executing MakeTriangleStrips()");

  for ( int i = 0; i < PtsU - 1; ++i )
    {
    // Allocate space
    if ( this->GetJoinV() )
      {
      strips->InsertNextCell( PtsV * 2 + 2 );
      }
    else
      {
      strips->InsertNextCell( PtsV * 2 );
      }

    // Fill the allocated space with the indexes to the points.
    for ( int j = 0; j < PtsV; ++j )
      {
      id1 = j + i * PtsV;
      id2 = (i + 1 ) * PtsV + j;
      if ( this->GetClockwiseOrdering() )
        { 
        strips->InsertCellPoint(id1);
        strips->InsertCellPoint(id2); 
        }
      else
        {
        strips->InsertCellPoint(id2);
        strips->InsertCellPoint(id1);
        }
      }
    // If necessary, connect the ends of the triangle strip.
    if ( this->GetJoinV() )
      {
      if ( this->GetTwistV() )
        {
        id1 = (i + 1) * PtsV;
        id2 = i * PtsV;
        }
      else
        {
        id1 = i * PtsV;
        id2 = (i + 1) * PtsV;
        }
      if ( this->GetClockwiseOrdering() )
        { 
        strips->InsertCellPoint(id1);
        strips->InsertCellPoint(id2); 
        }
      else
        {
        strips->InsertCellPoint(id2);
        strips->InsertCellPoint(id1);
        }
      }
    }
  // If required, connect the last triangle strip to the first by 
  // adding a new triangle strip and filling it with the indexes
  // to the points.
  if ( this->GetJoinU() )
    {
    if ( this->GetJoinV() )
      {
      strips->InsertNextCell( PtsV * 2 + 2 );
      }
    else
      {
      strips->InsertNextCell( PtsV * 2 );
      }
    for ( int j = 0; j < PtsV; ++j )
      {
      if ( this->GetTwistU() )
        {
        id1 = ( PtsU - 1 ) * PtsV + j;
        id2 = PtsV - 1 - j;
        }
      else
        {
        id1 = ( PtsU - 1 ) * PtsV + j;
        id2 = j;
        }
      if ( this->GetClockwiseOrdering() )
        { 
        strips->InsertCellPoint(id1);
        strips->InsertCellPoint(id2); 
        }
      else
        {
        strips->InsertCellPoint(id2);
        strips->InsertCellPoint(id1);
        }
      }

    // If necessary, connect the ends of the triangle strip.
    if ( this->GetJoinV() )
      {
      if ( this->GetTwistU() )
        {
        if ( this->GetTwistV() )
          {
          id1 = PtsV - 1;
          id2 = ( PtsU - 1 ) * PtsV;
          }
        else
          {
          id1 = ( PtsU - 1 ) * PtsV;
          id2 = PtsV - 1;
          }
        }
      else
        {
        if ( this->GetTwistV() )
          {
          id1 = 0;
          id2 = ( PtsU - 1 ) * PtsV;
          }
        else
          {
          id1 = ( PtsU - 1 ) * PtsV;
          id2 = 0;
          }
        }
      if ( this->GetClockwiseOrdering() )
        { 
        strips->InsertCellPoint(id1);
        strips->InsertCellPoint(id2); 
        }
      else
        {
        strips->InsertCellPoint(id2);
        strips->InsertCellPoint(id1);
        }
      }
    }
  vtkDebugMacro(<< "MakeTriangleStrips() finished.");
}

//----------------------------------------------------------------------------
void vtkmpjToroidSource::Execute()
{
  vtkPolyData *output = this->GetOutput();

  // Used to hold the surface
  vtkPolyData * pd = vtkPolyData::New();

  // Adjust so the range this->MinimumU ... this->GetMaximumU(), this->MinimumV
  // ... this->GetMaximumV() is included in the triangulation.
  double MaxU = this->GetMaximumU() + 
    (this->GetMaximumU() - this->GetMinimumU()) /
    (this->UResolution-1);
  int PtsU = this->UResolution;
  double MaxV = this->GetMaximumV() + 
    (this->GetMaximumV() - this->GetMinimumV()) / 
    (this->VResolution-1);
  int PtsV = this->VResolution;
  int totPts = PtsU * PtsV;

  // Scalars associated with each point 
  vtkFloatArray * sval = vtkFloatArray::New(); 
  sval->SetNumberOfTuples( totPts ); 

  // The normals to the surface
  vtkFloatArray * nval = vtkFloatArray::New();
  nval->SetNumberOfComponents(3);
  nval->SetNumberOfTuples(totPts); 

  // Texture coordinates
  double tc[2];
  vtkFloatArray *newTCoords;
  newTCoords = vtkFloatArray::New();
  newTCoords->SetNumberOfComponents(2);
  newTCoords->Allocate(2*totPts);

  vtkPoints * points = vtkPoints::New();
  points->SetNumberOfPoints( totPts );

  double uStep = ( MaxU - this->GetMinimumU() ) / PtsU;
  double vStep = ( MaxV - this->GetMinimumV() ) / PtsV;

  // Find the mid points of the (u,v) map.
  double u0 = this->GetMinimumU();
  double u_mp = (MaxU - u0)/2.0 + u0 - uStep;
  while ( u0 < u_mp ) 
    {
    u0 += uStep;
    }
  
  double v0 = this->GetMinimumV();
  double v_mp = (MaxV - v0)/2.0 + v0 - vStep;
  while ( v0 < v_mp )  
    {
    v0 += vStep;
    }
  u_mp += uStep;
  v_mp += vStep;

  // At this point (u_mp, v_mp) is the midpoint of the (u,v) map and (u0,v0)
  // corresponds to the nearest grid point to the midpoint of the (u,v) map.
  //
  double rel_u = 0; // will be u - u_mp
  double rel_v = 0; // will be v - v_mp

  int k = 0;
  double uv[3];
  uv[0] = this->GetMinimumU() - uStep;

  float MaxI = PtsU - 1;
  float MaxJ = PtsV - 1;

  for ( int i = 0; i < PtsU; ++i )
    {
    uv[0] += uStep;
    uv[1] = this->GetMinimumV() - vStep;

    if ( this->GenerateTextureCoordinates != 0 )
      {
      tc[0] = i/MaxI;
      }
      
    for ( int j = 0; j < PtsV; ++j )
      {
      uv[1] += vStep;

      if ( this->GenerateTextureCoordinates != 0 )
        {
        tc[1] = 1.0 - j/MaxJ;
        newTCoords->InsertNextTuple(tc);
        }
  
      // The point
      double Pt[3];
      // Partial derivative at Pt with respect to u,v,w.
      double Du[9];
      // Partial derivative at Pt with respect to v.
      double *Dv = Du+3;

      // Calculate fn(u)->(Pt,Du).
      this->Evaluate(uv,Pt,Du);

      // Insert the points and scalar.
      points->InsertPoint(k, Pt[0], Pt[1], Pt[2]);
      double scalar;

      if ( this->ScalarMode != SCALAR_NONE )
        {
        switch ( this->ScalarMode )
          {
          case SCALAR_U:
            scalar = uv[0];
            break;
          case SCALAR_V:
            scalar = uv[1];
            break;
          case SCALAR_U0:
            scalar = uv[0] == u0 ? 1 : 0;
            break;
          case SCALAR_V0:
            scalar = uv[1] == v0 ? 1 : 0;
            break;
          case SCALAR_U0V0:
            scalar = 0;
            // u0, v0
            if ( uv[0] == u0 && uv[1] == v0 ) 
              {
              scalar = 3;
              }
            else
              {
              // u0 line
              if ( uv[0] == u0 ) 
                {
                scalar = 1;
                }
              else 
                {
                // v0 line
                if ( uv[1] == v0 ) scalar = 2;
                }
              }
            break;
          case SCALAR_MODULUS:
            rel_u = uv[0] - u_mp;
            rel_v = uv[1] - v_mp;
            scalar = sqrt(rel_u * rel_u + rel_v * rel_v);
            break;
          case SCALAR_PHASE:
            rel_u = uv[0] - u_mp;
            rel_v = uv[1] - v_mp;
            if ( rel_v == 0 && rel_u == 0 )
            {
            scalar = 0;
            }
            else
            {
            scalar = vtkMath::DegreesFromRadians(atan2(rel_v,rel_u));
            if ( scalar < 0 ) scalar += 360;
            }
            break;
          case SCALAR_QUADRANT:
            if ( uv[0] >= u0 && uv[1] >= v0 )
              {
              scalar = 1;
              break;
              }
            if ( uv[0] < u0 && uv[1] >= v0 ) 
              {
              scalar = 2;
              break;
              }
            if ( uv[0] < u0 && uv[1] < v0 ) 
              {
              scalar = 3;
              }
            else 
              {
              scalar = 4;
              }
            break;
          case SCALAR_X:
            scalar = Pt[0];
            break;
          case SCALAR_Y:
            scalar = Pt[1];
            break;
          case SCALAR_Z:
            scalar = Pt[2];
            break;
          case SCALAR_DISTANCE:
            scalar = sqrt(Pt[0]*Pt[0] + Pt[1]*Pt[1] + Pt[2]*Pt[2]);
            break;
          case SCALAR_FUNCTION_DEFINED:
            scalar = 0;
            break;
          case SCALAR_NONE:
          default:
            scalar = 0;
          }
        sval->SetValue(k, scalar);
        }

      // Calculate the normal.
      if ( this->GetDerivativesAvailable() )
        {
        double n[3];
        vtkMath::Cross(Du,Dv,n);
        vtkMath::Normalize(n);
        nval->SetTuple3(k, n[0], n[1], n[2]);
        }

      ++k;
      }
    }

  // Make the triangle strips
  vtkCellArray * strips = vtkCellArray::New(); 
  // This is now a list of ID's defining the triangles.
  this->MakeTriangleStrips ( strips, PtsU, PtsV ); 

  pd->SetPoints( points ); 
  pd->SetVerts( strips );
  pd->SetStrips( strips );
  if ( this->ScalarMode != SCALAR_NONE )
    {
    pd->GetPointData()->SetScalars( sval );
    }

  if ( this->GetDerivativesAvailable() )
    {
    pd->GetPointData()->SetNormals( nval );
    }
  pd->Modified();

  vtkTriangleFilter * tri = vtkTriangleFilter::New();
  vtkPolyDataNormals * norm = vtkPolyDataNormals::New();
  if ( this->GetDerivativesAvailable() )
    {
    //Generate polygons from the triangle strips
    tri->SetInput(pd);
    }
  else
    {
    // Calculate Normals
    norm->SetInput(pd);
    // Generate polygons from the triangle strips
    tri->SetInput(norm->GetOutput());
    }
  tri->PassLinesOn();
  tri->PassVertsOff();
  tri->Update();

  vtkPolyData *outData = this->GetOutput();
  outData->DeepCopy(tri->GetOutput());

  if ( this->GenerateTextureCoordinates != 0 )
    {
    outData->GetPointData()->SetTCoords( newTCoords );
    }

  // Were done, clean up.
  points->Delete();
  sval->Delete();
  nval->Delete();
  newTCoords->Delete();
  strips->Delete();
  pd->Delete();
  tri->Delete();
  norm->Delete();
}

//----------------------------------------------------------------------------
void vtkmpjToroidSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  
  os << indent << "Alpha: " << this->Alpha << "\n";
  os << indent << "Beta: " << this->Beta << "\n";
  os << indent << "Gamma: " << this->Gamma << "\n";
  os << indent << "U Resolution: " << this->UResolution << "\n";
  os << indent << "V Resolution: " << this->VResolution << "\n";

}

