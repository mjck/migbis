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

  Program:   vtkpxcontrib library file
  Module:    $RCSfile: vtkbisBinaryThinningFilter.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/20 18:06:52 $
  Version:   $Revision: 1.1 $


Copyright (c) 1995-2001 Xenios Papademetris papad@noodle.med.yale.edu
All rights reserved.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
#include "vtkImageData.h"
#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "vtkbisBinaryThinningFilter.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformation.h"
#include "itkImage.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "vtkITKUtility.h"
#include "itkBinaryThinningImageFilter3D.h"
#include "vtkImageThreshold.h"

//------------------------------------------------------------------------------
// First a Helper Class
// -----------------------------------------------------------------------------
vtkbisBinaryThinningFilter* vtkbisBinaryThinningFilter::New()
{
  return new vtkbisBinaryThinningFilter;
}



vtkbisBinaryThinningFilter::vtkbisBinaryThinningFilter()
{

}
// ----------------------------------------------------------------------------
vtkbisBinaryThinningFilter::~vtkbisBinaryThinningFilter()
{
}
// ---------------------------------------------------------------------------------------------
void vtkbisBinaryThinningFilter::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs to vtkbisBinaryThinningFilter");
      return;
    }

  typedef short  PixelType;
  typedef itk::Image<PixelType,3>    ImageType;
  typedef itk::VTKImageImport<ImageType> ImportType;
  typedef itk::VTKImageExport<ImageType> ExportType;
  
  typedef itk::BinaryThinningImageFilter3D< ImageType, ImageType > ThinningFilterType;


  // Step 1: Take VTK Image and make it float
  vtkImageThreshold* thresholder=vtkImageThreshold::New();
  thresholder->SetInput(input);
  thresholder->SetOutputScalarTypeToShort();
  thresholder->SetOutValue(0);
  thresholder->SetInValue(100);
  thresholder->ThresholdByUpper(1.0);
  thresholder->SetOutputScalarTypeToShort();
  thresholder->ReplaceInOn();
  thresholder->ReplaceOutOn();

  // Step 2: Convert VTK Image to ITK
  vtkImageExport *vtkInputExporter = vtkImageExport::New();
  vtkInputExporter->SetInput(thresholder->GetOutput());
  
  ImportType::Pointer itkInputImporter =  ImportType::New();
  ConnectPipelines(vtkInputExporter, itkInputImporter);
  itkInputImporter->Update();
  
  ThinningFilterType::Pointer thinningFilter = ThinningFilterType::New();
  thinningFilter->SetInput(  itkInputImporter->GetOutput() );
  thinningFilter->Update();


  // Step 4:
  // Push it out to VTK
  ExportType::Pointer itkExporter = ExportType::New();
  itkExporter->SetInput(thinningFilter->GetOutput());
  itkExporter->Update();

  vtkImageImport* vtkImporter=vtkImageImport::New();
  ConnectPipelines(itkExporter, vtkImporter);
  vtkImporter->Update();

  output->DeepCopy(vtkImporter->GetOutput());
  
  vtkImporter->Delete();
  thresholder->Delete();
  vtkInputExporter->Delete();
  
  
}


// Tell the pipeline that the output has same dimensions as input but force SHORT one component
// --------------------------------------------------------------------------------------------
int vtkbisBinaryThinningFilter::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector, 
								     vtkInformationVector *outputVector)
{
  vtkInformation *inInfo =  inputVector[0]->GetInformationObject(0);
  vtkInformation *outInfo = outputVector->GetInformationObject(0);

  int wholeExtent[6]; inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), wholeExtent);
  double sp[3];  inInfo->Get(vtkDataObject::SPACING(), sp);
  double origin[3];  inInfo->Get(vtkDataObject::ORIGIN(),  origin);

  outInfo->Set(vtkDataObject::ORIGIN(),  origin, 3);
  outInfo->Set(vtkDataObject::SPACING(),sp , 3);
  outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),wholeExtent,6);
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), VTK_SHORT, 1);

  return 1;
}

