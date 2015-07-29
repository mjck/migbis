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
  Module:    $RCSfile: vtkpxLevelSetUtil.h,v $
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
// .NAME vtkpxLevelSetUtil - a collection of static utility functions
// .SECTION Description
// This class contains a collection of <EM>static</EM> utility functions for use elsewhere ala vtkMath
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION Caveats
// This requires pxutil 

#ifndef __vtkpxLevelSetUtil_h
#define __vtkpxLevelSetUtil_h


#include "vtkImageData.h"
#include "vtkPoints.h"
#include "itkImage.h"
#include "vtkFloatArray.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "vtkITKUtility.h"

enum { pxImageDimension = 3 };
typedef  float  pxPixelType;
typedef itk::Image<pxPixelType,pxImageDimension>    pxImageType;
typedef itk::VTKImageImport<pxImageType> pxImportType;
typedef itk::VTKImageExport<pxImageType> pxExportType;


class vtkpxLevelSetUtil : public vtkObject
{
public:
  static vtkpxLevelSetUtil *New();
  vtkTypeMacro(vtkpxLevelSetUtil,vtkObject);

  static vtkImageData* SigmoidFilter(vtkImageData* input,
				     float SigmoidAlpha=0.5,float SigmoidBeta=0.0,
				     float SigmoidMin=-1.0,float SigmoidMax=1.0);

  static vtkImageData* GenerateGradientMagnitudeSpeedImage(vtkImageData* input,int SmoothingIterations=5,float Conductance=9.0,
							   float GradientSigma=0.5,
							   float SigmoidAlpha=0.5,float SigmoidBeta=0.0,
							   float SigmoidMin=-1.0,float SigmoidMax=1.0);
  static vtkImageData* GenerateInputLevelSet(vtkImageData* input,vtkPoints* points,float offset=5.0,float factor=1.0);

  static int ResampleAndSmoothImage(vtkImageData* destination,vtkImageData* source,float factor);
  static int ResliceDistanceMap(vtkImageData* destination,vtkImageData* source,vtkImageData* ref);

  
};
#endif

