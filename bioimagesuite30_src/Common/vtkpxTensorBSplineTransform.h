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
  Module:    $RCSfile: vtkpxTensorBSplineTransform.h,v $
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

 ========================================================================*/

#ifndef __vtkpxTensorBSplineTransform_h
#define __vtkpxTensorBSplineTransform_h

#include "vtkWarpTransform.h"
#include "pxutil.h"
#include "vtkDoubleArray.h"
#include "vtkFloatArray.h"
#include "vtkCharArray.h"
#include "vtkIntArray.h"
#include "vtkPolyData.h"
#include "vtkpxMatrix.h"

class vtkTransform;
class vtkPolyData;
class vtkImageData;
class vtkPoints;
class vtkCollection;

class vtkpxTensorBSplineTransform : public vtkWarpTransform
{
public:
  vtkTypeMacro(vtkpxTensorBSplineTransform,vtkWarpTransform);
  void PrintSelf(ostream& os, vtkIndent indent);
  static vtkpxTensorBSplineTransform *New();

  // Description:
  // Make another transform of the same type.
  vtkAbstractTransform *MakeTransform();

  // Description:
  // Copy Transformation
  virtual void Copy(vtkpxTensorBSplineTransform* other);

  // Description:
  // Initialize from Image
  virtual int Initialize(vtkImageData* img,double spacing,double offset);
  virtual int Initialize(vtkImageData* img,double spacingx,double spacingy,double spacingz,double offset,int adjustspacing=0);

  virtual int CachedInitialize(vtkImageData* img,double spacingx,double spacingy,double spacingz,double offset);

  

  // Description:
  // Load/Save Transformation
  virtual int Load(const char* fname);
  virtual int Save(const char* fname);
  virtual int Save(const char* fname,int compression);

  virtual int Load(gzFile fin,int skipheader=0,const char* header=NULL);
  virtual int Save(FILE* fout);
  virtual int Save(gzFile fout);

  // Description:
  // Return Result of B-sline basis functions 
  static double B (int, double);
  static double dB (int, double);

  // Description:
  // Access to Internal Structures for Information
  vtkGetVector3Macro(Dimensions,int);
  vtkGetVector3Macro(Spacing,double);
  vtkGetVector3Macro(Origin,double);
  vtkGetVector3Macro(OffsetDimensions,int);
  vtkGetMacro(NumberOfActiveComponents,int);
  vtkGetMacro(NumberOfControlPoints,int);

  // Description:
  // Cache Mode
  // 0 = None
  // 1 = Points
  // 2 = Image
  // 3 = Both
  vtkGetMacro(CacheMode,int);
  vtkSetClampMacro(CacheMode,int,0,3);
  
  // Description:
  // Get Displacements and Cache Stuff
  vtkGetObjectMacro(Displacements,vtkDoubleArray);
  vtkGetObjectMacro(ActiveControlPointList,vtkIntArray);
  vtkGetObjectMacro(ControlPointStatus,vtkCharArray);
  vtkGetObjectMacro(WholeImageCache,vtkImageData);
  vtkGetObjectMacro(DetailImageCache,vtkImageData);
  vtkGetObjectMacro(CachedPoints,vtkPolyData);
  vtkGetObjectMacro(CachedPointsInfo,vtkFloatArray);
  
  // Description:
  // Grid Indexing Stuff
  virtual int GetControlPointIndex(int i,int j,int k);

  // Description:
  // Get control point location stuff
  virtual void  GetControlPointLocation(int index, double p[3]);
  virtual double GetControlPointLocation(int index, int comp);

  // Description:
  // Get Control Point Location  (by looking up ActiveControlPointList to map index)
  virtual void   GetActiveControlPointLocation(int index, double p[3]);
  virtual double GetActiveControlPointLocation(int index, int comp);


  // Description:
  // Modify individual displacements
  virtual double GetControlPointDisplacement(int index,int comp);
  virtual void  GetControlPointDisplacement(int index,double val[3]);
  virtual void  SetControlPointDisplacement(int index,int comp,double val);
  virtual void  SetControlPointDisplacement(int index,double val[3]);

  // Description:
  // Shift Control Point
  virtual void  ShiftControlPoint(int index,int comp,double val);
  virtual void  ShiftControlPoint(int index,double val[3]);

  // Description:
  // Shift Active Control Point index (by looking up ActiveControlPointList)
  virtual void  ShiftActiveControlPoint(int index,int comp,double val);
  virtual void  ShiftActiveControlPoint(int index,double val[3]);


  // Description:
  // Get Offset Coordinates
  virtual void GetCoordinatesInGrid(double p[3],int lat[3],double s[3]);

  //BTX
  // Description:
  // Pointer to Control Point
  virtual void   GetControlPointIndices(int index,int ind[3]);
  //ETX

  // Description:
  // Hook to Optimization Routines -- only active control points used
  virtual void  SetCurrentParameters(vtkDoubleArray* par);
  virtual void  GetCurrentParameters(vtkDoubleArray* par);

  // Description:
  // Create/Use Point Cache
  virtual int CreatePointCache(vtkPolyData* pts);
  virtual int FormCollectionStructure(vtkPoints* SourcePoints,vtkCollection* ControlsCollection,double PointWeightThreshold);
  virtual int CachedPointTransform(int index,double output[3]);


  // Description:
  // Bending Energy Stuff
  virtual double ComputeTotalBendingEnergy();
  virtual double ComputeBendingEnergyAroundControlPoint(int cpoint);
  virtual double ComputeBendingEnergyAtControlPoint(int cpoint);

  // Description:
  // Active Control Point Stuff
  virtual int GetNumberOfActiveControlPoints();
  virtual int GetActualControlPointIndexFromActiveIndex(int i);
  virtual int IsControlPointActive(int cp);

  // Description:
  // Functions to facilitate point approximation issues
  virtual int GenerateTestPoints(vtkPoints* points,double incr);

  // Description:
  // Test Approximation: Specify points1=NULL to use cached points
  virtual double TestApproximation(vtkAbstractTransform* tr,vtkPoints* points);
  virtual double TestApproximation(vtkPoints* points,vtkPoints* points2);
  virtual double TestWeightedApproximation(vtkPoints* points,vtkPoints* points2,vtkpxMatrix* lsqweights,double& sum2error);


  // Description:
  // Compute Distance to other transformation
  virtual double ComputeWeightedDistance(vtkpxTensorBSplineTransform* other,int cp,vtkDoubleArray* weights=NULL);


  // Description:
  // More Optimization Stuff
  virtual void  ShiftControlPoints(vtkDoubleArray* grad,double step);
  static  void  ScaleGradients(vtkDoubleArray* grad,double scale);


protected:
  vtkpxTensorBSplineTransform();
  virtual ~vtkpxTensorBSplineTransform();

  vtkpxTensorBSplineTransform(const vtkpxTensorBSplineTransform&) {};
  void operator=(const vtkpxTensorBSplineTransform&) {};


  // Description:
  // Members an array of N components to specify control point positions and values
  vtkDoubleArray* Displacements;
  double          Origin[3];
  double          Spacing[3];
  int             Dimensions[3];
  int             OffsetDimensions[3];
  int             NumberOfControlPoints;
  vtkImageData*   WholeImageCache;
  vtkImageData*   DetailImageCache;
  vtkFloatArray*  CachedPointsInfo;
  vtkPolyData*    CachedPoints;
  vtkIntArray* ActiveControlPointList;
  vtkCharArray* ControlPointStatus;
  int             NumberOfActiveComponents;
  int             CacheMode;

  // Description:
  // Prepare the transformation for application.
  virtual void InternalUpdate();

  // Description:
  // This method does no type checking, use DeepCopy instead.
  virtual void InternalDeepCopy(vtkAbstractTransform *transform);

  // Description:
  // Forward Transform Point -- OK
  virtual void ForwardTransformPoint(const float in[3], float out[3]);
  virtual void ForwardTransformPoint(const double in[3], double out[3]);

  // Description:
  // Forward Transform Derivative (Not done yet) -- OK
  virtual void ForwardTransformDerivative(const float in[3], float out[3],
				  float derivative[3][3]);
  virtual void ForwardTransformDerivative(const double in[3], double out[3],
					  double derivative[3][3]);

  // Description:
  // Initialize Caches and Control Point Status issues
  virtual int InitializeStatusAndCaches();

  // Description:
  // Recompute ActiveControlPoint List from ControlPoint Status stuff
  virtual int CreateActiveControlPointListFromControlPointStatusArray();

};

#endif






