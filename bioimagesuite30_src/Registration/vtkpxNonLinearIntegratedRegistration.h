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
  Module:    $RCSfile: vtkpxNonLinearRegistration.h,v $
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
// .NAME vtkpxNonLinearRegistration - Filter for ffd registration based on voxel similarity measures.

// .SECTION Description
// This class implements a registration filter for the non-rigid registration
// of images.


#ifndef _VTKPXNONLINEARINTREGISTRATION_H
#define _VTKPXNONLINEARINTREGISTRATION_H

#include "vtkpxNonLinearRegistration.h"
#include "vtkImageData.h"
#include "vtkpxGridTransform.h"
#include "vtkpxComboTransform.h"
#include "vtkDoubleArray.h"
#include "vtkGeneralTransform.h"
#include "vtkPointLocator.h"
#include "vtkpxRPMCorrespondenceFinder.h"
#include "vtkPoints.h"
#include "vtkCollection.h"

class  vtkpxNonLinearIntegratedRegistration : public vtkpxNonLinearRegistration {

public:
  static vtkpxNonLinearIntegratedRegistration* New();
  vtkTypeMacro(vtkpxNonLinearIntegratedRegistration,vtkpxNonLinearRegistration);

  // Description:
  // Get Internal Transformation Stuff
  vtkGetObjectMacro(RPMEstimator,vtkpxRPMCorrespondenceFinder);

  
  // Description:
  // Relative Weight of Intensity to Points 
  vtkGetMacro(PointsWeight,float);
  vtkSetClampMacro(PointsWeight,float,0.0,1.0);

  // Run Optimization for Level level
  virtual int ComputeRegistration(int level);

  // Description:
  // Whole Image -- if set to 0 only register on part of the image covered by points 
  vtkSetMacro(WholeImage,int);
  vtkGetMacro(WholeImage,int);
  vtkBooleanMacro(WholeImage,int);

  // Description:
  // If WholeImage==0 use this to bound the region to be registered on
  vtkSetMacro(MarginSize,double);
  vtkGetMacro(MarginSize,double);

  // Description:
  // FixedCorrespondencesk -- if 1 run RPM only once at the startof the registration
  vtkSetMacro(FixedCorrespondences,int);
  vtkGetMacro(FixedCorrespondences,int);
  vtkBooleanMacro(FixedCorrespondences,int);

protected:

  // Make all these protected 
  // Constructor
  vtkpxNonLinearIntegratedRegistration();
  virtual ~vtkpxNonLinearIntegratedRegistration();
  vtkpxNonLinearIntegratedRegistration(const vtkpxNonLinearIntegratedRegistration&) {};
  void operator=(const vtkpxNonLinearIntegratedRegistration&) {};

  // Description:
  // RPM Registration
  vtkpxRPMCorrespondenceFinder* RPMEstimator;

  // Description:
  // PointsWeight
  double PointsWeight;

  // Description:
  // Last Fitting Error
  double LastFitError;
  double LastFitAbsError;

  // Description:
  // Whole Image Stuff
  int WholeImage;
  double MarginSize;

  // Description:
  // Correspondence Stuff
  int FixedCorrespondences;

  // Description:
  // Initial set up for the registration at a multiresolution level
  virtual int InitializeAll();
  virtual int Initialize(int);


  // Description:
  // Initial set up for the registration at a multiresolution level
  virtual int FinalizeAll();

  // Description:
  // Generate Output
  virtual void GenerateOutput(int level, int step,int iter,double stepsize,double simil);

  // Description:
  // Needed for Optimizer
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);

  // Description:
  // Evaluates the registration. This function evaluates the registration by
  // looping over the target image and interpolating the transformed source 
  // image while filling the joint histogram. This function returns the value
  // of the similarity measure using Similarity().
  virtual double Evaluate();

  // Description:
  // Needed for RPM Optimizer
  virtual double OnBeginIteration(vtkDoubleArray* params);

  // Description:
  // Compute Distance Sub Error
  virtual double ComputeApproximationErrorAtControlPoint(vtkFloatArray* indices,int debug=0);
  virtual double ComputeTotalApproximationError(double& lastabsfiterror,int debug=0);
					  
  // Description:
  // Called from Initialize to Initialize the Transformation
  virtual int InitializeTransformation(int);
  
};


#endif

