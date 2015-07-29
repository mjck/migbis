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
  Module:    $RCSfile: vtkpxColorMapUtil.h,v $
  Language:  C++
  Date:      $Date: 2002/09/24 14:39:36 $
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
// .NAME vtkpxColorMapUtil - a collection of static utility functions
// .SECTION Description
// This class contains a collection of <EM>static</EM> utility functions for use elsewhere ala vtkMath
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION Caveats
// This requires pxutil 

#ifndef __vtkpxColorMapUtil_h
#define __vtkpxColorMapUtil_h

#include "vtkObject.h"

class vtkImageData;
class vtkLookupTable;
class vtkImageData;
class vtkDataArray;

class vtkpxColorMapUtil : public vtkObject
{
public:
  static vtkpxColorMapUtil *New();
  vtkTypeMacro(vtkpxColorMapUtil,vtkObject);
  


  // Description:
  // Gets intensity range of image and computes bounds for step function 
  static void  ComputeStepBounds(vtkImageData* image,vtkDataArray* output,int np=1001,float nsigma=3.0);

  // Colormap Stuff
  // Description:
  // Update Colormap from image
  static void AutoUpdateColormapRange(vtkLookupTable* cmap,vtkImageData* img,int opaque=0);

  // Description:
  // Create qunatized version of image source to destination and accompanying vtkLookupTable
  static int  QuantizeColorImage(vtkImageData* destination,vtkImageData* source,vtkLookupTable* cmap);

  // Description:
  // Creates gamma modified colormap (vol=1, is for volume rendering)
  static void SetGammaColorMap(vtkLookupTable *cmap,float gamma,float min_value,float max_value,
					 int num_colors,int vol=0, int opaque=0);

  // Description:
  // Creates constant hue colormap (vol=1, is for volume rendering)
  static void SetConstantHueColorMap(vtkLookupTable* cmap,float intensity,float hue1,float hue2,float cutoff,
				     float min_value,float max_value,int num_colors,int vol=0, int opaque=0);

  // Description:
  // Creates step colormap (vol=1, is for volume rendering)
  static void SetStepColorMap(vtkLookupTable *cmap,float min,float max,float min_value,float max_value,
			      int num_colors,int vol=0, int opaque=0);

  // Description:
  // Creates a rainbow colormap (vol=1, is for volume rendering)
  static void SetRainbowColorMap(vtkLookupTable *cmap,float min,float max,float min_value,float max_value,
			      int num_colors,int vol=0, int opaque=0);

    // Description:
  // Creates an inverted rainbow colormap (vol=1, is for volume rendering)
  static void SetReverseRainbowColorMap(vtkLookupTable *cmap,float min,float max,float min_value,float max_value,
			      int num_colors,int vol=0, int opaque=0);

    // Description:
  // Creates an blackbody radiation colormap (vol=1, is for volume rendering)
  static void SetBlackBodyColorMap(vtkLookupTable *cmap,float min,float max,float min_value,float max_value,
			      int num_colors,int vol=0, int opaque=0);

  // Description:
  // Set Auto Step Colormap
  static void SetAutoStepColorMap(vtkImageData* img,vtkLookupTable* cmap,int robust,int num_colors,int volume, int opaque);


  // Description:
  // Creates  colormap for fmri overlays (vol=1, is for volume rendering)
  static void SetFmriMergeColormap(vtkLookupTable* cmap,int mode=0,int vol=0,int opaque=0);
  static void SetScalarBarColormap(vtkLookupTable* cmap,int mode,double minv,double maxv,int positive=1);

  // Description:
  // Create Overlay Color Map
  static void CreateOverlayColorMap(vtkLookupTable *cmap,int mode,float min,float max,float min_value,float max_value,
				    float intensity,float hue1,float hue2,int vol,int updatefunction=1,int opaque=0);

  // Description:
  // CopyLookupTable
  static void CopyLookupTable(vtkLookupTable* in,vtkLookupTable* out);

  //BTX
  // Description:
  // Convert RGB to HSV
  static void RGBtoHSV( float r, float g, float b, float &h, float &s, float &v );
  static void HSVtoRGB( float *r, float *g, float *b, float h, float s, float v );
  //ETX

  // Description:
  // Colormap Load and Save both native .cmap and Analyze
  static int LoadColormap(vtkLookupTable* colormap,const char* name);
  static int SaveColormap(vtkLookupTable* colormap,const char* name);
  static int LoadAnalyzeColormap(vtkLookupTable* colormap,const char* name,int volume_mode=0);
  static int SaveAnalyzeColormap(vtkLookupTable* colormap,const char* name);

};
#endif

