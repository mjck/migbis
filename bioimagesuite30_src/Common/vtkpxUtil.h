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
  Module:    $RCSfile: vtkpxUtil.h,v $
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
// .NAME vtkpxUtil - a collection of static utility functions
// .SECTION Description
// This class contains a collection of <EM>static</EM> utility functions for use elsewhere ala vtkMath
// These deal with images and transformations.
// <br> <br> <EM>Copyright (c) 1995-2002 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION Caveats
// This requires pxutil 

#ifndef __vtkpxUtil_h
#define __vtkpxUtil_h

#include "vtkObject.h"
#include "vtkRenderer.h"
#include "vtkFloatArray.h"
#include "vtkDataArray.h"
#include "vtkPointData.h"
#include "pxutil.h"

class vtkImageData;
class vtkLookupTable;
class vtkImageData;
class vtkPolyData;
class vtkPoints;
class vtkContourFilter;
class vtkAbstractTransform;
class vtkDataArray;
class vtkpxMatrix;





class vtkpxUtil : public vtkObject
{
public:
  static vtkpxUtil *New();
  vtkTypeMacro(vtkpxUtil,vtkObject);
  

  // -------------------------------------------------------------------
  // Create And Copy Image Routines -- these are HUGELY important
  // -------------------------------------------------------------------

  // Description:
  // Create Image To have identical dimenions as Input
  static int CreateImage(vtkImageData* img,vtkImageData* SameSizeAsThisOne);

  // Description:
  // Create Float/Short or Generic Type Image with specified dimensions
  static int CreateFloatImage(vtkImageData* img,
			      int width,int height,int depth,int numcomp,
			      float sp1,float sp2,float sp3,
			      float or1,float or2,float or3);
  
    
  static  int CreateShortImage(vtkImageData* img,int width,int height,int depth,int numcomp,
			       float sp1,float sp2,float sp3,
			       float or1,float or2,float or3);
  
  static int CreateImage(vtkImageData* img,
			 int width,int height,int depth,int numcomp,
			 int scalar_type,
			 float sp1,float sp2,float sp3,
			 float or1,float or2,float or3);

  // -------------------------------------------------------------------
  // Basic Image Operations 
  // -------------------------------------------------------------------

  // Description:
  // Reorder 4D Image into frame-major order mode from slice-major order mode
  static int   ReorderImage(vtkImageData* destination,vtkImageData* source);

  // Description:
  // Extract Frames
  static int   ExtractFrames(vtkImageData* destination,vtkImageData* source,int begin,int end,int inc);

  // Description:
  // Resample image to spacing resol*resol*resol after smoothing with Gaussian sigma (all in mm)
  static int ResampleAndSmoothImage(vtkImageData* destination,
				    vtkImageData* source,
				    float gaussian,
				    float resolution,
				    float padvalue);

  static int ResampleAndSmoothImage(vtkImageData* destination,
				    vtkImageData* source,
				    float gaussian,
				    float resolution);

  // Description:
  // Average absolute difference of pixel intensity values of all voxels
  // If ignoreon=1, then any pixel with value=ignore is not used
  static float ComputeAbsoluteImageDifference(vtkImageData* image1,
					      vtkImageData* image2,
					      int ignoreon=0,
					      float ignore=-1.0);
  

  
  static int ComputeImageNormalizationFactors(vtkImageData* ref,
					      vtkImageData* scaled,
					      vtkFloatArray* params,
					      float threshold=0.0);
						
  // Description:
  // Using vtkpxAnalyzeImageWriter, saves ImageData to analyze file
  static int   SaveAnalyze(const char* fname,vtkImageData* points,int compression=0);

  // ------------------------------------------------------------------
  //  Reslice image input to have same dimensions as image reference 
  // and store in output
  //
  // Use transformation = transform
  //     interpolation mode = interp (0,1,3)
  //     background    value = background
  // ------------------------------------------------------------------

  static int EquiReslice(vtkImageData* output,
			 vtkImageData* input,
			 vtkImageData* reference);
  

  static int ResliceImage(vtkImageData* output,
			  vtkImageData* input,
			  vtkImageData* reference,
			  vtkAbstractTransform* transform,
			  int interp,float background);

  static int AutoCropImage(vtkImageData* img,int offset=5);

  // ------------------------------------------------------------------
  // Re-orient Image Routines 
  // ------------------------------------------------------------------
  
  
  // Description:
  // Procedures to Re-Orient an Image from Coronal/Axial/Saggital to C/A/S
  static int ReOrientMatrix(vtkImageData* source,int inputorient,int outputorient,vtkMatrix4x4* tmat,int centered=0);
  static int ReOrientImage(vtkImageData* destination,vtkImageData* source,int inputorient,int outputorient);

  //BTX
  // Lower Level Helper Routines
  static int ReOrientMatrix(double ori[3],double sp[3],int dim[3],int inputorient,int outputorient,vtkMatrix4x4* tmat,int centered=0);
  static int ReOrientDimensions(double sp_in[3],int dim_in[3],int inputorient,int outputorient,double sp_out[3],int dim_out[3]);
  //ETX
  


  // -------------------------------------------------
  // Utility Code
  // -------------------------------------------------

  static float Fmin(float a,float b);
  static float Fmax(float a,float b);
  static float Frange(float a,float minv,float maxv);

  static int Imin(int a,int b);
  static int Imax(int a,int b);
  static int Irange(int a,int minv,int maxv);

  // -------------------------------------------------------
  // Image Robust Range -- moved here from vtkpxNonLinearFit
  // -------------------------------------------------------
  static void ImageRobustRange(vtkImageData* image,float perlow,float perhigh,float& tlow,float& thigh,float dim=32.0);
  static void ImageRobustRange(vtkImageData* image,float perlow,float perhigh,vtkFloatArray* params,float dim=32.0);


  // -------------------------------------------------------
  // Compute Sum of Image Product
  // -------------------------------------------------------
  static double ComputeImageProductSum(vtkImageData* img1,vtkImageData* img2);
  static double ComputeImageSum(vtkImageData* img1,double threshold=0.05);
 
};
#endif

