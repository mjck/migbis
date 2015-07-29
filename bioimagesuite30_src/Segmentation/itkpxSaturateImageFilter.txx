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




#ifndef _itkSaturateImageFilter_txx
#define _itkSaturateImageFilter_txx
#include "itkpxSaturateImageFilter.h"

#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"

namespace itk {

template<class TInputImage, class TOutputImage>
SaturateImageFilter<TInputImage, TOutputImage>
::SaturateImageFilter()
{
  m_LowerBound= NumericTraits<OutputImagePixelType>::NonpositiveMin();
  m_UpperBound= NumericTraits<OutputImagePixelType>::max();
  m_Scale = NumericTraits<RealType>::One;
  m_ThreadUnderflow.SetSize(1);
  m_ThreadOverflow.SetSize(1);
}

template<class TInputImage, class TOutputImage>
SaturateImageFilter<TInputImage, TOutputImage>
::~SaturateImageFilter()
{
}

template<class TInputImage, class TOutputImage>
void
SaturateImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData ()
{
  int numberOfThreads = this->GetNumberOfThreads();

  //  Allocate and initialize the thread temporaries
  m_ThreadUnderflow.SetSize(numberOfThreads);
  m_ThreadUnderflow.Fill(0);
  m_ThreadOverflow.SetSize(numberOfThreads);
  m_ThreadOverflow.Fill(0);
}

template<class TInputImage, class TOutputImage>
void
SaturateImageFilter<TInputImage, TOutputImage>
::AfterThreadedGenerateData ()
{
  int numberOfThreads = this->GetNumberOfThreads();

  m_UnderflowCount = 0;
  m_OverflowCount = 0;

  // Accumulate counts for each thread
  for( int i = 0; i < numberOfThreads; i++)
    {
    m_UnderflowCount += m_ThreadUnderflow[i];
    m_OverflowCount += m_ThreadOverflow[i];
    }
}

template<class TInputImage, class TOutputImage>
void
SaturateImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread,
                       int threadId) 
{
  //  std::cerr << "Updating Saturate .....";
  RealType value;
  ImageRegionConstIterator<TInputImage>  it (this->GetInput(), outputRegionForThread);
  ImageRegionIterator<TOutputImage> ot (this->GetOutput(), outputRegionForThread);
  
  // support progress methods/callbacks
  ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());
          
  // shift and scale the input pixels
  while (!it.IsAtEnd())
    {
      value = (static_cast<RealType>(it.Get()));
      if (value<m_LowerBound)
	{
	  value=(m_LowerBound*m_Scale);
	}
      else if (value > m_UpperBound)
	{
	  value=(m_UpperBound*m_Scale);
	}
      else
	{
	  value=(value*m_Scale);
	}
	       
    if (value < NumericTraits<OutputImagePixelType>::NonpositiveMin())
      {
      ot.Set (NumericTraits<OutputImagePixelType>::NonpositiveMin());
      m_ThreadUnderflow[threadId]++;
      }
    else if (value > NumericTraits<OutputImagePixelType>::max())
      {
      ot.Set (NumericTraits<OutputImagePixelType>::max());      
      m_ThreadOverflow[threadId]++;
      }
    else
      {
	ot.Set( static_cast<OutputImagePixelType>( value ) );
      }
    ++it;
    ++ot;
    
    progress.CompletedPixel();
    }
  //std::cerr << "Updating Saturate Done " << std::endl;
}

template <class TInputImage, class TOutputImage>
void 
SaturateImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Bounds: "  << m_LowerBound << ": " << m_UpperBound << std::endl;
  os << indent << "Scale: "  << m_Scale << std::endl;
  os << indent << "Computed values follow:" << std::endl;
  os << indent << "UnderflowCount: "  << m_UnderflowCount << std::endl;
  os << indent << "OverflowCount: "  << m_OverflowCount << std::endl;
}


}// end namespace itk
#endif

