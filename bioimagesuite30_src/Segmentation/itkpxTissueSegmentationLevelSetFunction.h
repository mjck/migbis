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
  Module:    $RCSfile: itkTissueSegmentationLevelSetFunction.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:39 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTissueSegmentationLevelSetFunction_h_
#define __itkTissueSegmentationLevelSetFunction_h_

#include "itkSegmentationLevelSetFunction.h"
#include "itkConstNeighborhoodIterator.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkNumericTraits.h"

#ifndef M_PI
#define M_PI 3.14159
#endif

namespace itk {

/** \class TissueSegmentationLevelSetFunction
 *    
 * \brief This function is used in TissueSegmentationLevelSetImageFilter to
 * segment structures in images based on intensity values.
 *
 * \par  SegmentationLevelSetFunction is a subclass of the generic LevelSetFunction.
 * It useful for segmentations based on intensity values in an image.  It works
 * by constructing a speed term (feature image) with positive values inside an
 * intensity window (between a low and high threshold) and negative values
 * outside that intensity window.  The evolving level set front will lock onto
 * regions that are at the edges of the intensity window.
 *
 *  You may optionally add a Laplacian calculation on the image to the
 *  threshold-based speed term by setting the EdgeWeight parameter to a
 *  non-zero value.  The Laplacian term will cause the evolving surface to
 *  be more strongly attracted to image edges.   Several parameters control a
 *  preprocessing FeatureImage smoothing stage applied only to the Laplacian
 *  calculation.
 *
 *  \par
 *  Image $f$ is thresholded pixel by pixel using upper threshold
 *  $U$ and lower threshold $L$ according to the following formula.
 *
 * \par
 *  \f$  f(x) = \left\{ \begin{array}{ll} g(x) - L & \mbox{if $(g)x < (U-L)/2 + L$} \\ U - g(x) & \mbox{otherwise} \end{array} \right. \f$ 
 *
 * \sa SegmentationLevelSetImageFunction
 *  \sa TissueSegmentationLevelSetImageFilter */
template <class TImageType, class TFeatureImageType = TImageType>
class TissueSegmentationLevelSetFunction
  : public SegmentationLevelSetFunction<TImageType, TFeatureImageType>
{
public:
  /** Standard class typedefs. */
  typedef TissueSegmentationLevelSetFunction Self;
  typedef SegmentationLevelSetFunction<TImageType, TFeatureImageType> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  typedef TFeatureImageType FeatureImageType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro( TissueSegmentationLevelSetFunction, SegmentationLevelSetFunction );

  /** Extract some parameters from the superclass. */
  typedef typename Superclass::ImageType ImageType;
  typedef typename Superclass::ScalarValueType ScalarValueType;
  typedef typename Superclass::FeatureScalarType FeatureScalarType;
  typedef typename Superclass::RadiusType RadiusType;
  typedef typename Superclass::FloatOffsetType FloatOffsetType;
  typedef typename Superclass::VectorImageType VectorImageType;
  typedef typename Superclass::VectorType VectorType;
  typedef typename Superclass::PixelType PixelType;
  typedef typename Superclass::TimeStepType TimeStepType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;
  typedef typename Superclass::NeighborhoodType NeighborhoodType;
  

  /** Extract some parameters from the superclass. */
  itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);


  // ** Image and Image Iterator definition */
  typedef ImageRegionIterator<ImageType>                    ImageIterator;
  typedef ImageRegionConstIterator<ImageType>               ConstImageIterator;
  typedef ImageRegionConstIteratorWithIndex<ImageType>      ConstImageIteratorWithIndex;
  typedef ConstNeighborhoodIterator<ImageType>              NeighborhoodIterator;

  typedef itk::CovariantVector<float,3>                     GradientOutputPixel;
  typedef itk::Image<GradientOutputPixel,pxImageDimension>  GradientOutputType;
  typedef ImageRegionConstIterator<GradientOutputType>      GradientImageIterator;


  // -------------------------------------
  // Stuff for Using Tissue Specific Speed
  // -------------------------------------
  void SetUseTissueSpecificSpeedMode(int m) {
    if (m<0 || m>2)  m=0;
    m_UseTissueSpecificSpeedMode=m;
    
  }
  
  int GetUseTissueSpecificSpeedMode() { 
    return m_UseTissueSpecificSpeedMode;
  }

  // ------------------------------------------------------
  // Use Chen/Vesse \delta(x) instead of |\nabla \phi| term
  // ------------------------------------------------------
  void SetUseNonMorphometricTerm(int m) { 
    m_UseNonMorphometricTerm=(m>0);
  }
  
  int GetUseNonMorphometricTerm() {
    return m_UseNonMorphometricTerm;
  }

  // ------------------------------------------------------
  // Epsilon Term for Heavy Side Function
  // ------------------------------------------------------
  void SetHeavySideEpsilon(double d) {
    m_HeavySideEpsilon=d;
  }

  double GetHeavySideEpsilon() {
    return m_HeavySideEpsilon;
  }


  /** Set/Get the sigma for the Gaussian kernel used to compute the gradient
   * of the feature image needed for the advection term of the equation. */
  void SetDerivativeSigma( const double v )
  { m_DerivativeSigma = v; }
  double GetDerivativeSigma()
  { return m_DerivativeSigma; };


  /** Set/Get threshold values */
  void SetInsideTissue(FeatureScalarType m,FeatureScalarType s)  {
    m_InsideMean  = m;
    m_InsideSigma = s;
    if (m_InsideSigma<1.0)
      m_InsideSigma=1.0;
  }
  FeatureScalarType GetInsideMean() const  { return m_InsideMean; }
  FeatureScalarType GetInsideSigma() const  { return m_InsideSigma; }


  void SetOutsideTissue(FeatureScalarType m,FeatureScalarType s)  {
    m_OutsideMean  = m;
    m_OutsideSigma = s;
    if (m_OutsideSigma<1.0)
      m_OutsideSigma=1.0;

  }
  FeatureScalarType GetOutsideMean() const  { return m_OutsideMean; }
  FeatureScalarType GetOutsideSigma() const  { return m_OutsideSigma; }


  // Second Outside Class
  void SetBiExponentialOutside(int m) { 
    m_BiExponentialOutside=(m>0);
  }
  
  int GetUseBiExponentialOutside() {
    return m_BiExponentialOutside;
  }


  void SetOutside2Tissue(FeatureScalarType m,FeatureScalarType s)  {
    m_Outside2Mean  = m;
    m_Outside2Sigma = s;
    if (m_Outside2Sigma<1.0)
      m_Outside2Sigma=1.0;

  }
  FeatureScalarType GetOutside2Mean() const  { return m_Outside2Mean; }
  FeatureScalarType GetOutside2Sigma() const  { return m_Outside2Sigma; }


  // 
  //  This is from Yun Zhu et al Cardiac Segmentation Volume Constraint
  //
  // ApproximateVolume  is volume that the body should be approximately
  // ApproximateVolumeConstraintWeight is the weight of this constraint
  void SetApproximateVolume(double v)
  {
    m_ApproximateVolume=v;
  }

  double GetApproximateVolume() const
  {
    return m_ApproximateVolume;
  }

  void SetMyVolume(double v)
  {
    m_MyVolume=v;
  }

  double GetMyVolume() const
  {
    return m_MyVolume;
  }


  void SetApproximateVolumeConstraintWeight(double v)
  {
    m_ApproximateVolumeConstraintWeight=v;
  }

  double GetApproximateVolumeConstraintWeight() const
  {
    return m_ApproximateVolumeConstraintWeight;
  }





  virtual void CalculateSpeedImage();
  virtual void CalculateAdvectionImage();

  virtual void Initialize(const RadiusType &r)
  {
    Superclass::Initialize(r);
    //    this->SetAdvectionWeight( NumericTraits<ScalarValueType>::Zero);
    this->SetPropagationWeight(-1.0 * NumericTraits<ScalarValueType>::One);
    this->SetCurvatureWeight(NumericTraits<ScalarValueType>::One);
    this->SetAdvectionWeight( NumericTraits<ScalarValueType>::One );
    m_DerivativeSigma = 1.0;
  }

  /** Set/Get the weight applied to the edge (Laplacian) attractor in the speed
   *  term function. Zero will turn this term off. */
  void SetEdgeWeight(const ScalarValueType p)
  {
    m_EdgeWeight = p;
  }
  ScalarValueType GetEdgeWeight() const
  {
    return m_EdgeWeight;
  }
  
  
  FeatureScalarType InsideTissueFunction(FeatureScalarType v) {
    return ( (v-m_InsideMean)*(v-m_InsideMean)/(m_InsideSigma*m_InsideSigma )) + 0.5*log(2.0*M_PI)+log(m_InsideSigma);
  }

  FeatureScalarType OutsideTissueFunction(FeatureScalarType v) {
    return ( (v-m_OutsideMean)*(v-m_OutsideMean)/(m_OutsideSigma*m_OutsideSigma )) + 0.5*log(2.0*M_PI)+log(m_OutsideSigma);
  }

  FeatureScalarType Outside2TissueFunction(FeatureScalarType v) {
    return ( (v-m_Outside2Mean)*(v-m_Outside2Mean)/(m_Outside2Sigma*m_Outside2Sigma )) + 0.5*log(2.0*M_PI)+log(m_Outside2Sigma);
  }


  /** Get/Set the image used as the speed function in the level set equation */
  typedef typename Superclass::GlobalDataStruct GlobalDataStruct;

  virtual PixelType ComputeUpdate(const NeighborhoodType &neighborhood,
                                  void *globalData,
                                  const FloatOffsetType& = FloatOffsetType(0.0));




protected:
  TissueSegmentationLevelSetFunction()
  {
    this->SetAdvectionWeight(0.0);
    this->SetPropagationWeight(1.0);
    this->SetCurvatureWeight(1.0);
    this->SetEdgeWeight(0.0);
    this->SetInsideTissue(100.0,10.0);
    this->SetOutsideTissue(0.0,10.0);
    this->SetOutside2Tissue(0.0,-1.0);
    m_UseTissueSpecificSpeedMode=1;
    m_UseNonMorphometricTerm=0;
    m_BiExponentialOutside=0;
    m_HeavySideEpsilon=1.0;

    m_ApproximateVolume=100.0;
    m_MyVolume=100.0;
    m_ApproximateVolumeConstraintWeight=0.0;
  }

  virtual ~TissueSegmentationLevelSetFunction() {

    
  }

  TissueSegmentationLevelSetFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  void PrintSelf(std::ostream& os, Indent indent) const
  {
    Superclass::PrintSelf(os, indent );
    os << indent << "Use Tissue Specific Speed Mode (0=none,1=Chen & Vesse,2=Xiaolan)" << m_UseTissueSpecificSpeedMode << std::endl;
    os << indent << "Use NonMorphometric Speed Multiplier " << m_UseNonMorphometricTerm << std::endl;
    os << indent << "Heavy Side Epsilon " << m_HeavySideEpsilon << std::endl;
    os << indent << "Inside Tissue: " << m_InsideMean << " + " << m_InsideSigma << std::endl;
    os << indent << "Outside Tissue: " << m_OutsideMean << " + " << m_OutsideSigma << std::endl;
    os << indent << "Outside2 Tissue: " << m_Outside2Mean << " + " << m_Outside2Sigma << std::endl;
    os << indent << "BiExponenital Outside: " << m_BiExponentialOutside << std::endl;
    os << indent << "EdgeWeight: " << m_EdgeWeight << std::endl;
    os << indent << "DerivativeSigma: " << m_DerivativeSigma << std::endl;
  }
  
  FeatureScalarType m_InsideMean;
  FeatureScalarType m_InsideSigma;
  FeatureScalarType m_OutsideMean;
  FeatureScalarType m_OutsideSigma;
  FeatureScalarType m_Outside2Mean;
  FeatureScalarType m_Outside2Sigma;
  ScalarValueType   m_EdgeWeight;
  double            m_ApproximateVolume;
  double            m_MyVolume;
  double            m_ApproximateVolumeConstraintWeight;

  double m_DerivativeSigma;
  double m_HeavySideEpsilon;
  int  m_UseNonMorphometricTerm;
  int  m_UseTissueSpecificSpeedMode;
  int  m_BiExponentialOutside;

    /** The image holding the speed values for front propagation */

};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkpxTissueSegmentationLevelSetFunction.txx"
#endif

#endif

