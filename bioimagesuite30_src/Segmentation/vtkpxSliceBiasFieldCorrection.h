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
  Module:    $RCSfile: vtkpxSliceBiasFieldCorrection.h,v $
  Language:  C++
  Date:      $Date: 2002/04/16 13:19:58 $
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
// .NAME vtkpxSliceBiasFieldCorrection - Filter for ffd registration based on voxel similarity measures.

// .SECTION Description
// This class implements a registration filter for the non-rigid registration
// of images.


#ifndef _VTKPXSliceBiasFieldCorrection_H
#define _VTKPXSliceBiasFieldCorrection_H

#include "vtkImageData.h"
#include "vtkDoubleArray.h"
#include "vtkpxAbstractBiasFieldCorrection.h"
#include "vtkImageData.h"
#include "vtkFloatArray.h"

class  vtkpxSliceBiasFieldCorrection : public vtkpxAbstractBiasFieldCorrection {

public:
  static vtkpxSliceBiasFieldCorrection* New();
  vtkTypeMacro(vtkpxSliceBiasFieldCorrection,vtkpxAbstractBiasFieldCorrection);

  // Description:
  // Print information about the progress of the registration
  void PrintInformation();

  // Description:
  // Synthetic Data
  vtkImageData* GenerateSynthetic(int width,double factor);

  // Description:
  // Get Internal Transformation Stuff
  vtkGetObjectMacro(SliceParameters,vtkDoubleArray);

  // Description:
  // RobustMode
  // 0 == correlation
  vtkSetClampMacro(RobustMode,int,0,1);
  vtkGetMacro(RobustMode,int);


  // Description:
  // Threshold
  // Values below this are ignored in image ratio computation (Image Range 0..1)
  vtkSetClampMacro(Threshold,float,0.0,0.25);
  vtkGetMacro(Threshold,float);

  // Description:
  // Axis 0=X, 1=Y, 2=Z
  vtkSetClampMacro(Axis,int,0,2);
  vtkGetMacro(Axis,int);

  // Description:
  // Pure Scaling i.e. no Offset Computed
  vtkSetClampMacro(PureScaling,int,0,1);
  vtkGetMacro(PureScaling,int);


  // Description:
  // ComputePolynomial Fit 
  virtual int PolynomialFitToParameters(int order,vtkFloatArray* params);

  // Description:
  // Image Ratio
  virtual int ComputeWeightedImageRatio(vtkImageData* img1,vtkImageData* img2,vtkImageData* weight1,vtkImageData* weight2,float threshold,
					float* x,float* y,float *temp,float& a,float& b);

protected:

  // Make all these protected 
  // Constructor
  vtkpxSliceBiasFieldCorrection();
  virtual ~vtkpxSliceBiasFieldCorrection();
  vtkpxSliceBiasFieldCorrection(const vtkpxSliceBiasFieldCorrection&) {};
  void operator=(const vtkpxSliceBiasFieldCorrection&) {};

  // Description:
  // Execute Stuff 
  virtual void SimpleExecute(vtkImageData* in,vtkImageData* out);

  // Description:
  // Create Weight Image
  virtual int CreateWeightImage(vtkImageData* ref);

  // Description:
  // More Variables
  vtkDoubleArray* SliceParameters;
  int             RobustMode;
  int             Axis;
  float           Threshold;
  int             PureScaling;
};


#endif

