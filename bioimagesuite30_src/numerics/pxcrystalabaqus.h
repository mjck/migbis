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

// pxCrystalAbaqus.h
// -----------------------------------------------------------------
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July papad@noodle.med.yale.edu

_Module_Name : PxCrystalAbaqus 

_Description : A structures which stores crystal coordinates and generates Abaqus files for
               strain computation
               

_Call : 
  PxCrystalAbaqus();
  PxCrystalAbaqus(PXContourStackSolid *old);

_References :
              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 16th March 1999

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PxCrystalAbaqus
#define _PxCrystalAbaqus
#include "pxgeometrystruct.h"
#include "pxabaqusstructures.h"
#include "pxabaqussolid.h"
#include "pxmarkerclass.h"

const int PXCRYSTALFRAMES=200;
const int PXABCR_PT_OFFSET=1;

class PXCrystalAbaqus {
  
protected:

  PXNeighboursList*         elements;
  PXSolidNodes*             nodes;
  PXSolidNodes*             extraNodes;
  int                       numframes,numgroups;

  // Finite Element Data Postprocessing
  PXAbaqusOutputData*      femData;
  PXNodalDisplacements*    displacements[PXCRYSTALFRAMES];

  // Verify Existence
  int                       hasElements,hasNodes,hasDisplacements,hasFemData,hasExtraNodes;

public:
  
  PXCrystalAbaqus();
  PXCrystalAbaqus(PXCrystalAbaqus* old);
  PXCrystalAbaqus(const char* fname1,const char* fname2);
  PXCrystalAbaqus(PXMarkerCollection* markers);
  PXCrystalAbaqus(PXMarkerCollection* markers,int frameinc=1,int maxframe=-1);
 
  virtual ~PXCrystalAbaqus();
  void init();
  void cleanup();

  void Copy(PXCrystalAbaqus* other);

  int createFromCrystalFiles(const char* classfile,const char* positionfile,
			     float minvol=50.0,float minangle=15.0,int extranodes=0);
  
  // Access Functions
  // ----------------
  int      hasfemdata();
  int      hasdisplacements();
  int      hasnodes();           
  int      hasextranodes();           
  int      haselements();        

  int      getnumframes();
  int      getnumgroups();

  PXAbaqusOutputData*   getfemdata();
  PXNeighboursList*     getelements();
  PXSolidNodes*         getnodes();   
  PXSolidNodes*         getextranodes();   
  PXNodalDisplacements* getdisplacements(int i);

  // Geometry Related
  // ----------------
  void getElementCentroid(PXIntegerList* element,float&x,float&y,float& z);
  int  saveInventor(const char* fname,int mode=-1,int cmode=-1,float* params=NULL);
  int  saveInventor(FILE* fout,int mode=1,int cmode=-1,float* params=NULL);

  void savedistances(const char* fname1,const char* fname2,int crystal1,int crystal2);

  // CrystalAbaqusRelated
  // -------------
  void setorientations(int group,PX3Vector& radial,PX3Vector& circum,PX3Vector& longit);
  int  saveAbaqus(const char* fname,int resout=1,int numframes=-1,int skipframes=1);
  int  saveSolid(const char* fname);
  void setPieIndices();
  // Allocation Related
  // ------------------
  void allocateFemData();
  void allocateDisplacements();


  // Qhull stuff
  // -----------
  float tetravolume(int* points);
  void  getElementCentoid(PXIntegerList* elem,float& x,float& y,float& z);
  float tetraminangle(int* points);
  float trianglearea(int* points);
  void  doqhull(int* points,int numpoints,int group);
  void  saveqhull(const char* fname,int* points,int numpoints);
  void  readqhull(const char* fname,int* points,int numpoints,int group);
  void  checkelements(float minvol,float minangle,int start=0,int end=-1);


  // Marker Solids
  // -------------
  int createTagMarkerSolid(float avevol=30.0,float anrange=30.0);
  int createMarkerSolid(const char* fname);
  int createOrientations(PX3Vector* orient1,PX3Vector* orient2,PX3Vector* orient3,PXAbaqusSolid* sld);
  int saveMarkerAbaqus(const char* fname,int resout,PXAbaqusSolid* sld,int useorientations=1);
  

protected:

  // Cleanup Operations
  // ------------------
  void cleanupFemData();
  void cleanupNodeSpace();
  void cleanupElementSpace();
  void cleanupDisplacements();

  // More Allocation Stuff
  // ---------------------
  void allocateNodeSpace(int numnode);
  void allocateElementSpace();

  
  

public:

  // Static Functions mainly to do with postprocessing fem data
  // ----------------------------------------------------------
  static int debugmode;  
  
};

  
  
#endif  

