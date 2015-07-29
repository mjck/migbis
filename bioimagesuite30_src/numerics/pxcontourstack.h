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

// pxcontourstack.h
// -----------------------------------------------------------------

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July papad@noodle.med.yale.edu

_Module_Name : PXContourStack 

_Description : A collection class which stores splines which are the XY cross-sections of a surface
               

_Call : 
  PXContourStack();
  PXContourStack(PXContourStack *old);

_References : see pxcontour.h
              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 19th December 1997

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PXContourStack
#define _PXContourStack
#include "pxcontour.h"
#include "pxgeometrystruct.h"
#include "bis_znzlib.h"

const int PXCONTOURSTACK_MAXLEVELS=1000;

class PXContourStack  {

  protected :

  PXContour* contour[PXCONTOURSTACK_MAXLEVELS];
  float      zlevel[PXCONTOURSTACK_MAXLEVELS];
  int numcontours;

  // ******************************************************************************
  //          PUBLIC INTERFACE
  // ******************************************************************************

  public :

  PXContourStack(PXContourStack *old,int zbetween,int mode);
  PXContourStack(PXContourStack* old,int zbetween,int mode,int closedbottom,int closedtop,
		 float apexdz=-1.0,float basedz=-1.0);
  PXContourStack(int numberofcontours=2);

  virtual ~PXContourStack();

  // [A] Access Contours
  // ------------------
  virtual int  getnumpoints();
  virtual int  getnumcontours()    { return numcontours; }
  virtual int   getzbetween(float zdist);

  // [B] Set / Get individual contours whole point sets
  // -------------------------------------------------
  virtual void copypoints(PXContourStack* other);
  virtual int  cleancopypoints(PXContourStack* other);
  virtual void initfromold(PXContourStack*,int zbetween,int mode);
  virtual PXContour*   getcontour(int i)  { return contour[indexbounded(i)];}
  virtual float        getzlevel(int i)   { return zlevel[indexbounded(i)];}

  virtual void setellipticalcylinder(int np=12,float rx=5.0,float ry=5.0,float cx=50.0,float cy=50.0);

  virtual void setcontour(int i,PXContour* spl);
  virtual void setzlevel(int i,float zl);
  virtual void shiftandscalez(float shift,float scale);
  virtual int  ensureAnticlockwise();
  virtual int  ensureClockwise();
  virtual int  setDirection(int dir=1); // + = Anti , -1 =Clock
  // [C] I/O Functions
  // -----------------------------
  virtual int Load(const char* filename);
  virtual int Save(const char* filename,int complevel=0);

  virtual int Save(const char* filename,float dist,float zdist,int mode=1,
		   int clbottom=0,int cltop=0);
  virtual int SaveIGES(const char* filename);
  virtual int SaveIGES(const char* filename,float dist,float zdist,int mode=1,
			 int clbottom=0,int cltop=0);
  
  virtual int SaveNuages(const char* filename,int complevel=0);
  virtual int SaveNuages(const char* filename,float dist,float zdist,int mode=1,
			 int clbottom=0,int cltop=0);
  virtual int SaveNuages(FILE* fout);

  virtual int SaveAbaqusRigid(const char* filename,int nodeoffset=200000,int index=1);

  virtual int LoadNuages(gzFile fin);
  virtual int LoadNuages(const char* filename);
  

  // [D] Alignment/Adjustment Operators
  // ----------------------------------
  virtual void  Equispace(float dist);
  virtual void  Equisample(int npoints);

  virtual void  selfalign();
  virtual int   aligntocontourstack(PXContourStack* other);   

  virtual void  Smooth(float sigma=1.0,int width=5);
  int nonShrinkSmooth(float alpha,float beta,int iterations);
  int nonShrinkSmooth2D(float ds,float sigma,int iterations);

  // [E] Stack Statistics
  // ---------------------------------
  virtual void getPositionAndOrientation(float& cx,float& cy,float& cz,
					 float& sx2,float& sy2,float& sxy,
					 float& vol);

  virtual float findnearestneighbour(float&x,float& y,float &z,float dt=0.01,int dbg=0);


  protected :
  virtual int   indexbounded(int v,int np=-1);

  static  int   indexcyclic(int v,int np);
  static  float getsqdist(float x1,float x2,float y1,float y2);

  public :
  static int aligncontours(PXContour* cntr1,PXContour* cntr2);
  static int align2basecontours(PXContour* cntr0,PXContour* cntr1,PXContour* cntr2);
  static int createstackset(PXContourStack** outputStacks,int totalnum=3,int bias=1);


};



#endif  

