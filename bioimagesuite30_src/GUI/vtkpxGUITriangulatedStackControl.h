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
  Module:    $RCSfile: vtkpxGUITriangulatedStackControl.h,v $
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
// .NAME vtkpxGUITriangulatedStackControl - Base Image Viewer
// .SECTION Description

#ifndef __vtkpxGUITriangulatedStackControl_h
#define __vtkpxGUITriangulatedStackControl_h

#include "vtkpxTriangulatedStackSource.h"
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
#include "pxtkseldialog.h"

class vtkImageData;

const int TSP_MAX_COLLECTIONS = 12; 
const int TSP_MAX_RENDERERS   = 4; 

class vtkpxGUITriangulatedStackControl : public vtkpxGUIBaseCurveEditor 
{
public:

  static vtkpxGUITriangulatedStackControl *New();
  vtkTypeMacro(vtkpxGUITriangulatedStackControl,vtkpxGUIBaseCurveEditor);
  
  // Description:
  // Set/Get NumberOfTriangulatedStacks
  virtual void SetNumberOfTriangulatedStacks(int a);
  vtkGetMacro(NumberOfTriangulatedStacks,int);

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
  // Event Manager 
  virtual int    HandleEvent(int);
  
  // Get and Set Current TriangulatedStack 
  virtual vtkpxTriangulatedStackSource* GetTriangulatedStack(int index=-1);
  virtual vtkObject* GetItem(int index) { return GetTriangulatedStack(index);}
  virtual int  GetCurrentIndex();
  virtual void SetTriangulatedStack(vtkpxTriangulatedStackSource* land,int index=-1);

  // Description:
  // Set Current Image , including slice and orientation and frame
  // Discards plane and assumes plane =2 , and discards frame
  virtual void SetImage(vtkImageData* image,int orientation,int level=0,int frame=0);
  
  // Description:
  // Callback Handlers for Clipboard Operations
  virtual void CopyTriangulatedStack();
  virtual void PasteTriangulatedStack();

protected:

  vtkpxGUITriangulatedStackControl();
  virtual ~vtkpxGUITriangulatedStackControl();

  // Description:
  // TriangulatedStacks for editing backup and pointer to current 
  vtkpxTriangulatedStackSource *currentTriangulatedStack;
  vtkpxTriangulatedStackSource *triangulatedStackSet[TSP_MAX_COLLECTIONS];
  vtkpxTriangulatedStackSource *clipboardTriangulatedStack;
 
   // Description:
  // vtkActors which capture curves/control points 
  vtkActor*         triangulatedStackActor[TSP_MAX_RENDERERS][TSP_MAX_COLLECTIONS];
  vtkRenderer*      Renderer[TSP_MAX_RENDERERS];

  // Description:
  // Index of Current TriangulatedStack and Number of TriangulatedStacks 
  int             current_triangulatedstack;
  int             NumberOfTriangulatedStacks;

  // Description:
  // Visibility 0= No 1=Points 2=Wireframe 3=Solid
  int         display_mode[TSP_MAX_COLLECTIONS];
  int         color_mode[TSP_MAX_COLLECTIONS];

  // Description:
  // Options to select whether to show/hide controls mouse mode etc
  PXTkOptionMenu   *currentCollection;

  // Description:
  // Show/Hide Current TriangulatedStack, size of current triangulated 
  PXTkOptionMenu   *displayMode;
  PXTkOptionMenu   *colorMode;
  PXTkLabelFrame   *controlFrame;

  // Description:
  // Current Image and Energy map as well as image properties
  vtkImageData    *image;
  double           spacing[3];
  double           origin[3];
  int             dimensions[3];
  int             orientation;


  // Description:
  // Widgets for various GUIS
  // 1. Curvatures
  // 2. Solid
  // 3. Shape Tracking
  PXTkSelDialog*  optionDialog;

  // Description:
  // Callback Handlers for Changing TriangulatedStack, Updating GUI and Loading TriangulatedStack
  virtual void SetCurrentCollection(int num);
  virtual void LoadTriangulatedStack(const char* fname);

  // Description:
  // Initialize TriangulatedStacks
  virtual void InitializeTriangulatedStacks();

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


