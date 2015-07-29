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
  Module:    $RCSfile: itkjwsMultiResolutionPDEDeformableRegistration.h,v $
  Language:  C++
  Date:      $Date: 2007/07/31 23:12:20 $
  Version:   $Revision: 1.29 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkjwsMultiResolutionPDEDeformableRegistration_h
#define __itkjwsMultiResolutionPDEDeformableRegistration_h

#include "itkImage.h"
#include "itkImageToImageFilter.h"
#include "itkjwsPDEDeformableRegistrationFilter.h"
#include "itkjwsDemonsRegistrationFilter.h"
#include "itkMultiResolutionPyramidImageFilter.h"
#include "itkVectorResampleImageFilter.h"

#include <vector>

namespace itk
{
/**
 * \class jwsMultiResolutionPDEDeformableRegistration
 * \brief Framework for perfoming multi-resolution PDE deformable registration.
 *
 * jwsMultiResolutionPDEDeformableRegistration provides a generic framework
 * to peform multi-resolution deformable registration.
 *
 * At each resolution level a PDEDeformableRegistrationFilter is used
 * to register two images by computing the deformation field which will 
 * map a moving image onto a fixed image.
 *
 * A deformation field is represented as an image whose pixel type is some
 * vector type with at least N elements, where N is the dimension of
 * the fixed image. The vector type must support element access via operator
 * []. It is assumed that the vector elements behave like floating point
 * scalars.
 *
 * The internal PDEDeformationRegistrationFilter can be set using
 * SetRegistrationFilter. By default a DemonsRegistrationFilter is used.
 *
 * The input fixed and moving images are set via methods SetFixedImage
 * and SetMovingImage respectively. An initial deformation field maybe set via
 * SetInitialDeformationField or SetInput. If no initial field is set
 * a zero field is used as the initial condition.
 *
 * MultiResolutionPyramidImageFilters are used to downsample the fixed
 * and moving images. A VectorExpandImageFilter is used to upsample
 * the deformation as we move from a coarse to fine solution.
 *
 * This class is templated over the fixed image type, the moving image type,
 * and the Deformation Field type.
 *
 * \warning This class assumes that the fixed, moving and deformation
 * field image types all have the same number of dimensions.
 *
 * \sa PDEDeformableRegistrationFilter
 * \sa DemonsRegistrationFilter
 * \sa MultiResolutionPyramidImageFilter
 * \sa VectorExpandImageFilter
 *
 * The current implementation of this class does not support streaming.
 *
 * \ingroup DeformableImageRegistration
 */
template <class TFixedImage, class TMovingImage, class TDeformationField>
class ITK_EXPORT jwsMultiResolutionPDEDeformableRegistration :
    public ImageToImageFilter <TDeformationField, TDeformationField>
{
public:
  /** Standard class typedefs */
  typedef jwsMultiResolutionPDEDeformableRegistration Self;
  typedef ImageToImageFilter<TDeformationField, TDeformationField>
  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro( jwsMultiResolutionPDEDeformableRegistration, 
                ImageToImageFilter );

  /** Fixed image type. */
  typedef TFixedImage FixedImageType;
  typedef typename FixedImageType::Pointer FixedImagePointer;
  typedef typename FixedImageType::ConstPointer FixedImageConstPointer;

  /** Moving image type. */
  typedef TMovingImage MovingImageType;
  typedef typename MovingImageType::Pointer MovingImagePointer;
  typedef typename MovingImageType::ConstPointer MovingImageConstPointer;

  /** Deformation field image type. */
  typedef TDeformationField DeformationFieldType;
  typedef typename DeformationFieldType::Pointer DeformationFieldPointer;

  /** ImageDimension. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      FixedImageType::ImageDimension);

  /** Internal float image type. */
  typedef Image<float,itkGetStaticConstMacro(ImageDimension)> FloatImageType;

  /** The internal registration type. */
  typedef jwsPDEDeformableRegistrationFilter<
    FloatImageType, FloatImageType, DeformationFieldType > RegistrationType;
  typedef typename RegistrationType::Pointer RegistrationPointer;

  /** The default registration type. */
  typedef jwsDemonsRegistrationFilter<
    FloatImageType, FloatImageType, DeformationFieldType > DefaultRegistrationType;

  /** The fixed multi-resolution image pyramid type. */
  typedef MultiResolutionPyramidImageFilter<
    FixedImageType, FloatImageType > FixedImagePyramidType;
  typedef typename FixedImagePyramidType::Pointer FixedImagePyramidPointer;

  /** The moving multi-resolution image pyramid type. */
  typedef MultiResolutionPyramidImageFilter<
    MovingImageType, FloatImageType > MovingImagePyramidType;
  typedef typename MovingImagePyramidType::Pointer MovingImagePyramidPointer;
   
  /** The deformation field expander type. */
  typedef VectorResampleImageFilter<
    DeformationFieldType, DeformationFieldType > FieldExpanderType;
  typedef typename FieldExpanderType::Pointer  FieldExpanderPointer;

  /** Set the fixed image. */
  virtual void SetFixedImage( const FixedImageType * ptr );

  /** Get the fixed image. */
  const FixedImageType * GetFixedImage(void) const;

  /** Set the moving image. */
  virtual void SetMovingImage( const MovingImageType * ptr );

  /** Get the moving image. */
  const MovingImageType * GetMovingImage(void) const;

  /** Set initial deformation field. */
  virtual void SetInitialDeformationField( DeformationFieldType * ptr )
  {
    this->m_InitialDeformationField=ptr;
    // itkExceptionMacro( << "This feature not implemented yet"  );
  }

  /** Get output deformation field. */
  const DeformationFieldType * GetDeformationField(void)
  { return this->GetOutput(); }

  /** Get the number of valid inputs.  For
   * jwsMultiResolutionPDEDeformableRegistration, this checks whether the
   * fixed and moving images have been set. While
   * jwsMultiResolutionPDEDeformableRegistration can take a third input
   * as an initial deformation field, this input is not a required input.
   */
  virtual std::vector<SmartPointer<DataObject> >::size_type GetNumberOfValidRequiredInputs() const;

  /** Set the internal registrator. */
  itkSetObjectMacro( RegistrationFilter, RegistrationType );

  /** Get the internal registrator. */
  itkGetObjectMacro( RegistrationFilter, RegistrationType );
  
  /** Set the fixed image pyramid. */
  itkSetObjectMacro( FixedImagePyramid, FixedImagePyramidType );

  /** Get the fixed image pyramid. */
  itkGetObjectMacro( FixedImagePyramid, FixedImagePyramidType );

  /** Set the moving image pyramid. */
  itkSetObjectMacro( MovingImagePyramid, MovingImagePyramidType );

  /** Get the moving image pyramid. */
  itkGetObjectMacro( MovingImagePyramid, MovingImagePyramidType );

  /** Set number of multi-resolution levels. */
  virtual void SetNumberOfLevels( unsigned int num );

  /** Get number of multi-resolution levels. */
  itkGetConstReferenceMacro( NumberOfLevels, unsigned int );

  /** Get the current resolution level being processed. */
  itkGetConstReferenceMacro( CurrentLevel, unsigned int );

  /** Set number of iterations per multi-resolution levels. */
  itkSetVectorMacro( NumberOfIterations, unsigned int, m_NumberOfLevels );

  /** Get number of iterations per multi-resolution levels. */
  virtual const unsigned int * GetNumberOfIterations() const
  { return &(m_NumberOfIterations[0]); }

  /** Stop the registration after the current iteration. */
  virtual void StopRegistration();

protected:
  jwsMultiResolutionPDEDeformableRegistration();
  ~jwsMultiResolutionPDEDeformableRegistration() {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Generate output data by performing the registration
   * at each resolution level. */
  virtual void GenerateData();

  /** The current implementation of this class does not support
   * streaming. As such it requires the largest possible region
   * for the moving, fixed and input deformation field. */
  virtual void GenerateInputRequestedRegion();

  /** By default, the output deformation field has the same
   * spacing, origin and LargestPossibleRegion as the input/initial
   * deformation field.
   *
   * If the initial deformation field is not set, the output
   * information is copied from the fixed image. */
  virtual void GenerateOutputInformation();

  /** The current implementation of this class does not supprot
   * streaming. As such it produces the output for the largest
   * possible region. */
  virtual void EnlargeOutputRequestedRegion( DataObject *ptr );

  /** This method returns true to indicate that the registration should
   * terminate at the current resolution level. */
  virtual bool Halt();

private:
  jwsMultiResolutionPDEDeformableRegistration(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  RegistrationPointer        m_RegistrationFilter;
  FixedImagePyramidPointer   m_FixedImagePyramid;
  MovingImagePyramidPointer  m_MovingImagePyramid;
  FieldExpanderPointer       m_FieldExpander;
  DeformationFieldPointer    m_InitialDeformationField;

  unsigned int               m_NumberOfLevels;
  unsigned int               m_CurrentLevel;
  std::vector<unsigned int>  m_NumberOfIterations;

  /** Flag to indicate user stop registration request. */
  bool                      m_StopRegistrationFlag;

};


} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkjwsMultiResolutionPDEDeformableRegistration.txx"
#endif


#endif

