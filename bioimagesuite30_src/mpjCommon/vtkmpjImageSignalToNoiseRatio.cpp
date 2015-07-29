

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
#include "vtkmpjImageSignalToNoiseRatio.h"

//------------------------------------------------------------------------------
vtkmpjImageSignalToNoiseRatio* vtkmpjImageSignalToNoiseRatio::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkmpjImageSignalToNoiseRatio");
  if(ret)
    {
    return (vtkmpjImageSignalToNoiseRatio*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkmpjImageSignalToNoiseRatio;
}

// Construct object with no children.
vtkmpjImageSignalToNoiseRatio::vtkmpjImageSignalToNoiseRatio()
{
  this->SNR = 0.0;
  this->SignalCount = 0;
  this->SignalSum = 0.0;
  this->NoiseCount = 0;
  this->NoiseSum = 0.0;
  this->Mask = NULL;
  this->RatioType = VTK_MPJ_SNR_MEAN;
}

// ----------------------------------------------------------------------------
vtkmpjImageSignalToNoiseRatio::~vtkmpjImageSignalToNoiseRatio()
{ 
  if (this->Mask != NULL)
    this->Mask->Delete();  
}

// ----------------------------------------------------------------------------
void vtkmpjImageSignalToNoiseRatio::SimpleExecute(vtkImageData* input, vtkImageData* output)
{
  vtkDataArray *in=input->GetPointData()->GetScalars();
  vtkDataArray *out=output->GetPointData()->GetScalars();

  int numscalars=in->GetNumberOfTuples();
  
  vtkDataArray *mask=NULL;
  if (this->Mask != NULL) 
    mask = this->Mask->GetPointData()->GetScalars();
  
  // Progress computation
  int count = 0;
  float pog = 0.0;
  int tenth= (int)(numscalars / 10.0);
  
  int count_signal = 0;
  int count_noise = 0;  
  double sum_signal = 0.0;
  double sum_noise = 0.0;
  double sum_sqr_signal = 0.0;
  double sum_sqr_noise = 0.0;
  
  this->UpdateProgress(0.01);
  
  for (int n=0;n<numscalars;n++)      
    {      
      int inmask = (int)((mask != NULL)?(mask->GetComponent(n, 0) > 0):0);
      
      double value = (double)in->GetComponent(n, 0);

      if (inmask) {	
	sum_signal += value;
	sum_sqr_signal += value * value;
	count_signal++;	
	out->SetComponent(n, 0, value);
      } else {	
	sum_noise += value;
	sum_sqr_noise += value * value;
	count_noise++;
	out->SetComponent(n, 0, 0);
      }
      
      count++;
      if (count==tenth)
	{
	  pog+=0.1;
	  this->UpdateProgress(pog);
	  count=0;
	}
    }

  this->SignalSum = sum_signal;
  this->SignalCount = count_signal;
  this->NoiseSum = sum_noise;
  this->NoiseCount = count_noise;

  double mean_signal = sum_signal / (double)count_signal;
  double mean_noise = sum_noise / (double)count_noise;

  double signal_var = (sum_sqr_signal - (((double)count_signal)*mean_signal*mean_signal))/((double)count_signal-1);
  double noise_var = (sum_sqr_noise - (((double)count_noise)*mean_noise*mean_noise))/((double)count_noise-1);
  
  switch(this->RatioType) {
  case VTK_MPJ_SNR_MEANVAR:
    if (count_noise > 0 && count_signal > 0) {
      this->SNR = mean_signal / sqrt(noise_var);
    }
    break;
  case VTK_MPJ_SNR_VARIANCE:
    if (count_noise > 0 && count_signal > 0) {
      this->SNR = sqrt(signal_var) / sqrt(noise_var);
    }
    break;
  case VTK_MPJ_SNR_MEAN:
  default:
    if (mean_noise != 0) {
      this->SNR = mean_signal / mean_noise;
    };
  };
  
  this->SignalVariance = signal_var;
  this->NoiseVariance = noise_var;

  this->UpdateProgress(1.0);
}

void vtkmpjImageSignalToNoiseRatio::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkpxSimpleImageToImageFilter::PrintSelf(os,indent);

  os << indent << "SNR: " << this->SNR << "\n";
  
}
