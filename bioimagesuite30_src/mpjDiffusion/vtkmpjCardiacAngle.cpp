

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
#include "vtkmpjCardiacAngle.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjCardiacAngle* vtkmpjCardiacAngle::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjCardiacAngle");
  if(ret)
    {
      return (vtkmpjCardiacAngle*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjCardiacAngle;
}

// ----------------------------------------------------------------------------
// Construct object with no children.
vtkmpjCardiacAngle::vtkmpjCardiacAngle()
{
  this->AngleType = 0;
  this->EigenvectorType = 0;   // 1st
  this->Mask = NULL;
	this->Eigenvectors = NULL;
	this->ReferencePlane = 0;   // Short-axis	
}

// ----------------------------------------------------------------------------
vtkmpjCardiacAngle::~vtkmpjCardiacAngle()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
	if (this->Eigenvectors != NULL)	
    this->Eigenvectors->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjCardiacAngle::ExecuteInformation()
{
  vtkImageData *input=this->GetInput();
  vtkImageData *output=this->GetOutput();
  
  if (input==NULL) {
    vtkErrorMacro(<<"No Input Specified!!");
    return;
  }
  
	if (this->Eigenvectors==NULL) {
    vtkErrorMacro(<<"No Eigenvectors Specified!!");
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
inline void vtkmpjCardiacAngle::ProjectOntoPlane(double v[3], double n[3], double r[3])
{
  double d = vtkMath::Dot(v, n);
  r[0] = v[0] - n[0] * d; 
  r[1] = v[1] - n[1] * d; // r = v - (v . n) n
  r[2] = v[2] - n[2] * d;
}

// ----------------------------------------------------------------------------
double vtkmpjCardiacAngle::ComputeAngle(vtkDataArray *in, vtkDataArray *eig, int eigenvector, int index)
{
	double dot, dot2, angle, theta;
	double n[3], vf[3], vp[3], endoepi[3], apexbase[3], antpost[3], nvf[3];
	double Rz[3][3];	
	
	// get eigenvector to project
  	vf[0] = eig->GetComponent(index, eigenvector*3);
  	vf[1] = eig->GetComponent(index, eigenvector*3+1);
  	vf[2] = eig->GetComponent(index, eigenvector*3+2);
  	(void)vtkMath::Normalize(vf);

	// get endo-epi vector (circumferential plane)
	endoepi[0] = in->GetComponent(index, 0);
	endoepi[1] = in->GetComponent(index, 1);	
	endoepi[2] = in->GetComponent(index, 2);
	(void)vtkMath::Normalize(endoepi);	
	
	// get apex-base vector (radial plane)
	apexbase[0] = in->GetComponent(index, 3);
	apexbase[1] = in->GetComponent(index, 4);	
	apexbase[2] = in->GetComponent(index, 5);
    (void)vtkMath::Normalize(apexbase);		
	
	// get ant-post vector (longitudinal plane)
	antpost[0] = in->GetComponent(index, 6);
	antpost[1] = in->GetComponent(index, 7);	
	antpost[2] = in->GetComponent(index, 8);
	(void)vtkMath::Normalize(antpost);		
			
	// get plane normal
 	n[0] = in->GetComponent(index, this->ReferencePlane*3 + 0);	
  	n[1] = in->GetComponent(index, this->ReferencePlane*3 + 1);
  	n[2] = in->GetComponent(index, this->ReferencePlane*3 + 2);
  	(void)vtkMath::Normalize(n);	
  
	// calculate projected fiber vector
  	this->ProjectOntoPlane(vf, n, vp);
  	(void)vtkMath::Normalize(vp);	

	// calculate angle
	switch(this->ReferencePlane) {
		case 0: dot = vtkMath::Dot(vp, antpost);
			break;
		case 1: dot = vtkMath::Dot(vp, endoepi);
			break;
		case 2: dot = vtkMath::Dot(vp, apexbase);
			break;
	};

	if (this->AngleType == 0)
	{
		/* angle between 0 and 90 degrees */
		angle = vtkMath::DegreesFromRadians(acos(fabs(dot)));

	} else if (this->AngleType == 1) {

		angle = vtkMath::DegreesFromRadians(acos(dot));

		/* angle between -90 and 90 degrees */
		if (this->ReferencePlane == 0) {

			/* circumferential plane */
			dot2 = vtkMath::Dot(vp, apexbase);
			if (dot2 > 0) {
				if (dot < 0)
				   	angle = angle - 180.0;
				else
					angle = angle;	
			} else {
				if (dot < 0)
					angle = 180.0 - angle;
				else
					angle = -angle;
			}

		} else if (this->ReferencePlane == 1) {

			/* radial plane */
			dot2 = vtkMath::Dot(vp, antpost);
			if (dot2 > 0) {
				if (dot < 0)
				   	angle = angle - 180.0;
				else
					angle = angle;	
			} else {
				if (dot < 0)
					angle = 180.0 - angle;
				else
					angle = -angle;
			}

		} else {

			/* longitudinal plane */
			dot2 = vtkMath::Dot(vp, endoepi);
			if (dot2 > 0) {
				if (dot < 0)
				   	angle = angle - 180.0;
				else
					angle = angle;	
			} else {
				if (dot < 0)
					angle = 180.0 - angle;
				else
					angle = -angle;
			}

		}

		
	
		/*dot = vtkMath::Dot(v2,vp);
		dot2 = vtkMath::Dot(v1,vp);
		theta = acos(dot);
	
		if (dot2 > 0) theta*=-1;
		if (dot < 0) theta += 3.14159;
	
		Rz[0][0] = cos(theta);  Rz[0][1] = -sin(theta);  Rz[0][2] = 0;
		Rz[1][0] = sin(theta);  Rz[1][1] = cos(theta);   Rz[1][2] = 0;
		Rz[2][0] = 0;           Rz[2][1] = 0;            Rz[2][2] = 1;
		(void)vtkMath::Multiply3x3(Rz,vf,nvf);
	
		dot = vtkMath::Dot(v2,nvf);
		angle = acos(dot) * vtkMath::RadiansToDegrees();
	
		if (angle > 90) angle -= 180;
		
		if (AngleType == 2)
		  angle = -angle;
		else if (AngleType == 3 && angle < 0) {
		  angle = 180.0 + angle;
		}*/
	
	} else {
		/* angle between 0 and 180 degrees */
	}
	
	return angle;
}

// ----------------------------------------------------------------------------
inline int vtkmpjCardiacAngle::InsideMask(long index)
{
  return (this->Mask != NULL)?(this->Mask->GetPointData()->GetScalars()->GetComponent(index, 0) > 0) : 1;
}

// ----------------------------------------------------------------------------
void vtkmpjCardiacAngle::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
	int dim[3]; input->GetDimensions(dim);
  vtkDataArray *out = output->GetPointData()->GetScalars();
	vtkDataArray *eig = this->Eigenvectors->GetPointData()->GetScalars();
  vtkDataArray *in = input->GetPointData()->GetScalars();

  long index = 0;
  int coord[3];
  double Rz[3][3];
	double angle, dot, dot2;

  for (int k=0;k<dim[2];k++) 
	{    
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
			{
	  		angle = this->InsideMask(index) ? this->ComputeAngle(in, eig, this->EigenvectorType, index) : 180.0;			
				out->SetComponent(index, 0, angle);
	  		++index;	  
			}		  	  
		
    double pog=double(k)/double(dim[2]);
    this->UpdateProgress(pog);
	}
	
  this->UpdateProgress(1.0);
}

