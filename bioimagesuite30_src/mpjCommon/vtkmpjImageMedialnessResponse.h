

//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  All rights reserved. This file may not be edited/copied/redistributed
//BIOIMAGESUITE_LICENSE  without the explicit permission of the authors.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------


/*
 * Computes Krissian's medialness response measure Rt
 */

#ifndef __vtkmpjImageMedialnessResponse_h
#define __vtkmpjImageMedialnessResponse_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"

class vtkmpjImageMedialnessResponse : public vtkpxSimpleImageToImageFilter
{
public:
  static vtkmpjImageMedialnessResponse *New();
  vtkTypeMacro(vtkmpjImageMedialnessResponse,vtkpxSimpleImageToImageFilter);

  // Description:
  // Set Scale Sigma
  vtkGetMacro(Radius, float);
  vtkSetMacro(Radius, float);
  
  // Description:
  // Get/Set Hessian matrix
  vtkGetObjectMacro(Hessian, vtkImageData);
  vtkSetObjectMacro(Hessian, vtkImageData);

  // Description
  // Get/Set gradient calculation
  vtkGetMacro(CalculateGradient, int);
  vtkSetMacro(CalculateGradient, int);
  vtkBooleanMacro(CalculateGradient, int);
  
  // Description:
  // Set Number of points for sample the gradient vector along the circle
  vtkGetMacro(NumberOfCirclePoints, int);
  vtkSetMacro(NumberOfCirclePoints, int);
  
 protected:  
  vtkmpjImageMedialnessResponse();
  virtual ~vtkmpjImageMedialnessResponse();
  
  virtual void SimpleExecute(vtkImageData* input,vtkImageData* output);
  virtual void ExecuteInformation();
  
  int CalculateGradient;
  vtkImageData *Hessian;
  float Radius;
  int NumberOfCirclePoints;
  
 private:
  
  float ComputeMedialness(float point[3], float radius, float v1[3], float v2[3], vtkDataArray *scalars, vtkDataArray *cell_scalars);
  void ComputeEigenSystem(vtkDataArray *hess, int index, float lambda[3], float W[3][3]);
  void SortEigenvectors(float w[3], float V[3][3]);
  
  vtkmpjImageMedialnessResponse(const vtkmpjImageMedialnessResponse&) {};
  void operator=(const vtkmpjImageMedialnessResponse&) {};  

};

#endif


