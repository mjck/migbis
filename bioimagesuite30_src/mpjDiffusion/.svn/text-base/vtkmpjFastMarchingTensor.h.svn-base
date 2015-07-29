

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
  -- vtkmpjFastMarchingTensor.h --
   
  Implements Sethian's Fast Marching Method on 3D DTI Tensor Field.

  Solves the Eikonal equation |Gradient T| = 1 / F, where T is the
  arrival time of the front and F is the speed.

  Marcel Jackowski, January 2003
*/

#ifndef __vtkmpjFastMarchingTensor_h
#define __vtkmpjFastMarchingTensor_h

#include "vtkmpjFastMarching1stOrder.h"
#include "vtkmpjFastMarching2ndOrder.h"

class vtkmpjFastMarchingTensor : public vtkmpjFastMarching1stOrder
{
 public: 
  static vtkmpjFastMarchingTensor *New();
  vtkTypeMacro(vtkmpjFastMarchingTensor,vtkpxSimpleImageToImageFilter);
  
 protected:   
  vtkmpjFastMarchingTensor();
  virtual ~vtkmpjFastMarchingTensor();
  
  // The speed function 
  virtual double F(vtkImageData *input, vtkImageData *output, 
		   int coord[3], int index); 
 private: 
  vtkmpjFastMarchingTensor(const vtkmpjFastMarchingTensor&); // Not implemented
  void operator=(const vtkmpjFastMarchingTensor&); // Not Implemented
};

#endif
