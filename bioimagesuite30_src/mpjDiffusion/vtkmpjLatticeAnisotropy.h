

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


/* -- vtkmpjLatticeAnisotropy.h 
  
 Implements both Add and LI intervoxel anisotropy indices.

A description of the Add coherence index was just recently published in 
Skare S et al: "Noise considerations in the determination of diffusion tensor 
anisotropy", MRI, 18(6):659-69, 2000. ( Medline citation). 
The Add coherence index is similar to the Lattice Anisotropy (LI) defined in 
Pierpaoli C. and Basser P.J.: "Toward a Quantitative Assessment of Diffusion
Anisotropy", MRM 36, 893-906 (1996) (Medline citation). 
  
  Marcel Jackowski, February 2003

*/

#ifndef __vtkmpjLatticeAnisotropy_h
#define __vtkmpjLatticeAnisotropy_h

// Intervoxel anisotropy methods
#define VTK_MPJ_LATTICEANISO_BOTH    0    
#define VTK_MPJ_LATTICEANISO_BASSER  1
#define VTK_MPJ_LATTICEANISO_SKARE   2

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"

class vtkmpjLatticeAnisotropy : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjLatticeAnisotropy *New();
  vtkTypeMacro(vtkmpjLatticeAnisotropy,vtkpxSimpleImageToImageFilter);
  
  // Description: 
  // Sets/Gets mask for determining the extents for invariants
  // computation. If a mask is not provided, invariants are
  // computed for every voxel in the image.
  vtkSetObjectMacro(Mask, vtkImageData);
  vtkGetObjectMacro(Mask, vtkImageData);

  // Sets/Gets eigenvalues
  vtkSetObjectMacro(Eigenvalues, vtkImageData);
  vtkGetObjectMacro(Eigenvalues, vtkImageData);

  // Description:
  // Set/Get the kernel size for computations;
  vtkSetVector3Macro(KernelSize,int);
  vtkGetVector3Macro(KernelSize,int);

  // Description:
  // Set/Get the Method to employ
  vtkSetMacro(Method,int);
  vtkGetMacro(Method,int);

  void SetMethodToBasser() { this->SetMethod(VTK_MPJ_LATTICEANISO_BASSER);};
  void SetMethodToSkare() { this->SetMethod(VTK_MPJ_LATTICEANISO_SKARE);};
  void SetMethodToBasserAndSkare() {this->SetMethod(VTK_MPJ_LATTICEANISO_BOTH);};

  void SetEigenvectors(vtkImageData *input) { this->SetInput(input); }
  vtkImageData* GetEigenvectors() { return this->GetInput(); }

protected:
  int Method;
  int KernelSize[3];
  float MaskOutValue; 
  vtkImageData *Mask;
  vtkImageData *Eigenvalues;
  
  vtkmpjLatticeAnisotropy();
  virtual ~vtkmpjLatticeAnisotropy();

  float TensorProduct(float ru[3], float nu[3], float rv[3][3], float nv[3][3]);
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();

 private:
  vtkmpjLatticeAnisotropy(const vtkmpjLatticeAnisotropy&); // Not implemented
  void operator=(const vtkmpjLatticeAnisotropy&); // Not Implemented
};

#endif
