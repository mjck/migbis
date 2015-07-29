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
  Module:    $RCSfile: itkpxTissueSegmentationLevelSetImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:39 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkpxTissueSegmentationLevelSetImageFilter_txx_
#define __itkpxTissueSegmentationLevelSetImageFilter_txx_

#include "itkpxTissueSegmentationLevelSetImageFilter.h"
#include <math.h>
#include "pxisinf.h"

namespace itk {

template <class TInputImage, class TFeatureImage, class TOutputType>
TissueSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
::TissueSegmentationLevelSetImageFilter()
{
  m_TissueFunction = TissueFunctionType::New();
  m_DynamicUpdateParameters=1;
  this->SetSegmentationFunction(m_TissueFunction);
}
 
template <class TInputImage, class TFeatureImage, class TOutputType>
void TissueSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << "TissueFunction: " << m_TissueFunction;
}


template <class TInputImage, class TFeatureImage, class TOutputType>
void TissueSegmentationLevelSetImageFilter<TInputImage, TFeatureImage, TOutputType>
::InitializeIteration()
{

  if (this->m_DynamicUpdateParameters==0 && m_TissueFunction->GetApproximateVolumeConstraintWeight()<0.0001)
    return;

  double epsilon=m_TissueFunction->GetHeavySideEpsilon();

  typename Superclass::LayerType::ConstIterator layerIt;
  const typename Superclass::FiniteDifferenceFunctionType::Pointer df  = this->GetDifferenceFunction();
  NeighborhoodIterator<OutputImageType> outputIt(df->GetRadius(), this->GetOutput(), this->GetOutput()->GetRequestedRegion());
  NeighborhoodIterator<FeatureImageType> featureIt(df->GetRadius(), this->GetFeatureImage(), this->GetFeatureImage()->GetRequestedRegion());


  double volume=0.0;
  
  double sum[2],sum2[2],counter[2];
  for (int i=0;i<=1;i++)
    {
      counter[i]=0.0;
      sum[i]=0.0;
      sum2[i]=0.0;
    }
  
  for ( unsigned int k = 0; k < this->GetNumberOfLayers(); k++ )
    {
      for (layerIt = this->m_Layers[k]->Begin(); layerIt != this->m_Layers[k]->End(); ++layerIt)
	{
	  featureIt.SetLocation(layerIt->m_Value);
	  outputIt.SetLocation(layerIt->m_Value);
	  ValueType phi=outputIt.GetCenterPixel();

	  if (phi<0.0)
	    volume+=1.0;
	  
	  ValueType v=featureIt.GetCenterPixel();


	  for (int region=0;region<=1;region++)
	    {
	      double m=(1.0+0.63662*atan(phi/epsilon));
	      if (region==1)
		m=2.0-m;
	      //	      std::cerr << "Layer = " << k << ", region = " << region << ", m=" << m << " (" << phi << ") location=" << layerIt->m_Value[0] << " " << layerIt->m_Value[1] << ", value = " << v << "\n";

	      
	      sum[region]+=v*m;
	      sum2[region]+=v*v*m;
	      counter[region]+=m;
	    }
	}
    }

  if (this->m_DynamicUpdateParameters>0)
    {
      double mean[2],sigma[2];
      std::cerr << "Chan Vese Update:: Region Properties ( " << epsilon << "\t";
      for (int ia=0;ia<=1;ia++)
	{
	  if (counter[ia]>0)
	    mean[ia]=sum[ia]/(counter[ia]);
	  else
	    mean[ia]=0.0;
	  
	  if (counter[ia]>1)
	    sigma[ia]=sqrt(sum2[ia]/(counter[ia])-mean[ia]*mean[ia]);
	  else
	    sigma[ia]=1.0;
	  
	  
	  if (isnan(sigma[ia]))	sigma[ia]=1.0;
	  if (isnan(mean[ia]))	mean[ia]=0.0;
	  
	  
	  std::cerr << 
	    "ia=" << ia << 
	    " mean=" << mean[ia] << 
	    " + " << sigma[ia] << "( " << 
	    sum[ia] << "," << 
	    sum2[ia] << "," << 
	    counter[ia] << ")\t";
	}
      this->SetInsideTissue(mean[1],sigma[1]);
      this->SetOutsideTissue(mean[0],sigma[0]);
    }

  //  if(m_TissueFunction->GetApproximateVolumeConstraintWeight()>0.0) 
  //    {
  std::cerr << "Volume = " << volume << "\n";
  m_TissueFunction->SetMyVolume(volume);
      //    }

  std::cerr << "\n";
}


}// end namespace itk




#endif

