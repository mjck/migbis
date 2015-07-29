
#include "vtkObjectFactory.h"
#include "vtkSource.h"
#include "vtkmpjImageHessian.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkImageGradient.h"
#include "vtkImageExtractComponents.h"
//#include "vtkpxAppendComponents.h"
#include "vtkImageShiftScale.h"
#include "vtkImageMathematics.h"

//------------------------------------------------------------------------------
vtkmpjImageHessian* vtkmpjImageHessian::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageHessian");
  if(ret)
    {
      return (vtkmpjImageHessian*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageHessian;
}

// Construct object with no children.
vtkmpjImageHessian::vtkmpjImageHessian()
{
  this->EnableSmoothing = 0;
  this->EnableThresholding = 0;
  this->ThresholdValue = 0.0;
  this->AppendSigmaImage = 0;
  this->Sigma=1.0;
  this->Mask=NULL;
}

// ----------------------------------------------------------------------------
vtkmpjImageHessian::~vtkmpjImageHessian()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageHessian::ExecuteInformation()
{
  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  vtkImageData *output=this->GetOutput();
  output->SetScalarTypeToFloat();
  output->SetNumberOfScalarComponents(6 + this->AppendSigmaImage);
}

void vtkmpjImageHessian::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
      {
	vtkErrorMacro(<<"No Input Image Data !!");
	return;
      }
  
  if (input->GetNumberOfScalarComponents()!=1)
    {
      vtkErrorMacro(<<"Only Single Component Images are valid inputs\n");
      return;
    }

  for (int c=0+this->AppendSigmaImage;c<(6+this->AppendSigmaImage);c++) 
    {
      output->GetPointData()->GetScalars()->FillComponent(c,0.0);
      if (this->AppendSigmaImage) 
	output->GetPointData()->GetScalars()->FillComponent(0,this->Sigma);
    }
  
  
  double sp[3]; input->GetSpacing(sp);
  double sig[3];
  for (int ia=0;ia<=2;ia++)
    sig[ia]=this->Sigma*sp[0]/sp[ia];      

  vtkImageGaussianSmooth *sm = NULL;

  if (this->EnableSmoothing) 
    {
      printf("Smoothing: sigmas=%f %f %f\n",sig[0],sig[1],sig[2]);
      sm=vtkImageGaussianSmooth::New();
      sm->SetInput(input);
      sm->SetStandardDeviations(sig);
      sm->SetDimensionality(3);
      sm->Update();
    }
  
  this->UpdateProgress(0.25);

  vtkImageGradient* gradient=vtkImageGradient::New();
  if (this->EnableSmoothing) 
    gradient->SetInput(sm->GetOutput());
  else
    gradient->SetInput(input);
  
  //gradient->HandleBoundariesOff();
  gradient->SetDimensionality(3);
  gradient->Update();
  
  this->UpdateProgress(0.5);
  
  vtkImageData* grad=gradient->GetOutput();
  int dim[3]; grad->GetDimensions(dim);

  // gcomp -- component of gradient
  // gdiff -- direction of second derivative

  int outcomp=0;
  double d, val;

  double sp2[3];
  for (int ia=0;ia<=2;ia++)
    sp2[ia]=sp[ia]*2.0;

  for (int gcomp=0;gcomp<=2;gcomp++)
    for (int gdiff=gcomp;gdiff<=2;gdiff++)
      {
	for (int i=1;i<dim[0]-1;i++)
	  for (int j=1;j<dim[1]-1;j++)
	    for (int k=1;k<dim[2]-1;k++)
	      {
		d=0.0;
		val = input->GetScalarComponentAsDouble(i,j,k,0);
		if ((this->EnableThresholding) && (val < this->ThresholdValue)) 
		  continue;
		if ( ((this->Mask != NULL)?(this->Mask->GetScalarComponentAsDouble(i,j,k, 0) > 0) : 1) < 1)
		  continue;
		switch (gdiff)
		  {
		  case 0:
		    d=(grad->GetScalarComponentAsDouble(i+1,j,k,gcomp)-grad->GetScalarComponentAsDouble(i-1,j,k,gcomp))/(sp2[0]);
		    break;
		  case 1:
		    d=(grad->GetScalarComponentAsDouble(i,j+1,k,gcomp)-grad->GetScalarComponentAsDouble(i,j-1,k,gcomp))/(sp2[1]);
		    break;
		  case 2:
		    d=(grad->GetScalarComponentAsDouble(i,j,k+1,gcomp)-grad->GetScalarComponentAsDouble(i,j,k-1,gcomp))/(sp2[2]);
		    break;
		  }
		output->SetScalarComponentFromDouble(i,j,k,outcomp+this->AppendSigmaImage,d*this->Sigma*this->Sigma);
	      }
	++outcomp;
	this->UpdateProgress(0.5+0.1*double(outcomp));
      }

  gradient->Delete();
  if (this->EnableSmoothing) 
    sm->Delete();
  this->UpdateProgress(1.0);
}

