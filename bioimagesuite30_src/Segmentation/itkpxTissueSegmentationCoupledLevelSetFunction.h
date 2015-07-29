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
  Module:    $RCSfile: itkTissueSegmentationCoupledLevelSetFunction.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:39 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkTissueSegmentationCoupledLevelSetFunction_h_
#define __itkTissueSegmentationCoupledLevelSetFunction_h_

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

/** \class TissueSegmentationCoupledLevelSetFunction
 *    
 * \brief This function is used in TissueSegmentationCoupledLevelSetImageFilter to
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
 *  \sa TissueSegmentationCoupledLevelSetImageFilter */
template <class TImageType, class TFeatureImageType = TImageType>
class TissueSegmentationCoupledLevelSetFunction
  : public SegmentationLevelSetFunction<TImageType, TFeatureImageType>
{
public:
  /** Standard class typedefs. */
  typedef TissueSegmentationCoupledLevelSetFunction Self;
  typedef SegmentationLevelSetFunction<TImageType, TFeatureImageType> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  typedef TFeatureImageType FeatureImageType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods) */
  itkTypeMacro( TissueSegmentationCoupledLevelSetFunction, SegmentationLevelSetFunction );

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

  /* ------------------------------------------------------
     Use Chen/ Vesse \delta(x) instead of |\nabla \phi| term
  ------------------------------------------------------ */
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
  void SetWhiteMatterTissue(FeatureScalarType m,FeatureScalarType s)  {
    m_WhiteMatterMean  = m;
    m_WhiteMatterSigma = s;
    if (m_WhiteMatterSigma<1.0)
      m_WhiteMatterSigma=1.0;
  }
  FeatureScalarType GetWhiteMatterMean() const  { return m_WhiteMatterMean; }
  FeatureScalarType GetWhiteMatterSigma() const  { return m_WhiteMatterSigma; }


  void SetGrayMatterTissue(FeatureScalarType m,FeatureScalarType s)  {
    m_GrayMatterMean  = m;
    m_GrayMatterSigma = s;
    if (m_GrayMatterSigma<1.0)
      m_GrayMatterSigma=1.0;

  }
  FeatureScalarType GetGrayMatterMean() const  { return m_GrayMatterMean; }
  FeatureScalarType GetGrayMatterSigma() const  { return m_GrayMatterSigma; }

  void SetCSFTissue(FeatureScalarType m,FeatureScalarType s)  {
    m_CSFMean  = m;
    m_CSFSigma = s;
    if (m_CSFSigma<1.0)
      m_CSFSigma=1.0;

  }
  FeatureScalarType GetCSFMean() const  { return m_CSFMean; }
  FeatureScalarType GetCSFSigma() const  { return m_CSFSigma; }


  // Get/Set the boundaries of the two images 
  void SetCoupledLevelSetBoundaries(int innerboundary,int outerboundary)
  {
    m_InnerBoundary=innerboundary;
    m_OuterBoundary=outerboundary;
  }

  int GetInnerBoundary() const
  {
    return m_InnerBoundary;
  }

  int GetOuterBoundary() const
  {
    return m_OuterBoundary;
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
  

  void  SetLowerWhiteGrayBound(const float p)  {    m_LowerWhiteGrayBound = p;  }
  float GetLowerWhiteGrayBound() const         {    return m_LowerWhiteGrayBound;  }

  void  SetUpperWhiteGrayBound(const float p)  {    m_UpperWhiteGrayBound = p;  }
  float GetUpperWhiteGrayBound() const         {    return m_UpperWhiteGrayBound;  }

  void  SetLowerGrayCSFBound(const float p)  {    m_LowerGrayCSFBound = p;  }
  float GetLowerGrayCSFBound() const         {    return m_LowerGrayCSFBound;  }

  void  SetUpperGrayCSFBound(const float p)  {    m_UpperGrayCSFBound = p;  }
  float GetUpperGrayCSFBound() const         {    return m_UpperGrayCSFBound;  }

  void  SetConstraintOffset(const float p)  {    m_ConstraintOffset = p;  }
  float GetConstraintOffset() const         {    return m_ConstraintOffset;  }

  
  FeatureScalarType WhiteMatterTissueFunction(FeatureScalarType v) {
    return ( (v-m_WhiteMatterMean)*(v-m_WhiteMatterMean)/(m_WhiteMatterSigma*m_WhiteMatterSigma )) + 0.5*log(2.0*M_PI)+log(m_WhiteMatterSigma);
  }

  FeatureScalarType GrayMatterTissueFunction(FeatureScalarType v) {
    return ( (v-m_GrayMatterMean)*(v-m_GrayMatterMean)/(m_GrayMatterSigma*m_GrayMatterSigma )) + 0.5*log(2.0*M_PI)+log(m_GrayMatterSigma);
  }

  FeatureScalarType CSFTissueFunction(FeatureScalarType v) {
    return ( (v-m_CSFMean)*(v-m_CSFMean)/(m_CSFSigma*m_CSFSigma )) + 0.5*log(2.0*M_PI)+log(m_CSFSigma);
  }


  /** Get/Set the image used as the speed function in the level set equation */
  typedef typename Superclass::GlobalDataStruct GlobalDataStruct;

  virtual PixelType ComputeUpdate(const NeighborhoodType &neighborhood,
                                  void *globalData,
                                  const FloatOffsetType& = FloatOffsetType(0.0));



protected:
  TissueSegmentationCoupledLevelSetFunction()
  {
    this->SetAdvectionWeight(0.0);
    this->SetPropagationWeight(1.0);
    this->SetCurvatureWeight(1.0);
    this->SetEdgeWeight(0.0);
    this->SetWhiteMatterTissue(100.0,10.0);
    this->SetGrayMatterTissue(0.0,10.0);
    this->SetCSFTissue(0.0,-1.0);
    m_LowerWhiteGrayBound=-3.0;
    m_UpperWhiteGrayBound=-8.0;
    m_LowerGrayCSFBound=3.0;
    m_UpperGrayCSFBound=8.0;

    m_UseNonMorphometricTerm=0;
    m_HeavySideEpsilon=1.0;
    m_ConstraintOffset=1.0;
  }

  virtual ~TissueSegmentationCoupledLevelSetFunction() {
  }

  TissueSegmentationCoupledLevelSetFunction(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  void PrintSelf(std::ostream& os, Indent indent) const
  {
    Superclass::PrintSelf(os, indent );
    os << indent << "Use NonMorphometric Speed Multiplier " << m_UseNonMorphometricTerm << std::endl;
    os << indent << "Heavy Side Epsilon " << m_HeavySideEpsilon << std::endl;
    os << indent << "WhiteMatter Tissue: " << m_WhiteMatterMean << " + " << m_WhiteMatterSigma << std::endl;
    os << indent << "GrayMatter Tissue: " << m_GrayMatterMean << " + " << m_GrayMatterSigma << std::endl;
    os << indent << "CSF Tissue: " << m_CSFMean << " + " << m_CSFSigma << std::endl;
    os << indent << "Boundaries: " << m_InnerBoundary << " + " << m_OuterBoundary << std::endl;
    os << indent << "EdgeWeight: " << m_EdgeWeight << std::endl;

    os << indent << "WhiteGray  Bounds :" <<  m_LowerWhiteGrayBound << " to " << m_UpperWhiteGrayBound << std::endl;
    os << indent << "GrayCSF    Bounds :" <<  m_LowerGrayCSFBound << " to " << m_UpperGrayCSFBound << std::endl;
    os << indent << "Constraint Weight :" <<  m_ConstraintOffset << std::endl;
    os << indent << "DerivativeSigma: " << m_DerivativeSigma << std::endl;
  }
  
  FeatureScalarType m_WhiteMatterMean;
  FeatureScalarType m_WhiteMatterSigma;
  FeatureScalarType m_GrayMatterMean;
  FeatureScalarType m_GrayMatterSigma;
  FeatureScalarType m_CSFMean;
  FeatureScalarType m_CSFSigma;
  ScalarValueType   m_EdgeWeight;
  float             m_LowerWhiteGrayBound;
  float             m_UpperWhiteGrayBound;
  float             m_LowerGrayCSFBound;
  float             m_UpperGrayCSFBound;

  float             m_ConstraintOffset;
  double m_DerivativeSigma;
  double m_HeavySideEpsilon;
  int  m_UseNonMorphometricTerm;
  int  m_InnerBoundary;
  int  m_OuterBoundary;
  /** The image holding the speed values for front propagation */

};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkpxTissueSegmentationCoupledLevelSetFunction.txx"
#endif

#endif

