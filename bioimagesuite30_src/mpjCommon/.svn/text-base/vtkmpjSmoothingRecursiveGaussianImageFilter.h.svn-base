

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
  Module:    $RCSfile: itkSmoothingRecursiveGaussianImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2004/02/02 21:42:22 $
  Version:   $Revision: 1.8 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __vtkmpjSmoothingRecursiveGaussianImageFilter_h
#define __vtkmpjSmoothingRecursiveGaussianImageFilter_h

#include "vtkmpjRecursiveGaussianImageFilter.h"

/** \class SmoothingRecursiveGaussianImageFilter
 * \brief Computes the smoothing of an image by convolution
 *        with the Gaussian kernels implemented as IIR filters.
 * 
 * This filter is implemented using the recursive gaussian
 * filters
 *
 * 
 * \ingroup IntensityImageFilters
 * \ingroup Singlethreaded
 */
class vtkmpjSmoothingRecursiveGaussianImageFilter: public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjSmoothingRecursiveGaussianImageFilter *New();
  vtkTypeMacro(vtkmpjSmoothingRecursiveGaussianImageFilter,vtkpxSimpleImageToImageFilter);
  
  /** Set Sigma value. Sigma is measured in the units of image spacing.  */
  void SetSigma( float sigma );
  
  /** Define which normalization factor will be used for the Gaussian */
  void SetNormalizeAcrossScale( int normalizeInScaleSpace );
  vtkGetMacro( NormalizeAcrossScale, int );

 protected:
  vtkmpjSmoothingRecursiveGaussianImageFilter();
  virtual ~vtkmpjSmoothingRecursiveGaussianImageFilter();

  vtkmpjSmoothingRecursiveGaussianImageFilter(const vtkmpjSmoothingRecursiveGaussianImageFilter&) {};
  void operator=(const vtkmpjSmoothingRecursiveGaussianImageFilter&) {};

  virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);

 private:
  
  vtkmpjRecursiveGaussianImageFilter *m_FirstSmoothingFilter;
  vtkmpjRecursiveGaussianImageFilter *m_SmoothingFilters[2];

  /** Normalize the image across scale space */
  int NormalizeAcrossScale; 

};

#endif
