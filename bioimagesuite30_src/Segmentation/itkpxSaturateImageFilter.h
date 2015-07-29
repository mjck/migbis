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
  Module:    $RCSfile: itkpxSaturateImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2004/10/26 13:59:18 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSaturateImageFilter_h
#define __itkSaturateImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkArray.h"

namespace itk
{

/** \class SaturateImageFilter
 * \brief Shift and scale the pixels in an image.
 *
 * SaturateImageFilter shifts the input pixel by Shift (default 0.0)
 * and then scales the pixel by Scale (default 1.0). All computattions
 * are performed in the precison of the input pixel's RealType. Before
 * assigning the computed value to the output pixel, the value is clamped
 * at the NonpositiveMin and max of the pixel type.
 * \ingroup IntensityImageFilters
 *
 */
template <class TInputImage, class TOutputImage>
class SaturateImageFilter:
    public ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef SaturateImageFilter         Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage>  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);  

  /** Typedef to describe the output and input image region types. */
  typedef typename TInputImage::RegionType InputImageRegionType;
  typedef typename TOutputImage::RegionType OutputImageRegionType;

  /** Typedef to describe the pointer to the input/output. */  
  typedef typename TInputImage::Pointer InputImagePointer;
  typedef typename TOutputImage::Pointer OutputImagePointer;

  /** Typedef to describe the type of pixel. */
  typedef typename TInputImage::PixelType InputImagePixelType;
  typedef typename TOutputImage::PixelType OutputImagePixelType;

  /** Typedef to describe the output and input image index and size types. */
  typedef typename TInputImage::IndexType InputImageIndexType;
  typedef typename TInputImage::SizeType InputImageSizeType;
  typedef typename TInputImage::OffsetType InputImageOffsetType;
  typedef typename TOutputImage::IndexType OutputImageIndexType;
  typedef typename TOutputImage::SizeType OutputImageSizeType;
  typedef typename TOutputImage::OffsetType OutputImageOffsetType;

  /** Type to use form computations. */
  typedef typename NumericTraits<OutputImagePixelType>::RealType RealType;
      
  /** Image related typedefs. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension ) ;

  /** Run-time type information (and related methods). */
  itkTypeMacro(SaturateImageFilter, ImageToImageFilter);

  /** Set/Get the amount to Shift each Pixel. The shift is followed by a Scale. */
  itkSetMacro(LowerBound,RealType);
  itkGetMacro(LowerBound,RealType);

  /** Set/Get the amount to Shift each Pixel. The shift is followed by a Scale. */
  itkSetMacro(UpperBound,RealType);
  itkGetMacro(UpperBound,RealType);

  /** Set/Get the amount to Scale each Pixel. The Scale is applied after the Shift. */
  itkSetMacro(Scale,RealType);
  itkGetMacro(Scale,RealType);

  /** Get the number of pixels that underflowed and overflowed. */
  itkGetMacro(UnderflowCount,long);
  itkGetMacro(OverflowCount,long);


protected:
  SaturateImageFilter();
  ~SaturateImageFilter();
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Initialize some accumulators before the threads run. */
  void BeforeThreadedGenerateData ();
  
  /** Tally accumulated in threads. */
  void AfterThreadedGenerateData ();
  
  /** Multi-thread version GenerateData. */
  void  ThreadedGenerateData (const OutputImageRegionType& 
                              outputRegionForThread,
                              int threadId) ;
private:
  SaturateImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  RealType m_LowerBound;
  RealType m_UpperBound;
  RealType m_Scale;

  long m_UnderflowCount;
  long m_OverflowCount;
  Array<long> m_ThreadUnderflow;
  Array<long> m_ThreadOverflow;
};

  
} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkpxSaturateImageFilter.txx"
#endif
  
#endif

