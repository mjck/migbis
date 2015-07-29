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
  Module:    $RCSfile: itkpxTissueSegmentationCoupledLevelSetImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2004/06/01 18:33:45 $
  Version:   $Revision: 1.19 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkpxTissueSegmentationCoupledLevelSetImageFilter_h_
#define __itkpxTissueSegmentationCoupledLevelSetImageFilter_h_

#include "itkSegmentationLevelSetImageFilter.h"
#include "itkpxTissueSegmentationCoupledLevelSetFunction.h"

namespace itk {

/** \class TissueSegmentationCoupledLevelSetImageFilter
 *    \brief Segments structures in images based on intensity values.
 *
 *   \par IMPORTANT
 *   The SegmentationLevelSetImageFilter class and the
 *   TissueSegmentationCoupledLevelSetFunction class contain additional information necessary
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
 *   TissueSegmentationCoupledLevelSetFunction for a description of how these values
 *   affect the segmentation.
 *
 *   \sa SegmentationLevelSetImageFilter
 *   \sa TissueSegmentationCoupledLevelSetFunction,
 *   \sa SparseFieldLevelSetImageFilter */
template <class TInputImage,
          class TFeatureImage,
          class TOutputPixelType = float >
class TissueSegmentationCoupledLevelSetImageFilter : public SegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputPixelType >
{
public:
  /** Standard class typedefs */
  typedef TissueSegmentationCoupledLevelSetImageFilter Self;
  typedef  SegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputPixelType > Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Inherited typedef from the superclass. */
  typedef typename Superclass::ValueType ValueType;
  typedef typename Superclass::OutputImageType OutputImageType;
  typedef typename Superclass::FeatureImageType FeatureImageType;
  typedef typename Superclass::IndexType IndexType;

  
  /** Type of the segmentation function */
  typedef TissueSegmentationCoupledLevelSetFunction<OutputImageType,
                                                FeatureImageType> TissueFunctionType;
  typedef typename TissueFunctionType::Pointer TissueFunctionPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(TissueSegmentationCoupledLevelSetImageFilter, SegmentationLevelSetImageFilter);

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Get/Set the boundaries of the two images */
  void SetCoupledLevelSetBoundaries(int innerboundary,int outerboundary)
  {
    this->m_TissueFunction->SetCoupledLevelSetBoundaries(innerboundary,outerboundary);
    this->Modified();
  }

  int GetInnerBoundary() const
  {
    return m_TissueFunction->GetInnerBoundary();
  }

  int GetOuterBoundary() const
  {
    return m_TissueFunction->GetOuterBoundary();
  }

 
  /** Get/Set the threshold values that will be used to calculate the speed function. */
  void SetWhiteMatterTissue(ValueType m,ValueType f)
  {
    this->m_TissueFunction->SetWhiteMatterTissue(m,f);
    this->Modified();
  }
  void SetGrayMatterTissue(ValueType m,ValueType f)
  {
    this->m_TissueFunction->SetGrayMatterTissue(m,f);
    this->Modified();
  }

  void SetCSFTissue(ValueType m,ValueType f)
  {
    this->m_TissueFunction->SetCSFTissue(m,f);
    this->Modified();
  }


  ValueType GetWhiteMatterMean() const
  {
    return m_TissueFunction->GetWhiteMatterMean();
  }

  ValueType GetWhiteMatterSigma() const
  {
    return m_TissueFunction->GetWhiteMatterSigma();
  }

  ValueType GetGrayMatterMean() const
  {
    return m_TissueFunction->GetGrayMatterMean();
  }

  ValueType GetGrayMatterSigma() const
  {
    return m_TissueFunction->GetGrayMatterSigma();
  }



  ValueType GetCSFMean() const
  {
    return m_TissueFunction->GetCSFMean();
  }

  ValueType GetCSFSigma() const
  {
    return m_TissueFunction->GetCSFSigma();
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


  // Coupling of Levelsets Stuff
  // ---------------------------
  void  SetLowerWhiteGrayBound(const float p)  {    m_TissueFunction->SetLowerWhiteGrayBound(p);       }
  float GetLowerWhiteGrayBound() const         {    return m_TissueFunction->GetLowerWhiteGrayBound(); }
  
  void  SetUpperWhiteGrayBound(const float p)  {    m_TissueFunction->SetUpperWhiteGrayBound(p);       }
  float GetUpperWhiteGrayBound() const         {    return m_TissueFunction->GetUpperWhiteGrayBound(); }

  void  SetLowerGrayCSFBound(const float p)  {    m_TissueFunction->SetLowerGrayCSFBound(p);       }
  float GetLowerGrayCSFBound() const         {    return m_TissueFunction->GetLowerGrayCSFBound(); }
  
  void  SetUpperGrayCSFBound(const float p)  {    m_TissueFunction->SetUpperGrayCSFBound(p);       }
  float GetUpperGrayCSFBound() const         {    return m_TissueFunction->GetUpperGrayCSFBound(); }

  void  SetConstraintOffset(const float p)  {    m_TissueFunction->SetConstraintOffset(p);       }
  float GetConstraintOffset() const         {    return m_TissueFunction->GetConstraintOffset(); }

  
protected:
  ~TissueSegmentationCoupledLevelSetImageFilter() {}
  TissueSegmentationCoupledLevelSetImageFilter();

  virtual void PrintSelf(std::ostream &os, Indent indent) const; 

  TissueSegmentationCoupledLevelSetImageFilter(const Self &); // purposely not impl.
  void operator=(const Self&); //purposely not implemented

  /** Overrides parent implementation. Allows for Chen and Vesse like term computation */
  //  virtual void InitializeIteration();



private:
  TissueFunctionPointer m_TissueFunction;
};

} // end namespace itk



#ifndef ITK_MANUAL_INSTANTIATION
#include "itkpxTissueSegmentationCoupledLevelSetImageFilter.txx"
#endif

#endif

