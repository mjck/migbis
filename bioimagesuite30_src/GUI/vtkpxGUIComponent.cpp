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
  Module:    $RCSfile: vtkpxGUIComponent.cpp,v $
  Language:  C++
  Date:      $Date: 2002/07/18 15:04:07 $
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
#include "vtkpxGUIComponent.h"
#include "vtkObjectFactory.h"
#include "vtkTclUtil.h"
#include "pxtkapp.h"
//#include "pxtkanaheadercontrol.h"
#include "vtkpxGUICallback.h"
#include <string>


char   vtkpxGUIComponent::pxtk_buffer[4000];
char   vtkpxGUIComponent::pxtk_buffer2[4000];
char   vtkpxGUIComponent::pxtk_buffer3[4000];
char   vtkpxGUIComponent::pxtk_buffer4[4000];



// -----------------------------------------------------------------------
vtkpxGUIComponent* vtkpxGUIComponent::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxGUIComponent");
  if(ret)
      {
	return (vtkpxGUIComponent*)ret;
      }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxGUIComponent;
}
// -----------------------------------------------------------------------
// Construct object with no children.
vtkpxGUIComponent::vtkpxGUIComponent()
{
  this->EventManager=NULL;
  this->BaseWindow=NULL;
  this->Initialized=0;
  this->EventParent=NULL;
  this->CallbackNo=-1;
  this->CallbackName=NULL;
  this->ProgressCommand=NULL;
  this->LastEvent=-1;
  this->Buffer="";
}
// -----------------------------------------------------------------------
vtkpxGUIComponent::~vtkpxGUIComponent()
{
  if (this->EventManager!=NULL)
      delete this->EventManager;
  if (this->BaseWindow!=NULL)
      delete this->BaseWindow;
  if (this->CallbackName!=NULL)
    delete [] this->CallbackName;

  if (this->ProgressCommand!=NULL)
    delete [] this->ProgressCommand;

}
// -----------------------------------------------------------------------
char* vtkpxGUIComponent::Initialize(const char* name,int inside)
{
  if (this->BaseWindow==NULL)
      {
	this->BaseWindow=new PXTkFrame(NULL,name);

	PXBool createinside=PXFalse;
	if (inside)
	    createinside=PXTrue;

	this->EventManager=new PXVTkTkDialog(this,this->BaseWindow,
					     "vtkpxGUIComponent",createinside);
	this->EventManager->initDisplay();
	this->Initialized=1;
      }

  return GetWidgetName();
}
// -----------------------------------------------------------------------
 char* vtkpxGUIComponent::GetWidgetName()
{
  if (this->Initialized==1)
    return this->EventManager->getMainWindow()->getWidgetName();

  char* line=new char[2];
  strcpy(line,".");
  return line;
}
// -----------------------------------------------------------------------

int vtkpxGUIComponent::BindMouseEvents(const char* widgetname,int callno1,int callno2)
{
  sprintf(pxtk_buffer,"pxtkcallback %ld %d",this->EventManager->getHandlerNo(),callno1);
  sprintf(pxtk_buffer2,"pxtkcallback %ld %d",this->EventManager->getHandlerNo(),callno2);
  return BindMouseEvents(widgetname,pxtk_buffer,pxtk_buffer2);
}
// -----------------------------------------------------------------------
int vtkpxGUIComponent::BindMouseEvents(const char* widgetname,const char* commandname1,const char* commandname2)
{
  strcpy(pxtk_buffer2," %x %y");
  for (int mouse=1;mouse<=3;mouse++)
      {
	sprintf(pxtk_buffer,"bind %s <ButtonPress-%d> { %s %d 0 %s %s }",
		widgetname,mouse,commandname1,mouse,pxtk_buffer2,widgetname);
	PXTkApp::executeTclCommand(pxtk_buffer);
	
	sprintf(pxtk_buffer,"bind %s <B%d-Motion> { %s %d 1 %s %s }",
		widgetname,mouse,commandname1,mouse,pxtk_buffer2,widgetname);
	PXTkApp::executeTclCommand(pxtk_buffer);
	
	sprintf(pxtk_buffer,"bind %s <ButtonRelease-%d> { %s %d 2 %s %s }",
		widgetname,mouse,commandname1,mouse,pxtk_buffer2,widgetname);
	PXTkApp::executeTclCommand(pxtk_buffer);

	sprintf(pxtk_buffer,"bind %s <Shift-ButtonPress-%d> { %s %d 0 %s %s }",
		widgetname,mouse,commandname1,mouse+3,pxtk_buffer2,widgetname);
	PXTkApp::executeTclCommand(pxtk_buffer);
	
	sprintf(pxtk_buffer,"bind %s <Shift-B%d-Motion> { %s %d 1 %s %s }",
		widgetname,mouse,commandname1,mouse+3,pxtk_buffer2,widgetname);
	PXTkApp::executeTclCommand(pxtk_buffer);
	
	sprintf(pxtk_buffer,"bind %s <Shift-ButtonRelease-%d> { %s %d 2 %s %s }",
		widgetname,mouse,commandname1,mouse+3,pxtk_buffer2,widgetname);
	PXTkApp::executeTclCommand(pxtk_buffer);

	sprintf(pxtk_buffer,"bind %s <Control-ButtonPress-%d> { %s %d 0 %s %s }",
		widgetname,mouse,commandname1,mouse+6,pxtk_buffer2,widgetname);
	PXTkApp::executeTclCommand(pxtk_buffer);
	
	sprintf(pxtk_buffer,"bind %s <Control-B%d-Motion> { %s %d 1 %s %s }",
		widgetname,mouse,commandname1,mouse+6,pxtk_buffer2,widgetname);
	PXTkApp::executeTclCommand(pxtk_buffer);
	
	sprintf(pxtk_buffer,"bind %s <Control-ButtonRelease-%d> { %s %d 2 %s %s }",
		widgetname,mouse,commandname1,mouse+6,pxtk_buffer2,widgetname);
	PXTkApp::executeTclCommand(pxtk_buffer);
      }
  
  if (strlen(commandname2)>1)
    {
      sprintf(pxtk_buffer,"bind %s <Expose> { %s %s }",
	      widgetname,commandname2,widgetname);
      PXTkApp::executeTclCommand(pxtk_buffer);
    }

  return TCL_OK;
}
// -----------------------------------------------------------------------
void vtkpxGUIComponent::SetCallback(vtkpxGUIComponent* comp,int call_no)
{
  this->EventParent=comp;
  this->CallbackNo=call_no;
}
// -----------------------------------------------------------------------
void vtkpxGUIComponent::WatchOn(int dodot)
{
  int flag=0;
  if (dodot)
    {
      PXTkApp::executeTclCommand(". config -cursor watch");
      flag=1;
    };

  if (this->Initialized)
    {
      sprintf(pxtk_buffer,"%s config -cursor watch",this->GetWidgetName());
      PXTkApp::executeTclCommand(pxtk_buffer);
      flag=1;
    }

  if (flag==1)
    PXTkApp::executeTclCommand("update idletasks");
}
// -----------------------------------------------------------------------
void vtkpxGUIComponent::WatchOff(int dodot)
{
  int flag=0;
  if (dodot)
    {
      PXTkApp::executeTclCommand(". config -cursor \"\"");
      flag=1;
    };

  if (this->Initialized)
    {
      sprintf(pxtk_buffer,"%s config -cursor \"\"",this->GetWidgetName());
      PXTkApp::executeTclCommand(pxtk_buffer);
      flag=1;
    }

  if (flag==1)
    PXTkApp::executeTclCommand("update idletasks");
}
// -----------------------------------------------------------------------
void vtkpxGUIComponent::Show(int show)
{
  if (this->Initialized)
    {
      if (show)
	this->EventManager->popupDialog();
      else
	this->EventManager->popdownDialog();
    }
}

int vtkpxGUIComponent::IsOpen()
{
  if (this->Initialized)
    {
      if (this->EventManager->isOpen())
	return 1;
    }
  return 0;
}
// -----------------------------------------------------------------------
void vtkpxGUIComponent::SetCallback(const char* name)
{
  if (name==NULL)
    return;

  if (this->CallbackName!=NULL)
    delete [] this->CallbackName;

  this->CallbackName=new char[strlen(name)+1];
  strcpy(this->CallbackName,name);
}
// -----------------------------------------------------------------------
vtkpxGUICallback* vtkpxGUIComponent::AddMethodsToFilter(vtkObject* filter,const char* label)
{
  if (filter==NULL || label==NULL)
    return NULL;

  vtkpxGUICallback* clb=vtkpxGUICallback::New();
  clb->AttachFilter((vtkProcessObject*)filter,this,label);
  filter->AddObserver(vtkCommand::StartEvent,clb);
  filter->AddObserver(vtkCommand::ProgressEvent,clb);
  filter->AddObserver(vtkCommand::EndEvent,clb);

  return clb;

  
}
// -----------------------------------------------------------------------
void vtkpxGUIComponent::SetProgressCommand(const char* name)
{
  if (name==NULL)
    return;

  if (this->ProgressCommand!=NULL)
    delete [] this->ProgressCommand;

  this->ProgressCommand=new char[strlen(name)+1];
  strcpy(this->ProgressCommand,name);
}
// -----------------------------------------------------------------------
void vtkpxGUIComponent::ExecuteProgressMethod(const char* label,float progress)
{
  if (this->ProgressCommand==NULL)
    return;

  if (progress<1.0)
    sprintf(pxtk_buffer,"%s \"%s\" %f",this->ProgressCommand,label,progress);
  else
    sprintf(pxtk_buffer,"%s Done 0.0",this->ProgressCommand);

  PXTkApp::executeTclCommand(pxtk_buffer);
}
// -----------------------------------------------------------------------
char* vtkpxGUIComponent::CreateAnaHeaderControl(const char* name)
{
  PXTkFrame* frame=new PXTkFrame(NULL,name);
  PXTkAnaHeaderControl* ana=new PXTkAnaHeaderControl(NULL,frame);
  ana->initDisplay();
  ana->popupDialog();
  return ana->getMainWindow()->getWidgetName();
}


#ifdef BIOIMAGESUITE3_USE_PYTHON
#include <vtkPythonUtil.h>
#include "vtkTclUtil.h"

int vtkpxGUIComponent::vtkpxPythonTkInitialize(int interpaddr)
{
/*
 4321: - Add a new method of interpreter objects, interpaddr().  This returns
 4322: the address of the Tcl interpreter object, as an integer.  Not very
 4323: useful for the Python programmer, but this can be called by another C
 4324: extension that needs to make calls into the Tcl/Tk C API and needs to
 4325: get the address of the Tcl interpreter object.  A simple cast of the
 4326: return value to (Tcl_Interp *) will do the trick.
static PyObject *
Tkapp_InterpAddr(PyObject *self, PyObject *args)
{

        if (!PyArg_ParseTuple(args, ":interpaddr"))
                return NULL;

        return PyInt_FromLong((long)Tkapp_Interp(self));
}

*/
  Tcl_Interp* interp = (Tcl_Interp*)interpaddr;

  std::cout << "interp->result: " << interp->result << std::endl;
  std::cout << "hex of interp: " << interp << std::endl;

  std::cout << "lets run something: expr 4+7 " << interp->result << std::endl;
  Tcl_Eval(interp, "expr 4+7");

  std::cout << "interp->result: " << interp->result << std::endl;

  int b=PXTkApp::initialize(interp);
  fprintf(stderr,"b=%d\n",b);
  PXTkApp::createConsole();
  return b;
}
#else
int vtkpxGUIComponent::vtkpxPythonTkInitialize(int value)
{
  return 0;
}
#endif

