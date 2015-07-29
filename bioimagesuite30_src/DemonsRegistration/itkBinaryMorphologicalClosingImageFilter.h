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
  Module:    $RCSfile: itkBinaryMorphologicalClosingImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2007/06/07 09:05:07 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkBinaryMorphologicalClosingImageFilter_h
#define __itkBinaryMorphologicalClosingImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk {

/**
 * \class BinaryMorphologicalClosingImageFilter
 * \brief binary morphological closing of an image.
 *
 * This filter removes small (i.e., smaller than the structuring 
 * element) holes and tube like structures in the interior or at the
 * boundaries of the image. The morphological closing of an image
 * "f" is defined as:
 * Closing(f) = Erosion(Dilation(f)).
 *
 * The structuring element is assumed to be composed of binary
 * values (zero or one). Only elements of the structuring element
 * having values > 0 are candidates for affecting the center pixel.
 * 
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA of Jouy-en-Josas, France.
 *
 * \sa MorphologyImageFilter, GrayscaleDilateImageFilter, GrayscaleErodeImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */

template<class TInputImage, class TOutputImage, class TKernel>
class ITK_EXPORT BinaryMorphologicalClosingImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryMorphologicalClosingImageFilter         Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self>                            Pointer;
  typedef SmartPointer<const Self>                      ConstPointer;
  
  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(BinaryMorphologicalClosingImageFilter, 
               ImageToImageFilter);

  typedef TInputImage                              InputImageType;
  typedef TOutputImage                             OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;

  /** Declaration of pixel type. */
  typedef typename TInputImage::PixelType InputPixelType;
  typedef typename TInputImage::PixelType OutputPixelType;

  /** Kernel typedef. */
  typedef TKernel KernelType;

  /** Set kernel (structuring element). */
  itkSetMacro(Kernel, KernelType);

  /** Get the kernel (structuring element). */
  itkGetConstReferenceMacro(Kernel, KernelType);

  /** Set the value in the image to consider as "foreground". Defaults to
   * maximum value of InputPixelType. */
  itkSetMacro(ForegroundValue, InputPixelType);
 
  /** Get the value in the image considered as "foreground". Defaults to
   * maximum value of InputPixelType. */
  itkGetMacro(ForegroundValue, InputPixelType);
 
  
  /** A safe border is added to input image to avoid borders effects
   * and remove it once the closing is done */
  itkSetMacro(SafeBorder, bool);
  itkGetConstReferenceMacro(SafeBorder, bool);
  itkBooleanMacro(SafeBorder);
   
protected:
  BinaryMorphologicalClosingImageFilter();
  ~BinaryMorphologicalClosingImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** BinaryMorphologicalClosingImageFilter need to make sure they request enough of an
   * input image to account for the structuring element size.  The input
   * requested region is expanded by the radius of the structuring element.
   * If the request extends past the LargestPossibleRegion for the input,
   * the request is cropped by the LargestPossibleRegion. */
  void GenerateInputRequestedRegion();

  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleDilateImageFilter GrayscaleErodeImageFilter. */
  void  GenerateData ();

private:
  BinaryMorphologicalClosingImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  /** kernel or structuring element to use. */
  KernelType m_Kernel;
   
  InputPixelType m_ForegroundValue;
  
  bool m_SafeBorder;
}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryMorphologicalClosingImageFilter.txx"
#endif

#endif

