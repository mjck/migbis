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





// pxtkscale.cpp
#include "pxtkscale.h"
#include "pxtkeventobj.h"
#include "math.h"

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000   papad@noodle.med.yale.edu
   
   see pxtkscale.h for more details

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- */

/* ********************************************************************

    Definition of PXTkScale Class 

 ********************************************************************* */

PXTkScale::PXTkScale(PXTkEventObj* evpar,PXTkGadget* gadg_par,const char* label,int moption,
		     PXBool horizontal,PXBool dodisplay):PXTkGadget(evpar,gadg_par)
{
  if (dodisplay)
      {
	call_no=moption;
	initDisplay();
	setLabel(label);
	setHorizontalOrient(horizontal);
      }
}
  
int PXTkScale::setLabel(const char* label)
{
  return (configurequote("-label",label));
}


float PXTkScale::getValue()
{
  char* line=getWidgetVariable();
  if (line==NULL)
    {
      fprintf(stderr,"NULL scale::getValue %s widgetvar%d\n",widget_name,object_id);
      return 0.0;
    }
  return atof(line);
}

/******************************************************************************/

float PXTkScale::getMin()
{
  return atof(cget("-from"));
}

/******************************************************************************/

float PXTkScale::getMax()
{
  return atof(cget("-to"));
}

/******************************************************************************/

int PXTkScale::setValue(float val,PXBool callback)
{
  char line[20];
  sprintf(line,"%f",val);
  char* ll=setWidgetVariable(line);
  if (ll!=NULL)
      {
	if (callback && call_no>0)
	    return event_parent->handleEvent(call_no);
	else
	    return TCL_OK;
      }
  else
      return TCL_ERROR;
}

int PXTkScale::setRange(float min,float max)
{
  configure("-from",min);
  return configure("-to",max);
  //return setResolution(fabs(max-min)/100.0);
}

int PXTkScale::setLengthWidth(int length,int width)
{
  if (width!=-1)
      configure("-width",width);
  return configure("-length",length);
}

int PXTkScale::showValue(PXBool show)
{
  if (show)
      return configure("-showValue","true");
  else
      return configure("-showValue","false");
}

int PXTkScale::setResolution(float res)
{
 if (res==-1.0)
   {
     float min=atof(cget("-from"));
     float max=atof(cget("-to"));
     if (fabs(max-min)>1e-6)
       res=(fabs(max-min))/50.0;
     else
       res=1.0;
   }
  return configure("-resolution",res);
}

int PXTkScale::setTickInterval(float tickInt)
{
  if (tickInt==-1.0)
    {
      float min=atof(cget("-from"));
      float max=atof(cget("-to"));
      if (fabs(max-min)>1e-6)
	tickInt=(fabs(max-min))/2.0;
      else
	tickInt=1.0;
    }
  return configure("-tickInterval",tickInt);
}

int PXTkScale::setDigits(int digits)
{
  return configure("-digits",digits);
}

int PXTkScale::setHorizontalOrient(PXBool orient)
{
  if (orient)
      return configure("-orient","horizontal");
  else
      return configure("-orient","vertical");
}

int PXTkScale::initDisplay()
{
  setWidgetVariable("0");

  sprintf(pxtk_buffer,"scale %s -variable widgetvar%d ",widget_name,object_id);
  int a=PXTkApp::executeTclCommand(pxtk_buffer);

  if (call_no>0)
      {
	sprintf(pxtk_buffer,"bind %s <ButtonRelease> { pxtkcallback %ld %d %ld $widgetvar%d}",
		widget_name,event_parent->getHandlerNo(),call_no,(long)this,object_id);


	a=PXTkApp::executeTclCommand(pxtk_buffer);
      }

  if (a==TCL_OK)
      is_display_ok=PXTrue;
  return a;

}

