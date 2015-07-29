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









// pxtkcomplexwidgets.h
// -----------------------------------------------------------------

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXTk library 

   Xenios Papademetris May 2000 papad@noodle.med.yale.edu


_Module_Name : PXTkComplexWidgets


_Description : 

_Call : (examples)
  
_References : 

	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 12th May 2000

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PXTk_WIDGETS
#define _PXTk_WIDGETS

#include "pxtkgadgets.h"
#include "math.h"
#include "pxtkeventobj.h"

class PXTkDrawButton;

// ****************************************
// ******* PXTkArrowScale Class ************
// ****************************************

class PXTkArrowScale : public PXTkEventObj {
  
protected:
  
  PXTkDrawButton* left,*right;
  PXTkScale* scale;
  PXTkLabel* label;
  float    min_value,max_value,increment;
  int      menu_option;
  PXBool   label_above;
  PXBool   show_entry;

public:
  
  PXTkArrowScale(PXTkEventObj* par,PXTkGadget* w,const char* name,int moption,PXBool labelabove=PXTrue,
		 PXBool showentry=PXFalse);

  float     getmin()         { return (min_value); }
  float     getmax()         { return (max_value); }
  float     getincrement()   { return (increment); }


  void    setLabel(const char* label);
  float   getValue();

  PXTkScale* getScale();

  void    setRange(float min,float max);
  void    setIncrement(float inc);
  void    setLengthWidth(int len,int wid=-1);
  void    setValue(float vl,PXBool callBack=PXFalse);
  void    setDecimalPoints(int dp);
  void    setResolution(float res);

  int     enable(PXBool enab=PXTrue);

  virtual int initDisplay();
  virtual int handleEvent(int);   
  virtual int setMenuOption(int m);

};

// ****************************************
// ******* PXTkSpinner Class ************
// ****************************************

class PXTkSpinner : public PXTkEventObj {
  
protected:
  
  float      min_value,max_value,increment,current_value;
  int        decimal_points,menu_option;
  PXTkLabel* label,*value_label;
  PXTkDrawButton* left,*right;
public:
  
  PXTkSpinner(PXTkEventObj* par,PXTkGadget* w,const char* name,int moption);

  float     getValue()       { return current_value;  }

  void    setRange(float min,float max);
  void    setIncrement(float inc);
  void    setValue(float vl,PXBool callBack=PXFalse);
  void    setDecimalPoints(int dp);

  virtual int initDisplay();
  virtual int handleEvent(int);   

  int     enable(PXBool enab=PXTrue);
};

/* ******************************************
   *******  Class PXTkLabelFrame
   ****************************************** */

// Uses Iwidgets

class PXTkLabelFrame : public PXTkFrame {

public:

  PXTkLabelFrame(PXTkEventObj*,PXTkGadget* par,const char* label,int width=-1,int height=-1);

  virtual int initDisplay(const char* text);

private:
    static int initializeLabelFrame();
};

/* --------------------------------------*/
/*   PXTkDrawButton Class               */
/* --------------------------------------*/
class PXTkDrawButton: public PXTkButton
{
  
protected:
  int mode;
public:
  PXTkDrawButton(PXTkEventObj* evpar,PXTkGadget* gadg_par,int mode,int callno,
		  PXBool dodisplay=PXTrue);
  virtual int initDisplay();
  
private:
  static int initializeBimaps();
};

/* --------------------------------------*/
/*   PXTkDualArrowLabel Class            */
/* --------------------------------------*/

class PXTkDualArrowLabel : public PXTkLabel
{

protected:
  int callback_up,callback_down;
  int orientation;

public:
  PXTkDualArrowLabel(PXTkEventObj* objpar,PXTkGadget* gadg_par,const char* label,
		     int upcall,int downcall,int orient=0,PXBool dodisplay=PXTrue);
  virtual int initDisplay();
};   



#endif   



