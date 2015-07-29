

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
  -- vtkmpjLevelSetMethod.h --
   
  Implements Sethian's Level Set Method on 3D uniform lattice domains.

  Marcel Jackowski, May 2003.
*/

#ifndef __vtkmpjLevelSetMethod_h
#define __vtkmpjLevelSetMethod_h

#include "vtkObjectFactory.h"
#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkmpjPriorityQueue.h"
#include "vtkImageAccumulate.h"
#include "vtkFloatArray.h"
#include "vtkIntArray.h"
#include "vtkMath.h"
#include "math.h"

class vtkmpjLevelSetMethod : public vtkpxSimpleImageToImageFilter
{
 public: 
  static vtkmpjLevelSetMethod *New();
  vtkTypeMacro(vtkmpjLevelSetMethod,vtkpxSimpleImageToImageFilter);
  
  // Set/Get time step size
  vtkGetMacro(TimeStepSize,float);
  vtkSetMacro(TimeStepSize,float);  

  // Set/Get narrow band size
  vtkGetMacro(NarrowBandwidth,float);
  vtkSetMacro(NarrowBandwidth,float);  

  // Set/Get maximum propagation time
  vtkGetMacro(PropagationTime,float);
  vtkSetMacro(PropagationTime,float);  
  
  // Set/Get grid spacing
  vtkGetMacro(Spacing,float);
  vtkSetMacro(Spacing,float);
  
 protected:
  vtkFloatArray *NarrowBandPoints;
  float TimeStepSize;    // default 0.5
  float NarrowBandwidth; // default 12
  float PropagationTime;
  float Spacing;

  vtkmpjLevelSetMethod();
  virtual ~vtkmpjLevelSetMethod();
    
  void ExecuteInformation();
  
  void ComputeNormal(vtkImageData *input, int index, int coord[3], float result[3]);
  double ComputeEntropyGradientMagnitude(vtkImageData *input, double speed, int index, int coord[3]);
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  virtual void Initialize(vtkImageData *input, vtkImageData *output);
  virtual void Propagate(vtkImageData *input, vtkImageData *output);
  virtual double ComputeSpeed(vtkImageData *input, int index, int coord[3]);
  
 private:
  //BTX
  float pog;
  int count, tenth;  
  vtkImageData *InputImage;
  vtkImageData *OutputImage;

  void CopyImage(vtkImageData *source, vtkImageData *dest);
  void SwapImages();

  //ETX
  
  vtkmpjLevelSetMethod(const vtkmpjLevelSetMethod&);// Not implemented
  void operator=(const vtkmpjLevelSetMethod&); // Not Implemented
};

#endif
