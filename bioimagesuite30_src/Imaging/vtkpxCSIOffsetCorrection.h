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
  Module:    $RCSfile: vtkpxCSIOffsetCorrection.h,v $
  Language:  C++
  Date:      $Date: 2002/09/18 20:22:27 $
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
// .NAME vtkpxCSIOffsetCorrection -- corrects for B0 inhomogeneity in 1-H CSI Data for fat quantification

#ifndef __vtkpxCSIOffsetCorrection_h
#define __vtkpxCSIOffsetCorrection_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkFloatArray.h"

class vtkpxCSIOffsetCorrection : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkpxCSIOffsetCorrection *New();
  vtkTypeMacro(vtkpxCSIOffsetCorrection,vtkpxSimpleImageToImageFilter);

  // Description:
  //Set the level between 0 and 1 below which the data (in the ref image) is
  // not used to compute fat percentage stuff
  vtkGetMacro(Threshold,float);
  vtkSetClampMacro(Threshold,float,0.001,1.0);

  // Description:
  // Set the original location of the water_peak and the fat_peak in the integrated spectrum
  vtkGetMacro(WaterPeak,float);
  vtkSetMacro(WaterPeak,float);
  vtkGetMacro(FatPeak,float);
  vtkSetMacro(FatPeak,float);

  // Description:
  // Scale Factor, multiply output data by constant=0.00001 to keep numbers small
  vtkGetMacro(Scale,float);
  vtkSetMacro(Scale,float);

  // Description:
  // MRF Smoothness Factor
  vtkGetMacro(Lambda,float);
  vtkSetClampMacro(Lambda,float,0.0,200.0);

  // Description:
  // MRF Iterations
  vtkGetMacro(Iterations,int);
  vtkSetClampMacro(Iterations,int,0,30);
  
  

  // Description:
  // Spectral Smoothing (Sigma=1.0)
  vtkGetMacro(SpectralSigma,float);
  vtkSetClampMacro(SpectralSigma,float,0.0,2.0);

  // Description:
  // Input2 -- imaginary data
  // Input  -- real      data
  vtkSetObjectMacro(Input2,vtkImageData);
  vtkGetObjectMacro(Input2,vtkImageData);

  // Description:
  // FatPercentageOuput and integrated Spectrum
  vtkGetObjectMacro(FatPercentageOutput, vtkImageData);
  vtkGetObjectMacro(GlobalFatPercentageOutput, vtkImageData);
  vtkGetObjectMacro(Spectrum, vtkFloatArray);
  vtkGetObjectMacro(OffsetMap, vtkImageData);
  vtkGetObjectMacro(SmoothMagnitude, vtkImageData);
  vtkGetObjectMacro(GlobalOutput, vtkImageData);

protected:

  vtkpxCSIOffsetCorrection();
  virtual ~vtkpxCSIOffsetCorrection();
  vtkpxCSIOffsetCorrection(const vtkpxCSIOffsetCorrection&) {};
  void operator=(const vtkpxCSIOffsetCorrection&) {};


  // Description
  // Boilerplate code from filter
  virtual void ExecuteInformation();
  virtual void SimpleExecute(vtkImageData* inp,vtkImageData* out);

  // Description:
  // Generate Smooth Input Spectrum
  virtual vtkImageData* GenerateSmoothInputSpectrum(vtkImageData* real,vtkImageData* imag,float sigma,float scale);

  // Description
  // Generate PercentageFatOutput once correction is done, input is this->GetOutput()
  virtual vtkImageData* GenerateFatPercentageOutput(vtkImageData* input);
  
  // Description:
  // Compute Initial Object Map on a per voxel basis 
  virtual vtkImageData* ComputeInitialOffsetmap(vtkImageData* magnitude);
    
  
  // Description:
  // Optimization Process -- Greedy Search
  virtual int   UpdateVoxel(int eveni,int evenj,int evenk,int pass);
  virtual float OptimalOffsetForVoxel(int i,int j,int k,int dim[3],float weights[6],float lambda,float current,int nc);
  virtual int   OptimizeOffsetMap();

  // Description:
  // Integrate Spectrum Given Offset Map
  void IntegrateMagnitude(vtkImageData* realimg,vtkImageData* imagimg,vtkImageData* peaks,
			  vtkImageData* output,vtkFloatArray* volint);



  // Description
  // Data Members
  float       Threshold;
  float       WaterPeak;
  float       FatPeak;
  float       Scale;
  float       Lambda;
  int         Iterations;
  float       SpectralSigma;
  vtkImageData*  Input2;
  vtkImageData*  FatPercentageOutput;
  vtkImageData*  GlobalFatPercentageOutput;
  vtkImageData*  OffsetMap;
  vtkImageData*  SmoothMagnitude;
  vtkImageData*  GlobalOutput;
  vtkFloatArray* Spectrum;




};

#endif

