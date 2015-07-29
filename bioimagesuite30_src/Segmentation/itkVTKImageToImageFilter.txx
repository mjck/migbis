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
  Module:    $RCSfile: itkVTKImageToImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/06/12 01:23:44 $
  Version:   $Revision: 1.4 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkVTKImageToImageFilter_txx
#define _itkVTKImageToImageFilter_txx

#include "itkVTKImageToImageFilter.h"

namespace itk
{



/**
 * Constructor
 */
template <class TOutputImage>
VTKImageToImageFilter<TOutputImage>
::VTKImageToImageFilter()
{

  m_Exporter = vtkImageExport::New();

  m_Importer = ImporterFilterType::New();

  m_Importer->SetUpdateInformationCallback( m_Exporter->GetUpdateInformationCallback());
  m_Importer->SetPipelineModifiedCallback( m_Exporter->GetPipelineModifiedCallback());
  m_Importer->SetWholeExtentCallback( m_Exporter->GetWholeExtentCallback());
  m_Importer->SetSpacingCallback( m_Exporter->GetSpacingCallback());
  m_Importer->SetOriginCallback( m_Exporter->GetOriginCallback());
  m_Importer->SetScalarTypeCallback( m_Exporter->GetScalarTypeCallback());
  m_Importer->SetNumberOfComponentsCallback( m_Exporter->GetNumberOfComponentsCallback());
  m_Importer->SetPropagateUpdateExtentCallback( m_Exporter->GetPropagateUpdateExtentCallback());
  m_Importer->SetUpdateDataCallback( m_Exporter->GetUpdateDataCallback());
  m_Importer->SetDataExtentCallback( m_Exporter->GetDataExtentCallback());
  m_Importer->SetBufferPointerCallback( m_Exporter->GetBufferPointerCallback());
  m_Importer->SetCallbackUserData( m_Exporter->GetCallbackUserData());

}




/**
 * Destructor
 */
template <class TOutputImage>
VTKImageToImageFilter<TOutputImage>
::~VTKImageToImageFilter()
{
  if( m_Exporter )
    {
    m_Exporter->Delete();
    m_Exporter = 0;
    }
}



/**
 * Set a vtkImageData as input 
 */
template <class TOutputImage>
void
VTKImageToImageFilter<TOutputImage>
::SetInput( vtkImageData * inputImage )
{
  m_Exporter->SetInput( inputImage );
}



/**
 * Get an itk::Image as output
 */
template <class TOutputImage>
const typename VTKImageToImageFilter<TOutputImage>::OutputImageType *
VTKImageToImageFilter<TOutputImage>
::GetOutput() const
{
  return m_Importer->GetOutput();
}




/**
 * Get the exporter filter
 */
template <class TOutputImage>
vtkImageExport *
VTKImageToImageFilter<TOutputImage>
::GetExporter() const
{
  return m_Exporter;
}



/**
 * Get the importer filter
 */
template <class TOutputImage>
typename VTKImageToImageFilter<TOutputImage>::ImporterFilterType *
VTKImageToImageFilter<TOutputImage>
::GetImporter() const
{
  return m_Importer;
}




/**
 * Delegate the Update to the importer
 */
template <class TOutputImage>
void
VTKImageToImageFilter<TOutputImage>
::Update()
{
  m_Importer->Update();
}




} // end namespace itk

#endif


