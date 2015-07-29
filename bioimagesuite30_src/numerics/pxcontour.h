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

// pxcontour.h
// -----------------------------------------------------------------

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : 1. PXContour

_Description : A class which stores contour with options for
               Subsampling and forming closed contour
	       Smoothing and Sampling and Equispacing
	       
_Call : 
  PXContour();
  PXContour(PXContour *oldcontour);
        
_References : Computational Geometry, Preparata and Shamos              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 25th Sep 1995
                    2. Added Centroid + Moment Calculation 2nd Oct 1995
		    3. Added Sampling and Smoothing Options 6th Oct 1995
		    4. Provision for isolated points-draw  16th Oct 1995
		    5. Conversion to floating point to allow greater accuracy
		    6. Added equispacing and length calculation
                       also potential for normal/curvature storage
		    7. Change load/save Format to conform to IPAG standards 
		       (i.e. no header) 11th Jul 1996
		    8. Chenged Load/Save Stack to read PengCheng's stack format 11/07/96
		    9. Tidied-up 2nd Sep 1996
		   10. Added SaveFloatStack to save stack as floats
		   11. Added area calculation for closed contours
		       (4th November 1996 -- from Francois code)
		   12. Fixed Touching function 13th Nov 1997 (JC)
		   13. Fixed Load function to be less crashable
		   14. Eliminated `filename' and added a copypoints routine (1/14/98)
		   15. Eliminated all references to contour stack, created new pxcontourstack class
		       5th Feb 1998
		   16. Created new anticlockwise function, ensure contour is anticlockwise
		   17. Added cache and sub-pixel search 21st April 1998
		   18. Added non-shrinking smoothing
		   19. Added 3D Option to Contour
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PX_CONTOUR
#define _PX_CONTOUR

#include <string.h>
#include <math.h>
#include <stdio.h>
#include "bis_znzlib.h"

class PXImage;

struct PXContourPoint {
  float        x;
  float        y;
  PXContourPoint* previous;
  PXContourPoint* next;
};

struct PXContourGrad {
  float          x;          // Normal componenets (x,y)
  float          y;
  float          k;          // Curvature k
  PXContourGrad* previous;
  PXContourGrad* next;
};




class PXContour {

  // ************** Class Fields *************************************

  protected :
  unsigned char  status,closed,isolated,gradient,contourhascache;
  unsigned short numpoints;
  PXContourPoint *lastpoint,*toppoint,*realnextpoint;
  PXContourGrad  *lastgradient,*topgradient;
  float *px,*py;

  // ******************************************************************************
  //          PUBLIC INTERFACE
  // ******************************************************************************

 public:

  PXContour();
  PXContour(PXContour *oldcontour);
  virtual ~PXContour();

  // [A] General Contour Status
  // ---------------------------

  virtual int  isvalid()     { return status;}
  virtual void suspend()     { status=0;}
  virtual void reinstate()   { status=1;}

  virtual void closedcontour(unsigned char flag)    
      { closed=flag;}

  virtual void setclosed(unsigned char flag)
      { closedcontour(flag);}
  virtual unsigned char isclosed()
    {return closed;}

  virtual void isolatedcontour(unsigned char flag)
    { isolated=flag;}
  virtual unsigned char isisolated()
    {return isolated;}

  virtual void gradientcontour(unsigned char flag)
    { gradient=flag;}
  virtual unsigned char hasgradient()
    {return gradient;}

  virtual unsigned char hascache() { return contourhascache;}

  // [B] Contour Parameters
  // ----------------------

  virtual unsigned short  getnumpoints()   {return numpoints;}
  virtual PXContourPoint* gettoppoint()   {return toppoint;}
  virtual PXContourPoint* getlastpoint()  {return lastpoint;} 
  virtual PXContourGrad*  gettopgradient() {return topgradient;}
  virtual float*          getcachex()      {return px;}
  virtual float*          getcachey()      {return py;}
  
  //[C] Add Remove Point functions
  //------------------------------

  virtual int  ensureAnticlockwise();
  virtual int  ensureClockwise();
  virtual int  setDirection(int dir=1); // + = Anti , -1 =Clock
  virtual void setcircle(int np=10,float radius=5.0,float x=50.0,float y=50.0);
  virtual void AddP(unsigned long xx,unsigned long yy);
  virtual void Add(float xx,float yy);
  virtual void AddSpline(double** x,int n,int np);
  virtual void RemovePoint();
  virtual void DeleteAll();

  virtual void AddGrad(float xx,float yy,float kk);
  virtual void RemoveGrad(); 
  virtual void DeleteGrad();

  
  //[D]Contour Manipulation
  //------------------------
  /*
    Sample    - averages width points to  get one 
    Subsample - adds maxratio points between each pair of points
    Touching  - subsamples contour until points are touching
    Moments   - finds moments of contour
    */



  virtual void  copypoints(PXContour *oldcontour);
  virtual void  CalculateGradients(int scale);

  virtual void  Smooth(float sigma=1.0,int width=5);
  virtual void  nonShrinkSmooth(float sigma=1.0,int iterations=10,
				float alpha=0.33,float beta=-0.34,int width=5);
  virtual void  Sample(int width);
  virtual void  Subsample(int maxratio);
  virtual void  Touching();

  virtual void  Equispace(float spacing);
  virtual void  Equisample(int npoints);

  virtual float findnearestneighbour(float&x,float& y,float dt=0.01,int dbg=0);

  virtual void  generateCache(int forceClosed=1);
  virtual void  cleanupCache();


  // [E] I/O Related Routines
  // ------------------------
  virtual int  Load(const char* filename);
  virtual int  Save(const char* filename);
  virtual void SaveStream(FILE* fout,int in2d=1,int opencurve=1,float z=0.0);
  virtual int  SaveInteger(const char* filename,int scale=1);
  virtual int  Save4Mesh(const char* filename);
  virtual int  SaveIGES(const char* filename,float z=0.0);

  // [F] Check Inside or Outside 
  // ---------------------------
  virtual int   IsInside (float testpointx,float testpointy);

  // [G] Contour Statistics
  // ----------------------
  virtual float getarea();  
  virtual float getcontourlength();
  virtual void  getcentroid(float& x,float& y);
  virtual void  calcStats(float& mean,float& max,float& min,int& nsegm,int opencurve);
  virtual void  getmajoraxis(float& nx,float& ny);
  virtual void  getextent(float &xmin,float& xmax,float& ymin,float& ymax);
  virtual void  getextrema(int direction,int min,float& x,float& y);

  virtual void  getdualcentroid(PXContour* other,float& x,float& y);
  virtual void  getdualmajoraxis(PXContour* other,float& nx,float& ny);
  virtual void  getmajoraxis(float* x,float* y,int np,float& nx,float& ny);

  // ******************************************************************************
  //          IMPLEMENTATION SPECIFIC
  // ******************************************************************************

  virtual void  copy2vector(double* x,double *y,int& np);
  virtual void  copy2vector(float* x,float *y,int& np);

  protected :

  virtual int   getcardinal(int ii,int np,unsigned char cl);
  virtual int   getcyclicindex(int pt,int np);
  virtual int   IsInside (float testpointx,float testpointy,
			  float *pointx,float* pointy,
			  int numberofpoints);
public:
  static float Break_const;
  static  int   Intersect (float x1,float y1, 
			   float x2,float y2, 
			   float x,float y); 
  static int  saveContourPair(const char* fname,
			      float* px1,float* py1,
			      float* px2,float* py2,
			      int np1,int np2,
			      int* used,int* corr);
  static int  cyclicDistance(int p1,int p2,int np);
  static int  saveContourPair(const char* fname,PXContour* cntr1,PXContour* cntr2);

  static PXContour*  mapContours(PXContour* cnt1,PXContour* cntr2,int resample=1);

  
};
#endif   /* _PX_CONTROUR */

