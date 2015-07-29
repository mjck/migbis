

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


#include "vtkmpjVectorStreamlineStatistics.h"

#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkPointData.h"
#include "vtkPolyData.h"
#include "vtkPolyLine.h"

vtkStandardNewMacro(vtkmpjVectorStreamlineStatistics);

// Construct object 
vtkmpjVectorStreamlineStatistics::vtkmpjVectorStreamlineStatistics()
{
  this->NumberOfFibers = 0;
  this->Minimum = this->Maximum = this->Mean = this->Variance = 0.0;
}

void vtkmpjVectorStreamlineStatistics::Execute()
{
  vtkPolyData *input = this->GetInput();
  //vtkPolyData *output = this->GetOutput();
  vtkPointData *pd=input->GetPointData();
  vtkCellData *cd=input->GetCellData();
  
  //vtkPointData *outPD=output->GetPointData();
  //vtkCellArray *inLines = NULL;
  

  vtkIdType numCells = input->GetNumberOfCells();

  vtkDataArray *scalars = cd->GetScalars();
  
  double sum = 0.0, sum2=0.0;
  double min = 0.0, max=0.0;
  double val = 0.0, var = 0.0, mean=0.0;

  if (numCells > 0) {

    min = scalars->GetComponent(0, 0);
    max = min;
    
    for (int i=0; i<numCells; i++) {
      
      val = scalars->GetComponent(0, i);
      if (val < min) min = val;
      if (val > max) max = val;
      
      sum += val;
      sum2 += val*val;      
    }

    mean = sum / (float)numCells;

    if (numCells > 1) {
      var = (sum2 - (((float)numCells)*mean*mean))/((float)numCells-1);
    }
  }

  printf("sum=%f\n", sum);
  printf("sum2=%f\n",sum2);
  printf("mean=%f\n",mean);
  printf("variance=%f\n",var);

  this->NumberOfFibers = numCells;
  this->Minimum = min;
  this->Maximum = max;
  this->Mean = mean;
  this->Variance = var;
}

