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
  Module:    $RCSfile: vtkpxAverageImages.h,v $
  Language:  C++
  Date:      $Date: 2002/09/26 18:08:12 $
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
// .NAME vtkpxAverageImages -- combines images to form either mean or median
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>

#ifndef __vtkpxAverageImages_h
#define __vtkpxAverageImages_h

#include "vtkpxSimpleImageToImageFilter.h"

class vtkpxAverageImages : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkpxAverageImages *New();
  vtkTypeMacro(vtkpxAverageImages,vtkpxSimpleImageToImageFilter);

  // Description:
  // Median, 0=Mean 1=Median 
  vtkGetMacro(Median,int);
  vtkSetClampMacro(Median,int,0,1);
  vtkBooleanMacro(Median,int);

  // Description:
  // SumOnly if (median==0) do sum not mean
  vtkGetMacro(SumOnly,int);
  vtkSetClampMacro(SumOnly,int,0,1);
  vtkBooleanMacro(SumOnly,int);

  // Description: 
  // Ignore Flag -- if set to 1 if any one image has a pixel value = Ignore Value
  // The output there will be set to 0.0
  vtkSetClampMacro(IgnoreFlag, int,0,1);
  vtkGetMacro(IgnoreFlag, int);
  vtkBooleanMacro(IgnoreFlag, int);

  // Description:
  // Ignore Value -- used for test
  vtkSetMacro(IgnoreValue, float);
  vtkGetMacro(IgnoreValue, float);
  
  // Description:
  // Ignore Value -- used for test
  vtkSetMacro(IgnoreValueOutput, float);
  vtkGetMacro(IgnoreValueOutput, float);

  // Description:
  // Flag to Compute StandardDeviation -- to be obtained from StandardDeviationOutput
  vtkSetClampMacro(ComputeStandardDeviation, int,0,1);
  vtkGetMacro(ComputeStandardDeviation, int);
  vtkBooleanMacro(ComputeStandardDeviation, int);


  // Description: 
  // Get the Output Standard Deviation
  vtkGetObjectMacro(OutputStandardDeviation, vtkImageData);

protected:

  //BTX
  vtkpxAverageImages();
  virtual ~vtkpxAverageImages();
  vtkpxAverageImages(const vtkpxAverageImages&) {};
  void operator=(const vtkpxAverageImages&) {};

  virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);
  virtual void ExecuteInformation();

  int         Median;
  int         SumOnly;
  float       IgnoreValue;
  float       IgnoreValueOutput;
  int         IgnoreFlag;
  int         ComputeStandardDeviation;
  vtkImageData* OutputStandardDeviation;


  //ETX
};

#endif



