/*=========================================================================

  Program:   vtkmpjInterpolatedTensorField.cpp

  Copyright (c) Marcel P. Jackowski, Choukri Mekkaoui
  All rights reserved.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkmpjInterpolatedTensorField.h"

#include "vtkDataSet.h"
#include "vtkGenericCell.h"
#include "vtkObjectFactory.h"

//---------------------------------------------------------------------------
vtkStandardNewMacro( vtkmpjInterpolatedTensorField ); 

//----------------------------------------------------------------------------
void vtkmpjInterpolatedTensorField::AddDataSet( vtkDataSet * dataset )
{
  if ( !dataset )
    {
    return;
    }

  // insert the dataset (do NOT register the dataset to 'this')
  this->DataSets->push_back( dataset );

  int size = dataset->GetMaxCellSize();
  if ( size > this->WeightsSize )
    {
    this->WeightsSize = size;
    if ( this->Weights )
      {
      delete[] this->Weights;
      this->Weights = NULL;
      }
    this->Weights = new double[size]; 
    }
}

//----------------------------------------------------------------------------
void vtkmpjInterpolatedTensorField::SetLastCellId( vtkIdType c, int dataindex )
{
  this->LastCellId  = c; 
  this->LastDataSet = ( *this->DataSets )[dataindex];
  
  // if the dataset changes, then the cached cell is invalidated
  // we might as well prefetch the cached cell either way
  if ( this->LastCellId != -1 )
  {
    this->LastDataSet->GetCell( this->LastCellId, this->GenCell );
  } 
  
  this->LastDataSetIndex = dataindex;
}

//----------------------------------------------------------------------------
int vtkmpjInterpolatedTensorField::TensorValues( double * x, double * u, double ** t )
{
  vtkDataSet * ds;
  if(!this->LastDataSet && !this->DataSets->empty())
    {
    ds = ( *this->DataSets )[0];
    this->LastDataSet      = ds;
    this->LastDataSetIndex = 0;
    }
  else
    {
    ds = this->LastDataSet;
    }
    
  int retVal = this->TensorValues( ds, x, u, t );
  
  if ( !retVal )
    {
    for( this->LastDataSetIndex = 0; 
         this->LastDataSetIndex < static_cast<int>( this->DataSets->size() );
         this->LastDataSetIndex ++ )
      {
      ds = this->DataSets->operator[]( this->LastDataSetIndex );
      if( ds && ds != this->LastDataSet )
        {
        this->ClearLastCellId();
        retVal = this->TensorValues( ds, x, u, t );
        if ( retVal ) 
          {
            this->LastDataSet = ds;
            return retVal;
          }
        }
      }
    this->LastCellId  = -1;
    this->LastDataSetIndex = 0;
    this->LastDataSet = (*this->DataSets)[0];
    return 0;
    }
    
  return retVal;
}

//----------------------------------------------------------------------------
int vtkmpjInterpolatedTensorField::FunctionValues( double * x, double * f )
{
  vtkDataSet * ds;
  if(!this->LastDataSet && !this->DataSets->empty())
    {
    ds = ( *this->DataSets )[0];
    this->LastDataSet      = ds;
    this->LastDataSetIndex = 0;
    }
  else
    {
    ds = this->LastDataSet;
    }
    
  int retVal = this->FunctionValues( ds, x, f );
  
  if ( !retVal )
    {
    for( this->LastDataSetIndex = 0; 
         this->LastDataSetIndex < static_cast<int>( this->DataSets->size() );
         this->LastDataSetIndex ++ )
      {
      ds = this->DataSets->operator[]( this->LastDataSetIndex );
      if( ds && ds != this->LastDataSet )
        {
        this->ClearLastCellId();
        retVal = this->FunctionValues( ds, x, f );
        if ( retVal ) 
          {
            this->LastDataSet = ds;
            return retVal;
          }
        }
      }
    this->LastCellId  = -1;
    this->LastDataSetIndex = 0;
    this->LastDataSet = (*this->DataSets)[0];
    return 0;
    }
    
  return retVal;
}

//----------------------------------------------------------------------------
void vtkmpjInterpolatedTensorField::PrintSelf( ostream & os, vtkIndent indent )
{
  this->Superclass::PrintSelf( os, indent );
}
