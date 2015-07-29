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





// pxvtkColormap.h
// -----------------------------------------------------------------

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000 papad@noodle.med.yale.edu


_Module_Name : vtkpxGUIColormapEditor


_Description : Definition of Classes to draw iso-surfaces 
 
               vtkpxGUIColormapEditor()

_Call : 

  
_References : The Visualization Toolkit: An Object-Oriented
              Approach to Graphics. 2nd Edition. Prentice-Hall
	      Schroeder, Martin and Lorensen
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 17th August 2000
                                
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _vtkpxGUIColormapEditor
#define _vtkpxGUIColormapEditor


#include "pxutil.h"
#include "pxtkgadgets.h"
#include "pxtkcomplexwidgets.h"
#include "pxtkseldialog.h"
#include "pxtkmenu.h"
#include "vtkLookupTable.h"
#include "pxvtkgadgets.h"
#include "vtkXYPlotActor.h"
#include "vtkpxGUIComponent.h"
#include "vtkProperty.h"
#include "vtkPlaneSource.h"
#include "vtkPolyDataMapper.h" 
#include "vtkScalarBarActor.h"
#include "vtkpxBaseCurve.h"


// ****************************************
// ******* vtkpxGUIColormapEditor Class ********
// ****************************************

class  vtkpxGUIColormapEditor : public vtkpxGUIComponent
{
public:

  static vtkpxGUIColormapEditor *New();
  vtkTypeMacro(vtkpxGUIColormapEditor,vtkpxGUIComponent);
  
  virtual char* Initialize(const char* name,int inside);
  virtual int   HandleEvent(int );
  virtual int   HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* ev);

  virtual vtkLookupTable* GetColormap();
  virtual void  SetColormap(vtkLookupTable* cmap);
  virtual void  SetSimpleMode(int simple=0);
  virtual void  SetVolumeMode(int vol=0);
  virtual void  SetAutoOpacity(int mode=1);
  virtual void  SetFmriMode(int fmri=1);

  virtual void  SetRange(int min,int max);
  virtual void  SetRange(vtkImageData* image);
  virtual void  SetNumberOfColors(int numc=256);
  virtual void  SetLock(int lock=1);
  virtual void  Update();

protected:

  //BTX
  vtkLookupTable *colormap;
  vtkLookupTable *internal_colormap;
  char*           colormap_filename;
  int             owns_colormap,simple_mode,volume_mode,locked;
  int             num_colors,fmri_mode,num_decimal_points;
  float           min_value,max_value;
  float           auto_min,auto_max;
  char            data_type[25];
  int             packed[5];
  
  PXVTkTkRenderWidget          *render_widget;
  PXTkFrame                    *simple_parent;
  PXTkButton                   *lockunlock;
  PXTkArrowScale               *scale[5],*levelscale[8],*valuescale[8];
  PXTkOptionMenu               *colormap_type,*editor_type;
  PXTkDialog                   *editor_dialog;
  PXTkCheckButton              *auto_opacity_chkbtn;
  //  vtkScalarBarActor*           scalar_bar;
  vtkXYPlotActor*              plot_actor;
  //ETX

  
  vtkpxGUIColormapEditor();
  virtual ~vtkpxGUIColormapEditor();
  vtkpxGUIColormapEditor(const vtkpxGUIColormapEditor&) {};
  void operator=(const vtkpxGUIColormapEditor&) {};

  virtual int CreateMenu(PXTkFrame* menubar);
  virtual int CreateSimpleControls(PXTkFrame* frame);
  virtual int CreateEditorControls(PXTkFrame* edit);
  virtual int CreateRenderer(PXTkFrame* frame);
  virtual int CreateComplexControls(PXTkFrame* frame);

  virtual int  ReadColormap(const char* name);
  virtual int  SaveColormap(const char* name);

  virtual int  ImportColormap(const char* name);
  virtual int  ExportColormap(const char* name);

  virtual int  DisplayColormap();
  virtual int  UpdateGraphFromColormap();

  virtual int  ResetColormap(int controlsonly=0);
  virtual int  ResetComplexColormap(int controlsonly=0);
  virtual int  ResetSimpleColormap(int controlsonly=0);

  virtual int  SetColormapFromDialog();
  virtual int  SetComplexColormapFromDialog(int active=-1);
  virtual int  SetSimpleColormapFromDialog();
  virtual int  SetPickedColorMapControl(int picked_value); 

  virtual void AutoSetNumDecimalPoints();

  // Description:
  // Interactor Stuff
  //BTX
  //  vtkGenericRenderWindowInteractor* WidgetInteractor;
  vtkScalarBarActor *scalar_bar;
  vtkpxBaseCurve *contourRep_red;
  vtkpxBaseCurve *contourRep_green;
  vtkpxBaseCurve *contourRep_blue;
  vtkpxBaseCurve *contourRep_alpha;
  vtkpxBaseCurve *contourRep_rgb;
  vtkpxBaseCurve *contourRep_box;

  // Pointer to all of these
  vtkpxBaseCurve *contourRep[6];
  double          contourOffset[3];
  double          contourScale[3];
  int             activeShiftPoint;
  int             activeColorContour;
  //ETX


};




#endif   


