#ifndef __vtkmpjThresholdPolyDataByDistance_h
#define __vtkmpjThresholdPolyDataByDistance_h

#include "vtkPolyDataToPolyDataFilter.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"

class vtkmpjThresholdPolyDataByDistance : public vtkPolyDataToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjThresholdPolyDataByDistance,vtkPolyDataToPolyDataFilter);

  // Description:
  // Construct object 
  static vtkmpjThresholdPolyDataByDistance *New();

  // Set/Get minimum distance
  vtkGetMacro(MinimumDistance,double);
  vtkSetMacro(MinimumDistance,double);

  // Set/Get Origin value
  vtkGetVectorMacro(Origin,double,3);
  vtkSetVectorMacro(Origin,double,3);

 protected:
  vtkmpjThresholdPolyDataByDistance();
  ~vtkmpjThresholdPolyDataByDistance() {}
  
  // Usual data generation method
  void Execute();

  double Origin[3];
  double MinimumDistance;

private:
  vtkmpjThresholdPolyDataByDistance(const vtkmpjThresholdPolyDataByDistance&);// Not implemented.
  void operator=(const vtkmpjThresholdPolyDataByDistance&);  // Not implemented.
};

#endif
