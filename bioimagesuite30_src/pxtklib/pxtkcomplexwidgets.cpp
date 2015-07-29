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









#include "pxtkcomplexwidgets.h"
#include <stdlib.h>


/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu

   see pxtkcomplexwidgets.h for details
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

/* -------------------------------------------------------------------------*/
/*                   PXGrArrowScale                                         */
/* -------------------------------------------------------------------------*/
 
PXTkArrowScale::PXTkArrowScale(PXTkEventObj* par,PXTkGadget* w,const char* wname,int moption,
			       PXBool labelabove,PXBool showentry):PXTkEventObj(par,w)
{
  menu_option=moption;
  label_above=labelabove;
  show_entry=showentry;
  label=NULL;
  initDisplay();
  setRange(0.0, 10.0);
  setValue(5.0);

  setDecimalPoints(0);
  setIncrement(1.0);
  setLengthWidth(70);
  setLabel(wname);
}

void PXTkArrowScale::setLabel(const char* wname)
{
  if (label_above)
      {
	scale->setLabel(wname);
      }
  else
      {
	scale->setLabel("");
	label->setText(wname);
      }
}

// ----------------------------------------------------------------
void PXTkArrowScale::setLengthWidth(int length,int width)
{
  if (!is_display_ok)
      return;

  if (length<20)      length=20;
  if (length>300)     length=300;

  if (show_entry)
    length-=15;

  scale->setLengthWidth(length,width);

}

void PXTkArrowScale::setRange(float min,float max)
{
  min_value=min;
  max_value=max;
  
  if (min>max)
      {
	min_value=max;
	max_value=min;
      }

  if (min==max)
      max+=0.0001;

  if (is_display_ok)
      scale->setRange(min,max);

}

void PXTkArrowScale::setIncrement(float inc)
{
  increment=inc;
}

void PXTkArrowScale::setValue(float vl,PXBool callback)
{
  if (!is_display_ok)
      return;

  if (vl<min_value)      vl=min_value;
  if (vl>max_value)      vl=max_value;
  
  scale->setValue(vl);
  if (callback)
      event_parent->handleEvent(menu_option);
}


void PXTkArrowScale::setDecimalPoints(int dp)
{
  if (!is_display_ok)
      return;

  if (dp<0) dp=0;
  if (dp>8) dp=8;

  
  float a=fabs(min_value);
  if (a<fabs(max_value))
      a=fabs(max_value);
  
  int dig=int(log(a)/log(10.0)+0.5)+dp;
  float res=pow(10.0,-(dig-1));


  //  fprintf(stderr,"dp=%d dig=%d res=%.2f\n",dp,dig,res);

  scale->setDigits(dig);
  scale->setResolution(res);

}

void PXTkArrowScale::setResolution(float res)
{
  if (is_display_ok)
      scale->setResolution(res);
}

float  PXTkArrowScale::getValue()       
{ 
  if (is_display_ok)
      return scale->getValue();
  else
      return 0.0;
}

// ----------------------------------------------------------------
int PXTkArrowScale::initDisplay()
{
  PXTkFrame* main=new PXTkFrame(this,gadget_parent);
  main_window=main;

  //main->configure("-relief","raised");
  main->configure("-bd ","2");

  if (!label_above)
    {
      label=new PXTkLabel(this,main,"Hello");
      label->configure("relief","ridge");
    }
  
  left=new PXTkDrawButton(this,main,1,101);
  scale=new PXTkScale(this,main,"val",100);
  right=new PXTkDrawButton(this,main,3,102);

  scale->setLengthWidth(100,10);
  scale->setValue(0.0);
  scale->configure("-bd",0);

  main->setPackMode(PXFalse);
  if (!label_above)
    main->addChildren("-row 0 -column 0 -sticky es",label);
  
  main->addChildren("-row 0 -column 1 -sticky s",left);
  main->addChildren("-row 0 -column 2 -sticky nsew",scale);
  main->addChildren("-row 0 -column 3 -sticky s",right);
  
  if (show_entry)
    {
      int id=scale->getObjectId();
      sprintf(pxtk_buffer,
	      "entry %s_e -textvariable widgetvar%d -width 6; grid %s_e -row 0 -column 4 -stick sw",scale->getWidgetName(),id,scale->getWidgetName());
      PXTkApp::executeTclCommand(pxtk_buffer);
    }

  main->gridExpandColumnConfigure(0,0);
  main->gridExpandColumnConfigure(1,0);
  main->gridExpandColumnConfigure(2,1);
  main->gridExpandColumnConfigure(3,0);

  is_display_ok=PXTrue;
  return TCL_OK;
     
}
// ----------------------------------------------------------------
int PXTkArrowScale::handleEvent(int n)
{
  float vl=getValue();

  switch(n)
      {
      case 102:
	setValue(vl+increment);
	break;
      case 101:
	setValue(vl-increment);
	break;
      case 100:
	// Round up or down
	break;
      }

  return event_parent->handleEvent(menu_option);
}

int PXTkArrowScale::setMenuOption(int m)
{
  menu_option=m;
  return TCL_OK;
}

PXTkScale* PXTkArrowScale::getScale()
{
  return scale;
}

int PXTkArrowScale::enable(PXBool enab)
{
  left->enable(enab);
  right->enable(enab);
  return scale->enable(enab);
  return TCL_OK;
}

/* -------------------------------------------------------------------------*/
/*                   PXTkSpinner                                         */
/* -------------------------------------------------------------------------*/


PXTkSpinner::PXTkSpinner(PXTkEventObj* par,PXTkGadget* w,const char* wname,int moption):PXTkEventObj(par,w)
{
  menu_option=moption;
  initDisplay();
  setRange(0.0,1.0);
  setValue(0.5);
  setDecimalPoints(1);
  setIncrement((float)0.1);
  label->setText(wname);
}
 
void PXTkSpinner::setRange(float min,float max)
{
  min_value=min;
  max_value=max;
  if (min_value>max_value)
      {
	min_value=max;
	max_value=min;
      }
  if (min_value==max_value)
      max_value+=1.0;

  if (current_value<min_value || current_value>max_value)
      setValue(current_value);
}

void PXTkSpinner::setIncrement(float inc)
{
  increment=inc;
}

void PXTkSpinner::setValue(float vl,PXBool callback)
{
  if (vl<min_value)
      vl=min_value;
  if (vl>max_value)
      vl=max_value;

  current_value=vl;
  if (is_display_ok)
      {
	switch(decimal_points)
	    {
	    case 1:
	      sprintf(pxtk_buffer2,"%4.1f",current_value);
	      break;
	    case 2:
	      sprintf(pxtk_buffer2,"%5.2f",current_value);
	      break;
	    case 3:
	      sprintf(pxtk_buffer2,"%6.3f",current_value);
	      break;

	    default:
	      sprintf(pxtk_buffer2,"%3.0f",current_value);
	      break;

	    }
	value_label->setText(pxtk_buffer2);
	
	if (callback)
	    event_parent->handleEvent(menu_option);
      }
}

void PXTkSpinner::setDecimalPoints(int dp)
{
  if (dp<0) dp=0;
  if (dp>3) dp=3;
  decimal_points=dp;
}

int PXTkSpinner::initDisplay()
{
  PXTkFrame* main=new PXTkFrame(this,gadget_parent);
  main_window=main;
  //main->configure("-relief","raised");
  main->configure("-bd ","2");


  label=new PXTkLabel(this,main,"Value");
  //label->configure("relief","ridge");

  left=new PXTkDrawButton(this,main,2,101);
  value_label=new PXTkLabel(this,main,"0.0");
  right=new PXTkDrawButton(this,main,0,102);
  
  main->setPackMode(PXFalse);
  main->addChildren("-row 0 -column 0 -sticky ens",label);
  main->addChildren("-row 0 -column 1 -sticky sw",left);
  main->addChildren("-row 0 -column 2 -sticky sn",value_label);
  main->addChildren("-row 0 -column 3 -sticky nw",right);


  is_display_ok=PXTrue;
  return TCL_OK;
}

int PXTkSpinner::handleEvent(int n)
{
  switch(n)
      {
      case 101:
	setValue(current_value-increment);
	break;
      case 102:
	setValue(current_value+increment);
	break;
      }
  return event_parent->handleEvent(menu_option);
}


int PXTkSpinner::enable(PXBool enab)
{
  if (!is_display_ok)
      return TCL_OK;
  left->enable(enab);
  return right->enable(enab);
}
 

/* --------------------------------------*/
/*   PXTkLabelFrame Class                */
/* --------------------------------------*/
 
PXTkLabelFrame::PXTkLabelFrame(PXTkEventObj* evpar,PXTkGadget* par,const char* label,int width,
			       int height):PXTkFrame(evpar,par,-1,-1,PXFalse)
{
  initializeLabelFrame();
  initDisplay(label);
  setDimensions(width,height);
}

int PXTkLabelFrame::initDisplay(const char *text)
{
  sprintf(pxtk_buffer,"LabelFrame:create %s -text \"%s\" -relief ridge",
	  widget_base_name,text);

  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  char* line=PXTkApp::getTclStringResult();

  delete [] widget_name;
  widget_name=new char[strlen(line)+1];
  strcpy(widget_name,line);

  if (a==TCL_OK)
      is_display_ok=PXTrue;
  return a;
}

int PXTkLabelFrame::initializeLabelFrame()
{
  static int num=0;
  num++;

  if (num==1)
      {
	static char pxtk_rlabelframe[] = "package require Iwidgets;proc LabelFrame:create { w args } {    iwidgets::Labeledframe $w -labelpos nw;     foreach { tag value } $args { 	switch -- $tag { 	    -text { $w configure -labeltext $value }	    -width { [ $w childsite ] configure -width $value } } } ;    return [ $w childsite ]}";
	return PXTkApp::executeTclCommand(pxtk_rlabelframe);
      }
  
  return TCL_OK;
}

/* --------------------------------------*/
/*   PXTkDrawButton Class               */
/* --------------------------------------*/
PXTkDrawButton::PXTkDrawButton(PXTkEventObj* evpar,PXTkGadget* gadg_par,
				 int md,int callno,PXBool dodisplay):PXTkButton(evpar,gadg_par," ",callno,dodisplay)
{
  mode=Irange(md,0,6);
  if (dodisplay)
      {
	initializeBimaps();
	initDisplay();
      }
}

int PXTkDrawButton::initDisplay()
{
  switch (mode)
      {
      case 0:
	configure("-image","$pxtk_arrow_up");
	break;
      case 1:
	configure("-image","$pxtk_arrow_left");
	break;
      case 2:
	configure("-image","$pxtk_arrow_down");
	break;
      case 3:
	configure("-image","$pxtk_arrow_right");
	break;
      case 4:
	configure("-image","$pxtk_stop_button");
	break;
      case 5:
	configure("-image","$pxtk_play_button");
	configure("-fg","black");
	configure("-bg","darkgray");
	break;
      case 6:
	configure("-image","$pxtk_record_button");
	configure("-fg","red");
	configure("-bg","white");
	break;

      }
  configure("-height",12);
  configure("-width",12);
  configure("-padx",3);
  configure("-pady",3);
  return TCL_OK;
}

int PXTkDrawButton::initializeBimaps()
{
  static int num=0;
  num++;

  if (num==1)
      {
	static char arrow_definitions1[] = "set pxtk_arrow_up [ image create bitmap -data { #define up_width 10\n #define up_height 10\n static unsigned char up_bits[] = {    0x30, 0x00, 0x30, 0x00, 0x78, 0x00, 0x78, 0x00, 0xfc, 0x00, 0xfc, 0x00,  0xfe, 0x01, 0xfe, 0x01, 0xff, 0x03, 0xff, 0x03;}}] ";

	static char arrow_definitions2[]="set pxtk_arrow_down [ image create bitmap -data { #define down_width 10\n #define down_height 10\n static unsigned char down_bits[] = {   0xff, 0x03, 0xff, 0x03, 0xfe, 0x01, 0xfe, 0x01, 0xfc, 0x00, 0xfc, 0x00,   0x78, 0x00, 0x78, 0x00, 0x30, 0x00, 0x30, 0x00;}}]";

	static char arrow_definitions3[]="set pxtk_arrow_right [ image create bitmap -data {#define right_width 10\n #define right_height 10\n static unsigned char right_bits[] = {   0x03, 0x00, 0x0f, 0x00, 0x3f, 0x00, 0xff, 0x00, 0xff, 0x03, 0xff, 0x03,   0xff, 0x00, 0x3f, 0x00, 0x0f, 0x00, 0x03, 0x00;}}]";

	static char arrow_definitions4[]="set pxtk_arrow_left [ image create bitmap -data { #define left_width 10\n #define left_height 10\n static unsigned char left_bits[] = {   0x00, 0x03, 0xc0, 0x03, 0xf0, 0x03, 0xfc, 0x03, 0xff, 0x03, 0xff, 0x03,   0xfc, 0x03, 0xf0, 0x03, 0xc0, 0x03, 0x00, 0x03;}}]";

	static char arrow_definitions5[]="set pxtk_stop_button [ image create bitmap -data {#define stop_width 10\n #define stop_height 10\n static unsigned char stop_bits[] = {   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x00, 0x78, 0x00, 0x78, 0x00,   0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00;}}]";

	static char arrow_definitions6[]="set pxtk_play_button [ image create bitmap -data {#define play_width 11\n #define play_height 11\n static unsigned char play_bits[] = {   0x00, 0x00, 0x04, 0x00, 0x1c, 0x00, 0x7c, 0x00, 0xfc, 0x01, 0xfc, 0x03,   0xfc, 0x01, 0x7c, 0x00, 0x1c, 0x00, 0x04, 0x00, 0x00, 0x00;}}]";

	static char arrow_definitions7[]="set pxtk_record_button [ image create bitmap -data {#define record_width 11\n #define record_height 11\n static unsigned char record_bits[] = {   0x00, 0x00, 0x20, 0x00, 0xf8, 0x00, 0xfc, 0x01, 0xfc, 0x01, 0xfe, 0x03,   0xfc, 0x01, 0xfc, 0x01, 0xf8, 0x00, 0x20, 0x00, 0x00, 0x00;}}]";

	PXTkApp::executeTclCommand(arrow_definitions1);
	PXTkApp::executeTclCommand(arrow_definitions2);
	PXTkApp::executeTclCommand(arrow_definitions3);
	PXTkApp::executeTclCommand(arrow_definitions3);
	PXTkApp::executeTclCommand(arrow_definitions4);
	PXTkApp::executeTclCommand(arrow_definitions5);
	PXTkApp::executeTclCommand(arrow_definitions6);
	return PXTkApp::executeTclCommand(arrow_definitions7);
      }
  else
      return TCL_OK;
}   

/* --------------------------------------*/
/*   PXTkDualArrowLabel Class            */
/* --------------------------------------*/


PXTkDualArrowLabel::PXTkDualArrowLabel(PXTkEventObj* evpar,PXTkGadget* gadg_par,const char* label,
				       int upcall,int downcall,int orient,PXBool dodisplay):PXTkLabel(evpar,gadg_par,label,PXFalse)
{
  orientation=orient>0;
  callback_up=upcall;
  callback_down=downcall;
  if (dodisplay)
      {
	initDisplay();
	sprintf(pxtk_buffer,"%s configure -text \"%s\" ",widget_name,label);
	PXTkApp::executeTclCommand(pxtk_buffer);
      }
}

int PXTkDualArrowLabel::initDisplay()
{

  sprintf(pxtk_buffer,"frame %s",widget_name);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);
  
  if (a!=TCL_OK)
      return TCL_ERROR;
      
  PXTkFrame* main=new PXTkFrame(event_parent,widget_name);
  
  int upmode=0,downmode=2;
  if (orientation==0)
      {
	upmode=3;
	downmode=1;
      }

  PXTkDrawButton* up=new PXTkDrawButton(event_parent,main,upmode,callback_up);
  PXTkLabel* lab=new PXTkLabel(event_parent,main," ");
  PXTkDrawButton* down=new PXTkDrawButton(event_parent,main,downmode,callback_down);
  
  if (orientation==0)
      main->addChildren("-side left -expand true -fill x",down,lab,up);
  else
      main->addChildren("-side top -expand true -fill y",up,lab,down);
  
  delete [] widget_name;
  widget_name=new char[strlen(lab->getWidgetName())+1];
  strcpy(widget_name,lab->getWidgetName());
  return TCL_OK;
}

