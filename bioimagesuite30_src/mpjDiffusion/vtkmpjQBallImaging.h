

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


/* -- vtkmpjQBallImaging.h 
   
      Marcel Jackowski, 2003

      Q-Ball Imaging. (D. Tuch, MRM 52, 2004)

*/

#ifndef __vtkmpjQBallImaging_h
#define __vtkmpjQBallImaging_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkPoints.h"
#include "vtkDoubleArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjQBallImaging : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjQBallImaging *New();
  vtkTypeMacro(vtkmpjQBallImaging,vtkpxSimpleImageToImageFilter);
  
  // Sets/Gets the baseline image A(b=0); the echo amplitude without any
  // diffusion attenuation
  vtkSetObjectMacro(Baseline, vtkImageData);
  vtkGetObjectMacro(Baseline, vtkImageData);
  
  // Description: 
  // Sets/Gets mask for determining the extents of tensor
  // computation. If a mask is not provided, tensor is
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);

  // Description:
  // Sets/Gets the array of gradient directions (wavevectors)
  vtkSetObjectMacro(Gradients, vtkDoubleArray);
  vtkGetObjectMacro(Gradients, vtkDoubleArray);
  
  // Description
  // Sets/Gets the array of reconstruction points
  vtkSetObjectMacro(Reconstruction, vtkPoints);
  vtkGetObjectMacro(Reconstruction, vtkPoints);

  // Set/Get Shift Factor so that S(b)/S(0) can be computed when S(0)=0
  // ADC = (S(b)+shift)/(S(0)+shift), shift ~= 0
  vtkSetMacro(Shift,float);
  vtkGetMacro(Shift,float);
  
  // Description:
  // Set/Get the b-Value
  vtkSetMacro(bValue,float);
  vtkGetMacro(bValue,float);
  
 protected:
  float Shift;
  float bValue;
  vtkImageData *Baseline;
  vtkImageData *Mask;
  vtkDoubleArray *Gradients;
  vtkPoints *Reconstruction;

  vtkmpjQBallImaging();
  virtual ~vtkmpjQBallImaging();

  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

  double Smooth(double x, double sigma);
  void ComputeReconstructionMatrix(double **a, double **b, double **c, int n, int m, int p);
  void VectorMultiply(double **A, double *b, double *c, int n, int m);

 private:  
  vtkmpjQBallImaging(const vtkmpjQBallImaging&); // Not implemented
  void operator=(const vtkmpjQBallImaging&); // Not implemented
};

#endif
