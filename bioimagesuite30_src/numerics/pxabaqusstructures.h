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

// pxabaqusstructures.h 
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July papad@noodle.med.yale.edu


_Module_Name : pxabaqusstructures.h

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
#ifndef _PXAbaqusstructures
#define  _PXAbaqusstructures

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "pxgeometrystruct.h"
#include "pxshapetrackingclass.h"
// ---------------------------------------------------
//  Basic Data Stuctures 
// ---------------------------------------------------

struct PXNodeAuxiliary
{
  int index[2];
  float dx[3];
  float temperature;
  PXIntegerList* neighbours;
};

class PXSolidNodes 
{
protected:
  
  int numnodes;
  PXTriangulatedStackPoint* points;
  PXNodeAuxiliary* pointDetails;

public:
  
  PXSolidNodes(int numnodes);
  PXSolidNodes(PXSolidNodes* oldnodes);
  PXSolidNodes(PXSolidNodes* oldnodes1,PXSolidNodes* oldnodes2);
  virtual ~PXSolidNodes();

  void init(int numnodes);
  void cleanup();

  int getnumnodes();
  PXTriangulatedStackPoint* getpoint(int pointidx);
  PXNodeAuxiliary* getdetails(int pointidx);

  // Further utility functions
  int getindex(int point,int index);
  void setindex(int point,int index,int val);
  PXIntegerList* getneighbours(int point);


  virtual void  settemperature(int node,float val=0.0);
  virtual float gettemperature(int node);

  // Old Code
  // -------
  void setfromnodecollection(PXNodeCollection* oldnodes,int scale=-1);
  void Copy(PXSolidNodes* other,int check=1,int offset=0);

  // I/O Stuff
  int  Save(FILE* fout);
  int  Save(const char* fname);
  int  Load(gzFile fout);
  int  Load(const char* fname);

  
};

// ---------------------------------------------------

class PXNodalDisplacements 
{
protected:

  int endoindex,epiindex;
  int *valid;
  PX3Vector* disp;
  float* conf;
  PXSolidNodes* solidNodes;

public:
  PXNodalDisplacements(PXSolidNodes* nodes,int endo=0,int epi=100);
  PXNodalDisplacements(PXNodalDisplacements* other);
  virtual ~PXNodalDisplacements();

  void init(int num);
  void cleanup();

  PX3Vector* getdisplacement(int i);
  float      getdispcomponent(int i,int j);
  float getconfidence(int i);

  void setdisplacement(int i,PX3Vector* vec);
  void setdisplacement(int i,float x,float y,float z);
  void setdispcomponent(int i,int j,float x);
  
  void setconfidence(int i,float cnf);

  int           getvalid(int i);
  int           getendoindex();
  int           getepiindex();
  PXSolidNodes* getsolidnodes();

  int  setfromshapetrackingclass(PXShapeTrackingClass* shapeTr,int index,float tolerance=0.05);
  int  setfromshapetrackingclassnosearch(PXShapeTrackingClass* shapeTr,int index);
  int  loadPengChengDisplacementsSingle(const char* endofile,const char* epifile,float tolerance=0.05);
  int  loadDisplacementsSingle(const char* file,int all=0);
  void Copy(PXNodalDisplacements* other);

  // I/O Stuff
  int  Save(FILE* fout);
  int  Save(const char* fname);
  int  Load(gzFile fout);
  int  Load(const char* fname);

};
// ---------------------------------------------------
class PXAbaqusMaterialModel {
  
protected:
  int modeltype;
  int nlgeom;
  int active;
  int numparameters;
  float *parameter,heatcoeff[3];

public:
  PXAbaqusMaterialModel(int mode);
  PXAbaqusMaterialModel(PXAbaqusMaterialModel *other);
  virtual ~PXAbaqusMaterialModel();

  void init(int mode);
  void cleanup();
  
  void setgeometry(int nl=1);
  int  getgeometry();

  void setactive(int av=1);
  int  getactive();

  int   getmodeltype();
  float getparameter(int i);

  float getheatcoeff(int i);
  void setheatcoeff(float a=-0.01,float b=0.03,float c=-0.02);

  void setHyperelastic(float incompressibility=20.0);
  void setElastic(float Modulus=1.0,float Poisson=0.45);
  void setTransverseIsotropy(float Ep=1.0,float vp=0.4,float Et=3.5,float vtp=0.4);

  void setfrommaterialmodel(PXMaterialModel* old);
  void Copy(PXAbaqusMaterialModel* other);

  void SaveAbaqus(FILE* fout,const char* name);
  
  int Save(const char* fname);
  int Save(FILE* fout);
  int Load(const char* fname);
  int Load(gzFile fin);

};
// ---------------------------------------------------

class PXAbaqusOutputData {

protected:
  int                       numnodes,numelements;
  int                       numpies,numsectors,numslices,numtransmural;
  PX3Vector                 junctionpoint;

  PX3Vector                 *nodalDisplacements; 
  float                     *strain[12];
  float*                    elementVolumes;
  PX3Vector*                elementCentroids;
  PX3Vector*                radial,*circum;
  int*                      pieindex;

  int                       outputStepNo,outputIncNo;

public:
  PXAbaqusOutputData(int numnodes,int numelements);
  PXAbaqusOutputData(PXAbaqusOutputData* other);
  virtual ~PXAbaqusOutputData();

  void       cleanup();
  void       init(int numno,int numel);
  void       Copy(PXAbaqusOutputData* other);

  float      getvolume(int el);

  float      getstraincomponent(int el,int str);
  void       setstraincomponent(int el,int str,float v);

  float      getdispcomponent(int node,int comp);

  int        getnumelements();
  int        getnumnodes();

  PX3Vector* getdisplacement(int nd);
  PX3Vector* getcentroid(int el);
  PX3Vector* getradial(int el);
  PX3Vector* getcircumeferential(int el);

  int        getpieindex(int el);
  int        copypieindex(PXAbaqusOutputData* other);
  int        getnumpies();
  int        getnumsectors();
  int        getnumslices();
  int        getnumtransmural();
  PX3Vector* getjunctionpoint();

  int getoutputstep();
  int getoutputinc();


  void setRadial(int el,float x,float y,float z);
  void setCircum(int el,float x,float y,float z);
  void setCentroid(int el,float x,float y,float z);
  void setNodeInfo(int nodeno,float du,float dv,float dw);
  void setElementInfo(int elno,int mode,float evol,
		      float e11,float e22,float e33,
		      float e12,float e13,float e23);

  int  getElementPieIndex(int elno);
  void setElementPieIndex(int elno,int pieindex);
  void setPieStats(int nump=1,int nunse=1,int numsl=1,int numt=1);
  void setOutputStepInc(int os,int oi);


  void converttoprincipal();

  int saveNodeDetails(const char* fname);
  int loadNodeDetails(const char* fname);

  int savePieDivision(const char* fname);
  int loadPieDivision(const char* fname);

  int Save(const char* fname,int nodirections=1);
  int Load(const char* fname,int rotate=1,int check=1,int integrate=0);
  int LoadOld(const char* fnamestem,int rotate=1);

  int rotatestrains();
  int subdivideinpies(float junctionx=0.0,float junctiony=0.0,float junctionz=0.0,
		      int numslices=3,int numsectors=8,int anticlockwise=1,int nump=-1);
  int averagestrains(PXAbaqusOutputData *ref);
  int modifyFromDisplacements(PXSolidNodes* nodes,int inner,int outer,
			      PX3Vector* dispinner,PX3Vector* dispouter);

  
  int combineWithOtherScale(PXAbaqusOutputData* other,float scaleother,float scalethis);

  static int getsectorno(float angle,float phase_ref,int numsectors,int mode=0);
};
// ---------------------------------------------------


#endif

