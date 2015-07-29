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

// pxchamferdistancemap.h
// -----------------------------------------------------------------

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris  papad@noodle.med.yale.edu


_Module_Name : 1. PXChamferDistanceMap

_Description : 
               
	       
	       
_Call : 
        
_References : Computational Geometry, Preparata and Shamos              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in November 14th 1997
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PX_CHAMFERDISTANCEMAP
#define _PX_CHAMFERDISTANCEMAP

#include "pxutil.h"
#include "px3dimage.h"
#include "pxcontour.h"

class PXChamferDistanceMap {
  PXImage* image;
  PXContour* contour;
  int offsetx,offsety;
  int subpixel,hasmap;
  int min_x,max_x,min_y,max_y;
  
public:
  // Global Control Parameters
  static short pixel_size;
  static short obj_gray,out_gray;
  static short margin;
  
  // --------------------------
  // Functions
  // --------------------------
  PXChamferDistanceMap(PXContour* cntr,int auto_mode=1,
		       int subpixel=1,int imsizex=-1,int imsizey=-1);


  PXChamferDistanceMap(PXContour* cntr,int auto_mode,
		       int subpixel,PXChamferDistanceMap* clone);


  PXChamferDistanceMap(PXContour* cntr1,PXContour* cntr2,
		       int fraction1=1,int fraction2=1);

  PXChamferDistanceMap(PXContour* spl0,PXContour* spl1,
		       PXContour* spl2,PXContour* spl3,
		       float spc=0.5);

  PXChamferDistanceMap(PXChamferDistanceMap* map1,PXChamferDistanceMap* map2,
		       int fraction1=1,int fraction2=1,int imsizex=-1,int imsizey=-1);
  PXChamferDistanceMap(PXChamferDistanceMap* map0,PXChamferDistanceMap* map1,
		       PXChamferDistanceMap* map2,PXChamferDistanceMap* map3,
		       float spc=0.5); 
  ~PXChamferDistanceMap();


  void generateMap(int filtermode=1,int perturb=1,float per_amount=0.1);
  int  hasMap() { return hasmap; }
  
  void init(int auto_mode,int subpixel,int imsizex,int imsizey);
  void lininterpolate(PXChamferDistanceMap* map1,PXChamferDistanceMap* map2,
		      int fraction1=1,int fraction2=1,int imsizex=-1,int imsizey=-1);
  void catsplineinterpolate(PXChamferDistanceMap* map0,PXChamferDistanceMap* map1,
			    PXChamferDistanceMap* map2,PXChamferDistanceMap* map3,
			    float fr=0.0);
  void setlevels(short obj=32767,int short=-32767);
  void saveImage(const char* fname,int binary=1);

  short getvalue(int i,int j);
  int getminx() { return min_x;}
  int getminy() { return min_y;}
  int getmaxx() { return max_x;}
  int getmaxy() { return max_y;}
  int getoffsetx() { return offsetx;}
  int getoffsety() { return offsety;}

  PXContour* extractContour();
  PXImage*   getDistanceMap() { return image; }
  PXImage*   getEnergyMap();

  void chamferfilter();

protected:

  int findexit(int i,int j,float xin,float yin,
	       float& xout,float& yout,int& iout,int& jout,int debug=0);
  int  validcell(int i,int j,int debug=0);
  
  void initial_contour(int perturb=1,float pert_dist=0.1,int debug=0);
  int  initial_outside(int x=1,int y=1);
  void initializemembers();


};

#endif   

