

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


#include "vtkmpjPointSource.h"

#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkTransform.h"

vtkStandardNewMacro(vtkmpjPointSource);

vtkmpjPointSource::vtkmpjPointSource()
{
  this->Resolution[0] = 1.0;
  this->Resolution[1] = 1.0;
  this->Resolution[2] = 1.0;

  this->Origin[0] = 0.0;
  this->Origin[1] = 0.0;
  this->Origin[2] = 0.0;

  this->Spacing[0] = this->Spacing[1] = this->Spacing[2] = 1.0;

  this->Size[0] = 1;
  this->Size[1] = 1;
  this->Size[2] = 1;

}

void vtkmpjPointSource::Execute()
{
  float x[3];
  int i, j, k;
  vtkPoints *newPoints; 
  vtkPolyData *output = this->GetOutput();

  float t0 = this->Spacing[0]*this->Resolution[0]*((float)this->Size[0]) / 2.0;
  float t1 = this->Spacing[1]*this->Resolution[1]*((float)this->Size[1]) / 2.0;
  float t2 = this->Spacing[2]*this->Resolution[2]*((float)this->Size[2]) / 2.0;

  // create transf matrix
  vtkTransform *transform = vtkTransform::New();
  transform->PostMultiply();
  transform->Translate(-t0,-t1,-t2);
  transform->TransformPoint(this->Origin,this->Origin);  
  transform->Delete();
  // Set things up; allocate memory
  //
  //numPts = (this->XResolution+1) * (this->YResolution+1) * (this->ZResolution+1);
  
  newPoints = vtkPoints::New();
  //  newPoints->Allocate(numPts);
  
  // Generate points and point data
  //
  for (k=0; k<(this->Size[2]+1); k++)    
    for (i=0; i<(this->Size[1]+1); i++)
      for (j=0; j<(this->Size[0]+1); j++)
	{
	  
	  x[0] = this->Origin[0] + this->Spacing[0]*this->Resolution[0] * ((float) j);
	  x[1] = this->Origin[1] + this->Spacing[1]*this->Resolution[1] * ((float) i);
	  x[2] = this->Origin[2] + this->Spacing[2]*this->Resolution[2] * ((float) k);
	  
	  newPoints->InsertNextPoint(x);
	}
  
  
  // Update ourselves and release memory
  output->SetPoints(newPoints);
  newPoints->Delete();
  
}





