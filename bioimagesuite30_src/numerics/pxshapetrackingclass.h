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



#ifndef _PxShapeTrackingClass
#define _PxShapeTrackingClass
#include "pxgeometrystruct.h"
#include "pxtriangulatedstack.h"


class PXShapeTrackingClass {
  
protected:

  // Original Inputs 
  PXTriangulatedStack*      tstack[2];

  // Geometrical Parameters 
  PX3Vector*                displacements;
  float                     *confidences;
  int                       *used,*correspondence,*fixed;
  int                       hasDisplacements;
  int                       numpoints,hasStack[2];

public:
  
  PXShapeTrackingClass();
  PXShapeTrackingClass(PXShapeTrackingClass* old);
  PXShapeTrackingClass(PXTriangulatedStack* stack1,PXTriangulatedStack* stack2);
  PXShapeTrackingClass(const char* fname1,const char* fname2);
  
  virtual ~PXShapeTrackingClass();
  void init(int np);
  void cleanup(); 
  void Copy(PXShapeTrackingClass* other);
  
  void nextFrame(const char* newstack2fname,int deform=1);
  void nextFrame(PXTriangulatedStack* newstack2,int deform=1);

  // Access functions
  // ----------------
  PXTriangulatedStack* getstack(int i);
  PX3Vector*           getdisplacement(int i);
  float                getconfidence(int i);
  
  int                  getused(int i);
  int                  getcorrespondence(int i);
  int                  getfixed(int i);
  int                  hasdisplacements();
  int                  getnumpoints();
  int                  hasstack(int i);
  int                  hasstacks();

  void setconfidence(int i,float s);
  void setdisplacement(int i,int comp,float s);
  // --------------------------------------------------------------
  // Running algorithms
  // --------------------------------------------------------------
  int saveInventor(const char* fname);
  int saveInventor(const char* fname,int number);
  int saveDeformed(const char* base,int number);
  int saveStackInventor(const char* fname,int colormode=0);

  int findSmoothMap(int mode,int iterations,int smoothiterations,int subdivide=0);
  int initialCurvatureTracking(int searchwindow);
  int initialDualCurvatureTracking(int searchwindow,PXShapeTrackingClass* other);
  int smoothCurvatureTracking(int iterations=100,float lambda=1.0);

  int volumePreserveZshift(PXShapeTrackingClass* other);

  // --------------------------------------------------------------
  // I/O Stuff
  // --------------------------------------------------------------
  int  Save(FILE* fout,int doNotsavebothStacks=1);
  int  Save(const char* fname,int doNotsavebothStacks=1,int complevel=0);
  int  Load(gzFile fout,int doNotloadbothStacks=1);
  int  Load(const char* fname,int doNotloadbothStacks=1);


  //  Create default fields 
  //  ---------------------
  void translate(float px,float py,float pz);
  void expand(float factor1,float factor2,float factor3); 
      
protected:
  int findSymmetricPair(PXNeighboursList* pairs,int seedt);
  int smoothEuclidean(int movefixed=1,int subpixel=1);
  int smoothConnection(int movefixed=1);

public:
  static int debugmode;


};

  
  
#endif  

