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


#ifndef _PXTriangulatedStack
#define _PXTriangulatedStack
#include "pxcontourstack.h"
#include "pxgeometrystruct.h"




struct PXColorPoint {
  float red,green,blue;
};

struct PXStackIndex {
  int   numcontours;
  int   *index;
  float *zlevel;
};

class PXTriangulatedStack  {

  protected :

  PXStackIndex stackIndex;
  PXTriangulatedStackPoint* point;
  PXIntegerList* neighbours;
  PXNeighboursList* neighboursList;
  PXTriangle* triangle;
  int numpoints,numtriangles,maxconnection,useindex;
  int haspointstructures,hastrianglestructures,hasneighbourstructures,normalsok,curvaturesok;
  int stackindexok,trianglecentroidsok;

  // ******************************************************************************
  //          PUBLIC INTERFACE
  // ******************************************************************************

  public :
  static float max_curvature,min_curvature;
  static int   debugmode;

  PXTriangulatedStack(PXTriangulatedStack* old);
  PXTriangulatedStack(const char* nuagesfname,int mode);
  PXTriangulatedStack(PXContourStack* old,int mode);
  PXTriangulatedStack();
  virtual ~PXTriangulatedStack();

  void init(int np=0,int nt=0);
  void cleanup();
  void cleanupneighbours();

  void Copy(PXTriangulatedStack* old);
  void setfromcontourstack(PXContourStack* old,int mode);
  int  setfrompolarcontourstack(PXContourStack* old,float middlex);
  PXContourStack* exporttocontourstack(int skiplevels=0,int skipapex=0);
  
  

  // [A] Access Contours
  // ------------------
  virtual int   haspoints();       
  virtual int   hastriangles();    
  virtual int   hasneighbours();   
  virtual int   hasnormals();      
  virtual int   hascurvatures();   
  virtual int   hascentroids();    
  virtual int   hasstackindex();   
  				  
  virtual int   getmaxconnection();
  virtual int   getnumpoints();    
  virtual int   getnumtriangles(); 

  virtual PXTriangulatedStackPoint* getpoint(int pointidx);
  virtual PXTriangle*     gettriangle(int triangleidx);
  virtual PXIntegerList*  getneighbours(int idx);
  virtual PXStackIndex    getstackindex();
  

  // [B] Internal Operations
  // -----------------------
  virtual int  generateNeighbours(int openSurfaceMode=0);
  virtual int  generateNormals();
  virtual int  calculateCurvatures(int scale);
  virtual int  scalepoints(float sx,float sy,float sz=1.0);
  virtual int  calculateCentroids();
  virtual void initialiseStackIndex(PXContourStack* old);
  virtual int  getrelationship(int p1,int p2,int max=1000);
  virtual int  findCentroid(int* p,int np,int iterations=2,int max=5);
  virtual int  findCentroid(PXIntegerList* origList,int iterations=2,int maxd=5);
  virtual int  collectNeighbours(int point,int distance,PXIntegerList* list);

  virtual int updateStackIndexZlevels();

  // [C] I/O Functions
  // -----------------------------
  virtual int Load(const char* filename);
  virtual int Load(gzFile fin);
  virtual int LoadOffset(const char* filename);
  virtual int LoadOffset2(const char* filename);
  virtual int Save(const char* filename);
  virtual int Save(FILE* fout);
  virtual int SaveAbaqusRigid(const char* filename,int nodeoffset=200000,int index=1);
  virtual int SaveAbaqusRigid(FILE* fout,int nodeoffset=200000,int index=1);


  
  // [D] Smoothing, Subdivision and Trimming Operations
  // --------------------------------------------------
  int subdivide();
  int trimtstack(float zbot=-100.0,float ztop=100.0);
  int trimtstackz(float zbot,float ztop);
  int nonShrinkSmooth(float alpha,float beta,int iterations,int movez=0,int uniform=1);
  PX3Vector* nonShrinkSmoothD(float alpha,float beta,int iterations,int movez=0,int uniform=1);

  // [E] Shape Tracking Deformation
  // ------------------------------
  virtual float findnearestpoint(float&x,float& y,float &z,int& pt,int interpolate=1,float maxdist=0.1);
  virtual void deformUsingDisplacements(PX3Vector* disp,float factor=1.0,int smoothiter=0);
  virtual void updateCurvaturesFromStack(PXTriangulatedStack* otherstack);
  

  // [F] Triangulation Code
  // ----------------------
  static void findorigin2D(int np[2],float* x[2],float* y[2],
			   int origin[2]);
  static int  triangulate2D(int np[2],float* x[2],float* y[2],int origin[2],
			    PXNeighboursList* faces,int maxfaces);

  static void findorigin(int np[2],float* x[2],float* y[2],float* z[2],
			 int origin[2],int dimension=3);
  static int  triangulate(int np[2],float* x[2],float* y[2],float* z[2],int origin[2],
			 PXNeighboursList* faces,int maxfaces,int dimension=3);

  
  // More Static Code
  // ----------------
  static int  saveTstackPair(const char *fname,
			     PXTriangulatedStack* stack1,PXTriangulatedStack* stack2,
			     PX3Vector* disp,int* used=NULL,int* fixed=NULL);

  static int  findSymmetricPair(PXTriangulatedStack* tstack1,PXTriangulatedStack* tstack2,
				PXNeighboursList* tetra,int seedt=0);

  static int  smoothEuclidean(PXTriangulatedStack* tstack1,PXTriangulatedStack* tstack2,
			      PX3Vector* newdisp,PX3Vector* olddisp,
			      int* used,int* fixed,int* corr,
			      int mode,int saveno=-1,int smoothOutput=0);

  static int smoothConnection(PXTriangulatedStack* tstack1,PXTriangulatedStack* tstack2,
			      PX3Vector* dx,
			      int* used,int* fixed,int* corr,
			      int mode,int saveno=-1,int smoothOutput=0);
  static int curvatureTracking(PXTriangulatedStack* tstack1,PXTriangulatedStack* tstack2,
			       PX3Vector* disp,int* corr,
			       float lamda=0.3,int smoothIterations=10);
      

  static int findSmoothMap(PXTriangulatedStack* tstack1,PXTriangulatedStack* tstack2,
			   int* index,PX3Vector* dx,int mode=2,int iterations=20,
			   int curvmode=-1,float lamda=0.5);


  // [F] Combatibility with old Peng-Cheng Displacements
  // ---------------------------------------------------
  void findbestpair(PXDispPair* pair,int npoints,
		    float& x,float& y,float& z,float threshold);
  void deformUsingShapeDisplacements(const char* dispname,float threshold=0.2);
  int loadPengChengDisplacementsSingle(const char* filename,PX3Vector* dx,float tolerance=0.5);


  // [G] Inventor Colormap Code 
  // -----------------------------------
  static float curvature_lookup_red(float val);
  static float curvature_lookup_green(float val);
  static float curvature_lookup_blue(float val);
  static float bending_energy_lookup_red(float val);
  static float bending_energy_lookup_green(float val);
  static float bending_energy_lookup_blue(float val);
  static float shape_index_lookup_red(float val);
  static float shape_index_lookup_green(float val);
  static float shape_index_lookup_blue(float val); 
  PXColorPoint* generateColors(int mode,float gamma=1.5);
  virtual int SaveInventor(const char* fname,PXColorPoint* colors=NULL);
  virtual int SaveInventor(FILE* fout,PXColorPoint* colors=NULL);

  // [H] Rigid Deformation Code
  // --------------------------
  virtual void getstatistics(float mean[3],float var[6],PXTriangulatedStack* other=NULL);



  // [I] Solid related Code
  // -----------------------
  // Generate Solid -- these are static
  // ----------------------------------
  static int save2qhull(const char* fname,PXTriangulatedStack** stack,int numstakcs);
  static int loadqhull(const char* fname, PXTriangulatedStack** stack,int numstacks,
		       PXNeighboursList* tetra,int& nt);
  static int* gettotal(PXTriangulatedStack** stack,int numstacks);
  static int whichstack(int index,int* total,int numstacks);
  static int cleanup(PXTriangulatedStack* tstack[],int numstacks,
		     PXNeighboursList* tetra,int nt);
  static int save2inventor(const char *fname,PXTriangulatedStack** tstack,int numstacks,
			   PXNeighboursList* tetra,int nt);
  static int save2inventor(FILE* fout,PXTriangulatedStack** tstack,int numstacks,
			   PXNeighboursList* tetra,int nt);


  static float* generateVolumes(const char* fnamename,const char* fnameepi,int end,int begin);


};

#endif


