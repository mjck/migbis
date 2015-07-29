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
  Module:    $RCSfile: itkjwsDenseFiniteDifferenceImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/10/27 14:52:31 $
  Version:   $Revision: 1.29 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkjwsDenseFiniteDifferenceImageFilter_txx_
#define __itkjwsDenseFiniteDifferenceImageFilter_txx_
#include "itkjwsDenseFiniteDifferenceImageFilter.h"

#include <list>
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "itkNeighborhoodAlgorithm.h"
#include <stdio.h>

namespace itk {

template <class TInputImage, class TOutputImage>
void
jwsDenseFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::CopyInputToOutput()
{
  typename TInputImage::ConstPointer  input  = this->GetInput();
  typename TOutputImage::Pointer      output = this->GetOutput();

  if ( !input || !output )
    {
    itkExceptionMacro(<< "Either input and/or output is NULL.");
    }

  // Check if we are doing in-place filtering
  if ( this->GetInPlace() && (typeid(TInputImage) == typeid(TOutputImage)) )
    {
    typename TInputImage::Pointer tempPtr = 
      dynamic_cast<TInputImage *>( output.GetPointer() );
    if ( tempPtr && tempPtr->GetPixelContainer() == input->GetPixelContainer() )
      {
      // the input and output container are the same - no need to copy
      return;
      }
    }
  
  ImageRegionConstIterator<TInputImage>  in(input, output->GetRequestedRegion());
  ImageRegionIterator<TOutputImage> out(output, output->GetRequestedRegion());

  while( ! out.IsAtEnd() )
    {
    out.Value() =  static_cast<PixelType>(in.Get());  // Supports input image adaptors only
    ++in;
    ++out;
    }
}

template <class TInputImage, class TOutputImage>
void
jwsDenseFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::AllocateUpdateBuffer()
{
  // The update buffer looks just like the output.
  typename TOutputImage::Pointer output = this->GetOutput();

  m_UpdateBuffer->SetSpacing(output->GetSpacing());
  m_UpdateBuffer->SetOrigin(output->GetOrigin());
  m_UpdateBuffer->SetLargestPossibleRegion(output->GetLargestPossibleRegion());
  m_UpdateBuffer->SetRequestedRegion(output->GetRequestedRegion());
  m_UpdateBuffer->SetBufferedRegion(output->GetBufferedRegion());
  m_UpdateBuffer->Allocate();
}

template<class TInputImage, class TOutputImage>
void
jwsDenseFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::ApplyUpdate(TimeStepType dt)
{
  // Set up for multithreaded processing.
  DenseFDThreadStruct str;
  str.Filter = this;
  str.TimeStep = dt;
  this->GetMultiThreader()->SetNumberOfThreads(this->GetNumberOfThreads());
  this->GetMultiThreader()->SetSingleMethod(this->ApplyUpdateThreaderCallback,
                                            &str);
  // Multithread the execution
  this->GetMultiThreader()->SingleMethodExecute();
}

template<class TInputImage, class TOutputImage>
ITK_THREAD_RETURN_TYPE
jwsDenseFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::ApplyUpdateThreaderCallback( void * arg )
{
  DenseFDThreadStruct * str;
  int total, threadId, threadCount;

  threadId = ((MultiThreader::ThreadInfoStruct *)(arg))->ThreadID;
  threadCount = ((MultiThreader::ThreadInfoStruct *)(arg))->NumberOfThreads;

  str = (DenseFDThreadStruct *)(((MultiThreader::ThreadInfoStruct *)(arg))->UserData);

  // Execute the actual method with appropriate output region
  // first find out how many pieces extent can be split into.
  // Using the SplitRequestedRegion method from itk::ImageSource.
  ThreadRegionType splitRegion;
  total = str->Filter->SplitRequestedRegion(threadId, threadCount,
                                            splitRegion);
  
  if (threadId < total)
    {
    str->Filter->ThreadedApplyUpdate(str->TimeStep, splitRegion, threadId);
    }

  return ITK_THREAD_RETURN_VALUE;
}

template <class TInputImage, class TOutputImage>
typename
jwsDenseFiniteDifferenceImageFilter<TInputImage, TOutputImage>::TimeStepType
jwsDenseFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::CalculateChange()
{
  int threadCount;
  TimeStepType dt;

  // Set up for multithreaded processing.
  DenseFDThreadStruct str;
  str.Filter = this;
  str.TimeStep = NumericTraits<TimeStepType>::Zero;  // Not used during the
  // calculate change step.
  this->GetMultiThreader()->SetNumberOfThreads(this->GetNumberOfThreads());
  this->GetMultiThreader()->SetSingleMethod(this->CalculateChangeThreaderCallback,
                                            &str);

  // Initialize the list of time step values that will be generated by the
  // various threads.  There is one distinct slot for each possible thread,
  // so this data structure is thread-safe.
  threadCount = this->GetMultiThreader()->GetNumberOfThreads();  
  str.TimeStepList = new TimeStepType[threadCount];                 
  str.ValidTimeStepList = new bool[threadCount];
  for (int i =0; i < threadCount; ++i)
    {      str.ValidTimeStepList[i] = false;    } 

  // Multithread the execution
  this->GetMultiThreader()->SingleMethodExecute();

  // Resolve the single value time step to return
  dt = this->ResolveTimeStep(str.TimeStepList, str.ValidTimeStepList, threadCount);
  delete [] str.TimeStepList;
  delete [] str.ValidTimeStepList;

  return  dt;
}

template <class TInputImage, class TOutputImage>
ITK_THREAD_RETURN_TYPE
jwsDenseFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::CalculateChangeThreaderCallback( void * arg )
{
  DenseFDThreadStruct * str;
  int total, threadId, threadCount;

  threadId = ((MultiThreader::ThreadInfoStruct *)(arg))->ThreadID;
  threadCount = ((MultiThreader::ThreadInfoStruct *)(arg))->NumberOfThreads;

  str = (DenseFDThreadStruct *)(((MultiThreader::ThreadInfoStruct *)(arg))->UserData);

  // Execute the actual method with appropriate output region
  // first find out how many pieces extent can be split into.
  // Using the SplitRequestedRegion method from itk::ImageSource.
  ThreadRegionType splitRegion;

  total = str->Filter->SplitRequestedRegion(threadId, threadCount,
                                            splitRegion);

  if (threadId < total)
    { 
    str->TimeStepList[threadId]
      = str->Filter->ThreadedCalculateChange(splitRegion, threadId);
    str->ValidTimeStepList[threadId] = true;
    }

  return ITK_THREAD_RETURN_VALUE;  
}

template <class TInputImage, class TOutputImage>
void
jwsDenseFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::ThreadedApplyUpdate(TimeStepType dt, const ThreadRegionType &regionToProcess,
                      int)
{
  ImageRegionIterator<UpdateBufferType> u(m_UpdateBuffer,    regionToProcess);
  ImageRegionIterator<OutputImageType>  o(this->GetOutput(), regionToProcess);

  u = u.Begin();
  o = o.Begin();

  while ( !u.IsAtEnd() )
    {
    o.Value() += static_cast<PixelType>(u.Value() * dt);  // no adaptor support here
    ++o;
    ++u;
    }
}

template <class TInputImage, class TOutputImage>
typename
jwsDenseFiniteDifferenceImageFilter<TInputImage, TOutputImage>::TimeStepType
jwsDenseFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::ThreadedCalculateChange(const ThreadRegionType &regionToProcess, int)
{

 //fprintf(stderr,"## jwsDenseFiniteDifference ###");

  typedef typename OutputImageType::RegionType RegionType;
  typedef typename OutputImageType::SizeType   SizeType;
  typedef typename OutputImageType::SizeValueType   SizeValueType;
  typedef typename OutputImageType::IndexType  IndexType;
  typedef typename OutputImageType::IndexValueType  IndexValueType;
  typedef typename FiniteDifferenceFunctionType::NeighborhoodType
    NeighborhoodIteratorType;
  typedef ImageRegionIterator<UpdateBufferType> UpdateIteratorType;

  typename OutputImageType::Pointer output = this->GetOutput();
  TimeStepType timeStep;
  void *globalData;

  // Get the FiniteDifferenceFunction to use in calculations.
  const typename FiniteDifferenceFunctionType::Pointer df
    = this->GetDifferenceFunction();
  const SizeType  radius = df->GetRadius();
  
  // Break the input into a series of regions.  The first region is free
  // of boundary conditions, the rest with boundary conditions.  We operate
  // on the output region because input has been copied to output.
  typedef NeighborhoodAlgorithm::ImageBoundaryFacesCalculator<OutputImageType>
    FaceCalculatorType;
  typedef typename FaceCalculatorType::FaceListType FaceListType;

  FaceCalculatorType faceCalculator;
    
  FaceListType faceList = faceCalculator(output, regionToProcess, radius);
  typename FaceListType::iterator fIt = faceList.begin();

  // Ask the function object for a pointer to a data structure it
  // will use to manage any global values it needs.  We'll pass this
  // back to the function object at each calculation and then
  // again so that the function object can use it to determine a
  // time step for this iteration.
  globalData = df->GetGlobalDataPointer();

  // Process the non-boundary region.
  NeighborhoodIteratorType nD(radius, output, *fIt);
  UpdateIteratorType       nU(m_UpdateBuffer,  *fIt);
  nD.GoToBegin();
  while( !nD.IsAtEnd() )
    {       //fprintf(stderr," nnnnnnU::df->ComputeUpdate() at jwsDenseFiniteDifference~.txx\n");
    nU.Value() = df->ComputeUpdate(nD, globalData);
    ++nD;
    ++nU;
    }

  // Process each of the boundary faces.

  NeighborhoodIteratorType bD;
  UpdateIteratorType   bU;
  for (++fIt; fIt != faceList.end(); ++fIt)
    {
    bD = NeighborhoodIteratorType(radius, output, *fIt);
    bU = UpdateIteratorType  (m_UpdateBuffer, *fIt);
     
    bD.GoToBegin();
    bU.GoToBegin();
    while ( !bD.IsAtEnd() )
      {     fprintf(stderr," bbbbbbU::df->ComputeUpdate() at jwsDenseFiniteDifference~.txx\n");
      bU.Value() = df->ComputeUpdate(bD, globalData);
      ++bD;
      ++bU;
      }
    }

  // Ask the finite difference function to compute the time step for
  // this iteration.  We give it the global data pointer to use, then
  // ask it to free the global data memory.
  timeStep = df->ComputeGlobalTimeStep(globalData);
  df->ReleaseGlobalDataPointer(globalData);

  return timeStep;
}

template <class TInputImage, class TOutputImage>
void
jwsDenseFiniteDifferenceImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  
}

}// end namespace itk

#endif


