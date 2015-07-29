

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
#include "vtkmpjImageToTensors.h"
#include "vtkFloatArray.h"

//------------------------------------------------------------------------------
vtkmpjImageToTensors* vtkmpjImageToTensors::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageToTensors");
  if(ret)
    {
      return (vtkmpjImageToTensors*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageToTensors;
}

// ----------------------------------------------------------------------------
vtkmpjImageToTensors::vtkmpjImageToTensors()
{
  this->SymmetricTensor = 0;
  this->FlipX = this->FlipY = this->FlipZ = 0;
} 

vtkmpjImageToTensors::~vtkmpjImageToTensors()
{
} 

// ----------------------------------------------------------------------------
void vtkmpjImageToTensors::Execute()
{ 
  vtkIdType numTensors;

  vtkDataSet *input=this->GetInput();
  vtkDataSet *output=this->GetOutput();

  vtkPointData *inPD=input->GetPointData();
  vtkPointData *outPD=output->GetPointData();

  vtkDataArray *in=inPD->GetScalars();
  vtkDataArray *tensors;
  
  output->CopyStructure(input);
  //  output->SetNumberOfScalarComponents(input->GetNumberOfScalarComponents());

  numTensors = in->GetNumberOfTuples();

  tensors = vtkFloatArray::New();
  tensors->SetNumberOfComponents(9);
  tensors->SetNumberOfTuples(numTensors);
  
  float Txx, Tyx, Tzx;
  float Txy, Tyy, Tzy;
  float Txz, Tyz, Tzz;
  
  float flipx = this->FlipX ? -1.0: 1.0;
  float flipy = this->FlipY ? -1.0: 1.0;
  float flipz = this->FlipZ ? -1.0: 1.0;
  
  for(int index=0; index<numTensors; index++) {
  
    if (this->SymmetricTensor) {
      
      Txx = in->GetComponent(index, 0);
      Txy = in->GetComponent(index, 1);
      Txz = in->GetComponent(index, 2);
      
      Tyx = Txy;
      Tyy = in->GetComponent(index, 3);   //3
      Tyz = in->GetComponent(index, 4);   //4
      
      Tzx = Txz * flipx;
      Tzy = Tyz * flipy;
      Tzz = in->GetComponent(index, 5);  //5

    } else {

      Txx = in->GetComponent(index, 0);
      Txy = in->GetComponent(index, 1);
      Txz = in->GetComponent(index, 2);
      
      Tyx = in->GetComponent(index, 3);
      Tyy = in->GetComponent(index, 4);
      Tyz = in->GetComponent(index, 5);
      
      Tzx = in->GetComponent(index, 6);
      Tzy = in->GetComponent(index, 7);
      Tzz = in->GetComponent(index, 8);

    }
    
    /*tensors->SetComponent(index, 0, Txx*flipx);
    tensors->SetComponent(index, 1, Txy*flipy);
    tensors->SetComponent(index, 2, Txz*flipz);
    tensors->SetComponent(index, 3, Tyx*flipx);
    tensors->SetComponent(index, 4, Tyy*flipy);
    tensors->SetComponent(index, 5, Tyz*flipz);
    tensors->SetComponent(index, 6, Tzx*flipx);
    tensors->SetComponent(index, 7, Tzy*flipy);
    tensors->SetComponent(index, 8, Tzz*flipz);
    */

    tensors->InsertTuple9(index,Txx*flipx,Txy*flipy,Txz*flipz,
    			  Tyx*flipx,Tyy*flipy,Tyz*flipz,
    			  Tzx*flipx,Tzy*flipy,Tzz*flipz);

  }
  
  outPD->SetTensors(tensors);
  tensors->Delete();
  
  outPD->CopyTensorsOff();
  outPD->PassData(inPD);
}

// ----------------------------------------------------------------------------
