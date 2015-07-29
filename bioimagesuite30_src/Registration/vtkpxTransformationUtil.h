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
  Module:    $RCSfile: vtkpxTransformationUtil.h,v $
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
// .NAME vtkpxTransformationUtil - a collection of static utility functions
// .SECTION Description
// This class contains a collection of <EM>static</EM> utility functions for use elsewhere ala vtkMath
// These deal with images and transformations.
// <br> <br> <EM>Copyright (c) 1995-2002 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION Caveats
// This requires pxutil 

#ifndef __vtkpxTransformationUtil_h
#define __vtkpxTransformationUtil_h

#include "vtkObject.h"
#include "vtkObjectFactory.h"
#include "pxutil.h"

class vtkMatrix4x4;
class vtkThinPlateSplineTransform;
class vtkpxGridTransform;
class vtkHomogeneousTransform;
class vtkAbstractTransform;
class vtkDataArray;
class vtkPoints;
class vtkTransform;
class vtkpxMatrix;
class vtkImageData;
class vtkPolyData;
class vtkFloatArray;



class vtkpxTransformationUtil : public vtkObject
{
public:
  static vtkpxTransformationUtil *New();
  vtkTypeMacro(vtkpxTransformationUtil,vtkObject);
  

  // -----------------------------------------------------
  // Transformation Stuff
  // -----------------------------------------------------

  // Description:
  // Save And Load Abstract Transformation as matrix/grid/thin-plate-spline
  static int SaveAbstractTransform(vtkAbstractTransform* tr,const char* fname);
  static vtkAbstractTransform* LoadAbstractTransform(const char* fname);


  // Description:
  // Matrix I/O
  static int LoadMatrix(vtkMatrix4x4* mat,const char* fname);
  static int SaveMatrix(vtkMatrix4x4* mat,const char* fname);

  static int LoadMatrixTransform(vtkTransform* tr,const char* fname);
  static int SaveMatrixTransform(vtkTransform* tr,const char* fname);

  // Description:
  // Thin-Plate-Spline I/O
  static int LoadThinPlateSplineTransform(vtkThinPlateSplineTransform* mat,const char* fname);
  static int SaveThinPlateSplineTransform(vtkThinPlateSplineTransform* mat,const char* fname);

  // Description:
  // Thin-Plate-Spline I/O from stream
  static int LoadThinPlateSplineTransform(vtkThinPlateSplineTransform* mat,gzFile fin);
  static int SaveThinPlateSplineTransform(vtkThinPlateSplineTransform* mat,FILE* fout);


  // Description:
  // Grid Transform  I/O
  static int LoadGridTransform(vtkpxGridTransform* mat,const char* fname);
  static int SaveGridTransform(vtkpxGridTransform* mat,const char* fname);


  // Description:
  // Creates reslice transform given 2 or 3 points (specified by npoints ) stored in vtkPoints 
  static int     GenerateTransform(vtkTransform* tr,vtkPoints* pt,int npoints=-1);

  // Description:
  // Fixes mmvreg output to standard coordinate space
  static void FixMmvregOutput(vtkMatrix4x4* matrix,int permute);

  // Description:
  // GetJacobian
  static float GetJacobian(vtkAbstractTransform* tr,vtkpxMatrix* jac,float x,float y,float z);

  // Description:
  // Mouse Model Stuff
    // Description:
  // Create Basis Image
  static int CreatePolygonalModel(vtkImageData* multiframeinput,vtkPolyData* surface,float threshold=50.0);
  static vtkImageData* CreateRegionLabelImage(vtkImageData* input,float sigma);
  static vtkImageData* CreateRegionLabelImage(vtkImageData* input,float sigma,vtkPolyData* surface);
  static vtkImageData* ChopLeg(vtkImageData* Leg,vtkFloatArray* planes,int offset,int numcuts,int legmode=1);

  static vtkImageData* GetMaskImage(vtkImageData* input,int above,double n[3],double c[3],double offset);
  static vtkImageData* GetMaskImage(vtkImageData* input,vtkFloatArray* planes,int offset);
  static vtkImageData* GetMaskImage(vtkImageData* input,vtkFloatArray* planes,int offset,int flip);
  static vtkImageData* CreateWholeBodyRegionLabelImage(vtkImageData* input,vtkFloatArray* planes,int hasfeet,
						       int hashindlimbs,int hasforelimbs,int hashead, int taildivisions,
						       int spinedivisions);


  // Joint Locations
  // ----------------------------------------
  static vtkPoints* GetCentroids(vtkImageData* img,double axislength=10.0);
  static int        GetPlaneNumber(int jointno,int hasfeet,
				   int hashindlimbs,int hasforelimbs,int hashead, int taildivisions,
				   int spinedivisions);
    



 
};
#endif

