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

// pxAbaqusSolid.h
// -----------------------------------------------------------------
/* #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- 

   This file is part of the PXx library 

   Xenios Papademetris July papad@noodle.med.yale.edu

_Module_Name : PxAbaqusSolid 

_Description : A structures which stores a triangulated StackSolid
               

_Call : 
  PxAbaqusSolid();
  PxAbaqusSolid(PXContourStackSolid *old);

_References :
              
	      
_I/O : 

_System : Unix
_Remarks : 
 
_Author : X. Papademetris papad@noodle.med.yale.edu
_Revisions History: 1. Key in 14th August 1998

#-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#- #-#-#-#-  */

#ifndef _PxAbaqusSolid
#define _PxAbaqusSolid
#include "pxgeometrystruct.h"
#include "pxabaqusstructures.h"
#include "pxtriangulatedstack.h"


const int ABAQUSSOLIDMAXLEVELS=20;
const int ABAQUSSOLIDMAXFRAMES=30;
const int PXABAQUSOFFSET=1000;

class PXAbaqusSolid {
  
protected:

  // Original Inputs 
  PXTriangulatedStack*      tstack[2];

  // Geometrical Parameters 
  int                       elementtype,elementmode,hasmidwallnodes,hasmidwallelements;
  int                       numlayers,numpointscontour;
  PXNeighboursList*         elements,*springElements;
  PXSolidNodes*             nodes,*springNodes;

  // Mechanical  Model Parameters
  PXAbaqusMaterialModel*    material;
  float                     springMultiplier;

  // Boundary Conditions
  float                     *volume;
  int                       hasVolumes,numvolumes;

  // Numerical Solution
  float                     outputStep;
  int                       outputInc;
  int                       enforcementMode,orientationMode;

  // Finite Element Data Postprocessing
  PXAbaqusOutputData*      femData;

  // Verify Existence
  int                       hasElements,hasNodes,hasMaterial,hasStacks;
  int                       hasFemData,hasSpringElements,hasSpringNodes;

public:
  
  PXAbaqusSolid();
  PXAbaqusSolid(PXAbaqusSolid* old);
  PXAbaqusSolid(PXTriangulatedStack* stack1,PXTriangulatedStack* stack2);
  PXAbaqusSolid(const char* fname1,const char* fname2);
 
  virtual ~PXAbaqusSolid();
  void init();
  void cleanup();

  void Copy(PXAbaqusSolid* other);

  // Access Functions
  // ----------------
  int      getelementtype(); 
  int      getelementmode(); 
  int      getnumlayers();
  int      getnumpointscontour();

  float    getoutputstep();  
  int      getoutputinc();   
  int      getenforcementmode(); 
  int      getorientationmode(); 
  float    getspringmulitplier();

  int      hasstacks();
  int      hasfemdata();
  int      hasnodes();           
  int      haselements();        
  int      hasspringnodes();        
  int      hasspringelements();     

  int      hasmaterial();        
  int      hasvolumes();
  float*   getvolumes();
  int      getnumvolumes();

  PXAbaqusOutputData*  getfemdata();
  PXNeighboursList*    getelements();
  PXNeighboursList*    getspringelements();
  PXSolidNodes*        getnodes();   
  PXSolidNodes*        getspringnodes();   
  PXAbaqusMaterialModel* getmaterial();
  PX3Vector*           getnodeoutput();

  PXTriangulatedStack* getstack(int i);

  // Geometry Related
  // ----------------
  int generateTetrahedralSolid(float spacing,int bias);
  int generateHexahedralSolid(int nodespercontour,int totalnumstacks,int bias,float sample,int skipslices=0);
  int  saveInventor(const char* fname,int mode=-1,int cmode=-1,float* params=NULL);
  int  saveInventor(FILE* fout,int mode=1,int cmode=-1,float* params=NULL);

  // I/O Related
  // -----------
  int Save(const char* fname);
  int Save(FILE* fout);
  int Load(const char* fname);
  int Load(gzFile fin);

  // Element Stuff
  // -------------
  void getdirections(PXIntegerList* element,
		     float& nx1,float& ny1,float& nz1,
		     float& nx2,float& ny2,float& nz2,
		     float& nx3,float& ny3,float& nz3,
		     int movetoepi=1);

  void rotateFibreAngle(float& nx1,float& ny1,float& nz1,
			float& nx2,float& ny2,float& nz2,
			float  nx3,float  ny3,float  nz3,
			float angle); 

  int  rotateStrainsFiberToCardiac(int inverse=0);

  float getFibreAngle(float z,int layer,float bottomz=1.5,float topz=36.0);
  void  getElementCentroid(PXIntegerList* element,float&x,float&y,float& z);
  int   getforcingnode(int nodeno,int offset=1000);

  // Stacks and Nodes Connection
  // ---------------------------
  int modifyStacksFromNodes();
  int updateCurvaturesFromStacks();
  int updateFromOutputStructures(int position=0);
  int correctForIncormpressibility(PXAbaqusOutputData* baseData);
  int updateTemperaturesFromDisplacements(PXNodalDisplacements* disp,int mode=0);
  int setCurvaturesFromDenserStacks(PXTriangulatedStack* denseInner,PXTriangulatedStack* denseOuter);
  void setorientationmode(int m=0); 
  void sethybridmode(int h=0); 

  // FemData Related
  // ---------------
  int updateFemDataDirections(int centroidsonly=0);
  
  // AbaqusRelated
  // -------------
  void setSpringParameters(int enfMode=1,float springk=0.5);
  void setOrientationMode(int ormode=0);
  void setNumericalParameters(float stepSize=1.0,int numInc=40);
  int  insertKinematicConstraints(FILE* fout,int mode=0);
  int  fixApexorBase(FILE* fout,int md=4);
  int  CreateJobBase(FILE* fout,const char* includefile,int springmode);
  int  CreateStepHeader(FILE* fout);
  int  CreateStepFooter(FILE* fout,int resout=0,int fieldout=0);
  int  CreateSteps2DEcho(FILE* fout,int numframes,int resout);
  int  CreateStepsPassive3D(FILE* fout,int numframes,int resout,int flattopbottom);
  int  CreateStepsActive3D(FILE* fout,int numframes,int resout,int flattopbottom);
  int  CreateStepsSinglePass(FILE* fout,int numframes,int springmode,int resout,int flattopbottom);

  int  saveAbaqus(const char* fname,int resout=1,int numframes=-1,const char* includefile=NULL,int flattopbottom=0);
  int  saveAbaqusFortran(const char* fortranfile,const char* jobname,
                         const char* solidname,const char* endostem,const char* epistem,const char* priorstem,
			 int curvmode=-1,float lamda=0.5,int frameoffset=0);


  // Volume Stuff
  // --------------
  int  generateVolumes(const char* fnameendo,const char* fnameepi,int end,int begin=1);
  int  makeIncompressibleLL();


  // Allocation Related
  // ------------------

  void allocateMaterial();
  void allocateFemData();
  void allocateSpringSpace(int order=0);

  // Location in solid related (for 8 element ordered solids)
  // --------------------------------------------------------
  int getnodecoordinates(int node,int& layer,int& slice,int& point);
  int getelementcoordinates(int element,float& layer,float& slice,float& point);
  int getelementno(double layer,double slice,double point);
  int getnodenofromfractionalcoordinates(float layer,float slice,float point);

  // Compare with other solid
  // ------------------------

  void  getElementZThetaRange(PXIntegerList* element,
			      float cx,float cy,
			      float& minz,float& maxz,
			      float& minangle,float& maxangle);


  void generatePoints(PXIntegerList* element,PX3Vector* vec,int& maxp);
  int compareWithOtherSolid(PXAbaqusSolid* sld,const char* fname,float dist=1.0);
				   


protected:

  // Solid Generation
  // ----------------
  PXTriangulatedStackPoint* getpoint(int index,int *total,int numstacks);

  // Cleanup Operations
  // ------------------
  void cleanupFemData();
  void cleanupNodeSpace();
  void cleanupElementSpace();
  void cleanupSpringSpace();
  void cleanupMaterial();


  // More Allocation Stuff
  // ---------------------
  void allocateNodeSpace(int numnode);
  void allocateElementSpace();


public:

  // Static Functions mainly to do with postprocessing fem data
  // ----------------------------------------------------------
  static int debugmode;  
  static void color_lookup(PX3Vector& output,float value,float* params=NULL,int mode=1);


  static int  loadJobIntoSolidArray(const char* fname,PXAbaqusSolid** solids,int maxsolids,
				    int numslices=3,int numsectors=8,int rotate=1);
  static int  averageStrainsInSolidArray(PXAbaqusSolid** solids,int numframes,
					 float jx,float jy,float jz,int numslices=3,int numsectors=8,
					 int anticlockwise=1);
  static int  averageStrainsInSolidArray(PXAbaqusSolid** solids,int numframes);
  static int  savePies(const char* fname,PXAbaqusSolid** solids,int numframes,int mode=0,int skip=1);


  // Sub-slice subdivision code 
  static float returnElementFraction(float elementminz,float elementmaxz,float globalminz,float globalmaxz,
				     int slice,int numslices,int debug=0);
  static void  getElementRange(PXIntegerList* element,PXSolidNodes* nodes,float cx,float cy,float phase_ref,
			       float& minz,float& maxz,
			       float& minangle,float& maxangle,
			       float& mintransmural,float& maxtransmural);
  static int subdivideandSavePies(const char* fname,PXAbaqusSolid** solids,int numframes,int mode,
				  float rx,float ry,float rz,int numslices,int numsectors,
				  int numtransmural,
				  int anticlockwise,int skipframes);



  
  // For temperature adjusted points 
  // -------------------------------
  static float temperatureForVolChange(float vol1,float vol2,int nlgeom=1);
  
};

  
  
#endif  

