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
  Module:    $RCSfile: vtkpxImageRobustHistogram.h,v $
  Language:  C++
  Date:      $Date: 2003/05/13 15:37:02 $
  Version:   $Revision: 1.2 $
  
  
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
// .NAME vtkpxImageRobustHistogram - extracts slices from 3D Images
// .SECTION Description
// Extracts single 2D slice from 3D Images determined by slice number, CurrentPlane and flip 
// CurrentPlane = 0=YZ, 1=XZ, 2=XY
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION See Also
//  vtkExtractVOI

#ifndef __vtkpxImageRobustHistogram_h
#define __vtkpxImageRobustHistogram_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkDataArray.h"
#include "vtkDoubleArray.h"

class vtkpxImageRobustHistogram : public vtkpxSimpleImageToImageFilter
{
public:
  vtkTypeMacro(vtkpxImageRobustHistogram,vtkpxSimpleImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  // Description:
  // Construct object to extract all of the input data.
  static vtkpxImageRobustHistogram *New();
  
  // Description:
  // Polar Mode used for ultrasound images
  vtkSetClampMacro(NumberOfBins,int,2,1024);
  vtkGetMacro(NumberOfBins,int);


  // Description:
  // Get/Set the Mask Image
  vtkSetObjectMacro(WeightImage,vtkImageData);
  vtkGetObjectMacro(WeightImage,vtkImageData);

  // Description:
  // Flag Igored Mask Voxels
  vtkSetClampMacro(IgnoreMaskedVoxels,int,0,1);
  vtkGetMacro(IgnoreMaskedVoxels,int);
  vtkBooleanMacro(IgnoreMaskedVoxels,int);


  // Description:
  // Use this to set sliceno,numslices and currentplane
  // for extracting frames
  vtkSetClampMacro(Interpolation,int,0,1);
  vtkGetMacro(Interpolation,int);
  vtkBooleanMacro(Interpolation,int);

  // Description:
  // Use this to set sliceno,numslices and currentplane
  // for extracting frames
  vtkSetMacro(Frame,int);
  vtkGetMacro(Frame,int);


  // Description:
  // Robust Mode Automatically Determines Bounds from Lower and UpperBound
  vtkSetClampMacro(RobustMode,int,0,1);
  vtkGetMacro(RobustMode,int);
  vtkBooleanMacro(RobustMode,int);

  // Description:
  // Saturation Mode (if v<Saturate Ignore if saturation is off)
  vtkSetClampMacro(SaturationMode,int,0,1);
  vtkGetMacro(SaturationMode,int);
  vtkBooleanMacro(SaturationMode,int);

  // Description:
  // Plain Histogram i.e. no tricks
  virtual void SetPlainHistogram();

  // Description:
  // Upper and Lower Bounds of Cumulative Histogram 
  vtkSetClampMacro(LowerBound,double,0.0,0.5);
  vtkGetMacro(LowerBound,double);
  vtkSetClampMacro(UpperBound,double,0.5,1.0);
  vtkGetMacro(UpperBound,double);
  
  
  // Description:
  // Key Outputs
  vtkGetMacro(LowerThreshold,double);
  vtkGetMacro(UpperThreshold,double);
  vtkSetMacro(LowerThreshold,double);
  vtkSetMacro(UpperThreshold,double);

  vtkGetMacro(Entropy,double);




  // Description:
  // Modify Current Histogram
  virtual void  BackupHistogram();
  virtual int AddImage(vtkImageData* newimage,vtkImageData* weight=NULL);
  virtual int RemoveImage(vtkImageData* newimage,vtkImageData* weight=NULL);
  virtual int UndoLastChange();
  virtual double ComputeEntropy();

  // Description
  // Save Gnuplot
  virtual int SaveGnuplot(char* fname);

  // Description:
  // Get Centroids
  static int GetObjectmapStatistics(vtkImageData* img,vtkDoubleArray* array);

protected:
  vtkpxImageRobustHistogram();
  ~vtkpxImageRobustHistogram();
  vtkpxImageRobustHistogram(const vtkpxImageRobustHistogram&) {};
  void operator=(const vtkpxImageRobustHistogram&) {};

  virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);
  virtual void  ExecuteInformation();

  virtual void  UpdateThresholds();
  virtual void  AddDataArray(vtkDataArray* data,float factor=1.0,int Frame=0,vtkDataArray* weights=NULL);

  virtual vtkDataArray* GetWeightArray(vtkImageData* image,vtkImageData* weights);

  int NumberOfBins;
  int Interpolation;
  int RobustMode;
  int SaturationMode;
  double UpperBound;
  double LowerBound;
  double LowerThreshold;
  double UpperThreshold;
  double Entropy;
  int  Initialized;
  int  IgnoreMaskedVoxels;
  int  Frame;

  vtkImageData* LastOutput;
  vtkImageData* WeightImage;
};

#endif



