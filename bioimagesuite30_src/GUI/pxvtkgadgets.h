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





// pxvtkgadgets.h

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PxTk library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : PXTkGadgets

_Description : Definition of wrapper classes for the three vtkTkClasses

   PXVTkTkGadget            --- abstract base class
   PXVTkTkRenderWidget      --- wrapper around vtkTkRenderWidget
   PXVTkTkImageWindowWidget --- wrapper around vtkTkImageWindowWidget
   PXVTkTkImageViewerWidget --- wrapper around vtkTikImageViewerWidget


_Call :   

 PXVTkTkRenderWidget(PXTkEventObj* par,PXTkGadget* par,int moption=-1,PXBool dodisplay=PXTrue);
 PXVTkTkImageWindowWidget(PXTkEventObj* par,PXTkGadget* par,int moption=-1,PXBool dodisplay=PXTrue);
 PXVTkTkImageViewerWidget(PXTkEventObj* par,PXTkGadget* par,int moption=-1,PXBool dodisplay=PXTrue);

        
	
_References : Practical Programming in Tcl and Tk, Brent B. Welch (Prentice Hall 2nd Edition)
              The Visualization Toolki User's Guide, Kitware Inc (www.kitware.com)
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 6th July 2000

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#include "pxtkgadgets.h"
#include "pxtkdialog.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkImageViewer.h"

/*class vtkImageWindow;
class vtkImageWindowWidget;


class vtkImageViewer;
class vtkImageViewerWidget;*/

#ifndef _PXVTkGadget
#define  _PXVTkGadget

/* ----  Definition of PXVTkTkGadget Class  ---- */

class  PXVTkTkGadget : public PXTkGadget
{
protected:
  int call_no;

public:
  PXVTkTkGadget(PXTkEventObj* evpar,PXTkGadget* gadg_par,int moption=-1);
  
  virtual int setWidthHeight(int w=-1,int h=-1);
  virtual int setPadding(int bd=-1,int padx=-1,int pady=-1);
  virtual int bindMouseEvents();
};

/* ----  Definition of PXVTkTkRenderWidget Class  ---- */
class PXVTkTkRenderWidget : public PXVTkTkGadget
{
protected:
  vtkRenderWindow* renderWindow;
  vtkRenderer*     renderer;
  
public:
  PXVTkTkRenderWidget(PXTkEventObj* evpar,PXTkGadget* gadg_par,int moption=-1);
  
  virtual int initDisplay();

  virtual vtkRenderWindow* getRenderWindow();
  virtual vtkRenderer*     getRenderer();

  virtual void             Render();
  virtual void             RenderDelay(int t=3);

private:
  static void handleRenderEvent(void *arg1);
};

/* ----  Definition of PXVTkTkImageWindowWidget Class  ---- */

/*class PXVTkTkImageWindowWidget : public PXVTkTkGadget
{
protected:
  vtkImageWindow* imageWindow;

public:
  PXVTkTkImageWindowWidget(PXTkEventObj* evpar,PXTkGadget* gadg_par,int moption=-1);
  
  virtual int initDisplay();
  virtual vtkImageWindow *       getImageWindow();
};*/

/* ----  Definition of PXVTkTkImageViewerWidget Class  ---- */

/*class PXVTkTkImageViewerWidget : public PXVTkTkGadget
{
protected:
  vtkImageViewer* imageViewer;

public:
  PXVTkTkImageViewerWidget(PXTkEventObj* evpar,PXTkGadget* gadg_par,int moption=-1);
  
  virtual int initDisplay();
  virtual vtkImageViewer*        getImageViewer();
};*/

#endif

