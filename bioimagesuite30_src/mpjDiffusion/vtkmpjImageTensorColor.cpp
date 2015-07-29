

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
#include "vtkmpjImageTensorColor.h"
#include "vtkMath.h"
#include "math.h"

//------------------------------------------------------------------------------
vtkmpjImageTensorColor* vtkmpjImageTensorColor::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageTensorColor");
  if(ret)
    {
      return (vtkmpjImageTensorColor*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageTensorColor;
}

// Construct object with no children.
vtkmpjImageTensorColor::vtkmpjImageTensorColor()
{
  this->Mask = NULL;
  this->Magnitude = NULL;
  this->MaskOutValue[0] = 0;
  this->MaskOutValue[1] = 0;
  this->MaskOutValue[2] = 0;
  this->SaturationScale = 0.5;
  this->Scaling = 1.0;
  this->ColorSchemeType = VTK_MPJ_DEC_ABSOLUTE;
}

// ----------------------------------------------------------------------------
vtkmpjImageTensorColor::~vtkmpjImageTensorColor()
{
  if (this->Mask != NULL)
    this->Mask->Delete();
  if (this->Magnitude != NULL)
    this->Magnitude->Delete();
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorColor::ExecuteInformation()
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
  
  if (nc < 9) {
    vtkErrorMacro(<< "Three eigenvalues and three eigenvectors per voxel are required as input!");
    return;
  }
  
  output->SetNumberOfScalarComponents(3);  
  output->SetScalarType(VTK_UNSIGNED_CHAR);
  output->SetWholeExtent(0,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
}

// ----------------------------------------------------------------------------

void vtkmpjImageTensorColor::AbsoluteValueScheme(vtkImageData *input, vtkImageData *output)
{
  vtkDataArray* in =input->GetPointData()->GetScalars();  
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* mask =NULL;
  vtkDataArray *mag = NULL;
  
  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
  
  double mrange[2];
  if (this->Magnitude != NULL) {
    mag = this->Magnitude->GetPointData()->GetScalars();
    mag->GetRange(mrange);
  }

  int index = 0;
  float v[3], mgn;
  float rgb[3], nrgb[3];
  float scale = this->Scaling;
  
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  rgb[0] = rgb[1] = rgb[2] = 0;
	  
	  if ((mask != NULL)?(mask->GetComponent(index, 0) > 0):1)
	    {
	      v[0] = in->GetComponent(index, 0);
	      v[1] = in->GetComponent(index, 1);
	      v[2] = in->GetComponent(index, 2);

	      vtkMath::Normalize(v);
	      
	      rgb[0] = fabs(v[0]);
	      rgb[1] = fabs(v[1]);
	      rgb[2] = fabs(v[2]);
	      
	      /* compute color */
	      mgn = (mag != NULL) ? \
		((mag->GetComponent(index, 0) - mrange[0]) / mrange[1]): 1.0;	 

	      rgb[0] *= mgn;
	      rgb[1] *= mgn;
	      rgb[2] *= mgn;

	      /* RGBs have to be in [0,255] */
	      nrgb[0] = rgb[0]*255*scale;
	      nrgb[1] = rgb[1]*255*scale;
	      nrgb[2] = rgb[2]*255*scale;
	      
	      out->SetComponent(index, 0, nrgb[0] < 256 ? nrgb[0] : 255);
	      out->SetComponent(index, 1, nrgb[1] < 256 ? nrgb[1] : 255);
	      out->SetComponent(index, 2, nrgb[2] < 256 ? nrgb[2] : 255);
	      
	    } else {
	      
	      /* RGBs have to be in [0,255] */
	      out->SetComponent(index, 0, MaskOutValue[0]);
	      out->SetComponent(index, 1, MaskOutValue[1]);
	      out->SetComponent(index, 2, MaskOutValue[2]);
	      
	    }
	  
	  ++index;
	  
	}
     
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
  }
  
  this->UpdateProgress(1.0);
}

void vtkmpjImageTensorColor::RotationalSymmetryScheme(vtkImageData *input, vtkImageData *output)
{
  vtkDataArray* in =input->GetPointData()->GetScalars();
  
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* mask =NULL;
  vtkDataArray *mag = NULL;
  
  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
  
  double mrange[2];
  if (this->Magnitude != NULL) {
    mag = this->Magnitude->GetPointData()->GetScalars();
    mag->GetRange(mrange);
  }
  
  int index = 0;
  float v[3];
  float rgb[3], nrgb[3], hsv[3];
  float theta, phi;
  float vlenxy, vlen;
  float pi = vtkMath::Pi();
  float ps = this->SaturationScale;
  float scale = this->Scaling;

  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  rgb[0] = rgb[1] = rgb[2] = 0;
	  
	  if ((mask != NULL)?(mask->GetComponent(index, 0) > 0):1)
	    {
	      nrgb[0] = v[0] = in->GetComponent(index, 0);
	      nrgb[1] = v[1] = in->GetComponent(index, 1);
	      nrgb[2] = v[2] = in->GetComponent(index, 2);
	      
	      /* normalize vector */
	      vlenxy = vtkMath::Norm2D(v);

	      /* compute hue */
	      if (v[0] > 0) 
		phi = asin(v[1] / vlenxy);
	      else
		phi = pi - asin(v[1] / vlenxy);	      
	      hsv[0] = fmod((phi+2*pi+pi/2)*2,2*pi) / (2*pi);
	      if (hsv[0] > 1.0) hsv[0] = 1.0;
	      
	      /* compute saturation */
	      vlen = vtkMath::Norm(v);
	      theta = asin(vlenxy / vlen);
	      if (theta > 1) theta = 1.0;
	      hsv[1] = sin(ps*theta) / sin(ps*pi/2);

	      /* compute value */
	      hsv[2] = (mag != NULL) ? \
		((mag->GetComponent(index, 0) - mrange[0]) / mrange[1]): 1.0;	 
	      
	      vtkMath::HSVToRGB(hsv, rgb);
	     	        
	      /* RGBs have to be in [0,255] */
	      nrgb[0] = rgb[0]*255+scale*nrgb[0];
	      nrgb[1] = rgb[1]*255+scale*nrgb[1];
	      nrgb[2] = rgb[2]*255+scale*nrgb[2];
	      
	      out->SetComponent(index, 0, nrgb[0] < 256 ? nrgb[0] : 255);
	      out->SetComponent(index, 1, nrgb[1] < 256 ? nrgb[1] : 255);
	      out->SetComponent(index, 2, nrgb[2] < 256 ? nrgb[2] : 255);
	      
	    } else {
	      
	      /* RGBs have to be in [0,255] */
	      out->SetComponent(index, 0, MaskOutValue[0]);
	      out->SetComponent(index, 1, MaskOutValue[1]);
	      out->SetComponent(index, 2, MaskOutValue[2]);
	      
	    }
	  
	  ++index;
	  
	}
     
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
  }
  
  this->UpdateProgress(1.0);
}

void vtkmpjImageTensorColor::NoSymmetryScheme(vtkImageData *input, vtkImageData *output) {
  vtkDataArray* in =input->GetPointData()->GetScalars();
  
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* mask =NULL;
  vtkDataArray *mag = NULL;
  
  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
      
  double mrange[2];
  if (this->Magnitude != NULL) {
    mag = this->Magnitude->GetPointData()->GetScalars();
    mag->GetRange(mrange);
  }
  
  int index = 0;
  float v[3];
  float rgb[3], nrgb[3], hsv[3];
  float theta, phi;
  float vlenxy, vlen;
  float pi = vtkMath::Pi();
  float ps = this->SaturationScale;
  float scale = this->Scaling;
  
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  rgb[0] = rgb[1] = rgb[2] = 0;
	  
	  if ((mask != NULL)?(mask->GetComponent(index, 0) > 0):1)
	    {
	      nrgb[0] = v[0] = in->GetComponent(index, 0);
	      nrgb[1] = v[1] = in->GetComponent(index, 1);
	      nrgb[2] = v[2] = in->GetComponent(index, 2);
	      
	      /* normalize vector */
	      vlenxy = vtkMath::Norm2D(v);

	      /* compute hue */
	      if (v[0] > 0) 
		phi = asin(v[1] / vlenxy);
	      else
		phi = pi - asin(v[1] / vlenxy);	      
	      hsv[0] = fmod(phi+2*pi,2*pi) / (2*pi);
	      if (hsv[0] > 1.0) hsv[0] = 1.0;
	      
	      /* compute saturation */
	      vlen = vtkMath::Norm(v);
	      theta = asin(vlenxy / vlen);
	      if (theta > 1) theta = 1.0;
	      hsv[1] = sin(ps*theta) / sin(ps*pi/2);

	      /* compute value */
	      hsv[2] = (mag != NULL) ? \
		((mag->GetComponent(index, 0) - mrange[0]) / mrange[1]): 1.0;	 
	      
	      vtkMath::HSVToRGB(hsv, rgb);

	      /* RGBs have to be in [0,255] */
	      nrgb[0] = rgb[0]*255+scale*nrgb[0];
	      nrgb[1] = rgb[1]*255+scale*nrgb[1];
	      nrgb[2] = rgb[2]*255+scale*nrgb[2];
	      
	      out->SetComponent(index, 0, nrgb[0] < 256 ? nrgb[0] : 255);
	      out->SetComponent(index, 1, nrgb[1] < 256 ? nrgb[1] : 255);
	      out->SetComponent(index, 2, nrgb[2] < 256 ? nrgb[2] : 255);
	      
	    } else {
	  
	      /* RGBs have to be in [0,255] */
	      out->SetComponent(index, 0, MaskOutValue[0]);
	      out->SetComponent(index, 1, MaskOutValue[1]);
	      out->SetComponent(index, 2, MaskOutValue[2]);
	      
	    }
	  
	  ++index;
	  
	}
    
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
  }
  
  this->UpdateProgress(1.0);
}

void vtkmpjImageTensorColor::MirrorSymmetryScheme(vtkImageData *input, vtkImageData *output)
{
  vtkDataArray* in =input->GetPointData()->GetScalars();
  
  int dim[3];  input->GetDimensions(dim);
  
  vtkDataArray* out=output->GetPointData()->GetScalars();
  vtkDataArray* mask =NULL;
  vtkDataArray *mag = NULL;
  
  if (this->Mask != NULL)
    mask = this->Mask->GetPointData()->GetScalars();
  
  double mrange[2];
  if (this->Magnitude != NULL) {
    mag = this->Magnitude->GetPointData()->GetScalars();
    mag->GetRange(mrange);
  }

  int index = 0;
  float v[3];
  float rgb[3], nrgb[3], hsv[3];
  float theta, phi;
  float vlenxy, vlen;
  float pi = vtkMath::Pi();
  float ps = this->SaturationScale;
  float scale = this->Scaling;
  
  for (int k=0;k<dim[2];k++) {
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	{	
	  rgb[0] = rgb[1] = rgb[2] = 0;
	  
	  if ((mask != NULL)?(mask->GetComponent(index, 0) > 0):1)
	    {
	      nrgb[0] = v[0] = in->GetComponent(index, 0);
	      nrgb[1] = v[1] = in->GetComponent(index, 1);
	      nrgb[2] = v[2] = in->GetComponent(index, 2);
	      
	      /* normalize vector */
	      vlenxy = vtkMath::Norm2D(v);

	      /* compute hue */
	      if (v[0] > 0) 
		phi = asin(v[1] / vlenxy);
	      else
		phi = pi + asin(v[1] / vlenxy);	      
	      hsv[0] = 2 * fmod(phi + pi, pi) / (2*pi);
	      if (hsv[0] > 1.0) hsv[0] = 1.0;
	      
	      /* compute saturation */
	      vlen = vtkMath::Norm(v);
	      theta = asin(vlenxy / vlen);
	      if (theta > 1) theta = 1.0;
	      hsv[1] = sin(ps*theta) / sin(ps*pi/2);

	      /* compute value */
	      hsv[2] = (mag != NULL) ? \
		((mag->GetComponent(index, 0) - mrange[0]) / mrange[1]): 1.0;	 
	      
	      vtkMath::HSVToRGB(hsv, rgb);

	      /* RGBs have to be in [0,255] */
	      nrgb[0] = rgb[0]*255+scale*nrgb[0];
	      nrgb[1] = rgb[1]*255+scale*nrgb[1];
	      nrgb[2] = rgb[2]*255+scale*nrgb[2];
	      
	      out->SetComponent(index, 0, nrgb[0] < 256 ? nrgb[0] : 255);
	      out->SetComponent(index, 1, nrgb[1] < 256 ? nrgb[1] : 255);
	      out->SetComponent(index, 2, nrgb[2] < 256 ? nrgb[2] : 255);

	    } else {
	  
	      /* RGBs have to be in [0,255] */
	      out->SetComponent(index, 0, MaskOutValue[0]);
	      out->SetComponent(index, 1, MaskOutValue[1]);
	      out->SetComponent(index, 2, MaskOutValue[2]);
	      
	    }

	  ++index;
	  
	}
     
    float pog=float(k)/float(dim[2]);
    this->UpdateProgress(pog);	
  }
  
  this->UpdateProgress(1.0);
}

// ----------------------------------------------------------------------------
void vtkmpjImageTensorColor::SimpleExecute(vtkImageData *input, vtkImageData *output)
{
  if (this->GetInput()==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }
  
  switch(this->ColorSchemeType) 
    {
    case VTK_MPJ_DEC_SYMMETRY:
      this->RotationalSymmetryScheme(input, output);
      break;
    case VTK_MPJ_DEC_NOSYMMETRY:
      this->NoSymmetryScheme(input, output);
      break;
    case VTK_MPJ_DEC_MIRROR:
      this->MirrorSymmetryScheme(input, output);
      break;
    case VTK_MPJ_DEC_ABSOLUTE:
    default:
      this->AbsoluteValueScheme(input, output);
  }

}

// ----------------------------------------------------------------------------
