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





// pxtk.cc
#include "pxtkeventobj.h"
#include "pxtkgadget.h"


/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu
   
   see pxtk.h for details

// 	$Id: pxtkeventobj.cpp,v 1.1 2002/02/20 18:04:13 Xenios Exp $	


#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

 /* ********************************************************************

    Definition of PXTkEventObj Class 

 ********************************************************************* */

char   PXTkEventObj::pxtk_buffer[4000];
char   PXTkEventObj::pxtk_buffer2[4000];

// Do not use these variables; they are used for internal implementation
// ---------------------------------------------------------------------
char   PXTkEventObj::pxtk_intbuffer[4000];
char   PXTkEventObj::pxtk_intbuffer2[4000];
char   PXTkEventObj::pxtk_intbuffer3[4000];
char   PXTkEventObj::pxtk_intbuffer4[4000];


PXTkEventObj::PXTkEventObj(PXTkEventObj *p,PXTkGadget* gadg):PXTkBase(p,gadg)
{
 
  main_window=NULL;
  //  PXTkApp::registerEventHandler(this);
}

PXTkEventObj::~PXTkEventObj() 
{
  if (main_window!=NULL)
      delete main_window;
}				


PXBool PXTkEventObj::isGadget() 
{ 
return PXFalse;
}

PXTkClassId PXTkEventObj::getClassId()
{
  return PXTkObjId;
}


long PXTkEventObj::getHandlerNo()
{
  return long(this);
  //return handler_no;
}

/*void PXTkEventObj::setHandlerNo(int h)
{
  handler_no=h;
}*/

PXTkGadget* PXTkEventObj::getMainWindow()
{
  return main_window;
}

int PXTkEventObj::handleEvent(int )
{
  return TCL_OK;
}

/* -------------------------------------------------------------------------*/
//           Graphic Utility Code 
/* -------------------------------------------------------------------------*/

int PXTkEventObj::packMainWindow()
{
  sprintf(pxtk_intbuffer,"pack %s -fill both -expand true",main_window->getWidgetName());
  return (PXTkApp::executeTclCommand(pxtk_intbuffer));
}


int PXTkEventObj::setWaitCursor(PXBool setwait)
{
  if (is_display_ok)
      {
	if (!setwait)
	  sprintf(pxtk_intbuffer,"%s config -cursor \"\"",main_window->getWidgetName());
	else
	  sprintf(pxtk_intbuffer,"%s config -cursor watch",main_window->getWidgetName());
	PXTkApp::executeTclCommand(pxtk_intbuffer);
	return PXTkApp::executeTclCommand("update");
      }
  else
      return TCL_ERROR;
}

int PXTkEventObj::messageBox(const char* message, const char *title, const char* mboxtype)
{
  strcpy(pxtk_intbuffer2,"ok");
  if (mboxtype!=NULL)
      strcpy(pxtk_intbuffer2,mboxtype);

  strcpy(pxtk_intbuffer3,"For your information ...");
  if (title!=NULL)
      strcpy(pxtk_intbuffer3,title);

  sprintf(pxtk_intbuffer,"tk_messageBox -title \"%s\" -type %s -message \"%s\" -parent %s",
	  pxtk_intbuffer3,pxtk_intbuffer2,message,this->main_window->getWidgetName());
  return PXTkApp::executeTclCommand(pxtk_intbuffer);
}

int PXTkEventObj::colorBox(const char* title,float& r,float& g,float& b)
{
  int c[3];
  c[0]=int(Frange(r,0,1.0)*255.0);
  c[1]=int(Frange(g,0,1.0)*255.0);
  c[2]=int(Frange(b,0,1.0)*255.0);

  strcpy(pxtk_buffer,"#");
  for (int k=0;k<=2;k++)
    {
      for (int byte=0;byte<=1;byte++)
	{
	  int val=0;
	  switch(byte)
	    {
	    case 0:
	      val=c[k]/16;
	      break;
	    case 1:
	      val=c[k]-16*int(c[k]/16);
	      break;
	    }
	  
	  if (val<10)
	      {
		sprintf(pxtk_buffer,"%s%1d",pxtk_buffer,val);
	      }
	  else
	      {
		switch (val)
		    {
		    case 10:
		      strcat(pxtk_buffer,"A");
		      break;
		    case 11:
		      strcat(pxtk_buffer,"B");
		      break;
		    case 12:
		      strcat(pxtk_buffer,"C");
		      break;
		    case 13:
		      strcat(pxtk_buffer,"D");
		      break;
		    case 14:
		      strcat(pxtk_buffer,"E");
		      break;
		    case 15:
		      strcat(pxtk_buffer,"F");
		      break;
		    }
	      }
	}
    }

  //  PXTkApp::printToConsole(pxtk_buffer);
  sprintf(pxtk_buffer2,"tk_chooseColor -title \"%s\" -initialcolor %s -parent %s",
	  title,pxtk_buffer,this->main_window->getWidgetName());
  PXTkApp::executeTclCommand(pxtk_buffer2);
  
  char* line=PXTkApp::getTclStringResult();

  if (line!=NULL)
    {
      if (strlen(line)>0)
	{
	  for (int i=0;i<=2;i++)
	    {
	      c[i]=0;
	      for (int ib=0;ib<=1;ib++)
		{
		  int index=1+i*2+ib;
		  int val=(int)line[index];
		  if (val<65)
		    val-=48;
		  else if (val<70)
		    val-=55;
		  else 
		    val-=87;
		  
		  if (ib==0)
		    c[i]+=val*16;
		  else
		    c[i]+=val;
		}
	    }
	  
	  r=float(c[0])/255.0;
	  g=float(c[1])/255.0;
	  b=float(c[2])/255.0;
	  /*sprintf(pxtk_buffer," --> %s ---> (%d,%d,%d) , (%f,%f,%f)\n",
	    line,c[0],c[1],c[2],r,g,b);*/
	  //PXTkApp::printToConsole(pxtk_buffer);
	  return 1;
	}
    }
  return 0;
}

int  PXTkEventObj::questionBox(const char* message)
{
  sprintf(pxtk_buffer,
	  "tk_messageBox -type yesno -default no -title \"Think again ...\" -message \"%s\" -icon question -parent %s",
	  message,this->main_window->getWidgetName());
  PXTkApp::executeTclCommand(pxtk_buffer);
  char* line=PXTkApp::getTclStringResult();

  if (strncmp(line,"yes",3)==0)
      return 1;
  else
      return 0;
}

char* PXTkEventObj::getOpenFilename(const char* title, const char *extname, const char* ext)
{
  if (extname!=NULL && ext!=NULL)
    sprintf(pxtk_intbuffer2," {\"%s\" {\"%s\"}} {\"All Files\" {\"*\"}} ",extname,ext);
  else 
    sprintf(pxtk_intbuffer2," {\"All Files\" {\"*\"}} ");
  
  sprintf(pxtk_intbuffer,
	  "tk_getOpenFile -title \"%s\" -filetypes { %s } -parent %s",
	  title,pxtk_intbuffer2,this->main_window->getWidgetName());
  PXTkApp::executeTclCommand(pxtk_intbuffer);
  return PXTkApp::getTclStringResult();
}

char* PXTkEventObj::getOpenFilenameComplex(const char* title, const char* filetypes)
{
  sprintf(pxtk_intbuffer,
	  "tk_getOpenFile -title \"%s\" -filetypes { %s } -parent %s",
	  title,filetypes,this->main_window->getWidgetName());

  PXTkApp::executeTclCommand(pxtk_intbuffer);
  return PXTkApp::getTclStringResult();
}

char* PXTkEventObj::getSaveFilename(const char* title, const char* orig, const char* extname, const char* ext)
{
  if (extname!=NULL && ext!=NULL)
    sprintf(pxtk_intbuffer2," {\"%s\" {\"%s\"}} {\"All Files\" {\"*\"}} ",extname,ext);
  else 
    sprintf(pxtk_intbuffer2," {\"All Files\" {\"*\"}} ");

  if (orig!=NULL)
    strcpy(pxtk_intbuffer3,orig);
  else
    strcpy(pxtk_intbuffer3,"");

  sprintf(pxtk_intbuffer,"tk_getSaveFile -title \"%s\" -filetypes { %s } -initialfile \"%s\" -parent %s",
	  title, pxtk_intbuffer2, pxtk_intbuffer3,this->main_window->getWidgetName());
  PXTkApp::executeTclCommand(pxtk_intbuffer);
  return PXTkApp::getTclStringResult();
}


char* PXTkEventObj::getSaveFilenameComplex(const char* title, const char* orig, const char* filetypes)
{
  sprintf(pxtk_intbuffer,"tk_getSaveFile -title \"%s\" -filetypes { %s } -initialfile \"%s\" -parent %s",
	  title, filetypes, orig,this->main_window->getWidgetName());
  PXTkApp::executeTclCommand(pxtk_intbuffer);
  return PXTkApp::getTclStringResult();
}

/******************************************************************************/

char* PXTkEventObj::chooseDirectory(const char* title)
{
   sprintf(pxtk_intbuffer,"tk_chooseDirectory -title \"%s\" -parent %s", 
		   title, this->main_window->getWidgetName());
   PXTkApp::executeTclCommand(pxtk_intbuffer);
   return PXTkApp::getTclStringResult();
}

/******************************************************************************/

int PXTkEventObj::handleMouseButtonEvent(int,int,int ,int,PXTkEventObj* )
{
  return 0;
}

/******************************************************************************/
int PXTkEventObj::d_colorBox(const char* title,double& dr,double& dg,double& db)
{
  float r,b,g;
  r=dr;
  b=db;
  g=dg;

  int a=this->colorBox(title,r,g,b);
  dr=r;
  db=b;
  dg=g;
  return a;
}

