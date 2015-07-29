

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


/* -- vtkmpjHeartFiberAngle.h 
   
      Marcel Jackowski, 2006

      This class computes the fiber angle.

*/

#ifndef __vtkmpjHeartFiberAngle_h
#define __vtkmpjHeartFiberAngle_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkImageData.h"
#include "vtkPointData.h"
#include "vtkMath.h"

class vtkmpjHeartFiberAngle : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjHeartFiberAngle *New();
  vtkTypeMacro(vtkmpjHeartFiberAngle,vtkpxSimpleImageToImageFilter);

  // Description:
  // Sets/Gets mask where eigenvalues are to be computed
  vtkGetObjectMacro(Mask, vtkImageData);
  vtkSetObjectMacro(Mask, vtkImageData);

  // Description:
  // Set/Get Angle representation
  vtkGetMacro(AngleRepresentation, int);
  vtkSetMacro(AngleRepresentation, int);

   // Description:
  // Set/Get Angle representation
  vtkGetMacro(OutputCoordinates, int);
  vtkSetMacro(OutputCoordinates, int);

  // Description:
  // Center of rotation -- base point
  vtkGetVectorMacro(Center, float, 3);
  vtkSetVectorMacro(Center, float, 3);

  // Description: 
  // apex point
  vtkGetVectorMacro(Center2, float, 3);
  vtkSetVectorMacro(Center2, float, 3);

 protected:
  int AngleRepresentation;
  int OutputCoordinates;
  float Center[3];
  float Center2[3];
  vtkImageData *Mask;
  
  vtkmpjHeartFiberAngle();
  virtual ~vtkmpjHeartFiberAngle();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();
  
 private:
  vtkmpjHeartFiberAngle(const vtkmpjHeartFiberAngle&); // Not implemented
  void operator=(const vtkmpjHeartFiberAngle&); // Not Implemented
};

#endif
