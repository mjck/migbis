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
  Module:    $RCSfile: vtkpxLinearMotionCorrect.cpp,v $
  Language:  C++
  Date:      $Date: 2003/10/16 19:43:04 $
  Version:   $Revision: 1.2 $


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

#include "vtkObjectFactory.h"
#include "vtkTransform.h"
#include "vtkpxLinearTransform.h"
#include "vtkpxUtil.h"
#include "vtkpxRegistration.h"
#include "vtkImageExtractComponents.h"
#include "vtkbisImageReslice.h"
#include "vtkpxMath.h"
#include "vtkIdentityTransform.h"
#include "vtkpxLinearMotionCorrect.h"

#ifdef BIOIMAGESUITE_EXPERIMENTAL
#include "vtkpxImageBSplineReslice.h"
#endif

vtkpxLinearMotionCorrect* vtkpxLinearMotionCorrect::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxLinearMotionCorrect");
  if(ret)
    {
      return (vtkpxLinearMotionCorrect*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxLinearMotionCorrect;
  
}

vtkpxLinearMotionCorrect::vtkpxLinearMotionCorrect()
{
  this->BeginFrame=-1;
  this->EndFrame=-1;
  this->ReferenceFrame=-1;
  this->RegistrationParameters=vtkDoubleArray::New();
  this->SimilarityResults=vtkDoubleArray::New();
  this->ReferenceMotionImage=NULL;
  this->MotionSequence=NULL;
  this->InitialParameters=NULL;
  this->UseLastFrameForInitialization=1;
  this->InterpolationMode=3;
  this->LinearRegistration=vtkpxLinearRegistration::New();

}
//-------------------------------------------------------------------------
vtkpxLinearMotionCorrect::~vtkpxLinearMotionCorrect()
{
  this->RegistrationParameters->Delete();
  this->SimilarityResults->Delete();
  this->SetReferenceMotionImage(NULL);
  this->SetMotionSequence(NULL);
  this->SetInitialParameters(NULL);

  if (this->LinearRegistration)
    this->LinearRegistration->Delete();
}

//-------------------------------------------------------------------------
int vtkpxLinearMotionCorrect::SaveMotionParameters(vtkDoubleArray* scalars,char* fname)
{
  return vtkpxLinearMotionCorrect::SaveMotionParameters(scalars,fname,-1);
}

int vtkpxLinearMotionCorrect::SaveMotionParameters(vtkDoubleArray* scalars,char* fname,int nparam)
{
  FILE* fout=fopen(fname,"w");
  if (!fout)
    return 0;

  fprintf(fout,"#Motion Correct Parameters\n");
  int nt=scalars->GetNumberOfTuples();
  int nc=scalars->GetNumberOfComponents();
  if (nparam>0 && nparam<nc)
    nc=nparam;
  fprintf(fout,"#Frames\n#%d\n#Components\n#%d\n",nt,nc);
  fprintf(fout,"#Actual Parameters\n");

  for (int i=0;i<nt;i++)
    {
      int count=0;
      fprintf(fout,"%d\t",i);
      for (int j=0;j<nc;j++)
	{
	  fprintf(fout,"%.4f ",scalars->GetComponent(i,j));
	  ++count;
	  if (count>=6 && j!=nc-1)
	    {
	      fprintf(fout,"\n\t");
	      count=0;
	    }
	}
      fprintf(fout,"\n");
    }
  fclose(fout);
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxLinearMotionCorrect::InitializeMotionParameters(vtkDoubleArray* scalars,int numframes)
{
  if (numframes<0 || numframes>2000 || scalars==NULL)
    return 0;

  vtkpxLinearTransform* InitialTransform=vtkpxLinearTransform::New();
  InitialTransform->Identity();
  scalars->SetNumberOfComponents(InitialTransform->GetNumberOfParameters());
  scalars->SetNumberOfTuples(numframes);

  for (int i=0;i<numframes;i++)
    InitialTransform->GetParameters(scalars,i);

  InitialTransform->Delete();
  return 1;
}

//-------------------------------------------------------------------------
int vtkpxLinearMotionCorrect::LoadMotionParameters(vtkDoubleArray* scalars,char* fname)
{
  char pxtk_buffer[500];

  gzFile fin=gzsuffixopen(fname,"rb");
  if (!fin)
    return 0;
  
  gzgets(fin,pxtk_buffer,100);
 
  if (gzfindstringinstring(pxtk_buffer,"#Motion Correct Parameters")==0)
    {
      gzclose(fin);
      return 0;
    }

  int nc,nt;
  gzgets(fin,pxtk_buffer,200);   gzgets(fin,pxtk_buffer,200);  sscanf(pxtk_buffer,"#%d",&nt);
  gzgets(fin,pxtk_buffer,200);  gzgets(fin,pxtk_buffer,200);   sscanf(pxtk_buffer,"#%d",&nc);
  scalars->SetNumberOfComponents(nc);
  scalars->SetNumberOfTuples(nt);
  gzgets(fin,pxtk_buffer,200);

  for (int i=0;i<nt;i++)
    {
      int tmp,index=0;
      float ftmp[6];
      while (index<nc)
	{
	  gzgets(fin,pxtk_buffer,200);
	  int n=0;
	  if (index==0)
	    n=sscanf(pxtk_buffer,"%d %f %f %f %f %f %f",&tmp,
		     &ftmp[0],&ftmp[1],&ftmp[2],
		     &ftmp[3],&ftmp[4],&ftmp[5])-1;
	  else
	    n=sscanf(pxtk_buffer,"%f %f %f %f %f %f",
		     &ftmp[0],&ftmp[1],&ftmp[2],
		     &ftmp[3],&ftmp[4],&ftmp[5]);

	  for (int j=index;j<index+n;j++)
	    scalars->SetComponent(i,j,ftmp[j-index]);
	  index+=n;
	}
    }
  gzclose(fin);
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxLinearMotionCorrect::MotionCorrectSequence(vtkImageData* Reference,
						    vtkImageData* Output,vtkImageData* Input,
						    vtkDoubleArray* Parameters,int interp_mode)
{
  return vtkpxLinearMotionCorrect::MotionCorrectSequence(Reference,Output,Input,Parameters,interp_mode,1);
}


int vtkpxLinearMotionCorrect::MotionCorrectSequenceMask(vtkImageData* Reference,
							vtkImageData* Output,vtkImageData* Input,
							vtkDoubleArray* Parameters,int interp_mode)
{
  return vtkpxLinearMotionCorrect::MotionCorrectSequence(Reference,Output,Input,Parameters,interp_mode,1,-300,1);
}


int vtkpxLinearMotionCorrect::MotionCorrectSequence(vtkImageData* Reference,
						    vtkImageData*  Output,vtkImageData* Input,
						    vtkDoubleArray* Parameters,int interp_mode,int center,short background,short final)
{
  if (Input==NULL || Output==NULL || Parameters==NULL || Reference==NULL)
    return 0;

  if (interp_mode>=0)
    {
      if (interp_mode==2)
	interp_mode=1;
      if (interp_mode>3)
	interp_mode=3;
    }
  else
    {
      if (interp_mode==-1)
	interp_mode=-2;
      else if (interp_mode<-5)
	interp_mode=-5;
    }

  int nf=Input->GetNumberOfScalarComponents();
  if (Parameters->GetNumberOfTuples()<nf)
    {
      fprintf(stderr,"Insufficient Number Of Parameters nframes=%d ntuples=%d\n",nf,Parameters->GetNumberOfTuples());
      return 0;
    }

  if (center>0)
    {
      vtkpxRegistration::CenterImageOrigin(Reference);
      vtkpxRegistration::CenterImageOrigin(Input);
    }

  Output->SetSpacing(Reference->GetSpacing());
  Output->SetDimensions(Reference->GetDimensions());
  Output->SetOrigin(Reference->GetOrigin());
  Output->SetScalarType(Reference->GetScalarType());
  Output->SetExtent(Reference->GetExtent());
  Output->SetWholeExtent(Reference->GetWholeExtent());
  Output->SetNumberOfScalarComponents(nf);
  Output->AllocateScalars();

  vtkDataArray* outarr=Output->GetPointData()->GetScalars();
  int nt=outarr->GetNumberOfTuples();

  vtkpxLinearTransform* tr=vtkpxLinearTransform::New();

  fprintf(stderr,"\t\t Reslicing frame: %d",interp_mode);

  vtkbisImageReslice* resl=NULL;
#ifdef BIOIMAGESUITE_EXPERIMENTAL
  vtkpxImageBSplineReslice* bresl=NULL;
#endif
  int max_count=nf/10;
  int count=0;

  for (int frame=0;frame<nf;frame++)
    {
      ++count;
      if (count==max_count)
	{
	  fprintf(stderr," %d",frame+1);
	  count=0;
	}
      tr->SetParameters(Parameters,frame);
      vtkDataArray* inarr=NULL;

#ifdef BIOIMAGESUITE_EXPERIMENTAL
      if (interp_mode>=0)
	{
#endif
	  if (interp_mode<0 || interp_mode>1)
	    interp_mode=3;


	  vtkImageExtractComponents* extr=vtkImageExtractComponents::New();
	  extr->SetInput(Input);
	  extr->SetComponents(frame);
	  
	  resl=vtkbisImageReslice::New();    resl->OptimizationOff();
	  resl->SetInput(extr->GetOutput());
	  resl->SetInformationInput(Reference);
	  resl->SetResliceTransform(tr);
	  resl->SetInterpolationMode(interp_mode);
	  resl->SetBackgroundLevel(background);
	  resl->Update();
	  inarr=resl->GetOutput()->GetPointData()->GetScalars();
	  extr->Delete();
#ifdef BIOIMAGESUITE_EXPERIMENTAL
	}
      else 
	{
	  bresl=vtkpxImageBSplineReslice::New();

	  bresl->SetInput(Input);
	  bresl->SetInformationInput(Reference);
	  bresl->SetComponent(frame);
	  bresl->SetResliceTransform(tr);
	  bresl->SetSplineDegree(abs(interp_mode));
	  //bresl->SetBackgroundLevel(-1);
	  bresl->Update();
	  inarr=bresl->GetOutput()->GetPointData()->GetScalars();
	}
#endif

      outarr->CopyComponent(frame,inarr,0);
      double range[2]; outarr->GetRange(range,frame);
      //      fprintf(stderr,"(%.2f %.2f)",range[0],range[1]);

#ifdef BIOIMAGESUITE_EXPERIMENTAL
      if (resl!=NULL)
	{
#endif
	  resl->Delete();
	  resl=NULL;
#ifdef BIOIMAGESUITE_EXPERIMENTAL
	}
      else
	{

	  bresl->Delete();
	  bresl=NULL;
	}
#endif
    }

  if (final==1)
    {
      fprintf(stderr,"Clearing Image %d\n",background);
      int nt=outarr->GetNumberOfTuples();
      int nc=outarr->GetNumberOfComponents();

      for (int voxel=0;voxel<nt;voxel++)
	{
	  int ok=1;
	  int frame=0;
	  while (frame<nc && ok==1)
	    {
	      if (outarr->GetComponent(voxel,frame)==background)
		ok=0;
	      ++frame;
	    }
	  
	  if (ok==0)
	    for (int i=0;i<nc;i++)
	      outarr->SetComponent(voxel,i,0);
	}
    }

  fprintf(stderr,"\n");
  tr->Delete();
  return 1;
}
//-------------------------------------------------------------------------
int vtkpxLinearMotionCorrect::CheckInputs()
{
  if (this->ReferenceMotionImage==NULL || this->MotionSequence==NULL)
    {
      vtkErrorMacro(<<"No Reference Image or Motion Sequence Specified\n");
      return 0;
    }

  int nf=this->MotionSequence->GetNumberOfScalarComponents();
  int rf=this->ReferenceMotionImage->GetNumberOfScalarComponents();

  this->BeginFrame=Irange(this->BeginFrame,0,nf-1);

  if (this->EndFrame==-1)
    this->EndFrame=nf-1;
  else
    this->EndFrame=Irange(this->EndFrame,this->BeginFrame,nf-1);

  this->ReferenceFrame=Irange(this->ReferenceFrame,0,rf-1);
  
  return nf;
}
//-------------------------------------------------------------------------
void vtkpxLinearMotionCorrect::RunSingleFrame(vtkImageData* ref,vtkImageData* sequence,int frame,vtkpxLinearTransform* initial)
{

  //  this->LinearRegistration->DebugOn();
  this->LinearRegistration->SetReferenceImage(ref,0);
  this->LinearRegistration->SetReferenceOrientation(this->ReferenceOrientation);
  this->LinearRegistration->SetTransformImage(sequence,frame);
  this->LinearRegistration->SetTransformOrientation(this->TransformOrientation);
  this->LinearRegistration->SetInitialTransform(initial);
  this->LinearRegistration->SetReferenceWeightImage(this->ReferenceWeightImage);
  this->LinearRegistration->SetTransformWeightImage(this->TransformWeightImage);

  this->LinearRegistration->SetInterpolationMode(this->InterpolationMode);
  this->LinearRegistration->SetPositiveIntensitiesOnly(this->PositiveIntensitiesOnly);
  this->LinearRegistration->SetAutoNormalizeIntensities(this->AutoNormalizeIntensities);
  this->LinearRegistration->SetJointlyNormalizeIntensities(this->JointlyNormalizeIntensities);

  this->LinearRegistration->SetNumberOfLevels(this->NumberOfLevels);
  this->LinearRegistration->SetNumberOfBins(this->NumberOfBins);
  this->LinearRegistration->SetNumberOfIterations(this->NumberOfIterations);
  this->LinearRegistration->SetNumberOfSteps(this->NumberOfSteps);
  this->LinearRegistration->SetResolution(this->Resolution);
  this->LinearRegistration->SetBlurFactor(this->BlurFactor);
  this->LinearRegistration->SetStepSize(this->StepSize);
  this->LinearRegistration->SetSimilarityMeasure(this->SimilarityMeasure);
  this->LinearRegistration->SetOptimizationMethod(this->OptimizationMethod);
  this->LinearRegistration->SetAutoCenterOrigin(this->AutoCenterOrigin);
  this->LinearRegistration->SetTransformModeToRigid();

  this->LinearRegistration->Run();
  //  this->LinearRegistration->DebugOff();
  this->SimilarityResults->SetComponent(frame,0,this->LinearRegistration->GetLastSimilarity());
  this->LinearRegistration->GetLinearTransform()->GetParameters(this->RegistrationParameters,frame);

  if (this->UseLastFrameForInitialization)
    initial->Copy(this->LinearRegistration->GetLinearTransform());

  fprintf(stderr,"Done With Frame %d\n",frame);

  return;

}
//-------------------------------------------------------------------------
void vtkpxLinearMotionCorrect::Run()
{
  int nf=this->CheckInputs();
  if (nf==0)
    {
      vtkErrorMacro(<<"Bad Inputs");
      return;
    }

  this->AutoCenterOriginOff();
  vtkpxRegistration::CenterImageOrigin(this->ReferenceMotionImage);
  vtkpxRegistration::CenterImageOrigin(this->MotionSequence);

  this->InitialTransform=vtkpxLinearTransform::New();
  this->InitialTransform->Identity();
  if (this->InitialParameters!=NULL)
    this->InitialTransform->SetParameters(this->InitialParameters,0);
    
  int   lv=this->GetNumberOfLevels();
  this->RegistrationParameters->SetNumberOfComponents(this->InitialTransform->GetNumberOfParameters());
  this->RegistrationParameters->SetNumberOfTuples(nf);

  this->SimilarityResults->SetNumberOfComponents(1);
  this->SimilarityResults->SetNumberOfTuples(nf);
  this->SimilarityResults->FillComponent(0,0.0);

  for (int i=0;i<nf;i++)
    this->InitialTransform->GetParameters(this->RegistrationParameters,i);

  vtkDebugMacro(<< "\n Using Previous frame = " << this->UseLastFrameForInitialization );

  vtkImageData* refimg=vtkImageData::New();
  vtkImageExtractComponents* extr=vtkImageExtractComponents::New();

  extr->SetInput(this->ReferenceMotionImage);
  extr->SetComponents(this->ReferenceFrame);
  extr->Update();
  refimg->ShallowCopy(extr->GetOutput());
  extr->Delete();

  for (int frame=this->BeginFrame;frame<=this->EndFrame;frame++)
    {
      vtkDebugMacro(<<"----------------------------------------------------------------");
      vtkDebugMacro(<<"Beginning Frame "<< frame);

      fprintf(stderr,"----------------------------------------------------------------\n");
      fprintf(stderr," Beginning Frame  %d\n",frame);

      
      if (this->UseLastFrameForInitialization)
	{
	  fprintf(stderr,"Using Last Frame\n");
	  if (frame!=this->BeginFrame && this->UseLastFrameForInitialization==1)
	    {
	      if (this->NumberOfLevels>2)
		this->SetNumberOfLevels(2);
	    }
	} 
      else if (this->InitialParameters!=NULL)
	{
	  if (this->InitialParameters->GetNumberOfTuples()>=this->RegistrationParameters->GetNumberOfTuples())
	    {
	      fprintf(stderr,"\n\n*** Using True Initial Parameters -*-*\n\n");
	      this->InitialTransform->SetParameters(this->InitialParameters,frame);
	    }
	  else
	    fprintf(stderr,"Using Nothing\n");
	}
	  
      this->RunSingleFrame(refimg,this->MotionSequence,frame,this->InitialTransform);
    }
  vtkDebugMacro(<<"Done All");
  this->SetNumberOfLevels(lv);

  refimg->Delete();
}
//-------------------------------------------------------------------------
void vtkpxLinearMotionCorrect::ComputeAllSimilarityMetricsBetweenImages(vtkImageData* reference,int frame,
									vtkImageData* series,
									vtkDoubleArray* results)
{
  if (results==NULL || reference == NULL || series==NULL)
    return;

  int numframes=series->GetNumberOfScalarComponents();

  results->SetNumberOfComponents(5);
  results->SetNumberOfTuples(numframes);

  double resolution[3];
  reference->GetSpacing(resolution);
  
  float sigma=0.0;
  
  int interpolation=1;
  int numbins=64;

  // Generate Reference Image 
  vtkImageData* ref=vtkImageData::New();
  vtkImageExtractComponents* extr1=vtkImageExtractComponents::New();

  extr1->SetInput(reference);
  extr1->SetComponents(frame);
  extr1->Update();
  vtkpxRegistration::ResampleAndSmoothImage(ref,extr1->GetOutput(),resolution,sigma,1.0);
  extr1->Delete();
  short minr=0,maxr=1; 
  int num_ref=vtkpxRegistration::CalculateNumberOfBinsAndAdjustImage(ref,numbins,minr,maxr,0);

  // On to Series 
  vtkImageData* trn=vtkImageData::New();
  vtkImageExtractComponents* extr2=vtkImageExtractComponents::New();

  extr2->SetInput(series);

  vtkIdentityTransform* tr=vtkIdentityTransform::New();

  for (int fr=0;fr<numframes;fr++)
    {
      fprintf(stderr,"Computing for frame %d\n",fr);
      extr2->SetComponents(fr);
      extr2->Update();
      vtkpxRegistration::ResampleAndSmoothImage(trn,extr2->GetOutput(),
						resolution,sigma,1.0);
      short mint=0,maxt=1;
      int num_tar=vtkpxRegistration::CalculateNumberOfBinsAndAdjustImage(trn,numbins,mint,maxt,0);
      vtkpxJointHistogram* Histogram=vtkpxJointHistogram::New();
      Histogram->Allocate(num_ref,num_tar);

      vtkImageData* tmp=vtkImageData::New();
      vtkpxRegistration::ResliceImage(tmp,trn,ref,tr,interpolation);

      Histogram->FillHistogram(ref,tmp,1);
      
      for (int metric=1;metric<=5;metric++)
	{
	  double val=0.0;
	  switch (metric) 
	    {
	    case 1:
	      val= +Histogram->CrossCorrelation();
	      break;
	    case 2:
	      val= +Histogram->MutualInformation();
	      break;
	    case 3:
	      val= +Histogram->NormalizedMutualInformation();
	      break;
	    case 4:
	      val= -Histogram->SumsOfSquaredDifferences() / 
		(double)Histogram->GetNumberOfSamples();
	      break;
	    case 5:
	      val= Histogram->ComputeEntropyOfDifference();
	      break;
	    }
	  results->SetComponent(fr,metric-1,val);
	}
      tmp->Delete();
      Histogram->Delete();

    }
  tr->Delete();
  trn->Delete();
  extr2->Delete();
  ref->Delete();
  return;
}



