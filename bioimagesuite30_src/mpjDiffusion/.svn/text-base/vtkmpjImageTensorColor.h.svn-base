

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


/* -- vtkmpjImageTensorColor.h 
   
*/

#ifndef __vtkmpjImageTensorColor_h
#define __vtkmpjImageTensorColor_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

// Color schemes
#define VTK_MPJ_DEC_ABSOLUTE    0
#define VTK_MPJ_DEC_SYMMETRY    1
#define VTK_MPJ_DEC_NOSYMMETRY  2
#define VTK_MPJ_DEC_MIRROR      3

class vtkmpjImageTensorColor : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageTensorColor *New();
  vtkTypeMacro(vtkmpjImageTensorColor,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining the extents for invariants
  // computation. If a mask is not provided, invariants are
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);
  
  // Description::
  // Sets/Gets magnitude image. Magnitude values have to be in [0,1]
  // Final color is modulated by this image.
  vtkSetObjectMacro(Magnitude, vtkImageData);
  vtkGetObjectMacro(Magnitude, vtkImageData);
  
  // Description:
  // Set/Get the color scheme
  vtkSetMacro(ColorSchemeType,int);
  vtkGetMacro(ColorSchemeType,int);
  
  void SetColorSchemeTypeToSymmetry() {this->SetColorSchemeType(VTK_MPJ_DEC_SYMMETRY);};
  void SetColorSchemeTypeToNoSymmetry() {this->SetColorSchemeType(VTK_MPJ_DEC_NOSYMMETRY);};
  void SetColorSchemeTypeToMirror() {this->SetColorSchemeType(VTK_MPJ_DEC_MIRROR);};
  void SetColorSchemeTypeToAbsolute() {this->SetColorSchemeType(VTK_MPJ_DEC_ABSOLUTE);};
  void SetColorSchemeTypeToDefault() {this->SetColorSchemeType(VTK_MPJ_DEC_ABSOLUTE);};
  
  // Description:
  // Set/Get the RGB value for results falling outside the mask
  vtkSetVectorMacro(MaskOutValue,float,3);
  vtkGetVectorMacro(MaskOutValue,float,3);
  
  // Description:
  // Set/Get saturation scale
  vtkSetMacro(SaturationScale,float);
  vtkGetMacro(SaturationScale,float);
  
  // Description:
  // Set/Get the scaling
  vtkSetMacro(Scaling,float);
  vtkGetMacro(Scaling,float);
  
 protected:
  int ColorSchemeType;
  float SaturationScale;
  float Scaling;
  float MaskOutValue[3]; 
  vtkImageData *Mask;
  vtkImageData *Magnitude;
  
  vtkmpjImageTensorColor();
  virtual ~vtkmpjImageTensorColor();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  void AbsoluteValueScheme(vtkImageData *input, vtkImageData *output);
  void RotationalSymmetryScheme(vtkImageData *input, vtkImageData *output);
  void NoSymmetryScheme(vtkImageData *input, vtkImageData *output);
  void MirrorSymmetryScheme(vtkImageData *input, vtkImageData *output);

  vtkmpjImageTensorColor(const vtkmpjImageTensorColor&); // Not implemented
  void operator=(const vtkmpjImageTensorColor&); // Not Implemented
};

#endif
