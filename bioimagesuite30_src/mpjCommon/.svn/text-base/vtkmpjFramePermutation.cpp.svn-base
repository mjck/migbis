

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
#include "vtkmpjFramePermutation.h"
#include "vtkFloatArray.h"
#include "vtkMath.h"
#include "math.h"


//------------------------------------------------------------------------------
vtkmpjFramePermutation* vtkmpjFramePermutation::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjFramePermutation");
  if(ret)
    {
    return (vtkmpjFramePermutation*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjFramePermutation;
}

// Construct object with no children.
vtkmpjFramePermutation::vtkmpjFramePermutation()
{
  this->Permutation = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjFramePermutation::~vtkmpjFramePermutation()
{
  if (this->Permutation != NULL)
    this->Permutation->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjFramePermutation::SimpleExecute(vtkImageData* input, vtkImageData* output)
{

  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }

  vtkDataArray* in=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();
  int numscalars=in->GetNumberOfTuples();

  int dim[3];  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();

  float pog = 0.0;
  int tenth=(int)(numscalars * nc / 10.0);
  
  int count = 0;
  for(int c=0; c<this->Permutation->GetNumberOfTuples(); c++) {
    int target = (int)this->Permutation->GetTuple1(c);
    //    fprintf(stderr,"%d to target=%d\n",c,target);
    for(int n=0;n<numscalars;n++) {
      out->SetComponent(n,c,in->GetComponent(n,target));
      
      count++;
      if (count==tenth) {
	pog+=0.1;
	this->UpdateProgress(pog);
	count=0;
      }
    }
  }

  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
