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
  Module:    $RCSfile: vtkpxElectrodeGridToSurfaceFilter.h,v $
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
// .NAME vtkpxElectrodeGridToSurfaceFilter - stores and manipulates a set of electrodes arranged as a grid
// .SECTION Description
// This class stores all the necesary information for an implanted Electrode Grid such as
// width x height (in electrodes e.g. 8x8)
// electrode spacing x,y
// electrode type and dimensions
// grid name
// and finally an array of the actual electrodes
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION See Also
//  vtkPoints vtkPolyData vtkPolyDataSource vtkpxElectrodeSource



#ifndef __vtkpxElectrodeGridToSurfaceFilter_h
#define __vtkpxElectrodeGridToSurfaceFilter_h

class vtkPoints;
class vtkPolyData;
class vtkCollection;
class vtkDoubleArray;

#include "vtkpxElectrodeSource.h"
#include "vtkPolyDataSource.h"
#include "vtkpxElectrodeGridSource.h"
#include "vtkThinPlateSplineTransform.h"
#include "vtkImageData.h"
#include "vtkpxOptimizer.h"
#include "vtkDoubleArray.h"

class vtkpxElectrodeGridToSurfaceFilter : public vtkPolyDataSource 
{
public:
  static vtkpxElectrodeGridToSurfaceFilter *New();
  vtkTypeMacro(vtkpxElectrodeGridToSurfaceFilter,vtkPolyDataSource);

public:

  // Description:
  // Copy Electrode Attributs from another Electrode
  vtkGetObjectMacro(Grid,vtkpxElectrodeGridSource);
  virtual void SetElectrodeGrid(vtkpxElectrodeGridSource* grd);

  // Description:
  // Copy Electrode Attributs from another Electrode
  vtkGetObjectMacro(BaseImage,vtkImageData);
  virtual void SetImage(vtkImageData* img);

  // Description:
  // Get the Transformation
  vtkGetObjectMacro(Transformation,vtkThinPlateSplineTransform);

  // Description:
  // Sigma == smoothing kernel for blurring the input image
  vtkGetMacro(Sigma,double);
  vtkSetMacro(Sigma,double);

  // Descripion:
  // Lambda == smoothness of spline
  vtkGetMacro(Lambda,double);
  vtkSetMacro(Lambda,double);
  
  // Description:
  // Mode 0 = MRI, 1= CT
  vtkSetClampMacro(CTMode,int,0,1);
  vtkGetMacro(CTMode,int);
  vtkBooleanMacro(CTMode,int);

  // Description:
  // SampleRate
  vtkSetClampMacro(SampleRate,int,1,10);
  vtkGetMacro(SampleRate,int);

  // Description:
  // Optimize Grid
  virtual int Optimize(int iterations);

  // Description:
  // Optimization Routines
  static void  OptimizerFeedback(vtkObject* self,const char* line,double val);
  static double CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output);
  static double CalculateFunction(vtkObject* self,vtkDoubleArray* position);

  // Description:
  // Modify Image
  virtual int GenerateSmoothImage();
  virtual int GenerateBaseSurface();
  virtual int InitializeTransformation();
    
protected:
  vtkpxElectrodeGridToSurfaceFilter();
  virtual ~vtkpxElectrodeGridToSurfaceFilter();

  vtkpxElectrodeGridToSurfaceFilter(const vtkpxElectrodeGridToSurfaceFilter&) {};
  void operator=(const vtkpxElectrodeGridToSurfaceFilter&) {};


  // Description:
  // Update Output 
  virtual void Execute();  

  
  // Description:
  // Needed for Optimizer -- set,get,optimizer,optimizervalue
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);
  virtual void  SetCurrentParameters(vtkDoubleArray* params);
  virtual void  GetCurrentParameters(vtkDoubleArray* params);

  // Description:
  // Protected to avoid setting directly
  vtkSetObjectMacro(BaseImage,vtkImageData);
  vtkSetObjectMacro(Grid,vtkpxElectrodeGridSource);


  // Description:
  // Data Members
  //BTX
  vtkpxElectrodeGridSource*    Grid;
  vtkPolyData*                 BaseSurface;
  vtkPolyData*                 GridPointSurface;
  vtkImageData*                BaseImage;
  vtkImageData*                SmoothImage;
  vtkThinPlateSplineTransform* Transformation;

  double  Sigma;
  double  Lambda;
  int    CTMode;
  int    SampleRate;
  //ETX


};

#endif   


