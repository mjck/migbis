

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
#include "vtkmpjImageT2Relax.h"

#define MIN(a,b) ((a)<(b)?(a):(b))

//------------------------------------------------------------------------------
vtkmpjImageT2Relax* vtkmpjImageT2Relax::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageT2Relax");
  if(ret)
    {
    return (vtkmpjImageT2Relax*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageT2Relax;
}

// Construct object with no children.
vtkmpjImageT2Relax::vtkmpjImageT2Relax()
{
  this->EchoTime1=0.0;
  this->EchoTime2=0.0;
  this->ThresholdValue=0;
}

// ----------------------------------------------------------------------------
vtkmpjImageT2Relax::~vtkmpjImageT2Relax()
{
}
//----------------------------------------------------------------------------
void vtkmpjImageT2Relax::SetInput1(vtkImageData *input)
{
  this->vtkProcessObject::SetNthInput(0, input);
}
//----------------------------------------------------------------------------
void vtkmpjImageT2Relax::SetInput2(vtkImageData *input)
{
  this->vtkProcessObject::SetNthInput(1, input);
}
//----------------------------------------------------------------------------
vtkImageData *vtkmpjImageT2Relax::GetInput1()
{ 
  return (vtkImageData*)(this->Inputs[0]);
}
//----------------------------------------------------------------------------
vtkImageData *vtkmpjImageT2Relax::GetInput2()
{
  return (vtkImageData*)(this->Inputs[1]);
}
// ----------------------------------------------------------------------------
void vtkmpjImageT2Relax::ExecuteInformation()
{
  vtkImageData *input=this->GetInput1();

  if (input==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }

  vtkImageData *output=this->GetOutput();

  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
 
  int wholeExtent[6];
  input->GetWholeExtent( wholeExtent );
  output->SetWholeExtent( wholeExtent );
  output->SetNumberOfScalarComponents(1);
}

// ----------------------------------------------------------------------------

void vtkmpjImageT2Relax::SimpleExecute(vtkImageData *input, vtkImageData *output)
{  
  if (this->GetInput1()!=NULL) {        
    
    int nc = this->GetInput1()->GetNumberOfScalarComponents();    
    if (nc == 1) {      
      
      // In case the number of components is one, we need a second input
      if (this->GetInput2()!=NULL) {    
	
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
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }
    }
      
  } else {
    vtkErrorMacro(<<"No Input Image Data !!");
    return;
  }
  
  // Create Output Array
  // -------------------
  int nc = this->GetInput1()->GetNumberOfScalarComponents();
  int dim[3];  this->GetInput1()->GetDimensions(dim);
  
  //vtkImageData* output=this->GetOutput();
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

  int count1=0; // number of voxels processed
  int count2=0; // number of voxels where PD == T2
  
  for (int n=0;n<numscalars;n++)
    {
      float outv = 0.0;
      float v1, v2;
      
      v1=this->GetInput1()->GetPointData()->GetScalars()->GetComponent(n,0);
      v2=(nc==1)?this->GetInput2()->GetPointData()->GetScalars()->GetComponent(n,0):
	this->GetInput1()->GetPointData()->GetScalars()->GetComponent(n,1);
      
      if (MIN(v1,v2) >= this->ThresholdValue) {
	
	if (v1 != v2) 	
	  outv = fabs(this->EchoTime1-this->EchoTime2) / fabs(log(v1 / v2));
	else {
	  count2++;
	  outv = 5000.0;	  
	}

	count1++;
      }
      
      out->SetComponent(n,0,outv);
      
      count++;
      if (count==tenth)
	{
	  pog+=0.1;
	  this->UpdateProgress(pog);
	  count=0;
	}
    }

  fprintf(stdout,"A total of %d image voxels (%4.2f%%) were processed,\n",
	  count1,(float)count1/numscalars*100);
  fprintf(stdout,"where %d voxels (%4.2f%%) had IPDW equal to IT2W.\n",
	  count2,(float)count2/count1*100);
  
  this->UpdateProgress(1.0);
}

