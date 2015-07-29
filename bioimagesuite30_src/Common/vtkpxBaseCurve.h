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
  Module:    $RCSfile: vtkpxBaseCurve.h,v $
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
// .NAME vtkpxBaseCurve - stores and manipulates a set of points.
// .SECTION Description
// The points can be represented either as landmarks or as an open or closed space curve.
// The output of this class is vtkPolyData
// This class essentially captures major aspects of curves as used in Medical Image Analysis.
// The points are stored in voxel coordinates and are scaled to mm using the Origin and Spacing
// parameters. If you want to store points as mm simply set spacing to 1.0 1.0 1.0 and Origin to 
// 0.0 0.0 0.0
// There are facilities for calculating areas, resampling, smoothing, iso-contour extraction and
// snakes
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION See Also
//  vtkPoints vtkPolyData vtkPolyDataSource 



#ifndef __vtkpxBaseCurve_h
#define __vtkpxBaseCurve_h

class vtkPoints;
class vtkPolyData;

#include "pxcontour.h"
#include "vtkpxUtil.h"
#include "vtkPolyDataSource.h"

class vtkpxBaseCurve : public vtkPolyDataSource 
{
public:
  static vtkpxBaseCurve *New();
  vtkTypeMacro(vtkpxBaseCurve,vtkPolyDataSource);

public:

  // Description:
  // Get the Number Of Points -- this is different from the actual number of points stored in the Points ivar
  vtkGetMacro(NumPoints,int);

  // Description:
  // For display purposes -- set the radius of the spheres used to display the points
  // also set the radius (x0.5) of the tubes used to display lines if ShowCurve=1
  vtkGetMacro(PointScale,double);
  vtkSetMacro(PointScale,double);

  // Description:
  // Determines whether the curve is treated as if it were open or closed
  vtkGetMacro(ClosedCurve,int);
  vtkSetMacro(ClosedCurve,int);

  // Description:
  // Show or hide spheres representing individual points
  vtkGetMacro(ShowControls,int);
  vtkSetMacro(ShowControls,int);

// Description:
  // Show or hide spheres representing individual points
  vtkGetMacro(ControlsDisplayMode,int);
  vtkSetClampMacro(ControlsDisplayMode,int,0,1);
  void SetControlsDisplayModeToSphere() { this->SetControlsDisplayMode(0);}
  void SetControlsDisplayModeToDisc() { this->SetControlsDisplayMode(1);}
  

  // Description:
  // Show or hide the lines joining the points
  vtkGetMacro(ShowCurve,int);
  vtkSetMacro(ShowCurve,int);

  // Description:
  // Show or hide the lines joining the points
  vtkGetMacro(ShowBoundingBox,int);
  vtkSetMacro(ShowBoundingBox,int);

  // Description:
  // Scale factors to scale from voxels (used for internal storage) to mm using for the ouput
  vtkGetVectorMacro(Spacing,double,3);
  vtkSetVectorMacro(Spacing,double,3);

  // Description:
  // Shift factor to translate from voxels (used for internal storage) to mm using for the ouput
  vtkGetVectorMacro(Origin,double,3);
  vtkSetVectorMacro(Origin,double,3);

  // Description:
  // Change voxel coordinates to reflect new spacing/origin
  void RescaleOriginSpacing(double ori[3],double sp[3]);
  void RescaleSpacing(double sx,double sy,double sz);
  
  // I/O
  // Description:
  // Store in .land format
  virtual int          Save(const char* fname);  

  // Description:
  // Load from .land format
  virtual int          Load(const char* fname);

  // Description:
  // Scale internal points to mm and make Points ready for export (adjusting number of points etc).
  // Use this before GetPoints() is called
  virtual int          Compact();

  // Description:
  // Gets internal storage. Use Compact before calling this
  virtual vtkPoints*   GetPoints();

  // Description:
  // Copy another curve
  virtual void         Copy(vtkpxBaseCurve* other);

  // Description:
  // Get Individual Point Coordinate
  virtual void         GetPoint(int index,double& x,double& y,double &z);

  // Description:
  // Set Point index to (x,y,z), if scaled=1 then we assume that (x,y,z) are in mm else
  // if scaled=0 they are treated as voxels
  virtual void         SetPoint(int index,double x,double y,double z,int scaled);
  virtual void         SetPoint(int index,double x,double y,double z) { SetPoint(index,x,y,z,1);}

  // Description:
  // Add point (x,y,z), if scaled=1 then we assume that (x,y,z) are in mm else
  // if scaled=0 they are treated as voxels
  virtual void         AddPoint(double x,double y,double z,int scaled);
  virtual void         AddPoint(double x,double y,double z) { AddPoint(x,y,z,1);}

  // Description:
  // Delete point i
  virtual void         DeletePoint(int i=-1);


  // Description:
  // Delete All points
  virtual void         DeleteAllPoints();

  //BTX

  // Interface with PXContour and vtkPolyData
  virtual int          StoreInContour(PXContour* cntr,int plane);
  virtual int          SetFromContour(PXContour* cntr,int plane=2,int level=0);
  virtual int          SetFromPolyData(vtkPolyData* polydata,double x[3],int plane=-1,int level=0);

  //ETX


  // Description:
  // Get Plane Based on Points
  virtual int GetCurrentInternalPlane();
  virtual void GetCurrentInternalPlaneAndLevel(int& plane,int& level);


  // Description:
  // Using the first point as origin and the second to find the radius
  // it creates a plance circle with the other coordinate=level and plane
  // plane=0 XY, 1=XZ, 2=YZ
  virtual int          CreateCircle(int plane,int level);

  // Description:
  // Sets points from points
  virtual int          SetFromPoints(vtkPoints* points);

  // Curve Properties
  // Description:
  // Finds closest point with distance < threshold
  // If scale==1 px is assumed to be in mm otherwise in voxel
  virtual int          FindClosestPoint(double px[3],double threshold,int scale=1);
  virtual int          FindClosestPoint(double x,double y, double z,double threshold,int scale=1);

  // Description:
  // Return area of curve in plane plane (see CreateCircle)
  virtual float        GetPlaneArea(int plane=2);  
  virtual float        GetLength();

  //BTX
  virtual int          ExtractVOIProperties(vtkImageData* image,int plane,int level,int fr,float& mean,float& sigma);
  virtual float        GetAreaFromPixels(int num,int image_plane);
  //ETX

  // Curve Operations 
  // Description:
  // Resamples curve by factor rate
  virtual void         Sample(int rate=2);

  // Description:
  // Subsamples curve by factor rate
  virtual void         SubSample(int rate=2);

  // Description:
  // Smooth curve
  virtual void         Smooth(float relaxation=0.33);

  //BTX
  virtual void         RemoveIntersections(int keeplongest,int image_plane,int image_level);
  //ETX

  // Description:
  // Equispace curve at distance sp
  virtual void         Equispace(float sp=-1.0);

  //BTX
  virtual int ExtractImageContour(vtkImageData* image,
				  int plane,int level,int frame,
				  float& threshold,int median=0,
				  float sigma=0.0);


  virtual vtkImageData*  UpdateSnake(vtkImageData* image,vtkImageData* energyMap,
					    int plane,int level,int frame,
					    float& residual,float edge=1.0,float baloon=0.0,
					    float smooth=1.0,float sigma=1.0,float step=0.2,int maxiter=10);
  //ETX

  virtual int AddToImage(vtkImageData* image,float value);

  //BTX
  // Interpolation Stuff
  static double GetDoubleVoxel(vtkImageData* img,int i,int j,int k,int dim[3]);
  static double GetImageVoxel(vtkImageData* img,double x,double y,double z,int dim[3]);
  static double GetImageValue(vtkImageData* img,double x,double y,double z,double spacing[3],double origin[3],int dim[3]);
  //ETX

  // Segmentation Stuff
  // Description:
  // Create Edge map for slice=level plane=plane, sigma=sigma 
  static  vtkImageData*  CreateEnergyMap(vtkImageData* image,int plane,int level,int frame,float sigma);
  
  // Description:
  // Export to poly data after equisampling resampling to distance d (d<=0.0 -> no resampling)
  int ExportToPolyData(vtkPolyData* output,float d);

  
protected:
  vtkpxBaseCurve();
  virtual ~vtkpxBaseCurve();

  vtkpxBaseCurve(const vtkpxBaseCurve&) {};
  void operator=(const vtkpxBaseCurve&) {};

  vtkPoints             *Points;
  int                   NumPoints;
  int                   ShowControls;
  int                   ControlsDisplayMode;
  float                 PointScale;
  double                Spacing[3];
  double                Origin[3];
  int                   ClosedCurve;
  int                   ShowCurve;
  int                   ShowBoundingBox;

  void Execute();  
};

#endif   


