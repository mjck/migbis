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
  Module:    $RCSfile: vtkpxGUIObjectmapSplineStackControl.h,v $
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
// .NAME vtkpxGUIObjectmapSplineStackControl - Base Image Viewer
// .SECTION Description

#ifndef __vtkpxGUIObjectmapSplineStackControl_h
#define __vtkpxGUIObjectmapSplineStackControl_h

#include "vtkpxGUISplineStackControl.h"
#include "pxtkseldialog.h"
#include "pxtkwaitdialog.h"
#include "vtkpxGUI2DSplineObjectmapViewer.h"
#include "vtkpxGUIObjectmapOrthogonalViewer.h"
class vtkImageData;


class vtkpxGUIObjectmapSplineStackControl : public vtkpxGUISplineStackControl 
{
public:

  static vtkpxGUIObjectmapSplineStackControl *New();
  vtkTypeMacro(vtkpxGUIObjectmapSplineStackControl,vtkpxGUISplineStackControl);
  
  // Description:
  // Initialize Display and add Renderer 
  virtual char*  Initialize(const char* name,int inside);
  virtual char*  Initialize(const char* name,const char* name2,const char* name3,int inside);


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
  virtual void UpdateSplineEditor(int slice=-1);
  virtual void UpdateFromSplineEditor(int slice=-1);

  // Description:
  // Communitcation with Masterframe (slice/frame)
  virtual void UpdateMasterFrame();
  virtual void UpdateFromMasterFrame(int mode);
  
  // Description:
  // Set Spline Control to facilitate proper updating
  virtual void SetSplineEditor(vtkpxGUI2DSplineObjectmapViewer* splineeditor);
  virtual void SetItemColor(int index,double color[3]);
  virtual void SetItemFilename(int index,const char* s);

  // Description:
  // Set Current Spline Stack
  virtual void SetCurrentCollection(int num);

  // Description:
  // Base Viewer for communication
  vtkSetObjectMacro(BaseViewer,vtkpxGUIObjectmapOrthogonalViewer);
  vtkGetObjectMacro(BaseViewer,vtkpxGUIObjectmapOrthogonalViewer);

protected:

  vtkpxGUIObjectmapSplineStackControl();
  virtual ~vtkpxGUIObjectmapSplineStackControl();

  // Description:
  // Collection Stuff
  vtkpxGUIObjectmapOrthogonalViewer*  BaseViewer;
  int                  wireframemode[SSP_MAX_RENDERERS];

  // Description:
  // Frame Scale Widget
  PXTkOptionMenu       *currentCollection2;
  PXTkCheckButton*     autosavebutton;
  PXTkCheckButton*     backupbutton;
  PXBool               do_autosave;
  PXBool               do_backup;
  
  // Description:
  // Widgets for various GUIS
  // 1. Multi-resample
  // 2. Multi-shift+scale
  PXTkSelDialog*  optionDialog;

  // Description:
  // Initialize SplineStacks
  virtual void InitializeSplineStacks();
  virtual void ResetSplineStacks();

  // Description:
  // Implement Autosave
  virtual int DoAutosave();
  virtual int DoBackup(int stackindex,const char* rootname);


  // Description:
  // Handle Callbacks for GUI Stuff
  virtual void DoShiftScale(int mode,int moption=-1);
  virtual void DoResample(int mode,int moption=-1);
  virtual void DoExport(int mode,int moption=-1);
  virtual void DoCylinders(int mode,int moption=-1);


  // Description:
  // Handle VOI Props
  virtual void ComputeVOIProperties(int frame);

  // Description:
  // Load/Save Surface Sets
  virtual int LoadSet(const char* fname);
  virtual int SaveSet(const char* fname);


  // Description:
  vtkpxGUI2DSplineObjectmapViewer* SplineEditor;
  PXTkArrowScale            *sliceScale;
  //  PXTkArrowScale            *frameScale;
  PXTkFrame*                 masterFrame;

  char*       surface_setfilename;


};



#endif   


