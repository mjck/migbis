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
  Module:    $RCSfile: vtkpxProcessFmri.h,v $
  Language:  C++
  Date:      $Date: 2002/09/26 19:13:42 $
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
// .NAME vtkpxProcessFmri -- creates activation maps from raw fmri data
// .SECTION Description
// This class takes a number of fmri image series and a description of activations
// and generates t-maps of the activation.
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>

#ifndef __vtkpxProcessFmri_h
#define __vtkpxProcessFmri_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkPolyData;
class vtkDataArray;

class vtkpxProcessFmri : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkpxProcessFmri *New();
  vtkTypeMacro(vtkpxProcessFmri,vtkpxSimpleImageToImageFilter);

  // Description:
  // Sets the activation black diagram for input=0 (If others are NULL this is used for all inputs)
  virtual void SetActivations(vtkDataArray* act);

  // Description:
  // Set value of scalefactor to multiply t-values
  vtkGetMacro(ScaleFactor,float);
  vtkSetMacro(ScaleFactor,float);

  // Description:
  // Enable IntensityRescaling of each slice to have the same mean
  vtkGetMacro(IntensityRescaling,int);
  vtkSetMacro(IntensityRescaling,int);

  // Description:
  // Enable Right/Left Flip of Output
  vtkGetMacro(FlipLeftRight,int);
  vtkSetMacro(FlipLeftRight,int);

  // BTX
  static void ProcessScalars(vtkDataArray* activations,int frames,int tshift,
			     int numact,char* activ,
			     int numbase,char* base);
  static void CreatePolyDataFromScalars(vtkDataArray* activations,vtkPolyData* polydata);
  // ETX
  static void CreatePolyDataFromScalars(vtkDataArray* activations,vtkPolyData* polydata,float scale);


  // Description:
  // MR Intensity Gain Estimation and Re-Normalization
  static void NormalizeIntensity(vtkImageData* input,vtkImageData* output,float newmean);
  
protected:

  vtkpxProcessFmri();
  virtual ~vtkpxProcessFmri();
  vtkpxProcessFmri(const vtkpxProcessFmri&) {};
  void operator=(const vtkpxProcessFmri&) {};

  void SimpleExecute(vtkImageData*,vtkImageData*);
  void ExecuteInformation();

  vtkDataArray* Activations;
  float       ScaleFactor;
  int         IntensityRescaling;
  int         FlipLeftRight;


  void CheckActivationSeries(vtkDataArray* act,int timeframes,float& Numbaseline,float& Numactivation);

  
};

#endif

