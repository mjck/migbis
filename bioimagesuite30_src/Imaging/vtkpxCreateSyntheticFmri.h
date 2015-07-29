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
  Module:    $RCSfile: vtkpxCreateSyntheticFmri.h,v $
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
// .NAME vtkpxCreateSyntheticFmri -- compute Field Map from succesive phase images 
// .SECTION Description

#ifndef __vtkpxCreateSyntheticFmri_h
#define __vtkpxCreateSyntheticFmri_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkPolyData;
class vtkAbstractTransform;
class vtkTransform;

class vtkpxCreateSyntheticFmri : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkpxCreateSyntheticFmri *New();
  vtkTypeMacro(vtkpxCreateSyntheticFmri,vtkpxSimpleImageToImageFilter);

  // Description:
  // Set the number of frames 
  vtkGetMacro(NumberOfFrames,int);
  vtkSetClampMacro(NumberOfFrames,int,2,200);

  // Description:
  // Set value of the block period e.g 8on/8off
  vtkGetMacro(BlockPeriod,int);
  vtkSetClampMacro(BlockPeriod,int,1,100);

  // Description:
  // Set value of scale factor to add step change on on blocks
  vtkGetMacro(ScaleFactor,float);
  vtkSetClampMacro(ScaleFactor,float,0.0001,1.0);

  // Description: 
  // Set/Get the Activation Image 
  vtkSetObjectMacro(ActivationMap, vtkImageData);
  vtkGetObjectMacro(ActivationMap, vtkImageData);

  // Description:
  // Perform ROC Analysis on Tmaps to find true positives and false positives 
  static int DoROCAnalysis(vtkImageData* input1,
			   vtkImageData* trueinput,
			   vtkFloatArray* tvaluesin,
			   vtkFloatArray* tripletsout,
			   float ignore=-32000);


  // Description:
  // Compute Reslice Transformation given Reslice Set and Reslice Image 
  static int ComputeResliceTransformation(vtkTransform* trans,
					  vtkImageData* inputImage,
					  float offsetx,float offsety,float offsetz,
					  int   dimx,int dimy,int dimz,
					  float fovx,float fovy,float slicespacing,
					  float anglex,float angley,float anglez);  
  
  static int  ResliceImageUsingOffsets(vtkImageData* inputImage,
				       vtkImageData* outputImage,
				       vtkTransform* trans,
				       int dimx,int dimy,int dimz,
				       float fovx,float fovy,float slicespacing,
				       float slicethickness,
				       int interpolation_mode);

  static int CreateReferenceImageGeometry(vtkImageData* inputImage,
					  vtkImageData* outputReference,
					  int dimx,int dimy,int dimz,
					  float fovx,float fovy,float slicespacing);

  static int CreatePolyData(vtkTransform* trans,
			    vtkImageData* inputImage,
			    vtkPolyData* polydata,
			    int xr,int yr,
			    int   dimx,int dimy,int dimz,
			    float fovx,float fovy,float slicespacing);
  
  
protected:

  vtkpxCreateSyntheticFmri();
  virtual ~vtkpxCreateSyntheticFmri();
  vtkpxCreateSyntheticFmri(const vtkpxCreateSyntheticFmri&) {};
  void operator=(const vtkpxCreateSyntheticFmri&) {};

  void SimpleExecute(vtkImageData* input,vtkImageData *output);  
  void ExecuteInformation();

  float                ScaleFactor;
  int                  NumberOfFrames;
  int                  BlockPeriod;
  vtkImageData* ActivationMap;


};

#endif

