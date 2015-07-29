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
  Module:    $RCSfile: vtkpxPolySimilarityTransform.h,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
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

 ========================================================================*/

#ifndef __vtkpxPolySimilarityTransform_h
#define __vtkpxPolySimilarityTransform_h

#include "vtkWarpTransform.h"
#include "vtkCollection.h"
#include "vtkImageData.h"
#include "vtkpxPolyAbstractTransform.h"
#include "vtkpxArticulatedJointTransform.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkTubeFilter.h"
#include "vtkpxMatrix.h"
#include "vtkpxGridTransform.h"
#include "vtkpxComboTransform.h"
#include "vtkpxTransformationUtil.h"
#include "vtkThinPlateSplineTransform.h"

//BTX
class vtkpxPolySimilarityOptimizationStructure {
public:
  double step;
  int   st;
  int   maxdofs;

  vtkPoints *sourcePoints,*targetPoints;
  vtkpxMatrix* weights;

  vtkpxPolySimilarityOptimizationStructure() {
    sourcePoints=NULL;
    targetPoints=NULL;
    weights=NULL;
  }

  void SetValues(int maxdofs0,double step0,
		 int st0)
  {
    this->step=step0;
    this->maxdofs=maxdofs0;
    this->st=st0;
  }

  void SetLandmarksAndWeights(vtkPoints* p1,vtkPoints* p2,vtkpxMatrix* w)
  {
    sourcePoints=p1;
    targetPoints=p2;
    weights=w;
  }
};
//ETX

class vtkpxPolySimilarityTransform : public vtkpxPolyAbstractTransform
{
public:
  vtkTypeMacro(vtkpxPolySimilarityTransform,vtkpxPolyAbstractTransform);
  static vtkpxPolySimilarityTransform *New();


  // Description:
  // Set Label Image
  virtual void SetRegionLabel(vtkImageData* h);

  // Description:
  // Get Linear Transform
  virtual void SetTransform(int i,vtkpxArticulatedJointTransform* tr);
  virtual vtkpxArticulatedJointTransform* GetTransform(int i);
  virtual int GetNumberOfTransformations();

  // Description:
  // This is a preset
  virtual void SetWholeBodyPreset(int doscale,int hasfeet,
				 int hashindlimbs,int hasforelimbs,
				 int hashead, int taildivisions,
					 int spinedivisions);

  virtual void SetCylinderPreset(int numparts=2);


  // Description:
  // Save & Load Transform into .tal file
  int Load(const char* fname);
  int Save(const char* fname);

  // Description:
  // Optimization Parameters
  virtual int  GetNumberOfParameters();
  virtual void SetCurrentParameters(vtkDoubleArray* param);
  virtual void GetCurrentParameters(vtkDoubleArray* param);

  // Description:
  // These are needed for the Optimizer 
  static void   OptimizerFeedback(vtkObject* self,const char* line,double val);
  static double CalculateGradient(vtkObject* self,vtkDoubleArray* position,vtkDoubleArray* output);
  static double CalculateFunction(vtkObject* self,vtkDoubleArray* position);

  // Description:
  // Compute Gradients etc
  virtual double OptimizerGradient(vtkDoubleArray* params,vtkDoubleArray* grad);
  virtual double OptimizerValue(vtkDoubleArray* params);

  // Description:
  // Fix Origins
  virtual void FixOrigins(double gap=2.0);
  virtual void SetOriginAxes(vtkPoints* points);
  vtkPolyData* DrawAxis(float scale=2.0,float length=100.0);

  // Description:
  // Approximate Points
  virtual double ApproximatePoints(vtkPoints* SourceLandmarks,vtkPoints* TargetLandmarks,vtkpxMatrix* Weights,
				   int numsteps=1,float beginstepsize=1.0,float tolerance=0.01);



  
  // Description:
  // Initialize Rotations
  virtual int InitializeRearLimbPresetRotations(vtkPoints* points);
  virtual int InitializeRearLimbPresetRotations(vtkPoints* points,vtkLinearTransform* tr);
  virtual vtkPoints* CreateTestPoints();


  // Description:
  // Identity
  virtual void Identity();

  // Description:
  // Specify name of the region label image
  vtkSetStringMacro(RegionLabelImageName);
  vtkGetStringMacro(RegionLabelImageName);

  // Description:
  // Create Displacement Field
  virtual int UpdateThinPlateSplineTransform();
  virtual int CreateThinPlateSplineTransform(int numberofcontrolpoints=150,float smooth=2.0,float ratio=1.25,float startsample=-1.0);
  virtual vtkPoints* CreateSamplePoints(int numberofpoints=1500,float smooth=1.5,float ratio=1.25,float startsample=-1.0);

  // Description:
  // Initialize from a Poly Similarity Transform
  virtual void InitializeComboTransform(vtkpxComboTransform* combo,int numpoints,int numsteps,double smoothness=0.0001);


protected:
  vtkpxPolySimilarityTransform();

  vtkpxPolySimilarityTransform(const vtkpxPolySimilarityTransform&) {};
  virtual ~vtkpxPolySimilarityTransform();

  void operator=(const vtkpxPolySimilarityTransform&) {};

  // Get Transformation Collection
  vtkGetObjectMacro(TransformationCollection,vtkCollection);

  vtkpxPolySimilarityOptimizationStructure OptimizationStructure;

  // Description:
  // File Name for RegionLabelImage
  char          *RegionLabelImageName;


  // Description:
  // Evaluate
  virtual double Evaluate();

};

#endif






