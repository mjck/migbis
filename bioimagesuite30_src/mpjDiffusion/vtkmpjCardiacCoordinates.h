

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


/* -- vtkmpjCardiacCoordinates.h 
   
      Marcel Jackowski, 2012

      This class takes the radial vector field derived from the FMM
      calculates the specified plane vector for cardiac apps.

*/

#ifndef __vtkmpjCardiacCoordinates_h
#define __vtkmpjCardiacCoordinates_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"
#include "vtkDataArray.h"
#include "vtkImageData.h"
#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkMath.h"

class vtkmpjCardiacCoordinates : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjCardiacCoordinates *New();
  vtkTypeMacro(vtkmpjCardiacCoordinates,vtkpxSimpleImageToImageFilter);

  // Description:
  // Sets/Gets mask where angles are to be computed
  vtkGetObjectMacro(Mask, vtkImageData);
  vtkSetObjectMacro(Mask, vtkImageData);
	
  // Description: Set/Get eigenvalues
  vtkSetVector3Macro(Eigenvalues,double);
  vtkGetVector3Macro(Eigenvalues,double);

  // Description: Turn tensor generation on/off
  vtkGetMacro(OutputTensor,int);
  vtkSetClampMacro(OutputTensor,int,0,1);
  vtkBooleanMacro(OutputTensor,int);

	// Description:
  // Set/Get the apex-base direction vector
	vtkGetVectorMacro(ApexBaseDirection, double, 3);
  vtkSetVectorMacro(ApexBaseDirection, double, 3);

 protected:
  double Eigenvalues[3];
  int OutputTensor;
	double ApexBaseDirection[3];
  vtkImageData *Mask;

  vtkmpjCardiacCoordinates();
  virtual ~vtkmpjCardiacCoordinates();
  
  virtual void SimpleExecute(vtkImageData *input, vtkImageData *output);
  void ExecuteInformation();
  
 private:
	//BTX 
  vtkDataArray *mask;
  //ETX
	void ProjectOntoPlane(double v[3], double n[3], double r[3]);	
  vtkmpjCardiacCoordinates(const vtkmpjCardiacCoordinates&); // Not implemented
  void operator=(const vtkmpjCardiacCoordinates&); // Not Implemented
  
  int InsideMask(int index);
	void FixCoordinates(vtkImageData *input, vtkImageData *output, int coord[3], int index);
	void ComputeCoordinates(vtkImageData *input, int coord[3], int index, double l[3], double r[3], double c[3]);
};

#endif
