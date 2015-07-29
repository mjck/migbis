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
  Module:    $RCSfile: vtkpxProcessRawBrukerFIDImage.h,v $
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
// .NAME vtkpxProcessRawBrukerFIDImage -- compute phase/magnitude from raw reconstruction image on Bruker
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>

#ifndef __vtkpxProcessRawBrukerFIDImage_h
#define __vtkpxProcessRawBrukerFIDImage_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkIntArray.h"


class vtkpxProcessRawBrukerFIDImage : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkpxProcessRawBrukerFIDImage *New();
  vtkTypeMacro(vtkpxProcessRawBrukerFIDImage,vtkpxSimpleImageToImageFilter);

  // Description:
  // Mode -- is input echoplanar or not
  vtkSetClampMacro(EchoPlanarMode,int,0,1);
  vtkGetMacro(EchoPlanarMode,int);
  vtkBooleanMacro(EchoPlanarMode, int);

  // Description:
  // Mode -- is input echoplanar or not
  vtkSetClampMacro(ConventionalAppendMultiSliceMode,int,0,1);
  vtkGetMacro(ConventionalAppendMultiSliceMode,int);
  vtkBooleanMacro(ConventionalAppendMultiSliceMode, int);


  // Description:
  // Output Mode -- Output Polar Complex (non FFT) Or Not
  vtkSetClampMacro(PolarComplexMode,int,0,1);
  vtkGetMacro(PolarComplexMode,int);
  vtkBooleanMacro(PolarComplexMode, int);

  // Description:
  // Output Mode -- Output Polar Complex (non FFT) Or Not
  vtkSetClampMacro(ShiftSpectrum,int,0,1);
  vtkGetMacro(ShiftSpectrum,int);
  vtkBooleanMacro(ShiftSpectrum, int);


  static int ConvertConventional(vtkImageData* input,vtkImageData* output);	
  static int ConvertEchoPlanar(vtkImageData* input,vtkImageData*output);
  static int PerformInverseFourierTransform(vtkImageData* input,vtkImageData*output,int shiftspectrum);
  static int DoShiftSpectrum(vtkImageData* input,vtkImageData* output);
  static int ReOrderMultipleEchos(vtkImageData* input,vtkImageData* output);	

  // Exponential T2 Maps
  static int FillMap(vtkImageData* map,vtkImageData* density,float thr,int bimap);

  // Main Routines
  static int FitExponentialT2Model(vtkImageData* input,vtkImageData* output,float first=15.0,float dt=15.0,int mode=1,float threshold=10.0,
				   float int_threshold=0.02,float maxt2=2000.0);

  static vtkImageData* GenerateSyntheticData(float d1,float d2,float t1,float t2,float dte,int nte,float noise);

  // 3D Fourier Spectra 
  // ------------------

  static int ReadFIDSpectrumAndSpitIntoFile(char* fname,char* outname,int width,int height,int depth,int components);

  static vtkImageData* ReadFIDSpectrum(char* fname,int width,int height,int sliceno,int depth,int components);
  static vtkImageData* ProcessFIDSpectrum(char* fname,int width,int height,int sliceno,int depth,int components,float scalefactor=0.00001);
  static void ComputeSums(vtkImageData* img);
  static void FitFIDIntoGnuplot(char* fname1,char* fname2,int width,int height,int depth,int components);

  static float AutoPhaseCorrection(vtkImageData* complexinput);
  static vtkImageData* PhaseCorrection(vtkImageData* complexinput,float theta,float theta2);
  static vtkImageData* CenterPhaseData(vtkImageData* complexinput);

  static int PlotImageComponentToGnuplot(vtkImageData* image,int x,int y,char* output);

  
protected:

  vtkpxProcessRawBrukerFIDImage();
  vtkpxProcessRawBrukerFIDImage(const vtkpxProcessRawBrukerFIDImage&) {};
  void operator=(const vtkpxProcessRawBrukerFIDImage&) {};

  virtual void SimpleExecute(vtkImageData*,vtkImageData*);
  void ExecuteInformation();

  int EchoPlanarMode;
  int PolarComplexMode;
  int ShiftSpectrum;
  int ConventionalAppendMultiSliceMode;


};

#endif



