

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


#ifndef __vtkmpjToroidalDissociation_h
#define __vtkmpjToroidalDissociation_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjToroidalDissociation : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjToroidalDissociation *New();
  vtkTypeMacro(vtkmpjToroidalDissociation,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining the extents for invariants
  // computation. If a mask is not provided, invariants are
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);

  // Description:
  // Sets/Gets magnitude image to be multiplied by torus
  // classification results
  vtkSetObjectMacro(Magnitude, vtkImageData);
  vtkGetObjectMacro(Magnitude, vtkImageData);

  // Description:
  // Set/Get the value for results falling outside the mask
  vtkSetMacro(MaskOutValue,float);
  vtkGetMacro(MaskOutValue,float);
  
  // Description:
  // Set/Get the threshold value for dissociation
  vtkSetMacro(EigenvalueThreshold,float);
  vtkGetMacro(EigenvalueThreshold,float);

  // Description:
  // Set/Get eigenvalue classification values
  vtkSetMacro(CaseL1GreaterL2GreaterL3,float);
  vtkGetMacro(CaseL1GreaterL2GreaterL3,float);

  // Description:
  // Set/Get eigenvalue classification values
  vtkSetMacro(CaseL1GreaterL2EqualL3,float);
  vtkGetMacro(CaseL1GreaterL2EqualL3,float);
  
  // Description:
  // Set/Get eigenvalue classification values
  vtkSetMacro(CaseL1EqualL2GreaterL3,float);
  vtkGetMacro(CaseL1EqualL2GreaterL3,float);
  
  // Description:
  // Set/Get eigenvalue classification values
  vtkSetMacro(CaseL1EqualL2EqualL3,float);
  vtkGetMacro(CaseL1EqualL2EqualL3,float);

  // Description:
  // Set/Get eigenvalue classification values
  vtkSetMacro(CaseOther,float);
  vtkGetMacro(CaseOther,float);

protected:
  float MaskOutValue;
  float EigenvalueThreshold;
  vtkImageData *Mask;
  vtkImageData *Magnitude;
  float CaseL1GreaterL2GreaterL3;
  float CaseL1GreaterL2EqualL3;
  float CaseL1EqualL2GreaterL3;
  float CaseL1EqualL2EqualL3;
  float CaseOther;
  
  vtkmpjToroidalDissociation();
  virtual ~vtkmpjToroidalDissociation();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:

	 float ClassifyEigenvalues(double w[3], double thresh);

	 vtkmpjToroidalDissociation(const vtkmpjToroidalDissociation&); // Not implemented
     void operator=(const vtkmpjToroidalDissociation&); // Not Implemented
};

#endif
