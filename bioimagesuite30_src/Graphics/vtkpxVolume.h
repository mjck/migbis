//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
//BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
//BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
//BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
//BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
//BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
//BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//BIOIMAGESUITE_LICENSE  See also  http://www.gnu.org/licenses/gpl.html
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
//BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
//BIOIMAGESUITE_LICENSE 
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------





/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxVolume.h,v $
  Language:  C++
  Date:      $Date: 2005/11/11 15:11:09 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
// .NAME vtkpxVolume - represents a volume (data & properties) in a rendered scene
//
// .SECTION Description
// vtkpxVolume is a derived class of vtkVolume which was used
//  to represent a volumetric entity in a rendering scene.
// It's additional features include (mandatory)
// internal mappers and switching of modes as well as
// The use of a lookup table
//
// .SECTION see also
// vtkVolume vtkVolumeMapper vtkVolumeProperty vtkProp3D

#ifndef __vtkpxVolume_h
#define __vtkpxVolume_h

class vtkPiecewiseFunction;
class vtkColorTransferFunction;
class vtkLookupTable;
class vtkImageShiftScale;
class vtkVolumeMapper;
class vtkImageReslice;
class vtkImageExtractComponents;
class vtkImageMapToColors;
class vtkImageBlend;
class vtkImageMapToRGBA; 

class vtkCudaVolumeMapper; 

#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkVolume.h"

class vtkpxVolume : public vtkVolume
{
public:
  vtkTypeMacro(vtkpxVolume,vtkVolume);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Creates a Volume with the following defaults: origin(0,0,0) 
  // position=(0,0,0) scale=1 visibility=1 pickable=1 dragable=1
  static vtkpxVolume *New();

  // Description:
  virtual void SetVolumeRenderingMode(int text);
  vtkGetMacro(VolumeRenderingMode,int);
  virtual void SetVolumeRenderingModeToRayCast() { this->SetVolumeRenderingMode(0);}
  virtual void SetVolumeRenderingModeToMIP() { this->SetVolumeRenderingMode(1);}
  virtual void SetVolumeRenderingModeToTextureMap() { this->SetVolumeRenderingMode(2);}
  virtual void SetVolumeRenderingModeToTextureMap3D() { this->SetVolumeRenderingMode(3);}
  virtual void SetVolumeRenderingModeToCroppedRayCast() { this->SetVolumeRenderingMode(4);}
  virtual void SetVolumeRenderingModeToCUDA() { this->SetVolumeRenderingMode(5);}


  // Description:
  // Set Cropping Mode 1=Yes, 0=No
  void SetIrregularCroppingMode(int inputCropMode);
  vtkGetMacro(cropMode,int);

  // Description:
  // Set Cropping Mode 1=Yes, 0=No
  void SetGradientEnhancementMode(int inputgradientEnhancementMode);
  vtkGetMacro(gradientEnhancementMode,int);

   // Description:
  // Set Cropping Mode 1=Yes, 0=No
  void SetCropValues(int shapeIndex, double SphereRadius, double CubeLengthX, double CubeLengthY, double CubeLengthZ, double EllipseRadiusX, double EllipseRadiusY,  double EllipseRadiusZ, double CylinderRadius, double CylinderHeight, double dirvec[3]);
  

  // Description:
  // Set Reslice Factor, to shrink image before rendering
  void SetResliceMode(int re);
  vtkGetMacro(ResliceMode,int);

  // Description:
  // Set Reslice Factor, to shrink image before rendering
  void SetDualInputMode(int m);
  vtkGetMacro(DualInputMode,int);

  
  // Description:
  // Set Reslice Factor, to shrink image before rendering
  virtual void SetRGBAMode(int m);
  vtkGetMacro(RGBAMode,int);

  // Description:
  // Reslice Mode Macros 
  void SetResliceModeToOriginal() { this->SetResliceMode(0); }
  void SetResliceModeToHalfOriginal() { this->SetResliceMode(1); }
  void SetResliceModeToQuarterOriginal() { this->SetResliceMode(2); }
  void SetResliceModeTo64_64() { this->SetResliceMode(3); }
  void SetResliceModeTo128_64() { this->SetResliceMode(4); }
  void SetResliceModeTo128_128() { this->SetResliceMode(5); }
  void SetResliceModeTo256_128() { this->SetResliceMode(6); }
  void SetResliceModeTo256_256() { this->SetResliceMode(7); }
  void SetResliceModeTo512_256() { this->SetResliceMode(8); }
  void SetResliceModeTo512_512() { this->SetResliceMode(9); }
  

  // Description:
  // Set Reslice Factor, to shrink image before rendering
  void SetInterpolationMode(int re);
  vtkGetMacro(InterpolationMode,int);

  // Description:
  // Set Smooth and Reslice Factors
  void SetResliceInterpolationModes(int re,int interp,int force=0);

  // Description:
  // Set Shading Mode 1=Yes, 0=No
  void SetShadingMode(int shad);
  vtkGetMacro(ShadingMode,int);

  // Description:
  // Max Intensity
  vtkGetMacro(MaxIntensity,int);

  // Description:
  // Set Transparency/Intensity from LookupTable
  virtual void SetFromLookupTable(vtkLookupTable* table);
  virtual void SetSecondLookupTable(vtkLookupTable* table);

  // Description:
  // Update 
  virtual void Update();
  virtual void UpdateRange();

  // Description:
  // Cropping Stuff
  virtual void               GetImageRange(int bounds[6]);
  virtual void               GetCroppingRange(int bounds[6]);
  virtual void               SetCroppingRange(int bounds[6]);
  virtual void               ResetCroppingRange();


  // irregular cropping functions 
  virtual void               GetProbeLocation(double probeLocation[3]);
  virtual void               SetProbeLocation(double probeLocation[3]);


  // Description:
  // Set Input Image
  virtual void SetInput(vtkImageData* image);
  virtual void SetInput(vtkImageData* image,int orientation);
  virtual void SetSecondInput(vtkImageData* img);


  vtkImageData* GetInputImage() { return this->InputImage;}
  vtkImageData* GetSecondInputImage() { return this->SecondInputImage;}

  // Description:
  // Set Frame Number
  virtual void SetFrame(int frame);

  // Description:
  // WARNING: INTERNAL METHOD - NOT INTENDED FOR GENERAL USE
  // DO NOT USE THIS METHOD OUTSIDE OF THE RENDERING PROCESS
  // Support the standard render methods.
  // Depending on the mapper type, the volume may be rendered using
  // this method (FRAMEBUFFER volume such as texture mapping will
  // be rendered this way)
  int RenderTranslucentGeometry(vtkViewport *viewport);

  // Description:
  // Get Min/Max Value for Colormap
  vtkGetMacro(MinValue,float);
  vtkGetMacro(MaxValue,float);

protected: 

  
  vtkpxVolume();
  virtual ~vtkpxVolume();

  // Made inaccesible 
  // Description:
  // Set The Mapper 
  virtual void SetMapper(vtkVolumeMapper *mapper);

  // Description:
  // Set/Get the volume property.
  virtual void SetProperty(vtkVolumeProperty *property);

  

  float MinValue;
  float MaxValue;
  int VolumeRenderingMode;
  int ShadingMode;
  int cropMode;
  int gradientEnhancementMode;
  double probeLocation[3]; 
  int cropping_range[6];
  int ResliceMode;
  int  InterpolationMode;
  int RGBAMode;	
  int RGBAMode_changed; 
  int DualInputMode;

  // Cropping parameters
  int CropShape;
  double CropSphereRadius;
  double CropCubeLengthX;
  double CropCubeLengthY;
  double CropCubeLengthZ;
  double CropEllipseRadiusX;
  double CropEllipseRadiusY;
  double CropEllipseRadiusZ;  
  double direction_vector[3];
  double CropCylinderRadius; 
  double CropCylinderHeight; 


  int                          MaxIntensity;
  int                          Frame;
  int                          Orientation;
  int                          NumberOfFrames;
  int                          TargetDimensions[3];
  double                        TargetOrigin[3];
  double                        TargetSpacing[3];
  double                        TargetSampleDistance;


  vtkImageExtractComponents*   ExtractFrame;
  vtkImageExtractComponents*   SecondExtractFrame;

  vtkImageShiftScale*          ShiftScale;
  vtkImageShiftScale*          SecondShiftScale;

  vtkImageReslice*             ImageReslice;
  vtkImageReslice*             SecondImageReslice;

  vtkImageBlend*               ImageBlend;
  vtkImageMapToRGBA*         ImageMapToRGBA1;
  vtkImageMapToRGBA*         ImageMapToRGBA2;
  vtkImageMapToColors*         ImageMapToColors;	

  vtkImageData*                InputImage;
  vtkImageData*                SecondInputImage;


  vtkColorTransferFunction     *cTFun;
  vtkPiecewiseFunction         *oTFun;
  vtkPiecewiseFunction         *gTFun;
  vtkPiecewiseFunction         *gTFun2;
  vtkPiecewiseFunction         *dualTransferFunction;
  vtkLookupTable               *LookupTable;
  vtkLookupTable               *SecondLookupTable;



  // Description:
  // Update Helper Functions
  virtual int Update_IsModeTheSame();
  virtual int Update_UpdateMapper();

};

#endif


