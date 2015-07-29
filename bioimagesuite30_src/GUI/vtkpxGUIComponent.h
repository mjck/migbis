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
  Module:    $RCSfile: vtkpxGUIComponent.h,v $
  Language:  C++
  Date:      $Date: 2002/07/18 15:04:57 $
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
// .NAME vtkpxGUIComponent - create hierarchies of actors
// .SECTION Description
#ifndef __vtkpxGUIComponent_h
#define __vtkpxGUIComponent_h

#include "bioimagesuite_settings.h"


#include "vtkObject.h"
#include "pxvtktkdialog.h"
#include "pxtkframe.h"
#include "pxtkanaheadercontrol.h"
#include "vtkProcessObject.h"

#include <string>
class vtkpxGUICallback;


class vtkpxGUIComponent : public vtkObject
{
public:
  static vtkpxGUIComponent *New();
  vtkTypeMacro(vtkpxGUIComponent,vtkObject);
  
  virtual char*  Initialize(const char* name,int inside);
  virtual char*  Initialize(const char* name) { return Initialize(name,1);}
  virtual char*  GetWidgetName();

  virtual int    HandleEvent(int ) {return TCL_OK;}
  virtual int    HandleMouseButtonEvent(int nbutton,int state,int x,int y,PXTkEventObj* obj) {return 0;}
  virtual int    HandleMouseButtonEvent(int nbutton,int state,int x,int y) 
  { return this->HandleMouseButtonEvent(nbutton,state,x,y,this->EventManager);}

  virtual int    BindMouseEvents(const char* widgetname,const char* commandname1,const char* commandname2);
  virtual int    BindMouseEvents(const char* widgetname,int callno1,int callno2);

  virtual void   SetCallback(vtkpxGUIComponent* comp,int callno);
  virtual void   SetCallback(const char* CallbackName);

  virtual void WatchOn(int dodot=1);
  virtual void WatchOff(int dodot=1);

  //BTX
  virtual PXVTkTkDialog* GetEventManager() { return EventManager;}
  //ETX
  virtual int  IsOpen();
  virtual void Show(int show=1);
  virtual void Hide() { Show(0);}

  //BTX
  virtual vtkpxGUICallback* AddMethodsToFilter(vtkObject* filter,const char* label);
  //ETX
  virtual void SetProgressCommand(const char* CallbackName);
  virtual void ExecuteProgressMethod(const char* label,float progress);

  // Description:
  // Returns the name of tk widget contains analyze header edit control
  static char* CreateAnaHeaderControl(const char* name);

  // THis is a hack for PythonTkInter -- use with care!
  static int vtkpxPythonTkInitialize(int value);


protected:

  PXTkFrame*         BaseWindow;
  PXVTkTkDialog*     EventManager;
  int                Initialized;
  vtkpxGUIComponent* EventParent;	  
  int                CallbackNo;
  int                LastEvent;
  char*              CallbackName;
  char*              ProgressCommand;
  

  vtkpxGUIComponent();
  virtual ~vtkpxGUIComponent();
  vtkpxGUIComponent(const vtkpxGUIComponent&) {};
  void operator=(const vtkpxGUIComponent&) {};

  //BTX
  static char pxtk_buffer[4000], pxtk_buffer2[4000], pxtk_buffer3[4000], pxtk_buffer4[4000];
  std::string Buffer;
  //ETX
  
};
#endif

