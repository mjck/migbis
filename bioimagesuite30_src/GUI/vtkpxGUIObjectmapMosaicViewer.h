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
  Module:    $RCSfile: vtkpxGUIObjectmapMosaicViewer.h,v $
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
// .NAME vtkpxGUIObjectmapMosaicViewer 
// .SECTION Description

#ifndef __vtkpxGUIObjectmapMosaicViewer_h
#define __vtkpxGUIObjectmapMosaicViewer_h


#include "pxtkcomplexwidgets.h"
#include "pxtkdialog.h"
#include "pxvtkgadgets.h"

#include "vtkpxGUIMosaicViewer.h"
#include "vtkpxGUIRenderer.h"
#include "vtkpxImageSlice.h"
#include "vtkpxUtil.h"

class vtkpxGUIObjectmapMosaicEditor;


class  vtkpxGUIObjectmapMosaicViewer : public vtkpxGUIMosaicViewer
{
public:
  static vtkpxGUIObjectmapMosaicViewer *New();
  vtkTypeMacro(vtkpxGUIObjectmapMosaicViewer,vtkpxGUIMosaicViewer);
  

  virtual int  ResetViewer();
  virtual int  UpdateViewers(int firstv,int lastv);
  virtual int  HandleEvent(int eventno);

  // Description:
  // Set Objectmap Transparency 
  virtual float GetObjectmapTransparency();
  virtual void  SetObjectmapTransparency(float v);
  virtual void  UpdateObjectmap();
  virtual void  QuickUpdateObjectmap();

  // Description:
  // Set Objectmap  Image
  virtual int SetObjectMapImage(vtkImageData* img);
  virtual int SetObjectLookupTable(vtkLookupTable* lkp);
  virtual int SetObjectLookupTable(vtkLookupTable* lkp,int updateeditor);
  virtual vtkImageData* GetObjectMapImage() { return this->maskImage;}
  virtual vtkLookupTable* GetObjectMapLookupTable() { return this->maskLookupTable;}


  // Description:
  // Clean Objectmap
  virtual void ClearMaskImage();

  // Description:
  virtual void SetCreateInternalEditorOn();
  vtkGetObjectMacro(InternalEditor,vtkpxGUIObjectmapMosaicEditor);

  // Description::
  virtual int  HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev);

  // Description:
  // Does this have an objectmap
  virtual int HasObjectmap() { return 1;}
  virtual int PaintingEnabled();

protected:

  vtkpxGUIObjectmapMosaicViewer();
  virtual ~vtkpxGUIObjectmapMosaicViewer();

  // Create Viewer
  virtual void CreateViewerAndSlice(int index);

  // Create Controls
  virtual void InitControls(PXTkFrame* parent,int hm=1);

  // Description:
  // If we have no Editor
  virtual int AutoCreateMaskAndLookupTable();


  vtkImageData*              maskImage;
  vtkLookupTable*            maskLookupTable;
  vtkLookupTable*            maskVolumeLookupTable;
  vtkpxImageSlice*           maskImageSlice[MVP_MAX_VIEWERS];
  PXTkArrowScale             *maskTransparency;

  // Description:
  // Editor;
  int                            create_internal_editor;
  vtkpxGUIObjectmapMosaicEditor* InternalEditor;

  // Description:
  // Objectmap Value
  //BTX
  virtual std::string GetObjectmapValueAsText(int a=0,int b=0,int c=0);
  //ETX
};


#endif   


