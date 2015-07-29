

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
#include "vtkmpjCreateSymmetricTensor.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjCreateSymmetricTensor* vtkmpjCreateSymmetricTensor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjCreateSymmetricTensor");
  if(ret)
    {
      return (vtkmpjCreateSymmetricTensor*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjCreateSymmetricTensor;
}

// Construct object with no children.
vtkmpjCreateSymmetricTensor::vtkmpjCreateSymmetricTensor()
{
}

// ----------------------------------------------------------------------------
vtkmpjCreateSymmetricTensor::~vtkmpjCreateSymmetricTensor()
{
}

// ----------------------------------------------------------------------------
void vtkmpjCreateSymmetricTensor::ExecuteInformation()
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
  
  if (nc != 12) {
    vtkErrorMacro(<<"A total of 3 eigenvalues and 3 eigenvectors are required as input!");
    return;
  }
    
  output->SetNumberOfScalarComponents(6);
  output->SetScalarType(VTK_FLOAT);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------
void vtkmpjCreateSymmetricTensor::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  vtkDataArray* in =input->GetPointData()->GetScalars();
  int dim[3];  input->GetDimensions(dim);
  vtkDataArray* out=output->GetPointData()->GetScalars();  

  float D[3][3]={{0,0,0},{0,0,0},{0,0,0}};  /* matrix of eigenvalues */
  float V[3][3], VT[3][3], R[3][3];         /* matrix of eigenvectors */
  float T[3][3];                            /* tensor matrix */

  int index=0;

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  D[0][0] = in->GetComponent(index, 0);  /* l1 */
	  D[1][1] = in->GetComponent(index, 1);  /* l2 */
	  D[2][2] = in->GetComponent(index, 2);  /* l3 */

	  V[0][0] = in->GetComponent(index, 3);  /* e1 x */
	  V[0][1] = in->GetComponent(index, 4);  /* e1 y */
	  V[0][2] = in->GetComponent(index, 5);  /* e1 z */

	  V[1][0] = in->GetComponent(index, 6);  /* e2 x */
	  V[1][1] = in->GetComponent(index, 7);  /* e2 y */
	  V[1][2] = in->GetComponent(index, 8);  /* e2 z */

	  V[2][0] = in->GetComponent(index, 9);  /* e3 x */
	  V[2][1] = in->GetComponent(index, 10); /* e3 y */
	  V[2][2] = in->GetComponent(index, 11); /* e3 z */

	  /* Tensor = V * D * V' */
	  (void) vtkMath::Transpose3x3(V,VT);
	  (void) vtkMath::Multiply3x3(D,VT,R);
	  (void) vtkMath::Multiply3x3(V,R,T);

	  fprintf(stderr,"%f %f %f\n",T[0][0],T[1][0],T[2][0]);
	  fprintf(stderr,"%f %f %f\n",T[0][1],T[1][1],T[2][1]);
	  fprintf(stderr,"%f %f %f\n",T[0][2],T[1][2],T[2][2]);
	  fprintf(stderr,"-----------------------\n");

	  out->SetComponent(index, 0, T[0][0]); /* Dxx */
	  out->SetComponent(index, 1, T[1][0]); /* Dxy */
	  out->SetComponent(index, 2, T[2][0]); /* Dxz */
	  out->SetComponent(index, 3, T[1][1]); /* Dyy */
	  out->SetComponent(index, 4, T[2][1]); /* Dyz */
	  out->SetComponent(index, 5, T[2][2]); /* Dzz */
	  	  
	  ++index;	   	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
    
  }
  
  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
