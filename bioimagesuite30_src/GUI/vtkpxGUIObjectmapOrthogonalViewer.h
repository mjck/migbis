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
  Module:    $RCSfile: vtkpxGUIObjectmapOrthogonalViewer.h,v $
  Language:  C++
  Date:      $Date: 2004/04/27 18:53:07 $
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

// .NAME vtkpxGUIObjectmapOrthogonalViewer 
// .SECTION Description

#ifndef __vtkpxGUIObjectmapOrthogonalViewer_h
#define __vtkpxGUIObjectmapOrthogonalViewer_h

#include "vtkpxGUIOrthogonalViewer.h"
#include "vtkLookupTable.h"
//#include "vtkCollection.h"

class vtkpxGUIObjectmapOrthogonalEditor;
class vtkpxGUI2DSplineObjectmapViewer;

class  vtkpxGUIObjectmapOrthogonalViewer : public vtkpxGUIOrthogonalViewer 
{
public:
  
  static vtkpxGUIObjectmapOrthogonalViewer *New();
  vtkTypeMacro(vtkpxGUIObjectmapOrthogonalViewer,vtkpxGUIOrthogonalViewer);

  // Jury is out but chance are some of these only
  virtual int   InitializeViewer();
  virtual int   ResetViewer();
  virtual int   HandleEvent(int eventno);
  virtual void  ChangeFrame(int t);
  virtual void  SetOrthoSliceVisibility(int orthovis);
  virtual void SetVolumeVisibility(int volvis);
  virtual void SetObliqueSliceVisibility(int volvis);


  // Description:
  // Objectmap Editor
  virtual void SetSplineObjectmapEditor(vtkpxGUI2DSplineObjectmapViewer* editor);
  virtual vtkpxGUI2DSplineObjectmapViewer* GetSplineObjectmapEditor();

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
  virtual vtkLookupTable* GetObjectLookupTable() { return this->maskLookupTable;}

  // Description:
  // Get Combined Image and Lookup Table of Current View
  //  vtkCollection* GetCombinedImageAndLookupTable();


  // Description:
  virtual void SetCreateInternalEditorOn();
  vtkGetObjectMacro(InternalEditor,vtkpxGUIObjectmapOrthogonalEditor);

  // Description::
  virtual int  HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev);


  // Description:
  // Does this have an objectmap
  virtual int HasObjectmap() { return 1;}
  virtual int PaintingEnabled();

  // Description:
  // This is useful for VOI painting
  virtual void  SetPresetLookupTable(int mode);

protected:

  vtkpxGUIObjectmapOrthogonalViewer();
  virtual ~vtkpxGUIObjectmapOrthogonalViewer();

  vtkImageData*              maskImage;
  vtkLookupTable*            maskLookupTable;
  vtkLookupTable*            maskVolumeLookupTable;

  vtkpxImageSlice*           maskImageSlice[3];
  vtkpxOrthoImageSlice       *maskOrthoSlice;
  PXTkArrowScale             *maskTransparency;
  vtkpxGUI2DSplineObjectmapViewer    *SplineObjectmapEditor;

  // Description:
  // Initialization Stuff 
  virtual void InitControls(int hm=1);

  // Description:
  // Change Coordinates Frame Stuff 
  virtual void ChangeSliceLevels(int lv[3],int infoonly=0);


  // Description:
  // Polar and Cartesian Stuff
  virtual int ResetViewerUpdatePolar();
  virtual int ResetViewerUpdateCartesian();
  
  // Description:
  // If we have no Editor
  virtual int AutoCreateMaskAndLookupTable();

  // Description:
  // Fix Volume Lookup Table
  virtual void UpdateMaskVolumeLookupTableAndImage();


  // Description:
  // Editor;
  int                                create_internal_editor;
  vtkpxGUIObjectmapOrthogonalEditor* InternalEditor;

  // Get Objectmap Value
  //BTX
  virtual std::string GetObjectmapValueAsText(int a=0,int b=0,int c=0);
  //ETX

};


#endif   


