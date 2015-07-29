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
  Module:    $RCSfile: vtkpxAddMotionToTimeSeries.cpp,v $
  Language:  C++
  Date:      $Date: 2002/02/26 17:28:22 $
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
#include "math.h"
#include "vtkAbstractTransform.h"
#include "vtkFloatArray.h"
#include "vtkGeneralTransform.h"
#include "vtkImageExtractComponents.h"
#include "vtkImageGaussianSmooth.h"
#include "vtkbisImageReslice.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkTransform.h"
#include "vtkpxAddMotionToTimeSeries.h"
#include "vtkpxUtil.h"

//------------------------------------------------------------------------------
vtkpxAddMotionToTimeSeries* vtkpxAddMotionToTimeSeries::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkpxAddMotionToTimeSeries");
  if(ret)
    {
    return (vtkpxAddMotionToTimeSeries*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkpxAddMotionToTimeSeries;
}

// ----------------------------------------------------------------------------
vtkpxAddMotionToTimeSeries::vtkpxAddMotionToTimeSeries()
{
  this->TransformationParameters=NULL;
  this->ReferenceImage=NULL;
  this->BlockPeriod=4;
  this->UseMotionParameters=1;
  this->ObliqueTransform=NULL;
  this->SliceThickness=-1.0;
}
// ----------------------------------------------------------------------------
vtkpxAddMotionToTimeSeries::~vtkpxAddMotionToTimeSeries()
{
  if (this->TransformationParameters!=NULL)
    {
      this->TransformationParameters->UnRegister(this);
      this->TransformationParameters=NULL;
    }

  if (this->ReferenceImage!=NULL)
    {
      this->ReferenceImage->UnRegister(this);
      this->ReferenceImage=NULL;
    }

  if (this->ObliqueTransform!=NULL)
    {
      this->ObliqueTransform->UnRegister(this);
      this->ObliqueTransform=NULL;
    }


}
// --------------------------------------------------------------------------------------
void vtkpxAddMotionToTimeSeries::ExecuteInformation()
{
  vtkImageData *input=this->GetImageDataInput(0);
  vtkDataArray* scal=this->GetTransformationParameters();
  

  if (input==NULL || scal==NULL)
      {
	vtkErrorMacro(<<"No Input FMRI Data or no Input Motion Parameters!!");
	return;
      }

  vtkImageData *output=this->GetOutput();

  this->vtkpxSimpleImageToImageFilter::ExecuteInformation();
  output->SetNumberOfScalarComponents(scal->GetNumberOfTuples());

}

// --------------------------------------------------------------------------------------
// Helper Routines for Execute
// --------------------------------------------------------------------------------------
void vtkpxAddMotionToTimeSeries::ResampleAndSmoothImage(vtkImageData* image,
							vtkImageData* ref,
							float SliceThickness,
							vtkAbstractTransform* xform1,
							vtkAbstractTransform* xform2)

{
  double sp1[3],sp2[3];
  image->GetSpacing(sp1);
  ref->GetSpacing(sp2);

  // Use SliceThickness if Set!
  if (SliceThickness>0.0)
    sp2[2]=SliceThickness;

  vtkGeneralTransform* generalxform=vtkGeneralTransform::New();
  generalxform->Identity();
  generalxform->PostMultiply();
  if (xform2!=NULL)
    generalxform->Concatenate(xform2);

  if (xform1!=NULL)
    generalxform->Concatenate(xform1);


  /*float spr[3],orr[3]; int dimr[3];
  ref->GetSpacing(spr);
  ref->GetOrigin(orr);
  ref->GetDimensions(dimr);

  float p[3],q[3],q1[3],q2[3];
  for (int ib=0;ib<=2;ib++)
    p[ib]=orr[ib]+0.5*spr[ib]*dimr[ib];

  
  float bounds[6]; image->GetBounds(bounds);
  float boundsr[6]; ref->GetBounds(boundsr);
  generalxform->TransformPoint(p,q);
  if (xform2!=NULL)
    xform2->TransformPoint(p,q1);
  fprintf(stderr,"Transforming p (%.1f,%.1f,%.1f) to q(%.1f,%.1f,%.1f) q1(%.2f,%.2f,%.2f)\n\t bounds = %.1f:%.1f, %.1f:%.1f %.1f:%.1f \n\t boundsref = %.1f:%.1f, %.1f:%.1f %.1f:%.1f \n",
	  p[0],p[1],p[2],q[0],q[1],q[2],q1[0],q1[1],q1[2],
	  bounds[0],bounds[1],bounds[2],bounds[3],bounds[4],bounds[5],
	  boundsr[0],boundsr[1],boundsr[2],boundsr[3],boundsr[4],boundsr[5]);*/


  double sigma[3],radius[3],ratio[3];
  for (int ia=0;ia<=2;ia++)
    {
      ratio[ia]=sp2[ia]/sp1[ia];
      sigma[ia] =10.0;
      radius[ia]=0.5*(ratio[ia]/sigma[ia]);
    }

  



  vtkImageGaussianSmooth* smooth=vtkImageGaussianSmooth::New();
  smooth->SetInput(image);
  smooth->SetStandardDeviations(sigma);
  smooth->SetRadiusFactors(radius);

  vtkbisImageReslice* reslice=vtkbisImageReslice::New(); reslice->OptimizationOff();
  reslice->SetInput(smooth->GetOutput());
  reslice->SetInterpolationModeToCubic();
  reslice->SetBackgroundLevel(0.0) ;
  reslice->SetInformationInput(ref);
  reslice->SetResliceTransform(generalxform);
  reslice->Update();

  image->ShallowCopy(reslice->GetOutput());
  reslice->Delete();
  smooth->Delete();
  generalxform->Delete();
}

// --------------------------------------------------------------------------------------
void vtkpxAddMotionToTimeSeries::CopyComponent(vtkImageData* input,vtkImageData* output,
							 int inframe,int outframe)

{
  int dim[3];
  input->GetDimensions(dim);

  vtkDataArray* inScalars=input->GetPointData()->GetScalars();
  vtkDataArray* outScalars=output->GetPointData()->GetScalars();

  outScalars->CopyComponent(outframe,inScalars,inframe);

  /*  int nt=inScalars->GetNumberOfTuples();

  for (int k=0;k<nt;k++)
  outScalars->SetComponent(k,outframe,inScalars->GetComponent(k,inframe));*/

  
}

// --------------------------------------------------------------------------------------
void vtkpxAddMotionToTimeSeries::SetMotionTransform(vtkTransform* transform,
						    vtkFloatArray* parameters,int frame,
						    float minusshift[3],float shift[3])
{
  transform->Identity();
  transform->PostMultiply();
  transform->Translate(minusshift);
  transform->RotateX(parameters->GetComponent(frame,3));
  transform->RotateY(parameters->GetComponent(frame,4));
  transform->RotateZ(parameters->GetComponent(frame,5));
  transform->Translate(shift);
  transform->Translate(parameters->GetComponent(frame,0),
		       parameters->GetComponent(frame,1),
		       parameters->GetComponent(frame,2));
}

// --------------------------------------------------------------------------------------
//  Reslice and Create Motion Path 
// --------------------------------------------------------------------------------------

void PrintImageSpecs(vtkImageData* im,char* name)
{
  int dim[3],numc;
  im->GetDimensions(dim);
  numc=im->GetNumberOfScalarComponents();
  double sp[3],ori[3];
  im->GetOrigin(ori);
  im->GetSpacing(sp);
  fprintf(stderr,"Image %s dim= [ %d %d %d, %d ] sp =[ %.2f %.2f %.2f ] or = [ %.2f %.2f %.2f ]\n",
	  name,dim[0],dim[1],dim[2],numc,sp[0],sp[1],sp[2],ori[0],ori[1],ori[2]);
}

void vtkpxAddMotionToTimeSeries::SimpleExecute(vtkImageData* input,vtkImageData *output)
{

  if (input==NULL || this->TransformationParameters==NULL)
    {
      vtkErrorMacro(<<"No Input Image\n Exiting\n");
      return;
    }

  int inputFrames=input->GetNumberOfScalarComponents();
  int numframes=this->TransformationParameters->GetNumberOfTuples();

  if (inputFrames!=2 && inputFrames!=numframes)
    {
      vtkErrorMacro(<<"Bad Number of Frames Supplied in Input Time Series\n Exiting\n");
      return;
    }

  vtkImageData *ref=this->GetReferenceImage();
  if (ref==NULL)
    ref=input;

  output->SetSpacing(ref->GetSpacing());
  output->SetOrigin(ref->GetOrigin());
  output->SetScalarType(input->GetScalarType());
  output->SetDimensions(ref->GetDimensions());
  output->SetWholeExtent(ref->GetWholeExtent());
  output->SetNumberOfScalarComponents(numframes);
  output->AllocateScalars();

  float shift[3], minusshift[3]; double sp[3];
  int  dim[3];

  input->GetSpacing(sp);
  input->GetDimensions(dim);

  // Create Base Transformation Stuff
  // --------------------------------
  vtkTransform* transform=vtkTransform::New();
  transform->Identity();
  for (int ia=0;ia<=2;ia++)
    {
      shift[ia]=0.5*dim[ia]*sp[ia];
      minusshift[ia]=-0.5*float(dim[ia])*sp[ia];
    }
  

  // Create Sequence 
  // ---------------
  int count=0,origframe=0;

  //PrintImageSpecs(input," Input Image");
  //PrintImageSpecs(ref,"Reference Image");

  int numf=numframes;
  //  numf=2;
  
  for (int f=0;f<numf;f++)
    {
      //      fprintf(stderr,"Creating Frame %d from %d\n",f,origframe);
      vtkImageExtractComponents* extr=vtkImageExtractComponents::New();
      extr->SetComponents(origframe);
      extr->SetInput(input);
      extr->Update();

      //PrintImageSpecs(extr->GetOutput(),"Extract Component");

      vtkImageData* tmp=vtkImageData::New();
      tmp->ShallowCopy(extr->GetOutput());
      extr->Delete();

      if (this->UseMotionParameters)
	SetMotionTransform(transform,this->TransformationParameters,f,
			   minusshift,shift);

      ResampleAndSmoothImage(tmp,ref,
			     this->SliceThickness,
			     transform,this->ObliqueTransform);
      //PrintImageSpecs(tmp,"Resampled tmp");

      CopyComponent(tmp,output,0,f);

      //PrintImageSpecs(output,"Output");

      if (inputFrames==numframes)
	{
	  ++origframe;
	}
      else
	{
	  ++count;
	  if (count==this->BlockPeriod)
	    {
	      count=0;
	      origframe=1-origframe;
	    }
	}
      tmp->Delete();
      this->UpdateProgress(float(f+1)/float(numframes));
    }


  transform->Delete();
}	     


// --------------------------------------------------------------------------------------
//   Create Motion Path Stuff 
// --------------------------------------------------------------------------------------

float vtkpxAddMotionToTimeSeries::GaussianRandom()
{
  static int iset=0;
  static float gset;
  float fac,rsq,v1,v2;

  if  (iset == 0) 
    {
      do {
	v1=2.0*vtkMath::Random()-1.0;
	v2=2.0*vtkMath::Random()-1.0;
	rsq=v1*v1+v2*v2;
      } 
      while (rsq >= 1.0 || rsq == 0.0);
      
      fac=sqrt(-2.0*log(rsq)/rsq);
      gset=v1*fac;
      iset=1;
      return v2*fac;
    } 
  else
    {
      iset=0;
      return gset;
    }
}
// --------------------------------------------------------------------------------------
int vtkpxAddMotionToTimeSeries::SampleMotionPath(vtkFloatArray* param_in,vtkFloatArray* param_out,
						 int samplerate)
{
  int framestep=Irange(samplerate,1,3);

  if (framestep==1)
    {
      param_out->DeepCopy(param_in);
      return 1;
    }

  int numframes=param_in->GetNumberOfTuples();
  int outnframes;

  if (framestep==2)
    outnframes=(2*numframes)/3;
  else
    outnframes=(numframes)/3;

  int nc=param_in->GetNumberOfComponents();

  param_out->SetNumberOfComponents(nc);
  param_out->SetNumberOfTuples(outnframes);

  int fcount=0,frames=0;
  for (int outframes=0;outframes<outnframes;outframes++)
    {
      for (int ia=0;ia<nc;ia++)
	param_out->SetComponent(outframes,ia,param_in->GetComponent(frames,ia));
      
      switch (framestep) 
	{
	case 2:
	  ++fcount;
	  ++frames;
	  if (fcount==2)
	    {
	      ++frames;
	      fcount=0;
	    }
	  break;
	case 3:
	  frames+=3;
	  break;
	}
    }
  return outnframes;
}
// --------------------------------------------------------------------------------------
int vtkpxAddMotionToTimeSeries::CreateMotionPath(vtkFloatArray* newScalars,int numframes,
						 float rotation,float translation,int orientation,
						 float noise1,float noise2)
{
  newScalars->SetNumberOfComponents(6);
  newScalars->SetNumberOfTuples(numframes);

  int rot=3,trans=0;

  switch (orientation)
    {
    case 0: // Axial 
      rot=3;
      trans=2;
      break;
    case 1: // Coronal
      rot=3;
      trans=1;
      break;
    case 2: // Sagittal 
      rot=5;
      trans=1;
      break;
    }


  float p[6];
  for (int ia=0;ia<=5;ia++)
    p[ia]=0.0;
  

  for (int frames=0;frames<numframes;frames++)
    {
      float fraction=float(frames)/float(numframes-1);
      float rot_v=rotation*fraction;
      float trans_v=translation*fraction;

      for (int i=0;i<=5;i++)
	{ 
	  if (i!=rot && i!=trans )
	    p[i]=GaussianRandom()*noise1+p[i];
	  else
	    p[i]=GaussianRandom()*noise2;
	}
      
      // Fix ty and rx
      p[rot]+=rot_v;
      p[trans]+=trans_v;

      for (int ib=0;ib<=5;ib++)
	newScalars->SetComponent(frames,ib,p[ib]);
    }

  for (int ib=0;ib<=5;ib++)
    newScalars->SetComponent(0,ib,0.0);

  return 1;
}
 
// --------------------------------------------------------------------------------------
int vtkpxAddMotionToTimeSeries::CreateMotionPath(vtkFloatArray* newScalars,int numframes,
						 vtkFloatArray* params)

{
  if (params->GetNumberOfComponents()!=2 && params->GetNumberOfTuples()!=6)
    return 0;

  newScalars->SetNumberOfComponents(6);
  newScalars->SetNumberOfTuples(numframes);

  float p[6],mean[6],noise[6];
  for (int ia=0;ia<=5;ia++)
    {
      p[ia]=0.0;
      mean[ia]=params->GetComponent(ia,0);
      noise[ia]=params->GetComponent(ia,1);
      newScalars->FillComponent(ia,0.0);
    }
  
  
  for (int frames=1;frames<numframes;frames++)
    {
      float fraction=float(frames)/float(numframes-1);
      for (int i=0;i<=5;i++)
	p[i]+=GaussianRandom()*noise[i]+fraction*mean[i];

      newScalars->SetTuple(frames,p);
    }

  return 1;
}
 
// --------------------------------------------------------------------------------------
//   Transformation Parameter Comparison Stuff 
// --------------------------------------------------------------------------------------

int vtkpxAddMotionToTimeSeries::MultiplyTransformationParameters(int beginp,int endp,
							       float factor,vtkFloatArray* scal)
{

  if (scal==NULL)
    return 0;

  int nframes=scal->GetNumberOfTuples();
  for (int param=beginp;param<=endp;param++)
    {
      for (int frames=0;frames<nframes;frames++)
	{
	  float v=scal->GetComponent(frames,param);
	  scal->SetComponent(frames,param,factor*v);
	}
    }
  return endp-beginp+1;
}
// --------------------------------------------------------------------------------------
int vtkpxAddMotionToTimeSeries::CompareTransformationParameters(vtkFloatArray* scal1,vtkFloatArray* scal2,
							      vtkFloatArray *output)
{
  if (scal1==NULL || scal2==NULL || output==NULL)
    return 0;


  int n1=scal1->GetNumberOfTuples();
  int nc1=scal1->GetNumberOfComponents();
  int n2=scal2->GetNumberOfTuples();
  int nc2=scal2->GetNumberOfComponents();

  if (n1!=n2 || nc2!=nc1)
    {
      output->SetNumberOfTuples(0);
      return 0;
    }

  int nframes=n1;
  int numparam=nc1;

  output->SetNumberOfComponents(8);
  output->SetNumberOfTuples(numparam); // Mean, std ,correlation ratio, mean1 , std1, mean2, std2 ,  maxdiff

  for (int param=0;param<numparam;param++)
    {
      float mean[3],var[3],sum[3],sum2[3],sumprod=0.0,maxd=0.0;
      for (int ia=0;ia<=2;ia++)
	{
	  sum[ia]=0.0;
	  sum2[ia]=0.0;
	}
  
      for (int frames=0;frames<nframes;frames++)
	{
	  float v0=scal1->GetComponent(frames,param);
	  sum[0]+=v0;
	  sum2[0]+=v0*v0;
	  
	  float v1=scal2->GetComponent(frames,param);
	  sum[1]+=v1;
	  sum2[1]+=v1*v1;
	  
	  sumprod+=v0*v1;

	  float d=fabs(v0-v1);

	  sum[2]+=d;
	  sum2[2]+=d*d;

	  if (d>maxd)
	    maxd=d;
	}
      
      
      for (int j=0;j<=2;j++)
	{
	  mean[j]=sum[j]/float(nframes);
	  var[j] =Fmax(sum2[j]/float(nframes)-mean[j]*mean[j],0.0001);
	}

      float covar=pow(sumprod/float(nframes)-mean[0]*mean[1],float(2.0));
      float covar2=covar/(var[0]*var[1]);
      
      float std=sqrt(var[2]);

      output->SetComponent(param,0,mean[2]);
      output->SetComponent(param,1,std);
      output->SetComponent(param,2,covar2);
      output->SetComponent(param,3,mean[0]);
      output->SetComponent(param,4,sqrt(var[0]));
      output->SetComponent(param,5,mean[1]);
      output->SetComponent(param,6,sqrt(var[1]));
      output->SetComponent(param,7,maxd);

      /*fprintf(stderr,"Meand = %4.2f +- %4.2f  r2=%4.2f means=(%4.2f +- %4.2f ,%4.2f +- %4.2f ) maxd=%4.2f \n",
	mean[2],std,covar2,mean[0],sqrt(var[0]),mean[1],sqrt(var[1]),maxd);*/
    }
  
  return numparam;
}



