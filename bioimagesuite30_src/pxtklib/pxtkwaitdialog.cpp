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









#include "pxtkwaitdialog.h"
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu

   see pxtkwaitdialog.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

/* -------------------------------------------------------------------------*/
/*                   PXTkWaitDialog                                          */
/* -------------------------------------------------------------------------*/

/* -------------------------------------------------------------------------*/
PXTkWaitDialog::PXTkWaitDialog(PXTkEventObj* objpar,const char* title,int moption,PXBool showstop,
			       int x,int y):PXTkDialog(objpar,title)
{
  height=y;   width=x; showStop=showstop; menuoption=moption;
  this->debugMode=PXFalse;
}
/* -------------------------------------------------------------------------*/
int PXTkWaitDialog::initDisplay()
{
  PXTkDialog::initDisplay();
  PXTkFrame* manager=(PXTkFrame*)(main_window);

  PXTkFrame* desc=new PXTkFrame(this,manager,width,70);
  desc->configure("-relief","sunken");

  bottom_frame=new PXTkFrame(this,manager,width,70);

  manager->addChildren("-side bottom -expand f -fill x -padx 5 -pady 5",bottom_frame);
  manager->addChildren("-side top -expand true -fill both -padx 5 -pady 5",desc);

  PXTkLabel* lab=new PXTkLabel(this,desc," ");
  lab->configure("-bitmap","hourglass");

  message=new PXTkMessage(this,desc,"Description");
  message->configure("-width",width);
  message->configure("-apsect",100*(width/65));

  desc->addChildren("-side left -expand true ",lab,message);

  stop_button=new PXTkButton(this,bottom_frame,"Stop",1); 
  bottom_frame->addChildren(" ",stop_button);

  setText("Please Wait");
  showStopPrompt(showStop);
  return TCL_OK;
}
// -------------------------------------------------------------------------
int PXTkWaitDialog::setText(const char *line)
{
  message->setText(line);
  if (this->debugMode==PXTrue)
    fprintf(stderr,"%s\n",line);
  PXTkApp::executeTclCommand("update idletasks");
  return TCL_OK;
}
// -------------------------------------------------------------------------
int PXTkWaitDialog::setText(const char *text,int d)
{
  sprintf(pxtk_buffer,"%s %d",text,d);
  if (this->debugMode==PXTrue)
    fprintf(stderr,"%s\n",pxtk_buffer);
  message->setText(pxtk_buffer);
  PXTkApp::executeTclCommand("update idletasks");
  return TCL_OK;
}
// -------------------------------------------------------------------------
void PXTkWaitDialog::showStopPrompt(PXBool shows)
{
  showStop=shows;
  if (is_display_ok)
    {
      if (showStop)
	bottom_frame->mapChild(stop_button, " ");
      else
	bottom_frame->unmapChild(stop_button);
    }
}

int PXTkWaitDialog::handleEvent(int event)
{
  if (event==1)
      {
	setModality(PXTrue);
	setWaitCursor(PXTrue);
	if (this->debugMode==PXTrue)
	  fprintf(stderr,"Stop Event Captured\n");
	if (menuoption>=0)
	    return event_parent->handleEvent(menuoption);
      }

  return TCL_OK;
}

void PXTkWaitDialog::setDebugMode(PXBool debugmode)
{
  this->debugMode=debugmode;
}

