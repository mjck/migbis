

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


#ifndef __vtkmpjScalarsToFieldData_h
#define __vtkmpjScalarsToFieldData_h

#include "vtkDataSetToDataSetFilter.h"
#include "vtkDataSet.h"
#include "vtkPointData.h"

class vtkmpjScalarsToFieldData : public vtkDataSetToDataSetFilter
{
 public:
  static vtkmpjScalarsToFieldData *New();
  vtkTypeMacro(vtkmpjScalarsToFieldData,vtkDataSetToDataSetFilter);

  // Description
  // Sets/Gets the name of the new field to hold the scalars
  vtkSetStringMacro(FieldName);
  vtkGetStringMacro(FieldName);

 protected:  
  vtkmpjScalarsToFieldData();
  virtual ~vtkmpjScalarsToFieldData();
  
  char *FieldName;

  void Execute();
  
 private:
  vtkmpjScalarsToFieldData(const vtkmpjScalarsToFieldData&); // Not implemented
  void operator=(const vtkmpjScalarsToFieldData&); // Not implemented
};

#endif
