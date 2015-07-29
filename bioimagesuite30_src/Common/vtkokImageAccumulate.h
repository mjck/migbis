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

// .NAME vtkokImageAccumulate - Generalized histograms up to 4 dimensions.
//
// .SECTION Description
// vtkokImageAccumulate - This filter divides component space into
// discrete bins.  It then counts the number of pixels associated
// with each bin.  The output is this "scatter plot" (histogram values for 1D).
// The dimensionality of the output depends on how many components the 
// input pixels have.  Input pixels with one component generate a 1D histogram.
// This filter can only handle images with 1 to 3 scalar components.
// The input can be any type, but the output is always int.
// Some statistics are computed on the pixel values at the same time.
// The SetStencilFunction, SetClippingExtents and ReverseStencil
// functions allow the statistics to be computed on an arbitrary
// portion of the input data.
// See the documentation for vtkImageStencil for more information.
//
// .SECTION See also
//  vtkokImageAccumulate.tcl,Test_vtkokImageAccumulate.tcl
//
// .SECTION Author
//  Hirohito Okuda <hirohito.okuda@yale.edu>
//


#ifndef __vtkokImageAccumulate_h
#define __vtkokImageAccumulate_h


#include "vtkImageToImageFilter.h"

class vtkImageStencilData;

class  vtkokImageAccumulate : public vtkImageToImageFilter
{
public:
  static vtkokImageAccumulate *New();
  vtkTypeRevisionMacro(vtkokImageAccumulate,vtkImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set/Get - The component spacing is the dimension of each bin.
  // This ends up being the spacing of the output "image".
  // If the number of input scalar components are less than three,
  // then some of these spacing values are ignored.
  // For a 1D histogram with 10 bins spanning the values 1000 to 2000,
  // this spacing should be set to 100, 0, 0
  vtkSetVector3Macro(ComponentSpacing, double);
  vtkGetVector3Macro(ComponentSpacing, double);

  // Description:
  // Set/Get - The component origin is the location of bin (0, 0, 0).
  // Note that if the Component extent does not include the value (0,0,0),
  // then this origin bin will not actually be in the output.
  // The origin of the output ends up being the same as the componenet origin.
  // For a 1D histogram with 10 bins spanning the values 1000 to 2000,
  // this origin should be set to 1000, 0, 0
  vtkSetVector3Macro(ComponentOrigin, double);
  vtkGetVector3Macro(ComponentOrigin, double);

  // Description:
  // Set/Get - The component extent sets the number/extent of the bins.
  // For a 1D histogram with 10 bins spanning the values 1000 to 2000,
  // this extent should be set to 0, 9, 0, 0, 0, 0.
  // The extent specifies inclusive min/max values.  
  // This implies the the top extent should be set to the number of bins - 1.
  void SetComponentExtent(int extent[6]);
  void SetComponentExtent(int minX, int maxX, int minY, int maxY,
        int minZ, int maxZ);
  void GetComponentExtent(int extent[6]);
  int *GetComponentExtent() {return this->ComponentExtent;}


  // Description:
  // Use a stencil to specify which voxels to accumulate.
  void SetStencil(vtkImageStencilData *stencil);
  vtkImageStencilData *GetStencil();

  // Description:
  // Reverse the stencil.
  vtkSetMacro(ReverseStencil, int);
  vtkBooleanMacro(ReverseStencil, int);
  vtkGetMacro(ReverseStencil, int);

  // Description:
  // Get the statistics information for the data.
  vtkGetVector3Macro(Min, double);
  vtkGetVector3Macro(Max, double);
  vtkGetVector3Macro(Mean, double);
  vtkGetVector3Macro(StandardDeviation, double);
  vtkGetMacro(VoxelCount, long int);
 
  vtkGetVector3Macro(ThLowCollect,double);
  vtkSetVector3Macro(ThLowCollect,double);

  vtkGetVector3Macro(ThHighCollect,double);
  vtkSetVector3Macro(ThHighCollect,double);

  vtkGetMacro(ThK,double);
  vtkSetMacro(ThK,double);

  vtkGetMacro(OutlierExceptionMode,int);
  vtkSetMacro(OutlierExceptionMode,int);

  void SetOutlierExceptionModeToConstantSigma(){this->OutlierExceptionMode = 1;}

protected:
  vtkokImageAccumulate();
  ~vtkokImageAccumulate();

  double ComponentSpacing[3];
  double ComponentOrigin[3];
  int ComponentExtent[6];

  void ExecuteInformation(vtkImageData *input, vtkImageData *output);
  void ComputeInputUpdateExtent(int inExt[6], int outExt[6]);
  void ExecuteInformation(){this->vtkImageToImageFilter::ExecuteInformation();};
  void ExecuteData(vtkDataObject *out);

  double Min[3];
  double Max[3];
  double Mean[3];
  double StandardDeviation[3];
  long int VoxelCount;

  double ThLowCollect[3];
  double ThHighCollect[3];
  double ThK;
  int    OutlierExceptionMode;

  int ReverseStencil;

private:
  vtkokImageAccumulate(const vtkokImageAccumulate&);  // Not implemented.
  void operator=(const vtkokImageAccumulate&);  // Not implemented.
};

#endif




