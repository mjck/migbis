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
  Module:    $RCSfile: vtkpxImageReplaceSlice.h,v $
  Language:  C++
  Date:      $Date: 2003/05/13 15:37:02 $
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
// .NAME vtkpxImageReplaceSlice - extracts slices from 3D Images
// .SECTION Description
// ReplaceSlices single 2D slice from 3D Images determined by slice number, CurrentPlane and flip 
// CurrentPlane = 0=YZ, 1=XZ, 2=XY
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION See Also
//  vtkReplaceSliceVOI

#ifndef __vtkpxImageReplaceSlice_h
#define __vtkpxImageReplaceSlice_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"

class vtkpxImageReplaceSlice : public vtkpxSimpleImageToImageFilter
{
public:
  vtkTypeMacro(vtkpxImageReplaceSlice,vtkpxSimpleImageToImageFilter);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  // Description:
  // Construct object to extract all of the input data.
  
  static vtkpxImageReplaceSlice *New();
  
  // Description:
  // CurrentPlane  0=YZ, 1=XZ, 2=XY slice=sliceno
  void SetCurrentPlane(int);
  int  GetCurrentPlane();
  
  // Description:
  // Set Slice Number 0--Dim-1
  vtkSetMacro(SliceNo,int);
  vtkGetMacro(SliceNo,int);
  
  // Description:
  // Use this to set sliceno,numslices and currentplane
  // for extracting frames
  vtkSetMacro(Frame,int);
  vtkGetMacro(Frame,int);
  
  // Description:
  // Slice To Insert
  vtkSetObjectMacro(InsertedSlice,vtkImageData);
  vtkGetObjectMacro(InsertedSlice,vtkImageData);

  // Description:
  vtkSetClampMacro(DeepCopyMode,int,0,1);
  vtkGetMacro(DeepCopyMode,int);
  

protected:
  //BTX
  vtkpxImageReplaceSlice();
  ~vtkpxImageReplaceSlice() {};
  vtkpxImageReplaceSlice(const vtkpxImageReplaceSlice&) {};
  void operator=(const vtkpxImageReplaceSlice&) {};

  virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);

  int InternalCurrentPlane;
  int SliceNo;
  int Frame;
  int DeepCopyMode;
  vtkImageData* InsertedSlice;

  //ETX

  vtkSetClampMacro(InternalCurrentPlane,int,0,2);
  vtkGetMacro(InternalCurrentPlane,int);


};

#endif



