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
  Module:    $RCSfile: itkImageToVTKImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2004/02/19 03:47:59 $
  Version:   $Revision: 1.6 $

  Copyright (c) 2002 Insight Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkImageToVTKImageFilter_h
#define __itkImageToVTKImageFilter_h

#include "itkVTKImageExport.h"
#include "vtkImageImport.h"
#include "vtkImageData.h"

namespace itk
{
  
/** \class ImageToVTKImageFilter
 * \brief Converts an ITK image into a VTK image and plugs a 
 *  itk data pipeline to a VTK datapipeline.   
 *
 *  This class puts together an itkVTKImageExporter and a vtkImageImporter.
 *  It takes care of the details related to the connection of ITK and VTK
 *  pipelines. The User will perceive this filter as an adaptor to which
 *  an itk::Image can be plugged as input and a vtkImage is produced as 
 *  output.
 * 
 * \ingroup   ImageFilters     
 */
template <class TInputImage >
class  ImageToVTKImageFilter : public ProcessObject
{
public:
  /** Standard class typedefs. */
  typedef ImageToVTKImageFilter       Self;
  typedef ProcessObject             Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ImageToVTKImageFilter, ProcessObject);

  /** Some typedefs. */
  typedef TInputImage InputImageType;
  typedef typename    InputImageType::ConstPointer    InputImagePointer;
  typedef VTKImageExport< InputImageType>            ExporterFilterType; 
  typedef typename ExporterFilterType::Pointer        ExporterFilterPointer;
 
  /** Get the output in the form of a vtkImage. 
      This call is delegated to the internal vtkImageImporter filter  */
  vtkImageData *  GetOutput() const;

  /** Set the input in the form of an itk::Image */
  void SetInput( const InputImageType * );

  /** Return the internal VTK image importer filter.
      This is intended to facilitate users the access 
      to methods in the importer */
  vtkImageImport * GetImporter() const;

  /** Return the internal ITK image exporter filter.
      This is intended to facilitate users the access 
      to methods in the exporter */
  ExporterFilterType * GetExporter() const;
  
  /** This call delegate the update to the importer */
  void Update();
  
protected:
  ImageToVTKImageFilter(); 
  virtual ~ImageToVTKImageFilter(); 

private:
  ImageToVTKImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  ExporterFilterPointer       m_Exporter;
  vtkImageImport            * m_Importer;

};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkImageToVTKImageFilter.txx"
#endif

#endif




