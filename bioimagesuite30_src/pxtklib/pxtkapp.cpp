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






// 	$Id: pxtkapp.cpp,v 1.3 2002/12/18 16:11:12 papad Exp $	


#include "pxtkapp.h"
#include "pxtkinfodialog.h"
#include "vtkstd/string"
#include "sstream"
#include "iostream"
#ifdef _WIN32
#include <windows.h>
#endif

/* ********************************************************************

   Definition of PXTkApp Class 

********************************************************************* */

 Tcl_Interp*       PXTkApp::tcl_interpreter; 
 int               PXTkApp::num_arguments;
 int               PXTkApp::initialized;
 Tcl_Obj**         PXTkApp::arguments;
 PXTkInfoDialog*   PXTkApp::console;
 PXBool            PXTkApp::debug_tcl;  
 int               PXTkApp::debug_mode;
// -----------------------------------------------------------------------------
int  PXTkApp::Irange(int   a,int   minv,int   maxv)
{
  if (a<minv) a=minv;
  if (a>maxv) a=maxv;
  return a;
}


float PXTkApp::Frange(float a,float minv,float maxv)
{
  if (a<minv) a=minv;
  if (a>maxv) a=maxv;
  return a;
}

int PXTkApp::initialize(Tcl_Interp* tclinterp)
{
  if (PXTkApp::initialized==1000 && PXTkApp::tcl_interpreter!=NULL)
      return TCL_OK;

  PXTkApp::num_arguments=0;
  PXTkApp::arguments=NULL;
  PXTkApp::tcl_interpreter=tclinterp;
  PXTkApp::console=NULL;
  PXTkApp::debug_tcl=PXFalse;
  PXTkApp::debug_mode=0;

  Tcl_CreateObjCommand(tclinterp, "pxtkcallback", PXTkApp::handleEvent,
		       (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  
  Tcl_CreateObjCommand(tclinterp, "pxtkconsole", PXTkApp::consoleEvent,
		       (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);
  
  Tcl_CreateObjCommand(tclinterp, "pxtkprint", PXTkApp::consolePrint,
		       (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

  Tcl_CreateObjCommand(tclinterp, "pxtksetdebugmode", PXTkApp::setDebugMode,
		       (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

  Tcl_CreateObjCommand(tclinterp, "pxtkexit", PXTkApp::exitCommand,
		       (ClientData) NULL, (Tcl_CmdDeleteProc *) NULL);

  PXTkApp::initialized=1000;


  return PXTkApp::createConsole();
}

int PXTkApp::startEventLoop()
{
/*   executeTclCommand("bind all <Control-a>  { pxtkcallback 0 0 }");
     executeTclCommand("bind all <Control-q>  { pxtkcallback 0 1 }");
     executeTclCommand("bind all <Control-d>  { pxtkcallback 0 1 }");
     executeTclCommand("bind all <Control-w>  { pxtkcallback 0 4 }");*/
   executeTclCommand("bind all <F1> { pxtkconsole }");
   executeTclCommand("wm protocol . WM_DELETE_WINDOW { pxtkcallback 0 1} ");
   executeTclCommand("update idletasks");
   return executeTclCommand("vwait forever");
}

int    PXTkApp::getNumCommandLineArguments()
{
  PXTkApp::executeTclCommand("llength $argv");
  return getTclIntResult()+1;
}

char*  PXTkApp::getCommandLineArgument(int i)
{
  if (i==0)
    return getTclVariable("argv0");

  std::stringstream s;
  s << "lindex $argv ";
  s << i-1;

  PXTkApp::executeTclCommand(s.str().c_str());
  return getTclStringResult();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

/*int PXTkApp::registerEventHandler(PXTkEventObj* newhandler)
{
  if (num_event_handlers< PXTK_NUMHANDLERS)
      {
	event_handlers[num_event_handlers]=newhandler;
	newhandler->setHandlerNo(num_event_handlers);
	num_event_handlers++;
	return 1;
      }
  else
      return 0;
}

int PXTkApp::getNumEventHandlers()
{
  return num_event_handlers;
}*/

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
int PXTkApp::handleEventSetup(int objc,Tcl_Obj *CONST objv[],long& val1,int& val2)
{
  if (objc<3)
      {
	fprintf(stderr,"Num Parameters =%d\n",objc);
	return TCL_ERROR;
      }

  num_arguments=objc-3;
  arguments=(Tcl_Obj**)&objv[3];
 
  Tcl_GetLongFromObj(tcl_interpreter, objv[1], &val1);
  Tcl_GetIntFromObj(tcl_interpreter, objv[2], &val2);

  return TCL_OK;
}


int PXTkApp::handleEvent(ClientData cl, Tcl_Interp *interp,
			 int objc, Tcl_Obj *CONST objv[])
{
  tcl_interpreter=interp;
  //  fprintf(stderr,"Num Parameters =%d\n",objc);

  long val1=0; int val2=0;
  if (PXTkApp::handleEventSetup(objc,objv,val1,val2)==TCL_ERROR)
      return TCL_ERROR;

  if (val1==0 && val2==0)
    return PXTkApp::exitCommand(cl,interp,objc,objv);

  PXTkEventObj* handler=(PXTkEventObj*)val1;
  if (handler!=NULL)
      return handler->handleEvent(val2);
  else
      return TCL_ERROR;
  //return event_handlers[val1]->handleEvent(val2);
}

int PXTkApp::getNumArguments()
{
  return num_arguments;
}

int  PXTkApp::getIntArgument(int i)
{
  i=Irange(i,0,num_arguments-1);
  int v=0;
  Tcl_GetIntFromObj(tcl_interpreter,arguments[i],&v);
  return v;
}

float PXTkApp::getFloatArgument(int i)
{
  i=Irange(i,0,num_arguments-1);
  double v=0.0;
  Tcl_GetDoubleFromObj(tcl_interpreter,arguments[i],&v);
  return (float)v;
}

char* PXTkApp::getStringArgument(int i)
{
  i=Irange(i,0,num_arguments-1);
  return Tcl_GetString(arguments[i]);
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

char* PXTkApp::getTclVariable(const char* variablename)
{
  //return Tcl_GetVar(tcl_interpreter,variablename,0);
  std::stringstream s; 
  s << "set pxtk_dummy $";
  s << variablename;
  
  PXTkApp::executeTclCommand(s.str().c_str());
  //  char line[200];
  //  sprintf(line,"set pxtk_dummy $%s",variablename);
  //  fprintf(stderr,"new command \"%s\" vs \"%s\"\n",s.str().c_str(),line);
  //  PXTkApp::executeTclCommand(line);
  
  return getTclStringResult();

}

int PXTkApp::deleteTclVariable(const char* variablename)
{
  return Tcl_UnsetVar(tcl_interpreter,variablename,TCL_GLOBAL_ONLY);
}

char* PXTkApp::setTclVariable(const char* variablename, const char* value)
{
  //return Tcl_SetVar(tcl_interpreter,variablename,value,TCL_GLOBAL_ONLY);
  //  char pxtk_buffer[200];
  //  sprintf(pxtk_buffer,"set %s \"%s\" ",variablename,value);
  //  PXTkApp::executeTclCommand(pxtk_buffer);
  std::stringstream s; //  char line[200];
  s << "set ";
  s << variablename;
  s << " \"";
  s << value;
  s << "\"";
  PXTkApp::executeTclCommand(s.str().c_str());
  //fprintf(stderr,"new command \"%s\" vs \"%s\"\n",s.str().c_str(),pxtk_buffer);
  return getTclStringResult();
}

int PXTkApp::executeTclCommand(const char* command)
{
  if (tcl_interpreter!=NULL)
      {
	if (PXTkApp::debug_tcl)
	  {
	    printToConsole(command);
	    printToConsole("\n");
	  }
	//std::string command_buffer(command);
	return Tcl_GlobalEval(tcl_interpreter,command);
      }
  else
    {
      fprintf(stderr,"Interpreter=NULL init=%d\n",  PXTkApp::initialized);
      return TCL_ERROR;
    }
}

char* PXTkApp::getTclStringResult()
{
  Tcl_Obj* obj=Tcl_GetObjResult(tcl_interpreter);
  if (obj!=NULL) {
      return Tcl_GetString(obj);
  }
  else
      return NULL;

}

float PXTkApp::getTclFloatResult()
{
  Tcl_Obj* obj=Tcl_GetObjResult(tcl_interpreter);
  if (obj!=NULL)
      {
	double v=0.0;
	Tcl_GetDoubleFromObj(tcl_interpreter,obj,&v);
	return (float)v;
      }
  else
      return 0.0;
}
 
int PXTkApp::getTclIntResult()
{
  Tcl_Obj* obj=Tcl_GetObjResult(tcl_interpreter);
  if (obj!=NULL)
      {
	int v=0;
	Tcl_GetIntFromObj(tcl_interpreter,obj,&v);
	return v;
      }
  else
      return 0;

}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

Tk_Window PXTkApp::getWidgetPointer(const char* name)
{
  if (name!=NULL)
      {
	Tk_Window mainwind=Tk_MainWindow(PXTkApp::tcl_interpreter);
	if (mainwind!=NULL)
	    return Tk_NameToWindow(PXTkApp::tcl_interpreter,name,mainwind);
      }
  return NULL;
}


// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  

int PXTkApp::createConsole()
{
  if (PXTkApp::console==NULL)
    {
      PXTkApp::console=new PXTkInfoDialog("PXTkConsole",500,200);
      return PXTkApp::console->initDisplay();
    }
  return 0;
}

int PXTkApp::consoleEvent(ClientData, Tcl_Interp* ,int, Tcl_Obj *CONST objv[])
{
  return PXTkApp::popupConsole();
}


int PXTkApp::exitCommand(ClientData, Tcl_Interp* ,int, Tcl_Obj *CONST objv[])
{

#ifdef _WIN32
  PXTkApp::executeTclCommand("destroy .");
  PXTkApp::executeTclCommand("update idletasks");
  PXTkApp::executeTclCommand("after idle exit");
#endif
  exit(0);

  return TCL_OK;
}


int PXTkApp::setDebugMode(ClientData, Tcl_Interp* ,int objc, Tcl_Obj *CONST objv[] )
{
  if (objc<2)
    return TCL_OK;
  //  char line[250];
  //  strncpy(line,Tcl_GetString(objv[1]),200);
  std::string s(Tcl_GetString(objv[1]));

  int a=atoi(s.c_str());
  if (a>0)
    PXTkApp::debug_mode=1;
  else
    PXTkApp::debug_mode=0;
  return TCL_OK;
}

int PXTkApp::consolePrint(ClientData, Tcl_Interp* ,int objc, Tcl_Obj *CONST objv[])
{
  if (objc<2)
      return TCL_OK;
  //  char line[355];
  std::string str(Tcl_GetString(objv[1]));
  PXTkApp::printToConsole(str.c_str());
  return TCL_OK;
}

int PXTkApp::popupConsole()
{
  if (PXTkApp::console!=NULL)
    {
      //char line[400];
      //	sprintf(line,"wm geometry %s -5-5",PXTkApp::console->getMainWindow()->getWidgetName());
      std::string s("wm geometry ");
      s+=PXTkApp::console->getMainWindow()->getWidgetName();
      s+=" -5-5 ";
      //      fprintf(stderr,"executing %s\n",s.c_str());
      executeTclCommand(s.c_str());
      //sprintf(line,"wm deiconify %s",PXTkApp::console->getMainWindow()->getWidgetName());
      std::string s2("wm deiconify "); s2 = s2 + PXTkApp::console->getMainWindow()->getWidgetName();

      //      fprintf(stderr,"executing %s\n",s2.c_str());
      executeTclCommand(s2.c_str());
      return TCL_OK;
    }
  else
    return 0;
}

PXTkInfoDialog* PXTkApp::GetConsole()
{
  return PXTkApp::console;
}

int PXTkApp::printToConsole(const char* line)
{
 if (PXTkApp::console!=NULL)
   {
     //     fprintf(stderr,"Printing to Console %s\n",line);
     PXTkApp::console->addText(line);
     if (PXTkApp::debug_mode>0)
       fprintf(stderr,"*C*:%s",line);
     executeTclCommand("update idletasks");
     return 1;
   }
 else
   {
     fprintf(stderr,"%s\n",line);
   }
 return 0;
}

int PXTkApp::popdownConsole()
{
  if (PXTkApp::console!=NULL)
      {
	PXTkApp::console->popdownDialog();
	return 1;
      }
  else
      return 0;
}

int PXTkApp::autoArrangeWindows()
{
  return TCL_OK;
/*  int x1=0,y1=0,w1=0,h1=0;
  char app_buffer[200];
  sprintf(app_buffer,"winfo geometry .");
  if ( PXTkApp::executeTclCommand(app_buffer)==TCL_OK)
      {
	strcpy(app_buffer,PXTkApp::getTclStringResult());
	sscanf(app_buffer,"%dx%d+%d+%d",&w1,&h1,&x1,&y1);
	x1+=w1+25;
      }
  else
      return 0;

  //  int maxw=0;
  
  int id=0;
  for (int i=1;i<num_event_handlers-1;i++)
      {
	if (event_handlers[i]->getClassId()==PXTkDialogId)
	    {
	      PXTkDialog* dlg=(PXTkDialog*)event_handlers[i];
	      if (dlg->isOpen() && dlg->getObjectId()!=console->getObjectId() && !dlg->isInside())
		  {
		    id++;
		    ((PXTkToplevel*)dlg->getMainWindow())->show(id);
		  }
	    }
      }
  return 1;*/
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
int PXTkApp::popdownChildren(PXTkEventObj* closingparent)
{
  return TCL_OK;
  /*for (int i=1;i<num_event_handlers-1;i++)
      {
	if (event_handlers[i]->getClassId()==PXTkDialogId)
	    {
	      PXTkDialog* dlg=(PXTkDialog*)event_handlers[i];
	      if (dlg->isOpen() && dlg->getObjectId()!=console->getObjectId() && !dlg->isInside())
		  {
		    if (dlg->getEventParent() == closingparent)
			dlg->popdownDialog();
		  }
	    }
      }
  return 1;*/
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
int PXTkApp::debugMessage(const char* msg)
{
  //  char line[400];
  //  sprintf(line,"tk_messageBox -title \"Debug Message\" -type ok -message \"%s\" ",
  //	  msg);
  std::string s("tk_messageBox -title \"Debug Message\" -type ok -message \"");
  s+=msg;
  s+="\"";

  return PXTkApp::executeTclCommand(s.c_str());
}


int PXTkApp::setWaitCursor(PXBool setwait)
{
  if (!setwait)
      return executeTclCommand(". config -cursor top_left_arrow");
  else
      return executeTclCommand(". config -cursor watch");
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
int PXTkApp:: getpid()
{
  executeTclCommand("pid");
  return getTclIntResult();
}
 
char* PXTkApp::getpwd()
{
  executeTclCommand("pwd");
  return getTclStringResult();
}

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
void PXTkApp::fileOperation(const char* command, const char* fname1, const char* fname2)
{
  int n=5+strlen(command)+20+strlen(fname1)+2;
  if (fname2!=NULL)
    n+=strlen(fname2)+2;
  
  char* line=new char[n];
  if (fname2==NULL)
    sprintf(line,"file %s \"%s\"",command,fname1);
  else
    sprintf(line,"file %s \"%s\" \"%s\"",command,fname1,fname2);

  executeTclCommand(line);
  delete [] line;
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
void PXTkApp::deleteFile(const char* fname)
{
  fileOperation("delete",fname,NULL);
}

void PXTkApp::copyFile(const char* source,const char* dest)
{
  fileOperation("copy",source,dest);
}

void PXTkApp::renameFile(const char* source,const char* dest)
{
  fileOperation("rename",source,dest);
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
char* PXTkApp::getFilenameExtension(const char* fname)
{
  fileOperation("extension",fname,NULL);
  return getTclStringResult();
}

char* PXTkApp::getFilenamePath(const char* fname)
{
  fileOperation("dirname",fname,NULL);
  return getTclStringResult();
}

char* PXTkApp::getFilenameTail(const char* fname)
{
  fileOperation("tail",fname,NULL);
  return getTclStringResult();
}

char* PXTkApp::getFilenameRoot(const char* fname)
{
  fileOperation("rootname",fname,NULL);
  return getTclStringResult();
}

/******************************************************************************/

int PXTkApp::fileExists(const char* fname)
{
  fileOperation("exists",fname,NULL);
  return getTclIntResult();
}

/******************************************************************************/

int PXTkApp::isFile(const char* fname)
{
  fileOperation("isfile",fname,NULL);
  return getTclIntResult();
}

/******************************************************************************/

int PXTkApp::isDirectory(const char* fname)
{
  fileOperation("isdirectory",fname,NULL);
  return getTclIntResult();
}

/******************************************************************************/

int PXTkApp::fileReadable(const char* fname)
{
  fileOperation("readable",fname,NULL);
  return getTclIntResult();
}

/******************************************************************************/

int PXTkApp::fileWritable(const char* fname)
{
  fileOperation("writable",fname,NULL);
  return getTclIntResult();
}

/******************************************************************************/

int PXTkApp::fileExecutable(const char* fname)
{
  fileOperation("executable",fname,NULL);
  return getTclIntResult();
}

/******************************************************************************/

char PXTkApp::getDirSeparator()
{
   char DirSeparator = '/';
  
#ifdef _WIN32
      DirSeparator = '\\';
#endif

   return DirSeparator;
}

/******************************************************************************/

int PXTkApp::createDirectory(const char* dirName)
{
   fileOperation("mkdir", dirName,NULL);
   return getTclIntResult();
}

/******************************************************************************/

// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
char* PXTkApp::getIPAddress()
{
  executeTclCommand("set me [socket -server garbage_word -myaddr [info hostname] 0];set ip [lindex [fconfigure $me -sockname] 0];close $me;return $ip");
  return getTclStringResult();
    
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
char* PXTkApp::getUserName()
{
  executeTclCommand("set $tcl_platform(user)");
  return getTclStringResult();
}
// -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  
#
void pxtkapp_find_and_replace( std::string &source, const std::string find, std::string replace ) 
{
  size_t j=1,lastpos=0;
  int ok=0;
  while (j!=std::string::npos)
    {
      j=source.find(find,lastpos);
      if (j!=std::string::npos)
	{
	  //	  std::cerr << "In = " << source.c_str() << "(len="<<source.length() << "-->";
	  source.replace(j,find.length(),replace);
	  lastpos=j+replace.length();
	  ++ok;
	  //	  std::cerr << source.c_str() << "(len="<< source.length() << "\n";
	}
    }
}

void PXTkApp::StringSanitize(std::string &s)
{
  pxtkapp_find_and_replace(s,std::string("\""),std::string("\\\""));
  pxtkapp_find_and_replace(s,std::string("]"),std::string("\\]"));
  pxtkapp_find_and_replace(s,std::string("["),std::string("\\["));
  //  fprintf(stderr,"Sanitizing 3 %s\n",s.c_str());
}


