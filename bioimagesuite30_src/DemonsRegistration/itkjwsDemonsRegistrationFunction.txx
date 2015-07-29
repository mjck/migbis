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
  Module:    $RCSfile: itkjwsDemonsRegistrationFunction.txx,v $
  Language:  C++
  Date:      $Date: 2004/12/21 22:47:26 $
  Version:   $Revision: 1.30 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkjwsDemonsRegistrationFunction_txx_
#define _itkjwsDemonsRegistrationFunction_txx_

#include "itkjwsDemonsRegistrationFunction.h"
#include "itkExceptionObject.h"
#include "vnl/vnl_math.h"
#include <stdio.h>

namespace itk {

/*
 * Default constructor
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
jwsDemonsRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::jwsDemonsRegistrationFunction()
{

  RadiusType r;
  unsigned int j;
  for( j = 0; j < ImageDimension; j++ )
    {
    r[j] = 0;
    }
  this->SetRadius(r);

  m_TimeStep = 1.0;
  m_DenominatorThreshold = 1e-9;
  m_IntensityDifferenceThreshold = 0.001;
  this->SetMovingImage(NULL);
  this->SetFixedImage(NULL);
  m_FixedImageSpacing.Fill( 1.0 );
  m_FixedImageOrigin.Fill( 0.0 );
  m_Normalizer = 1.0;
  m_FixedImageGradientCalculator = GradientCalculatorType::New();


  typename DefaultInterpolatorType::Pointer interp =
    DefaultInterpolatorType::New();

  m_MovingImageInterpolator = static_cast<InterpolatorType*>(
    interp.GetPointer() );

  m_Metric = NumericTraits<double>::max();
  m_SumOfSquaredDifference = 0.0;
  m_NumberOfPixelsProcessed = 0L;
  m_RMSChange = NumericTraits<double>::max();
  m_SumOfSquaredChange = 0.0;

  m_MovingImageGradientCalculator = MovingImageGradientCalculatorType::New();
  m_UseMovingImageGradient = false;

}


/*
 * Standard "PrintSelf" method.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsDemonsRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "MovingImageIterpolator: ";
  os << m_MovingImageInterpolator.GetPointer() << std::endl;
  os << indent << "FixedImageGradientCalculator: ";
  os << m_FixedImageGradientCalculator.GetPointer() << std::endl;
  os << indent << "DenominatorThreshold: ";
  os << m_DenominatorThreshold << std::endl;
  os << indent << "IntensityDifferenceThreshold: ";
  os << m_IntensityDifferenceThreshold << std::endl;

  os << indent << "UseMovingImageGradient: ";
  os << m_UseMovingImageGradient << std::endl;

  os << indent << "Metric: ";
  os << m_Metric << std::endl;
  os << indent << "SumOfSquaredDifference: ";
  os << m_SumOfSquaredDifference << std::endl;
  os << indent << "NumberOfPixelsProcessed: ";
  os << m_NumberOfPixelsProcessed << std::endl;
  os << indent << "RMSChange: ";
  os << m_RMSChange << std::endl;
  os << indent << "SumOfSquaredChange: ";
  os << m_SumOfSquaredChange << std::endl;

}


/**
 *
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsDemonsRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::SetIntensityDifferenceThreshold(double threshold)
{
  m_IntensityDifferenceThreshold = threshold;
}

/**
 *
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
double
jwsDemonsRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::GetIntensityDifferenceThreshold() const
{
  return m_IntensityDifferenceThreshold;
}


/*
 * Set the function state values before each iteration
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsDemonsRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::InitializeIteration()
{
  if( !this->GetMovingImage() || !this->GetFixedImage() || !m_MovingImageInterpolator )
    {
    itkExceptionMacro( << "MovingImage, FixedImage and/or Interpolator not set" );
    }

  // cache fixed image information
  m_FixedImageSpacing    = this->GetFixedImage()->GetSpacing();
  m_FixedImageOrigin     = this->GetFixedImage()->GetOrigin();

  // compute the normalizer
  m_Normalizer      = 0.0;
  for( unsigned int k = 0; k < ImageDimension; k++ )
    {
    m_Normalizer += m_FixedImageSpacing[k] * m_FixedImageSpacing[k];
    }
  m_Normalizer /= static_cast<double>( ImageDimension );


  // setup gradient calculator
  m_FixedImageGradientCalculator->SetInputImage( this->GetFixedImage() );
  m_MovingImageGradientCalculator->SetInputImage( this->GetMovingImage() );

  // setup moving image interpolator
  m_MovingImageInterpolator->SetInputImage( this->GetMovingImage() );

  // initialize metric computation variables
  m_SumOfSquaredDifference  = 0.0;
  m_NumberOfPixelsProcessed = 0L;
  m_SumOfSquaredChange      = 0.0;

}


/*
 * Compute update at a specify neighbourhood
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
typename jwsDemonsRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::PixelType
jwsDemonsRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::ComputeUpdate(const NeighborhoodType &it, void * gd,
                const FloatOffsetType& itkNotUsed(offset))
{

 //fprintf(stderr,"In jwsDemonsReg Method Update\n");

  PixelType update;
  unsigned int j;

  IndexType index = it.GetIndex();

  // Get fixed image related information
  double fixedValue;
  CovariantVectorType gradient;
  double gradientSquaredMagnitude = 0;

  // Note: no need to check the index is within
  // fixed image buffer. This is done by the external filter.
  fixedValue = (double) this->GetFixedImage()->GetPixel( index );

  // Get moving image related information
  double movingValue;
  PointType mappedPoint;

  for( j = 0; j < ImageDimension; j++ )
    {
    mappedPoint[j] = double( index[j] ) * m_FixedImageSpacing[j] + 
      m_FixedImageOrigin[j];
    mappedPoint[j] += it.GetCenterPixel()[j];
    }
  if( m_MovingImageInterpolator->IsInsideBuffer( mappedPoint ) )
    {
    movingValue = m_MovingImageInterpolator->Evaluate( mappedPoint );
    }
  else
    {
    for( j = 0; j < ImageDimension; j++ )
      {
      update[j] = 0.0;
      }
    return update;
    }

  // Compute the gradient of either fixed or moving image
  if( !m_UseMovingImageGradient )
    {
    gradient = m_FixedImageGradientCalculator->EvaluateAtIndex( index );
    }
  else
    {
    gradient = m_MovingImageGradientCalculator->Evaluate( mappedPoint );
    }

  for( j = 0; j < ImageDimension; j++ )
    {
    gradientSquaredMagnitude += vnl_math_sqr( gradient[j] );
    } 


  /**
   * Compute Update.
   * In the original equation the denominator is defined as (g-f)^2 + grad_mag^2.
   * However there is a mismatch in units between the two terms. 
   * The units for the second term is intensity^2/mm^2 while the
   * units for the first term is intensity^2. This mismatch is particularly
   * problematic when the fixed image does not have unit spacing.
   * In this implemenation, we normalize the first term by a factor K,
   * such that denominator = (g-f)^2/K + grad_mag^2
   * where K = mean square spacing to compensate for the mismatch in units.
   */
  double speedValue = fixedValue - movingValue;
  
  // update the metric
  GlobalDataStruct *globalData = (GlobalDataStruct *)gd;
  if ( globalData )
    {
    globalData->m_SumOfSquaredDifference += vnl_math_sqr( speedValue );
    globalData->m_NumberOfPixelsProcessed += 1;
    }

  double denominator = vnl_math_sqr( speedValue ) / m_Normalizer + 
    gradientSquaredMagnitude;

  if ( vnl_math_abs(speedValue) < m_IntensityDifferenceThreshold || 
       denominator < m_DenominatorThreshold )
    {
    for( j = 0; j < ImageDimension; j++ )
      {
      update[j] = 0.0;
      }
    return update;
    }

  for( j = 0; j < ImageDimension; j++ )
    {
    update[j] = speedValue * gradient[j] / denominator;
    if ( globalData )
      {
      globalData->m_SumOfSquaredChange += vnl_math_sqr( update[j] );
      }
    }

  return update;

}

/*
 * Update the metric and release the per-thread-global data.
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsDemonsRegistrationFunction<TFixedImage,TMovingImage,TDeformationField>
::ReleaseGlobalDataPointer( void *gd ) const
{
  GlobalDataStruct * globalData = (GlobalDataStruct *) gd;

  m_MetricCalculationLock.Lock();
  m_SumOfSquaredDifference  += globalData->m_SumOfSquaredDifference;
  m_NumberOfPixelsProcessed += globalData->m_NumberOfPixelsProcessed;
  m_SumOfSquaredChange += globalData->m_SumOfSquaredChange;
  if ( m_NumberOfPixelsProcessed )
    {
    m_Metric = m_SumOfSquaredDifference / 
               static_cast<double>( m_NumberOfPixelsProcessed ); 
    m_RMSChange = vcl_sqrt( m_SumOfSquaredChange / 
               static_cast<double>( m_NumberOfPixelsProcessed ) ); 
    }
  m_MetricCalculationLock.Unlock();

  delete globalData;
}



} // end namespace itk

#endif

