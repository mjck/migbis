

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


/* -- vtkmpjCardiacAngle.h 
   
      Marcel Jackowski, 2012

      This class computes the fiber angle with regard to a reference plane.

*/

#ifndef __vtkmpjCardiacAngle_h
#define __vtkmpjCardiacAngle_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkMath.h"

class vtkmpjCardiacAngle : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjCardiacAngle *New();
  vtkTypeMacro(vtkmpjCardiacAngle,vtkpxSimpleImageToImageFilter);

  // Description:
  // Sets/Gets mask where angles are to be computed
  vtkGetObjectMacro(Mask, vtkImageData);
  vtkSetObjectMacro(Mask, vtkImageData);

  // Description:
  // Sets/Gets eigenvectors 
  vtkGetObjectMacro(Eigenvectors, vtkImageData);
  vtkSetObjectMacro(Eigenvectors, vtkImageData);

  // Description:
  // Set/Get eigenvector to use
  vtkGetMacro(EigenvectorType, int);
  vtkSetMacro(EigenvectorType, int);

  // Description:
  // Set/Get Angle representation
  vtkGetMacro(AngleType, int);
  vtkSetMacro(AngleType, int);

  // Description:
  // Set/Get reference plane
  // 0: Circumferential, 1: Radial, 2: Longitudinal
  vtkGetMacro(ReferencePlane, int);
  vtkSetMacro(ReferencePlane, int);
		
 	// Description:
  // Set/Get Source points
  //vtkGetObjectMacro(Source, vtkDataSet);
  //vtkSetObjectMacro(Source, vtkDataSet);

  // Description:
  // Sets/Gets mask where eigenvalues are to be computed
  //vtkGetObjectMacro(PolyData, vtkPolyData);
  //vtkSetObjectMacro(PolyData, vtkPolyData);

 protected:
  int AngleType;
  int EigenvectorType;
	int ReferencePlane;
  vtkImageData *Mask;
	vtkImageData *Eigenvectors;
  
  vtkmpjCardiacAngle();
  virtual ~vtkmpjCardiacAngle();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();
  
 private:
  vtkmpjCardiacAngle(const vtkmpjCardiacAngle&); // Not implemented
  void operator=(const vtkmpjCardiacAngle&); // Not Implemented
  
	int InsideMask(long index);	
  void ProjectOntoPlane(double v[3], double n[3], double r[3]);
	double ComputeAngle(vtkDataArray *in, vtkDataArray *eig, int eigenvector, int index);	

};

#endif
