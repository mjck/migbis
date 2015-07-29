

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


#include "vtkmpjVertexSource.h"
#include "vtkPointSet.h"

vtkStandardNewMacro(vtkmpjVertexSource);

// Construct object with scaling on and scale factor 1.0. 
vtkmpjVertexSource::vtkmpjVertexSource()
{
  this->NumberOfPoints = 0;
  this->Points = NULL;
  this->ScaleFactor = 1.0;
}

vtkmpjVertexSource::~vtkmpjVertexSource()
{
  if (this->Points != NULL)
    this->Points->Delete();
}

void vtkmpjVertexSource::Execute()
{
  double *pt, x[3];
  vtkIdType i;

  vtkPoints *newPoints;
  vtkCellArray *newVerts;
  vtkPolyData *output = this->GetOutput();

  if (this->Points == NULL) 
    return;
  
  int numpts = this->Points->GetNumberOfPoints();
 
  newPoints = vtkPoints::New();
  newPoints->Allocate(numpts);
  newVerts = vtkCellArray::New();
  newVerts->Allocate(newVerts->EstimateSize(1,numpts));
  
  newVerts->InsertNextCell(numpts);
  
  for (i=0; i<numpts; i++)
    {
      pt = this->Points->GetPoint(i);
      
      x[0] = pt[0] * this->ScaleFactor;
      x[1] = pt[1] * this->ScaleFactor;
      x[2] = pt[2] * this->ScaleFactor;
      
      newVerts->InsertCellPoint(newPoints->InsertNextPoint(x));
    }
  
  //
  // Update ourselves and release memory
  //
  output->SetPoints(newPoints);
  newPoints->Delete();
  
  output->SetVerts(newVerts);
  newVerts->Delete();
}

void vtkmpjVertexSource::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
  
  os << indent << "Scale Factor: " << this->ScaleFactor << "\n";
}
