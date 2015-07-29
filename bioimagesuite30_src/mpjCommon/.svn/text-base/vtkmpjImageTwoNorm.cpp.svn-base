

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
#include "vtkSource.h"
#include "vtkpxStructuredPointsToStructuredPointsFilter.h"
#include "vtkmpjImageTwoNorm.h"

//------------------------------------------------------------------------------
vtkmpjImageTwoNorm* vtkmpjImageTwoNorm::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageTwoNorm");
  if(ret)
    {
    return (vtkmpjImageTwoNorm*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageTwoNorm;
}

// Construct object with no children.
vtkmpjImageTwoNorm::vtkmpjImageTwoNorm()
{
}

// ----------------------------------------------------------------------------
vtkmpjImageTwoNorm::~vtkmpjImageTwoNorm()
{
}
//----------------------------------------------------------------------------
void vtkmpjImageTwoNorm::SetInput1(vtkStructuredPoints *input)
{
  this->vtkProcessObject::SetNthInput(0, input);
}
//----------------------------------------------------------------------------
void vtkmpjImageTwoNorm::SetInput2(vtkStructuredPoints *input)
{
  this->vtkProcessObject::SetNthInput(1, input);
}
//----------------------------------------------------------------------------
vtkStructuredPoints *vtkmpjImageTwoNorm::GetInput1()
{ 
  return (vtkStructuredPoints*)(this->Inputs[0]);
}
//----------------------------------------------------------------------------
vtkStructuredPoints *vtkmpjImageTwoNorm::GetInput2()
{
  return (vtkStructuredPoints*)(this->Inputs[1]);
}
// ----------------------------------------------------------------------------
void vtkmpjImageTwoNorm::ExecuteInformation()
{
  vtkStructuredPoints *input=this->GetInput1();

  if (input==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }

  vtkStructuredPoints *output=this->GetOutput();

  this->vtkpxStructuredPointsToStructuredPointsFilter::ExecuteInformation();
 
  int wholeExtent[6];
  input->GetWholeExtent( wholeExtent );
  output->SetWholeExtent( wholeExtent );
  output->SetNumberOfScalarComponents(1);
}

// ----------------------------------------------------------------------------

void vtkmpjImageTwoNorm::Execute()
{  
  if (this->GetInput1()!=NULL && this->GetInput2()!=NULL) {        
    
    int nc1 = this->GetInput1()->GetNumberOfScalarComponents();    
    int nc2 = this->GetInput2()->GetNumberOfScalarComponents();

    if (nc1 != nc2) {
      vtkErrorMacro(<<"Multiple Images have different number of components !!");
      return;
    }

    int dim_0[3]; int dim_1[3];
    this->GetInput1()->GetDimensions(dim_0);
    this->GetInput2()->GetDimensions(dim_1);
	
    int sum=0;
    for (int kk=0;kk<=2;kk++)
      sum+=abs(dim_1[kk]-dim_0[kk]);
    if (sum!=0)
      {
	vtkErrorMacro(<<"Multiple Images have different Dimensions !!");
	return;
      } 
    
  } else {
    vtkErrorMacro(<<"Two images are required as input!!");
    return;
  }
  
  // Create Output Array
  // -------------------
  int nc = this->GetInput1()->GetNumberOfScalarComponents();
  int dim[3];  this->GetInput1()->GetDimensions(dim);
  
  vtkStructuredPoints* output=this->GetOutput();
  output->SetSpacing(this->GetInput1()->GetSpacing());
  output->SetOrigin(this->GetInput1()->GetOrigin());
  output->SetScalarType(VTK_FLOAT);
  output->SetNumberOfScalarComponents(1);
  output->SetDimensions(dim[0],dim[1],dim[2]);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  output->AllocateScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();
  int numscalars=out->GetNumberOfTuples();

  this->UpdateProgress(0.01);
  float pog=0.00;

  float tenth=numscalars/10.0;
  int count=0;

  for (int n=0;n<numscalars;n++)
    {
      float v1, v2, vout;
      float sum2=0.0;      
      
      for (int i=0;i<nc;i++) {
	v1=this->GetInput1()->GetPointData()->GetScalars()->GetComponent(n,i);
	v2=this->GetInput2()->GetPointData()->GetScalars()->GetComponent(n,i);	
	vout = (v1-v2)*(v1-v2);
	sum2 += vout;
      }
      
      vout = sqrt(sum2);
      
      out->SetComponent(n,0,vout);
      
      count++;
      if (count==tenth)
	{
	  pog+=0.1;
	  this->UpdateProgress(pog);
	  count=0;
	}
    }
  
  this->UpdateProgress(1.0);
}

