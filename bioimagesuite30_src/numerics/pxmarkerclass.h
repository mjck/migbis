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

// pxmarkerclass.h 

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : pxmarkerclass.h

_Description : Various Utility Routines for keeping track of Markers

	       	       

_Call : 

     
_References : 
              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in Sep 8th 1999
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
#ifndef _PXMarkerclass
#define  _PXMarkerclass

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "pxcontour.h"
#include "pxgeometrystruct.h"

// ---------------------------------------------------
// Marker Related Stuff
// ---------------------------------------------------
class PXClickedPoints {
  
protected:
  PXContour* cntrxy;
  PXContour* cntrzt;

public:
  
  PXClickedPoints();
  ~PXClickedPoints();

  void getcentroid(float& x,float& y,float& z,float max=-1.0);
  void addpoint(float x,float y,float z,float intensity,int remove=0);
  void clearpoints();
  void removelast();

  PXContour* getpositionxy();
  PXContour* getpositionzintensity();
};


class PXMarker {

protected:

  int numframes;
  int markerclass;
  PX3Vector *position;
  PXClickedPoints* points;
  char name[13];
  
public:

  PXMarker(int numframes);
  ~PXMarker();
  
  void setposition(int frame,float x,float y,float z);
  void setposition(int frame,PX3Vector* v);
  void setclass(int markclass);
  int getclass();
  PX3Vector* getposition(int frame);
  float  getcomponent(int frame,int comp);

  int getnumframes();
  PXClickedPoints* getclickedpoints(int frame);

  void setname(const char* name);
  char* getname();
};
 

class PXMarkerCollection {
protected:
  PXMarker* marker[400];
  int nummarkers;

public:
  PXMarkerCollection(int nummarkers,int numframes);
  ~PXMarkerCollection();

  PXMarker* getmarker(int markerno);
  int getnummarkers();

  int scalemarkerz(float zx=0.5);

  int loadmarkercollection(const char* fname);
  int loadmarkercollectiondon(const char* fname,int numframes=16);
  int savemarkercollection(const char* fname);
};


#endif

