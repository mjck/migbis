

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


#include "vtkmpjStreamlineDistance.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyLine.h"

vtkStandardNewMacro(vtkmpjStreamlineDistance);

// Construct object 
vtkmpjStreamlineDistance::vtkmpjStreamlineDistance()
{
  this->ReferenceIndex = 0;
  this->TargetIndex = 0;
  this->Minimum = 0;
  this->Maximum = 0;
  this->Mean = 0;
}

// ----------------------------------------------------------------------------
void vtkmpjStreamlineDistance::SetReference(vtkDataSet *input)
{
  this->vtkProcessObject::SetNthInput(0, input);  
}

// ----------------------------------------------------------------------------
void vtkmpjStreamlineDistance::SetTarget(vtkDataSet *input)
{
  this->vtkProcessObject::SetNthInput(1, input);  
}

// ----------------------------------------------------------------------------
vtkPolyData *vtkmpjStreamlineDistance::GetInput(int index)
{
  if (this->NumberOfInputs <= index)
    {
      return NULL;
    }
    
  return (vtkPolyData *)(this->Inputs[index]);
}

// ----------------------------------------------------------------------------
void vtkmpjStreamlineDistance::Execute()
{
  vtkPolyData *input1 = GetInput(0);
  vtkPolyData *input2 = GetInput(1);
  vtkPointData *pd1=input1->GetPointData();
  vtkPointData *pd2=input2->GetPointData();
  
  vtkPolyData *output = this->GetOutput();
  vtkPointData *outPD = output->GetPointData();
  vtkFloatArray *disterr = vtkFloatArray::New();
  disterr->SetName("fractani");
  disterr->SetNumberOfComponents(1);

  //vtkPointData *outPD=output->GetPointData();
  vtkCellArray *inLines1 = NULL;
  vtkCellArray *inLines2 = NULL;
  
  vtkPoints *inPts1;
  vtkPoints *inPts2;

  vtkIdType numPts1 = 0, numPts2 = 0;
  vtkIdType numLines1, numLines2;
  
  int abort=0;
  vtkIdType id,inCellId;
  
  // Check input and initialize
  //  
  if ( !(inPts1=input1->GetPoints()) || 
       (numPts1 = inPts1->GetNumberOfPoints()) < 1 ||
       !(inLines1 = input1->GetLines()) || 
       (numLines1 = inLines1->GetNumberOfCells()) < 1 )
    {
      return;
    }

  if ( !(inPts2=input2->GetPoints()) || 
       (numPts2 = inPts2->GetNumberOfPoints()) < 1 ||
       !(inLines2 = input2->GetLines()) || 
       (numLines2 = inLines2->GetNumberOfCells()) < 1 )
    {
      return;
    }
  
  vtkIdType npts;
  vtkIdType npts1=0, *pts1=NULL;
  vtkIdType npts2=0, *pts2=NULL;

  inLines1->GetCell(this->ReferenceIndex, npts1, pts1);
  inLines2->GetCell(this->TargetIndex, npts2, pts2);
  
  printf("Reference: npts1=%d\n",npts1);
  printf("Target: npts2=%d\n",npts2);
  
  vtkPoints *newPts = vtkPoints::New();
  vtkCellArray *newCells = vtkCellArray::New();

  if (npts1 < npts2)
    npts = npts1;
  else npts = npts2;
  
  double x1[3], x2[3], x3[3];
  double dist, dist2;
  double distsum=0.0; 
  double dist2sum=0.0;


  if (npts > 1) {
    
    distsum = 0.0;
    
    newCells->InsertNextCell(npts);

    // compute statistics for fiber
    for(int i=0; i<npts; i++) {	
      
      // fetch point
      inPts1->GetPoint(pts1[i],x1);
            
      // find closest point
      inPts2->GetPoint(pts2[i],x2);
      float min = vtkMath::Distance2BetweenPoints(x1,x2);
      vtkIdType minpos = pts2[i];
      
      for(int j=0; j<npts; j++) {
	inPts2->GetPoint(pts2[j],x2);
	dist2 = vtkMath::Distance2BetweenPoints(x1,x2);
	if (dist2 < min) {
	  min = dist2; minpos = pts2[j];
	}
      }

      inPts2->GetPoint(minpos,x2);
      
      x3[0] = (x1[0] + x2[0]) / 2.0;
      x3[1] = (x1[1] + x2[1]) / 2.0;
      x3[2] = (x1[2] + x2[2]) / 2.0;

      // compute distance
      dist2 = vtkMath::Distance2BetweenPoints(x1,x2);
      dist = sqrt(dist2);
      
      if (dist < this->Minimum) this->Minimum = dist;
      if (dist > this->Maximum) this->Maximum = dist;
      
      dist2sum += dist2;
      distsum += sqrt(dist2);

      // insert distance
      disterr->InsertNextTuple(&dist);

      id = newPts->InsertNextPoint(x3);

      newCells->InsertCellPoint(id);

    }
    
    // compute mean distance
    this->Mean = distsum / (float)npts;
    
  } else {
    printf("npts=%d\n",npts);
  }
  
  outPD->AddArray(disterr);
  disterr->Delete();
  
  output->SetPoints(newPts);
  newPts->Delete();
  
  output->SetLines(newCells);
  newCells->Delete();
}

