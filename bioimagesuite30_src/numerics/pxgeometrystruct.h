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

// pxgeometrystruct.h 

/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : pxgeometrystruct.h

_Description : Various Utility Routines for Geometrical Entities like points,neighbours,triangles,
               tetrahedra

	       
	       

_Call : 

     
_References : 
              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in May 22nd 1995
#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */
#ifndef _PXGeometryStruct
#define  _PXGeometryStruct

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "bis_znzlib.h"

// -------------------------------------------
//  Geometry Classes
// -------------------------------------------

class PX3Vector {

public:
  float x[3];
  
  PX3Vector();
  PX3Vector(float a1,float a2,float a3);
  void setvector(float a1,float a2,float a3);
  float getlength();
  float getdistance(PX3Vector* other);
  void  copy(PX3Vector* other);
  float normalize();
  
  float dotwith(PX3Vector* other);
  PX3Vector* crosswith(PX3Vector* other);

  void crosswithoverwrite(PX3Vector* other);
  void subtract(PX3Vector* other,float factor=1.0);
  void add(PX3Vector* other,float factor=1.0,float thisfactor=1.0);
  
};

struct PX2Vector {
  float x[2];
  PX2Vector();
  PX2Vector(float a1,float a2);
  void setvector(float a1,float a2);
  float getlength();
  float getdistance(PX2Vector* other);
  void copy(PX2Vector* other);
  float normalize();
  
  float dotwith(PX2Vector* other);
  PX2Vector* crosswith(PX2Vector* other);

};


// ---------------------------------------------------
//  Basic Data Stuctures 
// ---------------------------------------------------
struct PXTriangulatedStackPoint {
  int   index;   
  float x[3];
  float nx[3];
  float k[2];
};

struct PXNeighbours {
  int  num;
  int* pt;
};

struct PXNodeCollection {
  int numnodes;
  int   *stackno,*pointno;
  PX3Vector* x;
  PX3Vector* dx;
  PX2Vector* k;
};

struct PXDispPair {
  float x1,y1,z1,x2,y2,z2;
};

struct PXDisplacementCollection {
  int numframes;
  int numdisplacements;
  int *index;
  float *x,*y,*z;
  float *conf1,*conf2;
  float *mag;
};

struct PXElementOutputInfo {
  int numelement;
  float e11,e12,e13,e22,e23,e33,evol;
};

struct PXMaterialModel {
  int modeltype;
  int nlgeom;
  int numparameters;
  float *parameter;
};

// Accessory Structures for on-line processing
// -------------------------------------------
struct PXElementOutput
{
  unsigned char flag;
  float evol;
  float e11,e22,e33,e12,e13,e23;
};

struct PXNodeOutput
{
  unsigned char flag;
  float estu1,estu2,estu3;
  float currentu1,currentu2,currentu3;
  unsigned char cache;
  float nx,ny,nz;
};

struct PXTetrahedron {
  int index;
  int p[4];
};

struct PXTriangle {
  int   index;
  float c[3];
  int   p[3];
};


struct PXIntegerNode {
  int value;
  PXIntegerNode* next;
  PXIntegerNode* previous;
};

// ---------------------------------------------------
// Collection Classes
// ---------------------------------------------------

class PXIntegerList {

protected:
  
  PXIntegerNode    *topnode,*lastnode,*iterationnode;
  int  numnodes;


public:
  PXIntegerList();
  ~PXIntegerList();
  void init();
  
  void  Rewind();
  void  Next(); // circular 
  void  Add(int pt);
  void  AddNonDuplicate(int pt,int exclude=-32767);
  void  DeleteAll();
  void  Copy(PXIntegerList* other);
  void  Copy(PXTriangle* tri);
  int   CopyTo(PXTriangle* tri);

  void Sort(PXIntegerList* linked=NULL);

  // Comparisons
  int  isSame(PXIntegerList *other);
  int  isSamePresorted(PXIntegerList* other);
  int  numberinCommon(PXIntegerList* other);
  int  numberinCommon(int* points,int numpoints);

  void  eliminateDuplicates(int exclude=-32767);
 
  PXIntegerNode*  getcurrentnode();
  int   getcurrent();
  int   getnumnodes();
};


struct PXNeighboursNode {
  int  index;
  PXIntegerList*    points;
  PXNeighboursNode* next;
  PXNeighboursNode* previous;
};


class PXNeighboursList {

protected:
  
  PXNeighboursNode *topnode,*lastnode,*iterationnode,*olditerationnode;
  int  numnodes;

  int  hascache,*c_index,c_numnodes,*c_flag,*c_buffer;
  PXNeighbours* c_neighbours;

public:
  PXNeighboursList();
  PXNeighboursList(PXNeighbours* neighbours,int numneighbours);
  PXNeighboursList(PXIntegerList* lists,int numlists);
  ~PXNeighboursList();

  void init();
  
  void  Rewind();
  void  GotoEnd();
  void  Next(); // circular 
  void  Add(PXNeighbours* element,int externalindex);
  void  Add(PXIntegerList* list,int externalindex);
  void  Add2(int p1,int p2,int externalindex);
  void  Add3(int p1,int p2,int p3,int externalindex);
  void  AddN(int *p,int np,int externalindex);
  void  Add(PXNeighboursNode* node);
  void  DeleteAll();
  void  RemoveInvalid();
  void  Sort();

  void  Copy(PXNeighboursList* other);
  void  storeIterationPointer();
  void  popIterationPointer();
  
  PXNeighboursNode* getcurrentnode()  { return iterationnode;}

  int  getcurrentindex();
  int  getnumnodes();    

  int  Save(FILE* fout);
  int  Save(const char* fname);
  int  Load(gzFile fout);
  int  Load(const char* fname);

  // Connectivity Code, uses Cache
  // -----------------------------
  int  tagconnectedto(int p,int maxrelationship=1000);  
  int  getcentroid();
  int  getrelationship(int i,int j,int max=1000);
  int  cacheIndexConversion(int index);
  int  getcacheindex(int i);
  void updateIndicesFromCache();
  void updateCacheIndices();

protected:
  void generateCache();
  void cleanupCache();

  
};


#endif

