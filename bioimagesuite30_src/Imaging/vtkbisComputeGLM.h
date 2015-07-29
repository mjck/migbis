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





#ifndef __vtkbisComputeGLM_h
#define __vtkbisComputeGLM_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkpxMatrix.h"


class vtkbisComputeGLM : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkbisComputeGLM *New();
  vtkTypeMacro(vtkbisComputeGLM,vtkpxSimpleImageToImageFilter);

  // Parameters
  // Regressor matrix
  vtkSetObjectMacro(RegressorMatrix,vtkpxMatrix);
  vtkGetObjectMacro(RegressorMatrix,vtkpxMatrix);

  // Mask Image
  vtkSetObjectMacro(ImageMask,vtkImageData);
  vtkGetObjectMacro(ImageMask,vtkImageData);

  // Residual Image
  vtkGetObjectMacro(ResidualImage,vtkImageData);


  // Residual Image
  vtkGetObjectMacro(UnfitTimeSeries,vtkImageData);

  // Drift Terms
  vtkGetObjectMacro(DriftBetaValues,vtkImageData);

  // If using ``pre-corrected'' stuff then set num of drift terms explicitly here (this is only for fstat computation)
  // This is ignored unless NumberOfTasks = Number of columns of supplied regressor matrix
  vtkSetClampMacro(OrderOfFstatDriftPolynomial,int,0,3);
  vtkGetMacro(OrderOfFstatDriftPolynomial,int);


  // Invidual Regressor Ttest Image
  vtkGetObjectMacro(TmapImage,vtkImageData);

  // Number of Tasks (last N columns of regressor matrix, first few columns are drift)
  vtkSetMacro(NumberOfTasks,int);
  vtkGetMacro(NumberOfTasks,int);

  // Number of Tasks (last N columns of regressor matrix, first few columns are drift)
  vtkSetMacro(DebugIndex,int);
  vtkGetMacro(DebugIndex,int);


  // Debug name for saving intermediate results
  vtkSetStringMacro(DebugName);
  vtkGetStringMacro(DebugName);



  // Inverse of GLM
  // Multiple Matrix with Image and save
  static vtkImageData* MatrixImageMultiply(vtkImageData* betas,vtkpxMatrix* glmmatrix);

  // Compute Direct GLM
  static vtkImageData* ComputeSimpleGLM(vtkImageData* timeseriesimage,vtkpxMatrix* glmmatrix);

protected:


  vtkbisComputeGLM();
  virtual ~vtkbisComputeGLM();

  // Description:
  // Threshold mode 0 = absolute, 1 = normalized i.e. 0:1
  virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);
  virtual void ExecuteInformation();

  // Description:
  // GLM Basics
  static vtkpxMatrix* ComputeLSQMatrix(vtkpxMatrix* A,vtkpxMatrix* S2BMatrix=NULL);
  static  void  StoreVoxelIndexIntoColumn(vtkDataArray* dat,int index,vtkpxMatrix* colvect);
  static  void  StoreColumnIntoVoxelIndex(vtkpxMatrix* vect,vtkDataArray* dat,int index);

  virtual void   ComputeBetas(vtkpxMatrix* SQL,vtkpxMatrix* beta,vtkpxMatrix* colvect);

  virtual double ComputeFStats(vtkpxMatrix* A,vtkpxMatrix* ReducedA,vtkpxMatrix* beta,vtkpxMatrix* nullbeta,
			       vtkpxMatrix* colvect,double factor,double tmapdof,vtkDataArray* res,vtkDataArray* unfit,int index,int debug=0);


  // Description:
  // Fill In Drift Terms
  virtual void CreateNULLRegressorMarix(vtkpxMatrix* inout,vtkpxMatrix* regressor,int order);

  int OrderOfFstatDriftPolynomial;
  vtkImageData* ImageMask;
  vtkImageData* ResidualImage;
  vtkImageData* UnfitTimeSeries;
  vtkImageData* DriftBetaValues;
  vtkImageData* TmapImage;
  vtkpxMatrix*  RegressorMatrix;
  vtkpxMatrix*  TempMatrix1;
  vtkpxMatrix*  TempMatrix2;
  vtkpxMatrix*  TempMatrix3;
  vtkpxMatrix*  TempMatrix4;

  vtkpxMatrix*  NULLRegressorMatrix;
  char* DebugName;
  int   NumberOfTasks;
  int   DebugIndex;

  // This is protected it should not be needed
  vtkSetObjectMacro(ResidualImage,vtkImageData);
  vtkSetObjectMacro(TmapImage,vtkImageData);
  vtkSetObjectMacro(UnfitTimeSeries,vtkImageData);
  vtkSetObjectMacro(DriftBetaValues,vtkImageData);

};
#endif

