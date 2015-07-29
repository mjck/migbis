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
  Module:    $RCSfile: vtkpxGUIMosaicViewer.h,v $
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
// .NAME vtkpxGUIMosaicViewer 
// .SECTION Description

#ifndef __vtkpxGUIMosaicViewer_h
#define __vtkpxGUIMosaicViewer_h


#include "pxtkcomplexwidgets.h"
#include "pxtkdialog.h"
#include "pxvtkgadgets.h"

#include "vtkpxGUIBaseImageViewer.h"
#include "vtkpxGUIRenderer.h"
#include "vtkpxImageSlice.h"
#include "vtkpxUtil.h"
#include "vtkpxGUIBaseCurveEditor.h"



const int MVP_MAX_VIEWERS=100;

class  vtkpxGUIMosaicViewer : public vtkpxGUIBaseImageViewer
{
public:
  static vtkpxGUIMosaicViewer *New();
  vtkTypeMacro(vtkpxGUIMosaicViewer,vtkpxGUIBaseImageViewer);
  
  // Initialize Display  
  virtual char*  Initialize(const char* name,int inside);
  virtual void   Update();


  virtual void SetOffset(int off);
  virtual void SetXYSliceOnly();
  virtual void SetSingleSliceMode();
  virtual void ShowFrameControls(int show=1);

  virtual int  InitializeViewer();
  virtual int  ResetViewer();

  virtual int  UpdateAllViewers();
  virtual int  UpdateViewers(int firstv,int lastv);
  virtual int  UpdateDisplay();
  virtual int  ResetAll();

  virtual int GetBeginSlice();
  virtual int GetIncrement();

  virtual void SetCurrentFrame(int t);

  virtual char* GetRenderWidgetName();

  virtual int  HandleEvent(int eventno);
  virtual int  HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev);

  vtkGetVectorMacro(HighlightLevels,int,6);
  virtual void SetHighlightLevels(int p1,int p2,int p3,int p4,int p5,int p6);

  virtual void SetEditor(vtkpxGUIBaseCurveEditor* cont) { Editor=cont;}
  vtkSetMacro(MouseToEditor,int);
  vtkGetMacro(MouseToEditor,int);
  vtkSetMacro(ShiftMouseToEditor,int);
  vtkGetMacro(ShiftMouseToEditor,int);
  vtkGetMacro(NumberOfViewers,int);
  
  virtual vtkRenderer* GetRenderer(int i);
  virtual vtkRenderer* GetRenderer() { return GetRenderer(0);}

  virtual vtkpxGUIRenderer* GetGUIRenderer(int i);
  virtual vtkpxGUIRenderer* GetGUIRenderer() { return GetGUIRenderer(0);}

  virtual char* GetControlFrameName();

  //BTX
  virtual PXTkFrame* GetControlFrame();
  //ETX

  virtual void SetConfiguration(int row,int col);
  virtual void  SetShowControls(int show=1);

protected:

  vtkpxGUIRenderer*         renderer[MVP_MAX_VIEWERS];
  PXVTkTkRenderWidget*      render_widget;
  vtkpxImageSlice*          imageSlice[MVP_MAX_VIEWERS];
  PXBool                    initialized[MVP_MAX_VIEWERS];

  vtkpxGUIBaseCurveEditor*  Editor;
  int                       MouseToEditor;
  int                       ShiftMouseToEditor;
  int                       CreateHighlights;

  PXTkFrame*                controlFrame;
  PXTkFrame*                extraFrame;
  PXTkFrame*                timeframeControlFrame;

  PXTkLabel*                statusLabel;
  PXTkLabel*                mniLabel;



  int                       NumberOfViewers;
  int                       NumberOfRows;
  int                       NumberOfColumns;
  int                       MaximumNumberOfViewers;

  int                       offset;
  int                       donotRender;


  PXTkCheckButton*          interpolateMode;
  PXTkCheckButton*          showLabels;
  PXTkCheckButton*          reverseOrder;
  PXTkCheckButton*          transposeOrder;
  PXTkArrowScale            *viewerPresetsRow;
  PXTkArrowScale            *viewerPresetsCol;
  PXTkArrowScale            *beginSlice;
  PXTkArrowScale            *incrementSlice;
  PXTkArrowScale            *frameScale;

  PXTkDialog*               highlightGUI;
  PXTkArrowScale*           highlightscale[6];
  PXTkCheckButton*          showhighlight;
  int                       HighlightLevels[6];

  PXTkOptionMenu*           currentPlane;



  PXBool                    single_slice_mode;
  PXBool                    xy_slice_only;

  vtkpxGUIMosaicViewer();
  virtual ~vtkpxGUIMosaicViewer();
   

  virtual void SetNumberOfViewers(int numv);
  virtual void ChangeSliceIncrement(int sl=-1,int incr=-1,int fr=-1);

  virtual void SetViewports();
  virtual void CreateViewerAndSlice(int index);
  virtual int  GetViewerSlice(int v);

  virtual void InitControls(PXTkFrame* parent,int hm=1);
  virtual void InitViewer(PXTkFrame* parent);

  virtual void GetBounds(double bounds[6]);
  virtual void UpdateHighlights();

  virtual int UpdateIntensityLabelInfo() { return this->UpdateStatusLabels(); }
  virtual int UpdateStatusLabels();

  virtual int ResetEditor();

  // Flags to check for Rendering
  //BTX
  int inexpose,inexposecount,firsttime,olddimensions[3];
  int                        show_controlframe;
  //ETX
};


#endif   


