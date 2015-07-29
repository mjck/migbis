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
  Module:    $RCSfile: vtkpxGUI4DSplineStackControl.h,v $
  Language:  C++
  Date:      $Date: 2002/04/09 15:46:27 $
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
// .NAME vtkpxGUI4DSplineStackControl - Base Image Viewer
// .SECTION Description

#ifndef __vtkpxGUI4DSplineStackControl_h
#define __vtkpxGUI4DSplineStackControl_h

#include "vtkpxGUISplineStackControl.h"
#include "pxtkseldialog.h"
#include "pxtkwaitdialog.h"

class vtkImageData;
class vtkpxGUI4DRenderer;
class vtkCollection;
class vtkActorCollection;

class vtkpxGUI4DSplineStackControl : public vtkpxGUISplineStackControl 
{
public:

  static vtkpxGUI4DSplineStackControl *New();
  vtkTypeMacro(vtkpxGUI4DSplineStackControl,vtkpxGUISplineStackControl);
  
  // Description:
  // Initialize Display and add Renderer 
  virtual char*  Initialize(const char* name,int inside);
  virtual char*  Initialize(const char* name,const char* name2,const char* name3,int inside);

  // Description:
  // Set Renderer
  virtual void   SetRenderer(vtkpxGUI4DRenderer *rendererer,int num);
  virtual void   SetRenderer(vtkpxGUI4DRenderer *rendererer,int num,int wireframe);

  // Description:
  // Set/Get Base Viewer
  vtkSetObjectMacro(BaseViewer,vtkpxGUIBaseImageViewer);
  vtkGetObjectMacro(BaseViewer,vtkpxGUIBaseImageViewer);

  // Description:
  // Update Display
  virtual void   Update();
  virtual void   UpdateDisplay();


  // Description:
  // Set Image Level,Plane And Frame 
  virtual void SetImage(vtkImageData* image,int orientation,int level=0,int frame=0);
  virtual void SetImage(vtkImageData* image);

  // Description:
  // Event Manager 
  virtual int    HandleEvent(int);

  // Description:
  // Communication with SplineEditor
  // Add Frame Checking
  virtual void UpdateSplineEditor();
  virtual void UpdateFromSplineEditor();


  // Description:
  // Get Spline Stack from Collection
  virtual vtkpxSplineStackSource* GetSplineStack(int frame,int index);

  // Description:
  // If outside process modified splines (using GetSplineStack) this functions needs to
  // be called to re-synchronize the GUI etc
  virtual void UpdateAfterExternalEditing();

  // Description:
  // Enable Change Of Frame from this control -- bad idea 
  //virtual void EnableFrameScale(int enable);

  // Description:
  // Frame Stuff
  virtual void SetCurrentFrame(int fr);
  virtual int  GetCurrentFrame();
  virtual int  GetNumberOfFrames();

  // Description:
  // Load / Save
  virtual int  MultiLoad(const char* fname);
  virtual int  MultiSave(const char* fname);

protected:

  vtkpxGUI4DSplineStackControl();
  virtual ~vtkpxGUI4DSplineStackControl();

  // Description:
  // Disable these two functions insist on multirenderer 
  virtual void   SetRenderer(vtkRenderer *rendererer,int num);
  virtual void   SetRenderer(vtkRenderer *rendererer,int num,int wireframe);


  // Description:
  // Collection Stuff
  vtkCollection*       splineStackCollection;
  vtkActorCollection*  splineStackActorCollection;
  vtkpxGUI4DRenderer*  MultiRenderer[SSP_MAX_RENDERERS];
  vtkpxGUIBaseImageViewer*  BaseViewer;
  int                  wireframemode[SSP_MAX_RENDERERS];

  // Description:
  // Frame Scale Widget
  //PXTkArrowScale*      frameScale;
  PXTkOptionMenu       *currentCollection2;
  PXTkCheckButton*     autosavebutton;
  PXTkCheckButton*     backupbutton;
  PXBool               do_autosave;
  PXBool               do_backup;
  
  // Description:
  // Widgets for various GUIS
  // 1. Multi-resample
  // 2. Multi-shift+scale



  // Description:
  // Frame Stuff
  int   NumberOfFrames;
  int   ActualNumberOfFrames;


  // Description:
  // Initialize SplineStacks
  virtual void InitializeSplineStacks();
  virtual void ResetSplineStacks();

  // Description:
  // Frame Update Stuff 
  virtual void CreateFrames(int numframes);
  virtual void UpdateFrame(int frame);
  virtual void ChangeFrame(int newframe);
  virtual void SetCurrentCollection(int num);


  // Description:
  // Implement Autosave
  virtual int DoAutosave(int frame);
  virtual int DoBackup(int stackindex,const char* rootname);


  // Description:
  // Handle Callbacks for GUI Stuff
  virtual void DoShiftScale(int mode,int moption=-1);
  virtual void DoResample(int mode,int moption=-1);
  virtual void DoVolume(int mode,int moption=-1);
  virtual void DoCylinders(int mode,int moption=-1);
  virtual void DoSmoothApex(int mode,int moption=-1);
  virtual void DoExport(int mode,int moption=-1);

  // Description:
  // Handle VOI Props
  virtual void ComputeVOIProperties(int frame);

  //BTX
  // -------------------------------------------------------------------------
  // Segmentation Stuff
  // -------------------------------------------------------------------------
  PXTkSelDialog*  optionDialog;
  PXTkWaitDialog           *wait_dialog;
  PXTkCheckButton          *toggle_up,*toggle_down,*toggle_past,*toggle_future;
  PXTkArrowScale           *frameScale[2],*sliceScale[2];
  int                      AbortProcessing,AutoSegmentMode;

  virtual void CreateSegmentationControl(PXTkFrame* segmentFrame);

  virtual void AutoSegment();
  virtual int AutoSegmentWork();
  static  void AutoSegmentStaticWork(void*  clientdata);
  
  virtual void AutoTemporalVolumeSegment(int mode);



  virtual int  InterpolateTemporally(int allslices);
  virtual int  InterpolateSpatially();

  //ETX


};



#endif   


