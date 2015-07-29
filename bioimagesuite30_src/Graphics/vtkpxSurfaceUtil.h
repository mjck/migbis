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
  Date:      $Date: 2002/07/15 12:58:30 $
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
// .NAME vtkpxSurfaceUtil - a collection of static utility functions
// .SECTION Description
// This class contains a collection of <EM>static</EM> utility functions for use elsewhere ala vtkMath
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION Caveats
// This requires pxutil 

#ifndef __vtkpxSurfaceUtil_h
#define __vtkpxSurfaceUtil_h

#include "vtkObject.h"
#include "pxutil.h"
#include "pxcontour.h"
#include "pxcontourstack.h"
#include "pxgeometrystruct.h"
#include "pxtriangulatedstack.h"
#include "vtkPointData.h"
#include <string>

class vtkImageData;
class vtkLookupTable;
class vtkImageData;
class vtkPolyData;
class vtkTransform;
class vtkPoints;
class vtkMatrix4x4;
class vtkThinPlateSplineTransform;
class vtkGridTransform;
class vtkContourFilter;
class vtkHomogeneousTransform;
class vtkAbstractTransform;
class vtkDataArray;
class vtkFloatArray;
class vtkUnsignedCharArray;
class vtkPointLocator;

class vtkpxSurfaceUtil : public vtkObject
{
public:
  static vtkpxSurfaceUtil *New();
  vtkTypeMacro(vtkpxSurfaceUtil,vtkObject);
  
  //BTX
  // Draw Contour Stuff
  static void createsinglesection(vtkPolyData* sep,PXContour* cnt1,PXContour* cnt2,float z1,float z2,
				  float du=1.0,int debug=0);
  static void createsinglepolarsection(vtkPolyData* sep,PXContour* cnt1,PXContour* cnt2,
				       float theta1,float theta2,float middlex,
				       float du=1.0,int debug=0);
  static void createsection(PXContourStack* stack,vtkPolyData* grp,int polar=0,float middlex=0.0,int begin=-1,int end=100000);
  static void drawtstack(vtkPolyData* surface,PXTriangulatedStack* tstack);
  //ETX


  // -------------------------------------------------
  // ObjectMap  To PolyData
  // -------------------------------------------------
  
  static int ObjectMapToPolyData(vtkImageData* image,vtkPolyData* poly);
  static int ObjectMapToPolyData(vtkImageData* image,vtkPolyData* poly,int minobj,int maxobj,
				 double smooth=1.0,double smoothz=1.0,double reslice=1.0);
  static int MapScalarsToSurface(vtkPolyData* source,vtkPolyData* destination);
  static int AddObjectLabelsToSurface(vtkPolyData* source,int offset);

  static int DefaultObjectMapLookupTableNumberOfColors(); 
  static int DefaultObjectMapLookupTable(vtkLookupTable* lkp,int numc=-1,int blackzero=0);
  static int VOIObjectMapLookupTable(vtkLookupTable* lkp,int numc=-1);

  static int AddObjectMapLookupTableToSurface(vtkPolyData* source);
  static int AddShapeIndexLookupTableToSurface(vtkPolyData* source);
  static int AddCurvatureLookupTableToSurface(vtkPolyData* source);


  // --------------------------------------------------
  // Mesh Generation Random Stuff
  // --------------------------------------------------
  static int CleanPoints(vtkPolyData* input,vtkPolyData* other,vtkImageData* mask,int usemask);
  static int ExportQHull(vtkPolyData* input,const char* fname);
  static int AddLabels(vtkPolyData* inout,vtkPolyData* ref);
  static vtkPolyData* ReadQHull(const char* fname);
  static int SaveAbaqus(vtkPolyData* qhullsurface,const char* abname);

  // --------------------------------------------------
  // Neighbors Stuff -- needed for shape tracking etc
  // --------------------------------------------------
  static PXIntegerList* GenerateNeighbors(vtkPolyData* polydata,int opensurface);
  static int            CollectNeighbours(vtkPolyData* polydata,int point,int distance,
					  PXIntegerList* list,PXIntegerList* neighbors_array);
  static int            CalculateCurvatures(vtkPolyData* polydata,int scale,int opensurface,int c_mode=2,float upper=1.0);
  static int            CalculateCurvatures(vtkPolyData* polydata,int scale,PXIntegerList* neighbours);

  static int            NonShrinkSmooth(vtkPolyData* poly,int opensurface,
					float alpha,float beta,int iterations,int movez);
  static int            NonShrinkSmooth(vtkPolyData*,PXIntegerList* neighbors_array,
					float alpha,float beta,int iterations,int movez);

  // ---------------------------------------------------
  // Displacement Stuff
  // ---------------------------------------------------
  static void ZeroAllDisplacements(vtkPolyData* poly,vtkFloatArray* displ);
  static int  AddToDisplacements(vtkPolyData* orig,vtkPolyData* targ,vtkFloatArray* displ);
  static int  ShiftPointsByDisplacements(vtkPolyData* poly,vtkFloatArray* displ,float factor=1.0);


  // ---------------------------------------------------
  // Some Standard Colors
  // ---------------------------------------------------
  static void GetColor(int i,double c[3]);
  static const char* GetColor(int i);
  

  //BTX
  static std::string buffer;
  //ETX

};
#endif

