

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
#include "vtkmpjHeartSheetAngle.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjHeartSheetAngle* vtkmpjHeartSheetAngle::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjHeartSheetAngle");
  if(ret)
    {
      return (vtkmpjHeartSheetAngle*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjHeartSheetAngle;
}

// Construct object with no children.
vtkmpjHeartSheetAngle::vtkmpjHeartSheetAngle()
{
  this->Mask = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjHeartSheetAngle::~vtkmpjHeartSheetAngle()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjHeartSheetAngle::ExecuteInformation()
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
  
  output->SetNumberOfScalarComponents(1);
  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjHeartSheetAngle::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  int nc = input->GetNumberOfScalarComponents();
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* in=input->GetPointData()->GetScalars(); 
  vtkDataArray* out=output->GetPointData()->GetScalars();  
  
  vtkDataArray* mask=NULL;
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  float sum, dot, angle, dot2;
  float v1[3], v2[3], v3[3], temp[3];
  float vp[3], vf[3], nvf[3], sign, theta;
  float Rz[3][3];
  int index=0;
  int inmask;
  
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;
	  
	  if (inmask) {
	    
	    // calculate lv coordinate system
	    // radial vector
	    v1[0]=i-this->Center[0]; 
	    v1[1]=j-this->Center[1]; 
	    v1[2]=0;
	    (void)vtkMath::Normalize(v1);
	    
	    // longitudinal vector
	    v3[0]=0.0;
	    v3[1]=0.0;
	    v3[2]=1.0;
	    
	    // get circumferential by cross-product
	    vtkMath::Cross(v1,v3,v2);	

	    // get secondary eigenvector
	    vf[0] = in->GetComponent(index, 6);
	    vf[1] = in->GetComponent(index, 7);
	    vf[2] = in->GetComponent(index, 8);
	    (void)vtkMath::Normalize(vf);
	    if (vf[2] > 0) {
	      vf[0]=-vf[0]; vf[1]=-vf[1]; vf[2]=-vf[2];
	    }
	    
	    // projected fiber vector
	    vp[0] = vf[0];
	    vp[1] = vf[1];
	    vp[2] = 0;
	    (void)vtkMath::Normalize(vp);
	    
	    dot = vtkMath::Dot(v1,vp);
	    dot2 = vtkMath::Dot(v2,vp);
	    theta = acos(dot);
	    
	    if (dot2 < 0) theta*=-1;
	    //if (dot > 0) theta += 3.14159;
	    
	    Rz[0][0] = cos(theta);  Rz[0][1] = -sin(theta);  Rz[0][2] = 0;
	    Rz[1][0] = sin(theta);  Rz[1][1] = cos(theta);   Rz[1][2] = 0;
	    Rz[2][0] = 0;           Rz[2][1] = 0;            Rz[2][2] = 1;

	    (void)vtkMath::Multiply3x3(Rz,vf,nvf);
	    (void)vtkMath::Normalize(nvf);

	    dot = vtkMath::Dot(v1,nvf);
#if ((VTK_MAJOR_VERSION >= 5)&&(VTK_MINOR_VERSION >= 4))
            angle = vtkMath::DegreesFromRadians(acos(dot));
#else
            angle = acos(dot) * vtkMath::RadiansToDegrees();
#endif

	    if (angle > 90) angle -= 180;
	    //if (angle > 0) angle-=90; else angle+=90;

	    out->SetComponent(index, 0, angle);

	  } else {
	    out->SetComponent(index, 0, 180.0);
	  }
	  
	  ++index;	   	 
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
