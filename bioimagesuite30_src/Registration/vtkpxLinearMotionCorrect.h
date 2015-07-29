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
  Module:    $RCSfile: vtkpxSurfaceUtil.h,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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

// .NAME vtkpxLinearMotionCorrect - Filter for affine registration based on voxel similarity measures
// .SECTION Description
// This class implements a registration filter for the affine registration
// of images. The basic algorithm is described in Studholme, Medical Image
// Analysis, Vol. 1, No. 2, 1996.
//
// developed by the Computational Imaging Science Group, KCL <www-ipg.umds.ac.uk>
//

#ifndef _VTKPXLINEARMOTIONCORRECT_H
#define _VTKPXLINEARMOTIONCORRECT_H

#include "vtkpxLinearRegistration.h"
#include "vtkpxLinearTransform.h"


class vtkpxLinearMotionCorrect : public vtkpxLinearRegistration {

public:
  static vtkpxLinearMotionCorrect* New();
  vtkTypeMacro(vtkpxLinearMotionCorrect,vtkpxLinearRegistration);

  // Description:
  // Return Final Set of Registration Parameters
  vtkGetObjectMacro(RegistrationParameters,vtkDoubleArray);
  vtkGetObjectMacro(SimilarityResults,vtkDoubleArray);

  // Description:
  // Return Final Set of Registration Parameters
  vtkGetObjectMacro(InitialParameters,vtkDoubleArray);
  vtkSetObjectMacro(InitialParameters,vtkDoubleArray);

  // Description:
  // Sets referenceimage for the registration filter
  vtkSetObjectMacro(ReferenceMotionImage,vtkImageData);
  vtkGetObjectMacro(ReferenceMotionImage,vtkImageData);
  
  // Description:
  // Set Transform Image Sequence
  vtkSetObjectMacro(MotionSequence,vtkImageData);
  vtkGetObjectMacro(MotionSequence,vtkImageData);

  // Description:
  // Get Linear Registration 
  vtkGetObjectMacro(LinearRegistration,vtkpxLinearRegistration);
  

  // Description:
  // Begin Frame, End Frame, Reference Frame
  vtkSetMacro(BeginFrame,int);
  vtkGetMacro(BeginFrame,int);
  vtkGetMacro(EndFrame,int);
  vtkSetMacro(EndFrame,int);
  vtkGetMacro(ReferenceFrame,int);
  vtkSetMacro(ReferenceFrame,int);

  // Description:
  // UseLastFrameForInitialization
  vtkSetClampMacro(UseLastFrameForInitialization,int,0,1);
  vtkGetMacro(UseLastFrameForInitialization,int);
  vtkBooleanMacro(UseLastFrameForInitialization,int);



  // Description:
  // Warp Output Image
  static int MotionCorrectSequence(vtkImageData* Reference,
				   vtkImageData* Output,vtkImageData* Input,
				   vtkDoubleArray* Parameters,int interp_mode);

  static int MotionCorrectSequenceMask(vtkImageData* Reference,
				       vtkImageData* Output,vtkImageData* Input,
				       vtkDoubleArray* Parameters,int interp_mode);


  static int MotionCorrectSequence(vtkImageData* Reference,
				   vtkImageData* Output,vtkImageData* Input,
				   vtkDoubleArray* Parameters,int interp_mode,int center,short background=-100,short final=0);


  // Description:
  // Save/Load Motion Parameters (Scalars)
  static int SaveMotionParameters(vtkDoubleArray* scalars,char* fname);
  static int SaveMotionParameters(vtkDoubleArray* scalars,char* fname,int nparam);
  static int LoadMotionParameters(vtkDoubleArray* scalars,char* fname);
  static int InitializeMotionParameters(vtkDoubleArray* scalars,int numframes);

  // Description:
  // Run Motion Correction
  virtual void Run();

  
  // Description:
  // Compute All Similarity Metrics Between Images After Reslicing
  static void ComputeAllSimilarityMetricsBetweenImages(vtkImageData* reference,int frame,
						       vtkImageData* series,
						       vtkDoubleArray* results);



protected:
  // Make all these protected 
  // Constructor
  vtkpxLinearMotionCorrect();
  virtual ~vtkpxLinearMotionCorrect();
  vtkpxLinearMotionCorrect(const vtkpxLinearMotionCorrect&) {};
  void operator=(const vtkpxLinearMotionCorrect&) {};

  // Description:
  // Reference Image
  vtkImageData*                ReferenceMotionImage;
  vtkImageData*                MotionSequence;

  // Description:
  // Registration Parameters
  vtkDoubleArray*               RegistrationParameters;
  vtkDoubleArray*               SimilarityResults;

  // Description:
  // Initial Parameters
  vtkDoubleArray*               InitialParameters;

  // Description:
  // Use this to do actual registration
  vtkpxLinearRegistration*      LinearRegistration;

  // Descriptions:
  // Various Flags
  int BeginFrame;
  int EndFrame;
  int ReferenceFrame;
  int UseLastFrameForInitialization;
  int ComputeRegistrationExtra;


  // Description:
  // Check Inputs
  virtual int  CheckInputs();

  // Description:
  // Run Single Frame
  virtual void RunSingleFrame(vtkImageData* ref,vtkImageData* sequence,int frame,vtkpxLinearTransform* initial);
};


#endif

