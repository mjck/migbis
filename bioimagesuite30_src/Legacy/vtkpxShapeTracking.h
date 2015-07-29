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





/*=========================================================================

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkpxShapeTracking.h,v $
  Language:  C++
  Date:      $Date: 2002/04/01 14:18:00 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
// .NAME vtkpxShapeTracking 


#ifndef __vtkpxShapeTracking_h
#define __vtkpxShapeTracking_h

#include "vtkPolyDataSource.h"
#include "pxgeometrystruct.h"
#include "vtkLandmarkTransform.h"
#include "vtkPolyData.h"

class vtkPointLocator;
class vtkFloatArray;
class vtkIntArray;
class vtkpxTriangulatedStackSource;

class vtkpxShapeTracking : public vtkPolyDataSource
{
public:
  static vtkpxShapeTracking *New();
  vtkTypeMacro(vtkpxShapeTracking,vtkPolyDataSource);

  //Description:
  // Input Surfaces Reference and Target
  vtkSetObjectMacro(ReferenceSurface,vtkPolyData);
  vtkGetObjectMacro(ReferenceSurface,vtkPolyData);
  vtkSetObjectMacro(TargetSurface,vtkPolyData);
  vtkGetObjectMacro(TargetSurface,vtkPolyData);

  //Description:
  // I/O Code -- save as .disp file 
  virtual int Load(const char* fname);
  virtual int Save(const char* fname);

  //Description:
  // Mode for the transformation
  vtkSetClampMacro(Mode,int,0,3);
  vtkGetMacro(Mode,int);
  virtual void SetModeToNearestNeighbor()     { this->SetMode(0);}
  virtual void SetModeToSymmetricNeighbor()   { this->SetMode(1);}
  virtual void SetModeToAffinePlusNN()        { this->SetMode(2);}
  virtual void SetModeToAffinePlusSNN()       { this->SetMode(3);}

  // Description:
  // Set Mode for Affine Initialization
  vtkSetMacro(InitialMode,int);
  void SetInitialModeToRigidBody() { this->SetInitialMode(VTK_LANDMARK_RIGIDBODY); };
  void SetInitialModeToSimilarity() { this->SetInitialMode(VTK_LANDMARK_SIMILARITY); };
  void SetInitialModeToAffine() { this->SetInitialMode(VTK_LANDMARK_AFFINE); };
  vtkGetMacro(InitialMode,int);

  
  // Description:
  // Window Size for Shape Match
  vtkSetClampMacro(SearchRadius,int,0,3);
  vtkGetMacro(SearchRadius,int);

  // Description:
  // Number of Iterations for Symmetric Nearest Neighbor Interpolation
  vtkSetClampMacro(InterpolateIterations,int,2,50);
  vtkGetMacro(InterpolateIterations,int);

  // Description:
  // Threshold for Symmetric Nearest Neighbor Interpolation Convergence
  vtkSetClampMacro(InterpolateThreshold,float,0,3);
  vtkGetMacro(InterpolateThreshold,float);


  // Description:
  // Access Fem Data
  virtual float GetDisplacement(int node,int comp);
  virtual float GetConfidence(int node);

  // Description:
  // Get Displacements as Scalars
  vtkGetObjectMacro(Displacements,vtkFloatArray);

  // Description:
  // Old Style Shape Tracking
  static void OldStyleShapeTracking(vtkpxTriangulatedStackSource* st1,
			     vtkpxTriangulatedStackSource* st2,
			     int searchwindow,
			     vtkFloatArray *displacements,
			     const char* fname);


  static void OldStyleShapeTracking(vtkpxTriangulatedStackSource* st1,
			     vtkpxTriangulatedStackSource* st2,
			     vtkFloatArray *displ,
			     int searchwindow=3);

  static int OldStyleShapeTracking(vtkpxTriangulatedStackSource* st1,
				   vtkpxTriangulatedStackSource* st2,
				   const char *filename,
				   int searchwindow=3,
				   int subdivide=1);


  static int OldStyleShapeTracking(const char* name1,const char* name2,
				   char *outfilename,
				   int searchwindow=3,
				   int subdivide=1);



protected:

  vtkpxShapeTracking();
  virtual ~vtkpxShapeTracking();
  vtkpxShapeTracking(const vtkpxShapeTracking&) {};
  void operator=(const vtkpxShapeTracking&) {};
  
  vtkFloatArray* Displacements;
  vtkFloatArray* Confidences;
  vtkIntArray*   Flags;
  vtkPolyData*   ReferenceSurface;
  vtkPolyData*   TargetSurface;
    
  int            Mode;
  int            InitialMode;
  int            SearchRadius;
  int            InterpolateIterations;
  float          InterpolateThreshold;

  // Description:
  // Generate Output
  void Execute();

  // Description:
  // Actual Shape Tracking Routine 
  int DoCurvatureTracking(vtkPolyData* ref,vtkPolyData* targ,
			  vtkFloatArray* displ,vtkIntArray* flags,vtkFloatArray* Confidences);

  // --------------------------------------------------
  // Surface Matching Stuff
  // --------------------------------------------------
  void InitializeFlags(vtkPolyData* poly,vtkIntArray* flags);
  void ComputeNearestNeighbor(vtkPolyData* ref,vtkPolyData* targ,vtkFloatArray* displ,vtkIntArray* flags);
  void ComputeSymmetricNearestNeighbor(vtkPolyData* ref,vtkPolyData* targ,vtkFloatArray* displ,vtkIntArray* flags);
  void ComputeAffineMatch(vtkPolyData* ref,vtkPolyData* targ,vtkFloatArray* displ);


  // ------------------------------------------------
  // Interpolate SymmetricNearestNeighbor Stuff
  // ------------------------------------------------

  int   SmoothSpreadDisplacements(PXIntegerList* neighbors,
				  vtkFloatArray* displ,vtkFloatArray* olddisp,
				  vtkIntArray* flags,
				  int movefixed);

  float MapDisplacements(vtkPoints* refpoints,vtkPoints* targpoints,
			 vtkFloatArray* displ,vtkFloatArray* olddisp,
			 vtkIntArray* flags,
			 vtkPointLocator* locator);

  void InterpolateSymmetricNearestNeighbor(vtkPolyData* ref,vtkPolyData* targ,vtkFloatArray* displ,
					   vtkIntArray* flags,vtkPointLocator* loc,
					   int iterations=5,float threshold=0.1);
  
  
};

#endif



