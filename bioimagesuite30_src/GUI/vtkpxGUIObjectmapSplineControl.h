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
  Module:    $RCSfile: vtkpxGUIObjectmapSplineControl.h,v $
  Language:  C++
  Date:      $Date: 2002/12/18 16:11:29 $
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
// .NAME vtkpxGUIObjectmapSplineControl - Base Image Viewer
// .SECTION Description

#ifndef __vtkpxGUIObjectmapSplineControl_h
#define __vtkpxGUIObjectmapSplineControl_h

#include "vtkpxSplineSource.h"
#include "vtkpxBaseCurve.h"
#include "vtkpxGUIBaseCurveEditor.h"
#include "vtkActor.h"
#include "pxtkapp.h"
#include "pxtkeventobj.h"
#include "pxtkgadgets.h"
#include "pxtkcomplexwidgets.h"
#include "pxtkdialog.h"
#include "pxtkmenu.h"
#include "pxcontour.h"
#include "vtkpxGUISplineControl.h"

class vtkpxGUIObjectmapEditor;

//const int SP_MAX_COLLECTIONS = 2; 

class vtkpxGUIObjectmapSplineControl : public vtkpxGUIBaseCurveEditor 
{
public:

  static vtkpxGUIObjectmapSplineControl *New();
  vtkTypeMacro(vtkpxGUIObjectmapSplineControl,vtkpxGUIBaseCurveEditor);
  

  // Description:
  // Set/Get Number Of Splines 
  virtual void SetNumberOfSplines(int a);
  vtkGetMacro(NumberOfSplines,int);

  // Description:
  // Initialize Display and add Renderer 
  virtual char*  Initialize(const char* name,int inside);
  virtual void   SetRenderer(vtkRenderer *rendererer,int num);
  virtual void   SetRenderer(vtkRenderer *rendererer);

  // Description:
  // Update Display
  virtual void   Update();
  virtual void   UpdateDisplay();

  // Description:
  // Event Manager 
  virtual int    HandleEvent(int);
  
  // Description:
  // Update status label with info on current spline 
  virtual void    UpdateStatus();

  // Description:
  // Handle mouse point 
  virtual int     HandleClickedPoint(float x,float y,float z,int scaled=1,int state=0);

  // Description:
  // Get and Set Current Spline 
  virtual vtkpxSplineSource* GetSpline(int index=-1);
  virtual vtkObject* GetItem(int index) { return GetSpline(index);}
  virtual int  GetCurrentIndex();
  virtual void SetSpline(vtkpxSplineSource* land,int index=-1);
  virtual int  GetCurrentSliceNumber();
  virtual void SetItemColor(int index,double color[3]);
  virtual void SetItemFilename(int index,const char* s);

  // Description:
  // Set Current Image , including slice and orientation
  // Discards plane and assumes plane =2 
  virtual void SetImage(vtkImageData* image,int plane=2,int level=0,int frame=0);
  virtual void SetPlaneLevel(int plane,int level);
  virtual void SetSpacingOrigin(double sp[3],double ori[3]);
  virtual void SetLevel(int level);
  
  // Description:
  // Snake Operations 
  virtual int  DoSnake(int apply);


  // Description:
  // Callback Handlers for Clipboard Operations
  virtual void UpdateControlsFromCurrentSpline();
  virtual void FitSplineToCurrentControls();

  //BTX
  PXTkFrame* getBottomButtonFrame() { return bottomButtonFrame;}
  //ETX


  // Description:
  // Enable/Disable Spline
  virtual void EnableSpline(int i);
  virtual void DisableSpline(int i);

  // Description:
  // Get Auto Update Mode
  vtkGetMacro(AutoUpdateMode,int);

  // Description:
  // Clean Changed Status
  virtual void SetChangedStatus(int index,int val);
  virtual int  GetChangedStatus(int index);

  // Description:
  // Slave Mode i.e. not main program
  vtkSetClampMacro(SlaveMode,int,0,1);
  vtkGetMacro(SlaveMode,int);
  vtkBooleanMacro(SlaveMode,int);
  
  // Description:
  // Default Smoothing 
  vtkSetClampMacro(DefaultSmoothing,double,0.0001,10.0);
  vtkGetMacro(DefaultSmoothing,double);

  // Description:
  // ObjectMap Editor 
  virtual void SetObjectmapEditor(vtkpxGUIObjectmapEditor* ed);
 

protected:

  vtkpxGUIObjectmapSplineControl();
  virtual ~vtkpxGUIObjectmapSplineControl();

  // Description:
  // Splines for editing backup and pointer to current 
  vtkpxSplineSource *currentSpline;
  vtkpxSplineSource *splineSet[SP_MAX_COLLECTIONS];
  vtkRenderer* Renderer;

  // Description:
  // Parent Object
  vtkpxGUIObjectmapEditor* ObjectmapEditor;

  // Description:
  // Curve to display control points 
  vtkpxBaseCurve    *editCurve;
  vtkpxBaseCurve    *manualEditCurve;
  


  // Description:
  // vtkActors which capture curves/control points 
  vtkActor*         splineActor[SP_MAX_COLLECTIONS];
  vtkActor*         editActor;
  vtkActor*         manualEditActor;

  // Description:
  // Gadgets/Widgets for Snakes
  PXTkDialog*       snakeGUI;
  PXTkArrowScale* snakeEdgeWeight;
  PXTkArrowScale* snakeSmoothWeight;
  PXTkArrowScale* snakeStep;
  PXTkArrowScale* snakeSigma;
  PXTkArrowScale* snakeIterations;


  // Description:
  // Gadgets/Widgets for Snakes
  PXTkDialog*       resampleGUI;
  PXTkArrowScale*   resampleNumberOfPoints;
  PXTkArrowScale*   resampleSmoothing;
  PXTkArrowScale*   resampleDs;
  PXTkArrowScale*   resamplePreSmooth;
  PXTkArrowScale*   resamplePreSmoothIterations;


  // Description:
  // Index of Current Spline and Number of Splines 
  int             current_spline;
  int             NumberOfSplines;

  // Description:
  // Visibility and Size (in units 0f 0.25 pixels) of all splines 
  PXBool          display_mode[SP_MAX_COLLECTIONS];
  int             scale_mode[SP_MAX_COLLECTIONS];
  int             spline_enabled[SP_MAX_COLLECTIONS];
  int             manual_add_mode;
  int             SlaveMode;
  float           manual_zcoord;

  // Description:
  // Status Label
  PXTkLabel*       message;

  // Description:
  // Options to select whether to show/hide controls mouse mode etc
  PXTkCheckButton*  showControls;
  PXTkCheckButton*  mouseMode;
  PXTkCheckButton*  editMode;
  PXTkCheckButton*  autoUpdate;
  PXTkOptionMenu   *currentCollection;

  // Description:
  // Show/Hide Current Spline, size of current spline 
  PXTkCheckButton  *displayMode;
  PXTkOptionMenu   *scaleMode;
  PXTkFrame        *bottomButtonFrame;
  PXTkEntry        *thresholdEntry[2];
  PXTkEntry        *SmoothingEntry;


  // Description:
  // Current Image and Energy map as well as image properties
  vtkImageData    *image;
  int             image_level;
  int             image_frame;
  double          image_range[2];
  double          spacing[3];
  double          origin[3];
  double          DefaultSmoothing;
  int             dimensions[3];
  double           slice_offset;
  int             AutoUpdateMode;
  float           DistanceThreshold;
  float           ControlPointScaleFactor;
  int             changed[SP_MAX_COLLECTIONS];

  // Description:
  // Callback Handlers for Changing Spline, Updating GUI and Loading Spline
  virtual void SetCurrentCollection(int ) { return; }


  // Description:
  // Initialize Splines
  virtual void InitializeSplines();


  // Description:
  // Following Needed to satisfy undefined calls
  virtual void SetFrame(int) { } 
  virtual void SetLookupTable (vtkLookupTable *) { }
  virtual void SetTransform(vtkAbstractTransform *) { } 
  virtual vtkAbstractTransform* GetTransform () { return NULL;}

};



#endif   


