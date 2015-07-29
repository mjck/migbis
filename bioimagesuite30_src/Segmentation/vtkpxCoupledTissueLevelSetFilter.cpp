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
  Module:    $RCSfile: vtkpxCoupledTissueLevelSetFilter.cpp,v $
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
#include "vtkpxLevelSetUtil.h"
#include "vtkImageImport.h"
#include "vtkImageExport.h"
#include "itkpxLevelSetCallback.h"
#include "itkpxTissueSegmentationCoupledLevelSetImageFilter.h"
#include "vtkImageCast.h"
#include "itkReinitializeLevelSetImageFilter.h"
#include "itkpxSaturateImageFilter.h"
#include "vtkpxCoupledTissueLevelSetFilter.h"
#include "vtkpxImageExtractVOI.h"
#include "vtkpxAnalyzeImageWriter.h"
#include "vtkImageChangeInformation.h"
//------------------------------------------------------------------------------
// First a Helper Class
// -----------------------------------------------------------------------------
//------------------------------------------------------------------------------

vtkpxCoupledTissueLevelSetFilter* vtkpxCoupledTissueLevelSetFilter::New()
{
  return new vtkpxCoupledTissueLevelSetFilter;
}



vtkpxCoupledTissueLevelSetFilter::vtkpxCoupledTissueLevelSetFilter()
{
  this->CouplingOffset=1.0;
  this->SetPresetMode(0);
  this->Output2=NULL;
  this->MinThickness=2.0;
  this->MaxThickness=5.0;
  this->Padding=10;
  this->SetIterationName("joined");
}
// ----------------------------------------------------------------------------
vtkpxCoupledTissueLevelSetFilter::~vtkpxCoupledTissueLevelSetFilter()
{
  if (this->Output2!=NULL)
    this->Output2->Delete();
  this->Output2=NULL;

}
// ---------------------------------------------------------------------------------------------
void vtkpxCoupledTissueLevelSetFilter::SetPresetMode(int m)
{
  if (m<0 || m>2) 
    m=0;

  switch (m)
    {
    case 0:
      this->WhiteMatterMean= 275;   
      this->WhiteMatterSigma=13; 
      this->GrayMatterMean= 200; 
      this->GrayMatterSigma= 21;
      this->CSFMean= 80;  
      this->CSFSigma= 20;
      break;
    case 1: 
      this->WhiteMatterMean= 285; 
      this->WhiteMatterSigma=14; 
      this->GrayMatterMean= 200; 
      this->GrayMatterSigma= 21;
      this->CSFMean= 80;
      this->CSFSigma= 20;
      break;
    case 2:
      this->WhiteMatterMean= 300; 
      this->WhiteMatterSigma=18; 
      this->GrayMatterMean= 200; 
      this->GrayMatterSigma= 21;
      this->CSFMean= 80;  
      this->CSFSigma= 20;
      break;
    }
}

int vtkpxCoupledTissueLevelSetFilter::CheckInputs()
{
  return 1;
}
// ---------------------------------------------------------------------------
template <int ImageDimension>
int vtkpxCoupledTissueLevelSetFilter_RunCoupledActiveContour(vtkImageData* input,vtkImageData* output,vtkImageData* output2,
							     vtkpxCoupledTissueLevelSetFilter* self)
{
  typedef  float  PixelType;
  typedef itk::Image<PixelType,ImageDimension>    ImageType;
  typedef itk::VTKImageImport<ImageType> ImportType;
  typedef itk::VTKImageExport<ImageType> ExportType;
  typedef itk::TissueSegmentationCoupledLevelSetImageFilter< ImageType, ImageType > LevelSetFilterType;
  typedef itk::ReinitializeLevelSetImageFilter<ImageType> Reinitializer;
  typedef itk::SaturateImageFilter<ImageType,ImageType> SaturateFilterType;
  typedef CommandIterationUpdate<LevelSetFilterType,SaturateFilterType,ImageType> CommandType;
  //typedef CommandIterationUpdate<LevelSetFilterType,Reinitializer,ImageType> CommandType;


  int original_dimx=input->GetDimensions()[0];
  double meanthickness=0.5*(self->GetMinThickness()+self->GetMaxThickness());
  vtkImageData* paddedimage=vtkpxCoupledTissueLevelSetFilter::CreatePaddedImage(input,
										self->GetPadding(),
										0.0);

  vtkImageData* paddedinput=vtkpxCoupledTissueLevelSetFilter::CreatePaddedImage(self->GetInputLevelSet(),
										self->GetPadding(),
										meanthickness);
										

  /*  vtkpxAnalyzeImageWriter* w=vtkpxAnalyzeImageWriter::New();
  w->SetInput(paddedimage);
  w->Save("00_padimage.nii.gz");

  
  w->SetInput(paddedinput);
  w->Save("00_padinput.nii.gz");
  w->Delete();*/

  // Export original image
  vtkImageExport *vtkInputExporter = vtkImageExport::New();
  vtkInputExporter->SetInput(paddedimage);
  typename ImportType::Pointer itkInputImporter =  ImportType::New();
  ConnectPipelines(vtkInputExporter, itkInputImporter);
  itkInputImporter->Update();
  
  // Export Initial Levelset
  vtkImageExport *vtkInputLevelSetExporter = vtkImageExport::New();
  vtkInputLevelSetExporter->SetInput(paddedinput);
  typename ImportType::Pointer itkInputLevelSetImporter =  ImportType::New();
  ConnectPipelines(vtkInputLevelSetExporter, itkInputLevelSetImporter);
  itkInputLevelSetImporter->Update();


  typename LevelSetFilterType::Pointer levelSet;
  typename Reinitializer::Pointer reinit;
  typename SaturateFilterType::Pointer sat;
  typename CommandType::Pointer observer;

  levelSet=    LevelSetFilterType::New();
  levelSet->SetPropagationScaling( self->GetPropagationWeight() );
  levelSet->SetCurvatureScaling(   self->GetCurvatureWeight() );
  levelSet->SetAdvectionScaling(self->GetAdvectionWeight());
  levelSet->SetInput(  itkInputLevelSetImporter->GetOutput() );
  levelSet->SetFeatureImage(itkInputImporter->GetOutput());
  levelSet->SetCoupledLevelSetBoundaries(original_dimx,original_dimx+self->GetPadding());
  levelSet->SetConstraintOffset(self->GetCouplingOffset());

  levelSet->SetWhiteMatterTissue(self->GetWhiteMatterMean(),self->GetWhiteMatterSigma());
  levelSet->SetGrayMatterTissue(self->GetGrayMatterMean(),self->GetGrayMatterSigma());
  levelSet->SetCSFTissue(self->GetCSFMean(),self->GetCSFSigma());
  levelSet->SetIsoSurfaceValue(0.0);
  levelSet->SetLowerWhiteGrayBound(self->GetMinThickness());
  levelSet->SetUpperWhiteGrayBound(self->GetMaxThickness());
  levelSet->SetLowerGrayCSFBound(-self->GetMaxThickness());
  levelSet->SetUpperGrayCSFBound(-self->GetMinThickness());
  levelSet->SetMaximumRMSError(0.0);
  levelSet->SetNumberOfIterations(1);
  levelSet->SetNumberOfLayers(int(2*self->GetMaxThickness()));
  levelSet->Update();

  self->SetRealIteration(self->GetRealIteration()+1);

  typename ExportType::Pointer itkExporter = ExportType::New();
  itkExporter->SetInput(levelSet->GetOutput());

  vtkImageImport* vtkImporter=vtkImageImport::New();
  ConnectPipelines(itkExporter, vtkImporter);

  int maxiter=int(self->GetLevelsetIterations());
  int iter=1;
  int count=1;
  int fraction=int(self->GetUpdatePercentage()*float(maxiter));
  float maxerror=self->GetRMSError()+1.0;

  char line[220];
  sprintf(line,"Begining level set evolution, updating every %d iterations", fraction);
  self->SetOutputStatus(line);
  //  fprintf(stderr,"%s\n",line);

  while( iter <= maxiter && maxerror > self->GetRMSError() )
    {
      levelSet->SetStateToInitialized();
      levelSet->SetNumberOfIterations(levelSet->GetElapsedIterations()+1);
      levelSet->Modified();
      levelSet->Update();
      maxerror=levelSet->GetRMSChange();
      self->SetRealIteration(self->GetRealIteration()+1);
      
      sprintf(line,"Completed  %d (%d) /%d error=%6.4f",iter,self->GetRealIteration(),maxiter,maxerror);
      self->SetOutputStatus(line);
      int doupdate=0;
      if (count==fraction)
	{
	  doupdate=1;
	  count=0;
	  itkExporter->Update();
	  vtkImporter->Update();

	  vtkImageData* tmp_out=vtkpxCoupledTissueLevelSetFilter::CreateInnerOutputImage(vtkImporter->GetOutput(),original_dimx);
	  output2->ShallowCopy(tmp_out);
	  tmp_out->Delete();

	  
	  vtkImageData* tmp_out2=vtkpxCoupledTissueLevelSetFilter::CreateOuterOutputImage(vtkImporter->GetOutput(),
											  self->GetPadding(),original_dimx);
	  output->ShallowCopy(tmp_out2);
	  tmp_out2->Delete();
	  self->UpdateProgress(self->GetProgress()+float(iter)/float(maxiter));
	}

      ++iter;
      ++count;
    }

  
  reinit=Reinitializer::New();
  reinit->SetInput(levelSet->GetOutput());
  reinit->NarrowBandingOn();
  reinit->SetNarrowBandwidth(4.0*meanthickness);
      
  sat=SaturateFilterType::New();
  sat->SetInput(reinit->GetOutput());
  sat->SetLowerBound(-3.0*meanthickness);
  sat->SetUpperBound( 3.0*meanthickness);
  sat->Update();

  itkExporter->SetInput(sat->GetOutput());
  itkExporter->Update();
  ConnectPipelines(itkExporter, vtkImporter);
  vtkImporter->Update();
  
  vtkImageData* tmp_out=vtkpxCoupledTissueLevelSetFilter::CreateInnerOutputImage(vtkImporter->GetOutput(),original_dimx);
  output2->ShallowCopy(tmp_out);
  tmp_out->Delete();


  vtkImageData* tmp_out2=vtkpxCoupledTissueLevelSetFilter::CreateOuterOutputImage(vtkImporter->GetOutput(),
										 self->GetPadding(),original_dimx);
  output->ShallowCopy(tmp_out2);
  tmp_out2->Delete();
  
  vtkInputLevelSetExporter->Delete();
  vtkInputExporter->Delete();
  vtkImporter->Delete();

  paddedimage->Delete();
  paddedinput->Delete();

  return 1;
}


int vtkpxCoupledTissueLevelSetFilter::RunCoupledActiveContour(vtkImageData* input,vtkImageData* output,vtkImageData* output2)
{
  int dim[3]; input->GetDimensions(dim);

  if (dim[2]==1)
    return vtkpxCoupledTissueLevelSetFilter_RunCoupledActiveContour<2>(input,output,output2,this);

  return vtkpxCoupledTissueLevelSetFilter_RunCoupledActiveContour<3>(input,output,output2,this);




}

// ----------------------------------------------------------------------------
void vtkpxCoupledTissueLevelSetFilter::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxCoupledTissueLevelSetFilter");
      return;
    }

  fprintf(stderr,"Creating Input Level Set\n");
  this->GenerateInputLevelSet(input);
  fprintf(stderr,"Running Level Set\n");

  if (this->Output2!=NULL)
    this->Output2->Delete();
  this->Output2=vtkImageData::New();

  this->RunCoupledActiveContour(input,output,Output2);

  this->UpdateProgress(1.0);
}
// ----------------------------------------------------------------------------
vtkImageData* vtkpxCoupledTissueLevelSetFilter::CreatePaddedImage(vtkImageData* input,int padding,double offset)
{
  if (input==NULL)
    return NULL;
  int dim[3];  input->GetDimensions(dim);
  int odim[3]; input->GetDimensions(odim);
  odim[0]=odim[0]*2+padding;

  vtkImageData* output=vtkImageData::New();
  output->CopyStructure(input);
  output->SetNumberOfScalarComponents(1);
  output->SetDimensions(odim);
  output->SetScalarTypeToFloat();
  output->AllocateScalars();

  int shift=dim[0]+padding;

  int index=0;
  vtkDataArray* inp=input->GetPointData()->GetScalars();
  vtkDataArray* out=output->GetPointData()->GetScalars();
  if (fabs(offset)<0.01)
    out->FillComponent(0,0.0);
  else
    out->FillComponent(0,100.0);

  for (int ic=0;ic<dim[2];ic++)
    for (int ib=0;ib<dim[1];ib++)
      for (int ia=0;ia<dim[0];ia++)
	{
	  double v=inp->GetComponent(index,0);
	  output->SetScalarComponentFromDouble(ia,ib,ic,0,v);
	  output->SetScalarComponentFromDouble(ia+shift,ib,ic,0,v-offset);
	  ++index;
	}
  return output;
}

vtkImageData* vtkpxCoupledTissueLevelSetFilter::CreateInnerOutputImage(vtkImageData* result,int dimx)
{
  if (result==NULL)
    return NULL;

  int dim[3]; result->GetDimensions(dim);

  vtkpxImageExtractVOI* voi=vtkpxImageExtractVOI::New();
  voi->SetInput(result);
  voi->SetVOI(0,dimx-1,0,dim[1]-1,0,dim[2]-1);
  voi->Update();
  
  vtkImageData* out=vtkImageData::New();
  out->ShallowCopy(voi->GetOutput());
  voi->Delete();
  return out;
}

vtkImageData* vtkpxCoupledTissueLevelSetFilter::CreateOuterOutputImage(vtkImageData* result,int padding,int dimx)
{
  if (result==NULL)
    return NULL;

  int dim[3]; result->GetDimensions(dim);

  vtkpxImageExtractVOI* voi=vtkpxImageExtractVOI::New();
  voi->SetInput(result);
  voi->SetVOI(dimx+padding,dim[0]-1,0,dim[1]-1,0,dim[2]-1);
  voi->Update();

  vtkImageChangeInformation* info=vtkImageChangeInformation::New();
  info->SetInput(voi->GetOutput());
  info->SetOutputOrigin(result->GetOrigin());
  info->Update();
  
  vtkImageData* out=vtkImageData::New();
  out->ShallowCopy(info->GetOutput());
  voi->Delete();
  return out;
}


