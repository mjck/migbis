

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


/* -- vtkmpjImageTensorInvariants.h 
   
      Marcel Jackowski, November 2002

*/

#ifndef __vtkmpjImageTensorInvariants_h
#define __vtkmpjImageTensorInvariants_h

// Invariant options
#define VTK_MPJ_TENSORINV_ALL   0    
#define VTK_MPJ_TENSORINV_FA    1
#define VTK_MPJ_TENSORINV_RA    2
#define VTK_MPJ_TENSORINV_VR    3
#define VTK_MPJ_TENSORINV_MD    4
#define VTK_MPJ_TENSORINV_SK    5

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImageTensorInvariants : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageTensorInvariants *New();
  vtkTypeMacro(vtkmpjImageTensorInvariants,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining the extents for invariants
  // computation. If a mask is not provided, invariants are
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);

  // Description:
  // Set/Get the Operation to perform.
  vtkSetMacro(Operation,int);
  vtkGetMacro(Operation,int);

  void SetOperationToFractionalAnisotropy() {this->SetOperation(VTK_MPJ_TENSORINV_FA);};
  void SetOperationToRelativeAnisotropy() {this->SetOperation(VTK_MPJ_TENSORINV_RA);};
  void SetOperationToVolumeRatio() {this->SetOperation(VTK_MPJ_TENSORINV_VR);};
  void SetOperationToMeanDiffusivity() {this->SetOperation(VTK_MPJ_TENSORINV_MD);};
  void SetOperationToDefault() {this->SetOperation(VTK_MPJ_TENSORINV_ALL);};

  // Description:
  // Set/Get the value for results falling outside the mask
  vtkSetMacro(MaskOutValue,float);
  vtkGetMacro(MaskOutValue,float);
  
protected:
  int Operation;  /* Invariant to be computed */
  float MaskOutValue; 
  vtkImageData *Mask;
  
  vtkmpjImageTensorInvariants();
  virtual ~vtkmpjImageTensorInvariants();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  vtkmpjImageTensorInvariants(const vtkmpjImageTensorInvariants&); // Not implemented
  void operator=(const vtkmpjImageTensorInvariants&); // Not Implemented
};

#endif
