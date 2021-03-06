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
  Module:    $RCSfile: vtkpxCoupledTissueLevelSetFilter.h,v $
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
// .NAME vtkpxCoupledTissueLevelSetFilter -- corrects for B0 inhomogeneity in 1-H CSI Data for fat quantification

#ifndef __vtkpxCoupledTissueLevelSetFilter_h
#define __vtkpxCoupledTissueLevelSetFilter_h

#include "vtkpxBaseLevelSetFilter.h"
#include "vtkpxLevelSetUtil.h"

class vtkpxCoupledTissueLevelSetFilter : public vtkpxBaseLevelSetFilter
{
public:
  static vtkpxCoupledTissueLevelSetFilter *New();
  vtkTypeMacro(vtkpxCoupledTissueLevelSetFilter,vtkpxBaseLevelSetFilter);


  // Description:
  // Part 1 Coupled Description
  // Inside Mean and Sigma
  vtkSetMacro(GrayMatterMean,float);
  vtkGetMacro(GrayMatterMean,float);
  vtkSetMacro(GrayMatterSigma,float);
  vtkGetMacro(GrayMatterSigma,float);

  vtkSetMacro(WhiteMatterMean,float);
  vtkGetMacro(WhiteMatterMean,float);
  vtkSetMacro(WhiteMatterSigma,float);
  vtkGetMacro(WhiteMatterSigma,float);

  vtkSetMacro(CSFMean,float);
  vtkGetMacro(CSFMean,float);
  vtkSetMacro(CSFSigma,float);
  vtkGetMacro(CSFSigma,float);

  // Mode (0 = Wm = 275, 1=285 2=300)
  virtual void SetPresetMode(int mode);

  // Part 1 -- Input Seeds or Levelset (if the later is not NULL use it first)
  vtkGetObjectMacro(Output2,vtkImageData);

  // LengthPenalty
  vtkSetClampMacro(MinThickness,float,0.0,10.0);
  vtkGetMacro(MinThickness,float);

  vtkSetClampMacro(MaxThickness,float,0.0,10.0);
  vtkGetMacro(MaxThickness,float);

  vtkSetClampMacro(Padding,int,1,20);
  vtkGetMacro(Padding,int);

  // Coupling Weight
  vtkSetClampMacro(CouplingOffset,float,0.0,100.0);
  vtkGetMacro(CouplingOffset,float);


  // Description:
  // Operations for creating and splitting padded images
  static vtkImageData* CreatePaddedImage(vtkImageData* input,int padding,double offset);
  static vtkImageData* CreateInnerOutputImage(vtkImageData* result,int dimx);
  static vtkImageData* CreateOuterOutputImage(vtkImageData* result,int padding,int dimx);

protected:


  vtkpxCoupledTissueLevelSetFilter();
  virtual ~vtkpxCoupledTissueLevelSetFilter();
  vtkpxCoupledTissueLevelSetFilter(const vtkpxCoupledTissueLevelSetFilter&) {};
  void operator=(const vtkpxCoupledTissueLevelSetFilter&) {};


  // Description
  // Boilerplate code from filter
  virtual void SimpleExecute(vtkImageData* inp,vtkImageData* out);

  // Description:
  // Basic Methods
  virtual int  CheckInputs();
  virtual int  RunCoupledActiveContour(vtkImageData* input,vtkImageData* output,vtkImageData* output2);


  float         GrayMatterMean;
  float         GrayMatterSigma;
  float         WhiteMatterMean;
  float         WhiteMatterSigma;
  float         CSFMean;
  float         CSFSigma;
  float         MaxThickness;
  float         MinThickness;
  float         CouplingOffset;
  int           Padding;

  //BTX
  vtkImageData* Output2;
  //ETX
};



#endif

