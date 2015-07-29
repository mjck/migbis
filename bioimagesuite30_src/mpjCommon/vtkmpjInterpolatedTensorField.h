/*=========================================================================

  Program:   vtkmpjInterpolatedTensorField.h

  Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkmpjInterpolatedTensorField - A concrete class for obtaining
//  the interpolated velocity values at a point.
//
// .SECTION Description
//  vtkmpjInterpolatedTensorField acts as a continuous velocity field via
//  cell interpolation on a vtkDataSet, NumberOfIndependentVariables = 4
//  (x,y,z,t) and NumberOfFunctions = 3 (u,v,w). As a concrete sub-class 
//  of vtkAbstractInterpolatedVelocityField, this class adopts two levels 
//  of cell caching for faster though less robust cell location than its 
//  sibling class vtkCellLocatorInterpolatedVelocityField. Level #0 begins 
//  with intra-cell caching. Specifically, if the previous cell is valid 
//  and the nex point is still within it, ( vtkCell::EvaluatePosition() 
//  returns 1, coupled with the new parametric coordinates and weights ), 
//  the function values are interpolated and vtkCell::EvaluatePosition() 
//  is invoked only. If it fails, level #1 follows by inter-cell location 
//  of the target cell (that contains the next point). By inter-cell, the 
//  previous cell gives an important clue / guess or serves as an immediate
//  neighbor to aid in the location of the target cell (as is typically the 
//  case with integrating a streamline across cells) by means of vtkDataSet::
//  FindCell(). If this still fails, a global cell search is invoked via 
//  vtkDataSet::FindCell(). 
//  
//  Regardless of inter-cell or global search, vtkPointLocator is employed 
//  as a crucial tool underlying the cell locator. The use of vtkPointLocator
//  casues vtkmpjInterpolatedTensorField to return false target cells for 
//  datasets defined on complex grids.
//
// .SECTION Caveats
//  vtkmpjInterpolatedTensorField is not thread safe. A new instance should be
//  created by each thread.

// .SECTION See Also
//  vtkmpjAbstractInterpolatedTensorField vtkCellLocatorInterpolatedVelocityField
//  vtkGenericInterpolatedVelocityField vtkCachingInterpolatedVelocityField
//  vtkTemporalInterpolatedVelocityField vtkFunctionSet vtkStreamer vtkStreamTracer

#ifndef __vtkmpjInterpolatedTensorField_h
#define __vtkmpjInterpolatedTensorField_h

#include "vtkmpjAbstractInterpolatedTensorField.h"

class vtkmpjInterpolatedTensorField 
  : public vtkmpjAbstractInterpolatedTensorField
{
public:
  vtkTypeMacro( vtkmpjInterpolatedTensorField,
                        vtkmpjAbstractInterpolatedTensorField );
  void PrintSelf( ostream & os, vtkIndent indent );

  // Description:
  // Construct a vtkmpjInterpolatedTensorField without an initial dataset.
  // Caching is set on and LastCellId is set to -1.
  static vtkmpjInterpolatedTensorField * New();
  
  // Description:
  // Add a dataset used for the implicit function evaluation. If more than
  // one dataset is added, the evaluation point is searched in all until a 
  // match is found. THIS FUNCTION DOES NOT CHANGE THE REFERENCE COUNT OF 
  // DATASET FOR THREAD SAFETY REASONS.
  virtual void AddDataSet( vtkDataSet * dataset );
  
  // Description:
  // Evaluate the tensor field f at point (x, y, z).
  virtual int FunctionValues( double * x, double * f );
  virtual int TensorValues ( double * x, double * u, double ** t);
  
  // Description:
  // Set the cell id cached by the last evaluation within a specified dataset.
  virtual void SetLastCellId( vtkIdType c, int dataindex );
  
  // Description:
  // Set the cell id cached by the last evaluation.
  virtual void SetLastCellId( vtkIdType c ) 
    { this->Superclass::SetLastCellId( c ); }

protected:
  vtkmpjInterpolatedTensorField() { }
  ~vtkmpjInterpolatedTensorField() { }

  // Description:
  // Evaluate the velocity field f at point (x, y, z) in a specified dataset
  // by either involving vtkPointLocator, via vtkPointSet::FindCell(), in
  // locating the next cell (for datasets of type vtkPointSet) or simply
  // invoking vtkImageData/vtkRectilinearGrid::FindCell() to fulfill the same
  // task if the point is outside the current cell.
  virtual int FunctionValues( vtkDataSet * ds, double * x, double * f )
    { return this->Superclass::FunctionValues( ds, x, f ); }

  virtual int TensorValues ( vtkDataSet *ds, double * x, double * u, double **t )
    { return this->Superclass::TensorValues( ds, x, u, t); }

private:
  vtkmpjInterpolatedTensorField
    ( const vtkmpjInterpolatedTensorField & );  // Not implemented.
  void operator = 
    ( const vtkmpjInterpolatedTensorField & );  // Not implemented.
};

#endif
