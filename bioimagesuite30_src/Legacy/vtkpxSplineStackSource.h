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


Copyright (c) 1995-2002 Xenios Papademetris papad@noodle.med.yale.edu
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

#ifndef __vtkpxSplineStackSource_h
#define __vtkpxSplineStackSource_h

#include "vtkPolyDataSource.h"


//BTX
class PXContourStack;
class PXFPBSpline;
class PXFPBSplineStack;
//ETX

typedef float real;

#include "vtkpxSplineSource.h"

class vtkImageData;
class vtkPolyDataCollection;
class vtkCollection;

class vtkpxSplineStackSource : public vtkPolyDataSource
{
public:
  static vtkpxSplineStackSource *New();
  vtkTypeMacro(vtkpxSplineStackSource,vtkPolyDataSource);


  // Description:
  // Initialization
  virtual void SetEllipticalCylinder(int np,int numslices,float radiusx=10.0,float radiusy=5.0,
				     float x=50.0,float y=50.0,float zspacing=1.0,float bottomz=0.0);
  virtual int SetLizzardCylinder(int np,int numslices,int bottomtrim,
				 int lizzardslices, 
				 int blendslices,int thinslices,
				 float minradius,float maxradius,
				 float yscale=1.2,
				 float x=50.0,float y=50.0,float 
				 zspacing=1.0,float bottomz=0.0);

  virtual void Copy(vtkpxSplineStackSource* other,int ignoretrim);
  virtual void Copy(vtkpxSplineStackSource* other) { Copy(other,0);}
  
  // Descripition
  // Input/Output to .sur format 
  virtual int  Load(const char* fname);
  virtual int  Load(gzFile fin);
  virtual int  Save(const char* fname);
  virtual int  Save(FILE* fout);
  
  // Description:
  // Display Parameter 
  vtkGetMacro(DrawMode,int);
  virtual void SetDrawMode(int md);

  // Description:
  // Sampling Rate in-plane 
  vtkGetMacro(ResolutionU,float);
  vtkSetMacro(ResolutionU,float);

  // Description:
  // Display Offset for top & bottom slices to make them visible
  vtkGetMacro(ZDisplayOffset,float);
  vtkSetMacro(ZDisplayOffset,float);

  // Description:
  // Sampling Rate out-of-plane
  vtkGetMacro(ResolutionV,float);
  vtkSetMacro(ResolutionV,float);

  // Description:
  // Center of rotation for polar (3D ultrasound) spline 
  vtkGetMacro(PolarMiddle,float);
  vtkSetMacro(PolarMiddle,float);

  // Description:
  // Set/Get Polar Mode 
  vtkGetMacro(PolarMode,int);
  virtual void SetPolarMode(int pm);

  // Description:
  // Update Whole Surface 
  vtkGetMacro(WholeSurface,int);
  virtual void SetWholeSurface(int ws);


  // Description:
  // Force Re-Draw of Surface 
  vtkSetMacro(RedrawSurface,int);
  vtkGetMacro(RedrawSurface,int);

  // Description:
  // Trims are extent of spline in image numsplines = numslices 
  virtual int  GetTrimBottom();
  virtual int  GetTrimTop();
  virtual void SetTrimBottom(int tb);
  virtual void SetTrimTop(int tt);
  virtual int  GetNumSplines();
  virtual float GetZSpacing();
  
  // Description:
  // Set Spline 
  int SetSpline(vtkpxSplineSource* source,int ind);
  int SetSplineFromSplineStack(vtkpxSplineSource* source,int ind);

  // BTX
  virtual PXFPBSpline* GetSpline(int i);
  virtual int SetSpline(PXFPBSpline* spl,int i);

  // ContourStac Interface
  virtual int FitToContourStack(PXContourStack* stack,float smooth=5.0,float csmooth=0.5,
				int npoints=-1,int closed=1);
  virtual PXContourStack* ExportToContourStack(real dist);
  virtual PXContourStack* ExportToContourStack(real dist,real dist2);
  // ETX

  // Description:
  // Cardiac Related Isuues, to set Apex 
  virtual void SetApex(float x,float y,float z);
  virtual void GetApex(float &x,float &y,float &z);
  virtual void EnableApex(int enable=1);
  virtual int  ApexEnabled();
  virtual void AutoSetApex(int active=1);



  // Description:
  // Resample and Smooth Spline 
  virtual int Resample(float ds_step,int npoints);
  virtual int Resample(int npoints) { return Resample(0.01,npoints);}
  virtual int Resample(float ds) { return Resample(ds,-1);}
  virtual int ChangeSpacing(float smooth=0.5,float npoints=-1.0,
			    float step=0.05,float csmooth=0.0,int iterations=50);


  // Description:
  // Shift And Scale Z-Coordinate 
  virtual void ScaleShiftZ(float shift,float scale);
  virtual void ScaleShiftXY(float shiftx,float shifty,float scalex,float scaley);
  virtual void AdjustNumberOfSplines(int newnumber);

  // Description: 
  // VOI Stuff area, mean intensity area>0 std of mean intensity 
  virtual float GetVolume();
  virtual float GetVOIVolume(vtkImageData* image,int frame,float threshold=0.1);
  virtual int   GetVOIProperties(vtkImageData* image,int frame,
				 float& volume,float& mean,float& std,float threshold=0.0);
  virtual int   GetDualVOIProperties(vtkImageData* image,int frame,
				     float volume[2],float mean[2],float std[2],
				     int numvoxels[2],float threshold[2]);
  

  
  virtual int  AddToObjectMap(vtkImageData* objectmap,float value=1.0,int add=0,int background=0);
  virtual int  CreateObjectMap(vtkImageData* objectmap,int inside=2,int outside=1,int offslice=0);
  virtual int  CreateBoundedObjectMap(vtkpxSplineStackSource* bounding,vtkImageData* objectmap,
				      int inside_below=2,int inside_above=3,
				      int outside=1,int offslice=0);
  virtual int  FillDistanceMap(vtkImageData* distancemap);


  // Description:
  // Flip Left
  virtual int FlipX(vtkImageData* image);

  // Description:
  // Get the raw spline stack
  virtual PXFPBSplineStack* GetSplineStack();

  // Description:
  virtual void LoadAndFitContourStack(const char* fname);


  // Description:
  // Compute Z-range
  static int ComputeZRange(vtkCollection* collection,int numframes,int numsurfaces,
			   int beginframe,int endframe,
			   int index,float z[2]);


  // Description:
  // Upsample, i.e. from interpoated tracings add in-between slice
  virtual void UpSample();

  // Description:
  // 
  virtual int CreateSurfaceFromImage(vtkImageData* img,int minslice,int maxslice,int frame,float curvesmooth=1.0,float sigma=1.0);

  // Description:
  // A few static methods for Zak
  static vtkImageData* PreprocessBinaryImage(vtkImageData* img,int frame);
  static int SetFromPolyData(PXContour* cntr,vtkPolyData* polydata);
  static PXContour* ExtractContourFromImage(vtkImageData* img,int slice,float sigma);
  static PXContourStack* ExtractContourStackFromImage(vtkImageData* img,int& minslice,int& maxslice,float sigma=1.0);

  
		    
protected:

  vtkpxSplineStackSource();
  virtual ~vtkpxSplineStackSource();
  vtkpxSplineStackSource(const vtkpxSplineStackSource&) {};
  void operator=(const vtkpxSplineStackSource&) {};
  
  void Execute();
  
  PXFPBSplineStack* SplineStack;
  float ResolutionU;
  float ResolutionV;
  float PolarMiddle;
  int   DrawMode;
  float ZDisplayOffset;

  int      WholeSurface;
  int      PolarMode;
  int      ShowSplinesOnly;
  int      RedrawSurface;

  vtkPolyDataCollection* sliceSurfaceData;
  vtkPolyData*           wholeSurfaceData;


  void  CreateSurface();
  void  AddApex();
  void  AddSections(int lower,int upper);
  void  RemoveSections(int lower,int upper);
  void  CreateSingleSection(int splineno);



};

#endif



