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
  Module:    $RCSfile: itkTissueSegmentationCoupledLevelSetFunction.txx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:39 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTissueSegmentationCoupledLevelSetFunction_txx_
#define __itkTissueSegmentationCoupledLevelSetFunction_txx_

#include "itkpxTissueSegmentationCoupledLevelSetFunction.h"
#include "vtkpxLevelSetUtil.h"
#include "itkGradientRecursiveGaussianImageFilter.h"
#include "itkImageRegionIterator.h"

namespace itk {

template <class TImageType, class TFeatureImageType>
void TissueSegmentationCoupledLevelSetFunction<TImageType, TFeatureImageType>
::CalculateSpeedImage()
{
  //  ImageRegionIterator<FeatureImageType> lit;
  ImageRegionConstIterator<FeatureImageType>  fit(this->GetFeatureImage(), this->GetFeatureImage()->GetRequestedRegion());
  ImageRegionIterator<ImageType>              sit(this->GetSpeedImage(), this->GetFeatureImage()->GetRequestedRegion());

  for ( fit.GoToBegin(), sit.GoToBegin(); ! fit.IsAtEnd(); ++sit, ++fit)
    sit.Set( static_cast<ScalarValueType>(fit.Get()));
}


template <class TImageType, class TFeatureImageType>
void TissueSegmentationCoupledLevelSetFunction<TImageType, TFeatureImageType>
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
typename TissueSegmentationCoupledLevelSetFunction<TImageType, TFeatureImageType>::PixelType
TissueSegmentationCoupledLevelSetFunction<TImageType, TFeatureImageType>::ComputeUpdate(
										 const NeighborhoodType &it,
										 void *globalData,
										 const FloatOffsetType& offset )
{
  unsigned int i, j;  
  const ScalarValueType ZERO = NumericTraits<ScalarValueType>::Zero;
  const ScalarValueType center_value  = it.GetCenterPixel();
  
  


  ScalarValueType laplacian, x_energy, laplacian_term, propagation_term,
    curvature_term, advection_term, propagation_gradient;
  VectorType advection_field;

  ScalarValueType mean1  = static_cast<ScalarValueType>(m_WhiteMatterMean);
  ScalarValueType sigma1 = static_cast<ScalarValueType>(m_WhiteMatterSigma);


  //  ScalarValueType mean2  = static_cast<ScalarValueType>(m_GrayMatterMean);
  //  ScalarValueType sigma2 = static_cast<ScalarValueType>(m_GrayMatterSigma);


  // Global data structure
  GlobalDataStruct *gd = (GlobalDataStruct *)globalData;

  // ----------------------------------------------------------------------------
  // 
  // Step -1: Where are we?
  //
  // ---------------------------------------------------------------------------

  typename ImageType::IndexType idx=it.GetIndex();

  
  int BoundaryMode=0; // 0 = None , 1 = White/Gray ... 2= Gray/CS
  if (idx[0] < m_InnerBoundary)
    BoundaryMode=1;
  else if (idx[0] >= m_OuterBoundary)
    BoundaryMode=2;
  
  gd->m_GradMagSqr = 1.0e-6;


  //  std::cerr << m_InnerBoundary << " m_outer= " << m_OuterBoundary << "\n";


  // ----------------------------------------------------------------------------
  // 
  // Step 0 Compute Derivatives etc
  //
  // ---------------------------------------------------------------------------

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


  curvature_term = ZERO;
  if ( this->m_CurvatureWeight != ZERO && BoundaryMode!=0 )
    {
      curvature_term = this->ComputeCurvatureTerm(it, offset, gd) * this->m_CurvatureWeight * this->CurvatureSpeed(it, offset);
      gd->m_MaxCurvatureChange = vnl_math_max(gd->m_MaxCurvatureChange, vnl_math_abs(curvature_term));
    }

  

  // ----------------------------------------------------------------------------
  // 
  // Step 2 Compute Advection Term 
  //
  // ---------------------------------------------------------------------------

  advection_term = ZERO;
  if (this->m_AdvectionWeight != ZERO && BoundaryMode!= 0 )
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
  

  // ----------------------------------------------------------------------------
  // 
  // Step 3 Compute Propagation Term 
  //
  // ---------------------------------------------------------------------------



	  /*	  typename NeighborhoodIterator<FeatureImageType>::IndexType loc;
	    loc[0]=layerIt->m_Value[0]/2;
	    loc[1]=layerIt->m_Value[1];
	    loc[2]=layerIt->m_Value[2];
	    outputIt.SetLocation(loc);
	    outputIt.SetCenterPixel(phi);*/


  propagation_term = ZERO;

  if (BoundaryMode!=0)
    {
      if (this->m_PropagationWeight > 0 ) 
	{
	  ScalarValueType intensity=this->PropagationSpeed(it, offset, gd);
	  ScalarValueType t1,t2;
	  if (BoundaryMode==1)
	    {
	      t1=this->WhiteMatterTissueFunction(intensity);
	      t2=this->GrayMatterTissueFunction(intensity);
	    } 
	  else
	    {
	      t1=this->GrayMatterTissueFunction(intensity);	      
	      t2=this->CSFTissueFunction(intensity);
	    }
	  
	  ScalarValueType sp=t2-t1;
	  
	  // if (intensity < (mean1-3.0*sigma1 ) || intensity < (mean1+5.5*sigma1)  )
	  // sp=0.0;

	  propagation_term = this->m_PropagationWeight * sp;
	  //	  std::cerr << "\nPoint " << idx[0] << "," << idx[1] << "," << idx[2] << "\t";
	  //	  std::cerr << "Intensity = " << intensity << ", t1= " << t1 << " t2 = " << t2 << " mode= " << BoundaryMode << "\n";

	}
      
      
      if (this->m_ConstraintOffset > 0.0)
	{
	  // Now get other level set and constrain to it 

	  unsigned int positionOffset = 0;

	  typename ImageType::IndexType idx2;
	  // All coordinates other than x are fixed!
	  for ( unsigned int j = 1; j < ImageDimension; j++ )
	    idx2[j]=idx[j];

	  float lower_bound,upper_bound;
	  if (BoundaryMode==1)
	    {
	      // Look over to the right 
	      idx2[0]=idx[0]+m_OuterBoundary;
	      lower_bound=m_LowerWhiteGrayBound;
	      upper_bound=m_UpperWhiteGrayBound;
	    }
	  else 
	    {
	      // Look to the left
	      idx2[0]=idx[0]-m_OuterBoundary;
	      lower_bound=m_LowerGrayCSFBound;
	      upper_bound=m_UpperGrayCSFBound;
	    }

	  

	  ScalarValueType phi_other=it.GetImagePointer()->GetPixel(idx2);
	  ScalarValueType phi_diff=(center_value-phi_other);


	  ScalarValueType factor=1.0;


	  if (phi_diff < lower_bound )
	    factor=1.0-fabs(phi_diff-lower_bound)/m_ConstraintOffset;
	  else if (phi_diff > upper_bound)
	    factor=1.0-fabs(phi_diff-upper_bound)/m_ConstraintOffset;
	  
	  if (factor<0.0)
	    factor=0.0;
	  
	  propagation_term=propagation_term*factor;
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
    }



  // ----------------------------------------------------------------------------
  // 
  // Step 4  Compute Laplacian Term 
  //
  // ---------------------------------------------------------------------------

  laplacian_term = ZERO;  
  if(this->m_LaplacianSmoothingWeight != ZERO && BoundaryMode!=0)
    {
      laplacian = ZERO;
      for(i = 0;i < ImageDimension; i++)
	  laplacian += gd->m_dxy[i][i];
        laplacian_term = laplacian * this->m_LaplacianSmoothingWeight * this->LaplacianSmoothingSpeed(it,offset, gd);
    }

  
  // ----------------------------------------------------------------------------
  // 
  // Step 5  Add it Up
  //
  // ---------------------------------------------------------------------------


  /*  std::cerr << "\n Pixel("<<idx[0]<<","<<idx[1]<<")\n";
  std::cerr << "Curvature   =" << curvature_term     << "(" << m_CurvatureWeight          << ")"   << std::endl;
  std::cerr << "Propagation =" << propagation_term   << "(" << m_PropagationWeight        <<")"    << std::endl;
  std::cerr << "Advection   =" << advection_term     << "(" << m_AdvectionWeight          << ")"   << std::endl;
  std::cerr << "Laplacian   =" << laplacian_term     << "(" << m_LaplacianSmoothingWeight << ")" << std::endl;
  PixelType a=( curvature_term - propagation_term 
		- advection_term - laplacian_term );
		std::cerr << "Overall     =" << a << "\n\n";*/
  
  // Return the combination of all the terms.
  return ( PixelType ) ( curvature_term - propagation_term - advection_term - laplacian_term );
}




} // end namespace itk


#endif


