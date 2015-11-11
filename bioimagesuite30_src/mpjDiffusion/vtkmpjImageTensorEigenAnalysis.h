

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


/* -- vtkmpjImageTensorEigenAnalysis.h 
   
      Marcel Jackowski, 2016

      This class computes the eigenvalues and eigenvectors of a
      symmetric tensor image.
      
      It expects as the input a vtkImageData structure
      containing 6 frames containing the 6 tensor elements.
      See component ordering information below.

      It will output 12 frames; the first three frames containing the
      eigenvalues for each one of the 3 columns of the eigenvector matrix 
      and the remaining 9 frames containing the eigenvector elements 
      stored in row-wise fashion.      
*/

#ifndef __vtkmpjImageTensorEigenAnalysis_h
#define __vtkmpjImageTensorEigenAnalysis_h

// Output types
#define VTK_MPJ_TENSOREIG_VALUES  0    
#define VTK_MPJ_TENSOREIG_VECTORS 1
#define VTK_MPJ_TENSOREIG_BOTH    2

// Ordering
#define VTK_MPJ_TENSOR_XXXYXZYYYZZZ 0 
#define VTK_MPJ_TENSOR_XXYYZZXYYZXZ 1
#define VTK_MPJ_TENSOR_YYYZZZXYXZXX 2

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjImageTensorEigenAnalysis : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjImageTensorEigenAnalysis *New();
  vtkTypeMacro(vtkmpjImageTensorEigenAnalysis,vtkpxSimpleImageToImageFilter);

  // Description:
  // Set/Get the component ordering information
  vtkSetMacro(Ordering,int);
  vtkGetMacro(Ordering,int);
  
  // Description:
  // Set/Get the Output type: eigenvalues only (default), eigenvectors only or both 
  vtkSetMacro(OutputType,int);
  vtkGetMacro(OutputType,int);

  // Description: Create or not a mask depicting places where eigenvalues are negative
  vtkSetMacro(MaskNegative,int);
  vtkGetMacro(MaskNegative,int);
  vtkBooleanMacro(MaskNegative,int);

  // Description: Set/Get whether eigenvalues should always be positive. Default is true
  vtkSetMacro(OutputAbsoluteEigenvalues,int);
  vtkGetMacro(OutputAbsoluteEigenvalues,int);
  vtkBooleanMacro(OutputAbsoluteEigenvalues,int);

  // Description: Set/Get reordering of eigenvalues
  vtkSetMacro(SortByMagnitude,int);
  vtkGetMacro(SortByMagnitude,int);
  vtkBooleanMacro(SortByMagnitude,int);

  // Description: Set/Get reordering of eigenvalues
  vtkSetMacro(FixZeroEigenvalues,int);
  vtkGetMacro(FixZeroEigenvalues,int);
  vtkBooleanMacro(FixZeroEigenvalues,int);

  // Description: Set/Get eigenvector normalization
  vtkSetMacro(NormalizeEigenvectors,int);
  vtkGetMacro(NormalizeEigenvectors,int);
  vtkBooleanMacro(NormalizeEigenvectors,int);

  // Description:
  // Set/Get the outside value for the mask
  vtkSetMacro(MaskNegativeOutValue,float);
  vtkGetMacro(MaskNegativeOutValue,float);
  
  // Description:
  // Set/Get the inside value for the mask
  vtkSetMacro(MaskNegativeInValue,float);
  vtkGetMacro(MaskNegativeInValue,float);

  // Description:
  // Sets/Gets mask where eigenvalues are to be computed
  vtkGetObjectMacro(Mask, vtkImageData);
  vtkSetObjectMacro(Mask, vtkImageData);

  // Description:
  // Gets mask where eigenvalues are negative
  vtkGetObjectMacro(NegativeMask, vtkImageData);
  vtkSetObjectMacro(NegativeMask, vtkImageData);
  
  void SetOutputTypeToEigenvalues() {this->SetOutputType(VTK_MPJ_TENSOREIG_VALUES);};
  void SetOutputTypeToEigenvectors() {this->SetOutputType(VTK_MPJ_TENSOREIG_VECTORS);};
  void SetOutputTypeToBoth() {this->SetOutputType(VTK_MPJ_TENSOREIG_BOTH);};
  
 protected:
  int Ordering;
  int OutputType;
  int MaskNegative;
  int SortByMagnitude;
  int NormalizeEigenvectors;
  int OutputAbsoluteEigenvalues;
  int FixZeroEigenvalues;
  vtkImageData *Mask;
  vtkImageData *NegativeMask;
  float MaskNegativeInValue;
  float MaskNegativeOutValue;

  vtkmpjImageTensorEigenAnalysis();
  virtual ~vtkmpjImageTensorEigenAnalysis();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();
  
 private:
  vtkmpjImageTensorEigenAnalysis(const vtkmpjImageTensorEigenAnalysis&); // Not implemented
  void operator=(const vtkmpjImageTensorEigenAnalysis&); // Not Implemented
  
  void SortEigenvectorsByMagnitude(float *w, float **v);
  void ExecuteWithReducedTensor(vtkImageData *input, vtkImageData *output);
  void ExecuteWithFullTensor(vtkImageData *input, vtkImageData *output);
};

#endif
