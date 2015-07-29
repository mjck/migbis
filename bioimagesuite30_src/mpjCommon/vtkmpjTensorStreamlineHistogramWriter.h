

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


#ifndef __vtkmpjTensorStreamlineHistogramWriter_h
#define __vtkmpjTensorStreamlineHistogramWriter_h

#include "vtkPolyDataToPolyDataFilter.h"
#include "vtkCellArray.h"
#include "vtkCellData.h"
#include "vtkDataArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkPoints.h"

class vtkmpjTensorStreamlineHistogramWriter : public vtkPolyDataToPolyDataFilter
{
public:
  vtkTypeMacro(vtkmpjTensorStreamlineHistogramWriter,vtkPolyDataToPolyDataFilter);

  // Description:
  // Construct object 
  static vtkmpjTensorStreamlineHistogramWriter *New();

  // Description:
  // Get/Set FileName
  vtkGetStringMacro(FileName);
  vtkSetStringMacro(FileName);

  vtkGetMacro(NumberOfBins,int);
  vtkSetMacro(NumberOfBins,int);

 protected:
  vtkmpjTensorStreamlineHistogramWriter();
  ~vtkmpjTensorStreamlineHistogramWriter() {}
  
  // Usual data generation method
  void Execute();

  char *FileName;

  int NumberOfBins;

private:
  vtkmpjTensorStreamlineHistogramWriter(const vtkmpjTensorStreamlineHistogramWriter&);// Not implemented.
  void operator=(const vtkmpjTensorStreamlineHistogramWriter&);  // Not implemented.
};

#endif
