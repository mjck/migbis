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

// .NAME vtkokKochCoefficientSource - Compute displacement field caused
//  by target susceptibility based on Koch Koch's model.
//  Input  : SusceptibilityField 
//  Output : DisplacementField
// .SECTION Description
// Take susceptibility map as input, estimate induced field and displacement 
// field based on Koch Koch's model. You need to create susceptibility map
// in advance.
// Refer to (10) of "Rapid calculations of susceptibility-induced magnetic field
// perturbations for in vivo magnetic resonance", Koch Koch et al,Phys. Med.BioL 
// 51(2006)6831-6402


#ifndef __vtkokKochCoefficientSource_h
#define __vtkokKochCoefficientSource_h

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"

#define OLD_SOURCE 0

class vtkokKochCoefficientSource : public vtkSimpleImageToImageFilter
{
public:
  static vtkokKochCoefficientSource *New();
  vtkTypeRevisionMacro(vtkokKochCoefficientSource,vtkSimpleImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);   
  
  // Description:
  // Chi(kx,ky,kz),fourier transform of the susceptibility field
  vtkGetObjectMacro(ImageFFT,vtkImageData);

  // Description:
  // coefficient image Ck(kx,ky,kz) in the fourier domain
  vtkGetObjectMacro(ImageCoefficient,vtkImageData);

  // Description:
  // Chi(kx,ky,kz) * Ck(kx,ky,kz)
  vtkGetObjectMacro(ImageMultiplied,vtkImageData);

  // Description:
  //Inverse transform of Ck(kx,ky,kz) * Chi(kx,ky,kz)
  //which equals B_dz(x,y,z)*B0^(-1)
  vtkGetObjectMacro(ImageRFFT,vtkImageData);

  // Description:
  //induced field B_dz(x,y,z)
  vtkGetObjectMacro(InducedField,vtkImageData);
  
  // Description:
  // displacement field
  vtkGetObjectMacro(CoefficientMap,vtkImageData);

  // Description:
  // scaling factor to convert from induced field to displacement field
  vtkSetMacro(ScaleFactorOfDisplacement,float);

  // Description:
  //static magnetic field
  vtkSetMacro(B0,float);
  vtkGetMacro(B0,float);


  // Description:
  vtkSetMacro(OutputScalarType,int);
  vtkGetMacro(OutputScalarType,int);

  // Description:
  // If set to 1, average displacement in the BrainVOI will 
  // be caribrated as zero
  vtkSetMacro(MeanDisplacementToZero,int);
  vtkGetMacro(MeanDisplacementToZero,int);

  void SetMeanDisplacementToZeroOn()
    {this->SetMeanDisplacementToZero(1);}
  void SetMeanDisplacementToZeroOff()
    {this->SetMeanDisplacementToZero(0);}

  // Description:
  void SetOutputScalarTypeToFloat()
  {this->OutputScalarType=VTK_FLOAT;}
  void SetOutputScalarTypeToDouble()
  {this->OutputScalarType=VTK_DOUBLE;}
  

  // Description:
  // Padding Width --> add voxels of air around image to improve accuracy
  vtkSetClampMacro(Padding,int,0,99);
  vtkGetMacro(Padding,int);

protected:
  vtkokKochCoefficientSource();
  ~vtkokKochCoefficientSource();

  // Description:
  // suscptibility field
  vtkImageData*  SusceptibilityField; //[ppm = e-06]

  // Description:
  //Compute fourier transform Chi(kx,ky,kz) of susceptibility map 
  vtkImageData*  ImageFFT;

  // Description:
  //"Coeffcient Image" Ck(kx,ky,kz)
  vtkImageData*  ImageCoefficient;

  // Description:
  // Ck(kx,ky,kz) * Chi(kx,ky,kz)
  vtkImageData*  ImageMultiplied;

  // Description:
  // Inverse transform of Ck(kx,ky,kz) * Chi(kx,ky,kz)
  // which equals B_dz(x,y,z)*B0^(-1)
  vtkImageData*  ImageRFFT;

  // Description:
  // Induced field B_dz(x,y,z)
  vtkImageData*  InducedField;//[Hz]

  // Description:
  // Padded Input
  vtkImageData*  PaddedInput;

  // Description:
  // Coefficient Output
  vtkImageData*  CoefficientMap; 

  // Description:
  // BrainVOI 
  vtkImageData*  ExtractedVOI;

  // Description:
  //Optionally compute average displacement in this region.
  //Comptuted average displacement is subtracted from the
  //estimated displacemnt so that the average displacement
  //in the region should be zero.
  int    BrainVOI[6];

  // Description:
  int    OutputScalarType;

  // Description:
  // scaling factor to convert from induced field to displacement field
  float  ScaleFactorOfDisplacement;

  // Description:
  // Static magnetic field
  float  B0; //[T]

  // Description:
  // Amount of Air Image to Add around input for computational purposes
  int  Padding;//[voxels]

  // Description:
  // If set to 1, average displacement in soft tissuue (susc=-9.0) will 
  // be caribrated as zero
  int    MeanDisplacementToZero;

  // Description:
  // In case the argument "mode" is set to nonzero value, small
  // value will be assigned to voxels whose values are zero in the ImageMultiplied.
  int    mode;

  // Description:
  // Run The Filter
  virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);

  // Execute Information Replacements
  virtual int RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector, 
				 vtkInformationVector *outputVector);


  // Description:
  //  Compute induced displacement field from susceptibility map
  virtual vtkImageData* EstimateInductionField(vtkImageData* susceptibilitymap,vtkImageData* kochcoefficient);

  // Description:
  // Pad Image To Compute Field
  virtual vtkImageData* PadImage(vtkImageData* img,int padsize);


private:
  vtkokKochCoefficientSource(const vtkokKochCoefficientSource&);  // Not implemented.
  void operator=(const vtkokKochCoefficientSource&);  // Not implemented.
};


#endif



