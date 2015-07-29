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
  Module:    $RCSfile: itkjwsSymmetricForcesDemonsRegistrationFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/11/04 20:40:37 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkjwsSymmetricForcesDemonsRegistrationFilter_txx
#define _itkjwsSymmetricForcesDemonsRegistrationFilter_txx
#include "itkjwsSymmetricForcesDemonsRegistrationFilter.h"

namespace itk {

/*
 * Default constructor
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
jwsSymmetricForcesDemonsRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::jwsSymmetricForcesDemonsRegistrationFilter()
{
 
  typename jwsSymmetricForcesDemonsRegistrationFunctionType::Pointer drfp;
  drfp = jwsSymmetricForcesDemonsRegistrationFunctionType::New();

  this->SetDifferenceFunction( static_cast<FiniteDifferenceFunctionType *>(
                                 drfp.GetPointer() ) );

}


/*
 * Set the function state values before each iteration
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsSymmetricForcesDemonsRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::InitializeIteration()
{


  // update variables in the equation object
  jwsSymmetricForcesDemonsRegistrationFunctionType *f = 
    dynamic_cast<jwsSymmetricForcesDemonsRegistrationFunctionType *>
    (this->GetDifferenceFunction().GetPointer());

  if ( !f )
    {
    itkExceptionMacro(<<"FiniteDifferenceFunction not of type jwsSymmetricForcesDemonsRegistrationFunctionType");
    }

  f->SetDeformationField( this->GetDeformationField() );


 
  // call the superclass  implementation
  Superclass::InitializeIteration();

  /*
   * Smooth the deformation field
   */
  if ( this->GetSmoothDeformationField() )
    {
    this->SmoothDeformationField();
    }

}


/*
 * Get the metric value from the difference function
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
double
jwsSymmetricForcesDemonsRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GetMetric() const
{
 
  jwsSymmetricForcesDemonsRegistrationFunctionType *drfp = 
    dynamic_cast<jwsSymmetricForcesDemonsRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());
 
  if( !drfp )
   {
   itkExceptionMacro( << 
     "Could not cast difference function to jwsSymmetricForcesDemonsRegistrationFunction" );
   }
   
  return drfp->GetMetric();

}


/////////////////////////////////////////////////////////////////////////////////
/*
 * Added by wook 05/16/08
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsSymmetricForcesDemonsRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::setMyDField( DeformationFieldPointer ptr )
{
   // set the gradient selection flag
  jwsSymmetricForcesDemonsRegistrationFunctionType *drfp = 
    dynamic_cast<jwsSymmetricForcesDemonsRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());
 
  if( !drfp )
   {
   itkExceptionMacro( << 
     "Could not cast difference function to jwsDemonsRegistrationFunction" );
   }
   
  drfp->SetDeformationField( ptr );

}

/*
 * 
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
double
jwsSymmetricForcesDemonsRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GetIntensityDifferenceThreshold() const
{
 
  jwsSymmetricForcesDemonsRegistrationFunctionType *drfp = 
    dynamic_cast<jwsSymmetricForcesDemonsRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());
 
  if( !drfp )
   {
   itkExceptionMacro( << 
     "Could not cast difference function to DemonsRegistrationFunction" );
   }
   
  return drfp->GetIntensityDifferenceThreshold();

}

/*
 * 
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsSymmetricForcesDemonsRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::SetIntensityDifferenceThreshold(double threshold) 
{
 
  jwsSymmetricForcesDemonsRegistrationFunctionType *drfp = 
    dynamic_cast<jwsSymmetricForcesDemonsRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());
 
  if( !drfp )
   {
   itkExceptionMacro( << 
     "Could not cast difference function to SymmetricDemonsRegistrationFunction" );
   }
   
  drfp->SetIntensityDifferenceThreshold(threshold);

}

/*
 * Get the metric value from the difference function
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
const double &
jwsSymmetricForcesDemonsRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::GetRMSChange() const
{
 
  jwsSymmetricForcesDemonsRegistrationFunctionType *drfp = 
    dynamic_cast<jwsSymmetricForcesDemonsRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());
 
  if( !drfp )
   {
   itkExceptionMacro( << 
     "Could not cast difference function to jwsSymmetricForcesDemonsRegistrationFunction" );
   }
   
  return drfp->GetRMSChange();

}


/*
 * Get the metric value from the difference function
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsSymmetricForcesDemonsRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::ApplyUpdate(TimeStepType dt)
{
  // If we smooth the update buffer before applying it, then the are
  // approximating a viscuous problem as opposed to an elastic problem
  if ( this->GetSmoothUpdateField() )
    {
    this->SmoothUpdateField();
    }
  
  this->Superclass::ApplyUpdate(dt);

  jwsSymmetricForcesDemonsRegistrationFunctionType *drfp = 
    dynamic_cast<jwsSymmetricForcesDemonsRegistrationFunctionType *>
      (this->GetDifferenceFunction().GetPointer());
 
  if( !drfp )
   {
   itkExceptionMacro( << 
     "Could not cast difference function to DemonsRegistrationFunction" );
   }

  this->SetRMSChange( drfp->GetRMSChange() );
   
}

template <class TFixedImage, class TMovingImage, class TDeformationField>
void
jwsSymmetricForcesDemonsRegistrationFilter<TFixedImage,TMovingImage,TDeformationField>
::PrintSelf(std::ostream& os, Indent indent) const
{ 
  Superclass::PrintSelf( os, indent );
  os << indent << "Intensity difference threshold: " <<
    this->GetIntensityDifferenceThreshold() << std::endl;
}


} // end namespace itk

#endif


