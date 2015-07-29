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
  Module:    $RCSfile: vtkpxGUISplineEditor.h,v $
  Language:  C++
  Date:      $Date: 2002/12/18 16:11:33 $
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
// .NAME vtkpxGUISplineEditor 
// .SECTION Description

#ifndef __vtkpxGUISplineEditor_h
#define __vtkpxGUISplineEditor_h

#include "vtkpxGUIMosaicViewer.h"
#include "vtkpxGUISplineControl.h"
#include "vtkpxGUIBaseCurveControl.h"
const int CSE_MAX_STACKS=50;

class  vtkpxGUISplineEditor : public vtkpxGUIMosaicViewer {
  
public:

  static vtkpxGUISplineEditor *New();
  vtkTypeMacro(vtkpxGUISplineEditor,vtkpxGUIMosaicViewer);
  
  // Description:
  // Initialize Display  
  virtual char*  Initialize(const char* name,int inside);

  // Description:
  // Handle Events
  virtual int  HandleEvent(int eventno);

  // Description:
  // Set/Get Number Of Splines 
  virtual void SetNumberOfSplines(int a);
  vtkGetMacro(NumberOfSplines,int);

  // Description:
  // Get Spline Control Editor 
  vtkGetObjectMacro(SplineControl,vtkpxGUISplineControl);

  // Set Slice 
  void SetSlice(int sl=-1);

  // Description:
  // Set Slice Increment, Frame 
  virtual void ChangeSliceIncrement(int sl=-1,int incr=-1,int fr=-1);

  // Description:
  // Enable/Disable Spline
  void EnableSpline(int i);
  void DisableSpline(int i);

  // Description:
  // Get Current Slice
  virtual int GetBeginSlice();

  // Description:
  // Automation Interface
  virtual void CreateSnake();
  virtual void UpdateSnake();
  virtual void CopyCallback();
  virtual void PasteCallback();

  // Description:
  // Keep Auto Update Off
  vtkSetMacro(ForceNoAutoUpdate,int);
  vtkGetMacro(ForceNoAutoUpdate,int);

  // Description:
  // Slave Mode i.e. not main program
  vtkSetClampMacro(SlaveMode,int,0,1);
  vtkGetMacro(SlaveMode,int);
  vtkBooleanMacro(SlaveMode,int);

  // Description:
  // Default Scale Mode
  vtkSetClampMacro(DefaultScaleMode,int,0,8);
  vtkGetMacro(DefaultScaleMode,int);


  // Description;
  // Slice Changed Callback 
  virtual void   SetSliceChangeCallback(const char* CallbackName);
  vtkGetMacro(EnableSliceChangeCallback,int);
  vtkSetClampMacro(EnableSliceChangeCallback,int,0,1);
  virtual void  ExecuteSliceChangeCallback();

  // Descrpition:
  // DisableAutoUpdateCallback() -- disables autoupdate callback for 4D editors
  vtkSetClampMacro(EnableAutoUpdateModeCallback,int,0,1);
  vtkGetMacro(EnableAutoUpdateModeCallback,int);
  vtkBooleanMacro(EnableAutoUpdateModeCallback,int);

  // Description:
  // Is AutoUpdateMode Enabled
  virtual int IsAutoUpdateModeEnabled();

protected:

  vtkpxGUISplineEditor();
  virtual ~vtkpxGUISplineEditor();

  vtkpxGUISplineControl* SplineControl;


  virtual int  ResetViewer();

  int NumberOfSplines;
  int CurrentSlice;
  int ForceNoAutoUpdate;
  int SlaveMode;
  int EnableAutoUpdateModeCallback;


  char*                   sliceChangeCallbackName; 
  int                     EnableSliceChangeCallback;
  int                     DefaultScaleMode;

};


#endif   


