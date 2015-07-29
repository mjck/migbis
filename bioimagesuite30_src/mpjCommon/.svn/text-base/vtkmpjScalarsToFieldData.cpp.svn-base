

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
#include "vtkmpjScalarsToFieldData.h"
#include "vtkFloatArray.h"

//------------------------------------------------------------------------------
vtkmpjScalarsToFieldData* vtkmpjScalarsToFieldData::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjScalarsToFieldData");
  if(ret)
    {
      return (vtkmpjScalarsToFieldData*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjScalarsToFieldData;
}

// ----------------------------------------------------------------------------
vtkmpjScalarsToFieldData::vtkmpjScalarsToFieldData()
{
  this->FieldName = NULL;
} 

vtkmpjScalarsToFieldData::~vtkmpjScalarsToFieldData()
{
} 

// ----------------------------------------------------------------------------
void vtkmpjScalarsToFieldData::Execute()
{ 
  vtkIdType numTuples;
  vtkIdType numComp;

  vtkDataSet *input=this->GetInput();
  vtkDataSet *output=this->GetOutput();

  vtkPointData *inPD=input->GetPointData();
  vtkPointData *outPD=output->GetPointData();

  vtkDataArray *in=inPD->GetScalars();
  
  output->CopyStructure(input);
  //  output->SetNumberOfScalarComponents(input->GetNumberOfScalarComponents());
  
  numTuples = in->GetNumberOfTuples();
  numComp = in->GetNumberOfComponents(); 

  vtkFloatArray *diffusion = vtkFloatArray::New();
  diffusion->SetName(this->FieldName);
  diffusion->SetNumberOfComponents(numComp);
  diffusion->SetNumberOfTuples(numTuples);
  
  for(int index=0; index<numTuples; index++) {
    
    for(int n=0; n<numComp; n++)
      diffusion->SetComponent(index, n, in->GetComponent(index, n));
    
    float pog=float(index)/float(numTuples);
    this->UpdateProgress(pog);	
  }
  
  outPD->AddArray(diffusion);
  diffusion->Delete();
  
  outPD->CopyScalarsOff();
  outPD->PassData(inPD);

  this->UpdateProgress(1.0);
}

// ----------------------------------------------------------------------------
