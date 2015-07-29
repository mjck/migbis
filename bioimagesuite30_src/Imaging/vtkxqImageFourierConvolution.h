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
  Module:    $RCSfile: vtkxqImageFourierConvolution.h,v $
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
// .NAME vtkxqImageFourierConvolution -- compute Convolution of Image with Filter Bank
// in Fourier Space

#ifndef __vtkxqImageFourierConvolution_h
#define __vtkxqImageFourierConvolution_h

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPoints.h"
#include "vtkFloatArray.h"

class vtkxqImageFourierConvolution : public vtkSimpleImageToImageFilter
{
public:
  static vtkxqImageFourierConvolution *New();
  vtkTypeMacro(vtkxqImageFourierConvolution,vtkSimpleImageToImageFilter);

  // Description:
  // DistanceMap -- distance from traced structure
  vtkSetObjectMacro(FilterBank,vtkImageData);
  vtkGetObjectMacro(FilterBank,vtkImageData);

  // Description:
  // Mode
  vtkGetMacro(Mode,int);
  vtkSetClampMacro(Mode,int,0,2);
  virtual void SetModeToConvolve()  { this->SetMode(0); }
  virtual void SetModeToDeviation() { this->SetMode(1); }
  virtual void SetModeToBoth()      { this->SetMode(2); }

  // Description:
  // Second Output is Mean Image in case when Mode = 2
  vtkGetObjectMacro(SecondOutput,vtkImageData);

  // Description:
  // Precision
  vtkSetClampMacro(DoublePrecision,int,0,1);
  vtkGetMacro(DoublePrecision,int);

  // Description:
  // Precision
  vtkSetClampMacro(ForceCPU,int,0,1);
  vtkGetMacro(ForceCPU,int);


  // Description:
  // Standard Filter Banks for gradients/hessian/vessel filters and sheet filters
  // ----------------------------------------------------------------------------
  virtual int SetFilterBankToGaussianDerivative(double sigma);
  virtual int SetFilterBankToGaussian(double sigma);
  virtual int SetFilterBankToHessian(double sigma);

  virtual int SetFilterBankToGaussianDerivative(double sigma1,double sigma2,double sigma3,double radius);
  virtual int SetFilterBankToGaussian(double sigma1,double sigma2,double sigma3,double radius);
  virtual int SetFilterBankToHessian(double sigma1,double sigma2,double sigma3,double radius);


  virtual int SetFilterBankToHessian(double sigma[3]);
  virtual int SetFilterBankToVessel(double anglespacing,int radius,double sigma_r=2.0,double sigma=2.0);
  virtual int SetFilterBankToSheet(double anglespacing,double anglespacing2,int radius,double sigma_r=2.0,double sigma=2.0);

  // Description:
  virtual int SetFilterBankToMultiScaleBlob(double scalefactor,double sigma);
  virtual int SetFilterBankToMultiScaleBlob(double scalefactor,vtkFloatArray* sigmas);

  // Description:
  // Static Utility Functions to Generate Some Standard Filters
  // ----------------------------------------------------------------------------
  static vtkImageData* Gaussian(double sigma,int dim);
  static vtkImageData* Gaussian(double sigma[3],int dim[3]);
  static vtkImageData* GaussianDerivative(double sigma,int dim);
  static vtkImageData* GaussianDerivative(double sigma[3],int dim[3]);

  static vtkImageData* Hessian(double sigma,int dim);
  static vtkImageData* Hessian(double sigma[3],int dim[3]);

  // Set sigmas to 0 to get rectmode
  static vtkImageData* VesselFilter(double anglespacing,int radius,double sigma_r=3.0,double sigma=2.0);
  static vtkImageData* SheetFilter(double anglespacing,double anglespacing2,int radius,double sigma=0.2,double sigma_r=1.0);

  // XQ's version:
  static vtkImageData* qVesselFilter(double anglespacing,int radius,double sigma_r=3.0,double sigma=2.0);
  static vtkImageData* qSheetFilter(double anglespacing,double anglespacing2,int radius,double sigma=0.2,double sigma_r=1.0);
  
  static vtkImageData* SumComponents(vtkImageData* img);
  static vtkPoints* GenerateSpherePoints(double& anglespacing,int halfspace=0,vtkFloatArray* angles=NULL);
  static vtkPoints* GenerateSpherePoints(int radius);
protected:

  vtkxqImageFourierConvolution();
  virtual ~vtkxqImageFourierConvolution();
  vtkxqImageFourierConvolution(const vtkxqImageFourierConvolution&) {};
  void operator=(const vtkxqImageFourierConvolution&) {};

  // Description:
  // Basic Input Outputs
  // ----------------------------------------------------------------------------
  virtual int RequestInformation(vtkInformation *vtkNotUsed(request),  
				 vtkInformationVector **inputVector, 
				 vtkInformationVector *outputVector);
  virtual void SimpleExecute(vtkImageData* inp,vtkImageData* out);

  // Description:
  // Data
  // ----------------------------------------------------------------------------
  vtkImageData* FilterBank;
  vtkImageData* SecondOutput;
  int           Mode;
  int           DoublePrecision;
  int           ForceCPU;

  // Description:
  // Normalize 'new' sphere filters.
  virtual void normalizeFilters(vtkImageData* FILTERS);
  // Description:
  // Basic Operations for Convolutions in Fourier Space
  // ----------------------------------------------------------------------------
  virtual vtkImageData* PadImage(vtkImageData* img,int paddim[3]);
  virtual vtkImageData* FFTImage(vtkImageData* img,int frame,int paddim[3],int dosquare);
  virtual vtkImageData* DoFourierConvolution(vtkImageData* img1,vtkImageData* img2,int origdim[3],double origori[3], int DoSqaure);
};

#endif

