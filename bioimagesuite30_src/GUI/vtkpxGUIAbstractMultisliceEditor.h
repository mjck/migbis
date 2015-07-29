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
  Module:    $RCSfile: vtkpxGUI3DObjectmapEditor.h,v $
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
// .NAME vtkpxGUI3DObjectmapEditor 
// .SECTION Description

#ifndef __vtkpxGUIAbstractMultisliceEditor_h
#define __vtkpxGUIAbstractMultisliceEditor_h


#include "vtkpxGUIComponent.h"
#include "vtkpxGUIObjectmapMosaicViewer.h"
#include "pxtkradiobutton.h"
#include <string>

class vtkpxDataArrayUndoStack;


const int pxAME_NUM_COLOR_BUTTONS=9;
const int pxAME_NUM_DIALOG_COLOR_BUTTONS=60;


class  vtkpxGUIAbstractMultisliceEditor : public vtkpxGUIComponent {
  
public:

  vtkTypeMacro(vtkpxGUIAbstractMultisliceEditor,vtkpxGUIComponent);
  
  // Description:
  // Initialize Display  
  virtual char*  Initialize(const char* name,int inside);

  // Description:
  // Handle Events
  virtual int  HandleEvent(int eventno);

  // Description:
  // Handle Clicked Point
  virtual int HandleClickedPoint(int x,int y,int z,int nbutton,int state);

  // Description:
  // Default Scale Mode
  vtkSetClampMacro(DefaultScaleMode,int,0,8);
  vtkGetMacro(DefaultScaleMode,int);

  // Description:
  // Set Images 
  virtual int SetImagesAndLookupTable(vtkImageData* mainImage,vtkImageData* objectmap,vtkLookupTable* lkp,int iscolor);

  // Description:
  // Set objectmap and create undo 
  virtual double SetObjectmapWithUndo(vtkImageData* objectmap);

  // Description:
  // Create Default Lookup Table
  virtual int CreateDefaultLookupTable(int voimode=0);

  // Description:
  // Update From Viewer
  virtual void UpdateFromViewer(int docmap=0) = 0;

  // Description:
  // Simple Mode
  vtkSetClampMacro(SimpleMode,int,0,1);

  // Description:
  // Set 2D Axis
  vtkSetClampMacro(Paint2DAxis,int,0,2);

  // Description:
  virtual int GetPaintConnectMode();
  virtual int GetPaintUseThresholdMode();
  virtual int GetPaintUseMaskMode();
  virtual void GetThresholds(float val[2]);
  virtual int GetCurrentColor();
  virtual int GetPaintMode();
  virtual int GetPaint3DMode();

  // Description:
  // Get Extra Frame
  virtual PXTkFrame* GetExtraFrame();
  virtual char* GetExtraFrameName();
  virtual int      UpdateColorButtons();
  virtual void     RenamePaintOffLabel(const char* newname);


  // Description:
  // Set VOI Select Mode
  virtual int SetVOISelectMode(double minthr,double maxthr,int force3dbrush=1);

  // Description:
  // Some Undo Stuff
  virtual void ChangeObjectmapValue(vtkDataArray* element,int i,int j,int k,int new_color);
  virtual vtkDataArray*  GetNextArray();


  // Description:
  // Disable Painting
  virtual int DisablePainting();
  virtual int EnablePainting();


  // Description:
  // VOI Names
  virtual int SetVOIName(int index,char* name);
  virtual const char* GetVOIName(int index);
  virtual int GetNumberOfVOIS();
  virtual void ResetVOINames();
  
protected:

  vtkpxGUIAbstractMultisliceEditor();
  virtual ~vtkpxGUIAbstractMultisliceEditor();
  
  // Description:
  // Issues to do with painting and controls
  char*            objectmapcolormapname;
  int              current_fill_color;
  virtual int      CreateFillControl(PXTkFrame* fillframe);
  virtual int      UpdateObjectmapColormap();
  virtual int      LoadObjectmapColormap();
  virtual int      SaveObjectmapColormap();
  virtual int      EditColorButton(int n);

  // Description:
  // Viewer Communications
  virtual void UpdateViewerDisplay() = 0;
  virtual void UpdateViewerObjectmapColormap() = 0;
  virtual void UpdateViewerObjectmap() = 0;


  // Description:
  // Undo/Redo Operation
  virtual void UndoLastObjectmapChange();
  virtual void RedoLastObjectmapChange();
  virtual void UpdateUndoIndicesDisplay();


  // Description:
  // Smooth Objectmap
  virtual int SmoothObjectmap();


  // Description:
  // Data Members
  PXTkRadioButton*  colorButton[pxAME_NUM_COLOR_BUTTONS];
  PXTkRadioButton*  dialogColorButton[pxAME_NUM_DIALOG_COLOR_BUTTONS];
  //BTX
  std::string       voiNames[pxAME_NUM_DIALOG_COLOR_BUTTONS];
  //ETX
  PXTkDialog*       colorButtonDialog;
  PXTkFrame*        editcolorframe;
  PXTkFrame*        extraFrame;
  PXTkEntry*        fillThresholds[2];
  PXTkEntry*        voiNameEntry;
  PXTkOptionMenu* colorSelection;
  PXTkCheckButton*  fillMode;
  PXTkCheckButton*  paintMode3D;
  PXTkCheckButton*  connectFillMode;
  PXTkCheckButton*  baseImageMode;
  PXTkOptionMenu*   paintMode;
  PXTkOptionMenu*   smoothMode;
  PXTkOptionMenu*   smoothConvergenceMode;
  int               draw_mode;
  int               old_draw_mode;
  int               draw_mode3D;
  int               DefaultScaleMode;
  int               SimpleMode;
  int               SingleSliceMode;
  int               Paint2DAxis;
  int               CurrentImageIsColor;
  vtkImageData*     MainImage;
  vtkImageData*     Objectmap;
  vtkLookupTable*   ObjectmapLookupTable;

  //  PXTkEntry*        undoIndices[3];

  vtkpxDataArrayUndoStack* UndoStack;


};


#endif   


