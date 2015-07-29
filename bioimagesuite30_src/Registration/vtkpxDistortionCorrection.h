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
  Module:    $RCSfile: vtkpxDistortionCorrection.h,v $
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
// .NAME vtkpxDistortionCorrection - Filter for ffd registration based on voxel similarity measures.

// .SECTION Description
// This class implements a registration filter for the non-rigid registration
// of images.


#ifndef _VTKPXDISTORTIONCORRECTION_H
#define _VTKPXDISTORTIONCORRECTION_H

#include "vtkpxNonLinearRegistration.h"
#include "vtkImageData.h"
#include "vtkpxComboTransform.h"
#include "vtkDoubleArray.h"


class vtkpxDistortionCorrection : public vtkpxNonLinearRegistration {

public:
  static vtkpxDistortionCorrection* New();
  vtkTypeMacro(vtkpxDistortionCorrection,vtkpxNonLinearRegistration);

  // Description:
  // Use Jacobian Correction
  vtkSetClampMacro(UseJacobian,int,0,1);
  vtkGetMacro(UseJacobian,int);
  vtkBooleanMacro(UseJacobian,int);

  // Description:
  // Use Jacobian Correction
  vtkSetClampMacro(UseSignalLossCorrection,int,0,1);
  vtkGetMacro(UseSignalLossCorrection,int);
  vtkBooleanMacro(UseSignalLossCorrection,int);

  // Description:
  // Set Phase Encode Axis
  vtkSetClampMacro(PhaseEncodeAxis,int,0,2);
  vtkGetMacro(PhaseEncodeAxis,int);
  void SetPhaseEncodeAxisToX() { this->SetPhaseEncodeAxis(0); }
  void SetPhaseEncodeAxisToY() { this->SetPhaseEncodeAxis(1); }
  void SetPhaseEncodeAxisToZ() { this->SetPhaseEncodeAxis(2); }
  
  // Description:
  // Scale Intensities by Computing the Jacobian of the Transformation
  static  int ScaleIntensitiesUsingJacobian(vtkImageData* inout,vtkAbstractTransform* tr,vtkImageData* jac,int logmode=0);
  static  int ScaleIntensitiesUsingJacobianAndSignalLoss(vtkImageData* inout,vtkAbstractTransform* tr,
							 vtkImageData* jac,vtkImageData* signalloss,
							 int logmode=0);
  static int ComputeSimpleJacobianAndSignalLoss(vtkImageData* ref,vtkImageData* jacobian,vtkImageData* signalloss,
						vtkAbstractTransform* tr);

  // Description:
  // Compute Similarity between images allowing for distortion and signalloss (metric=0 nmi 1=jac nmi,2=jac & loss nmi)
  static double ComputeSimilarityBetweenImages(vtkImageData* img1,vtkImageData* img2,
					      vtkAbstractTransform* tr,
					      double resolution[3],double sigma,
					      int metric,int logarithmmode,
					      int interpolation,int numbins);
    
protected:

  // Make all these protected 
  // Constructor
  vtkpxDistortionCorrection();
  virtual ~vtkpxDistortionCorrection();
  vtkpxDistortionCorrection(const vtkpxDistortionCorrection&) {};
  void operator=(const vtkpxDistortionCorrection&) {};

  // Description:
  // Image Data to store last match (for removing bins from histogram)
  vtkImageData* LastImageJacobian;
  vtkImageData* LastSignalLossImage;
  
  // Description:
  // Flag to store use Jacobian
  int UseJacobian;
  int UseSignalLossCorrection;
  int PhaseEncodeAxis;

  // Description:
  // Initialize for level level
  virtual int Initialize(int level);


  // Description:
  // Evaluates the registration. This function evaluates the registration by
  // looping over the target image and interpolating the transformed source 
  // image while filling the joint histogram. This function returns the value
  // of the similarity measure using Similarity().
  virtual double Evaluate();

  // Description:
  // Evaluates the registration. This function evaluates the registration by
  // looping over the target image and interpolating the transformed source 
  // image while filling the joint histogram. This function returns the value
  // of the similarity measure using Similarity().
  virtual double EvaluatePiece(vtkImageData* reference,
			      vtkImageData* target,
			      vtkImageData* previoustarget,
			       vtkImageData* weightimage,
			      int cpoint=-1);


  // Make these protected in this case 
  // Description:
  // Enable control point displacements in X,Y and Z directions (defaults=1)
  vtkSetClampMacro(WarpX,int,0,1);
  vtkSetClampMacro(WarpY,int,0,1);
  vtkSetClampMacro(WarpZ,int,0,1);

  vtkGetMacro(WarpX,int);
  vtkGetMacro(WarpY,int);
  vtkGetMacro(WarpZ,int);

  vtkBooleanMacro(WarpX,int);
  vtkBooleanMacro(WarpY,int);
  vtkBooleanMacro(WarpZ,int);

  // Description:
  // Scale Image with Jacobian 
  virtual int ScaleIntensitiesUsingJacobian(vtkImageData* inout);

  virtual int Finalize(int level);

};


#endif

