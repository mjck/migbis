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






// 	$Id: pxtkapp.h,v 1.1 2002/02/20 18:04:13 Xenios Exp papad $	
// pxtkapp.h

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PxTk library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : PXTk

_Description : Base Header Definitions and Abstract Classes
               For C++ and TK interaction
	       Definitions of basic abstraction mechanisms

	       Definition of 
	       
	       PXTkApp                 --- static functions for implementing callbackmechanisms
	                                   and Tcl/Tk Command Interface  


_Call :   
        
	
_References : 
              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 11th May 2000

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PXTkApp
#define  _PXTkApp

#include "tcl.h"
#include "tk.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef _WIN32
   #include <unistd.h>
#endif


#ifndef PXBOOL
 #ifndef _WIN32
   enum PXBool { PXFalse = 0, PXTrue };
 #else
   #define PXBool  bool
   #define PXFalse false
   #define PXTrue  true
  #endif
#define PXBOOL 1
#endif


class PXTkInfoDialog;
class PXTkBase;
class PXTkEventObj;
#include <vtkstd/string>

// Number of Event Handlers
const int PXTK_NUMHANDLERS = 2000;

#define PXTK_APP 1
// Some functions were added by Rajeevan on 16th Feb, 2004

#define PXTK_APP 1

class   PXTkApp
{
private:
  //  static int           num_event_handlers;
  //  static PXTkEventObj* event_handlers[PXTK_NUMHANDLERS];
  static int             num_arguments;
  static int             initialized;
  static Tcl_Obj         **arguments;
  static PXTkInfoDialog  *console;
  static int              debug_mode;

public:
  static Tcl_Interp*   tcl_interpreter;
  static PXBool        debug_tcl;  

  // Initialization Stuff
  // --------------------
  static int          initialize(Tcl_Interp* tclinterp);
  static int          startEventLoop();
  static int          getNumCommandLineArguments();
  static char*  getCommandLineArgument(int i);

  // Event Handler Stuff
  // -------------------
  //  static int          registerEventHandler(PXTkEventObj* newhandler);
  //  static int          getNumEventHandlers();

  // Event Loop Handling
  // -------------------

  static int          handleEventSetup(int objc,Tcl_Obj *CONST objv[],
				       long& val1,int& val2);

  static int          handleEvent(ClientData clientData, Tcl_Interp *interp,
				  int objc, Tcl_Obj *CONST objv[]);


  static int          getNumArguments();
  static int          getIntArgument(int);
  static float        getFloatArgument(int);
  static char*  getStringArgument(int);
  
  // Tcl Command Execution
  // ---------------------
  static int          executeTclCommand(const char* command);
  static char*        getTclStringResult();
  static float        getTclFloatResult();
  static int          getTclIntResult();

  static char*         getTclVariable(const char* variablename);
  static char*         setTclVariable(const char* variablename, const char* value);
  static int           deleteTclVariable(const char* variablename);

  // Tcl Pointer Stuff
  // -----------------
  static Tk_Window     getWidgetPointer(const char* name);

  // Console Related
  // ---------------
  static int          consoleEvent(ClientData, Tcl_Interp* ,int, Tcl_Obj *CONST objv[] );
  static int          consolePrint(ClientData, Tcl_Interp* ,int, Tcl_Obj *CONST objv[] );
  static int          setDebugMode(ClientData, Tcl_Interp* ,int, Tcl_Obj *CONST objv[] );
  static int          exitCommand(ClientData, Tcl_Interp* ,int, Tcl_Obj *CONST objv[] );
  static int          createConsole();
  static int          popupConsole();
  static int          printToConsole(const char* line);
  static int          popdownConsole();
  static PXTkInfoDialog* GetConsole();

  // Auto Arrange
  // ------------
  static int          autoArrangeWindows();
  static int          popdownChildren(PXTkEventObj* parent);

  // MessageRelated
  // --------------
  static int          debugMessage(const char* msg);

  // Wait Cursor
  static int          setWaitCursor(PXBool setwait);
  
  // System Related
  // --------------
  static int          getpid();
  static char*        getpwd();

  // File Processing
  // -------------------
  static void         fileOperation(const char* command, const char* fname1, const char* fname2=NULL);
  static void         deleteFile(const char* fname);
  static void         copyFile(const char* source,const char* dest);
  static void         renameFile(const char* source,const char* dest);

  static char*        getFilenameExtension(const char* fname);
  static char*        getFilenamePath(const char* fname);
  static char*        getFilenameTail(const char* fname);
  static char*        getFilenameRoot(const char* fname);

  // File functions added by rajeevan
  static int          fileExists(const char* fname);
  static int          isFile(const char* fname);
  static int          isDirectory(const char* fname);
  static int          fileReadable(const char* fname);
  static int          fileWritable(const char* fname);
  static int          fileExecutable(const char* fname);

  static char         getDirSeparator();
  static int          createDirectory(const char* dirName);

  // Some Machine Information
  // ------------------------
  static char*        getIPAddress();
  static char*        getUserName();
  

  static int   Irange(int   a,int   minv,int   maxv);
  static float Frange(float a,float minv,float maxv);

  // String Stuff
  static void  StringSanitize(std::string& source);

};


#endif /*  _PXTkEventObj */

