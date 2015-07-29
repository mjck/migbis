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
  Module:    $RCSfile: vtkpxLogExponentialFit.h,v $
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
// .NAME vtkpxLogExponentialFit -- compute phase/magnitude from raw reconstruction image on Bruker
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>

#ifndef __vtkpxLogExponentialFit_h
#define __vtkpxLogExponentialFit_h

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkIntArray.h"
#include "vtkpxMatrix.h"
#include "vtkFloatArray.h"

class vtkpxLogExponentialFit : public vtkSimpleImageToImageFilter
{
public:
  static vtkpxLogExponentialFit *New();
  vtkTypeMacro(vtkpxLogExponentialFit,vtkSimpleImageToImageFilter);

  // Description: 
  // Create Echo Train
  virtual int CreateEchoTrain(float FirstEchoTime,float  EchoSpacing,int numechos=-1);

  // Description:
  // Get Outputs
  vtkGetObjectMacro(EchoTrain,vtkFloatArray);
  vtkSetObjectMacro(EchoTrain,vtkFloatArray);

  // Description: 
  // Thresholds 
  vtkGetMacro(MaxT2Value,float);
  vtkSetMacro(MaxT2Value,float);

  // Description: 
  // Thresholds 
  vtkSetClampMacro(IntensityThreshold,float,0.0,0.5);
  vtkGetMacro(IntensityThreshold,float);

  // Description;
  // density-map (i.e. magnitude of intensity as opposed to relaxation time)
  vtkGetObjectMacro(DensityMap, vtkImageData);

  // Description;
  // r2-map (i.e. 1000.0/T2)
  vtkGetObjectMacro(R2Map, vtkImageData);




protected:

  vtkpxLogExponentialFit();
  virtual ~vtkpxLogExponentialFit();
  vtkpxLogExponentialFit(const vtkpxLogExponentialFit&) {};
  void operator=(const vtkpxLogExponentialFit&) {};


  virtual void SimpleExecute(vtkImageData*,vtkImageData*);
  virtual int RequestInformation(vtkInformation *vtkNotUsed(request),  vtkInformationVector **inputVector, 
				 vtkInformationVector *outputVector);


  //BTX
  float IntensityThreshold;
  vtkImageData *R2Map,*DensityMap;
  float MaxT2Value;
  vtkFloatArray* EchoTrain;
  //ETX

  




};

#endif



