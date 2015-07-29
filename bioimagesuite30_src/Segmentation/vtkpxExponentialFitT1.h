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
  Module:    $RCSfile: vtkpxExponentialFitT1.h,v $
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
// .NAME vtkpxExponentialFitT1 -- compute phase/magnitude from raw reconstruction image on Bruker
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>

#ifndef __vtkpxExponentialFitT1_h
#define __vtkpxExponentialFitT1_h

#include "vtkSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkIntArray.h"
#include "vtkpxMatrix.h"
#include "vtkFloatArray.h"

class vtkpxExponentialFitT1 : public vtkSimpleImageToImageFilter
{
public:
  static vtkpxExponentialFitT1 *New();
  vtkTypeMacro(vtkpxExponentialFitT1,vtkSimpleImageToImageFilter);

  // Description: 
  // Create Relaxation Train
  virtual int CreateRelaxationTimes(float FirstRelaxationTime,float  RelaxationSpacing,int numechos=-1);

  // Description:
  // Get Outputs
  vtkGetObjectMacro(RelaxationTimes,vtkFloatArray);
  vtkSetObjectMacro(RelaxationTimes,vtkFloatArray);

  // Description: 
  // Thresholds 
  vtkGetMacro(MaxT1Value,float);
  vtkSetMacro(MaxT1Value,float);

  // Description: 
  // Thresholds 
  vtkSetClampMacro(IntensityThreshold,float,0.0,0.5);
  vtkGetMacro(IntensityThreshold,float);

  // Description;
  // density-map (i.e. magnitude of intensity as opposed to relaxation time)
  vtkGetObjectMacro(DensityMap, vtkImageData);

  // Description;
  // r1-map (i.e. 1000.0/T1)
  vtkGetObjectMacro(R1Map, vtkImageData);

  // Description;
  // Alpha-map (in case of three parameter fit)
  vtkGetObjectMacro(AlphaMap, vtkImageData);


  // Description:
  // Enable or Disable Fitting for alpha parameter
  vtkSetClampMacro(ThreeParameterFit,int,0,1);
  vtkGetMacro(ThreeParameterFit,int);
  vtkBooleanMacro(ThreeParameterFit,int);


  // Description:
  // These are needed for the Optimizer 
  static void  OptimizerFeedback(vtkObject* self,const char* line,double val);
  static double CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output);
  static double CalculateFunction(vtkObject* self,vtkDoubleArray* position);

protected:

  vtkpxExponentialFitT1();
  virtual ~vtkpxExponentialFitT1();
  vtkpxExponentialFitT1(const vtkpxExponentialFitT1&) {};
  void operator=(const vtkpxExponentialFitT1&) {};


  virtual void SimpleExecute(vtkImageData*,vtkImageData*);
  virtual int RequestInformation(vtkInformation *vtkNotUsed(request),  vtkInformationVector **inputVector, 
				 vtkInformationVector *outputVector);


  //BTX
  float IntensityThreshold;
  vtkImageData *R1Map,*DensityMap,*AlphaMap;
  float MaxT1Value;
  vtkFloatArray* RelaxationTimes;
  int ThreeParameterFit;
  double* xvalues;
  double* yvalues;
  double currentalpha,currentdensity,currentr1;
  int numgoodvalues;
  int DebugOpt;
  double alphascale;
  double r1scale;
  //ETX

  

  // Description:
  // Needed for Optimizer -- set,get,optimizer,optimizervalue
  virtual void SetCurrentParameters(vtkDoubleArray* par);
  virtual void GetCurrentParameters(vtkDoubleArray* par);
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);
  virtual void  SetOutputStatus(const char* line,double v);

  // Get Initial Estimates
  virtual void InitializeFit(vtkDoubleArray* position);
  virtual void InitializeFit2(vtkDoubleArray* position,double initialden);



};

#endif



