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
  Module:    $RCSfile: vtkpxImageAddPhaseCorrection.cpp,v $
  Language:  C++
  Date:      $Date: 2002/03/08 15:11:46 $
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
#include <math.h>
#include "vtkpxImageAddPhaseCorrection.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkpxUtil.h"
#include "vtkImageFFT.h"
#include "vtkImageFourierCenter.h"
#include "vtkImageRFFT.h"
#include "vtkpxImageExtract.h"
#include "vtkExtractVOI.h"
#include "vtkDataArray.h"
#include "vtkImageMagnitude.h"
#include "vtkImageAppendComponents.h"
#include "vtkpxImagePolarToEuclidean.h"
//------------------------------------------------------------------------------
vtkpxImageAddPhaseCorrection* vtkpxImageAddPhaseCorrection::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxImageAddPhaseCorrection");
  if(ret)
    {
      return (vtkpxImageAddPhaseCorrection*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxImageAddPhaseCorrection;
}
//----------------------------------------------------------------------------
vtkpxImageAddPhaseCorrection::vtkpxImageAddPhaseCorrection()
{
  this->PhaseOffset=0.0;
  this->PhaseShift=0.0;
  this->SingleValue=1;
  this->PhaseOffsetArray=NULL;
  this->PhaseShiftArray=NULL;
}

//----------------------------------------------------------------------------
// This templated function executes the filter for any type of data.
template <class T>
static void vtkpxImageAddPhaseCorrectionExecute(vtkpxImageAddPhaseCorrection *self,
					      vtkImageData *inData, T *inPtr,
					      vtkImageData *outData, T *outPtr,
					      int outExt[6], int id)
{
  int idxX, idxY, idxZ;
  int maxC, maxX, maxY, maxZ;
  vtkIdType inIncX, inIncY, inIncZ;
  vtkIdType outIncX, outIncY, outIncZ;
  unsigned long count = 0;
  unsigned long target;
  float  Theta, R , X , Y;
  float PhaseOffset = self->GetPhaseOffset();
  float PhaseShift  = self->GetPhaseShift();

  int SingleValue=self->GetSingleValue();
  vtkFloatArray* PhaseOffsetArray=NULL;
  vtkFloatArray* PhaseShiftArray=NULL;

  if (SingleValue==0)
    {
      PhaseOffsetArray=self->GetPhaseOffsetArray();
      PhaseShiftArray=self->GetPhaseShiftArray();
    }
  
  // find the region to loop over
  maxC = inData->GetNumberOfScalarComponents();
  maxX = outExt[1] - outExt[0]; 
  maxY = outExt[3] - outExt[2]; 
  maxZ = outExt[5] - outExt[4];
  target = (unsigned long)((maxZ+1)*(maxY+1)/50.0);
  target++;


  // Get increments to march through data 
  inData->GetContinuousIncrements(outExt, inIncX, inIncY, inIncZ);
  outData->GetContinuousIncrements(outExt, outIncX, outIncY, outIncZ);

  // Loop through ouput pixels

  float addoffset=0.0;

  float t_offset=PhaseOffset;
  float t_shift=PhaseShift;

  for (idxZ = 0; idxZ <= maxZ; idxZ++)
    {
      if (SingleValue==0)
	{
	  t_offset=PhaseOffsetArray->GetComponent(idxZ+outExt[4],0);
	  t_shift=PhaseShiftArray->GetComponent(idxZ+outExt[4],0);
	  fprintf(stderr,"Slice= %d (offset(odd,even)=%.2f shift(translation)=%.2f)\n",
		  outExt[4]+idxZ,t_offset,t_shift);
	}
	

      for (idxY = 0; !self->AbortExecute && idxY <= maxY; idxY++)
	{
	  if (!id) 
	    {
	      if (!(count%target))
		{
		  self->UpdateProgress(count/(50.0*target));
		}
	      count++;
	    }

	  if ( !( (idxY + outExt[2]) & 1 ))
	    addoffset=t_offset;
	  else
	    addoffset=0.0;

	  addoffset+=t_shift*float(idxY+outExt[2]);

	  for (idxX = 0; idxX <= maxX; idxX++)
	    {
	      // Pixel operation
	      X = (float)(*inPtr);
	      Y = (float)(inPtr[1]);

	      Theta = atan2(Y, X);
	      R = sqrt(X*X + Y*Y);
	      
	      Theta +=addoffset;
	      
	      X=R*cos(Theta);
	      Y=R*sin(Theta);
	      
	      *outPtr = (T)(X);
	      outPtr[1] = (T)(Y);
	      inPtr += maxC;
	      outPtr += maxC;
	    }
	  outPtr += outIncY;
	  inPtr += inIncY;
	}
      outPtr += outIncZ;
      inPtr += inIncZ;
    }
}

//----------------------------------------------------------------------------
void vtkpxImageAddPhaseCorrection::ThreadedExecute(vtkImageData *inData, 
						 vtkImageData *outData,
						 int outExt[6], int id)
{
  void *inPtr = inData->GetScalarPointerForExtent(outExt);
  void *outPtr = outData->GetScalarPointerForExtent(outExt);
  
  vtkDebugMacro(<< "Execute: inData = " << inData 
		<< ", outData = " << outData);
  
  // this filter expects that input is the same type as output.
  if (inData->GetScalarType() != outData->GetScalarType())
    {
      vtkErrorMacro(<< "Execute: input ScalarType, " << inData->GetScalarType()
      << ", must match out ScalarType " << outData->GetScalarType());
      return;
    }
  
  // input must have at least two components
  if (inData->GetNumberOfScalarComponents() < 2)
    {
      vtkErrorMacro(<< "Execute: input does not have at least two components");
      return;
    }
  
  switch (inData->GetScalarType())
    {
      vtkTemplateMacro7(vtkpxImageAddPhaseCorrectionExecute, this, 
			inData, (VTK_TT *)(inPtr), 
			outData, (VTK_TT *)(outPtr), outExt, id);
    default:
      vtkErrorMacro(<< "Execute: Unknown ScalarType");
      return;
    }
}

void vtkpxImageAddPhaseCorrection::PrintSelf(ostream& os, vtkIndent indent)
{
  vtkImageToImageFilter::PrintSelf(os,indent);
  os << indent << "Phase Shift: " << this->PhaseShift << "\n";
  os << indent << "Phase Offset: " << this->PhaseOffset << "\n";
}

//----------------------------------------------------------------------------
float vtkpxImageAddPhaseCorrection::GetIntensitySum(vtkImageData* image,
						    int slice,float topline)
{

  if (image==NULL)
    return 0.0;

  topline=Frange(topline,0,0.5);

  int dim[3];
  image->GetDimensions(dim);

  float sum_in=0.0;
  slice=Irange(slice,0,dim[2]-1);
  
  int ja=int(topline*float(dim[1]));
  int jb=Irange(int((topline+0.5)*float(dim[1])),0,dim[1]);

  for (int i=0;i<dim[0];i+=1)
    for (int j=ja;j<jb;j+=1)
      sum_in+=fabs(image->GetScalarComponentAsDouble(i,j,slice,0));

  return sum_in;
  
}
//----------------------------------------------------------------------------
float vtkpxImageAddPhaseCorrection::ComputeCorrelationCoefficient(vtkDataArray* scal0,vtkDataArray* scal1,
								  int debug)
{
  
  int numscalars=scal0->GetNumberOfTuples();
  int numscalars2=scal1->GetNumberOfTuples();
  if (numscalars!=numscalars2)
    {
      fprintf(stderr,"Different num scalars %d %d\n",numscalars,numscalars2);
      return 0.0;
    }

  float mean[2],var[2],sum[2],sum2[2],sumprod=0.0;
  sum[0]=0.0; sum[1]=0.0; sum2[0]=0.0; sum2[1]=0.0;
  
  for (int i=0;i<numscalars;i++)
    {
      float v0=scal0->GetComponent(i,0);
      sum[0]+=v0;
      sum2[0]+=v0*v0;

      float v1=scal1->GetComponent(i,0);
      sum[1]+=v1;
      sum2[1]+=v1*v1;

      sumprod+=v0*v1;
    }
  

  for (int j=0;j<=1;j++)
    {
      mean[j]=sum[j]/float(numscalars);
      var[j] =Fmax(sum2[j]/float(numscalars)-mean[j]*mean[j],0.0001);
    }

  float covar=pow(sumprod/float(numscalars)-mean[0]*mean[1],float(2.0));
  float covar2=covar/(var[0]*var[1]);

  if (debug)
    fprintf(stderr,"NumScalars = %d (mean,var = %.1f + %.1f, %.1f + %.1f covar=%.2f, ratio=%.2f\n",
	    numscalars,mean[0],var[0],mean[1],var[1],covar,covar2);

  return covar2;
  
}
//----------------------------------------------------------------------------
float vtkpxImageAddPhaseCorrection::ComputeSimilarity(vtkImageData *img,
						      vtkImageData* ref,
						      float shift,float offset,
						      int mode,float topline,int dofft)
{
  mode=Irange(mode,0,1);

  vtkImageFFT* fft=vtkImageFFT::New();
  fft->SetDimensionality(2);
  fft->SetInput(img);
  
  vtkpxImageAddPhaseCorrection* addphase=vtkpxImageAddPhaseCorrection::New();
  if (dofft)
    addphase->SetInput(fft->GetOutput());
  else
    addphase->SetInput(img);
  addphase->SetPhaseShift(shift);
  addphase->SetPhaseOffset(offset);
	      
  vtkImageRFFT* rft=vtkImageRFFT::New();
  rft->SetDimensionality(2);
  rft->SetInput(addphase->GetOutput());

  vtkImageMagnitude* magn=vtkImageMagnitude::New();
  magn->SetInput(rft->GetOutput());
  magn->Update();

  float d=0.0;

  switch (mode)
    {
    case 1:
      d=vtkpxImageAddPhaseCorrection::GetIntensitySum(magn->GetOutput(),
						      0,topline);
      break;

    case 0:
      d=vtkpxImageAddPhaseCorrection::ComputeCorrelationCoefficient(magn->GetOutput()->GetPointData()->GetScalars(),
								    ref->GetPointData()->GetScalars(),0);
      break;
    }
							  
  magn->Delete();
  rft->Delete();
  addphase->Delete();
  fft->Delete();
	
  return d;
}
//----------------------------------------------------------------------------
float vtkpxImageAddPhaseCorrection::ComputeTopLine(vtkImageData* ref)
{

  float besttopline=0.25;
  float best=GetIntensitySum(ref,0,besttopline);


  float increment=0.1;
  int   iter=0;


  while (iter<20 && increment>0.001 && besttopline>=0.01 && besttopline<=0.49 )
    {
      int besttest=0;
      float topline=besttopline;
      
      for (int test=-1;test<=1;test+=2)
	{
	  float testline  = topline + float (test)* increment;
	  float d=GetIntensitySum(ref,0,testline);


	  if ( d > best ) 
	    {
	      besttopline=testline;
	      best=d;
	      besttest=test;
	      fprintf(stderr,"\t Testline=%6.3f (test=%d topline=%.4f) d=%.1f (best=%.1f) (incr=%.4f)\n",testline,
		      test,topline,d,best,increment);
	    }
	}

      if (besttest==0)
	increment*=0.5;
      
      ++iter;
    }
      
  besttopline=Frange(besttopline,0.0,0.499);
  fprintf(stderr,"Bestline=%6.3f  bestd=%.3f (incr=%.4f)\n\n",besttopline,best,increment);
  return besttopline;
  
}
//----------------------------------------------------------------------------
int vtkpxImageAddPhaseCorrection::OptimizeSimilarity(vtkImageData* image,
						     vtkImageData* reference,
						     vtkFloatArray* shiftScalars,
						     vtkFloatArray* offsetScalars)

{
  static float dp[4][2] = { { -1.0,0.0 }, { 1.0,0.0 }, { 0.0,1.0}, { 0.0,-1.0} };

  if (image==NULL || reference==NULL)
    return 0;

  int dim[3];
  image->GetDimensions(dim);

  vtkImageData* resl_ref=vtkImageData::New();
  vtkpxUtil::EquiReslice(resl_ref,reference,image);
  
  shiftScalars->SetNumberOfComponents(1);
  shiftScalars->SetNumberOfTuples(dim[2]);
  shiftScalars->FillComponent(0,0.0);

  offsetScalars->SetNumberOfComponents(1);
  offsetScalars->SetNumberOfTuples(dim[2]);
  offsetScalars->FillComponent(0,0.0);
  

  vtkpxImageExtract* extr=vtkpxImageExtract::New();
  extr->SetInput(image);
  extr->SetCurrentPlane(2);

  vtkpxImageExtract* extr2=vtkpxImageExtract::New();
  extr2->SetInput(resl_ref);
  extr2->SetCurrentPlane(2);

  for (int slice=0;slice<dim[2];slice++)
    {
      fprintf(stderr,"-----------------------------------\n Beginning Slice = %d\n",slice);
      extr->SetSliceNo(slice);
      extr->Update();
      
      extr2->SetSliceNo(slice);
      extr2->Update();

      float topline=ComputeTopLine(extr2->GetOutput());
      float bestshift=0.0,bestoffset=0.0;

      float best;
      int iter;
      float increment;
      
      iter=0;
      increment=M_PI/16.0;

      best=ComputeSimilarity(extr->GetOutput(),extr2->GetOutput(),
			     bestshift,bestoffset,
			     0,topline,1);

      // Step 1 Optimize Shift for correlation coefficient
      while (iter < 40 && increment> 0.001 )
	{
	  int besttest=-1;
	  float shift=bestshift;

	  for (int test=0;test<=1;test++)
	    {
	      
	      float test_shift  = shift + dp[test][0]*increment;
	      float d=ComputeSimilarity(extr->GetOutput(),extr2->GetOutput(),
					test_shift,bestoffset,
					0,topline,1);
	      if ( d > best ) 
		{
		  bestshift =test_shift;
		  best=d;
		  besttest=test;
		  fprintf(stderr,"\t Shift=%5.2f Offset=%5.2f d0=%.4f (incr=%.4f)\n",bestshift,bestoffset,best,
			  increment);
		}
	    }

	  if (besttest==-1)
	    increment*=0.5;

	  ++iter;
	}
      
      fprintf(stderr,"Stage 1 Shift=%5.2f Offset=%5.2f d0=%.4f (incr=%.4f iter=%d)\n",bestshift,bestoffset,best,
	      increment,iter);


      iter=0;
      increment=M_PI/16.0;
      best=ComputeSimilarity(extr->GetOutput(),extr2->GetOutput(),
			     bestshift,bestoffset,
			     1,topline,1);

      

      // Step 2 Optimize Shift + Offset for Sum 
      while (iter < 40 && increment> 0.001 )
	{
	  int besttest=-1;
	  float offset=bestoffset;

	  for (int test=0;test<=1;test++)
	    {
	      
	      float test_offset = offset+ dp[test][0]*increment;
	      float d=ComputeSimilarity(extr->GetOutput(),extr2->GetOutput(),
					bestshift,test_offset,
					1,topline,1);
	      if ( d > best ) 
		{
		  bestoffset=test_offset;
		  best=d;
		  besttest=test;
		  fprintf(stderr,"\t Shift=%5.2f Offset=%5.2f d0=%.4f (incr=%.4f)\n",bestshift,bestoffset,best,
			  increment);
		}
	    }

	  if (besttest==-1)
	    increment*=0.5;

	  ++iter;
	}

      fprintf(stderr,"Stage 2 Shift=%5.2f Offset=%5.2f d0=%.4f (incr=%.4f iter=%d)\n",bestshift,bestoffset,best,
	      increment,iter);

      
      best=ComputeSimilarity(extr->GetOutput(),extr2->GetOutput(),
			     bestshift,bestoffset,
			     0,topline,1);
      
      fprintf(stderr,"Final Shift=%5.2f Offset=%5.2f d0=%.4f (incr=%.4f iter=%d)\n",bestshift,bestoffset,best,
	      increment,iter);
      shiftScalars->SetComponent(slice,0,bestshift);
      offsetScalars->SetComponent(slice,0,bestoffset);
    }
  extr2->Delete();
  extr->Delete();
  resl_ref->Delete();
  return 1;
}

//----------------------------------------------------------------------------
int vtkpxImageAddPhaseCorrection::CorrectEPIPhase(vtkImageData* image,
						  vtkFloatArray* shiftScalars,
						  vtkFloatArray* offsetScalars)
{

  if (image==NULL || shiftScalars == NULL || offsetScalars ==NULL)
    return 0;

  vtkImageFFT* fft=vtkImageFFT::New();
  fft->SetDimensionality(2);
  fft->SetInput(image);
  
  vtkpxImageAddPhaseCorrection* addphase=vtkpxImageAddPhaseCorrection::New();
  addphase->SetInput(fft->GetOutput());
  addphase->SetSingleValue(0);
  addphase->SetPhaseOffsetArray(offsetScalars);
  addphase->SetPhaseShiftArray(shiftScalars);
  
  vtkImageRFFT* rfft=vtkImageRFFT::New();
  rfft->SetInput(addphase->GetOutput());
  rfft->SetDimensionality(2);
  rfft->Update();
  
  image->ShallowCopy(rfft->GetOutput());
  rfft->Delete();
  addphase->Delete();
  fft->Delete();

  return 1;
}

//----------------------------------------------------------------------------
int vtkpxImageAddPhaseCorrection::DoGhostCorrection(vtkImageData* image,float topline)
{
  int dim[3];
  image->GetDimensions(dim);

  int nc=image->GetNumberOfScalarComponents();
  fprintf(stderr,"Ghost Dimensions = %d %d %d x %d\n",dim[0],dim[1],dim[2],nc);
  

  vtkFloatArray* shiftScalars=vtkFloatArray::New();
  shiftScalars->SetNumberOfComponents(1);
  shiftScalars->SetNumberOfTuples(dim[2]);
  shiftScalars->FillComponent(0,0.0);

  vtkFloatArray* offsetScalars=vtkFloatArray::New();
  offsetScalars->SetNumberOfComponents(1);
  offsetScalars->SetNumberOfTuples(dim[2]);
  offsetScalars->FillComponent(0,0.0);
  
  vtkpxImageExtract* extr=vtkpxImageExtract::New();
  extr->SetInput(image);

  vtkpxImageExtract* extr2=vtkpxImageExtract::New();
  extr2->SetInput(image);

  vtkImageAppendComponents* comp=vtkImageAppendComponents::New();

  for (int slice=0;slice<dim[2];slice++)
    {
      fprintf(stderr,"-----------------------------------\n Beginning Slice = %d\n",slice);
      extr->SetSliceNo(slice);
      extr->SetFrame(0);
      extr->Update();
      
      extr2->SetSliceNo(slice);
      extr2->SetFrame(1);
      extr2->Update();
      
      comp->AddInput(extr->GetOutput());
      comp->AddInput(extr2->GetOutput());
      comp->Update();
      

      vtkImageData* im=comp->GetOutput();
      int d[3];	im->GetDimensions(d);
      int n=im->GetNumberOfScalarComponents();
      fprintf(stderr,"Ghost Dimensions = %d %d %d x %d\n",d[0],d[1],d[2],n);


      float bestoffset=0.0;
      int iter =0;
      float increment=M_PI/3.0;
      float best=ComputeSimilarity(comp->GetOutput(),comp->GetOutput(),
				   0.0,bestoffset,
				   1,topline,0);

      for (int kk=1;kk<=7;kk++)
	{
	  float offset=float(kk)/4.0*M_PI;
	  float b=ComputeSimilarity(comp->GetOutput(),comp->GetOutput(),
				   0.0,offset,
				   1,topline,0);
	  if (b>best)
	    {
	      best=b;
	      bestoffset=offset;
	    }
	}

      // Step 2 Optimize Shift + Offset for Sum 
      while (iter < 40 && increment> 0.001 )
	{
	  int besttest=-1;
	  float offset=bestoffset;

	  for (int test=-1;test<=1;test+=2)
	    {
	      float test_offset = offset+ float(test)*increment;
	      float da=ComputeSimilarity(comp->GetOutput(),comp->GetOutput(),
					0.0,test_offset,
					1,topline,0);
	      if ( da > best ) 
		{
		  bestoffset=test_offset;
		  best=da;
		  besttest=test;
		  fprintf(stderr,"\tOffset=%5.2f d0=%.4f (incr=%.4f)\n",bestoffset,best,increment);
		}
	    }

	  if (besttest==-1)
	    increment*=0.5;

	  ++iter;
	}

      fprintf(stderr,"Stage 2  Offset=%5.2f d0=%.4f (incr=%.4f iter=%d)\n",bestoffset,best,
	      increment,iter);

      best=ComputeSimilarity(comp->GetOutput(),comp->GetOutput(),
			     0.0,bestoffset,
			     0,topline,0);
      
      fprintf(stderr,"Final Offset=%5.2f d0=%.4f (incr=%.4f iter=%d)\n",bestoffset,best,
	      increment,iter);
      shiftScalars->SetComponent(slice,0,0.0);
      offsetScalars->SetComponent(slice,0,bestoffset);
    }

  vtkpxImageAddPhaseCorrection* addphase=vtkpxImageAddPhaseCorrection::New();
  addphase->SetInput(image);
  addphase->SetSingleValue(0);
  addphase->SetPhaseOffsetArray(offsetScalars);
  addphase->SetPhaseShiftArray(shiftScalars);
  addphase->Update();
  image->ShallowCopy(addphase->GetOutput());
  addphase->Delete();

  extr->Delete();
  extr2->Delete();
  comp->Delete();
  shiftScalars->Delete();
  offsetScalars->Delete();

  return 1;
}

int vtkpxImageAddPhaseCorrection::DoGhostCorrectionPolarComplex(vtkImageData* image,float topline)
{
  vtkpxImagePolarToEuclidean* pol=vtkpxImagePolarToEuclidean::New();
  pol->SetInput(image);
  
  vtkImageFFT* fft=vtkImageFFT::New();
  fft->SetDimensionality(2);
  fft->SetInput(pol->GetOutput());
  fft->Update();
  pol->Delete();

  vtkImageData* tmp=vtkImageData::New();
  tmp->ShallowCopy(fft->GetOutput());
  fft->Delete();

  DoGhostCorrection(tmp,topline);

  vtkImageRFFT* rfft=vtkImageRFFT::New();
  rfft->SetDimensionality(2);
  rfft->SetInput(tmp);

  vtkpxImagePolarToEuclidean* pol2=vtkpxImagePolarToEuclidean::New();
  pol2->InverseOn();
  pol2->SetInput(rfft->GetOutput());
  pol2->Update();
  
  rfft->Delete();
  image->ShallowCopy(pol2->GetOutput());
  pol2->Delete();
 
  return 1;
}

