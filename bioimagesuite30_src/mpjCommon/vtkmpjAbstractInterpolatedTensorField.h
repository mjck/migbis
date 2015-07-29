/*=========================================================================

  Program:   vtkmpjAbstractInterpolatedTensorField.h

  Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkmpjAbstractInterpolatedTensorField - An abstract class for 
//  obtaining the interpolated tensor values at a point
//
// .SECTION Description
//  vtkmpjAbstractInterpolatedTensorField acts as a continuous field
//  by performing cell interpolation on the underlying vtkDataSet. This is an
//  abstract sub-class of vtkFunctionSet, NumberOfIndependentVariables = 4
//  (x,y,z,t) and NumberOfFunctions = 6 or 9. With a brute-force scheme,
//  every time an evaluation is performed, the target cell containing point 
//  (x,y,z) needs to be found by calling FindCell(), via either vtkDataSet or
//  vtkAbstractCelllocator's sub-classes (vtkCellLocator & vtkModifiedBSPTree). 
//  As it incurs a large cost, one (for vtkCellLocatorInterpolatedVelocityField
//  via vtkAbstractCellLocator) or two (for vtkInterpolatedVelocityField via 
//  vtkDataSet that involves vtkPointLocator in addressing vtkPointSet) levels
//  of cell caching may be exploited to increase the performance.
//
//  For vtkInterpolatedVelocityField, level #0 begins with intra-cell caching. 
//  Specifically if the previous cell is valid and the next point is still in
//  it ( i.e., vtkCell::EvaluatePosition() returns 1, coupled with newly created 
//  parametric coordinates & weights ), the function values can be interpolated
//  and only vtkCell::EvaluatePosition() is invoked. If this fails, then level #1 
//  follows by inter-cell search for the target cell that contains the next point. 
//  By an inter-cell search, the previous cell provides an important clue or serves 
//  as an immediate neighbor to aid in locating the target cell via vtkPointSet::
//  FindCell(). If this still fails, a global cell location / search is invoked via
//  vtkPointSet::FindCell(). Here regardless of either inter-cell or global search, 
//  vtkPointLocator is in fact employed (for datasets of type vtkPointSet only, note
//  vtkImageData and vtkRectilinearGrid are able to provide rapid and robust cell 
//  location due to the simple mesh topology) as a crucial tool underlying the cell 
//  locator. However, the use of vtkPointLocator makes vtkInterpolatedVelocityField 
//  non-robust in cell location for vtkPointSet.
//  
//  For vtkCellLocatorInterpolatedVelocityField, the only caching (level #0) works
//  by intra-cell trial. In case of failure, a global search for the target cell is 
//  invoked via vtkAbstractCellLocator::FindCell() and the actual work is done by 
//  either vtkCellLocator or vtkModifiedBSPTree (for datasets of type vtkPointSet 
//  only, while vtkImageData and vtkRectilinearGrid themselves are able to provide 
//  fast robust cell location). Without the involvement of vtkPointLocator, robust 
//  cell location is achieved for vtkPointSet. 
//
// .SECTION Caveats
//  vtkmpjAbstractInterpolatedTensorField is not thread safe. A new instance 
//  should be created by each thread.

// .SECTION See Also
//  vtkInterpolatedVelocityField vtkCellLocatorInterpolatedVelocityField 
//  vtkGenericInterpolatedVelocityField vtkCachingInterpolatedVelocityField
//  vtkTemporalInterpolatedVelocityField vtkFunctionSet vtkStreamer vtkStreamTracer

#ifndef __vtkmpjAbstractInterpolatedTensorField_h
#define __vtkmpjAbstractInterpolatedTensorField_h

#include "vtkFunctionSet.h"
//BTX
#include <vector> // STL Header; Required for vector
//ETX

class vtkDataSet;
//BTX
class vtkDataArray;
//ETX
class vtkPointData;
class vtkGenericCell;
class vtkmpjAbstractInterpolatedTensorFieldDataSetsType;

#define VTK_MPJ_INTERPOLATE_MAJOR  0
#define VTK_MPJ_INTERPOLATE_MEDIUM 1
#define VTK_MPJ_INTERPOLATE_MINOR  2

class vtkmpjAbstractInterpolatedTensorField : public vtkFunctionSet
{
public:
  vtkTypeMacro( vtkmpjAbstractInterpolatedTensorField, vtkFunctionSet );
  void PrintSelf( ostream & os, vtkIndent indent );
  
  // Description:
  // Set/Get the caching flag. If this flag is turned ON, there are two levels
  // of caching for derived concrete class vtkInterpolatedVelocityField and one
  // level of caching for derived concrete class vtkCellLocatorInterpolatedVelocityField.
  // Otherwise a global cell location is always invoked for evaluating the 
  // function values at any point.
  vtkSetMacro( Caching, bool );
  vtkGetMacro( Caching, bool );

  // Description:
  // Get the caching statistics. CacheHit refers to the number of level #0 cache
  // hits while CacheMiss is the number of level #0 cache misses.
  vtkGetMacro( CacheHit, int );
  vtkGetMacro( CacheMiss, int );
  
  // Description:
  // Get the most recently visited dataset and it id. The dataset is used 
  // for a guess regarding where the next point will be, without searching
  // through all datasets. When setting the last dataset, care is needed as
  // no reference counting or checks are performed. This feature is intended
  // for custom interpolators only that cache datasets independently.
  vtkGetMacro( LastDataSetIndex, int );
  vtkGetObjectMacro( LastDataSet, vtkDataSet );

  // Description:
  // Get/Set the id of the cell cached from last evaluation.
  vtkGetMacro( LastCellId, vtkIdType );
  virtual void SetLastCellId( vtkIdType c ) { this->LastCellId = c; }
  
  // Description:
  // Set the id of the most recently visited cell of a dataset.
  virtual void SetLastCellId( vtkIdType c, int dataindex ) = 0;
  
  // Description:
  // Get/Set the name of a spcified vector array. By default it is NULL, with
  // the active vector array for use.
  //vtkGetStringMacro( VectorsSelection );
  //void SelectVectors( const char * fieldName )
  //  { this->SetVectorsSelection( fieldName ); }
    
  // Description:
  // Set/Get the flag to make eigenvectors form a consistent
  // coordinate system between subsequent interpolation calls.
  vtkSetMacro( FixEigenvectors, bool );
  vtkGetMacro( FixEigenvectors, bool );
  
  // Description:
  // Set/Get the eigenvector to interpolate
  vtkGetMacro( Eigenvector, int );
  vtkSetClampMacro(Eigenvector,int,
                   VTK_MPJ_INTERPOLATE_MAJOR, VTK_MPJ_INTERPOLATE_MINOR);

   // Description:
  // Set/Get the eigenvector to interpolate
  vtkGetMacro( NormalEigenvector, int );
  vtkSetClampMacro(NormalEigenvector,int,
                   VTK_MPJ_INTERPOLATE_MAJOR, VTK_MPJ_INTERPOLATE_MINOR);

   // Description:
  // Set/Get the eigenvector to interpolate
  vtkGetMacro( BinormalEigenvector, int );
  vtkSetClampMacro(BinormalEigenvector,int,
                   VTK_MPJ_INTERPOLATE_MAJOR, VTK_MPJ_INTERPOLATE_MINOR);

  // Set/Get the eigenvector to flip
  vtkSetMacro( FlipX, bool );
  vtkGetMacro( FlipX, bool );

  // Set/Get the eigenvector to flip
  vtkSetMacro( FlipY, bool );
  vtkGetMacro( FlipY, bool );

  // Set/Get the eigenvector to flip
  vtkSetMacro( FlipZ, bool );
  vtkGetMacro( FlipZ, bool );

  // Description:
  // Import parameters. Sub-classes can add more after chaining.
  virtual void CopyParameters( vtkmpjAbstractInterpolatedTensorField * from )
    { this->Caching = from->Caching; }
  
  // Description:
  // Add a dataset for implicit velocity function evaluation. If more than 
  // one dataset is added, the evaluation point is searched in all until a
  // match is found. THIS FUNCTION DOES NOT CHANGE THE REFERENCE COUNT OF 
  // dataset FOR THREAD SAFETY REASONS.
  virtual void AddDataSet( vtkDataSet * dataset ) = 0;
  
  // Description:
  // Evaluate the tensor field f at point (x, y, z).
  virtual int FunctionValues( double * x, double * f ) = 0;
  virtual int TensorValues( double * x, double * u, double ** t) = 0;

  // Description:
  // Set the last cell id to -1 to incur a global cell search for the next point.
  void ClearLastCellId() { this->LastCellId = -1; }

  // Description: 
  // Get the interpolation weights cached from last evaluation. Return 1 if the
  // cached cell is valid and 0 otherwise.
  int GetLastWeights( double * w );
  int GetLastLocalCoordinates( double pcoords[3] );

  //BTX
  void SetLastVectors(double **current, int iv, int ix, int iy);
  //ETX

protected:
  vtkmpjAbstractInterpolatedTensorField();
  ~vtkmpjAbstractInterpolatedTensorField();
  
  static const double TOLERANCE_SCALE;

  int       CacheHit;
  int       CacheMiss;
  int       WeightsSize;
  int       LastDataSetIndex;
  bool      Caching;
  int       Eigenvector;
  int       NormalEigenvector;
  int       BinormalEigenvector;
  bool      FixEigenvectors;
  bool      FlipX, FlipY, FlipZ;
  double *  Weights;
  double    LastPCoords[3];
  double    LastVectors[3][3];
  vtkIdType LastCellId;
  vtkDataSet *     LastDataSet;
  vtkGenericCell * Cell;
  vtkGenericCell * GenCell; // the current cell
  vtkmpjAbstractInterpolatedTensorFieldDataSetsType * DataSets;
  
  // Description:
  // Set the name of a specific vector to be interpolated. 
  //vtkSetStringMacro( VectorsSelection );
  
  // Description:
  // Evaluate the velocity field f at point (x, y, z) in a specified dataset
  // by invoking vtkDataSet::FindCell() to locate the next cell if the given 
  // point is outside the current cell. To address vtkPointSet, vtkPointLocator
  // is involved via vtkPointSet::FindCell() in vtkInterpolatedVelocityField 
  // for cell location. In vtkCellLocatorInterpolatedVelocityField, this function 
  // is invoked just to handle vtkImageData and vtkRectilinearGrid that are not
  // assigned with any vtkAbstractCellLocatot-type cell locator.
  virtual int FunctionValues( vtkDataSet * ds, double * x, double * f );
  virtual int TensorValues ( vtkDataSet * ds, double * x, double * u, double ** t);
  
//BTX
  friend class vtkTemporalInterpolatedVelocityField;
  // Description:
  // If all weights have been computed (parametric coords etc all valid), a
  // scalar/vector can be quickly interpolated using the known weights and
  // the cached generic cell. This function is primarily reserved for use by 
  // vtkTemporalInterpolatedVelocityField
  void FastCompute( vtkDataArray * vectors, double * f );
  bool InterpolatePoint( vtkPointData * outPD, vtkIdType outIndex );
  vtkGenericCell * GetLastCell()
    { return ( this->LastCellId != -1 ) ? this->GenCell : NULL; }
  // Description:
  // Make eigenvectors form a consistent coordinate system between interpolation
  // calls.
  void FixVectors(double **current, int iv, int ix, int iy);

//ETX

private:
  vtkmpjAbstractInterpolatedTensorField
    ( const vtkmpjAbstractInterpolatedTensorField & );  // Not implemented.
  void operator = ( const vtkmpjAbstractInterpolatedTensorField & );  // Not implemented.
};

//BTX
typedef std::vector< vtkDataSet * > DataSetsTypeBase;
class   vtkmpjAbstractInterpolatedTensorFieldDataSetsType: public DataSetsTypeBase { };
//ETX

#endif
