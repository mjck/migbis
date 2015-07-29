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

#ifndef __WOOK_H__
#define  __WOOK_H__

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"

///////////////////////////////   for mythreshold.cxx     ////////////////////////////////////////
#include "itkBinaryMorphologicalClosingImageFilter.h"
#include "itkBinaryBallStructuringElement.h" 
#include "itkBinaryThresholdImageFilter.h"
#include "itkSignedDanielssonDistanceMapImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"

/////////////////////////   for DeformableRegistration.cxx /////////////////////////////////////////    
#include "itkImageRegionIterator.h"
#include "itkjwsDemonsRegistrationFilter.h"
#include "itkjwsSymmetricForcesDemonsRegistrationFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkjwsMultiResolutionPDEDeformableRegistration.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkCastImageFilter.h"
#include "itkWarpImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkExceptionObject.h"
#include "itkCastImageFilter.h"

#include <math.h>
#include <stdio.h>

///////////////////////////  for Vector3DImage.cxx  //////////////////////////////////
#include "itkCompose3DVectorImageFilter.h" 
#include "itkCompose2DVectorImageFilter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkDiscreteGaussianImageFilter.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////
  using namespace std;

namespace jws_demons {

  typedef  unsigned short  InputPixelType;
  typedef  unsigned short  OutputPixelType;
  typedef  float		   FloatPixelType;
  
  typedef  short  GrayPixelType;
  
  const unsigned int Dimension = 3;
  
  typedef itk::Image< InputPixelType,  Dimension >   InputImageType;
  typedef itk::Image< OutputPixelType, Dimension >   OutputImageType;
  typedef itk::Image< FloatPixelType, Dimension > FloatImageType;
  typedef itk::Image< GrayPixelType,  Dimension >   GrayImageType;
  

  typedef itk::ImageFileReader< InputImageType  >  ReaderType;
  typedef itk::ImageFileWriter< OutputImageType >  WriterType;
  typedef itk::ImageFileReader< FloatImageType  >  FloatReaderType;
  typedef itk::ImageFileWriter< FloatImageType >   FloatWriterType;
  
  typedef itk::ImageFileReader< GrayImageType  >  GrayReaderType;
  typedef itk::ImageFileWriter< GrayImageType >   GrayWriterType;
  
  typedef itk::ImageRegionIteratorWithIndex<  InputImageType >  IteratorType;
  typedef itk::ImageRegionIteratorWithIndex< OutputImageType >  OutImageIteratorType;
  
  typedef itk::ImageRegionIteratorWithIndex<  GrayImageType >  IteratorGrayType;
  typedef itk::ImageRegionIteratorWithIndex< GrayImageType >  OutImageIteratorGrayType;  
  
  typedef itk::BinaryThresholdImageFilter< InputImageType, InputImageType >  ThresholdFilterType;
   
  typedef itk::BinaryBallStructuringElement< 
                      InputPixelType,
                      Dimension  >             StructuringElementType;

  typedef itk::BinaryMorphologicalClosingImageFilter<
    InputImageType, 
                            OutputImageType, 
                            StructuringElementType >  ClosingFilterType; 

  typedef itk::SignedDanielssonDistanceMapImageFilter<
                                         InputImageType, 
                                         FloatImageType >  DMapFilterType; 

/////////////////////////   for DeformableRegistration.cxx ///////////////////////////////////////// 

  typedef float RegPixelType;

  typedef itk::Image< RegPixelType, Dimension >  FixedImageType;
  typedef itk::Image< RegPixelType, Dimension >  MovingImageType;
  typedef itk::Image< RegPixelType, Dimension >  InternalImageType;
  
  typedef itk::Vector< RegPixelType, Dimension >    VectorPixelType;
  typedef itk::Image<  VectorPixelType, Dimension > DeformationFieldType;
  
  typedef itk::ImageFileReader< DeformationFieldType  >  DFieldReaderType;
  
  typedef itk::ImageFileWriter< DeformationFieldType > FieldWriterType;

  typedef itk::ImageRegionIteratorWithIndex< InternalImageType >  InternalIteratorType;
  typedef itk::ImageRegionIteratorWithIndex< DeformationFieldType >  DfieldIteratorType;
  
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  typedef itk::WarpImageFilter<                                               
                          MovingImageType, 
                          MovingImageType,
                          DeformationFieldType  >     WarperType;
  
    typedef itk::WarpImageFilter< GrayImageType, 
                               GrayImageType, 
                                DeformationFieldType  >  GrayWarperType;
  
    typedef itk::DiscreteGaussianImageFilter<
                 InternalImageType, InternalImageType >  DiscreteGaussianFilterType;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

};
#endif

