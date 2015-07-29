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
  Module:    $RCSfile: vtkpxSurfaceUtil.h,v $
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

// .NAME vtkpxSplineSource - BSpline parameterized curve 
// .SECTION Description
// vtkSplineSource is a source object that creates loads and saves b-spline curves
// .SECTION Caveats
// Binary files written on one system may not be readable on other systems.

#ifndef __vtkpxSplineSource_h
#define __vtkpxSplineSource_h

#include "vtkPolyDataSource.h"
#include "vtkpxOptimizer.h"
#include "pxutil.h"

class vtkImageData;
class vtkTransform;
class vtkPoints;
class vtkpxBaseCurve;

//BTX
class PXFPBSpline;
class PXContour;
//ETX

class vtkpxSplineSource : public vtkPolyDataSource
{

public:
  static vtkpxSplineSource *New();
  vtkTypeMacro(vtkpxSplineSource,vtkPolyDataSource);

  // Description:
  // Initialization -- Set as Ellipse and Copy Other 
  virtual void SetEllipse(int np,float radiusx=10.0,float radiusy=5.0,
			  float x=50.0,float y=50.0,float z=0.0,int dim=2);
  virtual void Copy(vtkpxSplineSource* other);
  
  // Description:
  // Input/Output to standard .spl format 
  virtual int  Load(const char* fname);
  virtual int  Save(const char* fname);
  
  // Description:
  // Display Parameter 
  vtkGetMacro(Resolution,float);
  vtkSetMacro(Resolution,float);

  // Description:
  // Information regarding spline 
  virtual int   GetNumPoints();
  virtual int   HasSnake();

  // Description:
  // Zlevel is constant z-coordinate or slice level 
  virtual float GetZlevel();
  virtual void  SetZlevel(float lev);

  // Description:
  // Interface to vtkPoints 
  virtual int ExportToPoints(vtkPoints* points);
  virtual int ExportKnotPointsToPoints(vtkPoints* points);

  virtual int FitPoints(vtkPoints* points,float smooth,int numpoints,
			int closed,int iterations);
  virtual int FitPoints(vtkPoints* points,float smooth=2.0,int numpoints=-1)
      { return FitPoints(points,smooth,numpoints,1,100); }

  virtual int SetControlPoints(vtkPoints* points,int np);
  virtual int SetControlPoints(vtkPoints* points)
      { return SetControlPoints(points,-1);}


  // Description:
  // Interface to vtkpxBaseCurve
  virtual int ExportToBaseCurve(vtkpxBaseCurve* baseCurve);
  virtual int SetFromBaseCurve(vtkpxBaseCurve* baseCurve);

  // Description:
  // Resample and Smooth Spline 
  virtual int Resample(float ds_step,int npoints);
  virtual int Resample(int npoints) { return Resample(0.01,npoints);}
  virtual int Resample(float ds) { return Resample(ds,-1);}
  virtual int ChangeSpacing(float smooth=0.5,int npoints=-1,
			    float step=-1.0,float csmooth=0.0,int iterations=50);

  
  // Description:
  // Scale factors to scale from voxels (used for snake stuff) to mm 
  vtkGetVectorMacro(ImageSpacing,double,3);
  vtkSetVectorMacro(ImageSpacing,double,3);

  // Description:
  // Shift factor to translate from voxels (used for snake stuff) to mm 
  vtkGetVectorMacro(ImageOrigin,double,3);
  vtkSetVectorMacro(ImageOrigin,double,3);


  // Description:
  // Apply Transformation to BSpline
  virtual int Transform(vtkTransform* Transform);

  // Description:
  // Snake Stuff -- crude segmentation algorithm
  virtual void CreateSnake(vtkImageData* image,int level,int frame,
			   float img_coeff,float smooth_coeff,
			   float timestep,float sigma,int niter);
  virtual void CreateSnake(vtkImageData* image,int level,int frame,float img_coeff=5.0,float smooth_coeff=0.5)
  {	CreateSnake(image,level,frame,img_coeff,smooth_coeff,0.05,1.0,10);}

  virtual float UpdateSnake(float img_coeff,float smooth_coeff);
  virtual float UpdateSnake() { return UpdateSnake(-1.0,-1.0);}


  // Description: 
  // VOI Stuff area, mean intensity area>0 std of mean intensity 
  virtual float GetArea();
  virtual float GetVOIArea(vtkImageData* image,int level,int frame,float threshold=0.1);
  virtual int   GetVOIProperties(vtkImageData* image,int level,int frame,
				 float& area,float& mean,float& std,float threshold=0.0);
  
  virtual int   GetDualVOIProperties(vtkImageData* image,int level,int frame,
				     float area[2],float mean[2],float std[2],
				     int numpixels[2],float threshold[2]);
  


  // Description:
  virtual int  InsertKnot(float x,float y);
  virtual int  InsertKnot(float x,float y,float mindist);

  //BTX
  // Code to Interface to Internal Data Structures 
  virtual PXFPBSpline* GetSpline();

  // Contour, vtkPoints Interfaces
  virtual PXContour* ExportToContour();
  virtual int FitToContour(PXContour* cntr,float smooth=2.0,float csmooth=0.0,
			   int numpoints=-1,int closed=1,int iterations=100);

  //ETX 


  // Description:
  // Add To Objectmap
  virtual int  AddToObjectMap(vtkImageData* objectmap,int level=0,float value=1.0,int add=0);		    
  virtual int  ThresholdAddToObjectMap(vtkImageData* objectmap,vtkImageData*img, int level,int imagelevel,
				       float value=1.0,int thresholdmode=0,float min=0.0,float max=0.0);
  virtual int  AddToDistanceMap(vtkImageData* distancemap,int level=0);		    


  // Description:
  // Optimization Stuff
  static void  OptimizerFeedback(vtkObject* self,const char* line,double val);
  static double CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output);
  static double CalculateFunction(vtkObject* self,vtkDoubleArray* position);


protected:

  vtkpxSplineSource();
  virtual ~vtkpxSplineSource();
  vtkpxSplineSource(const vtkpxSplineSource&) {};
  void operator=(const vtkpxSplineSource&) {};
  
  void Execute();
  
  //BTX
  PXFPBSpline* Spline;
  float Resolution;
  //ETX

  double ImageSpacing[3];
  double ImageOrigin[3];


  // Description:
  // Snake Stuff
  //BTX
  float SNAKE_img, SNAKE_step, SNAKE_sigma, SNAKE_baloon,SNAKE_smooth,SNAKE_imagemax;
  int   SNAKE_niterations;
  vtkImageData* EdgeImage;
  vtkpxOptimizer* Optimizer;
  vtkDoubleArray* Position;
  //ETX

    // Description:
  // Needed for Optimizer -- set,get,optimizer,optimizervalue
  virtual void SetCurrentParameters(vtkDoubleArray* par);
  virtual void GetCurrentParameters(vtkDoubleArray* par);
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);


};

#endif



