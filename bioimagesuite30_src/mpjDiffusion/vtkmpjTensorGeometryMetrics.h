

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


/* -- vtkmpjTensorGeometryMetrics.h 
   
   Description: Computes geometrical measures of diffusion (Westin '97).

   Marcel Jackowski, February 2003

*/

#ifndef __vtkmpjTensorGeometryMetrics_h
#define __vtkmpjTensorGeometryMetrics_h

// Invariant options
#define VTK_MPJ_TENSOR_GEOM_ALL 0    
#define VTK_MPJ_TENSOR_GEOM_L   1
#define VTK_MPJ_TENSOR_GEOM_P   2
#define VTK_MPJ_TENSOR_GEOM_S   3

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjTensorGeometryMetrics : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjTensorGeometryMetrics *New();
  vtkTypeMacro(vtkmpjTensorGeometryMetrics,vtkpxSimpleImageToImageFilter);
  
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

  void SetOperationToLinear() {this->SetOperation(VTK_MPJ_TENSOR_GEOM_L);};
  void SetOperationToPlanar() {this->SetOperation(VTK_MPJ_TENSOR_GEOM_P);};
  void SetOperationToSpherical() {this->SetOperation(VTK_MPJ_TENSOR_GEOM_S);};
  void SetOperationToDefault() {this->SetOperation(VTK_MPJ_TENSOR_GEOM_ALL);};

  // Description:
  // Set/Get the value for results falling outside the mask
  vtkSetMacro(MaskOutValue,float);
  vtkGetMacro(MaskOutValue,float);
  
protected:
  int Operation;  /* Invariant to be computed */
  float MaskOutValue; 
  vtkImageData *Mask;
  
 vtkmpjTensorGeometryMetrics();
  virtual ~vtkmpjTensorGeometryMetrics();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
 vtkmpjTensorGeometryMetrics(const vtkmpjTensorGeometryMetrics&); // Not implemented
  void operator=(const vtkmpjTensorGeometryMetrics&); // Not Implemented
};

#endif
