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
  Module:    $RCSfile: vtkpxComputeTmap.h,v $
  Language:  C++
  Date:      $Date: 2002/09/26 19:05:20 $
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
// .NAME vtkpxComputeTmap -- compute a tmap from two input images and two input variances
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>

#ifndef __vtkpxComputeTmap_h
#define __vtkpxComputeTmap_h

#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkpxSimpleImageToImageFilter.h"

class vtkpxComputeTmap : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkpxComputeTmap *New();
  vtkTypeMacro(vtkpxComputeTmap,vtkpxSimpleImageToImageFilter);

  // Description:
  // Set value of scalefactor to multiply t-values
  vtkGetMacro(ScaleFactor,float);
  vtkSetMacro(ScaleFactor,float);

  // Description:
  // Input2 -- second input to compare with input
  vtkSetObjectMacro(Input2,vtkImageData);
  vtkGetObjectMacro(Input2,vtkImageData);

  // Description:
  // Input StandardDeviation , i.e. variance of first input 
  vtkSetObjectMacro(InputStandardDeviation,vtkImageData);
  vtkGetObjectMacro(InputStandardDeviation,vtkImageData);

  // Description:
  // Input StandardDeviation2 , i.e. variance of second input 
  vtkSetObjectMacro(InputStandardDeviation2,vtkImageData);
  vtkGetObjectMacro(InputStandardDeviation2,vtkImageData);


  // Description:
  // Set ScannerIndex of Fitting 
  vtkSetClampMacro(NumberOfInputs,int,1,2);
  vtkGetMacro(NumberOfInputs,int);

  // Description:
  // Set Number Of Samples 
  vtkSetMacro(NumberOfSamples1, int);
  vtkGetMacro(NumberOfSamples1, int);

  vtkSetMacro(NumberOfSamples2, int);
  vtkGetMacro(NumberOfSamples2, int);

  // Description:
  // Get "Gaussianized" output
  vtkGetObjectMacro(OutputZScore,vtkImageData);

// Description:
  // Set Number Of Samples 
  vtkSetClampMacro(ComputeZScore, int,0,1);
  vtkGetMacro(ComputeZScore, int);


  // Description:
  // Convert ZScore to TMap
  static vtkImageData* TmapToZScoreConversion(vtkImageData* tmap,int ndof,double scale=1.0);
  static vtkImageData* TmapToPvalueConversion(vtkImageData* tmap,int dof,double scale=1.0);

protected:

  vtkpxComputeTmap();
  virtual ~vtkpxComputeTmap();
  vtkpxComputeTmap(const vtkpxComputeTmap&) {};
  void operator=(const vtkpxComputeTmap&) {};

  void ExecuteInformation();
  void SimpleExecute(vtkImageData* inp,vtkImageData* outp);

  float       ScaleFactor;
  int         NumberOfInputs;
  int         NumberOfSamples1;
  int         NumberOfSamples2;
  int         ComputeZScore;

  vtkImageData* Input2;
  vtkImageData* InputStandardDeviation;
  vtkImageData* InputStandardDeviation2;
  vtkImageData* OutputZScore;

  void ComputeTmapToZScoreConversion(vtkImageData* tmap);
};

#endif

