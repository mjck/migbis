

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
#include "vtkmpjImageToVectors.h"
#include "vtkFloatArray.h"

//------------------------------------------------------------------------------
vtkmpjImageToVectors* vtkmpjImageToVectors::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageToVectors");
  if(ret)
    {
      return (vtkmpjImageToVectors*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageToVectors;
}

// ----------------------------------------------------------------------------
vtkmpjImageToVectors::vtkmpjImageToVectors()
{
  this->FlipX = this->FlipY = this->FlipZ = 0;
} 

vtkmpjImageToVectors::~vtkmpjImageToVectors()
{
} 

// ----------------------------------------------------------------------------
void vtkmpjImageToVectors::Execute()
{ 
  vtkIdType numVectors;

  vtkDataSet *input=this->GetInput();
  vtkDataSet *output=this->GetOutput();

  vtkPointData *inPD=input->GetPointData();
  vtkPointData *outPD=output->GetPointData();

  vtkDataArray *in=inPD->GetScalars();
  vtkDataArray *vectors;
  
  output->CopyStructure(input);

  numVectors = in->GetNumberOfTuples();

  vectors = vtkFloatArray::New();
  vectors->SetNumberOfComponents(3);
  vectors->SetNumberOfTuples(numVectors);

  float flipx = this->FlipX ? -1.0 : 1.0;
  float flipy = this->FlipY ? -1.0 : 1.0;
  float flipz = this->FlipZ ? -1.0 : 1.0;

  for(int index=0; index<numVectors; index++) {
    
    //    fprintf(stderr,"%f %f %f\n",inPD->GetComponent(index, 0),inPD->GetComponent(index, 1),inPD->GetComponent(index, 2));
    vectors->SetComponent(index, 0, flipx * in->GetComponent(index, 0));
    vectors->SetComponent(index, 1, flipy * in->GetComponent(index, 1));
    vectors->SetComponent(index, 2, flipz * in->GetComponent(index, 2));
  }
  
  outPD->SetVectors(vectors);
  vectors->Delete();
  
  outPD->CopyVectorsOff();
  outPD->PassData(inPD);
}

// ----------------------------------------------------------------------------
