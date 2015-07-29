//BIOIMAGESUITE_LICENSE  ---------------------------------------------------------------------------------
//BIOIMAGESUITE_LICENSE  This file is part of the BioImage Suite Software Package.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  X. Papademetris, M. Jackowski, N. Rajeevan, H. Okuda, R.T. Constable, and L.H
//BIOIMAGESUITE_LICENSE  Staib. BioImage Suite: An integrated medical image analysis suite, Section
//BIOIMAGESUITE_LICENSE  of Bioimaging Sciences, Dept. of Diagnostic Radiology, Yale School of
//BIOIMAGESUITE_LICENSE  Medicine, http://www.bioimagesuite.org.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is free software; you can redistribute it and/or
//BIOIMAGESUITE_LICENSE  modify it under the terms of the GNU General Public License version 2
//BIOIMAGESUITE_LICENSE  as published by the Free Software Foundation.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  This program is distributed in the hope that it will be useful,
//BIOIMAGESUITE_LICENSE  but WITHOUT ANY WARRANTY; without even the implied warranty of
//BIOIMAGESUITE_LICENSE  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//BIOIMAGESUITE_LICENSE  GNU General Public License for more details.
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  You should have received a copy of the GNU General Public License
//BIOIMAGESUITE_LICENSE  along with this program; if not, write to the Free Software
//BIOIMAGESUITE_LICENSE  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//BIOIMAGESUITE_LICENSE  See also  http://www.gnu.org/licenses/gpl.html
//BIOIMAGESUITE_LICENSE  
//BIOIMAGESUITE_LICENSE  If this software is modified please retain this statement and add a notice
//BIOIMAGESUITE_LICENSE  that it had been modified (and by whom).  
//BIOIMAGESUITE_LICENSE 
//BIOIMAGESUITE_LICENSE  -----------------------------------------------------------------------------------



/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkjwsPDEDeformableRegistrationFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/12/21 22:47:28 $
  Version:   $Revision: 1.30 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkjwsPDEDeformableRegistrationFilter_txx_
#define _itkjwsPDEDeformableRegistrationFilter_txx_

#include "itkjwsPDEDeformableRegistrationFilter.h"

#include "itkExceptionObject.h"
#include "itkImageRegionIterator.h"
#include "itkImageLinearIteratorWithIndex.h"
#include "itkDataObject.h"

#include "itkGaussianOperator.h"
#include "itkVectorNeighborhoodOperatorImageFilter.h"

#include "vnl/vnl_math.h"

namespace itk {

/*
 * Default constructor
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::jwsPDEDeformableRegistrationFilter()
{
 
  this->SetNumberOfRequiredInputs(2);

  this->SetNumberOfIterations(10);
 
  unsigned int j;
  for( j = 0; j < ImageDimension; j++ )
    {
    m_StandardDeviations[j] = 1.0;
    m_UpdateFieldStandardDeviations[j] = 1.0;
    }

  m_TempField = DeformationFieldType::New();
  m_MaximumError = 0.1;
  m_MaximumKernelWidth = 30;
  m_StopRegistrationFlag = false;

  m_SmoothDeformationField = true;
  m_SmoothUpdateField = false;
}


/*
 * Set the fixed image.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::SetFixedImage(
  const FixedImageType * ptr )
{
  this->ProcessObject::SetNthInput( 1, const_cast< FixedImageType * >( ptr ) );
}


/*
 * Get the fixed image.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
const typename jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::FixedImageType *
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GetFixedImage() const
{
  return dynamic_cast< const FixedImageType * >
    ( this->ProcessObject::GetInput( 1 ) );
}


/*
 * Set the moving image.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::SetMovingImage(
  const MovingImageType * ptr )
{
  this->ProcessObject::SetNthInput( 2, const_cast< MovingImageType * >( ptr ) );
}


/*
 * Get the moving image.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
const typename jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::MovingImageType *
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GetMovingImage() const
{
  return dynamic_cast< const MovingImageType * >
    ( this->ProcessObject::GetInput( 2 ) );
}


/*
 * 
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
std::vector<SmartPointer<DataObject> >::size_type
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GetNumberOfValidRequiredInputs() const
{
  typename std::vector<SmartPointer<DataObject> >::size_type num = 0;

  if (this->GetFixedImage())
    {
    num++;
    }

  if (this->GetMovingImage())
    {
    num++;
    }
  
  return num;
}


/*
 * Set the standard deviations.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::SetStandardDeviations(
  double value )
{

  unsigned int j;
  for( j = 0; j < ImageDimension; j++ )
    {
    if( value != m_StandardDeviations[j] )
      {
      break;
      }
    }
  if( j < ImageDimension )
    {
    this->Modified();
    for( j = 0; j < ImageDimension; j++ )
      {
      m_StandardDeviations[j] = value;
      }
    }

}

/*
 * Set the standard deviations.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::SetUpdateFieldStandardDeviations(
  double value )
{

  unsigned int j;
  for( j = 0; j < ImageDimension; j++ )
    {
    if( value != m_UpdateFieldStandardDeviations[j] )
      {
      break;
      }
    }
  if( j < ImageDimension )
    {
    this->Modified();
    for( j = 0; j < ImageDimension; j++ )
      {
      m_UpdateFieldStandardDeviations[j] = value;
      }
    }

}


/*
 * Standard PrintSelf method.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Smooth deformation field: "
     << (m_SmoothDeformationField ? "on" : "off") << std::endl;
  os << indent << "Standard deviations: [";
  unsigned int j;
  for( j = 0; j < ImageDimension - 1; j++ )
    {
    os << m_StandardDeviations[j] << ", ";
    }
  os << m_StandardDeviations[j] << "]" << std::endl;
  os << indent << "Smooth update field: "
     << (m_SmoothUpdateField ? "on" : "off") << std::endl;
  os << indent << "Update field standard deviations: [";
  for( j = 0; j < ImageDimension - 1; j++ )
    {
    os << m_UpdateFieldStandardDeviations[j] << ", ";
    }
  os << m_UpdateFieldStandardDeviations[j] << "]" << std::endl;
  os << indent << "StopRegistrationFlag: ";
  os << m_StopRegistrationFlag << std::endl;
  os << indent << "MaximumError: ";
  os << m_MaximumError << std::endl;
  os << indent << "MaximumKernelWidth: ";
  os << m_MaximumKernelWidth << std::endl; 

}


/*
 * Set the function state values before each iteration
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::InitializeIteration()
{

  MovingImageConstPointer movingPtr = this->GetMovingImage();
  FixedImageConstPointer fixedPtr = this->GetFixedImage();

  if( !movingPtr || !fixedPtr )
    {
    itkExceptionMacro( << "Fixed and/or moving image not set" );
    }

  // update variables in the equation object
  jwsPDEDeformableRegistrationFunctionType *f = 
    dynamic_cast<jwsPDEDeformableRegistrationFunctionType *>
    (this->GetDifferenceFunction().GetPointer());

  if ( !f )
    {
    itkExceptionMacro(<<"FiniteDifferenceFunction not of type jwsPDEDeformableRegistrationFilterFunction");
    }

  f->SetFixedImage( fixedPtr );
  f->SetMovingImage( movingPtr );

  this->Superclass::InitializeIteration();           

}


/*
 * Override the default implemenation for the case when the 
 * initial deformation is not set.
 * If the initial deformation is not set, the output is
 * fill with zero vectors.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::CopyInputToOutput()
{

  typename Superclass::InputImageType::ConstPointer  inputPtr  = this->GetInput();
  
  if( inputPtr )
    {
    this->Superclass::CopyInputToOutput();
    }
  else
    {
    typename Superclass::PixelType zeros;
    for( unsigned int j = 0; j < ImageDimension; j++ )
      {
      zeros[j] = 0;
      }

    typename OutputImageType::Pointer output = this->GetOutput();
  
    ImageRegionIterator<OutputImageType> out(output, output->GetRequestedRegion());

    while( ! out.IsAtEnd() )
      {
      out.Value() =  zeros;
      ++out;
      }
    }
}


template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GenerateOutputInformation()
{

  typename DataObject::Pointer output;

  if( this->GetInput(0) )
    {
    // Initial deformation field is set.
    // Copy information from initial field.
    this->Superclass::GenerateOutputInformation();

    }
  else if( this->GetFixedImage() )
    {
    // Initial deforamtion field is not set. 
    // Copy information from the fixed image.
    for (unsigned int idx = 0; idx < 
           this->GetNumberOfOutputs(); ++idx )
      {
      output = this->GetOutput(idx);
      if (output)
        {
        output->CopyInformation(this->GetFixedImage());
        }  
      }

    }

}


template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GenerateInputRequestedRegion()
{

  // call the superclass's implementation
  Superclass::GenerateInputRequestedRegion();

  // request the largest possible region for the moving image
  MovingImagePointer movingPtr = 
    const_cast< MovingImageType * >( this->GetMovingImage() );
  if( movingPtr )
    {
    movingPtr->SetRequestedRegionToLargestPossibleRegion();
    }
  
  // just propagate up the output requested region for
  // the fixed image and initial deformation field.
  DeformationFieldPointer inputPtr = 
    const_cast< DeformationFieldType * >( this->GetInput() );
  DeformationFieldPointer outputPtr = this->GetOutput();
  FixedImagePointer fixedPtr = 
    const_cast< FixedImageType *>( this->GetFixedImage() );

  if( inputPtr )
    {
    inputPtr->SetRequestedRegion( outputPtr->GetRequestedRegion() );
    }

  if( fixedPtr )
    {
    fixedPtr->SetRequestedRegion( outputPtr->GetRequestedRegion() );
    }

}


/*
 * Release memory of internal buffers
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::PostProcessOutput()
{
  this->Superclass::PostProcessOutput();
  m_TempField->Initialize();
}


/*
 * Initialize flags
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::Initialize()
{
  this->Superclass::Initialize();
  m_StopRegistrationFlag = false;
}


/*
 * Smooth deformation using a separable Gaussian kernel
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::SmoothDeformationField()
{

  DeformationFieldPointer field = this->GetOutput();

  // copy field to TempField
  m_TempField->SetSpacing( field->GetSpacing() );
  m_TempField->SetOrigin( field->GetOrigin() );
  m_TempField->SetLargestPossibleRegion( 
    field->GetLargestPossibleRegion() );
  m_TempField->SetRequestedRegion(
    field->GetRequestedRegion() );
  m_TempField->SetBufferedRegion( field->GetBufferedRegion() );
  m_TempField->Allocate();
  
  typedef typename DeformationFieldType::PixelType VectorType;
  typedef typename VectorType::ValueType           ScalarType;
  typedef GaussianOperator<ScalarType,ImageDimension> OperatorType;
  typedef VectorNeighborhoodOperatorImageFilter<
    DeformationFieldType,
    DeformationFieldType> SmootherType;

  OperatorType * oper = new OperatorType;
  typename SmootherType::Pointer smoother = SmootherType::New();

  typedef typename DeformationFieldType::PixelContainerPointer 
    PixelContainerPointer;
  PixelContainerPointer swapPtr;

  // graft the output field onto the mini-pipeline
  smoother->GraftOutput( m_TempField );

  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    // smooth along this dimension
    oper->SetDirection( j );
    double variance = vnl_math_sqr( m_StandardDeviations[j] );
    oper->SetVariance( variance );
    oper->SetMaximumError( m_MaximumError );
    oper->SetMaximumKernelWidth( m_MaximumKernelWidth );
    oper->CreateDirectional();

    // todo: make sure we only smooth within the buffered region
    smoother->SetOperator( *oper );
    smoother->SetInput( field );
    smoother->Update();

    if ( j < ImageDimension - 1 )
      {
      // swap the containers
      swapPtr = smoother->GetOutput()->GetPixelContainer();
      smoother->GraftOutput( field );
      field->SetPixelContainer( swapPtr );
      smoother->Modified();
      }

    }

  // graft the output back to this filter
  m_TempField->SetPixelContainer( field->GetPixelContainer() );
  this->GraftOutput( smoother->GetOutput() );

  delete oper;

}

/*
 * Smooth deformation using a separable Gaussian kernel
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsPDEDeformableRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::SmoothUpdateField()
{
  // The update buffer will be overwritten with new data.
  DeformationFieldPointer field = this->GetUpdateBuffer();

  typedef typename DeformationFieldType::PixelType VectorType;
  typedef typename VectorType::ValueType           ScalarType;
  typedef GaussianOperator<ScalarType,ImageDimension> OperatorType;
  typedef VectorNeighborhoodOperatorImageFilter<
    DeformationFieldType,
    DeformationFieldType> SmootherType;
  
  OperatorType opers[ImageDimension];
  typename SmootherType::Pointer smoothers[ImageDimension];

  for( unsigned int j = 0; j < ImageDimension; j++ )
    {
    // smooth along this dimension
    opers[j].SetDirection( j );
    double variance = vnl_math_sqr( this->GetUpdateFieldStandardDeviations()[j] );
    //double variance = vnl_math_sqr( 1.0 );
    opers[j].SetVariance( variance );
    opers[j].SetMaximumError( this->GetMaximumError() );
    opers[j].SetMaximumKernelWidth( this->GetMaximumKernelWidth() );
    opers[j].CreateDirectional();

    smoothers[j] = SmootherType::New();
    smoothers[j]->SetOperator( opers[j] );
    smoothers[j]->ReleaseDataFlagOn();

    if (j > 0)
      {
      smoothers[j]->SetInput( smoothers[j-1]->GetOutput() );
      }
    }
  smoothers[0]->SetInput( field );
  smoothers[ImageDimension-1]->GetOutput()
    ->SetRequestedRegion( field->GetBufferedRegion() );

  smoothers[ImageDimension-1]->Update();
  
  // field to contain the final smoothed data, do the equivalent of a graft
  field->SetPixelContainer( smoothers[ImageDimension-1]->GetOutput()
                            ->GetPixelContainer() );
  field->SetRequestedRegion( smoothers[ImageDimension-1]->GetOutput()
                             ->GetRequestedRegion() );
  field->SetBufferedRegion( smoothers[ImageDimension-1]->GetOutput()
                            ->GetBufferedRegion() );
  field->SetLargestPossibleRegion( smoothers[ImageDimension-1]->GetOutput()
                                   ->GetLargestPossibleRegion() );
  field->CopyInformation( smoothers[ImageDimension-1]->GetOutput() );
}


} // end namespace itk

#endif



