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
  Module:    $RCSfile: vtkpxRPMCorrespondenceFinder.h,v $
  Language:  C++
  Date:      $Date: 2003/11/13 20:02:03 $
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

#ifndef __vtkpxRPMCorrespondenceFinder_h
#define __vtkpxRPMCorrespondenceFinder_h

#include "vtkTransform.h"
#include "vtkProcessObject.h"
#include "vtkCollection.h"
#include "vtkpxMatrix.h"
#include "vtkPoints.h"
#include "vtkPointLocator.h"
#include "vtkPolyData.h"

class vtkpxRPMCorrespondenceFinder : public vtkProcessObject
{
public:
  static vtkpxRPMCorrespondenceFinder *New();
  vtkTypeMacro(vtkpxRPMCorrespondenceFinder,vtkProcessObject);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Set the current Temperature
  vtkSetClampMacro(Temperature,float,0.0,2000.0);
  vtkGetMacro(Temperature,float);

  // Description:
  // Initialize by allocating Memory etc
  virtual void Initialize(vtkAbstractTransform* OffsetTransform);
  virtual void Initialize() { this->Initialize(NULL); }

  // Description:
  // Warp and Update Correspondence
  virtual void WarpAndUpdateCorrespondence(vtkAbstractTransform* warpTransform,
					   float temperature);
  virtual float ComputeDistance(const char* label);
  virtual float ComputeDistance();

  // Description:
  // Access Certain Internal Stuff
  vtkGetObjectMacro(PointsX, vtkPoints);
  vtkGetObjectMacro(PointsVY, vtkPoints);
  vtkGetObjectMacro(M_Outliers_Column, vtkpxMatrix);
  vtkGetObjectMacro(Collection,vtkCollection);
  

  // Description:
  // Specify the source and target data sets.
  void SetSource(vtkPolyData *source);
  void SetTarget(vtkPolyData *target);
  vtkGetObjectMacro(Source, vtkPolyData);
  vtkGetObjectMacro(Target, vtkPolyData);

  // Description:
  // Specify the initial transformation
  vtkSetObjectMacro(InitialTransform,vtkAbstractTransform);
  vtkGetObjectMacro(InitialTransform,vtkAbstractTransform);

  // Description: 
  // Set/Get vertex step
  vtkSetMacro(MaximumNumberOfLandmarks, int);
  vtkGetMacro(MaximumNumberOfLandmarks, int);

  // Description:
  // Extra Debug
  vtkSetClampMacro(ExtraDebug,int,0,1);
  vtkGetMacro(ExtraDebug,int);

  // Description:
  // Set the current mode 0=ICP, 1=RPM-Mixture 2=RPM 3=RPM-Mixture-Fast 4=RPM Fast 5=ICP-Plus
  // 6=RPM-Mixture-Hybrid 7-RPM Hybrid
  vtkSetClampMacro(MatchMode,int,0,5);
  vtkGetMacro(MatchMode,int);

  // Description:
  // Dedicated Set/Get MatchMode stuff 
  virtual void SetMatchModeToICP()          { this->SetMatchMode(0);}
  virtual void SetMatchModeToMixture()      { this->SetMatchMode(1);}
  virtual void SetMatchModeToRPM()          { this->SetMatchMode(2);}
  virtual void SetMatchModeToMixtureFast()  { this->SetMatchMode(3);}
  virtual void SetMatchModeToRPMFast()      { this->SetMatchMode(4);}
  virtual void SetMatchModeToICPPlus()      { this->SetMatchMode(5);}

  // Description: 
  // UseLabels
  vtkSetClampMacro(UseLabels, int,0,1);
  vtkGetMacro(UseLabels, int);
  vtkBooleanMacro(UseLabels, int);

  // Description: 
  // Use Attributes 
  vtkSetClampMacro(UseAttributes, int,0,1);
  vtkGetMacro(UseAttributes, int);
  vtkBooleanMacro(UseAttributes, int);


  // Description:
  // Threshold for fast search
  vtkSetMacro(AttributesWeight,float);
  vtkGetMacro(AttributesWeight,float);

  // Description: 
  // UseLabels
  vtkSetClampMacro(PreferentialSampling, int,0,1);
  vtkGetMacro(PreferentialSampling, int);
  vtkBooleanMacro(PreferentialSampling, int);

  // Description: 
  // UseWeightedLeastSquares
  vtkSetClampMacro(UseWeightedLeastSquares, int,0,1);
  vtkGetMacro(UseWeightedLeastSquares, int);
  vtkBooleanMacro(UseWeightedLeastSquares, int);

  // Description: 
  // UseInitialTransform
  vtkSetClampMacro(UseInitialTransform, int,0,1);
  vtkGetMacro(UseInitialTransform, int);
  vtkBooleanMacro(UseInitialTransform, int);

  // Description: 
  // MatchMatrixType
  vtkSetClampMacro(MatchMatrixType, int,-1,1);
  vtkGetMacro(MatchMatrixType, int);
  vtkBooleanMacro(MatchMatrixType, int);
  virtual void SetMatchMatrixTypeToAuto()          { this->SetMatchMatrixType(-1);}
  virtual void SetMatchMatrixTypeToSparse()        { this->SetMatchMatrixType(0);}
  virtual void SetMatchMatrixTypeToDense()         { this->SetMatchMatrixType(1);}

  // Description:
  // Threshold for fast search
  vtkSetMacro(FastThreshold,float);
  vtkGetMacro(FastThreshold,float);

  // Description:
  // Enable/Disable Update of Transformation in conjuction with UpdateProgress
  vtkSetMacro(EnableFeedback,int);
  vtkGetMacro(EnableFeedback,int);
  vtkBooleanMacro(EnableFeedback, int);

  // Description:
  // Threshold for fast search
  vtkGetMacro(AverageDistance,float);

protected:
  // Description:
  
  vtkpxRPMCorrespondenceFinder();
  ~vtkpxRPMCorrespondenceFinder();
  vtkpxRPMCorrespondenceFinder(const vtkpxRPMCorrespondenceFinder&) {};
  void operator=(const vtkpxRPMCorrespondenceFinder&) {};

  // Description:
  // Current Temperature
  float Temperature;

  // Description:
  // Internal Storage 
  vtkpxAbstractMatrix* M;
  vtkpxMatrix* M_Outliers_Row;
  vtkpxMatrix* M_Outliers_Column;
  vtkpxMatrix* X;
  vtkpxMatrix* VX;
  vtkpxMatrix* Y;
  vtkpxMatrix* VY;
  vtkpxMatrix* LX;
  vtkpxMatrix* LY;                  
  vtkPointLocator* Locator;
  vtkCollection* Collection;
  vtkPoints* PointsX;
  vtkPoints* PointsVY;

  // Description:
  // Clean Up Stuff 
  void cleanobject(vtkObject* f) {
    if (f!=NULL)
      f->Delete();
  }

  // Description:
  // Clean Up Procedure
  virtual void CleanUp();

  // Description:
  // Update pointsVY
  virtual void UpdateVYPoints();

  // Description:
  // Release source and target
  void ReleaseSource(void);
  void ReleaseTarget(void);

  vtkPolyData* Source;
  vtkPolyData* Target;

  int EnableFeedback;
  int MaximumNumberOfIterations;
  int MaximumNumberOfLandmarks;

  // Progress/Update handling
  vtkAbstractTransform* InitialTransform;

  // Description:
  // Parameters for Various Things

  float FastThreshold;
  int   UseLabels;
  int   PreferentialSampling;
  int   UseInitialTransform;
  int   MatchMode;
  int   ExtraDebug;
  int   UseWeightedLeastSquares;
  int   MatchMatrixType;
  int   UseAttributes;
  float AttributesWeight;
  float AverageDistance;

  // -----------------------------------------------------------
  // Setup Routines, utility stuff
  // -----------------------------------------------------------
  // Description:
  // Sample and Scale points appropriately 
  virtual void  SamplePoints(vtkpxMatrix* X,vtkpxMatrix* Y,
			     vtkpxMatrix* LX=NULL,vtkpxMatrix* LY=NULL,int numlandmarks=-1,int sampleonlyfirstlabel=0);
  virtual void  DoStraightSampling(vtkPolyData* surface,vtkpxMatrix* X,vtkpxMatrix* LX,int nump,int step,int dolabels);
  virtual int   DoPreferentialSampling(vtkPolyData* surface,vtkpxMatrix* X,vtkpxMatrix* LX,int nump);


  void ZeroArray(int num,float *x);
  void ZeroArray(int num,double *x);

  // Create Points from Matrix and vice-versa
  virtual vtkPoints*   CreatePointsFromMatrix(vtkpxMatrix* X,int sample=1);
  virtual vtkpxMatrix* CreateMatrixFromPoints(vtkPoints* pts);

  // Description:
  // Initialize Labels and Match Matrices 
  void InitMatchMatrices(vtkpxAbstractMatrix* M,vtkpxMatrix* M_Outlier_Row,vtkpxMatrix* M_Outlier_Column,
			 int rows,int cols);


  // -------------------------------------------------------------
  // Correspondence Stuff
  // -------------------------------------------------------------

  // Description:
  // Update Correspondence
  virtual void UpdateCorrespondence(vtkpxAbstractMatrix* m,
				    vtkpxMatrix* m_outliers_row,vtkpxMatrix* m_outliers_column,
				    vtkpxMatrix* vx,vtkpxMatrix* y,vtkpxMatrix* vy,
				    float T,vtkPointLocator* locator,
				    vtkpxMatrix* lx,vtkpxMatrix* ly);


  // Description:
  // Routines for Estimating correspondence in one-shot, no distance matrix involved
  virtual void MapPointsICP(vtkpxMatrix* vx,vtkpxMatrix* y,vtkpxMatrix* vy,vtkPointLocator* locator);
  virtual void MapPointsICPPlus(vtkpxMatrix* vx,vtkpxMatrix* y,vtkpxMatrix* vy,float T,vtkPointLocator* locator);
  
  // Description:
  // Routines to estimate distance matrices 
  
  virtual void FormDistanceMatrix(vtkpxAbstractMatrix* M,vtkpxMatrix* vx,vtkpxMatrix* y,float T,
				  vtkpxMatrix* LX,vtkpxMatrix* LY);
  virtual void FormDistanceMatrixFast(vtkpxAbstractMatrix* M,vtkpxMatrix* vx,vtkpxMatrix* y,
				      float T,vtkPointLocator* locator,
				      vtkpxMatrix* LX,vtkpxMatrix* LY);

  // Description:
  // Allocate the Match Matrix
  virtual vtkpxAbstractMatrix* AllocateMatchMatrix(int dense=-1);
  

  // Description:
  // Create Point Locator
  virtual vtkPointLocator* CreateLocator(vtkpxMatrix* Y,int pointsperbucket=2);
  

  // Description:
  // Routine for warping  points using the transformation 
  virtual void TransformPoints(vtkAbstractTransform* xform,
			       vtkpxMatrix* x,vtkpxMatrix* vx);


  // Description:
  // Outlier Initialization and Transformation To Weights
  float SetOutliers(vtkpxMatrix* M_Outlier_Row,vtkpxMatrix* M_Outlier_Column,float T);
  void  TransformOutlierColumnToWeights(vtkpxMatrix* M_Outliers_Column,int Mode,int UseWLS);

  
};

#endif

