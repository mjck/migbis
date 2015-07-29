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









#include "pxtkseldialog.h"

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July 1995   papad@noodle.med.yale.edu

   see pxgrseldialog.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

/* -------------------------------------------------------------------------*/
/*                   PXGrSelDialog                                          */
/* -------------------------------------------------------------------------*/

/* -------------------------------------------------------------------------*/
PXTkSelDialog::PXTkSelDialog(PXTkEventObj* objpar,PXTkGadget* w, int n):PXTkDialog(objpar,w,"Option Dialog",PXFalse)
{
  max_num_options=Irange(n,1,20);
  num_options=max_num_options;
  menu_option=-1;
  debug_option=-1;
  label_width=15;
  entry_width=8;
  pack_step=1;
  debugbutton=NULL;
  show_debug=0;
}

PXTkSelDialog::~PXTkSelDialog()
{
  if (is_display_ok)
    {
      delete message;
      for (int i=0;i<num_options;i++)
	{
	  delete label[i];
	  delete entry[i];
	}
    }
}

/* -------------------------------------------------------------------------*/

int PXTkSelDialog::handleEvent(int n)
{
  switch (n) 
    {
    case 1:   // OK
      popdownDialog();
      if (has_event_parent && menu_option>0)
	  event_parent->handleEvent(menu_option);
      break;

    case 2 : // Cancel
      popdownDialog();
      if (has_event_parent && menu_option>0)
	  event_parent->handleEvent(menu_option+1);
      break;

    case 3 : // Debug
      for (int i=0;i<num_options;i++)
	  {
	    sprintf(pxtk_buffer,"Value of Field %d (%s)\n",i,getoption(i));
	    PXTkApp::printToConsole(pxtk_buffer);
	  }
      if (has_event_parent && menu_option>0)
	  event_parent->handleEvent(menu_option+2);
      break;
    }
  return TCL_OK;
}
/* -------------------------------------------------------------------------*/
int PXTkSelDialog::setNumOptions(int newnumoptions)
{
  if (!is_display_ok)
    return num_options;

  if (isOpen())
    return num_options;

  newnumoptions=Irange(newnumoptions,1,max_num_options);
  if (newnumoptions==num_options)
    return num_options;


  if (num_options<newnumoptions)
    {
      for (int i=num_options;i<newnumoptions;i++)
	{
	  optionFrame->gridRowColumnChild(label[i],this->getGridRow(i),this->getGridColumn(i));
	  optionFrame->gridRowColumnChild(entry[i],this->getGridRow(i),this->getGridColumn(i)+1);
	}
    }
  else
    {
      for (int i=newnumoptions;i<num_options;i++)
	{
	  optionFrame->unmapChild(label[i]);
	  optionFrame->unmapChild(entry[i]);
	}
    }
  
  num_options=newnumoptions;
  return num_options;
}

int PXTkSelDialog::initDisplay()
{
  int odd=0;
  int pr=num_options;
  if (pr>8)
      {
	if (2*int(num_options/2)<num_options) 
	    odd=1;
	else
	    odd=0;
	height=190+(int((odd+num_options)/2)*45);
	width=280;
      }
  else
      {
	height=190+num_options*45;
	width=220;
      }

  PXTkDialog::initDisplay();
  PXTkFrame* manager=(PXTkFrame*)(main_window);
  manager->setPackMode(PXTrue);
  sprintf(pxtk_buffer,"%d %d,",width,height);
  manager->wmconfigure("maxsize",pxtk_buffer);
  manager->wmconfigure("minsize",pxtk_buffer);

  PXTkFrame *desc=new PXTkFrame(this,manager,width,70);
  desc->configure("-relief","sunken");

  PXTkFrame *main=new PXTkFrame(this,manager,width,height-140);
  main->configure("-relief","raised");

  PXTkFrame *bot=new PXTkFrame(this,manager,width,70);

  manager->addChildren("-fill both -expand true -padx 2 -pady 2",desc,main,bot);
  
  message=new PXTkMessage(this,desc,"Description");
  message->configure("-relief","ridge");
  message->configure("-width",width);
  
  desc->addChildren("-fill x -expand true",message);

  pack_step=1;
  if (pr>8) 
      pack_step=2;

  optionFrame=new PXTkFrame(this,main);
  optionFrame->configure("-width",width);
  optionFrame->configure("-height",height-140);
  optionFrame->setPackMode(PXFalse);

  main->addChildren("-fill x -expand true",optionFrame);
  for (int i=0;i<num_options;i+=pack_step)
      {
	for (int k=0;k<pack_step;k++)
	    {
	      int ind=i+k;
	      if (ind<num_options)
		{
		  label[ind]=new PXTkLabel(this,optionFrame,"Desc");
		  label[ind]->configure("-width",15);
		  optionFrame->gridRowColumnChild(label[ind],this->getGridRow(ind),this->getGridColumn(ind));
		  
		  entry[ind]=new PXTkEntry(this,optionFrame);
		  entry[ind]->configure("-width",8);
		  entry[ind]->configure("-relief","sunken");
		  entry[ind]->setText("0.0");
		  optionFrame->gridRowColumnChild(entry[ind],this->getGridRow(ind),this->getGridColumn(ind)+1);
		}
	    }
      }

  PXTkButton*     ok=new PXTkButton(this,bot,"OK",1);
  PXTkButton* cancel=new PXTkButton(this,bot,"Cancel",2);
  debugbutton=new PXTkButton(this,bot,"Info",3);
  bot->addChildren("-fill x -expand f -side left",ok);
  bot->addChildren("-fill x -expand f -side right",cancel);
  bot->addChildren("-fill x -expand f -side left",debugbutton);
  if (show_debug==0)
    bot->unmapChild(debugbutton);
  return TCL_OK;

}

/* -------------------------------------------------------------------------*/

int PXTkSelDialog::getGridRow(int optionid)
{
  return int(optionid/pack_step);
}
// ----------------------------------------------------------
int PXTkSelDialog::getGridColumn(int optionid)
{
  if (pack_step==1)
    return 0;
  
  int row=int(optionid/2);
  if (row*2 == optionid)
    return 0;
  
  return 2;
}
// ----------------------------------------------------------
// Parameter Setup
// ----------------------------------------------------------

void PXTkSelDialog::setfoption(int i,const char* lab,const char* fmt,float val)
{ 
  if (!is_display_ok)
    return;


  if (i>=0 || i<num_options)
      {
	label[i]->setText(lab);
	sprintf(pxtk_buffer,fmt,val);
	entry[i]->setText(pxtk_buffer);
      }
}

void PXTkSelDialog::setoption(int i,const char* lab,const char* fmt,const char* val)
{      
  if (!is_display_ok)
    return;

  if (i>=0 || i<num_options)
      {
	label[i]->setText(lab);
	sprintf(pxtk_buffer,fmt,val);
	entry[i]->setText(pxtk_buffer);
      }
}

void PXTkSelDialog::setioption(int i,const char* lab,const char* fmt,int val)
{      
  if (!is_display_ok)
    return;


  if (i>=0 || i<num_options)
      {
	label[i]->setText(lab);
	sprintf(pxtk_buffer,fmt,val);
	entry[i]->setText(pxtk_buffer);
      }
}
// ----------------------------------------------------------

char* PXTkSelDialog::getoption(int i)
{ 
  if (is_display_ok && i>=0 && i<num_options)
      return entry[i]->getText();
  else
      return NULL;
}

float PXTkSelDialog::getfoption(int i)
{ 
  char *line=getoption(i);
  if (line!=NULL)
      return atof(line);
  else 
      return 0;
}

int PXTkSelDialog::getioption(int i)
{ 
  char *line=getoption(i);
  if (line!=NULL)
      return atoi(line);
  else 
      return 0;
}
// ----------------------------------------------------------

int PXTkSelDialog::getNumOptions()
{
  return num_options;
}

void PXTkSelDialog::setDescription(const char* description)
{
   if (!is_display_ok)
    return;

 if (isOpen())
    return;
 
 message->setText(description);
}
  
void PXTkSelDialog::setTitleDescription(const char* title,const char* description)
{
  this->setTitle(title);
  this->setDescription(description);
}



void PXTkSelDialog::setMenuOption(int m)
{
   if (!is_display_ok)
    return;

 if (isOpen())
    return;
 
 menu_option=m;
}

void PXTkSelDialog::setDebugButton(const char* label,int option)
{
   if (!is_display_ok)
    return;

   debugbutton->setText(label);
   debug_option=option;
}

void PXTkSelDialog::hideDebugButton()
{
   if (!is_display_ok)
    return;

   ((PXTkFrame*)(debugbutton->getGadgetParent()))->unmapChild(debugbutton);
}

void PXTkSelDialog::showDebugButton()
{
   if (!is_display_ok)
    return;

   ((PXTkFrame*)(debugbutton->getGadgetParent()))->addChildren("-fill x -expand f -side left",debugbutton);
}

