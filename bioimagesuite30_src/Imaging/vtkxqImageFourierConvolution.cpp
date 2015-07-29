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


#include "vtkObjectFactory.h"
#include "vtkxqImageFourierConvolution.h"
#include "vtkImageReslice.h"
#include "vtkImageExtractComponents.h"
#include "vtkImageFFT.h"
#include "vtkImageRFFT.h"
#include "vtkImageFFT.h"
#include "vtkImageRFFT.h"
#include "vtkpxUtil.h"

#include "vtkImageCast.h"
#include "vtkImageMagnitude.h"
#include "vtkImageMathematics.h"
#include "vtkPointData.h"
#include "vtkDataArray.h"
#include "vtkImageShiftScale.h"
#include "vtkMath.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataWriter.h"
#include "vtkCellArray.h"
#include "vtkSphereSource.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkjoSphereSource.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"

#include "pxisinf.h"
#include "bioimagesuite_settings.h"
// -------------------------------------------------------
// CUDA Definitions
// -------------------------------------------------------
// CUDA include files
#include "vtkbisCudaFFT.h"
#include "vtkbisCudaRFFT.h"

// -------------------------------------------------------

// Uncomment to force CPU
//#undef BIOIMAGESUITE3_USE_CUDA

//------------------------------------------------------------------------------
vtkxqImageFourierConvolution* vtkxqImageFourierConvolution::New()
{
  // First try to create the object from the vtkObjectFactory
  vtkObject* ret = vtkObjectFactory::CreateInstance("vtkxqImageFourierConvolution");
  if(ret)
    {
    return (vtkxqImageFourierConvolution*)ret;
    }
  // If the factory was unable to create the object, then create it here.
  return new vtkxqImageFourierConvolution;
}

// Construct object with no children.
vtkxqImageFourierConvolution::vtkxqImageFourierConvolution()
{
  this->Mode=1;
  this->DoublePrecision=1;
    this->FilterBank=NULL;
  this->SecondOutput=NULL;
  this->ForceCPU=0;
}
// ----------------------------------------------------------------------------
vtkxqImageFourierConvolution::~vtkxqImageFourierConvolution()
{
  this->SetFilterBank(NULL);
  if (this->SecondOutput!=NULL)
    this->SecondOutput->Delete();
}

// ----------------------------------------------------------------------------
int vtkxqImageFourierConvolution::RequestInformation(vtkInformation *vtkNotUsed(request),  vtkInformationVector **inputVector, 
						     vtkInformationVector *outputVector)

{  
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_FLOAT, -1);
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, 1);

  if (this->FilterBank==NULL)
    {
      vtkErrorMacro(<<"No Filter Bank Image Set\n");
      return 1;
    }
  int nc=this->FilterBank->GetNumberOfScalarComponents();
  vtkDataObject::SetPointDataActiveScalarInfo(outputVector->GetInformationObject(0), -1, nc);
  return 1; 
}
// -------------------------------------------------------------------------------------------------------------
vtkImageData* vtkxqImageFourierConvolution::PadImage(vtkImageData* img,int paddim[3])
{
  //  static int count=1;

  int dim[3]; img->GetDimensions(dim);
  double ori[3];  img->GetOrigin(ori);
  double spa[3];  img->GetSpacing(spa);
  if (this->Debug)
    fprintf(stderr,"\t Padding = (%d,%d,%d) ori=(%.1f,%.1f,%.1f) spa=(%.1f,%.1f,%.1f)\n",
	    dim[0],	  dim[1],	  dim[2],
	    ori[0],	  ori[1],	  ori[2],
	    spa[0],	  spa[1],	  spa[2]);

  

  vtkImageReslice* resl=vtkImageReslice::New();
  resl->SetInput(img);
  resl->SetOutputExtent(0,paddim[0]-1,0,paddim[1]-1,0,paddim[2]-1);
  resl->SetOutputOrigin(img->GetOrigin());
  resl->SetOutputSpacing(img->GetSpacing());
  resl->SetInterpolationMode(0);
  resl->SetBackgroundLevel(0.0);
  resl->Update();
  
  vtkImageData* out=vtkImageData::New();
  out->ShallowCopy(resl->GetOutput());
  resl->Delete();

  out->GetDimensions(dim);
  out->GetOrigin(ori);
  out->GetSpacing(spa);

  if (this->Debug)
    {
      fprintf(stderr,"\t Padding Done = (%d,%d,%d) ori=(%.1f,%.1f,%.1f) spa=(%.1f,%.1f,%.1f)\n",
	      dim[0],	  dim[1],	  dim[2],
	      ori[0],	  ori[1],	  ori[2],
	      spa[0],	  spa[1],	  spa[2]);
      
      
      /*      char line[100];
      sprintf(line,"padded_%d.nii.gz",count); ++count;
      vtkpxUtil::SaveAnalyze(line,out,9);
      fprintf(stderr,"padded image saved in %s\n",line);*/
    }
  return out;
}
// -------------------------------------------------------------------------------------------------------------
vtkImageData* vtkxqImageFourierConvolution::FFTImage(vtkImageData* img,int frame,int paddim[3],int dosquare)
{
  int dim[3]; img->GetDimensions(dim);
  int nc=img->GetNumberOfScalarComponents();
  if (this->Debug)
    fprintf(stderr,"Performing FFT (%d x %d x %d, %d) --> (%d x %d x %d at %d) dosquare=%d\n",
	    dim[0],dim[1],dim[2],nc,
	    paddim[0],paddim[1],paddim[2],
	    frame,dosquare);

  vtkImageCast* cast=vtkImageCast::New();
  vtkImageData* tmp=NULL;
  if (img->GetNumberOfScalarComponents()>1)
    {
      vtkImageExtractComponents* comp=vtkImageExtractComponents::New();
      comp->SetInput(img);
      comp->SetComponents(frame);
      if (this->Debug)
	fprintf(stderr,"\t\t Extracting frame=%d\n",frame);
      comp->Update();
      tmp=this->PadImage(comp->GetOutput(),paddim);
      comp->Delete();
    }
  else
    {
     tmp=this->PadImage(img,paddim);
    }


  cast->SetInput(tmp);
  tmp->Delete();
  if (this->DoublePrecision)
    cast->SetOutputScalarTypeToDouble();
  else
    cast->SetOutputScalarTypeToFloat();
  //  cast->Update();

  vtkImageMathematics* math=vtkImageMathematics::New();	
  
  
  /*
#ifndef BIOIMAGESUITE3_USE_CUDA
  fprintf(stderr,"\nUsing cpu\n");
  vtkImageFourierFilter* fft=vtkImageFFT::New();  
  fft->SetDimensionality(3);
#else
  fprintf(stderr,"\nUsing CUDA\n");
  vtkbisCudaFFT* fft=vtkbisCudaFFT::New();  
#endif
  */

  int usegpu=0;
#ifdef BIOIMAGESUITE3_USE_CUDA
  if (this->ForceCPU==0)
    usegpu=1;
#endif
  
  vtkImageAlgorithm* fft=NULL;
  if (usegpu)
    {
      if (this->Debug)
	fprintf(stderr,"\nUsing CUDA\n");
      fft=vtkbisCudaFFT::New();  
    }
  else
    {
      if (this->Debug)
	fprintf(stdout,"\nUsing cpu this->ForceCPU=%d, %d\n",this->ForceCPU,usegpu);
      vtkImageFourierFilter* fft2=vtkImageFFT::New();  
      fft2->SetDimensionality(3);
      fft=fft2;   
    }
 

  if (dosquare)
    {
      math->SetInput1(cast->GetOutput());
      math->SetOperationToSquare();
      math->Update();
      fft->SetInput(math->GetOutput());
    }
  else
    {
      fft->SetInput(cast->GetOutput());
    }
 
  fft->Update();

  vtkImageData* out=vtkImageData::New();
  out->ShallowCopy(fft->GetOutput());

  double range1[2];out->GetPointData()->GetScalars()->GetRange(range1,0);
  double range2[2];out->GetPointData()->GetScalars()->GetRange(range2,1);
  if (this->Debug)
    fprintf(stderr,"\t FFT RANGE=%f:%f\n",range1[0],range1[1],range2[0],range2[1]);

  out->GetDimensions(dim);
  if (this->Debug)
    fprintf(stderr,"\t FFT Done (%dx%dx%d,%d)\n",
	    dim[0],dim[1],dim[2],out->GetNumberOfScalarComponents());


  cast->Delete(); 
  fft->Delete();
  math->Delete();

  return out;
}
// -------------------------------------------------------------------------------------------------------------
vtkImageData* vtkxqImageFourierConvolution::DoFourierConvolution(vtkImageData* img1,vtkImageData* img2,int origdim[3],double origori[3],int dosquare)
{
  //  static int count=1;
  int dim1[3];  img1->GetDimensions(dim1);
  double ori1[3]; img1->GetOrigin(ori1);

  // Force Origin and Spacing the same -- not that it should matter
  img2->SetSpacing(img1->GetSpacing());
  img2->SetOrigin(img1->GetOrigin());

  int dim2[3]; img2->GetDimensions(dim2);
  double ori2[3]; img2->GetOrigin(ori2);

  int nc1=img1->GetNumberOfScalarComponents();
  int nc2=img2->GetNumberOfScalarComponents();
  if (this->Debug)
    fprintf(stderr,"Performing F-Conv (%d x %d x %d, %d) and (%d x %d x %d, %d) ori1=(%.1f,%.1f,%.1f), ori2=(%.1f,%.1f,%.1f)--> (%d x %d x %d ), oriout=(%.1f,%.1f,%.1f)\n",
	    dim1[0],dim1[1],dim1[2],nc1,
	    dim2[0],dim2[1],dim2[2],nc2,
	    ori1[0],ori1[1],ori1[2],
	    ori2[0],ori2[1],ori2[2],
	    origdim[0],origdim[1],origdim[2],
	    origori[0],origori[1],origori[2]);



  vtkImageMathematics* math=vtkImageMathematics::New();
  math->DebugOff();
  math-> SetInput1(img1);
  math-> SetInput2(img2);
  math->SetOperationToComplexMultiply();
  //  math->Update();

  /*#ifndef BIOIMAGESUITE3_USE_CUDA
  fprintf(stderr,"\nUsing cpu\n");
  vtkImageFourierFilter* rfft=vtkImageRFFT::New();
  rfft->SetDimensionality(3);
  rfft->DebugOff();
#else
  fprintf(stderr,"\nUsing CUDA\n");
  vtkbisCudaRFFT* rfft=vtkbisCudaRFFT::New();  
  #endif
  */
  
  int usegpu=0;
#ifdef BIOIMAGESUITE3_USE_CUDA
  if (this->ForceCPU==0)
    usegpu=1;
#endif

  vtkImageAlgorithm* rfft=NULL;
  if (usegpu)
    {
      if (this->Debug)
	fprintf(stderr,"\nUsing CUDA\n");
      rfft=vtkbisCudaRFFT::New();  
    }
  else
    {
      if (this->Debug)
	fprintf(stderr,"\nUsing cpu\n");
      vtkImageFourierFilter* rfft2=vtkImageRFFT::New();  
      rfft2->SetDimensionality(3);
      rfft=rfft2;
    }
  

  rfft->SetInput(math->GetOutput());
  math->Delete();
  rfft->Update();

  rfft->GetOutput()->GetDimensions(dim1);
  rfft->GetOutput()->GetOrigin(ori1);
  nc1=rfft->GetOutput()->GetNumberOfScalarComponents();

  if (this->Debug)
    fprintf(stderr,"\t RFFT Done (%d x %d x %d, %d) ori=(%.1f,%.1f,%.1f)\n",
	    dim1[0],dim1[1],dim1[2],nc1,
	    ori1[0],ori1[1],ori1[2]);

  vtkImageCast* cast=vtkImageCast::New();
  cast->SetInput(rfft->GetOutput());
  cast->SetOutputScalarTypeToFloat();
  rfft->Delete();


  //  vtkImageData* tmp_img1=vtkImageData::New();
  vtkImageReslice* resl=vtkImageReslice::New();
  double ori[3];  

  if (this->Mode==1 || dosquare == 1)
    {
      // Get the Magnitude
      // -----------------
      vtkImageMagnitude* magn=vtkImageMagnitude::New();
      magn->DebugOff();
      magn->SetInput(cast->GetOutput());
      magn->Update();
      resl->SetInput(magn->GetOutput());
      magn->GetOutput()->GetOrigin(ori);
      //  tmp_img1->ShallowCopy(magn->GetOutput());
      
      magn->Delete();
    }
  else
    {
      // Get the Real Part, convolution of two real images should be real
      // ----------------------------------------------------------------
      vtkImageExtractComponents* comp=vtkImageExtractComponents::New();
      comp->SetInput(cast->GetOutput());
      comp->SetComponents(0);
      comp->Update();
      resl->SetInput(comp->GetOutput());
      //    tmp_img1->ShallowCopy(comp->GetOutput());

      comp->GetOutput()->GetOrigin(ori);
      comp->Delete();
      
    }
  cast->Delete();
  resl->SetOutputExtent(0,origdim[0]-1,0,origdim[1]-1,0,origdim[2]-1);
  resl->SetOutputOrigin(origori);
  resl->SetOutputSpacing(img1->GetSpacing());
  resl->SetInterpolationMode(0);
  resl->SetBackgroundLevel(0.0);
  resl->Update();
  
  vtkImageData* out=vtkImageData::New();
  out->ShallowCopy(resl->GetOutput());
  resl->Delete();

  if (this->Debug)
    {
      fprintf(stderr,"Reslice Input Origin = (%.3f,%.3f,%.3f)\n",ori[0],ori[1],ori[2]);
      fprintf(stderr,"Reslice Output Origin = (%.3f,%.3f,%.3f)\n",origori[0],origori[1],origori[2]);
      /*
      char line[100];
      sprintf(line,"zz_uncrop_%d.nii.gz",count); 
      vtkpxUtil::SaveAnalyze(line,tmp_img1,9);
      fprintf(stderr,"uncropped image saved in %s\n",line);
      
      sprintf(line,"zz_crop_%d.nii.gz",count); 
      vtkpxUtil::SaveAnalyze(line,out,9);
      fprintf(stderr,"uncropped image saved in %s\n",line);*/
    }

  //  tmp_img1->Delete();
  //  ++count;


  out->GetDimensions(dim1);
  out->GetOrigin(ori1);
  nc1=out->GetNumberOfScalarComponents();

  double range[2];out->GetPointData()->GetScalars()->GetRange(range);

  if (this->Debug)
    fprintf(stderr,"\t Done F-Conv (%d x %d x %d, %d) ori=(%.1f,%.1f,%.1f) RANGE=%f:%f\n",
	    dim1[0],dim1[1],dim1[2],nc1,
	    ori1[0],ori1[1],ori1[2],range[0],range[1]);

  return out;
}

void vtkxqImageFourierConvolution::normalizeFilters(vtkImageData* FILTERS)
{
  vtkDataArray* filters=FILTERS->GetPointData()->GetScalars();
  double r[2]; filters->GetRange(r);
  if (r[0]<0.0)
    {
      if (this->Debug)
	fprintf(stderr,"Not normalizing, r[0]=%.2f\n",r[0]);
      return;
    }
  if (this->Debug)
    fprintf(stderr, "Normalizing filters.\n");
  

  int frames=FILTERS->GetNumberOfScalarComponents();
  for (int i=0; i<frames; i++){
    int points=filters->GetNumberOfTuples();
    double sum=0.0;
    for (int j=0; j<points; j++)
      sum+=filters->GetComponent(j, i);

    if (sum>0.0001)
      {
	for (int j=0; j<points; j++)
	  filters->SetComponent(j, i, (filters->GetComponent(j, i))/sum);
      }
  }
}


  
// -------------------------------------------------------------------------------------------------------------
void vtkxqImageFourierConvolution::SimpleExecute(vtkImageData* input,vtkImageData* output)
{
  if (this->Debug)
    fprintf(stderr, "In simple execute of FOURIER CONVOLUTION ..........................\n");
  if (input==NULL    || this->FilterBank ==NULL )
    {
      vtkErrorMacro(<<"Bad Inputs to vtkxqImageFourierConvolution");
      return;
    }

  this->normalizeFilters(this->FilterBank);

  
  int nc=this->FilterBank->GetNumberOfScalarComponents();
  int original_dim[3];    input->GetDimensions(original_dim);
  double offset_origin[3]; input->GetOrigin(offset_origin);
  double spa[3]; input->GetSpacing(spa);
  int fdim[3]; this->FilterBank->GetDimensions(fdim);
  
  if (this->SecondOutput!=NULL)
    this->SecondOutput->Delete();
  this->SecondOutput=NULL;

  if (this->Mode==2)
    {
      this->SecondOutput=vtkImageData::New();
      this->SecondOutput->CopyStructure(output);
      this->SecondOutput->SetNumberOfScalarComponents(output->GetNumberOfScalarComponents());
      this->SecondOutput->SetScalarTypeToFloat();
      this->SecondOutput->AllocateScalars();
      if (this->Debug)
	fprintf(stderr, "fb nc: %d output nc:%d secondoutput nc: %d\n", nc, output->GetNumberOfScalarComponents(), this->SecondOutput->GetNumberOfScalarComponents());
    }


  int paddim[3];
  for (int ia=0;ia<=2;ia++)
    {
      paddim[ia]=original_dim[ia]+fdim[ia]+1;
      offset_origin[ia]+=0.5*spa[ia]*double(fdim[ia]-1);
    }

  if (this->Debug)
    {
      fprintf(stderr,"Beginning Image Fourier Convolution: Mode = %d , Double Precision =%d\n",
	      this->Mode,this->DoublePrecision);
      fprintf(stderr,"Input dims = (%d,%d,%d) filter=(%d,%d,%d) nc=%d padded=(%d,%d,%d) ori=(%.1f,%.1f,%.1f)\n",
	      original_dim[0],   original_dim[1],   original_dim[2],
	      fdim[0],  fdim[1],  fdim[2],nc,
	      paddim[0],paddim[1],paddim[2],
	      offset_origin[0],offset_origin[1],offset_origin[2]);
    }

  int odim[3]; output->GetDimensions(odim);
  int onc=output->GetNumberOfScalarComponents();
  if (this->Debug)
    fprintf(stderr,"Output = (%d,%d,%d, %d) \n",
	    odim[0],odim[1],odim[2],onc);


  // Normalize Image between 0 and 1
  // --------------------------------
  double range[2]; input->GetPointData()->GetScalars()->GetRange(range);
  double sc=1.0/(range[1]-range[0]);

  if (this->Debug)
    fprintf(stderr,"Range = %f:%f, scale=%f\n",range[0],range[1],sc);
  
  vtkImageCast* cast=vtkImageCast::New();
  cast->SetInput(input);
  cast->SetOutputScalarTypeToFloat();
  cast->GetOutput()->SetDimensions(input->GetDimensions()); 
  cast->Update();

  vtkImageData* normalized=vtkImageData::New();
  if (this->Mode>=1)
    {
      vtkImageShiftScale* shiftscale=vtkImageShiftScale::New();
      shiftscale->SetInput(cast->GetOutput());
      shiftscale->SetShift(-range[0]);
      shiftscale->SetScale(sc);
      shiftscale->Update();
      normalized->ShallowCopy(shiftscale->GetOutput());
      shiftscale->Delete();
    } 
  else
    {
      cast->Update();
      normalized->ShallowCopy(cast->GetOutput());
    }
  cast->Delete();

  // --------------------------------

  vtkImageData* FI=this->FFTImage(normalized,0,paddim,0);
  vtkImageData* FI2=NULL;

  if (this->Mode==1 || this->Mode==2)
    {
      FI2=this->FFTImage(normalized,0,paddim,1);
    }
  
  vtkDataArray *out=output->GetPointData()->GetScalars();
  vtkDataArray *inp=normalized->GetPointData()->GetScalars();

  vtkDataArray *out2=NULL;
  if (this->SecondOutput!=NULL)
    out2=this->SecondOutput->GetPointData()->GetScalars();
  
  //  out->SetNumberOfComponents(output->GetNumberOfScalarComponents()); 
  
  
  //  fprintf(stderr, "outnc: %d, nc2: %d \n", out->GetNumberOfComponents(), out2->GetNumberOfComponents()); 

  int nt=out->GetNumberOfTuples();

  for (int frame=0;frame<nc;frame++)
  {
    out->FillComponent(frame,0.0);
    if (out2!=NULL)
      out2->FillComponent(frame,0.0);
  }
  
  int debugv=original_dim[0]/2+original_dim[0]*(original_dim[1]/2)+(original_dim[0]*original_dim[1])*original_dim[2]/2;

  //  if (nc>10)
  //    nc=10;

  for (int frame=0;frame<nc;frame++)
    {
      if (this->Debug)
	fprintf(stderr,"\n Beginning Filter %d/%d\n",frame+1,nc);
      this->UpdateProgress(double(frame)/double(nc));
      
      vtkImageData* Bi = FFTImage(this->FilterBank,frame,paddim,0);
      vtkImageData* IxBi=this->DoFourierConvolution(FI,Bi,original_dim,offset_origin,0);
      vtkDataArray *data=IxBi->GetPointData()->GetScalars();

      if (this->Mode==1 || this->Mode==2)
	{
	  vtkImageData* I2xBi=this->DoFourierConvolution(FI2,Bi,original_dim,offset_origin,1);
	  vtkDataArray *data2=I2xBi->GetPointData()->GetScalars();
	  
	  for (int j=0;j<nt;j++)
	    {
	      double lm=inp->GetComponent(j,0);
	      double vl=data2->GetComponent(j,0)-
		2.0*lm*data->GetComponent(j,0)+
		lm*lm;
	      if (isnan(vl) || isinf(vl))	
		vl=0.0;
	      out->SetComponent(j,frame,vl);

	      if (j==debugv && this->Debug)
		fprintf(stderr,"Image Center vals = image=%f,  I2xBi=%f, IxBI=%f out=%f\n",
			lm,
			data2->GetComponent(j,0),
			data->GetComponent(j,0),
			out->GetComponent(j,frame));
	    }
	  I2xBi->Delete();
	  if (this->Mode==2)
	    {
	      if (this->Debug)
		fprintf(stderr,"Mode=2, frame=%d\n",frame);
	      out2->CopyComponent(frame,data,0);
	    }
	}
      else
	{
	  if (this->Debug)
	    fprintf(stderr,"Mode=0, frame=%d\n",frame);
	  out->CopyComponent(frame,data,0);
	}
      //      fprintf(stderr,"Cleaning up 1\n");
      IxBi->Delete();
      Bi->Delete();
    }
  
  //  fprintf(stderr,"Cleaning up 2\n");
  FI->Delete();


  //  fprintf(stderr,"Cleaning up 3\n");
  if (FI2!=NULL)
    FI2->Delete();

  //  fprintf(stderr,"Cleaning up 4\n");
  normalized->Delete();

  //  fprintf(stderr,"Cleaning up 5\n");

  //fprintf(stderr,"nc = %d,  nc2=%d, \n", output->GetNumberOfScalarComponents(), this->SecondOutput->GetNumberOfScalarComponents());
  this->UpdateProgress(1.0);
}

// -------------------------------------------------------------------------------------------------------------
// More Fourier Filters
// -------------------------------------------------------------------------------------------------------------
int vtkxqImageFourierConvolution::SetFilterBankToGaussian(double sigma1,double sigma2,double sigma3,double radius) 
{
  double sigma[3] = { sigma1, sigma2, sigma3 };
  int dim[3];
  for (int ia=0;ia<=2;ia++)
    dim[ia]=int(radius*sigma[ia]+0.5)*2+1;

  vtkImageData* tmp=vtkxqImageFourierConvolution::Gaussian(sigma,dim);
  this->SetFilterBank(tmp);
  this->SetMode(0);
  tmp->Delete();

  return 1;

}
// -------------------------------------------------------------------------------------------------------------
int vtkxqImageFourierConvolution::SetFilterBankToGaussianDerivative(double sigma1,double sigma2,double sigma3,double radius) 
{
  double sigma[3] = { sigma1, sigma2, sigma3 };
  int dim[3];
  for (int ia=0;ia<=2;ia++)
    dim[ia]=int(radius*sigma[ia]+0.5)*2+1;

  vtkImageData* tmp=vtkxqImageFourierConvolution::GaussianDerivative(sigma,dim);
  this->SetFilterBank(tmp);
  this->SetMode(0);
  tmp->Delete();

  return 1;


}
// -------------------------------------------------------------------------------------------------------------
int vtkxqImageFourierConvolution::SetFilterBankToHessian(double sigma1,double sigma2,double sigma3,double radius)
{
  double sigma[3] = { sigma1, sigma2, sigma3 };
  int dim[3];
  for (int ia=0;ia<=2;ia++)
    dim[ia]=int(radius*sigma[ia]+0.5)*2+1;

  vtkImageData* tmp=vtkxqImageFourierConvolution::Hessian(sigma,dim);
  this->SetFilterBank(tmp);
  this->SetMode(0);
  tmp->Delete();

  return 1;
}
// -------------------------------------------------------------------------------------------------------------
int vtkxqImageFourierConvolution::SetFilterBankToGaussianDerivative(double sigma)
{
  int length=int(3.0*sigma+0.5)*2+1;
  vtkImageData* tmp=vtkxqImageFourierConvolution::GaussianDerivative(sigma,length);
  this->SetFilterBank(tmp);
  tmp->Delete();
  this->SetMode(0);
  return 1;
}

int vtkxqImageFourierConvolution::SetFilterBankToGaussian(double sigma)
{
  int length=int(3.0*sigma+0.5)*2+1;
  vtkImageData* tmp=vtkxqImageFourierConvolution::Gaussian(sigma,length);
  this->SetFilterBank(tmp);
  this->SetMode(0);

  //  vtkpxUtil::SaveAnalyze("filter.hdr",tmp,9);
  tmp->Delete();

  return 1;
}



int vtkxqImageFourierConvolution::SetFilterBankToHessian(double sigma)
{
  int length=int(3.0*sigma+0.5)*2+1;
  vtkImageData* tmp=vtkxqImageFourierConvolution::Hessian(sigma,length);
  this->SetFilterBank(tmp);
  tmp->Delete();
  this->SetMode(0);
  return 1;
}


vtkImageData* vtkxqImageFourierConvolution::GaussianDerivative(double sigma,int dim)
{
  int d[3]; double s[3];
  for (int ia=0;ia<=2;ia++)
    {
      d[ia]=dim;
      s[ia]=sigma;
    }

  return vtkxqImageFourierConvolution::GaussianDerivative(s,d);
}

vtkImageData* vtkxqImageFourierConvolution::GaussianDerivative(double sigma[3],int dim[3])
{
  vtkImageData* newimg=vtkImageData::New();
  newimg->SetScalarTypeToDouble();
  newimg->SetNumberOfScalarComponents(3);
  newimg->SetDimensions(dim);
  newimg->AllocateScalars();
  
  double midx=0.5*double(dim[0]-1);
  double midy=0.5*double(dim[1]-1);
  double midz=0.5*double(dim[2]-1);
  
  double invs2[3];
  for (int ia=0;ia<=2;ia++)
    invs2[ia]=1.0/(sigma[ia]*sigma[ia]);

  double scalefactor=1.0/(sqrt(2.0*vtkMath::Pi())*sigma[0]*sigma[1]*sigma[2]);


  double scale[3];
  for (int k=0;k<dim[2];k++)
    {
      double z=double(k)-midz;
      double z2=z*z*invs2[2];
      
      for (int j=0;j<dim[1];j++)
	{
	  double y=double(j)-midy;
	  double y2=y*y*invs2[1];
	  for (int i=0;i<dim[0];i++)
	    {
	      double x=double(i)-midx;
	      double x2=x*x*invs2[0];

	      double G=scalefactor*exp(- ( x2+y2+z2));
	      newimg->SetScalarComponentFromDouble(i,j,k,0, -x*invs2[0]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,1, -y*invs2[1]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,2, -z*invs2[2]*G);
	    }
	}
    }
	

  return newimg;
}
// ---------------------------------------------------------------------------------------
vtkImageData* vtkxqImageFourierConvolution::Hessian(double sigma,int dim)
{
  int d[3]; double s[3];
  for (int ia=0;ia<=2;ia++)
    {
      d[ia]=dim;
      s[ia]=sigma;
    }

  return vtkxqImageFourierConvolution::Hessian(s,d);
}

// -------------------------------------------------------------------------------------------------------------------------
int vtkxqImageFourierConvolution::SetFilterBankToHessian(double sigma[3])
{
  int length[3]; 
  double max_sigma = sigma[0];
  for (int ia=1; ia<=2; ia++)
    if (max_sigma < sigma[ia])
      max_sigma = sigma[ia];
  for (int ia=0; ia<=2; ia++)
    length[ia] = int(3.0*max_sigma+0.5)*2+1;
  
  vtkImageData* tmp=vtkxqImageFourierConvolution::Hessian(sigma,length);
  this->SetFilterBank(tmp);
  
  if (this->Debug)
    {
      int fdim[3]; tmp->GetDimensions(fdim);
      if (this->Debug)
	{
	  fprintf(stderr,"filterbank: \n");
	  fprintf(stderr,"filter=(%d,%d,%d); length = (%d %d %d)\n",
		  fdim[0],  fdim[1],  fdim[2],
		  length[0], length[1], length[2]);
	}
      this->FilterBank->GetDimensions(fdim);
    }
  tmp->Delete();
  return 1;
}


vtkImageData* vtkxqImageFourierConvolution::Hessian(double sigma[3],int dim[3])
{
  vtkImageData* newimg=vtkImageData::New();
  newimg->SetScalarTypeToDouble();
  newimg->SetNumberOfScalarComponents(6);
  newimg->SetDimensions(dim);
  newimg->AllocateScalars();

  
	double midx=0.5*double(dim[0]-1);
  double midy=0.5*double(dim[1]-1);
  double midz=0.5*double(dim[2]-1);
  
  double invs2[3];
  double s2[3];
  double invs4[3];
  for (int ia=0;ia<=2;ia++)
    {
      s2[ia]=sigma[ia]*sigma[ia];
      invs2[ia]=1.0/(s2[ia]);
      invs4[ia]=invs2[ia]*invs2[ia];
    }

  double scalefactor=1.0/(sqrt(2.0*vtkMath::Pi())*sqrt(2.0*vtkMath::Pi())*sqrt(2.0*vtkMath::Pi())*sigma[0]*sigma[1]*sigma[2]); // should be sqrt(2pi)^3 but just a constant...

  for (int k=0;k<dim[2];k++)
    {
      double z=double(k)-midz;
      double z2=z*z*invs2[2];
      
      for (int j=0;j<dim[1];j++)
	{
	  double y=double(j)-midy;
	  double y2=y*y*invs2[1];
	  for (int i=0;i<dim[0];i++)
	    {
	      double x=double(i)-midx;
	      double x2=x*x*invs2[0];

	      double G=scalefactor*exp(- 0.5*( x2+y2+z2));
	      newimg->SetScalarComponentFromDouble(i,j,k,0, (x*x - s2[0])*invs4[0]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,3, (y*y - s2[1])*invs4[1]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,5, (z*z - s2[2])*invs4[2]*G);

	      newimg->SetScalarComponentFromDouble(i,j,k,1, x*y * invs2[0]*invs2[1]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,2, x*z * invs2[0]*invs2[2]*G);
	      newimg->SetScalarComponentFromDouble(i,j,k,4, y*z * invs2[1]*invs2[2]*G);
	      
	    }
	}
    }
  
  return newimg;
}
// ---------------------------------------------------------------------------------------  
int vtkxqImageFourierConvolution::SetFilterBankToVessel(double anglespacing,int radius,double sigma,double sigma_r)
{
  vtkImageData* tmp=vtkxqImageFourierConvolution::qVesselFilter(anglespacing,radius,sigma,sigma_r);
  this->SetFilterBank(tmp);
  tmp->Delete();
  this->SetMode(1);
  return 1;
}

int vtkxqImageFourierConvolution::SetFilterBankToSheet(double anglespacing,double anglespacing2,int radius,double sigma,double sigma_r)
{
  vtkImageData* tmp=vtkxqImageFourierConvolution::qSheetFilter(anglespacing,anglespacing2,radius,sigma,sigma_r);
  this->SetFilterBank(tmp);
  tmp->Delete();
  this->SetMode(1);
  return 1;
}
// ---------------------------------------------------------------------------------------  


vtkPoints* vtkxqImageFourierConvolution::GenerateSpherePoints(int radius)
{
  vtkPoints* points=vtkPoints::New();
    
  // Create the initial sphere source based on the radius and COG
  vtkjoSphereSource* sphereSrc = vtkjoSphereSource::New();
  sphereSrc->SetRadius(radius);
  sphereSrc->SetCenter(0,0,0);
  sphereSrc->IcosahedralTessellationOn();
  sphereSrc->SetSubdivisionLevels(10);
  sphereSrc->Update();
  points->ShallowCopy(sphereSrc->GetOutput()->GetPoints());
  sphereSrc->Delete();

  /*if (this->Debug)
    fprintf(stderr,"here inside new gen sphere");*/
    
  return points;
}

vtkPoints* vtkxqImageFourierConvolution::GenerateSpherePoints(double& anglespacing,int halfspace,vtkFloatArray* angles)
{
  int Debug=0;
  /* When halfspace ==1  this is supposed to generate points for half a sphere
     BUG -- equator needs to be cut in half
  */
  if (Debug)
    fprintf(stderr,"Input Spacing=%f\n",anglespacing);
  int res=int(180.0/anglespacing+0.5);
  if (res<2)
    res=2;

  int numtheta=res*2;
  int numphi=res-1;
  int numpoints=(numtheta*numphi)+2;

  double pi=vtkMath::Pi();
  anglespacing=360.0/double(numtheta);
  double theta_spacing = anglespacing;
  double phi_spacing = anglespacing;
  
  if (halfspace==1)
    {
	  if ((int)fmod((float)numphi, 2.0f) == 0)
		numphi = numphi/2;
	  else
	    numphi = int(double(numphi)/2.0+0.5);
	  if (numphi<2)
		numphi=2;
	  phi_spacing = 90.0/double(numphi);
      
      numpoints=numtheta*(numphi-1)+numtheta/2+1;
    }

  if (Debug)
    fprintf(stderr,"Output numtheta=%d numphi=%d numtotal=%d new spacing=%f halfspace=%d\n",
	    numtheta,numphi,numpoints,anglespacing,halfspace);
  
  double kwdt=theta_spacing*pi/180.0;
  double kwdp=phi_spacing*pi/180.0;

  int index=0;

  vtkPoints* points=vtkPoints::New();
  points->SetNumberOfPoints(numpoints);
  points->SetPoint(0,0.0,0.0,1.0);  index++;
  if (Debug)
    fprintf(stderr,"Inserting Point %d (%.2f,%.2f,%.2f)  (theta=%.2f,phi=%.2f)\n",
	    index,0.0,0.0,1.0,0.0,0.0);

  if (angles!=NULL)
    {
      angles->SetNumberOfComponents(2);
      angles->SetNumberOfTuples(numpoints);
      angles->SetComponent(0,0,0.0);
      angles->SetComponent(0,1,0);
    }


  if (!halfspace)
  {
  for (int i_phi=1;i_phi<=numphi;i_phi++)
    {
      double phi=double(i_phi)*kwdp;
      for (int i_theta=0;i_theta<numtheta;i_theta++)
	{
	  double theta=double(i_theta)*kwdt;
	  double x=cos(theta)*sin(phi);
	  double y=sin(theta)*sin(phi);
	  double z=cos(phi);

	  if (angles!=NULL)
	    {
	      angles->SetComponent(0,0,theta);
	      angles->SetComponent(0,1,phi);
	    }

	  points->SetPoint(index,x,y,z);
	  if (Debug)
	    fprintf(stderr,"Inserting Point %d (%.2f,%.2f,%.2f)  (theta=%.2f,phi=%.2f)\n",
		    index,x,y,z,theta*180.0/pi,phi*180.0/pi);
	  ++index;
	}
    }
    points->SetPoint(index,0.0,0.0,-1.0);  
    if (angles!=NULL)
    {
      angles->SetComponent(0,0,0);
      angles->SetComponent(0,1,pi);
    }

  }
  else
  {
  for (int i_phi=1;i_phi<numphi;i_phi++)
    {
      double phi=double(i_phi)*kwdp;
      for (int i_theta=0;i_theta<numtheta;i_theta++)
	{
	  double theta=double(i_theta)*kwdt;
	  double x=cos(theta)*sin(phi);
	  double y=sin(theta)*sin(phi);
	  double z=cos(phi);

	  if (angles!=NULL)
	    {
	      angles->SetComponent(0,0,theta);
	      angles->SetComponent(0,1,phi);
	    }

	  points->SetPoint(index,x,y,z);
	  if (Debug)
	    fprintf(stderr,"Inserting Point %d (%.2f,%.2f,%.2f)  (theta=%.2f,phi=%.2f)\n",
		    index,x,y,z,theta*180.0/pi,phi*180.0/pi);
	  ++index;
	}
	}
	double phi=pi/2.0;
    for (int i_theta=0;i_theta<(numtheta/2);i_theta++)
	{
	  double theta=double(i_theta)*kwdt;
	  double x=cos(theta)*sin(phi);
	  double y=sin(theta)*sin(phi);
	  double z=cos(phi);

	  if (angles!=NULL)
	    {
	      angles->SetComponent(0,0,theta);
	      angles->SetComponent(0,1,phi);
	    }

	  points->SetPoint(index,x,y,z);
	  if (Debug)
	    fprintf(stderr,"Inserting Point %d (%.2f,%.2f,%.2f)  (theta=%.2f,phi=%.2f)\n",
		    index,x,y,z,theta*180.0/pi,phi*180.0/pi);
	  ++index;
	}
    


  }

  return points;
}    

// ---------------------------------------------------------------------------------------  
// --- should take more trivial ones (uniform sampling problem on spheres)
vtkImageData* vtkxqImageFourierConvolution::VesselFilter(double anglespacing,int radius,double sigma,double sigma_r)
{
  int Debug=0;
  int rectmode=0;
  if (sigma<0.01 || sigma_r<0.01)
    {
      rectmode=1;
      // dummy values here
      sigma=1.0;
      sigma_r=1.0;
    }
  
  int dim=radius*2+1;
  double mid[3];
  for (int ia=0;ia<=2;ia++)
    mid[ia]=0.5*double(dim-1);

  double newsp=anglespacing;

  vtkPoints* points=vtkxqImageFourierConvolution::GenerateSpherePoints(newsp);
  //vtkPoints* points=vtkxqImageFourierConvolution::GenerateSpherePoints(radius);
  int numfilters=points->GetNumberOfPoints();
  if (Debug)
    fprintf(stderr,"\n-%d-\n",numfilters);

  vtkImageData* newimg=vtkImageData::New();
  newimg->SetScalarTypeToDouble();
  newimg->SetNumberOfScalarComponents(numfilters);
  newimg->SetDimensions(dim,dim,dim);
  newimg->AllocateScalars();

  vtkDataArray* data_arr=newimg->GetPointData()->GetScalars();
  int numtuples=data_arr->GetNumberOfTuples();

  // Fill Filter
  vtkDataArray *data=newimg->GetPointData()->GetScalars();  
  for (int i=0;i<numfilters;i++)
    data->FillComponent(i,0.0);

  double pi=vtkMath::Pi();
  double kwd=newsp*pi/180.0;
  double halfkwd=kwd*0.5;

  double dradius=double(radius);
  double norm_r=1.0/(sqrt(2.0*pi)*sigma_r);
  double denom_r=2.0*sigma_r*sigma_r;

  //  sigma=sigma/(dradius*sin(kwd));

  sigma=sigma/(6.0*kwd);
  double norm  =1.0/(sqrt(2.0*pi)*sigma);
  double denom=2.0*sigma*sigma;

  if (Debug)
    {
      fprintf(stderr,"Rectmode = %d\n",rectmode);
      fprintf(stderr,"Constants (R) sigma_r=%f norm_r=%f denom_r=%f\n",
	      sigma_r,norm_r,denom_r);
      
      fprintf(stderr,"Constants (A) sigma=%f norm=%f denom=%f\n",
	      sigma,norm,denom);
    }

  vtkPoints* points2=vtkPoints::New();
  points2->Allocate(50,10);

  vtkCellArray* lines2=vtkCellArray::New();
  lines2->Allocate(50,10);

  points2->InsertNextPoint(mid);


  for (int index=0;index<numfilters;index++)
    {
      double x[3];
      points->GetPoint(index,x);
      vtkMath::Normalize(x);

      double p1[3];
      for (int ia=0;ia<=2;ia++)
	p1[ia]=radius*x[ia]+mid[ia];

      int pindex=points2->GetNumberOfPoints();
      points2->InsertNextPoint(p1);

      vtkIdType ln[2];
      ln[0]=0; ln[1]=pindex;
      lines2->InsertNextCell(2,ln);
	      
      double sumframe=0.0;
      for (int k=0;k<dim;k++)
	{
	  double p[3];
	  p[2]=double(k)-mid[2];
	  for (int j=0;j<dim;j++)
	    {
	      p[1]=double(j)-mid[1];
	      for (int i=0;i<dim;i++)
		{
		  p[0]=double(i)-mid[0];
		  
		  double r=vtkMath::Norm(p); //+0.0001;
		  double dotx=(p[0]*x[0]+p[1]*x[1]+p[2]*x[2]);
		  double g=1.0;

		  double q[3];
		  for (int ia=0;ia<=2;ia++)
		    q[ia]=p[ia]-dotx*x[ia];
		  double dist=vtkMath::Norm(q);
 
		  double normalized_r=r/dradius;
		  double normalized_d;
		  if (r>0.0)
			  normalized_d=asin(dist/r);
		  else 
			  normalized_d=0.0;

		  if (dotx<0.0)
		    normalized_d=pi-normalized_d;
			  
		  if (rectmode==0)
		    g=norm_r*exp(-pow(normalized_r,2.0)/denom_r);
		  if (normalized_r>1.0)
		    g=0.0;
		      
		  if (rectmode==0)
		    g*=norm*exp(-pow(normalized_d,2.0)/denom);
		  if (normalized_d>halfkwd)
		    g=0.0;

		  sumframe+=g;
		  newimg->SetScalarComponentFromDouble(i,j,k,index,g);
		}
	    }
	}
      
      for (int i=0;i<numtuples;i++)
	data_arr->SetComponent(i,index,data_arr->GetComponent(i,index)/sumframe);
    }



  vtkPolyData* poly=vtkPolyData::New();
  poly->SetPoints(points2); points2->Delete();
  poly->SetLines(lines2);   lines2->Delete();
  
  /*  vtkPolyDataWriter* w=vtkPolyDataWriter::New();
  w->SetInput(poly);
  w->SetFileName("ves.vtk");
  w->Update();
  w->Delete();*/
  poly->Delete();
  points->Delete();


  return newimg;
}


// ---------------------------------------------------------------------------------------  
// --- should take more trivial ones (uniform sampling problem on spheres)
vtkImageData* vtkxqImageFourierConvolution::qVesselFilter(double anglespacing,int radius,double sigma,double sigma_r)
{
  int Debug=0;
  if (Debug)
    fprintf(stderr, "In qvessel\n"); 
  int rectmode=0;
  if (sigma<0.01 || sigma_r<0.01)
    {
      rectmode=1;
      // dummy values here
      sigma=1.0;
      sigma_r=1.0;
    }
  
  int dim=radius*2+1;
  double mid[3];
  for (int ia=0;ia<=2;ia++)
    mid[ia]=0.5*double(dim-1);


  int res=int(180.0/anglespacing+0.5);
  if (res<2)
    res=2;

  int numtheta=res*2;
  int numphi=res;
  int numfilters=(numtheta*numphi);

  vtkImageData* newimg=vtkImageData::New();
  newimg->SetScalarTypeToDouble();
  newimg->SetNumberOfScalarComponents(numfilters);
  newimg->SetDimensions(dim,dim,dim);
  newimg->AllocateScalars();


  vtkDataArray* data_arr=newimg->GetPointData()->GetScalars();
  int numtuples=data_arr->GetNumberOfTuples();

  // Fill Filter
  vtkDataArray *data=newimg->GetPointData()->GetScalars();  
  for (int i=0;i<numfilters;i++)
    data->FillComponent(i,0.0);


  double pi=vtkMath::Pi();
  double kwd=anglespacing*pi/180.0;
  double halfkwd=kwd*0.5;

  double dradius=double(radius);
  double norm_r=1.0/(sqrt(2.0*pi)*sigma_r);
  double denom_r=2.0*sigma_r*sigma_r;

  //  sigma=sigma/(dradius*sin(kwd));

  sigma=sigma/(6.0*kwd);
  double norm  =1.0/(sqrt(2.0*pi)*sigma);
  double denom=2.0*sigma*sigma;


  if (Debug)
    {
      fprintf(stderr,"Rectmode = %d\n",rectmode);
      fprintf(stderr,"Constants (R) sigma_r=%f norm_r=%f denom_r=%f\n",
	      sigma_r,norm_r,denom_r);
      
      fprintf(stderr,"Constants (A) sigma=%f norm=%f denom=%f\n",
	      sigma,norm,denom);
    }

  int aindex = 0;
  for (int index=0;index<numphi;index++)
    {
      double sphi = index*kwd;
      double ephi = (index+1)*kwd;
      double mphi = sphi + halfkwd;
      for (int indt=0; indt<numtheta; indt++)
	{
	  double stht = indt*kwd; 
	  double etht = (indt+1)*kwd;
	  double mtht = stht + halfkwd;
	  
	  
	  double sumframe=0.0;
	  for (int k=0;k<dim;k++)
	    {
	      double p[3];
	      p[2]=double(k)-mid[2];
	      for (int j=0;j<dim;j++)
		{
		  p[1]=double(j)-mid[1];
		  for (int i=0;i<dim;i++)
		    {
		      p[0]=double(i)-mid[0];
		      
		      double r=vtkMath::Norm(p); //+0.0001;
		      double g=1.0;
		      
		      // another way:
		      double cs; 
		      if (r>0.0)
			cs = acos(p[2]/r);
		      else
			cs = 0.0;
		      
		      double xcs;
		      double xr = sqrt(p[0]*p[0]+p[1]*p[1]);
		      if (xr > 0.0)
			{
			  xcs = acos(p[0]/xr);
			  if (p[1]<0.0)
			    xcs = 2*pi - xcs;
			}
		      else
			xcs = 0.0;
		      
		      
		      double normalized_r=r/dradius;
		      
		      if (rectmode==0)
			g=norm_r*exp(-pow(normalized_r,2.0)/denom_r);
		      else if (normalized_r>1.0)
			g=0.0;
		      
		      if (rectmode==0)
			{
			  double normalized_d = fabs(cs-mphi);
			  double normalized_xd = fabs(xcs-mtht);
			  if (normalized_xd > fabs(xcs-mtht+2*pi))
			    normalized_xd = fabs(xcs-mtht+2*pi);
			  if (normalized_xd > fabs(xcs-mtht-2*pi))
			    normalized_xd = fabs(xcs-mtht-2*pi);
			  
			  g*=norm*exp(-(pow(normalized_d,2.0)+pow(normalized_xd,2.0))/denom);
			}
		      else
			{
			  if ((cs >= ephi) || (cs < sphi))
		    g=0.0;
			  else
			    {
			      if ((xcs >= etht) || (xcs < stht))
				g = 0.0;
			    }
			}
		      
		      sumframe+=g;
		      newimg->SetScalarComponentFromDouble(i,j,k,aindex,g);
		    }
		}
	    }
	  
	  for (int i=0;i<numtuples;i++)
	    data_arr->SetComponent(i,aindex,data_arr->GetComponent(i,aindex)/sumframe);
	  ++aindex;
	}
    }

  return newimg;
}
// ---------------------------------------------------------------------------------------  
vtkImageData* vtkxqImageFourierConvolution::SumComponents(vtkImageData* img)
{
  if (img==NULL)
    return NULL;

  vtkImageData* newimg=vtkImageData::New();
  newimg->CopyStructure(img);
  newimg->SetNumberOfScalarComponents(1);
  newimg->AllocateScalars();
  
  vtkDataArray* out=newimg->GetPointData()->GetScalars();
  vtkDataArray* inp=img->GetPointData()->GetScalars();
  int numtuples=inp->GetNumberOfTuples();
  int numframes=inp->GetNumberOfComponents();

  for (int i=0;i<numtuples;i++)
    {
      double sum=0.0;
      for (int j=0;j<numframes;j++)
	sum+=inp->GetComponent(i,j);
      out->SetComponent(i,0,sum);
    }
  return newimg;
}
// ---------------------------------------------------------------------------------------  
vtkImageData* vtkxqImageFourierConvolution::SheetFilter(double anglespacing,double anglespacing2,int radius,double sigma,double sigma_r)
{
  int Debug=0;
  int rectmode=0;
  if (sigma<0.01 || sigma_r<0.01)
    {
      rectmode=1;
      // dummy values here
      sigma=1.0;
      sigma_r=1.0;
    }

  double newsp=anglespacing;
  vtkPoints* points=vtkxqImageFourierConvolution::GenerateSpherePoints(newsp,1);

  
  //  int numrotations=int(180.0/anglespacing2+0.5);
  //  if (numrotations<1 || )
  int numrotations=4;  // quarter-diskcs
  
  int numlines=points->GetNumberOfPoints();
  int numfilters=numlines*numrotations;

  int dim=radius*2+1;

  vtkImageData* newimg=vtkImageData::New();
  newimg->SetScalarTypeToDouble();
  newimg->SetNumberOfScalarComponents(numfilters);
  newimg->SetDimensions(dim,dim,dim);
  newimg->AllocateScalars();

  vtkDataArray* data_arr=newimg->GetPointData()->GetScalars();
  int numtuples=data_arr->GetNumberOfTuples();

  if (Debug)
    fprintf(stderr,"Numlines=%d numrotations=%d numfilters=%d\n",numlines,numrotations,numfilters);
  
  double mid[3];
  for (int ia=0;ia<=2;ia++)
    mid[ia]=0.5*double(dim-1);

  // Fill Filter
  vtkDataArray *data=newimg->GetPointData()->GetScalars();  
  for (int i=0;i<numfilters;i++)
    data->FillComponent(i,0.0);

  double pi=vtkMath::Pi();
  double kwd=newsp*pi/180.0;

  double norm_r=1.0/(sqrt(2.0*pi)*sigma_r);
  double denom_r=2.0*sigma_r*sigma_r;

  sigma=sigma*0.4*sin(0.5*kwd);

  double norm  =1.0/(sqrt(2.0*pi)*sigma);
  double denom=2.0*sigma*sigma;

  if (Debug)
    {
      fprintf(stderr,"Rectmode = %d\n",rectmode);
      fprintf(stderr,"Constants (R) sigma_r=%f norm_r=%f denom_r=%f\n",
	      sigma_r,norm_r,denom_r);
      
      fprintf(stderr,"Constants (A) sigma=%f norm=%f denom=%f\n",
	      sigma,norm,denom);
    }


  vtkPoints* points2=vtkPoints::New();
  points2->Allocate(50,10);

  vtkCellArray* lines2=vtkCellArray::New();
  lines2->Allocate(50,10);

  points2->InsertNextPoint(mid);
  int index=0;
  double dradius=double(radius);

  for (int outer=0;outer<numlines;outer++)
    {
      double x[3]; points->GetPoint(outer,x);
      vtkMath::Normalize(x);
      for (int inner=0;inner<numrotations;inner++)
	{
	  double angle=pi*double(inner)/double(numrotations);
	  double y[3],z[3]; vtkMath::Perpendiculars(x,y,z,angle);

	  vtkMath::Normalize(y);
	  vtkMath::Normalize(z);
	  double p1[3],p2[3];
	  int pindex=points2->GetNumberOfPoints();
	  for (int ia=0;ia<=2;ia++)
	    {
	      p1[ia]=20.0*x[ia]+mid[ia];
	      p2[ia]=5.0*y[ia]+p1[ia];
	    }
	  points2->InsertNextPoint(p1);
	  points2->InsertNextPoint(p2);
	  vtkIdType ln[2];
	  ln[0]=0; ln[1]=pindex;
	  lines2->InsertNextCell(2,ln);
	  ln[0]=pindex+1;
	  lines2->InsertNextCell(2,ln);
	      
	  double sumframe=0.0;
	  for (int k=0;k<dim;k++)
	    {
	      double p[3];
	      p[2]=double(k)-mid[2];
	      for (int j=0;j<dim;j++)
		{
		  p[1]=double(j)-mid[1];
		  for (int i=0;i<dim;i++)
		    {
		      p[0]=double(i)-mid[0];

		      double r=vtkMath::Norm(p)+0.001;
		      double dotx=(p[0]*x[0]+p[1]*x[1]+p[2]*x[2]);
		      double g=1.0;
		      dotx=fabs(dotx);

		      if (dotx>=0.0)
			{
			  double dot=(p[0]*z[0]+p[1]*z[1]+p[2]*z[2]);
			  double normalized_r=r/dradius;
			  double normalized_d=fabs(dot)/dradius;
			  
			  if (rectmode==0)
			    g=norm_r*exp(-pow(normalized_r,2.0)/denom_r);
			  if (normalized_r>1.0)
			    g=0.0;
			  
			  if (rectmode==0)
			    g*=norm*exp(-pow(normalized_d,2.0)/denom);
			  else if (normalized_d>0.2)
			    g=0.0;
			}
		      else
			{
			  g=0.0;
			}
		      sumframe+=g;
		      newimg->SetScalarComponentFromDouble(i,j,k,index,g);
		    }
		}
	    }


	  for (int i=0;i<numtuples;i++)
	    data_arr->SetComponent(i,index,data_arr->GetComponent(i,index)/sumframe);
	  ++index;
	}
    }


  vtkPolyData* poly=vtkPolyData::New();
  poly->SetPoints(points2); points2->Delete();
  poly->SetLines(lines2);   lines2->Delete();
  
  /*  vtkPolyDataWriter* w=vtkPolyDataWriter::New();
  w->SetInput(poly);
  w->SetFileName("sheet.vtk");
  w->Update();
  w->Delete();*/
  poly->Delete();
  points->Delete();

  return newimg;
}

// ---------------------------------------------------------------------------------------  
vtkImageData* vtkxqImageFourierConvolution::qSheetFilter(double anglespacing,double anglespacing2,int radius,double sigma,double sigma_r)
{
  int Debug=0;
  int rectmode=0;
  if (sigma<0.01 || sigma_r<0.01)
    {
      rectmode=1;
      // dummy values here
      sigma=1.0;
      sigma_r=1.0;
    }

  double newsp=anglespacing;
  vtkPoints* points=vtkxqImageFourierConvolution::GenerateSpherePoints(newsp,1);

  
  //  int numrotations=int(180.0/anglespacing2+0.5);
  //  if (numrotations<1 || )
  int numrotations=4;  // quarter-diskcs
  
  int numlines=points->GetNumberOfPoints();
  int numfilters=numlines*numrotations;

  int dim=radius*2+1;

  vtkImageData* newimg=vtkImageData::New();
  newimg->SetScalarTypeToDouble();
  newimg->SetNumberOfScalarComponents(numfilters);
  newimg->SetDimensions(dim,dim,dim);
  newimg->AllocateScalars();

  vtkDataArray* data_arr=newimg->GetPointData()->GetScalars();
  int numtuples=data_arr->GetNumberOfTuples();

  if (Debug)
    fprintf(stderr,"Numlines=%d numrotations=%d numfilters=%d\n",numlines,numrotations,numfilters);
  
  double mid[3];
  for (int ia=0;ia<=2;ia++)
    mid[ia]=0.5*double(dim-1);

  // Fill Filter
  vtkDataArray *data=newimg->GetPointData()->GetScalars();  
  for (int i=0;i<numfilters;i++)
    data->FillComponent(i,0.0);

  double pi=vtkMath::Pi();
  double kwd=newsp*pi/180.0;

  double norm_r=1.0/(sqrt(2.0*pi)*sigma_r);
  double denom_r=2.0*sigma_r*sigma_r;

  sigma=sigma*0.4*sin(0.5*kwd);

  double norm  =1.0/(sqrt(2.0*pi)*sigma);
  double denom=2.0*sigma*sigma;

  if (Debug)
    {
      fprintf(stderr,"Rectmode = %d\n",rectmode);
      fprintf(stderr,"Constants (R) sigma_r=%f norm_r=%f denom_r=%f\n",
	      sigma_r,norm_r,denom_r);
      
      fprintf(stderr,"Constants (A) sigma=%f norm=%f denom=%f\n",
	      sigma,norm,denom);
    }

  int index=0;
  double dradius=double(radius);

  for (int outer=0;outer<numlines;outer++)
    {
      double x[3]; points->GetPoint(outer,x);
      vtkMath::Normalize(x);
	  
      for (int inner=0;inner<numrotations;inner++)
	{
	  double angle=2*pi*double(inner)/double(numrotations);
	  double y[3],z[3]; vtkMath::Perpendiculars(x,y,z,angle);

	  vtkMath::Normalize(y);
	  vtkMath::Normalize(z);

      //fprintf(stderr,"out%i\t x= (%f %f %f); \n\t y=(%f %f %f); \n\t z= (%f %f %f)\n",
	  //outer, x[0], x[1], x[2], y[0], y[1], y[2], z[0], z[1], z[2]);

	  double sumframe=0.0;
	  for (int k=0;k<dim;k++)
	    {
	      double p[3];
	      p[2]=double(k)-mid[2];
	      for (int j=0;j<dim;j++)
		{
		  p[1]=double(j)-mid[1];
		  for (int i=0;i<dim;i++)
		    {
		      p[0]=double(i)-mid[0];

		      double r=vtkMath::Norm(p)+0.001;
		      double dotx=(p[0]*x[0]+p[1]*x[1]+p[2]*x[2]);
			  double doty=(p[0]*y[0]+p[1]*y[1]+p[2]*y[2]);
			  double dotz=(p[0]*z[0]+p[1]*z[1]+p[2]*z[2]);
		      double g=1.0;
		      //dotx=fabs(dotx);

		      if (doty>=0.0 && dotz>=0.0)
			{
			  double normalized_r=r/dradius;
			  double normalized_d=fabs(dotx)/dradius;
			  
			  if (rectmode==0)
			    g=norm_r*exp(-pow(normalized_r,2.0)/denom_r);
			  else if (normalized_r>1.0)
			    g=0.0;
			  
			  if (rectmode==0)
			    g*=norm*exp(-pow(normalized_d,2.0)/denom);
			  else if (normalized_d>0.3)
			    g=0.0;
//      fprintf(stderr,"Rectmode = %d\n",rectmode);
//  fprintf(stderr,"Constants (R) normalized_r=%f normalized_d=%f g=%f\n",
//	  normalized_r,normalized_d,g);
			}
		      else
			{
			  g=0.0;
			}
		      sumframe+=g;
		      newimg->SetScalarComponentFromDouble(i,j,k,index,g);

		    }
		}
	    }

	  for (int i=0;i<numtuples;i++)
	    data_arr->SetComponent(i,index,data_arr->GetComponent(i,index)/sumframe);
	  ++index;
	}
    }

  points->Delete();

  return newimg;
}
// -------------------------------------------------------------------------------------------
vtkImageData* vtkxqImageFourierConvolution::Gaussian(double sigma,int dim)
{
  int d[3]; double s[3];
  for (int ia=0;ia<=2;ia++)
    {
      d[ia]=dim;
      s[ia]=sigma;
    }

  return vtkxqImageFourierConvolution::Gaussian(s,d);
}

vtkImageData* vtkxqImageFourierConvolution::Gaussian(double sigma[3],int dim[3])
{
  vtkImageData* newimg=vtkImageData::New();
  newimg->SetScalarTypeToDouble();
  newimg->SetNumberOfScalarComponents(1);
  newimg->SetDimensions(dim);
  newimg->AllocateScalars();
  
  double midx=0.5*double(dim[0]-1);
  double midy=0.5*double(dim[1]-1);
  double midz=0.5*double(dim[2]-1);
  
  double invs2[3];
  for (int ia=0;ia<=2;ia++)
    invs2[ia]=1.0/(sigma[ia]*sigma[ia]);

  double scalefactor=1.0/(sqrt(2.0*vtkMath::Pi())*sigma[0]*sigma[1]*sigma[2]);
  double sum=1.0;

  double scale[3];
  for (int k=0;k<dim[2];k++)
    {
      double z=double(k)-midz;
      double z2=z*z*invs2[2];
      
      for (int j=0;j<dim[1];j++)
	{
	  double y=double(j)-midy;
	  double y2=y*y*invs2[1];
	  for (int i=0;i<dim[0];i++)
	    {
	      double x=double(i)-midx;
	      double x2=x*x*invs2[0];

	      double G=exp(- ( x2+y2+z2));
	      sum+=G;
	      newimg->SetScalarComponentFromDouble(i,j,k,0, G);
	    }
	}
    }

  for (int k=0;k<dim[2];k++)
    for (int j=0;j<dim[1];j++)
      for (int i=0;i<dim[0];i++)
	newimg->SetScalarComponentFromDouble(i,j,k,0, newimg->GetScalarComponentAsDouble(i,j,k,0)/sum);

	
  return newimg;
}

int vtkxqImageFourierConvolution::SetFilterBankToMultiScaleBlob(double scalefactor,double sigma)
{
  vtkFloatArray* s=vtkFloatArray::New();
  s->SetNumberOfTuples(1);
  s->SetComponent(0,0,sigma);
  int ok=this->SetFilterBankToMultiScaleBlob(scalefactor,s);
  s->Delete();
  return ok;
}
  

int vtkxqImageFourierConvolution::SetFilterBankToMultiScaleBlob(double scalefactor,vtkFloatArray* sigmas)
{
  if (sigmas==NULL)
    return 0;


  int nfilters=sigmas->GetNumberOfTuples();
  double range[2]; sigmas->GetRange(range);

  int length=int(2.0*range[1]+0.5)*2+1;
  double mid=int(2.0*range[1]+0.5);

  length=3;
  mid=1.0;


  if (length<3)
    {
      length=3;
      mid=1.0;
    }

  vtkImageData* newimg=vtkImageData::New();

  newimg->SetScalarTypeToDouble();
  newimg->SetNumberOfScalarComponents(nfilters);
  newimg->SetDimensions(length,length,length);
  newimg->AllocateScalars();

  fprintf(stderr,"Image Size=%d, mid=%f\n",length,mid);



  for (int i=0;i<=2;i++)
    for (int j=0;j<=2;j++)
      for (int k=0;k<=2;k++)
	{
	  double v=0.0;
	  if (i==1 && j==1 && k==1)
	    v=1.0;
	  else if ( (i==0 || i==2) && j==1 && k==1)
	    v=-0.125;
	  else if ( (j==0 || j==2) && i==1 && k==1)
	    v=-0.125;
	  else if ( (k==0 || k==2) && j==1 && i==1)
	    v=-0.125;
	  for (int f=0;f<nfilters;f++)
	    newimg->SetScalarComponentFromDouble(i,j,k,f,v);
	}
  
  this->SetFilterBank(newimg);
  this->SetMode(0);
  newimg->Delete();
  return 1;







  for (int f=0;f<nfilters;f++)
    {
      double sigma=sigmas->GetComponent(f,0);
      double s2=sigma*sigma;
      double invs2=1.0/(s2);
      double invs4=invs2*invs2;
      double norm=scalefactor/(sqrt(2.0*vtkMath::Pi())*sqrt(2.0*vtkMath::Pi())*sqrt(2.0*vtkMath::Pi())*sigma*sigma*sigma); // should be sqrt(2pi)^3 but just a constant...

      fprintf(stderr,"sigma=%.2f f=%d/%d length=%d\n",sigma,f,nfilters,length);

      double sum=0.0;
      for (int k=0;k<length;k++)
	{
	  double z=double(k)-mid;
	  double z2=z*z*invs2;

	  for (int j=0;j<length;j++)
	    {
	      double y=double(j)-mid;
	      double y2=y*y*invs2;
	      for (int i=0;i<length;i++)
		{
		  double x=double(i)-mid;
		  double x2=x*x*invs2;
		  
		  /*		  double G=scalefactor*exp(- 0.5*( x2+y2+z2));

		  double zf=(z*z - s2);
		  double yf=(x*x - s2);
		  double xf=(y*y - s2);

		  double v=(xf+yf+zf)*invs4[0]*G;*/

		  double rsq=x*x+y*y+z*z;
		  double r=sqrt(rsq);
		  double r2=r*r*invs2;
		  double G=norm*exp(- 0.5*r2);
		  double v=(rsq - s2)*invs4*G;
		  newimg->SetScalarComponentFromDouble(i,j,k,f,v);
		  sum+=v;
		}
	    }
	}
      
      double mean=sum/(length*length*length);
      fprintf(stdout,"f=%d, siggma=%f mean=%f\n",f,sigma,mean);
      for (int k=0;k<length;k++)
	for (int j=0;j<length;j++)
	  for (int i=0;i<length;i++)
	    newimg->SetScalarComponentFromDouble(i,j,k,f,
						 newimg->GetScalarComponentAsDouble(i,j,k,f)-mean);
    }
  

  this->SetFilterBank(newimg);
  this->SetMode(0);
  newimg->Delete();



  return 1;
}

