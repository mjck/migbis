

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


/* -- vtkmpjHeartSheetAngle.h 
   
      Marcel Jackowski, 2006

      This class computes the fiber angle.

*/

#ifndef __vtkmpjHeartSheetAngle_h
#define __vtkmpjHeartSheetAngle_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkMath.h"

class vtkmpjHeartSheetAngle : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjHeartSheetAngle *New();
  vtkTypeMacro(vtkmpjHeartSheetAngle,vtkpxSimpleImageToImageFilter);

  // Description:
  // Sets/Gets mask where eigenvalues are to be computed
  vtkGetObjectMacro(Mask, vtkImageData);
  vtkSetObjectMacro(Mask, vtkImageData);

  // Description:
  // Center of rotation
  vtkGetVectorMacro(Center, float, 3);
  vtkSetVectorMacro(Center, float, 3);

 protected:
  float Center[3];
  vtkImageData *Mask;
  
  vtkmpjHeartSheetAngle();
  virtual ~vtkmpjHeartSheetAngle();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();
  
 private:
  vtkmpjHeartSheetAngle(const vtkmpjHeartSheetAngle&); // Not implemented
  void operator=(const vtkmpjHeartSheetAngle&); // Not Implemented
};

#endif
