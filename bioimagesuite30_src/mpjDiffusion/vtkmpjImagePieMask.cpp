

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
#include "vtkmpjImagePieMask.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjImagePieMask* vtkmpjImagePieMask::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImagePieMask");
  if(ret)
    {
      return (vtkmpjImagePieMask*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImagePieMask;
}

// Construct object with no children.
vtkmpjImagePieMask::vtkmpjImagePieMask()
{
    this->Mask = NULL;
  	this->NumberOfSlices = 3;
	this->NumberOfPies = 2;
	this->MaskOutValue = 0; 
    this->MaskOutAngle = -1;
	this->Anchor[0]=0; this->Anchor[1]=0; this->Anchor[2]=0;	
	this->Apex[0]=0; this->Apex[1]=0; this->Apex[2]=0;
	this->Base[0]=0; this->Base[1]=0; this->Base[2]=0;
}

// ----------------------------------------------------------------------------
vtkmpjImagePieMask::~vtkmpjImagePieMask()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImagePieMask::ExecuteInformation()
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
  
  if (nc < 3) {
    vtkErrorMacro(<< "Three eigenvalues per voxel are required as input!");
    return;
  }
  
  output->SetNumberOfScalarComponents(1);
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjImagePieMask::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkDataArray* in =input->GetPointData()->GetScalars();
 
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* mask =NULL;
  
  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
  
  float PI = vtkMath::Pi();
  float angle, aangle, pie;
  float v0[3], v1[3];
  float factor = 360.0 / this->NumberOfSlices; 
  
  int first[3], last[3], delta[3];
  //int stride;
  //int afirst[3], alast[3], adelta[3];
  float center[3], anchor[3];
  float scale; 

  if (this->Base[2] <= this->Apex[2]) {
	  last[0] = this->Apex[0];
	  last[1] = this->Apex[1];
	  last[2] = this->Apex[2];

	  first[0] = this->Base[0];
	  first[1] = this->Base[1];
	  first[2] = this->Base[2];	  

	  delta[0] = this->Apex[0] - this->Base[0];
	  delta[1] = this->Apex[1] - this->Base[1];
      delta[2] = this->Apex[2] - this->Base[2];
	 
  } else {

	  last[0] = this->Base[0];
	  last[1] = this->Base[1];
	  last[2] = this->Base[2];
	  
	  first[0] = this->Apex[0];
	  first[1] = this->Apex[1];
	  first[2] = this->Apex[2];
	  
	  delta[0] = this->Base[0] - this->Apex[0];
	  delta[1] = this->Base[1] - this->Apex[1];
      delta[2] = this->Base[2] - this->Apex[2];

  }

  // the strike in k direction
  //stride = delta[2] / (this->NumberOfPies - 1);

  int index=0;
  
  // calculate angle for anchor point
  scale = (this->Anchor[2] - first[2]) / (float)delta[2];
  center[0] = first[0] + scale * delta[0];
  center[1] = first[1] + scale * delta[1];
  center[2] = 0;
		
  // calculate angle for anchor line
  v0[0] = this->Anchor[0]-center[0];
  v0[1] = this->Anchor[1]-center[1];
  v0[2] = 0;		  
  (void)vtkMath::Normalize(v0);
 
  aangle = atan2(v0[1], v0[0]) + PI;
  aangle = (aangle / (2.0 * PI)) * 360.0;
		
  for (int k=0;k<dim[2];k++) {

	  // select correct slice
	  //if (k >= first[2] && k <= last[2] && ((k - first[2]) % stride == 0)) { 
          
		  // determine center point for slice
		  scale = (k - first[2]) / (float)delta[2];
		  center[0] = first[0] + scale * delta[0];
		  center[1] = first[1] + scale * delta[1];
		  center[2] = 0;
		  		  
		  for (int j=0;j<dim[1];j++)
			  for (int i=0;i<dim[0];i++) {

				  // calculate angle for point (i,j)
				  v1[0]=i-center[0]; 
				  v1[1]=j-center[1]; 
				  v1[2]=0;
				  (void)vtkMath::Normalize(v1);

				  // determine angle in degrees
				  angle = atan2(v1[1], v1[0]) + PI;
				  angle = (angle / (2.0 * PI)) * 360.0;                

				  // selection of anchor line
	              if (angle >= aangle)
		             angle -= aangle;
				  else
		             angle += 360.0 - aangle;

				  // output pie number
				  pie = (int)(angle / factor) + 1;	  
	  
				  int output=1;	
				  output = (mask != NULL)?(mask->GetComponent(index, 0) > 0):1;
	  
				  if (output)
				  {
					out->SetComponent(index, 0, pie);     
					//out->SetComponent(index, 1, anchor);
				  } else {
					out->SetComponent(index, 0, this->MaskOutValue);     
					//out->SetComponent(index, 1, this->MaskOutAngle);
				  }

				  ++index;
			  }

		//} else 
			//index += dim[0]*dim[1];

			/* if (k >= ) */ 
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);

}
// ----------------------------------------------------------------------------
