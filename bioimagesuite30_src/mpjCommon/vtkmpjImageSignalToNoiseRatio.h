

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



#ifndef __vtkmpjImageSignalToNoiseRatio_h
#define __vtkmpjImageSignalToNoiseRatio_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

// Options
#define VTK_MPJ_SNR_MEAN      0     
#define VTK_MPJ_SNR_VARIANCE  1
#define VTK_MPJ_SNR_MEANVAR   2

class vtkmpjImageSignalToNoiseRatio : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageSignalToNoiseRatio *New();
  vtkTypeMacro(vtkmpjImageSignalToNoiseRatio,vtkpxSimpleImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description: 
  // Sets/Gets mask for computing statistics.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
  // Description:
  // Get SNR between mask and background.
  vtkGetMacro(SignalSum, float);
  vtkGetMacro(SignalCount, int);
  vtkGetMacro(SignalVariance, float);
  
  vtkGetMacro(NoiseSum, float);
  vtkGetMacro(NoiseCount, int);
  vtkGetMacro(NoiseVariance, float);
    
  // Description:
  // Set/Get the type of SNR ratio to be calculated
  vtkSetMacro(RatioType,int);
  vtkGetMacro(RatioType,int);

  // Description: set ratio type
  void SetRatioTypeToMeanRatio() {this->SetRatioType(VTK_MPJ_SNR_MEAN);};  // M(S)/M(N)
  void SetRatioTypeToVarianceRatio() {this->SetRatioType(VTK_MPJ_SNR_VARIANCE);}; // sqrt(V(S))/sqrt(V(N))
  void SetRatioTypeToMeanAndVarianceRatio() {this->SetRatioType(VTK_MPJ_SNR_MEANVAR);}; // M(S)/sqrt(V(N))
  void SetRatioTypeToDefault() {this->SetRatioType(VTK_MPJ_SNR_MEAN);};
  
  // Description:
  // Get resulting SNR
  vtkGetMacro(SNR, float);

 protected:
  vtkImageData *Mask;
  float SNR;
  float NoiseSum;
  float NoiseVariance;
  float SignalSum;
  float SignalVariance;
  int SignalCount;
  int NoiseCount;
  int RatioType;

  vtkmpjImageSignalToNoiseRatio();
  virtual ~vtkmpjImageSignalToNoiseRatio();
  
  virtual void SimpleExecute(vtkImageData* in, vtkImageData* out);
  //  virtual void ExecuteInformation();

 private:
  vtkmpjImageSignalToNoiseRatio(const vtkmpjImageSignalToNoiseRatio&); // Not implemented
  void operator=(const vtkmpjImageSignalToNoiseRatio&); // Not implemented
};

#endif


