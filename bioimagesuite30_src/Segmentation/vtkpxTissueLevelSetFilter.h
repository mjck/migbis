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
  Module:    $RCSfile: vtkpxTissueLevelSetFilter.h,v $
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
// .NAME vtkpxTissueLevelSetFilter -- corrects for B0 inhomogeneity in 1-H CSI Data for fat quantification

#ifndef __vtkpxTissueLevelSetFilter_h
#define __vtkpxTissueLevelSetFilter_h

#include "vtkpxBaseLevelSetFilter.h"
#include "vtkpxLevelSetUtil.h"

class vtkpxTissueLevelSetFilter : public vtkpxBaseLevelSetFilter
{
public:
  static vtkpxTissueLevelSetFilter *New();
  vtkTypeMacro(vtkpxTissueLevelSetFilter,vtkpxBaseLevelSetFilter);

  // Description:
  // Part 1 Tissue Description
  // Inside Mean and Sigma
  vtkSetMacro(InsideMean,float);
  vtkGetMacro(InsideMean,float);
  vtkSetMacro(InsideSigma,float);
  vtkGetMacro(InsideSigma,float);

  vtkSetMacro(OutsideMean,float);
  vtkGetMacro(OutsideMean,float);
  vtkSetMacro(OutsideSigma,float);
  vtkGetMacro(OutsideSigma,float);

  vtkSetMacro(Outside2Mean,float);
  vtkGetMacro(Outside2Mean,float);
  vtkSetMacro(Outside2Sigma,float);
  vtkGetMacro(Outside2Sigma,float);

  // Bi-exponential outside
  vtkSetClampMacro(BiExponentialOutside,int,0,1);
  vtkGetMacro(BiExponentialOutside,int);
  vtkBooleanMacro(BiExponentialOutside,int);
  
  
  // Dynamic Update
  vtkSetClampMacro(DynamicParameterUpdate,int,0,1);
  vtkGetMacro(DynamicParameterUpdate,int);
  vtkBooleanMacro(DynamicParameterUpdate,int);

  // Multiresolution
  vtkSetClampMacro(BeginLevel,int,1,4);
  vtkGetMacro(BeginLevel,int);

  vtkSetClampMacro(EndLevel,int,1,4);
  vtkGetMacro(EndLevel,int);

  virtual void SetNumberOfLevels(int n) {
    this->SetEndLevel(1);
    this->SetBeginLevel(n);
  }

  vtkGetMacro(NumberOfLevels,int);

// Multiresolution
  vtkSetMacro(VolumeWeight,float);
  vtkGetMacro(VolumeWeight,float);

  vtkSetMacro(ApproximateVolume,float);
  vtkGetMacro(ApproximateVolume,float);


protected:

  vtkpxTissueLevelSetFilter();
  virtual ~vtkpxTissueLevelSetFilter();
  vtkpxTissueLevelSetFilter(const vtkpxTissueLevelSetFilter&) {};
  void operator=(const vtkpxTissueLevelSetFilter&) {};


  // Description
  // Boilerplate code from filter
  virtual void SimpleExecute(vtkImageData* inp,vtkImageData* out);

  // Description:
  // Basic Methods
  virtual int  CheckInputs();
  virtual int  RunActiveContour(vtkImageData* input,vtkImageData* initial,vtkImageData* output);

  float         InsideMean;
  float         InsideSigma;
  float         OutsideMean;
  float         OutsideSigma;
  float         Outside2Mean;
  float         Outside2Sigma;
  float         VolumeWeight;
  float         ApproximateVolume;
  int           DynamicParameterUpdate;
  int           EndLevel;
  int           BeginLevel;
  int           NumberOfLevels;
  int           BiExponentialOutside;
};



#endif

