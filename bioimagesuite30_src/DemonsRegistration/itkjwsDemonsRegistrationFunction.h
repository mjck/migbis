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
  Module:    $RCSfile: itkjwsjwsDemonsRegistrationFunction.h,v $
  Language:  C++
  Date:      $Date: 2004/06/17 15:25:58 $
  Version:   $Revision: 1.27 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkjwsDemonsRegistrationFunction_h_
#define _itkjwsDemonsRegistrationFunction_h_

#include "itkjwsPDEDeformableRegistrationFunction.h"
#include "itkPoint.h"
#include "itkCovariantVector.h"
#include "itkInterpolateImageFunction.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkCentralDifferenceImageFunction.h"

namespace itk {

/**
* \class jwsDemonsRegistrationFunction
*
* This class encapsulate the PDE which drives the demons registration
* algorithm. It is used by DemonsRegistrationFilter to compute the
* output deformation field which will map a moving image onto a
* a fixed image.
*
* Non-integer moving image values are obtained by using
* interpolation. The default interpolator is of type
* LinearInterpolateImageFunction. The user may set other
* interpolators via method SetMovingImageInterpolator. Note that the input
* interpolator must derive from baseclass InterpolateImageFunction.
*
* This class is templated over the fixed image type, moving image type,
* and the deformation field type.
*
* \warning This filter assumes that the fixed image type, moving image type
* and deformation field type all have the same number of dimensions.
*
* \sa DemonsRegistrationFilter
* \ingroup FiniteDifferenceFunctions
*/
template<class TFixedImage, class TMovingImage, class TDeformationField>
class  jwsDemonsRegistrationFunction :
    public jwsPDEDeformableRegistrationFunction< TFixedImage,
                                              TMovingImage, 
TDeformationField>
{
public:
  /** Standard class typedefs. */
  typedef jwsDemonsRegistrationFunction    Self;
  typedef jwsPDEDeformableRegistrationFunction< TFixedImage,
                                             TMovingImage, TDeformationField >    Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro( jwsDemonsRegistrationFunction,
                jwsPDEDeformableRegistrationFunction );

  /** MovingImage image type. */
  typedef typename Superclass::MovingImageType     MovingImageType;
  typedef typename Superclass::MovingImagePointer  MovingImagePointer;

  /** FixedImage image type. */
  typedef typename Superclass::FixedImageType     FixedImageType;
  typedef typename Superclass::FixedImagePointer  FixedImagePointer;
  typedef typename FixedImageType::IndexType      IndexType;
  typedef typename FixedImageType::SizeType       SizeType;
  typedef typename FixedImageType::SpacingType    SpacingType;

  /** Deformation field type. */
  typedef typename Superclass::DeformationFieldType    DeformationFieldType;
  typedef typename Superclass::DeformationFieldTypePointer
  DeformationFieldTypePointer;

  /** Inherit some enums from the superclass. */
  itkStaticConstMacro(ImageDimension, unsigned 
int,Superclass::ImageDimension);

  /** Inherit some enums from the superclass. */
  typedef typename Superclass::PixelType     PixelType;
  typedef typename Superclass::RadiusType    RadiusType;
  typedef typename Superclass::NeighborhoodType    NeighborhoodType;
  typedef typename Superclass::FloatOffsetType  FloatOffsetType;
  typedef typename Superclass::TimeStepType TimeStepType;

  /** Interpolator type. */
  typedef double CoordRepType;
  typedef InterpolateImageFunction<MovingImageType,CoordRepType> 
InterpolatorType;
  typedef typename InterpolatorType::Pointer         InterpolatorPointer;
  typedef typename InterpolatorType::PointType       PointType;
  typedef LinearInterpolateImageFunction<MovingImageType,CoordRepType>
  DefaultInterpolatorType;

  /** Covariant vector type. */
  typedef CovariantVector<double,itkGetStaticConstMacro(ImageDimension)> 
CovariantVectorType;

  /** Fixed image gradient calculator type. */
  typedef CentralDifferenceImageFunction<FixedImageType> 
GradientCalculatorType;
  typedef typename GradientCalculatorType::Pointer   
GradientCalculatorPointer;

 /** Moving image gradient calculator type. */
  typedef CentralDifferenceImageFunction<MovingImageType,CoordRepType>
      MovingImageGradientCalculatorType;
  typedef typename MovingImageGradientCalculatorType::Pointer
      MovingImageGradientCalculatorPointer;

  /** Set the moving image interpolator. */
  void SetMovingImageInterpolator( InterpolatorType * ptr )
  { m_MovingImageInterpolator = ptr; }

  /** Get the moving image interpolator. */
  InterpolatorType * GetMovingImageInterpolator(void)
  { return m_MovingImageInterpolator; }

  /** This class uses a constant timestep of 1. */
  virtual TimeStepType ComputeGlobalTimeStep(void * itkNotUsed(GlobalData)) 
const
  { return m_TimeStep; }

  /** Return a pointer to a global data structure that is passed to
   * this object from the solver at each calculation.  */
  virtual void *GetGlobalDataPointer() const
  {
    GlobalDataStruct *global = new GlobalDataStruct();
    global->m_SumOfSquaredDifference  = 0.0;
    global->m_NumberOfPixelsProcessed = 0L;
    global->m_SumOfSquaredChange      = 0;
    return global;
  }

  /** Release memory for global data structure. */
  virtual void ReleaseGlobalDataPointer( void *GlobalData ) const;

  /** Set the object's state before each iteration. */
  virtual void InitializeIteration();

  /** This method is called by a finite difference solver image filter at
   * each pixel that does not lie on a data set boundary */
  virtual PixelType  ComputeUpdate(const NeighborhoodType &neighborhood,
                                   void *globalData,
                                   const FloatOffsetType &offset = 
FloatOffsetType(0.0));

  /** Get the metric value. The metric value is the mean square difference 
   * in intensity between the fixed image and transforming moving image 
   * computed over the the overlapping region between the two images. */
  virtual double GetMetric() const
    { return m_Metric; }

  /** Get the rms change in deformation field. */
  virtual double GetRMSChange() const
    { return m_RMSChange; }





///////////////////// Added by Xenios  02/15/08  ///////////////////////
/** Set the relative weight of the second distance map to the first */
  virtual void SetSecondDistanceMapWeight( float w )
    { m_SecondDistanceMapWeight = w; }
  virtual float GetSecondDistanceMapWeight( float w )
    { return m_SecondDistanceMapWeight; }
/////////////////////////////////////////////////////////////////////////



  /** Select if the fixed image or moving image gradient is used for
   * the computating the demon forces. The fixed image gradient is used
   * by default. */
    virtual void SetUseMovingImageGradient( bool flag )
    { m_UseMovingImageGradient = flag; }
  virtual bool GetUseMovingImageGradient() const
    { return m_UseMovingImageGradient; }

  /** Set/Get the threshold below which the absolute difference of
   * intensity yields a match. When the intensities match between a
   * moving and fixed image pixel, the update vector (for that
   * iteration) will be the zero vector. Default is 0.001. */
  virtual void SetIntensityDifferenceThreshold(double);
  virtual double GetIntensityDifferenceThreshold() const;
  
protected:
  jwsDemonsRegistrationFunction();
  ~jwsDemonsRegistrationFunction() {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** FixedImage image neighborhood iterator type. */
  typedef ConstNeighborhoodIterator<FixedImageType> 
FixedImageNeighborhoodIteratorType;

  /** A global data type for this class of equation. Used to store
   * information for computing the metric. */
  struct GlobalDataStruct
  {
    double          m_SumOfSquaredDifference;
    unsigned long   m_NumberOfPixelsProcessed;
    double          m_SumOfSquaredChange;
  };

private:
  jwsDemonsRegistrationFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** Cache fixed image information. */
  SpacingType                     m_FixedImageSpacing;
  PointType                       m_FixedImageOrigin;
  double                          m_Normalizer;

  /** Function to compute derivatives of the fixed image. */
  GradientCalculatorPointer       m_FixedImageGradientCalculator;

  /** Function to compute derivatives of the moving image. */
  MovingImageGradientCalculatorPointer       m_MovingImageGradientCalculator;
  bool                                       m_UseMovingImageGradient;

  /** Function to interpolate the moving image. */
  InterpolatorPointer             m_MovingImageInterpolator;

  /** The global timestep. */
  TimeStepType                    m_TimeStep;

  /** Threshold below which the denominator term is considered zero. */
  double                          m_DenominatorThreshold;

  /** Threshold below which two intensity value are assumed to match. */
  double                          m_IntensityDifferenceThreshold;

/** This is the relative weight of the second distance map. */
  float							m_SecondDistanceMapWeight;


  /** The metric value is the mean square difference in intensity between
   * the fixed image and transforming moving image computed over the 
   * the overlapping region between the two images. */
  mutable double                  m_Metric;
  mutable double                  m_SumOfSquaredDifference;
  mutable unsigned long           m_NumberOfPixelsProcessed;
  mutable double                  m_RMSChange;
  mutable double                  m_SumOfSquaredChange;

  /** Mutex lock to protect modification to metric. */
  mutable SimpleFastMutexLock     m_MetricCalculationLock;

};


} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkjwsDemonsRegistrationFunction.txx"
#endif

#endif


