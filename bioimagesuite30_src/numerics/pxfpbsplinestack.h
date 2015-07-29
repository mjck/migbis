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


//
//
//
//
//---------------------------------------------------------------------------------------------------------

// pxfpbsplinestack.h
// -----------------------------------------------------------------

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July papad@noodle.med.yale.edu

_Module_Name : PXFPBSplineStack 

_Description : A collection class which stores splines which are the XY cross-sections of a surface
               

_Call : 
  PXFPBSplineStack();
  PXFPBSplineStack(PXFPBSplineStack *old);

_References : see pxfpbspline.h
              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 19th December 1997

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PXFPBSplineStack
#define _PXFPBSplineStack
#include "pxfpbspline.h"
#include "pxtriangulatedstack.h"
#include "pxgeometrystruct.h"

const int PXFPBSPLINESTACK_MAXLEVELS=600;

class PXFPBSplineStack  {

protected:

  PXFPBSpline* spline[PXFPBSPLINESTACK_MAXLEVELS];
  int numsplines,trim_bottom,trim_top,hasapex;
  PX3Vector apex;

  

  // ******************************************************************************
  //          PUBLIC INTERFACE
  // ******************************************************************************

 public:

  PXFPBSplineStack(int numberofsplines=4);
  PXFPBSplineStack(PXFPBSplineStack *old);
  PXFPBSplineStack(PXFPBSplineStack *old,int zbetween,int mode=1,
		   int resample_original=1);
  PXFPBSplineStack(PXFPBSplineStack *old1,PXFPBSplineStack* old2,float ratio,
		   float smooth=2.0,int doapex=0);

  virtual ~PXFPBSplineStack();

  // [A] Access Splines
  // ------------------
  virtual int  gettrimbottom() { return trim_bottom;}
  virtual int  gettrimtop()    { return trim_top;}
  virtual void settrimbottom(int tb);
  virtual void settrimtop(int tt);
  virtual void autosettrims(float zmin,float zmax);
  virtual int  getnumsplines()    { return numsplines; }

  virtual void setapex(float x,float y,float z) { apex.x[0]=x; apex.x[1]=y; apex.x[2]=z;}
  virtual PX3Vector* getapex() { return &apex;}
  virtual void enableapex(int enable=1) { hasapex=enable>0; }
  virtual int  apexenabled() { return hasapex; }
  virtual void autosetapex(int active=1);

  // [B] Find Nearest Point
  // ----------------------
  virtual float findnearestneighbour(float&x,float& y,float &z,float dt=0.01,int dbg=0);

  // [C] Set Original Shape
  // ----------------------
  virtual void setellipticalcylinder(int np,int numslices,float radiusx,float radiusy,float x,float y,
				     float zscale=1.0,float bottomz=0.0);
  virtual void setfromboundingbox(int np,float zfactor,
			  float bx,float by,float bz,float bw,float bh,
			  float tx,float ty,float tz,float tw,float th);

  // [D] Set / Get individual splines whole point sets
  // -------------------------------------------------
  virtual void copypoints(PXFPBSplineStack* other,int ignoretrim=0);
  virtual PXFPBSpline*   getspline(int i)  { return spline[indexbounded(i)];}
  virtual void setspline(PXFPBSpline* spl,int ind);


  virtual int sample(int sm=2,float step=0.05);
  virtual int subsample(int sm=2,float step=0.05);
  virtual int resample(float ds_step=0.05,int npoints=-1);
  virtual int changespacing(real smth=1.0,int npoints=-1,float step=0.05);

  // [E] I/O Functions
  // -----------------------------
  virtual int Save(const char*  filename);
  virtual int Save(FILE* fout);
  virtual int Load(const char*  filename);
  virtual int Load(gzFile fout);

  // [F] Interface to Older Code
  // ---------------------------

  virtual int fittocontourstack(PXContourStack* stack,float smooth=5.0,float csmooth=0.5,
				int npoints=-1,int closed=1);


  virtual PXContourStack* exporttocontourstack(real dist);
  virtual PXContourStack* exporttocontourstack(real dist,real zdist,int mode=1);
  virtual PXContourStack* exporttocontourstack(real dist,int zbetween,int mode=1);

  virtual PXContourStack* polarexporttocontourstack(real middlex,float zspacing=2.0,
						    float bottomz=-1000.0,float topz=-1000.0);
  
  virtual float getpolarbottomz();
  virtual float getpolartopz();
  virtual int polarpoint(float zl,int slice,real middlex,int pass,real& x,real& y);
  virtual void adjustnumberofsplines(int newnumber);

protected:

  virtual void adjusttrimmed();
  virtual int  indexbounded(int v,int np=-1);
  static  int  indexcyclic(int v,int np);
  static  real getsqdist(float x1,float x2,float y1,float y2);


public:
  static  PXContour* compareSplines(PXFPBSpline* spl1,PXFPBSpline* spl2,int np=20);
  static  int   debugmode;
  static  void  setdebugmode(int d) { debugmode=(d>0);}


  static int  equitrimstacks(PXFPBSplineStack* stack1,PXFPBSplineStack* stack2,int trims[4]);
  static void untrimstacks(PXFPBSplineStack* stack1,PXFPBSplineStack* stack2,int trims[4]);
  static int  createstackset(PXFPBSplineStack* newstacks[],
			     PXFPBSplineStack *stack1,PXFPBSplineStack *stack2,
			     int numstacks=1,int NP=25);
  static int  temporallySmoothApex(PXFPBSplineStack**  stacks,int numstacks,
				   int areacheck,float sigma,int closed);

};



#endif  

