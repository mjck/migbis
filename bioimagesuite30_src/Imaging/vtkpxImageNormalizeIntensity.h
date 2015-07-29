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
  Module:    $RCSfile: vtkpxImageNormalizeIntensity.h,v $
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
// .NAME vtkpxImageNormalizeIntensity - piecewise linear intensity map
// .SECTION Description
// With vtkpxImageNormalizeIntensity Pixels Intensities are mapped using
// a piecewise linear function defined by the 4-nodes
// (InputLeveln,OutputLeveln) where n=1-4
// A value less than InputLeveln goes to OutputLevel0 and a value>InputLevel4 goes to OutputLevel4
// As a convenience, this class allows you to set the output scalar type
// similar to vtkImageCast.
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>


#ifndef __vtkpxImageNormalizeIntensity_h
#define __vtkpxImageNormalizeIntensity_h


class vtkImageToImageFilter;

#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkImageToImageFilter.h"

class vtkpxImageNormalizeIntensity : public vtkImageToImageFilter
{
public:
  static vtkpxImageNormalizeIntensity *New();
  vtkTypeMacro(vtkpxImageNormalizeIntensity,vtkImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  // Description:
  // Set/Get the InputLevel1
  vtkSetMacro(InputLevel1,float);
  vtkGetMacro(InputLevel1,float);

  // Description:
  // Set/Get the InputLevel2
  vtkSetMacro(InputLevel2,float);
  vtkGetMacro(InputLevel2,float);

  // Description:
  // Set/Get the InputLevel3
  vtkSetMacro(InputLevel3,float);
  vtkGetMacro(InputLevel3,float);

  // Description:
  // Set/Get the InputLevel4
  vtkSetMacro(InputLevel4,float);
  vtkGetMacro(InputLevel4,float);

  // Description:
  // Set/Get the OutputLevel0
  vtkSetMacro(OutputLevel0,float);
  vtkGetMacro(OutputLevel0,float);

  // Description:
  // Set/Get the OutputLevel1
  vtkSetMacro(OutputLevel1,float);
  vtkGetMacro(OutputLevel1,float);

  // Description:
  // Set/Get the OutputLevel2
  vtkSetMacro(OutputLevel2,float);
  vtkGetMacro(OutputLevel2,float);

  // Description:
  // Set/Get the OutputLevel3
  vtkSetMacro(OutputLevel3,float);
  vtkGetMacro(OutputLevel3,float);

  // Description:
  // Set/Get the OutputLevel4
  vtkSetMacro(OutputLevel4,float);
  vtkGetMacro(OutputLevel4,float);


  // Description:
  // Helper Routine for Tcl Scripts 
  static void SetInputToNull(vtkImageToImageFilter* source);
  
  
protected:
  //BTX
  vtkpxImageNormalizeIntensity();
  ~vtkpxImageNormalizeIntensity() {};
  vtkpxImageNormalizeIntensity(const vtkpxImageNormalizeIntensity&) {};
  void operator=(const vtkpxImageNormalizeIntensity&) {};



  float InputLevel1;
  float InputLevel2;
  float InputLevel3;
  float InputLevel4;

  float OutputLevel0;
  float OutputLevel1;
  float OutputLevel2;
  float OutputLevel3;
  float OutputLevel4;

  void ThreadedExecute(vtkImageData *inData, vtkImageData *outData,
		       int extent[6], int id);
  //ETX
};

#endif




