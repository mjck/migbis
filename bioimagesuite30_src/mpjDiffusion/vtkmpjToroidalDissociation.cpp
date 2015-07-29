

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
#include "vtkmpjToroidalDissociation.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjToroidalDissociation* vtkmpjToroidalDissociation::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjToroidalDissociation");
  if(ret)
    {
      return (vtkmpjToroidalDissociation*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjToroidalDissociation;
}

// Construct object with no children.
vtkmpjToroidalDissociation::vtkmpjToroidalDissociation()
{
  this->Mask = NULL;
  this->Magnitude = NULL;
  this->MaskOutValue = 0;
  this->EigenvalueThreshold = 0.1;
  this->CaseL1GreaterL2GreaterL3 = 1.0;
  this->CaseL1GreaterL2EqualL3 = 2.0;
  this->CaseL1EqualL2GreaterL3 = 3.0;
  this->CaseL1EqualL2EqualL3 = 4.0;
  this->CaseOther = 0.0;
}

// ----------------------------------------------------------------------------
vtkmpjToroidalDissociation::~vtkmpjToroidalDissociation()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
  if (this->Magnitude != NULL)
    this->Magnitude->Delete();
}

// ----------------------------------------------------------------------------
float vtkmpjToroidalDissociation::ClassifyEigenvalues(double w[3], double thresh)
{	
	double l1 = fabs(w[0]);
	double l2 = fabs(w[1]);
	double l3 = fabs(w[2]);

	//double l1l2 = l1-l2;
	//double l2l3 = l2-l3;

	double d12p = l2 / l1;
	double d23p = l3 / l2;

	int dissoc = 0;

	if (l1 > 0 && l2 > 0 && l3 > 0) {

		if (d12p >= thresh) {
			// L1 == L2
			if (d23p >= thresh) {
				// L1 == L2 == L3
				dissoc = this->CaseL1EqualL2EqualL3;
			} else {
				// L1 == L2 > L3
				dissoc = this->CaseL1EqualL2GreaterL3;
			}
		} else
		{
			// L2 > L3
			if (d23p >= thresh) {
				// L1 > L2 == L3
				dissoc = this->CaseL1GreaterL2EqualL3;
			} else {
				// L1 > L2 > L3
				dissoc = this->CaseL1GreaterL2GreaterL3;
			}
		}

	  } else 
		  dissoc = this->CaseOther;

	  return dissoc;
}

// ----------------------------------------------------------------------------
void vtkmpjToroidalDissociation::ExecuteInformation()
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
void vtkmpjToroidalDissociation::SimpleExecute(vtkImageData *input, vtkImageData *output)
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
  vtkDataArray* mag =NULL;

  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
  if (this->Magnitude != NULL)
    mag = this->Magnitude->GetPointData()->GetScalars();

  int index=0;
  int eigen_dissoc = 0;
  double w[3];

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	

		w[0] = in->GetComponent(index, 0);
		w[1] = in->GetComponent(index, 1);
		w[2] = in->GetComponent(index, 2);

		// eigenvalue classification
		eigen_dissoc = this->ClassifyEigenvalues(w, this->EigenvalueThreshold);

		int output = (mask != NULL)?(mask->GetComponent(index, 0) > 0):1;
		float magval = (mag != NULL)?(mag->GetComponent(index, 0)):0.0; 
	  
	    if (output == 0)
	     eigen_dissoc = this->MaskOutValue;	      
	    
	  out->SetComponent(index, 0, ((float)eigen_dissoc) + magval);     
	  
	  ++index;	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);

}
// ----------------------------------------------------------------------------
