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





#include "pxtkinfodialog.h"
#include "pxtkgadgets.h"
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu

   see pxtkinfodialog.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

/* -------------------------------------------------------------------------*/
/*                   PXTkInfoDialog                                          */
/* -------------------------------------------------------------------------*/

/* -------------------------------------------------------------------------*/
PXTkInfoDialog::PXTkInfoDialog(const char* title,int x,int y):PXTkDialog((PXTkGadget*)NULL,title)
{
  height=y;   width=x; 
}
/* -------------------------------------------------------------------------*/
int PXTkInfoDialog::initDisplay()
{
  create_inside_parent=PXFalse;
  PXTkToplevel *dlg=new PXTkToplevel(this,gadget_parent,dlg_title,width,height);
  main_window=dlg;
  dlg->hide();
  modality=PXFalse;  
  is_display_ok=PXTrue;
   
  PXTkFrame* manager=(PXTkFrame*)(main_window);

  PXTkFrame* top_frame=new PXTkFrame(this,manager,width,height-70);
  PXTkFrame* bottom_frame=new PXTkFrame(this,manager,width,70);

  manager->addChildren("-side bottom -expand false -fill x  -pady 2",bottom_frame);
  manager->addChildren("-side top -expand true -fill both",top_frame);

  PXTkScrollbar* ybar=new PXTkScrollbar(this,top_frame,PXTrue);
  PXTkScrollbar* xbar=new PXTkScrollbar(this,manager,PXFalse);
  manager->addChildren("-side bottom -expand false -fill x",xbar);
  
  text_box=new PXTkText(this,top_frame);
  ybar->attachGadget(text_box);
  xbar->attachGadget(text_box);

  text_box->configure("-width",150);
  text_box->configure("-height",15);
  //  text_box->configurequote("-font","{times 10}");
  text_box->configure("-wrap","none");
  text_box->configure("-relief","ridge");
  text_box->setEditableState(PXFalse);


  top_frame->addChildren("-side right -expand false -fill y",ybar);
  top_frame->addChildren("-side left -expand true -fill both",text_box);

  //  bottom_frame->addChildren("-side left -expand true -fill y",ybar);

  PXTkButton *close_button=new PXTkButton(this,bottom_frame,"Close",3); 
  PXTkButton *clear_button=new PXTkButton(this,bottom_frame,"Clear",101); 
  PXTkCheckButton *edit_button=new PXTkCheckButton(this,bottom_frame,"Edit",102); 
  edit_button->setState(PXFalse);

  bottom_frame->addChildren("-side left -fill x -padx 10",close_button,clear_button,edit_button);
  
  addText("PXTk Console\n--------------------\n");
  return TCL_OK;
}
// -------------------------------------------------------------------------
int PXTkInfoDialog::addText(const char *line)
{
  if (is_display_ok)
      return text_box->addText(line);
  else
      return 0;
}
// -------------------------------------------------------------------------
int PXTkInfoDialog::clearText()
{
  if (is_display_ok)
      return text_box->clearText();
  else
      return 0;
}
// -------------------------------------------------------------------------
int PXTkInfoDialog::handleEvent(int event)
{
  if (event==3)
      {
	this->popdownDialog();
      }
  if (event==101)
      {
	this->clearText();
      }
  if (event==102)
      {
	PXTkCheckButton* bt=(PXTkCheckButton*)PXTkApp::getIntArgument(0);
	if (bt!=NULL)
	    text_box->setEditableState(bt->getState());
      }
  return TCL_OK;
}

PXTkText* PXTkInfoDialog::GetTextBox()
{
  return text_box;
}

