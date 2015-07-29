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
  Module:    $RCSfile: vtkpxGUISplineStackControl.h,v $
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
// .NAME vtkpxGUISplineStackControl - Base Image Viewer
// .SECTION Description

#ifndef __vtkpxGUISplineStackControl_h
#define __vtkpxGUISplineStackControl_h

#include "vtkpxSplineStackSource.h"
#include "vtkpxBaseCurve.h"
#include "vtkpxGUIBaseCurveEditor.h"
#include "vtkpxGUISplineEditor.h"
#include "vtkActor.h"
#include "pxtkapp.h"
#include "pxtkeventobj.h"
#include "pxtkgadgets.h"
#include "pxtkcomplexwidgets.h"
#include "pxtkdialog.h"
#include "pxtkmenu.h"
#include "pxcontour.h"
#include "pxtkseldialog.h"

class vtkImageData;

const int SSP_MAX_COLLECTIONS = 50; 
const int SSP_MAX_RENDERERS   = 4; 

class vtkpxGUISplineStackControl : public vtkpxGUIBaseCurveEditor 
{
public:

  static vtkpxGUISplineStackControl *New();
  vtkTypeMacro(vtkpxGUISplineStackControl,vtkpxGUIBaseCurveEditor);
  
  // Description:
  // Set/Get NumberOfSplineStacks
  virtual void SetNumberOfSplineStacks(int a);
  vtkGetMacro(NumberOfSplineStacks,int);

  // Description:
  // Initialize Display and add Renderer 
  virtual char*  Initialize(const char* name,int inside);
  virtual void   SetRenderer(vtkRenderer *rendererer,int num);
  virtual void   SetRenderer(vtkRenderer *rendererer,int num,int wireframe);

  // Description:
  // Update Display
  virtual void   Update();
  virtual void   UpdateDisplay();

  // Description:
  // If outside process modified splines (using GetSplineStack) this functions needs to
  // be called to re-synchronize the GUI etc
  virtual void UpdateAfterExternalEditing();


  // Description:
  // Event Manager 
  virtual int    HandleEvent(int);
  virtual int    HandleEventSecond(int);
  

  // Get and Set Current SplineStack 
  virtual vtkpxSplineStackSource* GetSplineStack(int index=-1);
  virtual vtkObject* GetItem(int index) { return GetSplineStack(index);}
  virtual int  GetCurrentIndex();
  virtual void SetSplineStack(vtkpxSplineStackSource* land,int index=-1);

  // Description:
  // Set Current Image , including slice and orientation and frame
  // Discards plane and assumes plane =2 , and discards frame
  virtual void SetImage(vtkImageData* image,int orientation,int level=0,int frame=0);
  
  // Description:
  // Callback Handlers for Clipboard Operations
  virtual void CopySplineStack();
  virtual void PasteSplineStack();

  // Description:
  // Set Spline Control to facilitate proper updating
  virtual void SetSplineEditor(vtkpxGUISplineEditor* splineeditor);

  // Description:
  // Communication with SplineEditor
  virtual void UpdateSplineEditor();
  virtual void UpdateFromSplineEditor();

protected:

  vtkpxGUISplineStackControl();
  virtual ~vtkpxGUISplineStackControl();

  // Description:
  // SplineStacks for editing backup and pointer to current 
  vtkpxSplineStackSource *currentSplineStack;
  vtkpxSplineStackSource *splineStackSet[SSP_MAX_COLLECTIONS];
  vtkpxSplineStackSource *clipboardSplineStack;
 

  // Description:
  // Spline Control used for Editing Splines
  vtkpxGUISplineEditor* SplineEditor;

   // Description:
  // vtkActors which capture curves/control points 
  vtkActor*         splineStackActor[SSP_MAX_RENDERERS][SSP_MAX_COLLECTIONS];
  vtkRenderer*      Renderer[SSP_MAX_RENDERERS];

  // Description:
  // Index of Current SplineStack and Number of SplineStacks 
  int             current_splinestack;
  int             NumberOfSplineStacks;
  int             BlockEditorCallback;

  // Description:
  // Visibility 0= No 1=Spline Only 2=Surface
  int         display_mode[SSP_MAX_COLLECTIONS];
  char*       surface_filename[SSP_MAX_COLLECTIONS];
  int         surface_modified[SSP_MAX_COLLECTIONS];

  // Description:
  // Options to select whether to show/hide controls mouse mode etc
  PXTkOptionMenu   *currentCollection;

  // Description:
  // Show/Hide Current SplineStack, size of current spline 
  PXTkOptionMenu   *displayMode;
  PXTkOptionMenu   *drawMode;
  PXTkOptionMenu   *scaleMode;
  PXTkArrowScale   *trimScale[2];
  PXTkFrame        *controlFrame;
  PXTkEntry        *thresholdEntry[2];

  // Descrtiption:
  // Needed by The Import From Image GUI
  PXTkSelDialog*  optionDialog;
  

  // Description:
  // Current Image and Energy map as well as image properties
  vtkImageData    *image;
  double           spacing[3];
  double           origin[3];
  double           defaultsmoothing;
  int             dimensions[3];
  int             orientation;
  int             CurrentFrame;

  // Description:
  // Callback Handlers for Changing SplineStack, Updating GUI and Loading SplineStack
  virtual void SetCurrentCollection(int num);
  virtual void UpdateControlsFromCurrentSplineStack();
  virtual void LoadSplineStack(const char* fname);
  virtual void SaveSplineStack(const char* fname);
  virtual void SetItemColor(int index,double color[3]);
  virtual void SetItemFilename(int index,const char* s);

  // Description:
  // Initialize SplineStacks
  virtual void InitializeSplineStacks();

  // Description:
  // VOI Property Stuff
  virtual void ComputeVOIProperties(int frame=0);

  // Description:
  // Import From Image
  virtual void DoImport(int mode,int moption);


  // Description:
  // Following Needed to satisfy undefined calls
  virtual int  HandleClickedPoint (float, float, float, int , int ) { return TCL_OK;}
  virtual void SetSpacingOrigin (double sp[3], double ori[3]) { }
  virtual void SetPlaneLevel(int, int) { }
  virtual void SetFrame(int) { } 
  virtual void SetLookupTable (vtkLookupTable *) { }
  virtual void SetTransform(vtkAbstractTransform *) { } 
  virtual vtkAbstractTransform* GetTransform () { return NULL;}



};


#endif   



