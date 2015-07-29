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
  Module:    $RCSfile: vtkbisCylinderSource.cxx,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkbisCylinderSource.h"

#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkPolyData.h"
#include "vtkTransform.h"
#include "vtkCellArray.h"

#include <math.h>

vtkCxxRevisionMacro(vtkbisCylinderSource, "$Revision: 1.74 $");
vtkStandardNewMacro(vtkbisCylinderSource);

//----------------------------------------------------------------------------
// Construct with default resolution 6, height 1.0, radius 0.5, and capping
// on.
vtkbisCylinderSource::vtkbisCylinderSource(int res)
{

  res = (res < 3 ? 3 : res);
  this->Resolution = res;
}

//----------------------------------------------------------------------------
int vtkbisCylinderSource::RequestData(
  vtkInformation *vtkNotUsed(request),
  vtkInformationVector **vtkNotUsed(inputVector),
  vtkInformationVector *outputVector)
{
  // get the info objects
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  
  // get the output
  vtkPolyData *output = vtkPolyData::SafeDownCast(
  outInfo->Get(vtkDataObject::DATA_OBJECT()));

  double angle;
  int numLines, numPolys, numPts;
  double x[3], xbot;
  int i;
  vtkIdType pts[VTK_CELL_SIZE];
  vtkPoints *newPoints; 
  vtkCellArray *newLines=0;
  vtkCellArray *newPolys=0;
 
  // for streaming
  int piece;
  int numPieces;
  int maxPieces;
  int start, end;
  
  piece = output->GetUpdatePiece();
  if (piece >= this->Resolution && !(piece == 0 && this->Resolution == 0))
    {
    return 1;
    }
  numPieces = output->GetUpdateNumberOfPieces();
  maxPieces = this->Resolution;
  if (numPieces > maxPieces)
    {
    numPieces = maxPieces;
    }
  if (piece >= maxPieces)
    {
    // Super class should do this for us, 
    // but I put this condition in any way.
    return 1;
    }
  start = maxPieces * piece / numPieces;
  end = (maxPieces * (piece+1) / numPieces) - 1;
   
  vtkDebugMacro("ConeSource Executing");
  
  if ( this->Resolution )
    {
    angle = 2.0*3.141592654/this->Resolution;
    }
  else
    {
    angle = 0.0;
    }

    numPts = this->Resolution * 2; 
    numPolys = end - start + 3; 

  newPolys = vtkCellArray::New();
  newPolys->Allocate(newPolys->EstimateSize(numPolys,this->Resolution));

  newPoints = vtkPoints::New();
  newPoints->SetDataTypeToFloat(); //used later during transformation
  newPoints->Allocate(numPts);

  xbot = -this->Height / 2.0;


 //create Resolution triangles and single cap
 // create the bottom.
    if ( this->Capping )
      {
      	for (i=0; i < this->Resolution; i++) 
        {
        x[0] = xbot;
        x[1] = this->Radius * cos (i*angle);
        x[2] = this->Radius * sin (i*angle);
        // Reverse the order
        pts[this->Resolution - i - 1] = newPoints->InsertNextPoint(x);
        }
      newPolys->InsertNextCell(this->Resolution,pts);
      
      // creating top 
       
         for (i=0; i < this->Resolution; i++) 
        {
        x[0] = this->Height / 2.0;
        x[1] = this->Radius * cos (i*angle);
        x[2] = this->Radius * sin (i*angle);
        // Reverse the order
        pts[this->Resolution - i - 1] = newPoints->InsertNextPoint(x);
        }
      newPolys->InsertNextCell(this->Resolution,pts);
    }    
    else{
        for (i=0; i < this->Resolution; i++) 
        {
        	x[0] = this->Height / 2.0;
        	x[1] = this->Radius * cos (i*angle);
        	x[2] = this->Radius * sin (i*angle);
        	// Reverse the order
        	pts[this->Resolution - i - 1] = newPoints->InsertNextPoint(x);
       	}
       	
        for (i=0; i < this->Resolution; i++) 
        {	
        	x[0] = -this->Height / 2.0;
        	x[1] = this->Radius * cos (i*angle);
        	x[2] = this->Radius * sin (i*angle);
        	// Reverse the order
        	pts[this->Resolution - i - 1] = newPoints->InsertNextPoint(x);
        }
      }
      
      // Creating the polygons (sides of the cylinder) connecting the top and bottom disks
      for (i=start; i <= end; i++) 
        {
        pts[0] = end + i + 1; 
        pts[1] = i;
        pts[2] = i+1;
        pts[3] = end + i + 2; 
        
        if (pts[2] == this->Resolution)
        {
	          pts[2] = start;
	          pts[3] = end + 1; 
        }          

        newPolys->InsertNextCell(4,pts);
        }


  // A non-default origin and/or direction requires transformation
  //
  if ( this->Center[0] != 0.0 || this->Center[1] != 0.0 || 
       this->Center[2] != 0.0 || this->Direction[0] != 1.0 || 
       this->Direction[1] != 0.0 || this->Direction[2] != 0.0 )
    {
    vtkTransform *t = vtkTransform::New();
    t->Translate(this->Center[0], this->Center[1], this->Center[2]);
    double vMag = vtkMath::Norm(this->Direction);
    if ( this->Direction[0] < 0.0 && 
         this->Direction[1] == 0.0 && 
         this->Direction[2] == 0.0 )
      {
      // just flip x
      t->RotateWXYZ(180.0,0,1,0);
      }
    else
      {
      t->RotateWXYZ(180.0, (this->Direction[0]+vMag)/2.0,
                    this->Direction[1]/2.0, this->Direction[2]/2.0);
      }
    float *ipts=
      static_cast<vtkFloatArray *>(newPoints->GetData())->GetPointer(0);
    for (i=0; i<numPts; i++, ipts+=3)
      {
      t->TransformPoint(ipts,ipts);
      }
    
    t->Delete();
    }
  
  // Update ourselves
  //
  output->SetPoints(newPoints);
  newPoints->Delete();

  if ( newPolys )
    {
    newPolys->Squeeze(); // we may have estimated size; reclaim some space
    output->SetPolys(newPolys);
    newPolys->Delete();
    }
  else
    {
    output->SetLines(newLines);
    newLines->Delete();
    }

  return 1;
}

