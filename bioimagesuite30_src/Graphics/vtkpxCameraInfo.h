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
  Module:    $RCSfile: vtkpxCameraInfo.h,v $
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
// .NAME vtkpxCameraInfo - 
// .SECTION Description
// <br> <br> <EM>Copyright (c) 1995-2001 Xenios Papademetris <a href="mailto:papad@noodle.med.yale.edu">papad@noodle.med.yale.edu</a></EM>
// .SECTION Caveats


#ifndef __vtkpxCameraInfo_h
#define __vtkpxCameraInfo_h

#include "vtkObject.h"
#include "vtkCamera.h"
#include "vtkRenderer.h"
#include "pxutil.h"
#include "vtkDoubleArray.h"

class vtkpxCameraInfo : public vtkObject
{
public:
  static vtkpxCameraInfo *New();
  vtkTypeMacro(vtkpxCameraInfo,vtkObject);
  
  // Description:
  // Number of cameras
  virtual int GetNumberOfCameras();

  // Description:
  // Interact with Camera and/or Renderer
  virtual int UpdateCamera(vtkCamera* camera,int index=0);
  virtual int UpdateRenderer(vtkRenderer* ren,int index=0);

  // Description:
  // Store camera info in this
  virtual int CopyCamera(vtkCamera* camera,int index=-1);
  virtual int CopyRenderer(vtkRenderer* ren,int index=-1);

// Description:
  // Store camera info in this
  virtual int InsertCamera(vtkCamera* camera) { return this->CopyCamera(camera,-1); }
  virtual int InsertRenderer(vtkRenderer* ren,int index=-1) { return this->CopyRenderer(ren,-1); }
  

  // Description:
  // Load/Save Information
  virtual int Load(const char* fname);
  virtual int Load(gzFile fin);

  virtual int Save(const char* fname);
  virtual int Save(FILE*  fout);

protected:
  vtkpxCameraInfo();
  virtual ~vtkpxCameraInfo();

  // Description:
  // Variables --- there are 13 components --> a Double Array should do it
  vtkDoubleArray* CameraParameters;
  double          params[13];
  /*  double FocalPoint[3];
  double Position[3];
  double ViewUp[3];
  double ClippingRange[2];
  int    ParallelProjection;
  double ParallelScale;*/
  
};
#endif

