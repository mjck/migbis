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
  Module:    $RCSfile: vtkpxEpilepsyCSIReader.h,v $
  Language:  C++
  Date:      $Date: 2002/08/09 20:40:16 $
  Version:   $Revision: 1.2 $


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
// .NAME vtkpxEpilepsyCSIReader - reads analyze (or binary) 3d images 
// .SECTION Description
// This class reads analyze (.hdr,.img) 3D/4D images from files
// By manually setting the header information, it can also read binary images stored in a single file
// The output of this class is vtkStructuredPoints (either uchar,short,float)
// It can handle byteswapping automatically in the case of analyze image input
// and can read gzipped compressed files directly (automatically adds .gz suffix if needed)
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION Caveats
// This uses the pxutil library -- for analyze header 
//
// .SECTION See Also
//  vtkpxAnalyzeImageWriter vtkVolume16Reader


#ifndef __vtkpxEpilepsyCSIReader
#define __vtkpxEpilepsyCSIReader

#include "vtkStructuredPoints.h"
#include "vtkStructuredPointsReader.h"
#include "vtkFloatArray.h"


class vtkpxEpilepsyCSIReader : public vtkStructuredPointsReader
{
public:
  vtkTypeMacro(vtkpxEpilepsyCSIReader,vtkStructuredPointsReader);
  static vtkpxEpilepsyCSIReader* New();

  // Description:
  // Convenience Routine  use Load("fname.dat")
  virtual int  Load(const char* fname);

  // Description:
  vtkGetMacro(FlipZOffset,int);
  vtkSetClampMacro(FlipZOffset,int,0,1);
  vtkBooleanMacro(FlipZOffset,int);

  // Description:
  // Extra Information
  vtkGetObjectMacro(ScaleFactors,vtkFloatArray);
  vtkGetStringMacro(CategoryNames);

protected:
  //BTX
  vtkpxEpilepsyCSIReader();
  virtual ~vtkpxEpilepsyCSIReader();

  virtual void Execute();
  
  int Status;
  int FlipZOffset;  
  char* CategoryNames;
  vtkFloatArray* ScaleFactors;
  //ETX
};

#endif



