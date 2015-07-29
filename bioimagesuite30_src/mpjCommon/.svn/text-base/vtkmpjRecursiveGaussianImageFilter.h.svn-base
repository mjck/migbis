

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


/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRecursiveGaussianImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2004/02/02 21:42:22 $
  Version:   $Revision: 1.24 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __vtkmpjRecursiveGaussianImageFilter_h
#define __vtkmpjRecursiveGaussianImageFilter_h

#include "vtkmpjRecursiveSeparableImageFilter.h"

/** \class RecursiveGaussianImageFilter
 * \brief Base class for computing IIR convolution with a Gaussian kernel.
 *
 *    \f[    
 *      \frac{ 1 }{ \sigma \sqrt{ 2 \pi } } \exp{ \left( - \frac{x^2}{ 2 \sigma^2 } \right) }
 *    \f]
 *
 * RecursiveGaussianImageFilter is the base class for recursive filters that
 * approximate convolution with the Gaussian kernel.
 * This class implements the recursive filtering
 * method proposed by R.Deriche in IEEE-PAMI
 * Vol.12, No.1, January 1990, pp 78-87.
 * 
 * As compared to itk::DiscreteGaussianImageFilter, this filter tends
 * to be faster for large kernels, and it can take the derivative
 * of the blurred image in one step.  Also, note that we have
 * itk::RecursiveGaussianImageFilter::SetSigma(), but
 * itk::DiscreteGaussianImageFilter::SetVariance().
 * 
 * \ingroup ImageEnhancement Singlethreaded
 */
class vtkmpjRecursiveGaussianImageFilter : public vtkmpjRecursiveSeparableImageFilter
{
 public:
  static vtkmpjRecursiveGaussianImageFilter *New();
  vtkTypeMacro(vtkmpjRecursiveGaussianImageFilter,vtkpxSimpleImageToImageFilter);
  
  /** Set/Get the Sigma, measured in world coordinates, of the Gaussian
   * kernel.  The default is 1.0.  */   
  vtkGetMacro( Sigma, float );
  vtkSetMacro( Sigma, float );

  /** Enum type that indicates if the filter applies the equivalent operation
      of convolving with a gaussian, first derivative of a gaussian or the 
      second derivative of a gaussian.  */
  //BTX
  typedef  enum { ZeroOrder = 0, FirstOrder = 1, SecondOrder = 2 } OrderEnumType;
  //ETX 

  /** Set/Get the flag for normalizing the gaussian over scale Space
      When this flag is ON the filter will be normalized in such a way 
      that larger sigmas will not result in the image fading away.
      
      \f[    
            \frac{ 1 }{ \sqrt{ 2 \pi } };
      \f]

      When the flag is OFF the normalization will conserve contant the 
      integral of the image intensity. 
      \f[    
            \frac{ 1 }{ \sigma  \sqrt{ 2 \pi } };
      \f]
      For analyzing an image across Scale Space you want to enable
      this flag.  It is disabled by default.  */
  vtkSetMacro( NormalizeAcrossScale, int );
  vtkGetMacro( NormalizeAcrossScale, int );
  vtkBooleanMacro( NormalizeAcrossScale, int );

  /** Set/Get the Order of the Gaussian to convolve with. 
      \li ZeroOrder is equivalent to convolving with a Gaussian.  This
      is the default.
      \li FirstOrder is equivalet to convolving with the first derivative of a Gaussian
      \li SecondOrder is equivalet to convolving with the second derivative of a Gaussian
  */
  vtkSetMacro( Order, int );
  vtkGetMacro( Order, int );

  /** Explicitly set a zeroth order derivative */
  void SetZeroOrder() { this->SetOrder( ZeroOrder ); }

  /** Explicitly set a first order derivative */
  void SetFirstOrder() { this->SetOrder( FirstOrder ); }

  /** Explicitly set a second order derivative */
  void SetSecondOrder() { this->SetOrder( SecondOrder ); }
  
 protected:

  vtkmpjRecursiveGaussianImageFilter();
  virtual ~vtkmpjRecursiveGaussianImageFilter() {};

  vtkmpjRecursiveGaussianImageFilter(const vtkmpjRecursiveGaussianImageFilter&) {};
  void operator=(const vtkmpjRecursiveGaussianImageFilter&) {};

  /** Set up the coefficients of the filter to approximate a specific kernel.
   * typically it can be used to approximate a gaussian or one of its
   * derivatives. Parameter is the spacing along the dimension to
   * filter. */
  virtual void SetUp(RealType spacing);
  
  /** Compute Recursive Filter Coefficients. This method prepares the values of
   * the coefficients used for filtering the image. The symmetric flag is
   * used to enforce that the filter will be symmetric or antisymmetric. For
   * example, the Gaussian kernel is symmetric, while its first derivative is
   * antisymmetric. The spacing parameter specifies the data spacing
   * along the dimension to be filtered. */
  void ComputeFilterCoefficients(bool symmetric, RealType spacing);
  
 private:

  /** Sigma of the gaussian kernel. */   
  float Sigma;
  
  /** Normalize the image across scale space */
  int NormalizeAcrossScale; 
  
  int Order;

};



#endif
