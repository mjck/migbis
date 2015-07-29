//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
//BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
//BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
//BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
//BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
//BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
//BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//BIOIMAGESUITE_LICENSE  See also  http://www.gnu.org/licenses/gpl.html
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
//BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
//BIOIMAGESUITE_LICENSE 
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: vtkEllipseSource.cxx,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkEllipseSource.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkStreamingDemandDrivenPipeline.h"

#include <math.h>

//----------------------------------------------------------------------------
// Construct Ellipse with radiusx=0.5, radiusy=1.0, radiusz=0.5 and default resolution 8 in both Phi
// and Theta directions. Theta ranges from (0,360) and phi (0,180) degrees.
vtkEllipseSource::vtkEllipseSource(int res)
{
  res = res < 4 ? 4 : res;
  this->RadiusX = 0.5;
  this->RadiusY = 0.5;
  this->RadiusZ = 0.5;
  this->Center[0] = 0.0;
  this->Center[1] = 0.0;
  this->Center[2] = 0.0;

  this->ThetaResolution = res;
  this->PhiResolution = res;
  this->StartTheta = 0.0;
  this->EndTheta = 360.0;
  this->StartPhi = 0.0;
  this->EndPhi = 180.0;
  this->LatLongTessellation = 0;

  this->SetNumberOfInputPorts(0);
}

vtkEllipseSource* vtkEllipseSource::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkEllipseSource");
  if(ret)
      {
	return (vtkEllipseSource*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkEllipseSource;
}

//----------------------------------------------------------------------------
int vtkEllipseSource::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  // get the ouptut
  vtkPolyData *output = vtkPolyData::SafeDownCast(
    outInfo->Get(vtkDataObject::DATA_OBJECT()));

  int i, j;
  int jStart, jEnd, numOffset;
  int numPts, numPolys;
  vtkPoints *newPoints;
  vtkFloatArray *newNormals;
  vtkCellArray *newPolys;
  double x[3], n[3], deltaPhi, deltaTheta, phi, theta, norm;
  double startTheta, endTheta, startPhi, endPhi;
  int base, numPoles=0, thetaResolution, phiResolution;
  vtkIdType pts[4];
  int piece =
    outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_PIECE_NUMBER());
  int numPieces =
    outInfo->Get(vtkStreamingDemandDrivenPipeline::UPDATE_NUMBER_OF_PIECES());

  if (numPieces > this->ThetaResolution)
    {
    numPieces = this->ThetaResolution;
    }
  if (piece >= numPieces)
    {
    // Although the super class should take care of this,
    // it cannot hurt to check here.
    return 1;
    }

  // I want to modify the ivars resoultion start theta and end theta,
  // so I will make local copies of them.  THese might be able to be merged
  // with the other copies of them, ...
  int localThetaResolution = this->ThetaResolution;
  double localStartTheta = this->StartTheta;
  double localEndTheta = this->EndTheta;

  while (localEndTheta < localStartTheta)
    {
    localEndTheta += 360.0;
    }
  deltaTheta = (localEndTheta - localStartTheta) / localThetaResolution;

  // Change the ivars based on pieces.
  int start, end;
  start = piece * localThetaResolution / numPieces;
  end = (piece+1) * localThetaResolution / numPieces;
  localEndTheta = localStartTheta + (double)(end) * deltaTheta;
  localStartTheta = localStartTheta + (double)(start) * deltaTheta;
  localThetaResolution = end - start;

  // Set things up; allocate memory
  //
  vtkDebugMacro("EllipseSource Executing piece index " << piece
                << " of " << numPieces << " pieces.");

  numPts = this->PhiResolution * localThetaResolution + 2;
  // creating triangles
  numPolys = this->PhiResolution * 2 * localThetaResolution;

  newPoints = vtkPoints::New();
  newPoints->Allocate(numPts);
  newNormals = vtkFloatArray::New();
  newNormals->SetNumberOfComponents(3);
  newNormals->Allocate(3*numPts);
  newNormals->SetName("Normals");
  
  newPolys = vtkCellArray::New();
  newPolys->Allocate(newPolys->EstimateSize(numPolys, 3));

  // Create sphere
  //
  // Create north pole if needed
  if ( this->StartPhi <= 0.0 )
    {
    x[0] = this->Center[0];
    x[1] = this->Center[1];
    x[2] = this->Center[2] + this->RadiusZ;
    newPoints->InsertPoint(numPoles,x);

    x[0] = x[1] = 0.0; x[2] = 1.0;
    newNormals->InsertTuple(numPoles,x);
    numPoles++;
    }

  // Create south pole if needed
  if ( this->EndPhi >= 180.0 )
    {
    x[0] = this->Center[0];
    x[1] = this->Center[1];
    x[2] = this->Center[2] - this->RadiusZ;
    newPoints->InsertPoint(numPoles,x);

    x[0] = x[1] = 0.0; x[2] = -1.0;
    newNormals->InsertTuple(numPoles,x);
    numPoles++;
    }

  // Check data, determine increments, and convert to radians
  startTheta = (localStartTheta < localEndTheta ? localStartTheta : localEndTheta);
  startTheta *= vtkMath::Pi() / 180.0;
  endTheta = (localEndTheta > localStartTheta ? localEndTheta : localStartTheta);
  endTheta *= vtkMath::Pi() / 180.0;
  
  startPhi = (this->StartPhi < this->EndPhi ? this->StartPhi : this->EndPhi);
  startPhi *= vtkMath::Pi() / 180.0;
  endPhi = (this->EndPhi > this->StartPhi ? this->EndPhi : this->StartPhi);
  endPhi *= vtkMath::Pi() / 180.0;

  phiResolution = this->PhiResolution - numPoles;
  deltaPhi = (endPhi - startPhi) / (this->PhiResolution - 1);
  thetaResolution = localThetaResolution;
  if (fabs(localStartTheta - localEndTheta) < 360.0)
    {
    ++localThetaResolution;
    }
  deltaTheta = (endTheta - startTheta) / thetaResolution;

  jStart = (this->StartPhi <= 0.0 ? 1 : 0);
  jEnd = (this->EndPhi >= 180.0 ? this->PhiResolution - 1
        : this->PhiResolution);

  this->UpdateProgress(0.1);

  // Create intermediate points
  for (i=0; i < localThetaResolution; i++)
    {
    theta = localStartTheta * vtkMath::Pi() / 180.0 + i*deltaTheta;

    for (j=jStart; j<jEnd; j++)
      {
      phi = startPhi + j*deltaPhi;
      //radiusx = this->RadiusX * sin((double)phi);
      n[0] = this->RadiusX * sin((double)phi) * cos((double)theta);
      n[1] = this->RadiusY * sin((double)phi) * sin((double)theta);
      //n[2] = this->RadiusZ * sin((double)phi) * cos((double)phi);
	  n[2] = this->RadiusZ * cos((double)phi);
      x[0] = n[0] + this->Center[0];
      x[1] = n[1] + this->Center[1];
      x[2] = n[2] + this->Center[2];
      newPoints->InsertNextPoint(x);

      if ( (norm = vtkMath::Norm(n)) == 0.0 )
        {
        norm = 1.0;
        }
      n[0] /= norm; n[1] /= norm; n[2] /= norm;
      newNormals->InsertNextTuple(n);
      }
    this->UpdateProgress (0.10 + 0.50*i/static_cast<float>(localThetaResolution));
    }

  // Generate mesh connectivity
  base = phiResolution * localThetaResolution;

  if (fabs(localStartTheta - localEndTheta) < 360.0)
    {
    --localThetaResolution;
    }
  
  if ( this->StartPhi <= 0.0 )  // around north pole
    {
    for (i=0; i < localThetaResolution; i++)
      {
      pts[0] = phiResolution*i + numPoles;
      pts[1] = (phiResolution*(i+1) % base) + numPoles;
      pts[2] = 0;
      newPolys->InsertNextCell(3, pts);
      }
    }
  
  if ( this->EndPhi >= 180.0 ) // around south pole
    {
    numOffset = phiResolution - 1 + numPoles;
    
    for (i=0; i < localThetaResolution; i++)
      {
      pts[0] = phiResolution*i + numOffset;
      pts[2] = ((phiResolution*(i+1)) % base) + numOffset;
      pts[1] = numPoles - 1;
      newPolys->InsertNextCell(3, pts);
      }
    }
  this->UpdateProgress (0.70);

  // bands in-between poles
  for (i=0; i < localThetaResolution; i++)
    {
    for (j=0; j < (phiResolution-1); j++)
      {
      pts[0] = phiResolution*i + j + numPoles;
      pts[1] = pts[0] + 1;
      pts[2] = ((phiResolution*(i+1)+j) % base) + numPoles + 1;
      if ( !this->LatLongTessellation )
        {
        newPolys->InsertNextCell(3, pts);
        pts[1] = pts[2];
        pts[2] = pts[1] - 1;
        newPolys->InsertNextCell(3, pts);
        }
      else
        {
        pts[3] = pts[2] - 1;
        newPolys->InsertNextCell(4, pts);
        }
      }
    this->UpdateProgress (0.70 + 0.30*i/static_cast<double>(localThetaResolution));
    }

  // Update ourselves and release memeory
  //
  newPoints->Squeeze();
  output->SetPoints(newPoints);
  newPoints->Delete();

  newNormals->Squeeze();
  output->GetPointData()->SetNormals(newNormals);
  newNormals->Delete();

  newPolys->Squeeze();
  output->SetPolys(newPolys);
  newPolys->Delete();

  return 1;
}

//----------------------------------------------------------------------------
//void vtkEllipseSource::PrintSelf(ostream& os, vtkIndent indent)
//{
//  this->Superclass::PrintSelf(os,indent);
//
//  os << indent << "Theta Resolution: " << this->ThetaResolution << "\n";
//  os << indent << "Phi Resolution: " << this->PhiResolution << "\n";
//  os << indent << "Theta Start: " << this->StartTheta << "\n";
//  os << indent << "Phi Start: " << this->StartPhi << "\n";
//  os << indent << "Theta End: " << this->EndTheta << "\n";
//  os << indent << "Phi End: " << this->EndPhi << "\n";
//  os << indent << "RadiusX: " << this->RadiusX << "\n";
//  os << indent << "RadiusY: " << this->RadiusY << "\n";
//  os << indent << "RadiusZ: " << this->RadiusZ << "\n";
//  os << indent << "Center: (" << this->Center[0] << ", " 
//     << this->Center[1] << ", " << this->Center[2] << ")\n";
//  os << indent
//     << "LatLong Tessellation: " << this->LatLongTessellation << "\n";
//}

//----------------------------------------------------------------------------
int vtkEllipseSource::RequestInformation(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{
  // get the info object
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  outInfo->Set(vtkStreamingDemandDrivenPipeline::MAXIMUM_NUMBER_OF_PIECES(),
               -1);

  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_BOUNDING_BOX(),
               this->Center[0] - this->RadiusX,
               this->Center[0] + this->RadiusX,
               this->Center[1] - this->RadiusY,
               this->Center[1] + this->RadiusY,
               this->Center[2] - this->RadiusZ,
               this->Center[2] + this->RadiusZ);

  return 1;
}

