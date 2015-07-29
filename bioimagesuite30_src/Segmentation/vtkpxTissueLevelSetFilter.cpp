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
  Module:    $RCSfile: vtkpxTissueLevelSetFilter.cpp,v $
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
#include "vtkpxTissueLevelSetFilter.h"
#include "itkpxTissueSegmentationLevelSetImageFilter.h"
#include "itkThresholdSegmentationLevelSetImageFilter.h"
#include "vtkImageCast.h"
#include "itkReinitializeLevelSetImageFilter.h"
#include "itkpxSaturateImageFilter.h"
#include "vtkpxAnalyzeImageWriter.h"
//------------------------------------------------------------------------------
// First a Helper Class
// -----------------------------------------------------------------------------
//------------------------------------------------------------------------------

vtkpxTissueLevelSetFilter* vtkpxTissueLevelSetFilter::New()
{
  return new vtkpxTissueLevelSetFilter;
}



vtkpxTissueLevelSetFilter::vtkpxTissueLevelSetFilter()
{
  this->InsideMean=100.0;
  this->InsideSigma=10.0;
  this->OutsideMean=0.0;
  this->OutsideSigma=10.0;
  this->Outside2Mean=0.0;
  this->Outside2Sigma=10.0;
  this->DynamicParameterUpdate=1;
  this->BeginLevel=1;
  this->EndLevel=1;
  this->NumberOfLevels=2;
  this->BiExponentialOutside=0;
  this->VolumeWeight=0.0;
  this->ApproximateVolume=0.0;
}
// ----------------------------------------------------------------------------
vtkpxTissueLevelSetFilter::~vtkpxTissueLevelSetFilter()
{
}
// ---------------------------------------------------------------------------------------------
int vtkpxTissueLevelSetFilter::CheckInputs()
{
  return 1;
}
// ----------------------------------------------------------------------------
template <int ImageDimension>
int vtkpxTissueLevelSetFilter_RunActiveContour(vtkImageData* input,vtkImageData* initial,vtkImageData* output,vtkpxTissueLevelSetFilter* self)
{
  typedef  float  PixelType;
  typedef itk::Image<PixelType,ImageDimension>    ImageType;
  typedef itk::VTKImageImport<ImageType> ImportType;
  typedef itk::VTKImageExport<ImageType> ExportType;
  typedef itk::TissueSegmentationLevelSetImageFilter< ImageType, ImageType > LevelSetFilterType;
  typedef itk::SaturateImageFilter<ImageType,ImageType> SaturateFilterType;
  typedef itk::ReinitializeLevelSetImageFilter<ImageType> Reinitializer;
  

  vtkImageCast* cast=vtkImageCast::New();
  cast->SetInput(input);
  cast->SetOutputScalarTypeToFloat();
  // Export original image
  vtkImageExport *vtkInputExporter = vtkImageExport::New();
  vtkInputExporter->SetInput(cast->GetOutput());
  typename ImportType::Pointer itkInputImporter =  ImportType::New();
  ConnectPipelines(vtkInputExporter, itkInputImporter);
  itkInputImporter->Update();
  
  //  fprintf(stderr,"Importer Done\n");

  // Export Initial Levelset
  vtkImageExport *vtkInputLevelSetExporter = vtkImageExport::New();
  vtkInputLevelSetExporter->SetInput(initial);
  typename ImportType::Pointer itkInputLevelSetImporter =  ImportType::New();
  ConnectPipelines(vtkInputLevelSetExporter, itkInputLevelSetImporter);
  itkInputLevelSetImporter->Update();

  //  fprintf(stderr,"Input Level Importer Done\n");

  typename  LevelSetFilterType::Pointer levelSet =    LevelSetFilterType::New();
  levelSet->SetInsideTissue(self->GetInsideMean(),self->GetInsideSigma());
  levelSet->SetOutsideTissue(self->GetOutsideMean(),self->GetOutsideSigma());
  if (self->GetBiExponentialOutside())
    {
      levelSet->SetOutside2Tissue(self->GetOutside2Mean(),self->GetOutside2Sigma());
      levelSet->SetBiExponentialOutside(1);
    }
  else
    levelSet->SetBiExponentialOutside(0);

  levelSet->SetDynamicUpdateParameters(self->GetDynamicParameterUpdate());
  levelSet->SetPropagationScaling( self->GetPropagationWeight() );
  levelSet->SetCurvatureScaling(   self->GetCurvatureWeight() );
  levelSet->SetAdvectionScaling(self->GetAdvectionWeight());

  if (self->GetVolumeWeight()>0.0)
    {
      levelSet->SetApproximateVolumeConstraintWeight(self->GetVolumeWeight());
      levelSet->SetApproximateVolume(self->GetApproximateVolume());
      fprintf(stderr,"VolumeWeight=%.5f, ApproximateVolume=%.5f\n",
	      self->GetVolumeWeight(),
	      self->GetApproximateVolume());
    }

  levelSet->SetMaximumRMSError(0.0);//self->GetRMSError());
  levelSet->SetNumberOfIterations(1);
  levelSet->SetInput(  itkInputLevelSetImporter->GetOutput() );
  levelSet->SetIsoSurfaceValue(0.0);
  levelSet->SetFeatureImage(itkInputImporter->GetOutput());
  levelSet->SetNumberOfIterations(1);



  //  fprintf(stderr,"Just Before Update\n");

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


  while( iter <= maxiter && maxerror > self->GetRMSError() )
    {
      //      fprintf(stderr,"maxiter=%d iter=%d maxerror=%.2f (<%.5f)\n",maxiter,iter,maxerror,self->GetRMSError());
      levelSet->SetStateToInitialized();
      levelSet->SetNumberOfIterations(levelSet->GetElapsedIterations()+1);
      levelSet->Modified();
      levelSet->Update();
      maxerror=levelSet->GetRMSChange();
      self->SetRealIteration(self->GetRealIteration()+1);

      sprintf(line,"Completed  %d (%d) /%d error=%6.4f",iter,self->GetRealIteration(),maxiter,maxerror);
      self->SetOutputStatus(line);
      //      fprintf(stderr,"%s\n",line);

      int doupdate=0,dosave=0;
      if (self->GetIterationSave() && 
	  self->GetIterationSaveFrequency()*int(self->GetRealIteration()/self->GetIterationSaveFrequency())==self->GetRealIteration())
	{
	  doupdate=1;
	  dosave=1;
	}
      
      if (count==fraction)
	{
	  doupdate=1;
	  count=0;
	}

      if (doupdate)
	{
	  itkExporter->Update();
	  vtkImporter->Update();
	  output->ShallowCopy(vtkImporter->GetOutput());
	  self->UpdateProgress(float(iter)/float(maxiter*self->GetNumberOfLevels()));

	  if (dosave)
	    {

	      sprintf(line,"%s_%03d",self->GetIterationName(),self->GetRealIteration());
	      vtkpxAnalyzeImageWriter* anaw=vtkpxAnalyzeImageWriter::New();
	      anaw->SetInput(output);
	      anaw->SetOrientation(self->GetIterationSaveOrientation());
	      anaw->Save(line);
	      std::cerr << "\t\t ***** Saving levelset " << line  << std::endl;
	      anaw->Delete();
	    }
	}

      ++iter;
      ++count;
    }

  typename Reinitializer::Pointer reinit=Reinitializer::New();
  reinit->SetInput(levelSet->GetOutput());
  reinit->NarrowBandingOn();
  reinit->SetNarrowBandwidth(12);

  typename SaturateFilterType::Pointer sat=SaturateFilterType::New();
  sat->SetInput(reinit->GetOutput());
  sat->SetLowerBound(-20.0);
  sat->SetUpperBound(20.0);


  itkExporter->SetInput(sat->GetOutput());
  itkExporter->Modified();

  reinit->Update();
  sat->Update();
  itkExporter->Update();
  vtkImporter->Update();

  output->DeepCopy(vtkImporter->GetOutput());
  
  vtkImporter->Delete();
  vtkInputLevelSetExporter->Delete();
  cast->Delete();
  vtkInputExporter->Delete();

  return 1;
}


int vtkpxTissueLevelSetFilter::RunActiveContour(vtkImageData* input,vtkImageData* initial,vtkImageData* output)
{
  int dim[3]; input->GetDimensions(dim);
  int   t[3]; initial->GetDimensions(t);
  int   q[3]; output->GetDimensions(q);

  /*    fprintf(stderr,"\n\n Dimensions input (%d x %d x%d)\n",dim[0],dim[1],dim[2]);
    fprintf(stderr,"\n\n Dimensions intal (%d x %d x%d)\n",t[0],t[1],t[2]);
    fprintf(stderr,"\n\n Dimensions outpt (%d x %d x%d)\n",q[0],q[1],q[2]);*/


  if (dim[2]==1)
    return vtkpxTissueLevelSetFilter_RunActiveContour<2>(input,initial,output,this);

  return vtkpxTissueLevelSetFilter_RunActiveContour<3>(input,initial,output,this);
}
// ----------------------------------------------------------------------------
void vtkpxTissueLevelSetFilter::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (input==NULL)
    {
      vtkErrorMacro(<<"Bad Inputs to vtkpxTissueLevelSetFilter");
      return;
    }

  //  fprintf(stderr,"Beginning Simple Execute\n");

  if (this->EndLevel>this->BeginLevel)
    this->BeginLevel=this->EndLevel;

  this->NumberOfLevels=this->EndLevel-this->BeginLevel+1;
  
  vtkImageData* internal_input=vtkImageData::New();
  internal_input->ShallowCopy(input);


  vtkImageData* internal_output=vtkImageData::New();
  internal_output->CopyStructure(output);
  internal_output->AllocateScalars();

  vtkImageData* lv_input=vtkImageData::New();
  vtkImageData* lv_initial=vtkImageData::New();


  for (int level=this->BeginLevel;level>=this->EndLevel;level=level-1)
    {
      float factor=pow(2.0,level-1);

      char line[150];   
      sprintf(line,"********* Beginning Level %d of %d\n",this->NumberOfLevels-level+1,this->NumberOfLevels);
      this->SetOutputStatus(line);
      //      fprintf(stderr,"%s\n",line);
      this->UpdateProgress(0.01);

      if (level>1)
	vtkpxLevelSetUtil::ResampleAndSmoothImage(lv_input,internal_input,factor);
      else
	lv_input->ShallowCopy(internal_input);

      if (level==this->BeginLevel)
	{
	  this->SetOutputStatus("********* Creating Input Level Set");
	  this->GenerateInputLevelSet(lv_input,factor);
	  lv_initial->ShallowCopy(this->InputLevelSet);
	  this->UpdateProgress(0.01);
	}
      else
	vtkpxLevelSetUtil::ResliceDistanceMap(lv_initial,internal_output,lv_input);

      //      fprintf(stderr,"\n*********  On to Running Active Contour for Level=%d (End=%d)\n-----------------------------------------\n",level,this->EndLevel);

      this->RunActiveContour(lv_input,lv_initial,internal_output);
      //      fprintf(stderr,"********* Done Running Active Contour for Level=%d (End=%d)\n\n",level,this->EndLevel);
    }

  if (this->EndLevel!=1)
    vtkpxLevelSetUtil::ResliceDistanceMap(output,internal_output,input);
  else
    output->ShallowCopy(internal_output);
  
  internal_output->Delete();
  internal_input->Delete();
  lv_input->Delete();
  lv_initial->Delete();

}

