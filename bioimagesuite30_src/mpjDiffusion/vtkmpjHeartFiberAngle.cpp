

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
#include "vtkmpjHeartFiberAngle.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjHeartFiberAngle* vtkmpjHeartFiberAngle::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjHeartFiberAngle");
  if(ret)
    {
      return (vtkmpjHeartFiberAngle*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjHeartFiberAngle;
}

// Construct object with no children.
vtkmpjHeartFiberAngle::vtkmpjHeartFiberAngle()
{
  this->AngleRepresentation = 0;
  this->OutputCoordinates = 0;
  this->Center[0] = this->Center[1] = this->Center[2] = 0;
  this->Center2[0] = this->Center2[1] = this->Center2[2] = 1;
  this->Mask = NULL;
}

// ----------------------------------------------------------------------------
vtkmpjHeartFiberAngle::~vtkmpjHeartFiberAngle()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjHeartFiberAngle::ExecuteInformation()
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
  
  if (this->OutputCoordinates == 1)
  	output->SetNumberOfScalarComponents(10);
  else
  	output->SetNumberOfScalarComponents(1);
  
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjHeartFiberAngle::SimpleExecute(vtkImageData *input, vtkImageData *output)
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
  float u[3], q[3], r[3], c[3];
  float vp[3], vp1[3], vf[3], nvf[3], sign, theta;
  float Rz[3][3];
  int index=0;
  int inmask;
  
  float pa[3], pb[3], cx, cy;
  
  // establish ordering of apex and base points based on the slice number
  if (this->Center2[2] > this->Center[2]) {
    pa[0] = this->Center[0]; pa[1] = this->Center[1]; pa[2] = this->Center[2];
    pb[0] = this->Center2[0]; pb[1] = this->Center2[1]; pb[2] = this->Center2[2];
  } else {
    pa[0] = this->Center2[0]; pa[1] = this->Center2[1]; pa[2] = this->Center2[2];
    pb[0] = this->Center[0]; pb[1] = this->Center[1]; pb[2] = this->Center[2];
  }

  //printf("pa=[%f,%f,%f]\n pb=[%f,%f,%f]\n",pa[0],pa[1],pa[2],pb[0],pb[1],pb[2]);

  // compute longitudinal axis from ordered points apex and base
  u[0] = pb[0] - pa[0]; 
  u[1] = pb[1] - pa[1]; 
  u[2] = pb[2] - pa[2]; 
  (void)vtkMath::Normalize(u);

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{
	  inmask = (mask != NULL)?(mask->GetComponent(index, 0) > 0) : 1;
	  
	  if (inmask) {
    
	    // calculate vector to point
	  	q[0] = i - pa[0];
	  	q[1] = j - pa[1];
	  	q[2] = k - pa[2];
	  	(void)vtkMath::Normalize(q);

	  	// determine circumferential vector
  		vtkMath::Cross(u,q,c);	
  		(void)vtkMath::Normalize(c);

  		// determine radial vector
  		vtkMath::Cross(u,c,r);	
  		(void)vtkMath::Normalize(r);

    	//if (i==45 && j==16 && k==35)
    	//	printf("v1=[%f,%f,%f]\n v2=[%f,%f,%f], v3=[%f,%f,%f]\n",v1[0],v1[1],v1[2], v2[0],v2[1],v2[2], v3[0],v3[1],v3[2]);
	    
	    // retrieve fiber vector
	    vf[0] = in->GetComponent(index, 0);
	    vf[1] = in->GetComponent(index, 1);
	    vf[2] = in->GetComponent(index, 2);
	    (void)vtkMath::Normalize(vf);

	    // orient vector fiber to match the longitudinal orientation
	    dot = vtkMath::Dot(vf, u);
	    if (dot < 0) {
	    	vf[0]=-vf[0]; vf[1]=-vf[1]; vf[2]=-vf[2];
	    }
	    
	    // project fiber direction onto plane defined by longitudinal vector
	    dot = vtkMath::Dot(vf, u);
	    vp[0] = vf[0] - dot * u[0];
	    vp[1] = vf[1] - dot * u[1];
	    vp[2] = vf[2] - dot * u[2];
	    (void)vtkMath::Normalize(vp);
	    
	    // output coordinates if requested
	    if (this->OutputCoordinates == 1)
  		{
  			out->SetComponent(index, 1, u[0]);
  			out->SetComponent(index, 2, u[1]);
			out->SetComponent(index, 3, u[2]);

			out->SetComponent(index, 4, c[0]);
  			out->SetComponent(index, 5, c[1]);
			out->SetComponent(index, 6, c[2]);

			out->SetComponent(index, 7, r[0]);
  			out->SetComponent(index, 8, r[1]);
			out->SetComponent(index, 9, r[2]);
  		}

  		// calculate the helix angle
		dot = vtkMath::Dot(vp, vf);
		angle = vtkMath::DegreesFromRadians(acos(dot));

	    if (AngleRepresentation == 1 || AngleRepresentation == 2 || AngleRepresentation == 3) 
	    {
	    	// projection of vp onto the plane defined by the circumferential vector
	    	dot2 = vtkMath::Dot(vp, r);
	    	vp1[0] = vp[0] - dot2 * r[0];
	    	vp1[1] = vp[1] - dot2 * r[1];
	    	vp1[2] = vp[2] - dot2 * r[2];
	    	(void)vtkMath::Normalize(vp1);

	    	if (AngleRepresentation == 1) {
	    		if (vtkMath::Dot(vp1, c) < 0)
	    			angle = -angle;
	    	} else if (AngleRepresentation == 2) {
	    		if (vtkMath::Dot(vp1, c) > 0)
	    			angle = -angle;
	    	} else {
	    		if (vtkMath::Dot(vp1, c) < 0)
	    			angle = 180 - angle;
	    	}	
	    } 
	    
	    //if (i==45 && j==16 && k==35)
	    //	printf("%f, %f\n",angle,theta);

	    out->SetComponent(index, 0, angle);
	    
	  } else {
	    
	    if (this->OutputCoordinates == 1)
	    	for(int l=1;l<10;l++)
	    		out->SetComponent(index, l, 0.0);

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
