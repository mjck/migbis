

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



#include "vtkObjectFactory.h"
#include "vtkmpjImageToGrid.h"
#include "vtkFloatArray.h"
#include "vtkStructuredGridWriter.h"

//------------------------------------------------------------------------------
vtkmpjImageToGrid* vtkmpjImageToGrid::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageToGrid");
  if(ret)
    {
      return (vtkmpjImageToGrid*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageToGrid;
}

// ----------------------------------------------------------------------------
vtkmpjImageToGrid::vtkmpjImageToGrid()
{
} 

vtkmpjImageToGrid::~vtkmpjImageToGrid()
{
} 

void vtkmpjImageToGrid::SetInput(vtkImageData *input)
{ 
   this->vtkProcessObject::SetNthInput(0, input);
} 
 
vtkImageData* vtkmpjImageToGrid::GetInput()
{ 
  return (vtkImageData*)(this->Inputs[0]);
}

void vtkmpjImageToGrid::Update()
{
  this->Execute();
}

// ----------------------------------------------------------------------------
void vtkmpjImageToGrid::Execute()
{ 
  vtkIdType numVals;

  vtkImageData *input=this->GetInput();
  this->Output = vtkStructuredGrid::New();
  vtkStructuredGrid *output=this->Output;

  vtkPointData *inPD=input->GetPointData();
  vtkPointData *outPD=output->GetPointData();

  vtkDataArray *in=inPD->GetScalars();
  vtkDataArray *vectors;
  vtkPoints *points;
  
  int dim[3]; input->GetDimensions(dim);

  fprintf(stderr,"1 %d %d %d\n",dim[0],dim[1],dim[2]);
  output->SetDimensions(dim);
  //->SetDimensions(dims);
  
  numVals = in->GetNumberOfTuples();
  fprintf(stderr,"numtuples=%d\n",numVals);
  
  vectors = vtkFloatArray::New();
  vectors->SetNumberOfComponents(3);
  vectors->SetNumberOfTuples(numVals);

  points = vtkPoints::New();
  points->Allocate(numVals);

  //  points->InsertNextPoint(0.1, 0.1, 0.1);

  float p[3];
  int index = 0;
  for (int k=0;k<dim[2];k++) 
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  vectors->SetComponent(index, 0, inPD->GetComponent(index, 0));
	  vectors->SetComponent(index, 1, inPD->GetComponent(index, 1));
	  vectors->SetComponent(index, 2, inPD->GetComponent(index, 2));
	  points->InsertPoint(index, i, j, k);
	  index++;
	}
    
  output->SetPoints(points);
  points->Delete();
  output->GetPointData()->SetVectors(vectors);
  vectors->Delete();

  fprintf(stderr,"3.0 --> %d\n",points->GetNumberOfPoints());
  fprintf(stderr,"3.1 --> %d\n",output->GetNumberOfPoints());
  
  //fprintf(stderr,"writing...\n");
  //vtkStructuredGridWriter *writer = vtkStructuredGridWriter::New();
  //writer->SetInput(output);
  //writer->SetFileName("test.vtk");
  //writer->Update();
  //fprintf(stderr,"done.\n");
  
    fprintf(stderr,"4\n");

  //outPD->CopyVectorsOff();
  //  outPD->PassData(inPD);
}

// ----------------------------------------------------------------------------
