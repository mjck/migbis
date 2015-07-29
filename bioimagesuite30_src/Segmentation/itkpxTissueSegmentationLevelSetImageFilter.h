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
  Module:    $RCSfile: itkpxTissueSegmentationLevelSetImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2004/06/01 18:33:45 $
  Version:   $Revision: 1.19 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkpxTissueSegmentationLevelSetImageFilter_h_
#define __itkpxTissueSegmentationLevelSetImageFilter_h_

#include "itkSegmentationLevelSetImageFilter.h"
#include "itkpxTissueSegmentationLevelSetFunction.h"

namespace itk {

/** \class TissueSegmentationLevelSetImageFilter
 *    \brief Segments structures in images based on intensity values.
 *
 *   \par IMPORTANT
 *   The SegmentationLevelSetImageFilter class and the
 *   TissueSegmentationLevelSetFunction class contain additional information necessary
 *   to the full understanding of how to use this filter.
 *
 *    \par OVERVIEW
 *    This class is a level set method segmentation filter.  It constructs a
 *    speed function which is close to zero at the upper and lower bounds of an
 *    intensity window, effectively locking the propagating front onto those
 *    edges.  Elsewhere, the front will propagate quickly.
 *
 *    \par INPUTS
 *    This filter requires two inputs.  The first input is a seed
 *    image.  This seed image must contain an isosurface that you want to use as the
 *    seed for your segmentation.  It can be a binary, graylevel, or floating
 *    point image.  The only requirement is that it contain a closed isosurface
 *    that you will identify as the seed by setting the IsosurfaceValue parameter
 *    of the filter.  For a binary image you will want to set your isosurface
 *    value halfway between your on and off values (i.e. for 0's and 1's, use an
 *    isosurface value of 0.5).
 *
 *    \par
 *    The second input is the feature image.  This is the image from which the
 *    speed function will be calculated.  For most applications, this is the
 *    image that you want to segment. The desired isosurface in your seed image
 *    should lie within the region of your feature image that you are trying to
 *    segment. Note that this filter does no preprocessing of the feature image
 *    before thresholding.
 *
 *    \par
 *    See SegmentationLevelSetImageFilter for more information on Inputs.
 *
 *    \par OUTPUTS
 *    The filter outputs a single, scalar, real-valued image.
 *    Positive values in the output image are inside the segmentated region
 *    and negative values in the image are outside of the inside region.  The
 *    zero crossings of the image correspond to the position of the level set
 *    front.
 *
 *   \par
 *   See SparseFieldLevelSetImageFilter and
 *   SegmentationLevelSetImageFilter for more information.
 *
 *   \par PARAMETERS
 *   In addition to parameters described in SegmentationLevelSetImageFilter,
 *   this filter adds the UpperTissue and LowerTissue.  See
 *   TissueSegmentationLevelSetFunction for a description of how these values
 *   affect the segmentation.
 *
 *   \sa SegmentationLevelSetImageFilter
 *   \sa TissueSegmentationLevelSetFunction,
 *   \sa SparseFieldLevelSetImageFilter */
template <class TInputImage,
          class TFeatureImage,
          class TOutputPixelType = float >
class TissueSegmentationLevelSetImageFilter : public SegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputPixelType >
{
public:
  /** Standard class typedefs */
  typedef TissueSegmentationLevelSetImageFilter Self;
  typedef  SegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputPixelType > Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Inherited typedef from the superclass. */
  typedef typename Superclass::ValueType ValueType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename Superclass::FeatureImageType FeatureImageType;
  typedef typename Superclass::IndexType IndexType;

  
  /** Type of the segmentation function */
  typedef TissueSegmentationLevelSetFunction<OutputImageType,
                                                FeatureImageType> TissueFunctionType;
  typedef typename TissueFunctionType::Pointer TissueFunctionPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(TissueSegmentationLevelSetImageFilter, SegmentationLevelSetImageFilter);

  /** Method for creation through the object factory */
  itkNewMacro(Self);
  
  /** Get/Set the threshold values that will be used to calculate the speed function. **/
  void SetInsideTissue(ValueType m,ValueType f)
  {
    this->m_TissueFunction->SetInsideTissue(m,f);
    this->Modified();
  }
  void SetOutsideTissue(ValueType m,ValueType f)
  {
    this->m_TissueFunction->SetOutsideTissue(m,f);
    this->Modified();
  }

  void SetOutside2Tissue(ValueType m,ValueType f)
  {
    this->m_TissueFunction->SetOutside2Tissue(m,f);
    this->Modified();
  }


  ValueType GetInsideMean() const
  {
    return m_TissueFunction->GetInsideMean();
  }

  ValueType GetInsideSigma() const
  {
    return m_TissueFunction->GetInsideSigma();
  }

  ValueType GetOutsideMean() const
  {
    return m_TissueFunction->GetOutsideMean();
  }

  ValueType GetOutsideSigma() const
  {
    return m_TissueFunction->GetOutsideSigma();
  }



  ValueType GetOutside2Mean() const
  {
    return m_TissueFunction->GetOutside2Mean();
  }

  ValueType GetOutside2Sigma() const
  {
    return m_TissueFunction->GetOutside2Sigma();
  }


  /** Set/Get the weight applied to the edge (Laplacian) attractor in the speed
   *  term function. Zero will turn this term off. */
  void SetEdgeWeight(ValueType v)
  {
    this->m_TissueFunction->SetEdgeWeight(v);
    this->Modified();
  }

  ValueType GetEdgeWeight() const
  {
    return m_TissueFunction->GetEdgeWeight();
  }


  void SetHeavySideEpsilon(double v)
  {
    this->m_TissueFunction->SetHeavySideEpsilon(v);
  }

  double GetHeavySideEpsilon() const
  {
    return m_TissueFunction->GetHeavySideEpsilon();
  }

  // 
  //  This is from Yun Zhu et al Cardiac Segmentation Volume Constraint
  //
  // ApproximateVolume  is volume that the body should be approximately
  // ApproximateVolumeConstraintWeight is the weight of this constraint
  void SetApproximateVolume(double v)
  {
    this->m_TissueFunction->SetApproximateVolume(v);
  }

  double GetApproximateVolume() const
  {
    return m_TissueFunction->GetApproximateVolume();
  }

  void SetApproximateVolumeConstraintWeight(double v)
  {
    this->m_TissueFunction->SetApproximateVolumeConstraintWeight(v);
  }

  double GetApproximateVolumeConstraintWeight() const
  {
    return m_TissueFunction->GetApproximateVolumeConstraintWeight();
  }


  void  SetBiExponentialOutside(const int p)  {    m_TissueFunction->SetBiExponentialOutside(p);       }
  int   GetBiExponentialOutside() const       {    return m_TissueFunction->GetBiExponentialOutside(); }


  itkSetMacro(DynamicUpdateParameters,int);
  itkGetMacro(DynamicUpdateParameters,int);
  
protected:
  ~TissueSegmentationLevelSetImageFilter() {}
  TissueSegmentationLevelSetImageFilter();

  virtual void PrintSelf(std::ostream &os, Indent indent) const; 

  TissueSegmentationLevelSetImageFilter(const Self &); // purposely not impl.
  void operator=(const Self&); //purposely not implemented

  /** Overrides parent implementation. Allows for Chen and Vesse like term computation */
  virtual void InitializeIteration();



private:
  TissueFunctionPointer m_TissueFunction;
  int m_DynamicUpdateParameters;
};

} // end namespace itk



#ifndef ITK_MANUAL_INSTANTIATION
#include "itkpxTissueSegmentationLevelSetImageFilter.txx"
#endif

#endif

