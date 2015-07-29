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
  Module:    $RCSfile: vtkjwsSignedDanielssonDistanceMapImageFilter.cpp,v $
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

#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "vtkjwsSignedDanielssonDistanceMapImageFilter.h"
#include "itkSignedDanielssonDistanceMapImageFilter.h"
#include "itkGrayscaleMorphologicalClosingImageFilter.h"
#include "itkBinaryBallStructuringElement.h" 
#include "vtkImageThreshold.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkInformation.h"
#include "itkImage.h"
#include "itkVTKImageExport.h"
#include "itkVTKImageImport.h"
#include "vtkITKUtility.h"



//------------------------------------------------------------------------------
// First a Helper Class
// -----------------------------------------------------------------------------
vtkjwsSignedDanielssonDistanceMapImageFilter* vtkjwsSignedDanielssonDistanceMapImageFilter::New()
{
  return new vtkjwsSignedDanielssonDistanceMapImageFilter;
}



vtkjwsSignedDanielssonDistanceMapImageFilter::vtkjwsSignedDanielssonDistanceMapImageFilter()
{
  this->LowerThreshold=100;
  this->UpperThreshold=2000;
  this->InValue=3000;
  this->OutValue=0;
  this->Radius=1;
}
// ----------------------------------------------------------------------------
vtkjwsSignedDanielssonDistanceMapImageFilter::~vtkjwsSignedDanielssonDistanceMapImageFilter()
{
}
// ---------------------------------------------------------------------------------------------
template <int ImageDimension>
int vtkjwsSignedDanielssonDistanceMapImageFilter_RunFilter(vtkImageData* input,vtkImageData* output,vtkjwsSignedDanielssonDistanceMapImageFilter* self)
{
  typedef float  PixelType;
  typedef itk::Image<PixelType,ImageDimension>    ImageType;
  typedef itk::VTKImageImport<ImageType> ImportType;
  typedef itk::VTKImageExport<ImageType> ExportType;
  
  typedef itk::SignedDanielssonDistanceMapImageFilter<ImageType,ImageType>  DMapFilterType; 
  typedef itk::BinaryBallStructuringElement<PixelType,ImageDimension>  StructuringElementType;
  typedef itk::GrayscaleMorphologicalClosingImageFilter<ImageType,ImageType,StructuringElementType >  ClosingFilterType; 


  // Step 1: Take VTK Image and make it float
  vtkImageThreshold* thresholder=vtkImageThreshold::New();
  thresholder->SetInput(input);
  thresholder->SetOutputScalarTypeToShort();
  thresholder->SetOutValue(self->GetOutValue());
  thresholder->SetInValue(self->GetInValue());
  thresholder->ThresholdBetween(self->GetLowerThreshold(),self->GetUpperThreshold());
  thresholder->SetOutputScalarTypeToFloat();
  thresholder->ReplaceInOn();
  thresholder->ReplaceOutOn();

  // Step 2: Convert VTK Image to ITK
  vtkImageExport *vtkInputExporter = vtkImageExport::New();
  vtkInputExporter->SetInput(thresholder->GetOutput());
  typename ImportType::Pointer itkInputImporter =  ImportType::New();
  ConnectPipelines(vtkInputExporter, itkInputImporter);
  itkInputImporter->Update();
  
  // Step 3:
  // Run Pipeline 
  StructuringElementType  structuringElement;
  structuringElement.SetRadius( self->GetRadius() );  // 3x3 structuring element
  structuringElement.CreateStructuringElement();

  typename ClosingFilterType::Pointer binaryClosing = ClosingFilterType::New();  
  binaryClosing->SetKernel( structuringElement );
  binaryClosing->SetInput( itkInputImporter->GetOutput() );

  typename  DMapFilterType::Pointer dmap =    DMapFilterType::New();
  dmap->SetInput(binaryClosing->GetOutput());
  dmap->Update();


  // Step 4:
  // Push it out to VTK
  typename ExportType::Pointer itkExporter = ExportType::New();
  itkExporter->SetInput(dmap->GetOutput());
  itkExporter->Update();

  vtkImageImport* vtkImporter=vtkImageImport::New();
  ConnectPipelines(itkExporter, vtkImporter);
  vtkImporter->Update();

  output->DeepCopy(vtkImporter->GetOutput());
  
  vtkImporter->Delete();
  thresholder->Delete();
  vtkInputExporter->Delete();
  return 1;
}


int vtkjwsSignedDanielssonDistanceMapImageFilter::RunFilter(vtkImageData* input,vtkImageData* output)
{
  int dim[3]; input->GetDimensions(dim);


  if (dim[2]==1)
    return vtkjwsSignedDanielssonDistanceMapImageFilter_RunFilter<2>(input,output,this);

  return vtkjwsSignedDanielssonDistanceMapImageFilter_RunFilter<3>(input,output,this);
}
// ----------------------------------------------------------------------------
void vtkjwsSignedDanielssonDistanceMapImageFilter::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs to vtkjwsSignedDanielssonDistanceMapImageFilter");
      return;
    }

  this->RunFilter(input,output);
}


// Tell the pipeline that the output has same dimensions as input but force SHORT one component
// --------------------------------------------------------------------------------------------
int vtkjwsSignedDanielssonDistanceMapImageFilter::RequestInformation(vtkInformation *request,  vtkInformationVector **inputVector, 
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

