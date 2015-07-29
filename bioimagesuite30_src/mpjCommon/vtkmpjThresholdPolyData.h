
#ifndef __vtkmpjThresholdPolyData_h
#define __vtkmpjThresholdPolyData_h

#include "vtkDataSetToPolyDataFilter.h"

#define VTK_ATTRIBUTE_MODE_DEFAULT 0
#define VTK_ATTRIBUTE_MODE_USE_POINT_DATA 1
#define VTK_ATTRIBUTE_MODE_USE_CELL_DATA 2

class vtkmpjThresholdPolyData : public vtkDataSetToPolyDataFilter
{
public:
  static vtkmpjThresholdPolyData *New();
  vtkTypeMacro(vtkmpjThresholdPolyData,vtkDataSetToPolyDataFilter);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Criterion is cells whose scalars are less or equal to lower threshold.
  void ThresholdByLower(float lower);

  // Description:
  // Criterion is cells whose scalars are greater or equal to upper threshold.
  void ThresholdByUpper(float upper);

  // Description:
  // Criterion is cells whose scalars are between lower and upper thresholds.
  void ThresholdBetween(float lower, float upper);

  // Description:
  // Get the Upper and Lower thresholds.
  vtkGetMacro(UpperThreshold,float);
  vtkGetMacro(LowerThreshold,float);

  // Description:
  // Control how the filter works with scalar point data and cell attribute
  // data.  By default (AttributeModeToDefault), the filter will use point
  // data, and if no point data is available, then cell data is
  // used. Alternatively you can explicitly set the filter to use point data
  // (AttributeModeToUsePointData) or cell data (AttributeModeToUseCellData).
  vtkSetMacro(AttributeMode,int);
  vtkGetMacro(AttributeMode,int);
  void SetAttributeModeToDefault() 
    {this->SetAttributeMode(VTK_ATTRIBUTE_MODE_DEFAULT);};
  void SetAttributeModeToUsePointData() 
    {this->SetAttributeMode(VTK_ATTRIBUTE_MODE_USE_POINT_DATA);};
  void SetAttributeModeToUseCellData() 
    {this->SetAttributeMode(VTK_ATTRIBUTE_MODE_USE_CELL_DATA);};
  const char *GetAttributeModeAsString();

  // Description:
  // If using scalars from point data, all scalars for all points in a cell 
  // must satisfy the threshold criterion if AllScalars is set. Otherwise, 
  // just a single scalar value satisfying the threshold criterion enables
  // will extract the cell.
  vtkSetMacro(AllScalars,int);
  vtkGetMacro(AllScalars,int);
  vtkBooleanMacro(AllScalars,int);
  
protected:
  vtkmpjThresholdPolyData();
  ~vtkmpjThresholdPolyData();

//BTX
  // This is temporary solution. The vtkCutMaterial must be able
  // to call SelectInputScalars().
  friend class vtkCutMaterial;

  // Description:
  // If you want to threshold by an arbitrary array, then set its name here.
  // By default this in NULL and the filter will use the active scalar array.
  vtkGetStringMacro(InputScalarsSelection);
  virtual void SelectInputScalars(const char *fieldName) 
    {this->SetInputScalarsSelection(fieldName);}
//ETX
  
  // Usual data generation method
  void Execute();

  int   AllScalars;
  float LowerThreshold;
  float UpperThreshold;
  int   AttributeMode;

  //BTX
  int (vtkmpjThresholdPolyData::*ThresholdFunction)(float s);
  //ETX

  int Lower(float s) {return ( s <= this->LowerThreshold ? 1 : 0 );};
  int Upper(float s) {return ( s >= this->UpperThreshold ? 1 : 0 );};
  int Between(float s) {return ( s >= this->LowerThreshold ? 
                               ( s <= this->UpperThreshold ? 1 : 0 ) : 0 );};

  char *InputScalarsSelection;
  vtkSetStringMacro(InputScalarsSelection);

private:
  vtkmpjThresholdPolyData(const vtkmpjThresholdPolyData&);  // Not implemented.
  void operator=(const vtkmpjThresholdPolyData&);  // Not implemented.
};

#endif
