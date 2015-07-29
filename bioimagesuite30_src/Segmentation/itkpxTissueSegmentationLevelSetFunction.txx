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
  Module:    $RCSfile: itkTissueSegmentationLevelSetFunction.txx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:39 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTissueSegmentationLevelSetFunction_txx_
#define __itkTissueSegmentationLevelSetFunction_txx_

#include "itkpxTissueSegmentationLevelSetFunction.h"
#include "vtkpxLevelSetUtil.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkImageRegionIterator.h"

namespace itk {

template <class TImageType, class TFeatureImageType>
void TissueSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::CalculateSpeedImage()
{
  //  ImageRegionIterator<FeatureImageType> lit;
  ImageRegionConstIterator<FeatureImageType>  fit(this->GetFeatureImage(), this->GetFeatureImage()->GetRequestedRegion());
  ImageRegionIterator<ImageType>              sit(this->GetSpeedImage(), this->GetFeatureImage()->GetRequestedRegion());

  for ( fit.GoToBegin(), sit.GoToBegin(); ! fit.IsAtEnd(); ++sit, ++fit)
    sit.Set( static_cast<ScalarValueType>(fit.Get()));
}


template <class TImageType, class TFeatureImageType>
void TissueSegmentationLevelSetFunction<TImageType, TFeatureImageType>
::CalculateAdvectionImage()
{
  /* compoute the gradient of the feature image. */
  typedef GradientRecursiveGaussianImageFilter<FeatureImageType,VectorImageType>
    DerivativeFilterType;

  typename DerivativeFilterType::Pointer derivative = DerivativeFilterType::New();
  derivative->SetInput( this->GetFeatureImage() );
  derivative->SetSigma( m_DerivativeSigma );
  derivative->Update();
  
  /* copy negative gradient into the advection image. */
  ImageRegionIterator<VectorImageType>
    dit( derivative->GetOutput(), this->GetFeatureImage()->GetRequestedRegion() );
  ImageRegionIterator<VectorImageType>
    ait( this->GetAdvectionImage(), this->GetFeatureImage()->GetRequestedRegion() );

  for( dit.GoToBegin(), ait.GoToBegin(); !dit.IsAtEnd(); ++dit, ++ait )
    {
      typename VectorImageType::PixelType v = dit.Get();
      for ( unsigned int j = 0; j < ImageDimension; j++ )
	  v[j] *= -1.0L;

      ait.Set( v);
    }
}


template <class TImageType, class TFeatureImageType>
typename TissueSegmentationLevelSetFunction<TImageType, TFeatureImageType>::PixelType
TissueSegmentationLevelSetFunction<TImageType, TFeatureImageType>::ComputeUpdate(
										 const NeighborhoodType &it,
										 void *globalData,
										 const FloatOffsetType& offset )
{
  static int count=0;
  ++count;
  unsigned int i, j;  
  const ScalarValueType ZERO = NumericTraits<ScalarValueType>::Zero;
  const ScalarValueType center_value  = it.GetCenterPixel();
  
  


  ScalarValueType laplacian, x_energy, laplacian_term, propagation_term,
    curvature_term, advection_term, propagation_gradient;
  VectorType advection_field;

  ScalarValueType mean1  = static_cast<ScalarValueType>(m_InsideMean);
  ScalarValueType sigma1 = static_cast<ScalarValueType>(m_InsideSigma);


  //  ScalarValueType mean2  = static_cast<ScalarValueType>(m_OutsideMean);
  //  ScalarValueType sigma2 = static_cast<ScalarValueType>(m_OutsideSigma);


  // Global data structure
  GlobalDataStruct *gd = (GlobalDataStruct *)globalData;


  // ----------------------------------------------------------------------------
  // 
  // Step 0 Compute Derivatives etc
  //
  // ---------------------------------------------------------------------------

  gd->m_GradMagSqr = 1.0e-6;
  for( i = 0 ; i < ImageDimension; i++)
    {
      const unsigned int positionA = 
	static_cast<unsigned int>( this->m_Center + this->m_xStride[i]);    
      const unsigned int positionB = 
	static_cast<unsigned int>( this->m_Center - this->m_xStride[i]);    
      
      gd->m_dx[i] = 0.5 * (it.GetPixel( positionA ) - 
			   it.GetPixel( positionB )    );
      
      gd->m_dxy[i][i] = it.GetPixel( positionA )
	+ it.GetPixel( positionB ) - 2.0 * center_value;
      
      gd->m_dx_forward[i]  = it.GetPixel( positionA ) - center_value;
      gd->m_dx_backward[i] = center_value - it.GetPixel( positionB );
      gd->m_GradMagSqr += gd->m_dx[i] * gd->m_dx[i];
      
      for( j = i+1; j < ImageDimension; j++ )
	{
	  const unsigned int positionAa = static_cast<unsigned int>( 
								    this->m_Center - this->m_xStride[i] - this->m_xStride[j] );
	  const unsigned int positionBa = static_cast<unsigned int>( 
								    this->m_Center - this->m_xStride[i] + this->m_xStride[j] );
	  const unsigned int positionCa = static_cast<unsigned int>( 
								    this->m_Center + this->m_xStride[i] - this->m_xStride[j] );
	  const unsigned int positionDa = static_cast<unsigned int>( 
								    this->m_Center + this->m_xStride[i] + this->m_xStride[j] );
	  
	  gd->m_dxy[i][j] = gd->m_dxy[j][i] = 0.25 *( it.GetPixel( positionAa )
						      - it.GetPixel( positionBa )
						      - it.GetPixel( positionCa )
						      + it.GetPixel( positionDa )
						      );
	}
    }
  

  // ----------------------------------------------------------------------------
  // 
  // Step 1 Compute Curvature Term 
  //
  // ---------------------------------------------------------------------------


  if ( this->m_CurvatureWeight != ZERO )
    {
      curvature_term = this->ComputeCurvatureTerm(it, offset, gd) * this->m_CurvatureWeight * this->CurvatureSpeed(it, offset);
      gd->m_MaxCurvatureChange = vnl_math_max(gd->m_MaxCurvatureChange, vnl_math_abs(curvature_term));
    }
  else
    {
      curvature_term = ZERO;
    }
  

  // ----------------------------------------------------------------------------
  // 
  // Step 2 Compute Advection Term 
  //
  // ---------------------------------------------------------------------------

  if (this->m_AdvectionWeight != ZERO)
    {
      advection_field = this->AdvectionField(it, offset, gd);
      advection_term = ZERO;
      for(i = 0; i < ImageDimension; i++)
	{
	  x_energy = this->m_AdvectionWeight * advection_field[i];
	  if (x_energy > ZERO)
	    {
	      advection_term += advection_field[i] * gd->m_dx_backward[i];
	    }
	  else
	    {
	      advection_term += advection_field[i] * gd->m_dx_forward[i];
	    }
	  
	  gd->m_MaxAdvectionChange = vnl_math_max(gd->m_MaxAdvectionChange, vnl_math_abs(x_energy)); 
	}
      advection_term *= this->m_AdvectionWeight;
    }
  else
    {
      advection_term = ZERO;
    }
  

  // ----------------------------------------------------------------------------
  // 
  // Step 3 Compute Propagation Term 
  //
  // ---------------------------------------------------------------------------

  typename ImageType::IndexType idx=it.GetIndex();


  if (this->m_PropagationWeight > 0 ) 
    {
      
      if (this->m_UseTissueSpecificSpeedMode==0)
	{
	  //	  std::cerr << "Tissue SPECIFIC MODE is ZERO .................... \n";
	  propagation_term = this->m_PropagationWeight * this->PropagationSpeed(it, offset, gd);
	}
      else
	{
	  ScalarValueType intensity=this->PropagationSpeed(it, offset, gd);
	  
	  ScalarValueType t1=this->InsideTissueFunction(intensity);
	  ScalarValueType t2=this->OutsideTissueFunction(intensity);
	  if (this->m_BiExponentialOutside==1)
	    {
	      ScalarValueType t3=this->Outside2TissueFunction(intensity);
	      if (t3<t2)
		t2=t3;
	    }
	  
	  ScalarValueType sp=t2-t1;
	  
	  if (this->m_UseTissueSpecificSpeedMode==2)
	    {
	      if (intensity < (mean1-3.0*sigma1 ) || intensity < (mean1+5.5*sigma1)  )
		sp=0.0;
	    }
	  propagation_term = this->m_PropagationWeight * sp;
	}
    
  
      if (    m_ApproximateVolumeConstraintWeight > 0.0)
	{
	  //	  std::cerr << "Tissue ON to Approxumate Volume Constraint .................... \n";
	  //2*lamda*vc
	  double addp=m_ApproximateVolumeConstraintWeight*(m_ApproximateVolume-m_MyVolume)/(m_ApproximateVolume);
	  propagation_term = propagation_term + addp;

	  /*	  if (count==1000)
	    {
	      std::cerr << "Volume   =" << m_MyVolume     << ", target_volume = " << m_ApproximateVolume << ", (add = " << addp  << ")" << std::endl; 
	      }*/
	}
      
      
      //
      // Construct upwind gradient values for use in the propagation speed term:
      //  $\beta G(\mathbf{x})\mid\nabla\phi\mid$
      //
      // The following scheme for ``upwinding'' in the normal direction is taken
      // from Sethian, Ch. 6 as referenced above.
      //
      propagation_gradient = ZERO;
      
      if (this->m_UseNonMorphometricTerm == 0 ) 
	{
	  if ( propagation_term > ZERO )
	    {
	      for(i = 0; i< ImageDimension; i++)
		{
		  propagation_gradient += vnl_math_sqr( vnl_math_max(gd->m_dx_backward[i], ZERO) )
		    + vnl_math_sqr( vnl_math_min(gd->m_dx_forward[i],  ZERO) );
		}
	    }
	  else
	    {
	      for(i = 0; i< ImageDimension; i++)
		{
		  propagation_gradient += vnl_math_sqr( vnl_math_min(gd->m_dx_backward[i], ZERO) )
		    + vnl_math_sqr( vnl_math_max(gd->m_dx_forward[i],  ZERO) );
		}        
	    }
	}
      else
	{
	  propagation_gradient=m_HeavySideEpsilon/(3.1415927* (m_HeavySideEpsilon*m_HeavySideEpsilon+center_value*center_value));
	}
      
      // Collect energy change from propagation term.  This will be used in
      // calculating the maximum time step that can be taken for this iteration.
      gd->m_MaxPropagationChange =	vnl_math_max(gd->m_MaxPropagationChange,  vnl_math_abs(propagation_term));
      
      propagation_term *= vcl_sqrt( propagation_gradient );
      
      //      propagation_term = ZERO;
    }
  /*  else
    {
      std::cerr << "Propagation Weight is ZERO .................... \n";
      }*/

  // ----------------------------------------------------------------------------
  // 
  // Step 4  Compute Laplacian Term 
  //
  // ---------------------------------------------------------------------------
  
  if(this->m_LaplacianSmoothingWeight != ZERO)
    {
      laplacian = ZERO;
        for(i = 0;i < ImageDimension; i++)
	  laplacian += gd->m_dxy[i][i];
        laplacian_term = laplacian * this->m_LaplacianSmoothingWeight * this->LaplacianSmoothingSpeed(it,offset, gd);
    }
  else 
    laplacian_term = ZERO;
  
  // ----------------------------------------------------------------------------
  // 
  // Step 5  Add it Up
  //
  // ---------------------------------------------------------------------------

  /*  if (count==1000)
    {
      std::cerr << "\n Pixel("<<idx[0]<<","<<idx[1]<<"," << idx[2]<< ")\n";
      std::cerr << "Curvature   =" << curvature_term     << "(" << this->m_CurvatureWeight          << ")"   << std::endl;
      std::cerr << "Propagation =" << propagation_term   << "(" << this->m_PropagationWeight        <<")"    << std::endl;
      std::cerr << "Advection   =" << advection_term     << "(" << this->m_AdvectionWeight          << ")"   << std::endl;
      std::cerr << "Laplacian   =" << laplacian_term     << "(" << this->m_LaplacianSmoothingWeight << ")" << std::endl;



      PixelType a=( curvature_term - propagation_term 
                         - advection_term - laplacian_term );
      std::cerr << "Overall     =" << a << "\n\n";
      count=0;
      }*/
  
  // Return the combination of all the terms.
  return ( PixelType ) ( curvature_term - propagation_term - advection_term - laplacian_term );
}




} // end namespace itk


#endif


