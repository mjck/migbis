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

// pxfpbspline.h
// -----------------------------------------------------------------

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : PXFPBSpline

_Description : A class which stores control points
               for a controlled continuity spline of variable degrees
	       essentially a C++ Wrapper around routines in FITPACK
               FITPACK Routines used
	          1. clocur  -- fitting of closed curves
		  2. parcur  -- fitting of open curves
		  3. curev   -- evaluation of curve position
		  4. cualde  -- evaluation of curve derivatives

_Call : 
  PXFPBSpline();
  PXFPBSpline(PXFPBSpline *old);

_References : 
  Paul Dierckx "Curve and Surface Fitting with Splines" Oxford Science Publications
  De Boor      "A Practical Guide to Splines"
  Gerald Farin "Curves and Surfaces for Computer Aided Design", Academic Press
  
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 8th December 1997
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PXFPBSpline
#define _PXFPBSpline
class PXFloatImage;
class PXContour;


#include "pximage.h"
#include "bio_f2c.h"

class PX2DRegion;

class PXFPBSpline {

protected:

  // Spline Parameters 
  // -----------------
  integer  degree,numpoints,numknots,numcontrolpoints,dimension;
  real     *controlpoints,*knots;
  real     *controlx,*controly,*controlz;
  double** invMatrix;
  int      hasvectors,closedcurve,hasmatrices,matsize,hassnake;
  int      oldnumpoints;
  real     zlevel;

  // Snake Parameters
  // ----------------

  real         sn_imweight,sn_smweight,sn_usrtstep,sn_sigma,sn_maxgrad,sn_baloon;
  int          sn_numiter,sn_offsetx,sn_offsety,ownsenergymap,ownsgradients;
  PXFloatImage *sn_gradx,*sn_grady,*sn_energymap;

  // Debug Parameters
  // ----------------
  public :
  static   int      debugmode;
  static   real     lastfiterror;

  // ******************************************************************************
  //          PUBLIC INTERFACE
  // ******************************************************************************

  public :

  PXFPBSpline(int deg=3,int dimnsion=2,real zlevel=0.0,int closed=1);
  PXFPBSpline(PXFPBSpline *old);
  virtual ~PXFPBSpline();

  //  [A] Set and Delete Individual Control Points 
  // ---------------------------------------------
  virtual int     isclosed()            { return closedcurve; }
  virtual integer getdegree()           { return degree;}
  virtual integer getdimension()        { return dimension;}
  virtual integer getnumpoints()        { return numpoints;}
  virtual integer getnumknots()         { return numknots;}
  virtual integer getnumcontrolpoints() { return numcontrolpoints;}
  virtual int     splinehassnake()      { return hassnake;}
  virtual float*  getknotsarray()       { return (float*)knots;}

  virtual real getcontrolx(int i);
  virtual real getcontroly(int i);
  virtual real getcontrolz(int i);
  virtual real getrawcontrol(int i);
  virtual void setcontrol(int i,real x,real y,real z=0.0);
  virtual void shiftcontrol(int i,real dx,real dy,real dz=0.0);

  virtual void getcentroid(real& x,real&y, real& z,float ds=0.1);
  virtual real getknot(int i);
  virtual real getcknot(int i);
  virtual void setknot(int i,real t);
  
  virtual real getzlevel();
  virtual void setzlevel(real z);

  // [B] Manipulate Whole Spline
  // ---------------------------
  virtual void setcircle(int np,real radius=5.0,real x=50.0,real y=50.0,int dim=2);
  virtual void setellipse(int np,real radiusx=10.0,real radiusy=5.0,
			  real x=50.0,real y=50.0,int dim=2);

  virtual void rigid2D(float tx,float ty,float angle=0.0);
  virtual void rigid2D(float tx,float ty,float cx,float cy,float angle,int reverse=0);
  virtual void scaleshift2D(float tx,float ty,float sx,float sy);
 
  virtual int sample(int sm=2,float step=0.005);
  virtual int subsample(int sm=2,float step=0.005);
  virtual int resample(float ds_step=0.005,int npoints=-1);
  virtual int changespacing(real smth=0.5,int npoints=-1,float step=0.005,float csmooth=0.0,
			    int iterations=50);

  // [C] Add/Remove Points
  // ---------------------
  virtual real findnearestneighbour(real& x,real& y,real& mindist,int dbg=0);
  virtual real findnearestknot(int& index,real& dist,real x,real y,real z=0.0);
  virtual int  insertknot(real x,real y,real& mindist);
  
  // [D] Interface to PXContour and other Splines
  // --------------------------------------------
  virtual void copypoints(PXFPBSpline* other);
  virtual PXContour* storeincontour(int nodepoints=0);
  virtual PXContour* exporttoNcontour(int numpoints=10,int resample=1);
  virtual PXContour* exporttocontour(real dist=1.0);

  virtual void getknotpoints(real* xp,real* yp,int np);
  virtual void getknotpoints(real* xp,real* yp,real* zp,int np);

  // [E] Curve Fitting
  // -----------------
  virtual int  fittocontour(PXContour* cntr,real smooth=2.0,real csmooth=0.0,
			    int numpoints=-1,int closed=1,int iterations=100);
  virtual int  fitcontour(PXContour* cntr,real smooth=2.0,real csmooth=0.0,
			  int deg=-1,int cl=1,int uniform=0,int numpoints=10,int iterations=100);

  virtual void ResetPoints(int np,real* xpt,real* ypt);
  virtual void ResetPoints(int np,real* xpt,real* ypt,real *zpt);


  // [F] Location Evaluation
  // ------------------------------
  virtual real evalweight(real x, integer index);

  virtual void getposition(real s,real& x,real& y,real& z);
  virtual void getallderivatives(integer n,real  s,real* xd,real* yd,real* zd);
  virtual real getcurvature(real s);
 
  virtual void getposition(real    s,real& x,real& y);
  virtual void getderivative(real  s,real& x,real& y);
  virtual void getderivative(real  s,real& x,real& y,real &z);
  virtual void get2derivative(real s,real& x,real& y);
  virtual void get2derivative(real s,real& x,real& y,real& z);
  virtual real getlength(real s0,real s1,real ds_step=0.005);


  // [G] I/O Functions
  // -----------------------------
  virtual int Save(const char* filename);
  virtual int Save(FILE* fout);
  virtual int Load(const char* filename);
  virtual int Load(gzFile fin);
  virtual int SaveContour(const char* filename,real dist=1.4);

  // [H] Snake Related Functions
  // ---------------------------
  virtual void createsnake(PXFloatImage* potential,float img_coeff=5.0,
			   float smooth_coeff=0.5,
			   float timestep=0.05,float sigma=1.0,int niter=3,float baloon_coeff=0.0);

  virtual void createsnake(PXImage* pimage,float img_coeff=5.0,
			   float smooth_coeff=0.5,
			   float timestep=0.05,float sigma=1.0,int niter=3,float baloon_coeff=0.0);
  virtual void createsnake(PX2DRegion* region,
			   float img_coeff=5.0,
			   float smooth_coeff=0.5,
			   float timestep=0.05,float sigma=1.0,int niter=3,float balooon_coeff=0.0);

  virtual void createsnake(PXImage* pimage,PX2DRegion* region,
			   float img_coeff=5.0,float region_weight=2.0,
			   float smooth_coeff=0.5,
			   float timestep=0.05,float sigma=1.0,int niter=3,float baloon_coeff=0.0);

  virtual void createsnake(PXImage* pimage,PXFloatImage* external,
			   float img_coeff=5.0,
			   float ext_coeff=5.0,
			   float smooth_coeff=0.5,
			   float timestep=0.05,float sigma=1.0,int niter=3,float baloon_coeff=0.0);

  virtual void createsnake(PX2DRegion* region,PXFloatImage* external,
			   float img_coeff=5.0,
			   float ext_coeff=5.0,
			   float smooth_coeff=0.5,
			   float timestep=0.05,float sigma=1.0,int niter=3,float baloon_coeff=0.0);

  virtual void createsnake(PXImage* pimage,PX2DRegion* region,PXFloatImage* external,
			   float img_coeff=5.0,float region_weight=2.0,
			   float ext_coeff=5.0,
			   float smooth_coeff=0.5,
			   float timestep=0.05,float sigma=1.0,int niter=3,float baloon_coeff=0.0);


  virtual void removesnake(); 
  virtual real calcpotential(PXImage* pimage,int fullimage=0);
  virtual real calc3potential(PXImage* pimage,PX2DRegion *region,PXFloatImage* ext,
			      float w_im,float w_wr,float w_e,
			      int fullimage=0);
  virtual real generategradients();

  virtual PXFloatImage*  getenergymap();
  virtual unsigned char* getenergytexturemap(float gamma=1.0,int numbytes=1);
  virtual real updatesnake(real tolerance=0.05);
  
  // [I] Ensure Anticlockwise
  // ------------------------
  virtual int ensureAnticlockwise();

  // [J] Find Intersection with plane 
  // --------------------------------
  virtual real findIntersectY(real y,real minx=-100000.0,real maxx=100000.0,
			      real ds=0.005,real tol=0.02,int maxatt=20);
  virtual int  findIntersectX(real x,real& s1,real& s2,
			      real ds=0.005,real tol=0.02,int maxatt=20);


  // Move dual controlpoints
  // -----------------------
  virtual void setClosedControl(int i,real x,real y,real z=0.0,int off=1);
  virtual void shiftClosedControl(int i,real dx,real dy,real dz=0.0,int off=1);
  
 protected:
  virtual void ResetUniformKnots();

  // Recuurence Relationship
  // -----------------------
  virtual integer evalbspl(real x,real* h,integer l=-1);
  virtual real    evalweight(real* h,integer l, integer index);

  

  // Snake Part II
  // -------------
  virtual void  controlexternalforce(real& fx,real& fy,int k);
  virtual void  controlbaloonforce(real& fx,real& fy,int k);
  virtual int   checkbounds(int j,int x0,int x1,int y0,int y1,int tol=5);
  virtual real  splineinternalenergy(int k1,int k2);
  virtual void  splinechangeinternalenergy(int j,float ds,real &sx,real& sy);

  // Other Stuff
  // -----------
  virtual void  generatecontrolvectors();
  virtual int   genmatrices(int msize=-1);

  virtual int   fitpoints(integer M,integer MX,real* X,float smth);
  virtual int   fitpoints(integer M,real* W,real* U,integer MX,real* X,
			  integer IOPT,integer IPAR,float smth,int numpoints=10);
  
  virtual int   indexbound(int t,int np=-1,int off=0);
  virtual int   indexcyclic(int t,int np=-1,int off=0);
  virtual real  arclengthcyclic(real t);


  virtual void setdegree(integer deg=3);
  virtual void setclosed(int c);


public:
  // Debugging Functions
  // -------------------
  static int   getdebugmode() { return debugmode;}
  static void  setdebugmode(int d) { debugmode=(d>0);}
  static float getlastfiterror()   { return lastfiterror; }


  
};





#endif   

