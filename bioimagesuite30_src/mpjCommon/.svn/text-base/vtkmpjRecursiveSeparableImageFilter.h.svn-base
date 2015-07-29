

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
  Module:    $RCSfile: itkRecursiveSeparableImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2004/02/04 16:20:08 $
  Version:   $Revision: 1.20 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __vtkmpjRecursiveSeparableImageFilter_h
#define __vtkmpjRecursiveSeparableImageFilter_h

#include "vtkpxSimpleImageToImageFilter.h"
#include "vtkFloatArray.h"

/** \class RecursiveSeparableImageFilter
 * \brief Base class for recursive convolution with a kernel.
 *
 * RecursiveSeparableImageFilter is the base class for recursive 
 * filters that are applied in each dimension separatedly.
 * 
 * This class implements the recursive filtering
 * method proposed by R.Deriche in IEEE-PAMI
 * Vol.12, No.1, January 1990, pp 78-87.
 * 
 * \ingroup ImageFilters
 */

typedef float RealType;

class vtkmpjRecursiveSeparableImageFilter : public vtkpxSimpleImageToImageFilter
{
 public:
  static vtkmpjRecursiveSeparableImageFilter *New();
  vtkTypeMacro(vtkmpjRecursiveSeparableImageFilter,vtkpxSimpleImageToImageFilter);

  /** Get the direction in which the filter is to be applied. */   
  vtkGetMacro(Direction, unsigned int);
  
  /** Set the direction in which the filter is to be applied. */   
  vtkSetMacro(Direction, unsigned int);
  
 private:

  /** Direction in which the filter is to be applied
   * this shoul in the range [0,ImageDimension-1] */ 
  unsigned int Direction;
  
 protected:
  
  //BTX
  vtkmpjRecursiveSeparableImageFilter();
  virtual ~vtkmpjRecursiveSeparableImageFilter();
  vtkmpjRecursiveSeparableImageFilter(const vtkmpjRecursiveSeparableImageFilter&) {};
  void operator=(const vtkmpjRecursiveSeparableImageFilter&) {};
  //ETX
  
  /** Set up the coefficients of the filter to approximate a specific kernel.
   * typically it can be used to approximate a gaussian or one of its
   * derivatives. Parameter is the spacing along the dimension to
   * filter. */
  virtual void SetUp(RealType spacing) {};
  
  /** Compute Recursive Filter Coefficients this method prepares the values of
   * the coefficients used for filtering the image. The symmetric flag is
   * used to enforce that the filter will be symmetric or antisymmetric. For
   * example, the Gaussian kernel is symmetric, while its first derivative is
   * antisymmetric. The spacing parameter is the spacing of the pixels
   * along the dimension to be filtered. */
  virtual void ComputeFilterCoefficients(bool symmetric, RealType spacing) {};
  
  /** Apply the Recursive Filter to an array of data.  This method is called
   * for each line of the volume. Parameter "scratch" is a scratch
   * area used for internal computations that is the same size as the
   * parameters "outs" and "data". The scratch area must be allocated
   * outside of this routine (this avoids memory allocation and
   * deallocation in the inner loop of the overall algorithm. */
  void FilterDataArray(RealType *outs, const RealType *data, RealType *scratch,
                       unsigned int ln);
  
  virtual void SimpleExecute(vtkImageData* input, vtkImageData* output);
  
 protected:

  /**  Normalization factor. */
  RealType m_K;                       

  /**  Parameter of exponential series. */
  RealType m_A0;
  RealType m_A1;
  RealType m_B0;
  RealType m_B1;
  RealType m_C0;
  RealType m_C1;
  RealType m_W0;
  RealType m_W1; 

  /** Causal coefficients that multiply the input data. */
  RealType m_N00;
  RealType m_N11;
  RealType m_N22;
  RealType m_N33; 
  
  /** Recursive coefficients that multiply previously computed values at the output.
      In this case the Causal coefficients == Anticausal coefficients. */
  RealType m_D11;
  RealType m_D22;
  RealType m_D33;
  RealType m_D44; 
  
  /** Anti-Causal coefficients (symmetric case). that multiply the input data */
  RealType m_M11;
  RealType m_M22;
  RealType m_M33;
  RealType m_M44; 

  /** Recursive coefficients to be used at the boundaries to prevent border effects */
  RealType m_BN1;
  RealType m_BN2;
  RealType m_BN3;
  RealType m_BN4; 
 
  RealType m_BM1;
  RealType m_BM2;
  RealType m_BM3;
  RealType m_BM4; 

};

#endif
