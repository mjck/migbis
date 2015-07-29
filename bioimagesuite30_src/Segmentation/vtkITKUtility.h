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





#ifndef __vtkITKUtility_h
#define __vtkITKUtility_h


#include "vtkObjectFactory.h"
#include "vtkSetGet.h"

/**
 * This function will connect the given itk::VTKImageExport filter to
 * the given vtkImageImport filter.
 */
template <typename ITK_Exporter, typename VTK_Importer>
void ConnectPipelines(ITK_Exporter exporter, VTK_Importer* importer)
{
  importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
  importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
  importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
  importer->SetSpacingCallback(exporter->GetSpacingCallback());
  importer->SetOriginCallback(exporter->GetOriginCallback());
  importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
  importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
  importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
  importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
  importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
  importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
  importer->SetCallbackUserData(exporter->GetCallbackUserData());
}

/**
 * This function will connect the given vtkImageExport filter to
 * the given itk::VTKImageImport filter.
 */
template <typename VTK_Exporter, typename ITK_Importer>
void ConnectPipelines(VTK_Exporter* exporter, ITK_Importer importer)
{
  importer->SetUpdateInformationCallback(exporter->GetUpdateInformationCallback());
  importer->SetPipelineModifiedCallback(exporter->GetPipelineModifiedCallback());
  importer->SetWholeExtentCallback(exporter->GetWholeExtentCallback());
  importer->SetSpacingCallback(exporter->GetSpacingCallback());
  importer->SetOriginCallback(exporter->GetOriginCallback());
  importer->SetScalarTypeCallback(exporter->GetScalarTypeCallback());
  importer->SetNumberOfComponentsCallback(exporter->GetNumberOfComponentsCallback());
  importer->SetPropagateUpdateExtentCallback(exporter->GetPropagateUpdateExtentCallback());
  importer->SetUpdateDataCallback(exporter->GetUpdateDataCallback());
  importer->SetDataExtentCallback(exporter->GetDataExtentCallback());
  importer->SetBufferPointerCallback(exporter->GetBufferPointerCallback());
  importer->SetCallbackUserData(exporter->GetCallbackUserData());
}


#define DelegateSetMacro(name,arg) DelegateITKInputMacro(Set##name,arg)
#define DelegateITKInputMacro(name,arg) \
if ( 1 ) { \
  vtkDebugMacro( << this->GetClassName() << " (" << this << "): setting " #name " to " << #arg ); \
  ImageFilterType* tempFilter = dynamic_cast<ImageFilterType*> ( this->m_Filter.GetPointer() ); \
  if ( tempFilter ) \
    { \
    tempFilter->name ( arg ); \
    this->Modified(); \
    } \
  }

#define DelegateGetMacro(name) DelegateITKOutputMacro (Get##name)
#define DelegateITKOutputMacro(name) \
if ( 1 ) { \
  vtkDebugMacro(<< this->GetClassName() << " (" << this << "): returning " #name ); \
  ImageFilterType* tempFilter = dynamic_cast<ImageFilterType*> ( this->m_Filter.GetPointer() ); \
  if ( tempFilter ) \
    { \
    return tempFilter->name (); \
    } \
    else \
    { \
    vtkErrorMacro ( << this->GetClassName() << " Error getting " #name " Dynamic cast returned 0" ); \
    return 0; \
    } \
  }


// struct vtkITKProgressDisplay
// {
//   ProgressDisplay(vtkObject* obj, itk::ProcessObject* process): m_Process(process), m_Object(obj) {}
  
//   void Display()
//   {
//     m_Object->SetProgress ( m_Process->GetProgress() );
//     }
  
//   itk::ProcessObject::Pointer m_Process;
//   vtkObject* m_Object();
// };

//   // Add a progress observer for the itk::CurvatureFlowImageFilter.
//   // This will make it clear when this part of the ITK pipeline
//   // executes.
//   ProgressDisplay progressDisplay(denoiser);
//   itk::SimpleMemberCommand<ProgressDisplay>::Pointer progressEvent =
//     itk::SimpleMemberCommand<ProgressDisplay>::New();
//   progressEvent->SetCallbackFunction(&progressDisplay,
//                                      &ProgressDisplay::Display);
//   denoiser->AddObserver(itk::ProgressEvent(), progressEvent);



#endif

