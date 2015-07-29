

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
  -- vtkmpjFastMarchingTractography.h --
   
  Implements DTI WM tractography with Sethian's Fast Marching Method.

  Marcel Jackowski, March 2003
*/

#ifndef __vtkmpjFastMarchingTractography_h
#define __vtkmpjFastMarchingTractography_h

#include "vtkmpjFastMarchingMethod.h"

class vtkmpjFastMarchingTractography : public vtkmpjFastMarchingMethod
{
 public: 
  static vtkmpjFastMarchingTractography *New();
  vtkTypeMacro(vtkmpjFastMarchingTractography,vtkpxSimpleImageToImageFilter);
  
  vtkGetMacro(SpeedFunctionIndex, int);
  vtkSetMacro(SpeedFunctionIndex, int);

 protected:
  int SpeedFunctionIndex;
  vtkmpjFastMarchingTractography();
  virtual ~vtkmpjFastMarchingTractography();
 
  double F0(vtkImageData *input, vtkImageData *output, 
	    int coord[3], int index, double normal[3]);
  double F1(vtkImageData *input, vtkImageData *output, 
	    int coord[3], int index, double normal[3]); 

  // The speed function 
  virtual double F(vtkImageData *input, vtkImageData *output, 
		   int coord[3], int index, double normal[3]); 
 
 private: 
  vtkmpjFastMarchingTractography(const vtkmpjFastMarchingTractography&); // Not implemented
  void operator=(const vtkmpjFastMarchingTractography&); // Not Implemented
};

#endif
