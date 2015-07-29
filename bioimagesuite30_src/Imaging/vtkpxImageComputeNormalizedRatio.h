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
  Module:    $RCSfile: vtkpxImageComputeNormalizedRatio.h,v $
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
// .NAME vtkpxImageComputeNormalizedRatio -- compute the ratio and a normalized ratio between two images
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>

#ifndef __vtkpxImageComputeNormalizedRatio_h
#define __vtkpxImageComputeNormalizedRatio_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"

class vtkpxImageComputeNormalizedRatio : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkpxImageComputeNormalizedRatio *New();
  vtkTypeMacro(vtkpxImageComputeNormalizedRatio,vtkpxSimpleImageToImageFilter);

  // Description:
  //Set the level between 0 and 1 below which the data (in the ref image) is
  // not used to compute the scale and shift parameters
  vtkGetMacro(Threshold,float);
  vtkSetClampMacro(Threshold,float,0,1);

  // Description:
  // Set value of threshold between 0 and 1 below which the percentage
  // change is set to zero by default
  vtkGetMacro(NormalizedThreshold,float);
  vtkSetClampMacro(NormalizedThreshold,float,0,1);

  // Description:
  // Average Ratio
  vtkGetMacro(AverageRatio,float);

  // Description:
  // Smooth Difference before computing percentages
  vtkGetMacro(Sigma,float);
  vtkSetClampMacro(Sigma,float,0.0,3.0);


  // Description:
  // Input2 -- second input to compare with input
  vtkSetObjectMacro(Input2,vtkImageData);
  vtkGetObjectMacro(Input2,vtkImageData);

  // Description:
  // NormalizedOuput
  vtkGetObjectMacro(NormalizedOutput, vtkImageData);

protected:

  vtkpxImageComputeNormalizedRatio();
  virtual ~vtkpxImageComputeNormalizedRatio();
  vtkpxImageComputeNormalizedRatio(const vtkpxImageComputeNormalizedRatio&) {};
  void operator=(const vtkpxImageComputeNormalizedRatio&) {};

  virtual void SimpleExecute(vtkImageData* in,vtkImageData* out);

  float       Threshold;
  float       NormalizedThreshold;
  float       AverageRatio;
  float       Sigma;
  vtkImageData* Input2;
  vtkImageData* NormalizedOutput;

};

#endif

