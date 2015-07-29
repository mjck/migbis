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









// pxtkgadgets.cc
#include "pxtkframe.h"
#include "pxtktoplevel.h"
#include "pxtkdialog.h"
#include <stdlib.h>
#include <math.h>

int  PXTkToplevel::num_top_levels=0;

/***********************************************

    Definition of PXTkToplevel Class 

 ********************************************************************* */

PXTkToplevel::PXTkToplevel(PXTkEventObj* evpar,PXTkGadget* gadg_par,const char* title,
			   int width,int height):PXTkFrame(evpar,gadg_par,width,height,PXFalse)
{
  initDisplay();
  setDimensions(width,height);
  setTitle(title);
  if (has_gadget_parent)
      {
	PXTkToplevel::num_top_levels++;
	top_level_id=PXTkToplevel::num_top_levels;
      }
}

PXTkClassId PXTkToplevel::getClassId()
{
  return PXTkToplevelId;
}


int PXTkToplevel::initDisplay()
{
  sprintf(pxtk_buffer,"toplevel %s",widget_name);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  sprintf(pxtk_buffer,"wm protocol %s WM_DELETE_WINDOW { grab release %s ; wm withdraw %s} ",
	widget_name,widget_name,widget_name);
  PXTkApp::executeTclCommand(pxtk_buffer);
  bindCallback("<Control-w>",3);

  if (a==TCL_OK)
    {
      is_display_ok=PXTrue;
      this->hide();
    }
  return a;
}

int PXTkToplevel::setGrab(PXBool grab)
{
  if (!is_display_ok)
      return TCL_ERROR;
  if (grab)
      {
	sprintf(pxtk_buffer,"grab %s",widget_name);
	return PXTkApp::executeTclCommand(pxtk_buffer);
      }
  else
      {
	sprintf(pxtk_buffer,"grab release %s",widget_name);
	return PXTkApp::executeTclCommand(pxtk_buffer);
      }
  return TCL_OK;
}
 
int PXTkToplevel::show(int id)
{
  if (!is_display_ok)
      return TCL_ERROR;

  sprintf(pxtk_buffer," ::pxtclutil::ShowWindow %s  ",widget_name);
  if (PXTkApp::executeTclCommand(pxtk_buffer)==TCL_OK)
    return TCL_OK;

  wmconfigure("deiconify","");
  sprintf(pxtk_buffer,"raise %s",widget_name);
  PXTkApp::executeTclCommand(pxtk_buffer);

  int x1=0,y1=0,w1=0,h1=0;
  sprintf(pxtk_buffer,"winfo parent %s",widget_name);
  PXTkApp::executeTclCommand(pxtk_buffer);
  strcpy(pxtk_buffer2,PXTkApp::getTclStringResult());
  if (strlen(pxtk_buffer)>0)
    {
      sprintf(pxtk_buffer,"winfo geometry %s",pxtk_buffer2);
      PXTkApp::executeTclCommand(pxtk_buffer);
      strcpy(pxtk_buffer,PXTkApp::getTclStringResult());
      sscanf(pxtk_buffer,"%dx%d+%d+%d",&w1,&h1,&x1,&y1);

      sprintf(pxtk_buffer,"lsearch [ winfo children %s] %s",pxtk_buffer2,widget_name);
      PXTkApp::executeTclCommand(pxtk_buffer);
      int offset=PXTkApp::getTclIntResult();

#ifdef _WIN32
      sprintf(pxtk_buffer,"focus -force %s",widget_name);
      PXTkApp::executeTclCommand(pxtk_buffer);
#endif
      sprintf(pxtk_buffer,"wm geometry %s +%d+%d",widget_name,x1+2*w1/3+20*offset,y1+h1/2+20*offset);
      PXTkApp::executeTclCommand(pxtk_buffer);
    }
  return TCL_OK;
}

int PXTkToplevel::getToplevelId()
{
  return top_level_id;
}

int PXTkToplevel::hide()
{
  if (!is_display_ok)
      return TCL_ERROR;

  sprintf(pxtk_buffer," ::pxtclutil::HideWindow %s  ",widget_name);
  if (PXTkApp::executeTclCommand(pxtk_buffer)==TCL_OK)
    return TCL_OK;

  return wmconfigure("withdraw","");
}

int PXTkToplevel::setTitle(const char* title)
{
  if (!is_display_ok)
      return TCL_ERROR;
  return wmconfigure("title",title,PXFalse);
}


