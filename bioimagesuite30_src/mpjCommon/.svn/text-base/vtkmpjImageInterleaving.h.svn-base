

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



#ifndef __vtkmpjImageInterleaving_h
#define __vtkmpjImageInterleaving_h

// Input Interleaving options
#define VTK_MPJ_FRAMEINTERLEAVE 0
#define VTK_MPJ_SLICEINTERLEAVE 1

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImageInterleaving : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkmpjImageInterleaving *New();
  vtkTypeMacro(vtkmpjImageInterleaving,vtkpxSimpleImageToImageFilter);

  // Set number of frames in case of frame interleaving
  vtkSetMacro(NumberOfFrames, int);
  vtkGetMacro(NumberOfFrames, int);

  // Set input interleaving type:
  //
  // FRAME: XXXXYYYYZZZZ ... 
  // SLICE: XYZXYZXYZXYZ ..
  void SetFrameInterleave() {this->Interleaving = VTK_MPJ_FRAMEINTERLEAVE;}
  void SetSliceInterleave() {this->Interleaving = VTK_MPJ_SLICEINTERLEAVE;}

protected:

  vtkmpjImageInterleaving();
  virtual ~vtkmpjImageInterleaving();
  vtkmpjImageInterleaving(const vtkmpjImageInterleaving&) {};
  void operator=(const vtkmpjImageInterleaving&) {};

  int Interleaving;
  int NumberOfFrames;

  virtual void SimpleExecute(vtkImageData* in,vtkImageData* out);
  virtual void ExecuteInformation();


};

#endif
