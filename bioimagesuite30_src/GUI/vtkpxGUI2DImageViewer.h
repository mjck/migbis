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
  Module:    $RCSfile: vtkpxGUI2DImageViewer.h,v $
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
// .NAME vtkpxGUI2DImageViewer 
// .SECTION Description

#ifndef __vtkpxGUI2DImageViewer_h
#define __vtkpxGUI2DImageViewer_h


#include "pxtkcomplexwidgets.h"
#include "pxtkdialog.h"
#include "pxvtkgadgets.h"

#include "vtkpxGUIBaseImageViewer.h"
#include "vtkpxGUIRenderer.h"
#include "vtkpx2DImageSlice.h"
#include "vtkpxUtil.h"
#include "vtkpxGUIBaseCurveEditor.h"



class  vtkpxGUI2DImageViewer : public vtkpxGUIBaseImageViewer
{
public:
  static vtkpxGUI2DImageViewer *New();
  vtkTypeMacro(vtkpxGUI2DImageViewer,vtkpxGUIBaseImageViewer);
  
  // Initialize Display  
  virtual char*  Initialize(const char* name,int inside);
  virtual void   Update();


  // Auto Lookup Table
  virtual void SetPresetLookupTable(int mode=0);
  
  virtual void SetOffset(int off);
  virtual void ShowFrameControls(int show=1);

  virtual int  InitializeViewer();
  virtual int  ResetViewer();

  virtual int  UpdateViewer();
  virtual int  UpdateDisplay();
  virtual int  ResetAll();

  virtual int  GetSliceNumber();
  virtual void SetCurrentFrame(int t);
  //virtual void SetCurrentSlice(int sl);

  virtual char* GetRenderWidgetName();

  virtual int  HandleEvent(int eventno);
  virtual int  HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev);

  virtual void SetEditor(vtkpxGUIBaseCurveEditor* cont) { Editor=cont;}

  vtkSetMacro(MouseToEditor,int);
  vtkGetMacro(MouseToEditor,int);
  vtkSetMacro(ShiftMouseToEditor,int);
  vtkGetMacro(ShiftMouseToEditor,int);

  virtual vtkRenderer* GetRenderer();
  virtual vtkpxGUIRenderer* GetGUIRenderer();

  virtual char* GetControlFrameName();


  //BTX
  virtual PXTkFrame* GetControlFrame();
  virtual PXTkFrame* GetTimeSliceFrame();
  //ETX

  virtual void SetMaskImage(vtkImageData* maskimage,int orientation);
  virtual vtkImageData* GetMaskImage();
  virtual int  ClearMaskImage();
  // V in 0:100
  virtual void  SetObjectmapTransparency(float v);
  virtual float GetObjectmapTransparency();

  virtual vtkLookupTable*  GetMaskLookupTable();

  // Description:
  // Rapid Access
  virtual vtkImageData* Get2DImageRegion() { return this->currentImageRegion; }
  virtual vtkImageData* Get2DImageMaskRegion()   { return this->maskImageRegion; }
  virtual vtkpx2DImageSlice* Get2DImageSlice()   { return this->maskImageSlice; }

  // Description:
  // Does this have an objectmap
  virtual int HasObjectmap() { return 1;}

protected:

  vtkpxGUIRenderer*         renderer;
  PXVTkTkRenderWidget*      render_widget;
  vtkpx2DImageSlice*        imageSlice;
  PXBool                    initialized;

  vtkpxGUIBaseCurveEditor*  Editor;
  int                       MouseToEditor;
  int                       ShiftMouseToEditor;
  int                       CreateHighlights;

  PXTkFrame*                controlFrame;
  PXTkFrame*                timeframeControlFrame;

  PXTkLabel*                statusLabel;

  int                       offset;
  int                       donotRender;

  PXTkCheckButton*          interpolateMode;
  PXTkArrowScale            *sliceNumber;
  PXTkArrowScale            *frameScale;
  vtkpxGUIColormapEditor*   ColormapEditor;


  vtkpxGUI2DImageViewer();
  virtual ~vtkpxGUI2DImageViewer();
   
  virtual void ChangeSliceAndFrame(int sl,int fr=-1);

  virtual void CreateViewerAndSlice();

  virtual void InitControls(PXTkFrame* parent,int hm=1);
  virtual void InitViewer(PXTkFrame* parent);

  virtual void GetBounds(double bounds[6]);

  // Flags to check for Rendering
  //BTX
  int inexpose,inexposecount;
  //ETX

  vtkImageData*       maskImage;
  vtkImageData*       maskImageRegion;
  vtkImageData*       currentImageRegion;

  vtkpx2DImageSlice*  maskImageSlice;
  PXTkArrowScale*     maskTransparency;
  vtkLookupTable*     maskLookupTable;

  virtual void CreateMaskImage(vtkImageData* currentImage);
  // BTX
  virtual int  UpdateUndoImages() { return 1; }
  virtual int  UpdateObjectmapChanges() { return 1; }
  // ETX
  int               LastExtractedSlice;
  int               LastExtractedOrientation;
  int               LastExtractedFrame;

  //BTX
  int firsttime;
  int olddimensions[3];
  //ETX

  virtual int UpdateIntensityLabelInfo() { return 0;}
  virtual int UpdateObjectmapSlice() { return 1;}

  
};


#endif   


