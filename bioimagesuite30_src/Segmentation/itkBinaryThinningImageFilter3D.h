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

#ifndef __itkBinaryThinningImageFilter3D_h
#define __itkBinaryThinningImageFilter3D_h

#include <itkNeighborhoodIterator.h>
#include <itkImageToImageFilter.h>
#include <itkImageRegionIteratorWithIndex.h>
#include <itkConstantBoundaryCondition.h>

namespace itk
{
/** \class BinaryThinningImageFilter3D
*
* \brief This filter computes one-pixel-wide skeleton of a 3D input image.
*
* This class is parametrized over the type of the input image
* and the type of the output image.
* 
* The input is assumed to be a binary image. All non-zero valued voxels
* are set to 1 internally to simplify the computation. The filter will
* produce a skeleton of the object.  The output background values are 0,
* and the foreground values are 1.
* 
* A 26-neighbourhood configuration is used for the foreground and a
* 6-neighbourhood configuration for the background. Thinning is performed
* symmetrically in order to guarantee that the skeleton lies medial within
* the object.
*
* This filter is a parallel thinning algorithm and is an implementation
* of the algorithm described in:
* 
* T.C. Lee, R.L. Kashyap, and C.N. Chu.
* Building skeleton models via 3-D medial surface/axis thinning algorithms.
* Computer Vision, Graphics, and Image Processing, 56(6):462--478, 1994.
* 
* To do: Make use of multi-threading.
*
* \author Hanno Homann, Oxford University, Wolfson Medical Vision Lab, UK.
* 
* \sa MorphologyImageFilter
* \ingroup ImageEnhancement MathematicalMorphologyImageFilters
*/

template <class TInputImage,class TOutputImage>
class BinaryThinningImageFilter3D :
    public ImageToImageFilter<TInputImage,TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryThinningImageFilter3D    Self;
  typedef ImageToImageFilter<TInputImage,TOutputImage> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Method for creation through the object factory */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro( BinaryThinningImageFilter3D, ImageToImageFilter );

  /** Type for input image. */
  typedef   TInputImage       InputImageType;

  /** Type for output image: Skelenton of the object.  */
  typedef   TOutputImage      OutputImageType;

  /** Type for the region of the input image. */
  typedef typename InputImageType::RegionType RegionType;

  /** Type for the index of the input image. */
  typedef typename RegionType::IndexType  IndexType;

  /** Type for the pixel type of the input image. */
  typedef typename InputImageType::PixelType InputImagePixelType ;

  /** Type for the pixel type of the input image. */
  typedef typename OutputImageType::PixelType OutputImagePixelType ;

  /** Type for the size of the input image. */
  typedef typename RegionType::SizeType SizeType;

  /** Pointer Type for input image. */
  typedef typename InputImageType::ConstPointer InputImagePointer;

  /** Pointer Type for the output image. */
  typedef typename OutputImageType::Pointer OutputImagePointer;
  
  /** Boundary condition type for the neighborhood iterator */
  typedef ConstantBoundaryCondition< TInputImage > ConstBoundaryConditionType;
  
  /** Neighborhood iterator type */
  typedef NeighborhoodIterator<TInputImage, ConstBoundaryConditionType> NeighborhoodIteratorType;
  
  /** Neighborhood type */
  typedef typename NeighborhoodIteratorType::NeighborhoodType NeighborhoodType;

  /** Get Skelenton by thinning image. */
  OutputImageType * GetThinning(void);

  /** ImageDimension enumeration   */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension );
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension );

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(SameDimensionCheck,
    (Concept::SameDimension<InputImageDimension, 3>));
  itkConceptMacro(SameTypeCheck,
    (Concept::SameType<InputImagePixelType, OutputImagePixelType>));
  itkConceptMacro(InputAdditiveOperatorsCheck,
    (Concept::AdditiveOperators<InputImagePixelType>));
  itkConceptMacro(InputConvertibleToIntCheck,
    (Concept::Convertible<InputImagePixelType, int>));
  itkConceptMacro(IntConvertibleToInputCheck,
    (Concept::Convertible<int, InputImagePixelType>));
  itkConceptMacro(InputIntComparableCheck,
    (Concept::Comparable<InputImagePixelType, int>));
  /** End concept checking */
#endif

protected:
  BinaryThinningImageFilter3D();
  virtual ~BinaryThinningImageFilter3D() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Compute thinning Image. */
  void GenerateData();

  /** Prepare data. */
  void PrepareData();

  /**  Compute thinning Image. */
  void ComputeThinImage();
  
  /**  isEulerInvariant [Lee94] */
  bool isEulerInvariant(NeighborhoodType neighbors, int *LUT);
  void fillEulerLUT(int *LUT);  
  /**  isSimplePoint [Lee94] */
  bool isSimplePoint(NeighborhoodType neighbors);
  /**  Octree_labeling [Lee94] */
  void Octree_labeling(int octant, int label, int *cube);


private:   
  BinaryThinningImageFilter3D(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

}; // end of BinaryThinningImageFilter3D class

} //end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryThinningImageFilter3D.txx"
#endif

#endif

