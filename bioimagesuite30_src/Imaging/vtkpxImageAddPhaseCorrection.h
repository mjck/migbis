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
  Module:    $RCSfile: vtkpxImageAddPhaseCorrection.h,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:02:52 $
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
// .NAME vtkpxImageAddPhaseCorrection - Add Phase Offsets to Correct EPI ghosting 
// .SECTION Description
// For each pixel with vector components real imaginary this filter outputs 
// real in component0, and imaginary in component1, adding phaseoffset to the phase component

#ifndef __vtkpxImageAddPhaseCorrection_h
#define __vtkpxImageAddPhaseCorrection_h


#include "vtkImageToImageFilter.h"
#include "vtkFloatArray.h"


class vtkpxImageAddPhaseCorrection : public vtkImageToImageFilter
{
public:
  static vtkpxImageAddPhaseCorrection *New();
  vtkTypeMacro(vtkpxImageAddPhaseCorrection,vtkImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Phase Offset for odd lines 
  vtkSetMacro(PhaseOffset,float);
  vtkGetMacro(PhaseOffset,float);

  // Description:
  // Phase Offset for all lines 
  // i.e. odd lines = PhaseOffset+PhaseShift
  vtkSetMacro(PhaseShift,float);
  vtkGetMacro(PhaseShift,float);

  // Description:
  // Optimize Shift
  static int  OptimizeSimilarity(vtkImageData* image,vtkImageData* reference,
				 vtkFloatArray* optimal_shifts,vtkFloatArray* optimal_offsets);

  
  // Description:
  // Do Ghost Correction
  static int DoGhostCorrection(vtkImageData* image,float topline=0.25);
  static int DoGhostCorrectionPolarComplex(vtkImageData* image,float topline=0.25);

  // Description:
  // Correct Image
  static int CorrectEPIPhase(vtkImageData* image,vtkFloatArray* optimal_shifts,
			     vtkFloatArray* optimal_offsets);


  // Description:
  // Flag to Determine whether to use a single value for shift/offset or one value per slice 
  vtkSetClampMacro(SingleValue, int,0,1);
  vtkGetMacro(SingleValue, int);
  vtkBooleanMacro(SingleValue, int);

  // Description:
  // PhaseOffset Array
  vtkGetObjectMacro(PhaseOffsetArray,vtkFloatArray);
  vtkSetObjectMacro(PhaseOffsetArray,vtkFloatArray);

  // Description:
  // PhaseShift Array
  vtkGetObjectMacro(PhaseShiftArray,vtkFloatArray);
  vtkSetObjectMacro(PhaseShiftArray,vtkFloatArray);

  
protected:
  vtkpxImageAddPhaseCorrection();
  ~vtkpxImageAddPhaseCorrection() {};
  vtkpxImageAddPhaseCorrection(const vtkpxImageAddPhaseCorrection&) {};
  void operator=(const vtkpxImageAddPhaseCorrection&) {};

  float PhaseOffset;
  float PhaseShift;
  int   SingleValue;
  vtkFloatArray* PhaseOffsetArray;
  vtkFloatArray* PhaseShiftArray;

  void ThreadedExecute(vtkImageData *inData, vtkImageData *outData,int ext[6], int id);


  static float ComputeSimilarity(vtkImageData *img,
				 vtkImageData* ref,
				 float shift,float offset,
				 int mode,float topline=0.25,int dofft=1);

  // Description:
  // Compute Signal in outer-lobes vs signal inside using reference image 
  static float GetIntensitySum(vtkImageData* image,int slice,float topline=0.25);


  // Description:
  // Determine Location of anatomical image in slice to find good window 
  static float ComputeTopLine(vtkImageData* ref);
  static float ComputeCorrelationCoefficient(vtkDataArray* scal0,vtkDataArray* scal1,
					     int debug);

};

#endif




