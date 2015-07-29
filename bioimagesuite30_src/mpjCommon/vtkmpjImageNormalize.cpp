

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
#include "vtkmpjImageNormalize.h"
#include "pxisinf.h"


//------------------------------------------------------------------------------
vtkmpjImageNormalize* vtkmpjImageNormalize::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageNormalize");
  if(ret)
    {
    return (vtkmpjImageNormalize*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageNormalize;
}

// Construct object with no children.
vtkmpjImageNormalize::vtkmpjImageNormalize()
{
  this->NumberOfRegions = 1;
  this->RegionStatistics = NULL;
  this->Mask = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjImageNormalize::~vtkmpjImageNormalize()
{ 
  if (this->Mask != NULL)
    this->Mask->Delete();  
  if (this->RegionStatistics != NULL)
    this->RegionStatistics->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageNormalize::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();

  int dim[3];  input->GetDimensions(dim);
  int nc=input->GetNumberOfScalarComponents();
  
  output->SetNumberOfScalarComponents(nc);
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImageNormalize::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
  vtkDataArray *in=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();  
  int numscalars=in->GetNumberOfTuples();
   
  vtkDataArray *mask=NULL;
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  // make sure number of regions is at least 1
  if (this->NumberOfRegions <= 0)
    this->NumberOfRegions = 1;
  
  // Progress computation
  int count = 0;
  float pog = 0.0;
  int tenth= (int)(numscalars / 10.0);
  double min, max;  

  this->UpdateProgress(0.01);
  
  for (int n=0;n<numscalars;n++)      
    {      
      int index = (int)((mask != NULL)?(mask->GetComponent(n, 0)):0);
      index--;

	  min = this->RegionStatistics->GetComponent(index, 0);
	  max = this->RegionStatistics->GetComponent(index, 1);
	  // mean, etc...

      if (index >= 0 && index < this->NumberOfRegions) {
	
	      double value = (double)in->GetComponent(n, 0);
	
	      if (!isnan(value)) {
	  
			  // normalization
			  value = (value - min) / (max - min);
			  //printf("value=%lf, min=%lf, max=%lf\n",value,min,max);
              out->SetComponent(n, 0, value);

	      }
      }
      
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

void vtkmpjImageNormalize::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkpxSimpleImageToImageFilter::PrintSelf(os,indent);  
}
