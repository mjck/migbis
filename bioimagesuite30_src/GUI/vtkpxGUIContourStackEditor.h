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
  Module:    $RCSfile: vtkpxGUIContourStackEditor.h,v $
  Language:  C++
  Date:      $Date: 2002/07/16 18:54:26 $
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
// .NAME vtkpxGUIContourStackEditor 
// .SECTION Description

#ifndef __vtkpxGUIContourStackEditor_h
#define __vtkpxGUIContourStackEditor_h

#include "vtkpxGUI2DImageViewer.h"
#include "vtkpxGUIBaseCurveControl.h"
const int CSE_MAX_STACKS=5;
//BTX
class PXContourStack;
//ETX


class  vtkpxGUIContourStackEditor : public vtkpxGUI2DImageViewer {
  
public:

  static vtkpxGUIContourStackEditor *New();
  vtkTypeMacro(vtkpxGUIContourStackEditor,vtkpxGUI2DImageViewer);
  
  // Initialize Display  
  virtual char*  Initialize(const char* name,int inside);
  //  virtual void   Update();
  
  virtual int  ResetViewer();
  virtual int  HandleEvent(int eventno);

  void Operate(int mode);
  void GetSurfaceFittingParameters();
  int  FitAndSaveBSplineSurface(const char* filename);

  int  Interpolate(int begin,int end);
  int  Snake(int begin,int end,int mode);
  int  Extract(int begin,int end);
  int  CopySlice(int source,int target);
  void UpdateFromEditor(PXBool currentonly=PXFalse);


  virtual void SetCurveControl(vtkpxGUIBaseCurveControl* cont);
  virtual void InitMenus(const char* parentname);
  virtual void ExportToPolyData(vtkPolyData* poly,int stack);

  virtual int CreateObjectMap(vtkImageData* img,int begin,int end);


protected:

  vtkpxGUIContourStackEditor();
  virtual ~vtkpxGUIContourStackEditor();

  vtkpxGUIBaseCurveControl* CurveEditor;

  PXContourStack* surfaceStack[CSE_MAX_STACKS];
  //BTX
  PXBool owns_surfaces,allow_multi_save,auto_save_mode;
  int num_contour_stacks,last_slice;
  PXTkDialog* multisliceGUI;
  PXTkDialog* exportAsSurfaceGUI;

  PXTkMenu*       optionMenu;
  PXTkOptionMenu* multisliceSelect;
  PXTkArrowScale  *beginMultislice,*endMultislice;
  PXTkOptionMenu*  surfaceSmoothness;
  //ETX

  virtual void ChangeSliceAndFrame(int sl=-1,int fr=-1);
  virtual void CreateThresholdStats();


  // Description:
  // Export as B-Spline
  

};


#endif   


